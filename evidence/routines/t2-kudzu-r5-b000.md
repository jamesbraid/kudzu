# Routine role entries, round r5 batch 0

These entries state what each routine does in the 6.5.22 IP27 kernel image. They describe software behavior, not hardware behavior.

- `router_nic_access` in `6.5.22/unix.IP27` at 0xc00000000001b174
  **Inferred.** It calls router_reg_write with 0x38 as its second argument [call 2] and stores to +0x8 of a0 [@0xc00000000001b1b8]. It calls router_reg_read [call 3] and loads +0x8 of a0 [@0xc00000000001b1dc]. It calls router_reg_read a second time [call 4, @0xc00000000001b1e4 fall]. It calls us_delay [call 5] once the polling loop is left, which happens when a read is negative [@0xc00000000001b1d4 taken] or bit 0x2 of the word at +0x8 is set [@0xc00000000001b1e4 taken].
  Role (from facts): it sends a phase word to a register [call 2] and then polls that register [call 3, call 4], going back while a read is nonnegative [@0xc00000000001b1fc taken], until a read is negative [@0xc00000000001b1d4 taken] or bit 0x2 of the word at +0x8 is set [@0xc00000000001b1e4 taken], and then calls us_delay [call 5].
  Reviewed: 2026-11-12
- `router_set_leds` in `6.5.22/unix.IP27` at 0xc00000000001b224
  **Inferred.** It loads the byte at +0x6 of a0 [@0xc00000000001b228] and branches to the load at 0xc00000000001b248 when that byte differs from the byte at +0x7 [@0xc00000000001b230 taken]. It also stores to +0x6 of a0 [@0xc00000000001b260] and calls router_reg_write with 0 as its second argument [call 1, @0xc00000000001b230 taken].
  Role (from facts): it keeps the byte at +0x6 in step with the byte at +0x7, and on the differs path it calls router_reg_write with a zero second argument [call 1, @0xc00000000001b230 taken].
  Reviewed: 2026-11-12
- `router_acquire_info` in `6.5.22/unix.IP27` at 0xc00000000001b4d4
  **Inferred.** It takes the lock at a0+404 [call 1]. For each counter value whose bit is set in the byte at +0x8 of a0 [@0xc00000000001b548 fall] it releases the lock [call 2], calls router_acquire_port_info [call 3] and takes the lock again [call 4]. It then calls router_set_leds [call 5] and releases the lock [call 6]. When bit 0x1 of the word at +0x160 of a0 is set [@0xc00000000001b640 fall] it calls router_reg_write with 0x18 as its second argument [call 7]. It calls router_reg_write with 0x18 as its second argument again [call 8, @0xc00000000001b5d4 taken], before the release [call 6].
  Role (from facts): it updates port information under the lock at a0+404 [call 1, call 2, call 3, call 4, call 6], and on those conditional paths brackets the update with router_reg_write calls that use 0x18 as the second argument [call 7, call 8].
  Reviewed: 2026-11-12
- `router_check_errors` in `6.5.22/unix.IP27` at 0xc00000000001b7e8
  **Inferred.** It calls router_check_port [call 1] inside a loop whose back edge is [@0xc00000000001b81c taken]. The loop repeats while the counter is below 7 (signed) [@0xc00000000001b81c taken], and the routine returns at [@0xc00000000001b838].
  Role (from facts): it applies router_check_port [call 1] to each counter value below 7 [@0xc00000000001b81c taken].
  Reviewed: 2026-11-12
- `router_port_init` in `6.5.22/unix.IP27` at 0xc00000000001b840
  **Inferred.** It calls router_reg_read [call 1] and then calls router_reg_write [call 2]. It calls router_reg_write again [call 3] and router_reg_read [call 4]. It loads the halfword at +0x158 of a0 [@0xc00000000001b89c] and branches ahead when that halfword is at least 0x50 [@0xc00000000001b8a8 taken].
  Role (from facts): it sets up one port's registers, calling router_reg_read [call 1] and router_reg_write [call 2, call 3], and it tests the halfword at +0x158 [@0xc00000000001b8a8 taken].
  Reviewed: 2026-11-12
- `router_set_fences` in `6.5.22/unix.IP27` at 0xc00000000001baec
  **Inferred.** It calls router_reg_read with 0x10 as its second argument [call 1]. The outer loop exits when the counter reaches 7 (signed) [@0xc00000000001bb3c taken]. For each counter value below 7 [@0xc00000000001bb3c fall] whose bit is set in the byte at +0x8 of a0 [@0xc00000000001bb54 fall] it calls router_reg_read again [call 2]. A counter value whose bit is clear [@0xc00000000001bb54 taken] goes back to the loop head. It stores to +0x170 of a0 [@0xc00000000001bbd0] and calls router_reg_write with 0x10 as its second argument [call 4].
  Role (from facts): it reads a register word once [call 1], reads one more for each counter value below 7 that has its bit set [call 2], and ends with a router_reg_write call [call 4].
  Reviewed: 2026-11-12
- `router_state_empty` in `6.5.22/unix.IP27` at 0xc00000000001c40c
  **Inferred.** It calls router_reg_read [call 1] and loads the byte at +0x8 of a0 [@0xc00000000001c460]. For each counter value whose bit is set in that byte [@0xc00000000001c4c8 fall] it calls router_reg_read [call 2]. It repeats the loop while the counter is below 7 (signed) [@0xc00000000001c4fc taken].
  Role (from facts): it reads one register word [call 1] and then one word for each counter value whose bit is set in the byte at +0x8 of a0 [call 2], and returns one of 0x0 or 0x1 [@0xc00000000001c524].
  Reviewed: 2026-11-12
- `router_print_state_specific` in `6.5.22/unix.IP27` at 0xc00000000001c52c
  **Inferred.** It takes the lock at a0+404 [call 1] and loads the byte at +0x190 of a0 [@0xc00000000001c564]. When the byte masked with a3 is nonzero [@0xc00000000001c56c fall] it releases the lock [call 2] and returns. Otherwise [@0xc00000000001c56c taken] it stores a byte to +0x190 of a0 [@0xc00000000001c5a4] and releases the lock [call 3]. It calls router_state_empty [call 7] and, when that returns zero [@0xc00000000001c62c fall], it calls router_reg_read [call 11] and show_rr_status_rev_id [call 12]. In a loop that repeats while the counter is below 7 (signed) [@0xc00000000001c6f8 taken] it calls show_rr_error_status [call 14].
  Role (from facts): it takes the lock [call 1], marks the byte at +0x190 [@0xc00000000001c5a4] and releases the lock [call 3], and when router_state_empty returns zero [@0xc00000000001c62c fall] it calls show_rr_status_rev_id [call 12] and then show_rr_error_status [call 14] in a loop.
  Reviewed: 2026-11-12
- `router_nic_probe` in `6.5.22/unix.IP27` at 0xc00000000001c7d4
  **Inferred.** It calls nic_get_phase_bits [call 1] and then calls vector_write with 0x38 as its third argument [call 2]. It calls vector_read [call 3] and again [call 4, @0xc00000000001c848 fall]. The polling loop goes back while a read is nonnegative [@0xc00000000001c860 taken]. It calls us_delay [call 5] once the loop is left, which happens when a read is negative [@0xc00000000001c838 taken] or bit 0x2 is set [@0xc00000000001c848 taken].
  Role (from facts): it does the same write, poll and delay sequence as router_nic_access, using vector_write [call 2] and vector_read [call 3, call 4].
  Reviewed: 2026-11-12
- `hub_vector_read` in `6.5.22/unix.IP27` at 0xc00000000001d25c
  **Inferred.** It loads a pointer from a fixed address [@0xc00000000001d27c] and indexes a table with a0 [@0xc00000000001d28c]. It calls in_panic_mode [call 1]. When that returns zero [@0xc00000000001d2b0 taken] it takes the lock [call 5]. It calls vector_read_node with a1 at entry as its first argument [call 3, @0xc00000000001d2bc taken], which it reaches only when a halfword at a fixed address equals a0 [@0xc00000000001d2bc taken]. It then calls in_panic_mode again [call 2] and, when that returns zero [@0xc00000000001d2cc taken], releases the lock [call 4]. It stores bytes at the indexed entry [@0xc00000000001d2f0, @0xc00000000001d314].
  Role (from facts): it passes a vector word for the node at index a0 through vector_read_node [call 3, @0xc00000000001d2bc taken], and only when that node matches a halfword at a fixed address; it takes the lock when panic mode is off [call 5, @0xc00000000001d2b0 taken].
  Reviewed: 2026-11-12
- `hub_vector_write` in `6.5.22/unix.IP27` at 0xc00000000001d33c
  **Inferred.** It loads a pointer from a fixed address [@0xc00000000001d35c] and indexes a table with a0 [@0xc00000000001d36c]. It calls in_panic_mode [call 1]. When that returns zero [@0xc00000000001d390 taken] it takes the lock [call 5]. It calls vector_write_node with a1 at entry as its first argument [call 3, @0xc00000000001d39c taken], which it reaches only when a halfword at a fixed address equals a0 [@0xc00000000001d39c taken]. It then calls in_panic_mode again [call 2] and, when that returns zero [@0xc00000000001d3ac taken], releases the lock [call 4]. It stores bytes at the indexed entry [@0xc00000000001d3d4, @0xc00000000001d3f8].
  Role (from facts): it passes a vector word for the node at index a0 through vector_write_node [call 3, @0xc00000000001d39c taken], and only when that node matches a halfword at a fixed address; it takes the lock when panic mode is off [call 5, @0xc00000000001d390 taken].
  Reviewed: 2026-11-12
- `hub_widget_id` in `6.5.22/unix.IP27` at 0xc00000000001d41c
  **Inferred.** It loads a doubleword from an address computed from a0 [@0xc00000000001d428]. It returns the low four bits of that doubleword [@0xc00000000001d42c].
  Role (from facts): it reads one doubleword at an address computed from a0 [@0xc00000000001d428] and returns its low four bits [@0xc00000000001d42c].
  Reviewed: 2026-11-12
- `get_nasid` in `6.5.22/unix.IP27` at 0xc00000000001d434
  **Inferred.** It loads a doubleword from the fixed address 0x9200000001600000 [@0xc00000000001d438]. It returns a 9-bit field of it, taken from bit 8 upward and sign-extended from 16 bits [@0xc00000000001d448].
  Role (from facts): it reads one doubleword from a fixed address [@0xc00000000001d438] and returns a 9-bit field of it [@0xc00000000001d448].
  Reviewed: 2026-11-12
- `is_master_baseio` in `6.5.22/unix.IP27` at 0xc00000000001dcb0
  **Inferred.** It loads a doubleword through a0 [@0xc00000000001dcd0] and calls find_lboard_modslot [call 1]. When the first search returns zero [@0xc00000000001dd30 fall zero], it loads a halfword from a per-node structure [@0xc00000000001dd50] and, unless that halfword is -1 [@0xc00000000001dd60 fall], calls find_lboard_modslot a second time [call 2, @0xc00000000001dd60 fall]. It loads the byte at +0x9 of the search result [@0xc00000000001ddb8].
  Role (from facts): it looks up a board entry with find_lboard_modslot [call 1], searches again [call 2, @0xc00000000001dd60 fall] when the first search finds nothing [@0xc00000000001dd30 fall zero] and a per-node halfword is not -1, and reads a byte of the result [@0xc00000000001ddb8].
  Reviewed: 2026-11-12
- `get_cpu_cnode` in `6.5.22/unix.IP27` at 0xc00000000001e0d8
  **Inferred.** It indexes the table at 0xc0000000014d3880 with a0 and loads a halfword [@0xc00000000001e0e8]. It returns [@0xc00000000001e0e4].
  Role (from facts): it returns the halfword at index a0 of a fixed table [@0xc00000000001e0e8].
  Reviewed: 2026-11-12
- `get_actual_nasid` in `6.5.22/unix.IP27` at 0xc00000000001e210
  **Inferred.** When a0 is zero [@0xc00000000001e220 fall] it reaches the return [@0xc00000000001e230] with -1 in v0. Otherwise it calls find_first_component with 0x2 as its second argument [call 1, @0xc00000000001e220 taken nonzero]. When that returns zero [@0xc00000000001e244 taken] it also reaches that return with -1 in v0. It reads the byte at +0x2 of the component it finds [@0xc00000000001e24c]. When bit 0x1 of that byte is set [@0xc00000000001e254 fall] it reads the halfword at +0x20 of a0 [@0xc00000000001e260]; when it is clear [@0xc00000000001e254 taken] it reads the byte at +0x18 of the component [@0xc00000000001e270].
  Role (from facts): it gives -1 for a zero a0 or when no component is found, and otherwise returns either the halfword at +0x20 of a0 [@0xc00000000001e260] or the byte at +0x18 of the component [@0xc00000000001e270], chosen by bit 0x1 of the byte at +0x2 [@0xc00000000001e254].
  Reviewed: 2026-11-12
- `nasid_slice_to_cpuinfo` in `6.5.22/unix.IP27` at 0xc00000000001e274
  **Inferred.** It calls find_lboard with 0x11 as its second argument [call 1]. When that returns nonzero [@0xc00000000001e2d0 taken nonzero] it calls find_first_component on the result with 0x1 as its second argument [call 2]. It calls find_component [call 3] in a loop that repeats while the byte at +0x18 of the component differs from a1 at entry [@0xc00000000001e324 taken]. It reads that byte [@0xc00000000001e30c, @0xc00000000001e320].
  Role (from facts): it finds a board [call 1] and walks its components with find_first_component [call 2] and find_component [call 3] until the byte at +0x18 equals a1 at entry [@0xc00000000001e324 fall].
  Reviewed: 2026-11-12
- `compact_to_nasid_nodeid` in `6.5.22/unix.IP27` at 0xc00000000001f518
  **Inferred.** It indexes the table at 0xc0000000014d3800 with a0 and loads a halfword [@0xc00000000001f52c]. It returns [@0xc00000000001f528].
  Role (from facts): it returns the halfword at index a0 of a fixed table [@0xc00000000001f52c].
  Reviewed: 2026-11-12
- `xswitch_volunteer_delete` in `6.5.22/unix.IP27` at 0xc000000000023eec
  **Inferred.** It calls hwgraph_info_remove_LBL with 0xc000000001404960 as its second argument [call 1]. It calls kmem_free with 0x20 as its second argument [call 2] and returns its result [@0xc000000000023f10].
  Role (from facts): it calls one routine on its first argument [call 1] and then calls kmem_free with a 0x20 size [call 2].
  Reviewed: 2026-11-12
- `mlreset` in `6.5.22/unix.IP27` at 0xc000000000025eb0
  **Inferred.** It calls r1nkperf_control_register_set with 0x0 as its first argument [call 1] and with 0x1 as its first argument [call 2]. It calls r1nkperf_data_register_set with 0x0 as its first argument [call 3] and with 0x1 as its first argument [call 4]. It calls get_cpu_irr [call 5], and it calls get_cpu_irr with 0x2 as its first argument [call 6, @0xc000000000025f00 fall]. When a0 is nonzero [@0xc000000000025f20 fall] it calls flush_tlb [call 7] and then flush_cache [call 8], and returns [@0xc000000000025f68]. When a0 is zero [@0xc000000000025f20 taken] it calls ioc3_mlreset [call 19, @0xc000000000025f20 taken] and nvram_baseinit [call 20, @0xc000000000025f20 taken].
  Role (from facts): it is a long sequence of calls that sets the registers of r1nkperf_control_register_set [call 1, call 2] and r1nkperf_data_register_set [call 3, call 4] and reads CPU state [call 5, call 6]. It then takes one of two paths on a0 [@0xc000000000025f20 taken]: flush_tlb [call 7] then flush_cache [call 8] on the nonzero side [@0xc000000000025f20 fall], or the node and I/O set-up routines such as ioc3_mlreset [call 19] and nvram_baseinit [call 20].
  Reviewed: 2026-11-12
- `node_getfirstfree` in `6.5.22/unix.IP27` at 0xc000000000029c9c
  **Inferred.** It calls get_freemem_start [call 1]. It returns [@0xc000000000029cbc] the call's result after adding 0x3fff and shifting right by 14, not unchanged.
  Role (from facts): it calls get_freemem_start [call 1] and returns its result rounded up to a multiple of 0x4000 and divided by 0x4000 [@0xc000000000029cbc].
  Reviewed: 2026-11-12
- `hubii_eint_handler` in `6.5.22/unix.IP27` at 0xc000000000034c34
  **Inferred.** It calls check_ni_errors [call 1] and, when that returns nonzero [@0xc000000000034c48 fall nonzero], it calls hubni_error_handler [call 2, @0xc000000000034c48 fall nonzero]. It calls hwgraph_info_get_LBL [call 3]. It tests bit 32 of a doubleword at an address computed from that result [@0xc000000000034c9c taken]. It calls hubiio_crb_error_handler [call 4] and hubiio_prb_error_handler [call 5] when that bit is clear [@0xc000000000034c9c fall].
  Role (from facts): it runs the NI error check and handler [call 1, call 2], then calls two hub I/O error handlers [call 4, call 5] when bit 32 of a per-node doubleword is clear [@0xc000000000034c9c fall].
  Reviewed: 2026-11-12
- `hub_error_status_war` in `6.5.22/unix.IP27` at 0xc000000000034d94
  **Inferred.** It loads a word from a fixed address [@0xc000000000034d98]. When that word is zero [@0xc000000000034da4 taken] it loads a second fixed word [@0xc000000000034dcc] and, when bit 63 of it is set [@0xc000000000034dd4 taken], returns [@0xc000000000034dc0]. When the first word is nonzero [@0xc000000000034da4 fall] it loads a different fixed word [@0xc000000000034db0] and, when bit 63 of it is set [@0xc000000000034db8 fall], returns [@0xc000000000034dc0]. It calls _badaddr_val with 0x9200000002400208 as its first argument and 0x8 as its second argument [call 1, @0xc000000000034db8 taken].
  Role (from facts): it selects one of two fixed words by testing a third [@0xc000000000034da4 taken], and probes an address with _badaddr_val [call 1, @0xc000000000034db8 taken] when bit 63 of the selected word is clear.
  Reviewed: 2026-11-12
- `hub_error_count_get` in `6.5.22/unix.IP27` at 0xc000000000035098
  **Inferred.** It returns 0 when a0 is negative [@0xc0000000000350a0 taken] or at least 0x100 [@0xc0000000000350b4 taken]. It loads a halfword indexed by a0 [@0xc0000000000350bc] and returns 0 when that halfword is -1 [@0xc0000000000350c0 taken]. Otherwise [@0xc0000000000350c0 fall] it stores to +0x0 of a1 [@0xc0000000000350cc], loads the doubleword at that halfword's index in the table at 0xc0000000014dc2e8 [@0xc0000000000350d4], and returns it [@0xc0000000000350d8].
  Role (from facts): it maps the index a0 through a halfword table and returns the doubleword at the mapped index of a second table [@0xc0000000000350d8], and returns 0x0 for an out-of-range index [@0xc0000000000350e0] or an unmapped one [@0xc0000000000350e8].
  Reviewed: 2026-11-12
- `intr_init_vecblk` in `6.5.22/unix.IP27` at 0xc000000000037c74
  **Inferred.** It stores doublewords [@0xc000000000037d18, @0xc000000000037d20, @0xc000000000037d28] and words [@0xc000000000037d2c, @0xc000000000037d54, @0xc000000000037d64]. It loops while a3 is below 0x40 [@0xc000000000037d60 taken]. It calls init_spinlock with 0xc00000000140b768 as its second argument [call 1].
  Role (from facts): it fills a block of fields in a loop over indexes below 0x40 [@0xc000000000037d60 taken] and calls init_spinlock [call 1] passing the constant 0xc00000000140b768 as its second argument.
  Reviewed: 2026-11-12
