# O2 GBE capture block as the IRIX video driver programs it

<!-- generated from defs/gbe_capture.toml by defs-gen - do not edit, change the source -->

The nine capture control words the video driver clears and loads for a capture path. Offsets are from the capture block base, which is a fixed displacement in the GBE register block. Prose and the reasoning are in docs/mace-video.md.

Unit `gbe_capture`, macro prefix `GBE_CAPTURE`.
Default source: shipped IRIX O2 video driver object, per docs/mace-video.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `vc` | 0x0 | rw | 32 | 9 |  | unknown | inferred | shipped IRIX O2 video driver object, per docs/mace-video.md |

### Register `vc`

Capture control words VC_0 to VC_8, as the reading names them. The reading gives the first and last word offsets, 0x80000 and 0x80020, so the 4-byte stride is inferred. The reading established that VC_0 to VC_3 are written as zero when a capture path stops. Its word width is inferred from the word-sized copies the start-up makes. VC_2 bit 0 enables DMA and VC_3 holds a path byte, both inferred from the start routine.

Array of 9 registers, stride 4.
