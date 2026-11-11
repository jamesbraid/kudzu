# Routine readings, kudzu reading round 1, batch 9

Role statements for 25 routines at positions 225 to 249 of the ranked round-1 target list for kudzu (`6.5.22/unix.IP35` and the IP22 boot objects under `6.5.22/usr/cpu/sysgen/IP22boot/`). Every statement comes from the disassembly facts for that routine: registers, call sites, constants and branches, cited by address. Where a routine's name suggests a role the facts do not show, the entry says so and leaves the point open.

- `ioc4_eiattach` in `6.5.22/unix.IP35` at 0xc0000000000c46c0
  **Inferred.** The routine attaches one IOC4 sub-device, the one whose number 6 it passes to `ioc4_subdev_enabled` at 0xc0000000000c46d0. When the sub-device is enabled it finds the controller vertex with `hwgraph_traverse` and `hwgraph_fastinfo_get`, allocates a 0x38-byte record with `kmem_zalloc` at 0xc0000000000c4714, and registers a character device through `hwgraph_char_device_add` at 0xc0000000000c47a0. It connects an interrupt with `ioc4_intr_connect` at 0xc0000000000c4824. The facts do not show what device class the sub-device is, so the role beyond the IOC4 attach is open.

- `eiattach` in `6.5.22/unix.IP35` at 0xc0000000000c67fc
  **Inferred.** This is the IOC3 counterpart of `ioc4_eiattach`, with the same shape: it tests sub-device 7 through `ioc3_subdev_enabled` at 0xc0000000000c680c, reads the controller state through `ioc3_mem_ptr`, and allocates the same 0x38-byte record. It registers a character device and connects an interrupt with `ioc3_intr_connect`, and one call at 0xc0000000000c6940 goes to an address the disassembly could not name. The return at 0xc0000000000c6980 is -1 or 0. I read the two attach routines as IOC3 and IOC4 variants of one driver from their call pattern alone, which the facts do not confirm.

- `ioc4_pckm_attach` in `6.5.22/unix.IP35` at 0xc0000000000cb5dc
  **Inferred.** This attaches the keyboard and mouse side of an IOC4 controller, the "pckm" function. It checks the sub-device with `ioc4_subdev_enabled`, writes controller registers through `ioc4_write_ireg`, and asks `ioc4_is_console`. The callees `initkbdtype`, `pckm_flushinp` and `pckm_pollcmd` suggest keyboard setup and polling, and two character devices are registered, while `kopt_find` and `strstr` look up option strings whose names the facts do not show.

- `pckm_attach` in `6.5.22/unix.IP35` at 0xc0000000000cdbcc
  **Inferred.** This is the IOC3 version of `ioc4_pckm_attach`, calling `ioc3_subdev_enabled`, `ioc3_mem_ptr`, `ioc3_write_ireg` and `ioc3_is_console` in place of the IOC4 calls. It sets the keyboard type with `initkbdtype`, registers two character devices, and connects an interrupt with `ioc3_intr_connect`. The disassembled calls show no `pckm_pollcmd` or `pckm_flushinp`, unless the unnamed call sites are those routines, so its polling path may differ from the IOC4 one.

- `get_myid` in `6.5.22/unix.IP35` at 0xc0000000000d4eec
  **Established.** The routine does one thing: the doubleword at the fixed address 0x9200000001000020 is loaded at 0xc0000000000d4ef0 and returned unchanged. It makes no call and no store. The name suggests a node or machine identifier, which the facts do not confirm.

- `cn_init` in `6.5.22/unix.IP35` at 0xc0000000000dea68
  **Inferred.** This initialises the console record `cn`, which the facts reach through the global at 0xc0000000015c39c0. It calls `gfx_earlyinit` at 0xc0000000000dea80, releases the pointer at record+8 with `vn_rele`, and initialises a spinlock and a mutex inside the record. It then calls `tp_init` and `du_init` for the serial side, and passes `arg_console` (read at 0xc0000000000deb2c) to `is_specified` at 0xc0000000000deb18. The rule that picks the console is not in the facts.

- `ef_ioc3eaddr` in `6.5.22/unix.IP35` at 0xc00000000014d814
  **Inferred.** The routine works out the Ethernet address of an IOC3 interface. It reads fields of its argument at offsets 0x38c and 0x398, passes the region at 0x9000000014000000 to `get_elsc`, and gets the address from `ibrick_mac_addr_get`, then formats it with `sprintf` and converts it with `mactoh` and `nic_get_eaddr`. It gets the system serial number from `iobrick_get_sys_snum`, and `strstr` and `kopt_find` choose among these sources, with one return path yielding 0 or 5. The order of preference between the sources is not clear from the facts.

- `doacvec` in `6.5.22/unix.IP35` at 0xc000000000257768
  **Inferred.** The routine tests a flag byte at the fixed address 0xffffffffffffc080 and, on one path, calls `panicspin` with the doubleword at 0x92000000017a0038. It stores bytes to 0xffffffffffffc04e and 0xffffffffffffc081, and it calls `tlbflush_rand` at 0xc0000000002577b4 and `tlbflush` at 0xc0000000002577c8 before storing the doubleword at 0xffffffffffffc0d0 back. The facts do not say what the flags mean, so the name is the only hint at a TLB-flush role.

- `syssgi` in `6.5.22/unix.IP35` at 0xc0000000002a7524
  **Inferred.** This is the `syssgi` call, and the facts cover only a summary of it: 383 direct calls, 14 indirect calls, and over 1000 branch sites. Its calls are mostly `copyin` and `copyout` for argument and result transfer, `cap_able` for capability checks, and `syssgi_*` helpers such as `syssgi_getjobpid`, `syssgi_killjob` and `syssgi_nvram`. Many callees are unnamed in the disassembly output, so the mapping from request to helper is only partly recoverable here.

- `dogetitimer` in `6.5.22/unix.IP35` at 0xc0000000002cff28
  **Inferred.** The routine copies timer state into the caller's buffer at a1, using a per-process record reached from a2 with the field at +0x300 and a mutex at +2144. It raises the priority with `spl65` at 0xc0000000002cff4c, reads timeout state with `chktimeout_tick` at 0xc0000000002cffc4 and 0xc0000000002d0080, converts one value with `tick_to_timeval` at 0xc0000000002cffe4, and restores priority with `splx` and `spl1`. The name points to the getitimer request, but the field layout comes from the stores alone and is not checked against any header.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/unix.IP35` at 0xc000000000428658
  **Inferred.** This is an interrupt pre-handler. It loads a halfword from the fixed address 0xffffffffffffc428 and uses it to index the table at 0xc000000001594098, so the record it reaches supplies two handler arguments from fields at +0x20 and +0x28. It then makes an indirect call at 0xc0000000004286d8 whose target the disassembly could not resolve. The name's "hiwater" part suggests a high-water mark on a flow, but the facts do not confirm that.

- `gfx_flow_alloc` in `6.5.22/unix.IP35` at 0xc00000000042885c
  **Inferred.** The routine allocates a flow record with `kmem_alloc` at 0xc00000000042889c, asking for 0x70 bytes, and fills it with 2-byte and 4-byte stores across offsets 0x0 to 0x68. It takes the widget record from `xwidget_info_get` at 0xc000000000428930 and its id from `xwidget_info_id_get`, then finds the master through `xwidget_info_master_get` and `master_node_get`, and finishes with `gfx_flow_init`. One return path at 0xc000000000428928 yields 0 or the allocation result, which suggests a failed allocation returns 0.

- `gfx_flow_dealloc` in `6.5.22/unix.IP35` at 0xc000000000428ab8
  **Inferred.** This frees a flow record. It reads a halfword at +0x50 and a word at +0x54 of its argument, then calls `kmem_free` at 0xc000000000428aec with the argument and the size 0x70, the same size `gfx_flow_alloc` allocates. The reads are not followed by any branch in the facts, so their purpose is open.

- `gfx_flow_attach_me` in `6.5.22/unix.IP35` at 0xc000000000428b98
  **Inferred.** This links a flow record into a table. It stores a halfword at +0x60 and a doubleword at +0x68 of its argument, stores into the table at 0xc000000001594098 through the same halfword index that the pre-handler uses, and stores into the table at 0x9200000001000000. It reads the argument's fields at +0x14, +0x8, +0x4 and +0x58, and the stored values are not shown, so the link's contents are open.

- `gfx_flow_detach_me` in `6.5.22/unix.IP35` at 0xc000000000428d0c
  **Inferred.** This is the counterpart of `gfx_flow_attach_me`, and it stores into the same fields: +0x58, +0x60, +0x68, and a word at +0x14. It also writes the table at 0xc000000001594098 and the table at 0x9200000001000000. The stored values are not constants in the facts, so whether these fields are cleared or set to other values is open; the pairing rests on the matching offsets.

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP22boot/cn.o` at 0x804
  **Inferred.** This has the same name as the IP35 `cn_init` but a different body and layout. It calls `gfx_earlyinit` at 0x820, `vn_rele` on the pointer at `cn+4` at 0x854, `du_init` twice and `tp_init` for the serial side, and `spinlock_init` and `init_mutex` on `cn`. It stores into `cn` at +0x4, +0xc, +0x10, +0x24 and +0x26, and reads `arg_console` at 0x920 and 0x98c, with `is_specified` at 0x904 doing the lookup.

- `dummyret0_func` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x14c0
  **Established.** The only instruction in this range is a return that sets v0 to 0x0, so the routine returns 0 and does nothing else. The inventory row for this address names it `getcpuid`, but the binary's symbol is `dummyret0_func`, and this entry uses the binary's name. A placeholder that returns 0 fits the code, but that reading comes from the symbol name alone.

- `_get_timestamp` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x14d0
  **Established.** The routine is a single direct jump, at 0x14d0, to `get_r4k_counter`, with its four argument registers passed on unchanged. Its result is whatever that counter routine returns. The name suggests a timestamp from the R4000-family cycle counter, but the facts do not say what the counter measures.

- `start` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1760
  **Inferred.** This looks like the early boot entry. It stores three words to `_argc`, `_argv` and `_envirn` at 0x1778 to 0x1788, the stored values not shown, and calls `_check_dbg` at 0x17a8 with those values. It calls `debug` at 0x17e4 only when the `dbgmon_loadstop` word is nonzero (the `beq` at 0x17d8 skips that call when it is zero), then `mlsetup` at 0x1880 and `main` at 0x1890. Four branches on a3 around 0x1828 to 0x184c skip to 0x1860, and the order of those checks is not in the facts.

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x1918
  **Inferred.** The routine loops from 0x1928 to 0x1930 and, in the body, reads and rewrites one byte at the abs address 0xffffffffa0080061 (the `lb` at 0x1954 and `sb` at 0x195c). It returns early when the `bellok` word at 0x1944 is zero. The address sits where a device register would, but the facts do not say which device, so the name's audio-muting reading is only consistent with the code.

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x3f1c
  **Inferred.** This is machine reset setup for the IP22 family. It writes uncached addresses such as 0xffffffffbfa000ec, 0xbfa0002c, 0xbfa00084 and 0xbfbd9870 and 0xbfbdd600, and it updates the word at 0xbfa00004 several times. It also stores into `.data` near +0x760 to +0x774, calls `flushbus` six times, `setlclvector` three times, `setup_ip22_flags` and `set_endianness`, and cache routines, and the facts do not say what the individual registers do.

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x555c
  **Inferred.** The routine tests a byte at 0xa0000000 plus 0x1fbd900b or 0x1fbd988b (the `lbu` at 0x55a4) for bit 0x2 and a word at 0xbfbd9850 (the `lw` at 0x55bc). A nonzero `is_fullhouse_flag` sends it straight to the `cpu_powerdown` call at 0x559c, and `is_ioc1_flag` appears to pick between the two byte addresses (the facts show the branch, not what selects it). The name suggests waiting for a reset to finish, but that depends on what the bits mean, which the facts leave open.

- `dobuserr_common` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x5c8c
  **Inferred.** This is the common bus-error handler for the IP22 family. It reads `cpu_err_stat`, `gio_err_stat`, `hpc3_ext_io` and `hpc3_bus_err_stat` and branches on their bits, for example 0x3f00 in `cpu_err_stat` at 0x5d10 and 0xff00 in `gio_err_stat` at 0x5d34. It calls `ecc_perr_is_forced`, `map_physaddr`, `unmap_physaddr`, `fix_bad_parity`, `enable_sysad_parity`, `disable_sysad_parity` and `flushbus`, and reports through `cmn_err` (17 call sites), `log_perr` and `perr_save_info`. Two indirect calls at 0x6260 and 0x62a8 go through `special_bus_error_handler`, and four blocks are not reached from its entry according to the facts.

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x78e8
  **Inferred.** The routine reads a byte from an address the disassembly could not resolve (the `lbu` at 0x7910) and stores a word to the uncached address 0xffffffffbfbd9870 (the `sw` at 0x792c). It returns that same address. `mlreset` writes the same word, so this is probably a register that drives the front-panel LEDs, but the facts do not show that.

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP22boot/kernel.o` at 0x7938
  **Inferred.** The body is one call to `panic` at 0x7944, with a constant message from `.rodata` at offset 2240 and the routine's own argument registers, and it returns the result of `panic`. The facts show no interrupt write, so the name does not match the code shown. Either this is a stub that panics, or the disassembled range is incomplete; the facts do not settle which.
