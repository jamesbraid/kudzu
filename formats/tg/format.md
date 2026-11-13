<!-- generated from formats/tg/format.toml by defs-gen - do not edit, change the source -->

# gigabit Ethernet controller match table of the Origin 300/350 driver

Software table, not a register map: the PCI vendor, device and subsystem ids the Origin 300/350 gigabit Ethernet driver accepts.

Unit `tg`, macro prefix `TG`.
Default source: shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `ROW_W0` | 32 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |
| `ROW_W1` | 32 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |

### Word `ROW_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `vendor` | established |  | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |
| 15:0 | `device` | established |  | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |

### Word `ROW_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `subsys_vendor` | established |  | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |
| 15:0 | `subsys_device` | established |  | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `CFG_OFF_DEVICE` | 0x0 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |
| `CFG_OFF_SUBSYS_DEVICE` | 0x2e | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |
| `CFG_OFF_SUBSYS_VENDOR` | 0x2c | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |
| `CFG_OFF_VENDOR` | 0x2 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md |  |
| `MATCH_ROW_COUNT` | 0x5 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | rows the attach loop walks |
| `MATCH_ROW_STRIDE` | 0x10 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | bytes between table rows |
| `PCI_DEVICE_1645` | 0x1645 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | three rows (subsystem 0x8010, 0x8011, 0x8012) |
| `PCI_DEVICE_1648` | 0x1648 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | one row (subsystem 0x8013) |
| `PCI_DEVICE_16A8` | 0x16a8 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | one row (subsystem 0x8014) |
| `PCI_SUBSYS_DEVICE_FIRST` | 0x8010 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | subsystem device of the first row; the rows run consecutively to 0x8014 |
| `PCI_SUBSYS_VENDOR` | 0x10a9 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | the subsystem vendor of every row |
| `PCI_VENDOR` | 0x14e4 | established | shipped IRIX kernel binaries, per software/kernel/ip35-tg-driver.md | the vendor id of every row |
