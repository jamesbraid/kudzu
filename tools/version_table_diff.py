#!/usr/bin/env python3
"""Differences between the 6.5.22 and 6.5.30 system call and syssgi selector tables.

Usage: tools/version_table_diff.py [--out docs/inventory/version-table-diff.tsv]

Run it from the repository root, inside the toolchain container (retools/tools/re-run python3
tools/version_table_diff.py), after retools/tools/kudzu/os_tables.py has written syscalls.tsv and
selectors.tsv for both releases. The argument-table bytes are read from root/<release>/unix.IPnn.

Inputs: docs/inventory/syscalls.tsv, docs/inventory/selectors.tsv, docs/version-diffs-queue.tsv
(the per-routine classes of the linked kernels, written by the version-diffs pass, not by a script
in this repository) and the two kernels of each platform.

Output: one row per difference, with the columns

  kind        what differs (see the kinds below)
  kernel      unix.IP27, unix.IP30 or unix.IP35
  table       sysent or irix5_64_sysent for system calls; the selector group for selectors
  key         call number or selector value
  value_6.5.22, value_6.5.30   the two sides, as the tables give them
  detail      what the row says beyond the two sides
  grade       established (read from the tables or the argument-table bytes), class-only (the
              routine differs by the queue's byte class, its body not read here) or heuristic
              (the first routine a selector calls, from the extractor's heuristic)

Kinds:

  syscall-renumbered    a handler name is at different call numbers in the two releases
  syscall-added         a call number exists only in 6.5.30 (none found)
  syscall-removed       a call number exists only in 6.5.22 (none found)
  syscall-handler       the handler name at a call number differs (none found)
  syscall-nargs         the argument-count byte differs (none found)
  syscall-flags         the flag bytes differ (none found)
  syscall-arg-words     the argument-type bytes of a table differ, up to the first address (none found)
  syscall-handler-body  the handler's routine differs by a class other than layout
  selector-added        a selector value is reached only in 6.5.30 (heuristic in a compare tree, where the
                        older image may simply not narrow to the value)
  selector-removed      a selector value is reached only in 6.5.22 (none found)
  selector-fill         the selector's target is shared with other selectors in one release only; in a
                        compare tree the address can be the test block ahead of a body that the other
                        release also reaches, so those rows are heuristic
  selector-first-call   the first routine the target calls differs by name, in a jump table (a compare
                        tree's target can be a test block ahead of the body, so its first call is not
                        comparable); `none` means the block branches before any call. Two addresses
                        of routines the image does not name are not compared: they move with the image

The argument-table comparison reads up to 96 bytes at each table address and stops at the first
code or data address (the bytes `c0 00 00 00` or `a8 00 00 00`, at any even offset) in either release: the type tables are packed
one after another, and an address word is the next data object, not part of the table, so it moves
with the relocation of the image whatever the table says. The table's own length is not recorded
anywhere, so the neighbouring tables that precede that address word are still compared.
"""
import argparse
import collections
import csv
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(REPO, 'retools', 'tools'))  # syscall_table

KERNELS = ('unix.IP27', 'unix.IP30', 'unix.IP35')
RELEASES = ('6.5.22', '6.5.30')
ARG_BYTES = 96
COLUMNS = ['kind', 'kernel', 'table', 'key', 'value_6.5.22', 'value_6.5.30', 'detail', 'grade']


def read_tsv(path):
    with open(path, encoding='utf-8') as f:
        return list(csv.DictReader(f, delimiter='\t'))


def split_binary(value):
    rel, kern = value.split('/')
    return rel, kern


def system_calls(rows):
    """{(kernel, table): {release: {number: row}}} from docs/inventory/syscalls.tsv."""
    out = collections.defaultdict(lambda: collections.defaultdict(dict))
    for r in rows:
        rel, kern = split_binary(r['binary'])
        out[(kern, r['table'])][rel][int(r['number'])] = r
    return out


def selectors(rows):
    """{kernel: {release: {selector: row}}} from docs/inventory/selectors.tsv."""
    out = collections.defaultdict(lambda: collections.defaultdict(dict))
    for r in rows:
        rel, kern = split_binary(r['binary'])
        out[kern][rel][int(r['selector'])] = r
    return out


def queue_classes(rows):
    """{(kernel, routine): row} for the changed linked-kernel routines."""
    return {(r['object'], r['routine']): r for r in rows}


ADDRESS_HIGH = (b'\xc0\x00\x00\x00', b'\xa8\x00\x00\x00')


def table_end(data):
    """Offset of the first kernel address (0xc0000000 or 0xa8000000 high bytes) in data, else its length.
    The type tables are packed with 2-byte alignment, so the address can start at any even offset."""
    ends = [i for i in range(0, len(data) - 3, 2) if data[i:i + 4] in ADDRESS_HIGH]
    return ends[0] if ends else len(data)


def byte_offsets(a, b):
    """Offsets of the bytes of a and b that differ, before the first kernel address in either."""
    end = min(table_end(a), table_end(b))
    return [i for i in range(end) if a[i] != b[i]]


def ranges(offs):
    """Compact '0x4-0x7,0x10' text for sorted offsets."""
    out = []
    for o in offs:
        if out and o == out[-1][1] + 1:
            out[-1][1] = o
        else:
            out.append([o, o])
    return ','.join('0x%x' % x if x == y else '0x%x-0x%x' % (x, y) for x, y in out)


def arg_bytes(read, rel, kern, addr):
    if addr in (None, 0):
        return None
    return read(os.path.join(REPO, 'root', rel, kern), addr, ARG_BYTES)


def make_reader():
    import syscall_table
    cache = {}

    def read(path, addr, n):
        if path not in cache:
            data, _elf, secs, _syms, _funcs = syscall_table.load(path)
            cache[path] = (data, secs)
        data, secs = cache[path]
        return syscall_table.read(data, secs, addr, n)
    return read


def diff_system_calls(sc, queue, read):
    rows = []
    for (kern, table), by_rel in sorted(sc.items()):
        if kern not in KERNELS:
            continue
        a, b = by_rel['6.5.22'], by_rel['6.5.30']
        for n in sorted(set(b) - set(a)):
            rows.append(('syscall-added', kern, table, n, '', b[n]['handler'], '', 'established'))
        for n in sorted(set(a) - set(b)):
            rows.append(('syscall-removed', kern, table, n, a[n]['handler'], '', '', 'established'))
        names_a = collections.defaultdict(set)
        names_b = collections.defaultdict(set)
        for n, r in a.items():
            names_a[r['handler']].add(n)
        for n, r in b.items():
            names_b[r['handler']].add(n)
        for name in sorted(set(names_a) | set(names_b)):
            if names_a.get(name) != names_b.get(name):
                rows.append(('syscall-renumbered', kern, table, name,
                             ','.join(str(x) for x in sorted(names_a.get(name, ()))),
                             ','.join(str(x) for x in sorted(names_b.get(name, ()))), '', 'established'))
        for n in sorted(set(a) & set(b)):
            ra, rb = a[n], b[n]
            if ra['handler'] != rb['handler']:
                rows.append(('syscall-handler', kern, table, n, ra['handler'], rb['handler'], '', 'established'))
            if ra['nargs'] != rb['nargs']:
                rows.append(('syscall-nargs', kern, table, n, ra['nargs'], rb['nargs'], '', 'established'))
            if ra['flags'] != rb['flags']:
                rows.append(('syscall-flags', kern, table, n, ra['flags'], rb['flags'], '', 'established'))
            if ra['arg_table'] != '0x0' or rb['arg_table'] != '0x0':
                ca = arg_bytes(read, '6.5.22', kern, int(ra['arg_table'], 16))
                cb = arg_bytes(read, '6.5.30', kern, int(rb['arg_table'], 16))
                if ca is None or cb is None:
                    if ca is not cb:
                        rows.append(('syscall-arg-words', kern, table, n, ra['arg_table'], rb['arg_table'],
                                     'one side has no readable argument table', 'established'))
                elif ca != cb:
                    offs = byte_offsets(ca, cb)
                    if offs:
                        detail = 'bytes at ' + ranges(offs)
                        rows.append(('syscall-arg-words', kern, table, n, ra['arg_table'], rb['arg_table'],
                                     detail, 'established'))
            q = queue.get((kern, ra['handler']))
            if q and q['difference'] != 'layout':
                detail = ('size %s -> %s' % (q['size_6.5.22'], q['size_6.5.30']))
                rows.append(('syscall-handler-body', kern, table, n, q['address_6.5.22'],
                             q['address_6.5.30'], '%s; %s' % (q['difference'], detail), 'class-only'))
    return rows


def diff_selectors(se):
    rows = []
    for kern in KERNELS:
        by_rel = se.get(kern, {})
        a, b = by_rel.get('6.5.22', {}), by_rel.get('6.5.30', {})
        for s in sorted(set(b) - set(a)):
            r = b[s]
            kind = 'shared fill' if r['shared'] else 'own target'
            # a compare tree cannot tell a selector that is new from one the extractor did not narrow
            # in the older image, so only jump-table additions are read from table entries
            grade = 'heuristic' if r['group'] == 'compare' else 'established'
            rows.append(('selector-added', kern, r['group'], s, '', r['target'],
                         '%s; first call %s' % (kind, r['first_call'] or 'none'), grade))
        for s in sorted(set(a) - set(b)):
            rows.append(('selector-removed', kern, a[s]['group'], s, a[s]['target'], '', '', 'established'))
        for s in sorted(set(a) & set(b)):
            ra, rb = a[s], b[s]
            compare = rb['group'] == 'compare'
            if bool(ra['shared']) != bool(rb['shared']):
                rows.append(('selector-fill', kern, rb['group'], s,
                             'shared' if ra['shared'] else 'own target',
                             'shared' if rb['shared'] else 'own target',
                             'compare tree: the address can be a test block ahead of a shared body' if compare else '',
                             'heuristic' if compare else 'established'))
            moved = ra['first_call'].startswith('0x') and rb['first_call'].startswith('0x')
            if ra['first_call'] != rb['first_call'] and not compare and not moved:
                blank = not ra['first_call'] or not rb['first_call']
                rows.append(('selector-first-call', kern, rb['group'], s,
                             ra['first_call'] or 'none', rb['first_call'] or 'none',
                             'none: the block branches before a call' if blank else '', 'heuristic'))
    return rows


def write(path, rows):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        f.write('\t'.join(COLUMNS) + '\n')
        for r in rows:
            f.write('\t'.join(str(x) for x in r) + '\n')


def main():
    out = os.path.join(REPO, 'docs', 'inventory', 'version-table-diff.tsv')
    if '--out' in sys.argv:
        out = sys.argv[sys.argv.index('--out') + 1]
    inv = os.path.join(REPO, 'docs', 'inventory')
    sc = system_calls(read_tsv(os.path.join(inv, 'syscalls.tsv')))
    se = selectors(read_tsv(os.path.join(inv, 'selectors.tsv')))
    queue = queue_classes(read_tsv(os.path.join(REPO, 'docs', 'version-diffs-queue.tsv')))
    rows = diff_system_calls(sc, queue, make_reader()) + diff_selectors(se)
    write(out, rows)
    counts = collections.Counter(r[0] for r in rows)
    for kind in sorted(counts):
        print('%s %d' % (kind, counts[kind]))


if __name__ == '__main__':
    main()
