# O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe

<!-- generated from defs/gbe.toml by defs-gen - do not edit, change the source -->

Register offsets, packed timing words and option-probe constants that the O2 kernel writes on the GBE block.

Unit `gbe`, macro prefix `GBE`.
Default source: shipped IRIX kernel binaries, per docs/gbe.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `dotclk` | 0x4 | rw | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `device_id` | 0x14 | r | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_xy` | 0x10000 | rw | 32 |  |  | unknown | inferred | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_1` | 0x10004 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_2` | 0x10008 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_3` | 0x1000c | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_vblank` | 0x10010 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_hblank` | 0x10014 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `timing_flags` | 0x10018 | rw | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_7` | 0x1001c | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `fp_hdrv` | 0x10028 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `fp_vdrv` | 0x1002c | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `fp_de` | 0x10030 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_13` | 0x10034 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_14` | 0x10038 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_15` | 0x1003c | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_16` | 0x10040 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_17` | 0x10044 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_18` | 0x10048 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_19` | 0x1004c | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `ovr_readback` | 0x20004 | r | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `ovr_control` | 0x20008 | rw | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `frm_readback` | 0x30008 | r | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `frm_control` | 0x3000c | rw | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `did_control` | 0x40004 | rw | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `wid` | 0x48000 | w | 32 | 32 |  | unknown | established | shipped IRIX X display module, per docs/gbe.md |
| `cursor_pos` | 0x70000 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `cursor_ctrl` | 0x70004 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `cursor_color1` | 0x70008 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `cursor_color2` | 0x7000c | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `cursor_glyph` | 0x78000 | w | 32 | 64 |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |

### Register `dotclk`

Dot-clock (PLL) word. The timing routine clears the run bit, writes the timing words, then writes this word with the run bit set and waits 11 milliseconds. The coefficient fields hold M minus one, N minus one and P, as the kernel stores them.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 20 | `run` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | cleared before a new value and set with it; what it does in the hardware is inferred (see DOTCLK_RUN_BIT) |
| 15:14 | `pll_p` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | PLL post-divider P, stored unchanged |
| 13:8 | `pll_n` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | PLL divider N, stored as N minus one |
| 7:0 | `pll_m` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | PLL multiplier M, stored as M minus one |

### Register `device_id`

Identity word the kernel reads first. It accepts the block only when the word reads 0x666; any other value means the kernel leaves the block untouched.

### Register `vt_xy`

Vertical timing position word, called VT_0 in the reading. The start-up sequence compares its whole unmasked value with the vertical blank start line, and the stop sequence freezes it with the value in VT_XY_FREEZE. The offset follows from the VT_n numbering of the other timing words; the reading does not state it directly.

### Register `vt_1`

Frame totals. The timing routine writes 0 here, and the start-up sequence writes the totals once the frame buffer is set up. The kernel stores the record's total pixels and total lines, not one less than them; whether the hardware expects one less is unknown.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `total_lines` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | total lines per frame, as stored |
| 11:0 | `total_pixels` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | total pixels per line, as stored |

### Register `vt_2`

Vertical sync position, in lines.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `vsync_start` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | line where vertical sync starts |
| 11:0 | `vsync_end` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | line where vertical sync ends |

### Register `vt_3`

Horizontal sync position, in pixels.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `hsync_start` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | pixel where horizontal sync starts |
| 11:0 | `hsync_end` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | pixel where horizontal sync ends |

### Register `vt_vblank`

Vertical blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active height and 0xfff. Other stores to this offset are outside what this unit records.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `hi` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | active height |
| 11:0 | `lo` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 0xfff in the interlace case |

### Register `vt_hblank`

Horizontal blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active width and 0xfff. Other stores to this offset are outside what this unit records.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `hi` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | active width |
| 11:0 | `lo` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 0xfff in the interlace case |

### Register `timing_flags`

Timing flag word. The timing routine reads it, clears bits 1 and 7, and ORs in a value of 0 or 2 for the byte 132 set-up. What bit 1 does to the hardware is unknown. The value is undefined when the option lookup that feeds it fails.

### Register `vt_7`

Vertical blank start line in the high field. The low field repeats it when the frame-lock flag bits of the timing record (mask 0x12) are set, and holds 0xfff otherwise.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `blank_start` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | vertical blank start line |
| 11:0 | `low` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | the blank start line when a frame-lock flag is set, else 0xfff |

### Register `fp_hdrv`

Flat-panel horizontal drive word. The value depends on the board's panel set-up byte and on whether a dual-channel option is fitted. The table of values is in docs/gbe.md.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `hi` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 12-bit field; its meaning is unknown |
| 11:0 | `lo` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 12-bit field; its meaning is unknown |

### Register `fp_vdrv`

Flat-panel vertical drive word. The value depends on the panel set-up byte and on the option, as the table in docs/gbe.md gives.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `hi` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 12-bit field; its meaning is unknown |
| 11:0 | `lo` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 12-bit field; its meaning is unknown |

### Register `fp_de`

Flat-panel data-enable word. The timing routine writes it only for the default set-up and panel set-ups 130 and 131, and only when no dual-channel option is fitted. It is not written otherwise.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `hi` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 12-bit field; its meaning is unknown |
| 11:0 | `lo` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | 12-bit field; its meaning is unknown |

### Register `vt_13`

Parked holding 0xffffff before the frame buffer is set up and written last in the start-up sequence. The final value is ((active width + v - 2) mod total pixels) in the low field and v in the high field, where v is the horizontal blank end minus 0x13, with total pixels added first when that is negative.

### Register `vt_14`

Parked holding 0xffffff and written last in the start-up sequence with the vertical blank start in the low field and the vertical blank end in the high field.

### Register `vt_15`

Horizontal blank window, in pixels.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `hblank_start` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | pixel where horizontal blank starts |
| 11:0 | `hblank_end` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | pixel where horizontal blank ends |

### Register `vt_16`

Vertical blank window, in lines.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:12 | `vblank_start` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | line where vertical blank starts |
| 11:0 | `vblank_end` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | line where vertical blank ends |

### Register `vt_17`

Two parts, with bit placement not stated in the reading. The vertical part is minus (vertical blank start minus vertical blank end minus 1) when that difference is positive, else the difference, masked to 12 bits. The horizontal part is the horizontal blank end minus 20, with total pixels added when the blank end is below 20.

### Register `vt_18`

The vertical part of VT_17 and the horizontal blank end minus 54, with total pixels added when that is negative, then 0x1000 added for one line. The reading gives the 54 as a kernel constant. The 0x1000 step places the line part in bits 23:12 (inferred), so the horizontal part is in bits 11:0 (inferred).

### Register `vt_19`

The vertical part of VT_17 without the extra line that VT_18 adds, and the horizontal blank end minus 4. Bit placement is inferred as for VT_18.

### Register `ovr_readback`

Live readback of the overlay control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the overlay control.

### Register `ovr_control`

Overlay control. The stop routine clears its enable bit and leaves the word otherwise unwritten.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 0 | `enable` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | cleared by the stop routine; the meaning as a DMA enable is inferred |

### Register `frm_readback`

Live readback of the frame control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the frame control.

### Register `frm_control`

Frame-buffer scan-out control. The stop routine saves it, clears its enable bit, and later writes it back with its pointer field bumped by one unit. The pointer field starts at bit 5 (inferred).

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 0 | `enable` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | cleared by the stop routine; the meaning as a DMA enable is inferred |

### Register `did_control`

Display-ID table control. The stop routine clears its enable bit and leaves the word otherwise unwritten.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 16 | `enable` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | cleared by the stop routine; the stop routine also tests it at entry |
| 15:0 | `table_hi` | inferred |  | shipped IRIX kernel binaries, per docs/gbe.md | Upper address bits of the 64 KiB display-ID table. The specification gives this field; the kernel never writes it |

### Register `wid`

Window-ID table, one 32-bit word per window index. Indices of 32 or more are rejected by the kernel. The kernel writes entry 0 with 3 at text-port set-up.

Array of 32 registers, stride 4.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 12:11 | `aux` | inferred |  | shipped IRIX X display module, per docs/gbe.md | Auxiliary code. The kernel's compare mask of 0x7fc leaves it out. The position is from the specification |
| 10 | `gamma_disable` | established |  | shipped IRIX X display module, per docs/gbe.md | gamma disable. The kernel clears it before a write when the alternative gamma table is in use. |
| 9:5 | `map_group` | inferred |  | shipped IRIX X display module, per docs/gbe.md | colour-map group; the group's first entry is this value times 256. The bit position is inferred from the table values. |
| 4:2 | `type` | inferred |  | shipped IRIX X display module, per docs/gbe.md | pixel type: 0 indexed 8-bit, 1 indexed 12-bit, 2 RG3B2, 4 RGB5, 5 RGB8. The bit position is inferred from the table values and the compare mask. |
| 1:0 | `buf` | inferred |  | shipped IRIX X display module, per docs/gbe.md | buffer code: 3 buffers both, 0 marks the window to be skipped by the buffer flip, 1 and 2 exchange on a swap. The bit position is inferred from the table values. |

### Register `cursor_pos`

Hardware cursor position word. The text port writes 0 at set-up.

### Register `cursor_ctrl`

Cursor control word. The text port writes 1 after the glyph and colours are loaded. What the bits mean beyond that is unknown.

### Register `cursor_color1`

Cursor colour 1 in the byte layout red, green, blue, zero. The text port writes 0xffffff00.

### Register `cursor_color2`

Cursor colour 2 in the byte layout red, green, blue, zero. The text port writes 0xff000000.

### Register `cursor_glyph`

Cursor bitmap, 64 words. The text port loads words 0 to 31 from a 32-word table and writes 0 to words 32 to 63. Reading the words as a 32 by 32 bitmap at two bits per pixel is inferred; the text port draws with the first 32 words only (inferred).

Array of 64 registers, stride 4.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `DID_FRAME` | 32 | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `DID_LINE` | 16 | established | shipped IRIX kernel binaries, per docs/gbe.md |

### Word `DID_FRAME`

Frame entry of a display-ID table: one per horizontal band of lines. The X module packs it as block << 19 | offset << 11 | yend.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 25:19 | `block` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | block that holds the band's line table |
| 18:11 | `offset` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | offset of the band's line table within its block |
| 10:0 | `yend` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | last line of the band, counted from the first active line |

### Word `DID_LINE`

Line-table entry of a display-ID table: one per rectangle in a band, sorted by its last column. The X module packs it as index << 11 | xend.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15:11 | `did` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | display (window) ID of the rectangle |
| 10:0 | `xend` | established |  | shipped IRIX kernel binaries, per docs/gbe.md | last column of the rectangle; the last visible entry of a line ends at width minus 1 |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `BOARD_INTERLACE_OFF` | 0x2d | established | shipped IRIX kernel binaries, per docs/gbe.md | Board description byte that selects the interlace case when non-zero. The layout of the board description is not established here. |
| `BOARD_PANEL_OFF` | 0x2e | established | shipped IRIX kernel binaries, per docs/gbe.md | Board description byte that selects a flat-panel set-up. |
| `CMAP_ENTRIES_REV11` | 0x1200 | inferred | shipped IRIX kernel binaries, per docs/gbe.md | Colour-map entries in revision 1.1. The IRIX revision-2 tables and the public Linux driver use 6144 |
| `CMAP_ENTRY_BASE` | 0x50000 | established | shipped IRIX kernel binaries, per docs/gbe.md | Colour-map entry i is the word at this offset plus 4 times i; the word is red << 24 \| green << 16 \| blue << 8 |
| `CMFIFO_BATCH` | 0x21 | established | shipped IRIX kernel binaries, per docs/gbe.md | Colour-map writes the X module issues between FIFO checks |
| `CMFIFO_FREE_MIN` | 0x30 | established | shipped IRIX kernel binaries, per docs/gbe.md | Free colour-map FIFO entries the X module waits for after each batch of 33 writes |
| `CMFIFO_MASK` | 0x3f | established | shipped IRIX kernel binaries, per docs/gbe.md | Colour-map FIFO occupancy field, bits 5:0. The register that holds it is not recorded here. |
| `DEVICE_ID_VALUE` | 0x666 | established | shipped IRIX kernel binaries, per docs/gbe.md | Value the identity word at offset 0x14 must read for the kernel to accept the block |
| `DID_BLOCK_BYTES` | 0x200 | established | shipped IRIX kernel binaries, per docs/gbe.md | Size of one block of a display-ID table; line tables are packed in 16-bit units inside a block |
| `DID_BLOCK_ENTRIES` | 0x100 | established | shipped IRIX kernel binaries, per docs/gbe.md | Entries after which packing restarts in the next block |
| `DID_END_BLOCK` | 0x80 | established | shipped IRIX kernel binaries, per docs/gbe.md | Block number that ends a display-ID table; the X module stops filling it there |
| `DID_LAST_XEND` | 0x7ff | established | shipped IRIX kernel binaries, per docs/gbe.md | Right-hand column of the last band's line entry, which covers every line below the screen |
| `DID_LINE_TABLE_OFFSET` | 0x2000 | established | shipped IRIX kernel binaries, per docs/gbe.md | Byte offset of the first line table in a display-ID table |
| `DID_TABLE_BYTES` | 0x10000 | established | shipped IRIX kernel binaries, per docs/gbe.md | Size of one display-ID table as the X module fills it. The frame table starts at byte 0 and the line tables start at DID_LINE_TABLE_OFFSET. |
| `DOTCLK_RUN_BIT` | 0x14 | inferred | shipped IRIX kernel binaries, per docs/gbe.md | Bit of the dot-clock word that the kernel clears before a new value and sets with it. Inferred to run the PLL or its output; the specification leaves it undefined. |
| `DOTCLK_SETTLE_MS` | 0xb | established | shipped IRIX kernel binaries, per docs/gbe.md | Milliseconds the kernel waits after it writes the dot-clock word with the run bit set |
| `FIELD_MASK` | 0xfff | established | shipped IRIX kernel binaries, per docs/gbe.md | Mask applied to each 12-bit field before it is shifted or stored. |
| `FIELD_SHIFT` | 0xc | established | shipped IRIX kernel binaries, per docs/gbe.md | Bit position of the high 12-bit field in the packed timing words. |
| `GBE_LINE_IRQ_BASE` | 0x10 | established | shipped IRIX kernel binaries, per docs/gbe.md | CRIME interrupt bit of GBE line 0; GBE line n raises bit 16 + n. Lines 2 and 3 are the CRIME inputs 18 and 19. |
| `OPT_CMD_CLOCK_KHZ` | 0x186a0 | established | shipped IRIX kernel binaries, per docs/gbe.md | Pixel clock in kHz. Above this the timing routine sends 0xff to the option port; at or below it, 0x7f. |
| `OPT_CMD_ID_HIGH` | 0xff | established | shipped IRIX kernel binaries, per docs/gbe.md | Command written before the first id read. Its id bits come from bits 6 and 5 of the byte read. |
| `OPT_CMD_ID_LOW` | 0x7f | established | shipped IRIX kernel binaries, per docs/gbe.md | Command written before the second id read. Its id bits come from bits 6 and 5 of the byte read. |
| `OPT_DELAY_SMALL` | 0x2 | established | shipped IRIX kernel binaries, per docs/gbe.md | Horizontal delay used when the installed value is below the limit. It is zero otherwise. |
| `OPT_ID_MASK` | 0x60 | established | shipped IRIX kernel binaries, per docs/gbe.md | Bits 6 and 5 of an id read. |
| `OPT_ID_RESTORE` | 0x70 | established | shipped IRIX kernel binaries, per docs/gbe.md | Bits ORed into the first byte read when the probe writes that byte back. |
| `OPT_INSTALLED_LIMIT` | 0x9 | established | shipped IRIX kernel binaries, per docs/gbe.md | Installed values below this use the small horizontal delay. The reason for the split is inferred. |
| `OPT_PORT_ADDR` | 0x7e | established | shipped IRIX kernel binaries, per docs/gbe.md | Write address of the option port on the flat-panel bus. A read uses the same address with bit 0 set. |
| `OVERLAY_TRANSPARENT_INDEX` | 0x0 | inferred | shipped IRIX kernel binaries, per docs/gbe.md | Overlay pixel value that is transparent, so the normal planes show through |
| `PANEL_SETUP_A` | 0x82 | established | shipped IRIX kernel binaries, per docs/gbe.md | Panel set-up byte: fixed values for the first panel. |
| `PANEL_SETUP_B` | 0x83 | established | shipped IRIX kernel binaries, per docs/gbe.md | Panel set-up byte: fixed values for the second panel. |
| `PANEL_SETUP_C` | 0x84 | established | shipped IRIX kernel binaries, per docs/gbe.md | Panel set-up byte: fixed horizontal value, and the timing flag rewrite. |
| `REF_CLOCK_KHZ` | 0x4e20 | inferred | shipped IRIX kernel binaries, per docs/gbe.md | PLL reference clock behind the dot-clock table. The kernel states none; the source reading takes 20 MHz from a public Linux driver, not re-checked here. |
| `TILE_LIST_ALIGN_BYTES` | 0x200 | inferred | shipped IRIX kernel binaries, per docs/gbe.md | Tile-list requests start on this boundary |
| `TILE_LIST_OUTSTANDING` | 0x8 | inferred | shipped IRIX kernel binaries, per docs/gbe.md | Tile-list requests that may be outstanding at once |
| `TILE_LIST_PAGE_BYTES` | 0x2000 | inferred | shipped IRIX kernel binaries, per docs/gbe.md | A tile-list request does not cross a page of this size |
| `TILE_NUMBER_MASK` | 0x1fff | established | shipped IRIX kernel binaries, per docs/gbe.md | Mask the kernel applies to a physical tile address before it is written into a tile list |
| `TWO_WIRE_FP_OFF` | 0x10 | established | shipped IRIX kernel binaries, per docs/gbe.md | Register offset of the flat-panel two-wire bus. Bus selector 1 drives it. |
| `TWO_WIRE_MON_OFF` | 0x8 | established | shipped IRIX kernel binaries, per docs/gbe.md | Register offset of the monitor two-wire bus. Every other bus selector drives it. |
| `TWO_WIRE_WAIT_BIT` | 0x1 | established | shipped IRIX kernel binaries, per docs/gbe.md | Bit of the bus register that a start condition waits to read clear. |
| `TWO_WIRE_WAIT_LIMIT` | 0x3c | established | shipped IRIX kernel binaries, per docs/gbe.md | Most delays a start condition polls the bus bit before it gives up. |
| `VT_XY_FREEZE` | 0x80000000 | established | shipped IRIX kernel binaries, per docs/gbe.md | Value the kernel writes to the vertical timing position word to freeze it; the word's bit 31 is read first as the freeze state |
