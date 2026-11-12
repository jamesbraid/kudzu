# Tier-2 routine readings, kudzu batch 4

Role statements for 25 routines of the tier-2 list, written from the rendered
instructions only: calls, branches, loads, stores and constants. Every grade is
inferred. Where a routine's arguments or return value depend on code the render
does not show, the entry says so.

- `xtalk_intr_alloc_nothd` in `6.5.22/unix.IP27` at 0xc00000000006da28
  **Inferred.** A 28-byte pass-through. It passes its four incoming argument registers to `hub_intr_alloc_nothd` (0xc00000000003bcc8) with the `jal` at 0xc00000000006da30 and returns that callee's v0. The body has no other memory access or branch, so any logic sits inside the callee.

- `xtalk_intr_free` in `6.5.22/unix.IP27` at 0xc00000000006da44
  **Inferred.** A 28-byte pass-through with the same shape. It forwards its arguments to `hub_intr_free` (0xc00000000003bce4) via the `jal` at 0xc00000000006da4c and returns the callee's v0.

- `xtalk_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000006da60
  **Inferred.** A 28-byte pass-through. It forwards its arguments to `hub_intr_connect` (0xc00000000003bd5c) via the `jal` at 0xc00000000006da68 and returns the callee's v0.

- `xtalk_intr_disconnect` in `6.5.22/unix.IP27` at 0xc00000000006da7c
  **Inferred.** The `jal` at 0xc00000000006da84 is the only call, into `hub_intr_disconnect` (0xc00000000003bde4). The four entry argument registers go in unchanged, and the return at 0xc00000000006da90 leaves that callee's result in v0.

- `xtalk_intr_cpu_get` in `6.5.22/unix.IP27` at 0xc00000000006da98
  **Inferred.** A single `jal` at 0xc00000000006daa0 reaches `hub_intr_cpu_get` (0xc00000000003be48) with the entry registers a0 to a3 intact. The value in v0 at the return, 0xc00000000006daac, is whatever that callee produced.

- `xtalk_intr_cpuid_get` in `6.5.22/unix.IP27` at 0xc00000000006dab4
  **Inferred.** The routine's one call is at 0xc00000000006dabc, to `hub_intr_cpuid_get` (0xc00000000003be64). Entry arguments reach the callee untouched, and the result the return at 0xc00000000006dac8 gives back is that callee's v0.

- `xtalk_error_devenable` in `6.5.22/unix.IP27` at 0xc00000000006db7c
  **Inferred.** Here the `jal` at 0xc00000000006db84 goes to `hub_error_devenable` (0xc000000000027870). The return at 0xc00000000006db90 hands back that routine's v0, with the arguments unaltered.

- `xtalk_provider_startup` in `6.5.22/unix.IP27` at 0xc00000000006db98
  **Inferred.** The `jal` at 0xc00000000006dba0 dispatches to `hub_provider_startup` (0xc00000000003be6c). The return at 0xc00000000006dbac carries its v0 out, and no register is rewritten before the call.

- `xtalk_provider_shutdown` in `6.5.22/unix.IP27` at 0xc00000000006dbb4
  **Inferred.** Via the `jal` at 0xc00000000006dbbc the routine reaches `hub_provider_shutdown` (0xc00000000003bea0). Nothing is stored or tested around that call, and the return at 0xc00000000006dbc8 reports the callee's v0.

- `xtalk_widgetdev_enable` in `6.5.22/unix.IP27` at 0xc00000000006dbd0
  **Inferred.** Its `jal` at 0xc00000000006dbd8 calls `hub_widgetdev_enable` (0xc0000000000277d0). The return at 0xc00000000006dbe4 forwards whatever v0 that call leaves behind.

- `xtalk_widgetdev_shutdown` in `6.5.22/unix.IP27` at 0xc00000000006dbec
  **Inferred.** The `jal` at 0xc00000000006dbf4 is the only call, to `hub_widgetdev_shutdown` (0xc0000000000275f8). The return at 0xc00000000006dc00 passes the callee's v0 to the caller.

- `xtalk_dma_enabled` in `6.5.22/unix.IP27` at 0xc00000000006dc08
  **Inferred.** The `jal` at 0xc00000000006dc10 targets `hub_dma_enabled` (0xc000000000027534), and the return at 0xc00000000006dc1c passes its v0 back. The name suggests a DMA-enabled query, but the render shows only the forwarding.

- `xwidget_info_chk` in `6.5.22/unix.IP27` at 0xc00000000006dc84
  **Inferred.** A 44-byte routine that calls `hwgraph_info_get_LBL` (0xc000000000296160) at 0xc00000000006dc98, passing the constant label address 0xc0000000003c5e80 and a stack buffer at sp-16. Its return value at 0xc00000000006dca8 is not resolved by the render, so what it checks is unknown here.

- `xwidget_info_get` in `6.5.22/unix.IP27` at 0xc00000000006dcb0
  **Inferred.** An 88-byte routine that calls `hwgraph_fastinfo_get` (0xc000000000296ee8) and returns its result in v0. A zero result takes the `beq` at 0xc00000000006dcc4 to the tail; a nonzero result loads the doubleword at its +0 and compares it with the constant 0xc000000001436d68. On a mismatch it calls `cmn_err` (0xc0000000001f8090) at 0xc00000000006dcf8 with the format constant 0xc0000000003c5e90, and the render does not show whether that call returns.

- `xwidget_info_set` in `6.5.22/unix.IP27` at 0xc00000000006dd08
  **Inferred.** An 88-byte routine that stores a doubleword to the pointer in a1 at 0xc00000000006dd28, only when a1 is nonzero (the `beq` at 0xc00000000006dd20 skips it otherwise). It then calls `hwgraph_fastinfo_set` (0xc000000000296eb8) with the same constant 0xc000000001436d68 that `xwidget_info_get` compares against, and calls `hwgraph_info_add_LBL` (0xc0000000002960bc) with the label constant 0xc0000000003c5e80 that `xwidget_info_chk` queries. The render does not show the stored value or whether the second call runs on the same path as the store.

- `xwidget_info_dev_get` in `6.5.22/unix.IP27` at 0xc00000000006dd60
  **Inferred.** A 52-byte accessor. It returns the 32-bit word at +0x8 of its pointer argument. A null pointer takes the `beq` at 0xc00000000006dd68 to a call to `panic` (0xc0000000001f9228) with the format constant 0xc0000000003c5ea8.

- `xwidget_info_id_get` in `6.5.22/unix.IP27` at 0xc00000000006dd94
  **Inferred.** A 52-byte accessor that returns the byte at +0xc of its pointer argument. A null pointer reaches a call to `panic` (0xc0000000001f9228) with the format constant 0xc0000000003c5ed0.

- `xwidget_info_master_get` in `6.5.22/unix.IP27` at 0xc00000000006ddc8
  **Inferred.** A 52-byte accessor that returns the 32-bit word at +0x1c of its pointer argument. A null pointer reaches a call to `panic` (0xc0000000001f9228) with the format constant 0xc0000000003c5ef8.

- `xwidget_info_masterid_get` in `6.5.22/unix.IP27` at 0xc00000000006ddfc
  **Inferred.** A 52-byte accessor that returns the byte at +0x20 of its pointer argument. A null pointer reaches a call to `panic` (0xc0000000001f9228) with the format constant 0xc0000000003c5f28.

- `xwidget_info_part_num_get` in `6.5.22/unix.IP27` at 0xc00000000006de30
  **Inferred.** A 52-byte accessor that returns the 32-bit word at +0x10 of its pointer argument. A null pointer reaches a call to `panic` (0xc0000000001f9228) with the format constant 0xc0000000003c5f58.

- `xwidget_info_mfg_num_get` in `6.5.22/unix.IP27` at 0xc00000000006de64
  **Inferred.** A 52-byte accessor that returns the 32-bit word at +0x18 of its pointer argument. A null pointer reaches a call to `panic` (0xc0000000001f9228) with the format constant 0xc0000000003c5f88.

- `xwidget_info_name_get` in `6.5.22/unix.IP27` at 0xc00000000006de98
  **Inferred.** A 52-byte accessor that returns the doubleword at +0x38 of its pointer argument. A null pointer reaches a call to `panic` (0xc0000000001f9228) with the format constant 0xc0000000003c5fb8.

- `xwidget_driver_register` in `6.5.22/unix.IP27` at 0xc00000000006df78
  **Inferred.** A 116-byte routine that tests the doubleword at +0x8 of the global at 0xc0000000014c9db8. If that is zero, it first calls `xtalk_init` (0xc00000000006decc) at 0xc00000000006dfd0. The two paths rejoin before `cdl_add_driver` (0xc0000000000d15bc), which it calls at 0xc00000000006dfb8 on every path with that same value as its first argument, and it returns the callee's v0. The render does not show what the driver argument contains.

- `xwidget_driver_unregister` in `6.5.22/unix.IP27` at 0xc00000000006dfec
  **Inferred.** A 36-byte routine that calls `cdl_del_driver` (0xc0000000000d1900) with the doubleword at +0x8 of the global 0xc0000000014c9db8 as its first argument and a second argument equal to its own a0, then returns the callee's v0. It is the counterpart of `xwidget_driver_register`, which reads the same global.

- `xwidget_register` in `6.5.22/unix.IP27` at 0xc00000000006e038
  **Inferred.** A 292-byte routine that allocates a 0x40-byte record with `kmem_alloc` (0xc000000000181bbc) at 0xc00000000006e078 and copies fields from its argument at offsets 0x0, 0x4 and 0x8 into it. It gets the device name with `dev_to_name` (0xc0000000002979bc) into a 256-byte stack buffer, allocates a second block of strlen+1 bytes, copies the name with `strcpy` (0xc00000000014635c), and attaches the record with `xwidget_info_set` (0xc00000000006dd08). It then calls `device_master_set` (0xc000000000297a70) and `cdl_add_connpt` (0xc0000000000d1af8), calling `async_attach_add_info` (0xc0000000000d2680) only when a5 is nonzero, and returns cdl_add_connpt's v0. The render does not show what the record's fields mean.
