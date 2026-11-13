#!/usr/bin/env python3
"""Hardware-graph add calls in the linked 6.5.30 kernels, with the path or edge name each one passes.

Usage: tools/hwgraph_sites.py extract [--show] [--json OUT]
       tools/hwgraph_sites.py render [--out evidence/inventory/hwgraph-adds.tsv]

Run from the repository root inside the toolchain environment (retools/tools/re-run python3 ...), with root/ populated.

extract finds every jal or bal to one of the hardware-graph add routines in each root/6.5.30/unix.IP* image,
attributes the call to its routine the way retools/tools/kudzu/panic_sites.py does, and reads the argument that carries the
name (ADD_CALLEES says which). Argument values come from the same straight-line scan (retools/tools/kudzu/panic_sites.py
`value_at_call`); a string at a resolved address is read from the image's own data. A name that is built at run
time (sprintf output, a register that another path sets) is reported as unresolved with the reason.

render writes evidence/inventory/hwgraph-adds.tsv: one row per (callee, name, routine), with the builds that
have the site and the site addresses in each. Nothing here is hand-edited.
"""
import argparse
import bisect
import collections
import json
import os
import re
import subprocess
import sys

os.environ['LC_ALL'] = os.environ['LANG'] = 'C'
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'retools', 'tools', 'kudzu'))

import panic_sites as ps  # noqa: E402  (shared argument scan and string reader)

BUILDS = ('IP27', 'IP30', 'IP35')
BINARY = '6.5.30/unix.%s'
# Argument that carries the name, from the IRIX graph routines' prototypes read as hints and confirmed
# against the call sites in the images ("kind" names what the string is).
ADD_CALLEES = {
    'hwgraph_path_add': ('path', 'a1'),
    'hwgraph_edge_add': ('edge', 'a2'),
    'hwgraph_link_add': ('edge', 'a2'),
    'hwgraph_device_add': ('path', 'a1'),
    'hwgraph_block_device_add': ('path', 'a1'),
    'hwgraph_char_device_add': ('path', 'a1'),
    'if_hwgraph_add': ('path', 'a1'),
    'if_hwgraph_alias_add': ('alias', 'a1'),
    'graph_edge_add': ('edge', 'a3'),
}
# A second name argument, for the routines that take a prefix (the device name base) as well.
PREFIX_ARG = {'hwgraph_device_add': 'a2', 'hwgraph_block_device_add': 'a2', 'hwgraph_char_device_add': 'a2',
              'if_hwgraph_add': 'a2'}
TABLE = os.path.join(REPO, 'evidence', 'inventory', 'hwgraph-adds.tsv')
CACHE = os.path.join(REPO, '.cache', 'hwgraph-sites')
CALL = re.compile(r'\b(?:jal|bal)\s+([0-9a-f]+)\b')
COND_BRANCHES = re.compile(r'^(beq|bne|bnez|beqz|blez|bgtz|bltz|bgez|blt|bge|bgt|ble|b)[a-z]*$')


def callee_targets(binary):
    """Addresses of the add routines in the image, from its symbol table."""
    out = subprocess.run(['mips64-linux-gnuabi64-nm', os.path.join(REPO, 'root', binary)],
                         capture_output=True, text=True).stdout
    found = {}
    for ln in out.splitlines():
        p = ln.split()
        if len(p) == 3 and p[1] in 'Tt' and p[2] in ADD_CALLEES:
            found[int(p[0], 16)] = p[2]
    return found


def sizes_of(binary):
    nm = subprocess.run(['mips64-linux-gnuabi64-nm', '-S', '--defined-only', os.path.join(REPO, 'root', binary)],
                        capture_output=True, text=True).stdout
    sizes = {}
    for ln in nm.splitlines():
        p = ln.split()
        if len(p) == 4 and p[2] in 'Tt':
            sizes[int(p[0], 16)] = int(p[1], 16)
    return sizes


def arg_value(insns, idx, reg, ctx):
    v, how = ps.value_at_call(insns, idx, reg, ctx)
    entry = {'value': ('0x%x' % v) if v is not None else None, 'how': how}
    if v is not None:
        s = ps.read_string(ctx['secs'], v)
        if s is not None:
            entry['string'] = s
    return entry


def gp_of(binary, addr, cache):
    """The $gp value the routine at addr sets, from its fact sheet. The kernel sets one $gp for all its
    routines, so the first routine's value is kept for the image; every routine read so far agrees."""
    if 'gp' not in cache:
        fs = ps.facts(binary, hex(addr))
        cache['gp'] = int(fs['routine']['gp'], 16) if fs['routine'].get('gp') else None
    return cache['gp']


def extract(show):
    out = []
    for build in BUILDS:
        binary = BINARY % build
        insns, labels, _ps_targets, _ = None, None, None, None
        insns, labels, sizes_ps, _t = ps.listing(binary)
        targets = callee_targets(binary)
        sizes = sizes_of(binary)
        label_addrs = [a for a, _n in labels]
        index_of = {a: i for i, (a, _t2) in enumerate(insns)}
        secs = ps.sections(binary)
        bentries = ps.branch_entries(insns)
        ctx_base = {'secs': secs, 'targets': set(bentries)}
        gp_cache = {}
        for idx, (addr, text) in enumerate(insns):
            m = CALL.search(text)
            if not m:
                continue
            callee = targets.get(int(m.group(1), 16))
            if not callee:
                continue
            li = bisect.bisect_right(label_addrs, addr) - 1
            if li < 0:
                continue
            lab_addr, lab_name = labels[li]
            size = sizes.get(lab_addr, 0)
            if size and addr < lab_addr + size:
                name, rstart = lab_name, lab_addr
            else:
                rstart = ps.unlabelled_start(insns, index_of, lab_addr + size)
                name = 'sub_0x%x' % rstart
            ctx = dict(ctx_base, gp=gp_of(binary, lab_addr, gp_cache))
            kind, reg = ADD_CALLEES[callee]
            name_arg = arg_value(insns, idx, reg, ctx)
            rec = {'build': build, 'binary': binary, 'routine': name, 'routine_address': '0x%x' % rstart,
                   'callee': callee, 'site': '0x%x' % addr, 'kind': kind, 'reg': reg, 'arg': name_arg}
            if callee in PREFIX_ARG:
                rec['prefix'] = arg_value(insns, idx, PREFIX_ARG[callee], ctx)
            out.append(rec)
            if show:
                print('== %s %s %s @ %s (%s): %s=%s %s' % (
                    build, name, callee, rec['site'], kind, reg, name_arg['value'],
                    ('"%s"' % name_arg['string']) if 'string' in name_arg else ('(%s)' % name_arg['how'])))
    return out


def name_of(rec):
    """The name a site passes: the string when it resolved, else the reason it did not (addresses removed,
    so sites with the same cause share a row)."""
    a = rec['arg']
    if 'string' in a:
        return a['string']
    return '(not a literal: %s)' % re.sub(r'0x[0-9a-f]+', 'ADDR', a['how'])


def render(recs):
    groups = collections.OrderedDict()
    for r in recs:
        key = (r['callee'], name_of(r), r['routine'])
        g = groups.setdefault(key, {'builds': collections.OrderedDict(), 'prefix': set()})
        g['builds'].setdefault(r['build'], []).append(r['site'])
        if 'prefix' in r and 'string' in r['prefix']:
            g['prefix'].add(r['prefix']['string'])
    lines = ['callee\tkind\tname\tprefix\troutine\tbuilds\tsites\tgrade']
    for (callee, name, routine), g in sorted(groups.items()):
        kind = ADD_CALLEES[callee][0]
        builds = ','.join(g['builds'].keys())
        sites = '; '.join('%s %s' % (b, ','.join(s)) for b, s in g['builds'].items())
        grade = 'unresolved' if name.startswith('(not a literal') else 'located'
        lines.append('\t'.join([callee, kind, name, ','.join(sorted(g['prefix'])), routine, builds, sites, grade]))
    return '\n'.join(lines) + '\n'


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest='cmd', required=True)
    ex = sub.add_parser('extract')
    ex.add_argument('--show', action='store_true')
    ex.add_argument('--json', help='write the site records to this file')
    rd = sub.add_parser('render')
    rd.add_argument('--out', default=TABLE)
    args = ap.parse_args()
    if args.cmd == 'extract':
        recs = extract(args.show)
        if args.json:
            with open(args.json, 'w', encoding='utf-8') as f:
                json.dump(recs, f, indent=1)
        print('%d add sites' % len(recs), file=sys.stderr)
    else:
        recs = extract(False)
        with open(args.out, 'w', encoding='utf-8') as f:
            f.write(render(recs))


if __name__ == '__main__':
    main()
