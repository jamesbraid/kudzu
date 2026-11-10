/*
 * VOLHDR_DECODE.h - SGI disk volume header
 *
 * Publish-safe contract for the 512-byte volume header at block 0 of an
 * SGI-labelled disk: a magic, root and swap partition numbers, a boot file name,
 * drive parameters, a 15-entry volume directory of named files, a 16-entry
 * partition table and a checksum. Every multi-byte integer is big-endian. All
 * block numbers and counts are in 512-byte units.
 *
 * Partition conventions (established by the tools that build headers, not by the
 * format): slot 0 is the default root, slot 1 the default swap, slot 8 the volume
 * header partition itself (type volhdr, starting at block 0) and slot 10 the
 * whole disk (type volume, starting at block 0).
 *
 * Checksum: read the 512 bytes as 128 big-endian words. The header is valid when
 * the 32-bit wrapping sum of all 128 words, including the checksum word, is
 * zero. To write one, set the checksum word to 0, sum the other 127 words and
 * store the negation of the sum.
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
 * Derivation. Offsets, sizes and the partition type numbers come from reading how
 * the shipped disk tools, the boot loader, the disk-label library and the kernel
 * disk driver read, write and validate the header, and from one real block 0
 * written by IRIX itself. The drive-parameter fields other than the ones listed
 * are unrecovered and zero in that block. The real block came from an emulated
 * disk, so its geometry values reflect the emulator, not a drive.
 */
#ifndef VOLHDR_DECODE_H
#define VOLHDR_DECODE_H

#include <stdint.h>

#define DVH_SIZE 512 /* established: bytes in the header */
#define DVH_MAGIC 0x0be5a941u /* established: bytes 0b e5 a9 41 */
#define DVH_BOOTFILE_OFF 0x08 /* established */
#define DVH_DP_OFF 0x18 /* established: drive parameters, 48 bytes */
#define DVH_DP_BYTES 48 /* established */
#define DVH_DP_FLAG_TAGGED 0x40 /* inferred: set together with the tag depth */
#define DVH_DIR_OFF 0x48 /* established: start of the volume directory */
#define DVH_DIR_COUNT 15 /* established: directory entries */
#define DVH_DIR_STRIDE 16 /* established: bytes per directory entry */
#define DVH_DIR_NAME_LEN 8 /* established: name bytes in a directory entry */
#define DVH_PART_OFF 0x138 /* established: start of the partition table */
#define DVH_PART_COUNT 16 /* established: partition entries */
#define DVH_PART_STRIDE 12 /* established: bytes per partition entry */
#define DVH_CSUM_OFF 0x1f8 /* established: checksum word */
#define DVH_SLOT_ROOT 0 /* established: conventional root partition slot */
#define DVH_SLOT_SWAP 1 /* established: conventional swap partition slot */
#define DVH_SLOT_VOLHDR 8 /* established: conventional volume-header partition slot */
#define DVH_SLOT_WHOLEDISK 10 /* established: conventional whole-disk partition slot */

/* ---- volume header (block 0 of the disk, 512 bytes) ---- */
#define DVH_HDR_SIZE 512 /* established */
#define DVH_HDR_OFF_BOOTFILE 8 /* established: boot file name, NUL padded, first byte 0 means none */
#define DVH_HDR_LEN_BOOTFILE 16 /* established */

/* HDR_W0: the 32-bit big-endian word at byte 0 */
/* magic: bytes 0 to 3 */
/* FIELD: HDR_W0.magic 31:0 established */
#define DVH_HDR_W0_MAGIC_SHIFT 0
#define DVH_HDR_W0_MAGIC_MASK  0xffffffffu

/* HDR_W1: the 32-bit big-endian word at byte 4 */
/* root: root partition number, signed 16 bits, bytes 4 to 5 */
/* FIELD: HDR_W1.root 31:16 established */
#define DVH_HDR_W1_ROOT_SHIFT 16
#define DVH_HDR_W1_ROOT_MASK  0xffffu
/* swap: swap partition number, signed 16 bits, bytes 6 to 7 */
/* FIELD: HDR_W1.swap 15:0 established */
#define DVH_HDR_W1_SWAP_SHIFT 0
#define DVH_HDR_W1_SWAP_MASK  0xffffu

/* HDR_W7: the 32-bit big-endian word at byte 28 */
/* dp_cylinders: drive parameters start at byte 0x18 and take 48 bytes; cylinder count, bytes 0x1c to 0x1d */
/* FIELD: HDR_W7.dp_cylinders 31:16 inferred */
#define DVH_HDR_W7_DP_CYLINDERS_SHIFT 16
#define DVH_HDR_W7_DP_CYLINDERS_MASK  0xffffu
/* dp_head_offset: zero in the real block, bytes 0x1e to 0x1f */
/* FIELD: HDR_W7.dp_head_offset 15:0 unknown */
#define DVH_HDR_W7_DP_HEAD_OFFSET_SHIFT 0
#define DVH_HDR_W7_DP_HEAD_OFFSET_MASK  0xffffu

/* HDR_W8: the 32-bit big-endian word at byte 32 */
/* dp_heads: tracks per cylinder, bytes 0x20 to 0x21 */
/* FIELD: HDR_W8.dp_heads 31:16 inferred */
#define DVH_HDR_W8_DP_HEADS_SHIFT 16
#define DVH_HDR_W8_DP_HEADS_MASK  0xffffu
/* dp_tag_depth: command queue depth, byte 0x22; written as a field when the drive supports tagged queuing */
/* FIELD: HDR_W8.dp_tag_depth 15:8 established */
#define DVH_HDR_W8_DP_TAG_DEPTH_SHIFT 8
#define DVH_HDR_W8_DP_TAG_DEPTH_MASK  0xffu
/* dp_cyl_high: high cylinder bits, byte 0x23, zero in the real block */
/* FIELD: HDR_W8.dp_cyl_high 7:0 unknown */
#define DVH_HDR_W8_DP_CYL_HIGH_SHIFT 0
#define DVH_HDR_W8_DP_CYL_HIGH_MASK  0xffu

/* HDR_W9: the 32-bit big-endian word at byte 36 */
/* dp_sectors: sectors per track, bytes 0x26 to 0x27 */
/* FIELD: HDR_W9.dp_sectors 15:0 inferred */
#define DVH_HDR_W9_DP_SECTORS_SHIFT 0
#define DVH_HDR_W9_DP_SECTORS_MASK  0xffffu

/* HDR_W10: the 32-bit big-endian word at byte 40 */
/* dp_secbytes: bytes per sector, bytes 0x28 to 0x29 */
/* FIELD: HDR_W10.dp_secbytes 31:16 established */
#define DVH_HDR_W10_DP_SECBYTES_SHIFT 16
#define DVH_HDR_W10_DP_SECBYTES_MASK  0xffffu

/* HDR_W11: the 32-bit big-endian word at byte 44 */
/* dp_flags: bit 0x40 is set together with a tag depth; other bits unknown */
/* FIELD: HDR_W11.dp_flags 31:0 inferred */
#define DVH_HDR_W11_DP_FLAGS_SHIFT 0
#define DVH_HDR_W11_DP_FLAGS_MASK  0xffffffffu

/* HDR_W17: the 32-bit big-endian word at byte 68 */
/* dp_capacity: drive capacity in 512-byte blocks, bytes 0x44 to 0x47 */
/* FIELD: HDR_W17.dp_capacity 31:0 established */
#define DVH_HDR_W17_DP_CAPACITY_SHIFT 0
#define DVH_HDR_W17_DP_CAPACITY_MASK  0xffffffffu

/* HDR_W126: the 32-bit big-endian word at byte 504 */
/* checksum: the 128 words of the header sum to zero with it included */
/* FIELD: HDR_W126.checksum 31:0 established */
#define DVH_HDR_W126_CHECKSUM_SHIFT 0
#define DVH_HDR_W126_CHECKSUM_MASK  0xffffffffu

/* ---- volume directory entry (16 bytes, 15 entries from byte 0x48) ---- */
#define DVH_DIRENT_SIZE 16 /* established */
#define DVH_DIRENT_OFF_NAME 0 /* established: file name, NUL padded, not terminated at 8 characters */
#define DVH_DIRENT_LEN_NAME 8 /* established */

/* DIRENT_W2: the 32-bit big-endian word at byte 8 */
/* start_block: start block in 512-byte blocks from block 0 of the disk, bytes 8 to 11 */
/* FIELD: DIRENT_W2.start_block 31:0 established */
#define DVH_DIRENT_W2_START_BLOCK_SHIFT 0
#define DVH_DIRENT_W2_START_BLOCK_MASK  0xffffffffu

/* DIRENT_W3: the 32-bit big-endian word at byte 12 */
/* length_bytes: length in bytes, 0 marks a free slot, bytes 12 to 15 */
/* FIELD: DIRENT_W3.length_bytes 31:0 established */
#define DVH_DIRENT_W3_LENGTH_BYTES_SHIFT 0
#define DVH_DIRENT_W3_LENGTH_BYTES_MASK  0xffffffffu

/* ---- partition entry (12 bytes, 16 entries from byte 0x138) ---- */
#define DVH_PART_SIZE 12 /* established */

/* PART_W0: the 32-bit big-endian word at byte 0 */
/* nblks: block count, 0 marks an unused entry */
/* FIELD: PART_W0.nblks 31:0 established */
#define DVH_PART_W0_NBLKS_SHIFT 0
#define DVH_PART_W0_NBLKS_MASK  0xffffffffu

/* PART_W1: the 32-bit big-endian word at byte 4 */
/* first: first block in 512-byte blocks from block 0 of the disk */
/* FIELD: PART_W1.first 31:0 established */
#define DVH_PART_W1_FIRST_SHIFT 0
#define DVH_PART_W1_FIRST_MASK  0xffffffffu

/* PART_W2: the 32-bit big-endian word at byte 8 */
/* type: bytes 8 to 11 */
/* FIELD: PART_W2.type 31:0 established */
#define DVH_PART_W2_TYPE_SHIFT 0
#define DVH_PART_W2_TYPE_MASK  0xffffffffu
/* ENUM: PART_W2.type 0 volhdr established */
#define DVH_PART_W2_TYPE_VOLHDR 0
/* ENUM: PART_W2.type 1 trkrepl inferred */
#define DVH_PART_W2_TYPE_TRKREPL 1
/* ENUM: PART_W2.type 2 secrepl inferred */
#define DVH_PART_W2_TYPE_SECREPL 2
/* ENUM: PART_W2.type 3 raw established */
#define DVH_PART_W2_TYPE_RAW 3
/* ENUM: PART_W2.type 4 bsd inferred */
#define DVH_PART_W2_TYPE_BSD 4
/* ENUM: PART_W2.type 5 sysv inferred */
#define DVH_PART_W2_TYPE_SYSV 5
/* ENUM: PART_W2.type 6 volume established */
#define DVH_PART_W2_TYPE_VOLUME 6
/* ENUM: PART_W2.type 7 efs established */
#define DVH_PART_W2_TYPE_EFS 7
/* ENUM: PART_W2.type 8 lvol established */
#define DVH_PART_W2_TYPE_LVOL 8
/* ENUM: PART_W2.type 9 rlvol established */
#define DVH_PART_W2_TYPE_RLVOL 9
/* ENUM: PART_W2.type 10 xfs established */
#define DVH_PART_W2_TYPE_XFS 10
/* ENUM: PART_W2.type 11 xfslog established */
#define DVH_PART_W2_TYPE_XFSLOG 11
/* ENUM: PART_W2.type 12 xlv established */
#define DVH_PART_W2_TYPE_XLV 12
/* ENUM: PART_W2.type 13 xvm established */
#define DVH_PART_W2_TYPE_XVM 13
/* ENUM: PART_W2.type 14 vxvm established */
#define DVH_PART_W2_TYPE_VXVM 14

/*
 * Vectors. TESTVEC values are ones the notes record; EXAMPLE lines only show a layout.
 * TESTVEC: 0x00001000 as PART_W0 => nblks=0x1000
 */
/*
 * TESTVEC: 0x00040000 as PART_W0 => nblks=0x40000
 */
/*
 * TESTVEC: 0x00041000 as PART_W1 => first=0x00041000
 */
/*
 * TESTVEC: 0x00041400 as DIRENT_W3 => length_bytes=0x00041400
 */
/*
 * EXAMPLE (unverified): 0x0000000a as PART_W2 => type=10
 */
#endif
