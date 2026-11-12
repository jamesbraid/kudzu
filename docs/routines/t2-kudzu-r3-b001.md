# Tier-2 routine readings, kudzu batch 1

Role statements for ranks 25 to 49 of the tier-2 list, written from the rendered
instructions only: loads, stores, calls, branches and constants. Every grade is
inferred. Several routines are small wrappers whose callees are named, and the
statements say what the wrapper passes on rather than what the callee does. The
binary is the IP27 unix kernel image in the 6.5.22 build.

- `intr` in `6.5.22/unix.IP27` at 0xc000000000038ff4
  **Inferred.** It makes eight pairs of calls, each a splx (0xc00000000003b0e4) whose a0 is loaded from a fixed word between 0xc0000000014161a0 and 0xc000000001416210 and whose a1 is the doubleword at abs -0x3af8, followed by an indirect call that is passed intr's a0 at entry and whose target the facts do not resolve. Most of these pairs sit behind branches the facts do not interpret. Its direct callees are check_delay_tlbflush (0xc0000000001d91f0, called with a0 = 1 at 0xc000000000039098 and with a0 = 0 at 0xc000000000039160), nointr (0xc000000000038d98), intr_stray (0xc000000000037c4c) and log_tstamp_event (0xc0000000002383b0), which it calls with the constants 0x66 and 0x65 at 0xc000000000039240 and 0xc000000000039260. The facts do not resolve the indirect targets or what the tests against the constants 0x20, 0x30, 0x800 and 0x4000 select.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.22/unix.IP27` at 0xc000000000039810
  **Inferred.** It reads a halfword from abs 0xffffffffffffc420, then a byte at +0x520 of a table entry that the facts do not resolve, and ANDs the hub doubleword at 0x92000000010000a0 with a mask derived from that byte. It calls dbgintr (0xc000000000039518) with a0 = 0 only when the result is nonzero, and on that path returns dbgintr's result; the other path returns an unresolved value. The facts do not show which bit of the hub word the mask selects.
  Reviewed: 2026-11-12 (sonnet)

- `hub_migrintr_init` in `6.5.22/unix.IP27` at 0xc0000000000398c8
  **Inferred.** It calls cnodetocpu (0xc000000000026d14) with its arguments first. It then calls intr_connect_level (0xc000000000038108) with a1 = 3, a2 = 0, a3 = 0xc00000000026b4a8 and a5 = 0xc00000000026b41c, unless the word at 0xc0000000014d269c equals 1 or a register that the facts do not tie to cnodetocpu's result equals -1. When intr_connect_level returns nonzero it calls cmn_err (0xc0000000001f8090). The facts do not show what the connected handler does.
  Reviewed: 2026-11-12 (sonnet)

- `intr_reserve_hardwired` in `6.5.22/unix.IP27` at 0xc000000000039a20
  **Inferred.** It calls cnodetocpu (0xc000000000026d14) with its arguments first, reads the byte at 0xc000000001416218 and the halfword at +0x422 (decimal 1058) of the table entry at 0xc0000000014d39a8, and calls intr_reserve_level (0xc00000000003805c) with a3 = 0. It also calls cpu_enabled (0xc000000000026f74). It calls cmn_err (0xc0000000001f8090) at 0xc000000000039b28 when the reservation result differs from a value held in s2. The facts do not settle which hardwired interrupt number the reserved level corresponds to.
  Reviewed: 2026-11-12 (sonnet)

- `intr_clear_bits` in `6.5.22/unix.IP27` at 0xc000000000039b44
  **Inferred.** It loads a doubleword through a1 and, if it is nonzero, loops with a back branch at 0xc000000000039bac: each pass tests the doubleword against a mask value, and on a match stores a doubleword (sd) through a pointer the facts do not resolve and loads a doubleword back from another. The facts do not show the addresses, the value written or which bits are cleared, so the name is the only evidence of intent.
  Reviewed: 2026-11-12 (sonnet)

- `intr_clear_all` in `6.5.22/unix.IP27` at 0xc000000000039bbc
  **Inferred.** It makes four doubleword stores, then calls intr_clear_bits (0xc000000000039b44) twice. The calls pass a1 = (a0 << 0x20) + 0x9200000001800098 with a2 = 0x0, and a1 = (a0 << 0x20) + 0x92000000018000a0 with a2 = 0x40, and return the second call's result. The facts do not resolve the store addresses. Its callees and constants are the only evidence of its role.
  Reviewed: 2026-11-12 (sonnet)

- `enable_ithreads` in `6.5.22/unix.IP27` at 0xc00000000003a054
  **Inferred.** It calls intr_thread_setup (0xc00000000003a1e4) with a3 loaded from 0xc0000000014c97e0, and uses mutex_spinlock_spl (0xc0000000002871c0) and mutex_spinunlock (0xc000000000287260) around its work. It also writes a 4-byte word. The facts do not show what that word records or whether the setup runs once.
  Reviewed: 2026-11-12 (sonnet)

- `intr_thread_setup` in `6.5.22/unix.IP27` at 0xc00000000003a1e4
  **Inferred.** It formats a name into a stack buffer at sp-64 with sprintf (0xc0000000001f8e8c) from the format at 0xc00000000140bc18, then calls xthread_setup (0xc0000000001c75c8) with that buffer, its arg3, and the fixed entry address 0xc00000000003a188. The name and the entry are the only facts; the body of 0xc00000000003a188 is outside this routine.
  Reviewed: 2026-11-12 (sonnet)

- `assfail` in `6.5.22/unix.IP27` at 0xc00000000003a2b4
  **Inferred.** It stores a register at +8 from a base of 0xc00000000153e3b0 and loads the word at panic_cpu (0xc0000000014ca6d4) before branching. When panic_cpu equals -1 it falls through to store the halfword at 0xffffffffffffc420 into panic_cpu and make 34 more doubleword stores (35 in all) through that base; otherwise it skips them. Both paths end by tail-jumping to _assfail (0xc0000000001faa00) at 0xc00000000003a6e8 with a0 to a3 unchanged. The facts show the save and the tail call, not the message handling.
  Reviewed: 2026-11-12 (sonnet)

- `hubio_init` in `6.5.22/unix.IP27` at 0xc00000000003b128
  **Inferred.** It is an 8-byte leaf with no loads, stores or calls in its body. Its return value in v0 is not resolved by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_pio_init` in `6.5.22/unix.IP27` at 0xc00000000003b130
  **Inferred.** It calls hwgraph_info_get_LBL (0xc000000000296160) with a1 = 0xc00000000140bd80, writes a run of 4-byte, 8-byte, 2-byte and 1-byte fields through pointers the facts do not resolve, then calls hub_set_piomode (0xc00000000003c0bc) with a1 = 2 and a3 = the table at 0x9200000001c00000. It then calls init_sv (0xc00000000018ea70) with a1 = 0 and a2 = 0xc00000000140bd88, and spinlock_init (0xc000000000286fe8) with a1 = 0xc00000000140bd88, and returns spinlock_init's result. The facts do not show the meaning of the fields or what 0xc00000000140bd88 is.
  Reviewed: 2026-11-12 (sonnet)

- `hub_piomap_alloc` in `6.5.22/unix.IP27` at 0xc00000000003b280
  **Inferred.** It calls xwidget_info_get (0xc00000000006dcb0), then xwidget_info_id_get (0xc00000000006dd94) and xwidget_info_master_get (0xc00000000006ddc8) on its result, calls hwgraph_info_get_LBL (0xc000000000296160) with the master as a0, and calls mutex_spinlock (0xc000000000287214) and mutex_spinunlock (0xc000000000287260). It also stores 8-byte, 4-byte, 2-byte and 1-byte fields through pointers the facts do not resolve. One path calls sv_wait (0xc00000000018eda0) with a1 = 0x19 and a3 = mutex_spinlock's result. The facts do not show the condition under which it waits or what the stored fields are.
  Reviewed: 2026-11-12 (sonnet)

- `hub_piomap_free` in `6.5.22/unix.IP27` at 0xc00000000003b5ac
  **Inferred.** It tests bit 0x2 of the word at a0+0x28 and returns at once when it is clear. Otherwise it calls hwgraph_info_get_LBL (0xc000000000296160) with a0 = the word at a0+0x20 and a1 = 0xc00000000140bd80, calls mutex_spinlock (0xc000000000287214), and decrements the halfword at a0+0x24. If the new value is nonzero it calls mutex_spinunlock (0xc000000000287260) and returns. If it is zero it stores a doubleword at offset 0x160 of the table, then tests bit 0x4 of the word at a0+0x28. When that bit is set it stores the word back with bits 0x1 and 0x4 cleared, decrements a word at +0x320 of a table entry and calls sv_signal (0xc00000000018f4fc) with a2 = the table at 0x9200000001c00000; when it is clear it stores the word back with bit 0x1 cleared and makes no sv_signal call. The facts show no mutex_spinunlock call on the zero path. The name and the signal are the evidence for its role; the field meanings are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `hub_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000003b6fc
  **Inferred.** It calls xwidget_info_get (0xc00000000006dcb0), xwidget_info_id_get (0xc00000000006dd94) and xwidget_info_master_get (0xc00000000006ddc8), then hwgraph_info_get_LBL (0xc000000000296160). After an unsigned comparison of 0x1000000 against the sum of two values, it calls hub_piomap_addr (0xc00000000003b694) with its arg2 and arg3 as the second and third arguments only on the side where 0x1000000 is not less than the sum. It returns 0x0 or an unresolved value. The facts show the forwarding but not how the address is computed.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dma_init` in `6.5.22/unix.IP27` at 0xc00000000003b7c0
  **Inferred.** It is an 8-byte leaf with only a return. The facts show no loads, stores or calls, and the return value is not resolved.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dmamap_alloc` in `6.5.22/unix.IP27` at 0xc00000000003b7c8
  **Inferred.** It calls xwidget_info_get (0xc00000000006dcb0), xwidget_info_id_get (0xc00000000006dd94) and xwidget_info_master_get (0xc00000000006ddc8), then kern_malloc (0xc000000000181da4) with a0 = 0x10. It stores words at +0, +0xc and +8 of the returned block, and returns the block pointer. When bit 0 of a3 at entry is set it also stores a byte at +4 and a word at +0xc again. The field meanings are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dmamap_free` in `6.5.22/unix.IP27` at 0xc00000000003b848
  **Inferred.** It reads the word at a0+0xc, calls kern_free (0xc000000000181f2c) with a0 at entry, and stores a word to a0+0xc in the delay slot of that call, so the value written back is not resolved by the facts. Role (from name): it frees a map record such as the one hub_dmamap_alloc makes; the facts show only the kern_free call.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dmamap_addr` in `6.5.22/unix.IP27` at 0xc00000000003b870
  **Inferred.** It reads the word at a0+0xc and tests bits 0x2 and 0x4. If 0x2 is clear it stores that word ORed with 0x2 back to a0+0xc and returns a1 without a call. If 0x2 is set and 0x4 is set it returns a1 without a store or call. If 0x2 is set and 0x4 is clear it reads the word at a0+0 and calls cmn_err (0xc0000000001f8090) with a0 = 4, a2 = that word, a5 = a0 at entry and a6 = the word at a0+12 ORed with 0x2. The facts do not show what the address result is.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dmamap_list` in `6.5.22/unix.IP27` at 0xc00000000003b8cc
  **Inferred.** It has the same branch layout as hub_dmamap_addr at 0xc00000000003b870. It reads the word at a0+0xc and tests bits 0x2 and 0x4. If 0x2 is clear it stores a word back to a0+0xc and returns a1 without a call. If 0x2 is set and 0x4 is clear it reads the word at a0+0 and calls cmn_err (0xc0000000001f8090) with a0 = 4 and a different message address (0xc00000000140bdb8, against 0xc00000000140bd90 in hub_dmamap_addr). The facts do not show the list operation itself.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dmamap_done` in `6.5.22/unix.IP27` at 0xc00000000003b928
  **Inferred.** It reads the word at a0+0xc and tests bits 0x2 and 0x4. If 0x2 is set it stores the word with that bit cleared back to a0+0xc and returns, with no call. If 0x2 is clear and 0x4 is clear it reads the word at a0+0 and calls cmn_err (0xc0000000001f8090) with a0 = 4, a2 = that word, a4 = a0 at entry and a5 = -3. If 0x2 is clear and 0x4 is set it returns without a call. The facts do not show what the done state means.
  Reviewed: 2026-11-12 (sonnet)

- `hub_intr_init` in `6.5.22/unix.IP27` at 0xc00000000003b9a0
  **Inferred.** It is an 8-byte leaf with only a return. The facts show no loads, stores or calls, and the return value is not resolved.
  Reviewed: 2026-11-12 (sonnet)

- `hub_device_desc_update` in `6.5.22/unix.IP27` at 0xc00000000003b9a8
  **Inferred.** It calls device_desc_intr_swlevel_set (0xc000000000175f7c) with its arguments unchanged, formats a path with sprintf (0xc0000000001f8e8c) from the format at 0xc00000000140be10 and arg2, resolves that path with hwgraph_path_to_dev (0xc0000000002967d8), and calls device_desc_intr_target_set (0xc000000000175f6c) with arg0 and the device it found. The facts show the call order and the callees, not the descriptor layout.
  Reviewed: 2026-11-12 (sonnet)

- `hub_intr_alloc` in `6.5.22/unix.IP27` at 0xc00000000003bcac
  **Inferred.** It is a wrapper that calls do_hub_intr_alloc (0xc00000000003b9fc) with arguments a0 through a2 unchanged and a3 = 0, then returns that call's result. Its sibling hub_intr_alloc_nothd at 0xc00000000003bcc8 passes a3 = 1, so a3 looks like a mode flag for the callee.
  Reviewed: 2026-11-12 (sonnet)

- `hub_intr_alloc_nothd` in `6.5.22/unix.IP27` at 0xc00000000003bcc8
  **Inferred.** It is the same wrapper as hub_intr_alloc at 0xc00000000003bcac: it calls do_hub_intr_alloc (0xc00000000003b9fc) with arguments a0 through a2 unchanged and a3 = 1, then returns the result. The name suggests the flag selects a no-thread mode, but the facts do not show what do_hub_intr_alloc does with it.
  Reviewed: 2026-11-12 (sonnet)

- `hub_intr_free` in `6.5.22/unix.IP27` at 0xc00000000003bce4
  **Inferred.** It reads the word at a0+0x28, the halfword at a0+0x24 and the word at a0+0x2c. If bit 0x4 of the word at a0+0x2c is set it stores a word at +0, a byte at a0+5 and a doubleword at a0+8, then calls hub_intr_disconnect (0xc00000000003bde4) with a3 = the word at a0+44. If bit 0x1 of that word is set it calls kmem_free (0xc000000000181d4c) with a0 at entry and a1 = 0x30. It always ends with intr_unreserve_level (0xc000000000038084), passing the halfword at a0+0x24 and the word at a0+40, and returns its result. Role (from name): a release path for a record such as the one hub_intr_alloc creates; the facts do not show that record.
  Reviewed: 2026-11-12 (sonnet)
