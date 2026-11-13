#!/usr/bin/env -S uv run --script
# /// script
# dependencies = ["pyelftools", "capstone"]
# ///
"""Re-derive the XVM on-disk facts in xvm.md from shipped binaries.

usage: xvm-check.py IRIX_XVM_A_MEMBER_OBJECT LINUX_SGI_XVM_KO

  IRIX_XVM_A_MEMBER_OBJECT  xvm_label.o extracted from a 6.5.30 xvm.a
  LINUX_SGI_XVM_KO          sgi-xvm.ko from a ProPack 4 debug kernel module package

Exits non-zero on the first mismatch. Prints one line per check.
"""
import struct, sys
from elftools.elf.elffile import ELFFile
from capstone import Cs, CS_ARCH_MIPS, CS_MODE_MIPS64, CS_MODE_BIG_ENDIAN

irix_o, linux_ko = sys.argv[1:3]
fails = 0

def ok(cond, msg):
    global fails
    print(("ok   " if cond else "FAIL ") + msg)
    fails += 0 if cond else 1

# 1. Linux byte-swap tables: widths must sum to the on-disk struct sizes.
e = ELFFile(open(linux_ko, "rb"))
data = e.get_section_by_name(".data").data()
si = e.get_section_index(".data")
syms = {s.name: (s["st_value"], s["st_size"])
        for s in e.get_section_by_name(".symtab").iter_symbols()
        if s["st_shndx"] == si and s.name.endswith("_endian")}

def table(name):
    a, sz = syms[name]
    v = [struct.unpack_from("<q", data, a + 8 * i)[0] for i in range(sz // 8)]
    assert v[-1] == 0
    return v[:-1]

def width(name):
    v, w, i = table(name), 0, 0
    while i < len(v):
        if v[i] < 0:
            w += v[i + 1]; i += 2        # opaque run: -n, length
        else:
            w += v[i]; i += 1            # one swapped item of that width
    return w

for name, size in [("xvm_physlab_endian", 124), ("xvm_labelhdr_endian", 8),
                   ("xvm_disk_attr_endian", 24), ("xvm_dau_perm_endian", 48),
                   ("xvm_dau_name_endian", 48)]:
    ok(width(name) == size, f"{name} covers {size} bytes (got {width(name)})")
# The ve and attr tables cover the fixed head only (56 and 24 bytes); the
# 16-byte ve union and the 48-byte attr union have their own tables.
ok(width("xvm_disk_ve_endian") == 56, "xvm_disk_ve_endian covers the 56-byte head")
for u in ("slice", "concat", "stripe", "raid", "mirror", "subvol", "vol", "cow"):
    ok(width(f"xvm_veu_{u}_endian") == 16, f"xvm_veu_{u}_endian covers 16 bytes")
ok(table("xvm_physlab_endian") ==
   [4, 2, 1, 1, -1, 32, -1, 16, 2, 2, 2, 2, 4, 4, -1, 48, 4],
   "physlab field-width list")

# 2. IRIX: label header initialiser writes magic 0x6c646872, version 2.
st = e.__class__(open(irix_o, "rb"))
t = st.get_section_by_name(".text").data()
sym = {s.name: (s["st_value"], s["st_size"])
       for s in st.get_section_by_name(".symtab").iter_symbols()
       if s["st_info"]["type"] == "STT_FUNC"}
md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS64 | CS_MODE_BIG_ENDIAN)

def dis(name):
    a, sz = sym[name]
    return [(i.address, i.mnemonic, i.op_str) for i in md.disasm(t[a:a + sz], a)]

ins = dis("xvm_labent_init")
ok(any(m == "lui" and o.endswith("0x6c64") for _, m, o in ins) and
   any(m == "ori" and o.endswith("0x6872") for _, m, o in ins),
   "xvm_labent_init stores label header magic 0x6c646872")
ok(any(m == "addiu" and o.endswith(", 2") for _, m, o in ins) and
   any(m == "sh" and o.endswith("4($a0)") for _, m, o in ins),
   "xvm_labent_init stores version 2 at +4 (halfword)")

# 3. IRIX mem2dsk_ve stores: type +0, state +1, pieces +2, tid +4, uuid +8,
#    child uuid +0x18, length +0x28, position +0x30.
ins = dis("xvm_mem2dsk_ve")
stores = {(m, o.split(",")[1].strip()) for _, m, o in ins if m in ("sb", "sh", "sw", "sd")}
for m, off in [("sb", "($t0)"), ("sw", "4($t0)"), ("sh", "0x30($t0)"),
               ("sd", "0x28($t0)")]:
    ok(any(mm == m and off in oo for mm, oo in stores) or
       any(mm == m and off.replace("$t0", "$a4") in oo for mm, oo in stores) or
       any(mm == m and off.replace("$t0", "$s0") in oo for mm, oo in stores),
       f"xvm_mem2dsk_ve: {m} at {off}")
sys.exit(1 if fails else 0)
