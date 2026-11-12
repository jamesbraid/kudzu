# Tier-2 routine readings, kudzu round 2 batch 7

- `nic_hub_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf1c
  **Inferred.** It makes one call to nic_vertex_info_set, at 0xc00000000006bcd0, from 0xc00000000006bf2c. The first argument is the constant 0xc00000000006be18, a1 is zero, a2 is its own first argument (a0 at entry) and a3 is its own fourth argument (a3 at entry). It returns the callee's v0 at 0xc00000000006bf38. Role (from name): a thin registration of the hub's vertex record; the meaning of the constant descriptor is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_bridge_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf40
  **Inferred.** It calls nic_vertex_info_set (0xc00000000006bcd0) at 0xc00000000006bf4c with the constant 0xc00000000006b66c as the first argument, its own a1 as a1, its own first argument (a0 at entry) as a2 and its own a3 as a3. It returns the callee's v0 at 0xc00000000006bf58. The call has the same shape as the hub variant at 0xc00000000006bf1c, except that the constant differs and a1 is its own a1 where the hub variant passes zero. Role (from name): a vertex-info registration for a bridge; what the descriptor constants select is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_hq4_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf60
  **Inferred.** It calls nic_vertex_info_set (0xc00000000006bcd0) at 0xc00000000006bf6c with the constant 0xc00000000006b66c as the first argument, its own a1 as a1, its own first argument (a0 at entry) as a2 and its own a3 as a3, the same call facts as the bridge variant at 0xc00000000006bf40. It returns the callee's v0 at 0xc00000000006bf78. Role (from name): a vertex-info registration for an hq4 device; whether it differs from the bridge variant in anything but its address is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_ioc3_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf80
  **Inferred.** When its third argument a2 is not zero, it stores a 4-byte word through a2 at 0xc00000000006bf94; the branch at 0xc00000000006bf8c skips that store when a2 is zero. The stored register is the one that holds 0x200000, which the call also passes as a5. It then calls nic_vertex_info_set (0xc00000000006bcd0) at 0xc00000000006bf9c with the constant 0xc00000000006b66c as the first argument, its own a1 as a1, its own first argument (a0 at entry) as a2 and a4, its own a3 as a3, and 0x200000 as a5. It returns the callee's v0 at 0xc00000000006bfa8. Role (from name): the ioc3 variant of the vertex-info registration; what the constant 0x200000 selects is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_vmc_add` in `6.5.22/unix.IP27` at 0xc00000000006bfb0
  **Inferred.** It allocates a 24-byte block with kern_malloc at 0xc00000000006bfc4 and writes three doublewords into it at offsets 0x0, 0x8 and 0x10. It then calls swap_int64 at 0xc00000000006bfe4 with the global 0xc0000000014c9da8 and the new block, and stores a doubleword through the value that call returns at 0xc00000000006bffc. The routine returns the kern_malloc result at 0xc00000000006c000. It looks like it creates and links a small record, but the facts do not show what swap_int64 does with its arguments, so the link step is open.
  Reviewed: 2026-11-12 (sonnet)

- `nic_vmc_check` in `6.5.22/unix.IP27` at 0xc00000000006c014
  **Inferred.** It tests the 8-byte global at 0xc0000000014c9da0 at 0xc00000000006c038 and, when it is nonzero, loops through a chain of records with 8-byte loads at 0xc00000000006c058, 0xc00000000006c064, 0xc00000000006c070 and 0xc00000000006c07c; the branches at 0xc00000000006c05c, 0xc00000000006c068, 0xc00000000006c074 and 0xc00000000006c080 test the loaded values against zero. In the loop it calls strstr at 0xc00000000006c048 with its own second argument (a1 at entry) as the first argument and a value not resolved by the facts as the second. When strstr returns nonzero (branch at 0xc00000000006c050) it makes an indirect call at 0xc00000000006c0a4 with its own first argument as a0, whose target the facts do not resolve. The return at 0xc00000000006c09c has an unresolved v0. Role (from name): a check of a VMC entry against a string; the source of the loop and the callee are not resolved, so the purpose is open.
  Reviewed: 2026-11-12 (sonnet)

- `sndrv_attach` in `6.5.22/unix.IP27` at 0xc00000000006c308
  **Inferred.** It makes one call, to sn0drv_attach through 0xc00000000006c0b4 at 0xc00000000006c310, and passes its four entry arguments on unchanged. It returns the callee's v0 at 0xc00000000006c31c. The body is a single forwarding call, so the routine appears to be an entry point that delegates the attach to sn0drv_attach.
  Reviewed: 2026-11-12 (sonnet)

- `sn0drv_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006c358
  **Inferred.** It is a large dispatcher of 2252 bytes. It subtracts 8 from its second argument and range-checks the result against 15 at 0xc00000000006c390. A value in range goes through a table load and an indirect jump at 0xc00000000006c39c, and an out-of-range value calls sndrv_ioctl_common at 0xc00000000006c3ac with its four entry arguments. Across its paths the facts show calls to device_info_get, hwgraph_info_get_LBL, check_router (0xc00000000006cc24), router_port_map_get, hwgraph_connectpt_get, router_port_mfg_info_get, memerror_get_stats, nic_hub_vertex_info (call at 0xc00000000006c788), md_perf_get_node_ctrl, md_perf_get_node_count, md_perf_node_enable, md_perf_node_disable, elsc_command, elsc_nvram_read, elsc_nvram_write, rou_fence_op (call at 0xc00000000006cba4), strstr and strtoull, with copyin and copyout moving user buffers. cap_able with the mask 0x2000000000 is called at several sites, for example 0xc00000000006c3e8, and some paths branch on its result. The facts show the call set and the data movement but not which request number reaches which path or what each means to a user, so the per-case roles are open.
  Reviewed: 2026-11-12 (sonnet)

- `check_router` in `6.5.22/unix.IP27` at 0xc00000000006cc24
  **Inferred.** It calls device_info_get at 0xc00000000006cc3c and compares the result with 1 at 0xc00000000006cc50. When the result is not 1 it stores a word through its fourth argument at 0xc00000000006cc58 and returns -1 at 0xc00000000006cc68. When the result is 1 it calls cap_able_cred at 0xc00000000006cc78 with its own third argument as a0 and the mask 0x2000000000 as a1, and when that returns nonzero it calls hwgraph_info_get_LBL at 0xc00000000006cc94 with its first argument, the constant 0xc0000000003c5ca0 and its own second argument as a2. It stores words through the fourth argument at two further sites, 0xc00000000006ccb4 and 0xc00000000006ccd0, and the facts show a single return whose value is -1 or 0. Role (from name): a permission and validity check on a router path; the facts do not show which failure each return code stands for.
  Reviewed: 2026-11-12 (sonnet)

- `do_reg_op` in `6.5.22/unix.IP27` at 0xc00000000006ccd4
  **Inferred.** It dispatches on its first argument: the value 1 selects a read and the value 2 selects a write. The read calls router_reg_read at 0xc00000000006cd1c, and the write calls router_reg_write at 0xc00000000006cd5c with the 8-byte value at [a3]. Each failure reports through net_errmsg and printf, with the message constants 0xc0000000003c5dc0 and 0xc0000000003c5df0, and the device field at offset 0x1a8 of its second argument. The register-access semantics come from the router helpers, which this routine does not show.
  Reviewed: 2026-11-12 (sonnet)

- `do_router_op` in `6.5.22/unix.IP27` at 0xc00000000006cd98
  **Inferred.** It reads a 2-byte request code from its first argument at 0xc00000000006cda8 and compares it against 2, 4, 8 and 10 at 0xc00000000006cdc8 to 0xc00000000006cde0. Its cases call do_reg_op at 0xc00000000006ce08, 0xc00000000006ce2c, 0xc00000000006ce48, 0xc00000000006ce58, 0xc00000000006ce68 and 0xc00000000006ce8c, with its own a1 and a2 passed on and the constant 1 in a0 at the first, third and fifth calls and 2 at the others. It loads 8-byte values at offset 0x8 at 0xc00000000006ce1c and 0xc00000000006ce70 and stores 8 bytes at offset 0x8 at 0xc00000000006ce40 and 0xc00000000006cea0; the facts do not show the base register of those accesses. It appears to expand one router request into one or more register operations, but the meaning of each request code and which case makes which call is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `rou_fence_op` in `6.5.22/unix.IP27` at 0xc00000000006cea4
  **Inferred.** It calls do_router_op at 0xc00000000006cec0 with the constant 0x10 as its third argument (a2) and branches to 0xc00000000006cf44 at 0xc00000000006ced8 if the result is negative. Otherwise it loads a byte at offset 0x8 of its second argument at 0xc00000000006cee8 (the branch at 0xc00000000006cef0 also goes to 0xc00000000006cf44 when a3 is zero), tests it against a mask at 0xc00000000006cf04 and, when the masked value is nonzero, makes a second do_router_op call at 0xc00000000006cf1c; a negative result there also goes to 0xc00000000006cf44. The branch at 0xc00000000006cf3c returns to 0xc00000000006cee4 while a4 is nonzero. The single return at 0xc00000000006cf60 gives the do_router_op result. Role (from name): a fence operation on a router; the flag byte, the mask and the loop's exit condition are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `prominfo_add` in `6.5.22/unix.IP27` at 0xc00000000006d194
  **Inferred.** It allocates a 16-byte record with kmem_alloc (a0 = 0x10, a1 = 0) at 0xc00000000006d1ac and stores a 4-byte word at offset 0x4 at 0xc00000000006d1b8. It then takes the lock at 0xc0000000014d1590 with mutex_spinlock at 0xc00000000006d1c4, and after the lock stores a 4-byte word at offset 0x0 at 0xc00000000006d1c8 and an 8-byte value at offset 0x8 at 0xc00000000006d1dc. It calls mutex_spinunlock on the same lock at 0xc00000000006d1e4; at that call the facts show a2 holding the kmem_alloc result and a3 holding the 8-byte value of the global at 0xc0000000014d1588, and the return is the unlock result. The routine appears to add a record to a list headed by that global, but the facts do not show the link operation itself or the base of the stores.
  Reviewed: 2026-11-12 (sonnet)

- `prominfo_del` in `6.5.22/unix.IP27` at 0xc00000000006d1f8
  **Inferred.** It takes the lock at 0xc0000000014d1590 with mutex_spinlock at 0xc00000000006d208 and reads the 8-byte global at 0xc0000000014d1588. The branch at 0xc00000000006d214 skips the list walk when that value is zero. In the walk it loads a 4-byte word at 0xc00000000006d220 and compares it with its first argument at 0xc00000000006d224, and loads an 8-byte link at 0xc00000000006d22c while that link is nonzero (branch at 0xc00000000006d234). On a match it goes to 0xc00000000006d258, loads and stores 8 bytes there (0xc00000000006d258 and 0xc00000000006d25c) and unlocks with mutex_spinunlock at 0xc00000000006d260. When the walk ends without a match it unlocks at 0xc00000000006d240 and returns that call's result at 0xc00000000006d250. Role (from name): removal of a node from the list, the counterpart of prominfo_add; which link fields are written is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `prominfo_nodeget` in `6.5.22/unix.IP27` at 0xc00000000006d270
  **Inferred.** It takes the lock at 0xc0000000014d1590 with mutex_spinlock at 0xc00000000006d284, the same lock prominfo_add and prominfo_del take, then tests the 8-byte list head at 0xc0000000014d1588 at 0xc00000000006d290. Its loop loads a 4-byte word at 0xc00000000006d298, compares it with its first argument at 0xc00000000006d29c and follows an 8-byte link loaded at 0xc00000000006d2a4 (loop branch at 0xc00000000006d2a8). It unlocks at 0xc00000000006d2b0 and 0xc00000000006d2d0, and the facts show a 4-byte load at 0xc00000000006d2dc after the second. The single return at 0xc00000000006d2c8 gives zero or a value the facts do not resolve. Role (from name): a lookup of a list node by the compared field, by the name and the shared lock; the field and what the result carries are not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpuprom_detailed_inventory_info_add` in `6.5.22/unix.IP27` at 0xc00000000006d2e0
  **Inferred.** It gets a CNode id with hubdev_cnodeid_get at 0xc00000000006d2f0 and calls klhwg_invent_alloc at 0xc00000000006d304 with a1 = 0x15 and a2 = 0x20. It then stores a 4-byte word at offset 0x14 at 0xc00000000006d314, a byte at offset 0x4 at 0xc00000000006d318, a 4-byte word at offset 0xc at 0xc00000000006d328 and a 4-byte word at offset 0x10 at 0xc00000000006d33c. The facts show loads from the fixed addresses 0x90000000100000a0 and 0x90000000100000a4, passed as a3 and a4 of hwgraph_info_add_LBL at 0xc00000000006d338 together with the label 0xc0000000003c5e20 as a1, the klhwg_invent_alloc result as a2 and a5 = 1. It then calls hwgraph_info_export_LBL at 0xc00000000006d348 with the same label and a2 = 0x20, and returns its result at 0xc00000000006d358. The meaning of the two fixed words is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpuprom_attach` in `6.5.22/unix.IP27` at 0xc00000000006d360
  **Inferred.** It gets the CNode id with hubdev_cnodeid_get at 0xc00000000006d36c and adds a character device with hwgraph_char_device_add at 0xc00000000006d384, using the constants 0xc0000000003c5e30 and 0xc0000000003c5e38. It then calls device_inventory_add with class 0x15, calls cpuprom_detailed_inventory_info_add at 0xc00000000006d3ac, sets device info with device_info_set, and records the PROM entry with prominfo_add at 0xc00000000006d3c4. It returns 0 at 0xc00000000006d3d8. The sequence reads as an attach that publishes a CPU PROM device, but the device's class constants are not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpuprom_map` in `6.5.22/unix.IP27` at 0xc00000000006d3e0
  **Inferred.** It looks up the node with prominfo_nodeget at 0xc00000000006d3f0 and takes the failure path when the result is zero (branch at 0xc00000000006d3fc); the facts show a return of 5 or the v_mapphys result at 0xc00000000006d494. It gets the PROM base with hubdev_prombase_get at 0xc00000000006d404 and the CNode id with hubdev_cnodeid_get at 0xc00000000006d410, then calls v_mapphys at 0xc00000000006d430 with its own second argument as a0 and the PROM base as a1; a nonzero result takes the same exit path as the failed lookup. After a successful mapping it makes loads at 0xc00000000006d454, 0xc00000000006d45c and 0xc00000000006d470, and makes one 8-byte store at 0xc00000000006d484 only when the byte loaded at 0xc00000000006d45c is nonzero. Role (from name): mapping the CPU's PROM window into kernel space; what the store updates is not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpuprom_unmap` in `6.5.22/unix.IP27` at 0xc00000000006d4ac
  **Established.** The routine is 8 bytes long, makes no calls and no stores, and returns 0 at 0xc00000000006d4ac. Its only effect is that return value.
  Reviewed: 2026-11-12 (sonnet)

- `hubspc_init` in `6.5.22/unix.IP27` at 0xc00000000006d4b4
  **Inferred.** It calls hubdev_register twice, at 0xc00000000006d4bc with a0 = 0xc000000000279850 and its own a1 to a3 passed on, and at 0xc00000000006d4c4 with a0 = 0xc00000000006d360, the address of cpuprom_attach. It then calls spinlock_init at 0xc00000000006d4d4 on the lock at 0xc0000000014d1590, the lock the prominfo routines take, with a1 = 0xc0000000003c5e40, and returns that call's result at 0xc00000000006d4e0. Role (from name): the init step for hub space; what the first registered address is and what the registrations install are not shown by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `hubspc_open` in `6.5.22/unix.IP27` at 0xc00000000006d4e8
  **Inferred.** It calls device_info_get at 0xc00000000006d500 on the word at its first argument, which it loads at 0xc00000000006d504. A device type of 0 goes to mem_refcnt_open at 0xc00000000006d54c, and a type of 1 first runs a capability check, cap_able with the mask 0x2000000000, at 0xc00000000006d530. The other paths return the constant 0x13 or the mem_refcnt_open result. The routine appears to be the open handler for the hub device node, but the meaning of the type values is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hubspc_close` in `6.5.22/unix.IP27` at 0xc00000000006d55c
  **Inferred.** It calls device_info_get at 0xc00000000006d570 and then mem_refcnt_close at 0xc00000000006d5b8, with the constant 0x1 and the device info result. The branch at 0xc00000000006d580 sends a device type of zero to the close call, and the branch at 0xc00000000006d588 sends a type of 1 along a path where the facts show no call. The routine is the close counterpart of hubspc_open, but what the type-1 path does is not visible.
  Reviewed: 2026-11-12 (sonnet)

- `hubspc_map` in `6.5.22/unix.IP27` at 0xc00000000006d5c8
  **Inferred.** A branch on a3 at 0xc00000000006d5e8 precedes two device_info_get calls, at 0xc00000000006d5f0 and 0xc00000000006d5f8, and the type returned is tested at 0xc00000000006d608 and 0xc00000000006d610. A type of 0 goes to mem_refcnt_mmap at 0xc00000000006d63c and a type of 1 goes to cpuprom_map at 0xc00000000006d654, both with the constant 0x1 in a7. The return at 0xc00000000006d624 is 0x6 or a value the facts do not resolve. Role (from name): the mmap dispatcher for hub devices; the device type meanings are not given by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `hubspc_unmap` in `6.5.22/unix.IP27` at 0xc00000000006d664
  **Inferred.** It calls device_info_get at 0xc00000000006d670 and then branches on the returned type at 0xc00000000006d680 and 0xc00000000006d688. A type of 0 goes to mem_refcnt_unmap at 0xc00000000006d6b4, and a type of 1 goes to cpuprom_unmap at 0xc00000000006d6a0, both with the constants 0x13 and 0x1 in a4 and a5. It is the unmap counterpart of hubspc_map. The type values are not named in the facts, so the mapping from type to handler rests on the call pattern alone.
  Reviewed: 2026-11-12 (sonnet)

- `hubspc_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006d6c4
  **Inferred.** It calls device_info_get at 0xc00000000006d6e0 and branches on the returned type at 0xc00000000006d6f0 and 0xc00000000006d6f8. A type of 0 leads to mem_refcnt_ioctl at 0xc00000000006d730, with its four entry arguments passed on, and a type of 1 returns without a call. The routine is the ioctl dispatcher for the hub device, but the facts do not show what the type-1 case does.
  Reviewed: 2026-11-12 (sonnet)
