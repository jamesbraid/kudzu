# Routine role reads, kudzu round 2 batch 2

These entries say what 25 routines of the IP27 kernel image (`6.5.22/unix.IP27`) do, read from the instruction facts rendered for each one. Every label is Inferred: the facts give control flow, loads, stores, callees and constants, but not the meaning of the hardware registers or table fields they touch. Where a role rests on the routine name alone, the entry says so.

- `cpu_enabled` in `6.5.22/unix.IP27` at 0xc000000000026f74
  **Inferred.** Loads the 8-byte word at table 0xc0000000014d3990 and branches on whether a0 equals -1 (the beq at 0xc000000000026f88 goes to the return at 0xc000000000026fa4). The return yields 0x0 or a value the facts do not name. The name suggests a per-CPU enabled test; the facts do not show what the table holds.
  Reviewed: 2026-11-12 (sonnet)

- `cnode_slice_to_cpuid` in `6.5.22/unix.IP27` at 0xc000000000026fac
  **Inferred.** Indexes a pointer table by a0 scaled by 8, reads halfwords at +0x4956 and +0x4958 of the selected record, and loops over candidates, calling cpu_enabled at 0xc000000000026ff4 for each and comparing a1 with a value from the table at 0xc0000000014d39a8. It returns -1 on one path (the return at 0xc000000000027050). The name suggests a mapping from a node-and-slice pair to a CPU id; the matching path's value is not named in the facts, so the mapping is a reading of the loop shape.
  Reviewed: 2026-11-12 (sonnet)

- `hub_ioerror_local` in `6.5.22/unix.IP27` at 0xc0000000000273bc
  **Inferred.** Compares a2 with 0x0, 0x2 and 0x3 (beq at 0xc0000000000273c8 and 0xc0000000000273d4, bne at 0xc0000000000273dc) and returns 0x0 for a2 of 0x0, 0x2 or 0x3 without reading the record in a3. For any other a2 it reads the record: a word at +0x0, halfwords at +0xc and +0xe (each used to index a table) and a doubleword at +0x10. The sign of the shifted word (bgez at 0xc000000000027420) selects a value, and a test of a loaded doubleword against a mask (beq at 0xc000000000027458) decides whether the first pair of calls runs: cmn_err at 0xc0000000001f8090 (level 4, call at 0xc00000000002746c) followed by kl_panic at 0xc0000000000374f4 (call at 0xc000000000027478). A second cmn_err (level 4, at 0xc00000000002749c) and kl_panic (at 0xc0000000000274a8) pair follows on the remaining path. The facts show the tests and the calls, not what the fields mean or whether the panics return.
  Reviewed: 2026-11-12 (sonnet)

- `hub_xp_error_handler` in `6.5.22/unix.IP27` at 0xc0000000000274b8
  **Inferred.** Finds a per-node record through the pointer table at [-0x3c00] indexed by a halfword from table 0xc0000000014d3600 and reads a word at +0x49e0 of it. A beq at 0xc0000000000274e0 tests bits 0x11 of a doubleword loaded from an address built from a1 shifted left by 32 plus a table value; when those bits are clear it skips the call and returns 0x7. Otherwise it calls xtalk_error_handler at 0xc00000000006dad0 with that word as a0, the caller's a2, a3 and a4 as a1, a2 and a3, and a6=0x7, and returns the callee's result. The name and callee suggest forwarding a hub-side crossbar error to the generic widget error handler; the facts do not show what the tested bits mean.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dma_enabled` in `6.5.22/unix.IP27` at 0xc000000000027534
  **Inferred.** Resolves a widget record through xwidget_info_get at 0xc00000000006dcb0, xwidget_info_id_get and xwidget_info_master_get, then looks up an info item with hwgraph_info_get_LBL at 0xc000000000296160. When the lookup result is nonzero and the halfword it points to is not -1 (beq at 0xc000000000027588 and 0xc000000000027598), it reads the hub register at 0x9200000001c00118 and tests it against a mask (beq at 0xc0000000000275b8). The return is 0x1 when the masked bits are nonzero; when they are zero the routine calls cmn_err at 0xc0000000001f8090 (level 6, call at 0xc0000000000275e8). Every other path the facts show returns 0x0. The name suggests a DMA-enabled query for one widget; the mask's meaning is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_widgetdev_shutdown` in `6.5.22/unix.IP27` at 0xc0000000000275f8
  **Inferred.** Uses the same widget lookup chain as hub_dma_enabled. When the lookup result is nonzero and the halfword it points to is not -1 (beq at 0xc00000000002766c and 0xc000000000027680), it reads and writes the 8-byte hub register at 0x9200000001c00120 (ld at 0xc000000000027698, sd at 0xc0000000000276a8). A loop follows whose index is compared with 0xf (beq at 0xc0000000000276c0); it reads registers starting at 0x9200000001c00400, tests shifted bits of them and calls hubiio_crb_free at 0xc0000000000346f0. Errors are reported through cmn_err at 0xc000000000027798 and 0xc0000000000277b4 (level 6). The name suggests disabling one widget's device on the hub; the facts show the register accesses, not which bits they clear.
  Reviewed: 2026-11-12 (sonnet)

- `hub_widgetdev_enable` in `6.5.22/unix.IP27` at 0xc0000000000277d0
  **Inferred.** Resolves the widget with the same lookup chain. When the lookup result is nonzero (beq at 0xc00000000002782c) it reads the register at 0x9200000001c00120 and tests it against a mask held in a register (beq at 0xc000000000027858). The store back to that register (sd at 0xc00000000002786c) runs only on the path where that test is zero. The return is the hwgraph lookup result when the lookup result is zero, and the base 0x9200000001c00000 otherwise. The name suggests setting a widget device enable bit; the facts show the test and the store, not which bit is set.
  Reviewed: 2026-11-12 (sonnet)

- `hub_error_devenable` in `6.5.22/unix.IP27` at 0xc000000000027870
  **Inferred.** Resolves the widget record with the same lookup chain and compares a2 with 0x5 and 0x6 (beq at 0xc0000000000278c8 and 0xc0000000000278d0). For a2 of 0x6, or any other value, it returns 0x0 without reaching the register code. For a2 of 0x5 it reads the register at 0x9200000001c002c8 and branches on its sign after a shift by 7 (bltz at 0xc000000000027908). The store to 0x9200000001c00388 (sd at 0xc000000000027930) runs only when that branch is taken. Every return the facts show is 0x0. The name suggests enabling error reporting on a widget; the meanings of the 0x5 and 0x6 cases and of the stored value are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_ioerror_handler` in `6.5.22/unix.IP27` at 0xc00000000002799c
  **Inferred.** Dispatches on a1 against 0x5, 0x6, 0x9 and 0xa (beq at 0xc0000000000279e4 through 0xc000000000027a00) to per-error branches. Those branches call hub_xp_error_handler at 0xc0000000000274b8 and hub_ioerror_local at 0xc0000000000273bc, and use is_widget_pio_enabled at 0xc000000000027934 to decide whether the PIO path applies. It writes fields of the record in a3 (sd at +0x18, sw at +0x0, sh at +0x6) and calls ioerror_dump at 0xc00000000004c050 (twice, at 0xc000000000027d74 and 0xc000000000027d90). The return at 0xc000000000027a3c is one of 0x4, 0x5, 0x0 or a value the facts do not name. The name suggests the top-level IO error handler for a hub; the case meanings are inferred from the dispatch shape.
  Reviewed: 2026-11-12 (sonnet)

- `hub_merge_clean` in `6.5.22/unix.IP27` at 0xc000000000028a90
  **Inferred.** Loops back (bnez at 0xc000000000028b0c) while a1 is nonzero; a1 is a register the routine sets itself (by the disassembly, a pass counter compared with 0xf), not an argument. Each pass tests the low 15 bits of a masked word (beq at 0xc000000000028ae0 on t3 & 0x7fff) and, when they are nonzero, loads further words (ld at 0xc000000000028ae8, lw at 0xc000000000028b08) from addresses the facts do not resolve; the first load's address is built from a0 shifted left by 32 plus a table value. It makes no calls and returns an unnamed value. The name suggests clearing merged error state, but the rendered facts show only the loop and the mask, so the purpose is a reading from the name.
  Reviewed: 2026-11-12 (sonnet)

- `hub_init_ifdr_war` in `6.5.22/unix.IP27` at 0xc000000000028b1c
  **Inferred.** Indexes the pointer table at [-0x3c00] with a0 scaled by 8 and writes three fields of that per-node record: halfwords at +0x4c82 and +0x4c84 and a word at +0x4c7c, the first copied from table 0xc0000000014d3800. It then compares the halfword at +19588 of the record (the +0x4c84 field) unsigned with an unnamed value (beq at 0xc000000000028ba4, taken when the halfword is not below it, which goes to the return). On the fall-through side, when a0 is zero (beq at 0xc000000000028bb8), it calls cmn_err at 0xc0000000001f8090 (level 5) at 0xc000000000028be8. The name suggests initializing a per-node value for a hardware workaround; the facts show the comparison, not the hardware meaning.
  Reviewed: 2026-11-12 (sonnet)

- `hub_ifdr_check_value` in `6.5.22/unix.IP27` at 0xc000000000028c18
  **Inferred.** Reads the halfword at +0x4c84 (19588) of the per-node record selected by a0 and compares it, unsigned, with an unnamed value (beq at 0xc000000000028c70). It returns 0x1 when the halfword is below that value and 0x0 otherwise (returns at 0xc000000000028c78 and 0xc000000000028c80). It returns 0x0 early when the word at 0xc0000000014c8f8c is nonzero (the beq at 0xc000000000028c1c is taken, to the comparison, only when the word is zero). The name suggests a range check on a stored workaround value; the facts show the comparison only.
  Reviewed: 2026-11-12 (sonnet)

- `hub_ifdr_fix_value` in `6.5.22/unix.IP27` at 0xc000000000028c88
  **Inferred.** Reads and rewrites the halfword at +0x4c82 of the per-node record selected by a0 (lhu at 0xc000000000028cd8, sh at 0xc000000000028cec), compares an unnamed register with 0x41 unsigned (beq at 0xc000000000028cb4), and runs a branch-likely loop that decrements v0 (bnel at 0xc000000000028d18). It returns -1 or 1. The name suggests adjusting a stored value into a valid range; the loop and masks do not show what the field means.
  Reviewed: 2026-11-12 (sonnet)

- `hub_setup_kick` in `6.5.22/unix.IP27` at 0xc000000000028d74
  **Inferred.** When a0 is nonzero (beq at 0xc000000000028d84) it calls spinlock_init at 0xc000000000286fe8 on a lock inside the per-node record selected by a0 (+19312, call at 0xc000000000028da8), writes a word at +0x4b6c, and writes a word at +0x4b68 as it returns spinlock_init's result (sw at 0xc000000000028de8); it does not reach the other two calls. When a0 is zero it calls spinlock_init on the locks at 0xc0000000014d13e0 and 0xc0000000014d13e4 instead (calls at 0xc000000000028df4 and 0xc000000000028e00). The name suggests setting up the state that a timeout kick routine uses; the facts show the lock inits and the stores.
  Reviewed: 2026-11-12 (sonnet)

- `hub_dmatimeout_kick` in `6.5.22/unix.IP27` at 0xc000000000028e90
  **Inferred.** Takes the lock at 0xc0000000014d13e0 with mutex_spinlock at 0xc000000000287214 when a1 is nonzero, or with mutex_spintrylock at 0xc00000000028748c when a1 is zero (beq at 0xc000000000028ec4), and releases it with mutex_spinunlock at 0xc000000000287260. It writes the two doublewords at 0xc0000000014d41a0 and 0xc0000000014d41a8 (sd at 0xc000000000028f14 and 0xc000000000028f20) and tests them later. A loop bounded by the count at 0xc0000000014d13a0 calls cpuaction at 0xc0000000001e1940, and bte_wait_for_xfer_completion at 0xc00000000002d748 is called after it. A later section compares a read of the hub counter at 0x9200000001030100 with an earlier read of it plus 0x1c9c380 (30,000,000) and calls us_delay at 0xc000000000047274 (with 0x5, then 0x2 twice), hub_merge_clean at 0xc000000000028a90 and hub_ifdr_fix_value at 0xc000000000028c88. The name suggests a DMA-timeout recovery kick; the sequence is clear, but the counter's meaning is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_widget_reset` in `6.5.22/unix.IP27` at 0xc0000000000293ac
  **Inferred.** Calls master_node_get at 0xc000000000297a94. When it returns -1 (beq at 0xc0000000000293e0) the routine takes a separate path. Otherwise it calls spl1 at 0xc00000000003affc, calls hub_setup_prb at 0xc00000000003c028 with a2=0x3 and a3=0x2, and calls splx with spl1's result. Two assfail calls at 0xc00000000003a2b4, each with a2=0x562, are on other paths; the facts do not show when they are reached. The name suggests resetting one widget's hub-side state; the stored fields are not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `mem_init` in `6.5.22/unix.IP27` at 0xc000000000029704
  **Inferred.** Calls init_mutex at 0xc00000000018dc14 with a0 set to the lock at 0xc0000000014ddbd8, a1=0, a2 set to a pointer at 0xc0000000014066d8 and a3=-1, and returns that call's result. The name suggests setting up the memory subsystem's lock at boot; this routine is one call and the lock's other users are outside it.
  Reviewed: 2026-11-12 (sonnet)

- `szmem` in `6.5.22/unix.IP27` at 0xc00000000002972c
  **Inferred.** Reads a slot count at 0xc0000000014d269c, loops over slot indices while s0 is below 0x20 (bne at 0xc0000000000297d4), and calls slot_psize_compute at 0xc000000000029d70 for each slot. It stores a word and two halfwords per slot (sw at 0xc0000000000297a0, sh at 0xc0000000000297b4 and 0xc0000000000297c8), and calls assfail at 0xc00000000003a2b4 with a2=0x6a on a path the facts do not give a condition for. The name suggests sizing installed memory slot by slot; the stored fields and their page-size meaning are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `node_getfirstfree` in `6.5.22/unix.IP27` at 0xc000000000029c9c
  **Inferred.** Passes its four argument registers unchanged (a0 to a3 at entry) to get_freemem_start at 0xc00000000004fd3c. The facts give the return value as unnamed (v0 = ?), not as the callee's result, so what it returns is not established. The name suggests a node's first free memory address, which the callee supplies in some form.
  Reviewed: 2026-11-12 (sonnet)

- `node_getmaxclick` in `6.5.22/unix.IP27` at 0xc000000000029cc4
  **Inferred.** Calls slot_getsize at 0xc000000000029d50 with a0 at entry in a loop over an index, skips a zero size (beq at 0xc000000000029ce8), and steps the index down while it stays non-negative (bgez at 0xc000000000029d2c). On a nonzero size it reads a halfword from table 0xc0000000014d3800 (lh at 0xc000000000029cf8). It returns 0x0 or a value the facts do not name. The name suggests the highest populated memory click; the loop shape fits that reading.
  Reviewed: 2026-11-12 (sonnet)

- `node_getlastslot` in `6.5.22/unix.IP27` at 0xc000000000029d3c
  **Inferred.** Returns the halfword stored at table 0xc0000000014d41b0 (lh at 0xc000000000029d4c) with no branches or calls. The name suggests a node's last slot number, so the routine is a plain read of that table entry.
  Reviewed: 2026-11-12 (sonnet)

- `slot_getsize` in `6.5.22/unix.IP27` at 0xc000000000029d50
  **Inferred.** Returns the halfword at table 0xc0000000014d4230 (lh at 0xc000000000029d6c) with no branches or calls. The facts show a fixed address and do not use a0, although node_getmaxclick passes a slot index to it. The name suggests a per-slot size, so the routine reads one size entry whatever index the caller intended.
  Reviewed: 2026-11-12 (sonnet)

- `memory_present` in `6.5.22/unix.IP27` at 0xc000000000029eac
  **Inferred.** Calls ckphyspnum at 0xc0000000001ae724 (the routine's only call, jal at 0xc000000000029eb8) with a1, a2 and a3 at entry as its a1 to a3 and an unnamed a0. The facts give the return value as unnamed (v0 = ?), not as the callee's result. The name suggests a check that a physical page is present in memory; the facts do not show what is returned.
  Reviewed: 2026-11-12 (sonnet)

- `flush_cache` in `6.5.22/unix.IP27` at 0xc00000000002b714
  **Inferred.** Loads a word at the fixed address 0xffffffffffffc6a4 and calls __cache_wb_inval at 0xc00000000002b1c8 with a0 set to the constant 0xa800000000000000, a1 set to that word, and a2 and a3 passed through. It returns the callee's result. The name suggests a write-back and invalidate over a range; the facts do not show what the loaded word is.
  Reviewed: 2026-11-12 (sonnet)

- `net_link_up` in `6.5.22/unix.IP27` at 0xc00000000002e2f0
  **Inferred.** Loads one doubleword from the fixed hub-side address 0x9200000001600000 (ld at 0xc00000000002e2f4) and returns with v0 unnamed. The name suggests a network link status check. The facts do not show what the register holds, so the reading comes from the name alone.
  Reviewed: 2026-11-12 (sonnet)
