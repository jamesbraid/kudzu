# Tier-2 routine readings, kudzu batch 9

Role statements for routines 226 to 250 of the tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Where the facts stop short of the meaning, the statement says so.
Addresses are in the binary named in each header.

- `pciio_dma_enabled` in `6.5.22/unix.IP27` at 0xc000000000095730
  **Inferred.** Forwards its four entry arguments unchanged to pcibr_dma_enabled at 0xc0000000000767a4 (jal at 0xc000000000095738) and returns that call's result (return at 0xc000000000095744). The routine itself has no branch or memory access, so what the value means is whatever pcibr_dma_enabled computes.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_ei_intr` in `6.5.22/unix.IP27` at 0xc000000000095b8c
  **Inferred.** Role (from name): handles an IOC3 external interrupt for the record at arg0. It calls frs_handle_eintr at 0xc0000000003c4028 (call at 0xc000000000095bb0) only when the word tested by the beq at 0xc000000000095ba8 is non-zero; the ioc3_write_ireg call at 0xc000000000095bc0 shares that condition. It calls ioc3_write_ireg at 0xc00000000009688c with a2 = 0 three times in total (calls at 0xc000000000095bc0, 0xc000000000095bec and 0xc000000000095cb0), not all on the same path. When the field at +0x2c is non-negative (bltz at 0xc000000000095bd4) it calls uli_callup at 0xc00000000020be3c with that field. On the path where the bltz is taken it takes mutex_spinlock at 0xc000000000287214 on arg0+20 and releases it with mutex_spinunlock at 0xc000000000287260; in between, a loop with ll/sc at 0xc000000000095c38 can call sigtopid at 0xc0000000001bac4c with the constant 0x12, nested_spinlock, sv_signal at 0xc00000000018f4fc and nested_spinunlock. Which interrupt cause selects which path, and what the list walked by that loop holds, are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_mlreset` in `6.5.22/unix.IP27` at 0xc0000000000967e0
  **Inferred.** Forwards its four entry arguments unchanged to ioc3_sio_mlreset at 0xc0000000001797b0 (jal at 0xc0000000000967ec) and returns that call's result. The body adds no test or memory access of its own.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_poll` in `6.5.22/unix.IP27` at 0xc000000000096800
  **Inferred.** Role (from name): polls the IOC3 interrupt state for the record at arg0. It calls ioc3_handle_intr at 0xc0000000000973c0 with a1 = 0 first (jal at 0xc000000000096814), then takes the lock at +2876 with mutex_spinlock_spl at 0xc0000000002871c0 and loads the byte at +2884 (lbu at 0xc00000000009682c). The test of bit 0x2 at 0xc000000000096834 splits two paths. When the bit is set, it calls mutex_spinunlock at 0xc000000000287260 (call at 0xc000000000096840), then prtimeout_nothrd at 0xc0000000001b48b4 (jal at 0xc000000000096854) and returns at 0xc000000000096868. When the bit is clear, control goes to 0xc000000000096870, after which the facts show a mutex_spinunlock call at 0xc00000000009687c and a store to the byte at +0xb44 (sb at 0xc000000000096880). The meaning of bit 0x2 and of the timeout re-arm is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_write_ireg` in `6.5.22/unix.IP27` at 0xc00000000009688c
  **Inferred.** Role (from name): writes a value into the IOC3 state of the record at arg0, under the lock at +2876 (mutex_spinlock_spl at 0xc0000000002871c0 with a1 = 0xc00000000003b0ac). It loads the doubleword at +0x18 and the word at +0xb40 and branches on a2 (beq at 0xc0000000000968c8 and 0xc0000000000968d0: a2 = 0 and a2 = 1 take separate paths). It stores to +0xb40 of the record, stores words at +0x20 and +0x24 (the base register of those stores is not shown in the facts) and stores the byte at +0xb44. It calls mutex_spinunlock at 0xc000000000287260 (call at 0xc0000000000968e8) on the path where a2 is neither 0 nor 1; the facts show no unlock call on the other paths. It calls prtimeout_nothrd at 0xc000000000096a14 with a6 = (byte at +2884 | 0x2). The register offset written is not in these facts, so the target register is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_pending_intrs` in `6.5.22/unix.IP27` at 0xc000000000096a24
  **Inferred.** Role (from name): reads the pending-interrupt state of the record at arg0 under the same lock as ioc3_write_ireg (mutex_spinlock_spl at 0xc0000000002871c0, mutex_spinunlock at 0xc000000000287260). Inside the lock it loads the word at +0xb40 and a word at +0x1c; the base register of the +0x1c load is not shown, though the doubleword at +0x18 is loaded earlier in the routine. The value it returns, v0 at 0xc000000000096a78, is not traced by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_init` in `6.5.22/unix.IP27` at 0xc000000000096a80
  **Inferred.** Role (from name): registers the IOC3 PCI driver and adds network and serial sub-drivers. It calls pciio_driver_register at 0xc000000000095030 (call at 0xc000000000096a98) with a0 = 0x10a9, a1 = 0x3, a2 = the driver record at 0xc0000000003c8180 and a3 = 0x1, then calls nic_vmc_add at 0xc00000000006bfb0 three times. The first call passes a0 = 0xc0000000003c8150 and a1 = 0xc000000000096ad8 (pci_ioc3_enet_vmc); the second and third pass a1 = 0xc000000000096af4 (pci_ioc3_sio_vmc) with a0 = 0xc0000000003c8160 and 0xc0000000003c8170.
  Reviewed: 2026-11-12 (sonnet)

- `pci_ioc3_enet_vmc` in `6.5.22/unix.IP27` at 0xc000000000096ad8
  **Inferred.** Role (from name): enables one IOC3 sub-device, the Ethernet one. It calls ioc3_subdev_enables at 0xc000000000097ae8 (jal at 0xc000000000096ae0) with a1 = 0x9, passes its entry arguments a0, a2 and a3 through, and returns that call's result. The constant 0x9 is only passed as the second argument here; the facts do not say whether it is an index or a bit mask.
  Reviewed: 2026-11-12 (sonnet)

- `pci_ioc3_sio_vmc` in `6.5.22/unix.IP27` at 0xc000000000096af4
  **Inferred.** Role (from name): enables the serial sub-device. It calls ioc3_subdev_enables at 0xc000000000097ae8 (jal at 0xc000000000096afc) with a1 = 0x30, passes a0, a2 and a3 through, and returns its result. The constant is passed through as-is and its meaning is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_attach` in `6.5.22/unix.IP27` at 0xc000000000096b10
  **Inferred.** Attaches one IOC3 device. It first maps two windows with pciio_pio_addr at 0xc00000000009429c (a4 = 0x48 and a4 = 0x20180), looks up hwgraph vertices with hwgraph_traverse at 0xc0000000002965ec, and adds a path and edges with hwgraph_path_add at 0xc00000000029646c and hwgraph_edge_add at 0xc000000000295c78. It then allocates a 0xb48-byte record with kmem_zalloc at 0xc000000000181ce0, initializes the lock at +2876 with spinlock_init at 0xc000000000286fe8, and fills fields (sb at +0xb45, sd at +0x10 and +0x18). It registers ioc3_error_handler at 0xc00000000009762c through pciio_error_register at 0xc0000000000956c8 (two calls). Only on paths that pass branches such as the beq at 0xc000000000096d9c on the result of ioc3_subdev_enabled does it connect an interrupt through pciio_intr_alloc at 0xc000000000094690 and pciio_intr_connect at 0xc000000000094730, with ioc3_intr at 0xc0000000000973a4 as the handler, followed by calls to ioc3_serial_attach at 0xc00000000017a208 and pckm_attach at 0xc000000000097c60. cmn_err at 0xc0000000001f8090 is called from several sites, and the routine returns -1 or 0 at 0xc000000000096e78; which conditions pick those returns, whether they mean failure, and the device-description values are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_intr_connect` in `6.5.22/unix.IP27` at 0xc000000000096fc8
  **Inferred.** Role (from name): connects an interrupt for an IOC3 device. It looks up the device vertex with hwgraph_traverse at 0xc0000000002965ec (constant 0xc0000000003c8328) and its per-device record with hwgraph_fastinfo_get at 0xc000000000296ee8. It updates three words of that record with ll/sc sequences: +0xb38 (ll at 0xc000000000097040), +0xb34 (ll at 0xc0000000000970dc) and +0xb30 (ll at 0xc0000000000971a4). On one path it builds a device description with device_desc_dup at 0xc000000000175dbc, formats a string with sprintf at 0xc0000000001f8e8c (calls at 0xc000000000097144 and 0xc0000000000971cc) and calls xthread_setup at 0xc0000000001c75c8 with a3 = 0xc000000000097968 (ioc3_intrd_setup). cmn_err at 0xc0000000001f8090 and panic at 0xc0000000001f9228 are also called; the return at 0xc000000000097194 is one of ret(xthread_setup) and ret(cmn_err). Which condition picks each path is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_intr_disconnect` in `6.5.22/unix.IP27` at 0xc000000000097240
  **Inferred.** Role (from name): undoes an interrupt connection made by ioc3_intr_connect. It finds the device with hwgraph_traverse at 0xc0000000002965ec and its record with hwgraph_fastinfo_get at 0xc000000000296ee8, then loops over table entries whose count is the word at +0xb30 (blez at 0xc000000000097298 on the word at +2864 branches to 0xc000000000097348 when it is not positive). Inside the loop it compares three loaded values with a2, a3 and a4 and tests a fourth value ANDed with a1, and it runs ll/sc sequences, including on the words at +0xb34 (ll at 0xc00000000009731c) and +0xb38 (ll at 0xc000000000097370). cmn_err at 0xc0000000001f8090 has one call site, at level 1 (call at 0xc000000000097394); which condition reaches it is not settled by these facts, nor is the layout of the table entries.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_intr` in `6.5.22/unix.IP27` at 0xc0000000000973a4
  **Inferred.** Interrupt entry point that forwards to ioc3_handle_intr at 0xc0000000000973c0 with a1 = 1 (jal at 0xc0000000000973ac). The other arguments pass through unchanged and the call's result is returned. ioc3_poll calls the same handler with a1 = 0, so the flag separates two callers, but what the flag changes inside the handler is not shown here.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_handle_intr` in `6.5.22/unix.IP27` at 0xc0000000000973c0
  **Inferred.** Role (from name): services the pending IOC3 interrupts of the device at arg0. It calls ioc3_pending_intrs at 0xc000000000096a24 (jal at 0xc000000000097404 and 0xc000000000097460), ioc3_write_ireg at 0xc00000000009688c with a2 = 0x1 (jal at 0xc0000000000974ac) and vsema at 0xc00000000018ac88 (jal at 0xc0000000000974b4). The branch at 0xc00000000009742c goes to 0xc0000000000974d8 when the count in s3 is below s5. Two indirect calls at 0xc000000000097518 and 0xc000000000097530 have callees the facts do not name, and cmn_err at 0xc0000000001f8090 is called at 0xc000000000097584 at level 1. A word at +0xd0 is updated with ll/sc at 0xc000000000097440; the base register is not shown. The meaning of the pending masks and of the loop count is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_is_console` in `6.5.22/unix.IP27` at 0xc000000000097594
  **Inferred.** Role (from name): tests whether the device at arg0 is the console device. It looks up the vertex with hwgraph_traverse at 0xc0000000002965ec (constant 0xc0000000003c8328); when that call returns zero it calls hwgraph_fastinfo_get at 0xc000000000296ee8 (call at 0xc0000000000975d4), and the return at 0xc0000000000975cc is one of 0 and an untraced value. A non-zero doubleword at 0xc0000000014d15a0 selects a different path (bne at 0xc00000000009759c). Which value is the console flag is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_is_io7` in `6.5.22/unix.IP27` at 0xc0000000000975ec
  **Inferred.** Role (from name): checks a flag for the IO7 bridge on the device at arg0. It looks up the vertex with hwgraph_traverse at 0xc0000000002965ec (constant 0xc0000000003c8328); when that call returns zero it calls hwgraph_fastinfo_get at 0xc000000000296ee8 (call at 0xc00000000009761c) and loads the byte at +0xb45 of the record it returns (lbu at 0xc000000000097628). It returns that byte, or 0 when hwgraph_traverse returns non-zero. The bit meaning of the byte is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_error_handler` in `6.5.22/unix.IP27` at 0xc00000000009762c
  **Inferred.** Error handler for an IOC3 device, registered by ioc3_attach through pciio_error_register at 0xc0000000000956c8 (a1 = 0xc00000000009762c in the calls at 0xc000000000096c94 and 0xc000000000096ca4). It loads the record pointer at +0x10 of arg0 and, through it, the words at +0x4, +0x40 and +0x44, and it stores a word at +0x40 (sw at 0xc0000000000977d0). Many further loads at +0x0 lie between these; their base registers are not rendered in the facts. A mask test on the word at +0x4 (beq at 0xc00000000009766c) selects one path, and an unsigned compare of a3 against 0xd at 0xc000000000097768 skips the loads that follow it when a3 is 0xd or more. cmn_err at 0xc0000000001f8090 is called at level 5 at 0xc00000000009771c and 0xc0000000000977bc. A return of 0 is at 0xc000000000097748, reached when bit 0x1 of the word at +0x40 is clear (bne at 0xc000000000097730); which return the other path takes is not shown. The bit-to-error mapping is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_intrd` in `6.5.22/unix.IP27` at 0xc000000000097920
  **Inferred.** Role (from name): thread body for the IOC3 interrupt service thread, set by xthread_set_func in ioc3_intrd_setup (a1 = 0xc000000000097920 there). It loads values at +0x0, +0x20, +0x18, +0x8, +0x1c and +0x10; only the fields at +0x0 and +0x10 of arg0 are confirmed, because they appear as a2 and a0 of the next call. It makes an indirect call at 0xc000000000097948 whose callee the facts do not name, passing the doubleword at +0x10 of arg0 as a0 and the word at +0x0 as a2, then calls ipsema at 0xc00000000018abb4 (jal at 0xc000000000097950) with a0 = arg0+48 and returns its result.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_intrd_setup` in `6.5.22/unix.IP27` at 0xc000000000097968
  **Inferred.** Role (from name): sets up the IOC3 interrupt service thread. It loads the halfword at +0x2a of the record at [arg0+32] and passes it as a0 to setmustrun at 0xc0000000001959f4 (call at 0xc00000000009797c). It passes ioc3_intrd at 0xc000000000097920 as a1 to xthread_set_func at 0xc0000000001c771c with a2 = arg0. It updates the word at +0x28 with an ll/sc sequence (ll at 0xc00000000009799c, sc at 0xc0000000000979a4), storing that word ORed with 0x400, then calls ipsema at 0xc00000000018abb4 (jal at 0xc0000000000979b0) with a0 = arg0+48, a1 = the word at +0x28, a2 = arg0+40 and a3 = 0x400. The meaning of the 0x400 flag is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_console_vhdl_get` in `6.5.22/unix.IP27` at 0xc0000000000979c8
  **Inferred.** Role (from name): returns the console vertex handle kept in the global at 0xc0000000014d15a0. It tests a2, which holds the doubleword at that global, against zero (beq at 0xc0000000000979cc, which goes to the return at 0xc0000000000979d8 when it is zero); otherwise it loads the word at the address that doubleword holds (lw at 0xc0000000000979d4). The return at 0xc0000000000979d8 gives -1 or that word; the facts do not say which path gives -1.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_subdev_ptr` in `6.5.22/unix.IP27` at 0xc0000000000979e0
  **Inferred.** Role (from name): returns the sub-device state record for the device at arg0, creating it on demand. It finds the vertex with hwgraph_traverse at 0xc0000000002965ec (constant 0xc0000000003c8318) and calls hwgraph_info_get_LBL at 0xc000000000296160 with the label at 0xc0000000003c8668. When a1 is non-zero and that call returns non-zero (beq at 0xc000000000097a1c and 0xc000000000097a24), it allocates 8 bytes with kmem_zalloc at 0xc000000000181ce0 and attaches the block with hwgraph_info_add_LBL at 0xc0000000002960bc. It then calls hwgraph_info_get_LBL again, and when the value it reads back differs from the block it allocated (bne at 0xc000000000097a70), it frees the block with kmem_free at 0xc000000000181d4c. The return value at 0xc000000000097a84 is not traced by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_subdev_enabled` in `6.5.22/unix.IP27` at 0xc000000000097aa0
  **Inferred.** Role (from name): reads the enable state of a sub-device on the device at arg0. It calls ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 0x0, which per that routine's own facts does not allocate a record; when the pointer is non-null it loads the doubleword at +0 of that record (ld at 0xc000000000097ac8). The return value at 0xc000000000097ae0 is open in these facts, and which bit of the loaded doubleword is tested is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_subdev_enables` in `6.5.22/unix.IP27` at 0xc000000000097ae8
  **Inferred.** Role (from name): sets the enable state of a sub-device on the device at arg0. It calls ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 0x1, which allocates a record when none exists. When the pointer is non-null it stores a doubleword at +0 of that record (sd at 0xc000000000097b08); the routine returns the pointer (return at 0xc000000000097b0c). The stored value is not traced in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_subdev_enable` in `6.5.22/unix.IP27` at 0xc000000000097b14
  **Inferred.** Role (from name): sets an enable bit for a sub-device. It calls ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 0x1, and when the record is non-null it runs a 64-bit load-linked and store-conditional loop on the doubleword at +0 (lld at 0xc000000000097b3c, scd at 0xc000000000097b44). The retry test at 0xc000000000097b48 uses a0, whose value the facts do not trace. The routine returns the record pointer (return at 0xc000000000097b50). Which bit is changed, and in which direction, is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `ioc3_subdev_disable` in `6.5.22/unix.IP27` at 0xc000000000097b58
  **Inferred.** Role (from name): clears an enable bit for a sub-device, the counterpart of ioc3_subdev_enable. It calls ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 0x1, and when the record is non-null it runs a 64-bit load-linked and store-conditional loop on the doubleword at +0 (lld at 0xc000000000097b84, scd at 0xc000000000097b8c). The retry test at 0xc000000000097b90 ANDs the loaded doubleword with the register v1, whose value is not shown, and compares the result with zero. The routine returns the record pointer (return at 0xc000000000097b98). Which bit is changed is not shown.
  Reviewed: 2026-11-12 (sonnet)
