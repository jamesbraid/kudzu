# Routine readings, tier-1 kudzu batch 40

These entries are role statements for 25 routines from the tier-1 reading round, batch 40. Each one is written from the disassembly (registers, calls and constants) and cites the addresses and callees it shows. Every role is inferred: the disassembly shows structure, not intent.

- `silence_all_audio` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1c58
  **Inferred.** The disassembly for this 8-byte routine shows only a return at 0x1c58 with v0 unknown, so no calls, device accesses or branches are visible. The name suggests it quiets audio, but nothing in the disassembly touches an audio device, so that reading stays unconfirmed. Treat it as a stub until a wider disassembly shows its callees.

- `cpu_node_probe` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x630c
  **Inferred.** This 768-byte probe calls get_nasid (0x63ac), do_cpumask (0x6418) and printf three times, with assfail at 0x65e8 as its failure path. It reads a per-node record through a hub-region address built from get_nasid's result (shown as 0x9600000000000000 | 0x2000 with offsets 0xd0, 0x184 and 388), and it walks arrays bounded by comparisons against 0x80, 0x200 and 0x3 while storing halfwords and words. A magic value 0x5045626b is compared at 0x644c, and a mismatch skips to 0x6584; the disassembly does not say what that constant identifies. The purpose, a per-node CPU census, is a guess from the callees and bounds.

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0xd7cc
  **Inferred.** This 1152-byte routine sequences early machine-level reset for the master node. It programs performance-counter registers through r1nkperf_control_register_set and r1nkperf_data_register_set (0xd7e0 to 0xd888), flushes TLB and cache, reads master_nasid, and calls per-device resets (ioc3_mlreset, ioc4_mlreset, l1_mlreset, nvram_baseinit) at 0xd8e0 to 0xd928. It then loops over arg_maxnodes and numnodes, calls cpu_node_probe (0xd988), gen_region_mask and setup_replication_mask, and runs initializers (spinlock_init, mem_init, hubdev_init, intrgroup_init) before its assfail checks at 0xdc04 to 0xdc3c. The call order suggests a boot-time initialization sequence, but the disassembly does not say what each initializer does.

- `getcpuid` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0xe718
  **Inferred.** This 116-byte routine returns a 16-bit value for the calling CPU. It raises the interrupt level with splhi (0xe724), asks get_nasid for the local node, looks up a per-CPU record with nasid_slice_to_cpuinfo (passing a slice value loaded from 0x9200000001000020), and restores the level with splx. A null record reaches an assfail at 0xe77c; otherwise the routine returns the halfword at offset 0x1e of that record. The name fits a CPU-id lookup, but the disassembly shows only the field read, not what it means.

- `page_local_calias` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10124
  **Inferred.** This 60-byte routine loads a doubleword from 0x9200000001000028, a fixed address in the local-state region. A zero value branches to 0x10154; otherwise an unsigned compare at 0x1014c decides between the two results returned at 0x10158 (0 or 1). The disassembly does not show what the word or the compared values represent, so the page-alias purpose cannot be checked from this disassembly.

- `save_cache_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x12888
  **Inferred.** This 416-byte routine saves cache-error state into a record. It reads a table entry at 0x9000000034000000, a hub-region register at offset 0x48, and the local-state word at 0x9200000001000020, then stores several doubleword, word and byte fields (sd at 0x129f4, 0x129f8, 0x12a08; sw at 0x129fc; sb at 0x12a04). It calls get_nasid and chk_for_pebrick (at 0x12a18), and it branches to that call when a field equals 0xc000 at 0x128d4. The byte and word loops are bounded by a count that reaches 0x10, but the disassembly does not name the fields.

- `save_hub_pi_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1362c
  **Inferred.** This 216-byte routine copies hub PI error registers from its first argument into a record at its second. It zeroes the 0x60-byte destination with bzero, then copies source offsets 0x400, 0x50, 0x58, 0x430, 0x440, 0x450, 0x460, 0x4c8 and 0x4d0 into record offsets 0x10 through 0x50. Three of those copies happen only when a bit tested against 0x8000000000000000 is set, and the routine returns the doubleword at source offset 0x58. Field names are not in the disassembly, so the source-to-record mapping is inferred from the access pattern.

- `save_hub_md_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13704
  **Inferred.** This 236-byte routine does the same kind of copy for hub memory-directory error registers. It zeroes a 0x58-byte destination with bzero and copies five source doublewords at offsets 0x7c0010, 0x7e0010, 0x7c0020, 0x780040 and 0x780048 into record offsets 0x8 through 0x40. A loop tests bits of the 0x7c0010 and 0x7c0020 values with shifts and sign checks to decide whether to repeat. The record fields are not named in the disassembly, so each mapping is inferred.

- `save_hub_io_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13868
  **Inferred.** This 164-byte routine copies hub I/O error registers into a record and then hands off to two helpers. It zeroes a 0xd0-byte destination with bzero, copies source doublewords at offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008 and 0x420010, then calls save_hub_io_crb (with the 0x410000 and 0x400008 values as arguments) and save_hub_io_ieph. Its return value is whatever save_hub_io_ieph returns. The grouping of the copies follows the offsets in the access pattern, not any named register list.

- `save_hub_err_ext` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1390c
  **Inferred.** This 184-byte routine zeroes a 0xc0-byte destination with bzero, then copies 8-byte values in a loop indexed by a2. The addresses are computed from a2, not fixed constants, so the disassembly does not name the registers involved. The loop runs while a2 stays below 0xf, and a second bound at 0x9 selects a different path. The name suggests extended hub error fields, but that stays unconfirmed.

- `save_hub_ni_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x139c4
  **Inferred.** This 100-byte routine saves NI error registers. It zeroes a 0x30-byte destination with bzero and copies four doublewords from source offsets 0x680100 through 0x680118 into record offsets 0x8 through 0x20. It skips the copies when the doubleword at 0x680000 is zero. Those source offsets are the only register identifiers in the disassembly; the rest of the record layout is inferred from the store pattern.

- `hub_error_init` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x17950
  **Inferred.** This 1464-byte routine initializes hub error handling for each node. It iterates numnodes, maps each compact node id to a nasid through compact_to_nasid_node, and writes a set of hub registers at addresses built from that nasid. Those addresses are one of two node-region bases (0x9200000001800... or 0x9200000001a00...), and the input that picks between them is not in the disassembly. It also calls hub_error_clear, hubii_eint_init, hubni_eint_init, and recover_error_init four times, allocates buffers with kmem_zalloc_node (sizes 0x2258 and 0x40), and connects hubpi_eint_handler with cause_intr_connect at 0x17d8c. A failure path reaches assfail and cmn_err at 0x17ef8.

- `hubpi_eint_handler` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x181d0
  **Inferred.** This 1624-byte routine is the hub PI error interrupt handler. It first calls check_ni_errors (0x181d8), then reads the PI error words (pi_fatal_err_cpu_a, pi_fatal_err_cpu_b and pi_err_int_pend) and the hub register at 0x9200000001000400. It tests bit masks in several groups and branches to many cmn_err reports at severities 0x12, 0x14 and 0x15; on some paths it calls kl_panic_setup, log_md_errors, md_int_block and process_error_spool. The control structure is visible in the disassembly, but which fatal category each mask stands for is not settled.

- `check_ni_errors_nasid` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18a24
  **Inferred.** This 764-byte routine checks the NI error state of one nasid. It reads NI registers at addresses built from the nasid argument, calls part_id, and calls ni_port_down_reset at 0x18a80, 0x18b00 and 0x18cc8 under conditions the disassembly only partly shows. It reports through many cmn_err calls, sends protected_broadcast messages 0x67 and 0x68 at 0x18c94 and 0x18c9c, and returns 0 or 1. The NI register offsets appear only as address arithmetic, so their names remain open.

- `check_ni_errors` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18ec0
  **Inferred.** This 36-byte routine is a thin wrapper. It calls get_nasid at 0x18ec8 with its own arguments, then calls check_ni_errors_nasid (0x18a24) with the resolved nasid and returns that callee's result. The disassembly shows no other work, so the nasid lookup is the whole job.

- `hubni_error_handler` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18ee4
  **Inferred.** This 56-byte routine is another wrapper. It calls get_nasid at 0x18ef4, then calls hubni_error_handler_nasid at 0x18d20 with the resolved nasid and passes its own a0 and a1 through. It returns that callee's result. The disassembly does not show what the nasid-level handler does.

- `klsave_spool_registers` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1994c
  **Inferred.** This 468-byte routine saves register state into an error spool. It reads the local-state word at 0x9200000001000020, calls get_nasid several times, and uses find_lboard (type 0x11) and find_first_component (type 0x2) to locate a board record. It reads hub-region registers and writes six doubleword and word fields, with a chk_for_pebrick call on one path. The record layout and the meaning of the type codes are not established by the disassembly.

- `ii_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1b19c
  **Inferred.** This 104-byte routine reads and writes a link-status word and a small record. It loads from 0x9200000001400130 and stores back to the same address, and the disassembly does not show a changed value. It then copies two doublewords and two bytes at offsets 0x0 to 0x11 of its argument, with the byte updates guarded by comparisons to 0xffff. The link-monitor purpose is inferred from the shape of the accesses.

- `ni_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1b204
  **Inferred.** This 300-byte routine is the NI counterpart of ii_llp_error_monitor. It reads the register at 0x9200000001680120, copies three doublewords and three bytes of its argument at offsets 0x0 to 0x1a, and tests bit fields of the register and of several byte values with masks of 0xf. A cmn_err call at 0x1b320 (severity 0x4) is skipped by the early branches. The meaning of the bit tests is not given in the disassembly.

- `err_int_unblock_mask` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1b980
  **Inferred.** This 76-byte routine looks up the per-CPU block for the caller through pdaindr, reads a flag byte at offset 0x528 of it, and reads the halfword at 0xffffffffffffc428. It stores a doubleword to 0x9200000001000400 and then reads and writes one of two words, 0x9200000001000408 or 0x9200000001000410, with the choice not shown in the disassembly. A zero argument skips one of the stores. The name points at an interrupt mask being unblocked, but the disassembly does not say whether bits are set or cleared.

- `stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1bcf0
  **Inferred.** This 572-byte routine loops over CPUs and hub ports while accessing several hub-region addresses. It reads the local-state word at 0x9200000001000020 and tests its low bit, reads 0x9200000001600000, 0x9200000001604020, 0x9200000001604028 and 0x9200000001780000, and writes 0x9200000001000410, 0x9200000001fa0038, 0x9200000001800000 and 0x9200000001000408. Loop counters are compared against 0x4, 0x2 and 0x1 under a mask of 0x3. The name points to halting all CPUs, but the disassembly shows no call that would do that, so the purpose stays open.

- `handle_io_buserr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1cd5c
  **Inferred.** This 592-byte routine handles I/O bus errors. It calls get_matching_spool_addr, then kl_ioerror_handler up to three times (0x1ce58, 0x1cecc, 0x1cf88) with the nasid-to-compact map entry and an error-class argument. It reports through cmn_err_tag with codes 0xb6c and 0xb6d (0x1cf2c and 0x1cf48). It reads the halfwords at 0xffffffffffffc428 and 0xffffffffffffc42a, the error word at 0x9200000001000400, and a per-node structure at offset 0x110, then tests bits 0x100000 and 0x200000 of that word. The branch structure is visible, but the disassembly does not say what each error class means.

- `handle_uncac_buserr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1cfac
  **Inferred.** This 2308-byte routine handles uncached bus errors. It calls get_matching_spool_addr, then nasid_to_partid and part_id to compare partitions, and reports through cmn_err_tag codes 0xb6e, 0xb6f, 0xb70 to 0xb73 and 0xb79. It calls partition_handle_error twice with class 0x9, and calls error_page_discard (0x1d528) and error_check_part_page (0x1d55c) on the page path. Its indirect jump at 0x1d048 goes through a jump table whose targets the disassembly could not resolve, so part of the routine is unread.

- `handle_cac_buserr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1d8b0
  **Inferred.** This 2420-byte routine handles cached bus errors, with a larger set of checks than the uncached handler. It calls get_matching_spool_addr, nasid_to_region, nasid_to_partid and part_id, and can reach kl_panic at 0x1d964. It also calls error_lookup_inst_type, error_check_memory_access, error_check_part_page, error_check_poisoned_state, memory_iscalias and klerror_check_recoverable, with partition_handle_error at classes 0x2, 0x5, 0x6 and 0x9 and cmn_err_tag codes 0xb7a through 0xb82. Its indirect jump at 0x1d95c uses a jump table whose targets the disassembly could not resolve, so the decision tree is only partly visible.

- `intpend0` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x200f0
  **Inferred.** This 352-byte routine dispatches pending hub interrupt bits. It reads a pending word at 0x9200000001000098 masked against the value at -0x3a60, uses ms1bit to find the most significant set bit, and indexes a per-slot table with the result. It takes and releases a semaphore on that slot with cvsema, valusema and vsema, then calls through a function pointer at slot offset 0x10 (shown as an unknown callee at 0x201ac and 0x20240). The name fits interrupt dispatch and the semaphore use is visible, but the table contents are not in the disassembly.
