#!/usr/bin/env python3
"""Driver, object and tunable tables from the kernel configuration files of each release.

Usage: tools/gen-sysgen.py [--out docs/inventory]

Run it from the repository root, inside the toolchain container (tools/re-run python3 tools/gen-sysgen.py).
It reads every root/<release>/var/sysgen.IPnn directory that is present, and the object archives beside
it in root/<release>/usr/cpu/sysgen/IPnnboot. It writes three tab-separated tables and never copies the
comments of the source files.

  sysgen-drivers.tsv  one row per master.d entry (one driver or module) per release: the flags, the prefix,
                      the external major numbers, the device count and the dependencies from the entry,
                      then for each machine (IP27, IP30, IP35) how the machine's system files name the
                      module. Status values:
                        use, include, vector  named by a USE, INCLUDE or VECTOR line of the machine's files
                        dependency            pulled in by a dependency line of a module named above
                        excluded              named by an EXCLUDE line (wins over the other kinds)
                        ''                    not named for that machine
                      A module named only in a dependency line of an excluded module is not pulled in.
  sysgen-objects.tsv  one row per (release, machine, module): the object or archive the boot directory holds
                      for the module (`M.o`, `M.a`, or `stubs:Mstubs.a` when only the stub archive is there),
                      the number of defined symbols that start with the entry's prefix, and which of the
                      usual entry-point suffixes exist as `<prefix><suffix>` in that object.
  sysgen-tunables.tsv one row per tunable in the mtune files per release: the file, the section heading and its
                      class (static or run), the name, the default and the minimum and maximum when the line
                      gives exactly three numbers or expressions (default, minimum, maximum). Trailing flag
                      tokens (LL, ll) and comments are kept out of the values. A line with other shapes keeps
                      its raw values in `raw` and leaves minimum and maximum blank.

Nothing here is hand-edited, and the output is deterministic. The machines' tunable overrides (stune)
are read too; a non-comment line there is recorded in `stune` for that machine.
"""
import collections
import glob
import os
import re
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MACHINES = ('IP27', 'IP30', 'IP35')
ENTRY_SUFFIXES = ('open', 'close', 'read', 'write', 'ioctl', 'strategy', 'attach', 'init', 'intr',
                  'probe', 'poll', 'mmap', 'unload', 'load', 'getdevice', 'halt', 'shutdown')
FLAG_MEANINGS = {
    'k': 'kernel object, one per kernel',
    'j': 'file system type',
    'f': 'stream framework device',
    'm': 'stream module',
    'o': 'one specification only',
    't': 'tty table entry',
    'r': 'required',
    'b': 'block device',
    'c': 'character device',
    's': 'software driver',
    'x': 'configurable object, not a driver',
    'n': 'semaphored',
    'p': 'unsemaphored, master processor only',
    'u': 'stub functions, loaded last',
    'w': 'write-back cache operation on write data',
    'd': 'dynamically loadable',
    'R': 'auto-registering dynamic module',
    'N': 'not auto-unloaded',
    'D': 'loaded then unloaded',
}
SECTION_RE = re.compile(r'^(\w[\w.\-]*)\s*:\s*(static|run)\s*$')
MODULE_RE = re.compile(r'module=(\S+)')
NUM_RE = re.compile(r'^(-?\d+|0x[0-9a-fA-F]+)$')
NM_RE = re.compile(r'^(?P<path>[^:]+):(?:(?P<member>[^:\s]+):)?(?:[0-9a-fA-F]+\s+)?(?P<type>[A-Za-z]) (?P<name>\S+)$')


def comment_free(line):
    return re.sub(r'/\*.*?\*/', '', line)


def content_lines(path):
    with open(path, encoding='latin-1') as f:
        for line in f:
            s = line.rstrip('\n')
            if not s.strip() or s.lstrip().startswith('*'):
                continue
            yield s


def parse_master(path):
    """Header fields of one master.d entry: (flags, prefix, soft, ndev, deps) or None for a non-driver file."""
    for s in content_lines(path):
        if s.startswith('$'):
            break
        toks = re.split(r'\s+', s.strip())
        if toks[0] == 'beginseg' or not re.fullmatch(r'[A-Za-z]+|-', toks[0]):
            return None
        # Trailing fields are optional in the shipped files; a missing one is recorded as blank.
        flags, prefix, soft, ndev, deps = (toks + [''] * 5)[:5]
        flags = '' if flags == '-' else flags
        return flags, prefix, soft, ndev, [d for d in re.split(r'[\s,]+', deps) if d]
    return None


def parse_system(sysdir):
    """Directive sets for one machine: named modules by kind and the excluded names."""
    named = collections.defaultdict(set)
    excluded = set()
    for path in sorted(glob.glob(os.path.join(sysdir, 'system', '*.sm'))):
        for s in content_lines(path):
            if ':' not in s:
                continue
            kw, _, val = s.partition(':')
            kw = kw.strip().upper()
            if kw in ('USE', 'INCLUDE'):
                for name in re.split(r'[\s,]+', val.strip()):
                    if name:
                        named[name].add(kw.lower())
            elif kw == 'VECTOR':
                m = MODULE_RE.search(val)
                if m:
                    named[m.group(1)].add('vector')
            elif kw == 'EXCLUDE':
                for name in re.split(r'[\s,]+', val.strip()):
                    if name:
                        excluded.add(name)
    return named, excluded


def configured(named, excluded, deps_of, known):
    status = {}
    for name, kinds in named.items():
        if name in excluded or name not in known:
            continue
        status[name] = '+'.join(sorted(kinds))
    queue = list(status)
    while queue:
        mod = queue.pop()
        for dep in deps_of.get(mod, ()):
            if dep in excluded or dep not in known or dep in status:
                continue
            status[dep] = 'dependency'
            queue.append(dep)
    for name in excluded:
        if name in known:
            status[name] = 'excluded'
    return status


def defined_symbols(nm, files):
    """Defined symbol names per object file, read from one nm call."""
    syms = collections.defaultdict(set)
    if not files:
        return syms
    # nm exits 1 when some member has no symbol table (an archive of data only); the symbols it did print are kept.
    out = subprocess.run([nm, '-A', '--defined-only'] + files, capture_output=True,
                         text=True, env=dict(os.environ, LC_ALL='C')).stdout
    for line in out.splitlines():
        m = NM_RE.match(line)
        if m:
            syms[m.group('path')].add(m.group('name'))
    return syms


def mtune_rows(release, mtune_dir):
    """Tunable rows for one release's mtune directory; the files are shared across machines."""
    rows = []
    for path in sorted(glob.glob(os.path.join(mtune_dir, '*'))):
        fname = os.path.basename(path)
        heading = ''
        klass = ''
        for s in content_lines(path):
            m = SECTION_RE.match(s.strip())
            if m:
                heading, klass = m.group(1), m.group(2)
                continue
            body = comment_free(s).strip()
            if not body or body.startswith('*'):
                continue
            toks = body.split()
            name, vals = toks[0], toks[1:]
            flag = ''
            if vals and vals[-1] in ('LL', 'll'):
                flag = vals.pop()
            default = vals[0] if vals else ''
            lo = hi = ''
            raw = ' '.join(vals[1:])
            if len(vals) == 3:
                lo, hi = vals[1], vals[2]
                raw = ''
            rows.append({'release': release, 'file': fname, 'section': heading, 'class': klass,
                         'name': name, 'default': default, 'min': lo, 'max': hi,
                         'flag': flag, 'raw': raw})
    return rows


def stune_values(path):
    out = {}
    if not os.path.exists(path):
        return out
    for s in content_lines(path):
        body = comment_free(s).strip()
        if '=' in body:
            k, _, v = body.partition('=')
            out[k.strip()] = v.strip()
    return out


def write_tsv(path, header, rows):
    with open(path, 'w', encoding='utf-8') as f:
        f.write('\t'.join(header) + '\n')
        for r in rows:
            f.write('\t'.join(str(r.get(h, '')) for h in header) + '\n')


def main(argv):
    out = os.path.join(REPO, 'docs', 'inventory')
    if '--out' in argv:
        out = argv[argv.index('--out') + 1]
    nm = os.environ.get('NM', 'mips64-linux-gnuabi64-nm')
    releases = sorted(os.path.basename(os.path.dirname(os.path.dirname(p)))
                      for p in glob.glob(os.path.join(REPO, 'root', '*', 'var', 'sysgen.IP*')))
    releases = sorted(set(releases))
    if not releases:
        sys.exit('no root/<release>/var/sysgen.IPnn directories; populate root/ first')

    driver_rows, object_rows, tune_rows, notes = [], [], [], []
    for rel in releases:
        base = os.path.join(REPO, 'root', rel)
        mdir = {p: os.path.join(base, 'var', 'sysgen.' + p) for p in MACHINES}
        present = [p for p in MACHINES if os.path.isdir(os.path.join(mdir[p], 'master.d'))]
        if not present:
            continue
        masters = {}
        for p in present:
            names = {}
            for path in sorted(glob.glob(os.path.join(mdir[p], 'master.d', '*'))):
                name = os.path.basename(path)
                if name == 'README':
                    continue
                names[name] = parse_master(path)
            masters[p] = names
        ref = masters[present[0]]
        for p in present[1:]:
            if masters[p] != ref:
                notes.append(f'{rel}: master.d differs between {present[0]} and {p}')
        mt = os.path.join(base, 'var', 'sysgen.' + present[0], 'mtune')
        for p in present[1:]:
            if not os.path.isdir(os.path.join(mdir[p], 'mtune')):
                continue
            if subprocess.run(['diff', '-rq', mt, os.path.join(mdir[p], 'mtune')],
                              capture_output=True).returncode:
                notes.append(f'{rel}: mtune differs between {present[0]} and {p}')
        tune_rows.extend(mtune_rows(rel, mt))
        for p in present:
            st = stune_values(os.path.join(mdir[p], 'stune'))
            for k, v in st.items():
                tune_rows.append({'release': rel, 'file': 'stune:' + p, 'section': '', 'class': '',
                                  'name': k, 'default': v, 'min': '', 'max': '', 'flag': '', 'raw': ''})

        boot = {p: os.path.join(base, 'usr', 'cpu', 'sysgen', p + 'boot') for p in present}
        bootfiles = {p: sorted(glob.glob(os.path.join(boot[p], '*.[ao]'))) for p in present}
        syms = {p: defined_symbols(nm, bootfiles[p]) for p in present}

        system = {p: parse_system(mdir[p]) for p in present}
        deps_of = {n: (ref[n][4] if ref[n] else []) for n in ref}
        status = {p: configured(system[p][0], system[p][1], deps_of, set(ref)) for p in present}

        for name in sorted(ref):
            entry = ref[name]
            if entry is None:
                notes.append(f'{rel}: {name} is not a driver entry (no header)')
                continue
            flags, prefix, soft, ndev, deps = entry
            meanings = '; '.join(FLAG_MEANINGS.get(c, '?') for c in flags if c in FLAG_MEANINGS)
            row = {'release': rel, 'module': name, 'flags': flags, 'flag_meanings': meanings,
                   'prefix': prefix, 'major': soft, 'devices': ndev, 'dependencies': ','.join(deps)}
            for p in MACHINES:
                row[p.lower()] = status[p].get(name, '') if p in present else 'no-sysgen'
            driver_rows.append(row)

            for p in present:
                obj_path = None
                for cand in (name + '.o', name + '.a'):
                    full = os.path.join(boot[p], cand)
                    if os.path.exists(full):
                        obj_path = full
                        break
                if obj_path:
                    where = os.path.basename(obj_path)
                    defined = syms[p].get(obj_path, set())
                else:
                    stub = os.path.join(boot[p], name + 'stubs.a')
                    where = 'stubs:' + os.path.basename(stub) if os.path.exists(stub) else ''
                    defined = set()
                pref = prefix if prefix not in ('-', '') else ''
                count = sum(1 for s in defined if pref and s.startswith(pref))
                entries = ' '.join(s for s in ENTRY_SUFFIXES if pref and (pref + s) in defined) \
                    if pref else ''
                object_rows.append({'release': rel, 'machine': p, 'module': name, 'object': where,
                                    'prefix': prefix, 'prefix_symbols': count if pref else '',
                                    'entry_points': entries, 'status': status[p].get(name, '')})

    os.makedirs(out, exist_ok=True)
    write_tsv(os.path.join(out, 'sysgen-drivers.tsv'),
              ['release', 'module', 'flags', 'flag_meanings', 'prefix', 'major', 'devices',
               'dependencies'] + [p.lower() for p in MACHINES], driver_rows)
    write_tsv(os.path.join(out, 'sysgen-objects.tsv'),
              ['release', 'machine', 'module', 'object', 'prefix', 'prefix_symbols', 'entry_points',
               'status'], object_rows)
    write_tsv(os.path.join(out, 'sysgen-tunables.tsv'),
              ['release', 'file', 'section', 'class', 'name', 'default', 'min', 'max', 'flag', 'raw'],
              tune_rows)
    for n in notes:
        print('note:', n, file=sys.stderr)
    print(f'releases {",".join(releases)}; drivers {len(driver_rows)}; objects {len(object_rows)}; '
          f'tunables {len(tune_rows)}')


if __name__ == '__main__':
    main(sys.argv[1:])
