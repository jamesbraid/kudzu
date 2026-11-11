# Tier-1 routine readings, batch 12 (kudzu)

Role statements for the 25 routines at target indexes 300 to 324 of the tier-1 list of hardware-linked routines (located in the inventory). Each statement comes only from the disassembly facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the inventory and the binaries' symbol tables. Grades are inferred throughout. Nothing here is hardware-verified, and where a register or callee is unresolved the statement says so.

- `bte_init` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x12b2c
  **Inferred.** Stores to several fields of the structure at a0, from offset 0x0 up to 0x70, and calls spinlock_init at 0x12b68 on a0+0x68. It tests a flag word loaded through the global scache_linemask against a1 before choosing a path. The name suggests block-transfer engine setup, but the facts do not establish that, and the meaning of the stored fields is open.

- `save_cache_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x15cc4
  **Inferred.** Calls get_nasid at 0x15cd4, then reads a per-node table entry (base 0x9600000000000000 with 0x2000 added, offset 0x48) and loops over word and byte loads from unresolved addresses. The loop count sits in t3 and is compared to 0x10 at 0x15d50, and the stores go to unresolved targets. The routine plausibly records cache-error state for the node that get_nasid reports, but the mapping from fields to meaning is not established.

- `save_hub_pi_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16670
  **Inferred.** Copies 8-byte words from the structure at a0 (offsets 0x50, 0x58, 0x400, 0x430, 0x440, 0x450, 0x460) into the record at a1 (offsets 0x10, 0x28, 0x30, 0x38, 0x40). Some copies are gated by a 16-bit global at 0xffffffffffffc524, or by a flag at a0+1072 and a0+1104 tested against 0x8000000000000000. The name suggests a processor-interface error snapshot, and the copies fit that shape, but which bits gate them is not established.

- `save_hub_md_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16770
  **Inferred.** Copies four 8-byte words from a0 at offsets 0x200050, 0x200060, 0x200070 and 0x200080 into a1 at offsets 0x8, 0x28, 0x18 and 0x38. Several branches skip a copy when a source value is zero, and a sign test on a5 shifted left by one controls the loop back edge. The name suggests a memory-directory error snapshot, but the facts do not show what the fields mean.

- `save_hub_io_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x167dc
  **Inferred.** Calls bzero at 0x167f8 on a1 with length 0xc0, then copies seven 8-byte words from a0 (offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008, 0x420010) into a1 at offsets 0x8 through 0xb0. A loop near 0x16860 runs while a register is below 0xf. The routine looks like an I/O error snapshot that is cleared before filling, but the meaning of the destination fields comes only from the stores.

- `save_hub_err_ext` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x168b4
  **Inferred.** Calls bzero at 0x168d0 on a1 with length 0xc0, then copies three pairs of 8-byte words whose source registers are unresolved. A range dispatch on a2 (against 0xf and 0x9, signed) picks which group of fields gets copied. The sources and the dispatch meaning are not resolved by the facts, so this is only a shape description.

- `save_hub_ni_error` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1696c
  **Inferred.** Calls bzero at 0x16984 on a1 with length 0x20, copies two 8-byte words from a0 (offsets 0x600300 and 0x608008) into a1 at offsets 0x8 and 0x10, and returns a1. The name suggests a network-interface error snapshot. The facts show the copies and the clear, but not what the source words hold.

- `hub_error_init` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x190d8
  **Inferred.** Works on a per-node table indexed by a0 (the kl_error_log array and compact_to_nasid_node), looping over the node count at 0x19340. It calls hub_error_clear, get_compact_nodeid, recover_error_init (twice), cause_intr_connect at 0x192c4 with interrupt 0x6 and hubpi_eint_handler as the handler, hubii_eint_init, hubni_eint_init, and kmem_zalloc_node for two sizes (0x1938 and 0x40). Assertion calls to assfail at 0x1941c, 0x19438 and 0x19450, and a cmn_err at 0x19468, mark failure paths. This reads as per-node hub error-log setup with interrupt registration, but the purpose of interrupt 0x6 is only the immediate value in the call.

- `hubpi_eint_handler` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1970c
  **Inferred.** Calls check_ni_errors at 0x19714 first, then tests a status word (s0) against several bit groups (0x1 or 0x2, 0x40 or 0x80, 0x10 or 0x20, 0x100000 or 0x200000, 0x4 or 0x8). Depending on those tests it calls hubni_error_handler, log_md_errors, process_error_spool (three times, once with a5=0x80 and once with a5=0x20), enter_panic_mode and kl_panic. The routine reads as the processor-interface error interrupt handler that dispatches to the other error routines, but the meaning of each bit group is not established.

- `check_ni_errors` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19b24
  **Inferred.** Reads five fixed network-interface status words in the 0x9200000001 window and tests them against masks such as 0x3eff000000, 0x30000000, 0x2000000000 and the 0x200000000 to 0x1000000000 range. Matches are reported through a series of cmn_err calls, and protected_broadcast is called with 0x67 and 0x68 at 0x19c90 and 0x19c98. The routine returns 1 or 0 in v0. Two blocks are not reached from entry, and their branches are not listed.

- `hubni_error_handler` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19d80
  **Inferred.** Reads the NI status at 0x9200000001608008 and a related word at 0x9200000001600010, and tests a bit (0x2000000000) against the status. On a match it sets a panic flag through set_node_panic_flag, prints NI and router state (hubni_print_state, router_print_state), and enters panic mode or promlog mode, with branches on a1 choosing between them. Otherwise it writes messages through ip27log_printf or cmn_err. The ordinary path is not clear from the facts.

- `klsave_spool_registers` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1a828
  **Inferred.** Calls get_nasid five times, then find_lboard with class 0x11, then find_first_component with type 0x2 on its result. It reads a 32-bit field at find_first_component's result plus 0x30, and a per-node table entry, and stores three 8-byte words near the end. The name suggests saving registers into a spool record, which the stores support, but the spool layout is not visible in the facts.

- `hub_error_status_war` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bc14
  **Inferred.** Reads three hub status words (0x9200000001000020, 0x9200000001000450, 0x9200000001000430). On one path it calls _badaddr_val at 0x1bc64 on the address 0x9200000002400208 with size 8, a probe that asks whether that address responds. The name suggests a hardware workaround, and the probe fits a check that a register is present, but the facts do not show what the workaround corrects.

- `ii_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bc74
  **Inferred.** Reads and writes back a 64-bit word at 0x9200000001400130, and copies 8-byte fields and two bytes at a0 offsets 0x0, 0x8, 0x10 and 0x11 through load and store pairs at the same offsets. It compares two registers against 0xffff before the write-back. The routine reads and rewrites state at a0, so it may be a counter or a clear-on-read record, but the facts do not show whether the write-back clears or increments.

- `ni_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bcdc
  **Inferred.** Reads the NI status at 0x9200000001608008 and a second word at 0x9200000001608088. It copies 8-byte fields and three bytes at a0 offsets 0x0 to 0x1a through read and write pairs, and logs through cmn_err at 0x1bd90 when the status has bits 0x3e00000000 set. It follows the same pattern as ii_llp_error_monitor, and the name suggests a link-level protocol monitor for the NI port.

- `err_int_unblock_mask` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1c228
  **Inferred.** Reads a 16-bit value from 0xffffffffffffc420 and a per-CPU record through pdaindr, then stores a 64-bit value to 0x9200000001000400. It also reads and writes a word at an address built from a per-CPU table entry and a byte in that record. The word at 0x9200000001000400 is also read by handle_io_buserr and handle_uncac_buserr, so it is probably an error status word, and the mask semantics and the branch on a1 are not established.

- `handle_io_buserr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d31c
  **Inferred.** Calls get_matching_spool_addr at 0x1d378, then kl_ioerror_handler up to three times (0x1d41c, 0x1d48c, 0x1d540) with a fixed type of 0x5, and logs through cmn_err_tag with codes 0xb6c and 0xb6d. It compares node IDs from the nasid_to_compact_node table and reads a field at a0+0x110. The return value is 0x1 or 0x2, and the meanings of those values are not established.

- `handle_uncac_buserr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d564
  **Inferred.** Calls get_matching_spool_addr at 0x1d598, then routes through many cmn_err_tag messages (codes 0xb6e through 0xb79) and partition_handle_error with small codes from 0x1 to 0xa. It checks partition IDs through nasid_to_partid and part_id, and calls error_check_memory_access, error_check_part_page, error_check_poisoned_state and error_page_discard. The return values are 0x0, 0x2, 0x3 or helper results. The routine looks like the uncached bus-error handler, and the meanings of the small codes are known only from the callees' names.

- `handle_cac_buserr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1da00
  **Inferred.** The largest routine in this batch. It calls get_matching_spool_addr at 0x1da38, log_md_errors at 0x1daf8 and kl_panic at 0x1dae4, then uses nasid_to_partid, part_id, nasid_to_region, error_lookup_inst_type, error_check_memory_access, error_check_part_page, error_page_discard, memory_iscalias, klerror_check_recoverable, verify_snchip_rev, dump_crb and get_dir_ent. It issues about eighteen cmn_err_tag messages (codes 0xb7a through 0xb8c). The shape matches the uncached handler with cache-line checks added, but the path through each branch is not traced here.

- `intpend0` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1fc28
  **Inferred.** Reads pending-interrupt words from fixed addresses (0xffffffffffffc598, 0xffffffffffffc508, 0xffffffffffffc5a8) and a global at 0x9200000001000098, then finds the most significant set bit through ms1bit at 0x1fca0. It takes a semaphore with cvsema at 0x1fc78, calls valusema and vsema, and makes indirect calls at 0x1fce4 and 0x1fd78 through a table entry at +0x10. The callee of those indirect calls is unresolved, and the loop logic around hub_intr_wakeup_cnt is only partly traced.

- `intpend1` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1fd88
  **Inferred.** Has the same shape as intpend0. It reads pending words (0xffffffffffffc5a0, 0xffffffffffffc5b0 and 0x92000000010000a0), finds the most significant bit with ms1bit at 0x1fdd4, and makes an indirect call at 0x1fe0c through a table entry. When no entry matches, it reports through cmn_err at 0x1fe64 with a level of 0 and the bit index. The routine serves a second interrupt group, though the facts do not show which group.

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x201dc
  **Inferred.** Reads a per-CPU record through pdaindr indexed by a0, then a 16-bit field at +0x424 and a byte at +0x520. It branches on a1 == 0xab, and stores a 64-bit value built from the 16-bit field at +0x424, shifted left 0x20, to a remote-register address at 0x20218. It returns 0. The meaning of 0xab and the exact path to the store are not fully traced.

- `cpuintr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20224
  **Inferred.** Reads a few fixed interrupt-state words, then copies 0x168 bytes with bcopy at 0x202c8. It calls doacvec and doactions in turn, and logs timestamp events 0x71 and 0x65 through log_tstamp_event at 0x2030c and 0x20328. Flag tests against a byte at [-0x3f80] and a word at [-0x3d80] gate parts of the path. The routine reads as the CPU-level interrupt entry, but the copy destination is not named.

- `dbgintr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20398
  **Inferred.** Reads a 16-bit value at 0xffffffffffffc420 and a per-CPU record, stores a word at 0x9200000001000090 and reads a word at 0x9200000001000098. It then calls debug at 0x203d0 with a format string, a value computed from a byte in the record, and the stored word, and returns the callee's result. The name suggests the debugger interrupt entry, and the call to debug supports that reading.

- `chkdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x20690
  **Inferred.** Reads a 16-bit value at 0xffffffffffffc420 and a per-CPU record, then tests a flag at 0x92000000010000a0 against a value in v0. When the test passes it calls dbgintr (the routine at 0x20398) with a0=0 and the caller's a1 to a3. The register v0 that gates the call is not resolved by the facts.
