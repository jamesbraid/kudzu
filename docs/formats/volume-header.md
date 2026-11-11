# SGI disk volume header (dvh): on-disk format

Scope: the 512-byte volume header at block 0 of an SGI-labelled disk. Derived from binary
reverse engineering of shipped IRIX 6.5.30 and from one real block 0 written by that release.
No vendor source, headers or source-derived notes were read. Field names below are our own
descriptive names, not vendor identifiers. Byte order is big-endian throughout.

Companion: `VOLHDR_DECODE.h` (repository root) states the layout in the publish-safe decode-header form, with generated Rust under `generated/disk-formats/`. `defs.toml` ties its constants and field positions to the tables on this page.

Grades: **established** = observed directly in more than one binary and/or a real block 0;
**inferred** = follows from one binary or from layout arithmetic; **unknown** = not exercised
by anything examined.

Binaries (all under `refs/irix-media/6.5.30/`): `sbin/dvhtool` (N32, symbols kept),
`usr/sbin/prtvtoc`, `lib32/libdisk.so` (N32, symbols kept), `stand/fx` (ECOFF, disk formatter),
`stand/sash.CPUBOARD-IP27-CPUBOARD-IP30-CPUBOARD-IP35` (ELF relocatable), and the kernel disk
driver `usr/cpu/sysgen/IP30boot/dksc.o` plus `io.a(disksub.o)`. Real block 0: first sector of
`irix-6.5.30-accepted.qcow2` (an IRIX 6.5.30 install on the emulated Octane, written by IRIX
itself; read-only via `qemu-img dd -U`).

## 1. Overall layout (512 bytes, one sector)

| Offset | Size | Field | Grade |
|---|---|---|---|
| 0x000 | 4 | magic = `0x0BE5A941` (bytes `0b e5 a9 41`) | established |
| 0x004 | 2 | root partition number (signed 16) | established |
| 0x006 | 2 | swap partition number (signed 16) | established |
| 0x008 | 16 | boot file name, NUL padded (first byte 0 = none) | established |
| 0x018 | 48 | device parameters (dp), section 3 | established (size), see per-field grades |
| 0x048 | 15 x 16 = 240 | volume directory, section 2 | established |
| 0x138 | 16 x 12 = 192 | partition table, section 4 | established |
| 0x1F8 | 4 | checksum, section 5 | established |
| 0x1FC | 4 | padding, zero in the real block | inferred |

Evidence for the pieces:
- Magic: `valid_vh` (libdisk), `is_vh` (kernel disksub.o, sash, dvhtool imports libdisk's) all
  compare the first word to `lui 0x0be5 / ori 0xa941`. `putdiskheader` (libdisk), `set_vhchksum`
  (sash) and the kernel's default-label builder write it. Real block 0 starts `0b e5 a9 41`.
- Size 0x200: `getheaderfromdisk` reads 0x1000 bytes into a buffer and `bcopy`s 0x200 to the
  caller; `putdiskheader` writes 0x200 bytes; the kernel `bcopy`s 0x200 into the softc; sash
  `dopart` reads 0x1000 and uses the first 0x200.
- Root/swap: dvhtool `show_bootinfo` prints `lh 4(vh)` and `lh 6(vh)` and the string at +8;
  fx's bootinfo menu has "root partition", "swap partition", "bootfile" (`%s = %.16s`, so 16
  bytes). Real block: root 0, swap 1, bootfile `/unix`.
- The kernel's in-memory label is the first 0x200 bytes of the per-disk state structure
  (dksc.o reads block size and capacity at later offsets of the same object; vh accessors are
  at offsets 0..0x1FF of it).
- Sector size: all arithmetic is in 512-byte units here (the "block" below). The header's dp
  carries the drive sector size (section 3), but every observed writer uses 512.

## 2. Volume directory (0x048, 15 entries, 16 bytes each) - established

| Entry offset | Size | Field |
|---|---|---|
| +0 | 8 | file name, NUL padded; **not** NUL terminated at 8 chars |
| +8 | 4 | start block (512-byte units from block 0 of the disk) |
| +12 | 4 | length in bytes |

Evidence:
- dvhtool `vd_list` loops i = 0..14 over `vh+0x48`, stride 0x10, prints name `%.8s`, `+8` and
  `+12`, skipping entries whose `+12` is zero; fx prints `"%2d: %-10.8s block %4d size %7d"`
  and the heading "directory entries".
- Free slot test: dvhtool `vd_creat` treats `+12 == 0` as free (it tests `lw 0x54(vh+i*16)`).
  Name length: `vd_creat` requires `strlen(name) < 9` and errors with "no longer than 8
  characters"; `strncmp`/`strncpy` use 8.
- Size rounding: `roundblk` rounds the byte length up to the block size (global, 512 here);
  data is stored at block `+8` .. `+8 + ceil(len/512)`. `vh_contig` does first-fit over the
  entries sorted by start block (`lbncmp`), within the volhdr partition; `vh_availspace` =
  volhdr partition size minus the rounded sizes of all entries.
- Order: slots are not required to be in block order on disk; dvhtool sorts a copy for
  allocation (`sortdir`).
- Real block 0, three entries:
  `ide` block 2 size 0x41400; `sash` block 0x20C size 0x41400; `IP30prom` block 0x416 size
  0xD6C00 (names NUL padded; slot 3 onward all zero).
- File content has no per-file header; it is the raw bytes (dvhtool `copy_tovh` copies exactly
  the byte count, padding the last block).

## 3. Device parameters dp (0x018, 48 bytes)

dp is the drive geometry/behaviour block. The binaries only touch a few fields; the rest are
graded by layout arithmetic and by agreement with the real block.

| dp offset | vh offset | Size | Meaning | Grade |
|---|---|---|---|---|
| +4 | 0x1C | u16 | cylinder count | inferred (real block 0x4104 = 16644; 16644*16*63 = 16 777 152, within 64 of capacity) |
| +6 | 0x1E | u16 | head offset of volume 0 | unknown (zero) |
| +8 | 0x20 | u16 | tracks (heads) per cylinder | inferred (real block 16) |
| +10 | 0x22 | u8 | command-tag-queue depth | established as a field: kernel stores 0x10 here when the drive supports tagged queuing; real block 0 (untagged emulated disk) |
| +11 | 0x23 | u8 | high cylinder bits | unknown (zero) |
| +14 | 0x26 | u16 | sectors per track | inferred (real block 63) |
| +16 | 0x28 | u16 | **bytes per sector** | established: kernel stores block size here, sash `dopart` stores 0x200, fx compares it to the drive's value and prints "bytes/sec" on mismatch; real block 0x0200 |
| +20 | 0x2C | u32 | flags | inferred: kernel sets 0x40 together with the tag depth; fx's compare routine tests it when nonzero |
| +44 | 0x44 | u32 | **drive capacity in blocks** | established: kernel `dk_chkptsize` writes the READ CAPACITY result here; sash writes capacity here when it synthesises a header; fx compares it ("drive capacity") against the drive when nonzero; real block 0x01000000 = 16 777 216 blocks = 8 GiB, equal to the qcow2 virtual size |
| others (+0..+3, +12, +18, +22..+43) | | | skew/gap/interleave/retry/data-rate-style legacy fields | unknown; no examined binary reads or writes them, all zero in the real block |

Notes:
- fx copies exactly 0x30 bytes from `vh+0x18` into its own parameter copy, then compares the
  `+16` (u16) and `+44` (u32) fields (`lhu 0x10`, `lw 0x2c` in the compare routine). That
  bounds dp to 48 bytes at 0x18 and makes those two the geometry-validity checks fx applies.
- Treat dp as advisory. Nothing in the disk-open path fails on stale cylinders/heads/sectors;
  the kernel overwrites capacity (+44) from the drive on every open and rewrites the label if
  it changed (see section 7).
- Older drives encoded the capacity as cylinders*heads*sectors; the +44 field is the 6.5.30
  path for large disks. This is inference from where fx and the kernel look.

## 4. Partition table (0x138, 16 entries, 12 bytes each) - established

| Entry offset | Size | Field |
|---|---|---|
| +0 | 4 | block count (0 = unused entry) |
| +4 | 4 | first block (512-byte units from block 0 of the disk) |
| +8 | 4 | type, section 6 |

Evidence: dvhtool `getvhpart` scans entries from `vh+0x138` stride 12 for `nblks != 0 &&
type == 0` (the volhdr partition); sash `dump_vh` prints entry i as `(i, +0 nblks, +4 first,
+8 type)` for 16 entries; fx walks 16 entries from `vh+0x138` stride 0xC; the kernel's
`dk_chkptsize` reads `+0` and `+4` of each entry to clamp it to disk capacity.

Conventional slots (established by fx's validation messages, sash's header builder and the
kernel's default-label builder, not by the format itself):

| Slot | Use | Type | Grade |
|---|---|---|---|
| 0 | root (default `root partition` = 0) | efs/xfs | established |
| 1 | swap (default `swap partition` = 1) | raw | established |
| 8 | volume header: first block 0, type volhdr; fx errors "volhdr doesn't start at 0", "not of type volhdr", "volhdr is too small", "volhdr is too big -- must be less than 2GB" | 0 | established |
| 10 | whole disk: first block 0, count = capacity, type volume; fx errors "not of type volume" | 6 | established |

The other slots are free for user layouts; slot 7 and the rest have no fixed meaning in the
tools examined. Partitions may overlap (slot 10 overlaps everything by design); fx warns about
overlap with the volhdr partition ("intersects volhdr partition") and about "base out of
range" / "size out of range" against capacity.

Real block 0, decoded:

| Slot | nblks | first | type |
|---|---|---|---|
| 0 | 0x00FBF000 (16 445 440) | 0x00041000 | 10 (xfs) |
| 1 | 0x00040000 (262 144) | 0x00001000 | 3 (raw) |
| 8 | 0x00001000 (4096) | 0 | 0 (volhdr) |
| 10 | 0x01000000 (16 777 216) | 0 | 6 (volume) |

The layout is volhdr 4096 blocks, swap 128 MiB at 0x1000, root from 0x41000 (= 4096 + 262144)
to the end of the disk; sash's miniroot builder uses the same ordering. Slot 0 end =
0x41000 + 0xFBF000 = 0x1000000 = capacity, exactly.

Default header synthesised by the kernel when block 0 has no valid header (dksc.o, opening a
disk): zero 512 bytes, set the magic, set partition 8 = {min(capacity, 0x1000), 0, volhdr},
partition 10 = {capacity, 0, volume}, write dp bytes-per-sector and capacity, then the
checksum. (Offsets 0x198/0x19C/0x1A0 and 0x1B0/0x1B4/0x1B8 in the code.) established.

## 5. Checksum (0x1F8) - established

Treat the 512 bytes as 128 big-endian u32 words. Valid when the 32-bit wrapping sum of all 128
words, including the checksum word, is zero. To write: set the checksum word to 0, sum the
other 127 words, store the negation of the sum.

- Kernel `vh_checksum` (disksub.o) and sash `vh_checksum`: sum of words 0..127 (loop to
  `buf+0x1ff`); `is_vh` = magic matches AND that sum is zero.
- libdisk `putdiskheader`: writes magic, `csum = 0`, `csum = -vh_checksum(buf)`.
  dvhtool's own `vh_checksum` returns `-sum` (so it returns the value to store); libdisk
  `valid_vh` accepts when the function returns 0.
- Real block 0: the sum of the 128 words is `0x00000000`; stored word `0x6FEEB2DA`.
- A header whose magic matches but whose checksum is wrong is treated as invalid by every
  consumer examined (dksc opens it as "no label" and builds the default one; dvhtool reports
  "does not appear to have a valid volume header").

## 6. Partition type numbers

Source for each number is listed. "fx menu" is the name/value table in fx's `.data` (selectable
types); "libdisk table" is `pttype_to_name`'s table.

| Type | Name | Grade | Evidence |
|---|---|---|---|
| 0 | volhdr | established | fx menu, libdisk table, fx volhdr-slot validation, sash/kernel default headers |
| 1 | track-replacement (trkrepl) | inferred | neither name nor value appears in any binary; slot is unnamed (-1) in libdisk's table |
| 2 | sector-replacement (secrepl) | inferred | as type 1 |
| 3 | raw | established | fx menu, libdisk table, sash writes 3 for swap, fx swap check compares with 3 |
| 4 | bsd | inferred | only in fx's text "raw, bsd, sysv, efs, xfs, lvol, rlvol, xlv, or xfslog" (listed in ascending numeric order, so bsd falls between raw=3 and efs=7); libdisk's table leaves 4 unnamed |
| 5 | sysv | established as a number, inferred as a name | libdisk table maps type 5 to the string "efs" (the legacy EFS/sysv-type slot); fx text puts "sysv" between "bsd" and "efs" |
| 6 | volume (whole disk) | established | fx menu, libdisk table, kernel/sash/fx slot-10 convention |
| 7 | efs | established | fx menu, libdisk table |
| 8 | lvol (logical volume) | established | fx menu (value 8) |
| 9 | rlvol (raw logical volume) | established | fx menu (value 9) |
| 10 | xfs | established | fx menu, libdisk table, sash miniroot root = 10, real block slot 0 |
| 11 | xfslog | established | fx menu, libdisk table |
| 12 | xlv | established | fx menu, libdisk table |
| 13 | xvm | established (libdisk only) | libdisk table; not in fx's menu (this fx predates it) |
| 14 | vxvm | established (libdisk only) | libdisk table; Veritas VxVM, treated as a data type |

Requested types confirmed: xfslog = 11, xlv = 12, xvm = 13.

Notes:
- libdisk's `pttype_to_name` only returns names for 0, 3, 5, 6, 7, 10, 11, 12, 13, 14 (type 5
  and 7 both give "efs"). `prtvtoc` prints the string from this function; a type with no name
  prints blank. Do not use these names as proof that 1, 2, 4, 8, 9 are unused.
- fx's allowed set for the root and swap partitions: raw, bsd, sysv, efs, xfs, lvol, rlvol,
  xlv, xfslog (so 3, 4, 5, 7, 8, 9, 10, 11, 12). Not allowed there: volhdr, volume, xvm, vxvm.
- The numbers above 12 are not recognised by this fx at all (menu stops at xlv), but the
  kernel and libdisk carry them; the format treats the field as a plain u32.
- Type 6 (volume) is conventionally slot 10 only; fx refuses to change it ("sorry, can't
  change volume partition").

## 7. How the kernel uses the header (summary)

- Opening a disk reads block 0; if `is_vh` is false the kernel reads capacity and builds the
  default header (section 4) and rewrites it to memory (disk write happens on a later label
  update).
- `dk_chkptsize` reconciles the header with the drive: stores capacity in dp+44; for each of
  16 partitions, if `first + nblks` exceeds capacity then either clears the entry (first >
  capacity) or truncates `nblks` to `capacity - first`; whole-disk slot 10 is reset to first 0,
  nblks = capacity if it starts nonzero or covers less than a quarter of the disk; recomputes
  the checksum when anything changed. (Inferred from the disassembly; the quarter threshold is
  the least certain part.)
- The volume header is exposed as partition 8 (`.../volume_header`, device `partition(8)` in
  ARCS paths such as `scsi()disk(1)rdisk()partition(8)/sash`) and slot 10 as `volume`.
- Boot code (sash) reads the volume directory by name from partition 8 and the root/swap
  numbers from the header.

## 8. Things not established

- dp fields other than +4/+6/+8/+10/+14/+16/+20/+44, and the meaning of flag bits besides
  0x40 (set alongside tagged queuing).
- Names of types 1, 2, 4 and, strictly, 5 in the binaries; the numbers 1, 2, 4 are from the
  ordering evidence only.
- Whether 0x1FC..0x1FF is ever used (zero in the real block; no code touches it).
- Behaviour with a non-512-byte sector size in dp+16: fx compares it, libdisk has
  `disk_setblksz`, but no examined writer produces any other value. All block fields stay in
  512-byte units in the examples.
- The real block comes from an emulated disk, so its geometry values (dp +4/+8/+14) reflect the
  emulator's SCSI answers, not a physical drive.

## 9. Method

Disassembly with Capstone (MIPS64 BE) over the ELF and ECOFF files; `stand/fx` was loaded
with a small ECOFF section reader (its text carries unresolved call targets, so callee names
inside fx are not recovered; only data references and field offsets were used). Every offset
above was read from instruction displacements, not from a header file. Scratch work is under
`/storage/nvme/origami/tmp/storage-formats/vh/` (not for commit).
