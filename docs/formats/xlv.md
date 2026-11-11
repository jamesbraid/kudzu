# XLV on-disk format

The label that SGI's XLV logical volume manager keeps on each member disk, recovered from the shipped IRIX 6.5.30 kernel module and the shipped XLV tools. No public document describes it. This page is binary reverse engineering only: no IRIX source, headers or comments were read. Field names are this document's own; routine names quoted are symbol names that the binaries themselves carry (kernel objects) or list in their debug name tables (tools).

Companion files, same directory:

- `xlv-disk.h`: the structures below as a C header (compiles with `gcc -std=c11 -fsyntax-only`; every stated offset and size is asserted).
- `XLV_DECODE.h` (repository root): the same structures in the publish-safe decode-header form, with generated Rust under `generated/disk-formats/`. `defs.toml` ties its constants and field positions to the tables on this page, so the two cannot drift apart.
- `xlv-check.py`: re-derives the checkable facts from the binaries (`uv run --script xlv-check.py refs/irix-media/6.5.30`). 23 checks, all pass on the 6.5.30 media.

## Grades

- **Established**: the routine that reads or writes the field was disassembled in the named binary, and for most fields two independent routines (a writer and a reader, or a printer) agree.
- **Inferred**: one source only, a name or a conclusion drawn from established facts. The text says what would settle it.
- **Unknown**: not recovered. What was tried is stated.

## Evidence base

| Short name | Binary | Notes |
|---|---|---|
| kernel | `6.5.30/usr/cpu/sysgen/IP30boot/xlv.a`, members `xlv_root.o` (label read at boot), `xlv.o` (I/O mapping) | MIPS64 big-endian, unstripped objects with symbols. The IP27, IP32 and IP35 archives carry the same members. The 6.5-base kernel object has the same constants (`0x11201989`, `0x12a0`, `0x0be5a941`, offsets `0x28`, `0x50`); it was not compared instruction by instruction. |
| labd | `6.5.30/sbin/xlv_labd` (n32, stripped) | Holds the label writer, reader, consistency check and printer, linked in statically (no shared XLV library). Function addresses below are virtual addresses in this build. |
| make / assemble / mgr | `6.5.30/usr/sbin/xlv_make`, `sbin/xlv_assemble`, `sbin/xlv_mgr` | Same label code linked in; used for strings and for the printer in `xlv_mgr`. |
| libdisk | `6.5.30/usr/lib32/libdisk.so` | Partition-type name table, volume-header checksum, volume-header read and write. |

The kernel objects' DWARF has no XLV record types (only function and variable entries; the one record-bearing object, `xlv_icrash.o`, covers buffer and memory-pool structures). The userland tools carry no `.symtab` and no `.debug_info`. Everything below was therefore read from instructions; tools were Capstone-based (a function-boundary and annotation script over the stripped n32 binaries).

**No held disk image carries an XLV label** (the survey of the stores found none), so nothing here was compared with real on-disk bytes. The first real image is the discriminating test for every **inferred** line, and for the whole layout; see [How a reader should use this](#how-a-reader-should-use-this).

## Byte order

All multi-byte integers on disk are **big-endian**. Established: the kernel (MIPS big-endian) and the tools read the buffer with plain word loads, and neither contains any byte-swap code for the label. UUIDs are 16-byte opaque values (the tools only `bcopy`, `uuid_is_nil` and `uuid_equal` them): compare them as 16 raw bytes. Name strings are fixed 32-byte fields, NUL padded; the tools print them with width-bounded formats (`%.*s`, 32), so a name may use all 32 bytes with no terminator.

## Where the label lives: the `xlvlab` volume-directory entry

Every disk that carries a label has an entry named `xlvlab` in the SGI volume header's directory (the standard 512-byte `0x0be5a941` header at block 0 of the whole disk; checksum is the usual one, the 128 big-endian words sum to zero: `vhchksum`, `libdisk.so` +0xc00510c). There is exactly one such entry per disk; the tools look up this exact name (8-byte NUL-padded field) and no other (`xlv_lab1_open`, labd 0x10007c24; `xlv_lab1_init`, 0x100080b0; the delete path calls `vd_delete("xlvlab")`).

Volume header facts this depends on (the standard SGI format, restated only as far as XLV uses it):

| Item | Where | Grade |
|---|---|---|
| Header magic | `0x0be5a941` at byte 0 | established (`xlv_get_xlv_label`, kernel +0x344..0x350; `valid_vh`, libdisk) |
| Sector size | big-endian u16 at header byte `0x28` (the device-parameter `secbytes` field); 0 means 512 | established (kernel +0x358 reads it, then keeps 0x200 when zero; labd 0x10007d0c does the same) |
| Directory | 15 entries of 16 bytes from header byte `0x48`: name[8], block number u32 at +8, byte length u32 at +12 | established (kernel loop `slti 0xf`, `daddiu 0x48`, then `lw 0x50` = first entry's block number; labd compares `xlvlab` at `0x450` + 16*i in its in-memory copy of the header, which starts at `0x408`) |
| Block number unit | the disk's sector size S. Userland seeks `lbn * S` (labd 0x10007df0..0x10007e08). The kernel passes the same number to `bread` in 512-byte units (`xlv_read_dev` +0x38..0x48), so the two agree for S = 512 | established for S = 512; for S > 512 the userland rule is the one to follow, kernel behaviour unchecked |
| Placement | the file is carved out of the volume-header partition by `vh_contig` (first fit among the sorted directory entries); the directory entry's byte length is `roundup(512, S) + 2 * roundup(0x12a0, S)` | established (`xlv_lab1_init`, labd 0x100080b0..0x100082b4) |

`roundup(n, S)` is `ceil(n / S) * S` (`roundblk`, kernel xlv_root.o +0x0, labd 0x10008b20). For S = 512 the file is `512 + 2 * 5120 = 10752` bytes (21 sectors).

## Label file layout

The `xlvlab` file is three regions, back to back:

| Region | File offset | Size | Contents |
|---|---:|---:|---|
| master block | 0 | `roundup(512, S)` | update counter (below). The tools read and rewrite this region as one sector of S bytes. |
| label slot 0 | `roundup(512, S)` | `roundup(0x12a0, S)` | label image, `0x12a0` bytes used |
| label slot 1 | `roundup(512, S) + roundup(0x12a0, S)` | `roundup(0x12a0, S)` | label image |

Established: `xlv_get_current_label` (kernel xlv_root.o +0xa0), `xlv_lab1_open` (labd 0x10007c24), `xlv_lab1_write` (0x100083f0), `xlv_lab1_write_master_blk` (0x10008340).

### Master block

| Off | Size | Field | Meaning | Evidence | Grade |
|---:|---:|---|---|---|---|
| 0 | 8 | `seq` | update counter. **Bit 0 selects the current slot: even = slot 0, odd = slot 1.** The writer never overwrites the current slot (see [Write protocol](#write-protocol)). The printer shows the low 32 bits (`seq_num: %d`), so the high 32 bits are probably zero in practice. | kernel `xlv_get_current_label` +0xd0..0x124 (`ld`, `andi 1`, picks `buf + rb(512)` or `buf + rb(512) + rb(0x12a0)`); labd printer 0x10007a1c..0x10007a34; recovery 0x10006dd8 increments it | established |
| 8 | S-8 | (rest of the sector) | not read or written by any routine I found; the writer rewrites the whole sector from the buffer it read | `xlv_lab1_write_master_blk` writes S bytes from the master buffer | unknown (assume opaque, preserve on rewrite) |

### Label slot (`0x12a0` bytes)

A slot is a 0x120-byte header followed by 16 fixed 0x118-byte entries: `0x120 + 16 * 0x118 = 0x12a0`. The kernel validity test for a slot is the first four bytes equal to `0x11201989` and nothing else (no checksum, no version check): `xlv_get_current_label`. **There is no checksum or CRC anywhere in the label.** Established: no instruction in any reader or writer computes one, and the only magic tests are the ones cited.

If the slot the master selects has no magic, the kernel returns "no label"; it does not fall back to the other slot.

#### Header (`0x120` bytes)

| Off | Size | Field | Meaning | Evidence | Grade |
|---:|---:|---|---|---|---|
| 0x00 | 4 | `magic` | `0x11201989` (bytes `11 20 19 89`) | kernel `xlv_get_current_label` +0xd8..0xe4, `xlv_get_current_label` compares with `beq`; labd writer 0x10006150..0x1000615c sets it | established |
| 0x04 | 2 | `version` | 2. The writer stores 2 (`sh`) at the moment an unused entry becomes used. No reader checks it. | labd 0x1000620c; printer `lhu 4` | established for the written value; no version 1 layout seen. The message "Ignore version %d label ... for label check" prints the first word (the magic), not this field. |
| 0x06 | 1 | `nparts` | number of entries in use (non-nil `part_uuid`). Incremented when an entry goes from nil to used, decremented when one is cleared; when it reaches zero the writer clears the first byte of `nodename` | labd 0x10006208..0x10006214 (increment), 0x10005fb0..0x10005fbc and 0x10005fc4 (decrement, clear) | established |
| 0x07 | 1 | `reserved` | printed by the printer only in verbose mode ("reserved: 0x%x") | printer 0x10007414 | unknown (no writer sets it; expect 0) |
| 0x08 | 8 | `timestamp` | printed in verbose mode ("timestamp: 0x%llx") | printer 0x10007418 | unknown (no writer sets it; expect 0) |
| 0x10 | 8 | `trans_id` | configuration generation. Written on every label write. Comes from a process-wide counter that starts at `(gettimeofday seconds << 32) \| microseconds` on the first call and is incremented by one for each later call. Readers compare it between disks and between the two slots of a disk to find the newest configuration. | labd 0x10006950..0x100069a4 (counter), 0x10006198 and 0x100062a4 region (`sd` to +0x10), consistency check 0x10006b2c..0x10006f44 (compares +0x10 of both slots and across disks of the same `nodename`) | established |
| 0x18 | up to 0x108 | `nodename` | NUL-terminated name of the host that owns the volumes (the "XLV nodename", from `uname` or `xlv_mgr -h`). The writers `strncpy` 0x102 bytes into it (bytes 0x18..0x119 written; 0x11a..0x11f stay as in the template). `xlv_assemble` drops volumes whose entries carry another host's name ("Removing XLV Volume ... : not local"). | labd 0x10005ef0, 0x1000601c, 0x100062f0; consistency check strcmps it between slots | established for the copy length; ownership use inferred from the assemble message |

#### Entry (`0x118` bytes), entry `i` at slot offset `0x120 + i * 0x118`

**Entry `i` describes the volume element that owns partition `i` of this disk.** i is the volume-header partition number, 0..15. Established: the writer picks the entry with `dev_to_partnum(part device)` (labd 0x100061c8..0x100061e8), and the kernel builds the partition's device node from the entry index (`xlv_scan_invent`, +0x4f8..0x540, with special names for 8 and 10; slot 8 is the volume header and slot 10 is the whole-disk partition, which cannot hold a volume element). An entry is **unused when `part_uuid` is the nil UUID** (all zero); the kernel skips it (`uuid_is_nil(entry+0xf8)`, `xlv_scan_invent` +0x570..0x578) and so does every tool.

Because each used entry restates the whole chain volume > subvolume > plex > volume element > partition, a reader needs no cross-disk pointer table: collect the used entries of every disk and group them.

| Off | Size | Field | Meaning | Evidence | Grade |
|---:|---:|---|---|---|---|
| 0x000 | 4 | `vol_flags` | volume flag word. Printed as "flags=0x%x". One candidate: `xlv_make` ORs `0x2` for a `-readonly` option in its option parser (make 0x10008498), but I could not tie that parser to the volume line rather than the subvolume line. Other bits unrecovered. | builder labd 0x1000631c region (`vol+0x34`); kernel `xlv_scan_invent` +0x5a8 copies it to the in-core volume; printer 0x10007550 | width and position established; bit meanings unknown |
| 0x004 | 16 | `vol_uuid` | UUID of the volume | builder 0x10006358..0x1000636c; kernel +0x580 nil test and copy | established |
| 0x014 | 16 | `log_sv_uuid` | UUID of the log subvolume; nil if the volume has none | builder 0x10006380 (copies `vol.log_subvol.uuid`, else `uuid_create_nil`) | established |
| 0x024 | 16 | `data_sv_uuid` | UUID of the data subvolume; nil if none | builder 0x100063a0 | established |
| 0x034 | 16 | `rt_sv_uuid` | UUID of the real-time subvolume; nil if none | builder 0x100063c0 | established |
| 0x044 | 32 | `vol_name` | volume name | builder 0x100063d4..0x100063e0 (bcopy 0x20); kernel `strcmp(entry+0x164)` against a global name | established |
| 0x064 | 4 | `sv_flags_hi` | the subvolume flag word with the **low 16 bits cleared**: only bits 16..31 are stored. The one named bit in that half is bit 31, "No_err_retry" (`xlv_make -no_error_retry` sets `0x80000000`). | builder 0x100063f4..0x10006400 (`and 0xffff0000`); flag-name table in the tools; make 0x100087a4 | stored form established; bits 16..30 unnamed ("B16".."B30" in the tool's table) |
| 0x068 | 4 | (zero) | always written 0 | builder 0x100063f0 | established |
| 0x06c | 4 | `time` | seconds since the epoch, taken from `time()` at the start of the label write; the printer shows it as `time:` via `asctime(localtime())` | builder 0x10006404..0x10006408 with `time()` at labd 0x1000611c; printer 0x100076b0 | established (width, source); "last write of this entry" is inferred from the source |
| 0x070 | 4 | `vol_attr16` | a 16-bit volume-record value, stored sign-extended into 32 bits. The kernel copies the low 16 bits back to the in-core volume. | builder 0x1000640c..0x10006410 (`lh` then `sw`); kernel `xlv_scan_invent` +0x5b0..0x5bc (`lw` then `sh`) | width established; meaning unknown (not printed by any tool). Settling test: find every load or store of the in-core volume record's halfword at +0x3a, the one the label scan routine fills from this field (`xlv_root.o` +0x5bc, next to the `vol_flags` word at +0x34), in `xlv.a`, `xlv_plex.a`, `xlvidbg.o` and the label daemon, make and manager tools; a consumer that branches on or prints the value gives its meaning, and if the copy to and from the label is the only use the value is a no-software-dependency field |
| 0x074 | 1 | `sv_type` | subvolume type of this entry: **1 = log, 2 = data, 3 = real-time** | printer 0x1000767c..0x10007698 (labels `[log ]`, `[data]`, `[rt  ]`); kernel `xlv_scan_invent` +0x634..0x650 selects which of the three uuid fields above to use | established |
| 0x075 | 1 | (zero) | always 0 | builder 0x10006424 | established |
| 0x076 | 2 | `sv_depth` | subvolume depth: the number of distinct volume-element address starts across the subvolume's plexes (`xlv_tab_subvol_depth`, labd 0x1000a800). Printed as "depth". | builder 0x1000641c..0x1000642c | width established; the definition is read from the routine's loop and is inferred |
| 0x078 | 16 | `plex_uuid` | UUID of the plex | builder 0x10006444..0x10006450; kernel `xlv_add_plex` +0x1c0 | established |
| 0x088 | 32 | `plex_name` | plex name | builder 0x10006454..0x10006464; kernel +0x1f8 | established |
| 0x0a8 | 1 | `sv_nplex` | number of plexes in the subvolume (written from the subvolume's plex count; at most 4: the size routine loops over four plex slots, `xlv_tab_subvol_size`, labd 0x1000a880) | builder 0x1000646c..0x10006474; printer "plex %d (%d of %d)" | established |
| 0x0a9 | 1 | `plex_index` | this plex's index within the subvolume (0-based; the printer shows `index+1 of sv_nplex`) | builder 0x1000648c..0x10006498; kernel `xlv_add_plex` +0x128 | established |
| 0x0aa | 2 | `plex_nve` | number of volume elements in the plex | builder 0x10006618; kernel +0x164 and +0x21c | established |
| 0x0ac | 2 | `plex_flags` | plex flag word (16 bits stored) | builder 0x10006620..0x10006624; kernel +0x210 | width established; bits unknown. Settling test: read the in-core plex halfword that the plex-adding routine fills from this field (`xlv_root.o` `xlv_add_plex` +0x210), then every test of its bits in `xlv.a` and `xlv_plex.a` and any flag-name table in the volume tools, as was done for the subvolume flag word; the plex print routine in `xlvidbg.o` has a `flags=0x%x` format whose member is not yet matched to this one |
| 0x0ae | 2 | (pad) | not written | builder never stores here | inferred (zero) |
| 0x0b0 | 16 | `ve_uuid` | UUID of the volume element | builder 0x100064d4..0x100064dc, 0x100064e4..0x100064fc | established |
| 0x0c0 | 32 | `ve_name` | volume element name | builder 0x100064e8..0x100064fc | established |
| 0x0e0 | 1 | `ve_state` | **0 empty, 1 clean, 2 active, 3 stale, 4 offline, 5 incomplete** | builder 0x1000650c (`vol_elmnt+0x34`); name table at labd `.data` 0x100116f8 and `xlv_ve_state_str` (0x1000b168, accepts 0..5) | established |
| 0x0e1 | 1 | `ve_type` | **1 = stripe, 2 = concatenate** (anything other than 2 is handled as stripe) | printer 0x100077b0..0x100077f4 (`cat` when 2, `stripe` otherwise; `step=` only when 1); kernel `format_bufs` +0x248c..0x2494 picks `do_concatenated_ve` for 2 and `do_striped_ve` otherwise | established |
| 0x0e2 | 1 | `ve_nparts` | number of partitions in the volume element | builder 0x10006514; printer "(%d of %d)"; kernel `xlv_add_plex` +0x24c | established |
| 0x0e3 | 1 | `ve_index` | this element's index within the plex (0-based) | builder 0x100065f0..0x10006628 (found by scanning the plex's element array); kernel +0x1a0 | established |
| 0x0e4 | 2 | `ve_stripe_unit` | stripe unit in 512-byte blocks. Meaningful only for `ve_type` 1; the builder stores whatever the element holds | builder 0x100065f4; kernel `do_striped_ve` +0x1d4c (`lhu`), unit in blocks since `format_bufs` shifts block counts by 9 to bytes | width established; unit (512-byte blocks) established by the stripe arithmetic |
| 0x0e6 | 2 | (pad) | not written | | inferred (zero) |
| 0x0e8 | 8 | `ve_start` | first block of the element in the subvolume's address space, 512-byte blocks | builder 0x100065f8; kernel `xlv_add_plex` +0x25c | established |
| 0x0f0 | 8 | `ve_end` | last block of the element, **inclusive** (element size is `end - start + 1`; subvolume size is the last element's `end + 1`) | builder 0x10006600; `xlv_tab_subvol_size` (labd 0x1000a9dc..0x1000aa50: `size = max(end of last element) + 1`); kernel `format_bufs` +0x246c..0x2480 (`end - cur + 1`) | established |
| 0x0f8 | 16 | `part_uuid` | UUID of this partition; **nil means the entry is unused** | builder 0x10006560..0x10006570; kernel nil test | established |
| 0x108 | 4 | `part_dev` | in memory the device number of the partition. **On disk the writer replaces it with `0x10 \| i`** (i = this entry's index) for every used entry, writes, then restores the real value. Readers get the device from the partition node, not from here. | labd `xlv_lab1_write` 0x100084b8..0x100084d4 (store `i\|0x10`), 0x10008514..0x1000852c (restore); builder 0x10006588 | established |
| 0x10c | 4 | `part_size` | size of the partition's slice in 512-byte blocks (as recorded for this element; for a stripe the creation step truncates unequal partitions to the smaller size, see below) | builder 0x1000658c; printer "size=%u"; kernel `xlv_add_plex` +0x288 stores it as the in-core part size, which `do_concatenated_ve` uses | width and use established; whether it is the raw slice size or the truncated size is inferred (see [Unknowns](#unknowns)) |
| 0x110 | 1 | `part_index` | this partition's index within the element (0-based) | builder 0x100065a0..0x100065d0; kernel `xlv_add_plex` +0x1a8 | established |
| 0x111 | 1 | (zero) | always written 0 | builder 0x100065dc | established |
| 0x112 | 6 | (pad) | not written | | inferred (zero) |

Total: `0x118` bytes. Slot sanity: `0x120 + 16 * 0x118 = 0x12a0`, equal to the constant the kernel and the tools round.

## The hierarchy as the label records it

```
volume   (vol_uuid, vol_name, vol_flags, nodename in the header)
 ├─ log subvolume   (log_sv_uuid  — sv_type 1)
 ├─ data subvolume  (data_sv_uuid — sv_type 2)
 └─ real-time subvol(rt_sv_uuid   — sv_type 3)
     └─ plex 0..sv_nplex-1   (plex_uuid, plex_name, plex_index, plex_flags)
         └─ volume element 0..plex_nve-1   (ve_uuid, ve_name, ve_index, ve_type, ve_state,
                                            ve_start..ve_end, ve_stripe_unit, ve_nparts)
             └─ partition 0..ve_nparts-1   (part_uuid, part_index, part_size; one label entry
                                            on the disk whose partition number is the entry index)
```

- **Data, log and real-time subvolumes are recorded by `sv_type`.** Each used entry belongs to exactly one subvolume, named by `sv_type` (1, 2, 3). The three subvolume UUIDs in the entry are the volume-wide set, with the nil UUID for a missing subvolume. There is no separate subvolume record; the subvolume's parameters are `sv_type`, `sv_depth`, `sv_nplex`, `sv_flags_hi` and its UUID (the one of the three selected by `sv_type`, `xlv_scan_invent` +0x634..0x7a8).
- **Mirroring is by plex.** Up to 4 plexes per subvolume (`sv_nplex`); every plex of a subvolume spans the same address range with its own volume elements. Established for the cap by `xlv_tab_subvol_size`.
- **Concatenation and striping are per element.** A plex is a sequence of volume elements, each covering `ve_start..ve_end` of the subvolume; the elements of a plex are in address order ("The volume element's address range must be increasing", `xlv_make` error table).

### Address translation (kernel, `xlv.o`)

Given subvolume block `b`, the plex's element with `ve_start <= b <= ve_end`, and `o = b - ve_start` (`format_bufs`, +0x246c..0x2478):

- **Concatenate** (`ve_type` 2): walk the partitions in `part_index` order, subtracting each `part_size` until `o` falls inside a partition; block `o` (remaining) of that partition is the target (`do_concatenated_ve`, +0x21a8..0x21d0 and +0x2220..0x2228). Established.
- **Stripe** (`ve_type` 1), `u = ve_stripe_unit`, `n = ve_nparts`: chunk `c = o / u`; partition index `c mod n`; block within the partition `(o / (u * n)) * u + (o mod u)` (`do_striped_ve`, +0x1d50 `ddiv`, +0x1d80 `ddiv`, +0x1fe8..0x202c). Established.
- The target block is relative to the **start of the partition**: no header skip. The partition holds data from its block 0. Established (the computed value is stored unadjusted as the buffer's block number).

The device of partition `part_index` is the partition node of whichever disk holds the entry; to reassemble from raw disks, partition `i` of a disk is the slice with number `i` in that disk's volume header (the volume header's partition table, standard SGI layout).

## Partition types (volume header)

The partition table type numbers, from the `libdisk` name table (`libdisk.so` `.data` +0x0, used by `pttype_to_name`): `0 volhdr`, `3 raw`, `5 efs`, `6 volume` (whole disk), `7 efs` (listed twice), `10 xfs`, `11 xfslog`, `12 xlv`, `13 xvm`, `14 vxvm`. **Established.** The coordinator-supplied guess "xfslog = 11, xlv = 12, xvm = 13" is confirmed.

What XLV does to a member partition's type: `xlv_check_partition_type` (labd 0x10009240..0x1000931c; used by `xlv_make` and the label writer) accepts, for a **log** subvolume, a partition of type 11 or 12; for **data** and **real-time**, type 12, and with an extra "retype existing" argument also 7, 10 or 11. `xlv_lab1_update_ptype` rewrites the type. The Administrator's Guide statement (data and real-time partitions become type `xlv`, log partitions become `xfslog`) is consistent with this and was not contradicted. **Inferred** for the exact retyping rule; a real disk is the test.

## Write protocol

Two-phase, so a crash leaves one valid slot (`xlv_lab2_write` driver at labd 0x10006680):

1. For every disk of the volume set: build the new label (copy the current slot, set `trans_id`, update entries), then write it into the **other** slot: `xlv_lab1_write` chooses by bit 0 of the in-memory `seq`: bit set writes slot 0, clear writes slot 1 (0x100083f4..0x10008574). The current slot is never touched.
2. For every disk: increment `seq` and rewrite the master sector. After that the written slot is current.

On failure the second phase is undone by decrementing `seq` and rewriting (labd 0x100068dc..0x10006908). The recovery pass (labd 0x10006b2c..0x10006f44, `xlv_lab2_check`/`make_consistent`) compares `trans_id` between the slots of a disk and between disks with the same `nodename`, and flips `seq` to adopt the newer slot ("Recovered xlv label").

For a read-only reader: read the master, take `slot = seq & 1`, require `magic` there, then use the entries. If the disks of one volume disagree on `trans_id`, the largest is the newest configuration; the two slots of one disk can also differ by one generation (a crash between phase 1 and phase 2).

## How a reader should use this

1. Read block 0, check `0x0be5a941` and the checksum, find the directory entry named exactly `xlvlab`, take its block number and byte length; `S` is the u16 at header `0x28` (0 means 512).
2. Read the master sector and both slots. Current slot = `seq & 1`. Require magic `0x11201989`.
3. For each i in 0..15 with a non-nil `part_uuid`, the slice for the element is partition i of this disk (offset and length from the volume header's partition table; the label's `part_size` is a cross-check).
4. Group by `vol_uuid` (and `nodename`), then `sv_type`, `plex_index`, `ve_index`, `part_index`. A volume is complete when every plex's element index and partition index set is complete, as given by `sv_nplex`, `plex_nve` and `ve_nparts`.
5. Translate with the concatenate and stripe rules above. Subvolume addresses are 512-byte blocks regardless of `S`.

## Unknowns

- **No real image was available.** The layout is closed under three independent checks (kernel reader, userland writer, userland printer all agree on every offset; sizes `0x120 + 16*0x118 = 0x12a0` match the constant used for I/O) but no byte of a real label was compared.
- `vol_flags` bit meanings, `plex_flags` bits, `sv_flags_hi` bits 16..30, and the `vol_attr16` value: positions and widths only. Tried: tool flag-name tables (they name the in-core subvolume flag word, bits 0..7: Char_IO, FS_IO, Block_IO, Swap_IO, Higher_driver, Offline, Copy_in_progress, In_retry; the stored half is above those), `xlv_make` option parsing.
- Header bytes `0x07` and `0x08..0x0f`: printed in verbose mode but no writer sets them.
- Master sector bytes 8..S-1: unused by anything read.
- Whether `part_size` for a stripe is the raw slice size or the size after truncation to equal partitions. The kernel uses only `ve_start`/`ve_end` and `ve_stripe_unit` to place stripe blocks, so a reader should rely on those and treat `part_size` as informational for stripes. Concatenation uses `part_size` directly.
- The block-number unit of the directory entry when `S` is not 512 (see above).
- Sector sizes other than 512 were not exercised anywhere; the rounding rules are from the code.
- Label versions other than 2: none seen.
- Anything the tools do not touch: for example whether a volume with more than one `nodename` ever coexists in the label (the header has one name per slot, so one host per disk).

## Provenance

Binary RE only. Source for each fact: the disassembly cited by address. No IRIX source, headers or comments were read. The shipped help text and strings were used for names and enumerations. The Administrator's Guide (007-2825) was used only to cross-check the partition-type statement.
