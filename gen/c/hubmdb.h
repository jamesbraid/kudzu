/* generated from defs/hubmdb.toml by defs-gen - do not edit, change the source */
/* Origin 300 hub memory-directory migration registers */
#ifndef HUBMDB_GEN_H
#define HUBMDB_GEN_H

#include <stdint.h>


/*
 * The mode control sits in a different window of the hub, not in the block
 * above: its offset is the local register base of the public vendor header
 * subtracted from the address the notes record. No shipped kernel reads or
 * writes it, so only the offset is stated. See MIG_CONFIG_LOCAL_OFFSET.
 */
/* mig_diff_thresh: w64, reset unknown; shipped IRIX kernel binaries, per docs/page-migration.md */
#define HUBMDB_MIG_DIFF_THRESH_OFFSET                0x40u /* established */
#define HUBMDB_MIG_DIFF_THRESH_VALUE_SHIFT           0 /* inferred */
#define HUBMDB_MIG_DIFF_THRESH_VALUE_MASK            0x7fffull
#define HUBMDB_MIG_DIFF_THRESH_VALUE_INPLACE         (HUBMDB_MIG_DIFF_THRESH_VALUE_MASK << HUBMDB_MIG_DIFF_THRESH_VALUE_SHIFT)

#define HUBMDB_MIG_CANDIDATE_CLR_OFFSET              0x38u /* unknown */
#define HUBMDB_MIG_CANDIDATE_OFFSET                  0x30u /* unknown */
#define HUBMDB_MIG_CONFIG_LOCAL_OFFSET               0x780018u /* inferred */
#define HUBMDB_MIG_DIFF_THRESH_VALID_BIT             0x3fu /* established */
#define HUBMDB_MIG_VALUE_THRESH_OFFSET               0x48u /* unknown */
#define HUBMDB_NODE_SHIFT                            0x21u /* established */
#define HUBMDB_WINDOW_BASE_UNITS                     0x1fcu /* established */
#define HUBMDB_WINDOW_BASE_UNIT_SHIFT                0x10u /* established */

#endif /* HUBMDB_GEN_H */
