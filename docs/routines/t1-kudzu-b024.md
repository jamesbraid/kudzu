# Tier-1 routine readings, kudzu batch 24

These role statements cover 25 routines from the tier-1 reading round, at positions 600 to 624 of the ranked list. Each one is drawn from its disassembly: the registers, calls and constants at the cited addresses. They describe software behavior in the IRIX 6.5.30 IP27 kernel image, not hardware behavior. Where a purpose depends on a symbol name or on a value the facts do not pin down, the entry says so.

- `counter_intr` in `6.5.30/unix.IP27` at 0xc0000000000457a0
  **Inferred.** The routine tests bit 0x20 of the 8-byte word at -0x3d80 (the disassembler's form for an absolute address) and calls `frs_handle_counter_intr` at 0xc0000000003c38a4, `hwperf_overflowed_intr` at 0xc000000000057c9c, `l2_sbe_intr` at 0xc0000000000556b0 and `hwperf_intr` at 0xc0000000000588e8 on different paths. It calls `resetcounter` at 0xc0000000000535d4 from two sites, calls `clock` at 0xc0000000001b22d4, and logs two timestamp events with tags 0x6c and 0x65 through `log_tstamp_event` at 0xc000000000238b24. Role (from name): a counter interrupt handler; the facts do not show what is counted or what the resetcounter calls re-arm. The meaning of bit 0x20 and of the `cmn_err` severity at 0xc0000000001fcbd4 are not established by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.30/unix.IP27` at 0xc000000000047128
  **Inferred.** The routine raises the interrupt level with `spl65` at 0xc00000000003b3a4, calls `__nanotime` at 0xc000000000047314 and takes the lock at 0xc0000000014d2b3c with `nested_spinlock` at 0xc000000000286420. The `__nanotime` call sits before the lock on each of the two paths that reach it; the facts show no call to it after the lock is taken. It compares against the constant 0xf4240 (one million), which suggests a microsecond carry. It releases the lock with `mutex_spinunlock` at 0xc000000000286400, passing the word at 0xc0000000014d2b4c plus one on one path. The facts show no store of an adjusted time, so the arithmetic itself is not visible.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.30/unix.IP27` at 0xc000000000047314
  **Inferred.** The routine reads the 8-byte word at 0x9200000001030100 and takes the lock at 0xc0000000014d2b3c with `mutex_spinlock_spl` at 0xc000000000286360. It updates an 8-byte cell at 0xc0000000014de518 and a 4-byte cell at 0xc0000000014de510, carrying at the constant 0x3b9aca00 (one billion). The name suggests a nanosecond clock and the 0x92... address looks like its counter source, but the facts do not show what that address is or what the cells hold.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.30/unix.IP27` at 0xc000000000047aa4
  **Inferred.** The routine raises the level with `spl1` at 0xc00000000003b30c and clears the count with `_set_count` at 0xc0000000000535c8. It then waits until the word at 0x9200000001030100 has moved forward by 0x1312d0 (1,250,000 ticks), reads the count back with `_get_count` at 0xc0000000000535b0, and passes it to `cpu_cycles_adjust` at 0xc00000000001e7e8 before `splx` at 0xc00000000003b3f4. The name and the wait loop suggest a CPU cycle-rate measurement, but the facts do not confirm the unit or how the return value is used.
  Reviewed: 2026-11-12 (sonnet)

- `acktmoclock` in `6.5.30/unix.IP27` at 0xc000000000048550
  **Inferred.** The routine loads the halfword at 0xffffffffffffc420 and a flag byte at offset 0x520 of the table entry at 0xc0000000014d5028. It stores a zero doubleword to 0x9200000001000128 when the flag byte is nonzero and to 0x9200000001000120 when it is zero, so exactly one of the two stores runs. It returns the halfword shifted left by four. It makes no calls, so what the stores mean is not visible. The name points at the clock tick acknowledge, which is an inference.
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.30/unix.IP27` at 0xc000000000048588
  **Inferred.** The routine has the same shape as `acktmoclock`: a halfword load from 0xffffffffffffc420, the flag byte at offset 0x520 of the table entry at 0xc0000000014d5028, a zero doubleword store to 0x9200000001000138 when the flag byte is nonzero or to 0x9200000001000130 when it is zero (exactly one runs), and a return of the shifted halfword. It makes no calls. Its role as the kernel-clock acknowledge comes from the name and its sibling, not from these facts.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.30/unix.IP27` at 0xc0000000000485c0
  **Inferred.** The routine takes the lock at 0xc0000000014d2ba0 with `mutex_spinlock_spl`. When the word at 0xc0000000014ca308 is nonzero it continues with several loads and stores, including a store of 0x1 to 0x9200000001000158 when the flag byte is nonzero, a doubleword store to 0x9200000001000118, and a `cnodetocpu` call at 0xc000000000026fc8 on one branch. When that word is zero it stores 0x1 to 0x9200000001000150 and calls `setkgvector` at 0xc000000000039578 with `prof_intr` at 0xc000000000049360 as its first argument. Both paths release the lock with `mutex_spinunlock`. Installing the kernel-clock vector comes from the name and the vector argument; the facts do not show what the stores configure.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.30/unix.IP27` at 0xc0000000000486d8
  **Inferred.** The routine reads the halfword at 0xffffffffffffc420 and the flag byte at offset 0x520 of the table entry at 0xc0000000014d5028. When the word at 0xc0000000014ca310 is nonzero it takes the lock at 0xc0000000014d2ba0 with `mutex_spinlock_spl`, and some paths release it early with `mutex_spinunlock`. The path that continues stores a word at 0xffffffffffffc534 and decrements a word at offset 0x4930 through the pointer at [-0x3c00]:8 (the same field `prof_intr` and `startkgclock` use), releases the lock, and calls `ackkgclock` at 0xc000000000048588. On a separate path, reached when the word at 0xc0000000014ca310 is zero, it calls `spinlock_init` at 0xc00000000028618c on the same lock without taking it first. The facts do not show why the lock is re-initialized on that path.
  Reviewed: 2026-11-12 (sonnet)

- `prof_intr` in `6.5.30/unix.IP27` at 0xc000000000049360
  **Inferred.** The routine reads doublewords at 0xffffffffffffc280 and 0xffffffffffffc400, a word at 0xffffffffffffc534, halfwords at 0xffffffffffffc420 and 0xffffffffffffc422, and a word through the pointer at [-0x3c00]:8 +0x4930. It tests bit 0x20 of the word at -0x3d80, the same word `counter_intr` tests. `fastick_maint` at 0xc000000000045ec4 is called on the path where that bit is clear. `cnode_subnode_to_cpu` at 0xc000000000027078 is called only when the word at -0x3acc equals 0x4e2. It also calls `log_tstamp_event` at 0xc000000000238b24 twice, with tags 0x6e and 0x65. It stores a doubleword to 0x9200000001000118. The name and the event tags suggest profiling, but the facts do not show what is counted.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_mem` in `6.5.30/unix.IP27` at 0xc000000000049974
  **Inferred.** The routine reads an 8-byte word at 0x9200000001200018 and a 4-byte word at 0x9000000010000090. It has four `sprintf` call sites at 0xc0000000001fd9d0, on different paths, each writing into the first argument with a different format string at 0xc00000000140f370, 0xc00000000140f3a0, 0xc00000000140f350 or 0xc00000000140f388. The output is text, so the translation step the name suggests is not visible; the facts show a formatting path only.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_pdir` in `6.5.30/unix.IP27` at 0xc000000000049aec
  **Inferred.** The routine reads an 8-byte word at 0x9200000001200018 and has three `sprintf` call sites at 0xc0000000001fd9d0, on separate paths, with format strings at 0xc00000000140f3d0, 0xc00000000140f3e8 and 0xc00000000140f3b8. Its branches test the sign of its third argument. One path returns the `sprintf` result directly. Like the memory variant it formats text, and the facts show no address arithmetic.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_sdir` in `6.5.30/unix.IP27` at 0xc000000000049bf0
  **Inferred.** The routine reads 8 bytes at 0x9200000001200018 and 4 bytes at 0x9000000010000090, then has four `sprintf` call sites at 0xc0000000001fd9d0, on different paths, with format strings at 0xc00000000140f408, 0xc00000000140f3d0, 0xc00000000140f3f8 and 0xc00000000140f420. Its branches test the word at 0x9000000010000090 and the sign of its third argument. It has the same formatting-only shape as the other two variants, and the facts do not show what distinguishes the three.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.30/unix.IP27` at 0xc00000000004d4b0
  **Inferred.** The routine stores doublewords to the low absolute addresses 0x548, 0x550 and 0x480, loads a doubleword from 0xfd8 and branches on it. It then jumps through a register at 0xc00000000004d4f8, passing its four argument registers on, and the facts do not resolve that target. The shape fits an error-vector stub that saves state and hands off, but the handler is not visible here.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.30/unix.IP27` at 0xc0000000000515cc
  **Inferred.** The routine loads a byte, a halfword or a word from its first argument, chosen by whether its second argument is 1, 2 or 4. It stores a 4-byte word twice to 0xc0000000014d3904, which the tool labels as a nofault cell. On one path it calls `cmn_err` at 0xc000000000051660 with a string at 0xc000000001419bb3, and it returns 0. The probe reading comes from the size dispatch and the loads, and the name suggests early boot; the facts do not show what the message reports.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.30/unix.IP27` at 0xc000000000051718
  **Inferred.** The routine loads an 8-byte pointer from 0xffffffffffffc020. When that pointer is nonzero, it first stores a halfword at offset 0xf6 from it. On both paths it then stores a word to 0xffffffffffffc484 and returns 1. It reads like a bus-error record update, but the facts do not identify the record layout or what the pointer refers to.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.30/unix.IP27` at 0xc000000000051748
  **Inferred.** The routine stores a word to 0xc0000000014d3904, the same nofault cell that `earlybadaddr` writes, and returns 1. The name makes it a short early-boot variant of `baerror`, and the shared cell suggests a fault flag. The facts do not name the cell's meaning.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.30/unix.IP27` at 0xc000000000053678
  **Inferred.** The routine is 8 bytes long, and its one branch at 0xc000000000053678 targets its own address and is always taken, so it spins in place. The facts establish that spin. The name suggests a wait for a CPU reset, which the facts do not confirm, so the purpose is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.30/unix.IP27` at 0xc000000000053ad0
  **Inferred.** The routine loads a halfword from 0xffffffffffffc420, stores a doubleword at offset 0x140 from its first argument, and jumps to `VEC_trap` at 0xc000000000043b20 with its arguments unchanged. It is a short vector stub. The name suggests a data-bus-error vector, but the meaning of the halfword and of the stored value is not established.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.30/unix.IP27` at 0xc00000000005765c
  **Inferred.** The routine passes a pointer into its stack frame (sp-48) and its other arguments to `probe_common` at 0xc00000000005772c, then returns that result. It is a thin wrapper, and the name suggests a read probe. The access width and direction live in `probe_common`, which is not disassembled here, so they are not visible in this routine.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.30/unix.IP27` at 0xc00000000005768c
  **Inferred.** The routine makes the same `probe_common` call as `badaddr`, at 0xc00000000005772c with the stack-frame pointer as its first argument, and returns the result. The name suggests a value-returning probe, but these facts show no difference from `badaddr`, so any difference lies in `probe_common`.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.30/unix.IP27` at 0xc0000000000576bc
  **Inferred.** The routine calls `probe_common` at 0xc00000000005772c with the same argument shape as the read probes and returns its result. The name suggests a write probe, but this routine shows nothing write-specific, so any distinction sits in `probe_common`.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.30/unix.IP27` at 0xc00000000005bba0
  **Inferred.** The routine loads doublewords from addresses the facts do not resolve, stores one doubleword through a pointer it does not resolve, and loads the halfword at 0xffffffffffffc420. It branches on the sign of the return-address register, stores a doubleword at offset 0x140 from its first argument, and jumps to `VEC_trap` at 0xc000000000043b20. It parallels `VEC_dbe`, and the name suggests an instruction-bus-error vector.
  Reviewed: 2026-11-12 (sonnet)

- `cpuprom_detailed_inventory_info_add` in `6.5.30/unix.IP27` at 0xc00000000006b998
  **Inferred.** The routine gets a CNode id from `hubdev_cnodeid_get` at 0xc00000000004c20c and allocates a record with `klhwg_invent_alloc` at 0xc000000000040a58, using the arguments 0x15 and 0x20. It fills four fields of that record, two of them from the words at 0x90000000100000a0 and 0x90000000100000a4. It then attaches the record to the hardware graph with `hwgraph_info_add_LBL` at 0xc00000000029526c, under the label at 0xc0000000003c53d0, and exports it with `hwgraph_info_export_LBL` at 0xc0000000002953d0. The name says CPU PROM inventory, and the facts show the registration sequence, but not what the copied words mean.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_id_get` in `6.5.30/unix.IP27` at 0xc00000000007ec2c
  **Inferred.** The routine calls `panic` at 0xc0000000001fdd6c with the string at 0xc0000000014143d0 when its first argument is null. It reads a halfword at offset 0x72, a word at offset 0x4 and a doubleword at offset 0x8 from that argument. It then compares values against a fixed set of constants (0xe000, 0xe010, 0xd102, 0xd112, 0xd002, 0xc002, 0x36 and 0x24), returning 0 or an unresolved value. The name suggests a PCI ID read, but the facts do not show which register sits at each offset.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_stat_get` in `6.5.30/unix.IP27` at 0xc00000000007ed8c
  **Inferred.** The routine has the same shape as `pcireg_id_get`: a null check that panics with the string at 0xc0000000014143f8, the same reads at offsets 0x72, 0x4 and 0x8, and the same constant comparisons. The facts do not separate the two beyond the panic string, so the status-register reading comes only from the name.
  Reviewed: 2026-11-12 (sonnet)

Dropped entries: none.
