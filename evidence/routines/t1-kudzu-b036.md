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
  two doublewords to 0xffffffffaff10020, an address the facts do not identify, and the stored values are
  not resolved, so whether this halts or signals the other CPUs is open.
  Reviewed: 2026-11-12 (sonnet)

- `stop_bell` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x4108
  **Inferred.** `stop_bell` is a 52-byte routine. It calls `splhi` at 0x4110, stores zero to the gp-relative
  word at gp-0x7fc0 at 0x4120, calls `splx` at 0x4128, and in that call's delay slot (0x412c) stores the word
  0x100000 through the pointer held at `.sdata`+64. Role (from name): ends a keyboard bell. `pckbd_bell` loads
  the same gp-0x7fc0 word and stores its `timeout` result there, and passes `stop_bell` to `timeout`, which
  fits the name, but nothing else here confirms what it is for.
  Reviewed: 2026-11-12 (sonnet)

- `pckbd_bell` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x413c
  **Inferred.** `pckbd_bell` is a 140-byte routine. Role (from name): sounds the keyboard bell. A third
  argument (a2) of zero or less branches to 0x4184 and returns 0. When a2 is positive it calls `splhi` at
  0x4150 and tests the gp-relative word at gp-0x7fc0, the word `stop_bell` clears. If that word is nonzero it
  calls `splx` at 0x416c and returns 1. If it is zero it stores 0x100000 through the pointer held at
  `.sdata`+72 (0x41b0), calls `timeout` at 0x41b4 with `stop_bell` as the first argument, 0 as the second and
  0x100000 as the fourth (a3), and stores the call's result to the gp-0x7fc0 word at 0x41c4. The facts do not
  show what a2 means (duration, pitch or a count), so that stays open.
  Reviewed: 2026-11-12 (sonnet)

- `counter_intr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x41f0
  **Inferred.** `counter_intr` is a 1724-byte routine. Role (from name): handler for the timer compare
  interrupt. The branch at 0x4204 goes to the `hwperf_overflowed_intr` call at 0x4244 when the word at abs
  -0x39a8 is zero, and `hwperf_intr` at 0x4254 is called only when that result is nonzero.
  `frs_handle_counter_intr` at 0x421c sits on the path where bit 0x20 of the doubleword at abs -0x3d80 is
  clear. It calls `_set_timer_intr` at 0x42cc with the CPU number (the halfword at abs -0x3c60) and the result
  of `first_timein`, reads the count and compare registers through `_get_count` and `_get_compare` at 0x46e4
  and 0x46ec, and makes four `log_tstamp_event` calls at 0x47a8, 0x47c8, 0x47e4 and 0x4800. It calls `cmn_err`
  at 0x488c; the conditions that reach it are partly open and the facts do not say what the message is.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x5cbc
  **Inferred.** `doadjtime` is a 500-byte routine. Role (from name): applies a pending time-of-day adjustment.
  Its first argument is tested three ways: zero goes to 0x5e24, a negative value branches to 0x5e14, and a
  positive value falls through. The zero path calls `splprof` at 0x5e24, `__nanotime` at 0x5e2c and
  `nested_spinlock` on `timelock` at 0x5e38, then `mutex_spinunlock` at 0x5e98. The code from 0x5d04 makes the
  same three calls (at 0x5d04, 0x5d0c and 0x5d1c) and releases the lock at 0x5dc4. Where the negative and
  positive paths reach it is not traced here. The routine loads and stores the `adjtime_delta`, `adjtime_RTC`
  and `adjtime_nanos` words. A value the facts do not resolve is compared with 0x3e8 at 0x5cf4 and with
  0xf4240 at 0x5df8; these look like milliseconds and microseconds, but the stepping arithmetic is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x5eb0
  **Inferred.** `__nanotime` is a 548-byte routine. Role (from name): returns the current time as seconds and
  nanoseconds. The facts show its return value as the result of `mutex_spinunlock`, so what it hands back is
  not shown. It takes `timelock` through `mutex_spinlock_spl` at 0x5ebc and releases it at 0x604c and 0x60c4.
  It reads the 64-bit counter at 0xffffffffaff20000 and compares it with the sum of the `adjtime_delta` and
  `adjtime_RTC` words. Compares of a nanosecond-sized value against 0x3b9aca00 (1e9) at 0x5fcc, 0x6004 and
  0x6014 may carry whole seconds, but the arithmetic is not shown. It loads and stores two fields at
  `pdaindr`+0x20 and +0x28 and stores a copy of the first to the `time` word at 0x6038; what those fields mean
  is inferred from offsets only.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x65e8
  **Inferred.** `findcpufreq_cycles` is a 52-byte routine. Role (from name): caches the CPU clock rate in
  cycles. It reads the word at abs -0x390c (0xffffffffffffc6f4) and returns it when it is nonzero. When the
  word is zero it calls `findcpufreq_hz` at 0x6604 and stores a word back to the same address at 0x6618; the
  facts do not tie the stored value to the call's result. `startkgclock` reads the same word, which fits a
  cached value, but the callee's units are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x811c
  **Inferred.** `startkgclock` is a 408-byte routine. Role (from name): starts the periodic kernel clock on
  the current CPU. It reads the `fasthz` word and calls `fasthzerr` at 0x817c, 0x8204 and 0x8278 with 1000,
  500 and 2500 as the third argument (a2). The 500 and 2500 calls sit behind compares of `fasthz` against
  bounds. It also calls `splprof` at 0x81bc, `set_timer_intr` at 0x81e0 with the CPU number (the halfword at
  abs -0x3c60) and -1 as its first two arguments, and `splx` at 0x81e8, and stores 1 to the doubleword at abs
  -0x3ab0 at 0x81d4 (`slowkgclock` stores 0 there). What `fasthzerr` does with those values is not visible in
  this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x82b4
  **Inferred.** `slowkgclock` is a 40-byte wrapper. Role (from name): switches to the slow kernel clock. It
  calls `splprof` at 0x82bc and `splx` on its result at 0x82c8, and stores the doubleword 0 to abs
  0xffffffffffffc550 at 0x82cc, in the delay slot of the `splx` call. `startkgclock` stores 1 to the same
  address, which suggests a shared clock-mode word, but nothing else here confirms that.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8680
  **Inferred.** `cpu_waitforreset` is a 48-byte spin loop. Role (from name): waits for a reset. It reads the
  word at 0x900000001f000104 and keeps polling while bit 0x40 is clear. Only when the bit is set
  does it reach the call to `cpu_soft_powerdown` at 0x86a0; the branch at 0x86a8 that follows is always taken, back
  to the poll. The facts show no return,
  so the routine has no exit. What the bit reports is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `_clock_func` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x88e4
  **Inferred.** `_clock_func` is a 524-byte routine for the clock device. It takes `ip30clocklock` at
  0x8904, saves the serial state with `slow_ioc3_sio` at 0x890c, and moves a 14-byte record
  between the caller's buffer at a1 and two byte-wide register windows at 0x900000001f6c0000
  and 0x900000001f6a0000. It restores the serial state through `restore_ioc3_sio` at 0x89f0
  and 0x8a50, and a zero first argument takes a separate path at 0x8a20. The meaning of each
  register byte is not established, and the serial-state calls suggest a shared device only
  from the callee names.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_clearpower` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8e70
  **Inferred.** `cpu_clearpower` is a 112-byte routine. Role (from name): clears a power state. It takes
  `ip30clocklock` at 0x8e7c and writes the same two register windows as `_clock_func`, with four byte stores
  in address order: 0xa to 0x900000001f6a0000, 0x30 to 0x900000001f6c0000, 0x4a to 0x900000001f6a0000, and 0
  to 0x900000001f6c0000 (the last in the delay slot of the `flushbus` call at 0x8eb4, whose arguments are 0x30
  and 0xa). It brackets the work with `slow_ioc3_sio` at 0x8e88 and `restore_ioc3_sio` at 0x8ebc and releases
  the lock at 0x8ecc. What the written values mean is not established.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_soft_powerdown` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8ee0
  **Inferred.** `cpu_soft_powerdown` is a 200-byte routine. Role (from name): a software power-off path. When
  the `fp_poweroff` word is nonzero it calls the hook it holds through `jalr` at 0x8ef0. It then calls
  `conbuf_flush` at 0x8ef8 with 3, calls `heart_imr_bits_rmw` at 0x8f0c with the `master_procid` halfword as
  a0, 0x200000000000000 as a1 and 0 as a2, takes `ip30clocklock` through `mutex_spinlock_spl` at 0x8f30 and
  calls `slow_ioc3_sio` at 0x8f38. It writes bytes to the 0x900000001f6a0000 and 0x900000001f6c0000 windows
  and ends in a loop that calls `us_delay` at 0x8f98 with 10,000,000. The facts show no return. Whether the
  writes remove power is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `slow_ioc3_sio` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8fa8
  **Inferred.** `slow_ioc3_sio` is a 32-byte helper. It reads the word at 0x900000001f600028 and returns that
  word. It writes a word to the same address at 0x8fc4, in the delay slot of the return jump, and the stored
  value is not resolved.
  Reviewed: 2026-11-12 (sonnet)

- `restore_ioc3_sio` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x8fc8
  **Inferred.** `restore_ioc3_sio` is a 32-byte helper. It stores the byte 0xe to 0x900000001f6a0000, loads a
  byte from 0x900000001f6c0000 and returns it, and writes its first argument (a0) as a word to
  0x900000001f600028 at 0x8fe4, in the delay slot of the return. `_clock_func` and `cpu_clearpower` pass it
  the result of `slow_ioc3_sio` as a0, so it writes back the word that helper read. What the bytes mean is
  open.
  Reviewed: 2026-11-12 (sonnet)

- `nvchecksum` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x9020
  **Inferred.** `nvchecksum` is a 164-byte routine with two loops. It stores five bytes to 0x900000001f6a0000
  and three to 0x900000001f6c0000 and loads a byte from 0x900000001f6c0000 at two sites (0x9058 and 0x9094).
  The loop limits are 0x80 and 0x100 (128 and 256), which suggests a fixed-size block. The two windows look
  like an index and a data port, but that comes from the access pattern only. The checksum arithmetic and its
  output are not shown, so the name is only a guess.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc080
  **Inferred.** `badaddr` is a 296-byte probe that reads 1, 2, 4 or 8 bytes at a0, with the width in a1 chosen
  by a compare chain. It stores a halfword at offset 0xf6 of the structure at [-0x3fe0], 1 before the access
  and 0 after it, which looks like a no-fault marker. Any other width is reported through `cmn_err` at 0xc144.
  After the access it tests the hub error word at 0xffffffffaff10040 against 0x10000 and tests a value the
  facts do not resolve against 0x4000; a nonzero result of either test branches to `baerror` at 0xc788.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc1a8
  **Inferred.** `badaddr_val` is a 364-byte probe like `badaddr`, but it copies the value it reads. It
  loads 1, 2, 4 or 8 bytes from a0 and stores the same width to the destination, and it uses
  the same no-fault marker and hub error bit checks. Whether a bus error leaves the copied
  destination unchanged is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc314
  **Inferred.** `wbadaddr` is a 332-byte write probe. It stores 1, 2, 4 or 8 bytes to a0 by width, then runs
  the same hub error checks and no-fault marker as `badaddr`. On one path it also reads a word
  from 0x9000000000000000, and the disassembly does not show why.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc5c8
  **Inferred.** `earlybadaddr` is a 312-byte probe for early boot. It handles only 1, 2 and 4-byte widths,
  and any other width goes to a `cmn_err` report at 0xc688. It uses the global word `nofault`
  as its marker, not the -0x3fe0 structure.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc788
  **Inferred.** `baerror` is a 92-byte routine. Role (from name): records a bus error. It loads the doubleword
  at 0xffffffffb8000060, stores 0 to 0xffffffffb8000040, loads from 0xffffffffb8000000 (the result is
  discarded) and writes the first value back to 0xffffffffb8000060. It loads the doubleword at
  0xffffffffaff10040 and writes it back, and loads from 0xffffffffaff50000. When the doubleword at -0x3fe0 is
  nonzero it stores the halfword 0 at +0xf6 of the structure it points to, and it stores the word 0 to abs
  0xffffffffffffc404. It returns 1 on both paths. What the 0xb8 block is for is not established.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xc7e4
  **Inferred.** `earlybaerror` is a 96-byte routine. Role (from name): the early-boot version of `baerror`. It
  makes the same accesses as `baerror` to the 0xffffffffb8000000 block, 0xffffffffaff10040 and
  0xffffffffaff50000, then stores 0 to the `nofault` word and returns 1. The disassembly does not show more
  than that.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0xdbb0
  **Inferred.** `_ecc_errorvec` is a 104-byte stub. Role (from name): an ECC error vector. It loads a value
  from 0xffffffffaff50000, shifts it left by 3 and loads a doubleword from that offset plus 0xff0, then stores
  two doublewords through the pointer it loaded, at +0xc8 and +0; the stored values are not resolved. It then
  jumps indirectly through a register with the original arguments, and the jump target is not resolved from
  this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x13df4
  **Inferred.** `dobuserr_common` is a 9460-byte routine. Role (from name): bus-error dispatcher for the Heart
  error path. It updates `heart_error_cpumap` with load-linked and store-conditional at 0x13e80 and 0x13e88
  and calls `compare_and_swap_int` on it at 0x13ebc. It stores per-CPU entries into the `_heart_mode` table,
  indexed by the CPU number at abs -0x3c60. It calls `heart_error_refine` at five sites, with a1 of 4, 5, 6, 9
  or 10 where the facts resolve it, and calls `hem_add_ioe` at five sites and `hem_add_sync` at seven. It
  makes 127 `cmn_err` calls and one `cmn_err_tag` call, nearly all with one format string, and what each error
  class means is open.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x167e0
  **Inferred.** `VEC_dbe` is an 88-byte routine. Role (from name): the bus-error exception vector. It loads a
  value from 0xffffffffaff50000, loads a doubleword from 0xffffffffaff10000 plus that value shifted left by 3
  at 0x16800 and stores a doubleword back to the same address at 0x16814; the stored value is not resolved. It
  stores the halfword at abs -0x3c60 as a doubleword at a0+0x140, which may be in the saved exception frame,
  and then jumps to `VEC_trap` at 0x16830 with the original arguments.
  Reviewed: 2026-11-12 (sonnet)
