# getpwuid failing at boot when the clock is past 2^31 (IRIX 6.5.30 nsd / libc / NFS client)

Method: binary RE only, shipped 6.5.30 media under `refs/irix-media/6.5.30`. No IRIX source or headers opened
(the shipped `usr/include/ns_*.h` were deliberately not read). Own-words notes; symbol names come from each
binary's own symbol table where it has one (`nsd` has a full one; `libc.so.1` has only its exported dynamic
symbols, so internal functions there are unnamed). Disassembly is capstone (mips64, big-endian) with a local GOT-slot annotator.
Nothing here was run on a guest: every conclusion is static.

Objects examined:
* `usr/etc/nsd` (n32, not stripped, has a symtab)
* `var/ns/lib/libns_files.so`, `libns_mdbm.so` (nsd plug-in modules)
* `lib32/libc.so.1` (n32; dynamic symbols and debug sections only, no full symbol table): `getpwuid`, `ns_lookup` (= `_ns_lookup`), `__ns_cache_open`
* kernel NFS client: `usr/cpu/sysgen/IP27boot/nfs.a` and `nfs3.a`, member `nfs_client.o` (n64 relocatable)

Cross-release check against another `refs/irix-media/<version>` was not done.

## Corrections (2026-11-10)

An independent re-check against the shipped binaries changed the following. Readers who relied on the
earlier text should note:

* **The headline answer was superseded by section 9 and not updated.** Section 0 and section 5 below graded
  the libc `ns_lookup` signed compare as "fails safe" (a harmless miss) and named the kernel
  `nfs_getattr_cache` compare as the best first patch. Section 9 shows that a permanent client-cache miss is
  itself the boot regression (about 12 minutes at a 2038 clock, fixed by the one-word libc change). The libc
  compare is the primary userland fix; the kernel compare remains a real, separate signed-time bug. Sections 0,
  3 and 5 are annotated in place.
* **`nsd` log-site addresses in section 9 were swapped and mislabelled.** They are call sites, not string
  addresses. Corrected there: the call at `0x1000539c` logs "bget: not found", the call at `0x10005354` logs
  "bget: not found 2", and `0x100053b8` logs "bget: empty". The strings themselves are at `0x1001c158`,
  `0x1001c170` and `0x1001c148`.
* **`nsd_file_timeout` (section 2) does have callers.** Two direct calls (at `0x1000b5c0` and `0x1000efe0`),
  both passing a zero "now", which skips the one time compare. The conclusion (not exercised, not the cause)
  stands; "dead/unreachable" was wrong.
* **libc symbol source.** `lib32/libc.so.1` is not a symtab-bearing binary; the header wording was
  corrected. The quoted function addresses and sizes are unchanged and were re-verified.
* Re-verified unchanged: the `ns_lookup` signed compare word `010b402a` at `0xfa4e2c8` with the `beqz` miss branch
  at `0xfa4e2d8`. Not re-checked here: the installed-disk build offsets and hashes, and every live guest result
  in section 9 (they come from guest runs this check had no logs for).

## 0. Answer first

I could not prove, from static analysis alone, a single comparison that produces the observed "survives restart
of nsd and survives setting a sane date" behaviour. What I can establish:

1. The name-service path has three layers that each hold times: kernel NFS client (nsd is the NFS server of the
   `/ns` mount), nsd (in-memory file objects and on-disk cache), libc `ns_lookup` (reads the on-disk cache
   directly). The signed/unsigned usage differs between them, and one of them is a real signed-time bug.
2. A clear bug in the kernel, originally ranked best candidate (section 9 supersedes the ranking: the libc
   `ns_lookup` compare in item 4 is the one measured as the 2038 boot flood): kernel NFS client
   `nfs_getattr_cache` in `nfs_client.o` (offset 0xb74) tests attribute-cache validity as `slt time, r_attrtime`, signed, where `time` is the 4-byte
   kernel global. The client invalidates by storing 0 in `r_attrtime` (`nfs_purge_caches` at 0x60; new/zeroed
   rnodes are also 0). With `time` >= 2^31 (negative as a signed 32-bit word), `time < 0` is true, so an
   invalidated or brand-new rnode looks permanently valid. Stale attributes are then served without contacting
   nsd. This explains why restarting nsd does not help (a re-mounted `/ns` gets fresh rnodes, which start in the
   same "valid forever" state while the clock is still negative).
3. Mismatch with the report: this theory predicts healing once the clock is set back below 2^31, because then
   `time < 0` is false and the entry revalidates. The report says a sane date later does not heal. I did not find
   a persisted value that explains that (section 5 lists what I checked and why each one self-heals). Either the
   later date-set was not seen by this path, or a second persistent element exists that static reading did not
   reveal. Flagged unresolved, not papered over.
4. In userland there is no other signed compare that fails on a negative "now" by serving a *bad hit*. The two
   userland compares that are signed against a persisted time (libc `ns_lookup` at 0xfa4e2c8, nsd
   `nsd_map_update` at 0x10008d18) fail towards a miss or an overwrite, not a bad hit. "Fail safe" was too
   generous for the libc one: a miss on every lookup is a performance collapse (section 9), which is the
   observed 2038 regression. The nsd one is a write-side decision and is not on the hot path.

Original suggestion, superseded by section 9 (patch the libc `ns_lookup` compare first): `nfs_getattr_cache` +0xb74 (`slt v0,v0,v1`; treat as unsigned, i.e. `sltu`,
or additionally require `r_attrtime != 0`). Verify by observing, not by assuming (see section 7).

## 1. Call chain from getpwuid down

All libc addresses are file offsets-as-VMAs in `lib32/libc.so.1` (n32).

* `getpwuid` (0xfa4cbe8, 172 bytes) allocates two static buffers once (0x3c and 0x1000 bytes) then calls an
  unnamed static helper at 0xfa4c86c (this is the shared "look up one passwd entry" worker; no symbol).
* Worker 0xfa4c86c:
  * if the global `_getpwent_no_yp` is set, it skips the name service and scans `/etc/passwd` linearly;
  * otherwise it formats the uid as text (`_ltoa`);
  * unless `_getXbyY_no_stat` is set, it `_stat`s `/etc/passwd` and stores the 32-bit `st_mtime` word
    (stat struct offset 0x4c in the n32 layout) into the first word of a static "ns handle";
  * calls `_ns_lookup(handle, NULL, "<passwd table name>", <uid text>, ...)`.
  * result 0 = found, parse the returned line; result 1 = not found, return NULL; any other result (error)
    falls back to a linear scan of `/etc/passwd` only when the `stat` above succeeded.
* `ns_lookup` / `_ns_lookup` (0xfa4e05c, 1624 bytes):
  1. argument sanity, total key length <= 0x200;
  2. client-side cache: `__ns_cache_open` (0xfa4defc) opens (read-only mdbm) a per-table file under
     `/var/ns/cache` (string present in libc and in nsd). Key = key text (optionally plus an extra component);
     value layout (read here, written by nsd, see section 3): bytes 0..3 expiry, 4..7 creation time (both
     big-endian), byte 8 status, bytes 9.. payload;
  3. cache hit requires all of:
     * value length >= 9;
     * `handle[0] < created`, **signed** (`slt` at 0xfa4e2c8). handle[0] is the passwd file mtime saved by the
       worker, so the rule is "entry was created after the file last changed";
     * status byte is 0 or 1;
     * `time() <u expiry`, **unsigned** (`sltu` at 0xfa4e314, fed by `time` at 0xfa4e300);
  4. on miss it builds a path from fixed prefix + optional domain + table name (':' turned into '/') + optional
     `.`-suffix component + key and `open`/`read`s it. That is the `/ns` NFS mount served by nsd. Retries up to
     4 times on error 0x97 (ESTALE-class, value 151) and errno 5 (EIO), with `_usleep` back-off (0x61a8 µs steps,
     up to 10) on one error path; on ENOENT it can `mkdir` the cache directory and retry.
     I did not resolve the exact `/ns` prefix string (not asserted).
* On the nsd side the same request arrives as an NFS v3 LOOKUP/READ on a file under the `/ns` mount:
  `nsd_dispatch` (0x1001171c) -> `nfs3_lookup` (0x1000cd7c) -> directory callouts -> `libns_files` for passwd:
  `file_passwd_lookup` (0x40b690) -> `file_generic_lookup` (0x40409c) -> `file_domain_lookup` (0x40f8ac) ->
  `file_open` (0x40f520) which `stat`s the file and `time()`s.

## 2. nsd in-memory time handling (all 32-bit words, all signed)

nsd is an N32 binary; `time_t` is 32 bits; every time access is `lw`/`sw`.

* Timer queue: `nsd_timeout_new` (0x100169d4) stores absolute (sec, usec) from `gettimeofday` and inserts in
  ascending order using signed `slt` on sec, then usec. `nsd_timeout_set` (0x100168a8) computes the delay to
  the head as `head - now` in 32-bit wrap arithmetic, normalises usec to < 1,000,000 (the loop logging
  "adjusting time structure to within limits"), and clamps a negative sec to (0 s, 1 µs). Because it only ever
  uses differences, it is immune to the clock being negative, and to the sign flip while running. This is why
  crossing 2^31 at run time works. Not a cause.
* Result-file expiry: `nfs3_lookup` creates a result file object and sets file +0x5c to `ttl + time()` (ttl from
  attribute "timeout", default `nsd_timeout/1000`; `ttl < 0` stores bare `time()` instead). Sign is irrelevant to
  its only use: `nsd_dispatch` (at 0x10011818) treats any non-zero +0x5c on a non-directory object as
  "temporary, delete from parent after first handle operation", with no time compare at all.
* `nsd_file_timeout` (0x10006744) does contain `slt [file+0x5c], now` (signed). It has two external callers
  (direct calls at 0x1000b5c0 and 0x1000efe0), and both pass a zero "now" in a1, which skips that compare
  (the entry code tests the argument and branches over it). So the time-bearing compare is not exercised; not a
  cause. (The first read missed these callers because each loads its target through a GOT slot just before the call.)
* Creation stamp: `nsd_file_init` (0x10005cd8) calls `gettimeofday` into file +0x54/+0x58 and copies it to
  +0x44/+0x48 (atime) and +0x4c/+0x50 (mtime). The NFS attribute encoder (0x1000c244 range, unnamed static)
  calls `gettimeofday` into +0x44 again on every GETATTR (atime = now) and ships atime/mtime/ctime as three raw
  32-bit seconds + words. With a clock past 2^31 these go out as values >= 0x80000000 (unsigned on the wire).
  Dependency: how the kernel NFS client converts them. In `nfs3_cache_check_fattr3` the client zero-extends
  (`lwu`) these into 64-bit values and compares for equality only, so no sign problem there. The "future mtime
  vs now" comparisons live in kernel code outside the NFS client (not examined here).
* `nsd_map_shake` (0x100085a4) closes idle cache mdbm handles when `lastuse < now + 30*level - 300` (signed
  `slt`, `lastuse` = a `time()` stamp stored in the map at +0xc by `nsd_map_open`). Differences of like-signed
  values; harmless.

## 3. nsd's on-disk cache and its two compares

* `nsd_map_open` (0x10008644) creates/opens the cache mdbm (`/var/ns/cache/<name>`), `chown`/`chmod`s it,
  and logs "Recreating invalide cache file" (it unlinks and recreates a file that fails `mdbm_pre_split`).
  It records `time()` into the map at +0xc. No time is persisted by the open itself.
* `nsd_map_update` (0x10008aec) is the only writer of cached results. Record = {expiry = now+ttl, created = now,
  status byte, payload}, both times big-endian 32-bit. ttl defaults to 300 s; a separate (negative-result)
  timeout is used when the request status is non-zero; ttl <= 0 means "do not cache".
  Before writing it fetches the existing value for the key and:
  * rebuilds the stored expiry from its four bytes and compares **signed**: `slt now, stored_expiry` (0x10008d18);
    if the stored entry is still valid it then tests the high byte of the *created* word (0x10008d28) and, if
    that is zero, skips the write and logs at level 4; otherwise it overwrites. (The high-byte test looks like it
    was meant for another field; stated as observed, not explained.)
  * Effect when `now` is a sane positive value and the old entry was written while the clock was past 2^31:
    stored expiry is negative as a signed word, `now < expiry` is false, so the entry is treated as expired and is
    overwritten. Safe direction.
* libc reader of the same file: see section 1 step 3. For an entry written with clock >= 2^31:
  `created` is negative, so `handle[0] (positive file mtime) < created` is false and the cache is **missed**,
  not hit. Wrong-hit-safe, but a permanent miss: this is the boot flood of section 9. The expiry's unsigned compare is the opposite choice from nsd's and is the only
  way an entry could outlive a clock correction (see section 5 for the one case).

## 4. libns_files (the passwd/group file source)

* `file_open` (0x40f520): `stat` the file, `time()` into s3. Saves mtime sec at rec+4 and mtime/1000 (µs) at
  rec+8, and `time()` at rec+0xc. If `now < mtime` (**signed** `slt`, at 0x40f69c) it sets bit 0 of rec+0x10 and
  logs once, via `ctime`, a message about the file having a time in the future; otherwise clears bit 0.
  With the clock negative and a positive on-disk mtime this branch is always taken. Intent: mark "mtime is in
  the future, don't trust it for staleness".
* `verify` (0x40f474, 172 bytes) is the callout the NFS lookup uses (`nfs3_lookup` at 0x1000d0d0..0x1000d0e8)
  to decide whether a cached result object is still current. It compares the object's creation stamp
  (file object +0x54 sec, +0x58 µs) against the source file record (rec+4, rec+8), signed, lexicographic:
  returns 1 (valid) if creation >= mtime; if creation < mtime it returns 1 only when the future-mtime bit
  (rec+0x10 bit 0) is set, else 0 (so nsd deletes the object and relooks it up). Sign interplay:
  * boot with clock >= 2^31 (creation negative, mtime positive): creation < mtime, but bit 0 is set (now<mtime),
    so objects stay valid. Works.
  * later with a sane clock (creation of those old objects still negative): bit 0 is cleared by the next
    `file_open`, `creation < mtime`, returns 0, the object is deleted and rebuilt. Heals.
  So `verify` is not the failure, and its behaviour is consistent with the "heals later" observation.
* `file_generic_lookup` (0x40409c) returns status 1 (not found) when the record is missing or the key is not in
  the mapped file, status 5 for a bad key. Not time-dependent.
* `libns_mdbm` `lookup` (0x40134c) and `shake` (0x4018ec): `shake` has one signed `slt` against a time passed in
  by `nsd_shake` (the generic per-minute hook); it only closes idle handles. Not a cause.

## 5. Why "restart does not fix, sane date later does not heal" is not explained by nsd or libc

Persisted/long-lived values I looked for, and what each does with a negative clock:

| value | where | compare | result |
|---|---|---|---|
| cache entry expiry/created | `/var/ns/cache/*` mdbm | libc: created signed, expiry unsigned; nsd: expiry signed | libc: permanent cache miss at a 2038 boot (the live regression, section 9); nsd: overwritten on next write. Survives restart (on disk) but is never wrongly hit by the compares above. |
| libc handle[0] = `/etc/passwd` mtime | static libc data, refreshed per call | signed vs created | positive mtime vs negative created: miss. |
| nsd file object stamps | nsd memory | verify (signed, future bit) | heals. Gone on nsd restart anyway. |
| nsd timers | nsd memory | differences only | fine. |
| kernel rnode `r_attrtime` | kernel memory | `slt time, attrtime`, sentinel 0 | **broken while clock is negative** (section 6). |

One narrow way a libc cache entry could survive a date fix: if `/etc/passwd` itself carries an mtime >= 2^31 (for
instance rewritten during boot after the clock was set), `handle[0]` is negative, a boot-time entry with negative
`created` passes `handle[0] < created`, and with `now` sane its unsigned expiry (written as `boot_time + ttl`,
which is > 2^31) is still in the future for decades, so a boot-time negative/failed result (status 1) would be
served until the real clock passes that value. That requires (a) passwd mtime on disk >= 2^31 and (b) a failed
result having been cached at boot. I have no evidence for either; it is the only mechanism found in userland that
matches "persists across nsd restart and across a date change". It depends on the kernel stat path returning
mtime sign (kernel-only, covered by another investigation). If the guest's `/etc/passwd` mtime is >= 2^31 this
should be checked first: dump the `/var/ns/cache` files and compare `created`.

## 6. Kernel NFS client, where the clear signed bug is

(Out of the userland scope but it is the name service's transport, so recorded.)

* `time` and `lbolt` are 4-byte globals in `kernel.o`; every NFS client read of `time` is a 32-bit `lw`.
* `nfs3getattr` (nfs_client.o 0x1038): marks the request flag word, calls `nfs_getattr_cache` (reloc at 0x105c);
  if that returns 0 it calls `nfs3_getattr_otw` (the on-the-wire GETATTR, 0xc38).
* `nfs_getattr_cache` (0xb34, 296 bytes): under the rnode mutex loads `r_attrtime` (rnode +0x178) and `time`, and
  computes `valid = slt(time, r_attrtime)` (0xb74, **signed**). On valid it copies the 0x140-byte cached attribute
  block (rnode +0xd0) to the caller and returns 1. Otherwise returns 0 (and returns the cached attrs anyway if
  the caller passed flag 0x80).
* `nfs_purge_caches` (0x24) invalidates by `dnlc_purge_vp` then `r_attrtime = 0` (0x60). `nfs_cache_check` (0x200)
  also treats `r_attrtime == 0` as "must purge" (0x268). So 0 is the "no valid attributes" sentinel, and the
  signed compare is only correct while `time >= 0`.
* `set_attrcache_time` (0x878) sets `r_attrtime = time + timeo` where timeo is `time - r_last_change`
  (+0x17c) clamped (signed `slt`) into the mount's min/max attribute-timeout fields (+0x1a8..+0x1b4). Like-signed
  differences; fine.
* Consequence with `time` >= 2^31: `time < 0` is true, so any rnode with `r_attrtime == 0` (new, purged, or
  invalidated after a name-cache purge) is served from its stale cached attribute block and never revalidated,
  for as long as the clock stays negative. nsd deletes each lookup result after the first handle operation
  (section 2), so a client that keeps trusting cached attributes/name-cache entries will keep presenting handles
  that nsd no longer has, which surfaces as ESTALE/ENOENT in libc's `open`/`read` and eventually as
  "no login associated with uid 0". Inference from code, not an observed trace. I did not trace the name-cache
  (`dnlc`) lookup side or the NFS `lookup`/`read` vnode ops, so the step from "stale attrs" to the exact failing
  syscall is unconfirmed.
* Because a restarted nsd gets a fresh `/ns` mount whose rnodes start at `r_attrtime == 0`, the broken state is
  reproduced immediately: this matches "restarting nsd doesn't fix it".

## 7. What would settle it, and the single comparison to change

If the bug is the kernel one, the minimal fix is `nfs_getattr_cache` (nfs_client.o, +0xb74): make the validity
test `sltu` (unsigned), or require `r_attrtime != 0` before the compare. All `time`-relative uses in
`set_attrcache_time` are differences and need no change.

Checks that would confirm or reject it without a guess (not run, need a guest with the 2106 RTC):
* With the clock past 2^31, look at nsd's own log at high verbosity (`nsd -l`, level 4+ in the log calls seen
  above): if nsd never receives a LOOKUP for `passwd.byuid` on the second `getpwuid`, the client served it from
  cache (kernel path). If nsd does receive it and answers "not found" (status 1), the failure is in
  `libns_files`/`file_generic_lookup` instead.
* `ls -l /etc/passwd` and the `st_mtime` word: if >= 2^31, apply the persistent-cache scenario in section 5.
* Inspect `/var/ns/cache/*` entries' first eight bytes: expiry/created words >= 0x80000000 mean entries written
  under the negative clock.

## 8. Unreachable or not done

* No runtime evidence; everything is static.
* The `/ns` path prefix string in libc `ns_lookup` and the exact failing syscall in section 6 were not resolved.
* Unnamed static functions (libc worker 0xfa4c86c, nsd attribute encoder near 0x1000c244, `nfs_cache_check`
  callers) were read by address; none carries a symbol, so names given for them are descriptive.
* Kernel `stat` mtime sign handling and the 2106 floor patch were not examined (another investigation).
* DNLC lookup path and the NFS vnode `lookup`/`read` ops were not examined.
* `libns_nis`/`libns_dns`/`libns_ldap`/`libns_db`/`libns_ndbm` import no `time`, `gettimeofday` or `stat`
  (checked by dynsym), so they cannot hold a time compare; only `libns_files` and `libns_mdbm` do.
* Scratch disassembly and scripts were kept in a local work directory
  (`nsd.a`, `ns_files.a`, `ns_mdbm.a`, `libc.a`, `ar/nfs_client.s`).

## 9. Update (2026-11-09): the boot flood is the libc client-cache permanent miss

After the kernel `nfs_getattr_cache` fix in section 6/7 landed, a guest booting at 2038-02-01 still
degrades: `nsd` logs ~531,000 lines and ~22,900 "bget: not found" (vs ~990 lines and zero at 2026 and
at 2037-06-01), and every `canonhost`/`emgrlogger` lookup takes 10-70 s, so boot takes ~11 minutes.
That measurement settles which of the userland compares this document dismissed on correctness (a miss
rather than a bad hit) is the live regression: a *permanent* client-cache miss is exactly a boot flood.

The live site is the libc `ns_lookup` freshness check from section 1 step 3 and section 3:

* binary `lib32/libc.so.1` (n32), function `ns_lookup`/`_ns_lookup` (0xfa4e05c).
* instruction `slt $t0, $t0, $t3`, word `010b402a` — its offset is build-specific (see "Which libc
  build, which offset" below; the install-media and installed-disk builds differ). `t0` =
  `handle[0]` = `/etc/passwd` `st_mtime`; `t3` = cache record bytes 4..7 = `created`. A cache hit
  requires `mtime < created` **signed**; the `beqz` at 0xfa4e2d8 skips the cache (falls through to the
  `/ns` NFS path, i.e. nsd) when the compare is false.
* At a clean 2038 boot `/etc/passwd` `mtime` is pre-2038 (positive as a signed word) and every record
  nsd writes that boot is stamped `created = now` which is negative as a signed word. Signed `slt`:
  positive < negative is false for every key on every lookup, so libc never serves from
  `/var/ns/cache` and goes to nsd on every `getpwuid`/`canonhost`. nsd's in-memory node cache missing
  per lookup (the "bget: not found" flood, itself 2038-independent) then re-runs the slow backend each
  time. Fixing the compare lets the client cache absorb repeat lookups and stops the nsd traffic.

**One-word fix:** `slt` → `sltu`, word `010b402a` → `010b402b` (only the funct field changes,
0x2a→0x2b), at the offset for the libc build you are patching (see below — patch the build that is
actually installed). Both operands are genuine Unix timestamps; across 1970..2106 every value fits
unsigned 32-bit, so unsigned order is the true wall-clock order across 2038. The paired expiry test
(at `0xfa4e314` in the media build) is already `sltu`, so this makes the pair consistent. No
negative-sentinel caveat: `created` is a timestamp, not a flag (the flag is the separate status byte
8), and `created == 0` gives `mtime <u 0` = false = a benign miss, as today.

### Which libc build, which offset

The routine is at a different address in two distinct `lib32/libc.so.1` builds (the installed one sits
0x480 above the media one), so patching by offset needs the right build. Confirm the 4 bytes read
`01 0b 40 2a` before writing `2b` to the funct byte.

| Build | sha256 | `slt` VMA | file offset | patch byte (2a→2b) |
|---|---|---|---|---|
| **Installed golden disk** (the one that runs; live-patched) | `e774e2fb081ab653b22a9b633eae4904dd3df3ea58ca3430e9e511cd5311953b` | 0xfa4e748 | **0x6e748** | 0x6e74b |
| Install media (`refs/irix-media/6.5.30/lib32/libc.so.1`, first read here) | `3fe29d6e191a3fe62969d46ffa1088fc7fb7029ccc57006072ae7cb357f93362` | 0xfa4e2c8 | 0x6e2c8 | 0x6e2cb |

Patch the installed build. Live result (confirmed on a guest): the fix takes a 2038 boot from ~12
minutes to 39 seconds, with the `nsd` "bget" count dropping from 69,501 to 80. The media-build offsets
(0x6e2c8 and the `0xfa4e...` addresses in sections 0-4 above) are this note's original read and apply
only to that build. Live `nsd` log call sites in `nsd_bget` (0x100052a0), for reference: the call at 0x1000539c logs
"bget: not found: %u", the call at 0x10005354 logs "bget: not found 2: %u", and the call at 0x100053b8
logs "bget: empty" (an earlier working note mislabelled 0x100053b8 as the "not found" site, and a first
correction of it swapped the other two). The format strings sit in nsd's data at 0x1001c158, 0x1001c170
and 0x1001c148 respectively.

Ruled out as the live driver (checked, not assumed):

* nsd serve-path address 0x1000c348 is the NFS reply-transmit stub (its real call is `tsix_sendto_mac`)
  — no `mdbm_fetch`, no time compare.
* `libmdbm.so` is a pure hash database: its dynsym imports no `time`/`gettimeofday`/`clock` and it
  defines no expiry function. No freshness logic lives there.
* nsd `nsd_map_update` `slt` at 0x10008d18 is the write-side overwrite decision (section 3) and fails
  safe; it is not on the hot read path.

Caveats: the patch target is the n32 `lib32/libc.so.1`; other on-media libc copies are separate builds,
so confirm the word `010b402a` at the offset for that build before patching any of them (the offset is
build-specific: the installed disk and the install media already differ, see the table above). The
10-70 s/lookup cost is a property of the resolver backend, not a 2038 compare; the
2038 regression is only that libc stops absorbing repeat lookups and exposes that cost on every call.
The miss → flood chain was found statically and is now confirmed live: patching the installed build's
compare took a guest's 2038 boot from ~12 minutes to 39 seconds. The companion whole-binary scan is in
[y2038-signed-time-scan.md](y2038-signed-time-scan.md).
