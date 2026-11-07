# Resource control and NUMA placement

What the IRIX 6.5.30 kernels offer for confining work to processors and nodes (cpusets), for batch scheduling (miser), for per-job limits (job limits), and for placing memory (placement objects, migration). The question behind the page is practical: if something outside the kernel wants to treat a group of processes as one unit with a processor set, a memory budget, a process count and a file count, which of those exist in the shipped kernel, through which entry points, and which are only present as unlinked code.

Evidence is the linked kernels `6.5.30/unix.IP35` (read first) and `6.5.30/unix.IP27`, with `6.5.22/unix.IP35` for one symbol check, plus the kernel object archive `6.5.30/usr/cpu/sysgen/IP35boot/jlimits.a` and the 6.5.22 configuration tree (`var/sysgen.IP35`: `master.d`, `mtune`, `system`, `master.c`). Routine names, sizes and addresses come from the kernels' symbol tables where the routine has an entry there (for example `jlimits_init`, `hub_migrintr_init` on IP27) and otherwise from the debug information the kernels carry (for example `cpumask_to_nodemask`, `generate_schedmasks`, `cpuset_load_volatile` and `cpuset_get_volatile`, which the fact tool resolves from the inventory); instruction facts come from `retools/scripts/hostfacts.py` and a capstone walk of the same images. No source tree and no manual text was used; where a name below has no entry in either, it is said so. The two shipped user-space commands that would sit on top of these calls (the cpuset, miser and placement tools) are not held, so nothing here says how a command forms its arguments.

Conventions are those of [syssgi.md](syssgi.md) and [syscalls.md](syscalls.md): a handler receives a pointer to the argument block in its first register; the selector is the doubleword at +0, `arg1` the doubleword at +8, `arg2` at +16; a 16-bit value in the low half of `arg1` is at +14. The return register carries an errno; a result goes through the second pointer. Numbers are decimal unless written `0x`.

## Summary

| Facility | Interface | State in the shipped kernels |
|---|---|---|
| cpuset | `sysmp` selector 35, one request record, 24 operations | linked and live (IP35 and IP27) |
| miser | `sysmp` selectors 36 to 41 | linked and live |
| job limits | `syssgi` selectors 1210 to 1220 | **stubs only**: every entry returns 65; the real code is in `jlimits.a`, which is not linked |
| per-process limits | `setrlimit` defaults `rlimit_*` | linked; per process, not per job |
| placement and migration | `syssgi` 208 (`pmoctl`), 212 (`numa_tune`), 218, 1103 | linked and live ([syssgi.md](syssgi.md)) |

## The `sysmp` entry point

- `sysmp` (`6.5.30/unix.IP35`, 0xc0000000002ee5a4, 5200 bytes; system call 1056) multiplexes processor and scheduler operations the way `syssgi` does for machine operations. It compares the selector (the doubleword at +0) unsigned with 55 (`sltiu` at 0xc0000000002ee65c) and jumps through a 55-entry table at 0xc0000000004d22e8; an out-of-range selector, and any entry that is a gap, ends at 0xc0000000002ee67c with error 22. Selectors 16, 17, 18, 22, 23, 24, 25, 26, 50, 51, 52 and 53 first check the low word of `arg1` against `numcpus` (the word at gp+0x2820, 0xc000000001645f90) and return 22 when it is negative or not below it (0xc0000000002ee640 to 0xc0000000002ee6a0). Established. The selector table was extracted with `retools/scripts/syssgi_selectors.py --function sysmp`; the sysmp table is otherwise not yet in the inventory, see the open question at the end.

`sysmp` on IP27 is at 0xc00000000023e908 (5124 bytes), with a 54-entry table at 0xc0000000003ed888 (largest selector 53). IP35 has one more selector, 54, whose case code (0xc0000000002ee6bc) makes no call. Established.

Handlers for the selectors this page needs (IP35 case code; the callee is read from the case, the argument roles from the loads it makes):

| Selector | IP35 case code | Calls | Notes |
|---|---|---|---|
| 16 | 0xc0000000002eed84 | `cap_able` (1 << 33), `cpuset_destroy` | CPU number in the low half of `arg1` |
| 17 | 0xc0000000002eed14 | `cap_able` (1 << 33), `cpuset_create` | CPU number in the low half of `arg1`; the master CPU (`master_procid`) is refused; the request is built on the stack |
| 22, 50 | 0xc0000000002eebc4, 0xc0000000002ee7bc | `cap_able`, `mp_isolate` (50 also `procscan`) | `mp_isolate` (0xc0000000002f0474, 680 bytes) itself calls `cpuset_create` with a stack request (the constant 0x300 is loaded at 0xc0000000002f0538) |
| 23, 51 | 0xc0000000002eeb88, 0xc0000000002ee758 | `cap_able`, `mp_unisolate` | `mp_unisolate` (0xc0000000002f071c, 448 bytes) |
| 35 | 0xc0000000002ee9e4 | `cpuset_process(arg1, arg2)` | the cpuset interface |
| 36 | 0xc0000000002ee9d4 | `miser_get_request(arg1)` | |
| 37 | 0xc0000000002ee9c0 | `miser_send_request_scall(arg1, arg2)` | |
| 38 | 0xc0000000002ee9b0 | `miser_respond(arg1)` | |
| 39 | 0xc0000000002ee978 | `miser_get_quantum`, `miser_get_bids`, `miser_get_job` | sub-command in `arg1` |
| 40 | 0xc0000000002ee944 | `miser_set_quantum`, `miser_set_resources`, `miser_reset_job` | sub-command in `arg1` |
| 41 | 0xc0000000002ee930 | `miser_check_access(arg1, arg2)` | |
| 42 | 0xc0000000002ee920 | none | stores `numnodes` (gp+0x2824) through the result pointer |
| 43 | 0xc0000000002ee8fc | `copy_distmatrix`, `copyout` | node distance table, see NUMA |
| 44 | 0xc0000000002ee8dc | `build_cpumap`, `copyout` | CPU map, see NUMA |

IP27 has the same callee for selectors 35 to 38 and 41 (case code at 0xc00000000023ed10, 0xc00000000023ed00, 0xc00000000023ecec, 0xc00000000023ecdc and 0xc00000000023ec5c). Established from the table walks. Which public name belongs to which selector number was not checked; the symbol tables name the handlers, not the selectors.

## Cpusets

A cpuset here is a kernel object that names processors and nodes, owns a memory budget, and has processes attached to it. All 24 operations arrive through one routine.

- `cpuset_process` (`6.5.30/unix.IP35`, 0xc00000000024c450, 596 bytes; IP27 0xc00000000019da50, 600 bytes). Allocates a 0x470-byte (1136) record with `kmem_alloc`, zeroes it, and `copyin`s 0x470 bytes from `arg1` (the call at 0xc00000000024c490); a failed copy returns 14. The halfword at +0 of the record is the operation. An operation of 0x18 (24) or more returns 22. Otherwise the code jumps through a table of 24 code addresses at 0xc0000000004c8580 (the pointer in the doubleword at gp minus 10752 plus 456). Each case calls its routine with the record's body, which is the record plus 8; `arg2` is passed on as a second argument by the cases that take one. On success every operation copies the whole 0x470 bytes back to `arg1` (`copyout` at 0xc00000000024c528, error 14 if that fails), so the request and the reply share one layout. Established. The record layout itself was not decoded, only the fields the cases and `cpuset_create` read.

| Operation | Routine (IP35) | Operation | Routine (IP35) |
|---|---|---|---|
| 0 | none: returns 22 | 12 | `cpuset_get_properties` |
| 1 | `cpuset_attach` | 13 | `cpuset_move_op` (last argument 1) |
| 2 | `cpuset_create` | 14 | `cpuset_move_op` (last argument 0) |
| 3 | `cpuset_destroy` | 15 | `cpuset_get_mems` |
| 4 | `cpuset_list_procs` | 16 | `cpuset_get_nodes` |
| 5 | `cpuset_procs_count` | 17 | `cpuset_get_cpulimits` |
| 6 | `cpuset_move_procs` | 18 | `cpuset_get_memlimits` |
| 7 | `cpuset_get_cpus` | 19 | `cpuset_get_unixperm` |
| 8 | `cpuset_query_current` | 20 | `cpuset_get_trustperm` |
| 9 | `cpuset_query_names` | 21 | `cpuset_get_flags` |
| 10 | `cpuset_attach_pid` | 22 | `cpuset_load_volatile` (0xc00000000024bb0c, 364 bytes) |
| 11 | `cpuset_detach_pid` | 23 | `cpuset_get_volatile` (0xc00000000024bc78, 280 bytes) |

The table above is the IP35 table read from the image; the IP27 table (pointer at 0xc0000000003e1088 plus 0x1c8) has the same callees for 0 to 21, and `cpuset_load_volatile` (0xc00000000019d1e0, 312 bytes) and `cpuset_get_volatile` (0xc00000000019d318, 308 bytes) for 22 and 23. Established. The names come from the debug information, not the symbol table; what the two operations do beyond their names is unknown, because neither routine was read.

- `cpuset_create` (`6.5.30/unix.IP35`, 0xc000000000247534, 3744 bytes; IP27 0xc000000000199194, 3016 bytes). Reads the properties doubleword at +8 of the request body. When bit 0x200 of it is set the caller must pass `cap_able_cred` with capability 1 << 33 (the constant is built at 0xc000000000247594 to 0xc00000000024759c); a refusal returns 1. The calls it then makes, in address order, include `cpu_enabled` (0xc00000000024778c), `cpumask_to_nodemask` (0xc000000000246d04, at 0xc000000000247954 and 0xc000000000247d04), `cpuset_share_node`, `complement`, `mutex_spinlock` and `find_cpuset` (0xc000000000247e04, 0xc000000000247e18), `check_cpuset`, `generate_schedmasks`, `inc_mem_exclusive`, `update_all_mems`, `set_cputocpuset` and `cpuset_init_memsys` (0xc0000000002481a4), `getf`, `strncmp`, `sthread_boot_bind` and `xthread_boot_bind` (0xc0000000002482fc, 0xc000000000248304), and `copyin` of two user buffers sized from the counts at +0x30 (0xc000000000247a28 to 0xc000000000247ab4). The request offsets it reads are +0x0, +0x8, +0x10, +0x18, +0x20, +0x28, +0x30, +0x38, +0x40, +0x44, +0x48 and +0x50. Established for the capability gate, the calls and the offsets; what each field holds, and the order of the checks, is inferred from the callee names only.

State is a table and a CPU-to-cpuset map set up at boot. `cpuset_init` (`6.5.30/unix.IP35`, 0xc000000000246728, 560 bytes; IP27 0xc00000000019860c, 516 bytes), called from `init_batchsys` (0xc000000000241f98) at 0xc00000000024208c. It sets up these tables:

`max_cpusets` (0xc000000001645ac0) is `maxcpus + 2`; `cpusets` (0xc000000001645aa8) is a `kmem_zalloc` of `maxcpus * 256 + 512` bytes, which is `max_cpusets` records of 256 bytes (the shift by 8 in every index computation agrees); `cpu_to_cpuset` (0xc000000001645ab0) is `2 * maxcpus` bytes, a halfword per CPU; `mem_exclusive` (0xc000000001645ab8) is `2 * maxnodes` bytes, a halfword per node, cleared with `blkclr`. Established from the loads and calls.

The first record (the table base, no index) gets the master processor in its halfword at +0x104 (`master_procid`) and its node mask at +0x170 and +0x178 from `boot_cnodemask`, two doublewords on IP35; the same two doublewords go into `kernel_allow_cnodemask` and `global_schedmask`. Established. That the first record is the whole-system set is inferred from it taking the whole boot mask. On IP27 `cpuset_init` stores a single doubleword of `boot_cnodemask` into `kernel_allow_cnodemask` and `global_schedmask` and the listing has no access to `boot_cnodemask+8`: the mask is one doubleword there and two on IP35. Established as absence of that access in the IP27 listing.

`cpuset_default` (0xc00000000024c080, 164 bytes) is called when a kernel thread is created (`xthread_create`, `sthread_create_thread`), so new kernel threads start in a default set. Inferred from the call sites; the body was not read.

Where the scheduler and the allocator consult cpusets:

`mp_mustrun` (the routine behind CPU binding) calls `cpuset_check_access` (0xc00000000024bd90, 284 bytes) at 0xc0000000002efe08 and `cpuset_move_thread` (0xc0000000002483e8, 140 bytes) at 0xc0000000002f002c. `cpuset_check_access` looks up the cpuset through `cpu_to_cpuset` and, under the cpuset's lock and a behaviour read lock, calls a method through a register with the constant 0x40 (the limit getters pass 0x100). A boolean kernel tunable, `cpuset_nobind` (0xc00000000163c1ac), says a process under miser-managed cpusets may not bind itself to a CPU (`mtune/kernel`, default 0). Established for the call graph; that the 0x40 call is the "may bind" test is inferred.

Memory: a page-allocator routine at 0xc00000000025b8b4 (no symbol; it follows `pagealloc_size_node_hint`) calls `cpuset_memalloc_ok` (0xc00000000031dcfc, 272 bytes; IP27 0xc00000000026ee70, same size). That routine takes a record through the per-processor pointer at absolute address 0xffffffffffffc018 (presumably the current thread; inferred), reads the cpuset index from its halfword at +0xf4, loads a pointer to the cpuset's memory record (cpuset +0xd0), compares a counter at +0x20 of it with `maxfailcount` (0xc00000000163c6a8) and calls `signal_proc_nomem` with 1 or 0. So an allocation that cannot be satisfied inside a cpuset's own memory is counted and, past a limit, the process is signalled. `cpuset_kick_vhand` (0xc00000000031d2f0) is called from `vhand_router`. Established for loads and calls; that this is the cpuset memory limit is inferred from the routine names and from the comment text on the cpuset memory tunables in `mtune/kernel`, not from the code; that comment names `nodeslimitfrac` (the node fraction past which the system uses global memory management instead) and `chunk_nodeshogfrac` (the fraction of pages on the set's nodes past which a buffer is flushed).

`get_effective_nodemask` (0xc00000000024bfac, 212 bytes) combines the cpuset's node mask (record +0x70, +0x78) with two masks in the placement structure it is given (+0x280, +0x288) and `boot_cnodemask`. Its callers include `physmem_mld_place`, `physmem_mldset_place`, `pmo_ns_pmolist_replace` and `memaff_getbestnode`, and unnamed routines in the page allocator and placement code. Established for the loads and the callers; the combining rule (intersection) is inferred.

`cpuset_teardown` (0xc000000000248d2c, 100 bytes) is called from `exitrunq`, so process exit detaches it from its cpuset. Inferred from the call site.

## Miser

Miser is a batch scheduler for sets of CPUs and memory kept by the kernel for jobs that ask for them in advance. The kernel half is a request queue between a user daemon and the kernel, plus per-job accounting.

Start-up: `init_batchsys` (`6.5.30/unix.IP35`, 0xc000000000241f98, 264 bytes) calls `miser_init` (0xc00000000024f4e8, 100 bytes) at 0xc000000000242084 and `cpuset_init`. `batchd` (0xc000000000241c04, 704 bytes) calls `miser_purge` (0xc000000000250d34, 196 bytes). `miser_queue` and `miser_batch` (0xc0000000016a8c00, 0xc0000000016a8c18) are 24-byte objects, probably queue heads (inferred). Established for the calls.

Daemon side: selector 36 `miser_get_request` (0xc00000000024fa04, 312 bytes) calls `mq_is_okay`, `copyout` and `sv_wait_sig`, which reads as: wait for a request, then copy it out; selector 38 `miser_respond` (0xc00000000024fbc8, 408 bytes) calls `copyin`, `rmq`, `miser_return`, `miser_handle_exception`, `miser_find_bid` and `pid_op`, which reads as: take the daemon's reply and complete the request or handle an exception. Established for the calls; the reading is inferred. The daemon is not held.

Client side: selector 37 `miser_send_request_scall` (0xc00000000024ff0c, 1224 bytes; IP27 0xc0000000001a14a8, same size) calls `copyin`, queues the request, sleeps (`sv_wait_sig`) and calls `copyout`. Among its calls are `vpag_miser_create` (0xc0000000002d8584, 652 bytes, at 0xc00000000025030c), `batch_create` (0xc0000000002423f8), `batch_set_params` and `for_each_thread_init_batch`, `miser_find_bid` and `user_resched`. So a miser job appears to be a process aggregate (`vpag_*`: `vpag_create`, `vpag_join`, `vpag_leave` exist) whose threads are put on the batch run queue. Established for the calls; the reading is inferred.

Queries and updates: selector 39 sub-commands 0x45 to 0x49 reach `miser_get_quantum` (0x45), a block shared by 0x46 and 0x47 that returns the word `batch_cpus` (0xc000000001645838) as the routine's result (the load sits in the delay slot of the branch to the common return, at 0xc0000000002ef40c), `miser_get_bids` (0x48) and `miser_get_job` (0x49); selector 40 sub-commands 0x45, 0x46 and 0x47 together, and 0x49, reach `miser_set_quantum`, `miser_set_resources` and `miser_reset_job`. `miser_set_resources` (0xc0000000002508fc, 404 bytes) needs capability 1 << 33 (0xc000000000250920), then resizes or creates `miser_pool` (0xc000000001645ae0) with `vm_pool_resize` and `vm_pool_create`, and recomputes the batch CPU set with `batch_init_cpus`. So the memory budget miser reserves is a kernel `vm_pool`, and the CPU budget is `batch_cpus`. Established for the calls. Which sub-command is which public operation is not read.

Memory accounting for jobs as units: `miser_inform` (0xc000000000250ea0, 116 bytes) looks up the bid (`miser_find_bid`) and sends a request to the daemon. It is called from the fault and growth paths: `pas_pfault`, `pas_vfault` (six call sites), `pas_addstackexec` and `dupreg` (two). A kernel variable `miser_rss_account` (0xc00000000163c32c, default 1) switches between exact and inexact accounting (`mtune/kernel`; the page states the cost trade). Established for the call sites.

Access: selector 41 `miser_check_access` (0xc000000000250a90, 332 bytes) calls `getf`, takes locks including a behaviour read lock and makes a call through a register. It reads as a check on the object behind the descriptor; the register call was not resolved, so that is inferred.

## Job limits

The kernel has a complete job-limit design, and the shipped kernels leave it out.

**Interface.** Eleven `syssgi` selectors, listed in [selectors.tsv](inventory/selectors.tsv) with the routines they reach on IP35 (case code addresses in parentheses): 1210 `syssgi_getjlimit` (0xc0000000002e2300), 1211 `syssgi_setjlimit` (0xc0000000002e308c), 1212 `syssgi_getjusage` (0xc0000000002e1a84), 1213 `syssgi_setjusage` (0xc0000000002e2a5c, after a `copyin`), 1214 `syssgi_getjid` (0xc0000000002e2520), 1215 `syssgi_killjob` (0xc0000000002e13a8), 1216 `syssgi_makenewjob` (0xc0000000002e1e38), 1217 `syssgi_getjobpid` (0xc0000000002e1b6c), 1218 `syssgi_joinjob` (0xc0000000002e2e0c), 1219 `syssgi_setwaitjobpid` (0xc0000000002e1458) and 1220 `syssgi_waitjob` (0xc0000000002e2b78). Same numbers on IP27 (case code 0xc000000000231a80 and neighbours). Established. The selectors that follow in the table (1231 to 1238) call `copyin` first and were not tied to this feature.

**What is linked.** In `6.5.30/unix.IP35` every one of these eleven routines is 28 bytes: save `ra`, `jal nopkg`, restore `ra`, return (for example `syssgi_setjlimit`, 0xc000000000496824; the eleven occupy 0xc0000000004967d0 to 0xc000000000496904). `nopkg` (0xc000000000256040, 8 bytes) returns 65. The hook routines the rest of the kernel calls are 8-byte constant stubs in the same block: `pproc_accumjob`, `pproc_getjob`, `proc_job_accum`, `mem_job_accum` and `job_limit_check` return 1700 (`addiu v0,zero,1700`); `pproc_isinjob`, `jlimits_getmachid`, `job_as_exit`, `proc_leavejob`, `proc_joinjob`, `proc_as_joinjob`, `proc_job_accumcpufin`, `set_jl_enforce_flag`, `jid_jobtab` and `release_jobtab` return zero or leave `v0` alone and do nothing; `jlimits_init` (0xc0000000004967b8) clears the word `jlimits_enabled` (0xc0000000016464a4). The same bodies are in `6.5.30/unix.IP27` (`jlimits_init` 0xc0000000003c2600, `syssgi_setjlimit` 0xc0000000003c266c, `job_limit_check` 0xc0000000003c2774) and in `6.5.22/unix.IP35` (`syssgi_setjlimit` 0xc0000000004435a4, `job_limit_check` 0xc0000000004436ac). None of the `jlimit_*` limit variables exists in the symbol tables of `6.5.30/unix.IP35`, `6.5.30/unix.IP27` or `6.5.22/unix.IP35`. Established. A caller of these selectors therefore gets 65 back on the shipped kernels (that the errno reaches the user unchanged follows the convention of the other selector handlers; inferred).

**Why the stubs are linked.** The configuration lists `USE: jlimits` and, further down, `USE: jlimitsstubs` in the IP35 system file (`system/irix.sm`, 6.5.22). The 6.5.22 IP27 system file lists the same two lines (493 and 524) and its `master.c` also names only `jlimitsstubs.a`. The link command recorded in `master.c` names `jlimitsstubs.a` and no `jlimits.a`. The `master.d` files give `jlimits` the "configurable object, not a driver" flags and the stub module the "load after all others" flag. The 6.5.30 object directory holds both archives for IP35 (`jlimits.a` 58820 bytes, `jlimitsstubs.a` 11444 bytes) and for IP27 (`jlimits.a` 59108 bytes). What decides which one is linked on a given system: `jlimits.a` is the optional subsystem `eoe.sw.jlimits` of the base product on the installation media, and the 6.5.30 IP35 `irix.sm` lists `USE: jlimits` (line 548) and `USE: jlimitsstubs` (line 580); `lboot` links a `USE`d module only when its object is in the boot directory and otherwise emits the stub module (the `lboot` strings and the `master.d` stub flag say so; not disassembled), so a system without that subsystem installed gets the stubs. Inferred: the system the shipped kernels were linked on lacked it. The job-limit tunables file ships in the base subsystem either way. A kernel linked with `jlimits.a` would carry the code described next, so the facility is relinkable, not absent. Established for the files and the link line; the selection rule is inferred as above.

**What the real code does (`jlimits.a`, 6.5.30 IP35).** The archive has three members, `jlimits.o`, `jid.o` and `jl_pproc.o`. Read from the objects' symbol tables and from `jl_pproc.o`:

Job identity: `jid.o` has `jid_init`, `jidtab_init`, `jid_alloc`, `jid_job_free`, `jid_jobtab`, `jid_dealloc`, `release_jobtab` and `rm_zombiejobs`, over a job table (`jidtab`, `jidtabsz`, `jidfirstfree`, `jidlastfree`) with a lock; `jlimits.o` has the eleven selector routines, `sendjidsig`, `findjob` and `global_jid_mask`. `jlimits.o` also calls `csa_jstart` and refers to `do_csaacct`: job accounting records are produced by the accounting code that is linked in the base kernel (`csa_acct`, `csa_jstart` 0xc000000000309340 and `csa_jexit` 0xc000000000309464 are present in `6.5.30/unix.IP35`). `acctctl` is the only kernel caller of `jid_jobtab` and `release_jobtab`, at 0xc000000000308974 and 0xc0000000003089f8. Established.

Limit check: `job_limit_check` (`jl_pproc.o`, 0x750, 68 bytes) takes a job record, a resource index and an amount. It returns 0 when the amount is 0 or the job pointer is null. Otherwise it adds the amount to the doubleword at +0x50 of the record's 32-byte entry for that resource (clamped at 0 when negative) and returns 1 when the doubleword at +0x40 of that entry is below the sum. So each resource has a limit at +0x40 and a running usage at +0x50, kept once per job, not once per process. Established from the disassembly.

Accounting: `proc_job_accum` (`jl_pproc.o`, 0x794, 0x1ec bytes) takes the process, the resource, a signed amount and flags. With a job pointer found at (the process's job info, process +0x7a0), it takes a per-resource spinlock (job +0x140 plus 4 times the resource), adds the amount to the job usage (+0x50) and to the process's own counter (+0x70 of its info record plus 8 times the resource), and raises a peak (+0x58). It stores the new usage only when the sum is within the limit or flag bit 2 forces it, and returns 1 for a crossing; when the job's enforcement word (job +8, mask 2) is set, it posts signal 9 (the constant is loaded at offsets 0x8b0 and 0x928 of the object): by process id with `sigtopid` when flag bit 4 is set, else to the process with `sigtoproc`, and writes a `cmn_err` line when `sigtopid` fails. A non-forced crossing returns 1, which is what makes `fdt_alloc` fail (see below). Established for the lock, the counters, the limit comparison, the flags and the signal constant; "kill the process that crosses the limit" is inferred from the signal number, taking 9 as SIGKILL by the usual numbering.

Resource numbering. The tunable file `mtune/jlimits` lists the limits in this order: cpu, data, vmem, rss, nofile, pthread, numproc, pmem, each with a soft (`_cur`) and hard (`_max`) value, and each with an `_ign` switch that turns accumulation and enforcement off for jobs created afterwards. The call sites pass indexes consistent with that order: `fdt_alloc`, `fdt_alloc_many` and `fdt_dup` pass 4 (open files); `pas_break` passes 1 and 2 and `pas_addshm` passes 2 (data and virtual memory); a static routine in the process-creation code after `sprocsp` (call sites 0xc0000000002eb9bc to 0xc0000000002ebb0c) passes 7, 6, 4, 1, 2 and 3, with 6 and an amount of 1 for the process count; `pproc_exec` passes 2, 1, 3 and 7; and `clock` passes 7, 1, 2 and 3 and one site whose index was not a constant. Established for the call-site constants; the index-to-name mapping is inferred from the order of the tunable file.

**Per-job limits for memory, process count and open files.** The set exists in the design: virtual memory (vmem), data size, resident set (rss) and physical memory (pmem) for memory; numproc and pthread for counts; nofile for open files; plus CPU time. All default in `mtune/jlimits` to 0x7fffffffffffffff (no limit); the lowest values the file accepts are 40 for the soft nofile limit and 1 for the pthread and numproc limits. In the shipped kernels none is available: the interface returns 65 and the accounting calls are stubs. Established for the design (tunable file and objects) and for the absence (stubs). The kernel call sites are real and would be live in a relinked kernel: `fdt_alloc` (0xc0000000002bcf9c) calls `proc_job_accum` with resource 4 and amount 1 at 0xc0000000002bd024 only when the doubleword at +0x7a0 of the process record is non-zero and a flag test on the job record passes (0xc0000000002bcfcc to 0xc0000000002bcfec); `closefd` and `closefp` call it with -1; `pproc_exec` and `sprocsp` call it for the other resources; `pas_break`, `pas_addmmap`, `pas_addmmapdevice`, `pas_addstackexec`, `pas_addshm` and `pas_stackgrow` call `job_limit_check` before growing an address space. Established.

**Does any hook enforce limits for a job as a unit?** Not in the shipped kernels. The job-wide check and accounting hooks are called from the file, process, exec and address-space paths, and with the stubs linked they do nothing, so no job-wide process count, file count or memory total is enforced by the shipped `unix.IP35` or `unix.IP27`. Established. The units that are enforced are narrower or different: per-process limits (below), the cpuset memory behaviour above (a cpuset is a unit for memory and CPUs, not for process or file counts), and miser's accounting of resident memory per job. The only routines of the real archive that attach a job to a process are `proc_joinjob` and `proc_as_joinjob`, and both are empty stubs here, so nothing stores a job at +0x7a0; a scan of every store to that offset in the kernel was not made. No per-cpuset process-count or open-file limit was found: `cpuset_get_cpulimits` (0xc00000000024b478, 276 bytes) copies two words from record offsets +0xe4 and +0xe8 and `cpuset_get_memlimits` (0xc00000000024b58c, 276 bytes) copies two doublewords from +0xf0 and +0xf8, and nothing else limit-shaped was seen in the 24 operations. Inferred from absence in the routines read; the unnamed operations 22 and 23 were not read.

## Per-process limits

The ordinary resource limits are kernel variables with `mtune/kernel` defaults: `rlimit_cpu`, `fsize`, `data`, `stack`, `core`, `vmem`, `rss`, `nofile` and `pthread`, each `_cur` and `_max` (for example `rlimit_nofile_cur` 0xc00000000163c4a8, `rlimit_nofile_max` 0xc00000000163c4a0, `rlimit_vmem_cur` 0xc00000000163c4c8). Two flags adjust file-limit behaviour (`rlimit_nofile_cur_clamps_setrlimit`, `rlimit_nofile_cur_clamps_fdhi`) and `reset_limits_on_exec` (0xc00000000163c764) decides whether limits are reset on exec; the table `rlimits` (0xc0000000016e0f88, 144 bytes) holds nine pairs. These apply per process. Established for the symbols; their role is inferred from the names and the defaults in the 6.5.22 tunable file.

## NUMA placement and migration

The placement interface is `syssgi` 208 (`pmoctl`): memory locality domains and sets, policy modules, page migration and node-mask control, 62 commands on IP27 and 103 table slots on IP35. [syssgi.md](syssgi.md) lists the commands and their callees, including 40 `migr_xface_migrate`, 41 `pmo_xface_as_migrate`, 42 `pmo_xface_as_migrate_close`, and the IP35-only commands 100 to 102 that move a 16-byte node mask. `numa_tune` (212), `numa_stats_get` (218) and `numa_tests` (1103) are in the same page. Nothing is repeated here beyond what ties them to cpusets.

Placement objects and a policy per address space: `pmo_init` (0xc000000000326e24), `mld_create`, `mldset_create`, `mldset_place`, `physmem_mld_place`, `physmem_mldset_place` (5784 bytes), the six placement policy modules (`plac_policy_default`, `fixed`, `firsttouch`, `thread`, `roundrobin`, `cachecolor`), the fallback policy and the migration policies (`migr_policy_default`, `control`, `refcnt`) are initialised in the order [klconfig.md](klconfig.md) gives. The per-address-space default policy and node mask go through `pmo_xface_aspm_setdefaultpm` and `pmo_xface_aspm_setcnodemask` (0xc00000000031caf0, 368 bytes). Established as symbols; behaviour is read only as far as the callers listed above.

Migration engine: `migr_vas_pages` (0xc00000000032e8e4, 812 bytes) migrates pages of an address space; `migr_coald_move_pages` (0xc00000000032db84), `migr_pfdat_xfer` and `migr_pfdat_do_xfer` move the page frames; `migr_check_migratable` and `migr_check_migr_poisoned` decide whether a page may move; `migr_page_unmap_tlb` flushes. On IP27 `hub_migrintr_init` connects the migration interrupt ([ip27-kernel-io.md](ip27-kernel-io.md)); IP35 has no routine of that name. Established as symbols and call relationships only.

Cpuset to placement: `get_effective_nodemask` (above) is how a cpuset limits where `physmem_mld_place` and `memaff_getbestnode` may put memory. Established for the callers.

User commands: no `dplace`, `cpuset`, `miser` or placement program or library is in `root/`. Their argument conventions are unknown here.

## How IP35 node topology reaches the scheduler

The numbers come from boot. [klconfig.md](klconfig.md) establishes that `numnodes` and `maxnodes` come from the topology walk, `maxcpus` from `cpu_node_probe`, and `boot_cnodemask` as the low `numnodes` bits. `cpuset_init` sizes every cpuset structure from `maxcpus`, `maxnodes` and `numcpus` and seeds cpuset 0 and the allowed and scheduling masks from `boot_cnodemask` (above). Established.

CPU to node: `cpumask_to_nodemask` (0xc000000000246d04) takes a CPU mask and a result of two doublewords. It walks the CPUs below `maxcpus` through a table of 16-byte entries (a word at +0 that is -1 for an absent CPU, a pointer at +8), tests each CPU's bit in the input, loads the halfword at +0x42a of the record the pointer names, and sets that bit number in the result (the word index is the value shifted right by 6, `sra` at 0xc000000000246d60). `generate_schedmasks` (0xc000000000246eb8, 368 bytes) loads the same halfword for the CPUs of each cpuset, builds the per-set scheduling masks and rebuilds `global_schedmask`. Established for the loads, the shift and the bit set; that the halfword is a compact node number follows from its use as a bit in a node mask and is inferred. IP27's copy was not read.

CPU numbering for user space: `build_cpumap` (0xc00000000004ea3c, 264 bytes), reached by `sysmp` 44, fills a halfword table of `numcpus` entries, initially -1, indexed by node and slice through `cnode_slice_to_cpuid` (0xc00000000004e980, 188 bytes); the case code refuses a buffer smaller than `2 * numcpus` bytes with 22. `sysmp` 43 `copy_distmatrix` (0xc0000000003238fc, 116 bytes) calls `bcopy` with a length of `numnodes`, from the table that the pointer at gp-0x1590 (0xc0000000016e1600) names; the case code refuses a buffer smaller than `numnodes` squared with 22. `sysmp` 42 stores `numnodes` through the result pointer. A caller can therefore read the node count, the CPU map and the node distance matrix without any other file. Established for the code; one byte per distance entry is inferred from the copy length.

What fills the distance table is not read. `node_distance` (0xc000000000323c30, 28 bytes) is the only accessor seen. How the router-hop counts from the NUMAlink topology ([ip35-router.md](ip35-router.md)) become distance values is unknown.

Memory fallback: `memaff_getbestnode` (0xc00000000032b2e0, 452 bytes) and `memaff_nextnode` (0xc00000000032b4a4) choose the closest node with room; `get_effective_nodemask` restricts them to the cpuset. Inferred from the names and the one caller; the distance usage was not read.

## Differences between IP27 and IP35

`sysmp`: 54 entries on IP27, 55 on IP35 (selector 54 added, no call). Same callees for the cpuset and miser selectors.

Node masks: one doubleword (64 nodes) on IP27, two (128 nodes) on IP35, in `cpuset_init`, and in `pmoctl` commands 60 to 61 and 100 to 102 ([syssgi.md](syssgi.md)).

Record sizes: the thread record is 768 bytes on IP35 and 752 on IP27 ([syscalls.md](syscalls.md)); the cpuset code reads thread offsets on IP35 (+0xf4 for the cpuset index) that were not rechecked on IP27.

`numa_tune` on IP35 never touches the quiesce and reactivate routines IP27 calls ([syssgi.md](syssgi.md)).

Job-limit stubs: identical on both.

## Not established

What the cpuset operations 22 and 23 (`cpuset_load_volatile` and `cpuset_get_volatile`) do beyond their names, and the field layout of the 0x470-byte request.

Whether the system the shipped kernels were linked on had the optional subsystem installed (its software-version database is not held).

What fills the node distance table, and what the 0x42a halfword of the per-processor record holds on each platform.

Which public name belongs to which `sysmp` selector, and the argument conventions of the user commands, which are not held.

Whether any per-cpuset limit exists for process count or open files: none was found, and `cpuset_load_volatile` and `cpuset_get_volatile` were not read.

These are recorded in [open-questions.md](open-questions.md).
