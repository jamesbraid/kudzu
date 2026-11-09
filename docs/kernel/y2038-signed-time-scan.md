# Year-2038 signed 32-bit time scan across the shipped IRIX 6.5.30 binaries

Method: binary RE only of the shipped ELF objects under `refs/irix-media/6.5.30`. No IRIX source
tree or header was opened. Symbol names come from each binary's own symbol table. Disassembly is
capstone (mips64, big-endian). Nothing was run on a guest; every row here is static.

This turns boot-by-boot discovery of signed 32-bit time bugs into a patch-and-test checklist. It is
a companion to the hand-done readings in [2106-core-time.md](2106-core-time.md),
[boot-time-nsd-getpwuid.md](boot-time-nsd-getpwuid.md) and [xfs-timestamps.md](xfs-timestamps.md).
The full machine-readable hit list is [`y2038-signed-time-hits.tsv`](y2038-signed-time-hits.tsv) in
this directory (1264 userland + 11 kernel rows).

## What it looks for

A 32-bit time value — from `time()`, `gettimeofday`/`clock_gettime` `tv_sec`, `stat` `st_mtime`/
`st_atime`/`st_ctime`, utmp stamps, or shadow/ageing day fields — that then reaches a **signed**
`slt`/`slti`, `bltz`/`bgez`/`bgtz`/`blez`, or `div`/`ddiv`. Each such site breaks when the value goes
negative after 2038-01-19 03:14:07 (INT32_MAX seconds). On N32, O32 and even N64 userland here,
`time_t` is a 32-bit load (`lw`): N64 `pam_unix` still signed-divides the 32-bit `time()` return. The
only 64-bit time loads seen were a `gettimeofday` buffer in `pmcd` and kernel `clock.o`, not flagged.

## How the scan works, and where it is blind

1. Enumerate every MIPS ELF in the media (5898 files); keep the 2068 whose dynamic symbol table
   imports or exports a time source. A raw word scan finds `lw/ld $t9, off($gp)` GOT loads resolving
   to one of those names; the enclosing function prologue defines a candidate window (7743 windows).
2. Decode each window with capstone and run a linear taint pass (with delay-slot handling): the
   return of `time()`, the `tv_sec` word of a `gettimeofday`/`clock_gettime` buffer, and the
   `st_*time` words of a `stat` buffer are tainted and propagated through moves, adds, shifts and
   stack spills; a hit is a signed `slt/slti`, `bltz`-family branch or `div` on a tainted value.
3. A raw scan of all 5898 files for `lui 1; ori 0x5180` (86400) next to a signed `div`, to catch
   date arithmetic with no visible time call (333 sites in 152 files).
4. The same taint pass over 79 kernel functions in the `usr/cpu/sysgen/IP35boot` relocatable objects
   that call `microtime`/`nanotime` (call targets from relocation records).

Blind spots, worst first:

- **Heuristic, not data-flow.** Linear walk, no control-flow graph. A compare of a *stored* time
  (a cache deadline reloaded from a global or struct field) against `time()` only shows up when the
  `time()` read is in the same function. Expect misses and some false hits.
- **Confidence rubric, applied mechanically.** `high` = a time value signed-divided by
  86400/3600/60/604800 (the shape of the fixed login-ageing exemplars). `medium` = a time value in a
  signed compare or branch. `low` = `delta` (post-subtract; a signed compare of a difference is
  mostly benign across the wrap), sub-second divides (by 1000/1e6), multiplies by a time constant,
  and const-only divides with no visible source. "read" = the surrounding disassembly was read and
  the chain is really there; everything else is scanner output.
- **`struct stat` offsets inferred** from load-frequency, not a header (n32 well supported, mtime at
  0x4c; o32 and n64 stat taint weak).
- **Inlined time math** is covered only for the 86400-divide shape; compiler reciprocal-multiply
  divides and kernel reads of the `time`/`hrestime` globals are not detected. Kernel coverage is only
  the 79 `microtime`/`nanotime` callers; XFS/EFS/NFS inode time compares and `lbolt` users were not
  scanned. There is no combined `unix` image in the media, only `.o`/`.a`, so kernel addresses are
  offsets in each object's `.text`.
- **Duplicates.** The same code ships as o32, n32 (`/lib32`, `mips3`, `mips4`, `CPUARCH` variants),
  n64 and hashed copies; the table names one representative and its siblings. The TSV carries every
  distinct site, deduplicated by basename/abi/address.
- **Stripped userland.** Almost all binaries have only `.dynsym`; "function" is the nearest preceding
  dynamic symbol (often `main`) — treat the address as the identifier.
- `nsd` is covered separately ([boot-time-nsd-getpwuid.md](boot-time-nsd-getpwuid.md)); its hits are
  in the TSV but not ranked here. There is no `/usr/bin/login` in this media tree, so login ageing is
  represented by `su`, `iaf/scheme` and the PAM modules.

## Already fixed, kept as exemplars

| Exemplar | Pattern |
|---|---|
| kernel `inittodr` (`kernel.o`) | wall-clock vs saved time signed compare |
| kernel `nfs_getattr_cache` (`nfs3.a:nfs_client.o`) | attribute-cache age: signed compare of `tv_sec` against cached expiry |
| `libpthread` timeout-queue insert (n32/n64/o32) | signed compare of two timespec seconds to order the queue |
| `libpthread` timespec-subtract helper (n32/n64/o32) | signed borrow/compare of seconds |
| `/usr/lib/iaf/scheme` login ageing | `time()` signed-divided by 86400 then `slt` against lstchg + max; the scan reproduces this shape at three sites (n32 `0x10009c4c`, `0x100052f4`, `0x100089dc`) |

The remaining rows reproduce the last exemplar's shape (`div <time>, 0x15180; mflo; slt ... lstchg/max/expire`) or the first four (a stored time compared against `time()`/`tv_sec`).

## Ranked checklist (boot/login first)

"Read" = the disassembly around the address was read and the chain is really there. "Scan" = scanner
output only. Addresses are virtual addresses in the named binary (kernel: offset in the object's `.text`).

| # | Binary | Function | Address | Pattern | Confidence | Boot/login |
|---|---|---|---|---|---|---|
| 1 | `/usr/lib32/security/pam_unix.so` (also n64 `/usr/lib64/security/pam_unix.so`, o32 `/usr/dt/lib/security/pam_unix.so.1`) | `pam_sm_acct_mgmt` | n32 `0x402a14`, `0x403e2c`; n64 `0x40316c`, `0x40461c`; o32 ~`0x5ffe2278` | `time()` -> signed `div` by 86400 -> `slt` against the account record's change/max/expire day fields; a negative day count makes every account look not-yet-expired or never-changed | high: read (n32 `0x402a14`), same shape as the scheme exemplar | login (every PAM account check: sshd, ftpd, rsh, dtlogin) |
| 2 | `/sbin/su` | `main` window | `0x100047e4`, `0x1000486c` | `time()` to stack, signed `div` by 0x15180, `slt` vs `ia_get_logchg`/`logmax`/`logmin`, `bltz` on logmax | high: read | login (su; `sulogin` shares libia) |
| 3 | `/usr/lib/iaf/scheme` | three local sites `0x10009c4c` (fixed), `0x100052f4`, `0x100089dc`/`0x100089cc` | as listed | THREE signed `div` by 86400 in one binary; only `0x10009c4c` was patched. `0x100052f4` and `0x100089dc`/`0x100089cc` sit behind a `limit > 0` test, so they bite only for accounts with ageing set | high: read; live-confirmed which site was patched | login |
| 4 | `/usr/etc/ftpd` | `user` | `0x10005f8c` | `time()` / 86400 to global `now`, `slt ia_expire, now`, `slt now, ia_lstchg` | high: read | login (ftp) |
| 5 | `/usr/etc/rshd` | `main` window | `0x7fc03a68` | `time()` / 86400 `slt` against a stored day value (ageing field) | high: read | login (rsh) |
| 6 | `/usr/bin/passwd` | `main` window `0x10002650` | 11 divs from `0x10002d00`; 604800 divs `0x10005828`; `mult` days*86400 at `0x100046d4` -> `gmtime` | signed `div` of `time()` into days for ageing; separately lstchg-days * 86400 overflows 32 bits at day 24855 (2038-01-19) before `gmtime` | high for the divs (scan); medium for the `mult` overflow (read) | login (password change) |
| 7 | `/usr/etc/rpc.passwd`; `/usr/lib32/security/pam_ldap.so` | `main`; `pam_sm_authenticate` | rpc.passwd `0x10003afc`; pam_ldap `0x4063cc`, `0x406874` | same `time()`/86400 ageing divide | high: scan | login |
| 8 | `/lib32/libc.so.1` (o32/n64/mips3/mips4/CPUARCH copies) | gmtime core (local fn `0xfa55cfc`) | `0xfa55d08` (n32) | `lw` time_t, signed `div` by 86400 with negative fix-up: a time_t >= 2^31 yields dates in 1901. `localtime_r` also `slt`s the clock against DST transition times (`0xfa56084`, inferred); TZ code multiplies days by 86400 | high: gmtime core read; localtime_r/mktime inferred | boot (every process that formats a date: syslog, cron, fsck, sendmail, shell prompts) |
| 9 | `/usr/etc/cron` | `readcron` window | `0x7fc07070`, `0x7fc07138` | `time()` `slt v0, s1` against the next-run/last time; a negative `time()` is always "earlier", so jobs never come due or all fire | medium: read (compare present; origin of `s1` not traced) | boot (rc), periodic |
| 10 | `/usr/etc/syslogd` | `filtermsg` window | `0x7fc05114` | `time()` `slt` against a global repeat-suppression deadline that is cleared after expiry: suppression never expires | medium: read | boot |
| 11 | kernel `sgi.a:fs_bio.o` | `bdflush_wake_proc` | `0x3ecc` (and `0x3eec`) | `nanotime(&ts)`; `lw ts.tv_sec` signed `slt` against saved 32-bit last-wake seconds | medium-high: read | boot (buffer-flush daemon from early boot) |
| 12 | kernel `xvm.a:xvm_garbage.o` | `xvm_ve_collect` | `0x5b0` | `nanotime`; `lw tv_sec` `slt` against `ve->time + 60` (60 s garbage collect) | medium: read | boot (XVM volumes) |
| 13 | `/lib32/libc.so.1` (+ copies) | `_yp_dobind` | `0xfb0a28c`, `0xfb0a2b4` | `time()` `slt` against a NIS bind-retry deadline and `time()+25` | medium: read | boot/login when NIS is configured |
| 14 | `/usr/etc/portmap`, `/usr/etc/rpcbind` | `pmap_service` window | portmap `0x10005a60`, rpcbind `0x10007e14` | `gettimeofday` `tv_sec` `slt` against (global last + 0x3c): 60-second cache; and `time()` vs fd-age | medium: read (`0x10005a60`) | boot |
| 15 | `/usr/etc/timed`; `/usr/etc/timeslave`; `/usr/sbin/ntpd` | timed `slave`/`measure`; timeslave `secdiff`; ntpd date | timed `0x10003ca8`, `0x10002cd8` div 86400; timeslave `0x100025c8`; ntpd `0x1001f9a8` div 86400 | `BSDgettimeofday` seconds vs each other and slave deadlines; day splits for logging | medium (timed read); ntpd/timeslave divs high by rubric, not read | boot when time sync is enabled |
| 16 | `/usr/etc/lmgrd` (FlexLM; also `fs2d`, `ggd`, `ggd2`, `mam`, `lmutil`) | `lc_expire_days`, `l_ckout_borrow`, `l_read_borrow`, `l_borrow_stat` | div `0x1003aa3c` (lmgrd); slt `0x10029038`, `0x1003bafc`, `0x100410b4` | license expiry/borrow compares against `time()` and days-to-expiry via 86400 divide; "permanent" licenses encode 0x7fffffff-class dates | medium: scan | boot (`S45flexlm`), license check in licensed tools |
| 17 | `/usr/etc/ipfilterd` | `cache` | `0x100086dc` (and `0x100086b4` `bltz`) | `fstat` mtime word + interval `slt` against `time()`; `bltz $v0` right after `time()` treats a negative time as failure | medium: read | boot (ipfilterd) |
| 18 | `/usr/etc/autofsd` | `pingnfs` | `0x1000f4c0`, `0x1000f540` | stacked `tv_sec` `slt` against a cached server-ping time | medium: read (first) | boot (autofs) |
| 19 | `/usr/etc/mediad`, `fam`, `miser`, `arrayd`, `eventmond`, `proclaim`, `route6d`, `snmpd`, `qpage.d` | scheduler/timer fns | see the TSV (mediad `0x1001a358`, fam `0x10014654`, arrayd `0x10006d24`, proclaim `0x10003e3c`) | `gettimeofday`/`time()` seconds vs stored deadlines or fd timestamps; a negative `tv_sec` makes every deadline already past or never reached (event-loop stall) | medium: scan | boot (rc services, many chkconfig-gated) |
| 20 | `/usr/lib/sendmail`, `mail.local`, `mail`, `/usr/sbin/named`, `host` | timers, `named` window | sendmail `0x10017470`; mail.local `0x100061e4`; named `0x1011d048`, `0x1011d184` (`bltz` on tv_sec) | stat mtime or tv_sec vs stored times; named `bltz` on a tv_sec-derived value | medium: scan | boot (MTA, DNS) |
| 21 | kernel `devpoll.o` | `devpoll_scan` | `0x1c08` | `nanotime` tv_sec `slt` against deadline, affects `poll()`-on-`/dev/poll` timeouts | medium: scan | boot (if a daemon uses /dev/poll) |
| 22 | kernel `os.a:syssgi.o`; `os.a:clock.o` | `syssgi`; `second_thread` | `syssgi 0x774`; `clock.o 0x384` | `lw tv_sec` `slt` (syssgi); `ld tv_sec` `blez` (clock.o is a 64-bit load, probably fine) | low: scan | rare / boot |
| 23 | `/sbin/xlv_assemble`, `/sbin/xvm`, `xvminit`, `xlv_mgr` | date formatting | `xlv_assemble 0x1003c0ec`; xvm `0x100a6d5c` | signed divide by 86400 of a volume timestamp for display; no time source in the window | low (const-only) | boot (volume assembly), display only |
| 24 | `/lib/rld`, `/lib32/rld`, `/lib64/rld` | private time-format helper | o32 `0xfb8405c`, n32 `0xfb7dfd4`, n64 `0xfb8000c` | a copy of the gmtime-style `div` by 86400 inside the runtime linker | low-medium: read (n32) | runs in every dynamic exec, but only if this helper is used (diagnostics) |
| 25 | `/sbin/csh`, `find`, `pwconv`, `passmgmt`, `zdump`/`zic`, `acctcom`, `/usr/lib/acct/*` | date math in tools | csh `0xe0128f8`, pwconv `0x10001944`, passmgmt `0x10002ee4` | signed divide of a time or day value by 86400; `pwconv`/`passmgmt` handle shadow day counts | low-medium: const-only scan | login admin / periodic (accounting) |
| 26 | `/sbin/ps`, `make`, `restore`, `/usr/bin/more`, `icrash`, `X11/xfs` | `ps` tty mtime/ctime vs `time`, `make doname`, `restore extractfile` | as listed | `st_mtime`/`st_ctime` signed vs another time word | medium: scan | rare / periodic |

Kernel items in rows 11, 12, 21, 22 come from only 79 scanned functions; the rest of the kernel
(XFS/EFS inode time updates, `time` variable readers, NFS/lock managers) is unscanned.

## Counts

- Userland: 5898 MIPS ELF files enumerated, 2068 reference a time source, 7743 candidate windows
  scanned. Kernel: 79 `microtime`/`nanotime` callers. Raw 86400-divide scan: 333 sites in 152 files.
- Scanner hits, deduplicated across arch/hash copies: 1264 userland (45 high, 528 medium, 691 low)
  plus 11 kernel. By likelihood (userland): login 24 high / 11 medium / 25 low; boot 9 / 148 / 240;
  rare 12 / 369 / 426.
- Hand-read (chain confirmed in disassembly): rows 1 (n32), 2, 3 (`0x100052f4`), 4, 5, 8 (gmtime
  core), 9, 10, 11, 12, 13, 14, 15 (timed), 17, 18, 24 (n32).

## Act on these first

- **`iaf/scheme` has three signed day-divides, and only one is patched.** The earlier fix covered
  `0x10009c4c`. The other two (`0x100052f4` and `0x100089dc`/`0x100089cc`) sit behind a `limit > 0`
  test, so they bite only for accounts that have ageing configured — patch them too for completeness.
- **`passwd` multiplies a shadow day count by 86400** (`0x100046d4`) before `gmtime`; that overflows
  32 bits at day 24855 = 2038-01-19, independent of any `div`.
- **N64 `time_t` is 32-bit here.** The n64 `pam_unix` path signed-divides the 32-bit `time()`
  return, so the n64 PAM account check needs the fix too, not just n32/o32.

## Provenance

Scripts and the full hit list are under `/scratch/origami/graphics/y2038-scan/` (`scripts/`,
`hits.tsv`); the TSV is copied here as [`y2038-signed-time-hits.tsv`](y2038-signed-time-hits.tsv)
(columns: likelihood, confidence, binary, abi, window start, nearest dynsym, address, op, taint kind,
source, other operand, instruction). All addresses are virtual addresses in the named binary; kernel
addresses are offsets in the object's `.text`.
