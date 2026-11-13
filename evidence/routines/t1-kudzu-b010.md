# Routine role statements: tier-1 batch 10

Twenty-five routines from the ranked tier-1 list of kudzu, targets 250 to 274 (0-based). All are read from the 6.5.22 install, and all come from IP22boot boot directories. The batch follows the ranking, not a platform choice. Each entry is graded inferred: the disassembly facts give the instructions, branches, constants and callees, and the statements below reason from those facts and from the symbol names the binaries carry. Where the facts do not settle the role, the entry says so.

- `_clock_func` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x806c
  **Inferred.** `_clock_func` (260 bytes) calls `splhi` at 0x8080 and `splx` at 0x8104 and branches on a0 at 0x8098. When a0 is nonzero it falls through to a run that loads bytes +0x0 to +0x6 (0x80b8 to 0x80ec), each followed by a word store; the stores' base is in the 0xbfbe0000 range (the `splx` call's a1 is 0xbfbe0000 in the facts). When a0 is zero the branch goes to 0x8118, which loads words (0x8124 to 0x8160) and stores bytes +0x0 to +0x6 (0x8128 to 0x816c). The two copies are separate paths of the branch, not one after the other. The facts list one return, at 0x8110, and do not say where the zero-a0 path rejoins. Role (from name): a clock callback or clock register access. The facts do not show what the copied bytes mean.
  Reviewed: 2026-11-12 (sonnet)

- `nvchecksum` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x8c84
  **Inferred.** `nvchecksum` (132 bytes) calls `get_nvreg` inside a loop at 0x8ccc. The loop value s0 is compared with 0x80 as a signed value at 0x8cc4 and tested for zero at 0x8cd8. That fits a checksum over non-volatile register bytes, but the accumulator arithmetic and the return value (v0 at 0x8d00 is unknown) are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `stop_bell` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x9b1c
  **Inferred.** `stop_bell` (172 bytes) runs under `splhi` at 0x9b24 and `splx` at 0x9b7c. It calls `quiet_bell` at 0x9b30, then updates the .bss word at offset 0x604 and compares it with the word at offset 0x600. When they differ, it calls `start_bell` at 0x9ba8 with two halfwords read from a .bss table at offsets 0x580 and 0x582. Both paths then test bit 0 of the .bss word at offset 0x610. When that bit is set, the routine clears it, calls `wakeup` at 0x9b74 with the table address at offset 0x580, and then calls `splx`. `pckbd_bell` at 0x9c9c writes the same table offsets and reads the same .bss words at 0x600, 0x604 and 0x610, so the two routines share state. The name fits silencing a bell, but the facts do not give the meaning of the halfword arguments.
  Reviewed: 2026-11-12 (sonnet)

- `pckbd_bell` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x9c9c
  **Inferred.** `pckbd_bell` (476 bytes) calls `splhi` at 0x9cf4 and `splx` at 0x9d94 and 0x9e28. Three early branches at 0x9ccc, 0x9cd4 and 0x9cdc skip to 0x9dc0 when the word `bellok` (0x9ccc), a1 or a2 is zero. It calls `sleep` at 0x9e18 with a0 a .bss address, a1 = 0x19 and a2 = -0x21. When `sleep` returns zero (0x9e20 taken) control goes back through 0x9e38 to 0x9d24; otherwise it calls `splx` at 0x9e28. It calls `start_bell` at 0x9de4 with a1 = a2 at entry and a2 = 0. It indexes a .bss halfword table by the .bss word at 0x600 shifted left by 2, reading table offsets 0x57c and 0x57e and storing offsets 0x57e, 0x580 and 0x582. Role (from name): a PC keyboard bell generator. The unit of the sleep argument and the element layout of the table are not established.
  Reviewed: 2026-11-12 (sonnet)

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa100
  **Inferred.** `debug_stop_all_cpus` (8 bytes) has no call and no memory access in the disassembly facts, only a return whose v0 is unknown. A body this short is consistent with a stub. The name suggests a processor halt for the debugger, but nothing in the disassembly confirms that.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa71c
  **Inferred.** `startkgclock` (48 bytes) branches on the .sdata word at +0x90 at 0xa724 and calls `init_timer` at 0xa72c only when that word is nonzero (the branch is taken to 0xa734 when it is zero). It then makes an indirect call at 0xa738, whose callee the facts do not name, and returns. Its neighbours `slowkgclock` and `ackkgclock` have the same shape. Role (from name): a start step for a clock; the facts do not show what the indirect call does.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa74c
  **Inferred.** `slowkgclock` (48 bytes) has the same shape as `startkgclock`: it branches on the .sdata word at +0x90 at 0xa754 (taken to 0xa764 when zero), calls `init_timer` at 0xa75c only when that word is nonzero, and makes an indirect call at 0xa768 whose callee the facts do not name. Role (from name): a slow-clock variant. The facts do not show how it differs from `startkgclock`.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xa840
  **Inferred.** `ackkgclock` (48 bytes) follows the same pattern: it branches on the .sdata word at +0x90 at 0xa848 (taken to 0xa858 when zero), calls `init_timer` at 0xa850 only when that word is nonzero, and makes an indirect call at 0xa85c. Role (from name): an acknowledge step for the same clock. The facts do not show what the indirect call does.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xb288
  **Inferred.** `doadjtime` (380 bytes) calls `splhi` and `splx` on two paths, at 0xb2d0 and 0xb318 and at 0xb3a8 and 0xb3d0. It branches on a0 being zero at 0xb2a0 and on its sign at 0xb2ac, and compares s0 with the constant 0xafc80 at 0xb358. The .sdata words at +0x80 and +0x84 appear in the a3 and a4 slots at the `splx` calls, but `splx` takes only a0, which is the `splhi` result, so they are not shown to be `splx` arguments; the facts do not say how those words are used. Role (from name): a time-of-day adjustment. The meaning and units of the constants 0x4650, 0x190 and 0xafc80 are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0xec20
  **Inferred.** `ecc_handler` (1988 bytes, 0xec20 to 0xf3e4) calls `_r4600sc_disable_scache_erl` at 0xec7c, `perr_save_info` at 0xedf0, `ecc_log_error` at 0xefe8 with a0 = 0x6, `_r4600sc_enable_scache_erl` at 0xf0a8 and 0xf27c, and `ecc_fixcache` at 0xf2e8, among other calls. Its branches depend on a two-set-cache flag, on the r4000 config word and on a structure at 0xa0000e68. Role (from name): an ECC or parity error handler. Whether it handles secondary-cache errors, and the mapping from hardware error bits to its paths, are not established.
  Reviewed: 2026-11-12 (sonnet)

- `_ticksper1024inst` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x171b4
  **Inferred.** `_ticksper1024inst` (344 bytes) makes no call. It reads a status word at 0xbfbd9858, tests bits 0x20 and 0x1 at 0x171c8 and 0x171d4, and stores bytes to uncached addresses with a base that is one of two values. An early return at 0x171ec gives 0x13. The branch at 0x172fc compares the constant 0x2710 (10000) with a value assembled from two byte loads (0x172ac and 0x172b4) and, when 0x2710 is the smaller, goes back to 0x171fc. Role (from name): a count of ticks per 1024 instructions. The facts do not confirm the unit or where the count comes from, and the value of the second return is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x17c88
  **Inferred.** `badaddr` (372 bytes) probes the address in a0 with a width from a1: the facts show separate loads of 1, 2, 4 and 8 bytes, selected by branches at 0x17d00 to 0x17d48. Before the probe it stores to the two error words at 0xbfa000ec and 0xbfa000fc. After the probe, three branches go to 0x18524, the entry of `baerror`: one when bit 0x4000 of a register value is set (0x17d98), and one each when the word at 0xbfa000ec or at 0xbfa000fc is nonzero (0x17dac, 0x17dc0). Otherwise it returns 0. The `cmn_err` call at 0x17d64 follows the branch at 0x17d48 that is taken when a1 is not 8, that is, when a1 matched none of the widths 1, 2, 4 and 8.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x17dfc
  **Inferred.** `badaddr_val` (440 bytes) has the same width-selected probe as `badaddr`, but it stores the value it reads (`sb`, `sh`, `sw` or `sd` at 0x17e94 to 0x17ef4), which reads as an output pointer in a2. It stores to the two error words first. It branches to `baerror` at 0x18524 when bit 0x4000 of a register value is set (0x17f3c) or when the word at 0xbfa000ec or 0xbfa000fc is nonzero (0x17f50, 0x17f64), and otherwise returns 0. It calls `cmn_err` at 0x17f08 with a message at .data+9377 on the path where a1 matched none of the widths 1, 2, 4 and 8 (the branch at 0x17ee4 taken).
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x17fb4
  **Inferred.** `wbadaddr` (416 bytes) is the store-side probe: it stores 1, 2, 4 or 8 bytes to a0 (`sb`, `sh`, `sw`, `sd` at 0x18034 to 0x1807c), with the width selected by a1 at the branches 0x1802c to 0x18074. It stores to the two error words first, branches to `baerror` at 0x18524 when bit 0x4000 of a register value is set (0x180f0) or when the word at 0xbfa000ec or 0xbfa000fc is nonzero (0x18104, 0x18118), and otherwise returns 0. The `cmn_err` call at 0x18090 is on the path where a1 matched none of the widths 1, 2, 4 and 8. Role (from name): a write probe. The store value and the way a fault reaches this routine are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x18310
  **Inferred.** `earlybadaddr` (356 bytes) is a read probe with widths of 1, 2 and 4 bytes only; the facts show no 8-byte case (branches at 0x1838c to 0x183bc). It stores to the global `nofault` at 0x1837c and 0x18444, and the facts do not show how that variable is read. It branches to `earlybaerror` at 0x1859c when bit 0x4000 of a register value is set (0x1840c) or when the word at 0xbfa000ec or 0xbfa000fc is nonzero (0x18420, 0x18434), and otherwise returns 0. The `cmn_err` call at 0x183d8 is on the path where a1 matched none of the widths 1, 2 and 4. Role (from name): a read probe for use before the full fault machinery is in place; the facts neither confirm nor rule that out.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x18524
  **Inferred.** `baerror` (120 bytes) is the entry that the probe routines branch to on an error. It stores to the two error words at 0xbfa000ec and 0xbfa000fc, reads 0xbfa00004, and tests the word at 0xffffa018 at 0x1856c. When that word is nonzero it stores a halfword at offset 0xc2 from the address that word holds (0x18574); when it is zero it stores a word at 0xffffa1d8 (0x18580). Both paths store a word at 0xa00002f0 and then return the constant 1 at 0x18594. Role (from name): the bus-error recording path. The constant 1 suggests a fault counted as handled, but the facts do not give the meaning of the stored values.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1859c
  **Inferred.** `earlybaerror` (108 bytes) is the early twin of `baerror`. It writes the same two error words and reads 0xbfa00004, writes the global `nofault` at 0x185e8, and writes a word at 0xa00002f0. It returns the constant 1 at 0x18600. Unlike `baerror`, the facts show no halfword write and no test of the word at 0xffffa018.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x18c14
  **Inferred.** `_ecc_errorvec` (24 bytes) does nothing but pass its four argument registers to `ecc_exception` with a direct jump at 0x18c24. Role (from name): an exception vector stub. The facts show only the jump, not how the vector is selected or how `ecc_exception` relates to `ecc_handler`.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1ed58
  **Inferred.** `VEC_dbe` (172 bytes) copies error state into globals: it loads 0xbfa000ec into `cpu_err_stat`, 0xbfa000fc into `gio_err_stat`, 0xbfa000e4 into `cpu_par_adr` and 0xbfa000f4 into `gio_par_adr`. It also reads 0xbfbd9900 and 0xbfbb0010 into `hpc3_ext_io` and `hpc3_bus_err_stat`, writes the two error words at 0xbfa000fc and 0xbfa000ec again, saves a doubleword at a0+0x140, and jumps to `VEC_trap` at 0x1edfc. The name and the global names suggest a data-bus-error vector.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x20c64
  **Inferred.** `VEC_ibe` (196 bytes) is the instruction-bus-error counterpart of `VEC_dbe`. It copies the same error words into the same globals and writes the error words back, then saves a doubleword at a0+0x140. It jumps to `VEC_trap` at 0x20d20 with a2 not set from the entry (unknown in the facts). The name suggests an instruction-bus-error vector, but the facts do not show how the two vectors are selected.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP22boot/ksync_metered.a(sema.o)` at 0x35b8
  **Inferred.** `_mraccess` (240 bytes) calls `wchanmeter` at 0x35d0 and branches on whether it returns nonzero (0x35d8). On the zero-return side it goes to 0x35e0 and calls `mr_access`; the facts do not list where the nonzero side rejoins. With a nonzero return it takes timestamps with `_get_timestamp` at 0x3678 and 0x35f0, updates a word at +0x138 with an `ll`/`sc` pair at 0x3650 and 0x365c, and records an event with `mrmeter_entry` at 0x3690 using type 5. Role (from name): a metering hook for a wait-channel meter. The event types and field meanings are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP22boot/ksync_metered.a(sema.o)` at 0x36e0
  **Inferred.** `_mrupdate` (220 bytes) calls `wchanmeter` at 0x36f8, `mr_update` at 0x3708, and `_get_timestamp` at 0x3718 and 0x37a4. It stores fields between offsets 0x140 and 0x178 and records an event with `mrmeter_entry` at 0x379c using type 6, where `_mraccess` uses type 5. The facts show a branch on bit 0x4 of a flags value at 0x3768, not its meaning. Role (from name): the update path of a wait-channel meter.
  Reviewed: 2026-11-12 (sonnet)

- `MgrasGeneralInterrupt` in `6.5.22/usr/cpu/sysgen/IP22boot/mgras.a(mgras.o)` at 0x7498
  **Inferred.** `MgrasGeneralInterrupt` (752 bytes) indexes a per-board table `MgrasBoards` with a 36-byte stride and reads status words at offsets 0x70100 and 0x70104 from each board, testing bits 0x40 and 0x1. It calls `vsema` at 0x75d4, 0x761c and 0x7640. Four message IDs, 0xaad to 0xab0, go out through `cmn_err_tag` at 0x7694 to 0x76f4, each after a branch on a flag or status bit, and `mgras_error` is called at 0x7700. `MgrasSchedSwapBufFinish` is called at 0x7724. Role (from name): the general interrupt handler of an MGRAS graphics board, with semaphore releases, error reports and a buffer-swap completion. Which board status bit means what is not established by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP22boot/os.a(pda.o)` at 0x1594
  **Inferred.** `doacvec` (96 bytes) reads bytes and words from absolute 0xffffa070 to 0xffffa090 and stores to 0xffffa03e, 0xffffa071 and 0xffffa090. It calls `panicspin` at 0x15a4 when the byte at 0xffffa070 is nonzero (0x159c fall), `tlbflush_rand` with a0 = 1 at 0x15cc when the word at 0xffffa074 is nonzero (0x15c4 fall), and `tlbflush` at 0x15e0 with the doubleword at 0xffffa090 as a0 when that doubleword is nonzero (0x15d8 fall). The name does not say what "cvec" refers to, so the purpose stays open. The TLB flush and panic calls are the firm facts.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP22boot/os.a(syssgi.o)` at 0xcc
  **Inferred.** `syssgi` (10944 bytes, 0xcc to 0x2b8c) calls `get_current_abi` at 0xec and reads the word at a0. When that word is below 0x4d7 (0x11c fall) it indexes a .rodata table with it at 0x124 and jumps through that table at 0x128. When it is 0x4d7 or more (0x11c taken) the one explicit return on the followed path gives the constant 0x16 (22) at 0x158; that this is the usual invalid-argument errno is an inference the facts do not establish. The jump-table targets are unresolved, and the code from 0x160 to 0x28ec and from 0x2908 to 0x2b88 is not reached by any path the disassembly followed, so most of the routine is open. A branch at 0x100 to a block at 0x28f0 calls `sat_never` and `sat_set_subsysnum` when `sat_enabled` is nonzero. Role (from name): the entry of the IRIX `syssgi` system call.
  Reviewed: 2026-11-12 (sonnet)
