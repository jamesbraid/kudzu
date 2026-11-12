/* generated from defs/nic.toml by defs-gen - do not edit, change the source */
/* bit-serial access to the one-wire identity parts */
#ifndef NIC_GEN_H
#define NIC_GEN_H

#include <stdint.h>


/*
 * The I/O controller's general-purpose control word, written through its set
 * alias. Before reading the address part the routine stores 0x200000 to the
 * register address its caller passes, when that address is not null; with a
 * null address it prints that it is bypassing the enable and goes on. The word
 * is written whole, so bit 21 is the only bit the routine sets. That the store
 * enables the one-wire path to the part is read from the routine's own message,
 * and that a set alias leaves other bits alone is inferred from its name.
 */
/* word GPCR_S: 32 bits; shipped IRIX kernel binaries, per docs/nic.md */
#define NIC_GPCR_S_WIDTH                             32 /* inferred */
#define NIC_GPCR_S_NIC_ENABLE_SHIFT                  21 /* inferred */
#define NIC_GPCR_S_NIC_ENABLE_MASK                   0x1u
#define NIC_GPCR_S_NIC_ENABLE_INPLACE                (NIC_GPCR_S_NIC_ENABLE_MASK << NIC_GPCR_S_NIC_ENABLE_SHIFT)

/*
 * The 32-bit microlan control word.
 */
/* word MCR: 32 bits; shipped IRIX kernel binaries, per docs/nic.md */
#define NIC_MCR_WIDTH                                32 /* established */
#define NIC_MCR_PULSE_SHIFT                          10 /* established */
#define NIC_MCR_PULSE_MASK                           0x1u
#define NIC_MCR_PULSE_INPLACE                        (NIC_MCR_PULSE_MASK << NIC_MCR_PULSE_SHIFT)
#define NIC_MCR_SAMPLE_SHIFT                         2 /* established */
#define NIC_MCR_SAMPLE_MASK                          0x1u
#define NIC_MCR_SAMPLE_INPLACE                       (NIC_MCR_SAMPLE_MASK << NIC_MCR_SAMPLE_SHIFT)
#define NIC_MCR_DONE_SHIFT                           1 /* established */
#define NIC_MCR_DONE_MASK                            0x1u
#define NIC_MCR_DONE_INPLACE                         (NIC_MCR_DONE_MASK << NIC_MCR_DONE_SHIFT)
#define NIC_MCR_RESULT_SHIFT                         0 /* established */
#define NIC_MCR_RESULT_MASK                          0x1u
#define NIC_MCR_RESULT_INPLACE                       (NIC_MCR_RESULT_MASK << NIC_MCR_RESULT_SHIFT)

#define NIC_GPCR_S_NIC_ENABLE_WORD                   0x200000u /* established */
#define NIC_HUB_ACCESS_NODE_SHIFT                    0x21u /* established */
#define NIC_HUB_ACCESS_OFFSET                        0xe04030u /* established */
#define NIC_HUB_ACCESS_PHYS_LOW                      0x1e04030u /* established */
#define NIC_MAC_CRC16_RESIDUE                        0xb001u /* established */
#define NIC_MAC_ID_BYTE0                             0x8du /* established */
#define NIC_MAC_ID_BYTE1                             0xau /* established */
#define NIC_MAC_RECORD_LEN                           0xeu /* established */

#endif /* NIC_GEN_H */
