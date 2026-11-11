# Tier-2 routine readings, kudzu round 2 batch 4

- `hubdev_docallouts` in `6.5.22/unix.IP27` at 0xc00000000004b7a4
  **Inferred.** Takes the mutex at 0xc0000000014dd3b8 (`mutex_lock` at 0xc00000000018dda4) and loops over a count in s0, making an indirect call at 0xc00000000004b7dc while the byte flag at 0xc0000000014c9088 is nonzero. A nonzero result from that call goes straight to the unlock at 0xc00000000004b820, and the other exit unlocks at 0xc00000000004b7f8 (`mutex_unlock` at 0xc00000000018dcdc). The rendered facts do not resolve the indirect target.

- `hubdev_prombase_get` in `6.5.22/unix.IP27` at 0xc00000000004b830
  **Inferred.** Calls `hwgraph_info_get_LBL` at 0xc000000000296160 with its first argument passed through and a stack buffer at sp-16, then loads a halfword at 0xc00000000004b854 from an address the facts do not resolve. The return at 0xc00000000004b864 has v0 shown only as table(0x9000000030000000), which the facts do not expand.

- `hubdev_cnodeid_get` in `6.5.22/unix.IP27` at 0xc00000000004b86c
  **Inferred.** Calls `hwgraph_info_get_LBL` at 0xc000000000296160 with a1 set to 0xc00000000140ebc0 and a stack buffer at sp-16, then loads a halfword at 0xc00000000004b890 from an address the facts do not resolve. The return at 0xc00000000004b894 has an unresolved v0, so the facts do not show what value it returns.

- `intrnorm_rsasave` in `6.5.22/unix.IP27` at 0xc00000000004b8d0
  **Inferred.** Reads state words at abs 0xffffffffffffc018, 0xffffffffffffc030 and 0xffffffffffffc038 and at [-0x3fe8]:8, tests them against k0 and k1 with a run of branches to 0xc00000000004baf8, and stores saved values through pointers the facts do not resolve. It tail-jumps to `ecommon_allsaved` at 0xc00000000004bfa0 from 0xc00000000004b9fc. The use of k0 and k1 as scratch suggests an exception entry path, but the facts do not show which state it saves.

- `intrnorm` in `6.5.22/unix.IP27` at 0xc00000000004ba04
  **Inferred.** A large dispatch routine on the same path as `intrnorm_rsasave` at 0xc00000000004b8d0. It tests bit fields and byte flags at abs 0xffffffffffffc04f and [-0x3fb1] with k0 and k1, calls `tfi_save` at 0xc00000000003a240 (jal at 0xc00000000004bc6c) and `uli_setasids` at 0xc00000000004330c (jal at 0xc00000000004bc80), and makes an unresolved indirect call at 0xc00000000004bd7c. It tail-jumps to `ecommon` at 0xc00000000004bf50 or `ecommon_allsaved` at 0xc00000000004bfa0, depending on the path. The facts do not settle which condition selects each path.

- `get_freemem_start` in `6.5.22/unix.IP27` at 0xc00000000004fd3c
  **Inferred.** Indexes a table through the halfword at [-0x3bde]:2 and the pointer at [-0x3c00]:8, and returns one of two values built by OR-ing a loaded word with the constants 0x40382b and 0x5b1bc0 (returns at 0xc00000000004fdac and 0xc00000000004fe28). A zero a0 branches at 0xc00000000004fd48 to 0xc00000000004fdfc, which returns an unresolved value. The facts do not show what the returned address refers to.

- `setup_replication_mask` in `6.5.22/unix.IP27` at 0xc00000000004fe38
  **Inferred.** Stores two words to ktext_repmask at 0xc0000000014dd3c8 and ktext_repmask+8 at 0xc0000000014dd3d0 (stores at 0xc00000000004fe58 and 0xc00000000004fe4c). It calls `get_nasid` at 0xc00000000001d434 with a5 set to 1 (jal at 0xc00000000004feb4) and goes back to that call while a0 is below 2 (branch at 0xc00000000004fe54) and a2 is at least a0 (branch at 0xc00000000004fe74). For the returned id it writes a word at offset 0x40 of a structure at a computed address (sd at 0xc00000000004fed8). The facts do not show how the mask words are used afterwards.

- `ecc_cacheOp` in `6.5.22/unix.IP27` at 0xc000000000055020
  **Inferred.** Calls `cacheOP` at 0xc00000000002b8c8 (jal at 0xc000000000055030) with a0 set to a 32-byte stack buffer at sp-32 and its own a1 to a3 passed through. It returns the unresolved value at 0xc00000000005504c. The rendered facts do not show what the buffer holds.

- `ecc_cacheop_get` in `6.5.22/unix.IP27` at 0xc000000000055054
  **Inferred.** Stores a 64-bit value at offset 0 and a 32-bit value at offset 8 of the record in a2 (stores at 0xc000000000055054 and 0xc000000000055058). It then calls `cacheOP` at 0xc00000000002b8c8 with a0 set to that record (jal at 0xc000000000055064) and returns that call's result (return at 0xc000000000055070). The sources of the stored values are not in the facts.

- `ecc_display` in `6.5.22/unix.IP27` at 0xc000000000055078
  **Inferred.** A large report routine that tests address bits against the masks 0xffffffffc0000000, 0x40000000 and 0x80000000 and branches on a 16-bit entry from the table at 0xc0000000014d3600. It calls `cmn_err_tag` at 0xc0000000001f80dc with message numbers 0xb90, 0xb91, 0xb92, 0xb94 and 0xb95 (jal sites at 0xc00000000005515c through 0xc000000000055404). It calls `error_page_discard` at 0xc0000000000379e0 on one path and `mdir_xlate_addr_mem` at 0xc000000000048fe4 on another. The facts show the calls and masks but not what each message number reports.

- `ecc_cleanRegion` in `6.5.22/unix.IP27` at 0xc0000000000554b0
  **Inferred.** Loops while a1 is positive (test at 0xc0000000000554c4) and while s1 is positive (test at 0xc0000000000554dc), calling `ecc_cacheOp` at 0xc000000000055020 with 0x17 as its first argument on each pass (jal at 0xc0000000000554d0). It returns either an unresolved value or the result of that call. The rendered facts do not show what region the loop covers.

- `ecc_init` in `6.5.22/unix.IP27` at 0xc0000000000554f8
  **Inferred.** Allocates with `kmem_zalloc` at 0xc000000000181ce0, once with a0 = 0x2000 (jal at 0xc000000000055514) and once with a0 = 0x1000 (jal at 0xc0000000000555d0). It clears 0x1000 bytes through `ecc_cleanRegion` at 0xc0000000000554b0 (jal at 0xc00000000005555c) and stores the block pointers at 0x9000000000000fe0 and 0x9000000000000fd8 (stores at 0xc000000000055588 and 0xc0000000000555b8). It reports failure through `cmn_err` at 0xc0000000001f8090 (jal at 0xc0000000000555fc). The facts do not show which code reads those stored pointers.

- `ecc_handler` in `6.5.22/unix.IP27` at 0xc0000000000557c0
  **Inferred.** A large error handler that reads a record passed in a1 (fields at +0x0, +0x18, +0x28 to +0x34, +0x2b8 and +0x2bc) and checks it against the masks 0x40000000 and 0x80000000. It asks `eccf_recoverable_threshold` at 0xc00000000005560c whether the error is recoverable (call at 0xc0000000000558c0, branch at 0xc0000000000559c4). It runs `ecc_cacheOp` at 0xc000000000055020 with operation codes 0x0, 0x1, 0x3, 0x4, 0x5 and 0x7, and reports uncorrectable errors through `memerror_handle_uce` at 0xc000000000048acc and `cmn_err_tag` message 0xb96 (call at 0xc000000000055f00). Which operation runs for which error class is not settled by the facts.

- `ecc_panic` in `6.5.22/unix.IP27` at 0xc000000000055f80
  **Inferred.** Calls `machine_error_dump` at 0xc0000000001fc1b8 with its arguments passed through (jal at 0xc000000000055f98), then `ecc_display` at 0xc000000000055078 with a0 = 0x14 and fields from a2 at +24 and +8 (jal at 0xc000000000055fb0). It finishes with `cmn_err` at 0xc0000000001f8090 with a0 = 0x10 and returns its result. The name suggests a panic path, but the rendered facts show no halt, so they do not confirm it.

- `ecc_printPending` in `6.5.22/unix.IP27` at 0xc000000000056118
  **Inferred.** Walks pending error entries, reading fields through the pointer at pdaindr+8 (`ld` at 0xc000000000056294, address 0xc0000000014d39a8). It calls one of three indirect routines at 0xc0000000000561d0, 0xc0000000000561ec or 0xc000000000056224, each with a0 set to a fixed buffer (0xc00000000140ff78, 0xc00000000140ff90 or 0xc00000000140ffc8). The loop bounds are s4 (tested against 0x80 at 0xc000000000056198) and s1 (compared with t9 and a2). The facts do not resolve the three indirect targets.

- `ecc_compute_sctag_ecc` in `6.5.22/unix.IP27` at 0xc000000000056298
  **Inferred.** Calls `ecc_bitcount` at 0xc000000000056518 (jal at 0xc0000000000562e4) and keeps the low bit of its result as a check bit (branch at 0xc0000000000562f4). A loop on a3 (branch at 0xc000000000056300) repeats the computation while a3 is nonzero. The rendered facts do not show the value it returns.

- `ecc_compute_scdata_ecc` in `6.5.22/unix.IP27` at 0xc000000000056450
  **Inferred.** Calls `ecc_bitcount` at 0xc000000000056518 twice per pass (jal sites at 0xc0000000000564b8 and 0xc0000000000564e0) and takes a parity bit from the low bit of each result (branches at 0xc0000000000564c8 and 0xc0000000000564fc). A loop on a4 (branch at 0xc0000000000564d4) repeats the pass while a4 is nonzero. The facts do not show the value it returns.

- `_check_dbg` in `6.5.22/unix.IP27` at 0xc00000000005b4b8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4b8.

- `tpusim_version` in `6.5.22/unix.IP27` at 0xc00000000005b4c0
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4c0.

- `tpusim_info_chk` in `6.5.22/unix.IP27` at 0xc00000000005b4c8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4c8.

- `tpusim_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000005b4d0
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4d0.

- `tpusim_dmatrans_list` in `6.5.22/unix.IP27` at 0xc00000000005b4d8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4d8.

- `tpusim_intr_alloc` in `6.5.22/unix.IP27` at 0xc00000000005b4e0
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4e0.

- `tpusim_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000005b4e8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4e8.

- `tpuidbg_version` in `6.5.22/unix.IP27` at 0xc00000000005b4f8
  **Established.** Returns 0 in v0. The routine is 8 bytes, and the rendered facts show no calls, branches or memory accesses, only the return with v0 = 0x0 at 0xc00000000005b4f8.
