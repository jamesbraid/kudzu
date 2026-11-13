# Tier-2 routine readings, kudzu round 3 batch 5

- `xtalk_intr_alloc` in `6.5.22/unix.IP27` at 0xc00000000006da0c
  **Inferred.** It passes its four argument registers unchanged to hub_intr_alloc, reached through the jal at 0xc00000000006da14 (target 0xc00000000003bcac), and returns that call's v0 at 0xc00000000006da20. The 28-byte body makes no other call and has no branch. It looks like a thin forwarding wrapper, but what the allocation does depends on the callee, which these facts do not show.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_intr_alloc_nothd` in `6.5.22/unix.IP27` at 0xc00000000006da28
  **Inferred.** It forwards its four argument registers unchanged to hub_intr_alloc_nothd through the jal at 0xc00000000006da30 (target 0xc00000000003bcc8) and returns the callee's v0 at 0xc00000000006da3c. The body is the same shape as the allocator at 0xc00000000006da0c. The name differs from that allocator only by a suffix, but what the callee does differently is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_intr_free` in `6.5.22/unix.IP27` at 0xc00000000006da44
  **Inferred.** It forwards its four argument registers unchanged to hub_intr_free through the jal at 0xc00000000006da4c (target 0xc00000000003bce4) and returns the callee's v0 at 0xc00000000006da58. It has no other call and no branch, so it is a pass-through. The release behaviour is whatever hub_intr_free does, which is not rendered here.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000006da60
  **Inferred.** It forwards its four argument registers unchanged to hub_intr_connect through the jal at 0xc00000000006da68 (target 0xc00000000003bd5c) and returns the callee's v0 at 0xc00000000006da74. It is a 28-byte pass-through with no branch. The connect semantics are inferred from the callee's name alone.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_intr_disconnect` in `6.5.22/unix.IP27` at 0xc00000000006da7c
  **Inferred.** It forwards its four argument registers unchanged to hub_intr_disconnect through the jal at 0xc00000000006da84 (target 0xc00000000003bde4) and returns the callee's v0 at 0xc00000000006da90. It has no branch and no other call. It pairs by name with xtalk_intr_connect at 0xc00000000006da60, but these facts do not show that the two share state.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_intr_cpu_get` in `6.5.22/unix.IP27` at 0xc00000000006da98
  **Inferred.** It forwards its four argument registers unchanged to hub_intr_cpu_get through the jal at 0xc00000000006daa0 (target 0xc00000000003be48) and returns the callee's v0 at 0xc00000000006daac. The body is a single pass-through call. The name suggests it reports the CPU an interrupt is routed to, but the value returned is whatever the callee produces.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_intr_cpuid_get` in `6.5.22/unix.IP27` at 0xc00000000006dab4
  **Inferred.** It forwards its four argument registers unchanged to hub_intr_cpuid_get through the jal at 0xc00000000006dabc (target 0xc00000000003be64) and returns the callee's v0 at 0xc00000000006dac8. It has no branch and no other call. The name suggests a CPU identifier rather than a CPU handle, which the facts cannot confirm.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_error_devenable` in `6.5.22/unix.IP27` at 0xc00000000006db7c
  **Inferred.** It forwards its four argument registers unchanged to hub_error_devenable through the jal at 0xc00000000006db84 (target 0xc000000000027870) and returns the callee's v0 at 0xc00000000006db90. It is a 28-byte pass-through. The name suggests it enables error reporting for a device, but that is inferred from the name and the callee's name, not from any rendered code here.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_provider_startup` in `6.5.22/unix.IP27` at 0xc00000000006db98
  **Inferred.** It forwards its four argument registers unchanged to hub_provider_startup through the jal at 0xc00000000006dba0 (target 0xc00000000003be6c) and returns the callee's v0 at 0xc00000000006dbac. It has no branch and no other call. Its role as a bus provider start hook rests on the callee's name, which these facts do not confirm.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_provider_shutdown` in `6.5.22/unix.IP27` at 0xc00000000006dbb4
  **Inferred.** It forwards its four argument registers unchanged to hub_provider_shutdown through the jal at 0xc00000000006dbbc (target 0xc00000000003bea0) and returns the callee's v0 at 0xc00000000006dbc8. It is the teardown counterpart of xtalk_provider_startup at 0xc00000000006db98 by name, and the body has the same shape. Any ordering between the two lives outside this routine.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_widgetdev_enable` in `6.5.22/unix.IP27` at 0xc00000000006dbd0
  **Inferred.** It forwards its four argument registers unchanged to hub_widgetdev_enable through the jal at 0xc00000000006dbd8 (target 0xc0000000000277d0) and returns the callee's v0 at 0xc00000000006dbe4. The body is a 28-byte pass-through with no branch. The name suggests enabling one widget device, but which device and what enabling means are settled only by the callee.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_widgetdev_shutdown` in `6.5.22/unix.IP27` at 0xc00000000006dbec
  **Inferred.** It forwards its four argument registers unchanged to hub_widgetdev_shutdown through the jal at 0xc00000000006dbf4 (target 0xc0000000000275f8) and returns the callee's v0 at 0xc00000000006dc00. It is the shutdown pair of xtalk_widgetdev_enable at 0xc00000000006dbd0 by name, with the same body shape. What the shutdown does is inferred from the callee's name only.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dma_enabled` in `6.5.22/unix.IP27` at 0xc00000000006dc08
  **Inferred.** It forwards its four argument registers unchanged to hub_dma_enabled through the jal at 0xc00000000006dc10 (target 0xc000000000027534) and returns the callee's v0 at 0xc00000000006dc1c. It has no branch and no other call. The name suggests a query of whether DMA is enabled, which the callee's return value would carry, but that is not visible here.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_chk` in `6.5.22/unix.IP27` at 0xc00000000006dc84
  **Inferred.** It calls hwgraph_info_get_LBL through the jal at 0xc00000000006dc98 (target 0xc000000000296160), passing its own arg0 and arg3 unchanged in a0 and a3, the constant 0xc0000000003c5e80 in a1 and the address sp-16 (a stack slot) in a2. What the constant and the stack slot mean to the callee is not shown. The return value is shown as unknown in the facts (v0 = ?), so what the routine reports is not settled. The name suggests a check that the widget info record exists.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_get` in `6.5.22/unix.IP27` at 0xc00000000006dcb0
  **Inferred.** It calls hwgraph_fastinfo_get through the jal at 0xc00000000006dcbc (target 0xc000000000296ee8) with its own four arguments. If that call returns zero, the beq at 0xc00000000006dcc4 skips to 0xc00000000006dcd8; otherwise the routine loads the doubleword at offset 0 of the returned pointer (ld at 0xc00000000006dccc) and compares it with the constant 0xc000000001436d68 (bne at 0xc00000000006dcd4). A mismatch leads to a cmn_err call at 0xc00000000006dcf8 (target 0xc0000000001f8090) with the message constant 0xc0000000003c5e90. It returns the fastinfo pointer at 0xc00000000006dce4. The exact error-path order is only partly rendered, so the mismatch handling is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_set` in `6.5.22/unix.IP27` at 0xc00000000006dd08
  **Inferred.** It stores a doubleword through its second argument at 0xc00000000006dd28 (sd), but only when a1 is nonzero, because the beq at 0xc00000000006dd20 skips the store when a1 is zero. It then calls hwgraph_fastinfo_set at 0xc00000000006dd30 (target 0xc000000000296eb8) with the constant 0xc000000001436d68 as its third argument, and calls hwgraph_info_add_LBL at 0xc00000000006dd44 (target 0xc0000000002960bc) with the label constant 0xc0000000003c5e80. It returns the add call's v0 at 0xc00000000006dd58. The stored value is not shown in the facts, so the pairing with xwidget_info_get at 0xc00000000006dcb0 is inferred from the shared constants.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_dev_get` in `6.5.22/unix.IP27` at 0xc00000000006dd60
  **Inferred.** It loads a 4-byte word at offset 0x8 of its first argument (lw at 0xc00000000006dd70) and returns it. When the pointer is zero, the beq at 0xc00000000006dd68 leads to a panic call at 0xc00000000006dd80 (target 0xc0000000001f9228) with the message constant 0xc0000000003c5ea8. The routine is a null-checked field accessor. The field's meaning is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_id_get` in `6.5.22/unix.IP27` at 0xc00000000006dd94
  **Inferred.** It loads a 1-byte value at offset 0xc of its first argument (lbu at 0xc00000000006dda4) and returns it. A zero pointer reaches a panic call at 0xc00000000006ddb4 (target 0xc0000000001f9228) with the message constant 0xc0000000003c5ed0. It is the same null-checked accessor shape as xwidget_info_dev_get at 0xc00000000006dd60, and the field's meaning is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_master_get` in `6.5.22/unix.IP27` at 0xc00000000006ddc8
  **Inferred.** It loads a 4-byte word at offset 0x1c of its first argument (lw at 0xc00000000006ddd8) and returns it. A zero pointer reaches a panic call at 0xc00000000006dde8 (target 0xc0000000001f9228) with the message constant 0xc0000000003c5ef8. It is a null-checked field accessor like the others in this run, and the field's meaning is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_masterid_get` in `6.5.22/unix.IP27` at 0xc00000000006ddfc
  **Inferred.** It loads a 1-byte value at offset 0x20 of its first argument (lbu at 0xc00000000006de0c) and returns it. A zero pointer reaches a panic call at 0xc00000000006de1c (target 0xc0000000001f9228) with the message constant 0xc0000000003c5f28. It is a null-checked field accessor, and the field's meaning is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_part_num_get` in `6.5.22/unix.IP27` at 0xc00000000006de30
  **Inferred.** It loads a 4-byte word at offset 0x10 of its first argument (lw at 0xc00000000006de40) and returns it. A zero pointer reaches a panic call at 0xc00000000006de50 (target 0xc0000000001f9228) with the message constant 0xc0000000003c5f58. It is a null-checked field accessor, and the field's meaning is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_mfg_num_get` in `6.5.22/unix.IP27` at 0xc00000000006de64
  **Inferred.** It loads a 4-byte word at offset 0x18 of its first argument (lw at 0xc00000000006de74) and returns it. A zero pointer reaches a panic call at 0xc00000000006de84 (target 0xc0000000001f9228) with the message constant 0xc0000000003c5f88. It is a null-checked field accessor, and the field's meaning is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_info_name_get` in `6.5.22/unix.IP27` at 0xc00000000006de98
  **Inferred.** It loads an 8-byte doubleword at offset 0x38 of its first argument (ld at 0xc00000000006dea8) and returns it. A zero pointer reaches a panic call at 0xc00000000006deb8 (target 0xc0000000001f9228) with the message constant 0xc0000000003c5fb8. It is a null-checked field accessor, and the field is probably a pointer, but the facts do not show what it points to.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_driver_register` in `6.5.22/unix.IP27` at 0xc00000000006df78
  **Inferred.** It loads the doubleword stored at the global 0xc0000000014c9db8 (an 8-byte load of the global itself, not a field of it) and branches on it at 0xc00000000006dfa0. When that word is nonzero it calls cdl_add_driver (jal at 0xc00000000006dfb8, target 0xc0000000000d15bc) with the loaded word as the first argument and its own arg0, arg1 and arg2 as the second, third and fourth, and returns that call's v0 at 0xc00000000006dfc8. When it is zero, the branch goes to a call of xtalk_init (jal at 0xc00000000006dfd0, target 0xc00000000006decc) with its own four arguments passed through. The names suggest registering a bus driver with a device framework, and the same global is used by xwidget_driver_unregister at 0xc00000000006dfec, but the path after the zero-case call is only partly rendered.
  Reviewed: 2026-11-12 (sonnet)

- `xwidget_driver_unregister` in `6.5.22/unix.IP27` at 0xc00000000006dfec
  **Inferred.** It calls cdl_del_driver through the jal at 0xc00000000006dffc (target 0xc0000000000d1900), passing the doubleword stored at the global 0xc0000000014c9db8 (an 8-byte load of the global itself) as the first argument, its own arg0 as the second, a zero as the third, and its arg3 as the fourth. It returns the callee's v0 at 0xc00000000006e008. The body is one call with no branch, so it is the counterpart of xwidget_driver_register at 0xc00000000006df78 by name and by the shared global.
  Reviewed: 2026-11-12 (sonnet)
