# Tier-2 routine readings, kudzu batch 0

Role statements for the first 25 routines of the tier-2 list, written from the rendered
instructions only: loads, stores, calls, branches and constants. Every grade is inferred.
Where the facts stop short of a meaning, the statement says so. Hub registers appear as
the uncached `0x92...` addresses the facts report.

- `get_xbow_peer` in `6.5.22/unix.IP27` at 0xc00000000001e58c
  **Inferred.** Makes no calls. It loads a 16-bit index from the table at 0xc0000000014d3600,
  follows two pointers through fixed addresses, and returns a 16-bit field at offset 0x4990
  of the structure the chain reaches. The name suggests a crossbar peer lookup, but the
  rendered facts do not settle what the returned field means.
- `board_serial_number_get` in `6.5.22/unix.IP27` at 0xc00000000001e830
  **Inferred.** Searches the component tree with find_first_component at 0xc00000000001d944
  for four component types (type arguments 0xa, 0x2, 0x5 and 0x9), then reads each found
  component's serial with component_serial_number_get at 0xc00000000001e6f4 into fixed
  buffers at 0xc000000001404050, 0xc000000001404060 and 0xc000000001404068. It tests a flag
  byte at 0xc000000001404050 and returns 0 or 1. The rendered facts do not show which
  return value means found.
- `membank_check_mixed_hidensity` in `6.5.22/unix.IP27` at 0xc00000000001ea90
  **Inferred.** Finds a board record with find_lboard at 0xc00000000001d6b4 (type 0x11),
  then a component of type 3 with find_first_component at 0xc00000000001d944. It reads a
  32-bit and a 16-bit field from the result, compares them with the constant 0x200 and with
  each other, and returns 0 or 1. The name suggests a check for banks of mixed density, but
  the compares do not settle which outcome yields 1.
- `hub_merge_clean` in `6.5.22/unix.IP27` at 0xc000000000028a90
  **Inferred.** Indexes a per-node structure by a0 and reads a 64-bit word and a 32-bit word
  from it. It tests the low 15 bits of a loaded value against zero and loops back while a1
  is nonzero. No stores appear in the rendered facts, and the return value is not settled.
- `net_link_up` in `6.5.22/unix.IP27` at 0xc00000000002e2f0
  **Inferred.** A 20-byte leaf that loads one 64-bit word from the fixed address
  0x9200000001600000 and returns. The rendered facts do not show how the return value is
  formed from that word, so the function's result is unknown.
- `hub_error_clear` in `6.5.22/unix.IP27` at 0xc0000000000321f8
  **Inferred.** Builds a base address from a0 in the 0x92... hub window and reads five 64-bit
  registers at fixed offsets from it. It writes two of them, rereads one and writes it back,
  then returns a computed address in the 0x9200000001c00000 window. The name suggests
  clearing hub error registers. The values written are not in the rendered facts, so which
  bits are cleared is open.
- `huberror_process_rrb_error` in `6.5.22/unix.IP27` at 0xc000000000033db8
  **Inferred.** Dispatches on the error-type argument a0 against 1, 2 and 3. Each path maps
  the node to a partition with nasid_to_partid at 0xc0000000003c2928 and part_id at
  0xc0000000003c2910, checks the page with error_check_part_page at 0xc000000000037a7c, and
  reports through partition_handle_error at 0xc000000000037afc. It can discard a page with
  page_isdiscarded at 0xc0000000001b1c00 and error_page_discard at 0xc0000000000379e0, and it
  logs three messages with cmn_err at 0xc0000000001f8090. The return values are one of
  0x0, 0x2, 0x4, 0x6, 0x7 or 0x8, and their meanings are not settled by the facts.
- `hubii_prb_handler` in `6.5.22/unix.IP27` at 0xc000000000034b10
  **Inferred.** Reads a 16-bit field at offset 0xa of its second argument and stores a 64-bit
  word to a hub address formed from that field. It reports the field through cmn_err at
  0xc0000000001f8090 with the message at 0xc00000000140a218. The name suggests a handler for
  a probe response, but the stored data and the return value are not in the rendered facts.
- `hubiio_prb_error_handler` in `6.5.22/unix.IP27` at 0xc000000000034b68
  **Inferred.** Reads the same 16-bit field at offset 0xa of its second argument and loads a
  64-bit register from a hub address built from it. It calls hubii_prb_handler at
  0xc000000000034b10 from two places, and a loop continues while a counter in s1 is below 0x10
  (the compare at 0xc000000000034bd8). The rendered facts do not show what the loop counts.
- `hubii_eint_handler` in `6.5.22/unix.IP27` at 0xc000000000034c34
  **Inferred.** Interrupt-side handler for hub errors. It calls check_ni_errors at
  0xc000000000032ca4 and, on a nonzero result, hubni_error_handler at 0xc000000000032f00.
  It then tests sign and shifted bits of the status word at 0x9200000001c00008 to decide
  whether to call hubiio_crb_error_handler at 0xc000000000034790 and hubiio_prb_error_handler
  at 0xc000000000034b68. It also writes back registers at 0x9200000001c001f8 and
  0x9200000001c00138. The name suggests an error interrupt, which the bit tests and calls
  support, but the write values are not in the rendered facts.
- `hub_error_count_disable` in `6.5.22/unix.IP27` at 0xc000000000035044
  **Inferred.** Checks the 16-bit entry at 0xc0000000014d3600 for the index in a0 and returns
  -1 on the early path. Otherwise it writes a 32-bit word to a counter reached through the
  pointer at 0xc0000000014dc2e8 and returns -1 or 0. The name suggests it turns off hub error
  counting. The stored value and the counter's meaning are not in the rendered facts.
- `intr_init_vecblk` in `6.5.22/unix.IP27` at 0xc000000000037c74
  **Inferred.** Sets up an interrupt vector block. It reads a flag byte at 0xc00000000140b764
  and a word at 0xc00000000140b760, initializes a spinlock at 0xc00000000140b768 with
  init_spinlock at 0xc000000000286fdc, and loops over up to 64 slots, storing per-slot
  words and halfwords. Its fifth argument is one of two fixed addresses, passed in
  but not resolved by the rendered facts. The slot store targets are not settled.
- `intr_reserve_level` in `6.5.22/unix.IP27` at 0xc00000000003805c
  **Inferred.** Forwards a0, a1 and a2 and the next two arguments to do_intr_reserve_level at
  0xc000000000037e28, with the fourth argument fixed to 1, and returns its result. The fixed 1
  is probably a reserve mode, but the rendered facts do not say what it selects.
- `intr_unreserve_level` in `6.5.22/unix.IP27` at 0xc000000000038084
  **Inferred.** Calls do_intr_reserve_level at 0xc000000000037e28 with the first two arguments
  forwarded and zeros in the other four, then returns its result. The zero where
  intr_reserve_level passes 1 suggests the same routine releases a reservation when that
  argument is zero. The rendered facts show the call only.
- `intr_get_ptrs` in `6.5.22/unix.IP27` at 0xc0000000000380ac
  **Inferred.** Fills out-parameters from the table at 0xc0000000014d39a8. It loads two
  pointers at offsets 0x5a8 and 0x5b0 and stores them through a3 and a4, and stores 32-bit
  values through a2 and a5. A path taken when a1 is 0x40 or more differs from the other.
  Both return paths give the address of the table entry at offset 0x598.
- `intr_connect_level` in `6.5.22/unix.IP27` at 0xc000000000038108
  **Inferred.** Connects a handler to an interrupt level. It gets pointers from intr_get_ptrs
  at 0xc0000000000380ac and takes a lock with mutex_spinlock_spl at 0xc0000000002871c0. It
  updates the target's table entries in the 0x92... window, using an ll and sc pair at
  0xc000000000038318 and 0xc000000000038320 for an atomic bit update. It calls
  intr_thread_setup at 0xc00000000003a1e4 and releases the lock with mutex_spinunlock at
  0xc000000000287260. It returns -1 or 0.
- `intr_disconnect_level` in `6.5.22/unix.IP27` at 0xc0000000000383a4
  **Inferred.** Looks like the counterpart of intr_connect_level. It takes the same pointers
  from intr_get_ptrs at 0xc0000000000380ac, takes and releases the lock with
  mutex_spinlock_spl and mutex_spinunlock, and stores a 64-bit word to a hub address at
  offset 0xa8 or 0xb0 chosen from the table. It returns -1 or 0. The stored value is not in
  the rendered facts, so the teardown reading rests on the shared callees and the name.
- `do_intr_block_bit` in `6.5.22/unix.IP27` at 0xc000000000038550
  **Inferred.** Changes the blocked state of an interrupt bit. It reads pointers with
  intr_get_ptrs at 0xc0000000000380ac, takes a lock with mutex_spinlock_spl at
  0xc0000000002871c0, reads and writes one of four 64-bit words in the 0x92000000018000a8 to
  0x92000000018000c0 range, and releases with mutex_spinunlock at 0xc000000000287260. The
  third argument selects block or unblock, as the callers show. The exact update is not in
  the rendered facts.
- `intr_block_bit` in `6.5.22/unix.IP27` at 0xc0000000000386bc
  **Inferred.** Forwards its arguments to do_intr_block_bit at 0xc000000000038550 with the
  third argument set to 1, and returns its result. The 1 selects blocking, given the name.
- `intr_unblock_bit` in `6.5.22/unix.IP27` at 0xc0000000000386d8
  **Inferred.** Forwards its arguments to do_intr_block_bit at 0xc000000000038550 with the
  third argument set to 0, and returns its result. The 0 is the unblock counterpart of the
  1 that intr_block_bit passes.
- `intr_cpu_choose_from_node` in `6.5.22/unix.IP27` at 0xc00000000003881c
  **Inferred.** Forwards its four arguments to do_intr_cpu_choose at 0xc0000000000386f4 and
  returns that result. The name suggests choosing a CPU for an interrupt on a node, but the
  choice is made in the callee, which is not part of this routine.
- `intr_target_from_desc` in `6.5.22/unix.IP27` at 0xc000000000038934
  **Inferred.** Maps a device descriptor to an interrupt target. It gets a target vertex with
  device_desc_intr_target_get at 0xc000000000175f44. On one path it converts the vertex to a
  CPU id with cpuvertex_to_cpuid at 0xc000000000297b48, and on another it resolves a
  sub-node and its master node with subnodevertex_to_subnode at 0xc000000000038838 and
  master_node_get at 0xc000000000297a94. When no CPU is chosen it calls
  intr_cpu_choose_from_node at 0xc00000000003881c. It returns -1 or a value the facts do not
  settle.
- `intr_cnode_seen` in `6.5.22/unix.IP27` at 0xc0000000000389ec
  **Inferred.** Reads a 32-bit word through its third argument and a 16-bit value from a
  computed address, then returns either its second argument or zero. The rendered facts do
  not settle the test that picks between the two returns. The name suggests it records
  whether a node's CPU has been seen during the target search.
- `intr_bit_reserve_test` in `6.5.22/unix.IP27` at 0xc000000000038a2c
  **Inferred.** Picks a CPU for the node with intr_cpu_choose_from_node at
  0xc00000000003881c, then calls do_intr_reserve_level at 0xc000000000037e28 with the mode
  argument set to 1. It writes a 32-bit result through its eighth argument, a pointer. It
  returns -1 on one path and another value the facts do not settle on the other. Whether the
  reservation is committed or only tested is not in the rendered facts.
- `intr_heuristic` in `6.5.22/unix.IP27` at 0xc000000000038ad4
  **Inferred.** Looks for an interrupt target when none is given, apparently. It takes a
  maximum radius from physmem_maxradius at 0xc000000000273b70, maps a descriptor with
  intr_target_from_desc at 0xc000000000038934 using -1 as the target, and tries candidate
  bits with intr_bit_reserve_test at 0xc000000000038a2c. When the first attempts fail it asks
  physmem_select_neighbor_node at 0xc000000000273024 for a nearby node, and it logs through
  cmn_err at 0xc0000000001f8090 on three failure paths. It reads a 16-bit field at the fixed
  address 0xffffffffffffc422. The search order and the return values are not settled by the
  rendered facts.
