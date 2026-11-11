# Tier-2 routine readings, kudzu round 2 batch 7

- `nic_hub_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf1c
  **Inferred.** It makes one call to nic_vertex_info_set through the pointer at 0xc00000000006bcd0, at 0xc00000000006bf2c. The first argument is the constant 0xc00000000006be18, a1 is zero, and a2 and a3 pass through from its own arguments. It returns the callee's v0 at 0xc00000000006bf38. The routine looks like a thin registration of the hub's vertex record, but the meaning of the constant descriptor is not settled by these facts.

- `nic_bridge_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf40
  **Inferred.** It calls nic_vertex_info_set through 0xc00000000006bcd0 at 0xc00000000006bf4c, with the constant 0xc00000000006b66c as the first argument and a1 and a2 taken from its own entry arguments. It returns the callee's v0 at 0xc00000000006bf58. Its shape matches the hub variant at 0xc00000000006bf1c, so the difference between them lies in the constant descriptor and the argument order, which these facts do not resolve.

- `nic_hq4_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf60
  **Inferred.** It makes the same call as the bridge variant, through 0xc00000000006bcd0 at 0xc00000000006bf6c, with the constant 0xc00000000006b66c and the same entry-argument mapping. It returns the callee's v0 at 0xc00000000006bf78. The two routines differ only in their addresses, so any difference in what they register must come from the callee's view of the constant, which is not visible here.

- `nic_ioc3_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf80
  **Inferred.** It stores the 4-byte word of its third argument through the pointer a2 at 0xc00000000006bf94, but only when a2 is not zero, because the branch at 0xc00000000006bf8c skips that store when a2 is zero. It then calls nic_vertex_info_set through 0xc00000000006bcd0 at 0xc00000000006bf9c, with the constant 0xc00000000006b66c, the same entry-argument pass-through as the other vertex variants, and the constant 0x200000 in a5. It returns the callee's v0 at 0xc00000000006bfa8. The conditional store and the extra constant set this variant apart, and the facts do not say what the constant 0x200000 selects.

- `nic_vmc_add` in `6.5.22/unix.IP27` at 0xc00000000006bfb0
  **Inferred.** It allocates a 24-byte block with kern_malloc at 0xc00000000006bfc4 and writes three doublewords into it at offsets 0x0, 0x8 and 0x10. It then calls swap_int64 at 0xc00000000006bfe4 with the global 0xc0000000014c9da8 and the new block, and stores a doubleword through the value that call returns at 0xc00000000006bffc. The routine returns the kern_malloc result at 0xc00000000006c000. It looks like it creates and links a small record, but the facts do not show what swap_int64 does with its arguments, so the link step is open.

- `nic_vmc_check` in `6.5.22/unix.IP27` at 0xc00000000006c014
  **Inferred.** It tests the 8-byte global at 0xc0000000014c9da0 at 0xc00000000006c038 and loops over five 8-byte loads, with the loop controlled by the registers s0 and s1 at 0xc00000000006c05c and 0xc00000000006c068. Each pass calls strstr at 0xc00000000006c048 to search for its second argument, and a match at 0xc00000000006c050 leads to an indirect call at 0xc00000000006c0a4 with its first argument, whose target the facts do not resolve. Its return value depends on that unresolved call. The name suggests a check of a VMC entry against a string, but the loop's source list and the callee are not visible here, so the purpose is open.

- `sndrv_attach` in `6.5.22/unix.IP27` at 0xc00000000006c308
  **Inferred.** It makes one call, to sn0drv_attach through 0xc00000000006c0b4 at 0xc00000000006c310, and passes its four entry arguments on unchanged. It returns the callee's v0 at 0xc00000000006c31c. The body is a single forwarding call, so the routine appears to be an entry point that delegates the attach to sn0drv_attach.

- `sn0drv_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006c358
  **Inferred.** It is a large dispatcher of 2252 bytes. Its request number is range-checked at 0xc00000000006c390 against a range of 15 after subtracting 8, and the value indexes a jump table at 0xc0000000003c5d30 with an indirect jump at 0xc00000000006c39c. Its cases call device_info_get and hwgraph_info_get_LBL on the device, check_router at 0xc00000000006cc24, router_port_map_get, nic_hub_vertex_info at 0xc00000000006c788, md_perf_get_node_ctrl and md_perf_get_node_count, elsc_nvram_read and elsc_nvram_write, rou_fence_op at 0xc00000000006cba4, and the copyin and copyout helpers for user buffers. It checks capabilities with cap_able and the mask 0x2000000000. The facts show the case set and the data movement but not what each case means to a user, so the per-case roles are open.

- `check_router` in `6.5.22/unix.IP27` at 0xc00000000006cc24
  **Inferred.** It calls device_info_get at 0xc00000000006cc3c, then cap_able_cred at 0xc00000000006cc78 with the mask 0x2000000000, and hwgraph_info_get_LBL at 0xc00000000006cc94 with the constant 0xc0000000003c5ca0. It writes results through its fourth argument at three sites and returns either -1 or 0 at 0xc00000000006cc68. The routine looks like a permission and validity check on a router path, but the facts do not show which failure each return code stands for.

- `do_reg_op` in `6.5.22/unix.IP27` at 0xc00000000006ccd4
  **Inferred.** It dispatches on its first argument: the value 1 selects a read and the value 2 selects a write. The read calls router_reg_read at 0xc00000000006cd1c, and the write calls router_reg_write at 0xc00000000006cd5c with the 8-byte value at [a3]. Each failure reports through net_errmsg and printf, with the message constants 0xc0000000003c5dc0 and 0xc0000000003c5df0, and the device field at offset 0x1a8 of its second argument. The register-access semantics come from the router helpers, which this routine does not show.

- `do_router_op` in `6.5.22/unix.IP27` at 0xc00000000006cd98
  **Inferred.** It reads a 2-byte request code from its first argument at 0xc00000000006cda8, and compares it against 2, 4, 8 and 10 at 0xc00000000006cdc8 to 0xc00000000006cde0. Each case calls do_reg_op at 0xc00000000006ce08, 0xc00000000006ce2c, 0xc00000000006ce48, 0xc00000000006ce58, 0xc00000000006ce68 or 0xc00000000006ce8c, with the operation 1 or 2 and the register set by the case. It writes 8-byte fields back into its request block at offset 0x8 at 0xc00000000006ce40 and 0xc00000000006cea0. The routine expands one router request into one or more register operations, but the meaning of each request code is not in the facts.

- `rou_fence_op` in `6.5.22/unix.IP27` at 0xc00000000006cea4
  **Inferred.** It calls do_router_op at 0xc00000000006cec0 with the constant 0x10 as its third argument, and returns at once if the result is negative. It then reads a byte at offset 0x8 of its second argument at 0xc00000000006cee8 and ANDs it with a mask before a second do_router_op call at 0xc00000000006cf1c. A loop at 0xc00000000006cf3c repeats while a4 is nonzero, and the routine returns the callee's result. The name suggests a fence on a router, but the flag byte and the loop's exit condition are not settled by these facts.

- `prominfo_add` in `6.5.22/unix.IP27` at 0xc00000000006d194
  **Inferred.** It allocates a 16-byte record with kmem_alloc at 0xc00000000006d1ac, writes a 4-byte word at offset 0x4, another at offset 0x0, and an 8-byte value at offset 0x8, then takes the lock at 0xc0000000014d1590 with mutex_spinlock. It unlocks with mutex_spinunlock at 0xc00000000006d1e4, passing the current 8-byte value of the global at 0xc0000000014d1588. The routine appears to add a node to a list headed by that global, but the facts do not show the link operation itself.

- `prominfo_del` in `6.5.22/unix.IP27` at 0xc00000000006d1f8
  **Inferred.** It takes the lock at 0xc0000000014d1590 with mutex_spinlock at 0xc00000000006d208 and reads the 8-byte global at 0xc0000000014d1588. The branch at 0xc00000000006d214 skips the list walk when that value is zero, and the walk compares each node's field with its argument at 0xc00000000006d224. On a match it stores 8 bytes at 0xc00000000006d25c to unlink the node, then unlocks at 0xc00000000006d240 and 0xc00000000006d260. The routine looks like the removal half of prominfo_add, but the link fields it writes are not named in the facts.

- `prominfo_nodeget` in `6.5.22/unix.IP27` at 0xc00000000006d270
  **Inferred.** It takes the same lock at 0xc0000000014d1590 as prominfo_add and prominfo_del, then tests the list head at 0xc00000000006d290. It walks the list comparing a field with its argument at 0xc00000000006d29c, and loads 4-byte words at 0xc00000000006d298 and 0xc00000000006d2dc along the way. It unlocks at 0xc00000000006d2b0 and 0xc00000000006d2d0, and returns zero or a node pointer. The lookup role follows from the shared lock and the return values, though the compared field is not named.

- `cpuprom_detailed_inventory_info_add` in `6.5.22/unix.IP27` at 0xc00000000006d2e0
  **Inferred.** It gets a CNode id with hubdev_cnodeid_get at 0xc00000000006d2f0 and allocates a 32-byte inventory record with klhwg_invent_alloc, class 0x15, at 0xc00000000006d304. It fills the record with a byte at offset 0x4, and two 4-byte words at offsets 0xc and 0x10 taken from the fixed addresses 0x90000000100000a0 and 0x90000000100000a4. It then adds the record under a label with hwgraph_info_add_LBL at 0xc00000000006d338 and exports it with hwgraph_info_export_LBL at 0xc00000000006d348. The meaning of the two fixed words is not settled by the facts.

- `cpuprom_attach` in `6.5.22/unix.IP27` at 0xc00000000006d360
  **Inferred.** It gets the CNode id with hubdev_cnodeid_get at 0xc00000000006d36c and adds a character device with hwgraph_char_device_add at 0xc00000000006d384, using the constants 0xc0000000003c5e30 and 0xc0000000003c5e38. It then calls device_inventory_add with class 0x15, calls cpuprom_detailed_inventory_info_add at 0xc00000000006d3ac, sets device info with device_info_set, and records the PROM entry with prominfo_add at 0xc00000000006d3c4. It returns 0 at 0xc00000000006d3d8. The sequence reads as an attach that publishes a CPU PROM device, but the device's class constants are not named in the facts.

- `cpuprom_map` in `6.5.22/unix.IP27` at 0xc00000000006d3e0
  **Inferred.** It looks up the node with prominfo_nodeget at 0xc00000000006d3f0 and returns 5 on a failed lookup at 0xc00000000006d494. It gets the PROM base with hubdev_prombase_get and the CNode id with hubdev_cnodeid_get, then maps the physical range with v_mapphys at 0xc00000000006d430. The routine also writes 8-byte entries through a computed address derived from the table at 0xc0000000014d3800. The name suggests mapping the CPU's PROM window into kernel space, but the facts do not show what the table entry stores.

- `cpuprom_unmap` in `6.5.22/unix.IP27` at 0xc00000000006d4ac
  **Established.** The routine is 8 bytes long, makes no calls and no stores, and returns 0 at 0xc00000000006d4ac. Its only effect is that return value.

- `hubspc_init` in `6.5.22/unix.IP27` at 0xc00000000006d4b4
  **Inferred.** It registers two hub device handlers with hubdev_register at 0xc00000000006d4bc and 0xc00000000006d4c4. The first registration passes the address 0xc000000000279850 with its entry arguments, and the second passes the address of cpuprom_attach, 0xc00000000006d360. It then initializes the lock at 0xc0000000014d1590 with spinlock_init at 0xc00000000006d4d4, which is the same lock the prominfo routines take. The routine looks like the init step for the hub space, but the registered table is not named in these facts.

- `hubspc_open` in `6.5.22/unix.IP27` at 0xc00000000006d4e8
  **Inferred.** It calls device_info_get at 0xc00000000006d500 on the word at its first argument, which it loads at 0xc00000000006d504. A device type of 0 goes to mem_refcnt_open at 0xc00000000006d54c, and a type of 1 first runs a capability check, cap_able with the mask 0x2000000000, at 0xc00000000006d530. The other paths return the constant 0x13 or the mem_refcnt_open result. The routine appears to be the open handler for the hub device node, but the meaning of the type values is not in the facts.

- `hubspc_close` in `6.5.22/unix.IP27` at 0xc00000000006d55c
  **Inferred.** It calls device_info_get at 0xc00000000006d570 and then mem_refcnt_close at 0xc00000000006d5b8, with the constant 0x1 and the device info result. The branch at 0xc00000000006d580 sends a device type of zero to the close call, and the branch at 0xc00000000006d588 sends a type of 1 along a path where the facts show no call. The routine is the close counterpart of hubspc_open, but what the type-1 path does is not visible.

- `hubspc_map` in `6.5.22/unix.IP27` at 0xc00000000006d5c8
  **Inferred.** It calls device_info_get twice, at 0xc00000000006d5f0 and 0xc00000000006d5f8, and branches on the returned type at 0xc00000000006d608 and 0xc00000000006d610. A type of 0 goes to mem_refcnt_mmap at 0xc00000000006d63c, and a type of 1 goes to cpuprom_map at 0xc00000000006d654, both with the constant 0x1 in a7. The return value is 0x6 or the callee's result. The routine looks like the mmap dispatcher for hub devices, and the device type meanings are not given by the facts.

- `hubspc_unmap` in `6.5.22/unix.IP27` at 0xc00000000006d664
  **Inferred.** It calls device_info_get at 0xc00000000006d670 and then branches on the returned type at 0xc00000000006d680 and 0xc00000000006d688. A type of 0 goes to mem_refcnt_unmap at 0xc00000000006d6b4, and a type of 1 goes to cpuprom_unmap at 0xc00000000006d6a0, both with the constants 0x13 and 0x1 in a4 and a5. It is the unmap counterpart of hubspc_map. The type values are not named in the facts, so the mapping from type to handler rests on the call pattern alone.

- `hubspc_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006d6c4
  **Inferred.** It calls device_info_get at 0xc00000000006d6e0 and branches on the returned type at 0xc00000000006d6f0 and 0xc00000000006d6f8. A type of 0 leads to mem_refcnt_ioctl at 0xc00000000006d730, with its four entry arguments passed on, and a type of 1 returns without a call. The routine is the ioctl dispatcher for the hub device, but the facts do not show what the type-1 case does.
