# Tier-2 reading batch 1 of round r5 of kudzu

Routine role entries for batch 1 of reading round r5, from the `6.5.22/unix.IP27` build. Each role is read from the rendered facts only: instruction addresses, callees, branch conditions and constants.

- `intr_connect_level` in `6.5.22/unix.IP27` at 0xc000000000038108
  **Inferred.** Gets the interrupt pointers from intr_get_ptrs [call 1]. It takes a spin lock with mutex_spinlock_spl [call 2] and releases it with mutex_spinunlock [call 3]. It reaches intr_thread_setup [call 4] only when a flag bit 0x100 is set [@0xc000000000038310 fall] and a word is nonzero [@0xc000000000038358 fall], passing 0x200 as its fifth argument [call 4]. The return at 0xc0000000000381a0 gives -0x1 or 0x0 [@0xc0000000000381a0].
  Role (from facts): sets up an interrupt level under the spin lock, and hands the thread setup to intr_thread_setup [call 4, @0xc000000000038310 fall].
  Reviewed: 2026-11-12

- `intr_target_from_desc` in `6.5.22/unix.IP27` at 0xc000000000038934
  **Inferred.** Asks device_desc_intr_target_get for a target [call 1]. It turns that result into a CPU with cpuvertex_to_cpuid [call 2, @0xc000000000038954 fall]. It also goes through subnodevertex_to_subnode [call 3, @0xc000000000038970 taken] and master_node_get [call 4]. It chooses a CPU on a node with intr_cpu_choose_from_node [call 5] and [call 6]. When the target equals -0x1 [@0xc000000000038954 taken] it skips to 0xc000000000038978.
  Role (from facts): maps an interrupt descriptor to a CPU id: it asks for a target vertex [call 1], converts it with cpuvertex_to_cpuid [call 2, @0xc000000000038954 fall], and chooses a CPU on a node [call 5] [call 6] found from that target [call 3] [call 4].
  Reviewed: 2026-11-12

- `intr_cnode_seen` in `6.5.22/unix.IP27` at 0xc0000000000389ec
  **Inferred.** Reads the word at a2 [@0xc0000000000389ec]. When that word is at most zero [@0xc0000000000389f4 taken] it returns a1 [@0xc000000000038a1c]. When the value compared at 0xc000000000038a0c equals a0 [@0xc000000000038a0c taken] it returns 0x0 [@0xc000000000038a24].
  Role (from facts): returns a1 or 0x0 according to a word read through a2 [@0xc0000000000389ec] and a compare with a0 [@0xc000000000038a0c taken].
  Reviewed: 2026-11-12

- `intr_heuristic` in `6.5.22/unix.IP27` at 0xc000000000038ad4
  **Inferred.** Gets a radius with physmem_maxradius [call 1]. When s5 is nonzero [@0xc000000000038b20 fall nonzero] it calls intr_target_from_desc [call 2, @0xc000000000038b20 fall] and then intr_bit_reserve_test [call 3, @0xc000000000038b20 fall] on that result. When a1 equals -0x1 [@0xc000000000038b60 taken] it goes to the code that calls master_node_get [call 4] and intr_bit_reserve_test [call 5] on the node.
  Role (from facts): picks an interrupt target by trying the descriptor target first [call 2] and the master node next [call 4].
  Reviewed: 2026-11-12

- `hub_piomap_free` in `6.5.22/unix.IP27` at 0xc00000000003b5ac
  **Inferred.** Calls hwgraph_info_get_LBL [call 1, @0xc00000000003b5c0 fall]. It calls mutex_spinlock [call 2, @0xc00000000003b5c0 fall] and mutex_spinunlock [call 3, @0xc00000000003b5c0 fall] around the field updates. It stores 2 bytes to a0 +0x24 [@0xc00000000003b600] and 4 bytes to a0 +0x28 [@0xc00000000003b65c].
  Role (from facts): updates the PIO map record a0 under the spin lock [call 2] [call 3].
  Reviewed: 2026-11-12

- `hub_dmamap_free` in `6.5.22/unix.IP27` at 0xc00000000003b848
  **Inferred.** Reads the word at a0 +0xc [@0xc00000000003b84c]. It stores 4 bytes back to a0 +0xc [@0xc00000000003b860] in the delay slot of the call to kern_free [call 1], so the store runs before the call, and passes a0 = a0 at entry (arg0) to kern_free [call 1]. It returns the result of kern_free [@0xc00000000003b868].
  Role (from name): frees a DMA map record. The facts show a0 passed to kern_free [call 1].
  Reviewed: 2026-11-12

- `hub_intr_free` in `6.5.22/unix.IP27` at 0xc00000000003bce4
  **Inferred.** It calls hub_intr_disconnect [call 1, @0xc00000000003bd08 fall] only when bit 0x4 of the word at a0 +0x2c is set. It calls kmem_free [call 2, @0xc00000000003bd30 fall] with a1 = 0x30 only when bit 0x1 of that word is set. It then calls intr_unreserve_level [call 3] on every path and returns its result [@0xc00000000003bd54].
  Role (from facts): tears down an interrupt connection: it disconnects [call 1, @0xc00000000003bd08 fall], frees the block [call 2, @0xc00000000003bd30 fall], and releases the level [call 3].
  Reviewed: 2026-11-12

- `hub_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000003bd5c
  **Inferred.** Calls intr_connect_level [call 1]. When the result is negative [@0xc00000000003bd9c taken] it returns that result. Otherwise it makes an indirect call [call 2] when a3 is nonzero [call 2, @0xc00000000003bda8 taken nonzero] and returns zero. It returns the result of intr_connect_level or zero [@0xc00000000003bdcc].
  Role (from facts): connects an interrupt through intr_connect_level [call 1] and returns that result or zero.
  Reviewed: 2026-11-12

- `hub_intr_disconnect` in `6.5.22/unix.IP27` at 0xc00000000003bde4
  **Inferred.** When the doubleword at a0 +0x18 is nonzero [call 2, @0xc00000000003be04 taken nonzero] it first makes an indirect call [call 2]. It then calls intr_disconnect_level [call 1]. It returns -0x5 [@0xc00000000003be30].
  Role (from facts): removes an interrupt connection through intr_disconnect_level [call 1], after an indirect call [call 2, @0xc00000000003be04 taken nonzero] when a0 +0x18 is set.
  Reviewed: 2026-11-12

- `hub_setup_prb` in `6.5.22/unix.IP27` at 0xc00000000003c028
  **Inferred.** Reads the doubleword at table(0x9200000001c00198) [@0xc00000000003c078] and stores 8 bytes to the same place [@0xc00000000003c0b8]. It tests the word at 0xc0000000014c9c9c [@0xc00000000003c030 taken] and the register a5 [@0xc00000000003c054 taken].
  Role (from name): sets up the hub PRB state. The facts show no calls and one doubleword read and written back.
  Reviewed: 2026-11-12

- `hub_device_inquiry` in `6.5.22/unix.IP27` at 0xc00000000003c290
  **Inferred.** Looks up a hwgraph path with hwgraph_traverse [call 2]. When the lookup returns zero [@0xc00000000003c2c8 taken zero] it calls device_master_get [call 3]. It reads the label of the master with hwgraph_info_get_LBL [call 4, @0xc00000000003c2e8 fall]. Its messages go through cmn_err [call 5].
  Role (from facts): finds a device master vertex by path [call 2] [call 3] and reports the device through cmn_err [call 5].
  Reviewed: 2026-11-12

- `hub_slotbits_to_slot` in `6.5.22/unix.IP27` at 0xc00000000003c7ec
  **Inferred.** Reads one byte at abs 0xffffffffffffc523 [@0xc00000000003c7ec]. When it is nonzero [@0xc00000000003c7f4 fall] the return at 0xc00000000003c808 gives a computed value [@0xc00000000003c808]. Otherwise it takes the low three bits of a0 into a3 and, when that value is at least 8 [@0xc00000000003c824 taken], returns 0xf0 [@0xc00000000003c808]. In the remaining case it reads a word at 0x90000000100000a8 [@0xc00000000003c82c] and returns a byte from one of two tables chosen by bit 0x1 of that word [@0xc00000000003c834]: entry(0xc0000000014164e3) [@0xc00000000003c840] or entry(0xc0000000014164c3) [@0xc00000000003c850].
  Role (from facts): maps slot bits in a0 to a slot value: a computed value when the byte at abs 0xffffffffffffc523 is nonzero [@0xc00000000003c7f4 fall], otherwise a byte read from one of two tables [@0xc00000000003c844] [@0xc00000000003c854].
  Reviewed: 2026-11-12

- `hub_slot_to_widget` in `6.5.22/unix.IP27` at 0xc00000000003c858
  **Inferred.** Reads the word at abs 0x9000000010000090 [@0xc00000000003c870]. When the low nibble of a0 (a4) is at least 8 [@0xc00000000003c86c taken] it goes to the return at 0xc00000000003c894 [@0xc00000000003c894]. When the high nibble of a0 equals 0x50 [@0xc00000000003c880 taken] it reads a byte from a third table [@0xc00000000003c8bc]. Otherwise it picks between two tables by whether the word it read is nonzero [@0xc00000000003c888]: a byte at [@0xc00000000003c890] when nonzero or at [@0xc00000000003c8a8] when zero.
  Role (from facts): maps a slot number in a0 to a widget number through byte tables [@0xc00000000003c890] [@0xc00000000003c8a8] [@0xc00000000003c8bc], with a separate table when the high nibble is 0x50 [@0xc00000000003c880 taken].
  Reviewed: 2026-11-12

- `klhwg_invent_alloc` in `6.5.22/unix.IP27` at 0xc0000000000404d0
  **Inferred.** Calls kern_malloc [call 1]. When the result is nonzero [@0xc0000000000404e8 taken nonzero] it stores 2 bytes [@0xc000000000040514] and 4 bytes [@0xc000000000040530] into the block. The return at 0xc00000000004053c gives 0x0 or the result of kern_malloc [@0xc00000000004053c].
  Role (from facts): allocates a record with kern_malloc [call 1] and returns it, or zero when the allocation fails [@0xc0000000000404e8 fall zero].
  Reviewed: 2026-11-12

- `klhwg_cpu_invent_info` in `6.5.22/unix.IP27` at 0xc000000000040544
  **Inferred.** Calls klhwg_invent_alloc [call 1] with a2 = 0x24. When the allocation is zero [@0xc000000000040578 taken zero] it goes to 0xc000000000040658. It loads a2 +0x38 [@0xc000000000040594] and stores into the new record [@0xc000000000040598]. It registers the record with hwgraph_info_add_LBL [call 3, @0xc000000000040578 fall] and hwgraph_info_export_LBL [call 4, @0xc000000000040578 fall].
  Role (from facts): builds a CPU inventory record from the argument record at a2 [@0xc000000000040594] and registers it [call 3] [call 4].
  Reviewed: 2026-11-12

- `klhwg_disabled_cpu_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000406c4
  **Inferred.** Calls klhwg_invent_alloc [call 1] with a2 = 0x24. Calls klhwg_invent_alloc [call 2, @0xc0000000000406fc fall] with a2 = 0x5c. When the first allocation is zero [@0xc0000000000406fc taken zero] it goes to 0xc0000000000407e4. It formats a string with get_diag_string [call 3, @0xc000000000040714 fall] and sprintf [call 4, @0xc000000000040714 fall]. It registers records with hwgraph_info_add_LBL [call 5, @0xc000000000040714 fall] and hwgraph_info_add_LBL [call 7, @0xc000000000040714 fall].
  Role (from facts): builds two records for a disabled CPU [call 1] [call 2], formats a diagnostic string [call 3] [call 4], and registers the records [call 5] [call 7].
  Reviewed: 2026-11-12

- `klhwg_mem_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000409b4
  **Inferred.** Calls klhwg_invent_alloc [call 1] with a2 = 0x50. When the allocation is zero [@0xc000000000040a04 taken zero] it goes to 0xc000000000040bf8. It calls ip27log_getenv [call 2, @0xc000000000040a04 fall]. It calls ip27log_getenv [call 3, @0xc000000000040a40 fall]. It passes a string to strchr [call 4, @0xc000000000040adc fall]. It registers the record with hwgraph_info_add_LBL [call 5, @0xc000000000040a04 fall] with a3 = 0x100. It calls hwgraph_info_export_LBL [call 6, @0xc000000000040a04 fall] with a2 = 0x50.
  Role (from facts): builds the memory inventory record [call 1] and registers it [call 5] [call 6].
  Reviewed: 2026-11-12

- `klhwg_add_rps` in `6.5.22/unix.IP27` at 0xc000000000040fb4
  **Inferred.** Calls hwgraph_path_add [call 1] with a1 = 0xc00000000140cd20. It calls device_master_set [call 2, @0xc000000000040fe4 taken] and klhwg_invent_alloc [call 3, @0xc000000000040fe4 taken] with a1 = 0x17 and a2 = 0x10. It registers the record with hwgraph_info_add_LBL [call 4, @0xc000000000040fe4 taken] and hwgraph_info_export_LBL [call 5, @0xc000000000040fe4 taken].
  Role (from facts): adds a record at a path in the hardware graph and registers its label information [call 4] [call 5].
  Reviewed: 2026-11-12

- `klhwg_add_xbox_rps` in `6.5.22/unix.IP27` at 0xc0000000000410ec
  **Inferred.** Calls hwgraph_path_add [call 1] with a1 = 0xc00000000140cd28. It calls device_master_set [call 2, @0xc00000000004111c taken] and klhwg_invent_alloc [call 3, @0xc00000000004111c taken] with a1 = 0x17 and a2 = 0x10. It registers the record with hwgraph_info_add_LBL [call 4, @0xc00000000004111c taken] and hwgraph_info_export_LBL [call 5, @0xc00000000004111c taken].
  Role (from facts): adds a record at a path in the hardware graph and registers its label information [call 4] [call 5].
  Reviewed: 2026-11-12

- `klhwg_update_xbox_rps` in `6.5.22/unix.IP27` at 0xc000000000041198
  **Inferred.** When a0 is -0x1 [@0xc0000000000411a8 taken] it skips to 0xc0000000000411e0. Otherwise it calls hwgraph_edge_get [call 1, @0xc0000000000411a8 fall] with a1 = 0xc00000000140cd28 and, when that returns zero [@0xc0000000000411d8 taken], hwgraph_info_get_LBL [call 2]. It then stores a single byte through the record that call 2 returned [@0xc00000000004121c] when a1 at entry is zero [@0xc000000000041208 taken], and [@0xc000000000041214] when it is nonzero [@0xc000000000041208 fall].
  Role (from facts): looks up a graph edge [call 1] and a label [call 2], and stores a flag byte through the label's record, set by a1 [@0xc000000000041214] [@0xc00000000004121c].
  Reviewed: 2026-11-12

- `klhwg_device_disable_hints_add` in `6.5.22/unix.IP27` at 0xc000000000042268
  **Inferred.** Calls device_admin_table_init [call 1] with its four arguments. It proceeds only when the count word at 0xc0000000014d269c is positive [@0xc000000000042298 fall]. It calls device_component_canonical_name_get [call 2, @0xc0000000000422d4 fall] and device_admin_table_update [call 3, @0xc0000000000422d4 fall].
  Role (from facts): initializes a device admin table [call 1] and adds canonical component names to it [call 2] [call 3].
  Reviewed: 2026-11-12

- `klhwg_add_all_modules` in `6.5.22/unix.IP27` at 0xc00000000004283c
  **Inferred.** It proceeds only when the count word at 0xc0000000014d1500 is positive [@0xc000000000042864 fall]. In a loop over the modules [@0xc000000000042904 taken] it calls sprintf [call 1, @0xc000000000042904 taken] and hwgraph_path_add [call 2, @0xc000000000042904 taken], then hwgraph_fastinfo_set [call 3, @0xc000000000042904 taken], then sprintf [call 4, @0xc000000000042904 taken] and hwgraph_path_add [call 5, @0xc000000000042904 taken]. When call 5 returns zero [@0xc0000000000428cc fall] it calls hwgraph_info_add_LBL [call 6] and sndrv_attach [call 7]; when it returns nonzero [@0xc0000000000428cc taken] it calls assfail [call 8] first.
  Role (from facts): adds hwgraph paths for each module [call 2] [call 5] and attaches a driver to each with sndrv_attach [call 7].
  Reviewed: 2026-11-12

- `klhwg_add_all_nodes` in `6.5.22/unix.IP27` at 0xc000000000042940
  **Inferred.** Calls get_nasid [call 1] and klhwg_add_node [call 2, @0xc000000000042978 fall]. It repeats the node loop while the counter is below the count at 0xc0000000014d269c [@0xc0000000000429a4 taken]. It calls klhwg_add_all_xbows [call 3, @0xc0000000000429ac fall] in a second loop with the same bound [@0xc0000000000429d4 taken]. After the loops it calls klhwg_add_all_routers [call 4], klhwg_connect_routers [call 5], klhwg_connect_hubs [call 6], router_guardians_set [call 7] and klhwg_device_disable_hints_add [call 8].
  Role (from facts): builds the hardware graph: it adds each node [call 2] and its xbows [call 3] in loops over the node count, then adds routers [call 4], connects them [call 5] [call 6], sets the guardians [call 7] and adds device disable hints [call 8].
  Reviewed: 2026-11-12

- `hub_xtalk_link_up` in `6.5.22/unix.IP27` at 0xc000000000042a18
  **Inferred.** Loads a doubleword at an address formed from a0 [@0xc000000000042a24]. It tests bit 0x2000 of that doubleword [@0xc000000000042a2c]. The return at 0xc000000000042a38 gives 0 or 1 [@0xc000000000042a38].
  Role (from facts): returns 0 or 1 according to bit 0x2000 of a doubleword addressed by a0 [@0xc000000000042a2c].
  Reviewed: 2026-11-12

- `hub_rtc_init` in `6.5.22/unix.IP27` at 0xc000000000047b68
  **Inferred.** Calls get_compact_nodeid [call 1]. When its result equals a0 [call 1, @0xc000000000047b7c fall] it stores four doublewords into a per-node block [@0xc000000000047ba4] [@0xc000000000047bb0]. It loads a halfword from table(0xc0000000014d3800) [@0xc000000000047b90]. The return at 0xc000000000047bb8 gives the result of get_compact_nodeid or 1 [@0xc000000000047bb8].
  Role (from name): initialises the hub RTC block for the node. The facts show the four doubleword stores, gated on the node id match [@0xc000000000047b7c fall].
  Reviewed: 2026-11-12
