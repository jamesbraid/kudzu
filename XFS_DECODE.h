/*
 * XFS_DECODE.h - XFS superblock and inode core as the shipped release writes them
 *
 * Publish-safe contract for the first two structures of the XFS format as the
 * shipped IRIX release writes and accepts them: the superblock (versions 1 to 4)
 * and the inode core (versions 1 and 2). Every multi-byte integer is big-endian.
 * Directories, the attribute fork, the extent and btree records and the realtime
 * structures are described in prose only. In the extent record, bit offsets
 * counted from the most significant bit of the 128-bit record are: unwritten
 * flag at bit 0, file block at bits 1 to 54, filesystem block at bits 55 to 106,
 * block count at bits 107 to 127; the grammar cannot express a record wider
 * than 32 bits.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   FIELD: <WORD>.<field> <hi>:<lo> <grade>             a field of a 32-bit word
 *   ENUM: <target> <value> <label> <grade>
 *   TESTVEC: <value> as <WORD> => field=val, ...
 *   EXAMPLE (unverified): <value> as <WORD> => field=val, ...
 *
 * How an on-disk structure is written in this grammar. The grammar describes
 * 32-bit words and register blocks; these formats have no registers, only
 * big-endian structures. Each structure is therefore cut into aligned 32-bit
 * words read as one big-endian number, so the first byte of a word is bits 31:24.
 * A word is named <STRUCTURE>_W<n> where n is the byte offset divided by four,
 * counted from the start of that structure. There are no REG lines. A 64-bit
 * value is two words (name_hi, name_lo). A byte string (a UUID, a name, padding)
 * is not a field: it appears only as an _OFF_ and _LEN_ constant pair. A bitmask
 * byte is split into one single-bit field per named bit. Structures that repeat
 * (directory entries, partition entries, label entries) are described once, with
 * word numbers relative to the start of one entry, plus constants for where the
 * array starts, how many entries there are and the stride. A union is described
 * once per variant, each variant named after its tag, with word numbers counted
 * from the start of the enclosing structure. Fixed sizes and magic values are
 * plain constants. Algorithms (a checksum, an address calculation) are described
 * in words only.
 *
 * Derivation. Offsets and widths come from the format debugger's own field
 * tables, read directly from the shipped tool, and from the checks and update
 * paths of the shipped kernel module. The version word is shown split into the
 * version number (its low nibble) and one single-bit field per feature bit.
 * Version 1 to 3 superblocks imply the attribute and link-count features without
 * setting their bits. The superblock has no checksum, no feature words beyond
 * features2, and no metadata UUID in this release.
 */
#ifndef XFS_DECODE_H
#define XFS_DECODE_H

#include <stdint.h>

#define XFS_SB_MAGIC 0x58465342u /* established: bytes 58 46 53 42 */
#define XFS_SB_READ_BYTES 0xd0 /* established: bytes the kernel copies into its in-core superblock */
#define XFS_SB_DESCRIBED_BYTES 0xcc /* established: the format debugger names fields up to this end */
#define XFS_INO_MAGIC 0x494eu /* established */
#define XFS_INO_FORK_OFFSET 0x64 /* established: start of the data fork */
#define XFS_INO_ATTR_FORK_UNIT 8 /* established: forkoff counts units of this many bytes */
#define XFS_INO_FLAGS_OFFSET 0x5a /* established: the 16-bit flags word */

/* ---- superblock (sector 0 of the data device and the start of every allocation group) ---- */
#define XFS_SB_SIZE 0xd0 /* established */
#define XFS_SB_OFF_UUID 32 /* established: opaque bytes */
#define XFS_SB_LEN_UUID 16 /* established */
#define XFS_SB_OFF_FNAME 108 /* established: volume name, first half */
#define XFS_SB_LEN_FNAME 6 /* established */
#define XFS_SB_OFF_FPACK 114 /* established: volume name, second half; fname and fpack form one 12-byte name */
#define XFS_SB_LEN_FPACK 6 /* established */

/* SB_W0: the 32-bit big-endian word at byte 0 */
/* magicnum: bytes 0 to 3 */
/* FIELD: SB_W0.magicnum 31:0 established */
#define XFS_SB_W0_MAGICNUM_SHIFT 0
#define XFS_SB_W0_MAGICNUM_MASK  0xffffffffu

/* SB_W1: the 32-bit big-endian word at byte 4 */
/* blocksize: bytes 4 to 7 */
/* FIELD: SB_W1.blocksize 31:0 established */
#define XFS_SB_W1_BLOCKSIZE_SHIFT 0
#define XFS_SB_W1_BLOCKSIZE_MASK  0xffffffffu

/* SB_W2: the 32-bit big-endian word at byte 8 */
/* dblocks_hi: high word of a 64-bit value, bytes 8 to 11 */
/* FIELD: SB_W2.dblocks_hi 31:0 established */
#define XFS_SB_W2_DBLOCKS_HI_SHIFT 0
#define XFS_SB_W2_DBLOCKS_HI_MASK  0xffffffffu

/* SB_W3: the 32-bit big-endian word at byte 12 */
/* dblocks_lo: low word of the same value, bytes 12 to 15 */
/* FIELD: SB_W3.dblocks_lo 31:0 established */
#define XFS_SB_W3_DBLOCKS_LO_SHIFT 0
#define XFS_SB_W3_DBLOCKS_LO_MASK  0xffffffffu

/* SB_W4: the 32-bit big-endian word at byte 16 */
/* rblocks_hi: high word of a 64-bit value, bytes 16 to 19 */
/* FIELD: SB_W4.rblocks_hi 31:0 established */
#define XFS_SB_W4_RBLOCKS_HI_SHIFT 0
#define XFS_SB_W4_RBLOCKS_HI_MASK  0xffffffffu

/* SB_W5: the 32-bit big-endian word at byte 20 */
/* rblocks_lo: low word of the same value, bytes 20 to 23 */
/* FIELD: SB_W5.rblocks_lo 31:0 established */
#define XFS_SB_W5_RBLOCKS_LO_SHIFT 0
#define XFS_SB_W5_RBLOCKS_LO_MASK  0xffffffffu

/* SB_W6: the 32-bit big-endian word at byte 24 */
/* rextents_hi: high word of a 64-bit value, bytes 24 to 27 */
/* FIELD: SB_W6.rextents_hi 31:0 established */
#define XFS_SB_W6_REXTENTS_HI_SHIFT 0
#define XFS_SB_W6_REXTENTS_HI_MASK  0xffffffffu

/* SB_W7: the 32-bit big-endian word at byte 28 */
/* rextents_lo: low word of the same value, bytes 28 to 31 */
/* FIELD: SB_W7.rextents_lo 31:0 established */
#define XFS_SB_W7_REXTENTS_LO_SHIFT 0
#define XFS_SB_W7_REXTENTS_LO_MASK  0xffffffffu

/* SB_W12: the 32-bit big-endian word at byte 48 */
/* logstart_hi: 0 means an external log; high word of a 64-bit value, bytes 48 to 51 */
/* FIELD: SB_W12.logstart_hi 31:0 established */
#define XFS_SB_W12_LOGSTART_HI_SHIFT 0
#define XFS_SB_W12_LOGSTART_HI_MASK  0xffffffffu

/* SB_W13: the 32-bit big-endian word at byte 52 */
/* logstart_lo: 0 means an external log; low word of the same value, bytes 52 to 55 */
/* FIELD: SB_W13.logstart_lo 31:0 established */
#define XFS_SB_W13_LOGSTART_LO_SHIFT 0
#define XFS_SB_W13_LOGSTART_LO_MASK  0xffffffffu

/* SB_W14: the 32-bit big-endian word at byte 56 */
/* rootino_hi: high word of a 64-bit value, bytes 56 to 59 */
/* FIELD: SB_W14.rootino_hi 31:0 established */
#define XFS_SB_W14_ROOTINO_HI_SHIFT 0
#define XFS_SB_W14_ROOTINO_HI_MASK  0xffffffffu

/* SB_W15: the 32-bit big-endian word at byte 60 */
/* rootino_lo: low word of the same value, bytes 60 to 63 */
/* FIELD: SB_W15.rootino_lo 31:0 established */
#define XFS_SB_W15_ROOTINO_LO_SHIFT 0
#define XFS_SB_W15_ROOTINO_LO_MASK  0xffffffffu

/* SB_W16: the 32-bit big-endian word at byte 64 */
/* rbmino_hi: high word of a 64-bit value, bytes 64 to 67 */
/* FIELD: SB_W16.rbmino_hi 31:0 established */
#define XFS_SB_W16_RBMINO_HI_SHIFT 0
#define XFS_SB_W16_RBMINO_HI_MASK  0xffffffffu

/* SB_W17: the 32-bit big-endian word at byte 68 */
/* rbmino_lo: low word of the same value, bytes 68 to 71 */
/* FIELD: SB_W17.rbmino_lo 31:0 established */
#define XFS_SB_W17_RBMINO_LO_SHIFT 0
#define XFS_SB_W17_RBMINO_LO_MASK  0xffffffffu

/* SB_W18: the 32-bit big-endian word at byte 72 */
/* rsumino_hi: high word of a 64-bit value, bytes 72 to 75 */
/* FIELD: SB_W18.rsumino_hi 31:0 established */
#define XFS_SB_W18_RSUMINO_HI_SHIFT 0
#define XFS_SB_W18_RSUMINO_HI_MASK  0xffffffffu

/* SB_W19: the 32-bit big-endian word at byte 76 */
/* rsumino_lo: low word of the same value, bytes 76 to 79 */
/* FIELD: SB_W19.rsumino_lo 31:0 established */
#define XFS_SB_W19_RSUMINO_LO_SHIFT 0
#define XFS_SB_W19_RSUMINO_LO_MASK  0xffffffffu

/* SB_W20: the 32-bit big-endian word at byte 80 */
/* rextsize: bytes 80 to 83 */
/* FIELD: SB_W20.rextsize 31:0 established */
#define XFS_SB_W20_REXTSIZE_SHIFT 0
#define XFS_SB_W20_REXTSIZE_MASK  0xffffffffu

/* SB_W21: the 32-bit big-endian word at byte 84 */
/* agblocks: bytes 84 to 87 */
/* FIELD: SB_W21.agblocks 31:0 established */
#define XFS_SB_W21_AGBLOCKS_SHIFT 0
#define XFS_SB_W21_AGBLOCKS_MASK  0xffffffffu

/* SB_W22: the 32-bit big-endian word at byte 88 */
/* agcount: bytes 88 to 91 */
/* FIELD: SB_W22.agcount 31:0 established */
#define XFS_SB_W22_AGCOUNT_SHIFT 0
#define XFS_SB_W22_AGCOUNT_MASK  0xffffffffu

/* SB_W23: the 32-bit big-endian word at byte 92 */
/* rbmblocks: bytes 92 to 95 */
/* FIELD: SB_W23.rbmblocks 31:0 established */
#define XFS_SB_W23_RBMBLOCKS_SHIFT 0
#define XFS_SB_W23_RBMBLOCKS_MASK  0xffffffffu

/* SB_W24: the 32-bit big-endian word at byte 96 */
/* logblocks: bytes 96 to 99 */
/* FIELD: SB_W24.logblocks 31:0 established */
#define XFS_SB_W24_LOGBLOCKS_SHIFT 0
#define XFS_SB_W24_LOGBLOCKS_MASK  0xffffffffu

/* SB_W25: the 32-bit big-endian word at byte 100 */
/* versionnum_morebits: bit 0x8000 of the version word: features2 is valid */
/* FIELD: SB_W25.versionnum_morebits 31:31 established */
#define XFS_SB_W25_VERSIONNUM_MOREBITS_SHIFT 31
#define XFS_SB_W25_VERSIONNUM_MOREBITS_MASK  0x1u
/* versionnum_ci_dirs: bit 0x4000 of the version word: case-insensitive version 2 directory mode, needs dirv2 */
/* FIELD: SB_W25.versionnum_ci_dirs 30:30 inferred */
#define XFS_SB_W25_VERSIONNUM_CI_DIRS_SHIFT 30
#define XFS_SB_W25_VERSIONNUM_CI_DIRS_MASK  0x1u
/* versionnum_dirv2: bit 0x2000 of the version word: version 2 directories */
/* FIELD: SB_W25.versionnum_dirv2 29:29 established */
#define XFS_SB_W25_VERSIONNUM_DIRV2_SHIFT 29
#define XFS_SB_W25_VERSIONNUM_DIRV2_MASK  0x1u
/* versionnum_extflg: bit 0x1000 of the version word: unwritten extents allowed */
/* FIELD: SB_W25.versionnum_extflg 28:28 established */
#define XFS_SB_W25_VERSIONNUM_EXTFLG_SHIFT 28
#define XFS_SB_W25_VERSIONNUM_EXTFLG_MASK  0x1u
/* versionnum_rejected: bit 0x0800 of the version word: rejected by the kernel */
/* FIELD: SB_W25.versionnum_rejected 27:27 established */
#define XFS_SB_W25_VERSIONNUM_REJECTED_SHIFT 27
#define XFS_SB_W25_VERSIONNUM_REJECTED_MASK  0x1u
/* versionnum_logv2: bit 0x0400 of the version word: version 2 log */
/* FIELD: SB_W25.versionnum_logv2 26:26 established */
#define XFS_SB_W25_VERSIONNUM_LOGV2_SHIFT 26
#define XFS_SB_W25_VERSIONNUM_LOGV2_MASK  0x1u
/* versionnum_shared: bit 0x0200 of the version word: shared filesystem */
/* FIELD: SB_W25.versionnum_shared 25:25 established */
#define XFS_SB_W25_VERSIONNUM_SHARED_SHIFT 25
#define XFS_SB_W25_VERSIONNUM_SHARED_MASK  0x1u
/* versionnum_dalign: bit 0x0100 of the version word: unit and width are valid stripe values */
/* FIELD: SB_W25.versionnum_dalign 24:24 established */
#define XFS_SB_W25_VERSIONNUM_DALIGN_SHIFT 24
#define XFS_SB_W25_VERSIONNUM_DALIGN_MASK  0x1u
/* versionnum_align: bit 0x0080 of the version word: inode chunks aligned to inoalignmt */
/* FIELD: SB_W25.versionnum_align 23:23 established */
#define XFS_SB_W25_VERSIONNUM_ALIGN_SHIFT 23
#define XFS_SB_W25_VERSIONNUM_ALIGN_MASK  0x1u
/* versionnum_quota: bit 0x0040 of the version word: quota in use */
/* FIELD: SB_W25.versionnum_quota 22:22 established */
#define XFS_SB_W25_VERSIONNUM_QUOTA_SHIFT 22
#define XFS_SB_W25_VERSIONNUM_QUOTA_MASK  0x1u
/* versionnum_nlink: bit 0x0020 of the version word: 32-bit link counts, version 2 inodes */
/* FIELD: SB_W25.versionnum_nlink 21:21 established */
#define XFS_SB_W25_VERSIONNUM_NLINK_SHIFT 21
#define XFS_SB_W25_VERSIONNUM_NLINK_MASK  0x1u
/* versionnum_attr: bit 0x0010 of the version word: attribute forks may exist */
/* FIELD: SB_W25.versionnum_attr 20:20 established */
#define XFS_SB_W25_VERSIONNUM_ATTR_SHIFT 20
#define XFS_SB_W25_VERSIONNUM_ATTR_MASK  0x1u
/* versionnum_vers: low nibble of the 16-bit version word at byte 0x64 */
/* FIELD: SB_W25.versionnum_vers 19:16 established */
#define XFS_SB_W25_VERSIONNUM_VERS_SHIFT 16
#define XFS_SB_W25_VERSIONNUM_VERS_MASK  0xfu
/* sectsize: bytes 0x66 to 0x67, must be above 0 */
/* FIELD: SB_W25.sectsize 15:0 established */
#define XFS_SB_W25_SECTSIZE_SHIFT 0
#define XFS_SB_W25_SECTSIZE_MASK  0xffffu

/* SB_W26: the 32-bit big-endian word at byte 104 */
/* inodesize: 0x100 to 0x800, bytes 0x68 to 0x69 */
/* FIELD: SB_W26.inodesize 31:16 established */
#define XFS_SB_W26_INODESIZE_SHIFT 16
#define XFS_SB_W26_INODESIZE_MASK  0xffffu
/* inopblock: bytes 0x6a to 0x6b */
/* FIELD: SB_W26.inopblock 15:0 established */
#define XFS_SB_W26_INOPBLOCK_SHIFT 0
#define XFS_SB_W26_INOPBLOCK_MASK  0xffffu

/* SB_W30: the 32-bit big-endian word at byte 120 */
/* blocklog: 9 to 16, byte 0x78 */
/* FIELD: SB_W30.blocklog 31:24 established */
#define XFS_SB_W30_BLOCKLOG_SHIFT 24
#define XFS_SB_W30_BLOCKLOG_MASK  0xffu
/* sectlog: byte 0x79 */
/* FIELD: SB_W30.sectlog 23:16 established */
#define XFS_SB_W30_SECTLOG_SHIFT 16
#define XFS_SB_W30_SECTLOG_MASK  0xffu
/* inodelog: byte 0x7a */
/* FIELD: SB_W30.inodelog 15:8 established */
#define XFS_SB_W30_INODELOG_SHIFT 8
#define XFS_SB_W30_INODELOG_MASK  0xffu
/* inopblog: byte 0x7b */
/* FIELD: SB_W30.inopblog 7:0 established */
#define XFS_SB_W30_INOPBLOG_SHIFT 0
#define XFS_SB_W30_INOPBLOG_MASK  0xffu

/* SB_W31: the 32-bit big-endian word at byte 124 */
/* agblklog: byte 0x7c */
/* FIELD: SB_W31.agblklog 31:24 established */
#define XFS_SB_W31_AGBLKLOG_SHIFT 24
#define XFS_SB_W31_AGBLKLOG_MASK  0xffu
/* rextslog: byte 0x7d */
/* FIELD: SB_W31.rextslog 23:16 established */
#define XFS_SB_W31_REXTSLOG_SHIFT 16
#define XFS_SB_W31_REXTSLOG_MASK  0xffu
/* inprogress: nonzero means mkfs or grow was interrupted, byte 0x7e */
/* FIELD: SB_W31.inprogress 15:8 established */
#define XFS_SB_W31_INPROGRESS_SHIFT 8
#define XFS_SB_W31_INPROGRESS_MASK  0xffu
/* imax_pct: below 101, byte 0x7f */
/* FIELD: SB_W31.imax_pct 7:0 established */
#define XFS_SB_W31_IMAX_PCT_SHIFT 0
#define XFS_SB_W31_IMAX_PCT_MASK  0xffu

/* SB_W32: the 32-bit big-endian word at byte 128 */
/* icount_hi: high word of a 64-bit value, bytes 128 to 131 */
/* FIELD: SB_W32.icount_hi 31:0 established */
#define XFS_SB_W32_ICOUNT_HI_SHIFT 0
#define XFS_SB_W32_ICOUNT_HI_MASK  0xffffffffu

/* SB_W33: the 32-bit big-endian word at byte 132 */
/* icount_lo: low word of the same value, bytes 132 to 135 */
/* FIELD: SB_W33.icount_lo 31:0 established */
#define XFS_SB_W33_ICOUNT_LO_SHIFT 0
#define XFS_SB_W33_ICOUNT_LO_MASK  0xffffffffu

/* SB_W34: the 32-bit big-endian word at byte 136 */
/* ifree_hi: high word of a 64-bit value, bytes 136 to 139 */
/* FIELD: SB_W34.ifree_hi 31:0 established */
#define XFS_SB_W34_IFREE_HI_SHIFT 0
#define XFS_SB_W34_IFREE_HI_MASK  0xffffffffu

/* SB_W35: the 32-bit big-endian word at byte 140 */
/* ifree_lo: low word of the same value, bytes 140 to 143 */
/* FIELD: SB_W35.ifree_lo 31:0 established */
#define XFS_SB_W35_IFREE_LO_SHIFT 0
#define XFS_SB_W35_IFREE_LO_MASK  0xffffffffu

/* SB_W36: the 32-bit big-endian word at byte 144 */
/* fdblocks_hi: high word of a 64-bit value, bytes 144 to 147 */
/* FIELD: SB_W36.fdblocks_hi 31:0 established */
#define XFS_SB_W36_FDBLOCKS_HI_SHIFT 0
#define XFS_SB_W36_FDBLOCKS_HI_MASK  0xffffffffu

/* SB_W37: the 32-bit big-endian word at byte 148 */
/* fdblocks_lo: low word of the same value, bytes 148 to 151 */
/* FIELD: SB_W37.fdblocks_lo 31:0 established */
#define XFS_SB_W37_FDBLOCKS_LO_SHIFT 0
#define XFS_SB_W37_FDBLOCKS_LO_MASK  0xffffffffu

/* SB_W38: the 32-bit big-endian word at byte 152 */
/* frextents_hi: high word of a 64-bit value, bytes 152 to 155 */
/* FIELD: SB_W38.frextents_hi 31:0 established */
#define XFS_SB_W38_FREXTENTS_HI_SHIFT 0
#define XFS_SB_W38_FREXTENTS_HI_MASK  0xffffffffu

/* SB_W39: the 32-bit big-endian word at byte 156 */
/* frextents_lo: low word of the same value, bytes 156 to 159 */
/* FIELD: SB_W39.frextents_lo 31:0 established */
#define XFS_SB_W39_FREXTENTS_LO_SHIFT 0
#define XFS_SB_W39_FREXTENTS_LO_MASK  0xffffffffu

/* SB_W40: the 32-bit big-endian word at byte 160 */
/* uquotino_hi: high word of a 64-bit value, bytes 160 to 163 */
/* FIELD: SB_W40.uquotino_hi 31:0 established */
#define XFS_SB_W40_UQUOTINO_HI_SHIFT 0
#define XFS_SB_W40_UQUOTINO_HI_MASK  0xffffffffu

/* SB_W41: the 32-bit big-endian word at byte 164 */
/* uquotino_lo: low word of the same value, bytes 164 to 167 */
/* FIELD: SB_W41.uquotino_lo 31:0 established */
#define XFS_SB_W41_UQUOTINO_LO_SHIFT 0
#define XFS_SB_W41_UQUOTINO_LO_MASK  0xffffffffu

/* SB_W42: the 32-bit big-endian word at byte 168 */
/* pquotino_hi: project quota inode in the shipped tools, group quota inode in Linux; which one a given filesystem stores is unknown; high word of a 64-bit value, bytes 168 to 171 */
/* FIELD: SB_W42.pquotino_hi 31:0 established */
#define XFS_SB_W42_PQUOTINO_HI_SHIFT 0
#define XFS_SB_W42_PQUOTINO_HI_MASK  0xffffffffu

/* SB_W43: the 32-bit big-endian word at byte 172 */
/* pquotino_lo: project quota inode in the shipped tools, group quota inode in Linux; which one a given filesystem stores is unknown; low word of the same value, bytes 172 to 175 */
/* FIELD: SB_W43.pquotino_lo 31:0 established */
#define XFS_SB_W43_PQUOTINO_LO_SHIFT 0
#define XFS_SB_W43_PQUOTINO_LO_MASK  0xffffffffu

/* SB_W44: the 32-bit big-endian word at byte 176 */
/* qflags: bit values not enumerated, bytes 0xb0 to 0xb1 */
/* FIELD: SB_W44.qflags 31:16 established */
#define XFS_SB_W44_QFLAGS_SHIFT 16
#define XFS_SB_W44_QFLAGS_MASK  0xffffu
/* flags_shared_ro: bit 0 of the flags byte (byte 0xb2): set by unmount of a shared filesystem marked read-only; other bits unknown */
/* FIELD: SB_W44.flags_shared_ro 8:8 established */
#define XFS_SB_W44_FLAGS_SHARED_RO_SHIFT 8
#define XFS_SB_W44_FLAGS_SHARED_RO_MASK  0x1u
/* shared_vn: must be 0 when morebits is set; meaning unknown, byte 0xb3 */
/* FIELD: SB_W44.shared_vn 7:0 established */
#define XFS_SB_W44_SHARED_VN_SHIFT 0
#define XFS_SB_W44_SHARED_VN_MASK  0xffu

/* SB_W45: the 32-bit big-endian word at byte 180 */
/* inoalignmt: bytes 180 to 183 */
/* FIELD: SB_W45.inoalignmt 31:0 established */
#define XFS_SB_W45_INOALIGNMT_SHIFT 0
#define XFS_SB_W45_INOALIGNMT_MASK  0xffffffffu

/* SB_W46: the 32-bit big-endian word at byte 184 */
/* unit: bytes 184 to 187 */
/* FIELD: SB_W46.unit 31:0 established */
#define XFS_SB_W46_UNIT_SHIFT 0
#define XFS_SB_W46_UNIT_MASK  0xffffffffu

/* SB_W47: the 32-bit big-endian word at byte 188 */
/* width: bytes 188 to 191 */
/* FIELD: SB_W47.width 31:0 established */
#define XFS_SB_W47_WIDTH_SHIFT 0
#define XFS_SB_W47_WIDTH_MASK  0xffffffffu

/* SB_W48: the 32-bit big-endian word at byte 192 */
/* dirblklog: version 2 directory block is 1 shifted left by blocklog plus dirblklog bytes, byte 0xc0 */
/* FIELD: SB_W48.dirblklog 31:24 established */
#define XFS_SB_W48_DIRBLKLOG_SHIFT 24
#define XFS_SB_W48_DIRBLKLOG_MASK  0xffu
/* logsectlog: byte 0xc1 */
/* FIELD: SB_W48.logsectlog 23:16 established */
#define XFS_SB_W48_LOGSECTLOG_SHIFT 16
#define XFS_SB_W48_LOGSECTLOG_MASK  0xffu
/* logsectsize: bytes 0xc2 to 0xc3 */
/* FIELD: SB_W48.logsectsize 15:0 established */
#define XFS_SB_W48_LOGSECTSIZE_SHIFT 0
#define XFS_SB_W48_LOGSECTSIZE_MASK  0xffffu

/* SB_W49: the 32-bit big-endian word at byte 196 */
/* logsunit: bytes 196 to 199 */
/* FIELD: SB_W49.logsunit 31:0 established */
#define XFS_SB_W49_LOGSUNIT_SHIFT 0
#define XFS_SB_W49_LOGSUNIT_MASK  0xffffffffu

/* SB_W50: the 32-bit big-endian word at byte 200 */
/* features2_high: bits 31:5 of features2, rejected by validation */
/* FIELD: SB_W50.features2_high 31:5 unknown */
#define XFS_SB_W50_FEATURES2_HIGH_SHIFT 5
#define XFS_SB_W50_FEATURES2_HIGH_MASK  0x7ffffffu
/* features2_parent: parent pointer attributes */
/* FIELD: SB_W50.features2_parent 4:4 established */
#define XFS_SB_W50_FEATURES2_PARENT_SHIFT 4
#define XFS_SB_W50_FEATURES2_PARENT_MASK  0x1u
/* features2_attr2: version 2 inline attributes, the attribute fork offset may move */
/* FIELD: SB_W50.features2_attr2 3:3 established */
#define XFS_SB_W50_FEATURES2_ATTR2_SHIFT 3
#define XFS_SB_W50_FEATURES2_ATTR2_MASK  0x1u
/* features2_bit2: bit 2 of features2, rejected by validation */
/* FIELD: SB_W50.features2_bit2 2:2 unknown */
#define XFS_SB_W50_FEATURES2_BIT2_SHIFT 2
#define XFS_SB_W50_FEATURES2_BIT2_MASK  0x1u
/* features2_lazysbcount: superblock counters are updated lazily */
/* FIELD: SB_W50.features2_lazysbcount 1:1 established */
#define XFS_SB_W50_FEATURES2_LAZYSBCOUNT_SHIFT 1
#define XFS_SB_W50_FEATURES2_LAZYSBCOUNT_MASK  0x1u
/* features2_bit0: bit 0 of features2, accepted by validation, use not found */
/* FIELD: SB_W50.features2_bit0 0:0 unknown */
#define XFS_SB_W50_FEATURES2_BIT0_SHIFT 0
#define XFS_SB_W50_FEATURES2_BIT0_MASK  0x1u

/* ---- inode core (0x64 bytes, then the data fork) ---- */
#define XFS_INO_SIZE 0x64 /* established */

/* INO_W0: the 32-bit big-endian word at byte 0 */
/* magic: bytes 0 to 1 */
/* FIELD: INO_W0.magic 31:16 established */
#define XFS_INO_W0_MAGIC_SHIFT 16
#define XFS_INO_W0_MAGIC_MASK  0xffffu
/* mode: 0 means an unused inode, bytes 2 to 3 */
/* FIELD: INO_W0.mode 15:0 established */
#define XFS_INO_W0_MODE_SHIFT 0
#define XFS_INO_W0_MODE_MASK  0xffffu

/* INO_W1: the 32-bit big-endian word at byte 4 */
/* version: inode version 1 or 2, byte 4 */
/* FIELD: INO_W1.version 31:24 established */
#define XFS_INO_W1_VERSION_SHIFT 24
#define XFS_INO_W1_VERSION_MASK  0xffu
/* format: data fork format, byte 5; the kernel read path rejects uuid */
/* FIELD: INO_W1.format 23:16 established */
#define XFS_INO_W1_FORMAT_SHIFT 16
#define XFS_INO_W1_FORMAT_MASK  0xffu
/* ENUM: INO_W1.format 0 dev established */
#define XFS_INO_W1_FORMAT_DEV 0
/* ENUM: INO_W1.format 1 local established */
#define XFS_INO_W1_FORMAT_LOCAL 1
/* ENUM: INO_W1.format 2 extents established */
#define XFS_INO_W1_FORMAT_EXTENTS 2
/* ENUM: INO_W1.format 3 btree established */
#define XFS_INO_W1_FORMAT_BTREE 3
/* ENUM: INO_W1.format 4 uuid established */
#define XFS_INO_W1_FORMAT_UUID 4
/* onlink: 16-bit link count of version 1, zero in version 2, bytes 6 to 7 */
/* FIELD: INO_W1.onlink 15:0 established */
#define XFS_INO_W1_ONLINK_SHIFT 0
#define XFS_INO_W1_ONLINK_MASK  0xffffu

/* INO_W2: the 32-bit big-endian word at byte 8 */
/* uid: bytes 8 to 11 */
/* FIELD: INO_W2.uid 31:0 established */
#define XFS_INO_W2_UID_SHIFT 0
#define XFS_INO_W2_UID_MASK  0xffffffffu

/* INO_W3: the 32-bit big-endian word at byte 12 */
/* gid: bytes 12 to 15 */
/* FIELD: INO_W3.gid 31:0 established */
#define XFS_INO_W3_GID_SHIFT 0
#define XFS_INO_W3_GID_MASK  0xffffffffu

/* INO_W4: the 32-bit big-endian word at byte 16 */
/* nlinkv2: 32-bit link count of version 2, ignored in version 1 */
/* FIELD: INO_W4.nlinkv2 31:0 established */
#define XFS_INO_W4_NLINKV2_SHIFT 0
#define XFS_INO_W4_NLINKV2_MASK  0xffffffffu

/* INO_W5: the 32-bit big-endian word at byte 20 */
/* projid: version 2 only, bytes 0x14 to 0x15 */
/* FIELD: INO_W5.projid 31:16 established */
#define XFS_INO_W5_PROJID_SHIFT 16
#define XFS_INO_W5_PROJID_MASK  0xffffu

/* INO_W7: the 32-bit big-endian word at byte 28 */
/* flushiter: incremented on every flush, wraps at the 16-bit maximum, bytes 0x1e to 0x1f */
/* FIELD: INO_W7.flushiter 15:0 established */
#define XFS_INO_W7_FLUSHITER_SHIFT 0
#define XFS_INO_W7_FLUSHITER_MASK  0xffffu

/* INO_W8: the 32-bit big-endian word at byte 32 */
/* atime_sec: bytes 32 to 35 */
/* FIELD: INO_W8.atime_sec 31:0 established */
#define XFS_INO_W8_ATIME_SEC_SHIFT 0
#define XFS_INO_W8_ATIME_SEC_MASK  0xffffffffu

/* INO_W9: the 32-bit big-endian word at byte 36 */
/* atime_nsec: bytes 36 to 39 */
/* FIELD: INO_W9.atime_nsec 31:0 established */
#define XFS_INO_W9_ATIME_NSEC_SHIFT 0
#define XFS_INO_W9_ATIME_NSEC_MASK  0xffffffffu

/* INO_W10: the 32-bit big-endian word at byte 40 */
/* mtime_sec: bytes 40 to 43 */
/* FIELD: INO_W10.mtime_sec 31:0 established */
#define XFS_INO_W10_MTIME_SEC_SHIFT 0
#define XFS_INO_W10_MTIME_SEC_MASK  0xffffffffu

/* INO_W11: the 32-bit big-endian word at byte 44 */
/* mtime_nsec: bytes 44 to 47 */
/* FIELD: INO_W11.mtime_nsec 31:0 established */
#define XFS_INO_W11_MTIME_NSEC_SHIFT 0
#define XFS_INO_W11_MTIME_NSEC_MASK  0xffffffffu

/* INO_W12: the 32-bit big-endian word at byte 48 */
/* ctime_sec: bytes 48 to 51 */
/* FIELD: INO_W12.ctime_sec 31:0 established */
#define XFS_INO_W12_CTIME_SEC_SHIFT 0
#define XFS_INO_W12_CTIME_SEC_MASK  0xffffffffu

/* INO_W13: the 32-bit big-endian word at byte 52 */
/* ctime_nsec: bytes 52 to 55 */
/* FIELD: INO_W13.ctime_nsec 31:0 established */
#define XFS_INO_W13_CTIME_NSEC_SHIFT 0
#define XFS_INO_W13_CTIME_NSEC_MASK  0xffffffffu

/* INO_W14: the 32-bit big-endian word at byte 56 */
/* size_hi: high word of a 64-bit value, bytes 56 to 59 */
/* FIELD: INO_W14.size_hi 31:0 established */
#define XFS_INO_W14_SIZE_HI_SHIFT 0
#define XFS_INO_W14_SIZE_HI_MASK  0xffffffffu

/* INO_W15: the 32-bit big-endian word at byte 60 */
/* size_lo: low word of the same value, bytes 60 to 63 */
/* FIELD: INO_W15.size_lo 31:0 established */
#define XFS_INO_W15_SIZE_LO_SHIFT 0
#define XFS_INO_W15_SIZE_LO_MASK  0xffffffffu

/* INO_W16: the 32-bit big-endian word at byte 64 */
/* nblocks_hi: high word of a 64-bit value, bytes 64 to 67 */
/* FIELD: INO_W16.nblocks_hi 31:0 established */
#define XFS_INO_W16_NBLOCKS_HI_SHIFT 0
#define XFS_INO_W16_NBLOCKS_HI_MASK  0xffffffffu

/* INO_W17: the 32-bit big-endian word at byte 68 */
/* nblocks_lo: low word of the same value, bytes 68 to 71 */
/* FIELD: INO_W17.nblocks_lo 31:0 established */
#define XFS_INO_W17_NBLOCKS_LO_SHIFT 0
#define XFS_INO_W17_NBLOCKS_LO_MASK  0xffffffffu

/* INO_W18: the 32-bit big-endian word at byte 72 */
/* extsize: bytes 72 to 75 */
/* FIELD: INO_W18.extsize 31:0 established */
#define XFS_INO_W18_EXTSIZE_SHIFT 0
#define XFS_INO_W18_EXTSIZE_MASK  0xffffffffu

/* INO_W19: the 32-bit big-endian word at byte 76 */
/* nextents: bytes 76 to 79 */
/* FIELD: INO_W19.nextents 31:0 established */
#define XFS_INO_W19_NEXTENTS_SHIFT 0
#define XFS_INO_W19_NEXTENTS_MASK  0xffffffffu

/* INO_W20: the 32-bit big-endian word at byte 80 */
/* naextents: bytes 0x50 to 0x51 */
/* FIELD: INO_W20.naextents 31:16 established */
#define XFS_INO_W20_NAEXTENTS_SHIFT 16
#define XFS_INO_W20_NAEXTENTS_MASK  0xffffu
/* forkoff: attribute fork starts at 0x64 plus 8 times this; 0 means none, byte 0x52 */
/* FIELD: INO_W20.forkoff 15:8 established */
#define XFS_INO_W20_FORKOFF_SHIFT 8
#define XFS_INO_W20_FORKOFF_MASK  0xffu
/* aformat: byte 0x53 */
/* FIELD: INO_W20.aformat 7:0 established */
#define XFS_INO_W20_AFORMAT_SHIFT 0
#define XFS_INO_W20_AFORMAT_MASK  0xffu
/* ENUM: INO_W20.aformat 1 local established */
#define XFS_INO_W20_AFORMAT_LOCAL 1
/* ENUM: INO_W20.aformat 2 extents established */
#define XFS_INO_W20_AFORMAT_EXTENTS 2
/* ENUM: INO_W20.aformat 3 btree established */
#define XFS_INO_W20_AFORMAT_BTREE 3

/* INO_W21: the 32-bit big-endian word at byte 84 */
/* dmevmask: bytes 84 to 87 */
/* FIELD: INO_W21.dmevmask 31:0 established */
#define XFS_INO_W21_DMEVMASK_SHIFT 0
#define XFS_INO_W21_DMEVMASK_MASK  0xffffffffu

/* INO_W22: the 32-bit big-endian word at byte 88 */
/* dmstate: bytes 0x58 to 0x59 */
/* FIELD: INO_W22.dmstate 31:16 established */
#define XFS_INO_W22_DMSTATE_SHIFT 16
#define XFS_INO_W22_DMSTATE_MASK  0xffffu
/* flags_rtinherit: bit 0x0100 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_rtinherit 8:8 established */
#define XFS_INO_W22_FLAGS_RTINHERIT_SHIFT 8
#define XFS_INO_W22_FLAGS_RTINHERIT_MASK  0x1u
/* flags_nodump: bit 0x0080 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_nodump 7:7 established */
#define XFS_INO_W22_FLAGS_NODUMP_SHIFT 7
#define XFS_INO_W22_FLAGS_NODUMP_MASK  0x1u
/* flags_noatime: bit 0x0040 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_noatime 6:6 established */
#define XFS_INO_W22_FLAGS_NOATIME_SHIFT 6
#define XFS_INO_W22_FLAGS_NOATIME_MASK  0x1u
/* flags_sync: bit 0x0020 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_sync 5:5 established */
#define XFS_INO_W22_FLAGS_SYNC_SHIFT 5
#define XFS_INO_W22_FLAGS_SYNC_MASK  0x1u
/* flags_append: bit 0x0010 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_append 4:4 established */
#define XFS_INO_W22_FLAGS_APPEND_SHIFT 4
#define XFS_INO_W22_FLAGS_APPEND_MASK  0x1u
/* flags_immutable: bit 0x0008 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_immutable 3:3 established */
#define XFS_INO_W22_FLAGS_IMMUTABLE_SHIFT 3
#define XFS_INO_W22_FLAGS_IMMUTABLE_MASK  0x1u
/* flags_newrtbm: bit 0x0004 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_newrtbm 2:2 established */
#define XFS_INO_W22_FLAGS_NEWRTBM_SHIFT 2
#define XFS_INO_W22_FLAGS_NEWRTBM_MASK  0x1u
/* flags_prealloc: bit 0x0002 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_prealloc 1:1 established */
#define XFS_INO_W22_FLAGS_PREALLOC_SHIFT 1
#define XFS_INO_W22_FLAGS_PREALLOC_MASK  0x1u
/* flags_realtime: bit 0x0001 of the 16-bit flags word at byte 0x5a */
/* FIELD: INO_W22.flags_realtime 0:0 established */
#define XFS_INO_W22_FLAGS_REALTIME_SHIFT 0
#define XFS_INO_W22_FLAGS_REALTIME_MASK  0x1u

/* INO_W23: the 32-bit big-endian word at byte 92 */
/* gen: bytes 92 to 95 */
/* FIELD: INO_W23.gen 31:0 established */
#define XFS_INO_W23_GEN_SHIFT 0
#define XFS_INO_W23_GEN_MASK  0xffffffffu

/* INO_W24: the 32-bit big-endian word at byte 96 */
/* next_unlinked: bytes 96 to 99 */
/* FIELD: INO_W24.next_unlinked 31:0 established */
#define XFS_INO_W24_NEXT_UNLINKED_SHIFT 0
#define XFS_INO_W24_NEXT_UNLINKED_MASK  0xffffffffu

/*
 * Vectors. TESTVEC values are ones the notes record; EXAMPLE lines only show a layout.
 * EXAMPLE (unverified): 0x00040000 as SB_W25 => versionnum_vers=4, versionnum_attr=0, versionnum_nlink=0
 */
/*
 * TESTVEC: 0x0000001b as SB_W50 => features2_bit0=1, features2_lazysbcount=1, features2_attr2=1, features2_parent=1
 */
/*
 * EXAMPLE (unverified): 0x494e41ed as INO_W0 => magic=0x494e, mode=0x41ed
 */
/*
 * EXAMPLE (unverified): 0x02010000 as INO_W1 => version=2, format=1
 */
#endif
