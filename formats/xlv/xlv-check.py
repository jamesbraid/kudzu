#!/usr/bin/env -S uv run --script
# /// script
# dependencies = ["pyelftools", "capstone"]
# ///
"""Re-derive the XLV on-disk facts in xlv.md from the shipped 6.5.30 binaries.

usage: xlv-check.py MEDIA_ROOT

  MEDIA_ROOT  the extracted IRIX 6.5.30 media root (the directory that holds
              sbin/, usr/ and lib32/), e.g. refs/irix-media/6.5.30

Needs: usr/cpu/sysgen/IP30boot/xlv.a, sbin/xlv_labd, usr/lib32/libdisk.so
(the IP27, IP32 and IP35 archives carry the same routines).

Each check disassembles one routine with Capstone and asserts that the
instructions the document cites are present. Exits non-zero on the first
mismatch group. Prints one line per check.
"""
import io
import struct
import sys

from capstone import (CS_ARCH_MIPS, CS_MODE_BIG_ENDIAN, CS_MODE_MIPS64, Cs,
                      CS_OP_IMM, CS_OP_MEM)
from elftools.elf.elffile import ELFFile

root = sys.argv[1].rstrip("/")
fails = 0


def ok(cond, msg):
    global fails
    print(("ok   " if cond else "FAIL ") + msg)
    fails += 0 if cond else 1


def ar_member(path, name):
    d = open(path, "rb").read()
    assert d[:8] == b"!<arch>\n"
    p = 8
    while p < len(d):
        h = d[p:p + 60]
        n = h[:16].decode().strip().rstrip("/")
        sz = int(h[48:58])
        if n == name:
            return d[p + 60:p + 60 + sz]
        p += 60 + sz + (sz & 1)
    raise KeyError(name)


md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS64 | CS_MODE_BIG_ENDIAN)
md.detail = True
md.skipdata = True


def insns(code, base):
    return [i for i in md.disasm(code, base) if i.mnemonic != ".byte"]


def facts(code, base):
    """Return (displacements used by loads/stores, immediates, mnemonics)."""
    disp, imm, mn = set(), set(), []
    for i in insns(code, base):
        mn.append(i.mnemonic)
        for o in i.operands:
            if o.type == CS_OP_MEM:
                disp.add(o.mem.disp)
            elif o.type == CS_OP_IMM:
                imm.add(o.imm)
        # lui+ori/addiu constants are rebuilt below; keep raw immediates only
    return disp, imm, mn


def rebuilt_constants(code, base):
    """Constants built with lui + ori/addiu into the same register."""
    out, hi = set(), {}
    for i in insns(code, base):
        ops = i.op_str.replace(" ", "").split(",")
        if i.mnemonic == "lui":
            hi[ops[0]] = int(ops[1], 0) << 16
        elif i.mnemonic in ("ori", "addiu") and ops[0] == ops[1] and ops[0] in hi:
            v = (hi[ops[0]] | int(ops[2], 0)) if i.mnemonic == "ori" else hi[ops[0]] + int(ops[2], 0)
            out.add(v & 0xffffffff)
    return out


def elf_text(data):
    e = ELFFile(io.BytesIO(data))
    t = e.get_section_by_name(".text")
    return e, t.data(), t["sh_addr"]


def func(e, text, base, name):
    for s in e.get_section_by_name(".symtab").iter_symbols():
        if s.name == name and s["st_info"]["type"] == "STT_FUNC":
            a, n = s["st_value"], s["st_size"]
            return text[a - base:a - base + n], a
    raise KeyError(name)


# --- 1. kernel module, IP30 archive, member xlv_root.o -------------------------------
ar = f"{root}/usr/cpu/sysgen/IP30boot/xlv.a"
e, text, base = elf_text(ar_member(ar, "xlv_root.o"))

code, a = func(e, text, base, "xlv_get_current_label")
d, imm, _ = facts(code, a)
k = rebuilt_constants(code, a)
ok(0x11201989 in k, "kernel xlv_get_current_label compares the label magic 0x11201989")
ok({0x200, 0x12a0} <= imm, "kernel xlv_get_current_label rounds 0x200 and 0x12a0")
ok(1 in imm and 0 in d, "kernel xlv_get_current_label tests bit 0 of the 8-byte word at the buffer start")

code, a = func(e, text, base, "xlv_get_xlv_label")
d, imm, _ = facts(code, a)
k = rebuilt_constants(code, a)
ok(0xbe5a941 in k, "kernel xlv_get_xlv_label checks the volume-header magic 0x0be5a941")
ok(0x28 in d, "kernel xlv_get_xlv_label reads the sector size at volume-header +0x28")
ok(0x48 in imm and 0xf in imm, "kernel xlv_get_xlv_label walks 15 directory entries from +0x48")
ok(0x50 in d, "kernel xlv_get_xlv_label reads the entry block number at directory +8 (header +0x50)")
ok(0x200 in imm and 0x12a0 in imm, "kernel xlv_get_xlv_label sizes the read 0x200 + 2 * 0x12a0 (rounded)")

code, a = func(e, text, base, "xlv_scan_invent")
d, imm, _ = facts(code, a)
ok({0x120, 0x124, 0x164, 0x190, 0x194, 0x218} <= (d | imm),
   "kernel xlv_scan_invent reads flags/uuid/name/subvolume-type/part-uuid at the entry offsets")
ok(0x10 in imm, "kernel xlv_scan_invent loops over 16 partition entries")

code, a = func(e, text, base, "xlv_add_plex")
d, imm, _ = facts(code, a)
want = {0xa9, 0xaa, 0xac, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe8, 0xf0, 0x10c, 0x110}
ok(want <= d, "kernel xlv_add_plex reads plex/ve/part fields at entry +0xa9..+0x110: missing "
   + str(sorted(want - d)))
ok({0x78, 0x88, 0xb0, 0xf8} <= (d | imm), "kernel xlv_add_plex copies the plex uuid/name, ve uuid, part uuid")

# --- 2. userland: xlv_labd (stripped, n32, VAs fixed for 6.5.30) ---------------------
labd = open(f"{root}/sbin/xlv_labd", "rb").read()
e, text, base = elf_text(labd)


def at(lo, hi):
    return text[lo - base:hi - base], lo


code, a = at(0x10007374, 0x1000798c)  # label printer
d, imm, _ = facts(code, a)
want = {0, 4, 6, 7, 8, 0x10, 0x18, 0x120, 0x18c, 0x194, 0x196, 0x1a8, 0x1c8, 0x1c9, 0x1ca, 0x1cc,
        0x1e0, 0x200, 0x201, 0x202, 0x203, 0x204, 0x208, 0x210, 0x218, 0x228, 0x22c, 0x230}
ok(want <= (d | imm), "labd label printer reads the header and entry fields: missing " + str(sorted(want - (d | imm))))
ok(0x118 in imm and 0x10 in imm, "labd label printer steps 16 entries of 0x118 bytes")

code, a = at(0x10006318, 0x10006680)  # entry builder
d, imm, _ = facts(code, a)
want = {0, 0x64, 0x68, 0x6c, 0x70, 0x74, 0x75, 0x76, 0xa8, 0xa9, 0xaa, 0xac, 0xe0, 0xe1, 0xe2, 0xe3,
        0xe4, 0xe8, 0xf0, 0x108, 0x10c, 0x110, 0x111}
ok(want <= d, "labd entry builder stores the entry fields: missing " + str(sorted(want - d)))
ok({0x4, 0x14, 0x24, 0x34, 0x44, 0x78, 0x88, 0xb0, 0xc0, 0xf8} <= (d | imm),
   "labd entry builder places the four volume uuids, the names and the plex/ve/part uuids")

code, a = at(0x100060fc, 0x10006318)  # label writer: header init
k = rebuilt_constants(code, a)
d, imm, _ = facts(code, a)
ok(0x11201989 in k and 2 in imm and 0x10 in d and 6 in d, "labd label writer sets magic, version 2, trans id at +0x10, part count at +6")

code, a = at(0x10007c24, 0x10007f8c)  # lab1_open
d, imm, _ = facts(code, a)
ok({0x430, 0x450, 0x458, 0x614, 0x618, 0x61c} <= (d | imm) and {0x200, 0x12a0} <= imm,
   "labd label file open: finds the directory entry, reads 0x200 + 2*0x12a0 (rounded)")
sr = ELFFile(io.BytesIO(labd)).get_section_by_name(".srdata").data()
ok(b"xlvlab\0" in sr, "labd .srdata holds the directory-entry name 'xlvlab'")

code, a = at(0x100083f0, 0x100085b8)  # lab1_write
d, imm, _ = facts(code, a)
ok(0x10 in imm and 0x118 in imm and 1 in imm and 0x228 in d,
   "labd label write picks the slot by bit 0, and stores 0x10|partition in the dev field (+0x228)")

code, a = at(0x10008340, 0x100083f0)  # write_master_blk
d, imm, _ = facts(code, a)
ok({0x610, 0x60c, 0x614} <= d, "labd master-block write writes one sector at the label-file start")

# --- 3. libdisk -----------------------------------------------------------------------
ld = open(f"{root}/usr/lib32/libdisk.so", "rb").read()
e = ELFFile(io.BytesIO(ld))
sec = {s.name: s for s in e.iter_sections()}
dat = sec[".data"]
img = ld
words = struct.unpack(">%dI" % (dat["sh_size"] // 4), img[dat["sh_offset"]:dat["sh_offset"] + dat["sh_size"]])
names = {}
for i in range(0, 30, 2):
    p, t = words[i], words[i + 1]
    for s in (sec[".srdata"], sec[".rodata"]):
        if s["sh_addr"] <= p < s["sh_addr"] + s["sh_size"]:
            o = s["sh_offset"] + p - s["sh_addr"]
            names[img[o:img.index(b"\0", o)].decode()] = t
ok(names.get("xfs") == 10 and names.get("xfslog") == 11 and names.get("xlv") == 12 and names.get("xvm") == 13,
   "libdisk partition-type table: xfs=10 xfslog=11 xlv=12 xvm=13 (found %s)" % {k: v for k, v in names.items() if k})
text_s = sec[".text"]
code = text_s.data()
off = 0xc00510c - text_s["sh_addr"]
i = insns(code[off:off + 0x28], 0xc00510c)
ok(any(x.mnemonic == "addiu" and x.op_str.endswith("0x80") for x in i),
   "libdisk vhchksum sums 128 words (volume-header checksum)")

print("%d failure(s)" % fails)
sys.exit(1 if fails else 0)
