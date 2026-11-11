# XVM on-disk format

The label and metadata that SGI's XVM volume manager (the successor to XLV) keeps on each disk, recovered from the shipped IRIX 6.5.30 kernel module and the shipped Linux ProPack kernel modules. No public document describes it. This page is binary reverse engineering only: no IRIX source, headers or comments were read. Names for fields are the ones the shipped Linux ProPack debug info already uses for the same records; IRIX symbol names are quoted where a routine is cited.

Companion files, same directory:

- `xvm-disk.h`: the structures below as a C header (compiles with `gcc -std=c11 -fsyntax-only`; sizes are asserted).
- `XVM_DECODE.h` (repository root): the same structures in the publish-safe decode-header form, with generated Rust under `generated/disk-formats/`. `defs.toml` ties its constants and field positions to the tables on this page, so the two cannot drift apart.
- `xvm-check.py`: re-derives the checkable facts from the binaries (`uv run --script xvm-check.py XVM_LABEL_O SGI_XVM_KO`). Passes 21 checks on ProPack 4 GA, SP2 and SP4 modules; run against the ProPack 6 module it stops at the first table lookup because that release replaced the swap tables with code (see [Linux ProPack differences](#linux-propack-differences)).

## Grades

- **Established**: the routine body that reads or writes the field was read in the named binary, or the shipped Linux debug info declares it and the same offset is confirmed by IRIX instructions.
- **Inferred**: one source only (a help string, a name, a single-sided access) or a conclusion drawn from established facts. The text says what would settle it.
- **Unknown**: not recovered. What was tried is stated.

## Evidence base

| Short name | Binary | Notes |
|---|---|---|
| IRIX kernel | `6.5.30/usr/cpu/sysgen/IP35boot/xvm.a` (members `xvm_label.o`, `xvm_pal.o`, `xvm_gpt.o`, `xvm_user.o`, `xvm_build.o`, `xvm_serverpalio.o`; sha256 `0b940b1e...f037456`) | MIPS big-endian. The IP27 and IP30 archives have byte-identical text for the routines cited; IP32 differs only in ABI (n32). |
| IRIX tool | `6.5.30/sbin/xvm` (stripped, n32) | Help text and error strings only; it never builds a label itself (no use of either magic). |
| PP4 kernel | ProPack 4 `sgi-xvm.ko`, ia64 little-endian, with DWARF: GA (2.6.5-7.139), SP2 (.191), SP4 (.308) | The structure definitions below come from its DWARF. Layouts are identical in all three. |
| PP4 kerntypes | `sgi-xvm-kerntypes-2.6.5-7.308-debug` | Preprocessor macros (magics, versions, limits). |
| PP4 tool | `xvm-cmds-4-sgi404r1.ia64.rpm`, `/sbin/xvm` | Strings and help only. |
| PP6 kernel | ProPack 6 SP3 `sgi-xvm-standalone-kmp-default-2.6.16.60_0.21-sgi603r1.ia64.rpm`, `sgi-xvm.ko`, with DWARF | A newer physical label; see [Linux ProPack differences](#linux-propack-differences). |

No tool in this environment disassembles IA-64, so every Linux fact is from DWARF types, data tables, symbol and relocation tables, macros and strings, not from instructions. All instruction-level reads are IRIX (MIPS), through Capstone.

No held disk image carries an XVM label, so nothing here was checked against real on-disk bytes. A reader should treat the first real image as the discriminating test for every **inferred** line.

## Byte order

All multi-byte integers on disk are **big-endian**, on both platforms. **Established.**

- IRIX stores the label-header magic with a single word store of `0x6c646872` (`xvm_labent_init`, `xvm_label.o` +0x2844..0x284c) on a big-endian CPU, so the bytes on disk are `6c 64 68 72`. The physical-label magic `0x786c6162` is compared with `strncmp` against the four-byte string `xlab` (`xvm_find_master_sector`, `xvm_pal.o` +0x1c40..0x1c64; the string is at `.rodata` +0x248), so the bytes are `78 6c 61 62`, "xlab".
- The Linux module carries a byte-swap description for every on-disk structure: `xvm_physlab_endian`, `xvm_labelhdr_endian`, `xvm_disk_ve_endian`, `xvm_disk_attr_endian`, per-type union tables and `xvm_last_region_endian` in `.data`, applied by `xvm_xlate_physlab`, `xvm_xlate_labelhdr`, `xvm_xlate_diskve`, `xvm_xlate_diskattr` and `xvm_xlate_lastreg` (all via `CxfsEndian`). Relocations show `xvm_xlate_cacheline` applies the first three to every label sector read and `xvm_labdisk_commit` and `xvm_scan_done` apply `xvm_xlate_physlab`. A little-endian host therefore swaps to and from the same big-endian disk image.
- **UUIDs and name strings are never swapped.** In every table the 16-byte UUID, the 32/48-byte strings and the pad runs are opaque skips. A reader must compare UUIDs as 16 raw bytes. Established.

The Administrator's Guide quotes the physical-label magic as "balx". That is the same four bytes read as a little-endian 32-bit value and printed as characters (`0x786c6162` printed least-significant byte first is `b a l x`). The IRIX tool prints it from a big-endian integer as `xlab` (format `Magic: 0x%x (%c%c%c%c)`). The byte sequence at the start of the physical-label sector is `78 6c 61 62`. **Inferred** from the two sides; a hex dump of one real label would confirm.

## Where the labels live

A labelled disk ("physvol") has a physical label, a label area and a user area.

| Item | Where | Grade |
|---|---|---|
| Physical label (physlab) on a disk with an SGI volume header (DVH) | device block 1 (the second 512-byte block of the whole-disk device). The constant is `XVM_PHYSLAB_START = 1` (PP4 macros); `xvm_find_master_sector` compares the first four bytes at one device block into the buffer with `xlab` and reports master sector 1 (`xvm_pal.o` +0x1c40..0x1ccc). | established |
| Physical label on a GPT disk | the first block of the first partition in the GPT partition list, and only if the list has a second entry. `xvm_find_master_sector` sees `EFI PART` at block 1, reads the partition list (`xvm_gpt.o`: node holds first LBA at +0, last LBA at +8, next at +16), multiplies the device block size by the first partition's first LBA and compares `xlab` there, and reports that first LBA as the master sector (`xvm_pal.o` +0x1cd4..0x1dc8). With a first partition starting at LBA 34, as a standard GPT leaves it, the physlab is at LBA 34 and the label area begins at LBA 35, which agrees with the guide's "label area at block 35 on GPT". | established for the rule; the LBA 34 example is inferred |
| Label area | `pl_xvmlabelblks` consecutive device blocks starting at block `pl_xvmlabel`; every label I/O adds the label-sector index to `pl_xvmlabel` (`xvm_cacheline_refill` +0x1720; `xvm_labdisk_commit` +0x2190; `xvm_serverpal_io` +0x28). The tool reports `pl_xvmlabel` as `volhdrblks` (`xvm_physvol_info`, `xvm_user.o` +0x2954), so the label area starts immediately after the volume header. Defaults: 512 blocks on system disks, 1024 on option disks (tool help); the guide's examples use 3072 or 4096 as the start on DVH. | established for the formula; guide numbers not checked here |
| User area | data block `n` of a slice is device block `pl_dbn + n`. `xvm_serverpal_io` adds `pl_dbn` to data I/O and rejects label I/O at or beyond `pl_dbn` ("attempt to issue i/o beyond XVM label area"). `pl_dbn` must be at least `pl_xvmlabel + pl_xvmlabelblks`; otherwise the label is ignored ("label area extends into user area", `xvm_create_pal_label` +0xa64..0xaa4). | established |
| Device the blocks are relative to | the physvol's `volume/block` node under its disk, which on a DVH disk is the whole-disk partition that starts at block 0. | inferred (`xvm_read_physlab` traverses `volume/block`; the partition table was not read) |

Address and size units: `pl_xvmlabel`, `pl_xvmlabelblks`, `pl_dbn` are 16-bit and count device blocks; `pl_nblks` counts device blocks of the whole disk. For 512-byte disks that is the 512-byte block the tool uses everywhere. Behaviour on larger device blocks was not recovered. **Unknown.**

## Physical label (`xvm_physlab`, 124 bytes, version 2)

| Off | Size | Field | Meaning | Evidence | Grade |
|---:|---:|---|---|---|---|
| 0 | 4 | `pl_magic` | `0x786c6162`, bytes `78 6c 61 62` | `xvm_pal.o` +0x26cc compares; PP4 macro `XVM_PHYSLAB_MAGIC` | established |
| 4 | 2 | `pl_version` | 2. Version 1 is converted on read; a label of any other version is ignored ("XVM label version %d instead of version %d") | `xvm_physlab_fixup` +0x26dc; message at `.rodata` +0x250; PP4 `XVM_PHYSLAB_VERSION 2` | established |
| 6 | 1 | `pl_state` | bitmask. 0x01 online, 0x02 spare, 0x04 media error, 0x08 recovering, 0x10 private (local) domain, 0x20 cluster domain, 0x40 draining. The fixup tests `& 0x30`; if neither domain bit is set it sets 0x10 (cluster services absent) or 0x20 | enum from PP4 DWARF; domain bits from `xvm_physlab_fixup` +0x26e8..0x2750 | domain bits established; others inferred (names only) |
| 7 | 1 | `pl_flags` | 0x02 bootable, 0x04 duplicate, 0x08 sync. `xvm_create_pal_label` keeps `& 6` as persistent and copies bit 0x08 separately | +0xb28, +0xdc4; enum from PP4 DWARF | established |
| 8 | 32 | `pl_hostname` | host that owns the label (private domain). In version 1 these bytes held the disk name; the fixup copies 32 bytes from here to +72 and clears this field | `xvm_physlab_fixup` +0x2758..0x2784 | established |
| 40 | 16 | `pl_uuid` | physvol UUID, copied unchanged into the in-memory physvol | `xvm_create_pal_label` +0xb88..0xb98 | established |
| 56 | 2 | `pl_secsize` | sector size in units of 512 bytes (1 for a 512-byte disk). The tool reports bytes as this value shifted left 9; the label cache unit is this value times 512 | `xvm_physvol_info` +0x2918..0x2920; `xvm_cacheline_refill` +0x1710..0x171c; `xvm_slice_check` aligns slice start and length to it. Zero is rejected ("zero sector size") | established |
| 58 | 2 | `pl_xvmlabel` | first block of the label area; equals the volume header size | see above | established |
| 60 | 2 | `pl_xvmlabelblks` | length of the label area in blocks | `xvm_create_pal_label` +0xb74; the commit routine rewrites it from the cache size | established |
| 62 | 2 | `pl_dbn` | first block of the user area | see above | established |
| 64 | 4 | `pl_nblks` | size of the whole disk in blocks; becomes the physvol size | +0xb68..0xb6c | established |
| 68 | 4 | `pl_timestamp` | last label update, seconds; the commit routine stamps it on every label write; the tool prints it as "last update" | `xvm_labdisk_commit` +0x22a8..0x22ac; tool string | seconds since the epoch is inferred |
| 72 | 48 | `pl_diskname` | the physvol's name, NUL padded | `xvm_physlab_fixup` +0x27c4; tool string "Disk Name" | established |
| 120 | 4 | `pl_clusterid` | cluster the disk belongs to; zeroed when a version 1 label is converted | +0x276c; tool string "Clusterid" | established (meaning of the value inferred) |

The Linux swap table for this record is `[4, 2, 1, 1, skip 32, skip 16, 2, 2, 2, 2, 4, 4, skip 48, 4]` = 124 bytes, which matches every offset above.

## Label area

The label area is an array of label sectors, each `pl_secsize * 512` bytes, sector `i` at device block `pl_xvmlabel + i`. A sector holds one header and then fixed-size entries of a single kind. Established from `xvm_labcache_fill`, `xvm_labent_alloc` and `xvm_labent_init`.

### Label sector header (`xvm_labelhdr`, 8 bytes)

| Off | Size | Field | Meaning | Grade |
|---:|---:|---|---|---|
| 0 | 4 | `lh_magic` | `0x6c646872`, bytes `6c 64 68 72` ("ldhr"). A sector without it is skipped. | established (`xvm_labent_init` +0x2844; fill compares at +0xce0) |
| 4 | 2 | `lh_version` | 2. A version 1 sector is accepted: if its type is 2 it is kept and relabelled version 2 in memory; any other type is discarded. So version 1 sectors hold virtual-element entries only; attribute sectors begin at version 2. A sector of any other version is skipped. | established |
| 6 | 1 | `lh_type` | 0 free, 1 skipped by the reader (purpose not recovered), 2 virtual-element entries, 3 attribute entries. Values of 4 and above are skipped. | established for 0, 2, 3; unknown for 1 |
| 7 | 1 | `lh_size` | entry size; the reader only accepts 72 (0x48) for types 2 and 3 | established (`xvm_labcache_fill` +0x1094, +0x10c0) |

Entries follow back to back from offset 8, `lh_size` bytes each: `(sector size - 8) / 72` of them, which is 7 in a 512-byte sector. `xvm_labent_alloc` zeroes whole entries while at least one more fits. The bytes after the last entry are unused. An entry is free when its first byte (the element or attribute type) is 0. Established.

There is no checksum, generation counter or sequence number on the sector header or on the entries. The only checksum table in the Linux module (`cksum_endian`, 128 words) belongs to the volume header; no checksum call appears in the IRIX label routines. **Established as absent** from the label routines read (`xvm_label.o` has no checksum symbols or calls); a hostile or torn write is detected only by the magic, version and size checks. The per-entry `tid` (below) is the closest thing to a generation and its ordering rule was not recovered.

### Virtual-element entry (`xvm_disk_ve`, 72 bytes, sector type 2)

Each entry describes one **piece** of one object: object `dve_uuid`, child `dve_childuuid` at position `dve_pos` of `dve_pieces`. An object with N children has N entries. Established: `xvm_mem2dsk_ve` (`xvm_label.o` +0x28dc) fills `pos` from its position argument and `childuuid` from the per-piece UUID in the in-memory element; Linux DWARF gives the names and the 72-byte size.

| Off | Size | Field | Meaning | Evidence | Grade |
|---:|---:|---|---|---|---|
| 0 | 1 | `dve_type` | object type, see enum below. The reader accepts 1..7, 11 and 12 and ignores anything else | `xvm_labcache_fill` +0xf98..0xfb8 | established |
| 1 | 1 | `dve_state` | bit 0x10 disabled. For mirror pieces, bits 0x01 clean and 0x02 valid are taken from the piece's state; a mirror piece whose child is absent is stored as 0 | `xvm_mem2dsk_ve` +0x290c..0x2924, +0x29ac..0x29cc | established |
| 2 | 2 | `dve_pieces` | number of pieces of this object | +0x2958..0x295c | established |
| 4 | 4 | `dve_tid` | transaction id | +0x28e4 | established (role inferred: the operation that last wrote the record) |
| 8 | 16 | `dve_uuid` | this object's UUID | +0x2930..0x2938 | established |
| 24 | 16 | `dve_childuuid` | UUID of the child at `dve_pos`. For a slice the child is the physvol, so this is expected to equal `pl_uuid` of the disk that holds it | +0x2940..0x2954; the slice-to-physvol link is in `params2ve`, `xvm_user.o` +0x2e30..0x2e5c | offsets established; slice case inferred |
| 40 | 8 | `dve_length` | length of the object. The tool states lengths in 512-byte blocks | +0x2960..0x2964 | offset established; unit inferred |
| 48 | 2 | `dve_pos` | this record's child position, counted from 0 | +0x28dc | established |
| 50 | 2 | `dve_cowfirst` | copy-on-write first region (COW objects only) | +0x2a5c, +0x2b74 | established offset; role inferred |
| 52 | 2 | `dve_cownext` | copy-on-write next region | +0x2a64 | as above |
| 54 | 2 | `dve_cowregsz` | copy-on-write region size | +0x2a6c | as above |
| 56 | 16 | `dve_u1` | per-type union, below | jump table in `xvm_mem2dsk_ve` (`.rodata` +0x158, 13 entries) | established |

Object type enum (`dve_type`; matches the IRIX jump-table indexes and the PP4 DWARF): 0 free, 1 slice, 2 concat, 3 stripe, 4 raid, 5 mirror, 6 subvolume, 7 volume, 11 copy-on-write, 12 snapshot. Values 8, 9, 10 are the in-memory physvol, unlabelled-disk and foreign types and never appear in an entry. Established.

Union at offset 56, by type. The IRIX routine writes the bytes of each case at the stated offsets; the Linux swap table `xvm_veu_*_endian` gives the widths.

| Type | Off in union | Size | Field | Meaning | Grade |
|---|---:|---:|---|---|---|
| slice | 0 | 8 | `vsl_startblk` | start of the slice in the user area, in 512-byte blocks relative to `pl_dbn` (the tool's `-start` defaults to 0 and `-all` means the whole data area) | offset established; base and unit inferred |
| slice | 8 | 2 | `vsl_seq` | slice sequence number | established offset; role inferred |
| slice | 10 | 1 | `vsl_flags` | bits 0..2 are set from three request flags at slice creation (`params2ve`, `xvm_user.o` +0x2de4..0x2ea0). The tool maps root, swap and usr slices to volume-header partitions, so the three bits are presumably those types; the bit-to-type assignment was not recovered | bits established; names unknown |
| slice | 11 | 1 | `vsl_partition` | the volume-header partition number the slice is mapped to (see [Slices and partitions](#slices-and-partitions)) | offset established; meaning inferred |
| stripe | 0 | 4 | `vs_susize` | stripe unit in blocks (the tool's default is 128) | offset established; unit inferred |
| concat | - | - | none (16 pad bytes) | | established |
| mirror | 0 | 8 | `vm_oldlength` | length before the last resize | established offset; role inferred |
| mirror | 8 | 2 | `vm_logfreq` | | established offset |
| mirror | 10 | 1 | `vm_policy` | 0 round-robin, 1 sequential | established |
| mirror | 11 | 1 | `vm_config` | bit 0 is stored only on the primary piece's record: the routine clears it on any other position | bit 0 inferred |
| raid | 0 | 4 | `vr_susize` | stripe unit | Established that IRIX writes nothing for type 4: entry 4 of the jump table in `xvm_mem2dsk_ve` (`.rodata` +0x158 of `xvm_label.o`, 6.5.30 IP35boot `xvm.a`, sha256 `0b940b1e...f037456`) targets 0x29a0, the same exit the free, concat and snapshot entries use. Name, offset and width come only from the Linux module's structure (a hint, not evidence for IRIX); the role is inferred from the name |
| raid | 4 | 1 | `vr_layout` | layout of the raid set | Same as `vr_susize`: nothing is written by IRIX (established), offset and width from the Linux structure only (hint), role inferred |
| copy-on-write | 0 | 16 | `vcw_ruuid` | UUID of the repository object | offsets established; role inferred |
| subvolume | 0 | 1 | `vsv_svtype` | 0 all, 1 log, 2 data, 3 realtime, 4 swap, 16 user | established (Linux enum; IRIX stores the byte unchanged, +0x2998) |
| volume | 0 | 1 | `vv_repfull` | replication-full flag | offset established; role inferred |

Unused union bytes are padding (the swap tables treat them as opaque). The remaining bytes of a free or unused entry are not defined; zero is what the allocator writes.

### Attribute entry (`xvm_disk_attr`, 72 bytes, sector type 3)

Names and permissions are separate entries tied to an object by UUID. Established from `xvm_mem2dsk_attr` (`xvm_label.o` +0x2bac) and the Linux DWARF.

| Off | Size | Field | Meaning | Grade |
|---:|---:|---|---|---|
| 0 | 1 | `da_type` | 0 free, 1 name, 2 permission. The reader accepts only 1 and 2 | established |
| 1 | 1 | `da_flag` | bit 0 set when the name is persistent (user-assigned); written only when the object's flag at in-memory +0x2c bit 0x10 is set | offset established; meaning inferred (the tool says created volume names are "persistent") |
| 2 | 2 | pad | | established |
| 4 | 4 | `da_tid` | transaction id | established |
| 8 | 16 | `da_uuid` | the object being named or protected (a virtual-element UUID or a physvol UUID) | established |
| 24 | 48 | name, or permission block | for a name: NUL-padded string of up to 48 bytes (`LAB_VE_NAME_SIZE`). For a permission: `int32 uid` at 24, `int32 gid` at 28, `uint32 mode` at 32, then 36 bytes padding. Defaults on volumes are uid 0, gid 0, mode 0600 (PP4 macros). | established |

A physvol's own name is not an attribute entry; it is `pl_diskname` in the physical label.

## How the objects fit together (reader outline)

**Inferred**, from `xvmbuild_add` dispatching sector type 2 to the element builder and type 3 to the attribute builder, the per-piece layout of `xvm_mem2dsk_ve`, and the tool's object model. Nothing here has been run against a real disk.

1. Read the physical label (DVH: block 1; GPT: first partition's first block). Require `xlab` and version 2.
2. Read the `pl_xvmlabelblks` label sectors. Keep type 2 and type 3 sectors whose header and `lh_size` are valid; skip free entries.
3. Collect type-2 entries from every disk of the volume set. Group by `dve_uuid`; the group is one object of type `dve_type` with `dve_pieces` children; entry `dve_pos` gives the child `dve_childuuid` at that position. A child UUID is either another object's `dve_uuid` or, for a slice, a physvol's `pl_uuid`.
4. The tree is: volume (7) holds subvolumes (6); a subvolume holds one child, usually a mirror (5), stripe (3), concat (2) or slice (1); mirror, stripe and concat hold further such objects; leaves are slices. The tool imposes a maximum depth of 10 (`XVM_MAXDEPTH`) and up to 8 mirror legs (`XVM_MIRROR_MAX`). A subvolume's role (log, data, real-time) is `vsv_svtype`.
5. Attach names and permissions from type-3 entries by `da_uuid`.
6. A slice's data lives at device blocks `pl_dbn + vsl_startblk` for `dve_length` blocks on the disk whose `pl_uuid` equals the slice's child UUID. Stripe and concat objects map their children in order of `dve_pos`; a stripe interleaves in `vs_susize`-block units. A mirror's pieces hold the same data; `dve_state` per piece says whether the piece is clean and valid.

Which of several copies of the same object entry wins when disks disagree (for example after a failed update) depends on `dve_tid` and the state bits; the ordering rule was not recovered. **Unknown.** Leads from reading the 6.5.30 kernel's label merge (`xvm_build.o` in `IP35boot/xvm.a`): `xvmbuild_addve` (0x22d8) hands each element entry to `xvmb_add_piece` (0x1004, 1312 bytes), which copies the entry's fields, `dve_tid` among them, into the in-core piece record on several paths and reports duplicates only by message ("Slice label collision on physvol", "Dup label is not slice"); `xvmbuild_addattr` (0x2024) loads `da_tid` and has "Newer name attr" and "Older name attr" messages, so name and permission entries are ordered by transaction id while no ordering test on an element entry has been seen. Not yet read branch by branch. **Settling test:** with `retools/tools/hostfacts.py` on `xvm_build.o:xvmb_add_piece`, follow every branch that depends on the entry's `+0x4` word and on the stored piece's `+0x4` word, and record, per branch, whether the later or the larger-`dve_tid` entry wins or the duplicate is dropped; then check the same on `6.5.22`. The on-disk check is two disks that hold the same slice entry with different `dve_tid` values.

## Slices and partitions

- Slices are not partitions. Slice discovery is entirely from label entries; the kernel does not read the DVH partition table to find them. **Established** by absence: the slice checks and the physvol builder read only the physical label and label sectors.
- On a DVH disk the tool maps a slice of type root, swap or usr to a volume-header partition entry so the PROM and loader can boot from it. Allowed numbers (tool help and error strings in both tools): swap only partition 1; usr only partition 6; root 0, 2-5, 7, 11-15; an option slice is never mapped. The chosen number is stored in `vsl_partition`. The original partition table is saved in a volume-header file named `backvh` and restored when the disk is unlabelled. **Established** for the numbers and the file name (strings in `/sbin/xvm`); **inferred** that the byte at slice-union offset 11 is the stored partition number (offset is established by `xvm_mem2dsk_ve` case 1 storing the byte from in-memory +0x5d, which the creation code sets from the request's partition field).
- Partitions 8 (volume header) and 10 (whole-disk volume) are never given to slices (tool help). For a disk with XVM only these two remain as raw partitions plus the mapped ones.
- GPT: the tool contains GPT reading (`gpt_read_label`, `gpt_read_partition_list`) and an error "gpt label not usable by xvm". No path that maps a slice to a GPT partition was found, and the kernel reads GPT only to locate the physical label. **Unknown** whether root, swap or usr slices on GPT disks map to GPT entries. Tried: strings and symbols in `/sbin/xvm` on both OSes, the kernel GPT routines. Settling it needs the ProPack tool's partition code (IA-64, not decodable here) or a real GPT image.

## Linux ProPack differences

The ProPack 4 modules (GA, SP2, SP4) carry the **same** on-disk structures as IRIX 6.5.30 for everything above: same magics and versions (`XVM_PHYSLAB_VERSION 2`, `XVM_LABELHDR_VERSION 2`), same sizes (124, 8, 72, 72), same field order, same enums. Differences found:

- Little-endian host, big-endian disk (see [Byte order](#byte-order)). Swap tables are in the module; IRIX needs none.
- The Linux module defines RAID objects (type 4) and a `vr_*` union; the IRIX module writes nothing for type 4. Whether any shipped IRIX release can create RAID objects was not checked.
- Linux reads the SGI volume header and swaps it (`xvm_xlate_volume_header`, `volume_header_endian`) and ships `lvhtool` for it; DVH disks are therefore handled on Linux.
- The Linux `xvm_last_region` swap table does not match the Linux in-memory structure (24 bytes covered against a 32-byte structure with a 64-bit time), so the on-disk "last region" record used by copy-on-write repositories is not defined here. **Unknown.**

**ProPack 6 SP3 (xvm 5.0, kernel 2.6.16.60) changed the physical label.** Its `xvm_physlab_s` is 160 bytes with 64-bit geometry and a queue depth, and the old 124-byte layout survives as `xvm_physlab_ver2_s`, converted by `xvm_physlab_ver_to_cur` and `xvm_physlab_cur_to_ver`:

| Off | Size | Field |
|---:|---:|---|
| 0 | 4 | `pl_magic` |
| 4 | 2 | `pl_version` |
| 6 | 1 | `pl_state` |
| 7 | 1 | `pl_flags` |
| 8 | 32 | `pl_hostname` |
| 40 | 16 | `pl_uuid` |
| 56 | 8 | `pl_secsize` |
| 64 | 8 | `pl_xvmlabel` |
| 72 | 8 | `pl_xvmlabelblks` |
| 80 | 8 | `pl_dbn` |
| 88 | 8 | `pl_nblks` |
| 96 | 8 | `pl_queue_depth` |
| 104 | 4 | `pl_timestamp` |
| 108 | 4 | `pl_clusterid` |
| 112 | 48 | `pl_diskname` |

Offsets and sizes are **established** from the PP6 DWARF. The version number the new label carries, whether `pl_secsize` is still in 512-byte units, and byte order of the new 64-bit fields (presumably big-endian) are **unknown**: the PP6 module expresses the swap as code, not as a table, and the version constant is in IA-64 text. Tried: symbol, DWARF and string analysis (`xvm_xlate_physlab_v2` exists, error text "can't handle ver %i"). **Next test:** build the pinned `binutils-gdb` submodule for an ia64 target (`--target=ia64-linux-gnu`; no IA-64 disassembler is installed here) and disassemble `xvm_physlab_ver_to_cur`, `xvm_physlab_cur_to_ver` and `xvm_xlate_physlab_v2` in the ProPack 6 `sgi-xvm.ko`. Three reads settle the three unknowns: the immediate compared with the 16-bit `pl_version` at label offset 4 (and the one stored by `xvm_physlab_cur_to_ver`) gives the version; whether the 32-bit geometry fields of the old layout are widened by a plain zero-extension or after a multiply or shift tells whether `pl_secsize` keeps its 512-byte units; and the presence or absence of an 8-byte byte reversal (`mux1 @rev`) on `pl_dbn`, `pl_nblks` and the other 64-bit fields gives their byte order. The name of the retained `xvm_physlab_ver2_s` structure suggests, without proving, that the new label is a higher version than 2. A label written by a ProPack 6 host is the on-disk alternative. The label-sector header, element entry and attribute entry have identical DWARF layouts in PP6 and PP4, so only the physical label moved. IRIX 6.5.30 never writes the 160-byte form.

## Not covered

- The copy-on-write repository and snapshot on-disk layout (`XVM_SNAP_VERSION 2`, `XVM_SNAP_REGBLKS_DEF 128`, base path `/xvm/repositories`, the region records): constants only; no record layout.
- Cluster ownership handshakes and the failover state, which live in memory.
- Behaviour for device blocks larger than 512 bytes.
- ProPack 3 and ProPack 1.x XVM modules were not examined; the ProPack 4 modules above were.
- The Administrator's Guide's block numbers (35, 3072, 4096, 8192) were taken from the coordinator's summary, not re-read; they agree with the structure above and with the 16-bit width of the fields.

## Provenance

Clean-room status: binary analysis only. The IRIX kernel archive and the ProPack modules are shipped products; their debug info is the source of field names for the Linux side. No kudzu or IRIX source tree was opened. Extracted copies sit in a local scratch directory (`irix/`, `pp4sp4/`, `pp4ga/`, `pp4sp2/`, `pp6/`), with the DWARF dumps `xvm-ko-types.txt`, `xvm-ko-info.txt`, `xvm-kt-macros.txt`, and the throwaway disassembly helpers `dis.py`, `dumpend.py`, `relfn.py` in the same place.
