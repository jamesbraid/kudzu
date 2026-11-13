<!-- generated from formats/nic_record/format.toml by defs-gen - do not edit, change the source -->

# Ethernet address record the IRIX kernel reads from a one-wire part

Software table, not a register map: the layout the kernel checks in the record it reads from the one-wire part on an Ethernet controller. The one-wire access registers are described in the chips repository.

Unit `nic_record`, macro prefix `NIC`.
Default source: shipped IRIX kernel binaries, per docs/nic.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `MAC_CRC16_RESIDUE` | 0xb001 | established | shipped IRIX kernel binaries, per docs/nic.md | CRC-16 residue over record bytes 1 to 13 |
| `MAC_ID_BYTE0` | 0x8d | established | shipped IRIX kernel binaries, per docs/nic.md | required value of record byte 0 |
| `MAC_ID_BYTE1` | 0xa | established | shipped IRIX kernel binaries, per docs/nic.md | required value of record byte 1 |
| `MAC_RECORD_LEN` | 0xe | established | shipped IRIX kernel binaries, per docs/nic.md | bytes read from the part |
