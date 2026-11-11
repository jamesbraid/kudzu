# Routine role statements: kudzu 6.5.30 IP30 boot objects

Role statements for 25 routines from the IP30 boot objects of the 6.5.30 install. Each one comes from the disassembled instructions (calls, constants, register use) at the cited addresses. Every grade is inferred, because the disassembly of none of these routines fully establishes its role. Where the disassembly leaves part of a role open, the entry says so.

- `_64bitunlockdebug` in `6.5.30/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** A debug path of a 64-bit unlock. It compares the halfword at +0x2a of its second argument with the halfword at 0xffffffffffffc3a0 (`bne` at 0x2208). On a mismatch it jumps to 0x22a8, rewrites the doubleword at +0x40, and reports through `cmn_err` at 0x22c0 with level 4; both paths then loop back into the field code at 0x2210 and 0x2250. The meaning of the halfword comparison and the loop bound are left open.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** The high-water interrupt prehandler for a graphics flow. It loads the owner for index a0 from `gfx_flow_owners` (0xc), and when that owner is set (`bne` at 0x14) it reads the owner's fields at +0x28 and +0x30 (0x6c, 0x78) and makes an indirect call at 0x74 with the +0x30 value as its first argument. It also takes and releases `gfx_flow_heart_lock` (0x3c and 0x5c) and logs through `cmn_err` at 0x24 and 0x34. The indirect call's target does not resolve in the disassembly, so what the handler dispatches to is left open.

- `gfx_flow_pipe_head_init` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0xd0
  **Inferred.** Fills in the head of a flow pipe record. It reads the fields at +0x8, +0x10, +0x1c and +0x20 of its first argument (0xf8 to 0x11c), stores a word at +0x58 (0xdc), and writes them into the owner slot picked by the halfword at 0xffffffffffffc3a0 and into the fixed words at 0xffffffffaff00040 and 0xffffffffaff00048. The stores at 0x13c and 0x150 fall between `mutex_spinlock` (0x138) and `mutex_spinunlock` (0x160) on `gfx_flow_heart_lock`. Two stores use computed addresses that the disassembly does not resolve, so the layout of those fields is partly open.

- `gfx_flow_alloc` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x178
  **Inferred.** Creates a flow record under `gfx_flow_init_lock` (`mutex_spinlock` at 0x1ac, `mutex_spinunlock` at 0x1d8). It calls `kmem_alloc` with size 0x68 (0x1e4), and on success looks up the widget with `xwidget_info_get` and `xwidget_info_id_get` (0x1fc, 0x208). It then allocates a second block with `kvalloc` (0x29c), passes the `kmem_alloc` record to `gfx_flow_pipe_head_init` (0x2d8), calls `gfx_flow_init` (0x308), and connects `gfx_flow_delayed_hiwater_intr_prehandler` through `heart_flow_control_connect` (0x348). The 0x68 size matches the block that `gfx_flow_dealloc` frees at 0x444.

- `gfx_flow_dealloc` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x384
  **Inferred.** Tears down a flow record. It reads the halfword at +0x5c and the words at +0x58 of its argument, and logs through `cmn_err` at 0x3a8 with level 4 when the halfword is not -1 (`beq` at 0x398). It then takes `gfx_flow_heart_lock` (0x3b4) and `gfx_flow_init_lock` (0x3ec) in turn, frees the record with `kmem_free` at size 0x68 (0x444), and calls `heart_flow_control_disable` (0x464) and `heart_flow_control_disconnect` (0x480) around a signed test of a count at 0x490. Which of the two heart calls runs for which count is left open.

- `gfx_flow_attach_me` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x53c
  **Inferred.** Attaches a flow to its owner table. It reads the halfword at 0xffffffffffffc3a0 to pick a slot and stores a doubleword into `gfx_flow_owners` at that slot (0x548, 0x560). It copies the doublewords at +0x8 and +0x10 of its argument into +0x60 (0x564 to 0x574), writes a halfword at +0x5c and a word at +0x58, and takes `gfx_flow_heart_lock` (`mutex_spinlock` at 0x5a8, `mutex_spinunlock` at 0x5f4). After the unlock it calls `heart_flow_control_enable` with the value at +0x58 (0x600).

- `gfx_flow_detach_me` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x620
  **Inferred.** The detach side of `gfx_flow_attach_me`. It takes `gfx_flow_heart_lock` (`mutex_spinlock` at 0x630, passing its own arguments through) and releases it at 0x674. It reads and writes the doubleword at 0xffffffffaff00040 (0x65c, 0x678), rewrites the record's +0x60 doubleword, +0x5c halfword and +0x58 word (0x694 to 0x6c4), and calls `heart_flow_control_disable` with the value at +0x58 (0x67c). The values it stores are not in the disassembly, so the exact state it leaves is left open.

- `giobr_attach` in `6.5.30/usr/cpu/sysgen/IP30boot/giobr.o` at 0x744
  **Inferred.** Attaches a GIO bridge to the xtalk bus. It maps the bridge's register window with `xtalk_piotrans_addr` (0x780, size 0x1000000), tests bit 0x20 of the word at +0xc of that mapping (0x790), and registers the bridge with `hwgraph_path_add` (0x7bc) and `hwgraph_char_device_add` (0x7dc). It allocates and zeroes a 0x108-byte per-bridge record (`kmem_alloc` at 0x7ec, `bzero` at 0x7fc), probes up to four slots through `giobr_slot_probe` (0x934, loop bound 4 at 0x96c), resets the links with `do_giobr_link_reset` (0x988), and registers and attaches a GIO provider and device (0x994 to 0x9c4). Which slots end up with devices depends on the probe results, which the disassembly does not split out.

- `heart_mlreset` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x4c8
  **Inferred.** Resets HEART chip state at boot. It calls `getcpuid` first (0x4d0), then writes doublewords at 0x9000000018000060 and 0x9000000018000068 (0x4e8 to 0x528) and a word at 0x9000000018000044 (0x508), and reads and writes the doubleword at 0xffffffffaff10040 (0x50c, 0x518). It stores into the words at 0xffffffffaff10000 through 0xffffffffaff10018 (0x534 to 0x554) and returns the table pointer at 0xffffffffaff10000 (0x568). The 0x90000000180000xx addresses are read as HEART registers from the routine's name and the chip's window, which these disassembly does not confirm.

- `heart_startup` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x5dc
  **Inferred.** Brings the HEART up as an xtalk provider. It registers the interrupt table `heart_ivec` with `xtalk_provider_register` (0x624) and starts it with `xtalk_provider_startup` (0x62c). It adds hwgraph paths and edges (0x614, 0x640, 0x660, 0x67c), sets the device master (0x68c), and reads the switch with `xswitch_info_get` and `xswitch_info_link_ok` (0x6dc, 0x708). It registers widgets through `xwidget_register` (0x6cc, 0x784) in a loop controlled by a signed counter tested at 0x790, and builds a path name with `sprintf` (0x720). The loop's limit and how the link check gates each widget are left open.

- `heart_intr` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x1630
  **Inferred.** The HEART interrupt dispatcher. It reads the pending mask at 0xffffffffaff10038 (0x1654, 0x16a8), finds an interrupt number with `ffintr64` (0x16b8), and calls `buserror_intr` (0x171c) when that number is 0x3b or higher. Otherwise it indexes the vector table `heart_ivec` at a 64-byte stride, synchronizes through the record's semaphore with `cvsema`, `valusema` and `vsema` (0x1690, 0x16f8, 0x170c), and calls the record's handler indirectly at 0x1730 with the record's +0x0 and +0x8 values. The ll/sc pair at 0x1774 and 0x1780 updates a counter at +0xd0 of a global pointer, and what that counter means is left open.

- `heart_rev` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x1978
  **Inferred.** Returns the HEART revision. It returns the global `the_heart_rev` when that value is non-negative (`bltz` at 0x197c), and otherwise reads the word at 0x9000000018000004 (0x1990) and returns it. No store to `the_heart_rev` appears in these disassembly, so whether the cache is filled anywhere else is left open.

- `ef_ioc3eaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/if_ef.o` at 0x738
  **Inferred.** Fetches the Ethernet address for an ioc3 interface. It loads the pointer at +0x398 of its argument (0x73c) and calls `nic_get_eaddr` with the buffers at that pointer's +0x30 and +0x34 (0x750). On a nonzero result it logs through `cmn_err` at level 1 (0x77c), using the word at +0x38c of its argument. It returns 0 or 5 (0x768), and which path gives which value is not split out in the disassembly.

- `pckm_attach` in `6.5.30/usr/cpu/sysgen/IP30boot/ioc3_pckm.o` at 0x5c
  **Inferred.** Attaches the PS/2 keyboard and mouse ports of the ioc3 chip. It allocates port state with `kmem_zalloc` (0xf0, 0x110, 0x120), initializes a mutex in it (`init_mutex` at 0x13c), and checks the subdevice with `ioc3_subdev_enabled` (0x6c). It traverses the hardware graph for the two port nodes (0x8c, 0xa0) and sends the bytes 0xf5, 0xf4 and 0xf6 through `pckm_pollcmd`, checking each reply against 0xfa with `data_check` (0x240, 0x26c, 0x388); those constants look like PS/2 command bytes. It registers the devices with `device_inventory_add` (0x20c, 0x350) and connects `pckm_intr` through `ioc3_intr_connect` (0x450). A branch on `ioc3_is_console` (0xdc) and the `showconfig` flag selects a console path that the disassembly only partly maps.

- `_get_timestamp` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x11a8
  **Inferred.** Returns a 32-bit counter value. It loads the unsigned word at 0xffffffffaff20004 (0x11b0) and returns it with no other effect. The routine's name suggests a timestamp source, and the address is the only place the value comes from in these disassembly.

- `getcpuid` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x11b4
  **Inferred.** Returns a 64-bit word that identifies the calling CPU. It loads the doubleword at 0xffffffffaff50000 (0x11bc) and returns it unchanged, without reading any of its arguments. Its callers include `bootstrap` (0x1310) and `mlreset` (0x2348), which pass their own argument registers in.

- `bootstrap` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x12b4
  **Inferred.** Early boot entry that sets up a wired TLB entry and then jumps to the next stage. It reads the word at +0x10 of its first argument (0x12d4), calls `getcpuid` with a3 = 6 (0x1310), and installs a wired entry through `tlbwired` with arguments 0, 0, -0x4000, 1 and 0x6000 (0x1370). It stores a doubleword at 0xffffffffffffc4a8, a byte at 0xffffffffffffc04f and a word at 0xffffffffffffc65c (0x137c, 0x1380, 0x138c), and then makes a direct jump to `cboot` (0x1388). The meaning of those globals and of the field at +0x10 is left open.

- `_ticksper1024inst` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1470
  **Inferred.** A timing calibration. It reads the doubleword at 0xffffffffaff20000 at 0x1480 and again at 0x1498, around two loops that branch while a0 and t1 are positive (0x1488, 0x1490). The name suggests ticks per 1024 instructions, but the disassembly does not show the arithmetic that turns the two reads into a count, so that part is open.

- `start` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1940
  **Inferred.** Kernel entry stub that hands off to another routine. It makes a relocated jump to `.text+6536` (`jr` at 0x1984) and passes its four argument registers through unchanged. The disassembly reaches no code from 0x198c onward, and the target is given only as a relocation offset, so the purpose of the hand-off is left open.

- `silence_all_audio` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1b9c
  **Inferred.** Silences the audio path and then halts through `ip30_rad_halt`. It loads the word at `is_octane_lx` (0x1bb4) and, when that word is zero, jumps straight to the halt (`beq` at 0x1bb8). When it is nonzero, it first writes a word to 0x900000001f600038 (0x1bd8), the same register that `mlreset` writes at 0x258c, and then tail-jumps to `ip30_rad_halt` (`j` at 0x1bdc) with its arguments unchanged.

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x22d0
  **Inferred.** Machine-dependent reset for the boot CPU on IP30. It sets up the r1nkperf control and data registers for counters 0 to 3 (0x22e4 to 0x2308, 0x2538 to 0x255c), reads interrupt request state with `get_cpu_irr` (0x2310, 0x2328), and calls `getcpuid` and `heart_mp_intr_init` (0x2348, 0x2350). It flushes the TLB and cache (0x2358, 0x2360, 0x24c0), reads board revision, system id and NVRAM tables (`init_board_rev`, `init_sysid`, `load_nvram_tab` at 0x23fc to 0x2414), and tests the `arg_heart_invalidate_war` setting with `is_specified` and `atoi` (0x2484, 0x2494). It initializes `ip30clocklock` and `ip30ledlock` (0x24d0, 0x24e0), writes the `bzero_pref`, `bcopy_pref1` and `bcopy_pref2` words (0x24b0 to 0x24bc), and tests `cpu_enabled` in a loop (0x24f0, 0x25b4). What the invalidate-workaround setting selects, and how the CPU loop ends, are left open.

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2690
  **Inferred.** Updates the front-panel LED register pair under a lock. It takes `ip30ledlock` (`mutex_spinlock` at 0x26a0), writes words to 0x900000001f600040 and 0x900000001f600044 (0x26bc to 0x26ec), releases the lock at 0x26c8, and calls `flushbus` (0x26d0). It returns the result of `flushbus`. The values written come from the arguments, which the disassembly shows only as pass-through.

- `cpu_disable` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2884
  **Inferred.** Marks one CPU as disabled. It reads and writes the doubleword at 0xffffffffaff00000 (0x2894, 0x28a8) and stores a word into `processor_enabled` at the index in a0 (0x28b0). The stored value is not in the disassembly, so the disabled marker is inferred from the routine's name and the table it writes.

- `cpu_exists` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x28b4
  **Inferred.** Reports whether a CPU is present. It loads the doubleword at 0xffffffffaff00080 (0x28c0), the same word that `mlreset` loads at 0x23b4 and passes to `init_board_rev`. The disassembly leaves the return value unresolved, so what it reports is left open.

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x38ac
  **Inferred.** Sends an interrupt by writing a doubleword to 0xffffffffaff10020 (0x38bc) and returns 0 (0x38c0). The address sits in the same 0xffffffffaff10000 window that `heart_mlreset` and `heart_intr` use, which suggests a HEART interrupt register. The value written is not in the disassembly.
