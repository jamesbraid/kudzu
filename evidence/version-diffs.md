# IRIX 6.5.22 against 6.5.30: routine diffs

This page compares the routines of the IRIX 6.5.22 and 6.5.30 kernels. It covers the three linked kernels (`unix.IP27`, `unix.IP30` and `unix.IP35`) and the kernel object files of the IP27, IP30 and IP35 boot directories (`usr/cpu/sysgen/IP27boot` and so on). The per-object counts are in [version-diffs.tsv](version-diffs.tsv). The changed routines, hardware-facing first, are in [version-diffs-queue.tsv](version-diffs-queue.tsv).

## Inputs

- The 6.5.22 linked kernels are in `root/6.5.22/`. Their SHA-256 hashes were added to `tools/config/inventory-root.sha256`; they match a direct hash of the files. This repository does not record which installed system they came from.
- The scope rule now names the 6.5.22 linked kernels with the same name rule as 6.5.30 (`retools/tools/kudzu/gen-scope.py`). Before this, the 6.5.22 kernel rows were all counted as in scope.
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

Hardware-facing routines are those whose names start with a prefix in the narrow rule of [tools/config/scope.json](../tools/config/scope.json): the hardware graph, KLCONFIG, interrupts, NUMA, the crossbow, Heart and Bridge drivers and the panic entry points.

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

The code class was checked by reading the word-level differences for `intr_init_vecblk` in IP27. The two callee-saved registers `s2` and `s7` trade places in the stores and loads at words 1, 2, 5 and 19 of the body. The other differences are gp offsets (words 32, 63 and 94) and one jump target (word 67). The other code rows were not read in this pass; the queue is where they are read. The reads that have been done are in [Changed code routines read](#changed-code-routines-read).

## Changed code routines read

The fifteen hardware-facing routines whose class is code (ranks 1 to 15 of the queue) are read here. Each pair was compared over the whole body, with three normalisations: a branch target inside the routine is written as an offset from the routine's start, a call is written as the callee name at that call site, and a gp-relative load is written as the address it reads. The differences that remain after that are listed. Grades follow [AGENTS.md](../AGENTS.md) section 2.

| Kernel | Routine | 6.5.30 address | 6.5.22 address | Grade | Result |
|---|---|---|---|---|---|
| `unix.IP27` | `intr_init_vecblk` | `0xc000000000037f58` | `0xc000000000037c74` | established | Same 109 words, same stores and callee. See below. |
| `unix.IP35` | `intr_init_vecblk` | `0xc00000000005ee74` | `0xc00000000005f6fc` | established | Same pattern as IP27. See below. |
| `unix.IP27` | `klhwg_connect_hubs` | `0xc0000000000425bc` | `0xc000000000042034` | established | Same 141 words and the same twelve call sites in the same order. Differences are the save order of `s6` and `s8` (words 1 to 4) and the gp data slots, which point at different addresses. |
| `unix.IP27` | `hub_device_inquiry` | `0xc00000000003c59c` | `0xc00000000003c290` | established for the instructions; inferred for the effect in the last paragraph | Same 109 words, same frame, same callees in the same order. The final block differs. |
| `unix.IP27` | `ecc_printPending` | `0xc000000000056abc` | `0xc000000000056118` | established for the instructions; role unknown | Same instructions after the prologue, including three indirect calls (`jalr` at words 46, 53 and 67). The 6.5.30 side has no entry, so no role is given. |

Ten code-class routines are not read yet: the IP35 copies of `huberror_process_wrb_error`, `hubiio_crb_error_handler`, `ioc3_serial_attach`, `klhwg_add_node`, `ecc_printPending`, `elsc_ody_expander_set`, `ip27log_setup`, `klhwg_premium_std_dimm_mix_check` and `prom_reboot`, and the IP27 copy of `huberror_process_wrb_error`. The IP27 `huberror_process_wrb_error` has its error-class branches in a different order (about 80 of its 139 words differ after normalisation), so it needs its own pass.

**`intr_init_vecblk` (IP27 and IP35).** Both builds save `s2` and `s7` at the start and reload them at the end. Both load the same two pointers into them: the `intr_stray` routine and the routine the inventory names `spl7` and `splerr` (one address). The 6.5.22 build does the two saves and two loads in the opposite order: its word 1 saves `s7`, its word 5 saves `s2`, its word 2 loads `s7` and its word 19 loads `s2`. The registers end up holding the same pointers, so this is a scheduling difference. The gp slots that hold the pointers sit at different addresses in each build because the gp value differs (IP27: `0xc0000000014d2170` in 6.5.30, `0xc0000000014d0ef0` in 6.5.22). The loads at words 32, 63 and 94 read the same kinds of pointer in both builds, at different addresses. The jump at word 67 goes to `init_spinlock` in both builds, at `0xc000000000286180` (6.5.30) and `0xc000000000286fdc` (6.5.22). The IP35 build has the same pattern, with `init_spinlock` at `0xc000000000331d40` and `0xc0000000002fa8dc`.

**`klhwg_connect_hubs` (IP27).** The call sites are `is_specified`, `find_lboard`, `find_first_component`, `is_specified`, `board_to_path`, `hwgraph_traverse`, `board_to_path`, `hwgraph_traverse`, `hwgraph_edge_add` and three `cmn_err` calls, in the same order in both builds. The callee addresses differ, as they do for every routine that moves. The one unreached block sits at the same offset (0x1cc) from the start in both.

**`hub_device_inquiry` (IP27).** The calls are the same in both builds: `sprintf`, `hwgraph_traverse`, `device_master_get`, `hwgraph_info_get_LBL` and six `cmn_err` calls at the same offsets. The final block differs. On the path where `a3` is zero, 6.5.30 branches from word 96 to word 106, which loads the gp pointer into `a2` and then jumps to the register restore at word 99. 6.5.22 branches from word 96 to word 107, which jumps to word 100 and does not reload `a2`. On the other path, 6.5.22 loads the gp pointer twice (words 97 and 98) before adding 436 at word 99, where 6.5.30 loads once and adds at word 98. Whether the `a2` that reaches `cmn_err` on the 6.5.22 zero path is the same as in 6.5.30 depends on what the earlier paths set. That was not checked on every path. Inferred.

**`ecc_printPending` (IP27).** Only the prologue differs. Two table bases are built from `lui` and `addiu` immediates. In 6.5.30 they are `0xc0000000014d5020` (`lui 0x14d` and 20512) and `0xc0000000014109f0` (`lui 0x141` and 2544). In 6.5.22 they are `0xc0000000014d39a0` (`lui 0x14d` and 14752) and `0xc00000000140fb98` (`lui 0x141` and -1128). The saves of `s0` and `s3` are also in the opposite order. The rest of the body, including the three indirect calls, has the same instructions. Its purpose is not stated, because the 6.5.30 entry for it does not exist yet.

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

191 routines that the pages understand on 6.5.30 have bytes identical to a 6.5.22 copy of the same name. Twelve already had 6.5.22 rows (the `find_lboard` family in the `kernel.o` objects, in [klconfig.md](../formats/klconfig/README.md)). Another 107 now have a 6.5.22 row on their page:

- [klconfig.md](../formats/klconfig/README.md), in the table "The same routines in the other builds": 20 rows.
- [ip30-kernel-functions.md](../software/kernel/ip30-kernel-functions.md), at the end: 31 rows.
- [nic.md](../formats/nic_record/README.md), at the end: 46 rows.
- [interrupts.md](../software/kernel/interrupts.md), at the end: 4 rows.
- [syssgi.md](../software/kernel/syssgi.md), at the end: 3 rows.
- [ip35-ioc3.md](../software/kernel/ip35-ioc3.md), at the end: 2 rows.
- [ip35-hub-errors.md](../software/kernel/ip35-hub-errors.md), at the end: 1 row.

The other 74 have no row yet. Their 6.5.30 entry on the page either has no role sentence for that build, or the page describes the routine in a layout the row generator did not read. Those entries need writing before the 6.5.22 row can state a role.

A linked-kernel row is labelled established when the whole body has no calls, address constants or gp accesses (the `find_lboard` family and `part_operation` are in that group). The others are inferred, because the 6.5.22 target of each call or constant was not checked. The role text in each row is the one the 6.5.30 entry gives, because the bytes are the same.

## Changed routines already understood on 6.5.30

553 routines that the pages understand on 6.5.30 have a 6.5.22 copy whose bytes differ. 499 of them are hardware-facing. Each has `understood_6.5.30` set to `yes` in the queue, so the 6.5.22 side can be read against the 6.5.30 entry.

## What the moved gp slots name

The 1,186 hardware-facing routines whose class is layout and whose differences include gp offsets (106 with gp offsets alone, 1,080 with gp offsets and jump targets) were checked slot by slot with `tools/gp_slots.py`; the result is [version-diffs-gp-slots.tsv](version-diffs-gp-slots.tsv), one row per differing gp-relative operand of each routine (2,396 rows in 1,182 routines).

Method. Both copies of a routine have the same instruction sequence, so the instructions are paired by position. At each pair whose gp offset differs, gp plus the offset is resolved in each kernel (gp from the kernel's `.MIPS.options` record): a symbol at that address is a variable; a pointer-sized word in a literal or data section is a pool word, named by the symbol or the string it points to; any other word is a constant, compared by value (four-byte accesses compare the word). A pool word that points to a routine or table with no symbol is compared by its first 32 bytes with call targets, gp offsets and pointers masked.

Result (**established** for what the method can see):

| Verdict | Rows |
|---|---:|
| the slot names the same symbol, string or value in both kernels | 2,372 |
| the slot is an unnamed static in both kernels (no symbol in either; the first 32 bytes of a pointed-to routine or table are equal in 160 of the 2,372 above) | 21 |
| the slot differs | 3 |

- Every moved gp slot of the 106 gp-only routines, including the examples `cnode_exists`, `nasid_to_compact_nodeid` and `hub_slot_to_widget`, names the same thing in 6.5.22 as in 6.5.30 on each platform (the compact-node table, the slot-bits table, the register address constants). The offsets moved because the data around them moved. Two exceptions are weaker readings: three slots (in `hub_slot_to_widget`, `hub_slotbits_to_slot` and `router_slotbits_to_slot`) point to a table with no symbol and match by the first 32 bytes only, and the two `ioc3_console_vhdl_get` slots (IP27 and IP35) are unnamed statics in both kernels.
- The 21 unnamed rows are the same few statics seen from several routines: the ioc3 console record pointer (`ioc3_attach`, `ioc3_handle_intr` and `ioc3_intr` on IP30, `ioc3_console_vhdl_get`, `ioc3_is_console`), the PROM-information list head (`prominfo_add`, `prominfo_del`, `prominfo_nodeget`), the ioc3 serial record (IP30 `ioc3_serial_attach`) and a table read by `idbg_dnlc`. For the first, `ioc3_attach` stores the pointer at the same position in the routine in both releases of the IP27 and IP35 kernels (a doubleword store 0x178 bytes into the routine; the IP30 pair was not checked). That the other unnamed slots are the same static is **inferred** from the same routine reading the same position, not read per slot.
- The 3 differing rows are one slot, in `ioc3_intr_connect` on each platform. The pool word holds the address of a routine with no symbol, and the two releases' routines differ at the start (the 6.5.30 one saves with a 32-byte frame, the 6.5.22 one with a 16-byte frame). The handler registered through this routine has different code in the two releases. **Inferred** from the first 32 bytes only; the handlers were not read.
- Four routines of the 1,186 have no differing gp-relative load or store: `ecc_exception` on the three platforms and `intrnorm_rsasave` on IP27 build the kernel's own gp with a `lui`, `daddiu` and `dsll` chain (the chain's immediates are what differ) and then call `tfi_save` (`ecc_exception`) or save the registers themselves and jump to `ecommon_allsaved`, past the `tfi_save` call in `ecommon` (`intrnorm_rsasave`); no slot is read.

## Reading order

[version-diffs-queue.tsv](version-diffs-queue.tsv) lists every changed routine, with its class and its scope. Rows are ordered as follows:

1. hardware-facing, then out of scope;
2. within each group, by class: code, size, immediate, layout, and object bytes (relocatable objects);
3. within a class, understood on 6.5.30 first, then by object and name.

The hardware-facing code and size rows are the first to read. The layout rows of the linked kernels differ only in jump targets and gp offsets, so the question they raise is whether the gp slots name the same data; [What the moved gp slots name](#what-the-moved-gp-slots-name) answers it.

## System call and syssgi selector tables

The two system call tables (`sysent` and `irix5_64_sysent`) and the `syssgi` selector dispatch were extracted from the three linked kernels of both releases, with [retools/tools/syscall_table.py](../retools/tools/syscall_table.py) and [retools/tools/syssgi_selectors.py](../retools/tools/syssgi_selectors.py) through `retools/tools/kudzu/os_tables.py`. The rows are in [syscalls.tsv](inventory/syscalls.tsv) and [selectors.tsv](inventory/selectors.tsv), where the release is the prefix of the `binary` column. The differences are one row each in [version-table-diff.tsv](inventory/version-table-diff.tsv), written by [tools/version_table_diff.py](../tools/version_table_diff.py). Its `grade` column means: **established**, read from the tables or the argument-table bytes; **class-only**, the routine differs by the byte class in the queue and its body was not read; **heuristic**, taken from the extractor's own reading of a compare tree or of the first call in a block, so the row is a lead to read, not a finding.

### System calls

- Numbers and names. Each table has the same 237 call numbers in both releases, in all three kernels. No number is added, removed or renumbered, and the handler name at every number is the same (211 distinct handlers per kernel in `sysent`, 201 in `irix5_64_sysent`). The argument-count and flag bytes are the same at every number. Established.
- Argument tables. The table address moves in every entry, because the relocation shifts all data. The argument-type bytes of every entry are the same in the two releases, in all three kernels and both tables: the comparison reads up to 96 bytes at each table and stops at the first kernel address, because the type tables are packed one after another and an address word is the next data object, which moves with the image. A first pass that compared past that address word reported 75 entries as different; the ones read (`poll` in `sysent` and call 1162 in `irix5_64_sysent`, on IP27) differed only in that neighbouring address and in the text `sgimips` that 6.5.30 places after it, so those rows were wrong and are gone. The table length is not recorded, so the bytes after the first address are not compared. Established for the compared bytes.
- Handler bodies. The queue puts 27 `sysent` handlers of IP27, 28 of IP30 and 27 of IP35 in a byte class other than layout (rows `syscall-handler-body`, class-only). Per kernel the classes are 17 immediate, 8 or 9 size, and 1 to 3 code. The code-class routines were read at the instruction level:
  - `uadmin` (IP27): both builds have 170 instructions. Before word 25, the differences are branch targets and gp offsets. At word 25, 6.5.22 loads a gp word (`lw $t4,-0x6088($gp)`) that 6.5.30 does not load there. 6.5.30 loads a gp word at word 41 (`lw $t4,-0x6030($gp)`), where 6.5.22 has `addiu $s1,$zero,1`. What the reordered block does was not read.
  - `ptrace` (IP30): both builds have 179 instructions. Before word 61, the differences are branch targets and gp offsets. At word 61, 6.5.22 has `addiu $a1,$zero,4` and 6.5.30 has `ld $a0,0x18($sp)`. The effect was not read.
  - `nfs_getfh` (IP30 and IP35): both builds have 130 instructions. Stack and gp loads move between the two, and the instruction streams stop lining up after the first move. Not resolved.
- Seven handlers (`getpagesize`, `nopkg`, `setgid`, `setuid`, `sgi_sesmgr`, `timer_getoverrun`, `umask`) have the same code hash in both releases, in all three kernels ([functions.tsv](inventory/functions.tsv)), so their bodies are byte-identical. They have no queue row. Established.

### syssgi selectors

- IP27. The jump table grows from 1239 to 1261 entries. Selectors 1239 to 1259 are added and share one target (`0xc000000000231848`, reached by 1018 selectors in 6.5.30). Selector 1260 is added with its own target (`0xc000000000231894`). Established.
- IP30. The table grows from 1251 to 1261 entries. Selectors 1251 to 1259 are added and share one target (`0xa8000000201eba10`, reached by 1022 selectors). Selector 1260 is added with its own target (`0xa8000000201eba5c`). The 6.5.22 tables of IP27 and IP30 have different lengths (1239 and 1251). Both have 1261 entries in 6.5.30. Established.
- IP35 (compare tree). The extractor resolves 248 selectors to code in 6.5.22 and 257 in 6.5.30. Nine selectors resolve only in 6.5.30: 23, whose target is the shared error path that 11 selectors reach, and 199, 250, 365, 366, 1064, 1065, 1066 and 1260, each with its own target. A compare tree has no entry to read, so a selector the extractor did not narrow in the older image looks the same as a new one. For 250, 365, 366, 1064, 1065 and 1066 the IP27 and IP30 jump tables show the same selectors new, which supports them; the rest are heuristic.
- Shared targets change. In IP27 and IP30, selectors 250, 365, 366, 1064, 1065 and 1066 were shared in 6.5.22 and have their own targets in 6.5.30 (their first calls are `copyin` and `cap_able`). Selector 193 goes the other way. Both come from the table entries and are established. In the IP35 compare tree, 27 selectors change between sharing a target and having their own, but that is a heuristic: the address the extractor reports can be the test block in front of a body that both releases share. Selectors 118 and 119 show it. In 6.5.22 both reach one body that stores zero through the result pointer and returns 0. In 6.5.30 that body is still shared, with 118 entering through its own test, so 118 changes from shared to own target in the table although the code is the same. The rows are graded heuristic.
- First calls (heuristic). The first routine a target calls differs by name for 8 selectors in IP27 and 8 in IP30: 250, 365, 366, 1064, 1065 and 1066 go from no call to `copyin` or `cap_able` (the 6.5.22 target is the shared fill), selector 1062 goes from `cap_able` to `nfs_generic_cmd`, and selector 66 goes between `copyin_xlate` and no call. The 33 other selectors per kernel whose first call is a routine the extractor could not name, at a moved address, are not compared. The IP35 compare tree is not compared at all. There the target can be a test block that branches before it reaches its call: in 6.5.22 the block for selector 145 starts with that test and its first call is the routine at `0xc0000000002acbc4`, where the table said no call, and selector 1233 is the same in 6.5.30. A blank first call means the block branches first, not that the handler calls nothing, and a changed first call does not by itself show a changed handler.

The `sysmp` and other selector functions are not in these tables; see [open-questions.md](../questions/README.md).

## Limits

- A changed hash on a linked kernel is not proof of changed behaviour, and a layout result is not proof of unchanged behaviour. A gp offset that moved can name a different variable.
- Routines with the same name in several compilation units are paired with the first unused partner of that name. Those pairs are not checked one by one.
- The sizes come from the inventory's symbol table. A routine whose size changed is in the size class and is not compared byte by byte.
- No routine of a relocatable object is re-read in this pass. Their relocations were not compared, so a call that now reaches another routine can match the 6.5.30 bytes.
- The argument-table comparison covers up to 96 bytes of each table and stops at the first kernel address. The table length is not recorded, so the bytes after that address are not compared, and a neighbouring table before it can be.
- The syscall handler rows of class other than layout are class-only: their bodies were not read, except the code-class routines named above.
