# Tier-2 routine readings, kudzu batch 1

Role statements for ranks 25 to 49 of the tier-2 list, written from the rendered
instructions only: loads, stores, calls, branches and constants. Every grade is
inferred. Several routines are small wrappers whose callees are named, and the
statements say what the wrapper passes on rather than what the callee does. The
binary is the IP27 unix kernel image in the 6.5.22 build.

- `intr` in `6.5.22/unix.IP27` at 0xc000000000038ff4
  **Inferred.** It walks cause_intr_tbl in 16-byte slots, reading an 8-byte word at slot+0 and 4-byte words at slot+8 and slot+12, and calls splx (0xc00000000003b0e4) with a slot word as a0 before each indirect call through the slot's handler word. Its direct callees are nointr (0xc000000000038d98), check_delay_tlbflush (0xc0000000001d91f0), intr_stray (0xc000000000037c4c) and log_tstamp_event (0xc0000000002383b0), which it calls with the constants 0x66 and 0x65 at 0xc000000000039240 and 0xc000000000039260. The facts do not resolve the indirect targets or what the tests against the constants 0x20, 0x30, 0x800 and 0x4000 select.

- `chkdebug` in `6.5.22/unix.IP27` at 0xc000000000039810
  **Inferred.** It reads a halfword from abs 0xffffffffffffc420, then a byte at +0x520 of a table entry reached through 0xc0000000014d39a8, and tests that byte against the hub doubleword at 0x92000000010000a0. It calls dbgintr (0xc000000000039518) only when the masked test is nonzero, and returns dbgintr's result. The facts do not show which bit of the hub word the mask selects.

- `hub_migrintr_init` in `6.5.22/unix.IP27` at 0xc0000000000398c8
  **Inferred.** It calls cnodetocpu (0xc000000000026d14) with its arguments first, then calls intr_connect_level (0xc000000000038108) with the constant 3 and two code addresses, 0xc00000000026b4a8 and 0xc00000000026b41c. It skips the connect when the word at 0xc0000000014d269c equals 1 or a0 is -1, and on a connect failure it reports through cmn_err (0xc0000000001f8090). The facts do not show what the connected handler does.

- `intr_reserve_hardwired` in `6.5.22/unix.IP27` at 0xc000000000039a20
  **Inferred.** It calls cnodetocpu (0xc000000000026d14) with its arguments first, reads the byte at 0xc000000001416218 (hardwired_intr) and the halfword at +0x422 (decimal 1058) of the table entry at 0xc0000000014d39a8, and reserves a level with intr_reserve_level (0xc00000000003805c) and a3 = 0. It checks cpu_enabled (0xc000000000026f74) and reports a failed reservation through cmn_err (0xc0000000001f8090) at 0xc000000000039b28. The facts do not settle which hardwired interrupt number the reserved level corresponds to.

- `intr_clear_bits` in `6.5.22/unix.IP27` at 0xc000000000039b44
  **Inferred.** It loads a doubleword through arg1 and stores to the hub register at 0x9200000001000090, then reads the hub register at 0x9200000001000098 in a second pair of accesses. The facts do not show the value written or which bits are cleared, so the name is the only evidence of intent.

- `intr_clear_all` in `6.5.22/unix.IP27` at 0xc000000000039bbc
  **Inferred.** It stores four doublewords at addresses formed by shifting a0 into bit 32 and adding constants, then calls intr_clear_bits (0xc000000000039b44) twice. The two calls pass register addresses 0x9200000001800098 and 0x92000000018000a0 for node a0, with a2 set to 0x0 and then 0x40. Its callees and constants are the only evidence of its role.

- `enable_ithreads` in `6.5.22/unix.IP27` at 0xc00000000003a054
  **Inferred.** It calls intr_thread_setup (0xc00000000003a1e4) with a3 loaded from 0xc0000000014c97e0, and uses mutex_spinlock_spl (0xc0000000002871c0) and mutex_spinunlock (0xc000000000287260) around its work. It also writes a 4-byte word. The facts do not show what that word records or whether the setup runs once.

- `intr_thread_setup` in `6.5.22/unix.IP27` at 0xc00000000003a1e4
  **Inferred.** It formats a name into a stack buffer at sp-64 with sprintf (0xc0000000001f8e8c) from the format at 0xc00000000140bc18, then calls xthread_setup (0xc0000000001c75c8) with that buffer, its arg3, and the fixed entry address 0xc00000000003a188. The name and the entry are the only facts; the body of 0xc00000000003a188 is outside this routine.

- `assfail` in `6.5.22/unix.IP27` at 0xc00000000003a2b4
  **Inferred.** It saves the register arguments and a run of registers into the _assregs block at 0xc00000000153e3b8 with about 40 doubleword stores from +8 to +352. It records the CPU identifier from the halfword at 0xffffffffffffc420 into panic_cpu (0xc0000000014ca6d4), then tail-jumps to _assfail (0xc0000000001faa00) with its arguments unchanged at 0xc00000000003a6e8. The facts show the save and the tail call, not the message handling.

- `hubio_init` in `6.5.22/unix.IP27` at 0xc00000000003b128
  **Inferred.** It is an 8-byte leaf with no loads, stores or calls in its body. Its return value in v0 is not resolved by the facts, so it does nothing observable here.

- `hub_pio_init` in `6.5.22/unix.IP27` at 0xc00000000003b130
  **Inferred.** It looks up a hwgraph label with hwgraph_info_get_LBL (0xc000000000296160) at 0xc00000000140bd80, calls hub_set_piomode (0xc00000000003c0bc) with the constant 2 and the table at 0x9200000001c00000, and calls init_sv (0xc00000000018ea70) and spinlock_init (0xc000000000286fe8), which set up a sync variable and a spinlock at 0xc00000000140bd88. It also writes a run of 4-byte, 8-byte, 2-byte and 1-byte fields into the table at 0x9200000001c00160. The facts do not show the meaning of those fields.

- `hub_piomap_alloc` in `6.5.22/unix.IP27` at 0xc00000000003b280
  **Inferred.** It finds a widget with xwidget_info_get (0xc00000000006dcb0), its id with xwidget_info_id_get (0xc00000000006dd94) and its master with xwidget_info_master_get (0xc00000000006ddc8), then takes a spinlock with mutex_spinlock (0xc000000000287214) around updates to a table of 8-byte, 4-byte and 2-byte fields. Its path to sv_wait (0xc00000000018eda0) passes the constant 0x19. The facts do not show the condition under which it waits or what the table holds.

- `hub_piomap_free` in `6.5.22/unix.IP27` at 0xc00000000003b5ac
  **Inferred.** It looks up a hwgraph path through hwgraph_info_get_LBL (0xc000000000296160), with the pointer at a0+32 as its first argument, takes and releases a spinlock around updates to halfword and word fields at a0+0x24 and a0+0x28, and writes the doubleword at table 0x9200000001c00160. It ends by calling sv_signal (0xc00000000018f4fc) on the table at 0x9200000001c00000. The name and the signal are the evidence; the field meanings are not shown.

- `hub_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000003b6fc
  **Inferred.** It resolves a widget chain with xwidget_info_get (0xc00000000006dcb0), xwidget_info_id_get (0xc00000000006dd94) and xwidget_info_master_get (0xc00000000006ddc8), looks up a label with hwgraph_info_get_LBL (0xc000000000296160), and calls hub_piomap_addr (0xc00000000003b694) with its arg2 and arg3 as the second and third arguments. It returns 0x0 or an unresolved value. The facts show the forwarding but not how the address is computed.

- `hub_dma_init` in `6.5.22/unix.IP27` at 0xc00000000003b7c0
  **Inferred.** It is an 8-byte leaf with only a return. The facts show no loads, stores or calls, and the return value is not resolved.

- `hub_dmamap_alloc` in `6.5.22/unix.IP27` at 0xc00000000003b7c8
  **Inferred.** It finds the widget with xwidget_info_get (0xc00000000006dcb0), xwidget_info_id_get (0xc00000000006dd94) and xwidget_info_master_get (0xc00000000006ddc8), then calls kern_malloc (0xc000000000181da4) with size 0x10. It fills the returned block with a 4-byte word at +0, a word at +8 and +12, and a byte at +4, and returns the block pointer. The field meanings are not shown.

- `hub_dmamap_free` in `6.5.22/unix.IP27` at 0xc00000000003b848
  **Inferred.** It reads and writes the word at a0+0xc, then passes a0 to kern_free (0xc000000000181f2c). The store sits in the delay slot of that call, so the value written back is not resolved by the facts. The routine frees the map record that hub_dmamap_alloc created.

- `hub_dmamap_addr` in `6.5.22/unix.IP27` at 0xc00000000003b870
  **Inferred.** It reads the word at a0+0xc, writes it back at a0+0xc, and reads the word at a0+0. On one path it returns a1 without a call, and on the other it reports through cmn_err (0xc0000000001f8090) with level 4, the word at a0+0, and the word at a0+12 ORed with 0x2. The facts do not show what the address result is.

- `hub_dmamap_list` in `6.5.22/unix.IP27` at 0xc00000000003b8cc
  **Inferred.** It has the same shape as hub_dmamap_addr at 0xc00000000003b870: it reads and writes the word at a0+0xc, and on one path it reports through cmn_err (0xc0000000001f8090) with level 4 and the word at a0+0. A different message address is passed, so it is a sibling diagnostic path. The facts do not show the list operation itself.

- `hub_dmamap_done` in `6.5.22/unix.IP27` at 0xc00000000003b928
  **Inferred.** It reads the word at a0+0xc, writes a modified value back at a0+0xc, then reports through cmn_err (0xc0000000001f8090) with level 4 and the word at a0+0, passing a4 = a0 and a5 = -3. The facts do not show the modification or what the done state means.

- `hub_intr_init` in `6.5.22/unix.IP27` at 0xc00000000003b9a0
  **Inferred.** It is an 8-byte leaf with only a return. The facts show no loads, stores or calls, and the return value is not resolved.

- `hub_device_desc_update` in `6.5.22/unix.IP27` at 0xc00000000003b9a8
  **Inferred.** It calls device_desc_intr_swlevel_set (0xc000000000175f7c) with its arguments unchanged, formats a path with sprintf (0xc0000000001f8e8c) from the format at 0xc00000000140be10 and arg2, resolves that path with hwgraph_path_to_dev (0xc0000000002967d8), and calls device_desc_intr_target_set (0xc000000000175f6c) with arg0 and the device it found. The facts show the call order and the callees, not the descriptor layout.

- `hub_intr_alloc` in `6.5.22/unix.IP27` at 0xc00000000003bcac
  **Inferred.** It is a wrapper that calls do_hub_intr_alloc (0xc00000000003b9fc) with arguments a0 through a2 unchanged and a3 = 0, then returns that call's result. Its sibling hub_intr_alloc_nothd at 0xc00000000003bcc8 passes a3 = 1, so a3 looks like a mode flag for the callee.

- `hub_intr_alloc_nothd` in `6.5.22/unix.IP27` at 0xc00000000003bcc8
  **Inferred.** It is the same wrapper as hub_intr_alloc at 0xc00000000003bcac: it calls do_hub_intr_alloc (0xc00000000003b9fc) with arguments a0 through a2 unchanged and a3 = 1, then returns the result. The name suggests the flag selects a no-thread mode, but the facts do not show what do_hub_intr_alloc does with it.

- `hub_intr_free` in `6.5.22/unix.IP27` at 0xc00000000003bce4
  **Inferred.** It reads the word at a0+0x28, the halfword at a0+0x24 and the word at a0+0x2c, stores a byte at a0+5 and a doubleword at a0+8. It calls hub_intr_disconnect (0xc00000000003bde4), then kmem_free (0xc000000000181d4c) with size 0x30 on a0, and intr_unreserve_level (0xc000000000038084) with the halfword at a0+0x24 and the word at a0+40. This matches a release path for the record that hub_intr_alloc creates.
