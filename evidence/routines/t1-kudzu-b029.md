# Tier-1 routine readings, batch 29 (kudzu)

Role statements for the 25 routines at target indexes 725 to 749 of the ranked tier-1 hardware-linked list (`t1list`, located rows, `--hardware-only`). Each statement comes only from the instruction facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the inventory rows. Grades are inferred throughout, because the facts show what the code does, not what the author meant. Where a callee, register or constant is unresolved, the statement says so.

All 25 entries are in `6.5.30/unix.IP35`. None were left out.

- `stop_all_cpus` in `6.5.30/unix.IP35` at 0xc00000000005be70
  **Inferred.** Loads a 64-bit status word at 0x9200000001000020 and branches away (to 0xc00000000005c07c) when its low bit is clear. A loop compares s5 with a4 (unsigned) and a2 with s7 = 4, and loads words whose addresses the facts do not show. The facts show 64-bit stores at 0xc00000000005beec, 0xc00000000005bfe4, 0xc00000000005bffc, 0xc00000000005c018 and 0xc00000000005c080, but not their target addresses. The facts show no calls. Role (from name): stops the other processors. Which registers the stores hit is not established.
  Reviewed: 2026-11-12 (sonnet)

- `handle_io_buserr` in `6.5.30/unix.IP35` at 0xc00000000005cedc
  **Inferred.** Calls get_matching_spool_addr at 0xc00000000005cf38, kl_ioerror_handler at three sites (0xc00000000005cfd8, 0xc00000000005d04c, 0xc00000000005d108), cmn_err_tag at 0xc00000000005d0ac and 0xc00000000005d0c8 with message ids 0xb6c and 0xb6d, and get_nasid at 0xc00000000005d118. The calls sit on different branches, selected mainly by the result of get_matching_spool_addr compared with 0, -1, 1 and 2. The one return shown yields 1 or 2 in v0, and what those values mean is not established. Role (from name): handles an I/O bus error.
  Reviewed: 2026-11-12 (sonnet)

- `handle_uncac_buserr` in `6.5.30/unix.IP35` at 0xc00000000005d12c
  **Inferred.** Calls get_matching_spool_addr at 0xc00000000005d160 first and branches on its result (0, -1, 1, 2). Many call sites pair nasid_to_partid and part_id, compare the two results, and on one branch call cmn_err_tag (message ids between 0xb6e and 0xbb1, not a contiguous run) and then partition_handle_error at 0xc00000000005ecfc, with a2 set to a small constant that varies by site (values 1, 2, 4 to 0xa seen). Other sites call error_check_part_page, error_page_discard and error_check_memory_access. The facts show no get_nasid call here. The meanings of the a2 codes are not in the facts. Role (from name): handles uncached bus errors.
  Reviewed: 2026-11-12 (sonnet)

- `handle_cac_buserr` in `6.5.30/unix.IP35` at 0xc00000000005da30
  **Inferred.** The routine is 2420 bytes. It has one kl_panic call at 0xc00000000005dae4, reached on a branch when a1 is non-zero, and several checks: klerror_check_recoverable, error_check_memory_access, error_check_part_page and error_check_poisoned_state. It also calls log_md_errors and error_page_discard. The facts do not show how the result of klerror_check_recoverable is decoded, and nothing in them shows page retirement. Role (from name): the cache-error counterpart of the uncached bus-error handler.
  Reviewed: 2026-11-12 (sonnet)

- `intpend0` in `6.5.30/unix.IP35` at 0xc000000000060270
  **Inferred.** Tests a word loaded from a global at -0x3a60 against the word at 0x9200000001000098 (a nonzero AND keeps looping), and calls ms1bit at 0xc0000000000602e8 to pick a bit index. That index selects a record (72-byte stride from the base at -0x3a50), and cvsema, valusema and vsema are called on the address record+32; vsema (0xc00000000006035c) only when valusema's result is below a global word. It makes two indirect calls (0xc00000000006032c, 0xc0000000000603c0) whose callee is not resolved and which pass the doubleword at record +16 as the first argument and its own a0 as the second, each conditional on branches over record fields. Role (from name): dispatch of pending bits to semaphores. The record layout is unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `intpend1` in `6.5.30/unix.IP35` at 0xc0000000000603d0
  **Inferred.** Tests a word loaded from a global at -0x3a58 against the word at 0x92000000010000a0, then calls ms1bit (0xc00000000006041c) to pick a bit index and finds a record from the base at -0x3a48 (72-byte stride). The facts show no semaphore calls. When the doubleword at record +0 is nonzero it makes an indirect call at 0xc000000000060454 with an unresolved callee, passing the doubleword at record +16 as the first argument and its own a0 as the second. cmn_err is called at 0xc0000000000604ac on a path past the zero test of record +0, and the purpose of that path is not established. Role (from name): the companion of intpend0.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.30/unix.IP35` at 0xc000000000060824
  **Inferred.** Loads a per-CPU table entry and two fields from it (a byte at +0x528 and a halfword at +0x42c). It branches on a1 equal to 0xab and does one 64-bit store; the facts do not show its address or value, so which address it writes and how the halfword selects it are not established. It returns 0.
  Reviewed: 2026-11-12 (sonnet)

- `cpuintr` in `6.5.30/unix.IP35` at 0xc000000000060890
  **Inferred.** Reads a global word at -0x3d80 and tests bit 0x20 of it twice. When a byte at -0x3f80 and a word at 0xc00000000163b894 are both nonzero it calls bcopy with a2 = 0x168. It then calls doacvec and doactions, and, when bit 0x20 of the global is set, log_tstamp_event twice (codes 0x71 and 0x65). The facts show a 64-bit store at 0xc0000000000608d4 without its address. It returns doactions' or log_tstamp_event's result. Role (from name): the CPU-level interrupt entry. The copied structure is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `dbgintr` in `6.5.30/unix.IP35` at 0xc000000000060a04
  **Inferred.** Loads a halfword and a per-CPU word, does one 64-bit store whose address the facts do not show, and calls debug at 0xc000000000060a40 with a0 = 0xc000000001524a40 and a2 = 0x9200000001000090. It returns debug's result. Nothing in the facts shows what debug does with the arguments. Role (from name): a small wrapper for the debug interrupt.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.30/unix.IP35` at 0xc000000000060d34
  **Inferred.** ANDs the word at 0x92000000010000a0 with a value in v0 and, when the result is nonzero, calls dbgintr at 0xc000000000060d74 with a0 = 0 and a1 to a3 passed through. Otherwise it returns without the call. The test value in v0 comes from an unresolved source, so the trigger condition is not established. It returns dbgintr's result or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `intr_clear_bits` in `6.5.30/unix.IP35` at 0xc000000000061060
  **Inferred.** Loads the doubleword at [a1], returns at once when it is zero, and otherwise does up to two 64-bit stores, each guarded by an AND of a loaded word with an unresolved register (branches at 0xc000000000061094 and 0xc0000000000610b8); a loop at 0xc0000000000610c8 repeats while a5 is nonzero. The facts show neither the store addresses nor the stored values. Role (from name): clears interrupt bits. The masks and the bit meanings are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.30/unix.IP35` at 0xc000000000065468
  **Inferred.** Loads a 64-bit word at 0xc000000000065494 and two halfwords at 0xc0000000000654b4 and 0xc0000000000654b8, with addresses the facts do not show. It then calls mapped_kernel_setup_tlb at 0xc0000000000654bc with a2 and a3 passed through, and makes a tail transfer at 0xc0000000000654dc to bootstrap+124 (0xc0000000000654e4). Role (from name): early kernel setup. The callee name says it sets up mapped TLB entries, but the facts show only the call.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.30/unix.IP35` at 0xc000000000068d0c
  **Inferred.** Branches on a0 being zero and on its sign (0xc000000000068d38, 0xc000000000068d44). The main path calls spl65 (0xc000000000062004) at 0xc000000000068d94, then __nanotime at 0xc000000000068d9c, then nested_spinlock on 0xc000000001644c5c at 0xc000000000068dac, and later releases that lock with mutex_spinunlock. A second path (0xc000000000068f30 onward) repeats the same four calls. Compares against 0x3e8 and 0xf4240 are made on s0, and 0xf4240 may be a microseconds-per-second figure. Role (from name): adjusts the time of day. The adjustment arithmetic is not traced.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.30/unix.IP35` at 0xc000000000068fbc
  **Inferred.** Takes the lock at 0xc000000001644c5c with mutex_spinlock_spl and releases it with mutex_spinunlock (three call sites in all, two of them unlocks on different paths). It loads globals at 0xc000000001644c70, 0xc000000001644c78, 0xc000000001644c80 and 0xc000000001644c88 and compares the counter at 0x9200000001030100 with the sum of the last two (unsigned). It compares values against 0x3b9aca00, which is the figure for nanoseconds per second, and does one 64-bit and one 32-bit store whose addresses the facts do not show. Role (from name): returns the time as seconds plus nanoseconds; the constant is the only support.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.30/unix.IP35` at 0xc0000000000698bc
  **Inferred.** Loads the word at -0x38b4 and, when it is nonzero, returns it. When it is zero it calls spl1 at 0xc0000000000698d4, clears the counter with _set_count(0), runs loops that read 0x9200000001030100 and compare it with a3 (unsigned), then calls _get_count and passes the result to cpu_cycles_adjust, does a 32-bit store, and restores the level with splx. Role (from name): calibrates the CPU cycle rate against a timer. The loop's exit condition depends on a3, which is not traced.
  Reviewed: 2026-11-12 (sonnet)

- `acktmoclock` in `6.5.30/unix.IP35` at 0xc00000000006a104
  **Inferred.** Loads a halfword at abs 0xffffffffffffc428 and a per-CPU entry, then tests bit 0 of the byte at entry+0x528. Each path does one 64-bit store (at 0xc00000000006a12c on the bit-set path, at 0xc00000000006a13c on the bit-clear path); the facts do not show the store addresses. It returns the halfword shifted left by 4. Role (from name): acknowledges the time-of-day clock interrupt. The register meanings are not established.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.30/unix.IP35` at 0xc00000000006a140
  **Inferred.** The same shape as acktmoclock: it tests bit 0 of the byte at entry+0x528, does one 64-bit store on each path (0xc00000000006a168 and 0xc00000000006a178) with addresses the facts do not show, and returns the same shifted halfword. Role (from name): acknowledges a second clock whose meaning is not in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.30/unix.IP35` at 0xc00000000006a17c
  **Inferred.** Takes the lock at 0xc000000001644cc0 (mutex_spinlock_spl at 0xc00000000006a188) and reads several 64-bit counters. When the word at 0xc00000000163b8f0 is zero it branches to the setkgvector call at 0xc00000000006a344 with a0 = 0xc00000000006ae00, the address of prof_intr. The facts do not show a flag being set. Otherwise it calls cnodetocpu at 0xc00000000006a284 (when a word at [-0x3bf8]+15448 is not 1), makes several 64-bit stores whose addresses are not shown, and releases the lock with mutex_spinunlock. That suggests the vector for this clock is prof_intr, but the facts show only the address passed, not the install. Role (from name): starts the kg clock.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.30/unix.IP35` at 0xc00000000006a35c
  **Inferred.** Branches on the word at 0xc00000000163b8f8: when it is zero it does a store and calls spinlock_init on 0xc000000001644cc0 at 0xc00000000006a424 and does not take the lock. Otherwise it takes the lock with mutex_spinlock_spl at 0xc00000000006a370 and calls mutex_spinunlock on one of three paths (0xc00000000006a3a0, 0xc00000000006a3fc), and on one calls ackkgclock at 0xc00000000006a404. The values 0x9200000001000158 and 0x9200000001000150 appear as register contents at the unlock calls, and the facts show stores without their addresses. Role (from name): the slow path of the kg clock. The branch conditions depend on globals whose values are not known here.
  Reviewed: 2026-11-12 (sonnet)

- `prof_intr` in `6.5.30/unix.IP35` at 0xc00000000006ae00
  **Inferred.** startkgclock passes this routine's address to setkgvector. When bit 0x20 of the global at -0x3d80 is clear it calls fastick_maint at 0xc00000000006ae18, and on a later branch calls cnode_subnode_to_cpu at 0xc00000000006aea8 with a3 = 0x9200000001604200 (an argument, not a read). It compares the counter at 0x9200000001030100 with a value loaded from [[-0x3bf8]+15440]. When bit 0x20 is set it also logs two timestamp events (codes 0x6e and 0x65). Role (from name): a profiling interrupt handler; fastick_maint suggests a periodic maintenance step, and the facts do not say what it does.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_mem` in `6.5.30/unix.IP35` at 0xc00000000006b9ec
  **Inferred.** The facts show one 64-bit load (0xc00000000006b9f0, address not shown), a sign test on a2 at 0xc00000000006ba28, and two sprintf calls at 0xc00000000006ba60 and 0xc00000000006ba80 with a1 = 0xc000000001525778 and 0xc000000001525758. No address arithmetic is visible here. Role (from name): translates a memory-directory address. The translation part is not established by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_pdir` in `6.5.30/unix.IP35` at 0xc00000000006ba90
  **Inferred.** The same shape as mdir_xlate_addr_mem: one 64-bit load whose address is not shown, a sign test on a2 at 0xc00000000006bac8, and two sprintf calls at 0xc00000000006bae0 and 0xc00000000006bb00 with the same two a1 constants. The first path returns sprintf's result. Role (from name): a page-directory variant. No page-level arithmetic is visible in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_sdir` in `6.5.30/unix.IP35` at 0xc00000000006bb10
  **Inferred.** One 64-bit load whose address is not shown, then one sprintf call at 0xc00000000006bb4c with the same first a1 constant (0xc000000001525778) and a4 = 0x1ffffffff, and a return of sprintf's result. It calls sprintf with the same first constant as its siblings, but the facts do not show a shared formatting path. Role (from name): a directory variant of the translate routines.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.30/unix.IP35` at 0xc00000000006e440
  **Inferred.** Does three 64-bit stores at 0xc00000000006e440, 0xc00000000006e444 and 0xc00000000006e448 whose addresses the facts do not show, loads a doubleword from 0xfd8 into k1, and branches on k1 being nonzero. It ends with a tail call at 0xc00000000006e488 whose callee is unresolved, passing a0 to a3 through. Role (from name): an ECC exception-vector stub. The role of the stores in a saved exception frame is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.30/unix.IP35` at 0xc0000000000778bc
  **Inferred.** Compares a1 with 1, 2 and 4 (branches at 0xc0000000000778f4, 0xc00000000007790c and 0xc000000000077924) and does a byte, halfword or word load of that size at offset 0; the base register is not shown. It does a 32-bit store at 0xc0000000000778ec and another at 0xc000000000077984, with addresses the facts do not show. It calls cmn_err at 0xc000000000077950 with a0 = 0 and returns 0. Role (from name): probes a bad address. Whether a fault is caught is not shown by the facts.
  Reviewed: 2026-11-12 (sonnet)
