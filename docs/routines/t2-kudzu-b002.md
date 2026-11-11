# Routine role reads, kudzu round 2 batch 2

These entries say what 25 routines of the IP27 kernel image (`6.5.22/unix.IP27`) do, read from the instruction facts rendered for each one. Every label is Inferred: the facts give control flow, loads, stores, callees and constants, but not the meaning of the hardware registers or table fields they touch. Where a role rests on the routine name alone, the entry says so.

- `cpu_enabled` in `6.5.22/unix.IP27` at 0xc000000000026f74
  **Inferred.** Loads the 8-byte word at table 0xc0000000014d3990 and branches on whether a0 equals -1 (the beq at 0xc000000000026f88 goes to the return at 0xc000000000026fa4). The return yields 0x0 or a value the facts do not name. The name suggests a per-CPU enabled test; the facts do not show what the table holds.

- `cnode_slice_to_cpuid` in `6.5.22/unix.IP27` at 0xc000000000026fac
  **Inferred.** Indexes a pointer table by a0 scaled by 8, reads halfwords at +0x4956 and +0x4958 of the selected record, and loops over candidates, calling cpu_enabled at 0xc000000000026ff4 for each and comparing a1 with a value from the table at 0xc0000000014d39a8. It returns -1 on one path (the return at 0xc000000000027050). The name suggests a mapping from a node-and-slice pair to a CPU id; the matching path's value is not named in the facts, so the mapping is a reading of the loop shape.

- `hub_ioerror_local` in `6.5.22/unix.IP27` at 0xc0000000000273bc
  **Inferred.** Selects on a2 against 0x0, 0x2 and 0x3 (beq at 0xc0000000000273c8, 0xc0000000000273d4, bne at 0xc0000000000273dc), then reads fields of the record in a3: a word at +0x0, halfwords at +0x6, +0xc and +0xe, and a doubleword at +0x10. Bounds checks on those fields lead to cmn_err at 0xc0000000001f8090 (level 4) on two paths and kl_panic at 0xc0000000000374f4 on two others. The routine looks like a checker for a local hub error record that reports or panics on a bad field; the facts show the checks, not what the fields mean.

- `hub_xp_error_handler` in `6.5.22/unix.IP27` at 0xc0000000000274b8
  **Inferred.** Finds a per-node record through the pointer table at [-0x3c00] indexed by a halfword from table 0xc0000000014d3600, reads a word at +0x49e0 of it, and tail-calls xtalk_error_handler at 0xc00000000006dad0 with that word and the caller's arguments, with a6=0x7. A beq at 0xc0000000000274e0 tests bits 0x11 of a doubleword at a1 before the call. The name and callee suggest forwarding a hub-side crossbar error to the generic widget error handler; the facts do not show what the tested bits mean.

- `hub_dma_enabled` in `6.5.22/unix.IP27` at 0xc000000000027534
  **Inferred.** Resolves a widget record through xwidget_info_get at 0xc00000000006dcb0, xwidget_info_id_get and xwidget_info_master_get, then looks up a name with hwgraph_info_get_LBL at 0xc000000000296160. It reads the hub register at 0x9200000001c00118 and tests it against a mask (beq at 0xc0000000000275b8), returning 0x0 or 0x1. A cmn_err call at 0xc0000000000275e8 (level 6) follows the lookups. The name suggests a DMA-enabled query for one widget; the mask's meaning is not in the facts.

- `hub_widgetdev_shutdown` in `6.5.22/unix.IP27` at 0xc0000000000275f8
  **Inferred.** Uses the same widget lookup chain as hub_dma_enabled, then reads and writes the 8-byte hub register at 0x9200000001c00120 (ld at 0xc000000000027698, sd at 0xc0000000000276a8). It reads 0x9200000001c00400, loops over an index compared with 0xf (beq at 0xc0000000000276c0), calls hubiio_crb_free at 0xc0000000000346f0, and reports errors through cmn_err at 0xc000000000027798 and 0xc0000000000277b4 (level 6). The name suggests disabling one widget's device on the hub; the facts show the register accesses, not which bits they clear.

- `hub_widgetdev_enable` in `6.5.22/unix.IP27` at 0xc0000000000277d0
  **Inferred.** Resolves the widget with the same lookup chain, then tests a bit mask built from the register at 0x9200000001c00120 (beq at 0xc000000000027858) and stores the register back with sd at 0xc00000000002786c. It returns the hwgraph lookup result or the base 0x9200000001c00000. The name suggests setting a widget device enable bit; the facts show the test and the store, not which bit is set.

- `hub_error_devenable` in `6.5.22/unix.IP27` at 0xc000000000027870
  **Inferred.** Resolves the widget record with the same lookup chain, compares a2 with 0x5 and 0x6 (beq at 0xc0000000000278c8 and 0xc0000000000278d0), reads the register at 0x9200000001c002c8, and branches on its sign after a shift by 7 (bltz at 0xc000000000027908). It stores a value to 0x9200000001c00388 (sd at 0xc000000000027930) and returns 0x0. The name suggests enabling error reporting on a widget; the meanings of the 0x5 and 0x6 cases and of the stored value are not in the facts.

- `hub_ioerror_handler` in `6.5.22/unix.IP27` at 0xc00000000002799c
  **Inferred.** Dispatches on a1 against 0x5, 0x6, 0x9 and 0xa (beq at 0xc0000000000279e4 through 0xc000000000027a00) to per-error branches. Those branches call hub_xp_error_handler at 0xc0000000000274b8 and hub_ioerror_local at 0xc0000000000273bc, and use is_widget_pio_enabled at 0xc000000000027934 to decide whether the PIO path applies. It writes fields of the record in a3 (sd at +0x18, sw at +0x0, sh at +0x6) and calls ioerror_dump at 0xc00000000004c050 before returning 0x4, 0x5 or 0x0. The name suggests the top-level IO error handler for a hub; the case meanings are inferred from the dispatch shape.

- `hub_merge_clean` in `6.5.22/unix.IP27` at 0xc000000000028a90
  **Inferred.** Loops while a1 is nonzero, walking entries from the address in a0, and tests the low 15 bits of a loaded word (beq at 0xc000000000028ae0 on t3 & 0x7fff). It makes no calls and returns an unnamed value. The name suggests clearing merged error state, but the rendered facts show only the loop and the mask, so the purpose is a reading from the name.

- `hub_init_ifdr_war` in `6.5.22/unix.IP27` at 0xc000000000028b1c
  **Inferred.** Indexes the pointer table at [-0x3c00] with a0 scaled by 8 and writes three fields of that per-node record: halfwords at +0x4c82 and +0x4c84 and a word at +0x4c7c, the first copied from table 0xc0000000014d3800. A bounds check (bgeu at 0xc000000000028ba4) on a2 against a value at +19588 of the record guards a cmn_err call at 0xc000000000028be8 (level 5). The name suggests initializing a per-node value for a hardware workaround; the facts show the bounds check, not the hardware meaning.

- `hub_ifdr_check_value` in `6.5.22/unix.IP27` at 0xc000000000028c18
  **Inferred.** Reads the halfword at +0x4c84 of the per-node record selected by a0 and compares it, unsigned, with a value at +19588 of the same record (beq at 0xc000000000028c70). It returns 0x1 when the stored halfword is below that bound and 0x0 otherwise (returns at 0xc000000000028c78 and 0xc000000000028c80), and it returns 0x0 early when the word at 0xc0000000014c8f8c is zero (beq at 0xc000000000028c1c). The name suggests a range check on a stored workaround value; the facts show the comparison only.

- `hub_ifdr_fix_value` in `6.5.22/unix.IP27` at 0xc000000000028c88
  **Inferred.** Reads and rewrites the halfword at +0x4c82 of the per-node record selected by a0 (lhu at 0xc000000000028cd8, sh at 0xc000000000028cec), compares an unnamed register with 0x41 unsigned (beq at 0xc000000000028cb4), and runs a branch-likely loop that decrements v0 (bnel at 0xc000000000028d18). It returns -1 or 1. The name suggests adjusting a stored value into a valid range; the loop and masks do not show what the field means.

- `hub_setup_kick` in `6.5.22/unix.IP27` at 0xc000000000028d74
  **Inferred.** Writes two words in the per-node record selected by a0 (sw at +0x4b6c and +0x4b68) and calls spinlock_init at 0xc000000000286fe8 on three locks: one at 0xc0000000014d13e0, one at 0xc0000000014d13e4, and one inside the per-node record at +19312. The first block runs only when a0 is nonzero (beq at 0xc000000000028d84). The name suggests setting up the state that a timeout kick routine uses; the facts show the lock inits and the stores.

- `hub_dmatimeout_kick` in `6.5.22/unix.IP27` at 0xc000000000028e90
  **Inferred.** Takes the lock at 0xc0000000014d13e0 with mutex_spinlock at 0xc000000000287214, retries with mutex_spintrylock at 0xc00000000028748c, and releases it with mutex_spinunlock at 0xc000000000287260. In a loop bounded by the count at 0xc0000000014d13a0, it compares a hub counter at 0x9200000001030100 with the same counter plus 0x1c9c380 (30,000,000) and stores its two saved words at 0xc0000000014d41a0 and 0xc0000000014d41a8. It calls cpuaction, bte_wait_for_xfer_completion, us_delay (with 0x5, then 0x2 twice), hub_merge_clean at 0xc000000000028a90 and hub_ifdr_fix_value at 0xc000000000028c88. The name suggests a DMA-timeout recovery kick; the sequence is clear, but the counter's meaning is not in the facts.

- `hub_widget_reset` in `6.5.22/unix.IP27` at 0xc0000000000293ac
  **Inferred.** Calls master_node_get at 0xc000000000297a94, raises the interrupt level with spl1 at 0xc00000000003affc and restores it with splx, and calls hub_setup_prb at 0xc00000000003c028 with a2=0x3 and a3=0x2. When the master lookup returns -1 (beq at 0xc0000000000293e0) it takes a separate path. Two assfail calls at 0xc00000000003a2b4 with the constant 0x562 mark states the code treats as impossible. The name suggests resetting one widget's hub-side state; the stored fields are not named in the facts.

- `mem_init` in `6.5.22/unix.IP27` at 0xc000000000029704
  **Inferred.** Calls init_mutex at 0xc00000000018dc14 with a0 set to the lock at 0xc0000000014ddbd8, a1=0, a2 set to a pointer at 0xc0000000014066d8 and a3=-1, and returns that call's result. The name suggests setting up the memory subsystem's lock at boot; this routine is one call and the lock's other users are outside it.

- `szmem` in `6.5.22/unix.IP27` at 0xc00000000002972c
  **Inferred.** Reads a slot count at 0xc0000000014d269c, loops over slot indices while s0 is below 0x20 (bne at 0xc0000000000297d4), and calls slot_psize_compute at 0xc000000000029d70 for each slot. It stores a word and two halfwords per slot (sw at 0xc0000000000297a0, sh at 0xc0000000000297b4 and 0xc0000000000297c8), and calls assfail at 0xc00000000003a2b4 with the constant 0x6a on a bad argument. The name suggests sizing installed memory slot by slot; the stored fields and their page-size meaning are not in the facts.

- `node_getfirstfree` in `6.5.22/unix.IP27` at 0xc000000000029c9c
  **Inferred.** Passes its four argument registers unchanged to get_freemem_start at 0xc00000000004fd3c and returns that call's result. The routine has no logic of its own. The name suggests a node's first free memory address, which the callee supplies.

- `node_getmaxclick` in `6.5.22/unix.IP27` at 0xc000000000029cc4
  **Inferred.** Starts from the halfword at table 0xc0000000014d3800 (lh at 0xc000000000029cf8), calls slot_getsize at 0xc000000000029d50 for each index, skips a zero size (beq at 0xc000000000029ce8), and steps the index down while it stays non-negative (bgez at 0xc000000000029d2c). It returns 0x0 or an index the facts do not name. The name suggests the highest populated memory click; the loop shape fits that reading.

- `node_getlastslot` in `6.5.22/unix.IP27` at 0xc000000000029d3c
  **Inferred.** Returns the halfword stored at table 0xc0000000014d41b0 (lh at 0xc000000000029d4c) with no branches or calls. The name suggests a node's last slot number, so the routine is a plain read of that table entry.

- `slot_getsize` in `6.5.22/unix.IP27` at 0xc000000000029d50
  **Inferred.** Returns the halfword at table 0xc0000000014d4230 (lh at 0xc000000000029d6c) with no branches or calls. The facts show a fixed address and do not use a0, although node_getmaxclick passes a slot index to it. The name suggests a per-slot size, so the routine reads one size entry whatever index the caller intended.

- `memory_present` in `6.5.22/unix.IP27` at 0xc000000000029eac
  **Inferred.** Passes a1, a2 and a3 to ckphyspnum at 0xc0000000001ae724 and returns its result, which is the routine's only call (jal at 0xc000000000029eb8). The name suggests a check that a physical page is present in memory; the callee does the work.

- `flush_cache` in `6.5.22/unix.IP27` at 0xc00000000002b714
  **Inferred.** Loads a word at the fixed address 0xffffffffffffc6a4 and calls __cache_wb_inval at 0xc00000000002b1c8 with a0 set to the constant 0xa800000000000000, a1 set to that word, and a2 and a3 passed through. It returns the callee's result. The name suggests a write-back and invalidate over a range; the loaded word is read as a cache parameter from its use, not from a documented field.

- `net_link_up` in `6.5.22/unix.IP27` at 0xc00000000002e2f0
  **Inferred.** Loads one doubleword from the fixed hub-side address 0x9200000001600000 (ld at 0xc00000000002e2f4) and returns with v0 unnamed. The name suggests a network link status check. The facts do not show what the register holds, so the reading comes from the name alone.
