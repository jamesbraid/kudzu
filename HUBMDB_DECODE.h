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
 * Derivation. The one offset the shipped kernels access comes from reading
 * the routine that arms the threshold. Register names are labels from a
 * public vendor header. A register that header names and no
 * shipped kernel accesses is graded unknown, including its access type.
 *
 * View. The kernel stores the 64-bit register through the node-indexed alias:
 * a base word plus the node number shifted left by HUBMDB_NODE_SHIFT, then
 * offset 0x40 of this block. The register is write-only for the kernel: one
 * store, no load, anywhere in the kernel. The chip register list in the chips
 * repository shows the same store as one 8-byte write at a node-indexed
 * offset that keeps only the low 24 bits of the window address. Whether the
 * register reads back is unknown: no shipped software depends on it.
 */
#ifndef HUBMDB_DECODE_H
#define HUBMDB_DECODE_H

#include <stdint.h>

#define HUBMDB_NODE_SHIFT 33 /* established: physical node number shift */
#define HUBMDB_WINDOW_BASE_UNITS 0x1fc /* established: physical base of the block for node 0, in units of 1 << HUBMDB_WINDOW_BASE_UNIT_SHIFT bytes; split because the full value is also a boot-region address the publish check refuses */
#define HUBMDB_WINDOW_BASE_UNIT_SHIFT 16 /* established */

/* These three are plain constants, not REG lines: a REG line needs an access
 * type and no shipped kernel accesses them. Their offsets coincide with the
 * Origin 200/2000 block's, which a REG line would put in a separate address
 * space. */
#define HUBMDB_MIG_CANDIDATE_OFFSET 0x30 /* unknown: access and width; named only by a public vendor header, no shipped kernel accesses it */
#define HUBMDB_MIG_CANDIDATE_CLR_OFFSET 0x38 /* unknown: access and width; the public vendor header names it the read-to-clear candidate register, no shipped kernel accesses it */
#define HUBMDB_MIG_VALUE_THRESH_OFFSET 0x48 /* unknown: access and width; named only by a public vendor header, no shipped kernel accesses it. The Origin 200/2000 block reads its candidate register at this offset in a loop; the two layouts differ, so that does not carry over */

/* The mode control sits in a different window of the hub, not in the block
 * above: its offset is the local register base of the public vendor header
 * subtracted from the address the notes record. No shipped kernel reads or
 * writes it, so only the offset is stated. */
#define HUBMDB_MIG_CONFIG_LOCAL_OFFSET 0x780018 /* inferred: offset from the hub-local register base, by subtraction against a public vendor header; unknown: access, width and fields, no shipped kernel accesses it */

/* REG: HUBMDB.mig_diff_thresh 0x40 w64 reset=unknown established */
/* value: 0x7fff minus the derived count; the width is inferred from the constant. The kernel only stores it: whether it reads back is unknown. */
/* FIELD: HUBMDB.mig_diff_thresh.value 14:0 inferred */
#define HUBMDB_MIG_DIFF_THRESH_VALUE_SHIFT 0
#define HUBMDB_MIG_DIFF_THRESH_VALUE_MASK  0x7fffu
/* The generator reads fields of the low 32 bits only, so the top bit is a plain constant. */
#define HUBMDB_MIG_DIFF_THRESH_VALID_BIT 63 /* established: set to arm, cleared to disarm */

#endif
