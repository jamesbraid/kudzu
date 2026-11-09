# IRIX 6.5.30 core kernel time (2038 / 2106) - binary RE notes

Scope: shipped 6.5.30 IP35 kernel objects (`usr/cpu/sysgen/IP35boot/kernel.o` and the members of
`os.a`, `xfs.a`, `nfs.a`, `nfs3.a`, `snfs`, `oncrpc.a`, `efs.a`, `udf.a`). There is no linked `/unix`
on the media; `kernel.o` is the relocatable that carries the clock and hub RTC code, and offsets below
are file-relative offsets inside the named object (relocations are unresolved, symbol names come from
the objects' own symbol tables). The requester's linked addresses map by a constant: linked
`inittodr` 0xc000000000067b30 = `kernel.o` .text 0x279b0 (delta 0x40180).
Method: Capstone disassembly of each routine with relocations annotated; layouts of kernel structs are
inferred from load/store offsets (the extracted members carry no usable DWARF), so field names are my
labels, offsets are exact. Nothing here was run in an emulator.
Cross-check: `inittodr` has the same floor test and the same signed compare in the 6.5-base (1998) IP27
kernel and the 6.5.30 IP27 kernel. IP30 has a longer variant (not analysed).

Conventions: "kernel timeval" = 16-byte struct {64-bit sec, 64-bit usec}. "kernel timespec" (also the
n64 user timespec and the in-kernel vattr time) = {32-bit sec at +0, 4 pad bytes, 64-bit nsec at +8}.
This pair is why the answers differ between gettimeofday and stat (see A2).

## A1. The "preposterous time in file system" check (confirms the requester's finding)

* Function: `inittodr(base)`, `kernel.o` 0x279b0, 244 bytes. Thin wrapper `clkset(base)` at 0x295c0
  just calls it.
* Floor test: `lui/ori a3,0x966/0x180` (0x09660180 = 157,680,000 s = 1825 days, about 31 Dec 1974), then
  `slt a3,a0,a3` at 0x279e8 (signed, 64-bit register that holds a sign-extended 32-bit value, so it is
  a signed 32-bit compare). If base < floor: `cmn_err_tag(0x34, CE_WARN, "preposterous time in file
  system")` then `cmn_err_tag(0x36, CE_WARN, "CHECK AND RESET THE DATE!")` and return. Any base with
  bit 31 set looks negative and takes this path. Changing that `slt` to `sltu` is what the requester
  proposes; `a3` and `a0` are both canonical sign-extended so `sltu` would accept 0x80000000-0xffffffff
  (2038-2106) as large unsigned values.
* What it leaves behind when it rejects: nothing. It never writes `time` or the realtime base. The
  only stores in the whole routine are `fastick = 1000000/fasthz` (every call) and a static 32-bit flag
  at .sdata+0x134 (set to 1 only when base passed the floor; unnamed, reader unknown). The clock stays at
  whatever `init_timebase` loaded from the RTC.
* After the floor: `diff = time - base` (32-bit `subu`), absolute value, if >= 172,800 s (0x2a300, two
  days) it prints `"clock %s %d days"` ("lost" when time < base signed, "gained" otherwise) and the
  CHECK AND RESET message. Still no write to `time`. So inittodr is a diagnostic only, never a setter.
* Argument is not a superblock time for XFS: `xfs_cmountfs` (xfs_vfsops.o) calls `rtodc()` and passes
  that to `clkset` (guarded by an incoming flag in its 8th argument; I could not resolve what that flag
  means, so I cannot say which XFS mounts take it). `udf_mountfs` and `krpc_init` do the same. Only
  `efs_mountfs` (efs superblock 32-bit field, `lw`) and `dl_mnt_root` (NFS root; a 32-bit field
  reached through the behavior chain, `lw`) pass a stored time. All loads are `lw` (sign-extending).
* `time` itself is a 32-bit signed int (`lw`/`sw`) set by `init_timebase` (0x27864) as `time = rtodc()`.

### rtodc and wtodc (the reason the value goes negative)
* `rtodc` (0x2a2b8, 0x52c bytes): reads BCD fields for three RTC chip types (selected by the chip id
  in a hub-visible register / cached type word), year = BCD value + 1968 (constant 0x7b0). So the
  chip can only express 1968-2067 and the kernel never checks the year. Range checks are only on
  sec<60, min<60, hour<24, mday<32, month<13 (signed `slti`). It accumulates seconds in 32-bit
  `mult/addu` (days loop from year 1970 with 400/100/4 leap rules) and returns the 32-bit sum
  sign-extended, so a chip date at or after 2038-01-19 03:14:08 returns a negative number.
* On a failed field check it prints `rtodc: preposterous time in tod chip: ...` (CE_WARN), substitutes
  a hard-coded date in 1998 (year 0x7ce; my read of the other constants gives month 5, day 18,
  00:00:00; inferred, medium confidence), stores it in `time` and calls `wtodc` to rewrite the chip.
  This is the one place the kernel resets the RTC after rejecting it.
* `wtodc` (0x2a7e4) is the opposite sign convention: it loads `time` with `lwu` (zero-extended) and
  uses `sltu`/`ddivu`, so it writes any 32-bit value correctly through 2106 as far as the loop goes,
  but the year register is year-1968 (max 99, so 2067); beyond that the BCD nibble overflows. So the
  write path is unsigned and the read path signed, a mismatch by itself.
* `chktimedrift` (0x27c2c, called from `onesec_maint`): `rtodc() - time` as 32-bit `subu`, `slti` against
  +3 / -2, otherwise `doadjtime`. Wrap-safe because it only looks at a small signed difference.

## A2. Signed / unsigned 32-bit seconds, path by path

Summary table (S = sign-extending or signed compare, U = zero-extending/unsigned, raw = bits copied):

| Path | Result | Where |
|---|---|---|
| `time` global, `lbolt` | 32-bit signed int | clock.o |
| `nanotime_syscall` / `nanotime` | seconds = 32-bit `addu` result, stored with `sw` at +0 of a kernel timespec | kernel.o 0x290d4 / 0x292ec |
| `microtime` | `lw` sec -> `sw` at **+4** of a zeroed 64-bit slot: **zero-extends** (high word never written) | kernel.o 0x29460 |
| `irix5_microtime` | `lw` -> `sw` 32-bit {sec,usec} | kernel.o 0x2941c |
| gettimeofday n64 | native copy of the zeroed kernel timeval: **zero-extended**, positive for 2^31..2^32-1 | clock.o 0x2c84 |
| gettimeofday o32/n32 | `timeval_to_irix5_xlate` stores low 32 bits (user reads int32, negative) | clock.o 0x2e3c |
| time(2) `gtime` | `lw` low word -> `sw` at +4 of a zeroed 64-bit return slot: **zero-extended** to 64 bits for n64; plain 32-bit result for o32/n32 | scalls.o 0x0 |
| stat/fstat/lstat, all ABIs | raw 32-bit copy of the inode second; **no extension in the kernel** (see below) | vncalls.o |
| utime / utimets | raw 32-bit in, no checks | vncalls.o |
| stime | `lw` (signed) of the argument slot, no range check | scalls.o 0x34 |
| settimeofday (via syssgi) | `lw` low word (signed), no range check | clock.o 0x2cec |
| settime/local_settime | stores low 32 bits (`sw`) into `time` and the base | kernel.o 0x2957c/0x294a4 |
| NFS v2/v3 | raw 32-bit pass-through; no sign logic, no range checks | nfs/nfs3/snfs |
| itimers, callout wheel | relative tick/interval based, not wall-clock | time.o, clock.o |

### Kernel clock core
* `nanotime_syscall` (0x290d4): reads the base {32-bit sec at +0, 64-bit nsec at +8} under a
  generation counter (`.sbss` seq words), adds ticks-since-base scaled by `timetrim`, folds in
  `adjtime_*`, normalizes ns with a loop (`slt` against 1e9), seconds = `addu t7,t0` (32-bit wrap), `sw`
  to the output. Then it keeps the global `time` monotonic: `slt time, secs` (**signed 32-bit**); if
  `time < secs` it takes `timelock` and stores `secs`. Consequence: when seconds cross 0x7fffffff ->
  0x80000000 the new value is "less than" `time` in a signed compare, so `time` stops advancing until
  something calls `local_settime`. `nanotime_syscall` itself does not read `time` for its result, so
  wall-clock readers keep counting; only consumers of the `time` global stall. (Inferred from the
  compare; not observed running.)
* `local_settime(sec, usec)` (0x294a4): normalises usec with a signed `slt` loop (usec -= 1e6, sec++),
  no negative-usec handling, stores `time` and the base with `sw` (low 32 bits only), clears
  `adjtime_*`. `settime` then forwards to the cell's vhost vector (offset 0x78 of the ops table).
* `time` is referenced by many subsystems (autofs, ARP/route/ND timers, TCP timers, SysV msg/sem/shm
  ctime, NFS attribute cache, quotas, procnew/deadscan, rpc). I did not audit each user's compare
  sign; they all load it with `lw` so they see a negative number for 2038+.

### gettimeofday / time (a measurement said n64 gettimeofday zero-extends: confirmed)
* `gettimeofday` (clock.o 0x2c84): zeroes both 8-byte words of a kernel timeval, calls `microtime`, which
  writes only the low half (`sw` to +4) of sec and a 64-bit usec (`nsec/1000`). It then calls
  `xlate_copyout` with the current ABI. `xlate_copyout` (subr.o 0xed0): if `(abi & 4) != 0` (n64) it is
  a plain 16-byte `copyout` of the kernel struct, so tv_sec is zero-extended: value is 0x00000000hhhhhhhh.
  Otherwise (o32/n32) it calls `timeval_to_irix5_xlate`, which writes `lw sec`/`usec` as two 32-bit
  words, so userland receives the low 32 bits and reads them as int32 (negative from 2038).
  `irix5_to_timeval_xlate` (0x2dc8) is the input direction (`lw` sec, `lw` usec, stored at +4 and +8).
* time(2): `gtime` (scalls.o) calls `microtime` into a stack timeval and writes `lw` low word to `rvp+4`.
  The syscall dispatcher (trap.o `syscall`) zeroes the 64-bit return slot before the call and loads the
  user's v0 from it afterwards, so n64 time(2) returns a zero-extended value (correct to 2106 as an
  unsigned 32-bit count in a 64-bit register); o32/n32 get the low 32 bits. Sysent index 13 is `gtime` in
  both `sysent` and `irix5_64_sysent`.
* gettimeofday/settimeofday sysent: index 128 is `gettimeofday`; there is no table entry for
  `settimeofday`; it is reached through `syssgi` (calls `settimeofday` at syssgi 0x2bcc) and the older
  `sysmips` entry, which calls `settime` directly with a 64-bit `ld` of its argument.

### stat family (the "could a daemon see a negative mtime" question)
* ABI dispatch is in `xcstat` (vncalls.o 0x38d4): o32 -> `vattr_to_irix5_stat`, n32 ->
  `vattr_to_irix5_n32_stat`, n64 -> `vattr_to_irix5_64_stat` (0xf0-byte stat), plus `..._stat64`.
  `xstat`/`lxstat`/`fxstat` are wrappers. None of the four converters contains any sign test or
  extension of seconds (the only compare-like op in the stat code is the final `sltu` that turns the
  copyout result into EFAULT).
* Source: the filesystem getattr fills the vattr times. XFS `xfs_getattr` (xfs_vnodeops.o) stores
  atime/mtime/ctime seconds with `lw` from the in-core inode (+0x268/+0x270/+0x278) to vattr +0x30/+0x40/+0x50
  using **`sw` (32-bit)** and nanoseconds to +0x38/+0x48/+0x58 with `sd`. `xfs_setattr` does the reverse
  (`lw`/`sw` at inode +0x268/+0x270). `xfs_ichgtime` stamps with the low word of `nanotime_syscall`.
  EFS and NFS also hand 32-bit values.
* n64 (`vattr_to_irix5_64_stat`): zeroes the 0xf0 buffer, then copies each time with a 64-bit `ld`/`sd`
  from vattr +0x30 (atime sec), +0x38 (atime nsec), +0x40/+0x48 (mtime), +0x50/+0x58 (ctime) into
  stat +0x60/+0x68, +0x70/+0x78, +0x80/+0x88. Because the vattr second is a 4-byte field followed by
  padding, the user n64 timespec is {int32 sec at +0, pad, int64 nsec at +8}. **The kernel neither
  sign- nor zero-extends: it passes the 32-bit pattern as the 4-byte first member.** How it prints is
  decided by the userland type: as a signed int32 it is negative for 2038-2106. (Also copies the pad word
  from the vattr, which is not cleared by the converter.)
* n32 and o32 variants: `lw` then `sw` for seconds, `ld` then `sw` (truncate) for nanoseconds. Same
  bit pattern; userland int32 is negative.
* Conclusion for the second cause: a daemon in the same process family sees mutually consistent
  (negative) values from stat and gettimeofday only if it is o32/n32. **An n64 process sees
  gettimeofday/time(2) as a positive 64-bit value (2^31..2^32-1) but stat mtime as the same bits in a
  signed int32: negative.** Comparing `now - mtime` or `mtime + ttl > now` in such a process is wrong in
  the kernel-supplied data itself, independent of libc. For o32/n32 everything is negative together,
  and differences still work when done in 32-bit arithmetic.

### utime / utimes / utimets
* `utime` (vncalls.o 0x4de4): copies 8 bytes (two 32-bit values, same layout for every ABI, so the
  time_t of this call is 32-bit everywhere). Null argument -> `nanotime_syscall` low word. Builds a
  vattr request (mask 0x300 = atime|mtime) and calls `namesetattr_audit_extended`. No compare, no
  range check.
* `utimets` (0x4ee8): for n64 (`get_current_abi() & 4`) copies 0x20 bytes {int32 sec, pad, int64 nsec} x2
  with `bsd_copyin`; for o32/n32 copies 0x10 bytes {int32 sec, int32 nsec} x2. No validation of
  nanoseconds (no comparison instruction in the routine).
* `xfs_setattr` stores the seconds `lw -> sw` unchanged. Same for tv_nsec (no 0..1e9 check seen).

### stime / settimeofday / adjtime
* `stime` (scalls.o 0x34): `cap_able(CAP_TIME_MGT)`, `setmustrun(clock_processor)`, `settime(lw a0 = argument
  low word at slot+4, 0)`, `wtodc()`, `restoremustrun`, optional SAT audit. **No validation** of the value
  at all, signed `lw`.
* `settimeofday` (clock.o 0x2cec): capability check, `copyin_xlate` (n64 raw 16 bytes; o32/n32 via
  `irix5_to_timeval_xlate`), then `settime(lw buf+4, ld buf+8)`, `wtodc`. No range check on seconds or
  usec (normalisation is in `local_settime`). Timezone argument is ignored. `sysmips` settime path uses a
  64-bit `ld` of the user argument and `local_settime` truncates it to 32 bits.
* `adjtime` (clock.o 0x2ac4): the only range check in these syscalls: delta seconds `slti` -0x862 /
  +0x863 (signed, about +/- 2146 s), else EINVAL(0x16). Result delta in microseconds is formed in 64 bits.
* The write to the chip (`wtodc`) is unsigned (see A1), so a `settimeofday` to a date past 2038 does
  reach the RTC correctly up to year 2067 even though `time` itself is "negative".

### Timers, itimers, callout wheel
* Callout/timeout wheel (`dotimeout`, `timein`, `chktimeout_tick`, `callout_time_to_hz`): all work in ticks
  against `lbolt` (32-bit tick counter), using signed `slt`/`blez`/`slti` on small relative deltas. It
  never consults wall-clock seconds, so a 2038/2106 clock does not influence it. (Reviewed the compare
  instructions only, not every branch.)
* `hzto` (time.o 0x11cc) converts a relative timeval to ticks: seconds `lw` and signed `slt` against
  0x20c0b3 (2,146,483) and 0x147ae14 (21,474,836) selecting different scales, returns 0x7fffffff above.
  `fasthzto` guards with `slt` against 0x7fffffff/fasthz. `itimerfix` (0xc90): sec `bltz` and `> 100,000,000`
  (signed) -> EINVAL; usec `bltz` or >= 1e6 -> EINVAL; non-zero usec below one tick is rounded up to
  10,000. `timevaladd`/`timevalsub` do 32-bit `addu`/`subu` on seconds (wrap silently) and
  `timevalfix` normalises usec with signed compares. `itimerval` for o32/n32 is two `lw` -> 64-bit.
  ITIMER_REAL goes through `hzto` (relative), not an absolute wall time.
* POSIX timers (`ptimers.o`): kernel timespec is {int32 sec at +0, pad, int64 nsec at +8}.
  `timespec_add`/`timespec_sub` are 32-bit `addu`/`subu` on seconds with a signed `bltz` on nsec;
  `timespec_to_tick` does `lw` sec then `dmult`. `timer_settime` with TIMER_ABSTIME computes
  `timespec_sub(abs, nanotime)` then tests the 32-bit seconds with `bltz`; because both operands wrap the
  same way the difference is correct while |diff| < 2^31, so absolute timers keep working across 2038.
  `nanosleep` is relative.
* `second_thread` (clock.o): per-second housekeeping measures the interval between two `nanotime` reads with
  `timespec_sub` and `blez` on the seconds difference, wrap-safe; it also reads `time` only for `time & 3`.

### NFS timestamp conversion
* v2 client `nattr_to_vattr` (nfs_subr.o 0x13f0): seconds `lw` (wire fields at +0x2c/+0x34/+0x3c) -> `sw` at
  vattr +0x30/+0x40/+0x50 (raw); usec `lw` times 1000 via shift-add (32-bit arithmetic, no check for overflow
  above ~2.1 s of usec) -> `sd`. `vattr_to_sattr` (0x16c8) writes seconds `lw -> sw` and usec as signed `ddiv` by 1000;
  fields not being set are written as 0xFFFFFFFF both words. Wire codec `xdr_time_t` is `xdr_int`; the v2
  timeval is two `kxdr_int` (signed XDR ints) in `xdr_irix5_timeval`.
* v2 server `mapsattr2vattr` (nfs_server.o 0xa4b8): atime/mtime set only if the wire seconds field is not -1
  (32-bit `beq` with the sign-extended `lw`, equality only; usec is not consulted). A real timestamp of
  0xFFFFFFFF (2106-02-07 06:28:15) is therefore read as "do not change". `vattr_to_nattr` (0xa1cc) writes
  seconds `lw -> sw`, usec = `ld nsec / 1000`.
* v3: `xdr_nfstime3` is two `kxdr_uint32` (unsigned on the wire). Client `fattr3_to_vattr`
  (nfs_client.o 0x10a8): seconds `lw -> sw` (vattr +0x30/+0x40/+0x50), nanoseconds `lwu` zero-extended -> `sd`.
  Server `vattr_to_fattr3` (nfs_common.o): seconds `lw -> sw`, nsec `ld -> sw`; `sattr3_to_vattr` (nfs_server.o
  0x9858) copies `lw` secs and `lw` nsec and uses `nanotime_syscall` for "set to server time". No sign logic or
  range check anywhere in NFS; sign is decided by whoever consumes the vattr.
* The attribute cache (`set_attrcache_time`, `nfs_getattr_cache`, etc.) reads the `time` global; they are
  affected by the `time` stall described above, not by the wire format.

## Candidates for the second cause (kernel side only)
1. n64 vs stat mismatch: n64 `gettimeofday`/`time(2)` return the clock zero-extended (positive) while stat
   returns the same seconds as 32-bit signed-interpreted bits (negative). Any n64 daemon (or n64 libc path)
   that does `now - mtime`, `mtime + ttl < now`, or caches `now` and compares to a file's mtime sees
   inconsistent signs. o32/n32 callers do not (both come back as the same low-32 bits).
2. The `time` global can stall at the 0x7fffffff boundary because of the signed `slt` in `nanotime_syscall`
   (see above), which matters only if the clock crosses the boundary while running, not if the RTC is
   already past it at boot (then `time` starts negative and both sides compare consistently).
3. Not a candidate: the callout/timeout wheel and itimers (relative ticks), stat copyout (no
   extension), NFS conversion (raw), utime/stime/settimeofday (no checks).
4. Unresolved: what reads the static flag at .sdata+0x134 that `inittodr` sets; the meaning of the incoming
   flag that gates the `rtodc`/`clkset` call in `xfs_cmountfs`; the `rtodc` fallback day (inferred).
   A boot-time daemon that "caches a time and compares it signed" is most likely in userland (the other
   agent's scope); the kernel supplies the inputs described in item 1.
