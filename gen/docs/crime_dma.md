# Pixel-DMA request the IRIX kernel builds for the O2 renderer

<!-- generated from defs/crime_dma.toml by defs-gen - do not edit, change the source -->

Software table, not a register map: the size of the kernel's pixel-DMA request, how long it polls for idle and the ioctl request number that carries it. The CRIME registers it drives are described in the crime repository.

Unit `crime_dma`, macro prefix `CRIME`.
Default source: shipped IRIX O2 graphics objects, per docs/crime.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `DMA_IOCTL` | 0x55f2 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Request number of the kernel pixel-DMA ioctl |
| `DMA_POLL_LIMIT` | 0x7a120 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Polls the kernel makes for idle before the pixel DMA gives up |
| `DMA_REQ_BYTES` | 0x28 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Size of the kernel pixel-DMA request |
