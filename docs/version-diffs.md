# IRIX 6.5.22 against 6.5.30: routine diffs

This page compares the routines of the IRIX 6.5.22 and 6.5.30 kernels. It covers the three linked kernels (`unix.IP27`, `unix.IP30` and `unix.IP35`) and the kernel object files of the IP27, IP30 and IP35 boot directories (`usr/cpu/sysgen/IP27boot` and so on). The per-object counts are in [version-diffs.tsv](version-diffs.tsv). The changed routines, hardware-facing first, are in [version-diffs-queue.tsv](version-diffs-queue.tsv).

## Inputs

- The 6.5.22 linked kernels are in `root/6.5.22/`. Their SHA-256 hashes were added to `scripts/inventory-root.sha256`; they match a direct hash of the files. This repository does not record which installed system they came from.
- The scope rule now names the 6.5.22 linked kernels with the same name rule as 6.5.30 (`scripts/gen-scope.py`). Before this, the 6.5.22 kernel rows were all counted as in scope.
- `var/sysgen.IP27`, `var/sysgen.IP30` and `var/sysgen.IP35` in 6.5.22 hold configuration files and no object code. They are not compared.

## Method

Routines are matched by the code hash in [functions.tsv](inventory/functions.tsv): the first 12 hex digits of the SHA-256 of the routine's bytes, over the symbol's size. Rows without a code hash have no bytes and are not matched. Across the linked kernels and the IP27, IP30 and IP35 boot objects there are 13,308 such rows in 6.5.30 and 12,753 in 6.5.22; most are DWARF names with no address.

Within each linked kernel or object, a routine in one release is paired with the other release in this order:

1. **identical**: same name, same bytes, same address.
2. **moved**: same name, same bytes, different address.
3. **moved (renamed)**: same bytes under another name. This pairing uses the bytes alone, so it only means something for a distinctive body. Most of these pairs are two-instruction stubs of 8 bytes that many routines share.
4. **changed**: same name, different bytes.
5. **added** or **removed**: no partner in the other release.

The linked kernels are re-read, because their bytes include absolute jump targets, gp-relative offsets and address constants. A changed hash there does not mean the code changed. The changed routines are sorted into classes:

- **layout**: only jump targets and gp offsets differ.
- **immediate**: the instructions match in opcode and registers, and a 16-bit immediate differs (possibly with jump targets or gp offsets). This can be a moved stack or data offset or a changed constant.
- **code**: an opcode, a register or the instruction count differs.
- **size**: the routine's size differs.

Relocatable objects (`.o` files and archive members) hold zero in every jump and address field until the link, and their relocations are not in the hashed bytes. So a changed hash there means the instruction bytes differ. These routines are not re-read, so they have no class.

## Linked kernels

| Kernel | Located in 6.5.30 | Located in 6.5.22 | Identical | Moved | Moved (renamed) | Changed | Added | Removed |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| `unix.IP27` | 9,353 | 9,471 | 0 | 1,930 | 26 | 7,123 | 274 | 392 |
| `unix.IP30` | 8,939 | 8,361 | 0 | 1,879 | 18 | 6,417 | 625 | 47 |
| `unix.IP35` | 10,101 | 9,860 | 0 | 1,971 | 21 | 7,727 | 382 | 141 |

Identical is zero: no routine keeps both its name and its address with the same bytes. Seventy IP27 pairs and three IP35 pairs share an address and bytes under different names. All of them are 8-byte bodies, and the two releases name them differently, so they are not counted as identical. Every kernel's changed routines split by class as follows.

| Kernel | Layout | Immediate | Code | Size |
|---|---:|---:|---:|---:|
| `unix.IP27` | 5,945 | 417 | 141 | 620 |
| `unix.IP30` | 5,221 | 412 | 151 | 633 |
| `unix.IP35` | 6,240 | 490 | 185 | 812 |

Hardware-facing routines are those whose names start with a prefix in the narrow rule of [scripts/scope.json](../scripts/scope.json): the hardware graph, KLCONFIG, interrupts, NUMA, the crossbow, Heart and Bridge drivers and the panic entry points.

| Kernel | Layout | Immediate | Code | Size | Added | Removed |
|---|---:|---:|---:|---:|---:|---:|
| `unix.IP27` | 810 | 12 | 5 | 12 | 4 | 1 |
| `unix.IP30` | 587 | 8 | 0 | 7 | 6 | 0 |
| `unix.IP35` | 738 | 29 | 10 | 23 | 6 | 1 |

The hardware-facing routines whose code differs are these. Each is listed in the queue with its addresses.

| Kernel | Routine | 6.5.30 address | 6.5.22 address | Size | Notes |
|---|---|---|---|---:|---|
| `unix.IP27` | `hub_device_inquiry` | `0xc00000000003c59c` | `0xc00000000003c290` | 436 | immediate and gp offsets also differ |
| `unix.IP27` | `huberror_process_wrb_error` | `0xc0000000000342ac` | `0xc00000000003400c` | 556 | immediate also differs |
| `unix.IP27` | `intr_init_vecblk` | `0xc000000000037f58` | `0xc000000000037c74` | 436 | `s2` and `s7` trade places (see below) |
| `unix.IP27` | `klhwg_connect_hubs` | `0xc0000000000425bc` | `0xc000000000042034` | 564 | |
| `unix.IP27` | `ecc_printPending` | `0xc000000000056abc` | `0xc000000000056118` | 384 | not understood on 6.5.30 |
| `unix.IP35` | `huberror_process_wrb_error` | `0xc00000000005a410` | `0xc00000000005acfc` | 1148 | immediate and jump targets also differ |
| `unix.IP35` | `hubiio_crb_error_handler` | `0xc00000000005acd8` | `0xc00000000005b5c4` | 808 | gp offsets and jump targets also differ |
| `unix.IP35` | `intr_init_vecblk` | `0xc00000000005ee74` | `0xc00000000005f6fc` | 436 | not read in this pass |
| `unix.IP35` | `ecc_printPending` | `0xc00000000007c788` | `0xc00000000007c5bc` | 384 | not understood on 6.5.30 |
| `unix.IP35` | `ioc3_serial_attach` | `0xc000000000221394` | `0xc0000000001ec214` | 1300 | gp offsets and jump targets also differ |
| `unix.IP35` | `klhwg_add_node` | `0xc000000000073268` | `0xc000000000073154` | 1356 | |
| `unix.IP35` | `elsc_ody_expander_set` | `0xc0000000000d1ddc` | `0xc0000000000d4034` | 328 | not understood on 6.5.30 |
| `unix.IP35` | `ip27log_setup` | `0xc00000000004a9c0` | `0xc00000000004b2d0` | 476 | not understood on 6.5.30 |
| `unix.IP35` | `klhwg_premium_std_dimm_mix_check` | `0xc000000000074520` | `0xc000000000074400` | 560 | not understood on 6.5.30 |
| `unix.IP35` | `prom_reboot` | `0xc0000000002afa98` | `0xc00000000027194c` | 216 | not understood on 6.5.30 |

The code class was checked by reading the word-level differences for `intr_init_vecblk` in IP27. The two callee-saved registers `s2` and `s7` trade places in the stores and loads at words 1, 2, 5 and 19 of the body. The other differences are gp offsets (words 32, 63 and 94) and one jump target (word 67). The other code rows were not read in this pass; the queue is where they are read.

## Sysgen objects

Counts for the objects that hold routines in either release. Objects present in only one release are included with zero on the other side.

| Platform | Objects | Located in 6.5.30 | Located in 6.5.22 | Identical | Moved | Changed | Added | Removed |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| IP27, hardware-facing driver objects | 103 | 1,433 | 1,492 | 1,102 | 278 | 50 | 3 | 62 |
| IP27, platform core (`kernel.o`, `idbg.o`, `mem.o`) | 3 | 1,677 | 1,672 | 46 | 1,045 | 581 | 5 | 0 |
| IP27, other objects | 866 | 17,141 | 16,367 | 10,076 | 3,035 | 3,057 | 968 | 194 |
| IP30, hardware-facing driver objects | 45 | 1,116 | 1,126 | 817 | 263 | 34 | 2 | 12 |
| IP30, platform core | 3 | 1,036 | 1,031 | 50 | 704 | 277 | 5 | 0 |
| IP30, other objects | 675 | 14,954 | 13,906 | 8,151 | 2,760 | 2,831 | 1,207 | 159 |
| IP35, hardware-facing driver objects | 78 | 1,311 | 1,370 | 916 | 320 | 72 | 3 | 62 |
| IP35, platform core | 3 | 1,634 | 1,607 | 46 | 783 | 771 | 34 | 7 |
| IP35, other objects | 784 | 17,684 | 16,418 | 9,039 | 3,250 | 3,778 | 1,611 | 345 |

Objects with routines in only one release: IP27 21 in 6.5.30 only and 16 in 6.5.22 only; IP30 28 and 8; IP35 30 and 20. The "other objects" rows are out of the current scope and are counted here so the totals are complete. The moved (renamed) pairs number 5, 5 and 6 in the sysgen objects of IP27, IP30 and IP35.

## Sibling rows in the 6.5.30 pages

191 routines that the pages understand on 6.5.30 have bytes identical to a 6.5.22 copy of the same name. Twelve already had 6.5.22 rows (the `find_lboard` family in the `kernel.o` objects, in [klconfig.md](klconfig.md)). Another 107 now have a 6.5.22 row on their page:

- [klconfig.md](klconfig.md), in the table "The same routines in the other builds": 20 rows.
- [ip30-kernel-functions.md](ip30-kernel-functions.md), at the end: 31 rows.
- [nic.md](nic.md), at the end: 46 rows.
- [interrupts.md](interrupts.md), at the end: 4 rows.
- [syssgi.md](syssgi.md), at the end: 3 rows.
- [ip35-ioc3.md](ip35-ioc3.md), at the end: 2 rows.
- [ip35-hub-errors.md](ip35-hub-errors.md), at the end: 1 row.

The other 74 have no row yet. Their 6.5.30 entry on the page either has no role sentence for that build, or the page describes the routine in a layout the row generator did not read. Those entries need writing before the 6.5.22 row can state a role.

A linked-kernel row is labelled established when the whole body has no calls, address constants or gp accesses (the `find_lboard` family and `part_operation` are in that group). The others are inferred, because the 6.5.22 target of each call or constant was not checked. The role text in each row is the one the 6.5.30 entry gives, because the bytes are the same.

## Changed routines already understood on 6.5.30

553 routines that the pages understand on 6.5.30 have a 6.5.22 copy whose bytes differ. 499 of them are hardware-facing. Each has `understood_6.5.30` set to `yes` in the queue, so the 6.5.22 side can be read against the 6.5.30 entry.

## Reading order

[version-diffs-queue.tsv](version-diffs-queue.tsv) lists every changed routine, with its class and its scope. Rows are ordered as follows:

1. hardware-facing, then out of scope;
2. within each group, by class: code, size, immediate, layout, and object bytes (relocatable objects);
3. within a class, understood on 6.5.30 first, then by object and name.

The hardware-facing code and size rows are the first to read. The layout rows of the linked kernels differ only in jump targets and gp offsets, so the question they raise is whether the gp slots name the same data; that is recorded in [open-questions.md](open-questions.md).

## Limits

- A changed hash on a linked kernel is not proof of changed behaviour, and a layout result is not proof of unchanged behaviour. A gp offset that moved can name a different variable.
- Routines with the same name in several compilation units are paired with the first unused partner of that name. Those pairs are not checked one by one.
- The sizes come from the inventory's symbol table. A routine whose size changed is in the size class and is not compared byte by byte.
- No routine of a relocatable object is re-read in this pass. Their relocations were not compared, so a call that now reaches another routine can match the 6.5.30 bytes.
