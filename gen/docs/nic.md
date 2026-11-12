# bit-serial access to the one-wire identity parts

<!-- generated from defs/nic.toml by defs-gen - do not edit, change the source -->

Bit-serial control word for the one-wire identity parts and the hub address that reaches the same bus.

Unit `nic`, macro prefix `NIC`.
Default source: shipped IRIX kernel binaries, per docs/nic.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `GPCR_S` | 32 | inferred | shipped IRIX kernel binaries, per docs/nic.md |
| `MCR` | 32 | established | shipped IRIX kernel binaries, per docs/nic.md |

### Word `GPCR_S`

The I/O controller's general-purpose control word, written through its set
alias. Before reading the address part the routine stores 0x200000 to the
register address its caller passes, when that address is not null; with a
null address it prints that it is bypassing the enable and goes on. The word
is written whole, so bit 21 is the only bit the routine sets. That the store
enables the one-wire path to the part is read from the routine's own message,
and that a set alias leaves other bits alone is inferred from its name.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 21 | `nic_enable` | inferred |  | shipped IRIX kernel binaries, per docs/nic.md |  |

### Word `MCR`

The 32-bit microlan control word.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 10 | `pulse` | established |  | shipped IRIX kernel binaries, per docs/nic.md |  |
| 2 | `sample` | established |  | shipped IRIX kernel binaries, per docs/nic.md |  |
| 1 | `done` | established |  | shipped IRIX kernel binaries, per docs/nic.md |  |
| 0 | `result` | established |  | shipped IRIX kernel binaries, per docs/nic.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `GPCR_S_NIC_ENABLE_WORD` | 0x200000 | established | shipped IRIX kernel binaries, per docs/nic.md | the value stored |
| `HUB_ACCESS_NODE_SHIFT` | 0x21 | established | shipped IRIX kernel binaries, per docs/nic.md | node number shift |
| `HUB_ACCESS_OFFSET` | 0xe04030 | established | shipped IRIX kernel binaries, per docs/nic.md | offset of the register from the hub's local-window base |
| `HUB_ACCESS_PHYS_LOW` | 0x1e04030 | established | shipped IRIX kernel binaries, per docs/nic.md | physical address of the register for node 0 |
| `MAC_CRC16_RESIDUE` | 0xb001 | established | shipped IRIX kernel binaries, per docs/nic.md | CRC-16 residue over record bytes 1 to 13 |
| `MAC_ID_BYTE0` | 0x8d | established | shipped IRIX kernel binaries, per docs/nic.md | required value of record byte 0 |
| `MAC_ID_BYTE1` | 0xa | established | shipped IRIX kernel binaries, per docs/nic.md | required value of record byte 1 |
| `MAC_RECORD_LEN` | 0xe | established | shipped IRIX kernel binaries, per docs/nic.md | bytes read from the part |
