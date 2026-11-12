# Tier-1 routine readings, batch 29 (kudzu)

Role statements for the 25 routines at target indexes 725 to 749 of the ranked tier-1 hardware-linked list (`t1list`, located rows, `--hardware-only`). Each statement comes only from the instruction facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the inventory rows. Grades are inferred throughout, because the facts show what the code does, not what the author meant. Where a callee, register or constant is unresolved, the statement says so.

All 25 entries are in `6.5.30/unix.IP35`. None were left out.

- `stop_all_cpus` in `6.5.30/unix.IP35` at 0xc00000000005be70
  **Inferred.** Reads a status word at 0x9200000001000020 and branches to a different block when its low bit is clear. A loop bounded by a count in s5 and a counter in a2 (compared with s7 = 4) reads per-CPU words. It writes 0x9200000001000410 early and 0x9200000001000408 near its exit. The instruction facts make no calls. The name suggests halting the other processors, but which registers the stores hit is not established.

- `handle_io_buserr` in `6.5.30/unix.IP35` at 0xc00000000005cedc
  **Inferred.** Handles an I/O bus error. It calls get_matching_spool_addr at 0xc00000000005cf38, kl_ioerror_handler at three sites (0xc00000000005cfd8, 0xc00000000005d04c, 0xc00000000005d108), and cmn_err_tag at 0xc00000000005d0ac and 0xc00000000005d0c8 with message ids 0xb6c and 0xb6d. It returns 1 or 2 in v0. What the two return values mean is not established.

- `handle_uncac_buserr` in `6.5.30/unix.IP35` at 0xc00000000005d12c
  **Inferred.** A longer handler for uncached bus errors. Many call sites pair nasid_to_partid and part_id with a cmn_err_tag message (ids 0xb6f through 0xbb0), and then call partition_handle_error at 0xc00000000005ecfc with a2 set to a small code that varies by site. The code values are used as selectors, but their meanings are not in these facts. It also calls get_nasid at 0xc000000000041434.

- `handle_cac_buserr` in `6.5.30/unix.IP35` at 0xc00000000005da30
  **Inferred.** The cache-error counterpart, at 2420 bytes. It has one kl_panic call at 0xc00000000005dae4 and several recoverable-error checks: klerror_check_recoverable, error_check_memory_access, error_check_part_page and error_check_poisoned_state. It also calls log_md_errors and error_page_discard. The second name suggests page retirement, which the facts do not show. The instruction facts do not show how the recoverable check's result is decoded.

- `intpend0` in `6.5.30/unix.IP35` at 0xc000000000060270
  **Inferred.** Tests a word from a global at -0x3a60 against the word at 0x9200000001000098, then calls ms1bit at 0xc0000000000602e8 to pick a bit index. It uses that index to find a per-bit record and calls cvsema, valusema and vsema on the record at +32. It also calls a function pointer stored at record +16. The name suggests dispatch of pending bits to semaphores, but the record layout is unresolved.

- `intpend1` in `6.5.30/unix.IP35` at 0xc0000000000603d0
  **Inferred.** The companion to intpend0 with the same pending-bit lookup through ms1bit (0xc00000000006041c), but it does not touch the semaphores. It makes an indirect call with jalr at 0xc000000000060454 that passes the doubleword at record +16 as its first argument and its own a0 as the second, and the callee is not resolved. It also calls cmn_err at 0xc0000000000604ac on one path. The purpose of that path is not established.

- `sendintr` in `6.5.30/unix.IP35` at 0xc000000000060824
  **Inferred.** Reads a per-CPU table entry and a few fields from it, then stores a 64-bit word to one of two addresses, 0x9200000001800090 or 0x9200000001a00090. The choice is made from a shifted halfword field and a branch on a1 equal to 0xab. Returns 0. Which of the two addresses belongs to which target is not established.

- `cpuintr` in `6.5.30/unix.IP35` at 0xc000000000060890
  **Inferred.** The CPU-level interrupt entry. It copies 0x168 bytes with bcopy, calls doacvec and doactions, and logs two timestamp events (codes 0x71 and 0x65) when a bit in a global word is set. It also reads and writes 0x9200000001000090 and 0x9200000001000098. The name fits interrupt entry, and the calls agree, but the copied structure is not identified.

- `dbgintr` in `6.5.30/unix.IP35` at 0xc000000000060a04
  **Inferred.** A small wrapper. It reads a halfword and a per-CPU word, stores to 0x9200000001000090, and calls debug at 0xc000000000060a40 with a pointer to a global. It returns debug's result. Nothing in the facts shows what debug does with the arguments.

- `chkdebug` in `6.5.30/unix.IP35` at 0xc000000000060d34
  **Inferred.** Tests a bit from the word at 0x92000000010000a0 against a value in v0, and calls dbgintr at 0xc000000000060d74 with its arguments passed through when the bit is set. The test value comes from an unresolved source, so the trigger condition is not established.

- `intr_clear_bits` in `6.5.30/unix.IP35` at 0xc000000000061060
  **Inferred.** Reads a pointer's word at a1 and clears bits in two 64-bit words at 0x9200000001000090 and 0x9200000001000098. The stores are guarded by zero tests on the loaded words. The name suggests clearing interrupt bits, but the masks and the bit meanings are not in the facts.

- `bootstrap` in `6.5.30/unix.IP35` at 0xc000000000065468
  **Inferred.** Reads one word from 0x9200000001600000 and two halfwords from an unresolved address, then calls mapped_kernel_setup_tlb at 0xc0000000000654bc with a2 and a3 passed through. A jr at 0xc0000000000654dc goes to a register-held address in bootstrap+124, but the instruction listing did not resolve it and the code after it is not reached from the entry. The name fits early kernel setup. The callee name says it sets up mapped TLB entries, but the facts show only the call.

- `doadjtime` in `6.5.30/unix.IP35` at 0xc000000000068d0c
  **Inferred.** Adjusts the time of day. It raises the interrupt level with spl65 (0xc000000000062004), takes a spin lock at 0xc000000001644c5c with nested_spinlock, reads the time through __nanotime, and releases the lock with mutex_spinunlock. A loop compares a count with 0x3e8 and with 0xf4240, which may be a microseconds-per-second figure, but the adjustment arithmetic is not traced.

- `__nanotime` in `6.5.30/unix.IP35` at 0xc000000000068fbc
  **Inferred.** Reads a 64-bit counter at 0x9200000001030100 and a clock word at 0x9200000001604200, under the spin lock at 0xc000000001644c5c (mutex_spinlock_spl and mutex_spinunlock). It keeps a pair of 64-bit words at 0xc000000001651638 and a 32-bit word at 0xc000000001651630, and normalizes against 0x3b9aca00, which is a nanoseconds-per-second figure. The name and the constant point to seconds plus nanoseconds, which is an inference from the constant.

- `findcpufreq_cycles` in `6.5.30/unix.IP35` at 0xc0000000000698bc
  **Inferred.** Calls spl1 at 0xc0000000000698d4, clears the counter with _set_count(0), reads it back with _get_count, and passes the value to cpu_cycles_adjust. Loops read 0x9200000001030100 and compare it with a3 unsigned, and the routine restores the level with splx. The name suggests calibrating the CPU cycle rate against a timer, but the loop's exit condition depends on a3, which is not traced.

- `acktmoclock` in `6.5.30/unix.IP35` at 0xc00000000006a104
  **Inferred.** Reads a halfword at abs 0xffffffffffffc428 and a per-CPU entry, then writes 0x9200000001000128 and 0x9200000001000120. The second write is skipped when bit 0 of a byte in the entry is set. It returns a halfword shifted left by 4. The name suggests acknowledging the time-of-day clock interrupt, but the register meanings are not established.

- `ackkgclock` in `6.5.30/unix.IP35` at 0xc00000000006a140
  **Inferred.** The same shape as acktmoclock, with writes to 0x9200000001000138 and 0x9200000001000130. The name refers to a second clock whose meaning is not in these facts. It returns the same shifted halfword.

- `startkgclock` in `6.5.30/unix.IP35` at 0xc00000000006a17c
  **Inferred.** Takes the lock at 0xc000000001644cc0 (mutex_spinlock_spl at 0xc00000000006a188), reads several 64-bit counters. When the flag word at -32384(gp) is zero, it calls setkgvector at 0xc00000000006a344 with the address of prof_intr (0xc00000000006ae00) and sets the flag to 1. The counter code calls cnodetocpu at 0xc00000000006a284 and writes 0x9200000001000158, 0x9200000001000118 and 0x9200000001000150. That suggests it installs prof_intr as the vector for this clock, but the facts show only the address, not the install.

- `slowkgclock` in `6.5.30/unix.IP35` at 0xc00000000006a35c
  **Inferred.** The slow path of the same clock. It takes the lock at 0xc000000001644cc0, writes 0x9200000001000158 and 0x9200000001000150 and a 32-bit word at abs 0xffffffffffffc53c, and calls ackkgclock at 0xc00000000006a404. It ends by calling spinlock_init on the same lock. The branch conditions depend on globals whose values are not known here.

- `prof_intr` in `6.5.30/unix.IP35` at 0xc00000000006ae00
  **Inferred.** The profiling interrupt, whose address startkgclock passes to setkgvector. It calls fastick_maint at 0xc00000000006ae18 and cnode_subnode_to_cpu at 0xc00000000006aea8, reads counters at 0x9200000001604200 and 0x9200000001030100, and logs two timestamp events (codes 0x6e and 0x65). The name fits profiling, and the fastick_maint call suggests a periodic maintenance step. The facts do not say what fastick_maint does.

- `mdir_xlate_addr_mem` in `6.5.30/unix.IP35` at 0xc00000000006b9ec
  **Inferred.** The name suggests translating a memory-directory address. The instruction facts show only a read from 0x9200000001780008, one sign test on a2 at 0xc00000000006ba28, and two sprintf calls at 0xc00000000006ba60 and 0xc00000000006ba80 that format text from fixed strings (0xc000000001525778 and 0xc000000001525758). No address arithmetic is visible here. The translation part of the name is not established by these facts.

- `mdir_xlate_addr_pdir` in `6.5.30/unix.IP35` at 0xc00000000006ba90
  **Inferred.** The same shape as mdir_xlate_addr_mem: a read from 0x9200000001780008, a sign test on a2 at 0xc00000000006bac8, and two sprintf calls at 0xc00000000006bae0 and 0xc00000000006bb00 using the same two format strings. It returns sprintf's result. The name suggests a page-directory variant, but no page-level arithmetic is visible in the facts.

- `mdir_xlate_addr_sdir` in `6.5.30/unix.IP35` at 0xc00000000006bb10
  **Inferred.** The third variant. It reads 0x9200000001780008, makes one sprintf call at 0xc00000000006bb4c with the same first format string and a4 = 0x1ffffffff, and returns sprintf's result. Its format and argument pattern are the same as its siblings, so the three routines appear to share one formatting path.

- `_ecc_errorvec` in `6.5.30/unix.IP35` at 0xc00000000006e440
  **Inferred.** An exception-vector stub. It saves three 64-bit values into low absolute addresses 0x4c8, 0x4d0 and 0x400, loads a doubleword from 0xfd8 into k1, and branches on k1. One path reaches a register jump at 0xc00000000006e488, whose target is unresolved. The stores to low memory are facts. Their role in a saved exception frame is inferred from the vector shape.

- `earlybadaddr` in `6.5.30/unix.IP35` at 0xc0000000000778bc
  **Inferred.** A probe for a bad address. It tests access sizes 1, 2 and 4 (the branches on a1 at 0xc0000000000778f4, 0xc00000000007790c and 0xc000000000077924), reads one value of that size through a0, and stores to a no-fault location at 0xc000000001645ae8. It reports through cmn_err at 0xc000000000077950 and returns 0. Whether a fault is caught by the no-fault store is inferred, not shown by the facts.
