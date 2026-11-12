# node hub network interface registers

<!-- generated from defs/hubni.toml by defs-gen - do not edit, change the source -->

Node hub network interface words: node number, link state, link reset and port error.

Unit `hubni`, macro prefix `HUBNI`.
Default source: shipped IRIX kernel binaries, per docs/ip35-hub-errors.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `NSRI` | 32 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |

### Word `NSRI`

The low 32 bits of the Origin 200/2000 network status word.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 16:8 | `nodeid` | established |  | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `O200_STATUS_OFFSET` | 0x600000 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | Origin 200/2000 network status word, carries the node number |
| `O300_ERROR_CLEAR_OFFSET` | 0x680120 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | Origin 300 port error clear word; the kernel reads it, then stores zero to the error word; whether the read clears anything is unknown |
| `O300_ERROR_LATCHED_HI` | 0x34 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | top bit of the range of latched error bits the kernel tests |
| `O300_ERROR_LATCHED_LO` | 0x18 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | bottom bit of that range |
| `O300_ERROR_LINK_RESET_BIT` | 0x34 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | link reset caused by another partition, with the enable word zero; that the chip latches it when a reset is refused is unknown |
| `O300_ERROR_OFFSET` | 0x680100 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md |  |
| `O300_ERROR_REPORT_A_OFFSET` | 0x680108 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | read through the local alias and printed in the link error report; role unknown |
| `O300_ERROR_REPORT_B_OFFSET` | 0x680110 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | as report word A |
| `O300_ERROR_REPORT_C_OFFSET` | 0x680118 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | as report word A |
| `O300_PORT_RESET_OFFSET` | 0x680008 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | Origin 300 port reset word; the kernel writes the value 6 |
| `O300_RESET_ENABLE_OFFSET` | 0x680010 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | Origin 300 reset enable word; the kernel writes 1; bit 0 is the one that matters (datasheet "reset ok" is inferred) |
| `O300_STATUS_OFFSET` | 0x680000 | established | shipped IRIX kernel binaries, per docs/ip35-hub-errors.md | Origin 300 network status word; bits 1:0 are tested as link state, bits 2:1 equal to 2 starts a link-reset wait |
