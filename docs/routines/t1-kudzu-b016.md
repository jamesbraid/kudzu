# Routine role statements, kudzu batch 16

Role statements for 25 routines from the ranked tier-1 list, restricted to hardware-linked rows (target indexes 400 to 424). Each statement comes only from the instruction facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the binaries' symbol tables. Grades are inferred throughout. Nothing here is hardware-verified, and where a register, device window or callee is unresolved the statement says so.

The binaries are `kernel.o` under `6.5.22/usr/cpu/sysgen/IP30boot/`, the `sema.o` member of `ksync_metered.a` and the `mgras.o` and `mgras_init.o` members of `mgras.a` in the same directory. The device windows at `0x900000001f...` and the global at `[-0x3fe0]` are named by address only; the instruction facts do not establish which device owns each one.

- `findcpufreq_cycles` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x6258
  **Inferred.** Loads a word from the global at 0xffffffffffffc6f4. When that word is zero, it calls findcpufreq_hz at 0x6274 with the incoming arguments, and it stores a value to the same word at 0x6288, after an always-taken branch at 0x6284. The name suggests a cached CPU frequency or cycle count, which fits the shape, but the facts do not show the units.

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x7d8c
  **Inferred.** Raises interrupt priority with splprof at 0x7e2c, passing 0xf4240 (1,000,000) as a4, arms a timer with set_timer_intr at 0x7e50, and restores priority with splx at 0x7e58. Three error reports go through the pointer at 0x7d40 with third arguments 0x3e8, 0x1f4 and 0x9c4, and branches test the global at sym(fasthz) against zero and 0x1f4. The shape looks like clock-rate setup, but the timer argument's units are not established.

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x7f24
  **Inferred.** A short wrapper that raises priority with splprof at 0x7f2c, stores an 8-byte value to the global at 0xffffffffffffc550 at 0x7f3c, and restores priority with splx at 0x7f38. The instruction facts do not show the stored value, so what the store does is open. The name suggests a slower variant of the routine above.

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x82f0
  **Inferred.** Polls a word at 0x900000001f000104 and spins while bit 0x40 is clear. When the bit is set it calls cpu_soft_powerdown at 0x8310, and the always-taken branch at 0x8318 returns to the poll. The instruction facts list no return, so the routine appears to wait for reset rather than return by its own path. The bit's meaning comes from the name only.

- `_clock_func` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8554
  **Inferred.** Holds the ip30clocklock spinlock (mutex_spinlock_spl with splprof at 0x8574) and wraps device-window accesses in a slow_ioc3_sio and restore_ioc3_sio pair. Inside, it copies halfwords between the argument buffer a1 and offsets 0x0 through 0xc, and it tests a bit of the window at 0x900000001f6c0000 at 0x86b8. It returns 0 or 1. The name suggests a clock tick or time-of-day routine, but the meaning of the copied fields is not established.

- `cpu_clearpower` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8ae0
  **Inferred.** Holds the same clock spinlock and brackets a bus flush (flushbus with 0x30 and 0xa at 0x8b24) with slow_ioc3_sio and restore_ioc3_sio. It writes single bytes to the windows at 0x900000001f6a0000 and 0x900000001f6c0000 between 0x8b0c and 0x8b28. The name suggests clearing a power-control latch, but the byte values and the register behind each window are unresolved.

- `cpu_soft_powerdown` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8b50
  **Inferred.** Reads the optional hook through the pointer sym(fp_poweroff) at 0x8b60 and calls it when the pointer is set. It flushes the console buffer with conbuf_flush(3) at 0x8b68 and updates an interrupt mask for the master processor with heart_imr_bits_rmw at 0x8b7c. It then writes bytes to both device windows, reads one back, and calls us_delay with 0x989680 (10,000,000) at 0x8c08. The shape is a power-off sequence, but the delay's units and the meaning of the byte writes are not established.

- `slow_ioc3_sio` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8c18
  **Inferred.** Reads the word at 0x900000001f600028 and returns it, and stores a word to the same address at 0x8c34. The facts do not resolve the stored value. Its pairing with restore_ioc3_sio suggests it saves a setting for the duration of a window access, but the register's meaning is open.

- `restore_ioc3_sio` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8c38
  **Inferred.** Writes a byte to 0x900000001f6a0000 at 0x8c48, reads a byte from 0x900000001f6c0000 at 0x8c4c, and writes a word to 0x900000001f600028 at 0x8c54. It returns the byte it read. Paired with slow_ioc3_sio, it looks like the restore half of a save and restore, but the values involved are unresolved.

- `nvchecksum` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8c90
  **Inferred.** Loops over bytes read from the 0x900000001f6c0000 window (lbu at 0x8cc8 and 0x8d04) and writes bytes to both windows inside the loop. Its branches compare a2 with 0x80 and a5 with 0x100 (at 0x8ce4 and 0x8d24). The name and the bounds suggest a checksum over a small non-volatile area, but the facts do not identify the data or the checksum formula.

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xba80
  **Inferred.** A bus probe that reads from a0 at a width chosen by a1 (1, 2, 4 or 8, tested from 0xbad0 onward) and returns 0 when no error flag is set. Before the probe it saves doublewords through 0xffffffffb8000040 and 0xffffffffb8000060, and it reads the error block at 0xffffffffaff10040 and 0xffffffffaff50000. It branches to baerror at 0xc188 when bit 0x4000 or 0x10000 is set, and an unsupported width appears to reach cmn_err at 0xbb44.

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xbba8
  **Inferred.** The same probe as badaddr, except the loaded value is stored to a2 at the same width (sb, sh, sw or sd between 0xbc18 and 0xbc78) rather than returned. It returns 0, branches to baerror at 0xc188 on the same error-flag bits, and reports a bad width through cmn_err at 0xbc9c. The routine's name fits a value-returning probe, though the facts show no return value other than 0.

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xbd14
  **Inferred.** A write probe: it stores to a0 at a width chosen by a1 (sb, sh, sw or sd between 0xbd6c and 0xbdb4), with the same error-flag test that routes to baerror and the same cmn_err on a bad width at 0xbdd8. It reads a word from the fixed address 0x9000000000000000 at 0xbe10, and the facts do not show why. The value stored is not named in the facts, so the probe's data is open.

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xbfc8
  **Inferred.** An early-boot read probe with a narrower width set: lb, lh and lw at 0xc034 through 0xc064 handle widths 1, 2 and 4, and other widths appear to reach cmn_err at 0xc088. It stores to sym(nofault) at 0xc024 and 0xc0e8, which suggests a flag the fault path reads, though the stored values are unresolved. Errors branch to earlybaerror at 0xc1e4, and the routine returns 0.

- `baerror` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc188
  **Inferred.** Copies doublewords among the error block addresses 0xffffffffb8000000, 0xffffffffb8000040, 0xffffffffb8000060, 0xffffffffaff10040 and 0xffffffffaff50000, and reads 0xffffffffffffc020. It stores a halfword to [-0x3fe0]+0xf6, and it writes a word to 0xffffffffffffc404 only when the global at [-0x3fe0] is nonzero (beq at 0xc1bc). It returns 1, which appears to be what probe callers receive when they branch here instead of returning 0.

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc1e4
  **Inferred.** The early-boot version of baerror. It copies the same error-block doublewords (0xb8000060 to 0xb8000040, 0xb8000000 to 0xb8000060, and the 0xaff10040 and 0xaff50000 pair) and stores a word to sym(nofault) at 0xc230. It does not touch the [-0x3fe0] global or the 0xffffffffffffc404 word, and it returns 1. That difference suggests the early path avoids that global, but the facts do not say why.

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xd5b0
  **Inferred.** Reads a word at 0xffffffffaff50000 and uses it as an index, scaled by 8 and offset by 0xff0, to select an 8-byte table entry. It loads that entry and stores two doublewords into it at offsets 0x0 and 0xc8, then jumps through a register at 0xd610 with the incoming arguments passed on. The name suggests an ECC error vector dispatch, but the table contents and the jump target are not resolved.

- `dobuserr_common` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x137f4
  **Inferred.** The common data-bus-error dispatcher. It reads HEART error registers at 0xffffffffaff00000 and 0xffffffffaff10040, claims a per-CPU slot in heart_error_cpumap with an ll/sc loop (0x13880 through 0x13888), and classifies the event with heart_error_refine (five call sites, among them 0x13c2c and 0x13de0) against a table at .rodata+5064. Reporting goes through hem_add_sync (via the pointer at 0x134e0), hem_add_ioe and cmn_err, which is called 127 times, and heart_imr_bits_rmw at 0x13eac changes an interrupt mask. The volume of cmn_err calls suggests most paths are error text, but the classification logic is not established by the facts.

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x161e0
  **Inferred.** The data-bus-error exception vector. It indexes a table by the word at 0xffffffffaff50000 and loads and stores one doubleword at that entry offset by -0x500f0000. It reads 0xffffffffb8000000 and the halfword at 0xffffffffffffc3a0, stores a doubleword into the frame at a0+0x140, and jumps to VEC_trap at 0x16230 with the same arguments.

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x17854
  **Inferred.** A cache-error handler that works on the record at a1, reading and writing fields at offsets 0x0 through 0x34 and 0x238, and formats a register with cer_to_hex at 0x178b0. It calls cache operations through the pointer at 0x17238 with op codes 0x0, 0x1, 0x3, 0x4, 0x5, 0x7 and 0x13, and it forces line invalidation with ecc_force_cache_line_invalid on the table at 0xa800000000000000. It maps the physical address to a DIMM with maddr_to_dimm at 0x17dd8 and formats the report into cacheErr_frames with sprintf at 0x17e1c. The facts do not name the processor, and the meaning of each op code is inferred from the call pattern.

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1b7f0
  **Inferred.** The instruction-bus-error exception vector, a sibling of VEC_dbe. It has the same table access (0x1b810 through 0x1b83c) and the same frame store at 0x1b85c, and it jumps to VEC_trap at 0x1b858. A sign test on ra at 0x1b7f8 chooses between two paths at 0x1b804, but the facts do not show what that choice selects.

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** A metering hook for an access to a sync object. It finds the meter record through wchanmeter at 0x38b0 and updates fields at offsets 0x198, 0x1c0, 0x1c8 and 0x1d0, using an ll/sc pair on offset 0x198 (0x3930 and 0x393c). It calls mr_access at 0x38c0 for an existing record, or mrmeter_getentry at 0x391c and mrmeter_entry at 0x3974 when the entry is new, and it reads 0xffffffffaff20000 twice. The meaning of the counter read from that register is not established.

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP30boot/ksync_metered.a(sema.o)` at 0x39cc
  **Inferred.** The update-side counterpart of _mraccess. It finds the meter record through wchanmeter at 0x39ec, calls mr_update at 0x39fc, and on a new entry calls mrmeter_entry at 0x3a94 with type code 6 (the access hook passes 5). It updates fields at offsets 0x1a0, 0x1a8, 0x1b0, 0x1b4, 0x1b8 and 0x1d8, and tests bit 0x4 of a3 at 0x3a60. The meaning of the type codes and of that bit is not established.

- `MgrasGeneralInterrupt` in `6.5.22/usr/cpu/sysgen/IP30boot/mgras.a(mgras.o)` at 0x7fa4
  **Inferred.** The interrupt handler for the MGRAS graphics board. It loads a per-device pointer from a0+0xd0 and reads and writes words in that state block at offsets 0x11020 through 0x11048, and it reads a run of doublewords at 0xffffffffaff00040 through 0xffffffffaff00080, plus 0xffffffffaff10030 and 0xffffffffaff10038. It dispatches on bits of an interrupt-status word (0x400, 0x200, 0x2000, 0x4000 and 0x8000) to post semaphores with vsema at 0x7fe4, 0x8018 and 0x8128, calls MgrasSchedSwapBufFinish at 0x8044, and reports error codes 0xaa5 through 0xaa9 through cmn_err_tag. The facts do not map each bit to its event, and the 0xffffffffaff40008 store at 0x82b0 has an unresolved value.

- `Mgras_FC_Disable` in `6.5.22/usr/cpu/sysgen/IP30boot/mgras.a(mgras_init.o)` at 0x1230
  **Inferred.** Loads a doubleword at 0xffffffffaff00040 at 0x1234 and stores a doubleword to the same address at 0x1244, then returns the block base 0xffffffffaff00000. The name suggests turning off the MGRAS frame controller, but the facts do not show which bit changes or what value is stored, so the effect is open.
