# Routine role statements, tier-1 batch 20

Role statements for 25 routines in the kudzu tier-1 list, batch 20 (targets 500 to 524 of the ranked list). Each routine was written up from its instruction facts (registers, calls, constants, branches). Roles come from those facts plus the routine's name. Where the facts do not settle the role, the entry says so.

Binary for every entry: `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o`. Addresses with `0x92000000...` are read as uncached node-addressed windows, with the NASID in the high bits.

- `save_hub_md_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x14004
  **Inferred.** It zeroes a 0x58-byte block at a1 with `bzero` (call at 0x14020), then copies five doubleword fields from a0 into a1 at offsets +0x8, +0x18, +0x28, +0x38 and +0x40 (the sources are a0+0x7c0010, +0x7e0010, +0x7c0020, +0x780040 and +0x780048). The branches at 0x140ac, 0x140b4 and 0x140bc test shifted copies of the word at a0+0x7c0010 and jump back to 0x1404c or forward to 0x14050; what they count or select is not shown by the facts. The name suggests it saves an MD error record, but the facts do not say which block a0 points to.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_io_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x14168
  **Inferred.** It zeroes a 0xd0-byte block at a1 with `bzero` (call at 0x14184), then copies doubleword fields from a0 (the +0x400008 and +0x410000 to +0x420010 groups) into a1 at +0x8 through +0xb0. It calls `save_hub_io_crb` with words loaded from a0+0x410000 and a0+0x400008 (as a2 and a3), then `save_hub_io_ieph`, and returns the latter's result. `save_hub_md_error` and `save_hub_ni_error` also zero a block with `bzero` and copy fields from a0, and the shared `save_hub_` prefix suggests a family of error-record save functions; the facts for this routine alone do not show that.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_ni_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x142c4
  **Inferred.** It zeroes a 0x30-byte block at a1 with `bzero` (call at 0x142e0), then copies four doublewords from a0+0x680100 through a0+0x680118 into a1 at +0x8 through +0x20. The branch at 0x142fc skips those copies when the word at a0+0x680100 is zero. The shape matches the other `save_hub_*` routines, but these facts do not show which NI registers the words are.
  Reviewed: 2026-11-12 (sonnet)

- `hub_error_init` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18250
  **Inferred.** It walks nodes through the compact-to-NASID table (it passes the table entry to `hub_error_clear` as a0) and calls `get_compact_nodeid`. It makes stores and loads at offsets and addresses the facts mostly do not resolve; one load at 0x18650 resolves to 0x9200000001800428. It branches on `kl_error_log` entries, calls `recover_error_init` four times (0x18384 through 0x183a8) with addresses derived from a `kl_error_log` entry, and connects `hubpi_eint_handler` with `cause_intr_connect` (call at 0x1868c). It also calls `hubii_eint_init`, `hubni_eint_init`, `kmem_zalloc_node` (twice, sizes 0x2258 and 0x40) and `spinlock_init`, and has three `assfail` paths plus a final `cmn_err`. Role (from name): hub error setup; the facts do not show which registers the stores write or whether the `kl_error_log` records are cleared.
  Reviewed: 2026-11-12 (sonnet)

- `hubpi_eint_handler` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18ad0
  **Inferred.** `hub_error_init` connects this routine as a handler with `cause_intr_connect` (call at 0x1868c). It calls `check_ni_errors` first (0x18ad8), and calls `hubni_error_handler` only when that returns nonzero. It then reads the `pi_fatal_err_cpu_a`, `pi_fatal_err_cpu_b` and `pi_err_int_pend` words and tests their bits against masks, some of them constants (for example 0x1 or 0x2, 0x40 or 0x80, 0x10 or 0x20), with `process_error_spool` called from three of the mask tests. It also calls `get_nasid`, `log_md_errors`, `md_int_block`, `kl_panic_setup` and `part_id`, and makes many `cmn_err` calls. Role (from name): PI error interrupt handler. Which bit selects which path, and what the `cmn_err` message codes mean, are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `check_ni_errors_nasid` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x19324
  **Inferred.** It takes a NASID in a0 and reads words at (a0 << 0x21) plus fixed offsets, the same node-addressed form `hub_error_init` uses. It calls `part_id`, `ni_port_down_reset` (three call sites) and `protected_broadcast` with codes 0x67 and 0x68, and makes many `cmn_err` calls, several of them gated by masks tested against one loaded word. It returns 0 or 1 in v0. Role (from name): NI error check; it comes from the name and the port-down calls, and the facts do not show what the registers or the message arguments mean.
  Reviewed: 2026-11-12 (sonnet)

- `check_ni_errors` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x197c0
  **Inferred.** This is a two-call wrapper: `get_nasid` (call at 0x197c8) turns the entry arguments into a NASID, and `check_ni_errors_nasid` (call at 0x197d0) runs on it and returns its value. The routine has no logic of its own. The wrapper structure is established by the calls; the error-checking meaning comes from the callee name.
  Reviewed: 2026-11-12 (sonnet)

- `hubni_error_handler` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x197e4
  **Inferred.** Another two-call wrapper: `get_nasid` (call at 0x197f4) gives a NASID, and the routine calls `hubni_error_handler_nasid` (call at 0x19804) with its own a0 passed through as a0, the NASID in a1 and its own a1 in a2. It returns that callee's result. It has the same two-call wrapper shape as `check_ni_errors`.
  Reviewed: 2026-11-12 (sonnet)

- `klsave_spool_registers` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1a24c
  **Inferred.** It calls `get_nasid` at seven sites, only some of them on every path. When a loaded word compares unequal to 0xc000 (branch at 0x1a28c) it goes on to `find_lboard` (argument 0x11) and `find_first_component` (argument 0x2), each followed by a zero check on its result; on the other path it calls `get_nasid` then `chk_for_pebrick` (call at 0x1a3f4) on that NASID. It reads a word from the component record at +0x30. Its six stores go to addresses the facts do not resolve, so the destinations are open. The facts do not show which registers it reads. The name suggests it spools registers into the kernel error log.
  Reviewed: 2026-11-12 (sonnet)

- `ii_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1ba88
  **Inferred.** It loads a doubleword at 0x1ba8c and stores a doubleword at 0x1ba9c; the facts do not resolve either address or show that the stored value is the loaded one. It then works over the fields of a0 (doublewords at +0x0 and +0x8, bytes at +0x10 and +0x11), with branches that compare a4 (0xffff) against registers a3 and a2 and skip the matching copies. The name suggests the Hub II link-level protocol, but the facts do not establish the field meanings.
  Reviewed: 2026-11-12 (sonnet)

- `ni_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1baf0
  **Inferred.** It reads the status word at 0x9200000001680120 and passes it to `cmn_err` (call at 0x1bc04, first argument 0x4, status word as a2). Before that it tests a sign bit of that word shifted left by 0xb and the low nibbles of several registers, and copies doubleword and byte fields of a0 (+0x0 through +0x1a), some copies skipped when a register equals 0xff. Its name pairs it with `ii_llp_error_monitor`; the structure layout is not resolved by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `err_int_unblock_mask` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1c260
  **Inferred.** It loads a halfword, then a doubleword at +0x8 and a byte at +0x528 of a per-CPU style entry. It branches on a1 being zero, and has a store, a load and a second store at addresses the facts do not resolve. Role (from name): the name suggests it clears an error-interrupt mask bit, but the facts do not show the addresses or whether the load and stores form a read-modify-write.
  Reviewed: 2026-11-12 (sonnet)

- `stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1c5d0
  **Inferred.** It makes no calls. It runs a loop bounded by 4 (register s7), reads the status word at 0x9200000001000020 and tests its bit 0, and makes stores whose addresses the facts do not resolve. The name suggests a halt of the other processors, but the facts show a status-polling loop with no halt request, so the role stays partly open.
  Reviewed: 2026-11-12 (sonnet)

- `handle_io_buserr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1d628
  **Inferred.** It calls `get_matching_spool_addr` (call at 0x1d684) and branches on its result (0, -1, 1 or 2). Depending on the path it calls `kl_ioerror_handler` at one of three sites (0x1d728, 0x1d798, 0x1d850) with a0 set to the `nasid_to_compact_node` symbol (the facts do not show a table lookup), a3 = 0x5, a4 = its own a1 and a5 = the word at a0+0x110. It also calls `cmn_err_tag` (calls at 0x1d7f4 and 0x1d810) and `get_nasid` (call at 0x1d860). Role (from name): I/O-side bus error handler; it follows from the name and the ioerror callee.
  Reviewed: 2026-11-12 (sonnet)

- `handle_uncac_buserr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1d874
  **Inferred.** Role (from name): handler for bus errors on uncached accesses. It calls `get_matching_spool_addr` (call at 0x1d8a8) and branches on the result (0, -1, 1 or 2), calls `partition_handle_error` (a2 = 0x9) at 0x1d9ac and 0x1de24, and calls `error_page_discard` and `error_check_part_page`. It uses `nasid_to_partid` and `part_id` for partition checks, and its `cmn_err_tag` message numbers include 0xb6e through 0xb73, 0xb79 (the lowest is 0xb6e and the highest 0xb79; not every value in between appears). A bounds check at 0x1d900 (a value at or above 8, unsigned) followed by a load at 0x1d908 suggests a jump table, but the facts show no indirect jump, so part of the control flow is unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `handle_cac_buserr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1e160
  **Inferred.** Role (from name): handler for bus errors on cacheable accesses. It calls `get_matching_spool_addr` (call at 0x1e198) and branches on the result (-1, 0, 1 or 2). It calls `kl_panic` (call at 0x1e214) on one path, and on others reaches `error_lookup_inst_type`, `error_check_memory_access`, `error_check_part_page`, `error_check_poisoned_state`, `memory_iscalias` and `klerror_check_recoverable`, each reached through branches on earlier results. Its `partition_handle_error` calls use codes 0x2, 0x5, 0x6 and 0x9. A bounds check at 0x1e200 (a value at or above 8, unsigned) followed by a load at 0x1e208 suggests a jump table, but the facts show no indirect jump, so the mapping from error type to path is open.
  Reviewed: 2026-11-12 (sonnet)

- `intpend0` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20978
  **Inferred.** It appears to service pending interrupt bits for one group. It reads the pending word at 0x9200000001000098 and a mask word, calls `ms1bit` (call at 0x209f0), and uses its result to index a table. Through a table entry it makes unresolved indirect calls at 0x20a34 and 0x20ac8 with a0 = the word at +16 of the entry and a1 = its own a0; the call at 0x20a34 is skipped when the word at +8 of the entry is zero. `cvsema` runs at the top of the loop, and `valusema` and `vsema` follow the first indirect call. The indirect callee is not resolved, and the meaning of `ms1bit` as a most-significant-bit search comes from its name.
  Reviewed: 2026-11-12 (sonnet)

- `intpend1` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20ad8
  **Inferred.** It is the second-group twin of `intpend0` by name and shape. It reads the word at 0x92000000010000a0 and a mask word, calls `ms1bit` (call at 0x20b24), and makes an unresolved indirect call at 0x20b5c with a0 = the word at +16 of a table entry, skipped when the word at +0 of that entry is zero. Its `cmn_err` call (0x20bb4) passes the `ms1bit` result as a2 on one path. It has no semaphore calls, unlike `intpend0`.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20f2c
  **Inferred.** It loads a doubleword at +0x8 and a byte at +0x528 of a per-CPU style entry, then a halfword at +0x42c. A compare of a1 with 0xab (branch at 0x20f64) jumps past a doubleword store at 0x20f88, whose address the facts do not resolve, and the routine returns 0 on one path. The facts do not show what the halfword or 0xab mean. The name suggests an interrupt sent to another CPU, but the facts show only the store, so that interpretation is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `cpuintr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20f98
  **Inferred.** Role (from name): CPU-level interrupt routine. It reads per-CPU style fields and a word at 0xffffffffffffc280, and has stores and loads at addresses the facts do not resolve. It calls `bcopy` (a2 = 0x168, call at 0x21038) only when a byte flag and the `nmied` word are both nonzero, then calls `doacvec` and `doactions` and logs two timestamp events through `log_tstamp_event` (codes 0x71 and 0x65). A bit test of 0x20 in a kernel data word (branches at 0x20fac and 0x21058) selects between paths.
  Reviewed: 2026-11-12 (sonnet)

- `dbgintr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2110c
  **Inferred.** It loads a halfword, then a doubleword at +0x8 and a byte at +0x528 of a per-CPU style entry, as the other interrupt routines do, and makes a store the facts do not resolve. It then calls `debug` (call at 0x21148) with a0 = a `.rodata` address (.rodata+43328), a2 = 0x9200000001000090 and its own a3 passed through, and returns the callee's value. The debugger-entry role comes from the name.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2143c
  **Inferred.** It reads the per-CPU entry and the word at 0x92000000010000a0, and branches at 0x21474 on that word's masked value against v0. Only the fall-through path reaches the call to `dbgintr` (call at 0x2147c), which takes a0 = 0 and passes a1 to a3 through. The facts do not name the flag gating the call, so the gate stays open.
  Reviewed: 2026-11-12 (sonnet)

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x214fc
  **Inferred.** It loops over CPU indices up to `maxcpus`, calling `get_cpu_slice` (call at 0x2152c) and, for indices that differ from the halfword at [-0x3bd8], `cpu_enabled` (call at 0x2159c). The facts do not show what that halfword holds. It makes a store to an address the facts do not resolve. The name suggests a debugger-mode stop of the other CPUs, but no stop request appears among the calls, so the role comes from the name.
  Reviewed: 2026-11-12 (sonnet)

- `bootstrap` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x255f8
  **Inferred.** It loads a doubleword and two halfwords (addresses not resolved by the facts), calls `mapped_kernel_setup_tlb` (call at 0x2564c) with a2 and a3 passed through, and then makes a tail jump at 0x2566c to a reloc-resolved target at `.text+153204`. It looks like an early kernel entry that sets up the mapped TLB and transfers control. The jump target is not named by the facts.
  Reviewed: 2026-11-12 (sonnet)
