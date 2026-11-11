# Routine role statements: kudzu IP30 kernel and boot objects

Role statements for 25 routines in the shipped 6.5.30 kernel objects and linked kernels. Each entry's register uses, calls, branch targets and constants come from the disassembly of that routine. The role sentences are our reading of that disassembly. Where the disassembly stops short of a purpose, the entry says so. Every grade is inferred unless the entry says otherwise.

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3844c
  **Inferred.** earlybadaddr probes one address with a load whose width (1, 2 or 4 bytes) is chosen by a1 through the compare chain at 0x38484 to 0x384b4. Before the load it stores to the nofault slot (`sw` at 0x3847c), and it returns 0 at 0x38524 when the access completes. A width outside 1, 2 and 4 seems to reach the `cmn_err` call at 0x384e0, and the bit test on t1 at 0x384f4 can send control to earlybaerror at 0x385c8. The probe reading comes from the access widths and the nofault store; the disassembly does not show what the caller does with the result.

- `baerror` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x38598
  **Inferred.** baerror reads a 64-bit value from the fixed address 0xffffffffffffc020 (`ld` at 0x3859c), then tests a global slot for zero at 0x385a0. When that slot is zero it skips the 2-byte store at 0x385a8, and it always writes a 4-byte value to the fixed address 0xffffffffffffc484 (`sw` at 0x385b4). It returns 1 at 0x385c0. The disassembly does not say what the fixed-address registers are, so the bus-error reading rests on the name alone.

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x385c8
  **Inferred.** earlybaerror stores 4 bytes to the nofault slot (`sw` at 0x385e8) and returns 1 at 0x385f4. The disassembly shows no other work. Its role as the early bus-error return is inferred from the name and from earlybadaddr branching to it at 0x384f4.

- `cpu_waitforreset` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3a4f8
  **Established.** The whole routine is one `beq` at 0x3a4f8 that branches to itself and is always taken, so once entered it never returns. The name suggests it parks a processor until reset, but nothing in the routine shows a reset or an exit; that purpose is inferred.

- `VEC_dbe` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3a950
  **Inferred.** VEC_dbe reads a 2-byte value from the fixed address 0xffffffffffffc420 (`lh` at 0x3a950). It jumps to VEC_trap with its four arguments passed through (`j` at 0x3a954), and the 8-byte store to a0+0x140 (`sd` at 0x3a958) sits in the jump's delay slot. The stored value is not traced by the disassembly. The name points to a data bus error vector, which is inferred.

- `ecc_handler` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3cfcc
  **Inferred.** ecc_handler is the largest routine in this set, about 2 KB. It reads an error record through a1 at fixed offsets such as +0x0, +0x18 and +0x2b8, and writes results back into that record. It calls ecc_cacheOp through the pointer at 0x3c820 with operation codes 0x0, 0x1, 0x3, 0x4, 0x5 and 0x7, calls eccf_recoverable_threshold at 0x3d0cc, passes a result to memerror_handle_uce at 0x3d63c, and reports through cmn_err_tag with message number 0xb96 at 0x3d720. The meaning of the record fields and the operation codes is not established by the disassembly; the cache and memory ECC reading comes from the callee names.

- `badaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3e4dc
  **Inferred.** badaddr is a thin wrapper. It passes sp-48 as a0 to the pointer-called probe_common at 0x3e5ac (`jal` at 0x3e4f8), forwards a1 to a3 unchanged, and returns probe_common's result (`v0` at 0x3e504). What the probe checks is in probe_common, which these disassembly does not show. The name suggests a read probe that reports whether an address is bad.

- `badaddr_val` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3e50c
  **Inferred.** badaddr_val has the same shape as badaddr: sp-48 as a0, a1 to a3 forwarded, and the result of probe_common at 0x3e5ac returned (`jal` at 0x3e528, return at 0x3e534). The name suggests a variant that also checks a value, but nothing in this entry shows that. Any difference from badaddr would have to come from probe_common, which is outside these disassembly.

- `wbadaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3e53c
  **Inferred.** wbadaddr has the same wrapper shape again: sp-48 as a0, a1 to a3 forwarded, and the result of probe_common at 0x3e5ac returned (`jal` at 0x3e560, return at 0x3e56c). The name suggests a write probe, but this entry has no store of its own, so the write direction is left open until probe_common is read.

- `VEC_ibe` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42a20
  **Inferred.** VEC_ibe follows the same pattern as VEC_dbe. It reads a 2-byte value from the fixed address 0xffffffffffffc420 (`lh` at 0x42a38), tail-jumps to VEC_trap (`j` at 0x42a3c) with a0, a1 and a3 passed through, and stores 8 bytes at a0+0x140 (`sd` at 0x42a40). A sign test on ra at 0x42a28 chooses between two paths before one of the stores at 0x42a34, and the disassembly does not resolve the a2 argument. The instruction bus error reading comes from the name and the pattern match with VEC_dbe.

- `_mraccess` in `6.5.30/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** _mraccess wraps a sync access with metering. It fetches a record from wchanmeter at 0x38b0 (passing sym(syncinfo)), calls mr_access at 0x38c0, and, when a record exists, looks up an entry with mrmeter_getentry at 0x392c using the constant 5. It updates counters in that record at offsets +0x1c0 through +0x1d0, with an ll/sc pair on +0x198 at 0x3940 and 0x394c, and adds an entry with mrmeter_entry at 0x3994 using the same constant. The meaning of the meter fields is inferred from the callee names.

- `_mrupdate` in `6.5.30/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x39ec
  **Inferred.** _mrupdate looks like the update side of _mraccess. It calls wchanmeter at 0x3a0c with sym(syncinfo), calls mr_update at 0x3a1c, and calls mrmeter_entry at 0x3ac4 with the constant 6 where _mraccess uses 5. It writes fields at offsets +0x1a0 through +0x1d8 and reads a 2-byte value from the fixed address 0xffffffffffffc420 (`lh` at 0x3a7c). The field meanings are not established; the pairing with _mraccess is inferred from the matching callees and offsets.

- `MgrasGeneralInterrupt` in `6.5.30/usr/cpu/sysgen/IP27boot/mgras.a(mgras.o)` at 0x83b8
  **Inferred.** MgrasGeneralInterrupt reads a pointer at a0+0xd0 and then reads and writes words at offsets +0x11020 through +0x11048 of the block it names (`lw` at 0x83cc and 0x83d0, `sw` at 0x83f8 and others). It signals waiters with vsema at 0x83f4, 0x8428 and 0x855c, and calls MgrasSchedSwapBufFinish at 0x8454 with a1 set to 0x400. Status bits in s1 are tested against 0x400, 0x200 and 0x2000a200, and the other paths report through numbered cmn_err_tag messages (0xaa5 to 0xaa9 at 0x848c to 0x852c) or mgras_error at 0x8538. The register meanings are not shown; the interrupt handler reading comes from the callee names and the status-bit tests.

- `nic_get_phase_bits` in `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` at 0x68
  **Inferred.** nic_get_phase_bits reads one 8-byte value from the fixed address 0x9000000010000078 (`ld` at 0x70) and returns a value in v0 at 0x94. The disassembly does not trace where the returned value comes from, so what it holds is unknown. The disassembly list no branches and no calls. The phase-bits name is the only basis for the reading.

- `doacvec` in `6.5.30/usr/cpu/sysgen/IP27boot/os.a(pda.o)` at 0x1ba0
  **Inferred.** doacvec tests a flag byte at the fixed address 0xffffffffffffc080 (`lbu` at 0x1ba0) and a second byte at c081 (`lbu` at 0x1bb8), writes c04e and c081 (`sb` at 0x1bc4 and 0x1bc8), and reads and writes a word at c084 and a doubleword at c0a0. On one path it calls panicspin at 0x1bb0, on another it calls tlbflush_rand with 1 at 0x1bd8, and on another it calls tlbflush at 0x1bec with the value at -0x3f60. The name suggests a per-CPU action vector; the disassembly does not show what the flags mean.

- `syssgi` in `6.5.30/usr/cpu/sysgen/IP27boot/os.a(syssgi.o)` at 0x140
  **Inferred.** syssgi is a large routine, about 12.7 KB. Its entry reads a 64-bit selector from a0 (`ld` at 0x180, and at 0x3064 in a later block), uses it to index a .rodata table at 0x19c, and makes an indirect jump at 0x1a0 whose targets the disassembly does not resolve. The entry also calls get_current_abi at 0x164, checks sat_enabled at 0x178, and returns 0x16 at 0x1d0. Most of the body after 0x1d8 is not reached from the entry as disassembled, so the full role is unknown; the dispatcher reading comes from the table-indexed jump and the name.

- `dogetitimer` in `6.5.30/usr/cpu/sysgen/IP27boot/os.a(time.o)` at 0xc0
  **Inferred.** dogetitimer copies timer state out of the object that the pointer at a2+0x2f0 names: four 8-byte values from that object's offsets +0x840 through +0x858 are stored into a1 (`ld` at 0x100 to 0x118, `sd` at 0x104 to 0x124). It locks and unlocks a mutex at offset +2144 of that object (mutex_lock at 0xf8, mutex_unlock at 0x120), and it raises and restores interrupt priority with splprof, splhi and splx (calls at 0xe4, 0x194 and 0x184). It takes the remaining count from chktimeout_tick at 0x15c and converts it with tick_to_timeval at 0x178, passing either 0x320 or 0x989680 as the scale. The getitimer reading comes from the name and the timeval conversion.

- `part_discovery` in `6.5.30/usr/cpu/sysgen/IP27boot/partition.o` at 0x1c10
  **Inferred.** part_discovery takes a semaphore on sym(.bss) with cpsema at 0x1c2c and retries at 0x1c3c, then reads a vector with vector_read at 0x1c58 at the address 0x600000. It probes NASIDs with part_probe_nasid at 0x1c7c, 0x1d78 and 0x1d94 in a loop bounded by 0x100, wrapping per-NASID state in mutex_spinlock and mutex_spinunlock. It registers part_activate as an interrupt target through part_interrupt_set at 0x1d4c and starts a partition thread through part_start_partition_thread at 0x1d5c. The discovery reading comes from the callee names; the layout of the vector data is not shown.

- `pcireg_intr_addr_vect_set` in `6.5.30/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x147d0
  **Inferred.** pcireg_intr_addr_vect_set writes an interrupt vector register on a PCI bridge. The index in a1 must be between 0 and 7 (`bltz` at 0x14814 and `beq` at 0x14820 against 8), and out-of-range values go to assfail at 0x149e8, which passes the constant 0x85b, or to panic at 0x149fc. It dispatches on a4 against the constants 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002, which the disassembly does not name, and it reads and writes fields through the object at a0. The register meanings are not shown; the register-setter reading comes from the name and the assertion and panic paths.

- `pcireg_intr_addr_addr_set` in `6.5.30/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x14c3c
  **Inferred.** pcireg_intr_addr_addr_set is the address-half sibling of pcireg_intr_addr_vect_set. It has the same index check, the same dispatch constants on a4 (0xe000, 0xe010, 0xd102, 0xd112, 0xd002, 0xc002), and the same assfail and panic callees. Its assertion call at 0x14e64 passes the constant 0x89a, and its panic path is at 0x14e78. Which register each arm writes is not established by these disassembly; the sibling reading is inferred from the matching structure.

- `scsiha_ioctl` in `6.5.30/usr/cpu/sysgen/IP27boot/scsiha.o` at 0x188
  **Inferred.** scsiha_ioctl handles a set of SCSI host adapter ioctl codes. It compares the command in a1 with 0x7306 at 0x1e0 and 0x7321 at 0x200 and branches at 0x1f4 on codes at or above 0xb. One path allocates a 16-byte block with kmem_zalloc at 0x1e8, copies it in with copyin at 0x23c, and frees it with kern_free at 0x220. Controller identity comes from scsi_ctlr_info_get at 0x1d0 and a hardware-graph path built with sprintf at 0x2a8, hwgraph_traverse at 0x2b8 and hwgraph_edge_add at 0x2e0, and cap_able_cred at 0x344 checks a capability. The command meanings are not named in the disassembly, so the ioctl reading comes from the codes and callees.

- `xbridge_attach` in `6.5.30/usr/cpu/sysgen/IP27boot/xbridge.o` at 0x54
  **Inferred.** xbridge_attach is the attach routine for a bridge widget. It allocates an 0xab8-byte state block with kmem_zalloc at 0x1c8, clears it with memset at 0x1dc, and fills fields from the results of pcibr_bridge_ptr_get and pcibr_hints_get. It registers a PCI provider with pciio_provider_register at 0xf8 and pciio_provider_startup at 0x100, and an error handler with xwidget_error_register at 0x454 plus an interrupt through xtalk_intr_alloc at 0x554 and xtalk_intr_connect at 0x584 with pcibr_error_intr_handler. It sets address windows with pciio_device_win_map_new at 0x68c, 0x6b0 and 0x6d8, and runs per-slot setup through pcibr_slot_info_init, pcibr_slot_addr_space_init and pcibr_slot_device_init at 0x740, 0x770 and 0x7a0. The layout of the state block and the register meanings are not shown here; the attach reading comes from the callee names.

- `cn_init` in `6.5.30/usr/cpu/sysgen/IP30boot/cn.o` at 0xaac
  **Inferred.** cn_init sets up the early console. It calls gfx_earlyinit at 0xac4 first, then initializes locks with spinlock_init at 0xb2c and init_mutex at 0xb44 on the console structure. It compares the selector byte at sym(arg_console) with 0x67 and 0x47 (`beq` at 0xb74 and 0xb7c, both to 0xbb8), and its other paths reach tp_init at 0xb8c and du_init at 0xba8, with is_specified at 0xb5c deciding one branch. Which console device those letters select is not shown in the disassembly, so the console reading is inferred.

- `bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** bitlockdebug acquires a bit lock on the object in a2 by calling try_bitlock at 0x16cc and 0x17f8, with ll/sc pairs on the words at a2+0xa8, +0xac, +0xb0, +0xb8 and +0xbc (from 0x16b4 to 0x1850). It compares a1 against a2 in its retry logic (`bne` at 0x1750, `beq` at 0x1780), and on the failure path it reports through cmn_err at 0x18bc, reading a value from the fixed address 0xffffffffaff20000. The debug role comes from the name and the reporting call; the disassembly does not show the lock-word layout.

- `_64bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** _64bitlockdebug looks like the 64-bit counterpart of bitlockdebug. It calls try_64bitlock at 0x1f70 and 0x209c where bitlockdebug calls try_bitlock, and it uses the same field offsets, with ll/sc pairs on a2+0xa8 through +0xbc and the same reads from the fixed address 0xffffffffaff20000. Its failure report is cmn_err at 0x2160 with the same argument shape. Whether the 64-bit form uses a wider lock word is not shown by the disassembly, so that difference is inferred from the callee name.
