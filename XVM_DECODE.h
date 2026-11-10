/*
 * XVM_DECODE.h - XVM volume manager on-disk label structures
 *
 * Publish-safe contract for the labels the XVM volume manager keeps on each
 * disk: a physical label, then label sectors holding virtual-element entries
 * and attribute entries. Every multi-byte integer on disk is big-endian. UUIDs
 * and name strings are opaque bytes and are never byte-swapped.
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
 * Derivation. Offsets, sizes, magic values and enumerations come from reading
 * the shipped volume manager code and tool strings, and from the field names
 * and layouts that the shipped Linux kernel module's debug information declares
 * for the same records. The physical label is shown in its 124-byte, version 2
 * form; a later Linux release widened it to 160 bytes and is not described here.
 * No real disk image carrying a label was available, so an inferred grade means
 * a reading that a real image would settle.
 */
#ifndef XVM_DECODE_H
#define XVM_DECODE_H

#include <stdint.h>

#define XVM_PHYSLAB_MAGIC 0x786c6162u /* established: bytes 78 6c 61 62 */
#define XVM_PHYSLAB_VERSION 2 /* established */
#define XVM_PHYSLAB_VERSION_LEGACY 1 /* established: converted on read */
#define XVM_PHYSLAB_START_BLOCK 1 /* established: device block holding the label on a disk with an SGI volume header */
#define XVM_LABELHDR_MAGIC 0x6c646872u /* established: bytes 6c 64 68 72 */
#define XVM_LABELHDR_VERSION 2 /* established */
#define XVM_LABELHDR_VERSION_LEGACY 1 /* established: accepted for element sectors only */
#define XVM_LABENT_SIZE 72 /* established: bytes per entry in a label sector */
#define XVM_NAME_MAX 48 /* established: bytes in an object name */
#define XVM_HOSTNAME_MAX 32 /* established */
#define XVM_DISKNAME_MAX 48 /* established */

/* ---- physical label (the disk label sector) ---- */
#define XVM_PL_SIZE 124 /* established */
#define XVM_PL_OFF_HOSTNAME 8 /* established: owning host in the private domain; a version 1 label held the disk name here */
#define XVM_PL_LEN_HOSTNAME 32 /* established */
#define XVM_PL_OFF_UUID 40 /* established: physical volume UUID, opaque bytes */
#define XVM_PL_LEN_UUID 16 /* established */
#define XVM_PL_OFF_DISKNAME 72 /* established: physical volume name, NUL padded */
#define XVM_PL_LEN_DISKNAME 48 /* established */

/* PL_W0: the 32-bit big-endian word at byte 0 */
/* magic: bytes 0 to 3 */
/* FIELD: PL_W0.magic 31:0 established */
#define XVM_PL_W0_MAGIC_SHIFT 0
#define XVM_PL_W0_MAGIC_MASK  0xffffffffu

/* PL_W1: the 32-bit big-endian word at byte 4 */
/* version: bytes 4 to 5 */
/* FIELD: PL_W1.version 31:16 established */
#define XVM_PL_W1_VERSION_SHIFT 16
#define XVM_PL_W1_VERSION_MASK  0xffffu
/* state_draining: state byte (byte 6) bit 6 */
/* FIELD: PL_W1.state_draining 14:14 inferred */
#define XVM_PL_W1_STATE_DRAINING_SHIFT 14
#define XVM_PL_W1_STATE_DRAINING_MASK  0x1u
/* state_cluster: state byte (byte 6) bit 5 */
/* FIELD: PL_W1.state_cluster 13:13 established */
#define XVM_PL_W1_STATE_CLUSTER_SHIFT 13
#define XVM_PL_W1_STATE_CLUSTER_MASK  0x1u
/* state_private: state byte (byte 6) bit 4 */
/* FIELD: PL_W1.state_private 12:12 established */
#define XVM_PL_W1_STATE_PRIVATE_SHIFT 12
#define XVM_PL_W1_STATE_PRIVATE_MASK  0x1u
/* state_recovering: state byte (byte 6) bit 3 */
/* FIELD: PL_W1.state_recovering 11:11 inferred */
#define XVM_PL_W1_STATE_RECOVERING_SHIFT 11
#define XVM_PL_W1_STATE_RECOVERING_MASK  0x1u
/* state_mediaerror: state byte (byte 6) bit 2 */
/* FIELD: PL_W1.state_mediaerror 10:10 inferred */
#define XVM_PL_W1_STATE_MEDIAERROR_SHIFT 10
#define XVM_PL_W1_STATE_MEDIAERROR_MASK  0x1u
/* state_spare: state byte (byte 6) bit 1 */
/* FIELD: PL_W1.state_spare 9:9 inferred */
#define XVM_PL_W1_STATE_SPARE_SHIFT 9
#define XVM_PL_W1_STATE_SPARE_MASK  0x1u
/* state_online: state byte (byte 6) bit 0 */
/* FIELD: PL_W1.state_online 8:8 inferred */
#define XVM_PL_W1_STATE_ONLINE_SHIFT 8
#define XVM_PL_W1_STATE_ONLINE_MASK  0x1u
/* flags_sync: flags byte (byte 7) bit 3 */
/* FIELD: PL_W1.flags_sync 3:3 established */
#define XVM_PL_W1_FLAGS_SYNC_SHIFT 3
#define XVM_PL_W1_FLAGS_SYNC_MASK  0x1u
/* flags_duplicate: flags byte (byte 7) bit 2 */
/* FIELD: PL_W1.flags_duplicate 2:2 established */
#define XVM_PL_W1_FLAGS_DUPLICATE_SHIFT 2
#define XVM_PL_W1_FLAGS_DUPLICATE_MASK  0x1u
/* flags_bootable: flags byte (byte 7) bit 1 */
/* FIELD: PL_W1.flags_bootable 1:1 established */
#define XVM_PL_W1_FLAGS_BOOTABLE_SHIFT 1
#define XVM_PL_W1_FLAGS_BOOTABLE_MASK  0x1u

/* PL_W14: the 32-bit big-endian word at byte 56 */
/* secsize: sector size in units of 512 bytes, bytes 56 to 57 */
/* FIELD: PL_W14.secsize 31:16 established */
#define XVM_PL_W14_SECSIZE_SHIFT 16
#define XVM_PL_W14_SECSIZE_MASK  0xffffu
/* xvmlabel: first block of the label area, bytes 58 to 59 */
/* FIELD: PL_W14.xvmlabel 15:0 established */
#define XVM_PL_W14_XVMLABEL_SHIFT 0
#define XVM_PL_W14_XVMLABEL_MASK  0xffffu

/* PL_W15: the 32-bit big-endian word at byte 60 */
/* xvmlabelblks: label area length in blocks, bytes 60 to 61 */
/* FIELD: PL_W15.xvmlabelblks 31:16 established */
#define XVM_PL_W15_XVMLABELBLKS_SHIFT 16
#define XVM_PL_W15_XVMLABELBLKS_MASK  0xffffu
/* dbn: first block of the user area, bytes 62 to 63 */
/* FIELD: PL_W15.dbn 15:0 established */
#define XVM_PL_W15_DBN_SHIFT 0
#define XVM_PL_W15_DBN_MASK  0xffffu

/* PL_W16: the 32-bit big-endian word at byte 64 */
/* nblks: bytes 64 to 67 */
/* FIELD: PL_W16.nblks 31:0 established */
#define XVM_PL_W16_NBLKS_SHIFT 0
#define XVM_PL_W16_NBLKS_MASK  0xffffffffu

/* PL_W17: the 32-bit big-endian word at byte 68 */
/* timestamp: last label update in seconds, bytes 68 to 71 */
/* FIELD: PL_W17.timestamp 31:0 established */
#define XVM_PL_W17_TIMESTAMP_SHIFT 0
#define XVM_PL_W17_TIMESTAMP_MASK  0xffffffffu

/* PL_W30: the 32-bit big-endian word at byte 120 */
/* clusterid: bytes 120 to 123 */
/* FIELD: PL_W30.clusterid 31:0 established */
#define XVM_PL_W30_CLUSTERID_SHIFT 0
#define XVM_PL_W30_CLUSTERID_MASK  0xffffffffu

/* ---- label sector header ---- */
#define XVM_LH_SIZE 8 /* established */

/* LH_W0: the 32-bit big-endian word at byte 0 */
/* magic: bytes 0 to 3 */
/* FIELD: LH_W0.magic 31:0 established */
#define XVM_LH_W0_MAGIC_SHIFT 0
#define XVM_LH_W0_MAGIC_MASK  0xffffffffu

/* LH_W1: the 32-bit big-endian word at byte 4 */
/* version: bytes 4 to 5 */
/* FIELD: LH_W1.version 31:16 established */
#define XVM_LH_W1_VERSION_SHIFT 16
#define XVM_LH_W1_VERSION_MASK  0xffffu
/* type: bytes 6 to 6 */
/* FIELD: LH_W1.type 15:8 established */
#define XVM_LH_W1_TYPE_SHIFT 8
#define XVM_LH_W1_TYPE_MASK  0xffu
/* ENUM: LH_W1.type 0 free established */
#define XVM_LH_W1_TYPE_FREE 0
/* ENUM: LH_W1.type 1 reserved unknown */
#define XVM_LH_W1_TYPE_RESERVED 1
/* ENUM: LH_W1.type 2 ve established */
#define XVM_LH_W1_TYPE_VE 2
/* ENUM: LH_W1.type 3 attr established */
#define XVM_LH_W1_TYPE_ATTR 3
/* size: entry size; readers accept only 72, byte 7 */
/* FIELD: LH_W1.size 7:0 established */
#define XVM_LH_W1_SIZE_SHIFT 0
#define XVM_LH_W1_SIZE_MASK  0xffu

/* ---- virtual-element entry (one piece of one object) ---- */
#define XVM_VE_SIZE 72 /* established */
#define XVM_VE_OFF_UUID 8 /* established: this object, opaque bytes */
#define XVM_VE_LEN_UUID 16 /* established */
#define XVM_VE_OFF_CHILDUUID 24 /* established: the child at the piece position, opaque bytes */
#define XVM_VE_LEN_CHILDUUID 16 /* established */

/* VE_W0: the 32-bit big-endian word at byte 0 */
/* type: bytes 0 to 0 */
/* FIELD: VE_W0.type 31:24 established */
#define XVM_VE_W0_TYPE_SHIFT 24
#define XVM_VE_W0_TYPE_MASK  0xffu
/* ENUM: VE_W0.type 0 free established */
#define XVM_VE_W0_TYPE_FREE 0
/* ENUM: VE_W0.type 1 slice established */
#define XVM_VE_W0_TYPE_SLICE 1
/* ENUM: VE_W0.type 2 concat established */
#define XVM_VE_W0_TYPE_CONCAT 2
/* ENUM: VE_W0.type 3 stripe established */
#define XVM_VE_W0_TYPE_STRIPE 3
/* ENUM: VE_W0.type 4 raid established */
#define XVM_VE_W0_TYPE_RAID 4
/* ENUM: VE_W0.type 5 mirror established */
#define XVM_VE_W0_TYPE_MIRROR 5
/* ENUM: VE_W0.type 6 subvol established */
#define XVM_VE_W0_TYPE_SUBVOL 6
/* ENUM: VE_W0.type 7 volume established */
#define XVM_VE_W0_TYPE_VOLUME 7
/* ENUM: VE_W0.type 11 cow established */
#define XVM_VE_W0_TYPE_COW 11
/* ENUM: VE_W0.type 12 snapshot established */
#define XVM_VE_W0_TYPE_SNAPSHOT 12
/* state_disabled: state byte (byte 1) bit 4 */
/* FIELD: VE_W0.state_disabled 20:20 established */
#define XVM_VE_W0_STATE_DISABLED_SHIFT 20
#define XVM_VE_W0_STATE_DISABLED_MASK  0x1u
/* state_valid: state byte (byte 1) bit 1 */
/* FIELD: VE_W0.state_valid 17:17 established */
#define XVM_VE_W0_STATE_VALID_SHIFT 17
#define XVM_VE_W0_STATE_VALID_MASK  0x1u
/* state_clean: state byte (byte 1) bit 0 */
/* FIELD: VE_W0.state_clean 16:16 established */
#define XVM_VE_W0_STATE_CLEAN_SHIFT 16
#define XVM_VE_W0_STATE_CLEAN_MASK  0x1u
/* pieces: bytes 2 to 3 */
/* FIELD: VE_W0.pieces 15:0 established */
#define XVM_VE_W0_PIECES_SHIFT 0
#define XVM_VE_W0_PIECES_MASK  0xffffu

/* VE_W1: the 32-bit big-endian word at byte 4 */
/* tid: bytes 4 to 7 */
/* FIELD: VE_W1.tid 31:0 established */
#define XVM_VE_W1_TID_SHIFT 0
#define XVM_VE_W1_TID_MASK  0xffffffffu

/* VE_W10: the 32-bit big-endian word at byte 40 */
/* length_hi: high word of a 64-bit value, bytes 40 to 43 */
/* FIELD: VE_W10.length_hi 31:0 established */
#define XVM_VE_W10_LENGTH_HI_SHIFT 0
#define XVM_VE_W10_LENGTH_HI_MASK  0xffffffffu

/* VE_W11: the 32-bit big-endian word at byte 44 */
/* length_lo: low word of the same value, bytes 44 to 47 */
/* FIELD: VE_W11.length_lo 31:0 established */
#define XVM_VE_W11_LENGTH_LO_SHIFT 0
#define XVM_VE_W11_LENGTH_LO_MASK  0xffffffffu

/* VE_W12: the 32-bit big-endian word at byte 48 */
/* pos: bytes 48 to 49 */
/* FIELD: VE_W12.pos 31:16 established */
#define XVM_VE_W12_POS_SHIFT 16
#define XVM_VE_W12_POS_MASK  0xffffu
/* cowfirst: bytes 50 to 51 */
/* FIELD: VE_W12.cowfirst 15:0 established */
#define XVM_VE_W12_COWFIRST_SHIFT 0
#define XVM_VE_W12_COWFIRST_MASK  0xffffu

/* VE_W13: the 32-bit big-endian word at byte 52 */
/* cownext: bytes 52 to 53 */
/* FIELD: VE_W13.cownext 31:16 established */
#define XVM_VE_W13_COWNEXT_SHIFT 16
#define XVM_VE_W13_COWNEXT_MASK  0xffffu
/* cowregsz: bytes 54 to 55 */
/* FIELD: VE_W13.cowregsz 15:0 established */
#define XVM_VE_W13_COWREGSZ_SHIFT 0
#define XVM_VE_W13_COWREGSZ_MASK  0xffffu

/* ---- variant for object type slice (bytes 56 to 71 of the entry) ---- */

/* VE_SLICE_W14: the 32-bit big-endian word at byte 56 */
/* startblk_hi: high word of a 64-bit value, bytes 56 to 59 */
/* FIELD: VE_SLICE_W14.startblk_hi 31:0 established */
#define XVM_VE_SLICE_W14_STARTBLK_HI_SHIFT 0
#define XVM_VE_SLICE_W14_STARTBLK_HI_MASK  0xffffffffu

/* VE_SLICE_W15: the 32-bit big-endian word at byte 60 */
/* startblk_lo: low word of the same value, bytes 60 to 63 */
/* FIELD: VE_SLICE_W15.startblk_lo 31:0 established */
#define XVM_VE_SLICE_W15_STARTBLK_LO_SHIFT 0
#define XVM_VE_SLICE_W15_STARTBLK_LO_MASK  0xffffffffu

/* VE_SLICE_W16: the 32-bit big-endian word at byte 64 */
/* seq: bytes 64 to 65 */
/* FIELD: VE_SLICE_W16.seq 31:16 established */
#define XVM_VE_SLICE_W16_SEQ_SHIFT 16
#define XVM_VE_SLICE_W16_SEQ_MASK  0xffffu
/* flags: byte 66; bits 0 to 2 are set from request flags, names not recovered */
/* FIELD: VE_SLICE_W16.flags 15:8 established */
#define XVM_VE_SLICE_W16_FLAGS_SHIFT 8
#define XVM_VE_SLICE_W16_FLAGS_MASK  0xffu
/* partition: byte 67; the volume-header partition number the slice maps to */
/* FIELD: VE_SLICE_W16.partition 7:0 established */
#define XVM_VE_SLICE_W16_PARTITION_SHIFT 0
#define XVM_VE_SLICE_W16_PARTITION_MASK  0xffu

/* ---- variant for object type stripe ---- */

/* VE_STRIPE_W14: the 32-bit big-endian word at byte 56 */
/* susize: bytes 56 to 59 */
/* FIELD: VE_STRIPE_W14.susize 31:0 established */
#define XVM_VE_STRIPE_W14_SUSIZE_SHIFT 0
#define XVM_VE_STRIPE_W14_SUSIZE_MASK  0xffffffffu

/* ---- variant for object type mirror ---- */

/* VE_MIRROR_W14: the 32-bit big-endian word at byte 56 */
/* oldlength_hi: high word of a 64-bit value, bytes 56 to 59 */
/* FIELD: VE_MIRROR_W14.oldlength_hi 31:0 established */
#define XVM_VE_MIRROR_W14_OLDLENGTH_HI_SHIFT 0
#define XVM_VE_MIRROR_W14_OLDLENGTH_HI_MASK  0xffffffffu

/* VE_MIRROR_W15: the 32-bit big-endian word at byte 60 */
/* oldlength_lo: low word of the same value, bytes 60 to 63 */
/* FIELD: VE_MIRROR_W15.oldlength_lo 31:0 established */
#define XVM_VE_MIRROR_W15_OLDLENGTH_LO_SHIFT 0
#define XVM_VE_MIRROR_W15_OLDLENGTH_LO_MASK  0xffffffffu

/* VE_MIRROR_W16: the 32-bit big-endian word at byte 64 */
/* logfreq: bytes 64 to 65 */
/* FIELD: VE_MIRROR_W16.logfreq 31:16 established */
#define XVM_VE_MIRROR_W16_LOGFREQ_SHIFT 16
#define XVM_VE_MIRROR_W16_LOGFREQ_MASK  0xffffu
/* policy: bytes 66 to 66 */
/* FIELD: VE_MIRROR_W16.policy 15:8 established */
#define XVM_VE_MIRROR_W16_POLICY_SHIFT 8
#define XVM_VE_MIRROR_W16_POLICY_MASK  0xffu
/* ENUM: VE_MIRROR_W16.policy 0 round_robin established */
#define XVM_VE_MIRROR_W16_POLICY_ROUND_ROBIN 0
/* ENUM: VE_MIRROR_W16.policy 1 sequential established */
#define XVM_VE_MIRROR_W16_POLICY_SEQUENTIAL 1
/* config: byte 67; bit 0 is kept only on the primary piece */
/* FIELD: VE_MIRROR_W16.config 7:0 inferred */
#define XVM_VE_MIRROR_W16_CONFIG_SHIFT 0
#define XVM_VE_MIRROR_W16_CONFIG_MASK  0xffu

/* ---- variant for object type raid (layout from the published Linux module only) ---- */

/* VE_RAID_W14: the 32-bit big-endian word at byte 56 */
/* susize: bytes 56 to 59 */
/* FIELD: VE_RAID_W14.susize 31:0 inferred */
#define XVM_VE_RAID_W14_SUSIZE_SHIFT 0
#define XVM_VE_RAID_W14_SUSIZE_MASK  0xffffffffu

/* VE_RAID_W15: the 32-bit big-endian word at byte 60 */
/* layout: bytes 60 to 60 */
/* FIELD: VE_RAID_W15.layout 31:24 inferred */
#define XVM_VE_RAID_W15_LAYOUT_SHIFT 24
#define XVM_VE_RAID_W15_LAYOUT_MASK  0xffu

/* ---- variant for object type copy-on-write ---- */
#define XVM_VE_COW_OFF_RUUID 56 /* established: repository object UUID, opaque bytes; no word fields */
#define XVM_VE_COW_LEN_RUUID 16 /* established */

/* ---- variant for object type subvolume ---- */

/* VE_SUBVOL_W14: the 32-bit big-endian word at byte 56 */
/* svtype: bytes 56 to 56 */
/* FIELD: VE_SUBVOL_W14.svtype 31:24 established */
#define XVM_VE_SUBVOL_W14_SVTYPE_SHIFT 24
#define XVM_VE_SUBVOL_W14_SVTYPE_MASK  0xffu
/* ENUM: VE_SUBVOL_W14.svtype 0 all established */
#define XVM_VE_SUBVOL_W14_SVTYPE_ALL 0
/* ENUM: VE_SUBVOL_W14.svtype 1 log established */
#define XVM_VE_SUBVOL_W14_SVTYPE_LOG 1
/* ENUM: VE_SUBVOL_W14.svtype 2 data established */
#define XVM_VE_SUBVOL_W14_SVTYPE_DATA 2
/* ENUM: VE_SUBVOL_W14.svtype 3 rt established */
#define XVM_VE_SUBVOL_W14_SVTYPE_RT 3
/* ENUM: VE_SUBVOL_W14.svtype 4 swap established */
#define XVM_VE_SUBVOL_W14_SVTYPE_SWAP 4
/* ENUM: VE_SUBVOL_W14.svtype 16 user1 established */
#define XVM_VE_SUBVOL_W14_SVTYPE_USER1 16

/* ---- variant for object type volume ---- */

/* VE_VOL_W14: the 32-bit big-endian word at byte 56 */
/* repfull: bytes 56 to 56 */
/* FIELD: VE_VOL_W14.repfull 31:24 established */
#define XVM_VE_VOL_W14_REPFULL_SHIFT 24
#define XVM_VE_VOL_W14_REPFULL_MASK  0xffu

/* ---- attribute entry (a name or a permission block) ---- */
#define XVM_ATTR_SIZE 72 /* established */
#define XVM_ATTR_OFF_UUID 8 /* established: the object named or protected, opaque bytes */
#define XVM_ATTR_LEN_UUID 16 /* established */
#define XVM_ATTR_OFF_NAME 24 /* established: for a name entry: NUL padded string */
#define XVM_ATTR_LEN_NAME 48 /* established */

/* ATTR_W0: the 32-bit big-endian word at byte 0 */
/* type: bytes 0 to 0 */
/* FIELD: ATTR_W0.type 31:24 established */
#define XVM_ATTR_W0_TYPE_SHIFT 24
#define XVM_ATTR_W0_TYPE_MASK  0xffu
/* ENUM: ATTR_W0.type 0 free established */
#define XVM_ATTR_W0_TYPE_FREE 0
/* ENUM: ATTR_W0.type 1 name established */
#define XVM_ATTR_W0_TYPE_NAME 1
/* ENUM: ATTR_W0.type 2 perm established */
#define XVM_ATTR_W0_TYPE_PERM 2
/* flag_persistent: flag byte (byte 1) bit 0: the name was assigned by the user */
/* FIELD: ATTR_W0.flag_persistent 16:16 inferred */
#define XVM_ATTR_W0_FLAG_PERSISTENT_SHIFT 16
#define XVM_ATTR_W0_FLAG_PERSISTENT_MASK  0x1u

/* ATTR_W1: the 32-bit big-endian word at byte 4 */
/* tid: bytes 4 to 7 */
/* FIELD: ATTR_W1.tid 31:0 established */
#define XVM_ATTR_W1_TID_SHIFT 0
#define XVM_ATTR_W1_TID_MASK  0xffffffffu

/* ---- variant for a permission entry (bytes 24 to 35 of the entry) ---- */

/* ATTR_PERM_W6: the 32-bit big-endian word at byte 24 */
/* uid: signed 32-bit value, bytes 24 to 27 */
/* FIELD: ATTR_PERM_W6.uid 31:0 established */
#define XVM_ATTR_PERM_W6_UID_SHIFT 0
#define XVM_ATTR_PERM_W6_UID_MASK  0xffffffffu

/* ATTR_PERM_W7: the 32-bit big-endian word at byte 28 */
/* gid: signed 32-bit value, bytes 28 to 31 */
/* FIELD: ATTR_PERM_W7.gid 31:0 established */
#define XVM_ATTR_PERM_W7_GID_SHIFT 0
#define XVM_ATTR_PERM_W7_GID_MASK  0xffffffffu

/* ATTR_PERM_W8: the 32-bit big-endian word at byte 32 */
/* mode: bytes 32 to 35 */
/* FIELD: ATTR_PERM_W8.mode 31:0 established */
#define XVM_ATTR_PERM_W8_MODE_SHIFT 0
#define XVM_ATTR_PERM_W8_MODE_MASK  0xffffffffu

/*
 * Vectors. TESTVEC values are ones the notes record; EXAMPLE lines only show a layout.
 * EXAMPLE (unverified): 0x00020348 as LH_W1 => version=2, type=3, size=0x48
 */
/*
 * EXAMPLE (unverified): 0x03110002 as VE_W0 => type=3, state_clean=1, state_valid=0, state_disabled=1, pieces=2
 */
#endif
