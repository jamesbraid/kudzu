# Routine role statements for 25 kudzu routines

These statements describe software routines in the identified binaries, taken from their instruction facts (registers, calls, constants and branches). They do not describe hardware behavior. Where the facts do not settle a role, the statement says so and leaves it open.

Every entry below is graded inferred. The facts name each routine from the binary's symbol table or the inventory, and none of the names is checked against a second source.

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP35boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** `_mraccess` calls `wchanmeter` at 0x38b0 and then `mr_access` at 0x38c0; when `wchanmeter` returned nonzero it first calls `mrmeter_entry` at 0x3994. The branch at 0x38c8 returns after `mr_access` when the `wchanmeter` result is zero. Only when that result is nonzero does it update fields of the returned record at offsets 0x198 to 0x1d0, call `mrmeter_getentry` at 0x392c and branch on its result at 0x3934. The load-linked and store-conditional pair at 0x3940 and 0x394c, retried by the branch at 0x3950, runs only when the `mrmeter_getentry` result is nonzero. The name suggests a metered access path for a sync object, but the facts do not say what is counted or whether the pair is a lock.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP35boot/ksync_metered.a(sema.o)` at 0x39ec
  **Inferred.** `_mrupdate` has a similar outline to `_mraccess`. It calls `wchanmeter` at 0x3a0c and then `mr_update` at 0x3a1c; when `wchanmeter` returned nonzero it first calls `mrmeter_entry` at 0x3ac4. The branch at 0x3a2c returns after `mr_update` when the `wchanmeter` result is zero. Otherwise it updates the returned record at offsets 0x1a0 to 0x1d8, and a bit test on a7 at 0x3a90 decides whether the stores at 0x3a98 to 0x3aa4 run. From its name it reads as the update half of the pair, but what it records is not clear from the facts.
  Reviewed: 2026-11-12 (sonnet)

- `get_myid` in `6.5.22/usr/cpu/sysgen/IP35boot/l1.o` at 0xc3c
  **Inferred.** `get_myid` is a 16-byte leaf. It loads a doubleword from the fixed address 0x9200000001000020 at 0xc40 and returns it in v0 after a shift by zero bits at the return at 0xc44, so the facts do not show the full doubleword being returned unchanged. The name suggests a processor or node identifier, which fits a value read from a fixed location, but the facts do not give the meaning of the stored value.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_phase_bits` in `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` at 0x68
  **Inferred.** `nic_get_phase_bits` is a 52-byte routine. It loads a doubleword from 0x9000000018000078 at 0x70 and returns a value the facts do not resolve. The name suggests it reads phase bits from a NIC register, but the single load is the only evidence for that, and the address space is not identified.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyAttach` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_attach.o)` at 0x68
  **Inferred.** `gf_OdsyAttach` calls `gfxdd_recycle` at 0xfc and, on the paths that continue past its result test at 0x104, calls `gfxdd_mmap` at 0x18c with `gfx_fault` as its fourth argument (a3). When `gfxdd_mmap` returns zero it calls `splhi` at 0x19c, `gf_OdsyUnMapGfx` at 0x1b4 and `splx` at 0x1bc. When it returns nonzero it logs through `cmn_err` at 0x1e0. Its return values are 0x0, 0xc and 0x16, which look like errno-style codes, but the facts do not name them.
  Reviewed: 2026-11-12 (sonnet)

- `gfP_OdsySaveHwShadow` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_ctxsw.o)` at 0xc0
  **Inferred.** `gfP_OdsySaveHwShadow` is a 1180-byte routine that is mostly flow control. It arms a timeout on `odsy_ctxsw_timeout` at 0x18c, waits on it with `untimeout_wait` at 0x1a0, and makes an indirect call at 0x20c with several register arguments taken from its context record. It detaches and reattaches the gfx flow with `gfx_flow_detach_me` and `gfx_flow_attach_me`, and waits for returned credits through `odsy_flow_await_returned_credits` with the thread pinned by `rt_pin_thread`. The name suggests a save of hardware shadow state on context switch, but the facts show far more credit and flow handling than saving, so what gets saved is open.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyDestroyDDRN` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_ddrn.o)` at 0x430
  **Inferred.** `gf_OdsyDestroyDDRN` is a 724-byte teardown routine. On the side of the branch at 0x468 that does not go to `gfx_flow_clean_up_other_gfxp` at 0x5e0, it calls `gf_OdsyUnSchedRetraceEvent` at 0x4a4 and, only when the word at a2+28 is nonnegative, `gf_OdsyUnSchedSwapBuf` at 0x498. It then calls `odsy_hpipe_cleanup` at 0x4bc, frees a 0x4000-byte block with `kmem_free` at 0x4d8 only when the doubleword at offset 192 of the pointer at a2+112 is nonzero, and releases a 32 KB slot through `OdsyFree32kSlot` at 0x4e4. When the word at offset 408 of that same pointer is zero, it first calls `odsyDMAFinish`, `odsyMMdetach` and `odsyUnpinAndUnmapGctxRegions` at 0x624 to 0x638 and sets that word to 1; a nonzero word skips them. It calls `gfx_flow_dealloc` at 0x58c only when the word at offset 26424 of a0 is 1 and the pointer at offset 808 of a0 is nonzero. The name suggests destruction of a device descriptor, but the facts do not expand "DDRN", so the object being destroyed is open.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyInitialize` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_init.o)` at 0x15a8
  **Inferred.** `gf_OdsyInitialize` is a 1656-byte routine. It registers a board with `htp_register_board` at 0x161c, and it reads the nasid and chimera type through `get_nasid` and `get_chimera_type` at 0x17e8 to 0x180c. It allocates two gfx flows with `gfx_flow_alloc` at 0x1848 and 0x189c, sets up memory, DMA, retrace and interrupts through `odsyInitMM`, `odsyInitDMA`, `odsyInitRetrace` and `odsyInitInterrupts`, and calls `odsyReset` at 0x17cc. The branch at 0x178c can reach the test at 0x17c0 without passing `odsy_usdelay` at 0x17a0, so `odsyReset` does not always follow that delay. The calls fit a bring-up sequence, but the facts do not show that the others execute in the order they are placed.
  Reviewed: 2026-11-12 (sonnet)

- `odsyLowLevelReset` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_init.o)` at 0x2014
  **Inferred.** `odsyLowLevelReset` is an 876-byte routine. It reads and writes several fixed addresses in the 0x92000000e0000000 range, compares one loaded value with 0xd100 at 0x2070, and passes the constants 0xf4240 and 0x186a0 to `odsy_usdelay` as delay arguments. It reports through `cmn_err` at 0x2358. The read-modify-write pattern and the delays fit a reset, but the register meanings and the reason for the 0xd100 compare are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP35boot/os.a(pda.o)` at 0x1b70
  **Inferred.** `doacvec` is a 116-byte routine. It reads a byte from the sign-extended low address 0xffffffffffffc080. It calls `panicspin` at 0x1b94 when that byte is nonzero, or when bit 0 of the doubleword at 0x92000000017a0038, which it reads only when the byte is zero, is set. It then reads a byte at 0xffffffffffffc081 and only when that byte is nonzero writes bytes to 0xffffffffffffc04e and 0xffffffffffffc081. It calls `tlbflush_rand` at 0x1bbc and `tlbflush` at 0x1bd0 only when the words they take from low addresses are nonzero, before returning. The name does not settle what the vector is, but the TLB flush callees place the routine near TLB maintenance.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP35boot/os.a(syssgi.o)` at 0x140
  **Inferred.** `syssgi` is a 16016-byte routine and the largest one in this batch. The facts show it reaching many subsystems: capability checks through `cap_able` at 0x228, 0x664, 0x708 and 0x8c0, XFS and XLV calls (`xfs_itable` at 0xa90, `xlv_attr_get` at 0x44c), asynchronous I/O (`kaio_rw` at 0x4b0, `kaio_suspend` at 0xb84), and signal-stack and process-group calls. It has 14 calls through a register whose callee the facts do not resolve (for example at 0xa3c and 0x15ec), and one instruction at 0x2584 that no path from the entry reaches. The first branches compare the word at a0 with 0xa6, 0xa7 and 0x160 (0x184 to 0x1a4), which fits selector dispatch, and the name suggests a multiplexed system-call entry, but the facts do not resolve the dispatch beyond that, so this statement covers only the calls it makes.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP35boot/os.a(time.o)` at 0xc0
  **Inferred.** `dogetitimer` is a 400-byte routine. It branches on a0 at 0xdc. When a0 is zero it calls `splprof` at 0xe4, takes a lock reached through a2+768 with `mutex_lock` at 0xf8, stores doublewords at offsets 0x0 to 0x18 of its second argument and releases the lock with `mutex_unlock` at 0x120. It then calls `chktimeout_tick` at 0x15c with a0 = 0 and, on another path, at 0x218 with a0 = 2, and converts a tick count with `tick_to_timeval` at 0x17c. When a0 is nonzero it takes a `splhi` path at 0x198 that stores doublewords at the same offsets and calls `splx`, without the lock. The name suggests the body of a get-interval-timer call, which fits the tick-to-timeval conversion, but the facts do not say which timer the two tick reads come from.
  Reviewed: 2026-11-12 (sonnet)

- `part_discovery` in `6.5.22/usr/cpu/sysgen/IP35boot/partition.o` at 0x1f6c
  **Inferred.** `part_discovery` is a 724-byte routine. It takes a semaphore with `cpsema` at 0x1f9c and 0x1fac, calls `get_nasid` at 0x20a8, and probes partitions per nasid with `part_probe_nasid` at 0x20b0, 0x20f8 and 0x2220. It registers `part_activate` as an interrupt target through `part_interrupt_set` at 0x219c, starts a partition thread at 0x21ac, and reads a vector with `vector_read` at 0x21f0. Its branches compare s0 with 0x80 at 0x20dc and s1 with 0x40 at 0x2134, but the facts do not show these as loop counts, and they do not say whether the bounds are node limits.
  Reviewed: 2026-11-12 (sonnet)

- `pic_attach2` in `6.5.22/usr/cpu/sysgen/IP35boot/pic.o` at 0x5e4
  **Inferred.** `pic_attach2` is a 3176-byte attach routine. It registers the device in the hardware graph with `hwgraph_char_device_add` at 0x650 and `hwgraph_info_add_LBL` at 0x680, and it allocates a 0xac0-byte per-device block with `kmem_zalloc` at 0x740. It allocates a second, 0x18-byte record with `kmem_zalloc` at 0x750 and links that record into `pcibr_list` with `swap_ptr` at 0x770. It reads mode and speed through `pcireg_mode_get` and `pcireg_speed_get`, and looks up L1 and module records through `get_elsc`, `iobrick_l1type_get_nasid` and `iobrick_module_get_nasid`. It also initializes a spinlock and an mrlock inside the block. The symbol name suggests a PCI interface chip, but the facts do not identify the chip, so that is open.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.22/usr/cpu/sysgen/IP35boot/xbridge.o` at 0x54
  **Inferred.** `xbridge_attach` is a 3484-byte attach routine with the same call pattern as `pic_attach2`. It gets the bridge pointer with `pcibr_bridge_ptr_get` at 0x84, registers a PCI I/O provider with `pciio_provider_register` at 0xf4 and starts it at 0xfc, and adds a character device through `hwgraph_char_device_add` at 0x120. It allocates a 0xac0-byte block and an 0x18-byte record, links the record into `pcibr_list` with `swap_ptr` at 0x35c, and reads NIC address and mode fields through `pcireg_nic_addr`, `pcireg_mode_get` and `pcireg_speed_get`. The calls fit a bridge provider attach, but the facts do not prove which bridge the symbol refers to.
  Reviewed: 2026-11-12 (sonnet)

- `sn00_vmc` in `6.5.30/unix.IP27` at 0xc000000000025998
  **Inferred.** `sn00_vmc` is a 200-byte routine. It reads a halfword at the sign-extended address 0xffffffffffffc422 and a doubleword at 0xffffffffffffc400, then indexes a table reached through the pointer at -0x3c00 with the halfword at -0x3bde. It calls `device_inventory_add` at 0xc0000000000259d0 with the values 0xb and 0x3, then the bridge hint calls `pcibr_hints_dualslot`, `pcibr_hints_subdevs` and `pcibr_hints_fix_rrbs`, and registers `pcibr_set_rrb_callback` at 0xc000000000025a04. Between its `spl65` and `splx` calls it makes doubleword stores through a pointer the facts show as 0x9200000001220050 at the `splx` call: two on each side of the branch at 0xc000000000025a2c, so two execute on any one path. The prefix "sn00" points to the SN0 platform, but the facts do not say what "vmc" stands for.
  Reviewed: 2026-11-12 (sonnet)

- `restart` in `6.5.30/unix.IP27` at 0xc000000000029814
  **Inferred.** `restart` is a 356-byte routine. It calls `us_delay` with 0xf4240 at 0xc0000000000298a0, then branches at 0xc0000000000298b4 on a word at 0xc0000000014cb024. When that word equals 2 it calls `get_nasid`, then `elsc_init` and `elsc_system_reset` with a stack record, and returns unless the `elsc_system_reset` result is negative; only on a negative result does it call `printf` at 0xc00000000002994c and `us_delay` with 0xf4240, then store a doubleword to 0x9200000001600008 at 0xc000000000029968, the same register `ni_reset_port` writes. When the word is not 2 it enters a loop: it calls `sysctlr_power_off` at 0xc0000000000298cc with a0 zero when the word at 0xc0000000014d4350 is nonzero, or `ni_reset_port` at 0xc000000000029904 when it is zero, followed by `us_delay` with 0x2dc6c0 at 0xc0000000000298d4, and the facts list no return from that loop. The name suggests the system restart path, and the power-off and reset calls fit that, but the facts do not say what the printed text reports.
  Reviewed: 2026-11-12 (sonnet)

- `page_local_calias` in `6.5.30/unix.IP27` at 0xc00000000002b108
  **Inferred.** `page_local_calias` is a 60-byte routine. It loads a doubleword from 0x9200000001000028, branches on that value at 0xc00000000002b11c, and compares a3 with v1 as unsigned values at 0xc00000000002b130. The compare result selects a return of 0 or 1. The name suggests a check for a local alias on a page, which fits a flag test followed by a range compare, but the facts do not show what v1 holds, so the meaning of the compare is open.
  Reviewed: 2026-11-12 (sonnet)

- `bte_init` in `6.5.30/unix.IP27` at 0xc00000000002bf5c
  **Inferred.** `bte_init` is a 232-byte routine. It fills fields of its argument block at offsets 0x0 to 0x70, including a copy of the halfword at abs 0xffffffffffffc422 and a halfword from the table at 0xc0000000014d4e80. It writes a doubleword to abs 0x92000000014001f8, at 0xc00000000002c014 or 0xc00000000002c038 depending on the branch at 0xc00000000002c004 on a2, and calls `spinlock_init` at 0xc00000000002bf98 on the block at offset 0x44. It returns its first argument. The name suggests block transfer engine setup, but the facts do not confirm that the block is engine state.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.30/unix.IP27` at 0xc00000000001a750
  **Inferred.** `start` is a 544-byte routine. It loads a doubleword from 0x9200000001600000 at 0xc00000000001a780 and calls `mapped_kernel_setup_tlb` directly at 0xc00000000001a798, passing a2 and a3 through. It then jumps through a register at 0xc00000000001a7b8 to the code at start+112, and the facts say no path from the entry reaches that code. The name and the TLB call suggest a first-stage kernel startup, but the facts do not show the rest of the path.
  Reviewed: 2026-11-12 (sonnet)

- `silence_all_audio` in `6.5.30/unix.IP27` at 0xc00000000001aa18
  **Inferred.** `silence_all_audio` is an 8-byte routine with no loads, stores or calls. The name suggests it mutes an audio device, but the body does nothing the facts can show, and the return value in v0 is not set.
  Reviewed: 2026-11-12 (sonnet)

- `is_fine_dirmode` in `6.5.30/unix.IP27` at 0xc00000000001d690
  **Inferred.** `is_fine_dirmode` is a 20-byte leaf. It loads a doubleword from 0x9200000001600000 at 0xc00000000001d694 and returns a value the facts do not resolve. The name suggests a predicate on a directory mode setting, but the body shows only the one load, so the predicate is not established.
  Reviewed: 2026-11-12 (sonnet)

- `ni_reset_port` in `6.5.30/unix.IP27` at 0xc00000000001d760
  **Inferred.** `ni_reset_port` is a 16-byte routine. It stores a doubleword to 0x9200000001600008 in the delay slot of its return at 0xc00000000001d768, and it returns that same address in v0. The name suggests a NI port reset, which a single register write could be, but the facts show no read-back or wait.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.30/unix.IP27` at 0xc000000000027178
  **Inferred.** `getcpuid` is a 116-byte routine. It loads a doubleword from 0x9200000001000020, which appears to be the fixed location that `get_myid` reads in the IP35 binary. It gets a nasid with `get_nasid` at 0xc00000000002718c and maps it to a cpuinfo record with `nasid_slice_to_cpuinfo` at 0xc0000000000271a0, bracketing the lookup with `spl1` and `splx`. If the record is null it calls `assfail` at 0xc0000000000271dc, and otherwise it returns a halfword at offset 0x1e of the record. The name suggests the current CPU identifier, so that halfword may be the id, but the facts do not name the field.
  Reviewed: 2026-11-12 (sonnet)

- `net_link_up` in `6.5.30/unix.IP27` at 0xc00000000002e58c
  **Inferred.** `net_link_up` is a 20-byte leaf. It loads a doubleword from 0x9200000001600000 at 0xc00000000002e590 and returns a value the facts do not resolve. The name suggests the link state of a network port, but the body is only the load and the return, and the same location is read by `start` and `is_fine_dirmode`.
  Reviewed: 2026-11-12 (sonnet)
