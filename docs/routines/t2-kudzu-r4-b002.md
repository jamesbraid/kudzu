# Tier-2 routine readings, kudzu batch 2

Rendered facts from the 6.5.22 IP27 unix kernel image, read for the 25 routines at list positions 50 to 74. Each role line cites the addresses, callees and constants it comes from. Where a sub-fact is not settled by the rendered facts, the line says so.

- `pcibr_ate_addr` in `6.5.22/unix.IP27` at 0xc000000000077238
  **Inferred.** Returns the address of one ATE register for the bridge structure at a0. It loads a halfword at a0+0x70 and compares it with a1 (signed); the internal path at 0xc00000000007724c calls pcireg_int_ate_addr (0xc00000000008ac5c), and the external path at 0xc00000000007725c calls pcireg_ext_ate_addr (0xc00000000008a5b8), both with the arguments passed through unchanged. The helper's result is the return value at 0xc000000000077270.

- `ate_write` in `6.5.22/unix.IP27` at 0xc000000000077278
  **Inferred.** Writes one address-translation (ATE) entry on the bridge structure at a0, through pcireg_int_ate_set (0xc00000000008a9f0) or pcireg_ext_ate_set (0xc00000000008a3ec). It sets the slot configuration with pcibr_slot_config_set (0xc00000000007dd44) and a2=4, and brackets the update with mutex_spinlock (0xc000000000287214) and mutex_io_spinunlock (0xc000000000287120) on a0+0x80, with a tflush read through pcireg_tflush_get (0xc0000000000824c4). The source of the entry fields in a1 is not settled by the rendered facts.

- `do_pcibr_rrb_count_valid` in `6.5.22/unix.IP27` at 0xc00000000007759c
  **Inferred.** Counts the read-response buffers (RRBs) of one bridge slot that are marked valid. It reads the word at a0+0x4 and calls pcireg_rrb_get (0xc000000000088a04) for each entry in a bounded loop, and a bad slot tag is reported through cmn_err (0xc0000000001f8090) at 0xc00000000007775c. The branch that carries the count to the return is not settled by the rendered facts.

- `do_pcibr_rrb_count_avail` in `6.5.22/unix.IP27` at 0xc00000000007776c
  **Inferred.** Reports how many RRBs a bridge slot has available, in the same shape as do_pcibr_rrb_count_valid at 0xc00000000007759c. It calls pcireg_rrb_get (0xc000000000088a04) per slot with the 0xff sentinel checked at 0xc0000000000777ac, and reports a bad slot tag through cmn_err (0xc0000000001f8090) at 0xc000000000077870. The return at 0xc000000000077808 is 0 or a count, and the exact count is not settled by the rendered facts.

- `do_pcibr_rrb_alloc` in `6.5.22/unix.IP27` at 0xc000000000077880
  **Inferred.** Allocates RRBs for a bridge slot. It reads the RRB word with pcireg_rrb_get (0xc000000000088a04), computes the new value and writes it back with pcireg_rrb_set (0xc000000000088c14) at 0xc0000000000779c0, and rejects a negative request count (the bltz test on a2 at 0xc0000000000778f8). It returns -1 (v0 = -0x1) on the rejected paths and 0 otherwise, reporting bad slot tags through cmn_err (0xc0000000001f8090) at 0xc000000000077aa4.

- `do_pcibr_rrb_free` in `6.5.22/unix.IP27` at 0xc000000000077ab4
  **Inferred.** Releases RRBs held by a bridge slot. It reads the RRB word with pcireg_rrb_get (0xc000000000088a04) at 0xc000000000077b8c, writes the result back with pcireg_rrb_set (0xc000000000088c14) at 0xc000000000077c04, and calls do_pcibr_rrb_clear (0xc000000000077e50) at 0xc000000000077c18 on one of its paths. It returns -1 or 0, and reports bad slot tags through cmn_err (0xc0000000001f8090) at 0xc000000000077d4c.

- `do_pcibr_rrb_free_all` in `6.5.22/unix.IP27` at 0xc000000000077d5c
  **Inferred.** Frees every RRB of a bridge by calling do_pcibr_rrb_free (0xc000000000077ab4) at 0xc000000000077dc8 with a3=0x8 on each pass. It reads the word at a1+0x4 for each pass, and its return value is the one that do_pcibr_rrb_free returns, passed back at 0xc000000000077e08.

- `do_pcibr_rrb_clear` in `6.5.22/unix.IP27` at 0xc000000000077e50
  **Inferred.** Clears the RRB status bits of a slot. It loops over pcireg_rrb_status_get (0xc000000000089144) and pcireg_rrb_clear_set (0xc0000000000892a8), testing the status against the masks in s1 and s2 at 0xc000000000077e80 and 0xc000000000077ea8, and it returns the last status value at 0xc000000000077ef4.

- `do_pcibr_rrb_flush` in `6.5.22/unix.IP27` at 0xc000000000077efc
  **Inferred.** Flushes the RRB state of one bridge slot. It reads the RRB value with pcireg_rrb_get (0xc000000000088a04), clears the slot through do_pcibr_rrb_clear (0xc000000000077e50) at 0xc000000000077f60, and writes the value back with pcireg_rrb_set (0xc000000000088c14) at 0xc000000000077f78. The return is the clear result or the set result, depending on the a3 test at 0xc000000000077f6c.

- `pcibr_rrb_flush` in `6.5.22/unix.IP27` at 0xc0000000000782c0
  **Inferred.** Flushes the RRBs of a device under the bridge lock. It resolves the host info with pciio_hostinfo_get (0xc000000000094dbc) and the fast-info record with pciio_info_mfast_get (0xc000000000094ec4), reads the RRB word with pcireg_rrb_get (0xc000000000088a04), and calls do_pcibr_rrb_flush (0xc000000000077efc) at 0xc0000000000783a0 inside mutex_spinlock (0xc000000000287214) and mutex_io_spinunlock (0xc000000000287120) on the record at +0x80. The slot tags 0xc002 and 0xd002 are tested at 0xc000000000078310 and 0xc000000000078324 before the flush path.

- `pcibr_wrb_flush` in `6.5.22/unix.IP27` at 0xc00000000007843c
  **Inferred.** Triggers the write-buffer flush of a bridge. It looks up the host info and fast-info record with pciio_hostinfo_get (0xc000000000094dbc) and pciio_info_mfast_get (0xc000000000094ec4), then calls pcireg_wrb_flush_get (0xc0000000000887b0) with the bridge pointer at [fast-info+8] and the byte at host info +0x21c as the second argument. The routine returns 0 at 0xc000000000078470.

- `pcibr_alloc_all_rrbs` in `6.5.22/unix.IP27` at 0xc0000000000789b0
  **Inferred.** Allocates RRBs for every slot of one bridge found by a graph path. It resolves the path with hwgraph_traverse (0xc0000000002965ec) using the constant at 0xc000000001411370, converts an address with xtalk_piotrans_addr (0xc00000000006d7b0) with size 8, and then writes each slot's RRB word with pcireg_rrb_set (0xc000000000088c14) under mutex_spinlock (0xc000000000287214) and mutex_io_spinunlock (0xc000000000287120). It releases the graph vertex with hwgraph_vertex_unref (0xc000000000295c58) and returns -1 or 0 at 0xc000000000078ac4.

- `pcibr_hints_intr_bits` in `6.5.22/unix.IP27` at 0xc000000000079830
  **Inferred.** Stores interrupt-bit hints for a bridge device. It fetches the hints record with pcibr_hints_get (0xc000000000079664) using the flag 1 and writes a 64-bit value at the record's +0x18 with the store at 0xc000000000079854. If the record is missing it calls pcibr_debug (0xc0000000000767d0) and returns its result. The source of the stored 64-bit value is unknown from the rendered facts.

- `pcibr_wrap_get` in `6.5.22/unix.IP27` at 0xc000000000079a90
  **Inferred.** Returns an 8-byte field from a wrapper table indexed by the word at a0+0x8. It reads a0+0x4 and a0+0x8, scales the index by 8 to step into the table, and returns the 8-byte field at +8 of the selected entry at 0xc000000000079ad4. On one path it calls panic (0xc0000000001f9228) at 0xc000000000079ae0 when the tag at a0+0x4 does not match the index.

- `pcibr_wrap_put` in `6.5.22/unix.IP27` at 0xc000000000079af4
  **Inferred.** Stores an 8-byte value into the wrapper table entry selected by the index at a1+0x4, under mutex_spinlock (0xc000000000287214) on the record a1. It checks the tag at a1+0x8 against a5 at 0xc000000000079b30, calls panic (0xc0000000001f9228) at 0xc000000000079b70 on a mismatch, and releases the lock with mutex_spinunlock (0xc000000000287260). The source of the stored 8-byte value is not settled by the rendered facts.

- `pcibr_force_interrupt` in `6.5.22/unix.IP27` at 0xc000000000079b88
  **Inferred.** Forces an interrupt on one bridge interrupt line. It loads the bridge pointer from [a0] and the halfword at +0x72 of that bridge, writes the force register through pcireg_force_intr_set (0xc000000000087ce4) with [a0+8] as the bit, and reads the status through pcireg_intr_status_get (0xc00000000084ef0), retrying while the status bit is clear at 0xc000000000079bdc. The vector and CPU id come from xtalk_intr_vector_get (0xc00000000006dc34) and xtalk_intr_cpuid_get (0xc00000000006dab4). The 64-bit constant stored at 0xc000000000079c4c is unknown from the rendered facts.

- `pcibr_intrd` in `6.5.22/unix.IP27` at 0xc000000000079c50
  **Inferred.** Body of the interrupt service thread for a bridge. It takes the semaphore at a0+0x30 with ipsema (0xc00000000018abb4) at 0xc000000000079ca0, and then decrements a pending counter in the wrapper record at ret(pcibr_wrap_get)+0x18 with ll/sc at 0xc000000000079c7c and 0xc000000000079c88, calling pcibr_force_interrupt (0xc000000000079b88) at 0xc000000000079cb8 for each pending interrupt. The indirect call at 0xc000000000079c64 has a target that is not resolved.

- `pcibr_intrd_start` in `6.5.22/unix.IP27` at 0xc000000000079cc8
  **Inferred.** Starts the interrupt service thread for a bridge. It sets the thread's must-run CPU from the halfword at a0+0x50 with setmustrun (0xc0000000001959f4), installs pcibr_intrd (0xc000000000079c50) as its function with xthread_set_func (0xc0000000001c771c), and sets a bit in the flags at a0+0x28 with ll/sc at 0xc000000000079cf8. It then waits on the semaphore at a0+0x30 with ipsema (0xc00000000018abb4).

- `pcibr_thread_setup` in `6.5.22/unix.IP27` at 0xc000000000079d24
  **Inferred.** Creates the interrupt service thread for a bridge. It formats the thread name with sprintf (0xc0000000001f8e8c) into a stack buffer, calls xthread_setup (0xc0000000001c75c8) with pcibr_intrd_start (0xc000000000079cc8) as the start function, and records the thread state by storing a halfword at a0+0x48 and setting bits in the word at a0+0x28 with ll/sc. The return value is the result of xthread_setup.

- `pcibr_intr_alloc` in `6.5.22/unix.IP27` at 0xc000000000079d9c
  **Inferred.** Allocates the per-interrupt descriptor for a bridge interrupt. It calls kmem_zalloc (0xc000000000181ce0) for a 0xa0-byte record and for a 0x20-byte helper, initializes a spinlock in the record with spinlock_init (0xc000000000286fe8), and installs the helper in an empty slot with compare_and_swap_int64 (0xc0000000000479ec). It reserves the widget interrupt with xtalk_intr_alloc_nothd (0xc00000000006da28), sets the device bit with pcireg_intr_device_bit_set (0xc00000000008614c), and on the failure paths frees the helper with kmem_free (0xc000000000181d4c) and reports through cmn_err (0xc0000000001f8090).

- `pcibr_intr_free` in `6.5.22/unix.IP27` at 0xc00000000007a3f0
  **Inferred.** Tears down a bridge interrupt descriptor made by pcibr_intr_alloc. It frees the widget interrupt with xtalk_intr_free (0xc00000000006da44), clears the device bit with pcireg_intr_device_bit_clr (0xc0000000000862c0), and clears the descriptor's slot with compare_and_swap_int64 (0xc0000000000479ec). It signals the semaphore at +0x30 with vsema (0xc00000000018ac88), stops the thread with xthread_set_func (0xc0000000001c771c), and frees the 0xa0-byte record with kmem_free (0xc000000000181d4c).

- `pcibr_setpciint` in `6.5.22/unix.IP27` at 0xc00000000007a5e8
  **Inferred.** Programs a PCI interrupt target from its widget-side descriptor. It reads the address, vector, device and sfarg of the interrupt through xtalk_intr_addr_get (0xc00000000006dc3c), xtalk_intr_vector_get (0xc00000000006dc34), xtalk_intr_dev_get (0xc00000000006dc24) and xtalk_intr_sfarg_get (0xc00000000006dc44). It then writes the vector with pcireg_intr_addr_vect_set (0xc000000000086f2c) and the address with pcireg_intr_addr_addr_set (0xc000000000087398), both on the bridge pointer from pcibr_bridge_ptr_get (0xc000000000076a54).

- `pcibr_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000007a690
  **Inferred.** Connects a bridge interrupt. It stores the descriptor fields at offsets +0x18, +0x20 and +0x0 of the record, sets the enable bit with pcireg_intr_enable_bit_set (0xc000000000085494) under mutex_spinlock (0xc000000000287214) on the bridge record at +0x80, and flushes with pcireg_tflush_get (0xc0000000000824c4). It then calls xtalk_intr_connect (0xc00000000006da60) with pcibr_setpciint (0xc00000000007a5e8) as the callback. Its second argument is the code address 0xc00000000007ab0c, whose routine is outside this entry's rendered facts.

- `pcibr_intr_disconnect` in `6.5.22/unix.IP27` at 0xc00000000007a834
  **Inferred.** Disconnects a bridge interrupt from its widget. It clears the enable bit with pcireg_intr_enable_bit_clr (0xc000000000085314) under mutex_spinlock (0xc000000000287214), then calls xtalk_intr_disconnect (0xc00000000006da7c) at 0xc00000000007a984. When the descriptor still has bits set, it reconnects with xtalk_intr_connect (0xc00000000006da60) using pcibr_setpciint (0xc00000000007a5e8) as the callback, at 0xc00000000007a9d8.

- `pcibr_setwidint` in `6.5.22/unix.IP27` at 0xc00000000007aa88
  **Inferred.** Programs the widget-side interrupt destination of a bridge. It reads the target, address, vector and sfarg with xtalk_intr_target_get (0xc00000000006dc2c), xtalk_intr_addr_get (0xc00000000006dc3c), xtalk_intr_vector_get (0xc00000000006dc34) and xtalk_intr_sfarg_get (0xc00000000006dc44). It writes the target ID with pcireg_intr_dst_target_id_set (0xc000000000081b8c), the destination address with pcireg_intr_dst_addr_set (0xc000000000081ea8), and the host-error setting with pcireg_intr_host_err_set (0xc0000000000865a4).
