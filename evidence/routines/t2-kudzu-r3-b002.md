# Tier-2 routine readings, kudzu round 3, batch 2

Role statements for targets 50 to 74 of the ranked tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Names come from the binary's symbol table and are not checked
against any other source. Hub registers appear as the uncached `0x92...` and
`0x90...` addresses the facts report.

- `hub_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000003bd5c
  **Inferred.** Role (from name): connects one interrupt for a hub-side device from a record passed in a0. It reads a halfword at +0x24 and words at +0x28 and +0x20, and it stores doublewords at +0x18 and +0x10 and a word at +0x2c. It calls intr_connect_level [0xc000000000038108] at 0xc00000000003bd94 with the level from +0x24 and the values from +0x28 and +0x20 as its second and third arguments. When that call's result is negative (branch at 0xc00000000003bd9c) the routine takes the early exit; otherwise it stores the doublewords, and when a3 is nonzero (branch at 0xc00000000003bda8) it makes a second call at 0xc00000000003bdd4 to a callee the facts do not resolve.
  Reviewed: 2026-11-12 (sonnet)

- `hub_intr_disconnect` in `6.5.22/unix.IP27` at 0xc00000000003bde4
  **Inferred.** Role (from name): the counterpart of hub_intr_connect. It takes a record in a0. When the doubleword at +0x18 is nonzero (branch at 0xc00000000003be04) it goes to a call at 0xc00000000003be38 to a callee the facts do not resolve, passing a0, the word at +0x28, a2 and that doubleword. When it is zero it calls intr_disconnect_level [0xc0000000000383a4] at 0xc00000000003be0c with the halfword at +0x24 as the first argument and the word at +0x28 as the second, and the return at 0xc00000000003be30 is the constant -0x5. The facts do not say what -0x5 means.
  Reviewed: 2026-11-12 (sonnet)

- `hub_intr_cpu_get` in `6.5.22/unix.IP27` at 0xc00000000003be48
  **Inferred.** A three-load lookup: it reads a halfword at +0x24 of its argument, then loads a doubleword at 0xc0000000014d39a8 and returns the word at +0x734 of the object that value points to (the return at 0xc00000000003be5c). The facts show no use of the halfword in the later loads, so what key the lookup uses is unclear from these facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_provider_startup` in `6.5.22/unix.IP27` at 0xc00000000003be6c
  **Inferred.** Brings up the hub provider in three calls: hub_pio_init [0xc00000000003b130] at 0xc00000000003be78, hub_dma_init [0xc00000000003b7c0] at 0xc00000000003be80, and hub_intr_init [0xc00000000003b9a0] at 0xc00000000003be88. The first passes all four argument registers unchanged, and the return at 0xc00000000003be98 is the result of hub_intr_init. The order of the three steps is the only sequencing the facts show.
  Reviewed: 2026-11-12 (sonnet)

- `hub_provider_shutdown` in `6.5.22/unix.IP27` at 0xc00000000003bea0
  **Inferred.** A single pass-through: it calls xtalk_provider_unregister [0xc00000000006df40] at 0xc00000000003bea8 with its four argument registers unchanged and returns that call's result at 0xc00000000003beb4. The name pairs it with hub_provider_startup, which sets the same provider up.
  Reviewed: 2026-11-12 (sonnet)

- `hub_check_is_widget0` in `6.5.22/unix.IP27` at 0xc00000000003bebc
  **Inferred.** Returns 0x1 at 0xc00000000003bf28 on one path and 0x0 or 0x1 at 0xc00000000003bf30 on the other. The unsigned compare of a0 against the value table(0x9200000000000000) at 0xc00000000003bf04 and the tests of a5, a3 and a4 at 0xc00000000003bf0c, 0xc00000000003bf14 and 0xc00000000003bf1c choose the path. The name suggests it asks whether a0 is the widget-zero address, but the facts do not show that address being compared.
  Reviewed: 2026-11-12 (sonnet)

- `hub_check_window_equiv` in `6.5.22/unix.IP27` at 0xc00000000003bf38
  **Inferred.** Asks hub_check_is_widget0 [0xc00000000003bebc] about two arguments: the call at 0xc00000000003bf48 passes all four argument registers, and the call at 0xc00000000003bf58 passes a1 as its first argument. When the first answer is zero the branch at 0xc00000000003bf50 goes to 0xc00000000003bf74, and the second answer is tested the same way at 0xc00000000003bf60. The return at 0xc00000000003bf98 is 0x0 or 0x1, set by a comparison at 0xc00000000003bf84.
  Reviewed: 2026-11-12 (sonnet)

- `hub_check_pci_equiv` in `6.5.22/unix.IP27` at 0xc00000000003bfa0
  **Inferred.** Calls hub_check_window_equiv [0xc00000000003bf38] at 0xc00000000003bfb4 with all four argument registers. A zero result branches at 0xc00000000003bfbc to 0xc00000000003c01c. On a nonzero result a further comparison at 0xc00000000003bfd4 decides between the return at 0xc00000000003bfec and the call to check_nasid_equiv [0xc00000000001f6d0] at 0xc00000000003c00c, which is made with a2 set to 0x0. The return at 0xc00000000003bfec is 0x0 or the result of check_nasid_equiv.
  Reviewed: 2026-11-12 (sonnet)

- `hub_setup_prb` in `6.5.22/unix.IP27` at 0xc00000000003c028
  **Inferred.** Loads a doubleword at offset +0x0 (0xc00000000003c078) and stores a doubleword at offset +0x0 (0xc00000000003c0b8); the facts here do not resolve the base address of either. It is gated by a word at 0xc0000000014c9c9c (branch at 0xc00000000003c030) and by a5 (branch at 0xc00000000003c054). The callers hub_set_piomode and hub_widget_flags_set pass small constants in a2 and a3 (and hub_set_piomode passes its own a1 in a3 at two calls), but what value this routine writes is not shown in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_set_piomode` in `6.5.22/unix.IP27` at 0xc00000000003c0bc
  **Inferred.** Gets the console NASID from get_console_nasid [0xc00000000001f6c0] at 0xc00000000003c0d0 and compares it with a0 at 0xc00000000003c0e4; when they are equal that branch goes to 0xc00000000003c184, where it calls putbuf_lock [0xc0000000001fac94], and it calls putbuf_unlock [0xc0000000001fae08] at 0xc00000000003c15c. It calls hub_setup_prb [0xc00000000003c028] at 0xc00000000003c11c with a3=0x2, at 0xc00000000003c134 with a2=0x3, and at 0xc00000000003c1a0 with a2=0x3. The doubleword loads and stores at 0xc00000000003c0f8 and 0xc00000000003c150 use addresses built from a0 by shifting it left 32 bits.
  Reviewed: 2026-11-12 (sonnet)

- `hub_widget_flags_set` in `6.5.22/unix.IP27` at 0xc00000000003c1b0
  **Inferred.** Calls hub_setup_prb [0xc00000000003c028] at 0xc00000000003c1c8 with a2=0x3 and a3=0x1, and again at 0xc00000000003c1e4 with a2=0x3 and a3=0x2, passing a0 and a1 through. The first call runs only when bit 0 of a2 is set (branch at 0xc00000000003c1bc) and the second only when a4 is nonzero (branch at 0xc00000000003c1d8). It returns the constant 0x1 at 0xc00000000003c1f4.
  Reviewed: 2026-11-12 (sonnet)

- `hub_device_flags_set` in `6.5.22/unix.IP27` at 0xc00000000003c1fc
  **Inferred.** Looks up a widget's info and master, then calls hub_widget_flags_set [0xc00000000003c1b0] at 0xc00000000003c260 between a mutex_spinlock [0xc000000000287214] call at 0xc00000000003c244 and a mutex_spinunlock [0xc000000000287260] call at 0xc00000000003c274. The lookups are xwidget_info_get [0xc00000000006dcb0] at 0xc00000000003c208, xwidget_info_id_get [0xc00000000006dd94] at 0xc00000000003c214, xwidget_info_master_get [0xc00000000006ddc8] at 0xc00000000003c220, and hwgraph_info_get_LBL [0xc000000000296160] at 0xc00000000003c234. It returns the result of hub_widget_flags_set at 0xc00000000003c288.
  Reviewed: 2026-11-12 (sonnet)

- `hub_device_inquiry` in `6.5.22/unix.IP27` at 0xc00000000003c290
  **Inferred.** Role (from name): looks up a device in the hardware graph and prints about it. It formats a name with sprintf [0xc0000000001f8e8c] at 0xc00000000003c2b0 and calls hwgraph_traverse [0xc0000000002965ec] at 0xc00000000003c2c0. When that result is zero (branch at 0xc00000000003c2c8) it gets the master with device_master_get [0xc000000000297a3c] at 0xc00000000003c2dc, and when that result is not -0x1 (branch at 0xc00000000003c2e8) it reads a label through hwgraph_info_get_LBL at 0xc00000000003c2f8. Six cmn_err [0xc0000000001f8090] calls with a0=0x6 follow, at 0xc00000000003c340, 0xc00000000003c354, 0xc00000000003c38c, 0xc00000000003c3cc, 0xc00000000003c404 and 0xc00000000003c42c. Branches at 0xc00000000003c3a0 and 0xc00000000003c3b8 loop while s1 is below 0x8, and a branch at 0xc00000000003c37c tests (t8 & t9). The return at 0xc00000000003c2d4 is one of four values.
  Reviewed: 2026-11-12 (sonnet)

- `hub_slotbits_to_slot` in `6.5.22/unix.IP27` at 0xc00000000003c7ec
  **Inferred.** Role (from name): maps a slot bit index to a slot number. It reads a byte at abs 0xffffffffffffc523 and a word at abs 0x90000000100000a8, compares a3 against 0x8 at 0xc00000000003c824 (a3 >= 0x8 goes to the return at 0xc00000000003c808, which is 0xf0 or an unresolved value), and tests bit 0 of the word at 0xc00000000003c834. The other two returns, at 0xc00000000003c840 and 0xc00000000003c850, are bytes from tables at 0xc0000000014164e3 and 0xc0000000014164c3. The facts do not show how those tables are indexed.
  Reviewed: 2026-11-12 (sonnet)

- `hub_slot_to_widget` in `6.5.22/unix.IP27` at 0xc00000000003c858
  **Inferred.** Role (from name): maps a slot number to a widget number. It reads a word at abs 0x9000000010000090 at 0xc00000000003c870 and a byte from one of three tables (0xc0000000014c8fe7, 0xc0000000014164ff and 0xc00000000141650f). A compare of a4 with 0x8 at 0xc00000000003c86c splits the slots, a test of that word at 0xc00000000003c888 chooses a path, and a compare of a value with 0x50 at 0xc00000000003c880 picks between the returns at 0xc00000000003c8a4 and 0xc00000000003c8b8. The return at 0xc00000000003c894 is 0xff or a table byte.
  Reviewed: 2026-11-12 (sonnet)

- `get_my_slotid` in `6.5.22/unix.IP27` at 0xc00000000003ca64
  **Inferred.** Role (from name): returns a slot number. It loads a doubleword at offset +0x0 at 0xc00000000003ca68 (the facts here do not resolve its base address) and calls hub_slotbits_to_slot [0xc00000000003c7ec] at 0xc00000000003ca74, passing a1, a2 and a3 through unchanged; the first argument of that call is not shown in the facts. It returns that call's result at 0xc00000000003ca80. That the loaded value is the slot bits is inferred from the names.
  Reviewed: 2026-11-12 (sonnet)

- `hub_slot_get` in `6.5.22/unix.IP27` at 0xc00000000003cbe0
  **Inferred.** A thin wrapper: it calls get_my_slotid [0xc00000000003ca64] at 0xc00000000003cbe8 with all four argument registers unchanged and returns a value at 0xc00000000003cbfc that the facts do not resolve.
  Reviewed: 2026-11-12 (sonnet)

- `module_lookup` in `6.5.22/unix.IP27` at 0xc00000000003fde8
  **Inferred.** Searches a linked list for a matching entry. The list count is read from 0xc0000000014d1500, and a count of zero or less skips the loop (branch at 0xc00000000003fdf0). Each pass compares an entry field with a0 at 0xc00000000003fe0c, and the loop continues while the next pointer in v1 is nonzero (branch at 0xc00000000003fe1c). The return at 0xc00000000003fe28 is 0x0 when nothing matches.
  Reviewed: 2026-11-12 (sonnet)

- `cnodemodule_lookup` in `6.5.22/unix.IP27` at 0xc00000000003fea0
  **Inferred.** A search loop like module_lookup, bounded by the count at 0xc0000000014d1500: a count of zero or less goes to the 0x0 return at 0xc00000000003ff00 (branch at 0xc00000000003fea8). It compares an entry field with a0 at 0xc00000000003fedc (a match goes to the return at 0xc00000000003ff08, whose value is unresolved), tests a4 at 0xc00000000003fee4, then compares a5 with the count at 0xc00000000003fef8 to drive the loop. The name suggests a compute-node module, but the facts show only the loop.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_invent_alloc` in `6.5.22/unix.IP27` at 0xc0000000000404d0
  **Inferred.** Role (from name): allocates an inventory record. It calls kern_malloc [0xc000000000181da4] at 0xc0000000000404e0 with arg2 as its first argument. If the result is nonzero (branch at 0xc0000000000404e8) it stores a halfword at 0xc000000000040514, a word at 0xc000000000040530 and a halfword at 0xc000000000040538, at offsets +0x0, +0x8 and +0x2. Before those stores it loads doublewords at 0xc0000000000404fc, 0xc000000000040500, 0xc00000000004050c, 0xc000000000040518, 0xc00000000004051c and 0xc000000000040524, a halfword at +0x49aa (0xc000000000040510) and a byte at +0x49a8 (0xc00000000004052c). The return at 0xc00000000004053c is the pointer or 0x0.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_cpu_invent_info` in `6.5.22/unix.IP27` at 0xc000000000040544
  **Inferred.** Role (from name): fills an inventory record for a CPU. It allocates a 0x24-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc00000000004056c and branches at 0xc000000000040578 to an exit when the allocation is zero. It stores about eight fields taken from arg2 into the record, at offsets from 0x4 to 0x20, and reads an environment variable with ip27log_getenv [0xc000000000023970] at 0xc0000000000405e0. It registers the record with hwgraph_info_add_LBL [0xc0000000002960bc] at 0xc000000000040640 and hwgraph_info_export_LBL [0xc000000000296220] at 0xc000000000040650, and calls cmn_err [0xc0000000001f8090] at 0xc000000000040698 (a0=0x4) and 0xc0000000000406b4 (a0=0x0). The facts show a split at 0xc00000000004062c on the halfword at +0x38 of arg2 against 0x926.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_disabled_cpu_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000406c4
  **Inferred.** Role (from name): builds inventory records for a CPU the name suggests is disabled. It allocates a 0x24-byte record at 0xc0000000000406f0 and a 0x5c-byte record at 0xc00000000004070c with klhwg_invent_alloc [0xc0000000000404d0], each followed by a zero test (0xc0000000000406fc and 0xc000000000040714). It stores fields taken from arg2 at offsets up to 0x20 and at 0x50 to 0x58, the latter fitting only the 0x5c-byte record. It gets a diagnostic string with get_diag_string [0xc00000000004048c] at 0xc000000000040744 and formats it with sprintf [0xc0000000001f8e8c] at 0xc000000000040758. It registers both records through hwgraph_info_add_LBL and hwgraph_info_export_LBL at 0xc0000000000407ac through 0xc0000000000407dc and calls cmn_err [0xc0000000001f8090] at 0xc00000000004080c.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_baseio_inventory_add` in `6.5.22/unix.IP27` at 0xc00000000004081c
  **Inferred.** Adds a base I/O inventory record. It allocates a 0x20-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc000000000040840, passing 0x15 as its class argument, and then reads the NVRAM PROM version into a stack buffer through nvram_prom_version_get [0xc00000000004b034] at 0xc000000000040854. It stores words at +0x14, +0x10 and +0xc of the record (0xc000000000040850, 0xc000000000040864 and 0xc000000000040874), and registers it with hwgraph_info_add_LBL at 0xc000000000040878 and hwgraph_info_export_LBL at 0xc000000000040890. The facts show no zero test on the allocation before the stores.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_hub_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000408a8
  **Inferred.** Adds an inventory record for a hub. It allocates a 0x20-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc0000000000408cc, with 0xe as its class argument, and returns at 0xc000000000040964 if the allocation is zero (branch at 0xc0000000000408dc). It copies bytes from arg2 at +0x2 and +0x3 and a doubleword from +0x58 into the record, selects fields with compares of a3 at 0xc00000000004090c and 0xc000000000040920, and compares a value from 0xc0000000014d13a0 against 0x21 at 0xc00000000004092c. It registers the record through hwgraph_info_add_LBL at 0xc00000000004093c and hwgraph_info_export_LBL at 0xc00000000004094c, and prints with cmn_err [0xc0000000001f8090] at 0xc000000000040984 and 0xc0000000000409a4.
  Reviewed: 2026-11-12 (sonnet)

- `klhwg_mem_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000409b4
  **Inferred.** Role (from name): adds a memory inventory record. It allocates a 0x50-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc0000000000409e8, with 0x3 as its class argument, and it reads two environment variables with ip27log_getenv [0xc000000000023970] at 0xc000000000040a38 and 0xc000000000040a5c. It scans a string with strchr [0xc0000000001463b0] at 0xc000000000040b7c and walks the bytes at arg2 +0x1 and +0x2 in a loop from 0xc000000000040ab0 to 0xc000000000040c34, comparing a count against 0x100 at 0xc000000000040b00 and against 0xb4 at 0xc000000000040c2c. It registers the record through hwgraph_info_add_LBL at 0xc000000000040bdc and hwgraph_info_export_LBL at 0xc000000000040bf0, and calls cmn_err [0xc0000000001f8090] at 0xc000000000040c60 with a5=0xc3.
  Reviewed: 2026-11-12 (sonnet)
