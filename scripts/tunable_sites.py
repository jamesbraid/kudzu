#!/usr/bin/env python3
"""Read sites of each sysgen tunable in the linked 6.5.30 kernels.

Usage: scripts/tunable_sites.py [--root root/6.5.30] [--kernels IP27 IP30 IP35]
                                [--out docs/inventory]

Run from the repository root inside the toolchain container
(retools/tools/re-run python3 scripts/tunable_sites.py), with root/ populated.

Input: docs/inventory/sysgen-tunables.tsv (scripts/sysgen_tables.py). Every tunable name there is
looked up as a data symbol of the same name in each linked kernel. The tunables are read from the
6.5.22 configuration files; the lookup and the sites are of the 6.5.30 kernels.

For each function symbol of a kernel, retools/tools/facts.py runs its symbolic pass. A load, store or
address formation whose resolved absolute address falls inside a tunable's symbol is one site.
The pass follows the code as shipped; a site it cannot resolve (an address that depends on an
argument, for instance) is not counted, and the routine's count of unresolved accesses is not
measured here.

Outputs (tab-separated, deterministic):
  tunable-sites.tsv    one row per tunable, per kernel, per site: the routine containing it, the
                       instruction address, the access kind, width and text, and the symbolic
                       value the code loads or stores.
  tunable-summary.tsv  one row per tunable and kernel: the symbol's address and size, the counts of
                       loads, stores and address formations, the routines that read it, and the
                       grade and role from scripts/tunable-roles.json. A row is understood only
                       with a grade, a role and a citation.

A tunable with no symbol in a kernel is written with status `no-symbol`; one with a symbol and no
site is `no-site`. Both are facts about that kernel image.
"""
import argparse
import bisect
import collections
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
sys.path.insert(0, os.path.join(REPO, 'retools', 'tools'))

import facts  # noqa: E402  the symbolic pass shared with the fact sheets

KERNELS = ('IP27', 'IP30', 'IP35')
FUNC_SOURCES = ('symtab', 'dynsym', 'mdebug')


def load_tunables(path):
    """Unique tunable names in the sysgen table, with the first row's fields and every default."""
    rows = [l.rstrip('\n').split('\t') for l in open(path, encoding='utf-8')]
    head = rows[0]
    col = {k: head.index(k) for k in ('module', 'class', 'name', 'default', 'minimum', 'maximum', 'condition')}
    out = collections.OrderedDict()
    for r in rows[1:]:
        name = r[col['name']]
        t = out.setdefault(name, {'module': r[col['module']], 'class': r[col['class']],
                                  'minimum': r[col['minimum']], 'maximum': r[col['maximum']],
                                  'defaults': [], 'conditions': []})
        d = r[col['default']]
        if d not in t['defaults']:
            t['defaults'].append(d)
        c = r[col['condition']]
        if c != '-' and c not in t['conditions']:
            t['conditions'].append(c)
    return out


def data_symbol(img, name):
    """The data symbol(s) named `name`. Static variables of the same name in one image all count."""
    return [x for x in img.byname.get(name, []) if not x[4]]


def cache_disassembly(img, funcs):
    """Disassemble the whole function range once, then cut each routine's words from it. One objdump
    run per routine would take far longer than the pass itself."""
    lo = min(f[0] for f in funcs)
    hi = max(f[0] + f[1] for f in funcs)
    allins = img.disassemble(lo, hi - lo, None)
    addrs = [i['addr'] for i in allins]

    def sliced(addr, size, section):
        i = bisect.bisect_left(addrs, addr)
        j = bisect.bisect_left(addrs, addr + size)
        return allins[i:j]

    img.disassemble = sliced


def scan(img, names):
    """Every site in the image: (routine, routine address, access dict) for each access that resolves
    to a tunable's symbol, plus the address-formation sites."""
    ranges = []
    for name in names:
        for x in data_symbol(img, name):
            ranges.append((x[0], x[0] + max(x[1], 1), name, x))
    ranges.sort(key=lambda r: (r[0], r[2]))
    starts = [r[0] for r in ranges]
    widest = max([r[1] - r[0] for r in ranges] or [1])

    def owners(a):
        i = bisect.bisect_right(starts, a) - 1
        out = []
        while i >= 0 and starts[i] > a - widest:
            if ranges[i][0] <= a < ranges[i][1]:
                out.append(ranges[i][2])
            i -= 1
        return sorted(set(out))

    sites = []
    funcs = sorted({(x[0], x[1], x[2], x[3]) for x in img.syms if x[4] and x[5] in FUNC_SOURCES and x[1]})
    cache_disassembly(img, funcs)
    for addr, size, name, sec in funcs:
        r = facts.Routine(img, name, addr, size, sec, 'symtab', facts.DEFAULT_ERRORS)
        fs = r.facts(with_insns=False)
        for acc in fs['accesses']:
            if 'absolute' not in acc:
                continue
            a = int(acc['absolute'], 16)
            for tn in owners(a):
                sites.append({'tunable': tn, 'routine': name, 'routine_address': '0x%x' % addr,
                              'site': acc['address'], 'kind': acc['kind'], 'width': acc['width'],
                              'text': acc['text'], 'value': acc.get('value', ''),
                              'address': '0x%x' % a, 'origin': acc.get('origin', '')})
        for ad in fs['addresses']:
            v = ad.get('value', '')
            if not v.startswith('0x'):
                continue
            a = int(v, 16)
            for tn in owners(a):
                sites.append({'tunable': tn, 'routine': name, 'routine_address': '0x%x' % addr,
                              'site': ad['address'], 'kind': 'address', 'width': '',
                              'text': ad['text'], 'value': v, 'address': v, 'origin': ad.get('origin', '')})
    return sites


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default=os.path.join(REPO, 'root', '6.5.30'))
    ap.add_argument('--kernels', nargs='+', default=list(KERNELS))
    ap.add_argument('--out', default=os.path.join(REPO, 'docs', 'inventory'))
    ap.add_argument('--tunables', default=os.path.join(REPO, 'docs', 'inventory', 'sysgen-tunables.tsv'))
    ap.add_argument('--roles', default=os.path.join(REPO, 'scripts', 'tunable-roles.json'))
    ap.add_argument('--roles-only', action='store_true',
                    help='rewrite the grade, role and status columns of the existing tunable-summary.tsv '
                         'from the roles file, without scanning the kernels')
    a = ap.parse_args()
    if a.roles_only:
        return roles_only(a)

    tun = load_tunables(a.tunables)
    roles = json.load(open(a.roles, encoding='utf-8')) if os.path.exists(a.roles) else {}
    names = list(tun)
    site_rows, sum_rows = [], []
    for k in a.kernels:
        path = os.path.join(a.root, 'unix.' + k)
        img = facts.image(path)
        print('%s: scanning %d tunables' % (k, len(names)), file=sys.stderr)
        sites = scan(img, names)
        by = collections.defaultdict(list)
        for s in sites:
            by[s['tunable']].append(s)
        for n in names:
            t = tun[n]
            syms = data_symbol(img, n)
            if not syms:
                sum_rows.append([n, t['module'], t['class'], '|'.join(t['defaults']), k, '-', '-', 0, 0, 0,
                                 0, '-', 'no-symbol', '', '', ''])
                continue
            addr = syms[0][0]
            size = syms[0][1]
            ss = sorted(by.get(n, []), key=lambda s: (s['routine_address'], s['site']))
            for s in ss:
                site_rows.append([n, t['module'], k, '0x%x' % addr, s['routine'], s['routine_address'],
                                  s['site'], s['kind'], s['width'], s['origin'], s['text'], s['value']])
            loads = sum(1 for s in ss if s['kind'] == 'load')
            stores = sum(1 for s in ss if s['kind'] == 'store')
            adds = sum(1 for s in ss if s['kind'] == 'address')
            rts = sorted({s['routine'] for s in ss})
            role = roles.get(n, {})
            ok = role_covers(role, k) and all(role.get(x) for x in ('grade', 'role', 'citation')) and rts
            status = 'understood' if ok else ('located' if ss else 'no-site')
            sum_rows.append([n, t['module'], t['class'], '|'.join(t['defaults']), k, '0x%x' % addr, size,
                             loads, stores, adds, len(rts), ';'.join(rts) or '-', status,
                             role.get('grade', ''), role.get('role', ''), role.get('citation', '')])
        print('%s: %d sites' % (k, len(sites)), file=sys.stderr)

    head = ['tunable', 'module', 'class', 'defaults', 'kernel', 'symbol_address', 'symbol_size',
            'loads', 'stores', 'addresses', 'routines', 'routine_names', 'status', 'grade', 'role', 'citation']
    write(os.path.join(a.out, 'tunable-summary.tsv'), head, sum_rows)
    write(os.path.join(a.out, 'tunable-sites.tsv'),
          ['tunable', 'module', 'kernel', 'symbol_address', 'routine', 'routine_address', 'site', 'kind',
           'width', 'origin', 'text', 'value'], site_rows)


def role_covers(role, kernel):
    """A role counts for the kernels it names in `kernels`; without that list, for every kernel."""
    return kernel in role.get('kernels', KERNELS)


def roles_only(a):
    """Re-apply the roles file to the summary table the last scan wrote."""
    path = os.path.join(a.out, 'tunable-summary.tsv')
    rows = [l.rstrip('\n').split('\t') for l in open(path, encoding='utf-8')]
    head, body = rows[0], rows[1:]
    roles = json.load(open(a.roles, encoding='utf-8')) if os.path.exists(a.roles) else {}
    cols = {k: head.index(k) for k in ('tunable', 'kernel', 'status', 'grade', 'role', 'citation', 'routine_names')}
    out = []
    for r in body:
        role = roles.get(r[cols['tunable']], {})
        ok = (role_covers(role, r[cols['kernel']]) and all(role.get(x) for x in ('grade', 'role', 'citation'))
              and r[cols['routine_names']] != '-')
        if r[cols['status']] in ('no-symbol', 'no-site') and not ok:
            status = r[cols['status']]
        else:
            status = 'understood' if ok else 'located'
        r[cols['status']] = status
        r[cols['grade']] = role.get('grade', '')
        r[cols['role']] = role.get('role', '')
        r[cols['citation']] = role.get('citation', '')
        out.append(r)
    write(path, head, out)


def write(path, header, rows):
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.write('\t'.join(header) + '\n')
        for r in rows:
            f.write('\t'.join(str(x).replace('\t', ' ').replace('\n', ' ') for x in r) + '\n')
    print('%s: %d rows' % (os.path.relpath(path, REPO), len(rows)))


if __name__ == '__main__':
    main()
