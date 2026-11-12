/* generated from defs/hubpi.toml by defs-gen - do not edit, change the source */
/* node hub interrupt pending and mask registers */
#ifndef HUBPI_GEN_H
#define HUBPI_GEN_H

#include <stdint.h>


/* int_pend_mod: w64, reset unknown; shipped IRIX kernel binaries, per docs/interrupts.md */
#define HUBPI_INT_PEND_MOD_OFFSET                    0x90u /* inferred */
#define HUBPI_INT_PEND_MOD_SET_SHIFT                 8 /* inferred */
#define HUBPI_INT_PEND_MOD_SET_MASK                  0x1ull
#define HUBPI_INT_PEND_MOD_SET_INPLACE               (HUBPI_INT_PEND_MOD_SET_MASK << HUBPI_INT_PEND_MOD_SET_SHIFT)
#define HUBPI_INT_PEND_MOD_LEVEL_SHIFT               0 /* established */
#define HUBPI_INT_PEND_MOD_LEVEL_MASK                0x7full
#define HUBPI_INT_PEND_MOD_LEVEL_INPLACE             (HUBPI_INT_PEND_MOD_LEVEL_MASK << HUBPI_INT_PEND_MOD_LEVEL_SHIFT)

/* int_pend0: r64, reset unknown; shipped IRIX kernel binaries, per docs/interrupts.md */
#define HUBPI_INT_PEND0_OFFSET                       0x98u /* established */

/* int_pend1: r64, reset unknown; shipped IRIX kernel binaries, per docs/interrupts.md */
#define HUBPI_INT_PEND1_OFFSET                       0xa0u /* established */

/* int_mask0_a: rw64, reset unknown; shipped IRIX kernel binaries, per docs/interrupts.md */
#define HUBPI_INT_MASK0_A_OFFSET                     0xa8u /* established */

/* int_mask1_a: rw64, reset unknown; shipped IRIX kernel binaries, per docs/interrupts.md */
#define HUBPI_INT_MASK1_A_OFFSET                     0xb0u /* established */

/* int_mask0_b: rw64, reset unknown; shipped IRIX kernel binaries, per docs/interrupts.md */
#define HUBPI_INT_MASK0_B_OFFSET                     0xb8u /* established */

/* int_mask1_b: rw64, reset unknown; shipped IRIX kernel binaries, per docs/interrupts.md */
#define HUBPI_INT_MASK1_B_OFFSET                     0xc0u /* established */

#define HUBPI_INT_PEND_MOD_ALT_OFFSET                0x200090u /* established */
#define HUBPI_LOCAL_WINDOW_BASE                      0x1000000u /* established */
#define HUBPI_NODE_SHIFT                             0x20u /* established */
#define HUBPI_NODE_WINDOW_BASE                       0x1800000u /* established */

#endif /* HUBPI_GEN_H */
