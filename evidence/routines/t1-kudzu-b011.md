# Routine roles: tier-1 batch 11

Role statements for 24 routines in the shipped 6.5.22 IP22boot and IP27boot kernel objects. Each entry names the instruction facts it rests on (registers, constants, branches, callees) by address, then says what role those facts support. Names come from the binaries' own symbol tables. A grade of inferred means the role is a reading of the body and is open where the entry says so. The grade is established only where the whole body is shown and needs no interpretation.

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP22boot/os.a(time.o)` at 0xa8
  **Inferred.** Copies a set of words from a record (reached through a2 and the pointer at [a2+488]) into the caller's buffer at a1: it loads at 0xe8 through 0x100 and stores at 0xec through 0x10c. It takes a lock on the record with mutex_lock at 0xe0 and releases it with mutex_unlock at 0x108. It calls chktimeout_tick at 0x160 (a0 = 0x1, reached when the fastclock word tested at 0x14c is nonzero) and at 0x184 (a0 = 0x0), and passes the result to tick_to_timeval at 0x19c, with a1+8 as its second argument. Role (from name): a getitimer-style read; the facts do not say which timer is read or what chktimeout_tick returns.
  Reviewed: 2026-11-12 (sonnet)

- `scsiha_ioctl` in `6.5.22/usr/cpu/sysgen/IP22boot/scsiha.o` at 0x198
  **Inferred.** Compares the command in a1 with 0x7306 (at 0x1e8) and tests it against 0xb as a signed value (at 0x1fc). When the command equals 0x7306 the branch goes to 0x270, where it calls hwgraph_connectpt_get at 0x270, device_controller_num_get at 0x27c, hwgraph_path_to_dev at 0x290 and hwgraph_traverse at 0x2c0; hwgraph_edge_add at 0x2e8 is reached only when the result of hwgraph_traverse equals 2 (branch at 0x2cc). A command that is not 0x7306 and is 0xb or above (the branch at 0x1fc is not taken) goes on to copyin at 0x20c, with a2 at entry as its source, into the 12-byte buffer from kmem_zalloc at 0x1f0; kern_free at 0x23c releases that buffer when the value tested at 0x230 is nonzero. The facts do not show what command 0x7306 means or what the commands below 0xb do beyond the branch at 0x1fc, so the ioctl reading stops there.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP27boot/cn.o` at 0xaac
  **Inferred.** Calls gfx_earlyinit at 0xac4, initializes a lock and a mutex on the cn object (spinlock_init at 0xb2c, init_mutex at 0xb44), and writes several fields of that object. When is_specified (0xb5c, called with arg_console) returns nonzero it loads the console byte at arg_console and compares it with 0x67 and 0x47 (0xb74, 0xb7c); a match branches to 0xbb8, and otherwise the path falls to tp_init at 0xb8c. du_init at 0xba8 is the target of the branch at 0xb04 when a1 is zero. The facts do not resolve which init call runs for which console setting. Role (from name): console setup for the IP27 cn object.
  Reviewed: 2026-11-12 (sonnet)

- `bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** Calls try_bitlock at 0x16cc and 0x17f8 on its arguments, and around those calls runs ll/sc loops that update words of the structure in a2: +0xbc and +0xb8 near the start, then +0xac, +0xa8 and +0xb0 (0x1818 through 0x1860) and +0xa8 again (0x1888). The addends in the facts are +1 (and -1 for +0xa8 at 0x1888), so these loops read as counters rather than the lock word; which word holds the lock is not shown. It also stores to +0x98 and +0xa0 and to other fields (0x1754 through 0x17c8). A cmn_err call at 0x18bc is the last call, and the facts do not show the condition that reaches it. Role (from name): a debug variant of a bit-lock acquire.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** Has the same shape as bitlockdebug: ll/sc loops that update +0xbc, +0xb8, +0xac, +0xa8 and +0xb0 of the structure in a2 (increments, and a decrement of +0xa8 at 0x212c), stores to +0x98 and +0xa0, and try_64bitlock at 0x1f70 and 0x209c where bitlockdebug calls try_bitlock. A cmn_err call at 0x2160 is the last call. The facts show no difference between the two beyond the helper they call. Role (from name): the 64-bit debug variant of the acquire path.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitunlockdebug` in `6.5.22/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** Compares the halfword at a2+0x2a with the halfword at the fixed address 0xffffffffffffc420 (a gp-relative slot the facts do not name); a mismatch (bne at 0x2208) branches to 0x22a8 instead of the normal store path. It stores fields from a2+0x2a through a2+0x80, and calls cmn_err at 0x22c0 and 0x22d8. The facts do not show which condition reaches each report. Role (from name): the release side of the 64-bit debug lock.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** Looks up this CPU's entry in the gfx_flow_cpu_attachments table, indexed by the halfword at 0xffffffffffffc420 shifted by 3. If the entry pointer is nonzero (branch at 0x1c), it makes an indirect call at 0x7c to a function the facts do not name, with a0 read from the entry at +0x28 and a2 from the entry at +0x20, and a3 the entry itself. When the entry pointer is zero the path stores at 0x58 and returns at 0x60; the facts do not resolve the address of that store. The name points at a high-water interrupt, but the callee is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x23c
  **Inferred.** Allocates a 0x70-byte flow record with kmem_alloc at 0x2ac and fills its fields, with stores at offsets up to +0x68. It looks up the widget with xwidget_info_get at 0x2c4 and xwidget_info_id_get at 0x2d8, and the master node with xwidget_info_master_get at 0x3e8 and master_node_get at 0x3f0. It checks the globals gfx_flow_igfx_owners and hub_use_transaction_credits, may allocate a buffer with kvalloc at 0x3a0, and calls gfx_flow_init at 0x458. The error reports at 0x574 through 0x5b4 are followed by kmem_free at 0x5c0; the value ranges tested at 0x320, 0x520 and 0x548 are not named by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x5dc
  **Inferred.** A short wrapper that frees a flow record. It loads two fields at +0x50 and +0x54, then calls kmem_free at 0x610 with the record, size 0x70 and its two extra arguments, and returns that call's result. The two loaded fields feed no branch in the body, so their purpose is unclear.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x6b8
  **Inferred.** Role (from name): attaches a flow record to the current CPU. The facts show a store at 0x6dc (the return value is the address of this CPU's slot in gfx_flow_cpu_attachments, the stored value is not resolved) and stores to the record's fields at +0x60 and +0x68 (0x6c8, 0x6e0). Stores at 0x754, 0x770 and 0x78c are to addresses the facts do not resolve. It calls err_int_block_mask at 0x7f8 with a0 = 0x8 and a2 = gfx_flow_attach_count + 1. A branch at 0x704 goes to that call when a register is below 3 (unsigned). The facts show loads at 0x7a4 and 0x7a8 but no call there.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.22/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x85c
  **Inferred.** Role (from name): the reverse of the attach path. It loads the pointer at +0x58 (0x85c) and stores through a pointer at 0x86c, and makes stores at 0x8e4 through 0x900 and 0x940 through 0x95c (including sh +0x60 and sd +0x68 at 0x958 and 0x95c) whose bases the facts do not resolve. It calls err_int_unblock_mask at 0x970 with a0 = 0x8, a1 = 0x1 and the CPU index in a2, and passes the constant 0xdeadbabe in a5, whose role the facts do not show. A branch at 0x924 goes to 0x96c, just before that call, when a register is below 3 (unsigned); the return value is the word at 0x9200000001000098 or the callee's result.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.22/usr/cpu/sysgen/IP27boot/if_ef.o` at 0x73c
  **Inferred.** Loads the device structure pointer at a0+0x398 and calls nic_get_eaddr at 0x754 with the addresses at +48 and +52 of that structure as its first two arguments (computed, not loaded) and a1 and a3 at entry passed through to a2 and a3. It returns 0 or 5; the branch at 0x75c tests the result of nic_get_eaddr (taken when nonzero, to 0x774). A cmn_err call at 0x780 uses the field at a0+0x38c, but the facts do not say which path reaches it, so the reading is partial. Role (from name): gets the Ethernet address of an IOC3 Ethernet device.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP27boot/ioc3_pckm.o` at 0x5c
  **Inferred.** Attaches the IOC3 keyboard and mouse controller. It makes three kmem_zalloc allocations (one of 0x68 bytes and two of 0x58 bytes) and a mutex, probes the console and subdevice state with ioc3_is_console and ioc3_subdev_enabled, and connects pckm_intr through ioc3_intr_connect at 0x450 with a1 = 0x400000. It registers character devices with hwgraph_char_device_add at 0x1b8 and 0x310. It sends the command byte 0xf5 through pckm_pollcmd at 0x234 and checks the reply with data_check against 0xfa; only when data_check returns nonzero does it call default_kbd_setting and send 0xf4 (0x260, checked the same way). A later pckm_pollcmd with 0xf6 at 0x37c uses the record selected by the field at +44, again checked against 0xfa, and default_ms_setting at 0x398 follows only when that check is nonzero. Which device each command goes to is not resolved in the facts. The byte values match the common PS/2 command set, which is an outside reading that the binary itself does not confirm.
  Reviewed: 2026-11-12 (sonnet)

- `eiattach` in `6.5.22/usr/cpu/sysgen/IP27boot/ioc3ei.o` at 0x4a8
  **Inferred.** Attaches an IOC3 device whose name suggests an Ethernet interface. It skips the attach when ioc3_subdev_enabled, called with a1 = 7 at 0x4b8, returns zero (branch at 0x4c0). It allocates a 0x38-byte state block with kmem_zalloc at 0x510 and calls kvpalloc at 0x548 with a0 = 0x1 and a1 = 0x80000; the facts do not show how kvpalloc uses those arguments, so the 0x80000 is not shown to be a byte size. It publishes character devices at 0x590 and records inventory at 0x5cc. It connects ioc3_ei_intr through ioc3_intr_connect at 0x610 with a1 = 0x18000000, and the facts show only the subdevice number as the device class.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x13a0
  **Inferred.** Early entry of the IP27 boot kernel object. It loads a 64-bit word at 0x13d0 (the address is not resolved in the facts), calls mapped_kernel_setup_tlb at 0x13e8 with a2 and a3 passed through (a0 and a1 are not resolved), and then makes a tail jump at 0x1408 to .text+5136. The facts show no code reached after that jump.
  Reviewed: 2026-11-12 (sonnet)

- `get_nasid` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42b4
  **Inferred.** Loads one 64-bit word through a pointer (at 0x42b8) and returns. The facts do not resolve the load address or the return value's source, so the loaded word is not shown to be the result. cpu_node_probe calls it at 0x6014, mlreset at 0xce68 and 0xcf70, getcpuid at 0xdd58 and restart at 0x104e8. Role (from name): a node id.
  Reviewed: 2026-11-12 (sonnet)

- `get_slice` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42d0
  **Inferred.** Loads the 64-bit word at 0x9200000001000020 (at 0x42d4) and returns it unchanged. The same word is read by getcpuid at 0xdd64 and passed to nasid_slice_to_cpuinfo. The name suggests a slice number, but the body does no masking or shifting that would confirm it.
  Reviewed: 2026-11-12 (sonnet)

- `is_fine_dirmode` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42e0
  **Inferred.** Loads one 64-bit word through a pointer (at 0x42e4) and returns; the facts do not resolve the load address or what the return value is made of, and show no branch. mlreset calls it at 0xcec0, and the facts do not show how that caller uses the result. Role (from name): a directory-mode check.
  Reviewed: 2026-11-12 (sonnet)

- `ni_reset_port` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x43b0
  **Inferred.** Stores one 64-bit value at 0x43bc through the fixed address 0x9200000001600008 and returns that address. The stored value is not resolved, so what the write does is not shown. restart calls it at 0x104d0, and also has a store at 0x10534 whose address the facts do not show. Role (from name): a port reset.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_node_probe` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x5f48
  **Inferred.** Role (from name): a probe of one node's CPUs. It calls get_nasid at 0x6014, loads a word at offset 0xd0 of a computed address (0x6030), and calls do_cpumask at 0x6078 with a2 = a0 at entry and a3 = sp-64 (the facts do not show a mask argument). Loops compare a counter with 0x40, 0x100 and 0x80 (0x5f88 through 0x6008), and a failed check calls assfail at 0x6150 with a2 = 0xb9. The facts do not say what the three thresholds mean.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0xcd30
  **Inferred.** Role (from name): machine-level setup for the boot kernel. It calls r1nkperf_control_register_set and r1nkperf_data_register_set for registers 0 and 1 at 0xcd44 through 0xcd68, then get_cpu_irr at 0xcd70 and 0xcd88; the pairs for registers 2 and 3 are at 0xcdf4 through 0xce18, reached from the branches at 0xcd80 and 0xcd98. A branch at 0xcda0 on a0 goes to 0xce68 when a0 is zero; on the other side it compares r10k_bpmode with 0, 1, 2 and 3 (0xcda8 through 0xcdc4), which reach branch_prediction_on (0xd0cc), branch_prediction_fwd_not_taken (0xce38), branch_prediction_off (0xce48) and branch_prediction_taken (0xce58), with a printf at 0xce28. flush_tlb (a0 = 2, 0xcdd4) and flush_cache (0xcddc) lead to a return of flush_cache's result at 0xcde8; the facts show this as an exit path, not as a step before the node setup. The path from 0xce68 calls get_nasid, set_master_bridge_base, ioc3_mlreset (0xceb0), nvram_baseinit, is_fine_dirmode (0xcec0) and is_specified on arg_maxnodes (0xcecc); atoi and redo_nodes (0xcedc, 0xcee4) run only when is_specified returns nonzero. It then calls cpu_node_probe at 0xcf00 and, in address order, gen_region_mask, getcpuid, get_cpu_cnode, setup_replication_mask, get_nasid, spinlock_init, iograph_early_init (0xd020), mem_init, hubdev_init, gda_sym_init, verify_snchip_rev, he_arcs_set_vectors and intrgroup_init (0xd0bc). Three assfail calls (0xd0e4, 0xd100, 0xd11c, with a2 = 0x117, 0x11a and 0x12c) are reached from the branches at 0xcf08, 0xcf18 and 0xcf7c, and the purpose of each argument is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0xdd44
  **Inferred.** Calls splhi at 0xdd50, get_nasid at 0xdd58, and nasid_slice_to_cpuinfo at 0xdd6c with a0 = the get_nasid result and a1 = the word at 0x9200000001000020 (the word get_slice returns), then splx at 0xdd78. If the record pointer from nasid_slice_to_cpuinfo is zero the branch at 0xdd80 goes to 0xdd9c and the routine asserts (assfail at 0xdda8, a2 = 0x525); otherwise it returns the halfword at +0x1e of the record. The field at +0x1e is not named in the facts. Role (from name): returns the CPU id of the caller.
  Reviewed: 2026-11-12 (sonnet)

- `restart` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x103e0
  **Inferred.** Role (from name): the restart path. A branch at 0x103f8 goes to 0x104e0 when panic_level is zero, close to the get_nasid call at 0x104e8. Otherwise it loops while a counter is below maxnodes (0x10400 through 0x10458), then calls us_delay with 0xf4240 at 0x1046c. When reboot_on_panic equals 2 (0x10480) it goes to 0x104e8. Otherwise, when power_off_flag is nonzero (0x10490) it calls sysctlr_power_off at 0x10498 and us_delay with 0x2dc6c0 at 0x104a0, in a loop back to 0x10490 that tests the processor id against master_procid and a counter; when it is zero it calls ni_reset_port at 0x104d0. The delay unit is not shown. It then calls get_nasid, elsc_init (0x104f4) and elsc_system_reset (0x104fc), and calls printf at 0x10518 when elsc_system_reset returns a negative value; us_delay with 0xf4240 follows at 0x10520.
  Reviewed: 2026-11-12 (sonnet)

- `page_local_calias` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x11cd4
  **Inferred.** Loads the word at 0x9200000001000028 (at 0x11cd8) and takes one of two paths depending on whether that word is zero (branch at 0x11ce8). The other path compares two register values with an unsigned test (at 0x11cfc). The return value is 0 or 1. The name suggests a check on a local alias of a page, which the 0 or 1 result fits, but the facts do not show what the two compared values are.
  Reviewed: 2026-11-12 (sonnet)
