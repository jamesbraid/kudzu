# Router driver routines of the IP35 kernel

Routines of the NUMAlink router code in the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF). "ri +N" is byte N of the router state record (`struct router_info_s`, 704 bytes, from the debug information). Its members used here: +0 `ri_version`, +2 `ri_cnode`, +4 `ri_nasid`, +6 `ri_ledcache`, +7 `ri_leds`, +8 `ri_portmask`, +16 `ri_stat_rev_id`, +24 `ri_vector`, +32 `ri_writeid`, +40 `ri_timebase`, +48 `ri_timestamp`, +56 `ri_port[]`, +504 `ri_module`, +506 `ri_slotnum`, +512 `ri_glbl_parms[]`, +528 `ri_vertex`, +544 `ri_per_minute`, +552 `ri_hist_type`, +556 `ri_guardian`, +572 `ri_lock`, +576 `ri_vecarray`, +584 `ri_brd`, +592 `ri_name`, +600 `ri_port_maint`.

- `router_init` (`6.5.30/unix.IP35`, address 0xc000000000043500, 1296 bytes, 80-byte frame): brings one router up.
  `a0` is the compact node number the router is attached through, `a1` a write identifier, and `a2` a record from the
  discovery code: its word at +0 is the router's vertex, its doubleword at +16 the route vector to the router, its
  halfword at +4 receives the revision, its byte at +10 holds the slot number and its halfword at +8 the module
  number; the pointer to the new state record is stored at +24. It allocates the 704-byte state record with
  `kmem_zalloc_node(0x2c0, 0x101, a0)`, stores it at
  a2 +24 and labels the vertex with it (`hwgraph_info_add_LBL`, label "_router"), and initialises the lock at ri +572
  ("routerinfolock"). It fills ri +2 with the node, ri +6 with 0xff, ri +4 with the NASID from
  `compact_to_nasid_node`, ri +32 with `a1`, ri +24 with the vector, ri +556 with a word from the per-node table
  reached through the pointer at absolute address -15360 (offset 0x8c80), ri +528 with the vertex, and ri +40 and +48
  with the doubleword read from the uncached address 0x9200000001030100. It computes ri +544, the number of 60-second
  units in a clock count, as
  1,000,000,000
  divided by (10 x the 10-bit field in bits 13:4 of the doubleword at 0x9200000001604200, plus 10) times 60 (the
  divide is guarded by a trap on zero), and sets ri +552 to 1. Then it reads three router registers with
  `router_reg_read(ri, reg, destination)` (address 0xc0000000000427ec): register 0 into ri +16, register 0x20 into ri
  +512 and register 0x28 into ri +520. A failure on register 0 logs "Router init read failed: %v, cnode %d vec 0x%x
  err %d <%s>" through `cmn_err` and returns the error code; a failure on either of the others logs "Router init read
  failed: %v, err %d <%s>" and returns 1. The kernel flag `craylnk_bypass_disable` clears bit 0 of the stored
  register-0x20 value (after a warning "Disabling bypass on router %v" when the halfword `kdebug` is nonzero). It then
  modifies the stored values and writes them back with `router_reg_write` (address 0xc00000000004281c): register 0x20
  becomes (old with bits 19:16 cleared, bits 19:18 set, bits 8:5 cleared) with 0x140 set in it, and register 0x28
  becomes (old masked with 0xffffffff00000fff) with 0x40000000 set; a write failure logs "Router init write failed:
  %v, err %d <%s>" and returns 1. It stores bits 55:52 of the stat word into the record's revision halfword (a2 +4)
  and requires the part number in bits 27:12 to equal 0x3017 (12311): otherwise it logs "Not a router, %v" and frees
  the state record with `kmem_free(ri, 704)` after clearing a2 +24, and then carries on with the freed pointer as the
  code is written, so this path is not trusted here. It copies the slot byte and module halfword from a2 into ri +506
  and +504, allocates the vector array (`kmem_alloc_node(numnodes x 8)` into ri +576; assert "rip->ri_vecarray" at
  line 1131, file argument not copied), finds the router's board with `vertex_to_router_board(nasid, vertex)` into ri
  +584, builds the name (`kern_malloc_node(0x50)` into ri +592, assert "rip->ri_name" at line 1136, seeded with the
  first five bytes of the string at 0xc00000000151a390, then `board_to_path`), and, when the board exists, zeroes the
  eight 8-byte slots after the first of the board-specific block the board's pointer leads to. Then for ports 1 to 8
  it sets bit (port - 1) of ri +8 when bit 37 + 2 x (port - 1) of the stat word is set, calls
  `router_port_init(ri, port)` (address 0xc000000000042edc; a nonzero result logs "Router port init failed, cnode %d,
  %v, p 0x%x, err %d <%s>" and is returned) and clears ri +599 + port. After the ports it calls `router_set_fences`,
  `eeprom_vertex_info_set(0x200, nasid, vertex, ri +24, 0)`, stores 7 in ri +0, calls `build_router_invent_info(ri)`
  and `sndrv_attach(vertex)`, copies the kernel word `router_poll_ticks` to the word
  `p_routertick` and returns 0. When the board lookup fails it logs "Couldn't locate board structure for %v" and goes
  on to the port loop with no board. The router register fields beyond the bits named above: unknown here. Inferred.
- `router_print_state_specific` (`6.5.30/unix.IP35`, address 0xc000000000043c64, 504 bytes): prints the error state of
  one router, but only once for each requester. `a0` is the router state record, `a1` the opaque first argument of the
  print routine, `a2` the print routine (called as `a2(a1, format, ...)`) and `a3` a one-byte requester mask. It takes
  the record's lock at ri +572 (`mutex_spinlock`). If `a3` shares a bit with the byte at ri +568 (`ri_print`) it
  releases the lock and returns, doing nothing; otherwise it ORs `a3` into that byte, releases the lock and goes on.
  When the byte at +0x8594 of the node's per-node block is nonzero (the block is reached through the pointer at
  absolute address -15360, indexed by the halfword at ri +2; the byte says the vector unit is busy) it prints "vector
  unit busy for cnode %d.Waiting....", calls `timeout_poll` (address 0xc000000000043a94) for the node's NASID with
  0x60c020, 0x8000000000000000 and a count of 0x100, and clears that byte. It then calls an unnamed static routine at
  0xc000000000043b1c with the record (the printing continues only when it returns 0), prints "ROUTER ERROR STATE:" and
  the line "/hw/module/%M/" with the module number from ri +504, reads router register 0 with `router_reg_read`
  (address 0xc0000000000427ec) and passes it, with the port mask byte ri +8, to `show_rr_status_rev_id` (address
  0xc000000000057590). Then, for each of ports 1 to 8 whose bit is set in the mask at ri +8, it reads that port's
  register at (port index + 2) & 1 shifted left 11, plus ((port index + 2) & 0xe) shifted left 15, plus 8, and calls
  `show_rr_error_status` (address 0xc0000000000573a0) with the low 34 bits of it, the port number, `a1`, `a2` and 0.
  The routine returns nothing. What the register at the offset formed above holds is unknown here. Established.

## Port register fences and history

- `router_set_fences` (`6.5.30/unix.IP35`, address 0xc00000000004314c, 312 bytes, 80-byte frame): trims the router's
  register 0x10 and each live port's own register so that they refer only to ports that came up. `a0` is the router
  state record (`ri`); its byte at +8 is the mask of live ports, bit (port - 1) set for each live port 1 to 8, as
  `router_init` builds it. It reads register 0x10 into the doubleword at 0(sp) with
  `router_reg_read(ri, 0x10, &value)` (0xc0000000000427ec). The results of the reads and writes are not tested. For each
  port p from 1 to 8 (`s4`, with the bit index `s2` = p - 1 and `s3` = p + 1): when bit p - 1 of the mask is clear, it
  clears bit p - 1 of the value at 0(sp) and goes on to the next port. When the bit is set it forms the register number
  `((s3 & 0xe) << 15) | ((s3 & 1) << 11) | 0x18`,
  reads that register into the doubleword at 8(sp), clears in it bit q for every q from 0 to 7 whose mask bit q is
  clear (an inner loop of eight passes, reading the mask byte at +8 each time), and writes the result back with
  `router_reg_write(ri, number, value)` (0xc00000000004281c). After the eighth port it stores the final value at 0(sp)
  as the doubleword at ri +536, writes it to register 0x10 with `router_reg_write` and returns that call's result
  (0xc00000000004327c). What the bits of register 0x10 and of the per-port registers mean to the router: unknown here.
  Established.
- `router_port_hist_reselect` (`6.5.30/unix.IP35`, address 0xc000000000042f90, 268 bytes, 80-byte frame): changes the
  two-bit field in bits 19 and 18 of one router port's register and clears that port's registers at +0x20 and +0x28, doing
  the register accesses between a `setnoderun` and a `restorenoderun` call. `a0` is the router state record (its halfword at +2 is
  the compact node number), `a1` the port number and `a2` the new field value (saved at 40(sp)). It finds the node's
  private data through the doubleword at -15360 (a table of pointers indexed by node) and reads the halfword at +34198
  (0x8596) of it, the first-cpu number; when it is -1 the routine returns 22 (set in the delay slot at
  0xc000000000042fd4). Otherwise it calls `setnoderun(node)` (0xc00000000004b948) and
  keeps its result (the body of `setnoderun` was not read): the whole value at 0(sp), its upper word at 8(sp) and its bits 31 to 16 as a halfword at 12(sp). The
  port's base register number is `((n & 0xe) << 15) | ((n & 1) << 11)` for n = a1 + 1, formed in `s1`. It reads that
  register with `router_reg_read(ri, number, &value)` (0xc0000000000427ec; the result is not tested), clears bits 19 and
  18 of the value (the mask is the complement of 0xc0000, from `lui` and `nor`), ORs in `a2 << 18` and writes it back with
  `router_reg_write(ri, number, value)` (0xc00000000004281c), keeping that call's result at 24(sp). It then writes 0 to
  register `number | 0x20` and to `number | 0x28`, ignoring both results. It ends with `restorenoderun` (0xc00000000004ba50)
  on the doubleword at 8(sp), whose bytes 14 and 15 this routine never stores, and returns the result of the first
  write (exit 0xc000000000043094). What the two-bit field selects in the router hardware: unknown here. Established.
- `router_port_init` (`6.5.30/unix.IP35`, address 0xc000000000042edc, 180 bytes, 48-byte frame): brings one router
  port's registers to their starting state and returns the result of the first register write. `a0` is the router
  state record (`ri`) and `a1` the port number, 1 to 8 as `router_init` passes it. The port's base register number is
  `((n & 0xe) << 15) | ((n & 1) << 11)` for n = `a1` + 1 (kept in `s1`). It reads that register with
  `router_reg_read(ri, number, &value)` (0xc0000000000427ec), clears bits 19 and 18 of the value (the mask is the
  complement of 0xc0000, from `lui` and `nor`), sets 0x140000 in it (bits 20 and 18) and writes it back with
  `router_reg_write(ri, number, value)` (0xc00000000004281c), keeping that call's result in `s8`. It then writes 0
  to register `number | 0x20` and to `number | 0x28`, and reads register `number | 0x88` into the stack slot at 0(sp),
  which it does not use. No result but the first write's is tested or returned (exit 0xc000000000042f88). Which port
  state the bits 20 and 18-19 select, and what the register at |0x88 holds: unknown here. Established.

## Vector unit register access

- `hub_vector_read` (`6.5.30/unix.IP35`, address 0xc0000000000473c0, 240 bytes, 80-byte frame): reads a register
  of a router through the Hub's vector (router-access) unit, but only when run on the target node. `a0` is the compact
  node number, `a1` the router identifier, `a2`, `a3` and `a4` further arguments handed on unchanged; `router_reg_read`
  calls it. It returns -10 (the `li s0,-10` in the delay slot at 0xc000000000047424) when the processor running it is
  not on node `a0`, otherwise the result of `vector_read_node`; the single exit is 0xc00000000004744c. It finds the
  node's data block as the doubleword at index `a0` of the table that the pointer at absolute address -15360 reaches,
  and the node's NASID as the halfword `compact_to_nasid_node[a0]` (the table at 0xc000000001647000), kept at 32(sp). Unless
  `in_panic_mode()` is nonzero it takes the block's lock at +0x8590 with `mutex_spinlock` (saving the level at 0(sp)).
  It compares the halfword at absolute address -15318 (the processor's own node number) with `a0`; on a match it
  sets the byte at block +0x8594 to 1 (in the delay slot of the call), calls `vector_read_node(a1, NASID, a2, a3, a4)` (0xc000000000046f34) and
  clears that byte after the call returns (the store is in the delay slot of the branch at the join). It then calls `in_panic_mode()` again and, when it is zero,
  releases the lock with `mutex_spinunlock(block + 0x8590, saved level)`. The busy byte at +0x8594 is the one
  `router_print_state_specific` tests. Which of the router's registers `a1` and the other arguments select: unknown here.
  Established.
- `hub_vector_write` (`6.5.30/unix.IP35`, address 0xc0000000000474b0, 240 bytes, 80-byte frame): the write
  counterpart of `hub_vector_read`, with the same shape: the same lock, the same node test and the same busy byte,
  and a different worker. `a0` is the compact node number, `a1` the router identifier and `a2`, `a3` and `a4` are
  handed on unchanged; `router_reg_write` calls it. It returns -10 (the `li s0,-10` in the branch delay slot at
  0xc000000000047514) when the processor running it is not on node `a0`, otherwise the result of
  `vector_write_node` (0xc000000000046ccc); the single exit is 0xc000000000047538. The node's data block comes from the
  table at the pointer at absolute address -15360 (index `a0`) and its NASID from `compact_to_nasid_node[a0]` (0xc000000001647000). Unless
  `in_panic_mode()` is nonzero it takes the block's lock at +0x8590 (`mutex_spinlock`, level saved at 0(sp)). When the
  halfword at absolute address -15318 equals `a0` it sets the byte at block +0x8594 to 1 (in the delay slot of the call), calls
  `vector_write_node(a1, NASID, a2, a3, a4)` and clears the byte after the call returns (the store is in the delay slot of the branch at the join). It then calls
  `in_panic_mode()` again and, when that is zero, releases the lock with `mutex_spinunlock(block + 0x8590, level)`.
  Established.
