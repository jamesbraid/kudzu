# Routine readings, tier-1 kudzu batch 40

These entries are role statements for 25 routines from the tier-1 reading round, batch 40. Each one is written from the disassembly (registers, calls and constants) and cites the addresses and callees it shows. Every role is inferred: the disassembly shows structure, not intent.

- `silence_all_audio` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1c58
  **Inferred.** The disassembly for this 8-byte routine shows only a return at 0x1c58 with v0 unknown, so no calls, device accesses or branches are visible. The name suggests it quiets audio, but nothing in the disassembly touches an audio device, so that reading stays unconfirmed. Treat it as a stub until a wider disassembly shows its callees.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_node_probe` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x630c
  **Inferred.** This 768-byte probe calls get_nasid (0x63ac), do_cpumask (0x6418) and printf three times, with assfail at 0x65e8 as its failure path. It reads a per-node record through a hub-region address built from get_nasid's result (shown as 0x9600000000000000 | 0x2000 with offsets 0xd0, 0x184 and 388), and it walks arrays bounded by comparisons against 0x80, 0x200 and 0x3 while storing halfwords and words. A magic value 0x5045626b is compared at 0x644c, and a mismatch skips to 0x6584; the disassembly does not say what that constant identifies. The purpose, a per-node CPU census, is a guess from the callees and bounds.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0xd7cc
  **Inferred.** This 1152-byte routine calls r1nkperf_control_register_set and r1nkperf_data_register_set (0xd7e0 to 0xd804, and again at 0xd864 to 0xd888) and get_cpu_irr (0xd80c, 0xd824). Depending on get_cpu_irr results and a test of s3, a path calls flush_tlb (0xd844) and flush_cache (0xd84c) and then returns the flush_cache result at 0xd858; it is not a step on the way to the rest of the routine. The main path calls get_nasid, set_master_bridge_base, ioc3_mlreset, ioc4_mlreset, l1_mlreset and nvram_baseinit (0xd898 to 0xd928), then is_fine_dirmode and is_specified on arg_maxnodes (0xd940). Only when is_specified returns nonzero does it call atoi and redo_nodes on arg_maxnodes (0xd950, 0xd958). It then calls cpu_node_probe (0xd988) with boot_cpumask and numnodes, runs loops bounded by numnodes and maxnodes, and calls gen_region_mask, getcpuid, get_cpu_cnode, setup_replication_mask, get_nasid, spinlock_init, iograph_early_init, mem_init, hubdev_init, gda_sym_init, verify_snchip_rev, he_arcs_set_vectors and intrgroup_init (0xda3c to 0xdbec). It compares master_nasid with the get_nasid result at 0xda84. Three assfail calls (0xdc04, 0xdc20, 0xdc3c) are failure paths reached by branches, such as a nonpositive cpu_node_probe result at 0xd990 and a master_nasid mismatch at 0xda84, not a block that runs after the initializers. Role (from name): early machine reset on the master node. The disassembly does not say what each callee does.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0xe718
  **Inferred.** This 116-byte routine calls splhi (0xe724) and get_nasid (0xe72c), then calls nasid_to_cpuinfo (0xe740) with the get_nasid result and a doubleword loaded from 0x9200000001000020 as its second argument, and calls splx (0xe74c) with the splhi result. When the lookup returns null (branch at 0xe754) it reaches an assfail at 0xe77c; otherwise it returns the halfword at offset 0x1e of the returned record (0xe760). Role (from name): a CPU-id lookup for the calling CPU. The disassembly shows only the field read, not what it means.
  Reviewed: 2026-11-12 (sonnet)

- `page_local_calias` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10124
  **Inferred.** This 60-byte routine loads a doubleword from the fixed address 0x9200000001000028 (0x10128). A zero value branches to 0x10154; otherwise an unsigned compare at 0x1014c decides between the two results returned at 0x10158 (0 or 1). The disassembly does not show what the word or the compared values represent, so the page-alias purpose, which comes from the name, cannot be checked from this disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `save_cache_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x12888
  **Inferred.** This 416-byte routine calls get_nasid (0x12898) and loads a doubleword (0x128b8). A branch at 0x128d4 compares a value with 0xc000 and, when they match, goes to 0x12a18, which calls chk_for_pebrick. Otherwise it loads a doubleword at offset 0x48 (0x128dc) and a word (0x128e8), runs loops over bytes and words with a count compared against 0x10 (0x12938), compares a loaded doubleword with the word at 0x9200000001000020 (0x129cc), and then stores doublewords at 0x129f4, 0x129f8 and 0x12a08, a word at 0x129fc and a byte at 0x12a04 before returning at 0x12a10. Role (from name): saving cache-error state into a record. The disassembly does not name the fields.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_pi_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1362c
  **Inferred.** This 216-byte routine zeroes a 0x60-byte destination (its second argument) with bzero (0x13648). It loads doublewords from its first argument at offsets 0x400, 0x50, 0x58, 0x430, 0x440, 0x450, 0x460, 0x4c8 and 0x4d0. It stores the 0x400 value at destination +0x10 (0x13654), then tests the 0x50 and 0x58 words for zero (branches at 0x13660 and 0x1366c) and branches to 0x136e8 and 0x136f8 respectively when they are zero; the facts show those paths only as further stores to +0x10. The facts show no store of the 0x50 or 0x58 values themselves. For each pair (0x430 with 0x440, 0x450 with 0x460, 0x4c8 with 0x4d0) the first value is stored at destination +0x28, +0x30 and +0x48 and the second at +0x38, +0x40 and +0x50, only when bit 0x8000000000000000 of the first value is set (branches at 0x13680, 0x136a0, 0x136c0). One path returns the doubleword at source offset 0x58 (0x136e0). Role (from name): saving hub PI error registers. The field names are not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_md_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13704
  **Inferred.** This 236-byte routine zeroes a 0x58-byte destination (its second argument) with bzero (0x13720) and loads doublewords from its first argument at offsets 0x7c0010, 0x7e0010, 0x7c0020, 0x780040 and 0x780048, storing into destination offsets 0x8, 0x18, 0x28, 0x38 and 0x40. The stores are conditional: branches test whether the 0x7c0010 value is zero (0x13744) and whether its bits, shifted left by 1, 2 and 3, are negative (0x137ac to 0x137bc), whether a value loaded from 0x7e0010 masked with 0x3 is zero (0x13760), and whether the 0x7c0020 and 0x780040 values are zero (0x13774, 0x13784). Those branches select which stores run; the facts show no loop that repeats them. Role (from name): saving hub memory-directory error registers. The field names are not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_io_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13868
  **Inferred.** This 164-byte routine copies hub I/O error registers into a record and then hands off to two helpers. It zeroes a 0xd0-byte destination with bzero, copies source doublewords at offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008 and 0x420010, then calls save_hub_io_crb (with the 0x410000 and 0x400008 values as arguments) and save_hub_io_ieph. Its return value is whatever save_hub_io_ieph returns. The grouping of the copies follows the offsets in the access pattern, not any named register list.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_err_ext` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1390c
  **Inferred.** This 184-byte routine zeroes a 0xc0-byte destination (its second argument) with bzero (0x13928), then runs a loop that loads and stores 8-byte values (0x13958, 0x1395c) while a counter register (shown as a2) stays below 0xf (0x13960, 0x13984). A second loop of loads and stores (0x139a8, 0x139b0) is bounded by 0x9 (0x139ac). The addresses are computed from the counters, not fixed constants, so the disassembly does not name the registers involved. Role (from name): extended hub error fields; this stays unconfirmed.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_ni_error` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x139c4
  **Inferred.** This 100-byte routine zeroes a 0x30-byte destination (its second argument) with bzero (0x139e0) and loads four doublewords from its first argument at offsets 0x680100, 0x680108, 0x680110 and 0x680118, storing them at destination offsets 0x8, 0x10, 0x18 and 0x20. When the doubleword at offset 0x680100 is zero (0x139fc) it skips all four stores and returns the bzero result (0x13a20). Those source offsets are the only register identifiers in the disassembly. Role (from name): saving NI error registers; the record layout is inferred from the store pattern.
  Reviewed: 2026-11-12 (sonnet)

- `hub_error_init` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x17950
  **Inferred.** This 1464-byte routine calls hub_error_clear with a nasid read from compact_to_nasid_node (0x17974) and get_compact_nodeid (0x17988), then hubii_eint_init (0x17a0c) and hubni_eint_init (0x17a14), and recover_error_init four times (0x17a84 to 0x17aa8). It calls cause_intr_connect with 0x6 and hubpi_eint_handler (0x17d8c) and, in a loop bounded by numnodes (0x17db8, 0x17e20), kmem_zalloc_node with sizes 0x2258 (0x17ddc) and 0x40 (0x17df8). It calls spinlock_init on kl_error_log (0x17e90). Three assfail calls (0x17eac, 0x17ec8, 0x17ee0) and a cmn_err (0x17ef8) are reached by branches off the main path, such as those at 0x17e08 and 0x17e10. It also stores doublewords at addresses the facts do not resolve, so which hub registers it writes and which base address each loop uses are not shown. Role (from name): hub error initialization.
  Reviewed: 2026-11-12 (sonnet)

- `hubpi_eint_handler` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x181d0
  **Inferred.** This 1624-byte routine first calls check_ni_errors (0x181d8) and, when that returns nonzero, hubni_error_handler (0x18200). It then reads pi_fatal_err_cpu_a, pi_fatal_err_cpu_b and pi_err_int_pend and tests bit masks from them in several groups. It reaches many cmn_err calls at severities 0x10, 0x12, 0x14 and 0x15, some passing the doublewords at 0x92000000010004c8 and 0x92000000010004d0; on some paths it calls kl_panic_setup, get_nasid, log_md_errors, md_int_block, process_error_spool (0x1835c, 0x183c0, 0x18424) and part_id. Role (from name): the hub PI error interrupt handler. The control structure is visible in the disassembly, but which fatal category each mask stands for is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `check_ni_errors_nasid` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18a24
  **Inferred.** This 764-byte routine checks the NI error state of one nasid. It reads NI registers at addresses built from the nasid argument, calls part_id, and calls ni_port_down_reset at 0x18a80, 0x18b00 and 0x18cc8 under conditions the disassembly only partly shows. It reports through many cmn_err calls, sends protected_broadcast messages 0x67 and 0x68 at 0x18c94 and 0x18c9c, and returns 0 or 1. The NI register offsets appear only as address arithmetic, so their names remain open.
  Reviewed: 2026-11-12 (sonnet)

- `check_ni_errors` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18ec0
  **Inferred.** This 36-byte routine is a thin wrapper. It calls get_nasid at 0x18ec8 with its own arguments, then calls check_ni_errors_nasid (0x18a24) with the resolved nasid and returns that callee's result. The disassembly shows no other work, so the nasid lookup is the whole job.
  Reviewed: 2026-11-12 (sonnet)

- `hubni_error_handler` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18ee4
  **Inferred.** This 56-byte routine is another wrapper. It calls get_nasid at 0x18ef4, then calls hubni_error_handler_nasid at 0x18d20 with its own first argument as the first, the get_nasid result as the second and its own second argument as the third. It returns that callee's result. The disassembly does not show what the nasid-level handler does.
  Reviewed: 2026-11-12 (sonnet)

- `klsave_spool_registers` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1994c
  **Inferred.** This 468-byte routine reads the local-state word at 0x9200000001000020, calls get_nasid several times (0x1996c to 0x199f4), and calls find_lboard with type 0x11 (0x19a4c) and, when that returns nonzero, find_first_component with type 0x2 (0x19a60). It loads doublewords at offset 0x48 and a word at +0x30 and stores six doublewords, in two groups of three (0x19ac8 to 0x19ad4 and 0x19b0c to 0x19b1c) selected by a sign test at 0x19ac0. When a loaded value equals 0xc000 (0x1998c) it instead calls get_nasid and chk_for_pebrick (0x19af4). Role (from name): saving register state into an error spool. The record layout and the meaning of the type codes are not established by the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `ii_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1b19c
  **Inferred.** This 104-byte routine performs a load and store of doublewords at offset 0x0 (0x1b1a0, 0x1b1b0), then another load and store at +0x0 (0x1b1bc, 0x1b1c4) and at +0x8 (0x1b1d0, 0x1b1d8), and byte loads and stores at +0x10 and +0x11 (0x1b1e4, 0x1b1f0, 0x1b1f4, 0x1b200). The byte accesses are selected by comparisons with 0xffff (0x1b1b4, 0x1b1c8). The facts do not resolve the address of the first load and store or which side of each access is the argument record, so a copy or a changed value cannot be confirmed. Role (from name): an II link-error monitor.
  Reviewed: 2026-11-12 (sonnet)

- `ni_llp_error_monitor` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1b204
  **Inferred.** This 300-byte routine reads the register at 0x9200000001680120 (0x1b208) and tests its shifted sign bit and several byte values with masks of 0xf (0x1b224 to 0x1b284); those tests branch to 0x1b318 or 0x1b31c. A cmn_err call at 0x1b320 (severity 0x4, with the register value as an argument) follows them, and the facts do not show which of the branch targets reach it and which skip it. Separately, comparisons with 0xff (0x1b29c, 0x1b2b4, 0x1b2c8) choose between a doubleword load and store at +0x0, +0x8 and +0x10 and a byte load and store at +0x18, +0x19 and +0x1a, and the routine returns at 0x1b2e0. Role (from name): the NI counterpart of ii_llp_error_monitor. The meaning of the bit tests is not given in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `err_int_unblock_mask` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1b980
  **Inferred.** This 76-byte routine loads a halfword (0x1b980), a doubleword at offset +0x8 (0x1b990) and a flag byte at +0x528 (0x1b994), skips a doubleword store when a1 is zero (0x1b9a8, 0x1b9b4), then loads a doubleword (0x1b9bc) and stores a doubleword (0x1b9c8) before returning. The facts show these accesses only as offsets; the target addresses are not resolved here. Role (from name): unblocking an interrupt mask. The disassembly does not say whether bits are set or cleared.
  Reviewed: 2026-11-12 (sonnet)

- `stop_all_cpus` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1bcf0
  **Inferred.** This 572-byte routine reads the local-state word at 0x9200000001000020 and tests its low bit (0x1bd64), loads and stores doublewords at addresses the facts do not resolve, and compares registers with 0x1, 0x2 and 0x4 and masks one with 0x3 (0x1bd80 to 0x1be48). It returns at 0x1bef4. Role (from name): halting all CPUs; the disassembly shows no call that would do that, so the purpose stays open.
  Reviewed: 2026-11-12 (sonnet)

- `handle_io_buserr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1cd5c
  **Inferred.** This 592-byte routine calls get_matching_spool_addr (0x1cdb8), then kl_ioerror_handler at 0x1ce58, 0x1cecc and 0x1cf88 with the nasid_to_compact_node entry and a constant 0x5 as its fourth argument, each on a different branch path. It reports through cmn_err_tag with codes 0xb6c (0x1cf2c) and 0xb6d (0x1cf48) and calls get_nasid (0x1cf98). It loads the halfwords at 0xffffffffffffc428 and 0xffffffffffffc42a, the doubleword at 0x9200000001000400 and a per-node doubleword at offset 0x110, and tests bits 0x100000 and 0x200000 of that word (0x1cf0c). Role (from name): handling I/O bus errors. The branch structure is visible, but the disassembly does not say what each error class means.
  Reviewed: 2026-11-12 (sonnet)

- `handle_uncac_buserr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1cfac
  **Inferred.** This 2308-byte routine calls get_matching_spool_addr (0x1cfe0), then nasid_to_partid and part_id (0x1d098, 0x1d0a0, 0x1d4c0, 0x1d4c8) and compares their results. It reports through cmn_err_tag codes 0xb6e, 0xb6f, 0xb70, 0xb71, 0xb72, 0xb73 and 0xb79, calls partition_handle_error twice with 0x9 as its third argument (0x1d0e8, 0x1d588), and calls error_page_discard (0x1d528) and error_check_part_page (0x1d55c). A bounds check against 8 at 0x1d03c followed by a load at 0x1d044 may be a table dispatch; the facts do not resolve it, so part of the routine is unread. Role (from name): handling uncached bus errors.
  Reviewed: 2026-11-12 (sonnet)

- `handle_cac_buserr` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1d8b0
  **Inferred.** This 2420-byte routine calls get_matching_spool_addr (0x1d8e8), nasid_to_region (0x1d9a8, 0x1d9d4), nasid_to_partid and part_id (0x1da00, 0x1da08), and can reach kl_panic at 0x1d964 when a value is nonzero (0x1d924). It also calls error_lookup_inst_type, error_check_memory_access, error_check_part_page, error_check_poisoned_state, memory_iscalias and klerror_check_recoverable, calls partition_handle_error with third argument 0x6, 0x9, 0x5 and 0x2 (0x1da58, 0x1dd68, 0x1ddac, 0x1e194), and reports through cmn_err_tag with codes 0xb7a to 0xb7d, 0xb7f, 0xb80, 0xb82 and 0xb8c. A bounds check against 8 at 0x1d950 followed by a load at 0x1d958 may be a table dispatch; the facts do not resolve it, so the decision tree is only partly visible. Role (from name): handling cached bus errors.
  Reviewed: 2026-11-12 (sonnet)

- `intpend0` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x200f0
  **Inferred.** This 352-byte routine loads the doubleword at 0x9200000001000098 and a second doubleword at -0x3a60 and branches on their AND (0x20130, 0x201f8). It calls ms1bit (0x20168) and indexes a table at -0x3a50 with a 72-byte stride by the ms1bit result. On that slot it calls cvsema (0x20140), valusema (0x201bc) and vsema (0x201dc), and a branch at 0x201d4 compares the valusema result with hub_intr_wakeup_cnt. An indirect call whose target the facts do not resolve (0x201ac, 0x20240) takes the slot's doubleword at +0x10 as its first argument; the call at 0x201ac is made when the word at slot +0x18 has bit 0x1000 set and the pointer at slot +0x8 is nonzero, and the call at 0x20240 is on the path where that bit is clear. Role (from name): dispatching pending hub interrupt bits; what the semaphore calls guard and the table contents are not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)
