# Routine role statements, reading batch 19

Role statements for 25 routines from the 6.5.22 boot objects and kernel object set. Each statement is drawn from that routine's instruction facts (registers, calls, constants) and cites its addresses and callees. The grade is inferred unless the facts cover the whole body, and each statement says where it is unclear.

- `gbrick_eeprom_read` in `6.5.22/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x31fc
  **Inferred.** `gbrick_eeprom_read` calls `get_chimera_type` (0x3218) and `get_nasid` (0x3220), then tests the 0x800 bit of its third argument (0x3240). When the bit is clear the branch goes to 0x32e0, which these facts do not cover. When it is set, the routine compares its second argument directly with the `get_nasid` result (0x3248). On a match it takes the path at 0x32fc, which calls `get_elsc` with an argument built from a doubled value and the `nasid_to_compact_node` symbol. Otherwise it opens a session with `sc_open` (0x327c; a negative result branches to 0x3320), calls `read_board_ia` (0x32c0) with the `sc_open` result as its second argument, closes with `sc_close` (0x32d0) and branches on whether the `read_board_ia` result is zero (0x32d8). A comparison with 0x4000 (0x32a0) also selects a path. Role (from name): a G-brick EEPROM read. The facts do not show which board the record belongs to or what its fields mean.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** `gfx_flow_delayed_hiwater_intr_prehandler` loads a 16-bit value from a global slot (0x0) and uses it, shifted left by 3, to index the `gfx_flow_cpu_attachments` table. When the entry it finds is non-null (branch at 0x1c) it makes a call through a target the facts leave unresolved (call at 0x80), passing the entry itself and fields at +0x20 and +0x28 of it. Otherwise it falls through to a store (0x5c) and a return (0x64). The effect of the call is not established. Role (from name): an interrupt pre-handler for the graphics flow high-water path.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x204
  **Inferred.** `gfx_flow_alloc` first tests `gfx_flow_igfx_owners` (0x234); when it is zero the branch goes straight to `gfx_flow_init` (0x42c). Otherwise it calls `kmem_alloc` with size 0x70 (0x244) and returns either zero or that result (0x2d0). When the allocation succeeds (0x24c) it calls `xwidget_info_get` (0x2d8) and `xwidget_info_id_get` (0x2e8), stores a number of fields into the new record, calls `kvalloc` (0x39c, with arguments including 0x44, 0x38 and 0x100, and a bound check against 0x101 at 0x338), then `xwidget_info_master_get` (0x3e4) and `master_node_get` (0x3ec). `gfx_flow_init` (0x42c) is the last call in address order; the facts do not show which paths reach it. What the `kvalloc` region holds and what each record field means are not established.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x460
  **Inferred.** `gfx_flow_dealloc` is the counterpart of `gfx_flow_alloc`. It reads a halfword at offset 0x50 and a word at 0x54 of its argument, then calls `kmem_free` (0x494) with the argument and the 0x70 size that the allocator used. Its return value is whatever `kmem_free` returns. The two field loads have no branch on them in the facts, so their use is unclear.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x540
  **Inferred.** Role (from name): `gfx_flow_attach_me` attaches a flow record to the current CPU. The facts show no calls. It loads a halfword (0x544) and a byte at +0x528 (0x578), stores a halfword at +0x60 (0x550) and doublewords at +0x0 (0x568, 0x5b4, 0x5d0, 0x5ec, 0x648) and +0x68 (0x570), and loads at +0x8, +0x14, +0x58 and +0x4. The base registers and the stored values are not decoded, so which table, record or address window each store targets is not established. The return value is not decoded.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x6b4
  **Inferred.** Role (from name): `gfx_flow_detach_me` undoes `gfx_flow_attach_me`. The facts show it loading a link at +0x58 (0x6b4), a halfword (0x6b8) and a byte at +0x528 (0x6d4), storing doublewords at +0x0 (0x6c4, 0x73c, 0x748, 0x750, 0x758, 0x77c), a word at +0x14 (0x76c), a doubleword at +0x68 (0x784) and a halfword at +0x60 (0x78c). It returns an address formed from the `gfx_flow_cpu_attachments` table and a halfword index shifted left by 3, that is one slot of that table. The stored values are not in the facts, so whether any slot is cleared is not established.
  Reviewed: 2026-11-12 (sonnet)

- `prom_has_vga_support` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(klconfig.o)` at 0x330
  **Inferred.** `prom_has_vga_support` loads a word from the fixed address 0x90000000180000a0 (0x338) and a second word (0x33c) whose address the facts do not resolve. If the first word is below 6 (unsigned, branch at 0x344) it goes to 0x35c, which these facts do not cover. Otherwise a nonzero second value returns 0 (0x360) and a zero second value returns 1 (0x354). The threshold and the meaning of the two words are not established. Role (from name): a check for a VGA-class graphics device on the board.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.22/usr/cpu/sysgen/IP35boot/if_ef.o` at 0x738
  **Inferred.** Role (from name): `ef_ioc3eaddr` looks up the Ethernet address for an IOC3 Ethernet port. The facts show a `sprintf` into a stack buffer (0x75c) followed by two `strstr` calls on it (0x76c, 0x780). Either call returning zero branches to 0x7ec, which calls `nic_get_eaddr` (0x7f8). When both find a match, a comparison with 0x4000 (0x79c) either goes on to `ibrick_mac_addr_get` (0x7b8; a nonzero result branches to 0x8c4) or skips to 0x7cc. Other code calls `mactoh` (0x7dc), `get_elsc` (0x820), `iobrick_get_sys_snum` (0x82c), `kopt_find` (0x8dc, 0x920) and `cmn_err` (0x8b4, 0x8d0, 0x8f8, 0x914, 0x93c). The order in which these are reached across the branches and the meaning of the 0x4000 and 0x3a comparisons are not established.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc3_pckm.o` at 0x5c
  **Inferred.** Role (from name): `pckm_attach` attaches a PS/2 keyboard and mouse controller behind an IOC3 chip. The facts show a call to `ioc3_subdev_enabled` with 3 (0x6c), with a branch to 0x500 when it returns zero; three `kmem_zalloc` calls (0xf0, 0x11c, 0x12c, sizes 0x70, 0x58 and 0x58); `hwgraph_char_device_add` (0x240, 0x3a4), each followed by `hwgraph_chmod` with 0x1b6 (mode 0666); and `ioc3_intr_connect` (0x470) with `pckm_intr`. Under conditions on fields of the state block it calls `pckm_pollcmd` with 0xf5 (0x1e0), 0xf4 (0x20c) and 0xf6 (0x348), each followed by `data_check` against 0xfa; a failed check branches away. The byte values match the public PS/2 command set, but the facts show only that the bytes are sent, not what the controller answers.
  Reviewed: 2026-11-12 (sonnet)

- `eiattach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc3ei.o` at 0x4a8
  **Inferred.** Role (from name): `eiattach` attaches the Ethernet function of an IOC3 chip. It calls `ioc3_subdev_enabled` with 7 (0x4b8) and branches to 0x634 when the result is zero. It then calls `hwgraph_traverse` twice, `hwgraph_fastinfo_get`, `ioc3_mem_ptr`, `kmem_zalloc` with 0x38 and 1 (0x510), `spinlock_init` (0x538) and `kvpalloc` with arguments 1 and 0x80000 (0x548; a zero result is tested at 0x554). It creates a device node with `hwgraph_char_device_add` (0x590), calls `hwgraph_chmod` with 0x124, and calls `ioc3_is_console` (0x5d4). Depending on that result it calls `device_controller_num_set` with 1 or -1, and `ei_make_hwgraph_nodes` on the console path. It connects `ioc3_ei_intr` through `ioc3_intr_connect` (0x610). What the 0x80000 argument means and the purpose of the region are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `ioc4_eiattach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc4_ei.o` at 0x4b4
  **Inferred.** `ioc4_eiattach` has the same shape as `eiattach`. It calls `ioc4_subdev_enabled` with 6 (0x4c4) and branches to 0x640 when the result is zero. It then calls `hwgraph_traverse`, `hwgraph_fastinfo_get`, `ioc4_mem_ptr` (0x4f8), `kmem_zalloc` with 0x38 and 1 (0x508), `spinlock_init` (0x52c) and `kvpalloc` with arguments 1 and 0x80000 (0x53c). It calls `hwgraph_char_device_add` (0x594) with a different argument list from `eiattach` (a4 of -1 and further arguments), `hwgraph_chmod` with 0x124, and `ioc4_is_console` (0x5d8). It connects `ioc4_ei_intr` through `ioc4_intr_connect` (0x618) with arguments 0x1 and 0x2800000, whose meaning is not decoded. The IOC4 register meanings are not in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `ioc4_pckm_attach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc4_pckm.o` at 0x5c
  **Inferred.** Role (from name): `ioc4_pckm_attach` is the IOC4 counterpart of `pckm_attach`. It calls `ioc4_subdev_enabled` with 1 (0x6c) and branches to 0x4ec when the result is zero. It calls `ioc4_write_ireg` with 0x40 as its second argument (0xac, with a2 and a3 of 1; again at 0x4c4 with a2 of 0), `kmem_zalloc` three times (0xd8, 0x100, 0x110) and `ioc4_is_console` (0xc4). Under conditions on fields of the state block it calls `pckm_pollcmd` with 0xf5 (0x1c4), 0xf4 (0x1f0) and 0xf6 (0x32c), each followed by `data_check` against 0xfa. It creates device nodes with `hwgraph_char_device_add` (0x224, 0x388), connects `pckm_intr` through `ioc4_intr_connect` (0x458) and calls `ioc4_pckm_mlreset` (0x558), reached from the branches at 0x27c and 0x548. What the register write does is not established.
  Reviewed: 2026-11-12 (sonnet)

- `restart` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0xe90
  **Inferred.** Role (from name): `restart` runs a reset sequence. The facts show a test of `panic_level` (0xea8), a loop over `maxnodes` with a store per pass (0xed0, 0xef8), and `us_delay` with 0xf4240 (0xf1c, 0xfd8) and 0x2dc6c0 (0xf58). A comparison of `reboot_on_panic` with 2 (0xf30) branches to 0xfa0, which calls `get_nasid`, `elsc_init` and `elsc_system_reset` (0xfac, 0xfb4); a negative result goes on to `printf` (0xfd0). When `power_off_flag` is nonzero (0xf40) it calls `get_elsc` and `sc_power_down_all` (0xf48, 0xf50) and `us_delay`; when it is zero it calls `ni_reset_port` (0xf88). A test on the master processor id (0xf70) loops back to the `power_off_flag` test. The stores at 0xfec and 0xff8 have no resolved target in the facts. The exact order and the time unit of the delays are not established.
  Reviewed: 2026-11-12 (sonnet)

- `get_nasid` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1084
  **Inferred.** `get_nasid` loads one 64-bit word (0x1088) and returns a value derived from it; the facts do not decode the returned value or the address read. Callers in this object use the result as a node id: `restart` passes it to `elsc_init`, `getcpuid` passes it to `nasid_slice_to_cpuinfo`, and `cpu_node_probe` shifts it into a hub window address. Role (from name): the local node id (nasid).
  Reviewed: 2026-11-12 (sonnet)

- `get_slice` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10a0
  **Inferred.** `get_slice` loads the 64-bit word at 0x9200000001000020 and returns it unchanged. `getcpuid` (0xf010) passes that same word to `nasid_slice_to_cpuinfo` together with the nasid, so this word is probably the slice identifier. The window itself is not identified here.
  Reviewed: 2026-11-12 (sonnet)

- `is_fine_dirmode` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10b0
  **Inferred.** `is_fine_dirmode` loads one 64-bit word (0x10b4) and returns a value derived from it. The facts do not decode the returned value, so whether a bit test or other transformation is applied is not established. Role (from name): a directory-mode feature check; nothing in the facts shows that.
  Reviewed: 2026-11-12 (sonnet)

- `ni_reset_port` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1184
  **Inferred.** `ni_reset_port` stores two doublewords (0x1190, 0x119c) and returns 0x9200000001680008. The first store target and the stored values are not decoded in the facts, so which bits it sets or clears is unclear. `restart` calls it (0xf88).
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1760
  **Inferred.** `start` reads a word (0x1790), calls `mapped_kernel_setup_tlb` (0x17a8), and then jumps (0x17c8) to .text+6096, that is 0x17d0, which the facts mark as a tail call. The code at 0x17d0 onward is not part of these facts. Role (from name): an early kernel entry point that sets up the mapped kernel TLB and continues.
  Reviewed: 2026-11-12 (sonnet)

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1a28
  **Established.** `silence_all_audio` is an 8-byte routine with only a return. It has no loads, stores, or calls, so in this build it does nothing. The name suggests it silences audio hardware, but nothing in the routine shows that. Only the empty body is established; the name's intent is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_node_probe` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x60c0
  **Inferred.** `cpu_node_probe` calls `get_nasid` (0x6160), loads a pointer at +0xd0 of a structure addressed by the nasid shifted left by 0x21 into the base 0x9600000000000000 or'd with 0x2000, and loads the word at +0x184 of it (0x61e8); a comparison of that word with 0x5045626b (0x6200) selects a path. It stores 16-bit values in about a dozen places and loops with bounds 0x80, 0x200, 0x3 and 0x4. It calls `do_cpumask` with a fifth argument of `[a1]`+1 (0x61cc), `printf` three times (0x6358, 0x6378, 0x6384; the call at 0x6378 passes the +0x184 word and `num_pebricks`) and `assfail` with 0xb9 as its third argument (0x639c). The meaning of the fields and the hub window base are not established.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0xe0f0
  **Inferred.** Role (from name): `mlreset` is machine-level init. By address order the facts show `r1nkperf_control_register_set` and `r1nkperf_data_register_set` with indices 0 and 1 (0xe104 to 0xe128), then `get_cpu_irr` (0xe130, 0xe148). When its results equal 0xe or 0xf (0xe140, 0xe158) the code goes to 0xe188, which calls the same two functions with indices 2 and 3. Otherwise, when the first argument is nonzero (0xe160), it calls `flush_tlb` (0xe168) and `flush_cache` (0xe170) and returns the `flush_cache` result; when it is zero it continues to `get_nasid` (0xe1bc). The remaining calls, in address order, are `set_master_bridge_base`, `ioc3_mlreset`, `ioc4_mlreset`, `l1_mlreset`, `nvram_baseinit`, `is_fine_dirmode`, `is_specified` and `atoi` (0xe274) on `arg_maxnodes`, `redo_nodes`, `cpu_node_probe` (0xe2ac), `gen_region_mask` (0xe374), `getcpuid`, `get_cpu_cnode`, `setup_replication_mask` (0xe3a8), `get_nasid`, `spinlock_init`, `iograph_early_init`, `mem_init`, `hubdev_init`, `gda_sym_init`, `verify_snchip_rev`, `he_arcs_set_vectors` and `intrgroup_init` (0xe548). Three `assfail` calls use constants 0x117, 0x11a and 0x12c (0xe564, 0xe57c, 0xe594). The facts show the calls and their order in the code, not what each callee does or which are conditional.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0xf010
  **Inferred.** `getcpuid` returns a halfword at offset 0x1e of a per-slice CPU info record. It raises the interrupt level with `splhi` (0xf01c), gets the nasid through `get_nasid` (0xf024), and looks up the record with `nasid_slice_to_cpuinfo` using that nasid and the word at 0x9200000001000020. It restores the level with `splx` (0xf044), and an assertion call with constant 0x525 follows (0xf074), and its condition is not decoded here. The field at +0x1e is probably the CPU id, but its meaning is inferred from the name.
  Reviewed: 2026-11-12 (sonnet)

- `page_local_calias` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10a1c
  **Inferred.** `page_local_calias` loads the doubleword at 0x9200000001000028 (0x10a20), the one after the doubleword `get_slice` reads. When it is zero the branch goes to 0x10a4c, which these facts do not cover. Otherwise it returns 0 or 1 from an unsigned less-than comparison of two values the facts do not trace (0x10a44). Role (from name): a local-alias check for a page; that meaning is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `save_cache_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13188
  **Inferred.** `save_cache_error` calls `get_nasid` (0x13198), loads a doubleword (0x131b8) and compares a value with 0xc000 (0x131d4); on a match it goes to 0x13318 and calls `chk_for_pebrick` with 0xffffffffff as its fifth argument and arguments derived from 0x9600000000000000. Otherwise it loads a doubleword at +0x48 (0x131dc) and a number of 32-bit and byte fields through pointers the facts do not resolve, compares a doubleword with the one at 0x9200000001000020 (0x132cc), and stores 8-byte, 4-byte and byte values into a record (0x132f4 to 0x13308) whose layout is not decoded. The field meanings are not established. Role (from name): copies cache error state into a record.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_pi_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13f2c
  **Inferred.** `save_hub_pi_error` clears 0x60 bytes at its second argument with `bzero` (0x13f48). It loads doublewords at offsets 0x50 and 0x58 of its first argument and tests each against zero (0x13f60, 0x13f6c; zero goes to 0x13fe8 and 0x13ff8, which store at +0x10 of the record). It copies the doubleword at 0x400 to record offset 0x10, and, when bit 63 of the words at 0x430, 0x450 and 0x4c8 is set, copies 0x430 and 0x440 to 0x28 and 0x38, 0x450 and 0x460 to 0x30 and 0x40, and 0x4c8 and 0x4d0 to 0x48 and 0x50. It returns the doubleword at +0x58 of the input block. Role (from name): hub PI error registers; the facts do not show field names.
  Reviewed: 2026-11-12 (sonnet)
