<!-- generated from formats/scsi/format.toml by defs-gen - do not edit, change the source -->

# SCSI command opcodes the CD-ROM driver selects by drive class

Unit `scsi`, macro prefix `SCSI`.
Default source: shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `CDROM_VENDOR_C4` | 0xc4 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: sent by classes 1 to 3 in place of START STOP UNIT; use as software eject is inferred |
| `CDROM_VENDOR_C9` | 0xc9 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: sent only by classes 1 to 3; use as a mode switch is inferred |
| `INQ_LEN_PRODUCT` | 0x10 | inferred | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | inferred |
| `INQ_LEN_VENDOR` | 0x8 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: the notes and both readers treat vendor and product as one 24-byte text |
| `INQ_OFF_PRODUCT` | 0x10 | inferred | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | inferred: follows the vendor field in that text |
| `INQ_OFF_TYPE` | 0x0 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: device type byte; the PROMs test it for 0 and the daemon accepts only 5 |
| `INQ_OFF_VENDOR` | 0x8 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: first byte of the vendor text, the start of the text the daemon matches |
| `INQ_OFF_VERSION` | 0x2 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: version byte; the daemon masks it with 7 and tests for 2 to refine one drive family |
| `INQ_TYPE_CDROM` | 0x5 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: the daemon accepts it; the PROMs never compare the text of a type 5 unit |
| `INQ_TYPE_DISK` | 0x0 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: the PROMs test for this value before comparing the text |
| `INQ_TYPE_TAPE` | 0x1 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: named by the O2 PROM's probe; no other PROM tests it |
| `INQ_VERSION_MASK` | 0x7 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established |
| `INQ_VERSION_REFINE` | 0x2 | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established |
| `PROM_PREFIX_LEN` | 0xa | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: bytes of the trimmed vendor, separator and product that are compared |
| `START_STOP_UNIT` | 0x1b | established | shipped IRIX binaries, per docs/scsi-cdrom-inquiry.md | established: sent by every class except the first three |
