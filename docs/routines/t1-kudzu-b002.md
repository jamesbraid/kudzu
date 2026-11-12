# Routine role reads, kudzu batch 2

These entries say what 25 routines of the IP30 kernel image do, read from the instruction facts rendered for each one. Every label is Inferred: the facts give the control flow, loads, stores and callees, but not the names' meaning or the device behind each address, so the roles below are the reader's reading of those facts.

- `__nanotime` in `6.5.22/unix.IP30` at 0xa800000020009ca0
  **Inferred.** Takes the mutex at 0xa8000000203e1454 with `mutex_spinlock_spl` and releases it with `mutex_spinunlock` on the paths that reach the two unlock calls. It reads a 64-bit word at 0xffffffffaff20000 and compares it against the sum of the words at 0xa8000000203e1478 and 0xa8000000203e1480, tests the word at 0xa8000000203e1470 against zero, and compares values read from the tables at 0xffffffffc4653600 and 0xffffffff88ca6c00 against the constant 0x3b9aca00 (10^9). The stores and the returned value are not traced through every branch, so the arithmetic is open.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.22/unix.IP30` at 0xa80000002000a3d8
  **Inferred.** Loads a word at 0xffffffffffffc6f4 and returns it when it is non-zero. When it is zero it calls `findcpufreq_hz` at 0xa80000002000bda4, and a 32-bit store follows the call at 0xa80000002000a408 whose target the facts do not resolve. Role (from name): a cached CPU-frequency-in-cycles lookup; the caching is a reading of the load, call and store pattern.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.22/unix.IP30` at 0xa80000002000bf0c
  **Inferred.** On the path that does not report an error it calls `spl65` at 0xa80000002000f514 with a4=0xf4240 (1,000,000), makes a 32-bit and a 64-bit store whose targets the facts do not resolve, calls `set_timer_intr` at 0xa800000020008fe0 with a5=0x1, and then calls `splx` at 0xa80000002000f564 and returns its result. It tests the word at 0xa8000000203db010 against zero and against the bounds 0x1f4 (500) and 0x9c4 (2500), and calls `fasthzerr` at 0xa80000002000bec0 with the strings at 0xa80000002033b0f8, 0xa80000002033b108 and 0xa80000002033b118 on the failing paths. Role (from name): starts the kernel clock; what the stored words are is not recovered.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.22/unix.IP30` at 0xa80000002000c0a4
  **Inferred.** Calls `spl65` at 0xa80000002000f514, makes one 64-bit store whose target the facts do not resolve, and then calls `splx` at 0xa80000002000f564 and returns its result. The stored value and target are not derived in the facts, so the effect on the clock is open. Role (from name): slows the kernel clock.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.22/unix.IP30` at 0xa80000002000c470
  **Inferred.** Polls the word at 0x900000001f000104 and loops while bit 0x40 is clear. When the bit is set it calls `cpu_soft_powerdown` at 0xa80000002000ccd0. The facts show nothing after that call, so what follows it is not established. The register is not identified, so the reset wait is a reading of the bit test.
  Reviewed: 2026-11-12 (sonnet)

- `_clock_func` in `6.5.22/unix.IP30` at 0xa80000002000c6d4
  **Inferred.** Takes the lock at 0xa8000000203e14c8 with `mutex_spinlock_spl`, calls `slow_ioc3_sio` at 0xa80000002000cd98 and later `restore_ioc3_sio` at 0xa80000002000cdb8, and releases the lock with `mutex_spinunlock`. It branches on a0 being zero. On the non-zero path it makes a series of byte stores interleaved with halfword loads at offsets 0x0 to 0xc, calls `restore_ioc3_sio` with a4=0x900000001f6c0000 and a5=0x900000001f6a0000, unlocks and returns 0 or 1. On the zero path it makes byte stores, then tests bit 0x40 of the byte at 0x900000001f6c0000 once; with the bit set it calls `restore_ioc3_sio` and `mutex_spinunlock` and, with it clear, goes on to byte loads and halfword stores. The store targets, the data and the device are not identified, so the clock-output role rests on the name.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_clearpower` in `6.5.22/unix.IP30` at 0xa80000002000cc60
  **Inferred.** Takes the lock at 0xa8000000203e14c8 with `mutex_spinlock_spl`, calls `slow_ioc3_sio`, makes three byte stores, calls `flushbus` at 0xa80000002000fbc4 with a0=0x30 and a1=0xa, makes one more byte store, then calls `restore_ioc3_sio`, releases the lock with `mutex_spinunlock` and returns its result. The store targets and values are not recovered. Role (from name): clears a power state; the register it lives in is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_soft_powerdown` in `6.5.22/unix.IP30` at 0xa80000002000ccd0
  **Inferred.** Calls a hook through the pointer at 0xa8000000203e14d0 when that pointer is non-zero, calls `conbuf_flush` at 0xa8000000201a83a8 with a0=0x3, and calls `heart_imr_bits_rmw` at 0xa80000002003459c with a1=0x200000000000000. It then takes the lock at 0xa8000000203e14c8, calls `slow_ioc3_sio`, makes a run of byte stores and loads whose targets the facts do not resolve, and calls `us_delay` at 0xa8000000200053c0 with a0=0x989680 (10,000,000). The facts end at the delay call, so how the routine finishes is not shown. Role (from name): a soft power-down; the power-down step itself is not in the rendered facts.
  Reviewed: 2026-11-12 (sonnet)

- `slow_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cd98
  **Inferred.** Loads the 32-bit word at 0x900000001f600028 and returns the value it read, and makes one 32-bit store whose target and value the facts do not resolve. Role (from name): the IOC3 chip is suggested by the name and address range; the pairing with `restore_ioc3_sio` is a reading of the names.
  Reviewed: 2026-11-12 (sonnet)

- `restore_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cdb8
  **Inferred.** Makes a byte store, loads one byte from 0x900000001f6c0000 and returns that byte, and makes a 32-bit store; the facts do not resolve the targets of the two stores. Its pairing with `slow_ioc3_sio` is a reading of the names and of the shared address range, not a documented contract.
  Reviewed: 2026-11-12 (sonnet)

- `nvchecksum` in `6.5.22/unix.IP30` at 0xa80000002000ce10
  **Inferred.** Runs two byte-store loops with a byte load at 0xa80000002000ce48 and another at 0xa80000002000ce84, bounded by signed compares against 0x80 and 0x100, and returns a value the facts do not resolve. The store targets are not resolved and the facts do not show how bytes are combined, so the checksum role comes only from the name. Treat the arithmetic as open.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/unix.IP30` at 0xa80000002000fc00
  **Inferred.** Selects a load by a1: lb for 1, lh for 2, lw for 4 and ld for 8, and calls `cmn_err` at 0xa8000000201a63b4 when a1 is none of those. It then tests bit 0x4000 of a loaded word and bit 0x10000 of the word at 0xffffffffaff10040, and branches to `baerror` at 0xa800000020010308 when either is set. Otherwise it returns 0. The name and the probe shape suggest a bus-error probe that reports whether the address is bad.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/unix.IP30` at 0xa80000002000fd28
  **Inferred.** Selects a load by a1 as `badaddr` does (lb, lh, lw, ld for 1, 2, 4, 8) and follows each load with a store of the same width, whose destination the facts do not resolve. It makes the same two error-bit tests, bit 0x4000 of a loaded word and bit 0x10000 of the word at 0xffffffffaff10040, branches to `baerror` at 0xa800000020010308 when either is set, and otherwise returns 0. The extra store is the difference from `badaddr` and the basis for the name; that it is the value-out parameter is a reading.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/unix.IP30` at 0xa80000002000fe94
  **Inferred.** Selects a store by a1: sb for 1, sh for 2, sw for 4 and sd for 8, and calls `cmn_err` at 0xa8000000201a63b4 when a1 is none of those. It does one lw load whose address the facts do not resolve before the error tests, then tests bit 0x4000 of a loaded word and bit 0x10000 of the word at 0xffffffffaff10040, branches to `baerror` at 0xa800000020010308 when either is set, and otherwise returns 0. Role (from name): a write-probe variant of `badaddr`.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/unix.IP30` at 0xa800000020010148
  **Inferred.** Selects a load by a1: lb for 1, lh for 2 and lw for 4; there is no ld case in the width chain, and any other width calls `cmn_err` at 0xa8000000201a63b4. It makes a 32-bit store whose target the facts do not resolve, tests bit 0x4000 of a loaded word and bit 0x10000 of the word at 0xffffffffaff10040, branches to `earlybaerror` at 0xa800000020010364 when either is set, and otherwise returns 0. Role (from name): the early-boot variant of `badaddr`, supported by the narrower width chain.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/unix.IP30` at 0xa800000020010308
  **Inferred.** Makes a series of 64-bit loads and stores whose addresses the facts do not resolve, then loads a pointer at -0x3fe0; when that pointer is non-zero it stores a halfword at offset 0xf6 from it. It makes a 32-bit store and returns 1. Role (from name): a bus-error handler; what the loads and stores read and clear is not recovered.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/unix.IP30` at 0xa800000020010364
  **Inferred.** Makes a series of 64-bit loads and stores and one 32-bit store whose targets the facts do not resolve, then returns 1. Unlike `baerror` the facts show no load of the -0x3fe0 pointer and no halfword store at offset 0xf6. Role (from name): the early-boot counterpart of `baerror`.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/unix.IP30` at 0xa800000020011730
  **Inferred.** Makes two 64-bit loads, the second at offset 0xff0 (4080), and two 64-bit stores at offsets 0xc8 and 0x0, whose bases the facts do not resolve. It ends with a tail jump to 0x900000002000ddc0, which the facts do not resolve to a named callee, so the handler it dispatches to is unidentified. Role (from name): an ECC error vector.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.22/unix.IP30` at 0xa800000020017974
  **Inferred.** This is a large routine of 9460 bytes, and only its call summary and return sites were read. It calls `cmn_err` 127 times and calls `hem_add_sync`, `hem_add_ioe`, `heart_error_refine`, `heart_paddr_to_port`, `heart_intr`, `log_dimmerr`, `tlb_to_phys` and `__dcache_wb_inval`, so it looks like a bus-error decode and log routine. The decision tree between those calls was not traced, so the decode rules remain open.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/unix.IP30` at 0xa80000002001a360
  **Inferred.** Makes five 64-bit and one halfword load and one 64-bit store whose addresses the facts do not resolve, then tail-jumps to `VEC_trap` at 0xa80000002000f3d0. A 64-bit store at offset 0x140 sits in the jump's delay slot. Role (from name): the data-bus-error vector; the jump to `VEC_trap` is what the facts show.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/unix.IP30` at 0xa80000002001b9d4
  **Inferred.** Reads fields of the record in a1 at offsets 0x18, 0x28, 0x2c, 0x30, 0x34, 0x238 and 0x23c and writes some of them back, including offsets 0x28, 0x2c, 0x30, 0x34 and 0x238. It calls `ecc_cacheOp` at 0xa80000002001b3b8 eight times with codes drawn from 0x0, 0x1, 0x3, 0x4, 0x5, 0x7 and 0x13, and `ecc_force_cache_line_invalid` at 0xa80000002001b8dc three times. It calls `cer_to_hex` at 0xa80000002001b930 on the word at offset 0x18, and calls `maddr_to_dimm` at 0xa80000002001202c and then `sprintf` at 0xa8000000201a7100 with the result as an argument. The meaning of the cache op codes is not known from the facts.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/unix.IP30` at 0xa80000002001f970
  **Inferred.** Has the same shape as `VEC_dbe`: loads and stores whose addresses the facts do not resolve, then a tail jump to `VEC_trap` at 0xa80000002000f3d0 with a 64-bit store at offset 0x140 in the delay slot. It begins with a branch on the return register, and that branch is not traced further. Role (from name): the instruction-bus-error vector, supported by the shape shared with `VEC_dbe`.
  Reviewed: 2026-11-12 (sonnet)

- `heart_cpu_intr` in `6.5.22/unix.IP30` at 0xa800000020034998
  **Inferred.** Loads a halfword, calls `doacvec` at 0xa8000000201b2eac, makes one 64-bit store whose address the facts do not resolve, and calls `doactions` at 0xa8000000201b2bf0, returning the result of `doactions`. Role (from name): a Heart CPU interrupt handler, with the dispatch role taken from the callee names.
  Reviewed: 2026-11-12 (sonnet)

- `heart_debug_intr` in `6.5.22/unix.IP30` at 0xa8000000200349d0
  **Inferred.** Loads a halfword, calls `debug` at 0xa8000000200103dc with a0=0xa800000020340160, makes one 64-bit store whose address the facts do not resolve, and returns the result of `debug`. Role (from name): the debug-interrupt counterpart of `heart_cpu_intr`.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.22/unix.IP30` at 0xa800000020034a08
  **Inferred.** Loads a doubleword at 0xffffffffaff10030 and tests the bits in a2 against it, branching to 0xa800000020034a40 when they are clear. Otherwise it makes a 64-bit store whose address the facts do not resolve and calls `debug` at 0xa8000000200103dc with a0=0xa800000020340160, the same state pointer as `heart_debug_intr`. The return value is the result of `debug` or an unresolved value. The bit test is the only condition the facts show; what a2 holds at entry is unknown.
  Reviewed: 2026-11-12 (sonnet)
