# Routine role reads, kudzu tier-2 batch 3

These entries say what 25 routines of the 6.5.22 IP27 kernel image do, read from the instruction facts rendered for each one. Every label is Inferred: the facts give control flow, loads, stores, callees and constants, but not the names' meaning or the hardware behind each offset. Where a branch or constant decides the role and the facts do not settle it, the entry says so.

- `print_bridge_errcmd` in `6.5.22/unix.IP27` at 0xc00000000007adb8
  **Inferred.** A one-call logging helper. It passes severity 0x6 and its two entry arguments to `cmn_err` at 0xc00000000007add8, with a constant pointer at 0xc000000001437008 as one more argument, and returns the `cmn_err` result at 0xc00000000007ade4. The format string is not in the facts, so what it prints is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_intr_func` in `6.5.22/unix.IP27` at 0xc00000000007ab0c
  **Inferred.** Interrupt-side routine for a bridge slot. On one path it takes the lock at [a0]+128 (the pointer loaded from a0, plus 128) with `mutex_spinlock` at 0xc00000000007ac94 and drops it with `mutex_io_spinunlock` at 0xc00000000007acb8, and on another it clears and sets interrupt-enable bits through `pcireg_intr_enable_bit_clr` and `pcireg_intr_enable_bit_set` at 0xc00000000007ad24 and 0xc00000000007ad30. It ends with `pcibr_force_interrupt` at 0xc00000000007ad4c on the paths that reach it. Which path runs is decided by a 2-byte compare with 2 at 0xc00000000007ab44 and a test of the word at a0+0x18 at 0xc00000000007ab64, and the facts do not settle which test picks which path.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_error_dump` in `6.5.22/unix.IP27` at 0xc00000000007adec
  **Inferred.** Large report routine for bridge error state. It reads a long run of error registers through accessors such as `pcireg_bus_err_get` at 0xc00000000008080c, `pcireg_resp_err_get` at 0xc0000000000827b8, `pcireg_linkside_err_get` at 0xc000000000082654, `pcireg_pcix_req_err_addr_get` at 0xc000000000089c3c and `pcireg_ssram_parity_get` at 0xc000000000083840. It reports them through about 40 `cmn_err` calls and `print_bridge_errcmd` at 0xc00000000007adb8, and calls `pcibr_show_dir_state` at 0xc00000000007ad5c twice. The order of the reports and the meaning of each branch are not settled.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_pioerr_check` in `6.5.22/unix.IP27` at 0xc00000000007bf1c
  **Inferred.** Role (from name): checks a PIO error. It calls `pcireg_intr_status_get` at 0xc00000000007bf28 and tests the result against the mask 0x4400008800 at 0xc00000000007bf38; when the AND is zero it skips to 0xc00000000007c070. Otherwise it calls `pcireg_pci_bus_addr_get` at 0xc00000000007bf40 and enters a loop that tests a signed count at 0xc00000000007bf58 and 0xc00000000007bf68, compares an unsigned index with 0x6 at 0xc00000000007bfb8, and compares the bus address with a loaded value at 0xc00000000007bffc. It returns the status or the bus address. The loop's exact meaning is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_error_intr_handler` in `6.5.22/unix.IP27` at 0xc00000000007c084
  **Inferred.** Error interrupt handler for a bridge. It reads the status with `pcireg_intr_status_get` at 0xc00000000007c098, logs through `pcibr_debug` at 0xc00000000007c0b8, and calls `pcibr_pioerr_check` at 0xc00000000007c0e4 when the status ANDed with 0x4400008800 is non-zero. On one path it locks at a0+128 with `mutex_spinlock` at 0xc00000000007c280, resets the interrupt with `pcireg_intr_reset_set` at 0xc00000000007c328, and on paths that end in `panic` it calls `pcibr_error_dump` at 0xc00000000007c524 and 0xc00000000007c5cc, each followed by `machine_error_dump` and then `panic` at 0xc00000000007c53c and 0xc00000000007c5e0. Which branch runs on a given status is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_error_cleanup` in `6.5.22/unix.IP27` at 0xc00000000007c8e8
  **Inferred.** Short cleanup after an error. It loads the 2-byte field at a0+0x72 and compares it with 0 and 1 at 0xc00000000007c900 and 0xc00000000007c908. It calls `pcireg_intr_reset_set` at 0xc00000000007c910 with a1 set to 0x41 and a3 set to 1 unless the field equals 1, which branches past the call, then reads `pcireg_tflush_get` at 0xc00000000007c918 and returns its result at 0xc00000000007c928. The meaning of the 0x41 value is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_error_extract` in `6.5.22/unix.IP27` at 0xc00000000007c93c
  **Inferred.** Role (from name): maps an error to a slot. It calls `hwgraph_info_get_LBL` at 0xc00000000007c958 and returns 0xff at 0xc00000000007c96c when that call returns non-zero (branch at 0xc00000000007c960). Otherwise it calls `hwgraph_fastinfo_get` at 0xc00000000007c974, tests its result at 0xc00000000007c980, passes that result to `pcireg_pci_bus_addr_get` at 0xc00000000007c988, and calls `pcibr_addr_toslot` at 0xc00000000007c9a0 with the bus address. The facts show a single return, 0xff; where the other paths return and what they return is not in them.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_pioerror` in `6.5.22/unix.IP27` at 0xc00000000007c9bc
  **Inferred.** Role (from name): PIO error handler, a large routine. It reads the 2-byte field at a0+0x72 and the word at a0+0x48, reports through `pcibr_debug` at 0xc00000000007ca10 and 0xc00000000007cea8 and many `cmn_err` calls, and calls `pciio_error_handler` at 0xc00000000007cf38 and 0xc00000000007d2a0. It calls `pcibr_error_cleanup` at 0xc00000000007d280 and 0xc00000000007d2c4, `pcibr_device_disable` at 0xc00000000007d2b4, and `pcireg_device_get` at 0xc00000000007d590. It returns 0x7 on one path. The full case logic is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_dmard_error` in `6.5.22/unix.IP27` at 0xc00000000007d708
  **Inferred.** Role (from name): DMA read error handler. It calls `pcireg_resp_err_buf_get` at 0xc00000000007d734, `pcireg_resp_err_dev_get` at 0xc00000000007d740 and `pcireg_resp_err_get` at 0xc00000000007d760, makes stores at offsets 0x0, 0x8 and 0x28 of a base the facts do not resolve (the values stored are not in the facts), and calls `pciio_error_handler` at 0xc00000000007d788. When the handler returns non-zero, the fall-through of the branch at 0xc00000000007d790 calls `pcibr_device_disable` at 0xc00000000007d7a4; when it returns zero that call is skipped. Both paths then call `pcireg_intr_reset_set` with 0x10 at 0xc00000000007d7b8 and the read-buffer register calls `pcireg_rrb_get` at 0xc00000000007d7d4 and `pcireg_rrb_set` at 0xc00000000007d800 and 0xc00000000007d810. It returns the handler's result.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_config_addr` in `6.5.22/unix.IP27` at 0xc00000000007dde8
  **Inferred.** Role (from name): computes a configuration-space address for a device. It calls `pciio_info_get` at 0xc00000000007de00 and `pcibr_info_get` at 0xc00000000007de10, then `pciio_info_type1_get` at 0xc00000000007de1c. When that returns non-zero it calls `pcibr_func_config_addr` at 0xc00000000007de5c with a value read through the bridge info chain and returns its result. When it returns zero the path goes on to `pciio_info_hostdev_get` at 0xc00000000007de74. After that, compares of 0xff against bytes at +540 and +18 of the bridge info at 0xc00000000007dea4 and 0xc00000000007deac choose between paths; the facts do not show what each path returns.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_func_config_addr` in `6.5.22/unix.IP27` at 0xc00000000007dc08
  **Inferred.** Builds a configuration address for one function. When a1 is positive (the test at 0xc00000000007dc2c) it takes the type-1 path: `pcireg_type1_cntr_set` at 0xc00000000007dc3c is passed (a1 << 16) | (a2 << 11), and `pcireg_type1_cfg_addr` at 0xc00000000007dc4c is passed a3 and a4. Otherwise it takes the type-0 path through `pcireg_type0_cfg_addr` at 0xc00000000007dc7c. It returns one of the two cfg_addr results.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_slot_config_addr` in `6.5.22/unix.IP27` at 0xc00000000007dc8c
  **Inferred.** A thin wrapper. It calls `pcibr_func_config_addr` at 0xc00000000007dca4 with a1 and a3 set to 0, forwarding its own a1 as a2 and its a2 as a4, and returns that address. A zero a1 takes the type-0 path inside the callee. Role (from name): the zeroed arguments are bus and function.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_config_get` in `6.5.22/unix.IP27` at 0xc00000000007dedc
  **Inferred.** Role (from name): config-space read wrapper. It calls `pcibr_config_addr` at 0xc00000000007deec, then `do_pcibr_config_get` at 0xc00000000007defc with that address and its a2 argument, and returns that call's result.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_config_set` in `6.5.22/unix.IP27` at 0xc00000000007dfc0
  **Inferred.** Role (from name): config-space write wrapper, the counterpart of `pcibr_config_get`. It calls `pcibr_config_addr` at 0xc00000000007dfd4, then `do_pcibr_config_set` at 0xc00000000007dfe8 with the address and its a2 and a3 arguments, and returns that call's result.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_slot_info_init` in `6.5.22/unix.IP27` at 0xc00000000007e138
  **Inferred.** Role (from name): initializes the per-slot info record for a PCI slot. It calls `do_pcibr_config_get` (9 call sites) and `do_pcibr_config_set` (4 call sites), `pcibr_probe_slot` at 0xc000000000080038 (2 call sites), `kmem_zalloc` and `pcibr_device_info_new` at 0xc000000000073088, then `pciio_device_info_register`, and `pciio_device_win_populate` (2 call sites). It also calls `pcibr_find_capability` at 0xc00000000007ec68, `pcibr_is_slot_sys_critical` at 0xc00000000007ff28, `pcibr_bus_addr_alloc`, `sprintf` and `pcibr_debug`. Its returns are 0x0, 0x1, 0x13 or 0x16; the facts do not name those codes.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_slot_device_init` in `6.5.22/unix.IP27` at 0xc00000000007f7a4
  **Inferred.** Per-slot device setup. It calls `hwgraph_fastinfo_get` at 0xc00000000007f7b8, reads the device word through `pcireg_device_get` at 0xc00000000007f7fc and writes it through `pcireg_device_set` at 0xc00000000007f83c with 0x8000000 in a7. A 64-bit store at offset 0x138 of a base the facts do not resolve is at 0xc00000000007f838. It logs through `pcibr_debug` at 0xc00000000007f85c and returns 0 or 0x16. The compare of the 2-byte field at +0x72 with the constant 2 at 0xc00000000007f81c selects a second path whose purpose is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_piomap_probe` in `6.5.22/unix.IP27` at 0xc000000000080058
  **Inferred.** Pre-check for a PIO map probe. It loads a limit from a0+0x18 and compares it with a2+a1 at 0xc000000000080074; it appears to return -1 when the limit is below a2+a1. Otherwise it calls `pcibr_probe_work` at 0xc000000000080094 with a0 loaded through a0+0x40, a1 advanced by the word at a0+0x20, and a2 and a3 passed through. It returns -1 or the `pcibr_probe_work` result.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_probe_work` in `6.5.22/unix.IP27` at 0xc0000000000800a8
  **Inferred.** Probes one access. It compares the access width in a2 against 1, 2, 4 and 8 at 0xc0000000000800d0 to 0xc0000000000800ec. It calls `pcibr_disable_mst_timeout_work` at 0xc000000000080120, probes the address with `badaddr_val` at 0xc000000000080134 and calls `pcireg_intr_reset_set` with 0x40 at 0xc000000000080148. It calls `pcibr_enable_mst_timeout_work` at 0xc00000000008015c only when the disable call returned non-zero (the branch at 0xc000000000080154 skips it otherwise). It returns -1 on one path or a value the facts do not resolve.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_enable_mst_timeout` in `6.5.22/unix.IP27` at 0xc000000000080198
  **Inferred.** Wrapper for enabling the MST timeout. It calls `hwgraph_fastinfo_get` at 0xc0000000000801a0 and passes the doubleword at +0x8 of its result to `pcibr_enable_mst_timeout_work` at 0xc0000000000801a8. It returns that routine's result.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_disable_mst_timeout_work` in `6.5.22/unix.IP27` at 0xc0000000000801bc
  **Inferred.** Clears the mask 0x400000800 in the interrupt-enable register. It reads the register with `pcireg_intr_enable_get` at 0xc0000000000801cc, clears the bits through `pcireg_intr_enable_bit_clr` at 0xc0000000000801dc, and reads again at 0xc0000000000801e4. It compares the two reads at 0xc0000000000801f0 and returns 0 or 1. The facts do not name what the flag means to the caller.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_enable_mst_timeout_work` in `6.5.22/unix.IP27` at 0xc000000000080210
  **Inferred.** The mirror of the disable routine. It reads the enable register with `pcireg_intr_enable_get` at 0xc000000000080220, sets the mask 0x400000800 through `pcireg_intr_enable_bit_set` at 0xc000000000080230, reads again at 0xc000000000080238, compares the reads at 0xc000000000080244, and returns 0 or 1.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_id_get` in `6.5.22/unix.IP27` at 0xc000000000080548
  **Inferred.** Register accessor with a dispatch on the 2-byte field at a0+0x72, compared with 0, 1, 2, 3 and -1 at 0xc000000000080574 to 0xc000000000080598. It also compares a value with 0x9200000000000000 at 0xc000000000080564. It then compares a 32-bit value at +0x4 of a base the facts do not resolve (a0 is reloaded after the field compares) with the constants 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002 at 0xc0000000000805d4 to 0xc0000000000805fc. It calls `panic` at 0xc000000000080698 when a0 is zero, when the field is -1, or when no constant matches. The constants are not named in the facts, so the register each one stands for is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_bus_err_get` in `6.5.22/unix.IP27` at 0xc00000000008080c
  **Inferred.** Accessor with the same shape as `pcireg_id_get`: a dispatch on the 2-byte field at a0+0x72 and the same constant chain (0xe000 through 0xc002) around 0xc000000000080898 to 0xc0000000000808c8, with a `panic` at 0xc00000000008096c for a value outside those cases. The register each case reads is an open sub-fact: the stored value is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_control_get` in `6.5.22/unix.IP27` at 0xc00000000008097c
  **Inferred.** Accessor with the same dispatch shape as `pcireg_id_get`: the 2-byte field at a0+0x72 is compared with 0, 1, 2, 3 and -1, the constant chain 0xe000 through 0xc002 selects the case around 0xc000000000080a0c to 0xc000000000080a34, and a value outside those cases panics at 0xc000000000080ad4. The register each case reads is an open sub-fact: the stored value is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_control_set` in `6.5.22/unix.IP27` at 0xc000000000080ae4
  **Inferred.** Setter with the same field dispatch as `pcireg_control_get`. It calls `spl1` at 0xc000000000080af8 and `splx` with its result at 0xc000000000080b5c, stores 32-bit and 64-bit values at 0xc000000000080bd0, 0xc000000000080bec and 0xc000000000080c08, and calls `panic` at 0xc000000000080c5c on an unknown case. Whether every write path reaches the `splx` call is not in the facts. The only return is the `splx` result.
  Reviewed: 2026-11-12 (sonnet)
