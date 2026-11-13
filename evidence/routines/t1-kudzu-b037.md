# Routine role statements: 6.5.30 IP30 and IP32 boot objects

These role statements cover 23 routines in the 6.5.30 IP30 and IP32 boot objects. Each statement comes from the disassembly (registers, calls, constants) and cites addresses in the named binary. They describe what the software in those binaries does, not hardware behavior. Where the disassembly does not settle a role, the entry says so.

- `ecc_handler` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x17e54
  **Inferred.** It reads an error record through a1 (fields at +0x18, +0x238, +0x23c, +0x26 and +0x28 to +0x34) and branches on bits of the word at +0x18, testing masks 0x5500000, 0xaa00000, 0x12000000, 0x3800000 and 0x4000000 or 0x8000000. It drives cache operations through a dispatcher pointer at 0x17838 (ecc_cacheOp), calls ecc_force_cache_line_invalid at 0x18000, 0x18010 and 0x184b4, and calls maddr_to_dimm (0x183d8) and then sprintf into cacheErr_frames (0x1841c), with the maddr_to_dimm result as sprintf's fourth argument; what that value represents is not shown. The disassembly shows which masks gate which path but not the error class each mask names, so the classification stays partly open.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1bdf0
  **Inferred.** It loads from fixed addresses 0xffffffffaff50000 and 0xffffffffb8000000 and reads a halfword at 0xffffffffffffc3a0, stores a value at a0+0x140, and then tail-jumps to VEC_trap at 0x1be58 with a0, a1 and a3 unchanged. The name suggests an instruction-bus-error vector, which fits a stub that hands off to a common trap handler. The disassembly does not show what the loaded values mean, so the error type is inferred from the name only.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.30/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** It calls wchanmeter at 0x38b0 with the syncinfo symbol. When wchanmeter returns nonzero, the branch at 0x38b8 goes to a block at 0x395c that calls mrmeter_entry with tag 5 at 0x3974 (first argument the wchanmeter result) and then continues at 0x38c0; when it returns zero it goes straight there. It calls mr_access at 0x38c0, and the branch at 0x38c8 skips the rest when the wchanmeter result is zero. Otherwise it updates fields at offsets +0x1c0 to +0x1d0 of the record that wchanmeter returns and calls mrmeter_getentry (tag 5) at 0x391c. It returns the value from mr_access or mrmeter_getentry. The name suggests a metered read-access path for a synchronization object; the tag meanings are not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.30/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x39cc
  **Inferred.** It calls wchanmeter at 0x39ec and mr_update at 0x39fc. When wchanmeter returns nonzero, the branch at 0x39f4 goes to a block that calls mrmeter_entry with tag 6 at 0x3a94 and then continues at mr_update (0x39fc). One return path returns the result of mr_update; another returns a value the facts do not name. Role (from name): the update-side counterpart of `_mraccess`. The paths that skip the meter call are not traced here.
  Reviewed: 2026-11-12 (sonnet)

- `MgrasGeneralInterrupt` in `6.5.30/usr/cpu/sysgen/IP30boot/mgras.a(mgras.o)` at 0x8008
  **Inferred.** It follows the pointer at a0+208 to a device record, with state at offsets +69664 and +69696, and calls vsema on a0+1944, a0+1608 and a0+1592; each of those calls is followed by a store at +0x11028. The values 0x10000, 0x10000000, 0x80000 and 0x20000000 sit in registers around those calls, but the facts do not show them used as arguments or what they mean. It calls cmn_err_tag with codes 0xaa5 to 0xaa9 and cmn_err, and calls MgrasSchedSwapBufFinish at 0x80a8 and mgras_error at 0x816c. Role (from name): an interrupt handler; the masks tested on the status value are not named in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyAttach` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_attach.o)` at 0x68
  **Inferred.** It calls gfxdd_recycle with the odsy_regmatch symbol at 0xf4. It calls gfxdd_mmap at 0x17c with gfx_fault as its fourth argument. When gfxdd_mmap returns zero it brackets a call to gf_OdsyUnMapGfx at 0x1a4 with splhi and splx (0x18c, 0x1ac) and returns 0; when it returns nonzero the branch at 0x184 goes to a cmn_err call at 0x1d8. Its returns are 0, 0xc and 0x16, which look like ENOMEM and EINVAL style error numbers; that reading is an inference. What the mapping covers is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `gfP_OdsySaveHwShadow` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ctxsw.o)` at 0xc0
  **Inferred.** It calls RRM_CheckValidFault at 0x100 with its four incoming arguments and tests flow attachment through odsy_flow_am_i_attached and gfx_flow_am_i_attached, calling gfx_flow_detach_me and gfx_flow_attach_me on some paths. When odsy_flow_am_i_attached returns nonzero (the branch at 0x154 skips this otherwise) it calls timeout with odsy_ctxsw_timeout and 1500 (0x190) and later untimeout_wait on the result (0x1a4). An indirect call at 0x210 has a target the disassembly does not identify, so what it does and the exact save sequence stay open. Role (from name): a context-switch save path.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyDestroyDDRN` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ddrn.o)` at 0x438
  **Inferred.** It tears down one per-context record. It calls gf_OdsyUnSchedSwapBuf at 0x4a0 only when the word at a2+28 is not negative, and gf_OdsyUnSchedRetraceEvent at 0x4ac. When the word at offset 408 of the pointer at a2+112 is zero, it first calls odsyDMAFinish, odsyMMdetach and odsyUnpinAndUnmapGctxRegions (0x644 to 0x658), stores 1 in that word and then continues at 0x4c0; a nonzero word skips those three calls. It then calls odsy_hpipe_cleanup (0x4c4), frees a 0x4000-byte block (kmem_free at 0x4e0, only when the doubleword at offset 192 of that pointer is nonzero) and a 32K slot (OdsyFree32kSlot at 0x4ec). Only when the word at offset 28480 of a0 is 1 does it go on to the flow handling: gfx_flow_detach_me (0x570) is called only if the word at +4 of the record has bits 0x3 set and gfx_flow_am_i_attached returns nonzero, and gfx_flow_dealloc (0x590) only if the pointer tested at 0x588 is nonzero. The expansion of the DDRN name is not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyInitialize` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x1284
  **Inferred.** It calls htp_register_board at 0x12f8 (all four arguments zero). Unless the branch at 0x1464 skips them, it calls odsyReset at 0x146c and then gfx_flow_alloc at 0x1498, passing odsyHiwaterInterrupt as the sixth argument (a5). It then calls the 32K slot routines, odsyInitMM, odsyInitDMA, odsyInitRetrace and odsyInitInterrupts (0x14cc to 0x1520). When the word at offset 28480 of the record is nonzero, a separate path calls init_sema and starts a 500-tick timeout (odsy_init_ddrncount_timeout at 0x16f4) on the same semaphore, followed by psema. The routine returns the result of odsyInitCfifo or -1. The order comes from the call sequence, not from a documented procedure.
  Reviewed: 2026-11-12 (sonnet)

- `odsyLowLevelReset` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x1830
  **Inferred.** It makes a series of timed waits: odsy_usdelay with 0xf4240 (1,000,000) at 0x1ad8, which the branch at 0x1ab0 skips on one path, us_delay with 0x30d40 (200,000) at 0x1b14, odsy_usdelay with 0x3e8 (1,000) at 0x1b38 and with 0x186a0 (100,000) at 0x1b7c; the last follows the tests at 0x1b48 and 0x1b58, which otherwise branch to 0x1bd0. It calls odsyLLPReset at 0x1b2c and calls cmn_err at 0x1bd4, the target of those two branches. The disassembly shows the delays and the call order, but not why each delay is needed.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.30/usr/cpu/sysgen/IP30boot/os.a(pda.o)` at 0x1810
  **Inferred.** It reads flag bytes and words at fixed low addresses (0xffffffffffffc080 to 0xffffffffffffc0a0). On the path where the first flag byte is nonzero it calls panicspin at 0x1820. It calls tlbflush_rand with 1 at 0x1848 only when the word at c084 is nonzero, and calls tlbflush at 0x185c only when the doubleword at c0a0 is nonzero. Role (from name): a vector or TLB routine; the flags are not named in the disassembly, so the role is only partly read.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.30/usr/cpu/sysgen/IP30boot/os.a(syssgi.o)` at 0xd4
  **Inferred.** It calls get_current_abi at 0xf8. When the word at sat_enabled is nonzero, the branch at 0x10c first runs sat_never and sat_set_subsysnum (0x2d0c, 0x2d14) and then rejoins. It then tests a value loaded from the first argument against 0x4ed: below that it makes a tail call through a table in the read-only data section indexed by that value, and otherwise it returns 0x16 (0x164). The table targets are not identified in this disassembly, so the per-request behavior is open. Role (from name): the SGI system-call entry; the disassembly alone does not confirm that role for every path.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.30/usr/cpu/sysgen/IP30boot/os.a(time.o)` at 0xc0
  **Inferred.** The branch at 0xdc splits two paths on its first argument. When it is zero the routine calls splprof (0xe4), takes a mutex at offset +2144 of the record pointed to by a2+752 (mutex_lock at 0xf8, mutex_unlock at 0x120) and calls chktimeout_tick with first argument 0 (0x15c) or 2 (0x1f4), chosen by the tests at 0x13c and 0x14c. It converts the result with tick_to_timeval at 0x178, with the third argument 0x989680 on the first path and 0x50 on the second, and calls splx (0x184). When it is nonzero it calls splhi (0x194), copies four doublewords and calls splx (0x1c8), without the mutex. It returns 0. Role (from name): the getitimer behavior; the disassembly shows offsets rather than named fields.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_vect_set` in `6.5.30/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x1455c
  **Inferred.** It reads a 64-bit value at a0+8, a halfword at a0+0x72 and a word at a0+4, and checks the argument a1 against the range 0 to 7 (0x145a0, 0x145ac), sending an out-of-range value to a failure path that calls assfail at 0x14774 (line 0x85b). It then dispatches a selector against the constants 0x2 and 0x3 and a set of values including 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002; a selector equal to -1 reaches panic at 0x14788. The facts show doubleword and word stores at 0x14620, 0x14638, 0x14654 and 0x14668, so the routine does write; their targets and the meaning of the selector values are not established.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_addr_set` in `6.5.30/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x149c8
  **Inferred.** This routine has the same argument checks and the same constant dispatch as `pcireg_intr_addr_vect_set` at 0x1455c, and it fails through assfail at 0x14bf0 (line 0x89a) and panic at 0x14c04. The facts show doubleword and word stores at 0x14a90, 0x14aa8, 0x14ac8 and 0x14ae4, so it writes; they do not show which register the address field maps to. Role (from name): the address counterpart of the vector routine.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.30/usr/cpu/sysgen/IP30boot/xbridge.o` at 0x54
  **Inferred.** It gets the bridge pointer with pcibr_bridge_ptr_get at 0x84 and registers a PCI I/O provider with pciio_provider_register at 0xec and pciio_provider_startup at 0xf4. It adds a hardware-graph path and character device (0xe0, 0x118), reads the NIC address, the ID and the hints (pcireg_nic_addr, pcireg_id_get, pcibr_hints_get), and allocates a 0xab8-byte (2744-byte) per-bridge record with kmem_zalloc at 0x1bc. It copies the vertex name into a kmem_alloc'd string at 0x204 and 0x214. It returns 0 or -1.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.30/usr/cpu/sysgen/IP32boot/cn.o` at 0x804
  **Inferred.** It calls gfx_earlyinit at 0x820 first. Unless the halfword at cn+36 is zero (the branch at 0x834 skips this), it then calls du_init (0x8a8 or 0x8b8, depending on the tests at 0x868 to 0x8a0) or tp_init (0x954). It then sets up a spinlock and a mutex on the console record (0x8d0 and 0x8e8) and checks the console argument with is_specified at 0x904. The ordering between the graphics and serial initialization is read from the call sequence. Role (from name): a console front end; the device identities are not established.
  Reviewed: 2026-11-12 (sonnet)

- `dummyret0_func` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x13f0
  **Established.** The 8-byte body has no loads, stores or calls, and returns 0. The function table lists this address as getcpuid, but the symbol here is dummyret0_func, so the disassembly does not confirm the table's name. The stub behavior is established; any CPU-identification role is not.
  Reviewed: 2026-11-12 (sonnet)

- `_get_timestamp` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1400
  **Inferred.** The 8-byte body is a single tail jump to get_r4k_counter at 0x1400, with its arguments passed unchanged. The name fits a timestamp accessor that reads the R4000-style counter, but the disassembly shows only the jump.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1a10
  **Inferred.** This is an early C entry point. It calls _check_dbg at 0x1a58 with argc, argv and envp, calls debug at 0x1a94 only when the word at dbgmon_loadstop is nonzero (a zero word skips that call at the branch at 0x1a88), then mlsetup with argc and the constant 10 (fourth argument) at 0x1b3c, and finally main at 0x1b4c. Its role as the entry point rests on that call order. The branch paths between these calls are not expanded here.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x8738
  **Inferred.** It reads a doubleword at 0xffffffffb4000008 and writes several fixed addresses in the 0xffffffffb4000000 region (0x8760, 0x8768, 0x87ac to 0x87bc, 0x887c). It calls is_enabled on arg_triton_invall, get_crimerev, get_cpu_irr and flushbus. It installs handlers with setkgvector for ackkgclock (0x87e8) and with setcrimevector for vectors 0x1c, 0x1d, 0x15 and 0x14 (0x8804, 0x8820, 0x8840, 0x8858), bound to synthclock, synthackkgclock and buserror_intr (for both 0x15 and 0x14). On some paths it stores into wait_for_interrupt_fix_loc and calls __dcache_wb_inval and __icache_inval on it (0x890c to 0x8934), and it returns the result of mh16550_earlyinit. The region at 0xb4000000 is not named in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb1a8
  **Inferred.** Its only call is cmn_err at 0xb1b8, with level 0, a message string at .rodata+3184, and its own a2 and a3 passed through, and it returns that call's result. The name suggests sending an interrupt, but the disassembly shows only a logging call, so the interrupt role is not established.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb5f4
  **Inferred.** It calls init_timer through the pointer at 0xb52c with its own four arguments (0xb604), but only when the word at sdata+156 is nonzero: the branch at 0xb5fc skips to 0xb60c when it is zero. On both paths it then makes an indirect call through a register at 0xb610 whose target is not identified. The indirect target is the part this reading cannot name.
  Reviewed: 2026-11-12 (sonnet)
