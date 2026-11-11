# Tier-1 routine readings, kudzu batch 31

Role statements for the 25 routines at ranked positions 775 to 799 of the tier-1 list,
read from their instructions only. One entry is left out because its statement names a routine the symbol table gives differently at that address. Each grade is inferred: the facts show what the code loads, stores, calls
and branches on, but not what the fields and constants mean. Addresses are in the binary
named in each header. Hub register addresses are the uncached `0x92...` aliases the
instruction listing reports.

- `ef_ioc3eaddr` in `6.5.30/unix.IP35` at 0xc0000000001785c0
  **Inferred.** `ef_ioc3eaddr` (532 bytes) reads a word at offset 0x38c and doublewords at 0x398 from its first argument, plus a doubleword from the absolute address 0x9000000014000000. It calls `ibrick_mac_addr_get` at 0xc000000000091ee4, `nic_get_eaddr` at 0xc000000000094fe0, `get_elsc` at 0xc000000000065240 and `iobrick_get_sys_snum` at 0xc0000000000d1008, and it asks `kopt_find` at 0xc0000000002c4890 for an option. The name and that call order suggest it builds an Ethernet station address for the IOC3 interface by trying several sources in turn. The facts show the order of the calls but not which source wins, so the fallback rules stay open.

- `doacvec` in `6.5.30/unix.IP35` at 0xc000000000294c48
  **Inferred.** `doacvec` (116 bytes) tests bit 0 of the doubleword at hub address 0x92000000017a0038 and branches on it. Depending on flags read from absolute addresses, it calls `panicspin` at 0xc0000000002af314 with that hub address, or calls `tlbflush_rand` at 0xc000000000265ee4 and `tlbflush` at 0xc000000000265dec. It also rewrites a doubleword at absolute address 0xffffffffffffc0d0. The name does not say what the vector is, and the instruction listing does not resolve the absolute addresses or the flag meanings. The routine looks like a TLB flush guarded by a hub-side bit, with a panic path, but that is only partly established.

- `syssgi` in `6.5.30/unix.IP35` at 0xc0000000002e0310
  **Inferred.** `syssgi` (16704 bytes, the largest routine in this batch) is a dispatcher. It compares a word read through its first argument against several constants, including 0xab, 0xac and 0x16a, and then branches into helpers named `syssgi_*`, among them `syssgi_getjid`, `syssgi_makenewjob`, `syssgi_killjob`, `syssgi_tune` and `syssgi_nvram`. Along the way it copies user memory with `copyin` (19 call sites) and `copyout` (29 call sites), checks capabilities with `cap_able` (22 call sites), and reads credentials through `get_current_cred`. The helper names suggest the entry point for a family of system calls. The instruction listing does not show what each constant selects, so the mapping from command to helper is partly open.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/unix.IP35` at 0xc00000000047b788
  **Inferred.** `gfx_flow_delayed_hiwater_intr_prehandler` (148 bytes) reads two halfwords at absolute address 0xffffffffffffc428, indexes a per-CPU table with a value from that region, and stores a doubleword to hub address 0x9200000001000000. It loads a record through the chosen table entry and calls an indirect target with arguments taken from that record. A loop around the table walk tests one loaded pointer for nonzero. The name suggests a prehandler for a high-water interrupt in the graphics flow-control path. The indirect callee and the field meanings are not resolved, so the purpose rests on the name.

- `gfx_flow_alloc` in `6.5.30/unix.IP35` at 0xc00000000047b98c
  **Inferred.** `gfx_flow_alloc` (592 bytes) allocates a 0x70-byte record with `kmem_alloc` at 0xc00000000022c0f8 and fills it with stores at offsets 0x0 through 0x68. Before filling, it resolves its widget through `xwidget_info_get`, `xwidget_info_id_get`, `xwidget_info_master_get` and `master_node_get`, and it sizes a second buffer with `kvalloc`. It ends by calling `gfx_flow_init` at 0xc00000000047b81c. The return value is either 0 or the allocated record. The name and layout suggest a constructor for a per-widget graphics flow record, but the field meanings are not in the facts.

- `gfx_flow_dealloc` in `6.5.30/unix.IP35` at 0xc00000000047bbe8
  **Inferred.** `gfx_flow_dealloc` (72 bytes) reads a halfword at offset 0x50 and a word at 0x54 from its first argument, then calls `kmem_free` at 0xc00000000022c288 with that argument and the constant 0x70. The 0x70 matches the size `gfx_flow_alloc` requests, so this looks like its matching free path. The instruction listing shows no branch, so any check the code makes before the free is not visible here.

- `gfx_flow_attach_me` in `6.5.30/unix.IP35` at 0xc00000000047bcc8
  **Inferred.** `gfx_flow_attach_me` (292 bytes) reads a halfword from absolute address 0xffffffffffffc428 and stores a halfword at offset 0x60 and a doubleword at 0x68 of its argument. It writes an entry into the per-CPU attachment table, writes three doublewords to hub address 0x9200000001000000, and reads table records the same way the prehandler does. The name suggests registering a device or CPU with a flow. The instruction listing shows the table and hub writes but not the fields they fill, and it does not resolve the return value.

- `gfx_flow_detach_me` in `6.5.30/unix.IP35` at 0xc00000000047be3c
  **Inferred.** `gfx_flow_detach_me` (220 bytes) is the counterpart of `gfx_flow_attach_me`. It clears the attachment-table slot for the record it is given, writes to the same hub block at 0x9200000001000000 and also to 0x9200000001000090, and reads 0x9200000001000098 back. It returns the table slot it read. The mirrored shape supports reading the pair as attach and detach, but the meaning of the 0x90 and 0x98 registers is not in the facts.

- `cn_init` in `6.5.30/usr/cpu/sysgen/IP27boot/cn.o` at 0xaac
  **Inferred.** `cn_init` (284 bytes) stores into fields of the `cn` symbol's structure and then calls `spinlock_init` and `init_mutex`. It calls `gfx_earlyinit` first, then tests the byte in `arg_console` against the constants 0x67 and 0x47 before choosing among the `tp_init` and `du_init` paths. The name and the console tests suggest boot-time console setup. Which console each branch picks is not established by the facts.

- `bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** `bitlockdebug` (612 bytes) uses load-linked and store-conditional pairs on words at offsets 0xa8, 0xac, 0xb0, 0xb8 and 0xbc of its third argument, and it writes several doubleword and halfword fields of that structure. It calls `try_bitlock` at 0x16cc and again at 0x17f8, reads the hub word at 0x9200000001030100, and calls `cmn_err` with that value on one path. It calls `splx` on the exit path. The name suggests the debug variant of a bit lock that reports on failure. The structure layout is not named in the facts.

- `_64bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** `_64bitlockdebug` (612 bytes) has the same shape as `bitlockdebug` above: the same load-linked and store-conditional offsets, the same hub word at 0x9200000001030100, and the same diagnostic print on one path. The callee differs: it calls `try_64bitlock` at 0x1f70 and 0x209c. The name suggests the 64-bit counterpart of the same debug lock, which fits the callee. The instruction listing does not show the lock layout, so the two are treated as siblings rather than verified equivalents.

- `_64bitunlockdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** `_64bitunlockdebug` (280 bytes) looks like the unlock side of the pair above. It updates a set of fields in its third argument, including a halfword at offset 0x2a and doublewords at 0x40, 0x48, 0x58, 0x68, 0x78 and 0x80. Offsets 0x2a and 0x40 recur in the lock routines, which suggests shared fields, but the facts do not prove it. It reads the hub word at 0x9200000001030100 and calls `cmn_err` twice, once with a fixed format and once with a value from its first argument. The name suggests a debug unlock with logging. The field meanings are not in the facts.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** `gfx_flow_delayed_hiwater_intr_prehandler` (144 bytes, relocatable object copy) has the same shape as the kernel-image copy at 0xc00000000047b788. It reads halfwords from absolute address 0xffffffffffffc420, looks up entries through `gfx_flow_cpu_attachments` and `pdaindr`, stores a doubleword to hub address 0x9200000001000000, and makes an indirect call with arguments read from the attachment record. The halfword address differs from the kernel copy (c420 here, c428 there). That may reflect a different layout of the same data, and the facts do not settle it. The name suggests a high-water interrupt prehandler, but the callee is unresolved.

- `gfx_flow_alloc` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x23c
  **Inferred.** `gfx_flow_alloc` (916 bytes, object copy) allocates a 0x70-byte record with `kmem_alloc` and fills many fields at offsets 0x0 through 0x68. It resolves the widget chain through `xwidget_info_get`, `xwidget_info_id_get`, `xwidget_info_master_get` and `master_node_get`, sizes a buffer with `kvalloc`, and calls `gfx_flow_init` at 0x458. Unlike the kernel copy, it also reads the `compact_to_nasid_node` table and passes the value of `hub_use_transaction_credits` to a `cmn_err` call. The name matches the kernel copy. The purpose of the extra paths is not established.

- `gfx_flow_dealloc` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x5dc
  **Inferred.** `gfx_flow_dealloc` (72 bytes, object copy) matches the kernel-image copy. It reads a halfword at offset 0x50 and a word at 0x54 from its first argument, then calls `kmem_free` at 0x610 with that argument and the size 0x70. The size matches the allocation in the copy above, so this is probably the matching free. The instruction listing shows no branch, so there is no visible failure path.

- `gfx_flow_attach_me` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x6b8
  **Inferred.** `gfx_flow_attach_me` (344 bytes, object copy) matches the kernel-image attach routine in its table and hub writes. It writes the attachment-table slot, fields of its argument, and three words to hub address 0x9200000001000000. It also reads the hub word at 0x9200000001600000 and the `compact_to_nasid_node` table. It calls `err_int_block_mask` at 0x7f8 with the constant 0x8, its second argument, the attach count plus one, and an indexed value. The kernel copy has no calls at all, so this block call is new in the object copy. The name and the call suggest that attaching also blocks an error-interrupt source. The mask meaning is not in the facts.

- `gfx_flow_detach_me` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x85c
  **Inferred.** `gfx_flow_detach_me` (308 bytes, object copy) is the pair of the attach routine above. It clears the attachment slot, writes the same hub block, and reads 0x9200000001600000. It calls `err_int_unblock_mask` at 0x970 with 0x8, the constant 0x1, an indexed value and a computed mask. Attach blocks and detach unblocks, which reads as a paired lock and unlock of one error source. The mask arithmetic is not named, so the bit it toggles stays open.

- `ef_ioc3eaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/if_ef.o` at 0x73c
  **Inferred.** `ef_ioc3eaddr` (84 bytes, driver object copy) is a thin version of the kernel-image routine at 0xc0000000001785c0. It reads the same fields of its first argument (the doubleword at 0x398 and the word at 0x38c) and calls `nic_get_eaddr` at 0x754 with values from the 0x398 record. On failure it calls `cmn_err` with a fixed message. It has no brick or serial lookups, so the driver copy appears to rely on the NIC address alone. The return value is 0 or 5.

- `pckm_attach` in `6.5.30/usr/cpu/sysgen/IP27boot/ioc3_pckm.o` at 0x5c
  **Inferred.** `pckm_attach` (1332 bytes) appears to attach a keyboard and mouse controller that sits behind the IOC3. It enables an IOC3 subdevice with the constant 0x3, traverses two hardware-graph paths, writes the IOC3 register at 0x400000 twice, and checks for the console with `ioc3_is_console`. It allocates three records with `kmem_zalloc`, then sets keyboard and mouse defaults through `initkbdtype`, `set_ports`, `default_kbd_setting` and `default_ms_setting`. It sends the constants 0xf5, 0xf4 and 0xf6 through `pckm_pollcmd`, each followed by a `data_check`. It also adds two character devices and inventory entries, then connects `pckm_intr` with `ioc3_intr_connect`. The callee names make the device role likely. The instruction listing does not say what the constant command bytes mean.

- `eiattach` in `6.5.30/usr/cpu/sysgen/IP27boot/ioc3ei.o` at 0x4a8
  **Inferred.** `eiattach` (420 bytes) appears to attach an IOC3 interface device, probably the Ethernet one, to the hardware graph. It enables an IOC3 subdevice with the constant 0x7, traverses two graph paths, and allocates a 0x38-byte record with `kmem_zalloc`, filling its fields. It sets up a lock with `spinlock_init`, allocates a 0x80000-byte buffer with `kvpalloc`, adds a character device and a node with changed permissions, records an inventory entry, and calls `ei_make_hwgraph_nodes`. It connects `ioc3_ei_intr` through `ioc3_intr_connect`. The return value is 0 or -1. The `ei` prefix is not explained in the facts, so the Ethernet reading is an inference.

- `start` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x15d0
  **Inferred.** `start` (544 bytes, IP27 boot kernel object) reads the hub word at 0x9200000001600000 and calls `mapped_kernel_setup_tlb` at 0x1618. It then jumps through a relocated target at `.text+5696` (the `jr` at 0x1638). The instruction listing reports that the code from 0x1640 to 0x17ec is not reached from the entry, so the tail is not on the static path. The name and the early TLB setup suggest a boot entry point.

- `get_nasid` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x44e4
  **Inferred.** `get_nasid` (28 bytes, IP27 boot object) reads one doubleword from hub address 0x9200000001600000 and returns, with the return value unresolved in the instruction listing. The same hub word is also read by `start` and by `is_fine_dirmode` in this image. The name says node id, but the facts show no field mask, so which bits carry the node number stays open.

- `get_slice` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x4500
  **Inferred.** `get_slice` (16 bytes) loads one doubleword from hub address 0x9200000001000020 and returns it, shifted by zero, so the value comes back unchanged. The name suggests a slice number, but the facts show no mask or field extraction. Which bits, if any, name a slice stays open.

- `is_fine_dirmode` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x4510
  **Inferred.** `is_fine_dirmode` (20 bytes) loads one doubleword from hub address 0x9200000001600000, the word `get_nasid` reads, and returns. The instruction listing leaves the return value unresolved, so the link between that load and the return is not shown. The name points at a directory-mode check, but no compare or flag test appears in the facts, so that reading stays unconfirmed.
