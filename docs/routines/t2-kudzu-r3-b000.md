# Tier-2 routine readings, kudzu batch 0

Role statements for the first 25 routines of the tier-2 list, written from the rendered
instructions only: loads, stores, calls, branches and constants. Every grade is inferred.
Where the facts stop short of a meaning, the statement says so. Hub registers appear as
the uncached `0x92...` addresses the facts report.

- `get_xbow_peer` in `6.5.22/unix.IP27` at 0xc00000000001e58c
  **Inferred.** Makes no calls. It loads a 64-bit word from a fixed address, loads a pointer
  through it, indexes that with the 16-bit value at 0xc0000000014d3600 (scaled by 8), loads
  a pointer from the element, and returns the 16-bit field at offset 0x4990 of what that
  points to. The name suggests a crossbar peer lookup, but the rendered facts do not settle
  what the returned field means.
  Reviewed: 2026-11-12 (sonnet)
- `board_serial_number_get` in `6.5.22/unix.IP27` at 0xc00000000001e830
  **Inferred.** Branches to 0xc00000000001e8a8 when a0 or a1 is zero. It loads a byte at
  offset 0x5 of an unresolved base and compares it with 0x10, 0x20, 0x30 and 0x50; each
  match goes to its own find_first_component call at 0xc00000000001d944, with type argument
  0x2, 0x5, 0x9 and 0xa respectively. When that call returns nonzero it calls
  component_serial_number_get at 0xc00000000001e6f4 with a pointer loaded from the result
  and a destination buffer among 0xc000000001404050, 0xc000000001404060 and
  0xc000000001404068 (five such calls in all). A zero from component_serial_number_get
  branches to 0xc00000000001e894. It returns 0 or 1, and the rendered facts do not show
  which value means found.
  Reviewed: 2026-11-12 (sonnet)
- `membank_check_mixed_hidensity` in `6.5.22/unix.IP27` at 0xc00000000001ea90
  **Inferred.** Before the first call it loads a 64-bit word at offset 0x48 and a 32-bit
  word. It calls find_lboard at 0xc00000000001d6b4 with a1 = 0x11, and calls
  find_first_component at 0xc00000000001d944 with a1 = 0x3. A 16-bit load sits in a loop
  that continues while a4 is below 8 (signed). It compares a value with the constant 0x200
  and with another value, and returns 0 or 1. The base of the loaded fields is not resolved,
  so what is read from the find_lboard result is not shown. The name suggests a check for
  banks of mixed density, and the compares do not settle which outcome yields 1.
  Reviewed: 2026-11-12 (sonnet)
- `hub_merge_clean` in `6.5.22/unix.IP27` at 0xc000000000028a90
  **Inferred.** Loads a 64-bit word at a fixed displacement from an unresolved base, and
  later loads one more 64-bit word and a 32-bit word. It branches on whether (t3 & 0x7fff)
  is zero, where t3 is not resolved by the facts, and loops back while a1 is nonzero. No
  stores appear in the rendered facts, and the return value is not settled.
  Reviewed: 2026-11-12 (sonnet)
- `net_link_up` in `6.5.22/unix.IP27` at 0xc00000000002e2f0
  **Inferred.** A short leaf routine (20 bytes in the inventory) that loads one 64-bit word
  and returns. The rendered facts do not give the load address or show how the return value
  is formed from the word, so the function's result is unknown.
  Reviewed: 2026-11-12 (sonnet)
- `hub_error_clear` in `6.5.22/unix.IP27` at 0xc0000000000321f8
  **Inferred.** Loads five 64-bit values at fixed displacements from a base, stores 64-bit
  words at two other displacements, loads a sixth value, and returns (a0 << 0x20) +
  0x9200000001c00000. A further 64-bit store, at the same displacement as the sixth load, is
  listed at 0xc000000000032254 just after the return instruction. The facts do not resolve
  the base or the stored values, so which bits are cleared is open. The name suggests
  clearing hub error registers.
  Reviewed: 2026-11-12 (sonnet)
- `huberror_process_rrb_error` in `6.5.22/unix.IP27` at 0xc000000000033db8
  **Inferred.** Branches on the error-type argument a0 against 3, 2 and 1; any other value
  reaches a cmn_err call at 0xc0000000001f8090 with a0 = 0x14. Each of the three paths calls
  nasid_to_partid at 0xc0000000003c2928 and part_id at 0xc0000000003c2910 and compares the
  results. On a match it calls error_check_part_page at 0xc000000000037a7c, and a nonzero
  result there reaches partition_handle_error at 0xc000000000037afc; on a mismatch it calls
  partition_handle_error directly. That call passes a2 = 0xa on the type 1 path, 0x1 on type
  3 and 0x4 on type 2. On the type 2 path it also calls page_isdiscarded at
  0xc0000000001b1c00; when that returns zero it calls membank_pathname_get at
  0xc00000000001e9d8, cmn_err and error_page_discard at 0xc0000000000379e0. There are four
  cmn_err calls in all. The return values are one of 0x0, 0x2, 0x4, 0x6, 0x7 and 0x8 or a
  value the facts leave unresolved, and their meanings are not settled.
  Reviewed: 2026-11-12 (sonnet)
- `hubii_prb_handler` in `6.5.22/unix.IP27` at 0xc000000000034b10
  **Inferred.** Loads a 16-bit field at offset 0xa of its second argument and reports it
  through cmn_err at 0xc0000000001f8090 (a0 = 0x4, message at 0xc00000000140a218, the field
  as a2). It stores a 64-bit word at the instruction listed just after the return. The facts
  do not resolve that store's address, the stored data or the return value. The name
  suggests a handler for a probe response.
  Reviewed: 2026-11-12 (sonnet)
- `hubiio_prb_error_handler` in `6.5.22/unix.IP27` at 0xc000000000034b68
  **Inferred.** Reads the same 16-bit field at offset 0xa of its second argument and loads a
  64-bit register from a hub address built from it. It calls hubii_prb_handler at
  0xc000000000034b10 from two places, and a loop continues while a counter in s1 is below 0x10
  (the compare at 0xc000000000034bd8). The rendered facts do not show what the loop counts.
  Reviewed: 2026-11-12 (sonnet)
- `hubii_eint_handler` in `6.5.22/unix.IP27` at 0xc000000000034c34
  **Inferred.** Calls check_ni_errors at 0xc000000000032ca4 and, on a nonzero result,
  hubni_error_handler at 0xc000000000032f00. It calls hwgraph_info_get_LBL at
  0xc000000000296160 and branches on the sign of the status word at 0x9200000001c00008
  shifted left by 0x1f. When that is non-negative it calls hubiio_crb_error_handler at
  0xc000000000034790 and hubiio_prb_error_handler at 0xc000000000034b68 and returns the
  latter's result. When it is negative it tests the word shifted left by 0x36, 0x39 and
  0x3a, calls cmn_err with a0 = 0 on one path, and reads the word at 0x9200000001c001e8 and
  tests its low bit. The routine stores 64-bit words at two places, and the facts do not
  resolve their addresses or values. Role (from name): the handler for a hub error
  interrupt, which the bit tests and calls fit.
  Reviewed: 2026-11-12 (sonnet)
- `hub_error_count_disable` in `6.5.22/unix.IP27` at 0xc000000000035044
  **Inferred.** Returns -1 when a0 is negative (signed) or when an unresolved value v1 is
  zero. Past those checks it loads the 16-bit value at 0xc0000000014d3600 and compares it
  with -1. When the value is not -1 it loads a 64-bit pointer and stores a 32-bit word
  through it. Both outcomes of that compare reach the return at 0xc000000000035088, which
  gives -1 or 0. The stored value and the meaning of the word are not in the rendered facts.
  The name suggests it turns off hub error counting.
  Reviewed: 2026-11-12 (sonnet)
- `intr_init_vecblk` in `6.5.22/unix.IP27` at 0xc000000000037c74
  **Inferred.** Loads a byte and a 32-bit word (the facts do not give their addresses). It
  stores doublewords, words, halfwords and bytes per slot in a loop that continues while a3
  is below 0x40 (signed). When s3 is nonzero it calls init_spinlock at 0xc000000000286fdc
  with a1 = 0xc00000000140b768 and a4 chosen from fixed values, and returns the result of
  init_spinlock. The slot store targets are not settled. Role (from name): sets up an
  interrupt vector block.
  Reviewed: 2026-11-12 (sonnet)
- `intr_reserve_level` in `6.5.22/unix.IP27` at 0xc00000000003805c
  **Inferred.** Forwards a0, a1 and a2 and the next two arguments to do_intr_reserve_level at
  0xc000000000037e28, with the fourth argument fixed to 1, and returns its result. The fixed 1
  is probably a reserve mode, but the rendered facts do not say what it selects.
  Reviewed: 2026-11-12 (sonnet)
- `intr_unreserve_level` in `6.5.22/unix.IP27` at 0xc000000000038084
  **Inferred.** Calls do_intr_reserve_level at 0xc000000000037e28 with the first two arguments
  forwarded and zeros in the other four, then returns its result. The zero where
  intr_reserve_level passes 1 suggests the same routine releases a reservation when that
  argument is zero. The rendered facts show the call only.
  Reviewed: 2026-11-12 (sonnet)
- `intr_get_ptrs` in `6.5.22/unix.IP27` at 0xc0000000000380ac
  **Inferred.** Loads a 64-bit table pointer from 0xc0000000014d39a8 and branches on a1 <
  0x40 (signed). Each path loads one 64-bit pointer from the table, at +0x5a8 when a1 < 0x40
  and at +0x5b0 when a1 is 0x40 or more. Each path also makes two 64-bit stores and two
  32-bit stores through registers the facts do not resolve. Both return paths give the table
  address plus 0x598.
  Reviewed: 2026-11-12 (sonnet)
- `intr_connect_level` in `6.5.22/unix.IP27` at 0xc000000000038108
  **Inferred.** Calls intr_get_ptrs at 0xc0000000000380ac and mutex_spinlock_spl at
  0xc0000000002871c0. When bit 0x2 of a loaded word is set it calls mutex_spinunlock at
  0xc000000000287260 and returns -1 or 0; that is the only mutex_spinunlock call in the
  facts. The other path stores 64-bit words, one at a large negative displacement from an
  unresolved base. When (a2 & 0x100) is nonzero it runs an ll and sc loop at
  0xc000000000038318 and 0xc000000000038320, and when a later value is nonzero it calls
  intr_thread_setup at 0xc00000000003a1e4 with a4 = 0x200. Role (from name): connects a
  handler to an interrupt level.
  Reviewed: 2026-11-12 (sonnet)
- `intr_disconnect_level` in `6.5.22/unix.IP27` at 0xc0000000000383a4
  **Inferred.** Looks like the counterpart of intr_connect_level, judging by its name and
  shared callees. It calls intr_get_ptrs at 0xc0000000000380ac and mutex_spinlock_spl at
  0xc0000000002871c0. When a loaded word is not 0x3 it calls mutex_spinunlock at
  0xc000000000287260 and returns -1 or 0. The rest stores 64-bit and 32-bit words, including
  one 64-bit store at a negative displacement from an unresolved base. The stored values and
  the hub offsets are not in the rendered facts, so the teardown reading rests on the shared
  callees and the name.
  Reviewed: 2026-11-12 (sonnet)
- `do_intr_block_bit` in `6.5.22/unix.IP27` at 0xc000000000038550
  **Inferred.** Changes the blocked state of an interrupt bit. It reads pointers with
  intr_get_ptrs at 0xc0000000000380ac, takes a lock with mutex_spinlock_spl at
  0xc0000000002871c0, reads and writes one of four 64-bit words in the 0x92000000018000a8 to
  0x92000000018000c0 range, and releases with mutex_spinunlock at 0xc000000000287260. The
  third argument selects block or unblock, as the callers show. The exact update is not in
  the rendered facts.
  Reviewed: 2026-11-12 (sonnet)
- `intr_block_bit` in `6.5.22/unix.IP27` at 0xc0000000000386bc
  **Inferred.** Forwards its arguments to do_intr_block_bit at 0xc000000000038550 with the
  third argument set to 1, and returns its result. The 1 selects blocking, given the name.
  Reviewed: 2026-11-12 (sonnet)
- `intr_unblock_bit` in `6.5.22/unix.IP27` at 0xc0000000000386d8
  **Inferred.** Forwards its arguments to do_intr_block_bit at 0xc000000000038550 with the
  third argument set to 0, and returns its result. The 0 is the unblock counterpart of the
  1 that intr_block_bit passes.
  Reviewed: 2026-11-12 (sonnet)
- `intr_cpu_choose_from_node` in `6.5.22/unix.IP27` at 0xc00000000003881c
  **Inferred.** Forwards its four arguments to do_intr_cpu_choose at 0xc0000000000386f4 and
  returns that result. The name suggests choosing a CPU for an interrupt on a node, but the
  choice is made in the callee, which is not part of this routine.
  Reviewed: 2026-11-12 (sonnet)
- `intr_target_from_desc` in `6.5.22/unix.IP27` at 0xc000000000038934
  **Inferred.** Calls device_desc_intr_target_get at 0xc000000000175f44 with its four
  arguments forwarded and compares the result with -1. When it is not -1 the routine calls
  cpuvertex_to_cpuid at 0xc000000000297b48 on the result. Other paths call
  subnodevertex_to_subnode at 0xc000000000038838 on the same result. When that returns
  nonzero they call master_node_get at 0xc000000000297a94 and, if the result is not -1, call
  intr_cpu_choose_from_node at 0xc00000000003881c with a4 = the master_node_get result. When
  it returns zero they call intr_cpu_choose_from_node with unresolved arguments. It returns
  -1 or a value the facts do not settle. Role (from name): maps a device descriptor to an
  interrupt target.
  Reviewed: 2026-11-12 (sonnet)
- `intr_cnode_seen` in `6.5.22/unix.IP27` at 0xc0000000000389ec
  **Inferred.** Loads a 32-bit word through its third argument and returns its second
  argument when that word is zero or negative. When it is positive the routine loops over
  16-bit values from a computed address, returns 0 when one equals a0 (the compare at
  0xc000000000038a0c), and returns its second argument when the loop ends on a5 == 0 (a5 is
  not resolved by the facts). Role (from name): records whether a node's CPU has been seen
  during the target search.
  Reviewed: 2026-11-12 (sonnet)
- `intr_bit_reserve_test` in `6.5.22/unix.IP27` at 0xc000000000038a2c
  **Inferred.** Calls intr_cpu_choose_from_node at 0xc00000000003881c with a0 = a2 at entry,
  skipping the call when a2 is -1. When the result is not -1 it calls do_intr_reserve_level
  at 0xc000000000037e28 with a3 = 1, forwarding its fourth, fifth, sixth and seventh
  arguments in a1, a2, a4 and a5. When that returns a nonnegative value it stores a 32-bit
  word; the facts do not resolve the store's base register. It returns -1 on one path and a
  value the facts do not settle on the other. Whether the reservation is committed or only
  tested is not in the rendered facts.
  Reviewed: 2026-11-12 (sonnet)
- `intr_heuristic` in `6.5.22/unix.IP27` at 0xc000000000038ad4
  **Inferred.** Calls physmem_maxradius at 0xc000000000273b70 with its four arguments
  forwarded. When a1 is nonzero it calls intr_target_from_desc at 0xc000000000038934 with a1
  as its first argument and -1 as its second, and intr_bit_reserve_test at
  0xc000000000038a2c on the result. Later it calls intr_bit_reserve_test with -1 for a0 and
  a1 three more times (after master_node_get at 0xc000000000297a94, after
  physmem_select_neighbor_node at 0xc000000000273024, and with a 16-bit field at offset
  0x422 of the table at 0xc0000000014d39a8 as a2). The physmem_select_neighbor_node call
  passes 0xc0000000000389ec, the address of intr_cnode_seen, as a3. It makes three cmn_err
  calls at 0xc0000000001f8090 with a0 = 0x4. The search order and the return values are not
  settled by the rendered facts. Role (from name): looks for an interrupt target when none
  is given.
  Reviewed: 2026-11-12 (sonnet)
