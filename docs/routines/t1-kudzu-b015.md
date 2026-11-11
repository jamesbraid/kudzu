# Routine role statements from the 6.5.22 IP30 kernel and boot objects

Each entry names a routine in a 6.5.22 install binary and gives what the instruction facts show: the calls, constants, loads and stores, and branches. The role lines are interpretations of those facts. Every entry is graded inferred. Where the facts do not settle a role, the line says so.

- `gfx_flow_detach_me` in `6.5.22/usr/cpu/sysgen/IP30boot/gfxs.a(gfx_flow.o)` at 0x620
  **Inferred.** It takes a pointer in a0 to a flow-control record. It takes the `gfx_flow_heart_lock` spinlock (call at 0x630), rewrites fields at offsets 0x1c, 0x58, 0x5c and 0x60, and calls `heart_flow_control_disable` (0x67c) with the value at offset 0x58. The name suggests a detach, and the field meanings come only from the stores; the role of the value at 0x58 is not established.

- `giobr_attach` in `6.5.22/usr/cpu/sysgen/IP30boot/giobr.o` at 0x744
  **Inferred.** It attaches a GIO bridge under the crosstalk framework. It maps the bridge through `xtalk_piotrans_addr` (0x780), allocates and zeroes a 0x108-byte record with `kmem_alloc` and `bzero`, and adds hardware-graph entries and widget info. It then loops while a counter is below four, probing slots with `giobr_slot_probe` (0x934) and attaching found devices with `gioio_device_register` and `gioio_device_attach`. The loop bound and the meaning of the 0x80 test at 0x75c are inferred, not confirmed.

- `heart_mlreset` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x4c8
  **Inferred.** It resets the HEART chip. It reads the CPU number from `getcpuid` (0x4d0), writes words at addresses in the 0x9000000018000000 block and in a table at 0xffffffffaff10000, and returns the word at 0xffffffffaff10000. The addresses look like HEART register space, but no register names are in the binary, and what each write does is unknown.

- `heart_startup` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x5dc
  **Inferred.** It starts HEART as a crosstalk provider. It registers the interrupt table `heart_ivec` with `xtalk_provider_register` (0x624), adds hardware-graph paths and an edge, and registers a widget with `xwidget_register` (0x6cc, 0x784). It reads switch information with `xswitch_info_get` and `xswitch_info_link_ok`, formats a name with `sprintf`, and sets the switch's vertex info. A flag read at `kdebug` (0x600) picks one branch, and what that branch does beyond its calls is unknown.

- `heart_intr` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x1630
  **Inferred.** It dispatches HEART interrupts. It reads a pending word at 0xffffffffaff10038, masks it, and takes the set-bit index from `ffintr64` (0x16b8). It uses the 64-byte entry of `heart_ivec` at that index, with semaphore-style calls (`cvsema`, `valusema`, `vsema`) around the dispatch. Index values of 0x3b and up go to `buserror_intr` (0x171c), and smaller ones call the handler pointer at entry offset 8 (`jalr` at 0x1730). The ll/sc counter update at 0x1774 to 0x1780 has an unknown meaning.

- `heart_rev` in `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` at 0x1958
  **Inferred.** It returns the HEART revision. When the global `the_heart_rev` is non-negative, it returns that global. When the global is negative, it reads the HEART register at 0x9000000018000004 (`lw` at 0x1970) and returns that instead. The return value on the register path is not established, and the caching design is inferred from the code shape.

- `ef_ioc3eaddr` in `6.5.22/usr/cpu/sysgen/IP30boot/if_ef.o` at 0x738
  **Inferred.** It fills in the Ethernet address for the IOC3 Ethernet interface. It passes two address fields at offsets 0x30 and 0x34 of a buffer at interface offset 0x398 to `nic_get_eaddr` (0x750). The error report through `cmn_err` (0x77c) uses a word at offset 0x38c, and the branch at 0x758 leads to it when `nic_get_eaddr` returns nonzero, which fits nonzero meaning failure. The return values are 0 and 5; the meaning of 5 is unknown.

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP30boot/ioc3_pckm.o` at 0x5c
  **Inferred.** It attaches the keyboard and mouse controller on the IOC3 I/O chip. It allocates a 0x68-byte device record and two 0x58-byte port records with `kmem_zalloc`, traverses the hardware graph, and enables a subdevice with `ioc3_subdev_enabled` (argument 3). It sends device commands 0xf5, 0xf4 and 0xf6 with `pckm_pollcmd`, which match the standard PS/2 keyboard and mouse codes, checks each 0xfa acknowledge with `data_check`, applies default settings, and connects `pckm_intr` through `ioc3_intr_connect`. Tests against 0x13 and the `showconfig` flag pick between paths; their exact roles are not established.

- `_get_timestamp` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xf28
  **Inferred.** It returns a 32-bit word from the fixed address 0xffffffffaff20004, read with `lwu`, and makes no calls. The word is probably a time or counter snapshot in the per-CPU block that the timer code also reads, but that identification is inferred.

- `getcpuid` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0xf34
  **Inferred.** It returns a 64-bit word from the fixed address 0xffffffffaff50000. Its callers in this object use the result as a CPU number, which the name suggests. The stored format is not established.

- `bootstrap` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x1034
  **Inferred.** It is an early boot entry. It reads a word at offset 0x10 of its first argument, gets the CPU number with `getcpuid` (0x1090), and wires a TLB entry with `tlbwired` (0x10f0) using constant arguments including 0x6000 and -0x4000. It writes a few words and a byte at low kernel addresses and then jumps to `cboot` at 0x1108. The TLB slot and the meaning of the stored words are not established.

- `_ticksper1024inst` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x11f0
  **Inferred.** It reads a word from 0xffffffffaff20000 twice and runs a short check on a0 and t1 through two signed branches (0x1208, 0x1210). The return value is not set on every path, so the result is unknown. The name suggests a timing calibration constant, but no constant is produced in the facts.

- `start` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x16c0
  **Inferred.** It is an entry stub that forwards its four register arguments to a relocated target through a register jump at 0x1704. The instructions from 0x170c to 0x1870 are not reachable from the entry, so the real behavior lies in the target, which this reading does not cover. Treating this as the object's program entry is inferred.

- `silence_all_audio` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x191c
  **Inferred.** The name says it silences audio on the IP30 board, but the facts show only a test of the `is_octane_lx` flag, one word store to 0x900000001f600038 when that flag is nonzero, and a tail jump to `ip30_rad_halt` (0x195c). The value stored is not shown. That the store silences audio is inferred from the name.

- `mlreset` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2050
  **Inferred.** It is a per-CPU machine reset path for the IP30 board. It sets performance-counter registers through `r1nkperf_control_register_set` and `r1nkperf_data_register_set`, reads interrupt state with `get_cpu_irr`, and calls `heart_mp_intr_init`, `init_board_rev`, `ip30_chips_mlreset`, `init_sysid` and `load_nvram_tab`. It parses the `arg_heart_invalidate_war` argument with `is_specified` and `atoi`, initializes the `ip30clocklock` and `ip30ledlock` spinlocks, and writes three preference variables used by `bzero` and `bcopy`. The call order is read from the code; the purpose of the invalidate flag is inferred from its name.

- `set_leds` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2410
  **Inferred.** It sets the front-panel LED word pair. It takes the `ip30ledlock` spinlock and stores words at 0x900000001f600040 and 0x900000001f600044, then calls `flushbus` (0x2450). The test on a0 at 0x2434 selects between two paths that store to the same two addresses; the stored values are not shown in the facts.

- `cpu_disable` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2604
  **Inferred.** It marks one CPU as disabled. It reads and writes back a doubleword at 0xffffffffaff00000 and stores a word into the `processor_enabled` table at index a0. The stored value is not visible in the facts, and the purpose of the read-modify-write at 0xffffffffaff00000 is unknown.

- `cpu_exists` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x2634
  **Inferred.** It reads a doubleword from 0xffffffffaff00080 and returns it. The return value is not established. Given the name, the word is probably a CPU presence or mask, but the facts show no comparison or mask that would confirm it.

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x362c
  **Inferred.** It stores a doubleword to 0xffffffffaff10020 and returns zero. The stored value is not in the facts. `debug_stop_all_cpus` writes the same word, so the two probably signal other CPUs; that is inferred from the shared address.

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3648
  **Inferred.** It stops the other CPUs for debugging. It loops over CPU numbers up to the `maxcpus` global and compares each against a 16-bit global at -0x3c60, which is likely the current CPU number. For each CPU it writes a doubleword to 0xffffffffaff10020. The 16-bit word read three times from 0xffffffffffffc3a0 has an unknown role, and the stop mechanism is inferred from the paired write in `sendintr`.

- `stop_bell` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3e88
  **Inferred.** It is a timeout callback that turns the keyboard bell off. It raises the interrupt level with `splhi`, stores a word to a small-data variable at `.sdata+64` (the value is not in the facts), and restores the level with `splx`. The name pairs it with `pckbd_bell`, which arms it.

- `pckbd_bell` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3ebc
  **Inferred.** It starts the keyboard bell. It raises the interrupt level, stores a word to a small-data variable at `.sdata+72`, and arms `stop_bell` with `timeout` (0x3f34), passing the constant 0x100000 and the stored word. A branch at 0x3ec8 tests a2, the third argument, for a value at or below zero and jumps to the return at 0x3efc without arming the timeout. A small-data word at `.sbss+48` decides whether the bell is already on. The units of the timeout are not established.

- `counter_intr` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x3f70
  **Inferred.** It is the per-CPU counter (timer) interrupt handler. It reads the counter and compare state, takes the performance-counter overflow path (`hwperf_overflowed_intr`, `hwperf_intr`) and the `frs_handle_counter_intr` path, and rearms the timer with `_set_timer_intr` and `resetcounter`. It keeps latency and timestamp records with `resetlatency` and `log_tstamp_event`, and compares elapsed counts with `lbolt`-based limits. A `cmn_err` call at 0x45e0 may report an overrun against the `counter_intr_warn_cycles` global; the exact trigger is inferred.

- `doadjtime` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x592c
  **Inferred.** It applies a pending time adjustment in steps. It takes the `timelock` spinlock with `nested_spinlock` (0x598c, 0x5aa8), reads the time with `__nanotime` (0x597c, 0x5a9c), and loops while the adjustment is still nonzero. The step bounds, 1,000,000 (0xf4240) and 1000 (0x3e8), come from the code, but the adjustment's sign and arithmetic are not established.

- `__nanotime` in `6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` at 0x5b20
  **Inferred.** It returns a nanosecond time value. It takes `timelock` with `mutex_spinlock_spl` under `splprof`, reads per-CPU words at 0xffffffffaff20000 and through `pdaindr`, and combines them with the `adjtime_delta`, `adjtime_RTC` and `adjtime_nanos` globals. It normalizes with 1,000,000,000 (0x3b9aca00) and releases the lock on both exits (0x5cbc, 0x5d34). Two table reads at constant addresses are not identified, so the source of the raw counter is not established.
