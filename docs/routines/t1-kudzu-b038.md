# Routine role statements, tier-1 batch 38

Each entry below gives a role reading for one routine, drawn only from its disassembly (registers, calls, constants and branches at the cited addresses). Every role here is inferred: the disassembly shows the instruction-level shape, and the purpose is read from that shape plus the symbol name. Where the disassembly stops short of a purpose, the entry says so.

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb624
  **Inferred.** The routine tests a flag word at sdata+156 with the branch at 0xb62c. When the flag is nonzero it calls init_timer at 0xb634 and then makes an indirect call at 0xb640 whose target the disassembly does not resolve. The shape matches the neighbouring acknowledge routine, so it is probably one half of a clock hook pair.

- `ackkgclock` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb718
  **Inferred.** This is the same shape as `slowkgclock` at a fixed offset: the same flag test at 0xb720, a call to init_timer at 0xb728, and an unresolved indirect call at 0xb734. The name suggests an acknowledge step, but the disassembly shows only the call pattern, not what the indirect target does.

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xc160
  **Inferred.** The routine brackets its work with splhi at 0xc1a8 and 0xc280 and splx at 0xc1f0 and 0xc2a8, so it runs at raised interrupt priority. Its loop compares a1 against a value in s0 with a signed branch at 0xc230, and it uses the constants 0x4650, 0x190 and 0xafc80. The name suggests a time-of-day slew, but the disassembly does not show what the adjustment arithmetic computes.

- `ecc_handler` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xd894
  **Inferred.** This is the memory and cache ECC error handler. It reads error status words at abs 0xffffffffb4000048, 0xb4000250, 0xb4000040 and 0xb4000258 (loads at 0xd9ec through 0xda24), disables the secondary cache at 0xd8f8 before it works, and re-enables it at 0xdddc. In between it calls error-address and logging routines through pointers at [0xd754], [0xeac0] and [0xe8fc], schedules a cleanup with timeout at 0xdc98, and dispatches correction through ecc_fixmem at 0xdd68 and ecc_fixcache at 0xdd80. Several of those callees have no resolved body in the disassembly, so the correction steps are only partly described.

- `dobuserr_common` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10754
  **Inferred.** This is the shared bus-error path. It disables the secondary cache at 0x1079c, reads the CRIME revision with get_crimerev at 0x107a8, and picks print_buserr_rev1 at 0x107c0 or print_buserr_rev0 at 0x1094c from the result. It then reports memory errors through print_memerr, logs ECC events through log_ecc_err at 0x10980, and attempts correction with ecc_error_correction at 0x10994 before re-enabling the cache at 0x109b4 and 0x10a60. The block at 0x10878 is not reached from the entry, and the return value is 0 or 1 depending on the path.

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10eac
  **Inferred.** The routine does a load and a store of 8 bytes at abs 0xffffffffbf310008, with a branch at 0x10eb8 that depends on a0 and a second branch at 0x10ed8 that is always taken. The disassembly shows the register location and the dependence on the argument, but not the bit pattern written. The name suggests a front-panel LED write, which fits a single status register, but no call or constant confirms it.

- `debug_stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10f30
  **Inferred.** The routine spans 0x10f30 to 0x10f38, eight bytes, and consists of a return at 0x10f30 with no calls, no loads and no branches in the disassembly. The disassembly leaves v0 unset at that return, so no return value is shown. The name suggests a debugger hook that stops the other processors, but nothing in the body does that. The function is effectively a stub in this object.

- `cpu_waitforreset` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1121c
  **Inferred.** The routine polls several byte-wide status registers in the 0xbf3a0xxx range (for example abs 0xffffffffbf3a0a07 at 0x1126c and 0x11294, and 0xbf3a4a07 at 0x1127c), tests bit 0 and bit 0x80 of them, and tests bit 0x100 of the word at 0xbf310010 at 0x112b8. Depending on those bits it calls flash_write_env at 0x11238, flushbus at 0x11274 and cpu_powerdown at 0x1128c, bracketed by spl7 and splx at 0x112a8 and 0x112a0, and it prints a message with cmn_err at 0x112d0. Which bit leads to which callee is read from the branch structure, and the meaning of the flash and power-down steps comes from the callee names alone.

- `_clock_func` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x129e8
  **Inferred.** The routine copies an 8-byte real-time clock record in both directions. It first reads a series of byte registers through pciio_pio_read8 at addresses from 0xbf3a0007 to 0xbf3a4807 and stores each value into the 8-byte record at a1 (the sb stores at 0x12a94 through 0x12b00). It then writes the record bytes back to the same registers with pciio_pio_write8 (0x12b54 through 0x12bb8). Both halves run under splhi and splx, and a zero flag word at sdata+252 makes it call rtc_init at 0x12c18.

- `_ticksper1024inst` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x140c0
  **Inferred.** This is a short spin loop that reads a 64-bit value at abs 0xffffffffb4000038 with ld at 0x140d0 and 0x140dc. The loop continues while registers t1 and t3 stay positive, using signed branches at 0x140d4 and 0x140e8. The name suggests a count of ticks per 1024 instructions, but the disassembly shows no arithmetic that would produce that count, so the purpose is only a guess from the name.

- `badaddr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14270
  **Inferred.** This is a probe that reads a value at address a0 at a width selected by a1 (1, 2, 4 or 8 bytes, dispatched by the branches at 0x142d4 through 0x1431c). Around the read it saves the bus-error state at abs 0xffffffffb4000048 and 0xb4000250 and tests the error bits in the word at 0xb4000028 with mask 0x300080 at 0x14374. When the mask is set it branches to baerror at 0x149a8, and otherwise it returns 0, so 0 reads as "no fault". The value returned on a fault comes from baerror, which the disassembly shows returning 1.

- `badaddr_val` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x143a8
  **Inferred.** This is the same probe as `badaddr`, with the same width dispatch and error-bit test, but it copies the value it reads into the location at a2 (the stores at 0x1442c through 0x1448c) instead of returning it. The fault check at 0x144dc branches to baerror at 0x149a8, and the normal return is 0. The shared structure with `badaddr` suggests the two were written together, but the disassembly does not show that.

- `wbadaddr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14524
  **Inferred.** This is the write-side probe: it stores a value of width a1 to address a0 (stores at 0x14590 through 0x145d8) under the same bus-error bookkeeping. It also loads from abs 0xffffffffa0000000 at 0x14638 and checks the error mask at 0x14654, branching to baerror on a fault. The normal return is 0. The disassembly shows that load but not its purpose, so whether it verifies the write is not established.

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14808
  **Inferred.** This is an early-boot variant of the probe in `badaddr`. It dispatches on widths 1, 2 and 4 only, with no 8-byte case in the branches at 0x14870 through 0x148ac. It writes a fault marker to sym(nofault) at 0x14868 and 0x14908, and on a fault it branches to earlybaerror at 0x14a0c. The normal return is 0. Its reduced width set is what the disassembly shows, and the reason for it is not stated.

- `baerror` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x149a8
  **Inferred.** This is the bus-error recording path that the probes branch to. It saves the error state at abs 0xffffffffb4000048 and 0xb4000250, reads the status word at 0xffffffffffffa018, and conditionally stores a halfword at [-0x5fe8]+0xc2 and a word at 0xffffffffffffa1d8 (0x149e4 through 0x149f8). It returns 1, which is the fault value the probes pass back.

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14a0c
  **Inferred.** This is the early-boot counterpart of `baerror`. It saves the same error state at abs 0xffffffffb4000048 and 0xb4000250, writes the marker at sym(nofault) at 0x14a4c, and returns 1. It records less than `baerror` because the disassembly shows no conditional halfword or word write.

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x15934
  **Inferred.** This is a 48-byte vector stub that forwards its arguments to ecc_exception with a jump at 0x15944. The code from 0x1594c to 0x15960 is not reached from the entry, so only the forwarding path is described. The stub adds no state of its own in the disassembly.

- `VEC_dbe` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1c2d4
  **Inferred.** This is the data bus error vector. It reads the CPU and memory error status and address words at abs 0xffffffffb4000040, 0xb4000048, 0xb4000250, 0xb4000258 and 0xb4000058, and copies them into the globals crm_cpu_error_stat, crm_cpu_error_addr, crm_mem_error_stat, crm_mem_error_addr and crm_vice_error_addr. It then clears the status words at 0xb4000250 and 0xb4000048, writes a value to the exception frame at a0+0x140, and jumps to VEC_trap at 0x1c350 with the arguments unchanged.

- `VEC_ibe` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x20dc0
  **Inferred.** This is the instruction bus error vector, and it captures the same error words as `VEC_dbe` into the same globals with the same clears. Two loads at 0x20dc0 and 0x20dc4 use addresses the disassembly does not resolve, so the first step of the vector is unclear. It then writes the exception frame at a0+0x140 and jumps to VEC_trap at 0x20e54.

- `_mraccess` in `6.5.30/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x35b8
  **Inferred.** This is a metering hook on the access side of a synchronisation object. It calls wchanmeter at 0x35d0 with the metering table sym(mrinfolist), then mr_access at 0x35e0, and it reads timestamps with _get_timestamp at 0x35f0 and 0x3678. It updates a counter at wchanmeter+0x138 with an ll/sc pair at 0x3650 and 0x365c, and it records an entry with mrmeter_getentry at 0x363c and mrmeter_entry at 0x3690 (event code 5). The name and the metering structure suggest wait-channel accounting, but the disassembly shows the fields and not their exact meaning.

- `_mrupdate` in `6.5.30/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x36e0
  **Inferred.** This is the update side of the same metering pair as `_mraccess`. It calls wchanmeter at 0x36f8 and mr_update at 0x3708, takes timestamps with _get_timestamp at 0x3718 and 0x37a4, and writes fields of the wait-channel meter at offsets +0x140 through +0x178. It records an entry with mrmeter_entry at 0x379c, using event code 6 where `_mraccess` uses 5. The purpose follows the pairing and the name, and the disassembly does not confirm what each field holds.

- `pckm_attach` in `6.5.30/usr/cpu/sysgen/IP32boot/mhpckm.o` at 0x10
  **Inferred.** This is the attach routine for a keyboard and mouse controller in the IP32 boot directory. It reads a device word through pciio_pio_read64 at 0xbf310000 (0x40) and sends a sequence of controller command bytes through pckm_pollcmd, including 0xf5, 0xf6, 0xed, 0xf2 and 0xfa. Several of those byte values match PS/2 keyboard and mouse commands, which is an inference from the values, not something the disassembly establishes. It makes four hwgraph_char_device_add calls (0x170, 0x204, 0x280, 0x2b0), sets their mode with chmod 0x1b6, records inventory with device_inventory_add, and installs the interrupt handler pckm_intr with setmaceisavector at 0x318.

- `doacvec` in `6.5.30/usr/cpu/sysgen/IP32boot/os.a(pda.o)` at 0x15fc
  **Inferred.** The routine reads two per-processor flag bytes at abs 0xffffffffffffa070 and 0xffffffffffffa071, writes back to 0xffffffffffffa03e and 0xffffffffffffa071, and reads and writes a doubleword at 0xffffffffffffa090. On one path it calls panicspin at 0x160c, and on another it flushes TLB entries with tlbflush_rand(1) at 0x1634 and tlbflush at 0x1648. The per-processor reading comes from the object name, and the disassembly does not show which condition selects panicspin.

- `syssgi` in `6.5.30/usr/cpu/sysgen/IP32boot/os.a(syssgi.o)` at 0xcc
  **Inferred.** This is the entry of the system call that dispatches on a command number in a0. Its reachable path calls get_current_abi at 0xf0 and then makes an indirect jump at 0x12c through a table at sym(.rodata)+112, and the table's targets are unresolved in the disassembly. The other reachable code is a return of 0x16 (EINVAL) at 0x15c and a short block at 0x2b84 through 0x2b98 that calls sat_never and sat_set_subsysnum, which are audit-subsystem names. The rest of the routine, from 0x164 to 0x2b80 and from 0x2b9c to 0x2e1c, is not reached through any resolved path, so the individual sub-commands are unknown.

- `dogetitimer` in `6.5.30/usr/cpu/sysgen/IP32boot/os.a(time.o)` at 0xa8
  **Inferred.** This is the read side of the per-process interval timer. It takes a per-process lock with mutex_lock at 0xe0 and releases it at 0x108, indexes a table of 16-byte timer entries by a0 (loads at 0x1cc through 0x1e4), and computes the remaining time with chktimeout_tick at 0x160 and 0x184. It converts the result with tick_to_timeval at 0x19c and writes 32-bit words into the caller's buffer at a1 (stores at 0xec, 0xf4, 0xfc, 0x10c and again at 0x1d0 through 0x1f0). It returns 0, with splprof and splx wrapping the work at 0xcc and 0x1a8.
