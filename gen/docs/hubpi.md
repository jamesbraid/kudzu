# node hub interrupt pending and mask registers

<!-- generated from defs/hubpi.toml by defs-gen - do not edit, change the source -->

Node hub interrupt pending and mask registers.

Unit `hubpi`, macro prefix `HUBPI`.
Default source: shipped IRIX kernel binaries, per docs/interrupts.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `int_pend_mod` | 0x90 | w | 64 |  |  | unknown | inferred | shipped IRIX kernel binaries, per docs/interrupts.md |
| `int_pend0` | 0x98 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/interrupts.md |
| `int_pend1` | 0xa0 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/interrupts.md |
| `int_mask0_a` | 0xa8 | rw | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/interrupts.md |
| `int_mask1_a` | 0xb0 | rw | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/interrupts.md |
| `int_mask0_b` | 0xb8 | rw | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/interrupts.md |
| `int_mask1_b` | 0xc0 | rw | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/interrupts.md |

### Register `int_pend_mod`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 8 | `set` | inferred |  | shipped IRIX kernel binaries, per docs/interrupts.md |  |
| 6:0 | `level` | established |  | shipped IRIX kernel binaries, per docs/interrupts.md |  |

### Register `int_pend0`

### Register `int_pend1`

### Register `int_mask0_a`

### Register `int_mask1_a`

### Register `int_mask0_b`

### Register `int_mask1_b`

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `INT_PEND_MOD_ALT_OFFSET` | 0x200090 | established | shipped IRIX kernel binaries, per docs/interrupts.md | a processor whose slice number has bit 1 set reaches int_pend_mod at this offset in place of 0x90 |
| `LOCAL_WINDOW_BASE` | 0x1000000 | established | shipped IRIX kernel binaries, per docs/interrupts.md | physical base of the processor's own hub window; no node shift |
| `NODE_SHIFT` | 0x20 | established | shipped IRIX kernel binaries, per docs/interrupts.md | physical node number shift |
| `NODE_WINDOW_BASE` | 0x1800000 | established | shipped IRIX kernel binaries, per docs/interrupts.md | physical base of a named node's window |
