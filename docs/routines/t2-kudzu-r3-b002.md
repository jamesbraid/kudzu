# Tier-2 routine readings, kudzu round 3, batch 2

Role statements for targets 50 to 74 of the ranked tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Names come from the binary's symbol table and are not checked
against any other source. Hub registers appear as the uncached `0x92...` and
`0x90...` addresses the facts report.

- `hub_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000003bd5c
  **Inferred.** Connects one interrupt for a hub-side device from a record passed in a0. It reads a halfword at +0x24 and words at +0x28 and +0x20, and it stores doublewords at +0x18 and +0x10 and a word at +0x2c. It calls intr_connect_level [0xc000000000038108] at 0xc00000000003bd94 with the level from +0x24 and the two values from +0x28 and +0x20, returns that call's result when it is negative (branch at 0xc00000000003bd9c), and otherwise makes a second indirect call at 0xc00000000003bdd4 when a3 is nonzero.

- `hub_intr_disconnect` in `6.5.22/unix.IP27` at 0xc00000000003bde4
  **Inferred.** The counterpart of hub_intr_connect: it takes the same record in a0 and calls intr_disconnect_level [0xc0000000000383a4] at 0xc00000000003be0c with the level from +0x24. A nonzero word at +0x18 (branch at 0xc00000000003be04) sends it to an indirect call at 0xc00000000003be38 that passes +0x28 and +0x18. The return at 0xc00000000003be30 is the constant -0x5, which reads as an error code.

- `hub_intr_cpu_get` in `6.5.22/unix.IP27` at 0xc00000000003be48
  **Inferred.** A three-load lookup: it reads a halfword at +0x24 of its argument, then loads a pointer from the table at 0xc0000000014d39a8 and returns the word at +0x734 of that object (the return at 0xc00000000003be5c). The halfword read at +0x24 is not used by the later loads, so the lookup key is unclear from these facts.

- `hub_provider_startup` in `6.5.22/unix.IP27` at 0xc00000000003be6c
  **Inferred.** Brings up the hub provider in three calls: hub_pio_init [0xc00000000003b130] at 0xc00000000003be78, hub_dma_init [0xc00000000003b7c0] at 0xc00000000003be80, and hub_intr_init [0xc00000000003b9a0] at 0xc00000000003be88. The first passes all four argument registers unchanged, and the return at 0xc00000000003be98 is the result of hub_intr_init. The order of the three steps is the only sequencing the facts show.

- `hub_provider_shutdown` in `6.5.22/unix.IP27` at 0xc00000000003bea0
  **Inferred.** A single pass-through: it calls xtalk_provider_unregister [0xc00000000006df40] at 0xc00000000003bea8 with its four argument registers unchanged and returns that call's result at 0xc00000000003beb4. The name pairs it with hub_provider_startup, which sets the same provider up.

- `hub_check_is_widget0` in `6.5.22/unix.IP27` at 0xc00000000003bebc
  **Inferred.** Returns 0x1 at 0xc00000000003bf28 on one path and 0x0 or 0x1 at 0xc00000000003bf30 on the other. The compare of a0 with a table bound at 0xc00000000003bf04 and the tests of a5, a3 and a4 at 0xc00000000003bf0c, 0xc00000000003bf14 and 0xc00000000003bf1c choose the path. The name suggests it asks whether a0 is the widget-zero address, but the facts do not show that address being compared.

- `hub_check_window_equiv` in `6.5.22/unix.IP27` at 0xc00000000003bf38
  **Inferred.** Asks hub_check_is_widget0 [0xc00000000003bebc] about two arguments: the call at 0xc00000000003bf48 passes all four argument registers, and the call at 0xc00000000003bf58 passes a1 as its first argument. When the first answer is zero the branch at 0xc00000000003bf50 goes to 0xc00000000003bf74, and the second answer is tested the same way at 0xc00000000003bf60. The return at 0xc00000000003bf98 is 0x0 or 0x1, set by a comparison at 0xc00000000003bf84.

- `hub_check_pci_equiv` in `6.5.22/unix.IP27` at 0xc00000000003bfa0
  **Inferred.** Calls hub_check_window_equiv [0xc00000000003bf38] at 0xc00000000003bfb4 with all four argument registers. A zero result branches at 0xc00000000003bfbc to 0xc00000000003c01c, and a nonzero result leads to check_nasid_equiv [0xc00000000001f6d0] at 0xc00000000003c00c, called with a2 set to 0x0. The return at 0xc00000000003bfec is 0x0 or the result of check_nasid_equiv.

- `hub_setup_prb` in `6.5.22/unix.IP27` at 0xc00000000003c028
  **Inferred.** Reads one doubleword at table address 0x9200000001c00198 (the load at 0xc00000000003c078) and stores a doubleword back to the same address at 0xc00000000003c0b8. It is gated by a word at 0xc0000000014c9c9c (branch at 0xc00000000003c030) and by a5 (branch at 0xc00000000003c054). The callers hub_set_piomode and hub_widget_flags_set pass a small index in a3, so the routine is a shared hub register setup, but what value it writes is not shown in the facts.

- `hub_set_piomode` in `6.5.22/unix.IP27` at 0xc00000000003c0bc
  **Inferred.** Gets the console NASID from get_console_nasid [0xc00000000001f6c0] at 0xc00000000003c0d0 and compares it with a0 at 0xc00000000003c0e4. It calls hub_setup_prb [0xc00000000003c028] at 0xc00000000003c11c with a3=0x2, at 0xc00000000003c134 with a2=0x3, and at 0xc00000000003c1a0 with a2=0x3, and it takes putbuf_lock [0xc0000000001fac94] at 0xc00000000003c184 and putbuf_unlock [0xc0000000001fae08] at 0xc00000000003c15c. The doubleword loads and stores at 0xc00000000003c0f8 and 0xc00000000003c150 use addresses built from a0 by shifting it left 32 bits.

- `hub_widget_flags_set` in `6.5.22/unix.IP27` at 0xc00000000003c1b0
  **Inferred.** Calls hub_setup_prb [0xc00000000003c028] at 0xc00000000003c1c8 with a2=0x3 and a3=0x1, and again at 0xc00000000003c1e4 with a2=0x3 and a3=0x2, passing a0 and a1 through. The first call runs only when bit 0 of a2 is set (branch at 0xc00000000003c1bc). It returns the constant 0x1 at 0xc00000000003c1f4.

- `hub_device_flags_set` in `6.5.22/unix.IP27` at 0xc00000000003c1fc
  **Inferred.** Looks up a widget's info and master, then calls hub_widget_flags_set [0xc00000000003c1b0] at 0xc00000000003c260 between a mutex_spinlock [0xc000000000287214] call at 0xc00000000003c244 and a mutex_spinunlock [0xc000000000287260] call at 0xc00000000003c274. The lookups are xwidget_info_get [0xc00000000006dcb0] at 0xc00000000003c208, xwidget_info_id_get [0xc00000000006dd94] at 0xc00000000003c214, xwidget_info_master_get [0xc00000000006ddc8] at 0xc00000000003c220, and hwgraph_info_get_LBL [0xc000000000296160] at 0xc00000000003c234. It returns the result of hub_widget_flags_set at 0xc00000000003c288.

- `hub_device_inquiry` in `6.5.22/unix.IP27` at 0xc00000000003c290
  **Inferred.** Walks the hardware graph to find a device and reports on it. It formats a name with sprintf [0xc0000000001f8e8c] at 0xc00000000003c2b0, looks it up with hwgraph_traverse [0xc0000000002965ec] at 0xc00000000003c2c0, gets the master with device_master_get [0xc000000000297a3c] at 0xc00000000003c2dc, and reads a label through hwgraph_info_get_LBL at 0xc00000000003c2f8. A run of cmn_err [0xc0000000001f8090] calls with a0=0x6, from 0xc00000000003c340 through 0xc00000000003c42c, prints the findings, and a loop at 0xc00000000003c370 to 0xc00000000003c3b8 tests bits of s1 against 0x8. The return at 0xc00000000003c2d4 is one of four values.

- `hub_slotbits_to_slot` in `6.5.22/unix.IP27` at 0xc00000000003c7ec
  **Inferred.** Maps a slot bit index to a slot number. It reads a byte at abs 0xffffffffffffc523 and a word at abs 0x90000000100000a8, compares a3 against 0x8 at 0xc00000000003c824, and tests bit 0 of the word at 0xc00000000003c834. The three returns are 0xf0 at 0xc00000000003c808 and two byte-table entries at 0xc00000000003c840 and 0xc00000000003c850, so the table is a fixed byte mapping of 0x8 or more slot bits.

- `hub_slot_to_widget` in `6.5.22/unix.IP27` at 0xc00000000003c858
  **Inferred.** Maps a slot number to a widget number. It reads a word at abs 0x9000000010000090 at 0xc00000000003c870 and a byte from one of three tables (0xc0000000014c8fe7, 0xc0000000014164ff and 0xc00000000141650f). A compare of a4 with 0x8 at 0xc00000000003c86c splits the slots, and a compare of a value with 0x50 at 0xc00000000003c880 picks between the returns at 0xc00000000003c8a4 and 0xc00000000003c8b8. The return at 0xc00000000003c894 is 0xff or a table byte.

- `get_my_slotid` in `6.5.22/unix.IP27` at 0xc00000000003ca64
  **Inferred.** Returns the slot number of the calling CPU's hub. It loads a doubleword from abs 0x9200000001220048 at 0xc00000000003ca68 and passes its argument registers on to hub_slotbits_to_slot [0xc00000000003c7ec] at 0xc00000000003ca74, returning that call's result at 0xc00000000003ca80. The first argument of that call is not shown in the facts, so the link from the hub register to the slot bits is inferred from the names.

- `hub_slot_get` in `6.5.22/unix.IP27` at 0xc00000000003cbe0
  **Inferred.** A thin wrapper: it calls get_my_slotid [0xc00000000003ca64] at 0xc00000000003cbe8 with all four argument registers unchanged and returns a value at 0xc00000000003cbfc that the facts do not resolve.

- `module_lookup` in `6.5.22/unix.IP27` at 0xc00000000003fde8
  **Inferred.** Searches a linked list for a matching entry. The list count is read from 0xc0000000014d1500, and a count of zero or less skips the loop (branch at 0xc00000000003fdf0). Each pass compares an entry field with a0 at 0xc00000000003fe0c, and the loop continues while the next pointer in v1 is nonzero (branch at 0xc00000000003fe1c). The return at 0xc00000000003fe28 is 0x0 when nothing matches.

- `cnodemodule_lookup` in `6.5.22/unix.IP27` at 0xc00000000003fea0
  **Inferred.** The same kind of list search as module_lookup, bounded by the count at 0xc0000000014d1500: a count of zero or less goes to the 0x0 return at 0xc00000000003ff00 (branch at 0xc00000000003fea8). It compares an entry field with a0 at 0xc00000000003fedc, then compares a5 with the count at 0xc00000000003fef8 to drive the loop. The name suggests a compute-node module, but the facts show only the list.

- `klhwg_invent_alloc` in `6.5.22/unix.IP27` at 0xc0000000000404d0
  **Inferred.** Allocates an inventory record. It indexes a table by a0 shifted left 3 (the loads at 0xc00000000004050c and 0xc000000000040524) and reads a halfword and a byte from that entry at 0xc000000000040510 and 0xc00000000004052c. It calls kern_malloc [0xc000000000181da4] at 0xc0000000000404e0 with arg2 as its first argument, and if the result is nonzero (branch at 0xc0000000000404e8) it stores a halfword at 0xc000000000040514, a word at 0xc000000000040530 and a halfword at 0xc000000000040538 into the new block. The return at 0xc00000000004053c is the pointer or 0x0.

- `klhwg_cpu_invent_info` in `6.5.22/unix.IP27` at 0xc000000000040544
  **Inferred.** Fills an inventory record for a CPU. It allocates a 0x24-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc00000000004056c and returns without filling it when the allocation is zero (branch at 0xc000000000040578). It copies about twenty fields from arg2 into the record, at offsets from 0x4 to 0x20, and reads an environment variable with ip27log_getenv [0xc000000000023970] at 0xc0000000000405e0. It registers the record with hwgraph_info_add_LBL [0xc0000000002960bc] at 0xc000000000040640 and hwgraph_info_export_LBL [0xc000000000296220] at 0xc000000000040650, and prints with cmn_err [0xc0000000001f8090] at 0xc000000000040698 and 0xc0000000000406b4.

- `klhwg_disabled_cpu_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000406c4
  **Inferred.** Builds the inventory records for a CPU the name suggests is disabled. It allocates a 0x24-byte record at 0xc0000000000406f0 and a 0x5c-byte record at 0xc00000000004070c with klhwg_invent_alloc [0xc0000000000404d0], both guarded by zero tests at 0xc0000000000406fc and 0xc000000000040714. It copies fields from arg2 into the first record, gets a diagnostic string with get_diag_string [0xc00000000004048c] at 0xc000000000040744, and formats it with sprintf [0xc0000000001f8e8c] at 0xc000000000040758. It registers both records through hwgraph_info_add_LBL and hwgraph_info_export_LBL at 0xc0000000000407ac through 0xc0000000000407dc, and reports through cmn_err [0xc0000000001f8090] at 0xc00000000004080c.

- `klhwg_baseio_inventory_add` in `6.5.22/unix.IP27` at 0xc00000000004081c
  **Inferred.** Adds a base I/O inventory record. It allocates a 0x20-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc000000000040840, passing 0x15 as its class argument, and then reads the NVRAM PROM version into a stack buffer through nvram_prom_version_get [0xc00000000004b034] at 0xc000000000040854. It stores words at +0x14, +0x10 and +0xc of the record (0xc000000000040850, 0xc000000000040864 and 0xc000000000040874), and registers it with hwgraph_info_add_LBL at 0xc000000000040878 and hwgraph_info_export_LBL at 0xc000000000040890. The facts show no zero test on the allocation before the stores.

- `klhwg_hub_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000408a8
  **Inferred.** Adds an inventory record for a hub. It allocates a 0x20-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc0000000000408cc, with 0xe as its class argument, and returns at 0xc000000000040964 if the allocation is zero (branch at 0xc0000000000408dc). It copies bytes from arg2 at +0x2 and +0x3 and a doubleword from +0x58 into the record, selects fields with compares of a3 at 0xc00000000004090c and 0xc000000000040920, and compares a value from 0xc0000000014d13a0 against 0x21 at 0xc00000000004092c. It registers the record through hwgraph_info_add_LBL at 0xc00000000004093c and hwgraph_info_export_LBL at 0xc00000000004094c, and prints with cmn_err [0xc0000000001f8090] at 0xc000000000040984 and 0xc0000000000409a4.

- `klhwg_mem_invent_info` in `6.5.22/unix.IP27` at 0xc0000000000409b4
  **Inferred.** Adds memory inventory records. It allocates a 0x50-byte record with klhwg_invent_alloc [0xc0000000000404d0] at 0xc0000000000409e8, with 0x3 as its class argument, and it reads two environment variables with ip27log_getenv [0xc000000000023970] at 0xc000000000040a38 and 0xc000000000040a5c. It scans a string with strchr [0xc0000000001463b0] at 0xc000000000040b7c and walks the bytes at arg2 +0x1 and +0x2 in a loop from 0xc000000000040ab0 to 0xc000000000040c34, comparing a count against 0x100 at 0xc000000000040b00 and against 0xb4 at 0xc000000000040c2c. It registers the record through hwgraph_info_add_LBL at 0xc000000000040bdc and hwgraph_info_export_LBL at 0xc000000000040bf0, and prints with cmn_err [0xc0000000001f8090] at 0xc000000000040c60 using the constant 0xc3.
