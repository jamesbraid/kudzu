/*
 * XLV_DECODE.h - XLV logical volume manager on-disk label structures
 *
 * Publish-safe contract for the label the XLV volume manager keeps on each member
 * disk: a master block that selects one of two label slots, and in each slot a
 * header and sixteen entries, one per volume-header partition. Every multi-byte
 * integer is big-endian and is read with plain loads. UUIDs are 16 opaque bytes,
 * compared as raw bytes. Names are fixed width and NUL padded, and may use every
 * byte. The label lives in a file called xlvlab in the disk's volume directory.
 *
 * Label-file layout, S being the disk sector size in bytes (512 when the volume
 * header says 0): the master block takes round-up(512, S) bytes, slot 0 follows
 * and takes round-up(0x12a0, S) bytes, slot 1 follows slot 0 and takes the same.
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
 * Derivation. Offsets, widths and enumerations come from reading how the shipped
 * kernel module, the label daemon and the volume tools read, write and print the
 * label; where a reader, a writer and a printer agree the grade is established.
 * No real disk carrying a label was available, so no byte of a real label has
 * been compared with this description. Entry fields whose bit meanings were not
 * recovered carry only their width.
 */
#ifndef XLV_DECODE_H
#define XLV_DECODE_H

#include <stdint.h>

#define XLV_LABEL_MAGIC 0x11201989u /* established: bytes 11 20 19 89 */
#define XLV_LABEL_VERSION 2 /* established: the value writers store; no reader checks it */
#define XLV_LABEL_BYTES 0x12a0 /* established: used bytes of one label slot */
#define XLV_MASTER_BYTES 512 /* established: master block size before rounding to the sector size */
#define XLV_NPART 16 /* established: entries per label, one per partition */
#define XLV_NAME_LEN 32 /* established: bytes in a volume, plex or element name */
#define XLV_NODENAME_LEN 0x108 /* established: bytes reserved for the owning host name */
#define XLV_NODENAME_COPY 0x102 /* established: bytes the writers copy into the host name */
#define XLV_ENTRY_SIZE 0x118 /* established */
#define XLV_HDR_SIZE 0x120 /* established */
#define XLV_ENTRY_OFF 0x120 /* established: offset of entry 0 in a slot */
#define XLV_PTYPE_VOLHDR 0 /* established: volume-header partition type of the header partition */
#define XLV_PTYPE_RAW 3 /* established */
#define XLV_PTYPE_EFS_LEGACY 5 /* established: type 5 is mapped to the name efs by the shipped name table */
#define XLV_PTYPE_VOLUME 6 /* established: whole disk */
#define XLV_PTYPE_EFS 7 /* established */
#define XLV_PTYPE_XFS 10 /* established */
#define XLV_PTYPE_XFSLOG 11 /* established: accepted for a log subvolume */
#define XLV_PTYPE_XLV 12 /* established: accepted for a log, data or real-time subvolume */
#define XLV_PTYPE_XVM 13 /* established */
#define XLV_PTYPE_VXVM 14 /* established */

/* ---- master block (first 8 bytes only are known) ---- */

/* MASTER_W0: the 32-bit big-endian word at byte 0 */
/* seq_hi: high word of the 64-bit update counter, bytes 0 to 3 */
/* FIELD: MASTER_W0.seq_hi 31:0 established */
#define XLV_MASTER_W0_SEQ_HI_SHIFT 0
#define XLV_MASTER_W0_SEQ_HI_MASK  0xffffffffu

/* MASTER_W1: the 32-bit big-endian word at byte 4 */
/* seq_lo_rest: bits 31:1 of the low word of the update counter */
/* FIELD: MASTER_W1.seq_lo_rest 31:1 established */
#define XLV_MASTER_W1_SEQ_LO_REST_SHIFT 1
#define XLV_MASTER_W1_SEQ_LO_REST_MASK  0x7fffffffu
/* seq_lo_slot: bit 0 of the update counter selects the current slot: 0 is slot 0, 1 is slot 1 */
/* FIELD: MASTER_W1.seq_lo_slot 0:0 established */
#define XLV_MASTER_W1_SEQ_LO_SLOT_SHIFT 0
#define XLV_MASTER_W1_SEQ_LO_SLOT_MASK  0x1u

/* ---- label slot header (0x120 bytes) ---- */
#define XLV_LBL_SIZE 0x120 /* established */
#define XLV_LBL_OFF_NODENAME 24 /* established: owning host name; writers copy 0x102 bytes */
#define XLV_LBL_LEN_NODENAME 264 /* established */

/* LBL_W0: the 32-bit big-endian word at byte 0 */
/* magic: bytes 0 to 3 */
/* FIELD: LBL_W0.magic 31:0 established */
#define XLV_LBL_W0_MAGIC_SHIFT 0
#define XLV_LBL_W0_MAGIC_MASK  0xffffffffu

/* LBL_W1: the 32-bit big-endian word at byte 4 */
/* version: bytes 4 to 5 */
/* FIELD: LBL_W1.version 31:16 established */
#define XLV_LBL_W1_VERSION_SHIFT 16
#define XLV_LBL_W1_VERSION_MASK  0xffffu
/* nparts: entries in use, byte 6 */
/* FIELD: LBL_W1.nparts 15:8 established */
#define XLV_LBL_W1_NPARTS_SHIFT 8
#define XLV_LBL_W1_NPARTS_MASK  0xffu
/* reserved: byte 7; no writer sets it */
/* FIELD: LBL_W1.reserved 7:0 unknown */
#define XLV_LBL_W1_RESERVED_SHIFT 0
#define XLV_LBL_W1_RESERVED_MASK  0xffu

/* LBL_W2: the 32-bit big-endian word at byte 8 */
/* timestamp_hi: no writer sets it; high word of a 64-bit value, bytes 8 to 11 */
/* FIELD: LBL_W2.timestamp_hi 31:0 unknown */
#define XLV_LBL_W2_TIMESTAMP_HI_SHIFT 0
#define XLV_LBL_W2_TIMESTAMP_HI_MASK  0xffffffffu

/* LBL_W3: the 32-bit big-endian word at byte 12 */
/* timestamp_lo: no writer sets it; low word of the same value, bytes 12 to 15 */
/* FIELD: LBL_W3.timestamp_lo 31:0 unknown */
#define XLV_LBL_W3_TIMESTAMP_LO_SHIFT 0
#define XLV_LBL_W3_TIMESTAMP_LO_MASK  0xffffffffu

/* LBL_W4: the 32-bit big-endian word at byte 16 */
/* trans_id_hi: configuration generation; high word of a 64-bit value, bytes 16 to 19 */
/* FIELD: LBL_W4.trans_id_hi 31:0 established */
#define XLV_LBL_W4_TRANS_ID_HI_SHIFT 0
#define XLV_LBL_W4_TRANS_ID_HI_MASK  0xffffffffu

/* LBL_W5: the 32-bit big-endian word at byte 20 */
/* trans_id_lo: configuration generation; low word of the same value, bytes 20 to 23 */
/* FIELD: LBL_W5.trans_id_lo 31:0 established */
#define XLV_LBL_W5_TRANS_ID_LO_SHIFT 0
#define XLV_LBL_W5_TRANS_ID_LO_MASK  0xffffffffu

/* ---- label entry (0x118 bytes; entry i belongs to volume-header partition i) ---- */
#define XLV_E_SIZE 0x118 /* established */
#define XLV_E_OFF_VOL_UUID 4 /* established: volume UUID, opaque bytes */
#define XLV_E_LEN_VOL_UUID 16 /* established */
#define XLV_E_OFF_LOG_SV_UUID 20 /* established: nil if the volume has no log subvolume */
#define XLV_E_LEN_LOG_SV_UUID 16 /* established */
#define XLV_E_OFF_DATA_SV_UUID 36 /* established */
#define XLV_E_LEN_DATA_SV_UUID 16 /* established */
#define XLV_E_OFF_RT_SV_UUID 52 /* established */
#define XLV_E_LEN_RT_SV_UUID 16 /* established */
#define XLV_E_OFF_VOL_NAME 68 /* established: volume name, NUL padded, every byte may be used */
#define XLV_E_LEN_VOL_NAME 32 /* established */
#define XLV_E_OFF_PLEX_UUID 120 /* established: opaque bytes */
#define XLV_E_LEN_PLEX_UUID 16 /* established */
#define XLV_E_OFF_PLEX_NAME 136 /* established */
#define XLV_E_LEN_PLEX_NAME 32 /* established */
#define XLV_E_OFF_VE_UUID 176 /* established: opaque bytes */
#define XLV_E_LEN_VE_UUID 16 /* established */
#define XLV_E_OFF_VE_NAME 192 /* established */
#define XLV_E_LEN_VE_NAME 32 /* established */
#define XLV_E_OFF_PART_UUID 248 /* established: nil means the entry is unused, opaque bytes */
#define XLV_E_LEN_PART_UUID 16 /* established */

/* E_W0: the 32-bit big-endian word at byte 0 */
/* vol_flags: bytes 0 to 3 */
/* FIELD: E_W0.vol_flags 31:0 established */
#define XLV_E_W0_VOL_FLAGS_SHIFT 0
#define XLV_E_W0_VOL_FLAGS_MASK  0xffffffffu

/* E_W25: the 32-bit big-endian word at byte 100 */
/* sv_flags_hi_no_err_retry: bit 31 of the stored subvolume flags: no error retry */
/* FIELD: E_W25.sv_flags_hi_no_err_retry 31:31 established */
#define XLV_E_W25_SV_FLAGS_HI_NO_ERR_RETRY_SHIFT 31
#define XLV_E_W25_SV_FLAGS_HI_NO_ERR_RETRY_MASK  0x1u
/* sv_flags_hi_other: bits 30:16 of the stored subvolume flags; only the upper half of the flag word is stored, meanings not recovered */
/* FIELD: E_W25.sv_flags_hi_other 30:16 unknown */
#define XLV_E_W25_SV_FLAGS_HI_OTHER_SHIFT 16
#define XLV_E_W25_SV_FLAGS_HI_OTHER_MASK  0x7fffu
/* sv_flags_hi_low: bits 15:0 of the stored subvolume flags: the low half of the flag word is not stored, so these bits are zero */
/* FIELD: E_W25.sv_flags_hi_low 15:0 established */
#define XLV_E_W25_SV_FLAGS_HI_LOW_SHIFT 0
#define XLV_E_W25_SV_FLAGS_HI_LOW_MASK  0xffffu

/* E_W27: the 32-bit big-endian word at byte 108 */
/* time: seconds, written at label write time, bytes 0x6c to 0x6f */
/* FIELD: E_W27.time 31:0 established */
#define XLV_E_W27_TIME_SHIFT 0
#define XLV_E_W27_TIME_MASK  0xffffffffu

/* E_W28: the 32-bit big-endian word at byte 112 */
/* vol_attr16: 16-bit value sign extended to 32 bits, meaning not recovered, bytes 0x70 to 0x73 */
/* FIELD: E_W28.vol_attr16 31:0 established */
#define XLV_E_W28_VOL_ATTR16_SHIFT 0
#define XLV_E_W28_VOL_ATTR16_MASK  0xffffffffu

/* E_W29: the 32-bit big-endian word at byte 116 */
/* sv_type: byte 0x74 */
/* FIELD: E_W29.sv_type 31:24 established */
#define XLV_E_W29_SV_TYPE_SHIFT 24
#define XLV_E_W29_SV_TYPE_MASK  0xffu
/* ENUM: E_W29.sv_type 1 log established */
#define XLV_E_W29_SV_TYPE_LOG 1
/* ENUM: E_W29.sv_type 2 data established */
#define XLV_E_W29_SV_TYPE_DATA 2
/* ENUM: E_W29.sv_type 3 rt established */
#define XLV_E_W29_SV_TYPE_RT 3
/* sv_depth: width established, definition inferred, bytes 0x76 to 0x77 */
/* FIELD: E_W29.sv_depth 15:0 established */
#define XLV_E_W29_SV_DEPTH_SHIFT 0
#define XLV_E_W29_SV_DEPTH_MASK  0xffffu

/* E_W42: the 32-bit big-endian word at byte 168 */
/* sv_nplex: plexes in the subvolume, at most 4, byte 0xa8 */
/* FIELD: E_W42.sv_nplex 31:24 established */
#define XLV_E_W42_SV_NPLEX_SHIFT 24
#define XLV_E_W42_SV_NPLEX_MASK  0xffu
/* plex_index: byte 0xa9 */
/* FIELD: E_W42.plex_index 23:16 established */
#define XLV_E_W42_PLEX_INDEX_SHIFT 16
#define XLV_E_W42_PLEX_INDEX_MASK  0xffu
/* plex_nve: bytes 0xaa to 0xab */
/* FIELD: E_W42.plex_nve 15:0 established */
#define XLV_E_W42_PLEX_NVE_SHIFT 0
#define XLV_E_W42_PLEX_NVE_MASK  0xffffu

/* E_W43: the 32-bit big-endian word at byte 172 */
/* plex_flags: width only; bit meanings not recovered, bytes 0xac to 0xad */
/* FIELD: E_W43.plex_flags 31:16 established */
#define XLV_E_W43_PLEX_FLAGS_SHIFT 16
#define XLV_E_W43_PLEX_FLAGS_MASK  0xffffu

/* E_W56: the 32-bit big-endian word at byte 224 */
/* ve_state: byte 0xe0 */
/* FIELD: E_W56.ve_state 31:24 established */
#define XLV_E_W56_VE_STATE_SHIFT 24
#define XLV_E_W56_VE_STATE_MASK  0xffu
/* ENUM: E_W56.ve_state 0 empty established */
#define XLV_E_W56_VE_STATE_EMPTY 0
/* ENUM: E_W56.ve_state 1 clean established */
#define XLV_E_W56_VE_STATE_CLEAN 1
/* ENUM: E_W56.ve_state 2 active established */
#define XLV_E_W56_VE_STATE_ACTIVE 2
/* ENUM: E_W56.ve_state 3 stale established */
#define XLV_E_W56_VE_STATE_STALE 3
/* ENUM: E_W56.ve_state 4 offline established */
#define XLV_E_W56_VE_STATE_OFFLINE 4
/* ENUM: E_W56.ve_state 5 incomplete established */
#define XLV_E_W56_VE_STATE_INCOMPLETE 5
/* ve_type: byte 0xe1; any other value is handled as stripe */
/* FIELD: E_W56.ve_type 23:16 established */
#define XLV_E_W56_VE_TYPE_SHIFT 16
#define XLV_E_W56_VE_TYPE_MASK  0xffu
/* ENUM: E_W56.ve_type 1 stripe established */
#define XLV_E_W56_VE_TYPE_STRIPE 1
/* ENUM: E_W56.ve_type 2 concat established */
#define XLV_E_W56_VE_TYPE_CONCAT 2
/* ve_nparts: byte 0xe2 */
/* FIELD: E_W56.ve_nparts 15:8 established */
#define XLV_E_W56_VE_NPARTS_SHIFT 8
#define XLV_E_W56_VE_NPARTS_MASK  0xffu
/* ve_index: byte 0xe3 */
/* FIELD: E_W56.ve_index 7:0 established */
#define XLV_E_W56_VE_INDEX_SHIFT 0
#define XLV_E_W56_VE_INDEX_MASK  0xffu

/* E_W57: the 32-bit big-endian word at byte 228 */
/* ve_stripe_unit: in 512-byte blocks; meaningful for stripe only, bytes 0xe4 to 0xe5 */
/* FIELD: E_W57.ve_stripe_unit 31:16 established */
#define XLV_E_W57_VE_STRIPE_UNIT_SHIFT 16
#define XLV_E_W57_VE_STRIPE_UNIT_MASK  0xffffu

/* E_W58: the 32-bit big-endian word at byte 232 */
/* ve_start_hi: first block in the subvolume, in 512-byte blocks; high word of a 64-bit value, bytes 232 to 235 */
/* FIELD: E_W58.ve_start_hi 31:0 established */
#define XLV_E_W58_VE_START_HI_SHIFT 0
#define XLV_E_W58_VE_START_HI_MASK  0xffffffffu

/* E_W59: the 32-bit big-endian word at byte 236 */
/* ve_start_lo: first block in the subvolume, in 512-byte blocks; low word of the same value, bytes 236 to 239 */
/* FIELD: E_W59.ve_start_lo 31:0 established */
#define XLV_E_W59_VE_START_LO_SHIFT 0
#define XLV_E_W59_VE_START_LO_MASK  0xffffffffu

/* E_W60: the 32-bit big-endian word at byte 240 */
/* ve_end_hi: last block, inclusive; high word of a 64-bit value, bytes 240 to 243 */
/* FIELD: E_W60.ve_end_hi 31:0 established */
#define XLV_E_W60_VE_END_HI_SHIFT 0
#define XLV_E_W60_VE_END_HI_MASK  0xffffffffu

/* E_W61: the 32-bit big-endian word at byte 244 */
/* ve_end_lo: last block, inclusive; low word of the same value, bytes 244 to 247 */
/* FIELD: E_W61.ve_end_lo 31:0 established */
#define XLV_E_W61_VE_END_LO_SHIFT 0
#define XLV_E_W61_VE_END_LO_MASK  0xffffffffu

/* E_W66: the 32-bit big-endian word at byte 264 */
/* part_dev: written as 0x10 or'd with the entry index, bytes 0x108 to 0x10b */
/* FIELD: E_W66.part_dev 31:0 established */
#define XLV_E_W66_PART_DEV_SHIFT 0
#define XLV_E_W66_PART_DEV_MASK  0xffffffffu

/* E_W67: the 32-bit big-endian word at byte 268 */
/* part_size: in 512-byte blocks, bytes 0x10c to 0x10f */
/* FIELD: E_W67.part_size 31:0 established */
#define XLV_E_W67_PART_SIZE_SHIFT 0
#define XLV_E_W67_PART_SIZE_MASK  0xffffffffu

/* E_W68: the 32-bit big-endian word at byte 272 */
/* part_index: byte 0x110 */
/* FIELD: E_W68.part_index 31:24 established */
#define XLV_E_W68_PART_INDEX_SHIFT 24
#define XLV_E_W68_PART_INDEX_MASK  0xffu

/*
 * Vectors. TESTVEC values are ones the notes record; EXAMPLE lines only show a layout.
 * TESTVEC: 0x80000000 as E_W25 => sv_flags_hi_no_err_retry=1, sv_flags_hi_other=0, sv_flags_hi_low=0
 */
/*
 * EXAMPLE (unverified): 0x02010302 as E_W56 => ve_state=2, ve_type=1, ve_nparts=3, ve_index=2
 */
#endif
