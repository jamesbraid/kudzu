# Routine role statements, kudzu IP30 kernel and boot objects

Role statements for 25 routines in the IP30 boot kernel object (`kernel.o`, release
6.5.30), written from their disassembled instructions only: the registers, calls,
constants and branches each routine shows. Every grade is inferred. The disassembly
does not say what the fields mean, so where that gap matters the statement says so.
Absolute addresses in the `0xffffffffaff...` and `0x9000000018...` ranges look like
hub and device register aliases, as the disassembly reports them.

- `debug_stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x38c8
  **Inferred.** `debug_stop_all_cpus` is a 140-byte leaf with no calls. It loops over CPU indices up
  to the `maxcpus` word and compares each with the halfword at abs -0x3c60, which the
  other routines here also read and which looks like the current CPU number. It stores
  two doublewords to 0xffffffffaff10020, a hub-range address, but the stored values are
  not resolved, so whether this halts or signals the other CPUs is open.

- `stop_bell` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x4108
  **Inferred.** `stop_bell` is a 52-byte wrapper that runs its body at a raised interrupt level. It
  calls `splhi` at 0x4110 and `splx` at 0x4128 around one word store to `.sdata`+64 at
  0x412c, and the stored value is not resolved. `pckbd_bell` schedules it through
  `timeout`, which fits the name, but nothing else here confirms what it is for.

- `pckbd_bell` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x413c
  **Inferred.** `pckbd_bell` is a 140-byte routine that sounds the keyboard bell. When its third
  argument (a2) is positive, it stores a word to `.sdata`+72 at 0x41b0 and calls
  `timeout` at 0x41b4 with `stop_bell` as the callback, so the silence happens later.
  A zero or negative a2 branches to 0x4184 instead. The disassembly does not show what
  a2 means (duration, pitch or a count), so that stays open.

- `counter_intr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x41f0
  **Inferred.** `counter_intr` is a 1724-byte handler for the timer compare interrupt. It calls
  `hwperf_overflowed_intr` at 0x4244 for the performance counter overflow case, and
  `frs_handle_counter_intr` at 0x421c and `hwperf_intr` at 0x4254 on other paths. It
  rearms the next tick through `_set_timer_intr` at 0x42cc, reads the count and compare
  registers through `_get_count` and `_get_compare` at 0x46e4 and 0x46ec, and emits
  `log_tstamp_event` records at 0x47a8 to 0x4800. A warning goes through `cmn_err` at
  0x488c, and the conditions that reach it are partly open.

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x5cbc
  **Inferred.** `doadjtime` is a 500-byte routine that applies a pending time-of-day adjustment. The
  sign of its first argument picks one of two paths, and zero goes to 0x5e24. Both paths
  take `timelock` with `nested_spinlock` and read the clock through `__nanotime` at 0x5d0c
  and 0x5e2c, then release the lock at 0x5dc4 and 0x5e98. The constants 0xf4240 and 0x3e8
  bound the step size, which looks like microseconds and milliseconds, but the stepping
  arithmetic is not shown.

- `__nanotime` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x5eb0
  **Inferred.** `__nanotime` is a 548-byte routine that returns the current time as seconds and
  nanoseconds. It holds `timelock` through `mutex_spinlock_spl` at 0x5ebc and releases it
  at 0x604c and 0x60c4. It reads the 64-bit counter at 0xffffffffaff20000, adds the
  `adjtime_delta` and `adjtime_RTC` words, and carries whole seconds at the 1e9
  nanosecond boundary (0x3b9aca00). The layout of the per-CPU record at `pdaindr` is
  inferred from offsets only.

- `findcpufreq_cycles` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x65e8
  **Inferred.** `findcpufreq_cycles` is a 52-byte routine that caches the CPU clock rate in cycles. It
  reads the word at -0x390c (abs 0xffffffffffffc6f4), and when that word is zero it calls
  `findcpufreq_hz` at 0x6604 and stores the result back at 0x6618. `startkgclock` reads
  the same cached word, which supports this reading, but the callee's units are not shown.

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x811c
  **Inferred.** `startkgclock` is a 408-byte routine that arms the periodic kernel clock on the current
  CPU. It reads the `fasthz` tick rate and calls `fasthzerr` at 0x817c, 0x8204 and 0x8278
  with 1000, 500 and 2500 as the third argument. The 500 and 2500 calls sit behind compares
  of `fasthz` against bounds, and after that it calls `set_timer_intr` at 0x81e0 with the
  CPU index, inside `splprof` and `splx`. What `fasthzerr` does with those values is not
  visible in this disassembly.

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x82b4
  **Inferred.** `slowkgclock` is a 40-byte critical-section wrapper. It calls `splprof` at 0x82bc, stores
  a doubleword to abs 0xffffffffffffc550 at 0x82cc, and restores the level through `splx`
  at 0x82c8. `startkgclock` stores to the same address, which suggests a shared slow-clock
  state word, but the stored value is not resolved.

- `cpu_waitforreset` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8680
  **Inferred.** `cpu_waitforreset` is a 48-byte spin loop. It reads a word at 0x900000001f000104 and
  keeps polling while bit 0x40 is clear. When the bit is set, it calls `cpu_soft_powerdown`
  at 0x86a0 and loops back to the poll, so the only exit is the bit change. What the bit
  reports is not shown.

- `_clock_func` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x88e4
  **Inferred.** `_clock_func` is a 524-byte routine for the clock device. It takes `ip30clocklock` at
  0x8904, saves the serial state with `slow_ioc3_sio` at 0x890c, and moves a 14-byte record
  between the caller's buffer at a1 and two byte-wide register windows at 0x900000001f6c0000
  and 0x900000001f6a0000. It restores the serial state through `restore_ioc3_sio` at 0x89f0
  and 0x8a50, and a zero first argument takes a separate path at 0x8a20. The meaning of each
  register byte is not established, and the serial-state calls suggest a shared device only
  from the callee names.

- `cpu_clearpower` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8e70
  **Inferred.** `cpu_clearpower` is a 112-byte routine that takes `ip30clocklock` and writes to the same
  two register windows as `_clock_func`. It writes four bytes in total, flushes the bus with
  `flushbus` at 0x8eb4 using arguments 0x30 and 0xa, and brackets the work with `slow_ioc3_sio`
  and `restore_ioc3_sio`. The bits written are not resolved, so this describes the access
  sequence, not the effect.

- `cpu_soft_powerdown` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8ee0
  **Inferred.** `cpu_soft_powerdown` is a 200-byte software power-off path. It calls the hook held in
  `fp_poweroff` through `jalr` at 0x8ef0, then runs the sequence at 0x8ef8. That sequence
  flushes the console buffer with `conbuf_flush`, updates the master CPU's Heart interrupt
  bits with `heart_imr_bits_rmw` at 0x8f0c, takes `ip30clocklock` at 0x8f30, and waits
  through `us_delay` at 0x8f98 with a count of 10,000,000. Whether the final write removes
  power is not shown by the disassembly.

- `slow_ioc3_sio` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8fa8
  **Inferred.** `slow_ioc3_sio` is a 32-byte helper. It reads the word at 0x900000001f600028, writes a
  word to the same address at 0x8fc4, and returns a value. The disassembly does not show
  whether the return sees the read or the write, and the stored value is not resolved.

- `restore_ioc3_sio` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8fc8
  **Inferred.** `restore_ioc3_sio` is a 32-byte helper and the counterpart of `slow_ioc3_sio`. It writes a
  byte to 0x900000001f6a0000, reads a byte from 0x900000001f6c0000, and writes a word to
  0x900000001f600028 at 0x8fe4. The disassembly gives no bit values, so the saved state and
  the meaning of each byte stay open.

- `nvchecksum` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x9020
  **Inferred.** `nvchecksum` is a 164-byte loop over a byte region. It writes five bytes to the index window
  at 0x900000001f6a0000 and three to the data window at 0x900000001f6c0000, and reads the data
  window back twice. The loop limits are 0x80 and 0x100 (128 and 256), which suggests a
  fixed-size block, but the checksum arithmetic and its output are not shown, so the name is
  only a guess.

- `badaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc080
  **Inferred.** `badaddr` is a 296-byte probe that reads 1, 2, 4 or 8 bytes at a0, with the width in a1
  chosen by a compare chain. It stores a halfword at offset 0xf6 of the structure at
  [-0x3fe0] before and after the access, which looks like a no-fault marker. Any other width
  is reported through `cmn_err` at 0xc144, and after the access it tests the hub error word
  at 0xffffffffaff10040 against the 0x4000 and 0x10000 bits, branching to `baerror` at 0xc788
  on a hit.

- `badaddr_val` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc1a8
  **Inferred.** `badaddr_val` is a 364-byte probe like `badaddr`, but it copies the value it reads. It
  loads 1, 2, 4 or 8 bytes from a0 and stores the same width to the destination, and it uses
  the same no-fault marker and hub error bit checks. Whether a bus error leaves the copied
  destination unchanged is not shown.

- `wbadaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc314
  **Inferred.** `wbadaddr` is a 332-byte write probe. It stores 1, 2, 4 or 8 bytes to a0 by width, then runs
  the same hub error checks and no-fault marker as `badaddr`. On one path it also reads a word
  from 0x9000000000000000, and the disassembly does not show why.

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc5c8
  **Inferred.** `earlybadaddr` is a 312-byte probe for early boot. It handles only 1, 2 and 4-byte widths,
  and any other width goes to a `cmn_err` report at 0xc688. It uses the global word `nofault`
  as its marker, not the -0x3fe0 structure.

- `baerror` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc788
  **Inferred.** `baerror` is a 92-byte bus-error recorder. It copies a doubleword between the
  0xffffffffb8000040 and 0xffffffffb8000060 slots, reads 0xffffffffb8000000 and
  0xffffffffaff50000, and stores a halfword to the -0x3fe0 marker at +0xf6 and a word to
  0xffffffffffffc404. It returns 1, which probably signals that a bus error was seen, and
  the meaning of the 0xb8 block is not established.

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc7e4
  **Inferred.** `earlybaerror` is the 96-byte early version of `baerror`. It does the same 0xb8 block copies
  and reads the hub register at 0xffffffffaff10040, then writes the `nofault` word and returns
  1. The disassembly does not show more than that.

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xdbb0
  **Inferred.** `_ecc_errorvec` is a 104-byte exception stub for ECC errors. It reads a value from
  0xffffffffaff50000, uses it to index a table at offset 0xff0, and stores two doublewords
  into that table slot. It then jumps indirectly through a register with the original
  arguments, and the jump target is not resolved from this disassembly.

- `dobuserr_common` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x13df4
  **Inferred.** `dobuserr_common` is a 9460-byte bus-error dispatcher for the Heart error path. It claims
  the per-CPU error bitmap `heart_error_cpumap` through `compare_and_swap_int` at 0x13ebc,
  and writes per-CPU entries into the `_heart_mode` table, indexed by the CPU number at
  -0x3c60. It classifies errors with `heart_error_refine`, called with a1 of 4, 5, 6, 9 or 10,
  and reports them through `hem_add_ioe` and `hem_add_sync`. It makes 128 `cmn_err` calls,
  nearly all with one format string, and what each error class means is open.

- `VEC_dbe` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x167e0
  **Inferred.** `VEC_dbe` is an 88-byte bus-error exception vector. It loads a value from 0xffffffffaff50000,
  uses it to index a doubleword table at a fixed negative offset (-0x500f0000), and stores a
  doubleword at a0+0x140, probably in the saved exception frame. It then jumps to `VEC_trap`
  at 0x16830, and the table contents and the stored value are not resolved from the disassembly.
