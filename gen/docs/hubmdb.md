# Origin 300 hub memory-directory migration registers

<!-- generated from defs/hubmdb.toml by defs-gen - do not edit, change the source -->

Origin 300 hub memory-directory migration registers.

Unit `hubmdb`, macro prefix `HUBMDB`.
Default source: shipped IRIX kernel binaries, per docs/page-migration.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `mig_diff_thresh` | 0x40 | w | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/page-migration.md |

### Register `mig_diff_thresh`

The mode control sits in a different window of the hub, not in the block
above: its offset is the local register base of the public vendor header
subtracted from the address the notes record. No shipped kernel reads or
writes it, so only the offset is stated. See MIG_CONFIG_LOCAL_OFFSET.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 14:0 | `value` | inferred |  | shipped IRIX kernel binaries, per docs/page-migration.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `MIG_CANDIDATE_CLR_OFFSET` | 0x38 | unknown | shipped IRIX kernel binaries, per docs/page-migration.md | access and width; the public vendor header names it the read-to-clear candidate register, no shipped kernel accesses it |
| `MIG_CANDIDATE_OFFSET` | 0x30 | unknown | shipped IRIX kernel binaries, per docs/page-migration.md | access and width; named only by a public vendor header, no shipped kernel accesses it |
| `MIG_CONFIG_LOCAL_OFFSET` | 0x780018 | inferred | shipped IRIX kernel binaries, per docs/page-migration.md | offset from the hub-local register base, by subtraction against a public vendor header; unknown: access, width and fields, no shipped kernel accesses it |
| `MIG_DIFF_THRESH_VALID_BIT` | 0x3f | established | shipped IRIX kernel binaries, per docs/page-migration.md | set to arm, cleared to disarm |
| `MIG_VALUE_THRESH_OFFSET` | 0x48 | unknown | shipped IRIX kernel binaries, per docs/page-migration.md |  |
| `NODE_SHIFT` | 0x21 | established | shipped IRIX kernel binaries, per docs/page-migration.md | physical node number shift |
| `WINDOW_BASE_UNITS` | 0x1fc | established | shipped IRIX kernel binaries, per docs/page-migration.md | physical base of the block for node 0, in units of 1 << HUBMDB_WINDOW_BASE_UNIT_SHIFT bytes; split because the full value is also a boot-region address the publish check refuses |
| `WINDOW_BASE_UNIT_SHIFT` | 0x10 | established | shipped IRIX kernel binaries, per docs/page-migration.md |  |
