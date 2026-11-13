# Routine role statements: IP30 kernel clock, power, and bus-error paths

These statements cover 25 routines in the IP30 kernel image `6.5.22/unix.IP30`. Each one comes from its instruction facts (registers, calls, constants, fixed addresses) and names the callees and addresses it relies on. The facts describe what the code does, not what the author meant, so every label here is inferred. Where the facts stop short of the purpose, the statement says so.

No target in this batch was rejected.

- `debug_stop_all_cpus` in `6.5.22/unix.IP30` at 0xa8000000200077c8
  **Inferred.** `debug_stop_all_cpus` (140 bytes) loops over a count word at 0xa8000000203da594 and skips one index that matches a 2-byte global at [-0x3c60]. For each other index it stores a doubleword to the fixed address 0xffffffffaff10020, which suggests a signal that stops the other CPUs. The facts do not show the stored value, and the return value at 0xa800000020007840 is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `stop_bell` in `6.5.22/unix.IP30` at 0xa800000020008008
  **Inferred.** `stop_bell` (52 bytes) raises the interrupt level with spl1 at 0xa800000020008010, stores zero to the word at 0xa8000000203e1430, restores the level with splx at 0xa800000020008028, and returns splx's value. After the splx call it stores 0x100000 through a pointer loaded from 0xa8000000203da5c0. `pckbd_bell` passes this address to timeout and reads and writes the word at 0xa8000000203e1430, so it is probably the deferred callback and the zero store may clear a flag that `pckbd_bell` tests, but the facts do not show what the pointer targets or what the callback does when it runs.
  Reviewed: 2026-11-12 (sonnet)

- `pckbd_bell` in `6.5.22/unix.IP30` at 0xa80000002000803c
  **Inferred.** `pckbd_bell` (140 bytes) returns 0 without further work when a2 is not positive. Otherwise it brackets its work with spl1 at 0xa800000020008050 and splx at 0xa80000002000806c, and returns 1. Only when the word at 0xa8000000203e1430 is zero does it schedule `stop_bell` (0xa800000020008008) through timeout at 0xa8000000200080b4, with 0x100000 as the a3 argument, and it then stores timeout's return value to that word. The name suggests a keyboard bell, but the routine touches no device register in its facts.
  Reviewed: 2026-11-12 (sonnet)

- `counter_intr` in `6.5.22/unix.IP30` at 0xa8000000200080f0
  **Inferred.** `counter_intr` (1680 bytes) calls _get_count (0xa80000002001a490), _get_compare (0xa80000002001a49c), _set_timer_intr (0xa800000020009004) and resetcounter (0xa800000020005700). It also calls hwperf_overflowed_intr (0xa80000002001cbb8), then hwperf_intr (0xa80000002001d804) when that returns nonzero, and frs_handle_counter_intr (0xa80000002033a600) when a loaded word is nonzero and bit 0x20 of a loaded doubleword is clear. It calls log_tstamp_event (0xa8000000201d64e0) with codes 0x6c and 0x65, and cmn_err (0xa8000000201a63b4). Several of its loads come from 0xffffffffaff20000, which `__nanotime` also loads, and it passes one such load to _set_timer_intr as a4, so it probably reads a timer there, but the register meaning is not in the facts. Role (from name): counter interrupt handler that reads and reprograms the timer.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.22/unix.IP30` at 0xa800000020009aac
  **Inferred.** `doadjtime` (500 bytes) calls spl65 (0xa80000002000f514) with 0xf0000 as a1, samples time through `__nanotime` (0xa800000020009ca0), and holds the lock at 0xa8000000203e1454 through nested_spinlock (0xa800000020210fe0) and mutex_spinunlock (0xa800000020210fc0). That sequence appears at two call sites: one on the path taken when a0 is zero, and one on the nonzero-a0 path, where a branch can loop back to the spl65 call. Its branches compare values against 0x3e8 and 0xf4240 (1000 and 1,000,000), which suggests a microsecond adjustment loop, but the facts do not show how the adjustment is applied, and I have not confirmed the units.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.22/unix.IP30` at 0xa800000020009ca0
  **Inferred.** `__nanotime` (548 bytes) takes the lock at 0xa8000000203e1454 through mutex_spinlock_spl (0xa800000020210f20) at spl65, reads the 64-bit word at 0xffffffffaff20000, and reads and writes the cached pair at 0xa8000000203e33a8 and 0xa8000000203e33a0. Its branches compare against 0x3b9aca00 (1,000,000,000), which fits a normalization of ticks into seconds and nanoseconds. The facts do not show the full arithmetic, so the exact conversion is not established.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.22/unix.IP30` at 0xa80000002000a3d8
  **Inferred.** `findcpufreq_cycles` (52 bytes) loads the word at 0xffffffffffffc6f4. When that word is nonzero it returns it without a store. When it is zero it calls findcpufreq_hz (0xa80000002000bda4), passing the word as a2, and then stores a word to 0xffffffffffffc6f4. The name suggests a frequency-to-cycles conversion, but the facts show only one call and no arithmetic, so that reading comes from the name.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.22/unix.IP30` at 0xa80000002000bf0c
  **Inferred.** `startkgclock` (408 bytes) reads the word at 0xffffffffffffc6f4 that `findcpufreq_cycles` writes, and it writes the word at 0xffffffffffffc56c and the doubleword at 0xffffffffffffc550. Under spl65, called with 0xf4240 as a4, it calls set_timer_intr (0xa800000020008fe0) with the 2-byte global at [-0x3c60] as the first argument. Three error paths call fasthzerr (0xa80000002000bec0) with the constants 0x3e8, 0x1f4 and 0x9c4 (1000, 500 and 2500), which look like limits on a rate, but the facts do not say.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.22/unix.IP30` at 0xa80000002000c0a4
  **Inferred.** `slowkgclock` (40 bytes) stores a zero doubleword to 0xffffffffffffc550 in the delay slot of its splx call, inside a spl65 and splx bracket (0xa80000002000f514 and 0xa80000002000f564). It has no branches and no memory accesses other than stack saves. `startkgclock` stores 1 to the same word, which suggests this is the stop side of the clock that `startkgclock` starts, but the facts do not establish that.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.22/unix.IP30` at 0xa80000002000c470
  **Inferred.** `cpu_waitforreset` (48 bytes) spins on a word at the fixed uncached address 0x900000001f000104 until bit 0x40 is set, and then calls cpu_soft_powerdown (0xa80000002000ccd0). The loop is real, but the facts do not show what sets that bit or what the bit means. The name suggests a secondary CPU waiting for reset.
  Reviewed: 2026-11-12 (sonnet)

- `_clock_func` in `6.5.22/unix.IP30` at 0xa80000002000c6d4
  **Inferred.** `_clock_func` (524 bytes) takes the lock at 0xa8000000203e14c8 through mutex_spinlock_spl (0xa800000020210f20) and writes bytes to 0x900000001f6a0000 and 0x900000001f6c0000, reading back from the second. It wraps those accesses in slow_ioc3_sio (0xa80000002000cd98) and restore_ioc3_sio (0xa80000002000cdb8). On the path taken when a0 is zero it tests bit 0x40 of the byte read from 0x900000001f6c0000 once, with no branch back to the test: when the bit is set it calls restore_ioc3_sio and mutex_spinunlock (0xa800000020210fc0), and when it is clear it goes on to further reads from that address. Writing a changing value to one byte address and transferring data at the other looks like an index and data register pair, and the name suggests a clock chip, but the facts name no device.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_clearpower` in `6.5.22/unix.IP30` at 0xa80000002000cc60
  **Inferred.** `cpu_clearpower` (112 bytes) runs under the same lock at 0xa8000000203e14c8 and the same slow_ioc3_sio and restore_ioc3_sio window as `_clock_func`. It stores byte 0xa to 0x900000001f6a0000, 0x30 to 0x900000001f6c0000 and 0x4a to 0x900000001f6a0000, calls flushbus (0xa80000002000fbc4) with 0x30 and 0xa, stores zero to 0x900000001f6c0000 in the call's delay slot, and then calls restore_ioc3_sio. The facts do not show what the register writes mean, so what gets cleared is not established.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_soft_powerdown` in `6.5.22/unix.IP30` at 0xa80000002000ccd0
  **Inferred.** `cpu_soft_powerdown` (200 bytes) makes an indirect call through the pointer at 0xa8000000203e14d0, but only when that pointer is nonzero, and the target is not in the facts. It then calls conbuf_flush (0xa8000000201a83a8) with 3, updates an interrupt mask with heart_imr_bits_rmw (0xa80000002003459c) using the 64-bit mask 0x200000000000000, takes the lock at 0xa8000000203e14c8 through mutex_spinlock_spl, calls slow_ioc3_sio (0xa80000002000cd98), writes bytes to 0x900000001f6a0000 and 0x900000001f6c0000 and reads one back, and delays through us_delay (0xa8000000200053c0) with 0x989680 (10,000,000). The facts list no restore_ioc3_sio or unlock call, and what the byte accesses do is not established. The name suggests a power-off sequence, but no step in the facts is shown to cut power.
  Reviewed: 2026-11-12 (sonnet)

- `slow_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cd98
  **Inferred.** `slow_ioc3_sio` (32 bytes) reads a word at 0x900000001f600028, writes a word to the same address, and returns the value it read. It has no branches and no calls. The name suggests it slows IOC3 serial access, but the facts show neither the value written nor the timing effect.
  Reviewed: 2026-11-12 (sonnet)

- `restore_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cdb8
  **Inferred.** `restore_ioc3_sio` (32 bytes) writes the byte 0xe to 0x900000001f6a0000, reads a byte from 0x900000001f6c0000 and returns it, and writes a0 as a word to 0x900000001f600028, the address `slow_ioc3_sio` reads. Callers pass slow_ioc3_sio's return value as a0, so it is probably the restore half of a bracket, but the facts do not show what the byte 0xe selects.
  Reviewed: 2026-11-12 (sonnet)

- `nvchecksum` in `6.5.22/unix.IP30` at 0xa80000002000ce10
  **Inferred.** `nvchecksum` (164 bytes) runs two loops, with bounds of 0x80 and 0x100. Each loop writes bytes to 0x900000001f6a0000 and 0x900000001f6c0000 and reads a byte from the second. It makes no calls and returns an unknown value. The name suggests an NVRAM checksum over 128 or 256 units. Writing to one address and reading from the other could be an index and data pair, which would fit that reading, but the facts do not name the device or show the arithmetic.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/unix.IP30` at 0xa80000002000fc00
  **Inferred.** `badaddr` (296 bytes) dispatches on a1 being 1, 2, 4 or 8, which looks like an access width for a probe, and loads from a0 with a matching lb, lh, lw or ld. It saves and restores the 8-byte words at 0xffffffffb8000060 and 0xffffffffaff10040, stores zero to 0xffffffffb8000040, and reads 0xffffffffb8000000, 0xffffffffaff50000 and the pointer at 0xffffffffffffc020, which it also uses as a base for halfword stores at +0xf6 near entry and again before the normal return. It calls cmn_err (0xa8000000201a63b4) with the message at 0xa80000002037d788 only when a1 is none of 1, 2, 4 or 8. It returns 0 unless one of two tests (bit 0x4000 of a register the facts leave unknown, then bit 0x10000 of the word at 0xffffffffaff10040) branches to baerror (0xa800000020010308).
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/unix.IP30` at 0xa80000002000fd28
  **Inferred.** `badaddr_val` (364 bytes) has the same width dispatch and save and restore pattern as `badaddr`, with the same cmn_err path for an unsupported width (message at 0xa80000002037d791) and the same two tests that branch to baerror (0xa800000020010308), and otherwise returns 0. For widths 1, 2, 4 and 8 it loads from a0 and stores the loaded value through a2 at the same width. Only when the pointer at 0xffffffffffffc020 is zero does it store a 4-byte word to 0xffffffffffffc404, the value 1 near entry and zero before the return. The name suggests a probe that also returns the value read, which fits the store through a2.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/unix.IP30` at 0xa80000002000fe94
  **Inferred.** `wbadaddr` (332 bytes) follows the same width dispatch, save and restore pattern and unsupported-width cmn_err path (message at 0xa80000002037d79e) as `badaddr`. For widths 1, 2, 4 and 8 it stores zero through a0 at that width, which is consistent with a write probe. It also loads a 4-byte word from 0x9000000000000000. It returns 0 unless one of the same two tests branches to baerror (0xa800000020010308).
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/unix.IP30` at 0xa800000020010148
  **Inferred.** `earlybadaddr` (312 bytes) is the early-boot variant of `badaddr`: the same save and restore of the 8-byte words at 0xffffffffb8000060 and 0xffffffffaff10040, but with loads through a0 at widths 1, 2 and 4 only, and 4-byte stores to 0xa8000000203e2104 (the value 9, then zero before the normal return). The facts label that word "nofault", which suggests the fault path uses it, but that label is only an annotation and I have not checked it. It calls cmn_err (0xa8000000201a63b4) with the message at 0xa80000002037d7b6 only for a width other than 1, 2 or 4, and returns 0 unless one of the two tests branches to earlybaerror (0xa800000020010364).
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/unix.IP30` at 0xa800000020010308
  **Inferred.** `baerror` (92 bytes) saves and restores the 8-byte words at 0xffffffffb8000060 and 0xffffffffaff10040, stores zero to 0xffffffffb8000040, and reads 0xffffffffb8000000, 0xffffffffaff50000 and the pointer at 0xffffffffffffc020. Its one branch tests the pointer loaded from 0xffffffffffffc020. When it is zero the routine stores a zero word to 0xffffffffffffc404; otherwise it stores a zero halfword at +0xf6 from the pointer and skips the word store. It makes no calls and returns 1, which could mean the error was handled, but the facts do not show what the 1 means.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/unix.IP30` at 0xa800000020010364
  **Inferred.** `earlybaerror` (96 bytes) is the early variant of `baerror`: the same 8-byte accesses to the fixed addresses, and a 4-byte store to 0xa8000000203e2104, the word the facts label "nofault". It has no branches or calls and returns 1.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/unix.IP30` at 0xa800000020011730
  **Inferred.** `_ecc_errorvec` (104 bytes) loads the 8-byte word at 0xffffffffaff50000, which `badaddr`, `baerror` and `VEC_dbe` also read, then does an indexed 8-byte load through that value (scaled by 8, plus 0xff0) and two 8-byte stores through the loaded pointer, at +0xc8 and +0x0. It has no conditional branches and ends with an indirect tail jump through a register to 0x900000002000ddc0, passing a0 to a3 unchanged. The name suggests an ECC error vector, but what it saves and why is not established, so its role is open.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.22/unix.IP30` at 0xa800000020017974
  **Inferred.** `dobuserr_common` (9460 bytes) reads Heart error state from fixed addresses such as 0xffffffffaff00000, 0xffffffffaff10040 and 0x9000000018000060. It updates the word `heart_error_cpumap` at 0xa8000000203da660 with ll and sc, and it calls heart_intr (0xa800000020035bcc), heart_paddr_to_port (0xa800000020035988), heart_error_refine (0xa800000020035aa4) and hem_add_ioe (0xa800000020017734) on several paths. Most of its 155 calls are cmn_err reports, and the facts do not show which error class takes which path, so the decode order is open.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/unix.IP30` at 0xa80000002001a360
  **Inferred.** `VEC_dbe` (88 bytes) loads from 0xffffffffaff50000 and 0xffffffffb8000000, makes an indexed load and store through the value read from 0xffffffffaff50000, writes to the frame at arg0 +0x140 in the delay slot of the jump, and then jumps directly to VEC_trap (0xa80000002000f3d0) with the same four arguments. The facts do not show how the indexed store is used, and the frame offset is inferred from the store alone. Role (from name): data-bus-error exception vector.
  Reviewed: 2026-11-12 (sonnet)
