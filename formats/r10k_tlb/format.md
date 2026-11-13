<!-- generated from formats/r10k_tlb/format.toml by defs-gen - do not edit, change the source -->

# The wired-entry layout IRIX uses in the R10000 TLB

Software table, not a register map: the wired-entry layout and the page and ASID policy IRIX uses in its 64-bit kernel. The TLB entry count and ASID width of the R10000 itself are in the chips repository (unit r10000_cp0). Prose and the reasoning are in formats/r10k_tlb/README.md.

Unit `r10k_tlb`, macro prefix `R10K_TLB`.
Default source: IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `ASID_USABLE` | 0xff | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | ASIDs the kernel hands out; ASID 0 is reserved for the kernel and system processes |
| `BASE_PAGE_BYTES` | 0x4000 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | base page size of the 64-bit IRIX kernel; the 4 KB path is compiled out |
| `BROADCAST_SHARERS` | 0x10 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | address spaces with more sharers than this get a broadcast full flush instead of per-page probes |
| `IO_MAP_BYTES` | 0x1000000 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | size of the entries installed for I/O DMA-map misses |
| `KTEXT_BYTES` | 0x1000000 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | size of the single global kernel text and data entry on the mapped kernel |
| `LARGE_PAGE_MIN_BYTES` | 0x10000 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | smallest large page the kernel forms |
| `WIRED_INDEX_CPU` | 0x0 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | wired index holding the per-CPU data area with the thread's user page and kernel stack |
| `WIRED_INDEX_DROPIN_FIRST` | 0x2 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | first wired index of the per-process second-level drop-in slots |
| `WIRED_INDEX_DROPIN_LAST` | 0x6 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | last wired index of the per-process second-level drop-in slots |
| `WIRED_INDEX_KTEXT` | 0x1 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | wired index holding kernel text and data on the mapped kernel |
| `WIRED_MAPPED` | 0x7 | inferred | IRIX kernel translation-handling reading, one release outside the shipped-binary inventory; the R10000 geometry is checked against the published manual | wired entries on the mapped kernel |
