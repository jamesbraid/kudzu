# Routine role statements from the 6.5.22 IP30 kernel and boot objects

Each entry names a routine in a 6.5.22 install binary and gives what the instruction facts show: the calls, constants, loads and stores, and branches. The role lines are interpretations of those facts. Every entry is graded inferred. Where the facts do not settle a role, the line says so.

- `gfx_flow_detach_me` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x620
  **Inferred.** It takes a pointer in a0 to a flow-control record. It takes the `gfx_flow_heart_lock` spinlock (call at 0x630), rewrites fields at offsets 0x1c, 0x58, 0x5c and 0x60, and calls `heart_flow_control_disable` (0x67c) with the value at offset 0x58. The name suggests a detach, and the field meanings come only from the stores; the role of the value at 0x58 is not established.
  Reviewed: 2026-11-12 (sonnet)

- `giobr_attach` in `6.5.22/usr/cpu/sysgen/IP30boot/giobr.o` at 0x744
  **Inferred.** Role (from name): it attaches a GIO bridge under the crosstalk framework. It maps the bridge through `xtalk_piotrans_addr` (0x780), allocates and zeroes a 0x108-byte record with `kmem_alloc` and `bzero`, and adds hardware-graph entries and widget info. It then loops while a counter is below four, calling `xtalk_piotrans_addr` and `giobr_slot_probe` (0x934) once per pass. After the loop, only when the word at record offset 0x44 is nonzero, it calls `gioio_device_register` (0x9b8) and `gioio_device_attach` (0x9c4); those two calls are not inside the loop. The loop bound and the meaning of the 0x80 test at 0x75c are inferred, not confirmed.
  Reviewed: 2026-11-12 (sonnet)

- `heart_mlreset` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x4c8
  **Inferred.** Role (from name): it resets the HEART chip. It calls `getcpuid` (0x4d0), then stores doublewords and one word and loads two doublewords through bases the facts do not resolve. The facts do not show those addresses or the stored values, and what each write does is unknown. It does not return a loaded word: the value it returns is an address computed from 0xffffffffaff10000 (the facts list it as a table entry), and the last store at 0x568 runs in the return's delay slot.
  Reviewed: 2026-11-12 (sonnet)

- `heart_startup` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x5dc
  **Inferred.** Role (from name): it starts HEART as a crosstalk provider. It registers the interrupt table `heart_ivec` with `xtalk_provider_register` (0x624), adds hardware-graph paths and an edge, and registers a widget with `xwidget_register` (0x6cc, 0x784). It calls `xswitch_info_get`, and when that returns nonzero and `xswitch_info_link_ok` returns nonzero it formats a name with `sprintf` and sets the switch's vertex info, in a loop whose bound is not established. A halfword read at `kdebug` is tested at 0x600; both outcomes reach the same calls, and the facts show only a store of a different word on each side (0x608 and 0x7c0), so what the flag selects is not established.
  Reviewed: 2026-11-12 (sonnet)

- `heart_intr` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x1630
  **Inferred.** Role (from name): it dispatches HEART interrupts. It reads a pending word at 0xffffffffaff10038, masks it with a1, and takes the set-bit index from `ffintr64` (0x16b8), in a loop that re-reads the pending word. It uses the 64-byte entry of `heart_ivec` at that index. An index of 0x3b or more calls `buserror_intr` (0x171c). A smaller index calls through a register at 0x1730 (`jalr`) with a0 = the doubleword at entry offset 8 and a2 = the doubleword at entry offset 0, but only when bit 0x1000 of the word at entry offset 0x18 is clear. When that bit is set it makes no such call and instead calls `valusema` (0x16f8) on the semaphore address at entry offset 0x20, then `cvsema` (0x1690) when the result is 0x7001 or more and `vsema` (0x170c) when it is below. The ll/sc counter update at 0x1774 to 0x1780 has an unknown meaning.
  Reviewed: 2026-11-12 (sonnet)

- `heart_rev` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x1958
  **Inferred.** Role (from name): it returns the HEART revision. When the global `the_heart_rev` is non-negative, it returns that global. When the global is negative, it makes the `lw` at 0x1970 through a pointer taken from the literal pool and returns a value the facts leave unresolved. How that value relates to the loaded word, and whether the global is a cache of it, is inferred from the code shape and not established.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/if_ef.o` at 0x738
  **Inferred.** It fills in the Ethernet address for the IOC3 Ethernet interface. It passes two address fields at offsets 0x30 and 0x34 of a buffer at interface offset 0x398 to `nic_get_eaddr` (0x750). The error report through `cmn_err` (0x77c) uses a word at offset 0x38c, and the branch at 0x758 leads to it when `nic_get_eaddr` returns nonzero, which fits nonzero meaning failure. The return values are 0 and 5; the meaning of 5 is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP30boot/ioc3_pckm.o` at 0x5c
  **Inferred.** Role (from name): it attaches the keyboard and mouse controller on the IOC3 I/O chip. It starts by calling `ioc3_subdev_enabled` (0x6c) with a1 = 3 and branches to 0x4e0 when it returns zero, so the rest is the enabled path. It allocates a 0x68-byte device record and two 0x58-byte port records with `kmem_zalloc` and traverses the hardware graph. Conditionally, it sends 0xf5, 0xf4 and 0xf6 with `pckm_pollcmd` (0xf5 and 0xf4 at 0x234 and 0x260, 0xf6 at 0x37c), checks each reply against 0xfa with `data_check`, and calls `default_kbd_setting` or `default_ms_setting` when the check returns nonzero. It connects `pckm_intr` through `ioc3_intr_connect` (0x450). Tests of the record words at offsets 0x28 and 0x2c against 0x13 and of the `showconfig` flag choose between paths; their exact roles are not established.
  Reviewed: 2026-11-12 (sonnet)

- `_get_timestamp` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xf28
  **Inferred.** It returns a 32-bit word from the fixed address 0xffffffffaff20004, read with `lwu`, and makes no calls. What the word is cannot be told from the facts; the name suggests a timestamp. `_ticksper1024inst` reads the doubleword at 0xffffffffaff20000, but that is the only link between them.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xf34
  **Inferred.** It returns a 64-bit word from the fixed address 0xffffffffaff50000. The name suggests the word is a CPU number; the facts do not show how callers use it, and the stored format is not established.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1034
  **Inferred.** Role (from name): it is an early boot entry. It reads a word at offset 0x10 of its first argument, calls `getcpuid` (0x1090) and calls `tlbwired` (0x10f0) with a0 = 0, a1 = 0, a2 = -0x4000, a3 = 1 and a5 = 0x6000. It then stores a doubleword (0x10fc) and a byte (0x1100) through bases the facts do not resolve, loads a doubleword (0x1104), and tail-jumps to `cboot` at 0x1108, with a word store in the jump's delay slot (0x110c). What the TLB call and the stored values mean is not established.
  Reviewed: 2026-11-12 (sonnet)

- `_ticksper1024inst` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x11f0
  **Inferred.** It reads the doubleword at 0xffffffffaff20000 at 0x1200 and again at 0x1218. Between the reads, two branches on signed greater-than-zero tests (0x1208 branches to itself, 0x1210 back to 0x1204) form countdown loops on t1 and a0. The facts leave the return value unresolved. The name suggests a timing calibration constant, but no constant is produced in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x16c0
  **Inferred.** It is an entry stub whose only call in the facts is a tail jump at 0x1704 through a register to a relocated target (`.text+5896`, which is 0x1708, the address right after the jump), passing a0 to a3 unchanged. The code from 0x170c on is therefore the jump target's continuation, not unreachable code, and this reading does not cover it. Treating this as the object's program entry is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x191c
  **Inferred.** The name says it silences audio on the IP30 board, but the facts show only a test of the `is_octane_lx` flag, one word store to 0x900000001f600038 when that flag is nonzero, and a tail jump to `ip30_rad_halt` (0x195c). The value stored is not shown. That the store silences audio is inferred from the name.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2050
  **Inferred.** Role (from name): a per-CPU machine reset path for the IP30 board. It calls `r1nkperf_control_register_set` and `r1nkperf_data_register_set` with a0 = 0 and 1 at entry (0x2064 to 0x2088) and with a0 = 2 and 3 (0x22b8 to 0x22dc), the latter only on the paths where a `get_cpu_irr` result (0x2090, 0x20a8) equals 0xe or 0xf (branches at 0x20a0 and 0x20b8). When s0 is nonzero (0x20c0) it calls `getcpuid`, `heart_mp_intr_init`, `flush_tlb` and `flush_cache` and returns; otherwise it goes on to call `init_board_rev`, `ip30_chips_mlreset`, `init_sysid` and `load_nvram_tab` (0x217c to 0x2194). It calls `is_specified` on `arg_heart_invalidate_war` (0x2204) and calls `atoi` on it only when that returns nonzero. It initializes the `ip30clocklock` and `ip30ledlock` spinlocks (0x2250, 0x2260). On a separate path (0x2390, 0x23a0) it calls `bzero` with a0 = the symbols `bcopy` and `bzero`; the purpose of that is not established. The invalidate flag's purpose is inferred from its name.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2410
  **Inferred.** Role (from name): it sets an LED word pair. It calls `mutex_spinlock` on `ip30ledlock` (0x2420), stores two words on one of two paths chosen by the test of a0 at 0x2434 (0x243c and 0x2440 when a0 is nonzero; 0x2464 and 0x246c when it is zero), releases the lock with `mutex_spinunlock` (0x2448), and calls `flushbus` (0x2450). The facts show arguments 0x900000001f600040 and 0x900000001f600044 on the unlock call, which suggests the store addresses, but the stored values are not shown and the facts do not show where the zero path rejoins.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_disable` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2604
  **Inferred.** Role (from name): it marks one CPU as disabled. It loads a doubleword from 0xffffffffaff00000 (0x2614) and stores a doubleword back with `sd` at 0x2628, and stores a word at 0x2630, in the return's delay slot. The target of that word store is not in the facts, nor is the stored value, and the purpose of the doubleword update is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_exists` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2634
  **Inferred.** It loads a doubleword from 0xffffffffaff00080 (0x2640) and returns a value the facts leave unresolved; the facts do not show whether the return is the loaded word itself or a value derived from it. The name suggests a CPU presence test, but the facts show no comparison that would confirm it.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x362c
  **Inferred.** It stores a doubleword to 0xffffffffaff10020 and returns zero. The stored value is not in the facts. `debug_stop_all_cpus` writes the same word, so the two probably signal other CPUs; that is inferred from the shared address.
  Reviewed: 2026-11-12 (sonnet)

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3648
  **Inferred.** Role (from name): it stops the other CPUs for debugging. It loops over CPU numbers below the `maxcpus` global and compares each with the halfword at address 0xffffffffffffc3a0 (-0x3c60), read at 0x3680, 0x36a8 and 0x36d0. When the two are not equal it stores a doubleword to 0xffffffffaff10020 (0x368c, 0x36b4); equal numbers skip the store. What the halfword holds is not established; it may be the current CPU number. That `sendintr` stores to the same address suggests both signal other CPUs; the stop mechanism is inferred from that shared address.
  Reviewed: 2026-11-12 (sonnet)

- `stop_bell` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3e88
  **Inferred.** Role (from name): a timeout callback that turns the keyboard bell off. It calls `splhi`, stores a word at 0x3eac, in the delay slot of the `splx` call (0x3ea8) that restores the level, through a base the facts do not resolve. The stored value is not in the facts. `pckbd_bell` passes `stop_bell` to `timeout`, which links the two.
  Reviewed: 2026-11-12 (sonnet)

- `pckbd_bell` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3ebc
  **Inferred.** Role (from name): it starts the keyboard bell. When a2, the third argument, is above zero (a branch at 0x3ec8 skips everything otherwise) it calls `splhi`, then tests the word at `.sbss+48` (0x3ee4). When that word is nonzero it calls `splx` and returns 1 without arming the timeout. When it is zero it stores a word (0x3f30) and calls `timeout` (0x3f34) with a0 = `stop_bell`, a1 = 0, a3 = 0x100000 and a4 = the doubleword at `.sdata+72`. That the `.sbss+48` word means the bell is already on is inferred. The units of the timeout are not established.
  Reviewed: 2026-11-12 (sonnet)

- `counter_intr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3f70
  **Inferred.** Role (from name): the per-CPU counter (timer) interrupt handler. It calls `hwperf_overflowed_intr` (0x3fc4) and calls `hwperf_intr` (0x3fd4) only when that returns nonzero. When the 0x20 bit of a doubleword it tests (0x3f94) is clear it calls `frs_handle_counter_intr` (0x3f9c); when set it branches to other code. It calls `resetcounter` (0x3ffc, 0x44dc) and `_set_timer_intr` (0x404c), calls `resetlatency` (0x40f8, 0x430c), `log_tstamp_event` (four calls from 0x4500) and `_get_count` and `_get_compare` (0x4434, 0x443c), and compares `lbolt` with 0x3e9 and 0xb5 and the count read at 0xffffffffaff20000 with stored limits. A `cmn_err` call at 0x45e0 follows two signed compares (0x45ac, 0x45bc); what it reports and its relation to `counter_intr_warn_cycles` (compared at 0x4144, used at 0x4434 and 0x4484) are not established.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x592c
  **Inferred.** Role (from name): it applies a pending time adjustment. It calls `splprof` (0x5974, 0x5a94), `__nanotime` (0x597c, 0x5a9c) and `nested_spinlock` on `timelock` (0x598c, 0x5aa8), and releases `timelock` with `mutex_spinunlock` (0x5a34, 0x5b08). Branches test a0 for zero and for sign at entry (0x5948, 0x5954), and the constants 1,000,000 (0xf4240) and 1000 (0x3e8) appear as compare bounds and as the 0x3e8 argument of the second unlock. A loop back to 0x5974 exists (0x5a74), but its condition is on an unresolved value, so that the adjustment is applied in steps while it is nonzero is not established, and neither is its arithmetic.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x5b20
  **Inferred.** Role (from name): it returns a nanosecond time value. It calls `mutex_spinlock_spl` on `timelock` with `splprof` as the argument at 0x5b2c, reads the doubleword at 0xffffffffaff20000, and combines it with the `adjtime_delta`, `adjtime_RTC` and `adjtime_nanos` globals. It compares values against 1,000,000,000 (0x3b9aca00). It releases the lock with `mutex_spinunlock` on the two exit paths (0x5cbc, 0x5d34) and returns that call's result. Two table reads at constant addresses are not identified, so the source of the raw counter is not established.
  Reviewed: 2026-11-12 (sonnet)
