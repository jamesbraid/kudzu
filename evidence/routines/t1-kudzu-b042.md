# Routine role statements, tier-1 batch 42

These are role statements for 22 routines, the 42nd slice of the tier-1 reading order. The slice is the last 22 rows of the 1072-row ranked list, since the list ends before batch 42 reaches 25 rows. Each statement rests on what the disassembly shows for that routine: registers, calls, branches and constants. Where the disassembly stops short of a role, the entry says so. Addresses are in the binary's own space.

Most of the slice is the IP35 kernel object and its sibling archives in 6.5.30. Large routines (`syssgi`, `gf_OdsyInitialize`, `pic_attach2`, `xbridge_attach`) were summarized from their callees and entry disassembly, not traced branch by branch, so their statements are coarser than the small ones.

- `VEC_dbe` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x395a0
  **Inferred.** It loads a halfword from 0xffffffffffffc428 at 0x395a0, stores a doubleword at arg0 +0x140 at 0x395a8, and then jumps to `VEC_trap` at 0x395a4 with its four argument registers passed on unchanged. It has no return of its own. The role is most likely a data-bus-error vector stub that records a value in the trap frame and hands off to `VEC_trap`. The stored value and the meaning of the halfword are not established.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3bc90
  **Inferred.** It reads many fields of the error record in a1 (offsets +0x8, +0x10, +0x18, +0x26, +0x28 through +0x34, and +0x2b8 and +0x2bc). It calls `cer_to_hex` at 0x3bcec, `eccf_recoverable_threshold` at 0x3bd90, and `save_cache_error` at 0x3bf60. It makes several indirect calls through the table at 0x3b4f8 (`ecc_cacheOp`) with op codes 0x0, 0x1, 0x3, 0x4, 0x5 and 0x7, and calls `ecc_force_cache_line_invalid` at 0x3be80, 0x3be90 and 0x3c328. `failsoft_panic_check` at 0x3c21c is reached when `eccf_recoverable_threshold` returns 0 (branch at 0x3be9c). `memerror_handle_uce` is called at 0x3c300 and `cmn_err_tag` at 0x3c3ec with a0 set to 0xb96. Role (from name): a memory or cache ECC error handler. The op code meanings, the record layout, which paths are unrecoverable, and the exact recovery order are not shown by this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `bte_init` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3d1d8
  **Inferred.** It calls `spinlock_init` at 0x3d214 on the lock at arg0 +0x3c. It then stores to arg0 at offsets +0x0, +0x18, +0x20, +0x28, +0x4c and +0x68, and a halfword to +0x38. It reads a halfword at 0xffffffffffffc42a and a second halfword-sized table entry indexed by it, loaded through a gp-relative pointer whose symbol the facts do not name. It also tests a machine-wide word `scache_linemask` at 0x3d200 and a CPU count `maxcpus` at 0x3d2d8, and returns arg0. Role (from name): initialization of a block transfer engine. The stores fit a per-node control block with a lock and per-node slots, but the field meanings, and the destination of the remaining doubleword stores, are not established.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3eb9c
  **Inferred.** It builds a 48-byte frame and calls `probe_common` through the pointer at 0x3ec6c at 0x3ebb8, passing the frame address as a0 and passing a1 through a3 unchanged. It returns whatever `probe_common` returns. The role is most likely a wrapper that probes one address for a bad-address condition, with `probe_common` doing the access. The probe's read width and what the return value means are not shown here.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3ebcc
  **Inferred.** Its shape matches `badaddr`: a 48-byte frame and a call to `probe_common` at 0x3ebe8 through the same pointer at 0x3ec6c, returning its result. The name suggests a probe that also returns the value read, but this disassembly shows no store of the value and no separate return path. The value handling sits inside `probe_common`, which this statement does not cover.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3ebfc
  **Inferred.** Same frame and call shape as `badaddr`, with the call to `probe_common` at 0x3ec20. The name suggests a write probe. The disassembly shows the call but not the store, so the write behavior is inferred from the name only.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x439b0
  **Inferred.** It loads two doublewords at 0x439b0 and 0x439b4 from registers the disassembly does not resolve. It branches on the sign of ra at 0x439b8, stores a doubleword at 0x439c4, loads the same halfword at 0xffffffffffffc428 that `VEC_dbe` loads, stores a doubleword at arg0 +0x140 at 0x439d0, and jumps to `VEC_trap` at 0x439cc. The role is most likely an instruction-bus-error vector stub, parallel to `VEC_dbe` with an extra sign test on ra. What the sign test selects, and the value stored, are not established.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.30/usr/cpu/sysgen/IP35boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** It calls `wchanmeter` at 0x38b0 with the `syncinfo` symbol, `mr_access` at 0x38c0, `mrmeter_getentry` at 0x392c (entry code 0x5), and `mrmeter_entry` at 0x3994 (entry code 0x5). The metering record at `wchanmeter`'s return value is updated at offsets +0x198, +0x1c0, +0x1c8, +0x1cc and +0x1d0, and the word at +0x198 is updated with an `ll`/`sc` loop at 0x3940 through 0x394c. The branch at 0x38b8 tests whether `wchanmeter` returned a record. The `mr_access` call at 0x38c0 sits on the fall-through of that test, and the branch at 0x38c8 skips the counter updates when the record is null. The `mrmeter_entry` call is on the non-null branch target. Whether `mr_access` is also reached on that path is not shown by the facts. The counter updates and the `mrmeter_getentry` call are on the record-present path. Role (from name): the access-side hook of a metered synchronization object. The meaning of the `ll`/`sc` word and of the metering fields is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.30/usr/cpu/sysgen/IP35boot/ksync_metered.a(sema.o)` at 0x39ec
  **Inferred.** The update-side counterpart of `_mraccess`. It calls `wchanmeter` at 0x3a0c, `mr_update` at 0x3a1c, and `mrmeter_entry` at 0x3ac4 with entry code 0x6. It updates the record at offsets +0x1a0, +0x1a8, +0x1b0, +0x1b8 and +0x1d8, and tests bit 0x4 of a7 at 0x3a90 to choose a path. The role is most likely the update hook for the same metered object, with a separate meter entry code. The bit tested in a7 and the fields updated are not identified.
  Reviewed: 2026-11-12 (sonnet)

- `get_myid` in `6.5.30/usr/cpu/sysgen/IP35boot/l1.o` at 0xc3c
  **Inferred.** It loads a doubleword from the fixed address 0x9200000001000020 at 0xc40 and returns it unchanged at 0xc44. There is no shift or mask. The name suggests a returned identifier, most likely the current node or CPU id. Whether the full word is the id, or a field of it, depends on what the register holds, which this disassembly does not show.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_phase_bits` in `6.5.30/usr/cpu/sysgen/IP35boot/nic.o` at 0x68
  **Inferred.** It loads a doubleword at 0x70 through a base register the facts do not resolve, and returns a value in v0 at 0x94 that the facts do not decode. The return value is not shown to be the loaded word unchanged. Role (from name): returns NIC phase bits. The load address, any masking or shifting, and the bit positions are not established by this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyAttach` in `6.5.30/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_attach.o)` at 0x68
  **Inferred.** It contains a loop whose back-edge branch is at 0xc8 and whose body starts at 0xa0, and it calls `gfxdd_recycle` at 0xfc with the `odsy_regmatch` symbol. It calls `gfxdd_mmap` at 0x18c with a0 set to 0x1 and the `gfx_fault` symbol in a3. When `gfxdd_mmap` returns nonzero, the branch at 0x194 goes to a path that logs through `cmn_err` at 0x1e0 with level 0x4. When it returns 0, the fall-through path calls `splhi` at 0x19c, `gf_OdsyUnMapGfx` at 0x1b4 and `splx` at 0x1bc. It returns 0, 0xc or 0x16 at 0xec. Role (from name): attaching the Odyssey graphics context to a caller, with a fault handler for the mapping. The three return codes look like errno values, but the disassembly does not name them. The meaning of the context list, of the `gfxdd_mmap` arguments, and of why the unmap call is on the path where `gfxdd_mmap` returned 0 is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `gfP_OdsySaveHwShadow` in `6.5.30/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_ctxsw.o)` at 0xc0
  **Inferred.** It calls `RRM_CheckValidFault`, `odsy_flow_am_i_attached`, `odsy_flow_await_returned_credits`, `gfx_flow_detach_me`, `gfx_flow_attach_me` and `gfx_flow_attach_me_finish`. It reads flow credit counts with `gfx_flow_credit_count_get` and `gfx_flow_credit_bias_get`, pins and unpins the thread with `rt_pin_thread` and `rt_unpin_thread`, schedules a callout with `timeout` (the `odsy_ctxsw_timeout` symbol) at 0x18c, cancels it with `untimeout_wait` at 0x1a0, and polls with `us_delay`. Some callees are unresolved in the disassembly output, and `cmn_err` is called at 0x52c and 0x54c with levels 0x4 and 0x1. Role (from name): a context-switch save of the graphics hardware shadow state. The routine appears to wait for flow-control credits to return, detach and reattach the graphics flow, and keep the thread pinned while it does so. Which hardware registers are saved is not identified by this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyDestroyDDRN` in `6.5.30/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_ddrn.o)` at 0x438
  **Inferred.** It calls `gf_OdsyUnSchedSwapBuf` at 0x4a0 (when a field loaded at 0x490 is not negative) and `gf_OdsyUnSchedRetraceEvent` at 0x4ac. The branch at 0x4b8 then selects between two paths. The first calls `odsy_hpipe_cleanup`, `kmem_free` (when a pointer is nonzero) and `OdsyFree32kSlot`, and brackets the flow detach with `splhi` and `splx`, calling `gfx_flow_detach_me` and `gfx_flow_dealloc`. The second, at 0x64c, calls `odsyDMAFinish`, `odsyMMdetach` and `odsyUnpinAndUnmapGctxRegions`. `gfx_flow_clean_up_other_gfxp` is called at 0x5fc. Role (from name): the teardown of a per-context Odyssey resource. The name `DDRN` is not expanded by the disassembly, so the exact object being destroyed is unclear, and the order in which the two paths run relative to each other is not traced.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyInitialize` in `6.5.30/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_init.o)` at 0x15dc
  **Inferred.** It calls `htp_register_board` at 0x1650 with zero arguments, `get_nasid` at 0x1818 and 0x1834, and `get_chimera_type` at 0x1820 and 0x183c, comparing the result with 0x3 and 0x5. In address order it calls `odsyInitCfifo` at 0x17b4, `odsyReset` at 0x17fc, then `odsyInitMM`, `odsyInitDMA`, `odsyInitRetrace` and `odsyInitInterrupts` at 0x191c through 0x1940. It allocates a 32K slot through `OdsyInit32kSlotMask` and `OdsyAlloc32kSlot` (called twice), zeroes state with `odsyZeroOut`, and uses flow calls (`gfx_flow_alloc`, `gfx_flow_wg_info_get`, `gfx_flow_credit_tnum_get`) and `psema`, `us_delay` and `xtalk_intr_target_get`. Role (from name): the bring-up of the Odyssey graphics board. The calls cover board registration, chip reset, command FIFO, memory management, DMA, retrace and interrupt setup, and flow allocation. The execution order of `odsyInitCfifo` relative to `odsyReset`, the error paths and the purpose of the `get_chimera_type` comparisons are not traced.
  Reviewed: 2026-11-12 (sonnet)

- `odsyLowLevelReset` in `6.5.30/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_init.o)` at 0x2054
  **Inferred.** It accesses 32-bit words in a register window. The facts resolve the addresses 0x92000000e000010c, 0x92000000e0000114, 0x92000000e000013c and 0x92000000e0000024. Which accesses are reads and which are writes is not separated by the facts, and the window base for other accesses is not shown. It calls `odsy_usdelay` at 0x2304 with a delay of 0xf4240 and at 0x2368 with a delay of 0x186a0 (decimal 1,000,000 and 100,000), and calls `cmn_err` at 0x23b8 with level 0x4. Role (from name): a low-level reset of the Odyssey chip, most likely writing a control register, waiting with timed delays, polling status registers and reporting failure. The register meanings, the delay units and the reset sequence are not established by this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.30/usr/cpu/sysgen/IP35boot/os.a(pda.o)` at 0x1c00
  **Inferred.** It reads a byte from 0xffffffffffffc080 and tests it at 0x1c0c. Along one path it calls `panicspin` at 0x1c24 with the address 0x92000000017a0038 in a0. It stores to the byte at 0xffffffffffffc081, reads a word at 0xffffffffffffc084, and calls `tlbflush_rand` at 0x1c4c and `tlbflush` at 0x1c60 when the matching values are nonzero. The name suggests it runs the pending action vector for a CPU. What this code does with the flag bytes, and whether the `panicspin` path is a real failure branch, is not established by this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.30/usr/cpu/sysgen/IP35boot/os.a(syssgi.o)` at 0x140
  **Inferred.** It is the largest routine in the slice, at 16704 bytes, and calls many helpers. Among the callees are `get_current_abi`, `cap_able` (many sites), `acl_get`, `nanotime`, `setmustrun`, `copyin_xlate`, `nfs_generic_cmd`, `syssgi_tune`, `syssgi_ash`, `syssgi_eventctr`, `path_to_handle` and `vproc_lookup`, and it calls `nosys` at several sites (for example 0x64c, 0x1ef8 and 0x1fcc). Its loads are mostly through the argument block at a0, and the branches compare the first word of that block with many constants. Role (from name): the `syssgi` system call dispatcher, a large switch over subcommands that each call a helper. I did not trace the individual subcommands, so this statement covers the dispatch shape only.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.30/usr/cpu/sysgen/IP35boot/os.a(time.o)` at 0xc0
  **Inferred.** It calls `splprof` at 0xe4, takes a mutex with `mutex_lock` at 0xf8 and releases it at 0x120, calls `chktimeout_tick` at 0x15c and again at 0x218, and converts the tick count with `tick_to_timeval` at 0x17c into the caller's buffer at a1 +16. It calls `splx` at 0x188 with the value returned by `splprof`. A separate path calls `splhi` at 0x198 and `splx` at 0x1cc with its result, and returns 0 at 0x1e4. Role (from name): the getitimer path, reading the interval timer state under a lock and returning the remaining time as a timeval. The timer semantics and the second `chktimeout_tick` call are not established.
  Reviewed: 2026-11-12 (sonnet)

- `part_discovery` in `6.5.30/usr/cpu/sysgen/IP35boot/partition.o` at 0x1f6c
  **Inferred.** It takes a semaphore on a .bss object with `cpsema` at 0x1f9c and 0x1fac, calls `get_nasid` at 0x20a8, and calls `part_probe_nasid` at 0x20b0, 0x20f8 and 0x2220. It guards state with `mutex_spinlock` and `mutex_spinunlock`, registers `part_activate` through `part_interrupt_set` at 0x219c (a1 = 0x2), starts a partition thread with `part_start_partition_thread` at 0x21ac, and calls `vector_read` at 0x21f0 with a0 = 0, a1 = 0, a2 = 0x600000 and a stack address in a3. Role (from name): partition discovery at boot, probing nodes and wiring an interrupt for activation. The exact partition states and what `vector_read` returns are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pic_attach2` in `6.5.30/usr/cpu/sysgen/IP35boot/pic.o` at 0x64c
  **Inferred.** It is a 3276-byte attach routine. It allocates state with `kmem_zalloc` and `kmem_alloc`, registers a character device with `hwgraph_char_device_add`, and records labeled info with `hwgraph_info_add_LBL` and `hwgraph_fastinfo_set`. It reads the bridge's identity and mode (`pcireg_id_get`, `pcireg_mode_get`, `pcireg_speed_get`, `pcireg_nic_addr`), looks up the I/O brick and its L1 controller (`is_iobrick`, `get_elsc`, `iobrick_l1type_get_nasid`, `iobrick_module_get_nasid`), and records board data with `eeprom_vertex_info_set`. Past that setup it builds per-slot device state (`pcibr_slot_info_init`, `pcibr_device_info_new`, `pcibr_slot_device_init`), programs the bridge's interrupt routing through `pcireg_intr_*` and `xtalk_intr_alloc` and `xtalk_intr_connect`, sets up address windows and read-response buffers (`pciio_device_win_map_new`, `pcibr_initial_rrb`, `pcibr_directmap_init`), and attaches devices with `pciio_device_attach`. The name and the callee set suggest attaching a PCI bridge to the hardware graph and bringing up its slots and interrupts. Several callees are unresolved in the disassembly output, so the branch structure is only partly known.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.30/usr/cpu/sysgen/IP35boot/xbridge.o` at 0x54
  **Inferred.** It is a 3484-byte attach routine that shares most of its callee set with `pic_attach2`. It finds the bridge pointer with `pcibr_bridge_ptr_get`, adds the bridge to the hardware graph with `hwgraph_path_add` and `hwgraph_char_device_add`, and registers it as a PCI I/O provider with `pciio_provider_register` and `pciio_provider_startup`. It also calls `hubii_ixtt_get` and `hubii_ixtt_set` (the facts do not show what state these access), and connects interrupts with `xtalk_intr_alloc` and `xtalk_intr_connect`. It reads identity and mode data (`pcireg_id_get`, `pcireg_mode_get`, `pcireg_speed_get`, `pcireg_nic_addr`), records NIC and vertex info (`nic_bridge_vertex_info`, `eeprom_vertex_info_set`), and allocates state with `kmem_zalloc` and `kmem_alloc`. Role (from name): attaching an Xbridge bridge ASIC, building its hardware-graph vertices, recording its identity and registering it with the PCI layer. The two attach routines look like paired paths for different bridge types, but this disassembly does not show which bridge type each one serves.
  Reviewed: 2026-11-12 (sonnet)
