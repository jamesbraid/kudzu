# Namespace dispatch: pointer tables and where the code lives

For the three kernel states that [process-model.md](process-model.md#namespace-scope) found to be global (the mount list, the host name, the System V IPC keys), this page answers two questions per behaviour. A: is the code reached through a table of function pointers that other code could extend, or through a hard call? B: does the routine that would be replaced live in the always-linked core object, or in a member of an archive (or a stand-alone optional object) that an explicitly linked object could supply in its place?

It does not say how to change kernel memory while the kernel runs. It only says what the shipped link inputs and the linked kernel show about dispatch and symbol placement.

All addresses are `6.5.30/unix.IP35` unless said. Grades: **established** (read in the binary or in a shipped file, with the address), **inferred** (reasoned, reasoning given), **unknown** (with the route that would settle it). 6.5.22 is covered at the end.

## How core and archive were told apart

- The link inputs are the 353 files of `6.5.30/usr/cpu/sysgen/IP35boot/`: 181 objects, 170 archives and two headers. `nm` over every object and every archive member gave a map from each global symbol to its defining member (22,140 definitions). `readelf -r` and `nm -u` over the same files gave which members reference which symbols. Read directly.
- A member counts as linked into the kernel when its functions are in `unix.IP35` at the same size. Every member named below matches in full (for example 12 of 12 functions of the archive member that holds `uname`, 4 of 4 of the one that holds `lookuppn`, 5 of 5 of `ipc.o`, 42 of 42 of `sem.o`). Read directly.
- The one mandatory core object is `kernel.o`: its configuration file (`6.5.22/var/sysgen.IP35/master.d/kernel`) carries the flag `k` ("only one such object can be configured into a kernel", `master.d/README`) and `system/irix.sm` names it on its `KERNEL:` line. It defines 1,177 global text symbols, 1,076 of them with a recorded size. None of the routines asked about is in it; it only calls into them (it references `vhost_local` and `sysent`). Read directly. The 6.5.22 configuration files are used because no 6.5.30 copy is held (the same limit as in [module-interfaces.md](module-interfaces.md)).
- `os.a` is a required module (`master.d/os` flags `oxr`; `INCLUDE: ... os ...` in `irix.sm`). All of the targets except the System V semaphore and message code are members of it. Of its 146 members that define globals, 145 have at least one of them in the kernel; the one that does not (`ktrace.o`, one function) is referenced by no boot object. Every `os.a` member that is in the kernel is referenced by some other boot object. That is what archive-member selection by unresolved reference looks like, and it is not what a link of the whole archive looks like. Reasoned: the order, flags and duplicate-symbol policy that `lboot` gives the linker are not held (`lboot` is not in `root/`), so whether a same-named symbol in a boot object wins, clashes or is ignored is **unknown**; the standard archive rule (a member is pulled only to satisfy an undefined symbol; a second strong definition clashes) is assumed below and flagged where it matters.
- The vendor's own build already swaps same-named code by configuration. A stubs archive (flag `u`: "stub functions ... always loaded after all other modules", `master.d/README`; 72 `*stubs.a` in the directory) defines the same entry points as its real module, and `lboot` links one or the other; `noncell.a` is described in its master file as "local versions of routines having cellular counterparts", and `os.a` itself carries `_lp` ("local") members next to the layer above them. A module that is absent gets only the stubs its master file lists. Read directly for the files; that the swap is the supported way to replace a module is reasoned.

## 1. Host name and domain name

### A. Dispatch: a pointer table behind one record, but only for part of the name state

Established.

- `sethostname` (0xc000000000264ab8, 208 bytes) calls `vhost_local` (0xc0000000003154a8, 8 bytes), which returns the word `local_vhp` (0xc00000000163d610). It then loads the record's first behavior descriptor (word +0), that descriptor's operations pointer (+0x10), and calls through slot +0x28 (indirect call at 0xc000000000264b5c) with the descriptor itself, the kernel copy of the name and the address of a stack word holding the length. `gethostname` (0xc000000000264a34, 132 bytes) does the same through slot +0x30 (indirect call at 0xc000000000264a68).
- The table is `phost_ops` (0xc000000001599520, 136 bytes, initialised data in the writable `.data` section, defined in the `os.a` member for the host layer). Slots, from the linked kernel:

  | Slot | Routine | Slot | Routine |
  | --- | --- | --- | --- |
  | +0x00 | header word 0x0000010100000000 | +0x50 | `phost_reboot` |
  | +0x08 | `phost_register` (0xc000000000314eb4) | +0x58 | `phost_sync` |
  | +0x10 | `phost_deregister` (0xc000000000314ed0) | +0x60 | `phost_sysget` |
  | +0x18 | `phost_sethostid` | +0x68 | `phost_systune` |
  | +0x20 | `phost_gethostid` | +0x70 | `phost_sysacct` |
  | +0x28 | `phost_sethostname` (0xc000000000264d20) | +0x78 | `phost_set_time` |
  | +0x30 | `phost_gethostname` (0xc000000000264cec) | +0x80 | `phost_adj_time` |
  | +0x38 | `phost_setdomainname` | +0x40 | `phost_getdomainname` |
  | +0x48 | `phost_killall` | | |

- The record is built once. `vhost_init` (0xc0000000002de0a0) calls `vhost_cell_init` (0xc000000000315488), which stores the result of `vhost_create` (0xc0000000002de0bc) in `local_vhp`. `vhost_create` allocates a 32-byte head, initialises it as a behavior head and calls `phost_create` (0xc000000000314e68, 76 bytes). `phost_create` allocates a 32-byte behavior descriptor, stores the address of `phost_ops` at +0x10 (the address comes from a literal at 0xc0000000016420a8) and inserts it as the first behavior. Nothing selects another record by caller.
- `phost_register` and `phost_deregister` (the two slots that sound like a registration interface) only call `panic`. There is no call that takes a table or a pointer and installs it. So an object cannot add itself: it can only be the object that defines `phost_ops` (see B), or write into the table (a runtime store, outside this page).
- Callers of `vhost_local` are 16 call sites in `adjtime`, `bdflush_wake_proc`, `getdomainname`, `gethostid`, `gethostname`, `sethostid`, `sethostname`, `setdomainname`, `settime`, `sync`, `syssgi` (2) and `uadmin` (4). The table is the host-wide operations of the machine (time, sync, reboot, kill-all, system tuning), with the host name as two of its slots.
- **Not through the table:** `uname` (0xc000000000263ff0, 288 bytes) reads the five 257-byte members of the global `utsname` (0xc00000000158e638) directly and is reached by a direct call from `utssys` (at `utssys`+0xfc) and by a pointer in both syscall tables (`sysent`+0xec0, `irix5_64_sysent`+0xec0). `systeminfo` (0xc000000000264354) reads `hostname` and `utsname` through global-pointer slots and calls `vhost_local` nowhere; its set case calls `sethostname`. `inituname` (0xc000000000263f58) fills `utsname` and is referenced from the boot-time main routine. Other kernel code reads the globals directly: `utsname` is referenced by `nfs.a(nfs_subr.o)`, `os.a(acct.o)`, `os.a(main.o)` and `os.a(vmdump.o)`, and `hostname` by `lockd.o` and `oncrpc.a(kauth_unix.o)`.
- So the name has two readers' families: callers of `gethostname`, `sethostname`, `getdomainname`, `setdomainname` go through `phost_ops`; callers of `uname`, `systeminfo` and the kernel-internal users read the globals. A per-container name made only by replacing slots +0x28 to +0x40 would show in `gethostname` and not in `uname`, `systeminfo` or the NFS client.

### B. Where it lives

Established for placement, inferred for what replacing it takes.

| Symbol | Archive member | Linked | Referenced from other members |
| --- | --- | --- | --- |
| `phost_ops`, `phost_create`, `phost_register`, `phost_deregister`, `phost_killall`, `phost_reboot`, `phost_sync`, `phost_set_time`, `phost_adj_time` | `os.a` host-layer member | yes | only `phost_create` (from the member holding `vhost_create`); `phost_ops` by no other member |
| `vhost_init`, `vhost_create` | `os.a` member | yes | `vhost_init` from the main routine and the local-provider member |
| `local_vhp`, `vhost_local`, `vhost_cell_init`, `vhost_lp_init` | `os.a` local-provider member (`vhost_lp.o`) | yes | `vhost_local` from 8 members plus `kernel.o`; `vhost_lp_init` from the main routine |
| `uname`, `utsname`, `hostname`, `hostnamelen`, `inituname`, `systeminfo`, `sethostname`, `gethostname`, `setdomainname`, `getdomainname`, `phost_sethostname`, `phost_gethostname`, `phost_setdomainname`, `phost_getdomainname` (23 global entries, 5 of them absolute symbols) | `os.a(name.o)` | yes | `uname` from the syscall-table member and `utssys`; `utsname` from 4 members; `hostname` from 2 (`lockd.o`, `kauth_unix.o`); `inituname` from the main routine; the `phost_*name` routines from the host-layer member |
| `phost_gethostid`, `phost_sethostid` | `bsd.a(bsdinit.o)` (a second copy is in `socket.a`) | yes | slots +0x18, +0x20 |

- None is in `kernel.o`. All are in `os.a` (required module) or `bsd.a`.
- Placement of a replacement, reasoned under the standard archive rule: a boot object that defines `phost_create` and `phost_ops` (and the other six exports of the host-layer member) leaves that member unpulled, and its table can point slots +0x28 and +0x30 at the object's own routines. Only `phost_create` has an outside reference, so the surface to supply is small. Replacing `uname` and the globals means supplying the whole of `name.o` (18 real definitions, 13 referenced from other members), because `utsname` and `hostname` are data that other members use.
- The vendor's own bsd master file lists `gethostname`, `sethostname`, `getdomainname` and `setdomainname` as stubs ("not installed") for a kernel without `bsd`; these four are also defined in `name.o`. Why both exist was not followed: unknown.

## 2. Mount view

### A. Dispatch: a hard structure walk with per-file-system vectors at the ends

Established.

- Crossing a mount point: `lookuppn` (0xc0000000002c1698, 2,260 bytes) looks up each path component with one indirect call (at 0xc0000000002c184c) through the directory vnode's behavior chain (vnode word +0x60, its operations at +0x10). When the vnode's word at +0x28 is non-zero it calls `traverse` (0xc0000000002c1f6c, 376 bytes; direct call at 0xc0000000002c1894). `traverse` reads that word (a covering file-system record), takes `vfslock`, and calls the root operation through the record's own behavior chain (word +0x68, operations at +0x10, slot +0x30; indirect call at 0xc0000000002c2038). The choice of whether and what to cross is a field read; no per-process, per-root or per-file-system vector is consulted for it. The file-system vector is consulted only to obtain that file system's root vnode.
- Placing a mount: `mount` calls `cmount` (0xc000000000289eac), which resolves the path with `lookupname` (direct call at 0xc000000000289ee4) and calls the cover operation of the covered vnode through its behavior chain (slot +0x168 of the vnode operations; indirect call at 0xc000000000289f48). The default implementation of that slot is `fs_cover` (0xc000000000288ab4), which is the value in the vnode-operations tables of ten file-system types (the autofs, cachefs, efs, hwgfs, lofs, namefs, nfs, nfs3, udf and xfs members). `fs_cover` itself calls the file-system type's mount routine through the type's own vector (`vfssw` slot +8, per the earlier section) and `vfs_add` (direct), and stores the covering record in the vnode's word +0x28.
- So there is a per-file-system-type vector (vnode and vfs operations, 18 and 15 tables, see the general section) that a new file-system type fills, but the generic mount list (`rootvfs`, one list, `vfs_add`) and the crossing walk are not vectored. Mount point crossing is shared by all file systems and all processes.

### B. Where it lives

Established.

| Symbols | Archive member | Linked | Referenced from other members |
| --- | --- | --- | --- |
| `lookupname`, `lookuppn`, `traverse`, `lookup_init` | `os.a(lookup.o)` (4 functions, 4 of 4 linked) | yes | `lookupname` from about thirty members (file systems, loaders, NUMA and others); `lookuppn` from 4 (`exec.o`, `intp.o`, `vncalls.o`, `vnode.o`); `traverse` from `lofs.a(lofs_vnops.o)` and `snfs.a(nfs_server.o)` |
| `mount`, `cmount`, `vfs_add`, `vfs_remove`, `rootvfs` and 62 other global entries | `os.a(vfs.o)` | yes | `cmount` from `eag.a`; `vfs_add` from 5; `mount` from the syscall-table member; many of the rest from file-system members |
| `fs_cover` and 33 other global entries (`fs_nosys`, `fs_noerr`, `fs_dounmount`, ...) | `os.a(fs_subr.o)` | yes | `fs_cover` from the ten file-system members above; the others from nearly every file-system member |

- None is in `kernel.o`; all are in `os.a`, so the always-linked code is in an archive, not a stand-alone object. Read directly.
- Placement of a replacement, reasoned under the standard archive rule: the crossing walk is replaceable by supplying `lookup.o`'s four functions from another object (three are referenced from outside; `lookup_init` has no outside reference in the boot objects). That member is small. `vfs.o` (67 global entries) and `fs_subr.o` (34) are wide: they are the interface every file system links against, so replacing `fs_cover` or `cmount` as a whole member is not a small change. A per-container mount list therefore looks like a replacement of the crossing in `lookup.o` over the existing global cover words, not a replacement of mount placement.
- Another route needs no replacement and is reasoned only: a new file-system type, registered through the vector tables below, answers its own directory lookups through its own vnode operations and can return a different view by calling process. The existing `lofs` member shows the pattern (it calls `traverse` and `fs_cover`). Not tested.

## 3. System V IPC keys

### A. Dispatch: a direct scan of a global table; only the matched entry is vectored

Established.

- Shared memory: `shmsys` (0xc000000000317dd8, in the `os.a` member for the system call) function 3 runs the routine at 0xc000000000317a24, which calls `vshm_lookup_key` (0xc000000000316124, 464 bytes) directly. That routine scans `vshmtab` (0xc000000001646568, `vshmtabsz` entries), compares the word at +0x14 of each entry with the key (0xc0000000003161b4), then, for an entry that matches, calls the entry's key-check operation through its behavior chain (entry word +0x20, operations +0x10, slot +0x10; call at 0xc0000000003161e4). That slot is `pshm_keycheck` in `pshm_ops` (0xc000000001599600, 80 bytes; slots +8 `pshm_getmo`, +0x10 `pshm_keycheck`, +0x18 `pshm_attach`, +0x20 `pshm_setdtime`, +0x28 `pshm_rmid`, +0x30 `pshm_ipcset`, +0x38 `pshm_getstat`, +0x40 `pshm_mac_access`, +0x48 `pshm_destroy`). A second indirect call at 0xc000000000316298 is slot +0x40 (`pshm_mac_access`) on a newly created entry. Creation is a direct call to `vshm_create`.
- Semaphores: `vsem_lookup_key` (0xc0000000000e4074, 472 bytes) is the same shape over `vsemtab` (0xc000000001644eb8): key compare at +0x14 (0xc0000000000e4114), then slot +8 of the entry's operations (`psem_keycheck` in `psem_ops`, 0xc00000000155a5b0, 128 bytes; call at 0xc0000000000e4140). Direct call to `vsem_create`.
- Messages: `ipcget` (0xc0000000000e61dc, 660 bytes) walks `msgque` through two function pointers that its caller passes in as arguments (indirect calls at 0xc0000000000e6234, 0xc0000000000e625c, 0xc0000000000e6370, 0xc0000000000e639c); the caller `msgget` passes its own local routines, so the indirection is a parameter of one call, not a table that another object can extend. The key compare is in `ipcget`.
- So each facility has a per-object vector (`pshm_ops`, `psem_ops`) but the part that decides which object a key names, the scan of one global table, is not behind a vector in any of the three. A per-container key space needs the compare to include a container identity, so it needs the lookup routine itself replaced, not only an operations table.

### B. Where it lives

Established.

| Symbols | Member | Linked | Referenced from other members |
| --- | --- | --- | --- |
| `vshm_lookup_key`, `vshmtab`, `vshmtabsz` and 22 other globals | `os.a(vshm_mgr.o)` | yes | `vshm_lookup_key` from the system-call member (2 sites); 14 symbols in all from `shm_syscalls.o`, `pshm.o`, `vshm_mgr_lp.o`, `swapalloc.o`, `main.o` and `cpr.a`; `vshmtab` only within this member |
| `shmsys` and 5 other globals | `os.a(shm_syscalls.o)` | yes | `shmsys` from the syscall-table member |
| `pshm_ops` | `os.a(pshm.o)` | yes | from `vshm_mgr_lp.o` |
| `vsem_lookup_key`, `vsemtab`, `psem_ops`, `semsys` and 53 other globals | stand-alone `sem.o` (57 globals) | yes | **`semsys` only** among its code and data symbols, from the syscall-table member (`idbg.o` also names its absolute `mac_enabled` and `sat_enabled`, which about a hundred members define alike and which are not links into it) |
| `ipcget`, `msgque` and `msgsys` | stand-alone `msg.o` (20 globals), `ipcget` in stand-alone `ipc.o` (6 globals) | yes | `msgsys` from the syscall-table member and `uncintf.o`; `ipcget` from `msg.o` only; the other `ipc.o` symbols (`ipcaccess`, `ipc_init`, the two permission converters) from `msg.o`, `sem.o`, `pshm.o`, `shm_syscalls.o`, `xpmem.o` |

- None is in `kernel.o`. Shared memory is in the required archive; `sem.o`, `msg.o` and `ipc.o` are separate objects that `irix.sm` names on `USE:` lines and whose master files list stubs for the case that they are absent (`semsys`, `semexit`; `msgsys`; `ipc_init`, `ipcaccess`, the two converters; all "not installed"). Read directly.
- Consequence, reasoned under the standard rule: the semaphore and message modules are whole modules whose only outside entry points are the syscall functions `semsys` and `msgsys`, which the syscall table points at. A replacement object that defines those two and its own key space, and uses the existing `ipc.o` for permissions, replaces them completely. This is the module-level substitution the configuration language is built for. Shared memory is different: its lookup is in `vshm_mgr.o`, which has 14 symbols used from other members, so it is a replacement of one archive member of the required module (with the local-provider member that calls it).

## General characterisation

Measured on `6.5.30/unix.IP35` unless marked.

- **Indirect calls are rare in code, common in the shape of the data.** Counting instruction words in `.text` that are a direct call (`jal`) or a register call (`jalr`): 81,035 and 4,171, so 4.9% of calls are register-indirect. Almost all calls are direct; the indirect ones go through the tables listed next. The count is an opcode scan of the section and includes any data words in it.
- **Behavior operations tables (vector, extendable by data):** the pattern is a head record, a behavior descriptor whose word +0x10 holds an operations table, and an indirect call through a slot. Tables of this kind are `phost_ops`, `pshm_ops`, `psem_ops`, `psession_ops`, `ppgrp_ops`, `pfile_ops` and `pas_ops`, plus 18 vnode-operations tables and 15 file-system (vfs) operations tables (types `xfs`, `efs`, `nfs`, `nfs3`, `lofs`, `autofs`, `udf`, `umfs`, `fifo`, `pipe`, `spec`, `nm`, `fdfs`, `hwgfs` and the process file system; the vnode tables add a dead-vnode table and two more for devices). All are static initialised data in the same archive member as the code that creates the object. No registration call exists for the host, shared-memory and semaphore tables (see above); a file system is registered through the switch below.
- **Generated switch tables (extendable by configuration):** the file-system switch `vfssw` (0xc00000000154a798, 0x3f0 bytes), the character and block device switches `cdevsw` (0xc000000001542f78, 0x3d28) and `bdevsw` (0xc000000001540538, 0x29d8), and the streams module switch `fmodsw` (0xc00000000154a3b8, 0x3e0) are defined by no boot object. The shipped 6.5.22 `var/sysgen.IP35/master.c` is marked as generated by `lboot` and defines arrays of those names, built from the master files (flags `j` file system, `c` and `b` devices, `f` and `m` streams). So a new file system, device or streams module is added by a master file, an object in the boot directory and a `USE:` line. The 6.5.30 link of those arrays is reasoned from the 6.5.22 files.
- **Static dispatch table:** the system-call tables `sysent` and `irix5_64_sysent` (`sysent` at 0xc0000000015940e0, 0x1638 bytes) are initialised data in one `os.a` member, read by `kernel.o`, `proc.a(priface2.o)` and `uncintf.o`. Each entry holds the handler's address (`uname` at +0xec0). Whether anything writes the table at run time was not searched: unknown.
- **Hard calls and global tables:** the mount list and crossing, `utsname` and `hostname`, `vshmtab`, `vsemtab`, `msgque`, the credential and capability checks. These carry no vector.
- **Loadable modules** (flag `d`, loaded by the module loader) resolve their undefined names against the kernel's symbol table and add code and data; the loader in [module-interfaces.md](module-interfaces.md) has no rule for a module symbol that replaces a kernel one. A loadable module can therefore fill a switch table but not displace `phost_ops` or the lookup routines. Reasoned from the loader's name lookup; the replace-versus-reject case was not read.
- **Interrupt vectors, streams message dispatch and the device-driver entry points** were not read for this page: unknown.

## Bottom line

The consumer's question is which of mount, host name and IPC look reachable by supplying our own kernel objects at link time, rather than by changing kernel memory while it runs. All of this depends on the unknown linker policy for duplicate definitions above.

| Feature | Dispatch | Replaceable unit | Reachable by adding or relinking objects |
| --- | --- | --- | --- |
| Host and domain name via `gethostname`, `sethostname` and the domain calls | table (`phost_ops` slots +0x28 to +0x40) | the `os.a` host-layer member: define `phost_create` and `phost_ops` | **yes**, small surface (inferred) |
| `uname`, `systeminfo`, NFS client name, accounting | global data read directly | `os.a(name.o)` whole (13 symbols used elsewhere) | **yes, but wide** (inferred); the table alone is not enough |
| Mount crossing (per-container view) | hard walk of the vnode word +0x28 | `os.a(lookup.o)`, 4 functions | **yes, moderate** (inferred): a view layer in `traverse` and `lookuppn` |
| Mount placement and the mount list | vnode cover slot, global list | `os.a(vfs.o)` (67 global entries), `os.a(fs_subr.o)` (34) | wide; not needed if the crossing is replaced instead |
| Semaphore and message key spaces | direct scan, per-entry table | stand-alone `sem.o` and `msg.o`; only `semsys` and `msgsys` are outside entry points | **yes, cheap** (inferred): whole-module substitution |
| Shared-memory key space | direct scan, per-entry table | `os.a(vshm_mgr.o)` (14 outside-referenced symbols) | **yes, moderate** (inferred) |

Nothing in the six rows needs a run-time memory change if relinking with our own boot objects is acceptable. The run-time alternative for the name, storing into `phost_ops`, would be incomplete anyway because `uname` bypasses the table.

## 6.5.22

The same members hold the same symbols (`os.a` for `phost_ops`, `local_vhp`, `vhost_local`, `phost_create`, `lookuppn`, `traverse`, `cmount`, `fs_cover`, `uname`, `sysent`, `vshm_lookup_key`; stand-alone `sem.o` for `vsem_lookup_key`, `ipc.o` for `ipcget`). `phost_ops` is at 0xc00000000150cf68 (136 bytes) with the same 17 slots and the same routine order (`phost_sethostname` 0xc00000000022d630 in slot +0x28, `phost_gethostname` 0xc00000000022d5fc in +0x30); `sethostname` (0xc00000000022d3c8) reaches slot +0x28 through `vhost_local` (0xc0000000002dba30) with the same offsets. The other addresses were not re-read for 6.5.22.

## Open

- The link order, flags and duplicate-symbol policy of `lboot`'s linker call. It decides whether a boot object can supply a symbol that an archive member also defines, or must replace the whole member. Route: `lboot` is not in `root/`; read the strings and output of an `lboot` binary from a medium that has it, or a link log.
- Whether the loader of loadable modules can ever bind a module's definition over a kernel symbol: the rule in [module-interfaces.md](module-interfaces.md) was read for undefined names only.
- Argument conventions of the three-argument host operations beyond the call site (what the callee returns in the length word).
- Whether anything writes `sysent` or the vfs and device switches after boot.
