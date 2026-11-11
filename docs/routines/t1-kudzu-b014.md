# Routine role statements, kudzu batch 14

These statements come from instruction facts for 25 routines picked from the ranked tier-1 target list. Each one names the routine, its binary and its address. The statements use only the registers, calls, constants and addresses in those facts. Every grade is inferred: the facts do not fully establish any role, and the names are only a hint. Binaries are written with their full install path.

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3dd1c
  **Inferred.** A wrapper that passes the address of a stack slot (sp-48) as a0 and a1 to a3 through to `probe_common` at 0x3dd38, then returns that call's v0 at 0x3dd44. `badaddr_val` at 0x3dd4c and `wbadaddr` at 0x3dd7c have the same shape and call the same `probe_common` at 0x3dd68 and 0x3dda0. The facts do not show what `probe_common` does, so the bad-address probe purpose comes from the name alone.

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3dd4c
  **Inferred.** The same wrapper as `badaddr`, 48 bytes long, calling `probe_common` at 0x3dd68 with the same argument pattern and returning its v0 at 0x3dd74. The name suggests a variant that takes a value, but the facts show no extra argument handling, so that stays unconfirmed.

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x3dd7c
  **Inferred.** Calls `probe_common` at 0x3dda0 and returns its v0 at 0x3ddac, like the two routines before it. It is 56 bytes instead of 48, but the instruction facts show no extra call or memory access that accounts for the difference.

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` at 0x42230
  **Inferred.** Loads doublewords from addresses the facts leave unresolved and reads a halfword from the fixed address 0xffffffffffffc420. It stores a doubleword at a0+0x140, then branches on the sign of an unresolved register at 0x42238 and jumps to `VEC_trap` at 0x4224c with a0, a1 and a3 passed through and a2 unresolved. The name suggests an instruction-side bus-error vector entry, which the facts do not confirm.

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** Calls `wchanmeter` at 0x38b0, then `mr_access` at 0x38c0 on one path and `mrmeter_getentry` at 0x392c on another. It also calls `mrmeter_entry` at 0x3994 with type code 0x5. It reads and writes fields of the record `wchanmeter` returns, at offsets 0x198 through 0x1d0, and the loop between 0x38c0 and 0x3900 is only partly mapped. The meaning of each counter field is unknown from these facts.

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP27boot/ksync_metered.a(sema.o)` at 0x39ec
  **Inferred.** The sibling of `_mraccess`. It calls `wchanmeter` at 0x3a0c, then `mr_update` at 0x3a1c, and `mrmeter_entry` at 0x3ac4 with type code 0x6 where `_mraccess` uses 0x5. It updates a different set of record fields, at offsets 0x1a0 through 0x1d8, with the same branch shape. The shared `wchanmeter` record hints at an access and update pair, but the facts show only the shared call.

- `MgrasGeneralInterrupt` in `6.5.22/usr/cpu/sysgen/IP27boot/mgras.a(mgras.o)` at 0x8330
  **Inferred.** The name suggests a general interrupt handler for a graphics device. It reads a per-device record through a0+0xd0 and two status words at record offsets 0x11020 and 0x11040, and calls `vsema` at 0x836c, 0x83a0 and 0x84d4. Five `cmn_err_tag` reports (0x8404 to 0x84a4) sit on paths gated by bit tests against 0x400, 0x200 and 0x2000a200 on an unresolved register, with `mgras_error` at 0x84b0. The facts do not give the meaning of those status bits.

- `nic_get_phase_bits` in `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` at 0x68
  **Inferred.** Reads one doubleword from the fixed address 0x9000000010000078 at 0x70 and returns at 0x94 with v0 unresolved. It has no calls or branches in the facts, so it looks like a plain status read. The name suggests NIC phase bits, but the facts show no bit selection, so what the returned value holds is not established.

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP27boot/os.a(pda.o)` at 0x1afc
  **Inferred.** Reads and writes bytes and a word at fixed kernel addresses 0xffffffffffffc080, c081, c04e and c084, and a doubleword at c0a0. Four tests on bytes, a word and a doubleword decide whether `panicspin` at 0x1b0c, `tlbflush_rand` at 0x1b34 or `tlbflush` at 0x1b48 runs. The facts do not say what the tested values count, so the purpose is unclear.

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP27boot/os.a(syssgi.o)` at 0x140
  **Inferred.** A large dispatcher of 12104 bytes that reads a request code from the caller's a0 block. It jumps through a table at 0x19c whose targets are unresolved, so the individual requests cannot be listed from these facts. It calls `get_current_abi` at 0x160 and, when `sat_enabled` is set, goes on to `sat_never` and `sat_set_subsysnum`. The name fits a system-call entry, but the facts do not confirm it.

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP27boot/os.a(time.o)` at 0xc0
  **Inferred.** Fills the caller's buffer at a1 with timer fields copied from a table reached through a2+752, using offsets 0x840 through 0x858. It locks and unlocks a mutex in that table at offset 2144, and calls `chktimeout_tick` at 0x15c and 0x1f4. `tick_to_timeval` at 0x178 gets one of two constants, 0x320 or 0x989680, and `splprof`, `splhi` and `splx` bracket the work. The name suggests `getitimer`, but the table layout is not established.

- `part_discovery` in `6.5.22/usr/cpu/sysgen/IP27boot/partition.o` at 0x1c10
  **Inferred.** The name suggests partition discovery at boot. It takes a semaphore on a .bss object with `cpsema` at 0x1c2c and 0x1c3c, reads a vector from address 0x600000 with `vector_read` at 0x1c58, and loops `part_probe_nasid` at 0x1c7c while a counter stays below 0x100. It then registers `part_activate` as a partition interrupt handler with `part_interrupt_set` at 0x1d4c, and starts a partition thread at 0x1d5c. The loop limits and the vector contents are not resolved, so the number of partitions probed is unknown.

- `pcireg_intr_addr_vect_set` in `6.5.22/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x147a8
  **Inferred.** Writes a PCI bridge interrupt register set, indexed by a1. Range checks at 0x147ec and 0x147f8 send bad index values to an error exit at 0x149b8, which probably leads into `assfail` at 0x149c0 and `panic` at 0x149d4. A chain of comparisons on an identifier in a4 (0xe000 through 0xc002) picks one of several layouts for the stores into the structure at a0. The facts show the comparisons but not the register map, so which hardware register each store reaches is unresolved.

- `pcireg_intr_addr_addr_set` in `6.5.22/usr/cpu/sysgen/IP27boot/pcibr.o` at 0x14c14
  **Inferred.** The sibling of `pcireg_intr_addr_vect_set`. It has the same index checks, the same identifier comparison chain (0xe000 through 0xc002), and the same error exits at `assfail` 0x14e3c and `panic` 0x14e50. It stores into the same structure at a0, so it is probably the address-side setter for the same register group. The facts do not show which registers the stores reach, so this stays unconfirmed.

- `scsiha_ioctl` in `6.5.22/usr/cpu/sysgen/IP27boot/scsiha.o` at 0x188
  **Inferred.** The ioctl handler for a SCSI host adapter, per the name. It calls `scsi_ctlr_info_get` on entry and checks the request code against 0x7306 at 0x1d8, and that path looks up the controller through `hwgraph_connectpt_get`, `device_controller_num_get` and `hwgraph_path_to_dev`. A 16-byte `kmem_zalloc` followed by `copyin` at 0x1fc handles the other requests, and a name built by `sprintf` is traversed and linked into the hardware graph. Two calls go through unresolved function pointers (0x24c and 0x308), so the full request set is not determined.

- `xbridge_attach` in `6.5.22/usr/cpu/sysgen/IP27boot/xbridge.o` at 0x54
  **Inferred.** The attach routine for the Xbridge bridge chip, per the name. It allocates and zeroes a 0xab8-byte context with `kmem_zalloc` and `memset` at 0x1c8 and 0x1dc, then runs a loop whose bound comes from two bytes in the new record, registers the PCI provider, and maps device windows. It also sets interrupt and ATE registers and connects the error handler and interrupt, and compares an identifier against 0xc0021 through 0xc0024 and 0xd0021 and 0xd0022 to pick layouts. The facts do not confirm those layouts or the window addresses beyond the call arguments.

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP30boot/cn.o` at 0xaac
  **Inferred.** Early console setup. It calls `gfx_earlyinit` at 0xac4 first, and releases the vnode stored at sym(cn)+8 with `vn_rele` at 0xaec only when that pointer is nonzero (branch at 0xae4). It initializes a spinlock and a mutex on the console state at sym(cn) (0xb2c, 0xb44). It checks whether a console is specified with `is_specified` at 0xb5c, then compares the first byte of the console argument against 0x67 and 0x47 to pick a path, calling `tp_init` (0xb8c) and `du_init` (0xba8) along the way. The name suggests console init, but the facts do not show which device each byte selects.

- `bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** A debug variant of a bit-lock acquire, per the name. It uses `ll`/`sc` pairs on lock words at argument offsets 0xa8 through 0xbc, and calls `try_bitlock` at 0x16cc and 0x17f8. On the failure path it calls `cmn_err` at 0x18bc with the lock address and a value read from the fixed address 0xffffffffaff20000. The retry and backoff policy depends on counter values the facts leave unresolved, so the loop logic is only partly known.

- `_64bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** The 64-bit counterpart of `bitlockdebug` by name. It has the same field layout (offsets 0xa8 through 0xbc) and calls `try_64bitlock` at 0x1f70 and 0x209c, with a `cmn_err` failure report at 0x2160. The recorded accesses are 4-byte `ll`/`sc`, not 64-bit `lld`/`scd`, so the name and the operations disagree, and the facts cannot settle which one the code means.

- `_64bitunlockdebug` in `6.5.22/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** A debug unlock for the 64-bit lock. It loads a halfword from the fixed address 0xffffffffffffc3a0 at 0x21e8 and 0x227c, and compares it with the halfword at a2+0x2a at 0x2208. It then updates fields at a2 offsets 0x40 through 0x80 and a doubleword at a0+0. A branch at 0x2244 compares two unresolved registers unsigned, and a branch at 0x2264 compares a1 with a2+96 unsigned. It calls `cmn_err` at 0x22c0 and 0x22d8 on reporting paths. The failure conditions are only partly understood from these facts.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** The prehandler for a delayed high-water interrupt in the graphics flow-control code, per the name. It indexes the `gfx_flow_owners` table by a0 scaled by eight to find an owner record, and takes and releases `gfx_flow_heart_lock` (0x3c, 0x5c). It updates a doubleword at the fixed address 0xffffffffaff00040 (0x4c, 0x60). If the table entry for a0 is nonzero (branch at 0x14), the code at 0x6c calls a function taken from that record at 0x74, and a `cmn_err` at 0x24 reports on a path the facts show only in part.

- `gfx_flow_pipe_head_init` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0xd0
  **Inferred.** Initializes a flow pipe head at a0. It reads and writes fields of that record (a 32-bit store at +0x58, loads at +0x20, +0x1c, +0x10 and +0x8). It stores into the `gfx_flow_owners` entry chosen by a halfword at 0xffffffffffffc3a0, and writes doublewords to fixed addresses 0xffffffffaff00048 and 0xffffffffaff00040. A `mutex_spinlock` at 0x138 on `gfx_flow_heart_lock` wraps the update, and the routine returns the `mutex_spinunlock` result at 0x170. The meaning of the record fields is not established.

- `gfx_flow_alloc` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x178
  **Inferred.** Allocates and fills a flow record. It calls `kmem_alloc` with size 0x68 at 0x1e4 and stores into the new record at offsets 0x0 through 0x60. The branch at 0x1b8 jumps straight to the call at 0x308 when the `gfx_flow_owners` table pointer is zero, skipping `kvalloc` at 0x29c and `gfx_flow_pipe_head_init` at 0x2d8; otherwise those calls run first. The facts do not say which case is the usual one. `gfx_flow_init` at 0x308 and `heart_flow_control_connect` at 0x348 (with the prehandler) follow on both paths. The widget identity comes from `xwidget_info_get` and `xwidget_info_id_get` (0x1fc, 0x208), and the routine returns 0 or the `kmem_alloc` result.

- `gfx_flow_dealloc` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x384
  **Inferred.** Tears down a flow record at a0. A halfword at a0+0x5c is compared with -1 at 0x398, and any other value leads to a `cmn_err` at 0x3a8. It takes `gfx_flow_heart_lock` and then `gfx_flow_init_lock` (0x3b4, 0x3ec), and frees the record with `kmem_free` at 0x444. A branch on the counter s0 (below 2) decides whether `heart_flow_control_disable` at 0x464 runs, and `heart_flow_control_disconnect` runs at 0x480.

- `gfx_flow_attach_me` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x53c
  **Inferred.** Attaches a flow record at a0 to the `gfx_flow_owners` table, at the slot picked from a halfword at 0xffffffffffffc3a0. It copies fields from a0+0x8 and a0+0x10 into a0+0x60, and writes a0+0x5c and a0+0x58. Under `gfx_flow_heart_lock` (0x5a8, 0x5f4) it writes doublewords to the owner table and to the fixed address 0xffffffffaff00040, then calls `heart_flow_control_enable` at 0x600 with the value at a0+88 and returns that result. The meaning of the stored fields is not established.
