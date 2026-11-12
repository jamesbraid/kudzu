# XVM volume manager on-disk label structures

<!-- generated from defs/xvm.toml by defs-gen - do not edit, change the source -->

XVM volume manager on-disk label structures: the physical label, label sector header, element and attribute entries.

Unit `xvm`, macro prefix `XVM`.
Default source: shipped IRIX binaries, per docs/formats/xvm.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `ATTR_PERM_W6` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `ATTR_PERM_W7` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `ATTR_PERM_W8` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `ATTR_W0` | 32 | inferred | shipped IRIX binaries, per docs/formats/xvm.md |
| `ATTR_W1` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `LH_W0` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `LH_W1` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `PL_W0` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `PL_W1` | 32 | inferred | shipped IRIX binaries, per docs/formats/xvm.md |
| `PL_W14` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `PL_W15` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `PL_W16` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `PL_W17` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `PL_W30` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_MIRROR_W14` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_MIRROR_W15` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_MIRROR_W16` | 32 | inferred | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_RAID_W14` | 32 | inferred | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_RAID_W15` | 32 | inferred | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_SLICE_W14` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_SLICE_W15` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_SLICE_W16` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_STRIPE_W14` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_SUBVOL_W14` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_VOL_W14` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_W0` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_W1` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_W10` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_W11` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_W12` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |
| `VE_W13` | 32 | established | shipped IRIX binaries, per docs/formats/xvm.md |

### Word `ATTR_PERM_W6`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `uid` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `ATTR_PERM_W7`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `gid` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `ATTR_PERM_W8`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `mode` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `ATTR_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `type` | established | free=0, name=1, perm=2 | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 16 | `flag_persistent` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `ATTR_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `tid` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `LH_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `magic` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `LH_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `version` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:8 | `type` | established | free=0, reserved=1, ve=2, attr=3 | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 7:0 | `size` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `PL_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `magic` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `PL_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `version` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 14 | `state_draining` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 13 | `state_cluster` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 12 | `state_private` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 11 | `state_recovering` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 10 | `state_mediaerror` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 9 | `state_spare` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 8 | `state_online` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 3 | `flags_sync` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 2 | `flags_duplicate` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 1 | `flags_bootable` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `PL_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `secsize` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:0 | `xvmlabel` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `PL_W15`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `xvmlabelblks` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:0 | `dbn` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `PL_W16`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `nblks` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `PL_W17`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `timestamp` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `PL_W30`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `clusterid` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_MIRROR_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `oldlength_hi` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_MIRROR_W15`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `oldlength_lo` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_MIRROR_W16`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `logfreq` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:8 | `policy` | established | round_robin=0, sequential=1 | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 7:0 | `config` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_RAID_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `susize` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_RAID_W15`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `layout` | inferred |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_SLICE_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `startblk_hi` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_SLICE_W15`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `startblk_lo` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_SLICE_W16`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `seq` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:8 | `flags` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 7:0 | `partition` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_STRIPE_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `susize` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_SUBVOL_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `svtype` | established | all=0, log=1, data=2, rt=3, swap=4, user1=16 | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_VOL_W14`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `repfull` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `type` | established | free=0, slice=1, concat=2, stripe=3, raid=4, mirror=5, subvol=6, volume=7, cow=11, snapshot=12 | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 20 | `state_disabled` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 17 | `state_valid` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 16 | `state_clean` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:0 | `pieces` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `tid` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_W10`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `length_hi` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_W11`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `length_lo` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_W12`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `pos` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:0 | `cowfirst` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

### Word `VE_W13`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `cownext` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |
| 15:0 | `cowregsz` | established |  | shipped IRIX binaries, per docs/formats/xvm.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `ATTR_LEN_NAME` | 0x30 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `ATTR_LEN_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `ATTR_OFF_NAME` | 0x18 | established | shipped IRIX binaries, per docs/formats/xvm.md | for a name entry: NUL padded string |
| `ATTR_OFF_UUID` | 0x8 | established | shipped IRIX binaries, per docs/formats/xvm.md | the object named or protected, opaque bytes |
| `ATTR_SIZE` | 0x48 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `DISKNAME_MAX` | 0x30 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `HOSTNAME_MAX` | 0x20 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `LABELHDR_MAGIC` | 0x6c646872 | established | shipped IRIX binaries, per docs/formats/xvm.md | bytes 6c 64 68 72 |
| `LABELHDR_VERSION` | 0x2 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `LABELHDR_VERSION_LEGACY` | 0x1 | established | shipped IRIX binaries, per docs/formats/xvm.md | accepted for element sectors only |
| `LABENT_SIZE` | 0x48 | established | shipped IRIX binaries, per docs/formats/xvm.md | bytes per entry in a label sector |
| `LH_SIZE` | 0x8 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `NAME_MAX` | 0x30 | established | shipped IRIX binaries, per docs/formats/xvm.md | bytes in an object name |
| `PHYSLAB_GPT_FIRST_PARTITION_INDEX` | 0x0 | established | shipped IRIX binaries, per docs/formats/xvm.md | on a GPT disk the label is at the first LBA of the first partition-list entry, in units of the device block size |
| `PHYSLAB_GPT_MIN_PARTITIONS` | 0x2 | established | shipped IRIX binaries, per docs/formats/xvm.md | the GPT placement is tried only when the partition list has a second entry; with one entry no label is found |
| `PHYSLAB_MAGIC` | 0x786c6162 | established | shipped IRIX binaries, per docs/formats/xvm.md | bytes 78 6c 61 62 |
| `PHYSLAB_START_BLOCK` | 0x1 | established | shipped IRIX binaries, per docs/formats/xvm.md | device block holding the label on a disk with an SGI volume header |
| `PHYSLAB_VERSION` | 0x2 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `PHYSLAB_VERSION_LEGACY` | 0x1 | established | shipped IRIX binaries, per docs/formats/xvm.md | converted on read |
| `PL_LEN_DISKNAME` | 0x30 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `PL_LEN_HOSTNAME` | 0x20 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `PL_LEN_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `PL_OFF_DISKNAME` | 0x48 | established | shipped IRIX binaries, per docs/formats/xvm.md | physical volume name, NUL padded |
| `PL_OFF_HOSTNAME` | 0x8 | established | shipped IRIX binaries, per docs/formats/xvm.md | owning host in the private domain; a version 1 label held the disk name here |
| `PL_OFF_UUID` | 0x28 | established | shipped IRIX binaries, per docs/formats/xvm.md | physical volume UUID, opaque bytes |
| `PL_SIZE` | 0x7c | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `VE_COW_LEN_RUUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `VE_COW_OFF_RUUID` | 0x38 | established | shipped IRIX binaries, per docs/formats/xvm.md | repository object UUID, opaque bytes; no word fields |
| `VE_LEN_CHILDUUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `VE_LEN_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |
| `VE_OFF_CHILDUUID` | 0x18 | established | shipped IRIX binaries, per docs/formats/xvm.md | the child at the piece position, opaque bytes |
| `VE_OFF_UUID` | 0x8 | established | shipped IRIX binaries, per docs/formats/xvm.md | this object, opaque bytes |
| `VE_SIZE` | 0x48 | established | shipped IRIX binaries, per docs/formats/xvm.md |  |

## Test vectors

A vector is a value recorded from a source. An example only shows the layout.

| Target | Value | Kind | Decoded |
|---|---|---|---|
| `LH_W1` | 0x20348 | example | version=2, type=3, size=72 |
| `VE_W0` | 0x3110002 | example | type=3, state_clean=1, state_valid=0, state_disabled=1, pieces=2 |
