# Routine role statements, kudzu batch 13

Routines 325 to 349 (0-based) of the ranked tier-1 list, all in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o`. Each entry lists the facts read from the binary's instructions (registers, calls, constants, branch structure), then a role statement that uses only those facts and the binary's own symbol names. Every grade is inferred: the facts show what the code does, not why, so each statement leaves its purpose partly open where the facts do not settle it.

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x207b4
  **Inferred.** Compares a loop counter against `maxcpus` (branches at 0x207d0 and 0x20818) and calls `get_cpu_slice` (0x207e4) and `cpu_enabled` (0x20830). The branch at 0x20824 compares the halfword at `[-0x3be0]:2`, probably the running CPU's number, with `s0`; when they are equal it jumps to 0x20810 and `cpu_enabled` is not called on that pass. The initial value of `s0` and the start of the loop are not shown. It stores one doubleword to the fixed table address 0x9200000001800090 (0x20808). The name suggests it stops the other processors, but the facts do not say whether that store is the stop mechanism: the only calls are `get_cpu_slice` and `cpu_enabled`.
  ABI: n64.
  Range: 0x207b4-0x208a0 (236 bytes).
  It occupies 236 bytes.
  2 such `lh` accesses load 2 bytes from abs 0xffffffffffffc420, including `lh` at 0x207e0, `lh` at 0x20820.
  3 such `ld` accesses load 8 bytes from ?, including `ld` at 0x207ec, `ld` at 0x2084c, `ld` at 0x2086c.
  The `lh` at 0x207f0 loads 2 bytes from ?.
  The `sd` at 0x20808 stores 8 bytes to table(0x9200000001800090) +0x0.
  The `jal` at 0x207e4 calls get_cpu_slice with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x20830 calls cpu_enabled with a0=?, a1=[-0x3be0]:2, a2=[sym(maxcpus)], a3=?.
  The `blez` branch at 0x207d0 targets 0x20884, is taken when a2(=[sym(maxcpus)]) <= 0 (signed), and falls through when a2(=[sym(maxcpus)]) > 0 (signed).
  The `beq` branch at 0x207dc targets 0x20824 and is always taken.
  The `beq` branch at 0x20818 targets 0x20884, is taken when s0(?) >= a2(=[sym(maxcpus)]) (signed), and falls through when s0(?) < a2(=[sym(maxcpus)]) (signed).
  The `beq` branch at 0x20824 targets 0x20810, is taken when a1(=[-0x3be0]:2) == s0(?), and falls through when a1(=[-0x3be0]:2) != s0(?).
  The `beq` branch at 0x20844 targets 0x2080c, is taken when v0(=ret(cpu_enabled)) == 0, and falls through when v0(=ret(cpu_enabled)) != 0.
  The `beq` branch at 0x20854 targets 0x2080c, is taken when t9(?) == 0, and falls through when t9(?) != 0.
  The `beq` branch at 0x2085c targets 0x207e4, is taken when ra(=-0x1) == s2(=a0), and falls through when ra(=-0x1) != s2(=a0).
  The `bne` branch at 0x20874 targets 0x207e4, is taken when (at(?) & v0(?)) != 0, and falls through when (at(?) & v0(?)) == 0.
  The `beq` branch at 0x2087c targets 0x20810 and is always taken.
  The return at 0x20898 has v0 = ?.
  Reviewed: 2026-11-12 (sonnet)

- `get_my_slotid` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x238e4
  **Inferred.** Loads a doubleword from the fixed address 0x9200000001220048 (0x238e8) and calls `hub_slotbits_to_slot` (0x238f4) with the caller's a1 to a3, returning that call's value. The facts do not show which argument receives the loaded value (the call's a0 is shown as unknown). The name suggests the running CPU's slot number, which fits a helper that converts a hub field to a slot, but the loaded field is not identified.
  ABI: n64.
  Range: 0x238e4-0x23908 (36 bytes).
  It occupies 36 bytes.
  The `ld` at 0x238e8 loads 8 bytes from abs 0x9200000001220048.
  The `jal` at 0x238f4 calls hub_slotbits_to_slot with a0=?, a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The return at 0x23900 has v0 = ret(hub_slotbits_to_slot).
  Reviewed: 2026-11-12 (sonnet)

- `llp_pfail_intr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x24d24
  **Inferred.** Compares the result of `msc_owner_cpu` (0x24d84) with the halfword at `[-0x3be0]:2` (branch at 0x24d8c), and makes `vector_read` and `vector_write` calls with constant arguments (a0 of 0x0 or 0x6, a2 of 0x0, 0x8, 0x30 or 0x600110), `us_delay` calls with a0 of 0x64 or 0x12c, and one `llp_pfail_barrier` call (0x24dc4). Many of these calls are conditional (for example the branches at 0x24dcc, 0x24dec, 0x24ec4 and 0x24f40). It also tests bit 0x2000000000 against doublewords at 0x9200000001608088 in a loop. The name points to link power-fail handling, but nothing in the facts identifies the vector arguments or the meaning of the tested bit.
  ABI: n64.
  Range: 0x24d24-0x2503c (792 bytes).
  It occupies 792 bytes.
  The `lh` at 0x24d2c loads 2 bytes from abs 0xffffffffffffc420.
  3 such `lbu` accesses load 1 byte from abs 0xffffffffffffc523, including `lbu` at 0x24d34, `lbu` at 0x24d5c, `lbu` at 0x24e98.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0x24d88, `ld` at 0x24d98.
  6 such `ld` accesses load 8 bytes from abs 0x9200000001608088, including `ld` at 0x24e10, `ld` at 0x24e30, `ld` at 0x24e40.
  The `ld` at 0x24ee8 loads 8 bytes from abs 0x9200000001600110.
  The `sd` at 0x24f08 stores 8 bytes to abs 0x9200000001600110.
  The `sd` at 0x24f18 stores 8 bytes to abs 0x9200000001600008.
  The `jal` at 0x24d84 calls msc_owner_cpu [0x24a50] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x24dac calls vector_read with a0=0x0, a1=0x0, a2=0x0, a3=sp-64.
  The `jal` at 0x24dc4 calls llp_pfail_barrier with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x24dd4 calls us_delay with a0=0x64, a1=?, a2=?, a3=?.
  The `jal` at 0x24df4 calls us_delay with a0=0x64, a1=?, a2=?, a3=sp-48.
  The `jal` at 0x24dfc calls us_delay with a0=0x64, a1=?, a2=?, a3=?.
  The `jal` at 0x24e88 calls us_delay with a0=0x12c, a1=?, a2=?, a3=?.
  The `jal` at 0x24eb4 calls vector_read with a0=0x0, a1=0x0, a2=0x600110, a3=sp-56.
  The `jal` at 0x24edc calls vector_write with a0=0x0, a1=0x0, a2=0x600110, a3=?.
  The `jal` at 0x24f04 calls us_delay with a0=0x64, a1=-0x1001, a2=0x9200000001600110, a3=?.
  The `jal` at 0x24f24 calls vector_read with a0=0x6, a1=0x0, a2=0x0, a3=sp-48.
  The `jal` at 0x24f54 calls vector_read with a0=0x0, a1=0x0, a2=0x30, a3=sp-56.
  The `jal` at 0x24f78 calls vector_write with a0=0x0, a1=0x0, a2=0x30, a3=?.
  The `jal` at 0x24f80 calls us_delay with a0=0x64, a1=?, a2=?, a3=?.
  The `jal` at 0x24fa0 calls vector_write with a0=0x0, a1=0x0, a2=0x8, a3=0xe.
  The `jal` at 0x24fe0 calls vector_read with a0=0x6, a1=0x0, a2=0x30, a3=sp-56.
  The `jal` at 0x25004 calls vector_write with a0=0x6, a1=0x0, a2=0x30, a3=?.
  The `jal` at 0x25020 calls vector_write with a0=0x6, a1=0x0, a2=0x8, a3=0xe.
  The `blez` branch at 0x24d4c targets 0x24d9c, is taken when at(?) <= 0 (signed), and falls through when at(?) > 0 (signed).
  The `beq` branch at 0x24d54 targets 0x24d78 and is always taken.
  The `beq` branch at 0x24d74 targets 0x24d9c, is taken when s0(?) >= v0(?) (signed), and falls through when s0(?) < v0(?) (signed).
  The `bne` branch at 0x24d8c targets 0x24d5c, is taken when s1(=[-0x3be0]:2) != v0(=ret(msc_owner_cpu)), and falls through when s1(=[-0x3be0]:2) == v0(=ret(msc_owner_cpu)).
  The `beq` branch at 0x24d94 targets 0x24da0 and is always taken.
  The `beq` branch at 0x24dcc targets 0x24e88, is taken when s1(?) == 0, and falls through when s1(?) != 0.
  The `beq` branch at 0x24de4 targets 0x24e98, is taken when (a1(?) & 0xf) == 0, and falls through when (a1(?) & 0xf) != 0.
  The `beq` branch at 0x24dec targets 0x24f1c, is taken when s3(=0x1) == a1(?), and falls through when s3(=0x1) != a1(?).
  The `beq` branch at 0x24e28 targets 0x24e80, is taken when (a1(=0x2000000000) & a3(=[0x9200000001608088]:8)) == 0, and falls through when (a1(=0x2000000000) & a3(=[0x9200000001608088]:8)) != 0.
  The `beq` branch at 0x24e38 targets 0x24e80, is taken when (a1(=0x2000000000) & a3(=[0x9200000001608088]:8)) == 0, and falls through when (a1(=0x2000000000) & a3(=[0x9200000001608088]:8)) != 0.
  The `beq` branch at 0x24e48 targets 0x24e80, is taken when (a1(=0x2000000000) & a4(=[0x9200000001608088]:8)) == 0, and falls through when (a1(=0x2000000000) & a4(=[0x9200000001608088]:8)) != 0.
  The `beq` branch at 0x24e58 targets 0x24e80, is taken when (a1(=0x2000000000) & a5(=[0x9200000001608088]:8)) == 0, and falls through when (a1(=0x2000000000) & a5(=[0x9200000001608088]:8)) != 0.
  The `beq` branch at 0x24e68 targets 0x24e80, is taken when (a1(=0x2000000000) & a6(=[0x9200000001608088]:8)) == 0, and falls through when (a1(=0x2000000000) & a6(=[0x9200000001608088]:8)) != 0.
  The `bne` branch at 0x24e78 targets 0x24e30, is taken when (a1(=0x2000000000) & a7(=[0x9200000001608088]:8)) != 0, and falls through when (a1(=0x2000000000) & a7(=[0x9200000001608088]:8)) == 0.
  The `beq` branch at 0x24e90 targets 0x24e08 and is always taken.
  The `beq` branch at 0x24e9c targets 0x24dec, is taken when t0(=[-0x3add]:1) == 0, and falls through when t0(=[-0x3add]:1) != 0.
  The `beq` branch at 0x24ec4 targets 0x24fbc, is taken when s0(=[sym(.sbss)+160]) == s1(=0x8), and falls through when s0(=[sym(.sbss)+160]) != s1(=0x8).
  The `beq` branch at 0x24ef0 targets 0x24fc8, is taken when s0(=[sym(.sbss)+160]) == s1(=0x8), and falls through when s0(=[sym(.sbss)+160]) != s1(=0x8).
  The `beq` branch at 0x24f14 targets 0x24dfc and is always taken.
  The `beq` branch at 0x24f40 targets 0x24fd4, is taken when s3(=0x1) == at(?), and falls through when s3(=0x1) != at(?).
  The `beq` branch at 0x24f60 targets 0x24fb0, is taken when s0(=[sym(.sbss)+160]) == s1(=0x8), and falls through when s0(=[sym(.sbss)+160]) != s1(=0x8).
  The `beq` branch at 0x24f90 targets 0x25014, is taken when s3(=0x1) == at(?), and falls through when s3(=0x1) != at(?).
  The `beq` branch at 0x24fa8 targets 0x24dfc and is always taken.
  The `beq` branch at 0x24fb4 targets 0x24f70 and is always taken.
  The `beq` branch at 0x24fc0 targets 0x24ed4 and is always taken.
  The `beq` branch at 0x24fcc targets 0x24f00 and is always taken.
  The `beq` branch at 0x24fe8 targets 0x25030, is taken when s0(=[sym(.sbss)+160]) == s1(=0x8), and falls through when s0(=[sym(.sbss)+160]) != s1(=0x8).
  The `beq` branch at 0x2500c targets 0x24f4c and is always taken.
  The `beq` branch at 0x25028 targets 0x24f98 and is always taken.
  The `beq` branch at 0x25034 targets 0x24ffc and is always taken.
  The return at 0x24e80 has v0 = ret(us_delay).
  Reviewed: 2026-11-12 (sonnet)

- `force_system_shutdown` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x26558
  **Inferred.** Logs a message through `cmn_err` (0x26568), waits 0x7a120 units with `us_delay`, and calls `elsc_system_reset` (0x2657c). When that call returns a negative value (`bltz` at 0x26584), the routine prints through `printf`, waits 0xf4240 more units, and stores to the fixed address 0x9200000001600008 before returning. The name fits a forced shutdown and the order of steps matches it, but the facts do not say what a negative reset result means or what the store records.
  ABI: n64.
  Range: 0x26558-0x265bc (100 bytes).
  It occupies 100 bytes.
  The `sd` at 0x265b8 stores 8 bytes to abs 0x9200000001600008.
  The `jal` at 0x26568 calls cmn_err with a0=0x4, a1=sym(.rodata)+34712, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x26574 calls us_delay with a0=0x7a120, a1=?, a2=?, a3=?.
  The `jal` at 0x2657c calls elsc_system_reset with a0=sym(.bss)+37112, a1=?, a2=?, a3=?.
  The `jal` at 0x26598 calls printf with a0=sym(.rodata)+34712, a1=?, a2=?, a3=?.
  The `jal` at 0x265a4 calls us_delay with a0=0xf4240, a1=?, a2=?, a3=?.
  The `bltz` branch at 0x26584 targets 0x26594, is taken when v0(=ret(elsc_system_reset)) < 0 (signed), and falls through when v0(=ret(elsc_system_reset)) >= 0 (signed).
  The `beq` branch at 0x265b4 targets 0x26588 and is always taken.
  The return at 0x2658c has v0 = one of ret(elsc_system_reset), ?.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x29d58
  **Inferred.** Reads a doubleword from the fixed address 0x9200000001600000 (0x29d84) and another through an unnamed pointer (0x29da0), reads two halfwords, and calls `mapped_kernel_setup_tlb` (0x29dac). It then jumps through a register (`jr` at 0x29dcc) to a text address that the facts record only as a relocated offset, and the instructions after that jump are not reached from the entry. The name points at early kernel setup, which the TLB call supports, but the jump target and the loaded values are not identified.
  ABI: n64.
  Range: 0x29d58-0x29e28 (208 bytes).
  It occupies 208 bytes.
  The `ld` at 0x29d84 loads 8 bytes from abs 0x9200000001600000.
  The `ld` at 0x29da0 loads 8 bytes from ?.
  2 such `lh` accesses load 2 bytes from ?, including `lh` at 0x29da4, `lh` at 0x29da8.
  The `jal` at 0x29dac calls mapped_kernel_setup_tlb with a0=?, a1=?, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  Calls .text+171476 via reloc with a0=?, a1=?, a2=?, a3=? (`jr` at 0x29dcc).
  The instructions from 0x29dd4 to 0x29e24 are not reached by any path from the entry, and their branches are not listed.
  Reviewed: 2026-11-12 (sonnet)

- `counter_intr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2bdc4
  **Inferred.** Dispatches on flag tests to `frs_handle_counter_intr` (0x2bdec), `hwperf_overflowed_intr` (0x2be14), `l2_sbe_intr` (0x2be30) and `hwperf_intr` (0x2be48). It re-arms the counter through `resetcounter` (0x2be68, 0x2bf08), reads the count and compare values with `_get_count` and `_get_compare`, and logs timestamp events with tags 0x6c and 0x65. The name fits a count/compare timer interrupt, which these calls support. The `cmn_err` warning at 0x2c048 is gated by comparisons against `counter_intr_warn_cycles` (0x2beac) and two later comparisons (0x2c018, 0x2c028), and the facts do not settle the exact warning condition.
  ABI: n64.
  Range: 0x2bdc4-0x2c068 (676 bytes).
  It occupies 676 bytes.
  4 such `ld` accesses load 8 bytes from abs 0xffffffffffffc280, including `ld` at 0x2bdc4, `ld` at 0x2bdf4, `ld` at 0x2be74.
  2 such `lw` accesses load 4 bytes from abs 0xffffffffffffc6a0, including `lw` at 0x2bde0, `lw` at 0x2bfb4.
  The `lw` at 0x2be58 loads 4 bytes from abs 0xffffffffffffc048.
  2 such `lw` accesses load 4 bytes from abs 0xffffffffffffc564, including `lw` at 0x2be6c, `lw` at 0x2bf0c.
  The `ld` at 0x2be78 loads 8 bytes from abs 0xffffffffffffc5b8.
  The `ld` at 0x2be80 loads 8 bytes from abs 0x9200000001030100.
  The `lwu` at 0x2beb4 loads 4 bytes from abs 0xffffffffffffc564.
  4 such `lh` accesses load 2 bytes from abs 0xffffffffffffc420, including `lh` at 0x2bebc, `lh` at 0x2beec, `lh` at 0x2bf7c.
  The `lw` at 0x2bed8 loads 4 bytes from (sym(.bss)+39056 + ([-0x3be0]:2 << 0x2)) +0x10.
  The `lw` at 0x2bee8 loads 4 bytes from abs 0xffffffffffffc740.
  The `sd` at 0x2bf00 stores 8 bytes to abs 0xffffffffffffc5b8.
  The `sw` at 0x2bf04 stores 4 bytes to ?.
  The `lw` at 0x2bf34 loads 4 bytes from abs 0xffffffffffffc5cc.
  The `sw` at 0x2bf48 stores 4 bytes to abs 0xffffffffffffc5cc.
  The `lw` at 0x2bf4c loads 4 bytes from abs 0xffffffffffffc5c8.
  The `ld` at 0x2bf54 loads 8 bytes from abs 0xffffffffffffc5c0.
  The `sw` at 0x2bf5c stores 4 bytes to abs 0xffffffffffffc5c8.
  The `sd` at 0x2bf6c stores 8 bytes to abs 0xffffffffffffc5c0.
  The `ld` at 0x2c040 loads 8 bytes from a0 (arg0 at entry) +0xf0.
  The `ld` at 0x2c044 loads 8 bytes from a0 (arg0 at entry) +0x120.
  The `jal` at 0x2bdec calls frs_handle_counter_intr with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x2be14 calls hwperf_overflowed_intr with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x2be30 calls l2_sbe_intr with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x2be48 calls hwperf_intr with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0x2be68 calls resetcounter with a0=[-0x3a9c], a1=?, a2=?, a3=?.
  The `jal` at 0x2bf08 calls resetcounter with a0=[-0x3a9c], a1=?, a2=?, a3=?.
  The `jal` at 0x2bf10 calls clock with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0x2bf20 calls _get_count with a0=[-0x3a48]:8, a1=?, a2=?, a3=?.
  The `jal` at 0x2bf28 calls _get_compare with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x2bfa8 calls log_tstamp_event with a0=0x6c, a1=0x0, a2=0x0, a3=0x0, a4=0x0.
  The `jal` at 0x2bfc4 calls log_tstamp_event with a0=0x65, a1=0x6c, a2=0x0, a3=0x0, a4=0x0.
  The `jal` at 0x2c048 calls cmn_err with a0=one of 0x0, 0x4, a1=sym(.rodata)+43504, a2=[-0x3be0]:2, a3=?, a4=[a0+288]:8, a5=[a0+240]:8, a6=sym(.rodata)+43504.
  The `bne` branch at 0x2bdd8 targets 0x2bf98, is taken when (at(=[-0x3d80]:8) & 0x20) != 0, and falls through when (at(=[-0x3d80]:8) & 0x20) == 0.
  The `beq` branch at 0x2bde4 targets 0x2be14, is taken when v0(=[-0x3960]) == 0, and falls through when v0(=[-0x3960]) != 0.
  The `bne` branch at 0x2bdfc targets 0x2bfb8, is taken when (v1(=[-0x3d80]:8) & 0x20) != 0, and falls through when (v1(=[-0x3d80]:8) & 0x20) == 0.
  The `beq` branch at 0x2be1c targets 0x2be58, is taken when v0(=ret(hwperf_overflowed_intr)) == 0, and falls through when v0(=ret(hwperf_overflowed_intr)) != 0.
  The `beq` branch at 0x2be28 targets 0x2be48, is taken when (a0(=[sym(l2_sbe_check)]) & 0xff) == 0, and falls through when (a0(=[sym(l2_sbe_check)]) & 0xff) != 0.
  The `bne` branch at 0x2be40 targets 0x2be08, is taken when (a1(=[sym(l2_sbe_check)]) & 0x1) != 0, and falls through when (a1(=[sym(l2_sbe_check)]) & 0x1) == 0.
  The `beq` branch at 0x2be50 targets 0x2be08 and is always taken.
  The `beq` branch at 0x2be60 targets 0x2be78, is taken when (a2(=[-0x3fb8]) & 0x40) == 0, and falls through when (a2(=[-0x3fb8]) & 0x40) != 0.
  The `beq` branch at 0x2be70 targets 0x2bdf8 and is always taken.
  The `beq` branch at 0x2be84 targets 0x2bf70, is taken when a0(=[-0x3a48]:8) == 0, and falls through when a0(=[-0x3a48]:8) != 0.
  The `beq` branch at 0x2be9c targets 0x2bf20, is taken when v1(=[sym(lbolt)]) >= 0x3e9 (signed), and falls through when v1(=[sym(lbolt)]) < 0x3e9 (signed).
  The `bne` branch at 0x2beac targets 0x2beec, is taken when a5(?) < a0(=[sym(counter_intr_warn_cycles)]:8) (unsigned), and falls through when a5(?) >= a0(=[sym(counter_intr_warn_cycles)]:8) (unsigned).
  The `beq` branch at 0x2bed0 targets 0x2befc, is taken when a1(=[-0x3a9c]) == a4(=0xf0000000), and falls through when a1(=[-0x3a9c]) != a4(=0xf0000000).
  The `bne` branch at 0x2bedc targets 0x2bf00, is taken when a6(=[(sym(.bss)+39056 + ([-0x3be0]:2 << 0x2))+16]) != 0, and falls through when a6(=[(sym(.bss)+39056 + ([-0x3be0]:2 << 0x2))+16]) == 0.
  The `beq` branch at 0x2bee4 targets 0x2bfd4 and is always taken.
  The `beq` branch at 0x2bf18 targets 0x2bdf8 and is always taken.
  The `beq` branch at 0x2bf40 targets 0x2bf4c, is taken when a1(=[-0x3a34]) >= a0(?) (unsigned), and falls through when a1(=[-0x3a34]) < a0(?) (unsigned).
  The `beq` branch at 0x2bf68 targets 0x2bea4 and is always taken.
  The `beq` branch at 0x2bf74 targets 0x2bf90, is taken when a4(=[sym(counter_intr_warn_cycles)]:8) == 0, and falls through when a4(=[sym(counter_intr_warn_cycles)]:8) != 0.
  The `beq` branch at 0x2bf88 targets 0x2befc and is always taken.
  The `beq` branch at 0x2bf90 targets 0x2beb4 and is always taken.
  The `beq` branch at 0x2bfb0 targets 0x2bde4 and is always taken.
  The `beq` branch at 0x2bfcc targets 0x2be08 and is always taken.
  The `bne` branch at 0x2c018 targets 0x2c038, is taken when a3(?) < a1(?) (signed), and falls through when a3(?) >= a1(?) (signed).
  The `bne` branch at 0x2c028 targets 0x2c038, is taken when a7(?) < a3(?) (signed), and falls through when a7(?) >= a3(?) (signed).
  The `beq` branch at 0x2c030 targets 0x2c040 and is always taken.
  The `beq` branch at 0x2c060 targets 0x2befc and is always taken.
  The return at 0x2be0c has v0 = one of ?, ret(l2_sbe_intr), ret(hwperf_intr), ret(log_tstamp_event).
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2d610
  **Inferred.** The test of `a0` at 0x2d62c splits it into two paths. On each it calls `splprof` (0x2d654, 0x2d770), then `__nanotime` (0x2d65c, 0x2d778), then takes the spin lock on the symbol `timelock` through `nested_spinlock` (0x2d66c, 0x2d784), and releases it with `mutex_spinunlock` (0x2d714, 0x2d7e4), passing the `splprof` result as the unlock's a1. The `__nanotime` call comes before the lock call on both paths, so this routine does not make it under that lock. It branches on the sign of its first argument and of `s2`, and compares `s0` against 0xf4240 (one million) at 0x2d744; register a4 holds 0x3e8 at the unlock at 0x2d7e4. The name points at adjusting the time of day, which the lock and timestamp reads support, but the adjustment arithmetic is not shown as one identifiable step.
  ABI: n64.
  Range: 0x2d610-0x2d7fc (492 bytes).
  It occupies 492 bytes.
  The `jal` at 0x2d654 calls splprof with a0=0xd690000, a1=0xf0000, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x2d65c calls __nanotime [0x2d7fc] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x2d66c calls nested_spinlock with a0=sym(timelock), a1=?, a2=?, a3=?.
  The `jal` at 0x2d714 calls mutex_spinunlock with a0=sym(timelock), a1=ret(splprof), a2=[sym(.sbss)+204]+1, a3=?.
  The `jal` at 0x2d770 calls splprof with a0=a0 at entry (arg0), a1=0xf0000, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x2d778 calls __nanotime [0x2d7fc] with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x2d784 calls nested_spinlock with a0=sym(timelock), a1=?, a2=?, a3=?.
  The `jal` at 0x2d7e4 calls mutex_spinunlock with a0=sym(timelock), a1=ret(splprof), a2=?, a3=?, a4=0x3e8.
  The `beq` branch at 0x2d62c targets 0x2d770, is taken when a0 == 0, and falls through when a0 != 0.
  The `bltz` branch at 0x2d638 targets 0x2d760, is taken when a0 < 0 (signed), and falls through when a0 >= 0 (signed).
  The `beq` branch at 0x2d644 targets 0x2d73c, is taken when at(?) == 0, and falls through when at(?) != 0.
  The `bne` branch at 0x2d64c targets 0x2d758, is taken when v0(?) != 0, and falls through when v0(?) == 0.
  The `blez` branch at 0x2d6b0 targets 0x2d6f4, is taken when a0(?) <= 0 (signed), and falls through when a0(?) > 0 (signed).
  The `bltz` branch at 0x2d6f4 targets 0x2d768, is taken when s2(?) < 0 (signed), and falls through when s2(?) >= 0 (signed).
  The `bne` branch at 0x2d744 targets 0x2d750, is taken when t9(=0xf4240) < s0(?) (signed), and falls through when t9(=0xf4240) >= s0(?) (signed).
  The `beq` branch at 0x2d750 targets 0x2d654, is taken when v0(?) == 0, and falls through when v0(?) != 0.
  The `beq` branch at 0x2d758 targets 0x2d654 and is always taken.
  The `beq` branch at 0x2d760 targets 0x2d63c and is always taken.
  The `beq` branch at 0x2d768 targets 0x2d700 and is always taken.
  The `blez` branch at 0x2d79c targets 0x2d7f4, is taken when a0(?) <= 0 (signed), and falls through when a0(?) > 0 (signed).
  The `beq` branch at 0x2d7ec targets 0x2d720 and is always taken.
  The `beq` branch at 0x2d7f4 targets 0x2d7e0 and is always taken.
  The return at 0x2d734 has v0 = ?.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2d7fc
  **Inferred.** Takes the `timelock` spin lock through `mutex_spinlock_spl` (0x2d808) and loads the counter at the fixed address 0x9200000001030100 once (0x2d824). It compares that value with `adjtime_delta + adjtime_RTC` (0x2d858) and tests `adjtime_nanos` against zero (0x2d890). Branches at 0x2d920, 0x2d960 and 0x2d970 compare values against 0x3b9aca00 (1,000,000,000), which suggests a nanoseconds-per-second bound, but the facts do not show what is divided or added. It releases the lock with `mutex_spinunlock` (0x2d9a8, 0x2da28). The disassembly labels the table offsets it reads and writes from the nearest symbol `nasid_to_compact_node`, which may not be the real table, and the meaning of the counter at 0x9200000001030100 is not identified.
  ABI: n64.
  Range: 0x2d7fc-0x2da38 (572 bytes).
  It occupies 572 bytes.
  The `ld` at 0x2d824 loads 8 bytes from abs 0x9200000001030100.
  3 such `ld` accesses load 8 bytes from sym(nasid_to_compact_node) +0x9898, including `ld` at 0x2d8d8, `ld` at 0x2d928, `ld` at 0x2d9bc.
  2 such `sd` accesses store 8 bytes to sym(nasid_to_compact_node) +0x9898, including `sd` at 0x2d980, `sd` at 0x2da14.
  2 such `lw` accesses load 4 bytes from sym(nasid_to_compact_node) +0x9890, including `lw` at 0x2d984, `lw` at 0x2d990.
  2 such `sw` accesses store 4 bytes to sym(nasid_to_compact_node) +0x9890, including `sw` at 0x2d98c, `sw` at 0x2da10.
  The `jal` at 0x2d808 calls mutex_spinlock_spl with a0=sym(timelock), a1=sym(splprof), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x2d9a8 calls mutex_spinunlock with a0=sym(timelock), a1=ret(mutex_spinlock_spl), a2=?, a3=?, a7=[0x9200000001030100]:8.
  The `jal` at 0x2da28 calls mutex_spinunlock with a0=sym(timelock), a1=ret(mutex_spinlock_spl), a2=?, a3=?, a7=[0x9200000001030100]:8.
  The `beq` branch at 0x2d83c targets 0x2da08, is taken when at(=[sym(.sbss)+208]:8) == 0, and falls through when at(=[sym(.sbss)+208]:8) != 0.
  The `beq` branch at 0x2d858 targets 0x2d888, is taken when a7(=[0x9200000001030100]:8) >= a5(=([sym(adjtime_delta)]:8 + [sym(adjtime_RTC)]:8)) (unsigned), and falls through when a7(=[0x9200000001030100]:8) < a5(=([sym(adjtime_delta)]:8 + [sym(adjtime_RTC)]:8)) (unsigned).
  The `beq` branch at 0x2d880 targets 0x2d88c and is always taken.
  The `beq` branch at 0x2d890 targets 0x2d8c8, is taken when v1(=[sym(adjtime_nanos)]:8) == 0, and falls through when v1(=[sym(adjtime_nanos)]:8) != 0.
  The `bltz` branch at 0x2d8a8 targets 0x2d9f0, is taken when t0(?) < 0 (signed), and falls through when t0(?) >= 0 (signed).
  The `blez` branch at 0x2d8b0 targets 0x2d9f8, is taken when a5(?) <= 0 (signed), and falls through when a5(?) > 0 (signed).
  The `bne` branch at 0x2d920 targets 0x2d9bc, is taken when t9(?) < a6(=0x3b9aca00) (signed), and falls through when t9(?) >= a6(=0x3b9aca00) (signed).
  The `bne` branch at 0x2d960 targets 0x2d978, is taken when a4(=table(0xffffffffc4653600)) < a6(=0x3b9aca00) (signed), and falls through when a4(=table(0xffffffffc4653600)) >= a6(=0x3b9aca00) (signed).
  The `beq` branch at 0x2d970 targets 0x2d954, is taken when a4(=table(0xffffffff88ca6c00)) >= a6(=0x3b9aca00) (signed), and falls through when a4(=table(0xffffffff88ca6c00)) < a6(=0x3b9aca00) (signed).
  The `beq` branch at 0x2d97c targets 0x2d998, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0x2d9e8 targets 0x2d978 and is always taken.
  The `bltz` branch at 0x2d9f0 targets 0x2d8b8, is taken when a5(?) < 0 (signed), and falls through when a5(?) >= 0 (signed).
  The `beq` branch at 0x2da00 targets 0x2d8bc and is always taken.
  The `beq` branch at 0x2da30 targets 0x2d9b4 and is always taken.
  The return at 0x2d9b4 has v0 = ret(mutex_spinunlock).
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2df8c
  **Inferred.** Loads one word at 0xffffffffffffc744 (0x2df8c; the gp-relative `[-0x38bc]` is the same location). When it is nonzero the routine returns it unchanged (return at 0x2df9c). When it is zero the routine calls `splhi` (0x2dfa4), resets the counter with `_set_count` with a0=0 (0x2dfbc), and loops, reading the counter at 0x9200000001030100, until it reaches the value read before the loop plus 0x1312d0 (1,250,000). It then calls `_get_count` (0x2e034) and `cpu_cycles_adjust` (0x2e03c), stores a word to 0xffffffffffffc744 (0x2e048), and calls `splx` (0x2e04c) with the `splhi` result. The facts do not show the stored value, which may be the `cpu_cycles_adjust` result. The name suggests measuring the CPU clock in cycles, which the loop supports, but the meaning of the 1,250,000 threshold and of the stored word are not established.
  ABI: n64.
  Range: 0x2df8c-0x2e060 (212 bytes).
  It occupies 212 bytes.
  The `lw` at 0x2df8c loads 4 bytes from abs 0xffffffffffffc744.
  7 such `ld` accesses load 8 bytes from abs 0x9200000001030100, including `ld` at 0x2dfb4, `ld` at 0x2dfcc, `ld` at 0x2dfe4.
  The `sw` at 0x2e048 stores 4 bytes to abs 0xffffffffffffc744.
  The `jal` at 0x2dfa4 calls splhi with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x2dfbc calls _set_count with a0=0x0, a1=?, a2=?, a3=?.
  The `jal` at 0x2e034 calls _get_count with a0=?, a1=?, a2=?, a3=[0x9200000001030100]:8+0x1312d0, a4=0x1312d0.
  The `jal` at 0x2e03c calls cpu_cycles_adjust with a0=ret(_get_count), a1=?, a2=?, a3=?.
  The `jal` at 0x2e04c calls splx with a0=ret(splhi), a1=?, a2=?, a3=?.
  The `beq` branch at 0x2df94 targets 0x2dfa4, is taken when v0(=[-0x38bc]) == 0, and falls through when v0(=[-0x38bc]) != 0.
  The `beq` branch at 0x2dfdc targets 0x2e034, is taken when v0(=[0x9200000001030100]:8) >= a3(=[0x9200000001030100]:8+0x1312d0) (unsigned), and falls through when v0(=[0x9200000001030100]:8) < a3(=[0x9200000001030100]:8+0x1312d0) (unsigned).
  The `beq` branch at 0x2dfec targets 0x2e034, is taken when a5(=[0x9200000001030100]:8) >= a3(=[0x9200000001030100]:8+0x1312d0) (unsigned), and falls through when a5(=[0x9200000001030100]:8) < a3(=[0x9200000001030100]:8+0x1312d0) (unsigned).
  The `beq` branch at 0x2dffc targets 0x2e034, is taken when a6(=[0x9200000001030100]:8) >= a3(=[0x9200000001030100]:8+0x1312d0) (unsigned), and falls through when a6(=[0x9200000001030100]:8) < a3(=[0x9200000001030100]:8+0x1312d0) (unsigned).
  The `beq` branch at 0x2e00c targets 0x2e034, is taken when a7(=[0x9200000001030100]:8) >= a3(=[0x9200000001030100]:8+0x1312d0) (unsigned), and falls through when a7(=[0x9200000001030100]:8) < a3(=[0x9200000001030100]:8+0x1312d0) (unsigned).
  The `beq` branch at 0x2e01c targets 0x2e034, is taken when t0(=[0x9200000001030100]:8) >= a3(=[0x9200000001030100]:8+0x1312d0) (unsigned), and falls through when t0(=[0x9200000001030100]:8) < a3(=[0x9200000001030100]:8+0x1312d0) (unsigned).
  The `bne` branch at 0x2e02c targets 0x2dfe4, is taken when t1(=[0x9200000001030100]:8) < a3(=[0x9200000001030100]:8+0x1312d0) (unsigned), and falls through when t1(=[0x9200000001030100]:8) >= a3(=[0x9200000001030100]:8+0x1312d0) (unsigned).
  The `beq` branch at 0x2e058 targets 0x2df9c and is always taken.
  The return at 0x2df9c has v0 = one of [-0x38bc], ret(cpu_cycles_adjust).
  Reviewed: 2026-11-12 (sonnet)

- `acktmoclock` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2ea40
  **Inferred.** Reads a halfword at 0xffffffffffffc420 and uses the CPU index in `[-0x3be0]:2` to pick a per-CPU structure from `pdaindr`. If a flag byte at offset 0x520 in that structure is set, it stores to 0x9200000001000128, otherwise to 0x9200000001000120, and returns the CPU index shifted left by 4 in both cases. The name suggests acknowledging the timeout clock interrupt, but the facts do not show what the two stored addresses control.
  ABI: n64.
  Range: 0x2ea40-0x2ea78 (56 bytes).
  It occupies 56 bytes.
  The `lh` at 0x2ea40 loads 2 bytes from abs 0xffffffffffffc420.
  The `ld` at 0x2ea50 loads 8 bytes from (sym(pdaindr) + ([-0x3be0]:2 << 0x4)) +0x8.
  The `lbu` at 0x2ea54 loads 1 byte from [(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8 +0x520.
  The `sd` at 0x2ea64 stores 8 bytes to abs 0x9200000001000128.
  The `sd` at 0x2ea74 stores 8 bytes to abs 0x9200000001000120.
  The `beq` branch at 0x2ea5c targets 0x2ea70, is taken when at(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) == 0, and falls through when at(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) != 0.
  The return at 0x2ea68 has v0 = ([-0x3be0]:2 << 0x4).
  The return at 0x2ea70 has v0 = ([-0x3be0]:2 << 0x4).
  Reviewed: 2026-11-12 (sonnet)

- `ackkgclock` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2ea78
  **Inferred.** Reads the same halfword and per-CPU flag byte as `acktmoclock`, using the CPU index in `[-0x3be0]:2` to find the structure in `pdaindr`. If the flag byte is set, it stores to 0x9200000001000138, otherwise to 0x9200000001000130, and returns the shifted CPU index. The name suggests acknowledging the `kg` clock interrupt, but the `kg` prefix is not expanded in the facts and the stored addresses are not identified.
  ABI: n64.
  Range: 0x2ea78-0x2eab0 (56 bytes).
  It occupies 56 bytes.
  The `lh` at 0x2ea78 loads 2 bytes from abs 0xffffffffffffc420.
  The `ld` at 0x2ea88 loads 8 bytes from (sym(pdaindr) + ([-0x3be0]:2 << 0x4)) +0x8.
  The `lbu` at 0x2ea8c loads 1 byte from [(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8 +0x520.
  The `sd` at 0x2ea9c stores 8 bytes to abs 0x9200000001000138.
  The `sd` at 0x2eaac stores 8 bytes to abs 0x9200000001000130.
  The `beq` branch at 0x2ea94 targets 0x2eaa8, is taken when at(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) == 0, and falls through when at(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) != 0.
  The return at 0x2eaa0 has v0 = ([-0x3be0]:2 << 0x4).
  The return at 0x2eaa8 has v0 = ([-0x3be0]:2 << 0x4).
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2eab0
  **Inferred.** Takes a spin lock at `.sbss`+288 through `mutex_spinlock_spl` (0x2eabc) and releases it at 0x2eb7c and 0x2ebb8. A flag word at `sym(.sdata)+376` picks between two paths (branch at 0x2eacc). When it is zero the routine calls `setkgvector` with `prof_intr` as its first argument (0x2eb9c). Otherwise a word at `[-0x3acc]` is compared with 0x4e2 (1250) at 0x2eae0, and `cnodetocpu` (0x2eb34) is called only when the flag byte at offset 0x520 of the per-CPU structure from `pdaindr` is nonzero and the word at `[[-0x3c00]:8+18736]` is not 1. The name suggests starting the `kg` clock, which the vector install supports, but the lock, the 0x4e2 value and the stored addresses 0x9200000001000158, 0x9200000001000118 and 0x9200000001000150 are not identified.
  ABI: n64.
  Range: 0x2eab0-0x2ebc8 (280 bytes).
  It occupies 280 bytes.
  The `lw` at 0x2ead4 loads 4 bytes from abs 0xffffffffffffc534.
  4 such `ld` accesses load 8 bytes from abs 0xffffffffffffc400, including `ld` at 0x2eae8, `ld` at 0x2eb24, `ld` at 0x2eb48.
  The `sw` at 0x2eaec stores 4 bytes to abs 0xffffffffffffc534.
  2 such `lw` accesses load 4 bytes from [-0x3c00]:8 +0x4930, including `lw` at 0x2eaf0, `lw` at 0x2eb28.
  The `sw` at 0x2eaf8 stores 4 bytes to [-0x3c00]:8 +0x4930.
  2 such `lh` accesses load 2 bytes from abs 0xffffffffffffc420, including `lh` at 0x2eafc, `lh` at 0x2eb3c.
  The `ld` at 0x2eb08 loads 8 bytes from (sym(pdaindr) + ([-0x3be0]:2 << 0x4)) +0x8.
  The `lbu` at 0x2eb0c loads 1 byte from [(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8 +0x520.
  The `sd` at 0x2eb20 stores 8 bytes to abs 0x9200000001000158.
  The `lh` at 0x2eb38 loads 2 bytes from abs 0xffffffffffffc422.
  The `ld` at 0x2eb50 loads 8 bytes from abs 0x9200000001030100.
  The `sd` at 0x2eb58 stores 8 bytes to [-0x3c00]:8 +0x4928.
  The `ld` at 0x2eb64 loads 8 bytes from [-0x3c00]:8 +0x4928.
  The `sd` at 0x2eb68 stores 8 bytes to abs 0x9200000001000118.
  The `sd` at 0x2eb98 stores 8 bytes to abs 0x9200000001000150.
  The `jal` at 0x2eabc calls mutex_spinlock_spl with a0=sym(.sbss)+288, a1=sym(splprof), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x2eb34 calls cnodetocpu with a0=[-0x3bde]:2, a1=0x1, a2=[[-0x3c00]:8+18736]+1, a3=one of 0x9200000001000158, 0x4e2, a4=[[-0x3c00]:8+18736], a7=0x9200000001030100.
  The `jal` at 0x2eb7c calls mutex_spinunlock with a0=sym(.sbss)+288, a1=(ret(mutex_spinlock_spl) << 0x0), a2=?, a3=?.
  The `jal` at 0x2eb9c calls setkgvector with a0=sym(prof_intr), a1=?, a2=?, a3=?.
  The `jal` at 0x2ebb8 calls mutex_spinunlock with a0=sym(.sbss)+288, a1=(ret(mutex_spinlock_spl) << 0x0), a2=?, a3=0x4e2.
  The `beq` branch at 0x2eacc targets 0x2eb9c, is taken when at(=[sym(.sdata)+376]) == 0, and falls through when at(=[sym(.sdata)+376]) != 0.
  The `beq` branch at 0x2eae0 targets 0x2ebb4, is taken when v1(=[-0x3acc]) == a3(=0x4e2), and falls through when v1(=[-0x3acc]) != a3(=0x4e2).
  The `beq` branch at 0x2eb14 targets 0x2eb94, is taken when a0(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) == 0, and falls through when a0(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) != 0.
  The `beq` branch at 0x2eb2c targets 0x2eb50, is taken when a4(=[[-0x3c00]:8+18736]) == a1(=0x1), and falls through when a4(=[[-0x3c00]:8+18736]) != a1(=0x1).
  The `bne` branch at 0x2eb40 targets 0x2eb70, is taken when a5(=[-0x3be0]:2) != v0(=ret(cnodetocpu)), and falls through when a5(=[-0x3be0]:2) == v0(=ret(cnodetocpu)).
  The `beq` branch at 0x2eb94 targets 0x2eb24 and is always taken.
  The `beq` branch at 0x2ebac targets 0x2ead4 and is always taken.
  The `beq` branch at 0x2ebc0 targets 0x2eb88 and is always taken.
  The return at 0x2eb8c has v0 = ret(mutex_spinunlock).
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2ebc8
  **Inferred.** A flag word at `sym(.sdata)+384` splits it (branch at 0x2ebd0). When it is zero the routine first calls `spinlock_init` on `.sbss`+288 (0x2ec8c) and then continues to the lock call (the branch at 0x2ec98 returns to 0x2ebd8). Otherwise it goes straight to the lock call. The lock is the same one `startkgclock` takes (`.sbss`+288, through `mutex_spinlock_spl` at 0x2ebdc), and it is released at 0x2ec0c or 0x2ec64 depending on the path. The routine compares `master_procid` with the current CPU index (0x2ebf8), checks the word `sym(audioclock)` (0x2ec00), and calls `ackkgclock` (0x2ec6c) on one of the later paths. The name suggests a slower variant of the `kg` clock start, but the audio-clock check and the stored values at 0x9200000001000158 and 0x9200000001000150 are not explained by the facts.
  ABI: n64.
  Range: 0x2ebc8-0x2eca0 (216 bytes).
  It occupies 216 bytes.
  The `lh` at 0x2ebe4 loads 2 bytes from abs 0xffffffffffffc420.
  The `ld` at 0x2ec20 loads 8 bytes from (sym(pdaindr) + ([-0x3be0]:2 << 0x4)) +0x8.
  The `lbu` at 0x2ec24 loads 1 byte from [(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8 +0x520.
  The `sd` at 0x2ec34 stores 8 bytes to abs 0x9200000001000158.
  The `ld` at 0x2ec40 loads 8 bytes from abs 0xffffffffffffc400.
  The `lw` at 0x2ec4c loads 4 bytes from [-0x3c00]:8 +0x4930.
  The `sw` at 0x2ec58 stores 4 bytes to [-0x3c00]:8 +0x4930.
  The `sw` at 0x2ec68 stores 4 bytes to abs 0xffffffffffffc534.
  The `sd` at 0x2ec84 stores 8 bytes to abs 0x9200000001000150.
  The `jal` at 0x2ebdc calls mutex_spinlock_spl with a0=sym(.sbss)+288, a1=sym(splprof), a2=?, a3=?.
  The `jal` at 0x2ec0c calls mutex_spinunlock with a0=sym(.sbss)+288, a1=(ret(mutex_spinlock_spl) << 0x0), a2=([-0x3be0]:2 << 0x4), a3=[sym(audioclock)], a4=0x9200000001000158, a5=[-0x3be0]:2, a6=sym(pdaindr).
  The `jal` at 0x2ec64 calls mutex_spinunlock with a0=sym(.sbss)+288, a1=(ret(mutex_spinlock_spl) << 0x0), a2=?, a3=?, a5=[-0x3be0]:2, a6=sym(pdaindr), a7=0x9200000001000150.
  The `jal` at 0x2ec6c calls ackkgclock with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x2ec8c calls spinlock_init with a0=sym(.sbss)+288, a1=sym(.rodata)+44008, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `beq` branch at 0x2ebd0 targets 0x2ec88, is taken when at(=[sym(.sdata)+384]) == 0, and falls through when at(=[sym(.sdata)+384]) != 0.
  The `bne` branch at 0x2ebf8 targets 0x2ec1c, is taken when v1(=[sym(master_procid)]:2) != a5(=[-0x3be0]:2), and falls through when v1(=[sym(master_procid)]:2) == a5(=[-0x3be0]:2).
  The `beq` branch at 0x2ec00 targets 0x2ec20, is taken when a3(=[sym(audioclock)]) == 0, and falls through when a3(=[sym(audioclock)]) != 0.
  The `beq` branch at 0x2ec14 targets 0x2ec78 and is always taken.
  The `beq` branch at 0x2ec2c targets 0x2ec80, is taken when a0(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) == 0, and falls through when a0(=[[(sym(pdaindr) + ([-0x3be0]:2 << 0x4))+8]:8+1312]:1) != 0.
  The `beq` branch at 0x2ec38 targets 0x2ec5c, is taken when a3(=[sym(.sdata)+376]) == 0, and falls through when a3(=[sym(.sdata)+376]) != 0.
  The `beq` branch at 0x2ec48 targets 0x2ec54, is taken when a4(=[sym(.sdata)+380]) == 0, and falls through when a4(=[sym(.sdata)+380]) != 0.
  The `beq` branch at 0x2ec80 targets 0x2ec38 and is always taken.
  The `beq` branch at 0x2ec98 targets 0x2ebd8 and is always taken.
  The return at 0x2ec78 has v0 = one of ret(mutex_spinunlock), ret(ackkgclock).
  Reviewed: 2026-11-12 (sonnet)

- `prof_intr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2f850
  **Inferred.** Calls `fastick_maint` (0x2f868) and `cnode_subnode_to_cpu` (0x2f8b8), compares counters read through the pointer at gp-relative offset -0x3c00 (0x2f8a0 to 0x2f8f4), and stores to 0x9200000001000118 (0x2f90c). It logs timestamp events with tags 0x6e and 0x65 (0x2f920, 0x2f93c). The name fits a profiling tick interrupt, which the tags and per-CPU counters support, but the meaning of the compared fields and of the bit 0x20 tested at 0x2f860 and 0x2f888 is not established.
  ABI: n64.
  Range: 0x2f850-0x2f94c (252 bytes).
  It occupies 252 bytes.
  2 such `ld` accesses load 8 bytes from abs 0xffffffffffffc280, including `ld` at 0x2f850, `ld` at 0x2f880.
  The `lw` at 0x2f870 loads 4 bytes from abs 0xffffffffffffc534.
  4 such `ld` accesses load 8 bytes from abs 0xffffffffffffc400, including `ld` at 0x2f89c, `ld` at 0x2f8cc, `ld` at 0x2f8e4.
  The `lw` at 0x2f8a0 loads 4 bytes from [-0x3c00]:8 +0x4930.
  The `lh` at 0x2f8a8 loads 2 bytes from abs 0xffffffffffffc420.
  The `lh` at 0x2f8b4 loads 2 bytes from abs 0xffffffffffffc422.
  3 such `ld` accesses load 8 bytes from [-0x3c00]:8 +0x4928, including `ld` at 0x2f8d4, `ld` at 0x2f8e8, `ld` at 0x2f904.
  The `ld` at 0x2f8d8 loads 8 bytes from abs 0x9200000001030100.
  2 such `sd` accesses store 8 bytes to [-0x3c00]:8 +0x4928, including `sd` at 0x2f8e0, `sd` at 0x2f8fc.
  The `sd` at 0x2f90c stores 8 bytes to abs 0x9200000001000118.
  The `jal` at 0x2f868 calls fastick_maint with a0=a0 at entry (arg0), a1=?, a2=?, a3=?.
  The `jal` at 0x2f8b8 calls cnode_subnode_to_cpu with a0=[-0x3bde]:2, a1=0x0, a2=0x1, a3=[-0x3be0]:2.
  The `jal` at 0x2f920 calls log_tstamp_event with a0=0x6e, a1=0x0, a2=0x0, a3=0x0, a4=0x0.
  The `jal` at 0x2f93c calls log_tstamp_event with a0=0x65, a1=0x6e, a2=0x0, a3=0x0, a4=0x0.
  The `bne` branch at 0x2f860 targets 0x2f910, is taken when (at(=[-0x3d80]:8) & 0x20) != 0, and falls through when (at(=[-0x3d80]:8) & 0x20) == 0.
  The `beq` branch at 0x2f878 targets 0x2f89c, is taken when v0(=[-0x3acc]) == v1(=0x4e2), and falls through when v0(=[-0x3acc]) != v1(=0x4e2).
  The `bne` branch at 0x2f888 targets 0x2f930, is taken when (a0(=[-0x3d80]:8) & 0x20) != 0, and falls through when (a0(=[-0x3d80]:8) & 0x20) == 0.
  The `beq` branch at 0x2f8ac targets 0x2f8d0, is taken when a1(=[[-0x3c00]:8+18736]) == a2(=0x1), and falls through when a1(=[[-0x3c00]:8+18736]) != a2(=0x1).
  The `bne` branch at 0x2f8c4 targets 0x2f880, is taken when a4(=[-0x3be0]:2) != v0(=ret(cnode_subnode_to_cpu)), and falls through when a4(=[-0x3be0]:2) == v0(=ret(cnode_subnode_to_cpu)).
  The `beq` branch at 0x2f8f4 targets 0x2f908, is taken when a2(=[[-0x3c00]:8+18728]:8) >= a3(=[0x9200000001030100]:8) (unsigned), and falls through when a2(=[[-0x3c00]:8+18728]:8) < a3(=[0x9200000001030100]:8) (unsigned).
  The `beq` branch at 0x2f908 targets 0x2f880 and is always taken.
  The `beq` branch at 0x2f928 targets 0x2f868 and is always taken.
  The `beq` branch at 0x2f944 targets 0x2f894 and is always taken.
  The return at 0x2f894 has v0 = one of ?, ret(log_tstamp_event).
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_mem` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2fe64
  **Inferred.** Reads a doubleword from the fixed address 0x9200000001200018 and a word from 0x9000000010000090, then formats its result through `sprintf` at four call sites (0x2ff00, 0x2ff8c, 0x2ffa4, 0x2ffcc) using one format string in `.rodata`. One branch compares its argument a2 with 0x36 (54) at 0x2ff30. The name suggests translating a memory-directory address into text, which the formatting calls support, but the hub field the loads read and the 0x36 cutoff are not identified.
  ABI: n64.
  Range: 0x2fe64-0x2ffdc (376 bytes).
  It occupies 376 bytes.
  The `ld` at 0x2fe80 loads 8 bytes from abs 0x9200000001200018.
  The `lw` at 0x2fe8c loads 4 bytes from abs 0x9000000010000090.
  The `jal` at 0x2ff00 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=?, a3=?, a5=one of 0x0, 0x24, a7=a2 at entry (arg2).
  The `jal` at 0x2ff8c calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=one of 0x58, 0x59, a3=one of 0x48, 0x4c, a7=0x48.
  The `jal` at 0x2ffa4 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=?, a3=?, a5=one of 0x0, 0x24, a7=a2 at entry (arg2).
  The `jal` at 0x2ffcc calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=one of 0x58, 0x59, a3=?, a4=one of 0x58, 0x59, a7=a2 at entry (arg2).
  The `beq` branch at 0x2fea4 targets 0x2ff1c, is taken when at(=[0x9000000010000090]) == 0, and falls through when at(=[0x9000000010000090]) != 0.
  The `bne` branch at 0x2feac targets 0x2febc, is taken when a3(?) != 0, and falls through when a3(?) == 0.
  The `bne` branch at 0x2feb4 targets 0x2fec0, is taken when t2(?) != 0, and falls through when t2(?) == 0.
  The `bne` branch at 0x2febc targets 0x2ff14, is taken when a5(?) != 0, and falls through when a5(?) == 0.
  The `bltz` branch at 0x2fec8 targets 0x2ff9c, is taken when a7(=a2) < 0 (signed), and falls through when a7(=a2) >= 0 (signed).
  The `beq` branch at 0x2ff14 targets 0x2fec4 and is always taken.
  The `bne` branch at 0x2ff1c targets 0x2ff28, is taken when a3(?) != 0, and falls through when a3(?) == 0.
  The `bne` branch at 0x2ff24 targets 0x2ff38, is taken when t2(?) != 0, and falls through when t2(?) == 0.
  The `bne` branch at 0x2ff30 targets 0x2ffb4, is taken when a7(=a2) < 0x36 (signed), and falls through when a7(=a2) >= 0x36 (signed).
  The `bltz` branch at 0x2ff4c targets 0x2ffbc, is taken when a7(=a2) < 0 (signed), and falls through when a7(=a2) >= 0 (signed).
  The `beq` branch at 0x2ff94 targets 0x2ff0c and is always taken.
  The `beq` branch at 0x2ffac targets 0x2ff0c and is always taken.
  The `beq` branch at 0x2ffb4 targets 0x2ff40 and is always taken.
  The `beq` branch at 0x2ffd4 targets 0x2ff0c and is always taken.
  The return at 0x2ff0c has v0 = ret(sprintf).
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_pdir` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x2ffdc
  **Inferred.** Reads a doubleword from 0x9200000001200018 (0x30004) and formats output through `sprintf` at three call sites (0x30060, 0x30084, 0x300d0) with the same `.rodata` format string. It branches on the sign of its argument a2 (0x3001c) and on a register at 0x30024. The name suggests the page-directory variant of the memory-directory translation, which the parallel shape supports, but the meaning of the branches is not established.
  ABI: n64.
  Range: 0x2ffdc-0x300e0 (260 bytes).
  It occupies 260 bytes.
  The `ld` at 0x30004 loads 8 bytes from abs 0x9200000001200018.
  The `jal` at 0x30060 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=one of 0x58, 0x59, a3=one of 0x48, 0x4c, a5=a2 at entry (arg2).
  The `jal` at 0x30084 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=one of 0x58, 0x59, a3=?, a4=a2-16, a5=a2 at entry (arg2).
  The `jal` at 0x300d0 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=one of 0x58, 0x59, a3=one of 0x48, 0x4c, a5=one of 0x58, 0x59.
  The `bltz` branch at 0x3001c targets 0x30094, is taken when a2 < 0 (signed), and falls through when a2 >= 0 (signed).
  The `beq` branch at 0x30024 targets 0x30074, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0x3008c targets 0x3006c and is always taken.
  The `beq` branch at 0x300d8 targets 0x3006c and is always taken.
  The return at 0x3006c has v0 = ret(sprintf).
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_sdir` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x300e0
  **Inferred.** Reads the same doubleword at 0x9200000001200018 and a word at 0x9000000010000090, branches on the sign of a2 at 0x30138 and 0x3017c, and calls `sprintf` at four sites (0x30148, 0x30188, 0x3019c, 0x301b0). The name suggests the segment-directory variant, matching the memory and page versions, but the facts do not identify what the segment-directory fields hold.
  ABI: n64.
  Range: 0x300e0-0x301c0 (224 bytes).
  It occupies 224 bytes.
  The `ld` at 0x300ec loads 8 bytes from abs 0x9200000001200018.
  The `lw` at 0x30110 loads 4 bytes from abs 0x9000000010000090.
  The `jal` at 0x30148 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=?, a3=a2 at entry (arg2), a5=a2 at entry (arg2).
  The `jal` at 0x30188 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=one of 0x58, 0x59, a3=one of 0x48, 0x4c, a5=a2 at entry (arg2).
  The `jal` at 0x3019c calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=?, a3=?, a5=a2 at entry (arg2).
  The `jal` at 0x301b0 calls sprintf with a0=a0 at entry (arg0), a1=sym(.rodata)+44192, a2=one of 0x58, 0x59, a3=one of 0x48, 0x4c, a5=a2 at entry (arg2).
  The `beq` branch at 0x3012c targets 0x3015c, is taken when at(=[0x9000000010000090]) == 0, and falls through when at(=[0x9000000010000090]) != 0.
  The `bltz` branch at 0x30138 targets 0x30198, is taken when a5(=a2) < 0 (signed), and falls through when a5(=a2) >= 0 (signed).
  The `bltz` branch at 0x3017c targets 0x301ac, is taken when a5(=a2) < 0 (signed), and falls through when a5(=a2) >= 0 (signed).
  The `beq` branch at 0x30190 targets 0x30154 and is always taken.
  The `beq` branch at 0x301a4 targets 0x30154 and is always taken.
  The `beq` branch at 0x301b8 targets 0x30154 and is always taken.
  The return at 0x30154 has v0 = ret(sprintf).
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x339b0
  **Inferred.** Stores three doublewords at low absolute addresses 0x548, 0x550 and 0x480, which the disassembly labels from the nearest symbol as offsets of `br_isr_errs`. It loads a doubleword at 0xfd8, tests it, and jumps through a register (`jr` at 0x339f8) with its arguments passed through. The nearest-symbol label on 0xfd8 may not describe that data, and the register target is not identified. The name points at an ECC error vector, which the jump supports.
  ABI: n64.
  Range: 0x339b0-0x33a08 (88 bytes).
  It occupies 88 bytes.
  The `sd` at 0x339b0 stores 8 bytes to abs 0x548 (br_isr_errs+200).
  The `sd` at 0x339b4 stores 8 bytes to abs 0x550 (br_isr_errs+208).
  The `sd` at 0x339b8 stores 8 bytes to abs 0x480 (br_isr_errs).
  The `ld` at 0x339e0 loads 8 bytes from abs 0xfd8 (in_cksum_sub+104).
  Calls (unknown callee) via register with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3) (`jr` at 0x339f8).
  The `bne` branch at 0x339e4 targets 0x339f4, is taken when k1(=[0xfd8]:8) != 0, and falls through when k1(=[0xfd8]:8) == 0.
  The instructions from 0x33a00 to 0x33a04 are not reached by any path from the entry, and their branches are not listed.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x37acc
  **Inferred.** It dispatches on its size argument a1: a byte read for 1, a halfword for 2, and a word for 4 (branches at 0x37b04, 0x37b1c, 0x37b34), and reports any other size through `cmn_err` (0x37b60). It stores a word to `sym(nofault)` (0x37afc, 0x37b94), and tests bit 0x4000 of `t1` at 0x37b74, which picks between a return of 0 and a branch to `earlybaerror` (0x37c48). The name suggests an early probe that tolerates bad addresses, which the nofault store supports, but the bit tested in `t1` is not identified.
  ABI: n64.
  Range: 0x37acc-0x37bac (224 bytes).
  It occupies 224 bytes.
  2 such `sw` accesses store 4 bytes to sym(nofault) +0x0, including `sw` at 0x37afc, `sw` at 0x37b94.
  The `lb` at 0x37b0c loads 1 byte from a0 (arg0 at entry) +0x0.
  The `lh` at 0x37b24 loads 2 bytes from a0 (arg0 at entry) +0x0.
  The `lw` at 0x37b3c loads 4 bytes from a0 (arg0 at entry) +0x0.
  The `jal` at 0x37b60 calls cmn_err with a0=0x0, a1=sym(.data)+15891, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `bne` branch at 0x37b04 targets 0x37b18, is taken when a1 != at(=0x1), and falls through when a1 == at(=0x1).
  The `beq` branch at 0x37b10 targets 0x37b68 and is always taken.
  The `bne` branch at 0x37b1c targets 0x37b30, is taken when a1 != at(=0x2), and falls through when a1 == at(=0x2).
  The `beq` branch at 0x37b28 targets 0x37b68 and is always taken.
  The `bne` branch at 0x37b34 targets 0x37b48, is taken when a1 != at(=0x4), and falls through when a1 == at(=0x4).
  The `beq` branch at 0x37b40 targets 0x37b68 and is always taken.
  The `bne` branch at 0x37b74 targets 0x37c48, is taken when (t1(?) & 0x4000) != 0, and falls through when (t1(?) & 0x4000) == 0.
  The return at 0x37ba4 has v0 = 0x0.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x37c18
  **Inferred.** Loads the gp-relative doubleword at -0x3fe0 (0x37c1c) and tests it for zero (0x37c20). When it is zero the branch goes to 0x37c34; when it is nonzero the routine first stores a halfword at offset 0xf6 from that pointer (0x37c28). Both paths reach the word store to 0xffffffffffffc484 (0x37c34) and return 1. The name suggests a bus-error handler, but the stored fields and the meaning of the null test are not identified.
  ABI: n64.
  Range: 0x37c18-0x37c48 (48 bytes).
  It occupies 48 bytes.
  The `ld` at 0x37c1c loads 8 bytes from abs 0xffffffffffffc020.
  The `sh` at 0x37c28 stores 2 bytes to [-0x3fe0]:8 +0xf6.
  The `sw` at 0x37c34 stores 4 bytes to abs 0xffffffffffffc484.
  The `beq` branch at 0x37c20 targets 0x37c34, is taken when t2(=[-0x3fe0]:8) == 0, and falls through when t2(=[-0x3fe0]:8) != 0.
  The `beq` branch at 0x37c2c targets 0x37c38 and is always taken.
  The return at 0x37c40 has v0 = 0x1.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x37c48
  **Inferred.** Stores a word to `sym(nofault)` at 0x37c68 and returns 1, with no other work in the facts. The nofault store is consistent with `earlybadaddr`, which suggests an early bus-error path, but the facts do not show what callers do with the return value.
  ABI: n64.
  Range: 0x37c48-0x37c7c (52 bytes).
  It occupies 52 bytes.
  The `sw` at 0x37c68 stores 4 bytes to sym(nofault) +0x0.
  The return at 0x37c74 has v0 = 0x1.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x39b78
  **Inferred.** The routine is one branch that targets itself at 0x39b78 and is always taken, so it spins on that instruction. Nothing in the disassembled code leaves the loop, so only something outside this code, such as a reset, could end it. The name fits a wait for reset, and the spin is consistent with it.
  ABI: n64.
  Range: 0x39b78-0x39b80 (8 bytes).
  It occupies 8 bytes.
  The `beq` branch at 0x39b78 targets 0x39b78 and is always taken.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x39fd0
  **Inferred.** Loads a halfword from 0xffffffffffffc420 (the gp-relative `[-0x3be0]:2` that other routines in this batch also load), stores a doubleword at offset 0x140 of its first argument (0x39fd8), and jumps to `VEC_trap` (0x39fd4) with its arguments passed through. The name suggests a data-bus-error vector that hands off to a common trap, which the jump supports. The halfword and the 0x140 field are not identified.
  ABI: n64.
  Range: 0x39fd0-0x39fdc (12 bytes).
  It occupies 12 bytes.
  The `lh` at 0x39fd0 loads 2 bytes from abs 0xffffffffffffc420.
  The `sd` at 0x39fd8 stores 8 bytes to a0 (arg0 at entry) +0x140.
  Calls VEC_trap via direct with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3) (`j` at 0x39fd4).
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3c640
  **Inferred.** Reads and updates an error record passed in `a1`, branching on bits of its field at +0x18 and comparing its field at +0x2b8 with 0x11 (0x3c6c8). It calls `ecc_cacheOp` (the routine at 0x3bea0) with first arguments 0, 1, 3, 4, 5 and 7, invalidates lines with `ecc_force_cache_line_invalid`, and saves state with `save_cache_error`. It tests the return of `eccf_recoverable_threshold` (0x3c740) at several branches (for example 0x3c844, 0x3caf4 and 0x3ccec), and reports through `memerror_handle_uce` and `cmn_err_tag`. The name fits a cache and memory ECC error handler, but the record layout, the first-argument codes and what the threshold result means are not established.
  ABI: n64.
  Range: 0x3c640-0x3ce00 (1984 bytes).
  It occupies 1984 bytes.
  The `lwu` at 0x3c658 loads 4 bytes from a1 (arg1 at entry) +0x18.
  2 such `lw` accesses load 4 bytes from a1 (arg1 at entry) +0x2b8, including `lw` at 0x3c6ac, `lw` at 0x3c708.
  The `lw` at 0x3c6b0 loads 4 bytes from a1 (arg1 at entry) +0x2bc.
  The `sw` at 0x3c6f8 stores 4 bytes to ?.
  The `ld` at 0x3c6fc loads 8 bytes from a1 (arg1 at entry) +0x0.
  9 such `sd` accesses store 8 bytes to ?, including `sd` at 0x3c700, `sd` at 0x3c730, `sd` at 0x3c7d0.
  The `sw` at 0x3c724 stores 4 bytes to a1 (arg1 at entry) +0x2b8.
  The `ld` at 0x3c728 loads 8 bytes from abs 0x9200000001030100.
  The `sd` at 0x3c754 stores 8 bytes to a1 (arg1 at entry) +0x8.
  The `sd` at 0x3c75c stores 8 bytes to a1 (arg1 at entry) +0x10.
  2 such `ld` accesses load 8 bytes from a1 (arg1 at entry) +0x8, including `ld` at 0x3c864, `ld` at 0x3c8ec.
  2 such `ld` accesses load 8 bytes from a1 (arg1 at entry) +0x10, including `ld` at 0x3c86c, `ld` at 0x3c8e0.
  The `sb` at 0x3c878 stores 1 byte to abs 0xffffffffffffc730.
  The `lbu` at 0x3c87c loads 1 byte from a1 (arg1 at entry) +0x26.
  The `sb` at 0x3c88c stores 1 byte to a1 (arg1 at entry) +0x26.
  The `ld` at 0x3c898 loads 8 bytes from a0 (arg0 at entry) +0x0.
  The `sd` at 0x3c8a4 stores 8 bytes to a0 (arg0 at entry) +0x438.
  2 such `ld` accesses load 8 bytes from ?, including `ld` at 0x3c8b4, `ld` at 0x3c8c8.
  The `lw` at 0x3c938 loads 4 bytes from a1 (arg1 at entry) +0x28.
  The `sw` at 0x3c954 stores 4 bytes to a1 (arg1 at entry) +0x28.
  The `lw` at 0x3c95c loads 4 bytes from a1 (arg1 at entry) +0x2c.
  The `sw` at 0x3c970 stores 4 bytes to a1 (arg1 at entry) +0x2c.
  The `lw` at 0x3caa8 loads 4 bytes from a1 (arg1 at entry) +0x30.
  The `sw` at 0x3cac4 stores 4 bytes to a1 (arg1 at entry) +0x30.
  The `lw` at 0x3cb48 loads 4 bytes from a1 (arg1 at entry) +0x34.
  The `sw` at 0x3cb58 stores 4 bytes to a1 (arg1 at entry) +0x34.
  The `lh` at 0x3cd24 loads 2 bytes from table(sym(nasid_to_compact_node)) +0x0.
  The `ld` at 0x3cd34 loads 8 bytes from abs 0xffffffffffffc400.
  The `ld` at 0x3cd38 loads 8 bytes from [-0x3c00]:8 +0x0.
  The `ld` at 0x3cd44 loads 8 bytes from ([[-0x3c00]:8]:8 + (entry(sym(nasid_to_compact_node)) << 0x3)) +0x0.
  The `ld` at 0x3cd48 loads 8 bytes from [([[-0x3c00]:8]:8 + (entry(sym(nasid_to_compact_node)) << 0x3))]:8 +0x5140.
  The `jal` at 0x3c69c calls cer_to_hex with a0=sym(.rodata)+50024, a1=([a1+24] << 0x0), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x3c740 calls eccf_recoverable_threshold with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=([a1+24] << 0x0), a3=a3 at entry (arg3).
  The `jal` at 0x3c7b4 calls ecc_cacheOp [0x3bea0] with a0=0x4, a1=?, a2=?, a3=?.
  The `jal` at 0x3c800 calls ecc_cacheOp [0x3bea0] with a0=0x0, a1=table(0xa800000000000000), a2=?, a3=?.
  The `jal` at 0x3c818 calls ecc_cacheOp [0x3bea0] with a0=0x0, a1=?, a2=?, a3=?.
  The `jal` at 0x3c828 calls ecc_force_cache_line_invalid with a0=table(0xa800000000000000), a1=0x0, a2=0x0, a3=?.
  The `jal` at 0x3c838 calls ecc_force_cache_line_invalid with a0=table(0xa800000000000000), a1=0x1, a2=0x0, a3=?.
  The `jal` at 0x3c8e8 calls save_cache_error with a0=([a1+24] << 0x0), a1=[a1+8]:8, a2=[a1+16]:8, a3=?.
  The `jal` at 0x3c9b8 calls ecc_cacheOp [0x3bea0] with a0=0x1, a1=?, a2=?, a3=?.
  The `jal` at 0x3c9f0 calls ecc_cacheOp [0x3bea0] with a0=0x5, a1=?, a2=?, a3=?.
  The `jal` at 0x3cc08 calls clear_sline with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x3cc2c calls ecc_cacheOp [0x3bea0] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0x3cc68 calls ecc_cacheOp [0x3bea0] with a0=0x7, a1=?, a2=?, a3=?.
  The `jal` at 0x3cc94 calls memerror_handle_uce with a0=([a1+24] << 0x0), a1=ret(ecc_cacheOp), a2=a0 at entry (arg0), a3=a1 at entry (arg1).
  The `jal` at 0x3ccac calls ecc_cacheOp [0x3bea0] with a0=0x3, a1=?, a2=?, a3=?.
  The `jal` at 0x3ccbc calls ecc_force_cache_line_invalid with a0=table(0xa800000000000000), a1=?, a2=0x3, a3=?.
  The `jal` at 0x3cd80 calls cmn_err_tag with a0=0xb96, a1=0x4, a2=sym(.rodata)+50024, a3=table(0xa800000000000000), a7=0xfffffc000.
  The `bgez` branch at 0x3c678 targets 0x3c6ac, is taken when at(=([a1+24] << 0x22)) >= 0 (signed), and falls through when at(=([a1+24] << 0x22)) < 0 (signed).
  The `beq` branch at 0x3c6a4 targets 0x3c8f4 and is always taken.
  The `beq` branch at 0x3c6bc targets 0x3c734, is taken when t8(=[a1+700]) == t9(?), and falls through when t8(=[a1+700]) != t9(?).
  The `bne` branch at 0x3c6c8 targets 0x3c6e8, is taken when v0(=[a1+696]) < 0x11 (signed), and falls through when v0(=[a1+696]) >= 0x11 (signed).
  The `beq` branch at 0x3c72c targets 0x3c738 and is always taken.
  The `beq` branch at 0x3c764 targets 0x3c924, is taken when (a1(=0xc0000000) & s2(=[a1+24])) == 0, and falls through when (a1(=0xc0000000) & s2(=[a1+24])) != 0.
  The `beq` branch at 0x3c770 targets 0x3c958, is taken when a0(?) == a2(=0x40000000), and falls through when a0(?) != a2(=0x40000000).
  The `beq` branch at 0x3c77c targets 0x3caa4, is taken when a0(?) == a3(=0x80000000), and falls through when a0(?) != a3(=0x80000000).
  The `beq` branch at 0x3c784 targets 0x3cb44, is taken when a1(=0xc0000000) == a0(?), and falls through when a1(=0xc0000000) != a0(?).
  The `bgez` branch at 0x3c7a4 targets 0x3c854, is taken when s7(?) >= 0 (signed), and falls through when s7(?) < 0 (signed).
  The `beq` branch at 0x3c7ac targets 0x3c874 and is always taken.
  The `beq` branch at 0x3c7cc targets 0x3c7fc, is taken when s0(?) >= 0x2 (unsigned), and falls through when s0(?) < 0x2 (unsigned).
  The `bne` branch at 0x3c7e0 targets 0x3c7b4, is taken when (s2(=[a1+24]) & t9(=one of 0x5500000, 0xaa00000)) != 0, and falls through when (s2(=[a1+24]) & t9(=one of 0x5500000, 0xaa00000)) == 0.
  The `bne` branch at 0x3c7f4 targets 0x3c7d4, is taken when s0(?) < 0x2 (unsigned), and falls through when s0(?) >= 0x2 (unsigned).
  The `beq` branch at 0x3c844 targets 0x3cad0, is taken when at(=ret(eccf_recoverable_threshold)) == 0, and falls through when at(=ret(eccf_recoverable_threshold)) != 0.
  The `bltz` branch at 0x3c850 targets 0x3c874, is taken when s7(?) < 0 (signed), and falls through when s7(?) >= 0 (signed).
  The `beq` branch at 0x3c874 targets 0x3c8d8, is taken when s3(?) == 0, and falls through when s3(?) != 0.
  The `beq` branch at 0x3c884 targets 0x3c8d8, is taken when a1(=0x2) == a2(=[a1+38]:1), and falls through when a1(=0x2) != a2(=[a1+38]:1).
  The `bgtz` branch at 0x3c8d0 targets 0x3c8a8, is taken when a4(?) > 0 (signed), and falls through when a4(?) <= 0 (signed).
  The `beq` branch at 0x3c8d8 targets 0x3c8f4, is taken when v0(=one of 0x0, 0x1) == 0, and falls through when v0(=one of 0x0, 0x1) != 0.
  The `beq` branch at 0x3c950 targets 0x3c7d4 and is always taken.
  The `beq` branch at 0x3c96c targets 0x3cb80, is taken when (s6(=0x5500000) & s2(=[a1+24])) == 0, and falls through when (s6(=0x5500000) & s2(=[a1+24])) != 0.
  The `beq` branch at 0x3c9ac targets 0x3c9dc and is always taken.
  The `beq` branch at 0x3c9d4 targets 0x3ca70, is taken when s3(?) >= 0x2 (unsigned), and falls through when s3(?) < 0x2 (unsigned).
  The `beq` branch at 0x3c9e8 targets 0x3c9c0, is taken when (s2(=[a1+24]) & a1(=one of 0x5500000, 0xaa00000)) == 0, and falls through when (s2(=[a1+24]) & a1(=one of 0x5500000, 0xaa00000)) != 0.
  The `beq` branch at 0x3ca0c targets 0x3ca20, is taken when a3(?) == 0, and falls through when a3(?) != 0.
  The `beq` branch at 0x3ca1c targets 0x3cb04, is taken when a1(=0x3) == a4(?), and falls through when a1(=0x3) != a4(?).
  The `beq` branch at 0x3ca24 targets 0x3ca34, is taken when a5(?) == 0, and falls through when a5(?) != 0.
  The `beq` branch at 0x3ca2c targets 0x3cb24, is taken when a0(=0x2) == a6(?), and falls through when a0(=0x2) != a6(?).
  The `beq` branch at 0x3ca34 targets 0x3c9b4, is taken when a7(?) == 0, and falls through when a7(?) != 0.
  The `bne` branch at 0x3ca40 targets 0x3c9b4, is taken when a1(=0x3) != t0(?), and falls through when a1(=0x3) == t0(?).
  The `bne` branch at 0x3ca48 targets 0x3c9b8, is taken when a0(=0x2) != t1(?), and falls through when a0(=0x2) == t1(?).
  The `beq` branch at 0x3ca68 targets 0x3ca8c and is always taken.
  The `bgez` branch at 0x3ca90 targets 0x3caf0, is taken when t8(=([a1+24] << 0x23)) >= 0 (signed), and falls through when t8(=([a1+24] << 0x23)) < 0 (signed).
  The `beq` branch at 0x3ca9c targets 0x3c850 and is always taken.
  The `beq` branch at 0x3cac8 targets 0x3c850 and is always taken.
  The `beq` branch at 0x3cae8 targets 0x3c84c and is always taken.
  The `beq` branch at 0x3caf4 targets 0x3cbb8, is taken when t8(=ret(eccf_recoverable_threshold)) == 0, and falls through when t8(=ret(eccf_recoverable_threshold)) != 0.
  The `beq` branch at 0x3cafc targets 0x3c850 and is always taken.
  The `beq` branch at 0x3cb1c targets 0x3ca8c and is always taken.
  The `beq` branch at 0x3cb3c targets 0x3ca8c and is always taken.
  The `beq` branch at 0x3cb54 targets 0x3cbc0, is taken when (s2(=[a1+24]) & t9(=0x3800000)) == 0, and falls through when (s2(=[a1+24]) & t9(=0x3800000)) != 0.
  The `beq` branch at 0x3cb78 targets 0x3c850 and is always taken.
  The `bne` branch at 0x3cb84 targets 0x3c974, is taken when (s4(=0xaa00000) & s2(=[a1+24])) != 0, and falls through when (s4(=0xaa00000) & s2(=[a1+24])) == 0.
  The `bgez` branch at 0x3cb8c targets 0x3cdd8, is taken when t9(=([a1+24] << 0x23)) >= 0 (signed), and falls through when t9(=([a1+24] << 0x23)) < 0 (signed).
  The `beq` branch at 0x3cbb0 targets 0x3c850 and is always taken.
  The `beq` branch at 0x3cbb8 targets 0x3cafc and is always taken.
  The `bgez` branch at 0x3cbc4 targets 0x3cdb0, is taken when t8(=([a1+24] << 0x25)) >= 0 (signed), and falls through when t8(=([a1+24] << 0x25)) < 0 (signed).
  The `beq` branch at 0x3cbf8 targets 0x3cc50 and is always taken.
  The `bne` branch at 0x3cc20 targets 0x3cc08, is taken when t8(?) < 0x8 (signed), and falls through when t8(?) >= 0x8 (signed).
  The `beq` branch at 0x3cc48 targets 0x3cccc, is taken when s3(?) >= 0x2 (unsigned), and falls through when s3(?) < 0x2 (unsigned).
  The `beq` branch at 0x3cc60 targets 0x3cc34, is taken when (s2(=[a1+24]) & at(=one of 0x4000000, 0x8000000)) == 0, and falls through when (s2(=[a1+24]) & at(=one of 0x4000000, 0x8000000)) != 0.
  The `beq` branch at 0x3cc80 targets 0x3cc34, is taken when (s6(?) & 0x3) == 0, and falls through when (s6(?) & 0x3) != 0.
  The `bne` branch at 0x3cc9c targets 0x3cc00, is taken when v0(=ret(memerror_handle_uce)) != 0, and falls through when v0(=ret(memerror_handle_uce)) == 0.
  The `beq` branch at 0x3cca4 targets 0x3cd90, is taken when t9(=0x3) == s6(?), and falls through when t9(=0x3) != s6(?).
  The `beq` branch at 0x3ccc4 targets 0x3cc38 and is always taken.
  The `beq` branch at 0x3ccec targets 0x3ccfc, is taken when t8(=ret(eccf_recoverable_threshold)) == 0, and falls through when t8(=ret(eccf_recoverable_threshold)) != 0.
  The `beq` branch at 0x3ccf4 targets 0x3c850 and is always taken.
  The `beq` branch at 0x3cd2c targets 0x3cdf8, is taken when a2(=entry(sym(nasid_to_compact_node))) == t9(=-0x1), and falls through when a2(=entry(sym(nasid_to_compact_node))) != t9(=-0x1).
  The `beq` branch at 0x3cd88 targets 0x3ccf4 and is always taken.
  The `beq` branch at 0x3cda8 targets 0x3cce8 and is always taken.
  The `bltz` branch at 0x3cdb0 targets 0x3cbcc, is taken when t8(=([a1+24] << 0x24)) < 0 (signed), and falls through when t8(=([a1+24] << 0x24)) >= 0 (signed).
  The `beq` branch at 0x3cdd0 targets 0x3c850 and is always taken.
  The `beq` branch at 0x3cdf0 targets 0x3cbb0 and is always taken.
  The `beq` branch at 0x3cdf8 targets 0x3cd4c and is always taken.
  The return at 0x3c91c has v0 = one of sym(.rodata)+50024, ?.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3dbdc
  **Inferred.** Reads the byte at 0xffffffffffffc523 (0x3dbdc), which is the same gp-relative `[-0x3add]` byte tested at 0x3dbe8 to choose between two paths, stores a word to 0xffffffffffffc514 (0x3dbe0) and loads it back (0x3dc1c). Each path calls `get_slice` and `get_nasid` and stores a doubleword at a computed hub address, offset from one of two bases (0x9200000001a20050 or 0x9200000001a20080). The name suggests writing a board LED register for the slice and node, which the two-base store supports, but the register layout and the meaning of the byte at 0xffffffffffffc523 are not identified.
  ABI: n64.
  Range: 0x3dbdc-0x3dc58 (124 bytes).
  It occupies 124 bytes.
  The `lbu` at 0x3dbdc loads 1 byte from abs 0xffffffffffffc523.
  The `sw` at 0x3dbe0 stores 4 bytes to abs 0xffffffffffffc514.
  The `lwu` at 0x3dc1c loads 4 bytes from abs 0xffffffffffffc514.
  The `sd` at 0x3dc28 stores 8 bytes to (((ret(get_slice) << one of 0x3, 0x5) + (ret(get_nasid) << 0x20)) + one of 0x9200000001a20050, 0x9200000001a20080) +0x0.
  The `jal` at 0x3dbf0 calls get_slice with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x3dbf8 calls get_nasid with a0=?, a1=?, a2=?, a3=?.
  The `jal` at 0x3dc2c calls get_slice with a0=a0 at entry (arg0), a1=a1 at entry (arg1), a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  The `jal` at 0x3dc34 calls get_nasid with a0=?, a1=?, a2=?, a3=?.
  The `beq` branch at 0x3dbe8 targets 0x3dc2c, is taken when at(=[-0x3add]:1) == 0, and falls through when at(=[-0x3add]:1) != 0.
  The `beq` branch at 0x3dc50 targets 0x3dc18 and is always taken.
  The return at 0x3dc24 has v0 = ret(get_nasid).
  Reviewed: 2026-11-12 (sonnet)
