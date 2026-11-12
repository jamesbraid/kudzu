# Origin 200/2000 hub memory-directory migration registers

<!-- generated from defs/hubmda.toml by defs-gen - do not edit, change the source -->

Origin 200/2000 hub memory-directory migration registers.

Unit `hubmda`, macro prefix `HUBMDA`.
Default source: shipped IRIX kernel binaries.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `mig_diff_thresh` | 0x30 | rw | 64 |  |  | unknown | established | shipped IRIX kernel binaries |
| `mig_value_thresh` | 0x38 | rw | 64 |  |  | unknown | inferred | shipped IRIX kernel binaries |
| `mig_candidate` | 0x48 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries |

### Register `mig_diff_thresh`

Bit 63 is the valid bit: set when the kernel arms the threshold, cleared to disarm it while a candidate is handled. See MIG_DIFF_THRESH_VALID_BIT.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 19:0 | `value` | inferred |  | shipped IRIX kernel binaries |  |

### Register `mig_value_thresh`

Bit 63 is the valid bit, set to arm and cleared to disarm. See MIG_VALUE_THRESH_VALID_BIT.

### Register `mig_candidate`

Read-to-clear alias of the candidate register. The kernel reads it in a loop
until the valid bit (bit 63) is clear. The public Linux hub header calls 0x40
the candidate register and 0x48 its read-to-clear alias. The kernel reads only
0x48, so this is that alias; that a read consumes the candidate is inferred
from the name and the loop. Nothing accesses 0x40. See MIG_CANDIDATE_VALID_BIT.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 30 | `type` | inferred |  | shipped IRIX kernel binaries |  |
| 28:20 | `node` | inferred |  | shipped IRIX kernel binaries |  |
| 17:0 | `page` | inferred |  | shipped IRIX kernel binaries |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `MIG_CANDIDATE_VALID_BIT` | 0x3f | established | shipped IRIX kernel binaries | the loop ends when it is clear |
| `MIG_DIFF_THRESH_VALID_BIT` | 0x3f | established | shipped IRIX kernel binaries | set to arm, cleared to disarm |
| `MIG_VALUE_THRESH_VALID_BIT` | 0x3f | established | shipped IRIX kernel binaries | set to arm, cleared to disarm |
| `NODE_SHIFT` | 0x20 | established | shipped IRIX kernel binaries | physical node number shift |
| `REFCNT_COUNT_MASK_PREMIUM` | 0x7ffff | established | shipped IRIX kernel binaries | nineteen bits, premium memory; which memory is premium is inferred |
| `REFCNT_COUNT_MASK_STANDARD` | 0x7ff | established | shipped IRIX kernel binaries | eleven bits, standard memory |
| `REFCNT_COUNT_SHIFT` | 0x5 | established | shipped IRIX kernel binaries |  |
| `REFCNT_NODE_STRIDE` | 0x8 | established | shipped IRIX kernel binaries | bytes between the words of consecutive nodes for one page |
| `WINDOW_BASE` | 0x1a00000 | established | shipped IRIX kernel binaries | physical base of the block for node 0 |
