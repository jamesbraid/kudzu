# gbe_board: board description bytes that steer GBE timing

A software table, not a register map: the two bytes of the board description that select the interlace case and a flat-panel set-up, and the set-up values the timing routine recognises. [format.toml](format.toml) holds the values with their grades and [format.md](format.md) is the generated table.

The GBE registers they steer are described in the crime repository. How the shipped video code reads the board description is in [gbe.md](../../software/video/gbe.md).

## Used by

- [O2 GBE: two-wire buses, flat-panel timing words and the dual-channel option](../../software/video/gbe.md)
