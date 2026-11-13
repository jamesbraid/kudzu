/* generated from formats/klconfig/format.toml by defs-gen - do not edit, change the source */
/* hardware configuration board record */
#ifndef KLCONFIG_GEN_H
#define KLCONFIG_GEN_H

#include <stdint.h>


/* word BRD_W0: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_BRD_W0_WIDTH                        32 /* established */
#define KLCONFIG_BRD_W0_NEXT_SHIFT                   0 /* established */
#define KLCONFIG_BRD_W0_NEXT_MASK                    0xffffffffu
#define KLCONFIG_BRD_W0_NEXT_INPLACE                 (KLCONFIG_BRD_W0_NEXT_MASK << KLCONFIG_BRD_W0_NEXT_SHIFT)

/* word BRD_W1: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_BRD_W1_WIDTH                        32 /* inferred */
#define KLCONFIG_BRD_W1_CLASS_SHIFT                  20 /* established */
#define KLCONFIG_BRD_W1_CLASS_MASK                   0xfu
#define KLCONFIG_BRD_W1_CLASS_INPLACE                (KLCONFIG_BRD_W1_CLASS_MASK << KLCONFIG_BRD_W1_CLASS_SHIFT)
#define KLCONFIG_BRD_W1_CLASS_NODE                   1 /* established */
#define KLCONFIG_BRD_W1_CLASS_IO                     2 /* inferred */
#define KLCONFIG_BRD_W1_CLASS_ROUTER                 3 /* inferred */
#define KLCONFIG_BRD_W1_CLASS_GRAPHICS               5 /* inferred */
#define KLCONFIG_BRD_W1_TYPE_LOW_SHIFT               16 /* inferred */
#define KLCONFIG_BRD_W1_TYPE_LOW_MASK                0xfu
#define KLCONFIG_BRD_W1_TYPE_LOW_INPLACE             (KLCONFIG_BRD_W1_TYPE_LOW_MASK << KLCONFIG_BRD_W1_TYPE_LOW_SHIFT)

/* word BRD_W2: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_BRD_W2_WIDTH                        32 /* established */
#define KLCONFIG_BRD_W2_FLAGS_MASTER_BASEIO_SHIFT    21 /* established */
#define KLCONFIG_BRD_W2_FLAGS_MASTER_BASEIO_MASK     0x1u
#define KLCONFIG_BRD_W2_FLAGS_MASTER_BASEIO_INPLACE  (KLCONFIG_BRD_W2_FLAGS_MASTER_BASEIO_MASK << KLCONFIG_BRD_W2_FLAGS_MASTER_BASEIO_SHIFT)
#define KLCONFIG_BRD_W2_SLOT_SHIFT                   8 /* established */
#define KLCONFIG_BRD_W2_SLOT_MASK                    0xffu
#define KLCONFIG_BRD_W2_SLOT_INPLACE                 (KLCONFIG_BRD_W2_SLOT_MASK << KLCONFIG_BRD_W2_SLOT_SHIFT)

/* word BRD_W3: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_BRD_W3_WIDTH                        32 /* established */
#define KLCONFIG_BRD_W3_MODULE_SHIFT                 0 /* established */
#define KLCONFIG_BRD_W3_MODULE_MASK                  0xffffu
#define KLCONFIG_BRD_W3_MODULE_INPLACE               (KLCONFIG_BRD_W3_MODULE_MASK << KLCONFIG_BRD_W3_MODULE_SHIFT)

/* word BRD_W5: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_BRD_W5_WIDTH                        32 /* established */
#define KLCONFIG_BRD_W5_NUMCOMPTS_SHIFT              0 /* established */
#define KLCONFIG_BRD_W5_NUMCOMPTS_MASK               0xffu
#define KLCONFIG_BRD_W5_NUMCOMPTS_INPLACE            (KLCONFIG_BRD_W5_NUMCOMPTS_MASK << KLCONFIG_BRD_W5_NUMCOMPTS_SHIFT)

/* word BRD_W8: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_BRD_W8_WIDTH                        32 /* established */
#define KLCONFIG_BRD_W8_NASID_SHIFT                  16 /* established */
#define KLCONFIG_BRD_W8_NASID_MASK                   0xffffu
#define KLCONFIG_BRD_W8_NASID_INPLACE                (KLCONFIG_BRD_W8_NASID_MASK << KLCONFIG_BRD_W8_NASID_SHIFT)

/* word COMP_W0: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_COMP_W0_WIDTH                       32 /* established */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_SHIFT           24 /* established */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_MASK            0xffu
#define KLCONFIG_COMP_W0_STRUCT_TYPE_INPLACE         (KLCONFIG_COMP_W0_STRUCT_TYPE_MASK << KLCONFIG_COMP_W0_STRUCT_TYPE_SHIFT)
#define KLCONFIG_COMP_W0_STRUCT_TYPE_HUB             2 /* established */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_MEMORY          3 /* inferred */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_IO              5 /* inferred */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_ROUTER          9 /* inferred */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_GRAPHICS        10 /* inferred */
#define KLCONFIG_COMP_W0_FLAGS_ENABLED_SHIFT         8 /* established */
#define KLCONFIG_COMP_W0_FLAGS_ENABLED_MASK          0x1u
#define KLCONFIG_COMP_W0_FLAGS_ENABLED_INPLACE       (KLCONFIG_COMP_W0_FLAGS_ENABLED_MASK << KLCONFIG_COMP_W0_FLAGS_ENABLED_SHIFT)

/* word COMP_W6: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_COMP_W6_WIDTH                       32 /* established */
#define KLCONFIG_COMP_W6_PHYSID_SHIFT                24 /* established */
#define KLCONFIG_COMP_W6_PHYSID_MASK                 0xffu
#define KLCONFIG_COMP_W6_PHYSID_INPLACE              (KLCONFIG_COMP_W6_PHYSID_MASK << KLCONFIG_COMP_W6_PHYSID_SHIFT)

/* word CONS_W2: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_CONS_W2_WIDTH                       32 /* established */
#define KLCONFIG_CONS_W2_CONFIG_BASE_HI_SHIFT        0 /* established */
#define KLCONFIG_CONS_W2_CONFIG_BASE_HI_MASK         0xffffffffu
#define KLCONFIG_CONS_W2_CONFIG_BASE_HI_INPLACE      (KLCONFIG_CONS_W2_CONFIG_BASE_HI_MASK << KLCONFIG_CONS_W2_CONFIG_BASE_HI_SHIFT)

/* word CONS_W3: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_CONS_W3_WIDTH                       32 /* established */
#define KLCONFIG_CONS_W3_CONFIG_BASE_LO_SHIFT        0 /* established */
#define KLCONFIG_CONS_W3_CONFIG_BASE_LO_MASK         0xffffffffu
#define KLCONFIG_CONS_W3_CONFIG_BASE_LO_INPLACE      (KLCONFIG_CONS_W3_CONFIG_BASE_LO_MASK << KLCONFIG_CONS_W3_CONFIG_BASE_LO_SHIFT)

/* word CONS_W4: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_CONS_W4_WIDTH                       32 /* established */
#define KLCONFIG_CONS_W4_MEMORY_BASE_HI_SHIFT        0 /* established */
#define KLCONFIG_CONS_W4_MEMORY_BASE_HI_MASK         0xffffffffu
#define KLCONFIG_CONS_W4_MEMORY_BASE_HI_INPLACE      (KLCONFIG_CONS_W4_MEMORY_BASE_HI_MASK << KLCONFIG_CONS_W4_MEMORY_BASE_HI_SHIFT)

/* word CONS_W5: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_CONS_W5_WIDTH                       32 /* established */
#define KLCONFIG_CONS_W5_MEMORY_BASE_WIDGET_SHIFT    24 /* established */
#define KLCONFIG_CONS_W5_MEMORY_BASE_WIDGET_MASK     0xffu
#define KLCONFIG_CONS_W5_MEMORY_BASE_WIDGET_INPLACE  (KLCONFIG_CONS_W5_MEMORY_BASE_WIDGET_MASK << KLCONFIG_CONS_W5_MEMORY_BASE_WIDGET_SHIFT)
#define KLCONFIG_CONS_W5_MEMORY_BASE_REST_SHIFT      0 /* established */
#define KLCONFIG_CONS_W5_MEMORY_BASE_REST_MASK       0xffffffu
#define KLCONFIG_CONS_W5_MEMORY_BASE_REST_INPLACE    (KLCONFIG_CONS_W5_MEMORY_BASE_REST_MASK << KLCONFIG_CONS_W5_MEMORY_BASE_REST_SHIFT)

/* word CONS_W8: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_CONS_W8_WIDTH                       32 /* established */
#define KLCONFIG_CONS_W8_NASID_SHIFT                 16 /* established */
#define KLCONFIG_CONS_W8_NASID_MASK                  0xffffu
#define KLCONFIG_CONS_W8_NASID_INPLACE               (KLCONFIG_CONS_W8_NASID_MASK << KLCONFIG_CONS_W8_NASID_SHIFT)
#define KLCONFIG_CONS_W8_NPCI_SHIFT                  0 /* established */
#define KLCONFIG_CONS_W8_NPCI_MASK                   0xffu
#define KLCONFIG_CONS_W8_NPCI_INPLACE                (KLCONFIG_CONS_W8_NPCI_MASK << KLCONFIG_CONS_W8_NPCI_SHIFT)

/* word GDA_W97: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_GDA_W97_WIDTH                       32 /* established */
#define KLCONFIG_GDA_W97_PEBRICK_MARKER_SHIFT        0 /* established */
#define KLCONFIG_GDA_W97_PEBRICK_MARKER_MASK         0xffffffffu
#define KLCONFIG_GDA_W97_PEBRICK_MARKER_INPLACE      (KLCONFIG_GDA_W97_PEBRICK_MARKER_MASK << KLCONFIG_GDA_W97_PEBRICK_MARKER_SHIFT)

/* word MEM_W14: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_MEM_W14_WIDTH                       32 /* established */
#define KLCONFIG_MEM_W14_DIMM_SELECT_SHIFT           0 /* established */
#define KLCONFIG_MEM_W14_DIMM_SELECT_MASK            0xffffu
#define KLCONFIG_MEM_W14_DIMM_SELECT_INPLACE         (KLCONFIG_MEM_W14_DIMM_SELECT_MASK << KLCONFIG_MEM_W14_DIMM_SELECT_SHIFT)

/* word NODECLASS_W1: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_NODECLASS_W1_WIDTH                  32 /* established */
#define KLCONFIG_NODECLASS_W1_CLASS_SHIFT            12 /* established */
#define KLCONFIG_NODECLASS_W1_CLASS_MASK             0xfu
#define KLCONFIG_NODECLASS_W1_CLASS_INPLACE          (KLCONFIG_NODECLASS_W1_CLASS_MASK << KLCONFIG_NODECLASS_W1_CLASS_SHIFT)
#define KLCONFIG_NODECLASS_W1_CLASS_PEBRICK          12 /* established */

/* word PEPAIR_W0: 32 bits; shipped IRIX kernel binaries, per formats/klconfig/README.md */
#define KLCONFIG_PEPAIR_W0_WIDTH                     32 /* established */
#define KLCONFIG_PEPAIR_W0_PE_NASID_SHIFT            16 /* established */
#define KLCONFIG_PEPAIR_W0_PE_NASID_MASK             0xffffu
#define KLCONFIG_PEPAIR_W0_PE_NASID_INPLACE          (KLCONFIG_PEPAIR_W0_PE_NASID_MASK << KLCONFIG_PEPAIR_W0_PE_NASID_SHIFT)
#define KLCONFIG_PEPAIR_W0_HOST_NASID_SHIFT          0 /* established */
#define KLCONFIG_PEPAIR_W0_HOST_NASID_MASK           0xffffu
#define KLCONFIG_PEPAIR_W0_HOST_NASID_INPLACE        (KLCONFIG_PEPAIR_W0_HOST_NASID_MASK << KLCONFIG_PEPAIR_W0_HOST_NASID_SHIFT)

#define KLCONFIG_BRD_OFF_COMPTS                      0x24u /* established */
#define KLCONFIG_BRD_STRIDE_COMPTS                   0x4u /* established */
#define KLCONFIG_DIR_CONFIG_OFFSET_MASK              0xffffffffffull /* established */
#define KLCONFIG_DIR_OFF                             0x2000u /* established */
#define KLCONFIG_DIR_OFF_CONFIG_OFFSET               0x48u /* established */
#define KLCONFIG_GDA_COUNT_NASIDTABLE_IP27           0x40u /* established */
#define KLCONFIG_GDA_COUNT_NASIDTABLE_IP35           0x80u /* established */
#define KLCONFIG_GDA_COUNT_PEBRICK                   0x4u /* established */
#define KLCONFIG_GDA_OFF_NASIDTABLE                  0x80u /* established */
#define KLCONFIG_GDA_OFF_PEBRICK                     0x188u /* established */
#define KLCONFIG_GDA_PEBRICK_MAGIC                   0x5045626bu /* established */
#define KLCONFIG_GDA_STRIDE_NASIDTABLE               0x2u /* established */
#define KLCONFIG_GDA_STRIDE_PEBRICK                  0x4u /* established */
#define KLCONFIG_HDR_OFF_BOARD_INFO                  0x14u /* established */
#define KLCONFIG_MEM_COUNT_BNKSZ                     0x8u /* established */
#define KLCONFIG_MEM_OFF_BNKSZ                       0x3cu /* established */
#define KLCONFIG_MEM_STRIDE_BNKSZ                    0x2u /* established */
#define KLCONFIG_NODECLASS_OFF                       0x4000000u /* established */
#define KLCONFIG_NODECLASS_SPACE_LOCAL               0x10000000u /* established */
#define KLCONFIG_NODECLASS_SPACE_REMOTE              0x30000000u /* established */

#endif /* KLCONFIG_GEN_H */
