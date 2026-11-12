# Tier-2 routine readings, kudzu batch 7

Role statements for 25 routines of the 6.5.22 IP27 kernel image, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. The facts show what each routine touches and calls, not what the
fields and constants mean, so each statement leaves that open where the facts stop.
Addresses are in the binary named in each header.

- `xbow_enable_llp_monitor` in `6.5.22/unix.IP27` at 0xc000000000070480
  **Inferred.** Stores one 4-byte value at offset 0x1c4 of the record that hwgraph_fastinfo_get (0xc000000000296ee8) returns; the facts do not show where the stored value comes from. It then arms a timeout through timeout (0xc0000000001b46fc) whose callback is at 0xc000000000070324, with a 0x64 argument. The function returns 0 at 0xc0000000000704c4.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_get_bridge_pointers` in `6.5.22/unix.IP27` at 0xc0000000000704cc
  **Inferred.** The routine first runs a loop of 16 iterations that stores an 8-byte value at offset 0 on each pass (0xc00000000007050c, 0xc000000000070528). It then calls hwgraph_traverse (0xc0000000002965ec) with the constant 0xc0000000003c6858 (0xc00000000007054c); a nonzero result takes the return at 0xc000000000070564, which is 0. A second hwgraph_traverse follows with 0xc0000000003c6860. It reads widget records through xwidget_info_get (0xc00000000006dcb0) and walks edges with hwgraph_edge_get_next (0xc000000000295ee8), the first of the two calls passing an extra argument of 0x2. It compares the word at +0x10 of each widget record with 0, 0xd000, 0xd100, 0xc002 and 0xd002, and the word at +0x18 with 0, 0x24 and 0x36. Per the disassembly, the 8-byte stores at 0xc0000000000705fc and 0xc0000000000706ac go through the output argument a1, and a 4-byte word reached through a2 is loaded and stored back (0xc000000000070600, 0xc000000000070608); the facts do not show the values stored. The only return, at 0xc000000000070564, is 0. The facts do not turn the constants into named widget types.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_disable` in `6.5.22/unix.IP27` at 0xc000000000070740
  **Inferred.** Logs once through cmn_err (0xc0000000001f8090) at entry, then calls hwgraph_fastinfo_get (0xc000000000296ee8) and reads the 4-byte field at +0x8 of the record it returns. That field is the first argument of the hwgraph_edge_get_next (0xc000000000295ee8) calls, the first with an extra argument of 0x2 (0xc000000000070784) and the next without (0xc0000000000707b0). The only return, at 0xc00000000007079c, yields -1; what the walk does to each edge is not visible in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_enable` in `6.5.22/unix.IP27` at 0xc0000000000707c8
  **Inferred.** Logs once through cmn_err (0xc0000000001f8090) at entry, then walks the edges of its first argument with hwgraph_edge_get_next (0xc000000000295ee8), first with an extra argument of 0x2 and then without. The only return, at 0xc000000000070818, yields -1. Unlike xbow_disable it reads no record field and stores nothing the facts show, so the enable step is not visible here.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_reset_link` in `6.5.22/unix.IP27` at 0xc000000000070844
  **Inferred.** A branch at 0xc000000000070860 on a word the facts show as [0xc0000000014c9de8] goes straight to us_delay (0xc000000000047274) with argument 0x3e8 (0xc00000000007092c) when that word is zero. Otherwise it calls xwidget_info_get (0xc00000000006dcb0) on its first argument and xwidget_info_id_get (0xc00000000006dd94) on the result, then hwgraph_traverse (0xc0000000002965ec) with the constant 0xc0000000003c68a8 and hwgraph_fastinfo_get (0xc000000000296ee8). It loads an 8-byte value at +0x10 of the fastinfo record, then does a word load and a word store at offsets the facts do not resolve (0xc0000000000708a8, 0xc0000000000708ac). Two us_delay calls follow (0xc0000000000708b4, 0xc0000000000708c8) with the word at 0xc0000000014c9de4 as argument, and a poll loop: a branch at 0xc0000000000708d8 on an unresolved word leaves the loop, and a branch at 0xc0000000000708f0 compares [0xc0000000014c9de8] with an unresolved value (unsigned). The return at 0xc00000000007091c is -1 or 0; the facts do not show which condition selects it or what the polled word means.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_init` in `6.5.22/unix.IP27` at 0xc000000000071ca0
  **Inferred.** Calls pcibr_debug (0xc0000000000767d0) once at entry with a0 = 1 and a1 = 0, then calls xwidget_driver_register (0xc00000000006df78) twice: first with 0xd002 and 0x24 as its first two arguments, then with 0xc002 and 0x36, both with the table at 0xc0000000003c6aa0 as the third argument and 0 as the fourth. The return at 0xc000000000071cec is the second registration's result. The facts do not say what the arguments mean.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.22/unix.IP27` at 0xc000000000071cf4
  **Inferred.** Role (from name): attaches a PCI bridge under a crossbar widget. Branches on 0xc002 (0xc000000000071d64) and 0xd002 (0xc000000000071f08) select variants, and the facts do not give their meaning. If pcibr_hints_get (0xc000000000079664) returns nonzero and the word at +0xc of its result is nonzero, the routine takes the return at 0xc000000000071e1c (-1 or 0). Otherwise it calls kmem_zalloc (0xc000000000181ce0) for 0xab8 bytes and memset (0xc00000000001f9c8) on the result, and hwgraph_fastinfo_set (0xc000000000296eb8) with that allocation as its second argument; the facts do not show which vertex is its first argument. Other calls in its body include pcibr_bridge_ptr_get (0xc000000000076a54), pciio_provider_register, hwgraph_char_device_add, pciio_device_win_map_new (0xc0000000000954c0) and pciio_device_win_populate for three windows, xtalk_intr_alloc and xtalk_intr_connect, several pcireg_* register accessors, pcibr_directmap_init, pcibr_slot_info_init, pcibr_slot_addr_space_init, pcibr_slot_device_init and pcibr_slot_call_device_attach in per-slot loops, and pciio_device_attach (0xc000000000095420) at 0xc0000000000724ec. The facts do not give an overall order beyond address order.
  Reviewed: 2026-11-12 (sonnet)

- `pciiox_init` in `6.5.22/unix.IP27` at 0xc000000000072714
  **Inferred.** Passes a constant table at 0xc0000000003c6c88, the address 0xc000000000072734 (the routine pciiox_vmc), and its own arguments a2 and a3 to nic_vmc_add (0xc00000000006bfb0), and returns that call's result. It is a thin wrapper; the facts do not show what nic_vmc_add does with the callback.
  Reviewed: 2026-11-12 (sonnet)

- `pciiox_vmc` in `6.5.22/unix.IP27` at 0xc000000000072734
  **Inferred.** Calls pcibr_hints_fix_rrbs (0xc0000000000797b8) with its arguments, then asks xtalk_piotrans_addr (0xc00000000006d7b0) for a translation with zero for two arguments, a length of 0x1000000 (16 MiB) and zero for the last. It stores two 4-byte words into the returned pointer at offsets 0x284 and 0x28c and returns that pointer. The stored values are not shown by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_try_set_device` in `6.5.22/unix.IP27` at 0xc000000000072784
  **Inferred.** Works on a 128-byte slot entry inside its first argument, at a0 + (a1 << 7), with 4-byte fields at +0x144, +0x148 and +0x14c, which it loads and stores back on paths selected by the fourth argument being 0x1100000, 0x8f8000 or 0x898000, and an 8-byte field at +0x138 that it loads and, on one path, stores (0xc000000000072b08). It takes the lock at a0+128 with mutex_spinlock (0xc000000000287214) and releases it with mutex_io_spinunlock (0xc000000000287120). One path releases it at once (0xc000000000072ae8); the other calls pcireg_device_set (0xc000000000088124), stores the 8-byte field, calls pcireg_tflush_get (0xc0000000000824c4) and then releases (0xc000000000072b18). A third release (0xc000000000072b54) is on a path selected by an unresolved test. It tests many single bits of its third argument (0x10, 0x20, 0x1000, 0x2000 and others). Role (from name): tries to apply a device setting; the facts do not decode which setting.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_release_device` in `6.5.22/unix.IP27` at 0xc000000000072c28
  **Inferred.** Takes the lock at a0+128 with mutex_spinlock (0xc000000000287214) and unlocks with mutex_io_spinunlock (0xc000000000287120); the return is the unlock call's result. Between them it compares its third argument with 0x1100000, 0x8f8000 and 0x898000, and only when it equals one of them does it load and store back the 4-byte field of the slot entry at a0 + (a1 << 7) at +0x144, +0x148 or +0x14c respectively (0xc000000000072c84, 0xc000000000072c98, 0xc000000000072cac). The value written back is not shown by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_device_write_gather_flush` in `6.5.22/unix.IP27` at 0xc000000000072cd8
  **Inferred.** Passes its four arguments unchanged to pcireg_wrb_flush_get (0xc0000000000887b0) and returns that call's result. It is a forwarding stub. The name suggests a write-gather buffer flush on a PCI device, and the callee is where that work happens.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_open` in `6.5.22/unix.IP27` at 0xc000000000072cf4
  **Inferred.** Checks a capability with cap_able_cred (0xc0000000002881b0), passing the constant 0x2000000000 and its own third and fourth arguments. It returns 0 or 1 from the return at 0xc000000000072d18, following the branch at 0xc000000000072d0c on the check result. The facts show the check but not the open work it guards.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_map` in `6.5.22/unix.IP27` at 0xc000000000072d30
  **Inferred.** Looks up the device's connection point with hwgraph_connectpt_get (0xc00000000029771c) and its record with hwgraph_fastinfo_get (0xc000000000296ee8), then drops the vertex reference with hwgraph_vertex_unref (0xc000000000295c58). It maps a physical range with v_mapphys (0xc0000000001ebbdc), whose second argument is a2 plus the 8-byte field at +0x8 of the record and whose fourth argument (a3) is -0x4000. It calls pcireg_control_bit_set (0xc000000000080e18) with 0x80000000 (0xc000000000072dec) only when the halfword at +0x72 of the record is zero and v_mapphys returned zero (fall-throughs at 0xc000000000072da8 and 0xc000000000072db0), and then the branch at 0xc000000000072dbc or 0xc000000000072dc4 is taken; the other paths return v_mapphys's result at 0xc000000000072ddc.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_unmap` in `6.5.22/unix.IP27` at 0xc000000000072dfc
  **Inferred.** Finds the device record the same way as pcibr_map, through hwgraph_connectpt_get (0xc00000000029771c) and hwgraph_fastinfo_get (0xc000000000296ee8), and drops the vertex reference with hwgraph_vertex_unref (0xc000000000295c58). If the halfword at +0x72 of the record is nonzero it returns 0 at 0xc000000000072e40 without touching the control register. If it is zero it reads the control register with pcireg_control_get (0xc00000000008097c) and calls pcireg_control_bit_clr (0xc000000000080c6c) with 0x80000000 when that bit is set. The only return listed, at 0xc000000000072e40, is 0.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_detach` in `6.5.22/unix.IP27` at 0xc000000000073428
  **Inferred.** Calls pcibr_debug and hwgraph_traverse (0xc0000000002965ec) with the constant 0xc0000000014104c0; when the traverse result is nonzero it returns (0xc00000000007346c, 0 or 1) before doing anything else. Otherwise it reads the device record from hwgraph_fastinfo_get (0xc000000000296ee8), takes the lock at +0x80 with mutex_spinlock, calls pcireg_intr_enable_set (0xc0000000000851bc) with 0 as its second argument, and releases the lock. It calls pcibr_slot_detach (0xc00000000007fe48) in a loop bounded by the bytes at +0x38 and +0x39, then pciio_device_info_unregister, spinlock_destroy, kmem_free on the string at +0x10, rmfreemap on the fields at +0x60 and +0x68 when nonzero, xtalk_intr_disconnect and xtalk_intr_free, kmem_free of the record with size 0xab8 (0xc000000000073568), and hwgraph_info_remove_LBL and two hwgraph_edge_remove calls. The 0xab8 size matches the allocation in xbridge_attach, so this reads as its counterpart.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_directmap_init` in `6.5.22/unix.IP27` at 0xc0000000000735b8
  **Inferred.** Loads the word at +0x4 of its first argument and passes it to device_admin_info_get (0xc0000000001761c4) with the constant 0xc0000000014104c8. Where that returns nonzero it calls hwgraph_path_to_vertex (0xc000000000296748); if that returns -1 it calls vertex_to_name and logs through cmn_err (0xc0000000001f8090) at 0xc0000000000736d4. It also loads words and bytes from the argument (+0x0, +0x30) and calls xtalk_dmatrans_addr (0xc00000000006d980) with a3 = 0x4000. It then calls pcireg_dirmap_diroff_set (0xc000000000083348), pcireg_dirmap_wid_set (0xc0000000000831ac) with the byte at +0x30, and one of pcireg_dirmap_add512_clr or pcireg_dirmap_add512_set with a5 = 0x20000000, chosen by comparing that constant with the xtalk_dmatrans_addr result (branch at 0xc000000000073634). It stores a byte at +0x58 and a doubleword at +0x50. The return is one of the two add512 results.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_asic_rev` in `6.5.22/unix.IP27` at 0xc0000000000736e4
  **Inferred.** Calls hwgraph_traverse (0xc0000000002965ec) with the constant 0xc000000001410528; a nonzero result returns -1 at 0xc00000000007370c. Otherwise it calls hwgraph_info_get_LBL (0xc000000000296160) with the constant 0xc0000000014104a0 and an output slot at sp-24, then hwgraph_vertex_unref (0xc000000000295c58), and branches at 0xc000000000073734 on an unresolved value. The value it returns on that path is not resolved by the facts. Role (from name): reads an ASIC revision; the path and label are constants the facts do not decode.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_write_gather_flush` in `6.5.22/unix.IP27` at 0xc00000000007374c
  **Inferred.** Gets the host info with pciio_hostinfo_get (0xc000000000094dbc) and the device record with pciio_info_mfast_get (0xc000000000094ec4). It loads a byte at +0x21c of the host info, though no branch on that byte is listed, and calls pcibr_device_write_gather_flush (0xc000000000072cd8) with the record and the byte at +540 of the host info. The return is 0.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_addr_pci_to_xio` in `6.5.22/unix.IP27` at 0xc000000000073788
  **Inferred.** Role (from name): translates a PCI-side address into a crossbar address for a device. It calls pcibr_info_get (0xc00000000007306c), pciio_info_get (0xc000000000094d64) and pciio_info_mfast_get (0xc000000000094ec4), then takes the lock at the record's +0x80 with mutex_spinlock (0xc000000000287214) and releases it with mutex_io_spinunlock (0xc000000000287120). It reads per-slot tables indexed by a1 (0x80-byte entries, with a word at +0x100 and a doubleword at +0x118, and a byte at +0x104). Its range compares use the constants 0x3fffffff, 0x7fffffff, 0xbfffffff, 0xffffffff, 0x1ffffffff and 0x13fffffff, and it compares the halfword at +0x72 of the device record with 2 and the word at +0x48 with 0 and 1. It calls sprintf (0xc0000000001f8e8c) four times, pcireg_control_bit_set (0xc000000000080e18) with 0x400000 on one path (the facts show a single value), pcireg_control_bit_clr, pcireg_device_set and pcireg_tflush_get. The return value is not resolved by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_piomap_alloc` in `6.5.22/unix.IP27` at 0xc000000000074058
  **Inferred.** Gets the bridge record with pcibr_info_get (0xc00000000007306c) and converts the address with pcibr_addr_pci_to_xio (0xc000000000073788); a result of -1 takes the branch at 0xc0000000000740ec to code at 0xc000000000074284, which has a pcibr_debug call (0xc0000000000767d0) at 0xc000000000074290. It takes the lock at the device record's +0x80 with mutex_spinlock (0xc000000000287214) and releases it with mutex_io_spinunlock (0xc000000000287120) at 0xc000000000074138, then allocates a 0x60-byte record with kmem_zalloc (0xc000000000181ce0). A second mutex_spinlock (0xc0000000000741a8, on a path selected by a branch on an unresolved word) and a second unlock (0xc0000000000741cc) follow, and xtalk_piomap_alloc (0xc00000000006d740) is called at 0xc0000000000741ec after them in address order, so the facts do not show it running with the lock held. Two pcibr_debug calls pass 0x8000000 as their first argument (0xc000000000074274, 0xc000000000074290). The layout of the 0x60-byte record beyond these stores is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_piomap_addr` in `6.5.22/unix.IP27` at 0xc0000000000742f0
  **Inferred.** Loads three 8-byte fields from its first argument, at +0x28, +0x10 and +0x30, and passes them as the fifth, fourth and first arguments of xtalk_piomap_addr (0xc00000000006d778) along with its own third argument as the third. The second argument is not resolved by the facts. It returns that call's result. It is a forwarding wrapper; what the three fields hold is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_piotrans_addr` in `6.5.22/unix.IP27` at 0xc000000000074338
  **Inferred.** Gets the host info and device record through pciio_hostinfo_get (0xc000000000094dbc) and pciio_info_mfast_get (0xc000000000094ec4), converts the address with pcibr_addr_pci_to_xio (0xc000000000073788), and asks xtalk_piotrans_addr (0xc00000000006d7b0) for the crossbar translation. It returns that translation, or 0 on the path where the conversion returns -1, and logs with pcibr_debug (0xc0000000000767d0) flag 0x10000000 on that failure path.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_flags_to_d64` in `6.5.22/unix.IP27` at 0xc000000000074728
  **Inferred.** Role (from name): converts a flags word (its first argument) and a device record (its second) into a 64-bit DMA attribute value. It tests many single bits of the flags, compares the halfword at +0x72 of the device record with 1 and 2, and tests bit 0x1 of the halfword at +0x74. The call to assfail (0xc00000000003a2b4) at 0xc0000000000748cc passes 0x7d0 as its third argument and is reached by branches the facts do not resolve; they do not show that it is a rejected combination. The facts do not show the value it builds.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_addr_xio_to_pci` in `6.5.22/unix.IP27` at 0xc000000000074ed4
  **Inferred.** Role (from name): the inverse of pcibr_addr_pci_to_xio. It takes a device record and a range (base a1, length a2), reads the halfword at +0x72, the word at +0x48 and the bytes at +0x38 and +0x39. It compares the base with 0x40000000, 0x80000000, 0xc0000000 and 0x100000000, and the range end (a1 + a2 - 1) with 0x7fffffff, 0xbfffffff, 0xffffffff and 0x13fffffff. Its return is a1 plus 0xffffffffc0000000 or a1 plus 0xffffffff80000000 on some paths, and an unresolved value on others.
  Reviewed: 2026-11-12 (sonnet)
