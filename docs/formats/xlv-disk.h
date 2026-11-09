/*
 * XLV on-disk label structures, derived from binaries only (see xlv.md).
 *
 * All multi-byte integers are big-endian (IRIX native order; the tools and the
 * kernel read the buffer with plain loads, no swapping). UUID fields are 16
 * opaque bytes: compare them as raw bytes, never swap. Names are fixed-width
 * byte arrays, NUL padded, and may use every byte (no terminator guaranteed).
 *
 * Field names are this document's own. Grades per field are in xlv.md.
 * Structures are laid out with natural alignment; the asserts check every
 * offset the document states.
 */
#ifndef XLV_DISK_H
#define XLV_DISK_H
#include <stddef.h>
#include <stdint.h>

#define XLV_VH_NAME        "xlvlab"   /* volume-directory entry name */
#define XLV_LABEL_MAGIC    0x11201989u
#define XLV_LABEL_VERSION  2
#define XLV_LABEL_BYTES    0x12a0     /* used bytes of one label slot */
#define XLV_MASTER_BYTES   512        /* before rounding to the sector size */
#define XLV_NPART          16         /* entries per label: one per partition */
#define XLV_NAME_LEN       32
#define XLV_NODENAME_MAX   0x102      /* bytes the writers copy into the header */

/* label-file layout, S = disk sector size in bytes (512 when the volume
 * header says 0):  master = [0, roundup(512,S)),
 * slot0 = master_size, slot1 = master_size + roundup(0x12a0,S) */

/* entry.sv_type */
enum { XLV_SV_LOG = 1, XLV_SV_DATA = 2, XLV_SV_RT = 3 };

/* entry.ve_type */
enum { XLV_VE_STRIPE = 1, XLV_VE_CONCAT = 2 };

/* entry.ve_state (names from xlv_assemble/xlv_mgr output tables) */
enum {
	XLV_VES_EMPTY = 0, XLV_VES_CLEAN = 1, XLV_VES_ACTIVE = 2,
	XLV_VES_STALE = 3, XLV_VES_OFFLINE = 4, XLV_VES_INCOMPLETE = 5,
};

/* volume-header partition types (libdisk table) */
enum {
	XLV_PT_VOLHDR = 0, XLV_PT_RAW = 3, XLV_PT_EFS = 5, XLV_PT_VOLUME = 6,
	XLV_PT_XFS = 10, XLV_PT_XFSLOG = 11, XLV_PT_XLV = 12, XLV_PT_XVM = 13,
	XLV_PT_VXVM = 14,
};

/* sv_flags_hi: only the upper 16 bits of the subvolume flag word are stored */
#define XLV_SVF_NO_ERR_RETRY 0x80000000u

/* One partition's record. 0x118 bytes. A record is unused when part_uuid is nil. */
struct xlv_label_entry {
	uint32_t vol_flags;               /* 0x000 */
	uint8_t  vol_uuid[16];            /* 0x004 */
	uint8_t  log_sv_uuid[16];         /* 0x014 nil if the volume has no log subvolume */
	uint8_t  data_sv_uuid[16];        /* 0x024 */
	uint8_t  rt_sv_uuid[16];          /* 0x034 */
	char     vol_name[XLV_NAME_LEN];  /* 0x044 */
	uint32_t sv_flags_hi;             /* 0x064 */
	uint32_t zero_068;                /* 0x068 written as 0 */
	uint32_t time;                    /* 0x06c seconds, written with time(0) */
	int32_t  vol_attr16;              /* 0x070 16-bit volume attribute, sign extended */
	uint8_t  sv_type;                 /* 0x074 */
	uint8_t  zero_075;                /* 0x075 */
	uint16_t sv_depth;                /* 0x076 */
	uint8_t  plex_uuid[16];           /* 0x078 */
	char     plex_name[XLV_NAME_LEN]; /* 0x088 */
	uint8_t  sv_nplex;                /* 0x0a8 */
	uint8_t  plex_index;              /* 0x0a9 */
	uint16_t plex_nve;                /* 0x0aa */
	uint16_t plex_flags;              /* 0x0ac */
	uint8_t  pad_0ae[2];              /* 0x0ae */
	uint8_t  ve_uuid[16];             /* 0x0b0 */
	char     ve_name[XLV_NAME_LEN];   /* 0x0c0 */
	uint8_t  ve_state;                /* 0x0e0 */
	uint8_t  ve_type;                 /* 0x0e1 */
	uint8_t  ve_nparts;               /* 0x0e2 */
	uint8_t  ve_index;                /* 0x0e3 */
	uint16_t ve_stripe_unit;          /* 0x0e4 512-byte blocks; meaningful for stripe */
	uint8_t  pad_0e6[2];              /* 0x0e6 */
	uint64_t ve_start;                /* 0x0e8 512-byte blocks in the subvolume */
	uint64_t ve_end;                  /* 0x0f0 inclusive */
	uint8_t  part_uuid[16];           /* 0x0f8 */
	uint32_t part_dev;                /* 0x108 on disk: 0x10 | partition number */
	uint32_t part_size;               /* 0x10c 512-byte blocks */
	uint8_t  part_index;              /* 0x110 */
	uint8_t  zero_111;                /* 0x111 */
	uint8_t  pad_112[6];              /* 0x112 */
};

/* One label slot. 0x12a0 bytes. */
struct xlv_label {
	uint32_t magic;                   /* 0x000 XLV_LABEL_MAGIC */
	uint16_t version;                 /* 0x004 2 */
	uint8_t  nparts;                  /* 0x006 count of entries in use */
	uint8_t  reserved;                /* 0x007 never set by the writers seen */
	uint64_t timestamp;               /* 0x008 never set by the writers seen */
	uint64_t trans_id;                /* 0x010 configuration generation */
	char     nodename[0x108];         /* 0x018 owner host; writers copy 0x102 bytes */
	struct xlv_label_entry e[XLV_NPART]; /* 0x120 entry i = partition i */
};

/* Master block: first 8 bytes only are known. */
struct xlv_master_head {
	uint64_t seq;                     /* bit 0 selects the current slot */
};

_Static_assert(sizeof(struct xlv_label_entry) == 0x118, "entry size");
_Static_assert(sizeof(struct xlv_label) == 0x12a0, "label size");
_Static_assert(offsetof(struct xlv_label, trans_id) == 0x10, "trans_id");
_Static_assert(offsetof(struct xlv_label, nodename) == 0x18, "nodename");
_Static_assert(offsetof(struct xlv_label, e) == 0x120, "entries");
_Static_assert(offsetof(struct xlv_label_entry, vol_uuid) == 0x004, "vol_uuid");
_Static_assert(offsetof(struct xlv_label_entry, log_sv_uuid) == 0x014, "log_sv_uuid");
_Static_assert(offsetof(struct xlv_label_entry, data_sv_uuid) == 0x024, "data_sv_uuid");
_Static_assert(offsetof(struct xlv_label_entry, rt_sv_uuid) == 0x034, "rt_sv_uuid");
_Static_assert(offsetof(struct xlv_label_entry, vol_name) == 0x044, "vol_name");
_Static_assert(offsetof(struct xlv_label_entry, sv_flags_hi) == 0x064, "sv_flags_hi");
_Static_assert(offsetof(struct xlv_label_entry, time) == 0x06c, "time");
_Static_assert(offsetof(struct xlv_label_entry, vol_attr16) == 0x070, "vol_attr16");
_Static_assert(offsetof(struct xlv_label_entry, sv_type) == 0x074, "sv_type");
_Static_assert(offsetof(struct xlv_label_entry, sv_depth) == 0x076, "sv_depth");
_Static_assert(offsetof(struct xlv_label_entry, plex_uuid) == 0x078, "plex_uuid");
_Static_assert(offsetof(struct xlv_label_entry, plex_name) == 0x088, "plex_name");
_Static_assert(offsetof(struct xlv_label_entry, sv_nplex) == 0x0a8, "sv_nplex");
_Static_assert(offsetof(struct xlv_label_entry, plex_nve) == 0x0aa, "plex_nve");
_Static_assert(offsetof(struct xlv_label_entry, plex_flags) == 0x0ac, "plex_flags");
_Static_assert(offsetof(struct xlv_label_entry, ve_uuid) == 0x0b0, "ve_uuid");
_Static_assert(offsetof(struct xlv_label_entry, ve_name) == 0x0c0, "ve_name");
_Static_assert(offsetof(struct xlv_label_entry, ve_state) == 0x0e0, "ve_state");
_Static_assert(offsetof(struct xlv_label_entry, ve_stripe_unit) == 0x0e4, "ve_stripe_unit");
_Static_assert(offsetof(struct xlv_label_entry, ve_start) == 0x0e8, "ve_start");
_Static_assert(offsetof(struct xlv_label_entry, ve_end) == 0x0f0, "ve_end");
_Static_assert(offsetof(struct xlv_label_entry, part_uuid) == 0x0f8, "part_uuid");
_Static_assert(offsetof(struct xlv_label_entry, part_dev) == 0x108, "part_dev");
_Static_assert(offsetof(struct xlv_label_entry, part_size) == 0x10c, "part_size");
_Static_assert(offsetof(struct xlv_label_entry, part_index) == 0x110, "part_index");

#endif
