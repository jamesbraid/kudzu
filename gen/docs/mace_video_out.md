# O2 MACE video output channel registers as the IRIX O2 video driver reaches them

<!-- generated from defs/mace_video_out.toml by defs-gen - do not edit, change the source -->

Register offsets, widths and the fields the reading could state for the output channel. Offsets are from the output channel base, which is a fixed displacement in the video window. Prose and the reasoning are in docs/mace-video.md.

Unit `mace_video_out`, macro prefix `MACE_VIDEO_OUT`.
Default source: shipped IRIX O2 video driver object, per docs/mace-video.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `out_control` | 0x0 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_status` | 0x8 | r | 64 |  |  | unknown | inferred | MACE 2.0 specification, pages 36 and 37 as the reading cites them |
| `out_config` | 0x10 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_next_descriptor` | 0x18 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_field_offset` | 0x20 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_field_size` | 0x28 | rw | 64 |  |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_h_pad_odd` | 0x30 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_v_pad_odd` | 0x38 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_h_pad_even` | 0x40 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_v_pad_even` | 0x48 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_genlock_delay` | 0x50 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `out_hw_config` | 0x58 | rw | 64 |  |  | unknown | inferred | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md |

### Register `out_control`

Channel control word. The driver writes the whole word, changing only its low nine bits. The output recovery routines pulse bit 0. The reading read the routines that write it in a workspace and did not re-read them, so the grade is inferred.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 8:1 | `intr_enable` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Eight interrupt enables. Which cause each bit enables is not stated. |
| 0 | `dma_enable` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | DMA enable |

### Register `out_status`

Status word. Reading it clears the pending interrupt causes. The output recovery routines read it after their last write and discard the value. The page numbers were not checked against the document.

### Register `out_config`

Configuration word, written at start-up. Its field layout is not recorded here; the hardware configuration register at at 0x58 is a separate word.

### Register `out_next_descriptor`

Physical address of the next descriptor block, held as a zero-extended 32-bit word. The recovery routines write it as well. Which bits carry the flag byte the driver ORs in is not stated.

### Register `out_field_offset`

Field offset. The driver writes the low halfword of a transfer-record word here.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15:0 | `offset` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Field offset in the low halfword |

### Register `out_field_size`

Field size. The reading takes the line count and the bytes per line from one stored value, which it gives as 243 lines of 2880 bytes in one set and 288 lines in the other.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 21:12 | `lines` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Lines in the field |
| 11:0 | `line_bytes` | inferred |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Bytes per line |

### Register `out_h_pad_odd`

Horizontal padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded.

### Register `out_v_pad_odd`

Vertical padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded.

### Register `out_h_pad_even`

Horizontal padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded.

### Register `out_v_pad_even`

Vertical padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded.

### Register `out_genlock_delay`

Genlock delay word, written at start-up with a 64-bit store. Its layout is not recorded.

### Register `out_hw_config`

Hardware configuration word for the output channel: port reset and select bits, audio and genlock sync settings, the general-purpose output bit and the framelock start-of-field counts. Bits 4, 7 and 8 and the audio-sync reset bit are not assigned by the reading.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 35:32 | `ident` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Identity read by the video probe. The value that identifies the part is not in the reading. |
| 22:19 | `sof_count_even` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Start-of-field count for the even field |
| 18 | `general_output` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | General-purpose output bit |
| 17:14 | `sof_count_odd` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Start-of-field count for the odd field |
| 13 | `framelock_correction` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | GBE framelock correction |
| 12 | `framelock_precision` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | GBE framelock precision |
| 11:10 | `framelock_source` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | GBE framelock source |
| 9 | `framelock_reset` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | GBE framelock reset |
| 6:5 | `audio_sync_source` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Audio sync source |
| 3 | `port_cd_select` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Port CD select |
| 2 | `port_cd_reset` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Port CD reset. Released whenever an input or output is set up. |
| 1 | `port_ab_select` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Port AB select |
| 0 | `port_ab_reset` | inferred |  | MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md | Port AB reset. Released whenever an input or output is set up. |
