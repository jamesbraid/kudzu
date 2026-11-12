/* generated from defs/xvm.toml by defs-gen - do not edit, change the source */
/* XVM volume manager on-disk label structures */
#ifndef XVM_GEN_H
#define XVM_GEN_H

#include <stdint.h>


/* word ATTR_PERM_W6: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_ATTR_PERM_W6_WIDTH                       32 /* established */
#define XVM_ATTR_PERM_W6_UID_SHIFT                   0 /* established */
#define XVM_ATTR_PERM_W6_UID_MASK                    0xffffffffu
#define XVM_ATTR_PERM_W6_UID_INPLACE                 (XVM_ATTR_PERM_W6_UID_MASK << XVM_ATTR_PERM_W6_UID_SHIFT)

/* word ATTR_PERM_W7: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_ATTR_PERM_W7_WIDTH                       32 /* established */
#define XVM_ATTR_PERM_W7_GID_SHIFT                   0 /* established */
#define XVM_ATTR_PERM_W7_GID_MASK                    0xffffffffu
#define XVM_ATTR_PERM_W7_GID_INPLACE                 (XVM_ATTR_PERM_W7_GID_MASK << XVM_ATTR_PERM_W7_GID_SHIFT)

/* word ATTR_PERM_W8: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_ATTR_PERM_W8_WIDTH                       32 /* established */
#define XVM_ATTR_PERM_W8_MODE_SHIFT                  0 /* established */
#define XVM_ATTR_PERM_W8_MODE_MASK                   0xffffffffu
#define XVM_ATTR_PERM_W8_MODE_INPLACE                (XVM_ATTR_PERM_W8_MODE_MASK << XVM_ATTR_PERM_W8_MODE_SHIFT)

/* word ATTR_W0: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_ATTR_W0_WIDTH                            32 /* established */
#define XVM_ATTR_W0_TYPE_SHIFT                       24 /* established */
#define XVM_ATTR_W0_TYPE_MASK                        0xffu
#define XVM_ATTR_W0_TYPE_INPLACE                     (XVM_ATTR_W0_TYPE_MASK << XVM_ATTR_W0_TYPE_SHIFT)
#define XVM_ATTR_W0_TYPE_FREE                        0 /* established */
#define XVM_ATTR_W0_TYPE_NAME                        1 /* established */
#define XVM_ATTR_W0_TYPE_PERM                        2 /* established */
#define XVM_ATTR_W0_FLAG_PERSISTENT_SHIFT            16 /* inferred */
#define XVM_ATTR_W0_FLAG_PERSISTENT_MASK             0x1u
#define XVM_ATTR_W0_FLAG_PERSISTENT_INPLACE          (XVM_ATTR_W0_FLAG_PERSISTENT_MASK << XVM_ATTR_W0_FLAG_PERSISTENT_SHIFT)

/* word ATTR_W1: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_ATTR_W1_WIDTH                            32 /* established */
#define XVM_ATTR_W1_TID_SHIFT                        0 /* established */
#define XVM_ATTR_W1_TID_MASK                         0xffffffffu
#define XVM_ATTR_W1_TID_INPLACE                      (XVM_ATTR_W1_TID_MASK << XVM_ATTR_W1_TID_SHIFT)

/* word LH_W0: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_LH_W0_WIDTH                              32 /* established */
#define XVM_LH_W0_MAGIC_SHIFT                        0 /* established */
#define XVM_LH_W0_MAGIC_MASK                         0xffffffffu
#define XVM_LH_W0_MAGIC_INPLACE                      (XVM_LH_W0_MAGIC_MASK << XVM_LH_W0_MAGIC_SHIFT)

/* word LH_W1: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_LH_W1_WIDTH                              32 /* established */
#define XVM_LH_W1_VERSION_SHIFT                      16 /* established */
#define XVM_LH_W1_VERSION_MASK                       0xffffu
#define XVM_LH_W1_VERSION_INPLACE                    (XVM_LH_W1_VERSION_MASK << XVM_LH_W1_VERSION_SHIFT)
#define XVM_LH_W1_TYPE_SHIFT                         8 /* established */
#define XVM_LH_W1_TYPE_MASK                          0xffu
#define XVM_LH_W1_TYPE_INPLACE                       (XVM_LH_W1_TYPE_MASK << XVM_LH_W1_TYPE_SHIFT)
#define XVM_LH_W1_TYPE_FREE                          0 /* established */
#define XVM_LH_W1_TYPE_RESERVED                      1 /* unknown */
#define XVM_LH_W1_TYPE_VE                            2 /* established */
#define XVM_LH_W1_TYPE_ATTR                          3 /* established */
#define XVM_LH_W1_SIZE_SHIFT                         0 /* established */
#define XVM_LH_W1_SIZE_MASK                          0xffu
#define XVM_LH_W1_SIZE_INPLACE                       (XVM_LH_W1_SIZE_MASK << XVM_LH_W1_SIZE_SHIFT)

/* word PL_W0: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_PL_W0_WIDTH                              32 /* established */
#define XVM_PL_W0_MAGIC_SHIFT                        0 /* established */
#define XVM_PL_W0_MAGIC_MASK                         0xffffffffu
#define XVM_PL_W0_MAGIC_INPLACE                      (XVM_PL_W0_MAGIC_MASK << XVM_PL_W0_MAGIC_SHIFT)

/* word PL_W1: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_PL_W1_WIDTH                              32 /* established */
#define XVM_PL_W1_VERSION_SHIFT                      16 /* established */
#define XVM_PL_W1_VERSION_MASK                       0xffffu
#define XVM_PL_W1_VERSION_INPLACE                    (XVM_PL_W1_VERSION_MASK << XVM_PL_W1_VERSION_SHIFT)
#define XVM_PL_W1_STATE_DRAINING_SHIFT               14 /* inferred */
#define XVM_PL_W1_STATE_DRAINING_MASK                0x1u
#define XVM_PL_W1_STATE_DRAINING_INPLACE             (XVM_PL_W1_STATE_DRAINING_MASK << XVM_PL_W1_STATE_DRAINING_SHIFT)
#define XVM_PL_W1_STATE_CLUSTER_SHIFT                13 /* established */
#define XVM_PL_W1_STATE_CLUSTER_MASK                 0x1u
#define XVM_PL_W1_STATE_CLUSTER_INPLACE              (XVM_PL_W1_STATE_CLUSTER_MASK << XVM_PL_W1_STATE_CLUSTER_SHIFT)
#define XVM_PL_W1_STATE_PRIVATE_SHIFT                12 /* established */
#define XVM_PL_W1_STATE_PRIVATE_MASK                 0x1u
#define XVM_PL_W1_STATE_PRIVATE_INPLACE              (XVM_PL_W1_STATE_PRIVATE_MASK << XVM_PL_W1_STATE_PRIVATE_SHIFT)
#define XVM_PL_W1_STATE_RECOVERING_SHIFT             11 /* inferred */
#define XVM_PL_W1_STATE_RECOVERING_MASK              0x1u
#define XVM_PL_W1_STATE_RECOVERING_INPLACE           (XVM_PL_W1_STATE_RECOVERING_MASK << XVM_PL_W1_STATE_RECOVERING_SHIFT)
#define XVM_PL_W1_STATE_MEDIAERROR_SHIFT             10 /* inferred */
#define XVM_PL_W1_STATE_MEDIAERROR_MASK              0x1u
#define XVM_PL_W1_STATE_MEDIAERROR_INPLACE           (XVM_PL_W1_STATE_MEDIAERROR_MASK << XVM_PL_W1_STATE_MEDIAERROR_SHIFT)
#define XVM_PL_W1_STATE_SPARE_SHIFT                  9 /* inferred */
#define XVM_PL_W1_STATE_SPARE_MASK                   0x1u
#define XVM_PL_W1_STATE_SPARE_INPLACE                (XVM_PL_W1_STATE_SPARE_MASK << XVM_PL_W1_STATE_SPARE_SHIFT)
#define XVM_PL_W1_STATE_ONLINE_SHIFT                 8 /* inferred */
#define XVM_PL_W1_STATE_ONLINE_MASK                  0x1u
#define XVM_PL_W1_STATE_ONLINE_INPLACE               (XVM_PL_W1_STATE_ONLINE_MASK << XVM_PL_W1_STATE_ONLINE_SHIFT)
#define XVM_PL_W1_FLAGS_SYNC_SHIFT                   3 /* established */
#define XVM_PL_W1_FLAGS_SYNC_MASK                    0x1u
#define XVM_PL_W1_FLAGS_SYNC_INPLACE                 (XVM_PL_W1_FLAGS_SYNC_MASK << XVM_PL_W1_FLAGS_SYNC_SHIFT)
#define XVM_PL_W1_FLAGS_DUPLICATE_SHIFT              2 /* established */
#define XVM_PL_W1_FLAGS_DUPLICATE_MASK               0x1u
#define XVM_PL_W1_FLAGS_DUPLICATE_INPLACE            (XVM_PL_W1_FLAGS_DUPLICATE_MASK << XVM_PL_W1_FLAGS_DUPLICATE_SHIFT)
#define XVM_PL_W1_FLAGS_BOOTABLE_SHIFT               1 /* established */
#define XVM_PL_W1_FLAGS_BOOTABLE_MASK                0x1u
#define XVM_PL_W1_FLAGS_BOOTABLE_INPLACE             (XVM_PL_W1_FLAGS_BOOTABLE_MASK << XVM_PL_W1_FLAGS_BOOTABLE_SHIFT)

/* word PL_W14: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_PL_W14_WIDTH                             32 /* established */
#define XVM_PL_W14_SECSIZE_SHIFT                     16 /* established */
#define XVM_PL_W14_SECSIZE_MASK                      0xffffu
#define XVM_PL_W14_SECSIZE_INPLACE                   (XVM_PL_W14_SECSIZE_MASK << XVM_PL_W14_SECSIZE_SHIFT)
#define XVM_PL_W14_XVMLABEL_SHIFT                    0 /* established */
#define XVM_PL_W14_XVMLABEL_MASK                     0xffffu
#define XVM_PL_W14_XVMLABEL_INPLACE                  (XVM_PL_W14_XVMLABEL_MASK << XVM_PL_W14_XVMLABEL_SHIFT)

/* word PL_W15: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_PL_W15_WIDTH                             32 /* established */
#define XVM_PL_W15_XVMLABELBLKS_SHIFT                16 /* established */
#define XVM_PL_W15_XVMLABELBLKS_MASK                 0xffffu
#define XVM_PL_W15_XVMLABELBLKS_INPLACE              (XVM_PL_W15_XVMLABELBLKS_MASK << XVM_PL_W15_XVMLABELBLKS_SHIFT)
#define XVM_PL_W15_DBN_SHIFT                         0 /* established */
#define XVM_PL_W15_DBN_MASK                          0xffffu
#define XVM_PL_W15_DBN_INPLACE                       (XVM_PL_W15_DBN_MASK << XVM_PL_W15_DBN_SHIFT)

/* word PL_W16: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_PL_W16_WIDTH                             32 /* established */
#define XVM_PL_W16_NBLKS_SHIFT                       0 /* established */
#define XVM_PL_W16_NBLKS_MASK                        0xffffffffu
#define XVM_PL_W16_NBLKS_INPLACE                     (XVM_PL_W16_NBLKS_MASK << XVM_PL_W16_NBLKS_SHIFT)

/* word PL_W17: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_PL_W17_WIDTH                             32 /* established */
#define XVM_PL_W17_TIMESTAMP_SHIFT                   0 /* established */
#define XVM_PL_W17_TIMESTAMP_MASK                    0xffffffffu
#define XVM_PL_W17_TIMESTAMP_INPLACE                 (XVM_PL_W17_TIMESTAMP_MASK << XVM_PL_W17_TIMESTAMP_SHIFT)

/* word PL_W30: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_PL_W30_WIDTH                             32 /* established */
#define XVM_PL_W30_CLUSTERID_SHIFT                   0 /* established */
#define XVM_PL_W30_CLUSTERID_MASK                    0xffffffffu
#define XVM_PL_W30_CLUSTERID_INPLACE                 (XVM_PL_W30_CLUSTERID_MASK << XVM_PL_W30_CLUSTERID_SHIFT)

/* word VE_MIRROR_W14: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_MIRROR_W14_WIDTH                      32 /* established */
#define XVM_VE_MIRROR_W14_OLDLENGTH_HI_SHIFT         0 /* established */
#define XVM_VE_MIRROR_W14_OLDLENGTH_HI_MASK          0xffffffffu
#define XVM_VE_MIRROR_W14_OLDLENGTH_HI_INPLACE       (XVM_VE_MIRROR_W14_OLDLENGTH_HI_MASK << XVM_VE_MIRROR_W14_OLDLENGTH_HI_SHIFT)

/* word VE_MIRROR_W15: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_MIRROR_W15_WIDTH                      32 /* established */
#define XVM_VE_MIRROR_W15_OLDLENGTH_LO_SHIFT         0 /* established */
#define XVM_VE_MIRROR_W15_OLDLENGTH_LO_MASK          0xffffffffu
#define XVM_VE_MIRROR_W15_OLDLENGTH_LO_INPLACE       (XVM_VE_MIRROR_W15_OLDLENGTH_LO_MASK << XVM_VE_MIRROR_W15_OLDLENGTH_LO_SHIFT)

/* word VE_MIRROR_W16: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_MIRROR_W16_WIDTH                      32 /* established */
#define XVM_VE_MIRROR_W16_LOGFREQ_SHIFT              16 /* established */
#define XVM_VE_MIRROR_W16_LOGFREQ_MASK               0xffffu
#define XVM_VE_MIRROR_W16_LOGFREQ_INPLACE            (XVM_VE_MIRROR_W16_LOGFREQ_MASK << XVM_VE_MIRROR_W16_LOGFREQ_SHIFT)
#define XVM_VE_MIRROR_W16_POLICY_SHIFT               8 /* established */
#define XVM_VE_MIRROR_W16_POLICY_MASK                0xffu
#define XVM_VE_MIRROR_W16_POLICY_INPLACE             (XVM_VE_MIRROR_W16_POLICY_MASK << XVM_VE_MIRROR_W16_POLICY_SHIFT)
#define XVM_VE_MIRROR_W16_POLICY_ROUND_ROBIN         0 /* established */
#define XVM_VE_MIRROR_W16_POLICY_SEQUENTIAL          1 /* established */
#define XVM_VE_MIRROR_W16_CONFIG_SHIFT               0 /* inferred */
#define XVM_VE_MIRROR_W16_CONFIG_MASK                0xffu
#define XVM_VE_MIRROR_W16_CONFIG_INPLACE             (XVM_VE_MIRROR_W16_CONFIG_MASK << XVM_VE_MIRROR_W16_CONFIG_SHIFT)

/* word VE_RAID_W14: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_RAID_W14_WIDTH                        32 /* inferred */
#define XVM_VE_RAID_W14_SUSIZE_SHIFT                 0 /* inferred */
#define XVM_VE_RAID_W14_SUSIZE_MASK                  0xffffffffu
#define XVM_VE_RAID_W14_SUSIZE_INPLACE               (XVM_VE_RAID_W14_SUSIZE_MASK << XVM_VE_RAID_W14_SUSIZE_SHIFT)

/* word VE_RAID_W15: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_RAID_W15_WIDTH                        32 /* inferred */
#define XVM_VE_RAID_W15_LAYOUT_SHIFT                 24 /* inferred */
#define XVM_VE_RAID_W15_LAYOUT_MASK                  0xffu
#define XVM_VE_RAID_W15_LAYOUT_INPLACE               (XVM_VE_RAID_W15_LAYOUT_MASK << XVM_VE_RAID_W15_LAYOUT_SHIFT)

/* word VE_SLICE_W14: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_SLICE_W14_WIDTH                       32 /* established */
#define XVM_VE_SLICE_W14_STARTBLK_HI_SHIFT           0 /* established */
#define XVM_VE_SLICE_W14_STARTBLK_HI_MASK            0xffffffffu
#define XVM_VE_SLICE_W14_STARTBLK_HI_INPLACE         (XVM_VE_SLICE_W14_STARTBLK_HI_MASK << XVM_VE_SLICE_W14_STARTBLK_HI_SHIFT)

/* word VE_SLICE_W15: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_SLICE_W15_WIDTH                       32 /* established */
#define XVM_VE_SLICE_W15_STARTBLK_LO_SHIFT           0 /* established */
#define XVM_VE_SLICE_W15_STARTBLK_LO_MASK            0xffffffffu
#define XVM_VE_SLICE_W15_STARTBLK_LO_INPLACE         (XVM_VE_SLICE_W15_STARTBLK_LO_MASK << XVM_VE_SLICE_W15_STARTBLK_LO_SHIFT)

/* word VE_SLICE_W16: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_SLICE_W16_WIDTH                       32 /* established */
#define XVM_VE_SLICE_W16_SEQ_SHIFT                   16 /* established */
#define XVM_VE_SLICE_W16_SEQ_MASK                    0xffffu
#define XVM_VE_SLICE_W16_SEQ_INPLACE                 (XVM_VE_SLICE_W16_SEQ_MASK << XVM_VE_SLICE_W16_SEQ_SHIFT)
#define XVM_VE_SLICE_W16_FLAGS_SHIFT                 8 /* established */
#define XVM_VE_SLICE_W16_FLAGS_MASK                  0xffu
#define XVM_VE_SLICE_W16_FLAGS_INPLACE               (XVM_VE_SLICE_W16_FLAGS_MASK << XVM_VE_SLICE_W16_FLAGS_SHIFT)
#define XVM_VE_SLICE_W16_PARTITION_SHIFT             0 /* established */
#define XVM_VE_SLICE_W16_PARTITION_MASK              0xffu
#define XVM_VE_SLICE_W16_PARTITION_INPLACE           (XVM_VE_SLICE_W16_PARTITION_MASK << XVM_VE_SLICE_W16_PARTITION_SHIFT)

/* word VE_STRIPE_W14: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_STRIPE_W14_WIDTH                      32 /* established */
#define XVM_VE_STRIPE_W14_SUSIZE_SHIFT               0 /* established */
#define XVM_VE_STRIPE_W14_SUSIZE_MASK                0xffffffffu
#define XVM_VE_STRIPE_W14_SUSIZE_INPLACE             (XVM_VE_STRIPE_W14_SUSIZE_MASK << XVM_VE_STRIPE_W14_SUSIZE_SHIFT)

/* word VE_SUBVOL_W14: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_SUBVOL_W14_WIDTH                      32 /* established */
#define XVM_VE_SUBVOL_W14_SVTYPE_SHIFT               24 /* established */
#define XVM_VE_SUBVOL_W14_SVTYPE_MASK                0xffu
#define XVM_VE_SUBVOL_W14_SVTYPE_INPLACE             (XVM_VE_SUBVOL_W14_SVTYPE_MASK << XVM_VE_SUBVOL_W14_SVTYPE_SHIFT)
#define XVM_VE_SUBVOL_W14_SVTYPE_ALL                 0 /* established */
#define XVM_VE_SUBVOL_W14_SVTYPE_LOG                 1 /* established */
#define XVM_VE_SUBVOL_W14_SVTYPE_DATA                2 /* established */
#define XVM_VE_SUBVOL_W14_SVTYPE_RT                  3 /* established */
#define XVM_VE_SUBVOL_W14_SVTYPE_SWAP                4 /* established */
#define XVM_VE_SUBVOL_W14_SVTYPE_USER1               16 /* established */

/* word VE_VOL_W14: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_VOL_W14_WIDTH                         32 /* established */
#define XVM_VE_VOL_W14_REPFULL_SHIFT                 24 /* established */
#define XVM_VE_VOL_W14_REPFULL_MASK                  0xffu
#define XVM_VE_VOL_W14_REPFULL_INPLACE               (XVM_VE_VOL_W14_REPFULL_MASK << XVM_VE_VOL_W14_REPFULL_SHIFT)

/* word VE_W0: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_W0_WIDTH                              32 /* established */
#define XVM_VE_W0_TYPE_SHIFT                         24 /* established */
#define XVM_VE_W0_TYPE_MASK                          0xffu
#define XVM_VE_W0_TYPE_INPLACE                       (XVM_VE_W0_TYPE_MASK << XVM_VE_W0_TYPE_SHIFT)
#define XVM_VE_W0_TYPE_FREE                          0 /* established */
#define XVM_VE_W0_TYPE_SLICE                         1 /* established */
#define XVM_VE_W0_TYPE_CONCAT                        2 /* established */
#define XVM_VE_W0_TYPE_STRIPE                        3 /* established */
#define XVM_VE_W0_TYPE_RAID                          4 /* established */
#define XVM_VE_W0_TYPE_MIRROR                        5 /* established */
#define XVM_VE_W0_TYPE_SUBVOL                        6 /* established */
#define XVM_VE_W0_TYPE_VOLUME                        7 /* established */
#define XVM_VE_W0_TYPE_COW                           11 /* established */
#define XVM_VE_W0_TYPE_SNAPSHOT                      12 /* established */
#define XVM_VE_W0_STATE_DISABLED_SHIFT               20 /* established */
#define XVM_VE_W0_STATE_DISABLED_MASK                0x1u
#define XVM_VE_W0_STATE_DISABLED_INPLACE             (XVM_VE_W0_STATE_DISABLED_MASK << XVM_VE_W0_STATE_DISABLED_SHIFT)
#define XVM_VE_W0_STATE_VALID_SHIFT                  17 /* established */
#define XVM_VE_W0_STATE_VALID_MASK                   0x1u
#define XVM_VE_W0_STATE_VALID_INPLACE                (XVM_VE_W0_STATE_VALID_MASK << XVM_VE_W0_STATE_VALID_SHIFT)
#define XVM_VE_W0_STATE_CLEAN_SHIFT                  16 /* established */
#define XVM_VE_W0_STATE_CLEAN_MASK                   0x1u
#define XVM_VE_W0_STATE_CLEAN_INPLACE                (XVM_VE_W0_STATE_CLEAN_MASK << XVM_VE_W0_STATE_CLEAN_SHIFT)
#define XVM_VE_W0_PIECES_SHIFT                       0 /* established */
#define XVM_VE_W0_PIECES_MASK                        0xffffu
#define XVM_VE_W0_PIECES_INPLACE                     (XVM_VE_W0_PIECES_MASK << XVM_VE_W0_PIECES_SHIFT)

/* word VE_W1: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_W1_WIDTH                              32 /* established */
#define XVM_VE_W1_TID_SHIFT                          0 /* established */
#define XVM_VE_W1_TID_MASK                           0xffffffffu
#define XVM_VE_W1_TID_INPLACE                        (XVM_VE_W1_TID_MASK << XVM_VE_W1_TID_SHIFT)

/* word VE_W10: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_W10_WIDTH                             32 /* established */
#define XVM_VE_W10_LENGTH_HI_SHIFT                   0 /* established */
#define XVM_VE_W10_LENGTH_HI_MASK                    0xffffffffu
#define XVM_VE_W10_LENGTH_HI_INPLACE                 (XVM_VE_W10_LENGTH_HI_MASK << XVM_VE_W10_LENGTH_HI_SHIFT)

/* word VE_W11: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_W11_WIDTH                             32 /* established */
#define XVM_VE_W11_LENGTH_LO_SHIFT                   0 /* established */
#define XVM_VE_W11_LENGTH_LO_MASK                    0xffffffffu
#define XVM_VE_W11_LENGTH_LO_INPLACE                 (XVM_VE_W11_LENGTH_LO_MASK << XVM_VE_W11_LENGTH_LO_SHIFT)

/* word VE_W12: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_W12_WIDTH                             32 /* established */
#define XVM_VE_W12_POS_SHIFT                         16 /* established */
#define XVM_VE_W12_POS_MASK                          0xffffu
#define XVM_VE_W12_POS_INPLACE                       (XVM_VE_W12_POS_MASK << XVM_VE_W12_POS_SHIFT)
#define XVM_VE_W12_COWFIRST_SHIFT                    0 /* established */
#define XVM_VE_W12_COWFIRST_MASK                     0xffffu
#define XVM_VE_W12_COWFIRST_INPLACE                  (XVM_VE_W12_COWFIRST_MASK << XVM_VE_W12_COWFIRST_SHIFT)

/* word VE_W13: 32 bits; shipped IRIX binaries, per docs/formats/xvm.md */
#define XVM_VE_W13_WIDTH                             32 /* established */
#define XVM_VE_W13_COWNEXT_SHIFT                     16 /* established */
#define XVM_VE_W13_COWNEXT_MASK                      0xffffu
#define XVM_VE_W13_COWNEXT_INPLACE                   (XVM_VE_W13_COWNEXT_MASK << XVM_VE_W13_COWNEXT_SHIFT)
#define XVM_VE_W13_COWREGSZ_SHIFT                    0 /* established */
#define XVM_VE_W13_COWREGSZ_MASK                     0xffffu
#define XVM_VE_W13_COWREGSZ_INPLACE                  (XVM_VE_W13_COWREGSZ_MASK << XVM_VE_W13_COWREGSZ_SHIFT)

#define XVM_ATTR_LEN_NAME                            0x30u /* established */
#define XVM_ATTR_LEN_UUID                            0x10u /* established */
#define XVM_ATTR_OFF_NAME                            0x18u /* established */
#define XVM_ATTR_OFF_UUID                            0x8u /* established */
#define XVM_ATTR_SIZE                                0x48u /* established */
#define XVM_DISKNAME_MAX                             0x30u /* established */
#define XVM_HOSTNAME_MAX                             0x20u /* established */
#define XVM_LABELHDR_MAGIC                           0x6c646872u /* established */
#define XVM_LABELHDR_VERSION                         0x2u /* established */
#define XVM_LABELHDR_VERSION_LEGACY                  0x1u /* established */
#define XVM_LABENT_SIZE                              0x48u /* established */
#define XVM_LH_SIZE                                  0x8u /* established */
#define XVM_NAME_MAX                                 0x30u /* established */
#define XVM_PHYSLAB_GPT_FIRST_PARTITION_INDEX        0x0u /* established */
#define XVM_PHYSLAB_GPT_MIN_PARTITIONS               0x2u /* established */
#define XVM_PHYSLAB_MAGIC                            0x786c6162u /* established */
#define XVM_PHYSLAB_START_BLOCK                      0x1u /* established */
#define XVM_PHYSLAB_VERSION                          0x2u /* established */
#define XVM_PHYSLAB_VERSION_LEGACY                   0x1u /* established */
#define XVM_PL_LEN_DISKNAME                          0x30u /* established */
#define XVM_PL_LEN_HOSTNAME                          0x20u /* established */
#define XVM_PL_LEN_UUID                              0x10u /* established */
#define XVM_PL_OFF_DISKNAME                          0x48u /* established */
#define XVM_PL_OFF_HOSTNAME                          0x8u /* established */
#define XVM_PL_OFF_UUID                              0x28u /* established */
#define XVM_PL_SIZE                                  0x7cu /* established */
#define XVM_VE_COW_LEN_RUUID                         0x10u /* established */
#define XVM_VE_COW_OFF_RUUID                         0x38u /* established */
#define XVM_VE_LEN_CHILDUUID                         0x10u /* established */
#define XVM_VE_LEN_UUID                              0x10u /* established */
#define XVM_VE_OFF_CHILDUUID                         0x18u /* established */
#define XVM_VE_OFF_UUID                              0x8u /* established */
#define XVM_VE_SIZE                                  0x48u /* established */

#endif /* XVM_GEN_H */
