# hardware configuration board record

<!-- generated from defs/klconfig.toml by defs-gen - do not edit, change the source -->

Hardware configuration records: the board, component, console, memory and global-data fields the kernel reads to find boards, slots, modules and components.

Unit `klconfig`, macro prefix `KLCONFIG`.
Default source: shipped IRIX kernel binaries, per docs/klconfig.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `BRD_W0` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `BRD_W1` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `BRD_W2` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `BRD_W3` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `BRD_W5` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `BRD_W8` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `COMP_W0` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `COMP_W6` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `CONS_W2` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `CONS_W3` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `CONS_W4` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `CONS_W5` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `CONS_W8` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `GDA_W97` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `MEM_W14` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `NODECLASS_W1` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |
| `PEPAIR_W0` | 32 | established | shipped IRIX kernel binaries, per docs/klconfig.md |

### Word `BRD_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `next` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `BRD_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 23:20 | `class` | established | node=1, io=2, router=3, graphics=5 | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| 19:16 | `type_low` | inferred |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `BRD_W2`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 21 | `flags_master_baseio` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| 15:8 | `slot` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `BRD_W3`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15:0 | `module` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `BRD_W5`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 7:0 | `numcompts` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `BRD_W8`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `nasid` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `COMP_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `struct_type` | established | hub=2, memory=3, io=5, router=9, graphics=10 | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| 8 | `flags_enabled` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `COMP_W6`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `physid` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `CONS_W2`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `config_base_hi` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `CONS_W3`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `config_base_lo` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `CONS_W4`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `memory_base_hi` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `CONS_W5`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `memory_base_widget` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| 23:0 | `memory_base_rest` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `CONS_W8`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `nasid` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| 7:0 | `npci` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `GDA_W97`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `pebrick_marker` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `MEM_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15:0 | `dimm_select` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `NODECLASS_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15:12 | `class` | established | pebrick=12 | shipped IRIX kernel binaries, per docs/klconfig.md |  |

### Word `PEPAIR_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `pe_nasid` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| 15:0 | `host_nasid` | established |  | shipped IRIX kernel binaries, per docs/klconfig.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `BRD_OFF_COMPTS` | 0x24 | established | shipped IRIX kernel binaries, per docs/klconfig.md | first component offset, one 32-bit word each |
| `BRD_STRIDE_COMPTS` | 0x4 | established | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| `DIR_CONFIG_OFFSET_MASK` | 0xffffffffff | established | shipped IRIX kernel binaries, per docs/klconfig.md | 40 bits are kept |
| `DIR_OFF` | 0x2000 | established | shipped IRIX kernel binaries, per docs/klconfig.md | directory offset from the node base |
| `DIR_OFF_CONFIG_OFFSET` | 0x48 | established | shipped IRIX kernel binaries, per docs/klconfig.md | byte of the doubleword that gives the configuration header offset |
| `GDA_COUNT_NASIDTABLE_IP27` | 0x40 | established | shipped IRIX kernel binaries, per docs/klconfig.md | entries the Origin 200/2000 kernel reads at most |
| `GDA_COUNT_NASIDTABLE_IP35` | 0x80 | established | shipped IRIX kernel binaries, per docs/klconfig.md | entries the Origin 300/350 kernel reads at most |
| `GDA_COUNT_PEBRICK` | 0x4 | established | shipped IRIX kernel binaries, per docs/klconfig.md | pairs read at most |
| `GDA_OFF_NASIDTABLE` | 0x80 | established | shipped IRIX kernel binaries, per docs/klconfig.md | signed halfword node number of each compact node; the first -1 ends the list |
| `GDA_OFF_PEBRICK` | 0x188 | established | shipped IRIX kernel binaries, per docs/klconfig.md | Origin 300/350 only, pairs of signed halfwords; a first halfword of -1 ends the table |
| `GDA_PEBRICK_MAGIC` | 0x5045626b | established | shipped IRIX kernel binaries, per docs/klconfig.md | the word at byte 388 equals this when the pair table follows |
| `GDA_STRIDE_NASIDTABLE` | 0x2 | established | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| `GDA_STRIDE_PEBRICK` | 0x4 | established | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| `HDR_OFF_BOARD_INFO` | 0x14 | established | shipped IRIX kernel binaries, per docs/klconfig.md | word giving the offset of the first board record |
| `MEM_COUNT_BNKSZ` | 0x8 | established | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| `MEM_OFF_BNKSZ` | 0x3c | established | shipped IRIX kernel binaries, per docs/klconfig.md | bank sizes in megabytes, signed halfwords |
| `MEM_STRIDE_BNKSZ` | 0x2 | established | shipped IRIX kernel binaries, per docs/klconfig.md |  |
| `NODECLASS_OFF` | 0x4000000 | established | shipped IRIX kernel binaries, per docs/klconfig.md | offset of the register in the node's local register space |
| `NODECLASS_SPACE_LOCAL` | 0x10000000 | established | shipped IRIX kernel binaries, per docs/klconfig.md | added to the offset for the local alias |
| `NODECLASS_SPACE_REMOTE` | 0x30000000 | established | shipped IRIX kernel binaries, per docs/klconfig.md | added to the offset, with the node number, for the node-indexed alias |
