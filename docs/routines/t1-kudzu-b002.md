# Routine role reads, kudzu batch 2

These entries say what 25 routines of the IP30 kernel image do, read from the instruction facts rendered for each one. Every label is Inferred: the facts give the control flow, loads, stores and callees, but not the names' meaning or the device behind each address, so the roles below are the reader's reading of those facts.

- `__nanotime` in `6.5.22/unix.IP30` at 0xa800000020009ca0
  **Inferred.** Takes the mutex at 0xa8000000203e1454 with `mutex_spinlock_spl` and releases it with `mutex_spinunlock`, while it reads a 64-bit counter at 0xffffffffaff20000 and a pair of words at 0xa8000000203e1478 and 0xa8000000203e1480. It works on a seconds and nanoseconds pair kept at 0xa8000000203e33a0 and 0xa8000000203e33a8, normalizes against the constant 0x3b9aca00 (10^9), and compares against values read from the tables at 0xffffffffc4653600 and 0xffffffff88ca6c00. The exact arithmetic is not traced through every branch.

- `findcpufreq_cycles` in `6.5.22/unix.IP30` at 0xa80000002000a3d8
  **Inferred.** Reads a cached word at 0xffffffffffffc6f4 and, when it is zero, calls `findcpufreq_hz` at 0xa80000002000bda4 to compute it. Its return value is that cached word or the new result, and the store at 0xa80000002000a408 writes the cache back. The cache role is a reading of the load and store pattern.

- `startkgclock` in `6.5.22/unix.IP30` at 0xa80000002000bf0c
  **Inferred.** Brackets the work with `spl65` at 0xa80000002000f514 and `splx` at 0xa80000002000f564, passing 0xf4240 (1,000,000) in a4 to the `spl65` call, and calls `set_timer_intr` at 0xa800000020008fe0 with a5=0x1. It checks the word at 0xa8000000203db010 against the bounds 0x1f4 (500) and 0x9c4 (2500), and reports failures through `fasthzerr` at 0xa80000002000bec0 with the message strings at 0xa80000002033b0f8, 0xa80000002033b108 and 0xa80000002033b118. It stores to 0xffffffffffffc56c and 0xffffffffffffc550. Its name suggests it starts the kernel clock, but the exact role of the stored words is not recovered.

- `slowkgclock` in `6.5.22/unix.IP30` at 0xa80000002000c0a4
  **Inferred.** Runs under `spl65` and `splx` at 0xa80000002000f514 and 0xa80000002000f564 and stores 8 bytes to 0xffffffffffffc550, the same clock word that `startkgclock` writes. The stored value is not derived in the facts, so the routine's effect on the clock rate is open. The name suggests it slows the kernel clock.

- `cpu_waitforreset` in `6.5.22/unix.IP30` at 0xa80000002000c470
  **Inferred.** Polls the word at 0x900000001f000104 and loops while bit 0x40 is clear. When the bit is set it calls `cpu_soft_powerdown` at 0xa80000002000ccd0 and then loops back to the poll. The register is not identified, so the reset wait is a reading of the bit test.

- `_clock_func` in `6.5.22/unix.IP30` at 0xa80000002000c6d4
  **Inferred.** Takes its argument in a0, copies seven halfwords between the structure at a1 and back, and writes 23 bytes to 0x900000001f6a0000 and 13 bytes to 0x900000001f6c0000. It takes the lock at 0xa8000000203e14c8, wraps its port writes in `slow_ioc3_sio` at 0xa80000002000cd98 and `restore_ioc3_sio` at 0xa80000002000cdb8, and polls bit 0x40 of the byte at 0x900000001f6c0000 before it returns 0 or 1. The data it sends and the device it drives are not identified, so the clock-output role rests on the name.

- `cpu_clearpower` in `6.5.22/unix.IP30` at 0xa80000002000cc60
  **Inferred.** Takes the lock at 0xa8000000203e14c8 and calls `slow_ioc3_sio`, then `flushbus` at 0xa80000002000fbc4 with a0=0x30 and a1=0xa, and then `restore_ioc3_sio`. Between those calls it stores bytes to 0x900000001f6a0000 and 0x900000001f6c0000, and the stored values are not recovered. The name suggests it clears a power state, but the register that state lives in is unknown.

- `cpu_soft_powerdown` in `6.5.22/unix.IP30` at 0xa80000002000ccd0
  **Inferred.** Calls an optional hook through the pointer at 0xa8000000203e14d0 (skipped when that pointer is zero), flushes the console buffer with `conbuf_flush` at 0xa8000000201a83a8, and changes a Heart interrupt mask with `heart_imr_bits_rmw` at 0xa80000002003459c using bit 0x200000000000000. It then pokes the same two byte ports as `cpu_clearpower`, delays with `us_delay` at 0xa8000000200053c0 using 0x989680 (10,000,000), and jumps back into its own body at 0xa80000002000cd78 with no return in the facts. The power-down step itself is not reached in the rendered facts.

- `slow_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cd98
  **Inferred.** Reads the 32-bit word at 0x900000001f600028, writes a 32-bit value back to the same word, and returns the value it read. The name and the address range suggest the IOC3 chip, but the write's value is not named in the facts. This routine is the first half of the save and restore pair with `restore_ioc3_sio`.

- `restore_ioc3_sio` in `6.5.22/unix.IP30` at 0xa80000002000cdb8
  **Inferred.** Writes one byte to 0x900000001f6a0000, reads one byte from 0x900000001f6c0000, and writes a 32-bit word to 0x900000001f600028. It returns the byte it read from 0x900000001f6c0000. Its pairing with `slow_ioc3_sio` is a reading of the shared addresses, not a documented contract.

- `nvchecksum` in `6.5.22/unix.IP30` at 0xa80000002000ce10
  **Inferred.** Loops over byte-wide accesses, reading a byte from 0x900000001f6c0000 and writing bytes to 0x900000001f6a0000 and 0x900000001f6c0000, with bounds 0x80 and 0x100 in its signed compares. The facts do not show how the bytes are combined into a checksum, so the checksum role comes only from the name. Treat the arithmetic as open.

- `badaddr` in `6.5.22/unix.IP30` at 0xa80000002000fc00
  **Inferred.** Probes an address in a0 with a load whose width comes from a1: 1, 2, 4 or 8 bytes for a1 = 1, 2, 4 or 8. It checks bits 0x4000 and 0x10000 of the error words at 0xffffffffaff10040 and branches to `baerror` at 0xa800000020010308 when either is set, and it returns 0 on the fall-through path. The name and the probe shape suggest a bus-error probe that returns whether the address is bad.

- `badaddr_val` in `6.5.22/unix.IP30` at 0xa80000002000fd28
  **Inferred.** Probes the address in a0 the way `badaddr` does, with the same width selection by a1 and the same error-word checks, and it also stores the loaded value through the pointer in a2 with a matching sized store. It tails into `baerror` at 0xa800000020010308 on error and returns 0 on the normal path. The added store is the difference from `badaddr`, and that is the basis for the name.

- `wbadaddr` in `6.5.22/unix.IP30` at 0xa80000002000fe94
  **Inferred.** Probes by storing: it writes a 1, 2, 4 or 8 byte value to the address in a0, chosen by a1, under the same error-word checks used by `badaddr`. It reads 0x9000000000000000 with `lw` before it returns, and the facts do not say why. It returns 0 on the normal path and tails into `baerror` on error.

- `earlybadaddr` in `6.5.22/unix.IP30` at 0xa800000020010148
  **Inferred.** Probes the address in a0 with a load of 1, 2 or 4 bytes; the width-8 case is absent from its width chain. It records status in the word at 0xa8000000203e2104 and returns 0 or branches to `earlybaerror` at 0xa800000020010364 on error. The early-boot role comes from the narrower width chain and the nofault word.

- `baerror` in `6.5.22/unix.IP30` at 0xa800000020010308
  **Inferred.** Saves two words from 0xffffffffb8000000 and 0xffffffffb8000040 and 0xffffffffb8000060, reads and writes back the word at 0xffffffffaff10040, reads 0xffffffffaff50000, and sets a flag through the pointer at -0x3fe0 +0xf6 with a halfword store. It stores a word at 0xffffffffffffc404 and returns 1. The read-modify-write on 0xffffffffaff10040 suggests it clears the error status, but the write's effect is inferred from the pattern.

- `earlybaerror` in `6.5.22/unix.IP30` at 0xa800000020010364
  **Inferred.** Follows the same save, error-word and restore pattern as `baerror`, but stores a word to the nofault location at 0xa8000000203e2104 and reads 0xffffffffaff50000 without the halfword flag write. It returns 1. It is the early-boot counterpart of `baerror`.

- `_ecc_errorvec` in `6.5.22/unix.IP30` at 0xa800000020011730
  **Inferred.** Reads a value from 0xffffffffaff50000, uses it as an index into an 8-byte table at offset 4080 from the scaled index, and stores two doublewords into that entry. It ends with a `jr` to 0x900000002000ddc0, which the facts do not resolve to a named callee, so the handler it dispatches to is unidentified.

- `dobuserr_common` in `6.5.22/unix.IP30` at 0xa800000020017974
  **Inferred.** This is a large routine of 9460 bytes, and only its call summary and return sites were read. It calls `cmn_err` 127 times and calls `hem_add_sync`, `hem_add_ioe`, `heart_error_refine`, `heart_paddr_to_port`, `heart_intr`, `log_dimmerr`, `tlb_to_phys` and `__dcache_wb_inval`, so it looks like a bus-error decode and log routine. The decision tree between those calls was not traced, so the decode rules remain open.

- `VEC_dbe` in `6.5.22/unix.IP30` at 0xa80000002001a360
  **Inferred.** Updates a table entry indexed by the word at 0xffffffffaff50000 and reads 0xffffffffb8000000 and a halfword at 0xffffffffffffc3a0. It stores a word into the first argument at offset 0x140 and then jumps to `VEC_trap` at 0xa80000002000f3d0. It is the data-bus-error vector, and its jump to `VEC_trap` is what the facts show.

- `ecc_handler` in `6.5.22/unix.IP30` at 0xa80000002001b9d4
  **Inferred.** Reads fields of the record in a1 at offsets 0x0, 0x18, 0x238 and 0x23c and writes some of them back. It calls `ecc_cacheOp` at 0xa80000002001b3b8 eight times with codes drawn from 0x0, 0x1, 0x3, 0x4, 0x5, 0x7 and 0x13, and `ecc_force_cache_line_invalid` at 0xa80000002001b8dc three times. It formats a DIMM location with `maddr_to_dimm` at 0xa80000002001202c and `sprintf` at 0xa8000000201a7100, and it formats the error register with `cer_to_hex` at 0xa80000002001b930. The meaning of the cache op codes is not known from the facts.

- `VEC_ibe` in `6.5.22/unix.IP30` at 0xa80000002001f970
  **Inferred.** Follows the same pattern as `VEC_dbe`: it updates an indexed table from the word at 0xffffffffaff50000, stores a word at offset 0x140 of its first argument, and jumps to `VEC_trap` at 0xa80000002000f3d0. It begins with a branch on the return register, and that branch is not traced further. The role as the instruction-bus-error vector comes from the name and the shared shape with `VEC_dbe`.

- `heart_cpu_intr` in `6.5.22/unix.IP30` at 0xa800000020034998
  **Inferred.** Reads a halfword flag at 0xffffffffffffc3a0, stores a doubleword to 0xffffffffaff10028, then calls `doacvec` at 0xa8000000201b2eac and `doactions` at 0xa8000000201b2bf0, returning the result of `doactions`. The address range suggests the Heart chip's interrupt status, and the dispatch role comes from the callee names.

- `heart_debug_intr` in `6.5.22/unix.IP30` at 0xa8000000200349d0
  **Inferred.** Reads the same halfword flag, stores a doubleword to 0xffffffffaff10028, and calls `debug` at 0xa8000000200103dc with the state pointer 0xa800000020340160. It returns the result of `debug`. This is the debug-interrupt counterpart of `heart_cpu_intr`.

- `chkdebug` in `6.5.22/unix.IP30` at 0xa800000020034a08
  **Inferred.** Reads a doubleword at 0xffffffffaff10030 and tests the bits in a2 against it, branching to 0xa800000020034a40 when the bits are clear. It stores a doubleword to 0xffffffffaff10028 and calls `debug` at 0xa8000000200103dc with the same state pointer as `heart_debug_intr`. The bit test is the only condition the facts show; what a2 holds at entry is unknown.
