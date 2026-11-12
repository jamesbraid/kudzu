# Tier-2 routine readings, kudzu batch 2

Rendered facts from the 6.5.22 IP27 unix kernel image, read for the 25 routines at list positions 50 to 74. Each role line cites the addresses, callees and constants it comes from. Where a sub-fact is not settled by the rendered facts, the line says so.

- `pcibr_ate_addr` in `6.5.22/unix.IP27` at 0xc000000000077238
  **Inferred.** Returns the address of one ATE register for the bridge structure at a0. It loads a halfword at a0+0x70 and compares it with a1 (signed); the internal path at 0xc00000000007724c calls pcireg_int_ate_addr (0xc00000000008ac5c), and the external path at 0xc00000000007725c calls pcireg_ext_ate_addr (0xc00000000008a5b8), both with the arguments passed through unchanged. The helper's result is the return value at 0xc000000000077270.
  Reviewed: 2026-11-12 (sonnet)

- `ate_write` in `6.5.22/unix.IP27` at 0xc000000000077278
  **Inferred.** Writes address-translation (ATE) entries on the bridge structure at a0. It calls pcireg_int_ate_set (0xc00000000008a9f0) at 0xc0000000000772ec or pcireg_ext_ate_set (0xc00000000008a3ec) at 0xc000000000077318 inside a loop that exits when a3 is not positive (blez at 0xc0000000000772d4); a comparison against the halfword at a0+0x70 (bne at 0xc00000000007730c) is on the path between them. When the halfword at a0+0x72 is zero and a2 is nonzero (bne at 0xc000000000077330, beq at 0xc000000000077338), it calls pcibr_slot_config_set (0xc00000000007dd44) with a2=0x4 at 0xc000000000077360. It calls mutex_io_spinunlock (0xc000000000287120) on a0+0x80 at 0xc0000000000773f0 and pcireg_tflush_get (0xc0000000000824c4) at 0xc0000000000773f8. A later path, reached when the bgez at 0xc00000000007742c falls through, calls mutex_spinlock (0xc000000000287214) on a0+0x80 at 0xc000000000077434 and, further on, xtalk_dmamap_done (0xc00000000006d964) at 0xc000000000077498. Which iteration holds the lock, the source of the entry fields in a1 and the meaning of a2 are not settled by the rendered facts.
  Reviewed: 2026-11-12 (sonnet)

- `do_pcibr_rrb_count_valid` in `6.5.22/unix.IP27` at 0xc00000000007759c
  **Inferred.** Role (from name): counts the valid read-response buffers (RRBs) of one bridge slot. It compares the word at +0x4 of a loaded value with the constants 0xc002, 0xd002, 0x36 and 0x24 (beq at 0xc0000000000775d4 and 0xc0000000000775ec, bne at 0xc0000000000776bc and 0xc0000000000776dc), and calls pcireg_rrb_get (0xc000000000088a04) at 0xc000000000077648 inside a loop (bne at 0xc000000000077754 returns to 0xc000000000077620). It calls cmn_err (0xc0000000001f8090, level 4) at 0xc00000000007775c when a 0/1 validity value is zero (beq at 0xc000000000077618). The return at 0xc0000000000776a4 is 0 or an unresolved value; the facts do not show a count or what each iteration adds.
  Reviewed: 2026-11-12 (sonnet)

- `do_pcibr_rrb_count_avail` in `6.5.22/unix.IP27` at 0xc00000000007776c
  **Inferred.** Role (from name): reports how many RRBs a bridge slot has available. It has the same tag checks as do_pcibr_rrb_count_valid at 0xc00000000007759c: the word at +0x4 is compared with 0xc002, 0xd002, 0x36 and 0x24. It calls pcireg_rrb_get (0xc000000000088a04) at 0xc0000000000777c8 with a4=0xff inside a loop (bne at 0xc000000000077864 returns to it), after comparing a value with 0xff at 0xc0000000000777ac. It calls cmn_err (0xc0000000001f8090, level 4) at 0xc000000000077870 when a 0/1 validity value is zero (beq at 0xc0000000000777c0). The return at 0xc000000000077808 is 0 or an unresolved value; the facts do not show a count.
  Reviewed: 2026-11-12 (sonnet)

- `do_pcibr_rrb_alloc` in `6.5.22/unix.IP27` at 0xc000000000077880
  **Inferred.** Role (from name): allocates RRBs for a bridge slot. It calls pcireg_rrb_get (0xc000000000088a04) at 0xc000000000077944, then pcireg_rrb_set (0xc000000000088c14) at 0xc0000000000779c0; the facts do not show how the value passed to pcireg_rrb_set is computed. The test of a2 (bltz at 0xc0000000000778f8, together with tests of a1 against 0xff and of two other values) feeds a 0/1 validity value; when that value is zero (beq at 0xc00000000007790c) the code reaches cmn_err (0xc0000000001f8090, level 4) at 0xc000000000077aa4. The return at 0xc0000000000779e8 is -0x1 or 0x0; which path gives which is not settled by the rendered facts.
  Reviewed: 2026-11-12 (sonnet)

- `do_pcibr_rrb_free` in `6.5.22/unix.IP27` at 0xc000000000077ab4
  **Inferred.** Role (from name): releases RRBs held by a bridge slot. It calls pcireg_rrb_get (0xc000000000088a04) at 0xc000000000077b8c and, after a loop that tests a3 (blez at 0xc000000000077b9c and 0xc000000000077bc8), pcireg_rrb_set (0xc000000000088c14) at 0xc000000000077c04; the value passed to pcireg_rrb_set is not settled by the rendered facts. It calls do_pcibr_rrb_clear (0xc000000000077e50) at 0xc000000000077c18 inside a second loop bounded by 0x8 (beq at 0xc000000000077c2c). It returns -0x1 or 0x0 at 0xc000000000077c80, and calls cmn_err (0xc0000000001f8090, level 4) at 0xc000000000077d4c when a 0/1 validity value is zero (beq at 0xc000000000077b50).
  Reviewed: 2026-11-12 (sonnet)

- `do_pcibr_rrb_free_all` in `6.5.22/unix.IP27` at 0xc000000000077d5c
  **Inferred.** Role (from name): frees all RRBs of a bridge. It calls do_pcibr_rrb_free (0xc000000000077ab4) at 0xc000000000077dc8 with a0 and a1 taken from its own a1 and a2 at entry and a3=0x8, in a loop that ends when a counter reaches 0x2 or 0x4 (bne at 0xc000000000077de4). The loads of the word at +0x4 (at 0xc000000000077d68 and 0xc000000000077da4, with tag compares against 0xc002, 0xd002, 0x36 and 0x24) come before the call. The return at 0xc000000000077e08 is the result of do_pcibr_rrb_free.
  Reviewed: 2026-11-12 (sonnet)

- `do_pcibr_rrb_clear` in `6.5.22/unix.IP27` at 0xc000000000077e50
  **Inferred.** Role (from name): clears RRB status bits of a slot. It calls pcireg_rrb_status_get (0xc000000000089144) at 0xc000000000077e68 and polls it again at 0xc000000000077e88 while a mask in s1 has bits set in the result (bne at 0xc000000000077e94). Then, when a mask in s2 has bits set in the status (beq at 0xc000000000077ea8), it calls pcireg_rrb_clear_set (0xc0000000000892a8) once at 0xc000000000077eb0, reads the status at 0xc000000000077eb8 and polls pcireg_rrb_status_get at 0xc000000000077ecc while the s2 bits stay set (bne at 0xc000000000077ed8). It returns the last pcireg_rrb_status_get result at 0xc000000000077ef4.
  Reviewed: 2026-11-12 (sonnet)

- `do_pcibr_rrb_flush` in `6.5.22/unix.IP27` at 0xc000000000077efc
  **Inferred.** Flushes the RRB state of one bridge slot. It reads the RRB value with pcireg_rrb_get (0xc000000000088a04), clears the slot through do_pcibr_rrb_clear (0xc000000000077e50) at 0xc000000000077f60, and writes the value back with pcireg_rrb_set (0xc000000000088c14) at 0xc000000000077f78. The return is the clear result or the set result, depending on the a3 test at 0xc000000000077f6c.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_rrb_flush` in `6.5.22/unix.IP27` at 0xc0000000000782c0
  **Inferred.** Flushes the RRBs of a device under the bridge lock. It resolves the host info with pciio_hostinfo_get (0xc000000000094dbc) and the fast-info record with pciio_info_mfast_get (0xc000000000094ec4), reads the RRB word with pcireg_rrb_get (0xc000000000088a04), and calls do_pcibr_rrb_flush (0xc000000000077efc) at 0xc0000000000783a0 inside mutex_spinlock (0xc000000000287214) and mutex_io_spinunlock (0xc000000000287120) on the record at +0x80. The slot tags 0xc002 and 0xd002 are tested at 0xc000000000078310 and 0xc000000000078324 before the flush path.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_wrb_flush` in `6.5.22/unix.IP27` at 0xc00000000007843c
  **Inferred.** Role (from name): write-buffer flush of a bridge. It looks up the host info and fast-info record with pciio_hostinfo_get (0xc000000000094dbc) and pciio_info_mfast_get (0xc000000000094ec4), then calls pcireg_wrb_flush_get (0xc0000000000887b0) at 0xc00000000007845c with the doubleword at [fast-info+8] as the first argument and the byte at host info +0x21c as the second. The routine returns 0 at 0xc000000000078470; the facts do not show what pcireg_wrb_flush_get does with its arguments.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_alloc_all_rrbs` in `6.5.22/unix.IP27` at 0xc0000000000789b0
  **Inferred.** Role (from name): allocates RRBs for the slots of one bridge found by a graph path. It resolves the path with hwgraph_traverse (0xc0000000002965ec, a1=0xc000000001411370) at 0xc0000000000789f8 and, when that returns nonzero, calls xtalk_piotrans_addr (0xc00000000006d7b0) with a3=0x8 at 0xc000000000078a1c. A slot loop (compares against 0x9 at 0xc000000000078af4) calls pcireg_rrb_set (0xc000000000088c14) at 0xc000000000078c34. mutex_spinlock (0xc000000000287214) at 0xc000000000078c18 is skipped when a value is zero (beq at 0xc000000000078c10), and mutex_io_spinunlock (0xc000000000287120) at 0xc000000000078cc8 is skipped when a value tested after pcireg_rrb_set is zero (beq at 0xc000000000078c40). hwgraph_fastinfo_get (0xc000000000296ee8) is called at 0xc000000000078cf8 and hwgraph_vertex_unref (0xc000000000295c58) at 0xc000000000078d10. It returns -0x1 or 0x0 at 0xc000000000078ac4; the facts do not show the arguments of the spinlock or pcireg_rrb_set calls.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_hints_intr_bits` in `6.5.22/unix.IP27` at 0xc000000000079830
  **Inferred.** Stores interrupt-bit hints for a bridge device. It fetches the hints record with pcibr_hints_get (0xc000000000079664) using the flag 1 and writes a 64-bit value at the record's +0x18 with the store at 0xc000000000079854. If the record is missing it calls pcibr_debug (0xc0000000000767d0) and returns its result. The source of the stored 64-bit value is unknown from the rendered facts.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_wrap_get` in `6.5.22/unix.IP27` at 0xc000000000079a90
  **Inferred.** Returns an 8-byte field from a table in the record at a0. It loads the words at a0+0x4 and a0+0x8 and compares them (beq at 0xc000000000079aa4). When they are equal it calls panic (0xc0000000001f9228) at 0xc000000000079ae0. When they differ it stores a word to a0+0x8 (sw at 0xc000000000079abc and again at 0xc000000000079acc) and returns, at 0xc000000000079ad4, the doubleword at +8 of the entry at a0 + (([a0+8]+1) << 3). The facts do not show what the second store writes or what the table holds.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_wrap_put` in `6.5.22/unix.IP27` at 0xc000000000079af4
  **Inferred.** Stores into a table in the record a1 under mutex_spinlock (0xc000000000287214) on a1, called at 0xc000000000079b0c. It loads the words at a1+0x4 and a1+0x8 and compares the latter with a5 (beq at 0xc000000000079b30). When they are equal it calls panic (0xc0000000001f9228) at 0xc000000000079b70. When they differ it stores a doubleword at +0x10 (sd at 0xc000000000079b44) and a word at a1+0x4 (sw at 0xc000000000079b48), then releases the lock with mutex_spinunlock (0xc000000000287260) at 0xc000000000079b4c and returns its result. The facts do not show which value the doubleword store writes or how a5 is computed.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_force_interrupt` in `6.5.22/unix.IP27` at 0xc000000000079b88
  **Inferred.** Role (from name): forces an interrupt on a bridge interrupt line. It loads a bridge pointer from [a0] and the halfword at +0x72 of it. When that halfword is nonzero (beq at 0xc000000000079ba0 falls through) it calls pcireg_force_intr_set (0xc000000000087ce4) at 0xc000000000079bac with a0=[a0] and [a0+8] as the second argument; otherwise it goes straight to pcireg_intr_status_get (0xc000000000084ef0) at 0xc000000000079bc8. It branches back to 0xc000000000079bb8 while a masked status bit is zero (beq at 0xc000000000079bdc). It then calls xtalk_intr_vector_get (0xc00000000006dc34) at 0xc000000000079bf8 and xtalk_intr_cpuid_get (0xc00000000006dab4) at 0xc000000000079c04, and stores a 64-bit constant at 0xc000000000079c4c that is unknown from the rendered facts.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_intrd` in `6.5.22/unix.IP27` at 0xc000000000079c50
  **Inferred.** Role (from name): body of the interrupt service thread for a bridge. It makes an indirect call at 0xc000000000079c64 with a0=[a0+32] whose target is not resolved. It calls pcibr_wrap_get (0xc000000000079a90) with a0+104 at 0xc000000000079c6c, then decrements the word at ret(pcibr_wrap_get)+0x18 with ll/sc at 0xc000000000079c7c and 0xc000000000079c88. It reloads that word and, when it is nonzero (beq at 0xc000000000079c98 falls through), calls ipsema (0xc00000000018abb4) on a0+0x30 at 0xc000000000079ca0 and returns its result at 0xc000000000079cb0. When it is zero it calls pcibr_force_interrupt (0xc000000000079b88) at 0xc000000000079cb8 with a0=ret(pcibr_wrap_get) and a2=-0x1.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_intrd_start` in `6.5.22/unix.IP27` at 0xc000000000079cc8
  **Inferred.** Starts the interrupt service thread for a bridge. It sets the thread's must-run CPU from the halfword at a0+0x50 with setmustrun (0xc0000000001959f4), installs pcibr_intrd (0xc000000000079c50) as its function with xthread_set_func (0xc0000000001c771c), and sets a bit in the flags at a0+0x28 with ll/sc at 0xc000000000079cf8. It then waits on the semaphore at a0+0x30 with ipsema (0xc00000000018abb4).
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_thread_setup` in `6.5.22/unix.IP27` at 0xc000000000079d24
  **Inferred.** Creates the interrupt service thread for a bridge. It formats the thread name with sprintf (0xc0000000001f8e8c) into a stack buffer, calls xthread_setup (0xc0000000001c75c8) with pcibr_intrd_start (0xc000000000079cc8) as the start function, and records the thread state by storing a halfword at a0+0x48 and setting bits in the word at a0+0x28 with ll/sc. The return value is the result of xthread_setup.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_intr_alloc` in `6.5.22/unix.IP27` at 0xc000000000079d9c
  **Inferred.** Allocates the per-interrupt descriptor for a bridge interrupt. It calls kmem_zalloc (0xc000000000181ce0) for a 0xa0-byte record and for a 0x20-byte helper, initializes a spinlock in the record with spinlock_init (0xc000000000286fe8), and installs the helper in an empty slot with compare_and_swap_int64 (0xc0000000000479ec). It reserves the widget interrupt with xtalk_intr_alloc_nothd (0xc00000000006da28), sets the device bit with pcireg_intr_device_bit_set (0xc00000000008614c), and on the failure paths frees the helper with kmem_free (0xc000000000181d4c) and reports through cmn_err (0xc0000000001f8090).
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_intr_free` in `6.5.22/unix.IP27` at 0xc00000000007a3f0
  **Inferred.** Tears down a bridge interrupt descriptor, as the pair of pcibr_intr_alloc suggests. In a loop that is left when a counter reaches 0x8 (beq at 0xc00000000007a468) it may call xtalk_intr_free (0xc00000000006da44) at 0xc00000000007a43c, depending on the tests at 0xc00000000007a478 to 0xc00000000007a4dc. It also calls pcireg_intr_device_bit_clr (0xc0000000000862c0) on [a0+0x60] at 0xc00000000007a450 and compare_and_swap_int64 (0xc0000000000479ec) with a1=a0 and a2=0 at 0xc00000000007a4a0. On a path that tests bit 0x400 of the word at a0+0x28 (beq at 0xc00000000007a504) it calls xthread_set_func (0xc0000000001c771c) with a1=0xc0000000001c7698 at 0xc00000000007a51c and then vsema (0xc00000000018ac88) on a0+0x30 at 0xc00000000007a524; what the thread function does is not settled by the rendered facts. It frees the 0xa0-byte record with kmem_free (0xc000000000181d4c) at 0xc00000000007a548. Another path loops on nano_delay (0xc0000000001b5a98) at 0xc00000000007a594 while a counter stays below 0x3e8 (bne at 0xc00000000007a5b0) and reports through cmn_err (0xc0000000001f8090, level 4) at 0xc00000000007a5c8.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_setpciint` in `6.5.22/unix.IP27` at 0xc00000000007a5e8
  **Inferred.** Role (from name): programs a PCI interrupt target from its widget-side descriptor. It calls xtalk_intr_addr_get (0xc00000000006dc3c), xtalk_intr_vector_get (0xc00000000006dc34), xtalk_intr_dev_get (0xc00000000006dc24) and xtalk_intr_sfarg_get (0xc00000000006dc44, twice), then pcibr_bridge_ptr_get (0xc000000000076a54) on the result of xtalk_intr_dev_get. It calls pcireg_intr_addr_vect_set (0xc000000000086f2c) at 0xc00000000007a664 and pcireg_intr_addr_addr_set (0xc000000000087398) at 0xc00000000007a678, both with a0 set to the bridge pointer; only the second shows its value argument (a2 is the result of xtalk_intr_addr_get). It returns the result of pcireg_intr_addr_addr_set.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000007a690
  **Inferred.** Role (from name): connects a bridge interrupt. It stores doublewords at +0x20 and +0x18 and a word at +0x0 of the record (0xc00000000007a6f4, 0xc00000000007a700, 0xc00000000007a70c). It then loops over eight slots (s0 below 0x8, bne at 0xc00000000007a748 and 0xc00000000007a824): a slot is skipped when its bit in the word at +0x58 is clear (beq at 0xc00000000007a720) or a loaded word is nonzero (beq at 0xc00000000007a738 falls through); otherwise it calls pcireg_intr_addr_addr (0xc000000000086b1c), then xtalk_intr_connect (0xc00000000006da60) at 0xc00000000007a7e0 with a1=0xc00000000007ab0c and a3=0xc00000000007a5e8 (pcibr_setpciint), stores a word at 0xc00000000007a7f0 and calls pcireg_intr_addr_get (0xc000000000086700). After the loop it calls mutex_spinlock (0xc000000000287214) on [a0+0x60]+0x80, pcireg_intr_enable_bit_set (0xc000000000085494), pcireg_tflush_get (0xc0000000000824c4) and mutex_io_spinunlock (0xc000000000287120). The routine at 0xc00000000007ab0c is outside this entry's rendered facts. The return is -0x1 or 0x0.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_intr_disconnect` in `6.5.22/unix.IP27` at 0xc00000000007a834
  **Inferred.** Role (from name): disconnects a bridge interrupt from its widget. It stores doublewords at +0x18 and +0x20 on entry. When a value tested at 0xc00000000007a8c8 is nonzero it calls mutex_spinlock (0xc000000000287214) on [a0+0x60]+0x80, pcireg_intr_enable_bit_clr (0xc000000000085314), pcireg_tflush_get (0xc0000000000824c4) and mutex_io_spinunlock (0xc000000000287120). It calls xtalk_intr_disconnect (0xc00000000006da7c) at 0xc00000000007a984 on a path reached when a loaded word is zero (beq at 0xc00000000007a948). A later call to xtalk_intr_connect (0xc00000000006da60) at 0xc00000000007a9d8, with a1=0xc00000000007ab0c and a3=0xc00000000007a5e8 (pcibr_setpciint), is skipped when a loaded word is zero (beq at 0xc00000000007a9ac); the facts do not show what that word holds.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_setwidint` in `6.5.22/unix.IP27` at 0xc00000000007aa88
  **Inferred.** Role (from name): programs the widget-side interrupt destination of a bridge. It calls xtalk_intr_target_get (0xc00000000006dc2c), xtalk_intr_addr_get (0xc00000000006dc3c), xtalk_intr_vector_get (0xc00000000006dc34) and xtalk_intr_sfarg_get (0xc00000000006dc44), then pcireg_intr_dst_target_id_set (0xc000000000081b8c), pcireg_intr_dst_addr_set (0xc000000000081ea8) and pcireg_intr_host_err_set (0xc0000000000865a4), each with a0 set to the result of xtalk_intr_sfarg_get. Only pcireg_intr_dst_addr_set shows a value argument (a1 is the result of xtalk_intr_addr_get); the values passed to the other two are not settled by the rendered facts. It returns the result of pcireg_intr_host_err_set.
  Reviewed: 2026-11-12 (sonnet)
