/* generated from defs/hubmda.toml by defs-gen - do not edit, change the source */
/* Origin 200/2000 hub memory-directory migration registers */
#ifndef HUBMDA_GEN_H
#define HUBMDA_GEN_H

#include <stdint.h>


/*
 * Bit 63 is the valid bit: set when the kernel arms the threshold, cleared to disarm it while a candidate is handled. See MIG_DIFF_THRESH_VALID_BIT.
 */
/* mig_diff_thresh: rw64, reset unknown; shipped IRIX kernel binaries */
#define HUBMDA_MIG_DIFF_THRESH_OFFSET                0x30u /* established */
#define HUBMDA_MIG_DIFF_THRESH_VALUE_SHIFT           0 /* inferred */
#define HUBMDA_MIG_DIFF_THRESH_VALUE_MASK            0xfffffull
#define HUBMDA_MIG_DIFF_THRESH_VALUE_INPLACE         (HUBMDA_MIG_DIFF_THRESH_VALUE_MASK << HUBMDA_MIG_DIFF_THRESH_VALUE_SHIFT)

/*
 * Bit 63 is the valid bit, set to arm and cleared to disarm. See MIG_VALUE_THRESH_VALID_BIT.
 */
/* mig_value_thresh: rw64, reset unknown; shipped IRIX kernel binaries */
#define HUBMDA_MIG_VALUE_THRESH_OFFSET               0x38u /* inferred */

/*
 * Read-to-clear alias of the candidate register. The kernel reads it in a loop
 * until the valid bit (bit 63) is clear. The public Linux hub header calls 0x40
 * the candidate register and 0x48 its read-to-clear alias. The kernel reads only
 * 0x48, so this is that alias; that a read consumes the candidate is inferred
 * from the name and the loop. Nothing accesses 0x40. See MIG_CANDIDATE_VALID_BIT.
 */
/* mig_candidate: r64, reset unknown; shipped IRIX kernel binaries */
#define HUBMDA_MIG_CANDIDATE_OFFSET                  0x48u /* established */
#define HUBMDA_MIG_CANDIDATE_TYPE_SHIFT              30 /* inferred */
#define HUBMDA_MIG_CANDIDATE_TYPE_MASK               0x1ull
#define HUBMDA_MIG_CANDIDATE_TYPE_INPLACE            (HUBMDA_MIG_CANDIDATE_TYPE_MASK << HUBMDA_MIG_CANDIDATE_TYPE_SHIFT)
#define HUBMDA_MIG_CANDIDATE_NODE_SHIFT              20 /* inferred */
#define HUBMDA_MIG_CANDIDATE_NODE_MASK               0x1ffull
#define HUBMDA_MIG_CANDIDATE_NODE_INPLACE            (HUBMDA_MIG_CANDIDATE_NODE_MASK << HUBMDA_MIG_CANDIDATE_NODE_SHIFT)
#define HUBMDA_MIG_CANDIDATE_PAGE_SHIFT              0 /* inferred */
#define HUBMDA_MIG_CANDIDATE_PAGE_MASK               0x3ffffull
#define HUBMDA_MIG_CANDIDATE_PAGE_INPLACE            (HUBMDA_MIG_CANDIDATE_PAGE_MASK << HUBMDA_MIG_CANDIDATE_PAGE_SHIFT)

#define HUBMDA_MIG_CANDIDATE_VALID_BIT               0x3fu /* established */
#define HUBMDA_MIG_DIFF_THRESH_VALID_BIT             0x3fu /* established */
#define HUBMDA_MIG_VALUE_THRESH_VALID_BIT            0x3fu /* established */
#define HUBMDA_NODE_SHIFT                            0x20u /* established */
#define HUBMDA_REFCNT_COUNT_MASK_PREMIUM             0x7ffffu /* established */
#define HUBMDA_REFCNT_COUNT_MASK_STANDARD            0x7ffu /* established */
#define HUBMDA_REFCNT_COUNT_SHIFT                    0x5u /* established */
#define HUBMDA_REFCNT_NODE_STRIDE                    0x8u /* established */
#define HUBMDA_WINDOW_BASE                           0x1a00000u /* established */

#endif /* HUBMDA_GEN_H */
