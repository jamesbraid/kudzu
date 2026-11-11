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
  Each pass also calls `ms1bit` (0x1ff84) and makes an indirect call at 0x1ffc8 whose target the disassembly does
  not resolve. The shape fits draining pending interrupts for one level, but the meaning of the masks is
  not in the disassembly.

- `intpend1` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2006c
  **Inferred.** `intpend1` is 236 bytes and writes 0x9200000001000090 at 0x200c4, the same target as `intpend0`. It masks
  its pending word 0x92000000010000a0 against a global at 0x200a0, reads 0x9200000001000098 at 0x200e8, and
  calls `ms1bit` (0x200b8). Unlike `intpend0`, it calls no semaphore helpers, and it reaches a `cmn_err` call
  at 0x20148 with a format string at .rodata+32520 and makes an unresolved indirect call at 0x200f0. The
  disassembly does not say whether that `cmn_err` call is an error path.

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x204c0
  **Inferred.** `sendintr` is a 72-byte leaf that indexes the per-CPU table `pdaindr` by its first argument, using 16-byte
  entries, and follows the record's pointer at +8 to a second record. From there it reads a halfword at
  +0x424 and a byte at +0x520, compares its second argument with 0xab at 0x204dc, and stores a 64-bit value
  built from the halfword to offset +1060 at 0x204fc. It returns 0, and the disassembly shows no callers, so
  the purpose of the 0xab test and the stored value is open.

- `cpuintr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20508
  **Inferred.** `cpuintr` is a 276-byte handler that finds this CPU's record through `pdaindr` and the halfword at
  [-0x3be0]. It reads hub word 0x9200000001000098 and writes 0x9200000001000090, the same pair the
  pending-interrupt routines use, and a flag bit 0x20 in the word at [-0x3d80] gates two paths (0x2051c,
  0x205cc). It calls `bcopy` (0x205ac, length 0x168), `doacvec` (0x205b4), `doactions` (0x205bc), and two
  `log_tstamp_event` calls with event codes 0x71 and 0x65, the second passing 0x71 as its argument. That pair
  looks like an entry and exit marker, but the disassembly does not establish it.

- `dbgintr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2067c
  **Inferred.** `dbgintr` is a 76-byte routine that reads the halfword at abs 0xffffffffffffc420, the record byte at +0x520
  and hub word 0x9200000001000098, then writes 0x9200000001000090. It makes one call, to `debug` at 0x206b4,
  with the string at .rodata+32520, a byte from the record at +1312 plus 118, the hub address
  0x9200000001000090, and its own fourth argument. The disassembly does not show what that byte selects, so
  the routine's role as a debugger entry from the interrupt side is only the name's suggestion.

- `chkdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20974
  **Inferred.** `chkdebug` is an 80-byte gate that reads the halfword at abs 0xffffffffffffc420, the record's byte at +0x520
  and hub word 0x92000000010000a0. It masks that word with a register value and branches at 0x209a8, and the
  call to `dbgintr` at 0x209b0 sits on the nonzero path, receiving 0 as its first argument and forwarding its
  own second through fourth arguments. The masked word is the one `intpend1` reads, which fits a check for a
  pending debug request, but the disassembly does not show that link.

- `debug_stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20a98
  **Inferred.** `debug_stop_all_cpus` is a 236-byte loop over CPU indices from 0 up to the global `maxcpus`. Each index
  calls `get_cpu_slice` (0x20ac8) and `cpu_enabled` (0x20b14), disabled entries are skipped at 0x20b28, and a
  branch at 0x20b08 compares the index with the halfword at [-0x3be0], which fits skipping the calling CPU
  though the disassembly does not say so. The routine also stores 8 bytes to hub block 0x9200000001800090 at
  0x20aec, a page the interrupt words above do not use, and the disassembly has no interrupt-send call, so the
  stop mechanism is not visible despite the name.

- `get_my_slotid` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x23be4
  **Inferred.** `get_my_slotid` is a 36-byte wrapper that loads doubleword 0x9200000001220048, a hub register on a
  different page from the interrupt words in this set. It passes that value and its incoming argument
  registers to `hub_slotbits_to_slot` (0x23bf4) and returns the helper's result. The name suggests the slot
  number for this CPU, but the disassembly shows a conversion and not the bit fields it extracts.

- `llp_pfail_intr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x252a4
  **Inferred.** `llp_pfail_intr` is the largest routine in this set at 792 bytes, and it handles a link power-fail event.
  It reads hub word 0x9200000001608088 six times, testing the single bit 0x2000000000 on each read (0x253a8 to
  0x253e8), and calls `msc_owner_cpu` through a pointer at 0x24fd0 (0x25304) to find an owner CPU. It then
  makes a run of `vector_read` and `vector_write` calls (0x2532c to 0x255a0) with a2 set to 0x0, 0x30, 0x600110
  or 0x8, reads and writes 0x9200000001600110, writes 0x9200000001600008 at 0x25498, and waits 100 or 300
  microseconds (`us_delay`) between the calls, choosing a path by comparing a global at sbss+160 with 8. The
  disassembly gives the order of reads, writes and waits, but not what the vector values or the flag bit mean.

- `force_system_shutdown` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x26ad4
  **Inferred.** `force_system_shutdown` is a 100-byte routine that writes 0x9200000001600008, the same port word
  `llp_pfail_intr` writes at 0x25498, in the delay slot of the back branch at 0x26b30, so it runs on each pass
  of that loop. It calls `cmn_err` at level 4 with a string at .rodata+34712, `us_delay` with 0x7a120 and
  0xf4240 (500000 and 1000000), `elsc_system_reset` with a pointer into .bss+37112, and `printf` with the same
  string, and a negative return from `elsc_system_reset` takes a separate branch (0x26b00). The disassembly shows
  the return at 0x26b08 and the loop but not how the two paths relate. The name suggests a forced power-off,
  and these calls are the closest evidence for it.

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2a510
  **Inferred.** `set_leds` is a 124-byte routine that reads the byte at abs 0xffffffffffffc523, writes a word to abs
  0xffffffffffffc514 and reads it back. It calls `get_slice` and `get_nasid` on one of two paths chosen by a
  byte at [-0x3add] (0x2a51c), and the store at 0x2a55c writes 8 bytes to 0x9200000001a20050 or
  0x9200000001a20080 plus a term built from those two results, returning the `get_nasid` result. The name
  suggests front-panel LEDs, but the disassembly shows a store to a hub address picked by slice and node, not a bit
  pattern.

- `bootstrap` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2a5b8
  **Inferred.** `bootstrap` is a 208-byte early-boot routine that reads hub word 0x9200000001600000 and a doubleword and two
  halfwords at addresses the disassembly leaves unresolved. It calls `mapped_kernel_setup_tlb` (0x2a60c) with two
  incoming argument registers, then jumps through a register loaded for `jr` at 0x2a62c, which the relocation
  puts at .text+173620. The disassembly marks 0x2a634 to 0x2a684 as unreachable from the entry, and the name and
  the TLB setup call fit an early kernel entry, but the jump target is not identified.

- `counter_intr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2c620
  **Inferred.** `counter_intr` is a 728-byte counter interrupt handler that reads the flag word at [-0x3d80], as `cpuintr`
  does, plus the halfword at 0xffffc420 and the word at 0xffffc6a0. It calls `frs_handle_counter_intr`,
  `hwperf_overflowed_intr`, `l2_sbe_intr`, `hwperf_intr`, `resetcounter` (twice), `clock`, `_get_count` and
  `_get_compare`, and its second `resetcounter` call takes a5 from hub word 0x9200000001030100, the counter
  that `__nanotime` and `findcpufreq_cycles` read. A compare against 1001 (0x3e9) with `lbolt` (0x2c6f8) and a
  threshold in `counter_intr_warn_period` (0x2c704, 0x2c714) suggest a periodic warning, and two
  `log_tstamp_event` calls with codes 0x6c and 0x65 sit near the end, though the disassembly shows many paths
  but not what each branch means.

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2dfa8
  **Inferred.** `doadjtime` is a 492-byte routine that takes the `timelock` spinlock with `nested_spinlock` (0x2e004, 0x2e11c)
  inside a `splprof` bracket and reads the time with `__nanotime` (0x2dff4, 0x2e110). It uses the constants
  0xf4240 (1,000,000) and 0x3e8 (1,000) in compares and arithmetic, releases the lock with `mutex_spinunlock`
  at 0x2e0ac and 0x2e17c (the second passing 1000 in a4), and lists no direct memory stores. The name suggests
  a time-of-day adjustment step and the constants fit microsecond stepping, but the disassembly does not establish the
  adjustment rule.

- `__nanotime` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2e194
  **Inferred.** `__nanotime` is a 572-byte time read that takes `timelock` with `mutex_spinlock_spl` (0x2e1a0) inside
  `splprof` and reads hub counter 0x9200000001030100 (0x2e1bc). It compares that counter with the sum of the
  `adjtime_delta` and `adjtime_RTC` globals (0x2e1f0), reads `adjtime_nanos` (0x2e228), and normalizes seconds
  and nanoseconds against 0x3b9aca00 (10^9) in loops. It also reads and writes a pair of words the disassembly
  labels `sym(nasid_to_compact_node)+0x9898` and `+0x9890`, a label that looks misplaced, so the location of
  that cache is not identified, and it releases the lock with `mutex_spinunlock` at 0x2e340 and 0x2e3c0.

- `findcpufreq_cycles` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2e924
  **Inferred.** `findcpufreq_cycles` is a 212-byte routine that reads a word at abs 0xffffffffffffc744, writes it back at
  0x2e9e0, and runs inside a `splhi` bracket that clears the count register through `_set_count` with 0. It
  then spins from 0x2e974 to 0x2e9b4 until hub counter 0x9200000001030100 has advanced by 0x1312d0 (1,250,000)
  from its start value, and reads the count with `_get_count` and passes it to `cpu_cycles_adjust`. The loop is
  a timed wait, but the counter's tick rate is not in the disassembly, so the window's length in time is unknown.

- `acktmoclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f3d0
  **Inferred.** `acktmoclock` is a 56-byte acknowledgement that reads the halfword at abs 0xffffffffffffc420, then the per-CPU
  record through `pdaindr` and its byte at +0x520, and tests a byte at +1312 of a sub-record (0x2f3ec). Its two
  hub stores, to 0x9200000001000128 and 0x9200000001000120, sit on opposite sides of that test, so each path
  appears to write one word, and it returns the `pdaindr` offset. The name suggests it clears a timeout-clock
  interrupt, but the disassembly does not name the two hub words.

- `ackkgclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f408
  **Inferred.** `ackkgclock` has the same shape as `acktmoclock`: it reads the halfword at abs 0xffffffffffffc420 and the
  per-CPU record through `pdaindr` and its byte at +0x520, then gates on the byte at +1312 (0x2f424). It writes
  0x9200000001000138 and 0x9200000001000130 on opposite sides of that test and returns the `pdaindr` offset.
  The name suggests an acknowledgement for the kernel-global clock that `startkgclock` and `slowkgclock` drive,
  but the disassembly does not state that.

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f440
  **Inferred.** `startkgclock` is a 280-byte routine that takes the spinlock at sbss+288 with `mutex_spinlock_spl` (0x2f44c).
  A branch on the word at sdata+392 (0x2f45c) selects a path to the `setkgvector` call at 0x2f52c, which passes
  `prof_intr` (0x301e0) as its vector, and the routine also calls `cnodetocpu` (0x2f4c4) with the halfword at
  [-0x3be0] and a value read through [-0x3c00]+18736. It writes hub words 0x9200000001000158, 0x9200000001000118
  and 0x9200000001000150, reads the counter 0x9200000001030100 (0x2f4e0), and releases the lock twice with
  `mutex_spinunlock`. The disassembly does not say what the `cnodetocpu` result selects.

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f558
  **Inferred.** `slowkgclock` is a 216-byte routine that takes the same spinlock at sbss+288 with `mutex_spinlock_spl` (0x2f56c)
  and reads the halfword at 0xffffc420. It compares that halfword with `master_procid` (0x2f588), and on the
  master path it looks at the `audioclock` global (0x2f590), writing hub words 0x9200000001000158 and
  0x9200000001000150 (0x2f5c4, 0x2f614) and the word at 0xffffc534. It calls `ackkgclock` (0x2f5fc), calls
  `spinlock_init` (0x2f61c) with a string at .rodata+44008 on a path reached when the word at sdata+400 is zero
  (0x2f560), and releases the lock through `mutex_spinunlock` at 0x2f59c and 0x2f5f4. The name suggests a slower
  variant of the kernel-global clock tick, but the disassembly does not show the rate.

- `prof_intr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x301e0
  **Inferred.** `prof_intr` is a 252-byte profiling tick handler that reads the halfwords at 0xffffc420 and 0xffffc422, the word
  at 0xffffc534, the table at 0xffffc400 and the hub counter 0x9200000001030100. It writes 0x9200000001000118 and a
  doubleword in the record at [-0x3c00]+0x4928, and a loop compares a count in the record at +18728 against the
  hub counter. It calls `fastick_maint` (0x301f8) and `cnode_subnode_to_cpu` (0x30248, with 0, 1 and the halfword
  at [-0x3be0]), reaches `log_tstamp_event` with codes 0x6e and 0x65 (0x302b0, 0x302cc), and `startkgclock` passes
  it as its vector. The name suggests a profiling tick and `fastick_maint` suggests a fast-tick helper, but the
  disassembly confirms neither.

- `mdir_xlate_addr_mem` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x307f4
  **Inferred.** `mdir_xlate_addr_mem` is a 376-byte routine that reads hub word 0x9200000001200018 (0x30810) and the word at
  0x9000000010000090 (0x3081c), and branches on that second word at 0x30834. It makes four `sprintf` calls
  (0x30890, 0x3091c, 0x30934, 0x3095c), each with the format string at .rodata+44192 and byte-sized constants
  0x58, 0x59, 0x48, 0x4c and 0x24, and signed tests of the input (0x30858, 0x308dc) plus a compare against 0x36
  (0x308c0) pick the variant. The disassembly shows no table lookup and no translated address, so the routine may
  be a formatter for a memory-directory address rather than a translator, and the name claim is not established.
  Its return value is the `sprintf` result.

- `mdir_xlate_addr_pdir` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3096c
  **Inferred.** `mdir_xlate_addr_pdir` is a 260-byte sibling of the memory variant that reads only hub word 0x9200000001200018
  (0x30994). Its three `sprintf` calls (0x309f0, 0x30a14, 0x30a60) use the same format string at .rodata+44192 and
  byte-sized constants, and the sign of its input (0x309ac) and a zero test on a3 (0x309b4) choose the branch.
  Like the memory variant it returns the `sprintf` result, and the disassembly shows no lookup behind the name.

- `mdir_xlate_addr_sdir` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x30a70
  **Inferred.** `mdir_xlate_addr_sdir` is a 224-byte third variant that reads the same hub word 0x9200000001200018 (0x30a7c) and
  the word at 0x9000000010000090 (0x30aa0), and branches on that second word at 0x30abc. It calls `sprintf` four
  times (0x30ad8, 0x30b18, 0x30b2c, 0x30b40) with the same format string and byte-sized constants, and the sign of
  its input (0x30ac8, 0x30b0c) picks the variant. Like its siblings it returns the `sprintf` result with no lookup
  in the disassembly.

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x34330
  **Inferred.** `_ecc_errorvec` is an 88-byte exception vector that stores 8 bytes to low-memory offsets 0x548, 0x550 and 0x480,
  which the disassembly labels `br_isr_errs+200`, `+208` and the base. It reads the doubleword at 0xfd8, labeled
  `toidlestk+24`, branches on it (0x34364), and ends in an indirect jump at 0x34378 whose target the disassembly does not
  resolve, forwarding its four argument registers. The name suggests an ECC error exception vector, and the disassembly
  show it records error words and then dispatches elsewhere, with 0x34380 to 0x34384 marked unreachable from the entry.

## Rejected

None.
