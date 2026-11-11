# Routine roles: tier-1 batch 11

Role statements for 24 routines in the shipped 6.5.22 IP22boot and IP27boot kernel objects. Each entry names the instruction facts it rests on (registers, constants, branches, callees) by address, then says what role those facts support. Names come from the binaries' own symbol tables. A grade of inferred means the role is a reading of the body and is open where the entry says so. The grade is established only where the whole body is shown and needs no interpretation.

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP22boot/os.a(time.o)` at 0xa8
  **Inferred.** Copies a set of words from a record (reached through a2 and the pointer at [a2+488]) into the caller's buffer at a1. It takes a lock on the record at 0xe0 and releases it at 0x108, gets the remaining time from chktimeout_tick at 0x160 and 0x184, and converts it with tick_to_timeval at 0x19c into a1+8. The name suggests a getitimer-style read, but the facts do not say which timer is read.

- `scsiha_ioctl` in `6.5.22/usr/cpu/sysgen/IP22boot/scsiha.o` at 0x198
  **Inferred.** Dispatches on the command in a1. Command 0x7306 (compared at 0x1e8) takes a path that builds a hardware-graph entry for the controller, through hwgraph_connectpt_get at 0x270, device_controller_num_get at 0x27c, hwgraph_path_to_dev at 0x290 and hwgraph_edge_add at 0x2e8. Commands below 0xb take a copyin path into a 12-byte buffer from kmem_zalloc at 0x1f0, and the buffer is freed at 0x23c. The facts do not show what command 0x7306 means, so the ioctl reading stops there.

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP27boot/cn.o` at 0xaac
  **Inferred.** Calls gfx_earlyinit at 0xac4, initializes a lock and a mutex on the cn object (spinlock_init at 0xb2c, init_mutex at 0xb44), and writes several fields of that object. It compares the console byte at arg_console with 0x67 and 0x47, and calls tp_init at 0xb8c or du_init at 0xba8 depending on that comparison and on is_specified at 0xb5c. The role looks like console setup for the IP27 cn object, but the facts do not fully resolve which init call runs for which console setting.

- `bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** Acquires a bit lock on the structure in a2 with an ll/sc loop on the words at +0xa8 through +0xbc. The attempt is made by try_bitlock at 0x16cc and 0x17f8, and the routine updates fields at +0x98 and +0xa0 as it goes. A failure path reports through cmn_err at 0x18bc. The debug name fits the report path, but the facts do not show the condition that reaches it.

- `_64bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** This is the 64-bit form of the acquire path above. It uses the same ll/sc words at +0xa8 through +0xbc and the same field updates, with try_64bitlock at 0x1f70 and 0x209c in place of try_bitlock. The report goes through cmn_err at 0x2160. The facts do not show any difference between the two forms beyond the helper they call.

- `_64bitunlockdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** This is the release side for the pair above. It compares the halfword at a2+0x2a with the halfword at the fixed address 0xffffffffffffc420 (a gp-relative slot the facts do not name), and a mismatch branches to 0x22a8 instead of the normal store path. It writes fields from a2+0x2a through a2+0x80, and it reports through cmn_err at 0x22c0 and 0x22d8. The facts do not show which condition reaches each report.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** Looks up this CPU's entry in the gfx_flow_cpu_attachments table, indexed by the halfword at 0xffffffffffffc420 shifted by 3. If the entry pointer is nonzero (branch at 0x1c), it calls through a function pointer at 0x7c with arguments read from the entry at +0x20 and +0x28. It also writes a word into the hub table at 0x9200000001000000 (store at 0x58). The name points at a high-water interrupt, but the callee is not named in the facts.

- `gfx_flow_alloc` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x23c
  **Inferred.** Allocates a 0x70-byte flow record with kmem_alloc at 0x2ac and fills its fields, with stores at offsets up to +0x68. It looks up the widget with xwidget_info_get at 0x2c4 and xwidget_info_id_get at 0x2d8, and the master node with xwidget_info_master_get at 0x3e8 and master_node_get at 0x3f0. It checks the globals gfx_flow_igfx_owners and hub_use_transaction_credits, may allocate a buffer with kvalloc at 0x3a0, and calls gfx_flow_init at 0x458. The error reports at 0x574 through 0x5b4 are followed by kmem_free at 0x5c0; the value ranges tested at 0x320, 0x520 and 0x548 are not named by the facts.

- `gfx_flow_dealloc` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x5dc
  **Inferred.** A short wrapper that frees a flow record. It loads two fields at +0x50 and +0x54, then calls kmem_free at 0x610 with the record, size 0x70 and its two extra arguments, and returns that call's result. The two loaded fields feed no branch in the body, so their purpose is unclear.

- `gfx_flow_attach_me` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x6b8
  **Inferred.** Attaches a flow record to the current CPU. It writes this CPU's slot in gfx_flow_cpu_attachments (the stored value is not resolved), and writes the record's own fields at +0x60 and +0x68. It updates the hub table at 0x9200000001000000 (stores at 0x754 through 0x78c), looks up a node id in compact_to_nasid_node at 0x7a4, and calls err_int_block_mask at 0x7f8. A branch at 0x704 jumps forward to that call while a register is below 3, and an always-taken branch at 0x808 returns to 0x70c.

- `gfx_flow_detach_me` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x85c
  **Inferred.** The reverse of the attach path. It writes through the pointer at a0+0x58, writes this CPU's attachment slot, and updates the same hub table entries. It calls err_int_unblock_mask at 0x970 with block mask 0x8 and a CPU index, and passes the constant 0xdeadbabe in a5, whose role the facts do not show. A branch at 0x924 jumps toward that call while a register is below 3, and an always-taken branch at 0x988 returns to 0x92c; the return value is the word at 0x9200000001000098 or the callee's result.

- `ef_ioc3eaddr` in `6.5.22/usr/cpu/sysgen/IP27boot/if_ef.o` at 0x73c
  **Inferred.** Gets the Ethernet address for an IOC3 Ethernet device. It reads the device structure pointer at a0+0x398, passes two words from that structure at +48 and +52 to nic_get_eaddr at 0x754, and returns 0 or 5 depending on the result. A cmn_err report at 0x780 uses the field at a0+0x38c, but the facts do not say which branch is the error path, so the reading is partial.

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP27boot/ioc3_pckm.o` at 0x5c
  **Inferred.** Attaches the IOC3 keyboard and mouse controller. It makes three kmem_zalloc allocations (one of 0x68 bytes and two of 0x58 bytes) and a mutex, probes the console and subdevice state with ioc3_is_console and ioc3_subdev_enabled, and connects pckm_intr through ioc3_intr_connect at 0x450 on bit 0x400000. It registers character devices with hwgraph_char_device_add at 0x1b8 and 0x310, and sends the command bytes 0xf5 and 0xf4 (device not resolved in the facts) and then 0xf6 to the second device, each reply checked by data_check against 0xfa. The byte values match the common PS/2 command set, which is an outside reading that the binary itself does not confirm.

- `eiattach` in `6.5.22/usr/cpu/sysgen/IP27boot/ioc3ei.o` at 0x4a8
  **Inferred.** Attaches an IOC3 device whose name suggests an Ethernet interface. It skips the attach when ioc3_subdev_enabled reports subdevice 7 as disabled (call at 0x4b8). It allocates a 0x38-byte state block with kmem_zalloc at 0x510 and a 0x80000-byte buffer with kvpalloc at 0x548, publishes character devices at 0x590 and records inventory at 0x5cc. It connects ioc3_ei_intr through ioc3_intr_connect at 0x610 on bit 0x18000000, and the facts show only the subdevice number as the device class.

- `start` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x13a0
  **Inferred.** Early entry of the IP27 boot kernel object. It reads the fixed hub word at 0x9200000001600000 (at 0x13d0), calls mapped_kernel_setup_tlb at 0x13e8 with its own arguments, and then jumps through a register to a relocated target at 0x1408 whose callee is not resolved. The facts show no code reached after that jump, so the rest of the 544 bytes is not covered here.

- `get_nasid` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42b4
  **Inferred.** Loads one 64-bit word from the fixed address 0x9200000001600000 (at 0x42b8) and returns. The facts do not resolve the return value's source, so the loaded word is the likely result but is not shown to be. Callers combine the result with shifts and compares (cpu_node_probe at 0x6030 shifts it by 0x20), which fits a node id.

- `get_slice` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42d0
  **Inferred.** Loads the 64-bit word at 0x9200000001000020 (at 0x42d4) and returns it unchanged. The same word is read by getcpuid at 0xdd64 and passed to nasid_slice_to_cpuinfo. The name suggests a slice number, but the body does no masking or shifting that would confirm it.

- `is_fine_dirmode` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42e0
  **Inferred.** Loads the same fixed word that get_nasid reads (at 0x42e4) and returns it without testing any bit in the body. The name suggests a directory-mode check on a hardware bit, but the facts show no bit tested, so the role is open. mlreset calls it at 0xcec0, and the facts do not show how that caller uses the result.

- `ni_reset_port` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x43b0
  **Inferred.** Writes one 64-bit value to the fixed address 0x9200000001600008 (at 0x43bc) and returns that address. The stored value is not resolved, so what the write does is not shown. restart writes the same address at 0x10534, which ties the two routines to one port-reset register. The name suggests a port reset.

- `cpu_node_probe` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x5f48
  **Inferred.** The name suggests a probe of one node's CPUs. It calls get_nasid at 0x6014, reads a node-local word through an address built from the node id (at 0x6030, offset 0xd0), and passes a mask to do_cpumask at 0x6078. A loop compares a counter with 0x40, 0x80 and 0x100, and a failed check calls assfail at 0x6150 with line 0xb9. The facts do not say what those three thresholds mean.

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0xcd30
  **Inferred.** Machine-level setup for the boot kernel. In the order the facts show, it programs performance-counter register pairs (r1nkperf_control_register_set and r1nkperf_data_register_set at 0xcd44 through 0xce18), flushes the TLB and cache (flush_tlb at 0xcdd4, flush_cache at 0xcddc), and picks a branch-prediction mode from r10k_bpmode (calls at 0xce28 through 0xd0cc). It then probes nodes (redo_nodes at 0xcee4, cpu_node_probe at 0xcf00) and runs hub, memory and interrupt-group init (iograph_early_init at 0xd020 through intrgroup_init at 0xd0bc). Three assfail calls (0xd0e4, 0xd100, 0xd11c) guard the path, and the purpose of each argument is not shown.

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0xdd44
  **Inferred.** Returns the CPU id of the caller. It raises interrupt priority (splhi at 0xdd50), gets the node id from get_nasid at 0xdd58, and looks up the CPU record for that node and slice with nasid_slice_to_cpuinfo at 0xdd6c, using the word at 0x9200000001000020 that get_slice returns. If the record pointer is null it asserts (assfail at 0xdda8, line 0x525), and otherwise it returns the halfword at +0x1e of the record. The field at +0x1e is not named in the facts.

- `restart` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x103e0
  **Inferred.** The restart path. It checks panic_level and maxnodes, then loops over the nodes with delays of 0xf4240 and 0x2dc6c0 units (us_delay at 0x1046c and 0x104a0, unit not shown). Inside the loop it calls sysctlr_power_off at 0x10498 when power_off_flag is set, and ni_reset_port at 0x104d0 otherwise. It then gets the node id with get_nasid, initializes the ELSC system controller (elsc_init at 0x104f4), requests a system reset (elsc_system_reset at 0x104fc), and prints through printf at 0x10518 when that reset returns a negative value.

- `page_local_calias` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x11cd4
  **Inferred.** Loads the word at 0x9200000001000028 (at 0x11cd8) and takes one of two paths depending on whether that word is zero (branch at 0x11ce8). The other path compares two register values with an unsigned test (at 0x11cfc). The return value is 0 or 1. The name suggests a check on a local alias of a page, which the 0 or 1 result fits, but the facts do not show what the two compared values are.
