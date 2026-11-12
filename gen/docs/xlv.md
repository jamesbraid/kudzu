# XLV logical volume manager on-disk label structures

<!-- generated from defs/xlv.toml by defs-gen - do not edit, change the source -->

XLV logical volume manager on-disk label: master block, label slot header and per-partition entry.

Unit `xlv`, macro prefix `XLV`.
Default source: shipped IRIX binaries, per docs/formats/xlv.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `E_W0` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W25` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W27` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W28` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W29` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W42` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W43` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W56` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W57` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W58` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W59` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W60` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W61` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W66` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W67` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `E_W68` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `LBL_W0` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `LBL_W1` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `LBL_W2` | 32 | unknown | shipped IRIX binaries, per docs/formats/xlv.md |
| `LBL_W3` | 32 | unknown | shipped IRIX binaries, per docs/formats/xlv.md |
| `LBL_W4` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `LBL_W5` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `MASTER_W0` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |
| `MASTER_W1` | 32 | established | shipped IRIX binaries, per docs/formats/xlv.md |

### Word `E_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `vol_flags` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W25`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31 | `sv_flags_hi_no_err_retry` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 30:16 | `sv_flags_hi_other` | unknown |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 15:0 | `sv_flags_hi_low` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W27`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `time` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W28`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `vol_attr16` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W29`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `sv_type` | established | log=1, data=2, rt=3 | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 15:0 | `sv_depth` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W42`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `sv_nplex` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 23:16 | `plex_index` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 15:0 | `plex_nve` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W43`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `plex_flags` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W56`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `ve_state` | established | empty=0, clean=1, active=2, stale=3, offline=4, incomplete=5 | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 23:16 | `ve_type` | established | stripe=1, concat=2 | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 15:8 | `ve_nparts` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 7:0 | `ve_index` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W57`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `ve_stripe_unit` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W58`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `ve_start_hi` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W59`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `ve_start_lo` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W60`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `ve_end_hi` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W61`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `ve_end_lo` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W66`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `part_dev` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W67`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `part_size` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `E_W68`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:24 | `part_index` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `LBL_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `magic` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `LBL_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:16 | `version` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 15:8 | `nparts` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 7:0 | `reserved` | unknown |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `LBL_W2`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `timestamp_hi` | unknown |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `LBL_W3`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `timestamp_lo` | unknown |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `LBL_W4`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `trans_id_hi` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `LBL_W5`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `trans_id_lo` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `MASTER_W0`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:0 | `seq_hi` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

### Word `MASTER_W1`

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 31:1 | `seq_lo_rest` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |
| 0 | `seq_lo_slot` | established |  | shipped IRIX binaries, per docs/formats/xlv.md |  |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `ENTRY_OFF` | 0x120 | established | shipped IRIX binaries, per docs/formats/xlv.md | offset of entry 0 in a slot |
| `ENTRY_SIZE` | 0x118 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_DATA_SV_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_LOG_SV_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_PART_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_PLEX_NAME` | 0x20 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_PLEX_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_RT_SV_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_VE_NAME` | 0x20 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_VE_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_VOL_NAME` | 0x20 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_LEN_VOL_UUID` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_OFF_DATA_SV_UUID` | 0x24 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_OFF_LOG_SV_UUID` | 0x14 | established | shipped IRIX binaries, per docs/formats/xlv.md | nil if the volume has no log subvolume |
| `E_OFF_PART_UUID` | 0xf8 | established | shipped IRIX binaries, per docs/formats/xlv.md | nil means the entry is unused, opaque bytes |
| `E_OFF_PLEX_NAME` | 0x88 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_OFF_PLEX_UUID` | 0x78 | established | shipped IRIX binaries, per docs/formats/xlv.md | opaque bytes |
| `E_OFF_RT_SV_UUID` | 0x34 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_OFF_VE_NAME` | 0xc0 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `E_OFF_VE_UUID` | 0xb0 | established | shipped IRIX binaries, per docs/formats/xlv.md | opaque bytes |
| `E_OFF_VOL_NAME` | 0x44 | established | shipped IRIX binaries, per docs/formats/xlv.md | volume name, NUL padded, every byte may be used |
| `E_OFF_VOL_UUID` | 0x4 | established | shipped IRIX binaries, per docs/formats/xlv.md | volume UUID, opaque bytes |
| `E_SIZE` | 0x118 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `HDR_SIZE` | 0x120 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `LABEL_BYTES` | 0x12a0 | established | shipped IRIX binaries, per docs/formats/xlv.md | used bytes of one label slot |
| `LABEL_MAGIC` | 0x11201989 | established | shipped IRIX binaries, per docs/formats/xlv.md | bytes 11 20 19 89 |
| `LABEL_VERSION` | 0x2 | established | shipped IRIX binaries, per docs/formats/xlv.md | the value writers store; no reader checks it |
| `LBL_LEN_NODENAME` | 0x108 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `LBL_OFF_NODENAME` | 0x18 | established | shipped IRIX binaries, per docs/formats/xlv.md | owning host name; writers copy 0x102 bytes |
| `LBL_SIZE` | 0x120 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `MASTER_BYTES` | 0x200 | established | shipped IRIX binaries, per docs/formats/xlv.md | master block size before rounding to the sector size |
| `NAME_LEN` | 0x20 | established | shipped IRIX binaries, per docs/formats/xlv.md | bytes in a volume, plex or element name |
| `NODENAME_COPY` | 0x102 | established | shipped IRIX binaries, per docs/formats/xlv.md | bytes the writers copy into the host name |
| `NODENAME_LEN` | 0x108 | established | shipped IRIX binaries, per docs/formats/xlv.md | bytes reserved for the owning host name |
| `NPART` | 0x10 | established | shipped IRIX binaries, per docs/formats/xlv.md | entries per label, one per partition |
| `PTYPE_EFS` | 0x7 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `PTYPE_EFS_LEGACY` | 0x5 | established | shipped IRIX binaries, per docs/formats/xlv.md | type 5 is mapped to the name efs by the shipped name table |
| `PTYPE_RAW` | 0x3 | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `PTYPE_VOLHDR` | 0x0 | established | shipped IRIX binaries, per docs/formats/xlv.md | volume-header partition type of the header partition |
| `PTYPE_VOLUME` | 0x6 | established | shipped IRIX binaries, per docs/formats/xlv.md | whole disk |
| `PTYPE_VXVM` | 0xe | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `PTYPE_XFS` | 0xa | established | shipped IRIX binaries, per docs/formats/xlv.md |  |
| `PTYPE_XFSLOG` | 0xb | established | shipped IRIX binaries, per docs/formats/xlv.md | accepted for a log subvolume |
| `PTYPE_XLV` | 0xc | established | shipped IRIX binaries, per docs/formats/xlv.md | accepted for a log, data or real-time subvolume |
| `PTYPE_XVM` | 0xd | established | shipped IRIX binaries, per docs/formats/xlv.md |  |

## Test vectors

A vector is a value recorded from a source. An example only shows the layout.

| Target | Value | Kind | Decoded |
|---|---|---|---|
| `E_W25` | 0x80000000 | vector | sv_flags_hi_no_err_retry=1, sv_flags_hi_other=0, sv_flags_hi_low=0 |
| `E_W56` | 0x2010302 | example | ve_state=2, ve_type=1, ve_nparts=3, ve_index=2 |
