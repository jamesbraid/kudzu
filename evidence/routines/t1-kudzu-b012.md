# Tier-1 routine readings, batch 12 (kudzu)

Role statements for the 25 routines at target indexes 300 to 324 of the tier-1 list of hardware-linked routines (located in the inventory). Each statement comes only from the disassembly facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the inventory and the binaries' symbol tables. Grades are inferred throughout. Nothing here is hardware-verified, and where a register or callee is unresolved the statement says so.

- `bte_init` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x12b2c
  **Inferred.** Stores to several fields of the structure at a0, from offset 0x0 up to 0x70, and calls spinlock_init at 0x12b68 on a0+0x68. It tests a flag word loaded through the global scache_linemask against a1 before choosing a path. The name suggests block-transfer engine setup, but the facts do not establish that, and the meaning of the stored fields is open.
  Reviewed: 2026-11-12 (sonnet)

- `save_cache_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x15cc4
  **Inferred.** Calls get_nasid at 0x15cd4, then loads a 64-bit field at offset 0x48 of a base the facts do not resolve and loops over word and byte loads from unresolved addresses. The loop count sits in t3 and is compared to 0x10 at 0x15d50, and the stores go to unresolved targets. The routine plausibly records cache-error state for the node that get_nasid reports, but the mapping from fields to meaning is not established.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_pi_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16670
  **Inferred.** Loads 8-byte words from the structure at a0 (offsets 0x50, 0x58, 0x400, 0x430, 0x440, 0x450, 0x460) and stores 8-byte words into the record at a1 (offsets 0x10, 0x28, 0x30, 0x38, 0x40). The words at a0+0x50 and a0+0x58 are tested for zero, and the facts do not pair them with a destination offset. The stores at a1+0x28 and +0x38 are gated by a flag at a0+1072 tested against 0x8000000000000000, and those at a1+0x30 and +0x40 by a flag at a0+1104 tested the same way; both are also gated by bit 0x2 of the 16-bit global at 0xffffffffffffc524 and by compares against 0x480041 and 0x1fffffe0. The name suggests a processor-interface error snapshot, and the loads and stores fit that shape, but which bits gate them is not established.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_md_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16770
  **Inferred.** Loads four 8-byte words from a0 at offsets 0x200050, 0x200060, 0x200070 and 0x200080 and stores them into a1 at offsets 0x8, 0x28, 0x18 and 0x38 respectively. The stores at a1+0x18, +0x28 and +0x38 are skipped when the matching loaded value is zero. When the first word is zero, a sign test on [a0+0x200050] shifted left by one decides whether control goes straight to the load at 0x1678c or first stores at a1+0x8. The name suggests a memory-directory error snapshot, but the facts do not show what the fields mean.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_io_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x167dc
  **Inferred.** Calls bzero at 0x167f8 on a1 with length 0xc0, then copies seven 8-byte words from a0 (offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008, 0x420010) into a1 at offsets 0x8 through 0xb0. A loop near 0x16860 runs while a register is below 0xf. The routine looks like an I/O error snapshot that is cleared before filling, but the meaning of the destination fields comes only from the stores.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_err_ext` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x168b4
  **Inferred.** Calls bzero at 0x168d0 on a1 with length 0xc0, then copies three pairs of 8-byte words whose source registers are unresolved. A range dispatch on a2 (against 0xf and 0x9, signed) picks which group of fields gets copied. The sources and the dispatch meaning are not resolved by the facts, so this is only a shape description.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_ni_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1696c
  **Inferred.** Calls bzero at 0x16984 on a1 with length 0x20, copies two 8-byte words from a0 (offsets 0x600300 and 0x608008) into a1 at offsets 0x8 and 0x10, and returns a1. The name suggests a network-interface error snapshot. The facts show the copies and the clear, but not what the source words hold.
  Reviewed: 2026-11-12 (sonnet)

- `hub_error_init` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x190d8
  **Inferred.** Works on a per-node table indexed by a0 (the kl_error_log array and compact_to_nasid_node), looping over the node count at 0x19340. It calls hub_error_clear, get_compact_nodeid, recover_error_init (twice), cause_intr_connect at 0x192c4 with interrupt 0x6 and hubpi_eint_handler as the handler, hubii_eint_init, hubni_eint_init, and kmem_zalloc_node for two sizes (0x1938 and 0x40). Assertion calls to assfail at 0x1941c, 0x19438 and 0x19450, and a cmn_err at 0x19468, mark failure paths. This reads as per-node hub error-log setup with interrupt registration, but the purpose of interrupt 0x6 is only the immediate value in the call.
  Reviewed: 2026-11-12 (sonnet)

- `hubpi_eint_handler` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1970c
  **Inferred.** Calls check_ni_errors at 0x19714 first, and calls hubni_error_handler (a1=0x1) only when it returns nonzero. It then tests a status word (s0) against several bit groups (0x1 or 0x2, 0x40 or 0x80, 0x10 or 0x20, 0x100000 or 0x200000, 0x4 or 0x8) and against 0xcfff00. Depending on those tests and on a sign test of another loaded value it calls log_md_errors (after get_nasid), process_error_spool (three times, once with a5=0x80 and once with a5=0x20), enter_panic_mode, cmn_err and kl_panic. The routine reads as the processor-interface error interrupt handler that dispatches to the other error routines, but the meaning of each bit group is not established.
  Reviewed: 2026-11-12 (sonnet)

- `check_ni_errors` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19b24
  **Inferred.** Reads four fixed status words in the 0x9200000001 window (0x9200000001608008, 0x9200000001600000, 0x9200000001600010 and 0x92000000010000a0) and tests them against masks such as 0x3eff000000, 0x30000000, 0x2000000000 and the 0x200000000 to 0x1000000000 range. Matches are reported through a series of cmn_err calls, and protected_broadcast is called with 0x67 and 0x68 at 0x19c90 and 0x19c98. The routine returns 1 or 0 in v0.
  Reviewed: 2026-11-12 (sonnet)

- `hubni_error_handler` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19d80
  **Inferred.** Reads the NI status at 0x9200000001608008 and a related word at 0x9200000001600010, and tests bit 0x2000000000 of the status. When that bit is clear, or when it is set and the word at 0x9200000001600010 is nonzero, control reaches a splerr call. After it, if the byte at 0xffffffffffffc523 is nonzero, the routine calls set_node_panic_flag, hubni_print_state (a1=0x0, ip27log_printf), enter_panic_mode, router_print_state, enter_promlog_mode, then hubni_print_state and router_print_state again with cmn_err, and panicspin when a1 is zero. The ip27log_printf call at 0x19e14 runs only when a1 is nonzero. If that byte is zero it makes a cmn_err call with level 0 instead. When the bit is set and the word is zero, it calls cmn_err with level 0x15. The meaning of the bit and the word is not established.
  Reviewed: 2026-11-12 (sonnet)

- `klsave_spool_registers` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1a828
  **Inferred.** Calls get_nasid five times, then find_lboard with a second argument of 0x11, then find_first_component on its result with a second argument of 0x2. It reads a 32-bit field at find_first_component's result plus 0x30, and a per-node table entry, and stores three 8-byte words near the end. The name suggests saving registers into a spool record, which the stores support, but the spool layout is not visible in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_error_status_war` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bc14
  **Inferred.** Reads three hub status words (0x9200000001000020, 0x9200000001000450, 0x9200000001000430). On one path it calls _badaddr_val at 0x1bc64 on the address 0x9200000002400208 with size 8, a probe that asks whether that address responds. The name suggests a hardware workaround, and the probe fits a check that a register is present, but the facts do not show what the workaround corrects.
  Reviewed: 2026-11-12 (sonnet)

- `ii_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bc74
  **Inferred.** Does load and store pairs of 8 bytes at offset 0x0 (twice) and 0x8, and of single bytes at offsets 0x10 and 0x11. The facts do not resolve the base registers, so which pairs are at a0 and which touch a hardware word is open. It compares two registers against 0xffff before the write-back. Because it reads and rewrites state, it may be a counter or a clear-on-read record, but the facts do not show whether the write-back clears or increments.
  Reviewed: 2026-11-12 (sonnet)

- `ni_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bcdc
  **Inferred.** Reads the NI status at 0x9200000001608008 and passes the address 0x9200000001608088 to cmn_err in a5. It copies 8-byte fields at a0 offsets 0x0, 0x8 and 0x10 and single bytes at 0x18, 0x19 and 0x1a through load and store pairs, and logs through cmn_err at 0x1bd90 when the status has any of bits 0x3e00000000 set. It follows the same pattern as ii_llp_error_monitor, and the name suggests a link-level protocol monitor for the NI port.
  Reviewed: 2026-11-12 (sonnet)

- `err_int_unblock_mask` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1c228
  **Inferred.** Role (from name): err_int_unblock_mask suggests re-enabling error interrupts, which the facts do not show. It loads a 16-bit value, a 64-bit field at +0x8 and a byte at +0x520, from bases the facts do not resolve, and makes a 64-bit store only when a1 is nonzero. It also does a load and a store at an offset that equals 0x9200000001000000 modulo 2^64 added to a register the facts leave unresolved. The mask semantics and the branch on a1 are not established.
  Reviewed: 2026-11-12 (sonnet)

- `handle_io_buserr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d31c
  **Inferred.** Calls get_matching_spool_addr at 0x1d378, then kl_ioerror_handler up to three times (0x1d41c, 0x1d48c, 0x1d540), each with a fourth argument of 0x5 and a table entry derived from nasid_to_compact_node as its first argument, and logs through cmn_err_tag with codes 0xb6c and 0xb6d. It reads a field at a0+0x110, which it passes to kl_ioerror_handler in a5. It tests the spool-address result against 0, -1, 1 and 2, and tests mask 0x100000 or 0x200000 against the word at 0x9200000001000400. The return value is 0x1 or 0x2, and the meanings of those values are not established.
  Reviewed: 2026-11-12 (sonnet)

- `handle_uncac_buserr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d564
  **Inferred.** Calls get_matching_spool_addr at 0x1d598, then routes through many cmn_err_tag messages (codes 0xb6e through 0xb79) and partition_handle_error with small codes from 0x1 to 0xa. It checks partition IDs through nasid_to_partid and part_id, and calls error_check_memory_access, error_check_part_page, error_check_poisoned_state and error_page_discard. The return values include 0x2, 0x3 and the results of partition_handle_error, error_check_memory_access and error_check_poisoned_state. The routine looks like the uncached bus-error handler, and the meanings of the small codes are known only from the callees' names.
  Reviewed: 2026-11-12 (sonnet)

- `handle_cac_buserr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1da00
  **Inferred.** The largest routine in this batch. It calls get_matching_spool_addr at 0x1da38, log_md_errors at 0x1daf8 and kl_panic at 0x1dae4, then uses nasid_to_partid, part_id, nasid_to_region, error_lookup_inst_type, error_check_memory_access, error_check_part_page, error_page_discard, memory_iscalias, klerror_check_recoverable, verify_snchip_rev, dump_crb and get_dir_ent. It issues about eighteen cmn_err_tag messages (codes 0xb7a through 0xb8c). The shape matches the uncached handler with cache-line checks added, but the path through each branch is not traced here.
  Reviewed: 2026-11-12 (sonnet)

- `intpend0` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1fc28
  **Inferred.** Loads 64-bit words at 0xffffffffffffc598 (ANDed with the word at 0x9200000001000098 to decide whether to loop) and 0xffffffffffffc508 (passed to ms1bit at 0x1fca0), and uses the pointer at 0xffffffffffffc5a8 as the base of a table of 72-byte entries indexed by the ms1bit result. It calls cvsema at 0x1fc78 and valusema and vsema on the entry plus 32, and makes indirect calls at 0x1fce4 and 0x1fd78 through the field at entry+0x10. The callee of those indirect calls is unresolved, and the loop logic around hub_intr_wakeup_cnt is only partly traced. That the words hold pending-interrupt state is not established.
  Reviewed: 2026-11-12 (sonnet)

- `intpend1` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1fd88
  **Inferred.** Has the same shape as intpend0. It loads a 64-bit word at 0xffffffffffffc5a0 (ANDed with the word at 0x92000000010000a0) and uses the pointer at 0xffffffffffffc5b0 as the base of a table of 72-byte entries indexed by the ms1bit result from the call at 0x1fdd4. It makes an indirect call at 0x1fe0c through the field at entry+0x10, with the bit index plus 64 in a7. When the entry's first word is zero it reports through cmn_err at 0x1fe64 with a level of 0 and the bit index. Which interrupt group the routine serves is not established.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x201dc
  **Inferred.** Loads a 64-bit field at +0x8, a 16-bit field at +0x424 and a byte at +0x520 from bases the facts do not resolve. It branches on a1 == 0xab, returns 0 on one path, and makes a 64-bit store at 0x20218 to an address the facts leave as a register plus a large offset. The stored value, the base of the loads and the meaning of 0xab are not shown by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpuintr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20224
  **Inferred.** Reads a few fixed interrupt-state words, then copies 0x168 bytes with bcopy at 0x202c8. It calls doacvec and doactions in turn, and logs timestamp events 0x71 and 0x65 through log_tstamp_event at 0x2030c and 0x20328. Flag tests against a byte at [-0x3f80] and a word at [-0x3d80] gate parts of the path. The routine reads as the CPU-level interrupt entry, but the copy destination is not named.
  Reviewed: 2026-11-12 (sonnet)

- `dbgintr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20398
  **Inferred.** Loads a 16-bit value at 0xffffffffffffc420 and a per-CPU record through pdaindr (a 64-bit field at +0x8 and a byte at +0x520), does a 64-bit store, and calls debug at 0x203d0 with a format string in a0, the record byte at +0x520 plus 118 in a1 and the address 0x9200000001000090 in a2. A further load follows the call, with an unresolved address, and the routine returns debug's result. The name suggests the debugger interrupt entry, and the call to debug supports that reading.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20690
  **Inferred.** Loads a 16-bit value, a 64-bit field at +0x8 and a byte at +0x520 from bases the facts do not resolve, then tests the word at 0x92000000010000a0 against a value in v0. When the result is nonzero it calls dbgintr (the routine at 0x20398) with a0=0 and passes its own a1, a2 and a3 through unchanged. The register v0 that gates the call is not resolved by the facts.
  Reviewed: 2026-11-12 (sonnet)
