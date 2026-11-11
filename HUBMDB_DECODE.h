/*
 * HUBMDB_DECODE.h - Origin 300 hub memory-directory migration registers
 *
 * Publish-safe contract for the memory-directory migration registers of an
 * Origin 300 class node hub as far as the shipped kernels touch them.
 * Registers are 64-bit. Offsets are relative to the block's window, at the
 * window base plus the node number shifted left by the node shift. The
 * layout differs from the Origin 200/2000 block; do not carry offsets over.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   REG:   <BLOCK>.<name> <offset> <acc><width> [count=<n>] reset=<v> <grade>
 *   FIELD: <BLOCK>.<name>.<field> <hi>:<lo> <grade>     a field of a register
 *
 * Derivation. Offsets and constants come from reading the arm and interrupt
 * routines of the shipped kernels that carried automatic migration. Register
 * names are labels from a public vendor header. A register that header names
 * and no shipped kernel accesses is graded unknown.
 */
#ifndef HUBMDB_DECODE_H
#define HUBMDB_DECODE_H

#include <stdint.h>

#define HUBMDB_NODE_SHIFT 33 /* established: physical node number shift */
#define HUBMDB_WINDOW_BASE_UNITS 0x1fc /* established: physical base of the block for node 0, in units of 1 << HUBMDB_WINDOW_BASE_UNIT_SHIFT bytes; split because the full value is also a boot-region address the publish check refuses */
#define HUBMDB_WINDOW_BASE_UNIT_SHIFT 16 /* established */

/* The block is not described with REG lines for the offsets below because the
 * check that no two registers overlap treats every header as one address
 * space, and these offsets coincide with the Origin 200/2000 block's. They are
 * plain constants until that check can scope a register space per chip. */
#define HUBMDB_MIG_CANDIDATE_OFFSET 0x30 /* unknown: 64-bit read; named only by a public vendor header, no shipped kernel accesses it */
#define HUBMDB_MIG_CANDIDATE_CLR_OFFSET 0x38 /* inferred: 64-bit read, clears on read; kernels that carried automatic migration read it in a loop as the candidate, the newest kernels do not access it */
#define HUBMDB_MIG_VALUE_THRESH_OFFSET 0x48 /* inferred role: 64-bit read-modify-write; bit 63 is the valid bit the kernel sets to arm and clears to disarm */

/* REG: HUBMDB.mig_diff_thresh 0x40 rw64 reset=unknown established */
/* value: 0x7fff minus the derived count; the width is inferred from the constant */
/* FIELD: HUBMDB.mig_diff_thresh.value 14:0 inferred */
#define HUBMDB_MIG_DIFF_THRESH_VALUE_SHIFT 0
#define HUBMDB_MIG_DIFF_THRESH_VALUE_MASK  0x7fffu
/* The generator reads fields of the low 32 bits only, so the top bit is a plain constant. */
#define HUBMDB_MIG_DIFF_THRESH_VALID_BIT 63 /* established: set to arm, cleared to disarm */

#endif
