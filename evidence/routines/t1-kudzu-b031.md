# Tier-1 routine readings, kudzu batch 31

Role statements for the 25 routines at ranked positions 775 to 799 of the tier-1 list,
read from their instructions only. One entry is left out because its statement names a routine the symbol table gives differently at that address. Each grade is inferred: the facts show what the code loads, stores, calls
and branches on, but not what the fields and constants mean. Addresses are in the binary
named in each header. Hub register addresses are the uncached `0x92...` aliases the
instruction listing reports.

- `ef_ioc3eaddr` in `6.5.30/unix.IP35` at 0xc0000000001785c0
  **Inferred.** `ef_ioc3eaddr` (532 bytes) reads a word at offset 0x38c and a doubleword at 0x398 from its first argument. It calls `sprintf` and then `strstr` twice; when either `strstr` returns zero it goes to a path that calls `nic_get_eaddr` at 0xc000000000094fe0 with fields of the record at 0x398, and that path returns 0 or 5. Other paths call `ibrick_mac_addr_get` at 0xc000000000091ee4 and `mactoh`, or pass the absolute address 0x9000000014000000 to `get_elsc` at 0xc000000000065240, then call `iobrick_get_sys_snum` at 0xc0000000000d1008 and `kopt_find` at 0xc0000000002c4890, with `cmn_err` calls on failure branches. Role (from name): it builds an Ethernet station address for the IOC3 interface by trying several sources. The facts show the calls and branches but not which source wins, so the fallback rules stay open.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.30/unix.IP35` at 0xc000000000294c48
  **Inferred.** `doacvec` (116 bytes) loads a byte at absolute address 0xffffffffffffc080 and, when it is nonzero, calls `panicspin` at 0xc0000000002af314 with a0 = 0x92000000017a0038. Otherwise it loads the doubleword at hub address 0x92000000017a0038 and makes the same `panicspin` call when bit 0 is set. When bit 0 is clear it reads a byte at 0xffffffffffffc081 and, when that is nonzero, stores bytes to 0xffffffffffffc04e and 0xffffffffffffc081. It then calls `tlbflush_rand` at 0xc000000000265ee4 with the word at 0xffffffffffffc084 when that word is nonzero, and calls `tlbflush` at 0xc000000000265dec with the doubleword at 0xffffffffffffc0d0 when that is nonzero, storing zero back to 0xffffffffffffc0d0 in the call's delay slot. Role (from name): the callee names suggest a TLB flush request guarded by a hub-side bit, with a panic path. The facts do not say what the vector is or what the flag bytes mean.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.30/unix.IP35` at 0xc0000000002e0310
  **Inferred.** `syssgi` (16704 bytes, the largest routine in this batch) calls `get_current_abi` first, then compares a doubleword loaded through its first argument against a chain of constants, including 0xab, 0xac, 0x16a, 0x427, 0x4c1 and 0x4d3, and calls helpers named `syssgi_*`, among them `syssgi_getjid`, `syssgi_makenewjob`, `syssgi_killjob`, `syssgi_tune` and `syssgi_nvram`. Along the way it calls `copyin` (19 call sites), `copyout` (29), `cap_able` (22) and `get_current_cred` (8). Role (from name): the comparison chain and the helper names suggest the entry point for a family of system calls. The facts do not show which constant selects which helper, so that mapping is open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/unix.IP35` at 0xc00000000047b788
  **Inferred.** `gfx_flow_delayed_hiwater_intr_prehandler` (148 bytes) loads a halfword from absolute address 0xffffffffffffc428 (at the start and again after the indirect call) and uses it to index the table at `gfx_flow_cpu_attachments`. When the table entry is zero it stores a doubleword of 0 to hub address 0x9200000001000000 and returns. When the entry is nonzero it loads fields at +0x20 and +0x28 of the entry and +0xf8 of its second argument and makes an indirect call whose callee the facts do not resolve. Role (from name): a prehandler for a high-water interrupt in the graphics flow-control path. The field meanings are not in the facts, so the purpose rests on the name.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.30/unix.IP35` at 0xc00000000047b98c
  **Inferred.** `gfx_flow_alloc` (592 bytes) tests a word at 0xc00000000163da20; when it is zero it branches straight to the `gfx_flow_init` call at 0xc00000000047b81c, which passes the routine's own four arguments through. Otherwise it calls `kmem_alloc` at 0xc00000000022c0f8 with size 0x70. When the allocation succeeds it calls `xwidget_info_get` and `xwidget_info_id_get`, stores fields at offsets 0x0 through 0x68 of the record, calls `kvalloc` on one path, and then calls `xwidget_info_master_get` and `master_node_get`. The return value is either 0 or the allocated record. Role (from name): a constructor for a per-widget graphics flow record. The field meanings are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.30/unix.IP35` at 0xc00000000047bbe8
  **Inferred.** `gfx_flow_dealloc` (72 bytes) reads a halfword at offset 0x50 and a word at 0x54 from its first argument, then calls `kmem_free` at 0xc00000000022c288 with that argument and the constant 0x70. The 0x70 matches the size `gfx_flow_alloc` requests, so this looks like its matching free path. The instruction listing shows no branch, so any check the code makes before the free is not visible here.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.30/unix.IP35` at 0xc00000000047bcc8
  **Inferred.** `gfx_flow_attach_me` (292 bytes) reads a halfword from absolute address 0xffffffffffffc428 and stores a halfword at offset 0x60 and a doubleword at 0x68 of its argument. It writes an entry into the per-CPU attachment table, writes three doublewords to hub address 0x9200000001000000, and reads table records the same way the prehandler does. The name suggests registering a device or CPU with a flow. The instruction listing shows the table and hub writes but not the fields they fill, and it does not resolve the return value.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.30/unix.IP35` at 0xc00000000047be3c
  **Inferred.** `gfx_flow_detach_me` (220 bytes) is the counterpart of `gfx_flow_attach_me`. It stores zero through the pointer at +0x58 of its argument, stores to the hub block at 0x9200000001000000 and to 0x9200000001000090, loads 0x9200000001000098, and stores zero into the attachment-table slot indexed by the halfword at 0xffffffffffffc428. It stores -1 to +0x60 and +0x68 of its argument. It returns the address of that slot (table base plus index times 8), not a value it read. The mirrored shape supports reading the pair as attach and detach, but the meaning of the 0x90 and 0x98 registers is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.30/usr/cpu/sysgen/IP27boot/cn.o` at 0xaac
  **Inferred.** `cn_init` (284 bytes) calls `gfx_earlyinit` first. It then loads the halfword at cn+0x48; when that is zero it calls `spinlock_init` and `init_mutex` on the `cn` symbol, stores `cn` into fields at +0x18 and +0x20, and calls `is_specified` on `arg_console`. When `is_specified` returns nonzero it compares the first byte of `arg_console` with the constants 0x67 and 0x47. The routine also calls `tp_init` at 0xb8c and `du_init` at 0xba8, with stores to the halfwords at cn+0x48 and cn+0x4a. The facts do not resolve which condition reaches which of those two calls. Role (from name): boot-time console setup. Which console each branch picks is not established.
  Reviewed: 2026-11-12 (sonnet)

- `bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** `bitlockdebug` (612 bytes) uses load-linked and store-conditional pairs on words at offsets 0xa8, 0xac, 0xb0, 0xb8 and 0xbc of its third argument, and it writes several doubleword and halfword fields of that structure. It calls `try_bitlock` at 0x16cc and again at 0x17f8. Before the second call, when its sixth argument (a5) is nonzero, it calls `splx` and then a callee the facts do not resolve. On one path it calls `cmn_err` with level 0, its first argument, and the doubleword read from hub address 0x9200000001030100. Role (from name): the debug variant of a bit lock that reports on failure. The structure layout is not named in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitlockdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** `_64bitlockdebug` (612 bytes) has the same shape as `bitlockdebug` above: the same load-linked and store-conditional offsets, the same hub word at 0x9200000001030100, and the same diagnostic print on one path. The callee differs: it calls `try_64bitlock` at 0x1f70 and 0x209c. The name suggests the 64-bit counterpart of the same debug lock, which fits the callee. The instruction listing does not show the lock layout, so the two are treated as siblings rather than verified equivalents.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitunlockdebug` in `6.5.30/usr/cpu/sysgen/IP27boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** `_64bitunlockdebug` (280 bytes) reads a halfword at offset 0x2a of its third argument and compares it with a halfword at absolute address 0xffffffffffffc420. When they are equal it stores to fields of the third argument, including a halfword at 0x2a and doublewords at 0x48, 0x58, 0x68, 0x78 and 0x80, and returns. When they differ it stores a doubleword at 0x40 and calls `cmn_err` at 0x22c0 with level 4, its first argument, the hub word at 0x9200000001030100 and the +0x40 field plus one. A second `cmn_err` call at 0x22d8 uses level 0 with its first argument and the same hub word. Offsets 0x2a and 0x40 also appear in the lock routines, which suggests shared fields, but the facts do not prove it. Role (from name): the unlock side of the debug lock pair, with logging. The field meanings are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x0
  **Inferred.** `gfx_flow_delayed_hiwater_intr_prehandler` (144 bytes, relocatable object copy) has the same shape as the kernel-image copy at 0xc00000000047b788. It reads halfwords from absolute address 0xffffffffffffc420, looks up entries through `gfx_flow_cpu_attachments` and `pdaindr`, stores a doubleword to hub address 0x9200000001000000, and makes an indirect call with arguments read from the attachment record. The halfword address differs from the kernel copy (c420 here, c428 there). That may reflect a different layout of the same data, and the facts do not settle it. The name suggests a high-water interrupt prehandler, but the callee is unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x23c
  **Inferred.** `gfx_flow_alloc` (916 bytes, object copy) allocates a 0x70-byte record with `kmem_alloc` and fills many fields at offsets 0x0 through 0x68. It resolves the widget chain through `xwidget_info_get`, `xwidget_info_id_get`, `xwidget_info_master_get` and `master_node_get`, sizes a buffer with `kvalloc`, and calls `gfx_flow_init` at 0x458. Unlike the kernel copy, it also reads the `compact_to_nasid_node` table and passes the value of `hub_use_transaction_credits` to a `cmn_err` call. The name matches the kernel copy. The purpose of the extra paths is not established.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x5dc
  **Inferred.** `gfx_flow_dealloc` (72 bytes, object copy) matches the kernel-image copy. It reads a halfword at offset 0x50 and a word at 0x54 from its first argument, then calls `kmem_free` at 0x610 with that argument and the size 0x70. The size matches the allocation in the copy above, so this is probably the matching free. The instruction listing shows no branch, so there is no visible failure path.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x6b8
  **Inferred.** `gfx_flow_attach_me` (344 bytes, object copy) matches the kernel-image attach routine in its table and hub writes. It stores the attachment-table slot and fields of its argument, reads the hub word at 0x9200000001600000, reads the `compact_to_nasid_node` table, and writes three doublewords to hub address 0x9200000001000000. A branch on an unresolved value compared unsigned against 3 skips those hub writes and goes to a call of `err_int_block_mask` at 0x7f8, with a0 = 0x8, its second argument, the attach count plus one, and the halfword from 0xffffffffffffc420. The kernel copy has no calls at all, so this call is new in the object copy. Role (from name): attaching may also block an error-interrupt source. The mask meaning is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.30/usr/cpu/sysgen/IP27boot/gfxs.a(gfx_flow.o)` at 0x85c
  **Inferred.** `gfx_flow_detach_me` (308 bytes, object copy) is the pair of the attach routine above. It stores zero through the pointer at +0x58 of its argument, writes the same hub block at 0x9200000001000000 and 0x9200000001000090, reads 0x9200000001000098 and 0x9200000001600000, clears the attachment-table slot, and stores -1 to +0x60 and +0x68 of its argument. A branch on an unresolved value compared unsigned against 3 goes to a call of `err_int_unblock_mask` at 0x970 with a0 = 0x8, a1 = 0x1, the halfword from 0xffffffffffffc420 and a computed mask. Role (from name): attach blocks and detach unblocks one error source. The mask arithmetic is not named, so the bit it toggles stays open.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.30/usr/cpu/sysgen/IP27boot/if_ef.o` at 0x73c
  **Inferred.** `ef_ioc3eaddr` (84 bytes, driver object copy) is a thin version of the kernel-image routine at 0xc0000000001785c0. It reads the doubleword at 0x398 of its first argument and calls `nic_get_eaddr` at 0x754 with values from that record. When `nic_get_eaddr` returns nonzero it reads the word at 0x38c and calls `cmn_err` with level 1 and a fixed message. It makes no other calls, so this copy has no brick or serial lookups. The return value is 0 or 5.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.30/usr/cpu/sysgen/IP27boot/ioc3_pckm.o` at 0x5c
  **Inferred.** `pckm_attach` (1332 bytes) calls `ioc3_subdev_enabled` with the constant 0x3 and branches on the result. It traverses two hardware-graph paths with `hwgraph_traverse`, calls `ioc3_write_ireg` with 0x400000 at the start and again near the end, and calls `ioc3_is_console`. It allocates three records with `kmem_zalloc` (0x68, 0x58 and 0x58 bytes), then calls `initkbdtype`, `set_ports`, `default_kbd_setting` and `default_ms_setting`. It sends 0xf5 and 0xf4 (keyboard side) and 0xf6 (mouse side) through `pckm_pollcmd`, each followed by a `data_check` against 0xfa whose result a branch tests. It also calls `hwgraph_char_device_add` and `device_inventory_add` for up to two devices, and calls `ioc3_intr_connect` with `pckm_intr`. Role (from name): attaches a keyboard and mouse controller behind the IOC3. The facts do not say what the command bytes mean.
  Reviewed: 2026-11-12 (sonnet)

- `eiattach` in `6.5.30/usr/cpu/sysgen/IP27boot/ioc3ei.o` at 0x4a8
  **Inferred.** `eiattach` (420 bytes) calls `ioc3_subdev_enabled` with the constant 0x7 and branches on the result. It traverses two graph paths, allocates a 0x38-byte record with `kmem_zalloc`, and fills its fields. It calls `spinlock_init`, allocates a 0x80000-byte buffer with `kvpalloc`, and calls `hwgraph_char_device_add`, `hwgraph_chmod` and `device_inventory_add`. It then calls `ioc3_is_console`; when that returns nonzero it calls `device_controller_num_set` with 1, `ei_make_hwgraph_nodes` and `ioc3_intr_connect` with `ioc3_ei_intr`, and otherwise it calls `device_controller_num_set` with -1. The return value is 0 or -1. Role (from name): attaches an IOC3 interface device, probably the Ethernet one. The `ei` prefix is not explained in the facts, so the Ethernet reading is an inference.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x15d0
  **Inferred.** `start` (544 bytes, IP27 boot kernel object) reads the hub word at 0x9200000001600000 and calls `mapped_kernel_setup_tlb` at 0x1618. It then jumps through a relocated target at `.text+5696` (the `jr` at 0x1638). The instruction listing reports that the code from 0x1640 to 0x17ec is not reached from the entry, so the tail is not on the static path. The name and the early TLB setup suggest a boot entry point.
  Reviewed: 2026-11-12 (sonnet)

- `get_nasid` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x44e4
  **Inferred.** `get_nasid` (28 bytes, IP27 boot object) reads one doubleword from hub address 0x9200000001600000 and returns, with the return value unresolved in the instruction listing. The same hub word is also read by `start` and by `is_fine_dirmode` in this image. The name says node id, but the facts show no field mask, so which bits carry the node number stays open.
  Reviewed: 2026-11-12 (sonnet)

- `get_slice` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x4500
  **Inferred.** `get_slice` (16 bytes) loads one doubleword from hub address 0x9200000001000020 and returns it, shifted by zero, so the value comes back unchanged. The name suggests a slice number, but the facts show no mask or field extraction. Which bits, if any, name a slice stays open.
  Reviewed: 2026-11-12 (sonnet)

- `is_fine_dirmode` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x4510
  **Inferred.** `is_fine_dirmode` (20 bytes) loads one doubleword from hub address 0x9200000001600000, the word `get_nasid` reads, and returns. The instruction listing leaves the return value unresolved, so the link between that load and the return is not shown. The name points at a directory-mode check, but no compare or flag test appears in the facts, so that reading stays unconfirmed.
  Reviewed: 2026-11-12 (sonnet)
