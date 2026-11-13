# Routine role statements, tier-1 batch 38

Each entry below gives a role reading for one routine, drawn only from its disassembly (registers, calls, constants and branches at the cited addresses). Every role here is inferred: the disassembly shows the instruction-level shape, and the purpose is read from that shape plus the symbol name. Where the disassembly stops short of a purpose, the entry says so.

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb624
  **Inferred.** The routine tests a flag word at sdata+156 with the branch at 0xb62c. When the flag is nonzero it calls init_timer at 0xb634 and then makes an indirect call at 0xb640 whose target the disassembly does not resolve. The shape matches the neighbouring acknowledge routine, so it is probably one half of a clock hook pair.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xb718
  **Inferred.** This is the same shape as `slowkgclock` at a fixed offset: the same flag test at 0xb720, a call to init_timer at 0xb728, and an unresolved indirect call at 0xb734. The name suggests an acknowledge step, but the disassembly shows only the call pattern, not what the indirect target does.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xc160
  **Inferred.** The routine calls splhi at 0xc1a8 and 0xc280 and splx at 0xc1f0 and 0xc2a8, on two separate paths, so each path raises and restores the interrupt priority level. A signed branch at 0xc230 compares a1 (holding 0xafc80 there) with s0; the facts show a forward target, not a loop. The constants 0x4650 and 0x190 appear as register values at the splhi call at 0xc1a8, and 0x190 again at 0xc280. Role (from name): the name suggests a time-of-day slew, but the facts do not show what the adjustment arithmetic computes.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0xd894
  **Inferred.** Role (from name): this is the memory and cache ECC error handler. The facts show it loading from abs 0xffffffffb4000048, 0xb4000250, 0xb4000040 and 0xb4000258 (loads at 0xd9ec through 0xda24) and storing them into the structure at a1 (offsets +0x38, +0x40, +0x50, +0x58). It calls _r4600sc_disable_scache at 0xd8f8 on the path where two_set_pcaches (branch at 0xd8e4) and the word at [-0x5d50] (branch at 0xd8f0) are both nonzero, and calls _r4600sc_enable_scache at 0xdddc. In between it calls the resolved routines ecc_get_perr_addr (0xd754) at 0xda80, ecc_log_error (0xeac0) at 0xdb38 and ecc_assign_msg (0xe8fc) at 0xdc4c, calls timeout with ecc_cleanup and a2=-0x64 at 0xdc98, and calls ecc_fixmem at 0xdd68 and ecc_fixcache at 0xdd80. The facts do not show which branch selects each call.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10754
  **Inferred.** Role (from name): this is the shared bus-error path. It calls _r4600sc_disable_scache at 0x1079c only when two_set_pcaches and the word at [-0x5d50] are both nonzero, calls get_crimerev at 0x107a8, and calls print_buserr_rev1 at 0x107c0 when that returns nonzero or print_buserr_rev0 at 0x1094c when it returns zero. Later calls include print_memerr at 0x107ec, log_ecc_err at 0x10980 and ecc_error_correction at 0x10994, and _r4600sc_enable_scache at 0x109b4 and 0x10a60. The instruction at 0x10878 is not reached from the entry. The only return, at 0x108ec, has v0 equal to 0 or 1.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10eac
  **Inferred.** The routine does a load and a store of 8 bytes at abs 0xffffffffbf310008, with a branch at 0x10eb8 that depends on a0 and a second branch at 0x10ed8 that is always taken. The disassembly shows the register location and the dependence on the argument, but not the bit pattern written. The name suggests a front-panel LED write, which fits a single status register, but no call or constant confirms it.
  Reviewed: 2026-11-12 (sonnet)

- `debug_stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10f30
  **Inferred.** The routine spans 0x10f30 to 0x10f38, eight bytes, and consists of a return at 0x10f30 with no calls, no loads and no branches in the disassembly. The disassembly leaves v0 unset at that return, so no return value is shown. The name suggests a debugger hook that stops the other processors, but nothing in the body does that. The function is effectively a stub in this object.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1121c
  **Inferred.** The routine reads byte registers in the 0xbf3a0xxx range (abs 0xffffffffbf3a0a07 at 0x1126c and 0x11294, 0xbf3a4a07 at 0x1127c and 0xbf3a0c07 at 0x112c0), writes bytes back to 0xbf3a0a07 at 0x11278 and 0x1129c, and loads the doubleword at 0xbf310010. It tests bit 0 of 0xbf3a4a07 at 0x11284, bit 0x80 of 0xbf3a0c07 at 0x112c8 and bit 0x100 of 0xbf310010 at 0x112b8, and the branches at 0x112b8 and 0x112c8 jump backwards, so it loops. It calls flash_write_env at 0x11238, for which the facts list no deciding branch, and flushbus at 0x11274. It calls cpu_powerdown at 0x1128c only when bit 0 of 0xbf3a4a07 is set. It calls spl7 at 0x112a8 and splx at 0x112a0, and cmn_err at 0x112d0. The facts do not give the meaning of the flash and power-down steps beyond the callee names.
  Reviewed: 2026-11-12 (sonnet)

- `_clock_func` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x129e8
  **Inferred.** The routine reads or writes an 8-byte record at a1 through byte registers at abs 0xbf3a0007, 0xbf3a0207, 0xbf3a0407, 0xbf3a0707, 0xbf3a0807, 0xbf3a0907, 0xbf3a0607 and 0xbf3a4807, using pciio_pio_read8 and pciio_pio_write8. The branch at 0x12a50 on a0 selects the direction: when a0 is zero it reads each register and stores the byte into the record at a1 (sb stores at 0x12a94 through 0x12b00), then skips the write code (branch at 0x12afc); when a0 is nonzero it loads each record byte (lbu at 0x12b50 onward) and writes it back with pciio_pio_write8 (calls at 0x12b54 through 0x12bb8). The read path first reads and writes back 0xbf3a0b07 and 0xbf3a0a07 (calls at 0x12a58 through 0x12a80); the write path does so at 0x12b04 through 0x12b48. Both paths end with a read and write of 0xbf3a0b07 at 0x12be0 and 0x12bf4, then splx at 0x12c00. splhi is called at 0x129f8, and a zero word at sdata+252 (branch at 0x12a24) calls rtc_init at 0x12c18, after which the code continues at 0x12a30.
  Reviewed: 2026-11-12 (sonnet)

- `_ticksper1024inst` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x140c0
  **Inferred.** This is a short spin loop that reads a 64-bit value at abs 0xffffffffb4000038 with ld at 0x140d0 and 0x140dc. The loop continues while registers t1 and t3 stay positive, using signed branches at 0x140d4 and 0x140e8. The name suggests a count of ticks per 1024 instructions, but the disassembly shows no arithmetic that would produce that count, so the purpose is only a guess from the name.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14270
  **Inferred.** This is a probe that reads a value at address a0 at a width selected by a1 (1, 2, 4 or 8 bytes, dispatched by the branches at 0x142d4 through 0x1431c). Around the read it saves the bus-error state at abs 0xffffffffb4000048 and 0xb4000250 and tests the error bits in the word at 0xb4000028 with mask 0x300080 at 0x14374. When the mask is set it branches to baerror at 0x149a8, and otherwise it returns 0, so 0 reads as "no fault". The value returned on a fault comes from baerror, which the disassembly shows returning 1.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x143a8
  **Inferred.** This is the same probe as `badaddr`, with the same width dispatch and error-bit test, but it copies the value it reads into the location at a2 (the stores at 0x1442c through 0x1448c) instead of returning it. The fault check at 0x144dc branches to baerror at 0x149a8, and the normal return is 0. The shared structure with `badaddr` suggests the two were written together, but the disassembly does not show that.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14524
  **Inferred.** This is the write-side probe: it stores a value of width a1 to address a0 (stores at 0x14590 through 0x145d8) under the same bus-error bookkeeping. It also loads from abs 0xffffffffa0000000 at 0x14638 and checks the error mask at 0x14654, branching to baerror on a fault. The normal return is 0. The disassembly shows that load but not its purpose, so whether it verifies the write is not established.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14808
  **Inferred.** This is an early-boot variant of the probe in `badaddr`. It dispatches on widths 1, 2 and 4 only, with no 8-byte case in the branches at 0x14870 through 0x148a0. It stores a word to sym(nofault) at 0x14868 and again at 0x14908 (the values are not shown), and on a fault (error-mask test at 0x148f8) it branches to earlybaerror at 0x14a0c. The normal return is 0. The reduced width set is what the facts show; the reason for it is not stated.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x149a8
  **Inferred.** This is the bus-error recording path that the probes branch to. It stores to abs 0xffffffffb4000048 and 0xb4000250 and reloads both, stores and reloads a word at abs 0xffffffffbf080004, and loads a word from abs 0xffffffffffffa018. It stores a halfword at [-0x5fe8]+0xc2 only when that pointer word is nonzero (branch at 0x149e4), and stores a word to abs 0xffffffffffffa1d8 at 0x149f8 on either path. It returns 1, which is the fault value the probes pass back.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14a0c
  **Inferred.** This is the early-boot counterpart of `baerror`. It makes the same stores and reloads at abs 0xffffffffb4000048 and 0xb4000250 and at abs 0xffffffffbf080004, stores a word to sym(nofault) at 0x14a4c, and returns 1. Unlike `baerror` it has no branch, no halfword store at [-0x5fe8]+0xc2 and no access to 0xffffffffffffa018 or 0xffffffffffffa1d8.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x15934
  **Inferred.** This is a 48-byte vector stub that forwards its arguments to ecc_exception with a jump at 0x15944. The code from 0x1594c to 0x15960 is not reached from the entry, so only the forwarding path is described. The stub adds no state of its own in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1c2d4
  **Inferred.** This is the data bus error vector. It loads from abs 0xffffffffb4000040, 0xb4000048, 0xb4000250, 0xb4000258 and 0xb4000058 and stores each to the globals crm_cpu_error_stat, crm_cpu_error_addr, crm_mem_error_stat, crm_mem_error_addr and crm_vice_error_addr in that order, with 0x58 going to crm_vice_error_addr. It then stores 8 bytes to abs 0xb4000250 and 0xb4000048 (the stored values are not shown, so whether this clears them is not established), stores a doubleword to the frame at a0+0x140, and jumps to VEC_trap at 0x1c350 with a0 through a3 unchanged.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x20dc0
  **Inferred.** This is the instruction bus error vector. It makes the same loads from abs 0xffffffffb4000040, 0xb4000048, 0xb4000250, 0xb4000258 and 0xb4000058 and the same stores to the crm_* globals and to abs 0xb4000250 and 0xb4000048 as `VEC_dbe` (stored values not shown). Two loads at 0x20dc0 and 0x20dc4 and a store at 0x20dd4 use addresses the facts do not resolve, with a branch at 0x20dc8 on ra, so the first step of the vector is unclear. It then stores a doubleword to a0+0x140 and jumps to VEC_trap at 0x20e54 with a0, a1 and a3 unchanged and a2 not shown.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.30/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x35b8
  **Inferred.** This is a metering hook on the access side of a synchronisation object. It calls wchanmeter at 0x35d0 with the metering table sym(mrinfolist), then mr_access at 0x35e0, and it reads timestamps with _get_timestamp at 0x35f0 and 0x3678. It updates a counter at wchanmeter+0x138 with an ll/sc pair at 0x3650 and 0x365c, and it records an entry with mrmeter_getentry at 0x363c and mrmeter_entry at 0x3690 (event code 5). The name and the metering structure suggest wait-channel accounting, but the disassembly shows the fields and not their exact meaning.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.30/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x36e0
  **Inferred.** This is the update side of the same metering pair as `_mraccess`. It calls wchanmeter at 0x36f8 and mr_update at 0x3708, takes timestamps with _get_timestamp at 0x3718 and 0x37a4, and writes fields of the wait-channel meter at offsets +0x140 through +0x178. It records an entry with mrmeter_entry at 0x379c, using event code 6 where `_mraccess` uses 5. The purpose follows the pairing and the name, and the disassembly does not confirm what each field holds.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.30/usr/cpu/sysgen/IP32boot/mhpckm.o` at 0x10
  **Inferred.** This is the attach routine for a keyboard and mouse controller in the IP32 boot directory. It reads a device word through pciio_pio_read64 at 0xbf310000 (0x40) and sends a sequence of controller command bytes through pckm_pollcmd, including 0xf5, 0xf6, 0xed, 0xf2 and 0xfa. Several of those byte values match PS/2 keyboard and mouse commands, which is an inference from the values, not something the disassembly establishes. It makes four hwgraph_char_device_add calls (0x170, 0x204, 0x280, 0x2b0), sets their mode with chmod 0x1b6, records inventory with device_inventory_add, and installs the interrupt handler pckm_intr with setmaceisavector at 0x318.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.30/usr/cpu/sysgen/IP32boot/os.a(pda.o)` at 0x15fc
  **Inferred.** The routine loads bytes from abs 0xffffffffffffa070 and 0xffffffffffffa071, a word from 0xffffffffffffa074 and a doubleword from 0xffffffffffffa090. It calls panicspin at 0x160c when the byte at a070 is nonzero. When the byte at a071 is nonzero it stores bytes to 0xffffffffffffa03e and 0xffffffffffffa071. It calls tlbflush_rand(1) at 0x1634 when the word at a074 is nonzero, calls tlbflush at 0x1648 when the doubleword at [-0x5f70] is nonzero, and stores a doubleword to a090 at 0x164c after that call. The per-processor reading comes from the object name.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.30/usr/cpu/sysgen/IP32boot/os.a(syssgi.o)` at 0xcc
  **Inferred.** This is the entry of the system call that dispatches on a command number, which the facts show loaded from the word at a0 (a0 is dereferenced at 0x10c). It calls get_current_abi at 0xf0. When the loaded value is below 0x4ed it makes an indirect jump at 0x12c through a table at sym(.rodata)+112, whose targets are unresolved; when it is 0x4ed or above the branch goes to 0x134, and the facts show a return of 0x16 (EINVAL) at 0x15c. When sat_enabled is nonzero (branch at 0x104) it runs a short block at 0x2b84 through 0x2b98 that calls sat_never and sat_set_subsysnum, whose names suggest audit handling. The instructions from 0x164 to 0x2b80 and from 0x2b9c to 0x2e1c are not reached through any resolved path, so the individual sub-commands are unknown.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.30/usr/cpu/sysgen/IP32boot/os.a(time.o)` at 0xa8
  **Inferred.** Role (from name): the read side of the per-process interval timer. The branch at 0xc4 on a0 splits two paths. When a0 is zero it calls splprof at 0xcc, takes mutex_lock at 0xe0 and mutex_unlock at 0x108, stores words into the caller's buffer at a1 (stores at 0xec, 0xf4, 0xfc, 0x10c), calls chktimeout_tick at 0x160 and 0x184 on different paths, converts with tick_to_timeval at 0x19c, and calls splx at 0x1a8. When a0 is nonzero it calls splhi at 0x1b8, loads four words from the table at a2 + (a0 << 4) + 0x220 (loads at 0x1cc through 0x1e4), stores them to a1 (0x1d0 through 0x1f0) and calls splx at 0x1ec. The only return, at 0x204, has v0 = 0.
  Reviewed: 2026-11-12 (sonnet)
