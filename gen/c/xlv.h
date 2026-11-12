/* generated from defs/xlv.toml by defs-gen - do not edit, change the source */
/* XLV logical volume manager on-disk label structures */
#ifndef XLV_GEN_H
#define XLV_GEN_H

#include <stdint.h>


/* word E_W0: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W0_WIDTH                               32 /* established */
#define XLV_E_W0_VOL_FLAGS_SHIFT                     0 /* established */
#define XLV_E_W0_VOL_FLAGS_MASK                      0xffffffffu
#define XLV_E_W0_VOL_FLAGS_INPLACE                   (XLV_E_W0_VOL_FLAGS_MASK << XLV_E_W0_VOL_FLAGS_SHIFT)

/* word E_W25: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W25_WIDTH                              32 /* established */
#define XLV_E_W25_SV_FLAGS_HI_NO_ERR_RETRY_SHIFT     31 /* established */
#define XLV_E_W25_SV_FLAGS_HI_NO_ERR_RETRY_MASK      0x1u
#define XLV_E_W25_SV_FLAGS_HI_NO_ERR_RETRY_INPLACE   (XLV_E_W25_SV_FLAGS_HI_NO_ERR_RETRY_MASK << XLV_E_W25_SV_FLAGS_HI_NO_ERR_RETRY_SHIFT)
#define XLV_E_W25_SV_FLAGS_HI_OTHER_SHIFT            16 /* unknown */
#define XLV_E_W25_SV_FLAGS_HI_OTHER_MASK             0x7fffu
#define XLV_E_W25_SV_FLAGS_HI_OTHER_INPLACE          (XLV_E_W25_SV_FLAGS_HI_OTHER_MASK << XLV_E_W25_SV_FLAGS_HI_OTHER_SHIFT)
#define XLV_E_W25_SV_FLAGS_HI_LOW_SHIFT              0 /* established */
#define XLV_E_W25_SV_FLAGS_HI_LOW_MASK               0xffffu
#define XLV_E_W25_SV_FLAGS_HI_LOW_INPLACE            (XLV_E_W25_SV_FLAGS_HI_LOW_MASK << XLV_E_W25_SV_FLAGS_HI_LOW_SHIFT)

/* word E_W27: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W27_WIDTH                              32 /* established */
#define XLV_E_W27_TIME_SHIFT                         0 /* established */
#define XLV_E_W27_TIME_MASK                          0xffffffffu
#define XLV_E_W27_TIME_INPLACE                       (XLV_E_W27_TIME_MASK << XLV_E_W27_TIME_SHIFT)

/* word E_W28: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W28_WIDTH                              32 /* established */
#define XLV_E_W28_VOL_ATTR16_SHIFT                   0 /* established */
#define XLV_E_W28_VOL_ATTR16_MASK                    0xffffffffu
#define XLV_E_W28_VOL_ATTR16_INPLACE                 (XLV_E_W28_VOL_ATTR16_MASK << XLV_E_W28_VOL_ATTR16_SHIFT)

/* word E_W29: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W29_WIDTH                              32 /* established */
#define XLV_E_W29_SV_TYPE_SHIFT                      24 /* established */
#define XLV_E_W29_SV_TYPE_MASK                       0xffu
#define XLV_E_W29_SV_TYPE_INPLACE                    (XLV_E_W29_SV_TYPE_MASK << XLV_E_W29_SV_TYPE_SHIFT)
#define XLV_E_W29_SV_TYPE_LOG                        1 /* established */
#define XLV_E_W29_SV_TYPE_DATA                       2 /* established */
#define XLV_E_W29_SV_TYPE_RT                         3 /* established */
#define XLV_E_W29_SV_DEPTH_SHIFT                     0 /* established */
#define XLV_E_W29_SV_DEPTH_MASK                      0xffffu
#define XLV_E_W29_SV_DEPTH_INPLACE                   (XLV_E_W29_SV_DEPTH_MASK << XLV_E_W29_SV_DEPTH_SHIFT)

/* word E_W42: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W42_WIDTH                              32 /* established */
#define XLV_E_W42_SV_NPLEX_SHIFT                     24 /* established */
#define XLV_E_W42_SV_NPLEX_MASK                      0xffu
#define XLV_E_W42_SV_NPLEX_INPLACE                   (XLV_E_W42_SV_NPLEX_MASK << XLV_E_W42_SV_NPLEX_SHIFT)
#define XLV_E_W42_PLEX_INDEX_SHIFT                   16 /* established */
#define XLV_E_W42_PLEX_INDEX_MASK                    0xffu
#define XLV_E_W42_PLEX_INDEX_INPLACE                 (XLV_E_W42_PLEX_INDEX_MASK << XLV_E_W42_PLEX_INDEX_SHIFT)
#define XLV_E_W42_PLEX_NVE_SHIFT                     0 /* established */
#define XLV_E_W42_PLEX_NVE_MASK                      0xffffu
#define XLV_E_W42_PLEX_NVE_INPLACE                   (XLV_E_W42_PLEX_NVE_MASK << XLV_E_W42_PLEX_NVE_SHIFT)

/* word E_W43: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W43_WIDTH                              32 /* established */
#define XLV_E_W43_PLEX_FLAGS_SHIFT                   16 /* established */
#define XLV_E_W43_PLEX_FLAGS_MASK                    0xffffu
#define XLV_E_W43_PLEX_FLAGS_INPLACE                 (XLV_E_W43_PLEX_FLAGS_MASK << XLV_E_W43_PLEX_FLAGS_SHIFT)

/* word E_W56: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W56_WIDTH                              32 /* established */
#define XLV_E_W56_VE_STATE_SHIFT                     24 /* established */
#define XLV_E_W56_VE_STATE_MASK                      0xffu
#define XLV_E_W56_VE_STATE_INPLACE                   (XLV_E_W56_VE_STATE_MASK << XLV_E_W56_VE_STATE_SHIFT)
#define XLV_E_W56_VE_STATE_EMPTY                     0 /* established */
#define XLV_E_W56_VE_STATE_CLEAN                     1 /* established */
#define XLV_E_W56_VE_STATE_ACTIVE                    2 /* established */
#define XLV_E_W56_VE_STATE_STALE                     3 /* established */
#define XLV_E_W56_VE_STATE_OFFLINE                   4 /* established */
#define XLV_E_W56_VE_STATE_INCOMPLETE                5 /* established */
#define XLV_E_W56_VE_TYPE_SHIFT                      16 /* established */
#define XLV_E_W56_VE_TYPE_MASK                       0xffu
#define XLV_E_W56_VE_TYPE_INPLACE                    (XLV_E_W56_VE_TYPE_MASK << XLV_E_W56_VE_TYPE_SHIFT)
#define XLV_E_W56_VE_TYPE_STRIPE                     1 /* established */
#define XLV_E_W56_VE_TYPE_CONCAT                     2 /* established */
#define XLV_E_W56_VE_NPARTS_SHIFT                    8 /* established */
#define XLV_E_W56_VE_NPARTS_MASK                     0xffu
#define XLV_E_W56_VE_NPARTS_INPLACE                  (XLV_E_W56_VE_NPARTS_MASK << XLV_E_W56_VE_NPARTS_SHIFT)
#define XLV_E_W56_VE_INDEX_SHIFT                     0 /* established */
#define XLV_E_W56_VE_INDEX_MASK                      0xffu
#define XLV_E_W56_VE_INDEX_INPLACE                   (XLV_E_W56_VE_INDEX_MASK << XLV_E_W56_VE_INDEX_SHIFT)

/* word E_W57: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W57_WIDTH                              32 /* established */
#define XLV_E_W57_VE_STRIPE_UNIT_SHIFT               16 /* established */
#define XLV_E_W57_VE_STRIPE_UNIT_MASK                0xffffu
#define XLV_E_W57_VE_STRIPE_UNIT_INPLACE             (XLV_E_W57_VE_STRIPE_UNIT_MASK << XLV_E_W57_VE_STRIPE_UNIT_SHIFT)

/* word E_W58: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W58_WIDTH                              32 /* established */
#define XLV_E_W58_VE_START_HI_SHIFT                  0 /* established */
#define XLV_E_W58_VE_START_HI_MASK                   0xffffffffu
#define XLV_E_W58_VE_START_HI_INPLACE                (XLV_E_W58_VE_START_HI_MASK << XLV_E_W58_VE_START_HI_SHIFT)

/* word E_W59: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W59_WIDTH                              32 /* established */
#define XLV_E_W59_VE_START_LO_SHIFT                  0 /* established */
#define XLV_E_W59_VE_START_LO_MASK                   0xffffffffu
#define XLV_E_W59_VE_START_LO_INPLACE                (XLV_E_W59_VE_START_LO_MASK << XLV_E_W59_VE_START_LO_SHIFT)

/* word E_W60: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W60_WIDTH                              32 /* established */
#define XLV_E_W60_VE_END_HI_SHIFT                    0 /* established */
#define XLV_E_W60_VE_END_HI_MASK                     0xffffffffu
#define XLV_E_W60_VE_END_HI_INPLACE                  (XLV_E_W60_VE_END_HI_MASK << XLV_E_W60_VE_END_HI_SHIFT)

/* word E_W61: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W61_WIDTH                              32 /* established */
#define XLV_E_W61_VE_END_LO_SHIFT                    0 /* established */
#define XLV_E_W61_VE_END_LO_MASK                     0xffffffffu
#define XLV_E_W61_VE_END_LO_INPLACE                  (XLV_E_W61_VE_END_LO_MASK << XLV_E_W61_VE_END_LO_SHIFT)

/* word E_W66: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W66_WIDTH                              32 /* established */
#define XLV_E_W66_PART_DEV_SHIFT                     0 /* established */
#define XLV_E_W66_PART_DEV_MASK                      0xffffffffu
#define XLV_E_W66_PART_DEV_INPLACE                   (XLV_E_W66_PART_DEV_MASK << XLV_E_W66_PART_DEV_SHIFT)

/* word E_W67: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W67_WIDTH                              32 /* established */
#define XLV_E_W67_PART_SIZE_SHIFT                    0 /* established */
#define XLV_E_W67_PART_SIZE_MASK                     0xffffffffu
#define XLV_E_W67_PART_SIZE_INPLACE                  (XLV_E_W67_PART_SIZE_MASK << XLV_E_W67_PART_SIZE_SHIFT)

/* word E_W68: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_E_W68_WIDTH                              32 /* established */
#define XLV_E_W68_PART_INDEX_SHIFT                   24 /* established */
#define XLV_E_W68_PART_INDEX_MASK                    0xffu
#define XLV_E_W68_PART_INDEX_INPLACE                 (XLV_E_W68_PART_INDEX_MASK << XLV_E_W68_PART_INDEX_SHIFT)

/* word LBL_W0: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_LBL_W0_WIDTH                             32 /* established */
#define XLV_LBL_W0_MAGIC_SHIFT                       0 /* established */
#define XLV_LBL_W0_MAGIC_MASK                        0xffffffffu
#define XLV_LBL_W0_MAGIC_INPLACE                     (XLV_LBL_W0_MAGIC_MASK << XLV_LBL_W0_MAGIC_SHIFT)

/* word LBL_W1: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_LBL_W1_WIDTH                             32 /* established */
#define XLV_LBL_W1_VERSION_SHIFT                     16 /* established */
#define XLV_LBL_W1_VERSION_MASK                      0xffffu
#define XLV_LBL_W1_VERSION_INPLACE                   (XLV_LBL_W1_VERSION_MASK << XLV_LBL_W1_VERSION_SHIFT)
#define XLV_LBL_W1_NPARTS_SHIFT                      8 /* established */
#define XLV_LBL_W1_NPARTS_MASK                       0xffu
#define XLV_LBL_W1_NPARTS_INPLACE                    (XLV_LBL_W1_NPARTS_MASK << XLV_LBL_W1_NPARTS_SHIFT)
#define XLV_LBL_W1_RESERVED_SHIFT                    0 /* unknown */
#define XLV_LBL_W1_RESERVED_MASK                     0xffu
#define XLV_LBL_W1_RESERVED_INPLACE                  (XLV_LBL_W1_RESERVED_MASK << XLV_LBL_W1_RESERVED_SHIFT)

/* word LBL_W2: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_LBL_W2_WIDTH                             32 /* unknown */
#define XLV_LBL_W2_TIMESTAMP_HI_SHIFT                0 /* unknown */
#define XLV_LBL_W2_TIMESTAMP_HI_MASK                 0xffffffffu
#define XLV_LBL_W2_TIMESTAMP_HI_INPLACE              (XLV_LBL_W2_TIMESTAMP_HI_MASK << XLV_LBL_W2_TIMESTAMP_HI_SHIFT)

/* word LBL_W3: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_LBL_W3_WIDTH                             32 /* unknown */
#define XLV_LBL_W3_TIMESTAMP_LO_SHIFT                0 /* unknown */
#define XLV_LBL_W3_TIMESTAMP_LO_MASK                 0xffffffffu
#define XLV_LBL_W3_TIMESTAMP_LO_INPLACE              (XLV_LBL_W3_TIMESTAMP_LO_MASK << XLV_LBL_W3_TIMESTAMP_LO_SHIFT)

/* word LBL_W4: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_LBL_W4_WIDTH                             32 /* established */
#define XLV_LBL_W4_TRANS_ID_HI_SHIFT                 0 /* established */
#define XLV_LBL_W4_TRANS_ID_HI_MASK                  0xffffffffu
#define XLV_LBL_W4_TRANS_ID_HI_INPLACE               (XLV_LBL_W4_TRANS_ID_HI_MASK << XLV_LBL_W4_TRANS_ID_HI_SHIFT)

/* word LBL_W5: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_LBL_W5_WIDTH                             32 /* established */
#define XLV_LBL_W5_TRANS_ID_LO_SHIFT                 0 /* established */
#define XLV_LBL_W5_TRANS_ID_LO_MASK                  0xffffffffu
#define XLV_LBL_W5_TRANS_ID_LO_INPLACE               (XLV_LBL_W5_TRANS_ID_LO_MASK << XLV_LBL_W5_TRANS_ID_LO_SHIFT)

/* word MASTER_W0: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_MASTER_W0_WIDTH                          32 /* established */
#define XLV_MASTER_W0_SEQ_HI_SHIFT                   0 /* established */
#define XLV_MASTER_W0_SEQ_HI_MASK                    0xffffffffu
#define XLV_MASTER_W0_SEQ_HI_INPLACE                 (XLV_MASTER_W0_SEQ_HI_MASK << XLV_MASTER_W0_SEQ_HI_SHIFT)

/* word MASTER_W1: 32 bits; shipped IRIX binaries, per docs/formats/xlv.md */
#define XLV_MASTER_W1_WIDTH                          32 /* established */
#define XLV_MASTER_W1_SEQ_LO_REST_SHIFT              1 /* established */
#define XLV_MASTER_W1_SEQ_LO_REST_MASK               0x7fffffffu
#define XLV_MASTER_W1_SEQ_LO_REST_INPLACE            (XLV_MASTER_W1_SEQ_LO_REST_MASK << XLV_MASTER_W1_SEQ_LO_REST_SHIFT)
#define XLV_MASTER_W1_SEQ_LO_SLOT_SHIFT              0 /* established */
#define XLV_MASTER_W1_SEQ_LO_SLOT_MASK               0x1u
#define XLV_MASTER_W1_SEQ_LO_SLOT_INPLACE            (XLV_MASTER_W1_SEQ_LO_SLOT_MASK << XLV_MASTER_W1_SEQ_LO_SLOT_SHIFT)

#define XLV_ENTRY_OFF                                0x120u /* established */
#define XLV_ENTRY_SIZE                               0x118u /* established */
#define XLV_E_LEN_DATA_SV_UUID                       0x10u /* established */
#define XLV_E_LEN_LOG_SV_UUID                        0x10u /* established */
#define XLV_E_LEN_PART_UUID                          0x10u /* established */
#define XLV_E_LEN_PLEX_NAME                          0x20u /* established */
#define XLV_E_LEN_PLEX_UUID                          0x10u /* established */
#define XLV_E_LEN_RT_SV_UUID                         0x10u /* established */
#define XLV_E_LEN_VE_NAME                            0x20u /* established */
#define XLV_E_LEN_VE_UUID                            0x10u /* established */
#define XLV_E_LEN_VOL_NAME                           0x20u /* established */
#define XLV_E_LEN_VOL_UUID                           0x10u /* established */
#define XLV_E_OFF_DATA_SV_UUID                       0x24u /* established */
#define XLV_E_OFF_LOG_SV_UUID                        0x14u /* established */
#define XLV_E_OFF_PART_UUID                          0xf8u /* established */
#define XLV_E_OFF_PLEX_NAME                          0x88u /* established */
#define XLV_E_OFF_PLEX_UUID                          0x78u /* established */
#define XLV_E_OFF_RT_SV_UUID                         0x34u /* established */
#define XLV_E_OFF_VE_NAME                            0xc0u /* established */
#define XLV_E_OFF_VE_UUID                            0xb0u /* established */
#define XLV_E_OFF_VOL_NAME                           0x44u /* established */
#define XLV_E_OFF_VOL_UUID                           0x4u /* established */
#define XLV_E_SIZE                                   0x118u /* established */
#define XLV_HDR_SIZE                                 0x120u /* established */
#define XLV_LABEL_BYTES                              0x12a0u /* established */
#define XLV_LABEL_MAGIC                              0x11201989u /* established */
#define XLV_LABEL_VERSION                            0x2u /* established */
#define XLV_LBL_LEN_NODENAME                         0x108u /* established */
#define XLV_LBL_OFF_NODENAME                         0x18u /* established */
#define XLV_LBL_SIZE                                 0x120u /* established */
#define XLV_MASTER_BYTES                             0x200u /* established */
#define XLV_NAME_LEN                                 0x20u /* established */
#define XLV_NODENAME_COPY                            0x102u /* established */
#define XLV_NODENAME_LEN                             0x108u /* established */
#define XLV_NPART                                    0x10u /* established */
#define XLV_PTYPE_EFS                                0x7u /* established */
#define XLV_PTYPE_EFS_LEGACY                         0x5u /* established */
#define XLV_PTYPE_RAW                                0x3u /* established */
#define XLV_PTYPE_VOLHDR                             0x0u /* established */
#define XLV_PTYPE_VOLUME                             0x6u /* established */
#define XLV_PTYPE_VXVM                               0xeu /* established */
#define XLV_PTYPE_XFS                                0xau /* established */
#define XLV_PTYPE_XFSLOG                             0xbu /* established */
#define XLV_PTYPE_XLV                                0xcu /* established */
#define XLV_PTYPE_XVM                                0xdu /* established */

#endif /* XLV_GEN_H */
