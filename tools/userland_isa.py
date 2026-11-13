#!/usr/bin/env python3
"""Count MIPS ISA-level instructions in the executable sections of ELF files.

Usage: uv run --with pyelftools --with numpy python -I tools/userland_isa.py MODE ARGS...
  scan OUT.tsv ROOT FILE...         one row per file (FILE paths are relative to ROOT)
  funcs FILE OUT.tsv                per-function rows of a kernel with a symbol table

Every aligned 32-bit big-endian word of each SHF_EXECINSTR section is classified by its encoding.
There is no control-flow recovery, so a data word inside a text section would be counted; the
"scan" mode therefore also reports the words that decode to nothing in MIPS I to IV (these are
the check on that noise), and "funcs" repeats the count per function symbol. The ISA level of a
word is the lowest MIPS ISA that has the encoding: 1, 2, 3 or 4; 5 marks encodings outside MIPS I
to IV (MIPS32 SPECIAL2/SPECIAL3, the Loongson/DSP style opcodes, MIPS V paired single), and 0 an
unassigned word.
"""
import csv
import sys

import numpy as np
from elftools.elf.elffile import ELFFile

COLS = [
    "movz_movn",       # SPECIAL movz/movn
    "movf_movt",       # SPECIAL movci (integer movf/movt), except the word 0x00000001
    "pad_00000001",    # the word 0x00000001, which is movci with every field zero
    "fp_cmov",         # COP1 movf/movt/movz/movn .fmt
    "fp_recip_rsqrt",  # COP1 recip/rsqrt .fmt
    "prefx",           # COP1X prefx
    "pref",            # opcode PREF
    "fp_madd",         # COP1X madd/msub/nmadd/nmsub .s/.d
    "fp_idx_ldst",     # COP1X lwxc1/ldxc1/swxc1/sdxc1/luxc1/suxc1
    "fcc_nonzero",     # c.cond or bc1f/bc1t naming a condition code other than 0
    "branch_likely",   # beql bnel blezl bgtzl bltzl bgezl bltzall bgezall bc1fl bc1tl (MIPS II)
    "ll_sc",           # ll, sc (MIPS II)
    "lld_scd",         # lld, scd (MIPS III)
    "ldc1_sdc1",       # ldc1, sdc1 (MIPS II)
    "fp_sqrt_wround",  # sqrt.fmt, round/trunc/ceil/floor .w (MIPS II)
    "trap_sync",       # teq... tgei... sync (MIPS II)
    "cache_op",        # CACHE (MIPS III)
    "int64",           # doubleword integer ALU, load/store, shifts, ldl/ldr/sdl/sdr, lwu, dmfc1/dmtc1 (MIPS III)
    "fp_long",         # cvt.l, round/trunc/ceil/floor .l, fmt L (MIPS III)
    "mips32_other",    # SPECIAL2, SPECIAL3, jalx, or COP1X outside the MIPS IV set
    "undecoded",       # no meaning in MIPS I to IV
]
HEAD = ["path", "abi", "class", "e_flags", "arch_flag", "text_words"] + COLS + ["lowest_isa"]
ARCH = {0: "mips1", 1: "mips2", 2: "mips3", 3: "mips4", 4: "mips5", 5: "mips32", 6: "mips64"}


def abi_of(h):
    if h["e_ident"]["EI_CLASS"] == "ELFCLASS64":
        return "n64"
    return "n32" if h["e_flags"] & 0x20 else "o32"


def classify(w):
    """w: uint32 array. Returns (dict column -> boolean mask, level array 0..5)."""
    op = w >> 26
    rs = (w >> 21) & 31
    rt = (w >> 16) & 31
    fn = w & 63
    sp = op == 0
    ri = op == 1
    c0 = op == 0x10
    c1 = op == 0x11
    cx = op == 0x13
    m = {}
    m["movz_movn"] = sp & ((fn == 0x0A) | (fn == 0x0B))
    # movci: bit 17 clear (tf is bit 16) and the shift field zero
    # The word 0x00000001 (movf $0,$0,$fcc0) fills gaps between routines; counted apart, not as a use.
    m["pad_00000001"] = w == 1
    m["movf_movt"] = sp & (fn == 1) & (((w >> 6) & 31) == 0) & ((rt & 2) == 0) & (w != 1)
    fpw = c1 & ((rs == 16) | (rs == 17))
    m["fp_cmov"] = fpw & ((fn == 0x11) | (fn == 0x12) | (fn == 0x13))
    m["fp_recip_rsqrt"] = fpw & ((fn == 0x15) | (fn == 0x16))
    m["prefx"] = cx & (fn == 0x0F)
    m["pref"] = op == 0x33
    m["fp_madd"] = cx & np.isin(fn, [0x20, 0x21, 0x28, 0x29, 0x30, 0x31, 0x38, 0x39])
    m["fp_idx_ldst"] = cx & np.isin(fn, [0, 1, 8, 9])
    bc = c1 & (rs == 8)
    cond = fpw & (fn >= 0x30)
    m["fcc_nonzero"] = (bc & (((w >> 18) & 7) != 0)) | (cond & (((w >> 8) & 7) != 0))
    m["branch_likely"] = ((op >= 0x14) & (op <= 0x17)) | (ri & np.isin(rt, [2, 3, 0x12, 0x13])) | (bc & ((rt & 3) >= 2))
    m["ll_sc"] = (op == 0x30) | (op == 0x38)
    m["lld_scd"] = (op == 0x34) | (op == 0x3C)
    m["ldc1_sdc1"] = (op == 0x35) | (op == 0x3D)
    m["fp_sqrt_wround"] = fpw & ((fn == 4) | ((fn >= 0x0C) & (fn <= 0x0F)))
    m["trap_sync"] = (sp & np.isin(fn, [0x0F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x36])) | (ri & np.isin(rt, [8, 9, 0xA, 0xB, 0xC, 0xE]))
    m["cache_op"] = op == 0x2F
    sp64 = sp & np.isin(fn, [0x14, 0x16, 0x17, 0x1C, 0x1D, 0x1E, 0x1F, 0x2C, 0x2D, 0x2E, 0x2F, 0x38, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F])
    ops64 = np.isin(op, [0x18, 0x19, 0x1A, 0x1B, 0x27, 0x2C, 0x2D, 0x34, 0x37, 0x3C, 0x3F])
    cp64 = (c1 & ((rs == 1) | (rs == 5))) | (c0 & ((rs == 1) | (rs == 5)))
    m["int64"] = sp64 | ops64 | cp64
    m["fp_long"] = (c1 & (rs == 21)) | (fpw & (((fn >= 8) & (fn <= 0x0B)) | (fn == 0x25)))
    m["mips32_other"] = np.isin(op, [0x1C, 0x1D, 0x1F]) | (c1 & (rs == 22)) \
        | (cx & ~(m["prefx"] | m["fp_madd"] | m["fp_idx_ldst"]))
    sp_ok = [0, 1, 2, 3, 4, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x16, 0x17, 0x18, 0x19,
             0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2A, 0x2B, 0x2C,
             0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x36, 0x38, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F]
    m["undecoded"] = (op == 0x1E) | (op == 0x3B) | (sp & ~np.isin(fn, sp_ok)) \
        | (ri & ~np.isin(rt, [0, 1, 2, 3, 8, 9, 0xA, 0xB, 0xC, 0xE, 0x10, 0x11, 0x12, 0x13]))
    lvl = np.ones(len(w), dtype=np.uint8)
    for k in ("branch_likely", "ll_sc", "ldc1_sdc1", "fp_sqrt_wround", "trap_sync"):
        lvl[m[k]] = np.maximum(lvl[m[k]], 2)
    for k in ("int64", "fp_long", "cache_op"):
        lvl[m[k]] = np.maximum(lvl[m[k]], 3)
    for k in ("movz_movn", "movf_movt", "fp_cmov", "fp_recip_rsqrt", "prefx", "pref", "fp_madd", "fp_idx_ldst", "fcc_nonzero"):
        lvl[m[k]] = 4
    lvl[m["mips32_other"]] = 5
    lvl[m["undecoded"]] = 0
    return m, lvl


def text_words(f):
    out = []
    for s in f.iter_sections():
        if s["sh_flags"] & 4 and s["sh_type"] == "SHT_PROGBITS" and s["sh_size"] >= 4:
            d = s.data()
            n = len(d) // 4
            out.append((s, np.frombuffer(d[: n * 4], dtype=">u4").astype(np.uint32)))
    return out


def scan_file(path, rel):
    f = ELFFile(open(path, "rb"))
    h = f.header
    words = text_words(f)
    tot = {c: 0 for c in COLS}
    nwords = 0
    lowest = 1
    for _, w in words:
        m, lvl = classify(w)
        nwords += len(w)
        for c in COLS:
            tot[c] += int(m[c].sum())
        ok = lvl[(lvl >= 1) & (lvl <= 4)]
        if len(ok):
            lowest = max(lowest, int(ok.max()))
    return [rel, abi_of(h), h["e_ident"]["EI_CLASS"][8:], hex(h["e_flags"]),
            ARCH.get((h["e_flags"] >> 28) & 15, "?"), nwords] + [tot[c] for c in COLS] + [lowest]


def main():
    mode = sys.argv[1]
    if mode == "scan":
        out, root = sys.argv[2:4]
        files = sys.argv[4:]
        with open(out, "w", newline="") as fh:
            w = csv.writer(fh, delimiter="\t", lineterminator="\n")
            w.writerow(HEAD)
            for rel in files:
                try:
                    w.writerow(scan_file(f"{root}/{rel}", rel))
                except Exception as e:  # unreadable or not an ELF with sections
                    print("skip", rel, e, file=sys.stderr)
    elif mode == "funcs":
        path, out = sys.argv[2:4]
        f = ELFFile(open(path, "rb"))
        text = f.get_section_by_name(".text")
        base = text["sh_addr"]
        words = np.frombuffer(text.data()[: len(text.data()) // 4 * 4], dtype=">u4").astype(np.uint32)
        m, lvl = classify(words)
        syms = sorted((s["st_value"], s["st_size"], s.name) for s in f.get_section_by_name(".symtab").iter_symbols()
                      if s["st_info"]["type"] == "STT_FUNC" and s["st_size"] > 0 and base <= s["st_value"] < base + text["sh_size"])
        covered = np.zeros(len(words), dtype=bool)
        with open(out, "w", newline="") as fh:
            w = csv.writer(fh, delimiter="\t", lineterminator="\n")
            w.writerow(["function", "address", "words"] + COLS)
            for addr, size, name in syms:
                a = (addr - base) // 4
                b = a + size // 4
                covered[a:b] = True
                row = [int(m[c][a:b].sum()) for c in COLS]
                w.writerow([name, hex(addr), b - a] + row)
            rest = ~covered
            w.writerow(["(outside sized function symbols)", "", int(rest.sum())] + [int((m[c] & rest).sum()) for c in COLS])


if __name__ == "__main__":
    main()
