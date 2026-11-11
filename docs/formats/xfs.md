# XFS on-disk format as written by IRIX 6.5.30

What the shipped IRIX 6.5.30 XFS code and tools write to and accept from disk, read from binaries only: superblock versions 1 to 4, directories v1 and v2, inodes v1 and v2, the attribute fork (including SGI's ACL, MAC and capability attributes), and the realtime subvolume. Everything is big-endian. No IRIX source, headers or comments were read. Names are symbols or strings that appear in the shipped binaries. Prose and tables are my own wording.

Companion: `XFS_DECODE.h` (repository root) states the layout in the publish-safe decode-header form, with generated Rust under `generated/disk-formats/`. `defs.toml` ties its constants and field positions to the tables on this page.

Grades: **established** = read directly in instructions or in a tool's own data tables; **inferred** = follows from what was read but a link is missing; **unknown** = not settled. A grade applies to the row or sentence it sits on.

## Objects read

| Short name | Path under `refs/irix-media/6.5.30/` | Used for |
|---|---|---|
| kernel XFS | `usr/cpu/sysgen/IP27boot/xfs.a` (members `xfs_mount.o`, `xfs_inode.o`, `xfs_utils.o`, `xfs_bmap.o`, `xfs_dir_leaf.o`, `xfs_dir.o`, `xfs_dir2*.o`, `xfs_attr*.o`, `xfs_da_btree.o`, `xfs_parent.o`) | superblock checks, version logic, inode read/write, directory and attribute code |
| realtime | `usr/cpu/sysgen/IP27boot/xfsrt.a` (`xfs_rtalloc.o`) | realtime bitmap, summary, grow, pick |
| ACL / capability | `usr/cpu/sysgen/IP27boot/acl.a` (`acl.o`), `acl_xfs.o`, `xfs_attr_fetch.o`, `capability.a` (`capability.o`), `macstubs.a` | SGI attribute names and value layouts |
| `xfs_db` | `usr/sbin/xfs_db` (N32, stripped) | the debugger's field tables: names, byte and bit offsets, widths, per-version presence |
| `xfs_repair` | `usr/sbin/xfs_repair` (N32, stripped) | attribute value validation |
| `mkfs_xfs` | `sbin/mkfs_xfs` (N32, stripped) | option names and messages only |
| libc | `lib32/libc.so.1` | `mac_valid`, `mac_size` |

Method. `xfs_db` carries its structure descriptions as data: arrays of 24-byte records `{name pointer, type code, bit offset, count, flags, aux}`. I parsed those arrays directly from the executable image (non-PIC, absolute pointers), so field names, offsets and widths below come from the tool's tables, not from guessing at load/store patterns. Where the table gives a bit offset it is counted from the most significant bit of the storage unit. The kernel objects carry no usable struct debug info (only function names), so every kernel offset is derived from access patterns and is labelled by position. In-core `xfs_mount` offsets quoted below are in-core only; the in-core superblock starts at mount+0x40, so `mount+0x40+x` is on-disk superblock offset `x`. In-core inode core starts at inode+0x278, so `inode+0x278+x` is on-disk inode offset `x`. There is no endian swap anywhere in the kernel module, so on-disk, in-core and in-log core bytes are identical (see also `xfs-timestamps.md`, `xfs-mount-blockio.md`).

The inode rows in 2.1 were re-read against `usr/sbin/xfs_db` (sha256 `d02f7cbc9029e8954973792228c019bf16cef197067dcdd727e3a914e57fc193`) and, for the kernel statements, against `usr/cpu/sysgen/IP27boot/xfs.a` (sha256 `6f38630b1d591266a0de66feedf52bfcd8ab3d7c0238e87078ec16aa1071ddca`), both 6.5.30. Field widths are the gaps between successive table offsets.

Public Linux XFS was used only to orient which structures to look for. Nothing here is graded from Linux.

Boards. Kernel statements are from the IP27boot archive. The IP35boot archive was compared for `xfs_mount_validate_sb` and the timestamp code in the earlier notes and matched; the other kernel statements here were not repeated on IP35boot, IP30boot or IP32boot. `xfs_db64`, `xfs_repair64` and `xfs_check64` were not read; `xfs_check` is a shell wrapper.

---

## 1. Superblock

### 1.1 Location and size

- Sector 0 of the data device holds the primary superblock. The kernel reads exactly one 512-byte block there (`xfs_readsb`), validates it, and copies exactly 0xd0 bytes into the in-core copy. Established (earlier note, re-checked).
- `xfs_db` knows fields up to and including `features2` at 0xc8 (4 bytes), so the structure it describes ends at 0xcc. Bytes 0xcc to 0xcf are copied by the kernel but have no name in `xfs_db`. Contents unknown (assume zero).
- Every allocation group begins with a copy of the superblock. `xfs_db`'s `version` command writes the changed word "into all AGs" (string: "failed to set versionnum in AG %d"). Established. Per-AG copies of the counters (icount, ifree, fdblocks, frextents) are not kept in step; the kernel only uses the AG 0 copy for counts and recounts from the AGF/AGI at mount (not traced here). Inferred.
- Magic: 0x58465342 (`XFSB`). Established (validate_sb loads and compares).

### 1.2 Field table

Offsets and widths from the `xfs_db` superblock table (established for every row unless the grade column says otherwise). "Use" cites the kernel or tool code that reads the field.

| Off | Size | Field | Use / evidence |
|---:|---:|---|---|
| 0x00 | 4 | `magicnum` | 0x58465342; validate_sb |
| 0x04 | 4 | `blocksize` | bytes; must be nonzero; `blocklog` must satisfy 9..16 and blocksize*rextsize must be in [0x1000, 0x40000000] (validate_sb) |
| 0x08 | 8 | `dblocks` | nonzero; must fit `agcount*agblocks >= dblocks` and `(agcount-1)*agblocks + 0x40 <= dblocks` (validate_sb) |
| 0x10 | 8 | `rblocks` | realtime size in fs blocks; 0 means no realtime subvolume (`xfs_rtmount_init`) |
| 0x18 | 8 | `rextents` | `rblocks / rextsize` (`xfs_growfs_rt`) |
| 0x20 | 16 | `uuid` | mount does a duplicate-UUID check (`xfs_uuid_mount`) |
| 0x30 | 8 | `logstart` | fs block of internal log; **0 means an external log** (validate_sb refuses data and log on the same device in that case) |
| 0x38 | 8 | `rootino` | |
| 0x40 | 8 | `rbmino` | realtime bitmap inode |
| 0x48 | 8 | `rsumino` | realtime summary inode |
| 0x50 | 4 | `rextsize` | blocks per realtime extent |
| 0x54 | 4 | `agblocks` | |
| 0x58 | 4 | `agcount` | nonzero |
| 0x5c | 4 | `rbmblocks` | realtime bitmap length in blocks |
| 0x60 | 4 | `logblocks` | |
| 0x64 | 2 | `versionnum` | section 1.3 |
| 0x66 | 2 | `sectsize` | must be > 0 |
| 0x68 | 2 | `inodesize` | must be 0x100..0x800 |
| 0x6a | 2 | `inopblock` | |
| 0x6c | 6 | `fname` | volume name, first half; `xfs_db` prints two 6-byte halves |
| 0x72 | 6 | `fpack` | volume name, second half (so a 12-byte name at 0x6c) |
| 0x78 | 1 | `blocklog` | 9..16 |
| 0x79 | 1 | `sectlog` | |
| 0x7a | 1 | `inodelog` | |
| 0x7b | 1 | `inopblog` | |
| 0x7c | 1 | `agblklog` | |
| 0x7d | 1 | `rextslog` | `floor(log2(rextents))`; summary has `rextslog+1` levels (`xfs_rtmount_init`) |
| 0x7e | 1 | `inprogress` | nonzero means mkfs/grow was interrupted; mount refuses (validate_sb) |
| 0x7f | 1 | `imax_pct` | must be < 101 |
| 0x80 | 8 | `icount` | allocated inodes |
| 0x88 | 8 | `ifree` | free inodes |
| 0x90 | 8 | `fdblocks` | free data blocks |
| 0x98 | 8 | `frextents` | free realtime extents |
| 0xa0 | 8 | `uquotino` | user quota inode |
| 0xa8 | 8 | `pquotino` | **named `pquotino` in the 6.5.30 tool** (project quota inode); the same slot is called the group quota inode in Linux. Which of project/group quota a given filesystem stored here is not settled by the binaries read: unknown |
| 0xb0 | 2 | `qflags` | quota state flags; bit values not enumerated here |
| 0xb2 | 1 | `flags` | bit 0x01 is set by unmount when a "shared" filesystem is marked read-only (`mkfs_xfs` simulator string "Unmounting, marking shared read-only"). Other bits unknown |
| 0xb3 | 1 | `shared_vn` | must be 0 or mount fails once `versionnum` has the more-bits flag (validate_sb). Established for the check, meaning unknown |
| 0xb4 | 4 | `inoalignmt` | inode chunk alignment in blocks; used when the ALIGN bit is set (`xfs_mountfs_int` computes an alignment mask from it) |
| 0xb8 | 4 | `unit` | data stripe unit in blocks; used when the DALIGN bit is set (feeds the in-core `m_dalign`) |
| 0xbc | 4 | `width` | stripe width in blocks; same |
| 0xc0 | 1 | `dirblklog` | v2 directory block size is `1 << (blocklog + dirblklog)` bytes (`xfs_dir2_mount`) |
| 0xc1 | 1 | `logsectlog` | v2 log sector size log (field present in the tool; kernel use inferred from the LOGV2 bit and `mkfs_xfs` message "log stripe unit specified, using v2 logs") |
| 0xc2 | 2 | `logsectsize` | same |
| 0xc4 | 4 | `logsunit` | log stripe unit; same |
| 0xc8 | 4 | `features2` | section 1.4 |

Not present in this release's superblock as far as the tools and the 0xd0 copy show: any checksum, `meta_uuid`, `features_compat/ro/incompat`, `sb_crc`, inode btree counters. Established for the tools' tables and the copy length; a v5 superblock cannot be mounted (see `xfs-mount-blockio.md`).

### 1.3 `versionnum`

The low nibble is the **version**; above it are **feature bits**. The kernel check (established, `xfs_mount_validate_sb`):

- 0 is rejected.
- Values 1, 2 and 3 are accepted as they stand. No feature-bit check runs for them (the check is skipped when `versionnum < 4`).
- Values with `versionnum >= 4` must have low nibble exactly 4. Nibbles 0 and 5 or higher are rejected (this is what stops a v5 superblock).
- Bit 0x0800 is rejected on v4. Established: the mask used is `0xffff0800`, so only 0x0800 is tested. Other bits are not rejected here.
- If bit 0x8000 is set, `features2` (0xc8) must not have any bit outside 0x1b (so 0x01, 0x02, 0x08, 0x10 allowed) and `shared_vn` must be 0.

What each version means (established from `xfs_db`'s `version` printer and from the kernel's upgrade code; the printer names a version "V1".."V4"):

| Version | Attribute forks | 32-bit link counts (v2 inodes) | Feature bits | Grade |
|---|---|---|---|---|
| 1 | not supported | not supported | none; any bits ignored | established |
| 2 | implied present (printer shows ATTR) | no | none | established |
| 3 | implied present | implied present (printer shows ATTR, NLINK) | none | established |
| 4 | bit 0x0010 | bit 0x0020 | bits below | established |

Established: for versions 2 and 3 the printer lists the implied ATTR (and for 3, NLINK) without any bit test. For version 4 each is a bit test.

Upgrade rules (established in `xfs_bmap_add_attrfork` and `xfs_bump_ino_vers2`):

- Creating the first attribute fork on version 1 sets `versionnum = 2`. On version 2 or 3 nothing changes. On version 4 without 0x0010 it ORs in 0x0010.
- Making any inode version 2 (`xfs_bump_ino_vers2`): on version 1 or 2 the superblock becomes **version 3**; on version 4 it ORs in 0x0020; version 3 and version 4 with 0x0020 are left alone. Either case logs the `versionnum` field (mask 0x8000 in the logged-field set).
- So a filesystem never goes from version 3 to 4 by the kernel; version 4 is a mkfs-time choice (inferred: no kernel path sets it, and `mkfs_xfs` has no option text for it other than the feature options below; how `mkfs_xfs` assembles the word was not traced).

Feature bits on version 4 (names are those printed by `xfs_db`'s `version` command, established; semantics as read in the kernel where noted):

| Bit | `xfs_db` name | Meaning and evidence |
|---:|---|---|
| 0x0010 | ATTR | filesystem may contain attribute forks; `xfs_db` check reports "sb versionnum missing attr bit" if a fork exists without it |
| 0x0020 | NLINK | v2 inodes allowed; check reports "missing nlink bit" |
| 0x0040 | QUOTA | quota in use; check reports "missing quota bit"; mount tests it with `qflags` before mounting quotas |
| 0x0080 | ALIGN | inode chunks aligned to `inoalignmt`; check reports "extra align bit" when set with no aligned chunks |
| 0x0100 | DALIGN | `unit`/`width` are valid stripe values; mount copies them to in-core dalign/swidth |
| 0x0200 | SHARED | "shared" (clustered, read-only after unmount) filesystem; with `shared_vn` and `flags` bit 0x01 |
| 0x0400 | LOGV2 | version 2 log (stripe-aware); `xfs_log_mount` tests an in-core flag set from it |
| 0x0800 | (SECTOR in Linux) | not printed by `xfs_db`; **rejected** by the kernel on 6.5.30 |
| 0x1000 | EXTFLG | unwritten extents allowed; without it every bmap record must have the extent flag clear (`xfs_check_nostate_extents` runs on read when the bit is clear) |
| 0x2000 | DIRV2 | directories use format v2; if clear, v1 (section 3) |
| 0x4000 | (borg) `xfs_db` prints nothing; `mkfs_xfs` symbol `xfsv2ci_dirops` | case-insensitive v2 directory mode (section 3.6). Needs 0x2000 |
| 0x8000 | MOREBITS | `features2` at 0xc8 is valid |

Established: the name printer in `xfs_db` knows exactly ATTR, NLINK, QUOTA, ALIGN, DALIGN, SHARED, DIRV2, LOGV2, EXTFLG, MOREBITS (and the two features2 names below). The mapping of 0x0800 and 0x4000 comes from the kernel tests only.

### 1.4 `features2` (when 0x8000 is set)

| Bit | Name (`xfs_db`) | Meaning |
|---:|---|---|
| 0x01 | (unnamed) | accepted by validate_sb; use not found. Unknown |
| 0x02 | LAZYSBCOUNT | superblock counters are lazily updated (`mkfs_xfs` option `lazy-count`; kernel `xfs_log_sbcount` exists) |
| 0x08 | ATTR2 | "v2 inline attributes": the attribute-fork offset is allowed to move as the data fork changes (`xfs_db` help: "attr1 / attr2: enable v1 / v2 inline extended attributes"). Kernel: `xfs_bmap_add_attrfork` and `xfs_attr_shortform_bytesfit` consult it |
| 0x10 | PARENT | parent-pointer attributes (section 4.7) |

Bits 0x04 and everything above 0x1f are rejected by the kernel (mask 0x1b), so 0x04 is also rejected. Established.

### 1.5 Geometry rules (established)

`validate_sb` additionally requires: `agcount` and `sectsize` nonzero, `blocklog` in 9..16, `inodesize` in 0x100..0x800, `imax_pct` below 101, `dblocks` nonzero, and the AG-count arithmetic above. `inprogress != 0` is a refusal. All geometry failures return the "corrupted" errno class (0x3f2), version failures the "wrong type" class (0x3f3).

---

## 2. Inodes

### 2.1 Core (0x60 bytes) and fork area

Offsets and widths from the `xfs_db` inode table; presence by version is from the table's per-field conditions (decoded below). The core is `bcopy`-ed as 0x60 bytes between disk and memory (earlier notes).

| Off | Size | Field | v1 | v2 | Grade / note |
|---:|---:|---|---|---|---|
| 0x00 | 2 | `magic` | | | 0x494e (`IN`); read, checked and written in many places. Established |
| 0x02 | 2 | `mode` | | | `mode == 0` means unused inode: `xfs_iread` skips the format parse and only fills version, generation, flush counter. Established |
| 0x04 | 1 | `version` | 1 | 2 | Established |
| 0x05 | 1 | `format` | | | data fork format: 0 dev, 1 local, 2 extents, 3 btree, 4 uuid. Names are the `xfs_db` enum. The kernel's `xfs_iformat` accepts 0 for char/block/fifo/socket, and 1, 2, 3 for regular, directory and symlink; **format 4 is not accepted by the kernel read path** (falls through to the corruption error). Established |
| 0x06 | 2 | `onlink` / `nlinkv1` | link count (16 bit) | zero | Established |
| 0x08 | 4 | `uid` | | | Established (32-bit field at bit 64 of the `xfs_db` inode table) |
| 0x0c | 4 | `gid` | | | Established (32-bit field at bit 96 of the `xfs_db` inode table) |
| 0x10 | 4 | `nlinkv2` | ignored on read | link count (32 bit) | Established (see 2.2 for what v1 writeback leaves here) |
| 0x14 | 2 | `projid` | ignored on read, treated 0 | project id | Established |
| 0x16 | 8 | pad | | zeroed by the kernel when it makes an inode v2 | Established that the kernel zeroes 8 bytes at 0x16; `xfs_db` has no name for them |
| 0x1e | 2 | `flushiter` | | | incremented on every inode flush; wraps and resets at 0xffff. Offset established (`xfs_db` table, bit 240). Counter behaviour established: `xfs_iflush_int` adds one to the in-core halfword and stores zero when the result equals 0xffff (6.5.30 IP27boot `xfs.a(xfs_inode.o)` +0x3510 and +0x3628) |
| 0x20 | 4+4 | `atime` sec, nsec | | | seconds then nanoseconds (earlier timestamp note). Established |
| 0x28 | 8 | `mtime` | | | Established offset and encoding: bit 320 in the `xfs_db` table, the same two-word type code as `atime` (seconds then nanoseconds) |
| 0x30 | 8 | `ctime` | | | Established offset and encoding: bit 384, same type code as `atime` |
| 0x38 | 8 | `size` | | | Established offset and 64-bit width (bit 448 to the `nblocks` field at bit 512); that it is the file size in bytes is inferred from the name |
| 0x40 | 8 | `nblocks` | | | Established offset and 64-bit width (bit 512 to bit 576); that it counts filesystem blocks is inferred from the name |
| 0x48 | 4 | `extsize` | | | extent size hint. Established offset and 32-bit width (bit 576 to bit 608); the hint role is inferred from the name |
| 0x4c | 4 | `nextents` | | | data-fork extent count. Established offset and 32-bit width (bit 608 to bit 640); role inferred from the name |
| 0x50 | 2 | `naextents` | | | attribute-fork extent count. Established offset and 16-bit width (bit 640 to bit 656, the `forkoff` byte); role inferred from the name |
| 0x52 | 1 | `forkoff` | | | attribute fork starts at `0x64 + forkoff*8`; 0 means no attribute fork. Established |
| 0x53 | 1 | `aformat` | | | attribute fork format: 1 local, 2 extents, 3 btree (the only values `xfs_iformat` accepts for the attribute fork). Established: offset 0x53 from the `xfs_db` table (bit 664); the kernel's attribute-fork switch compares the byte at +0x53 against 1, 2 and 3 and any other value takes the error exit (6.5.30 IP27boot `xfs.a(xfs_inode.o)`, `xfs_iformat` 0x5e0 to 0x600) |
| 0x54 | 4 | `dmevmask` | | | Established offset and 32-bit width (bit 672 to bit 704); role (the data-management event mask) is inferred from the name |
| 0x58 | 2 | `dmstate` | | | Established offset and 16-bit width (bit 704 to bit 720); role inferred from the name |
| 0x5a | 2 | `flags` | | | bits below. Established (16-bit field at bit 720; the nine flag bits are table entries at bits 727 to 735) |
| 0x5c | 4 | `gen` | | | generation. Established offset and 32-bit width (bit 736 to the next table entry at bit 768); role inferred from the name |
| 0x60 | 4 | `next_unlinked` | | | AGI unlinked-list link. Established offset (a separate `xfs_db` entry beside the core, at bit 768); the list role is inferred from the name |
| 0x64 | to end of inode | data fork `u` (and attribute fork) | | | Established start: the `xfs_db` table places `u` at bit 800 and `a` after it; the attribute fork start is given by `forkoff` above |

`flags` bits (from the `xfs_db` table; the 16-bit word is at 0x5a, bit values below are as a 16-bit integer): 0x0001 realtime, 0x0002 prealloc, 0x0004 newrtbm, 0x0008 immutable, 0x0010 append, 0x0020 sync, 0x0040 noatime, 0x0080 nodump, 0x0100 rtinherit. Established.

Presence by version: the `xfs_db` table gates `nlinkv1` on `version == 1`, and `nlinkv2`, `onlink` and `projid` on `version == 2`. So `xfs_db` shows `onlink` (offset 6) for a v2 inode as well, and the kernel keeps it zero there. Established.

`newrtbm` (0x0004) on the realtime bitmap inode means the inode's `atime` field is used as a **64-bit allocation sequence counter** (see 5.5). Established.

### 2.2 v1 versus v2 behaviour

- A v1 inode has a 16-bit link count at offset 6, no project id, and nothing at 0x10 to 0x1d. Reading a v1 inode (`xfs_iread`): in-core link count = the 16-bit value, in-core `onlink` = 0, in-core `projid` = 0. Established.
- A v2 inode has the 32-bit link count at 0x10, project id at 0x14, 8 pad bytes at 0x16, and `onlink` = 0 at 6. Established.
- New inodes (`xfs_ialloc`): created as version 1; if the superblock has NLINK (version 3, or version 4 with 0x0020) they become version 2 at allocation. A nonzero project id on a version-1 inode calls `xfs_bump_ino_vers2` immediately. Established.
- Link count overflow (`xfs_bumplink`): fails (errno 0x1f) above 0x7fffffff; if the inode is still version 1 and the count would exceed 0xffff, the inode is bumped to version 2 first. Established.
- Writeback (`xfs_iflush_int`): after copying the core, a dirty **version 1 inode on a filesystem that has NLINK is rewritten as version 2** (version byte 2, `onlink` 0, 8 pad bytes at 0x16 zeroed). On a filesystem without NLINK the 16-bit link count is stored at offset 6; because the core copy happens first, the 32-bit field at 0x10 then also holds the same count on disk (duplicate), and the project id word is whatever the in-core zero is. Established for the order; that real v1 disks written by IRIX carry the duplicate is inferred from the copy order.
- `xfs_db`'s `check` reports a missing NLINK bit if any v2 inode exists on a filesystem without it.

### 2.3 Data fork (`u`, from 0x64)

- **dev** (format 0): a single 32-bit device number at 0x64 (`xfs_iformat` loads one word from 0x64). The encoding of that word (major/minor split) is not settled here. Unknown.
- **local** (format 1): bytes are the file body: symlink target, or a shortform directory (sections 3.1 and 3.4).
- **extents** (format 2): `nextents` records of 16 bytes (section 2.4).
- **btree** (format 3): a root header `{level 16 bit, numrecs 16 bit}` followed by keys and pointers sized to fill the fork (section 2.5).

The literal area is `inodesize - 0x64`. The default attribute-fork offset the kernel picks at mount: for 256-byte inodes `literal - 0x24` (120 bytes); for 512, 1024 and 2048-byte inodes a fixed 0xc4 (196) bytes, stored as `forkoff = value >> 3`. Other sizes: zero (no default). Established (`xfs_mountfs_int`).

### 2.4 Extent record (16 bytes), from the `xfs_db` tables

Bit offsets from the most significant bit of the 128-bit record:

| Bits | Width | Field |
|---:|---:|---|
| 0 | 1 | `extentflag` (1 = unwritten) |
| 1 to 54 | 54 | `startoff` (file block) |
| 55 to 106 | 52 | `startblock` (fs block, or rt block for realtime files) |
| 107 to 127 | 21 | `blockcount` |

Established. The flag is only allowed when EXTFLG is set (section 1.3).

### 2.5 Bmap btree

Long-form block header: `magic` 0, `level` 4 (16 bit), `numrecs` 6 (16 bit), `leftsib` 8 (64 bit), `rightsib` 0x10 (64 bit); records start after 0x18. Keys are the 8-byte `startoff`; pointers are 8-byte fs blocks. Magic 0x424d4150 (`BMAP`) appears in the kernel. Established for field offsets; the root-in-inode layout (keys then pointers laid out for the fork's capacity) follows the `xfs_db` `bmroot` table: `level` 0, `numrecs` 2, then keys and ptrs. Inferred that the pointer array begins at the maximum-records boundary for the fork size, as in the public layout; not traced.

---

## 3. Directories

Directory version is a filesystem property: bit 0x2000 of `versionnum` (with low nibble 4) selects v2; all other filesystems use v1. At mount the kernel installs a table of ten directory operations (`xfs_dir_*` for v1, `xfs_dir2_*` for v2); a third table, identical except for the name comparator, is installed when bit 0x4000 is set with 0x2000. Established (`xfs_mountfs_int` selects among three tables in `.srdata`; relocation targets in `xfs_dir.o` and `xfs_dir2.o`).

Directory blocks use the "da" btree machinery common to directories and attributes. Shared pieces:

- **Hash** (`xfs_da_hashname`), over the name bytes, 32 bit: for each full group of four bytes `h = (b0<<21) ^ (b1<<14) ^ (b2<<7) ^ b3 ^ rotl(h, 28)`; a tail of 3, 2 or 1 bytes uses the same shape with rotations 21, 14 and 7. Established (disassembled). Attribute names use the same function.
- **Block info header** (12 bytes, at the start of every da block: leaf, node, attribute leaf): `forw` 0 (32 bit block), `back` 4, `magic` 8 (16 bit), `pad` 10. Established (`xfs_db`). Magics: da node 0xfebe, directory v1 leaf 0xfeeb, attribute leaf 0xfbee, v2 leaf1 0xd2f1, v2 leafn 0xd2ff. All five appear as compares in the respective kernel members. Established.
- **da node**: info (12 bytes), `count` at 0xc, `level` at 0xe, entries from 0x10, each `{hashval 4, before 4}`. Established.

### 3.1 v1 shortform (data fork format local)

Established (`xfs_db`, plus `xfs_dir_shortform_create` and `_addname`):

- Header, 9 bytes: `parent` inode number (**64 bit**) at 0, `count` (8 bit) at 8.
- Entries from 9, unsorted, appended at the end: `inumber` (**64 bit**) at 0, `namelen` (8 bit) at 8, `name` at 9 (not terminated, no padding). Entry size is `namelen + 9`.
- Inode `size` = 9 + the sum of entry sizes (the kernel updates the size by `namelen + 9` per add). Creating an empty shortform directory sets size 9.
- Duplicate name on add returns errno 0x11 (EEXIST); no room when the size would exceed the literal area is handled by converting to a leaf (`xfs_dir_shortform_to_leaf`). The exact threshold is the fork's literal capacity; not separately derived.

### 3.2 v1 leaf (single block, hashed)

Block layout, established from `xfs_db` and the `xfs_dir_leaf_add*` code:

- Header (0x20 bytes): info (12 bytes, magic 0xfeeb), `count` 0xc (16 bit), `namebytes` 0xe, `firstused` 0x10, `holes` 0x12 (8 bit), `pad1` 0x13, `freemap[3]` from 0x14, each `{base 16 bit, size 16 bit}`.
- Entry table from 0x20, 8 bytes each, **sorted by hash**: `hashval` 0, `nameidx` 4 (16 bit offset of the name inside the block), `namelen` 6 (8 bit), `pad2` 7.
- Names stored from the end of the block downward (`firstused` is the lowest name offset). A name record is the **64-bit inode number then the name bytes**, no terminator, no padding: size `8 + namelen`. The same record layout as shortform entries minus the length byte.
- `freemap` entries track free spans; adding an entry that abuts the table end shifts the first span's base by 8 and shrinks its size by 8.
- Inode number is 8 bytes here, as in shortform. Established.

### 3.3 v1 node (more than one block)

A leaf that fills up becomes a da btree (`xfs_dir_leaf_to_node`): a da node root (magic 0xfebe) pointing at v1 leaves, with hashvals in nodes. Block addressing is by logical file block within the directory. Node entries and info as above. Structure established from `xfs_db`; how logical block numbers are assigned to nodes versus leaves (any fixed ordering) was not traced. Unknown.

### 3.4 v2 shortform

Established (`xfs_db`):

- Header: `count` (8 bit) at 0, `i8count` (8 bit) at 1, then `parent` at 2, **4 bytes when `i8count == 0`, 8 bytes when `i8count > 0`**. `i8count` is the number of entries whose inode number needs 8 bytes; when it is nonzero every inumber (including the parent) is stored as 8 bytes.
- Entry: `namelen` (8 bit) at 0, `offset` (16 bit) at 1, `name` at 3, then `inumber` (4 or 8 bytes by the header rule) immediately after the name. `offset` is the position the entry would have if the directory were converted to block form (the tool labels it `offset`; its value semantics are the public ones, inferred).

### 3.5 v2 block, data, leaf, node, free

Block size is `1 << (blocklog + dirblklog)` bytes (section 1.2). The directory file's logical space is split (established: `xfs_dir2_mount` sets the leaf-space start to 1<<35 and the free-space start to 1<<36 in the offset arithmetic, i.e. 32 GiB and 64 GiB byte offsets in an 8-byte-granular directory offset space):

- Data space from offset 0.
- Leaf space from 32 GiB.
- Free (bestfree index) space from 64 GiB.

Block formats (names, offsets and widths from `xfs_db`, established; magics are compares in the kernel members `xfs_dir2_block.o`, `xfs_dir2_data.o`, `xfs_da_btree.o`, `xfs_dir2_node.o`):

- **Data block** (also the data part of a **block-form** directory): magic at 0 (0x58443244 `XD2D`; block-form uses 0x58443242 `XD2B`), then `bestfree[3]` at 4, each `{offset 16 bit, length 16 bit}`; entries follow from 0x10.
  - Live entry: `inumber` (64 bit) at 0, `namelen` (8 bit) at 8, `name` at 9, then a 16-bit `tag` at the end (the tag is the entry's own offset in the block).
  - Unused span: `freetag` (16 bit, value 0xffff) at 0, `length` (16 bit) at 2, and the `tag` in the last two bytes.
  - Entry alignment is 8 bytes (inferred from the public layout and from the 3-bit shift in the offset space; the rounding was not traced).
- **Block form** (one block holding both data and leaf): header as a data block; the last 8 bytes are the tail `{count 32, stale 32}`; immediately before it, `count` leaf entries `{hashval 32, address 32}` sorted by hash, where `address` is the entry's byte offset in 8-byte units (the `offset` shortform value uses the same unit). Established for the tail and leaf-entry fields; the placement "entries grow down from the tail" follows the same `xfs_db` table (`bleaf`, `btail`).
- **Leaf1** (magic 0xd2f1): info (12), `count` at 0xc (16), `stale` at 0xe (16), entries from 0x10 `{hashval, address}`; then the `bests` array (16-bit free-length per data block) and a trailing 32-bit `bestcount` tail.
- **Leafn** (magic 0xd2ff): same header and entries, no bests; used with a da node tree.
- **Free block** (magic 0x58443246 `XD2F`): magic 0, `firstdb` 4 (32), `nvalid` 8 (32), `nused` 0xc (32), `bests[]` 16-bit from 0x10.
- **Data entry address and tag**: address is in 8-byte units of the data-space byte offset (shifted by 3).

### 3.6 v2 case-insensitive variant (0x2000 plus 0x4000)

The second v2 operation table swaps the name comparator from `strncmp` to `strncasecmp`, and the v2 hash routine (`xfs_dir2_hashname`) lowercases the name (`strntolower`) before hashing. `mkfs_xfs` calls this `xfsv2ci_dirops` and exposes it as a `-n version=` choice (the option text is `-n log=n|size=num,version=n`; the literal `ci` value is inferred from the symbol). The `mkfs_xfs` summary line prints "naming =version %d bsize=%d mixed-case=%c". Established for the table and hash behaviour; the exact 0x4000 flag is inferred from the two tests in `xfs_mountfs_int` that gate this table on 0x2000 and 0x4000 together.

---

## 4. Attribute fork

### 4.1 Where and how big

The attribute fork starts at inode byte `0x64 + forkoff*8` and runs to the end of the inode. It is read through the same three formats as the data fork (`aformat` 1 local, 2 extents, 3 btree). Attribute names are looked up by hash with the same function as directories. Established.

An attribute name is a byte string up to 255 bytes (name length is an 8-bit field). A value is up to 64 KiB (value length is 16 bit in local entries and 32 bit in remote entries; the kernel limit for a single value was not traced). Established for field widths.

Namespaces. Each entry has flag bits: 0x01 LOCAL (leaf only; value stored in the entry), 0x02 **ROOT** (the privileged namespace; names that are not user attributes), 0x08 **PARENT**, 0x80 INCOMPLETE (an interrupted add; invalid on disk). Established (`xfs_db` leaf-entry flags: `incomplete`, `root`, `local`, `parent` at bits 0x80, 0x02, 0x01, 0x08; shortform entries carry `root` and `parent` only). A user attribute has none of these flags.

### 4.2 Shortform (aformat 1)

Established:

- Header, 4 bytes: `totsize` (16 bit) at 0, `count` (8 bit) at 2, one pad byte at 3.
- Entry: `namelen` (8 bit) at 0, `valuelen` (8 bit) at 1, `flags` (8 bit) at 2, `name` at 3 followed immediately by `value`. No terminators or padding.
- Shortform values are therefore at most 255 bytes. `xfs_repair` treats an INCOMPLETE flag in a shortform entry as an error. Established.

### 4.3 Leaf (magic 0xfbee), single block

Established (`xfs_db` and `xfs_attr_leaf_newentsize`):

- Header 0x20 bytes: info (12, magic 0xfbee), `count` 0xc, `usedbytes` 0xe, `firstused` 0x10 (16 bit each), `holes` 0x12 (8 bit), `pad1` 0x13, `freemap[3]` from 0x14, each `{base 16, size 16}`.
- Entry table from 0x20, 8 bytes each, sorted by hash: `hashval` 0 (32), `nameidx` 4 (16 bit offset into the block), `flags` 6 (8 bit), `pad2` 7.
- Name records stored from the block end downward:
  - **Local** record (flag 0x01): `valuelen` (16) at 0, `namelen` (8) at 2, `nameval` from 3 (name immediately followed by value). Record size rounded up to 4: `(3 + namelen + valuelen + 3) & ~3`.
  - **Remote** record (flag 0x01 clear): `valueblk` (32) at 0 (logical block of the value in the attribute fork), `valuelen` (32) at 4, `namelen` (8) at 8, `name` from 9. Record size `(namelen + 14) & ~3`.
- A value is stored local when its local record size is less than three quarters of the block size, otherwise remote (`xfs_attr_leaf_newentsize`: `(bsize>>1) + (bsize>>2)`). Established.

### 4.4 Node / multi-block

Attribute btrees reuse the da node (magic 0xfebe) over attribute leaves, with logical block 0 as the root. Remote values occupy further blocks of the attribute fork starting at `valueblk`. Structure established from `xfs_db` (`nvlist`, `hdr`, `entries`, `btree`). Value-block framing (whether remote value blocks carry a header) not traced. Unknown.

### 4.5 Namespace storage for SGI attributes

SGI's security and ACL data is stored as ROOT-namespace attributes with fixed names. Established: the kernel helper `xfs_attr_fetch` builds its request with the root flag set (the flags word in the argument block is 2) before fetching, and the capability fetch passes flags 2 as well. The names (ASCII, no terminator on disk) are:

| Attribute name | Where seen | What it is |
|---|---|---|
| `SGI_ACL_FILE` | `acl.o`, `acl_xfs.o`, `xfs_repair` | the file's access ACL |
| `SGI_ACL_DEFAULT` | `acl.o`, `xfs_repair` | the default ACL inherited by new children (directories) |
| `SGI_CAP_FILE` | `capability.o`, `xfs.a` strings, `xfs_repair` | file capability sets |
| `SGI_MAC_FILE` | `efs.a`, `specfs.a`, `xfs_repair`, `attrinit` | MAC label (see 4.8); `xfs_repair`'s message calls it `SGI_MAC_LABEL`, a mismatch in the tool's text, not on disk |
| `SGI_MAC_POINTER` | `specfs.a` | MAC label indirection for device nodes; not an XFS attribute |

`xfs_repair` accepts a shortform attribute as one of the four named ones only for ROOT entries and matches by prefix with the lengths 12, 15, 12, 12 (strncmp). Established.

### 4.6 ACL value (`SGI_ACL_FILE`, `SGI_ACL_DEFAULT`)

Established (`acl_invalid`, `acl_sync_mode`, `acl_xfs_iaccess`, `xfs_repair`):

- Value: `acl_cnt` (32 bit signed) at 0, then `acl_cnt` entries of **12 bytes** from offset 4. Maximum 25 entries (`acl_invalid` rejects counts of 26 or more), so the value is at most 304 bytes. Both `acl_xfs_iaccess` and `xfs_repair` use a 304-byte (0x130) buffer, and `xfs_repair` zero-fills it before copying the value.
- Entry: `a_type` (32 bit) at 0, `a_id` (32 bit uid or gid) at 4, `a_perm` (16 bit) at 8, 2 pad bytes. Established for type, id, perm positions (`acl_invalid` compares type at +0 and id at +4; `acl_sync_mode` stores perm at +8).
- `a_type` values read: 0x01 user-object, 0x02 named user, 0x04 group-object, 0x08 named group, 0x10 mask, 0x20 other. Established (the validity check names these bit values by compare; the role labels are mine from usage).
- Validity (established): exactly one user-object, one group-object and one other entry; at most one mask; if any named user or group entry exists then exactly one mask is required; no duplicate named entries (same type and id).
- Permission bits are the usual 3-bit r/w/x in `a_perm` (the code shifts the mode word by 6, 3 and 0 and masks with 7).
- The mode and ACL stay in step: after a mode change `acl_sync_mode` rewrites the user-object, group-object (or mask when present) and other permissions from the mode. Established.

### 4.7 Capability value (`SGI_CAP_FILE`)

Established: the value is **24 bytes**, three consecutive 64-bit capability masks (`cap_vtocap` fetches with a length of 0x18; `xfs_repair` requires the stored length to be exactly 0x18; `cap_get` copies out 0x18).

Order. Inferred from the arithmetic in `cap_exec_newcap`: the word at +0x10 is combined with the process's word at +0x10 by AND to give the new set at +0x10 (the inheritable set); the word at +8 is `(process[8] & new[0x10]) | file[8]` (the permitted set); the word at +0 is `file[0] & new[8]` (the effective set). So the file attribute is {effective +0, permitted +8, inheritable +0x10}. The names of the three sets were read from the shape of the computation, not from a symbol. A flag-like test on bit 0 of the file's first word also appears in `cap_exec_newcap`; its meaning was not resolved. Unknown.

### 4.8 MAC label (`SGI_MAC_FILE`)

The shipped XFS module has only MAC stubs (`mac_xfs_getlabel` etc. are in `macstubs.a`), so the stored value is what `libc`'s `mac_valid` accepts. Established from `mac_valid` and `mac_size`:

- Maximum 508 (0x1fc) bytes. `xfs_repair` copies the value into a 508-byte buffer and calls `mac_valid`; `mac_size` returns 0x1fc when valid.
- Layout: byte 0 is a type letter in the range 'A' to 'T' (a jump table over 0x41..0x54), byte 1 is a second type letter and must be one of 'b', 'e', 'h', 'l' (0x62, 0x65, 0x68, 0x6c), byte 2 and byte 3 are level/grade bytes, 16-bit counts at +4 (first category count) and +6 (second category count), the sum of the two counts must be below 251, and the category list is 16-bit values from +8 (so the maximum size is 8 + 250*2 = 508). Established for the positions, counts and size bound; the roles of bytes 0 to 3 are labels from the checks only (for example 'b','e','h','l' are accepted second-type values; the names behind them are unknown).
- Whether a Trusted-IRIX-specific storage form exists beyond this struct is unknown: the MAC kernel module is not in the shipped media.

### 4.9 Parent pointers (`PARENT` feature, entry flag 0x08)

`xfs_parent.o` provides `xfs_attr_list_parents`, `xfs_get_parents`, `xfs_get_parentpaths`, `xfs_find_parent_ea_name`. Established: parent entries are attributes carrying the 0x08 flag, listed in the shortform and leaf entry tables. The listing callback reads the **first 8 bytes of the attribute name as the parent's inode number** and uses the **value as the file's name in that parent** (copies `valuelen` bytes into the result after looking up the parent inode and fetching its generation). Inferred for the name/value roles from the argument order. Whether the name carries extra bytes after the 8 (a dirent offset or hash) is unknown. The feature is in `features2` bit 0x10.

---

## 5. Realtime subvolume

### 5.1 Presence and geometry

Established (`xfs_rtmount_init`, `xfs_growfs_rt`):

- Present when `sb_rblocks != 0`. The realtime device is a separate device (the mount keeps a third device handle for it). If `rblocks != 0` and the realtime device is missing, mount fails with errno 7.
- `rextsize` (blocks per realtime extent) is fixed at creation; `xfs_growfs_rt` refuses a grow with a different extent size when a realtime volume already exists.
- `rextents = rblocks / rextsize`. `rbmblocks = ceil(rextents / (blocksize * 8))`. `rextslog = floor(log2(rextents))`. Summary levels = `rextslog + 1`. Summary size = `round_up_to_block(rbmblocks * levels * 4)`.
- At mount the last realtime block (`rblocks - 1`) is read to check the device is big enough; a short device fails.

### 5.2 Bitmap inode (`rbmino`)

An ordinary regular file in the filesystem (extent format) holding one bit per realtime extent.

- A bit is one realtime extent. **1 = free, 0 = allocated** (`xfs_rtallocate_range` clears bits; `xfs_rtfree_range` sets them). Established.
- Extent *n* is bit `n & 31` of the 32-bit word `(n >> 5) & wordmask` of bitmap block `n >> (blocklog + 3)`. Words are big-endian 32-bit; bit 0 is the **least significant bit** of the word (code builds masks with `1 << (n & 31)`; the "find next change" helper uses the lowest set bit). Established.
- File size is `rbmblocks * blocksize`. Fully mapped; reading a block is through the inode's block map (`xfs_bmapi_single`), not by physical address.
- After allocation of the file the bitmap is initialised as free for all `rextents` and with the tail bits of the last word treated per the grow code; the exact fill of padding bits beyond `rextents` was not traced. Unknown.

### 5.3 Summary inode (`rsumino`)

A regular file of 32-bit big-endian counters, indexed by `(level * rbmblocks + bitmap_block)`: the number of free-extent **runs** of length in `[2^level, 2^(level+1))` extents that begin in that bitmap block. Established: byte offset = `(level*rbmblocks + bbno) * 4`; counters are added to and subtracted from as signed words (`xfs_rtmodify_summary`), and the level used for a run is `highbit64(run_length_in_extents)` (`xfs_rtallocate_range`, `xfs_rtfree_range`). Runs that cross a bitmap block boundary are accounted to the block where they start (inferred from how the free-range code joins neighbouring runs; not separately proved).

### 5.4 Realtime file extents

Established: files with inode flag 0x0001 (realtime) use their normal extent records, but `startblock` is a **realtime block number** (in filesystem blocks, `rtextent_number * rextsize`, because the allocation result from `xfs_rtallocate_extent` is multiplied by `rextsize`), addressed within the realtime device, not an AG-relative fs block. `startoff` and `blockcount` are multiples of `rextsize` (the allocator aligns the request to `rextsize` and the result length is `rtextents * rextsize`). Inode flag 0x0100 (rtinherit) on a directory makes new files realtime. Established for the multiply; "multiples of `rextsize`" is inferred from the alignment helper calls.

### 5.5 `newrtbm` and the pick counter

`xfs_rtpick_extent` reads the 64-bit word at `atime` (inode offset 0x20) of the bitmap inode as a counter. If inode flag 0x0004 (newrtbm) is not set, it zeroes that word, sets the flag and logs the inode. Each pick increments the counter and uses it to compute a starting extent (a pseudo-random spread over the free space, bounded by `rextents`). Established for the field use. The arithmetic that maps the counter to a start was read only enough to confirm it uses the counter and `rextents`.

### 5.6 What was not read for realtime

`xfs_growfs_rt_alloc`'s zero-fill of new bitmap and summary blocks and the setting of padding bits; the realtime log and grio interactions (`xfs_grio.o` in `xfsrt.a`); `xfs_rtfind_back` bit-level boundary cases.

---

## 6. Other structures touched

Brief, for placement only (not graded to the same depth): AG header magics `XAGF` (0x58414746), `XAGI` (0x58414749), the free-space btrees (`ABTB`, `ABTC`) and inode btree (`IABT`) appear in the kernel members; `xfs_db` tables for AGF, AGI, AGFL and the btrees were located in the same data region and use the same record format. They are public-compatible in field names at the positions checked (AGF `magicnum`, `versionnum`, `seqno`, `length` at 0, 4, 8, 0xc; root and level words follow). Not otherwise verified here.

---

## 7. What `mkfs_xfs` tells us (options only)

Option grammar from the usage text: `-b log=n|size=num`; `-d agcount,agsize,file,name,size,sunit,swidth,su,sw,unwritten=0|1`; `-i log,perblock,size,maxpct,attr=0|1|2,parent=0|1`; `-l agnum,internal,size,version=n,sunit,su,lazy-count=0|1`; `-n log|size,version=n`; `-p prototype`; `-r extsize,size`. Established. Reading these with the bit tables: `-i attr=` chooses no attribute support / v1 attributes (ATTR bit) / v2 attributes (ATTR2 in `features2`, hence MOREBITS); `parent=1` selects the PARENT feature; `-l lazy-count` selects LAZYSBCOUNT; `-l version` and the "using v2 logs" message relate to LOGV2; `-n version` selects directory v1 or v2 (and the case-insensitive variant); `-d unwritten=1` selects EXTFLG. These pairings are inferred from names and messages; the code that builds `versionnum` in `mkfs_xfs` was not traced, so the **default** `versionnum` that IRIX 6.5.30's `mkfs_xfs` writes is unknown from this pass.

The `mkfs_xfs` summary lines print the chosen attr/parent/naming/log/realtime settings (`attr=%u, parent=%d`, `naming =version %d ... mixed-case=%c`, `log =... version=%d`, `lazy-count=%d`, `realtime =... extsz=%d`).

---

## 8. Open items

- Default `versionnum` and `features2` written by `mkfs_xfs` (code not traced).
- Encoding of the device number in a dev-format inode fork.
- `features2` bit 0x01 and `flags` bits other than 0x01 in the superblock.
- Whether the bytes at superblock 0xcc..0xcf are ever nonzero.
- Remote attribute value block framing; maximum single-value size enforced by the kernel.
- Meaning of the bit-0 test on the first word of `SGI_CAP_FILE` in `cap_exec_newcap`.
- Parent-pointer attribute names beyond the first 8 bytes.
- v1 directory node/leaf logical-block ordering.
- Realtime bitmap padding bits beyond `rextents`; cross-block summary accounting.
- Everything here is IP27boot kernel plus N32 userland. IP35boot, IP30boot and IP32boot kernels and the `*64` userland binaries were not compared.

## Reproduction

Scratch (not part of the findings): a local work directory holds the object extracts, capstone helpers (`tools/odis.py` for relocatable objects, `tools/udis_main.py` for the N32 executables), the `xfs_db` table parser (`tools/ptab.py`) and the raw disassembly (`dis/`, `ud/`).
