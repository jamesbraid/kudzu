#!/usr/bin/env python3
"""Tests of the release comparison in version_table_diff.py. They need no kernel image.

Run from the repository root: python3 scripts/test_version_table_diff.py
"""
import os
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import version_table_diff as vtd  # noqa: E402


def sel(rel, sel_no, target, first_call='', shared=''):
    return {'binary': '%s/unix.IP27' % rel, 'group': 'jumptable 0x1', 'selector': str(sel_no),
            'target': target, 'first_call': first_call, 'shared': shared}


class Selectors(unittest.TestCase):
    def test_added_removed_fill_and_first_call(self):
        rows = [sel('6.5.22', 1, '0x10', 'cap_able', 'yes'),
                sel('6.5.22', 2, '0x20', 'copyin', ''),
                sel('6.5.30', 1, '0x11', 'cap_able', ''),
                sel('6.5.30', 2, '0x21', 'copyout', ''),
                sel('6.5.30', 3, '0x30', '', 'yes')]
        out = vtd.diff_selectors(vtd.selectors(rows))
        kinds = sorted((r[0], r[3]) for r in out)
        self.assertEqual(kinds, [('selector-added', 3), ('selector-fill', 1),
                                 ('selector-first-call', 2)])
        fill = [r for r in out if r[0] == 'selector-fill'][0]
        self.assertEqual((fill[4], fill[5]), ('shared', 'own target'))
        first = [r for r in out if r[0] == 'selector-first-call'][0]
        self.assertEqual(first[7], 'heuristic')


class ArgumentBytes(unittest.TestCase):
    def test_address_ends_the_comparison(self):
        a = bytes(8) + b'\xc0\x00\x00\x00\x00\x20\x05\xf4' + bytes(8)
        b = bytes(8) + b'\xc0\x00\x00\x00\x00\x20\x52\x0c' + b'sgimips\x00'
        self.assertEqual(vtd.byte_offsets(a, b), [])

    def test_bytes_before_the_address_are_compared(self):
        a = b'\x25\x00\x11\x00' + b'\xc0\x00\x00\x00\x00\x20\x05\xf4'
        b = b'\x25\x00\x12\x00' + b'\xc0\x00\x00\x00\x00\x20\x52\x0c'
        self.assertEqual(vtd.byte_offsets(a, b), [2])

    def test_address_at_an_even_offset_inside_a_word(self):
        # a table that starts 2 bytes into an 8-byte slot puts its neighbour's address at offset 6
        a = bytes(6) + b'\xc0\x00\x00\x00\x00\x27\x71\xc4'
        b = bytes(6) + b'\xc0\x00\x00\x00\x00\x2b\x53\xd8'
        self.assertEqual(vtd.byte_offsets(a, b), [])

    def test_ranges(self):
        self.assertEqual(vtd.ranges([4, 5, 6, 7, 16]), '0x4-0x7,0x10')


class FirstCall(unittest.TestCase):
    def test_compare_trees_and_moved_addresses_are_not_compared(self):
        def row(rel, sel_no, call, group):
            r = sel(rel, sel_no, '0x10', call)
            r['group'] = group
            return r
        rows = [row('6.5.22', 1, 'copyin', 'compare'), row('6.5.30', 1, '', 'compare'),
                row('6.5.22', 2, '0x100', 'jumptable 0x1'), row('6.5.30', 2, '0x200', 'jumptable 0x1'),
                row('6.5.22', 3, '', 'jumptable 0x1'), row('6.5.30', 3, 'cap_able', 'jumptable 0x1')]
        out = vtd.diff_selectors(vtd.selectors(rows))
        self.assertEqual([(r[0], r[3]) for r in out], [('selector-first-call', 3)])


if __name__ == '__main__':
    unittest.main()
