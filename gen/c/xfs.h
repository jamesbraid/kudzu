/* generated from defs/xfs.toml by defs-gen - do not edit, change the source */
/* XFS superblock and inode core as the shipped release writes them */
#ifndef XFS_GEN_H
#define XFS_GEN_H

#include <stdint.h>


/* word INO_W0: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W0_WIDTH                             32 /* established */
#define XFS_INO_W0_MAGIC_SHIFT                       16 /* established */
#define XFS_INO_W0_MAGIC_MASK                        0xffffu
#define XFS_INO_W0_MAGIC_INPLACE                     (XFS_INO_W0_MAGIC_MASK << XFS_INO_W0_MAGIC_SHIFT)
#define XFS_INO_W0_MODE_SHIFT                        0 /* established */
#define XFS_INO_W0_MODE_MASK                         0xffffu
#define XFS_INO_W0_MODE_INPLACE                      (XFS_INO_W0_MODE_MASK << XFS_INO_W0_MODE_SHIFT)

/* word INO_W1: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W1_WIDTH                             32 /* established */
#define XFS_INO_W1_VERSION_SHIFT                     24 /* established */
#define XFS_INO_W1_VERSION_MASK                      0xffu
#define XFS_INO_W1_VERSION_INPLACE                   (XFS_INO_W1_VERSION_MASK << XFS_INO_W1_VERSION_SHIFT)
#define XFS_INO_W1_FORMAT_SHIFT                      16 /* established */
#define XFS_INO_W1_FORMAT_MASK                       0xffu
#define XFS_INO_W1_FORMAT_INPLACE                    (XFS_INO_W1_FORMAT_MASK << XFS_INO_W1_FORMAT_SHIFT)
#define XFS_INO_W1_FORMAT_DEV                        0 /* established */
#define XFS_INO_W1_FORMAT_LOCAL                      1 /* established */
#define XFS_INO_W1_FORMAT_EXTENTS                    2 /* established */
#define XFS_INO_W1_FORMAT_BTREE                      3 /* established */
#define XFS_INO_W1_FORMAT_UUID                       4 /* established */
#define XFS_INO_W1_ONLINK_SHIFT                      0 /* established */
#define XFS_INO_W1_ONLINK_MASK                       0xffffu
#define XFS_INO_W1_ONLINK_INPLACE                    (XFS_INO_W1_ONLINK_MASK << XFS_INO_W1_ONLINK_SHIFT)

/* word INO_W10: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W10_WIDTH                            32 /* established */
#define XFS_INO_W10_MTIME_SEC_SHIFT                  0 /* established */
#define XFS_INO_W10_MTIME_SEC_MASK                   0xffffffffu
#define XFS_INO_W10_MTIME_SEC_INPLACE                (XFS_INO_W10_MTIME_SEC_MASK << XFS_INO_W10_MTIME_SEC_SHIFT)

/* word INO_W11: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W11_WIDTH                            32 /* established */
#define XFS_INO_W11_MTIME_NSEC_SHIFT                 0 /* established */
#define XFS_INO_W11_MTIME_NSEC_MASK                  0xffffffffu
#define XFS_INO_W11_MTIME_NSEC_INPLACE               (XFS_INO_W11_MTIME_NSEC_MASK << XFS_INO_W11_MTIME_NSEC_SHIFT)

/* word INO_W12: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W12_WIDTH                            32 /* established */
#define XFS_INO_W12_CTIME_SEC_SHIFT                  0 /* established */
#define XFS_INO_W12_CTIME_SEC_MASK                   0xffffffffu
#define XFS_INO_W12_CTIME_SEC_INPLACE                (XFS_INO_W12_CTIME_SEC_MASK << XFS_INO_W12_CTIME_SEC_SHIFT)

/* word INO_W13: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W13_WIDTH                            32 /* established */
#define XFS_INO_W13_CTIME_NSEC_SHIFT                 0 /* established */
#define XFS_INO_W13_CTIME_NSEC_MASK                  0xffffffffu
#define XFS_INO_W13_CTIME_NSEC_INPLACE               (XFS_INO_W13_CTIME_NSEC_MASK << XFS_INO_W13_CTIME_NSEC_SHIFT)

/* word INO_W14: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W14_WIDTH                            32 /* established */
#define XFS_INO_W14_SIZE_HI_SHIFT                    0 /* established */
#define XFS_INO_W14_SIZE_HI_MASK                     0xffffffffu
#define XFS_INO_W14_SIZE_HI_INPLACE                  (XFS_INO_W14_SIZE_HI_MASK << XFS_INO_W14_SIZE_HI_SHIFT)

/* word INO_W15: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W15_WIDTH                            32 /* established */
#define XFS_INO_W15_SIZE_LO_SHIFT                    0 /* established */
#define XFS_INO_W15_SIZE_LO_MASK                     0xffffffffu
#define XFS_INO_W15_SIZE_LO_INPLACE                  (XFS_INO_W15_SIZE_LO_MASK << XFS_INO_W15_SIZE_LO_SHIFT)

/* word INO_W16: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W16_WIDTH                            32 /* established */
#define XFS_INO_W16_NBLOCKS_HI_SHIFT                 0 /* established */
#define XFS_INO_W16_NBLOCKS_HI_MASK                  0xffffffffu
#define XFS_INO_W16_NBLOCKS_HI_INPLACE               (XFS_INO_W16_NBLOCKS_HI_MASK << XFS_INO_W16_NBLOCKS_HI_SHIFT)

/* word INO_W17: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W17_WIDTH                            32 /* established */
#define XFS_INO_W17_NBLOCKS_LO_SHIFT                 0 /* established */
#define XFS_INO_W17_NBLOCKS_LO_MASK                  0xffffffffu
#define XFS_INO_W17_NBLOCKS_LO_INPLACE               (XFS_INO_W17_NBLOCKS_LO_MASK << XFS_INO_W17_NBLOCKS_LO_SHIFT)

/* word INO_W18: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W18_WIDTH                            32 /* established */
#define XFS_INO_W18_EXTSIZE_SHIFT                    0 /* established */
#define XFS_INO_W18_EXTSIZE_MASK                     0xffffffffu
#define XFS_INO_W18_EXTSIZE_INPLACE                  (XFS_INO_W18_EXTSIZE_MASK << XFS_INO_W18_EXTSIZE_SHIFT)

/* word INO_W19: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W19_WIDTH                            32 /* established */
#define XFS_INO_W19_NEXTENTS_SHIFT                   0 /* established */
#define XFS_INO_W19_NEXTENTS_MASK                    0xffffffffu
#define XFS_INO_W19_NEXTENTS_INPLACE                 (XFS_INO_W19_NEXTENTS_MASK << XFS_INO_W19_NEXTENTS_SHIFT)

/* word INO_W2: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W2_WIDTH                             32 /* established */
#define XFS_INO_W2_UID_SHIFT                         0 /* established */
#define XFS_INO_W2_UID_MASK                          0xffffffffu
#define XFS_INO_W2_UID_INPLACE                       (XFS_INO_W2_UID_MASK << XFS_INO_W2_UID_SHIFT)

/* word INO_W20: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W20_WIDTH                            32 /* established */
#define XFS_INO_W20_NAEXTENTS_SHIFT                  16 /* established */
#define XFS_INO_W20_NAEXTENTS_MASK                   0xffffu
#define XFS_INO_W20_NAEXTENTS_INPLACE                (XFS_INO_W20_NAEXTENTS_MASK << XFS_INO_W20_NAEXTENTS_SHIFT)
#define XFS_INO_W20_FORKOFF_SHIFT                    8 /* established */
#define XFS_INO_W20_FORKOFF_MASK                     0xffu
#define XFS_INO_W20_FORKOFF_INPLACE                  (XFS_INO_W20_FORKOFF_MASK << XFS_INO_W20_FORKOFF_SHIFT)
#define XFS_INO_W20_AFORMAT_SHIFT                    0 /* established */
#define XFS_INO_W20_AFORMAT_MASK                     0xffu
#define XFS_INO_W20_AFORMAT_INPLACE                  (XFS_INO_W20_AFORMAT_MASK << XFS_INO_W20_AFORMAT_SHIFT)
#define XFS_INO_W20_AFORMAT_LOCAL                    1 /* established */
#define XFS_INO_W20_AFORMAT_EXTENTS                  2 /* established */
#define XFS_INO_W20_AFORMAT_BTREE                    3 /* established */

/* word INO_W21: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W21_WIDTH                            32 /* established */
#define XFS_INO_W21_DMEVMASK_SHIFT                   0 /* established */
#define XFS_INO_W21_DMEVMASK_MASK                    0xffffffffu
#define XFS_INO_W21_DMEVMASK_INPLACE                 (XFS_INO_W21_DMEVMASK_MASK << XFS_INO_W21_DMEVMASK_SHIFT)

/* word INO_W22: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W22_WIDTH                            32 /* established */
#define XFS_INO_W22_DMSTATE_SHIFT                    16 /* established */
#define XFS_INO_W22_DMSTATE_MASK                     0xffffu
#define XFS_INO_W22_DMSTATE_INPLACE                  (XFS_INO_W22_DMSTATE_MASK << XFS_INO_W22_DMSTATE_SHIFT)
#define XFS_INO_W22_FLAGS_RTINHERIT_SHIFT            8 /* established */
#define XFS_INO_W22_FLAGS_RTINHERIT_MASK             0x1u
#define XFS_INO_W22_FLAGS_RTINHERIT_INPLACE          (XFS_INO_W22_FLAGS_RTINHERIT_MASK << XFS_INO_W22_FLAGS_RTINHERIT_SHIFT)
#define XFS_INO_W22_FLAGS_NODUMP_SHIFT               7 /* established */
#define XFS_INO_W22_FLAGS_NODUMP_MASK                0x1u
#define XFS_INO_W22_FLAGS_NODUMP_INPLACE             (XFS_INO_W22_FLAGS_NODUMP_MASK << XFS_INO_W22_FLAGS_NODUMP_SHIFT)
#define XFS_INO_W22_FLAGS_NOATIME_SHIFT              6 /* established */
#define XFS_INO_W22_FLAGS_NOATIME_MASK               0x1u
#define XFS_INO_W22_FLAGS_NOATIME_INPLACE            (XFS_INO_W22_FLAGS_NOATIME_MASK << XFS_INO_W22_FLAGS_NOATIME_SHIFT)
#define XFS_INO_W22_FLAGS_SYNC_SHIFT                 5 /* established */
#define XFS_INO_W22_FLAGS_SYNC_MASK                  0x1u
#define XFS_INO_W22_FLAGS_SYNC_INPLACE               (XFS_INO_W22_FLAGS_SYNC_MASK << XFS_INO_W22_FLAGS_SYNC_SHIFT)
#define XFS_INO_W22_FLAGS_APPEND_SHIFT               4 /* established */
#define XFS_INO_W22_FLAGS_APPEND_MASK                0x1u
#define XFS_INO_W22_FLAGS_APPEND_INPLACE             (XFS_INO_W22_FLAGS_APPEND_MASK << XFS_INO_W22_FLAGS_APPEND_SHIFT)
#define XFS_INO_W22_FLAGS_IMMUTABLE_SHIFT            3 /* established */
#define XFS_INO_W22_FLAGS_IMMUTABLE_MASK             0x1u
#define XFS_INO_W22_FLAGS_IMMUTABLE_INPLACE          (XFS_INO_W22_FLAGS_IMMUTABLE_MASK << XFS_INO_W22_FLAGS_IMMUTABLE_SHIFT)
#define XFS_INO_W22_FLAGS_NEWRTBM_SHIFT              2 /* established */
#define XFS_INO_W22_FLAGS_NEWRTBM_MASK               0x1u
#define XFS_INO_W22_FLAGS_NEWRTBM_INPLACE            (XFS_INO_W22_FLAGS_NEWRTBM_MASK << XFS_INO_W22_FLAGS_NEWRTBM_SHIFT)
#define XFS_INO_W22_FLAGS_PREALLOC_SHIFT             1 /* established */
#define XFS_INO_W22_FLAGS_PREALLOC_MASK              0x1u
#define XFS_INO_W22_FLAGS_PREALLOC_INPLACE           (XFS_INO_W22_FLAGS_PREALLOC_MASK << XFS_INO_W22_FLAGS_PREALLOC_SHIFT)
#define XFS_INO_W22_FLAGS_REALTIME_SHIFT             0 /* established */
#define XFS_INO_W22_FLAGS_REALTIME_MASK              0x1u
#define XFS_INO_W22_FLAGS_REALTIME_INPLACE           (XFS_INO_W22_FLAGS_REALTIME_MASK << XFS_INO_W22_FLAGS_REALTIME_SHIFT)

/* word INO_W23: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W23_WIDTH                            32 /* established */
#define XFS_INO_W23_GEN_SHIFT                        0 /* established */
#define XFS_INO_W23_GEN_MASK                         0xffffffffu
#define XFS_INO_W23_GEN_INPLACE                      (XFS_INO_W23_GEN_MASK << XFS_INO_W23_GEN_SHIFT)

/* word INO_W24: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W24_WIDTH                            32 /* established */
#define XFS_INO_W24_NEXT_UNLINKED_SHIFT              0 /* established */
#define XFS_INO_W24_NEXT_UNLINKED_MASK               0xffffffffu
#define XFS_INO_W24_NEXT_UNLINKED_INPLACE            (XFS_INO_W24_NEXT_UNLINKED_MASK << XFS_INO_W24_NEXT_UNLINKED_SHIFT)

/* word INO_W3: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W3_WIDTH                             32 /* established */
#define XFS_INO_W3_GID_SHIFT                         0 /* established */
#define XFS_INO_W3_GID_MASK                          0xffffffffu
#define XFS_INO_W3_GID_INPLACE                       (XFS_INO_W3_GID_MASK << XFS_INO_W3_GID_SHIFT)

/* word INO_W4: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W4_WIDTH                             32 /* established */
#define XFS_INO_W4_NLINKV2_SHIFT                     0 /* established */
#define XFS_INO_W4_NLINKV2_MASK                      0xffffffffu
#define XFS_INO_W4_NLINKV2_INPLACE                   (XFS_INO_W4_NLINKV2_MASK << XFS_INO_W4_NLINKV2_SHIFT)

/* word INO_W5: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W5_WIDTH                             32 /* established */
#define XFS_INO_W5_PROJID_SHIFT                      16 /* established */
#define XFS_INO_W5_PROJID_MASK                       0xffffu
#define XFS_INO_W5_PROJID_INPLACE                    (XFS_INO_W5_PROJID_MASK << XFS_INO_W5_PROJID_SHIFT)

/* word INO_W7: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W7_WIDTH                             32 /* established */
#define XFS_INO_W7_FLUSHITER_SHIFT                   0 /* established */
#define XFS_INO_W7_FLUSHITER_MASK                    0xffffu
#define XFS_INO_W7_FLUSHITER_INPLACE                 (XFS_INO_W7_FLUSHITER_MASK << XFS_INO_W7_FLUSHITER_SHIFT)

/* word INO_W8: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W8_WIDTH                             32 /* established */
#define XFS_INO_W8_ATIME_SEC_SHIFT                   0 /* established */
#define XFS_INO_W8_ATIME_SEC_MASK                    0xffffffffu
#define XFS_INO_W8_ATIME_SEC_INPLACE                 (XFS_INO_W8_ATIME_SEC_MASK << XFS_INO_W8_ATIME_SEC_SHIFT)

/* word INO_W9: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_INO_W9_WIDTH                             32 /* established */
#define XFS_INO_W9_ATIME_NSEC_SHIFT                  0 /* established */
#define XFS_INO_W9_ATIME_NSEC_MASK                   0xffffffffu
#define XFS_INO_W9_ATIME_NSEC_INPLACE                (XFS_INO_W9_ATIME_NSEC_MASK << XFS_INO_W9_ATIME_NSEC_SHIFT)

/* word SB_W0: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W0_WIDTH                              32 /* established */
#define XFS_SB_W0_MAGICNUM_SHIFT                     0 /* established */
#define XFS_SB_W0_MAGICNUM_MASK                      0xffffffffu
#define XFS_SB_W0_MAGICNUM_INPLACE                   (XFS_SB_W0_MAGICNUM_MASK << XFS_SB_W0_MAGICNUM_SHIFT)

/* word SB_W1: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W1_WIDTH                              32 /* established */
#define XFS_SB_W1_BLOCKSIZE_SHIFT                    0 /* established */
#define XFS_SB_W1_BLOCKSIZE_MASK                     0xffffffffu
#define XFS_SB_W1_BLOCKSIZE_INPLACE                  (XFS_SB_W1_BLOCKSIZE_MASK << XFS_SB_W1_BLOCKSIZE_SHIFT)

/* word SB_W12: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W12_WIDTH                             32 /* established */
#define XFS_SB_W12_LOGSTART_HI_SHIFT                 0 /* established */
#define XFS_SB_W12_LOGSTART_HI_MASK                  0xffffffffu
#define XFS_SB_W12_LOGSTART_HI_INPLACE               (XFS_SB_W12_LOGSTART_HI_MASK << XFS_SB_W12_LOGSTART_HI_SHIFT)

/* word SB_W13: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W13_WIDTH                             32 /* established */
#define XFS_SB_W13_LOGSTART_LO_SHIFT                 0 /* established */
#define XFS_SB_W13_LOGSTART_LO_MASK                  0xffffffffu
#define XFS_SB_W13_LOGSTART_LO_INPLACE               (XFS_SB_W13_LOGSTART_LO_MASK << XFS_SB_W13_LOGSTART_LO_SHIFT)

/* word SB_W14: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W14_WIDTH                             32 /* established */
#define XFS_SB_W14_ROOTINO_HI_SHIFT                  0 /* established */
#define XFS_SB_W14_ROOTINO_HI_MASK                   0xffffffffu
#define XFS_SB_W14_ROOTINO_HI_INPLACE                (XFS_SB_W14_ROOTINO_HI_MASK << XFS_SB_W14_ROOTINO_HI_SHIFT)

/* word SB_W15: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W15_WIDTH                             32 /* established */
#define XFS_SB_W15_ROOTINO_LO_SHIFT                  0 /* established */
#define XFS_SB_W15_ROOTINO_LO_MASK                   0xffffffffu
#define XFS_SB_W15_ROOTINO_LO_INPLACE                (XFS_SB_W15_ROOTINO_LO_MASK << XFS_SB_W15_ROOTINO_LO_SHIFT)

/* word SB_W16: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W16_WIDTH                             32 /* established */
#define XFS_SB_W16_RBMINO_HI_SHIFT                   0 /* established */
#define XFS_SB_W16_RBMINO_HI_MASK                    0xffffffffu
#define XFS_SB_W16_RBMINO_HI_INPLACE                 (XFS_SB_W16_RBMINO_HI_MASK << XFS_SB_W16_RBMINO_HI_SHIFT)

/* word SB_W17: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W17_WIDTH                             32 /* established */
#define XFS_SB_W17_RBMINO_LO_SHIFT                   0 /* established */
#define XFS_SB_W17_RBMINO_LO_MASK                    0xffffffffu
#define XFS_SB_W17_RBMINO_LO_INPLACE                 (XFS_SB_W17_RBMINO_LO_MASK << XFS_SB_W17_RBMINO_LO_SHIFT)

/* word SB_W18: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W18_WIDTH                             32 /* established */
#define XFS_SB_W18_RSUMINO_HI_SHIFT                  0 /* established */
#define XFS_SB_W18_RSUMINO_HI_MASK                   0xffffffffu
#define XFS_SB_W18_RSUMINO_HI_INPLACE                (XFS_SB_W18_RSUMINO_HI_MASK << XFS_SB_W18_RSUMINO_HI_SHIFT)

/* word SB_W19: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W19_WIDTH                             32 /* established */
#define XFS_SB_W19_RSUMINO_LO_SHIFT                  0 /* established */
#define XFS_SB_W19_RSUMINO_LO_MASK                   0xffffffffu
#define XFS_SB_W19_RSUMINO_LO_INPLACE                (XFS_SB_W19_RSUMINO_LO_MASK << XFS_SB_W19_RSUMINO_LO_SHIFT)

/* word SB_W2: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W2_WIDTH                              32 /* established */
#define XFS_SB_W2_DBLOCKS_HI_SHIFT                   0 /* established */
#define XFS_SB_W2_DBLOCKS_HI_MASK                    0xffffffffu
#define XFS_SB_W2_DBLOCKS_HI_INPLACE                 (XFS_SB_W2_DBLOCKS_HI_MASK << XFS_SB_W2_DBLOCKS_HI_SHIFT)

/* word SB_W20: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W20_WIDTH                             32 /* established */
#define XFS_SB_W20_REXTSIZE_SHIFT                    0 /* established */
#define XFS_SB_W20_REXTSIZE_MASK                     0xffffffffu
#define XFS_SB_W20_REXTSIZE_INPLACE                  (XFS_SB_W20_REXTSIZE_MASK << XFS_SB_W20_REXTSIZE_SHIFT)

/* word SB_W21: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W21_WIDTH                             32 /* established */
#define XFS_SB_W21_AGBLOCKS_SHIFT                    0 /* established */
#define XFS_SB_W21_AGBLOCKS_MASK                     0xffffffffu
#define XFS_SB_W21_AGBLOCKS_INPLACE                  (XFS_SB_W21_AGBLOCKS_MASK << XFS_SB_W21_AGBLOCKS_SHIFT)

/* word SB_W22: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W22_WIDTH                             32 /* established */
#define XFS_SB_W22_AGCOUNT_SHIFT                     0 /* established */
#define XFS_SB_W22_AGCOUNT_MASK                      0xffffffffu
#define XFS_SB_W22_AGCOUNT_INPLACE                   (XFS_SB_W22_AGCOUNT_MASK << XFS_SB_W22_AGCOUNT_SHIFT)

/* word SB_W23: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W23_WIDTH                             32 /* established */
#define XFS_SB_W23_RBMBLOCKS_SHIFT                   0 /* established */
#define XFS_SB_W23_RBMBLOCKS_MASK                    0xffffffffu
#define XFS_SB_W23_RBMBLOCKS_INPLACE                 (XFS_SB_W23_RBMBLOCKS_MASK << XFS_SB_W23_RBMBLOCKS_SHIFT)

/* word SB_W24: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W24_WIDTH                             32 /* established */
#define XFS_SB_W24_LOGBLOCKS_SHIFT                   0 /* established */
#define XFS_SB_W24_LOGBLOCKS_MASK                    0xffffffffu
#define XFS_SB_W24_LOGBLOCKS_INPLACE                 (XFS_SB_W24_LOGBLOCKS_MASK << XFS_SB_W24_LOGBLOCKS_SHIFT)

/* word SB_W25: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W25_WIDTH                             32 /* inferred */
#define XFS_SB_W25_VERSIONNUM_MOREBITS_SHIFT         31 /* established */
#define XFS_SB_W25_VERSIONNUM_MOREBITS_MASK          0x1u
#define XFS_SB_W25_VERSIONNUM_MOREBITS_INPLACE       (XFS_SB_W25_VERSIONNUM_MOREBITS_MASK << XFS_SB_W25_VERSIONNUM_MOREBITS_SHIFT)
#define XFS_SB_W25_VERSIONNUM_CI_DIRS_SHIFT          30 /* inferred */
#define XFS_SB_W25_VERSIONNUM_CI_DIRS_MASK           0x1u
#define XFS_SB_W25_VERSIONNUM_CI_DIRS_INPLACE        (XFS_SB_W25_VERSIONNUM_CI_DIRS_MASK << XFS_SB_W25_VERSIONNUM_CI_DIRS_SHIFT)
#define XFS_SB_W25_VERSIONNUM_DIRV2_SHIFT            29 /* established */
#define XFS_SB_W25_VERSIONNUM_DIRV2_MASK             0x1u
#define XFS_SB_W25_VERSIONNUM_DIRV2_INPLACE          (XFS_SB_W25_VERSIONNUM_DIRV2_MASK << XFS_SB_W25_VERSIONNUM_DIRV2_SHIFT)
#define XFS_SB_W25_VERSIONNUM_EXTFLG_SHIFT           28 /* established */
#define XFS_SB_W25_VERSIONNUM_EXTFLG_MASK            0x1u
#define XFS_SB_W25_VERSIONNUM_EXTFLG_INPLACE         (XFS_SB_W25_VERSIONNUM_EXTFLG_MASK << XFS_SB_W25_VERSIONNUM_EXTFLG_SHIFT)
#define XFS_SB_W25_VERSIONNUM_REJECTED_SHIFT         27 /* established */
#define XFS_SB_W25_VERSIONNUM_REJECTED_MASK          0x1u
#define XFS_SB_W25_VERSIONNUM_REJECTED_INPLACE       (XFS_SB_W25_VERSIONNUM_REJECTED_MASK << XFS_SB_W25_VERSIONNUM_REJECTED_SHIFT)
#define XFS_SB_W25_VERSIONNUM_LOGV2_SHIFT            26 /* established */
#define XFS_SB_W25_VERSIONNUM_LOGV2_MASK             0x1u
#define XFS_SB_W25_VERSIONNUM_LOGV2_INPLACE          (XFS_SB_W25_VERSIONNUM_LOGV2_MASK << XFS_SB_W25_VERSIONNUM_LOGV2_SHIFT)
#define XFS_SB_W25_VERSIONNUM_SHARED_SHIFT           25 /* established */
#define XFS_SB_W25_VERSIONNUM_SHARED_MASK            0x1u
#define XFS_SB_W25_VERSIONNUM_SHARED_INPLACE         (XFS_SB_W25_VERSIONNUM_SHARED_MASK << XFS_SB_W25_VERSIONNUM_SHARED_SHIFT)
#define XFS_SB_W25_VERSIONNUM_DALIGN_SHIFT           24 /* established */
#define XFS_SB_W25_VERSIONNUM_DALIGN_MASK            0x1u
#define XFS_SB_W25_VERSIONNUM_DALIGN_INPLACE         (XFS_SB_W25_VERSIONNUM_DALIGN_MASK << XFS_SB_W25_VERSIONNUM_DALIGN_SHIFT)
#define XFS_SB_W25_VERSIONNUM_ALIGN_SHIFT            23 /* established */
#define XFS_SB_W25_VERSIONNUM_ALIGN_MASK             0x1u
#define XFS_SB_W25_VERSIONNUM_ALIGN_INPLACE          (XFS_SB_W25_VERSIONNUM_ALIGN_MASK << XFS_SB_W25_VERSIONNUM_ALIGN_SHIFT)
#define XFS_SB_W25_VERSIONNUM_QUOTA_SHIFT            22 /* established */
#define XFS_SB_W25_VERSIONNUM_QUOTA_MASK             0x1u
#define XFS_SB_W25_VERSIONNUM_QUOTA_INPLACE          (XFS_SB_W25_VERSIONNUM_QUOTA_MASK << XFS_SB_W25_VERSIONNUM_QUOTA_SHIFT)
#define XFS_SB_W25_VERSIONNUM_NLINK_SHIFT            21 /* established */
#define XFS_SB_W25_VERSIONNUM_NLINK_MASK             0x1u
#define XFS_SB_W25_VERSIONNUM_NLINK_INPLACE          (XFS_SB_W25_VERSIONNUM_NLINK_MASK << XFS_SB_W25_VERSIONNUM_NLINK_SHIFT)
#define XFS_SB_W25_VERSIONNUM_ATTR_SHIFT             20 /* established */
#define XFS_SB_W25_VERSIONNUM_ATTR_MASK              0x1u
#define XFS_SB_W25_VERSIONNUM_ATTR_INPLACE           (XFS_SB_W25_VERSIONNUM_ATTR_MASK << XFS_SB_W25_VERSIONNUM_ATTR_SHIFT)
#define XFS_SB_W25_VERSIONNUM_VERS_SHIFT             16 /* established */
#define XFS_SB_W25_VERSIONNUM_VERS_MASK              0xfu
#define XFS_SB_W25_VERSIONNUM_VERS_INPLACE           (XFS_SB_W25_VERSIONNUM_VERS_MASK << XFS_SB_W25_VERSIONNUM_VERS_SHIFT)
#define XFS_SB_W25_SECTSIZE_SHIFT                    0 /* established */
#define XFS_SB_W25_SECTSIZE_MASK                     0xffffu
#define XFS_SB_W25_SECTSIZE_INPLACE                  (XFS_SB_W25_SECTSIZE_MASK << XFS_SB_W25_SECTSIZE_SHIFT)

/* word SB_W26: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W26_WIDTH                             32 /* established */
#define XFS_SB_W26_INODESIZE_SHIFT                   16 /* established */
#define XFS_SB_W26_INODESIZE_MASK                    0xffffu
#define XFS_SB_W26_INODESIZE_INPLACE                 (XFS_SB_W26_INODESIZE_MASK << XFS_SB_W26_INODESIZE_SHIFT)
#define XFS_SB_W26_INOPBLOCK_SHIFT                   0 /* established */
#define XFS_SB_W26_INOPBLOCK_MASK                    0xffffu
#define XFS_SB_W26_INOPBLOCK_INPLACE                 (XFS_SB_W26_INOPBLOCK_MASK << XFS_SB_W26_INOPBLOCK_SHIFT)

/* word SB_W3: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W3_WIDTH                              32 /* established */
#define XFS_SB_W3_DBLOCKS_LO_SHIFT                   0 /* established */
#define XFS_SB_W3_DBLOCKS_LO_MASK                    0xffffffffu
#define XFS_SB_W3_DBLOCKS_LO_INPLACE                 (XFS_SB_W3_DBLOCKS_LO_MASK << XFS_SB_W3_DBLOCKS_LO_SHIFT)

/* word SB_W30: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W30_WIDTH                             32 /* established */
#define XFS_SB_W30_BLOCKLOG_SHIFT                    24 /* established */
#define XFS_SB_W30_BLOCKLOG_MASK                     0xffu
#define XFS_SB_W30_BLOCKLOG_INPLACE                  (XFS_SB_W30_BLOCKLOG_MASK << XFS_SB_W30_BLOCKLOG_SHIFT)
#define XFS_SB_W30_SECTLOG_SHIFT                     16 /* established */
#define XFS_SB_W30_SECTLOG_MASK                      0xffu
#define XFS_SB_W30_SECTLOG_INPLACE                   (XFS_SB_W30_SECTLOG_MASK << XFS_SB_W30_SECTLOG_SHIFT)
#define XFS_SB_W30_INODELOG_SHIFT                    8 /* established */
#define XFS_SB_W30_INODELOG_MASK                     0xffu
#define XFS_SB_W30_INODELOG_INPLACE                  (XFS_SB_W30_INODELOG_MASK << XFS_SB_W30_INODELOG_SHIFT)
#define XFS_SB_W30_INOPBLOG_SHIFT                    0 /* established */
#define XFS_SB_W30_INOPBLOG_MASK                     0xffu
#define XFS_SB_W30_INOPBLOG_INPLACE                  (XFS_SB_W30_INOPBLOG_MASK << XFS_SB_W30_INOPBLOG_SHIFT)

/* word SB_W31: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W31_WIDTH                             32 /* established */
#define XFS_SB_W31_AGBLKLOG_SHIFT                    24 /* established */
#define XFS_SB_W31_AGBLKLOG_MASK                     0xffu
#define XFS_SB_W31_AGBLKLOG_INPLACE                  (XFS_SB_W31_AGBLKLOG_MASK << XFS_SB_W31_AGBLKLOG_SHIFT)
#define XFS_SB_W31_REXTSLOG_SHIFT                    16 /* established */
#define XFS_SB_W31_REXTSLOG_MASK                     0xffu
#define XFS_SB_W31_REXTSLOG_INPLACE                  (XFS_SB_W31_REXTSLOG_MASK << XFS_SB_W31_REXTSLOG_SHIFT)
#define XFS_SB_W31_INPROGRESS_SHIFT                  8 /* established */
#define XFS_SB_W31_INPROGRESS_MASK                   0xffu
#define XFS_SB_W31_INPROGRESS_INPLACE                (XFS_SB_W31_INPROGRESS_MASK << XFS_SB_W31_INPROGRESS_SHIFT)
#define XFS_SB_W31_IMAX_PCT_SHIFT                    0 /* established */
#define XFS_SB_W31_IMAX_PCT_MASK                     0xffu
#define XFS_SB_W31_IMAX_PCT_INPLACE                  (XFS_SB_W31_IMAX_PCT_MASK << XFS_SB_W31_IMAX_PCT_SHIFT)

/* word SB_W32: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W32_WIDTH                             32 /* established */
#define XFS_SB_W32_ICOUNT_HI_SHIFT                   0 /* established */
#define XFS_SB_W32_ICOUNT_HI_MASK                    0xffffffffu
#define XFS_SB_W32_ICOUNT_HI_INPLACE                 (XFS_SB_W32_ICOUNT_HI_MASK << XFS_SB_W32_ICOUNT_HI_SHIFT)

/* word SB_W33: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W33_WIDTH                             32 /* established */
#define XFS_SB_W33_ICOUNT_LO_SHIFT                   0 /* established */
#define XFS_SB_W33_ICOUNT_LO_MASK                    0xffffffffu
#define XFS_SB_W33_ICOUNT_LO_INPLACE                 (XFS_SB_W33_ICOUNT_LO_MASK << XFS_SB_W33_ICOUNT_LO_SHIFT)

/* word SB_W34: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W34_WIDTH                             32 /* established */
#define XFS_SB_W34_IFREE_HI_SHIFT                    0 /* established */
#define XFS_SB_W34_IFREE_HI_MASK                     0xffffffffu
#define XFS_SB_W34_IFREE_HI_INPLACE                  (XFS_SB_W34_IFREE_HI_MASK << XFS_SB_W34_IFREE_HI_SHIFT)

/* word SB_W35: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W35_WIDTH                             32 /* established */
#define XFS_SB_W35_IFREE_LO_SHIFT                    0 /* established */
#define XFS_SB_W35_IFREE_LO_MASK                     0xffffffffu
#define XFS_SB_W35_IFREE_LO_INPLACE                  (XFS_SB_W35_IFREE_LO_MASK << XFS_SB_W35_IFREE_LO_SHIFT)

/* word SB_W36: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W36_WIDTH                             32 /* established */
#define XFS_SB_W36_FDBLOCKS_HI_SHIFT                 0 /* established */
#define XFS_SB_W36_FDBLOCKS_HI_MASK                  0xffffffffu
#define XFS_SB_W36_FDBLOCKS_HI_INPLACE               (XFS_SB_W36_FDBLOCKS_HI_MASK << XFS_SB_W36_FDBLOCKS_HI_SHIFT)

/* word SB_W37: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W37_WIDTH                             32 /* established */
#define XFS_SB_W37_FDBLOCKS_LO_SHIFT                 0 /* established */
#define XFS_SB_W37_FDBLOCKS_LO_MASK                  0xffffffffu
#define XFS_SB_W37_FDBLOCKS_LO_INPLACE               (XFS_SB_W37_FDBLOCKS_LO_MASK << XFS_SB_W37_FDBLOCKS_LO_SHIFT)

/* word SB_W38: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W38_WIDTH                             32 /* established */
#define XFS_SB_W38_FREXTENTS_HI_SHIFT                0 /* established */
#define XFS_SB_W38_FREXTENTS_HI_MASK                 0xffffffffu
#define XFS_SB_W38_FREXTENTS_HI_INPLACE              (XFS_SB_W38_FREXTENTS_HI_MASK << XFS_SB_W38_FREXTENTS_HI_SHIFT)

/* word SB_W39: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W39_WIDTH                             32 /* established */
#define XFS_SB_W39_FREXTENTS_LO_SHIFT                0 /* established */
#define XFS_SB_W39_FREXTENTS_LO_MASK                 0xffffffffu
#define XFS_SB_W39_FREXTENTS_LO_INPLACE              (XFS_SB_W39_FREXTENTS_LO_MASK << XFS_SB_W39_FREXTENTS_LO_SHIFT)

/* word SB_W4: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W4_WIDTH                              32 /* established */
#define XFS_SB_W4_RBLOCKS_HI_SHIFT                   0 /* established */
#define XFS_SB_W4_RBLOCKS_HI_MASK                    0xffffffffu
#define XFS_SB_W4_RBLOCKS_HI_INPLACE                 (XFS_SB_W4_RBLOCKS_HI_MASK << XFS_SB_W4_RBLOCKS_HI_SHIFT)

/* word SB_W40: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W40_WIDTH                             32 /* established */
#define XFS_SB_W40_UQUOTINO_HI_SHIFT                 0 /* established */
#define XFS_SB_W40_UQUOTINO_HI_MASK                  0xffffffffu
#define XFS_SB_W40_UQUOTINO_HI_INPLACE               (XFS_SB_W40_UQUOTINO_HI_MASK << XFS_SB_W40_UQUOTINO_HI_SHIFT)

/* word SB_W41: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W41_WIDTH                             32 /* established */
#define XFS_SB_W41_UQUOTINO_LO_SHIFT                 0 /* established */
#define XFS_SB_W41_UQUOTINO_LO_MASK                  0xffffffffu
#define XFS_SB_W41_UQUOTINO_LO_INPLACE               (XFS_SB_W41_UQUOTINO_LO_MASK << XFS_SB_W41_UQUOTINO_LO_SHIFT)

/* word SB_W42: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W42_WIDTH                             32 /* established */
#define XFS_SB_W42_PQUOTINO_HI_SHIFT                 0 /* established */
#define XFS_SB_W42_PQUOTINO_HI_MASK                  0xffffffffu
#define XFS_SB_W42_PQUOTINO_HI_INPLACE               (XFS_SB_W42_PQUOTINO_HI_MASK << XFS_SB_W42_PQUOTINO_HI_SHIFT)

/* word SB_W43: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W43_WIDTH                             32 /* established */
#define XFS_SB_W43_PQUOTINO_LO_SHIFT                 0 /* established */
#define XFS_SB_W43_PQUOTINO_LO_MASK                  0xffffffffu
#define XFS_SB_W43_PQUOTINO_LO_INPLACE               (XFS_SB_W43_PQUOTINO_LO_MASK << XFS_SB_W43_PQUOTINO_LO_SHIFT)

/* word SB_W44: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W44_WIDTH                             32 /* established */
#define XFS_SB_W44_QFLAGS_SHIFT                      16 /* established */
#define XFS_SB_W44_QFLAGS_MASK                       0xffffu
#define XFS_SB_W44_QFLAGS_INPLACE                    (XFS_SB_W44_QFLAGS_MASK << XFS_SB_W44_QFLAGS_SHIFT)
#define XFS_SB_W44_FLAGS_SHARED_RO_SHIFT             8 /* established */
#define XFS_SB_W44_FLAGS_SHARED_RO_MASK              0x1u
#define XFS_SB_W44_FLAGS_SHARED_RO_INPLACE           (XFS_SB_W44_FLAGS_SHARED_RO_MASK << XFS_SB_W44_FLAGS_SHARED_RO_SHIFT)
#define XFS_SB_W44_SHARED_VN_SHIFT                   0 /* established */
#define XFS_SB_W44_SHARED_VN_MASK                    0xffu
#define XFS_SB_W44_SHARED_VN_INPLACE                 (XFS_SB_W44_SHARED_VN_MASK << XFS_SB_W44_SHARED_VN_SHIFT)

/* word SB_W45: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W45_WIDTH                             32 /* established */
#define XFS_SB_W45_INOALIGNMT_SHIFT                  0 /* established */
#define XFS_SB_W45_INOALIGNMT_MASK                   0xffffffffu
#define XFS_SB_W45_INOALIGNMT_INPLACE                (XFS_SB_W45_INOALIGNMT_MASK << XFS_SB_W45_INOALIGNMT_SHIFT)

/* word SB_W46: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W46_WIDTH                             32 /* established */
#define XFS_SB_W46_UNIT_SHIFT                        0 /* established */
#define XFS_SB_W46_UNIT_MASK                         0xffffffffu
#define XFS_SB_W46_UNIT_INPLACE                      (XFS_SB_W46_UNIT_MASK << XFS_SB_W46_UNIT_SHIFT)

/* word SB_W47: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W47_WIDTH                             32 /* established */
#define XFS_SB_W47_WIDTH_SHIFT                       0 /* established */
#define XFS_SB_W47_WIDTH_MASK                        0xffffffffu
#define XFS_SB_W47_WIDTH_INPLACE                     (XFS_SB_W47_WIDTH_MASK << XFS_SB_W47_WIDTH_SHIFT)

/* word SB_W48: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W48_WIDTH                             32 /* established */
#define XFS_SB_W48_DIRBLKLOG_SHIFT                   24 /* established */
#define XFS_SB_W48_DIRBLKLOG_MASK                    0xffu
#define XFS_SB_W48_DIRBLKLOG_INPLACE                 (XFS_SB_W48_DIRBLKLOG_MASK << XFS_SB_W48_DIRBLKLOG_SHIFT)
#define XFS_SB_W48_LOGSECTLOG_SHIFT                  16 /* established */
#define XFS_SB_W48_LOGSECTLOG_MASK                   0xffu
#define XFS_SB_W48_LOGSECTLOG_INPLACE                (XFS_SB_W48_LOGSECTLOG_MASK << XFS_SB_W48_LOGSECTLOG_SHIFT)
#define XFS_SB_W48_LOGSECTSIZE_SHIFT                 0 /* established */
#define XFS_SB_W48_LOGSECTSIZE_MASK                  0xffffu
#define XFS_SB_W48_LOGSECTSIZE_INPLACE               (XFS_SB_W48_LOGSECTSIZE_MASK << XFS_SB_W48_LOGSECTSIZE_SHIFT)

/* word SB_W49: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W49_WIDTH                             32 /* established */
#define XFS_SB_W49_LOGSUNIT_SHIFT                    0 /* established */
#define XFS_SB_W49_LOGSUNIT_MASK                     0xffffffffu
#define XFS_SB_W49_LOGSUNIT_INPLACE                  (XFS_SB_W49_LOGSUNIT_MASK << XFS_SB_W49_LOGSUNIT_SHIFT)

/* word SB_W5: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W5_WIDTH                              32 /* established */
#define XFS_SB_W5_RBLOCKS_LO_SHIFT                   0 /* established */
#define XFS_SB_W5_RBLOCKS_LO_MASK                    0xffffffffu
#define XFS_SB_W5_RBLOCKS_LO_INPLACE                 (XFS_SB_W5_RBLOCKS_LO_MASK << XFS_SB_W5_RBLOCKS_LO_SHIFT)

/* word SB_W50: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W50_WIDTH                             32 /* unknown */
#define XFS_SB_W50_FEATURES2_HIGH_SHIFT              5 /* unknown */
#define XFS_SB_W50_FEATURES2_HIGH_MASK               0x7ffffffu
#define XFS_SB_W50_FEATURES2_HIGH_INPLACE            (XFS_SB_W50_FEATURES2_HIGH_MASK << XFS_SB_W50_FEATURES2_HIGH_SHIFT)
#define XFS_SB_W50_FEATURES2_PARENT_SHIFT            4 /* established */
#define XFS_SB_W50_FEATURES2_PARENT_MASK             0x1u
#define XFS_SB_W50_FEATURES2_PARENT_INPLACE          (XFS_SB_W50_FEATURES2_PARENT_MASK << XFS_SB_W50_FEATURES2_PARENT_SHIFT)
#define XFS_SB_W50_FEATURES2_ATTR2_SHIFT             3 /* established */
#define XFS_SB_W50_FEATURES2_ATTR2_MASK              0x1u
#define XFS_SB_W50_FEATURES2_ATTR2_INPLACE           (XFS_SB_W50_FEATURES2_ATTR2_MASK << XFS_SB_W50_FEATURES2_ATTR2_SHIFT)
#define XFS_SB_W50_FEATURES2_BIT2_SHIFT              2 /* unknown */
#define XFS_SB_W50_FEATURES2_BIT2_MASK               0x1u
#define XFS_SB_W50_FEATURES2_BIT2_INPLACE            (XFS_SB_W50_FEATURES2_BIT2_MASK << XFS_SB_W50_FEATURES2_BIT2_SHIFT)
#define XFS_SB_W50_FEATURES2_LAZYSBCOUNT_SHIFT       1 /* established */
#define XFS_SB_W50_FEATURES2_LAZYSBCOUNT_MASK        0x1u
#define XFS_SB_W50_FEATURES2_LAZYSBCOUNT_INPLACE     (XFS_SB_W50_FEATURES2_LAZYSBCOUNT_MASK << XFS_SB_W50_FEATURES2_LAZYSBCOUNT_SHIFT)
#define XFS_SB_W50_FEATURES2_BIT0_SHIFT              0 /* unknown */
#define XFS_SB_W50_FEATURES2_BIT0_MASK               0x1u
#define XFS_SB_W50_FEATURES2_BIT0_INPLACE            (XFS_SB_W50_FEATURES2_BIT0_MASK << XFS_SB_W50_FEATURES2_BIT0_SHIFT)

/* word SB_W6: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W6_WIDTH                              32 /* established */
#define XFS_SB_W6_REXTENTS_HI_SHIFT                  0 /* established */
#define XFS_SB_W6_REXTENTS_HI_MASK                   0xffffffffu
#define XFS_SB_W6_REXTENTS_HI_INPLACE                (XFS_SB_W6_REXTENTS_HI_MASK << XFS_SB_W6_REXTENTS_HI_SHIFT)

/* word SB_W7: 32 bits; shipped IRIX binaries, per docs/formats/xfs.md */
#define XFS_SB_W7_WIDTH                              32 /* established */
#define XFS_SB_W7_REXTENTS_LO_SHIFT                  0 /* established */
#define XFS_SB_W7_REXTENTS_LO_MASK                   0xffffffffu
#define XFS_SB_W7_REXTENTS_LO_INPLACE                (XFS_SB_W7_REXTENTS_LO_MASK << XFS_SB_W7_REXTENTS_LO_SHIFT)

#define XFS_INO_ATTR_FORK_UNIT                       0x8u /* established */
#define XFS_INO_FLAGS_OFFSET                         0x5au /* established */
#define XFS_INO_FORK_OFFSET                          0x64u /* established */
#define XFS_INO_MAGIC                                0x494eu /* established */
#define XFS_INO_SIZE                                 0x64u /* established */
#define XFS_SB_DESCRIBED_BYTES                       0xccu /* established */
#define XFS_SB_LEN_FNAME                             0x6u /* established */
#define XFS_SB_LEN_FPACK                             0x6u /* established */
#define XFS_SB_LEN_UUID                              0x10u /* established */
#define XFS_SB_MAGIC                                 0x58465342u /* established */
#define XFS_SB_OFF_FNAME                             0x6cu /* established */
#define XFS_SB_OFF_FPACK                             0x72u /* established */
#define XFS_SB_OFF_UUID                              0x20u /* established */
#define XFS_SB_READ_BYTES                            0xd0u /* established */
#define XFS_SB_SIZE                                  0xd0u /* established */

#endif /* XFS_GEN_H */
