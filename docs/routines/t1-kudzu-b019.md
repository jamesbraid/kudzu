# Routine role statements, reading batch 19

Role statements for 25 routines from the 6.5.22 boot objects and kernel object set. Each statement is drawn from that routine's instruction facts (registers, calls, constants) and cites its addresses and callees. The grade is inferred unless the facts cover the whole body, and each statement says where it is unclear.

- `gbrick_eeprom_read` in `6.5.22/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x31fc
  **Inferred.** `gbrick_eeprom_read` maps its second argument through the nasid-to-compact-node table and compares it with the local nasid from `get_nasid` (0x3220). A match takes the `get_elsc` path (0x32fc); otherwise the routine opens a system-controller session with `sc_open` (0x327c), reads a board record with `read_board_ia` (0x32c0), and closes it with `sc_close` (0x32d0). `get_chimera_type` (0x3218) and the 0x800 bit of the third argument (0x3240) also steer the path. The name suggests a G-brick EEPROM read, but the facts do not show which board the record belongs to or what its fields mean.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** `gfx_flow_delayed_hiwater_intr_prehandler` loads a 16-bit id from a global slot and uses it to index the per-CPU attachment table and the per-CPU data table. When the attachment for that CPU is non-null (0x1c), it calls through a function pointer stored in the attachment record (`jalr` at 0x80) with fields from that record. The callee is not resolved, so the effect of the call is not established. The name suggests an interrupt pre-handler for the graphics flow high-water path.

- `gfx_flow_alloc` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x204
  **Inferred.** `gfx_flow_alloc` allocates a 0x70-byte flow record with `kmem_alloc` (0x244) and returns either zero or that record. On success it fills the record and looks up the widget with `xwidget_info_get` (0x2d8) and `xwidget_info_id_get` (0x2e8). It then allocates a table with `kvalloc` (0x39c, count 0x100 implied by the bound check at 0x338), looks up the master with `xwidget_info_master_get` (0x3e4) and `master_node_get` (0x3ec), and calls `gfx_flow_init` (0x42c). The branch at 0x234 jumps straight to `gfx_flow_init` when `gfx_flow_igfx_owners` is zero. What the table holds and what each record field means are not established.

- `gfx_flow_dealloc` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x460
  **Inferred.** `gfx_flow_dealloc` is the counterpart of `gfx_flow_alloc`. It reads a halfword at offset 0x50 and a word at 0x54 of its argument, then calls `kmem_free` (0x494) with the argument and the 0x70 size that the allocator used. Its return value is whatever `kmem_free` returns. The two field loads have no branch on them in the facts, so their use is unclear.

- `gfx_flow_attach_me` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x540
  **Inferred.** `gfx_flow_attach_me` records its argument as the current CPU's attachment. It reads the per-CPU index and the per-CPU data flags (the byte at +0x528 in the per-CPU record), stores the record into the attachment table (0x568), and writes a halfword at +0x60 and a doubleword at +0x68 of the record. It also stores to the fixed address window 0x9200000001000000 three times. That window is not identified by these facts.

- `gfx_flow_detach_me` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(gfx_flow.o)` at 0x6b4
  **Inferred.** `gfx_flow_detach_me` undoes `gfx_flow_attach_me`. It reads the link at +0x58 of its argument, writes the record fields at +0x60 and +0x68, and stores to the per-CPU attachment slot at 0x77c. The value stored into that slot is not in the facts, so it is unclear whether the slot is cleared or set to something else. It returns the address of that CPU's attachment slot.

- `prom_has_vga_support` in `6.5.22/usr/cpu/sysgen/IP35boot/gfxs.a(klconfig.o)` at 0x330
  **Inferred.** `prom_has_vga_support` reads two words at the fixed address 0x90000000180000a0 and the word after it. It compares the first word with 6 (unsigned) and then tests a second value (`bne` at 0x34c), and returns 1 or 0 on the two return paths. The threshold, the meaning of the two words, and the second test are not established. The name suggests a check for a VGA-class graphics device on the board. The decision logic stays open.

- `ef_ioc3eaddr` in `6.5.22/usr/cpu/sysgen/IP35boot/if_ef.o` at 0x738
  **Inferred.** `ef_ioc3eaddr` looks up the Ethernet address for an IOC3 Ethernet port. It formats a string with `sprintf` (0x75c), searches it with `strstr` twice (0x76c, 0x780), and tries `ibrick_mac_addr_get` (0x7b8) before falling back to `nic_get_eaddr` (0x7f8). It reads the system serial number through `get_elsc` (0x820) and `iobrick_get_sys_snum` (0x82c), reads kernel options with `kopt_find` (0x8dc, 0x920), and reports failures with `cmn_err`. The order of the attempts and the meaning of the 0x4000 and 0x3a comparisons are not established.

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc3_pckm.o` at 0x5c
  **Inferred.** `pckm_attach` attaches a PS/2 keyboard and mouse controller behind an IOC3 chip. It allocates state with `kmem_zalloc` (0xf0, 0x11c, 0x12c calls), creates device nodes with `hwgraph_char_device_add` (0x240, 0x3a4) and sets mode 0666 (0x1b6) with `hwgraph_chmod`. It connects `pckm_intr` through `ioc3_intr_connect` (0x470) and sends the PS/2 command bytes 0xf5 and 0xf4 (0x1e0, 0x20c) and 0xf6 (0x348), checking each acknowledgement with `data_check` against 0xfa. The byte values match the public PS/2 command set, but the facts show only that the bytes are sent, not what the controller answers.

- `eiattach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc3ei.o` at 0x4a8
  **Inferred.** `eiattach` attaches the Ethernet function of an IOC3 chip. It asks `ioc3_subdev_enabled` about subdevice 7 (0x4b8), creates a device node with `hwgraph_char_device_add` (0x590), and allocates a 0x38-byte state block with `kmem_zalloc` (0x510) and a 0x80000-byte region with `kvpalloc` (0x548). It connects `ioc3_ei_intr` through `ioc3_intr_connect` (0x610) and asks `ioc3_is_console` (0x5d4) whether the chip is the console. The purpose of the large region is not shown.

- `ioc4_eiattach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc4_ei.o` at 0x4b4
  **Inferred.** `ioc4_eiattach` is the IOC4 counterpart of `eiattach`. It asks `ioc4_subdev_enabled` about subdevice 6 (0x4c4), maps the chip with `ioc4_mem_ptr` (0x4f8), and allocates the same 0x38-byte state block and 0x80000-byte region. It creates a device node with a different argument set (0x594) and connects `ioc4_ei_intr` through `ioc4_intr_connect` (0x618) with arguments 0x1 and 0x2800000, whose meaning is not decoded. The IOC4 register meanings are not in these facts.

- `ioc4_pckm_attach` in `6.5.22/usr/cpu/sysgen/IP35boot/ioc4_pckm.o` at 0x5c
  **Inferred.** `ioc4_pckm_attach` is the IOC4 counterpart of `pckm_attach`. It enables subdevice 1 through `ioc4_subdev_enabled` (0x6c), writes the 0x40 register with `ioc4_write_ireg` (0xac, 0x4c4), and allocates state with `kmem_zalloc`. It sends the same PS/2 command bytes (0xf5, 0xf4, 0xf6) with 0xfa acknowledgements, creates device nodes (0x224, 0x388), connects `pckm_intr` through `ioc4_intr_connect` (0x458), and calls `ioc4_pckm_mlreset` (0x558).

- `restart` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0xe90
  **Inferred.** `restart` runs the reset sequence. It waits with `us_delay` (0xf1c, 0xf58, 0xfd8, with arguments 0xf4240 and 0x2dc6c0), and when `power_off_flag` is set it powers down the bricks through `get_elsc` and `sc_power_down_all` (0xf48, 0xf50). Otherwise the path goes to `ni_reset_port` (0xf88), and it calls `elsc_init` and `elsc_system_reset` (0xfac, 0xfb4) before printing with `printf` (0xfd0). It writes the two registers at 0x9200000001680010 and 0x9200000001680008 that `ni_reset_port` also writes. A loop keyed on the master processor id (0xf70) and a `reboot_on_panic` check against 2 (0xf30) shape the path, but the exact order and the time unit of the delays are not established.

- `get_nasid` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1084
  **Inferred.** `get_nasid` loads one 64-bit word from the fixed address 0x9200000001600000 and returns it. The routine does no shifts or masks, and the return value is not decoded further. Callers in this object pass the result as a node id (nasid), which is the basis for the name.

- `get_slice` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10a0
  **Inferred.** `get_slice` loads the 64-bit word at 0x9200000001000020 and returns it unchanged. `getcpuid` (0xf010) passes that same word to `nasid_slice_to_cpuinfo` together with the nasid, so this word is probably the slice identifier. The window itself is not identified here.

- `is_fine_dirmode` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10b0
  **Inferred.** `is_fine_dirmode` loads the word at 0x9200000001600000, the same address `get_nasid` reads, and returns it raw. It contains no bit test, so any flag decoding is left to the caller. The name suggests a directory-mode feature check, but nothing in this routine shows that.

- `ni_reset_port` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1184
  **Inferred.** `ni_reset_port` writes two doublewords, to 0x9200000001680010 and 0x9200000001680008, and returns the second address. The stored values are not decoded in the facts, so which bits it sets or clears is unclear. The same pair is written by `restart`.

- `start` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1760
  **Inferred.** `start` reads the word at 0x9200000001600000 (the one `get_nasid` reads), calls `mapped_kernel_setup_tlb` (0x17a8), and then jumps through a relocated pointer (`jr` at 0x17c8). The code from 0x17d0 onward is not reached by any path from the entry in the decoded facts. The jump target is resolved only by relocation and is not identified here, so this reads as an early entry that hands off to a later stage.

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1a28
  **Established.** `silence_all_audio` is an 8-byte routine with only a return. It has no loads, stores, or calls, so in this build it does nothing. The name suggests it silences audio hardware, but nothing in the routine shows that. Only the empty body is established; the name's intent is inferred.

- `cpu_node_probe` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x60c0
  **Inferred.** `cpu_node_probe` probes one node. It gets the nasid through `get_nasid` (0x6160), follows the pointer at +0xd0 of the node's hub window structure (base 0x9600000000000000 with the nasid shifted in, plus 0x2000) to a field at +0x184, and stores 16-bit values in about a dozen places. It passes an incremented count from `[a1]` to `do_cpumask` (0x61cc), prints node and brick counts with `printf` (0x6358, 0x6378, 0x6384), and calls an assertion with constant 0xb9 (0x639c). The meaning of the fields and the purpose of the hub window base are not established.

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0xe0f0
  **Inferred.** `mlreset` is the machine-level init for the master processor. It sets up performance counter registers through `r1nkperf_control_register_set` and `r1nkperf_data_register_set` (indices 0 to 3), flushes the TLB and cache (`flush_tlb` 0xe168, `flush_cache` 0xe170), and parses a maximum node count with `atoi` (0xe274). It then calls per-chip reset and init in order: `ioc3_mlreset`, `ioc4_mlreset`, `l1_mlreset`, `nvram_baseinit`, `cpu_node_probe` (0xe2ac), `gen_region_mask` (0xe374), `setup_replication_mask` (0xe3a8), `iograph_early_init`, `mem_init`, `hubdev_init`, `gda_sym_init`, `verify_snchip_rev`, `he_arcs_set_vectors`, and `intrgroup_init` (0xe548). Three assertion calls use constants 0x117, 0x11a, and 0x12c. The facts show the call order, not what each callee does.

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0xf010
  **Inferred.** `getcpuid` returns a halfword at offset 0x1e of a per-slice CPU info record. It raises the interrupt level with `splhi` (0xf01c), gets the nasid through `get_nasid` (0xf024), and looks up the record with `nasid_slice_to_cpuinfo` using that nasid and the word at 0x9200000001000020. It restores the level with `splx` (0xf044), and an assertion call with constant 0x525 follows (0xf074), and its condition is not decoded here. The field at +0x1e is probably the CPU id, but its meaning is inferred from the name.

- `page_local_calias` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x10a1c
  **Inferred.** `page_local_calias` reads the word at 0x9200000001000028, the word after the one `get_slice` reads. It branches to a short exit when that word is zero, and otherwise returns 0 or 1 from an unsigned comparison of an argument with a second value that is not decoded. The name suggests a local-alias check for a page, but that meaning is not in the facts.

- `save_cache_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13188
  **Inferred.** `save_cache_error` copies cache-error state into a record. It reads a doubleword from the table at 0x9000000034000000, reads several 32-bit and byte fields through pointers the facts do not resolve, and reads the doubleword at 0x9200000001000020. It stores 8-byte and 4-byte values into a record whose layout is not decoded, and calls `get_nasid` (0x13198) and `chk_for_pebrick` (0x13318) with a 0xffffffffff mask. The destination layout and the meaning of the 0x9000000034000000 window are not established.

- `save_hub_pi_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x13f2c
  **Inferred.** `save_hub_pi_error` copies hub PI error fields from the block its first argument points to into a fixed-layout record at its second. It reads doublewords at offsets 0x50, 0x58, 0x400, 0x430, 0x440, 0x450, 0x460, 0x4c8, and 0x4d0 and writes them at offsets 0x10 through 0x50 of the record. It clears the destination first with `bzero` (0x13f48, 0x60 bytes) and returns the doubleword at +0x58 of that input block. The name refers to hub PI error registers, but the facts show only the copy pattern, not the field names.
