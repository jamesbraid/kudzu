# Tier-2 routine readings, kudzu round 3, batch 6

Role entries for 24 crossbar and widget routines in the IP27 kernel build, read from
instruction facts rendered for each address. Each role states what the code does from
the calls, constants and branches at the cited addresses. Where the facts leave the
meaning open, the entry says so.

- `xwidget_register` in `6.5.22/unix.IP27` at 0xc00000000006e038
  **Inferred.** Allocates a 0x40-byte record with kmem_alloc at 0xc00000000006e078 and fills fields in it. It calls dev_to_name at 0xc00000000006e0bc, allocates strlen+1 bytes with kmem_alloc at 0xc00000000006e0d8 and copies the name into that allocation with strcpy at 0xc00000000006e0e8. It calls xwidget_info_set at 0xc00000000006e0f8 with a1 and a kmem_alloc result (the facts do not say which of the two allocations) and device_master_set at 0xc00000000006e108 with a1 and a3. A branch on a5 at 0xc00000000006e110 then splits the tail. When a5 is 0 it calls cdl_add_connpt at 0xc00000000006e12c and returns that result at 0xc00000000006e140. When a5 is nonzero it branches to 0xc00000000006e148 and calls async_attach_add_info at 0xc00000000006e14c with a1 and a5, skipping cdl_add_connpt.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_unregister` in `6.5.22/unix.IP27` at 0xc00000000006e15c
  **Inferred.** Looks up the widget record with xwidget_info_get at 0xc00000000006e16c. When the lookup returns 0 it returns 0 or 1 at 0xc00000000006e190, and the facts do not say which. When a record comes back it calls hwgraph_inventory_remove at 0xc00000000006e1ac and cdl_del_connpt at 0xc00000000006e1c4, then strlen on the stored pointer at offset 0x38 (0xc00000000006e1cc) and kmem_free on that pointer at 0xc00000000006e1d8. It then calls blkclr on the record at 0xc00000000006e1e4 with a1=0x8 (so the facts show only an 8-byte clear) and kmem_free with size 0x40 on the record at 0xc00000000006e1f0. The facts show no return after the final kmem_free.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_error_register` in `6.5.22/unix.IP27` at 0xc00000000006e200
  **Inferred.** Fetches the widget record with xwidget_info_get at 0xc00000000006e210 and stores two 8-byte words into it at offsets 0x28 and 0x30 (stores at 0xc00000000006e218 and 0xc00000000006e230). It returns the record pointer in v0 at 0xc00000000006e22c. The facts do not show what the two stored words are, so the error-registration role rests on the name.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_reset` in `6.5.22/unix.IP27` at 0xc00000000006e234
  **Established.** Forwards its four incoming arguments unchanged to xswitch_reset_link at 0xc00000000006e23c and returns that call's result. The routine holds no other calls or branches.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_gfx_reset` in `6.5.22/unix.IP27` at 0xc00000000006e250
  **Inferred.** Calls xswitch_reset_link at 0xc00000000006e25c, then looks up the widget record with xwidget_info_get at 0xc00000000006e264. On a found record it takes the master and ID through xwidget_info_master_get at 0xc00000000006e278 and xwidget_info_id_get at 0xc00000000006e284, and passes them to hub_widget_reset at 0xc00000000006e294. A null lookup branches at 0xc00000000006e26c to a path that ends in the assertion call at 0xc00000000006e2b8 (constant 0x441). The branches do not settle which path reaches that assertion.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_name_get` in `6.5.22/unix.IP27` at 0xc00000000006e2c8
  **Inferred.** Returns the static pointer 0xc0000000003c6020 when its first argument equals -1, which the branch at 0xc00000000006e2d8 tests. Otherwise it looks up the widget record with xwidget_info_get at 0xc00000000006e2e0 and returns the name from xwidget_info_name_get at 0xc00000000006e2e8. The static pointer reads as a fallback name.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_init` in `6.5.22/unix.IP27` at 0xc00000000006e658
  **Inferred.** Registers three crossbar widget drivers by calling xwidget_driver_register at 0xc00000000006e66c, 0xc00000000006e680 and 0xc00000000006e694. Each call passes a different first constant (0xd100, 0xd000 and 0x0), the shared argument pointer 0xc0000000003c6098 and the constant 1 in a3. It returns the last registration's result in v0 at 0xc00000000006e6a0. What the three constants identify is not derived here.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_attach` in `6.5.22/unix.IP27` at 0xc00000000006e6a8
  **Inferred.** Attaches one crossbar widget. It calls xtalk_piotrans_addr at 0xc00000000006e6c8, hwgraph_connectpt_get at 0xc00000000006e6d8, hwgraph_char_device_add at 0xc00000000006e6f0 and kmem_zalloc with size 0x290 at 0xc00000000006e700, then stores the allocation with hwgraph_fastinfo_set at 0xc00000000006e724. It builds a name with dev_to_name, strlen, kmem_alloc and sprintf (0xc00000000006e748 to 0xc00000000006e780). It duplicates and flags a device description (device_desc_dup, device_desc_flags_get, device_desc_flags_set, device_desc_intr_name_set), then calls xtalk_intr_alloc at 0xc00000000006e834 and xtalk_intr_connect at 0xc00000000006e850. The connect call passes the handler at 0xc00000000006f36c and a second code address 0xc00000000006ef94. xwidget_error_register at 0xc00000000006e868 takes the callback at 0xc00000000006f960. It then calls xswitch_info_new at 0xc00000000006e878, loops over xswitch_info_link_is_ok, xbow_link_alive and xbow_widget_present (0xc00000000006e894 to 0xc00000000006e8e0), calls xswitch_provider_register at 0xc00000000006e8f8 and returns 0 at 0xc00000000006e91c. A value whose source the facts do not show is compared against 2, 1, 3, 4, 5, 0xd0001 and 0xd0002 in branches from 0xc00000000006e7a4 to 0xc00000000006e95c, and a cmn_err call with a0=4 at 0xc00000000006e984 sits after those tests. The facts do not say what that value is or what each constant selects.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_open` in `6.5.22/unix.IP27` at 0xc00000000006e9c4
  **Inferred.** Calls cap_able_cred at 0xc00000000006e9d4 with a0=a3, the capability bit 0x2000000000 in a1, and a2 and a3 passed through. One branch, at 0xc00000000006e9dc, tests the result: nonzero falls through to a return of 0 or 1 at 0xc00000000006e9e8, and zero branches to 0xc00000000006e9f0. Which return value means the open is allowed is not settled from these facts.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_map` in `6.5.22/unix.IP27` at 0xc00000000006ea00
  **Inferred.** Fetches a per-vertex record with hwgraph_fastinfo_get at 0xc00000000006ea10, reads the 8-byte word at record offset 0x10 (ld at 0xc00000000006ea28), and calls v_mapphys at 0xc00000000006ea2c with the caller's a1, that word and the constant -0x4000 in a3. It returns the v_mapphys result. The mapping role comes from the callee name, and the meaning of the constant is not settled here.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_widget_num_get` in `6.5.22/unix.IP27` at 0xc00000000006ea4c
  **Inferred.** Takes a vertex handle in a0 and looks for the crossbar widget number behind it. It calls vertex_to_name at 0xc00000000006ea60 and nodevertex_to_cnodeid at 0xc00000000006ea68. When the second returns -1, a branch at 0xc00000000006ea74 skips ahead. Otherwise it loads a 16-bit value from the table at 0xc0000000014d3800 (lh at 0xc00000000006ea90, in the delay slot) and calls hub_widget_id at 0xc00000000006ea8c with that entry, and the routine can return that result. On the other path it calls strstr on the name twice, at 0xc00000000006eab4 and 0xc00000000006eac8, and both calls must return nonzero to continue (a zero result branches to 0xc00000000006eb04). It then calls hwgraph_edge_get at 0xc00000000006eaec, xwidget_info_chk at 0xc00000000006eb20 and xwidget_info_id_get at 0xc00000000006eb38. The return is one of the hub_widget_id result, the xwidget_info_id_get result or 0xff. The two string constants are not decoded from these facts.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006eb48
  **Inferred.** Dispatches on the command in a1 using compares against 5, 6, 3, 4, 1 and 0x6702 (branches at 0xc00000000006eb74, 0xc00000000006eb80, 0xc00000000006eb94 and 0xc00000000006eb9c, and further compares in the arms). The arms call xbow_get_perf_counters at 0xc00000000006ebe4, xbow_get_llp_status at 0xc00000000006ec6c, xbow_disable_llp_monitor at 0xc00000000006ecf4, xbow_enable_perf_counter at 0xc00000000006ed94 and xbow_enable_llp_monitor at 0xc00000000006eda8. xbow_prio_bw_alloc is called four times (0xc00000000006ee00, 0xc00000000006ee18, 0xc00000000006ee54, 0xc00000000006ee6c), each preceded by xbow_widget_num_get calls, and assfail is called at 0xc00000000006ee88 (constant 0x264) and 0xc00000000006eea4 (constant 0x28c). cap_able_cred or cap_able with capability 0x2000000000 is called at 0xc00000000006ec28, 0xc00000000006ecac, 0xc00000000006ecdc, 0xc00000000006ed0c and 0xc00000000006ed4c, and the facts show no capability call before the xbow_get_perf_counters call. Arguments move with copyin (sizes 0xc, 0x10, 0x10) and copyout (sizes 0x100 and 0x80). The return is 0, 1, 9, 0xe or a value the facts leave open at 0xc00000000006ebbc. The command numbers are not decoded here.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_widget_lookup` in `6.5.22/unix.IP27` at 0xc00000000006ef30
  **Inferred.** Maps a widget number, held in a1 and copied to s0, to a vertex handle. It fetches the switch record with xswitch_info_get at 0xc00000000006ef3c and asks it for the vertex with xswitch_info_vhdl_get at 0xc00000000006ef60. The branch at 0xc00000000006ef4c tests a1 < 8 and jumps forward to 0xc00000000006ef78, and a second branch at 0xc00000000006ef54 tests another register against 0; the facts show no backward branch, so this is a range check, not a loop. The routine returns the vhdl result at 0xc00000000006ef70. The assertion call at 0xc00000000006ef84 (constant 0x307) is on a path whose exact condition is not settled from the branches.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_setwidint` in `6.5.22/unix.IP27` at 0xc00000000006ef94
  **Inferred.** Presets one crossbar interrupt. It reads the target, address, vector and sfarg of the interrupt through xtalk_intr_target_get at 0xc00000000006efa4, xtalk_intr_addr_get at 0xc00000000006efb0, xtalk_intr_vector_get at 0xc00000000006efbc and xtalk_intr_sfarg_get at 0xc00000000006efc8. It then calls xbow_intr_preset at 0xc00000000006efe8 with the sfarg and the address, and returns that result. The registers written are inside xbow_intr_preset, not in this routine.
  Reviewed: 2026-11-12 (sonnet)

- `xem_add_ioe` in `6.5.22/unix.IP27` at 0xc00000000006f034
  **Inferred.** Reads a record at a0 field by field, with a word load at offset 0x0, halfword loads at offsets 0x4 to 0xe and doubleword loads at offsets 0x10 to 0x48, and prints fields with cmn_err calls (for example at 0xc00000000006f0e4 and 0xc00000000006f258). Bit tests on shifted copies of the first word, from 0xc00000000006f058 through 0xc00000000006f0bc, appear to gate which fields print, and a first test at 0xc00000000006f04c branches to 0xc00000000006f0d4. It returns at 0xc00000000006f0cc either a cmn_err result or a value the facts leave open. This reads as a formatter for an error record, but the record layout is not settled here.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_xmit_retry_error` in `6.5.22/unix.IP27` at 0xc00000000006f268
  **Inferred.** Takes a port index in a1 and works on a per-port record at a0 + (a1 << 3) + 0x188 (ld at 0xc00000000006f278). When that record is nonzero it loads a word and tests it against 0xc002, 0xd002, 0x24 and 0x36 in branches from 0xc00000000006f2a0 to 0xc00000000006f2dc, which lead to different exits; the facts do not show what the constants mean. When the record is 0 it calls xswitch_info_get at 0xc00000000006f300 and xswitch_info_vhdl_get at 0xc00000000006f320, and unless the vhdl is -1 calls xtalk_piotrans_addr at 0xc00000000006f340 with a3=0x58. The 8-byte store back into the per-port record at 0xc00000000006f358 happens only when xtalk_piotrans_addr returns nonzero. It returns 0 or 1 at 0xc00000000006f2c8, and the transmit-retry role rests on the name.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_errintr_handler` in `6.5.22/unix.IP27` at 0xc00000000006f36c
  **Inferred.** Interrupt-side handler for crossbar errors. It loads a pointer at a0 + 0x10 (ld at 0xc00000000006f398) and a word at offset 0x54 of what it points to, tests bit masks such as 0x25, 0x1, 0x40000, 0x10000 and 0x20000 in branches from 0xc00000000006f49c to 0xc00000000006f740, and calls xbow_xmit_retry_error at 0xc00000000006f4b8 and xbow_error_handler at 0xc00000000006f504 (a1=8, a2=1) on paths those tests select. It resolves the widget name with xbow_widget_lookup at 0xc00000000006f5a0 and xwidget_name_get at 0xc00000000006f5a8, reports through many cmn_err calls, and passes the stack buffer it gave xbow_error_handler to xem_add_ioe at 0xc00000000006f7d0. It returns at 0xc00000000006f56c either a cmn_err_tag result (call at 0xc00000000006f714) or a value the facts leave open.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_error_handler` in `6.5.22/unix.IP27` at 0xc00000000006f960
  **Inferred.** Handles a crossbar error for one port. It calls xbow_widget_lookup at 0xc00000000006fac8 with the word at a0 + 8 and the halfword at a3 + 6. A lookup result of -1 branches around the call at 0xc00000000006fb64; otherwise further tests, including a compare of the word at [a0] with the lookup result at 0xc00000000006fae0, decide whether xtalk_error_handler at 0xc00000000006fb64 runs, and its result is compared with 9 at 0xc00000000006fb7c. Most of the routine is cmn_err reporting of widget names and register fields, from 0xc00000000006fbcc to 0xc000000000070000, with bit tests against 0x20, 0x7ff, 0x4 and 0x40 on loaded words. It returns 0x6, 0x0 or a value the facts leave open at 0xc00000000006fa1c. Its role beyond reporting is not settled here.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_update_perf_counters` in `6.5.22/unix.IP27` at 0xc000000000070010
  **Inferred.** Refreshes the performance counters for one crossbar port. It gets the per-vertex record with hwgraph_fastinfo_get at 0xc000000000070030, updates counter words under mutex_spinlock at 0xc000000000070054 and mutex_spinunlock at 0xc0000000000700ac, and then schedules itself again with timeout at 0xc0000000000700ec, passing its own address and a 4-tick delay. The branches at 0xc0000000000700bc and 0xc0000000000700d8 test s1 against 2 before the reschedule, and the facts do not show what s1 holds.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_get_perf_counters` in `6.5.22/unix.IP27` at 0xc000000000070118
  **Inferred.** Returns the address of a per-port counter block. It calls hwgraph_fastinfo_get at 0xc000000000070120 with its four incoming arguments, then returns that record plus 0x40 (v0 = ret(hwgraph_fastinfo_get)+64 at 0xc000000000070130). The counter meaning comes from the name, because this routine only computes the address.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_enable_perf_counter` in `6.5.22/unix.IP27` at 0xc000000000070138
  **Inferred.** Enables one performance counter on a crossbar port. It gets the per-vertex record with hwgraph_fastinfo_get at 0xc00000000007015c and checks the counter index in a1 against 8 (branch at 0xc00000000007017c); other tests on a2 and a3 lead to the return of -1 or 0 at 0xc0000000000701c8. It takes mutex_spinlock at 0xc0000000000701dc, then stores bytes at offsets 0x24 to 0x27 (sb at 0xc000000000070258 through 0xc00000000007026c) and a 32-bit word at offset 0x20 (sw at 0xc0000000000702dc), with the offsets relative to a pointer that includes the index shifted left by 4. It arms a timeout at 0xc0000000000702d8 that calls xbow_update_perf_counters at 0xc000000000070010 with a 4-tick delay. mutex_spinunlock is called at 0xc000000000070248, 0xc0000000000702e4 and 0xc0000000000702f4; the timeout at 0xc0000000000702d8 comes before the unlock at 0xc0000000000702e4.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_get_llp_status` in `6.5.22/unix.IP27` at 0xc000000000070304
  **Inferred.** Returns the address of a link-status block for one port. It calls hwgraph_fastinfo_get at 0xc00000000007030c with its four incoming arguments, then returns that record plus 0x140 (v0 = ret(hwgraph_fastinfo_get)+320 at 0xc00000000007031c). The status meaning comes from the name, because this routine only computes the address.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_update_llp_status` in `6.5.22/unix.IP27` at 0xc000000000070324
  **Inferred.** Refreshes link-status words for one crossbar port. It fetches the per-vertex record with hwgraph_fastinfo_get at 0xc000000000070344. A loop bounded by s0 < 8 (backward bne at 0xc0000000000703cc to 0xc000000000070368) contains xbow_widget_lookup at 0xc00000000007036c and xwidget_name_get at 0xc000000000070374, so those calls run on each pass, along with loads and doubleword stores. After the loop a flag word at record offset 0x1c4 (lw at 0xc0000000000703d4) is tested. When it is 0 the branch goes to 0xc0000000000703f4, and the facts do not settle whether the return at 0xc000000000070410 or the cmn_err at 0xc000000000070428 follows. Otherwise it reschedules itself with timeout at 0xc0000000000703ec using a 0x64-tick delay. The cmn_err call passes a0=4 and the widget name; the facts do not show what the message says.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_disable_llp_monitor` in `6.5.22/unix.IP27` at 0xc000000000070438
  **Inferred.** Turns off the link-status monitor for one port. It gets the per-vertex record with hwgraph_fastinfo_get at 0xc000000000070440. A backward branch at 0xc000000000070468 to 0xc000000000070454 repeats while a2 < 8, around doubleword stores at 0xc000000000070464 and 0xc00000000007046c. A word is then written at record offset 0x1c4 (sw at 0xc000000000070474) and the routine returns 0 at 0xc000000000070478. The values written and the loop counter's origin are not decoded from these facts.
  Reviewed: 2026-11-12 (sonnet)
