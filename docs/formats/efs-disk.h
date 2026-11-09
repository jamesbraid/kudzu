/*
 * EFS (Extent File System) on-disk structures, derived from binaries and
 * real images only (see efs.md).
 *
 * All multi-byte integers on disk are big-endian. Every struct below is
 * laid out with explicit sizes; the static asserts pin each offset.
 * Field names follow the Linux kernel's published fs/efs driver where the
 * same field exists there; grades per field are in efs.md.
 *
 * Block numbers ("bb") are 512-byte blocks relative to the start of the
 * filesystem (the start of the type-5 partition of the volume header, or the
 * start of a bare image), never to the disk.
 */
#ifndef EFS_DISK_H
#define EFS_DISK_H
#include <stdint.h>
#include <stddef.h>

#define EFS_BBSIZE        512
#define EFS_SUPER_BB      1            /* bb 0 is the unused boot block */
#define EFS_BITMAP_BB     2            /* start of bitmap when sb.fs_bmblock == 0 */
#define EFS_OLD_MAGIC     0x00072959u  /* what every held image carries */
#define EFS_NEW_MAGIC     0x0007295au  /* bitmap and sb.size placed past the last group */
#define EFS_ROOT_INO      2            /* inodes 0 and 1 are never allocated */
#define EFS_INODE_SIZE    128
#define EFS_INODES_PER_BB 4

/* sb.fs_dirty */
#define EFS_CLEAN         0x0000       /* unmounted cleanly; the only value a normal mount accepts */
#define EFS_MOUNTED       0x7777       /* mounted read-write, was clean at mount */
#define EFS_DIRTY         0x0bad       /* mounted read-write, was dirty at mount, or corruption seen */
#define EFS_NEEDSCK       0x1234       /* unmounted after EFS_DIRTY, or made by mkfs -r */

struct efs_super {
	uint32_t fs_size;       /* 0x00 total blocks = firstcg + ncg*cgfsize (old magic) */
	uint32_t fs_firstcg;    /* 0x04 bb of the first cylinder group */
	uint32_t fs_cgfsize;    /* 0x08 blocks per cylinder group */
	uint16_t fs_cgisize;    /* 0x0c blocks of inodes at the start of each group */
	uint16_t fs_sectors;    /* 0x0e geometry hint, nonzero */
	uint16_t fs_heads;      /* 0x10 geometry hint (mkfs writes 10) */
	uint16_t fs_ncg;        /* 0x12 number of cylinder groups, nonzero */
	uint16_t fs_dirty;      /* 0x14 see EFS_* above */
	uint16_t fs_pad;        /* 0x16 zero in every image; never read */
	uint32_t fs_time;       /* 0x18 seconds; last superblock write */
	uint32_t fs_magic;      /* 0x1c */
	char     fs_fname[6];   /* 0x20 "noname" from mkfs */
	char     fs_fpack[6];   /* 0x26 "nopack" from mkfs */
	uint32_t fs_bmsize;     /* 0x2c bitmap bytes = ceil(fs_size/8) */
	uint32_t fs_tfree;      /* 0x30 free data blocks */
	uint32_t fs_tinode;     /* 0x34 free inodes (never counts inodes 0 and 1) */
	uint32_t fs_bmblock;    /* 0x38 bb of bitmap; 0 means bb 2 */
	uint32_t fs_replsb;     /* 0x3c bb of the secondary superblock; 0 = none */
	uint32_t fs_lastialloc; /* 0x40 mkfs: last inode it allocated (hint); 0 on old images */
	uint8_t  fs_spare[20];  /* 0x44 zero */
	uint32_t fs_checksum;   /* 0x58 over bytes 0..0x57, see efs_checksum() */
	/* 0x5c..0x1ff: not part of the format. The kernel writes 0xc8 bytes of its
	 * in-memory copy and zeroes the rest; mkfs leaves incidental words. Ignore on
	 * read, write zeros. */
};

/* 8 bytes. word0 = magic:8 (0) | bn:24, word1 = length:8 | offset:24 (all big-endian). */
struct efs_extent {
	uint32_t w0;
	uint32_t w1;
};
#define EFS_EX_MAGIC(e)  ((e).w0 >> 24)
#define EFS_EX_BN(e)     ((e).w0 & 0xffffffu)   /* first bb of the run */
#define EFS_EX_LEN(e)    ((e).w1 >> 24)         /* blocks, 1..248 */
#define EFS_EX_OFF(e)    ((e).w1 & 0xffffffu)   /* file block index of the first block */
#define EFS_EX_MAXLEN    248
#define EFS_DIRECTEXTENTS 12

struct efs_dinode {
	uint16_t di_mode;       /* 0x00 file type and permissions; 0 = free inode */
	int16_t  di_nlink;      /* 0x02 */
	uint16_t di_uid;        /* 0x04 */
	uint16_t di_gid;        /* 0x06 */
	uint32_t di_size;       /* 0x08 bytes; 0 for device nodes */
	uint32_t di_atime;      /* 0x0c seconds */
	uint32_t di_mtime;      /* 0x10 seconds */
	uint32_t di_ctime;      /* 0x14 seconds */
	uint32_t di_gen;        /* 0x18 generation; +1 each time the inode is freed */
	uint16_t di_numextents; /* 0x1c total extents (> 12: the list is indirect) */
	uint8_t  di_version;    /* 0x1e 0 in every image; 1 and 2 need a hook the kernel may lack */
	uint8_t  di_spare;      /* 0x1f */
	union {
		struct efs_extent di_extents[EFS_DIRECTEXTENTS];  /* 0x20.. */
		struct { uint16_t odev; uint16_t pad; uint32_t ndev; } di_dev; /* odev == 0xffff: ndev */
		uint8_t di_inline[96];  /* numextents == 0, symlink-like types, size <= 96 */
	} di_u;
};

/* di_mode & 0xf000 types the kernel accepts when reading an inode */
#define EFS_IFIFO 0x1000
#define EFS_IFCHR 0x2000
#define EFS_IFX3  0x3000   /* extent-based, may be inline; readlink accepts it */
#define EFS_IFDIR 0x4000
#define EFS_IFBLK 0x6000
#define EFS_IFX7  0x7000   /* extent-based, may be inline; readlink accepts it */
#define EFS_IFREG 0x8000
#define EFS_IFLNK 0xa000
#define EFS_IFSOCK 0xc000

/* Directory block: 512 bytes. */
#define EFS_DIRBLK_MAGIC    0xbeef
#define EFS_DIRBLK_HEADERSIZE 4
#define EFS_DIRBLK_MAXSLOTS 0x48
struct efs_dirblk {
	uint16_t magic;         /* 0xbeef */
	uint8_t  firstused;     /* offset of the lowest-addressed entry, in 2-byte units; 0 = block empty */
	uint8_t  slots;         /* number of slot bytes in use (<= 0x48) */
	uint8_t  space[508];    /* slot bytes grow up from here; entries grow down from byte 511 */
};
/* Slot byte s at block offset 4+s holds (entry offset)/2; 0 = unused slot.
 * Entry at that offset: */
struct efs_dentry {
	uint32_t ino;           /* big-endian; may sit on a 2-byte boundary only (stored as two halfwords) */
	uint8_t  namelen;       /* 1..255, not NUL terminated */
	char     name[];        /* namelen bytes, then pad so that 5+namelen is even */
};
#define EFS_DENTSIZE(namelen) (5 + (namelen) + (((namelen) + 1) & 1))

_Static_assert(sizeof(struct efs_super) == 0x5c, "efs_super");
_Static_assert(offsetof(struct efs_super, fs_dirty) == 0x14, "dirty");
_Static_assert(offsetof(struct efs_super, fs_time) == 0x18, "time");
_Static_assert(offsetof(struct efs_super, fs_magic) == 0x1c, "magic");
_Static_assert(offsetof(struct efs_super, fs_bmsize) == 0x2c, "bmsize");
_Static_assert(offsetof(struct efs_super, fs_replsb) == 0x3c, "replsb");
_Static_assert(offsetof(struct efs_super, fs_checksum) == 0x58, "checksum");
_Static_assert(sizeof(struct efs_extent) == 8, "extent is 8 bytes");
_Static_assert(sizeof(struct efs_dinode) == 128, "dinode");
_Static_assert(offsetof(struct efs_dinode, di_numextents) == 0x1c, "numextents");
_Static_assert(offsetof(struct efs_dinode, di_u) == 0x20, "di_u");
_Static_assert(sizeof(struct efs_dirblk) == 512, "dirblk");

/* Superblock checksum: 16-bit big-endian words 0..43 (bytes 0..0x57). */
static inline uint32_t efs_checksum(const uint8_t *sb)
{
	uint32_t c = 0;
	for (int i = 0; i < 0x58; i += 2) {
		c ^= (uint32_t)(sb[i] << 8 | sb[i + 1]);
		c = (c << 1) | (c >> 31);
	}
	return c;
}

/* inode number -> location */
static inline uint32_t efs_ino_bb(const struct efs_super *sb /* host order */, uint32_t ino)
{
	uint32_t per_cg = sb->fs_cgisize * EFS_INODES_PER_BB;
	return sb->fs_firstcg + (ino / per_cg) * sb->fs_cgfsize
	       + (ino / EFS_INODES_PER_BB) % sb->fs_cgisize;
}
#define efs_ino_off(ino) (((ino) % EFS_INODES_PER_BB) * EFS_INODE_SIZE)

#endif
