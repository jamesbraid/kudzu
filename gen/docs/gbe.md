# O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe

<!-- generated from defs/gbe.toml by defs-gen - do not edit, change the source -->

Register offsets, packed timing words and option-probe constants that the O2 kernel writes on the GBE block.

Unit `gbe`, macro prefix `GBE`.
Default source: shipped IRIX kernel binaries, per docs/gbe.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `vt_vblank` | 0x10010 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `vt_hblank` | 0x10014 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `timing_flags` | 0x10018 | rw | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `fp_hdrv` | 0x10028 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `fp_vdrv` | 0x1002c | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |
| `fp_de` | 0x10030 | w | 32 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/gbe.md |

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

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `BOARD_INTERLACE_OFF` | 0x2d | established | shipped IRIX kernel binaries, per docs/gbe.md | Board description byte that selects the interlace case when non-zero. The layout of the board description is not established here. |
| `BOARD_PANEL_OFF` | 0x2e | established | shipped IRIX kernel binaries, per docs/gbe.md | Board description byte that selects a flat-panel set-up. |
| `FIELD_MASK` | 0xfff | established | shipped IRIX kernel binaries, per docs/gbe.md | Mask applied to each 12-bit field before it is shifted or stored. |
| `FIELD_SHIFT` | 0xc | established | shipped IRIX kernel binaries, per docs/gbe.md | Bit position of the high 12-bit field in the packed timing words. |
| `OPT_CMD_CLOCK_KHZ` | 0x186a0 | established | shipped IRIX kernel binaries, per docs/gbe.md | Pixel clock in kHz. Above this the timing routine sends 0xff to the option port; at or below it, 0x7f. |
| `OPT_CMD_ID_HIGH` | 0xff | established | shipped IRIX kernel binaries, per docs/gbe.md | Command written before the first id read. Its id bits come from bits 6 and 5 of the byte read. |
| `OPT_CMD_ID_LOW` | 0x7f | established | shipped IRIX kernel binaries, per docs/gbe.md | Command written before the second id read. Its id bits come from bits 6 and 5 of the byte read. |
| `OPT_DELAY_SMALL` | 0x2 | established | shipped IRIX kernel binaries, per docs/gbe.md | Horizontal delay used when the installed value is below the limit. It is zero otherwise. |
| `OPT_ID_MASK` | 0x60 | established | shipped IRIX kernel binaries, per docs/gbe.md | Bits 6 and 5 of an id read. |
| `OPT_ID_RESTORE` | 0x70 | established | shipped IRIX kernel binaries, per docs/gbe.md | Bits ORed into the first byte read when the probe writes that byte back. |
| `OPT_INSTALLED_LIMIT` | 0x9 | established | shipped IRIX kernel binaries, per docs/gbe.md | Installed values below this use the small horizontal delay. The reason for the split is inferred. |
| `OPT_PORT_ADDR` | 0x7e | established | shipped IRIX kernel binaries, per docs/gbe.md | Write address of the option port on the flat-panel bus. A read uses the same address with bit 0 set. |
| `PANEL_SETUP_A` | 0x82 | established | shipped IRIX kernel binaries, per docs/gbe.md | Panel set-up byte: fixed values for the first panel. |
| `PANEL_SETUP_B` | 0x83 | established | shipped IRIX kernel binaries, per docs/gbe.md | Panel set-up byte: fixed values for the second panel. |
| `PANEL_SETUP_C` | 0x84 | established | shipped IRIX kernel binaries, per docs/gbe.md | Panel set-up byte: fixed horizontal value, and the timing flag rewrite. |
| `TWO_WIRE_FP_OFF` | 0x10 | established | shipped IRIX kernel binaries, per docs/gbe.md | Register offset of the flat-panel two-wire bus. Bus selector 1 drives it. |
| `TWO_WIRE_MON_OFF` | 0x8 | established | shipped IRIX kernel binaries, per docs/gbe.md | Register offset of the monitor two-wire bus. Every other bus selector drives it. |
| `TWO_WIRE_WAIT_BIT` | 0x1 | established | shipped IRIX kernel binaries, per docs/gbe.md | Bit of the bus register that a start condition waits to read clear. |
| `TWO_WIRE_WAIT_LIMIT` | 0x3c | established | shipped IRIX kernel binaries, per docs/gbe.md | Most delays a start condition polls the bus bit before it gives up. |
