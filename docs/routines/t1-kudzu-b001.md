# Tier-1 kudzu routine roles, batch 1

Role statements for 25 routines of the shipped IP27 kernel `6.5.22/unix.IP27`, the first batch of the tier-1 hardware-facing list. Each role comes from the rendered instruction facts: register uses, loads and stores (with fixed addresses), calls and branch conditions. Routine names are the inventory names. Nothing here was run on a machine. Every grade is inferred, because the facts show control flow and callees but not the intent behind the hub register accesses or the callee contracts. Where a meaning is a guess from a name, the entry says so.

Addresses such as `0x9200000001000090` are hub register addresses as the code uses them. `table(0xc0000000014d39a8)` is a per-node table pointer that many of these routines read. `0xffffffffffffc420` is a halfword global read by most of them.

### `6.5.22/unix.IP27` at `0xc000000000034e5c`

`ni_llp_error_monitor` reads two 64-bit words from fixed addresses `0x9200000001608008` and `0x9200000001608088`, then copies a 27-byte block at its argument to itself, field by field, using loads and stores at offsets 0 to 0x1a. A bit test on the first hub word at `0xc000000000034e78` decides whether the byte-field work runs. The `cmn_err` call (`0xc0000000001f8090`) at `0xc000000000034f10` is on a separate path whose condition the facts do not fully pin down.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc0000000000353a8`

`err_int_unblock_mask` reads a halfword at `0xffffffffffffc420` and the per-node table entry, then writes 8 bytes to hub address `0x9200000001000400`. It also read-modify-writes a 64-bit word at an address computed from a byte field of the table entry. The write sequence is skipped when argument `a1` is zero. The register's bit pattern and the purpose of the unblock are not established by these facts.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc00000000003649c`

`handle_io_buserr` reads error-record fields at offset 0x110 of its argument, plus halfwords at fixed addresses. It calls `get_matching_spool_addr` (`0xc0000000000343c8`) and branches on the result against 0, -1, 1 and 2. Depending on the branch it calls `kl_ioerror_handler` (`0xc000000000037330`), `cmn_err_tag` (`0xc0000000001f80dc`) and `get_nasid` (`0xc00000000001d434`). One path writes `0x9200000001000400` after testing bits `0x100000` and `0x200000`. The meaning of each result code is inferred from callee names only.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc0000000000366e4`

`handle_uncac_buserr` is a 1180-byte dispatcher for uncached bus errors. It calls `get_matching_spool_addr`, `nasid_to_partid` (`0xc0000000003c2928`) and `part_id` (`0xc0000000003c2910`), and compares their results against values from the caller and the table. It then calls `partition_handle_error` (`0xc000000000037afc`) with several codes, and on a page-state path calls `error_check_memory_access` (`0xc000000000035e90`), `error_check_part_page` (`0xc000000000037a7c`), `error_check_poisoned_state` (`0xc000000000035f7c`) and `error_page_discard` (`0xc0000000000379e0`). Many paths log through `cmn_err_tag`. The partition ownership rules behind the comparisons are not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000036b80`

`handle_cac_buserr` is the cached-access counterpart of `handle_uncac_buserr`, at 1656 bytes. It reuses the same kind of checks (`error_check_part_page`, `partition_handle_error`, `error_page_discard`) and adds `klerror_check_recoverable` (`0xc0000000000354cc`), `memory_iscalias` (`0xc00000000002adf4`) and `verify_snchip_rev` (`0xc00000000001d490`), which it compares against 5. One path reaches `kl_panic` (`0xc0000000000374f4`) and another reaches `dump_crb` (`0xc0000000000371f8`). The range at `0xc000000000037004` is not reached from the entry in the rendered facts, so it is not analysed.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000038da8`

`intpend0` reads pending-interrupt words from the hub region around `0x9200000001000090` and `0x9200000001000098`, plus fixed addresses `0xffffffffffffc598` and `0xffffffffffffc508`. It finds the most significant set bit with `ms1bit` (`0xc000000000037bd8`) and indexes a table by it. The loop takes a semaphore with `cvsema` (`0xc00000000018a930`), calls a handler through a function pointer at offset 16 of the table entry, then releases with `valusema` (`0xc00000000018aeb8`) and `vsema` (`0xc00000000018ac88`). The handler identities are not known from the facts.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000038f08`

`intpend1` is the second pending-interrupt word and has the same shape as `intpend0`. It reads hub addresses `0x9200000001000090`, `0x9200000001000098` and `0x92000000010000a0`, writes `0x9200000001000090` inside its loop, and takes the most significant set bit through `ms1bit`. It calls an indirect handler through the same table layout. One exit path logs through `cmn_err` (`0xc0000000001f8090`), and the condition for that path is not clear from the facts.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc00000000003935c`

`sendintr` is a 72-byte helper. It reads the per-node table entry, a halfword at its +0x424 and a flag byte at its +0x520, then stores 8 bytes through a pointer computed from a halfword at +1060. The store is skipped when argument `a1` equals 0xab, and the routine returns 0. What 0xab selects and what gets stored are not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc0000000000393a4`

`cpuintr` is the per-CPU interrupt entry, at 276 bytes. It copies 0x168 bytes with `bcopy` (`0xc000000000020178`), then runs `doacvec` (`0xc0000000001e1a50`) and `doactions` (`0xc0000000001e1794`). It logs timestamp events through `log_tstamp_event` (`0xc0000000002383b0`) with codes 0x71 and 0x65 on its paths. The copied block and the event meanings are not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000039518`

`dbgintr` reads the halfword at `0xffffffffffffc420` and the table entry, stores 8 bytes to `0x9200000001000090` and reads `0x9200000001000098`. It then calls `debug` (`0xc00000000004b33c`) with a per-node pointer, a value taken from the table entry at +1312 with offset 118, and the hub address `0x9200000001000090`. It returns what `debug` returns. The debugger's contract is not established by these facts.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000039810`

`chkdebug` reads the same halfword and table entry as `dbgintr`, and reads the hub word at `0x92000000010000a0`. It calls `dbgintr` (`0xc000000000039518`) with its own incoming arguments. A bit test of that hub word against the value `dbgintr` returns chooses the value `chkdebug` returns. The meaning of that bit is not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000039934`

`debug_stop_all_cpus` loops over CPUs. Each pass calls `get_cpu_slice` (`0xc00000000001e0ec`) and `cpu_enabled` (`0xc000000000026f74`), skips the CPU whose index matches the halfword at `0xffffffffffffc420`, and writes a word to hub address `0x9200000001800090`. The loop bound is read from `0xc0000000014d13a0`. The store is probably a per-CPU interrupt or stop register, but the facts do not identify it.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000039b44`

`intr_clear_bits` reads the first 64-bit word of its pointer argument and loops over a pair of masks, reading `0x9200000001000098` and writing `0x9200000001000090` in the loop. Bit tests on the argument's words select which steps run. The register roles and the layout of the mask pair are inferred from the access pattern only.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc00000000003ca64`

`get_my_slotid` reads the 64-bit word at hub address `0x9200000001220048` and calls `hub_slotbits_to_slot` (`0xc00000000003c7ec`) with its incoming arguments. It returns that call's value directly. The identity of the register at `0x9200000001220048` is not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc00000000003dea4`

`llp_pfail_intr` is a 792-byte power-fail handler. The name points to the link layer (LLP), and the routine calls `msc_owner_cpu` (`0xc00000000003dbd0`) and `llp_pfail_barrier` (`0xc00000000003dc78`). It also calls `vector_read` (`0xc00000000001cfe4`) and `vector_write` (`0xc00000000001d02c`) several times, with fixed waits through `us_delay` (`0xc000000000047274`) of 100 and 300 microseconds. It reads and writes the hub words at `0x9200000001600110` and `0x9200000001600008`, and its loop bound is not identified. The target of the vector transactions and the reason for each wait are not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc00000000003f6d8`

`force_system_shutdown` writes `0x9200000001600008`, prints a message through `cmn_err` (`0xc0000000001f8090`), waits 500000 microseconds with `us_delay`, and calls `elsc_system_reset` (`0xc00000000009b718`). The sign of that return value feeds a branch before a `printf` (`0xc0000000001f8f2c`) and a one-second `us_delay`. The controller behind the reset and the success condition are not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000042ed8`

`bootstrap` reads a 64-bit word at fixed address `0x9200000001600000` and two halfwords, then calls `mapped_kernel_setup_tlb` (`0xc000000000042fa8`) with its incoming arguments. It ends in an indirect jump (`jr` at `0xc000000000042f4c`) to the constant address `bootstrap+124`. The facts do not follow that jump, so the code at `bootstrap+124` is not covered here.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000044f44`

`counter_intr` is a 676-byte timer interrupt. It reads a 64-bit counter at `0x9200000001030100`, then reads and resets the count and compare through `_get_count` (`0xc000000000052c30`), `_get_compare` (`0xc000000000052c3c`) and `resetcounter` (`0xc000000000052c54`). It calls `frs_handle_counter_intr` (`0xc0000000003c4010`), `hwperf_overflowed_intr` (`0xc0000000000574ac`), `hwperf_intr` (`0xc0000000000580f8`) and `l2_sbe_intr` (`0xc000000000054d30`) on separate paths, and calls `clock` (`0xc0000000001b3594`). The name and accessors suggest the CPU count/compare timer, but the facts do not establish it.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000046790`

`doadjtime` applies a time-of-day adjustment under the spinlock at `0xc0000000014d143c`. It takes the lock with `nested_spinlock` (`0xc000000000287280`) and releases it with `mutex_spinunlock` (`0xc000000000287260`), raising and restoring the priority level with `spl65` (`0xc00000000003b094`). It reads `__nanotime` (`0xc00000000004697c`) twice, and its loop compares against 1,000,000 and 1,000, which suggests microsecond and millisecond step bounds. The exact update rule is not established by these facts.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc00000000004697c`

`__nanotime` takes the lock at `0xc0000000014d143c` through `mutex_spinlock_spl` (`0xc0000000002871c0`) with `spl65`, reads the 64-bit counter at `0x9200000001030100`, and updates two cached words at `0xc0000000014dce90` and `0xc0000000014dce98`. It carries the count into seconds and nanoseconds, comparing against 1,000,000,000 (0x3b9aca00), and unlocks with `mutex_spinunlock`. The conversion constants come from the comparisons, not from a documented counter scale.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc00000000004710c`

`findcpufreq_cycles` measures the CPU cycle rate. It raises the priority with `spl1` (`0xc00000000003affc`), clears the count with `_set_count` (`0xc000000000052c48`), and polls the counter at `0x9200000001030100` until it passes its starting value plus 0x1312d0 (1,250,000) ticks. It then reads the count with `_get_count` (`0xc000000000052c30`), passes the result to `cpu_cycles_adjust` (`0xc00000000001e5b8`), restores the level with `splx` (`0xc00000000003b0e4`), and stores 4 bytes at `0xffffffffffffc744`. The unit of the window and the meaning of the stored value are not established.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000047bc0`

`acktmoclock` acknowledges the timeout clock. It always writes hub address `0x9200000001000128`, and writes `0x9200000001000120` only when a flag byte at table entry +1312 is nonzero. It returns the halfword at `0xffffffffffffc420` shifted left by 4. The register meanings are inferred from the store pattern.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000047bf8`

`ackkgclock` has the same shape as `acktmoclock`, on hub addresses `0x9200000001000138` and `0x9200000001000130`. It always writes `0x9200000001000138`, writes `0x9200000001000130` only when the flag byte at table entry +1312 is nonzero, and returns the `0xffffffffffffc420` halfword shifted left by 4. The register meanings are inferred from the store pattern.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000047c30`

`startkgclock` starts the kg clock under the spinlock at `0xc0000000014d14a0`, taken with `mutex_spinlock_spl` and released with `mutex_spinunlock`. It calls `cnodetocpu` (`0xc000000000026d14`) with the halfword at `0xffffffffffffc422` and a constant 1, and calls `setkgvector` (`0xc000000000039294`) with the function address `0xc0000000000489d0` as its argument. It writes hub addresses `0x9200000001000158`, `0x9200000001000118` and `0x9200000001000150`, and two structure words at `[-0x3c00]+0x4930` and `[-0x3c00]+0x4928`. The handler behind `0xc0000000000489d0` is not identified.
Grade: inferred.

### `6.5.22/unix.IP27` at `0xc000000000047d48`

`slowkgclock` shares the lock at `0xc0000000014d14a0` and the `ackkgclock` call with `startkgclock`, but it has no `cnodetocpu` call. It calls `spinlock_init` (`0xc000000000286fe8`) on that lock with `0xc00000000140e418` as the second argument, and a branch on a flag word at `0xc0000000014c9080` decides whether that call runs. It writes hub addresses `0x9200000001000158` and `0x9200000001000150`, and 32-bit words at `0xffffffffffffc534` and `[-0x3c00]+0x4930`. Which path runs in normal operation is not established.
Grade: inferred.
