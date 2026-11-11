# Routine role statements: 6.5.30 IP30 and IP32 boot objects

These role statements cover 23 routines in the 6.5.30 IP30 and IP32 boot objects. Each statement comes from the disassembly (registers, calls, constants) and cites addresses in the named binary. They describe what the software in those binaries does, not hardware behavior. Where the disassembly does not settle a role, the entry says so.

- `ecc_handler` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x17e54
  **Inferred.** It reads an error record through a1 (fields at +0x18, +0x238, +0x23c, +0x26 and +0x28 to +0x34) and branches on bits of the word at +0x18, testing masks 0x5500000, 0xaa00000, 0x12000000, 0x3800000 and 0x4000000 or 0x8000000. It drives cache operations through a dispatcher pointer at 0x17838 (ecc_cacheOp), calls ecc_force_cache_line_invalid at 0x18000, 0x18010 and 0x184b4, and formats the faulting address with maddr_to_dimm (0x183d8) and sprintf into cacheErr_frames (0x1841c). The disassembly shows which masks gate which path but not the error class each mask names, so the classification stays partly open.

- `VEC_ibe` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1bdf0
  **Inferred.** It loads from fixed addresses 0xffffffffaff50000 and 0xffffffffb8000000 and reads a halfword at 0xffffffffffffc3a0, stores a value at a0+0x140, and then tail-jumps to VEC_trap at 0x1be58 with a0, a1 and a3 unchanged. The name suggests an instruction-bus-error vector, which fits a stub that hands off to a common trap handler. The disassembly does not show what the loaded values mean, so the error type is inferred from the name only.

- `_mraccess` in `6.5.30/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** It calls wchanmeter at 0x38b0 with the syncinfo symbol, then mr_access at 0x38c0, and on another path mrmeter_getentry at 0x391c and mrmeter_entry with tag 5 at 0x3974. It updates fields at offsets +0x1c0 to +0x1d0 of the record that wchanmeter returns, and returns the value from mr_access or mrmeter_getentry. The name suggests a metered read-access path for a synchronization object; the tag meanings are not in the disassembly.

- `_mrupdate` in `6.5.30/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x39cc
  **Inferred.** It is the update-side twin of `_mraccess`. It calls wchanmeter at 0x39ec, mr_update at 0x39fc and mrmeter_entry with tag 6 at 0x3a94, and returns the result of mr_update. The paths that skip the meter call are not traced here.

- `MgrasGeneralInterrupt` in `6.5.30/usr/cpu/sysgen/IP30boot/mgras.a(mgras.o)` at 0x8008
  **Inferred.** It follows the pointer at a0+208 to a device record, with state at offsets +69664 and +69696, and signals three semaphores (vsema on a0+1944, a0+1608 and a0+1592) with flag masks 0x10000, 0x10000000, 0x80000 and 0x20000000. It reports error cases through cmn_err_tag with codes 0xaa5 to 0xaa9 and through cmn_err, and calls MgrasSchedSwapBufFinish at 0x80a8 and mgras_error at 0x816c. Reading it as a status-dispatching interrupt handler follows from the call pattern; the masks are not named in the disassembly.

- `gf_OdsyAttach` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_attach.o)` at 0x68
  **Inferred.** It calls gfxdd_recycle with the odsy_regmatch symbol at 0xf4, maps the graphics window with gfxdd_mmap at 0x17c using gfx_fault as the fault handler, and unmaps through gf_OdsyUnMapGfx at 0x1a4. It brackets the unmap with splhi and splx at 0x18c and 0x1ac and logs through cmn_err at 0x1d8. Its returns are 0, 0xc and 0x16, which look like ENOMEM and EINVAL style error numbers; that reading is an inference.

- `gfP_OdsySaveHwShadow` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ctxsw.o)` at 0xc0
  **Inferred.** It checks a fault with RRM_CheckValidFault at 0x100, tests and changes flow attachment through gfx_flow_am_i_attached, gfx_flow_detach_me and gfx_flow_attach_me, and arms a 1500-tick timeout (odsy_ctxsw_timeout at 0x190) that it cancels with untimeout_wait at 0x1a4. An indirect call at 0x210 appears to do the state transfer, and its target is not identified by the disassembly, so the exact save sequence stays open. The name matches a context-switch save path.

- `gf_OdsyDestroyDDRN` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ddrn.o)` at 0x438
  **Inferred.** It tears down one per-context record. It unschedules the swap-buffer and retrace events (0x4a0, 0x4ac), cleans up the hpipe state (odsy_hpipe_cleanup at 0x4c4), frees a 0x4000-byte block (kmem_free at 0x4e0) and a 32K slot (OdsyFree32kSlot at 0x4ec), then detaches the graphics flow and deallocates it (0x570, 0x590). It finishes with odsyDMAFinish, odsyMMdetach and odsyUnpinAndUnmapGctxRegions at 0x644 to 0x658. The expansion of the DDRN name is not in the disassembly.

- `gf_OdsyInitialize` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x1284
  **Inferred.** It registers a board with htp_register_board at 0x12f8 and allocates a graphics flow whose handler is odsyHiwaterInterrupt (gfx_flow_alloc at 0x1498). It then sets up 32K slots, memory management, DMA, retrace and interrupts (0x14cc to 0x1520), resets the device through odsyReset at 0x146c, and starts a 500-tick timeout (odsy_init_ddrncount_timeout at 0x16f4) on a semaphore. The routine returns the result of odsyInitCfifo or -1. The order comes from the call sequence, not from a documented procedure.

- `odsyLowLevelReset` in `6.5.30/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x1830
  **Inferred.** It runs a series of timed waits: odsy_usdelay for 1,000,000 us at 0x1ad8, us_delay for 200,000 at 0x1b14, odsy_usdelay for 1,000 at 0x1b38 and for 100,000 at 0x1b7c. It calls odsyLLPReset at 0x1b2c and reports through cmn_err at 0x1bd4. The disassembly shows the delays and the call order, but not why each delay is needed.

- `doacvec` in `6.5.30/usr/cpu/sysgen/IP30boot/os.a(pda.o)` at 0x1810
  **Inferred.** It reads flag bytes and words at fixed low addresses (0xffffffffffffc080 to 0xffffffffffffc0a0). On the path where the first flag byte is nonzero it calls panicspin at 0x1820. It calls tlbflush_rand with 1 at 0x1848, and calls tlbflush at 0x185c only when the doubleword at c0a0 is nonzero. The name suggests a vector or TLB routine, but the flags are not named in the disassembly, so the role is only partly read.

- `syssgi` in `6.5.30/usr/cpu/sysgen/IP30boot/os.a(syssgi.o)` at 0xd4
  **Inferred.** It calls get_current_abi at 0xf8, then dispatches through a jump table in the read-only data section, indexed by a value loaded from the first argument. Audit-style calls sat_never and sat_set_subsysnum at 0x2d0c and 0x2d14 sit on some paths, and one return path yields 0x16 at 0x164. The table targets are not identified in these disassembly, so the per-request behavior is open. The syssgi name points to the SGI system-call entry, but the disassembly alone do not confirm that role for every path.

- `dogetitimer` in `6.5.30/usr/cpu/sysgen/IP30boot/os.a(time.o)` at 0xc0
  **Inferred.** It takes a mutex at offset +2144 of a per-CPU record (0xf8, unlocked at 0x120) and calls chktimeout_tick twice (0x15c and 0x1f4) to compute the remaining time. It converts that value with tick_to_timeval at 0x178, choosing between the constants 0x50 and 0x989680, and brackets the work with splprof, splhi and splx. It returns 0. The name matches the getitimer behavior, but the disassembly shows offsets rather than named fields.

- `pcireg_intr_addr_vect_set` in `6.5.30/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x1455c
  **Inferred.** It reads a 64-bit value at a0+8, a halfword at a0+0x72 and a word at a0+4, and checks the argument a1 against the range 0 to 7 (0x145a0, 0x145ac). It then dispatches a selector against the constants 0x2 and 0x3 and a set of values including 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002. Out-of-range cases reach assfail at 0x14774 (line 0x85b) and panic at 0x14788. The disassembly list no store instructions, so the routine computes and checks a value rather than writing it. What the selector values name is not established.

- `pcireg_intr_addr_addr_set` in `6.5.30/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x149c8
  **Inferred.** This routine has the same argument checks and the same constant dispatch as `pcireg_intr_addr_vect_set` at 0x1455c, and it fails through assfail at 0x14bf0 (line 0x89a) and panic at 0x14c04. The disassembly does not show which register the address field maps to, and no store appears in the disassembly, so the role is inferred from its pair with the vector routine.

- `xbridge_attach` in `6.5.30/usr/cpu/sysgen/IP30boot/xbridge.o` at 0x54
  **Inferred.** It gets the bridge pointer with pcibr_bridge_ptr_get at 0x84 and registers a PCI I/O provider with pciio_provider_register at 0xec and pciio_provider_startup at 0xf4. It adds a hardware-graph path and character device (0xe0, 0x118), reads the NIC address, the ID and the hints (pcireg_nic_addr, pcireg_id_get, pcibr_hints_get), and allocates a 0xab8-byte (2744-byte) per-bridge record with kmem_zalloc at 0x1bc. It copies the vertex name into a kmem_alloc'd string at 0x204 and 0x214. It returns 0 or -1.

- `cn_init` in `6.5.30/usr/cpu/sysgen/IP32boot/cn.o` at 0x804
  **Inferred.** It calls gfx_earlyinit at 0x820 first, then initializes a console device with du_init (0x8a8 and 0x8b8) and tp_init (0x954), sets up a spinlock and a mutex on the console record (0x8d0 and 0x8e8), and checks the console argument with is_specified at 0x904. The ordering between the graphics and serial initialization is read from the call sequence. The role fits a console front end, but the device identities are not established.

- `dummyret0_func` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x13f0
  **Established.** The 8-byte body has no loads, stores or calls, and returns 0. The function table lists this address as getcpuid, but the symbol here is dummyret0_func, so the disassembly does not confirm the table's name. The stub behavior is established; any CPU-identification role is not.

- `_get_timestamp` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1400
  **Inferred.** The 8-byte body is a single tail jump to get_r4k_counter at 0x1400, with its arguments passed unchanged. The name fits a timestamp accessor that reads the R4000-style counter, but the disassembly shows only the jump.

- `start` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1a10
  **Inferred.** This is an early C entry point. It calls _check_dbg at 0x1a58 with argc, argv and envp, calls debug at 0x1a94, then mlsetup with argc and the constant 10 at 0x1b3c, and finally main at 0x1b4c. Its role as the entry point rests on that call order. The branch paths between these calls are not expanded here.

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x8738
  **Inferred.** It reads a word at 0xffffffffb4000008 and writes several fixed addresses in the 0xffffffffb4000000 region (0x8760, 0x87ac to 0x87bc, 0x887c, 0x89a0). It calls is_enabled on arg_triton_invall, get_crimerev, get_cpu_irr and flushbus. It installs handlers with setkgvector for ackkgclock (0x87e8) and with setcrimevector for vectors 0x1c, 0x1d and 0x15 (0x8804, 0x8820, 0x8840), bound to synthclock, synthackkgclock and buserror_intr. It patches a wait-for-interrupt sequence through wait_for_interrupt_fix_loc (0x890c to 0x892c) and returns the result of mh16550_earlyinit. The region at 0xb4000000 is not named in the disassembly.

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb1a8
  **Inferred.** Its only call is cmn_err at 0xb1b8, with level 0, a message string at .rodata+3184, and its own a2 and a3 passed through, and it returns that call's result. The name suggests sending an interrupt, but the disassembly shows only a logging call, so the interrupt role is not established.

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb5f4
  **Inferred.** It calls init_timer through the pointer at 0xb52c with its own four arguments (0xb604). It then makes an indirect call through a register at 0xb610 whose target is not identified. A branch at 0xb5fc tests the word at sdata+156 and jumps to just before that indirect call, so the test decides the path into it. The indirect target is the part this reading cannot name.
