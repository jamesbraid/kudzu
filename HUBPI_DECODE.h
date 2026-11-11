/*
 * HUBPI_DECODE.h - node hub interrupt pending and mask registers
 *
 * Publish-safe contract for the interrupt registers of the processor
 * interface of an Origin 200/2000 class node hub. All registers are 64-bit.
 * Offsets are relative to the processor-interface window: the kernel reaches
 * a named node's window at the node-window base plus the node number shifted
 * left by the node shift, and its own hub at the local-window base with no
 * node bits.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   REG:   <BLOCK>.<name> <offset> <acc><width> [count=<n>] reset=<v> <grade>
 *   FIELD: <BLOCK>.<name>.<field> <hi>:<lo> <grade>     a field of a register
 *
 * Derivation. Offsets, widths and the level field come from reading the
 * interrupt connect and dispatch routines of the shipped kernels. Register
 * names are the ones the public Linux port uses and are labels only. What a
 * written bit does is read from the public header and only implied by the
 * kernel, so it is graded inferred.
 */
#ifndef HUBPI_DECODE_H
#define HUBPI_DECODE_H

#include <stdint.h>

#define HUBPI_NODE_SHIFT 32 /* established: physical node number shift */
#define HUBPI_LOCAL_WINDOW_BASE 0x1000000u /* established: physical base of the processor's own hub window; no node shift */
#define HUBPI_NODE_WINDOW_BASE 0x1800000u /* established: physical base of a named node's window */

/* REG: HUBPI.int_pend_mod 0x90 w64 reset=unknown inferred */
/* level: interrupt level number, 0 to 63 for the first pending register, 64 to 127 for the second */
/* FIELD: HUBPI.int_pend_mod.level 6:0 established */
#define HUBPI_INT_PEND_MOD_LEVEL_SHIFT 0
#define HUBPI_INT_PEND_MOD_LEVEL_MASK  0x7fu
/* set: with the bit clear a write clears the pending bit for the level; the kernel only writes with it clear, so the effect of setting it is from the public Linux header */
/* FIELD: HUBPI.int_pend_mod.set 8:8 inferred */
#define HUBPI_INT_PEND_MOD_SET_SHIFT 8
#define HUBPI_INT_PEND_MOD_SET_MASK  0x1u

/* REG: HUBPI.int_pend0 0x98 r64 reset=unknown established */
/* REG: HUBPI.int_pend1 0xa0 r64 reset=unknown established */
/* REG: HUBPI.int_mask0_a 0xa8 rw64 reset=unknown established */
/* REG: HUBPI.int_mask1_a 0xb0 rw64 reset=unknown established */
/* REG: HUBPI.int_mask0_b 0xb8 rw64 reset=unknown established */
/* REG: HUBPI.int_mask1_b 0xc0 rw64 reset=unknown established */

#endif
