# Tier-2 routine readings, kudzu round 3, batch 6

Role entries for 24 crossbar and widget routines in the IP27 kernel build, read from
instruction facts rendered for each address. Each role states what the code does from
the calls, constants and branches at the cited addresses. Where the facts leave the
meaning open, the entry says so.

- `xwidget_register` in `6.5.22/unix.IP27` at 0xc00000000006e038
  **Inferred.** Allocates a 0x40-byte widget record with kmem_alloc at 0xc00000000006e078, then copies a device name from dev_to_name at 0xc00000000006e0bc into a second allocation through strcpy at 0xc00000000006e0e8. It stores the record with xwidget_info_set at 0xc00000000006e0f8, registers the device master with device_master_set at 0xc00000000006e108, and adds a connection point with cdl_add_connpt at 0xc00000000006e12c. A branch on a5 at 0xc00000000006e110 decides whether async_attach_add_info at 0xc00000000006e14c runs.

- `xwidget_unregister` in `6.5.22/unix.IP27` at 0xc00000000006e15c
  **Inferred.** Looks up the widget record with xwidget_info_get at 0xc00000000006e16c. When a record comes back, it removes the hwgraph inventory entry with hwgraph_inventory_remove at 0xc00000000006e1ac and the connection point with cdl_del_connpt at 0xc00000000006e1c4. It zeroes the record with blkclr at 0xc00000000006e1e4, frees the name string at 0xc00000000006e1d8 and the 0x40-byte record at 0xc00000000006e1f0 with kmem_free, and returns 0 or 1 at 0xc00000000006e190.

- `xwidget_error_register` in `6.5.22/unix.IP27` at 0xc00000000006e200
  **Inferred.** Fetches the widget record with xwidget_info_get at 0xc00000000006e210 and stores two 8-byte words into it at offsets 0x28 and 0x30 (stores at 0xc00000000006e218 and 0xc00000000006e230). It returns the record pointer in v0 at 0xc00000000006e22c. The facts do not show what the two stored words are, so the error-registration role rests on the name.

- `xwidget_reset` in `6.5.22/unix.IP27` at 0xc00000000006e234
  **Established.** Forwards its four incoming arguments unchanged to xswitch_reset_link at 0xc00000000006e23c and returns that call's result. The routine holds no other calls or branches.

- `xwidget_gfx_reset` in `6.5.22/unix.IP27` at 0xc00000000006e250
  **Inferred.** Calls xswitch_reset_link at 0xc00000000006e25c, then looks up the widget record with xwidget_info_get at 0xc00000000006e264. On a found record it takes the master and ID through xwidget_info_master_get at 0xc00000000006e278 and xwidget_info_id_get at 0xc00000000006e284, and passes them to hub_widget_reset at 0xc00000000006e294. A null lookup branches at 0xc00000000006e26c to a path that ends in the assertion call at 0xc00000000006e2b8 (constant 0x441). The branches do not settle which path reaches that assertion.

- `xwidget_name_get` in `6.5.22/unix.IP27` at 0xc00000000006e2c8
  **Inferred.** Returns the static pointer 0xc0000000003c6020 when its first argument equals -1, which the branch at 0xc00000000006e2d8 tests. Otherwise it looks up the widget record with xwidget_info_get at 0xc00000000006e2e0 and returns the name from xwidget_info_name_get at 0xc00000000006e2e8. The static pointer reads as a fallback name.

- `xbow_init` in `6.5.22/unix.IP27` at 0xc00000000006e658
  **Inferred.** Registers three crossbar widget drivers by calling xwidget_driver_register at 0xc00000000006e66c, 0xc00000000006e680 and 0xc00000000006e694. Each call passes a different first constant (0xd100, 0xd000 and 0x0), the shared argument pointer 0xc0000000003c6098 and the constant 1 in a3. It returns the last registration's result in v0 at 0xc00000000006e6a0. What the three constants identify is not derived here.

- `xbow_attach` in `6.5.22/unix.IP27` at 0xc00000000006e6a8
  **Inferred.** Attaches one crossbar widget. It allocates a 0x290-byte state block with kmem_zalloc at 0xc00000000006e700, adds a character device with hwgraph_char_device_add at 0xc00000000006e6f0, and sets up a device description and interrupt with xtalk_intr_alloc at 0xc00000000006e834 and xtalk_intr_connect at 0xc00000000006e850. The connect call uses the handler at 0xc00000000006f36c, and xwidget_error_register at 0xc00000000006e868 takes the error callback at 0xc00000000006f960. It then records switch info with xswitch_info_new at 0xc00000000006e878, registers the provider at 0xc00000000006e8f8, and returns 0 at 0xc00000000006e91c. A chain of compare-and-branch tests from 0xc00000000006e7a4 to 0xc00000000006e9bc sorts the widget type by small constants, and the facts do not show what each code selects.

- `xbow_open` in `6.5.22/unix.IP27` at 0xc00000000006e9c4
  **Inferred.** Calls cap_able_cred at 0xc00000000006e9d4 with the fixed capability bit 0x2000000000 and the caller's credential arguments, then returns 0 or 1 by the branches at 0xc00000000006e9dc and 0xc00000000006e9f0. Which return value means the open is allowed is not settled from these facts.

- `xbow_map` in `6.5.22/unix.IP27` at 0xc00000000006ea00
  **Inferred.** Fetches a per-vertex record with hwgraph_fastinfo_get at 0xc00000000006ea10, reads the 8-byte word at record offset 0x10 (ld at 0xc00000000006ea28), and calls v_mapphys at 0xc00000000006ea2c with the caller's a1, that word and the constant -0x4000 in a3. It returns the v_mapphys result. The mapping role comes from the callee name, and the meaning of the constant is not settled here.

- `xbow_widget_num_get` in `6.5.22/unix.IP27` at 0xc00000000006ea4c
  **Inferred.** Takes a vertex handle and finds the crossbar widget number behind it. It reads a 16-bit entry from the table at 0xc0000000014d3800 (lh at 0xc00000000006ea90), converts the vertex name with vertex_to_name at 0xc00000000006ea60, and matches two constant name fragments with strstr at 0xc00000000006eab4 and 0xc00000000006eac8. Candidates are checked through hwgraph_edge_get at 0xc00000000006eaec, xwidget_info_chk at 0xc00000000006eb20 and xwidget_info_id_get at 0xc00000000006eb38. The return is one of the hub_widget_id result, the xwidget_info_id_get result or 0xff. The exact name patterns are not decoded from these facts.

- `xbow_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006eb48
  **Inferred.** Dispatches on the command in a1 using compares against small constants and 0x6702 (branches at 0xc00000000006eb74, 0xc00000000006eb80, 0xc00000000006eb94 and 0xc00000000006eb9c). The arms call xbow_get_perf_counters at 0xc00000000006ebe4, xbow_get_llp_status at 0xc00000000006ec6c, xbow_disable_llp_monitor at 0xc00000000006ecf4, xbow_enable_perf_counter at 0xc00000000006ed94, xbow_enable_llp_monitor at 0xc00000000006eda8 and xbow_prio_bw_alloc. Each arm is gated by cap_able_cred or cap_able with capability 0x2000000000 and moves argument structures with copyin and copyout. The return is 0, 1, 9, 0xe or a value the facts leave open at 0xc00000000006ebbc. The command numbers are not decoded here.

- `xbow_widget_lookup` in `6.5.22/unix.IP27` at 0xc00000000006ef30
  **Inferred.** Maps a widget number in a1 to its vertex handle. It fetches the switch record with xswitch_info_get at 0xc00000000006ef3c and asks it for the vertex with xswitch_info_vhdl_get at 0xc00000000006ef60, in a loop bounded by a1 < 8 (branch at 0xc00000000006ef4c). It returns the handle at 0xc00000000006ef70. The assertion call at 0xc00000000006ef84 (constant 0x307) is on a path whose exact condition I did not confirm from the branches.

- `xbow_setwidint` in `6.5.22/unix.IP27` at 0xc00000000006ef94
  **Inferred.** Presets one crossbar interrupt. It reads the target, address, vector and sfarg of the interrupt through xtalk_intr_target_get at 0xc00000000006efa4, xtalk_intr_addr_get at 0xc00000000006efb0, xtalk_intr_vector_get at 0xc00000000006efbc and xtalk_intr_sfarg_get at 0xc00000000006efc8. It then calls xbow_intr_preset at 0xc00000000006efe8 with the sfarg and the address, and returns that result. The registers written are inside xbow_intr_preset, not in this routine.

- `xem_add_ioe` in `6.5.22/unix.IP27` at 0xc00000000006f034
  **Inferred.** Reads a record at a0 field by field, word and doubleword loads at offsets 0x0 to 0x48, and prints each field with a cmn_err call (for example at 0xc00000000006f0e4 and 0xc00000000006f258). Bit tests on a shifted copy of the first word, from 0xc00000000006f058 through 0xc00000000006f0bc, appear to gate which fields print. It returns the last cmn_err result at 0xc00000000006f0cc. This reads as a formatter for an error record, but the record layout is not settled here.

- `xbow_xmit_retry_error` in `6.5.22/unix.IP27` at 0xc00000000006f268
  **Inferred.** Handles a transmit-retry condition for one port. It reads a per-port record at a0 + (a1 << 3) + 0x188 (ld at 0xc00000000006f278), calls xswitch_info_get at 0xc00000000006f300 and xswitch_info_vhdl_get at 0xc00000000006f320 to find the port's vertex, and calls xtalk_piotrans_addr at 0xc00000000006f340 with a 0x58 size argument. Compares against the constants 0xc002, 0xd002, 0x24 and 0x36 select the case, and the updated record is stored back at 0xc00000000006f358. It returns 0 or 1 at 0xc00000000006f2c8. The meaning of the case constants is not settled by these facts.

- `xbow_errintr_handler` in `6.5.22/unix.IP27` at 0xc00000000006f36c
  **Inferred.** Interrupt-side handler for crossbar errors. It reads the status word through the pointer at a0 + 0x10, tests bit masks such as 0x25, 0x1, 0x40000, 0x10000 and 0x20000 in the branches from 0xc00000000006f534 to 0xc00000000006f5f0, and calls xbow_xmit_retry_error at 0xc00000000006f4b8 and xbow_error_handler at 0xc00000000006f504 on the paths those tests select. It resolves the widget name with xbow_widget_lookup at 0xc00000000006f5a0 and xwidget_name_get at 0xc00000000006f5a8, reports through many cmn_err calls, and passes an error record to xem_add_ioe at 0xc00000000006f7d0. It returns the result of its last cmn_err or cmn_err_tag call at 0xc00000000006f56c.

- `xbow_error_handler` in `6.5.22/unix.IP27` at 0xc00000000006f960
  **Inferred.** Handles a crossbar error for one port. It looks up the widget with xbow_widget_lookup at 0xc00000000006fac8 using the word at a0 + 8, and passes the error to xtalk_error_handler at 0xc00000000006fb64 when the lookup finds a widget. Most of the routine is cmn_err reporting of widget names and register fields, from 0xc00000000006fbcc to 0xc000000000070000, with bit tests against 0x20, 0x7ff and 0x40 on the status word. It returns 0x6, 0x0 or a value the facts leave open at 0xc00000000006fa1c. Its role beyond reporting is not settled here.

- `xbow_update_perf_counters` in `6.5.22/unix.IP27` at 0xc000000000070010
  **Inferred.** Refreshes the performance counters for one crossbar port. It gets the per-vertex record with hwgraph_fastinfo_get at 0xc000000000070030, updates counter words under mutex_spinlock at 0xc000000000070054 and mutex_spinunlock at 0xc0000000000700ac, and then schedules itself again with timeout at 0xc0000000000700ec, passing its own address and a 4-tick delay. The branches at 0xc0000000000700bc and 0xc0000000000700d8 test s1 against 2 before the reschedule, and the facts do not show what s1 holds.

- `xbow_get_perf_counters` in `6.5.22/unix.IP27` at 0xc000000000070118
  **Inferred.** Returns the address of a per-port counter block. It calls hwgraph_fastinfo_get at 0xc000000000070120 with its four incoming arguments, then returns that record plus 0x40 (v0 = ret(hwgraph_fastinfo_get)+64 at 0xc000000000070130). The counter meaning comes from the name, because this routine only computes the address.

- `xbow_enable_perf_counter` in `6.5.22/unix.IP27` at 0xc000000000070138
  **Inferred.** Enables one performance counter on a crossbar port. It gets the per-vertex record with hwgraph_fastinfo_get at 0xc00000000007015c and checks the counter index against 8 (branch at 0xc00000000007017c). It updates enable and mode bytes at record offsets 0x24 to 0x27 (sb stores at 0xc000000000070258 through 0xc00000000007026c) and a 32-bit word at offset 0x20 (sw at 0xc0000000000702dc). It arms a timeout at 0xc0000000000702d8 that calls xbow_update_perf_counters at 0xc000000000070010 with a 4-tick delay, all inside mutex_spinlock at 0xc0000000000701dc and mutex_spinunlock at 0xc000000000070248. It returns -1 or 0 at 0xc0000000000701c8.

- `xbow_get_llp_status` in `6.5.22/unix.IP27` at 0xc000000000070304
  **Inferred.** Returns the address of a link-status block for one port. It calls hwgraph_fastinfo_get at 0xc00000000007030c with its four incoming arguments, then returns that record plus 0x140 (v0 = ret(hwgraph_fastinfo_get)+320 at 0xc00000000007031c). The status meaning comes from the name, because this routine only computes the address.

- `xbow_update_llp_status` in `6.5.22/unix.IP27` at 0xc000000000070324
  **Inferred.** Refreshes link-status words for one crossbar port. It fetches the per-vertex record with hwgraph_fastinfo_get at 0xc000000000070344, resolves the widget name through xbow_widget_lookup at 0xc00000000007036c and xwidget_name_get at 0xc000000000070374, and updates link words in a loop bounded by s0 < 8 (bne at 0xc0000000000703cc). It reschedules itself with timeout at 0xc0000000000703ec using a 0x64-tick delay. A flag word at record offset 0x1c4 (lw at 0xc0000000000703d4) selects a path, and cmn_err at 0xc000000000070428 reports a failure on one path.

- `xbow_disable_llp_monitor` in `6.5.22/unix.IP27` at 0xc000000000070438
  **Inferred.** Turns off the link-status monitor for one port. It gets the per-vertex record with hwgraph_fastinfo_get at 0xc000000000070440, and a branch at 0xc000000000070468 tests the index argument against 8 before a word is written at record offset 0x1c4 (sw at 0xc000000000070474). The routine returns 0 at 0xc000000000070478. The value written and the two doubleword stores at 0xc000000000070464 and 0xc00000000007046c are not decoded from these facts.
