# Origin 300 hub crosstalk-side register window

<!-- generated from defs/hubio.toml by defs-gen - do not edit, change the source -->

Origin 300 hub crosstalk-side window: the link status doubleword, the register tested by the error path, the request-buffer table the I/O error scan reads and the widget part numbers the kernel compares.

Unit `hubio`, macro prefix `HUBIO`.
Default source: shipped IRIX kernel binaries, per docs/ip35-hub-errors.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `llp_csr` | 0x128 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |
| `flags` | 0x1e8 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |
| `crb_a` | 0x400 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |
| `crb_b` | 0x408 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |
| `crb_c` | 0x410 | r | 64 |  |  | unknown | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |

### Register `llp_csr`

Link control and status doubleword that the early widget probe reads. Only bit 13
is described. With it clear, the probe returns -1 for the part, revision and
manufacturer without touching the widget. Whether the bit means link up is
inferred from how the I/O error interrupt set-up uses it.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 13 | `bit13` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | the early widget probe tests this bit; the I/O error interrupt set-up tests it before choosing a handler path |

### Register `flags`

The register the kernel's error path tests before it goes on to the widget error handler.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 4 | `bit4` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | see bit0; role unknown |
| 0 | `bit0` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | tested with bit 4; the error routine continues only when one of the two is set; role unknown |

### Register `crb_a`

Request-buffer table. Entry i (0 to 14) has three 64-bit words, crb_a, crb_b
and crb_c, at the register offsets below plus the stride times i; the scan
reads all fifteen entries in order. See CRB_STRIDE and CRB_COUNT.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 55 | `bit55` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | with the bit set the type selects the handling case; with it clear bit49 and bit51 are tested |
| 54:52 | `type` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | error type, 0 to 7; with bit55 set it picks one of eight handling cases |
| 51 | `bit51` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | reported as an uncorrectable data error on data from the network; tested only when bit55 is clear |
| 50 | `bit50` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | the scan skips the entry unless this bit is set |
| 49 | `bit49` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | reported as a crosstalk packet that arrived with its error bit set; tested only when bit55 is clear |
| 0 | `bit0` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | selects which of two forms of the error record the scan builds for a block-transfer error; role unknown |

### Register `crb_b`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 58 | `engine_alt` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | selects the block-transfer engine when crb_c bit48 is set |
| 42:41 | `sub` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | see kind |
| 32:30 | `kind` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | with the value 1 or 5 and sub 1 or 2, the entry is treated as a block-transfer-engine error; bit 32 alone also selects the engine when crb_c bit48 is clear |

### Register `crb_c`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 48 | `bit48` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | with the bit set the entry is treated as a block-transfer-engine error |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `CRB_COUNT` | 0xf | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | entries the scan reads |
| `CRB_STRIDE` | 0x20 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | bytes between entries |
| `NODE_SHIFT` | 0x21 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | node number shift |
| `WIDGET_PART_C104` | 0xc104 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | widget part number the Origin 300/350 kernels compare by value, read from the widget identification word; compared in both releases read and in no Origin 200/2000 kernel; role unknown |
| `WIDGET_PART_C105` | 0xc105 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | widget part number, compared like C104; the kernel's per-widget set-up takes the same programmed-I/O mode call for both as for its graphics parts; no driver registers either part |
| `WINDOW_BASE` | 0x1c00000 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | physical base of the window for node 0 |
