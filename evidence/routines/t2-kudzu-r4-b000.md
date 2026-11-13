# Tier-2 routine readings, kudzu batch 0

Routines from the first 25 targets of the tier-2 list for kudzu, read from the rendered `6.5.22/unix.IP27` facts. Each role names the addresses, callees and constants it comes from. Entries that only forward their arguments to another routine are marked established, since the rendered facts settle that behavior.

- `net_link_up` in `6.5.22/unix.IP27` at 0xc00000000002e2f0
  **Inferred.** Named `net_link_up`, this 20-byte leaf makes no calls. It loads one doubleword from the uncached address 0x9200000001600000 at 0xc00000000002e2f4 and returns at 0xc00000000002e2fc, with v0 not tied to that load in the rendered facts. The name points to a network link-status read, which a single device-register load would fit.
  Reviewed: 2026-11-12 (sonnet)

- `hub_piomap_free` in `6.5.22/unix.IP27` at 0xc00000000003b5ac
  **Inferred.** Named `hub_piomap_free`, this 232-byte routine reads fields of the record in a0 at +0x20, +0x24, +0x26 and +0x28 and tests bits 0x2 and 0x4 of the word at a0+0x28. Role (from name): it releases a PIO map record. When bit 0x2 is clear (branch at 0xc00000000003b5c0) it jumps to 0xc00000000003b614, just before the return at 0xc00000000003b61c. When bit 0x2 is set it calls hwgraph_info_get_LBL at 0xc00000000003b5d0 and mutex_spinlock at 0xc00000000003b5e4, then tests a value in v1 that the facts do not tie to a load (branch at 0xc00000000003b5fc). When v1 is nonzero it stores a halfword to a0+0x24 at 0xc00000000003b600, calls mutex_spinunlock at 0xc00000000003b60c and returns at 0xc00000000003b61c. When v1 is zero it branches to 0xc00000000003b624, where a doubleword store (sd +0x0) at 0xc00000000003b640 is followed by a test of bit 0x4 of a0+0x28 (branch at 0xc00000000003b654). sv_signal at 0xc00000000003b674 is called only when bit 0x4 is set, with a2 the table at 0x9200000001c00000. The facts do not show which address the store at 0xc00000000003b640 writes.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dmamap_alloc` in `6.5.22/unix.IP27` at 0xc00000000003b7c8
  **Inferred.** Named `hub_dmamap_alloc`, this 128-byte routine calls xwidget_info_get at 0xc00000000003b7d8, xwidget_info_id_get at 0xc00000000003b7e4 and xwidget_info_master_get at 0xc00000000003b7f0, then kern_malloc at 0xc00000000003b800 with a0 = 0x10, and fills the allocation. It stores words at offsets 0x0 (0xc00000000003b80c), 0xc (0xc00000000003b814) and 0x8 (0xc00000000003b820) on every path. The branch at 0xc00000000003b830 tests bit 0 of a3. When the bit is set, it also stores a byte at offset 0x4 (0xc00000000003b834) and a word at offset 0xc (0xc00000000003b838); when clear it skips both. It returns the kern_malloc result at 0xc00000000003b840.
  Reviewed: 2026-11-12 (sonnet)

- `hub_intr_cpu_get` in `6.5.22/unix.IP27` at 0xc00000000003be48
  **Inferred.** Named `hub_intr_cpu_get`, this 28-byte routine loads a halfword at a0+0x24 at 0xc00000000003be48. It then loads a doubleword (ld +0x0) at 0xc00000000003be58 and returns the word at offset 0x734 (1844) of it, loaded at 0xc00000000003be60 and returned at 0xc00000000003be5c. The rendered return expression is `[entry(0xc0000000014d39a8)+1844]` and does not mention the halfword. The facts therefore show no use of the halfword in the return value, but do not show how the pointer is addressed.
  Reviewed: 2026-11-12 (sonnet)

- `hub_setup_prb` in `6.5.22/unix.IP27` at 0xc00000000003c028
  **Inferred.** Named `hub_setup_prb`, this 148-byte routine tests the word at 0xc0000000014c9c9c (branch at 0xc00000000003c030) and then a5 (branch at 0xc00000000003c054). It loads a doubleword (ld +0x0) at 0xc00000000003c078 and stores a doubleword (sd +0x0) at 0xc00000000003c0b8. The facts do not show which of the branches guard these two accesses, the addresses they touch, or that the store writes back a modified copy of the load, so a read-modify-write of one table entry is not established. The name suggests a setup step.
  Reviewed: 2026-11-12 (sonnet)

- `get_my_slotid` in `6.5.22/unix.IP27` at 0xc00000000003ca64
  **Inferred.** Named `get_my_slotid`, this 36-byte routine loads a doubleword (ld +0x0) at 0xc00000000003ca68, then calls hub_slotbits_to_slot at 0xc00000000003c7ec from 0xc00000000003ca74 and returns that result at 0xc00000000003ca80. a1 through a3 are passed through as they entered; the facts do not tie a0 to the loaded doubleword (a0 = ?). The loaded register is probably the input to the conversion, going by the callee's name, but the facts do not show it.
  Reviewed: 2026-11-12 (sonnet)

- `intrgroup_init` in `6.5.22/unix.IP27` at 0xc000000000049fc0
  **Inferred.** Named `intrgroup_init`, this 112-byte routine stores doublewords (sd +0x0) at 0xc000000000049fd8 and 0xc000000000049fe4. It calls spinlock_init once at 0xc000000000049fe0 with a0 = 0xc0000000014dd2b0, before the loop. The loop body starts at 0xc000000000049fec, after that call, and calls spinlock_init at 0xc00000000004a008 on each pass. The branch at 0xc00000000004a018 loops back while the counter s0 is below 0x10 (signed). So it initializes a set of locks, going by the name; the iteration count depends on the start value of s0, which the facts do not show.
  Reviewed: 2026-11-12 (sonnet)

- `_check_dbg` in `6.5.22/unix.IP27` at 0xc00000000005b4b8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4b8. It makes no calls and no memory accesses, so it is a no-op that always reports zero, and the name suggests a debug-check hook.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_version` in `6.5.22/unix.IP27` at 0xc00000000005b4c0
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4c0 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator version query.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_info_chk` in `6.5.22/unix.IP27` at 0xc00000000005b4c8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4c8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator info check.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000005b4d0
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4d0 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator PIO translation hook.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_dmatrans_list` in `6.5.22/unix.IP27` at 0xc00000000005b4d8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4d8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator DMA translation hook.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_intr_alloc` in `6.5.22/unix.IP27` at 0xc00000000005b4e0
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4e0 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator interrupt allocation hook.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000005b4e8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4e8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator interrupt connect hook.
  Reviewed: 2026-11-12 (sonnet)

- `tpuidbg_version` in `6.5.22/unix.IP27` at 0xc00000000005b4f8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4f8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a debug-interface version query.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_piomap_alloc` in `6.5.22/unix.IP27` at 0xc00000000006d740
  **Established.** Its only action is a call to hub_piomap_alloc at 0xc00000000003b280, reached by the jal at 0xc00000000006d748 with a0 through a3 as they arrived. The v0 returned at 0xc00000000006d754 is that callee's result, so the routine is a pass-through to the hub implementation.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_piomap_free` in `6.5.22/unix.IP27` at 0xc00000000006d75c
  **Established.** The body is one jal, to hub_piomap_free at 0xc00000000003b5ac, at 0xc00000000006d764. Arguments a0 to a3 reach the callee untouched, and the value returned at 0xc00000000006d770 is the callee's v0.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_piomap_addr` in `6.5.22/unix.IP27` at 0xc00000000006d778
  **Established.** A single call at 0xc00000000006d780 reaches hub_piomap_addr at 0xc00000000003b694. The return at 0xc00000000006d78c hands back whatever that callee left in v0, with no extra handling of its arguments.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_piomap_done` in `6.5.22/unix.IP27` at 0xc00000000006d794
  **Established.** The one call, at 0xc00000000006d79c, goes to hub_piomap_done at 0xc00000000003b6f4. Nothing else in the 28 bytes touches memory, and v0 at 0xc00000000006d7a8 carries the callee result out.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000006d7b0
  **Established.** The jal at 0xc00000000006d7b8 targets hub_piotrans_addr at 0xc00000000003b6fc, receiving a0 through a3 as passed in. The value returned at 0xc00000000006d7c4 comes from that routine's v0, so this is a direct alias for the hub translation.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_pio_addr` in `6.5.22/unix.IP27` at 0xc00000000006d7cc
  **Inferred.** This 248-byte routine chains the translation and mapping calls. It tests a4 at 0xc00000000006d800 and stores a doubleword (sd +0x0) at 0xc00000000006d808 only when a4 is nonzero. It calls xtalk_piotrans_addr at 0xc00000000006d820; when the result is nonzero the branch at 0xc00000000006d828 skips the fallback, and when it is zero it calls xtalk_piomap_alloc at 0xc00000000006d86c. The call to xtalk_piomap_addr at 0xc00000000006d88c runs only when the alloc result is nonzero (branch at 0xc00000000006d874), with a0 = the alloc result. When that call returns zero (branch at 0xc00000000006d894) it calls xtalk_piomap_free at 0xc00000000006d8b4 with the alloc result. When it returns nonzero, a second doubleword store at 0xc00000000006d8a4 runs only if s2 (a copy of a4) is nonzero. The facts do not show the base register of either store. The order is translate first, and map on a zero translate result.
  Reviewed: 2026-11-12 (sonnet)

- `null_xtalk_early_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000006d8ec
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000006d8ec with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a placeholder for the piotrans path used before the translation layer is up.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmamap_alloc` in `6.5.22/unix.IP27` at 0xc00000000006d8f4
  **Established.** At 0xc00000000006d8fc it calls hub_dmamap_alloc at 0xc00000000003b7c8 with the entry arguments, then returns its v0 at 0xc00000000006d908. There is no other work in the routine.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmamap_free` in `6.5.22/unix.IP27` at 0xc00000000006d910
  **Established.** The call at 0xc00000000006d918 goes to hub_dmamap_free at 0xc00000000003b848 with the entry registers, and v0 from it is returned at 0xc00000000006d924. The routine has no other calls.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmamap_addr` in `6.5.22/unix.IP27` at 0xc00000000006d92c
  **Established.** A call at 0xc00000000006d934 reaches hub_dmamap_addr at 0xc00000000003b870, passing a0 through a3 as they entered. The v0 returned at 0xc00000000006d940 is the callee's, with nothing else done in the routine.
  Reviewed: 2026-11-12 (sonnet)
