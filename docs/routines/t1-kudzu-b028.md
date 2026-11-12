# Tier-1 kudzu routine roles, batch 28

Role statements for 25 routines from two shipped kernel images in the 6.5.30 tree: `6.5.30/unix.IP30` (routines 1 to 5) and `6.5.30/unix.IP35` (routines 6 to 25). They come from the instruction facts only: register uses, loads and stores with fixed addresses, calls with their constant arguments, and branch conditions. Nothing here was run on a machine.

Four entries are left out because their statements cite nothing beyond the routine's own name, or name a routine the symbol table gives differently at that address.

Grades are inferred, because the facts show control flow and callees but not the intent behind the hub register accesses or the callee contracts. One entry, `is_fine_dirmode`, is graded unknown because the facts do not resolve its return value. Where a meaning rests on a name alone, the entry says so. Addresses such as `0x9200000001680010` are hub register addresses as the code uses them. `0xffffffffffffc3a0`-style addresses are halfword globals read by several routines. `table(...)` marks a per-node table pointer.

- `gfx_flow_pipe_head_init` in `6.5.30/unix.IP30` at 0xa80000002038f270
  **Inferred.** `gfx_flow_pipe_head_init` reads a halfword global, stores one word at offset 0x58 of its argument, and loads words from the argument at offsets 0x8, 0x10, 0x1c and 0x20. It stores into a table slot and at fixed addresses through a base of `0xffffffffaff00000`. It takes the lock at `0xa8000000204b0024` with `mutex_spinlock` (`0xa80000002022d954`) and releases it with `mutex_spinunlock` (`0xa80000002022d9a0`), with a load after the lock call and a store in the unlock call's delay slot, both through that base. The purpose of each field is not established by these facts, and the lock's scope is inferred from the call order.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_alloc` in `6.5.30/unix.IP30` at 0xa80000002038f318
  **Inferred.** `gfx_flow_alloc` takes the lock at `0xa8000000204a8628` with `mutex_spinlock` and tests the doubleword at `0xa8000000204a8620` and the word at `0xa8000000204b0020`. When neither test branches away it releases the lock and allocates a 0x68-byte record with `kmem_alloc` (`0xa800000020148580`). When the allocation is nonzero it looks up a widget through `xwidget_info_get` (`0xa80000002003acd0`) and `xwidget_info_id_get` (`0xa80000002003adb4`), stores about twenty fields of the record at offsets 0x0 to 0x60, calls `kvalloc` (`0xa800000020170c9c`) and calls `gfx_flow_pipe_head_init` on the new record. `gfx_flow_init` (`0xa80000002038f230`) and `heart_flow_control_connect` (`0xa800000020040208`) are called on the paths where those two tests branch away, not after the allocation. That the allocation is the counterpart of `gfx_flow_dealloc` rests on the matching 0x68 size.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_dealloc` in `6.5.30/unix.IP30` at 0xa80000002038f524
  **Inferred.** `gfx_flow_dealloc` reads a halfword at offset 0x5c of its argument and compares it with -1; when it differs it calls `cmn_err` (`0xa8000000201c6b00`) with a0 = 0x4. It takes the lock at `0xa8000000204b0024`, reads the word at offset 0x58 and releases it, then takes the lock at `0xa8000000204a8628`. When the word at `0xa8000000204b0020` minus one is nonzero it releases that lock and calls `kmem_free` (`0xa800000020148710`) on its argument with size 0x68. When it is zero the path reaches a loop that calls `heart_flow_control_disable` (`0xa800000020040330`) and `heart_flow_control_disconnect` (`0xa80000002004028c`) while `s0` is below 2 (signed); where `s0` starts is not shown. Role (from name): the teardown of the record that `gfx_flow_alloc` makes; the meaning of the -1 check is open.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_attach_me` in `6.5.30/unix.IP30` at 0xa80000002038f6dc
  **Inferred.** `gfx_flow_attach_me` stores to its argument's fields at offsets 0x58, 0x5c and 0x60 before it takes the lock at `0xa8000000204b0024` with `mutex_spinlock`; one more store sits in that call's delay slot. Between the lock and `mutex_spinunlock` it reads the word at offset 0x58. After the unlock it calls `heart_flow_control_enable` (`0xa8000000200402f4`) with the value at offset 0x58 of its argument and returns that result. Role (from name): the attach side of `gfx_flow_detach_me`; what the table slot it stores to stands for is not established.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_detach_me` in `6.5.30/unix.IP30` at 0xa80000002038f7c0
  **Inferred.** `gfx_flow_detach_me` takes the lock at `0xa8000000204b0024` with `mutex_spinlock`, the same lock as the attach routine, and reads the word at offset 0x58. After `mutex_spinunlock` it calls `heart_flow_control_disable` (`0xa800000020040330`) with the value at offset 0x58 of its argument, and then stores to offsets 0x60, 0x5c, 0x58 and 0x1c; the stored values are not shown in the facts. It returns the result of `heart_flow_control_disable`. Role (from name): the detach side of `gfx_flow_attach_me`; the pairing rests on the shared field offsets and the lock.
  Reviewed: 2026-11-12 (sonnet)

- `restart` in `6.5.30/unix.IP35` at 0xc000000000041240
  **Inferred.** `restart` tests the word at `0xc00000000163d424`; unless it is zero it runs a loop, while a counter is below the node count held at `0xc000000001645f9c`, that loads a halfword and stores a doubleword each pass. It then calls `us_delay` (`0xc000000000069a4c`) with 1000000. Depending on the word at `0xc00000000163c76c` (equal to 2 or not) and on the word at `0xc000000001646510` (zero or not), it either calls `ni_reset_port` (`0xc000000000041534`) or loops over `get_elsc` (`0xc000000000065240`), `sc_power_down_all` (`0xc0000000000d0190`) and `us_delay` with 3000000. It then calls `get_nasid`, `elsc_init` (`0xc0000000000ce8d8`) and `elsc_system_reset` (`0xc0000000000cfdc4`). A negative return from `elsc_system_reset` leads to a `printf` (`0xc0000000002adeec`) and a further `us_delay` of 1000000; otherwise the routine returns that value. Role (from name): a system power-down and reset sequence; the order of the controller calls is read from the call graph, and the reset target is not established.
  Reviewed: 2026-11-12 (sonnet)

- `is_fine_dirmode` in `6.5.30/unix.IP35` at 0xc000000000041460
  **Unknown.** `is_fine_dirmode` loads one doubleword from the hub address `0x9200000001600000` and returns a value the facts do not resolve. Its caller `mlreset` (`0xc00000000004d94c`) calls it and keeps the result. The name suggests a directory-mode check, but nothing in the instruction listing establishes that.
  Reviewed: 2026-11-12 (sonnet)

- `ni_reset_port` in `6.5.30/unix.IP35` at 0xc000000000041534
  **Inferred.** `ni_reset_port` stores two doublewords to the hub addresses `0x9200000001680010` and `0x9200000001680008`, and returns the second address. The stored values do not appear in the facts, so the effect on the port is open. It is called from `restart` (`0xc000000000041240`), which fits a port-reset role.
  Reviewed: 2026-11-12 (sonnet)

- `start` in `6.5.30/unix.IP35` at 0xc000000000041b10
  **Inferred.** `start` loads a doubleword from the hub address `0x9200000001600000` and calls `mapped_kernel_setup_tlb` (`0xc000000000065538`). It then jumps through a register constant at `0xc000000000041b78` to `0xc000000000041b80`. The facts do not reach that target from the entry, so the rest of the 544-byte routine is not analysed. The first part looks like an early kernel start path that maps the kernel's TLB before handing off.
  Reviewed: 2026-11-12 (sonnet)

- `mlreset` in `6.5.30/unix.IP35` at 0xc00000000004d94c
  **Inferred.** `mlreset` is the largest routine in this batch, at 1152 bytes. It starts by calling `r1nkperf_control_register_set` (`0xc000000000063648`) and `r1nkperf_data_register_set` (`0xc0000000000635e4`) with a0 = 0 and 1 and a1 = 0, then calls `get_cpu_irr` (`0xc000000000064478`) and compares the result with 0xe. When that does not match it calls `get_cpu_irr` again with a0 = 2 and compares a result with 0xf. When neither matches and a register is nonzero it calls `flush_tlb` with a0 = 2 and `flush_cache`, and returns the `flush_cache` result; the same two set calls for a0 = 2 and 3 run only on the path where a comparison matched. The main path then calls the per-device resets `ioc3_mlreset` (`0xc0000000000c4ef4`), `ioc4_mlreset` (`0xc0000000000c2db0`), `l1_mlreset` (`0xc000000000228104`) and `nvram_baseinit` (`0xc00000000006e9a4`), and calls `cpu_node_probe`, `gen_region_mask`, `setup_replication_mask` and `mem_init`. Three `assfail` calls (`0xc0000000000619f4`) with line numbers 0x119, 0x11c and 0x12e end the routine. Role (from name): the machine-level reset and init pass; the order of steps comes from the call graph and the counter setup's purpose is not established.
  Reviewed: 2026-11-12 (sonnet)

- `getcpuid` in `6.5.30/unix.IP35` at 0xc00000000004e898
  **Inferred.** `getcpuid` reads a doubleword from the hub address `0x9200000001000020`, passes it to `nasid_slice_to_cpuinfo` (`0xc000000000044f8c`) together with the NASID from `get_nasid` (`0xc000000000041434`), and returns a halfword at offset 0x1e of the record it gets back. The call is wrapped in `spl1` (`0xc000000000061f6c`) and `splx` (`0xc000000000062054`). A null record hits `assfail` (line 0x535). The name and the return field are read as CPU identity lookup; the field's meaning is inferred from the name.
  Reviewed: 2026-11-12 (sonnet)

- `save_cache_error` in `6.5.30/unix.IP35` at 0xc000000000052a08
  **Inferred.** `save_cache_error` calls `get_nasid` (`0xc000000000041434`) and loads a doubleword from an entry of a table. It reads the hub address `0x9200000001000020` and compares it with a loaded byte. When a masked value equals 0xc000 it instead calls `chk_for_pebrick` (`0xc000000000046384`), passing the table base `0x9600000000000000` and `0x9600000000000000 | 0x2000` among its arguments. On the other path it stores three doublewords, a word and a byte into a destination whose address the facts do not resolve. Role (from name): copying cache-error state into a save area. The destination and the field meanings are open.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_pi_error` in `6.5.30/unix.IP35` at 0xc0000000000537ac
  **Inferred.** `save_hub_pi_error` clears 0x60 bytes at its second argument with `blkclr` (`0xc00000000004791c`) and copies several 8-byte fields from the first argument into the second, reading source offsets 0x400, 0x50, 0x58, 0x430, 0x440, 0x450, 0x460, 0x4c8 and 0x4d0. The words at offsets 0x50 and 0x58 are tested against zero, and the words at 0x430, 0x450 and 0x4c8 are tested for bit 63; those tests decide whether the copies after them run. Role (from name): a copy of the hub processor-interface error registers. The field names and gate meanings are not established.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_md_error` in `6.5.30/unix.IP35` at 0xc000000000053884
  **Inferred.** `save_hub_md_error` clears 0x58 bytes at its second argument with `blkclr` and copies five fields from the first argument, at offsets 0x7c0010, 0x7e0010, 0x7c0020, 0x780040 and 0x780048, into the second at offsets 0x8 to 0x40. The word at offset 0x7c0010 is tested at bits 62, 61 and 60 by shift and sign test, the word at 0x7e0010 for its low two bits, and the words at 0x7c0020 and 0x780040 against zero; those tests decide which copies run. Some of the branches go back to earlier addresses, and whether they form a loop is not established. Role (from name): memory-directory error state. The offsets in the first argument are large and unexplained by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_io_error` in `6.5.30/unix.IP35` at 0xc0000000000539e8
  **Inferred.** `save_hub_io_error` clears 0xd0 bytes at its second argument with `blkclr`, then copies doublewords from the first argument at offsets 0x400008, 0x410000, 0x410008, 0x410010, 0x420000 and 0x420008 into the second, and calls `save_hub_io_crb` (`0xc000000000053970`) with the doublewords read from offsets 0x410000 and 0x400008 as its third and fourth arguments. The doubleword read from offset 0x420010 is stored into the second argument in that call's delay slot. It then calls `save_hub_io_ieph` (`0xc0000000000539cc`) and returns its result. Role (from name): I/O error capture. The two sub-saver calls are named but not analysed here.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_ni_error` in `6.5.30/unix.IP35` at 0xc000000000053b44
  **Inferred.** `save_hub_ni_error` clears 0x30 bytes at its second argument and copies four doublewords from the first argument at offsets 0x680100 to 0x680118 into the second at offsets 0x8 to 0x20. A test on the value at the first argument's offset 0x680100 gates the copy. The name suggests the network-interface error capture, the sibling of `save_hub_io_error` and `save_hub_pi_error`.
  Reviewed: 2026-11-12 (sonnet)

- `check_ni_errors_nasid` in `6.5.30/unix.IP35` at 0xc000000000058ba4
  **Inferred.** `check_ni_errors_nasid` reads several per-node error fields from addresses derived from its argument, and the hub addresses `0x9200000001680108`, `0x9200000001680110` and `0x9200000001680118`. It calls `part_id` (`0xc000000000496400`), calls `ni_port_down_reset` (`0xc000000000058af4`) on several paths, logs through `cmn_err` many times with message codes 0x14 and 0x15, and calls `protected_broadcast` (`0xc000000000060c04`) with codes 0x67 and 0x68. One doubleword read from a per-node address, held in `s0`, is tested against seven masks from 0xf000000 to 0xf000000000000, each selecting a 4-bit field, and each set field leads to one `cmn_err` call with code 0x14. Role (from name): a check that resets ports that are down and logs their state; what the fields are, which one belongs to which port, and the broadcast codes are open.
  Reviewed: 2026-11-12 (sonnet)

- `klsave_spool_registers` in `6.5.30/unix.IP35` at 0xc000000000059acc
  **Inferred.** `klsave_spool_registers` reads the hub addresses `0x9200000001000020` and `0x9200000001030100`, calls `get_nasid` (`0xc000000000041434`) several times, and looks up a board record with `find_lboard` (`0xc000000000044370`, a1 = 0x11) and a component of it with `find_first_component` (`0xc000000000044600`, a1 = 0x2). It reads a word at offset 0x30 of the component found. It writes six doublewords to addresses the facts do not resolve and branches on bit tests of the hub value and its argument. Role (from name): a spool-area save. The exact destination and the bit meanings are open.
  Reviewed: 2026-11-12 (sonnet)

- `ii_llp_error_monitor` in `6.5.30/unix.IP35` at 0xc00000000005b31c
  **Inferred.** `ii_llp_error_monitor` loads from the hub register `0x9200000001400130` and stores to the same address; the stored value is not shown in the facts, so which bit it affects is open. It also loads and then stores two doublewords and two bytes at its argument's offsets 0x0, 0x8, 0x10 and 0x11, the stored values again not shown. Two comparisons against `0xffff` decide the path. Role (from name): link-level error monitoring on the I/O side.
  Reviewed: 2026-11-12 (sonnet)

- `ni_llp_error_monitor` in `6.5.30/unix.IP35` at 0xc00000000005b384
  **Inferred.** `ni_llp_error_monitor` reads the hub register `0x9200000001680120` and tests its bit 52 through a left shift and sign test. It loads and then stores three doublewords and three bytes at its argument's offsets 0x0, 0x8, 0x10, 0x18, 0x19 and 0x1a, the stored values not shown in the facts. Bit tests on nibble fields of several argument registers decide the path, and on one path it calls `cmn_err` (`0xc0000000002ad048`) with message code 4 and the hub register value. Role (from name): the network-interface sibling of `ii_llp_error_monitor`; the nibble meanings are not established.
  Reviewed: 2026-11-12 (sonnet)

- `err_int_unblock_mask` in `6.5.30/unix.IP35` at 0xc00000000005bb00
  **Inferred.** `err_int_unblock_mask` reads the global halfword at `0xffffffffffffc428` and a per-node table entry, and reads a byte at offset 0x528 of that entry. It stores a doubleword to the hub address `0x9200000001000400` and performs a read-modify-write on one of `0x9200000001000408` or `0x9200000001000410`, chosen by the table value. The write is skipped when argument `a1` is zero. The name suggests clearing an interrupt-unblock mask, but the register bit pattern and the choice of address are not established by these facts.
  Reviewed: 2026-11-12 (sonnet)
