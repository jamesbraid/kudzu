# Routine role statements for 25 kudzu routines

These statements describe software routines in the identified binaries, taken from their instruction facts (registers, calls, constants and branches). They do not describe hardware behavior. Where the facts do not settle a role, the statement says so and leaves it open.

Every entry below is graded inferred. The facts name each routine from the binary's symbol table or the inventory, and none of the names is checked against a second source.

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP35boot/ksync_metered.a(sema.o)` at 0x3890
  **Inferred.** `_mraccess` calls `wchanmeter` at 0x38b0 and `mr_access` at 0x38c0, then calls `mrmeter_getentry` at 0x392c and branches on its result at 0x3934. It updates fields of the `wchanmeter` record at offsets 0x198 to 0x1d0, including a load-linked and store-conditional pair at 0x3940 and 0x394c. The name suggests a metered access path for a sync object, but the facts do not say what is counted or whether the pair is a lock.

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP35boot/ksync_metered.a(sema.o)` at 0x39ec
  **Inferred.** `_mrupdate` follows the same shape as `_mraccess`. It calls `wchanmeter` at 0x3a0c and `mr_update` at 0x3a1c, then updates the same record at offsets 0x1a0 to 0x1d8. It calls `mrmeter_entry` at 0x3ac4, and a bit test on a7 at 0x3a90 gates one branch. It reads as the update half of the pair, but what it records is not clear from the facts.

- `get_myid` in `6.5.22/usr/cpu/sysgen/IP35boot/l1.o` at 0xc3c
  **Inferred.** `get_myid` is a 16-byte leaf. It loads a doubleword from the fixed address 0x9200000001000020 at 0xc40 and returns it unchanged in v0. The name suggests a processor or node identifier, which fits a value read from a fixed location, but the facts do not give the meaning of the stored value.

- `nic_get_phase_bits` in `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` at 0x68
  **Inferred.** `nic_get_phase_bits` is a 52-byte routine. It loads a doubleword from 0x9000000018000078 at 0x70 and returns a value the facts do not resolve. The name suggests it reads phase bits from a NIC register, but the single load is the only evidence for that, and the address space is not identified.

- `gf_OdsyAttach` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_attach.o)` at 0x68
  **Inferred.** `gf_OdsyAttach` maps graphics memory with `gfxdd_mmap` at 0x18c, passing `gfx_fault` as the fault handler, and calls `gfxdd_recycle` at 0xfc before that. On the error path it calls `gf_OdsyUnMapGfx` at 0x1b4, brackets that call with `splhi` and `splx`, and logs through `cmn_err` at 0x1e0. Its return values are 0x0, 0xc and 0x16, which look like errno-style codes, but the facts do not name them.

- `gfP_OdsySaveHwShadow` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_ctxsw.o)` at 0xc0
  **Inferred.** `gfP_OdsySaveHwShadow` is a 1180-byte routine that is mostly flow control. It arms a timeout on `odsy_ctxsw_timeout` at 0x18c, waits on it with `untimeout_wait` at 0x1a0, and makes an indirect call at 0x20c with several register arguments taken from its context record. It detaches and reattaches the gfx flow with `gfx_flow_detach_me` and `gfx_flow_attach_me`, and waits for returned credits through `odsy_flow_await_returned_credits` with the thread pinned by `rt_pin_thread`. The name suggests a save of hardware shadow state on context switch, but the facts show far more credit and flow handling than saving, so what gets saved is open.

- `gf_OdsyDestroyDDRN` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_ddrn.o)` at 0x430
  **Inferred.** `gf_OdsyDestroyDDRN` is a 724-byte teardown routine. It unschedules the swap-buffer and retrace event handlers at 0x498 and 0x4a4, calls `odsy_hpipe_cleanup` at 0x4bc, frees a 0x4000-byte block with `kmem_free` at 0x4d8 only when the doubleword at offset 192 of the pointer at a2+112 is nonzero, and releases a 32 KB slot through `OdsyFree32kSlot` at 0x4e4. When the word at offset 408 of that same pointer is zero, it also calls `odsyDMAFinish`, `odsyMMdetach` and `odsyUnpinAndUnmapGctxRegions` at 0x624 to 0x638 and sets that word to 1; a nonzero word skips them. It calls `gfx_flow_dealloc` at 0x58c only when the word at offset 26424 of a0 is 1 and the pointer at offset 808 of a0 is nonzero. The name suggests destruction of a device descriptor, but the facts do not expand "DDRN", so the object being destroyed is open.

- `gf_OdsyInitialize` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_init.o)` at 0x15a8
  **Inferred.** `gf_OdsyInitialize` is the largest init routine in this batch, at 1656 bytes. It registers a board with `htp_register_board` at 0x161c, and it reads the nasid and chimera type through `get_nasid` and `get_chimera_type` at 0x17e8 to 0x180c. It allocates two gfx flows with `gfx_flow_alloc` at 0x1848 and 0x189c, sets up memory, DMA, retrace and interrupts through `odsyInitMM`, `odsyInitDMA`, `odsyInitRetrace` and `odsyInitInterrupts`, and calls `odsyReset` at 0x17cc after `odsy_usdelay` at 0x17a0. The calls fit a bring-up sequence, but the facts do not show that they execute in the order they are placed.

- `odsyLowLevelReset` in `6.5.22/usr/cpu/sysgen/IP35boot/odyssey.a(odsy_init.o)` at 0x2014
  **Inferred.** `odsyLowLevelReset` is an 876-byte routine. It reads and writes several fixed addresses in the 0x92000000e0000000 range, compares one loaded value with 0xd100 at 0x2070, and passes the constants 0xf4240 and 0x186a0 to `odsy_usdelay` as delay arguments. It reports through `cmn_err` at 0x2358. The read-modify-write pattern and the delays fit a reset, but the register meanings and the reason for the 0xd100 compare are not in the facts.

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP35boot/os.a(pda.o)` at 0x1b70
  **Inferred.** `doacvec` is a 116-byte routine. It reads a byte from the sign-extended low address 0xffffffffffffc080 and a doubleword from 0x92000000017a0038, then writes bytes to 0xffffffffffffc04e and 0xffffffffffffc081. Depending on the byte at [-0x3f80], it calls `panicspin` at 0x1b94, and it can call `tlbflush_rand` at 0x1bbc and `tlbflush` at 0x1bd0 before returning. The name does not settle what the vector is, but the TLB flush callees place the routine near TLB maintenance.

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP35boot/os.a(syssgi.o)` at 0x140
  **Inferred.** `syssgi` is a 16016-byte routine and the largest one in this batch. The facts show it reaching many subsystems: capability checks through `cap_able` at 0x228, 0x664, 0x708 and 0x8c0, XFS and XLV calls (`xfs_itable` at 0xa90, `xlv_attr_get` at 0x44c), asynchronous I/O (`kaio_rw` at 0x4b0, `kaio_suspend` at 0xb84), and signal-stack and process-group calls. It has five indirect calls, and several blocks that no path from the entry reaches. The name suggests a multiplexed system-call entry, but the facts do not show the selector dispatch, so this statement covers only the calls it makes.

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP35boot/os.a(time.o)` at 0xc0
  **Inferred.** `dogetitimer` is a 400-byte routine. It takes a lock reached through a2+768 with `mutex_lock` at 0xf8 and releases it at 0x120, calls `chktimeout_tick` at 0x15c and 0x218 with different first arguments, and converts a tick count with `tick_to_timeval` at 0x17c. It writes doubleword fields at offsets 0x0 to 0x18 of its output argument. The name suggests the body of a get-interval-timer call, which fits the tick-to-timeval conversion, but the facts do not say which timer the two tick reads come from.

- `part_discovery` in `6.5.22/usr/cpu/sysgen/IP35boot/partition.o` at 0x1f6c
  **Inferred.** `part_discovery` is a 724-byte routine. It takes a semaphore with `cpsema` at 0x1f9c and 0x1fac, gets the local nasid with `get_nasid` at 0x20a8, and probes partitions per nasid with `part_probe_nasid` at 0x20b0, 0x20f8 and 0x2220. It registers `part_activate` as an interrupt target through `part_interrupt_set` at 0x219c, starts a partition thread at 0x21ac, and reads a vector with `vector_read` at 0x21f0. Its loops count a value in s0 against bounds of 0x40 and 0x80, but the facts do not say whether those bounds are node limits.

- `pic_attach2` in `6.5.22/usr/cpu/sysgen/IP35boot/pic.o` at 0x5e4
  **Inferred.** `pic_attach2` is a 3176-byte attach routine. It registers the device in the hardware graph with `hwgraph_char_device_add` at 0x650 and `hwgraph_info_add_LBL` at 0x680, and it allocates a 0xac0-byte per-device block with `kmem_zalloc` at 0x740. It links that block into a list with `swap_ptr`, reads mode and speed through `pcireg_mode_get` and `pcireg_speed_get`, and looks up L1 and module records through `get_elsc`, `iobrick_l1type_get_nasid` and `iobrick_module_get_nasid`. It also initializes a spinlock and an mrlock inside the block. The symbol name suggests a PCI interface chip, but the facts do not identify the chip, so that is open.

- `xbridge_attach` in `6.5.22/usr/cpu/sysgen/IP35boot/xbridge.o` at 0x54
  **Inferred.** `xbridge_attach` is a 3484-byte attach routine with the same call pattern as `pic_attach2`. It gets the bridge pointer with `pcibr_bridge_ptr_get` at 0x84, registers a PCI I/O provider with `pciio_provider_register` at 0xf4 and starts it at 0xfc, and adds a character device through `hwgraph_char_device_add` at 0x120. It allocates a 0xac0-byte block and an 0x18-byte record, links the record into `pcibr_list` with `swap_ptr` at 0x35c, and reads NIC address and mode fields through `pcireg_nic_addr`, `pcireg_mode_get` and `pcireg_speed_get`. The calls fit a bridge provider attach, but the facts do not prove which bridge the symbol refers to.

- `sn00_vmc` in `6.5.30/unix.IP27` at 0xc000000000025998
  **Inferred.** `sn00_vmc` is a 200-byte routine. It reads a halfword at the sign-extended address 0xffffffffffffc422 and a doubleword at 0xffffffffffffc400, then indexes a table reached through the pointer at -0x3c00 with the halfword at -0x3bde. It calls `device_inventory_add` at 0xc0000000000259d0 with the values 0xb and 0x3, then the bridge hint calls `pcibr_hints_dualslot`, `pcibr_hints_subdevs` and `pcibr_hints_fix_rrbs`, and registers `pcibr_set_rrb_callback` at 0xc000000000025a04. It stores four doublewords to abs 0x9200000001220050 near its `spl65` and `splx` calls. The prefix "sn00" points to the SN0 platform, but the facts do not say what "vmc" stands for.

- `restart` in `6.5.30/unix.IP27` at 0xc000000000029814
  **Inferred.** `restart` is a 356-byte routine. It calls `us_delay` three times with the constants 0xf4240 and 0x2dc6c0, calls `sysctlr_power_off` at 0xc0000000000298cc with a0 zero, and resets a NI port through `ni_reset_port` at 0xc000000000029904. It finds the local nasid with `get_nasid`, calls `elsc_init` and `elsc_system_reset` with a stack record, and prints through `printf` at 0xc00000000002994c. It also stores a doubleword to 0x9200000001600008 at 0xc000000000029968, the same register `ni_reset_port` writes. The name suggests the system restart path, and the power-off and reset calls fit that, but the facts do not say which step runs first or what the printed text reports.

- `page_local_calias` in `6.5.30/unix.IP27` at 0xc00000000002b108
  **Inferred.** `page_local_calias` is a 60-byte routine. It loads a doubleword from 0x9200000001000028, branches on that value at 0xc00000000002b11c, and compares a3 with v1 as unsigned values at 0xc00000000002b130. The compare result selects a return of 0 or 1. The name suggests a check for a local alias on a page, which fits a flag test followed by a range compare, but the facts do not show what v1 holds, so the meaning of the compare is open.

- `bte_init` in `6.5.30/unix.IP27` at 0xc00000000002bf5c
  **Inferred.** `bte_init` is a 232-byte routine. It fills fields of its argument block at offsets 0x0 to 0x70, including a copy of the halfword at abs 0xffffffffffffc422 and a halfword from the table at 0xc0000000014d4e80. It writes two doublewords to abs 0x92000000014001f8 and calls `spinlock_init` at 0xc00000000002bf98 on the block at offset 0x44. It returns its first argument. The name suggests block transfer engine setup, but the facts do not confirm that the block is engine state.

- `start` in `6.5.30/unix.IP27` at 0xc00000000001a750
  **Inferred.** `start` is a 544-byte routine. It loads a doubleword from 0x9200000001600000 at 0xc00000000001a780 and calls `mapped_kernel_setup_tlb` through the pointer at 0xc000000000043808, passing a2 and a3 through. It then jumps through a register at 0xc00000000001a7b8 to the code at start+112, and the facts say no path from the entry reaches that code. The name and the TLB call suggest a first-stage kernel startup, but the facts do not show the rest of the path.

- `silence_all_audio` in `6.5.30/unix.IP27` at 0xc00000000001aa18
  **Inferred.** `silence_all_audio` is an 8-byte routine with no loads, stores or calls. The name suggests it mutes an audio device, but the body does nothing the facts can show, and the return value in v0 is not set.

- `is_fine_dirmode` in `6.5.30/unix.IP27` at 0xc00000000001d690
  **Inferred.** `is_fine_dirmode` is a 20-byte leaf. It loads a doubleword from 0x9200000001600000 at 0xc00000000001d694 and returns a value the facts do not resolve. The name suggests a predicate on a directory mode setting, but the body shows only the one load, so the predicate is not established.

- `ni_reset_port` in `6.5.30/unix.IP27` at 0xc00000000001d760
  **Inferred.** `ni_reset_port` is a 16-byte routine. It stores a doubleword to 0x9200000001600008 in the delay slot of its return at 0xc00000000001d768, and it returns that same address in v0. The name suggests a NI port reset, which a single register write could be, but the facts show no read-back or wait.

- `getcpuid` in `6.5.30/unix.IP27` at 0xc000000000027178
  **Inferred.** `getcpuid` is a 116-byte routine. It loads a doubleword from 0x9200000001000020, which appears to be the fixed location that `get_myid` reads in the IP35 binary. It gets a nasid with `get_nasid` at 0xc00000000002718c and maps it to a cpuinfo record with `nasid_slice_to_cpuinfo` at 0xc0000000000271a0, bracketing the lookup with `spl1` and `splx`. If the record is null it calls `assfail` at 0xc0000000000271dc, and otherwise it returns a halfword at offset 0x1e of the record. The name suggests the current CPU identifier, so that halfword may be the id, but the facts do not name the field.

- `net_link_up` in `6.5.30/unix.IP27` at 0xc00000000002e58c
  **Inferred.** `net_link_up` is a 20-byte leaf. It loads a doubleword from 0x9200000001600000 at 0xc00000000002e590 and returns a value the facts do not resolve. The name suggests the link state of a network port, but the body is only the load and the return, and the same location is read by `start` and `is_fine_dirmode`.
