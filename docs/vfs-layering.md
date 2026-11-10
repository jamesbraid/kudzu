# Loopback file system and the layering interfaces of the VFS

How the IRIX 6.5.30 loopback file system (`lofs`) is built on the vnode and VFS interfaces, how a file system type is registered, which kernel services a stacked file system can call, and what the release offers for whiteouts, page sharing and NFS export. It is written for someone planning a union or copy-on-write file system as a loadable kernel module. The VFS switch table, the `vfsops` and `vnodeops` slot lists and the module loader are in [network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) (section 3) and [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md); this page links to them and adds what they leave out. Where it corrects a statement there, the other page was edited to point here.

Evidence is the linked kernel `6.5.30/unix.IP35` (with its symbol table) and the `lofs` objects in `6.5.30/usr/cpu/sysgen/IP35boot/lofs.a` (four members: `lofs_vfsops.o`, `lofs_vnops.o`, `lofs_subr.o`, `lofs_icrash.o`), which are linked into that kernel unchanged. The `lofs` routines are static, so the kernel's symbol table does not name them; their names and sizes come from the symbol tables of the archive members, and each member's text sits in the kernel as one block (`lofs_init` at 0xc0000000003c38a0 for `lofs_vfsops.o`, `lofs_open` at 0xc0000000003c3ea0 for `lofs_vnops.o`, `lofs_subrinit` at 0xc0000000003c5ad8 for `lofs_subr.o`), so a member offset plus its block start is the kernel address. The member debug records carry function and variable names only, not structure types. Field names below are descriptive names chosen here; offsets are read from the code. IP27 has no `lofs`; IP30 has the same 21-slot switch as IP35 ([network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) section 5) and its `lofs` was not compared. Names of public interfaces (argument names, flag names such as `FREAD`) are hints from the public vnode interface; the number of argument registers, the offsets and the flag values are read from the binaries. Grades follow [AGENTS.md](../AGENTS.md): Established means read in the named routine or table, Inferred names its basis, Unknown names what would settle it. Nothing here was run: no guest was started, no mount was made, and every statement about run-time behaviour is a reading of code. Open items are in [open-questions.md](open-questions.md) under "Loopback and file system layering".

## 1. How `lofs` is built

### 1.1 The shape in one paragraph

`lofs` is a stacked file system that mounts one directory tree over another path of the same host. Its mount takes the lower tree from a path name. For every directory it hands out, it keeps a small node (here called a lonode) that holds one reference to the lower directory vnode and sits in a per-mount hash table. Almost every vnode operation on such a directory finds the lower vnode through the node and calls the lower vnode's operation with the same arguments. The one structural rule that matters for a layered design: `lofs` wraps directories only. A lookup that finds anything other than a directory returns the lower file system's own vnode, and `create` does the same, so regular files, symbolic links and devices reached through a `lofs` mount are not `lofs` vnodes and never reach `lofs` code again. Established (`lofs_lookup` and `lofs_create`, sections 1.4 and 1.3).

### 1.2 Records

`lofs` keeps three kinds of record. Sizes and offsets are from the allocation sizes and the accesses in `lofs_mount`, `makelonode`, `makelfsnode`, `freelonode` and `lnode_flush`. The grade is Established for every offset listed; the meaning of a field is Inferred from its use unless the text says otherwise.

Per mount (288 bytes, allocated by `lofs_mount` with `kmem_zalloc`, called here the mount record):

| Offset | Size | Use |
|---:|---:|---|
| +0x00 | 32 | the behavior descriptor of the mount in the vfs's chain (private pointer to the record itself, object = the vfs, operations = `lofs_vfsops`, next) |
| +0x20 | 8 | the lower vfs (the `v_vfsp` of the vnode the mount path resolved to) |
| +0x28 | 8 | the `lofs` vfs |
| +0x30 | 8 | the root vnode of the mount (a lofs vnode wrapping the lower directory) |
| +0x38 | 8 | head of the list of shadow vfs records (section 1.2, third kind) |
| +0x40 | 16 | a mutex (class name "loinfo") protecting the shadow list and the count at +0x54 |
| +0x50 | 4 | mount flags: bit 0 read-only, bit 1 no-suid |
| +0x54 | 4 | number of lonodes alive (the unmount test wants it to be 1) |
| +0x58 | 4 | a number unique to the mount (section 1.7) |
| +0x60 | 192 | eight hash buckets of 24 bytes each: the list head at +0x60+24i and a 16-byte mutex (class name "lnodetable") at +0x68+24i; the bucket is chosen by bits 8 to 10 of the lower vnode's address |

Per directory vnode (0x60 bytes, the lonode; freed nodes go to a global list protected by a mutex of class "lnodefree", `lnode_freelist`):

| Offset | Size | Use |
|---:|---:|---|
| +0x00 | 32 | the behavior descriptor in the vnode's chain (private pointer to the lonode, object = the lofs vnode, operations = `lofs_vnodeops`, next) |
| +0x20 | 8 | next lonode in the hash bucket (or in the free list) |
| +0x28 | 8 | the lower vnode |
| +0x30 | 8 | the lofs vnode |
| +0x38 | 8 | a vnode pointer set by `lofs_lookup` in the nested-mount cases (meaning Unknown; section 1.4) |
| +0x40 | 8 | copy of the 8-byte word at +0x58 of the lower vnode, taken at creation |
| +0x48 | 4 | the global `vn_epoch` at creation |
| +0x4c | 4 | the halfword at +0x18 of the lower vnode, taken at creation |
| +0x50 | 4 | hold count on the lower vnode (atomic; starts at 1) |
| +0x54 | 4 | a flag set by `lofs_lookup`, copied from the parent lonode in some cases (meaning Unknown) |
| +0x58 | 1 | 1 when the lower vnode has bit 26 (0x4000000) of its flag word set (the vnode belongs to the local cache that is shared across the machine), else 0 |
| +0x59 | 1 | the hash bucket index |

The three words at +0x40, +0x48 and +0x4c are the triple that `vn_get` and `vn_purge` take to revalidate a vnode reference that was not held (a vnode map); Inferred from `vn_get(vp, &map, 0)` being called with the address of +0x40 in `makelonode`, and from `lofs_unmount`, `lnode_flush` and `lofs_reclaim` building the same triple on the stack from the vnode's fields at +0x58, +0x18 and `vn_epoch` before `vn_purge`.

Per distinct lower file system seen inside a mount (0xf0 bytes, called here a shadow vfs): +0x00 behavior descriptor (private pointer = the mount record), +0x20 next shadow, +0x28 the lower vfs it stands for, +0x30 count of lonodes using it, +0x38 a complete `vfs` record (184 bytes, behavior head at +0x68 of the vfs, so +0xa0 of the shadow). `makelfsnode` fills the type index, the device word (+0x20 of the vfs), the block size (+0x30) and the two identifier words (+0x38, +0x3c) from the lower vfs, and the flag word (+0x1c) from the lower vfs ORed with the mount flags, masked to 3. A lofs vnode whose lower vnode lives on the same file system as the mount's lower root gets the `lofs` vfs itself as `v_vfsp`; a lower vnode on another file system (reached by crossing a mount point below the lower root) gets the shadow vfs for that file system (`lofs_realvfs` maps a vfs back to the lower vfs). Established; why it is done (so that device, identifier and read-only state of the lower file system stay visible) is Inferred.

### 1.3 The `vnodeops` table

The table is 456 bytes at 0xc0000000015ebe00; the position word at +0 is 0x00000101 in its first four bytes, the other four bytes of the 8-byte slot being zero (the same word as the other tables of [network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) section 3.3, which also lists the slot names). Slot index below is the byte offset divided by 8. A routine receives the behavior descriptor of its own vnode in the first argument register (`a0`); the other arguments follow in `a1`, `a2`, ... and the "Argument registers" column gives the highest one the routine reads (`hostfacts`: argument registers read before written), so the first descriptor counts as argument 0. The "Public-interface arguments" column names the arguments with the names the public vnode interface uses and is Inferred; the register count is Established. Every forwarding routine finds the lower vnode as: load the private pointer at +0 of the descriptor (the lonode), load the lower vnode at +0x28, load the first descriptor of the lower vnode's behavior chain from +0x60, load that descriptor's operations table from +0x10 and call the same slot with the lower descriptor in `a0` and the remaining argument registers unchanged. Established. When the global `cell_enabled` (0 in the file) is nonzero the call is bracketed by `bhv_version_read_lock` and `bhv_version_read_unlock` on the lower chain head; the value at run time was not observed.

| Slot | Offset | Operation | Routine, address, size | Argument registers | Public-interface arguments | What it does |
|---:|---:|---|---|---|---|---|
| 1 | +8 | open | `lofs_open` 0xc0000000003c3ea0, 136 | a1..a3 | vpp, mode, cr | forward to lower slot +8; the `vpp` passed on is the caller's, so the lofs vnode stays the open file's vnode |
| 2 | +16 | close | `lofs_close` 0xc0000000003c3f28, 132 | a1..a3 | flag, lastclose, cr | forward |
| 3 | +24 | read | `lofs_read` 0xc0000000003c3fac, 140 | a1..a4 | uio, ioflag, cr, flid | forward |
| 4 | +32 | write | `lofs_write` 0xc0000000003c4038, 140 | a1..a4 | uio, ioflag, cr, flid | forward |
| 5 | +40 | ioctl | `lofs_ioctl` 0xc0000000003c40c4, 164 | a1..a6 | cmd, arg, flag, cr, rvalp, label | forward |
| 6 | +48 | setfl | `lofs_setfl` 0xc0000000003c4168, 132 | a1..a3 | oflags, nflags, cr | forward |
| 7 | +56 | getattr | `lofs_getattr` 0xc0000000003c41ec, 312 | a1..a3 | vattr, flags, cr | lower getattr, then overwrites the 32-bit word at +0x60 of the attribute buffer with the mount's number (section 1.5) |
| 8 | +64 | setattr | `lofs_setattr` 0xc0000000003c4324, 132 | a1..a3 | vattr, flags, cr | forward |
| 9 | +72 | access | `lofs_access` 0xc0000000003c43a8, 124 | a1..a2 | mode, cr | forward |
| 10 | +80 | lookup | `lofs_lookup` 0xc0000000003c4658, 1292 | a1..a6 | name, vpp, pathname, flags, rootdir, cr | section 1.4 |
| 11 | +88 | create | `lofs_create` 0xc0000000003c4b64, 164 | a1..a6 | name, vattr, excl, mode, vpp, cr | forward; the vnode the lower file system returns goes to the caller unwrapped |
| 12 | +96 | remove | `lofs_remove` 0xc0000000003c4c08, 124 | a1..a2 | name, cr | forward |
| 13 | +104 | link | `lofs_link` 0xc0000000003c4cd0, 184 | a1..a3 | source vp, name, cr | maps the directory and the source to non-lofs vnodes; returns 18 when their `v_vfsp` differ, else calls the lower link |
| 14 | +112 | rename | `lofs_rename` 0xc0000000003c4d88, 200 | a1..a5 | old name, target dir, new name, pathname, cr | maps the target directory to its non-lofs vnode; returns 18 when the two `v_vfsp` differ, else calls the lower rename |
| 15 | +120 | mkdir | `lofs_mkdir` 0xc0000000003c4e50, 228 | a1..a4 | name, vattr, vpp, cr | lower mkdir into a local, then `makelonode` wraps the new directory and stores the wrapper in `*vpp` |
| 16 | +128 | rmdir | `lofs_rmdir` 0xc0000000003c4f34, 132 | a1..a3 | name, cwd vp, cr | forward (the `cwd vp` is passed on as is, not mapped to its lower vnode) |
| 17 | +136 | readdir | `lofs_readdir` 0xc0000000003c50c0, 132 | a1..a3 | uio, cr, eofp | forward; no merging, no filtering |
| 18 | +144 | symlink | `lofs_symlink` 0xc0000000003c4fb8, 140 | a1..a4 | name, vattr, target, cr | forward |
| 19 | +152 | readlink | `lofs_readlink` 0xc0000000003c5044, 124 | a1..a2 | uio, cr | forward |
| 20 | +160 | fsync | `lofs_fsync` 0xc0000000003c4424, 140 | a1..a4 | flag, cr, start, stop | forward |
| 21 | +168 | inactive | `lofs_inactive` 0xc0000000003c44d0, 160 | none | cr | section 1.6 |
| 22 | +176 | fid | `lofs_fid` 0xc0000000003c4570, 116 | a1 | fidpp | forward |
| 23 | +184 | fid2 | `lofs_fid2` 0xc0000000003c45e4, 116 | a1 | fidp | forward |
| 24 | +192 | rwlock | `lofs_rwlock` 0xc0000000003c5144, 84 | a1 | mode | forward: the iolock taken is the lower vnode's |
| 25 | +200 | rwunlock | `lofs_rwunlock` 0xc0000000003c5198, 80 | none | mode | forward; `a1` is left as it arrived |
| 26 | +208 | seek | `lofs_seek` 0xc0000000003c51e8, 124 | a1..a2 | old offset, new offset ptr | forward |
| 27 | +216 | cmp | `lofs_cmp` 0xc0000000003c5264, 144 | a1 | other vp | maps both vnodes to their non-lofs vnodes; equal pointers give 1; when either lower vnode has bit 26 of its flag word set, calls `vn_cmp` on the pair; else 0 |
| 28 | +224 | frlock | `lofs_frlock` 0xc0000000003c52f4, 164 | a1..a6 | cmd, flock, flag, offset, vrwlock, cr | forward |
| 29 | +232 | realvp | `lofs_realvp` 0xc0000000003c5398, 140 | a1 | vpp | non-lofs vnode, then the lower realvp; if that fails stores the non-lofs vnode; always returns 0 |
| 30 | +240 | bmap | `lofs_bmap` 0xc0000000003c5424, 164 | a1..a6 | offset, count, flags, cr, bmap array, count ptr | forward |
| 31 | +248 | strategy | `lofs_strategy` 0xc0000000003c54c8, 108 | a1 | buf | forward |
| 32 | +256 | map | `lofs_map` 0xc0000000003c5534, 224 | a1..a6 | offset, length, prot, flags, cr, new-vp ptr | section 5 |
| 33 | +264 | addmap | `fs_nosys` | - | - | not implemented (returns 89) |
| 34 | +272 | delmap | `fs_nosys` | - | - | not implemented |
| 35 | +280 | poll | `lofs_poll` 0xc0000000003c5614, 148 | a1..a5 | events, anyyet, revents ptr, pollhead ptr, generation | forward |
| 36 | +288 | dump | `lofs_dump` 0xc0000000003c56a8, 132 | a1..a3 | addr, blkno, count | forward |
| 37 | +296 | pathconf | `lofs_pathconf` 0xc0000000003c572c, 132 | a1..a3 | cmd, value ptr, cr | forward |
| 38 | +304 | allocstore | `fs_nosys` | - | - | not implemented |
| 39 | +312 | fcntl | `fs_nosys` | - | - | not implemented |
| 40 | +320 | reclaim | `lofs_reclaim` 0xc0000000003c44b0, 32 | none | - | `freelonode` on the lonode; returns 0 |
| 41 | +328 | attr_get | `lofs_attrget` 0xc0000000003c5858, 204 | a1..a5 | name, value, length ptr, flags, cr | forward |
| 42 | +336 | attr_set | `lofs_attrset` 0xc0000000003c5924, 148 | a1..a5 | name, value, length, flags, cr | forward |
| 43 | +344 | attr_remove | `lofs_attrremove` 0xc0000000003c59b8, 132 | a1..a3 | name, flags, cr | forward |
| 44 | +352 | attr_list | `lofs_attrlist` 0xc0000000003c5a3c, 148 | a1..a5 | buffer, size, flags, cursor, cr | forward |
| 45 | +360 | cover | `fs_cover` | - | - | the generic mount procedure (section 2.4) |
| 46 | +368 | link removed | `fs_noval` | - | - | default |
| 47 | +376 | vnode change | `lofs_vnode_change` 0xc0000000003c57b0, 168 | a1..a2 | cmd, value | lower vnode change slot, then `fs_vnode_change` on the lofs vnode |
| 48 | +384 | tosspages | `fs_tosspages` | - | - | generic page routine (section 5) |
| 49 | +392 | flushinval_pages | `fs_flushinval_pages` | - | - | generic |
| 50 | +400 | flush_pages | `fs_flush_pages` | - | - | generic |
| 51 | +408 | invalfree_pages | `fs_invalfree_pages` | - | - | generic |
| 52 | +416 | pages_sethole | `fs_pages_sethole` | - | - | generic |
| 53 | +424 | commit | `fs_nosys` | - | - | not implemented |
| 54 | +432 | readbuf | `fs_nosys` | - | - | not implemented |
| 55 | +440 | strgetmsg | `fs_strgetmsg` | - | - | default |
| 56 | +448 | strputmsg | `fs_strputmsg` | - | - | default |

Forty-one of the 56 slots are `lofs_*` routines. Established. The offsets of the lower slots match the offsets of this table, so a stacked file system that fills the same slots can reuse the number. Established.

Vnode flag bits seen in this section, with their meanings read from the vnode header shipped on the 6.5.30 media (`usr/include/sys/vnode.h`; the header gives a one-line comment per bit, which is the only public statement of them): 0x1000 it is acceptable to mount on top of this file system root; 0x2000 the vnode is torn down at inactive time; 0x100000 root of its file system; 0x200000 the file cannot be used as a virtual swap device; 0x2000000 the vnode has a special compare operation; 0x4000000 the vnode is part of the local cache shared across the machine. Established for the meaning (published comment) and for the uses read above. The earlier open question named 0x200000 as set by the wrapper builder; the code sets 0x2000000 (checked at 0xc0000000003c5cb8 and 0xc0000000003c5e58), which matches the compare operation `lofs` supplies.

### 1.4 `lofs_lookup`: names, `..`, mount points and autofs

`lofs_lookup` (0xc0000000003c4658, 1292 bytes) works as follows. Established unless marked.

1. The directory's own lonode gives the lower directory, and the `lofs` vfs behavior (looked up in the vnode's `v_vfsp` chain with `bhv_lookup_unlocked` against `lofs_vfsops`) gives the mount record.
2. The name "." returns the directory itself: `vn_hold` on the lofs vnode, stored in `*vpp`, result 0, with no lower call.
3. For "..": if the lower directory has bit 20 of its flag word (0x100000) set, the routine climbs through covering vnodes before the lower call. It follows `v_vfsp`, then the vfs's covered vnode (+0x10 of the vfs), while that vnode also has bit 20 set, stopping at the process root directory (the global `rootdir`). So a lower directory that is the root of its own file system continues from the directory it is mounted on. Reading bit 20 as the "root of its file system" flag rests on this use and on `lofs_mount` setting it on the root vnode; no name for it was found in the binary. If the lower directory is itself a lofs vnode the climb and the lower call go to the non-lofs vnode below it (`bhv_lookup_unlocked` against `lofs_vnodeops` in a loop).
4. The lower directory's lookup is called with all arguments passed on (name, a local vnode pointer in place of `vpp`, pathname, flags, root directory, credential). The `lofs` lookup does not translate the `rootdir` argument and does not use or fill the name cache.
5. On success the result goes through `traverse` (0xc0000000002c1f6c, 376 bytes), which follows a file system mounted on the vnode (+0x28 of the vnode) to that file system's root with its `VFS_ROOT` slot (+0x30 of the vfs's operations), releasing the old vnode. So a mount point below the lower root is crossed inside the lookup, in the lower tree. If `traverse` returns an error the routine releases the vnode and returns 0 with `*vpp` left NULL; read as written, whether `VFS_ROOT` can fail here was not determined.
6. If the vnode's type word (+0x20) is not 2 (a directory, the value `xfs_rwlockf` also tests for directories), the lower vnode is stored in `*vpp` as it is and the routine returns 0. Nothing is wrapped.
7. If the vnode the lower lookup found is an autofs vnode (`bhv_lookup_unlocked` on its chain against `autofs_vnodeops` is nonzero) the routine does what a reading process would: it zeroes a 0x48-byte area on its stack, builds a `uio` with one 16-byte kernel buffer in it (segment flag 1), calls the lower `rwlock` (slot +0xc0, argument 1), `readdir` (slot +0x88) and `rwunlock` (slot +0xc8, argument 1), then follows a file system that the autofs mount may have placed on the directory (`traverse` again) before wrapping. The calls are read in full; the purpose (triggering an automount) is a reading.
8. Otherwise `makelonode` (section 1.6) wraps the lower directory: a new lofs vnode, or the existing one for that lower vnode. There is extra handling when the lower vnode's `v_vfsp` is a `lofs` vfs (a lofs mount whose lower tree contains another lofs mount) and when the found vnode is the mount's own lower root; it sets the two fields at +0x38 and +0x54 of the new lonode. Its purpose (probably to stop a lofs mount from recursing into itself) is Unknown; the code was read, not matched to a case.

### 1.5 `lofs_getattr`

`lofs_getattr` (0xc0000000003c41ec, 312 bytes) asks the lower vnode for the attributes and then, on success, stores the mount record's unique number (+0x58) into the word at +0x60 of the attribute buffer. In `xfs_getattr` the word at +0x60 of the buffer is where the device number of a block or character special file goes, so the field is the device number (Inferred). One special case: when the lofs vnode has bit 20 of its flag word set (the root of the mount), the routine walks the chain of covering vnodes as `..` does, and if the file system device of the vfs where the walk ends equals the device of the `lofs` vfs (which `lofs_mount` copied from the lower vfs, so this is true when the mount's lower tree and its mount point are on the same file system) it asks that covering vnode for the attributes instead of the lower vnode. So for such a mount the root answers with the attributes of the directory it covers. Established for the code; the intent is Unknown.

### 1.6 Vnode creation, reference counts and release

`makelonode` (0xc0000000003c5b04, 988 bytes) takes a lower vnode and the mount record, and returns the lofs vnode. It consumes one reference to the lower vnode that the caller holds. Established unless marked.

It hashes the lower vnode's address into one of the eight buckets and searches under the bucket mutex. A hit (Inferred; the branches around the second `vn_get` were read but not every path was traced): `vn_get` takes a reference on the lofs vnode with the recorded vnode map; if the lonode's flag byte (+0x58) is 1 and its hold count is 0 the lower vnode is reacquired with `vn_get` on the map stored at +0x40 and the hold count is incremented atomically; the caller's reference on the lower vnode is dropped with `vn_rele` when the lonode already holds one.

A miss: a lonode comes from the global free list or `kmem_zalloc`, the shadow vfs for the lower vnode's vfs comes from `makelfsnode` (or the mount's own `lofs` vfs if the lower vfs is the mount's lower vfs), `vn_alloc(vfs, type, rdev)` creates the lofs vnode (type copied from the lower vnode's +0x20, `rdev` the mount's number at +0x58 of the mount record), `bhv_insert_initial` installs the lonode's descriptor as the vnode's first behavior, the vnode's flag word is changed with `bitlock_set` (bits 0x2000000, the vnode has its own compare operation, and also 0x2000, the vnode is torn down when it goes inactive, when the lower vnode does not have bit 26 set), and the lonode is inserted into the bucket after a second search (a racing creator's lonode wins and the new one is freed with `bhv_remove_not_first`, a free-list push and `vn_free`).

The hold count at +0x50 starts at 1 and is the number of references the lonode owes the lower vnode. `lofs_inactive` (0xc0000000003c44d0, 160 bytes) swaps it to 0 atomically, and if the flag byte at +0x58 is not 1 (or the lower vnode is NULL) frees the lonode through `freelonode`; it then calls `vn_rele` on the lower vnode as many times as the old count, and returns 1 when it freed the lonode and 0 when it kept it. Returning 0 leaves the lofs vnode cached with no hold on the lower vnode; the stored vnode map (section 1.2) is what lets `makelonode` revalidate it later. That bit 26 of the lower flag word means "this vnode may be referenced weakly through a vnode map" is Inferred from this use and from `lofs_cmp`; its public name was not read.

`lofs_reclaim` always calls `freelonode` (0xc0000000003c6190, 468 bytes): it removes the lonode from its bucket, decrements the mount's lonode count, drops the shadow vfs count (freeing the shadow vfs at zero through `freelfsnode`), puts the lonode on the free list and clears the two flag bits from the vnode with `bitlock_clr`.

`lnode_flush` (0xc0000000003c6364, 360 bytes), called by `lofs_unmount` with the mount record, walks all eight buckets. It returns 0 (busy) if any lonode other than the root's has a lofs vnode with a nonzero count at +0x14. Otherwise it zeroes each such lonode's hold count, calls `vn_purge` on the lofs vnode and releases the lower vnode that many times, and returns 1.

### 1.7 Mount-time identity

`lofs_init` (0xc0000000003c38a0, 68 bytes) is called with a template switch record and the type index (section 2). It stores `lofs_vfsops` at +0x10 of the record, saves the index in `lofsfstype`, sets the counter `lofs_rdev` to 1 and initialises `lofs_rdev_lock` (a spin lock of class "lofsrdev"), then calls `lofs_subrinit` (0xc0000000003c5ad8, 44 bytes), which initialises the free-list mutex. Each `lofs_mount` takes the next value of `lofs_rdev` under that lock as the mount's number. At 0xffffffff the mount fails with 28 after a `cmn_err` of "lofs_rdev overflowed. No new lofs mount points.". Established.

### 1.8 The `vfsops` table and `lofs_mount`

The table is 152 bytes at 0xc0000000015ebd68. The slot list and the offsets are in [network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) section 3.2. A routine's first argument is the behavior descriptor of the vfs; the private pointer at +0 of it is the mount record and the object at +8 is the vfs.

| Slot | Offset | Operation | Routine, address, size | Arguments (registers read) | Notes |
|---:|---:|---|---|---|---|
| 1 | +8 | mount | `lofs_mount` 0xc0000000003c38e4, 704 | a0 vfs descriptor, a1 covered vnode, a2 mount arguments, a4 credential (a3 unused) | section 2.5 |
| 2 | +16 | rootinit | `fs_nosys` | - | |
| 3 | +24 | mntupdate | `fs_nosys` | - | a remount of a `lofs` mount is not supported (Inferred from `fs_cover` calling this slot on a remount) |
| 4 | +32 | dounmount | `fs_dounmount` | a0 descriptor, a1 flags, a2 vnode to release, a3 credential | generic (section 6) |
| 5 | +40 | unmount | `lofs_unmount` 0xc0000000003c3ba4, 328 | a0 descriptor, a1 flags (never read), a2 credential | section 6 |
| 6 | +48 | root | `lofs_root` 0xc0000000003c3cec, 40 | a0 descriptor, a1 vnode out | `vn_hold` on the root vnode (mount record +0x30), store it, return 0 |
| 7 | +56 | statvfs | `lofs_statvfs` 0xc0000000003c3d14, 200 | a0 descriptor, a1 buffer, a2 vnode | lower vfs's statvfs on the same arguments, then stores `lofsfstype` as an 8-byte word at +0x88 of the buffer |
| 8 | +64 | sync | `lofs_sync` 0xc0000000003c3ddc, 196 | a0 descriptor (NULL allowed, returns 0), a1 flags, a2 credential | the lower vfs's sync, so syncing a `lofs` mount syncs the lower file system |
| 9 | +72 | vget | `fs_nosys` | - | file handles cannot be resolved to a `lofs` vnode (section 6) |
| 10 | +80 | mountroot | `fs_nosys` | - | |
| 11 | +88 | realvfsops | `fs_noerr` | - | |
| 12 | +96 | import | `fs_noerr` | - | |
| 13 | +104 | quotactl | `fs_noerr` | - | |
| 14 to 18 | +112..+144 | pinned, magic, version, flags, init_vnode | 0 | - | the ops-magic word is 0, so the generic mount procedure copies no version or flags from this table |

`lofs_realvfs` (0xc0000000003c6148, 72 bytes) returns the lower vfs for a vfs given as the mount's own `lofs` vfs or one of its shadow vfs. Established.

## 2. Registering a file system type

### 2.1 The switch record

`vfssw` (0xc00000000154a798) has 21 records of 48 bytes. The meaning of the fields, from `vfsinit`, `filesys_init`, `fsys_alloc`, `fsys_free`, `vfs_getvfssw` and the records themselves. Established unless marked.

| Offset | Content | Notes |
|---:|---|---|
| +0x00 | pointer to the type name (a C string) | "BADFS" in slot 0, "" in the five free slots |
| +0x08 | init routine | NULL in the free slots; `fsys_alloc` calls a slot free when this is NULL and the name is empty |
| +0x10 | the `vfsops` table | `lofs_init` writes it here |
| +0x18 | the `vnodeops` table | the table of the root vnode's type, as in the static records |
| +0x20 | `dead_vnodeops` (0xc00000000158fb50) in every filled slot, 0 in the free ones | a second vnode table; its use is Unknown (the name suggests the table for vnodes of a dead file system; Inferred) |
| +0x28 | flag word, 0 in all slots | Unknown |

### 2.2 How the table is filled and read

`vfsinit` (0xc00000000028c4a4, 216 bytes) sets `nfstype` (0xc00000000163bdb4) from `vfsmax` (0xc00000000163bdb8). The file image holds 16 in `nfstype` and 21 in `vfsmax`; after `vfsinit` runs both are 21 (the code reads `vfsmax` and stores it in `nfstype`; nothing else writes `nfstype`). [network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) section 3.1 gave 16 for `nfstype`; that is the value in the file, not at run time. `vfsinit` then calls every nonzero init routine in slots 1 to `nfstype`-1 as `init(&vfssw[i], i)`. Established for the code; the run-time value of `nfstype` was not observed.

A type's init is therefore called with the address of its own switch record and its index. `lofs_init` is the example (section 1.7). Established.

`sysfs` (0xc00000000028b2bc, 112 bytes; GETFSIND is function 1) copies a name of up to 16 bytes with `copyinstr`, replaces the name "dbg" with "proc", and scans the slots from 1 below `nfstype` under `vfsswlock` for a non-empty name equal to it; it returns the slot number, or 22 if there is none. A type in a slot between 16 and 20 is found once `nfstype` is 21. Established. `vfs_getvfssw` (0xc00000000028c2f4, 276 bytes) does the same scan, with the same "dbg" alias, for the kernel's own users (the mount helper below, `swapadd`, `utssys`, `vfs_mountroot`).

The mount helper called from `fs_cover` (section 2.4) takes the type either as a small number (below 0x100, nonzero and below `nfstype`) and uses `vfssw[number].vfsops`, or as a user string (copied with `copyinstr`, 16 bytes) resolved by `vfs_getvfssw`. Established.

### 2.3 Adding and removing a type at run time

The module loader's file system kind ([kernel-tracing-and-modules.md](kernel-tracing-and-modules.md) "Module kinds") does the following. Established unless marked.

`fsys_load` (0xc000000000284d70, 120 bytes) takes the module lock, runs `filesys_check`, loads the object with `mloadfile` and calls `filesys_init`.

`filesys_check` (0xc000000000284c84, 236 bytes) refuses an empty name (94) and a name equal to a name already in slots 1 to `vfsmax`-1 (95), and calls `fsys_alloc` (0xc0000000002849a8, 200 bytes), which returns the highest free slot index (it keeps scanning and remembers the last free slot, so the first module type gets index 20) or 0 (the check then returns 50). The index is stored in the module's descriptor at +0x14.

`filesys_init` (0xc000000000284b6c, 280 bytes) runs `mversion_chk`, copies the module name (at most 17 bytes), and resolves four symbols in the module with `mfindname`: `init` (missing: 63), `vfsops` (61), `vnodeops` (62) and `unload` (optional: if it is missing the descriptor's unload pointer at +0x20 is set to 0). It builds a 48-byte record on its stack with the name pointer, the `init` address, the `vfsops` address, the `vnodeops` address, `dead_vnodeops` and 0, calls `init(&record, index)` and ignores its return value (`jalr` result unused), then `filesys_insert` (0xc000000000284adc, 144 bytes) copies the 48 bytes into `vfssw[index]` under `vfsswlock` and registers the module in a 32-bucket table by index. So the init routine of a module can change any field of the record before it is copied, and `lofs_init` shows one writing `vfsops`. The Established part is this order; that the switch record is live from the first mount after the copy is Inferred.

`fsys_unload` (0xc000000000284de8, 184 bytes): refuses if the module has no `unload` routine (60); walks the list from `rootvfs` and refuses (51) if any vfs has a behavior whose operations equal the type's `vfsops`; calls `unload()`, refuses (51) if it returns nonzero; otherwise `fsys_free` (0xc000000000284a70, 108 bytes) clears the init pointer and points the name at the empty string (the `vfsops`, `vnodeops` pointers stay).

The user entry (`syssgi` selector 69, the `mload` command) needs the conditions listed in [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md): a capability, a kernel started from a disk file, and the `mload version 7.0` string in the module. Nothing in the 6.5.22 `master.d` sets the dynamic-load flag for a file system module, which only means no shipped one is loaded this way.

### 2.4 What `mount(2)` does with a type

`fs_cover` (0xc000000000288ab4, 1364 bytes; the `cover` slot of every table that leaves the default) is the generic mount procedure for a mount point. Reading it gives the order of events. Established unless marked.

1. It takes the bit lock in the mount point vnode's flag word (+0x10, bit 8) and refuses with 16 if the vnode has the bit 0x200 (mount in progress) set, if a file system is already mounted on it (+0x28 nonzero), or, unless the arguments carry the remount flag 0x20, if it is the root of a file system (flag bit 0x100000 set and bit 0x1000 clear). Otherwise it sets 0x200.
2. It picks the `vfsops` table from the type argument (section 2.2); without `MS_FSS` (2) or `MS_DATA` (4) in the flags it uses the operations of the root file system's base vfs. If no table is found it returns 22.
3. If `MS_DATA` is not set it zeroes the data pointer and length fields of the arguments. The argument block is the system call's argument record, 8-byte slots: +0 special path, +8 mount point path, +0x10 flags, +0x18 type (number or string), +0x20 data pointer, +0x28 data length. The offsets +0, +0x10, +0x18, +0x20 and +0x28 are read from the code; +8 comes from the public mount interface.
4. For a new mount, `vfs_allocate` makes the vfs, flag bit 1 (read-only) is copied from the arguments to the vfs's flag word (+0x1c), the name cache entries for the mount point are purged, and when the operations table's magic word (+0x78) is 0x18732739 the two words at +0x80 and +0x88 of the table are copied to +0x88 and +0x90 of the vfs. The table's `mount` slot is then called as `mount(vfs descriptor, mount point vnode, arguments, attributes, credential)` (`a0`..`a4`; for a remount the `mntupdate` slot is called with the mount point vnode, the arguments and the credential). The `attributes` word is the third argument of `fs_cover`; `lofs_mount` ignores it.
5. On success `vfs_add` links the vfs (`vfs_add(mount point vnode, vfs, flags)`), the count at +0x24 of the mount point's own vfs goes up by one, the mount point vnode's +0x28 is set to the new vfs and the 0x200 bit is cleared. On failure the 0x200 bit is cleared and the error is returned.

So a file system does not set the covered vnode, the mounted-here pointer or the vfs list link; the generic procedure does. What `lofs_mount` itself fills in the vfs it is given: the behavior descriptor (`vfs_insertbhv`), +0x30 block size, +0x34 type index (`lofsfstype`), +0x38 and +0x3c identifier words and +0x20 device word, all copied from the lower vfs, and +0x40 zeroed; the flag word at +0x1c is read from the lower vfs and not written. Established.

### 2.5 The `lofs` mount arguments

`lofs_mount` (0xc0000000003c38e4, 704 bytes) reads the argument record twice and nothing else: the special path at +0 (a user string, the lower tree) and the flags at +0x10. It never reads or copies the data block. Established. In order:

1. `cap_able_cred(credential, 0x20000000)`; no capability gives 1. (`lofs_unmount` makes the same call; the capability's name was not read.)
2. `lookupname` on the special path, with the user-space segment (0), following symbolic links (1), no directory output, the vnode out. An error is returned as it came. A result equal to the mount point vnode returns 16.
3. The mount record is allocated (288 bytes), the lower vfs recorded, the mount record's flags set from the arguments (1 gives bit 0, 0x10 gives bit 1), then the lower vfs's flag bits 0 and 1 are ORed into the arguments' flags as 1 and 0x10, so the generic code, which reads the arguments' flags again for `vfs_add`, sees a read-only or no-suid lower file system as read-only or no-suid. The mount's number is taken and the mount mutex and eight bucket mutexes are initialised.
4. The vfs's descriptor and identity words are filled (section 2.4), `makelonode` wraps the lower vnode as the root, and bit 20 of the root's flag word (0x100000) is set with `bitlock_set`. If the wrapper already had that bit (the lower vnode already was the root of a lofs mount) the mount returns 16 unless the arguments carry 0x80; the 0x80 flag's public name was not read.

The routine does not check that the lower vnode is a directory. Neither the generic procedure nor the `mount` system call's body (`cmount`, 0xc000000000289eac) refuses a non-directory mount point: `cmount` resolves the mount-point path with `lookupname` (following links), tests only the vnode's flag word (the root-of-file-system bit, with the bit 0x1000 exception and the remount flag 0x20, as in `fs_cover`), and calls the `cover` slot; `fs_cover` reads the flag word at +0x10 and the covering-vfs word at +0x28 of the mount-point vnode and never the vnode type at +0x20. Established by reading both routines in full in the 6.5.30 IP35 kernel; the refusal with 22 after the flag tests is taken when the helper at 0xc000000000289008 returns zero, which maps the type number or string to a switch-table entry (inferred from its callers and its `copyinstr` of a 16-byte name). A non-directory lower path or mount point is therefore left to each file system's own `mount` slot; `lofs_mount` has no such test (above), so a `lofs` mount of a plain file over a plain file is not refused by the shipped generic code. Not run on a guest. Mounting `lofs` needs no `mount` data block, only the type (an index from `sysfs` or the string "lofs"), the lower path in the special-path slot and the mount point path.

## 3. Calling another file system from inside a vnode operation

### 3.1 Calling a vnode operation

The calling pattern is the one every forwarding routine in section 1.3 uses and the one kernel code outside `lofs` uses (`vn_rdwr`, `vn_open`, `execmap` read in this page): the vnode's behavior head is the 8-byte pointer at +0x60 of the vnode; that gives the first behavior descriptor; its operations pointer is at +0x10; the operation is the pointer at the slot offset of section 1.3; it is called with the descriptor as the first argument and the operation's own arguments after it. The descriptor, not the vnode, goes in `a0`; the vnode is the object at +8 of the descriptor. Established. When `cell_enabled` is nonzero each call is wrapped in `bhv_version_read_lock` and `bhv_version_read_unlock` on the head (they are 8-byte stubs in the kernel; the value of `cell_enabled` in the file is 0). The kernel has pass-through stubs for every slot (`vn_passthru_vop_*`, [network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) section 3.3) that skip behaviors whose slot holds the pass-through marker; `lofs` does not use them and calls the first behavior's slot directly. Whether a kernel configured with the DMAPI or CXFS layers puts another behavior ahead of the XFS one on an XFS vnode was not determined; an XFS-only kernel has the XFS behavior first (its operations table starts with the position word 0x00040101 and `xfs_*` routines). Established for what `lofs` and `vn_open` do; the other question is open.

Kernel helpers that wrap the pattern. Sizes and addresses are from the symbol table; the argument roles marked Established were read in the routine or its callers; the others are not claimed.

`vn_rdwr` (0xc000000000290378, 384 bytes): `vn_rdwr(rw, vp, base, len, offset, segflg, ioflag, limit, cr, residp, flid)`. `rw` is 0 for a read and 1 for a write; for a write it first returns 30 if the vnode's `v_vfsp` has flag bit 1 (read-only) set; a negative length gives 22. It builds an iovec and a uio on its own stack (section 3.2), calls the vnode's read slot (+0x18) or write slot (+0x20) with `(uio, ioflag, cr, flid)`, stores the residual count through `residp` when it is not NULL, and returns the operation's result, except that when `residp` is NULL and bytes remain the result is 5. It takes no `rwlock` itself. The roles of the arguments after `limit` (credential, residual pointer, lock identifier) are read from where each one goes. Established. The kernel's exec header read (`exrdhead`) and the module loader's file readers call it.

`vn_open`, `vn_create`, `vn_remove`, `vn_link`, `vn_rename` (0xc0000000002904f8, 0xc0000000002909e8, 0xc000000000291558, 0xc000000000291028, 0xc0000000002912a8; 1264, 1600, 588, 640 and 688 bytes) take path names. `vn_open` tests the open flags 1 (`FREAD`), 2 (`FWRITE`), 0x100 (create), 0x200 (truncate) and 0x400 (exclusive), resolves the path with `lookupname` (follow links) or `vn_create`, and then calls the vnode's slot +0x30 with `(0, flags, cr)`, slot +0x08 (open) with `(&vp, flags, cr)`, for truncate slot +0x40 (setattr) with a buffer whose mask is 0x80 and size 0, in the create branch slot +0x48 (access), and on failure slot +0x10 (close) with `(flags, 1, cr)` and `vn_rele`. It takes the credential from `get_current_cred` each time. Established for those calls; the full argument list of the five helpers was not read.

`lookupname` (0xc0000000002c161c, 116 bytes): `lookupname(path, segflg, followlink, dirvpp, vpp, pathname-out)`; `lofs_mount` calls it with the segment 0 for a user path. A kernel string needs the other segment value; which value was not read.

`vn_hold` (0xc00000000028ff40, 152 bytes) increments the count at +0x14 of the vnode under the flag-word bit lock and returns the vnode. `vn_rele` (0xc00000000028ffd8, 764 bytes) drops it and runs the inactive path at zero. `vn_cmp` (0xc0000000002917a4, 228 bytes) compares two vnodes through their base behaviors and the `cmp` slot.

### 3.2 Kernel-space uio and iovec

`vn_rdwr` builds the structures a caller needs. Offsets, Established from `vn_rdwr`, `uiomove` (0xc0000000002dca04, 392 bytes) and `xfs_read`:

| uio offset | Size | Field |
|---:|---:|---|
| +0x00 | 8 | pointer to the iovec array |
| +0x08 | 4 | number of iovecs (1 here) |
| +0x0c | 4 | open-mode word: 1 for a read, 2 for a write (Inferred name) |
| +0x10 | 8 | file offset |
| +0x18 | 2 | segment flag: 0 user data, 1 kernel data; 2 is handled like 0 by `uiomove` (Inferred names from the public interface; the three values are Established) |
| +0x1a to +0x1f | | zeroed by `vn_rdwr` |
| +0x20 | 8 | residual byte count |
| +0x28 | 8 | the file size limit argument |
| +0x30, +0x38 | 8 each | zeroed |

The uio is 0x40 bytes; an iovec is 16 bytes (base at +0, length at +8). `uiomove(cp, n, rw, uio)` copies `n` bytes between a kernel buffer and the uio (a `rw` of 0 copies to the uio's buffers, nonzero from them; `copyout` and `copyin` for segment 0, `bcopy` for 1), advances the iovec base and length, the uio offset and the residual, and returns 14 on a copy fault. Established. `xfs_read` (0xc00000000037f694, 652 bytes) calls `xfs_check_mapped_io` first when the uio has segment flag 0, one iovec and ioflag bit 4 clear; a kernel caller that wants to avoid that path passes segment 1. Established for the test, Inferred for the consequence.

### 3.3 Credentials

The credential argument is a pointer to a credential record whose first word is a reference count. `get_current_cred` (0xc0000000002d1fc0, 40 bytes) returns the calling thread's credential through an indirect call. `crhold` (0xc0000000002d1c3c, 36 bytes) and `crfree` (0xc0000000002d1c60, 92 bytes) add and drop a reference atomically; `crdup` and `crcopy` copy one. `cap_able_cred(cr, mask)` (0xc000000000332db8, 204 bytes) is the capability test that `lofs_mount`, `lofs_unmount`, `xfs_write` and `xfs_setattr` use. `sys_cred` (0xc0000000016463e0) is a global pointer to a kernel credential that `cred_init` allocates and fills: the count is 1, the word at +0x40 is -2, and the three 8-byte words at +0x28, +0x30 and +0x38 are set to one value read from the data area (a mask of all capabilities, Inferred). `lofs` never builds or changes a credential; every forwarding routine passes the caller's credential on. Whether an XFS operation called with `sys_cred` bypasses its permission checks was not determined. Established for the routines; the rest is Unknown (open question).

### 3.4 What an XFS vnode requires

Read in the `xfs_vnodeops` routines at the addresses of the table (0xc0000000015ea818; position word 0x00040101). The vnode is the object at +8 of the descriptor and the XFS inode begins 0x30 bytes before the descriptor (the routines subtract 0x30 from `a0`). Established unless marked.

Lookup. `xfs_lookup` (0xc0000000003677f0, 484 bytes) takes the directory's iolock shared (`xfs_ilock_map_shared`), checks execute permission with `xfs_iaccess(…, credential)`, then `xfs_dir_lookup_int`, which uses the name cache (`dnlc_lookup_fast`, `dnlc_enter_fast`). It returns the vnode with a reference held; the caller owes a `vn_rele`. If the vnode found is a block or character device or a FIFO, `xfs_lookup` calls `spec_vp` and returns the specfs vnode in its place, so a device node in an XFS directory is not an XFS vnode to the caller.

Read and write. `xfs_read` and `xfs_write` (0xc00000000037f694 and 0xc000000000381444, 652 and 2716 bytes) take `(descriptor, uio, ioflag, cr, flid)`. If bit 0x800 of `ioflag` is clear they take the inode iolock themselves (`xfs_rwlockf`, or `xfs_ilock_nowait` for a mapped-file case); if it is set they assume the caller holds it through the `rwlock` slot. A per-thread "nested locking" state (`xfs_is_nested_locking_enabled`, set by `xfs_enable_nested_locking`) lets a write that faults on the same file through a user buffer proceed. So a caller that does not hold the lock must leave 0x800 clear; a stacked file system that forwards its own `rwlock` to the lower one (as `lofs` does) must pass 0x800 on. `xfs_write`'s call list includes `xfs_check_frozen`, `cap_able_cred`, `xfs_write_clear_setuid` and the log transaction routines (`xfs_trans_alloc`, `xfs_trans_reserve`, `xfs_trans_commit`), so it checks the frozen state and the credential and logs the inode change itself. Names for the ioflag bits are Unknown.

Open and close. `xfs_open` (0xc000000000364e80, 140 bytes) returns 5 if the file system is shut down (flag bit 0x10 at +0x2e4 of the mount structure); for a directory it starts read-ahead of the first directory block when the inode has extents; otherwise 0. It ignores the mode. `xfs_close` (0xc000000000364f0c, 8 bytes) returns 0. So no open-mode flag matters for XFS regular files beyond what the generic layers check.

Attributes. `xfs_getattr` (0xc000000000364f14, 440 bytes) with a mask word in the buffer's first word: type at +4, mode (low 12 bits) at +8, uid at +0xc, gid at +0x10, device at +0x14, node number (8 bytes) at +0x18, link count at +0x20, size (8 bytes) at +0x28, device number at +0x60. These offsets also fit `vn_open`'s create (mask 3, type 1 at +4, mode at +8) and truncate (mask 0x80, size at +0x28) buffers. Mask values 1 (type), 2 (mode) and 0x80 (size) are Established from those two; the other mask bits follow the public interface (4 uid, 8 gid, 0x10 device, 0x20 node, 0x40 link count) and are Inferred.

Create, mkdir, remove, link, rename, setattr. `xfs_create` (0xc0000000003679d4, 292 bytes; 7 argument registers), `xfs_mkdir` (0xc00000000036a768, 2396), `xfs_remove` (0xc0000000003697c8, 1972), `xfs_link` (0xc000000000369f7c, 2028), `xfs_rename` (0xc0000000003a72cc, 3376) and `xfs_setattr` (0xc0000000003652cc, 4504) each start a log transaction (`xfs_trans_alloc`, `xfs_trans_reserve`), take the inode locks they need themselves, do quota work (`xfs_qm_*`), and call `xfs_iaccess` or `cap_able_cred` with the credential given. `xfs_rename` and `xfs_mkdir` update the name cache (`dnlc_remove`, `dnlc_enter`). None of these asks the caller to hold `rwlock`. Read from the call lists; the argument lists beyond the register counts were not decoded.

Behavior and vnode association. `xfs_*` operations find their inode by subtracting 0x30 from the descriptor and read the vnode from +8 of the descriptor, so an XFS descriptor must be the one the XFS file system embedded in its inode; a stacked file system's own descriptor must not be passed to an XFS operation. Established from `xfs_read`, `xfs_open` and `xfs_getattr`.

The XFS `map`, `addmap` and `delmap` slots are `fs_noerr`; the page-cache slots (toss, flush, invalidate) are the generic `fs_*` routines. Pages of an XFS file live on the XFS vnode (section 5).

## 4. Whiteouts, opaque directories and union semantics

None of this exists in the 6.5.30 IP35 kernel or in the kernel object archives held for IP35. Evidence: Established.

The 56 operation slots ([network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) section 3.3) include no whiteout, deletion-marker or union operation, and the file system types in the switch (`xfs`, `umfs`, `lofs`, `autofs`, `nfs`, `nfs3`, `udf`, `efs`, `namefs`, `fd`, `hwgfs`, `fifofs`, `proc`, `pipefs`, `specfs`) are the only ones linked. There is no ufs or svr4 type in the switch.

A search of every archive and object in `usr/cpu/sysgen/IP35boot` for the strings "whiteout", "white-out", "wht", "union mount", "unionfs", "overlay fs", "copyup" and "copy-up" found none. `cachefs.a` (13 members) is in the directory but `cachefs` is not in the switch of the held `unix.IP35` (Inferred: the stub archive `cachefsstubs.a` is the linked variant); its code was not read for negative-entry handling (open question). The kernel also has symbols with a `cfs_` prefix (for example `cfs_vfsops`, `cfs_fstype`); they were not examined.

The vnode layer has one way to say "no such name": the lookup operation returns an error number (which number `xfs_dir_lookup_int`, `efs_dirlookup` and `udf_dirlookup` use for a missing name was not read). There is no distinct value or flag for "deleted in an upper layer", and no sign of a negative name-cache entry: the callers of `dnlc_enter_fast` and `dnlc_lookup_fast` are `xfs_dir_lookup_int`, `efs_dirlookup`, `udf_dirlookup` and the NFS client, and the kernel holds no string about negative entries; whether `dnlc_enter_fast` accepts a NULL vnode was not read (open question).

`autofs` is the only type in the switch whose directory reads have a side effect on the namespace: a `readdir` on an autofs directory triggers a mount, and `lofs_lookup` relies on that (section 1.4, item 7). It does not hide names.

Readdir path for a merging layer. `lofs_readdir` forwards to the lower `readdir` with `(uio, cr, eofp)` and does nothing else, so `lofs` offers no merge. The pieces a layer would use are in the kernel: `uiomove` for filling a uio (section 3.2), `vn_rdwr`-style kernel uio construction, and `fs_fmtdirent` (0xc000000000289d1c, 184 bytes), which formats one directory entry into a buffer: `fs_fmtdirent(buf, inode number, offset cookie, name)` stores the 8-byte inode number at +0, the 8-byte cookie at +8, the 2-byte record length at +0x10 and the name from +0x12, pads with zero bytes to a multiple of 8, and returns the length. It has no direct caller in `unix.IP35` (searched by call instruction), so which file systems use it, and whether the `getdents` system call expects exactly this record, was not read. The offset cookie each file system hands back and accepts (the `uio` offset on re-entry) is private to that file system: no common rule was found, so a layer that merges two streams needs its own cookie scheme. The last point is Inferred from `lofs_readdir` passing the uio's offset straight through and from the XFS and EFS routines being separate codes.

## 5. Page cache, mapping and exec

`lofs` owns no pages. The imports of the four `lofs` objects are all listed here: `autofs_vnodeops`, `bhv_head_destroy`, `bhv_head_init`, `bhv_insert_initial`, `bhv_lookup_unlocked`, `bhv_remove_not_first`, `bhv_version_read_lock`, `bhv_version_read_lock_rv`, `bhv_version_read_unlock`, `bhv_version_read_unlock_rv`, `bitlock_clr`, `bitlock_set`, `bzero`, `cap_able_cred`, `cell_enabled`, `cmn_err`, `fs_cover`, `fs_dounmount`, `fs_flush_pages`, `fs_flushinval_pages`, `fs_invalfree_pages`, `fs_noerr`, `fs_nosys`, `fs_noval`, `fs_pages_sethole`, `fs_strgetmsg`, `fs_strputmsg`, `fs_tosspages`, `fs_vnode_change`, `init_mutex`, `init_spinlock`, `kmem_free`, `kmem_zalloc`, `lookupname`, `mutex_destroy`, `mutex_lock`, `mutex_spinlock`, `mutex_spinunlock`, `mutex_unlock`, `rootdir`, `traverse`, `vfs_insertbhv`, `vn_alloc`, `vn_cmp`, `vn_epoch`, `vn_free`, `vn_get`, `vn_hold`, `vn_purge`, `vn_rele`, plus `lofs`'s own symbols. None of the vnode page-cache routines (`vnode_pfind`, `vnode_pinsert`, `vnode_page_attach` and the others) is imported. Every name in this list is in the kernel's symbol table. Established.

The page-cache slots of the table (toss, flush-and-invalidate, flush, invalidate-and-free, set-hole) are the generic `fs_*` routines, which call the matching `vnode_*` routines on the vnode they are given. They would act on a lofs vnode's own (empty) page set. Because `lofs_lookup` and `lofs_create` return the lower vnode for files, the pages of a file reached through `lofs` are cached once, on the lower vnode. Established for the code; "empty" is Inferred.

`lofs_map` (0xc0000000003c5534, 224 bytes) calls the lower `map` slot with a local vnode pointer initialised to the lower vnode in the place of the new-vnode out argument, and if the call succeeds and the vnode stored there is not the lofs vnode's own object, holds it (`vn_hold`) and stores it in the caller's new-vnode pointer. A caller that maps through a lofs vnode therefore ends up with the lower vnode. `execmap` (0xc0000000002f4c10, 876 bytes) calls the `map` slot of the file's first behavior (+0x100) with a new-vnode out argument, and uses the vnode it gets back (checking its type for block and character devices) for the rest of the mapping, so a layer can substitute the vnode it wants mapped. Established for `lofs_map` and `execmap`. That `mmap(2)` (`mmap`, 0xc0000000002922fc and its static callee) does the same was not read.

`addmap` and `delmap` are `fs_nosys` in `lofs`. The routines that call those slots were not identified (the call sites match by offset only in unnamed code), so whether the generic mapping code calls them for the vnode `map` returned or for the original was not determined (open question). XFS makes all three of `map`, `addmap` and `delmap` no-ops (`fs_noerr`).

Exec on a file that `lofs` returned: the vnode is the lower one, so `exrdhead` (header read through `vn_rdwr`), access, getattr, open and `execmap` all see the lower file system's operations. The vnode's `v_vfsp` is the lower file system's, not the `lofs` vfs; which vfs flags exec consults (for example no-suid) was not read. A layered file system whose file vnodes are its own must implement read (reached by `vn_rdwr` with segment 1 and no 0x800 flag), getattr, access, open and `map`, and `map` must give back a vnode that owns the pages. Inferred from the callers read; the full exec path, including the text-busy test on a mounted-over file, was not read.

The names `fs_tosspages`, `fs_flushinval_pages`, `fs_flush_pages`, `fs_invalfree_pages` and `fs_pages_sethole` (0xc0000000002897f4, 0xc0000000002898b0, 0xc000000000289970, 0xc000000000289a44, 0xc000000000289a98) and the `vnode_*` routines they call are in the symbol table; the page ownership model (the pfdat fields) was not read. Page frames are looked up by `(vnode, offset)` with `vnode_pfind`, `vnode_pinsert` and `vnode_premove` (Inferred from the names and the argument registers; not read in full), which is why pages cached under the lower vnode are not visible under a wrapper.

## 6. File handles, NFS export and unmount

`fid` and `fid2` forward to the lower vnode (`lofs_fid` 0xc0000000003c4570, `lofs_fid2` 0xc0000000003c45e4), and the `lofs` vfs copies the lower vfs's identifier words (+0x38, +0x3c), so a handle built for a `lofs` directory carries the lower file system's identifier and a lower file identifier. Established for the forwarding; "carries" is Inferred. `lofs`'s `vget` slot is `fs_nosys`, so the file system cannot turn a handle into a `lofs` vnode. Whether the NFS server finds the file system for a handle through the identifier words (and so resolves a handle to the lower file system's own `vget`) or through the `lofs` vfs was not read (open question). The routines that build NFS handles (`makefh`, `makefh3`) reach the `fid` slot (the slot offset matches in those routines; Inferred).

Unmount. `lofs_unmount` (0xc0000000003c3ba4, 328 bytes) never reads its flags argument, so unmount and forced unmount behave the same inside `lofs`. It requires the capability (1 otherwise), then `lnode_flush` (busy: 16), then, under the mount mutex, that the mount's lonode count is exactly 1 and the root vnode's count (+0x14) is exactly 1 (busy: 16). It then releases the root, calls `vn_purge` with the root's saved map, removes the mount's descriptor from the vfs chain (setting the chain head to the next descriptor, or `bhv_remove_not_first`), destroys the mutexes, frees the mount record and returns 0. Established.

The generic part, `fs_dounmount` (0xc0000000002890c0, 372 bytes, the `dounmount` slot), takes the vfs lock (`vfs_lock_offline`), releases the extra vnode reference it was given, purges the name cache for the vfs (`dnlc_purge_vfsp`) and the NFS client caches (`nfs_purge_vfsp`), calls the base behavior's `sync` slot with flags 5, then its `unmount` slot with `(flags, credential)`, and on success removes the vfs (`vfs_remove`), decrements the mount point's vfs count and calls `imon_event` when the file monitor is enabled. Established. A directory of a `lofs` mount held as a process's current directory or an open file keeps the lofs vnode's count above zero, and `lnode_flush` then returns busy, so such an unmount fails with 16 whether or not force was asked. Inferred (the force flag is not read inside `lofs`; the system call layer was not read).

`lofs_unmount` frees the mount record; the shadow vfs records are freed by `freelonode` when their count reaches zero. Whether every shadow vfs is gone by the time the mount record is freed (`lnode_flush` purges the vnodes, and `lofs_reclaim` runs later) was not checked.

## 7. Routine index

All in `6.5.30/unix.IP35`. Most routines of the `lofs` archive members are static and are not in the kernel's symbol table (the vnode and vfs operations, `makelfsnode`, `freelfsnode`, `lfsfind`); their addresses are in the tables of sections 1.3 and 1.8. `lofs_init`, `lofs_subrinit`, `makelonode`, `freelonode`, `lnode_flush`, `lofs_realvfs` and `lofs_icrash` are global symbols. The module-loader routines `fsys_load`, `fsys_unload`, `fsys_alloc`, `fsys_free`, `filesys_check`, `filesys_init` and `filesys_insert` are static as well; their addresses and sizes are those of [network-dataplane-and-vfs.md](network-dataplane-and-vfs.md) section 3.1, from the kernel's debug records, and the bodies at those addresses were read for this page. The claim checker works on the entries below; the tables above were compared by hand.

- `lofs_init` (`6.5.30/unix.IP35`, 0xc0000000003c38a0, 68 bytes) stores `lofs_vfsops` in the switch record and starts the free-list mutex; calls `init_spinlock` and `lofs_subrinit`. Established.
- `lofs_subrinit` (`6.5.30/unix.IP35`, 0xc0000000003c5ad8, 44 bytes) initialises the free-list mutex; calls `init_mutex`. Established.
- `makelonode` (`6.5.30/unix.IP35`, 0xc0000000003c5b04, 988 bytes) returns the lofs vnode for a lower vnode; calls `mutex_lock`, `mutex_unlock`, `makelfsnode`, `vn_alloc`, `bhv_insert_initial`, `bitlock_set`, `vn_get`, `vn_rele`, `bhv_remove_not_first`, `vn_free`, `kmem_zalloc` and `bzero`. Established.
- `lofs_realvfs` (`6.5.30/unix.IP35`, 0xc0000000003c6148, 72 bytes) maps a lofs or shadow vfs to the lower vfs. Established.
- `freelonode` (`6.5.30/unix.IP35`, 0xc0000000003c6190, 468 bytes) removes a lofs node from its bucket and frees it; calls `bhv_lookup_unlocked`, `mutex_lock`, `mutex_unlock`, `bitlock_clr`, `lofs_realvfs`, `freelfsnode` and `bhv_remove_not_first`. Established.
- `lnode_flush` (`6.5.30/unix.IP35`, 0xc0000000003c6364, 360 bytes) purges the unreferenced lofs vnodes of a mount; calls `mutex_lock`, `mutex_unlock`, `vn_purge` and `vn_rele`; returns 0 or 1. Established.
- `lofs_icrash` (`6.5.30/unix.IP35`, 0xc0000000003c5ad0, 8 bytes) is the debugger hook object of the file system. Unknown beyond its size.
- `vfsinit` (`6.5.30/unix.IP35`, 0xc00000000028c4a4, 216 bytes) sets `nfstype` from `vfsmax`, initialises the vfs locks and the name cache, and calls each type's init; calls `spinlock_init`, `kmem_zone_init`, `vn_init` and `dnlc_init`. Established.
- `sysfs` (`6.5.30/unix.IP35`, 0xc00000000028b2bc, 112 bytes) is the `sysfs` call body; reads `nfstype`. Established.
- `fs_cover` (`6.5.30/unix.IP35`, 0xc000000000288ab4, 1364 bytes) is the generic mount procedure; calls `mutex_bitlock`, `vfs_allocate`, `dnlc_purge_vp`, `vfs_add`, `vn_rele`, `bitlock_clr`, `mutex_spinlock` and `mutex_spinunlock`. Established.
- `fs_dounmount` (`6.5.30/unix.IP35`, 0xc0000000002890c0, 372 bytes) is the generic unmount procedure; calls `vfs_lock_offline`, `dnlc_purge_vfsp`, `nfs_purge_vfsp`, `vfs_unlock`, `vfs_remove` and `vn_rele`. Established.
- `vfs_insertbhv` (`6.5.30/unix.IP35`, 0xc00000000028c6dc, 44 bytes) stores the private pointer, object and operations in a descriptor and installs it as a vfs's first behavior; calls `bhv_insert_initial`. Established.
- `traverse` (`6.5.30/unix.IP35`, 0xc0000000002c1f6c, 376 bytes) follows a file system mounted on a vnode to its root; calls `mutex_bitlock`, `nested_spinlock`, `nested_bitunlock`, `mutex_spinunlock`, `sv_wait`, `vfs_unbusy`, `vn_rele` and `mutex_bitunlock`. Established.
- `vn_rdwr` (`6.5.30/unix.IP35`, 0xc000000000290378, 384 bytes) reads or writes a vnode through a stack uio; calls `bhv_version_read_lock` and `bhv_version_read_unlock`. Established.
- `uiomove` (`6.5.30/unix.IP35`, 0xc0000000002dca04, 392 bytes) copies between a buffer and a uio; calls `copyout`, `copyin` and `bcopy`. Established.
- `fs_fmtdirent` (`6.5.30/unix.IP35`, 0xc000000000289d1c, 184 bytes) formats one directory entry; calls `strcpy` and `strlen`. Established.
- `get_current_cred` (`6.5.30/unix.IP35`, 0xc0000000002d1fc0, 40 bytes) returns the thread's credential through an indirect call. Established.
- `execmap` (`6.5.30/unix.IP35`, 0xc0000000002f4c10, 876 bytes) maps an executable; calls `get_current_cred` and `vn_rele`. Established.
- `xfs_read` (`6.5.30/unix.IP35`, 0xc00000000037f694, 652 bytes) calls `xfs_is_nested_locking_enabled`, `xfs_read_core`, `xfs_ilock_nowait`, `xfs_rwlockf`, `xfs_rwunlockf` and `xfs_check_mapped_io`. Established.
- `xfs_lookup` (`6.5.30/unix.IP35`, 0xc0000000003677f0, 484 bytes) calls `xfs_ilock_map_shared`, `xfs_iaccess`, `xfs_dir_lookup_int`, `xfs_iunlock_map_shared`, `spec_vp` and `vn_rele`. Established.
