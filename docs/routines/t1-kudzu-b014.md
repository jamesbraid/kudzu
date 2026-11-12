# Routine role statements, kudzu batch 14

These statements come from instruction facts for 25 routines picked from the ranked tier-1 target list. Each one names the routine, its binary and its address. The statements use only the registers, calls, constants and addresses in those facts. Every grade is inferred: the facts do not fully establish any role, and the names are only a hint. Binaries are written with their full install path.

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3dd1c
  **Inferred.** A wrapper that passes the address of a stack slot (sp-48) as a0 and a1 to a3 through to `probe_common` at 0x3dd38, then returns that call's v0 at 0x3dd44. `badaddr_val` at 0x3dd4c and `wbadaddr` at 0x3dd7c have the same shape and call the same `probe_common` at 0x3dd68 and 0x3dda0. The facts do not show what `probe_common` does, so the bad-address probe purpose comes from the name alone.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3dd4c
  **Inferred.** The same wrapper as `badaddr`, 48 bytes long, calling `probe_common` at 0x3dd68 with the same argument registers (a0 = sp-48, a1 to a3 as at entry) and returning its v0 at 0x3dd74. The name suggests a variant that takes a value, but the facts list no difference in the registers passed, so what the extra argument does is unconfirmed.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3dd7c
  **Inferred.** Calls `probe_common` at 0x3dda0 and returns its v0 at 0x3ddac, like the two routines before it. It is 56 bytes instead of 48. The facts list no stores for any of the three wrappers, so they do not explain the size difference.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42230
  **Inferred.** Loads two doublewords from addresses the facts leave unresolved (0x42230, 0x42234) and branches on the sign of the first at 0x42238. It stores a doubleword at 0x42244, reads a halfword from the fixed address 0xffffffffffffc420 at 0x42248, and jumps to `VEC_trap` at 0x4224c with a0, a1 and a3 passed through and a2 unresolved. A doubleword store at a0+0x140 follows the jump at 0x42250 (the instruction after the jump). The name suggests an instruction-side bus-error vector entry, which the facts do not confirm.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** Calls `wchanmeter` at 0x38b0. When its result is nonzero (branch at 0x38b8) the facts show a call to `mrmeter_entry` at 0x3994 with a0 = that result, a1 = 0x5 and a3 = 0. `mr_access` is called at 0x38c0, and a branch at 0x38c8 skips the rest when the `wchanmeter` result is zero. Otherwise it reads and writes fields of the record `wchanmeter` returns, at offsets 0x1c0 through 0x1d0, with an unsigned comparison at 0x3908 deciding whether the stores at 0x3910 to 0x391c run. It then calls `mrmeter_getentry` at 0x392c with a2 = 0x5 and a3 = 0x1. When that returns nonzero, a load-linked/store-conditional loop at 0x3940 to 0x3950 adds 1 to the word at record offset 0x198 and retries until the store succeeds. The routine returns at 0x3964 either the `mr_access` or the `mrmeter_getentry` result. The meaning of each counter field is unknown from these facts.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x39ec
  **Inferred.** Its name pairs with `_mraccess`. It calls `wchanmeter` at 0x3a0c and `mr_update` at 0x3a1c. When the `wchanmeter` result is nonzero (branch at 0x3a14) the facts show a call to `mrmeter_entry` at 0x3ac4 with a1 = 0x6, where `_mraccess` passes 0x5 to its own `mrmeter_entry` call. A branch at 0x3a2c skips the rest when the `wchanmeter` result is zero. Otherwise it reads and writes record fields at offsets 0x1a0 through 0x1d8, with an unsigned comparison at 0x3a6c deciding whether the stores at 0x3a74 to 0x3a80 run and a bit test (mask 0x4) at 0x3a90 gating a further store. Unlike `_mraccess`, the facts show no `mrmeter_getentry` call and no load-linked/store-conditional loop. The shared `wchanmeter` record hints at an access and update pair, but the facts show only the shared call.
  Reviewed: 2026-11-12 (sonnet)

- `MgrasGeneralInterrupt` in `6.5.22/usr/cpu/sysgen/IP27boot/mgras.a(mgras.o)` at 0x8330
  **Inferred.** Role (from name): a general interrupt handler for a graphics device. It loads a per-device record pointer from a0+0xd0 and reads two status words at record offsets 0x11020 and 0x11040. It calls `vsema` at 0x836c, 0x83a0 and 0x84d4, each on the fall-through of a sign test (branches at 0x8360, 0x8394 and 0x84c8). A bit test against 0x400 at 0x83b4 leads to `MgrasSchedSwapBufFinish` at 0x83cc. The five `cmn_err_tag` calls (0x8404 to 0x84a4) are reached only when a test against 0x2000a200 at 0x83e4 finds a bit set. Of those, the second is gated by a test against 0x200 and the later ones by tests on an unresolved register, each call passing a constant (0x200, 0x2000, 0x4000, 0x8000) as the last argument the facts show. `mgras_error` at 0x84b0 follows them. The facts do not give the meaning of the status bits.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_phase_bits` in `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` at 0x68
  **Inferred.** Loads one doubleword at 0x70 through a pointer the facts leave unresolved and returns at 0x94 with v0 unresolved. The facts list no calls or branches. The name suggests NIC phase bits, but the facts show neither the source address nor how the returned value is built from the load, so what it holds is not established.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP27boot/os.a(pda.o)` at 0x1afc
  **Inferred.** Reads bytes at the fixed kernel addresses 0xffffffffffffc080 and 0xffffffffffffc081, a word at 0xffffffffffffc084 and a doubleword at 0xffffffffffffc0a0. Three of its four tests decide whether a call runs: the byte at c080 gates `panicspin` at 0x1b0c, the word at c084 gates `tlbflush_rand` at 0x1b34, and the doubleword at c0a0 gates `tlbflush` at 0x1b48. The byte at c081 instead gates two byte stores at 0x1b20 and 0x1b24, and a doubleword store at 0x1b4c follows the `tlbflush` call. The facts do not say what the tested values count, so the purpose is unclear.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP27boot/os.a(syssgi.o)` at 0x140
  **Inferred.** A large dispatcher of 12104 bytes that reads a request code from the caller's a0 block. It jumps through a table at 0x19c whose targets are unresolved, so the individual requests cannot be listed from these facts. It calls `get_current_abi` at 0x160 and, when `sat_enabled` is set, goes on to `sat_never` and `sat_set_subsysnum`. The name fits a system-call entry, but the facts do not confirm it.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP27boot/os.a(time.o)` at 0xc0
  **Inferred.** Branches on a0 at 0xdc and fills the buffer at a1 with four doublewords on either path. When a0 is zero it calls `splprof` at 0xe4, locks a mutex at [a2+752]+2144 with `mutex_lock` at 0xf8, copies the doublewords at offsets 0x840 through 0x858 of the table at [a2+752] into a1+0 through a1+0x18, and unlocks at 0x120. Depending on a word at a1+0x14 and a doubleword at a1+0x18, it can then call `chktimeout_tick` at 0x15c or 0x1f4 and `tick_to_timeval` at 0x178 with a1+16. The facts show a2 = 0x320 for `tick_to_timeval`, and the instructions at 0x168 to 0x170 also set 0x989680. It ends with `splx` at 0x184. When a0 is nonzero it calls `splhi` at 0x194 instead, copies four doublewords from offsets 0x348 through 0x360 of an entry at a2 + (a0 << 5) into the same buffer, and calls `splx` at 0x1c8. It returns 0 at 0x1e0. The name suggests `getitimer`, but the table layout is not established.
  Reviewed: 2026-11-12 (sonnet)

- `part_discovery` in `6.5.22/usr/cpu/sysgen/IP27boot/partition.o` at 0x1c10
  **Inferred.** Role (from name): partition discovery at boot. It calls `cpsema` on a .bss object at 0x1c2c and again at 0x1c3c while the result stays nonzero, then calls `vector_read` at 0x1c58 with a2 = 0x600000 and a3 pointing at a stack buffer. A nonzero `vector_read` result goes to the return at 0x1cb8. When a low-nibble test (mask 0xf at 0x1c74) of a loaded value is nonzero, `part_probe_nasid` is called at 0x1c7c in a loop while a counter stays below 0x100; when it is zero, `part_probe_nasid` is called at 0x1d78 and 0x1d94 instead. A second counter is tested against 0x40 at 0x1cd8, and in that part of the routine `mutex_spinlock` (0x1d00) and `mutex_spinunlock` (0x1cc0, 0x1d38) are called, `part_interrupt_set` at 0x1d4c gets `part_activate` as a2, and `part_start_partition_thread` at 0x1d5c gets a1 = 0x2 and a2 = 0x1. The value read by `vector_read` is not resolved, so the number of partitions probed is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_vect_set` in `6.5.22/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x147a8
  **Inferred.** Role (from name): sets a PCI bridge interrupt address vector. Range checks on a1 at 0x147ec and 0x147f8 send values below 0 or at least 8 to an error exit at 0x149b8, which probably leads into `assfail` at 0x149c0 and `panic` at 0x149d4. A chain of comparisons on an identifier in a4 (0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002) branches to separate code for each value, and each of those blocks has loads and stores whose bases the facts do not show. The facts show the comparisons but not the register map, so which structure or hardware register each store reaches is unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_addr_set` in `6.5.22/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x14c14
  **Inferred.** Role (from name): sibling of `pcireg_intr_addr_vect_set`. It has the same index checks, the same identifier comparison chain (0xe000 through 0xc002), and the same error exits at `assfail` 0x14e3c and `panic` 0x14e50. The facts show loads and stores in the per-identifier blocks without their bases, so it is not established that it stores into the same structure or register group as the vector setter, and which registers the stores reach stays unconfirmed.
  Reviewed: 2026-11-12 (sonnet)

- `scsiha_ioctl` in `6.5.22/usr/cpu/sysgen/IP27boot/scsiha.o` at 0x188
  **Inferred.** Role (from name): the ioctl handler for a SCSI host adapter. A masked test (mask 0x1ff) at 0x1ac either returns 0x6 at 0x1c0 or reaches `scsi_ctlr_info_get` at 0x1c8. At 0x1d8 a1 is compared with 0x7306. When equal, `hwgraph_connectpt_get`, `device_controller_num_get` and `hwgraph_path_to_dev` run (0x25c to 0x278). Later, and depending on their results, `sprintf` at 0x298, `hwgraph_traverse` at 0x2a8 and `hwgraph_edge_add` at 0x2d0 run, the last only when `hwgraph_traverse` returns 0x2. Otherwise a 16-byte `kmem_zalloc` is made at 0x1e0, and `copyin` of 0x10 bytes from a2 at 0x1fc is reached only when a1 is at least 0xb. Two calls go through unresolved function pointers (0x24c and 0x308), so the full request set is not determined.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.22/usr/cpu/sysgen/IP27boot/xbridge.o` at 0x54
  **Inferred.** Role (from name): the attach routine for the Xbridge bridge chip. At lower addresses it calls `pcibr_bridge_ptr_get` (0x90), `hwgraph_path_add` (0xec), `pciio_provider_register` with `pci_xbridge_provider` (0xf8) and `pciio_provider_startup` (0x100). Later it allocates and zeroes a 0xab8-byte context with `kmem_zalloc` and `memset` at 0x1c8 and 0x1dc. Loops whose bounds come from two bytes at +0x38 and +0x39 of that record call `pcibr_slot_info_init` (0x740) and four more per-slot routines. It sets interrupt and ATE registers with `pcireg_*` calls, creates three device windows with `pciio_device_win_map_new` and `pciio_device_win_populate` (0x68c to 0x6e8), and calls `xwidget_error_register` with `pcibr_error_handler` at 0x454 and `xtalk_intr_connect` with `pcibr_error_intr_handler` at 0x584. An identifier is compared against 0xc0021 through 0xc0024 and 0xd0021 and 0xd0022 at 0x2fc to 0x330. The facts show those comparisons but not what each outcome sets, and not the window addresses beyond the call arguments.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP30boot/cn.o` at 0xaac
  **Inferred.** Role (from name): early console setup. It calls `gfx_earlyinit` at 0xac4 first, and releases the vnode stored at sym(cn)+8 with `vn_rele` at 0xaec only when that pointer is nonzero (branch at 0xae4). It initializes a spinlock and a mutex on the console state at sym(cn) (0xb2c, 0xb44). It checks whether a console is specified with `is_specified` at 0xb5c, then compares the first byte of the console argument against 0x67 and 0x47 to pick a path, calling `tp_init` (0xb8c) and `du_init` (0xba8) along the way. The facts do not show which device each byte selects.
  Reviewed: 2026-11-12 (sonnet)

- `bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** Role (from name): a debug variant of a bit-lock acquire. It uses `ll`/`sc` pairs on words at a2 offsets 0xa8 through 0xbc, and calls `try_bitlock` at 0x16cc and 0x17f8. A `cmn_err` at 0x18bc, passing the entry a0 as a2 and a doubleword read from the fixed address 0xffffffffaff20000 as a3, is reached when the unsigned comparison at 0x1750 finds a1 below a2. The retry and backoff policy depends on counter values the facts leave unresolved, so the loop logic is only partly known.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** Role (from name): the 64-bit counterpart of `bitlockdebug`. It has the same field layout (a2 offsets 0xa8 through 0xbc) and calls `try_64bitlock` at 0x1f70 and 0x209c. A `cmn_err` at 0x2160 is reached when the unsigned comparison at 0x1ff4 finds a1 below a2. The accesses the facts list on those fields are 4-byte `ll`/`sc`, not `lld`/`scd`, so they do not themselves show a 64-bit width.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitunlockdebug` in `6.5.22/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** Role (from name): a debug unlock for the 64-bit lock. It loads a halfword from the fixed address 0xffffffffffffc3a0 at 0x21e8 and 0x227c, and compares the first load with the halfword at a2+0x2a at 0x2208. It then updates fields at a2 offsets 0x40 through 0x80 and a doubleword at a0+0. A branch at 0x2244 compares two unresolved registers unsigned, and a branch at 0x2264 compares the word at a2+96 with an unresolved register unsigned. It calls `cmn_err` at 0x22c0 and 0x22d8 on paths whose conditions are only partly shown. The conditions that reach them are only partly understood from these facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** Role (from name): the prehandler for a delayed high-water interrupt in the graphics flow-control code. It indexes the `gfx_flow_owners` table by a0 scaled by eight to find an owner record (branch at 0x14). When that entry is zero it makes two `cmn_err` calls (0x24, 0x34), then takes and releases `gfx_flow_heart_lock` (0x3c, 0x5c) around a read and write of the doubleword at the fixed address 0xffffffffaff00040 (0x4c, 0x60). When the entry is nonzero, the code at 0x6c calls a function pointer taken from that record at 0x74. The facts do not show the meaning of the pointer or the tested record fields.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_pipe_head_init` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0xd0
  **Inferred.** Role (from name): initializes a flow pipe head at a0. It reads and writes fields of that record (a 32-bit store at +0x58, loads at +0x20, +0x1c, +0x10 and +0x8). It stores into the `gfx_flow_owners` entry chosen by a halfword at 0xffffffffffffc3a0, and writes doublewords at the fixed address 0xffffffffaff00048 and at addresses based on 0xffffffffaff00000 plus an index. A `mutex_spinlock` at 0x138 on `gfx_flow_heart_lock` precedes the later stores (0x13c and 0x164), and the routine returns the `mutex_spinunlock` result at 0x170. The meaning of the record fields is not established.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x178
  **Inferred.** Role (from name): allocates and fills a flow record. It takes `gfx_flow_init_lock` at 0x1ac and releases it at 0x1d8. The branch at 0x1b8 goes to `gfx_flow_init` at 0x308 when the `gfx_flow_owners` table pointer is zero, and the branch at 0x1cc goes to a block containing `heart_flow_control_connect` at 0x348 (with the prehandler as a1) when a counter word plus 1 equals 1. It calls `kmem_alloc` with size 0x68 at 0x1e4 and stores into the new record at offsets 0x0 through 0x60. The widget identity comes from `xwidget_info_get` and `xwidget_info_id_get` (0x1fc, 0x208). `kvalloc` at 0x29c and `gfx_flow_pipe_head_init` at 0x2d8 come later, after a branch at 0x260 on an unresolved value, and the facts do not show which entry paths reach them. The routine returns 0 or the `kmem_alloc` result at 0x300.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x384
  **Inferred.** Role (from name): tears down a flow record at a0. A halfword at a0+0x5c is compared with -1 at 0x398, and any other value leads to a `cmn_err` at 0x3a8. It takes and releases `gfx_flow_heart_lock` (0x3b4, 0x3e4), then takes `gfx_flow_init_lock` (0x3ec) and releases it at 0x41c, and frees the record with `kmem_free` of size 0x68 at 0x444. When a counter word minus 1 is zero (branch at 0x408), `heart_flow_control_disable` at 0x464 and `heart_flow_control_disconnect` at 0x480 run, and a branch at 0x490 loops back to 0x464 while the counter s0 is below 2.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x53c
  **Inferred.** Role (from name): attaches a flow record at a0 to the `gfx_flow_owners` table. It stores a0 into the table at the slot picked from a halfword at 0xffffffffffffc3a0 (0x560), stores a1 at a0+0x60 (0x574), and writes a halfword at a0+0x5c and a word at a0+0x58 (0x57c, 0x588). It combines doublewords loaded from a0+0x8 and a0+0x10 and stores the result at an address based on 0xffffffffaff00050 plus an index (0x598). Between `mutex_spinlock` at 0x5a8 and `mutex_spinunlock` at 0x5f4 on `gfx_flow_heart_lock` it stores at an address based on 0xffffffffaff00060 (0x5ac) and updates the doubleword at 0xffffffffaff00040 (0x5d0, 0x5f8). It then calls `heart_flow_control_enable` at 0x600 with the word at a0+0x58 and returns that result. The meaning of the stored fields is not established.
  Reviewed: 2026-11-12 (sonnet)
