# Tier-1 routine readings, batch 32 (kudzu)

Role statements for the 25 routines at target indexes 800 to 824 of the tier-1 hardware-linked list (`t1list`, `--hardware-only`). Each statement comes only from the instruction facts: registers, offsets, branch conditions and callees at the cited addresses. Names come from the binary's symbol table. Grades are inferred throughout. Nothing here is hardware-verified, and where a register, device window or callee is unresolved the statement says so.

The binary is `kernel.o` under `6.5.30/usr/cpu/sysgen/IP27boot/`.

The cited addresses and callees were checked by hand against the instruction facts.

- `is_fine_dirmode` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x4510
  **Inferred.** A 20-byte leaf that loads one doubleword at 0x4514 (the address is not shown in the facts) and returns a value at 0x451c whose source the facts do not resolve. The name suggests a directory-mode check, but no comparison or call in the instruction facts confirms it.
  Reviewed: 2026-11-12 (sonnet)

- `ni_reset_port` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x45e0
  **Inferred.** A 16-byte leaf whose return at 0x45e8 leaves 0x9200000001600008 in v0 and which stores one doubleword at 0x45ec. The facts do not show the base register of that store, so whether it targets the same address is not established, and the stored value is not shown. The reset effect stays open and comes from the name only.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_node_probe` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x6178
  **Inferred.** A 556-byte routine that calls get_nasid at 0x6244 and loads a doubleword at offset 0xd0 at 0x6260 (base register not shown). Its loops compare a register against 0x40, 0x100 and 0x80 (branches at 0x61b8, 0x61ec and 0x6224). It calls do_cpumask at 0x62a8 and assfail at 0x6380 with a2=0xb9. The facts do not say what the routine counts or probes; the name is the only hint.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0xcf60
  **Inferred.** A 1056-byte routine; the name suggests a machine-level reset, which the facts do not confirm. It calls r1nkperf_control_register_set and r1nkperf_data_register_set at 0xcf74 to 0xcf98, calls get_cpu_irr at 0xcfa0 and 0xcfb8, and branches at 0xcfb0 and 0xcfc8 to 0xd048, where the same two setters are called again through 0xd06c. A branch at 0xcfd0 sends the case s0 (the first argument, at entry) equal to 0 to 0xd0bc, where the calls to get_nasid, set_master_bridge_base, ioc3_mlreset, nvram_baseinit, is_fine_dirmode, cpu_node_probe at 0xd154, getcpuid at 0xd190 and later calls (0xd24c to 0xd310) are found. When s0 is nonzero it compares the global r10k_bpmode against 0, 1, 2 and 3 (0xcfd8 to 0xcff4), and the matching branches lead to branch_prediction_on, branch_prediction_fwd_not_taken, branch_prediction_off and branch_prediction_taken (0xd07c to 0xd0ac). A branch at 0xcffc tests s0 again and goes to the printf at 0xd320 when it is 0; a get_cpu_irr call at 0xd004 follows. The flush_tlb (0xd01c) and flush_cache (0xd024) calls are skipped when a branch at 0xd014 finds a value equal to 9, and the return at 0xd030 gives flush_cache's result. The facts do not show where the s0 = 0 path or the branch-prediction paths continue, or which steps apply to every board revision.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0xdff8
  **Inferred.** A 116-byte lookup that raises the interrupt level with splhi at 0xe004 and finds the node with get_nasid at 0xe00c. It passes the doubleword at 0x9200000001000020, loaded at 0xe018, to nasid_slice_to_cpuinfo at 0xe020, and it returns the 16-bit field at offset 0x1e of the record that call gives back. It restores the level with splx at 0xe02c, and a null record goes to assfail at 0xe05c with line 0x535.
  Reviewed: 2026-11-12 (sonnet)

- `restart` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x10694
  **Inferred.** A 356-byte routine, a reboot path by name, with a loop over nodes bounded by maxnodes (branches at 0x106b4 and 0x106e4, stores at 0x106d4 and 0x106fc). A branch at 0x106ac tests panic_level against zero. It calls us_delay with 1,000,000 (0xf4240) at 0x10720, and the facts name no unit; the name suggests microseconds. A branch at 0x10734 compares reboot_on_panic with 2 and, when equal, goes to 0x1079c, where it calls get_nasid, elsc_init at 0x107a8 and elsc_system_reset at 0x107b0 and branches on a negative result at 0x107b8 to a printf at 0x107cc. Otherwise it tests power_off_flag at 0x10744: when it is zero it calls ni_reset_port at 0x10784, and when it is nonzero it calls sysctlr_power_off at 0x1074c and us_delay with 3,000,000 (0x2dc6c0) at 0x10754. The facts do not show whether ni_reset_port runs once per node, or what happens when both panic_level and reboot_on_panic are set.
  Reviewed: 2026-11-12 (sonnet)

- `page_local_calias` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x11f88
  **Inferred.** A 60-byte predicate that loads the doubleword at the fixed address 0x9200000001000028 (0x11f8c) and branches on whether it is zero (0x11f9c). On one path it compares two register values as unsigned at 0x11fb0 and returns 0 or 1. The facts do not show what those two values are, and the name is the only hint that this is a local cache alias check.
  Reviewed: 2026-11-12 (sonnet)

- `bte_init` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x12ddc
  **Inferred.** A 232-byte setup routine for a block-transfer engine, going by its name. It tests the global scache_linemask against a1 at 0x12e04 and calls spinlock_init at 0x12e18 with a0 = a0+0x44. It stores doublewords at +0x28, +0x30, +0x18 and +0x20 and a halfword at +0x40, loads halfwords at 0x12e30 and 0x12e5c, and stores doublewords at +0x0 (0x12e88, 0x12e90, 0x12e94 and 0x12eb8) and words at +0x54 and +0x70. The facts do not show the base registers of these stores, so which are fields of the first argument is not established. It returns a0 in v0 at 0x12eac. The facts do not say what any field means.
  Reviewed: 2026-11-12 (sonnet)

- `save_cache_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x15f60
  **Inferred.** A 360-byte routine, named as a cache-error save, that calls get_nasid at 0x15f70 and loads a doubleword at offset 0x48 at 0x15f98 (base not shown). It loads words and bytes from addresses the facts do not resolve, in loops where a value is compared with 0x10 at 0x15fec. A doubleword load at 0x1606c and the branch at 0x16074 compare a loaded byte with the doubleword at 0x9200000001000020. It makes another doubleword load at 0x160a8 whose address is not shown, and stores into records whose base is unresolved, so the saved fields are not named.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_pi_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1690c
  **Inferred.** A 256-byte routine, named as a processor-interface error save (a name-derived role). It loads doublewords at offsets 0x400, 0x50, 0x58, 0x430, 0x440, 0x450 and 0x460 of its first argument and stores doublewords at offsets 0x10, 0x28, 0x38, 0x30 and 0x40 (base register not shown; 0x10 is stored at three sites). Several moves are gated by mask tests with 0x8000000000000000 and 0x2, and by comparisons against 0x480041 and 0x1fffffe0; the facts do not name the bits or say what those values select.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_md_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16a0c
  **Inferred.** A 108-byte routine, named as a memory-directory error save. It loads doublewords at offsets 0x200050, 0x200070, 0x200060 and 0x200080 of its first argument and stores doublewords at offsets 0x8, 0x18, 0x28, 0x38 and a second 0x8 (0x16a74), with each store following a load. The facts show neither the base register of the stores nor the stored values, so a copy into the second argument is only suggested. Some stores are guarded by zero tests on loaded values, and one test checks the sign of a shifted value at 0x16a68. The facts show only the loads, tests and stores.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_io_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16a78
  **Inferred.** A 216-byte routine, named as an I/O error save, that zeroes a 0xc0-byte record with bzero at 0x16a94 (a0 = its second argument). It loads doublewords at offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008 and 0x420010 of its first argument (the base is shown only for the return value, [a0+0x420008]) and stores doublewords at record offsets 0x8, 0x88, 0x98, 0xa8, 0x90, 0xa0 and 0xb0, each after a load. The stored values are not shown. Two more loads and stores at unresolved addresses are followed by a comparison against 0xf at 0x16b38, and the facts do not say which registers those offsets hold.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_err_ext` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16b50
  **Inferred.** A 184-byte routine that zeroes a 0xc0-byte record with bzero at 0x16b6c and then moves three doublewords between addresses the facts do not resolve (loads at 0x16b9c, 0x16bc4 and 0x16bec). Its branches compare a register against 0xf and 0x9, which suggests a count or class check. The name suggests extended error state, but the facts do not confirm which registers are saved.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_ni_error` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x16c08
  **Inferred.** An 88-byte routine, named as a network-interface error save, that zeroes a 0x20-byte record with bzero at 0x16c20 (a0 = its second argument). It loads doublewords at offsets 0x600300 and 0x608008 (base register not shown), stores doublewords at offsets 0x8 and 0x10 after them, and returns the second argument in v0 at 0x16c58. The stored values and the store base are not shown, so which registers the offsets hold, and whether the stores copy those loads, is open.
  Reviewed: 2026-11-12 (sonnet)

- `hub_error_init` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19374
  **Inferred.** A 928-byte routine, named as a hub error initializer, that indexes kl_error_log by a value derived from its first argument and calls hub_error_clear at 0x19394, recover_error_init twice (0x193ec and 0x193f8), hubii_eint_init and hubni_eint_init. It loads hub registers at 0x9200000001000058, 0x9200000001000050, 0x9200000001000040, 0x9200000001000048 and 0x9200000001000428, and stores log-entry doublewords at offsets 0x1910, 0x1918, 0x1920, 0x1928 and 0x1930. It calls cause_intr_connect at 0x19560 with a0=0x6, a1=hubpi_eint_handler and a4=0x9200000001000420, and kmem_zalloc_node at 0x19600 and 0x1961c. Other stores have bases the facts do not show. The facts do not show which bits it clears and which it only reads.
  Reviewed: 2026-11-12 (sonnet)

- `hubpi_eint_handler` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x199a8
  **Inferred.** A 728-byte interrupt handler for the processor-interface error source, going by its name. It calls check_ni_errors at 0x199b0 and, only when that returns nonzero (branch at 0x199c0), hubni_error_handler at 0x199d0. It calls get_nasid at 0x19a2c and log_md_errors at 0x19a34 when a value tested at 0x19a24 is negative. It calls process_error_spool at three sites (0x19a6c, 0x19acc and 0x19b2c), and reaches enter_panic_mode at 0x19bc0 or kl_panic at 0x19bdc on some paths. It tests a status word against masks such as 0xcfff00, 0x1 and 0x2, 0x40 and 0x80, 0x10 and 0x20, and 0x100000 and 0x200000, and the facts do not say which error each mask stands for.
  Reviewed: 2026-11-12 (sonnet)

- `check_ni_errors` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x19dc0
  **Inferred.** A 604-byte checker for network-interface error registers, going by its name. It reads 0x9200000001608008, 0x9200000001600000 and 0x9200000001600010, and also 0x92000000010000a0. It makes a series of cmn_err calls with a0 equal to 0x14 or 0x15, and calls protected_broadcast with a0=0x67 at 0x19f2c and a0=0x68 at 0x19f34. Bit tests use masks such as 0x3eff000000, 0x30000000 and 0x2000000000, and the facts do not name the bits.
  Reviewed: 2026-11-12 (sonnet)

- `hubni_error_handler` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1a01c
  **Inferred.** A 392-byte routine, a network-interface error handler by name. It reads 0x9200000001608008 and 0x9200000001600010 and branches on bit 0x2000000000 of the first (0x1a044) and on whether the second is zero (0x1a058). It calls splerr at 0x1a060 with the address 0x9200000001600010 as a4 (the facts show the address, not the value read from it). Depending on path it calls set_node_panic_flag at 0x1a074, hubni_print_state and router_print_state, enter_panic_mode at 0x1a098 and panicspin at 0x1a12c; the facts do not separate the paths those last two sit on. The conditions that choose each path are only partly resolved.
  Reviewed: 2026-11-12 (sonnet)

- `klsave_spool_registers` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1aac4
  **Inferred.** A 360-byte routine, named as a spool-register save. It calls get_nasid at 0x1aae8 and four more sites (0x1aaf8, 0x1ab3c, 0x1ab44 and 0x1ab4c), then find_lboard with a1=0x11 at 0x1ab8c and find_first_component with a1=0x2 at 0x1aba0, each followed by a zero test on the result. It loads a doubleword at 0x1aad8 (address not shown) and a word at offset 0x30 at 0x1abc4, and stores three doublewords at 0x1abf0, 0x1abf8 and 0x1ac00 to a target the facts do not resolve. The facts do not show what the spool holds.
  Reviewed: 2026-11-12 (sonnet)

- `hub_error_status_war` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1beb4
  **Inferred.** A 96-byte routine; the name suggests a workaround for an error-status register. It reads the hub registers at 0x9200000001000020, 0x9200000001000450 and 0x9200000001000430, and tests bit 0x8000000000000000 of the last two (branches at 0x1bed8 and 0x1bef4). It probes the bus address 0x9200000002400208 with _badaddr_val at 0x1bf04 and returns either that probe result or a value the facts do not resolve. The facts do not say what triggers the workaround.
  Reviewed: 2026-11-12 (sonnet)

- `ii_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bf14
  **Inferred.** A 104-byte routine, a link-protocol error monitor by name. It makes a doubleword load at 0x1bf18 and a doubleword store at 0x1bf28, with no address or stored value shown. It then loads and stores doublewords at offsets 0x0 and 0x8 and bytes at offsets 0x10 and 0x11 (base register not shown), guarded by comparisons against 0xffff at 0x1bf2c and 0x1bf40.
  Reviewed: 2026-11-12 (sonnet)

- `ni_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1bf7c
  **Inferred.** A 196-byte routine, a network-interface link monitor by name. It reads 0x9200000001608008 at 0x1bf84 and makes another doubleword load at 0x1bfa0 (address not shown; 0x9200000001608088 is passed to cmn_err in a5). It moves doublewords at offsets 0x0, 0x8 and 0x10 and bytes at 0x18, 0x19 and 0x1a (base register not shown). The cmn_err call at 0x1c030, with a0=0x4, follows the branch taken when the word at 0x9200000001608008 has bits in 0x3e00000000 set (branch at 0x1bf98). Comparisons against 0xff at 0x1bfb4, 0x1bfc8 and 0x1bfdc guard the byte and doubleword moves; the facts show no loop.
  Reviewed: 2026-11-12 (sonnet)

- `err_int_unblock_mask` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1c4c8
  **Inferred.** A 76-byte routine, an error-interrupt mask update by name. It loads a halfword at 0x1c4c8, a doubleword at offset 0x8 at 0x1c4d8 and a byte at offset 0x520 at 0x1c4dc (no addresses or bases shown). It branches on whether a1 is zero at 0x1c4e0, stores a doubleword at 0x1c4ec, and loads at 0x1c504 and stores at 0x1c510 using the same offset. The facts do not show the addresses, the mask bits or what is written.
  Reviewed: 2026-11-12 (sonnet)

- `handle_io_buserr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d5cc
  **Inferred.** A 588-byte handler for I/O bus errors, going by its name. It calls get_matching_spool_addr at 0x1d628, then kl_ioerror_handler at 0x1d6c8, 0x1d73c and 0x1d7f4, passing 5 as its fourth argument. It logs through cmn_err_tag with message ids 0xb6c and 0xb6d at 0x1d798 and 0x1d7b4. It reads the doubleword at 0x9200000001000400 at 0x1d764, tests it against 0x100000 or 0x200000 (0x1d778) and stores a doubleword at 0x1d7a8 whose target and value the facts do not show. The branches that choose among the three handler calls are only partly resolved.
  Reviewed: 2026-11-12 (sonnet)

- `handle_uncac_buserr` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1d818
  **Inferred.** A 1208-byte handler for uncached bus errors, going by its name. It calls get_matching_spool_addr at 0x1d84c and calls nasid_to_partid and part_id at several sites; the facts do not show what they map. It calls partition_handle_error with a2 equal to 0xa, 0x9, 0x1, 0x6 and 0x5 (for example at 0x1d924 and 0x1d9d0), and calls error_check_memory_access, error_check_part_page, error_check_poisoned_state and error_page_discard along the way. Its cmn_err_tag calls use a0 from 0xb6e to 0xb79, and the branch rules that choose between these paths are not fully resolved.
  Reviewed: 2026-11-12 (sonnet)
