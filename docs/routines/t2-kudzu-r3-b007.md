# Tier-2 routine readings, kudzu batch 7

Role statements for 25 routines of the 6.5.22 IP27 kernel image, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. The facts show what each routine touches and calls, not what the
fields and constants mean, so each statement leaves that open where the facts stop.
Addresses are in the binary named in each header.

- `xbow_enable_llp_monitor` in `6.5.22/unix.IP27` at 0xc000000000070480
  **Inferred.** Stores one 4-byte value at offset 0x1c4 of the record that hwgraph_fastinfo_get (0xc000000000296ee8) returns; the facts do not show where the stored value comes from. It then arms a timeout through timeout (0xc0000000001b46fc) whose callback is at 0xc000000000070324, with a 0x64 argument. The function returns 0 at 0xc0000000000704c4.

- `xbow_get_bridge_pointers` in `6.5.22/unix.IP27` at 0xc0000000000704cc
  **Inferred.** Walks the crossbar's child edges with hwgraph_edge_get_next (0xc000000000295ee8) and reads each widget record through xwidget_info_get (0xc00000000006dcb0). It compares the word at +0x10 of the widget record with the constants 0xd000, 0xd100, 0xc002 and 0xd002, compares the word at +0x18 with 0x24 and 0x36, and stores the pointers it finds through the output arguments a1 and a2. The only return, at 0xc000000000070564, is 0; the facts do not turn the constants into named widget types.

- `xbow_disable` in `6.5.22/unix.IP27` at 0xc000000000070740
  **Inferred.** Logs once through cmn_err (0xc0000000001f8090) at entry, then looks up a per-widget record with hwgraph_fastinfo_get (0xc000000000296ee8) and walks that record's edges with hwgraph_edge_get_next (0xc000000000295ee8), first with an extra argument of 0x2. It reads the 4-byte field at +0x8 of the record. The only return, at 0xc00000000007079c, yields -1; what the walk does to each edge is not visible in these facts.

- `xbow_enable` in `6.5.22/unix.IP27` at 0xc0000000000707c8
  **Inferred.** Logs once through cmn_err (0xc0000000001f8090) at entry, then walks the edges of its first argument with hwgraph_edge_get_next (0xc000000000295ee8), first with an extra argument of 0x2 and then without. The only return, at 0xc000000000070818, yields -1. Unlike xbow_disable it reads no record field and stores nothing the facts show, so the enable step is not visible here.

- `xbow_reset_link` in `6.5.22/unix.IP27` at 0xc000000000070844
  **Inferred.** Reads the pointer at +0x10 of the per-widget record from hwgraph_fastinfo_get (0xc000000000296ee8), resolves the widget through xwidget_info_get (0xc00000000006dcb0) and xwidget_info_id_get (0xc00000000006dd94), and reads the register at 0x9200000001030100 four times. It loops, delaying with us_delay (0xc000000000047274) and polling a word at 0xc0000000014c9de8; one delay passes 0x3e8 (1000) as its argument. It returns -1 or 0 depending on the poll outcome, and the facts do not show which bits the poll tests.

- `xbridge_init` in `6.5.22/unix.IP27` at 0xc000000000071ca0
  **Inferred.** Calls pcibr_debug (0xc0000000000767d0) once at entry with level 1, then registers two widget drivers through xwidget_driver_register (0xc00000000006df78): one for id 0xd002 with type 0x24, and one for id 0xc002 with type 0x36, both with the table at 0xc0000000003c6aa0. The return at 0xc000000000071cec is the second registration's result. The ids and types stay as constants here.

- `xbridge_attach` in `6.5.22/unix.IP27` at 0xc000000000071cf4
  **Inferred.** Attaches a PCI bridge under a crossbar widget. It allocates a 0xab8-byte record with kmem_zalloc (0xc000000000181ce0), clears it with memset (0xc00000000001f9c8), and stores it on the vertex through hwgraph_fastinfo_set (0xc000000000296eb8). It then reads the bridge register block through pcibr_bridge_ptr_get (0xc000000000076a54), registers the provider and character device, maps windows with pciio_device_win_map_new (0xc0000000000954c0), sets up interrupts and address-translation state, and ends by calling pciio_device_attach (0xc000000000095420). Branches on the ids 0xc002 and 0xd002 select variants; the facts do not give the meaning of those variants.

- `pciiox_init` in `6.5.22/unix.IP27` at 0xc000000000072714
  **Inferred.** Passes a constant table at 0xc0000000003c6c88, the address 0xc000000000072734 (the routine pciiox_vmc), and its own arguments a2 and a3 to nic_vmc_add (0xc00000000006bfb0), and returns that call's result. It is a thin wrapper; the facts do not show what nic_vmc_add does with the callback.

- `pciiox_vmc` in `6.5.22/unix.IP27` at 0xc000000000072734
  **Inferred.** Calls pcibr_hints_fix_rrbs (0xc0000000000797b8) with its arguments, then asks xtalk_piotrans_addr (0xc00000000006d7b0) for a translation with zero for two arguments, a length of 0x1000000 (16 MiB) and zero for the last. It stores two 4-byte words into the returned pointer at offsets 0x284 and 0x28c and returns that pointer. The stored values are not shown by the facts.

- `pcibr_try_set_device` in `6.5.22/unix.IP27` at 0xc000000000072784
  **Inferred.** Works on a 128-byte slot entry inside its first argument, at a0 + (a1 << 7), with 4-byte fields at +0x144, +0x148 and +0x14c and an 8-byte field at +0x138. It takes the lock at a0+128 with mutex_spinlock (0xc000000000287214), calls pcireg_device_set (0xc000000000088124), and releases with mutex_io_spinunlock (0xc000000000287120). It tests many single bits of its third argument (0x10, 0x20, 0x1000, 0x2000 and others) and compares its fourth argument with 0x1100000, 0x8f8000 and 0x898000. The name suggests it tries to apply a device setting, but the facts do not decode which setting.

- `pcibr_release_device` in `6.5.22/unix.IP27` at 0xc000000000072c28
  **Inferred.** Takes the slot entry at a0 + (a1 << 7) under the lock at a0+128 (mutex_spinlock, 0xc000000000287214), loads and stores back the 4-byte fields at +0x144, +0x148 and +0x14c, and unlocks with mutex_io_spinunlock (0xc000000000287120). Its third argument is compared with 0x1100000, 0x8f8000 and 0x898000 to choose the path. The value written back is not shown by the facts, and the return is the unlock call's result.

- `pcibr_device_write_gather_flush` in `6.5.22/unix.IP27` at 0xc000000000072cd8
  **Inferred.** Passes its four arguments unchanged to pcireg_wrb_flush_get (0xc0000000000887b0) and returns that call's result. It is a forwarding stub. The name suggests a write-gather buffer flush on a PCI device, and the callee is where that work happens.

- `pcibr_open` in `6.5.22/unix.IP27` at 0xc000000000072cf4
  **Inferred.** Checks a capability with cap_able_cred (0xc0000000002881b0), passing the constant 0x2000000000 and its own third and fourth arguments. It returns 0 or 1 from the return at 0xc000000000072d18, following the branch at 0xc000000000072d0c on the check result. The facts show the check but not the open work it guards.

- `pcibr_map` in `6.5.22/unix.IP27` at 0xc000000000072d30
  **Inferred.** Looks up the device's connection point with hwgraph_connectpt_get (0xc00000000029771c) and its record with hwgraph_fastinfo_get (0xc000000000296ee8), then drops the vertex reference with hwgraph_vertex_unref (0xc000000000295c58). It maps a physical range with v_mapphys (0xc0000000001ebbdc), taking the start as a2 plus the 8-byte field at +0x8 of the record and the constant -0x4000 as the last argument. On the path that the branches at 0xc000000000072da8 and 0xc000000000072db0 select, it sets bit 0x80000000 through pcireg_control_bit_set (0xc000000000080e18); the return is v_mapphys's result.

- `pcibr_unmap` in `6.5.22/unix.IP27` at 0xc000000000072dfc
  **Inferred.** Finds the device record the same way as pcibr_map, through hwgraph_connectpt_get (0xc00000000029771c) and hwgraph_fastinfo_get (0xc000000000296ee8), and drops the vertex reference with hwgraph_vertex_unref (0xc000000000295c58). It reads the control register with pcireg_control_get (0xc00000000008097c) and clears bit 0x80000000 through pcireg_control_bit_clr (0xc000000000080c6c) when that bit is set. The return at 0xc000000000072e40 is 0.

- `pcibr_detach` in `6.5.22/unix.IP27` at 0xc000000000073428
  **Inferred.** Reads the device record from hwgraph_fastinfo_get (0xc000000000296ee8), disables interrupts with pcireg_intr_enable_set (0xc0000000000851bc) under the lock at +0x80, and calls pcibr_slot_detach (0xc00000000007fe48) for each slot in a loop bounded by the bytes at +0x38 and +0x39. It then removes the device info, frees the 0xab8-byte record with kmem_free (0xc000000000181d4c), and removes the hwgraph labels and edges. The record size matches the one xbridge_attach allocates, so this reads as its counterpart. The return is 0 or 1.

- `pcibr_directmap_init` in `6.5.22/unix.IP27` at 0xc0000000000735b8
  **Inferred.** Reads fields of its first argument (words at +0x0 and +0x4, bytes at +0x30 and +0x38), looks up an admin path with device_admin_info_get (0xc0000000001761c4), and asks xtalk_dmatrans_addr (0xc00000000006d980) for a translation with length 0x4000. It programs the direct-map registers through pcireg_dirmap_diroff_set (0xc000000000083348), pcireg_dirmap_wid_set (0xc0000000000831ac) and one of the add512 accessors (pcireg_dirmap_add512_clr or pcireg_dirmap_add512_set) with 0x20000000. If hwgraph_path_to_vertex (0xc000000000296748) returns -1, the path logs the vertex name through cmn_err (0xc0000000001f8090). The return is one of the add512 results.

- `pcibr_asic_rev` in `6.5.22/unix.IP27` at 0xc0000000000736e4
  **Inferred.** Traverses a fixed hardware-graph path with hwgraph_traverse (0xc0000000002965ec), reads a labeled value from the vertex with hwgraph_info_get_LBL (0xc000000000296160) using the label at 0xc0000000014104a0, and drops the vertex with hwgraph_vertex_unref (0xc000000000295c58). It returns that value, or -1 when the traversal fails. The name suggests an ASIC revision, but the path and label are constants the facts do not decode.

- `pcibr_write_gather_flush` in `6.5.22/unix.IP27` at 0xc00000000007374c
  **Inferred.** Gets the host info with pciio_hostinfo_get (0xc000000000094dbc) and the device record with pciio_info_mfast_get (0xc000000000094ec4). It loads a byte at +0x21c of the host info, though no branch on that byte is listed, and calls pcibr_device_write_gather_flush (0xc000000000072cd8) with the record and the byte at +540 of the host info. The return is 0.

- `pcibr_addr_pci_to_xio` in `6.5.22/unix.IP27` at 0xc000000000073788
  **Inferred.** Translates a PCI-side address into a crossbar address for a device. It reads per-slot tables indexed by a1 (0x80-byte entries, with fields at +0x100, +0x118, +0x144 and +0x148), finds the device through pcibr_info_get (0xc00000000007306c) and pciio_info_mfast_get (0xc000000000094ec4), and tests the range against the constants 0x40000000, 0x7fffffff, 0x80000000, 0xbfffffff, 0xc0000000 and 0x100000000. It uses sprintf (0xc0000000001f8e8c) for messages and pcireg_control_bit_set (0xc000000000080e18) with 0x400000 or 0x800000. The return value is not resolved by the facts.

- `pcibr_piomap_alloc` in `6.5.22/unix.IP27` at 0xc000000000074058
  **Inferred.** Gets the bridge record with pcibr_info_get (0xc00000000007306c) and allocates a 0x60-byte map record with kmem_zalloc (0xc000000000181ce0). It converts the address with pcibr_addr_pci_to_xio (0xc000000000073788) and allocates the crossbar side with xtalk_piomap_alloc (0xc00000000006d740), holding the lock at the device record's +0x80 around the work. Two pcibr_debug calls (0xc0000000000767d0) log with flag 0x8000000 on failure paths. The layout of the map record beyond these stores is not shown.

- `pcibr_piomap_addr` in `6.5.22/unix.IP27` at 0xc0000000000742f0
  **Inferred.** Loads three 8-byte fields from its map argument, at +0x28, +0x10 and +0x30, and passes them with its own third argument to xtalk_piomap_addr (0xc00000000006d778). It returns that call's result. It is a forwarding wrapper that reads the crossbar mapping out of the map record.

- `pcibr_piotrans_addr` in `6.5.22/unix.IP27` at 0xc000000000074338
  **Inferred.** Gets the host info and device record through pciio_hostinfo_get (0xc000000000094dbc) and pciio_info_mfast_get (0xc000000000094ec4), converts the address with pcibr_addr_pci_to_xio (0xc000000000073788), and asks xtalk_piotrans_addr (0xc00000000006d7b0) for the crossbar translation. It returns that translation, or 0 on the path where the conversion returns -1, and logs with pcibr_debug (0xc0000000000767d0) flag 0x10000000 on that failure path.

- `pcibr_flags_to_d64` in `6.5.22/unix.IP27` at 0xc000000000074728
  **Inferred.** Converts a flags word (its first argument) and a device record (its second) into a 64-bit DMA attribute value. It tests many single bits of the flags, and compares the 2-byte fields at +0x72 and +0x74 of the device record with the constants 1 and 2. A bad combination calls assfail (0xc00000000003a2b4) with line constant 0x7d0 (2000). The name suggests a 64-bit DMA conversion; the facts do not show the value it builds.

- `pcibr_addr_xio_to_pci` in `6.5.22/unix.IP27` at 0xc000000000074ed4
  **Inferred.** The inverse of pcibr_addr_pci_to_xio: it takes a device record and a crossbar range (base a1, length a2), reads the 2-byte field at +0x72 and the bytes at +0x38 and +0x39, and compares the range end with 0x40000000, 0x80000000, 0xbfffffff, 0x7fffffff, 0xc0000000, 0x100000000 and 0x13fffffff. Its returns are the base minus 0x40000000 or minus 0x80000000 on some paths, and an unresolved value on others.
