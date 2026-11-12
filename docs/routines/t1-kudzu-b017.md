# Routine role statements, kudzu batch 17

These statements cover 25 routines from the IP30 and IP32 kernel objects of IRIX 6.5.22. Each one is written from the routine's instruction facts: the registers it touches, the calls it makes and the constants it uses. The facts describe what the code does, not what the hardware must do. A grade is inferred unless the whole body was read and the behaviour is fully established. Where a callee, constant or register is not identified, the statement says so.

- `gf_OdsyAttach` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_attach.o)` at 0x68
  **Inferred.** The attach step for the Odyssey graphics device. It maps a window with gfxdd_mmap at 0x17c, with gfx_fault as the fault handler, and calls gfxdd_recycle at 0xf4. It brackets the work with splhi and splx (0x18c, 0x1ac), and the calls at 0x1a4 and 0x1d8 sit after the mmap result test. The routine returns 0, 0xc or 0x16 in v0; the meaning of the constant 0xffffffffaff00000 and the flag masks at 0x120 and 0x154 are not identified.

- `gfP_OdsySaveHwShadow` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ctxsw.o)` at 0xc0
  **Inferred.** This copies Odyssey hardware state into a shadow record around a context switch. It runs RRM_CheckValidFault at 0x100, tests odsy_flow_am_i_attached and gfx_flow_am_i_attached, and arms a timeout (odsy_ctxsw_timeout, constant 0x5dc) at 0x190 that it cancels with untimeout_wait at 0x1a4. Ten of its stores go to the record at a3+0, and many others go to fields behind the device pointer at offset 40. An indirect call at 0x210 takes seven arguments and its target is not resolved, so the full save sequence is not established.

- `gf_OdsyDestroyDDRN` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ddrn.o)` at 0x430
  **Inferred.** This tears down a DDRN object; the facts do not say what the DDRN abbreviation stands for. It unschedules the swap-buffer and retrace callbacks (gf_OdsyUnSchedSwapBuf at 0x498, gf_OdsyUnSchedRetraceEvent at 0x4a4), cleans the hardware pipe with odsy_hpipe_cleanup at 0x4bc, and frees a 0x4000-byte block with kmem_free at 0x4d8 only when the doubleword at offset 192 of the pointer at a2+112 is nonzero. When the word at offset 408 of that same pointer is zero, it runs the DMA finish, MM detach and unpin-and-unmap steps (0x618, 0x624, 0x62c) and sets that word to 1; a nonzero word skips them. It ends with an indirect call at 0x670 whose target is unresolved followed by a cmn_err at 0x68c.

- `gf_OdsyInitialize` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x1250
  **Inferred.** The top-level bring-up of the Odyssey device. It registers the board with htp_register_board at 0x12c4, allocates a graphics flow with gfx_flow_alloc at 0x1468 (event handler odsyHiwaterInterrupt), and then runs odsyInitCfifo, odsyReset, odsyInitMM, odsyInitDMA, odsyInitRetrace and odsyInitInterrupts in that order. It also makes a delay call with the constant 0x2710 at 0x141c and a semaphore wait with a timeout (odsy_init_ddrncount_timeout, constant 0x1f4), and it returns either the odsyInitCfifo result or -1.

- `odsyLowLevelReset` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x17fc
  **Inferred.** This resets the Odyssey hardware. It reads and writes a fixed set of addresses in the 0x9000000010000000 range, compares one value against 0xd100 (0x1830), and inserts delays with the constants 0xf4240, 0x30d40, 0x3e8 and 0x186a0. It calls odsyLLPReset at 0x1aec to reset the link layer. The facts show the order of the writes but not what each address controls.

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP30boot/os.a(pda.o)` at 0x176c
  **Inferred.** This reads per-processor flag bytes at fixed negative absolute offsets (-0x3f80 and -0x3f7f, a word at -0x3f7c, a doubleword at -0x3f60) and branches on them. It calls panicspin at 0x177c when the first flag byte is nonzero, and otherwise performs tlbflush_rand with 1 (0x17a4) or tlbflush (0x17b8). The name suggests a vector action, but nothing in the facts fixes what the flags mean.

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP30boot/os.a(syssgi.o)` at 0xd4
  **Inferred.** This is the entry for the syssgi system call, and only its entry path is reachable in these facts. It calls get_current_abi at 0xf4, then compares the doubleword at the start of the argument block against 0x4e3 (unsigned) at 0x124. A value at or above that limit takes the branch to 0x138, and the only return on the reachable path has v0 = 0x16 at 0x160. Otherwise an indirect jump at 0x130 selects a handler from a table whose entries are unresolved, so the role of each command is unknown.

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP30boot/os.a(time.o)` at 0xc0
  **Inferred.** This copies four 8-byte fields from a timer structure reached through a2+0x2f0 into the caller buffer at a1+0 through a1+0x18. The fields come from offsets 0x840 to 0x858 of that structure. It takes and releases a mutex (mutex_lock at 0xf8, mutex_unlock at 0x120), calls chktimeout_tick at 0x15c and 0x1f4, and converts a tick count with tick_to_timeval at 0x178 using the constants 0x50 and 0x989680. Which timer each field belongs to is not identified.

- `pcireg_intr_addr_vect_set` in `6.5.22/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x14534
  **Inferred.** This writes the vector part of one PCI bridge interrupt register, selected by the index in a1. It skips its body when a0 is null, and it calls assfail with the constant 0x85b (0x1474c) when the index is outside 0 to 7. A separate path reaches panic (0x14760). A chain of compares on a4 against the constants 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002 picks the offset to write; the bridge registers those constants stand for are not identified here.

- `pcireg_intr_addr_addr_set` in `6.5.22/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x149a0
  **Inferred.** This has the same shape as the vector setter at 0x14534: a null check on a0, an index check against 0 to 7 with an assertion (constant 0x89a, call at 0x14bc8) and a panic, and the same chain of compares against 0xe000 through 0xc002. It differs in the offsets it stores to, which suggests it writes the address half of the interrupt register. That pairing is inferred from the matching structure, not from a call between the two routines.

- `xbridge_attach` in `6.5.22/usr/cpu/sysgen/IP30boot/xbridge.o` at 0x54
  **Inferred.** This is the attach routine for the Xbridge PCI bridge widget. It allocates and zeroes a 0xab8-byte state block with kmem_zalloc and memset, registers the graph vertex and the PCI provider (hwgraph_char_device_add at 0x118, pciio_provider_register with pci_xbridge_provider at 0xec), and sets up the error and interrupt path (xwidget_error_register with pcibr_error_handler at 0x440, xtalk_intr_connect with pcibr_error_intr_handler at 0x574). It maps device windows with the constants 0x10000000 and 0x40000000 as arguments, allocates translation entries with rmallocmap at 0x4c4, and loops over the slots bounded by bytes at +56 and +57 of the state block to initialize and attach them. Compares against revision-like constants (0xc0021 to 0xc0024, 0xd0021, 0xd0022) gate some feature setup, and those constants are not identified.

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP32boot/cn.o` at 0x804
  **Inferred.** This sets up the IP32 console. It calls gfx_earlyinit at 0x820, releases a vnode reference with vn_rele at 0x854 when one is held, and initializes the drivers du_init (0x8a8, 0x8b8) and tp_init (0x954) along with spinlock_init and init_mutex on the console record. It chooses a console by comparing the first byte of the console argument with 0x67, 0x47, 0x64 and 0x44 (0x924 to 0x940), then checks the second byte against 0x32 and the third against zero. The device each choice selects is not identified in the facts.

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x13e8
  **Established.** The function table lists this routine as getcpuid, but the symbol at this address in this object is dummyret0_func. The body is 8 bytes: it returns with v0 = 0x0 and touches nothing else. The body does not show whether this is the CPU-id routine the function table names.
  Grade scope: established for the body; the name mismatch is noted, not resolved.

- `_get_timestamp` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x13f8
  **Inferred.** The body is an 8-byte tail jump to get_r4k_counter, which receives the 4 argument registers unchanged. The return value is whatever get_r4k_counter returns, and the facts do not show it. The timestamp reading comes from the callee's name, so it is inferred.
  Grade scope: established for the jump; inferred for the timestamp role.

- `start` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1a00
  **Inferred.** This is the IP32 kernel entry. It stores the startup values argc, argv and envirn into _argc, _argv and _envirn, reads the dbgmon_loadstop flag, and calls _check_dbg at 0x1a48 and debug at 0x1a84. It writes the doubleword at 0xffffffffb4000018, which mlreset also writes, then calls mlsetup with argc and the constant 0xa at 0x1b2c and main at 0x1b3c. A series of tests on a3 (0x1ac8 to 0x1af8) skip to 0x1b0c, and what they check is not identified.

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1bd8
  **Established.** This is an 8-byte leaf: a bare return that touches no memory and makes no call. The routine does not write v0, so the caller's value is what comes back. The name suggests it mutes audio, but the body does nothing that would.
  Grade scope: established for the body.

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x8728
  **Inferred.** This is the IP32 machine-level setup. It reads and writes registers in the 0xffffffffb4000000 range and checks the revision with get_crimerev at 0x8754. It programs interrupt vectors: setkgvector for ackkgclock at 0x87d8, setcrimevector for the synthclock, synthackkgclock and buserror_intr handlers (0x87f4 to 0x8848), and setmaceisavector for powerintr at 0x885c. It also sets up the system id and NVRAM table, writes the PCI prefetch enable values with pciio_pio_write32 (0x8950 to 0x89b8), and runs cache writeback and invalidate over wait_for_interrupt_fix_loc (0x8918, 0x8924). The meaning of the vector numbers and of the constant 0x28 is not identified.

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb1a0
  **Established.** The body is 1 call to cmn_err with severity 0x0 and a string constant, passing a2 and a3 through, and the routine returns cmn_err's result. Despite the name, it sends no interrupt.
  Grade scope: established for the body.

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb5ec
  **Inferred.** This calls init_timer through the function pointer at 0xb524, passing its arguments through. A flag at .sdata+156 skips the init_timer call at 0xb5fc when the flag is zero. The indirect call at 0xb608, whose target is not resolved, runs either way. Its shape matches slowkgclock and ackkgclock, and the facts show nothing that separates them except their offsets.

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb61c
  **Inferred.** This has the same shape as startkgclock: init_timer through the pointer at 0xb524 (0xb62c), the .sdata+156 flag test at 0xb624, and an unresolved indirect call at 0xb638. The name suggests a slower clock setup, but the body shows no difference that would support it.

- `ackkgclock` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb710
  **Inferred.** This has the same shape again: init_timer through the pointer at 0xb524 (0xb720), the .sdata+156 flag test at 0xb718, and an unresolved indirect call at 0xb72c. The name suggests an acknowledge path, but no acknowledge register is accessed in the facts, so that reading is not established.

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xc158
  **Inferred.** This applies a time-of-day adjustment taken from a0, whose sign it tests at 0xc17c. It brackets its work with splhi and splx using the constants 0x4650 and 0x190, and it loops comparing a value against 0xafc80 (0xc228). The units of those constants are not identified, and the time-of-day reading rests on the name.

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xd88c
  **Inferred.** This is the ECC error handler for the IP32 kernel. It disables the secondary cache with _r4600sc_disable_scache at 0xd8f0 and re-enables it at 0xddd4, reads the error registers at 0xb4000040 through 0xb4000258 into an error record, and logs the event through ecc_log_error (code 6, 0xdb30). It then calls ecc_assign_msg, arms a timeout to ecc_cleanup, and repairs with ecc_fixmem and ecc_fixcache through pointers at 0xdd60 and 0xdd78. Which register bits select memory repair or cache repair, and what the constants 0x240000 and 0x10000000 mean, are not identified.

- `dobuserr_common` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1074c
  **Inferred.** This reports bus errors for the IP32 kernel. It reads the CPU and memory error status words and their addresses, picks a report format from the CRIME revision (print_buserr_rev1 at 0x107b8 or print_buserr_rev0 at 0x10944), and prints CPU and memory errors through cmn_err and cmn_err_tag. A VICE error address goes through vice_err at 0x10894, and a memory error goes through log_ecc_err (0x10978), ecc_error_correction (0x1098c) and a secondary cache re-enable (0x109ac). The masks 0x78080 and 0xe000000 and the severity and tag numbers are not identified.

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10ea4
  **Inferred.** This reads a 64-bit word at 0xffffffffbf310008, branches on whether a0 is zero (0x10eb0), and writes the word back at 0x10ec8. The facts show no constant that the stored value comes from beyond that test, so which LED bits change is not established. The address suggests a board register, but the facts do not identify it.
