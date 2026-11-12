# Routine role statements: kudzu 6.5.30 IP30 boot objects

Role statements for 25 routines from the IP30 boot objects of the 6.5.30 install. Each one comes from the disassembled instructions (calls, constants, register use) at the cited addresses. Every grade is inferred, because the disassembly of none of these routines fully establishes its role. Where the disassembly leaves part of a role open, the entry says so.

- `_64bitunlockdebug` in `6.5.30/usr/cpu/sysgen/IP30boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** Role (from name): a debug path of a 64-bit unlock. It compares the halfword at +0x2a of its third argument (a2) with the halfword at 0xffffffffffffc3a0 (`bne` at 0x2208). When they differ the branch goes to 0x22a8, which rewrites the doubleword at +0x40 and calls `cmn_err` at 0x22c0 with level 4. A second `cmn_err` call at 0x22d8, with level 0, is reached from the branch at 0x2244 (taken when a7 < t0, unsigned). Between 0x2210 and 0x2298 it also loads and stores fields at offsets such as +0x48, +0x60, +0x68 and +0x78 of a base the facts do not name. What the comparisons mean, what the stored values are and where the path through 0x22c0 continues are left open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** Role (from name): the high-water interrupt prehandler for a graphics flow. It loads the owner for index a0 from `gfx_flow_owners` (0xc). When the owner is nonzero (`bne` at 0x14, taken to 0x6c) it reads the owner's +0x28 and +0x30 fields (0x6c, 0x78), makes an indirect call at 0x74 with the +0x30 value as its first argument and a1 = 0, and returns. When the owner is zero it instead calls `cmn_err` twice at level 4 (0x24, 0x34), takes `gfx_flow_heart_lock` (`mutex_spinlock` at 0x3c) and releases it (`mutex_spinunlock` at 0x5c). The facts do not resolve the indirect call's target, so what the handler dispatches to is left open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_pipe_head_init` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0xd0
  **Inferred.** Role (from name): fills in the head of a flow pipe record. It reads the fields at +0x8, +0x10, +0x1c and +0x20 of its first argument (0xf8 to 0x11c), stores a word at +0x58 (0xdc) after reading the halfword at 0xffffffffffffc3a0 (0xd8), and stores into the `gfx_flow_owners` slot picked by that halfword (0xf0). Further stores at 0xffffffffaff00000-based computed addresses (0x100, 0x10c, 0x13c, 0x164) are not resolved by the facts. `gfx_flow_heart_lock` is taken at 0x138 and released at 0x160; the store at 0x13c is the instruction right after the lock call and a load at 0x150 sits between lock and unlock. What the stored fields mean is left open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x178
  **Inferred.** Role (from name): creates a flow record. It takes `gfx_flow_init_lock` (`mutex_spinlock` at 0x1ac) and releases it (`mutex_spinunlock` at 0x1d8). Two branches send it through setup first: when `gfx_flow_owners` is null (`beq` at 0x1b8) it goes to 0x308, which calls `gfx_flow_init`, and when a counter in .sbss becomes 1 (`beq` at 0x1cc) it goes to 0x320, which calls `heart_flow_control_connect` (0x348) with `gfx_flow_delayed_hiwater_intr_prehandler` as its second argument in a loop that tests a signed count against 2 (0x368). The main path calls `kmem_alloc` with size 0x68 (0x1e4); on a nonzero result it calls `xwidget_info_get` and `xwidget_info_id_get` (0x1fc, 0x208), allocates a second block with `kvalloc` (0x29c) and passes the `kmem_alloc` record to `gfx_flow_pipe_head_init` (0x2d8). It returns 0 or the `kmem_alloc` record (0x300). The 0x68 size matches the block that `gfx_flow_dealloc` frees at 0x444.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x384
  **Inferred.** Role (from name): tears down a flow record. It reads the halfword at +0x5c and the word at +0x58 of its argument. It calls `cmn_err` at 0x3a8 with level 4 only when the halfword is not -1 (`beq` at 0x398 skips it when equal). It then takes `gfx_flow_heart_lock` (0x3b4), releases it (0x3e4) and takes `gfx_flow_init_lock` (0x3ec). After a test of a count in .sbss minus 1 (`beq` at 0x408), the nonzero side releases `gfx_flow_init_lock` (0x41c) and calls `kmem_free` with size 0x68 (0x444). The side taken when that value is 0 runs a loop that calls `heart_flow_control_disable` (0x464) and `heart_flow_control_disconnect` (0x480) while a signed counter is below 2 (`bne` at 0x490). Where that loop exits is left open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x53c
  **Inferred.** Role (from name): attaches a flow to its owner table. It reads the halfword at 0xffffffffffffc3a0 to pick a slot and stores a doubleword into `gfx_flow_owners` at that slot (0x548, 0x560). It loads the doublewords at +0x8 and +0x10 of its argument (0x564, 0x56c), stores a doubleword at +0x60 (0x574), writes a halfword at +0x5c and a word at +0x58 (0x57c, 0x588), and takes `gfx_flow_heart_lock` (`mutex_spinlock` at 0x5a8, `mutex_spinunlock` at 0x5f4). After the unlock it calls `heart_flow_control_enable` with the value at +0x58 (0x600). How the loaded doublewords feed the stores, and the stores at computed addresses (0x598, 0x5ac), are not resolved by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.30/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x620
  **Inferred.** Role (from name): the detach side of `gfx_flow_attach_me`. It takes `gfx_flow_heart_lock` (`mutex_spinlock` at 0x630) and releases it at 0x674. It loads (0x65c) and stores (0x678) a doubleword at a computed base the facts do not name, rewrites a +0x60 doubleword, a +0x5c halfword and a +0x58 word (0x694 to 0x6c4), and calls `heart_flow_control_disable` with the value at +0x58 (0x67c). The facts do not show the stored values or the base address, so the state it leaves is left open.
  Reviewed: 2026-11-12 (sonnet)

- `giobr_attach` in `6.5.30/usr/cpu/sysgen/IP30boot/giobr.o` at 0x744
  **Inferred.** Role (from name): attaches a GIO bridge to the xtalk bus. It maps the bridge's register window with `xtalk_piotrans_addr` (0x780, size 0x1000000) and tests bit 0x20 of the word at +0xc of that mapping (0x790); when the bit is clear the branch continues at 0x7b0, and a return of -1 or 0 sits at 0x7a8 on the other side. It registers the bridge with `hwgraph_path_add` (0x7bc) and `hwgraph_char_device_add` (0x7dc), allocates and zeroes a 0x108-byte per-bridge record (`kmem_alloc` at 0x7ec, `bzero` at 0x7fc), and probes up to four slots through `giobr_slot_probe` (0x934, loop bound 4 at 0x96c). Only when the word at +0x44 of that record is nonzero (`beq` at 0x978) does it call `do_giobr_link_reset` (0x988), `gioio_provider_register` and `gioio_provider_startup` (0x994, 0x99c) and `gioio_device_register` and `gioio_device_attach` (0x9b8, 0x9c4); the zero side branches to 0xa10. Which slots end up with devices depends on the probe results, which the facts do not split out.
  Reviewed: 2026-11-12 (sonnet)

- `heart_mlreset` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x4c8
  **Inferred.** Role (from name): resets HEART chip state at boot. It calls `getcpuid` first (0x4d0). It then stores through three gp-relative pointers loaded at 0x4dc to 0x4e4: doublewords at 0x4e8, 0x4f4 (after a load at 0x4f0) and 0x528, and a word at 0x508. The facts do not resolve those addresses, so treating them as HEART registers rests on the routine's name. It also reads and writes a doubleword at 0xffffffffaff10040 (0x50c, 0x518), stores into the words at 0xffffffffaff10000 through 0xffffffffaff10018 (0x534 to 0x554) and returns the table pointer 0xffffffffaff10000 (0x564).
  Reviewed: 2026-11-12 (sonnet)

- `heart_startup` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x5dc
  **Inferred.** Brings the HEART up as an xtalk provider. It registers the interrupt table `heart_ivec` with `xtalk_provider_register` (0x624) and starts it with `xtalk_provider_startup` (0x62c). It adds hwgraph paths and edges (0x614, 0x640, 0x660, 0x67c), sets the device master (0x68c), and reads the switch with `xswitch_info_get` and `xswitch_info_link_ok` (0x6dc, 0x708). It registers widgets through `xwidget_register` (0x6cc, 0x784) in a loop controlled by a signed counter tested at 0x790, and builds a path name with `sprintf` (0x720). The loop's limit and how the link check gates each widget are left open.
  Reviewed: 2026-11-12 (sonnet)

- `heart_intr` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x1630
  **Inferred.** Role (from name): the HEART interrupt dispatcher. It reads the doubleword at 0xffffffffaff10038 (0x1654, 0x16a8), tests it against a1, and finds an interrupt number with `ffintr64` (0x16b8). When that number is 0x3b or higher (`beq` at 0x16d0) it goes to `buserror_intr` (0x171c). Otherwise it indexes the vector table `heart_ivec` at a 64-byte stride and uses the record's semaphore at +0x20 with `cvsema` (0x1690), `valusema` (0x16f8, reached only when bit 0x1000 of the record's +0x18 word is set) and `vsema` (0x170c). At 0x1730 it calls the record's handler indirectly, with the record's +0x8 value as the first argument and its +0x0 value as the third. An ll/sc pair at 0x1774 and 0x1780 updates a word at +0xd0 of a base the facts do not name; what that counter means is left open.
  Reviewed: 2026-11-12 (sonnet)

- `heart_rev` in `6.5.30/usr/cpu/sysgen/IP30boot/heart.o` at 0x1978
  **Inferred.** Role (from name): returns the HEART revision. It loads the global `the_heart_rev` and returns it when it is non-negative (`bltz` at 0x197c skips to 0x198c when it is negative). On the negative path it loads a word through a pointer (0x1990) and returns at 0x1998. The facts show that returned value as unresolved, so how it derives from the loaded word, and whether `the_heart_rev` is updated on that path, are left open.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.30/usr/cpu/sysgen/IP30boot/if_ef.o` at 0x738
  **Inferred.** Fetches the Ethernet address for an ioc3 interface. It loads the pointer at +0x398 of its argument (0x73c) and calls `nic_get_eaddr` with the buffers at that pointer's +0x30 and +0x34 (0x750). On a nonzero result it logs through `cmn_err` at level 1 (0x77c), using the word at +0x38c of its argument. It returns 0 or 5 (0x768), and which path gives which value is not split out in the disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.30/usr/cpu/sysgen/IP30boot/ioc3_pckm.o` at 0x5c
  **Inferred.** Role (from name): attaches the PS/2 keyboard and mouse ports of the ioc3 chip. It calls `ioc3_subdev_enabled` first (0x6c; the `beq` at 0x78 goes to 0x4e0 when it returns 0), traverses the hardware graph for the two port nodes (0x8c, 0xa0), allocates state with `kmem_zalloc` three times (0xf0, 0x110, 0x120) and initializes a mutex at +72 of an allocated block (`init_mutex` at 0x13c). Two port blocks are each skipped when their word (+0x28, +0x2c) equals 0x13. Where it is not skipped the first block sends 0xf5 through `pckm_pollcmd` (0x234) and, when `data_check` of the reply against 0xfa is nonzero, 0xf4 (0x260); the second sends 0xf6 (0x37c), each reply checked against 0xfa with `data_check` (0x240, 0x26c, 0x388). Those constants look like PS/2 command bytes. It registers devices with `device_inventory_add` (0x20c, 0x350) and connects `pckm_intr` through `ioc3_intr_connect` (0x450). The result of `ioc3_is_console` (call at 0xdc) and the `showconfig` flag select a console path (branches at 0x28c to 0x294, 0x2a0 to 0x2a8, 0x3cc to 0x3d4) that the disassembly only partly maps.
  Reviewed: 2026-11-12 (sonnet)

- `_get_timestamp` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x11a8
  **Inferred.** Returns a 32-bit counter value. It loads the unsigned word at 0xffffffffaff20004 (0x11b0) and returns it with no other effect. The routine's name suggests a timestamp source, and the address is the only place the value comes from in these disassembly.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x11b4
  **Inferred.** Role (from name): returns a 64-bit word that identifies the calling CPU. It loads the doubleword at 0xffffffffaff50000 (0x11bc) and returns it unchanged, without reading any of its arguments. `bootstrap` (0x1310, with a3 = 6) and `mlreset` (0x2348) call it.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x12b4
  **Inferred.** Role (from name): early boot entry that sets up a wired TLB entry and then jumps to the next stage. It reads the word at +0x10 of its first argument (0x12d4), calls `getcpuid` with a3 = 6 (0x1310), and calls `tlbwired` at 0x1370 with a0 = 0, a1 = 0, a2 = -0x4000, a3 = 1 and a5 = 0x6000 (a4 is not set in the facts). It stores a doubleword at 0xffffffffffffc4a8, a byte at 0xffffffffffffc04f and a word at 0xffffffffffffc65c (0x137c, 0x1380, 0x138c; the store at 0x138c is the delay slot of the jump), and then makes a direct tail jump to `cboot` (0x1388). The meaning of those globals and of the field at +0x10 is left open.
  Reviewed: 2026-11-12 (sonnet)

- `_ticksper1024inst` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1470
  **Inferred.** A timing calibration. It reads the doubleword at 0xffffffffaff20000 at 0x1480 and again at 0x1498, around two loops that branch while a0 and t1 are positive (0x1488, 0x1490). The name suggests ticks per 1024 instructions, but the disassembly does not show the arithmetic that turns the two reads into a count, so that part is open.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1940
  **Inferred.** Role unknown. Its only call is a relocated tail jump (`jr` at 0x1984) to `.text+6536`, with its four argument registers passed unchanged. 6536 is 0x1988, the address of the instruction in the delay slot of that same jump, so the target is not shown to be another routine. The facts reach no code from 0x198c onward, so what the jump does is left open.
  Reviewed: 2026-11-12 (sonnet)

- `silence_all_audio` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1b9c
  **Inferred.** Role (from name): silences the audio path and then halts through `ip30_rad_halt`. It loads the word at `is_octane_lx` (0x1bb4) and, when that word is zero, jumps straight to the halt (`beq` at 0x1bb8). When it is nonzero, it first writes a word to 0x900000001f600038 (0x1bd8) and then tail-jumps to `ip30_rad_halt` (`j` at 0x1bdc) with its arguments unchanged.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x22d0
  **Inferred.** Role (from name): machine-dependent reset on IP30. It sets the r1nkperf control and data registers for counters 0 and 1 on the straight path (0x22e4 to 0x2308), and reaches the code for counters 2 and 3 (0x2538 to 0x255c) only when a value derived from `get_cpu_irr` equals 0xe (`beq` at 0x2320) or 0xf (0x2338). When a0 is nonzero (`beq` at 0x2340 not taken) it calls `getcpuid` and `heart_mp_intr_init` (0x2348, 0x2350), `flush_tlb` and `flush_cache` (0x2358, 0x2360) and returns (0x2374). When a0 is zero it instead calls `getcpuid` (0x238c) and later `init_board_rev`, `ip30_chips_mlreset`, `init_sysid` and `load_nvram_tab` (0x23fc to 0x2414), then tests `arg_heart_invalidate_war` with `is_specified` and `atoi` (0x2484, 0x2494). It stores words at 0x24b0, 0x24b4 and 0x24bc, calls `flush_cache` (0x24c0), initializes `ip30clocklock` and `ip30ledlock` with `spinlock_init` (0x24d0, 0x24e0) and tests `cpu_enabled` in loops (0x24f0, 0x25b4). What the invalidate-workaround setting selects, and how the CPU loops end, are left open.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2690
  **Inferred.** Role (from name): updates the front-panel LED register pair under a lock. It takes `ip30ledlock` (`mutex_spinlock` at 0x26a0). A branch on a0 (`beq` at 0x26b4) chooses between two pairs of word stores, at 0x26bc and 0x26c0 when a0 is nonzero and at 0x26e4 and 0x26ec when it is zero. The `mutex_spinunlock` call at 0x26c8 has 0x900000001f600044 and 0x900000001f600040 in its argument registers, which the stores appear to use. It then calls `flushbus` (0x26d0) and returns its result (0x26dc). The facts do not show the values the stores write.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_disable` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2884
  **Inferred.** Role (from name): marks one CPU as disabled. It reads and writes the doubleword at 0xffffffffaff00000 (0x2894, 0x28a8) and stores a word into `processor_enabled` at the index in a0 (0x28b0). The facts list the stored values as unresolved, so the disabled marker is inferred from the routine's name and the table it writes.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_exists` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x28b4
  **Inferred.** Reports whether a CPU is present. It loads the doubleword at 0xffffffffaff00080 (0x28c0), the same word that `mlreset` loads at 0x23b4 and passes to `init_board_rev`. The disassembly leaves the return value unresolved, so what it reports is left open.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` at 0x38ac
  **Inferred.** Sends an interrupt by writing a doubleword to 0xffffffffaff10020 (0x38bc) and returns 0 (0x38c0). The address sits in the same 0xffffffffaff10000 window that `heart_mlreset` and `heart_intr` use, which suggests a HEART interrupt register. The value written is not in the disassembly.
  Reviewed: 2026-11-12 (sonnet)
