# Tier-2 routine readings, kudzu batch 1

Role statements for 25 routines of the tier-2 list in the IP27 kernel build,
written from the rendered instructions only: loads, stores, calls, branches and
constants. Every grade is inferred. The facts show what each routine reads and
calls, not what the fields mean, so each statement leaves the meaning open where
the facts stop. Hub register windows appear as the `0x92...` and `0x96...`
addresses the facts report.

- `xbow_port_io_enabled` in `6.5.22/unix.IP27` at 0xc00000000001e344
  **Inferred.** Looks up a board with `find_lboard` (class argument 0x41), then a
  component under it with `find_component` (arguments 0x0 and 0x4), branching on
  whether each lookup returns zero. The facts show no loop. It returns 0 or 1 from
  tests on the byte at offset -6 from `(component + (a1 << 3))`, loaded at
  0xc00000000001e3cc: the first branch tests bit 0x1, and a second branch follows that
  the facts show only as a comparison with zero. The name suggests a crossbow port with
  I/O enabled, but the facts do not show which port the argument selects.
  Reviewed: 2026-11-12 (sonnet)

- `board_to_path` in `6.5.22/unix.IP27` at 0xc00000000001e408
  **Inferred.** Formats a textual path for a board. It branches on a value held in `a2`
  against 0x10, 0x20, 0x30, 0x40 and 0x80, a register the facts do not trace to an
  entry argument, and on a board-type byte at offset 5 against 0x33 and 0x34. On the
  path through 0xc00000000001e474 it calls `get_slotname` (0xc00000000003c99c) with the
  byte at offset 0xa, then `sprintf` (0xc0000000001f8e8c) into the buffer passed as the
  second argument, and returns the `sprintf` result. The facts do not show whether every
  branch reaches both calls, or which string each branch produces.
  Reviewed: 2026-11-12 (sonnet)

- `get_module_id` in `6.5.22/unix.IP27` at 0xc00000000001e514
  **Inferred.** Finds a board of class 0x11 with `find_lboard` and returns the 16-bit
  value at offset 0xe of that board record (at 0xc00000000001e578). It returns -1 when
  no such board is found. The name suggests a module number; the facts show only
  the field offset.
  Reviewed: 2026-11-12 (sonnet)

- `get_xbow_peer` in `6.5.22/unix.IP27` at 0xc00000000001e58c
  **Inferred.** Walks a per-node pointer table: it reads the global at abs
  0xffffffffffffc400, follows the pointer at -0x3c00, indexes by an entry from table
  0xc0000000014d3600, and returns the 16-bit value at offset 0x4990 of that node's
  record (at 0xc00000000001e5b0). The name suggests the crossbow peer of a port, and
  nothing in the facts contradicts it, but the table meanings are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `device_component_canonical_name_get` in `6.5.22/unix.IP27` at 0xc00000000001e648
  **Inferred.** Builds a canonical device name from a component record. It takes a slot
  byte at offset 0xa, calls `get_slotname` (0xc00000000003c99c) and then `sprintf`
  (0xc0000000001f8e8c) with a format word from 0xc000000001404030, writing into the
  buffer passed as the third argument. When `a0` is zero, a branch at
  0xc00000000001e67c goes to 0xc00000000001e698, where an 8-byte load is followed by the
  unaligned stores `sdl` and `sdr` (0xc00000000001e69c and 0xc00000000001e6a0); the
  facts do not show the destination of those stores. When a class byte at offset 6 is 2
  or more, it calls `nic_name_convert` (0xc00000000001dacc) on the string at offset 153
  of the record. It returns the `sprintf` result.
  Reviewed: 2026-11-12 (sonnet)

- `board_serial_number_get` in `6.5.22/unix.IP27` at 0xc00000000001e830
  **Inferred.** Branches away from the lookups when `a0` or `a1` is zero (0xc00000000001e844
  and 0xc00000000001e84c). Otherwise it compares a byte loaded at offset 5 against
  0x10, 0x20, 0x30 and 0x50, and each match leads to one of four `find_first_component`
  calls (at 0xc00000000001e8d0, 0xc00000000001e8ec, 0xc00000000001e908 and
  0xc00000000001e8b4 respectively), with second arguments 0x2, 0x5, 0x9 and 0xa. A
  nonzero result is passed to `component_serial_number_get` (0xc00000000001e6f4), with
  a static buffer at 0xc000000001404050, 0xc000000001404060 or 0xc000000001404068 as
  the fourth argument. The routine also loads and stores single bytes at
  0xc00000000001e854, 0xc00000000001e858, 0xc00000000001e888 and 0xc00000000001e88c; the
  facts do not show the destination of those stores. It returns 0 or 1. The four
  lookups are selected by the byte compare, not tried in turn.
  Reviewed: 2026-11-12 (sonnet)

- `membank_check_mixed_hidensity` in `6.5.22/unix.IP27` at 0xc00000000001ea90
  **Inferred.** Finds a board of class 0x11 with `find_lboard`, then the first
  component of class 3 under it with `find_first_component` (at 0xc00000000001eae4).
  It loads 16-bit values in a loop, with a backward branch at 0xc00000000001eb1c
  bounded by 0x8, then compares a result against 0x200 and against another value
  (branches at 0xc00000000001eb28 and 0xc00000000001eb30), and returns 0 or 1. The name
  suggests a check for banks of mixed high density, but the facts do not identify the
  loaded fields.
  Reviewed: 2026-11-12 (sonnet)

- `mem_mixed_hidensity_banks` in `6.5.22/unix.IP27` at 0xc00000000001eb44
  **Inferred.** Loops over the bank count stored at 0xc0000000014d26a4, loading a
  16-bit entry for each index and skipping the entry when it equals -1 (branch at
  0xc00000000001eb78). For each other entry it calls `membank_check_mixed_hidensity`
  (0xc00000000001ea90). It returns 0 or 1 from the result and the loop state. The facts
  do not show what the caller does with a bank that fails the check.
  Reviewed: 2026-11-12 (sonnet)

- `do_cpumask` in `6.5.22/unix.IP27` at 0xc00000000001f2f4
  **Inferred.** Works on a board of class 0x11 found with `find_lboard`, with a
  component found by `find_first_component` and `find_component`, and the NASID from
  `get_actual_nasid` (0xc00000000001e210). It stores a 16-bit value to table
  0xc0000000014d3880 and updates a word at the caller's pointer `a3`, under a bit mask
  test. It has three assertion-failure calls (`assfail`, 0xc00000000003a2b4): one on the
  branch at 0xc00000000001f3a4 when a loaded 16-bit value is 0x80 or more, one on the
  branch at 0xc00000000001f408 when the masked bit is already set, and one when the
  first `find_lboard` returns zero. The name suggests building a CPU mask for a node;
  the facts do not confirm that the bit mask holds CPU numbers.
  Reviewed: 2026-11-12 (sonnet)

- `nasid_to_compact_nodeid` in `6.5.22/unix.IP27` at 0xc00000000001f504
  **Inferred.** Returns one 16-bit entry from table 0xc0000000014d3600. The name
  suggests a NASID-to-compact-node mapping, the table index is the argument, and no
  other step appears in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `compact_to_nasid_nodeid` in `6.5.22/unix.IP27` at 0xc00000000001f518
  **Inferred.** Returns one 16-bit entry from table 0xc0000000014d3800, indexed by the
  argument. It reads a different table from `nasid_to_compact_nodeid`
  (0xc00000000001f504). Role (from name): the reverse mapping of that routine; the
  facts do not show that the two tables are inverses.
  Reviewed: 2026-11-12 (sonnet)

- `get_lowest_nasid` in `6.5.22/unix.IP27` at 0xc00000000001f530
  **Inferred.** Scans the 16-bit entries of table 0xc0000000014d3800 in a loop and
  keeps the smallest value, using a signed compare at 0xc00000000001f56c. A count word
  at 0xc0000000014d26a4 gates the loop. The facts do not settle what the routine
  returns when no entry qualifies.
  Reviewed: 2026-11-12 (sonnet)

- `cnode_exists` in `6.5.22/unix.IP27` at 0xc00000000001f590
  **Inferred.** Rejects a negative argument and any value of 0x40 or more,
  returning 0 for either (at 0xc00000000001f5c4). Otherwise it reads a 16-bit entry from
  table 0xc0000000014d3800 (at 0xc00000000001f5b0) and returns a value derived from it
  at 0xc00000000001f5b8; the facts do not show the derivation. The name suggests a
  presence test for a compact node number.
  Reviewed: 2026-11-12 (sonnet)

- `set_master_bridge_base` in `6.5.22/unix.IP27` at 0xc00000000001f638
  **Inferred.** Reads through a hub register window address built from the pointer at
  0xc0000000014c8f80, with loads of a 32-bit word, a 64-bit value, a halfword and a
  byte. A branch at 0xc00000000001f698 tests a value masked with 0xff against zero, and
  the facts do not show which load it comes from. The rendered facts list no store
  instruction, which does not by itself show that the routine only queries. The name
  suggests it sets the master bridge base; the facts do not settle which.
  Reviewed: 2026-11-12 (sonnet)

- `get_console_nasid` in `6.5.22/unix.IP27` at 0xc00000000001f6c0
  **Inferred.** Returns the 16-bit global at 0xc0000000014d1388 and takes no
  arguments. The name suggests the NASID of the console hub, which a single load
  from one fixed word fits, but the facts do not identify the word's writer.
  Reviewed: 2026-11-12 (sonnet)

- `get_console_pcislot` in `6.5.22/unix.IP27` at 0xc00000000001f6c8
  **Inferred.** Returns the byte at 0xc0000000014d138b. Like `get_console_nasid`
  (0xc00000000001f6c0), it is a one-load accessor for a console field, and the name
  suggests a PCI slot number.
  Reviewed: 2026-11-12 (sonnet)

- `check_nasid_equiv` in `6.5.22/unix.IP27` at 0xc00000000001f6d0
  **Inferred.** Returns 1 at once when its two arguments are equal (branch at
  0xc00000000001f6d4). Otherwise it reads a 16-bit value at offset 0x4990 of the node
  record selected by table 0xc0000000014d3600 and returns 0 or 1 from comparing it
  with the first argument. The name suggests NASID equivalence between two hubs; the
  facts do not show which equivalence rule the table encodes.
  Reviewed: 2026-11-12 (sonnet)

- `is_master_nasid_widget` in `6.5.22/unix.IP27` at 0xc00000000001f710
  **Inferred.** Compares a widget byte at 0xc0000000014d138a with its second argument,
  and calls `check_nasid_equiv` (0xc00000000001f6d0) with the master NASID at
  0xc0000000014d1388. It returns 0 or 1, and the facts suggest it returns 1 only when
  both tests agree. The name suggests a test for the master widget, but the facts do
  not show what the widget byte means.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_exception` in `6.5.22/unix.IP27` at 0xc0000000000234a0
  **Inferred.** An exception entry for ECC errors. It stores a block of registers, calls
  `tfi_save` (0xc00000000003a240), then calls `ecc_handler` (0xc0000000000557c0) with
  arguments 0x480 and 0x5e8. The branch at 0xc000000000023588 skips the call to
  `ecc_panic` (0xc000000000055f80) when the handler returns zero, so `ecc_panic` is
  reached when the handler returns nonzero. It then restores state with `tfi_restore`
  (0xc00000000003a278). The name and the callees suggest this reading; the facts do not
  show what the handler does or what 0x480 and 0x5e8 mean.
  Reviewed: 2026-11-12 (sonnet)

- `xswitch_vertex_init` in `6.5.22/unix.IP27` at 0xc000000000023e94
  **Inferred.** Allocates a 32-byte record with `kmem_zalloc` (argument 0x20), sets up
  a mutex in it with `init_mutex`, and attaches the record to the vertex with
  `hwgraph_info_add_LBL` under the label at 0xc000000001404960. It returns the result
  of the attach. The name suggests crossbow switch vertex setup, which the allocation
  and label attach fit.
  Reviewed: 2026-11-12 (sonnet)

- `xswitch_volunteer_delete` in `6.5.22/unix.IP27` at 0xc000000000023eec
  **Inferred.** Removes the label at 0xc000000001404960 with `hwgraph_info_remove_LBL`
  (0xc0000000002960f0), passing a stack slot as the third argument, then calls
  `kmem_free` with size argument 0x20; the pointer argument is not shown in the facts.
  It mirrors `xswitch_vertex_init` (0xc000000000023e94) in the label and the size 0x20,
  which suggests it frees the record that routine allocated.
  Reviewed: 2026-11-12 (sonnet)

- `per_hub_init` in `6.5.22/unix.IP27` at 0xc000000000026878
  **Inferred.** A large per-hub setup routine. It takes a hub index and takes and
  releases the spinlock at 0xc0000000014d13b8 twice (calls 1 and 2, then calls 14 and
  15), the second time around a store. It calls `get_hub_chiprev` (0xc00000000001d474),
  `cnode_slice_to_cpuid` (0xc000000000026fac), `cpu_enabled` (0xc000000000026f74),
  `intr_reserve_hardwired` (0xc000000000039a20), `hub_error_init`, `install_eccintr`,
  `protect_hub_calias`, `hub_rtc_init`, `hub_migrintr_init`, `hub_setup_kick` and
  `hub_init_ifdr_war`. The callee set matches hub initialization, but the branch
  structure is too large for the facts to settle which steps run in which case.
  Reviewed: 2026-11-12 (sonnet)

- `cnodetocpu` in `6.5.22/unix.IP27` at 0xc000000000026d14
  **Inferred.** Maps a compact node number to a CPU number. It reads a per-node slice
  record through the pointer at -0x3c00, takes 16-bit fields at offsets 0x4956 and
  0x4958, and calls `cpu_enabled` (0xc000000000026f74) to test each candidate. It
  returns -1 when nothing qualifies, and otherwise the CPU it found. The name and the
  candidate loop suggest this reading; the facts do not show the field meanings.
  Reviewed: 2026-11-12 (sonnet)

- `cnode_subnode_to_cpu` in `6.5.22/unix.IP27` at 0xc000000000026dc4
  **Inferred.** The same shape as `cnodetocpu` (0xc000000000026d14), with a second
  index in `a1`. The second index is tested against zero at 0xc000000000026e30, inside
  the candidate loop and only after `cpu_enabled` returns nonzero. The name suggests a
  lookup by subnode as well as node; the facts do not show what the subnode field
  selects.
  Reviewed: 2026-11-12 (sonnet)

- `get_compact_nodeid` in `6.5.22/unix.IP27` at 0xc000000000026e90
  **Inferred.** Converts a node argument to a NASID with `get_nasid`
  (0xc00000000001d434), then returns the 16-bit entry from table 0xc0000000014d3600 for
  that NASID. It is the composition of `get_nasid` with the NASID-to-node table, so the
  name fits the code, although the table index is inferred from the call order.
  Reviewed: 2026-11-12 (sonnet)
