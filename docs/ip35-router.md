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
  line 1131 of "../ml/SN/router.c"), finds the router's board with `vertex_to_router_board(nasid, vertex)` into ri
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
