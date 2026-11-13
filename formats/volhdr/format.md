<!-- generated from formats/volhdr/format.toml by defs-gen - do not edit, change the source -->

# SGI disk volume header

SGI disk volume header: magic, drive parameters, volume directory and partition table.

Unit `volhdr`, macro prefix `DVH`.
Default source: shipped IRIX binaries, per formats/volhdr/README.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `DIRENT_W2` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `DIRENT_W3` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W0` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W1` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W10` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W11` | 32 | inferred | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W126` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W17` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W7` | 32 | unknown | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W8` | 32 | unknown | shipped IRIX binaries, per formats/volhdr/README.md |
| `HDR_W9` | 32 | inferred | shipped IRIX binaries, per formats/volhdr/README.md |
| `PART_W0` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `PART_W1` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |
| `PART_W2` | 32 | established | shipped IRIX binaries, per formats/volhdr/README.md |

### Word `DIRENT_W2`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `start_block` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `DIRENT_W3`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `length_bytes` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `magic` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `root` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |
| 15:0 | `swap` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W10`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `dp_secbytes` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W11`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `dp_flags` | inferred |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W126`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `checksum` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W17`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `dp_capacity` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W7`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `dp_cylinders` | inferred |  | shipped IRIX binaries, per formats/volhdr/README.md |  |
| 15:0 | `dp_head_offset` | unknown |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W8`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `dp_heads` | inferred |  | shipped IRIX binaries, per formats/volhdr/README.md |  |
| 15:8 | `dp_tag_depth` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |
| 7:0 | `dp_cyl_high` | unknown |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `HDR_W9`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 15:0 | `dp_sectors` | inferred |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `PART_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `nblks` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `PART_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `first` | established |  | shipped IRIX binaries, per formats/volhdr/README.md |  |

### Word `PART_W2`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `type` | established | volhdr=0, trkrepl=1, secrepl=2, raw=3, bsd=4, sysv=5, volume=6, efs=7, lvol=8, rlvol=9, xfs=10, xfslog=11, xlv=12, xvm=13, vxvm=14 | shipped IRIX binaries, per formats/volhdr/README.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `BOOTFILE_OFF` | 0x8 | established | shipped IRIX binaries, per formats/volhdr/README.md |  |
| `CSUM_OFF` | 0x1f8 | established | shipped IRIX binaries, per formats/volhdr/README.md | checksum word |
| `DIRENT_LEN_NAME` | 0x8 | established | shipped IRIX binaries, per formats/volhdr/README.md |  |
| `DIRENT_OFF_NAME` | 0x0 | established | shipped IRIX binaries, per formats/volhdr/README.md | file name, NUL padded, not terminated at 8 characters |
| `DIRENT_SIZE` | 0x10 | established | shipped IRIX binaries, per formats/volhdr/README.md |  |
| `DIR_COUNT` | 0xf | established | shipped IRIX binaries, per formats/volhdr/README.md | directory entries |
| `DIR_NAME_LEN` | 0x8 | established | shipped IRIX binaries, per formats/volhdr/README.md | name bytes in a directory entry |
| `DIR_OFF` | 0x48 | established | shipped IRIX binaries, per formats/volhdr/README.md | start of the volume directory |
| `DIR_STRIDE` | 0x10 | established | shipped IRIX binaries, per formats/volhdr/README.md | bytes per directory entry |
| `DP_BYTES` | 0x30 | established | shipped IRIX binaries, per formats/volhdr/README.md |  |
| `DP_FLAG_TAGGED` | 0x40 | inferred | shipped IRIX binaries, per formats/volhdr/README.md | set together with the tag depth |
| `DP_OFF` | 0x18 | established | shipped IRIX binaries, per formats/volhdr/README.md | drive parameters, 48 bytes |
| `HDR_LEN_BOOTFILE` | 0x10 | established | shipped IRIX binaries, per formats/volhdr/README.md |  |
| `HDR_OFF_BOOTFILE` | 0x8 | established | shipped IRIX binaries, per formats/volhdr/README.md | boot file name, NUL padded, first byte 0 means none |
| `HDR_SIZE` | 0x200 | established | shipped IRIX binaries, per formats/volhdr/README.md |  |
| `MAGIC` | 0xbe5a941 | established | shipped IRIX binaries, per formats/volhdr/README.md | bytes 0b e5 a9 41 |
| `PART_COUNT` | 0x10 | established | shipped IRIX binaries, per formats/volhdr/README.md | partition entries |
| `PART_OFF` | 0x138 | established | shipped IRIX binaries, per formats/volhdr/README.md | start of the partition table |
| `PART_SIZE` | 0xc | established | shipped IRIX binaries, per formats/volhdr/README.md |  |
| `PART_STRIDE` | 0xc | established | shipped IRIX binaries, per formats/volhdr/README.md | bytes per partition entry |
| `SIZE` | 0x200 | established | shipped IRIX binaries, per formats/volhdr/README.md | bytes in the header |
| `SLOT_ROOT` | 0x0 | established | shipped IRIX binaries, per formats/volhdr/README.md | conventional root partition slot |
| `SLOT_SWAP` | 0x1 | established | shipped IRIX binaries, per formats/volhdr/README.md | conventional swap partition slot |
| `SLOT_VOLHDR` | 0x8 | established | shipped IRIX binaries, per formats/volhdr/README.md | conventional volume-header partition slot |
| `SLOT_WHOLEDISK` | 0xa | established | shipped IRIX binaries, per formats/volhdr/README.md | conventional whole-disk partition slot |

## Test vectors

A vector is a value recorded from a source. An example only shows the layout.

| Target | Value | Kind | Decoded |
|---|---|---|---|
| `DIRENT_W3` | 0x41400 | vector | length_bytes=267264 |
| `PART_W0` | 0x1000 | vector | nblks=4096 |
| `PART_W0` | 0x40000 | vector | nblks=262144 |
| `PART_W1` | 0x41000 | vector | first=266240 |
| `PART_W2` | 0xa | example | type=10 |
