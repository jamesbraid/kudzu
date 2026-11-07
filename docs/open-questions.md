# Open questions

Questions the binaries have not yet answered. Each entry names the routes already tried and the next route. When a question is answered, move the answer into the owning document and delete the entry; when every route is exhausted, label the fact "unknown: no shipped software or published source depends on it" there and delete the entry.

## Triage (2026-11-01)

Six entries: 2 software-now, 3 missing-input, 1 hardware-only.

Route classes:

- **software-now**: the next route reads or runs something already held, in a 6.5.30 or 6.5.22 binary or a held file.
- **missing-input**: the next route needs a release, binary or configuration file that is not held. The entry names it.
- **hardware-only**: no shipped software or published source can answer it, so it is recorded and not scheduled. One entry is in this class (the SCSI CD-ROM section).

Premises corrected in place, with the check that corrected them:

- The 6.5.22 IP27, IP30 and IP35 linked kernels are held and in the inventory; [version-diffs.md](version-diffs.md) compares them with 6.5.30.
- Panic messages are resolved for 361 of the 890 call sites, not none (row 3).

Software-now, in order:

1. The panic messages: extend `scripts/panic_sites.py` to the 529 sites marked not resolved.
2. The `sysmp` and `syssgi` selector tables: run the extractor on the other selector functions. These are out of the current scope rules, so they come last.

Missing input:

- The IP32 linked kernel (row 1): no IP32 configuration is held for either release.
- The user-space `ioconfig` program and an `/etc/ioconfig.conf` file: neither is held for either release; the kernel side is in [ioconfig.md](ioconfig.md). Next route: extract `ioconfig` from the installation media named in [IRIX_FILES.md](../IRIX_FILES.md), then read how it assigns numbers and which control request sets the controller number from user space.
- The Octane2, Origin 350, Tezro and Fuel base-I/O PROMs, the IP22 PROM and an installed 6.5.22 `mediad` with its device module: none held ([scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md)). The CD-ROM device module (`dev_cdrom.so`) is not in `root/` for 6.5.30 either, so the routines at `0x01cf2c88`, `0x01cf31d8` and `0x01cf32f8` cannot be read to the end until it is extracted (checked 2026-11-01: no `dev_cdrom.so` or `mediad` file under `root/`).

Hardware-only:

- What opcodes `0xc4` and `0xc9` do to a Toshiba or Sony drive. Routes tried: both PROM families, both kernels, the media daemon and `scsicontrol` send them but nothing reveals their effect. Next route: a vendor drive manual ([scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md)).

## Scaffold

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| The IP32 linked kernel (`unix.IP32`): none held, and no IP32 configuration is held for either release. | missing-input | [IRIX_FILES.md](../IRIX_FILES.md) | The IP32 object archives are present and inventoried (230 entries in the 6.5.30 IP32 boot directory). `root/6.5.30` has no `var/`. `root/6.5.22/var/` has `sysgen.IP27`, `sysgen.IP30` and `sysgen.IP35` only. | Extract the IP32 configuration (`master.d`, `system`, `mtune`) from the installation media named in IRIX_FILES.md if they carry it, then link. Otherwise locate an installed O2 system disk. IP32 has no kernel row in `scripts/scope.json`. |
| `sysmp` and other selector tables are not generated; `syssgi` selectors that never narrow to a single value (shared error paths and ranges) are not listed. | software-now | `docs/inventory/selectors.tsv` | `scripts/syssgi_selectors.py` on `syssgi`; compared with public lists in [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md). Checked 2026-11-01: `sysmp` and `syssgi` are named-only, out-of-scope rows in the 6.5.22 `os.a` objects of all five platforms. | Run the extractor on the other selector functions, starting with `sysmp` in the 6.5.30 and 6.5.22 `os.a` objects. Explain the selectors that differ from the public lists. |
| Panic and assert message strings: 529 of the 890 call sites in [panics.md](panics.md) are `not resolved`. | software-now | `scripts/os_tables.py`, `scripts/panic_sites.py` | `scripts/panic_sites.py` resolves the format register at each call to a string in the file-backed data, which gives the 361 resolved sites. The 529 unresolved sites are the ones the scan cannot follow: two paths join before the call, or the pointer is loaded at run time (the Grades paragraph of panics.md). Checked 2026-11-01 by counting the table rows. | Extend the scan to follow the two-path joins and the loaded pointers, then recount. Add the 6.5.22 kernels to the table once row 1 lands. |
| The rule that links `jlimits.a` or its stubs into a kernel ([resource-control.md](resource-control.md)): the IP35 and IP27 system files list both `USE: jlimits` and `USE: jlimitsstubs`, yet the recorded link line names only the stubs and the kernels read carry only the stub bodies (6.5.22 IP27 checked by symbol sizes alone). | software-now | [resource-control.md](resource-control.md) | Compared the `master.d` flags of both modules, the `system` file, the `master.c` link line and the symbol tables of `6.5.30/unix.IP35`, `6.5.30/unix.IP27`, `6.5.22/unix.IP35` and `6.5.22/unix.IP27`. | Find where the configuration program decides a module's archive is absent or skipped (its own binary, if one is held, or a `var/sysgen` of an installed system that has the job-limit subsystem). |
| What the callers of the job-limit hooks do with the constant 1700 that `proc_job_accum`, `job_limit_check` and the other stubs return, and whether anything but `proc_joinjob` and `proc_as_joinjob` can store a job pointer at +0x7a0 of the process record. | software-now | [resource-control.md](resource-control.md) | Read the guard in `fdt_alloc` and the call sites of the hooks by resource index. | Scan the whole kernel for stores to +0x7a0 of the process record, and read the enforcement branch of `pas_break` to its error exit. |
| The 0x470-byte request layout of the cpuset call (`sysmp` selector 35): which field holds the name, the processor list, the memory limits, the permissions and the flags, and the meaning of each bit of the properties word. | software-now | [resource-control.md](resource-control.md) | Offsets read by `cpuset_create`, the 24 operation routines identified by callee, `cpuset_init`. | Read `cpuset_create`, `cpuset_get_properties` and `cpuset_get_flags` to the end with a field table, and the two unnamed operations (22 and 23). |
| Which public operation each `sysmp` selector 39 and 40 sub-command (0x45 to 0x49) is, and what the shared 0x46 and 0x47 query returns. | software-now | [resource-control.md](resource-control.md) | Branch targets of both selectors, the miser routines they reach. | Read `miser_get_quantum`, `miser_get_bids`, `miser_get_job`, `miser_set_resources` and the user-space miser program when one is held. |
| Whether a cpuset enforces any processor or process-count limit: `cpuset_get_cpulimits` returns two words that no routine read uses. | software-now | [resource-control.md](resource-control.md) | Readers of the cpuset memory and CPU limit fields (`cpuset_memalloc_ok` uses the memory side). | Find every reader of cpuset offsets +0xe4 and +0xe8 in the 6.5.30 IP35 kernel. |
| What fills the node distance table that `sysmp` selector 43 copies out, and what the halfword at +0x42a of the per-processor record holds on IP27. | software-now | [resource-control.md](resource-control.md) | `copy_distmatrix`, `node_distance`, `cpumask_to_nodemask`, `generate_schedmasks`. | Read `node_distance` callers and the writers of the table; compare the IP27 `cpumask_to_nodemask`. |
| The user-space half of cpusets, miser, job limits and data placement: the commands and libraries, their argument conventions and the published manual pages. | missing-input | [resource-control.md](resource-control.md) | Searched `root/6.5.30` for cpuset, miser, placement and job-limit programs and libraries: none held, and `usr/include` holds only `sys/invent.h`. | Extract the commands and libraries from the installation media named in IRIX_FILES.md, then read how they form the `sysmp` and `syssgi` arguments. |

## Hardware graph

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| How the kernels assign and persist device unit numbers through the ioconfig file, and which control request sets a controller number from user space. | missing-input | [ioconfig.md](ioconfig.md) | The kernel side is read: where the number lives (an inventory record word), the getter and setter, the Ethernet and audio readers and both messages, and the shipped configuration fragments. The setter calls found by a whole-kernel scan are driver attach and ioctl routines, not a user-space entry; only three were read. The user-space program is not held. | Extract `ioconfig` from the installation media, read how it assigns numbers and saves them, and read the remaining 20 setter call sites to find the one reachable from user space. |
| Which physical PCI pin (INTA to INTD) of each slot reaches which Bridge or PIC interrupt bit on IP35 (Origin 350, Tezro, Origin 300). The software side is settled in [pci-intr-pin.md](pci-intr-pin.md). | missing-input | [pci-intr-pin.md](pci-intr-pin.md) | IRIX software expectation read from the 6.5.30 and 6.5.22 IP35 kernels. Full text of the Tezro hardware guide (007-4643-002) and the Origin 300 user's guide (007-4365-002): no pin routing. 007-0911-210 driver guide: pin lines as a set, no wiring. Web search for hinv or ioconfig captures: none found. The Library search backends were refused connections, so no Library-wide negative is claimed. | Read the Origin 350 user's guide 007-4566-001 for interrupt or slot tables; search for an IP35 hinv -v or ioconfig capture with a dual-function PCI card; check the IDSEL-to-slot table when the Library is back. |

## Checkpoint and restart

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| The user-space half of checkpoint and restart: the `cpr` command, its library, the viewer, the on-disk checkpoint format, the restart sequence and the privilege rules. | missing-input | [checkpoint-restart.md](checkpoint-restart.md) | The kernel never writes a checkpoint file (no routine in `cpr.a` calls a file write routine). `root/6.5.30` and `root/6.5.22` hold no file whose name contains `cpr`, `ckpt`, `checkpoint` or `cview`, and only `sbin`, a few `usr/etc` and `usr/sbin` programs and `usr/cluster/bin` of user space. | Extract the checkpoint command, its library and the viewer from the installation media named in [IRIX_FILES.md](../IRIX_FILES.md) (the base `lib32`, `lib64`, `usr/bin` and `usr/sbin` trees), then read which `ckpt_sys` sub-commands and `/proc` requests they send, in what order, and how they lay out the saved image. |
| The published manual for the facility (an operation guide and the command and library pages) was not retrieved. | missing-input | [checkpoint-restart.md](checkpoint-restart.md) | The Library search backends refused connections on 2026-11-06, so no Library-wide negative is claimed and no manual is cited. | Look the guide up by title in the published-manual collection when the service is back, then cite it by document number and page. |
| Which kinds of process state a restart re-creates beyond what the kernel requests expose: sockets, message queues, semaphore sets, array sessions. | missing-input | [checkpoint-restart.md](checkpoint-restart.md) | `cpr.a` calls no socket, message-queue, semaphore-set or array-session routine; sockets have only `ckpt_sockmate` and `ckpt_fpaddr`, which return identities. | Read the user-space tool (row above) for what it refuses or records for each. |
| What opens a `/proc` file so that bit 0x400 is set and `prioctl` reaches `ckpt_prioctl`, and what the per-request flag words from `ckpt_prioctl_attr` (0x421, 0x423, 0x441, 0x443, 0x4c5, 0x601, 0x603) tell `prioctl`. | software-now | [checkpoint-restart.md](checkpoint-restart.md) | `prioctl` and its per-thread sibling read in the IP35 kernel up to the calls; the open path and the use of the attribute word were not followed. | Read the `/proc` open routine and the code after the call at 0xc000000000435dc4 in `prioctl`. |
| The meaning of the vnode type values 0, 2, 5, 7 and 8 that `ckpt_lookup_add` skips, of bit 0x400000 set by `ckpt_sys` sub-command 18, and the effect of the `ckpt_enabled` tests in the region-attribute routines (`pas_addmmap`, `pas_addexec`, `pas_addload`). | software-now | [checkpoint-restart.md](checkpoint-restart.md) | The tests and constants were located; the effects were not followed. | Read the vnode type table by its use in `vn_open` and the filesystems, the readers of the bit at +0x290 of the process record, and the three `pas_` routines. |
