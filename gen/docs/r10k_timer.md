# R10000 Count and Compare timer interrupt

<!-- generated from defs/r10k_timer.toml by defs-gen - do not edit, change the source -->

R10000 Count and Compare timer interrupt, from the published manual.

Unit `r10k_timer`, macro prefix `R10K_TIMER`.
Default source: published R10000 user's manual (the header gives no document number).

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `CAUSE` | 32 | established | published R10000 user's manual (the header gives no document number) |

### Word `CAUSE`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15 | `ip7` | established |  | published R10000 user's manual (the header gives no document number) |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `R10K_CP0_COMPARE` | 0xb | established | published R10000 user's manual (the header gives no document number) | established: Compare register number in coprocessor 0 |
| `R10K_CP0_COUNT` | 0x9 | established | published R10000 user's manual (the header gives no document number) | established: Count register number in coprocessor 0 |
