# Routine role statements, tier-1 batch 41

These are role statements for 25 routines of the IP35 boot kernel object in 6.5.30. They are the 41st group of 25 in the ranked routine list (list indices 1025 to 1049). Each statement rests on the disassembly for its routine: registers, calls, branches and constants. Where the disassembly stops short of a role, the entry says so. Every role is inferred, because no routine's disassembly establishes its role outright.

- `intpend0` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x200f0
  **Inferred.** It loops over a 72-byte table entry selected by `ms1bit` (call at 0x20168), with the same index arithmetic as `intpend1`. It takes a semaphore pair with `cvsema` (0x20140), `valusema` (0x201bc) and `vsema` (0x201dc), and it compares the `valusema` result with `hub_intr_wakeup_cnt` (branch at 0x201d4). It calls the handler at entry +0x10 through `jalr` at 0x201ac and 0x20240. The role is most likely the wakeup side of the same dispatch that `intpend1` does. The disassembly does not show which pending bits it masks.

- `intpend1` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20250
  **Inferred.** It loops while the word at -0x3a58 shares a bit with the hub word read at 0x92000000010000a0 (branch at 0x20284). Each pass calls `ms1bit` at 0x2029c, then indexes a 72-byte table entry by that bit. If the entry's first word is zero it calls `cmn_err` at 0x2032c, and otherwise it calls the handler at entry +0x10 through `jalr` at 0x202d4. The role is most likely pending-interrupt dispatch. The source of the mask and the meaning of the hub register are not established by this disassembly.

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x206a4
  **Inferred.** It takes a CPU index in a0 and reads that CPU's record through `pdaindr` (index shifted left by 4). It takes a byte at record +0x528 and a halfword at record +0x42c. The halfword is shifted left by 33 and added to one of two hub bases, 0x9200000001800090 or 0x9200000001a00090, chosen by whether a1 equals 0xab (branch at 0x206dc). It returns 0. The role is most likely sending an interrupt to another CPU through a hub store. What a1 encodes and what value is stored are not shown.

- `cpuintr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20710
  **Inferred.** It reads the halfword at 0xffffffffffffc428 and the per-CPU record, as the routines around it do. It copies 0x168 (360) bytes from a1 with `bcopy` (0x207b0), then calls `doacvec` (0x207b8) and `doactions` (0x207c0). It logs trace events 0x71 and 0x65 through `log_tstamp_event` (0x207f0 and 0x20810), and a flag word at -0x3d80 (bit 0x20) picks between two paths. The role is most likely a CPU-side interrupt entry that saves a block of state and runs the vector and action tables. What the 360-byte block holds, and where a1 comes from, are not in the disassembly.

- `dbgintr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20884
  **Inferred.** It reads the halfword at 0xffffffffffffc428 and the per-CPU byte at record +0x528. It stores to 0x9200000001000090, reads 0x9200000001000098, and calls `debug` at 0x208c0 with the same .rodata message that `intpend1` passes to `cmn_err`. It returns what `debug` returns. The role is most likely a debugger entry that hands off to `debug`. The message text and the meaning of the two hub words are not shown.

- `chkdebug` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20bb4
  **Inferred.** It reads the same halfword and per-CPU byte as `dbgintr`. It tests the hub word at 0x92000000010000a0, the word that `intpend1` masks against, with a value in v0. When the bit test is nonzero it calls `dbgintr` with a0=0 (0x20bf4), and otherwise it returns the unresolved v0. The role is most likely a gate that enters `dbgintr` only when a debug-enable bit is set. The producer of the v0 value is not in the disassembly.

- `debug_stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20c74
  **Inferred.** It loops over CPU indices from 0 up to `maxcpus`, and it skips the loop when that value is zero or less (branch at 0x20c90). Each index goes through `get_cpu_slice` (0x20ca4) and `cpu_enabled` (0x20d14). The index equal to the halfword at -0x3bd8 is skipped (branch at 0x20d08), which is most likely the calling CPU. The role is most likely stopping every other enabled CPU for a debugger. The stop action itself is not resolved, because its store goes to an address the disassembly leaves unknown (0x20cec). This is the weakest statement in the batch.

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x24748
  **Inferred.** It writes a 32-bit value to 0xffffffffffffc51c and reads it back. It computes a node address from `get_slice` and `get_nasid`, and it branches on the result of `get_chimera_type` compared with 3. It gets a controller handle through `get_elsc` on a window at 0x9000000030000000 plus a node offset, then calls `elsc_led_set` with codes 0x101, 0x102, 0x200 and 0x201. Some paths call `elsc_display_mesg` with a .rodata string, and the global `nmied` gates some of them. The role is most likely driving the node's front-panel LEDs and message display through that controller. What each code means and what `nmied` is are not shown.

- `bootstrap` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x252e8
  **Inferred.** It reads a doubleword from the hub address 0x9200000001600000 and loads two halfwords from addresses the disassembly leaves unresolved. It calls `mapped_kernel_setup_tlb` (0x2533c) with the incoming a2 and a3. It ends with an indirect jump at 0x2535c through a relocated address (`.text+152420`), and the instructions from 0x25364 to 0x253b4 are not reached from the entry. The role is most likely early kernel setup that maps the kernel through the TLB and then passes control on. Most operands are unresolved, so this is a partial reading.

- `counter_intr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x272f8
  **Inferred.** It is the largest routine in the batch (824 bytes). It reads counter state through `_get_count` and `_get_compare`, and it calls `frs_handle_counter_intr` (0x27320), `hwperf_overflowed_intr` (0x27348), `l2_sbe_intr` (0x27364), `hwperf_intr` (0x2737c) and `clock` (0x274b0). It resets the counter with `resetcounter` (0x2739c and 0x274a8) and compares the `lbolt` global with 0x3e9. It also calls `cmn_err` at 0x27610 on a path the disassembly does not fully resolve. The role is most likely the count-and-compare timer interrupt, which fans out to the performance counters, the L2 single-bit error check and the clock tick. The reset rule and the warning conditions are only partly resolved.

- `doadjtime` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x28b8c
  **Inferred.** It takes the `timelock` spin lock with `nested_spinlock` (0x28c2c and 0x28dc4) around `__nanotime` calls (0x28c1c and 0x28db8). It raises priority with `splprof` (0x28c14 and 0x28db0), and releases the lock with `mutex_spinunlock` (0x28cd4 and 0x28e24). It compares a value with 1000 (0x3e8) and with 0xf4240 (1,000,000), and it reads the hub word at 0x9200000001604200 in three places. The role is most likely the adjtime routine that slews the time of day toward a pending delta. The two constants suggest microsecond units, but the disassembly does not confirm that, and the source of the delta is not shown.

- `__nanotime` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x28e3c
  **Inferred.** It takes `timelock` with `mutex_spinlock_spl` (0x28e48) and reads the hub counter at 0x9200000001030100. It reads and writes two `.bss` words at +0xa830 and +0xa838, which look like a cached reading pair. It adds `adjtime_delta` and `adjtime_RTC`, normalizes against 0x3b9aca00 (1,000,000,000), and releases the lock with `mutex_spinunlock` (0x29034 and 0x290c4). The role is most likely returning the current time in nanoseconds, adjusted by the pending adjtime delta. The cached-pair reading comes only from the loads and stores.

- `findcpufreq_cycles` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2973c
  **Inferred.** It raises priority with `splhi` (0x29754) and resets the count through `_set_count` with 0 (0x2976c). It then reads the hub counter at 0x9200000001030100 in a loop, comparing each read with a threshold in a3, and it leaves the loop when a read reaches that threshold. After the loop it reads `_get_count` (0x2980c), passes the result to `cpu_cycles_adjust` (0x29814), and restores priority with `splx`. The role is most likely calibrating the CPU cycle count against the hub's real-time counter. The unit of the result, and what `cpu_cycles_adjust` does with it, are not shown.

- `acktmoclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29f84
  **Inferred.** It reads the halfword at 0xffffffffffffc428 and the per-CPU record, as `sendintr` does. It tests bit 0 of the record byte at +0x528 (branch at 0x29fa4). Its two stores, to 0x9200000001000128 and 0x9200000001000120, sit on paths this disassembly does not fully separate. The role is most likely acknowledging the `tmoclock` interrupt for this CPU, gated on that per-CPU bit. The meaning of the two hub words is not shown.

- `ackkgclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29fc0
  **Inferred.** It has the same shape as `acktmoclock`. It reads the halfword at 0xffffffffffffc428 and the per-CPU record, tests bit 0 of the byte at +0x528 (branch at 0x29fe0), and writes 0x9200000001000138 and 0x9200000001000130. The role is most likely the matching acknowledgement for the `kgclock` tick that `startkgclock` and `slowkgclock` use. The meaning of the hub words is not shown.

- `startkgclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x29ffc
  **Inferred.** It takes the lock at `.sbss`+192 with `mutex_spinlock_spl` (0x2a008) and releases it with `mutex_spinunlock` (0x2a184 and 0x2a1b4). It writes the hub words 0x9200000001000158, 0x9200000001000118 and 0x9200000001000150, reads the hub counter at 0x9200000001030100, and calls `cnodetocpu` (0x2a104). When a `.sdata` word is zero it calls `setkgvector` with `prof_intr` (0x2a1c4). The role is most likely starting the periodic `kgclock` tick on this CPU and registering `prof_intr` as its handler. The programmed values and the timing are not established by this disassembly.

- `slowkgclock` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2a1dc
  **Inferred.** It takes the same lock at `.sbss`+192 with `mutex_spinlock_spl` (0x2a1f0). It updates the hub word 0x9200000001000158 and the word at [-0x3bf8]+0x3c58 that `startkgclock` also uses, and it calls `ackkgclock` (0x2a284). It compares `master_procid` with the CPU halfword at -0x3bd8 (0x2a20c) and tests the `audioclock` global (0x2a214). It calls `spinlock_init` on the lock (0x2a2a4) when a `.sdata` word is zero. The role is most likely the slower-rate companion of `startkgclock`, with part of its work limited to the master processor. The `audioclock` test and the master-only reading are inferred from the comparisons.

- `prof_intr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2ac80
  **Inferred.** It calls `fastick_maint` (0x2ac98) and `cnode_subnode_to_cpu` (0x2ad28). It reads the hub counter at 0x9200000001030100 and the per-CPU record, writes the hub word 0x9200000001000118, and passes 0x3b9aca00 (1,000,000,000) to `cnode_subnode_to_cpu` as an argument. It logs trace event 0x6e (0x2ae20) and trace event 0x65 with argument 0x6e (0x2ae40), under the same flag bit (0x20 in -0x3d80) that `cpuintr` tests. The role is most likely the profiling tick for this CPU, which runs its maintenance and writes trace records. The meaning of the `fastick` work and of the counters is not in the disassembly.

- `mdir_xlate_addr_mem` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2b86c
  **Inferred.** It reads the doubleword at 0x9200000001780008 and formats text with `sprintf`, using the .rodata format at +47160. It calls `sprintf` from 0x2b8e0 when a2 is non-negative and from 0x2b900 when a2 is negative (branch at 0x2b8a8). It returns the `sprintf` result. The role is most likely formatting a memory-directory address for a caller, for a message or a log. The translation arithmetic is not visible in this disassembly.

- `mdir_xlate_addr_pdir` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2b910
  **Inferred.** It reads the same doubleword at 0x9200000001780008 and calls `sprintf` with the same format from 0x2b960 and 0x2b980. It branches on the sign of a2 (0x2b948) as the `mem` variant does, and it forwards a3 to its first call. The role is most likely the same formatting helper for the pdir case. The translation itself is not visible in this disassembly.

- `mdir_xlate_addr_sdir` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2b990
  **Inferred.** It reads 0x9200000001780008 and makes one `sprintf` call at 0x2b9cc with the same format, passing the constant 0x1ffffffff as a4. It has no sign branch in this disassembly. The role is most likely the sdir form of the same formatting helper. The meaning of the constant is not shown.

- `_ecc_errorvec` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2e2c0
  **Inferred.** It stores doublewords to the low addresses 0x4c8, 0x4d0 and 0x400, and loads 0xfd8 into k1. It branches on that value (0x2e2f4) and ends with an indirect jump at 0x2e308 that passes its arguments through, to a target the disassembly does not resolve. The disassembly attaches symbol names (`arcs_to_dsk`, `resumeidle`) to those small addresses, and this statement does not rely on those names. The role is most likely an ECC error entry that saves state and hands off through a pointer kept at 0xfd8. The target and the saved values are not shown.

- `earlybadaddr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x3773c
  **Inferred.** It stores to `nofault` twice (0x3776c and 0x37804), most likely arming and then disarming a recovery hook. It reads one byte, halfword or word at a0 according to a1 (1, 2 or 4; branches at 0x37774 and 0x3778c). A branch at 0x377e4 jumps to the address of `earlybaerror` (0x378b8) when a bit in t1 is set, and the routine returns 0. The role is most likely an early probe that tests whether an address faults, using `nofault` as the recovery hook. The cmn_err call at 0x377d0 is on a path the disassembly does not fully resolve, so the failure report is not confirmed.

- `baerror` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x37888
  **Inferred.** It loads a doubleword from 0xffffffffffffc020 and tests a pointer at -0x3fe0 (branch at 0x37890). When the pointer is nonzero it stores a halfword to the pointer's +0xf6 field. When it is zero it stores a word to 0xffffffffffffc48c, and it returns 1. The role is most likely a bus-error hook that records the error and reports it as handled. The stored values and the meaning of the pointer are not shown.

- `earlybaerror` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x378b8
  **Inferred.** It stores a word to `nofault` (at 0x378d8) and returns 1. The stored value is not in this disassembly. The role is most likely the early bus-error handler that marks the probe in `earlybadaddr` as faulted. That link is inferred from the shared `nofault` word and the branch at 0x377e4.
