/* generated from defs/pcibr.toml by defs-gen - do not edit, change the source */
/* PCI bridge widget identification and window layouts */
#ifndef PCIBR_GEN_H
#define PCIBR_GEN_H

#include <stdint.h>


/*
 * The widget identification word.
 */
/* word WID: 32 bits; shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md */
#define PCIBR_WID_WIDTH                              32 /* established */
#define PCIBR_WID_PART_SHIFT                         12 /* established */
#define PCIBR_WID_PART_MASK                          0xffffu
#define PCIBR_WID_PART_INPLACE                       (PCIBR_WID_PART_MASK << PCIBR_WID_PART_SHIFT)
#define PCIBR_WID_PART_LAYOUT32                      49154 /* established */
#define PCIBR_WID_PART_LAYOUT32B                     53250 /* established */
#define PCIBR_WID_PART_LAYOUT64B                     53506 /* established */
#define PCIBR_WID_PART_LAYOUT64BX                    53522 /* established */
#define PCIBR_WID_PART_LAYOUT64A                     57344 /* established */
#define PCIBR_WID_PART_LAYOUT64AX                    57360 /* established */
#define PCIBR_WID_FIELD11_SHIFT                      1 /* established */
#define PCIBR_WID_FIELD11_MASK                       0x7ffu
#define PCIBR_WID_FIELD11_INPLACE                    (PCIBR_WID_FIELD11_MASK << PCIBR_WID_FIELD11_SHIFT)
#define PCIBR_WID_FIELD11_VALUE36                    36 /* established */
#define PCIBR_WID_FIELD11_VALUE54                    54 /* established */

#define PCIBR_CFG_FUNC_SHIFT                         0x8u /* established */
#define PCIBR_CFG_SLOTS_32                           0x8u /* established */
#define PCIBR_CFG_SLOT_BIAS_64                       0x1u /* established */
#define PCIBR_CFG_SLOT_SHIFT                         0xcu /* established */
#define PCIBR_CFG_WINDOW32_OFFSET                    0x20000u /* established */
#define PCIBR_INTR_ADDR32_HI                         0x11u /* established */
#define PCIBR_INTR_ADDR32_LO                         0x8u /* established */
#define PCIBR_INTR_ADDR32_OFFSET                     0x134u /* established */
#define PCIBR_INTR_ADDR32_SRC_HI                     0x2fu /* established */
#define PCIBR_INTR_ADDR32_SRC_LO                     0x26u /* established */
#define PCIBR_INTR_ADDR64_OFFSET                     0x130u /* established */
#define PCIBR_INTR_COUNT                             0x8u /* established */
#define PCIBR_INTR_STRIDE                            0x8u /* established */
#define PCIBR_WID_OFFSET                             0x4u /* established */
#define PCIBR_WINDOW_BIT_A                           0x3fu /* established */
#define PCIBR_WINDOW_BIT_B                           0x3cu /* established */
#define PCIBR_WINDOW_BIT_C                           0x39u /* established */

#endif /* PCIBR_GEN_H */
