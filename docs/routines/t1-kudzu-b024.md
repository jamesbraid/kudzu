# Tier-1 routine readings, kudzu batch 24

These role statements cover 25 routines from the tier-1 reading round, at positions 600 to 624 of the ranked list. Each one is drawn from its disassembly: the registers, calls and constants at the cited addresses. They describe software behavior in the IRIX 6.5.30 IP27 kernel image, not hardware behavior. Where a purpose depends on a symbol name or on a value the facts do not pin down, the entry says so.

- `counter_intr` in `6.5.30/unix.IP27` at 0xc0000000000457a0
  **Inferred.** The routine tests bit 0x20 of the 8-byte word at -0x3d80 (the disassembler's form for an absolute address) and dispatches to `frs_handle_counter_intr` at 0xc0000000003c38a4, `hwperf_overflowed_intr` at 0xc000000000057c9c, `l2_sbe_intr` at 0xc0000000000556b0 and `hwperf_intr` at 0xc0000000000588e8. It re-arms the counter through `resetcounter` at 0xc0000000000535d4, calls `clock` at 0xc0000000001b22d4, and logs two timestamp events with tags 0x6c and 0x65 through `log_tstamp_event` at 0xc000000000238b24. The meaning of bit 0x20 and of the `cmn_err` severity at 0xc0000000001fcbd4 are not established by these facts.

- `doadjtime` in `6.5.30/unix.IP27` at 0xc000000000047128
  **Inferred.** The routine raises the interrupt level with `spl65` at 0xc00000000003b3a4 and takes the lock at 0xc0000000014d2b3c with `nested_spinlock` at 0xc000000000286420. It calls `__nanotime` at 0xc000000000047314 on both sides of its update and compares against the constant 0xf4240 (one million), which suggests a microsecond carry. It releases the lock with `mutex_spinunlock` at 0xc000000000286400, passing the word at 0xc0000000014d2b4c plus one on one path. The facts show no store of the adjusted time, so the arithmetic itself is not visible.

- `__nanotime` in `6.5.30/unix.IP27` at 0xc000000000047314
  **Inferred.** The routine reads the 8-byte word at 0x9200000001030100 and takes the lock at 0xc0000000014d2b3c with `mutex_spinlock_spl` at 0xc000000000286360. It updates an 8-byte cell at 0xc0000000014de518 and a 4-byte cell at 0xc0000000014de510, carrying at the constant 0x3b9aca00 (one billion). The name suggests a nanosecond clock and the 0x92... address looks like its counter source, but the facts do not show what that address is or what the cells hold.

- `findcpufreq_cycles` in `6.5.30/unix.IP27` at 0xc000000000047aa4
  **Inferred.** The routine raises the level with `spl1` at 0xc00000000003b30c and clears the count with `_set_count` at 0xc0000000000535c8. It then waits until the word at 0x9200000001030100 has moved forward by 0x1312d0 (1,250,000 ticks), reads the count back with `_get_count` at 0xc0000000000535b0, and passes it to `cpu_cycles_adjust` at 0xc00000000001e7e8 before `splx` at 0xc00000000003b3f4. The name and the wait loop suggest a CPU cycle-rate measurement, but the facts do not confirm the unit or how the return value is used.

- `acktmoclock` in `6.5.30/unix.IP27` at 0xc000000000048550
  **Inferred.** The routine loads the halfword at 0xffffffffffffc420 and a flag byte at offset 0x520 of the table entry at 0xc0000000014d5028. It stores doublewords to 0x9200000001000128 and 0x9200000001000120, the first only when the flag is nonzero, and returns the halfword shifted left by four. It makes no calls, so what the stored values mean is not visible. The name points at the clock tick acknowledge, which is an inference.

- `ackkgclock` in `6.5.30/unix.IP27` at 0xc000000000048588
  **Inferred.** The routine has the same shape as `acktmoclock`: a halfword load from 0xffffffffffffc420, the flag byte at offset 0x520 of the table entry at 0xc0000000014d5028, two doubleword stores to 0x9200000001000138 and 0x9200000001000130, and a return of the shifted halfword. It makes no calls. Its role as the kernel-clock acknowledge comes from the name and its sibling, not from these facts.

- `startkgclock` in `6.5.30/unix.IP27` at 0xc0000000000485c0
  **Inferred.** The routine takes the lock at 0xc0000000014d2ba0 with `mutex_spinlock_spl` and maps a CPU with `cnodetocpu` at 0xc000000000026fc8. It calls `setkgvector` at 0xc000000000039578 with `prof_intr` at 0xc000000000049360 as its first argument, stores to 0x9200000001000158, 0x9200000001000118 and 0x9200000001000150, and releases the lock with `mutex_spinunlock`. Installing the kernel-clock vector comes from the name and the vector argument; the facts do not show what the stores configure.

- `slowkgclock` in `6.5.30/unix.IP27` at 0xc0000000000486d8
  **Inferred.** The routine takes the lock at 0xc0000000014d2ba0 and reads the halfword at 0xffffffffffffc420 and the flag byte at offset 0x520 of the table entry at 0xc0000000014d5028. It updates the word at 0xffffffffffffc534 and a word at offset 0x4930 through the pointer at [-0x3c00]:8, the same field `prof_intr` and `startkgclock` use. It then calls `ackkgclock` at 0xc000000000048588 and `spinlock_init` at 0xc00000000028618c on the same lock; the facts do not show why the lock is re-initialized on that path or what condition leads to the call.

- `prof_intr` in `6.5.30/unix.IP27` at 0xc000000000049360
  **Inferred.** The routine reads doublewords at 0xffffffffffffc280 and 0xffffffffffffc400, a word at 0xffffffffffffc534, halfwords at 0xffffffffffffc420 and 0xffffffffffffc422, and a word through the pointer at [-0x3c00]:8 +0x4930. It calls `fastick_maint` at 0xc000000000045ec4, then `cnode_subnode_to_cpu` at 0xc000000000027078, and logs two timestamp events with tags 0x6e and 0x65 through `log_tstamp_event` at 0xc000000000238b24. It stores a doubleword to 0x9200000001000118 and tests the same bit 0x20 of the word at -0x3d80 that `counter_intr` tests. The name and the event tags suggest profiling, but the facts do not show what is counted.

- `mdir_xlate_addr_mem` in `6.5.30/unix.IP27` at 0xc000000000049974
  **Inferred.** The routine reads an 8-byte word at 0x9200000001200018 and a 4-byte word at 0x9000000010000090. It then calls `sprintf` at 0xc0000000001fd9d0 four times, each writing into its first argument with a different format string at 0xc00000000140f370, 0xc00000000140f3a0, 0xc00000000140f350 and 0xc00000000140f388. The output is text, so the translation step the name suggests is not visible; the facts show a formatting path only.

- `mdir_xlate_addr_pdir` in `6.5.30/unix.IP27` at 0xc000000000049aec
  **Inferred.** The routine reads an 8-byte word at 0x9200000001200018 and makes three `sprintf` calls at 0xc0000000001fd9d0, with format strings at 0xc00000000140f3d0, 0xc00000000140f3e8 and 0xc00000000140f3b8. Its branches test the sign of its third argument, and it returns the last `sprintf` result. Like the memory variant it formats text, and the facts show no address arithmetic.

- `mdir_xlate_addr_sdir` in `6.5.30/unix.IP27` at 0xc000000000049bf0
  **Inferred.** The routine reads 8 bytes at 0x9200000001200018 and 4 bytes at 0x9000000010000090, then makes four `sprintf` calls at 0xc0000000001fd9d0 with format strings at 0xc00000000140f408, 0xc00000000140f3d0, 0xc00000000140f3f8 and 0xc00000000140f420. Its branches test the word at 0x9000000010000090 and the sign of its third argument. It has the same formatting-only shape as the other two variants, and the facts do not show what distinguishes the three.

- `_ecc_errorvec` in `6.5.30/unix.IP27` at 0xc00000000004d4b0
  **Inferred.** The routine stores doublewords to the low absolute addresses 0x548, 0x550 and 0x480, loads a doubleword from 0xfd8 and branches on it. It then jumps through a register at 0xc00000000004d4f8, passing its four argument registers on, and the facts do not resolve that target. The shape fits an error-vector stub that saves state and hands off, but the handler is not visible here.

- `earlybadaddr` in `6.5.30/unix.IP27` at 0xc0000000000515cc
  **Inferred.** The routine loads a byte, a halfword or a word from its first argument, chosen by whether its second argument is 1, 2 or 4. It stores a 4-byte word twice to 0xc0000000014d3904, which the tool labels as a nofault cell. On one path it calls `cmn_err` at 0xc000000000051660 with a string at 0xc000000001419bb3, and it returns 0. The probe reading comes from the size dispatch and the loads, and the name suggests early boot; the facts do not show what the message reports.

- `baerror` in `6.5.30/unix.IP27` at 0xc000000000051718
  **Inferred.** The routine loads an 8-byte pointer from 0xffffffffffffc020. When that pointer is nonzero, it stores a halfword at offset 0xf6 from it, then stores a word to 0xffffffffffffc484 and returns 1. It reads like a bus-error record update, but the facts do not identify the record layout or what the pointer refers to.

- `earlybaerror` in `6.5.30/unix.IP27` at 0xc000000000051748
  **Inferred.** The routine stores a word to 0xc0000000014d3904, the same nofault cell that `earlybadaddr` writes, and returns 1. The name makes it a short early-boot variant of `baerror`, and the shared cell suggests a fault flag. The facts do not name the cell's meaning.

- `cpu_waitforreset` in `6.5.30/unix.IP27` at 0xc000000000053678
  **Inferred.** The routine is 8 bytes long, and its one branch at 0xc000000000053678 targets its own address and is always taken, so it spins in place. The facts establish that spin. The name suggests a wait for a CPU reset, which the facts do not confirm, so the purpose is inferred.

- `VEC_dbe` in `6.5.30/unix.IP27` at 0xc000000000053ad0
  **Inferred.** The routine loads a halfword from 0xffffffffffffc420, stores a doubleword at offset 0x140 from its first argument, and jumps to `VEC_trap` at 0xc000000000043b20 with its arguments unchanged. It is a short vector stub. The name suggests a data-bus-error vector, but the meaning of the halfword and of the stored value is not established.

- `badaddr` in `6.5.30/unix.IP27` at 0xc00000000005765c
  **Inferred.** The routine passes a pointer into its stack frame (sp-48) and its other arguments to `probe_common` at 0xc00000000005772c, then returns that result. It is a thin wrapper, and the name suggests a read probe. The access width and direction live in `probe_common`, which is not disassembled here, so they are not visible in this routine.

- `badaddr_val` in `6.5.30/unix.IP27` at 0xc00000000005768c
  **Inferred.** The routine makes the same `probe_common` call as `badaddr`, at 0xc00000000005772c with the stack-frame pointer as its first argument, and returns the result. The name suggests a value-returning probe, but these facts show no difference from `badaddr`, so any difference lies in `probe_common`.

- `wbadaddr` in `6.5.30/unix.IP27` at 0xc0000000000576bc
  **Inferred.** The routine calls `probe_common` at 0xc00000000005772c with the same argument shape as the read probes and returns its result. The name suggests a write probe, but this routine shows nothing write-specific, so any distinction sits in `probe_common`.

- `VEC_ibe` in `6.5.30/unix.IP27` at 0xc00000000005bba0
  **Inferred.** The routine loads doublewords from addresses the facts do not resolve, stores one doubleword through a pointer it does not resolve, and loads the halfword at 0xffffffffffffc420. It branches on the sign of the return-address register, stores a doubleword at offset 0x140 from its first argument, and jumps to `VEC_trap` at 0xc000000000043b20. It parallels `VEC_dbe`, and the name suggests an instruction-bus-error vector.

- `cpuprom_detailed_inventory_info_add` in `6.5.30/unix.IP27` at 0xc00000000006b998
  **Inferred.** The routine gets a CNode id from `hubdev_cnodeid_get` at 0xc00000000004c20c and allocates a record with `klhwg_invent_alloc` at 0xc000000000040a58, using the arguments 0x15 and 0x20. It fills four fields of that record, two of them from the words at 0x90000000100000a0 and 0x90000000100000a4. It then attaches the record to the hardware graph with `hwgraph_info_add_LBL` at 0xc00000000029526c, under the label at 0xc0000000003c53d0, and exports it with `hwgraph_info_export_LBL` at 0xc0000000002953d0. The name says CPU PROM inventory, and the facts show the registration sequence, but not what the copied words mean.

- `pcireg_id_get` in `6.5.30/unix.IP27` at 0xc00000000007ec2c
  **Inferred.** The routine calls `panic` at 0xc0000000001fdd6c with the string at 0xc0000000014143d0 when its first argument is null. It reads a halfword at offset 0x72, a word at offset 0x4 and a doubleword at offset 0x8 from that argument. It then compares values against a fixed set of constants (0xe000, 0xe010, 0xd102, 0xd112, 0xd002, 0xc002, 0x36 and 0x24), returning 0 or an unresolved value. The name suggests a PCI ID read, but the facts do not show which register sits at each offset.

- `pcireg_stat_get` in `6.5.30/unix.IP27` at 0xc00000000007ed8c
  **Inferred.** The routine has the same shape as `pcireg_id_get`: a null check that panics with the string at 0xc0000000014143f8, the same reads at offsets 0x72, 0x4 and 0x8, and the same constant comparisons. The facts do not separate the two beyond the panic string, so the status-register reading comes only from the name.

Dropped entries: none.
