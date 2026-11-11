# Tier-1 routine readings, batch 32 (kudzu)

Role statements for the 25 routines at target indexes 800 to 824 of the tier-1 hardware-linked list (`t1list`, `--hardware-only`). Each statement comes only from the instruction facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the binary's symbol table. Grades are inferred throughout. Nothing here is hardware-verified, and where a register, device window or callee is unresolved the statement says so.

The binary is `kernel.o` under `6.5.30/usr/cpu/sysgen/IP27boot/`.

The cited addresses and callees were checked by hand against the instruction facts.

- `is_fine_dirmode` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x4510
  **Inferred.** A 20-byte leaf that loads one doubleword from the fixed address 0x9200000001600000 at 0x4514 and returns a value at 0x451c whose source the facts do not resolve. The name suggests a directory-mode check, but no comparison or call in the instruction facts confirms it.

- `ni_reset_port` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x45e0
  **Inferred.** A 16-byte leaf that stores one doubleword at 0x45ec to the fixed address 0x9200000001600008. Its return at 0x45e8 leaves that same address in v0. The stored value is not shown, so the reset effect stays open.

- `cpu_node_probe` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x6178
  **Inferred.** A 556-byte probe that calls get_nasid at 0x6244 to identify the node and reads a doubleword at offset 0xd0 of a per-node table entry at 0x6260. Its loops compare a register against 0x40, 0x80 and 0x100 (branches at 0x61b8, 0x61ec and 0x6224). It calls do_cpumask at 0x62a8 and assfail at 0x6380 with line 0xb9 on a failed check, and the facts do not say what is being counted.

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0xcf60
  **Inferred.** A 1056-byte sequence for the boot node's machine-level reset. It programs performance-counter groups through r1nkperf_control_register_set and r1nkperf_data_register_set at 0xcf74 to 0xd06c, and it picks one of four branch_prediction_* calls (0xd07c to 0xd0ac) by comparing the global r10k_bpmode against 0 to 3. It flushes the TLB and cache at 0xd01c and 0xd024, then calls cpu_node_probe at 0xd154, getcpuid at 0xd190 and a run of platform init calls from 0xd24c to 0xd310. Which of these steps apply to every board revision is not shown.

- `getcpuid` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0xdff8
  **Inferred.** A 116-byte lookup that raises the interrupt level with splhi at 0xe004 and finds the node with get_nasid at 0xe00c. It passes the doubleword at 0x9200000001000020, loaded at 0xe018, to nasid_slice_to_cpuinfo at 0xe020, and it returns the 16-bit field at offset 0x1e of the record that call gives back. It restores the level with splx at 0xe02c, and a null record goes to assfail at 0xe05c with line 0x535.

- `restart` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x10694
  **Inferred.** A 356-byte reboot path that loops over nodes up to maxnodes. For each node where power_off_flag is clear, it calls ni_reset_port at 0x10784. It then either calls sysctlr_power_off at 0x1074c or resets the system controller through elsc_init at 0x107a8 and elsc_system_reset at 0x107b0. The delays of 1,000,000 and 3,000,000 (0xf4240 and 0x2dc6c0) go through us_delay, which the name suggests is microseconds, and panic_level at 0x106ac and reboot_on_panic at 0x10734 choose the branches; the facts do not show the order when both are set.

- `page_local_calias` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x11f88
  **Inferred.** A 60-byte predicate that loads the doubleword at the fixed address 0x9200000001000028 (0x11f8c) and branches on whether it is zero (0x11f9c). On one path it compares two register values as unsigned at 0x11fb0 and returns 0 or 1. The facts do not show what those two values are, and the name is the only hint that this is a local cache alias check.

- `bte_init` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x12ddc
  **Inferred.** A 232-byte setup routine for a block-transfer engine, going by its name. It stores doublewords at offsets 0x0, 0x18, 0x20, 0x28 and 0x30 of its first argument, a halfword at 0x40 and words at 0x54 and 0x70. It calls spinlock_init at 0x12e18 on the field at offset 0x44, writes the fixed address 0x92000000014001f8 twice (0x12e94 and 0x12eb8), and reads a 16-bit entry from compact_to_nasid_node. It returns the structure pointer in v0, and the facts do not say what any field means.

- `save_cache_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x15f60
  **Inferred.** A 360-byte copy routine that calls get_nasid at 0x15f70 and reads a doubleword at offset 0x48 of the node's table entry at 0x15f98. It reads words and bytes from addresses the facts do not resolve, in loops whose counter is compared against 0x10 (branch at 0x15fec). It also reads hub registers at 0x9200000001000020 (0x1606c) and 0x9200000001030100 (0x160a8), and stores into records whose base is unresolved, so the saved fields are not named.

- `save_hub_pi_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1690c
  **Inferred.** A 256-byte routine that copies processor-interface error registers from its first argument into the record at its second. It moves doublewords from offsets 0x400, 0x50, 0x58, 0x430, 0x440, 0x450 and 0x460 into record offsets 0x10 through 0x40. Several moves are gated by mask tests with 0x8000000000000000 and 0x2, and by comparisons against 0x480041 and 0x1fffffe0; the facts do not name the bits or say what those values select.

- `save_hub_md_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16a0c
  **Inferred.** A 108-byte copy routine that moves four doublewords from offsets 0x200050, 0x200060, 0x200070 and 0x200080 of its first argument into the record at its second, at record offsets 0x8, 0x28, 0x18 and 0x38. Some moves are guarded by zero tests on loaded values, and one test checks the sign of a shifted value at 0x16a68. The name points to the memory directory, but the instruction facts show only the copies and tests.

- `save_hub_io_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16a78
  **Inferred.** A 216-byte routine that zeroes a 0xc0-byte record with bzero at 0x16a94, then copies seven doublewords into it from offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008 and 0x420010 of its first argument. Two more loads and stores at unresolved addresses sit in a loop guarded by a comparison against 0xf at 0x16b38. The facts do not say which registers those offsets hold.

- `save_hub_err_ext` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16b50
  **Inferred.** A 184-byte routine that zeroes a 0xc0-byte record with bzero at 0x16b6c and then moves three doublewords between addresses the facts do not resolve (loads at 0x16b9c, 0x16bc4 and 0x16bec). Its branches compare a register against 0xf and 0x9, which suggests a count or class check. The name suggests extended error state, but the facts do not confirm which registers are saved.

- `save_hub_ni_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16c08
  **Inferred.** An 88-byte routine that zeroes a 0x20-byte record with bzero at 0x16c20. It copies the doubleword at offset 0x600300 of its first argument to record offset 0x8, and the doubleword at 0x608008 to record offset 0x10, and it returns the record pointer in v0 at 0x16c58. The facts give only the two source offsets, so which network-interface registers they hold is open.

- `hub_error_init` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19374
  **Inferred.** A 928-byte initializer that sets up a per-node error log. It indexes kl_error_log by node number, reads hub registers at 0x9200000001000040, 0x9200000001000048, 0x9200000001000050 and 0x9200000001000058, and at 0x9200000001000420 and 0x9200000001000428, and it writes the log entry at offsets 0x1910, 0x1920, 0x1928 and 0x1930. It stores to 0x9200000001000408, 0x9200000001000410, 0x9200000001000420, 0x9200000001000448 and 0x9200000001000468, and it connects hubpi_eint_handler through cause_intr_connect at 0x19560 with first argument 0x6. It also allocates two zeroed blocks with kmem_zalloc_node at 0x19600 and 0x1961c, and the facts do not show which bits it clears and which it only reads.

- `hubpi_eint_handler` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x199a8
  **Inferred.** A 728-byte top-level interrupt handler for the processor-interface error source, going by its name. It calls check_ni_errors at 0x199b0 and hubni_error_handler at 0x199d0 first. Later it calls get_nasid at 0x19a2c, log_md_errors at 0x19a34 and process_error_spool at three sites (0x19a6c, 0x19acc and 0x19b2c), and it reaches enter_panic_mode at 0x19bc0 or kl_panic at 0x19bdc on some paths. It tests a status word against masks such as 0xcfff00, 0x1 and 0x2, 0x40 and 0x80, and 0x100000 and 0x200000, and the facts do not say which error each mask stands for.

- `check_ni_errors` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19dc0
  **Inferred.** A 604-byte checker for network-interface error registers. It reads the hub addresses 0x9200000001608008, 0x9200000001600000, 0x9200000001600010 and 0x9200000001608088, and also 0x92000000010000a0. It reports set bits through a series of cmn_err calls with severity 0x14 or 0x15, and it sends protected_broadcast messages with codes 0x67 and 0x68 at 0x19f2c and 0x19f34. Bit tests use masks such as 0x3eff000000, 0x30000000 and 0x2000000000, and the facts do not name the bits.

- `hubni_error_handler` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1a01c
  **Inferred.** A 392-byte handler for a network-interface error. It reads 0x9200000001608008 and 0x9200000001600010, branches on bit 0x2000000000 of the first, and logs through splerr at 0x1a060, with the value at 0x9200000001600010 passed in a4. Depending on path, it sets a node panic flag through set_node_panic_flag at 0x1a074, prints state with hubni_print_state and router_print_state, and calls enter_panic_mode at 0x1a098 or panicspin at 0x1a12c. The conditions that choose each path are only partly resolved.

- `klsave_spool_registers` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1aac4
  **Inferred.** A 360-byte routine that saves spool-related registers for a node. It calls get_nasid at 0x1aae8 and four more sites, then find_lboard with class 0x11 at 0x1ab8c and find_first_component with 0x2 at 0x1aba0 to locate a board record. It reads the hub register at 0x9200000001000020 (0x1aad8) and a 32-bit field from the component found, and it writes three doublewords to a target the facts do not resolve. The name says spool, but the facts do not show what the spool holds.

- `hub_error_status_war` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1beb4
  **Inferred.** A 96-byte workaround routine, as its name says. It reads the hub registers at 0x9200000001000020, 0x9200000001000450 and 0x9200000001000430, and tests bit 0x8000000000000000 of the last two (branches at 0x1bed8 and 0x1bef4). It probes the bus address 0x9200000002400208 with _badaddr_val at 0x1bf04 and returns either that probe result or a value the facts do not resolve. The facts do not say what triggers the workaround.

- `ii_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bf14
  **Inferred.** A 104-byte monitor for a link-protocol error record. It loads and stores the doubleword at the fixed address 0x9200000001400130 (0x1bf18 and 0x1bf28), so the register is touched, but the facts do not show the value written back. It moves doublewords at record offsets 0x0 and 0x8 and bytes at 0x10 and 0x11, each loaded and stored through the same address, in a sequence guarded by comparisons against 0xffff at 0x1bf2c and 0x1bf40.

- `ni_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bf7c
  **Inferred.** A 196-byte monitor for the network-interface link. It reads the hub addresses 0x9200000001608008 (0x1bf84) and 0x9200000001608088 (0x1bfa0), and it moves doublewords and bytes at record offsets 0x0 to 0x1a. The cmn_err call at 0x1c030, with severity 4, sits on the branch taken when the word at 0x9200000001608008 has bits in 0x3e00000000 set (branch at 0x1bf98). A loop guarded by 0xff at 0x1bfb4 handles the byte fields.

- `err_int_unblock_mask` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1c4c8
  **Inferred.** A 76-byte routine, going by its name, that updates an error-interrupt mask. It reads a 16-bit value at 0xffffffffffffc420, looks up a record through the pdaindr table, and writes the fixed address 0x9200000001000400 at 0x1c4ec. It also does a read-modify-write of a field reached from that record (0x1c504 and 0x1c510), and it branches on whether its argument is zero at 0x1c4e0. The mask bits it clears or sets are not named in the facts.

- `handle_io_buserr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d5cc
  **Inferred.** A 588-byte handler for I/O bus errors, going by its name. It calls get_matching_spool_addr at 0x1d628, then kl_ioerror_handler at 0x1d6c8, 0x1d73c and 0x1d7f4, passing 5 as its fourth argument. It logs through cmn_err_tag with message ids 0xb6c and 0xb6d at 0x1d798 and 0x1d7b4, and it reads the hub register at 0x9200000001000400 and writes it back at 0x1d7a8. The branches that choose among the three handler calls are only partly resolved.

- `handle_uncac_buserr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d818
  **Inferred.** A 1208-byte handler for uncached bus errors. It calls get_matching_spool_addr at 0x1d84c and maps the node to a partition through nasid_to_partid and part_id. It passes the error to partition_handle_error with codes 0xa, 0x9, 0x1, 0x6 and 0x5 (for example at 0x1d924 and 0x1d9d0), and it calls error_check_memory_access, error_check_part_page, error_check_poisoned_state and error_page_discard along the way. Its cmn_err_tag ids run from 0xb6e to 0xb79, and the branch rules that choose between these paths are not fully resolved.
