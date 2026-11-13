# Tier-1 routine readings, batch 3

These role statements cover 25 routines from the ranked tier-1 list, positions 75 to 99. Each one comes from the facts of its routine: registers, calls and constants at the addresses cited. Where the facts stop short of a meaning, the statement says so. The label is "established" only where the whole body is covered by the facts and the behavior is mechanical. Everything else is "inferred". The names come from the inventory's name column. The labels and statements describe software behavior in these binaries, not hardware behavior.

- `pckm_attach` in `6.5.22/unix.IP27` at 0xc000000000097c60
  **Inferred.** It first calls `ioc3_subdev_enabled` (0xc000000000097c70) and branches to 0xc0000000000980e4 when that returns zero. It calls `ioc3_is_console` (0xc000000000097ce0), whose result gates several later branches. It allocates three blocks of 0x68, 0x58 and 0x58 bytes with `kmem_zalloc` (calls at 0xc000000000097cf4, 0xc000000000097d14 and 0xc000000000097d24). Depending on branches on fields of the first block, it can register two character device nodes with `hwgraph_char_device_add` (0xc000000000097dbc, 0xc000000000097f14) and send commands with `pckm_pollcmd`, checking each reply with `data_check` against 0xfa. The command bytes are 0xf5 and 0xf4 on the first of those paths and 0xf6 on the second. Those byte values look like PS/2 keyboard and mouse commands, but that is inferred from the values alone. Both `ioc3_write_ireg` calls pass 0x400000 as their second argument. The first, at 0xc000000000097ccc, passes 0x1 as the third argument. The second, at 0xc0000000000980b8, passes 0x0 as the third and a combination of 0x0 or 0x40000 with 0x0 or 0x80000 as the fifth (a4). What those writes do, and the meaning of the console branch, is not settled by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `get_myid` in `6.5.22/unix.IP27` at 0xc00000000009a1b8
  **Established.** It loads one doubleword from the fixed address 0x9200000001000020 and returns it unchanged, with no calls. The label covers the load and return. What the value means is not shown by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.22/unix.IP27` at 0xc0000000000a320c
  **Inferred.** It calls `gfx_earlyinit` (0xc0000000000a3224) first. A halfword at 0xc0000000014f5aa8 then selects between two paths. When it is zero, the routine calls `spinlock_init` (0xc0000000000a328c) on 0xc0000000014f5ab4 and `init_mutex` (0xc0000000000a32a4) on 0xc0000000014f5a88, then calls `is_specified` (0xc0000000000a32bc) on the string at 0xc000000001516560. If that returns nonzero it compares the argument's first byte with 0x67 and 0x47, which are ASCII lowercase and uppercase 'g' by value, and calls `tp_init` (0xc0000000000a32ec) when it matches neither. What happens on a match is not decoded. When the halfword is nonzero and a0 is nonzero, it calls `vn_rele` on the pointer at 0xc0000000014f5a68 if that pointer is nonzero, and clears it. It then reaches `tp_init` when a byte at 0xc0000000014d15f0 is zero, and otherwise `du_init` (0xc0000000000a3308) when a1 is zero. It stores 16-bit and 64-bit values to fields whose targets the facts do not show, and returns the result of `tp_init` or `du_init`, or a value that is not decoded. The name suggests console setup, but the meaning of the fields is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.22/unix.IP27` at 0xc0000000000f1468
  **Inferred.** It loads the pointer at offset 0x398 of its first argument, then calls `nic_get_eaddr` (0xc0000000000f1480) with pointers 0x30 and 0x34 bytes into the structure that pointer addresses as the first two arguments, passing its own second and fourth arguments through as the third and fourth. When `nic_get_eaddr` returns nonzero it logs through `cmn_err` at level 1 (0xc0000000000f14ac), with a word at offset 0x38c of its first argument as one argument. The routine returns either 0x0 or 0x5. The facts do not decode which path gives which value, so reading 0x5 as an error is open. The name suggests it fetches an Ethernet address for an IOC3 network port.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.22/unix.IP27` at 0xc0000000001e1a50
  **Inferred.** It reads a byte from 0xffffffffffffc080, a byte from 0xffffffffffffc081, a word from 0xffffffffffffc084 and a doubleword from 0xffffffffffffc0a0. When the first byte is nonzero it calls `panicspin` (0xc0000000001e1a60). When the second byte is nonzero it stores two bytes, whose targets the facts do not show. When the word is nonzero it calls `tlbflush_rand` with argument 1 (0xc0000000001e1a88). When the doubleword is nonzero it calls `tlbflush` (0xc0000000001e1a9c) with that doubleword as its argument, then stores a doubleword. It returns the result of `tlbflush` or a value that is not decoded. The routine's purpose is not clear from the name or from these facts. Whether `panicspin` returns to the later checks is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.22/unix.IP27` at 0xc0000000002312b0
  **Inferred.** It is a large dispatcher, 12104 bytes long. Its facts show calls grouped by area: job control (`syssgi_makenewjob`, `syssgi_joinjob`, `syssgi_killjob`, `syssgi_getjid`), timers and time-of-day (`nanotime`, `get_ust_nano`, `settimeofday`), file-handle conversion (`path_to_handle`, `open_by_handle`), XFS and XLV helpers, ACL and capability calls, and user copies (`copyin`, `copyout`). The mapping from the request number to each callee is not decoded here, so the routine can be described by area only. Grouping the callees does not establish which request does what.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.22/unix.IP27` at 0xc000000000258b9c
  **Inferred.** The first branch tests its first argument. When it is zero, the routine raises the interrupt level with `spl65`, takes a mutex with `mutex_lock`, copies fields at offsets 0x840 to 0x858, releases the mutex with `mutex_unlock`, and later restores the level with `splx`. On that path it also calls `chktimeout_tick` (0xc000000000258c38 or 0xc000000000258cd0) and passes its result to `tick_to_timeval` (0xc000000000258c54). When the argument is nonzero, it raises the level with `spl1`, copies fields at offsets 0x348 to 0x360 and restores the level with `splx`. Its only return is 0x0. The name suggests the handler for the getitimer request. The facts show the locking and the conversion calls, but not the exact argument layout.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/unix.IP27` at 0xc0000000003a815c
  **Inferred.** It reads a 16-bit value at 0xffffffffffffc420, indexes a table of doublewords through a pointer loaded from 0xc0000000014cacc8, and tests the entry. When the entry is nonzero it makes an indirect call through a register (`jalr` at 0xc0000000003a81d8), passing the entry's fields at offsets 0x28 and 0x20 as the first and third arguments and the entry itself as the fourth. The target cannot be resolved from the facts. On the path where the entry is zero it stores a doubleword whose target the facts do not show. The return value is not decoded. The name suggests a prehandler for a high-water interrupt of a graphics flow. The indirect target is unknown, so what the handler actually does is open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.22/unix.IP27` at 0xc0000000003a8398
  **Inferred.** On the paths where its guard conditions on the globals at 0xc0000000014cacb8 and 0xc0000000014cacb0 pass, it calls `kmem_alloc` with size 0x70, then looks up the widget and master node through `xwidget_info_get`, `xwidget_info_id_get`, `xwidget_info_master_get` and `master_node_get`. It calls `kvalloc` once, and calls `gfx_flow_init` with its own first four arguments passed through. A separate path logs through `cmn_err` five times and releases the 0x70-byte block with `kmem_free`. It returns either 0x0 or the pointer from `kmem_alloc`. The name suggests it creates a per-flow object tied to a graphics widget. The facts do not show what the object holds, what the `kvalloc` buffer is used for, or what `gfx_flow_init` operates on.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.22/unix.IP27` at 0xc0000000003a8738
  **Inferred.** It loads a halfword at offset 0x50 and a word at offset 0x54 (the base register is not shown), calls `kmem_free` (0xc0000000003a876c) with its first argument and size 0x70 (112 bytes), stores a doubleword whose target the facts do not show, and returns the result of `kmem_free`. The two loads are not visibly used in the facts, so the code is only partly explained.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.22/unix.IP27` at 0xc0000000003a8814
  **Inferred.** It stores a halfword at offset 0x60 and a doubleword at offset 0x68 (the base register is not shown), and does further loads and doubleword stores whose targets the facts do not show. On the path where an unsigned comparison is below 0x3, it calls `err_int_block_mask` (0xc0000000003a8954) with first argument 0x8, a1 passed through, the value at 0xc0000000014cacc0 plus 1, and the 16-bit value at 0xffffffffffffc420. It returns either an address in the table indexed by that 16-bit value and the pointer at 0xc0000000014cacc8, or the result of that call. The name suggests attaching a flow to an error-interrupt block. The meaning of the argument 0x8 and of the mask is not confirmed by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.22/unix.IP27` at 0xc0000000003a89b8
  **Inferred.** It stores doublewords, a word at offset 0x14 and a halfword at offset 0x60, whose targets the facts mostly do not show. On the path where an unsigned comparison is below 0x3, it calls `err_int_unblock_mask` (0xc0000000003a8acc) with first argument 0x8, passing the constant 0xdeadbabe as a5 and 0x9200000001000000 as a4. The constant reads like a sentinel value, which is inferred from the constant alone. It returns either the doubleword at 0x9200000001000098 or the result of the unblock call. The name and the shape suggest a counterpart of the attach routine. The reason for the sentinel is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `_get_timestamp` in `6.5.22/unix.IP30` at 0xa8000000200050a8
  **Established.** It loads one zero-extended word from 0xffffffffaff20004 and returns it. The body is three instructions and the facts cover all of them. The label covers the load and return. What the value counts and where it comes from is not shown by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.22/unix.IP30` at 0xa8000000200050b4
  **Established.** It loads one doubleword from 0xffffffffaff50000 and returns it. The body is three instructions and the facts cover all of them. `bootstrap` calls it at 0xa800000020005210, which suggests it returns the id of the running processor, but the facts do not decode the value. The label covers the load and return; the meaning of the value is inferred from the caller.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.22/unix.IP30` at 0xa8000000200051b4
  **Inferred.** It calls `getcpuid` (0xa800000020005210) and then `tlbwired` (0xa800000020005270) with constant arguments 0x0, 0x0, -0x4000, 0x1 and 0x6000 in a0, a1, a2, a3 and a5. By its name `tlbwired` installs a wired TLB entry. It also stores a doubleword, a byte and a word whose targets the facts do not show, and ends with a tail jump to `cboot` at 0xa800000020005288 rather than a return. It reads as an early boot routine that hands off, but that comes from the names and the shape. What `tlbwired` installs is only partly decoded from the argument registers.
  Reviewed: 2026-11-12 (sonnet)

- `_ticksper1024inst` in `6.5.22/unix.IP30` at 0xa800000020005370
  **Inferred.** It loads doublewords at 0xa800000020005380 and 0xa800000020005398 (the address read is not shown by the facts) and runs two loops that branch on a signed greater-than-zero test, at 0xa800000020005388 and 0xa800000020005390. It returns a value the facts do not decode. The name suggests a calibration of counter ticks per 1024 instructions, but the loop bounds and the return value are unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.22/unix.IP30` at 0xa800000020005840
  **Inferred.** The routine is 436 bytes long. From its entry, the facts show one register-constant jump to `start+72` (0xa800000020005888, via `jr` at 0xa800000020005884) and no other reached instruction. The facts do not say what the rest of the routine does or whether other paths reach it, so what the routine does after that jump is open. The name suggests the entry point of the boot code, but nothing here shows that.
  Reviewed: 2026-11-12 (sonnet)

- `silence_all_audio` in `6.5.22/unix.IP30` at 0xa800000020005a9c
  **Inferred.** It loads a word from 0xa8000000203e1410. When that word is nonzero it stores a word at 0xa800000020005ad8, whose target the facts do not show. On both paths it then tail-jumps to `ip30_rad_halt` (0xa800000020007fe4). The name suggests muting audio. The facts do not confirm what the stored-to address is or what `ip30_rad_halt` does.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.22/unix.IP30` at 0xa8000000200061d0
  **Inferred.** Its calls, in the facts, include `r1nkperf_control_register_set` and `r1nkperf_data_register_set`, `heart_mp_intr_init` and `get_cpu_irr`, `flush_tlb` and `flush_cache`, `init_board_rev`, `init_sysid`, `load_nvram_tab`, and `ip30_chips_mlreset` (0xa8000000200072e0). Grouped by callee name, these cover performance-counter setup, interrupt setup, TLB and cache flushing, board and system identification and the NVRAM table. Several of them sit on conditional paths. It returns the result of `flush_cache` or `cpu_enabled`. Role (from name): a machine-level reset hook for the board. The order of the calls and the branch conditions were not decoded in this entry, so the sequence is described from the callee list only.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/unix.IP30` at 0xa800000020006590
  **Inferred.** It takes a spinlock at 0xa8000000203e1408 with `mutex_spinlock`, branches on whether its first argument is zero, and stores two words on each path. The store targets are not shown, but the registers hold 0x900000001f600044 and 0x900000001f600040 when the lock is released. It then releases the lock with `mutex_spinunlock`, calls `flushbus` (0xa8000000200065d0) and returns its result. The name suggests setting the front-panel or status LEDs. The two words written are not identified by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_disable` in `6.5.22/unix.IP30` at 0xa800000020006784
  **Inferred.** It loads a doubleword, stores a doubleword and stores a word. The facts do not show the addresses. It returns a value the facts do not decode. The name suggests clearing a processor's enable bit, but which bit is cleared, and whether the word store marks the processor as disabled, is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_exists` in `6.5.22/unix.IP30` at 0xa8000000200067b4
  **Inferred.** It loads one doubleword (the address is not shown by the facts) and returns a value the facts do not decode. The name suggests a presence check, but whether the caller treats the value as a boolean or as a mask is open.
  Reviewed: 2026-11-12 (sonnet)

- `ip30_chips_mlreset` in `6.5.22/unix.IP30` at 0xa8000000200072e0
  **Inferred.** It calls `xtalk_set_early_piotrans_addr` with the address of `ip30_early_piotrans_addr` (0xa800000020007434), then runs the chip resets `heart_mlreset`, `xbow_mlreset` and `ioc3_mlreset`. It then preallocates and connects interrupt slots with `heart_intr_prealloc` and `heart_intr_preconn`. Their second argument takes the values 0x3a, 0x39, 0x11, 0x10, 0x39 and 0x29 across the six `heart_intr_prealloc` calls and 0x11, 0x10 and 0x29 across the three `heart_intr_preconn` calls. It returns the result of the last `heart_intr_preconn` (at 0xa80000002000742c). The name and callees suggest the chip-level reset sequence for the IP30 board, but the meaning of the interrupt argument values is inferred from their position only.
  Reviewed: 2026-11-12 (sonnet)

- `ip30_early_piotrans_addr` in `6.5.22/unix.IP30` at 0xa800000020007434
  **Inferred.** It loads a doubleword and several words from addresses the facts do not show, then runs a loop of comparisons and branches, one of which tests bit 0x20 of a loaded word. It calls `heart_addr_xio_to_kva` three times (at 0xa80000002000751c, 0xa800000020007560 and 0xa800000020007578). It returns either the result of one of those calls or 0x0. The name suggests it computes an early programmed-I/O translation address. The loop conditions are not fully decoded, so the exact translation is open.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.22/unix.IP30` at 0xa8000000200077ac
  **Inferred.** It stores one doubleword and returns 0x0. The routine is 28 bytes long, and the facts show the store (whose target they do not show) and the return. The meaning of the stored value and of the register written is not shown, so the routine is described only as a write and a success return. The name suggests sending an interrupt.
  Reviewed: 2026-11-12 (sonnet)
