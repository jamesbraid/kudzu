/* generated from defs/efs.toml by defs-gen - do not edit, change the source */
/* Extent File System on-disk structures */
#ifndef EFS_GEN_H
#define EFS_GEN_H

#include <stdint.h>


/* word DB_W0: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DB_W0_WIDTH                              32 /* established */
#define EFS_DB_W0_MAGIC_SHIFT                        16 /* established */
#define EFS_DB_W0_MAGIC_MASK                         0xffffu
#define EFS_DB_W0_MAGIC_INPLACE                      (EFS_DB_W0_MAGIC_MASK << EFS_DB_W0_MAGIC_SHIFT)
#define EFS_DB_W0_FIRSTUSED_SHIFT                    8 /* established */
#define EFS_DB_W0_FIRSTUSED_MASK                     0xffu
#define EFS_DB_W0_FIRSTUSED_INPLACE                  (EFS_DB_W0_FIRSTUSED_MASK << EFS_DB_W0_FIRSTUSED_SHIFT)
#define EFS_DB_W0_SLOTS_SHIFT                        0 /* established */
#define EFS_DB_W0_SLOTS_MASK                         0xffu
#define EFS_DB_W0_SLOTS_INPLACE                      (EFS_DB_W0_SLOTS_MASK << EFS_DB_W0_SLOTS_SHIFT)

/* word DEV_W8: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DEV_W8_WIDTH                             32 /* established */
#define EFS_DEV_W8_ODEV_SHIFT                        16 /* established */
#define EFS_DEV_W8_ODEV_MASK                         0xffffu
#define EFS_DEV_W8_ODEV_INPLACE                      (EFS_DEV_W8_ODEV_MASK << EFS_DEV_W8_ODEV_SHIFT)

/* word DEV_W9: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DEV_W9_WIDTH                             32 /* established */
#define EFS_DEV_W9_NDEV_SHIFT                        0 /* established */
#define EFS_DEV_W9_NDEV_MASK                         0xffffffffu
#define EFS_DEV_W9_NDEV_INPLACE                      (EFS_DEV_W9_NDEV_MASK << EFS_DEV_W9_NDEV_SHIFT)

/* word DI_W0: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W0_WIDTH                              32 /* established */
#define EFS_DI_W0_MODE_TYPE_SHIFT                    28 /* established */
#define EFS_DI_W0_MODE_TYPE_MASK                     0xfu
#define EFS_DI_W0_MODE_TYPE_INPLACE                  (EFS_DI_W0_MODE_TYPE_MASK << EFS_DI_W0_MODE_TYPE_SHIFT)
#define EFS_DI_W0_MODE_TYPE_FIFO                     1 /* established */
#define EFS_DI_W0_MODE_TYPE_CHR                      2 /* established */
#define EFS_DI_W0_MODE_TYPE_X3                       3 /* established */
#define EFS_DI_W0_MODE_TYPE_DIR                      4 /* established */
#define EFS_DI_W0_MODE_TYPE_BLK                      6 /* established */
#define EFS_DI_W0_MODE_TYPE_X7                       7 /* established */
#define EFS_DI_W0_MODE_TYPE_REG                      8 /* established */
#define EFS_DI_W0_MODE_TYPE_LNK                      10 /* established */
#define EFS_DI_W0_MODE_TYPE_SOCK                     12 /* established */
#define EFS_DI_W0_MODE_PERM_SHIFT                    16 /* established */
#define EFS_DI_W0_MODE_PERM_MASK                     0xfffu
#define EFS_DI_W0_MODE_PERM_INPLACE                  (EFS_DI_W0_MODE_PERM_MASK << EFS_DI_W0_MODE_PERM_SHIFT)
#define EFS_DI_W0_NLINK_SHIFT                        0 /* established */
#define EFS_DI_W0_NLINK_MASK                         0xffffu
#define EFS_DI_W0_NLINK_INPLACE                      (EFS_DI_W0_NLINK_MASK << EFS_DI_W0_NLINK_SHIFT)

/* word DI_W1: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W1_WIDTH                              32 /* established */
#define EFS_DI_W1_UID_SHIFT                          16 /* established */
#define EFS_DI_W1_UID_MASK                           0xffffu
#define EFS_DI_W1_UID_INPLACE                        (EFS_DI_W1_UID_MASK << EFS_DI_W1_UID_SHIFT)
#define EFS_DI_W1_GID_SHIFT                          0 /* established */
#define EFS_DI_W1_GID_MASK                           0xffffu
#define EFS_DI_W1_GID_INPLACE                        (EFS_DI_W1_GID_MASK << EFS_DI_W1_GID_SHIFT)

/* word DI_W2: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W2_WIDTH                              32 /* established */
#define EFS_DI_W2_SIZE_SHIFT                         0 /* established */
#define EFS_DI_W2_SIZE_MASK                          0xffffffffu
#define EFS_DI_W2_SIZE_INPLACE                       (EFS_DI_W2_SIZE_MASK << EFS_DI_W2_SIZE_SHIFT)

/* word DI_W3: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W3_WIDTH                              32 /* established */
#define EFS_DI_W3_ATIME_SHIFT                        0 /* established */
#define EFS_DI_W3_ATIME_MASK                         0xffffffffu
#define EFS_DI_W3_ATIME_INPLACE                      (EFS_DI_W3_ATIME_MASK << EFS_DI_W3_ATIME_SHIFT)

/* word DI_W4: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W4_WIDTH                              32 /* established */
#define EFS_DI_W4_MTIME_SHIFT                        0 /* established */
#define EFS_DI_W4_MTIME_MASK                         0xffffffffu
#define EFS_DI_W4_MTIME_INPLACE                      (EFS_DI_W4_MTIME_MASK << EFS_DI_W4_MTIME_SHIFT)

/* word DI_W5: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W5_WIDTH                              32 /* established */
#define EFS_DI_W5_CTIME_SHIFT                        0 /* established */
#define EFS_DI_W5_CTIME_MASK                         0xffffffffu
#define EFS_DI_W5_CTIME_INPLACE                      (EFS_DI_W5_CTIME_MASK << EFS_DI_W5_CTIME_SHIFT)

/* word DI_W6: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W6_WIDTH                              32 /* established */
#define EFS_DI_W6_GEN_SHIFT                          0 /* established */
#define EFS_DI_W6_GEN_MASK                           0xffffffffu
#define EFS_DI_W6_GEN_INPLACE                        (EFS_DI_W6_GEN_MASK << EFS_DI_W6_GEN_SHIFT)

/* word DI_W7: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_DI_W7_WIDTH                              32 /* inferred */
#define EFS_DI_W7_NUMEXTENTS_SHIFT                   16 /* established */
#define EFS_DI_W7_NUMEXTENTS_MASK                    0xffffu
#define EFS_DI_W7_NUMEXTENTS_INPLACE                 (EFS_DI_W7_NUMEXTENTS_MASK << EFS_DI_W7_NUMEXTENTS_SHIFT)
#define EFS_DI_W7_VERSION_SHIFT                      8 /* established */
#define EFS_DI_W7_VERSION_MASK                       0xffu
#define EFS_DI_W7_VERSION_INPLACE                    (EFS_DI_W7_VERSION_MASK << EFS_DI_W7_VERSION_SHIFT)
#define EFS_DI_W7_SPARE_SHIFT                        0 /* inferred */
#define EFS_DI_W7_SPARE_MASK                         0xffu
#define EFS_DI_W7_SPARE_INPLACE                      (EFS_DI_W7_SPARE_MASK << EFS_DI_W7_SPARE_SHIFT)

/* word EXT_W0: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_EXT_W0_WIDTH                             32 /* established */
#define EFS_EXT_W0_MAGIC_SHIFT                       24 /* established */
#define EFS_EXT_W0_MAGIC_MASK                        0xffu
#define EFS_EXT_W0_MAGIC_INPLACE                     (EFS_EXT_W0_MAGIC_MASK << EFS_EXT_W0_MAGIC_SHIFT)
#define EFS_EXT_W0_BN_SHIFT                          0 /* established */
#define EFS_EXT_W0_BN_MASK                           0xffffffu
#define EFS_EXT_W0_BN_INPLACE                        (EFS_EXT_W0_BN_MASK << EFS_EXT_W0_BN_SHIFT)

/* word EXT_W1: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_EXT_W1_WIDTH                             32 /* established */
#define EFS_EXT_W1_LENGTH_SHIFT                      24 /* established */
#define EFS_EXT_W1_LENGTH_MASK                       0xffu
#define EFS_EXT_W1_LENGTH_INPLACE                    (EFS_EXT_W1_LENGTH_MASK << EFS_EXT_W1_LENGTH_SHIFT)
#define EFS_EXT_W1_OFFSET_SHIFT                      0 /* established */
#define EFS_EXT_W1_OFFSET_MASK                       0xffffffu
#define EFS_EXT_W1_OFFSET_INPLACE                    (EFS_EXT_W1_OFFSET_MASK << EFS_EXT_W1_OFFSET_SHIFT)

/* word SB_W0: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W0_WIDTH                              32 /* established */
#define EFS_SB_W0_SIZE_SHIFT                         0 /* established */
#define EFS_SB_W0_SIZE_MASK                          0xffffffffu
#define EFS_SB_W0_SIZE_INPLACE                       (EFS_SB_W0_SIZE_MASK << EFS_SB_W0_SIZE_SHIFT)

/* word SB_W1: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W1_WIDTH                              32 /* established */
#define EFS_SB_W1_FIRSTCG_SHIFT                      0 /* established */
#define EFS_SB_W1_FIRSTCG_MASK                       0xffffffffu
#define EFS_SB_W1_FIRSTCG_INPLACE                    (EFS_SB_W1_FIRSTCG_MASK << EFS_SB_W1_FIRSTCG_SHIFT)

/* word SB_W11: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W11_WIDTH                             32 /* established */
#define EFS_SB_W11_BMSIZE_SHIFT                      0 /* established */
#define EFS_SB_W11_BMSIZE_MASK                       0xffffffffu
#define EFS_SB_W11_BMSIZE_INPLACE                    (EFS_SB_W11_BMSIZE_MASK << EFS_SB_W11_BMSIZE_SHIFT)

/* word SB_W12: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W12_WIDTH                             32 /* established */
#define EFS_SB_W12_TFREE_SHIFT                       0 /* established */
#define EFS_SB_W12_TFREE_MASK                        0xffffffffu
#define EFS_SB_W12_TFREE_INPLACE                     (EFS_SB_W12_TFREE_MASK << EFS_SB_W12_TFREE_SHIFT)

/* word SB_W13: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W13_WIDTH                             32 /* established */
#define EFS_SB_W13_TINODE_SHIFT                      0 /* established */
#define EFS_SB_W13_TINODE_MASK                       0xffffffffu
#define EFS_SB_W13_TINODE_INPLACE                    (EFS_SB_W13_TINODE_MASK << EFS_SB_W13_TINODE_SHIFT)

/* word SB_W14: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W14_WIDTH                             32 /* established */
#define EFS_SB_W14_BMBLOCK_SHIFT                     0 /* established */
#define EFS_SB_W14_BMBLOCK_MASK                      0xffffffffu
#define EFS_SB_W14_BMBLOCK_INPLACE                   (EFS_SB_W14_BMBLOCK_MASK << EFS_SB_W14_BMBLOCK_SHIFT)

/* word SB_W15: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W15_WIDTH                             32 /* established */
#define EFS_SB_W15_REPLSB_SHIFT                      0 /* established */
#define EFS_SB_W15_REPLSB_MASK                       0xffffffffu
#define EFS_SB_W15_REPLSB_INPLACE                    (EFS_SB_W15_REPLSB_MASK << EFS_SB_W15_REPLSB_SHIFT)

/* word SB_W16: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W16_WIDTH                             32 /* inferred */
#define EFS_SB_W16_LASTIALLOC_SHIFT                  0 /* inferred */
#define EFS_SB_W16_LASTIALLOC_MASK                   0xffffffffu
#define EFS_SB_W16_LASTIALLOC_INPLACE                (EFS_SB_W16_LASTIALLOC_MASK << EFS_SB_W16_LASTIALLOC_SHIFT)

/* word SB_W2: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W2_WIDTH                              32 /* established */
#define EFS_SB_W2_CGFSIZE_SHIFT                      0 /* established */
#define EFS_SB_W2_CGFSIZE_MASK                       0xffffffffu
#define EFS_SB_W2_CGFSIZE_INPLACE                    (EFS_SB_W2_CGFSIZE_MASK << EFS_SB_W2_CGFSIZE_SHIFT)

/* word SB_W22: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W22_WIDTH                             32 /* established */
#define EFS_SB_W22_CHECKSUM_SHIFT                    0 /* established */
#define EFS_SB_W22_CHECKSUM_MASK                     0xffffffffu
#define EFS_SB_W22_CHECKSUM_INPLACE                  (EFS_SB_W22_CHECKSUM_MASK << EFS_SB_W22_CHECKSUM_SHIFT)

/* word SB_W3: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W3_WIDTH                              32 /* established */
#define EFS_SB_W3_CGISIZE_SHIFT                      16 /* established */
#define EFS_SB_W3_CGISIZE_MASK                       0xffffu
#define EFS_SB_W3_CGISIZE_INPLACE                    (EFS_SB_W3_CGISIZE_MASK << EFS_SB_W3_CGISIZE_SHIFT)
#define EFS_SB_W3_SECTORS_SHIFT                      0 /* established */
#define EFS_SB_W3_SECTORS_MASK                       0xffffu
#define EFS_SB_W3_SECTORS_INPLACE                    (EFS_SB_W3_SECTORS_MASK << EFS_SB_W3_SECTORS_SHIFT)

/* word SB_W4: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W4_WIDTH                              32 /* established */
#define EFS_SB_W4_HEADS_SHIFT                        16 /* established */
#define EFS_SB_W4_HEADS_MASK                         0xffffu
#define EFS_SB_W4_HEADS_INPLACE                      (EFS_SB_W4_HEADS_MASK << EFS_SB_W4_HEADS_SHIFT)
#define EFS_SB_W4_NCG_SHIFT                          0 /* established */
#define EFS_SB_W4_NCG_MASK                           0xffffu
#define EFS_SB_W4_NCG_INPLACE                        (EFS_SB_W4_NCG_MASK << EFS_SB_W4_NCG_SHIFT)

/* word SB_W5: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W5_WIDTH                              32 /* established */
#define EFS_SB_W5_DIRTY_SHIFT                        16 /* established */
#define EFS_SB_W5_DIRTY_MASK                         0xffffu
#define EFS_SB_W5_DIRTY_INPLACE                      (EFS_SB_W5_DIRTY_MASK << EFS_SB_W5_DIRTY_SHIFT)
#define EFS_SB_W5_DIRTY_CLEAN                        0 /* established */
#define EFS_SB_W5_DIRTY_DIRTY                        2989 /* established */
#define EFS_SB_W5_DIRTY_NEEDSCK                      4660 /* established */
#define EFS_SB_W5_DIRTY_MOUNTED                      30583 /* established */

/* word SB_W6: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W6_WIDTH                              32 /* established */
#define EFS_SB_W6_TIME_SHIFT                         0 /* established */
#define EFS_SB_W6_TIME_MASK                          0xffffffffu
#define EFS_SB_W6_TIME_INPLACE                       (EFS_SB_W6_TIME_MASK << EFS_SB_W6_TIME_SHIFT)

/* word SB_W7: 32 bits; shipped IRIX binaries, per docs/formats/efs.md */
#define EFS_SB_W7_WIDTH                              32 /* established */
#define EFS_SB_W7_MAGIC_SHIFT                        0 /* established */
#define EFS_SB_W7_MAGIC_MASK                         0xffffffffu
#define EFS_SB_W7_MAGIC_INPLACE                      (EFS_SB_W7_MAGIC_MASK << EFS_SB_W7_MAGIC_SHIFT)
#define EFS_SB_W7_MAGIC_OLD                          469337 /* established */
#define EFS_SB_W7_MAGIC_NEW                          469338 /* established */

#define EFS_BBSIZE                                   0x200u /* established */
#define EFS_BITMAP_BB                                0x2u /* established */
#define EFS_DB_SIZE                                  0x200u /* established */
#define EFS_DENT_HEADER_BYTES                        0x5u /* established */
#define EFS_DEV_LONG_FORM                            0xffffu /* established */
#define EFS_DIRBLK_HEADERSIZE                        0x4u /* established */
#define EFS_DIRBLK_MAGIC                             0xbeefu /* established */
#define EFS_DIRBLK_MAXSLOTS                          0x48u /* established */
#define EFS_DIRECTEXTENTS                            0xcu /* established */
#define EFS_DI_SIZE                                  0x80u /* established */
#define EFS_EXT_SIZE                                 0x8u /* established */
#define EFS_EX_MAXLEN                                0xf8u /* established */
#define EFS_INODES_PER_BB                            0x4u /* established */
#define EFS_INODE_DATA_BYTES                         0x60u /* established */
#define EFS_INODE_SIZE                               0x80u /* established */
#define EFS_MAGIC_NEW                                0x7295au /* established */
#define EFS_MAGIC_OLD                                0x72959u /* established */
#define EFS_ROOT_INO                                 0x2u /* established */
#define EFS_SB_LEN_FNAME                             0x6u /* established */
#define EFS_SB_LEN_FPACK                             0x6u /* established */
#define EFS_SB_LEN_SPARE                             0x14u /* inferred */
#define EFS_SB_OFF_FNAME                             0x20u /* established */
#define EFS_SB_OFF_FPACK                             0x26u /* established */
#define EFS_SB_OFF_SPARE                             0x44u /* inferred */
#define EFS_SB_SIZE                                  0x5cu /* established */
#define EFS_SUPER_BB                                 0x1u /* established */

#endif /* EFS_GEN_H */
