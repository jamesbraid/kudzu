# Kernel routine role statements, IP30 and IP35 kernels

Each entry states what one kernel routine appears to do, drawn only from its instruction facts: registers, loads and stores, call sites with their arguments, and branch conditions. The routines are in the 6.5.22 IP30 and IP35 kernel images, and the addresses are the ones in those images. Callee names come from the image's symbol table, so a name hints at intent without proving it. The facts do not establish what any of these routines is for, so every entry is labelled inferred. No entry in this batch was dropped.

## IP30 interrupt and error paths

- `ecc_handler` in `6.5.22/unix.IP30` at 0xa80000002001b9d4
  **Inferred.** Loads fields of the record in a1 at offsets +0x18, +0x238 and +0x23c, then +0x28 to +0x34, and stores into it at +0x8, +0x10, +0x238 and +0x28 to +0x34. A store at +0x3b8 goes through a base the facts do not resolve. It calls `ecc_cacheOp` (0xa80000002001b3b8) at eight sites; where the facts resolve the first argument it is 0x0, 0x1, 0x3, 0x4, 0x5, 0x7 or 0x13. It calls `ecc_force_cache_line_invalid` (0xa80000002001b8dc) at three sites. `cer_to_hex` (0xa80000002001b930) is called only when the bit tested at 0xa80000002001ba0c in the word at +0x18 is set, and a DIMM name from `maddr_to_dimm` (0xa80000002001202c) is printed through `sprintf` (0xa8000000201a7100). The codes passed to `ecc_cacheOp` are not named by the facts, so which cache lines it touches is open.
  Reviewed: 2026-11-12 (sonnet)
- `VEC_ibe` in `6.5.22/unix.IP30` at 0xa80000002001f970
  **Inferred.** Loads from the absolute address 0xffffffffaff50000 and stores a doubleword at a0+0x140 after reading a halfword at 0xffffffffffffc3a0. It then jumps to `VEC_trap` (0xa80000002000f3d0), passing a0, a1 and a3 through. The name suggests an instruction bus error vector, but the facts show only the handoff and not how the error is decoded.
  Reviewed: 2026-11-12 (sonnet)
- `heart_cpu_intr` in `6.5.22/unix.IP30` at 0xa800000020034998
  **Inferred.** Reads the halfword at 0xffffffffffffc3a0, stores a doubleword to 0xffffffffaff10028, and calls `doacvec` (0xa8000000201b2eac) with its four entry arguments. It then calls `doactions` (0xa8000000201b2bf0) and returns that call's value. The register at 0xffffffffaff10028 is the same one `clear_cpu_intr` and `heart_debug_intr` write, but the facts do not say what the register is.
  Reviewed: 2026-11-12 (sonnet)
- `heart_debug_intr` in `6.5.22/unix.IP30` at 0xa8000000200349d0
  **Inferred.** Reads the same halfword at 0xffffffffffffc3a0, stores a doubleword to 0xffffffffaff10028, and calls `debug` (0xa8000000200103dc) with a0 set to the constant 0xa800000020340160 and a1 to a3 passed through. It returns `debug`'s value. Reading it as a debugger interrupt comes from the callee name, since the facts do not show what `debug` does with its arguments.
  Reviewed: 2026-11-12 (sonnet)
- `chkdebug` in `6.5.22/unix.IP30` at 0xa800000020034a08
  **Inferred.** Reads the same halfword and loads a doubleword from 0xffffffffaff10030, then tests it against a2 (branch at 0xa800000020034a28). Only when that test is nonzero does it store to 0xffffffffaff10028 and call `debug` (0xa8000000200103dc); otherwise it returns without either. The bits involved are not named by the facts.
  Reviewed: 2026-11-12 (sonnet)
- `clear_cpu_intr` in `6.5.22/unix.IP30` at 0xa800000020034a4c
  **Inferred.** Stores one doubleword to 0xffffffffaff10028 and returns the constant 0xffffffffaff10000, the base of the same register block. The stored value does not appear in the facts, so the "clear" reading comes from the name and the store target alone.
  Reviewed: 2026-11-12 (sonnet)
- `heart_mlreset` in `6.5.22/unix.IP30` at 0xa800000020034a64
  **Inferred.** Calls `getcpuid` (0xa8000000200050b4) and then fills the register block at 0xffffffffaff10000: doublewords go to offsets +0x0, +0x8, +0x10, +0x18 and +0x40, and a doubleword is stored through table(0xffffffffaff10000). It also writes to the 0x9000000018000044, 0x9000000018000060 and 0x9000000018000068 addresses. The reading suggests a per-processor reset of software shadow state, but the stored values are not decoded, so the reset's scope is partly open.
  Reviewed: 2026-11-12 (sonnet)
- `heart_startup` in `6.5.22/unix.IP30` at 0xa800000020034b78
  **Inferred.** Skips its body when the halfword at 0xa8000000203dbc52 is zero (branch at 0xa800000020034b9c). Otherwise its call sites, in address order, are `hwgraph_path_add` (0xa8000000202201cc), `xtalk_provider_register` (0xa800000020030c78), `xtalk_provider_startup` (0xa8000000200308ec), `hwgraph_path_add`, `xswitch_id_set` (0xa800000020031304), `hwgraph_path_add`, `hwgraph_edge_add` (0xa80000002021f9d8), `device_master_set` (0xa8000000202217d0), `xwidget_register` (0xa800000020030d8c) and `xswitch_info_get` (0xa800000020031098). A zero result from `xswitch_info_get` skips the loop (branch at 0xa800000020034c8c). The loop, closed by the backward branch at 0xa800000020034d2c, calls `xswitch_info_link_ok` (0xa80000002003129c) and, when it returns nonzero, `sprintf`, `hwgraph_path_add`, `xswitch_info_vhdl_set` (0xa8000000200310ec) and a second `xwidget_register`. Role (from names): registers a crosstalk provider and its widgets in the hardware graph.
  Reviewed: 2026-11-12 (sonnet)
- `heart_intr` in `6.5.22/unix.IP30` at 0xa800000020035bcc
  **Inferred.** Loads an 8-byte mask from 0xffffffffaff10038 and stores to 0xffffffffaff10028 (the store at 0xa800000020035c40 depends on a branch whose operand the facts do not resolve). It calls `cvsema` (0xa800000020137f28) on 0xa800000020398218, then loops on `ffintr64` (0xa800000020005300). When the result is 0x3b or more (branch at 0xa800000020035c6c) it calls `buserror_intr` (0xa800000020154f08). Otherwise, depending on bit 0x1000 of a word at 0xa800000020398210, it calls either `valusema` (0xa8000000201384b0) or an indirect callee the facts do not resolve; the `ffintr64` result shifted left by 6 is passed in a5, with 0xa8000000203981f8 in a4, to `valusema`, `buserror_intr` and the indirect call alike, and `vsema` (0xa800000020138280) on 0xa800000020398218 follows `valusema` only when its result is below 0x7001. The semaphore meanings come from the callee names.
  Reviewed: 2026-11-12 (sonnet)
- `heart_rev` in `6.5.22/unix.IP30` at 0xa800000020035ef4
  **Inferred.** Loads the signed word cached at 0xa8000000203db3c4 and returns it when it is not negative (branch at 0xa800000020035ef8). When it is negative, the routine loads a word at 0x9000000018000004 and returns a value the facts leave unresolved; they do not show that value to be the loaded word unchanged. Nothing in the facts stores to the cache word. The facts do not say what the revision bits mean.
  Reviewed: 2026-11-12 (sonnet)

## Crosstalk bridges and their register accessors

- `giobr_attach` in `6.5.22/unix.IP30` at 0xa8000000200367f4
  **Inferred.** Reads the word at 0x900000001800002c and compares it with 0x80 at 0xa80000002003680c; the store at 0xa800000020036818 runs only when the word is below 0x80. It calls `xtalk_piotrans_addr` (0xa800000020030504) and tests bit 0x20 of a word at +0xc of its result (branch at 0xa800000020036840), with the early return of -1 or 0 at 0xa800000020036858 following that test. It allocates a 0x108-byte block with `kmem_alloc` (0xa800000020130644) and clears it with `blkclr` (0xa80000002000affc), runs `do_giobr_hw_init` (0xa80000002003666c), and probes slots with `giobr_slot_probe` (0xa800000020036474) in a loop whose counter is tested against 4 at 0xa800000020036a1c. Only when the word at +0x44 of the block is nonzero (branch at 0xa800000020036a28) does it run `do_giobr_link_reset` (0xa800000020037d04), `gioio_provider_register`, `gioio_provider_startup`, `gioio_device_register` and `gioio_device_attach`. Its only `cmn_err` call (0xa800000020036ac8) has level argument 0x7, and the facts do not decode the message. The one return value listed is -1 or 0.
  Reviewed: 2026-11-12 (sonnet)
- `xbridge_attach` in `6.5.22/unix.IP30` at 0xa800000020037e18
  **Inferred.** Gets the bridge pointer from `pcibr_bridge_ptr_get` (0xa80000002003c8ec) and passes it to `pcireg_id_get` (0xa800000020046398) and `pcireg_nic_addr`. Early branches at 0xa800000020037e7c and 0xa800000020037e94 compare a word with 0xc002 and a second with 0x36; when both match, control goes to `pcireg_stat_get` (0xa8000000200464f8) at 0xa800000020038688 instead of the main body. The main body allocates a 0xab8-byte state block with `kmem_zalloc` (0xa800000020130768), registers the PCI I/O provider with `pciio_provider_register` (0xa800000020055c08), and passes the state block to `pcireg_mode_get` (0xa80000002004716c) and `pcireg_speed_get` (0xa800000020046ff4). It then calls the `pcireg_intr_reset_set`, `pcireg_intr_device_set`, `pcireg_intr_enable_set` and `pcireg_intr_mode_set` setters and `xtalk_intr_connect` (0xa8000000200307b4), calls `rmallocmap` (0xa80000002015071c) and `pcibr_ate_free`, and maps windows with `pciio_device_win_map_new` (0xa80000002005610c) at three sites, each followed by `pciio_device_win_populate`. `pcibr_init_ext_ate_ram` (0xa80000002003ce2c) is reached through compare branches, not unconditionally. The register bits behind these calls are not decoded, so the window layout is only as named in the calls.
  Reviewed: 2026-11-12 (sonnet)
- `pcireg_id_get` in `6.5.22/unix.IP30` at 0xa800000020046398
  **Inferred.** Calls `panic` (0xa8000000201a749c) when a0 is null, and also when the halfword at +0x72 is -1 or a later compare chain finds no match. It loads that halfword, a word at +0x4 and a doubleword at +0x8 from a0. The halfword is compared against 3, 2, 0, 1 and -1. The word at +0x4 is compared against 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002, with other values compared against 0x24 between those steps and one against 0x36. Exits that return give 0 or an unresolved value. The name points to an ID register that the facts do not confirm.
  Reviewed: 2026-11-12 (sonnet)
- `pcireg_stat_get` in `6.5.22/unix.IP30` at 0xa8000000200464f8
  **Inferred.** Has the same shape as `pcireg_id_get`: a null check that panics, a halfword at +0x72, a word at +0x4, a doubleword at +0x8, and the same compare chain of codes. Its `panic` call takes a different first-argument pointer (0xa800000020343c78 against 0xa800000020343c50), and the message text is not decoded in the facts. The name suggests a status register, but the facts do not establish which register it reads.
  Reviewed: 2026-11-12 (sonnet)
- `pcireg_nic_addr` in `6.5.22/unix.IP30` at 0xa800000020049f30
  **Inferred.** Uses the same accessor shape: a null check that panics, a halfword at +0x72, a word at +0x4 and a doubleword at +0x8. Its failure path calls `panic` (0xa8000000201a749c) through a different message pointer. The name and its use in `xbridge_attach` point to the address of a NIC part on the bridge, but nothing in these facts confirms that.
  Reviewed: 2026-11-12 (sonnet)
- `pcireg_intr_addr_vect_set` in `6.5.22/unix.IP30` at 0xa80000002004cd7c
  **Inferred.** Reads a doubleword at a0+0x8, a halfword at +0x72 and a word at +0x4, and stores through pointers the facts do not resolve. It branches to an `assfail` call (0xa80000002000f694) with a2 = 0x85b when a1 is negative or 0x8 or more (branches at 0xa80000002004cdc0 and 0xa80000002004cdcc), so a1 is range-checked against 0 to 7. A `panic` call takes a1 shifted left by 3 as an argument, which suggests an index into 8-byte entries. Which register it writes is open.
  Reviewed: 2026-11-12 (sonnet)
- `pcireg_intr_addr_addr_set` in `6.5.22/unix.IP30` at 0xa80000002004d1e8
  **Inferred.** Has the same structure as `pcireg_intr_addr_vect_set`: the same field reads, the same kind of unresolved stores, and the same range check on a1 (below 0 or 0x8 and above, branches at 0xa80000002004d22c and 0xa80000002004d238) leading to an `assfail` call (0xa80000002000f694) that passes 0x89a. Role (from name): sets the address half of the interrupt slot that the sibling routine sets the vector for; the facts do not show the register.
  Reviewed: 2026-11-12 (sonnet)

## Keyboard, console and Ethernet

- `pckm_attach` in `6.5.22/unix.IP30` at 0xa80000002005772c
  **Inferred.** Role (from names): attaches a keyboard and mouse controller under the IOC3 chip. It checks `ioc3_subdev_enabled` (0xa80000002005756c) and branches away when the result is zero. It allocates blocks of 0x68, 0x58 and 0x58 bytes with `kmem_zalloc`, and calls `hwgraph_char_device_add` (0xa80000002022079c) at two sites, each followed by `hwgraph_chmod` with 0x1b6. On the first path it sends 0xf5 and then 0xf4 through `pckm_pollcmd` (0xa800000020058ee0); on the second it sends 0xf6. Each reply is checked against 0xfa with `data_check` (0xa800000020059340). It connects an interrupt with `ioc3_intr_connect` (0xa800000020056b14). The byte values are shown by the facts, but their meaning as commands is not.
  Reviewed: 2026-11-12 (sonnet)
- `cn_init` in `6.5.22/unix.IP30` at 0xa80000002005f8a8
  **Inferred.** Calls `gfx_earlyinit` (0xa80000002031f924) first. It then loads a halfword at 0xa8000000203fb990. When that is nonzero and a0 is nonzero, it calls `vn_rele` on the pointer at 0xa8000000203fb950 if that is non-null, then stores to it. When it is zero it calls `spinlock_init` and `init_mutex` on blocks at 0xa8000000203fb99c and 0xa8000000203fb970, and then `is_specified` (0xa8000000201b69a0) with `arg_console` (0xa800000020437bd8). Only after `is_specified` returns nonzero is the byte at `arg_console` compared with 0x67 and 0x47 (0xa80000002005f970 and 0xa80000002005f978). `tp_init` (0xa80000002031f6bc) and `du_init` (0xa8000000201301f8) are called on separate paths. The facts do not show which console path the two byte values select.
  Reviewed: 2026-11-12 (sonnet)
- `ef_ioc3eaddr` in `6.5.22/unix.IP30` at 0xa8000000200ade4c
  **Inferred.** Loads a pointer from a0+0x398 and passes two addresses inside that structure (at +48 and +52) to `nic_get_eaddr` (0xa80000002002fbf8), whose third and fourth arguments are its own a1 and a3. When `nic_get_eaddr` returns nonzero it loads a word from a0+0x38c and calls `cmn_err` (0xa8000000201a63b4) with level argument 0x1. The return value is 0 or 5, and the facts do not say whether 5 is an error number.
  Reviewed: 2026-11-12 (sonnet)
- `doacvec` in `6.5.22/unix.IP30` at 0xa8000000201b2eac
  **Inferred.** Loads bytes at 0xffffffffffffc080 and 0xffffffffffffc081, a word at c084 and a doubleword at c0a0, and stores to c04e, c081 and c0a0. Depending on those values it calls `panicspin` (0xa8000000201a846c), `tlbflush_rand` (0xa800000020192670) with 1, or `tlbflush` (0xa80000002019257c) with a0 taken from a global at -0x3f60. The one caller in these facts is `heart_cpu_intr`, which passes its entry arguments through, so its role on the interrupt path is open.
  Reviewed: 2026-11-12 (sonnet)

## Process timers and the syssgi call

- `dogetitimer` in `6.5.22/unix.IP30` at 0xa8000000201f59bc
  **Inferred.** Behaves differently by a0. When a0 is zero it calls `spl65`, takes a mutex at [a2+752]+2144 with `mutex_lock` (0xa80000002013b39c), copies four doublewords from +0x840 to +0x858 of the structure at [a2+752] to a1 at +0x0, +0x8, +0x10 and +0x18, and releases the mutex. It may then call `chktimeout_tick` (0xa8000000200093c4) with first argument 0 or 2, and when the result is nonzero converts it with `tick_to_timeval` (0xa8000000201f6b4c) before `splx`. When a0 is nonzero it calls `spl1`, copies four doublewords from +0x348 to +0x360 of an entry of the structure at a2 indexed by a0 shifted left by 5 into a1, and calls `splx`. The name suggests `getitimer`, but the facts show only the timeval conversion, not an interval timer layout.
  Reviewed: 2026-11-12 (sonnet)
- `syssgi` in `6.5.22/unix.IP30` at 0xa8000000201cf9c4
  **Inferred.** Role (from name): the system call entry for the `syssgi` family. It starts by calling `get_current_abi` (0xa80000002033aa04), copies request blocks in with 17 `copyin` calls (0xa80000002000ac80), and compares the doubleword at a0 against 0x4e3 at 0xa8000000201cfa14. It calls many helpers, including job control (`syssgi_makenewjob` 0xa800000020339388, `syssgi_joinjob` 0xa8000000203393a4, `syssgi_killjob` 0xa80000002033936c), `kaio_rw` (0xa8000000203394fc) and `xfs_itable` (0xa800000020282860). `nosys` (0xa800000020154e74) is called at four sites; the facts do not show it as a default case for unmatched requests. The facts do not show every compare that picks a helper, so the request-to-helper mapping rests partly on callee names.
  Reviewed: 2026-11-12 (sonnet)

## IP35 restart and node id

- `restart` in `6.5.22/unix.IP35` at 0xc000000000041010
  **Inferred.** Role (from name): the restart path for the IP35 platform. It calls `us_delay` (0xc000000000069c48) at three sites. When the word at 0xc00000000159c440 is nonzero it calls `get_elsc` (0xc000000000065554) and `sc_power_down_all` (0xc0000000000d24f8) and then delays; a backward branch at 0xc0000000000410f0 returns to 0xc0000000000410c0. When that word is zero it calls `ni_reset_port` (0xc000000000041304) at 0xc000000000041108. It then calls `get_nasid` (0xc000000000041204), `elsc_init` (0xc0000000000d0c40) and `elsc_system_reset` (0xc0000000000d212c). `printf` (0xc00000000026fdb4) is called only when `elsc_system_reset` returns a negative value, followed by a delay and stores to 0x9200000001680010 and 0x9200000001680008. The loop bounds and delay units are not decoded.
  Reviewed: 2026-11-12 (sonnet)
- `get_nasid` in `6.5.22/unix.IP35` at 0xc000000000041204
  **Inferred.** Loads a doubleword from the hub address 0x9200000001600000 and returns a value the facts leave unresolved. Role (from name): returns the node's NASID. The facts do not resolve how the returned value relates to the loaded word, so the field layout is open.
  Reviewed: 2026-11-12 (sonnet)
