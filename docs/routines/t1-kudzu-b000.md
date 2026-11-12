# Tier-1 routine readings, kudzu batch 0

Role statements for the first 25 routines of the tier-1 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. The facts show what each routine touches, not what the fields
mean, so the statements leave the meaning open where the facts stop. Addresses
are in the binary named in each header. Hub registers appear as the uncached
`0x92...` addresses the facts report.

### 6.5.22/unix.IP27 0xc00000000001d434

`get_nasid` is a 28-byte leaf. It loads one doubleword from 0x9200000001600000 and
returns in v0 a value the facts leave unresolved, so it is not shown to return the
loaded word unchanged. It is called from `cpu_node_probe` (call at 0xc00000000001f194),
`mlreset` (0xc000000000025fe8), `getcpuid` (0xc000000000026ed8) and `hubpi_eint_handler`
(0xc000000000032910). The name says node id, but the facts do not show which bits carry
it.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000001d450

`get_slice` is a 16-byte leaf that loads one doubleword from 0x9200000001000020 and
returns it unchanged. `getcpuid` (0xc000000000026ec4) reads that same word directly
at 0xc000000000026ee4 and passes it to `nasid_slice_to_cpuinfo` (0xc00000000001e274).
The name suggests a slice number, but no field extraction appears here.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000001d460

`is_fine_dirmode` is a 20-byte leaf. It loads one doubleword from 0x9200000001600000,
the same word `get_nasid` reads, and returns in v0 a value the facts leave unresolved,
so it is not shown to return the word unchanged. `mlreset` calls it at
0xc000000000026040; the facts for this routine do not show how the caller uses the
result. The name suggests a directory mode test, and the facts do not show the test.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000001d530

`ni_reset_port` is a 16-byte routine with one store, a doubleword to
0x9200000001600008 at 0xc00000000001d53c. That address is the next doubleword after
the word `get_nasid` reads. The stored value is not resolved, so the reset it
performs is only suggested by the name. Its caller is `restart` (0xc000000000029650).

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000001f0c8

`cpu_node_probe` is a 556-byte routine with a loop. It stores halfwords and words
through pointers in its arguments and in a global table, and it branches on a loop
register compared against 0x40, 0x80 and 0x100. It calls `get_nasid` (0xc00000000001f194) and `do_cpumask`
(0xc00000000001f2f4), and it calls `assfail` with the constant 0xb9 on one failure
path. Its caller is `mlreset` (0xc000000000026080). What the tables hold is not
clear from the facts.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000025768

`sn00_vmc` is a 200-byte routine that reads an absolute doubleword at 0xffffffffffffc400
and a halfword at 0xffffffffffffc422, then walks an indirect table. It calls
`device_inventory_add` (0xc0000000001c1a18) and a set of PCI bridge hint routines
(`pcibr_hints_dualslot`, `pcibr_hints_subdevs`, `pcibr_hints_fix_rrbs`,
`pcibr_set_rrb_callback`). It then calls `spl65` and `get_my_slotid`, and branches at
0xc0000000000257fc on a comparison of the constant 1 with a value the facts leave
unresolved. The code has four stores to 0x9200000001220050, two on each side of that
branch, so two run on a given path, and `splx` is called after them. The name points at a
board controller, but the facts do not explain it, so that is open.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000025eb0

`mlreset` is a 1020-byte routine with many calls. Role (from name): machine-level reset.
In address order it calls `r1nkperf_control_register_set` and `r1nkperf_data_register_set`
for indexes 0 and 1 and then `get_cpu_irr`. Branches then compare unresolved registers with
0xe and 0xf, a register with zero, and the word at 0xc0000000014c97cc with 1, 2 and 3. The
paths they select reach `r1nkperf_control_register_set` and `r1nkperf_data_register_set`
for indexes 2 and 3, `printf`, `branch_prediction_fwd_not_taken`, `branch_prediction_off`
or `branch_prediction_taken`, or `flush_tlb` and `flush_cache` followed by a return. Later in
address order it calls `get_nasid`, `set_master_bridge_base`, `ioc3_mlreset`,
`nvram_baseinit`, `is_fine_dirmode` and `is_specified`. When `is_specified` returns nonzero
it calls `atoi` and `redo_nodes`. It then stores to `boot_cpumask` + 8 at
0xc000000000026078, calls `cpu_node_probe` (0xc000000000026080) and stores to
`boot_cpumask` at 0xc000000000026084. Calls after that include `gen_region_mask`,
`getcpuid`, `get_cpu_cnode`, `setup_replication_mask`, `get_nasid`, `spinlock_init`,
`iograph_early_init`, `mem_init`, `hubdev_init`, `gda_sym_init`, `verify_snchip_rev`,
`he_arcs_set_vectors`, `intrgroup_init` and `branch_prediction_on`. It calls `assfail` at
0xc000000000026264, 0xc000000000026280 and 0xc00000000002629c with the constants 0x117, 0x11a
and 0x12c. The facts do not settle in what sequence the paths in the first part run.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000026ec4

`getcpuid` is a 116-byte routine. It wraps a `spl1` and `splx` pair around a call to
`get_nasid` and a lookup, `nasid_slice_to_cpuinfo` with the slice word from
0x9200000001000020. It asserts if the lookup returns nothing (constant 0x525) and
otherwise returns the halfword at offset 0x1e of that lookup result. The name says
CPU id, but the facts do not show that the returned halfword is one.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000029560

`restart` is a 356-byte routine with delays. It calls `us_delay` with 0xf4240 and
0x2dc6c0, and `sysctlr_power_off` with 0 as its first argument. It calls
`ni_reset_port`, then `elsc_init` and `elsc_system_reset` on a stack buffer, and it
prints one message through `printf`. It also stores to 0x9200000001600008 at
0xc0000000000296b4. The name suggests a machine restart, and the calls fit that, but
the order and the exact outcome are not settled by the facts.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002ae54

`page_local_calias` is a 60-byte routine. It loads one doubleword from
0x9200000001000028 and returns 0 or 1. The return depends on whether that word is
zero and on an unsigned compare of two registers at 0xc00000000002ae7c. The name
suggests a local alias check for a page, but the facts do not show which alias.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002bcac

`bte_init` is a 232-byte routine that fills a structure passed in `a0`. It stores
doublewords at offsets 0x0, 0x18, 0x20, 0x28 and 0x30, halfwords and words at 0x40
and 0x54 and 0x70, and it stores to 0x92000000014001f8. It calls `spinlock_init` on
the structure at offset 0x44 and returns `a0`. The name suggests a block transfer
engine. The facts show a structure and a lock, and they do not show engine
registers.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002e2f0

`net_link_up` is a 20-byte leaf. It loads one doubleword from 0x9200000001600000 at
0xc00000000002e2f4, the same word `get_nasid` reads, and returns in v0 at 0xc00000000002e2fc a
value the facts leave unresolved, so it is not shown to return the word unchanged. The name
suggests a network link state check. The facts show only the load and the return.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002ee44

`save_cache_error` is a 360-byte routine that calls `get_nasid` and then reads error
state. It loads a doubleword through a table plus 0x48, and it loads several words
and bytes. It also loads from 0x9200000001000020 and 0x9200000001030100, and it stores
a series of doublewords and a word and a byte. Branches test register values
against 0x10 and zero. The name says it saves a cache error, and the stores fit a
save area. The facts do not show the record layout.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002f7f0

`save_hub_pi_error` is a 256-byte routine that copies doublewords from a structure in `a0`
into a second structure in `a1`. It loads the doublewords at offsets 0x50 and 0x58 and
branches on whether each is zero. It copies the doublewords at 0x400, 0x430, 0x440, 0x450
and 0x460 to offsets 0x10, 0x28, 0x38, 0x30 and 0x40. The copies at 0x430 and 0x450 are
gated on bit tests against 0x8000000000000000 and 0x2 and on comparisons with 0x480041 and
0x1fffffe0. It reads one halfword from 0xffffffffffffc524. The name suggests a copy of
processor interface error state. The facts show the copy but not the record meaning.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002f8f0

`save_hub_md_error` is a 108-byte routine that copies up to four doublewords from `a0` at
offsets 0x200050, 0x200060, 0x200070 and 0x200080 to offsets 0x8, 0x28, 0x18 and 0x38 of a
second structure. Each of the first three copies sits behind a test of a register derived
from the word just loaded, which the facts leave unresolved, and the last is skipped when
its word is zero. A path through 0xc00000000002f94c tests the sign of the doubleword at
offset 0x200050 shifted left by one and makes one more store at 0xc00000000002f958. The
facts show no counted loop. The name suggests memory directory error state, but the facts
show conditional copies and a sign test, and they do not show what the fields are.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002f95c

`save_hub_io_error` is a 216-byte routine. It clears 0xc0 bytes of `a1` through `blkclr`
at 0xc00000000002f978, then copies seven doublewords from `a0` into `a1`. The offsets read
from `a0` are 0x400008, 0x410000, 0x410008, 0x410010, 0x420000, 0x420008 and 0x420010. A loop
runs while its index is below 15. The routine returns the doubleword at `a0` + 0x420008. The
name suggests I/O error state, and the offsets fit three register blocks, but the block
names are not established.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002fa34

`save_hub_err_ext` is a 184-byte routine. It clears `a1` through `blkclr` (call at
0xc00000000002fa50) with 0xc0 bytes, then loads and stores doublewords at 0xc00000000002fa80,
0xc00000000002fab0 and 0xc00000000002fad8 on paths chosen by comparisons of `a2` against 0xf
(branches at 0xc00000000002fa88 and 0xc00000000002faac) and 9 (0xc00000000002fad4). The name
suggests extended hub error state. The facts show the copy paths but not the fields they copy.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000002faec

`save_hub_ni_error` is an 88-byte routine. It clears 0x20 bytes of `a1` through `blkclr`
(call at 0xc00000000002fb04), then copies the doubleword at `a0` + 0x600300 (load at
0xc00000000002fb2c) to `a1` + 0x8 and the one at `a0` + 0x608008 (load at 0xc00000000002fb34) to
`a1` + 0x10. It returns `a1` (return at 0xc00000000002fb3c). The name suggests network
interface error state. The facts show a clear and two copies.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000032258

`hub_error_init` is a 928-byte setup routine. It calls `hub_error_clear`, then
`get_compact_nodeid`, and it calls `recover_error_init` twice. It allocates buffers
with `kmem_zalloc_node`, sizes 0x1938 and 0x40. It stores to several hub registers in
the 0x9200000001000408 to 0x9200000001000468 range. It connects the interrupt handler
at 0xc00000000003288c (`hubpi_eint_handler`) with the first argument 6, and it calls
`hubii_eint_init` and `hubni_eint_init`. Its failure paths call `assfail` and
`cmn_err`. The facts show setup and wiring. The handler binding is established by the
call's address argument.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc00000000003288c

`hubpi_eint_handler` is a 728-byte routine. It reads hub status words, and it writes
0x9200000001000400 five times, which is consistent with clearing status. It calls
`check_ni_errors` first and calls `hubni_error_handler` only when that returns nonzero.
It calls `log_md_errors`, then `process_error_spool` three times with different
flags, each behind a bit test on the status word. It reaches `enter_panic_mode` or
`kl_panic` on some paths. Bit tests on the status word cover 0x1, 0x2, 0x4, 0x8, 0x10,
0x20, 0x40, 0x80, 0x100000 and 0x200000. The wiring in `hub_error_init` suggests it is an
interrupt handler, but the facts alone do not prove that.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000032ca4

`check_ni_errors` is a 604-byte routine. It reads hub words at 0x9200000001608008,
0x9200000001600000, 0x9200000001600010, 0x92000000010000a0 and 0x9200000001608088. It
tests bits in them against masks such as 0x3eff000000 and 0x8000000000, and it calls
`cmn_err` for many messages. It calls `protected_broadcast` with codes 0x67 and 0x68.
It returns 0 or 1. The facts show the checks and messages, not the meaning of the bits.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000032f00

`hubni_error_handler` is a 392-byte routine. It reads the NI status words at
0x9200000001608008 and 0x9200000001600010, and it writes 0x9200000001600008 at
0xc000000000032fa4. On the path that passes the two status tests its first call is `spl7`.
When a byte flag tests nonzero it then calls `set_node_panic_flag`, `get_nasid` and
`hubni_print_state`, then `enter_panic_mode`. When `a1` is nonzero it calls `ip27log_printf`
and makes the store at 0xc000000000032fa4. It then calls `router_print_state` and
`ip27log_printf`, then `enter_promlog_mode`, `get_nasid`, `hubni_print_state` and
`router_print_state` a second time, and calls `panicspin` only when `a1` is zero. The other
paths call `cmn_err`. The name says NI error handler, and the printing and panic calls fit
that. The facts do not show which error each branch handles.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc0000000000339a8

`klsave_spool_registers` is a 360-byte routine. It calls `get_nasid` several times,
then `find_lboard` with the type constant 0x11 and `find_first_component` with the
constant 2 on its result. It reads 0x9200000001000020 and 0x9200000001030100, and it
stores several doublewords into a buffer. Branches test bit 0 of a value and the
result of the lookups. The name says save spool registers. The facts show the lookups
and the saved words, and not the record format.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000034d94

`hub_error_status_war` is a 96-byte routine. It reads 0x9200000001000020, 0x9200000001000450
and 0x9200000001000430. It calls `_badaddr_val` on the address 0x9200000002400208 with size
8, behind tests that the first word is nonzero and that bit 63 is clear in the other two.
Role (from name): the `_badaddr_val` call probes whether that address responds. The name of
the routine suggests a workaround for a hub status error. The facts do not confirm that.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)

### 6.5.22/unix.IP27 0xc000000000034df4

`ii_llp_error_monitor` is a 104-byte routine. It loads and stores 0x9200000001400130 and
two doublewords at offsets 0x0 and 0x8 of `a0`, and it reads and writes two bytes at
offsets 0x10 and 0x11. Two comparisons test a value against 0xffff. The name points
at the link-level protocol error state of an I/O interface. The facts show a
read-and-write-back pattern, and what that pattern does is not settled by the facts.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)
