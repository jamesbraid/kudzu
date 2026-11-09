#!/usr/bin/env -S uv run --script
# /// script
# dependencies = ["pyelftools", "capstone"]
# ///
"""Re-derive the EFS on-disk facts in efs.md from images and shipped binaries.

usage: efs-check.py IMAGE [EFS_A_MEMBER_DIR]

  IMAGE             a disk or CD image with an SGI volume header (the first
                    partition of type 5 is used), or a bare EFS partition
                    (first 512 bytes are not a volume header, base block 0)
  EFS_A_MEMBER_DIR  optional: directory holding the members of a 6.5.30
                    efs.a (efs_vfsops.o, efs_dir.o, efs_inode.o, ...); enables
                    the instruction-level checks

Exits non-zero if any check fails. Prints one line per check.
"""
import struct, sys, os

img = sys.argv[1]
objdir = sys.argv[2] if len(sys.argv) > 2 else None
fails = 0


def ok(cond, msg):
    global fails
    print(("ok   " if cond else "FAIL ") + msg)
    fails += 0 if cond else 1


f = open(img, "rb")
hdr = f.read(512)
base = 0
if struct.unpack(">I", hdr[:4])[0] == 0x0BE5A941:
    for i in range(16):
        n, first, typ = struct.unpack(">iii", hdr[312 + 12 * i:324 + 12 * i])
        if n and typ in (5, 7):
            base = first
            break
    else:
        sys.exit("no type-5/7 partition in volume header")


def blk(n, c=1):
    f.seek((base + n) * 512)
    return f.read(512 * c)


# ---- superblock (block 1) -------------------------------------------------
sb = blk(1)
size, firstcg, cgfsize = struct.unpack(">III", sb[:12])
cgisize, sectors, heads, ncg, dirty = struct.unpack(">5H", sb[12:22])
time_, magic = struct.unpack(">II", sb[0x18:0x20])
bmsize, tfree, tinode, bmblock, replsb, lastialloc = struct.unpack(">6I", sb[0x2C:0x44])
cksum = struct.unpack(">I", sb[0x58:0x5C])[0]


def checksum(b):
    c = 0
    for i in range(0, 0x58, 2):
        c ^= struct.unpack(">H", b[i:i + 2])[0]
        c = ((c << 1) | (c >> 31)) & 0xFFFFFFFF   # rotate left 1
    return c


ok(magic in (0x72959, 0x7295A), "magic %#x is one of the two accepted values" % magic)
ok(checksum(sb) == cksum, "checksum over bytes 0..0x57, 16-bit words, rotate-left-1: %#x" % cksum)
end_of_cgs = firstcg + ncg * cgfsize
if magic == 0x72959:
    ok(size == end_of_cgs, "old magic: size %d == firstcg + ncg*cgfsize" % size)
    ok(bmblock == 0, "old magic: bitmap starts at block 2 (bmblock field 0)")
else:
    ok(size >= end_of_cgs and bmblock >= end_of_cgs, "new magic: size and bmblock beyond the last cylinder group")
bm_start = bmblock or 2
ok(bmsize == (size + 7) // 8, "bmsize %d == ceil(size/8)" % bmsize)
bm_blocks = (bmsize + 511) // 512
if magic == 0x72959:
    ok(firstcg >= 2 + bm_blocks and firstcg - (2 + bm_blocks) < 512,
       "firstcg %d is the first aligned block after 2 + %d bitmap blocks" % (firstcg, bm_blocks))
ok(dirty == 0, "dirty field is 0 (clean) on this image: %#x" % dirty)
ok(cgisize * 4 * ncg < 1 << 31, "inode count %d fits 31 bits" % (cgisize * 4 * ncg))
if replsb:
    r = blk(replsb)
    ok(r[:0x5C] == sb[:0x5C], "secondary superblock at block %d equals primary (bytes 0..0x5b)" % replsb)

# ---- bitmap ---------------------------------------------------------------
bm = blk(bm_start, bm_blocks)[:bmsize]


def bit(n):
    return (bm[n >> 3] >> (n & 7)) & 1     # LSB first within each byte


def data_blocks(cg):
    s = firstcg + cg * cgfsize
    return range(s + cgisize, s + cgfsize)


set_data = sum(bit(b) for cg in range(ncg) for b in data_blocks(cg))
ok(set_data == tfree, "bits set in cylinder-group data areas (%d) == tfree %d" % (set_data, tfree))
outside = sum(bit(b) for b in range(0, firstcg))
ok(outside == 0, "no bit set below firstcg (boot, superblock, bitmap, gap)")

# ---- inodes ---------------------------------------------------------------
ipcg = cgisize * 4
cache = {}


def inode(ino):
    cg = ino // ipcg
    b = firstcg + cg * cgfsize + (ino >> 2) % cgisize
    if b not in cache:
        cache[b] = blk(b)
    d = cache[b][(ino & 3) * 128:(ino & 3) * 128 + 128]
    mode, nlink, uid, gid, sz, at, mt, ct, gen, ne, ver, spare = struct.unpack(">HhHHIIIIIHBB", d[:32])
    ex = []
    for i in range(12):
        w0, w1 = struct.unpack(">II", d[32 + 8 * i:40 + 8 * i])
        ex.append((w0 >> 24, w0 & 0xFFFFFF, w1 >> 24, w1 & 0xFFFFFF))   # magic, bn, length, offset
    return dict(mode=mode, nlink=nlink, size=sz, gen=gen, ne=ne, ver=ver, spare=spare, ex=ex, raw=d)


def extents(i):
    if i["ne"] <= 12:
        return [(bn, ln, off) for (_, bn, ln, off) in i["ex"][:i["ne"]]], []
    n = i["ex"][0][3]
    ind = [(bn, ln) for (_, bn, ln, _) in i["ex"][:n]]
    raw = b"".join(blk(bn, ln) for bn, ln in ind)
    out = []
    for k in range(i["ne"]):
        w0, w1 = struct.unpack(">II", raw[8 * k:8 * k + 8])
        out.append((w0 & 0xFFFFFF, w1 >> 24, w1 & 0xFFFFFF))
    return out, ind


free_by_mode = 0
for ino in range(ipcg * ncg):
    if inode(ino)["mode"] == 0:
        free_by_mode += 1
# inodes 0 and 1 are never handed out; mkfs counts them out of tinode
ok(free_by_mode - 2 == tinode,
   "inodes with di_mode==0 (%d) minus reserved 0 and 1 == tinode %d" % (free_by_mode, tinode))

root = inode(2)
ok(root["mode"] & 0xF000 == 0x4000, "inode 2 is the root directory")

used = set()
seen = set()
bad = []
stats = dict(files=0, dirs=0, indirect=0, maxlen=0, dirblocks=0, entries=0)


def walkdir(ino, path):
    if ino in seen:
        return
    seen.add(ino)
    i = inode(ino)
    t = i["mode"] & 0xF000
    if t not in (0x4000, 0x8000):
        return
    ex, ind = extents(i)
    if i["ne"] > 12:
        stats["indirect"] += 1
        for bn, ln in ind:
            used.update(range(bn, bn + ln))
    pos = 0
    for bn, ln, off in ex:
        if off != pos or ln == 0 or ln > 248:
            bad.append((path, "extent", bn, ln, off, pos))
        pos = off + ln
        stats["maxlen"] = max(stats["maxlen"], ln)
        used.update(range(bn, bn + ln))
    if t == 0x8000:
        stats["files"] += 1
        return
    stats["dirs"] += 1
    if i["size"] != pos * 512:
        bad.append((path, "dir size", i["size"], pos))
    for bn, ln, off in ex:
        for b in range(bn, bn + ln):
            d = blk(b)
            stats["dirblocks"] += 1
            if struct.unpack(">H", d[:2])[0] != 0xBEEF:
                bad.append((path, "dirblk magic", b))
                continue
            fu, ns = d[2], d[3]
            if ns > 0x48 or (fu and fu * 2 < ns + 4):
                bad.append((path, "dirblk header", b, fu, ns))
            spans = []
            for s in range(ns):
                o = d[4 + s] * 2
                if o == 0:
                    continue
                ln_ = d[o + 4]
                tot = 5 + ln_ + ((ln_ + 1) & 1)   # 4 ino + 1 len + name, padded so the total is even
                spans.append((o, o + tot))
                cino = struct.unpack(">I", d[o:o + 4])[0]
                stats["entries"] += 1
                name = d[o + 5:o + 5 + ln_]
                if name not in (b".", b".."):
                    walkdir(cino, path + "/" + name.decode("latin1"))
            spans.sort()
            # packed: no gaps, last entry ends at 512, first starts at firstused*2
            if spans:
                if spans[0][0] != fu * 2 or spans[-1][1] != 512 or any(
                        spans[k][1] != spans[k + 1][0] for k in range(len(spans) - 1)):
                    bad.append((path, "dirblk not packed", b, fu, spans[:3]))
            elif fu != 0:
                bad.append((path, "empty dirblk with firstused", b, fu))


walkdir(2, "")
ok(not bad, "extents dense and <=248 long, dir blocks magic/slots/packing hold (%d problems %s)" % (len(bad), bad[:2]))
ok(stats["maxlen"] <= 248, "longest extent %d blocks" % stats["maxlen"])
print("     walked: %(files)d files, %(dirs)d dirs, %(indirect)d with indirect extents, %(dirblocks)d dir blocks, %(entries)d entries" % stats)
leaked = [b for b in used if bit(b)]
ok(not leaked, "every block named by a reachable inode or indirect extent has its bitmap bit clear")
unref = sum(1 for cg in range(ncg) for b in data_blocks(cg) if not bit(b) and b not in used)
ok(unref == 0, "every clear bit in a data area is named by a reachable inode (%d not)" % unref)

# ---- instruction-level checks on the kernel archive members --------------
if objdir:
    from elftools.elf.elffile import ELFFile
    from capstone import Cs, CS_ARCH_MIPS, CS_MODE_MIPS64, CS_MODE_BIG_ENDIAN
    md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS64 | CS_MODE_BIG_ENDIAN)

    def func(objname, fn):
        e = ELFFile(open(os.path.join(objdir, objname), "rb"))
        t = e.get_section_by_name(".text")
        for s in e.get_section_by_name(".symtab").iter_symbols():
            if s.name == fn and s["st_info"]["type"] == "STT_FUNC":
                a = s["st_value"]
                n = s["st_size"]
                return [(i.mnemonic, i.op_str) for i in md.disasm(t.data()[a:a + n], a)]
        raise SystemExit("no %s in %s" % (fn, objname))

    def has(ins, mn, *subs):
        return any(m == mn and all(s in o for s in subs) for m, o in ins)

    cs = func("efs_vfsops.o", "efs_checksum")
    ok((has(cs, "daddiu", "0x58") or has(cs, "addiu", "0x58")) and has(cs, "lhu") and has(cs, "xor") and has(cs, "slti", "0") and has(cs, "sll", "1"),
       "efs_checksum: 16-bit words, xor, rotate-left-1 (slti 0 + sll 1 + or), 0x58 bytes")
    oc = func("efs_vfsops.o", "efs_oldchecksum")
    ok(not has(oc, "slti", "0"), "efs_oldchecksum: same loop without the rotated-out carry")
    ck = func("efs_vfsops.o", "efs_checksb")
    ok(has(ck, "ori", "0x2959") and has(ck, "ori", "0x295a"), "efs_checksb accepts magic 0x72959 and 0x7295a")
    ok(has(ck, "lh", "0x14($s0)"), "efs_checksb reads the dirty halfword at offset 0x14")
    cl = func("efs_vfsops.o", "efs_cleanfs")
    ok(has(cl, "addiu", "0x7777") and has(cl, "addiu", "0x1234"), "efs_cleanfs: 0x7777 -> 0, anything else -> 0x1234")
    mf = func("efs_vfsops.o", "efs_mountfs")
    ok(has(mf, "addiu", "0xbad") and has(mf, "addiu", "0x7777"), "efs_mountfs: marks the in-memory dirty field 0xbad or 0x7777")
    gd = func("efs_dir.o", "getdirblk")
    ok(has(gd, "ori", "0xbeef") and has(gd, "sltiu", "0x49"), "getdirblk: block magic 0xbeef, slot count < 0x49")
    di = func("efs_dir.o", "efs_dirinit")
    ok(has(di, "addiu", "0xfd") and has(di, "addiu", "0xf9"), "efs_dirinit: '.' slot 0xfd, '..' slot 0xf9 (offsets 0x1fa and 0x1f2)")
    fm = func("efs_inode.o", "efs_iformat")
    ok(has(fm, "slti", "0xd"), "efs_iformat: direct extent count limit 12")
    ok(has(fm, "sltiu", "0x61"), "efs_iformat: short inline data limit 0x60 bytes (sltiu 0x61)")
    gv = func("efs_inode.o", "efs_getdev")
    ok(has(gv, "sra", "8") and has(gv, "sll", "0x12"), "efs_getdev: short = major<<8 | minor; -1 means the 32-bit dev at +4")
    gr = func("efs_dalloc.o", "efs_grow")
    ok(has(gr, "addiu", "0xf8"), "efs_grow: extent length cap 0xf8")

print("%d check(s) failed" % fails if fails else "all checks passed")
sys.exit(1 if fails else 0)
