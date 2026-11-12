# Routine role statements, kudzu batch 16

Role statements for 25 routines from the ranked tier-1 list, restricted to hardware-linked rows (target indexes 400 to 424). Each statement comes only from the instruction facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the binaries' symbol tables. Grades are inferred throughout. Nothing here is hardware-verified, and where a register, device window or callee is unresolved the statement says so.

The binaries are `kernel.o` under `6.5.22/usr/cpu/sysgen/IP30boot/`, the `sema.o` member of `ksync_metered.a` and the `mgras.o` and `mgras_init.o` members of `mgras.a` in the same directory. The device windows at `0x900000001f...` and the global at `[-0x3fe0]` are named by address only; the instruction facts do not establish which device owns each one.

- `findcpufreq_cycles` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x6258
  **Inferred.** Loads a word from the global at 0xffffffffffffc6f4 and branches on it at 0x6260. When that word is zero, it calls findcpufreq_hz at 0x6274, passing a0, a1 and a3 as received and the loaded word as a2. The facts show a word store at 0x6288 but do not resolve its address or value, and the return value is the loaded word or an unresolved value. The name suggests a cached CPU frequency or cycle count, which fits the shape, but the facts do not show the units.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x7d8c
  **Inferred.** Raises interrupt priority with splprof at 0x7e2c, passing 0xf4240 (1,000,000) as a4, arms a timer with set_timer_intr at 0x7e50, and restores priority with splx at 0x7e58. Three error reports go through the pointer at 0x7d40 with third arguments 0x3e8, 0x1f4 and 0x9c4, and branches test the global at sym(fasthz) against zero and 0x1f4. The shape looks like clock-rate setup, but the timer argument's units are not established.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x7f24
  **Inferred.** A short wrapper that raises priority with splprof at 0x7f2c and restores it with splx at 0x7f38, passing the splprof result to splx and returning the splx result. It also stores an 8-byte value at 0x7f3c; the facts resolve neither the address nor the value, so what the store does is open. The name suggests a slower variant of the routine above.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x82f0
  **Inferred.** Polls a word at 0x900000001f000104 and spins while bit 0x40 is clear (the beq at 0x8308 loops back to the load at 0x8300). When the bit is set it calls cpu_soft_powerdown at 0x8310. The instruction facts list no return, so the routine appears to wait for reset rather than return by its own path. The bit's meaning comes from the name only.
  Reviewed: 2026-11-12 (sonnet)

- `_clock_func` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8554
  **Inferred.** Holds the ip30clocklock spinlock (mutex_spinlock_spl with splprof at 0x8574, released by mutex_spinunlock at 0x866c or 0x86cc) and wraps device-window accesses in a slow_ioc3_sio and restore_ioc3_sio pair. A branch at 0x859c on a0 (taken when a0 is zero) picks between two runs over offsets 0x0 through 0xc: halfword loads feeding byte stores from 0x85a8, and byte loads feeding halfword stores from 0x86dc. The facts do not resolve the base registers of those loads and stores. On the a0-zero path it tests bit 0x40 of the byte at 0x900000001f6c0000 at 0x86b8 and, when the bit is set, calls restore_ioc3_sio and mutex_spinunlock at 0x86c0 and 0x86cc. It returns 0 or 1. The name suggests a clock tick or time-of-day routine, but the meaning of the copied fields is not established.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_clearpower` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8ae0
  **Inferred.** Holds the same clock spinlock (mutex_spinlock_spl at 0x8aec, mutex_spinunlock at 0x8b3c) and brackets a bus flush (flushbus with a0 = 0x30 and a1 = 0xa at 0x8b24) with slow_ioc3_sio at 0x8af8 and restore_ioc3_sio at 0x8b2c. It stores single bytes at 0x8b0c, 0x8b14, 0x8b20 and 0x8b28; the facts resolve neither the target addresses nor the values. It returns the mutex_spinunlock result. The name suggests clearing a power-control latch, but the registers written and the byte values are unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_soft_powerdown` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8b50
  **Inferred.** Loads the pointer sym(fp_poweroff) at 0x8b58 and, when it is nonzero, calls through it at 0x8b60. It calls conbuf_flush with a0 = 3 at 0x8b68 and heart_imr_bits_rmw at 0x8b7c (a0 = the halfword at master_procid, a1 = 0x200000000000000, a2 = 0). It then takes ip30clocklock with mutex_spinlock_spl at 0x8ba0, calls slow_ioc3_sio at 0x8ba8, stores bytes (the facts do not resolve their target addresses or values), loads a byte at 0x8c04 (the us_delay call carries a byte from 0x900000001f6c0000 in a5) and calls us_delay with 0x989680 (10,000,000) at 0x8c08. The shape is a power-off sequence, but the delay's units, the meaning of the heart_imr_bits_rmw arguments and the meaning of the byte writes are not established.
  Reviewed: 2026-11-12 (sonnet)

- `slow_ioc3_sio` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8c18
  **Inferred.** Loads the word at 0x900000001f600028 (0x8c20) and returns it. It also stores a word at 0x8c34; the facts resolve neither that address nor the value. Its pairing with restore_ioc3_sio suggests it saves a setting for the duration of a window access, but the register's meaning is open.
  Reviewed: 2026-11-12 (sonnet)

- `restore_ioc3_sio` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8c38
  **Inferred.** Stores a byte at 0x8c48, loads a byte from 0x900000001f6c0000 at 0x8c4c and stores a word at 0x8c54, and returns the byte it loaded. The facts resolve neither store address nor either stored value. Paired with slow_ioc3_sio it looks like the restore half of a save and restore, but the values involved are unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `nvchecksum` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8c90
  **Inferred.** Contains two byte-access loops. The first stores bytes at 0x8cb0 through 0x8cc4, loads a byte at 0x8cc8 and loops back to 0x8cc0 while a2 is below 0x80 (signed, branch at 0x8ce4). The second stores bytes at 0x8cec through 0x8d00, loads a byte at 0x8d04 and loops back to 0x8cf4 while a5 is below 0x100 (signed, branch at 0x8d24). The facts do not resolve the load or store addresses. The name and the bounds suggest a checksum over a small non-volatile area, but the facts do not identify the data or the checksum formula.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xba80
  **Inferred.** A bus probe that loads at a width chosen by a1 (1, 2, 4 or 8, tested from 0xbad0 onward; lb, lh, lw or ld at 0xbad8 through 0xbb20) and returns 0 at 0xbba0 when neither error test branches away. Before the probe it runs doubleword load and store pairs (0xba98 through 0xbab4) whose addresses the facts do not resolve. It reads the doubleword at 0xffffffffaff10040 and branches to baerror at 0xc188 when bit 0x10000 of it is set (0xbb84), and also when bit 0x4000 of a word the facts do not resolve is set (0xbb68). An unsupported width appears to reach cmn_err at 0xbb44.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xbba8
  **Inferred.** The same shape as badaddr, with one difference in the facts: after each width-selected load (lb, lh, lw or ld between 0xbc10 and 0xbc70) the value is stored at the same width (sb, sh, sw or sd between 0xbc18 and 0xbc78); the store base is not resolved, so the facts do not show that it is a2. It returns 0, branches to baerror at 0xc188 on the same error-flag bits, and reports a bad width through cmn_err at 0xbc9c. The routine's name fits a value-returning probe, though the facts show no return value other than 0.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xbd14
  **Inferred.** A write probe: it stores at a width chosen by a1 (sb, sh, sw or sd between 0xbd6c and 0xbdb4), with the same error-flag test that routes to baerror and the same cmn_err on a bad width at 0xbdd8. It also loads a word at 0xbe10; the facts do not resolve its address or show why. The value stored is not named in the facts, so the probe's data is open.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xbfc8
  **Inferred.** An early-boot read probe with a narrower width set: lb, lh and lw at 0xc034 through 0xc064 handle widths 1, 2 and 4, and other widths appear to reach cmn_err at 0xc088. It stores a word at 0xc024 and again at 0xc0e8; the facts resolve neither the address nor the value, though the name pairing with earlybaerror suggests a flag the fault path reads. Errors branch to earlybaerror at 0xc1e4 (0xc0ac and 0xc0c8), and the routine returns 0.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc188
  **Inferred.** Copies doublewords in load and store pairs (0xc18c through 0xc1a8); the facts do not resolve those addresses. It then reads the doubleword global at [-0x3fe0] and, when it is nonzero (the beq at 0xc1bc falls through), stores a halfword at offset 0xf6 from it at 0xc1c4. A word store at 0xc1d0 runs on both paths, with an address the facts do not resolve. It returns 1, which appears to be what probe callers receive when they branch here instead of returning 0.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc1e4
  **Inferred.** Role (from name): the early-boot variant of baerror. It copies doublewords in three load and store pairs (0xc1e8 through 0xc204), loads one more doubleword at 0xc20c and stores a word at 0xc230; the facts do not resolve any of those addresses, so which globals it touches, and how it differs from baerror, is not established. It returns 1.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xd5b0
  **Inferred.** Loads a doubleword at 0xd5cc and another at offset 0xff0 from a base at 0xd5d4, then stores doublewords at offsets 0xc8 and 0x0 (0xd5d8 and 0xd5dc). It tail-jumps through an unresolved target at 0xd610 with a0 to a3 as received. The facts do not resolve the addresses read or the table indexing. The name suggests an ECC error vector dispatch, but the table contents and the jump target are not resolved.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x137f4
  **Inferred.** Role (from name): the common data-bus-error dispatcher. It reads the doublewords at 0xffffffffaff10040 and 0xffffffffaff10030, uses ll/sc loops (0x13880, 0x13d28, 0x13ef4 and 0x13f2c) and a compare_and_swap_int call at 0x138bc, tests the heart_error_cpumap word at 0x1386c, and calls heart_error_refine from five sites (0x13c2c, 0x13de0, 0x140a8, 0x1420c and 0x143c4), each with .rodata+5064 as a0. It also calls hem_add_sync (via the pointer at 0x134e0), hem_add_ioe, cmn_err (127 call sites) and heart_imr_bits_rmw at 0x13eac (a1 = 0, a3 = 0x8000000000000000). The volume of cmn_err calls suggests most paths are error text, but the classification logic is not established by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x161e0
  **Inferred.** Role (from name): the data-bus-error exception vector. It loads doublewords at 0x161e4, 0x1621c and 0x16224 and a halfword at 0x1622c, and loads and stores a doubleword at offset -0x500f0000 from a base (0x16200 and 0x16214); the facts do not resolve the base addresses. It tail-jumps to VEC_trap at 0x16230 with a0 to a3 as received, and stores a doubleword at offset 0x140 at 0x16234, also with an unresolved base.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x17854
  **Inferred.** Role (from name): a cache-error handler. It works on a record at a1, reading and writing fields at offsets from 0x8 through 0x34 and at 0x238, and formats a register with cer_to_hex at 0x178b0. It calls ecc_cacheOp through the pointer at 0x17238 with a0 codes 0x0, 0x1, 0x3, 0x4, 0x5, 0x7 and 0x13, and calls ecc_force_cache_line_invalid on the table at 0xa800000000000000. It calls maddr_to_dimm at 0x17dd8 and sprintf at 0x17e1c with cacheErr_frames as a0 and the maddr_to_dimm result as a3. The facts do not name the processor, and the meaning of each op code is inferred from the call pattern.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1b7f0
  **Inferred.** Role (from name): the instruction-bus-error exception vector, a sibling of VEC_dbe. It has the same table access offsets (0x1b810 through 0x1b83c) and the same frame store offset at 0x1b85c, and it tail-jumps to VEC_trap at 0x1b858. A sign test on ra at 0x1b7f8 chooses between two paths at 0x1b804, but the facts do not show what that choice selects.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** Role (from name): a metering hook for an access to a sync object. It calls wchanmeter at 0x38b0 (a1 = syncinfo). When the result is zero (the fall-through of the bne at 0x38b8) it calls mr_access at 0x38c0; a nonzero result branches to 0x395c. It calls mrmeter_getentry at 0x391c (a2 = 5, a3 = 1) and mrmeter_entry at 0x3974 (a1 = 5, with a doubleword loaded from 0xffffffffaff20000 in a7). It stores fields at offsets 0x1c0, 0x1c8, 0x1cc and 0x1d0 and updates offset 0x198 with an ll/sc pair at 0x3930 and 0x393c. The facts do not say which paths correspond to an existing or a new record, and the meaning of the counter read from that register is not established.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x39cc
  **Inferred.** Role (from name): the update-side counterpart of _mraccess. It calls wchanmeter at 0x39ec. When the result is zero (the fall-through of the bne at 0x39f4) it calls mr_update at 0x39fc; a nonzero result branches to 0x3a88. It calls mrmeter_entry at 0x3a94 with a1 = 6 (_mraccess passes 5). It stores fields at offsets 0x1a0, 0x1a8, 0x1b0, 0x1b4, 0x1b8 and 0x1d8 and tests bit 0x4 of register a3 at 0x3a60. The facts do not say which paths correspond to an existing or a new record, and the meaning of the type codes and of that bit is not established.
  Reviewed: 2026-11-12 (sonnet)

- `MgrasGeneralInterrupt` in `6.5.22/usr/cpu/sysgen/IP30boot/mgras.a(mgras.o)` at 0x7fa4
  **Inferred.** Role (from name): the interrupt handler for the MGRAS graphics board. It loads a pointer from a0+0xd0 and reads and writes words in the block it points to at offsets 0x11020 through 0x11048. It calls vsema at 0x7fe4, 0x8018 and 0x8128, each after a sign test (0x7fd8, 0x800c, 0x811c) that finds a negative value, not after a status-bit test. It calls MgrasSchedSwapBufFinish at 0x8044 when bit 0x400 of s0 is set (0x802c). It calls cmn_err_tag with codes 0xaa6, 0xaa7, 0xaa8 and 0xaa9 when bits 0x200, 0x2000, 0x4000 and 0x8000 of s0 are set (0x8074, 0x8098, 0x80b8, 0x80e0). On a separate path entered by the bltz at 0x8068 it calls cmn_err_tag with code 0xaa5 and then cmn_err with doublewords read from 0xffffffffaff00040 through 0xffffffffaff00080, 0xffffffffaff10030 and 0xffffffffaff10038. The facts do not show that s0 is an interrupt-status word, do not map each bit to its event, and do not resolve the address or value of the store at 0x82b0.
  Reviewed: 2026-11-12 (sonnet)

- `Mgras_FC_Disable` in `6.5.22/usr/cpu/sysgen/IP30boot/mgras.a(mgras_init.o)` at 0x1230
  **Inferred.** Loads a doubleword at 0x1234, stores a doubleword at 0x1244 and returns the block base 0xffffffffaff00000 (the return is at 0x1240). The facts resolve neither the load address nor the store address, so they do not show that the two touch the same word, and they do not show the stored value. The name suggests turning off the MGRAS frame controller, but the facts do not show which bit changes, so the effect is open.
  Reviewed: 2026-11-12 (sonnet)
