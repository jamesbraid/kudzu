# Tier-1 routine readings, kudzu batch 21

Role statements for routines 525 to 549 of the tier-1 list, read from their instruction facts only. Every grade is inferred: the facts show what each routine loads, stores, calls and branches on, not what the fields and constants mean. Addresses are in the binary named in each header. Hub addresses are the 0x90... and 0x92... values the facts report. Offsets such as -0x3bd8 are relative to a base the facts do not name.

- `counter_intr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x275f8
  **Inferred.** `counter_intr` is a 728-byte handler that calls `_get_count`, `_get_compare` and `resetcounter`, which fits a periodic counter tick. Its first test is bit 0x20 of the doubleword at -0x3d80: when set, it goes to a `log_tstamp_event` call; when clear, it tests the word at -0x3958. If that word is nonzero it calls `frs_handle_counter_intr`. If it is zero it calls `hwperf_overflowed_intr`. When that returns nonzero, it calls `l2_sbe_intr` if the low byte of `l2_sbe_check` is nonzero (then `hwperf_intr` unless bit 0 of `l2_sbe_check` is set), and `hwperf_intr` directly if that byte is zero. When it returns zero, the path reaches `resetcounter`, `clock` and the timer reads. A warning path compares against `counter_intr_warn_cycles` and can reach `cmn_err` at 0x278b0. The bit meanings and the warning threshold are not in the facts, so the handler's full job stays partly open.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x28d88
  **Inferred.** `doadjtime` is a 688-byte routine that takes `timelock` with `nested_spinlock` and releases it with `mutex_spinunlock`, bracketing the work with `splprof`. It calls `__nanotime` at 0x29038 on two separate paths: one entered when its first argument is nonzero, and one entered when it is zero. A compare of `s0` against 0xf4240 (one million) at 0x28efc sits on the first path, not between the two reads. The constant 0x3e8 (1000) is in a2 at the `splprof` calls and is also a bound on `s0`; the facts do not show whether it is an argument, and give no units. The name points at an adjustment of the time of day, but the offset it changes is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29038
  **Inferred.** `__nanotime` takes `timelock` with `mutex_spinlock_spl` and releases it with `mutex_spinunlock`, so it reads a shared clock under a lock. It reads a doubleword at 0x9200000001030100 and compares it against the sum of `adjtime_delta` and `adjtime_RTC`, and it tests `adjtime_nanos` against zero. Several compares use 0x3b9aca00, which is 1,000,000,000, the nanoseconds in a second, so the routine probably carries a nanosecond count across seconds, but the facts show only the compares.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29938
  **Inferred.** `findcpufreq_cycles` is a 252-byte routine. When the word at -0x38b4 is nonzero it returns at once; when it is zero it brackets its work with `splhi` and `splx`, zeros the counter with `_set_count`, and reads it back with `_get_count`. Between those calls it reads the hub doubleword at 0x9200000001030100 several times and compares it against a register value, with a loop back to an earlier read. It passes the final count to `cpu_cycles_adjust`. The name suggests a CPU frequency measurement, but the facts show no arithmetic that turns a cycle count into a frequency.
  Reviewed: 2026-11-12 (sonnet)

- `acktmoclock` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2a174
  **Inferred.** `acktmoclock` is a 60-byte routine that stores doublewords to hub addresses 0x9200000001000128 and 0x9200000001000120. It finds a per-CPU structure through `pdaindr`, indexed by a halfword global at -0x3bd8, and tests bit 0 of the byte at offset 0x528 in it. Each side of that test makes exactly one store and then returns: the facts show a store at 0x2a19c on the fall side and one at 0x2a1ac on the taken side, but not which hub address goes with which. It returns the halfword global shifted left by 4. The register meanings and the acknowledge semantics are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2a1b0
  **Inferred.** `ackkgclock` has the same 60-byte shape as `acktmoclock`: the same halfword global, the same per-CPU lookup, the same bit-0 test at offset 0x528, one store on each side of that test, and the same shifted-index return. Its two hub stores go to 0x9200000001000138 and 0x9200000001000130 instead. The name suggests a kernel-global clock acknowledge, but the facts do not show what those hub words control.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2a1ec
  **Inferred.** `startkgclock` is a 480-byte setup routine that takes the lock at .sbss+192 with `mutex_spinlock_spl` and releases it at two points. It writes hub words at 0x9200000001000158, 0x9200000001000118 and 0x9200000001000150, and it reads the cycle word at 0x9200000001030100, which `prof_intr` and `__nanotime` also read. When a word at .sdata+360 is zero, it calls `setkgvector` with `prof_intr`, the handler at 0x2ae70. The listing does not show what that word holds or what `cnodetocpu` returns.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2a3cc
  **Inferred.** `slowkgclock` takes the same .sbss+192 lock as `startkgclock` and writes hub words at 0x9200000001000158 and 0x9200000001000150. It calls `ackkgclock` after releasing the lock, and it checks `master_procid` against the halfword global at -0x3bd8 to choose between two branches. When a flag at .sdata+368 is zero, it runs `spinlock_init` on the lock and then rejoins the main path. The name suggests a slower tick than `prof_intr`, but the listing gives no rate.
  Reviewed: 2026-11-12 (sonnet)

- `prof_intr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2ae70
  **Inferred.** `prof_intr` is a 464-byte interrupt handler that calls `fastick_maint` on the path where bit 0x20 of the word at -0x3d80 is clear, and `cnode_subnode_to_cpu` later. It reads the cycle word at 0x9200000001030100 and updates a doubleword at offset 0x3c50 from a pointer loaded from -0x3bf8; the facts do not say where that structure lives. It also writes 0x9200000001000118, the hub word that `startkgclock` also writes. Two `log_tstamp_event` calls run when bit 0x20 of the word at -0x3d80 is set, the same bit `counter_intr` tests. The name points at profiling, but the facts do not show what the doubleword counts.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_mem` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2ba5c
  **Inferred.** `mdir_xlate_addr_mem` reads one doubleword at 0x9200000001780008 and formats text with `sprintf`, using the format string at .rodata+46968. The sign of its third argument picks one of two `sprintf` calls, and the negative path also passes the second argument. The name suggests a memory-directory translation, and since the output is text it is probably diagnostic, but the facts do not settle that.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_pdir` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2bb00
  **Inferred.** `mdir_xlate_addr_pdir` is the same kind of routine as `mdir_xlate_addr_mem`. It reads the hub doubleword at 0x9200000001780008 and makes one of two `sprintf` calls with the format string at .rodata+46968. The two paths differ in arguments: the non-negative path passes the fourth argument, and the negative path does not pass it as far as the listing shows. The name suggests a page-directory translation, but the facts show only the formatting calls, so the purpose is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_sdir` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2bb80
  **Inferred.** `mdir_xlate_addr_sdir` reads the same hub doubleword at 0x9200000001780008 and makes one `sprintf` call with the format string at .rodata+46968. It has no branch, and it passes the constant 0x1ffffffff, a 33-bit all-ones value, as one of the extra operands. The name suggests a segment-directory translation, which the facts do not confirm, and the constant may be a mask or a sentinel.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2e4c0
  **Inferred.** `_ecc_errorvec` is an 88-byte exception vector stub. It stores doublewords to low absolute addresses 0x400, 0x4c8 and 0x4d0, and it reads a doubleword at 0xfd8. A branch on whether that word is zero sits just before an indirect `jr` call that passes its four argument registers through unchanged, so the real handler is chosen at run time through a register. What the stored words are is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3761c
  **Inferred.** `earlybadaddr` is an early boot probe. It reads 1, 2 or 4 bytes at its first argument, chosen by the size in its second argument, and a size that matches none of them goes to `cmn_err` at 0x376b0. It writes the `nofault` word at two points, the usual shape of a recovery hook armed before the access and cleared after, but the stored values are not in the facts. A status bit tested at 0x376c4 can send it to `earlybaerror`, which returns 1 instead of this routine's 0.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x37768
  **Inferred.** `baerror` is a 48-byte bus-error helper. It loads one doubleword, the pointer kept at -0x3fe0 (0xffffffffffffc020), and tests it against zero. If the pointer is non-null, it stores a halfword at offset 0xf6 through that pointer; if the pointer is null, it stores a word to 0xffffffffffffc48c instead. It returns 1 on either path. The name suggests a bus-error record, but the record layout and the stored values are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x37798
  **Inferred.** `earlybaerror` is a 52-byte exit path that writes one word to `nofault` at offset 0 and returns 1. `earlybadaddr` branches here when its status bit test is taken, so it is probably the bus-error return of that probe. The stored value is not visible in the listing.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x377d8
  **Inferred.** `cpu_waitforreset` is an 8-byte routine. The facts list no call, return, load, store or branch for it, so they show no way out of it, and a busy wait is the likely shape. The name suggests a secondary CPU parked until reset, but the facts show no reset check.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x39480
  **Inferred.** `VEC_dbe` is a 12-byte exception vector entry. It loads a halfword from 0xffffffffffffc428, stores a doubleword at offset 0x140 of its first argument, and jumps to `VEC_trap` with its argument registers unchanged. The name suggests a data bus error, and the store offset suggests the first argument points at saved state, but neither is confirmed by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3badc
  **Inferred.** `ecc_handler` is the largest routine in this batch, at 1992 bytes. It reads and writes an error record through its second argument at several offsets up to 0x34 and at 0x2b8, and reads 0x2bc. It tests bit masks of the word at +0x18, including 0x4000000 and 0x8000000, 0x5500000 and 0xaa00000, 0x3800000 and 0xc0000000. Depending on those tests it calls `ecc_cacheOp` with several operation codes, `ecc_force_cache_line_invalid`, `save_cache_error`, `clear_sline` and `memerror_handle_uce`, and it calls `eccf_recoverable_threshold` and reads from `nasid_to_compact_node`. On one path it calls `cmn_err_tag` with a0 = 0xb96 and a1 = 4; other paths return without it. The masks look like a split between correctable and uncorrectable errors, but the facts do not name the bits, so that classification is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3d12c
  **Inferred.** `set_leds` is a 308-byte routine that reads the node's slice and NASID with `get_slice` and `get_nasid`, and it compares the chassis type from `get_chimera_type` against 3. It sends LED values through `elsc_led_set` with codes 0x101, 0x102, 0x200 and 0x201 on different paths, and it sends a message through `elsc_display_mesg` on one of them. Each `elsc_led_set` call takes a controller handle from `get_elsc`; for two of the `get_elsc` calls the argument is an address built from the slice and NASID words on a base at 0x9000000030000000. The name fits an LED update, but the LED codes and the message text are not decoded in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `bte_init` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3d27c
  **Inferred.** `bte_init` is a 344-byte routine that fills a descriptor at its first argument. It writes doublewords at offsets 0x0, 0x18, 0x20 and 0x28, a halfword at 0x38, and words at 0x4c and 0x68. It runs `spinlock_init` on a lock at offset 0x3c, reads a halfword from a table indexed by the halfword at -0x3bd6, and stores to the hub global at 0x92000000014001f8 on one of two paths chosen by its third argument. A test of `scache_linemask` and a check that `maxcpus` is below 33 gate parts of the setup, and the routine returns its first argument. The facts do not name the table, and the name suggests a block transfer engine, which they do not confirm.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3ec7c
  **Inferred.** `badaddr` is a 48-byte wrapper. It calls `probe_common` through the pointer stored at 0x3ed4c, passing an address 48 bytes below `sp` as its first argument. It forwards its other three arguments unchanged and returns the result. The name suggests a bad-address test, but the wrapper does not show whether the probe reads or writes, or what the arguments mean.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3ecac
  **Inferred.** `badaddr_val` has the same 48-byte body as `badaddr`: a call to `probe_common` through the pointer at 0x3ed4c, with an address 48 bytes below `sp` and the other arguments forwarded. The listing shows no difference between the two beyond their entry addresses, so whatever the value handling does, it happens inside `probe_common`, which this entry does not read. The name suggests a probe that carries a value, which this wrapper does not show.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3ecdc
  **Inferred.** `wbadaddr` is a 56-byte wrapper with the same call as `badaddr`: `probe_common` through the pointer at 0x3ed4c, an address 48 bytes below `sp`, and the other arguments forwarded. It is 8 bytes longer than its neighbours, but the facts list no call, branch or memory access besides the call and the return, so the extra bytes are not explained. The name suggests a write probe, which the wrapper does not show.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x439c0
  **Inferred.** `VEC_ibe` is a 36-byte exception vector entry, and its hand-off matches `VEC_dbe`: it stores a doubleword at offset 0x140 and jumps to `VEC_trap` with `a0`, `a1` and `a3` at entry (`a2` is not the entry value). Before that it makes two doubleword loads whose bases the facts do not resolve, loads a halfword from 0xffffffffffffc428, and tests the sign of register `ra`. The name suggests an instruction bus error, and the facts do not explain the sign test.
  Reviewed: 2026-11-12 (sonnet)
