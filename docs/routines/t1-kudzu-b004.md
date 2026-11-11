# Routine role statements: IP30 kernel clock, power, and bus-error paths

These statements cover 25 routines in the IP30 kernel image `6.5.22/unix.IP30`. Each one comes from its instruction facts (registers, calls, constants, fixed addresses) and names the callees and addresses it relies on. The facts describe what the code does, not what the author meant, so every label here is inferred. Where the facts stop short of the purpose, the statement says so.

No target in this batch was rejected.

- `debug_stop_all_cpus` in `6.5.22/unix.IP30` at 0xa8000000200077c8
  **Inferred.** `debug_stop_all_cpus` (140 bytes) loops over a count word at 0xa8000000203da594 and skips one index that matches a 2-byte global at [-0x3c60]. For each other index it stores a doubleword to the fixed address 0xffffffffaff10020, which suggests a signal that stops the other CPUs. The facts do not show the stored value, and the return value at 0xa800000020007840 is unknown.

- `stop_bell` in `6.5.22/unix.IP30` at 0xa800000020008008
  **Inferred.** `stop_bell` (52 bytes) raises the interrupt level with spl1 at 0xa800000020008010, restores it with splx at 0xa800000020008028, and returns splx's value. It makes no other memory access or call. `pckbd_bell` passes this address to timeout, so it is probably the deferred callback, but the facts do not show what it does when it runs.

- `pckbd_bell` in `6.5.22/unix.IP30` at 0xa80000002000803c
  **Inferred.** `pckbd_bell` (140 bytes) brackets its work with spl1 at 0xa800000020008050 and splx at 0xa80000002000806c. It schedules `stop_bell` (0xa800000020008008) through timeout at 0xa8000000200080b4, with 0x100000 as the a3 argument, and its return value is 0 or 1. The name suggests a keyboard bell, but the routine touches no device register in its facts.

- `counter_intr` in `6.5.22/unix.IP30` at 0xa8000000200080f0
  **Inferred.** `counter_intr` (1680 bytes) is the counter interrupt handler. It reads and reprograms the timer through _get_count (0xa80000002001a490), _get_compare (0xa80000002001a49c), _set_timer_intr (0xa800000020009004) and resetcounter (0xa800000020005700), and it hands performance-counter overflow to hwperf_overflowed_intr (0xa80000002001cbb8), hwperf_intr (0xa80000002001d804) and frs_handle_counter_intr (0xa80000002033a600). It logs timestamp events through log_tstamp_event (0xa8000000201d64e0) with codes 0x6c and 0x65, and it reports through cmn_err (0xa8000000201a63b4). Several of its loads come from 0xffffffffaff20000, the same address `__nanotime` and `_set_timer_intr` use, so it probably reads timer hardware there, but the register meanings are not in the facts.

- `doadjtime` in `6.5.22/unix.IP30` at 0xa800000020009aac
  **Inferred.** `doadjtime` (500 bytes) runs under spl65 (0xa80000002000f514), passing 0xf0000 as a1, and holds the lock at 0xa8000000203e1454 through nested_spinlock (0xa800000020210fe0) and mutex_spinunlock (0xa800000020210fc0). It samples time twice through `__nanotime` (0xa800000020009ca0). Its branches compare a counter against 0x3e8 and 0xf4240 (1000 and 1,000,000), which suggests a microsecond adjustment loop, but the facts do not show how the adjustment is applied, and I have not confirmed the units.

- `__nanotime` in `6.5.22/unix.IP30` at 0xa800000020009ca0
  **Inferred.** `__nanotime` (548 bytes) takes the lock at 0xa8000000203e1454 through mutex_spinlock_spl (0xa800000020210f20) at spl65, reads the 64-bit word at 0xffffffffaff20000, and reads and writes the cached pair at 0xa8000000203e33a8 and 0xa8000000203e33a0. Its branches compare against 0x3b9aca00 (1,000,000,000), which fits a normalization of ticks into seconds and nanoseconds. The facts do not show the full arithmetic, so the exact conversion is not established.

- `findcpufreq_cycles` in `6.5.22/unix.IP30` at 0xa80000002000a3d8
  **Inferred.** `findcpufreq_cycles` (52 bytes) loads a word from 0xffffffffffffc6f4, calls findcpufreq_hz (0xa80000002000bda4) only when the global at [-0x390c] is nonzero, and stores a word back to 0xffffffffffffc6f4. The name suggests a frequency-to-cycles conversion, but the facts show only one call and no arithmetic, so that reading comes from the name.

- `startkgclock` in `6.5.22/unix.IP30` at 0xa80000002000bf0c
  **Inferred.** `startkgclock` (408 bytes) reads the word at 0xffffffffffffc6f4 that `findcpufreq_cycles` writes, and it writes the word at 0xffffffffffffc56c and the doubleword at 0xffffffffffffc550. Under spl65, called with 0xf4240 as a4, it calls set_timer_intr (0xa800000020008fe0) with the 2-byte global at [-0x3c60] as the first argument. Three error paths call fasthzerr (0xa80000002000bec0) with the constants 0x3e8, 0x1f4 and 0x9c4 (1000, 500 and 2500), which look like limits on a rate, but the facts do not say.

- `slowkgclock` in `6.5.22/unix.IP30` at 0xa80000002000c0a4
  **Inferred.** `slowkgclock` (40 bytes) stores a doubleword to 0xffffffffffffc550, the same word `startkgclock` writes, inside a spl65 and splx bracket (0xa80000002000f514 and 0xa80000002000f564). It has no branches or other memory accesses, and the stored value is not in the facts. The shared word suggests it is the stop side of the clock that `startkgclock` starts, but the facts do not establish that.

- `cpu_waitforreset` in `6.5.22/unix.IP30` at 0xa80000002000c470
  **Inferred.** `cpu_waitforreset` (48 bytes) spins on a word at the fixed uncached address 0x900000001f000104 until bit 0x40 is set, and then calls cpu_soft_powerdown (0xa80000002000ccd0). The loop is real, but the facts do not show what sets that bit or what the bit means. The name suggests a secondary CPU waiting for reset.

- `_clock_func` in `6.5.22/unix.IP30` at 0xa80000002000c6d4
  **Inferred.** `_clock_func` (524 bytes) takes the lock at 0xa8000000203e14c8 through mutex_spinlock_spl (0xa800000020210f20) and writes bytes to 0x900000001f6a0000 and 0x900000001f6c0000, reading back from the second. It wraps those accesses in slow_ioc3_sio (0xa80000002000cd98) and restore_ioc3_sio (0xa80000002000cdb8), and it polls bit 0x40 of the byte at 0x900000001f6c0000 before moving on. The two byte addresses sit 0x20000 apart, which suggests two serial ports behind an IOC3 chip, but the facts name no device.

- `cpu_clearpower` in `6.5.22/unix.IP30` at 0xa80000002000cc60
  **Inferred.** `cpu_clearpower` (112 bytes) runs under the same lock at 0xa8000000203e14c8 and the same slow_ioc3_sio and restore_ioc3_sio window as `_clock_func`. It stores bytes to 0x900000001f6a0000 and 0x900000001f6c0000, then calls flushbus (0xa80000002000fbc4) with 0x30 and 0xa before the restore. The stored values are not in the facts, so what gets cleared is not established.

- `cpu_soft_powerdown` in `6.5.22/unix.IP30` at 0xa80000002000ccd0
  **Inferred.** `cpu_soft_powerdown` (200 bytes) calls conbuf_flush (0xa8000000201a83a8) with 3, updates an interrupt mask with heart_imr_bits_rmw (0xa80000002003459c) using the 64-bit mask 0x200000000000000, and delays through us_delay (0xa8000000200053c0) with 0x989680 (10,000,000). It makes an indirect call through the pointer at 0xa8000000203e14d0, but only when that pointer is nonzero, and the target is not in the facts. The sequence reads as flush, mask, delay, then power-off, but the power-off step itself does not appear in the facts.

- `slow_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cd98
  **Inferred.** `slow_ioc3_sio` (32 bytes) reads a word at 0x900000001f600028, writes a word back to that address, and returns the value it read. It has no branches and no calls. The name suggests it slows IOC3 serial access, but the facts show neither the value written nor the timing effect.

- `restore_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cdb8
  **Inferred.** `restore_ioc3_sio` (32 bytes) writes a byte to 0x900000001f6a0000, reads a byte from 0x900000001f6c0000 and returns it, and writes a word to 0x900000001f600028, the address `slow_ioc3_sio` reads. The callers bracket their byte accesses with the pair, so it is probably the restore half, but the value it writes is not in the facts.

- `nvchecksum` in `6.5.22/unix.IP30` at 0xa80000002000ce10
  **Inferred.** `nvchecksum` (164 bytes) loops twice, with bounds of 0x80 and 0x100, and in the loop it writes bytes to 0x900000001f6a0000 and 0x900000001f6c0000 and reads from the second. It makes no calls and returns an unknown value. The name suggests an NVRAM checksum over 128 or 256 units, but byte writes to the same two addresses that `cpu_clearpower` uses make that a poor fit, so the name may mislead.

- `badaddr` in `6.5.22/unix.IP30` at 0xa80000002000fc00
  **Inferred.** `badaddr` (296 bytes) dispatches on a1 being 1, 2, 4 or 8, which looks like an access width for a probe. It saves and restores 8-byte words at 0xffffffffb8000040 and 0xffffffffb8000060, reads at 0xffffffffb8000000, 0xffffffffaff10040, 0xffffffffaff50000 and 0xffffffffffffc020, and reports through cmn_err (0xa8000000201a63b4) with the message at 0xa80000002037d788. It returns 0. The probing load itself is not visible in the facts, so the fault-detection path is open.

- `badaddr_val` in `6.5.22/unix.IP30` at 0xa80000002000fd28
  **Inferred.** `badaddr_val` (364 bytes) has the same width dispatch and save and restore pattern as `badaddr`. It stores 4 bytes to 0xffffffffffffc404 at two points, reports through cmn_err with the message at 0xa80000002037d791, and returns 0. The name suggests a check against an expected value, but the facts do not show a comparison of that kind, so the role here is inferred from the name and the shared pattern.

- `wbadaddr` in `6.5.22/unix.IP30` at 0xa80000002000fe94
  **Inferred.** `wbadaddr` (332 bytes) follows the same width dispatch and save and restore pattern as `badaddr`. It adds a 4-byte load from 0x9000000000000000 and reports through cmn_err with the message at 0xa80000002037d79e, then returns 0. The name suggests a write probe, but no store through the argument register appears in the facts.

- `earlybadaddr` in `6.5.22/unix.IP30` at 0xa800000020010148
  **Inferred.** `earlybadaddr` (312 bytes) is the early-boot variant of `badaddr`: the same 8-byte save and restore at the fixed addresses, and 4-byte stores to 0xa8000000203e2104. The facts label that word "nofault", which suggests the fault path uses it, but that label is only an annotation and I have not checked it. The routine reports through cmn_err with the message at 0xa80000002037d7b6 and returns 0.

- `baerror` in `6.5.22/unix.IP30` at 0xa800000020010308
  **Inferred.** `baerror` (92 bytes) saves and restores 8-byte words at 0xffffffffb8000040, 0xffffffffb8000060, 0xffffffffb8000000 and 0xffffffffaff10040, reads 0xffffffffaff50000 and 0xffffffffffffc020, and writes a word to 0xffffffffffffc404. Its one branch tests a global at [-0x3fe0] and skips that word write when it is zero. It makes no calls and returns 1, which could mean the error was handled, but the facts do not show what the 1 means.

- `earlybaerror` in `6.5.22/unix.IP30` at 0xa800000020010364
  **Inferred.** `earlybaerror` (96 bytes) is the early variant of `baerror`: the same 8-byte accesses to the fixed addresses, and a 4-byte store to 0xa8000000203e2104, the word the facts label "nofault". It has no branches or calls and returns 1.

- `_ecc_errorvec` in `6.5.22/unix.IP30` at 0xa800000020011730
  **Inferred.** `_ecc_errorvec` (104 bytes) has one memory access, an 8-byte load from 0xffffffffaff50000, which `badaddr`, `baerror` and `VEC_dbe` also read. It has no branches, no calls, and no stores in the facts. The name suggests an ECC error vector, but the body is too small to show what it does with the value it loads, so its role is open.

- `dobuserr_common` in `6.5.22/unix.IP30` at 0xa800000020017974
  **Inferred.** `dobuserr_common` (9460 bytes) reads Heart error state from fixed addresses such as 0xffffffffaff00000, 0xffffffffaff10040 and 0x9000000018000060. It updates the word `heart_error_cpumap` at 0xa8000000203da660 with ll and sc, and it calls heart_intr (0xa800000020035bcc), heart_paddr_to_port (0xa800000020035988), heart_error_refine (0xa800000020035aa4) and hem_add_ioe (0xa800000020017734) on several paths. Most of its 155 calls are cmn_err reports, and the facts do not show which error class takes which path, so the decode order is open.

- `VEC_dbe` in `6.5.22/unix.IP30` at 0xa80000002001a360
  **Inferred.** `VEC_dbe` (88 bytes) looks like a data-bus-error exception vector. It loads from 0xffffffffaff50000 and 0xffffffffb8000000, makes an indexed load and store through the value read from 0xffffffffaff50000, writes to the frame at arg0 +0x140, and then jumps directly to VEC_trap (0xa80000002000f3d0) with the same four arguments. The facts do not show how the indexed store is used, and the frame offset is inferred from the store alone.
