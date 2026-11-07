# Process model and isolation

How the IRIX 6.5.30 kernel creates processes, groups them, stores a process's root directory and decides whether a caller may do a privileged thing. The evidence is the linked IP35 kernel (`6.5.30/unix.IP35`, the Origin 300/350 and Tezro kernel); where the IP27 kernel (the Origin 200/2000 build of the same release) differs, the difference is stated. 6.5.22 has no linked kernel in the inventory for this page and nothing here is claimed for it. Reference pages are named as hints (`sproc(2)`, `prctl(2)`, `blockproc(2)`, `chroot(2)`, `capability(4)`): none of the facts below rests on them, and no document number was looked up.

Grades follow [AGENTS.md](../AGENTS.md): **established** is read in the binary at the address given, **inferred** says what it rests on, **unknown** says what would settle it. Kernel names are those of the shipped symbol table or debug records. The system call handlers are described one by one in [syscalls.md](syscalls.md); this page ties them together and does not repeat their bodies.

## Records this page uses

Field names and offsets are from the debug records of both kernels (dwarfdump, member names and offsets only). Established.

| Record | Field | IP35 | IP27 |
|---|---|---|---|
| `uthread_s` (a kernel thread of a process; 1592 bytes on IP35, 1584 on IP27) | `ut_proc` | +0x300 | +0x2f0 |
| | `ut_vproc` | +0x308 | +0x2f8 |
| | `ut_pproxy` | +0x328 | +0x318 |
| | `ut_flags`, `ut_update` | +0x340, +0x344 | +0x330, +0x334 |
| | `ut_job` | +0x358 | not read |
| | `ut_cdir`, `ut_rdir` (current and root directory) | +0x470, +0x478 | +0x468, +0x470 |
| `proc` (3072 bytes on both) | `p_proxy` (a `proc_proxy_s` at +0) | +0 | +0 |
| | `p_pgid`, `p_pid`, `p_ppid`, `p_sid` | +0x298, +0x29c, +0x2a0, +0x2a4 | same |
| | `p_shaddr` (share group record) | +0x990 | same |
| | `p_who` (lock taken by `setsid`) | +0x998 | same |
| | `p_cred` | +0xa80 | same |
| `proc_proxy_s` (656 bytes) | `prxy_utshare` (a `pshare_s`) | +0x8 | same |
| | `prxy_flags`, `prxy_shmask` (share mask of the process) | +0x94, +0xa0 | same |
| `pshare_s` (96 bytes; state the threads of one process share) | `ps_cdir`, `ps_rdir`, `ps_cmask`, `ps_cred`, `ps_sighold` | +0x10, +0x18, +0x24, +0x28, +0x30 | same |
| `shaddr_s` (160 bytes; the share group record) | `s_fdt`, `s_cdir`, `s_rdir`, `s_cmask`, `s_limit`, `s_cred` | +0x40, +0x48, +0x50, +0x5a, +0x60, +0x68 | same |
| `cred` (72 bytes) | `cr_uid`, `cr_gid`, `cr_ruid`, `cr_rgid`, `cr_suid`, `cr_sgid`, `cr_mac`, `cr_cap`, `cr_groups` | +0x8, +0xc, +0x10, +0x14, +0x18, +0x1c, +0x20, +0x28, +0x44 | same |
| `cap_set` (24 bytes, at `cr_cap`) | effective, permitted, inheritable (64-bit masks) | +0, +8, +0x10 | same |

The first operation table of the process object (`vproc` operations, the table `pproc_set_dir` sits in, at 0xc000000001597e28 on IP35) names its slots by function: +40 `pproc_set_dir`, +168 `pproc_setpgid`, +208 `pproc_setsid`, +216 `pproc_setpgrp`, +184 `pproc_setuid`, +504 `pproc_procblk`, +512 `pproc_prctl`, +600 `pproc_getjob`, +608 `pproc_accumjob`, +616 `pproc_isinjob`. Established (the pointers in the table).

## Process creation: one body for fork, sproc, pid-chosen forks

Established, IP35 addresses; the IP27 kernel has the same routines with the same sizes at other addresses (`sproc` 0xc00000000023aabc, `nsproc` 0xc00000000023a350, `nsproctid` 0xc00000000023a30c, `pidsprocsp` 0xc00000000023aa78, `pidfork` 0xc00000000023a294).

- `fork` (`6.5.30/unix.IP35`, 0xc0000000002e9fac, 40 bytes) and `pidfork` (`6.5.30/unix.IP35`, 0xc0000000002e9f5c, 80 bytes) call `dofork` (0xc0000000002eaab8, 176 bytes) with a share mask of 0. `pidfork` first requires its process-id argument to be above 0 and below 0x7ffffff0 and otherwise returns 3.
- `sproc_common` (`6.5.30/unix.IP35`, 0xc0000000002ea834, 340 bytes) is the worker behind `sproc` (call 1129), `sprocsp` (1132) and `pidsprocsp` (1210, 68 bytes at 0xc0000000002ea74c). The debug records give it a name and a size of 340 bytes; [syscalls.md](syscalls.md) lists it as unlabelled and 4936 bytes long, which spans the routines that follow it.

`sproc_common` returns 1 when the caller's own share mask has bit 0x80, makes the caller a share-group member first if it is not one (it calls `allocshaddr`, 0xc00000000029e6bc, and returns 11 when that fails), narrows the requested mask to `(caller's share mask | 0xff000000) & requested`, forces bit 0 on, and calls `dofork`. A child therefore cannot share more than its parent shares.

`dofork` calls `procfork` (`6.5.30/unix.IP35`, 0xc0000000002eab68, 1216 bytes), which calls `procnew` (0xc0000000002ebb7c, 536 bytes: the new process record and its process id), `uthread_dup` (0xc0000000002eb028, 344 bytes: the child's thread) and `procdup` (`6.5.30/unix.IP35`, 0xc0000000002eb3a4, 2008 bytes: the copy of the parent's state).

`procdup` copies the parent's `p_pgid` and `p_sid` into the child, copies the thread's `ut_cdir` and `ut_rdir` (taking a reference on each with `vn_hold`), attaches the child to the share group with `attachshaddr` when bit 0 of the mask is set, joins the parent's process aggregate with `vpag_join`, calls `fdt_fork` (copy of the descriptor table) on one path and `fdt_forkfree` on another, and calls `pid_associate`. `procfork` stores the caller's `p_pid` as the child's `p_ppid`. Established for each call and copy named; the condition that selects `fdt_fork` and the meaning of every mask bit other than 0x1 were not traced (see the open questions).

The mask `sproc` and `sprocsp` pass is the caller's mask ANDed with 0x30007f. Bits 0x7f are the `sproc` sharing bits of the reference page (hint). Bits 0x100000 and 0x200000 are kept by the AND and their use was not found. Unknown.

## Job IDs

There are no working job IDs in the shipped kernel. Established.

The job-limits package is not linked in the shipped kernel; its stub set is. The real code, `jlimits.a`, is in the optional subsystem `eoe.sw.jlimits` of the base product on the installation media, and the 6.5.30 IP35 `irix.sm` names it on `USE: jlimits` (line 548) and its stub module on `USE: jlimitsstubs` (line 580), so a system without that subsystem gets the stubs (see Privilege model below for the rule). `jid_jobtab` and `jlimits_getmachid` return 0, `jlimits_init` stores 0 into the word that the creation path tests (gp+0x2d34), `proc_joinjob`, `proc_leavejob`, `proc_as_joinjob`, `job_as_exit` and `proc_job_accumcpufin` return at once, and `job_limit_check` and `proc_job_accum` return the constant 1700.

All eleven job entry points that `syssgi` calls (`syssgi_getjusage`, `syssgi_setjusage`, `syssgi_getjlimit`, `syssgi_setjlimit`, `syssgi_getjid`, `syssgi_killjob`, `syssgi_makenewjob`, `syssgi_joinjob`, `syssgi_getjobpid`, `syssgi_setwaitjobpid`, `syssgi_waitjob`; 28 bytes each, consecutive from 0xc0000000004967d0) only call `nopkg` (0xc000000000256040), which returns 65 (0x41). Their `syssgi` selectors are in the range 1210 to 1220 on IP35, a different number space from the system call numbers used elsewhere on this page (1210 `syssgi_getjlimit`, 1211 `syssgi_setjlimit`, 1216 `syssgi_makenewjob`, 1218 `syssgi_joinjob`, 1220 `syssgi_waitjob`), so a call to any of them fails with 65. Established.

`procdup` (called on the creation path) still contains the calls (`proc_joinjob`, `proc_leavejob`, `proc_as_joinjob`, and `proc_job_accum` and `set_jl_enforce_flag` six times each) a job-limits kernel would use to make a child join its parent's job; `sproc_common` makes only six calls (`dofork`, `new_tlbpid`, `mutex_bitlock`, `blockset`, `allocshaddr` and the routine at 0xc0000000002ea988), none of them a job call. Because `procdup` reaches stubs, nothing is inherited. Inferred from the call sites; the archive `jlimits.a` that would replace the stubs is held in the IP35 boot directory but was not read.

The `ut_job` field and `job_create` / `job_destroy` (`6.5.30/unix.IP35`, 0xc000000000251094, 164 bytes, and 0xc0000000002511cc, 76 bytes) belong to the scheduler: `job_create` is called from `batch_create`, `uthread_sched_setscheduler`, `uthread_attachrunq` and `leaveshaddrRunq`. They group threads for scheduling and are not user-visible job IDs. Inferred from the callers.

The identifier that survives in this kernel is the array-session handle, held in a process aggregate (`vpagg_s`: id, reference count). `fork` and `sproc` children join the parent's aggregate through `vpag_join` in `procdup`; `pproc_exit` leaves it (`vpag_leave`). Selectors 225 and 226 (`arsess_ctl`, 1388 bytes at 0xc0000000002e64e8, and `arsess_op`, 1664 bytes at 0xc0000000002e70c4) manage it: `arsess_ctl` dispatches on a command below 16 and `arsess_op` on one below 10, and most commands call `cap_able` first. What each command number does was not read. Established for the dispatch bounds and the join and leave calls; unknown for the commands.

## Sessions and process groups

- `setsid` (`6.5.30/unix.IP35`, 0xc0000000002b58e4, 84 bytes) calls the process operation at slot +208, `pproc_setsid` (0xc0000000002d72c8, 292 bytes). Under the write lock `p_who` it returns 1 when `p_pgid` equals `p_pid` (already a group leader) or when a process group with the caller's process id already exists (`vpgrp_lookup`); otherwise it creates the new group and session and stores the new process-group id (the same number as the session id) through the return pointer. Established.

Sessions and groups are objects looked up by number: `vsession_create` (0xc0000000002ee330, 236 bytes) and `vpgrp_create` (0xc00000000030ba94, 284 bytes) take a lock, look the number up in a table (`vsessiontab`, `vpgrptab`), and either return a counted reference or allocate. A `vpgrp` (64 bytes) holds its group id `vpg_pgid`, session id `vpg_sid` and a reference count. Established.

A child inherits its parent's group and session by value (`procdup` copies `p_pgid` and `p_sid`). Share-group membership does not change them. Established for the copy.

`setpgid` (0xc0000000002b5938, 700 bytes) goes through the slot at +168; it looks the target up with `vproc_lookup`, checks the credentials with `pcred_access`, and calls `mac_access` (a stub in this kernel). The rules about changing a process in another session were not read. Unknown.

## What sproc shares

`sproc` makes the caller a share-group master on first use (`allocshaddr`) and creates children that join it (`attachshaddr`). The `shaddr_s` record holds the shared items: the descriptor table (`s_fdt`), current and root directory (`s_cdir`, `s_rdir`), file creation mask (`s_cmask`), file size limit (`s_limit`) and credentials (`s_cred`), plus the address-space bookkeeping (not read). Established for the record layout. The pairing of mask bits with those items follows the field names and the reference page (hint) and is inferred, except two bits that are read in code:

Mask bit 0x4: `pproc_set_dir` (below) tests `prxy_shmask & 4`, and then bit 8 of the proxy flags word at +0x94, to decide whether a directory change goes to the share group. Established.

Mask bit 0x1: `procdup` calls `attachshaddr` only when the mask has it. Established.

A process that is not a share-group member still has a `pshare_s` shared by its own threads (below): current directory, root directory, umask, credentials and signal mask. Established for the record; its creation is `allocpshare` (0xc00000000027d1e0, 384 bytes), which copies the creating thread's `ut_cdir` and `ut_rdir` into `ps_cdir` and `ps_rdir` (with `vn_hold` on each) and stores the record at `prxy_utshare`.

## nsproc

`nsproc` is not a namespace call. It creates another kernel thread inside the calling process and allocates no process id. Established by the call list; the names are from the symbol table.

- `nsproc` (`6.5.30/unix.IP35`, 0xc0000000002ea018, 28 bytes, call 1204) calls `nsproc_common` (0xc0000000002ea034, 1680 bytes) with a share-mask argument of 0xffff. `nsproctid` (0xc0000000002e9fd4, 68 bytes, call 1229) calls the same routine with its mask taken from the argument block at +0x24, and returns the new thread's identifier (the halfword `ut_id` at thread +0x348).

`nsproc_common` calls `uthread_dup` (the same routine `procfork` uses to make a child's thread), `kthread_afflink_new`, `uthread_setsigmask`, `uthread_attachrunq`, `putrunq` and `ktimer_init`; it calls neither `procnew` nor `pid_op`, so no process record and no process id are made. The new thread is placed on a run queue. A failure path calls `uthread_exit`. Established.

The second argument (+8) must have bit 0x80 or the call returns 22. A pointer argument (+0x10) is copied in as a 0x28-byte attribute block (five fields; a 32-bit caller's 0x14-byte block is widened by `copyin_xlate`). An optional pointer (+0x18) supplies 8 bytes of scheduling parameters and needs the capability `1 << 33`, else the call returns 1; the parameters go to `uthread_sched_setscheduler`. Established for the paths; the meaning of each attribute field was not read.

`sproc_common` returns 1 when the caller's share mask has 0x80 and `pproc_procblk` returns 1 when the target's has it, the same bit `nsproc` insists on in its request. This reads as: a process that uses native threads is not a share-group process. Inferred; whether `nsproc_common` sets the bit in the process's mask was not traced.

The new thread inherits the process's directories, credentials and signal mask because they live in the process's `pshare_s`, not in the thread. Inferred from the layout above.

## procblk

`procblk` (`6.5.30/unix.IP35`, 0xc0000000002fa818, 316 bytes, call 1131; syscalls.md has the entry) takes an operation 0 to 5, a process id and a count (operations 2 and 5 require the count to be from -10000 to 10000, compared against 0x2710 at 0xc0000000002fa8e8 and 0xc0000000002fa8f4, else 22), and calls `pproc_procblk` (0xc0000000002fb408, 400 bytes) through the process operation at +504. `doprocblk` (0xc0000000002fb2e8, 288 bytes) does the work; `blockset` (0xc0000000002fac34, 364 bytes) adds to or subtracts from a signed block count in the target thread's record and puts the thread to sleep or wakes it; it keeps the counts as halfwords at thread +0x5c0 and +0x5c2. Operations 2 and 5 take a count. Operations 3 to 5 walk every member of the share group under the group's list lock (`shaddr` +0x10), the others act on one process. These are the `blockproc`, `unblockproc`, `setblockproccnt` family of the reference page, in that order for 0 to 2 and as the "all members" forms for 3 to 5; the names are a hint and the walk is established. `pproc_procblk` returns 1 when the target's share mask has 0x80 (established). In its path for another process it tests `mac_enabled`, calls `mac_access` when that is set and asks `cap_able_cred` for `1 << 6` (inferred that this path is the "another process" one: the branch that selects it was read from the arguments only). IP27 is the same with `procblk` at 0xc00000000024ac90.

## The session-manager interface

There is no session manager in the shipped kernel. Established. The kernel half of the session manager is also on none of the installation media held for 6.5.22 and 6.5.30: no `sesmgr` object appears in any boot-directory listing, only `sesmgrstubs.a`, although the 6.5.30 IP35 `irix.sm` names the module (line 530) and the user daemon `usr/sbin/sesmgr` ships in the base subsystem. So the feature is not linked in the shipped kernel and its implementation is not held at all.

- `sgi_sesmgr` (`6.5.30/unix.IP35`, 0xc000000000496bc0, 8 bytes, call 1209) returns 65 (0x41, the value `nopkg` returns) and does nothing else.

The other 42 `sesmgr_*` entry points (`sesmgr_get_label`, `sesmgr_set_label`, `sesmgr_istsix`, `sesmgr_soreceive`, ...) are 8-byte returns; `sesmgr_enabled` (0xc00000000163b984) is 0 in the file image. No object named `sesmgr` is in the IP35 boot directory, only `sesmgrstubs.a`; the module exists in the sysgen tables ([sysgen.md](sysgen.md)) but its implementation is not held. Established for the stubs; the missing implementation is an open question.

`syssgi` selectors 81 and 82 (public names `GETPSOACL` and `SETPSOACL`, a hint) end in a load of 65 as their result at 0xc0000000002e1440 (the delay slot of the branch at 0xc0000000002e143c); the case at 0xc0000000002e1434 reaches it through a selector-equality compare that falls into the same load. Which of the two selectors reaches it by which path was not decoded. No `syssgi` case calls a `sesmgr_*` routine (a scan of the 16704-byte `syssgi` at 0xc0000000002e0310). Selectors 77 and 78 (`LOADATTR`, `UNLOADATTR`) have no case. Established.

What the public list calls the array session selectors (`SETASH`, `NEWARRAYSESS` and the like) are implemented through selectors 225 and 226 above. Inferred from the names the two routines call (`arsess_new`, `arsess_join`, `arsess_setash`, `arsess_enumashs`) and `vpag_*`.

## chroot: where the root is kept

The root directory is a per-thread pointer with a per-process and a per-share-group master copy. Established unless marked.

`chroot` (see [syscalls.md](syscalls.md)) checks `cap_able` with `1 << 40`, resolves the path to a directory vnode, checks search access (`access` operation with 64), optionally refuses (below), and calls the process operation at +40, `pproc_set_dir` (`6.5.30/unix.IP35`, 0xc0000000002d4b68, 440 bytes), passing the vnode, the address of the calling thread's `ut_rdir` and a flag of 1. `chdir` and `fchdir` pass 2, which selects the current directory instead.

`pproc_set_dir` has two cases. If `prxy_shmask & 4` is set and bit 8 of the proxy's flags word at +0x94 is set (the code at 0xc0000000002d4b74 to 0xc0000000002d4bb4 tests both; if either is clear it takes the other case), it takes the share group's lock (`shaddr` +0x28), brings the calling thread's copy up to date with `syncdir`, stores the new vnode into `s_rdir` (or `s_cdir`), and calls `setshdsync(shaddr, proc, 4, 1)` to mark the other members for a resync; the other members are expected to pick the new value up on their next kernel entry (inferred from the name `setshdsync` and the thread's `ut_update` bit tested first). Otherwise it takes the lock of the process's `pshare_s`, stores into `ps_rdir` (or `ps_cdir`) and calls `setpsync`. Either way it then stores the vnode in the calling thread's own `ut_rdir` and releases the old one. So the root is shared by all members of a share group whose members share directories, shared by all threads of one process, and otherwise private to the thread.

A new process or thread starts with its creator's root: `procdup` copies `ut_cdir` and `ut_rdir`; `allocshaddr` and `allocpshare` copy `ut_rdir` into the group and process records (both reach displacement 0x478 of the thread record). A chroot is inherited by children. Inheritance across `exec` is inferred: the scan of every load and store with displacement 0x478 (below) finds no routine of the `exec` path.

The only routines of the IP35 text that load `ut_rdir` through the thread pointer to decide on a path are `lookuppn` (0xc0000000002c1698, 2260 bytes, two loads), `chroot`, `pproc_set_dir` (through the pointer it is handed), `allocpshare`, `allocshaddr`, `procdup` and `utssys`. A scan of every `ld`/`sd` with displacement 0x478 lists 28 routines; the others (network drivers, NFS server routines and similar) use other records at that offset. Inferred: nothing else in the kernel is root-aware.

`lookuppn` starts an absolute path at `ut_rdir` when it is non-null, and on a `..` component stops climbing only when the vnode it is in is the same as `ut_rdir` (compared with `vn_cmp`) or the same as the global `rootdir` (0xc000000001645e20, reached as gp+0x26b0). Established; that a null `ut_rdir` starts at `rootdir` is inferred.

## chroot: open directories, /proc, /hw, and ways out

`chroot` consults the global `allow_chroot_with_open_dirs` (0xc00000000163c6e8 on IP35; 0xc0000000014cafa0 on IP27; both initialised to 1 in the file image). With 0 it scans the caller's descriptor table every time, with 1 only if the thread already has a root (`ut_rdir` non-null), with any other value never. The scan refuses (returns 1) if any open descriptor is a file with a vnode (flag bit 0x200 clear) whose type is 2, the directory type. So with the shipped value the first chroot of a process is **not** checked, and only a later chroot inside an existing chroot is. Established.

The scan looks at descriptors only. The call neither reads nor changes the caller's current directory `ut_cdir`. A process whose current directory lies outside the new root keeps it, and since `..` stops only at an exact match with the root vnode, a walk upward from a directory outside the root never meets it. Inferred from the two code readings above; no run was made.

Together these give the usual escape paths, all visible in the code: an open directory descriptor that survives the first chroot and is used with `fchdir` (which only checks that the descriptor is a directory and that the caller has search access); a current directory left outside the root; any process that can still reach a path outside the root through an open file or a unix-domain socket (descriptor passing is a socket-layer feature not read here). Inferred.

`chroot` does not touch the credential: its calls are `cap_able`, `lookupname`, `vn_rele`, the descriptor-table walk (`fdt_fd_to_chunk`, `mrupdatef`, `mrunlock`), the process operation and audit hooks. The process keeps its user and group ids and its capability set. A process that holds the capabilities for `mknod`, `mount` or device access can make a device node, mount a file system or open a device and so leave. Which capability bits those are is unknown (below). The `1 << 40` check for `chroot` itself and `1 << 29` for loopback mounts are established.

The checkpoint and restart facility can also set another process's root: its `ckpt_chroot` request takes a path and, as the only handler there that tests a capability, asks for `1 << 40` ([checkpoint-restart.md](checkpoint-restart.md)). The same page records that `ckpt_sys`, the entry that reaches `pidfork` and `pidsprocsp`, makes no privilege test of its own.

`/proc` and `/hw` are ordinary file systems mounted at directories of the global tree (the `proc` module with `prvfsops` and `prvnodeops`, 152 and 456 bytes; `hwgfs`). The lookup crosses mount points through the vnodes, with no per-process mount table. They are visible inside a root only if that root has the mount points; they are not reached by name outside it. A `/proc` file system mounted inside the new root exposes every process by process id, because process numbers are global: `procfs` code has no `ut_rdir` reference. `hwgfs` keeps its own permission bits (`hwgfs_get_permissions`, `hwgfs_set_permissions`). Established for the absence of root checks in the scan above; inferred for the exposure.

Device files are opened by device number (`spec_open`, 0xc0000000003ae3e8, 632 bytes); the root does not enter into it. Inferred.

## Loopback and per-process mount views

A loopback file system exists: `lofs`. `lofs_init` (0xc0000000003c38a0, 68 bytes) registers it; `lofs_mount` (0xc0000000003c38e4, 704 bytes) requires the capability `1 << 29` (`cap_able_cred`), resolves the source path with `lookupname`, and wraps the directory vnode in an `lnode` (`makelonode`); that `lofs_root` returns it as the mount's root is inferred, the routine being 40 bytes that were not read. `lofs_vnodeops` (456 bytes) forwards the vnode operations to the real vnode, with `lofs_realvp` giving the underlying vnode and `lofs_lookup` (1292 bytes) handling name lookup. A limit message reads "lofs_rdev overflowed. No new lofs mount points." Established. The module is configured through `lofs.a` and `lofsstubs.a` ([sysgen.md](sysgen.md) has the row); it is linked in `unix.IP35` (the symbols above are real code).

`namefs` (`nm_mount` 1184 bytes, `nm_unmount`, `nm_open`, 688 bytes) is the file system that mounts a descriptor over a path (the mechanism behind `fattach`). Inferred from the names; its routines were not read.

There is no mount flag for a bind or move and no per-process mount table. No routine, string or structure field in the kernel's symbols or strings names a mount namespace; the process records above carry no mount-view pointer; `lookuppn` crosses mounts from the vnode. The only per-process root is `ut_rdir`. Inferred from the absence; the kernel's strings were searched for "namespace" and "bind" and only unrelated disk and network routines matched.

`mount` (call 1021) goes through `cmount` (0xc000000000289eac, 348 bytes), which has no capability check of its own; `lofs_mount` has one. Whether other file systems' mount routines check a capability or only the credential was not read. Unknown.

## Privilege model

Present in the kernel (`6.5.30/unix.IP35` and `6.5.30/unix.IP27`):

| Piece | State in the shipped kernel | Evidence |
|---|---|---|
| user and group ids, supplementary groups | real | `cred` fields above |
| capabilities | real | 14 real `cap_*` routines (the 14 function symbols with that prefix) |
| audit (`sat_*`) | all 93 routines are bare returns; `sat_enabled` is 0 | `sat_enabled` at 0xc000000001646494 |
| ACL (`acl_*`) | all 17 routines are bare returns or constants; `acl_enabled` is 0 | `acl_enabled` at 0xc000000001646498 |
| mandatory access control (`mac_*`) | all 77 routines are bare returns; `mac_enabled` is 0 | `mac_enabled` at 0xc000000001646490 |
| session manager (`sesmgr_*`) | all stubs | above |
| job limits | stubs | above |

Established. Where the real code lives: `jlimits.a` is in `eoe.sw.jlimits`, `sat.a` in `eoe.sw.audit` and `acl.a`, `acl_hwg.o` and `acl_xfs.o` in `eoe.sw.acl`, all optional subsystems of the base product on the Overlays 1 disc (idb entries of the 6.5.30 media). The 6.5.30 IP35 `irix.sm` lists `USE: sat` at line 495, `USE: acl` and related ACL modules at lines 503 to 506, `USE: jlimits` at line 548, and, in a stub block starting near line 562, the matching `satstubs`, `aclstubs` and `jlimitsstubs` modules (lines 567, 568 and 580). `master.d/README` says a stub module is generated at link time when the real module is excluded, and the stub modules carry the `u` flag (always loaded after all others). `lboot` carries the strings "no object file for %s" and "/* stubs */", which fits the rule that a `USE`d module whose object is missing from the boot directory is skipped and its stubs are emitted (strings only; `lboot` was not disassembled). Inferred: the system the kernels were linked on lacked those subsystems. The base-product errno 65 that these stubs return is named for a package that is not installed (`ENOPKG` in the base product's error header). Mandatory access control (`mac.a`) and the session manager have no kernel object on any held media; their `USE:` lines (513 to 526 and 530) and stub modules (567, 568) are present. The other implementations: `IP35boot` holds `acl.a`, `sat.a`, `jlimits.a`, `capability.a` and `eag.a` next to `aclstubs.a`, `satstubs.a`, `jlimitsstubs.a`, `macstubs.a` and `sesmgrstubs.a`. There is no `mac.a` and no `sesmgr.a`. `cap_enabled`, `mac_enabled`, `acl_enabled` and `sat_enabled` are consecutive words (0xc000000001646490 to 0xc00000000164649c) that the code addresses as gp+0x2d20 to gp+0x2d2c. This also settles the global switches [syscalls.md](syscalls.md) leaves open: the word at gp+11552 is `mac_enabled` (tested by `ptrace`, `readlink`, `symlink`, and `pproc_procblk`, which `procblk` reaches through the process operation) and the one at gp+11556 is `sat_enabled` (the audit hooks). Established from the symbol addresses and the offsets.

### The capability check

- `cap_able` (`6.5.30/unix.IP35`, 0xc000000000332e84, 48 bytes) calls `get_current_cred` and then `cap_able_cred` with the result and its own mask argument. Established.
- `cap_able_cred` (`6.5.30/unix.IP35`, 0xc000000000332db8, 204 bytes) takes a credential and a 64-bit mask. It returns 1 (allowed) when every bit of the mask is in the credential's effective set (the first doubleword of the `cap_set`, at credential +0x28). Otherwise it returns 1 when `cr_uid` is 0 and the word `cap_enabled` is not 2, and 0 (refused) in every other case. With auditing on it calls `sat_set_suflag` with 1 on entry, with 4 when the set sufficed and with 2 when the superuser rule applied. Established.

So with the default `cap_enabled` the superuser id needs no capabilities at all, and capabilities only widen what a non-zero id can do. Setting `cap_enabled` to 2 makes root subject to the sets. `cap_init` (0xc0000000003322c4, 12 bytes) sets it to 1; `cap_style` (0xc000000000332d40, 56 bytes) sets it to its argument when the caller passes `cap_able(1 << 28)`. No call to `cap_style` was found in the text (a scan of the direct calls) or in a function-pointer table; who calls it is unknown. Established for the routines.

`cap_able_any` (0xc000000000332d78, 64 bytes) returns 1 if the credential has any effective capability, or if `cap_enabled` is not 2 and the user id is 0. Established.

There are 274 direct call sites of `cap_able` and 202 of `cap_able_cred`. Which bit means what is not recorded in the kernel, which carries no capability names: the masks seen so far are `1 << 6` (`procblk` on another process), `1 << 28` (`cap_style`), `1 << 29` (loopback mount), `1 << 33` (setting scheduling parameters in `nsproc`), `1 << 37` (the device-management ioctls in [driver-ioctls.md](driver-ioctls.md) and the front-panel selector in [syssgi.md](syssgi.md)) and `1 << 40` (`chroot`). Established as masks; the meanings are unknown.

### How a credential gets its capabilities

`cap_empower_cred` (0xc0000000003322d0, 20 bytes) sets all three sets of a credential to the full mask held at gp+0x12b0. Its only caller is `p0init`, so the first process starts with every capability. Established.

On `exec`, `cap_recalc` (0xc000000000332464, 156 bytes; called from `gexec_complete`) recomputes the sets with `cap_exec_newcap` (0xc00000000033236c, 248 bytes) from the old sets and the file's capability record. The record is read by `cap_vtocap` (0xc0000000003322e4, 136 bytes) from the extended attribute `SGI_CAP_FILE` (inferred from the string and the file-system call it makes). The routine follows the draft POSIX rules: permitted is built from the inheritable intersection and the file's permitted set, masked by the full mask. Inferred from the bitwise structure; the flag word it tests (bit 0 of the file record) is not decoded.

Capabilities are changed by `cap_setpcap` (0xc000000000332500, 552 bytes, whole process sets, needs `cap_able`), `cap_request` (0xc000000000332728, 160 bytes, raises capabilities that are permitted) and `cap_surrender` (0xc0000000003327c8, 132 bytes, drops them); `proc_attr_set` (0xc000000000333318, 548 bytes) dispatches to them on an attribute-name string (the names `SGI_CAP_PROCESS`, `SGI_CAP_PROCESS_FLAGS`, `SGI_CAP_REQUEST`, `SGI_CAP_SURRENDER` and `SGI_MAC_PROCESS` are in the kernel's strings), and `proc_attr_get` (0xc000000000333204, 276 bytes) reads the process sets and label. Both are `syssgi` selectors 85 and 86. Selectors 83 and 84 (`cap_get`, 456 bytes at 0xc00000000033284c, and `cap_set`, 812 bytes at 0xc000000000332a14) read and write the capability record of a file. File capabilities are stored in the file system's extended attribute `SGI_CAP_FILE` (the XFS write path removes it when a file is written, with the message "error removing SGI_CAP_FILE EA"). Established for the routines and strings; the mapping of the strings to the branches in `proc_attr_set` is inferred.

### What chroot leaves unrestricted

None of the following has a root check in the routines named above; each is inferred from the absence of a `ut_rdir` use.

Credentials, capability sets and every `cap_able` decision: the same inside and outside a root.

Process visibility and control: process numbers, `kill` / `sigsendsys`, `ptrace` (which goes through `ptrsrch` and `mac_access`), `procblk` and `/proc` are global.

Process and thread creation with `fork`, `sproc` and `nsproc`, resource limits, scheduling.

File systems and devices outside the root that the process can already open or that it can reach through an open descriptor or a mounted `/proc`, plus device nodes (by number) and mount points.

The network stack, System V message queues, semaphores and shared memory, the host name and `syssgi` / `sysmp` services: no per-root separation was seen in their handlers (the handlers listed in [syscalls.md](syscalls.md) do not mention one).

## Namespace scope

Where each of these is dispatched and which link input defines it: [namespace-dispatch.md](namespace-dispatch.md).

Whether the mount table, the host name and the System V IPC key space are global or can differ between groups of processes. All three are global in `6.5.30/unix.IP35`; nothing in these paths reads a process, share group, session, job or array session to choose a table or a value. IP27 and IP30 were not read for this section. Addresses are `6.5.30/unix.IP35`. Reference pages are not used.

### Mount table

Global. Established.

- `mount` (0xc000000000289e8c) calls `cmount` (0xc000000000289eac), which resolves the mount point with `lookupname` (0xc0000000002c161c) and calls the covered vnode's mount operation (slot +0x168 of its behavior table, the `cover` slot, `fs_cover`). `fs_cover` (0xc000000000288ab4, 1364 bytes) allocates the new file system record (`vfs_allocate`, 0xc00000000028baf8), calls the file system type's mount routine (slot +8 of the `vfssw` entry, call at 0xc000000000288db8), then `vfs_add` (call at 0xc000000000288df0) and stores the new record in the mount-point vnode's word at +0x28 (store at 0xc000000000288e20).
- `vfs_add` (0xc00000000028b8d4, 320 bytes) takes the spin lock `vfslock` (0xc000000001645cb8) and links the record into one doubly linked list, inserting it right after the record that the global `rootvfs` (0xc00000000163d368) points to. `vfs_simpleadd` (0xc00000000028ba14, called from `spec_init`) does the same without the flag handling; `vfs_remove` (0xc00000000028ba6c, called from `fs_dounmount` at 0xc0000000002891f8) unlinks. No call in the mount path reads the calling thread, its root or its credential except `get_current_cred` for the file system's mount operation.
- The mount is found by walking, not by looking up a process view: `lookuppn` calls `traverse` (0xc0000000002c1f6c, 376 bytes; call at 0xc0000000002c1894) when the vnode's word at +0x28 is non-zero, and `traverse` reads that word, takes `vfslock`, and calls the covering record's root operation (slot +0x30). It takes no thread or root argument. So any process that reaches the covered vnode by any path crosses into the mount.
- `vfs_add` translates six mount-flag bits into the record's flags (1, 0x10, 0x2000, 8, 0x100, 0x200 of the argument); none of them is a hidden, private or per-process mode. A vnode that already has a covering record, or has bit 0x200 of its flags set (a mount in progress), is refused with 16 at the start of `fs_cover`.
- A `mount` or `lofs` mount made after `chroot` resolves the mount-point and source paths from the caller's `ut_rdir` (through `lookuppn` (`6.5.30/unix.IP35`, 0xc0000000002c1698, 2260 bytes), above) and is then visible to every process that reaches the mount-point vnode, including processes outside the root that name the same directory by its real path. This is a reading of the two routines above, not a run.
- No per-process, per-share-group or per-session vfs root exists beyond `ut_rdir` / `ut_cdir` (the 28-routine scan under "chroot: where the root is kept", which includes `lookuppn` (`6.5.30/unix.IP35`, 0xc0000000002c1698, 2260 bytes)). That the `proc`, `pshare_s`, `shaddr` and session records hold no mount pointer rests on that scan and on the absence of any other reader of a mount view, and is unverified; their debug records could not be dumped for this section (no `dwarfdump` on the host and `readelf` rejects the kernel's `.debug_abbrev`).

The one path-name hook the walk has, `mac_moldy_path` behind `mac_enabled` in `lookuppn`, is a stub in the shipped kernel and redirects walks inside a shared tree; see [trusted-irix.md](trusted-irix.md), "The multilevel-directory hook". It does not change the mount table.

Per-container mount view reachable without new kernel code: **no**. What is reachable is a static tree: a supervisor builds a directory tree, mounts (`lofs`, capability `1 << 29`; other types unknown, see the open question above) on directories inside it, and `chroot`s the container into it. The mounts are global and appear in the host's view at the host path. Inferred.

### Host name and domain name

Global. Established.

- `sethostname` (0xc000000000264ab8) needs `cap_able` with `1 << 28` (the mask `cap_style` uses), refuses a length over 256, then calls the `vhost_local` record's slot +0x28. `vhost_local` (0xc0000000003154a8, 8 bytes) returns the word `local_vhp` (0xc00000000163d610). `local_vhp` is set once, by `vhost_cell_init` (0xc000000000315488, called from `vhost_init`, 0xc0000000002de0a0), from `vhost_create` (0xc0000000002de0bc). There is no second host record and nothing selects one by caller.
- The host record's operations are `phost_ops` (0xc000000001599520, 136 bytes): slot +0x28 is `phost_sethostname` (0xc000000000264d20), +0x30 `phost_gethostname` (0xc000000000264cec). The set path is the routine at 0xc00000000026495c (216 bytes, between `systeminfo` and `gethostname`): it takes the mutex at `hostname` +0x210, copies the name into the global `hostname` (0xc0000000016a8c60, 257 bytes), stores the length in `hostnamelen` (0xc000000001645bb8, 2 bytes), copies the name up to its first `.` or NUL into the `nodename` member (+0x101) of the global `utsname` (0xc00000000158e638, 3341 bytes), and calls `cxfs_sethostname` (0xc000000000496124, 8 bytes, a return). `phost_gethostname` copies `hostname` out. `uname` (0xc000000000263ff0) copies the five 257-byte members at +0, +0x101, +0x202, +0x303 and +0x404 of `utsname`; `inituname` (0xc000000000263f58) fills it at boot. `systeminfo` (0xc000000000264354) reaches the same two globals (its set case calls `sethostname`, call at 0xc000000000264538). The domain name has the same shape (`phost_getdomainname` 0xc000000000264d50, `phost_setdomainname` 0xc000000000264d88).
- A scan of the kernel text for loads through the two global-pointer slots that hold the addresses of `hostname` and `utsname` (gp-0x2720, gp-0x2740) finds only `inituname`, `uname`, `systeminfo` (`6.5.30/unix.IP35`, 0xc000000000264354, 1544 bytes) and the `phost_*` name routines. `syssgi` (0xc0000000002e0310) is not among them, so no `syssgi` selector reads or writes the name. The scan covers those slots only; a use through some other way of forming the address was not searched for.
- `sethostname` therefore renames the host for every process. A process in a root with the root user id and the default `cap_enabled` passes the `1 << 28` test without any capability (see "The capability check"); one with a non-zero user id needs the capability bit.

UTS-namespace-like feasibility without new kernel code: **no** for a kernel-visible per-container name. Every kernel path reads one global. The `vhost` / `phost` indirection (a behavior table behind `local_vhp`) is the structure a cell-aware kernel uses; `cell_enabled` (0xc00000000163dbb0) is 0 in the file image and no code here picks a second record. `phost_ops` has no registration call (its two register slots only panic), and `uname`, `systeminfo` and the NFS client read the global name data without it; [namespace-dispatch.md](namespace-dispatch.md) gives the slot table, which archive member defines each routine, and what that means for mount, name and IPC. Whether a loadable module could displace a kernel symbol stays unknown (see [module-interfaces.md](module-interfaces.md)). A user-space substitute (an interposer in the container's `gethostname` and `uname` calls) is not shown to exist in the binaries: unknown, settled by reading the loader's environment handling in [userland-abi-and-loader.md](userland-abi-and-loader.md) for a preload list.

### System V IPC keys and ids

One global key space and one global id space per facility. Established.

- Shared memory: `shmsys` (0xc000000000317dd8) function 3 runs the routine at 0xc000000000317a24 (316 bytes, no symbol of its own), which gets the credential and the caller's process id, then calls `vshm_lookup_key` (0xc000000000316124, 464 bytes) with the key, flags and size. `vshm_lookup_key` takes the lock `vshm_lookup_lock` (0xc0000000016e1528) and walks the global table `vshmtab` (0xc000000001646568; `vshmtabsz` 0xc000000001646560 entries of 0x78 bytes), comparing the word at +0x14 of each entry with the key (compare at 0xc0000000003161b4). Found: the entry's own operations check access. Not found: `vshm_create` (0xc0000000003173cc). Ids come from `vshm_allocid` (0xc000000000315d10), a bit map bounded by the tunable `shmmni` (0xc00000000163c2ac), based at `shmid_base` (0xc000000001646570).
- Semaphores: `semsys` (0xc0000000000e39f0) function 1 runs the routine at 0xc0000000000e3660 (no symbol of its own, right after `semctl`), which calls `vsem_lookup_key` (0xc0000000000e4074, 472 bytes), the same walk over `vsemtab` (0xc000000001644eb8, `vsemtabsz` 0xc000000001644eb0), key compared at +0x14 (0xc0000000000e4114).
- Messages: `msgget` (0xc0000000000e4dd0) calls `ipcget` (0xc0000000000e61dc, 660 bytes) with the key, the flags and an index routine (0xc0000000000e5a2c, no symbol of its own, right after `msgsnd`). `ipcget` loops over the index of the one global array `msgque` (0xc000000001644ee8, `msgmni` entries, 0xc00000000163c028), compares the word at +0x20 of each in-use entry (flag 0x8000 at +0x10) with the key, and with key 0 (private) always creates a new entry. `msgget` uses the credential only for the permission and label calls. The shared permission test `ipcaccess` (0xc0000000000e60f8, 228 bytes) compares the credential's user and group ids with the entry's owner and group (`groupmember`, 0xc0000000002d1e74), tests the owner, group or other mode bits, and falls back to `cap_able_cred` (call at 0xc0000000000e61a4; the mask was not recorded).
- None of `shmget`, `semget`, `msgget`, `vshm_lookup_key`, `vsem_lookup_key` or `ipcget` reads a session, share group, job, project or array-session record, a root directory or a cpuset; they call no `arsess_*`, `sesmgr_*` or job routine. The credential and, for shared memory, the caller's process id are the only per-caller inputs, and they feed the permission check and the creator fields, not the key compare. Separate IPC per container is therefore not reachable by an existing partition. The credential gives no namespace either: two groups can only be kept apart by choosing disjoint keys (or `IPC_PRIVATE`) and by file-style permission bits, which a process with the same user id or the superuser id ignores.
- The label hooks are stubs here: `mac_msg_access` (0xc000000000497260), `mac_sem_access` (0xc000000000497278), `mac_shm_access` (0xc0000000004972a8) and the `mac_*_setlabel` / `_init` routines are 8-byte returns, and `mac_enabled` (0xc000000001646490) is 0 in the file image. A kernel that linked the real session manager and label code could partition by label; the held media do not carry it (see "The session-manager interface"). Unknown what that code would do.

Separate IPC key space without new kernel code: **no**. The distributed layers (`vshm_*`, `vsem_*`) take a `cell_enabled` path in a cellular configuration; that is a cluster mechanism and not a namespace. Inferred from the names and the zero `cell_enabled`.

## Differences on IP27

Established unless noted: the same routines and the same stub set; the thread record is 8 bytes shorter and puts `ut_cdir` and `ut_rdir` at +0x468 and +0x470; `allow_chroot_with_open_dirs` is also 1; `cap_style` (0xc0000000002872d8) and `cap_able_cred` (0xc000000000287350) exist at other addresses; the loopback file system is not linked in the shipped 6.5.30 IP27 kernel: it has no `lofs_init`, `lofs_mount` or `lofs_vnodeops` symbol and not the "lofs_rdev overflowed" message, and its only loopback symbols are `lofsfstype` and `lofs_icrash`, an 8-byte routine at 0xc0000000003c27c4 that returns at once (the stub build; established from the symbol table and strings); `pproc_set_dir` (0xc0000000002256f4, 440 bytes) and `pproc_procblk` (0xc00000000024b4b4, 400 bytes) have the sizes they have on IP35. The chain `fork` / `sproc` / `nsproc` was compared by routine name and size, not instruction by instruction on IP27, so the creation details above are inferred for IP27. The IP30 kernel was not read for this page.

## Corrections and open points

[syscalls.md](syscalls.md) calls the worker behind `sproc` an unlabelled 4936-byte routine at 0xc0000000002ea834. The debug records name it `sproc_common` (340 bytes); the following routines (`newthread_eframe`, `dofork`, `procfork`, `uthread_dup`, `thread_save`, `procdup`) account for the rest. It also leaves the three global switches unnamed: the word at gp+11552 is `mac_enabled`, gp+11556 is `sat_enabled` and gp-23488 is `cell_enabled` (0xc00000000163dbb0), all established from the symbol addresses and the gp value 0xc000000001643770.

The open questions this page leaves are in [open-questions.md](open-questions.md), section "Process model and isolation".

