# Routine role statements: tier-1 batch 10

Twenty-five routines from the ranked tier-1 list of kudzu, targets 250 to 274 (0-based). All are read from the 6.5.22 install, and all come from IP22boot boot directories. The batch follows the ranking, not a platform choice. Each entry is graded inferred: the disassembly facts give the instructions, branches, constants and callees, and the statements below reason from those facts and from the symbol names the binaries carry. Where the facts do not settle the role, the entry says so.

- `_clock_func` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x806c
  **Inferred.** `_clock_func` (260 bytes) runs its middle section under `splhi` at 0x8080 and `splx` at 0x8104. Around that section it copies the seven bytes at a1 into words in the 0xbfbe00xx range and copies them back at 0x8124 to 0x816c. A zero flag in a0 at 0x8098 skips the middle section. The name suggests a clock tick callback, but nothing in the facts shows what the middle section does, so the role stays open.

- `nvchecksum` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x8c84
  **Inferred.** `nvchecksum` (132 bytes) calls `get_nvreg` inside a loop at 0x8ccc. The loop value s0 is compared with 0x80 as a signed value at 0x8cc4 and tested for zero at 0x8cd8. That fits a checksum over non-volatile register bytes, but the accumulator arithmetic and the return value (v0 at 0x8d00 is unknown) are not in the facts.

- `stop_bell` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x9b1c
  **Inferred.** `stop_bell` (172 bytes) runs under `splhi` at 0x9b24 and `splx` at 0x9b7c. It calls `quiet_bell` at 0x9b30, then updates the .bss word at offset 0x604 and compares it with the word at offset 0x600. When they differ, it calls `start_bell` at 0x9ba8 with two halfwords read from a .bss table at offsets 0x580 and 0x582. Both paths then test bit 0 of the .bss word at offset 0x610. When that bit is set, the routine clears it, calls `wakeup` at 0x9b74 with the table address at offset 0x580, and then calls `splx`. `pckbd_bell` at 0x9c9c writes the same table offsets and reads the same .bss words at 0x600, 0x604 and 0x610, so the two routines share state. The name fits silencing a bell, but the facts do not give the meaning of the halfword arguments.

- `pckbd_bell` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x9c9c
  **Inferred.** `pckbd_bell` (476 bytes) is bracketed by `splhi` at 0x9cf4 and `splx` at 0x9d94 and 0x9e28. It sleeps on a .bss address at 0x9e18 with the constants 0x19 and -0x21, and it calls `start_bell` at 0x9de4 with the same convention as `stop_bell`. It indexes a .bss halfword table by the .bss word at 0x600 shifted left by 2, reading and writing table offsets 0x57c through 0x582. A back-edge at 0x9e38 returns to a loop at 0x9d24. The name suggests a PC keyboard bell generator; the unit of the sleep argument and the element layout of the table are not established.

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa100
  **Inferred.** `debug_stop_all_cpus` (8 bytes) has no call and no memory access in the disassembly facts, only a return whose v0 is unknown. A body this short is consistent with a stub. The name suggests a processor halt for the debugger, but nothing in the disassembly confirms that.

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa71c
  **Inferred.** `startkgclock` (48 bytes) calls `init_timer` at 0xa72c and branches on the .sdata word at +0x90 at 0xa724. It makes an indirect call through a register at 0xa738 and returns. Its siblings `slowkgclock` and `ackkgclock` have the same shape, so the three look like variants of one clock-control helper. The indirect callee is not named in the facts.

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa74c
  **Inferred.** `slowkgclock` (48 bytes) has the same shape as `startkgclock`: a call to `init_timer` at 0xa75c, a branch on the .sdata word at +0x90 at 0xa754 to 0xa764, and an indirect call at 0xa768. The name suggests a slow-clock variant, but the facts do not show how it differs from `startkgclock` beyond the branch target.

- `ackkgclock` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa840
  **Inferred.** `ackkgclock` (48 bytes) follows the same pattern again: a call to `init_timer` at 0xa850, a branch on the .sdata word at +0x90 at 0xa848 to 0xa858, and an indirect call at 0xa85c. The name suggests an acknowledge step for the same clock. The facts do not show what the indirect call does.

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xb288
  **Inferred.** `doadjtime` (380 bytes) is bracketed by `splhi` and `splx` calls at 0xb2d0, 0xb318, 0xb3a8 and 0xb3d0. It branches on the sign of a0 at 0xb2ac and compares s0 with the constant 0xafc80 at 0xb358. It reads two .sdata words at +0x80 and +0x84 for the `splx` arguments. The name points to a time-of-day adjustment, but the meaning and units of the constants 0x4650, 0x190 and 0xafc80 are not in the facts.

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xec20
  **Inferred.** `ecc_handler` (1988 bytes, 0xec20 to 0xf3e4) is the largest routine in this batch, and the name and callees point to an ECC or parity exception handler for the secondary cache. It disables and re-enables the secondary-cache error mode through `_r4600sc_disable_scache_erl` at 0xec7c and `_r4600sc_enable_scache_erl` at 0xf0a8 and 0xf27c. It saves parity data with `perr_save_info` at 0xedf0, logs with `ecc_log_error` at 0xefe8 using code 6, and calls `ecc_fixcache` at 0xf2e8. Its branches depend on a two-set-cache flag, on the r4000 config word and on a structure at 0xa0000e68; the mapping from hardware error bits to these paths is open.

- `_ticksper1024inst` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x171b4
  **Inferred.** `_ticksper1024inst` (344 bytes) makes no call. It reads a status word at 0xbfbd9858, tests bits 0x20 and 0x1 at 0x171c8 and 0x171d4, and stores bytes to two uncached addresses. A loop at 0x172fc compares the constant 0x2710 (10000) with a counter, and an early return at 0x171ec gives 0x13. The name suggests a count of ticks per 1024 instructions, but the facts do not confirm the unit or where the count comes from, and the value of the second return is unknown.

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x17c88
  **Inferred.** `badaddr` (372 bytes) probes the address in a0 with a width from a1: the disassembly facts show separate loads of 1, 2, 4 and 8 bytes, selected at 0x17d00 to 0x17d54. Before the probe it writes the two error words at 0xbfa000ec and 0xbfa000fc. After the probe, a nonzero error word branches to 0x18524, the entry of `baerror`, and the function otherwise returns 0. The `cmn_err` call at 0x17d64 is on a path the facts do not place, so which outcome reports a fault is open.

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x17dfc
  **Inferred.** `badaddr_val` (440 bytes) has the same width-selected probe as `badaddr`, but it stores the value it reads through a2 (`sb`, `sh`, `sw` or `sd` at 0x17e94 to 0x17ef4), which reads as an output pointer. It writes the same two error words first and branches to `baerror` at 0x18524 on a nonzero error word. It calls `cmn_err` at 0x17f08 with a message at .data+9377 and returns 0. The facts do not place the `cmn_err` call on a path.

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x17fb4
  **Inferred.** `wbadaddr` (416 bytes) is the store-side probe: it writes 1, 2, 4 or 8 bytes to a0 (`sb`, `sh`, `sw`, `sd` at 0x18034 to 0x1807c), with the width taken from a1. It writes the same two error words, branches to `baerror` at 0x18524 on a nonzero error state, and returns 0 after a `cmn_err` call at 0x18090. The name fits a write probe. The store value and the way a fault reaches this routine are not in the facts.

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x18310
  **Inferred.** `earlybadaddr` (356 bytes) is a read probe with widths of 1, 2 and 4 bytes only; the facts show no 8-byte case (branches at 0x1838c to 0x183c8). It writes the global `nofault` at 0x1837c and 0x18444, and the facts do not show how that variable is read. Its error branch goes to `earlybaerror` at 0x1859c, and it returns 0 after a `cmn_err` call at 0x183d8. The name suggests use before the full fault machinery is in place, which the facts neither confirm nor rule out.

- `baerror` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x18524
  **Inferred.** `baerror` (120 bytes) is the bus-error recording path that the probe routines branch into. It writes the two error words at 0xbfa000ec and 0xbfa000fc, reads 0xbfa00004, and tests the word at 0xffffa018 at 0x1856c. It writes a halfword at 0xffffa018 plus 0xc2, a word at 0xffffa1d8 and a word at 0xa00002f0, then returns the constant 1 at 0x18594. The constant 1 suggests a fault counted as handled, but the facts do not give the meaning of the stored values.

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1859c
  **Inferred.** `earlybaerror` (108 bytes) is the early twin of `baerror`. It writes the same two error words and reads 0xbfa00004, writes the global `nofault` at 0x185e8, and writes a word at 0xa00002f0. It returns the constant 1 at 0x18600. Unlike `baerror`, the facts show no halfword write and no test of the word at 0xffffa018.

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x18c14
  **Inferred.** `_ecc_errorvec` (24 bytes) does nothing but pass its four argument registers to `ecc_exception` with a direct jump at 0x18c24. It is an exception vector stub that hands off to the ECC handler. The facts show only the jump, not how the vector is selected.

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1ed58
  **Inferred.** `VEC_dbe` (172 bytes) copies error state into globals: it loads 0xbfa000ec into `cpu_err_stat`, 0xbfa000fc into `gio_err_stat`, 0xbfa000e4 into `cpu_par_adr` and 0xbfa000f4 into `gio_par_adr`. It also reads 0xbfbd9900 and 0xbfbb0010 into `hpc3_ext_io` and `hpc3_bus_err_stat`, writes the two error words at 0xbfa000fc and 0xbfa000ec again, saves a doubleword at a0+0x140, and jumps to `VEC_trap` at 0x1edfc. The name and the global names suggest a data-bus-error vector.

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x20c64
  **Inferred.** `VEC_ibe` (196 bytes) is the instruction-bus-error counterpart of `VEC_dbe`. It copies the same error words into the same globals and writes the error words back, then saves a doubleword at a0+0x140. It jumps to `VEC_trap` at 0x20d20 with a2 not set from the entry (unknown in the facts). The name suggests an instruction-bus-error vector, but the facts do not show how the two vectors are selected.

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP22boot/ksync_metered.a(sema.o)` at 0x35b8
  **Inferred.** `_mraccess` (240 bytes) is a metering hook for a wait-channel meter. It calls `wchanmeter` at 0x35d0 and branches on whether it returns a pointer. On that structure it takes timestamps with `_get_timestamp` at 0x35f0 and 0x3678, and it updates a word at +0x138 with an `ll`/`sc` pair at 0x3650 and 0x365c. It records an event with `mrmeter_entry` at 0x3690 using type 5, and calls `mr_access` at 0x35e0 on one path. The event types and field meanings are not in the facts.

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP22boot/ksync_metered.a(sema.o)` at 0x36e0
  **Inferred.** `_mrupdate` (220 bytes) is the update path for the same wait-channel meter. It calls `wchanmeter` at 0x36f8, `mr_update` at 0x3708, and `_get_timestamp` at 0x3718 and 0x37a4. It writes fields between offsets 0x140 and 0x178 and records an event with `mrmeter_entry` at 0x379c using type 6, where `_mraccess` uses type 5. The facts show a branch on bit 0x4 of a flags value at 0x3768, not its meaning.

- `MgrasGeneralInterrupt` in `6.5.22/usr/cpu/sysgen/IP22boot/mgras.a(mgras.o)` at 0x7498
  **Inferred.** `MgrasGeneralInterrupt` (752 bytes) handles the general interrupt of an MGRAS graphics board. It indexes a per-board table `MgrasBoards` with a 36-byte stride and reads status words at offsets 0x70100 and 0x70104 from each board, testing bits 0x40 and 0x1. It releases semaphores with `vsema` at 0x75d4, 0x761c and 0x7640. Four message IDs, 0xaad to 0xab0, go out through `cmn_err_tag` at 0x7694 to 0x76f4, each on a path guarded by a status bit or flag, and an error path calls `mgras_error` at 0x7700. A buffer-swap completion calls `MgrasSchedSwapBufFinish` at 0x7724. Which board status bit means what is not established by these facts.

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP22boot/os.a(pda.o)` at 0x1594
  **Inferred.** `doacvec` (96 bytes) reads bytes and words from absolute 0xffffa070 to 0xffffa090 and writes back to 0xffffa03e, 0xffffa071 and 0xffffa090. It branches on the byte at 0xffffa070 and on the doubleword at [-0x5f70] at 0x15d8. Its calls are `panicspin` at 0x15a4, `tlbflush_rand` with a0 = 1 at 0x15cc, and `tlbflush` at 0x15e0; the facts do not order these paths. The name does not say what "cvec" refers to, so the purpose stays open. The TLB flush and panic calls are the firm facts.

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP22boot/os.a(syssgi.o)` at 0xcc
  **Inferred.** `syssgi` (10944 bytes, 0xcc to 0x2b8c) is the entry of the IRIX `syssgi` call. The disassembly facts resolve only the entry path: it calls `get_current_abi` at 0xec, reads the word at a0 and checks it against 0x4d7 (1239) at 0x11c, then indexes a .rodata table with it at 0x124 and jumps through that table at 0x128. The jump-table targets are unresolved, and the code from 0x160 to 0x28ec and from 0x2908 to 0x2b88 is not reached by any path the disassembly followed, so most of the routine is open. A branch at 0x100 to a block at 0x28f0 calls `sat_never` and `sat_set_subsysnum` when `sat_enabled` is set. The one explicit return on the followed path gives the constant 0x16 (22) at 0x158, which is the usual invalid-argument errno, an inference the facts do not establish.
