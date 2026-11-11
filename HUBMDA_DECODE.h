/*
 * HUBMDA_DECODE.h - Origin 200/2000 hub memory-directory migration registers
 *
 * Publish-safe contract for the memory-directory registers an Origin 200/2000
 * class node hub exposes for hardware page-reference counting, and for the
 * directory word the kernel reads counts from. Registers are 64-bit. Offsets
 * are relative to the block's window, which sits at the window base plus the
 * node number shifted left by the node shift.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   REG:   <BLOCK>.<name> <offset> <acc><width> [count=<n>] reset=<v> <grade>
 *   FIELD: <BLOCK>.<name>.<field> <hi>:<lo> <grade>     a field of a register
 *
 * Derivation. Offsets, widths and the constants stored come from reading
 * the migration arm, interrupt and counter routines of the shipped kernels.
 * Register names are labels from the public Linux port. The meaning of a
 * stored value or a decoded field is read from how the kernel uses it, not
 * measured, and is graded inferred where it is not a plain constant.
 */
#ifndef HUBMDA_DECODE_H
#define HUBMDA_DECODE_H

#include <stdint.h>

#define HUBMDA_NODE_SHIFT 32 /* established: physical node number shift */
#define HUBMDA_WINDOW_BASE 0x1a00000u /* established: physical base of the block for node 0 */

/* REG: HUBMDA.mig_diff_thresh 0x30 rw64 reset=unknown established */
/* value: 0xfffff minus the count the kernel derives from its threshold percentage and reference count; the field width is inferred from that constant */
/* FIELD: HUBMDA.mig_diff_thresh.value 19:0 inferred */
#define HUBMDA_MIG_DIFF_THRESH_VALUE_SHIFT 0
#define HUBMDA_MIG_DIFF_THRESH_VALUE_MASK  0xfffffu
/* valid: set when the kernel arms the threshold, cleared to disarm it while a candidate is handled */
/* The generator reads fields of the low 32 bits only, so the top bit is a plain constant. */
#define HUBMDA_MIG_DIFF_THRESH_VALID_BIT 63 /* established: set to arm, cleared to disarm */

/* REG: HUBMDA.mig_value_thresh 0x38 rw64 reset=unknown inferred */
/* The generator reads fields of the low 32 bits only, so the top bit is a plain constant. */
#define HUBMDA_MIG_VALUE_THRESH_VALID_BIT 63 /* established: set to arm, cleared to disarm */

/* REG: HUBMDA.mig_candidate 0x48 r64 reset=unknown established */
/* The kernel reads it in a loop until the valid bit is clear. The field positions are a reading of how the handler decodes each value. */
/* FIELD: HUBMDA.mig_candidate.page 17:0 inferred */
#define HUBMDA_MIG_CANDIDATE_PAGE_SHIFT 0
#define HUBMDA_MIG_CANDIDATE_PAGE_MASK  0x3ffffu
/* FIELD: HUBMDA.mig_candidate.node 28:20 inferred */
#define HUBMDA_MIG_CANDIDATE_NODE_SHIFT 20
#define HUBMDA_MIG_CANDIDATE_NODE_MASK  0x1ffu
/* FIELD: HUBMDA.mig_candidate.type 30:30 inferred */
#define HUBMDA_MIG_CANDIDATE_TYPE_SHIFT 30
#define HUBMDA_MIG_CANDIDATE_TYPE_MASK  0x1u
/* The generator reads fields of the low 32 bits only, so the top bit is a plain constant. */
#define HUBMDA_MIG_CANDIDATE_VALID_BIT 63 /* established: the loop ends when it is clear */

/* The directory memory holds one 64-bit reference-count word per page and
 * node. The count sits in bit 5 and up; what bits 4 to 0 hold is not read
 * here. */
#define HUBMDA_REFCNT_COUNT_SHIFT 5 /* established */
#define HUBMDA_REFCNT_COUNT_MASK_STANDARD 0x7ffu /* established: eleven bits, standard memory */
#define HUBMDA_REFCNT_COUNT_MASK_PREMIUM 0x7ffffu /* established: nineteen bits, premium memory; which memory is premium is inferred */
#define HUBMDA_REFCNT_NODE_STRIDE 8 /* established: bytes between the words of consecutive nodes for one page */

#endif
