/* generated from formats/tg/format.toml by defs-gen - do not edit, change the source */
/* gigabit Ethernet controller match table of the Origin 300/350 driver */
#ifndef TG_GEN_H
#define TG_GEN_H

#include <stdint.h>


/* word ROW_W0: 32 bits; shipped IRIX kernel binaries, per docs/ip35-tg-driver.md */
#define TG_ROW_W0_WIDTH                              32 /* established */
#define TG_ROW_W0_VENDOR_SHIFT                       16 /* established */
#define TG_ROW_W0_VENDOR_MASK                        0xffffu
#define TG_ROW_W0_VENDOR_INPLACE                     (TG_ROW_W0_VENDOR_MASK << TG_ROW_W0_VENDOR_SHIFT)
#define TG_ROW_W0_DEVICE_SHIFT                       0 /* established */
#define TG_ROW_W0_DEVICE_MASK                        0xffffu
#define TG_ROW_W0_DEVICE_INPLACE                     (TG_ROW_W0_DEVICE_MASK << TG_ROW_W0_DEVICE_SHIFT)

/* word ROW_W1: 32 bits; shipped IRIX kernel binaries, per docs/ip35-tg-driver.md */
#define TG_ROW_W1_WIDTH                              32 /* established */
#define TG_ROW_W1_SUBSYS_VENDOR_SHIFT                16 /* established */
#define TG_ROW_W1_SUBSYS_VENDOR_MASK                 0xffffu
#define TG_ROW_W1_SUBSYS_VENDOR_INPLACE              (TG_ROW_W1_SUBSYS_VENDOR_MASK << TG_ROW_W1_SUBSYS_VENDOR_SHIFT)
#define TG_ROW_W1_SUBSYS_DEVICE_SHIFT                0 /* established */
#define TG_ROW_W1_SUBSYS_DEVICE_MASK                 0xffffu
#define TG_ROW_W1_SUBSYS_DEVICE_INPLACE              (TG_ROW_W1_SUBSYS_DEVICE_MASK << TG_ROW_W1_SUBSYS_DEVICE_SHIFT)

#define TG_CFG_OFF_DEVICE                            0x0u /* established */
#define TG_CFG_OFF_SUBSYS_DEVICE                     0x2eu /* established */
#define TG_CFG_OFF_SUBSYS_VENDOR                     0x2cu /* established */
#define TG_CFG_OFF_VENDOR                            0x2u /* established */
#define TG_MATCH_ROW_COUNT                           0x5u /* established */
#define TG_MATCH_ROW_STRIDE                          0x10u /* established */
#define TG_PCI_DEVICE_1645                           0x1645u /* established */
#define TG_PCI_DEVICE_1648                           0x1648u /* established */
#define TG_PCI_DEVICE_16A8                           0x16a8u /* established */
#define TG_PCI_SUBSYS_DEVICE_FIRST                   0x8010u /* established */
#define TG_PCI_SUBSYS_VENDOR                         0x10a9u /* established */
#define TG_PCI_VENDOR                                0x14e4u /* established */

#endif /* TG_GEN_H */
