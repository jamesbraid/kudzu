# Routine role statements, kudzu batch 17

These statements cover 25 routines from the IP30 and IP32 kernel objects of IRIX 6.5.22. Each one is written from the routine's instruction facts: the registers it touches, the calls it makes and the constants it uses. The facts describe what the code does, not what the hardware must do. A grade is inferred unless the whole body was read and the behaviour is fully established. Where a callee, constant or register is not identified, the statement says so.

- `gf_OdsyAttach` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_attach.o)` at 0x68
  **Inferred.** Role (from name): the attach step for the Odyssey graphics device. It calls gfxdd_recycle at 0xf4 with odsy_regmatch as the first argument and the constant 0xffffffffaff00000 as the fifth (a4). It calls gfxdd_mmap at 0x17c, passing gfx_fault as its fourth argument. When gfxdd_mmap returns zero the code reaches splhi (0x18c), gf_OdsyUnMapGfx (0x1a4) and splx (0x1ac); a nonzero result branches at 0x184 to the code at 0x1d4, which reaches cmn_err at 0x1d8. The routine returns 0, 0xc or 0x16 in v0. The meaning of the constant 0xffffffffaff00000, of the flag test at 0x120 (mask 0x24) and of the unsigned compare at 0x154 is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `gfP_OdsySaveHwShadow` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ctxsw.o)` at 0xc0
  **Inferred.** Role (from name): saves Odyssey hardware state into a shadow record around a context switch. It calls RRM_CheckValidFault at 0x100, which the branch at 0xf8 skips when its tested value is zero. It calls odsy_flow_am_i_attached at 0x14c and gfx_flow_am_i_attached at 0x288, 0x2f4 and 0x364, and branches on the results. It calls timeout at 0x190 with odsy_ctxsw_timeout as the function and 0x5dc as the third argument, and untimeout_wait at 0x1a4 with that call's return value. Ten of its stores use offset +0x0 (0xcc, 0x140, 0x1c0, 0x1e8, 0x230, 0x250, 0x260, 0x3ac, 0x420, 0x440) and others use large offsets such as +0x8fc and +0x7dc0; the facts do not show the base register of these stores, so which record or device structure they reach is not established. An indirect call at 0x210 passes seven arguments (a0 through a6) and its target is not resolved, so the full save sequence is not established. It returns 0, 0x1, 0xe or 0x16 in v0.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyDestroyDDRN` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_ddrn.o)` at 0x430
  **Inferred.** Role (from name): tears down a DDRN object; the facts do not say what the abbreviation stands for. It calls gf_OdsyUnSchedSwapBuf at 0x498 unless the word at a2+28 is negative (branch at 0x48c), then gf_OdsyUnSchedRetraceEvent at 0x4a4. When the word at offset 408 of the structure that a2+112 points to is nonzero, it goes on to odsy_hpipe_cleanup at 0x4bc and, only when the doubleword at offset 192 of that structure is nonzero, kmem_free of that pointer with size 0x4000 at 0x4d8. When the word at offset 408 is zero, the branch at 0x4b0 jumps to 0x618, where odsyDMAFinish, odsyMMdetach and odsyUnpinAndUnmapGctxRegions are called (0x618, 0x624, 0x62c) and a word at offset +0x198 is stored at 0x63c; the stored value is not shown in the facts. The code at 0x618 is also reached by the path through gfx_flow_clean_up_other_gfxp at 0x5d4. It returns 0 or 0x1 at 0x5cc. A separate path makes an indirect call at 0x670 whose target is unresolved, branches on its result at 0x678, and reaches cmn_err at 0x68c.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyInitialize` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x1250
  **Inferred.** Role (from name): the bring-up of the Odyssey device. It calls htp_register_board at 0x12c4 with four zero arguments, and gfx_flow_alloc at 0x1468 with odsyHiwaterInterrupt as its sixth argument (a5). In address order it reaches odsyInitCfifo (0x1400), odsyReset (0x143c), odsyInitMM (0x14cc), odsyInitDMA (0x14d4), odsyInitRetrace (0x14e8) and odsyInitInterrupts (0x14f0); odsyReset and gfx_flow_alloc are skipped when the branch at 0x1434 finds s2 zero, and the source of s2 is not shown. A nonzero odsyInitCfifo result takes the path through gfx_flow_dealloc (0x1410) and odsy_usdelay with the constant 0x2710 (0x141c), followed by a compare of s8 with 6 at 0x142c; the full retry loop is not followed here. Another path calls init_sema (0x16b8), timeout with odsy_init_ddrncount_timeout and the constant 0x1f4 (0x16c8) and psema (0x16d4). The routine returns the odsyInitCfifo result or -1 in v0.
  Reviewed: 2026-11-12 (sonnet)

- `odsyLowLevelReset` in `6.5.22/usr/cpu/sysgen/IP30boot/odyssey.a(odsy_init.o)` at 0x17fc
  **Inferred.** Role (from name): resets the Odyssey hardware. It does word loads and stores whose addresses the facts show only in part (for example 0x900000001000010c, 0x9000000010000114 and 0x9000000010000024, in the 0x9000000010000000 range). It compares one value against 0xd100 at 0x1830 and calls delay routines with the constants 0xf4240 (odsy_usdelay at 0x1a9c, reached only when the branch at 0x1a78 does not skip it), 0x30d40 (us_delay at 0x1ad4), 0x3e8 (odsy_usdelay at 0x1af8) and 0x186a0 (odsy_usdelay at 0x1b3c). It calls odsyLLPReset at 0x1aec; what that call resets is not shown. The facts show the order of the accesses but not what each address controls.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP30boot/os.a(pda.o)` at 0x176c
  **Inferred.** Role (from name): not established. It loads a byte at absolute offset -0x3f80, a byte at -0x3f7f, a word at -0x3f7c and a doubleword at -0x3f60, and branches on each; the facts do not show that these are per-processor. It calls panicspin at 0x177c when the first byte is nonzero. Independently of that, when the byte at -0x3f7f is nonzero it stores two bytes at 0x1790 and 0x1794 (the facts do not show their addresses), when the word at -0x3f7c is nonzero it calls tlbflush_rand with 1 at 0x17a4, and when the doubleword at -0x3f60 is nonzero it calls tlbflush with that value at 0x17b8 and then stores a doubleword at 0x17bc. The two tlbflush calls are separate tests, not alternatives. The name suggests a vector action, but nothing in the facts fixes what the flags mean.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP30boot/os.a(syssgi.o)` at 0xd4
  **Inferred.** Role (from name): the entry of the syssgi system call. It calls get_current_abi at 0xf4, then branches at 0x108 on the word at sat_enabled. When that word is nonzero the code at 0x2aa4 calls sat_never and then sat_set_subsysnum (0x2aac) with the halfword at a0+6. When it is zero, the doubleword loaded at 0x110 through a0 is compared unsigned with 0x4e3 at 0x124: a value at or above 0x4e3 branches to 0x138, and the only return shown has v0 = 0x16 at 0x160; a smaller value makes an indirect tail call at 0x130 through a table indexed by that value shifted left by 3. The table entries are unresolved, so the role of each command is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP30boot/os.a(time.o)` at 0xc0
  **Inferred.** Role (from name): reads an interval timer into the buffer at a1. The branch at 0xdc tests a0. When a0 is zero the routine calls splprof (0xe4), loads a pointer at +0x2f0 of register a2 (the register at that point, not the entry argument), calls mutex_lock at 0xf8, copies four 8-byte fields from offsets 0x840 through 0x858 of the structure that pointer reaches to a1+0 through a1+0x18, and calls mutex_unlock at 0x120. On some paths after that it calls chktimeout_tick at 0x15c (first argument 0) and 0x1f4 (first argument 2), tick_to_timeval at 0x178 with the constant 0x50 as its third argument, and splx at 0x184. When a0 is nonzero it calls splhi at 0x194, loads four 8-byte fields at offsets 0x348 through 0x360 from a base the facts do not show, stores them to a1+0 through a1+0x18, and calls splx at 0x1c8. A return with v0 = 0 appears at 0x1e0. Which timer each field belongs to is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_vect_set` in `6.5.22/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x14534
  **Inferred.** Role (from name): writes the vector part of a PCI bridge interrupt register selected by the index in a1. The branch at 0x14550 skips past the body when a0 is zero. An index below 0 or at least 8 branches to 0x14744, next to the assfail call at 0x1474c with the constant 0x85b, and a compare of s0 with -1 at 0x145b4 branches to panic at 0x14760. A chain of compares of a4, after a word load at 0x14644, against the constants 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002 follows; the facts show the stores in this routine without their offsets, so which register each compare leads to is not established, and the bridge registers those constants stand for are not identified here.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_addr_set` in `6.5.22/usr/cpu/sysgen/IP30boot/pcibr.o` at 0x149a0
  **Inferred.** Role (from name): writes the address part of a PCI bridge interrupt register. The facts show the same branch structure as the vector setter at 0x14534: a null check on a0 (0x149bc), an index check against 0 to 7 that branches to 0x14bc0 next to the assfail call at 0x14bc8 (constant 0x89a), a compare of s0 with -1 at 0x14a1c that branches to panic at 0x14bdc, and the same chain of compares against 0xe000 through 0xc002. The facts show no store offsets in either routine, so what the two differ in, and which half of the register this one writes, is not established; the pairing rests on the routine names and the matching structure, not on a call between the two routines.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.22/usr/cpu/sysgen/IP30boot/xbridge.o` at 0x54
  **Inferred.** Role (from name): the attach routine for the Xbridge PCI bridge widget. It allocates and zeroes a 0xab8-byte state block with kmem_zalloc and memset, registers the graph vertex and the PCI provider (hwgraph_char_device_add at 0x118, pciio_provider_register with pci_xbridge_provider at 0xec), and sets up the error and interrupt path (xwidget_error_register with pcibr_error_handler at 0x440, xtalk_intr_connect with pcibr_error_intr_handler at 0x574). It creates device windows with pciio_device_win_map_new, using the constants 0x10000000 and 0x40000000 as the second argument of two of the calls. It calls rmallocmap at 0x4c4 with the halfword at state offset 112 as its first argument; what the map holds is not shown. It loops over slots bounded by the bytes at +56 and +57 of the state block, calling pcibr_slot_info_init, pcibr_slot_addr_space_init, pcibr_slot_device_init, pcibr_slot_guest_info_init and pcibr_slot_call_device_attach. It compares a value against revision-like constants (0xc0021 to 0xc0024, 0xd0021, 0xd0022); what they gate is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP32boot/cn.o` at 0x804
  **Inferred.** Role (from name): sets up the IP32 console. It calls gfx_earlyinit at 0x820, calls vn_rele at 0x854 only when the vnode word at cn+4 is nonzero, and calls du_init (0x8a8 or 0x8b8, depending on a branch) and tp_init (0x954), spinlock_init and init_mutex on the console record cn. After is_specified on arg_console at 0x904 returns nonzero, it compares the first byte of the console argument with 0x67, 0x47, 0x64 and 0x44 (0x924 to 0x940). Only the branches of 0x64 and 0x44 lead to a check of the second byte against 0x32 and the third against zero (0x984, 0x990); the 0x67 and 0x47 branches go elsewhere. The device each choice selects is not identified in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x13e8
  **Established.** The body is 8 bytes: it returns with v0 = 0x0 and touches nothing else. The function inventory lists many symbols at this address (dummyret0_func, getcpuid, apcvsema, appsema, apvsema, disarm_threeway_trigger, is_fullhouse, is_vme_space, threeway_trigger_armed, vme_adapter), so this is a shared return-zero body and getcpuid is one of several names for it. The body does not show what any one of those names does.
  Grade scope: established for the body; the aliases come from the inventory.
  Reviewed: 2026-11-12 (sonnet)

- `_get_timestamp` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x13f8
  **Inferred.** The body is an 8-byte tail jump to get_r4k_counter, which receives the 4 argument registers unchanged. The return value is whatever get_r4k_counter returns, and the facts do not show it. The timestamp reading comes from the callee's name, so it is inferred.
  Grade scope: established for the jump; inferred for the timestamp role.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1a00
  **Inferred.** Role (from name): the IP32 kernel entry. It stores the three startup arguments to _argc, _argv and _envirn, loads the dbgmon_loadstop flag, and calls _check_dbg at 0x1a48. It calls debug at 0x1a84 only when the dbgmon_loadstop flag is nonzero (the branch at 0x1a78 skips it when zero). It stores a doubleword to 0xffffffffb4000018 at 0x1b54. It calls mlsetup at 0x1b2c with _argc as the first argument and the constant 0xa as the fourth (a3), and main at 0x1b3c. A series of tests on a3 (0x1ac8 to 0x1af8) skip to 0x1b0c, and what they check is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1bd8
  **Established.** This is an 8-byte leaf: a bare return that touches no memory and makes no call. The routine does not write v0, so the caller's value is what comes back. The name suggests it mutes audio, but the body does nothing that would.
  Grade scope: established for the body.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x8728
  **Inferred.** Role (from name): the IP32 machine-level setup. It does loads and stores in the 0xffffffffb4000000 range and calls get_crimerev at 0x8754. It calls setkgvector with ackkgclock and 0x28 at 0x87d8, setcrimevector for synthclock, synthackkgclock and buserror_intr (0x87f4 to 0x8848), and setmaceisavector for powerintr at 0x885c. It also calls setup_ip32_flags, init_sysid and load_nvram_tab (0x8868 to 0x8878). It calls pciio_pio_write32 with the addresses 0xffffffffbf080004 and 0xffffffffbf080008 (0x8950, 0x896c, 0x89b8); the value at 0x896c or 0x89b8 depends on ip32_pci_enable_prefetch, and the facts do not say what those writes control beyond the variable's name. It calls __dcache_wb_inval and __icache_inval over wait_for_interrupt_fix_loc at 0x8918 and 0x8924, and the branches at 0x88e4 and 0x88ec can skip both calls. The meaning of the vector numbers and of the constant 0x28 is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb1a0
  **Established.** The body is 1 call to cmn_err with severity 0x0 and a string constant, passing a2 and a3 through, and the routine returns cmn_err's result. Despite the name, it sends no interrupt.
  Grade scope: established for the body.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb5ec
  **Inferred.** This makes a direct call to init_timer at 0xb524 (call site 0xb5fc), passing its four argument registers through. A flag at .sdata+156 skips that call when the flag is zero. The indirect call at 0xb608, whose target is not resolved, runs on either path, and its result is the return value. It has the same shape as slowkgclock and ackkgclock; the facts do not show whether their unresolved targets differ.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb61c
  **Inferred.** This has the same shape as startkgclock: a direct call to init_timer at 0xb524 (call site 0xb62c), the .sdata+156 flag test at 0xb624 that skips it when the flag is zero, and an unresolved indirect call at 0xb638. The name suggests a slower clock setup, but the facts show no difference that would support it.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb710
  **Inferred.** This has the same shape again: a direct call to init_timer at 0xb524 (call site 0xb720), the .sdata+156 flag test at 0xb718 that skips it when the flag is zero, and an unresolved indirect call at 0xb72c. The name suggests an acknowledge path, but no acknowledge register is accessed in the facts, so that reading is not established.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xc158
  **Inferred.** Role (from name): applies a time-of-day adjustment taken from a0. The branch at 0xc170 tests a0 for zero and the branch at 0xc17c tests its sign. It calls splhi (0xc1a0, 0xc278) and splx (0xc1e8, 0xc2a0). The constants 0x4650 and 0x190 are held in a0 and a3 around the first splhi call, but the facts do not show them as arguments to splhi or splx. At 0xc228 it compares the constant 0xafc80 with a value derived from a0; this is a branch, not shown to be a loop. The units of the constants are not identified.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0xd88c
  **Inferred.** Role (from name): the ECC error handler for the IP32 kernel. It calls _r4600sc_disable_scache at 0xd8f0, which the branches at 0xd8dc and 0xd8e8 skip when the two_set_pcaches word or the word at -0x5d50 is zero, and _r4600sc_enable_scache at 0xddd4. On the path the branch at 0xd9d8 (on a shifted r4000_config value) does not skip, it loads from the 0xb4000000 range at offsets 0x40, 0x48, 0x250 and 0x258 and stores the values into a record. It calls ecc_log_error with 0x6 as its first argument at 0xdb30, ecc_assign_msg at 0xdc44, and timeout with ecc_cleanup at 0xdc90 only when the word at ecc_info+8 is nonzero. It makes direct calls to ecc_fixmem at 0xdd60 (target 0xdde4) and ecc_fixcache at 0xdd78 (target 0xde58); the facts do not show which of them runs on a given path. Address order does not establish call order here, because the routine branches back and forth. Which register bits select memory repair or cache repair, and what the constants 0x240000 and 0x10000000 mean, are not identified.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1074c
  **Inferred.** Role (from name): reports bus errors for the IP32 kernel. It passes the CPU error status and address words (crm_cpu_error_stat, crm_cpu_error_addr) to print_buserr_rev1 at 0x107b8 when get_crimerev returns nonzero, and to print_buserr_rev0 at 0x10944 when it returns zero. It reads the memory error status and address words and calls print_memerr at 0x107e4. It calls get_current_name and cmn_err (severity 0x6) at several sites and cmn_err_tag with the tag numbers 0x39 to 0x3c; the facts do not show what those messages say. A VICE error address (crm_vice_error_addr) goes to vice_err at 0x10894. On some paths a memory error goes to log_ecc_err (0x10978), ecc_error_correction (0x1098c) and a call to _r4600sc_enable_scache (0x109ac). The masks 0x78080 and 0xe000000 and the severity and tag numbers are not identified.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10ea4
  **Inferred.** This reads a 64-bit word at 0xffffffffbf310008, branches on whether a0 is zero (0x10eb0), and writes the word back at 0x10ec8. The facts show no constant that the stored value comes from beyond that test, so which LED bits change is not established. The address suggests a board register, but the facts do not identify it.
  Reviewed: 2026-11-12 (sonnet)
