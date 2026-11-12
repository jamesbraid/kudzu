# MACE peripheral two-wire bus block as the IRIX video driver drives it

<!-- generated from defs/mace_two_wire.toml by defs-gen - do not edit, change the source -->

Configuration, control and data words of the MACE peripheral's two-wire bus, which the video driver uses to program video-port chips. The bus is base 0x1f330000. Prose and the reasoning are in docs/mace-video.md.

Unit `mace_two_wire`, macro prefix `MACE_TWO_WIRE`.
Default source: shipped IRIX O2 video driver object, per docs/mace-video.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `tw_config` | 0x0 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `tw_control` | 0x10 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |
| `tw_data` | 0x18 | rw | 64 |  |  | unknown | established | shipped IRIX O2 video driver object, per docs/mace-video.md |

### Register `tw_config`

Bus configuration word. The configuration helper reads it and writes it back with the bits selected by its mask replaced. The driver writes 1 to request a bus reset. Meanings of the other values written are not recorded.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 3:0 | `low_config` | established |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Low four bits, the only bits the configuration helper changes |

### Register `tw_control`

Bus control and status word. Transfer routines poll the status bits below. Writes of 1, 3 and 5 are commands, and their meanings are inferred in docs/mace-video.md.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 7 | `bus_error` | established |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Bus error. Only the force-idle path tests it in the reading. |
| 5 | `ack_missing` | established |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Acknowledge not received from the addressed chip |
| 4 | `transfer_busy` | established |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Transfer busy |
| 0 | `not_idle` | established |  | shipped IRIX O2 video driver object, per docs/mace-video.md | Bus not idle. Also set by the force-idle command, which stores its argument with this bit cleared. |

### Register `tw_data`

Data word. The read and write routines store the chip address byte and the register number byte through it, and read it back after the transfer. How its bytes map to bits is not recorded.
