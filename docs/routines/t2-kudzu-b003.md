# Tier-2 routine readings, batch 3

These role statements cover 25 routines from the ranked target list of the second reading round, positions 75 to 99. Each one comes from the rendered facts of its routine: registers, calls, branches and constants at the addresses cited. Where the facts stop short of a meaning, the statement says so, and every label here is "inferred".

- `router_guardian_search` in `6.5.22/unix.IP27` at 0xc00000000002e4fc
  **Inferred.** It allocates two 0x1008-byte blocks with `kmem_alloc` (0xc00000000002e54c, 0xc00000000002e56c), sets each up with `queue_init`, and frees them with `kmem_free` (0xc00000000002e5b8, 0xc00000000002e5c8). Between those steps it works the queues with `queue_push`, `queue_pop`, `queue_find` and `queue_empty`. It builds a name with `sprintf` (0xc00000000002e630) and resolves it through `hwgraph_traverse` (0xc00000000002e640), then maps the vertex to a CPU with `nodevertex_to_cnodeid` and `cnodetocpu` (0xc00000000002e678, 0xc00000000002e690). The caller `router_guardian_set` (0xc00000000002e8b4) treats a return of -1 as a miss, but the facts do not show what a hit returns.

- `router_guardian_set` in `6.5.22/unix.IP27` at 0xc00000000002e83c
  **Inferred.** It builds two path strings with `board_to_path` (0xc00000000002e864, 0xc00000000002e880) and resolves one with `hwgraph_traverse` (0xc00000000002e890). It calls `router_guardian_search` (0xc00000000002e8b4) with a byte taken from offset 0x17 of its second argument, and when the result is not -1 it calls `guardian_nodepda_update` (0xc00000000002e8d0). Failure paths log through `cmn_err` at levels 4 and 0 (0xc00000000002e8e8, 0xc00000000002e8fc). The meaning of the guardian value and the return values are not in the facts.

- `router_guardians_set` in `6.5.22/unix.IP27` at 0xc00000000002e90c
  **Inferred.** It loops over a count word at 0xc0000000014d26a4, calling `find_lboard_class` with class 0x31 (0xc00000000002e9a8, 0xc00000000002e9c8) on each pass. A non-zero result goes to `router_guardian_set` (0xc00000000002e9dc). Its other data comes from loads through pointers the facts do not resolve, including a halfword at 0xc0000000014d3800. The callees suggest it applies router guardian settings to each router board, but that rests on the names of the callees alone.

- `hub_error_clear` in `6.5.22/unix.IP27` at 0xc0000000000321f8
  **Inferred.** It reads five doublewords and writes two at addresses formed from its first argument shifted left by 32 bits (0xc000000000032214 through 0xc000000000032238), then reads and rewrites one more (0xc000000000032244, 0xc000000000032254). It makes no calls. Its return value is the shifted base plus 0x9200000001c00000 (0xc000000000032250). The registers are not named, so a clear of hub error state for one node fits the access pattern but is not shown.

- `hubii_eint_init` in `6.5.22/unix.IP27` at 0xc0000000000325f8
  **Inferred.** It looks up a block with `hwgraph_info_get_LBL` (0xc000000000032638), picks an interrupt target with `intr_heuristic` (0xc000000000032684), and connects the handler at 0xc000000000034c34 through `intr_connect_level` (0xc0000000000326b4). It stores two doublewords into the table at 0x9200000001c00128 and 0x9200000001c00138 (0xc000000000032708, 0xc000000000032728). Failure paths log through `cmn_err` at level 0x14 (0xc000000000032750) and reach two `assfail` calls with line numbers 0x135 and 0x153 (0xc000000000032738, 0xc00000000003276c). The facts do not show which branch reaches which failure.

- `hubni_eint_init` in `6.5.22/unix.IP27` at 0xc00000000003277c
  **Inferred.** It maps a cnode to a CPU with `cnodetocpu` (0xc000000000032788) and logs through `cmn_err` at level 0x14 when that fails (0xc000000000032868, 0xc00000000003287c). It walks slices with `cnode_slice_to_cpuid` and `cpu_enabled` (0xc0000000000327e8, 0xc00000000003280c), with the loop bounded at 2 (0xc0000000000327dc). It connects the handler at 0xc000000000033088 through `intr_connect_level` with 0x7e in a1 (0xc0000000000327bc), and the handler at 0xc0000000000330ac through a second `intr_connect_level` call (0xc00000000003282c). The facts do not show the second call's vector argument.

- `hubni_print_state` in `6.5.22/unix.IP27` at 0xc000000000032b64
  **Inferred.** It takes a lock word with `mutex_spintrylock` (0xc000000000032c08) and, if the try fails, pauses with `us_delay` of 1 (0xc000000000032c84) and retries. It releases the lock with `mutex_spinunlock` (0xc000000000032c20) and names a slot with `get_slotname` (0xc000000000032c30). Two calls through a register (0xc000000000032c40, 0xc000000000032c54) take the string addresses 0xc000000001409b68 and 0xc000000001409b80, and `show_ni_port_error` (0xc000000000032c74) receives the port error value. The routine reads like a state dump for a network interface port, but the message formats are not in the facts.

- `hubni_eint_handler` in `6.5.22/unix.IP27` at 0xc000000000033088
  **Inferred.** It is a short wrapper that calls `hubni_error_handler` (0xc000000000032f00) with the constant pointer 0xc000000001409da0 and the flag 1 (0xc000000000033098). It passes its own third and fourth arguments through and returns the result of that call (0xc0000000000330a4). The constant pointer and flag are not named, so the wrapper's meaning rests on its callee.

- `hubni_rmteint_handler` in `6.5.22/unix.IP27` at 0xc0000000000330ac
  **Inferred.** It has the same shape as `hubni_eint_handler` (0xc000000000033088). It calls `hubni_error_handler` (0xc000000000032f00) with the constant pointer 0xc000000001409db0 and the flag 0 (0xc0000000000330bc), and returns that result. The different flag suggests a variant for remote interrupts, but the facts do not say what the flag means.

- `hubpi_spool_get_next` in `6.5.22/unix.IP27` at 0xc000000000033b10
  **Inferred.** It raises the interrupt level with `spl7` (0xc000000000033b2c) and restores it with `splx` on each exit path. It finds the NASID with `get_nasid` (0xc000000000033b24), indexes a per-node table by a byte at offset 0x520 of another table, and compares two words in one entry (0xc000000000033c70) to decide whether an item is available. It saves state through `klsave_spool_registers` with argument 2 (0xc000000000033d98) and returns 0, 1 or 2 (0xc000000000033d1c). The name suggests it takes the next entry from a spool ring, which the facts support only in outline.

- `huberror_process_rrb_error` in `6.5.22/unix.IP27` at 0xc000000000033db8
  **Inferred.** The name suggests a read-response error path. It converts the NASID to a partition with `nasid_to_partid` and `part_id` (0xc000000000033df8, 0xc000000000033e00), checks the page with `error_check_part_page` (0xc000000000033e10), and reports through `partition_handle_error` with codes 0xa, 0x1 and 0x4 (0xc000000000033e24, 0xc000000000033eb0, 0xc000000000033f14). When `page_isdiscarded` (0xc000000000033ef0) reports a discarded page, it logs through `cmn_err` at level 0x14 and calls `error_page_discard` with arguments 1 and 1 (0xc000000000033fec). It returns one of the small codes 0x0, 0x2, 0x4, 0x6, 0x7 or 0x8, and the meaning of each code is not shown.

- `huberror_process_wrb_error` in `6.5.22/unix.IP27` at 0xc00000000003400c
  **Inferred.** The name suggests the write-response counterpart of `huberror_process_rrb_error`. It calls `kl_ioerror_handler` first (0xc000000000034050), then converts the NASID to a partition and checks the page as the read path does (0xc000000000034088 to 0xc0000000000340ac). It reports through `partition_handle_error` with code 3 (0xc0000000000340b8 and three later sites) and checks `error_check_poisoned_state` and `error_check_memory_access` (0xc000000000034184, 0xc0000000000341a8). It logs four messages through `cmn_err` at level 0x14, and returns one of 0x0, 0xb, 0xc, 0xe, 0xf or an unknown value.

- `huberror_process_stack_entry` in `6.5.22/unix.IP27` at 0xc000000000034238
  **Inferred.** It dispatches a stack entry to `huberror_process_wrb_error` (0xc000000000034288) when bit 0 of a register is set (0xc0000000000342c0), and otherwise to `huberror_process_rrb_error` (0xc0000000000342c8). The second argument is checked against 1 and 2 first (0xc000000000034254, 0xc00000000003425c), and an unexpected value is logged through `cmn_err` at level 0x14 (0xc0000000000342e0). It stores a doubleword through its fourth argument (0xc000000000034298) and returns the result of the processor it called.

- `hubiio_crb_free` in `6.5.22/unix.IP27` at 0xc0000000000346f0
  **Inferred.** The name suggests it releases a CRB slot. It reads a halfword at offset 0xa of its first argument (0xc0000000000346f4 and later loads), computes an address from that field and its second argument shifted left by 5, and writes a doubleword there (0xc00000000003471c). It then polls bit 0x10 of a status doubleword, pausing with `us_delay` of 1 on each pass (0xc000000000034758, 0xc000000000034774). The return is the second argument OR'd with 0x10 or the `us_delay` result, and the register names are not in the facts.

- `hubiio_crb_error_handler` in `6.5.22/unix.IP27` at 0xc000000000034790
  **Inferred.** It takes the spin lock at offset 796 of its first argument (`mutex_spinlock_spl`, 0xc0000000000347dc), passes the error to `bte_crb_error_handler` (0xc00000000003492c), and frees the entry with `hubiio_crb_free` (0xc000000000034938, 0xc000000000034a6c). It dispatches on a code through a jump table at 0xc00000000140a1d0, bounded by a check against 8 (0xc0000000000349a4, 0xc0000000000349b0), and dumps state with `sn0_error_dump` (0xc0000000000349b8, 0xc000000000034ae8). It reports through `hub_ioerror_handler` with the third argument set to 1 or 3 (0xc000000000034a48, 0xc000000000034a60), and an `assfail` with line 0x70b (0xc000000000034a8c) marks a state it treats as impossible. The meaning of each jump-table case is not in the facts.

- `hubii_prb_handler` in `6.5.22/unix.IP27` at 0xc000000000034b10
  **Inferred.** It reads a halfword at offset 0xa of its second argument (0xc000000000034b28), logs it through `cmn_err` at level 4 (0xc000000000034b34), and writes a doubleword to an address built from that field shifted left by 32 (0xc000000000034b64). The register behind the store is not named in the facts, so its effect is unknown. The routine has no branches apart from its return.

- `hubiio_prb_error_handler` in `6.5.22/unix.IP27` at 0xc000000000034b68
  **Inferred.** It reads a halfword at offset 0xa of its second argument (0xc000000000034b74) and tests a bit in a value it loads from a computed address (0xc000000000034ba0). It calls `hubii_prb_handler` (0xc000000000034b10) at 0xc000000000034c10 and again at 0xc000000000034c24, passing its own arguments on, and loops with a bound of 0x10 (0xc000000000034bd8). The facts do not resolve the return value, so the purpose of the loop is unclear.

- `hubii_eint_handler` in `6.5.22/unix.IP27` at 0xc000000000034c34
  **Inferred.** It is the top-level handler for the hub's I/O interrupt, and `hubii_eint_init` connects it (0xc0000000000326b4). It calls `check_ni_errors` (0xc000000000034c40), and when that returns non-zero it calls `hubni_error_handler` with flag 1 (0xc000000000034c58). It tests bits of the status words at 0x9200000001c00008 and 0x9200000001c001e8 (0xc000000000034c9c, 0xc000000000034d10, 0xc000000000034d34, 0xc000000000034d44), and dispatches to `hubiio_crb_error_handler` (0xc000000000034ca8) or `hubiio_prb_error_handler` (0xc000000000034cb4). The bit meanings and the `cmn_err` message at level 0 (0xc000000000034d20) are not shown.

- `hub_error_count_update` in `6.5.22/unix.IP27` at 0xc000000000034f20
  **Inferred.** It is a periodic task. It calls `ii_llp_error_monitor` with the block at offset 8 of a state structure (0xc000000000034f54) and `ni_llp_error_monitor` with the block at offset 0x20 (0xc000000000034f60). It re-arms itself through `timeout` with its own address and a delay argument of 0x64 (0xc000000000034f80), but only when a word in the structure is non-zero (0xc000000000034f70). The structure is reached through the pointer at 0xc0000000014dc2e8, and the facts do not show what the monitors report.

- `hub_error_count_enable` in `6.5.22/unix.IP27` at 0xc000000000034f98
  **Inferred.** It arms the periodic count that `hub_error_count_update` (0xc000000000034f20) runs. It clears a 0x40-byte block at the pointer held in 0xc0000000014dc2e8 with `blkclr` (0xc000000000035010), writes a word into it (0xc000000000035034), and schedules the update with `timeout` and a delay argument of 0x64 (0xc000000000035030). It returns early when its argument is negative (0xc000000000034fb0), and a table value at 0xc0000000014d3600 equal to -1 leads straight to the return (0xc000000000034fcc). The return values are 0 and -1 (0xc000000000034ff8).

- `hub_error_count_disable` in `6.5.22/unix.IP27` at 0xc000000000035044
  **Inferred.** The name pairs it with `hub_error_count_enable` (0xc000000000034f98). It writes a word to the block at the pointer in 0xc0000000014dc2e8 (0xc000000000035084) and returns -1 when its argument is negative (0xc000000000035090), or 0 or -1 otherwise (0xc000000000035088). The routine makes no calls, so it does not cancel the `timeout` that `hub_error_count_enable` schedules, and whether the periodic update stops is not shown.

- `hub_error_count_get` in `6.5.22/unix.IP27` at 0xc000000000035098
  **Inferred.** It reads the count set up by `hub_error_count_enable`. A negative argument, an argument of 0x100 or more, or a table value of -1 at 0xc0000000014d3600 returns 0 without a store (0xc0000000000350a0, 0xc0000000000350b4, 0xc0000000000350c0). Otherwise it stores a word through its second argument (0xc0000000000350cc) and returns the doubleword it loads from 0xc0000000014dc2e8 (0xc0000000000350d4). The facts do not say what the stored word means.

- `error_check_poisoned_state` in `6.5.22/unix.IP27` at 0xc000000000035f7c
  **Inferred.** The name and callees suggest it checks a page for poisoning on behalf of the partition that owns the node of its first argument, using `nasid_to_partid` and `part_id` (0xc000000000035fac, 0xc000000000035fb4) and `get_dir_ent` (0xc000000000035fd4). It calls `migr_check_migr_poisoned` (0xc000000000036000), `page_ispoison` and `page_isdiscarded` (0xc000000000036060, 0xc000000000036098), and `check_md_errors` (0xc0000000000360bc, 0xc0000000000360fc). It reports through `cmn_err_tag` with message IDs 0xb60 to 0xb63, calls `error_page_discard` for some outcomes, and calls `kl_panic` for another (0xc000000000036140). The return is 0 to 3, and the facts do not show which return means which outcome.

- `kl_panic` in `6.5.22/unix.IP27` at 0xc0000000000374f4
  **Inferred.** It is a thin wrapper. It calls `kl_panic_setup` (0xc000000000037500) with its four arguments, then logs through `cmn_err` at level 0x12 with its first argument (0xc00000000003750c), and returns that call's result (0xc00000000003751c). The facts do not show whether `kl_panic_setup` returns, so the log line may never run.

- `intr_stray` in `6.5.22/unix.IP27` at 0xc000000000037c4c
  **Inferred.** It reads a halfword at the fixed address 0xffffffffffffc420 (0xc000000000037c64) and passes it as the last argument to `cmn_err` at level 4 (0xc000000000037c60). The message is at 0xc00000000140b738, and the routine returns the result of that call (0xc000000000037c6c). The name suggests a log of a stray interrupt, which one `cmn_err` call is consistent with, but the register at 0xffffffffffffc420 is not named.
