# Routine role statements, tier-1 batch 20

Role statements for 25 routines in the kudzu tier-1 list, batch 20 (targets 500 to 524 of the ranked list). Each routine was written up from its instruction facts (registers, calls, constants, branches). Roles come from those facts plus the routine's name. Where the facts do not settle the role, the entry says so.

Binary for every entry: `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o`. Addresses with `0x92000000...` are read as uncached node-addressed windows, with the NASID in the high bits.

- `save_hub_md_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x14004
  **Inferred.** It zeroes a 0x58-byte block at a1 with `bzero` (call at 0x14020), then copies five doubleword fields from a0 into a1 at offsets +0x8 through +0x40. The loop branches at 0x140ac, 0x140b4 and 0x140bc test shifted copies of the word at a0+0x7c0010, so that word sets how many passes run. The name suggests it saves an MD error record, but the facts do not say which block a0 points to.

- `save_hub_io_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x14168
  **Inferred.** It zeroes a 0xd0-byte block at a1 with `bzero` (call at 0x14184), then copies doubleword fields from a0 (the +0x400008 and +0x410000 to +0x420010 groups) into a1 at +0x8 through +0xb0. It calls `save_hub_io_crb` with words loaded from a0+0x410000 and a0+0x400008, then `save_hub_io_ieph`, and returns the latter's result. The sibling routines `save_hub_md_error` and `save_hub_ni_error` follow the same shape, which fits a family of error-record save functions.

- `save_hub_ni_error` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x142c4
  **Inferred.** It zeroes a 0x30-byte block at a1 with `bzero` (call at 0x142e0), then copies four doublewords from a0+0x680100 through a0+0x680118 into a1 at +0x8 through +0x20. The branch at 0x142fc skips those copies when the word at a0+0x680100 is zero. The shape matches the other `save_hub_*` routines, but these facts do not show which NI registers the words are.

- `hub_error_init` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18250
  **Inferred.** It walks nodes through the compact-to-NASID table and writes error-control and interrupt registers in the node-addressed windows at 0x9200000001800000 and 0x9200000001a00000. It clears per-node records in `kl_error_log`, calls `recover_error_init` four times, and connects `hubpi_eint_handler` with `cause_intr_connect` (call at 0x1868c). It also calls `hubii_eint_init`, `hubni_eint_init` and `kmem_zalloc_node`, and has three `assfail` paths plus a final `cmn_err`; the register meanings are inferred from the address pattern and callee names.

- `hubpi_eint_handler` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x18ad0
  **Inferred.** This is the hub PI error interrupt handler that `hub_error_init` connects at 0x1868c. It calls `check_ni_errors` first (0x18ad8), then reads the `pi_fatal_err_cpu_a`, `pi_fatal_err_cpu_b` and `pi_err_int_pend` words and tests their bits against constant masks to choose between recovery and fatal paths. The tail is a long run of `cmn_err` calls, plus `process_error_spool`, `log_md_errors` and `kl_panic_setup`. Which bit selects which path is only partly established, and the meanings of the `cmn_err` message codes are not in the facts.

- `check_ni_errors_nasid` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x19324
  **Inferred.** It takes a NASID in a0 and reads NI registers at (a0 << 0x21) plus fixed offsets, the same node-addressed form as the error init routines. It calls `part_id`, `ni_port_down_reset` (three call sites), and `protected_broadcast` with codes 0x67 and 0x68, and reports port and link state through many `cmn_err` calls. It returns 0 or 1 in v0. The NI-error role comes from the name and the port-down calls, not from the register meanings.

- `check_ni_errors` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x197c0
  **Inferred.** This is a two-call wrapper: `get_nasid` (call at 0x197c8) turns the entry arguments into a NASID, and `check_ni_errors_nasid` (call at 0x197d0) runs on it and returns its value. The routine has no logic of its own. The wrapper structure is established by the calls; the error-checking meaning comes from the callee name.

- `hubni_error_handler` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x197e4
  **Inferred.** Another two-call wrapper: `get_nasid` (call at 0x197f4) gives a NASID, and the routine calls `hubni_error_handler_nasid` (call at 0x19804) with a0 and a2 passed through and the NASID in a1. It returns that callee's result. It is the same pattern as `check_ni_errors`, one level up.

- `klsave_spool_registers` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1a24c
  **Inferred.** It calls `get_nasid` seven times, then looks up a component with `find_lboard` (argument 0x11) and `find_first_component` (argument 0x2), and calls `chk_for_pebrick` on the node. It reads node-addressed registers at (ret(get_nasid) << 0x21) plus fixed offsets, and reads a word from the component record at +0x30. Its six stores go to addresses the facts do not resolve, so the destinations are open. The name suggests it spools those registers into the kernel error log.

- `ii_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1ba88
  **Inferred.** It reads the word at 0x9200000001400130 and writes the same value back at 0x1ba9c. It then works over the fields of a0 (doublewords at +0x0 and +0x8, bytes at +0x10 and +0x11), with branches that compare a4 against 0xffff and against registers a2 and a3. The name and the 0x14000xx window suggest the Hub II link-level protocol, but the field meanings are not established by these facts.

- `ni_llp_error_monitor` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1baf0
  **Inferred.** It reads the status word at 0x9200000001680120 and passes it to `cmn_err` (call at 0x1bc04, first argument 0x4), which suggests a log-only path. Before that it tests a sign bit and the low nibbles of several registers, and copies doubleword and byte fields of a0 (+0x0 through +0x1a). It is the NI counterpart of `ii_llp_error_monitor`, with the same unresolved structure layout.

- `err_int_unblock_mask` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1c260
  **Inferred.** It reads a halfword at 0xffffffffffffc428 and uses the `pdaindr` table entry to reach a field at +0x8 and a byte at +0x528. It writes 0x9200000001000400, then read-modify-writes one of two window words (0x...1000408 or 0x...1000410), with a branch on a1 being zero. The name suggests it clears an error-interrupt mask bit, but the facts show only the conditional write-back, so the role rests on the name.

- `stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1c5d0
  **Inferred.** It makes no calls. It runs a loop bounded by 4 (register s7), reads the status word at 0x9200000001000020 and tests its bit 0, and stores to 0x9200000001000410 and to window addresses in the 0x92000000016... and 0x92000000018... ranges. A final store at 0x1c7e0 writes 0x9200000001000408. The name suggests a halt of the other processors, but the facts show a status-polling loop with no halt request, so the role stays partly open.

- `handle_io_buserr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1d628
  **Inferred.** It is the I/O-side bus error handler. It calls `get_matching_spool_addr` (call at 0x1d684) and branches on its result (-1, 0, 1 or 2), then calls `kl_ioerror_handler` up to three times with the compact node from `nasid_to_compact_node` and the word at a0+0x110. `cmn_err_tag` (calls at 0x1d7f4 and 0x1d810) and `get_nasid` (call at 0x1d860) cover logging and fallback. The I/O role follows from the name and the ioerror callees.

- `handle_uncac_buserr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1d874
  **Inferred.** It handles bus errors on uncached accesses, going by the name. It calls `get_matching_spool_addr` (call at 0x1d8a8) and branches on the result, calls `partition_handle_error` (code 0x9) at 0x1d9ac and 0x1de24, and checks memory pages with `error_page_discard` and `error_check_part_page`. It uses `nasid_to_partid` and `part_id` for partition checks, and its `cmn_err_tag` message numbers run from 0xb6e to 0xb79. It ends in an indirect jump at 0x1d90c through a jump table whose targets the facts do not resolve, so part of the routine is outside the static listing.

- `handle_cac_buserr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1e160
  **Inferred.** It handles cacheable-access bus errors, going by the name. It calls `get_matching_spool_addr` (call at 0x1e198) and `kl_panic` (call at 0x1e214) on one path, then runs a chain of checks: `error_lookup_inst_type`, `error_check_memory_access`, `error_check_part_page`, `error_check_poisoned_state`, `memory_iscalias` and `klerror_check_recoverable`. Its `partition_handle_error` calls use codes 0x2, 0x5, 0x6 and 0x9. The indirect jump at 0x1e20c uses a table with unresolved targets, so the mapping from error type to path is open.

- `intpend0` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20978
  **Inferred.** It appears to service pending interrupt bits for one group. It reads the pending word at 0x9200000001000098 and a mask word, calls `ms1bit` (call at 0x209f0) to find a set bit, and dispatches through a pointer at +16 of a table entry with indirect calls at 0x20a34 and 0x20ac8. Semaphore calls (`cvsema`, `valusema`, `vsema`) surround the dispatch. The indirect callee is not resolved, and the meaning of `ms1bit` as a most-significant-bit search comes from its name.

- `intpend1` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20ad8
  **Inferred.** This is the second-group twin of `intpend0`. It reads the words at 0x92000000010000a0 and 0x9200000001000098, calls `ms1bit` (call at 0x20b24), and dispatches through an indirect call at 0x20b5c that uses a pointer at +16 of a table entry. Its `cmn_err` call (0x20bb4) passes the `ms1bit` result on one path. It has no semaphore calls, unlike `intpend0`.

- `sendintr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20f2c
  **Inferred.** It looks up the per-CPU entry for its argument a0 in `pdaindr` and reads a halfword at +0x42c, which it uses as a node number. That node number selects the window for a doubleword store to 0x9200000001800090 or 0x9200000001a00090. A compare of a1 with 0xab (branch at 0x20f64) takes a different path around the store, and the facts do not show what 0xab means. The name suggests an interrupt to another CPU, but the facts show only the store, so that interpretation is inferred.

- `cpuintr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x20f98
  **Inferred.** It is the CPU-level interrupt routine. It reads per-CPU fields through `pdaindr` and a word at 0xffffffffffffc280, stores to 0x9200000001000090, and loads the record at 0x9200000001000098. It block-copies 0x168 bytes with `bcopy` (call at 0x21038), then calls `doacvec` and `doactions` and logs two timestamp events through `log_tstamp_event` (codes 0x71 and 0x65). A bit test of 0x20 in a kernel data word (branch at 0x20fac) selects one path.

- `dbgintr` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2110c
  **Inferred.** It reads the per-CPU entry as the other interrupt routines do, writes 0x9200000001000090, and loads 0x9200000001000098. It then calls `debug` (call at 0x21148) with a string-table address, the 0x9200000001000090 address, and its own a3 passed through, and returns the callee's value. The debugger-entry role comes from the name.

- `chkdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x2143c
  **Inferred.** It reads the per-CPU entry and the word at 0x92000000010000a0, and branches at 0x21474 on that word's masked value against v0. Only the fall-through path reaches the call to `dbgintr` (call at 0x2147c), which takes a0 = 0 and passes a1 to a3 through. The facts do not name the flag gating the call, so the gate stays open.

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x214fc
  **Inferred.** It loops over CPU indices up to `maxcpus`, calling `get_cpu_slice` (call at 0x2152c) and `cpu_enabled` (call at 0x2159c) on each, and skips the index that matches the halfword at [-0x3bd8], which other routines here use as their current CPU id. It stores a word to an address the facts do not resolve. The name suggests a debugger-mode stop of the other CPUs, but no stop request appears among the calls, so the role comes from the name.

- `bootstrap` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x255f8
  **Inferred.** It reads a word at 0x9200000001600000 and two halfwords, calls `mapped_kernel_setup_tlb` (call at 0x2564c) with a2 and a3 passed through, and then jumps through `jr` at 0x2566c to a reloc-resolved target at `.text+153204`. It looks like an early kernel entry that sets up the mapped TLB and transfers control. The jump target is not named by the facts.
