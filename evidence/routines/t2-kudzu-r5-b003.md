# Routine readings, round r5, batch 3

These role statements cover the 25 routines at positions 75 to 99 of the round r5 ranked list. Each one comes from the rendered facts of its routine: calls, branches, loads, stores and returns at the addresses cited. Where the facts leave a meaning open, the statement says so.

- `xtalk_dmaaddr_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9d4
  **Inferred.** It calls hub_dmaaddr_drain [@0xc00000000006d9dc] and returns the result of hub_dmaaddr_drain [@0xc00000000006d9e8].
  Role (from facts): a forwarding wrapper around the hub's DMA address drain, with no logic of its own [@0xc00000000006d9dc].
  Reviewed: 2026-11-12

- `xtalk_dmalist_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9f0
  **Inferred.** It calls hub_dmalist_drain [@0xc00000000006d9f8] and returns the result of hub_dmalist_drain [@0xc00000000006da04].
  Role (from facts): a forwarding wrapper around the hub's DMA list drain, with no logic of its own [@0xc00000000006d9f8].
  Reviewed: 2026-11-12

- `xbow_init` in `6.5.22/unix.IP27` at 0xc00000000006e658
  **Inferred.** It calls xwidget_driver_register three times [call 1]. Call 1 has a0 = 0xd100, a1 = 0x0, a2 = 0xc0000000003c6098 and a3 = 0x1 [call 1]. Call 2 has a0 = 0xd000, a1 = 0x0, a2 = 0xc0000000003c6098 and a3 = 0x1 [call 2]. Call 3 has a0 = 0x0, a1 = 0x0, a2 = 0xc0000000003c6098 and a3 = 0x1 [call 3].
  It returns the result of xwidget_driver_register [@0xc00000000006e6a0].
  Role (from facts): it registers three widget drivers that share a2 and a3 and differ in a0 [call 3].
  Reviewed: 2026-11-12

- `xbow_widget_num_get` in `6.5.22/unix.IP27` at 0xc00000000006ea4c
  **Inferred.** It calls vertex_to_name [@0xc00000000006ea60] and then nodevertex_to_cnodeid [@0xc00000000006ea68].
  When nodevertex_to_cnodeid returns -1 it does not call hub_widget_id [@0xc00000000006ea74 taken]. On the other side it calls hub_widget_id [call 3, @0xc00000000006ea74 fall].
  It returns the result of hub_widget_id, the value 0xff, or the result of xwidget_info_id_get [@0xc00000000006eaa4].
  Role (from facts): it finds the widget number of a vertex from the hub widget id or from the xwidget info record [call 8].
  Reviewed: 2026-11-12

- `xbow_link_alive` in `6.5.22/unix.IP27` at 0xc00000000006ef18
  **Inferred.** It loads a 4-byte word from an address the facts leave unresolved [@0xc00000000006ef24] and returns the top bit of it, the word shifted right by 31 [@0xc00000000006ef28].
  Role (from name): reports whether one crossbar port's link is alive, as a single bit read from a per-port record [@0xc00000000006ef24].
  Reviewed: 2026-11-12

- `xbow_error_handler` in `6.5.22/unix.IP27` at 0xc00000000006f960
  **Inferred.** It calls xbow_widget_lookup [@0xc00000000006fac8] to find the widget named in the error record. It calls cmn_err [@0xc00000000006fbcc] to report the error fields.
  It passes the error on to xtalk_error_handler [@0xc00000000006fb64] when the widget lookup result differs from the stored widget id [@0xc00000000006fae0 taken].
  Role (from name): handles a crosstalk error for an Xbow widget, reporting it and passing it on.
  Reviewed: 2026-11-12

- `xbow_get_perf_counters` in `6.5.22/unix.IP27` at 0xc000000000070118
  **Inferred.** It calls hwgraph_fastinfo_get [@0xc000000000070120] and returns the result plus 64 bytes [@0xc000000000070130].
  Role (from facts): it returns a pointer 64 bytes into the record that hwgraph_fastinfo_get returns [@0xc000000000070120].
  Reviewed: 2026-11-12

- `xbow_get_llp_status` in `6.5.22/unix.IP27` at 0xc000000000070304
  **Inferred.** It calls hwgraph_fastinfo_get [@0xc00000000007030c] and returns the result plus 320 bytes (0x140) [@0xc00000000007031c].
  Role (from facts): it returns a pointer 0x140 bytes into the record that hwgraph_fastinfo_get returns [@0xc00000000007030c].
  Reviewed: 2026-11-12

- `xbow_disable_llp_monitor` in `6.5.22/unix.IP27` at 0xc000000000070438
  **Inferred.** It calls hwgraph_fastinfo_get [@0xc000000000070440]. It then loops eight times [@0xc000000000070468 taken], storing zero with two 8-byte stores on each pass [@0xc000000000070464] [@0xc00000000007046c]. It writes a zero 4-byte word at offset 0x1c4 of the result [@0xc000000000070474]. It returns 0x0 [@0xc000000000070478].
  Role (from facts): it clears eight pairs of 8-byte words and one 4-byte word in the record that hwgraph_fastinfo_get returns, and returns 0x0 [@0xc000000000070474].
  Reviewed: 2026-11-12

- `xbridge_attach` in `6.5.22/unix.IP27` at 0xc000000000071cf4
  **Inferred.** It calls pcibr_debug with a0 = 0x4 [call 1], then hub_device_flags_set with a1 = 0x1 [call 2], then pcibr_bridge_ptr_get with a1 = 0x0 [call 3]. It goes on through 79 calls in all [call 79]. They include hwgraph_path_add [call 5, @0xc0000000000725b0 taken], pciio_provider_register [call 6, @0xc0000000000725b0 taken], kmem_zalloc with a0 = 0xab8 [call 15, @0xc0000000000725b0 taken], pcibr_slot_info_init [call 62] and pciio_device_attach [call 68, @0xc0000000000725b0 taken].
  Role (from facts): a long bridge attach routine that, after the hub flag and bridge pointer setup, registers a graph vertex and PCI provider, allocates a 0xab8-byte record, and calls the slot initialisation and device attach routines [call 6, call 15, call 62, call 68].
  Reviewed: 2026-11-12

- `pciiox_vmc` in `6.5.22/unix.IP27` at 0xc000000000072734
  **Inferred.** It calls pcibr_hints_fix_rrbs [@0xc000000000072740] and then xtalk_piotrans_addr [call 2]. Call 2 has a1 = 0x0, a2 = 0x0, a3 = 0x1000000 and a4 = 0x0 [call 2].
  It stores a word at offset 0x284 of the result [@0xc000000000072774]. It stores a word at offset 0x28c of the result [@0xc000000000072780]. It returns the result of xtalk_piotrans_addr [@0xc00000000007277c].
  Role (from facts): it fixes the RRB hints and then asks the crosstalk layer for a 16 MiB PIO translation, patching two words of the result and returning it [call 2].
  Reviewed: 2026-11-12

- `pcibr_try_set_device` in `6.5.22/unix.IP27` at 0xc000000000072784
  **Inferred.** It takes a lock with mutex_spinlock [call 1]. It reads a 2-byte field at offset 0x72 of its first argument [@0xc000000000072798].
  Role (from name): it tries to set a device's configuration on a bridge slot under the bridge lock [call 1].
  Reviewed: 2026-11-12

- `pcibr_release_device` in `6.5.22/unix.IP27` at 0xc000000000072c28
  **Inferred.** It takes the lock with mutex_spinlock [call 1] and releases it with mutex_io_spinunlock [call 2].
  It writes the word at offset 0x144 of the device record when the third argument equals 0x1100000 [@0xc000000000072c8c] [@0xc000000000072c7c fall].
  Role (from name): releases a slot's per-device word state under the bridge lock [call 1].
  Reviewed: 2026-11-12

- `pcibr_asic_rev` in `6.5.22/unix.IP27` at 0xc0000000000736e4
  **Inferred.** It calls hwgraph_traverse [@0xc0000000000736f4] first.
  Role (from name): reads the ASIC revision of a bridge vertex from the hardware graph [call 1].
  Reviewed: 2026-11-12

- `pcibr_addr_pci_to_xio` in `6.5.22/unix.IP27` at 0xc000000000073788
  **Inferred.** It calls pcibr_info_get [@0xc0000000000737c0], then pciio_info_get [@0xc0000000000737cc] and pciio_info_mfast_get [call 3]. It takes the lock at offset 0x80 of the pciio_info_mfast_get result with mutex_spinlock [call 4].
  It branches on its third argument against 0x7 [@0xc000000000073830 taken] and against 0x1 [@0xc000000000073838 taken]. Later calls include pcireg_control_bit_set [call 9] and pcireg_device_set [call 11].
  Role (from name): maps a PCI bus address to a crosstalk address under the bridge lock, branching on its third argument and on some paths calling pcireg_device_set [call 11].
  Reviewed: 2026-11-12

- `pcibr_piomap_alloc` in `6.5.22/unix.IP27` at 0xc000000000074058
  **Inferred.** It calls pcibr_info_get [@0xc000000000074094] and then pciio_info_mfast_get [@0xc0000000000740a8].
  When the fifth argument (a4) is zero it branches to 0xc000000000074268 [@0xc0000000000740b4 taken].
  It calls pcibr_addr_pci_to_xio [@0xc0000000000740e0] only when a4 and a5 are both nonzero [@0xc0000000000740b4 fall] [@0xc0000000000740bc fall].
  Role (from name): allocates a PIO mapping for a PCI device, converting its address first.
  Reviewed: 2026-11-12

- `pcibr_piomap_addr` in `6.5.22/unix.IP27` at 0xc0000000000742f0
  **Inferred.** It calls xtalk_piomap_addr [@0xc000000000074308] with fields of its first argument and returns the result [@0xc000000000074314].
  Role (from facts): it forwards to xtalk_piomap_addr, taking its arguments from offsets 0x10, 0x28 and 0x30 of the map record [@0xc0000000000742f0].
  Reviewed: 2026-11-12

- `pcibr_flags_to_d64` in `6.5.22/unix.IP27` at 0xc000000000074728
  **Inferred.** It tests bits of its first argument with signed shifts [@0xc00000000007474c] and [@0xc0000000000747d4].
  Role (from name): converts PCI flag bits into a 64-bit direct-mapping attribute word.
  Reviewed: 2026-11-12

- `pcibr_addr_xio_to_pci` in `6.5.22/unix.IP27` at 0xc000000000074ed4
  **Inferred.** It makes no calls [@0xc000000000074ff4]. Its branches compare its second argument against 0x40000000 [@0xc000000000074f00 taken] and against 0x80000000 [@0xc000000000074f34 taken].
  Role (from name): converts a crosstalk address to a PCI address, using the range the address falls in.
  Reviewed: 2026-11-12

- `pcibr_reset` in `6.5.22/unix.IP27` at 0xc000000000075fcc
  **Inferred.** It calls pciio_hostinfo_get [call 1] and then pciio_info_mfast_get [call 2].
  It compares a value in s3 against the byte at offset 0x31 of the pciio_info_mfast_get result [@0xc000000000076028 taken] and against the byte at offset 0x33 [@0xc000000000076038 taken]. Either taken branch leads to a pcibr_debug call [call 12, @0xc000000000076038 taken] and a return of -0x1 [@0xc000000000076244].
  On the other side it takes the lock with mutex_spinlock [call 3, @0xc000000000076038 fall]. It clears control bits with pcireg_control_bit_clr [call 5, @0xc000000000076038 fall], waits with nano_delay [call 6, @0xc000000000076038 fall], sets them with pcireg_control_bit_set [call 7, @0xc000000000076038 fall] and waits again [call 8, @0xc000000000076038 fall].
  Role (from facts): it pulses a bridge control bit low and high with delays between, and reads and rewrites a function configuration register through pcibr_func_config_get [call 4] and pcibr_func_config_set [call 9].
  Reviewed: 2026-11-12

- `pcibr_endian_set` in `6.5.22/unix.IP27` at 0xc000000000076288
  **Inferred.** It calls pciio_hostinfo_get [call 1] and pciio_info_mfast_get [call 2], then takes the lock with mutex_spinlock [call 3].
  When the stored word at offset 0x138 differs from the second argument [@0xc0000000000762f4 taken], it calls pcireg_device_set with a4 = 0x180000 [call 6] and writes an 8-byte word at offset 0x138 of the slot record [@0xc000000000076364].
  It returns its third argument [@0xc000000000076340].
  Role (from name): sets the byte-order bits of a slot's device word.
  Reviewed: 2026-11-12

- `pcibr_priority_bits_set` in `6.5.22/unix.IP27` at 0xc000000000076370
  **Inferred.** It takes the lock with mutex_spinlock [call 1]. It reads the 8-byte word at offset 0x138 of the slot record [@0xc0000000000763d4].
  Role (from name): sets the priority bits of a slot's device word under the bridge lock [call 1].
  Reviewed: 2026-11-12

- `pcibr_device_flags_set` in `6.5.22/unix.IP27` at 0xc000000000076514
  **Inferred.** It calls pciio_hostinfo_get [call 1] and then pciio_info_mfast_get [call 2]. It returns 0x1 on one path [@0xc0000000000766ac].
  Role (from name): sets device flag bits in a slot's record under the bridge lock [call 3].
  Reviewed: 2026-11-12

- `pcibr_flags_to_ate` in `6.5.22/unix.IP27` at 0xc000000000077184
  **Inferred.** It tests bits of its first argument with signed shifts [@0xc0000000000771d4]. It makes no calls and returns [@0xc000000000077230].
  Role (from name): converts PCI flag bits into an ATE attribute word.
  Reviewed: 2026-11-12

- `do_pcibr_rrb_count_valid` in `6.5.22/unix.IP27` at 0xc00000000007759c
  **Inferred.** It calls pcireg_rrb_get with a0 = its first argument [call 1, @0xc000000000077754 taken]. It calls cmn_err with a0 = 0x4 [call 2, @0xc000000000077754 fall]. It also reaches that call when v0 is zero at the earlier test [@0xc000000000077618 taken]. It returns 0x0 on one path [@0xc0000000000776a4].
  Role (from name): checks the RRB count of a PCI bridge slot and logs through cmn_err [call 2].
  Reviewed: 2026-11-12
