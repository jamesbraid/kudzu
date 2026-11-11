# Tier-2 routine readings, kudzu batch 9

Role statements for routines 226 to 250 of the tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Where the facts stop short of the meaning, the statement says so.
Addresses are in the binary named in each header.

- `pciio_dma_enabled` in `6.5.22/unix.IP27` at 0xc000000000095730
  **Inferred.** Forwards its four entry arguments unchanged to pcibr_dma_enabled at 0xc0000000000767a4 (jal at 0xc000000000095738) and returns that call's result (return at 0xc000000000095744). The routine itself has no branch or memory access, so what the value means is whatever pcibr_dma_enabled computes.

- `ioc3_ei_intr` in `6.5.22/unix.IP27` at 0xc000000000095b8c
  **Inferred.** Handles an IOC3 external interrupt for the record at arg0. It calls frs_handle_eintr at 0xc0000000003c4028 first, writes the interrupt register through ioc3_write_ireg at 0xc00000000009688c three times with a2 = 0, and calls uli_callup at 0xc00000000020be3c with the field at +0x2c when that field is non-negative (bltz at 0xc000000000095bd4). Around the field at +0x14 it takes mutex_spinlock at 0xc000000000287214 and releases it with mutex_spinunlock at 0xc000000000287260, and between them it calls sigtopid at 0xc0000000001bac4c with constant 0x12 and signals through sv_signal at 0xc00000000018f4fc. Which interrupt cause selects which path, and what the list walked by the ll/sc loop at 0xc000000000095c38 holds, are not settled by these facts.

- `ioc3_mlreset` in `6.5.22/unix.IP27` at 0xc0000000000967e0
  **Inferred.** Forwards its four entry arguments unchanged to ioc3_sio_mlreset at 0xc0000000001797b0 (jal at 0xc0000000000967ec) and returns that call's result. The body adds no test or memory access of its own.

- `ioc3_poll` in `6.5.22/unix.IP27` at 0xc000000000096800
  **Inferred.** Polls the IOC3 interrupt state for the record at arg0. It calls ioc3_handle_intr at 0xc0000000000973c0 with a1 = 0 first (jal at 0xc000000000096814), then takes the lock at +2876 with mutex_spinlock_spl at 0xc0000000002871c0 and reads the byte at +2884. The bit-0x2 test at 0xc000000000096834 sends control either to the mutex_spinunlock call at 0xc00000000009687c or to prtimeout_nothrd at 0xc0000000001b48b4 (jal at 0xc000000000096854), and the routine stores to the byte at +0xb44 (sb at 0xc000000000096880). The meaning of bit 0x2 and of the timeout re-arm is not settled by these facts.

- `ioc3_write_ireg` in `6.5.22/unix.IP27` at 0xc00000000009688c
  **Inferred.** Writes a value into the IOC3 state of the record at arg0 under the lock at +2876 (mutex_spinlock_spl at 0xc0000000002871c0 with a1 = 0xc00000000003b0ac, mutex_spinunlock at 0xc000000000287260). It loads the doubleword at +0x18 and the word at +0xb40, branches on a2 (beq at 0xc0000000000968c8 and 0xc0000000000968d0), stores into the record at +0xb40 and at offsets 0x20 and 0x24 of the structure pointed to by +0x18, and updates the byte at +0xb44. It calls prtimeout_nothrd at 0xc000000000096a14 with a6 = (byte at +2884 | 0x2). The register offset written is not in these facts, so the target register is not identified.

- `ioc3_pending_intrs` in `6.5.22/unix.IP27` at 0xc000000000096a24
  **Inferred.** Reads the pending-interrupt state of the record at arg0 under the same lock as ioc3_write_ireg (mutex_spinlock_spl at 0xc0000000002871c0, mutex_spinunlock at 0xc000000000287260). Inside the lock it loads the word at +0xb40 and the word at +0x1c of the structure at +0x18. The value it returns, v0 at 0xc000000000096a78, is not traced by these facts.

- `ioc3_init` in `6.5.22/unix.IP27` at 0xc000000000096a80
  **Inferred.** Registers the IOC3 PCI driver and adds its network and serial sub-drivers. It calls pciio_driver_register at 0xc000000000095030 with the constant 0x10a9, 0x3 and the driver record at 0xc0000000003c8180, then calls nic_vmc_add at 0xc00000000006bfb0 three times. The first call passes pci_ioc3_enet_vmc at 0xc000000000096ad8 and the records at 0xc0000000003c8150; the second and third pass pci_ioc3_sio_vmc at 0xc000000000096af4 with the records at 0xc0000000003c8160 and 0xc0000000003c8170.

- `pci_ioc3_enet_vmc` in `6.5.22/unix.IP27` at 0xc000000000096ad8
  **Inferred.** Enables one IOC3 sub-device through ioc3_subdev_enables at 0xc000000000097ae8, passing the constant 0x9 as the sub-device index with its entry arguments a0, a2 and a3 (jal at 0xc000000000096ae0), and returns that result. The name points to the Ethernet function, but the constant is only used as an index here.

- `pci_ioc3_sio_vmc` in `6.5.22/unix.IP27` at 0xc000000000096af4
  **Inferred.** Enables the serial sub-device set through ioc3_subdev_enables at 0xc000000000097ae8, with the constant 0x30 as the second argument (jal at 0xc000000000096afc), and returns its result. The name points to the serial function. The constant is passed through as-is.

- `ioc3_attach` in `6.5.22/unix.IP27` at 0xc000000000096b10
  **Inferred.** Attaches one IOC3 device. It allocates a 0xb48-byte record with kmem_zalloc at 0xc000000000181ce0, initializes the lock at +2876 with spinlock_init at 0xc000000000286fe8, and fills fields in the record (sb at +0xb45, sd at +0x10 and +0x18). It maps the PIO windows with pciio_pio_addr at 0xc00000000009429c (constants 0x48 and 0x20180), looks up hwgraph vertices with hwgraph_traverse at 0xc0000000002965ec, adds paths and edges with hwgraph_path_add at 0xc00000000029646c and hwgraph_edge_add at 0xc000000000295c78, and registers the error handler ioc3_error_handler at 0xc00000000009762c through pciio_error_register at 0xc0000000000956c8. It connects an interrupt through pciio_intr_alloc at 0xc000000000094690 and pciio_intr_connect at 0xc000000000094730, with ioc3_intr at 0xc0000000000973a4 as the handler, and calls ioc3_serial_attach at 0xc00000000017a208 and pckm_attach at 0xc000000000097c60. On failure it reports through cmn_err at 0xc0000000001f8090 and returns -1 or 0. The conditions that choose between those returns, and the device-description values, are not all settled by these facts.

- `ioc3_intr_connect` in `6.5.22/unix.IP27` at 0xc000000000096fc8
  **Inferred.** Connects an interrupt for an IOC3 device. It looks up the device vertex with hwgraph_traverse at 0xc0000000002965ec and its per-device record with hwgraph_fastinfo_get at 0xc000000000296ee8. It then updates three words of that record at offsets 0xb30, 0xb34 and 0xb38 with ll/sc sequences (ll at 0xc000000000097040, 0xc0000000000970dc and 0xc0000000000971a4). It builds a device description with device_desc_dup at 0xc000000000175dbc and names a service thread with sprintf at 0xc0000000001f8e8c; xthread_setup at 0xc0000000001c75c8 starts it with ioc3_intrd_setup at 0xc000000000097968. cmn_err at 0xc0000000001f8090 and panic at 0xc0000000001f9228 are the failure paths. Which condition picks each path is not settled by these facts.

- `ioc3_intr_disconnect` in `6.5.22/unix.IP27` at 0xc000000000097240
  **Inferred.** Undoes an interrupt connection made by ioc3_intr_connect. It finds the device with hwgraph_traverse at 0xc0000000002965ec and its record with hwgraph_fastinfo_get at 0xc000000000296ee8, then scans a table whose count is at +0xb30 (blez at 0xc000000000097298 on the word at +2864). Matching slots are updated with ll/sc sequences on the words at +0xb34 and +0xb38. A missing device is reported with cmn_err at 0xc000000000097394 at level 1. The layout of the table entries is not settled by these facts.

- `ioc3_intr` in `6.5.22/unix.IP27` at 0xc0000000000973a4
  **Inferred.** Interrupt entry point that forwards to ioc3_handle_intr at 0xc0000000000973c0 with a1 = 1 (jal at 0xc0000000000973ac). The other arguments pass through unchanged and the call's result is returned. ioc3_poll calls the same handler with a1 = 0, so the flag separates two callers, but what the flag changes inside the handler is not shown here.

- `ioc3_handle_intr` in `6.5.22/unix.IP27` at 0xc0000000000973c0
  **Inferred.** Services the pending IOC3 interrupts of the device at arg0. It calls ioc3_pending_intrs at 0xc000000000096a24 (jal at 0xc000000000097404 and 0xc000000000097460), calls ioc3_write_ireg at 0xc00000000009688c with a2 = 1 (jal at 0xc0000000000974ac), and calls vsema at 0xc00000000018ac88 (jal at 0xc0000000000974b4). A branch at 0xc00000000009742c goes to 0xc0000000000974d8 when the count in s3 is below s5. Two indirect calls at 0xc000000000097518 and 0xc000000000097530 have callees the facts do not name, and cmn_err at 0xc000000000097584 reports an error. A word on a global structure is updated with ll/sc at 0xc000000000097440. The meaning of the pending masks and of the loop count is not settled by these facts.

- `ioc3_is_console` in `6.5.22/unix.IP27` at 0xc000000000097594
  **Inferred.** Tests whether the device at arg0 is the console device. It looks up the vertex with hwgraph_traverse at 0xc0000000002965ec (constant 0xc0000000003c8328) and, when that succeeds, gets the device record with hwgraph_fastinfo_get at 0xc000000000296ee8 (call at 0xc0000000000975d4). A non-zero doubleword at 0xc0000000014d15a0 selects a different path (bne at 0xc00000000009759c). The return is 0 on the paths shown, or a value from the record. Which value is the console flag is not settled by these facts.

- `ioc3_is_io7` in `6.5.22/unix.IP27` at 0xc0000000000975ec
  **Inferred.** Checks a flag for the IO7 bridge on the device at arg0. It looks up the vertex with hwgraph_traverse at 0xc0000000002965ec (constant 0xc0000000003c8328) and, when that succeeds, reads the byte at +0xb45 of the record that hwgraph_fastinfo_get at 0xc00000000009761c returns (lbu at 0xc000000000097628). It returns that byte, or 0 when the lookup fails. The bit meaning of the byte is not settled by these facts.

- `ioc3_error_handler` in `6.5.22/unix.IP27` at 0xc00000000009762c
  **Inferred.** Error handler for an IOC3 device, registered by ioc3_attach through pciio_error_register at 0xc0000000000956c8. It reads the record pointer at +0x10 of arg0, then the word at +0x4 and fields at +0x40 and +0x44. It reads many global status fields from addresses 0xc0000000003c8460 to 0xc0000000003c85f4. A mask test on the word at +0x4 selects one path (beq at 0xc00000000009766c), and an indirect jump at 0xc000000000097774 indexes a 13-entry table at 0xc0000000003c8600 when the value in a3 is below 13 (beq at 0xc000000000097768). Each path reports through cmn_err at 0xc0000000001f8090 at level 5 and returns 0 (return at 0xc000000000097748). The bit-to-error mapping and the table entries are not settled by these facts.

- `ioc3_intrd` in `6.5.22/unix.IP27` at 0xc000000000097920
  **Inferred.** Thread body for the IOC3 interrupt service thread, set by xthread_set_func in ioc3_intrd_setup. It reads the word at +0x0 and the doubleword at +0x8 and +0x10 of arg0, and the pointer at +0x20. From the structure at [arg0+32] it reads the doubleword at +0x18 and then the word at +0x1c of the structure that points to. It calls the function pointer at [a0+16] indirectly, with a callee the facts do not name (jalr at 0xc000000000097948), then takes the semaphore at +48 with ipsema at 0xc00000000018abb4 (jal at 0xc000000000097950).

- `ioc3_intrd_setup` in `6.5.22/unix.IP27` at 0xc000000000097968
  **Inferred.** Sets up the IOC3 interrupt service thread. It reads the halfword at +0x2a of the record at [arg0+32], passes it to setmustrun at 0xc0000000001959f4, and points the thread at ioc3_intrd at 0xc000000000097920 with xthread_set_func at 0xc0000000001c771c. It updates the word at +0x28 with an ll/sc sequence (ll at 0xc00000000009799c, sc at 0xc0000000000979a4), then calls ipsema at 0xc00000000018abb4 on the semaphore at +48 with the word at +0x28 and the same word with 0x400 ORed in (jal at 0xc0000000000979b0). The meaning of the 0x400 flag is not settled by these facts.

- `ioc3_console_vhdl_get` in `6.5.22/unix.IP27` at 0xc0000000000979c8
  **Inferred.** Returns the console vertex handle kept in the global at 0xc0000000014d15a0. It compares a2 with the doubleword at that global (beq at 0xc0000000000979cc) and otherwise loads the word from it (lw at 0xc0000000000979d4). The return at 0xc0000000000979d8 gives -1 or that word. Which branch gives -1 is not fully traced by these facts.

- `ioc3_subdev_ptr` in `6.5.22/unix.IP27` at 0xc0000000000979e0
  **Inferred.** Returns the sub-device state record for the device at arg0, creating it on demand. It finds the vertex with hwgraph_traverse at 0xc0000000002965ec (constant 0xc0000000003c8318) and reads an 8-byte label info record under the label at 0xc0000000003c8668 with hwgraph_info_get_LBL at 0xc000000000296160. When none exists and a1 is non-zero, it allocates 8 bytes with kmem_zalloc at 0xc000000000181ce0 and attaches it with hwgraph_info_add_LBL at 0xc0000000002960bc. It then reads the label again, and when the re-read pointer differs from the record it just allocated (bne at 0xc000000000097a70), the new record is freed with kmem_free at 0xc000000000181d4c. The return value at 0xc000000000097a84 is not traced by these facts.

- `ioc3_subdev_enabled` in `6.5.22/unix.IP27` at 0xc000000000097aa0
  **Inferred.** Reads the enable state of a sub-device on the device at arg0. It gets the sub-device record from ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 0, which does not create one. When the pointer is non-null it loads the doubleword at +0 of that record (ld at 0xc000000000097ac8). The return value at 0xc000000000097ae0 is open in these facts, and which bit of the loaded doubleword is tested is not shown.

- `ioc3_subdev_enables` in `6.5.22/unix.IP27` at 0xc000000000097ae8
  **Inferred.** Sets the enable state of a sub-device on the device at arg0. It gets the record from ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 1, which creates it when missing. On the non-null path it stores a doubleword at +0 of that record (sd at 0xc000000000097b08) and returns the pointer (return at 0xc000000000097b0c). The stored value is not traced in these facts.

- `ioc3_subdev_enable` in `6.5.22/unix.IP27` at 0xc000000000097b14
  **Inferred.** Sets an enable bit for a sub-device atomically. It gets the record from ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 1, and when the record is non-null it runs a 64-bit load-linked and store-conditional loop on the doubleword at +0 (lld at 0xc000000000097b3c, scd at 0xc000000000097b44). The retry test at 0xc000000000097b48 uses a0, whose value the facts do not trace. The routine returns the record pointer (return at 0xc000000000097b50). The bit that is set is not shown.

- `ioc3_subdev_disable` in `6.5.22/unix.IP27` at 0xc000000000097b58
  **Inferred.** Clears an enable bit for a sub-device atomically, the mirror of ioc3_subdev_enable. It gets the record from ioc3_subdev_ptr at 0xc0000000000979e0 with a1 = 1, and when the record is non-null it runs a 64-bit load-linked and store-conditional loop on the doubleword at +0 (lld at 0xc000000000097b84, scd at 0xc000000000097b8c). The retry test at 0xc000000000097b90 compares the loaded doubleword with the register v1, whose value is not shown. The routine returns the record pointer (return at 0xc000000000097b98). The bit that is cleared is not shown.
