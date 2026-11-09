# Cross-check of the syscall and syssgi tables against public lists

`retools/tools/syscall_table.py` and `retools/tools/syssgi_selectors.py` read the tables from the linked 6.5.30 kernels (IP27, IP30, IP35). Every row in [syscalls.tsv](inventory/syscalls.tsv) and [selectors.tsv](inventory/selectors.tsv) comes from the kernel image. The public lists below are used only to check numbering. A disagreement is a finding to explain, not a correction of the kernel's row.

Public sources (copies are not kept in this repository):

- Linux's old IRIX emulation, v2.6.12: the MIPS system-call entry code for IRIX binaries (system calls) and its IRIX syscall-compat code (the `case SGI_*` names).
- NetBSD's IRIX compatibility layer, netbsd-5 branch: https://github.com/NetBSD/src/blob/netbsd-5/sys/compat/irix/syscalls.master and https://github.com/NetBSD/src/blob/netbsd-5/sys/compat/irix/irix_syssgi.h
- qemu-irix: https://github.com/n64decomp/qemu-irix/blob/master/linux-user/irix/syscall_nr.h

## System calls

The kernel's native table (`sysent`, 237 entries, call numbers 1000 to 1236; the base is read from the dispatcher) is compared with each list. The 17 entries the kernel points at `nosys` are skipped, and entries a list does not have are not counted (Linux lacks 66, NetBSD 1, qemu-irix 12). The three kernels give identical results. The second table, `irix5_64_sysent`, is not compared.

| List | Names agree | Names differ | Argument counts agree | Argument counts differ |
|---|---:|---:|---:|---:|
| Linux (v2.6.12) | 147 | 7 | 134 | 20 |
| NetBSD (netbsd-5) | 202 | 17 | 119 | 12 |
| qemu-irix | 197 | 11 | not listed | not listed |

Names are compared after dropping the `sys_`, `irix_` and `svr4_sys_` prefixes and mapping `rexit` to `exit`, `exec` to `execv` and `syscall` to `sysindir`; a name that contains the other counts as agreeing. NetBSD gives an argument count only for entries it declares with arguments. Linux and qemu-irix number from 1000, NetBSD from 0. The Linux argument counts differ on 20 entries, NetBSD's on 12; no cause has been established for them.

### Disagreements

Identical for IP27, IP30 and IP35. For `nargs` rows, Kernel is the argument-count byte of the entry.

| List | Number | Field | Kernel | Public list |
|---|---:|---|---|---|
| linux | 1011 | nargs | 2 | 0 |
| linux | 1035 | nargs | 4 | 2 |
| linux | 1038 | nargs | 4 | 2 |
| linux | 1040 | nargs | 6 | 0 |
| linux | 1056 | nargs | 5 | 0 |
| linux | 1057 | nargs | 3 | 4 |
| linux | 1059 | nargs | 3 | 0 |
| linux | 1088 | nargs | 3 | 0 |
| linux | 1130 | nargs | 5 | 0 |
| linux | 1137 | nargs | 3 | 4 |
| linux | 1141 | nargs | 4 | 0 |
| linux | 1144 | name | BSDsetpgrp | vhangup |
| linux | 1144 | nargs | 2 | 0 |
| linux | 1145 | name | vhangup | fsync |
| linux | 1145 | nargs | 0 | 1 |
| linux | 1146 | name | fsync | fchdir |
| linux | 1147 | name | fchdir | getrlimit |
| linux | 1147 | nargs | 1 | 2 |
| linux | 1148 | name | getrlimit | setrlimit |
| linux | 1161 | nargs | 4 | 0 |
| linux | 1165 | nargs | 1 | 0 |
| linux | 1168 | nargs | 1 | 0 |
| linux | 1169 | nargs | 1 | 0 |
| linux | 1173 | name | sigsendsys | sigsendset |
| linux | 1179 | name | nopkg | priocntl |
| linux | 1185 | nargs | 8 | 0 |
| linux | 1186 | nargs | 8 | 0 |
| netbsd | 13 | nargs | 0 | 1 |
| netbsd | 22 | name | umount | System |
| netbsd | 39 | name | setpgrp | svr4_sys_pgrpsys |
| netbsd | 39 | nargs | 1 | 3 |
| netbsd | 57 | nargs | 3 | 4 |
| netbsd | 59 | name | exece | svr4_sys_execve |
| netbsd | 64 | name | nopkg | reserved |
| netbsd | 65 | name | nopkg | reserved |
| netbsd | 66 | name | nopkg | reserved |
| netbsd | 67 | name | nopkg | reserved |
| netbsd | 68 | name | nopkg | reserved |
| netbsd | 69 | name | nopkg | reserved |
| netbsd | 70 | name | nopkg | advfs |
| netbsd | 71 | name | nopkg | unadvfs |
| netbsd | 72 | name | nopkg | rmount |
| netbsd | 73 | name | nopkg | rumount |
| netbsd | 78 | nargs | 5 | 7 |
| netbsd | 130 | nargs | 5 | 2 |
| netbsd | 143 | nargs | 1 | 0 |
| netbsd | 163 | nargs | 1 | 2 |
| netbsd | 173 | name | sigsendsys | sigsendset |
| netbsd | 179 | name | nopkg | priocntl |
| netbsd | 183 | nargs | 4 | 3 |
| netbsd | 184 | nargs | 4 | 3 |
| netbsd | 185 | nargs | 8 | 7 |
| netbsd | 187 | nargs | 6 | 4 |
| netbsd | 188 | nargs | 6 | 4 |
| netbsd | 216 | name | timer_getoverrun | timer_setoverrun |
| netbsd | 232 | name | xpg4_select | xpg4_sigaltstack |
| qemu-irix | 17 | name | sbreak | brk |
| qemu-irix | 39 | name | setpgrp | pgrpsys |
| qemu-irix | 59 | name | exece | execve |
| qemu-irix | 119 | name | nfs_svc | nfssvc |
| qemu-irix | 143 | name | BSDgetpgrp | getpgid |
| qemu-irix | 144 | name | BSDsetpgrp | setpgid |
| qemu-irix | 156 | name | systeminfo | sysinfosgi |
| qemu-irix | 170 | name | waitsys_wrapper | waitid |
| qemu-irix | 173 | name | sigsendsys | sigsendset |
| qemu-irix | 179 | name | nopkg | priocntl |
| qemu-irix | 180 | name | sigqueue_sys | ksigqueue |

Many name disagreements are aliases (`rexit` and `exit`, `exece` and `execve`, `setpgrp` and `pgrpsys`). Entries the kernel names `nopkg` are reserved or removed calls in NetBSD's list. A few look like renamed or shifted entries (`sigsendsys` and `sigsendset`, `timer_getoverrun` and `timer_setoverrun`). These readings are not checked against the handlers' code.

## syssgi selectors

The kernel's selectors are compared with the `IRIX_SGI_*` values of NetBSD's the IRIX tree header 3 (253 distinct values) and with the 18 `case SGI_*` names of the Linux emulation (15 of which NetBSD gives a value). "Has a case" means the selector reaches code of its own, not a path shared with other selectors. A public selector without a case may be a default or a range the extractor could not narrow, because selectors that never narrow to one value are not listed.

| Kernel | Selector rows | Public values with a case | Public values without | Kernel cases with no public constant | Linux names with a case (of 15) |
|---|---:|---:|---:|---:|---:|
| IP35 (compare tree) | 257 | 192 | 61 | 27 | 13 |
| IP27 (jump table) | 1261 | 162 | 91 | 24 | 11 |
| IP30 (jump table) | 1261 | 158 | 95 | 24 | 11 |

The IP35 tree reaches selector 8194 (`0x2002`); the IP27 and IP30 jump tables stop at 1260 and NetBSD's header has no value there. On the selectors where IP35 and IP27 both have a case of their own, 78 of the 82 that name a first call name the same one (IP30: 74 of 78). `first_call` is a heuristic, see the [inventory README](inventory/README.md).

### Disagreements

#### 6.5.30/unix.IP27

Public selectors with no case in the kernel (91): 1 SYSID, 3 TUNE_SET, 8 SETNVRAM, 9 GETNVRAM, 10 SETKOPT, 30 READB, 31 WRITEB, 64 GETPGID, 65 GETSID, 77 LOADATTR, 78 UNLOADATTR, 79 RECVLUMSG, 81 GETPSOACL, 82 SETPSOACL, 98 SBE_GET_INFO, 99 SBE_CLR_INFO, 102 GET_EVCONF, 115 XLV_GET_TAB, 118 FP_IMPRECISE_SUPP, 119 CONFIG_NSMM_SUPP, 122 RT_TSTAMP_CREATE, 123 RT_TSTAMP_DELETE, 124 RT_TSTAMP_START, 125 RT_TSTAMP_STOP, 126 RT_TSTAMP_ADDR, 127 RT_TSTAMP_MASK, 128 RT_TSTAMP_EOB_MODE, 141 OLD_SOFTFP, 142 FS_INUMBERS, 143 FS_BULKSTAT, 144 RT_TSTAMP_WAIT, 145 RT_TSTAMP_UPDATE, 151 READ_DANGID, 154 SETASH, 155 GETASH, 156 SETPRID, 157 GETPRID, 158 SETSPINFO, 159 GETSPINFO, 161 NEWARRAYSESS, 162 GETDFLTPRID, 167 PIDSINASH, 175 GETPRUSAGE, 179 GETGRPPID, 180 GETSESPID, 181 ENUMASHS, 182 SETASMACHID, 183 GETASMACHID, 184 GETARSESS, 185 JOINARRAYSESS, 190 SWASH_INIT, 192 MODULE_INFO, 193 GET_CONTEXT_NAME, 198 BRICK_INFO, 200 NUMA_MIGR_PAGE, 201 NUMA_MIGR_PAGE_ALT, 210 SETVPID, 211 GETVPID, 219 DPIPE_FSPE_BIND, 221 TUNE_GET, 230 WRITE_IP32_FLASH, 320 POKE, 321 PEEK, 350 XLV_INDUCE_IO_ERROR, 351 XLV_UNINDUCE_IO_ERROR, 352 DKSC_INDUCE_IO_ERROR, 353 DKSC_UNINDUCE_IO_ERROR, 360 XFS_INJECT_ERROR, 361 XFS_CLEAR_ERROR, 362 XFS_CLEARALL_ERROR, 401 FO_SWITCH, 407 FO_TRESSPASS, 1030 DEBUGLPAGE, 1031 MAPLPAGE, 1040 MUTEX_TEST, 1041 MUTEX_TEST_INIT, 1042 MUTEX_TESTER_INIT, 1043 CREATE_MISER_POOL, 1044 CREATE_MISER_JOB, 1045 MISER_CRITICAL, 1051 KMEM_TEST, 1052 SHAKE_ZONES, 1100 NUMA_MIGR_INT_VADDR, 1101 NUMA_MIGR_INT_PFN, 1102 NUMA_PAGEMIGR_TEST, 1104 NUMA_RESERVED, 1105 MEMPROF_START, 1106 MEMPROF_GET, 1107 MEMPROF_CLEARALL, 1108 MEMPROF_STOP, 1250 IP30MISC

Kernel cases with no public constant (24): 93, 236, 250, 365, 366, 409, 410, 411, 501, 650, 1062, 1063, 1064, 1065, 1066, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1260

Linux-implemented names with no case (4): 1 SYSID, 8 SETNVRAM, 9 GETNVRAM, 65 GETSID

#### 6.5.30/unix.IP30

Public selectors with no case in the kernel (95): 1 SYSID, 3 TUNE_SET, 8 SETNVRAM, 9 GETNVRAM, 10 SETKOPT, 30 READB, 31 WRITEB, 64 GETPGID, 65 GETSID, 77 LOADATTR, 78 UNLOADATTR, 79 RECVLUMSG, 81 GETPSOACL, 82 SETPSOACL, 98 SBE_GET_INFO, 99 SBE_CLR_INFO, 102 GET_EVCONF, 115 XLV_GET_TAB, 118 FP_IMPRECISE_SUPP, 119 CONFIG_NSMM_SUPP, 122 RT_TSTAMP_CREATE, 123 RT_TSTAMP_DELETE, 124 RT_TSTAMP_START, 125 RT_TSTAMP_STOP, 126 RT_TSTAMP_ADDR, 127 RT_TSTAMP_MASK, 128 RT_TSTAMP_EOB_MODE, 141 OLD_SOFTFP, 142 FS_INUMBERS, 143 FS_BULKSTAT, 144 RT_TSTAMP_WAIT, 145 RT_TSTAMP_UPDATE, 151 READ_DANGID, 154 SETASH, 155 GETASH, 156 SETPRID, 157 GETPRID, 158 SETSPINFO, 159 GETSPINFO, 161 NEWARRAYSESS, 162 GETDFLTPRID, 167 PIDSINASH, 175 GETPRUSAGE, 179 GETGRPPID, 180 GETSESPID, 181 ENUMASHS, 182 SETASMACHID, 183 GETASMACHID, 184 GETARSESS, 185 JOINARRAYSESS, 190 SWASH_INIT, 192 MODULE_INFO, 193 GET_CONTEXT_NAME, 195 PART_OPERATIONS, 198 BRICK_INFO, 200 NUMA_MIGR_PAGE, 201 NUMA_MIGR_PAGE_ALT, 210 SETVPID, 211 GETVPID, 212 NUMA_TUNE, 218 NUMA_STATS_GET, 219 DPIPE_FSPE_BIND, 221 TUNE_GET, 230 WRITE_IP32_FLASH, 231 ROUTERSTATS_ENABLED, 320 POKE, 321 PEEK, 350 XLV_INDUCE_IO_ERROR, 351 XLV_UNINDUCE_IO_ERROR, 352 DKSC_INDUCE_IO_ERROR, 353 DKSC_UNINDUCE_IO_ERROR, 360 XFS_INJECT_ERROR, 361 XFS_CLEAR_ERROR, 362 XFS_CLEARALL_ERROR, 401 FO_SWITCH, 407 FO_TRESSPASS, 1030 DEBUGLPAGE, 1031 MAPLPAGE, 1040 MUTEX_TEST, 1041 MUTEX_TEST_INIT, 1042 MUTEX_TESTER_INIT, 1043 CREATE_MISER_POOL, 1044 CREATE_MISER_JOB, 1045 MISER_CRITICAL, 1051 KMEM_TEST, 1052 SHAKE_ZONES, 1100 NUMA_MIGR_INT_VADDR, 1101 NUMA_MIGR_INT_PFN, 1102 NUMA_PAGEMIGR_TEST, 1103 NUMA_TESTS, 1104 NUMA_RESERVED, 1105 MEMPROF_START, 1106 MEMPROF_GET, 1107 MEMPROF_CLEARALL, 1108 MEMPROF_STOP

Kernel cases with no public constant (24): 93, 236, 250, 365, 366, 409, 410, 411, 501, 650, 1062, 1063, 1064, 1065, 1066, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1260

Linux-implemented names with no case (4): 1 SYSID, 8 SETNVRAM, 9 GETNVRAM, 65 GETSID

#### 6.5.30/unix.IP35

Public selectors with no case in the kernel (61): 1 SYSID, 8 SETNVRAM, 10 SETKOPT, 30 READB, 31 WRITEB, 77 LOADATTR, 78 UNLOADATTR, 98 SBE_GET_INFO, 99 SBE_CLR_INFO, 102 GET_EVCONF, 115 XLV_GET_TAB, 123 RT_TSTAMP_DELETE, 125 RT_TSTAMP_STOP, 127 RT_TSTAMP_MASK, 141 OLD_SOFTFP, 142 FS_INUMBERS, 143 FS_BULKSTAT, 145 RT_TSTAMP_UPDATE, 151 READ_DANGID, 155 GETASH, 157 GETPRID, 159 GETSPINFO, 161 NEWARRAYSESS, 167 PIDSINASH, 175 GETPRUSAGE, 179 GETGRPPID, 181 ENUMASHS, 183 GETASMACHID, 185 JOINARRAYSESS, 189 SYNCH_CACHE_HANDLER, 190 SWASH_INIT, 192 MODULE_INFO, 198 BRICK_INFO, 200 NUMA_MIGR_PAGE, 201 NUMA_MIGR_PAGE_ALT, 219 DPIPE_FSPE_BIND, 230 WRITE_IP32_FLASH, 320 POKE, 321 PEEK, 350 XLV_INDUCE_IO_ERROR, 352 DKSC_INDUCE_IO_ERROR, 353 DKSC_UNINDUCE_IO_ERROR, 1030 DEBUGLPAGE, 1031 MAPLPAGE, 1040 MUTEX_TEST, 1041 MUTEX_TEST_INIT, 1042 MUTEX_TESTER_INIT, 1043 CREATE_MISER_POOL, 1044 CREATE_MISER_JOB, 1045 MISER_CRITICAL, 1051 KMEM_TEST, 1052 SHAKE_ZONES, 1100 NUMA_MIGR_INT_VADDR, 1101 NUMA_MIGR_INT_PFN, 1102 NUMA_PAGEMIGR_TEST, 1104 NUMA_RESERVED, 1105 MEMPROF_START, 1106 MEMPROF_GET, 1107 MEMPROF_CLEARALL, 1108 MEMPROF_STOP, 1250 IP30MISC

Kernel cases with no public constant (27): 93, 196, 199, 236, 250, 365, 366, 409, 410, 411, 501, 650, 1062, 1063, 1064, 1065, 1066, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1260, 8194

Linux-implemented names with no case (2): 1 SYSID, 8 SETNVRAM

## Regenerating

Put copies of the public files in one directory under the names `linux-irix5sys.S`, `netbsd-irix-syscalls.master`, the IRIX tree header 5, the IRIX tree header 4 and the IRIX tree source unit 1, then run `retools/tools/re-run --syscalls root/6.5.30/unix.IP35 --cross-check DIR --report FILE` and the same with `--syssgi`.
