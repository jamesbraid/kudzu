# XFS inode timestamps in shipped IRIX 6.5.30 (Q3: read/written/compared, widths, signedness)

Method: binary RE only. Object: `refs/irix-media/6.5.30/usr/cpu/sysgen/IP27boot/xfs.a` (members
`xfs_inode.o`, `xfs_vnodeops.o`, `xfs_rw.o`, `xfs_dfrag.o`, `xfs_itable.o`, ...), n64/mips4.
Cross-check: `IP35boot/xfs.a` (different md5, because it is compiled for the other board).
`xfs_ichgtime`, `xfs_swap_extents`, `xfs_bulkstat_one` are instruction-identical; the other time-relevant
members differ only in unrelated kernel-global displacements and one in-core offset elsewhere in `xfs_rw`.
Kernel side (`nanotime_syscall`) is from `IP35boot/kernel.o`. Disassembly: capstone, plus a local scanner
that finds every load/store at the timestamp displacements. The members carry DWARF, but it is
unrelocated and unreadable by readelf, so no struct names are available; every field below is inferred from
access patterns. Where I name a field it is by position, and I say so.

Not done: release cross-check against `6.5-base`; user-space tools (xfsdump, xfs_db, xfs_repair); any
analysis outside the XFS module except the two kernel items called out at the end.

## 1. Layout and widths

* There is no on-disk <-> in-core conversion routine in the XFS module (no endian swap symbol anywhere in
  `xfs.a`). `xfs_iread` copies the first 0x60 (96) bytes of the on-disk inode record with `bcopy` into the
  in-core inode at in-core offset 0x278 (`xfs_inode.o` 0xf98). `xfs_iflush_int` does the reverse `bcopy`
  (0x3504) after bumping a 16-bit counter at in-core 0x296. Log recovery (`xlog_recover_do_inode_trans`,
  0x227c) does one `bcopy` of the logged core, and `xfs_inode_item_format` logs the core as a 0x60-byte region
  (0x27c) with no copy. So on-disk, in-core and in-log core are the same big-endian bytes. In-core
  offset = on-disk offset + 0x278.
* Each timestamp is two 32-bit words: seconds then nanoseconds (8 bytes).

  | stamp | on-disk offset in core | in-core (ip-relative) | sec / nsec |
  |---|---|---|---|
  | atime | 0x20 | 0x298 | +0 / +4 |
  | mtime | 0x28 | 0x2a0 | +0 / +4 |
  | ctime | 0x30 | 0x2a8 | +0 / +4 |

  (On disk this order matches the public v4 inode core. The atime/mtime/ctime names come from the access
  pattern in `xfs_ichgtime`/`xfs_getattr`/`xfs_setattr`, which map them to the corresponding vattr stamps.)
* Many vnode-op routines hold a pointer 0x30 bytes into the inode (the embedded behaviour descriptor), so the
  same fields appear as 0x268/0x26c, 0x270/0x274, 0x278/0x27c in `xfs_getattr` and `xfs_setattr`. Those are
  not different fields.
* Width seen everywhere: 32-bit word loads (`lw`) and stores (`sw`). No 64-bit access to the
  seconds word exists in the XFS module. No zero-extending load (`lwu`) of a time word exists either.
  The module never decides signedness of the stored seconds; it just copies words. Signedness is
  imposed by consumers (kernel `time_t`, the stat layer, user tools).
* Kernel timespec (as seen from XFS): `{ 32-bit tv_sec @+0, pad, 64-bit tv_nsec @+8 }`. This is the layout
  of the vattr time fields as well (atime sec @0x30, nsec @0x38; mtime 0x40/0x48; ctime 0x50/0x58 in the
  vattr buffer used by getattr/setattr), and of the stack timespec in `xfs_ichgtime`.

## 2. Writers

### `xfs_ichgtime(ip, flags)` (xfs_inode.o 0x3c14, 188 bytes) -- the only "set to now" path
* flags: bit0 (1) = write mtime, bit1 (2) = write atime, bit2 (4) = write ctime. (Mapped from the stores:
  1 -> 0x2a0/0x2a4, 2 -> 0x298/0x29c, 4 -> 0x2a8/0x2ac.)
* Early outs: (a) a flags word reached via `ip+0x38 -> +0x30 -> +0x1c`, bit 0 set (read-only filesystem is
  my inference; the chain is not named in the binary); (b) mount flags word (`ip+0x28 -> +0x2e4`) bit 0x20 set
  AND `flags & 7 == 2` (atime-only update suppressed). Bit 0x20 is NOATIME: `xfsidbg.o` carries a mount-flag
  name table in order WSYNC INO64 RQCHK MNTCLEAN FSSHUTDN NOATIME ..., which puts NOATIME at 0x20 and FSSHUTDN
  at 0x10; `xfs_getattr` independently returns error 5 when mount flag 0x10 is set.
* Time source: calls `nanotime_syscall(&ts)` into a stack timespec; then `lw` sec (sign-extending load into a
  64-bit register), `sw` to the chosen in-core word(s); nsec is `ld` 8 bytes then `sw` truncating to 32.
  No compare, no clamp, no widening beyond the transient register. Same `lw` value is used for all selected
  stamps, so they are written with identical values.
* After the stores: `sync` barrier, then `sh 1, ip+0x25a` (marks the inode core dirty/needing-update).
  Return value is whatever the helper left in v0; callers ignore it.
* Reaching it: direct `jal` from the routines below, plus one function-pointer slot (below).

### Callers of `xfs_ichgtime` (flags are the immediate set in the call delay slot unless noted)
| function | member | flags |
|---|---|---|
| xfs_ialloc | xfs_inode | 7 (all three) |
| xfs_igrow_finish | xfs_inode | 4 |
| xfs_attr_set_int (x2), xfs_attr_remove_int | xfs_attr | 4 |
| xfs_droplink, xfs_bumplink | xfs_utils | 4 |
| xfs_truncate_file | xfs_utils | 5 |
| xfs_write, xfs_dio_write | xfs_rw | 5 |
| xfs_create_new (x3), xfs_create_exists (x2), xfs_remove, xfs_link, xfs_mkdir, xfs_rmdir, xfs_symlink, xfs_change_file_space | xfs_vnodeops | 5 each |
| xfs_rename (5 sites) | xfs_rename | 5, 4, 5, 5, 5 (in source order of the sites) |
| xfs_readlink, xfs_readdir | xfs_vnodeops | 2 (atime) |
| xfs_setattr (fast path) | xfs_vnodeops 0x570 | computed from the attr mask: mask bit 0x10000 -> 2, 0x20000 -> 1, 0x40000 -> 4 |
| xfs_setattr (end) | xfs_vnodeops 0x1544 | accumulated flag word (see below) |
* File reads: `xfs_read_core` (xfs_rw.o 0x1e30..0x1e38) and `xfs_vop_readbuf_core` (0xd04) update atime by
  calling `xfs_ichgtime` *through the iocore function-pointer slot* at `ip+0x490` with flags 2 (the pointer
  is stored at slot offset 0x60 of the iocore ops table `xfs_iocore_xfs`, in `xfs_iocore.o`, relocation
  to `xfs_ichgtime`). That table also exports `xfs_swap_extents` (slot 0x88). So read atime, and
  swap-extents, are reached indirectly. I did not look for a consumer of that table outside `xfs.a`
  (`cxfsstubs.a` is a stub set; the shared-filesystem build was not examined).

### `xfs_setattr` (xfs_vnodeops.o 0x44c), explicit stamps
* Explicit times arrive in the vattr; mask bits: 0x100 atime, 0x200 mtime, 0x400 ctime.
  The in-core words are written with `lw` (vattr sec, 4 bytes) -> `sw`, and `ld` (vattr nsec, 8 bytes) ->
  `sw` (truncates nsec to 32). At 0x1218-0x125c (atime, mtime) and 0x1318-0x1334 (ctime).
* After an explicit atime store the accumulated flag word loses bit 2; after an explicit mtime it loses
  bit 1 and gains bit 4 (ctime still updated to now); after an explicit ctime it loses bit 4. The final
  `xfs_ichgtime(ip, flags)` at 0x1544 then stamps whatever remains. Every path writes 0x22a=1 (dirty).
* No compare, range check or sign test is applied to the supplied stamps in this function. I searched every
  load of the vattr time offsets in the function: only the 6 loads above.

### `xfs_ialloc`
Sets creation times only through `xfs_ichgtime(ip, 7)` (xfs_inode.o 0x13c8). No other store to the stamp
words exists in the module (full scan of the in-core and on-disk displacements, all members).

## 3. Readers

* `xfs_getattr` (xfs_vnodeops.o 0x94): `lw` sec, `sw` into vattr sec word; `lw` nsec, `sd` into vattr
  64-bit nsec (this one sign-extends a 32-bit nsec into 64 bits -- harmless, nsec < 1e9). Copies 0x268..0x27c
  into vattr 0x30..0x58 (0x188-0x1b4). Mount shutdown flag 0x10 returns error 5 first. No compare.
* `xfs_bulkstat_one` (xfs_itable.o 0x0): copies on-disk (or in-core) core words 0x20/0x24, 0x28/0x2c,
  0x30/0x34 straight into the bulkstat record (record offsets 0x28/0x2c atime, 0x30/0x34 mtime, 0x38/0x3c
  ctime) with `lw`/`sw` pairs (0xb8..0xe4). Both words are 32-bit in the user-visible record. No compare.
* `xfsidbg.o` debug printer: format `atime 0x%x:%x mtime 0x%x:%x ctime 0x%x:%x` (unsigned hex). Debug only.
  `xfs_icrash.o` contains only type names (di_atime, bs_atime, ...), no code touching them; not reachable as
  a timestamp user.

## 4. Comparisons (the 2038 / 2106 question)

Inside the XFS inode path there is exactly **one** place that compares stamps, and it is an **equality**
test, so signedness does not matter:

* `xfs_swap_extents` (xfs_dfrag.o 0x214), at 0x504..0x544: compares the caller-supplied stat snapshot
  (user struct, mtime sec/nsec at +0x68/+0x6c, ctime sec/nsec at +0x70/+0x74) against the target inode
  (0x2a0/0x2a4 mtime, 0x2a8/0x2ac ctime). All are `lw` (32-bit, sign-extended into 64-bit regs) compared
  with `bne` (equality). Mismatch returns 0x10 (EBUSY). Both sides sign-extend the same way so the test
  behaves identically for any 32-bit value, including values above 0x7fffffff. No `slt`/`sltu` involved.
  Reached via the iocore ops slot 0x88; the ioctl caller was not traced (the fsr_xfs tool is the likely user, unverified).
* No sync/age logic in the inode flush path compares stamps. `lbolt` (ticks) is the only clock read in the
  module and only in `xfs_alloc` (stream timeout), `xfs_buf_item`, `xfs_mfstream`, `xfs_trans_ail`; none
  involve inode timestamps. `xfs_iflush*` and `xfs_inode_item*` do not read time at all.
* Every `lw` of a time word in the module is followed only by `sw`/`sd` (copy) or `bne` (swap-extents).
  I found no `slt`/`sltu`/`bltz`/`bgez` on a time value and no widening to 64-bit seconds in the inode path.

### Sign-extension points (all benign for storage; matter at the boundary)
| where | what |
|---|---|
| `xfs_ichgtime` | `lw` sec from stack timespec (sign-extended in reg) -> `sw` to inode. Storage is 32 bits, bit-exact. |
| `xfs_getattr` | `lw` sec -> `sw` into vattr: 32-bit; `lw` nsec -> `sd`: sign-extended into 64. |
| `xfs_setattr` | `lw` vattr sec -> `sw`; `ld` vattr nsec -> `sw` (truncation). |
| `xfs_bulkstat_one` | `lw`/`sw` pairs, 32-bit throughout. |
| `xfs_swap_extents` | `lw` both sides, equality only. |
So for XFS v4 on this kernel: seconds are 32-bit words end to end in the module; the sign/unsigned
interpretation is decided outside it. The only things that would stop a value in 0x80000000..0xffffffff
(post-2038, up to 2106 if read unsigned) from round-tripping through XFS are the callers' 4-byte vattr
sec field and the kernel clock (below), not XFS logic.

## 5. Adjacent signed time use found while checking (outside the inode timestamps but inside XFS archives)

* **Quota expiry (xfsquotas.a)**: `xfs_qm_adjust_dqtimers` (xfs_dquot.o 0x294) stores the grace expiry as
  `time` (kernel global, `lw`) + a 32-bit limit with `addu`, `sw` to dquot words 0x38/0x3c/0x60.
  `xfs_trans_dqresv` (xfs_trans_dquot.o 0xabc and 0xc08) then compares the stored expiry against `time`
  with **signed `slt`** on 32-bit values (expiry non-zero and `expiry < time` means grace expired,
  EDQUOT 0x46d). This is XFS-internal and signed: it breaks at 2038 (and anything above 0x7fffffff reads
  as negative/past). Present only when the quota archive (not `xfsquotasstubs.a`) is linked. Source of the
  `a2` operand at 0xab4 was not traced; the 0xc04 site loads it with `lw` from the dquot at 0xb0.
* **Kernel `nanotime_syscall` (IP35boot/kernel.o 0x290d4)**: builds seconds as a 32-bit sum
  (`lw` base + 32-bit `addu`), stores `sw` into the caller's timespec sec word, and maintains the global
  `time` with `lw` + signed `slt` + `sw` (`if (time < new) time = new`). So the producer of every
  `xfs_ichgtime` value is a signed 32-bit clock. Not XFS, but it is what feeds it. Not analysed further.
* `grio2_nanotime` called from `xfs_diordwr` (xfs_rw) is rate-guarantee timing, not inode stamps.

## 6. Unreachable / unsettled

* Names of the structure fields are inferred from position and vattr correspondence; DWARF unusable.
* Read-only-filesystem meaning of the first guard in `xfs_ichgtime` is inferred (flag word reached by three
  pointer hops, bit 0), not named in the binary.
* The iocore ops table consumer outside `xfs.a` (shared-fs build) was not examined, so whether
  `xfs_swap_extents`/read-atime are reached other than via `xfs_read_core`/`xfs_vop_readbuf_core`/the fsr
  ioctl is not settled.
* `xfs_macros.o`, `xfs_dir2_trace.o` have no text; `xfs_icrash.o` has no timestamp code.
