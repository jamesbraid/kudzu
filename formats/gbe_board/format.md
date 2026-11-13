<!-- generated from formats/gbe_board/format.toml by defs-gen - do not edit, change the source -->

# Board description bytes the O2 kernel reads to choose GBE timing

Software table, not a register map: the two bytes of the board description that select the interlace case and a flat-panel set-up, and the set-up values the timing routine recognises. The GBE registers they steer are described in the crime repository.

Unit `gbe_board`, macro prefix `GBE`.
Default source: shipped IRIX kernel binaries, per software/video/gbe.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `BOARD_INTERLACE_OFF` | 0x2d | established | shipped IRIX kernel binaries, per software/video/gbe.md | Board description byte that selects the interlace case when non-zero. The layout of the board description is not established here. |
| `BOARD_PANEL_OFF` | 0x2e | established | shipped IRIX kernel binaries, per software/video/gbe.md | Board description byte that selects a flat-panel set-up. |
| `PANEL_SETUP_A` | 0x82 | established | shipped IRIX kernel binaries, per software/video/gbe.md | Panel set-up byte: fixed values for the first panel. |
| `PANEL_SETUP_B` | 0x83 | established | shipped IRIX kernel binaries, per software/video/gbe.md | Panel set-up byte: fixed values for the second panel. |
| `PANEL_SETUP_C` | 0x84 | established | shipped IRIX kernel binaries, per software/video/gbe.md | Panel set-up byte: fixed horizontal value, and the timing flag rewrite. |
