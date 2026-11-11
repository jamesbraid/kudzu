# Tier-1 routine readings, kudzu batch 39

Twenty-five routines, ranked positions 975 to 999 (0-based) of the located, hardware-linked target list for kudzu.

Each role comes from the disassembly: instruction addresses, calls, constants and returns. Names come from the binary's symbols or the inventory. Where a role rests on the name alone, the paragraph says so.

- `scsiha_ioctl` in `6.5.30/usr/cpu/sysgen/IP32boot/scsiha.o` at 0x198
  **Inferred.** `scsiha_ioctl` handles two ioctl command words, 0x7306 and 0x7321, which the routine compares at 0x1f0 and 0x210. Each path copies a 12-byte request in with `copyin` (0x24c) into a `kmem_zalloc` buffer, then resolves a controller through `hwgraph_connectpt_get` and `hwgraph_path_to_dev` and adds graph edges and a path (0x2f8, 0x39c). The disassembly does not say what either command does, so the role is limited to the call pattern.

- `cn_init` in `6.5.30/usr/cpu/sysgen/IP35boot/cn.o` at 0xaac
  **Inferred.** `cn_init` fills the console block `cn` with halfword and doubleword stores and sets two bytes of `arg_console` (0xb40 to 0xbbc, 0xb80, 0xb8c). It calls `gfx_earlyinit` (0xb0c), initializes a spinlock and a mutex on the block (0xb90, 0xba8), and tests the console argument with `is_specified` (0xbc0) and byte compares against 0x67 and 0x47. It ends by calling `tp_init` (0xbf0) or `du_init` (0xc0c); the disassembly shows both targets but not the condition that picks one.

- `bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** `bitlockdebug` wraps a bit-lock acquire with a debug path. It calls `try_bitlock` at 0x16cc and again at 0x17f8 on another path, restores the saved level with `splx` (0x17e0), and reports through `cmn_err` (0x18bc). Load-linked and store-conditional pairs touch fields at offsets 0xa8 to 0xbc of its argument. The retry condition and the indirect call at 0x17e8, which has no resolved target, stay open.

- `_64bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** `_64bitlockdebug` has the same structure as `bitlockdebug`: `try_64bitlock` at 0x1f70 and 0x209c, `splx` at 0x2084, an unresolved indirect call at 0x208c, and `cmn_err` at 0x2160. Its lock accesses are 4-byte load-linked and store-conditional pairs at the same offsets, so the lock width does not confirm the 64-bit name. The doubleword loads at 0x1f50 and 0x1fd0 read the fixed address 0x9200000001030100, which the disassembly does not identify.

- `_64bitunlockdebug` in `6.5.30/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** `_64bitunlockdebug` is the unlock side. It reads a halfword at offset 0x2a of its third argument (a2) and compares it with a halfword at the fixed address 0xffffffffffffc428, then copies doubleword fields of the record (offsets 0x40 to 0x68 and 0x80) and rewrites offset 0 of its first argument. It reports through `cmn_err` at 0x22c0 and 0x22d8 with levels 0x4 and 0x0, and the disassembly does not say which report is the failure path.

- `fake_a_part_number` in `6.5.30/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x170c
  **Inferred.** `fake_a_part_number` writes a 13-byte constant into its first argument with unaligned store pairs (`sdl`/`sdr`, `swl`/`swr`) and a byte store at offsets 0 to 0xc. The constant comes from read-only data, and the path depends on `get_nasid` and `get_chimera_type` (0x17d0 to 0x182c). The routine returns a `get_chimera_type` value on some paths and the constant 0x2d303031 on another. The name suggests a placeholder part number, but the disassembly does not show the full string.

- `fake_a_product_name` in `6.5.30/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x1984
  **Inferred.** `fake_a_product_name` follows the same pattern as `fake_a_part_number`, but it writes into two buffers. The second argument gets unaligned and byte stores at offsets 0 to 0xc, and the first argument gets byte stores at offset 0 along each path. The branches test register a4 against 0x100, 0x200 and 0x400, and the calls go to `get_nasid` (0x1ac8) and `get_chimera_type` (0x1ad0). The return is 0x200, 0x4000 or a `get_chimera_type` value, and the disassembly does not say which one the usual path takes.

- `gbrick_eeprom_read` in `6.5.30/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x3228
  **Inferred.** `gbrick_eeprom_read` reads board identification data for a node. It maps the node through `nasid_to_compact_node` and compares the NASID from `get_nasid` (0x324c) with its second argument (0x3274). On a match it calls `get_elsc` (0x3328); otherwise it opens a system-controller channel with `sc_open` (0x32a8), reads with `read_board_ia` (0x32ec) and closes with `sc_close` (0x32fc). A bit-0x800 test on its third argument (0x326c) and a `get_chimera_type` call (0x3244) choose a further path that the disassembly leaves open.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** `gfx_flow_delayed_hiwater_intr_prehandler` is a 148-byte routine. It reads a CPU number as a halfword from the fixed address 0xffffffffffffc428, uses it to index `gfx_flow_cpu_attachments`, and branches on whether the attachment slot is non-zero (bne at 0x1c). The non-zero path makes an indirect call at 0x80 with arguments loaded from the attachment record at +0x20 to +0x28; the target is unresolved. The name says high-water interrupt, but the disassembly does not reach the handler, so its role is inferred.

- `gfx_flow_alloc` in `6.5.30/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x204
  **Inferred.** `gfx_flow_alloc` allocates a 0x70-byte record with `kmem_alloc` (0x244) and fills it with word and doubleword stores at offsets 0 to 0x68. It queries the xwidget for its info, ID and master node (`xwidget_info_get` at 0x2d8, `xwidget_info_id_get` at 0x2e8, `xwidget_info_master_get` at 0x3e4, `master_node_get` at 0x3ec), allocates a buffer with `kvalloc` (0x39c), and calls `gfx_flow_init` (0x42c). The 0x70 size matches the `kmem_free` in `gfx_flow_dealloc`, so the two are a pair. The disassembly does not show the success return or what the 0x4000000400000007 constant selects.

- `gfx_flow_dealloc` in `6.5.30/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x460
  **Inferred.** `gfx_flow_dealloc` is the release side of `gfx_flow_alloc`. It frees its argument with `kmem_free` at 0x494 using the same 0x70 size. Before the free it reads a halfword at +0x50 and a word at +0x54 of its argument, and the disassembly does not show what those values gate. The doubleword store at 0x498 goes to an address the disassembly does not resolve.

- `gfx_flow_attach_me` in `6.5.30/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x540
  **Inferred.** `gfx_flow_attach_me` records a flow client against the current CPU. It writes the client pointer into the `gfx_flow_cpu_attachments` slot selected by a CPU number (0x568), reads per-CPU fields from `pdaindr` and stores them into the client (0x574, 0x578, 0x550, 0x570), and writes three doublewords to the fixed table at 0x9200000001000000 (0x5b4, 0x5d0, 0x5ec). It also writes through the pointer at +0x58 (0x648) and reads a node map from `compact_to_nasid_node` (0x610). The return at 0x65c has no resolved value, so whether it reports success is open.

- `gfx_flow_detach_me` in `6.5.30/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x6b4
  **Inferred.** `gfx_flow_detach_me` undoes `gfx_flow_attach_me`. It clears the CPU's entry in `gfx_flow_cpu_attachments` (0x77c) and resets fields of the record (0x6c4, 0x76c, 0x784, 0x78c), then writes the fixed table at 0x9200000001000000 (0x73c to 0x750). The disassembly shows no branches in this routine, and it returns the address of the CPU's slot (0x788). The stored values are not in the disassembly, so "clears" is inferred from the pairing with attach.

- `prom_has_vga_support` in `6.5.30/usr/cpu/sysgen/IP35boot/gfxs.a(klconfig.o)` at 0x330
  **Inferred.** `prom_has_vga_support` is a predicate with no calls. It reads two words from the fixed addresses 0x90000000180000a0 and 0x90000000180000a4 and branches on the first being below 6 (unsigned, at 0x344) and on a register the disassembly does not name being non-zero (0x34c). It returns 1 at 0x354 and 0 at 0x360. The disassembly does not show which combination of the two words produces 1, so the VGA reading comes from the name.

- `ef_ioc3eaddr` in `6.5.30/usr/cpu/sysgen/IP35boot/if_ef.o` at 0x738
  **Inferred.** `ef_ioc3eaddr` looks up the Ethernet address for an IOC3 interface. It builds a string with `sprintf` (0x75c) and searches it with `strstr` (0x76c, 0x780). It calls `ibrick_mac_addr_get` (0x7b8), `mactoh` (0x7dc) and `nic_get_eaddr` (0x7f8) on different paths, reads a system serial through `get_elsc` and `iobrick_get_sys_snum` (0x820, 0x82c), checks kernel options with `kopt_find` (0x8dc, 0x920), and logs through `cmn_err` at level 1 on several paths. The byte compare against 0x3a at 0x854 is an ASCII colon, which suggests address text parsing, but the disassembly does not name the source string.

- `pckm_attach` in `6.5.30/usr/cpu/sysgen/IP35boot/ioc3_pckm.o` at 0x5c
  **Inferred.** `pckm_attach` attaches the keyboard and mouse controller that hangs off an IOC3 chip. It checks the subdevice with `ioc3_subdev_enabled` (0x6c), allocates per-device records with `kmem_zalloc` (0xf0, 0x11c, 0x12c), creates keyboard and mouse hardware-graph nodes with mode 0x1b6 (octal 0666), and connects the interrupt to `pckm_intr` through `ioc3_intr_connect` (0x470). It sends the command bytes 0xf5, 0xf4 and 0xf6 through `pckm_pollcmd` and checks each reply with `data_check` against 0xfa, which fits the usual PS/2 command and acknowledge values. That identification comes from the constants, not from the names in the disassembly.

- `eiattach` in `6.5.30/usr/cpu/sysgen/IP35boot/ioc3ei.o` at 0x4a8
  **Inferred.** `eiattach` attaches the Ethernet function of an IOC3 chip. It checks subdevice 7 with `ioc3_subdev_enabled` (0x4b8), allocates a 0x38-byte record (0x510) and initializes a spinlock in it (0x538), then allocates a 512 KiB buffer with `kvpalloc` (0x548). It registers a character device and its hardware-graph nodes (0x590, and `ei_make_hwgraph_nodes` at 0x5ec), connects `ioc3_ei_intr` through `ioc3_intr_connect` (0x610), and records inventory and controller numbers (0x5cc, 0x5e4, 0x63c). The disassembly does not show what the 512 KiB buffer holds or what the 0x18000000 selector in the interrupt connect decodes to.

- `ioc4_eiattach` in `6.5.30/usr/cpu/sysgen/IP35boot/ioc4_ei.o` at 0x4b4
  **Inferred.** `ioc4_eiattach` is the IOC4 version of `eiattach`, with the same sequence: a subdevice check, a 0x38-byte record, a spinlock, a 512 KiB `kvpalloc` (0x53c), a character device and its nodes (0x594, 0x5f0), inventory, and an interrupt connect to `ioc4_ei_intr` (0x618). The subdevice index is 6 rather than 7, the interrupt connect passes 0x1 and 0x2800000 where the IOC3 version passes 0x18000000, and the device-add call takes extra arguments (0x594). The two routines share a layout, so these differences are the ones the disassembly shows.

- `ioc4_pckm_attach` in `6.5.30/usr/cpu/sysgen/IP35boot/ioc4_pckm.o` at 0x5c
  **Inferred.** `ioc4_pckm_attach` is the IOC4 version of `pckm_attach`, with the same keyboard and mouse setup. It uses `ioc4_subdev_enabled` with subdevice 1 (0x6c), writes a register with `ioc4_write_ireg` (0xac, 0x4c4), and connects the interrupt with `ioc4_intr_connect` (0x458) using arguments 0x1 and 0x40. It sends the same 0xf5, 0xf4 and 0xf6 command bytes and checks replies against 0xfa, so the PS/2 reading is inferred from the constants, as it is for the IOC3 sibling.

- `restart` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10c0
  **Inferred.** `restart` is the reboot path in the IP35 kernel object. When `panic_level` is zero (0x10d8), it loops over `maxnodes` (0x10e0, 0x1110, 0x1138). When `power_off_flag` is zero it calls `ni_reset_port` (0x11b8), and when it is set it calls `get_elsc` and `sc_power_down_all` (0x1178, 0x1180). It then calls `elsc_init` and `elsc_system_reset` (0x11dc, 0x11e4), prints on a negative reset result (0x1200), and writes the fixed addresses 0x9200000001680010 and 0x9200000001680008 (0x121c, 0x1228) that `ni_reset_port` also writes. Its `us_delay` calls use 1,000,000 and 3,000,000 (0xf4240, 0x2dc6c0), which read as one and three seconds if the unit is microseconds; the `reboot_on_panic == 2` test (0x1160) and the loop bounds are only partly resolved.

- `get_nasid` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x12b4
  **Inferred.** `get_nasid` is a bare load and return. It reads the doubleword at the fixed address 0x9200000001600000 (0x12b8) and returns it, and the return value is not resolved in the disassembly (0x12c8). `is_fine_dirmode` and `start` read the same address. The name suggests a node-ID accessor, but the disassembly does not confirm that field.

- `get_slice` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x12d0
  **Inferred.** `get_slice` loads the doubleword at the fixed address 0x9200000001000020 (0x12d4) and returns it unchanged (0x12d8). Both the load and the return are in the disassembly. Calling the value a slice number comes only from the name.

- `is_fine_dirmode` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x12e0
  **Inferred.** `is_fine_dirmode` loads the doubleword at 0x9200000001600000 (0x12e4), the same word `get_nasid` reads, and returns a value the disassembly does not resolve (0x12f0). The disassembly shows no branch or mask on the loaded value. The directory-mode role comes from the name alone.

- `ni_reset_port` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13b4
  **Inferred.** `ni_reset_port` stores two doublewords, to 0x9200000001680010 (0x13c0) and 0x9200000001680008 (0x13cc), and returns the second address (0x13c8). `restart` writes the same two addresses (0x121c, 0x1228) and calls this routine on the non-power-off path. The stored values are not in the disassembly, so the port-reset role rests on the name and the caller.

- `start` in `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1990
  **Inferred.** `start` is the boot entry by name. It loads the doubleword at 0x9200000001600000 (0x19c0), calls `mapped_kernel_setup_tlb` (0x19d8) with the entry arguments a2 and a3, and then jumps through a relocated pointer (`jr` at 0x19f8) to a target the disassembly does not resolve. The block from 0x1a00 to 0x1bac is not reached from the entry in the disassembly, so most of this routine's behavior is open.
