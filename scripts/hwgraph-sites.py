#!/usr/bin/env python3
"""Hardware-graph calls in the 6.5.30 kernels: every site that adds a path or an edge.

Usage: scripts/hwgraph-sites.py [--kernel IP27 IP30 IP35] [--out docs]

Run from the repository root inside the toolchain container, with root/6.5.30 populated
(tools/re-run python3 scripts/hwgraph-sites.py). Writes two tab-separated tables:

  hwgraph-sites.tsv  one row per call to a graph-add primitive: kernel, call address, routine,
                     primitive, the argument that carries the name, the name as the call
                     passes it (a literal, a format and its sprintf arguments, or a value the
                     routine does not fix), the window the value was read in, and a grade.
  hwgraph-edges.tsv  one row per edge name: the name (a path component or an edge argument), its
                     form (literal, or a format with placeholders), the kernels and call sites
                     that add it, and the grade of its best site. Only names that are literals or
                     formats count; register and unset values stay in the sites table.

Grades. established: the call site is in a routine read by the fact sheet (scripts/facts.py), and
the name is a literal or a sprintf format built in the same window. inferred: the name comes from
a stack buffer whose writer is not in the window, or from a callee whose body was not read.
unknown: the value is a register or loaded pointer the routine does not fix.

Primitive roles, from the bodies read: hwgraph_path_add(from, path, out) creates one vertex and
one edge per component of path, the edge named by the component (its callees are
graph_path_get_component, hwgraph_vertex_create and hwgraph_edge_add); hwgraph_edge_add(from, to,
name) names its edge by a2; hwgraph_char_device_add(from, path, driver, out) adds path when a1 is
set, after device_driver_get on the driver in a2. hwgraph_block_device_add was not read, so its
sites are graded inferred even where the name is a literal.
Other primitives are listed with role "unread" and do not feed the edge table.
"""
import argparse
import bisect
import collections
import os
import re
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'scripts'))
import facts  # noqa: E402

KERNELS = {'IP27': 'root/6.5.30/unix.IP27', 'IP30': 'root/6.5.30/unix.IP30', 'IP35': 'root/6.5.30/unix.IP35'}
# name argument register per primitive; None means the call carries no name
ROLE = {
    'hwgraph_path_add': ('a1', 'path'),
    'hwgraph_edge_add': ('a2', 'edge'),
    'hwgraph_char_device_add': ('a1', 'path'),
    'hwgraph_block_device_add': ('a1', 'path'),
    'hwgraph_device_add': (None, 'unread'),
    'hwgraph_link_add': (None, 'unread'),
    'hwgraph_vertex_create': (None, 'vertex'),
    'graph_vertex_create': (None, 'unread'),
    'graph_edge_add': (None, 'unread'),
}
PRIMS = set(ROLE)
DISASM = re.compile(r'^([0-9a-f]+):\t[0-9a-f ]+\t(\S+)\s*(.*)$')
BRANCHES = {'beq', 'bne', 'beqz', 'bnez', 'b', 'bgez', 'bltz', 'blez', 'bgtz', 'j', 'beql', 'bnel',
            'beqzl', 'bnezl', 'bgezl', 'bltzl', 'blezl', 'bgtzl'}


def cstring(img, addr, limit=300):
    """The printable NUL-terminated string at a linked address in a file-backed section, or None."""
    for s in img.sections:
        a = int(s['address'], 16)
        if a and s['type'] != 'NOBITS' and a <= addr < a + s['size']:
            o = int(s['offset'], 16) + addr - a
            raw = img.data[o:o + limit]
            n = raw.find(b'\0')
            if n < 0:
                return None
            t = raw[:n]
            return t.decode('ascii') if all(32 <= c < 127 for c in t) else None
    return None


def arg_value(img, v, sprints, call_addr):
    """How one argument reads: a literal string, a format, a stack buffer or a value."""
    if re.fullmatch(r'0x[0-9a-f]+', v):
        text = cstring(img, int(v, 16))
        # an address with no printable string behind it is a buffer, not a name
        return {'kind': 'literal' if text else 'value', 'text': text}
    if re.fullmatch(r'sp[+-]\d+', v):
        fm = None
        for sp in sprints:
            if int(sp['address'], 16) < call_addr and sp.get('args', {}).get('a0') == v:
                a1 = sp.get('args', {}).get('a1', '')
                fmt = cstring(img, int(a1, 16)) if re.fullmatch(r'0x[0-9a-f]+', a1) else None
                fm = {'format': fmt, 'args': {k: x for k, x in sp.get('args', {}).items() if k not in ('a0', 'a1')}}
        return {'kind': 'format' if fm and fm['format'] is not None else 'stack', 'text': fm and fm['format'], 'sprintf': fm}
    return {'kind': 'value', 'text': v}


class Kernel:
    def __init__(self, label, path):
        self.label, self.path = label, path
        self.img = facts.image(path)
        self.funcs = sorted([x for x in self.img.syms if x[4]], key=lambda x: x[0])
        self.starts = [x[0] for x in self.funcs]
        self.lines = self.disassembly()
        self.jr_ra = sorted(a for a, mn, _ in self.lines if mn == 'jr' and 'ra' in _)

    def disassembly(self):
        out = subprocess.run(['mips64-linux-gnuabi64-objdump', '-d', self.path], check=True,
                             capture_output=True, text=True).stdout
        insns = []
        for ln in out.splitlines():
            m = DISASM.match(ln)
            if m:
                insns.append((int(m.group(1), 16), m.group(2), m.group(3)))
        return insns

    def sites(self):
        """(address, primitive) for every jal/j/bal to a graph-add primitive."""
        found = []
        for a, mn, ops in self.lines:
            if mn in ('jal', 'j', 'bal'):
                m = re.search(r'<(\w+)>', ops)
                if m and m.group(1) in PRIMS:
                    found.append((a, m.group(1)))
        return found

    def routine_for(self, addr):
        """A symbol start at or before addr, or the instruction after the last return when no symbol is near."""
        i = bisect.bisect_right(self.starts, addr) - 1
        if i >= 0 and addr - self.starts[i] <= 0x8000:
            return self.starts[i]
        k = bisect.bisect_left(self.jr_ra, addr) - 1
        return self.jr_ra[k] + 8 if k >= 0 else addr

    def block_leader(self, addr, targets):
        """Start of the straight-line block that holds addr: after the last control transfer, or a branch target."""
        idx = [a for a, _, _ in self.lines]
        k = bisect.bisect_left(idx, addr) - 1
        lead = 0
        j = k - 1
        while j >= 0:
            a, mn, _ = self.lines[j]
            if mn in BRANCHES or mn == 'jr' or mn.startswith('bc1') or mn.startswith('bgez') or \
                    mn.startswith('bltz') or mn.startswith('blez') or mn.startswith('bgtz'):
                lead = a + 8
                break
            j -= 1
        tg = [t for t in targets if lead <= t <= addr]
        return max(tg) if tg else lead

    def branch_targets(self):
        out = set()
        for a, mn, ops in self.lines:
            if mn in BRANCHES:
                m = re.search(r'([0-9a-f]{8,16}) <', ops)
                if m:
                    out.add(int(m.group(1), 16))
        return out

    def rows(self):
        sites = self.sites()
        by_routine = collections.defaultdict(list)
        for a, prim in sites:
            by_routine[self.routine_for(a)].append((a, prim))
        done = {}
        out = []
        for start, lst in sorted(by_routine.items()):
            hi = max(a for a, _ in lst) + 0x40
            symsz = [x[1] for x in self.funcs if x[0] == start]
            fs = facts.fact_sheet(self.path, '0x%x' % start, size=max([hi - start] + symsz))
            name = fs['routine']['name'] or 'sub_%x' % start
            for row in self.call_rows(fs, lst, name, 'routine'):
                done[int(row['site'], 16)] = row
                out.append(row)
        missing = [a for a, _ in sites if a not in done]
        targets = self.branch_targets()
        by_lead = collections.defaultdict(list)
        for a in missing:
            by_lead[self.block_leader(a, targets)].append((a, dict(sites)[a]))
        for lead, lst in sorted(by_lead.items()):
            hi = max(a for a, _ in lst) + 0x40
            fs = facts.fact_sheet(self.path, '0x%x' % lead, size=hi - lead)
            for row in self.call_rows(fs, lst, 'block_%x' % lead, 'block'):
                out.append(row)
        return sorted(out, key=lambda r: int(r['site'], 16))

    def call_rows(self, fs, lst, name, window):
        want = {a: p for a, p in lst}
        sprints = [c for c in fs['calls'] if c.get('callee') == 'sprintf']
        rows = []
        for c in fs['calls']:
            a = int(c['address'], 16)
            if a not in want:
                continue
            prim = want[a]
            reg, role = ROLE[prim]
            args = c.get('args', {})
            name_arg = None
            grade = 'unknown'
            if reg is None:
                grade = 'inferred' if role == 'unread' else 'established'
                text = None
                kind = 'none'
            else:
                v = args.get(reg)
                if v is None:
                    text, kind = None, 'unset'
                    grade = 'unknown'
                else:
                    av = arg_value(self.img, v, sprints, a)
                    kind, text = av['kind'], av['text']
                    if kind in ('literal', 'format'):
                        grade = 'established' if role != 'unread' and prim != 'hwgraph_block_device_add' else 'inferred'
                    elif kind == 'stack':
                        grade = 'inferred'
                    if role == 'unread':
                        grade = 'inferred'
                name_arg = v
            rows.append({'kernel': self.label, 'site': c['address'], 'routine': name, 'window': window,
                         'primitive': prim, 'role': role, 'arg': reg or '-', 'value': name_arg or '',
                         'kind': kind if reg else 'none', 'name': text if text is not None else '',
                         'grade': grade})
        return rows


def components(path):
    """Path components: the literal pieces between slashes, format pieces kept as written."""
    return [p for p in path.split('/') if p != '']


def edge_rows(kernels_rows):
    """One row per edge name from the path components and edge-argument names of the established sites."""
    edges = collections.defaultdict(lambda: {'kernels': set(), 'sites': set(), 'grades': set(), 'prims': set()})
    for r in kernels_rows:
        if r['role'] not in ('path', 'edge') or r['kind'] not in ('literal', 'format') or not r['name']:
            continue
        names = components(r['name']) if r['role'] == 'path' else [r['name']]
        for n in names:
            e = edges[n]
            e['kernels'].add(r['kernel'])
            e['sites'].add('%s:%s' % (r['kernel'], r['site']))
            e['grades'].add(r['grade'])
            e['prims'].add(r['primitive'])
    out = []
    order = {'established': 0, 'inferred': 1, 'unknown': 2}
    for n, e in sorted(edges.items()):
        best = min(e['grades'], key=lambda g: order.get(g, 9))
        out.append({'name': n, 'form': 'format' if '%' in n else 'literal', 'kernels': ','.join(sorted(e['kernels'])),
                    'primitives': ','.join(sorted(e['prims'])), 'grade': best, 'count': len(e['sites']),
                    'sites': ' '.join(sorted(e['sites']))})
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--kernel', nargs='*', default=sorted(KERNELS))
    ap.add_argument('--out', default=os.path.join(REPO, 'docs'))
    a = ap.parse_args(argv)
    allrows = []
    for k in a.kernel:
        allrows += Kernel(k, os.path.join(REPO, KERNELS[k])).rows()
    cols = ['kernel', 'site', 'routine', 'window', 'primitive', 'role', 'arg', 'value', 'kind', 'name', 'grade']
    with open(os.path.join(a.out, 'hwgraph-sites.tsv'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\t'.join(cols) + '\n')
        for r in allrows:
            f.write('\t'.join(str(r[c]) for c in cols) + '\n')
    ecols = ['name', 'form', 'count', 'grade', 'kernels', 'primitives', 'sites']
    with open(os.path.join(a.out, 'hwgraph-edges.tsv'), 'w', encoding='utf-8', newline='\n') as f:
        f.write('\t'.join(ecols) + '\n')
        for r in edge_rows(allrows):
            f.write('\t'.join(str(r[c]) for c in ecols) + '\n')
    print('sites %d' % len(allrows))


if __name__ == '__main__':
    main()
