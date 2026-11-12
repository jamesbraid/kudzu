# Tier-2 routine readings, kudzu round 3 batch 3

- `klhwg_add_hub` in `6.5.22/unix.IP27` at 0xc000000000040c8c
  **Inferred.** It registers a hub vertex with hwgraph_path_add at 0xc000000000040cb0 under the path string at 0xc00000000140cc80, then sets the master with device_master_set at 0xc000000000040cbc and adds a label with hwgraph_info_add_LBL at 0xc000000000040cec. It fills in hub inventory through klhwg_hub_invent_info at 0xc000000000040d04 and init_hub_stats at 0xc000000000040d20, attaches a driver through sndrv_attach at 0xc000000000040d2c, and returns that attach result. A failed label add at 0xc000000000040cf4 is reported through cmn_err at 0xc000000000040d4c. The callee names point at hub bring-up in the hardware graph, but the facts do not show what the label holds.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_disabled_cpu` in `6.5.22/unix.IP27` at 0xc000000000040d5c
  **Inferred.** It reads a halfword at +0x1e and a byte at +0x18 from its third argument. When the halfword equals the constant -1 at 0xc000000000040d84, it skips to the return. Otherwise it formats a name with sprintf at 0xc000000000040da4, adds a path with hwgraph_path_add at 0xc000000000040db4, marks the vertex with mark_cpuvertex_as_cpu at 0xc000000000040dc0, sets the master, and calls klhwg_disabled_cpu_invent_info at 0xc000000000040de0. It returns -1 or that callee's result. The shape matches the enabled-CPU variant, but the facts do not show what the disabled state means for the CPU.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_cpu` in `6.5.22/unix.IP27` at 0xc000000000040dfc
  **Inferred.** It reads CPU fields from its third argument at +0x1e and +0x1c, indexes a table at 0xc0000000014d39a8 with offsets up to +0x738, and formats a vertex name with sprintf at 0xc000000000040e50. It adds the path, marks the vertex as a CPU, adds a label, sets the master, and looks up an edge with hwgraph_edge_get at 0xc000000000040eb0. When that call returns nonzero (the fall-through of the branch at 0xc000000000040eb8) it calls klhwg_cpu_invent_info at 0xc000000000040ec8, formats a string with sprintf at 0xc000000000040edc, sizes it with strlen and allocates with kmem_alloc_node at 0xc000000000040ef4, and, when a table pointer at +0x690 is nonzero (branch at 0xc000000000040f28), copies the string with strcpy at 0xc000000000040f48 and returns its result. When the lookup returns zero it instead formats with sprintf at 0xc000000000040f78 and adds an edge with hwgraph_edge_add at 0xc000000000040f88. One path reaches an assertion call at 0xc000000000040fa4 with the constant 0x221 and the table pointer as an argument; the facts do not show which branch leads there. The table fields are not named by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_rps` in `6.5.22/unix.IP27` at 0xc000000000040fb4
  **Inferred.** A branch at 0xc000000000040fcc compares a1 with -1 and goes to 0xc000000000040fe8 when they are equal. It adds a path with hwgraph_path_add at 0xc000000000040fdc under the string at 0xc00000000140cd20; when that call returns zero it sets the master at 0xc000000000040ffc and allocates a record with klhwg_invent_alloc at 0xc00000000004100c using the constants 0x17 and 0x10. When the allocation returns nonzero and a2 is nonzero (branches at 0xc000000000041018 and 0xc000000000041020) it stores a word with sw at +0xc and a byte with sb at +0x4, adds the record as a label at 0xc000000000041038 and exports the label at 0xc000000000041048 with size 0x10; another sb at +0x4 at 0xc00000000004105c is on the path where a2 is zero. The facts do not show the base register of these stores. Role (from name): a router port registration, a guess from the name; the facts do not settle it or what the stored fields hold.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_update_rps` in `6.5.22/unix.IP27` at 0xc000000000041060
  **Inferred.** It loads a pointer from the global base at -0x3c00, indexes a pointer table by its first argument scaled by 8, and reads a word at +0x5150 from the record it finds. It looks up an existing edge with hwgraph_edge_get at 0xc000000000041098 under the path string at 0xc00000000140cd20, and reads a label with hwgraph_info_get_LBL at 0xc0000000000410bc. It writes two bytes through a pointer the facts do not resolve, at 0xc0000000000410e0 and 0xc0000000000410e8. It appears to refresh the state of a router port record that klhwg_add_rps created, though the facts do not show which field the bytes hold.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_xbox_rps` in `6.5.22/unix.IP27` at 0xc0000000000410ec
  **Inferred.** It has the same shape as klhwg_add_rps at 0xc000000000040fb4: a branch at 0xc000000000041104 comparing a1 with -1, a path add at 0xc000000000041114 under the string at 0xc00000000140cd28, a master set at 0xc000000000041134, and a 16-byte record from klhwg_invent_alloc at 0xc000000000041144. As there, it stores a word with sw at +0xc and a byte with sb at +0x4 when the allocation result and a2 are nonzero, with a second sb at +0x4 at 0xc000000000041194 on the path where a2 is zero (the facts do not show the base register), and adds and exports the label at 0xc000000000041170 and 0xc000000000041180. The only difference the facts show is the path string. Role (from name): probably the same registration for a different port kind, which the facts do not name.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_update_xbox_rps` in `6.5.22/unix.IP27` at 0xc000000000041198
  **Inferred.** It follows the pattern of klhwg_update_rps at 0xc000000000041060: when a0 is not -1 it indexes the same pointer table from the global base at -0x3c00 and reads a word at +0x5150 from the record it finds. It looks up the edge under the path string at 0xc00000000140cd28 with hwgraph_edge_get at 0xc0000000000411d0; when that returns zero it reads a label with hwgraph_info_get_LBL at 0xc0000000000411f4, and when that returns zero it stores a byte at 0xc000000000041214 (skipped when a1 is zero, branch at 0xc000000000041208) and a byte at 0xc00000000004121c through pointers the facts do not resolve. Role (from name): the refresh step for the xbox variant of the port record; the facts do not show which field the bytes hold.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_mem` in `6.5.22/unix.IP27` at 0xc000000000041220
  **Inferred.** It adds a path with hwgraph_path_add at 0xc000000000041244 under the string at 0xc00000000140cd38, and sets the master with device_master_set at 0xc000000000041260. It then forwards its second, third and fourth arguments to klhwg_mem_invent_info at 0xc000000000041278. The routine returns the path add result when it fails, and the memory info result otherwise. The facts do not show what the memory info records contain.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_xbow` in `6.5.22/unix.IP27` at 0xc000000000041288
  **Inferred.** It is 532 bytes. It finds a component with find_component at 0xc0000000000412d4, creates a vertex with hwgraph_vertex_create at 0xc0000000000412e4 and initializes it with xswitch_vertex_init at 0xc0000000000412ec. It then loops over slot numbers below 0x10 (branch at 0xc00000000004133c), calls hub_slot_to_crossbow at 0xc00000000004137c, and adds edges with hwgraph_edge_add at 0xc000000000041324. Error paths call cmn_err at 0xc000000000041434, 0xc000000000041454 and 0xc000000000041480. The facts show the loop bound but not every condition inside the loop.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_node` in `6.5.22/unix.IP27` at 0xc0000000000414a4
  **Inferred.** It finds the node board with find_lboard at 0xc00000000004152c for class 0x11, finds its first component with find_first_component at 0xc000000000041550, and then adds memory through klhwg_add_mem at 0xc000000000041570. It adds the node and subnode vertices and their paths, looks up the module with module_lookup at 0xc000000000041730, and calls klhwg_add_cpu at 0xc00000000004177c, klhwg_add_disabled_cpu at 0xc0000000000417b4 and klhwg_add_hub at 0xc0000000000417f8. Failures go to cmn_err at 0xc000000000041814 and 0xc000000000041828, and an assertion at 0xc000000000041844 uses the constant 0x409. The routine returns the board pointer it found.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_all_xbows` in `6.5.22/unix.IP27` at 0xc000000000041878
  **Inferred.** It finds a board with find_lboard at 0xc0000000000418e4 and 0xc000000000041914 for class 0x11, then searches for class 0x41 from the previous result at 0xc000000000041928. A branch at 0xc000000000041930 goes back to 0xc0000000000418f4 when that search returns zero, so the facts show a retry loop over the class 0x11 boards. For the board it finds, it reads a halfword at +0x20 and calls klhwg_add_xbow at 0xc00000000004193c with that value; the facts list no branch back after that call. The routine returns the board pointer from the last search. The facts do not show how the caller chooses the boards.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_all_routers` in `6.5.22/unix.IP27` at 0xc000000000041970
  **Inferred.** It loops over a count held at 0xc0000000014d26a4, finding router boards with find_lboard_class at 0xc000000000041a1c and 0xc000000000041a3c for class 0x31. It matches names with strstr at 0xc000000000041ac0 and 0xc000000000041ad8 against the constants at 0xc00000000140cf80 and 0xc00000000140cee0, and parses a number after the match with atoi at 0xc000000000041bac. It adds the router's path with board_to_path at 0xc000000000041b84 and hwgraph_path_add at 0xc000000000041b94, and it logs failures through cmn_err at 0xc000000000041c0c, 0xc000000000041c28 and 0xc000000000041c50. The facts do not show the full name format it matches.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_connect_one_router` in `6.5.22/unix.IP27` at 0xc000000000041c70
  **Inferred.** It takes a router board from its second argument, resolves its path with board_to_path at 0xc000000000041ce4, and traverses to two vertices with hwgraph_traverse at 0xc000000000041cf4 and 0xc000000000041de0. It checks the lookups with is_specified against the string at 0xc0000000014d21c5, then adds an edge with hwgraph_edge_add at 0xc000000000041e34 under a name built with sprintf at 0xc000000000041e24. It loops with a counter compared against 7 (branch at 0xc000000000041d7c) and logs through cmn_err at 0xc000000000041e78 and 0xc000000000041ea4, with further cmn_err calls at 0xc000000000041ec0 and 0xc000000000041ed4. The facts do not name the two vertices it connects.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_connect_routers` in `6.5.22/unix.IP27` at 0xc000000000041ef4
  **Inferred.** It loops over the count at 0xc0000000014d26a4, finding router boards with find_lboard_class at 0xc000000000041f94 and 0xc000000000041fb4 for class 0x31. For each router it calls klhwg_connect_one_router at 0xc000000000041fd0 with the board pointer. It reads a word from the board at +0x0 at 0xc000000000041fd8 and a halfword at 0xc000000000042030 from a base the facts do not resolve. Its return is either an unresolved value or the board pointer from find_lboard_class at 0xc000000000041fb4.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_connect_hubs` in `6.5.22/unix.IP27` at 0xc000000000042034
  **Inferred.** It loops over the count at 0xc0000000014d26a4 and finds hub boards with find_lboard at 0xc000000000042104 for class 0x11, then a first component with find_first_component at 0xc000000000042114 for class 0x2. It builds two paths with board_to_path at 0xc000000000042154 and 0xc000000000042190, traverses to two vertices with hwgraph_traverse at 0xc000000000042164 and 0xc0000000000421a0, and adds an edge with hwgraph_edge_add at 0xc0000000000421b8 under the string at 0xc00000000140d070. It logs through cmn_err at 0xc00000000004220c, 0xc000000000042224 and 0xc000000000042248. The facts show an edge added between the two traversed vertices, but not what they are or which side is which.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_device_disable_hints_add` in `6.5.22/unix.IP27` at 0xc000000000042268
  **Inferred.** It calls device_admin_table_init at 0xc000000000042270 with its four arguments, then loops over a count at 0xc0000000014d269c with inner loops over loaded values. Under conditions on loaded bytes it gets a canonical component name with device_component_canonical_name_get at 0xc0000000000423ec and updates the admin table with device_admin_table_update at 0xc000000000042400, using the strings at 0xc00000000140d078 and 0xc00000000140d080. It also loads a halfword at 0xc00000000004245c from a base the facts do not resolve. Role (from name): it adds device disable hints; the facts do not show what the hints contain.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_all_modules` in `6.5.22/unix.IP27` at 0xc00000000004283c
  **Inferred.** It loops over a count at 0xc0000000014d1500. On each pass it formats a name with sprintf at 0xc000000000042884, adds a path with hwgraph_path_add at 0xc000000000042894, sets fast info with hwgraph_fastinfo_set at 0xc0000000000428a0, formats a second name with sprintf at 0xc0000000000428b4 and adds a second path at 0xc0000000000428c4. When the second path add returns zero it adds a label with the value 1 at 0xc0000000000428e0 and calls sndrv_attach at 0xc0000000000428e8; when it returns nonzero the branch at 0xc0000000000428cc goes to 0xc00000000004292c, next to an assertion call at 0xc000000000042930 that uses the constant 0x6e0. Role (from name): it registers modules in the hardware graph and attaches their drivers.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_add_all_nodes` in `6.5.22/unix.IP27` at 0xc000000000042940
  **Inferred.** It gets the node id with get_nasid at 0xc000000000042950 and reads a doubleword from the computed node address at +0xd0 at 0xc00000000004297c, passing it to klhwg_add_node at 0xc000000000042988 inside a loop bounded by the count at 0xc0000000014d269c (branch at 0xc0000000000429a4). A second loop with the same bound (branch at 0xc0000000000429d4) calls klhwg_add_all_xbows at 0xc0000000000429b8. After the loops it calls klhwg_add_all_routers at 0xc0000000000429e4, klhwg_connect_routers at 0xc0000000000429ec, klhwg_connect_hubs at 0xc0000000000429f4, router_guardians_set at 0xc0000000000429fc and klhwg_device_disable_hints_add at 0xc000000000042a04, and returns that last result. Role (from name): it builds the hardware graph for all nodes; the facts do not show that its call order is required.
  Reviewed: 2026-11-12 (sonnet)

- `hub_xtalk_link_up` in `6.5.22/unix.IP27` at 0xc000000000042a18
  **Inferred.** It forms an address from its first argument shifted left by 0x20 plus the constant -0x6dfffffffe3ffed8 and loads a doubleword from it at 0xc000000000042a24. The branch at 0xc000000000042a2c tests bit 0x2000 of that doubleword and the routine returns 0x0 or 0x1; the facts do not show which value goes with which side. Role (from name): a hub crosstalk link-up test; the facts do not show which hub register holds the bit.
  Reviewed: 2026-11-12 (sonnet)

- `query_fasttimer` in `6.5.22/unix.IP27` at 0xc0000000000455c8
  **Established.** It takes no action and returns the constant 0x320 (800) at 0xc0000000000455c8. The routine is a fixed value, so its behavior is fully settled, though the meaning of 800 as a timer quantity is not.
  Reviewed: 2026-11-12 (sonnet)

- `hub_rtc_init` in `6.5.22/unix.IP27` at 0xc000000000047b68
  **Inferred.** It gets a compact node id with get_compact_nodeid at 0xc000000000047b74 and compares it with its first argument (branch at 0xc000000000047b7c). When they are equal it loads a halfword at 0xc000000000047b90 from a base the facts do not resolve and stores four doublewords at 0xc000000000047ba4, 0xc000000000047ba8, 0xc000000000047bac and 0xc000000000047bb0, at offsets -0x6dfffffffe7ffec0, -0x6dfffffffe7ffeb8, -0x6dfffffffe7ffeb0 and -0x6dfffffffe7ffea8 from a base register the facts do not resolve. It returns the node id result or 0x1. Role (from name): a real-time clock setup for the hub; the stored values are not resolved by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `intrgroup_init` in `6.5.22/unix.IP27` at 0xc000000000049fc0
  **Inferred.** It stores a doubleword at 0xc000000000049fd8 and another at 0xc000000000049fe4, and calls spinlock_init at 0xc000000000049fe0 on 0xc0000000014dd2b0. It then loops: the branch at 0xc00000000004a018 goes back to 0xc000000000049fec while a counter is below 0x10. Inside the loop it stores a word at 0xc000000000049ff8 and doublewords at 0xc00000000004a000 and 0xc00000000004a00c through pointers the facts do not resolve, and calls spinlock_init at 0xc00000000004a008 on a pointer the facts do not resolve. Role (from name): it sets up lock-protected records for interrupt groups, a guess from the name and the lock calls.
  Reviewed: 2026-11-12 (sonnet)

- `hubdev_init` in `6.5.22/unix.IP27` at 0xc00000000004b6a0
  **Inferred.** It calls init_mutex at 0xc00000000004b6b4 on the global at 0xc0000000014dd3b8, with the name constant at 0xc00000000140ebb8 and the constant -1 as its last argument. It returns the init_mutex result. hubdev_register and hubdev_unregister take the mutex at the same address, but the facts do not show what that mutex guards.
  Reviewed: 2026-11-12 (sonnet)

- `hubdev_register` in `6.5.22/unix.IP27` at 0xc00000000004b6cc
  **Inferred.** It allocates a 16-byte block with kmem_zalloc at 0xc00000000004b6e0 and takes the mutex at 0xc0000000014dd3b8 with mutex_lock at 0xc00000000004b6f0. While the lock is held it stores two doublewords into the new block at offsets +0x8 and +0x0, at 0xc00000000004b704 and 0xc00000000004b710. It releases the mutex at 0xc00000000004b70c and returns the mutex_unlock result. The facts do not show the values stored or whether the block is linked into a list.
  Reviewed: 2026-11-12 (sonnet)

- `hubdev_unregister` in `6.5.22/unix.IP27` at 0xc00000000004b724
  **Inferred.** It takes the mutex at 0xc0000000014dd3b8 with mutex_lock at 0xc00000000004b738, passing its last two arguments on. It loads a doubleword from the global at 0xc0000000014c9088; when that is zero the branch at 0xc00000000004b744 goes to 0xc00000000004b768, which calls mutex_unlock. Otherwise it walks entries through pointers the facts do not resolve: the branch at 0xc00000000004b750 goes to 0xc00000000004b784 when an entry equals s0, and the branch at 0xc00000000004b760 continues the walk while the next pointer is nonzero and falls to the mutex_unlock at 0xc00000000004b768 when it is zero. On the match path it calls kmem_free at 0xc00000000004b78c with size 0x10, stores a doubleword at 0xc00000000004b790 and releases the mutex again at 0xc00000000004b794. The return is one of 0x0 and 0x2; the facts do not show which value goes with the not-found or the match path.
  Reviewed: 2026-11-12 (sonnet)
