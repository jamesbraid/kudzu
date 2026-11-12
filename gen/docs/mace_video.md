# O2 MACE video window map and input channel registers as the IRIX O2 video driver reaches them

<!-- generated from defs/mace_video.toml by defs-gen - do not edit, change the source -->

Base addresses of the video window and its blocks, the offsets of the two input channels and their register layouts. Offsets in the input registers are from the start of an input channel. Prose and the reasoning are in docs/mace-video.md.

Unit `mace_video`, macro prefix `MACE_VIDEO`.
Default source: shipped IRIX O2 video driver object, per docs/mace-video.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `in_control` | 0x0 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_status` | 0x8 | r | 64 |  |  | unknown | inferred | MACE 2.0 specification, pages 36 and 37 as the reading cites them |
| `in_config` | 0x10 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_next_descriptor` | 0x18 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_field_offset` | 0x20 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_line_width` | 0x28 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_hclip_odd` | 0x30 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_vclip_odd` | 0x38 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_alpha_odd` | 0x40 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_hclip_even` | 0x48 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_vclip_even` | 0x50 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `in_alpha_even` | 0x58 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |

### Register `in_control`

Channel control word. The driver writes the whole word, changing only its low nine bits. The reading read the routines that write it in a workspace and did not re-read them, so the grade is inferred.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 8:1 | `intr_enable` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Eight interrupt enables. Which cause each bit enables is not stated. |
| 0 | `dma_enable` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | DMA enable. Bit 0 is stated for the output channel; the input reading gives only the nine-bit group. |

### Register `in_status`

Status word. Reading it clears the pending interrupt causes. The page numbers are the reading's citation and were not checked against the document.

### Register `in_config`

Configuration word, written at start-up. Its field layout is not recorded.

### Register `in_next_descriptor`

Physical address of the next descriptor block, held as a zero-extended 32-bit word. The driver ORs a flag byte into the word. Which bits carry the flag is not stated.

### Register `in_field_offset`

Field offset. The driver writes the low halfword of a transfer-record word here.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15:0 | `offset` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Field offset in the low halfword |

### Register `in_line_width`

Line width word. The name is the reading's label; the start-up writes it with a 64-bit store.

### Register `in_hclip_odd`

Horizontal clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded.

### Register `in_vclip_odd`

Vertical clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded.

### Register `in_alpha_odd`

Alpha word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded.

### Register `in_hclip_even`

Horizontal clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded.

### Register `in_vclip_even`

Vertical clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded.

### Register `in_alpha_even`

Alpha word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded.

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `GBE_CAPTURE_OFF` | 0x80000 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Offset of the GBE capture block from the GBE register block base. The driver stores the uncached pointer 0xb6080000 for it. |
| `IN2_OFF` | 0x80000 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Offset of input channel 2 from the start of the video window |
| `OUT_OFF` | 0x100000 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Offset of the output channel from the start of the video window |
| `TIMER_PHYS_ADDR` | 0x1f340000 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Physical base of the MACE count and compare timer block, read by the driver's timestamp routine. Its place as sub-block 4 of the MACE peripheral map is the reading's citation of the specification, not checked. |
| `TIMER_SNAP_IN1_OFF` | 0x38 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Timer snapshot doubleword for input channel 1, from the timer block base |
| `TIMER_SNAP_IN2_OFF` | 0x40 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Timer snapshot doubleword for input channel 2, from the timer block base |
| `TIMER_SNAP_OUT_OFF` | 0x48 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Timer snapshot doubleword for the output channel, from the timer block base |
| `TWO_WIRE_PHYS_ADDR` | 0x1f330000 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Physical base of the MACE peripheral two-wire bus block |
| `WINDOW_PHYS_ADDR` | 0x1f100000 | established | shipped IRIX O2 video driver object, per docs/mace-video.md | Physical base of the video window. The driver reaches it through the uncached address 0xbf100000 and stores that pointer as input channel 1. |
