#!/usr/bin/env python3
"""Resolve the gp-relative operands of routines that differ between the 6.5.22 and 6.5.30
linked kernels only in those operands, and compare what each slot names.

Reads evidence/version-diffs-queue.tsv (rows of scope hardware-facing, class layout, kinds gp or gp,jump)
and disassembles each routine in both kernels with capstone, pairs the instructions by position
and, at every pair whose gp offsets differ, resolves gp + offset in each kernel:

  variable   a symbol (data or bss) at that address
  pool       an 8-byte word in a literal or small-data section that holds an address; the
             word's own symbol, or the symbol or string the address points to
  constant   an 8-byte word that is not an address
  unknown    anything else

Output is evidence/version-diffs-gp-slots.tsv. Run: tools/gp_slots.py (inside retools/tools/re-run).
"""
import bisect
import csv
import struct
import sys

import capstone
from elftools.elf.elffile import ELFFile

QUEUE = 'evidence/version-diffs-queue.tsv'
OUT = 'evidence/version-diffs-gp-slots.tsv'
POOL_SECTIONS = ('.srdata', '.sdata', '.lit8', '.lit4', '.rodata', '.data')


class Kernel:
    def __init__(self, path):
        self.f = open(path, 'rb')
        self.e = ELFFile(self.f)
        self.secs = []
        for s in self.e.iter_sections():
            if s['sh_addr'] and s['sh_type'] in ('SHT_PROGBITS', 'SHT_NOBITS'):
                self.secs.append((s['sh_addr'], s['sh_size'], s.name, s))
        self.secs.sort(key=lambda x: x[0])
        self.starts = [x[0] for x in self.secs]
        self.gp = self.find_gp()
        syms = []
        st = self.e.get_section_by_name('.symtab')
        for s in st.iter_symbols():
            if s.name and s['st_value'] and s['st_info']['type'] in ('STT_OBJECT', 'STT_NOTYPE', 'STT_FUNC', 'STT_COMMON'):
                syms.append((s['st_value'], s['st_size'], s.name, s['st_info']['type']))
        syms.sort()
        self.syms = syms
        self.sym_starts = [x[0] for x in syms]
        self.cache = {}
        self.cs = capstone.Cs(capstone.CS_ARCH_MIPS, capstone.CS_MODE_MIPS64 | capstone.CS_MODE_BIG_ENDIAN)
        self.cs.detail = False

    def find_gp(self):
        op = self.e.get_section_by_name('.MIPS.options')
        d = op.data()
        p = 0
        gp = None
        while p + 8 <= len(d):
            kind, size = d[p], d[p + 1]
            if size == 0:
                break
            if kind == 1 and size >= 40:
                gp = struct.unpack('>Q', d[p + 32:p + 40])[0]
            p += size
        if not gp:
            raise SystemExit('no gp value')
        return gp

    def section_of(self, a):
        i = bisect.bisect_right(self.starts, a) - 1
        if i >= 0 and a < self.secs[i][0] + self.secs[i][1]:
            return self.secs[i]
        return None

    def read(self, a, n):
        sec = self.section_of(a)
        if sec is None or sec[3]['sh_type'] == 'SHT_NOBITS' or a + n > sec[0] + sec[1]:
            return None
        key = sec[0]
        if key not in self.cache:
            self.cache[key] = sec[3].data()
        o = a - sec[0]
        return self.cache[key][o:o + n]

    def sym_at(self, a):
        """Name of the symbol that starts at a or contains it, as name or name+off."""
        i = bisect.bisect_right(self.sym_starts, a) - 1
        best = None
        while i >= 0 and a - self.sym_starts[i] < 0x10000:
            v, sz, name, ty = self.syms[i]
            if v == a:
                return name
            if sz and v <= a < v + sz and best is None:
                best = '%s+%d' % (name, a - v)
            i -= 1
            if best:
                break
        return best

    def cstring(self, a):
        d = self.read(a, 96)
        if not d:
            return None
        s = d.split(b'\0')[0]
        if len(s) >= 3 and all(32 <= c < 127 or c in (9, 10) for c in s):
            return s.decode('ascii')
        return None

    def describe(self, off, width=8):
        a = self.gp + off
        sec = self.section_of(a)
        sname = sec[2] if sec else '?'
        name = self.sym_at(a)
        raw = self.read(a, 8)
        val = struct.unpack('>Q', raw)[0] if raw else None
        if width == 4:
            if val is not None and not name:
                return ('constant', '32-bit word 0x%x' % (val >> 32))
            val = None
        pointer = None
        if val is not None and self.section_of(val):
            vs = self.section_of(val)
            if vs[2] == '.rodata' or vs[2].startswith('.rodata'):
                st = self.cstring(val)
                if st is not None:
                    pointer = 'string "%s"' % st.replace('\t', ' ').replace('\n', '\\n')
            if pointer is None:
                n = self.sym_at(val)
                if n:
                    pointer = n
                else:
                    pointer = 'address in %s: %s' % (vs[2], self.shape(val, vs[2]))
        if sec is None:
            return ('unknown', 'no section at 0x%x' % a)
        if sname in ('.sbss', '.bss') or sec[3]['sh_type'] == 'SHT_NOBITS':
            return ('variable', name or 'unnamed 0x%x' % a)
        if name and not name.startswith(('maddrnd', 'EXP_MASK')) and sname != '.srdata':
            return ('variable', name)
        if pointer:
            return ('pool', pointer)
        if val is not None:
            return ('constant', '0x%x' % val)
        return ('unknown', name or '0x%x' % a)

    def shape(self, a, secname):
        """The first 32 bytes at a with address-like words and call targets masked, as hex."""
        d = self.read(a, 32)
        if not d:
            return 'no bytes'
        if secname.startswith('.text'):
            ws = [struct.unpack('>I', d[i:i + 4])[0] for i in range(0, 32, 4)]
            ws = [w & 0xfc000000 if (w >> 26) in (2, 3) else (w & 0xffff0000 if (w >> 21) & 31 == 28 else w) for w in ws]
            return ''.join('%08x' % w for w in ws)
        out = []
        for i in range(0, 32, 8):
            w = struct.unpack('>Q', d[i:i + 8])[0]
            out.append('PTR' if (w >> 48) in (0xc000, 0xa800) and w & 0xffffffffffff else '%016x' % w)
        return ' '.join(out)

    def insns(self, addr, size):
        d = self.read(addr, size)
        return list(self.cs.disasm(d, addr))


def gp_operand(ins):
    """gp-relative offset used by the instruction, or None."""
    op = ins.op_str.replace('$', '')
    if op.replace(' ', '').startswith('gp,'):
        return None  # writes gp itself (the kernel's own gp value is being loaded)
    if '(gp)' in op:
        try:
            return int(op.split(',')[-1].split('(')[0].strip(), 0)
        except ValueError:
            return None
    if ins.mnemonic in ('daddiu', 'addiu') and op.replace(' ', '').split(',')[1:2] == ['gp']:
        try:
            return int(op.replace(' ', '').split(',')[2], 0)
        except (ValueError, IndexError):
            return None
    return None


def derived_strings(kern, ins_list, idx):
    """Strings at base + immediate for the daddiu uses of a pool word loaded at ins_list[idx]."""
    ins = ins_list[idx]
    if ins.mnemonic != 'ld':
        return None
    reg = ins.op_str.replace('$', '').split(',')[0].strip()
    off = gp_operand(ins)
    raw = kern.read(kern.gp + off, 8)
    if not raw:
        return None
    base = struct.unpack('>Q', raw)[0]
    found = []
    for nxt in ins_list[idx + 1: idx + 16]:
        parts = nxt.op_str.replace('$', '').replace(' ', '').split(',')
        if nxt.mnemonic == 'daddiu' and len(parts) == 3 and parts[1] == reg:
            try:
                t = kern.cstring(base + int(parts[2], 0))
            except ValueError:
                t = None
            if t:
                found.append(t)
        if parts and parts[0] == reg and nxt.mnemonic not in ('daddiu', 'sd', 'sw'):
            break
    return found


def main():
    kern = {}
    rows = []
    with open(QUEUE, encoding='utf-8') as f:
        for r in csv.DictReader(f, delimiter='\t'):
            if r['scope'] == 'hardware-facing' and r['difference'] == 'layout' and r['lui_gp_jump_kinds'] in ('gp', 'gp,jump'):
                rows.append(r)
    out = []
    for r in rows:
        plat = r['object'].split('.')[-1]
        for rel in ('6.5.30', '6.5.22'):
            if (rel, r['object']) not in kern:
                kern[(rel, r['object'])] = Kernel('root/%s/%s' % (rel, r['object']))
        k30, k22 = kern[('6.5.30', r['object'])], kern[('6.5.22', r['object'])]
        a30, a22 = int(r['address_6.5.30'], 16), int(r['address_6.5.22'], 16)
        n = int(r['size_6.5.30'])
        i30, i22 = k30.insns(a30, n), k22.insns(a22, n)
        if len(i30) != len(i22):
            out.append((plat, r['routine'], r['address_6.5.30'], r['address_6.5.22'], '', '', '', '', '', '', 'length differs'))
            continue
        seen = set()
        for pos, (x, y) in enumerate(zip(i30, i22)):
            ox, oy = gp_operand(x), gp_operand(y)
            if ox is None and oy is None:
                continue
            if ox is None or oy is None or x.mnemonic != y.mnemonic:
                out.append((plat, r['routine'], r['address_6.5.30'], r['address_6.5.22'], '0x%x' % x.address, x.mnemonic, str(ox), str(oy), '', '', 'instruction differs'))
                continue
            if (ox, oy) in seen:
                continue
            seen.add((ox, oy))
            wd = 4 if x.mnemonic in ('lw', 'lwu', 'sw') else 8
            c30, t30 = k30.describe(ox, wd)
            c22, t22 = k22.describe(oy, wd)
            if c30 == 'unknown' or c22 == 'unknown':
                v = 'unresolved'
            elif t30 == t22 and t30.startswith('address in'):
                v = 'same (unnamed target, the first 32 bytes are equal with call targets and gp offsets masked)'
            elif t30 == t22 or (t30.split('+')[0] == t22.split('+')[0] and c30 == c22 == 'variable'):
                v = 'same'
            elif c30 == c22 == 'variable' and t30.startswith('unnamed') and t22.startswith('unnamed'):
                v = 'unnamed in both (no symbol)'
            else:
                v = 'differs'
                d30, d22 = derived_strings(k30, i30, pos), derived_strings(k22, i22, pos)
                if d30 and d30 == d22:
                    v = 'same (strings at the offsets used: %s)' % ' | '.join(x[:30].replace('\n', '\\n') for x in d30[:2])
            out.append((plat, r['routine'], r['address_6.5.30'], r['address_6.5.22'], '0x%x' % x.address,
                        x.mnemonic, '%d' % ox, '%d' % oy, '%s: %s' % (c30, t30), '%s: %s' % (c22, t22), v))
    with open(OUT, 'w', encoding='utf-8', newline='\n') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(['platform', 'routine', 'address_6.5.30', 'address_6.5.22', 'insn_6.5.30', 'mnemonic',
                    'gp_offset_6.5.30', 'gp_offset_6.5.22', 'slot_6.5.30', 'slot_6.5.22', 'verdict'])
        w.writerows(out)
    from collections import Counter
    print(Counter(x[-1] for x in out), len(rows), 'routines')


if __name__ == '__main__':
    sys.exit(main())
