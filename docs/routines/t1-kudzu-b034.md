# Routine role statements: kudzu IP30 kernel and boot objects

Role statements for 25 routines in the shipped 6.5.30 kernel objects and linked kernels. Each entry's register uses, calls, branch targets and constants come from the disassembly of that routine. The role sentences are our reading of that disassembly. Where the disassembly stops short of a purpose, the entry says so. Every grade is inferred unless the entry says otherwise.

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3844c
  **Inferred.** earlybadaddr probes one address with a load whose width (1, 2 or 4 bytes) is chosen by a1 through the compare chain at 0x38484 to 0x384b4. Before the load it stores to the nofault slot (`sw` at 0x3847c), and it returns 0 at 0x38524 when the access completes. A width outside 1, 2 and 4 seems to reach the `cmn_err` call at 0x384e0, and the bit test on t1 at 0x384f4 can send control to earlybaerror at 0x385c8. The probe reading comes from the access widths and the nofault store; the disassembly does not show what the caller does with the result.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x38598
  **Inferred.** baerror loads a 64-bit value from the fixed address 0xffffffffffffc020 (`ld` at 0x3859c) and tests that loaded value for zero at 0x385a0. When it is nonzero, it stores a 2-byte zero at offset +0xf6 from that value (`sh` at 0x385a8); when it is zero that store is skipped. It always stores a 4-byte zero to the fixed address 0xffffffffffffc484 (`sw` at 0x385b4) and returns 1 at 0x385c0. The disassembly does not say what the fixed-address registers are, so the bus-error reading rests on the name alone.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x385c8
  **Inferred.** earlybaerror stores 4 bytes to the nofault slot (`sw` at 0x385e8) and returns 1 at 0x385f4. The disassembly shows no other work. Its role as the early bus-error return is inferred from the name and from earlybadaddr branching to it at 0x384f4.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3a4f8
  **Established.** The whole routine is one `beq` at 0x3a4f8 that branches to itself and is always taken, so once entered it never returns. The name suggests it parks a processor until reset, but nothing in the routine shows a reset or an exit; that purpose is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3a950
  **Inferred.** VEC_dbe loads a 2-byte value from the fixed address 0xffffffffffffc420 (`lh` at 0x3a950). It jumps to VEC_trap with its four arguments passed through (`j` at 0x3a954), and the delay slot at 0x3a958 stores the loaded value with an 8-byte `sd` to a0+0x140. The name points to a data bus error vector, which is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3cfcc
  **Inferred.** ecc_handler is about 2 KB (2008 bytes). It reads an error record through a1 at fixed offsets such as +0x18, +0x2b8 and +0x2bc, and stores to offsets such as +0x28 to +0x34 and +0x2b8 (the base of some of these stores is not resolved). It makes direct calls to ecc_cacheOp, the routine at 0x3c820, with operation codes 0x0, 0x1, 0x3, 0x4, 0x5 and 0x7, calls eccf_recoverable_threshold at 0x3d0cc, calls memerror_handle_uce at 0x3d63c with the word at a1+0x18 as its first argument and ecc_cacheOp's return value as its second, and reports through cmn_err_tag with message number 0xb96 at 0x3d720. The meaning of the record fields and the operation codes is not established by the disassembly; the cache and memory ECC reading comes from the callee names.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3e4dc
  **Inferred.** badaddr is a thin wrapper. It fills a stack block (0 at offset +0x0, a1 at +0x4, a0 at +0x10 and sp-40 at +0x18), passes that block's address (sp-48) as a0 to probe_common, the routine at 0x3e5ac, by a direct call (`jal` at 0x3e4f8), forwards a1, a2 and a3 unchanged, and returns probe_common's result (`v0` at 0x3e504). What the probe checks is in probe_common, which this disassembly does not show. The name suggests a read probe that reports whether an address is bad.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3e50c
  **Inferred.** badaddr_val has the same shape as badaddr, with one visible difference. It fills a stack block with 2 at offset +0x0 (badaddr stores 0 there), a1 at +0x4, a0 at +0x10 and a2 at +0x18, passes the block's address (sp-48) as a0, forwards a1, a2 and a3, and returns the result of probe_common at 0x3e5ac (`jal` at 0x3e528, return at 0x3e534). The name suggests a variant that also checks a value, but nothing in this entry shows that; what the code in the first word selects would have to come from probe_common, which is outside this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3e53c
  **Inferred.** wbadaddr has the same wrapper shape again. It fills a stack block with 1 at offset +0x0 (badaddr stores 0 there and badaddr_val 2), a1 at +0x4, zero at +0x8, a0 at +0x10 and sp-40 at +0x18, passes the block's address (sp-48) as a0, forwards a1, a2 and a3, and returns the result of probe_common at 0x3e5ac (`jal` at 0x3e560, return at 0x3e56c). The name suggests a write probe, but every store in this entry goes to its own stack block, so the write direction is left open until probe_common is read.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42a20
  **Inferred.** VEC_ibe follows the same pattern as VEC_dbe. It loads two doublewords through k1 at +0x118 and +0x120 (`ld` at 0x42a20 and 0x42a24) and tests the sign of ra at 0x42a28 (`bgez`). A non-negative ra skips the one instruction at 0x42a30 and a negative one runs it, and both paths reach 0x42a34, which stores a2 through k1+0x110. It then loads a 2-byte value from the fixed address 0xffffffffffffc420 (`lh` at 0x42a38) and tail-jumps to VEC_trap (`j` at 0x42a3c) with a0, a1 and a3 passed through, and stores that value as 8 bytes at a0+0x140 in the delay slot (`sd` at 0x42a40). The disassembly does not resolve the a2 argument. The instruction bus error reading comes from the name and the pattern match with VEC_dbe.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.30/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** _mraccess wraps a sync access with metering. It fetches a record from wchanmeter at 0x38b0 (passing sym(syncinfo)), calls mr_access at 0x38c0, and, when a record exists, looks up an entry with mrmeter_getentry at 0x392c using the constant 5. It updates counters in that record at offsets +0x1c0 through +0x1d0, with an ll/sc pair on +0x198 at 0x3940 and 0x394c, and adds an entry with mrmeter_entry at 0x3994 using the same constant. The meaning of the meter fields is inferred from the callee names.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.30/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x39ec
  **Inferred.** _mrupdate looks like the update side of _mraccess. It calls wchanmeter at 0x3a0c with sym(syncinfo), calls mr_update at 0x3a1c, and calls mrmeter_entry at 0x3ac4 with the constant 6 where _mraccess uses 5. It writes fields at offsets +0x1a0 through +0x1d8 and reads a 2-byte value from the fixed address 0xffffffffffffc420 (`lh` at 0x3a7c). The field meanings are not established; the pairing with _mraccess is inferred from the matching callees and offsets.
  Reviewed: 2026-11-12 (sonnet)

- `MgrasGeneralInterrupt` in `6.5.30/usr/cpu/sysgen/IP27boot/mgras.a(mgras.o)` at 0x83b8
  **Inferred.** MgrasGeneralInterrupt reads a pointer at a0+0xd0 and then reads and writes words at offsets +0x11020 through +0x11048 of the block it names (`lw` at 0x83cc and 0x83d0, `sw` at 0x83f8 and others). It signals waiters with vsema at 0x83f4, 0x8428 and 0x855c, and calls MgrasSchedSwapBufFinish at 0x8454 with a1 set to 0x400. Status bits in s1 are tested against 0x400, 0x200 and 0x2000a200, and the other paths report through numbered cmn_err_tag messages (0xaa5 to 0xaa9 at 0x848c to 0x852c) or mgras_error at 0x8538. The register meanings are not shown; the interrupt handler reading comes from the callee names and the status-bit tests.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_phase_bits` in `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` at 0x68
  **Inferred.** nic_get_phase_bits reads one 8-byte value from the fixed address 0x9000000010000078 (`ld` at 0x70) and returns a value in v0 at 0x94. The disassembly does not trace where the returned value comes from, so what it holds is unknown. The disassembly list no branches and no calls. The phase-bits name is the only basis for the reading.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.30/usr/cpu/sysgen/IP27boot/os.a(pda.o)` at 0x1ba0
  **Inferred.** doacvec tests a flag byte at the fixed address 0xffffffffffffc080 (`lbu` at 0x1ba0) and, when it is nonzero, calls panicspin at 0x1bb0. It then reads a second byte at c081 (`lbu` at 0x1bb8) and, when that is nonzero, stores 1 to c04e and 0 to c081 (`sb` at 0x1bc4 and 0x1bc8). When the word at c084 is nonzero it calls tlbflush_rand with 1 at 0x1bd8; that word is only read. When the doubleword at c0a0 is nonzero it calls tlbflush at 0x1bec with that value and then stores 0 to c0a0 (`sd` at 0x1bf0). The name suggests a per-CPU action vector; the disassembly does not show what the flags mean.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.30/usr/cpu/sysgen/IP27boot/os.a(syssgi.o)` at 0x140
  **Inferred.** syssgi is a large routine, about 12.7 KB. Its entry reads a 64-bit selector from a0 (`ld` at 0x180, and at 0x3064 in a later block), uses it to index a .rodata table at 0x19c, and makes an indirect jump at 0x1a0 whose targets the disassembly does not resolve. The entry also calls get_current_abi at 0x164, checks sat_enabled at 0x178, and returns 0x16 at 0x1d0. Most of the body after 0x1d8 is not reached from the entry as disassembled, so the full role is unknown; the dispatcher reading comes from the table-indexed jump and the name.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.30/usr/cpu/sysgen/IP27boot/os.a(time.o)` at 0xc0
  **Inferred.** dogetitimer branches on a0 at 0xdc. When a0 is zero it calls splprof at 0xe4, loads the pointer at a2+0x2f0, takes mutex_lock at 0xf8 and mutex_unlock at 0x120 on offset +2144 of the object that pointer names, and stores four 8-byte values from that object's offsets +0x840 through +0x858 into a1 (`ld` at 0x100 to 0x118, `sd` at 0x104 to 0x124). Later on that path it passes the value returned by chktimeout_tick (called at 0x15c with a0=0, or at 0x1f4 with a0=2) to tick_to_timeval at 0x178 with a1+16 and a scale of either 0x320 or 0x989680, then restores priority with splx at 0x184. When a0 is nonzero it calls splhi at 0x194, copies four 8-byte values from the block at a2+(a0<<5), offsets +0x348 through +0x360, into a1, and restores priority with splx at 0x1c8. The getitimer reading comes from the name and the timeval conversion.
  Reviewed: 2026-11-12 (sonnet)

- `part_discovery` in `6.5.30/usr/cpu/sysgen/IP27boot/partition.o` at 0x1c10
  **Inferred.** part_discovery calls cpsema on sym(.bss) at 0x1c2c and again at 0x1c3c, repeating while the result is nonzero, then calls vector_read at 0x1c58 with a2 = 0x600000. It calls part_probe_nasid at 0x1c7c inside a loop bounded by 0x100 (`bne` at 0x1c94), and again at 0x1d78 and 0x1d94 on other paths. It also calls mutex_spinlock at 0x1d00 and mutex_spinunlock at 0x1cc0 and 0x1d38, calls part_interrupt_set at 0x1d4c with a1 = 2 and part_activate as a2, and calls part_start_partition_thread at 0x1d5c. The discovery reading comes from the callee names; the layout of the vector data and what the lock guards are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_vect_set` in `6.5.30/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x147d0
  **Inferred.** Role (from name): pcireg_intr_addr_vect_set sets an interrupt vector register on a PCI bridge. The index in a1 is checked against 0 and 8 (`bltz` at 0x14814, and `beq` at 0x14820 taken when it is 8 or more); both go to 0x149e0, and assfail is called at 0x149e8 with the constant 0x85b. A separate chain of compares of s0 against 2, 3, 0, 1 and -1 (0x14828 to 0x14850) reaches the panic call at 0x149fc when s0 is -1. It also dispatches on a4 against the constants 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002, which the disassembly does not name, and it loads and stores fields whose base the facts do not resolve. The register meanings are not shown; the register-setter reading comes from the name and the assertion and panic paths.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_addr_set` in `6.5.30/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x14c3c
  **Inferred.** pcireg_intr_addr_addr_set is the address-half sibling of pcireg_intr_addr_vect_set. It has the same index check, the same dispatch constants on a4 (0xe000, 0xe010, 0xd102, 0xd112, 0xd002, 0xc002), and the same assfail and panic callees. Its assertion call at 0x14e64 passes the constant 0x89a, and its panic path is at 0x14e78. Which register each arm writes is not established by these disassembly; the sibling reading is inferred from the matching structure.
  Reviewed: 2026-11-12 (sonnet)

- `scsiha_ioctl` in `6.5.30/usr/cpu/sysgen/IP27boot/scsiha.o` at 0x188
  **Inferred.** scsiha_ioctl calls scsi_ctlr_info_get at 0x1d0 first (what it does with the result is not shown), then compares the command in a1 with 0x7306 at 0x1e0. When a1 is 0x7306 it follows a path through hwgraph_connectpt_get, device_controller_num_get, hwgraph_path_to_dev, sprintf at 0x2a8, hwgraph_traverse at 0x2b8 and hwgraph_edge_add at 0x2e0. Otherwise it allocates a 16-byte block with kmem_zalloc at 0x1e8 and branches at 0x1f4 on codes at or above 0xb. Only that at-or-above-0xb branch reaches copyin at 0x23c, with the block and length 0x10. A compare with 0x7321 at 0x200 sends that code to cap_able_cred at 0x344, called with a4 and 0x2000000000. The other codes reach an unresolved call at 0x214 with the block and kern_free at 0x220. The command meanings are not named in the disassembly, so the ioctl reading comes from the codes and callees.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.30/usr/cpu/sysgen/IP27boot/xbridge.o` at 0x54
  **Inferred.** xbridge_attach is the attach routine for a bridge widget. It allocates an 0xab8-byte state block with kmem_zalloc at 0x1c8, clears it with memset at 0x1dc, and fills fields from the results of pcibr_bridge_ptr_get and pcibr_hints_get. It registers a PCI provider with pciio_provider_register at 0xf8 and pciio_provider_startup at 0x100, and an error handler with xwidget_error_register at 0x454 plus an interrupt through xtalk_intr_alloc at 0x554 and xtalk_intr_connect at 0x584 with pcibr_error_intr_handler. It sets address windows with pciio_device_win_map_new at 0x68c, 0x6b0 and 0x6d8, and runs per-slot setup through pcibr_slot_info_init, pcibr_slot_addr_space_init and pcibr_slot_device_init at 0x740, 0x770 and 0x7a0. The layout of the state block and the register meanings are not shown here; the attach reading comes from the callee names.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.30/usr/cpu/sysgen/IP30boot/cn.o` at 0xaac
  **Inferred.** cn_init sets up the early console. It calls gfx_earlyinit at 0xac4 first. When the halfword at cn+0x48 is zero (`beq` at 0xad4 to 0xb28) it initializes locks with spinlock_init at 0xb2c and init_mutex at 0xb44 on the console structure and calls is_specified at 0xb5c with arg_console. When that returns nonzero it compares the selector byte at sym(arg_console) with 0x67 and 0x47 (`beq` at 0xb74 and 0xb7c, both to 0xbb8). Its other paths reach tp_init at 0xb8c and du_init at 0xba8. Which console device those letters select is not shown in the disassembly, so the console reading is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** bitlockdebug calls try_bitlock at 0x16cc (passing a0, a1 and a2 from entry) and again at 0x17f8, with ll/sc pairs on the words at a2+0xa8, +0xac, +0xb0, +0xb8 and +0xbc (from 0x16b4 to 0x1850). It makes unsigned compares at 0x1750 (registers a1 and a2, which no longer hold the entry arguments there) and at 0x1780 (a1 holding the word at a2+0x98 against s2). The `bne` at 0x1750 goes to 0x18b0, and cmn_err is called at 0x18bc with a value read from the fixed address 0xffffffffaff20000. Role (from name): a debug variant of a bit-lock acquire; the disassembly does not show the lock-word layout or what the cmn_err path signals.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** _64bitlockdebug looks like the 64-bit counterpart of bitlockdebug. It calls try_64bitlock at 0x1f70 and 0x209c where bitlockdebug calls try_bitlock, and it uses the same field offsets, with ll/sc pairs on a2+0xa8 through +0xbc and the same read from the fixed address 0xffffffffaff20000. It calls cmn_err at 0x2160 with the same argument shape. Whether the 64-bit form uses a wider lock word is not shown by the disassembly, so that difference is inferred from the callee name.
  Reviewed: 2026-11-12 (sonnet)
