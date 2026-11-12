# Userland libraries and the instruction-set level

Two questions about the shipped user space that other projects ask before they plan containers, binary translation or a kernel extension: which functions the core libraries export, and which MIPS instruction-set level the programs and the kernels actually need. Both are answered for IP35 first; the IP27 differences are noted where they exist. The libraries do not depend on the platform: the same user-space files install on IP27, IP30 and IP35, and the media selects by board only a few standalone files such as the loaders (inferred from the board suffixes in the media's file names).

## Evidence and what is held

- **6.5.30 user space** is the installed files of the 6.5.30 installation and overlay discs, read at their install paths (`usr/lib32/mips3/libc.so.1` and so on). Only the files [IRIX_FILES.md](../IRIX_FILES.md) lists are in `root/6.5.30` (the loaders, the C, maths and thread libraries and the programs behind the open questions), so the tables below were generated from a separate extraction of those discs: the three-disc 6.5.30 Installation Tools and Overlays media named in [IRIX_FILES.md](../IRIX_FILES.md), unpacked so that each install path appears once under one directory. That directory is not in the repository. A reader who unpacks the same media can rerun `scripts/userland_symbols.py ROOT OUTDIR` on it; the 6.5.30 tables were reproduced byte for byte that way during review, and the 6.5.22 tables were not re-derived by the reviewer.
- **6.5.22 user space** is the same read from the 6.5.22 Installation Tools and Overlays discs (the product is `eoe_6522m`). Only those three discs are held. The 6.5.22 patch sets and the applications and complementary discs are not, so anything a later 6.5.22 patch changed is not seen here.
- **Kernels** are the linked `6.5.30/unix.IP35`, `6.5.30/unix.IP27` and their 6.5.22 counterparts in `root/`, plus the kernel objects in `usr/cpu/sysgen/IP27boot/` and `IP35boot/` for both releases.
- Signatures come from the published IRIX manual pages (`pthread_create(3P)`, `poll(2)`, `mmap(2)` and the others named below). No header was opened.
- Method: `scripts/userland_symbols.py` writes the export tables from `.dynsym`; `scripts/userland_isa.py` classifies every aligned word of every executable section by its encoding. Both are re-runnable. `scripts/hostfacts.py` and Capstone were used on a few routines to read the instructions behind a count.

Grades are as in [AGENTS.md](../AGENTS.md): established (shown by the binary or a published manual), inferred (with what it rests on) or unknown.

Generated files, all in [userland-symbols/](userland-symbols/):

| File | Contents |
|---|---|
| `6.5.30/<lib>-<abi>.tsv`, `6.5.22/<lib>-<abi>.tsv` | one row per exported symbol: name, type, binding, size in each CPU build of the library (`-` where that build lacks the name) |
| `6.5.30/summary.tsv`, `6.5.22/summary.tsv` | export counts per file |
| `6.5.30/probe.tsv`, `6.5.22/probe.tsv` | which of 168 thread, wait, mapping, signal, socket and loader names each library exports, with type, binding and size |
| `isa-6.5.30.tsv`, `isa-6.5.22.tsv` | instruction-class counts for every ELF executable and shared object of the user-space set (5419 and 4166 files) |
| `kernel-isa.tsv` | the same counts for the six linked kernels |
| `kernel-objects-isa.tsv` | the same counts for the kernel objects of both releases (659 files) |

## Exported functions of the core libraries (question 20)

### Where each part lives

Established from the file list and the dynamic symbol tables.

- **libc** is `6.5.30/lib32/libc.so.1` for n32 (and the o32 copy in `lib/`), and `6.5.30/usr/lib64/mips3/libc.so.1` or `6.5.30/usr/lib64/mips4/libc.so.1` for n64. Each ABI ships two builds of libc: one flagged MIPS III and one flagged MIPS IV (see the second half). The two builds of an ABI export the same names except two: `__memcpy_r8000` (1076 bytes) exists only in the MIPS IV build and `_memcpy_fp` (40 bytes) only in the MIPS III build, in both n32 and n64 and in both releases, so each table has one row more than either build has exports (the union of the two builds). Of the shared names, 596 differ in size in n32.

**The dynamic loader and `dlopen`.** `dlopen`, `dlsym`, `dlclose`, `dlerror`, `sgidladd` and `sgidlopen_version` are exported by libc itself (n32 and n64, 6.5.30 and 6.5.22; for n32, `dlopen` is a weak 112-byte routine and `_dlopen` its global twin). There is no separate n32 `libdl`. The only `libdl.so` on the media is `usr/lib64/abi/libdl.so`, an n64 compatibility stub with 11 exports (the four `dl*` names, their `_dl*` twins and three linker-defined symbols, each `dl*` routine 8 bytes long), part of the portable-ABI library set, not the library programs normally link. The loader itself is `lib32/rld` and `lib64/rld`, which export no dynamic symbols. Established.

**POSIX threads** are in `6.5.30/usr/lib32/libpthread.so` and `6.5.30/usr/lib64/libpthread.so`, which need only `libc.so.1`. libc also exports 22 weak `pthread_*` names (among them `pthread_create`, `pthread_mutex_*`, `pthread_cond_*`, `pthread_exit`, `pthread_self`, `pthread_testcancel`, `pthread_attr_init`), and the semaphore calls `sem_init`, `sem_wait`, `sem_post`, `sem_trywait`, `sem_open` and `sem_destroy`. Established.

**libm** is `6.5.30/usr/lib32/mips3/libm.so`, `6.5.30/usr/lib32/mips4/libm.so` (plus an R8000 build) and the same three for n64. Established.

### Export counts

Defined, non-local `.dynsym` entries. Established.

| Library | ABI | 6.5.30 | 6.5.22 | Grade |
|---|---|---:|---:|---|
| libc | n32 | 4737 | 4608 | established |
| libc | n64 | 4775 | 4646 | established |
| libpthread | n32 and n64 | 219 each | 207 each | established |
| libm | n32 and n64 | 755 each | 755 each | established |
| abi `libc.so` (n64) | n64 | 1554 | 1554 | established |
| abi `libdl.so` (n64) | n64 | 11 | 11 | established |

n32 and n64 differ little. In 6.5.30 libc, n32 has one name n64 lacks (`_finddatum64`) and n64 has 39 that n32 lacks: `aio_init`, `_aio_init`, the `_old_aio_*` and `old_aio_*` set, `dbm_nextkey64` and the `_*_compat` dbm names. libpthread and libm export the same names in both ABIs. Established from the tables.

6.5.30 against 6.5.22 libc (same in n32 and n64): 133 names added and 4 removed. all 133 added names are ONC RPC, XDR and netconfig helpers by name: the `svc_*` creation routines, the `rpcb_*` calls and the `xdr_rpcb*` and `_xdr_rpcb*` encoders, the `rpc_gss_*` and `__rpc_gss_*` calls, `xdr_netbuf` and `xdr_sizeof`, `__rpc_getconf`, `__rpc_setconf` and related netconfig routines, `netdir_error` and `terror_str` internals, and the `xprtlist_lock` and `proglst_lock` variables; the removed four are `__clntudp_bufcreate_sa`, `__pmap_findport_sa`, `__rpc_nettype_to_proto` and `__rpc_sockaddr_new_netbuf`. libpthread gained 12 names, the barrier set (`pthread_barrier_init`, `_destroy`, `_wait`, and the four `pthread_barrierattr_*`) and the spin-lock set (`pthread_spin_init`, `_destroy`, `_lock`, `_trylock`, `_unlock`). libm is identical by name. The RPC grouping is inferred from the names (the listed prefixes cover only part of the 133). Established for the counts and names.

Versioning. The libraries carry no per-symbol version table (no `.gnu.version` sections); each records one interface version string (`DT_MIPS_IVERSION`, `sgi1.0` in the n32 libpthread) and a timestamp and checksum. The symbol lists therefore have no version column. Established from the section list and dynamic tags of the 6.5.30 n32 libc and libpthread; the other libraries were not checked for a version table.

### How the thread calls fit together

libc's `pthread_create` is a stub. At `libc.so.1` (n32, MIPS III build) 0xfb1ee44, 84 bytes, it loads a pointer through the global offset table (the slot at gp-0x79ec holds 0xfb4d290, the address of the 4-byte object `_mtlib_ctl`, resolved from the dynamic symbol table); if that pointer is null it returns 0x59 (89) without calling anything, otherwise it calls the routine at offset 0xb4 of the table that pointer addresses. The other 21 weak `pthread_*` names are 64 to 88 bytes long, the same order of size. Only `pthread_create` was disassembled. Established for `pthread_create`; the other stubs are inferred from their size and name.

The published `pthreads(5)` page says that any pthread call made before the pthread library is loaded returns ENOSYS, and that the pthread library can be loaded later with `dlopen` or `sgidladd`. The value 89 is therefore ENOSYS (inferred: 89 is the number IRIX assigns, from the same page's statement, not from a table read here). Inferred.

libpthread exports `_SGIPT_libc_*` entries (`_SGIPT_libc_sigaction`, `_SGIPT_libc_sigprocmask`, `_SGIPT_libc_sigwait`, `_SGIPT_libc_fork`, `_SGIPT_libc_sem_wait` and others) and `_SGIPT_pt_usync_cntl`. These are the routines libc's hook table points at once libpthread is loaded, and the binding is now read. libc's `__new_libc_threadinit` (`6.5.30/lib32/libc.so.1`, 0xfb181b8, 260 bytes) stores its second argument into `_mtlib_ctl` (the `sw` at 0xfb18200, through the same GOT slot `pthread_create` uses), then calls through that table's first and `+0x18` entries (0xfb18204 to 0xfb18244). libpthread (`6.5.30/usr/lib32/libpthread.so`) calls it from `_SGIPT_libc_locks_init` (0xc008c3c) with 2 and the address of its table (the GOT slot at gp-0x7e14, value 0xc0187c8 in `.data`), and `_SGIPT_pt_bootstrap` calls `_SGIPT_libc_locks_init` (0xc00c1c8). The table is 48 words or more of routine addresses: `+0x28` to `+0x98` are the `_SGIPT_libc_*` routines (`sem_init` at +0x28, then `sem_getvalue`, `sem_post`, `sem_wait`, `sem_trywait`, `sem_destroy`, `blocking`, `unblocking`, `fork`, `sched_yield`, `setrlimit`, `sysconf`, `exit`, `raise`, `sigaction`, `siglongjmp`, `sigpending`, `sigprocmask`, `sigsuspend`, `sigtimedwait`, `sigwait`, `evt_start`, `cncl_test`, `cncl_list`, `threadbind`, `threadunbind`, `ptpri`, `pttokt`, `ptuncncl`) and `+0x9c` to `+0xbc` are `pthread_attr_init`, `pthread_attr_setdetachstate`, `pthread_cond_broadcast`, `pthread_cond_init`, `pthread_cond_signal`, `pthread_cond_wait`, `pthread_create` and `pthread_exit` and `pthread_mutex_init`. The slot libc's `pthread_create` stub calls (`+0xb4`) holds `pthread_create` at 0xc00c9cc. Established for the n32 MIPS III libc and `6.5.30/usr/lib32/libpthread.so`; the o32 and 64-bit libraries and the other 21 stubs were not read, and the meaning of the argument 2 is Unknown (the other value, 1, was not traced to a caller).

libpthread imports `__usync_cntl`, `nsproc`, `prctl`, `__sigpoll`, `__sched_yield`, `__nanosleep`, `__sginap` and `sysmp` from libc, and `mmap`, `mprotect` and `munmap`. The kernel has system calls of the same names: `usync_cntl` (1223), `nsproc` (1204), `sigpoll_sys` (1166), `prctl` (1130). So POSIX threads are built on the kernel's process-share and user-synchronisation calls (inferred from matching names; the call sites were not read). Inferred.

### The call families asked about

All rows are from `probe.tsv`. Present means exported by the library named; the sizes of 0 are assembly system-call stubs with no recorded size.

| Family | Present (6.5.30, n32 and n64) | Not exported by any core library of the shipped 6.5.30 installation |
|---|---|---|
| POSIX threads | libpthread: create, join, detach, exit, self, equal, once, cancel, testcancel, setcancelstate, setcanceltype, attr init, destroy, setstacksize, setdetachstate, setscope, mutex init, destroy, lock, trylock, unlock, mutexattr init, settype, setpshared, setprotocol, cond init, destroy, wait, timedwait, signal, broadcast, condattr init, rwlock init, rdlock, wrlock, unlock, barrier init and wait, spin init and lock, key create, delete, getspecific, setspecific, sigmask, kill, atfork, setschedparam, getschedparam, `pthread_setrunon_np`. libc: `sem_*`, `sproc`, `sprocsp` | `pthread_mutex_timedlock`. `pthread_cleanup_push` and `_pop` are not exported by libc or libpthread (the `pthreads(5)` page names them; whether they are macros over other exported names is unknown). |
| Wait for events | `poll`, `select`, `sigtimedwait` in libc | `pselect`, `ppoll`, `epoll_create`, `epoll_ctl`, `epoll_wait`, `kqueue`, `kevent`, `port_create` |
| Memory mapping | `mmap`, `mmap64`, `munmap`, `mprotect`, `msync`, `madvise`, `mlock`, `munlock`, `mlockall`, `munlockall`, `shm_open`, `shm_unlink`, `shmget`, `shmat`, `shmdt`, `shmctl`, `usinit` | `mremap`, `mincore` |
| Signals | `signal`, `sigaction`, `sigprocmask`, `sigsuspend`, `sigpending`, `sigwait`, `sigwaitinfo`, `sigtimedwait`, `sigqueue`, `sigaltstack`, `sigsend`, `sigsendset`, `sigset` family, `sighold`, `sigrelse`, `sigignore`, `sigpause`, `kill`, `raise`, `killpg`, `sigstack`, `siglongjmp`, `sigsetjmp`, `sigvec`, `sigblock`, `sigsetmask`; libpthread adds `pthread_sigmask`, `pthread_kill` | none of the names probed |
| Sockets | `socket`, `socketpair`, `bind`, `listen`, `accept`, `connect`, `send`, `sendto`, `sendmsg`, `recv`, `recvfrom`, `recvmsg`, `shutdown`, `getsockname`, `getpeername`, `getsockopt`, `setsockopt`, `gethostbyname`, `getaddrinfo`, `getnameinfo`, `freeaddrinfo`, `gai_strerror`, `inet_pton`, `inet_ntop`, `inet_ntoa`, `inet_addr`, `if_nametoindex`, `if_indextoname` | `accept4`, `getifaddrs`, `sendfile` |
| Dynamic loader | `dlopen`, `dlsym`, `dlclose`, `dlerror`, `sgidladd`, `sgidlopen_version` (libc) | `dladdr`, `dlinfo` |

Established as exports. The 6.5.22 table differs only in lacking the barrier and spin calls (the 12 added names above); every other row is the same in 6.5.22.

The kernel side agrees for the calls that are plain system calls. The 6.5.30 IP35 kernel table has `poll` (1087), `select` (1100), `xpg4_select` (1232), `socket` (1107), `socketpair` (1155), `accept` (1089), `mmap` (1134), `mmap64` (1185), `munmap` (1135), `mprotect` (1136), `msync` (1137), `madvise` (1138), `sigaction` (1162), `sigprocmask` (1164), `sigsuspend` (1165), `sigqueue_sys` (1180) and `sproc` (1129); see [syscalls.md](syscalls.md). The shipped 6.5.30 kernel links no function whose name contains `epoll` or `kqueue`. It does carry `devpoll_open`, `devpoll_write`, `devpoll_ioctl` and `devpoll_close` (`devpoll_open` at IP35 0xc0000000000dadf8), and the published `poll(7)` page describes a `/dev/poll` device driven by `write` and `ioctl`. So the large-descriptor-set wait on IRIX is `/dev/poll`, not an epoll-style call. Established that the symbols and the page exist. The write and ioctl routines were then read in the 6.5.30 IP35 kernel (`devpoll_write` 0xc0000000000db2e8, `devpoll_ioctl` 0xc0000000000dbc9c) and agree with the page on the points they cover: the three ioctl numbers are 0x647001 (poll), 0x647002 (is this descriptor polled) and 0x647003 (remove on close), the same values the shipped `sys/devpoll.h` gives (`'dp'` shifted left 8, or 1, 2, 3); any other number gives 22. Each instance records the file-descriptor table of the process that opened it (record +0x38); every write and ioctl compares it with the caller's current table (thread record +0x300, then +0x2b0) and fails with 13 (EACCES) on a mismatch, which is how a forked child or a non-sharing `sproc` child is refused. A write is taken as an array of 8-byte `pollfd` entries (byte count shifted right by 3); up to 64 entries are copied into a stack buffer and a longer array into a `kmem_alloc` buffer, with 14 for a copy fault, 12 when the allocation fails and 22 otherwise. The poll ioctl copies in a 24-byte `dvpoll` through the ABI translation routine `copyin_xlate` (pointer, count, timeout; 32-bit callers get the translated form), converts the millisecond timeout to seconds and nanoseconds, validates it with `itimerfix` (22 when invalid) and sleeps on a signalable wait. The ISPOLLED ioctl copies an 8-byte `pollfd` in and, when the result pointer is set, back out. The remove ioctl ORs the command number into a flag word at record +0x30 under the record's spin lock (+0x170). Established for each routine and constant named; the consumers of the +0x30 flag, the use of the 4096 stored at +0x4c of the poll worker's local record, and the exact blocking and wake-up rules were not read, so the page remains the only statement of those.

- Signatures, from the published pages: `int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start)(void *), void *arg)` (`pthread_create(3P)`); `int poll(struct pollfd *fds, unsigned long nfds, int timeout)` (`poll(2)`); `void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)` and the same with `off64_t` for `mmap64` (`mmap(2)`); `int socket(int domain, int type, int protocol)` (`socket(2)`); `int sigqueue(pid_t pid, int signo, const union sigval value)` (`sigqueue(3)`); `pid_t sproc(void (*entry)(void *), unsigned inh, ...)` (`sproc(2)`). The `sigaction(2)` page gives a `struct sigaction` that starts with `int sa_flags`, then the handler union, then `sigset_t sa_mask`. Signatures for the remaining names come from the shipped manual pages and system headers: `int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)` (`select(2)`, `unistd.h`, `sys/time.h`; `sys/time.h` redirects it to `__xpg4_select` when both the XPG4 and XPG5 feature tests are off, which is the `xpg4_select` kernel entry); `void *dlopen(const char *, int)`, `void *dlsym(void *, const char *)`, `int dlclose(void *)`, `char *dlerror(void)`, `void *sgidladd(const char *, int)` and `void *sgidlopen_version(const char *, int, const char *, int)` (`dlfcn.h`, which also gives `RTLD_LAZY` 1, `RTLD_NOW` 2 and `RTLD_GLOBAL` 4; no `dlopen(3)` page is held); `int sigwait(const sigset_t *set, int *sig)`, `int sigwaitinfo(const sigset_t *set, siginfo_t *value)` and `int sigtimedwait(const sigset_t *set, siginfo_t *value, timespec_t *timeout)` (`sigwait(3)`; the header declares the timeout `const`); `int sem_wait(sem_t *sem)` and `int sem_trywait(sem_t *sem)` (`sem_wait(3C)`, `semaphore.h`); `int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)` (`pthread_cond_wait(3P)`, `pthread.h`). All established from the 6.5 and 6.5.30 media (the 6.5.30 `pthread.h` and `semaphore.h` agree with the pages). `pthread_mutex_timedlock` appears in no held header or page and in no string of either release's `libpthread`, so no signature exists in the shipped software: unknown, and nothing shipped depends on it.

### Summary

libc (4737 exports for n32, 4775 for n64 in 6.5.30) holds `dlopen` and its family, `poll`, `select`, `mmap`, the signal calls, the socket calls and the semaphore calls; libpthread (219 exports) holds the POSIX thread calls and is loaded on demand over weak stubs in libc; libm has 755 exports. No epoll or kqueue and no `pselect` is exported by the shipped libc, and there is no separate n32 `libdl`. 6.5.22 differs by 133 more libc names (RPC) and 12 more libpthread names (barriers and spin locks) in 6.5.30.

## Instruction-set level (question 21)

### What was counted

Every aligned 32-bit big-endian word in every executable ELF section is classified by its encoding into the lowest MIPS ISA that has it. The classes reported (columns of the TSVs):

- MIPS IV only: `movz`/`movn`; integer `movf`/`movt` (the `movci` encoding); `movf.fmt`, `movt.fmt`, `movz.fmt`, `movn.fmt`; `recip` and `rsqrt`; `pref`; the indexed-opcode group (`prefx`, `lwxc1`, `ldxc1`, `swxc1`, `sdxc1`, `madd`/`msub`/`nmadd`/`nmsub` in `.s` and `.d`); a floating-point condition code other than 0 in `c.cond` or `bc1f`/`bc1t`.
- MIPS II: branch-likely (`beql`, `bnel`, `blezl`, `bgtzl`, `bltzl`, `bgezl`, `bltzall`, `bgezall`, `bc1fl`, `bc1tl`); `ll`/`sc`; `ldc1`/`sdc1`; `sqrt` and the word-result rounding conversions; `teq` and the other traps, `sync`.
- MIPS III: `lld`/`scd`; doubleword integer arithmetic, shifts, loads and stores (`ld`, `sd`, `daddu`, `dsll`, `ldl`, `lwu` and the rest); `dmfc1`/`dmtc1`; the long-integer conversions; `cache`.

The word `0x00000001` is `movf $0,$0,$fcc0` in the MIPS IV encoding. It fills the gaps between routines in linked images and is counted in its own column, `pad_00000001`, not as a use of `movf`. It appears in MIPS III-flagged files that contain no other MIPS IV encoding (for instance 23679 times across the n32 MIPS III files), so it is padding that is never meant to run. Inferred: padding, from where it sits (between routines) and that the files carrying it use no other MIPS IV encoding; its architectural effect (none; the destination is register 0) is established from the encoding.

A linear sweep cannot tell data inside a text section from code, so the scan also counts words that decode to nothing in MIPS I to IV (`undecoded`) and to MIPS32-only opcodes (`mips32_other`). Both are 0 for all 4490 n32 and n64 user-space files of 6.5.30 and for all six linked kernels, so the sweep found no stray data in those files. Only the o32 standalone diagnostic image `ide` (two copies in 6.5.30, one in 6.5.22) shows any; o32 is outside this question. Established.

### e_flags and the lowest ISA, user space

The ELF header `e_flags` top nibble is the architecture level. Both releases: n64 files have `e_flags` of `0x20000002` (MIPS III) or `0x30000002` (MIPS IV) for the libraries, and `0x20000000` for `rld`; the n32 libraries add the n32 ABI bit and the position-independent bit (`0x22`), so `0x20000022` (MIPS III) and `0x30000022` (MIPS IV); the n32 `rld` is `0x20000020`. Established.

The counts below are for the 6.5.30 set (the 6.5.22 set has the same pattern; its numbers are in `isa-6.5.22.tsv`). "Lowest ISA" is the highest class present in the file. Established by the scan.

| ABI | `e_flags` level | Files | Use a MIPS IV-only encoding | Lowest ISA III | Lowest ISA below III | Grade |
|---|---|---:|---:|---:|---:|---|
| n32 | MIPS III | 3187 | 0 | 3173 | 14 | established |
| n32 | MIPS IV | 403 | 214 | 189 | 0 | established |
| n64 | MIPS III | 721 | 0 | 716 | 5 | established |
| n64 | MIPS IV | 179 | 158 | 20 | 1 | established |

- No file flagged MIPS III contains a MIPS IV-only encoding. A file flagged MIPS IV may contain none: 189 n32 and 21 n64 MIPS IV-flagged files use no MIPS IV encoding and need only MIPS III. Of the 14 n32 and 5 n64 MIPS III files that reach no further than MIPS I or II, the sweep finds no doubleword instruction in them (examples: `libsocket.so`, `libCsup.so`, `pam_deny.so`, the `cmplrs` helper libraries, `libGLcore.so`). The other n32 MIPS III files all use doubleword instructions.

n32 therefore does not assume MIPS IV. 3187 of 3590 n32 files (89 percent) and 721 of 900 n64 files (80 percent) are flagged MIPS III and the scan finds no MIPS IV encoding in them. The lowest ISA for n32 and n64 user space is MIPS III: every n32 and n64 file in which the sweep found 64-bit instructions needs it, and nothing here needs less than that for n32 code that uses 64-bit registers. Established for the counts; the claim that n32 as an ABI requires MIPS III is inferred (it follows from n32 passing 64-bit values in 64-bit registers, which this scan confirms by `int64` counts in 3173 of 3187 MIPS III n32 files, but was not derived from a manual).

- The MIPS IV files are the ones SGI built in a second set. The core libraries ship twice:

| Library | Build | `e_flags` | MIPS IV encodings | Lowest ISA |
|---|---|---|---|---|
| libc n32 | `usr/lib32/mips3/libc.so.1` (the `6.5.30/lib32/libc.so.1` copy) | `0x20000022` | none; `ll`/`sc` 185, `lld`/`scd` 4, branch-likely 1915 | III |
| libc n32 | `6.5.30/usr/lib32/mips4/libc.so.1` (the `lib32/libc.so.1.CPUARCH-R8000-CPUARCH-R10000-CPUARCH-R5000` copy) | `0x30000022` | `movz`/`movn` 697, `pref` 44, FP conditional moves 46, condition code above 0 272, indexed FP load 8 | IV |
| libc n64 | `6.5.30/usr/lib64/mips3/libc.so.1` | `0x20000002` | none | III |
| libc n64 | `6.5.30/usr/lib64/mips4/libc.so.1` | `0x30000002` | `movz`/`movn` 680, `pref` 42, FP conditional moves 46, condition code above 0 248 | IV |
| libm n32 and n64 | `mips3/libm.so`, `mips4/libm.so`, and an R8000 build of the MIPS IV one | `0x2...` / `0x3...` | MIPS IV builds use `madd`/`msub` family 3261, FP conditional moves 1277, `pref` 327; MIPS III builds none | III / IV |
| libpthread n32 and n64 | single build | `0x20000022` / `0x20000002` | none | III |
| `rld` n32 and n64 | single build | `0x20000020` / `0x20000000` | none | III |

So the base libraries of an n32 or n64 system have a MIPS III build and a MIPS IV build, and the loader and the thread library have only a MIPS III build. Established. The two libc builds export the same names but two (see the libc description above). `prefx` (the indexed prefetch) is not used by any file in either release; `pref` is. Established.

Which build a machine gets: the media names the MIPS IV libc `lib32/libc.so.1.CPUARCH-R8000-CPUARCH-R10000-CPUARCH-R5000`, that is, the copy for machines whose CPU is an R8000, R10000 or R5000, and the unsuffixed MIPS III copy for the others (R4000-class). The run-time loader's built-in n32 search path is `/usr/lib32`, `/usr/lib32/internal`, `/lib32` and `/opt/lib32`; it names no `mips3` or `mips4` directory. So the choice is made at installation by CPU type, not by `rld` at run time. Inferred from the media's file names and the loader's path strings; the installer's own manifest was not read (open question).

Where the 403 + 179 MIPS IV-flagged programs sit: 190 are in the `mips4` library directories, the rest are in graphics libraries (`usr/gfx/arch/*/libGL*.so`), the Open Office and Netscape trees, security modules and a few programs under `usr/bin`. Established from the paths.

### e_flags and the lowest ISA, kernels

| Kernel | `e_flags` | Text words | `movz`/`movn` | `pref` | branch-likely | `ll`/`sc` | `lld`/`scd` | 64-bit integer | Lowest ISA |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| 6.5.30 `unix.IP35` | `0x30000000` | 1138498 | 2993 | 175 | 1659 | 1992 | 734 | 399341 | IV |
| 6.5.30 `unix.IP27` | `0x30000000` | 961244 | 2604 | 176 | 1427 | 1542 | 692 | 339185 | IV |
| 6.5.22 `unix.IP35` | `0x30000000` | 1053374 | 2712 | 168 | 1594 | 1878 | 768 | 375318 | IV |
| 6.5.22 `unix.IP27` | `0x30000000` | 961718 | 2542 | 158 | 1458 | 1524 | 770 | 345385 | IV |

(`unix.IP30` is in `kernel-isa.tsv`, with the same shape.) The kernels are n64 and flagged MIPS IV. None has FP conditional moves, indexed FP loads, `madd` or a condition code above 0. Of the floating-point classes counted, only `ldc1`/`sdc1` appear (96 on IP35, 128 on IP27). Established for the counted classes.

IP35, 6.5.30: of the 9961 sized function symbols, 985 contain `movz`/`movn` and 66 contain `pref`; 1133 `movz`/`movn` and 60 `pref` words sit outside any sized function symbol (assembly code without a size). Examples read with the disassembler: `hub_error_init` has `movn $t8,$a2,$t3` at 0xc000000000057b44 and `movn $ra,$t2,$t3` at 0xc000000000057b4c; `strioctl` has `pref 1,0x100($v0)` at 0xc00000000020f640; `pas_vfault` has `lld $t8,($s0)` at 0xc0000000002ff150, `scd $ra,($s0)` at 0xc0000000002ff158 and `beql` at 0xc0000000002ff15c (the lld/scd retry loop). Established.

IP27, 6.5.30: 909 functions with `movz`/`movn` and 70 with `pref`. `kdspioctl` has `movz $s0,$s2,$a1` at 0xc0000000003b4934 and `strioctl` has `pref 1,0x100($v0)` at 0xc000000000164ad4. Established.

The kernel objects agree. Every `IP27boot` and `IP35boot` object of both releases is flagged MIPS IV, 1062 and 1467 `movz`/`movn` words in the 6.5.30 IP27 and IP35 sets, and no object is flagged lower. Established. The lowest ISA of the linked IP35 kernel is MIPS IV by `e_flags` and by the encodings it contains. Whether any MIPS IV encoding sits only on a path guarded by a CPU-type test (so that a MIPS III CPU could run the kernel by skipping it) was not examined; `movz`/`movn` and `pref` appear in common routines such as `syssgi` (20 in IP35) and `strioctl`, which makes a guarded design unlikely, and the CPUs the IP35 and IP27 boards take are all MIPS IV parts, so no fallback is needed. Inferred.

### Summary

IRIX n32 user space does not assume MIPS IV: most n32 programs and the loader are flagged MIPS III and contain no MIPS IV-only encoding; the lowest ISA of n32 and n64 user space is MIPS III. The base libraries ship in a MIPS III and a MIPS IV build. The IP35 and IP27 kernels (both releases) are flagged MIPS IV and contain `movz`/`movn` and `pref` in common routines, so their lowest ISA is MIPS IV.

## Open questions

These are also in [open-questions.md](open-questions.md): the installer's CPU selection for the two libc builds, the binding between libc's thread stubs and libpthread, 6.5.22 patch contents, and the run-time guards in the kernel.
