# Routine role statements, tier-1 batch 41

These are role statements for 25 routines of the IP35 boot kernel object in 6.5.30. They are the 41st group of 25 in the ranked routine list (list indices 1025 to 1049). Each statement rests on the disassembly for its routine: registers, calls, branches and constants. Where the disassembly stops short of a role, the entry says so. Every role is inferred, because no routine's disassembly establishes its role outright.

- `intpend0` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x200f0
  **Inferred.** It loops over a 72-byte table entry selected by `ms1bit` (call at 0x20168), with the same index arithmetic as `intpend1`. It calls `cvsema` (0x20140), `valusema` (0x201bc) and `vsema` (0x201dc) on the address entry +32, and it compares the `valusema` result with `hub_intr_wakeup_cnt` (branch at 0x201d4). It makes two indirect calls through `jalr` (0x201ac and 0x20240) whose callee the facts leave unresolved. Both pass the doubleword at entry +0x10 as a0, and the call at 0x201ac runs only when the doubleword at entry +0x8 is nonzero (branch at 0x201a0). Which one runs depends on bit 0x1000 of the word at entry +0x18 (branch at 0x20194). The role is most likely the wakeup side of the same dispatch that `intpend1` does. The disassembly does not show which pending bits it masks.
  Reviewed: 2026-11-12 (sonnet)

- `intpend1` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20250
  **Inferred.** It loops while the doubleword at -0x3a58 shares a bit with the hub doubleword read at 0x92000000010000a0 (branch at 0x20284). Each pass calls `ms1bit` at 0x2029c, then indexes a 72-byte table entry by that bit. If the entry's first doubleword is zero it calls `cmn_err` at 0x2032c. Otherwise it makes an indirect call through `jalr` at 0x202d4, whose callee the facts leave unresolved, and passes the doubleword at entry +0x10 as a0. The role is most likely pending-interrupt dispatch. The source of the mask and the meaning of the hub register are not established by this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x206a4
  **Inferred.** It loads a doubleword at +0x8 from a base the facts do not resolve, then a byte at +0x528 (0x206c0) and a halfword at +0x42c (0x206d8). It compares a1 with 0xab (branch at 0x206dc), makes one doubleword store at 0x20700 to an address the facts do not show, and returns 0. Role (from name): sending an interrupt to another CPU. The base of the loads, the store target, the stored value and the meaning of 0xab are not shown by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpuintr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20710
  **Inferred.** It loads the per-CPU record (doubleword at +0x8 and byte at +0x528) and stores a doubleword at 0x20754. When a doubleword at -0x3d80 has bit 0x20 set it logs trace event 0x71 through `log_tstamp_event` (0x207f0) before that. The `bcopy` call at 0x207b0 copies 0x168 (360) bytes from a1, but it is skipped when the byte at -0x3f80 is zero (branch at 0x20760) or when `nmied` is zero (branch at 0x20768). `doacvec` (0x207b8) and then `doactions` (0x207c0) follow, with no branch between them and the skipped `bcopy`. At the end, when the same bit 0x20 is set, it logs event 0x65 (a0=0x65, a1=0x71) at 0x20810. The role is most likely a CPU-side interrupt entry that saves a block of state and runs the vector and action tables. What the 360-byte block holds, and where a1 comes from, are not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `dbgintr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20884
  **Inferred.** It loads a halfword, then a doubleword at +0x8 and a byte at +0x528 of a per-CPU style entry, makes a doubleword store at 0x208b8 to an address the facts do not show, and calls `debug` at 0x208c0 with a0 = .rodata+43384 (the same offset `intpend1` passes to `cmn_err`), a2 = 0x9200000001000090 and a3 passed through. It returns `debug`'s result. Role (from name): a debugger entry that hands off to `debug`. The facts do not show the message text, what `debug` does with a2, or the stored value.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20bb4
  **Inferred.** It loads the same halfword, per-CPU doubleword and byte as `dbgintr`. It ANDs the doubleword at 0x92000000010000a0, the one `intpend1` masks against, with a value in v0 that the facts leave unresolved (branch at 0x20bec). When the result is nonzero it calls `dbgintr` with a0=0 (0x20bf4) and passes a1 to a3 through. Otherwise it skips the call, and the return is `dbgintr`'s result or an unresolved value. Role (from name): a check that enters `dbgintr` only when a debug condition holds. The facts do not establish what the v0 mask selects.
  Reviewed: 2026-11-12 (sonnet)

- `debug_stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20c74
  **Inferred.** It skips its loop when `maxcpus` is zero or less (branch at 0x20c90). Otherwise it loops over CPU indices up to `maxcpus`, calling `get_cpu_slice` (0x20ca4) for each. An index equal to the halfword at -0x3bd8 skips the rest of the pass (branch at 0x20d08); the facts do not show what that halfword holds. For the other indices it calls `cpu_enabled` (0x20d14). It makes a store at 0x20cec to an address the facts leave unknown. Role (from name): stopping the other CPUs for a debugger. No stop request is visible among the calls, so the role rests on the name.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x24748
  **Inferred.** It writes a 32-bit value to 0xffffffffffffc51c and reads it back. It computes a node address from `get_slice` and `get_nasid`, and it branches on the result of `get_chimera_type` compared with 3. It gets a controller handle through `get_elsc` on a window at 0x9000000030000000 plus a node offset, then calls `elsc_led_set` with codes 0x101, 0x102, 0x200 and 0x201. Some paths call `elsc_display_mesg` with a .rodata string, and the global `nmied` gates some of them. The role is most likely driving the node's front-panel LEDs and message display through that controller. What each code means and what `nmied` is are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x252e8
  **Inferred.** It loads a doubleword at 0x25314 and loads two halfwords at 0x25334 and 0x25338 through a pointer the facts leave unresolved. It calls `mapped_kernel_setup_tlb` (0x2533c) with the incoming a2 and a3, and then ends with an indirect jump at 0x2535c to the relocated address `.text+152420`. Most operands are unresolved, so this is a partial reading. The role is most likely early kernel setup that maps the kernel through the TLB and then passes control on.
  Reviewed: 2026-11-12 (sonnet)

- `counter_intr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x272f8
  **Inferred.** It is an 824-byte routine. When the doubleword at -0x3d80 has bit 0x20 set it logs trace events 0x6c (0x2753c) and 0x65 (0x27558) through `log_tstamp_event`. It calls `frs_handle_counter_intr` (0x27320) only when the word at -0x3958 is nonzero (branch at 0x27318). It calls `hwperf_overflowed_intr` (0x27348) and then `l2_sbe_intr` (0x27364) and `hwperf_intr` (0x2737c) according to the result and to `l2_sbe_check`. It calls `resetcounter` at 0x2739c and 0x274a8, reads `_get_count` (0x274c0) and `_get_compare` (0x274c8), compares `lbolt` with 0x3e9 (branch at 0x273d0), and calls `clock` (0x274b0). It also calls `cmn_err` at 0x27610 on a path the disassembly does not fully resolve. The role is most likely the count-and-compare timer interrupt, which fans out to the performance counters, the L2 single-bit error check and the clock tick. Which paths reach each call, the reset rule and the warning conditions are only partly resolved.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x28b8c
  **Inferred.** It branches on a0 being zero (0x28bb8) and on its sign (0x28bc4). On each path it calls `splprof` (0x28c14 and 0x28db0), then `__nanotime` (0x28c1c and 0x28db8), and only then `nested_spinlock` on `timelock` (0x28c2c and 0x28dc4). It releases the lock with `mutex_spinunlock` (0x28cd4 and 0x28e24). It compares s0 with 1000 (0x3e8, branch at 0x28bd0) and with 0xf4240 (1,000,000, branch at 0x28d00), and it makes three loads at 0x28bd8, 0x28d08 and 0x28d50 whose addresses the facts do not show. Role (from name): the adjtime routine that slews the time of day toward a pending delta. The two constants suggest microsecond units, but the facts do not confirm that, and the source of the delta is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x28e3c
  **Inferred.** It takes `timelock` with `mutex_spinlock_spl` (0x28e48) and reads the hub counter at 0x9200000001030100. It compares that counter (unsigned, branch at 0x28ee8) with the sum of `adjtime_delta` and `adjtime_RTC`, and tests `adjtime_nanos` (branch at 0x28f20). It loads and stores two words at offsets +0xa830 (32-bit) and +0xa838 (64-bit) from a base the facts do not name. It compares values against 0x3b9aca00 (1,000,000,000, branches at 0x28f94 and 0x28fec) and releases the lock with `mutex_spinunlock` (0x29034 and 0x290c4). One return shows v0 as the result of `mutex_spinunlock` (0x29040). Role (from name): returning the current time in nanoseconds, adjusted by the pending adjtime delta. The facts do not show how the result is built.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2973c
  **Inferred.** It loads the word at -0x38b4 and, when it is nonzero, returns it (branch at 0x29744, return at 0x2974c). When it is zero it raises priority with `splhi` (0x29754) and resets the count through `_set_count` with 0 (0x2976c). It then reads the hub counter at 0x9200000001030100 in an unrolled loop, comparing each read with a3 (unsigned), and leaves the loop when a read reaches a3. After the loop it calls `_get_count` (0x2980c) and passes the result to `cpu_cycles_adjust` (0x29814). It makes a 32-bit store at 0x29820 and restores priority with `splx` (0x29824). The role is most likely calibrating the CPU cycle count against the hub's real-time counter. The unit of the result, the origin of a3 and what `cpu_cycles_adjust` does with the count are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `acktmoclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29f84
  **Inferred.** It loads the halfword at 0xffffffffffffc428 and the per-CPU record, tests bit 0 of the record byte at +0x528 (branch at 0x29fa4), and makes one doubleword store on each side of that branch: at 0x29fac when the bit is set and at 0x29fbc when it is clear, so only one store runs. The stores go to the hub words 0x9200000001000128 and 0x9200000001000120, and the stored value is not in the facts. It returns the halfword shifted left 4. Role (from name): acknowledging the `tmoclock` interrupt for this CPU. The meaning of the two hub words is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29fc0
  **Inferred.** It has the same shape as `acktmoclock`. It loads the halfword at 0xffffffffffffc428 and the per-CPU record, tests bit 0 of the byte at +0x528 (branch at 0x29fe0), and makes one doubleword store on one side of the branch (0x29fe8 when the bit is set, 0x29ff8 when it is clear), so only one store runs. The stores go to 0x9200000001000138 and 0x9200000001000130. Role (from name): the acknowledgement for the `kgclock` tick that `startkgclock` and `slowkgclock` use. The meaning of the hub words and the stored value are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29ffc
  **Inferred.** It takes the lock at `.sbss`+192 with `mutex_spinlock_spl` (0x2a008) and releases it with `mutex_spinunlock` (0x2a184 and 0x2a1b4). When the `.sdata` word at +368 is zero (branch at 0x2a020) it skips to the `setkgvector` call with a0 = `prof_intr` (0x2a1c4). Otherwise it makes several loads and doubleword stores (including 0x2a15c and 0x2a170) whose addresses the facts do not show, and it calls `cnodetocpu` (0x2a104) only when the word at [[-0x3bf8]+15448] is not 1 (branch at 0x2a0fc). The facts show the address of `prof_intr` passed to `setkgvector`, not the install itself. Role (from name): starting the periodic `kgclock` tick on this CPU. The programmed values and the timing are not established by this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2a1dc
  **Inferred.** It branches on the `.sdata` word at +376 (0x2a1e4). When it is zero it stores a doubleword and calls `spinlock_init` on the lock at `.sbss`+192 (0x2a2a4) without taking it. Otherwise it takes that lock with `mutex_spinlock_spl` (0x2a1f0), compares `master_procid` with the CPU halfword at -0x3bd8 (0x2a20c) and tests the `audioclock` global (0x2a214). It reaches `mutex_spinunlock` at 0x2a220 or 0x2a27c depending on those tests. It updates the word at [-0x3bf8]+0x3c58 (store at 0x2a270, the same field `startkgclock` uses) and calls `ackkgclock` (0x2a284) on one path. It also makes doubleword stores whose addresses the facts do not show. Role (from name): the slower-rate companion of `startkgclock`, with part of its work limited to the master processor. The master-only reading is inferred from the comparison, and the facts do not show which path runs when.
  Reviewed: 2026-11-12 (sonnet)

- `prof_intr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2ac80
  **Inferred.** When bit 0x20 of the doubleword at -0x3d80 is clear it calls `fastick_maint` (0x2ac98). On a later branch it calls `cnode_subnode_to_cpu` (0x2ad28) with a3 = 0x9200000001604200 and a4 = 0x3b9aca00, both arguments rather than reads. It compares the hub counter at 0x9200000001030100 with a value loaded from [[-0x3bf8]+15440] (branch at 0x2ada0), and it makes doubleword stores (0x2ad90, 0x2ade0, 0x2adf4) whose addresses the facts do not show. When the same bit 0x20 is set it logs trace event 0x6e (0x2ae20) and event 0x65 (0x2ae40, a0=0x65 and a1=0x6e). Role (from name): the profiling tick for this CPU. The facts do not say what the `fastick` work or the counters mean.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_mem` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2b86c
  **Inferred.** It makes one doubleword load at 0x2b870 whose address the facts do not show. It branches on the sign of a2 (0x2b8a8) and calls `sprintf` at 0x2b8e0 when a2 is non-negative and at 0x2b900 when it is negative. Both calls take a0 from the caller and show a1 as .rodata+47160 in the facts, and the first passes a4 = a2. It returns the result of `sprintf`. Role (from name): translating a memory-directory address, here only formatted into a buffer. The translation arithmetic and the format text are not visible in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_pdir` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2b910
  **Inferred.** It makes one doubleword load at 0x2b914 whose address the facts do not show, and calls `sprintf` at 0x2b960 or 0x2b980 depending on the sign of a2 (branch at 0x2b948), as `mdir_xlate_addr_mem` does. The first call passes a3 through, and both show a1 as .rodata+47160 in the facts. It returns the result of `sprintf`. Role (from name): the pdir counterpart of the formatting helper. The translation itself and the format text are not visible in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_sdir` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2b990
  **Inferred.** It makes one doubleword load at 0x2b994 whose address the facts do not show, then one `sprintf` call at 0x2b9cc with a1 as .rodata+47160 in the facts and a4 = 0x1ffffffff, and returns the result of `sprintf`. The facts show no sign branch. Role (from name): the sdir counterpart of the formatting helper. The meaning of the constant is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2e2c0
  **Inferred.** It makes three doubleword stores at 0x2e2c0, 0x2e2c4 and 0x2e2c8, loads a doubleword at 0xfd8 into k1 (0x2e2f0), and branches on k1 being nonzero (0x2e2f4). It ends with an indirect tail call at 0x2e308 that passes a0 to a3 through, to a callee the facts do not resolve. Role (from name): an ECC error vector stub that saves state and hands off. The jump target, the stored values and the use of the word at 0xfd8 are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3773c
  **Inferred.** It stores to `nofault` twice (0x3776c and 0x37804), most likely arming and then disarming a recovery hook. It reads one byte, halfword or word at a0 according to a1 (1, 2 or 4; branches at 0x37774 and 0x3778c). A branch at 0x377e4 jumps to the address of `earlybaerror` (0x378b8) when a bit in t1 is set, and the routine returns 0. The role is most likely an early probe that tests whether an address faults, using `nofault` as the recovery hook. The cmn_err call at 0x377d0 is on a path the disassembly does not fully resolve, so the failure report is not confirmed.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x37888
  **Inferred.** It loads a doubleword from 0xffffffffffffc020 and tests a pointer at -0x3fe0 (branch at 0x37890). When the pointer is nonzero it stores a halfword to the pointer's +0xf6 field. When it is zero it stores a word to 0xffffffffffffc48c, and it returns 1. The role is most likely a bus-error hook that records the error and reports it as handled. The stored values and the meaning of the pointer are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x378b8
  **Inferred.** It stores a word to `nofault` (at 0x378d8) and returns 1. The stored value is not in this disassembly. The role is most likely the early bus-error handler that marks the probe in `earlybadaddr` as faulted. That link is inferred from the shared `nofault` word and the branch at 0x377e4.
  Reviewed: 2026-11-12 (sonnet)
