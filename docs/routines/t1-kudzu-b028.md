# Tier-1 kudzu routine roles, batch 28

Role statements for 25 routines from two shipped kernel images in the 6.5.30 tree: `6.5.30/unix.IP30` (routines 1 to 5) and `6.5.30/unix.IP35` (routines 6 to 25). They come from the instruction facts only: register uses, loads and stores with fixed addresses, calls with their constant arguments, and branch conditions. Nothing here was run on a machine.

Four entries are left out because their statements cite nothing beyond the routine's own name, or name a routine the symbol table gives differently at that address.

Grades are inferred, because the facts show control flow and callees but not the intent behind the hub register accesses or the callee contracts. One entry, `is_fine_dirmode`, is graded unknown because the facts do not resolve its return value. Where a meaning rests on a name alone, the entry says so. Addresses such as `0x9200000001680010` are hub register addresses as the code uses them. `0xffffffffffffc3a0`-style addresses are halfword globals read by several routines. `table(...)` marks a per-node table pointer.

- `gfx_flow_pipe_head_init` in `6.5.30/unix.IP30` at 0xa80000002038f270
  **Inferred.** `gfx_flow_pipe_head_init` writes a block of fields at its argument, loading some of the values from the argument at offsets 0x8 to 0x20 and storing them into global table slots and fixed addresses such as `0xffffffffaff00040`. It brackets part of that work with `mutex_spinlock` (`0xa80000002022d954`) and `mutex_spinunlock` (`0xa80000002022d9a0`) on the lock at `0xa8000000204b0024`. The purpose of each field is not established by these facts, and the lock's scope is inferred from the call order.

- `gfx_flow_alloc` in `6.5.30/unix.IP30` at 0xa80000002038f318
  **Inferred.** `gfx_flow_alloc` allocates a 0x68-byte record with `kmem_alloc` (`0xa800000020148580`) and fills about twenty of its fields at offsets 0x0 to 0x60. It takes the lock at `0xa8000000204a8628` around the bookkeeping, looks up a widget through `xwidget_info_get` (`0xa80000002003acd0`) and `xwidget_info_id_get` (`0xa80000002003adb4`), and calls `kvalloc` (`0xa800000020170c9c`). It then calls `gfx_flow_pipe_head_init` on the new record, `gfx_flow_init` (`0xa80000002038f230`) and `heart_flow_control_connect` (`0xa800000020040208`). That the allocation is the counterpart of `gfx_flow_dealloc` rests on the matching 0x68 size.

- `gfx_flow_dealloc` in `6.5.30/unix.IP30` at 0xa80000002038f524
  **Inferred.** `gfx_flow_dealloc` reads a halfword at offset 0x5c and a word at offset 0x58 of its argument, and on one check it calls `cmn_err` (`0xa8000000201c6b00`) with message code 4. It takes both locks, calls `kmem_free` (`0xa800000020148710`) on the argument with size 0x68, and on a path gated by a value at `s0` being below 2 it calls `heart_flow_control_disable` (`0xa800000020040330`) and `heart_flow_control_disconnect` (`0xa80000002004028c`). The role is the teardown of the record that `gfx_flow_alloc` makes; the meaning of the error check is open.

- `gfx_flow_attach_me` in `6.5.30/unix.IP30` at 0xa80000002038f6dc
  **Inferred.** `gfx_flow_attach_me` takes the lock at `0xa8000000204b0024` and, under it, updates the argument's fields at offsets 0x58, 0x5c and 0x60, then sets a field in the shared table. It calls `heart_flow_control_enable` (`0xa8000000200402f4`) with the value at offset 0x58 of its argument. It reads as the attach side of `gfx_flow_detach_me`, but the entries that the table slot stands for are not established.

- `gfx_flow_detach_me` in `6.5.30/unix.IP30` at 0xa80000002038f7c0
  **Inferred.** `gfx_flow_detach_me` takes the same lock as the attach routine, reads the word at offset 0x58, and clears or rewrites the fields at offsets 0x5c and 0x60. It calls `heart_flow_control_disable` (`0xa800000020040330`) with the value at offset 0x58 of its argument. The pairing with `gfx_flow_attach_me` rests on the shared field offsets and the lock.

- `restart` in `6.5.30/unix.IP35` at 0xc000000000041240
  **Inferred.** `restart` reads and writes several halfword and doubleword fields in a loop over the node count held at `0xc000000001645f9c`, and it stores to the hub addresses `0x9200000001680010` and `0x9200000001680008`. It calls `us_delay` (`0xc000000000069a4c`) with 1000000 and 3000000 as the delay argument, `get_elsc` (`0xc000000000065240`), `sc_power_down_all` (`0xc0000000000d0190`), `ni_reset_port` (`0xc000000000041534`), `elsc_init` (`0xc0000000000ce8d8`) and `elsc_system_reset` (`0xc0000000000cfdc4`). A negative return from the reset call leads to a `printf` (`0xc0000000002adeec`). The overall shape is a system power-down and reset sequence; the order of the controller calls is read from the call graph, and the reset target is not established.

- `is_fine_dirmode` in `6.5.30/unix.IP35` at 0xc000000000041460
  **Unknown.** `is_fine_dirmode` loads one doubleword from the hub address `0x9200000001600000` and returns a value the facts do not resolve. Its caller `mlreset` (`0xc00000000004d94c`) calls it and keeps the result. The name suggests a directory-mode check, but nothing in the instruction listing establishes that.

- `ni_reset_port` in `6.5.30/unix.IP35` at 0xc000000000041534
  **Inferred.** `ni_reset_port` stores two doublewords to the hub addresses `0x9200000001680010` and `0x9200000001680008`, and returns the second address. The stored values do not appear in the facts, so the effect on the port is open. It is called from `restart` (`0xc000000000041240`), which fits a port-reset role.

- `start` in `6.5.30/unix.IP35` at 0xc000000000041b10
  **Inferred.** `start` loads a doubleword from the hub address `0x9200000001600000` and calls `mapped_kernel_setup_tlb` (`0xc000000000065538`). It then jumps through a register constant at `0xc000000000041b78` to `0xc000000000041b80`. The facts do not reach that target from the entry, so the rest of the 544-byte routine is not analysed. The first part looks like an early kernel start path that maps the kernel's TLB before handing off.

- `mlreset` in `6.5.30/unix.IP35` at 0xc00000000004d94c
  **Inferred.** `mlreset` is the largest routine in this batch, at 1152 bytes. It programs performance-counter registers for counters 0 to 3 through `r1nkperf_control_register_set` (`0xc000000000063648`) and `r1nkperf_data_register_set` (`0xc0000000000635e4`), reads CPU interrupt state twice with `get_cpu_irr` (`0xc000000000064478`), then flushes the TLB and cache. It calls the per-device resets `ioc3_mlreset` (`0xc0000000000c4ef4`), `ioc4_mlreset` (`0xc0000000000c2db0`), `l1_mlreset` (`0xc000000000228104`) and `nvram_baseinit` (`0xc00000000006e9a4`), and walks the node and CPU topology through `cpu_node_probe`, `gen_region_mask`, `setup_replication_mask` and `mem_init`, storing results to `boot_cnodemask` (`0xc0000000016b2510`). Three `assfail` calls (`0xc0000000000619f4`) with line numbers 0x119, 0x11c and 0x12e mark consistency checks. The routine looks like the machine-level reset and init pass, but the order of steps comes from the call graph and the counter setup's purpose is not established.

- `getcpuid` in `6.5.30/unix.IP35` at 0xc00000000004e898
  **Inferred.** `getcpuid` reads a doubleword from the hub address `0x9200000001000020`, passes it to `nasid_slice_to_cpuinfo` (`0xc000000000044f8c`) together with the NASID from `get_nasid` (`0xc000000000041434`), and returns a halfword at offset 0x1e of the record it gets back. The call is wrapped in `spl1` (`0xc000000000061f6c`) and `splx` (`0xc000000000062054`). A null record hits `assfail` (line 0x535). The name and the return field are read as CPU identity lookup; the field's meaning is inferred from the name.

- `save_cache_error` in `6.5.30/unix.IP35` at 0xc000000000052a08
  **Inferred.** `save_cache_error` reads a fixed-address doubleword from `table(0x9000000034000000)` and several fields from a per-node table at `0x9600000000000000 | 0x2000`. It reads the hub address `0x9200000001000020`, calls `get_nasid` (`0xc000000000041434`) and `chk_for_pebrick` (`0xc000000000046384`), and stores a few doublewords, a word and a byte into a destination whose address the facts do not resolve. The name indicates it copies cache-error state into a save area. The destination and the field meanings are open.

- `save_hub_pi_error` in `6.5.30/unix.IP35` at 0xc0000000000537ac
  **Inferred.** `save_hub_pi_error` clears 0x60 bytes at its second argument with `blkclr` (`0xc00000000004791c`) and copies several 8-byte fields from the first argument into the second, reading source offsets 0x400, 0x50, 0x58, 0x430, 0x440, 0x450, 0x460, 0x4c8 and 0x4d0. Some copies are gated by bit tests on the source fields at offsets 0x50, 0x58, 0x430, 0x450 and 0x4c8. The name suggests a copy of the hub processor-interface error registers. The field names and gate meanings are not established.

- `save_hub_md_error` in `6.5.30/unix.IP35` at 0xc000000000053884
  **Inferred.** `save_hub_md_error` clears 0x58 bytes at its second argument with `blkclr` and copies five fields from the first argument, at offsets 0x7c0010, 0x7e0010, 0x7c0020, 0x780040 and 0x780048, into the second at offsets 0x8 to 0x40. Bit tests on the values at the first argument's offsets 0x780000 and 0x7c0010 decide which copies run and whether a loop repeats. The name suggests memory-directory error state. The offsets in the first argument are large and unexplained by these facts.

- `save_hub_io_error` in `6.5.30/unix.IP35` at 0xc0000000000539e8
  **Inferred.** `save_hub_io_error` clears 0xd0 bytes at its second argument, copies seven doublewords from the first argument at offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008 and 0x420010 into the second, and then calls `save_hub_io_crb` (`0xc000000000053970`) and `save_hub_io_ieph` (`0xc0000000000539cc`). The name suggests I/O error capture. The two sub-saver calls are named but not analysed here.

- `save_hub_ni_error` in `6.5.30/unix.IP35` at 0xc000000000053b44
  **Inferred.** `save_hub_ni_error` clears 0x30 bytes at its second argument and copies four doublewords from the first argument at offsets 0x680100 to 0x680118 into the second at offsets 0x8 to 0x20. A test on the value at the first argument's offset 0x680100 gates the copy. The name suggests the network-interface error capture, the sibling of `save_hub_io_error` and `save_hub_pi_error`.

- `check_ni_errors_nasid` in `6.5.30/unix.IP35` at 0xc000000000058ba4
  **Inferred.** `check_ni_errors_nasid` reads several per-node error fields from addresses derived from its argument, and the hub addresses `0x9200000001680108`, `0x9200000001680110` and `0x9200000001680118`. It calls `part_id` (`0xc000000000496400`) and `ni_port_down_reset` (`0xc000000000058af4`) on several paths, logs through `cmn_err` many times with message codes 0x14 and 0x15, and calls `protected_broadcast` (`0xc000000000060c04`) with codes 0x67 and 0x68. The per-port state, read from a register at `s0`, is tested with masks that select one 4-bit field per port, so the routine reads as a check that resets ports that are down and logs each state. The field meanings and the broadcast codes are open.

- `klsave_spool_registers` in `6.5.30/unix.IP35` at 0xc000000000059acc
  **Inferred.** `klsave_spool_registers` reads the hub addresses `0x9200000001000020` and `0x9200000001030100`, calls `get_nasid` (`0xc000000000041434`) several times, and walks the board records with `find_lboard` (`0xc000000000044370`) and `find_first_component` (`0xc000000000044600`). It reads a word at offset 0x30 of the component found. It writes six doublewords to addresses the facts do not resolve and branches on bit tests of the hub value and its argument, which fits the spool-area save the name suggests. The exact destination and the bit meanings are open.

- `ii_llp_error_monitor` in `6.5.30/unix.IP35` at 0xc00000000005b31c
  **Inferred.** `ii_llp_error_monitor` reads and writes back the hub register `0x9200000001400130`, and copies two doublewords and two bytes at its argument's offsets 0x0, 0x8, 0x10 and 0x11 back onto themselves. The write-back of unchanged values means any effect comes from the store to the hub register, and the facts do not show which bit it clears. Two comparisons against `0xffff` decide the path. The name suggests link-level error monitoring on the I/O side.

- `ni_llp_error_monitor` in `6.5.30/unix.IP35` at 0xc00000000005b384
  **Inferred.** `ni_llp_error_monitor` reads the hub register `0x9200000001680120`, tests its bit 52 through a left shift and sign test, and copies three doublewords and three bytes at its argument's offsets 0x0, 0x8, 0x10, 0x18, 0x19 and 0x1a back onto themselves. Bit tests on nibble fields of several argument registers decide the path, and on one path it calls `cmn_err` (`0xc0000000002ad048`) with message code 4 and the hub register value. It is the network-interface sibling of `ii_llp_error_monitor`, and the nibble meanings are not established.

- `err_int_unblock_mask` in `6.5.30/unix.IP35` at 0xc00000000005bb00
  **Inferred.** `err_int_unblock_mask` reads the global halfword at `0xffffffffffffc428` and a per-node table entry, and reads a byte at offset 0x528 of that entry. It stores a doubleword to the hub address `0x9200000001000400` and performs a read-modify-write on one of `0x9200000001000408` or `0x9200000001000410`, chosen by the table value. The write is skipped when argument `a1` is zero. The name suggests clearing an interrupt-unblock mask, but the register bit pattern and the choice of address are not established by these facts.
