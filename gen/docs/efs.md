# Extent File System on-disk structures

<!-- generated from defs/efs.toml by defs-gen - do not edit, change the source -->

Extent File System on-disk structures: superblock, inode, extent and directory block.

Unit `efs`, macro prefix `EFS`.
Default source: shipped IRIX binaries, per docs/formats/efs.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `DB_W0` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DEV_W8` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DEV_W9` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W0` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W1` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W2` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W3` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W4` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W5` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W6` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `DI_W7` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `EXT_W0` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `EXT_W1` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W0` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W1` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W11` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W12` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W13` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W14` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W15` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W16` | 32 | inferred | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W2` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W22` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W3` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W4` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W5` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W6` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |
| `SB_W7` | 32 | established | shipped IRIX binaries, per docs/formats/efs.md |

### Word `DB_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `magic` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 15:8 | `firstused` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 7:0 | `slots` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DEV_W8`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `odev` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DEV_W9`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `ndev` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:28 | `mode_type` | established | fifo=1, chr=2, x3=3, dir=4, blk=6, x7=7, reg=8, lnk=10, sock=12 | shipped IRIX binaries, per docs/formats/efs.md |  |
| 27:16 | `mode_perm` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 15:0 | `nlink` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `uid` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 15:0 | `gid` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W2`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `size` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W3`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `atime` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W4`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `mtime` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W5`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `ctime` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W6`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `gen` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `DI_W7`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `numextents` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 15:8 | `version` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 7:0 | `spare` | inferred |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `EXT_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `magic` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 23:0 | `bn` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `EXT_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `length` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 23:0 | `offset` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `size` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `firstcg` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W11`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `bmsize` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W12`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `tfree` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W13`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `tinode` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `bmblock` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W15`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `replsb` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W16`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `lastialloc` | inferred |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W2`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `cgfsize` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W22`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `checksum` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W3`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `cgisize` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 15:0 | `sectors` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W4`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `heads` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |
| 15:0 | `ncg` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W5`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `dirty` | established | clean=0, dirty=2989, needsck=4660, mounted=30583 | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W6`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `time` | established |  | shipped IRIX binaries, per docs/formats/efs.md |  |

### Word `SB_W7`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `magic` | established | old=469337, new=469338 | shipped IRIX binaries, per docs/formats/efs.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `BBSIZE` | 0x200 | established | shipped IRIX binaries, per docs/formats/efs.md | block size in bytes |
| `BITMAP_BB` | 0x2 | established | shipped IRIX binaries, per docs/formats/efs.md | bitmap start when the superblock bitmap block is 0 |
| `DB_SIZE` | 0x200 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `DENT_HEADER_BYTES` | 0x5 | established | shipped IRIX binaries, per docs/formats/efs.md | inode number plus name length byte |
| `DEV_LONG_FORM` | 0xffff | established | shipped IRIX binaries, per docs/formats/efs.md | short device value that selects the long form |
| `DIRBLK_HEADERSIZE` | 0x4 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `DIRBLK_MAGIC` | 0xbeef | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `DIRBLK_MAXSLOTS` | 0x48 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `DIRECTEXTENTS` | 0xc | established | shipped IRIX binaries, per docs/formats/efs.md | extents that fit in the inode data area |
| `DI_SIZE` | 0x80 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `EXT_SIZE` | 0x8 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `EX_MAXLEN` | 0xf8 | established | shipped IRIX binaries, per docs/formats/efs.md | longest run in blocks |
| `INODES_PER_BB` | 0x4 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `INODE_DATA_BYTES` | 0x60 | established | shipped IRIX binaries, per docs/formats/efs.md | inode data area, from byte 0x20 to 0x7f |
| `INODE_SIZE` | 0x80 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `MAGIC_NEW` | 0x7295a | established | shipped IRIX binaries, per docs/formats/efs.md | bitmap and size placed past the last group |
| `MAGIC_OLD` | 0x72959 | established | shipped IRIX binaries, per docs/formats/efs.md | carried by every held image |
| `ROOT_INO` | 0x2 | established | shipped IRIX binaries, per docs/formats/efs.md | inodes 0 and 1 are never allocated |
| `SB_LEN_FNAME` | 0x6 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `SB_LEN_FPACK` | 0x6 | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `SB_LEN_SPARE` | 0x14 | inferred | shipped IRIX binaries, per docs/formats/efs.md |  |
| `SB_OFF_FNAME` | 0x20 | established | shipped IRIX binaries, per docs/formats/efs.md | filesystem name, not NUL terminated |
| `SB_OFF_FPACK` | 0x26 | established | shipped IRIX binaries, per docs/formats/efs.md | pack name, not NUL terminated |
| `SB_OFF_SPARE` | 0x44 | inferred | shipped IRIX binaries, per docs/formats/efs.md | zero in every image |
| `SB_SIZE` | 0x5c | established | shipped IRIX binaries, per docs/formats/efs.md |  |
| `SUPER_BB` | 0x1 | established | shipped IRIX binaries, per docs/formats/efs.md | block 0 is an unused boot block |

## Test vectors

A vector is a value recorded from a source. An example only shows the layout.

| Target | Value | Kind | Decoded |
|---|---|---|---|
| `DB_W0` | 0xbeef0300 | example | magic=48879, firstused=3, slots=0 |
| `EXT_W0` | 0x19b2 | example | magic=0, bn=6578 |
| `EXT_W1` | 0x1000001 | example | length=1, offset=1 |
| `SB_W5` | 0x77770000 | example | dirty=30583 |
