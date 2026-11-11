# Routine role reads, kudzu tier-2 batch 3

These entries say what 25 routines of the 6.5.22 IP27 kernel image do, read from the instruction facts rendered for each one. Every label is Inferred: the facts give control flow, loads, stores, callees and constants, but not the names' meaning or the hardware behind each offset. Where a branch or constant decides the role and the facts do not settle it, the entry says so.

- `print_bridge_errcmd` in `6.5.22/unix.IP27` at 0xc00000000007adb8
  **Inferred.** A one-call logging helper. It passes severity 0x6 and its two entry arguments to `cmn_err` at 0xc00000000007add8, with a constant pointer at 0xc000000001437008 as one more argument, and returns the `cmn_err` result at 0xc00000000007ade4. The format string is not in the facts, so what it prints is not settled.

- `pcibr_intr_func` in `6.5.22/unix.IP27` at 0xc00000000007ab0c
  **Inferred.** Interrupt-side routine for a bridge slot. It takes the lock at a0+128 with `mutex_spinlock` at 0xc00000000007ac94 and drops it with `mutex_io_spinunlock` at 0xc00000000007acb8, clears and sets interrupt-enable bits through `pcireg_intr_enable_bit_clr` and `pcireg_intr_enable_bit_set`, and ends with `pcibr_force_interrupt` at 0xc00000000007ad4c. Which of the 2-byte field tests at 0xc00000000007ab44 and 0xc00000000007ab64 picks the path is not settled from the facts.

- `pcibr_error_dump` in `6.5.22/unix.IP27` at 0xc00000000007adec
  **Inferred.** Large report routine for bridge error state. It reads a long run of error registers through accessors such as `pcireg_bus_err_get` at 0xc00000000008080c, `pcireg_resp_err_get` at 0xc0000000000827b8, `pcireg_linkside_err_get` at 0xc000000000082654, `pcireg_pcix_req_err_addr_get` at 0xc000000000089c3c and `pcireg_ssram_parity_get` at 0xc000000000083840. It reports them through about 40 `cmn_err` calls and `print_bridge_errcmd` at 0xc00000000007adb8, and calls `pcibr_show_dir_state` at 0xc00000000007ad5c twice. The order of the reports and the meaning of each branch are not settled.

- `pcibr_pioerr_check` in `6.5.22/unix.IP27` at 0xc00000000007bf1c
  **Inferred.** Checks a PIO error. It reads the bridge interrupt status with `pcireg_intr_status_get` at 0xc00000000007bf28 and tests it against the mask 0x4400008800 at 0xc00000000007bf38. When the mask bits are set it converts an address with `pcireg_pci_bus_addr_get` at 0xc00000000007bf40 and walks a loop over a count and a 0x6-bounded index, returning the status or the address. The loop's exact meaning is not settled.

- `pcibr_error_intr_handler` in `6.5.22/unix.IP27` at 0xc00000000007c084
  **Inferred.** Error interrupt handler for a bridge. It reads the status with `pcireg_intr_status_get` at 0xc00000000007c098, logs through `pcibr_debug` at 0xc00000000007c0b8, and checks for PIO errors through `pcibr_pioerr_check` at 0xc00000000007c0e4. It locks at a0+128 with `mutex_spinlock` at 0xc00000000007c280, resets the interrupt with `pcireg_intr_reset_set` at 0xc00000000007c328, and on its failure paths calls `pcibr_error_dump` at 0xc00000000007c524 and 0xc00000000007c5cc before `panic` at 0xc00000000007c53c and 0xc00000000007c5e0. Which of its 59 branches runs on a given status is not settled.

- `pcibr_error_cleanup` in `6.5.22/unix.IP27` at 0xc00000000007c8e8
  **Inferred.** Short cleanup after an error. It loads the 2-byte field at a0+0x72 and compares it with 0 and 1 at 0xc00000000007c900 and 0xc00000000007c908. On the non-zero path it calls `pcireg_intr_reset_set` at 0xc00000000007c910 with a1 set to 0x41 or a larger constant and a3 set to 1, then reads `pcireg_tflush_get` at 0xc00000000007c918. The meaning of the 0x41 value is not settled.

- `pcibr_error_extract` in `6.5.22/unix.IP27` at 0xc00000000007c93c
  **Inferred.** Maps an error to a slot. It looks up the bridge through `hwgraph_info_get_LBL` at 0xc00000000007c958 and `hwgraph_fastinfo_get` at 0xc00000000007c974, gets the bus address with `pcireg_pci_bus_addr_get` at 0xc00000000007c988, and converts it with `pcibr_addr_toslot` at 0xc00000000007c9a0. A failed lookup at 0xc00000000007c960 or 0xc00000000007c980 returns 0xff, and the other path returns the converted slot.

- `pcibr_pioerror` in `6.5.22/unix.IP27` at 0xc00000000007c9bc
  **Inferred.** Large PIO error handler. It reads the 2-byte field at a0+0x72 and the status word at a0+0x48, reports through `pcibr_debug` at 0xc00000000007ca10 and 0xc00000000007cea8 and many `cmn_err` calls, and hands the error to `pciio_error_handler` at 0xc00000000007cf38 and 0xc00000000007d2a0. It runs `pcibr_error_cleanup` at 0xc00000000007d280 and 0xc00000000007d2c4, `pcibr_device_disable` at 0xc00000000007d2b4, and reads device state with `pcireg_device_get` at 0xc00000000007d590. It returns 0x7 on one path. The full case logic is not settled.

- `pcibr_dmard_error` in `6.5.22/unix.IP27` at 0xc00000000007d708
  **Inferred.** DMA read error handler. It reads the response-error buffer, device and address through `pcireg_resp_err_buf_get` at 0xc00000000007d734, `pcireg_resp_err_dev_get` at 0xc00000000007d740 and `pcireg_resp_err_get` at 0xc00000000007d760, stores them into the record at a3, and calls `pciio_error_handler` at 0xc00000000007d788. When that handler returns zero, the branch at 0xc00000000007d790 leads to `pcibr_device_disable` at 0xc00000000007d7a4, an interrupt reset with 0x10 at 0xc00000000007d7b8, and the read-buffer register get and set calls at 0xc00000000007d7d4 to 0xc00000000007d810. It returns the handler's result.

- `pcibr_config_addr` in `6.5.22/unix.IP27` at 0xc00000000007dde8
  **Inferred.** Computes a configuration-space address for a device. It fetches the pciio info through `pciio_info_get` at 0xc00000000007de00, the bridge info through `pcibr_info_get` at 0xc00000000007de10, and checks `pciio_info_type1_get` at 0xc00000000007de1c and `pciio_info_hostdev_get` at 0xc00000000007de74. In the usual case it calls `pcibr_func_config_addr` at 0xc00000000007de5c with a value read from the bridge info chain. Compares of a0 against 0xff at 0xc00000000007dea4 and 0xc00000000007deac pick the fallback bytes at +540 and +18 of the bridge info. The role is inferred from those calls and loads.

- `pcibr_func_config_addr` in `6.5.22/unix.IP27` at 0xc00000000007dc08
  **Inferred.** Builds a configuration address for one function. A positive bus value (the test at 0xc00000000007dc2c) takes the type-1 path through `pcireg_type1_cntr_set` at 0xc00000000007dc3c and `pcireg_type1_cfg_addr` at 0xc00000000007dc4c, which uses the shifted bus value (<<16) and device value (<<11). Otherwise it takes the type-0 path through `pcireg_type0_cfg_addr` at 0xc00000000007dc7c. It returns one of the two addresses.

- `pcibr_slot_config_addr` in `6.5.22/unix.IP27` at 0xc00000000007dc8c
  **Inferred.** A thin wrapper. It calls `pcibr_func_config_addr` at 0xc00000000007dca4 with bus 0 and function 0, forwarding its slot and offset arguments, and returns that address. Bus 0 selects the type-0 path inside the callee.

- `pcibr_config_get` in `6.5.22/unix.IP27` at 0xc00000000007dedc
  **Inferred.** Config-space read wrapper. It calls `pcibr_config_addr` at 0xc00000000007deec, then `do_pcibr_config_get` at 0xc00000000007defc with that address and its a2 argument, and returns the read result.

- `pcibr_config_set` in `6.5.22/unix.IP27` at 0xc00000000007dfc0
  **Inferred.** Config-space write wrapper, the write-side counterpart of `pcibr_config_get`. It calls `pcibr_config_addr` at 0xc00000000007dfd4, then `do_pcibr_config_set` at 0xc00000000007dfe8 with the address and its a2 and a3 arguments, and returns that result.

- `pcibr_slot_info_init` in `6.5.22/unix.IP27` at 0xc00000000007e138
  **Inferred.** Initializes the per-slot info record for a PCI slot. It reads and writes configuration through `do_pcibr_config_get` (9 call sites) and `do_pcibr_config_set` (4 call sites), probes the slot with `pcibr_probe_slot` at 0xc000000000080038, allocates the record with `pcibr_device_info_new` at 0xc000000000073088 and `kmem_zalloc`, and registers it with `pciio_device_info_register` and `pciio_device_win_populate`. It also checks `pcibr_find_capability` at 0xc00000000007ec68 and `pcibr_is_slot_sys_critical` at 0xc00000000007ff28. Its returns are 0x0, 0x1, 0x13 or 0x16; the facts do not name those codes.

- `pcibr_slot_device_init` in `6.5.22/unix.IP27` at 0xc00000000007f7a4
  **Inferred.** Per-slot device setup. It gets the bridge's fast info through `hwgraph_fastinfo_get` at 0xc00000000007f7b8, reads the device word through `pcireg_device_get` at 0xc00000000007f7fc and writes it through `pcireg_device_set` at 0xc00000000007f83c with 0x8000000 in a7. It stores a 64-bit word at an offset from (a1<<7)+0x138 (at 0xc00000000007f838) and logs through `pcibr_debug` at 0xc00000000007f85c. It returns 0 or 0x16. The compare of the 2-byte field at +0x72 with a1=2 at 0xc00000000007f81c selects a second path whose purpose is not settled.

- `pcibr_piomap_probe` in `6.5.22/unix.IP27` at 0xc000000000080058
  **Inferred.** Pre-check for a PIO map probe. It loads a window bound from a0+0x18 and a0+0x20 and a pointer from a0+0x40, and checks at 0xc000000000080074 that the requested end (a2+a1) fits the window. It then calls `pcibr_probe_work` at 0xc000000000080094 with the bounds. It appears to return -1 when the range does not fit.

- `pcibr_probe_work` in `6.5.22/unix.IP27` at 0xc0000000000800a8
  **Inferred.** Probes one access. It dispatches on the access width in a2 against 1, 2, 4 and 8 at 0xc0000000000800d0 to 0xc0000000000800ec. It disables the MST timeout with `pcibr_disable_mst_timeout_work` at 0xc000000000080120, probes the address with `badaddr_val` at 0xc000000000080134, sets an interrupt reset with 0x40 at 0xc000000000080148, and re-enables the timeout with `pcibr_enable_mst_timeout_work` at 0xc00000000008015c. It returns -1 or the probe result.

- `pcibr_enable_mst_timeout` in `6.5.22/unix.IP27` at 0xc000000000080198
  **Inferred.** Wrapper for enabling the MST timeout. It gets the bridge fast info through `hwgraph_fastinfo_get` at 0xc0000000000801a0, loads the word at +0x8 of that record at 0xc0000000000801ac, and passes it to `pcibr_enable_mst_timeout_work` at 0xc0000000000801a8. It returns that routine's result.

- `pcibr_disable_mst_timeout_work` in `6.5.22/unix.IP27` at 0xc0000000000801bc
  **Inferred.** Clears the mask 0x400000800 in the interrupt-enable register. It reads the register with `pcireg_intr_enable_get` at 0xc0000000000801cc, clears the bits through `pcireg_intr_enable_bit_clr` at 0xc0000000000801dc, and reads again at 0xc0000000000801e4. It compares the two reads at 0xc0000000000801f0 and returns 0 or 1. The facts do not name what the flag means to the caller.

- `pcibr_enable_mst_timeout_work` in `6.5.22/unix.IP27` at 0xc000000000080210
  **Inferred.** The mirror of the disable routine. It reads the enable register with `pcireg_intr_enable_get` at 0xc000000000080220, sets the mask 0x400000800 through `pcireg_intr_enable_bit_set` at 0xc000000000080230, reads again at 0xc000000000080238, compares the reads at 0xc000000000080244, and returns 0 or 1.

- `pcireg_id_get` in `6.5.22/unix.IP27` at 0xc000000000080548
  **Inferred.** Register accessor with a dispatch on the 2-byte field at a0+0x72, compared with 0, 1, 2, 3 and -1 at 0xc000000000080574 to 0xc000000000080598. It then compares a 32-bit value at a0+0x4 with the constants 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002 at 0xc0000000000805d4 to 0xc0000000000805fc, and panics at 0xc000000000080698 on a value it does not know. The constants are not named in the facts, so the register each one stands for is unknown.

- `pcireg_bus_err_get` in `6.5.22/unix.IP27` at 0xc00000000008080c
  **Inferred.** Accessor with the same shape as `pcireg_id_get`: a dispatch on the 2-byte field at a0+0x72 and the same constant chain (0xe000 through 0xc002) around 0xc000000000080898 to 0xc0000000000808c8, with a `panic` at 0xc00000000008096c for a value outside those cases. The register each case reads is an open sub-fact: the stored value is unknown.

- `pcireg_control_get` in `6.5.22/unix.IP27` at 0xc00000000008097c
  **Inferred.** Accessor with the same dispatch shape as `pcireg_id_get`: the 2-byte field at a0+0x72 is compared with 0, 1, 2, 3 and -1, the constant chain 0xe000 through 0xc002 selects the case around 0xc000000000080a0c to 0xc000000000080a34, and a value outside those cases panics at 0xc000000000080ad4. The register each case reads is an open sub-fact: the stored value is unknown.

- `pcireg_control_set` in `6.5.22/unix.IP27` at 0xc000000000080ae4
  **Inferred.** Setter with the same field dispatch as `pcireg_control_get`. It raises the interrupt level with `spl1` at 0xc000000000080af8 and restores it with `splx` at 0xc000000000080b5c around the write, stores 32-bit and 64-bit values at 0xc000000000080bd0, 0xc000000000080bec and 0xc000000000080c08, and panics at 0xc000000000080c5c on an unknown case. The returns come from `splx`.
