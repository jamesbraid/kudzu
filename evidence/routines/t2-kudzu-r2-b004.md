# Tier-2 routine readings, kudzu round 2 batch 4

- `hubdev_docallouts` in `6.5.22/unix.IP27` at 0xc00000000004b7a4
  **Inferred.** Takes the mutex at 0xc0000000014dd3b8 (`mutex_lock` at 0xc00000000018dda4, a1 = 0x19), then tests the doubleword at 0xc0000000014c9088 once (branch at 0xc00000000004b7d0): zero skips to the unlock at 0xc00000000004b7f8. Otherwise it enters a loop that makes an indirect call at 0xc00000000004b7dc and repeats while s0 is nonzero (branch at 0xc00000000004b7f0). A nonzero result from the indirect call goes straight to the unlock at 0xc00000000004b820, and the other exits unlock at 0xc00000000004b7f8 (`mutex_unlock` at 0xc00000000018dcdc). The rendered facts do not resolve the indirect target or what s0 counts.
  Reviewed: 2026-11-12 (sonnet)

- `hubdev_prombase_get` in `6.5.22/unix.IP27` at 0xc00000000004b830
  **Inferred.** Calls `hwgraph_info_get_LBL` at 0xc000000000296160 with its first argument passed through and a stack buffer at sp-16, then loads a halfword at 0xc00000000004b854 from an address the facts do not resolve. The return at 0xc00000000004b864 has v0 shown only as table(0x9000000030000000), which the facts do not expand.
  Reviewed: 2026-11-12 (sonnet)

- `hubdev_cnodeid_get` in `6.5.22/unix.IP27` at 0xc00000000004b86c
  **Inferred.** Calls `hwgraph_info_get_LBL` at 0xc000000000296160 with a1 set to 0xc00000000140ebc0 and a stack buffer at sp-16, then loads a halfword at 0xc00000000004b890 from an address the facts do not resolve. The return at 0xc00000000004b894 has an unresolved v0, so the facts do not show what value it returns.
  Reviewed: 2026-11-12 (sonnet)

- `intrnorm_rsasave` in `6.5.22/unix.IP27` at 0xc00000000004b8d0
  **Inferred.** Opens with loads and a run of branches that mostly jump to 0xc00000000004baf8 (the first, at 0xc00000000004b8d4, jumps to 0xc00000000004bb00 when k0 is zero); the compared values are mostly unresolved in the facts, apart from an unsigned comparison against k1 at 0xc00000000004b91c. It then makes a long run of doubleword stores through pointers the facts do not resolve, and tail-jumps to `ecommon_allsaved` at 0xc00000000004bfa0 from 0xc00000000004b9fc. The use of k0 and k1 as scratch suggests an exception entry path, but the facts do not show which state it saves.
  Reviewed: 2026-11-12 (sonnet)

- `intrnorm` in `6.5.22/unix.IP27` at 0xc00000000004ba04
  **Inferred.** A long branching routine with a branch back to `intrnorm_rsasave` at 0xc00000000004b8d0 (bne at 0xc00000000004baf4). It tests byte values at [-0x3fb1] and other loaded words, calls `tfi_save` at 0xc00000000003a240 (jal at 0xc00000000004bc6c) and `uli_setasids` at 0xc00000000004330c (jal at 0xc00000000004bc80), and makes an indirect call at 0xc00000000004bd7c whose target the facts do not resolve. It ends in tail jumps to `ecommon` at 0xc00000000004bf50 (four sites) or `ecommon_allsaved` at 0xc00000000004bfa0 (calls at 0xc00000000004bbc4 and 0xc00000000004bc88). The facts do not settle which condition selects each exit.
  Reviewed: 2026-11-12 (sonnet)

- `get_freemem_start` in `6.5.22/unix.IP27` at 0xc00000000004fd3c
  **Inferred.** Loads a halfword at [-0x3bde]:2 and a pointer at [-0x3c00]:8 and uses them to index a table. A zero a0 branches at 0xc00000000004fd48 to 0xc00000000004fdfc, whose return at 0xc00000000004fe28 is a loaded doubleword OR 0x5b1bc0. For nonzero a0, the branch at 0xc00000000004fd78 on (a1 & a2) == 0 goes to 0xc00000000004fdb4; the other case returns at 0xc00000000004fdac a value built from a shifted entry and a loaded doubleword OR 0x40382b. From 0xc00000000004fdb4, a2 == 0 (halfword at 0xc0000000014ca72a, branch at 0xc00000000004fde4) returns an unresolved value at 0xc00000000004fe30, and nonzero a2 returns an unresolved value at 0xc00000000004fdf4. The facts do not show what the returned values refer to.
  Reviewed: 2026-11-12 (sonnet)

- `setup_replication_mask` in `6.5.22/unix.IP27` at 0xc00000000004fe38
  **Inferred.** Makes two doubleword stores (at 0xc00000000004fe4c and 0xc00000000004fe58) whose destinations the facts do not resolve. A branch at 0xc00000000004fe54 on a0 < 2 and a branch at 0xc00000000004fe74 on a2 >= a0 (signed) both jump to a call of `get_nasid` at 0xc00000000001d434 (jal at 0xc00000000004feb4, a5 = 1). After that call it loads a doubleword at offset 0xd0 (0xc00000000004fed4), stores a doubleword at offset 0x40 (0xc00000000004fed8) and returns the result of `get_nasid` (return at 0xc00000000004fee0). The facts do not show what the stored values are or how the mask words are used.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_cacheOp` in `6.5.22/unix.IP27` at 0xc000000000055020
  **Inferred.** Calls `cacheOP` at 0xc00000000002b8c8 (jal at 0xc000000000055030) with a0 set to the stack address sp-32 and its own a1 to a3 passed through. It returns an unresolved value at 0xc00000000005504c. The rendered facts do not show what is stored at sp-32 or what the return value is.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_cacheop_get` in `6.5.22/unix.IP27` at 0xc000000000055054
  **Inferred.** Stores a doubleword at offset 0 (0xc000000000055054) and a word at offset 8 (0xc000000000055058) from a base the facts do not resolve. It then calls `cacheOP` at 0xc00000000002b8c8 with a0 = a2 at entry and a1 to a3 passed through (jal at 0xc000000000055064) and returns that call's result (return at 0xc000000000055070). The sources of the stored values are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_display` in `6.5.22/unix.IP27` at 0xc000000000055078
  **Inferred.** A long routine that compares a6 and a2 against 0xffffffffc0000000 (branches at 0xc0000000000550dc and 0xc000000000055368), 0x40000000 (0xc0000000000550e8) and 0xffffffff80000000 (0xc0000000000552a0), and compares the value shown as entry(0xc0000000014d3600) with -1 (branch at 0xc00000000005545c). It calls `cmn_err_tag` at 0xc0000000001f80dc with message numbers 0xb90, 0xb91, 0xb92, 0xb94 and 0xb95 (jal sites at 0xc00000000005515c through 0xc000000000055404), `error_page_discard` at 0xc0000000000379e0 (jal at 0xc0000000000553f0) and `mdir_xlate_addr_mem` at 0xc000000000048fe4 (jal at 0xc000000000055440). The facts show the calls and comparisons but not what each message number reports.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_cleanRegion` in `6.5.22/unix.IP27` at 0xc0000000000554b0
  **Inferred.** If a1 is not positive (blez at 0xc0000000000554c4) it branches past the loop to 0xc0000000000554e4. Otherwise it loops, calling `ecc_cacheOp` at 0xc000000000055020 with a0 = 0x17 on each pass (jal at 0xc0000000000554d0) while s1 is positive (bgtz at 0xc0000000000554dc). It returns either an unresolved value or the result of that call. The rendered facts do not show what region the loop covers.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_init` in `6.5.22/unix.IP27` at 0xc0000000000554f8
  **Inferred.** Calls `kmem_zalloc` at 0xc000000000181ce0 with a0 = 0x2000 (jal at 0xc000000000055514) when bit 1 of the halfword at [-0x3adc]:2 is set (branch at 0xc000000000055508), and otherwise with a0 = 0x1000 (jal at 0xc0000000000555d0). It calls `ecc_cleanRegion` at 0xc0000000000554b0 with a1 = 0x1000 (jal at 0xc00000000005555c), and makes doubleword stores at 0xc000000000055588 and 0xc0000000000555b8 whose destinations the facts do not resolve. If the second `kmem_zalloc` returns zero (branch at 0xc0000000000555d8) it calls `cmn_err` at 0xc0000000001f8090 (jal at 0xc0000000000555fc). The facts do not settle the path conditions for the other calls or which code reads the stored values.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/unix.IP27` at 0xc0000000000557c0
  **Inferred.** A long routine that reads fields of the record passed in a1 (the facts show offsets +0x18, +0x2b8 and +0x2bc) and compares the +0x18 word with masks including 0xc0000000, 0x40000000 and 0x80000000 (branches at 0xc0000000000558e4 through 0xc000000000055904). It calls `eccf_recoverable_threshold` at 0xc00000000005560c with a2 = the +0x18 word (call at 0xc0000000000558c0) and branches on its result at 0xc0000000000559c4; the name suggests a recoverability test, which the facts do not confirm. It calls `ecc_cacheOp` at 0xc000000000055020 with first arguments 0x0, 0x1, 0x3, 0x4, 0x5 and 0x7, `memerror_handle_uce` at 0xc000000000048acc (call at 0xc000000000055e14) and `cmn_err_tag` with message 0xb96 (call at 0xc000000000055f00). Which call runs for which error class is not settled by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_panic` in `6.5.22/unix.IP27` at 0xc000000000055f80
  **Inferred.** Calls `machine_error_dump` at 0xc0000000001fc1b8 with its arguments passed through (jal at 0xc000000000055f98), then `ecc_display` at 0xc000000000055078 with a0 = 0x14, a2 = the word at a2+24, a3 = the doubleword at a2+0 and a4 = a2+8 (jal at 0xc000000000055fb0). It finishes with `cmn_err` at 0xc0000000001f8090 with a0 = 0x10 and returns its result. The name suggests a panic path, but the rendered facts show no halt, so they do not confirm it.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_printPending` in `6.5.22/unix.IP27` at 0xc000000000056118
  **Inferred.** Contains a loop (backward branches at 0xc00000000005624c and 0xc0000000000561fc) that makes three indirect calls, at 0xc0000000000561d0, 0xc0000000000561ec and 0xc000000000056224, with a0 set to the fixed addresses 0xc00000000140ff78, 0xc00000000140ff90 and 0xc00000000140ffc8 respectively. The third is reached only when s1 differs from t9 (branch at 0xc0000000000561fc). Other branches compare s4 with 0x80 (0xc000000000056198) and s1 with a2 (0xc00000000005624c). The facts do not resolve the three indirect targets, the loaded values or the return value (unresolved at 0xc000000000056280).
  Reviewed: 2026-11-12 (sonnet)

- `ecc_compute_sctag_ecc` in `6.5.22/unix.IP27` at 0xc000000000056298
  **Inferred.** Calls `ecc_bitcount` at 0xc000000000056518 (jal at 0xc0000000000562e4) and tests the low bit of its result (branch at 0xc0000000000562f4). A backward branch at 0xc000000000056300 repeats from 0xc0000000000562d8 while a3 is nonzero. The rendered facts do not show how the tested bit is used or the value it returns.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_compute_scdata_ecc` in `6.5.22/unix.IP27` at 0xc000000000056450
  **Inferred.** Calls `ecc_bitcount` at 0xc000000000056518 in a loop (jal at 0xc0000000000564b8), tests the low bit of its result (branch at 0xc0000000000564c8), and repeats from 0xc000000000056498 while a4 is nonzero (branch at 0xc0000000000564d4). After the loop it calls `ecc_bitcount` once more (jal at 0xc0000000000564e0) and tests the low bit of that result (branch at 0xc0000000000564fc). The facts do not show how the tested bits are used or the value it returns.
  Reviewed: 2026-11-12 (sonnet)

- `_check_dbg` in `6.5.22/unix.IP27` at 0xc00000000005b4b8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4b8.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_version` in `6.5.22/unix.IP27` at 0xc00000000005b4c0
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4c0.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_info_chk` in `6.5.22/unix.IP27` at 0xc00000000005b4c8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4c8.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000005b4d0
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4d0.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_dmatrans_list` in `6.5.22/unix.IP27` at 0xc00000000005b4d8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4d8.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_intr_alloc` in `6.5.22/unix.IP27` at 0xc00000000005b4e0
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4e0.
  Reviewed: 2026-11-12 (sonnet)

- `tpusim_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000005b4e8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4e8.
  Reviewed: 2026-11-12 (sonnet)

- `tpuidbg_version` in `6.5.22/unix.IP27` at 0xc00000000005b4f8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4f8.
  Reviewed: 2026-11-12 (sonnet)
