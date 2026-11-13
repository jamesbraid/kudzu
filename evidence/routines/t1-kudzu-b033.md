# Routine role statements, kudzu IP30 kernel and boot objects

Role statements for 25 routines from the routine list, all in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o`.
Each statement comes from the disassembled instructions only: loads, stores, calls, branches and constants.
Each grade is inferred. The disassembly shows what the code touches and when it branches, but not what the
fields and masks mean. Addresses are in the binary named in each header. Hub register addresses are the
uncached `0x92...` aliases the disassembly reports. Names in backticks are the symbol names the disassembly
gives; they are the only evidence for what a routine is for, so they are called out where they carry
the claim.

- `intpend0` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1ff0c
  **Inferred.** `intpend0` is a 352-byte routine that reads hub words 0x9200000001000098 and 0x9200000001000090 and
  writes the second. It wraps a loop around the semaphore helpers `cvsema` (0x1ff5c), `valusema` (0x1ffd8) and
  `vsema` (0x1fff8), and a branch at 0x1fff0 compares the `valusema` result with `hub_intr_wakeup_cnt`.
  It calls `ms1bit` (0x1ff84) on each pass of the outer loop closed by the branch at 0x20014. An indirect call at 0x1ffc8, whose target
  the disassembly does not resolve, is made only when the entry's word at +24 has bit 0x1000 set (0x1ffb0) and a pointer at +8 is nonzero
  (0x1ffbc). The shape fits draining pending interrupts for one level, but the meaning of the masks is
  not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `intpend1` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2006c
  **Inferred.** `intpend1` is 236 bytes and writes 0x9200000001000090 at 0x200c4, the same target as `intpend0`. It masks
  its pending word 0x92000000010000a0 against a global at 0x200a0, reads 0x9200000001000098 at 0x200e8, and
  calls `ms1bit` (0x200b8). Unlike `intpend0`, it calls no semaphore helpers, and it reaches a `cmn_err` call
  at 0x20148 with a format string at .rodata+32520 and makes an unresolved indirect call at 0x200f0. The
  disassembly does not say whether that `cmn_err` call is an error path.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x204c0
  **Inferred.** `sendintr` is a 72-byte leaf that indexes the per-CPU table `pdaindr` by its first argument, using 16-byte
  entries, and follows the record's pointer at +8 to a second record. From there it reads a halfword at
  +0x424 and a byte at +0x520 and compares its second argument with 0xab at 0x204dc. On the path where the second argument is not 0xab it
  returns 0 at 0x204f8, with a doubleword store at 0x204fc in the delay slot whose value and destination the facts do not
  resolve. The purpose of the 0xab test and of that store is open.
  Reviewed: 2026-11-12 (sonnet)

- `cpuintr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20508
  **Inferred.** `cpuintr` is a 276-byte handler that finds this CPU's record through `pdaindr` and the halfword at
  [-0x3be0]. It reads hub word 0x9200000001000098 and writes 0x9200000001000090, the same pair the
  pending-interrupt routines use, and a flag bit 0x20 in the word at [-0x3d80] gates two paths (0x2051c,
  0x205cc). It calls `bcopy` (0x205ac, length 0x168), `doacvec` (0x205b4), `doactions` (0x205bc), and two
  `log_tstamp_event` calls with event codes 0x71 and 0x65, the second passing 0x71 as its argument. That pair
  looks like an entry and exit marker, but the disassembly does not establish it.
  Reviewed: 2026-11-12 (sonnet)

- `dbgintr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2067c
  **Inferred.** `dbgintr` is a 76-byte routine that reads the halfword at abs 0xffffffffffffc420, the record byte at +0x520
  and hub word 0x9200000001000098, then writes 0x9200000001000090. It makes one call, to `debug` at 0x206b4,
  with the string at .rodata+32520, a byte from the record at +1312 plus 118, the hub address
  0x9200000001000090, and its own fourth argument. The disassembly does not show what that byte selects, so
  the routine's role as a debugger entry from the interrupt side is only the name's suggestion.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20974
  **Inferred.** `chkdebug` is an 80-byte gate that reads the halfword at abs 0xffffffffffffc420, the record's byte at +0x520
  and hub word 0x92000000010000a0. It masks that word with a register value and branches at 0x209a8, and the
  call to `dbgintr` at 0x209b0 sits on the nonzero path, receiving 0 as its first argument and forwarding its
  own second through fourth arguments. The masked word is the one `intpend1` reads, which fits a check for a
  pending debug request, but the disassembly does not show that link.
  Reviewed: 2026-11-12 (sonnet)

- `debug_stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20a98
  **Inferred.** `debug_stop_all_cpus` is a 236-byte loop over CPU indices from 0 up to the global `maxcpus`. Each index
  calls `get_cpu_slice` (0x20ac8) and `cpu_enabled` (0x20b14), disabled entries are skipped at 0x20b28, and a
  branch at 0x20b08 compares the index with the halfword at [-0x3be0], which fits skipping the calling CPU
  though the disassembly does not say so. The routine also stores 8 bytes to hub block 0x9200000001800090 at
  0x20aec, a page the interrupt words above do not use, and the disassembly has no interrupt-send call, so the
  stop mechanism is not visible despite the name.
  Reviewed: 2026-11-12 (sonnet)

- `get_my_slotid` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x23be4
  **Inferred.** `get_my_slotid` is a 36-byte wrapper that loads doubleword
  0x9200000001220048, a hub register on a
  different page from the interrupt words in this set. It calls `hub_slotbits_to_slot` (0x23bf4), forwarding its own
  second through fourth argument registers unchanged, and returns the helper's result. The facts do not resolve what the helper's first
  argument is, so that the loaded value is passed there is an inference. Role (from name): the name suggests the slot
  number for this CPU, but the disassembly shows a conversion and not the bit
  fields it extracts.
  Reviewed: 2026-11-12 (sonnet)

- `llp_pfail_intr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x252a4
  **Inferred.** `llp_pfail_intr` is the largest routine in this set at 792
  bytes. Role (from name): the name suggests it handles a link power-fail event; the disassembly does not say so.
  It reads hub word 0x9200000001608088 six times in a loop (0x25390 to 0x253f0), testing the single bit
  0x2000000000 on each read (branches at 0x253a8 to 0x253f8), and calls `msc_owner_cpu` through a pointer at 0x24fd0 (0x25304) in a loop
  that compares its result with the halfword at [-0x3be0]. It also
  makes a run of `vector_read` and `vector_write` calls (0x2532c to 0x255a0)
  with a2 set to 0x0, 0x30, 0x600110 or 0x8, reads and writes 0x9200000001600110, writes 0x9200000001600008 at
  0x25498, and waits 100 or 300
  microseconds (`us_delay`) between the calls, choosing a path by comparing a
  global at sbss+160 with 8. The
  disassembly gives the order of reads, writes and waits, but not what the
  vector values or the flag bit mean.
  Reviewed: 2026-11-12 (sonnet)

- `force_system_shutdown` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x26ad4
  **Inferred.** `force_system_shutdown` is a 100-byte routine that writes
  0x9200000001600008, the same port word
  `llp_pfail_intr` writes at 0x25498, at 0x26b34, after the `printf` call and the second `us_delay`. The facts do not list
  the branch at 0x26b30, so whether that store sits in a loop is not shown. It calls `cmn_err` with first argument 4 and a string at .rodata+34712,
  `us_delay` with 0x7a120 and
  0xf4240 (500000 and 1000000), `elsc_system_reset` with a pointer into
  .bss+37112, and `printf` with the same
  string. A negative return from `elsc_system_reset` takes a separate branch
  (0x26b00) to the `printf`; the return at 0x26b08 hands back the `elsc_system_reset` result. Role (from name): the name
  suggests a forced power-off,
  and these calls are the closest evidence for it.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2a510
  **Inferred.** `set_leds` is a 124-byte routine that reads the byte at abs
  0xffffffffffffc523, writes a word to abs
  0xffffffffffffc514 and reads it back. It calls `get_slice` and `get_nasid` on
  one of two paths chosen by a
  byte at [-0x3add] (0x2a51c). The single doubleword store at 0x2a55c, in the delay slot of the return at 0x2a558 (which returns the
  `get_nasid` result), writes 8 bytes to 0x9200000001a20050 or 0x9200000001a20080 plus a term built from those two results; the facts do not
  show which constant goes with which path. Role (from name): the name
  suggests front-panel LEDs, but the disassembly shows a store to a hub address
  picked by slice and node, not a bit
  pattern.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2a5b8
  **Inferred.** `bootstrap` is a 208-byte routine that reads hub word
  0x9200000001600000 and a doubleword and two
  halfwords at addresses the disassembly leaves unresolved. It calls
  `mapped_kernel_setup_tlb` (0x2a60c) with its incoming a2 and a3 argument registers, then jumps through a register loaded for `jr` at
  0x2a62c, which the relocation
  puts at .text+173620. Role (from name): the name and
  the TLB setup call fit an early kernel entry, but the jump target is not
  identified.
  Reviewed: 2026-11-12 (sonnet)

- `counter_intr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2c620
  **Inferred.** `counter_intr` is a 728-byte counter interrupt handler that reads the flag word at [-0x3d80], as `cpuintr`
  does, plus the halfword at 0xffffc420 and the word at 0xffffc6a0. It calls `frs_handle_counter_intr`,
  `hwperf_overflowed_intr`, `l2_sbe_intr`, `hwperf_intr`, `resetcounter` (twice), `clock`, `_get_count` and
  `_get_compare`, and its second `resetcounter` call takes a5 from hub word 0x9200000001030100, the counter
  that `__nanotime` and `findcpufreq_cycles` read. A compare against 1001 (0x3e9) with `lbolt` (0x2c6f8) and a
  threshold in `counter_intr_warn_period` (0x2c704, 0x2c714) suggest a periodic warning, and two
  `log_tstamp_event` calls with codes 0x6c and 0x65 sit near the end, though the disassembly shows many paths
  but not what each branch means.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2dfa8
  **Inferred.** `doadjtime` is a 492-byte routine that takes the `timelock`
  spinlock with `nested_spinlock` (0x2e004, 0x2e11c)
  inside a `splprof` bracket and reads the time with `__nanotime` (0x2dff4,
  0x2e110). It compares a value against 0xf4240 (1,000,000) at 0x2e0dc, releases the
  lock with `mutex_spinunlock`
  at 0x2e0ac and 0x2e17c (the second passing 0x3e8 in a4), and lists no direct
  memory stores. Role (from name): the name suggests
  a time-of-day adjustment step and the constants may fit microsecond stepping, but
  the disassembly does not establish the
  adjustment rule.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2e194
  **Inferred.** `__nanotime` is a 572-byte time read that takes `timelock` with `mutex_spinlock_spl` (0x2e1a0) inside
  `splprof` and reads hub counter 0x9200000001030100 (0x2e1bc). It compares that counter with the sum of the
  `adjtime_delta` and `adjtime_RTC` globals (0x2e1f0), reads `adjtime_nanos` (0x2e228), and normalizes seconds
  and nanoseconds against 0x3b9aca00 (10^9) in loops. It also reads and writes a pair of words the disassembly
  labels `sym(nasid_to_compact_node)+0x9898` and `+0x9890`, a label that looks misplaced, so the location of
  that cache is not identified, and it releases the lock with `mutex_spinunlock` at 0x2e340 and 0x2e3c0.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2e924
  **Inferred.** `findcpufreq_cycles` is a 212-byte routine that reads a word at
  abs 0xffffffffffffc744 and returns early when it is nonzero (0x2e92c). Otherwise it
  writes a word back at
  0x2e9e0 after a `cpu_cycles_adjust` call, and runs inside a `splhi` bracket that clears the count register
  through `_set_count` with 0. It
  then spins from 0x2e974 to 0x2e9b4 until hub counter 0x9200000001030100 has
  advanced by 0x1312d0 (1,250,000)
  from its start value, and reads the count with `_get_count` and passes it to
  `cpu_cycles_adjust`. The loop is
  a timed wait, but the counter's tick rate is not in the disassembly, so the
  window's length in time is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `acktmoclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f3d0
  **Inferred.** `acktmoclock` is a 56-byte acknowledgement that reads the halfword at abs 0xffffffffffffc420, then the per-CPU
  record through `pdaindr` and its byte at +0x520, and tests a byte at +1312 of a sub-record (0x2f3ec). Its two
  hub stores, to 0x9200000001000128 and 0x9200000001000120, sit on opposite sides of that test, so each path
  appears to write one word, and it returns the `pdaindr` offset. The name suggests it clears a timeout-clock
  interrupt, but the disassembly does not name the two hub words.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f408
  **Inferred.** `ackkgclock` has the same shape as `acktmoclock`: it reads the halfword at abs 0xffffffffffffc420 and the
  per-CPU record through `pdaindr` and its byte at +0x520, then gates on the byte at +1312 (0x2f424). It writes
  0x9200000001000138 and 0x9200000001000130 on opposite sides of that test and returns the `pdaindr` offset.
  The name suggests an acknowledgement for the kernel-global clock that `startkgclock` and `slowkgclock` drive,
  but the disassembly does not state that.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f440
  **Inferred.** `startkgclock` is a 280-byte routine that takes the spinlock at
  sbss+288 with `mutex_spinlock_spl` (0x2f44c).
  A branch on the word at sdata+392 (0x2f45c) selects a path to the
  `setkgvector` call at 0x2f52c, which passes
  `prof_intr` (0x301e0) as its vector. On the other path it calls `cnodetocpu`
  (0x2f4c4) only when a value read through [-0x3c00]+18736 differs from 1 (0x2f4bc), passing the halfword at
  [-0x3bde] as its first argument, 1 as its second and that value plus 1 as its third; the halfword at [-0x3be0] is compared with the result at 0x2f4d0.
  The routine writes hub words
  0x9200000001000158, 0x9200000001000118
  and 0x9200000001000150, reads the counter 0x9200000001030100 (0x2f4e0), and
  releases the lock twice with
  `mutex_spinunlock`. The disassembly does not say what the `cnodetocpu` result
  selects.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f558
  **Inferred.** `slowkgclock` is a 216-byte routine that takes the same spinlock at sbss+288 with `mutex_spinlock_spl` (0x2f56c)
  and reads the halfword at 0xffffc420. It compares that halfword with `master_procid` (0x2f588), and on the
  master path it looks at the `audioclock` global (0x2f590), writing hub words 0x9200000001000158 and
  0x9200000001000150 (0x2f5c4, 0x2f614) and the word at 0xffffc534. It calls `ackkgclock` (0x2f5fc), calls
  `spinlock_init` (0x2f61c) with a string at .rodata+44008 on a path reached when the word at sdata+400 is zero
  (0x2f560), and releases the lock through `mutex_spinunlock` at 0x2f59c and 0x2f5f4. The name suggests a slower
  variant of the kernel-global clock tick, but the disassembly does not show the rate.
  Reviewed: 2026-11-12 (sonnet)

- `prof_intr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x301e0
  **Inferred.** `prof_intr` is a 252-byte routine that reads the
  halfwords at 0xffffc420 and 0xffffc422, the word
  at 0xffffc534, the table at 0xffffc400 and the hub counter 0x9200000001030100.
  It writes 0x9200000001000118 and a
  doubleword in the record at [-0x3c00]+0x4928, and a branch at 0x30284 compares a count in
  the record at +18728 against the
  hub counter. It calls `fastick_maint` (0x301f8) and `cnode_subnode_to_cpu`
  (0x30248, inside a loop closed by the branch at 0x30254, with the halfword at [-0x3bde] first, then 0, 1 and the halfword
  at [-0x3be0]), reaches `log_tstamp_event` with codes 0x6e and 0x65 (0x302b0,
  0x302cc), and `startkgclock` passes
  it as its vector. Role (from name): the names suggest a profiling tick handler and a fast-tick helper, but the
  disassembly confirms neither.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_mem` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x307f4
  **Inferred.** `mdir_xlate_addr_mem` is a 376-byte routine that reads hub word
  0x9200000001200018 (0x30810) and the word at
  0x9000000010000090 (0x3081c), and branches on that second word at 0x30834. It
  makes four `sprintf` calls
  (0x30890, 0x3091c, 0x30934, 0x3095c), each with the format string at
  .rodata+44192 and constants that include
  0x58 and 0x48, and signed tests of the input (0x30858,
  0x308dc) plus a compare against 0x36
  (0x308c0) pick the variant. The disassembly shows no table lookup and no
  translated address, so the routine may
  be a formatter for a memory-directory address rather than a translator, and
  the name claim is not established.
  Its return at 0x3089c is the `sprintf` result.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_pdir` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3096c
  **Inferred.** `mdir_xlate_addr_pdir` is a 260-byte sibling of the memory variant that reads only hub word 0x9200000001200018
  (0x30994). Its three `sprintf` calls (0x309f0, 0x30a14, 0x30a60) use the same format string at .rodata+44192 and
  byte-sized constants, and the sign of its input (0x309ac) and a zero test on a3 (0x309b4) choose the branch.
  Like the memory variant it returns the `sprintf` result, and the disassembly shows no lookup behind the name.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_sdir` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x30a70
  **Inferred.** `mdir_xlate_addr_sdir` is a 224-byte third variant that reads the same hub word 0x9200000001200018 (0x30a7c) and
  the word at 0x9000000010000090 (0x30aa0), and branches on that second word at 0x30abc. It calls `sprintf` four
  times (0x30ad8, 0x30b18, 0x30b2c, 0x30b40) with the same format string and byte-sized constants, and the sign of
  its input (0x30ac8, 0x30b0c) picks the variant. Like its siblings it returns the `sprintf` result with no lookup
  in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x34330
  **Inferred.** `_ecc_errorvec` is an 88-byte routine that stores 8
  bytes to low-memory offsets 0x548, 0x550 and 0x480,
  which the disassembly labels `br_isr_errs+200`, `+208` and the base. It reads
  the doubleword at 0xfd8, labeled
  `toidlestk+24`, branches on it (0x34364), and ends in an indirect jump at
  0x34378 whose target the disassembly does not
  resolve, forwarding its four argument registers. Role (from name): the name suggests an ECC
  error exception vector, and the disassembly
  shows it records error words and then dispatches elsewhere.
  Reviewed: 2026-11-12 (sonnet)

## Rejected

None.
