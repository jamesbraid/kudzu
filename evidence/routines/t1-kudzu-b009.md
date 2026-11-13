# Routine readings, kudzu reading round 1, batch 9

Role statements for 25 routines at positions 225 to 249 of the ranked round-1 target list for kudzu (`6.5.22/unix.IP35` and the IP22 boot objects under `6.5.22/usr/cpu/sysgen/IP22boot/`). Every statement comes from the disassembly facts for that routine: registers, call sites, constants and branches, cited by address. Where a routine's name suggests a role the facts do not show, the entry says so and leaves the point open.

- `ioc4_eiattach` in `6.5.22/unix.IP35` at 0xc0000000000c46c0
  **Inferred.** The routine attaches one IOC4 sub-device, the one whose number 6 it passes to `ioc4_subdev_enabled` at 0xc0000000000c46d0. When that call returns nonzero it looks up a vertex with `hwgraph_traverse` and `hwgraph_fastinfo_get`, allocates a 0x38-byte record with `kmem_zalloc` at 0xc0000000000c4714, and registers a character device through `hwgraph_char_device_add` at 0xc0000000000c47a0. It calls `ioc4_is_console` at 0xc0000000000c47e4, and only when that returns nonzero does it call `ei_make_hwgraph_nodes` and connect an interrupt with `ioc4_intr_connect` at 0xc0000000000c4824. The facts do not show what device class the sub-device is, so the role beyond the IOC4 attach is open.
  Reviewed: 2026-11-12 (sonnet)

- `eiattach` in `6.5.22/unix.IP35` at 0xc0000000000c67fc
  **Inferred.** This looks like the IOC3 counterpart of `ioc4_eiattach`, with a similar shape: it tests sub-device 7 through `ioc3_subdev_enabled` at 0xc0000000000c680c, calls `ioc3_mem_ptr`, and allocates the same 0x38-byte record. It registers a character device. Only when `ioc3_is_console` (call at 0xc0000000000c6928) returns nonzero does it reach the interrupt connection with `ioc3_intr_connect`, and one call at 0xc0000000000c6940 goes to an address the disassembly could not name. The return at 0xc0000000000c6980 is -1 or 0. I read the two attach routines as IOC3 and IOC4 variants of one driver from their call pattern alone, which the facts do not confirm.
  Reviewed: 2026-11-12 (sonnet)

- `ioc4_pckm_attach` in `6.5.22/unix.IP35` at 0xc0000000000cb5dc
  **Inferred.** Role (from name): this attaches the keyboard and mouse side of an IOC4 controller, the "pckm" function. The facts show that it checks sub-device 1 with `ioc4_subdev_enabled`, writes controller registers through `ioc4_write_ireg` (at 0xc0000000000cb62c and again at 0xc0000000000cba44), and calls `ioc4_is_console`. The callees `initkbdtype`, `pckm_flushinp` and `pckm_pollcmd` suggest keyboard setup and polling. There are two `hwgraph_char_device_add` call sites (0xc0000000000cb7a4 and 0xc0000000000cb908), each reached only on some paths. `kopt_find` and `strstr` look up option strings whose names the facts do not show.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.22/unix.IP35` at 0xc0000000000cdbcc
  **Inferred.** Role (from name): this looks like the IOC3 version of `ioc4_pckm_attach`, calling `ioc3_subdev_enabled`, `ioc3_mem_ptr`, `ioc3_write_ireg` and `ioc3_is_console` in place of the IOC4 calls. It calls `initkbdtype`. It has two `hwgraph_char_device_add` call sites (0xc0000000000cddb0 and 0xc0000000000cdf14), each reached only on some paths, and a single `ioc3_intr_connect` call at 0xc0000000000cdfe0 that is also conditional. The disassembled calls show no `pckm_pollcmd` or `pckm_flushinp`, unless the unnamed call sites are those routines (the unnamed call at 0xc0000000000cdd50 passes the same constant 0xf5 as the IOC4 `pckm_pollcmd` call), so its polling path may differ from the IOC4 one.
  Reviewed: 2026-11-12 (sonnet)

- `get_myid` in `6.5.22/unix.IP35` at 0xc0000000000d4eec
  **Established.** The routine does one thing: it returns the doubleword at the fixed address 0x9200000001000020 unchanged (the return at 0xc0000000000d4ef4). The facts show no call and no store. The name suggests a node or machine identifier, which the facts do not confirm.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.22/unix.IP35` at 0xc0000000000dea68
  **Inferred.** Role (from name): console initialisation. The facts show it calls `gfx_earlyinit` at 0xc0000000000dea80. Only when the halfword at 0xc0000000015c3a00 is nonzero, its first argument is nonzero and the doubleword at 0xc0000000015c39c0 is nonzero does it call `vn_rele` on that doubleword (at 0xc0000000000deaa8). It initialises a spinlock at 0xc0000000015c3a0c and a mutex at 0xc0000000015c39e0, and passes the address 0xc00000000160d180 to `is_specified` at 0xc0000000000deb18. It then compares the byte at that address with 0x67 and 0x47, and it calls `tp_init` and `du_init` on some paths. The rule that picks the console is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.22/unix.IP35` at 0xc00000000014d814
  **Inferred.** Role (from name): this works out the Ethernet address of an IOC3 interface. It reads fields of its argument at offsets 0x38c and 0x398, formats the field at +0x38c with `sprintf` into a stack buffer, and searches that buffer twice with `strstr`. On some paths it calls `get_elsc` with the address 0x9000000014000000 as its second argument and then `iobrick_get_sys_snum`, and on others it calls `ibrick_mac_addr_get`, `mactoh` and `nic_get_eaddr`; `kopt_find` is also called. Several `cmn_err` calls are also present. One return path yields 0 or 5. Which source the address comes from, and the order of preference between them, is not clear from the facts.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.22/unix.IP35` at 0xc000000000257768
  **Inferred.** The routine tests a flag byte at the fixed address 0xffffffffffffc080. It calls `panicspin` with the address 0x92000000017a0038 when that byte is nonzero or when bit 0 of the doubleword at that address is set. When the byte at 0xffffffffffffc081 is nonzero it performs two byte stores (their addresses are not shown). It calls `tlbflush_rand` at 0xc0000000002577b4 when the word at 0xffffffffffffc084 is nonzero, and `tlbflush` at 0xc0000000002577c8 when the doubleword at 0xffffffffffffc0d0 is nonzero, followed by a doubleword store whose address is not shown. The facts do not say what the flags mean, so the name is the only hint at a TLB-flush role.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.22/unix.IP35` at 0xc0000000002a7524
  **Inferred.** Role (from name): the `syssgi` system call. The facts list 383 direct call sites and 619 branches. Many of the early branches compare a value loaded through the first argument against request-number constants (for example 0xa6, 0xa7, 0x160, 0x41f and 0x4c1), which looks like dispatch on a request number. The calls include `copyin` (18 sites), `copyout` (28), `cap_able` (17), `get_current_cred` (8) and 29 call sites of `syssgi_*` helpers such as `syssgi_getjobpid`, `syssgi_killjob` and `syssgi_nvram`. 22 call sites are unnamed in the disassembly, so the mapping from request to helper is only partly recoverable here.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.22/unix.IP35` at 0xc0000000002cff28
  **Inferred.** The routine copies timer values out through its second argument's buffer, as the facts show stores of values loaded from a per-process record reached through a2 at +0x300 (with a mutex at +2144, locked at 0xc0000000002cff60). When its first argument is zero it raises priority with `spl65` at 0xc0000000002cff4c, copies fields at +0x840 to +0x858 of the record, reads timeout state with `chktimeout_tick` at 0xc0000000002cffc4 and 0xc0000000002d0080, converts one value with `tick_to_timeval` at 0xc0000000002cffe4, and restores priority with `splx` at 0xc0000000002cfff0. That path then falls into the code at 0xc0000000002d0000, which a nonzero first argument branches to directly: it raises priority with `spl1`, copies fields at +0x358 to +0x370 and restores priority with `splx` at 0xc0000000002d0034. The name points to the getitimer request, but the field layout comes from the stores alone and is not checked against any header.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/unix.IP35` at 0xc000000000428658
  **Inferred.** Role (from name): an interrupt pre-handler. It loads a halfword from the fixed address 0xffffffffffffc428 and uses it to index the table whose base pointer is held at 0xc000000001594098. If the table entry is nonzero, the record it points to supplies two handler arguments from fields at +0x20 and +0x28, and the routine then makes an indirect call at 0xc0000000004286d8 whose target the disassembly could not resolve. The name's "hiwater" part suggests a high-water mark on a flow, but the facts do not confirm that.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.22/unix.IP35` at 0xc00000000042885c
  **Inferred.** Role (from name): allocates a flow record. When the word at 0xc000000001594088 is nonzero it allocates 0x70 bytes with `kmem_alloc` at 0xc00000000042889c and fills the record with stores of several widths (2-byte, 4-byte and 8-byte) across offsets 0x0 to 0x68; it also calls `kvalloc` at 0xc0000000004289f4. It takes the widget record from `xwidget_info_get` at 0xc000000000428930 and its id from `xwidget_info_id_get`, finds the master through `xwidget_info_master_get` and `master_node_get`, and on one path calls `gfx_flow_init` at 0xc000000000428a84. One return path at 0xc000000000428928 yields 0 or the allocation result, which suggests a failed allocation returns 0.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.22/unix.IP35` at 0xc000000000428ab8
  **Inferred.** This frees a flow record. It reads a halfword at +0x50 and a word at +0x54 of its argument, then calls `kmem_free` at 0xc000000000428aec with the argument and the size 0x70, the same size `gfx_flow_alloc` allocates. The reads are not followed by any branch in the facts, so their purpose is open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.22/unix.IP35` at 0xc000000000428b98
  **Inferred.** Role (from name): links a flow record into a table. The facts show a halfword store at +0x60 and a doubleword store at +0x68, plus several doubleword stores at offset 0 whose destination registers are not shown. They also show loads at +0x14, +0x8, +0x4 and +0x58, and halfword loads at offset 0. The facts do not show the base registers or the stored values, so which tables the stores reach, and what the link holds, is open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.22/unix.IP35` at 0xc000000000428d0c
  **Inferred.** Role (from name): the counterpart of `gfx_flow_attach_me`. The facts show stores of a halfword at +0x60, a word at +0x14, a doubleword at +0x68 and several doubleword stores at offset 0, and loads at +0x58 and halfword offset 0. The return value is an expression over the table whose base pointer is held at 0xc000000001594098, indexed by the halfword at 0xffffffffffffc428. The stored values and the base registers of the stores are not shown, so whether fields are cleared or set to other values is open; the pairing with the attach routine rests on the matching offsets.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP22boot/cn.o` at 0x804
  **Inferred.** This has the same name as the IP35 `cn_init` but a different body and layout. It calls `gfx_earlyinit` at 0x820, `vn_rele` on the pointer at `cn+4` at 0x854, `du_init` twice and `tp_init` for the serial side, and `spinlock_init` and `init_mutex` on `cn`. It stores into `cn` at +0x4, +0xc, +0x10, +0x24 and +0x26, and reads `arg_console` at 0x920 and 0x98c, with `is_specified` at 0x904 doing the lookup.
  Reviewed: 2026-11-12 (sonnet)

- `dummyret0_func` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x14c0
  **Established.** The only instruction in this range is a return that sets v0 to 0x0, so the routine returns 0 and does nothing else. The inventory row for this address names it `getcpuid`, but the binary's symbol is `dummyret0_func`, and this entry uses the binary's name. A placeholder that returns 0 fits the code, but that reading comes from the symbol name alone.
  Reviewed: 2026-11-12 (sonnet)

- `_get_timestamp` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x14d0
  **Established.** The routine is a single direct jump, at 0x14d0, to `get_r4k_counter`, with its four argument registers passed on unchanged. Its result is whatever that counter routine returns. The name suggests a timestamp from the R4000-family cycle counter, but the facts do not say what the counter measures.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1760
  **Inferred.** This looks like the early boot entry. It stores three words to `_argc`, `_argv` and `_envirn` at 0x1778 to 0x1788, the stored values not shown, and calls `_check_dbg` at 0x17a8 with those values. It calls `debug` at 0x17e4 only when the `dbgmon_loadstop` word is nonzero (the `beq` at 0x17d8 skips that call when it is zero), then `mlsetup` at 0x1880 and `main` at 0x1890. Four branches on a3 around 0x1828 to 0x184c skip to 0x1860, and the order of those checks is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1918
  **Inferred.** The routine loops from 0x1928 to 0x1930 (a store at 0x192c in the body and a backward `bgez`) and returns early at 0x1960 when the `bellok` word at 0x1944 is zero. Otherwise it loads a byte (`lb` at 0x1954) and stores a byte (`sb` at 0x195c); the facts do not show the address, so they do not say which device that is. The name's audio-muting reading is only consistent with the code.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x3f1c
  **Inferred.** Role (from name): machine reset setup. The facts show many word stores at offset 0 whose destination registers are not shown. They also show the addresses 0xffffffffbfa00000, 0xffffffffbfbd9878 and 0xffffffffbfbd9870 as arguments to `flushbus`, stores into `.data` at +0x760 to +0x774, six `flushbus` calls, three `setlclvector` calls, `setup_ip22_flags`, `set_endianness`, `__dcache_wb_inval` and `__icache_inval`. It also branches on several flags such as `is_fullhouse_flag`. The facts do not say what the individual registers do.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x555c
  **Inferred.** The routine polls a byte at 0xa0000000 plus 0x1fbd900b or 0x1fbd988b (the `lbu` at 0x55a4), looping while bit 0x2 is clear. Once that bit is set, a nonzero `is_fullhouse_flag` sends it to the `cpu_powerdown` call at 0x559c. Otherwise it tests a word at 0xbfbd9850 (the `lw` at 0x55bc) and loops again while its bit 0x2 is set. `is_ioc1_flag` appears to pick between the two byte addresses (the facts show the branch at 0x5580, not what selects it). The name suggests waiting for a reset to finish, but that depends on what the bits mean, which the facts leave open.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x5c8c
  **Inferred.** Role (from name): the common bus-error handler for the IP22 family. It reads `cpu_err_stat`, `gio_err_stat` and `hpc3_ext_io` and branches on their bits, for example 0x3f00 in `cpu_err_stat` at 0x5d10 and 0xff00 in `gio_err_stat` at 0x5d34; `hpc3_bus_err_stat` is only passed to a `cmn_err` call. It calls `ecc_perr_is_forced`, `map_physaddr`, `unmap_physaddr`, `fix_bad_parity`, `enable_sysad_parity`, `disable_sysad_parity` and `flushbus`, and reports through `cmn_err` (17 call sites), `log_perr` and `perr_save_info`. Three calls to unknown targets, at 0x6260, 0x62a8 and 0x6304, take `special_bus_error_ef_epc` as an argument, and the first two also pass `special_bus_error_handler`.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x78e8
  **Inferred.** The routine loads a byte from an address the disassembly could not resolve (the `lbu` at 0x7910), stores a word at 0x792c (base register not shown), and returns the constant 0xffffffffbfbd9870. `mlreset` passes the same address to `flushbus`, so this is possibly a register that drives the front-panel LEDs, but the facts do not show that.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x7938
  **Inferred.** The body is one call to `panic` at 0x7944, with a constant message from `.rodata` at offset 2240 and the routine's own argument registers, and it returns the result of `panic`. The facts show no interrupt write, so the name does not match the code shown. Either this is a stub that panics, or the disassembled range is incomplete; the facts do not settle which.
  Reviewed: 2026-11-12 (sonnet)
