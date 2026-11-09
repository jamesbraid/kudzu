# EFS on-disk format

The Extent File System, IRIX's filesystem before XFS: superblock, bitmap, cylinder groups, inodes, extents and directory blocks. Recovered from the shipped IRIX 6.5.30 kernel filesystem module, `mkfs_efs` and `fsck`, and checked field by field against real EFS images from IRIX distribution media (five walked in full, three more inode-scanned). Binary reverse engineering only: no IRIX source, headers or comments were read. Field names are the ones the Linux kernel's published `fs/efs` driver uses for the same records, with IRIX symbol names quoted where a routine is cited. Every offset was recovered independently from the IRIX binaries and the images.

Companion files, same directory:

- `efs-disk.h`: the structures below as a C header (`gcc -std=c11 -fsyntax-only` passes; sizes and key offsets are asserted; `efs_checksum()` reproduces the stored checksum of a real image).
- `efs-check.py`: re-derives the checkable facts (`uv run --script efs-check.py IMAGE [EFS_A_MEMBER_DIR]`). With an image alone it checks the superblock, bitmap, inode and directory invariants over the whole reachable tree; with a directory of `efs.a` members it also checks the instruction-level facts. All checks pass on the five fully walked images and, for the instruction-level part, on the `efs.a` members of the IP27, IP30, IP35 and IP32 kernels.

**Premise correction.** An EFS extent is **8 bytes**, not 12. The number 12 is the count of extents that fit in the inode (12 × 8 = 96 bytes, offset 0x20 to 0x7f). Details in [Extents](#extents).

## Grades

- **Established**: the routine that reads or writes the field was read in the named binary and the value is consistent with the real images (or, where no image exercises it, in at least two independent routines).
- **Inferred**: one routine only, a name or string, or a conclusion from established facts. The text says what would settle it.
- **Unknown**: not recovered. What was tried is stated.

## Evidence base

| Short name | Binary | Notes |
|---|---|---|
| IRIX kernel | `6.5.30/usr/cpu/sysgen/IP27boot/efs.a` (196132 bytes, sha256 `da3ed50f…1875cb9`) members `efs_vfsops.o`, `efs_inode.o`, `efs_dir.o`, `efs_bmap.o`, `efs_bitmap.o`, `efs_ialloc.o`, `efs_dalloc.o`, `efs_subr.o`, `efs_iget.o`, `efs_vnodeops.o` | MIPS64 big-endian relocatable objects with symbols and line-less DWARF (subprogram entries only, no types). Offsets cited are `.text` offsets in the member. `IP30boot`, `IP35boot` (both n64) and `IP32boot` (n32) carry the same routines recompiled; the constant-level checks in `efs-check.py` pass on all four. The text is not byte-identical across them, so a cited offset applies to IP27 only. There is no `efs.a` under `IP22boot`. |
| mkfs_efs | `6.5.30/sbin/mkfs_efs` (sha256 `baf791db…0b4f`, N32, stripped; `.dynsym` still names its global functions and variables) | Writes a fresh filesystem. Prints "read/write support for EFS filesystems will be removed from the next all-platform IRIX release." |
| fsck | `6.5.30/sbin/fsck` (sha256 `2417e3ea…8f03`, N32) | Contains the EFS checker. Used for its superblock and directory rules and the secondary-superblock logic. |
| fmt_efs.so | `6.5.30/usr/lib/devicelib/fmt_efs.so` | `has_efs_superblock` reads block 1 of the partition and hands it to `valid_efs` in libdisk. Confirms the superblock position. |
| dump / restore / fsr_efs | `usr/sbin/dump`, `sbin/restore`, `usr/etc/fsr_efs` | Strings only ("walkextents", "too many extents", "indirect extents", "efs_getextents"). Not disassembled. |
| Images | `refs/irix-media/…/efs/` and `…/irix-6.5.30/`: `IRIX-6.5-Foundation1.efs`, `IRIX-6.5.30-Overlays1.efs`, `IRIX-6.5.30-Applications.efs`, `ONC3-NFS-Version-3.efs`, `irix-4.0.5F/IRIX-4.0.5F.efs`, and the 1993 5.1 prerelease image | Volume-header images; the EFS partition is the one of type 5 (first block 64, or 44256 and 39424 on two of them). Fully walked by `efs-check.py`: Foundation1, 4.0.5F, 6.5.30 Overlays1, 6.5.30 Applications, ONC3. Inode-scanned only (type counts, magic, dirty, `bmblock`, `replsb`): the 5.1 prerelease image, one 1994 demo image and a 6.5.16 overlay image; all three are clean, old magic, no secondary superblock. |

Limits of the image evidence: all held images were written with the older magic, are clean, carry no secondary superblock, no device nodes, no symlinks and no `di_version` other than 0. Everything the text says about those cases is from the kernel and `mkfs_efs`/`fsck` code. [Unknowns](#unknowns-and-what-would-settle-them) lists them.

## Byte order, units, placement

- All multi-byte integers are **big-endian**. Established (every `lw`/`lh` of a disk field in the kernel is a plain big-endian load; the image decode in `efs-check.py` uses `>`).
- A **block** ("bb") is 512 bytes everywhere: the buffer reads in `efs_itobp` (`efs_inode.o` +0x318), the `<< 9` conversions in `efs_statvfs` (`f_bsize` 512) and every image. Block numbers are relative to the start of the filesystem, not the disk. Established.
- The filesystem lives in a volume-header partition. On the held distribution images it is the partition of type 5 (the data partition at first block 64), and `volume-header.md` records type 7 as the "efs" disk type. Both occur; the filesystem itself does not care. Established for 5 on media; type 7 inferred.
- Block 0 of the filesystem is an unused boot block. `mkfs_efs` writes one zeroed block there (`0x10003234`, message "boot block write failed"), and on every image it is all zero. Block 1 is the superblock. Established (`efs_sbupdate` writes block 1, `efs_mountfs` reads block 1, `fmt_efs.so` reads partition start + 1).

## Superblock (block 1, 0x5c meaningful bytes)

| Off | Size | Field | Meaning | Evidence | Grade |
|---:|---:|---|---|---|---|
| 0x00 | 4 | `fs_size` | filesystem size in blocks. Old magic: must equal `firstcg + ncg × cgfsize`. New magic: at least that. | `efs_checksb` +0xdd0..0xe38; `mkfs_efs` sb init `0x10002e80..0x10002eb8`; fsck "fsize %ld != firstcg %d + %dx%ld" at `0x100075ec..0x1000765c`; the five walked images | established |
| 0x04 | 4 | `fs_firstcg` | block number of cylinder group 0 | `efs_imap` +0x2b8; `mkfs_efs` `0x10002ec4`; images | established |
| 0x08 | 4 | `fs_cgfsize` | blocks per cylinder group (inode blocks included); 8000 on every image | `efs_imap` +0x290; `efs_statvfs` +0x1700; `mkfs_efs` `0x10002ed0` | established |
| 0x0c | 2 | `fs_cgisize` | inode blocks at the start of each group (32 on every image); must be below `cgfsize` | `efs_imap` +0x2a0; `efs_statvfs`; fsck "cgfsize %ld <= cgisize %ld" | established |
| 0x0e | 2 | `fs_sectors` | disk geometry hint: sectors per track (32 on every image). Must be nonzero; nothing else reads it. | `efs_checksb` +0xd44; `mkfs_efs` `0x10002ee4` | established for the check; meaning inferred |
| 0x10 | 2 | `fs_heads` | geometry hint; `mkfs_efs` stores the constant 10 | `mkfs_efs` `0x10002f28`; every image has 10 | established that it is written; use is unknown |
| 0x12 | 2 | `fs_ncg` | number of cylinder groups; must be positive | `efs_checksb` +0xd30; `efs_statvfs` +0x16fc..0x1704 | established |
| 0x14 | 2 | `fs_dirty` | mount state. See [Dirty state](#dirty-state). | `efs_mountfs`, `efs_cleanfs`, `efs_sbupdate`, `efs_checksb`; `mkfs_efs` `-r` | established |
| 0x16 | 2 | (pad) | zero on every image; no routine reads or writes it | scanned all `efs_*.o` stores to the superblock | inferred unused |
| 0x18 | 4 | `fs_time` | seconds since 1970; `efs_sbupdate` stores the current time here on every superblock write; `mkfs_efs` stores `time()` | `efs_sbupdate` +0xf88; `mkfs_efs` `0x10002efc`; images (e.g. 895859809 = May 1998) | established; signedness unknown |
| 0x1c | 4 | `fs_magic` | `0x00072959` (old) or `0x0007295a` (new). Anything else fails the mount (error 0x3f3). | `efs_mountfs` +0x6b8..0x6d4; fsck compares both | established |
| 0x20 | 6 | `fs_fname` | filesystem name, not NUL terminated; `mkfs_efs` writes "noname" | `mkfs_efs` `0x10002fd8`; images | established that it is stored; the kernel never reads it |
| 0x26 | 6 | `fs_fpack` | pack name; "nopack" | `mkfs_efs` `0x10002fc4` | as above |
| 0x2c | 4 | `fs_bmsize` | bitmap length in bytes = `ceil(fs_size / 8)`. Must be in 1..0xc80000. | `efs_checksb` +0xd50..0xd5c; `mkfs_efs` `0x10002f74..0x10002f88`; fsck "bitmap size %d should be %d"; images | established |
| 0x30 | 4 | `fs_tfree` | free data blocks. Equals the number of set bitmap bits in the cylinder-group data areas on every image. Must be 0..`ncg × (cgfsize − cgisize)`. | `efs_checksb` +0xd8c..0xdac; `efs_free`/`efs_grow` update it; `mkfs_efs` decrements it per allocated block (`0x10005c14`, `0x10005d28`); `efs-check.py` | established |
| 0x34 | 4 | `fs_tinode` | free inodes. Initially `ncg × cgisize × 4 − 2`; equals the count of `di_mode == 0` inodes minus the two reserved ones on every image. Must be 0..`ncg × cgisize × 4`. | `efs_statvfs` +0x1738 (`f_files = ncg × cgisize × 4 − 2`); `efs_ialloc` +0x7a8, `efs_ifree` +0x9ac; `mkfs_efs` `0x10003000` | established |
| 0x38 | 4 | `fs_bmblock` | block number where the bitmap starts; **0 means block 2**. Nonzero only with the new magic, where it must be at or past `firstcg + ncg × cgfsize`. | `efs_bitfunc` +0x1c, +0x1a4 (zero → 2); `efs_checksb` +0xe20; fsck "bitmap starts at %d, overlaps cylinder group space ending at %d" / "bitmap ends at %d, overlaps firstcg %d" | established |
| 0x3c | 4 | `fs_replsb` | block number of the secondary superblock, or 0. | `mkfs_efs` `0x10002f4c..0x10002f68`; `mkfs_efs` "Can't write 2nd superblock"; fsck "SECONDARY SUPERBLOCK MISSING" / "Trying secondary" and its creation rule at `0x100058fc..0x1000594c` | established as written by `mkfs_efs` and `fsck`; no held image has one |
| 0x40 | 4 | `fs_lastialloc` | the last inode number `mkfs_efs` allocated (128 on the 6.5 images; 0 on the 4.0.5F image). No kernel routine reads it. | `mkfs_efs` `efs_lastallocino` `0x10005694` → `0x10003ccc`; no load of this word in `efs_*.o` | inferred (a hint; use unknown) |
| 0x44 | 20 | `fs_spare` | zero | images | inferred |
| 0x58 | 4 | `fs_checksum` | see below | `efs_checksum` +0xe40; `efs_sbupdate` +0xf90; `mkfs_efs` `0x100056ac` | established |
| 0x5c | 0x1a4 | — | **not part of the format.** `efs_sbupdate` copies 0xc8 bytes of the in-memory mount structure over the buffer and zeroes the remaining 0x138 (`+0xfd4..0xff0`), so bytes 0x5c..0xc7 are a snapshot of runtime state. `mkfs_efs` writes its whole 0xa8-byte superblock work area, so incidental words appear (every 6.5 image has `0x80` at bytes 0x7f and 0xab, the inodes-per-group count). Readers ignore them; writers write zeros. | `efs_sbupdate`; images | established |

### Checksum

32-bit accumulator, initially 0, over the 44 big-endian 16-bit words at bytes 0..0x57: `acc ^= word; acc = rotl32(acc, 1)`. The result is stored at 0x58. Established: `efs_checksum` (`efs_vfsops.o` +0xe40: `lhu`, `xor`, `slti $v0,$v1,0` captures the top bit, `sll 1`, `or`), the identical loop in `mkfs_efs`, and exact agreement with the stored value on all five walked images.

`efs_oldchecksum` (+0xe80) is the same loop without the carried-out top bit (`acc = (acc ^ word) << 1`). `efs_checksb` accepts a superblock if either variant matches the stored value (+0xd10..0xdf0). No held image needs the old variant. Established that both are accepted; the era that wrote the old form is unknown.

### Dirty state

`fs_dirty` is a four-valued mount marker. Established from four routines:

| Value | Meaning | Set by |
|---:|---|---|
| `0x0000` | unmounted cleanly | `efs_cleanfs` when the value was 0x7777; `mkfs_efs` |
| `0x7777` | mounted read-write, was clean at mount | `efs_mountfs` (+0x890, +0xa6c): the in-memory copy is 0x7777 if the on-disk value was 0, then `efs_sbupdate` writes it |
| `0x0bad` | mounted read-write and the filesystem was already dirty, or a corruption was detected | `efs_mountfs` (nonzero on-disk value); `efs_sbupdate` writes 0xbad whenever the in-memory corrupt flag (mount structure byte 0x5e, set by `efs_setcorrupt`) is set |
| `0x1234` | unmounted without having been clean at mount; needs fsck | `efs_cleanfs` for any value other than 0x7777; `mkfs_efs -r` ("recover option") writes it into a new filesystem |

`efs_cleanfs` does nothing for a read-only mount. A normal mount (`efs_mount`, which passes mode 2) runs `efs_checksb`, which rejects any nonzero `fs_dirty`, so a dirty filesystem needs fsck before it mounts. The root-mount path takes its own mode value and does not always run the check; which values skip it was not worked out. **Unknown** for the root path.

### Superblock acceptance rules (the checks a conforming reader should apply)

From `efs_checksb` (established), plus `efs_mountfs` and fsck:

1. checksum matches (rotated or old form);
2. `fs_dirty == 0`;
3. `ncg > 0`, `sectors > 0`, `0 < bmsize ≤ 0xc80000`, `0 ≤ tinode ≤ ncg × cgisize × 4`, `0 ≤ tfree ≤ ncg × (cgfsize − cgisize)`;
4. old magic: `fs_size == firstcg + ncg × cgfsize`; new magic: `fs_size ≥` that and `fs_bmblock ≥` that;
5. `efs_mountfs` additionally rejects a filesystem larger than its device by more than 250 blocks (`fs_size > devsize + 0xfa`, +0x78c..0x794), inferred as slack for a trailing secondary superblock;
6. fsck also requires `cgisize < cgfsize`, and (old layout) `2 + ceil(bmsize / 512) ≤ firstcg`.

## Cylinder groups and bitmap

There is **no on-disk cylinder-group header**. A cylinder group is purely arithmetic from the superblock; per-group free counts are rebuilt at mount (`efs_builddsum`, `efs_subr.o` +0, reads the bitmap and builds the 40-byte in-memory records, which `efs_mountfs` allocates as `ncg × 40 + 0x118` bytes). Established.

```
bb 0                       boot block (unused, zero)
bb 1                       superblock
bb 2 .. 2+B-1              bitmap, B = ceil(fs_bmsize / 512)      (old magic; fs_bmblock == 0)
bb 2+B .. firstcg-1        gap (alignment); bitmap bits clear
bb firstcg + g*cgfsize     cylinder group g, g = 0 .. ncg-1
   + 0 .. cgisize-1        inode blocks, 4 inodes of 128 bytes each
   + cgisize .. cgfsize-1  data blocks
[ replsb ]                 secondary superblock, last block of the partition, if any
```

- `firstcg` is `2 + B` rounded up to a multiple of a `mkfs_efs` alignment parameter (`cg_align`, default 1, set from the geometry with the cylinder-align option). `mkfs_efs` main, `0x100023a0..0x100023e8`. All held images use 32: 288 (B=274), 128 (B=120), 256 (B=223). **Established** for the rule; the kernel and fsck require only `firstcg ≥ 2 + B`.
- `ncg × cgfsize` fills the rest: `fs_size = firstcg + ncg × cgfsize` (old magic). Group size, inode blocks per group and group count are chosen by `mkfs_efs` (defaults 8000 / 32 here for large filesystems; `mkfs_efs` `0x10002a64..0x10002b50` shows the sizing heuristics, not needed to read a filesystem).
- **Size limit.** 24-bit block numbers in extents cap a filesystem at 2²⁴ blocks (8 GiB); `mkfs_efs` refuses larger ("filesystems may not exceed 8 gigabytes!", `0x100022f0..0x10002300`). Established. `fs_size` itself is 32 bits and the kernel's bitmap limit (0xc80000 bytes) is looser; the true limit for blocks addressed through extents is 2²⁴.

### Inode location

`ino` → group `g = ino / (cgisize × 4)`; block `firstcg + g × cgfsize + (ino / 4) mod cgisize`; slot `ino mod 4`, byte offset `slot × 128` in the block. Established: `efs_imap` (+0x238), `efs_iget` in `mkfs_efs` (`0x10005704`), `efs_itobp`. Inode 0 and 1 are never used (they read as `di_mode == 0` on every image and are excluded from `fs_tinode`). The root directory is inode 2 (`efs_mountfs` calls `iget(…, 2)` and requires a directory, +0xa7c..0xb40). Established.

An inode is **free iff `di_mode == 0`**. Established: `efs_scanchunk` tests the first halfword of each inode (+0x3c8: `lhu` then `beqz` → take it), `mkfs_efs` `efs_allocino` (`0x10005608`).

### Bitmap

- One bit per block of the whole filesystem, `fs_size` bits, `fs_bmsize` bytes. Bit `n` is byte `n >> 3`, **bit `n & 7` counting from the least significant bit**. Established: `mkfs_efs` bitmap init (`0x1000336c..0x1000337c`: `byte |= 1 << (n & 7)`) and on all images.
- **Bit set = block free; bit clear = in use or not data.** Established: `mkfs_efs` sets bits only for cylinder-group data blocks and clears them when allocating (`0x10005bf4..0x10005bfc`); the kernel's `efs_bitfunc` mode 1 counts set bits for free-run search (`efs_findfree`); on the four large images the number of set data-area bits equals `fs_tfree` exactly, and every block reachable from the tree has its bit clear and every clear data-area bit is reachable.
- Blocks 0 .. `firstcg − 1` (boot, superblock, bitmap, gap) have clear bits. Established on images.
- **Bits for the inode blocks are not allocation state for blocks.** On a fresh image they are all zero. `efs_builddsum` sets the bits of every inode block of every group at a read-write mount (mode 5 for group 0, mode 3 for the rest, `efs_subr.o` +0xb8, +0xdc), `efs_scanchunk` clears one when it finds that inode block full (+0x470) and `efs_ifree` sets one when it frees an inode in the block (`efs_ialloc.o` +0xa50). They are an inode-allocation hint: a set bit means "this inode block may hold a free inode". A reader should ignore them; a writer that matches a fresh `mkfs_efs` leaves them zero. **Inferred** from the three routines' use; no held image has been through a read-write mount.
- The kernel reads the bitmap in chunks of 128 blocks (`efs_bitfunc` +0x4c: `bit >> 19`), an implementation detail.

### Secondary superblock

`mkfs_efs` stores `replsb = dev_blocks − 1` when a device size is known and the space beyond the filesystem is smaller than one cylinder group (`0x10002f44..0x10002f68`), then writes a copy of the primary at that block. fsck creates the same thing (`replsb = devsize − 1`) when `2 < devsize − fs_end` and the leftover is below `cgfsize / 2` (`0x100058fc..0x1000594c`), and reads it as a fallback when the primary is invalid. So the secondary is the **last block of the partition**, a full copy of block 1 with its own checksum. Established for what the two tools do; no held image has a secondary, and the kernel never reads or updates it (`efs_sbupdate` writes block 1 only), so it can go stale. Inferred.

## Inode (128 bytes)

| Off | Size | Field | Meaning | Evidence | Grade |
|---:|---:|---|---|---|---|
| 0x00 | 2 | `di_mode` | type and permission bits, POSIX layout. 0 = free inode. | `efs_iread` +0x92c..0x978 (loaded `lhu`, 0 → inode returns "not allocated"); `efs_iupdat` +0xf6c | established |
| 0x02 | 2 | `di_nlink` | signed halfword | `efs_iread` +0x980 (`lh`) | established |
| 0x04 | 2 | `di_uid` | unsigned 16 bits | `efs_iread` +0x98c (`lhu`) | established |
| 0x06 | 2 | `di_gid` | unsigned 16 bits | `efs_iread` +0x998 | established |
| 0x08 | 4 | `di_size` | bytes. Regular files: the on-disk size; device nodes: stored as 0. Directories: a multiple of 512 or the directory is reported corrupt. | `efs_iread` +0x9a0; `efs_iupdat` +0xdc8, +0x1178, +0x119c; `getdirblk` +0x130 | established |
| 0x0c | 4 | `di_atime` | seconds since 1970 | `efs_iupdat` +0xfc8 (stores into 0xc when the access flag 2 is set); `efs_iread` +0x9b8 | established |
| 0x10 | 4 | `di_mtime` | seconds (update flag 1) | `efs_iupdat` +0xfe8 | established |
| 0x14 | 4 | `di_ctime` | seconds (change flag 4) | `efs_iupdat` +0x1004 | established |
| 0x18 | 4 | `di_gen` | generation number. `mkfs_efs` stores the creation time in every inode of a new filesystem (`0x100030a4..0x100030b4`); the kernel increments it when it frees the inode (`efs_itrunc` +0x14b8..0x14c8) and compares it against file-handle generations (`efs_vget` +0x1e88). | as cited | established |
| 0x1c | 2 | `di_numextents` | total number of extents. See [Extents](#extents). Valid range 0..0x7fff. | `efs_iformat` +0x514..0x524 | established |
| 0x1e | 1 | `di_version` | 0 on every image. `efs_iread` (+0xa38..0xa90) stores it and for values **1 and 2** calls an optional conversion hook (`afsdptoipp`); with no hook the read fails with errno 0x41 and the inode is not returned. Other values take the normal path. | as cited | established for the dispatch; what versions 1 and 2 hold is **unknown** |
| 0x1f | 1 | `di_spare` | zero on every image | images | inferred |
| 0x20 | 96 | `di_u` | extents, or a device number, or short inline data | below | established |

### Types the kernel accepts (`efs_iformat`, `efs_iupdat`)

`di_mode & 0xf000` selects the interpretation of `di_u`. Anything not listed makes the read fail with EIO.

| Type | Value | `di_u` | Evidence |
|---|---:|---|---|
| FIFO, character device, block device, socket | 0x1000, 0x2000, 0x6000, 0xc000 | device number, `di_numextents` forced to 0, `di_size` 0 | `efs_iformat` +0x3e8..0x474, 0x500..0x508; `efs_iupdat` +0xdec..0xe28, +0x1184 |
| Directory, regular file | 0x4000, 0x8000 | extents (direct or indirect). Zero extents with a nonzero size is not decoded as inline. | `efs_iformat` +0x514 |
| Symbolic link | 0xa000 | extents, or **inline data** (below) | `efs_iformat` +0x408..0x50c; `efs_readlink` +0x1b9c..0x1bc4 |
| Two further types | 0x3000, 0x7000 | handled exactly like a symlink: extents or inline data; `efs_readlink` accepts them. No name for them was recovered. | `efs_iformat`, `efs_readlink` +0x1ba4..0x1bc4 |

The kernel's `efs_symlink` creates the link through the ordinary write path (`efs_ialloc` then `efs_writei`), which allocates extents. Inline data is therefore a form the kernel can read and rewrite but, from what was read, does not create. **Inferred**; no held image has a symlink.

### Device number (`efs_getdev`, `efs_putdev`)

`di_u` starts with a 16-bit value `v`. If `v == 0xffff` the device number is the 32-bit word at `di_u + 4` (stored whole). Otherwise it is `(v >> 8) << 18 | (v & 0xff)`: an 8-bit major and an 8-bit minor in the halfword. `efs_putdev` uses the short form when `major < 0x80` and `minor < 0x100` (it tests `dev >> 18 < 0x80` and `dev & 0x3ffff < 0x100`), otherwise writes `0xffff` and the full word. Established from the two routines (`efs_inode.o` +0, +0x2c); no held image has a device node.

### Inline short data

When the inode type is 0xa000, 0x3000 or 0x7000, `di_numextents` is 0 and `di_size` is 1..96, the contents are the first `di_size` bytes of `di_u`; `efs_iupdat` writes them back and zeroes the rest of the 96 bytes (+0x1108..0x1154). Established from `efs_iformat` +0x5c0..0x5dc and `efs_iupdat` +0x10fc. Anything of 97 bytes or more must use extents.

## Extents

An extent is **8 bytes** (two big-endian words):

```
word 0:  magic:8 (always 0) | bn:24      first block of the run, relative to the filesystem
word 1:  length:8           | offset:24  run length in blocks (1..248) | index of the first file block
```

- Field positions: `bn = w0 & 0xffffff` and `length = w1 >> 24`, `offset = w1 & 0xffffff` throughout the kernel (`efs_bmapExtent` +0x1fc..0x22c, `efs_iformat` +0x4b0, `efs_readindir`) and in `mkfs_efs` `efs_extend` (`0x10005b78..0x10005c58`, `0x10005dc4`, `0x10005f1c`). Established.
- **Maximum length is 248 blocks** (0xf8): `efs_grow` (+0xa64) and `mkfs_efs` (`0x10005b94`, `sltiu 0xf8`) never extend an extent past it. Established. fsck calls a zero length "ZERO LENGTH EXTENT" and a wrong one "ILLEGAL LENGTH IN EXTENT".
- The magic byte is 0 in every image. Whether the kernel rejects other values was not found; fsck has no message for it. Unknown.
- **Order and density.** Extents are in ascending file order and, on every held image, dense: each `offset` equals the sum of earlier lengths and the last one ends at `ceil(size / 512)`. `mkfs_efs` derives the logical position from the running total and so ignores `offset`; the kernel (`efs_bmapExtent` +0x1fc..0x214) scans for the first extent whose `offset + length` exceeds the file block and uses `offset` explicitly. fsck reports "EXTENT OUT OF ORDER". Whether sparse files (a gap between `offset + length` and the next `offset`) are valid is **unknown**; nothing creates one.
- **Direct list.** With `di_numextents ≤ 12` the extents are `di_u[0 .. numextents-1]`; the rest of the 96 bytes is zero (`efs_iupdat` +0xf44..0xf5c). Established.

### Indirect extents (more than 12)

If `di_numextents > 12` the inode holds **indirect extents** in `di_u`, and the real extent list (`di_numextents` × 8 bytes) lives on disk in the blocks they describe, concatenated in order. Established from `efs_iformat` (+0x52c..0x558), `efs_readindir` (+0xa94) and `efs_writeindir` (+0x1280), and decoded by `efs-check.py` on every indirect-extent file (9 to 42 per image), with block-bitmap agreement:

- `di_u[0].offset` (low 24 bits of word 1 of the first extent) holds the **number of indirect extents** `n`, with `1 ≤ n ≤ 12`. `efs_iformat` fails the read otherwise; `efs_readindir` clamps a 0 or a value above 12 to 1.
- `di_u[0 .. n-1].bn` and `.length` give the disk runs, in order, that together hold the real extent array; the `offset` of entries other than the first is not used. Each real extent is 8 bytes laid out as above. The list may end partway through the last indirect extent (the runs are allocated whole).
- In practice `n = 1` with a length of 1 or 2 blocks on every file seen (e.g. `di_u[0] = {bn 6578, length 1, offset 1}` for a 25-extent file).
- Where the runs live is ordinary data space; the bitmap bits of those blocks are clear (checked).
- `efs_writeindir` also clamps the last real extent's length so `offset + length` does not exceed `ceil(size / 512)` (+0x1380..0x13bc). Inferred as truncation housekeeping.

## Directory block

A directory is a regular-extent file whose size is a multiple of 512; each 512-byte block is independent. `getdirblk` (`efs_dir.o` +0x120): a size not a multiple of 512 is "dir corrupted"; the block must start with halfword **`0xbeef`**; the slot count must be below `0x49`. Established.

```
0x000  magic     u16  0xbeef
0x002  firstused u8   offset of the lowest-addressed entry, in 2-byte units; 0 = block holds no entries
0x003  slots     u8   number of slot bytes in use, at most 0x48
0x004  slot[0..slots-1]  one byte each: entry offset / 2, or 0 = unused slot
 ...   free space
 ...   entries, packed contiguously, growing downward from byte 0x200
```

Validity (`getdirblk` +0x2cc..0x324): if `firstused ≠ 0` then `firstused × 2 ≥ slots + 4`; if `firstused == 0` then `slots < 0x1fd` (an empty block; the new-block initialiser writes `0xbeef`, `firstused` 0, `slots` 0). Established. fsck names the failures: "BAD FREESPACE POINTER", "TOO MANY SLOT OFFSETS", "SLOT OFFSET TOO BIG", "SLOT OFFSET POINTS TO UNUSED SPACE", "OVERLAPPING DIRECTORY SLOTS/ENTRIES".

**Entry** (at `slot × 2`):

```
+0   u32  inode number (big-endian; the kernel stores it as two halfwords, so an entry may sit on a 2-byte boundary)
+4   u8   name length, 1..255
+5   ...  name bytes, not NUL terminated, then one pad byte if needed
```

Entry size is `5 + namelen`, plus one pad byte when that is odd (so the size is always even and entries stay 2-byte aligned): `EFS_DENTSIZE(1) = 6`, `EFS_DENTSIZE(2) = 8`. Established: `efs_direnter` computes `5 + len + ((len ^ 1) & 1)` (+0xcac..0xcc4), and `mkfs_efs` `efs_enter` (`0x10005034`), which writes a four-byte inode number at `end − pad − 5`, the length at `end − pad − 1`, and the name at `end − pad`.

**Layout rules** (established by `efs_direnter` +0xcc0..0xda0, `efs_dirremove` +0x1118..0x11b4, `efs_dirinit` +0x12b4..0x1348, `mkfs_efs`, and the `efs-check.py` packing check over every directory block on five images):

1. **Insert.** The new entry goes immediately below `firstused × 2` (or ends at byte 0x200 if the block is empty); `firstused` becomes its offset / 2. It needs `entsize < firstused × 2 − slots − 4` (an empty block uses `0x1fc − slots`). The entry's slot byte goes in the first slot holding 0, or a new slot at index `slots` (then `slots++`; at most 0x48).
2. **Remove (compaction).** The entry at offset E of size S is deleted by moving every entry in `[firstused × 2, E)` up by S (`bcopy`), zeroing the S bytes now vacated at the bottom, adding S / 2 to `firstused`, adding S / 2 to every slot byte whose offset was below E, setting the removed slot to 0, and if it was the last slot decrementing `slots`. If `firstused` becomes 0 (block empty) `slots` is set to 0. So the entries are always **contiguous from `firstused × 2` to the end of the block** with no holes, and slot indices are stable (a freed slot is reused later).
3. **Order.** Slot order is the order `getdent`/readdir returns names; physical order is the reverse of insertion modulo compaction. Slot value 0 is skipped (`getdent` +0x56c..0x574).
4. `.` and `..` are ordinary entries. The kernel's initial directory block (`efs_dirinit`) puts `.` at offset 0x1fa (slot byte 0xfd, inode of the directory) and `..` at 0x1f2 (slot byte 0xf9, parent), `firstused` 0xf9, `slots` 2. `efs_dirremove` refuses to remove either.
5. There is no hashing and no sorting; lookup is a linear scan of the slots of each block.

Example, the root of `IRIX-6.5-Foundation1.efs` (inode 2, one block at bb 320): `firstused 215`, `slots 7`; slot offsets 506 (`.`), 498 (`..`), 490 (`.IM`, ino 3), 480 (`dist`, ino 4), 462 (`.distribution`, ino 82), 444 (`RELEASE.info`, ino 83), 430 (`.IM_1398`, ino 84); the entries tile bytes 430..511 with no gap.

`mkfs_efs` also creates a `lost+found` directory with **20 blocks** (`0x2800` bytes) each containing only `0xbeef` and zeros (`efs_mklostandfound`, `0x10004f18`). Established from the code; none of the distribution images has one.

## Runtime-only structures (not on disk)

For anyone reading the kernel disassembly: the in-memory mount structure holds the first 0x5c bytes of the superblock at +0x78, then state: byte 0x5c read-only flag, 0x5d superblock-dirty, 0x5e corrupt flag, the inodes-per-group count (`cgisize × 4`) at +0x88 from the superblock start, the bitmap block count at +0x92, and `ncg` 40-byte per-group records starting at +0xa0 (first inode, free-block counts, hints) rebuilt by `efs_builddsum`. None of it is written except the first 0xc8 bytes copied by `efs_sbupdate`.

## What a reader/writer needs, in one paragraph

Read the volume header, take the EFS partition, read block 1, verify the magic and checksum, require `fs_dirty == 0` for a normal mount, compute `B = ceil(bmsize / 512)` and read the bitmap from `bmblock || 2`, locate inode `ino` by the group arithmetic above, follow `di_numextents` (direct up to 12, otherwise through `di_u[0].offset` indirect runs), and walk directories by their slot bytes. A writer clears bitmap bits and `fs_tfree` for each block it takes, decrements `fs_tinode` when it takes an inode (`di_mode != 0`), keeps directory entries compacted, rewrites `fs_time` and the checksum, and uses `fs_dirty` 0x7777 → 0 around its session.

## Unknowns and what would settle them

| Item | Why unknown | Would settle |
|---|---|---|
| New magic `0x7295a` layout (bitmap past the cylinder groups, `fs_size` including it) | No held image carries it and `mkfs_efs` never writes it. The rules are from `efs_checksb`, `efs_bitfunc` and fsck only. | an image or a Linux/IRIX tool that writes the new magic |
| Secondary superblock content and the old-form checksum's era | No held image has either. | an image with `fs_replsb ≠ 0` |
| `di_version` 1 and 2 | The kernel only calls a hook that is absent in the base kernel. | an image with a nonzero version |
| Device-node, FIFO and symlink inodes, the two extra types 0x3000 and 0x7000, and inline data | Only the kernel code was read; the held images are install media with none of them. | a root-disk image, e.g. an EFS system disk |
| Sparse files | No creator found; `offset` is honoured by the kernel but gaps were never seen. | a sparse file on a real disk or a read of the write path in `efs_bmapLogicalBlock` |
| Extent `magic` byte rejection | Not found in the kernel or fsck. | a read of `efs_inode_check` against a crafted extent |
| Meaning of `fs_heads`, `fs_sectors`, `fs_lastialloc` | Geometry/hint fields nothing reads. | none needed to mount |
| Root-mount path's treatment of a dirty superblock | the mode values passed by `efs_mountroot` were not enumerated. | read `efs_mountroot` call-sites |

## Reproduce

```
uv run --script efs-check.py IRIX-6.5.30-Overlays1.efs members-of-IP27boot-efs.a/
```

Extract the kernel members with `ar x usr/cpu/sysgen/IP27boot/efs.a`. The `cmp_IP30boot`, `cmp_IP35boot` and `cmp_IP32boot` directories used for the cross-platform run are the same extraction from the other kernels.
