#!/usr/bin/env python3
"""Driver, module and tunable tables from the kernel configuration files of one release.

Usage: tools/sysgen_tables.py [--release 6.5.30] [--out evidence/inventory]

Run from the repository root with root/ populated, inside the toolchain container
(retools/tools/re-run python3 tools/sysgen_tables.py); the symbol check runs nm from that container.

Input, per machine directory root/<release>/var/sysgen.IPnn/ (IP27, IP30, IP35):
  master.d/<module>   one driver configuration file per module. The first data line is
                      FLAGS PREFIX SOFT #DEV DEPENDENCIES; the stub lines follow until a
                      line starting with `$`, and the C section after that is not read.
  mtune/<module>      tunables: a `name: class` header opens a block (class `static` or `run`),
                      then `name default [minimum maximum] [flags]` lines.
  system/*.sm         lboot directives. USE, INCLUDE and `VECTOR: module=` name a module as
                      configured for that machine; EXCLUDE removes it again. The order of
                      directives across files is not modelled: an EXCLUDE anywhere wins.

Outputs (both tab-separated, deterministic):
  sysgen-drivers.tsv   one row per master.d module: flags, prefix, external major numbers,
                       device count, dependencies, stub count, the machines that configure it,
                       the kernel symbol check of the prefix and the grade from
                       tools/config/sysgen-roles.json (a role and a citation make a row understood).
  sysgen-tunables.tsv  one row per tunable line of the mtune files: block, class, name, default,
                       minimum, maximum, the form the line used and the machines that configure
                       its module.

The per-machine master.d and mtune trees of a release are compared by content before a run, and
the result is printed. A release that has the single var/sysgen layout (no sysgen.IPnn
directories) is refused: its machine split has not been checked.
"""
import argparse
import collections
import hashlib
import json
import os
import re
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MACHINES = ('IP27', 'IP30', 'IP35')
NM = 'mips64-linux-gnuabi64-nm'

FLAG_MEANING = {
    'k': 'kernel object, one per kernel', 'j': 'file system type', 'f': 'framework or stream device',
    'm': 'stream module', 'o': 'one device specification only', 't': 'creates a tty prefix in the character device table',
    'r': 'required device', 'b': 'block device', 'c': 'character device', 's': 'software driver',
    'x': 'configurable object, not a driver', 'n': 'semaphored', 'p': 'unsemaphored, master processor only',
    'u': 'stub module, loaded after all others', 'w': 'performs write-back before the strategy routine',
    'd': 'dynamically loadable', 'R': 'auto-registering loadable module', 'N': 'never auto-unloaded',
    'D': 'loaded then unloaded',
}


def sysgen_dirs(release):
    base = os.path.join(REPO, 'root', release, 'var')
    dirs = {m: os.path.join(base, 'sysgen.' + m) for m in MACHINES}
    missing = [m for m, d in dirs.items() if not os.path.isdir(d)]
    if missing:
        sys.exit('root/%s/var has no sysgen directory for %s; the single var/sysgen layout is not read here'
                 % (release, ', '.join(missing)))
    return dirs


def tree_digest(path):
    h = hashlib.sha256()
    for name in sorted(os.listdir(path)):
        h.update(name.encode() + b'\0')
        with open(os.path.join(path, name), 'rb') as f:
            h.update(hashlib.sha256(f.read()).digest())
    return h.hexdigest()[:12]


def parse_master(path):
    """Return (data fields, stub count, data line number) of one master.d file."""
    fields, stubs, line_no = None, 0, 0
    with open(path, encoding='latin-1') as f:
        for n, raw in enumerate(f, 1):
            line = raw.rstrip('\n')
            if line.startswith('$'):
                break
            if not line.strip() or line.lstrip().startswith('*'):
                continue
            if fields is None:
                toks = line.split()
                # A data line starts in column 1 with a flag field made of legend letters or '-'.
                # Other non-comment lines (segment directives in the element-spec files) are not one.
                if raw[0] in ' \t' or not toks or not all(c in FLAG_MEANING or c == '-' for c in toks[0]):
                    continue
                line_no = n
                fields = {
                    'flags': '' if toks[0] == '-' else toks[0],
                    'prefix': toks[1] if len(toks) > 1 and toks[1] != '-' else '',
                    'soft': toks[2] if len(toks) > 2 else '-',
                    'devices': toks[3] if len(toks) > 3 else '-',
                    'deps': ' '.join(toks[4:]),
                }
                continue
            if re.match(r'\s*\w+\s*\(\s*\)\s*\{', line):
                stubs += 1
    has_data = fields is not None
    if fields is None:
        # Some masters are retired: the data line is commented out or absent, and the file only stubs routines.
        fields = {'flags': '', 'prefix': '', 'soft': '-', 'devices': '-', 'deps': ''}
    deps = [d.strip() for d in fields['deps'].split(',') if d.strip()]
    fields['dep_list'] = deps
    fields['has_data'] = has_data
    return fields, stubs, line_no


def closure(names, deps):
    """Add every module a configured module depends on, the way lboot pulls in dependencies."""
    out, todo = set(), list(names)
    while todo:
        m = todo.pop()
        if m in out:
            continue
        out.add(m)
        todo.extend(deps.get(m, []))
    return out


def parse_system(sysdir):
    used, excluded = set(), set()
    for name in sorted(os.listdir(sysdir)):
        if not name.endswith('.sm'):
            continue
        with open(os.path.join(sysdir, name), encoding='latin-1') as f:
            for raw in f:
                line = raw.strip()
                if not line or line.startswith('*'):
                    continue
                m = re.match(r'(USE|INCLUDE|EXCLUDE|VECTOR)\s*:\s*(.*)$', line)
                if not m:
                    continue
                kind, rest = m.group(1), m.group(2)
                if kind == 'VECTOR':
                    names = re.findall(r'module\s*=\s*(\w+)', rest)
                else:
                    names = re.findall(r'[\w]+', rest)
                if kind == 'EXCLUDE':
                    excluded.update(names)
                else:
                    used.update(names)
    return used - excluded


def kernel_symbols(path):
    out = subprocess.run([NM, '--defined-only', path], capture_output=True, text=True, check=True).stdout
    names = []
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 3:
            names.append(parts[2])
    return names


def prefix_check(symbols, names, prefix, flags):
    """Count kernel symbols under a driver's prefix and check the character or block switch table it implies."""
    if not prefix:
        return '-'
    count = sum(1 for s in symbols if s.startswith(prefix))
    notes = []
    for kind in ('c', 'b'):
        if kind in flags:
            table = prefix + kind + 'devsw'
            notes.append('%sdevsw:%s' % (kind, 'yes' if table in names else 'no'))
    return '%d%s' % (count, (' ' + ' '.join(notes)) if notes else '')


def parse_mtune(path):
    rows, block, cls = [], '', ''
    with open(path, encoding='latin-1') as f:
        for n, raw in enumerate(f, 1):
            line = raw.strip()
            if not line or line.startswith('*'):
                continue
            m = re.fullmatch(r'(\w+):\s+(static|run)', line)
            if m:
                block, cls = m.group(1), m.group(2)
                continue
            toks = line.split()
            name = toks[0]
            cond = ''
            if ',' in name:
                name, cond = name.split(',', 1)
            vals = toks[1:]
            # Only the three-value form names a minimum and a maximum. A two-value line is kept
            # as written in `raw`: its second value could be a bound or something else.
            if len(vals) >= 3:
                default, lo, hi = vals[0], vals[1], vals[2]
                form = 'bounded'
            elif len(vals) == 2:
                default, lo, hi = vals[0], '', ''
                form = 'two-values'
            elif len(vals) == 1:
                default, lo, hi = vals[0], '', ''
                form = 'default'
            else:
                default, lo, hi, form = '', '', '', 'empty'
            if len(vals) >= 4 and vals[3] not in ('LL', 'll'):
                form = 'extra'
            rows.append({'block': block, 'class': cls, 'name': name, 'cond': cond,
                         'default': default, 'min': lo, 'max': hi, 'form': form, 'line': n,
                         'raw': ' '.join(vals)})
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--release', default='6.5.30')
    ap.add_argument('--out', default=os.path.join(REPO, 'evidence', 'inventory'))
    args = ap.parse_args()
    rel = args.release
    dirs = sysgen_dirs(rel)

    digests = {m: (tree_digest(os.path.join(d, 'master.d')), tree_digest(os.path.join(d, 'mtune')))
               for m, d in dirs.items()}
    same_master = len({v[0] for v in digests.values()}) == 1
    same_mtune = len({v[1] for v in digests.values()}) == 1
    print('master.d identical across machines: %s; mtune identical: %s' % (same_master, same_mtune))

    master_dir = os.path.join(dirs['IP27'], 'master.d')
    modules = sorted(os.listdir(master_dir))
    per_module = {}
    for mod in modules:
        if mod == 'README':
            continue
        per_module[mod] = parse_master(os.path.join(master_dir, mod))
    deps = {m: v[0]['dep_list'] for m, v in per_module.items()}
    configured = {}
    for m, d in dirs.items():
        configured[m] = closure(parse_system(os.path.join(d, 'system')), deps)

    kernel = os.path.join(REPO, 'root', rel, 'unix.IP27')
    symbols = kernel_symbols(kernel) if os.path.exists(kernel) else []
    kernel_names = set(symbols)

    roles_path = os.path.join(REPO, 'tools', 'config', 'sysgen-roles.json')
    roles = json.load(open(roles_path)) if os.path.exists(roles_path) else {}

    drv_rows = []
    for mod, (f, stubs, line_no) in per_module.items():
        machines = [m for m in MACHINES if mod in configured[m]]
        flags = f['flags']
        unknown = [c for c in flags if c not in FLAG_MEANING]
        if not f['has_data']:
            meaning = 'no active data line'
        else:
            meaning = ' '.join(FLAG_MEANING.get(c, '?') for c in flags) if not unknown else 'unknown flag'
        deps_missing = [d for d in f['dep_list'] if d not in per_module]
        in_kernel = prefix_check(symbols, kernel_names, f['prefix'], flags) if symbols else 'unchecked'
        role = roles.get(mod, {})
        grade = role.get('grade', '')
        status = 'understood' if role.get('role') and role.get('citation') and grade in ('established', 'inferred') else 'located'
        drv_rows.append([
            rel, mod, flags, meaning,
            f['prefix'] or '-', f['soft'], f['devices'], f['deps'] or '-',
            ','.join(deps_missing) or '-', stubs,
            *['yes' if m in machines else 'no' for m in MACHINES],
            in_kernel, status, grade,
            role.get('role', ''), role.get('citation', ''),
            'master.d/%s:%d' % (mod, line_no),
        ])

    tune_rows = []
    for mod in sorted(os.listdir(os.path.join(dirs['IP27'], 'mtune'))):
        if mod == 'README':
            continue
        machines = [m for m in MACHINES if mod in configured[m]]
        for r in parse_mtune(os.path.join(dirs['IP27'], 'mtune', mod)):
            tune_rows.append([
                rel, mod, r['block'], r['class'], r['name'], r['cond'] or '-', r['default'],
                r['min'] or '-', r['max'] or '-', r['form'], r['raw'] or '-',
                *['yes' if m in machines else 'no' for m in MACHINES],
                'located', 'mtune/%s:%d' % (mod, r['line']),
            ])

    os.makedirs(args.out, exist_ok=True)
    write(os.path.join(args.out, 'sysgen-drivers.tsv'),
          ['release', 'module', 'flags', 'flag_meanings', 'prefix', 'soft_major', 'devices', 'dependencies',
           'missing_dependencies', 'stub_count', 'IP27', 'IP30', 'IP35', 'kernel_symbols_ip27', 'status',
           'grade', 'role', 'citation', 'source'], drv_rows)
    write(os.path.join(args.out, 'sysgen-tunables.tsv'),
          ['release', 'module', 'block', 'class', 'name', 'condition', 'default', 'minimum', 'maximum', 'form', 'values',
           'IP27', 'IP30', 'IP35', 'status', 'source'], tune_rows)
    print('drivers: %d; tunables: %d; kernel symbols read: %d' % (len(drv_rows), len(tune_rows), len(symbols)))


def write(path, header, rows):
    with open(path, 'w', encoding='utf-8') as f:
        f.write('\t'.join(header) + '\n')
        for r in rows:
            f.write('\t'.join(str(x) for x in r) + '\n')
    print('%s: %d rows' % (os.path.relpath(path, REPO), len(rows)))


if __name__ == '__main__':
    main()
