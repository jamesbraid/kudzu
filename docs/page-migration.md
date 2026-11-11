# Page migration and cross-partition communication

This page answers two questions about the IRIX 6.5.30 kernels, IP35 first (Origin 300/350, Onyx 300/350) with the IP27 (Origin 200/2000, Onyx2) differences noted: whether the kernel moves pages between nodes by itself, and whether it has a facility for talking to, or sharing memory with, another partition. Evidence is the linked kernels `6.5.30/unix.IP35` and `6.5.30/unix.IP27`, the kernel objects under `6.5.30/usr/cpu/sysgen/IP35boot/` and `IP27boot/`, the interrupt table already read in [interrupts.md](interrupts.md), and the selector pages [syssgi.md](syssgi.md) and [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md). The 6.5.22 configuration tree (`root/6.5.22/var/sysgen.IP35` and `sysgen.IP27`) is used for the tunable comments and the module list; the 6.5.30 `mtune/numa` file is byte for byte the 6.5.22 one. The user-level programs, the C library and the manual pages come from the 6.5.30 `eoe` and `dev` products on the installation media; they are not in `root/`.

Grades follow [AGENTS.md](../AGENTS.md): established is read in full in a binary at the address given, inferred states its reasoning, and anything unlabelled or listed under [Unknowns](#unknowns) is unknown. No kernel or library code was run.

## Answers

| Question | Answer | Grade | Where |
|---|---|---|---|
| Does automatic (hardware-counter driven) page migration exist in IP35 6.5.30, under any name? | No. The shipped 6.5.30 IP35 kernel does not link it, and the IP35 object archive carries empty objects where IP27's migration code is. Nothing among its 15122 routines reads a page reference counter, connects the migration interrupt, runs a periodic migration tick or runs a migration manager. Every caller of a page-moving routine was followed to its root: a user request (`pmoctl`, `sysmp` cpuset moves, a range attribute change) or the large-page coalescing thread. The kernel does arm one hardware threshold register per node at boot and nothing reads the result. | Established for the absent routines, the callers and the unread register; inferred for the verdict that no mechanism exists under another name. | [Looking for an equivalent mechanism on IP35](#looking-for-an-equivalent-mechanism-on-ip35) |
| Was IP35 ever given automatic migration? | Yes. The IP35 object archives of 6.5.10 to 6.5.16 (October 2000 to April 2002) carry the full engine, a handler on interrupt level 3 and a counter reader; 6.5.17, 6.5.20 and 6.5.21 empty it in three steps and the manual pages and tuning file are rewritten to "Origin 2000/200 only" over the same period. The 15-bit threshold routine is what remains. Whether it ever ran on Origin 3000 hardware, and why it was withdrawn, are not recorded in any held release note or error string. | Established (objects, dates, texts); the reason is unknown | [Release history](#release-history-of-migration-and-replication-65-8-to-6530) |
| Is "built for the Origin 2000 only" proved? | No: the 6.5.8 to 6.5.16 objects show IP35 code was built and shipped. The shipped manual pages say from 6.5.15 that automatic migration is available on the Origin 2000/200 only. | Established (that IP35 code existed); the reason for the withdrawal is unknown | [Release history](#release-history-of-migration-and-replication-65-8-to-6530), [Verdict](#verdict) |
| What migration does IP35 keep? | Migration on request: a placement-module control call, address-space moves when a process joins or changes cpuset, a placement-attribute change on a range, and the large-page coalescing thread. Poisoned-page handling consults the migration code (it asks whether a page is being moved and unmaps it from the TLBs). | Established (call edges); roles inferred. | [What still moves pages on IP35](#what-still-moves-pages-on-ip35) |
| Policy interfaces | `syssgi` 208 (placement-module control, commands 40 to 42 for migration), 212 (tunable block read and write), 218 (statistics); three migration policy modules registered by name, whose service routines only destroy, name, hand back a parameter record and reject every other code; a hub-special `refcnt` device whose handlers return an error on IP35; no `/proc` migration control found on IP35. | Established. | [Policy interfaces](#policy-interfaces), [Policy modules and their services](#policy-modules-and-their-services) |
| Hardware counters read | None read by any IP35 routine. The one migration register the kernel touches, at offset 0x40 of `0x9200000001fc0000`, is written once per node at boot; a public vendor header names it as the page-migration count-difference threshold. IP27 reads three registers per node through the hub window. | Established (accesses); inferred (the register name). | [Hardware that counts references](#hardware-that-counts-references) |
| Daemon or thread | IP35: the large-page coalescing thread `coalesced` is the only thread that moves pages without a user call. IP27: the migration interrupt handler and the clock tick. No `memoryd`. | Established (call edges); inferred (roles). | [What drives migration](#what-drives-migration) |
| Tunables | 47 `numa_*`, `mem_tick_*`, `memoryd_enabled` and `coalesced_*` names with defaults; of the 41 migration-related ones IP35 reads three, and nine of all 47 have any reader. | Established (defaults and read sites). | [Tunables](#tunables) |
| User level | `dplace`, `sn`, `nstats`, `cpuset`, the placement library and the C library request migration or set parameters; no daemon drives it. | Established for the files held. | [User level](#user-level) |
| Cross-partition facility? | Yes in the shipped objects: a partition layer, a message layer (XPC) and a shared-memory driver (XPMEM), plus a character device and a network interface over XPC. The linked IP35 kernel contains XPMEM, which initialises as partition 0 and works locally, but only panicking stubs for XPC and the partition layer, so nothing can reach another partition in it. IP27 has no XPMEM and the same stubs. | Established. | [Cross-partition communication on IP35](#cross-partition-communication-on-ip35) |
| Messages only, or shared memory? | Both: XPC carries messages (control, data pulled with the block-transfer engine, inter-partition interrupts); XPMEM maps another partition's pages into a process and also copies with the block-transfer engine. | Established (call edges); inferred (end-to-end behaviour). | [XPMEM](#xpmem) |

## Automatic page migration on IP35

On IP27 the kernel moves pages on its own: a hub interrupt (level 3, "Migration") fires when a remote node's per-page access count exceeds the local one by a threshold, a handler reads the counters and calls the migration manager, and a clock tick drives periodic filters. None of those pieces exists in the IP35 kernel. The 6.5.22 `mtune` file for both machines (`root/6.5.22/var/sysgen.IP35/mtune/numa`) has the heading "Migration Parameters (Origin 2000 only)" over the migration parameters and "Extended Reference Counter Parameters (Origin 200 and Origin 2000 only)" over the extended reference-counter parameters, and its threshold comment describes the interrupt-on-difference rule; that is the vendor's statement of intent, and the routine evidence is below.

- `numa_init` (`6.5.30/unix.IP35`, address 0xc0000000003272f0, 96 bytes) walks the table `numa_init_table` (0xc0000000015e95c8, 19 entries of an init routine pointer and a name pointer, then a zero), calls each routine with a zero argument, and on a non-zero result calls `cmn_err` with the module name and the result (call at 0xc000000000327340). The names in order are Mld, Raff, MldSet, PmoList, Topology, Migr, six "Placement Policy" modules, two "Fallback Policy" modules, "Migration Policy: Default", "Migration Policy: Control", "Migration Policy: Refcnt", MemTickInit and AssignWards. The migration entries are `migr_init`, `migr_policy_default_init`, `migr_policy_control_init`, `migr_policy_refcnt_init` and `mem_tick_init`. Established.
- `mem_tick_init` (`6.5.30/unix.IP35`, address 0xc0000000003309d4, 8 bytes) is `jr ra` with `move v0,zero` in the delay slot: it does nothing and reports success. The IP27 routine of the same name (0xc00000000027a840, 124 bytes) calls `mem_tick_init_node`, which reads `numa_migr_default_mode` and `mem_tick_enabled`; the call is at 0xc00000000027a884 inside `mem_tick_init`, and `mem_tick_init_node` itself is at 0xc00000000027a424. Established.
- `migr_init` (`6.5.30/unix.IP35`, address 0xc00000000032b504, 176 bytes) loops over `numnodes`, calls `migr_effthreshold_diff_set` (0xc00000000032b5b4, 64 bytes) with a value computed from a per-node record, and clears the doubleword at offset 40 of that record. It does not call the IP27 `migr_refcnt_init`. Established.
- `migr_effthreshold_diff_set` (`6.5.30/unix.IP35`, address 0xc00000000032b5b4, 64 bytes) takes a node number and a count and stores the doubleword (0x7fff minus the low 32 bits of the count) with bit 63 set at offset 0x40 of a window whose base word is 0x9200000001fc0000 (the pool word at gp+2376 holds that value in the kernel data) plus a halfword looked up for the node, shifted left 33 (`dsll32` by 1 at 0xc00000000032b5dc). `migr_init` computes the count as the byte at +2 of a per-node structure times the word at +0xc, divided by 100. The IP27 routine of the same name (0xc00000000027d6f0, 184 bytes) stores at offset 0x30 of the 0x9200000001a00000 window with the node halfword shifted left 32 (`dsll32` by 0 at 0xc00000000027d750 and 0xc00000000027d790), a one-bit difference in the node shift from IP35 that matches the two address layouts (see [Hardware that counts references](#hardware-that-counts-references)). It has two paths: an earlier one (0xc00000000027d6f0 to 0xc00000000027d760) that stores a value held in a pool word (gp-0x5680) after tests on the node record that were not decoded, and one (from 0xc00000000027d764) that stores 0xfffff minus the count with bit 63 set. A public vendor header names the IP35 register as the page-migration count-difference threshold ([Hardware that counts references](#hardware-that-counts-references)); the width difference (a 15-bit field on IP35, a 20-bit field on IP27) is read from the two constants. Established for the stores; the register name is inferred.
- `migr_control_init` (`6.5.30/unix.IP35`, address 0xc00000000032dfe8, 600 bytes) allocates a 0x50-byte record per node with `low_mem_alloc`, reads only `numa_migr_default_mode` and `numa_migr_min_maxradius`, and stores 1 to `numa_migr_vehicle` (0xc00000000032e148). It clears the top nibble of the first byte of the record and then sets bits 0x10 for mode 1, 0x20 for mode 3, 0x30 for mode 2 and, for mode 4, 0x30 when `physmem_maxradius` is below `numa_migr_min_maxradius` and 0x20 otherwise; mode 0 leaves the nibble clear; any other mode calls `cmn_err` with the message "Invalid Default Migration Mode, Disabling migration" and also leaves it clear. It initialises a spin lock at offset 0x48 of the record, and also clears the top nibble of byte 1 and stores to offsets 0x37, 0x4c and 0x4d of it. The byte at 0x4c is the result of `klhwg_premium_std_dimm_mix_check` (0xc000000000074520, 560 bytes; call at 0xc00000000032e020) for the node. The IP27 routine (0xc000000000281558, 3176 bytes) also reads the threshold, mechanism, filter, queue and bounce tunables; the IP35 routine reads none of them. Established.
- `per_hub_init` (`6.5.30/unix.IP35`, address 0xc00000000004e31c, 612 bytes) calls `intr_reserve_hardwired`, `hub_error_init`, `install_eccintr`, `protect_hub_calias` and `hub_rtc_init`, and no migration routine. Established.
- `per_hub_init` (`6.5.30/unix.IP27`, address 0xc000000000026acc, 1000 bytes) makes the same five calls, plus `hub_setup_kick` and `hub_init_ifdr_war`, and, after `hub_rtc_init`, calls `hub_migrintr_init` (call at 0xc000000000026de0), which connects interrupt level 3 to `migr_intr_handler`. Established.

The shipped 6.5.30 IP35 kernel does not link `hub_migrintr_init`, `migr_intr_handler`, `migr_manager`, `migr_start`, the `migr_refcnt_*` family or `mem_tick`: none of those names is in the IP35 symbol table or its debug information ([inventory/functions.tsv](inventory/functions.tsv)). Established.

The hardwired interrupt table of the IP35 kernel (`hardwired_intr`, 0xc000000001531598, 416 bytes) still reserves level 3 with the name "Migration" and the threaded-handler flag 0x8, as on IP27 ([interrupts.md](interrupts.md)). The reservation alone does not connect a handler; the connect call is the one IP35 lacks. No routine in the IP35 kernel connects a handler to level 3 ([The migration interrupt](#the-migration-interrupt)). Established.

The IP35 migration tunables in the 6.5.22 `mtune` list carry the same headings as IP27's ("Origin 2000 only" over the migration parameters, "Origin 200 and Origin 2000 only" over the extended reference counters), because the file is shared. The data words in the linked IP35 kernel carry the same defaults as in IP27 ([Tunables](#tunables)). A scan of every global-pointer access in the whole text, including the 5109 routines with no symbol-table entry, finds a reader for nine of the 47 names and none for the other 38 ([Tunables](#tunables)).

## What still moves pages on IP35

The page mover and its callers are present. These are call edges read from the IP35 kernel (callers found by scanning every `jal` in the text for the callee; calls through pointers are not found).

- `migr_pagemove` (`6.5.30/unix.IP35`, address 0xc00000000032ba90, 604 bytes) copies one 16 KiB page (the length 0x4000 passed at the `bte_pbcopy` call at 0xc00000000032bb3c) with the block-transfer engine, or with `bte_poison_copy` (call at 0xc00000000032bc08; the poisoned-page case is inferred from the name). Two error paths report through `cmn_err`: one prints the value of `numa_migr_vehicle` with the message "Invalid Migration Vehicle" (call at 0xc00000000032bcc4, load of the tunable at 0xc00000000032bcc0), and one prints a failed `bte_copy` result (call at 0xc00000000032bcdc). Established for the calls; that vehicle value 1 means the block-transfer engine is inferred from the calls and from the build flag `SN1_USE_BTE` in the compile line recorded in the kernel image.
- `migr_page_prologue` (`6.5.30/unix.IP35`, address 0xc00000000032bcec, 376 bytes) transfers the page's identity from the old frame to the new one: it calls `anon_pagemigr` (call at 0xc00000000032bd9c) for an anonymous page or `vnode_pagemigr` (call at 0xc00000000032bdfc) for a file page. The anonymous-or-file split is inferred from the names. Established for the calls.
- `migr_migrate_frame` (`6.5.30/unix.IP35`, address 0xc00000000032bf8c, 564 bytes) is the single-frame mover: it calls `migr_pagemove` (call at 0xc00000000032c038) and `migr_page_epilogue` (call at 0xc00000000032c068). `plac_policy_sync` (`6.5.30/unix.IP35`, address 0xc000000000329cbc, 1896 bytes) reaches it directly (call to `migr_migrate_frame` at 0xc00000000032a1e8) as part of placement-policy synchronisation; the policy meaning is not traced. Established for the edges.
- `migr_migrate_page_list` (`6.5.30/unix.IP35`, address 0xc00000000032c380, 1020 bytes) moves a list of pages: it calls `migr_pagemove` (call at 0xc00000000032c628) and `migr_page_epilogue` (call at 0xc00000000032c5d8). Established for the calls.
- `migr_move_lpage` (`6.5.30/unix.IP35`, address 0xc00000000032ed54, 1412 bytes) moves a large page: it calls `pagealloc_size_node` (call at 0xc00000000032edb8) for the destination, `anon_pagemigr` (call at 0xc00000000032ee78) or `vnode_pagemigr` (call at 0xc00000000032eeec), `migr_pagemove` (call at 0xc00000000032efec) and `migr_page_epilogue` (calls at 0xc00000000032f06c and 0xc00000000032f090), and undoes a failed move with `anon_page_unmigr` (call at 0xc00000000032f170). Established for the calls.
- `migr_xface_migrate` (`6.5.30/unix.IP35`, address 0xc00000000032e3cc, 248 bytes) is the user-requested migration entry. It copies in a 16-byte argument block with `copyin_xlate` (call at 0xc00000000032e3fc), looks up a placement object with `pmo_ns_find` (call at 0xc00000000032e430) and calls `migr_user_range_migrate` (call at 0xc00000000032e4a0). Established.
- `migr_user_range_migrate` (`6.5.30/unix.IP35`, address 0xc00000000032e4f0, 1012 bytes) checks the range with `is_kuseg_abi` (call at 0xc00000000032e554), takes the destination node from a memory-locality object through `pmolist_getelem` and `mld_getnodeid` (calls at 0xc00000000032e5ac and 0xc00000000032e5b4), locks the address space (`as_lookup_current`, `mraccessf`), finds the region with `findpreg`, probes pages with `pmap_probe` and migrates each with `migr_user_page_migrate` (call at 0xc00000000032e7d0). Established.
- `migr_user_page_migrate` (`6.5.30/unix.IP35`, address 0xc00000000032ec10, 324 bytes) moves a large page with `migr_move_lpage` (call at 0xc00000000032ec88) and a base page with `migr_migrate_page` (call at 0xc00000000032ecf8). Established for the calls.
- `migr_vas_pages` (`6.5.30/unix.IP35`, address 0xc00000000032e8e4, 812 bytes) walks a whole address space: it calls `as_lookup`, `findfpreg`, `pmo_ns_mld_lookup_node` (call at 0xc00000000032ea78), `pmap_probe` (call at 0xc00000000032eaf4) and `migr_user_page_migrate` (call at 0xc00000000032eaa0). Established for the calls.
- `as_migrate` (`6.5.30/unix.IP35`, address 0xc00000000028dd0c, 772 bytes) moves a whole address space with `migr_vas_pages` (call at 0xc00000000028df60) and re-links thread affinity with `kthread_afflink_migrate` (calls at 0xc00000000028dee8 and 0xc00000000028df40). Established for the calls; the affinity role is inferred from the name.
- `pmo_xface_as_migrate` (`6.5.30/unix.IP35`, address 0xc00000000028e080, 52 bytes) calls `as_migrate` (call at 0xc00000000028e098). Established.
- `pmo_xface_as_migrate_close` (`6.5.30/unix.IP35`, address 0xc00000000028e0b4, 52 bytes) calls `as_migrate_close` (call at 0xc00000000028e0cc). Established.
- `cpuset_move_to_global` (`6.5.30/unix.IP35`, address 0xc000000000249c94, 512 bytes) calls `as_migrate` (call at 0xc000000000249ddc) and `as_migrate_close` (call at 0xc000000000249dec). Established.
- `cpuset_move_pid` (`6.5.30/unix.IP35`, address 0xc00000000024a1b0, 1148 bytes) calls `as_migrate` (call at 0xc00000000024a5f4). Established.
- `cpuset_migr_cleanup` (`6.5.30/unix.IP35`, address 0xc000000000249f58, 164 bytes) calls `as_migrate_close` (call at 0xc000000000249fe4). So moving a process into or out of a cpuset migrates its memory; the effect is inferred from the names. Established for the call edges.
- `coalesce_leaf_chunk` (`6.5.30/unix.IP35`, address 0xc00000000027ae7c, 608 bytes) calls `migr_coald_test_pagemoves` (call at 0xc00000000027af5c) and `migr_coald_move_pages` (call at 0xc00000000027afe0). Established for the calls.
- `migr_coald_move_pages` (`6.5.30/unix.IP35`, address 0xc00000000032db84, 980 bytes) hands pages to `migr_migrate_page_list` (call at 0xc00000000032dca4). The coalescing engine is driven by the thread `coalesced` ([What drives migration](#what-drives-migration)), so large-page coalescing moves small pages out of the way with the same mover; the purpose (compaction) is inferred. Established for the call.
- `error_check_poisoned_state` (`6.5.30/unix.IP35`, address 0xc00000000005c9a4, 576 bytes) calls `migr_check_migr_poisoned` (call at 0xc00000000005ca28) and `migr_page_unmap_tlb` (call at 0xc00000000005ca44), so the memory-error path asks the migration code whether a page is being moved. Established for the calls; the role is inferred.

- `pm_sync_policy` (`6.5.30/unix.IP35`, address 0xc0000000003253a4, 696 bytes) walks a user range, locks the address space, and for each placement attribute run calls the placement policy's service routine with code 3; the placement policies' code 3 calls `plac_policy_sync`, which calls `migr_migrate_frame`. Its one caller is `pas_madvise` (0xc0000000002bf8d8, 448 bytes). Established for the edges.

The full list of callers is in [Every caller of a page mover](#every-caller-of-a-page-mover).

Names that look like migration and are not page migration: `migrate_timeouts` (0xc000000000068a2c, 604 bytes; callers `cpuset_init`, `failsoft_avoid` and twice `sysmp`) and `kthread_afflink_migrate` move timeout entries and thread affinity links when a processor goes away or a thread changes cpu. They are listed in the inventory with the same prefix. Inferred from the callers.

## Policy interfaces

- `pmoctl` (`6.5.30/unix.IP35`, address 0xc00000000032f558, 1064 bytes) is `syssgi` selector 208, described in [syssgi.md](syssgi.md). Its jump table (0xc0000000004d47a8, 103 doublewords) sends the three migration commands to the same callees as IP27: the entries at index 40, 41 and 42 hold the case addresses 0xc00000000032f6f0, 0xc00000000032f6d8 and 0xc00000000032f6c0, which call `migr_xface_migrate`, `pmo_xface_as_migrate` and `pmo_xface_as_migrate_close` respectively. So migration on request is placement-module control commands 40 (range migrate), 41 (address-space migrate) and 42 (close of the address-space migrate). Established.
- `numa_tune` (`6.5.30/unix.IP35`, address 0xc000000000330cf8, 836 bytes) is selector 212. It reads and writes a per-node block of tunables (commands 100 and 101); the byte layout is partly traced in [syssgi.md](syssgi.md). Command 101 validates the block with `migr_parms_check` (calls at 0xc000000000330eb0 and 0xc000000000330ed4; the routine is at 0xc0000000003309dc, 796 bytes, against 828 bytes on IP27) and copies it. Unlike IP27 it does not quiesce or restart the memory tick. Established.
- `numa_stats_get` (`6.5.30/unix.IP35`, address 0xc00000000031c3c0, 192 bytes) is selector 218 and returns the per-node statistics block; `numa_stats_init` (0xc00000000031c37c, 68 bytes) is called from `nodepda_numa_init` (0xc000000000327350, 52 bytes). Established.
- `nodepda_numa_init` (`6.5.30/unix.IP35`, address 0xc000000000327350, 52 bytes) calls `numa_stats_init` (call at 0xc000000000327360) and `migr_control_init` (call at 0xc00000000032736c). Established.
- `migr_policy_default_init` (`6.5.30/unix.IP35`, address 0xc000000000330848, 44 bytes) registers a policy factory by calling `pmfactory_export` (0xc000000000326fc0, 32 bytes; call at 0xc00000000033085c) with the module's create routine and its name. The Control and Refcnt initialisers (0xc00000000032e240 and 0xc0000000003301ec, both 44 bytes) do the same. The names the three `*_name` routines return are the strings "MigrationDefault", "MigrationControl" and "MigrationRefcnt". The policy modules are therefore selectable by those names through the placement-module interface (pm creation, command 20 and 29 of `pmoctl`); the selection path itself was not traced. Established for the registration and names; inferred for the selection.
- `migr_policy_default_migr_srvc` (`6.5.30/unix.IP35`, address 0xc000000000330928, 172 bytes) is a service dispatcher for service codes 0, 1, 2, 4 and 6; any other code reaches `cmn_err` with a message that names the policy and says "Invalid service type". The Control (0xc00000000032e2b0, 172 bytes) and Refcnt (0xc000000000330294, 172 bytes) dispatchers have the same shape. The codes are 0 destroy, 1 name, 2 no-op, 4 hand back the eight-byte parameter record at +0x70 of the module and 6 store a zero word; the callers that pass them are in [Policy modules and their services](#policy-modules-and-their-services). Established.
- `mem_refcnt_attach` (`6.5.30/unix.IP35`, address 0xc000000000329c60, 52 bytes) adds a character device named "refcnt" to the hardware graph under the vertex it is given, using the prefix "hubspc_" (call to `hwgraph_char_device_add` at 0xc000000000329c70), then calls `device_info_set` with 0 (call at 0xc000000000329c7c). `hubspc_init` hands it to `hubdev_register`, and the hub special-device entry points forward device-information value 0 to the `mem_refcnt_*` routines ([ip35-hub-errors.md](ip35-hub-errors.md)). Established.
- `mem_refcnt_open` (`6.5.30/unix.IP35`, address 0xc000000000329c94, 8 bytes) returns 19 without a call. `mem_refcnt_mmap` (`6.5.30/unix.IP35`, address 0xc000000000329ca4, 8 bytes) and `mem_refcnt_ioctl` (`6.5.30/unix.IP35`, address 0xc000000000329cb4, 8 bytes) are the same two instructions. Established.
- `mem_refcnt_close` (`6.5.30/unix.IP35`, address 0xc000000000329c9c, 8 bytes) returns 0 without a call, as does `mem_refcnt_unmap` (`6.5.30/unix.IP35`, address 0xc000000000329cac, 8 bytes). So the "refcnt" node exists on IP35 but cannot be opened, mapped or controlled. Established.
- `mem_refcnt_attach` (`6.5.30/unix.IP27`, address 0xc00000000027a0c4, 52 bytes) is the same routine as the IP35 one. Established.
- `mem_refcnt_open` (`6.5.30/unix.IP27`, address 0xc00000000027a0f8, 108 bytes) checks privilege with `cap_able` (call at 0xc00000000027a104) and resolves the node with `master_node_get` (call at 0xc00000000027a114). Established.
- `mem_refcnt_mmap` (`6.5.30/unix.IP27`, address 0xc00000000027a16c, 156 bytes) resolves the node with `master_node_get` (call at 0xc00000000027a184) and maps physical memory into the caller with `v_mapphys` (call at 0xc00000000027a1f8). The mapped range is inferred to be the node's counter memory. Established for the calls.
- `mem_refcnt_ioctl` (`6.5.30/unix.IP27`, address 0xc00000000027a210, 532 bytes) takes commands 1 and 2 and returns 22 for any other. It resolves the node with `master_node_get` (call at 0xc00000000027a224), sizes slots with `slot_getsize` (call at 0xc00000000027a2a8) and copies records out with `copyout` (calls at 0xc00000000027a2d0 and 0xc00000000027a410). The record contents are unknown. Established for the structure.
- `prioctl_piocgetsn0refcntrs` (`6.5.30/unix.IP27`, address 0xc00000000035afc4, 596 bytes) and `prioctl_piocgetsn0extrefcntrs` (`6.5.30/unix.IP27`, address 0xc00000000035b218, 596 bytes) are `/proc` ioctl handlers that return reference-counter data for a process; the names are the evidence, the bodies were not read. IP35 has no routine with these names. Inferred.

The only user programs in `root/6.5.30` that touch NUMA state are `numastatd` and the `hinv` family. `numastatd` (`6.5.30/usr/sbin/numastatd`) samples `/proc/pinfo` and router monitor nodes and calls `syssgi`; it is a statistics sampler, not a migration controller ([userland.md](userland.md)). The other user programs and the manual pages are in [User level](#user-level) and [What the manuals say](#what-the-manuals-say).

## Hardware reference counters

On IP27 the migration code reads and re-arms per-node counter registers in the hub window.

- `migr_intr_handler` (`6.5.30/unix.IP27`, address 0xc00000000026c6c0, 564 bytes) runs from the level 3 interrupt. For the interrupted processor's node it reads the doubleword at offset 0x48 of that node's hub window (the base word 0x9200000001a00000 from gp-24440 plus the node's window selector shifted left 32), calls `migr_refcnt_update_counters` (0xc00000000026cdec, 1016 bytes; calls at 0xc00000000026c730 and 0xc00000000026c80c with 0x10 and 0x18 as the last argument), calls `migr_manager` (0xc000000000280294, 1780 bytes; call at 0xc00000000026c848) and `migr_refcnt_restart` (0xc00000000026d1e4, 792 bytes; call at 0xc00000000026c860), and writes offsets 0x30 and 0x38 of the same window with bit 63 set before it returns. Established for the offsets and calls.
- `migr_refcnt_init` (`6.5.30/unix.IP27`, address 0xc00000000026c940, 724 bytes) allocates the software extended-counter arrays (`kmem_alloc_node`, `kmem_contig_alloc_node` and `slot_getsize` per node) and stores a value with bit 63 set at offset 0x38 of each node's window (store at 0xc00000000026cb70). Established.
- `hub_migrintr_init` (`6.5.30/unix.IP27`, address 0xc000000000039bac, 108 bytes) skips single-node systems, then calls `intr_connect_level` (0xc0000000000383ec, call at 0xc000000000039be8) for interrupt level 3 with `migr_intr_handler` (0xc00000000026c6c0) and `migr_intr_prologue_handler` (0xc00000000026c634) as handler and prologue. Established.

The names of those registers come from outside the binaries. On IP35 the offsets match a public vendor header ([Hardware that counts references](#hardware-that-counts-references)). On IP27 the routine that arms the threshold, `migr_effthreshold_diff_set`, stores at offset 0x30, so 0x30 is the count-difference threshold, 0x38 is inferred to be the second (absolute-value) threshold, and 0x48 (read in a loop until bit 63 clears) the candidate register that clears on read; an earlier reading of this paragraph took 0x30 for the candidate register, which the store contradicts ([Release history](#release-history-of-migration-and-replication-65-8-to-6530)). The vendor comment for the extended counters says the hardware counter overflow value is moved to software counters; the counter widths it quotes (11-bit for ordinary memory, 19-bit for premium) are the vendor's wording, taken from the 6.5.22 `mtune/numa` file, and are not checked against a register specification.

No routine linked into the shipped 6.5.30 IP35 kernel reads a reference counter; the routines above are the whole family on IP27 and none is linked on IP35. The whole IP35 kernel was searched for accesses to the migration registers and to the protection-entry window, and the objects of `IP35boot` for the window constants; the result is in [Hardware that counts references](#hardware-that-counts-references). Established.

## What drives migration

- `coalesced` (`6.5.30/unix.IP35`, address 0xc00000000027a8f0, 252 bytes) is the large-page coalescing thread. It loops over `coalesce_engine` (calls at 0xc00000000027a934, 0xc00000000027a968, 0xc00000000027a998 and 0xc00000000027a9dc), reads `coalesced_precoalesce` (load at 0xc00000000027a958) and sleeps with `coalesce_daemon_sleep` (call at 0xc00000000027a9b4). Its data words are `coalesced_pri` 90, `coalesced_run_period` 300 and `coalesced_precoalesce` 0 in the IP35 kernel. This is the only IP35 thread found that moves pages without a user call. Established for the loop; the thread role and what `coalesced_run_period` counts are inferred.
- `coalesced_kick` (`6.5.30/unix.IP35`, address 0xc00000000027b268, 188 bytes) calls `coalesce_daemon_wakeup` (call at 0xc00000000027b308) to wake the thread. Established.
- `second_thread` (`6.5.30/unix.IP35`, address 0xc00000000026084c, 528 bytes) calls `coalesced_kick` (call at 0xc000000000260a4c). Established.
- `mem_tick` (`6.5.30/unix.IP27`, address 0xc00000000027a8bc, 556 bytes) is the periodic memory tick; the role is inferred from the name and the `mtune` comments. Inferred.
- `clock` (`6.5.30/unix.IP27`, address 0xc0000000001b22d4, 2520 bytes) calls `mem_tick` (call at 0xc0000000001b28c8). Established.
- `mem_tick_init_node` (`6.5.30/unix.IP27`, address 0xc00000000027a424, 1052 bytes) sets a node up for the tick; it reads `numa_migr_default_mode` and `mem_tick_enabled`. Inferred.

IP27 also has the migration interrupt ([Hardware reference counters](#hardware-reference-counters)). `migr_start` (0xc00000000027bd24, 836 bytes) is called from 17 sites in page-fault, copy, swap-in and mapping paths (four in `pas_vfault`, two each in `pas_pfault` and `unswap`, and one each in `pas_pflip`, `pas_addstackexec`, `pas_addshm`, `lpage_swapin`, `copy_pmap`, `copy_locked_page`, `rmap_addmap_nolock`, `segcommon_spt_modify` and `prusrio`), and `migr_manager` is reached from the interrupt handler. The periodic filters (`migr_periodic_unpegging_control`, `migr_periodic_bounce_control`, `migr_periodic_queue_control`) and the queue (`migr_queue_insert`) exist only on IP27. Established for the call edges (a scan of every `jal` in the IP27 text); the filter roles come from their names and the `mtune` comments.

No memory daemon named `memoryd` exists on either machine: the tunable `memoryd_enabled` is read by `migr_control_init` on IP27 only and has no reader on IP35, neither kernel has a routine or thread with that name (the debug-information names included), and the name appears elsewhere only in two statistic names that `nstats` prints. Established.

## Looking for an equivalent mechanism on IP35

The question here is wider than "are the IP27 routines linked": does anything in the 6.5.30 IP35 kernel, or in the IP35 objects and user programs that go with it, move pages between nodes without a user request, under any name? "The migration objects are empty" and "built for the Origin 2000 only" are statements to test, not premises. The answer is in the verdict at the end of this section; the rest is the evidence and what each candidate turned out to be.

### Coverage

**Routine table.** The linked 6.5.30 IP35 kernel has 15122 routines in its text: 10013 with symbol-table entries and 5109 static routines found only in the debug information (names and address ranges from `.debug_info`; the frame table has 15071 entries covering 4553112 of the 4553992 text bytes). The earlier passes on this page and the tunable inventory resolved only routines with symbol-table entries. Every "no routine of this name" below uses the debug-information names as well, and every routine was decoded. The same table was built for `6.5.30/unix.IP27` (13415 routines), `6.5.22/unix.IP35` (14501) and `6.5.22/unix.IP27` (13592). Established.

**Call edges.** Every `j` and `jal` word in the text; every doubleword in `.data`, `.rodata`, `.sdata` and `.srdata` that equals a routine address; every routine address that code loads from a global-pointer slot. For each page-moving routine below, no routine address other than the ones named is stored anywhere, so the caller lists are complete for those routines. Calls through other pointers are not resolved. Established.

**Data references.** Every instruction in the whole text that uses the global pointer as its base (loads, stores and address formation), every global-pointer pool word that holds a data address, and every absolute `lui` pair with the high half 0x163 or 0x164 (the tunable block is 0xc00000000163bf50 to 0xc00000000163bffc). The 12 such pairs form addresses of the `nasid_to_compact_node` table, `pdaindr`, `cell_enabled`, `oplock_timeout` (0xc00000000163bf40), `oplocks_enabled`, `xfs_fstype`, `vn_passthrup` and `idbgdefault`; none lands in the tunable block. A linked kernel has no relocations, so these are the forms the compiler emitted. Established for the tunables ([Tunables](#tunables)).

**Objects.** All 181 object files and the 949 members of the 169 non-empty archives in `6.5.30/usr/cpu/sysgen/IP35boot/` were scanned for 64-bit constants in the hub register windows (top byte 0x90 to 0x93) and for symbols named like migration, reference-count, tick, replica or coalescing routines. Established.

### Every caller of a page mover

The page-moving routines are `migr_pagemove` (`6.5.30/unix.IP35`, address 0xc00000000032ba90, 604 bytes), `migr_pfdat_do_xfer` (0xc00000000032b9b0, 224 bytes) and the routines above them in [What still moves pages on IP35](#what-still-moves-pages-on-ip35). Their callers, followed upward until a routine with no direct caller, end in four places and nowhere else.

| Root | Path down to `migr_pagemove` | What starts it | Grade |
|---|---|---|---|
| `syssgi` 208, `pmoctl` | commands 40, 41, 42: `migr_xface_migrate`, `pmo_xface_as_migrate`, `pmo_xface_as_migrate_close`, then `migr_user_range_migrate` or `as_migrate` and `migr_vas_pages` | a user request. The 6.5.30 C library issues command 40 from `migr_range_migrate` (`syssgi` 208, command 0x28) | Established |
| `sysmp` | `cpuset_process` (0xc00000000024c450, 596 bytes) calls `cpuset_move_op` (0xc000000000249ffc, 436 bytes), which calls `cpuset_move_pid` and `cpuset_migr_cleanup`; these reach `as_migrate` directly, through `cpuset_move_to_global`, or through `as_migrate_close` | cpuset administration by a user | Established |
| `madvise` | the address-space operations vector (`pas_ops`, slot at +144) calls `pas_setrangeattr` (0xc00000000030e59c, 1560 bytes), then `pas_madvise` (0xc0000000002bf8d8, 448 bytes), then `pm_sync_policy` (0xc0000000003253a4, 696 bytes), which calls the placement policy's service routine with code 3 (arm 3 of the jump table in `plac_policy_default_plac_srvc`, 0xc000000000327e6c, and of the five other placement policies); that arm calls `plac_policy_sync` (0xc000000000329cbc, 1896 bytes), which calls `migr_migrate_frame` | a user changing the placement attributes of a range | Established for the edges from `pas_setrangeattr` down; inferred that `madvise` reaches `pas_setrangeattr` (the call is through the vector) |
| `coalesced` thread | `coalesce_engine`, `coalesce_node`, `coalesce_chunk`, `coalesce_leaf_chunk`, `migr_coald_test_pagemoves` and `migr_coald_move_pages`, then `migr_migrate_page_list` | the kernel itself: see below | Established |

Three routines look like movers and are never called or referenced: `migr_fastcheck_migratable` (0xc00000000032b784, 148 bytes), `pfile_target_migrate` (0xc000000000314de4, 132 bytes) and `migr_pageref_disable` (0xc00000000032b5f4, 132 bytes). Established.

**The one autonomous mover is the large-page coalescing thread.** `main` starts `coalesced` (the address is taken in `main`, 0xc0000000002dd1bc, 1800 bytes, with priority `coalesced_pri`). It loops over `coalesce_engine`, which walks the nodes in rotation (`coalesce_node` per node) and tries to assemble free large pages by moving small pages out of the way with `migr_coald_move_pages`; between passes it sleeps in `coalesce_daemon_sleep` (0xc00000000027b0dc, 220 bytes: a timed wait, `timeout` of the period plus a `sv_wait`). It is woken early by `coalesce_daemon_wakeup` from `_lpage_watermarks_sanity`, `lpage_alloc_aligned_page`, `pagealloc_size_node` and `vfault_wait_lpage`, and once a second by `second_thread` through `coalesced_kick`. The data words it reads are `coalesced_run_period` (300), `coalesced_precoalesce` (0, read by `coalesced`, `coalesce_engine` and `coalesce_node`) and `coalesced_pri` (90). The trigger is the large-page free-memory watermark, not a page access count; no hardware counter is read. It is identical in the IP27 kernel. Established for the loop, the wake-up callers and the data words; inferred for the purpose (compaction for large pages), from the names and the callees.

### Threads, callouts and timeouts

The threads the kernel creates at start-up and from device and subsystem initialisers (every caller of `sthread_create`, `xthread_create`, `gthread_create` and the other creators, with the routine addresses each one loads): `main` starts `do_unmount`, `async_call_daemon`, `sched`, `coalesced`, `failsoftd` and one diagnostic thread; `pager_init` starts `vhand` and `shaked`; `init_second` starts `second_thread`; `kthread_reap_init`, `bdflushinit`, `xfs_start_daemons`, `miser_set_quantum` (`release_miser`, `batchd`), `cpuset_init_cpuset_vhand` (`cpuset_vhand`) and the driver initialisers start the rest. None is named for memory migration, and none reaches a page mover except `coalesced`. Established.

No address of a `migr_*`, `mem_tick*`, `pm_sync_policy` or `as_migrate*` routine is passed to `timeout`, `clock_prtimeout` or any other callout registration: the only address-taking sites are the policy-module tables, `coalesced` in `main`, and `coalesce_daemon_wakeup` in `coalesce_daemon_sleep`. `migrate_timeouts` and `kthread_afflink_migrate` move callouts and thread affinity links, not pages. Established.

There is no `memoryd` routine, thread or process. The name occurs as the tunable `memoryd_enabled` (no reader in the IP35 kernel; one reader, `migr_control_init`, in IP27) and as two statistic names that the user program `nstats` prints. Established.

### Names that are not the mechanism

Routines that match the names asked about and are not page migration: `memsched_init` (0xc000000000323970, 484 bytes) and the `physmem_*` routines choose the node for a new allocation; `sn1dmetrics` builds node distances from the routing tables; `memfit` and `memaff_*` track node preference for the scheduler; `kthread_afflink_*` relink thread affinity; `cpuset_vhand` is a cpuset reclaim thread; `replicate_kernel_text` (0xc000000000074d14, 376 bytes, called from `allowboot`) copies the kernel text to further nodes once at boot, in the ratio of `numa_kernel_replication_ratio` (inferred from the name and the single tunable it reads); `pm_repl_srvc` (0xc0000000003260cc, 108 bytes) is the stand-in service routine for the replication policy slot of a placement module. Page replication proper (`repl_*`, `page_replica_*`) is not in the 6.5.30 kernels; it is in 6.5.22 ([6.5.22 for comparison](#6522-for-comparison)). Established for the absence and the callers; inferred for the roles.

### Hardware that counts references

The register block that IP27 uses for migration counting is at offsets 0x30, 0x38 and 0x48 of the window `0x9200000001a00000`. The IP35 kernel's one migration store is at offset 0x40 of `0x9200000001fc0000` plus the node's NASID shifted left 33. A public Linux header written by the hardware vendor for the same chip family (the `hubmd.h` of the SN1 port in the old Linux history tree; a hint, not evidence) defines the registers of the memory/directory block with offsets relative to the local register base 0x1800000. Subtracting that base from the IP35 offsets matches names for the offsets this kernel touches, and `set_md_outgoing_rq_queue_size` (named for the register the header defines at the matching offset) is the check that the subtraction is right. The table lists every access the IP35 kernel makes to the part of the block that holds the migration registers.

| IP35 offset (window `0x9200000001fc0000`, NASID << 33) | Name in the hint | IP35 kernel accesses (all 15122 routines) | Grade |
|---|---|---|---|
| +0x30 | MD_MIG_CANDIDATE | none | Established (no access) |
| +0x38 | MD_MIG_CANDIDATE_CLR (read clears) | none | Established (no access) |
| +0x40 | MD_MIG_DIFF_THRESH | one store: `migr_effthreshold_diff_set`, called only by `migr_init` | Established (access); inferred (name) |
| +0x48 | MD_MIG_VALUE_THRESH | none | Established (no access) |
| +0x50 | MD_OUTGOING_RQ_QUEUE_SIZE (not migration) | `set_md_outgoing_rq_queue_size` loads and stores | Established |
| `0x9200000001f80018` | MD_MIG_CONFIG (page-migration mode control) | none | Established (no access) |

The other accesses to this block are the directory and error routines (`get_dir_ent`, `set_dir_state`, `show_dir_state`, `check_dir_access`, `write_hub_dir_mem`, `log_md_errors`, `memerror_sbe_intr`, `hub_error_clear`, `adjust_nmi_handler`, `handle_cac_buserr`, the `memory_*_accessible` and `memory_set_access` protection routines, `move_buffer`). None reads a candidate or a counter. The only other routine that names a counter store is `migr_pageref_disable`, which has no caller. Established.

`migr_effthreshold_diff_set` (`6.5.30/unix.IP35`, address 0xc00000000032b5b4, 64 bytes) stores bit 63 together with 0x7fff minus the low 32 bits of its count argument. By the hint, bit 63 is the register's valid bit and the low 15 bits are the threshold: when the difference between the requester's count and the home count reaches the threshold and the valid bit is set, the hub loads the migration candidate register and, if the page-migration mode enables it, raises an interrupt. The 15-bit width matches the constant 0x7fff. The IP27 routine uses 0xfffff (20 bits) and shifts the NASID left 32; the one-bit difference in the NASID shift is the difference between the two address layouts (the IP35 shift is 33), the same shift that every NASID-indexed access in the IP35 kernel uses. Inferred.

`migr_init` (`6.5.30/unix.IP35`, address 0xc00000000032b504, 176 bytes) is the only caller, from `numa_init_table`. For every node it passes the count `record[2] * record[0xc] / 100`. The per-node record is allocated by `low_mem_alloc` (0xc0000000002a2720, 196 bytes, which calls `bzero`) in `migr_control_init`, and `migr_control_init` never writes bytes 2 or 12. So at boot the count is 0 and the register is armed with a threshold of 0x7fff, the largest value the 15-bit field holds. Inferred (the order of the two initialisers and the allocator's zeroing are read, not run). No other routine calls `migr_effthreshold_diff_set`, and `numa_tune` (the path of `sn -t`) does not re-arm the register. Established.

`migr_pageref_disable` (`6.5.30/unix.IP35`, address 0xc00000000032b5f4, 132 bytes) stores the doubleword 0xffff for one page, on every node, at an address formed from the protection-entry base `0x90000001c0000408` that `memory_set_access` and `move_buffer` also use. The IP27 routine of the same name (0xc00000000027d7a8, 144 bytes) is the same idea at the IP27 base `0x90000000c0000000`. Neither kernel calls it. What the stored value does to the entry is unknown. In the hint, SN1 keeps the reference count and the protection field "at different addresses" with a 16 KB memory-directory page (4 KB on the Origin 2000), and the macros that read a migration candidate were left marked as needing a rewrite. Established for the store and the missing callers; unknown for the effect.

Of the 181 objects and the archive members in `IP35boot`, only six carry the protection-window constants (`kernel.o`, `klidbg.o`, `vmdump.o`, `partition.o`, `partition_part.o` and `numa_hw.o`), and only `numa_hw.o` has a migration routine in it (its two routines are the ones above). The debugger archive `numa_idbg.a` for IP35 still carries `idbg_refcnt` and two reference-state printers and prints the whole set of per-node migration parameters; the shipped kernel links the stub archive in its place, and the printers use no hub window. Established.

### The migration interrupt

`hardwired_intr` still reserves level 3 under the name "Migration" ([interrupts.md](interrupts.md)). A search of every call to `intr_connect_level` (0xc00000000005f308, 792 bytes) in the IP35 kernel finds 11 sites in 10 routines: `hubii_eint_init`, `hubni_eint_init` (two), `install_cpuintr`, `install_dbgintr`, `install_tlbintr`, `install_gfxintr`, `hub_intr_connect`, `install_eccintr`, `intrgroup_join` and `brl1_connect_intr`. The levels they pass are 1 and 2 (graphics), 7 and 8 (cpu action), 0x67, 0x68 and 0x7e (network interface), 0x7d (ECC), levels computed by the caller (`hubii_eint_init`, `hub_intr_connect`) and levels returned by `intr_reserve_level` (`install_dbgintr`, `install_tlbintr`, `intrgroup_join`). None passes 3. The IP27 kernel connects level 3 only in `hub_migrintr_init`, which IP35 does not link. `intr_reserve_hardwired` (0xc000000000060f04, 348 bytes) is called only from `per_hub_init`. So nothing in the IP35 kernel connects a handler to level 3. Established for the constants; inferred that the run-time levels never come out as 3, because they come from an allocator that treats reserved levels as taken (the allocator was not read).

### Policy modules and their services

Three migration policy modules register in `numa_init_table` (`migr_policy_default_init`, `migr_policy_control_init`, `migr_policy_refcnt_init`), each calling `pmfactory_export` (0xc000000000326fc0, 32 bytes) with its create routine and a name ("MigrationDefault", "MigrationControl", "MigrationRefcnt"); `pmfactory_export` hands them to `pmfactory_ns_insert` (0xc000000000326e84, 112 bytes). `pm_create` (0xc000000000324310, 636 bytes) looks policies up by name with `pmfactory_ns_find` or takes `pmfactory_ns_getdefault`, and keeps four policy slots in a placement module: placement at +0x30, fallback at +0x48, replication at +0x58 and migration at +0x68, each a pointer to a service routine. Established. How `pm_create` picks a default name was not traced.

The service routine of each migration policy takes the module, a code and an output pointer. The codes come from the callers in the placement-module code, which pass the same codes to all four slots:

| Code | What the callers use it for | Migration policy's answer | Grade |
|---|---|---|---|
| 0 | destroy: `pm_destroy` (0xc00000000032472c, 144 bytes) and the error path of `pm_create` | calls the policy's destroy routine (an 8-byte routine that returns) | Established |
| 1 | name: `pm_getstat` (0xc000000000325134, 468 bytes) asks every slot for its name | stores the pointer to "MigrationDefault", "MigrationControl" or "MigrationRefcnt" | Established |
| 2 | not passed by any placement-module routine found | returns 0 | Established (callers); role unknown |
| 4 | parameter record for the checkpoint information, `pmo_ckpt_pm_info` (0xc000000000326320, 988 bytes), for the migration slot only | stores the 8-byte record at offset 0x70 of the module | Established |
| 6 | the same routine passes 6 to the other three slots | stores a zero word | Established (shape); role unknown |
| 3, 7, 8 | sync, attach and use-count decrement, passed to the placement slot only (`pm_sync_policy`, `pm_attach`, `pm_decusg`) | not valid for a migration policy: any code outside 0, 1, 2, 4 and 6 reaches `cmn_err` with "Invalid service type" | Established |

`migr_policy_default_create` (0xc000000000330874, 164 bytes) fills the module: it stores its service routine at +0x68 and copies the first eight bytes of the node's migration record, nibble by nibble, to +0x70. The migration policies therefore hold a copy of the node's mode bits, and they offer a name and that copy to the statistics and checkpoint paths. No service code moves a page, starts a scan or reads a counter. Established. The only routines that read the first eight bytes of the node record are `migr_control_init`, `migr_policy_default_create`, `migr_init` (bytes 2 and 12) and `numa_tune` (the user copy); the byte at +0x37 is read by `migr_migrate_frame`, as the on/off gate of the requested moves, and is set to 1 at start-up. Established.

### User level

Everything in this section comes from the 6.5.30 `eoe` product on the installation media (the files are not in `root/`).

`dplace` (20856 bytes) takes `-migration [on|off]` and `-migration_level n` and passes them by the environment variables `__DPLACE_MIGRATION_` and `__DPLACE_MIGRATION_LEVEL_` to `libdplace`, which creates a placement module with a policy named "MigrationControl" (`pm_create`, `pm_filldefault`, `pm_setdefault`, and `migr_policy_args_init`) and, for `migrate_range`, calls `migr_range_migrate`. The messages are "Migration is on and level is %d%%" and "Migration is off". It starts nothing that runs on its own. Established.

`migr_policy_args_init` in the 6.5.30 C library reads the first block of per-node tunables with `syssgi` 212 (command 100, which is `numa_tune`) and converts the eight mode bytes into the argument structure; `migr_range_migrate` is `syssgi` 208 command 40. Established.

`sn` (19944 bytes) sets and shows the per-node parameters (migration mode, threshold, unpegging, queue, dampening, vehicle) with `syssgi` 212 (messages "MIGR_PARMS_GET" and "MIGR_PARMS_SET"). On IP35 `numa_tune` accepts the block and stores the first 0x38 bytes of it in the node record. The IP35 kernel reads those bytes in four places only: the gate at +0x37 (`migr_migrate_frame`), the eight bytes copied into a new migration policy (`migr_policy_default_create`), bytes 2 and 12 at boot (`migr_init`), and `numa_tune` itself. Established.

`nstats` (20860 bytes) reads `syssgi` 218 (`numa_stats_get`) and prints node counters named "MigrIntrs", "AutoMigr", "UserMigr", "CoalMigr", the queue and bounce counters, the reference-counter interrupt counters ("refcnt_introk_relative" and others) and the two `memoryd` activation counts. These are the IP27 statistic names; whether any IP35 routine increments them was not checked.

`cpuset` and `libcpuset` (`cpusetMoveMigrate` and variants): user requests that end in `as_migrate` through `sysmp`. `libfetchop` uses `pm_attach` to reserve its fetch-and-op pages. `libdlook` reads page information through `__pm_get_page_info`. `numastatd` samples and does not move pages ([userland.md](userland.md)). `topology`, `numa_view` and `runon` only read topology or bind a process.

No daemon on the installation media drives migration: the files of the `eoe` product that mention migration are `dplace`, `dlook`, `dprof`, `nstats`, `sn`, `cpuset`, `libdplace`, `libcpuset`, `libdlook`, `libdprof`, the C library and the manual pages. No `memoryd` binary or `init.d` script exists in the 6.5.30, 6.5.22 or 6.5 base idb manifests. Established for the media held.

### 6.5.22 for comparison

The IP35 kernel of 6.5.22 has the same missing set as 6.5.30: no `hub_migrintr_init`, `migr_intr_handler`, `migr_manager`, `migr_start`, `migr_refcnt_*`, `mem_tick`, `migr_queue_*` or periodic filters; `mem_tick_init` (0xc0000000002f94c8) is 8 bytes; `migr_effthreshold_diff_set` (0xc0000000002f2714, 64 bytes) and the dead `migr_pageref_disable` (0xc0000000002f2754, 132 bytes) are present; and the same eight `numa.a` members (`migr_bounce`, `migr_interrupt`, `migr_manager`, `migr_queue`, `migr_refcnt`, `migr_states`, `migr_unpegging`, `pfms`) are empty. The 6.5.22 archive has two more members than 6.5.30, `numa_traffic.o` and `mem_profiler.o`. The tunable access sites are identical to 6.5.30 in all 47 rows. Established.

6.5.22 has page replication, which 6.5.30 removed from both platforms. `repl_interpose` (`6.5.22/unix.IP35`, address 0xc0000000002e2af0, 308 bytes) is called from `execmap` and `gexec` and attaches a replication behaviour to an executable's file; `rp_default_getpage` (0xc0000000002f5e6c, 664 bytes) then asks `repl_control` (0xc0000000002f5590, 212 bytes) which node should hold a replica; `repl_control` calls `repl_resource_check` (0xc0000000002f5664, 220 bytes), which compares free memory against `numa_repl_mem_lowmark` and the traffic on the routes to and from the node against `numa_repl_traffic_highmark_percentage`, and `page_replica_insert` (0xc0000000002f458c, 496 bytes) enters the copy. It reads router traffic, not page reference counters. It is off: `repl_interpose` returns at once when `numa_page_replication_enable` (0xc000000001593d10) is 0, and `numa_repl_control_enabled` is 0; neither variable is a tuning-file entry, no routine writes them, and the `mtune` heading says "No effect on any systems, reserved for future". Established.

The 6.5.22 and 6.5.30 `mtune/numa` files are byte for byte the same.

### What the manuals say

The vendor documents are hints; the binary findings above stand on their own. They agree with them.

*Origin 2000 and Onyx2 Performance Tuning and Optimization Guide* (007-3430-003), chapter 2 ("Dynamic Page Migration") and chapter 8 ("Trying Dynamic Page Migration", "Enabling Page Migration"): the guide is written for the Origin 2000/Onyx2 (SN0) only. It says the kernel "maintains counters that show which nodes are making the heaviest use of each virtual page", that the counters are hardware-maintained, one per 4 KB block of memory (refcnt(5)), that the kernel "surveys the counters periodically", that migration fires when the difference between the local count and the count from one other node exceeds a percentage of the counter maximum, and that the facility is off by default and is turned on per program (`_DSM_MIGRATION`, `dplace -migration`) or per system (`sn`, or the tunable `numa_migr_base_enabled`, an older name than the 6.5.30 tuning file has). It does not mention the Origin 300 or 3000.

`numa(5)` (6.5.30 manual page, `eoe.man.base`): "Automatic Memory Locality Management ... is only available on Origin 2000/200 platforms. Any other Origin product line does not have automatic memory locality management." Memory-reference counters (refcnt(5)) are "provided by the Origin 2000/200 hardware".

`migration(5)` (6.5.30): "This document describes the dynamic memory migration system available in Origin 2000/200 systems. Dynamic migration is not available on other Origin platforms." Manual migration is "available to all Origin platforms" and is also used "to assist the memory coalescing algorithms", the same two uses this kernel keeps. The detection module is "mostly implemented in hardware": per-page counters compare the remote count against the local count, and an interrupt tells the kernel. The threshold "is stored in a per-node hardware register, which is initialized by the Migration Control Module", which is what `migr_effthreshold_diff_set` still does on IP35. The migration engine and the control module (filters for distance, memory pressure, bouncing and queues) run from the interrupt handler; those are the routines that are not linked on IP35.

`refcnt(5)` (6.5.30): "this is an Origin 2000/200 capability only, and does not apply to other Origin platforms"; the counter sets are per 4 KB hardware page, with 11-bit counters for standard memory (the width in the tuning-file comment).

`mmci(5)` (6.5.30): the three migration policy modules are "only applicable to Origin 2000 systems", "any use of this policy on other platforms has no effect"; "for Origin 2000 systems only, IRIX implements dynamic memory migration"; the replication policies "have not been implemented". `migration(3)`: `migr_policy_args_init` "has no effect on other Origin systems"; `migr_range_migrate` migrates a range to the node of a placed memory locality domain and skips pages under DMA, fetch-and-op, pinned or shared. That description is of the requested migration, which IP35 keeps.

So what page migration depends on, as the manuals tell it, is a hardware-maintained per-block access counter (the Origin 2000/200 memory and directory hardware), a per-node threshold register that the kernel programs, an interrupt to the home node, and a kernel migration control module and engine that run from that interrupt and from a periodic tick. The 6.5.30 IP35 kernel has only the register store from that list.

### Verdict

| Statement | Grade | What would change it |
|---|---|---|
| The 6.5.30 IP35 kernel has no code that reads a hardware page reference counter, takes a migration interrupt, runs a periodic memory tick, queues migration requests or runs a migration manager. | Established (all 15122 routines; the objects of `IP35boot`; the 6.5.22 IP35 kernel agrees) | A kernel built with other objects: none is held. |
| No routine in the 6.5.30 IP35 kernel moves a page except on a user request (`pmoctl`, cpuset, range attribute change) or as large-page compaction (`coalesced`). | Established for the complete caller closure of the page movers | An indirect call through a pointer that this scan could not resolve. |
| Automatic locality migration exists on IP35 under another name. | Not found. Inferred absent. | A page mover with a caller outside the table above. |
| The shipped IP35 kernel arms the migration-threshold register of every node at boot and then never reads the result. | Established (the store and the missing readers); the armed value is inferred | The order of the initialisers, run. |
| Absence is a property of the platform and not an accident of the build. | Absence in 6.5.30 is deliberate: the earlier IP35 objects (6.5.10 to 6.5.16) contain the whole engine and a connected level-3 handler, and later releases empty it in steps while rewriting the documentation to "Origin 2000/200 only" ([Release history](#release-history-of-migration-and-replication-65-8-to-6530)). The manual pages state the limit as a hardware one (the counters are an Origin 2000/200 capability); the binaries do not say why the IP35 code was withdrawn. | Established (that it was built and withdrawn); the cause is unknown. A Bedrock register document that says what the candidate interrupt does would bound it. |
| The hardware itself can raise a migration interrupt on the Origin 300. | Unknown. The power-on state of `MD_MIG_CONFIG` is not set by the kernel; the PROM was not read for it. | The IP35 PROM (a held disassembly exists) or a Bedrock register document. |

## Tunables

Defaults below are the 4-byte data words of the linked kernels at their tunable symbols (equal in `unix.IP35` and `unix.IP27`; 6.5.22 `mtune` agrees). An access site is a routine that loads the word, stores it (`numa_migr_vehicle` in `migr_control_init` is the one store) or forms its address. The sites come from a scan of every instruction in the text that uses the global pointer as base, every pool word holding a tunable's address and every absolute address formation, over all routines including those with no symbol-table entry; the 6.5.22 IP35 kernel has the same sites in all 47 rows. The scan finds two readers that the symbolic pass of [sysgen.md](sysgen.md) missed (`coalesce_engine` and `coalesce_node` for `coalesced_precoalesce`, `kmem_newzone_init` for `numa_zone_radius`) and agrees with it elsewhere. Besides these sites the words are reachable by name through the table `tunename` (0xc00000000154d5f8 onward, one 0x50-byte entry per tunable: name, address, size), whose only reader is `phost_systune`, the tuning interface; that is how a user changes a value, not a consumer of it.

| Tunable | Default in the IP35 kernel | IP35 access sites | IP27 access sites |
|---|---:|---|---|
| `coalesced_run_period` | 300 | _lpage_watermarks_sanity, lpage_init | _lpage_watermarks_sanity, lpage_init |
| `coalesced_precoalesce` | 0 | coalesce_engine, coalesce_node, coalesced | coalesce_engine, coalesce_node, coalesced |
| `coalesced_pri` | 90 | main | main |
| `numa_migr_default_mode` | 2 | migr_control_init | mem_tick_init_node, migr_control_init |
| `numa_migr_default_threshold` | 50 | none | migr_control_init |
| `numa_migr_threshold_reference` | 0 | none | migr_control_init |
| `numa_migr_vehicle` | 1 | migr_control_init (store), migr_pagemove | migr_control_init (store), migr_pagemove |
| `numa_migr_min_maxradius` | 2 | migr_control_init | migr_control_init |
| `numa_migr_auto_migr_mech` | 0 | none | migr_control_init |
| `numa_migr_user_migr_mech` | 0 | none | migr_control_init |
| `numa_migr_coaldmigr_mech` | 0 | none | migr_control_init |
| `numa_refcnt_default_mode` | 0 | none | migr_control_init |
| `numa_refcnt_overflow_threshold` | 50 | none | migr_control_init |
| `numa_migr_min_distance` | 1 | none | migr_control_init |
| `numa_migr_memory_low_enabled` | 1 | none | migr_control_init |
| `numa_migr_memory_low_threshold` | 20 | none | migr_control_init |
| `numa_migr_freeze_enabled` | 0 | none | migr_control_init |
| `numa_migr_freeze_threshold` | 20 | none | migr_control_init |
| `numa_migr_melt_enabled` | 0 | none | migr_control_init |
| `numa_migr_melt_threshold` | 90 | none | migr_control_init |
| `numa_migr_bounce_control_interval` | 0 | none | migr_control_init |
| `numa_migr_dampening_enabled` | 0 | none | migr_control_init |
| `numa_migr_dampening_factor` | 90 | none | migr_control_init |
| `mem_tick_enabled` | 1 | none | mem_tick_init_node, mem_tick_reactivate_node |
| `mem_tick_base_period` | 1 | none | alloc_cpupda |
| `numa_migr_unpegging_control_enabled` | 1 | none | migr_control_init |
| `numa_migr_unpegging_control_interval` | 0 | none | migr_control_init |
| `numa_migr_unpegging_control_threshold` | 90 | none | migr_control_init |
| `numa_migr_traffic_control_enabled` | 0 | none | migr_control_init |
| `numa_migr_traffic_control_interval` | 400 | none | migr_control_init |
| `numa_migr_traffic_control_threshold` | 80 | none | migr_control_init |
| `numa_kernel_replication_ratio` | 1 | setup_replication_mask | setup_replication_mask |
| `numa_paging_node_freemem_low_threshold` | 40 | _pagealloc_size_node | _pagealloc_size_node |
| `numa_zone_radius` | 1 | kmem_newzone_init | kmem_newzone_init |
| `numa_migr_enqonfail_enabled` | 0 | none | migr_control_init |
| `memoryd_enabled` | 0 | none | migr_control_init |
| `numa_migr_queue_maxlen` | 70 | none | migr_control_init |
| `numa_migr_queue_control_enabled` | 0 | none | migr_control_init |
| `numa_migr_queue_control_interval` | 500 | none | migr_control_init |
| `numa_migr_queue_control_mlimit` | 70 | none | migr_control_init |
| `numa_migr_queue_traffic_trigger_enabled` | 0 | none | migr_control_init |
| `numa_migr_queue_traffic_trigger_threshold` | 80 | none | migr_control_init |
| `numa_migr_queue_capacity_trigger_enabled` | 0 | none | migr_control_init |
| `numa_migr_queue_capacity_trigger_threshold` | 60 | none | migr_control_init |
| `numa_migr_queue_time_trigger_enabled` | 0 | none | migr_control_init |
| `numa_migr_queue_time_trigger_interval` | 1 | none | migr_control_init |
| `numa_migr_queue_control_max_page_age` | 8 | none | migr_control_init |

Meaning of the values, from the comments of the 6.5.22 `mtune/numa` file (the vendor's wording, graded as a hint for the binaries above): `numa_migr_default_mode` 0 disables migration and overrides users, 1 enables it and overrides users, 2 and 3 leave the default off or on and let users override, and 4 chooses between those by the machine's maximum router radius against `numa_migr_min_maxradius`. The IP35 routine's bit pattern for the modes (0x10 for 1, 0x20 for 3, 0x30 for 2) is consistent with that list. `numa_migr_vehicle` carries the note "changing this has no effect". The threshold comment gives the rule: a request fires when the difference between a remote and the local counter reaches the default threshold as a percentage of the reference set by `numa_migr_threshold_reference`. Of the 41 migration-related tunables, only `numa_migr_default_mode` and `numa_migr_min_maxradius` are used to build the IP35 control record, and `numa_migr_vehicle` is read by the page mover. The other 38 of the 47 names have no reader at all on IP35: they can be set and show in the tuning file, and nothing consults them. Of the six that are not migration controls, `coalesced_*` drive the coalescing thread, `numa_kernel_replication_ratio` the boot-time kernel text copy, `numa_paging_node_freemem_low_threshold` the large-page allocator and `numa_zone_radius` the kernel zone allocator.

The tuning command `systune` is not in `root/6.5.30`, so how these values are changed at run time, and what it does with the bounds, is unknown here.

## IP27 against IP35

| Piece | IP27 (6.5.30) | IP35 (6.5.30) |
|---|---|---|
| Migration interrupt connect | `hub_migrintr_init`, called from `per_hub_init` | not linked |
| Migration interrupt handler and manager | `migr_intr_handler`, `migr_manager`, `migr_start` and queue and filter routines | not linked |
| Hardware counter readers | `migr_refcnt_*` family (nine routines, one of them, `migr_refcnt_supersetbase_get`, known only from debug information) | not linked |
| Periodic tick | `mem_tick` called from `clock`; `mem_tick_init_node` | `mem_tick_init` is an 8-byte routine that returns success |
| `migr_control_init` | 3176 bytes, accesses 39 of the 41 migration-related tunables | 600 bytes, reads two and stores one (`numa_migr_vehicle`) |
| `mem_refcnt_*` device handlers | real | 8-byte routines returning 19 or 0 (the attach routine is 52 bytes and returns 0) |
| `/proc` counter ioctls | `prioctl_piocgetsn0refcntrs` and extended variant | not linked |
| Explicit migration (`pmoctl` 40 to 42), cpuset-driven `as_migrate`, coalescing thread, page mover | present | present |
| Threshold register at boot | `migr_effthreshold_diff_set` (20-bit field, window `0x9200000001a00000`, offset 0x30) | `migr_effthreshold_diff_set` (15-bit field, window `0x9200000001fc0000`, offset 0x40, NASID shifted 33) |
| Per-page counter access | `migr_refcnt_read`, `migr_refcnt_update_counters`, `migr_refcnt_restart`, `numa_tests` use the window `0x90000000c0000000` | only the uncalled `migr_pageref_disable` (window `0x90000001c0000408`) |
| Policy service routines | destroy, name, record, and the same invalid-code arm | the same |
| `numa_tune` | quiesces and restarts the tick on write | no tick calls |

The 6.5.22 kernels for the two machines have the same function-name pattern: IP27 has `migr_manager` and `migr_start`, IP35 has neither, and both have `migr_xface_migrate` and `coalesced` ([inventory/functions.tsv](inventory/functions.tsv); names only, bodies not compared). The IP30 kernel has 18 `migr_` routines and no `migr_manager` or `migr_start`; it is outside this page.

## Cross-partition communication on IP35

Three layers exist in the shipped IRIX objects. A partition layer (`part_*`) tracks partitions, exports memory pages to other partitions, handles heartbeats and cross-partition interrupts, and answers a `syssgi` selector. A message layer (XPC, "cross-partition communication") builds channels over it. A shared-memory driver (XPMEM) uses both. The 6.5.22 system file for IP35 notes that these modules come from a separate partition package on Origin 3000 systems and that partitioning is not supported on Origin 2000; the IP27 system file excludes the partition and XPC modules. That is a configuration statement for 6.5.22 only.

### What is linked in unix.IP35

- `part_id` (`6.5.30/unix.IP35`, address 0xc000000000496400, 8 bytes) is `jr ra` with `move v0,zero`: the local partition number is always 0. `part_installed` (`6.5.30/unix.IP35`, address 0xc0000000004963f8, 8 bytes) is the same two instructions; so are `part_from_addr`, `part_from_region` and `nasid_to_partid` ([ip35-halt-and-reset.md](ip35-halt-and-reset.md) records four of these). Established.
- `part_get_promid` (`6.5.30/unix.IP35`, address 0xc0000000004964bc, 8 bytes) is `jr ra` with -1 in the delay slot. Established.
- `xpc_allocate` (`6.5.30/unix.IP35`, address 0xc000000000496288, 32 bytes) calls `panic` with its own name as the message (call at 0xc000000000496290) and would return 3 if `panic` returned. Established.
- `xpc_connect` (`6.5.30/unix.IP35`, address 0xc0000000004962f0, 8 bytes) returns 3 with no call. `xpc_avail` (`6.5.30/unix.IP35`, address 0xc0000000004963d0, 8 bytes) is the same two instructions. Established.
- `xpc_send` (`6.5.30/unix.IP35`, address 0xc0000000004962a8, 36 bytes) calls `panic` with its own name (call at 0xc0000000004962b4) and returns 3 after it. Established.
- `part_register_handlers` (`6.5.30/unix.IP35`, address 0xc000000000496584, 32 bytes) calls `panic` with its own name (call at 0xc000000000496590). Established.
- `part_operation` (`6.5.30/unix.IP35`, address 0xc0000000004965c4, 8 bytes) returns 89 without a call, which is why `syssgi` selector 195 (PART_OPERATIONS) returns 89 ([syssgi.md](syssgi.md)). Established.

A scan of the 49 `xpc_*` and `part_*` routines in this stub block (addresses 0xc000000000496280 to 0xc0000000004966c0) found a call to `panic` in nine `xpc_*` routines (`xpc_allocate`, `xpc_send`, `xpc_send_notify`, `xpc_disconnect`, `xpc_receive_uio`, `xpc_receive`, `xpc_receive_xpmb`, `xpc_done`, `xpc_poll`) and nineteen `part_*` routines (the page allocate, free, permit, poison, register, unregister, export and unexport entries, the interrupt, variable, deactivate, handler and thread entries, and `part_heartbeat_timeout`). The other 21 are two-instruction returns; `xpc_init`, `xpc_check` and the three `xpc_dump_*` routines return at once with no value. Established.

- `xpmem_init` (`6.5.30/unix.IP35`, address 0xc0000000000e779c, 1068 bytes) is linked in full (71 `xpmem_*` routines) and its address is in the table of driver start-up routines at 0xc00000000153ff48, between `mminit` and `imoninit`, so it is inferred to run at boot. It stores `part_id()` in the byte `xpmem_my_partid`; it returns after a message when the mandatory-access-control flag `mac_enabled` is set ("Trix system detected, XPMEM driver was not initialized"), when the partition number is -1, when `/hw` cannot be found or when the device cannot be added. Otherwise it adds a character device "xpmem" with prefix "xpmem_" under `/hw` (calls at 0xc0000000000e77e0 and 0xc0000000000e77fc), sets its mode to 0666 with `hwgraph_chmod` (0x1b6, call at 0xc0000000000e780c), allocates its tables (a 0xe00-byte partition array, a 0x1c00-byte process table, a 0x1000-byte access table and per-processor RPC slots) and calls `part_installed`. Only if that is non-zero does it call `bte_init_buffers` and `part_register_handlers` (call at 0xc0000000000e7b14) with `xpmem_partition_up` and `xpmem_partition_down`; if it is zero, as with the stub, it goes straight to `xpmem_partition_up` for the local partition (call at 0xc0000000000e7b58) and returns. So the shipped kernel initialises XPMEM as partition 0 and does not reach the panicking `part_register_handlers`. Established for the paths.

The modules `xpc.o`, `xpc_part.o`, `partition.o`, `partition_part.o`, `cl_part.o` and `if_cl_part.o` are shipped in `6.5.30/usr/cpu/sysgen/IP35boot/` but their implementations are not linked into the shipped 6.5.30 `unix.IP35` (the real objects are `partition_part.o`, `xpc_part.o`, `cl_part.o` and `if_cl_part.o` of the optional subsystem `eoe.sw.partition`, whose inst entries link the plain names `partition.o`, `xpc.o`, `cl.o` and `if_cl.o` to them; the 6.5.30 IP35 `irix.sm` says in a comment that the modules come with that package for Origin 3000 systems, lists `USE: partition xpc if_cl cl xpmem` at lines 554 to 558 and the stubs `partitionstubs xpcstubs` and `if_clstubs xpmemstubs` at lines 584 and 588; the IP27 file instead excludes the four modules at lines 513 to 516) `part_activate`, `cl_open` and `ifcl_output` have no symbol there, and the `xpc_*` and `part_*` names that do exist (for example `xpc_connect`, 8 bytes at 0xc0000000004962f0) are the short routines described next. `xpc_part.o` and `partition_part.o` are byte-identical copies of `xpc.o` and `partition.o` (same hash); `IP27boot` has `xpc.o` and `partition.o` but no `xpc_part.o`, `cl_part.o`, `if_cl_part.o` or `xpmem.o`, and the IP30 and IP32 directories have only the stub archives. Established (file listing and function inventory).

For a consumer this means the linked kernel has a complete XPMEM implementation whose partition and message back ends are stubs: segments can be made and attached inside partition 0, and a segment id naming another partition would reach `xpmem_get_proxy_seg` and then the panicking `xpc_allocate`. That second step is inferred from the call edges (`xpmem_send_msg` calls `xpc_allocate` and `xpc_send`), not run. A kernel linked with the real objects needs a configuration the 6.5.30 tree does not show.

### The partition layer

The real code is in `6.5.30/usr/cpu/sysgen/IP35boot/partition.o` (72 routines, 76352 bytes). Names, imports and messages show what it does.

It numbers partitions from a field of the global data area and requires matching versions (a message says "GDA Version(%d) expected Version(%d): Partition ID set to %d"). It discovers remote partitions by probing NASIDs (`part_probe_nasid`, `part_discovery`), activates them, logs "Partition %d activated" and "Remote partition %d noticed", and declares one dead when it stops responding ("NASID 0x%x partition[%d] no longer responding"). Established for the routine names and messages; the protocol order is inferred.

It exports a local page to another partition and revokes it (`part_export_page`, `part_export_page_perms`, `part_unexport_page`, `part_permit_page`) using the memory directory state (it imports `get_dir_ent`, `set_dir_state` and `is_POISONED_dir_state`), and it poisons ranges (`bte_poison_range`). It rejects an invalid directory state with "Invalid partition directory state: pa=0x%x state=%d". Established for the imports; the state machine is not read.

It sets up and takes an inter-partition interrupt (`part_interrupt_set`, `part_interrupt_id`, `part_interrupt`, using `intr_connect_level`), runs a heartbeat (`part_heartbeat`, `part_heart_beater`, `part_heartbeat_timeout`) and starts a kernel thread per partition (`part_start_partition_thread`, `part_thread`, using `xthread_create`). Established for the imports.

- `part_operation` (`6.5.30/usr/cpu/sysgen/IP35boot/partition.o`, address 0x3f04, 624 bytes) serves `syssgi` 195 with commands 1 to 5. Command 1 returns the local partition number from `part_id` (call at 0x40e4) and an error when it is -1; command 2 copies out up to 128 records of 28 bytes (`copyout` with length 0x1c, call at 0x4050) and the count; command 3 copies out up to 64 records of 40 bytes and the count; commands 4 and 5 check `cap_able` first (calls at 0x410c and 0x4124), and 4 calls `part_id` (call at 0x413c) and then `part_deactivate` (call at 0x4164) for another partition, returning 22 for the local one. Anything else is rejected. Established for the control structure; the field meanings of the records are unknown.

### XPC

Source of the facts: `6.5.30/usr/cpu/sysgen/IP35boot/xpc.o` (44 routines, 57600 bytes). Its channel interface (`xpc_connect`, `xpc_allocate`, `xpc_send`, `xpc_send_notify`, `xpc_receive`, `xpc_poll`, `xpc_disconnect`) is what the stubs in the linked kernel replace.

- `xpc_get_remote` (`6.5.30/usr/cpu/sysgen/IP35boot/xpc.o`, address 0xf0c, 400 bytes) reads another partition's published variables: it calls `part_get_var` (call at 0xf20) and `xpc_pull_data` (call at 0xf50). Established.
- `xpc_pull_data` (`6.5.30/usr/cpu/sysgen/IP35boot/xpc.o`, address 0xaf4, 228 bytes) copies with the block-transfer engine through `bte_unaligned_copy` (call at 0xb24) and `bte_copy` (call at 0xbc4). Established.
- `xpc_part_activate` (`6.5.30/usr/cpu/sysgen/IP35boot/xpc.o`, address 0x109c, 832 bytes) gets a partition's variables with `xpc_get_remote` (call at 0x10a8), sends an inter-partition interrupt with `part_interrupt_set` (call at 0x113c) and adds a hardware-graph path with `hwgraph_path_add` (call at 0x1304). Established.
- `xpc_send_control` (`6.5.30/usr/cpu/sysgen/IP35boot/xpc.o`, address 0x1d14, 204 bytes) builds connect, disconnect and shutdown messages: it calls `xpc_allocate` (call at 0x1d3c) and `xpc_send` (call at 0x1dd0). Established.

So XPC messages are delivered by the receiver pulling the sender's buffers across the NUMA network with the block-transfer engine and being told about them by an interrupt. Inferred from these edges.

### XPMEM

- `xpmem_ioctl` (`6.5.30/unix.IP35`, address 0xc0000000000e8034, 1036 bytes) is the device's control entry. It takes eight commands, `0x787000` to `0x787007` (a range test of `cmd + 0xff879000` below 8 at 0xc0000000000e808c, then a jump table at 0xc00000000049fe40). Command 0 stores 0x10002 to the result (0xc0000000000e80d0 to 0xc0000000000e80dc), a version word; 1 calls `xpmem_make` (call at 0xc0000000000e8284); 2 calls `xpmem_seg_ref_id`, `xpmem_remove` and `xpmem_seg_deref` (calls at 0xc0000000000e83c0 to 0xc0000000000e83e0); 3 calls `xpmem_get` (0xc0000000000e8210); 4 calls `xpmem_release` (0xc0000000000e83f0); 5 returns 0 without doing anything; 6 calls `xpmem_attach` (0xc0000000000e8168); 7 calls `xpmem_detach` (0xc0000000000e8110). For a 32-bit caller the argument blocks go through `copyin_xlate` and the `irix5_n32_to_xpmem_*` routines. Established.
- `xpmem_make` (`6.5.30/unix.IP35`, address 0xc0000000000e8440, 644 bytes) exports a range of the caller's address space as a segment; it calls `kmem_zalloc` and `xpmem_make_handle`. Established for the calls; the role is inferred from the name.
- `xpmem_get` (`6.5.30/unix.IP35`, address 0xc0000000000e8ca0, 744 bytes) opens a segment by identifier: it calls `xpmem_seg_ref_id` (call at 0xc0000000000e8d1c), compares the identifier's top byte with `xpmem_my_partid` (the `lb` at 0xc0000000000e8d30 and compare at 0xc0000000000e8d3c) and calls `xpmem_get_proxy_seg` (call at 0xc0000000000e8d44) for a segment of another partition. That the top byte is a partition number is inferred from the compare. Established for the calls.
- `xpmem_attach` (`6.5.30/unix.IP35`, address 0xc0000000000ea9d0, 888 bytes) maps a segment into the caller. Established for the address and size; its body is not read.
- `xpmem_fault_handler` (`6.5.30/unix.IP35`, address 0xc0000000000ead94, 572 bytes) services a fault on an attached range: it calls `xpmem_ensure_valid_PFNs` and enters the page frames with `v_enterpfns`. Established.
- `xpmem_get_PFNs_PROCESS` (`6.5.30/unix.IP35`, address 0xc0000000000ebc38, 812 bytes) runs in the owning partition for a remote request: it calls `xpmem_lower_firewall` and answers with `xpmem_send_msg`. Established for the calls.
- `xpmem_lower_firewall` (`6.5.30/unix.IP35`, address 0xc0000000000ecec4, 176 bytes) calls `part_export_page_perms`. Established.
- `xpmem_send_msg` (`6.5.30/unix.IP35`, address 0xc0000000000ed744, 288 bytes) sends a request or reply: it calls `xpc_allocate` (call at 0xc0000000000ed780), `bcopy` (call at 0xc0000000000ed820) and `xpc_send` (call at 0xc0000000000ed82c). Established.

So a remote attach is genuine shared memory: loads and stores reach the owner's pages through the interconnect after the owner exports them, with XPC carrying only the page-frame requests and replies. Inferred from the edges above.

- `xpmem_copy` (`6.5.30/unix.IP35`, address 0xc0000000000e997c, 2404 bytes) is `syssgi` selector 196 (case code at 0xc0000000002e20d8, public name unknown). It checks segment access, translates both ends with `xpmem_vaddr_to_paddr` and copies with `bte_copy` or `bte_unaligned_copy` or `bcopy` through `xpmem_initiate_copy` (0xc0000000000ea2e0, 812 bytes). The selector is therefore a copy between address spaces without mapping. Established for the callees; the argument layout (a block of at least seven words read from the user block at +0xc, +0x10, +0x1c, +0x20, +0x28, +0x34, +0x38) is read from the case code only.
- `xpmem_partition_up` (`6.5.30/unix.IP35`, address 0xc0000000000ed02c, 476 bytes) allocates a per-partition record and, for a remote partition, calls `xpc_connect` (call at 0xc0000000000ed19c); `xpmem_partition_down` (0xc0000000000ed208, 224 bytes) and `xpmem_disconnect` (0xc0000000000ed2e8, 744 bytes) tear segments down when a partition goes away. Established.

The `xpmem` device is not linked in the shipped 6.5.30 IP27 kernel (no `xpmem_*` routine; only the stub archive is shipped for IP27, IP30 and IP32). The IP35 6.5.22 kernel has 71 `xpmem_*` routines by name, so the pattern is the same there ([inventory/functions.tsv](inventory/functions.tsv); names only).

### Message device and network interface

`cl_part.o` and `if_cl_part.o` (IP35boot, 17296 and 48944 bytes) are not in the linked kernel. `cl_part.o` registers a character driver with `cl_open`, `cl_close`, `cl_read`, `cl_write` and `cl_poll` on top of XPC (`xpc_connect`, `xpc_allocate`, `xpc_send`, `xpc_receive`, `xpc_poll`) with asynchronous I/O in a thread `cl_async`, and creates device nodes in the hardware graph with names built from four strings, "xplink", "partition", "admin" and "clshm" (format "%s/%s/%s/%s"); `cl_part_activate` (address 0x1ac, 228 bytes) calls `xpc_avail` first and then creates two devices. `if_cl_part.o` is a network interface "ifcl" over XPC channels: it requires `/hw/xplink`, adds a `unit` node under it, builds address-resolution entries ("snarp") and sends mbuf chains with `xpc_send`. The ordering of the four path strings, and so the exact device paths, is not established. Established for the imports and strings; inferred for the roles.

## Release history of migration and replication, 6.5.8 to 6.5.30

The sections above read the 6.5.22 and 6.5.30 kernels. This section reads the kernel object archives that every 6.5.x maintenance release from 6.5.8 (April 2000) to 6.5.30 (July 2006) ships on its first installation disc (`eoe.sw.base`: `usr/cpu/sysgen/IP27boot/numa.a`, `IP35boot/numa.a`, `kernel.o` and the other archives of both directories), plus the `mtune/numa` file, the manual pages and the release notes of the same discs. The kernels of the intermediate releases are not held; they are linked at install time from these objects, so an object's state is the state of that release's kernel. Releases 6.5.2 to 6.5.7 carry the IP27 objects only; IP35 first appears in 6.5.8. Objects were taken out of the disc images into scratch and are not committed.

Method and units: each archive member was parsed as an ELF object. "Bytes" is the member's size in the archive and "code" is the sum of its executable sections. A member with zero code and no symbols is called empty; every empty member is 1204 to 1300 bytes (headers and debug stubs only). Release dates are the modification time of the release's `eoe` product file inside the disc's file system; they are a mastering date, inferred, not an announced release date. Dates: 6.5.8 2000-04-19, 6.5.10 2000-10-13, 6.5.12 2001-04-16, 6.5.14 2001-10-10, 6.5.15 2002-01-10, 6.5.16 2002-04-11, 6.5.17 2002-07-12, 6.5.18 2002-10-16, 6.5.19 2003-01-13, 6.5.20 2003-04-11, 6.5.21 2003-07-15, 6.5.22 2003-10-08, 6.5.23 2004-01-09, 6.5.30 2006-07-21.

### Findings

| Question | Answer | Grade |
|---|---|---|
| Did IP35 ever carry automatic migration code? | Yes. From 6.5.10 (October 2000) to 6.5.16 (April 2002) the IP35 `numa.a` held the whole engine the IP27 build has: interrupt handler, manager, queue, bounce control, periodic tick, counter reader, state machine, unpegging and per-node state, and the IP35 kernel connected interrupt level 3 to the handler. The first two IP35 releases (6.5.8 and 6.5.9) carried the counter reader, manager, queue and tick, and connected level 3 to a placeholder that only prints a "Fixme:" line. | Established (object contents, call edges, instruction readings of the 6.5.8 and 6.5.16 objects) |
| When was it removed from IP35? | In three steps: the interrupt path and tick in 6.5.17 (July 2002), the page-state machine, counter reader and unpegging in 6.5.20 (April 2003), the control record and the `refcnt` device handlers in 6.5.21 (July 2003). The same releases removed the callers from the fault, map and swap paths of the generic VM. | Established |
| Is automatic migration documented as Origin 2000/200 only? | Yes, from 6.5.15 (January 2002) in the manual pages and from 6.5.17 (July 2002) in `mtune/numa`; the objects show the IP35 code still existed then. | Established (dates and texts), inferred (that the documentation changes are the same decision as the code removal) |
| Did IP35 ever work? | Not shown. No run was made, and no held document says the code was exercised on Origin 3000 hardware. The IP35 counter reader addresses a different window and node stride from IP27's and it is not shown that the addresses answer. | Unknown |
| When was page replication removed? | 6.5.23 (January 2004), from both platforms, in one step. It was already off in every release (the enable variable is a kernel variable that no tuning file ever set) and the manual pages called the policies unimplemented from 6.5.15. | Established (objects); inferred (never enabled by shipped configuration) |
| Do the release notes record any of it? | No. The release-note chapters of all 29 releases contain one unchanged paragraph about "S2MP systems" (the Origin 2000 generation) providing migration, and one unchanged paragraph about replication through a tunable that no `mtune/numa` of 6.5.7 or later lists. None of the 6.5.8 to 6.5.30 notes mentions the removal. | Established |

### Object sizes by release


IP35 `numa.a` members, bytes of member / bytes of executable code (`-` = member not in the archive)

| member | 6.5.8 | 6.5.10 | 6.5.16 | 6.5.17 | 6.5.19 | 6.5.20 | 6.5.21 | 6.5.22 | 6.5.23 | 6.5.30 |
|---|---|---|---|---|---|---|---|---|---|---|
| `migr_manager.o` | 8852/1944 | 8948/1944 | 8292/1780 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1268/0 | 1268/0 | 1220/0 |
| `migr_interrupt.o` | 2756/0 | 7396/788 | 7028/704 | 1236/0 | 1236/0 | 1236/0 | 1236/0 | 1268/0 | 1268/0 | 1236/0 |
| `migr_queue.o` | 10628/2208 | 10724/2208 | 10452/2244 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1268/0 | 1268/0 | 1220/0 |
| `migr_bounce.o` | 8660/1580 | 8756/1580 | 8468/1752 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 |
| `mem_tick.o` | 12612/3300 | 12708/3300 | 12388/2996 | 4948/8 | 4996/8 | 4996/8 | 4964/8 | 5140/8 | 5140/8 | 5028/8 |
| `migr_refcnt.o` | 13108/2860 | 13188/2860 | 13108/2772 | 13092/2772 | 13140/2772 | 1220/0 | 1220/0 | 1268/0 | 1268/0 | 1220/0 |
| `migr_states.o` | 11940/2312 | 12036/2312 | 11540/2320 | 11540/2320 | 11604/2320 | 1252/0 | 1252/0 | 1300/0 | 1300/0 | 1252/0 |
| `migr_unpegging.o` | - | 7412/1044 | 7172/1012 | 7172/1012 | 7220/1012 | 1236/0 | 1236/0 | 1268/0 | 1268/0 | 1236/0 |
| `pfms.o` | 5556/208 | 5636/208 | 5732/208 | 5732/208 | 5780/208 | 1204/0 | 1204/0 | 1252/0 | 1252/0 | 1204/0 |
| `migr_init.o` | 2740/0 | 6180/380 | 5972/392 | 5972/392 | 6004/392 | 4852/176 | 4852/176 | 4980/176 | 4980/176 | 4868/176 |
| `migr_control.o` | 15140/3272 | 15396/3356 | 14916/3276 | 14788/3236 | 14836/3236 | 14724/3192 | 6548/600 | 6676/600 | 6676/600 | 6580/600 |
| `mem_refcnt.o` | 8500/888 | 8580/880 | 8596/820 | 8596/820 | 8644/820 | 8644/820 | 6964/92 | 7172/92 | 7172/92 | 7060/92 |
| `migr_policy_control.o` | 8516/852 | 8596/852 | 8308/768 | 8308/768 | 8356/768 | 8356/768 | 7556/284 | 7732/284 | 7732/284 | 7652/284 |
| `numa_hw.o` | - | 6036/216 | 5764/196 | 5764/196 | 5812/196 | 5812/196 | 5812/196 | 5924/196 | 5924/196 | 5828/196 |
| `migr_engine.o` | 19508/5236 | 19588/5236 | 19348/4868 | 19284/4824 | 19332/4824 | 18900/4692 | 18884/4676 | 19316/4676 | 19284/4660 | 19588/4724 |
| `migr_coald.o` | 10532/2432 | 10628/2432 | 10356/2344 | 10356/2344 | 10420/2344 | 9092/1740 | 9092/1740 | 9556/1740 | 9556/1740 | 8596/1668 |
| `migr_user.o` | 11812/2096 | 12004/2188 | 18516/5124 | 18516/5124 | 18564/5124 | 15812/3908 | 15908/3928 | 16308/3928 | 16308/3928 | 16292/3964 |
| `migr_policy_default.o` | 7748/448 | 7828/448 | 7716/396 | 7716/396 | 7764/396 | 7764/396 | 7764/396 | 7940/396 | 7940/396 | 7876/396 |
| `migr_policy_refcnt.o` | 7604/392 | 7700/392 | 7572/340 | 7572/340 | 7620/340 | 7620/340 | 7620/340 | 7796/340 | 7796/340 | 7732/340 |
| `migr_traffic.o` | - | 2836/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | - | - | - | - |
| `repl_control.o` | 7828/636 | 7908/636 | 7732/608 | 7732/608 | 7780/608 | 7780/608 | 7780/608 | 7908/608 | - | - |
| `repl_pageops.o` | 7892/972 | 7972/972 | 7796/872 | 7796/872 | 7844/872 | 7844/872 | 7844/872 | 8052/872 | - | - |
| `repl_policy.o` | 7636/424 | 7716/424 | 7620/404 | 7620/404 | 7668/404 | 7668/404 | 7668/404 | 7828/404 | - | - |
| `repl_policy_default.o` | 16260/2712 | 16356/2712 | 15284/2396 | 15284/2396 | 15332/2396 | 15348/2396 | 15348/2396 | 15492/2396 | - | - |
| `repl_policy_one.o` | 7588/356 | 7684/356 | 7188/296 | 7188/296 | 7236/296 | 7236/296 | 7236/296 | 7412/296 | - | - |
| `repl_shoot.o` | 10788/1792 | 10884/1792 | 10708/1700 | 10708/1700 | 10756/1700 | 10756/1700 | 10756/1700 | 11300/1756 | - | - |
| `repl_vnodeops.o` | 11988/1500 | 12068/1500 | 11892/1348 | 11892/1348 | 11956/1348 | 11956/1348 | 11956/1348 | 12708/1552 | - | - |
| `replinit.o` | 10036/640 | 10116/640 | 9924/624 | 9924/624 | 9972/624 | 9972/624 | 9972/624 | 10196/624 | - | - |
| `repldebug.o` | 2740/0 | 2820/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | - | - |

IP27 `numa.a` members, bytes of member / bytes of executable code (`-` = member not in the archive)

| member | 6.5.8 | 6.5.10 | 6.5.16 | 6.5.17 | 6.5.19 | 6.5.20 | 6.5.21 | 6.5.22 | 6.5.23 | 6.5.30 |
|---|---|---|---|---|---|---|---|---|---|---|
| `migr_manager.o` | 9236/1944 | 9252/1944 | 8372/1780 | 8372/1780 | 8420/1780 | 8420/1780 | 8420/1780 | 8548/1780 | 8548/1780 | 8452/1780 |
| `migr_interrupt.o` | 7668/780 | 7684/780 | 7092/704 | 7108/704 | 7156/704 | 7156/704 | 7156/704 | 7284/704 | 7284/704 | 7188/704 |
| `migr_queue.o` | 11124/2204 | 11140/2204 | 10532/2236 | 7812/1128 | 7860/1128 | 7860/1128 | 7860/1128 | 7988/1128 | 7988/1128 | 7876/1128 |
| `migr_bounce.o` | 9156/1572 | 9188/1572 | 8548/1752 | 8548/1752 | 8596/1752 | 8596/1752 | 8596/1752 | 8724/1752 | 8724/1752 | 8628/1752 |
| `mem_tick.o` | 12932/3188 | 12964/3188 | 12356/2900 | 12100/2892 | 12148/2892 | 12164/2892 | 12132/2892 | 12276/2892 | 12276/2892 | 12180/2892 |
| `migr_refcnt.o` | 14484/3624 | 14516/3624 | 14692/3600 | 14692/3600 | 14740/3600 | 14740/3600 | 14740/3600 | 14932/3600 | 14932/3600 | 14836/3600 |
| `migr_states.o` | 12452/2304 | 12484/2304 | 11620/2312 | 11620/2312 | 11668/2312 | 11684/2312 | 11684/2312 | 11828/2312 | 11828/2312 | 11780/2312 |
| `migr_unpegging.o` | 7716/1028 | 7748/1028 | 7252/1008 | 7252/1008 | 7300/1008 | 7300/1008 | 7300/1008 | 7412/1008 | 7412/1008 | 7316/1008 |
| `pfms.o` | 5924/208 | 5940/208 | 5828/208 | 5828/208 | 5860/208 | 5876/208 | 5876/208 | 6084/208 | 6084/208 | 5972/208 |
| `migr_init.o` | 6468/380 | 6484/380 | 6052/392 | 6052/392 | 6100/392 | 6100/392 | 6100/392 | 6228/392 | 6228/392 | 6116/392 |
| `migr_control.o` | 15636/3320 | 15668/3320 | 14980/3256 | 14980/3256 | 15012/3256 | 15012/3256 | 14900/3176 | 15028/3176 | 15028/3176 | 14916/3176 |
| `mem_refcnt.o` | 8916/912 | 8948/912 | 8756/864 | 8756/864 | 8804/864 | 8804/864 | 8804/864 | 8996/864 | 8996/864 | 8900/864 |
| `migr_policy_control.o` | 8884/852 | 8900/852 | 8404/768 | 8388/768 | 8452/768 | 8452/768 | 8452/768 | 8516/756 | 8516/756 | 8452/756 |
| `numa_hw.o` | 6724/344 | 6756/344 | 6276/328 | 6276/328 | 6308/328 | 6324/328 | 6324/328 | 6436/328 | 6436/328 | 6340/328 |
| `migr_engine.o` | 19924/5140 | 19940/5140 | 19380/4804 | 19300/4760 | 19348/4760 | 19348/4760 | 19332/4744 | 19764/4744 | 19732/4728 | 19988/4800 |
| `migr_coald.o` | 11044/2420 | 11060/2420 | 10436/2332 | 10436/2332 | 10484/2332 | 10484/2332 | 10484/2332 | 10948/2332 | 10948/2332 | 10004/2272 |
| `migr_user.o` | 12292/2092 | 12436/2184 | 18548/5080 | 18548/5080 | 18596/5080 | 18084/5064 | 18084/5064 | 18484/5064 | 18484/5064 | 18468/5140 |
| `migr_policy_default.o` | 8116/448 | 8132/448 | 7796/396 | 7796/396 | 7844/396 | 7844/396 | 7844/396 | 8020/396 | 8020/396 | 7956/396 |
| `migr_policy_refcnt.o` | 7988/392 | 8004/392 | 7652/340 | 7652/340 | 7716/340 | 7716/340 | 7716/340 | 7892/340 | 7892/340 | 7812/340 |
| `migr_traffic.o` | 5156/16 | 5188/16 | 4820/16 | 4820/16 | 4868/16 | 4868/16 | - | - | - | - |
| `repl_control.o` | 8180/620 | 8212/620 | 7796/596 | 7796/596 | 7844/596 | 7844/596 | 7844/596 | 7988/596 | - | - |
| `repl_pageops.o` | 8260/972 | 8276/972 | 7892/872 | 7892/872 | 7940/872 | 7940/872 | 7940/872 | 8132/872 | - | - |
| `repl_policy.o` | 8004/424 | 8020/424 | 7700/404 | 7700/404 | 7748/404 | 7748/404 | 7748/404 | 7908/404 | - | - |
| `repl_policy_default.o` | 15220/2264 | 15236/2264 | 14692/2112 | 14692/2112 | 14740/2112 | 14740/2112 | 14740/2112 | 14900/2112 | - | - |
| `repl_policy_one.o` | 7764/300 | 7780/300 | 7156/248 | 7156/248 | 7204/248 | 7204/248 | 7204/248 | 7380/248 | - | - |
| `repl_shoot.o` | 11124/1760 | 11140/1760 | 10756/1668 | 10756/1668 | 10804/1668 | 10804/1668 | 10804/1668 | 11332/1724 | - | - |
| `repl_vnodeops.o` | 12356/1500 | 12388/1500 | 11972/1348 | 11972/1348 | 12036/1348 | 12036/1348 | 12036/1348 | 12804/1552 | - | - |
| `replinit.o` | 10404/640 | 10420/640 | 10020/624 | 10020/624 | 10052/624 | 10068/624 | 10068/624 | 10276/624 | - | - |
| `repldebug.o` | 3108/0 | 3124/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | 1220/0 | - | - |

Reading the tables:

- IP35, interrupt side: `migr_manager.o`, `migr_queue.o`, `migr_bounce.o`, `migr_interrupt.o` and the periodic parts of `mem_tick.o` go from full to empty between 6.5.16 and 6.5.17. `mem_tick.o` keeps only the 8-byte `mem_tick_init` that reports success. `hub_migrintr_init` (108 or 112 bytes, in `kernel.o`) is present in 6.5.8 to 6.5.16 and absent from 6.5.17. In 6.5.16 the `clock` object (`os.a`) calls `mem_tick` and `main.o` calls the 8-byte `memoryd_init`; both calls are gone in 6.5.17.
- IP35, state side: `migr_refcnt.o`, `migr_states.o`, `migr_unpegging.o` and `pfms.o` go empty between 6.5.19 and 6.5.20. In 6.5.17 to 6.5.19 the `os.a` members `fault.o`, `pas_addspace.o`, `pmap.o`, `region.o`, `rmap.o`, `swapalloc.o`, `getpages.o` and `sched.o` and `proc.a:prusrio.o` still reference `migr_start` and `migr_stop` (the page-fault and unmap hooks); from 6.5.20 nothing in the IP35 archives references them. `migr_init.o` shrinks from 392 to 176 bytes at 6.5.20 (it stops calling the counter initialiser and keeps the threshold store).
- IP35, 6.5.21: `migr_control.o` drops from 3192 to 600 bytes of code (the part that read the tunables and sized the queue), `mem_refcnt.o` from 820 to 92 (the `refcnt` device handlers become 8-byte error returns), `migr_policy_control.o` from 768 to 284, and `migr_traffic.o` leaves the archive on both platforms.
- IP35, earlier: `migr_interrupt.o`, `migr_init.o`, `migr_unpegging.o` and `numa_hw.o` (which holds `migr_effthreshold_diff_set`, 64 bytes, and `migr_pageref_disable`) are empty or absent in 6.5.8 and 6.5.9 and complete from 6.5.10. In 6.5.8 and 6.5.9 `kernel.o` supplies 40- to 44-byte stand-ins for `migr_init`, `migr_intr_handler` and `migr_intr_prologue_handler` that print a "Fixme:" line with a line number, and an 8-byte stand-in for the unpegging control. So the IP35 port was brought up in two steps in 2000 (6.5.8 stubs, 6.5.10 complete) and used for six releases.
- IP27: nothing is removed from the migration set, except that 6.5.17 removes the queue draining routines the two queue-draining routines (`migr_queue.o` halves) and the 8-byte `memoryd_init`, and 6.5.20 removes the two 8-byte freeze and thaw interface stubs on both platforms. `migr_traffic.o` (a 16-byte object) goes in 6.5.21. The user-requested path grows: `migr_move_lpage` (6.5.15) and `migr_vas_pages` (6.5.16) are added to `migr_user.o` on both platforms.
- Both platforms: `repl_control.o`, `repl_pageops.o`, `repl_policy.o`, `repl_policy_default.o`, `repl_policy_one.o`, `repl_shoot.o`, `repl_vnodeops.o`, `replinit.o` and `repldebug.o` are present in 6.5.8 to 6.5.22 and absent from 6.5.23. `repldebug.o` has no code on either platform in any release. `pm.o` gains `pm_repl_srvc` in 6.5.23, a policy-module service routine that reports an error for any request.
- `pfms.o` (the per-node page-state allocator, `nodepda_pfms_alloc`, 208 bytes) is present on IP27 in every release to 6.5.30.

### What 6.5.23 removed for replication

Established from the 6.5.22 and 6.5.30 linked kernels and the 6.5.22 and 6.5.23 objects:

- Routines (both platforms): `repl_interpose`, `repl_dispose`, `repl_init`, `repl_pfind`, `repl_open`, `repl_vninit`, `repl_inactive`, `repl_reclaim`, `repl_control`, `repl_control_init`, `repl_resource_check`, `repl_policy_init`, `repl_policy_setup`, `repl_policy_teardown`, `repl_policy_enable`, `repl_policy_disable`, `repl_policy_default_init`, the `repl_policy_one_*` set, the `rp_default_*` set, `page_replica_insert`, `page_replica_remove`, `page_replica_shoot`, `page_replica_free`, the `replattr_*` set (create, free, lookup, init and the state-transition routines) and `replica_selector`.
- Variables: `numa_page_replication_enable`, `numa_repl_control_enabled`, `numa_repl_mem_lowmark` and `numa_repl_traffic_highmark_percentage` are in the 6.5.22 kernels and not in the 6.5.30 ones. None is a tuning-file entry.
- VM paths that called them in 6.5.22 (object level): the executable mapping path (`exec.o`, and `elf.o` and `elf64.o` for the dispose call), the vnode initialisation and lookup (`vnode.o`) and the vnode page cache (`vnode_pcache.o`, the replica removal). In 6.5.23 none of these objects references a replication routine.
- Compile switches: the kernel configuration file of each CPU board (`system/irix.sm`, the `CCOPTS` line) lists the NUMA switches. Both boards carry `-DNUMA_REPLICATION` and `-DNUMA_REPL_CONTROL` through 6.5.22 and lose them in 6.5.23, the release that removes the replication objects; `-DNUMA_PM` and `-DNUMA_TBORROW` go in 6.5.21 on both boards. `-DNUMA_MIGR_CONTROL`, `-DNUMA_BASE` and `-DNUMA_SCHED` stay on both boards to 6.5.30, so the migration switch is still on for IP35 while the IP35 migration objects are empty: the withdrawal was not made by turning that switch off. The IP35 file also carries `-DSN1_USE_BTE` (IP27: `-DSN0_USE_BTE`), `-DBRINGUP` and `-DBEDROCK_BDECC_WAR` in every release from 6.5.8. The debug information of the linked 6.5.22 and 6.5.30 kernels records the switches per compile line as well; the 6.5.30 IP27 kernel still lists the replication switches on 12 lines although no replication code remains.
- What stayed as dead references in 6.5.30: the policy names `ReplicationDefault` and `ReplicationOne` (strings), `pm_repl_srvc`, an incomplete `repl_control_data_s` type in the debug information, and `numa_kernel_replication_ratio` with `replicate_kernel_text` and `setup_replication_mask`. The last three belong to kernel text replication, which is a different and live feature: `replication(5)` in 6.5.14 and later describes only it, and its tuning entry has been in the file since 6.5.7.
- Not found: a system call, tunable or `syssgi` selector that exists only for page replication. Replication was reached through policy-module names. The page-descriptor flag and field layout that marked replica pages was not decoded; unknown.

Tunables in `mtune/numa` (the same file for IP27 and IP35, compared across all releases): revision 1.37 (6.5.8 to 6.5.14) lists `replication(5)` in its header and names two migration vehicles in a comment (block-transfer engine, TLB synchronisation); revision 1.38 (6.5.15) drops `replication(5)` from the header and marks the vehicle entry "changing this has no effect"; revision 1.39 (6.5.17) adds a notice that the migration and extended reference-counter parameters apply "only to Origin 2000 systems", marks the migration, filter, periodic and experimental groups "Origin 2000 only" (the extended-counter group "Origin 200 and Origin 2000 only") and marks the replication group "No effect on any systems, reserved for future"; revision 1.40 (6.5.21) removes `numa_allocation`. No migration entry was deleted in any revision (`numa_zone_radius_max` went in 6.5.15 and `numa_allocation` in 6.5.21), so the migration entries survive as dead entries ([Tunables](#tunables)).

### Placement and cpuset code that shares the structures

The placement-policy modules (`plac_policy_*`, `fbck_policy_*`, `pm*`, `pmo_*`, `mld*`, `aspm`, `affinity`, `afflink`, `memsched`, `cpuset_mem`) are present on both platforms in every release, with growth rather than removal. Established changes on IP35 (IP27 is the same unless noted):

- 6.5.9: `miser_cpuset_memalloc_ok` becomes `cpuset_memalloc_ok`; 6.5.12: `cpuset_release_cm` is added and the `mld_relocate`, `mldset_relocate` and `aspm_relocate` routines are removed on both platforms.
- 6.5.15 and 6.5.16: `migr_move_lpage` and `migr_vas_pages` are added to the user-request path; `cpusetMoveMigrate(3x)` first appears in 6.5.16. Moving a process set between cpusets now moves its memory on request on every NUMA platform, in the same two releases that retire automatic migration on IP35.
- 6.5.16: `pmo_ns.o` roughly doubles (list-style lookups of memory locality domains) and `aspm_set_pmolookup`, `aspm_rel_pmolookup`, `aspm_setdefaultpmpage` and `aspm_clrdefaultinherit` are added; `memsched.o` gains `pmo_xface_mld_place`. 6.5.14: `physmem_place_node`.
- 6.5.17: `fbck_policy_lpage_create` and `fbck_policy_lpage_fallback` (large-page fallback).
- 6.5.23: `memaff_setaff` is added (both platforms) and `raff_setup` removed (IP35; IP27 not compared); 6.5.24: the `pmfactory_ns_*` set is removed.
- 6.5.30: `migr_pfdat_do_xfer` is added to `migr_engine.o` (the helper that moves a page's identity between frames; it is called from the anonymous-page and vnode-page-cache code). It is on both platforms and is part of the surviving explicit-migration path.

### What the working IP27 path consumed

Read from the 6.5.30 and 6.5.16 IP27 objects and cross-checked against the 6.5.16 IP35 objects. The readings are of instruction sequences; the vendor hardware names come from the manual pages and are graded in the last column.

| Topic | What the objects show | Grade |
|---|---|---|
| Counters | One 64-bit word per page and per node, read through an uncached window whose base word is `0x90000000c0000000` (the backdoor view of the directory memory; the base is in the `migr_refcnt` object's literal pool). The address is the base, bits taken from the page's physical page number, and the node slot times 8. The count is held in bits 5 and up: eleven bits (mask 0x7ff) for standard memory, nineteen bits (mask 0x7ffff) for premium memory, chosen by a per-node flag (inferred to come from the memory-population check). The update routine folds the hardware count into a 64-bit software counter per node and page (arrays allocated at boot with `kmem_contig_alloc_node`) and writes counter words back shifted left by five; the restart routine does the same with a baseline. | Established for masks, shifts and call edges; inferred for the baseline role and the meaning of the window |
| Threshold and compare | The tunables give a threshold percentage (`numa_migr_default_threshold`, 50) and a reference (`numa_migr_threshold_reference`: 2048 for the 11-bit counters, 524288 for 19-bit). At boot the threshold is stored through `migr_effthreshold_diff_set`, which writes `0xfffff` minus the derived count with bit 63 set at offset `0x30` of the hub register window `0x9200000001a00000` (the IP27 routine is described above; its caller was not decoded for IP27). The hub compares, per page, the remote counter against the local one and raises a candidate when the difference reaches the threshold (vendor comment in `mtune/numa`; on Bedrock the datasheet says the same). | Established for the store; inferred for the hub-side compare and the software compare |
| Registers and interrupt | `hub_migrintr_init` connects interrupt level 3 for the first processor of every node (skipping single-node systems). In the `IP27` handler the three registers are: `+0x30` the difference threshold, `+0x38` a second threshold register (inferred to be the absolute-value threshold) and `+0x48` the candidate register read in a loop. The prologue clears bit 63 of `+0x30` and `+0x38` (disarm), the handler reads `+0x48` until its valid bit (63) is clear and decodes each candidate (a page number in the low 18 bits, a node index in bits 20 to 28 and a type bit at bit 30; field meanings inferred), and the last act is to set bit 63 of `+0x30` and `+0x38` again (re-arm). This corrects the earlier reading in this page that took `+0x30`, `+0x38` and `+0x48` of the IP27 window for the candidate, candidate-clear and absolute-threshold registers: the same-named routine that arms the threshold writes `+0x30`, so `+0x30` is the difference threshold. The IP35 handler of 6.5.10 to 6.5.16 has the same shape on the Bedrock layout: it reads `+0x38` in a loop (candidate, read to clear), masks the page number to 21 bits, takes the node from the bits above, and re-arms `+0x40` and `+0x48`, which is the layout in the Bedrock memory/directory datasheet. (grade: established) | Established for the accesses and sequence; inferred for field and register meanings |
| Dispatch | For each candidate the handler locates the page descriptor through the node's page array and reads its migration-state word. Depending on the candidate's type bit and on flag bits of that word (0x10, 0x4000, 0x20000) it either restarts the page's counters (`migr_refcnt_restart`), folds them (`migr_refcnt_update_counters` with 0x10 or 0x18 as the last argument, the overflow case) or calls `migr_manager`; an unknown type calls `cmn_err` with "Invalid intr type". `migr_manager` takes the state word's low bit as a lock at raised priority level and tests the bits above it for the state. | Established for the branches; inferred for what each flag means |
| Queue, tick and daemon | There is no daemon: `memoryd_init` is an 8-byte stub to 6.5.16 on both platforms and `memoryd_enabled` is 0. The handler runs as an interrupt thread (the hardwired-table flag in [interrupts.md](interrupts.md)). The clock calls `mem_tick` each tick; `mem_tick` calls the bounce and unpegging controls only. `migr_queue_insert` holds requests that cannot move at once (bounded per node; the length tunable is `numa_migr_queue_maxlen`, inferred) and its draining routines were removed from both platforms in 6.5.17, so the queued-migration mechanism is vestigial in 6.5.30 ("Execute migration immediately (ONLY mode available)" in the 6.5.22 `mtune` comment). | Established for call edges; inferred for roles |
| Page move | `migr_manager` takes the page-state lock, folds the counters, then checks the node distance (`node_distance`), migratability (`migr_fastcheck_migratable`) and the bounce state (`migr_manager_bounce_control`), and either calls `migr_migrate_page` or queues the request with `migr_queue_insert`. The "Migration Filters" group of `mtune/numa` (minimum distance, destination memory pressure, freeze, melt, dampening) names the other conditions; that the manager consults them is inferred. Moving a page: allocate a frame on the destination node (`pagealloc_size_node`), then `migr_migrate_frame` runs the prologue (take the owning anonymous or vnode object with `anon_pagemigr` or `vnode_pagemigr` and walk every mapping with `rmap_scanmap` under the 64-bit lock), `migr_pagemove` (copy with the block-transfer engine, `bte_pbcopy`, or `bte_poison_copy` for poisoned pages), the epilogue (rewrite the mappings, `pagedone`, free the old frame), then `tlbsync` (cross-processor TLB flush) and `cache_operation` (flush the old page's cache lines). `migr_page_unmap_tlb` probes and unmaps the local TLB entry. `migr_pfdat_xfer` and `rmap_xfer` move the page's identity inside the anonymous and vnode caches. The same engine serves the user-request path (`migr_user_page_migrate`, `migr_move_lpage`, which also handles large pages). | Established for the call order; inferred for which filters are consulted |
| Freeze, thaw and hysteresis | `migr_start` and `migr_stop` bracket the 17 fault, copy, swap-in and mapping sites ([What drives migration](#what-drives-migration)): they fold or restart the counters when a page changes mapping state. `migr_periodic_bounce_control` (called from `mem_tick`) is inferred, from its name and the tuning comments, to find pages that moved too often and freeze them (`numa_migr_freeze_*`), applies dampening (`numa_migr_dampening_*`) and melts them (`numa_migr_melt_*`) after the interval; `migr_periodic_unpegging_control` thaws pages whose counters have decayed (`numa_migr_unpegging_control_*`, enabled by default). The tunable names, defaults and comments are in `mtune/numa`; the routines read those tunables. | Established for the structure; inferred for the exact policy |
| Which processes and regions use it | The system mode `numa_migr_default_mode` is 2 by default (migration off, a user may turn it on); `dplace -migration` and the `_DSM_MIGRATION` environment variable turn it on per program, `sn(1)` per node, and the memory management interface selects a migration policy module per region or address space (the default, control and counter policies registered by name in `numa_init`). The release notes say the same. | Established for the manual pages and the policy table; inferred for the precedence |

### Hardware side

| Item | Kernel evidence | Datasheets | Neither |
|---|---|---|---|
| Migration interrupt source | IP27 and IP35 (6.5.8 to 6.5.16) connect level 3 to the migration handler. The hardwired table of every release names level 3 "Migration". | The Bedrock processor-interface datasheet lists pending bank 0 bit 3 as page migration (from public vendor headers, not measured). | Whether Bedrock raises it. |
| Candidate and thresholds | The 6.5.10 to 6.5.16 IP35 handler reads window `+0x38` and arms `+0x40` and `+0x48`; the 6.5.10 to 6.5.30 boot code arms `+0x40` through the 15-bit routine. | The Bedrock memory/directory datasheet defines candidate (`0x7c0030`, 21-bit page address, 11-bit initiator field), candidate clear (`0x7c0038`), difference threshold (`0x7c0040`, 15 bits, valid bit 63) and value threshold (`0x7c0048`); the handler's mask of 21 bits, the clear register and the two re-armed registers agree with it. | The effect of the action fields; the candidate-clear semantics (the datasheet itself marks the clear as not established). |
| Migration enable | The 6.5.30 kernel has no access to the migration configuration register (`0x780018`, window address `0x9200000001f80018`; [Hardware that counts references](#hardware-that-counts-references)). No literal for that address appears in the 6.5.8, 6.5.10 or 6.5.16 IP35 objects; those kernels were not searched for computed addresses. | The datasheet lists the register and its fields. | Whether the enable bit is set at power-on, and the interval units. |
| Per-page counters | The IP35 reader of 6.5.8 to 6.5.19 (objects of 6.5.8 and 6.5.16 read) reads one 64-bit word per page at a base of `0x90000001c0000408` plus a page-derived part and a node stride of `(node & 0x1e) << 5`, and uses the same 11-bit and 19-bit masks as IP27. IP27 uses base `0x90000000c0000000` and stride 8. | The Bedrock memory/directory datasheet places the page-counter word at the backdoor `+8`, names a separate protection entry at `+0x408`, gives no field layout and says the counter fields, increment and saturation are not established. | The IP35 reader's base is the protection-entry base of the datasheet, not the counter base at `+8`. Either the kernel's address transform reads the counter block through a different alias, or the code was never exercised against hardware. Unknown. |
| Threshold width and node shift | `migr_effthreshold_diff_set` stores `0x7fff` minus the count (15 bits) on IP35 and `0xfffff` minus it (20 bits) on IP27; the node halfword is shifted by 33 on IP35 and 32 on IP27. | The datasheet gives a 15-bit threshold field. | The 20-bit IP27 field has no datasheet here (the Origin 200 hub datasheet lists four migration registers as out of scope). |

What the 15-bit routine implies about the counter layout: the threshold register is compared against a count difference, so the difference must fit in 15 bits; an 11-bit standard counter (maximum 2047) and a 19-bit premium counter (maximum 524287) both appear in the IP35 reader with the same masks as IP27, so the reader assumes the IP27 widths while the threshold field (maximum 32767) is narrower than the premium counter. The routine's use of `0x7fff` minus the count shows that the hardware compares against a down-counted value, as on IP27. Neither fact tells the real Bedrock counter layout. Inferred.

Not established by anything held: whether Bedrock silicon increments any per-page counter, and what it does on overflow.

### Why it was removed

What the sources say, in order:

| Release (date of the disc image) | What changed | Grade |
|---|---|---|
| 6.5.15 (2002-01-10) | `numa(5)`, `migration(5)`, `refcnt(5)`, `mmci(5)` and `dplace(1)` are reworded to say that dynamic migration and the reference counters are an Origin 2000/200 capability and that "any other Origin product line does not have automatic memory locality management". `replication(5)` (first in the held set at 6.5.14) describes only kernel text replication; `mmci(5)` calls `ReplicationDefault` and `ReplicationOne` "not implemented". The `mtune/numa` header drops `replication(5)` and the migration-vehicle choice. | Established for the texts; the IP35 code was still in the kernel then |
| 6.5.17 (2002-07-12) | `mtune/numa` gains the "Origin 2000 only" notice; `mmci(5)` stops saying locality comes "via dynamic page migration and replication" (it now says "via initial placement") and marks the paging policy not implemented; the IP35 interrupt path, manager, queue, bounce control and tick are emptied. | Established |
| 6.5.20 and 6.5.21 (2003) | the rest of the IP35 migration state and control code is emptied; `mmci(5)` says the three migration policy modules apply only to Origin 2000. | Established |
| 6.5.23 (2004-01-09) | replication objects disappear from both platforms. | Established |

What nothing says: a reason. No held release note, error string, tunable comment or install note states why the IP35 code was withdrawn, and the withdrawal is not recorded in any release note. The vendor texts call it a hardware property of the Origin 2000/200 (the reference counters), not a missing driver. Two readings fit the evidence: the Bedrock counters never behaved as the Origin 2000 ones did, or the facility did not pay for itself on a machine with fewer, closer nodes. The release notes of every release from 6.5.8 to 6.5.30 carry the sentence "on systems up to 32 processors, all memory in the system is sufficiently close topologically that page migration is normally not required". The Origin 200 hub datasheet adds a clue about the first generation: the firmware guards "a migration workaround" that names the memory unit (the Origin 300 PROM was not searched for one). None of these is evidence of the cause. Why: unknown; the sequence and the dates are established.

This replaces the earlier verdict that the objects "suggest the work was started for the Origin 300 and left out": the objects show the opposite, a completed port that shipped for six releases and was then withdrawn. The `Fixme:` stubs of 6.5.8 and 6.5.9 are the start of that port.

### What IP35 uses instead

Established from the 6.5.30 objects and manual pages:

- **Placement at allocation time.** `PlacementDefault` (a memory locality domain per four processes on an Origin 3000, with each process's affinity link set to it), `PlacementFirstTouch`, `PlacementFixed`, `PlacementRoundRobin`, `PlacementThreadLocal` and `PlacementCacheColor` (the `plac_policy_*` objects, all present on IP35 in every release), with the fallback policies and the large-page fallback added in 6.5.17. `numa(5)` calls first-touch placement plus migration "automatic memory locality management" and says it exists on Origin 2000/200 only; the placement objects are present and used on IP35 (`mmci(5)` describes the Origin 3000 behaviour of `PlacementDefault`), so what a 3000 gets by default is first-touch-style placement through the affinity link, without the migration half; that default is a reading of the manual pages and was not observed.
- **Allocation radius.** `numa_zone_radius` (default 1) bounds how far a zone allocation searches; the memory scheduler (`memsched`, `physmem_place_node`) and memory affinity links steer a thread's memory and its CPU together.
- **cpusets.** `cpuset_mem.o` keeps allocations inside the nodes of a cpuset when `CPUSET_MEMORY_LOCAL` is set, `CPUSET_MEMORY_EXCLUSIVE` keeps other threads' allocations out, `CPUSET_MEMORY_KERNEL_AVOID` keeps the buffer cache away, and `CPUSET_MEMORY_MANDATORY` with the page or kill policy limits allocation to the cpuset, and `cpusetMoveMigrate` / `cpuset -M` moves stopped processes and their memory when they change cpusets (6.5.16).
- **dplace.** A placement file names memory locations and threads (`run thread N on memory M`) and `-mustrun` pins threads to CPUs; `-migration` has no effect on Origin 3000.
- **Miser.** A batch scheduler reserving CPUs and memory per queue; it uses cpusets.
- **Explicit migration.** `migr_xface_migrate` (`pmoctl` 40 to 42), `migr_range_migrate` and the coalescing thread, as in [What still moves pages on IP35](#what-still-moves-pages-on-ip35).

A workload on an Origin 3000-class machine therefore gets first-touch or policy-module placement, optional cpuset memory confinement, large pages and, on request, one-shot migration. It never gets pages moved because of observed remote traffic. The 6.5.11 to 6.5.14 notes point out that small machines (up to 32 processors) rarely need it.

### What a re-implementation would need from the running system

Data requirements only; none of this is a statement of how to obtain them.

- The kernel's symbol table (the shipped kernels carry symbols and debug information) giving addresses for: the interrupt reservation and connect routines and the per-node hardwired-level table; the surviving explicit-move routines (`migr_migrate_page`, `migr_migrate_frame`, `migr_pagemove`, `migr_page_prologue`, `migr_page_epilogue`, `migr_pfdat_xfer`, `migr_user_page_migrate`, `migr_move_lpage`); the TLB shootdown and cache flush routines (`tlbsync`, `unmaptlb`, `cache_operation`); the reverse-mapping walkers (`rmap_scanmap`, `rmap_xfer`); the page allocator for a given node (`pagealloc_size_node`) and the free routines; the block-transfer copy (`bte_pbcopy`).
- The layout of the page descriptor (`pfdat`), the per-node record (`nodepda`, including the pointer to the control record `migr_control_init` allocates), the node array and the node-number conversions (`nasid_to_compact_node` and its inverse), and the owning-object back references (anonymous object and vnode) that `anon_pagemigr` and `vnode_pagemigr` follow; all from the kernel's debug information.
- The values and addresses of the 47 tunables in [Tunables](#tunables), so a policy can be configured the same way, and the address of the IP35 threshold register routine and its per-node record bytes.
- On the hardware side: whether Bedrock increments a per-page counter, the counter word's address transform and field layout, whether `MD_MIG_CONFIG` must be written to enable the interval and node mask, and the effect of the threshold action fields. The kernel evidence (the 6.5.10 to 6.5.16 reader and handler) gives a candidate address transform and masks to test against; it does not prove them.
- The node distance table and `physmem_maxradius`, to apply a minimum-distance filter.
- The statistics the `nstats` tool prints, so a reimplementation can report through the same names.

### The 6.5.10-6.5.16 IP35 handler in detail

What a re-implementation of the IP35 automatic page-migration handler has to know, read from the IP35 `numa.a`, `kernel.o` and `os.a` objects of one release. Nothing was run. The summary rows in [What the working IP27 path consumed](#what-the-working-ip27-path-consumed) and [Hardware side](#hardware-side) stay as they are; this section adds the detail and corrects two sentences of that summary (see [Corrections](#corrections-to-the-summary-above)).

**Release read: 6.5.16** (April 2002, the last release before the handler was emptied; the objects come from `eoe.sw.base` on the first installation disc). Method: the members were disassembled with `scripts/hostfacts.py` and a relocation-aware Capstone listing, and the register and structure accesses were followed by hand. A hash of the `.text` of the key members across 6.5.10 to 6.5.17 gives:

| Member (`numa.a`) | 6.5.10 | 6.5.11-6.5.13 | 6.5.14 | 6.5.15 and 6.5.16 | 6.5.17 |
|---|---|---|---|---|---|
| `migr_interrupt` | 788 bytes | 712 | 704 | 704, identical | emptied |
| `migr_manager` | 1944 | 1776 | 1780 | 1780, identical | emptied |
| `migr_bounce`, `migr_queue` | 1580, 2208 | 1748, 2244 | 1756, 2244 | 1752, 2244, identical | emptied |
| `migr_refcnt`, `migr_engine`, `migr_states`, `migr_control`, `migr_unpegging`, `mem_tick`, `migr_init` | present | present | present | present, identical | present; `migr_refcnt`, `migr_engine` and `migr_control` differ, `mem_tick` is an 8-byte stub |
| `numa_hw` | 216 | 196 | 196 | 196 | 196 |

6.5.15 and 6.5.16 are byte for byte the same in every member listed, so "6.5.16" below also holds for 6.5.15. The 6.5.10 `migr_intr_handler` was read as well: it decodes the candidate the same way (same masks, same bit 33 type test, same requester bits, same repeat check, same re-arm), only the register allocation and the code size differ. The other releases were compared by size and hash, not read line by line; any difference in the 6.5.10 to 6.5.14 members beyond the handler is unknown. Grades: **E** established (read in full at the address), **I** inferred (reasoning given), **U** unknown.

Addresses are `member+offset` inside the 6.5.16 IP35 `numa.a` (`kernel.o` and `os.a` where named). Fixed addresses `0xffffffffffffcNNN` are in the per-processor `pda_s` page (so the field is at offset `NNN - 0xc000` of `pda_s`; `p_nodepda` is +1024, `p_nodeid` +1066 and `p_cpuid` +1064, matching the 6.5.30 IP35 layout in [interrupts.md](interrupts.md)).

#### Routines

| Routine | Address | Size |
|---|---|---|
| `migr_intr_prologue_handler`, `migr_intr_handler` | `migr_interrupt+0x0`, `+0x8c` | 140, 564 |
| `migr_effthreshold_diff_set`, `migr_pageref_disable` | `numa_hw+0x0`, `+0x40` | 64, 132 |
| `migr_init` | `migr_init+0x0` | 392 |
| `migr_control_init` | `migr_control+0x0` | 3276 |
| `migr_refcnt_supersetbase_get`, `migr_refcnt_init`, `migr_refcnt_get_offsets`, `migr_refcnt_update_counters`, `migr_refcnt_restart` | `migr_refcnt+0x0`, `+0x48`, `+0x368`, `+0x490`, `+0x714` | 72, 624, 296, 644, 476 |
| `migr_start`, `migr_restart`, `migr_restart_clear`, `migr_stop` | `migr_states+0x0`, `+0x344`, `+0x42c`, `+0x514` | 836, 232, 232, 236 |
| `migr_manager` | `migr_manager+0x0` | 1780 |
| `migr_manager_bounce_control`, `migr_periodic_bounce_control` | `migr_bounce+0x4a8`, `+0x0` | 560, 1192 |
| `migr_periodic_unpegging_control` | `migr_unpegging+0x0` | 1012 |
| `migr_fastcheck_migratable`, `migr_check_migratable` | `migr_engine+0x144`, `+0x0` | 148, 324 |
| `migr_migrate_page`, `migr_migrate_frame`, `migr_page_prologue`, `migr_pagemove`, `migr_page_epilogue` | `migr_engine+0xbd8`, `+0x9a4`, `+0x6f4`, `+0x498`, `+0x87c` | 448, 564, 392, 604, 296 |
| `mem_tick`, `mem_tick_init`, `mem_tick_init_node` | `mem_tick+0x4c4`, `+0x448`, `+0x8` | 556, 124, 1088 |
| `nodepda_pfms_alloc` | `pfms+0x0` | 208 |
| `hub_migrintr_init`, `cnodetocpu`, `intr_connect_level`, `per_hub_init` (`kernel.o`) | `0x205d0`, `0xeb44`, `0x1eb04`, `0xe850` (call to `hub_migrintr_init` at `0xea60`) | 112, 192, 792, n/a |
| `clock` (`os.a`, `clock.o`) | `0x398`; the call to `mem_tick` is at `0x910` | 1776 |

#### Hub window: read and clear sequence, acknowledge and re-arm

The window base is the doubleword `0x9200000001fc0000` (`.lit8` of `migr_interrupt`, `numa_hw`, `migr_refcnt`). A node's window is that value plus the node's NASID shifted left 33 (`dsll32 ...,1`), the NASID being `compact_to_nasid_node[cnode]`. In the hub register numbering of [interrupts.md](interrupts.md) the window is hub offset `0x7c0000`, so the accesses below are hub offsets `0x7c0038`, `0x7c0040`, `0x7c0048`. All accesses are 64-bit loads and stores.

| Step | Where | What | Grade |
|---|---|---|---|
| Boot arm of +0x40 | `migr_init+0xa0` calls `migr_effthreshold_diff_set` (`numa_hw+0x0`) for every node, before the counters are initialised and whatever the counter mode | stores `(0x7fff - (uint32)count) \| bit 63` at +0x40, where `count = ctrl.threshold_percent * ctrl.reference / 100` (the two control-record fields in the records table). Standard memory, defaults (50 percent, reference 0x7ff): `0x8000000000007c00`. Premium memory (reference 0x7ffff, count 262143) gives a 64-bit subtraction that goes negative, so the stored value is `0xfffffffffffc8000` | E; the effect of the upper bits on hardware U |
| Boot arm of +0x48 | `migr_refcnt_init+0x1d8` to `+0x208`, only when the counter mode nibble is non-zero | stores `bit 63 \| (uint32)ctrl.overflow_count` at +0x48 (`ctrl+0x10`, see records), then ORs bit 63 into the register again | E |
| Interrupt, in the prologue (interrupt context) | `migr_intr_prologue_handler+0x20` to `+0x60` | reads **+0x38** once (the candidate); clears bit 63 of +0x40 and of +0x48 (read, AND with `0x7fffffffffffffff`, write; disarm); stores the candidate into the executing node's record at `nodepda+0x48` if its bit 63 is set, else stores 0; adds 1 to the 32-bit word at `stats+4` | E |
| Interrupt, in the thread | `migr_intr_handler+0x8c`, loop body `+0x134` to `+0x238`, re-read at `+0x108` to `+0x130` | processes the stored candidate; after each one re-reads **+0x38**; the loop ends when the register's bit 63 is clear or when the new value's low 21 bits equal the previous candidate's low 21 bits (a repeat guard) | E |
| Re-arm | `migr_intr_handler+0x23c` to `+0x290` | reads +0x48 and ORs in bit 63 (`0x8000000000000000`), writes it; then the same for +0x40. Order: +0x48 first, then +0x40 | E |
| Register +0x30 | nowhere in the 6.5.16 IP35 members | the handler touches +0x38, +0x40, +0x48 only. The IP27 build uses +0x30, +0x38, +0x48 with a different meaning; do not carry the IP27 offsets over | E |

What the three registers mean is not in the binaries. Reading +0x38 more than once per interrupt implies that a read removes (or exposes the next) candidate, as the repeat guard also suggests (I). That bit 63 of +0x40 and +0x48 is an arm bit that hardware clears by itself (the software clears it explicitly, so it does not depend on this) is U. There is no write to any interrupt-pending or clear register in the migration path (E); whether the generic dispatch clears the level 3 pending bit before it calls the prologue is not read here (U).

#### Candidate decode and dispatch

The candidate is the 64-bit value read from +0x38 (E for every field position).

| Bits | Use | Where |
|---|---|---|
| 63 | valid; the stored candidate is 0 and the loop ends when it is clear | `migr_interrupt+0x50`, `+0x114` |
| 33 | type: set takes the migration path, clear takes the counter-maintenance path; any other value of the one-bit test calls `cmn_err` with "Invalid intr type" (the test is written so that path cannot be reached) | handler `+0x190`, `+0x1e8` |
| 24 to 31 | requesting node's NASID, 8 bits, converted with `nasid_to_compact_node` (a 128-entry table; an 8-bit NASID above 127 would read past it) | handler `+0x1fc` to `+0x218` |
| 0 to 20 | page field, masked with `0x1fffff` | handler `+0xe0`, `+0x138` |

Page number. The software forms `pfn = (own NASID << 19) \| (candidate & 0x1fffff)`, where own NASID is `compact_to_nasid_node[p_nodeid]` of the **interrupted node** (handler `+0x134` to `+0x13c`). The node of the page therefore is not decoded from the candidate: a candidate always names a page of the memory behind the hub that raised it. `pfn` is a 16 KB frame number: physical address `pfn << 14`, NASID at bit 19 of the frame number (bit 33 of the address), 19 bits of frame within a node (8 GB), 1 GB slots of 2^16 frames (frame bits 16 to 18 select the slot, bits 0 to 15 the frame within the slot) (E for the arithmetic; I that the unit is 16 KB, from the page-descriptor addressing below, which is the same `pfn`). The page field has two bits more than a frame within a node needs; bits 19 and 20 of the field are OR-ed over the low NASID bits, so the code relies on them being 0, or on the hub reporting a different unit (U; see open questions).

State word of the page: `state = ((uint32 *) nodepda[home].ctrl[+0x50][ (pfn >> 16) & 7 ])[ pfn & 0xffff ]`, read without the lock (handler `+0x138` to `+0x1ac`).

| State word at read | Action | Call | Grade |
|---|---|---|---|
| type 0, bit 4 (`0x10`) clear | restart the page's counters | `migr_refcnt_restart(pfn, state, 0)` | E |
| type 0, bit 4 set, bit 17 (`0x20000`) clear | fold counters | `migr_refcnt_update_counters(own cnode, pfn, state, 0x10)` | E |
| type 0, bits 4, 17 and 14 (`0x4000`) set | fold counters | the same, `0x10` | E |
| type 0, bits 4 and 17 set, bit 14 clear | fold counters, keep the preload | the same, `0x18` | E |
| type 1, bit 17 clear | skip; read the next candidate | none | E |
| type 1, bit 17 set | move decision | `migr_manager(own cnode, compact(requester NASID), pfn)` | E |

Reading (inferred): type 1 is a difference-threshold candidate (the +0x40 register, the page is being read remotely more than locally) and type 0 is a value-threshold candidate (the +0x48 register, a counter nearing its limit), because the type 1 path asks for a move and the type 0 path only restarts or folds counters. The binaries do not say which register raises which type.

#### Interrupt wiring

| Item | Fact | Grade |
|---|---|---|
| Who calls | `per_hub_init` (`kernel.o+0xe850`) calls `hub_migrintr_init(cnode)` at `+0xea60`, once per node, right after `hub_rtc_init`. The 6.5.30 IP35 `per_hub_init` has no such call | E |
| Skip | `hub_migrintr_init` calls `cnodetocpu(cnode)` first (`kernel.o+0x205d8`), then returns without connecting anything when `numnodes == 1` (`+0x205f0`) or when the CPU is -1 (`+0x205f8`) | E |
| Target CPU | `cnodetocpu(cnode)` (`kernel.o+0xeb44`) walks the node's CPUs from `node_first_cpu` (halfword at `nodepda+34230`) for `node_num_cpus` (halfword at `+34232`) and returns the first for which `cpu_enabled` is true. So the first **enabled** CPU of each node takes the interrupt, not the boot CPU as such | E |
| Connect call | `intr_connect_level(cpu, 3, 0, migr_intr_handler, 0, migr_intr_prologue_handler)` (`kernel.o+0x20610`). `intr_connect_level` stores arguments 4, 6 and 5 at +0, +8 and +16 of the 72-byte vector record (`iv_func`, `iv_prefunc`, `iv_arg` of [structs/intr_vector_s.md](structs/intr_vector_s.md)) and argument 3 as a halfword at +0x38; a non-zero return prints a message and the node continues without migration | E |
| Level and mode | level 3, the "Migration" entry of the hardwired table, flagged as a threaded handler. So `migr_intr_prologue_handler` runs as the pre-function in interrupt context (it disarms and latches one candidate) and `migr_intr_handler` runs in the interrupt thread (it drains and re-arms) | E for the connect call; the threaded flag is read in the 6.5.30 table ([interrupts.md](interrupts.md)), so I for 6.5.16; I for the order, from the names and the vector layout |
| Hub side | the processor-interface source that raises level 3 and its routing are not in the binaries. A published hub header lists pending bank 0 bit 3 as page migration; that is a hint from a public header, not a reading of a shipped binary or a measurement, and it is the only source held. Settling test: the hub processor-interface register document's pending-bank table, plus a capture of bit 3 of the first pending register (offset 0x98) going high on hardware that counts page references | Unknown here (hint only) |

#### Counter address arithmetic

The per-page hardware word for node `n` of page `pfn`, as built in `migr_refcnt_update_counters` (`migr_refcnt+0x52c` to `+0x5c0`), `migr_refcnt_restart`, `migr_refcnt_read` and `migr_pageref_disable` (E):

```
word(pfn, n) = 0x90000001c0000408                           ; .lit8 + 8, a base
             | ((pfn << 14) & (0x7f << 33))                  ; NASID of the page, bits 33..39
             | ((pfn & 0x7ffff) << 11)                       ; frame in the node, bits 11..29
             | ((compact_to_nasid_node[n] & 0x1e) << 5)      ; node group, bits 6..9
```

The code also ORs in `((pfn << 11) & 0x30)`, which is always 0 for a frame number. Reading the arithmetic: 2 KB of space per 16 KB frame, 16 groups of 64 bytes selected by bits 1 to 4 of the counting node's NASID (so NASIDs 2k and 2k+1 share a group), and the fixed offset `0x408` inside the page's space. The base `0x90000001c0000408` is the protection-entry base of the Bedrock memory/directory datasheet, not the counter base (see [Hardware side](#hardware-side); that discrepancy is unchanged).

Field widths when the word is read (`migr_refcnt+0x5c8` to `+0x5ec`, `migr_refcnt_read` at `+0xa94`): `count = (word >> 5) & mask`, mask `0x7ff` (11 bits) or, when the **counting node's** control record has `byte[0x5c] == 1`, `0x7ffff` (19 bits). The flag comes from the control record of node `n`, not the page's home node (E). It is set in `migr_control_init` from `klhwg_premium_std_dimm_mix_check` (E for the call; I that it means premium memory).

Write form (`migr_refcnt+0x628` to `+0x6dc`, `+0x84c` to `+0x8cc`): the word stored is `((offset << 5) \| mode) \| 0xffff`; for the local node `offset` is `out8` and for every other node it is `outC`, and the mode value (a small constant, 0 locally when the mode is 0x18) is OR-ed in as well. For 11-bit counters `offset << 5` is at most `0xffe0`, so the OR leaves exactly `0xffff` whatever the offsets are; only a premium (19-bit) counter can keep a non-zero offset above bit 15 (E). What the low sixteen bits of this word mean when written is U; the same `0xffff` is what `migr_pageref_disable` stores, which supports reading these as protection-entry bits (I), but then the "preload" reading of the offsets is doubtful: see open questions.

#### Per-page and per-node state

**Per-page state word.** 4 bytes per frame, `nodepda_pfms_alloc` (`pfms+0x0`) builds for each node a 64-byte table of 8 pointers (one per 1 GB slot), each pointing at `slot_getsize(node, slot) * 4` zeroed bytes from `low_mem_alloc`; the table pointer is stored at `ctrl+0x50`. Bit 0 is a lock taken with `bitlock_acquire_32bit` at raised priority level (E). The other fields, as the routines read and write them (E for position and use, I for the names):

| Bits | Mask | Meaning | Set or used by |
|---|---|---|---|
| 0 | `0x1` | lock | every writer; `migr_manager+0x9c`, `migr_states+0x368` |
| 1-3 | `0xe` | tracking state: `0x8` idle and tracked; `0xc` a decision is running; 0 untracked. `migr_manager` refuses unless the field is exactly `0x8` (`+0xac`) and moves it to `0xc` (`+0x11c`); `migr_restart` and `migr_restart_clear` move `0xc` back to `0x8` | `migr_start` sets bit 3 (`migr_states+0x268` to `+0x274`: `(word & ~0xe) \| 8`); `migr_stop` clears the whole word but the lock bit (`migr_states+0x5a8`) |
| 4 | `0x10` | counters are on for this page | `migr_start` (`migr_states+0x10c`), cleared when the policy says off |
| 5 | `0x20` | dampening on | `migr_start` (`migr_states+0x1ec`) |
| 6-7 | `0xc0` | dampening counter | bounce control |
| 8-9 | `0x300` | dampening limit, `(policy value * 3 / 100) << 8` | `migr_start` |
| 10 | `0x400` | enqueue-on-failure allowed | `migr_start` (`migr_states+0x20c`), tested at `migr_manager+0x5e4` |
| 11-13 | `0x3800` | bounce count (0 to 7) | bounce control, cleared when a page is frozen or melted |
| 14 | `0x4000` | frozen (the manager refuses; counters are restarted) | `migr_manager` on refusal; `migr_manager_bounce_control` |
| 15 | `0x8000` | melt on | `migr_start` (`migr_states+0x1a8`) |
| 16 | `0x10000` | freeze on | `migr_start` (`migr_states+0x15c`) |
| 17 | `0x20000` | per-page threshold in use; also required before `migr_manager` is called | `migr_start` (`migr_states+0xa8`) |
| 18-20 | `0x1c0000` | melt threshold, `(policy value * 7 / 100) << 18` | `migr_start` |
| 21-23 | `0xe00000` | freeze threshold, `(policy value * 7 / 100) << 21` | `migr_start` |
| 24-30 | `0x7f000000` | per-page threshold percent (the policy's byte copy) | `migr_start` (`migr_states+0xa4`) |

`migr_start(pfn, object)` (called from the fault, map and swap-in paths) asks the owning object's policy (`vector+0x68`, selector 4, 16-byte result) for the page's settings and builds the word from them; `migr_stop(pfn)` folds the counters (`update_counters(..., 0)`) and clears the word; `pflip_pfms_get` and `pflip_pfms_set` read and write the word for a physical address with a counter restart (`restart(..., 1)`). The policy's own result layout (which result byte feeds which field) is I from the shifts only.

**Per-node control record** (`nodepda+8`, 96 bytes from `low_mem_alloc`, filled by `migr_control_init`; `nodepda_numa_init` in the `numa_init` object calls it for each node). Byte fields written by read-modify-write keep the other bits of their byte.

| Offset | Width | Content (tunable) | Grade |
|---|---|---|---|
| +0 | byte | bits 4-7: migration mode nibble (`numa_migr_default_mode`: 0 gives 0, 1 gives 0x1, 3 gives 0x2, 2 gives 0x3, 4 gives 0x3 below `numa_migr_min_maxradius` of `physmem_maxradius(4)` else 0x2); bits 2-3: freeze enable (2 bits); bits 0-1: melt enable | E |
| +1 | byte | bits 4-7: counter mode nibble (`numa_refcnt_default_mode`, same encoding); bits 2-3: enqueue-on-failure; bits 0-1: dampening enable. `migr_init`, `update_counters`, `restart` and `migr_refcnt_enabled` all test `byte[1] >> 4`; zero turns the counter code into a no-op | E |
| +2 | byte | default threshold percent (`numa_migr_default_threshold`, 50) | E |
| +3 | byte | minimum node distance (`numa_migr_min_distance`) | E |
| +4 | byte | `numa_migr_min_maxradius` | E |
| +5, +6, +7 | bytes | freeze threshold, melt threshold, dampening factor | E |
| +8 | byte | bit 7 `memoryd_enabled`, 6 traffic control, 5 unpegging control, 4 automatic mechanism, 3 user mechanism, 2 coalesce mechanism | E |
| +0xc | word | threshold reference: `0x7ff`, or `0x7ffff` when `numa_migr_threshold_reference` is 1 and the node has the premium flag | E |
| +0x10 | word | counter overflow count: `numa_refcnt_overflow_threshold` percent of the reference | I (arithmetic seen, fallback paths not traced) |
| +0x14, +0x18 | word, byte | traffic-control interval and threshold | E |
| +0x1c, +0x20 | words | bounce-control interval, unpegging interval | E |
| +0x24 | byte | unpegging threshold percent | E |
| +0x25 | byte | queue flags: bit 7 queue control, 6 traffic trigger, 5 capacity trigger, 4 time trigger | E |
| +0x26 to +0x34 | bytes and words | queue maximum length, control interval (word at +0x28), limit, trigger thresholds, time-trigger interval (word at +0x30), maximum page age | E |
| +0x38, +0x39 | bytes | copies of the traffic-control and queue-control intervals, truncated to a byte | E |
| +0x35, +0x36 | bytes | bit 7 of +0x35: destination-memory-low filter on; +0x36: its threshold percent | E |
| +0x37 | byte | migration vehicle (`numa_migr_vehicle`, forced to 1 by `migr_control_init`) | E |
| +0x40, +0x48 | pointers | migration queue (`migr_queue_alloc`); queue entry storage (`maxlen * 24` bytes) | E |
| +0x50 | pointer | state-word tables (above) | E |
| +0x58 | lock | spinlock (`spinlock_init`) | E |
| +0x5c | byte | premium-memory flag | E |

**Software counters.** `migr_refcnt_init(cnode)` (only when the counter nibble is non-zero) sums `slot_getsize` over the 8 slots to get the node's frame count `P`, allocates `numnodes * P * 8` bytes with `kmem_contig_alloc_node` (and a 64-byte table of 8 slot pointers with `kmem_alloc_node`), zeroes the bytes, and stores: `nodepda+0x28` slot-pointer table, `+0x30` base, `+0x38` size, `+0x40` (word) `P`. `migr_refcnt_supersetbase_get(cnode, pfn)` returns `table[(pfn >> 16) & 7] + numnodes * (pfn & 0xffff) * 8`: the 64-bit counter of node `n` for the page is the `n`th doubleword there (E). A page's counters exist (and the hardware words are folded into them) only while bit 4 of its state word is set.

**Per-node scratch.** `nodepda+0x48` (candidate stored by the prologue; `migr_init` zeroes it), `nodepda+0x18` points at the statistics record (of the executing or the named node, as the step says) whose offsets the path updates (inferred names): +4 interrupts, +0x20 + 4*vehicle pages copied per vehicle, +0x28 state not idle, +0x2c page not eligible (bit 17 clear), +0x30 page frozen, +0x34 pages moved in, +0x38 pages moved out, +0x3c refused, +0x88/+0x8c/+0x90 unpegging scans, +0x94 counter restarts after a refusal, +0x98 to +0xa0 bounce-control passes, melts and per-pass work, +0xa4 dampened, +0xc4 destination low on memory, +0xc8 too near, +0xcc not migratable. Offsets E, names I; whether `nstats` prints these is U (see [Unknowns](#unknowns)).

**Fold and restart.** `migr_refcnt_update_counters(cnode, pfn, state, mode)` (E):

1. If the counter nibble is 0 it returns.
2. `migr_refcnt_get_offsets(cnode, pfn, state, ...)` yields a pointer to the page's software counters (non-zero only when bit 4 is set) and three words: `out8`, `outC` and `out10`. With bit 17 clear, or with bits 17 and 14 both set, the offsets are zero and `out10` is `0x10` (0 when bit 4 is clear). With bit 17 set and bit 14 clear the page has its own threshold: `d = (state >> 24) & 0x7f` is compared with the control record's default percent `p`; if `d < p`, `outC = reference * (p - d) / 100` and `out8 = 0`, else `outC = 0` and `out8 = reference * (d - p) / 100`; `out10` is `0x18`. (`out10` is the third value `restart` ORs into the words it writes.)
3. For every node `n`, when the pointer is non-zero: read `count(n)` (above); `sw[n] += count(n) - (n == cnode ? out8 : outC)`. So the software value is the hardware value minus the offset the hardware word was loaded with.
4. Unless `mode == 0x18`, `out8` and `outC` are zeroed; then every node's hardware word is written back as above. So mode `0x10`, `8` and `0` reset the hardware words and `0x18` rewrites them with the offsets. `migr_refcnt_restart(pfn, state, clear)` does the write-back only, and zeroes the software counters first when `clear` is non-zero; it derives the home node from the frame (`pfn >> 19`). Callers: the handler (`clear = 0`), `migr_restart` (0), `migr_restart_clear` (1), `pflip_pfms_set` (1), `migr_start` (1).

The meaning of the preload (whether hardware counts up from it, or what the low bits do) is U.

#### Decision and move

`migr_manager(cnode, requester, pfn)` order of checks, each refusal ending with the lock released (E for the order, I for the names of the filters; each refusal bumps a statistic):

1. Lock the state word (`bitlock_acquire_32bit`, at raised level). Not exactly `0x8` in bits 1-3: stat +0x28, return. Bit 17 clear: stat +0x2c, return. Bit 14 set: stat +0x30, return.
2. Move the field to `0xc`, unlock, fold the counters (`update_counters(own, pfn, state, 8)`, which also resets the hardware words).
3. Destination memory filter (the executing node's control `+0x35` bit 7): `100 * [dest nodepda+0x614] / [dest nodepda+0x620]` is compared with the executing node's `ctrl[+0x36]`; when below it, set the frozen bit, clear the bounce count, `migr_restart`, stats +0x3c, +0xc4 and +0x94, return.
4. `node_distance(own, requester)` below `ctrl[+3]`: the same refusal (stats +0x3c, +0xc8).
5. `migr_fastcheck_migratable(pfn)` non-zero: the same refusal (stats +0x3c, +0xcc). The check (`migr_engine+0x144`) returns non-zero when the descriptor's halfword at +0x16 is non-zero (1), when its halfword at +0x14 exceeds `2 * numcpus + 32` (4), when its flags word has bit 1 (5) or bit 24 (5).
6. When state bits 5 or 16 are set (dampening or freeze on): `migr_manager_bounce_control(state*, pfn, own)`. Dampening: if bit 5 set and the dampening counter is below the limit, bump the counter and refuse (returns 1, stat +0xa4). Otherwise zero the counter. Freeze: if bit 16 set and the bounce count has reached the freeze threshold, set bit 14, clear the count, `migr_restart` and return 2 (stat +0x94); else bump the count (below 7).
7. When the queue control flag (`+0x25` bit 7) and the automatic-mechanism bit (`+8` bit 4) are both set, the request goes to `migr_queue_insert` instead of moving now. Otherwise `migr_migrate_page(pfn, requester, &newpfn)`.
8. Success (0): stats +0x38 and +0x34, `migr_restart_clear` on the new frame's state word. Failure: stat +0x3c; with queue control on and bit 10 set the request is enqueued, otherwise `migr_restart` (the code compares the error against 9 and 2 but both outcomes reach the enqueue).

Defaults of the 6.5.16 `mtune/numa`: migration mode 2 (off, user may enable), counter mode 0 (off), threshold 50 percent, reference 0, vehicle 1, minimum distance 1, memory-low filter on at 20 percent, freeze, melt, dampening, traffic control and queue control off, unpegging on, `mem_tick_enabled` 1. With these `ctrl.byte[1] >> 4` is 0 on every node, so `migr_refcnt_update_counters` and `migr_refcnt_restart` return at their first test and no software counters are allocated (E for the file and the test). The manager is still reachable for a page whose state has bit 17 set.

**The page move.** Chain: `migr_migrate_page` -> `migr_migrate_frame` -> `migr_page_prologue`, `tlbsync` (vehicle 1), `migr_pagemove`, `cache_operation` (vehicle 1), `migr_page_epilogue` (E for order).

| Step | Address | What it does | Grade |
|---|---|---|---|
| Allocate | `migr_engine+0xc0c` to `+0xc20` | `*out = pfn`; `pagealloc_size_node(dest, pfntocachekey(pfn, 0x4000), 0x2000000, 0x4000)`; zero result returns 2. The old descriptor is addressed directly: `0xa800000000000000 + (nasid << 33) + 0x2000000 + (frame in node) * 64` | E |
| Frame | `migr_engine+0x9e8` to `+0x9f8` | control `+0x37` (vehicle) zero returns 7. Calls the prologue; non-zero is returned | E |
| Prologue | `migr_engine+0x6f4` | clears flag bits `0x1c000` of the new descriptor's flags word (+0x10); takes the old descriptor's flag lock (bit 27); flags bit 8 (`0x100`) selects an anonymous object (`anon_hold`, then `anon_pagemigr(obj, old, new)`), flags bit 2 a vnode (`vnode_hold`, `vnode_pagemigr`), neither returns 9; then under the new descriptor's 64-bit lock at +0x30 runs `rmap_scanmap(new, 1, 0)` and `rmap_scanmap(new, 0x200, &newpfn)` (the owning object takes over the page, then every mapping is updated to the new frame) | E for calls and order, I for what ops 1 and 0x200 do |
| TLB sync | `migr_engine+0xae8` to `+0xb14` (vehicle 1 only) | `tlbsync(0, <the 64-byte all-clear cpumask by value>, 2)` before the copy | E for the call, I for the effect |
| Copy | `migr_engine+0x498` (`bte_pbcopy` call at `+0x540`, `bte_poison_copy` at `+0x610`) | vehicle 1: `bte_pbcopy(old, new, 0x4000)` between the two frames' physical addresses (each OR-ed with `0xa800000000000000`); vehicle 0: `bte_poison_copy`, a non-zero result prints "Failed bte_copy"; other vehicle numbers print "Invalid Migration Vehicle". Adds 1 to stat `+0x20 + 4 * vehicle`. For vehicle 1 it then clears flag bit 22 (`0x400000`) of the old descriptor under the flag lock | E |
| Cache | `migr_engine+0xaa0` to `+0xad8` (vehicle 1 only) | `cache_operation(0xa800000000000000 \| (new frame << 14), 0x4000, 0x2400)`. The address is built from the **new** frame, the one the block transfer wrote, which is what makes sense for invalidating stale lines; an earlier sentence of this page says the old page for the IP27 build | E for the argument; I for the purpose |
| Epilogue | `migr_engine+0x87c` | `pagedone(new)`; under the 64-bit lock `rmap_scanmap(new, 2, 0)`; clears descriptor flag bit 23 (`0x800000`); if the halfword reference count at +0x14 is 2 or more it is decremented, otherwise the page is freed (`pagefreesanon` when flag bit 18 is set, else `pagefreeanon_size`) | E |
| Free | `migr_engine+0xccc` to `+0xd00` | on success `pagefree_size(old, 0x4000)` and `*out` = the new frame number; on a frame error the new frame is freed instead | E |

The sequence has trace points `0xead6` to `0xeae0` through `log_tstamp_event`, enabled by bit 8 of the private-page doubleword at `0xffffffffffffc280` (E). After a successful move the new frame's state word and counters are reset by `migr_restart_clear` in the manager (E).

Local TLB removal: `migr_page_unmap_tlb(addr, vaddr)` (`migr_engine+0x1220`) probes the processor's own TLB for the virtual address and calls `unmaptlb` when the entry maps this frame; it is not on the manager's path (no caller among the IP35 members). Cross-processor invalidation is the `tlbsync` above plus whatever `rmap_scanmap` does in the mapping update (U).

#### Time-driven work (freeze, melt, dampening, unpegging)

| Piece | Fact | Grade |
|---|---|---|
| Clock | `clock` (`os.a`, `clock.o`+0x398) decrements the word at `0xffffffffffffc45c` (`pda_s+0x45c`) when the halfword at `0xffffffffffffc454` is non-zero and calls `mem_tick(p_cpuid)` at `+0x910` when it reaches zero, reloading it from `0xffffffffffffc458` | E |
| `mem_tick` | takes the CPU's `pda_s` through `pdaindr`; does nothing unless `pda+0x764` is non-zero; counts ticks in `pda+0x760`. On every tick it runs `migr_periodic_bounce_control(cnode, resume = pda+0x794, budget)` when freeze or dampening is on (control `+0` bits 2-3 or `+1` bits 0-1) and stores the returned frame number back in `pda+0x794`; likewise `migr_periodic_unpegging_control(cnode, pda+0x7a0, budget)` when control `+8` bit 5 is on. When the tick count is a multiple of the bounce (or unpegging) interval the resume frame is reset to `pda+0x768` and the budget is `pda+0x764 - pda+0x790 * (interval - 1)`; otherwise it is `pda+0x790` (`pda+0x79c` for unpegging). So one pass over the node's frames is spread over `interval` ticks. It also keeps min, max, last and mean tick durations in `pda+0x770` to `+0x788`. `memoryd_init` and the queue's process routine are 8-byte stubs | E for the calls and fields; I for the meaning of the budget fields |
| Registration | `numa_init`'s init table lists `migr_init` and, last, `mem_tick_init`, which initialises one bit lock per node and calls `mem_tick_init_node` | E |
| Bounce pass | `migr_periodic_bounce_control` walks frames of the node from the saved frame, up to the budget, using `pfdat_probe` to jump over holes; for each frame with state field `0x8` and bit 17 set it locks the word and: for a frozen page, melts it (clears bit 14 and the count, folds the counters with mode `0x18`) when melt is on and the count has passed the melt threshold, else adds 1 to the count (up to 7); for an unfrozen page it decays the bounce count and the dampening counter by 1 | E for the structure; I for the names |
| Unpegging pass | `migr_periodic_unpegging_control` walks frames the same way; for each page with state field `0x8`, bit 17 set and bit 14 clear it reads the page's hardware count for every node and, if any exceeds `ctrl[+0x24] * reference / 100`, calls `update_counters(cnode, pfn, state, 0x18)` (statistics +0x8c, +0x90). Purpose: fold counters before they saturate | E for the walk and threshold; I for the purpose |
| Queue | the queue (`ctrl+0x40`) is used only when `+0x25` bit 7 is set, which no shipped default does; `migr_queue_migrate` and `migr_periodic_queue_control` exist but have no caller among the IP35 members (the 8-byte process stub is what both call) | E for the stub and the default; the queue's behaviour was not read |

#### Corrections to the summary above

- The [IP35 row of the IP27 table](#what-the-working-ip27-path-consumed) says the handler "takes the node from the bits above" the page number. It does not: the page's node is the interrupted node's own NASID; the bits above the 21-bit page field carry the requesting node (bits 24 to 31) and the type (bit 33). Read at `migr_interrupt+0x134` to `+0x13c` and `+0x1fc` to `+0x218` (grade E).
- The Page move row says `cache_operation` flushes the old page's cache lines (read for the 6.5.30 IP27 build). In the IP35 6.5.16 build it is called with the new frame. The IP27 build was not re-read for this; its `migr_migrate_frame` has the same shape.

#### Open questions

These need a running kernel, real silicon or a register document; none is guessed above. They are rows of the page-migration table in [open-questions.md](open-questions.md): the hub's pending-bit and read semantics for the three registers, the unit and top bits of the candidate's page field and the requester field above NASID 127, the meaning of the `0xffff` low bits of the counter word and of the negative threshold for premium memory, and (software-now) the policy result layout `migr_start` consumes and the callee bodies of the page move.

## Unknowns

- The page-migration mode and enable state of the hub on an Origin 300: the kernel never writes `MD_MIG_CONFIG` (offset 0x1f80018 by the vendor header) or the candidate registers, and the only register it arms is the count-difference threshold. Whether the hardware raises the migration interrupt, and what the power-on state of the mode control is, is unknown. The IP35 PROM was searched for stores to those offsets with two coarse patterns (a `lui` of the window half followed by an offset, and a base-register copy) and found none; that is not a full read. Next route: the held PROM disassembly, and a Bedrock register document (unknown whether any exists outside the vendor header).
- What the 0xffff store of `migr_pageref_disable` does to a protection entry on either platform. The function has no caller, so no shipped software depends on it.
- Whether the node-record bytes 2 and 12 that `migr_init` multiplies are ever non-zero before it runs. The record is zeroed by its allocator and `migr_control_init` does not set them, so the boot-time threshold reads as 0x7fff by the order of the initialisers; the boot was not run.
- Whether `madvise` reaches `pas_setrangeattr` (the call is through the address-space operations vector) and how `pm_create` chooses a default migration policy name when the caller gives none.
- Whether any IP35 routine increments the migration statistics that `nstats` prints.
- Whether the system the shipped kernel was linked on had the optional subsystem `eoe.sw.partition` installed; a site that installs it gets the real objects through the `USE:` lines above (the reading rests on the `lboot` strings; it was not disassembled). Automatic page migration is a different case: the IP35 `numa.a` members that would hold the migration manager, state machine, reference counters, interrupt handler, queue, bounce control and unpegging are 1204 to 1252 byte objects with no code in 6.5.30 (IP27's are 6 to 15 KB) because they were emptied after 6.5.16, and `mem_tick.o` defines only `mem_tick_init`; see the [release history](#release-history-of-migration-and-replication-65-8-to-6530) and the [verdict](#verdict).
- Why the IP35 migration code was withdrawn in 6.5.17 to 6.5.21, and whether it ever ran on an Origin 3000. No held release note, manual page, tunable comment or error string says; the objects show it was shipped for six releases. Next route: a Bedrock register document, or any held IP35 PROM or diagnostic that touches the migration registers.
- Whether the IP35 counter reader of 6.5.8 to 6.5.19, which builds its address from the protection-entry base `0x90000001c0000408` where the memory/directory datasheet puts the page counter at the backdoor `+8`, ever read live counters.
- The page-descriptor flags and fields that marked a replica page in 6.5.22 and earlier, and the exact hardware meaning of the IP27 second threshold register at offset 0x38.
- The four hardware-graph path strings' order for the `cl` devices, and the record layouts copied by `part_operation`.
