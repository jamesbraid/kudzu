# Routine role statements, tier-1 batch 25

These statements describe what the disassembly shows for each routine. They describe software behavior in the identified binary, not hardware behavior. Names in backticks are the inventory's labels, not confirmed symbols. Where the facts stop short, the statement says so.

- `pcireg_nic_addr` in `6.5.30/unix.IP27` at 0xc0000000000827c4
  **Inferred.** The routine `pcireg_nic_addr` tests its first argument against 0 and, when it is 0, branches to the panic call at 0xc00000000008290c; otherwise it reads a 16-bit field at offset 0x72 of it and compares that against 2, 3, 0, 1 and -1. A value of -1 also leads to the panic call, and a value outside that list reaches the return at 0xc000000000082820. It loads an 8-byte field at offset 0x8 and a 4-byte field at offset 0x4 of the same argument, and on some paths compares the 4-byte field with 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002, going to the panic call when the last comparison fails. Role (from name): the name suggests a PCI register address for a network device, but the facts show only the dispatch shape, so the meaning of each case stays open.
  Reviewed: 2026-11-12 (sonnet)
- `pcireg_intr_addr_vect_set` in `6.5.30/unix.IP27` at 0xc000000000085610
  **Inferred.** This is `pcireg_intr_addr_vect_set`, with the same argument layout as `pcireg_nic_addr`: a 16-bit field at 0x72 and 8-byte and 4-byte fields at 0x8 and 0x4. It branches away when its first argument equals 0, and it tests its second argument against 0 and 8 (signed), with a path to the `assfail` call at 0xc000000000085828 when it is outside that range. It has a `panic` exit at 0xc00000000008583c. Its case paths perform 8-byte and 4-byte loads and stores, but the facts do not resolve where those values come from or go to. Role (from name): the name suggests it sets an interrupt vector address.
  Reviewed: 2026-11-12 (sonnet)
- `pcireg_intr_addr_addr_set` in `6.5.30/unix.IP27` at 0xc000000000085a7c
  **Inferred.** This is `pcireg_intr_addr_addr_set`, close in size (588 bytes) and branch count to the vector variant above, with the same `assfail` and `panic` exits and the same range test on its second argument. Its case paths perform 8-byte and 4-byte loads and stores, but the facts do not resolve their targets. Role (from name): the name suggests it sets the interrupt address rather than the vector, but the facts do not confirm which field it writes.
  Reviewed: 2026-11-12 (sonnet)
- `eiattach` in `6.5.30/unix.IP27` at 0xc000000000094458
  **Inferred.** This is the attach routine `eiattach`. It calls `ioc3_subdev_enabled` with 7 and branches away when that result is 0. Otherwise it allocates a 0x38-byte state block with `kmem_zalloc` and a 0x80000-byte buffer with `kvpalloc`, registers a character device with `hwgraph_char_device_add` and an inventory entry with `device_inventory_add`, and calls `ioc3_intr_connect` (call 16). Whether the `ioc3_intr_connect` call depends on the `ioc3_is_console` result is not established by the facts. It returns 0 or -1. Role (from name): an IOC3 subdevice driver; the device class is inferred from the callee names only.
  Reviewed: 2026-11-12 (sonnet)
- `pckm_attach` in `6.5.30/unix.IP27` at 0xc000000000096394
  **Inferred.** This attach routine `pckm_attach` sets up what looks like a keyboard and mouse controller behind an IOC3 chip, judging by its `ioc3_pckm_mlreset` callee and the `pckm_` names. It calls `ioc3_subdev_enabled` with 3 and branches away when that is 0, allocates state blocks with `kmem_zalloc`, and calls `ioc3_pckm_mlreset`. Later steps depend on tests: `pckm_flushinp` is called only when a state field at offset 0x28 differs from 0x13, `pckm_pollcmd` is called with 0xf5, 0xf4 and 0xf6, each followed by `data_check` against 0xfa, and `default_kbd_setting` and `default_ms_setting` run only when the preceding `data_check` result is nonzero. It calls `ioc3_intr_connect` and returns 0 or -1, and it contains `cmn_err` calls at level 6. The controller protocol is not visible in the facts, so the PS/2-style reading is a guess.
  Reviewed: 2026-11-12 (sonnet)
- `get_myid` in `6.5.30/unix.IP27` at 0xc0000000000988ec
  **Inferred.** This is a leaf routine, `get_myid`. It loads one 8-byte value from the fixed address 0x9200000001000020 and returns it. Role (from name): the name suggests an identifier, but the facts do not say what it identifies.
  Reviewed: 2026-11-12 (sonnet)
- `cn_init` in `6.5.30/unix.IP27` at 0xc0000000000a1940
  **Inferred.** This is the early console initialization `cn_init`. It calls `gfx_earlyinit` first. If a halfword at 0xc0000000014f7110 is 0, it initializes a spinlock and a mutex and calls `is_specified`, then compares the first byte of a string with 0x67 and 0x47. If that halfword is nonzero, `vn_rele` is called on a pointer at 0xc0000000014f70d0 only when that pointer and the first argument are both nonzero. `tp_init` (call 6) and `du_init` (call 7) are reached on separate paths, and the return value is one of their results or an unresolved value. The facts do not show which console is selected, and what `du_init` does is open.
  Reviewed: 2026-11-12 (sonnet)
- `ef_ioc3eaddr` in `6.5.30/unix.IP27` at 0xc0000000000e602c
  **Inferred.** This routine `ef_ioc3eaddr` calls `nic_get_eaddr` with two pointers at offsets 48 and 52 from a pointer loaded from offset 0x398 of its first argument. If that call returns nonzero, it reports through `cmn_err` at level 1 using the word at offset 0x38c of its first argument. It returns 0 or 5 depending on the path. Role (from name): the name suggests it reads the Ethernet address of an IOC3-based NIC. The facts do not establish what the value 5 means or which path is the failure path.
  Reviewed: 2026-11-12 (sonnet)
- `doacvec` in `6.5.30/unix.IP27` at 0xc0000000001e5890
  **Inferred.** This small routine `doacvec` reads two flag bytes, a word and a doubleword from a fixed block at the top of the address space. It calls `panicspin` when the first byte is nonzero. If the second byte is nonzero it stores two bytes. It calls `tlbflush_rand` with argument 1 when the word is nonzero and, separately, calls `tlbflush` with the doubleword value when that is nonzero, then stores a doubleword. Both flush calls can run. Role (from name): a TLB flush step is suggested by the callees, but the link between the flags and the flush is not shown.
  Reviewed: 2026-11-12 (sonnet)
- `syssgi` in `6.5.30/unix.IP27` at 0xc0000000002317e0
  **Inferred.** This is the large `syssgi` dispatch, about 12.7 KB of code with 395 calls. Its most common loads are at offsets 0x8, 0x10, 0x18, 0xc and 0x14 from a base the facts do not resolve. Its callees cover job control (`syssgi_makenewjob`, `syssgi_joinjob`, `syssgi_killjob`, `syssgi_getjusage`), wait-job and pid handling, capability checks through `cap_able`, NUMA tests, NFS and RPC controls, accounting through `acctctl`, and copy-in and copy-out of user arguments. The dispatch targets are only partly resolved, so the full request list is not established here.
  Reviewed: 2026-11-12 (sonnet)
- `dogetitimer` in `6.5.30/unix.IP27` at 0xc000000000259c48
  **Inferred.** This routine `dogetitimer` has two paths. When its first argument is 0 it calls `spl65`, copies four doublewords under a mutex, calls `chktimeout_tick` with selector 0 or 2 and then `tick_to_timeval`, and calls `splx`. When the first argument is nonzero it calls `spl1`, copies four doublewords from a table indexed by the argument shifted left by 5, and calls `splx`. The only return shown is 0. The mapping from selector to timer kind is inferred from the name and the constants.
  Reviewed: 2026-11-12 (sonnet)
- `MgrasGeneralInterrupt` in `6.5.30/unix.IP27` at 0xc000000000388cac
  **Inferred.** This is `MgrasGeneralInterrupt`, which by its name is an interrupt handler for the Mgras graphics board. It reads two words at offsets 0x11020 and 0x11040 from the block reached through the pointer at offset 0xd0 of its first argument. Depending on bit tests of those values it calls `vsema` (three call sites), calls `MgrasSchedSwapBufFinish` when the 0x400 bit is set, reports through `cmn_err_tag` with codes 0xaa5 to 0xaa9 for further bits, and calls `mgras_error` after the error-mask test against 0x2000a200. The bit-to-event mapping comes from the constants alone and is not confirmed.
  Reviewed: 2026-11-12 (sonnet)
- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/unix.IP27` at 0xc000000000395e6c
  **Inferred.** This is `gfx_flow_delayed_hiwater_intr_prehandler`. It reads a 16-bit value from the fixed address 0xffffffffffffc420 and indexes a table with it. When the table entry is nonzero it makes one call through a computed target that the disassembler could not name; otherwise it performs an 8-byte store whose destination the facts do not resolve. It returns a value the facts do not resolve. The callee and the meaning of the high-water mark are open.
  Reviewed: 2026-11-12 (sonnet)
- `gfx_flow_alloc` in `6.5.30/unix.IP27` at 0xc0000000003960a8
  **Inferred.** This is `gfx_flow_alloc`. It allocates a 0x70-byte record with `kmem_alloc` and its return value is 0 or the `kmem_alloc` result. It calls `xwidget_info_get` and `xwidget_info_id_get`, stores fields into the record, calls `kvalloc`, `xwidget_info_master_get` and `master_node_get`, and calls `gfx_flow_init` on a separate path. A group of five `cmn_err` calls and a `kmem_free` of the 0x70-byte record also appear, which suggests a cleanup path, but the facts do not state the failure condition. The field meanings are inferred from the offsets, not from names in the facts.
  Reviewed: 2026-11-12 (sonnet)
- `gfx_flow_dealloc` in `6.5.30/unix.IP27` at 0xc000000000396448
  **Inferred.** This routine `gfx_flow_dealloc` frees a 0x70-byte flow record that `gfx_flow_alloc` created. It reads a 16-bit field at offset 0x50 and a 4-byte field at offset 0x54 of the record, stores one 8-byte value to an unresolved location, and passes the record to `kmem_free` with its size. The facts do not say what the stored value is.
  Reviewed: 2026-11-12 (sonnet)
- `gfx_flow_attach_me` in `6.5.30/unix.IP27` at 0xc000000000396524
  **Inferred.** This routine `gfx_flow_attach_me` stores a halfword at offset 0x60 and a doubleword at offset 0x68, and calls `err_int_block_mask` with block code 8 only when an unresolved value is below 3. It returns either an address formed from a table base and an index, or the result of `err_int_block_mask`. Role (from name): attaching a flow record to an error-interrupt mask. The table and the meaning of the compared value are not resolved.
  Reviewed: 2026-11-12 (sonnet)
- `gfx_flow_detach_me` in `6.5.30/unix.IP27` at 0xc0000000003966c8
  **Inferred.** This routine `gfx_flow_detach_me` stores several doublewords, a word at offset 0x14, a halfword at 0x60 and a doubleword at 0x68. Only when an unresolved value is below 3 does it call `err_int_unblock_mask`, with 8 and 1 as its first two arguments and the halfword at 0xffffffffffffc420 as the third. Register a5 holds 0xdeadbabe at that call, but the facts do not show that it is an argument. It returns either the word at 0x9200000001000098 or the unblock result. Role (from name): the opposite of the attach routine.
  Reviewed: 2026-11-12 (sonnet)
- `_get_timestamp` in `6.5.30/unix.IP30` at 0xa800000020011328
  **Inferred.** This is a leaf routine, `_get_timestamp`. It loads one 4-byte value from 0xffffffffaff20004 and returns it. Role (from name): a timer or timestamp read, but the facts show only the single load.
  Reviewed: 2026-11-12 (sonnet)
- `getcpuid` in `6.5.30/unix.IP30` at 0xa800000020011334
  **Inferred.** This is a leaf routine, `getcpuid`. It loads an 8-byte value from 0xffffffffaff50000 and returns it. The name suggests a CPU identifier, but the field layout of the value is not established.
  Reviewed: 2026-11-12 (sonnet)
- `bootstrap` in `6.5.30/unix.IP30` at 0xa800000020011434
  **Inferred.** This is early boot code, `bootstrap`. It reads a word at offset 0x10, calls `getcpuid` (register a3 holds 6 at that call), calls `tlbwired` with 0, 0, -0x4000, 1 and a5 = 0x6000, performs three stores of different widths, and then tail-jumps to `cboot`. The facts do not resolve the stored values or addresses.
  Reviewed: 2026-11-12 (sonnet)
- `_ticksper1024inst` in `6.5.30/unix.IP30` at 0xa8000000200115f0
  **Inferred.** This routine `_ticksper1024inst` performs two 8-byte loads and has two signed greater-than-zero branches, both backward: one branches to itself and the other to the instruction before it. Its return value is not resolved. Role (from name): the name suggests a tick rate per 1024 instructions, but the load addresses, units and result are not established by the facts.
  Reviewed: 2026-11-12 (sonnet)
- `start` in `6.5.30/unix.IP30` at 0xa800000020011ac0
  **Inferred.** This is the IP30 entry point, `start`. The only path the facts show is a tail jump to 0xa800000020011b08, which is start+72. The routine is 436 bytes, and no other path from the entry is shown. The facts say nothing more about the boot state, so the role is left open.
  Reviewed: 2026-11-12 (sonnet)
- `silence_all_audio` in `6.5.30/unix.IP30` at 0xa800000020011d1c
  **Inferred.** This is `silence_all_audio`, a short routine that reads a word at 0xa8000000204ae390. When the word is nonzero it stores one word to 0x900000001f600038, and in either case it tail-jumps to `ip30_rad_halt`. Role (from name): muting audio before a halt; the facts show only the load, the conditional store and the jump.
  Reviewed: 2026-11-12 (sonnet)
- `cpu_disable` in `6.5.30/unix.IP30` at 0xa800000020012a04
  **Inferred.** This routine `cpu_disable` reads and rewrites an 8-byte word at 0xffffffffaff00000 and stores a 4-byte value at 0xa8000000204ae380. Role (from name): taking a CPU offline; the stored value is not resolved, so the meaning of the write is left open.
  Reviewed: 2026-11-12 (sonnet)
- `cpu_exists` in `6.5.30/unix.IP30` at 0xa800000020012a34
  **Inferred.** This routine `cpu_exists` loads an 8-byte value from 0xffffffffaff00080 and returns it without resolving it. Role (from name): a CPU presence check. The polarity of the test and the field width are not established.
  Reviewed: 2026-11-12 (sonnet)

Dropped entries: none.
