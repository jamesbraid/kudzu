/*
 * EFS_DECODE.h - Extent File System on-disk structures
 *
 * Publish-safe contract for the Extent File System: the superblock, the inode,
 * the 8-byte extent and the directory block. Every multi-byte integer on disk is
 * big-endian. A block number ("bb") counts 512-byte blocks from the start of the
 * filesystem (the start of its volume-header partition, or of a bare image),
 * never from the start of the disk.
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
 * Derivation. Layouts come from reading the shipped kernel filesystem module and
 * the format and check tools, and were compared field by field with real
 * filesystem images from distribution media (all written with the older magic and
 * clean). The Linux EFS driver names most of the same fields. Cases no image
 * exercises, such as the newer magic, a secondary superblock, device nodes and
 * inline data, rest on the code alone.
 *
 * The superblock checksum: a 32-bit accumulator, initially zero, run over the 44
 * big-endian 16-bit words of bytes 0 to 0x57; for each word, exclusive-or the
 * word in, then rotate the accumulator left by one bit. The result is stored at
 * byte 0x58. The kernel also accepts a variant without the carried bit.
 *
 * Inode location: group g = ino divided by (cgisize times 4); block bb =
 * firstcg + g times cgfsize + (ino divided by 4) modulo cgisize; byte offset in
 * the block = (ino modulo 4) times 128. Inodes 0 and 1 are never allocated.
 *
 * Directory entry, at byte 2 times a slot byte within the block: a 32-bit inode
 * number (it may sit on a 2-byte boundary), a one-byte name length from 1 to
 * 255, then the name bytes padded so that 5 plus the length is even. The slot
 * bytes sit at byte 4 and up and grow upward, the entries grow downward from
 * byte 512.
 */
#ifndef EFS_DECODE_H
#define EFS_DECODE_H

#include <stdint.h>

#define EFS_BBSIZE 512 /* established: block size in bytes */
#define EFS_SUPER_BB 1 /* established: block 0 is an unused boot block */
#define EFS_BITMAP_BB 2 /* established: bitmap start when the superblock bitmap block is 0 */
#define EFS_MAGIC_OLD 0x00072959u /* established: carried by every held image */
#define EFS_MAGIC_NEW 0x0007295au /* established: bitmap and size placed past the last group */
#define EFS_ROOT_INO 2 /* established: inodes 0 and 1 are never allocated */
#define EFS_INODE_SIZE 128 /* established */
#define EFS_INODES_PER_BB 4 /* established */
#define EFS_DIRECTEXTENTS 12 /* established: extents that fit in the inode data area */
#define EFS_EX_MAXLEN 248 /* established: longest run in blocks */
#define EFS_INODE_DATA_BYTES 96 /* established: inode data area, from byte 0x20 to 0x7f */
#define EFS_DEV_LONG_FORM 0xffff /* established: short device value that selects the long form */
#define EFS_DIRBLK_MAGIC 0xbeef /* established */
#define EFS_DIRBLK_HEADERSIZE 4 /* established */
#define EFS_DIRBLK_MAXSLOTS 0x48 /* established */
#define EFS_DENT_HEADER_BYTES 5 /* established: inode number plus name length byte */

/* ---- superblock (block 1 of the filesystem, 0x5c meaningful bytes) ---- */
#define EFS_SB_SIZE 0x5c /* established */
#define EFS_SB_OFF_FNAME 32 /* established: filesystem name, not NUL terminated */
#define EFS_SB_LEN_FNAME 6 /* established */
#define EFS_SB_OFF_FPACK 38 /* established: pack name, not NUL terminated */
#define EFS_SB_LEN_FPACK 6 /* established */
#define EFS_SB_OFF_SPARE 68 /* inferred: zero in every image */
#define EFS_SB_LEN_SPARE 20 /* inferred */

/* SB_W0: the 32-bit big-endian word at byte 0 */
/* size: bytes 0 to 3 */
/* FIELD: SB_W0.size 31:0 established */
#define EFS_SB_W0_SIZE_SHIFT 0
#define EFS_SB_W0_SIZE_MASK  0xffffffffu

/* SB_W1: the 32-bit big-endian word at byte 4 */
/* firstcg: bytes 4 to 7 */
/* FIELD: SB_W1.firstcg 31:0 established */
#define EFS_SB_W1_FIRSTCG_SHIFT 0
#define EFS_SB_W1_FIRSTCG_MASK  0xffffffffu

/* SB_W2: the 32-bit big-endian word at byte 8 */
/* cgfsize: bytes 8 to 11 */
/* FIELD: SB_W2.cgfsize 31:0 established */
#define EFS_SB_W2_CGFSIZE_SHIFT 0
#define EFS_SB_W2_CGFSIZE_MASK  0xffffffffu

/* SB_W3: the 32-bit big-endian word at byte 12 */
/* cgisize: bytes 12 to 13 */
/* FIELD: SB_W3.cgisize 31:16 established */
#define EFS_SB_W3_CGISIZE_SHIFT 16
#define EFS_SB_W3_CGISIZE_MASK  0xffffu
/* sectors: geometry hint, must be nonzero, bytes 0x0e to 0x0f */
/* FIELD: SB_W3.sectors 15:0 established */
#define EFS_SB_W3_SECTORS_SHIFT 0
#define EFS_SB_W3_SECTORS_MASK  0xffffu

/* SB_W4: the 32-bit big-endian word at byte 16 */
/* heads: geometry hint, bytes 0x10 to 0x11 */
/* FIELD: SB_W4.heads 31:16 established */
#define EFS_SB_W4_HEADS_SHIFT 16
#define EFS_SB_W4_HEADS_MASK  0xffffu
/* ncg: bytes 18 to 19 */
/* FIELD: SB_W4.ncg 15:0 established */
#define EFS_SB_W4_NCG_SHIFT 0
#define EFS_SB_W4_NCG_MASK  0xffffu

/* SB_W5: the 32-bit big-endian word at byte 20 */
/* dirty: bytes 20 to 21 */
/* FIELD: SB_W5.dirty 31:16 established */
#define EFS_SB_W5_DIRTY_SHIFT 16
#define EFS_SB_W5_DIRTY_MASK  0xffffu
/* ENUM: SB_W5.dirty 0 clean established */
#define EFS_SB_W5_DIRTY_CLEAN 0
/* ENUM: SB_W5.dirty 30583 mounted established */
#define EFS_SB_W5_DIRTY_MOUNTED 30583
/* ENUM: SB_W5.dirty 2989 dirty established */
#define EFS_SB_W5_DIRTY_DIRTY 2989
/* ENUM: SB_W5.dirty 4660 needsck established */
#define EFS_SB_W5_DIRTY_NEEDSCK 4660

/* SB_W6: the 32-bit big-endian word at byte 24 */
/* time: seconds since 1970, signedness unknown, bytes 0x18 to 0x1b */
/* FIELD: SB_W6.time 31:0 established */
#define EFS_SB_W6_TIME_SHIFT 0
#define EFS_SB_W6_TIME_MASK  0xffffffffu

/* SB_W7: the 32-bit big-endian word at byte 28 */
/* magic: bytes 28 to 31 */
/* FIELD: SB_W7.magic 31:0 established */
#define EFS_SB_W7_MAGIC_SHIFT 0
#define EFS_SB_W7_MAGIC_MASK  0xffffffffu
/* ENUM: SB_W7.magic 0x00072959 old established */
#define EFS_SB_W7_MAGIC_OLD 0x00072959
/* ENUM: SB_W7.magic 0x0007295a new established */
#define EFS_SB_W7_MAGIC_NEW 0x0007295a

/* SB_W11: the 32-bit big-endian word at byte 44 */
/* bmsize: bitmap bytes, bytes 0x2c to 0x2f */
/* FIELD: SB_W11.bmsize 31:0 established */
#define EFS_SB_W11_BMSIZE_SHIFT 0
#define EFS_SB_W11_BMSIZE_MASK  0xffffffffu

/* SB_W12: the 32-bit big-endian word at byte 48 */
/* tfree: free data blocks */
/* FIELD: SB_W12.tfree 31:0 established */
#define EFS_SB_W12_TFREE_SHIFT 0
#define EFS_SB_W12_TFREE_MASK  0xffffffffu

/* SB_W13: the 32-bit big-endian word at byte 52 */
/* tinode: free inodes */
/* FIELD: SB_W13.tinode 31:0 established */
#define EFS_SB_W13_TINODE_SHIFT 0
#define EFS_SB_W13_TINODE_MASK  0xffffffffu

/* SB_W14: the 32-bit big-endian word at byte 56 */
/* bmblock: first bitmap block, 0 means block 2 */
/* FIELD: SB_W14.bmblock 31:0 established */
#define EFS_SB_W14_BMBLOCK_SHIFT 0
#define EFS_SB_W14_BMBLOCK_MASK  0xffffffffu

/* SB_W15: the 32-bit big-endian word at byte 60 */
/* replsb: secondary superblock block, 0 means none */
/* FIELD: SB_W15.replsb 31:0 established */
#define EFS_SB_W15_REPLSB_SHIFT 0
#define EFS_SB_W15_REPLSB_MASK  0xffffffffu

/* SB_W16: the 32-bit big-endian word at byte 64 */
/* lastialloc: allocation hint written by the formatter, use unknown */
/* FIELD: SB_W16.lastialloc 31:0 inferred */
#define EFS_SB_W16_LASTIALLOC_SHIFT 0
#define EFS_SB_W16_LASTIALLOC_MASK  0xffffffffu

/* SB_W22: the 32-bit big-endian word at byte 88 */
/* checksum: bytes 88 to 91 */
/* FIELD: SB_W22.checksum 31:0 established */
#define EFS_SB_W22_CHECKSUM_SHIFT 0
#define EFS_SB_W22_CHECKSUM_MASK  0xffffffffu

/* ---- extent (8 bytes, two words) ---- */
#define EFS_EXT_SIZE 8 /* established */

/* EXT_W0: the 32-bit big-endian word at byte 0 */
/* magic: bits 31:24 of word 0, always 0 in every image */
/* FIELD: EXT_W0.magic 31:24 established */
#define EFS_EXT_W0_MAGIC_SHIFT 24
#define EFS_EXT_W0_MAGIC_MASK  0xffu
/* bn: bits 23:0 of word 0, first block of the run */
/* FIELD: EXT_W0.bn 23:0 established */
#define EFS_EXT_W0_BN_SHIFT 0
#define EFS_EXT_W0_BN_MASK  0xffffffu

/* EXT_W1: the 32-bit big-endian word at byte 4 */
/* length: bits 31:24 of word 1, run length in blocks, 1 to 248 */
/* FIELD: EXT_W1.length 31:24 established */
#define EFS_EXT_W1_LENGTH_SHIFT 24
#define EFS_EXT_W1_LENGTH_MASK  0xffu
/* offset: bits 23:0 of word 1, file block index of the first block */
/* FIELD: EXT_W1.offset 23:0 established */
#define EFS_EXT_W1_OFFSET_SHIFT 0
#define EFS_EXT_W1_OFFSET_MASK  0xffffffu

/* ---- inode (128 bytes) ---- */
#define EFS_DI_SIZE 128 /* established */

/* DI_W0: the 32-bit big-endian word at byte 0 */
/* mode_type: file type, the top nibble of the 16-bit mode; 0x3000 and 0x7000 are handled like a symbolic link and have no recovered name */
/* FIELD: DI_W0.mode_type 31:28 established */
#define EFS_DI_W0_MODE_TYPE_SHIFT 28
#define EFS_DI_W0_MODE_TYPE_MASK  0xfu
/* ENUM: DI_W0.mode_type 1 fifo established */
#define EFS_DI_W0_MODE_TYPE_FIFO 1
/* ENUM: DI_W0.mode_type 2 chr established */
#define EFS_DI_W0_MODE_TYPE_CHR 2
/* ENUM: DI_W0.mode_type 3 x3 established */
#define EFS_DI_W0_MODE_TYPE_X3 3
/* ENUM: DI_W0.mode_type 4 dir established */
#define EFS_DI_W0_MODE_TYPE_DIR 4
/* ENUM: DI_W0.mode_type 6 blk established */
#define EFS_DI_W0_MODE_TYPE_BLK 6
/* ENUM: DI_W0.mode_type 7 x7 established */
#define EFS_DI_W0_MODE_TYPE_X7 7
/* ENUM: DI_W0.mode_type 8 reg established */
#define EFS_DI_W0_MODE_TYPE_REG 8
/* ENUM: DI_W0.mode_type 10 lnk established */
#define EFS_DI_W0_MODE_TYPE_LNK 10
/* ENUM: DI_W0.mode_type 12 sock established */
#define EFS_DI_W0_MODE_TYPE_SOCK 12
/* mode_perm: permission and special bits, the low 12 bits of the mode; a mode of 0 is a free inode */
/* FIELD: DI_W0.mode_perm 27:16 established */
#define EFS_DI_W0_MODE_PERM_SHIFT 16
#define EFS_DI_W0_MODE_PERM_MASK  0xfffu
/* nlink: signed 16-bit link count, bytes 2 to 3 */
/* FIELD: DI_W0.nlink 15:0 established */
#define EFS_DI_W0_NLINK_SHIFT 0
#define EFS_DI_W0_NLINK_MASK  0xffffu

/* DI_W1: the 32-bit big-endian word at byte 4 */
/* uid: bytes 4 to 5 */
/* FIELD: DI_W1.uid 31:16 established */
#define EFS_DI_W1_UID_SHIFT 16
#define EFS_DI_W1_UID_MASK  0xffffu
/* gid: bytes 6 to 7 */
/* FIELD: DI_W1.gid 15:0 established */
#define EFS_DI_W1_GID_SHIFT 0
#define EFS_DI_W1_GID_MASK  0xffffu

/* DI_W2: the 32-bit big-endian word at byte 8 */
/* size: bytes; 0 for device nodes */
/* FIELD: DI_W2.size 31:0 established */
#define EFS_DI_W2_SIZE_SHIFT 0
#define EFS_DI_W2_SIZE_MASK  0xffffffffu

/* DI_W3: the 32-bit big-endian word at byte 12 */
/* atime: seconds */
/* FIELD: DI_W3.atime 31:0 established */
#define EFS_DI_W3_ATIME_SHIFT 0
#define EFS_DI_W3_ATIME_MASK  0xffffffffu

/* DI_W4: the 32-bit big-endian word at byte 16 */
/* mtime: seconds */
/* FIELD: DI_W4.mtime 31:0 established */
#define EFS_DI_W4_MTIME_SHIFT 0
#define EFS_DI_W4_MTIME_MASK  0xffffffffu

/* DI_W5: the 32-bit big-endian word at byte 20 */
/* ctime: seconds */
/* FIELD: DI_W5.ctime 31:0 established */
#define EFS_DI_W5_CTIME_SHIFT 0
#define EFS_DI_W5_CTIME_MASK  0xffffffffu

/* DI_W6: the 32-bit big-endian word at byte 24 */
/* gen: generation, incremented when the inode is freed */
/* FIELD: DI_W6.gen 31:0 established */
#define EFS_DI_W6_GEN_SHIFT 0
#define EFS_DI_W6_GEN_MASK  0xffffffffu

/* DI_W7: the 32-bit big-endian word at byte 28 */
/* numextents: total extents; more than 12 means the list is indirect, bytes 0x1c to 0x1d */
/* FIELD: DI_W7.numextents 31:16 established */
#define EFS_DI_W7_NUMEXTENTS_SHIFT 16
#define EFS_DI_W7_NUMEXTENTS_MASK  0xffffu
/* version: 0 in every image; 1 and 2 need a conversion hook the kernel may lack, byte 0x1e */
/* FIELD: DI_W7.version 15:8 established */
#define EFS_DI_W7_VERSION_SHIFT 8
#define EFS_DI_W7_VERSION_MASK  0xffu
/* spare: byte 0x1f, zero in every image */
/* FIELD: DI_W7.spare 7:0 inferred */
#define EFS_DI_W7_SPARE_SHIFT 0
#define EFS_DI_W7_SPARE_MASK  0xffu

/* ---- device number in the inode data area (bytes 0x20 to 0x27) ---- */

/* DEV_W8: the 32-bit big-endian word at byte 32 */
/* odev: 16-bit short form; 0xffff means the long form follows, bytes 0x20 to 0x21 */
/* FIELD: DEV_W8.odev 31:16 established */
#define EFS_DEV_W8_ODEV_SHIFT 16
#define EFS_DEV_W8_ODEV_MASK  0xffffu

/* DEV_W9: the 32-bit big-endian word at byte 36 */
/* ndev: 32-bit long form, used when the short form is 0xffff */
/* FIELD: DEV_W9.ndev 31:0 established */
#define EFS_DEV_W9_NDEV_SHIFT 0
#define EFS_DEV_W9_NDEV_MASK  0xffffffffu

/* ---- directory block header (512-byte block) ---- */
#define EFS_DB_SIZE 512 /* established */

/* DB_W0: the 32-bit big-endian word at byte 0 */
/* magic: bytes 0 to 1 */
/* FIELD: DB_W0.magic 31:16 established */
#define EFS_DB_W0_MAGIC_SHIFT 16
#define EFS_DB_W0_MAGIC_MASK  0xffffu
/* firstused: offset of the lowest entry in 2-byte units, 0 means no entries, byte 2 */
/* FIELD: DB_W0.firstused 15:8 established */
#define EFS_DB_W0_FIRSTUSED_SHIFT 8
#define EFS_DB_W0_FIRSTUSED_MASK  0xffu
/* slots: slot bytes in use, at most 0x48, byte 3 */
/* FIELD: DB_W0.slots 7:0 established */
#define EFS_DB_W0_SLOTS_SHIFT 0
#define EFS_DB_W0_SLOTS_MASK  0xffu

/*
 * Vectors. TESTVEC values are ones the notes record; EXAMPLE lines only show a layout.
 * EXAMPLE (unverified): 0x77770000 as SB_W5 => dirty=0x7777
 */
/*
 * EXAMPLE (unverified): 0x000019b2 as EXT_W0 => magic=0, bn=0x19b2
 */
/*
 * EXAMPLE (unverified): 0x01000001 as EXT_W1 => length=1, offset=1
 */
/*
 * EXAMPLE (unverified): 0xbeef0300 as DB_W0 => magic=0xbeef, firstused=3, slots=0
 */
#endif
