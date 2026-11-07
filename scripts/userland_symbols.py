#!/usr/bin/env python3
"""Exported dynamic symbols of the 6.5.30 core user libraries, as TSV tables.

Usage: uv run --with pyelftools python -I scripts/userland_symbols.py MEDIA_ROOT OUT_DIR

MEDIA_ROOT is a directory that mirrors the 6.5.30 install paths (usr/lib32/mips3/libc.so.1 and so
on). For each library and ABI it writes OUT_DIR/<lib>-<abi>.tsv with one row per exported symbol
(a defined, non-local entry of .dynsym): name, ELF type, binding, then one size column per CPU
variant of the library ("-" where that variant does not export the name). It also writes
OUT_DIR/summary.tsv (counts per file) and OUT_DIR/probe.tsv (which of a fixed list of
thread, event-wait, memory-map, signal, socket and dynamic-loader names each library exports).
"""
import csv
import os
import sys

from elftools.elf.elffile import ELFFile

# (library, abi, [(variant label, install path)])
LIBS = [
    ("libc", "n32", [("mips3", "usr/lib32/mips3/libc.so.1"), ("mips4", "usr/lib32/mips4/libc.so.1")]),
    ("libc", "n64", [("mips3", "usr/lib64/mips3/libc.so.1"), ("mips4", "usr/lib64/mips4/libc.so.1")]),
    ("libpthread", "n32", [("only", "usr/lib32/libpthread.so")]),
    ("libpthread", "n64", [("only", "usr/lib64/libpthread.so")]),
    ("libm", "n32", [("mips3", "usr/lib32/mips3/libm.so"), ("mips4", "usr/lib32/mips4/libm.so"),
                     ("mips4-r8000", "usr/lib32/mips4/libm.so.CPUARCH-R8000")]),
    ("libm", "n64", [("mips3", "usr/lib64/mips3/libm.so"), ("mips4", "usr/lib64/mips4/libm.so"),
                     ("mips4-r8000", "usr/lib64/mips4/libm.so.CPUARCH-R8000")]),
    ("libdl-abi", "n64", [("only", "usr/lib64/abi/libdl.so")]),
    ("libc-abi", "n64", [("only", "usr/lib64/abi/libc.so")]),
]

PROBES = {
    "thread": """pthread_create pthread_join pthread_detach pthread_exit pthread_self pthread_equal pthread_once
        pthread_cancel pthread_testcancel pthread_setcancelstate pthread_setcanceltype pthread_cleanup_push
        pthread_cleanup_pop pthread_attr_init pthread_attr_destroy pthread_attr_setstacksize pthread_attr_setdetachstate
        pthread_attr_setscope pthread_mutex_init pthread_mutex_destroy pthread_mutex_lock pthread_mutex_trylock
        pthread_mutex_unlock pthread_mutex_timedlock pthread_mutexattr_init pthread_mutexattr_settype
        pthread_mutexattr_setpshared pthread_mutexattr_setprotocol pthread_cond_init pthread_cond_destroy
        pthread_cond_wait pthread_cond_timedwait pthread_cond_signal pthread_cond_broadcast pthread_condattr_init
        pthread_rwlock_init pthread_rwlock_rdlock pthread_rwlock_wrlock pthread_rwlock_unlock pthread_barrier_init
        pthread_barrier_wait pthread_spin_init pthread_spin_lock pthread_key_create pthread_key_delete
        pthread_getspecific pthread_setspecific pthread_sigmask pthread_kill pthread_atfork pthread_setschedparam
        pthread_getschedparam sem_init sem_wait sem_post sem_open sem_trywait sem_destroy sproc sprocsp
        pthread_setrunon_np""".split(),
    "event-wait": """poll select pselect epoll_create epoll_ctl epoll_wait kqueue kevent port_create
        devpoll sigtimedwait ppoll""".split(),
    "mmap": """mmap mmap64 munmap mprotect msync mlock munlock mlockall munlockall madvise mincore mremap
        shm_open shm_unlink shmget shmat shmdt shmctl usinit usnewsema""".split(),
    "signal": """signal sigaction sigprocmask sigsuspend sigpending sigwait sigwaitinfo sigtimedwait sigqueue
        sigaltstack sigsend sigsendset sigemptyset sigfillset sigaddset sigdelset sigismember sigset sighold
        sigrelse sigignore sigpause kill raise killpg sigstack siglongjmp sigsetjmp sigvec sigblock sigsetmask
        pthread_sigmask pthread_kill""".split(),
    "socket": """socket socketpair bind listen accept connect send sendto sendmsg recv recvfrom recvmsg
        shutdown getsockname getpeername getsockopt setsockopt gethostbyname getaddrinfo getnameinfo
        freeaddrinfo gai_strerror inet_pton inet_ntop inet_ntoa inet_addr if_nametoindex if_indextoname
        getifaddrs accept4 sendfile""".split(),
    "dynamic-loader": """dlopen dlsym dlclose dlerror dladdr sgidladd sgidlopen_version dlinfo
        __dlopen _dlopen _dlsym""".split(),
}


def exports(path):
    f = ELFFile(open(path, "rb"))
    out = {}
    for s in f.get_section_by_name(".dynsym").iter_symbols():
        if not s.name or s["st_info"]["bind"] == "STB_LOCAL" or s["st_shndx"] == "SHN_UNDEF":
            continue
        out[s.name] = (s["st_info"]["type"].replace("STT_", "").lower(),
                       s["st_info"]["bind"].replace("STB_", "").lower(), s["st_size"])
    return out


def main():
    root, outdir = sys.argv[1:3]
    os.makedirs(outdir, exist_ok=True)
    summary = [("library", "abi", "variant", "path", "exported", "func", "object", "other", "weak", "global")]
    tables = {}
    for lib, abi, variants in LIBS:
        cols = []
        for label, rel in variants:
            cols.append((label, rel, exports(os.path.join(root, rel))))
        names = sorted(set().union(*[set(c[2]) for c in cols]))
        tables[(lib, abi)] = {n: [c[2].get(n) for c in cols] for n in names}
        with open(os.path.join(outdir, f"{lib}-{abi}.tsv"), "w", newline="") as fh:
            w = csv.writer(fh, delimiter="\t", lineterminator="\n")
            w.writerow(["name", "type", "bind"] + [f"size_{c[0]}" for c in cols])
            for n in names:
                first = next(v for v in tables[(lib, abi)][n] if v)
                w.writerow([n, first[0], first[1]] + [v[2] if v else "-" for v in tables[(lib, abi)][n]])
        for label, rel, ex in cols:
            t = [v[0] for v in ex.values()]
            summary.append((lib, abi, label, rel, len(ex), t.count("func"), t.count("object"),
                            len(t) - t.count("func") - t.count("object"),
                            sum(1 for v in ex.values() if v[1] == "weak"),
                            sum(1 for v in ex.values() if v[1] == "global")))
    with open(os.path.join(outdir, "summary.tsv"), "w", newline="") as fh:
        csv.writer(fh, delimiter="\t", lineterminator="\n").writerows(summary)
    # probe: name by library and ABI (mips3 or only variant)
    probe_libs = [("libc", "n32"), ("libc", "n64"), ("libpthread", "n32"), ("libpthread", "n64"),
                  ("libm", "n32"), ("libm", "n64"), ("libdl-abi", "n64")]
    with open(os.path.join(outdir, "probe.tsv"), "w", newline="") as fh:
        w = csv.writer(fh, delimiter="\t", lineterminator="\n")
        w.writerow(["family", "name"] + [f"{l}-{a}" for l, a in probe_libs])
        for fam, names in PROBES.items():
            for n in names:
                row = []
                for k in probe_libs:
                    v = tables[k].get(n)
                    if not v:
                        row.append("-")
                    else:
                        x = next(e for e in v if e)
                        row.append(f"{x[0]}/{x[1]}/{x[2]}")
                w.writerow([fam, n] + row)


if __name__ == "__main__":
    main()
