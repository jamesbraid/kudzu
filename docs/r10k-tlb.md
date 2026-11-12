# R10000 TLB as IRIX 6.5.7 uses it on IP27

This page records how the IRIX 6.5.7 kernel drives the R10000 translation buffer on IP27 (Origin 200 and 2000): which entries it wires, how it refills and probes, and how it flushes. The configuration is the 64-bit kernel with a 16 KB base page.

**Release and grade.** IRIX 6.5.7 is not one of this repository's releases (6.5.30 and 6.5.22). Nothing here is evidence about those kernels, and a fact from one release does not fill a gap in another. Every fact is graded **inferred**. The basis is a reading of the 6.5.7 kernel's translation and fault code in source form, not a shipped binary of a covered release. Where a fact states an R10000 architecture point (entry count, ASID width, the global-bit AND), it was not checked against the published R10000 manual in this import. No fact here is graded established. Each one needs a 6.5.30 or 6.5.22 binary, read in full, before it can be.

Constants are in [defs/r10k_tlb.toml](../defs/r10k_tlb.toml) and the generated page is in `gen/docs/r10k_tlb.md`.

## Configuration

- **Inferred.** The TLB has 64 dual entries and an 8-bit ASID field. The kernel uses 255 ASIDs and reserves ASID 0 for the kernel and system processes.
- **Inferred.** The 64-bit kernel's base page is 16 KB. The 4 KB page path belongs to the 32-bit kernel and is compiled out here.
- **Inferred.** The PageMask register is set to 16 KB at boot and stays there as the standing value.

## Wired entries

Seven entries are wired on the mapped kernel that IRIX ships for IP27, and six on an unmapped build.

- **Index 0.** The per-CPU data area together with the running thread's user page and kernel stack. It is global, 16 KB, and rewritten at every context switch to point at the incoming thread's kernel stack. **Inferred.**
- **Index 1, mapped kernel only.** Kernel text and data as one global 16 MB entry, installed once at boot. **Inferred.**
- **Indices 2 to 6.** Per-process second-level user mappings, 16 KB each and tagged with the ASID. The fast fault handler uses them as rotating drop-in slots. The kernel keeps a shadow of them in the thread's user area and replays it on the return path. **Inferred.**

These are not segment-table entries. The 64-bit kernel reaches its page tables through the unmapped kernel segments and wires no segment table. **Inferred.**

## Refill and probe

- **Ordinary refill.** The handler reads the even and odd page-table entries for the faulting page from the page-table self-map window. The hardware places that window's address in the extended-context register. It writes EntryLo0 and EntryLo1 and installs the entry with tlbwr (write random). It does not write EntryHi, PageMask, Index or Random, so the ASID is not rewritten on a refill. **Inferred.**
- **Second-level miss.** A miss on a page-table page or an I/O-map page probes the TLB before the second-level entry is dropped in. **Inferred.**
- **Real fault completion.** A demand page, copy-on-write, first write or protection change installs through the drop-in routine. That routine probes with tlbp under the current ASID. On a hit it reads the entry with tlbr and rewrites it with tlbwi. On a miss it uses tlbwr. Afterwards it forces PageMask back to 16 KB, because tlbr can load a large-page mask. **Inferred.**
- An ordinary refill of a resident page therefore does no probe, and every real fault completion does. **Inferred.**

## Page sizes

- **Inferred.** Every PageMask write in the 64-bit kernel is 16 KB or larger. The only 4 KB writer belongs to the 32-bit path.
- **Inferred.** Entries larger than 16 KB are installed by raising PageMask, writing the entry, and restoring 16 KB. This covers large pages of 64 KB and up, the 16 MB I/O DMA-map entries and the 16 MB kernel text and data entry.
- **Inferred.** Large pages are on by default on this board. A tunable sets a coalescing target of about one fifth of memory in 64 KB pages. Sizes from 256 KB to 16 MB are off by default. Large-page formation starts at 64 KB.
- **Inferred.** I/O DMA-map misses on IP27 install 16 MB entries.
- **Inferred, and not a fixed value.** A tlbr loads the probed entry's mask, which can be a large-page value. One diagnostic read-out leaves an unrelated value in PageMask because of a defect in the kernel source. So PageMask is not always 16 KB after a read, and code must not assume it is.

## Foreign-ASID probes and shootdowns

Every maintenance primitive that probes or reads the TLB follows one shape: save EntryHi, write a page number and an ASID into EntryHi, probe or read, then restore EntryHi. The ASID is often the target process's, not the running one. **Inferred.**

- **Single-page unmap or shootdown.** Probe for the target page under the target's ASID. On a hit, write one invalid entry with tlbwi. Then restore EntryHi. **Inferred.**
- **Protection change or query.** Probe, read with tlbr, read EntryLo0, EntryLo1 and PageMask, then restore EntryHi. **Inferred.**
- **Fault completion.** The probe uses the running thread's ASID and changes only the faulting page. It is the most frequent of these probes. **Inferred.**
- **Shootdown cost.** For each process that shares the address space, a shootdown issues one probe and at most one indexed invalidate for the page. A broadcast full flush is used only when more than 16 processes share the address space. **Inferred.**

## Context switch

- **Inferred.** Index 0 is rewritten at every switch with one tlbwi. The wired drop-in entries at indices 2 to 6 are replayed from the shadow with one tlbwi each, and no tlbr is issued. Index 1 is constant and untouched. No full flush happens.
- **Inferred.** The read-modify-write loop that re-stamps the ASID into the wired entries (tlbr, rewrite, tlbwi) runs only when a fresh ASID is allocated, not on each switch.

## ASID lifecycle and flushes

- **Inferred.** ASIDs are handed out per CPU by a least-recently-used allocator over the 255 usable values.
- **Inferred.** An ASID wrap does not flush the whole TLB. The allocator invalidates one non-wired entry every 255 ÷ (number of non-wired entries) uses and cycles through the non-wired entries. With 57 non-wired entries on the mapped kernel that is about every 4.5 uses. By the time an ASID is reused, every entry that could hold it has been cleared.
- **Inferred.** To flush an address space running on this CPU, the kernel writes invalid entries from the first non-wired index to the end, leaving the wired region alone. A space that is not running is flushed lazily: its ASID is marked stale on every CPU, so it gets a fresh ASID next time it runs, and no entry is touched.
- **Inferred.** A true broadcast full flush across CPUs happens only for an address space shared by more than 16 processes.

## Clock-paced recycle of the drop-in slots

- **Inferred.** On a clock tick, if the drop-in cursor has passed the last slot and a per-address-space tick counter has reached the pacing tunable (default 100), the kernel resets both. It then allocates a fresh ASID with the invalidate flag set. That discards the running thread's non-wired translations, so it is a full flush for that thread.
- **Inferred.** The counter advances only on clock ticks taken in user mode while the cursor is already exhausted. The fire rate is therefore at most one per 100 such ticks. At the default and a 100 Hz clock, that is at most one per second, and only for a thread that stays CPU-bound in user mode with its slots used up. It is an upper bound, not a rate every thread reaches.

## Invalidation idioms

- **Inferred.** To kill one translation, probe for the page and ASID. On a hit, write an invalid entry with tlbwi. An invalid entry carries a reserved kseg0 page number and a zero EntryLo.
- **Inferred.** To kill an entry by index, write an invalid entry at that index with tlbwi, with no probe. The ASID recycle scrub uses this form.
- **Inferred.** To flush a range, write invalid entries with tlbwi from a start index to the end. Boot and the non-wired region use this form.
- **Inferred.** Many invalid entries carry the same reserved page number. The kernel keeps only valid translations unique.

## Global bit

- **Inferred.** Mappings marked global: the kernel dynamic heap, the per-CPU data entry, the kernel stack page, mapped kernel text and data, the fork user area and the crash-dump maps.
- **Inferred (kernel use). Not checked against the manual in this import.** The effective global status of an entry is the hardware AND of the two halves' global bits. The kernel relies on that AND. When only one half of a pair is present, the fault drop-in copies that half's global bit into the empty half, so the AND gives the intended result.

## Kernel reads through ASID-tagged mappings

- **Inferred.** The kernel reads ASID-tagged mapped memory. The per-process page tables are reached through the self-map window, and the drop-in slots at indices 2 to 6 are ASID-tagged. A flush on an ASID change therefore cannot be narrowed to user-mode translations alone without losing mappings the kernel reads.

## R10000 interrupt-path probe

- **Inferred.** An interrupt taken in user mode, while the thread owns the floating-point unit and the page holding the faulting instruction is resident, triggers a workaround for an R10000 integer multiply and divide erratum. The handler re-reads the faulting instruction. It probes for that page under the current ASID with tlbp, reads the entry with tlbr, and restores PageMask to 16 KB. If the two instructions it re-reads straddle a page boundary, it does this for the adjacent page too.
- **Inferred.** The workaround is gated on processor revision. It is armed only on the original R10000, and the kernel turns it off at run time on the R12000 and later.

## What this page does not settle

The rates above are upper bounds, not measurements. This page does not say how often whole-TLB flushes happen on a running system. The flush cost depends on paths outside the TLB routines described here, and nothing in this import quantifies them.
