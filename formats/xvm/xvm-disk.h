/*
 * XVM on-disk structures, derived from binaries only (see xvm.md).
 *
 * All multi-byte integers on disk are big-endian (the IRIX native order);
 * the Linux ProPack module byte-swaps on little-endian hosts. UUID fields
 * are 16 opaque bytes and are never swapped. Every struct below is packed
 * to the sizes shown: no implicit padding is relied on.
 *
 * Grades per field are in xvm.md. Field names are the labels the shipped
 * ProPack 4 SP4 kernel module's debug info uses for the same records.
 */
#ifndef XVM_DISK_H
#define XVM_DISK_H
#include <stdint.h>

#define XVM_PHYSLAB_MAGIC   0x786c6162u /* bytes 78 6c 61 62 = "xlab" */
#define XVM_PHYSLAB_VERSION 2
#define XVM_PHYSLAB_VERSION1 1          /* legacy, upgraded on read */
#define XVM_PHYSLAB_START   1           /* DVH disks: device block 1 */
#define XVM_LABELHDR_MAGIC  0x6c646872u /* bytes 6c 64 68 72 = "ldhr" */
#define XVM_LABELHDR_VERSION 2
#define XVM_LABELHDR_VERSION1 1         /* legacy, VE sectors only */
#define XVM_LAB_ENTRY_SIZE  72
#define XVM_NAME_MAX        48
#define XVM_HOSTNAME_MAX    32
#define XVM_DISKNAME_MAX    48

/* labelhdr.lh_type */
enum { XVM_LH_FREE = 0, XVM_LH_RESERVED = 1, XVM_LH_VE = 2, XVM_LH_ATTR = 3 };

/* ve type, dve_type */
enum {
	XVM_VE_FREE = 0, XVM_VE_SLICE = 1, XVM_VE_CONCAT = 2, XVM_VE_STRIPE = 3,
	XVM_VE_RAID = 4, XVM_VE_MIRROR = 5, XVM_VE_SUBVOL = 6, XVM_VE_VOL = 7,
	XVM_VE_COW = 11, XVM_VE_SNAPSHOT = 12,
};

/* attribute type, da_type */
enum { XVM_ATTR_FREE = 0, XVM_ATTR_NAME = 1, XVM_ATTR_PERM = 2 };

/* subvolume type, vsv_svtype */
enum {
	XVM_SV_ALL = 0, XVM_SV_LOG = 1, XVM_SV_DATA = 2, XVM_SV_RT = 3,
	XVM_SV_SWAP = 4, XVM_SV_USER1 = 16,
};

/* physlab.pl_state */
#define XVM_PVS_ONLINE     0x01
#define XVM_PVS_SPARE      0x02
#define XVM_PVS_MEDIAERROR 0x04
#define XVM_PVS_RECOVERING 0x08
#define XVM_PVS_PRIVATE    0x10
#define XVM_PVS_CLUSTER    0x20
#define XVM_PVS_DRAINING   0x40

/* physlab.pl_flags */
#define XVM_PVF_BOOTABLE   0x02
#define XVM_PVF_DUPLICATE  0x04
#define XVM_PVF_SYNC       0x08

/* dve_state */
#define XVM_VES_CLEAN      0x01
#define XVM_VES_VALID      0x02
#define XVM_VES_DISABLED   0x10

/* vm_policy */
enum { XVM_MR_ROUND_ROBIN = 0, XVM_MR_SEQUENTIAL = 1 };

struct xvm_physlab {          /* 124 bytes at the physlab sector, offset 0 */
	uint32_t pl_magic;        /*   0 */
	uint16_t pl_version;      /*   4 */
	uint8_t  pl_state;        /*   6 */
	uint8_t  pl_flags;        /*   7 */
	char     pl_hostname[32]; /*   8 (version 1: disk name lived here) */
	uint8_t  pl_uuid[16];     /*  40 */
	uint16_t pl_secsize;      /*  56 units of 512 bytes */
	uint16_t pl_xvmlabel;     /*  58 first label block (== volume header size) */
	uint16_t pl_xvmlabelblks; /*  60 label area length in sectors */
	uint16_t pl_dbn;          /*  62 first data block: start of the user area */
	uint32_t pl_nblks;        /*  64 */
	uint32_t pl_timestamp;    /*  68 */
	char     pl_diskname[48]; /*  72 */
	uint32_t pl_clusterid;    /* 120 */
};

struct xvm_labelhdr {         /* 8 bytes at the start of every label sector */
	uint32_t lh_magic;        /* 0 */
	uint16_t lh_version;      /* 4 */
	uint8_t  lh_type;         /* 6 */
	uint8_t  lh_size;         /* 7: entry size, must be 72 */
};

struct xvm_disk_ve {          /* 72 bytes; one record per piece (child edge) */
	uint8_t  dve_type;        /*  0 */
	uint8_t  dve_state;       /*  1 */
	uint16_t dve_pieces;      /*  2 number of pieces of this object */
	uint32_t dve_tid;         /*  4 */
	uint8_t  dve_uuid[16];    /*  8 this object */
	uint8_t  dve_childuuid[16]; /* 24 the child at dve_pos */
	uint64_t dve_length;      /* 40 */
	uint16_t dve_pos;         /* 48 */
	uint16_t dve_cowfirst;    /* 50 */
	uint16_t dve_cownext;     /* 52 */
	uint16_t dve_cowregsz;    /* 54 */
	union {                   /* 56, 16 bytes */
		struct { uint64_t startblk; uint16_t seq; uint8_t flags;
			 uint8_t partition; uint8_t pad[4]; } slice;
		struct { uint32_t susize; uint8_t pad[12]; } stripe;
		struct { uint8_t pad[16]; } concat;
		struct { uint64_t oldlength; uint16_t logfreq; uint8_t policy;
			 uint8_t config; uint8_t pad[4]; } mirror;
		struct { uint32_t susize; uint8_t layout; uint8_t pad[11]; } raid;
		struct { uint8_t ruuid[16]; } cow;
		struct { uint8_t svtype; uint8_t pad[15]; } subvol;
		struct { uint8_t repfull; uint8_t pad[15]; } vol;
	} u;
};

struct xvm_disk_attr {        /* 72 bytes */
	uint8_t  da_type;         /*  0 */
	uint8_t  da_flag;         /*  1 */
	uint8_t  da_pad[2];       /*  2 */
	uint32_t da_tid;          /*  4 */
	uint8_t  da_uuid[16];     /*  8 the object this names */
	union {                   /* 24, 48 bytes */
		char name[48];
		struct { int32_t uid; int32_t gid; uint32_t mode; uint8_t pad[36]; } perm;
	} u;
};

_Static_assert(sizeof(struct xvm_physlab) == 124, "physlab");
_Static_assert(sizeof(struct xvm_labelhdr) == 8, "labelhdr");
_Static_assert(sizeof(struct xvm_disk_ve) == 72, "ve");
_Static_assert(sizeof(struct xvm_disk_attr) == 72, "attr");
#endif
