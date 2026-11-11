# Tier-1 routine readings, batch 3

These role statements cover 25 routines from the ranked tier-1 list, positions 75 to 99. Each one comes from the facts of its routine: registers, calls and constants at the addresses cited. Where the facts stop short of a meaning, the statement says so. The label is "established" only where the whole body is covered by the facts and the behavior is mechanical. Everything else is "inferred". The names come from the inventory's name column. The labels and statements describe software behavior in these binaries, not hardware behavior.

- `pckm_attach` in `6.5.22/unix.IP27` at 0xc000000000097c60
  **Inferred.** It allocates three blocks of 0x68, 0x58 and 0x58 bytes with `kmem_zalloc` (calls at 0xc000000000097cf4, 0xc000000000097d14 and 0xc000000000097d24), then gates the setup on `ioc3_subdev_enabled` (0xc000000000097c70) and `ioc3_is_console` (0xc000000000097ce0). It registers two character device nodes with `hwgraph_char_device_add` (0xc000000000097dbc, 0xc000000000097f14) and runs a short command sequence with `pckm_pollcmd` and `data_check`, using the bytes 0xf5, 0xf4 and 0xf6 with an expected 0xfa each time. Those byte values look like PS/2 keyboard and mouse commands, but that is inferred from the values alone. Both `ioc3_write_ireg` calls pass 0x400000 as the mask: the first, at 0xc000000000097ccc, passes 0x1 as the value, and the second, at 0xc0000000000980b8, passes 0x0 and takes a 0x0 or 0x80000 value in a5. What those writes do, and the meaning of the console branch, is not settled by the facts.

- `get_myid` in `6.5.22/unix.IP27` at 0xc00000000009a1b8
  **Established.** It loads one doubleword from the fixed address 0x9200000001000020 and returns it unchanged, with no calls. The body is four instructions and the facts cover all of them. What the value means is not shown: the same fixed address range is written by other routines in this batch, so it may be a node or processor identifier, but nothing here confirms that. The label covers the load and return; the meaning of the value is unknown.

- `cn_init` in `6.5.22/unix.IP27` at 0xc0000000000a320c
  **Inferred.** It calls `gfx_earlyinit` (0xc0000000000a3224) first, then clears and resets a set of 16-bit and 64-bit fields in the object at 0xc0000000014f5a68 and calls `spinlock_init` and `init_mutex` on two embedded locks. It checks whether the console argument is specified through `is_specified` (0xc0000000000a32bc), and compares the first byte of the console argument against 0x67 and 0x47, which look like ASCII lowercase and uppercase "g". After that it calls `tp_init` (0xc0000000000a32ec) and `du_init` (0xc0000000000a3308) on the two branches. The name suggests console setup, but the meaning of the 16-bit fields is unknown.

- `ef_ioc3eaddr` in `6.5.22/unix.IP27` at 0xc0000000000f1468
  **Inferred.** It reads a pointer at offset 0x398 of its first argument, then calls `nic_get_eaddr` (0xc0000000000f1480) with two fields of that pointed-to structure. On the path where `nic_get_eaddr` returns zero, it logs through `cmn_err` at level 1 (0xc0000000000f14ac). The routine returns either 0x0 or 0x5. Which path gives which value is not decoded, so the error reading of 0x5 is open. The name suggests it fetches an Ethernet address for an IOC3 network port.

- `doacvec` in `6.5.22/unix.IP27` at 0xc0000000001e1a50
  **Inferred.** It reads two bytes and a word from fixed low addresses (0xffffffffffffc080 and 0xffffffffffffc081). It writes some of those bytes back, then calls `panicspin` (0xc0000000001e1a60) if a condition on the bytes holds, and calls `tlbflush_rand` with argument 1 (0xc0000000001e1a88) and `tlbflush` (0xc0000000001e1a9c) on the other path. The routine's purpose is not clear from the name or from these facts. The condition that chooses between panic and flush is only partly decoded.

- `syssgi` in `6.5.22/unix.IP27` at 0xc0000000002312b0
  **Inferred.** It is a large dispatcher, 12104 bytes long. Its facts show calls grouped by area: job control (`syssgi_makenewjob`, `syssgi_joinjob`, `syssgi_killjob`, `syssgi_getjid`), timers and time-of-day (`nanotime`, `get_ust_nano`, `settimeofday`), file-handle conversion (`path_to_handle`, `open_by_handle`), XFS and XLV helpers, ACL and capability calls, and user copies (`copyin`, `copyout`). The mapping from the request number to each callee is not decoded here, so the routine can be described by area only. Grouping the callees does not establish which request does what.

- `dogetitimer` in `6.5.22/unix.IP27` at 0xc000000000258b9c
  **Inferred.** It raises the interrupt level with `spl65`, takes a mutex with `mutex_lock` and `mutex_unlock`, and calls `chktimeout_tick` and `tick_to_timeval` to turn a tick count into a timeval. `spl1` also appears among its callees, and where it is used is not decoded. It returns 0x0 on the main path. The name suggests the handler for the getitimer request. The facts show the locking and the conversion, but not the exact argument layout.

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.22/unix.IP27` at 0xc0000000003a815c
  **Inferred.** It reads a 16-bit index at 0xffffffffffffc420, indexes a table through a pointer that is itself loaded from 0xc0000000014cacc8, and writes a doubleword into the table at 0x9200000001000000. It then makes an indirect call through a register (`jalr` at 0xc0000000003a81d8) whose target cannot be resolved from the facts. The name suggests a prehandler for a high-water interrupt of a graphics flow. The indirect target is unknown, so what the handler actually does is open.

- `gfx_flow_alloc` in `6.5.22/unix.IP27` at 0xc0000000003a8398
  **Inferred.** It calls `kmem_alloc`, then looks up the widget and master node through `xwidget_info_get`, `xwidget_info_id_get`, `xwidget_info_master_get` and `master_node_get`. It allocates a buffer with `kvalloc`, initializes it with `gfx_flow_init`, and frees on failure with `kmem_free`, logging through `cmn_err`. It returns either 0x0 or the pointer from `kmem_alloc`. The name suggests it creates a per-flow object tied to a graphics widget, but the facts do not show what the object holds.

- `gfx_flow_dealloc` in `6.5.22/unix.IP27` at 0xc0000000003a8738
  **Inferred.** It reads a halfword at offset 0x50 and a word at offset 0x54 of its first argument, then calls `kmem_free` with the first argument and size 0x70 (112 bytes) at 0xc0000000003a876c. It returns the result of `kmem_free`. The two field loads are not visibly used in the facts, so the code is only partly explained.

- `gfx_flow_attach_me` in `6.5.22/unix.IP27` at 0xc0000000003a8814
  **Inferred.** It stores a halfword and a doubleword into fields of its first argument (offsets 0x60 and 0x68), writes a doubleword into the table at 0x9200000001000000, and calls `err_int_block_mask` with argument 0x8 at 0xc0000000003a8954. It returns either the table slot or the result of that call. The name suggests attaching a flow to an error-interrupt block. The meaning of the argument 0x8 and of the mask is not confirmed by the facts.

- `gfx_flow_detach_me` in `6.5.22/unix.IP27` at 0xc0000000003a89b8
  **Inferred.** It is the mirror of the attach path: it clears the flow link, stores to 0x9200000001000090, and calls `err_int_unblock_mask` with argument 0x8 at 0xc0000000003a8acc. The routine passes the constant 0xdeadbabe as an argument, which reads like a sentinel value; that is inferred from the constant. It returns either the value at 0x9200000001000098 or the result of the unblock call. The reason for the sentinel is not shown.

- `_get_timestamp` in `6.5.22/unix.IP30` at 0xa8000000200050a8
  **Established.** It loads one zero-extended word from 0xffffffffaff20004 and returns it. The body is three instructions and the facts cover all of them. The routine `_ticksper1024inst` reads the neighboring address 0xffffffffaff20000, which suggests a counter block, but the device behind it is not named by the facts. The label covers the load and return; the source of the counter is inferred.

- `getcpuid` in `6.5.22/unix.IP30` at 0xa8000000200050b4
  **Established.** It loads one doubleword from 0xffffffffaff50000 and returns it. The body is three instructions and the facts cover all of them. `bootstrap` calls it at 0xa800000020005210, which suggests it returns the id of the running processor, but the facts do not decode the value. The label covers the load and return; the meaning of the value is inferred from the caller.

- `bootstrap` in `6.5.22/unix.IP30` at 0xa8000000200051b4
  **Inferred.** It calls `getcpuid` (0xa800000020005210) and then `tlbwired` (0xa800000020005270) to install a wired TLB entry, and it writes a byte and a word to fixed low addresses. It ends with a jump to `cboot` at 0xa800000020005288 rather than a return. It reads as an early boot routine that sets up a wired mapping and hands off. Which mapping `tlbwired` installs is only partly decoded from the argument registers.

- `_ticksper1024inst` in `6.5.22/unix.IP30` at 0xa800000020005370
  **Inferred.** It reads doublewords from 0xffffffffaff20000 twice (at 0xa800000020005380 and 0xa800000020005398) and runs two signed-greater-than-zero loops around those reads. It returns a value the facts do not decode. The name suggests a calibration of counter ticks per 1024 instructions, but the loop bounds and the return value are unresolved.

- `start` in `6.5.22/unix.IP30` at 0xa800000020005840
  **Inferred.** From its entry, the routine makes one register-constant jump to `start+72` (0xa800000020005888, via `jr` at 0xa800000020005884). The facts say the remaining 436 bytes are not reached from the entry by any path. The routine is therefore a stub that jumps into its own body, and the reached part of the routine is open. The name suggests the entry point of the boot code, but nothing here shows that.

- `silence_all_audio` in `6.5.22/unix.IP30` at 0xa800000020005a9c
  **Inferred.** It reads a word from 0xa8000000203e1410, which the symbol table names `is_octane_lx`. When that word is nonzero, it writes a word to 0x900000001f600038 (at 0xa800000020005ad8). It then tail-jumps to `ip30_rad_halt` (0xa800000020007fe4). The name suggests muting audio, and the address written may be an audio control register. The facts do not confirm what that address is, and they do not show what `ip30_rad_halt` does.

- `mlreset` in `6.5.22/unix.IP30` at 0xa8000000200061d0
  **Inferred.** Its calls, in the facts, cover the performance-counter setup (`r1nkperf_control_register_set` and `r1nkperf_data_register_set`), interrupt setup (`heart_mp_intr_init`, `get_cpu_irr`), TLB and cache flushing (`flush_tlb`, `flush_cache`), board and system identification (`init_board_rev`, `init_sysid`), the NVRAM table (`load_nvram_tab`), and the chip reset `ip30_chips_mlreset` (0xa8000000200072e0). It returns the result of `flush_cache` or `cpu_enabled`. This is a board-level bring-up sequence. The order of the calls and the branch conditions were not decoded in this entry, so the sequence is described from the callee list only.

- `set_leds` in `6.5.22/unix.IP30` at 0xa800000020006590
  **Inferred.** It takes a spinlock at 0xa8000000203e1408 with `mutex_spinlock`, writes one of two word pairs to 0x900000001f600040 and 0x900000001f600044 depending on whether its first argument is zero, releases the lock with `mutex_spinunlock`, and then calls `flushbus` (0xa8000000200065d0). It returns the result of `flushbus`. The name suggests setting the front-panel or status LEDs. The two words written are not identified by the facts.

- `cpu_disable` in `6.5.22/unix.IP30` at 0xa800000020006784
  **Inferred.** It reads a doubleword from 0xffffffffaff00000, writes a doubleword back to the same address, and stores a word into the table at 0xa8000000203e1400. It returns a value the facts do not decode. The name suggests clearing a processor's enable bit, but which bit is cleared, and whether the table write marks the processor as disabled, is not shown.

- `cpu_exists` in `6.5.22/unix.IP30` at 0xa8000000200067b4
  **Inferred.** It loads one doubleword from 0xffffffffaff00080 and returns it. The return value is not decoded by the facts. The name suggests a presence check, but whether the caller treats the value as a boolean or as a mask is open.

- `ip30_chips_mlreset` in `6.5.22/unix.IP30` at 0xa8000000200072e0
  **Inferred.** It calls `xtalk_set_early_piotrans_addr` with the address of `ip30_early_piotrans_addr` (0xa800000020007434), then runs the chip resets `heart_mlreset`, `xbow_mlreset` and `ioc3_mlreset`. It then preallocates and connects interrupt slots with `heart_intr_prealloc` and `heart_intr_preconn` (the first argument values 0x3a, 0x39, 0x11, 0x10 and 0x29 vary per call). It returns the result of the last `heart_intr_preconn` (at 0xa80000002000742c). The name and callees suggest the chip-level reset sequence for the IP30 board, but the meaning of the interrupt argument values is inferred from their position only.

- `ip30_early_piotrans_addr` in `6.5.22/unix.IP30` at 0xa800000020007434
  **Inferred.** It reads a doubleword from 0xffffffffaff00080 and several words from the fixed windows at 0x9000000010000004 and 0x9000000000000000 plus a table entry, then runs a loop of comparisons and branches. It calls `heart_addr_xio_to_kva` three times (at 0xa80000002000751c, 0xa800000020007560 and 0xa800000020007578). It returns either that call's result or 0x0. The name suggests it computes an early programmed-I/O translation address. The loop conditions are not fully decoded, so the exact translation is open.

- `sendintr` in `6.5.22/unix.IP30` at 0xa8000000200077ac
  **Established.** It stores one doubleword to 0xffffffffaff10020 and returns 0x0. The body is three instructions and the facts cover all of them. The meaning of the register at that address is not shown, so the routine is described only as a write and a success return. The label covers the store and return; the register's function is unknown.
