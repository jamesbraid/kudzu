# PCI bridge widget identification and window layouts

<!-- generated from defs/pcibr.toml by defs-gen - do not edit, change the source -->

PCI bridge widget identification word, configuration address layout and interrupt address registers.

Unit `pcibr`, macro prefix `PCIBR`.
Default source: shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `WID` | 32 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |

### Word `WID`

The widget identification word.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 27:12 | `part` | established | layout32=49154, layout32b=53250, layout64b=53506, layout64bx=53522, layout64a=57344, layout64ax=57360 | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |  |
| 11:1 | `field11` | established | value36=36, value54=54 | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `CFG_FUNC_SHIFT` | 0x8 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | function shifted left by this |
| `CFG_SLOTS_32` | 0x8 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | slots 0 to 7 on the 32-bit layout |
| `CFG_SLOT_BIAS_64` | 0x1 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | on the 64-bit layouts the slot is incremented before use; the accepted range is then 1 to 4 |
| `CFG_SLOT_SHIFT` | 0xc | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | configuration address is window plus slot shifted left by this |
| `CFG_WINDOW32_OFFSET` | 0x20000 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | added on the 32-bit layout only, with the register offset first rounded down to a multiple of four |
| `INTR_ADDR32_HI` | 0x11 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | the 32-bit layout replaces bits 17:8 of the word |
| `INTR_ADDR32_LO` | 0x8 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |  |
| `INTR_ADDR32_OFFSET` | 0x134 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | first interrupt address register, 32-bit layout; a 32-bit word |
| `INTR_ADDR32_SRC_HI` | 0x2f | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | the 32-bit layout keeps source address bits 47:38 |
| `INTR_ADDR32_SRC_LO` | 0x26 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |  |
| `INTR_ADDR64_OFFSET` | 0x130 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | first interrupt address register, 64-bit layouts; its low 48 bits hold the address |
| `INTR_COUNT` | 0x8 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | interrupt numbers 0 to 7 |
| `INTR_STRIDE` | 0x8 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | byte stride of the interrupt address registers on every layout |
| `WID_OFFSET` | 0x4 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md | byte offset of the widget identification word in the window |
| `WINDOW_BIT_A` | 0x3f | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |  |
| `WINDOW_BIT_B` | 0x3c | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |  |
| `WINDOW_BIT_C` | 0x39 | established | shipped IRIX kernel binaries, per docs/ip35-pcibr-registers.md |  |
