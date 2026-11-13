/* generated from formats/volhdr/format.toml by defs-gen - do not edit, change the source */
/* SGI disk volume header */
#ifndef DVH_GEN_H
#define DVH_GEN_H

#include <stdint.h>


/* word DIRENT_W2: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_DIRENT_W2_WIDTH                          32 /* established */
#define DVH_DIRENT_W2_START_BLOCK_SHIFT              0 /* established */
#define DVH_DIRENT_W2_START_BLOCK_MASK               0xffffffffu
#define DVH_DIRENT_W2_START_BLOCK_INPLACE            (DVH_DIRENT_W2_START_BLOCK_MASK << DVH_DIRENT_W2_START_BLOCK_SHIFT)

/* word DIRENT_W3: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_DIRENT_W3_WIDTH                          32 /* established */
#define DVH_DIRENT_W3_LENGTH_BYTES_SHIFT             0 /* established */
#define DVH_DIRENT_W3_LENGTH_BYTES_MASK              0xffffffffu
#define DVH_DIRENT_W3_LENGTH_BYTES_INPLACE           (DVH_DIRENT_W3_LENGTH_BYTES_MASK << DVH_DIRENT_W3_LENGTH_BYTES_SHIFT)

/* word HDR_W0: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W0_WIDTH                             32 /* established */
#define DVH_HDR_W0_MAGIC_SHIFT                       0 /* established */
#define DVH_HDR_W0_MAGIC_MASK                        0xffffffffu
#define DVH_HDR_W0_MAGIC_INPLACE                     (DVH_HDR_W0_MAGIC_MASK << DVH_HDR_W0_MAGIC_SHIFT)

/* word HDR_W1: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W1_WIDTH                             32 /* established */
#define DVH_HDR_W1_ROOT_SHIFT                        16 /* established */
#define DVH_HDR_W1_ROOT_MASK                         0xffffu
#define DVH_HDR_W1_ROOT_INPLACE                      (DVH_HDR_W1_ROOT_MASK << DVH_HDR_W1_ROOT_SHIFT)
#define DVH_HDR_W1_SWAP_SHIFT                        0 /* established */
#define DVH_HDR_W1_SWAP_MASK                         0xffffu
#define DVH_HDR_W1_SWAP_INPLACE                      (DVH_HDR_W1_SWAP_MASK << DVH_HDR_W1_SWAP_SHIFT)

/* word HDR_W10: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W10_WIDTH                            32 /* established */
#define DVH_HDR_W10_DP_SECBYTES_SHIFT                16 /* established */
#define DVH_HDR_W10_DP_SECBYTES_MASK                 0xffffu
#define DVH_HDR_W10_DP_SECBYTES_INPLACE              (DVH_HDR_W10_DP_SECBYTES_MASK << DVH_HDR_W10_DP_SECBYTES_SHIFT)

/* word HDR_W11: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W11_WIDTH                            32 /* inferred */
#define DVH_HDR_W11_DP_FLAGS_SHIFT                   0 /* inferred */
#define DVH_HDR_W11_DP_FLAGS_MASK                    0xffffffffu
#define DVH_HDR_W11_DP_FLAGS_INPLACE                 (DVH_HDR_W11_DP_FLAGS_MASK << DVH_HDR_W11_DP_FLAGS_SHIFT)

/* word HDR_W126: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W126_WIDTH                           32 /* established */
#define DVH_HDR_W126_CHECKSUM_SHIFT                  0 /* established */
#define DVH_HDR_W126_CHECKSUM_MASK                   0xffffffffu
#define DVH_HDR_W126_CHECKSUM_INPLACE                (DVH_HDR_W126_CHECKSUM_MASK << DVH_HDR_W126_CHECKSUM_SHIFT)

/* word HDR_W17: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W17_WIDTH                            32 /* established */
#define DVH_HDR_W17_DP_CAPACITY_SHIFT                0 /* established */
#define DVH_HDR_W17_DP_CAPACITY_MASK                 0xffffffffu
#define DVH_HDR_W17_DP_CAPACITY_INPLACE              (DVH_HDR_W17_DP_CAPACITY_MASK << DVH_HDR_W17_DP_CAPACITY_SHIFT)

/* word HDR_W7: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W7_WIDTH                             32 /* unknown */
#define DVH_HDR_W7_DP_CYLINDERS_SHIFT                16 /* inferred */
#define DVH_HDR_W7_DP_CYLINDERS_MASK                 0xffffu
#define DVH_HDR_W7_DP_CYLINDERS_INPLACE              (DVH_HDR_W7_DP_CYLINDERS_MASK << DVH_HDR_W7_DP_CYLINDERS_SHIFT)
#define DVH_HDR_W7_DP_HEAD_OFFSET_SHIFT              0 /* unknown */
#define DVH_HDR_W7_DP_HEAD_OFFSET_MASK               0xffffu
#define DVH_HDR_W7_DP_HEAD_OFFSET_INPLACE            (DVH_HDR_W7_DP_HEAD_OFFSET_MASK << DVH_HDR_W7_DP_HEAD_OFFSET_SHIFT)

/* word HDR_W8: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W8_WIDTH                             32 /* unknown */
#define DVH_HDR_W8_DP_HEADS_SHIFT                    16 /* inferred */
#define DVH_HDR_W8_DP_HEADS_MASK                     0xffffu
#define DVH_HDR_W8_DP_HEADS_INPLACE                  (DVH_HDR_W8_DP_HEADS_MASK << DVH_HDR_W8_DP_HEADS_SHIFT)
#define DVH_HDR_W8_DP_TAG_DEPTH_SHIFT                8 /* established */
#define DVH_HDR_W8_DP_TAG_DEPTH_MASK                 0xffu
#define DVH_HDR_W8_DP_TAG_DEPTH_INPLACE              (DVH_HDR_W8_DP_TAG_DEPTH_MASK << DVH_HDR_W8_DP_TAG_DEPTH_SHIFT)
#define DVH_HDR_W8_DP_CYL_HIGH_SHIFT                 0 /* unknown */
#define DVH_HDR_W8_DP_CYL_HIGH_MASK                  0xffu
#define DVH_HDR_W8_DP_CYL_HIGH_INPLACE               (DVH_HDR_W8_DP_CYL_HIGH_MASK << DVH_HDR_W8_DP_CYL_HIGH_SHIFT)

/* word HDR_W9: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_HDR_W9_WIDTH                             32 /* inferred */
#define DVH_HDR_W9_DP_SECTORS_SHIFT                  0 /* inferred */
#define DVH_HDR_W9_DP_SECTORS_MASK                   0xffffu
#define DVH_HDR_W9_DP_SECTORS_INPLACE                (DVH_HDR_W9_DP_SECTORS_MASK << DVH_HDR_W9_DP_SECTORS_SHIFT)

/* word PART_W0: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_PART_W0_WIDTH                            32 /* established */
#define DVH_PART_W0_NBLKS_SHIFT                      0 /* established */
#define DVH_PART_W0_NBLKS_MASK                       0xffffffffu
#define DVH_PART_W0_NBLKS_INPLACE                    (DVH_PART_W0_NBLKS_MASK << DVH_PART_W0_NBLKS_SHIFT)

/* word PART_W1: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_PART_W1_WIDTH                            32 /* established */
#define DVH_PART_W1_FIRST_SHIFT                      0 /* established */
#define DVH_PART_W1_FIRST_MASK                       0xffffffffu
#define DVH_PART_W1_FIRST_INPLACE                    (DVH_PART_W1_FIRST_MASK << DVH_PART_W1_FIRST_SHIFT)

/* word PART_W2: 32 bits; shipped IRIX binaries, per formats/volhdr/README.md */
#define DVH_PART_W2_WIDTH                            32 /* established */
#define DVH_PART_W2_TYPE_SHIFT                       0 /* established */
#define DVH_PART_W2_TYPE_MASK                        0xffffffffu
#define DVH_PART_W2_TYPE_INPLACE                     (DVH_PART_W2_TYPE_MASK << DVH_PART_W2_TYPE_SHIFT)
#define DVH_PART_W2_TYPE_VOLHDR                      0 /* established */
#define DVH_PART_W2_TYPE_TRKREPL                     1 /* inferred */
#define DVH_PART_W2_TYPE_SECREPL                     2 /* inferred */
#define DVH_PART_W2_TYPE_RAW                         3 /* established */
#define DVH_PART_W2_TYPE_BSD                         4 /* inferred */
#define DVH_PART_W2_TYPE_SYSV                        5 /* inferred */
#define DVH_PART_W2_TYPE_VOLUME                      6 /* established */
#define DVH_PART_W2_TYPE_EFS                         7 /* established */
#define DVH_PART_W2_TYPE_LVOL                        8 /* established */
#define DVH_PART_W2_TYPE_RLVOL                       9 /* established */
#define DVH_PART_W2_TYPE_XFS                         10 /* established */
#define DVH_PART_W2_TYPE_XFSLOG                      11 /* established */
#define DVH_PART_W2_TYPE_XLV                         12 /* established */
#define DVH_PART_W2_TYPE_XVM                         13 /* established */
#define DVH_PART_W2_TYPE_VXVM                        14 /* established */

#define DVH_BOOTFILE_OFF                             0x8u /* established */
#define DVH_CSUM_OFF                                 0x1f8u /* established */
#define DVH_DIRENT_LEN_NAME                          0x8u /* established */
#define DVH_DIRENT_OFF_NAME                          0x0u /* established */
#define DVH_DIRENT_SIZE                              0x10u /* established */
#define DVH_DIR_COUNT                                0xfu /* established */
#define DVH_DIR_NAME_LEN                             0x8u /* established */
#define DVH_DIR_OFF                                  0x48u /* established */
#define DVH_DIR_STRIDE                               0x10u /* established */
#define DVH_DP_BYTES                                 0x30u /* established */
#define DVH_DP_FLAG_TAGGED                           0x40u /* inferred */
#define DVH_DP_OFF                                   0x18u /* established */
#define DVH_HDR_LEN_BOOTFILE                         0x10u /* established */
#define DVH_HDR_OFF_BOOTFILE                         0x8u /* established */
#define DVH_HDR_SIZE                                 0x200u /* established */
#define DVH_MAGIC                                    0xbe5a941u /* established */
#define DVH_PART_COUNT                               0x10u /* established */
#define DVH_PART_OFF                                 0x138u /* established */
#define DVH_PART_SIZE                                0xcu /* established */
#define DVH_PART_STRIDE                              0xcu /* established */
#define DVH_SIZE                                     0x200u /* established */
#define DVH_SLOT_ROOT                                0x0u /* established */
#define DVH_SLOT_SWAP                                0x1u /* established */
#define DVH_SLOT_VOLHDR                              0x8u /* established */
#define DVH_SLOT_WHOLEDISK                           0xau /* established */

#endif /* DVH_GEN_H */
