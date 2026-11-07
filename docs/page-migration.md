# Page migration and cross-partition communication

This page answers two questions about the IRIX 6.5.30 kernels, IP35 first (Origin 300/350, Onyx 300/350) with the IP27 (Origin 200/2000, Onyx2) differences noted: whether the kernel moves pages between nodes by itself, and whether it has a facility for talking to, or sharing memory with, another partition. Evidence is the linked kernels `6.5.30/unix.IP35` and `6.5.30/unix.IP27`, the kernel objects under `6.5.30/usr/cpu/sysgen/IP35boot/` and `IP27boot/`, the interrupt table already read in [interrupts.md](interrupts.md), and the selector pages [syssgi.md](syssgi.md) and [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md). The 6.5.22 configuration tree (`root/6.5.22/var/sysgen.IP35` and `sysgen.IP27`) is used for the tunable comments and the module list; the 6.5.30 configuration tree is not held, so a statement taken from it is about 6.5.22 only ([sysgen.md](sysgen.md)).

Grades follow [AGENTS.md](../AGENTS.md): established is read in full in a binary at the address given, inferred states its reasoning, and anything unlabelled or listed under [Unknowns](#unknowns) is unknown. No kernel or library code was run.

## Answers

| Question | Answer | Grade | Where |
|---|---|---|---|
| Does automatic (hardware-counter driven) page migration exist in IP35 6.5.30? | Not linked in the shipped 6.5.30 IP35 kernel. The policy and control scaffolding is linked and initialised, but the reference-counter readers, the migration interrupt handler, the periodic tick and the migration manager that IP27 has are not linked, and the tick initialiser is an 8-byte routine that returns success. | Established for the routines not linked and for the 8-byte routine; inferred for "nothing else drives it". | [Automatic page migration on IP35](#automatic-page-migration-on-ip35) |
| What migration does IP35 keep? | Migration on request: a placement-module control call, address-space moves when a process joins or changes cpuset, and the large-page coalescing thread. Poisoned-page handling consults the migration code (it asks whether a page is being moved and unmaps it from the TLBs). | Established (call edges); roles inferred. | [What still moves pages on IP35](#what-still-moves-pages-on-ip35) |
| Policy interfaces | `syssgi` 208 (placement-module control, commands 40 to 42 for migration), 212 (tunable block read and write), 218 (statistics); three migration policy modules registered by name; a hub-special `refcnt` device whose handlers return an error on IP35; no `/proc` migration control found on IP35. | Established. | [Policy interfaces](#policy-interfaces) |
| Hardware counters read | None read by any IP35 routine found; IP27 reads three registers per node through the hub window. Register names are not in either binary. | Established (no IP35 routine linked reads them); register meaning unknown. | [Hardware reference counters](#hardware-reference-counters) |
| Daemon or thread | IP35: the large-page coalescing thread `coalesced` is the only thread that moves pages without a user call. IP27: the migration interrupt handler and the clock tick. | Established (call edges); inferred (roles). | [What drives migration](#what-drives-migration) |
| Tunables | 47 `numa_*`, `mem_tick_*`, `memoryd_enabled` and `coalesced_*` names with defaults; of the 41 migration-related ones IP35 reads three. | Established (defaults and read sites). | [Tunables](#tunables) |
| Cross-partition facility? | Yes in the shipped objects: a partition layer, a message layer (XPC) and a shared-memory driver (XPMEM), plus a character device and a network interface over XPC. The linked IP35 kernel contains XPMEM, which initialises as partition 0 and works locally, but only panicking stubs for XPC and the partition layer, so nothing can reach another partition in it. IP27 has no XPMEM and the same stubs. | Established. | [Cross-partition communication on IP35](#cross-partition-communication-on-ip35) |
| Messages only, or shared memory? | Both: XPC carries messages (control, data pulled with the block-transfer engine, inter-partition interrupts); XPMEM maps another partition's pages into a process and also copies with the block-transfer engine. | Established (call edges); inferred (end-to-end behaviour). | [XPMEM](#xpmem) |

## Automatic page migration on IP35

On IP27 the kernel moves pages on its own: a hub interrupt (level 3, "Migration") fires when a remote node's per-page access count exceeds the local one by a threshold, a handler reads the counters and calls the migration manager, and a clock tick drives periodic filters. None of those pieces exists in the IP35 kernel. The 6.5.22 `mtune` file for both machines (`root/6.5.22/var/sysgen.IP35/mtune/numa`) has the heading "Migration Parameters (Origin 2000 only)" over the migration parameters and "Extended Reference Counter Parameters (Origin 200 and Origin 2000 only)" over the extended reference-counter parameters, and its threshold comment describes the interrupt-on-difference rule; that is the vendor's statement of intent, and the routine evidence is below.

- `numa_init` (`6.5.30/unix.IP35`, address 0xc0000000003272f0, 96 bytes) walks the table `numa_init_table` (0xc0000000015e95c8, 19 entries of an init routine pointer and a name pointer, then a zero), calls each routine with a zero argument, and on a non-zero result calls `cmn_err` with the module name and the result (call at 0xc000000000327340). The names in order are Mld, Raff, MldSet, PmoList, Topology, Migr, six "Placement Policy" modules, two "Fallback Policy" modules, "Migration Policy: Default", "Migration Policy: Control", "Migration Policy: Refcnt", MemTickInit and AssignWards. The migration entries are `migr_init`, `migr_policy_default_init`, `migr_policy_control_init`, `migr_policy_refcnt_init` and `mem_tick_init`. Established.
- `mem_tick_init` (`6.5.30/unix.IP35`, address 0xc0000000003309d4, 8 bytes) is `jr ra` with `move v0,zero` in the delay slot: it does nothing and reports success. The IP27 routine of the same name (0xc00000000027a840, 124 bytes) calls `mem_tick_init_node`, which reads `numa_migr_default_mode` and `mem_tick_enabled`; the call is at 0xc00000000027a884 inside `mem_tick_init`, and `mem_tick_init_node` itself is at 0xc00000000027a424. Established.
- `migr_init` (`6.5.30/unix.IP35`, address 0xc00000000032b504, 176 bytes) loops over `numnodes`, calls `migr_effthreshold_diff_set` (0xc00000000032b5b4, 64 bytes) with a value computed from a per-node record, and clears the doubleword at offset 40 of that record. It does not call the IP27 `migr_refcnt_init`. Established.
- `migr_effthreshold_diff_set` (`6.5.30/unix.IP35`, address 0xc00000000032b5b4, 64 bytes) takes a node number and a count and stores the doubleword (0x7fff minus the low 32 bits of the count) with bit 63 set at offset 0x40 of a window whose base word is 0x9200000001fc0000 (the pool word at gp+2376 holds that value in the kernel data) plus a halfword looked up for the node, shifted left 33 (`dsll32` by 1 at 0xc00000000032b5dc). `migr_init` computes the count as the byte at +2 of a per-node structure times the word at +0xc, divided by 100. The IP27 routine of the same name (0xc00000000027d6f0, 184 bytes) stores at offset 0x30 of the 0x9200000001a00000 window with the node halfword shifted left 32 (`dsll32` by 0 at 0xc00000000027d750 and 0xc00000000027d790), a one-bit difference in the node-window shift from IP35 whose reason is unknown. It has two paths: an earlier one (0xc00000000027d6f0 to 0xc00000000027d760) that stores a value held in a pool word (gp-0x5680) after tests on the node record that were not decoded, and one (from 0xc00000000027d764) that stores 0xfffff minus the count with bit 63 set. Which hardware register either store reaches is unknown; the width difference (a 15-bit field on IP35, a 20-bit field on IP27) is read from the two constants. Established for the stores; the register meaning is unknown.
- `migr_control_init` (`6.5.30/unix.IP35`, address 0xc00000000032dfe8, 600 bytes) allocates a 0x50-byte record per node with `low_mem_alloc`, reads only `numa_migr_default_mode` and `numa_migr_min_maxradius`, and stores 1 to `numa_migr_vehicle` (0xc00000000032e148). It clears the top nibble of the first byte of the record and then sets bits 0x10 for mode 1, 0x20 for mode 3, 0x30 for mode 2 and, for mode 4, 0x30 when `physmem_maxradius` is below `numa_migr_min_maxradius` and 0x20 otherwise; mode 0 leaves the nibble clear; any other mode calls `cmn_err` with the message "Invalid Default Migration Mode, Disabling migration" and also leaves it clear. It initialises a spin lock at offset 0x48 of the record, and also clears the top nibble of byte 1 and stores to offsets 0x37, 0x4c and 0x4d of it. The IP27 routine (0xc000000000281558, 3176 bytes) also reads the threshold, mechanism, filter, queue and bounce tunables; the IP35 routine reads none of them. Established.
- `per_hub_init` (`6.5.30/unix.IP35`, address 0xc00000000004e31c, 612 bytes) calls `intr_reserve_hardwired`, `hub_error_init`, `install_eccintr`, `protect_hub_calias` and `hub_rtc_init`, and no migration routine. Established.
- `per_hub_init` (`6.5.30/unix.IP27`, address 0xc000000000026acc, 1000 bytes) makes the same five calls, plus `hub_setup_kick` and `hub_init_ifdr_war`, and, after `hub_rtc_init`, calls `hub_migrintr_init` (call at 0xc000000000026de0), which connects interrupt level 3 to `migr_intr_handler`. Established.

The shipped 6.5.30 IP35 kernel does not link `hub_migrintr_init`, `migr_intr_handler`, `migr_manager`, `migr_start`, the `migr_refcnt_*` family or `mem_tick`: none of those names is in the IP35 symbol table or its debug information ([inventory/functions.tsv](inventory/functions.tsv)). Established.

The hardwired interrupt table of the IP35 kernel (`hardwired_intr`, 0xc000000001531598, 416 bytes) still reserves level 3 with the name "Migration" and the threaded-handler flag 0x8, as on IP27 ([interrupts.md](interrupts.md)). The reservation alone does not connect a handler; the connect call is the one IP35 lacks. Inferred from the missing routine and the unchanged table; whether another routine connects a handler to level 3 was not searched kernel-wide.

The IP35 migration tunables in the 6.5.22 `mtune` list carry the same headings as IP27's ("Origin 2000 only" over the migration parameters, "Origin 200 and Origin 2000 only" over the extended reference counters), because the file is shared. The data words in the linked IP35 kernel carry the same defaults as in IP27 ([Tunables](#tunables)), and the symbolic pass finds no read of most of them in IP35. That a tunable has no resolved read site is not proof that nothing reads it; the pass resolves only accesses it can follow ([sysgen.md](sysgen.md)).

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

Names that look like migration and are not page migration: `migrate_timeouts` (0xc000000000068a2c, 604 bytes; callers `cpuset_init`, `failsoft_avoid` and twice `sysmp`) and `kthread_afflink_migrate` move timeout entries and thread affinity links when a processor goes away or a thread changes cpu. They are listed in the inventory with the same prefix. Inferred from the callers.

## Policy interfaces

- `pmoctl` (`6.5.30/unix.IP35`, address 0xc00000000032f558, 1064 bytes) is `syssgi` selector 208, described in [syssgi.md](syssgi.md). Its jump table (0xc0000000004d47a8, 103 doublewords) sends the three migration commands to the same callees as IP27: the entries at index 40, 41 and 42 hold the case addresses 0xc00000000032f6f0, 0xc00000000032f6d8 and 0xc00000000032f6c0, which call `migr_xface_migrate`, `pmo_xface_as_migrate` and `pmo_xface_as_migrate_close` respectively. So migration on request is placement-module control commands 40 (range migrate), 41 (address-space migrate) and 42 (close of the address-space migrate). Established.
- `numa_tune` (`6.5.30/unix.IP35`, address 0xc000000000330cf8, 836 bytes) is selector 212. It reads and writes a per-node block of tunables (commands 100 and 101); the byte layout is partly traced in [syssgi.md](syssgi.md). Command 101 validates the block with `migr_parms_check` (calls at 0xc000000000330eb0 and 0xc000000000330ed4; the routine is at 0xc0000000003309dc, 796 bytes, against 828 bytes on IP27) and copies it. Unlike IP27 it does not quiesce or restart the memory tick. Established.
- `numa_stats_get` (`6.5.30/unix.IP35`, address 0xc00000000031c3c0, 192 bytes) is selector 218 and returns the per-node statistics block; `numa_stats_init` (0xc00000000031c37c, 68 bytes) is called from `nodepda_numa_init` (0xc000000000327350, 52 bytes). Established.
- `nodepda_numa_init` (`6.5.30/unix.IP35`, address 0xc000000000327350, 52 bytes) calls `numa_stats_init` (call at 0xc000000000327360) and `migr_control_init` (call at 0xc00000000032736c). Established.
- `migr_policy_default_init` (`6.5.30/unix.IP35`, address 0xc000000000330848, 44 bytes) registers a policy factory by calling `pmfactory_export` (0xc000000000326fc0, 32 bytes; call at 0xc00000000033085c) with the module's create routine and its name. The Control and Refcnt initialisers (0xc00000000032e240 and 0xc0000000003301ec, both 44 bytes) do the same. The names the three `*_name` routines return are the strings "MigrationDefault", "MigrationControl" and "MigrationRefcnt". The policy modules are therefore selectable by those names through the placement-module interface (pm creation, command 20 and 29 of `pmoctl`); the selection path itself was not traced. Established for the registration and names; inferred for the selection.
- `migr_policy_default_migr_srvc` (`6.5.30/unix.IP35`, address 0xc000000000330928, 172 bytes) is a service dispatcher for service codes 0, 1, 2, 4 and 6; any other code reaches `cmn_err` with a message that names the policy and says "Invalid service type". The Control (0xc00000000032e2b0, 172 bytes) and Refcnt (0xc000000000330294, 172 bytes) dispatchers have the same shape. What the codes mean is not read. Established for the code set; meanings unknown.
- `mem_refcnt_attach` (`6.5.30/unix.IP35`, address 0xc000000000329c60, 52 bytes) adds a character device named "refcnt" to the hardware graph under the vertex it is given, using the prefix "hubspc_" (call to `hwgraph_char_device_add` at 0xc000000000329c70), then calls `device_info_set` with 0 (call at 0xc000000000329c7c). `hubspc_init` hands it to `hubdev_register`, and the hub special-device entry points forward device-information value 0 to the `mem_refcnt_*` routines ([ip35-hub-errors.md](ip35-hub-errors.md)). Established.
- `mem_refcnt_open` (`6.5.30/unix.IP35`, address 0xc000000000329c94, 8 bytes) returns 19 without a call. `mem_refcnt_mmap` (`6.5.30/unix.IP35`, address 0xc000000000329ca4, 8 bytes) and `mem_refcnt_ioctl` (`6.5.30/unix.IP35`, address 0xc000000000329cb4, 8 bytes) are the same two instructions. Established.
- `mem_refcnt_close` (`6.5.30/unix.IP35`, address 0xc000000000329c9c, 8 bytes) returns 0 without a call, as does `mem_refcnt_unmap` (`6.5.30/unix.IP35`, address 0xc000000000329cac, 8 bytes). So the "refcnt" node exists on IP35 but cannot be opened, mapped or controlled. Established.
- `mem_refcnt_attach` (`6.5.30/unix.IP27`, address 0xc00000000027a0c4, 52 bytes) is the same routine as the IP35 one. Established.
- `mem_refcnt_open` (`6.5.30/unix.IP27`, address 0xc00000000027a0f8, 108 bytes) checks privilege with `cap_able` (call at 0xc00000000027a104) and resolves the node with `master_node_get` (call at 0xc00000000027a114). Established.
- `mem_refcnt_mmap` (`6.5.30/unix.IP27`, address 0xc00000000027a16c, 156 bytes) resolves the node with `master_node_get` (call at 0xc00000000027a184) and maps physical memory into the caller with `v_mapphys` (call at 0xc00000000027a1f8). The mapped range is inferred to be the node's counter memory. Established for the calls.
- `mem_refcnt_ioctl` (`6.5.30/unix.IP27`, address 0xc00000000027a210, 532 bytes) takes commands 1 and 2 and returns 22 for any other. It resolves the node with `master_node_get` (call at 0xc00000000027a224), sizes slots with `slot_getsize` (call at 0xc00000000027a2a8) and copies records out with `copyout` (calls at 0xc00000000027a2d0 and 0xc00000000027a410). The record contents are unknown. Established for the structure.
- `prioctl_piocgetsn0refcntrs` (`6.5.30/unix.IP27`, address 0xc00000000035afc4, 596 bytes) and `prioctl_piocgetsn0extrefcntrs` (`6.5.30/unix.IP27`, address 0xc00000000035b218, 596 bytes) are `/proc` ioctl handlers that return reference-counter data for a process; the names are the evidence, the bodies were not read. IP35 has no routine with these names. Inferred.

The only user programs in `root/6.5.30` that touch NUMA state are `numastatd` and the `hinv` family. `numastatd` (`6.5.30/usr/sbin/numastatd`) samples `/proc/pinfo` and router monitor nodes and calls `syssgi`; it is a statistics sampler, not a migration controller ([userland.md](userland.md)). The vendor's own man pages `numa(5)`, `migration(5)` and `mmci(5)` are named in the `mtune` comments as the reference for these parameters; they were not read here, and the published manuals are the next route for user-level semantics (see [Unknowns](#unknowns)).

## Hardware reference counters

On IP27 the migration code reads and re-arms per-node counter registers in the hub window.

- `migr_intr_handler` (`6.5.30/unix.IP27`, address 0xc00000000026c6c0, 564 bytes) runs from the level 3 interrupt. For the interrupted processor's node it reads the doubleword at offset 0x48 of that node's hub window (the base word 0x9200000001a00000 from gp-24440 plus the node's window selector shifted left 32), calls `migr_refcnt_update_counters` (0xc00000000026cdec, 1016 bytes; calls at 0xc00000000026c730 and 0xc00000000026c80c with 0x10 and 0x18 as the last argument), calls `migr_manager` (0xc000000000280294, 1780 bytes; call at 0xc00000000026c848) and `migr_refcnt_restart` (0xc00000000026d1e4, 792 bytes; call at 0xc00000000026c860), and writes offsets 0x30 and 0x38 of the same window with bit 63 set before it returns. Established for the offsets and calls.
- `migr_refcnt_init` (`6.5.30/unix.IP27`, address 0xc00000000026c940, 724 bytes) allocates the software extended-counter arrays (`kmem_alloc_node`, `kmem_contig_alloc_node` and `slot_getsize` per node) and stores a value with bit 63 set at offset 0x38 of each node's window (store at 0xc00000000026cb70). Established.
- `hub_migrintr_init` (`6.5.30/unix.IP27`, address 0xc000000000039bac, 108 bytes) skips single-node systems, then calls `intr_connect_level` (0xc0000000000383ec, call at 0xc000000000039be8) for interrupt level 3 with `migr_intr_handler` (0xc00000000026c6c0) and `migr_intr_prologue_handler` (0xc00000000026c634) as handler and prologue. Established.

What the registers at offsets 0x30, 0x38 and 0x48 of that window are called in the hub specification, and how the threshold and the "overflow into software counters" rule in the `mtune` comments map onto their fields, is unknown here: no name or field list is in either binary. The vendor comment for the extended counters says the hardware counter overflow value is moved to software counters; the counter widths it quotes (11-bit for ordinary memory, 19-bit for premium) are the vendor's wording, taken from the 6.5.22 `mtune/numa` file, and are not checked against a register specification.

No routine linked into the shipped 6.5.30 IP35 kernel reads a reference counter; the routines above are the whole family on IP27 and none is linked on IP35. Whether some other IP35 routine, for example in the error path, reads the same hub offsets was not searched across the whole kernel. Inferred.

## What drives migration

- `coalesced` (`6.5.30/unix.IP35`, address 0xc00000000027a8f0, 252 bytes) is the large-page coalescing thread. It loops over `coalesce_engine` (calls at 0xc00000000027a934, 0xc00000000027a968, 0xc00000000027a998 and 0xc00000000027a9dc), reads `coalesced_precoalesce` (load at 0xc00000000027a958) and sleeps with `coalesce_daemon_sleep` (call at 0xc00000000027a9b4). Its data words are `coalesced_pri` 90, `coalesced_run_period` 300 and `coalesced_precoalesce` 0 in the IP35 kernel. This is the only IP35 thread found that moves pages without a user call. Established for the loop; the thread role and what `coalesced_run_period` counts are inferred.
- `coalesced_kick` (`6.5.30/unix.IP35`, address 0xc00000000027b268, 188 bytes) calls `coalesce_daemon_wakeup` (call at 0xc00000000027b308) to wake the thread. Established.
- `second_thread` (`6.5.30/unix.IP35`, address 0xc00000000026084c, 528 bytes) calls `coalesced_kick` (call at 0xc000000000260a4c). Established.
- `mem_tick` (`6.5.30/unix.IP27`, address 0xc00000000027a8bc, 556 bytes) is the periodic memory tick; the role is inferred from the name and the `mtune` comments. Inferred.
- `clock` (`6.5.30/unix.IP27`, address 0xc0000000001b22d4, 2520 bytes) calls `mem_tick` (call at 0xc0000000001b28c8). Established.
- `mem_tick_init_node` (`6.5.30/unix.IP27`, address 0xc00000000027a424, 1052 bytes) sets a node up for the tick; it reads `numa_migr_default_mode` and `mem_tick_enabled`. Inferred.

IP27 also has the migration interrupt ([Hardware reference counters](#hardware-reference-counters)). `migr_start` (0xc00000000027bd24, 836 bytes) is called from 17 sites in page-fault, copy, swap-in and mapping paths (four in `pas_vfault`, two each in `pas_pfault` and `unswap`, and one each in `pas_pflip`, `pas_addstackexec`, `pas_addshm`, `lpage_swapin`, `copy_pmap`, `copy_locked_page`, `rmap_addmap_nolock`, `segcommon_spt_modify` and `prusrio`), and `migr_manager` is reached from the interrupt handler. The periodic filters (`migr_periodic_unpegging_control`, `migr_periodic_bounce_control`, `migr_periodic_queue_control`) and the queue (`migr_queue_insert`) exist only on IP27. Established for the call edges (a scan of every `jal` in the IP27 text); the filter roles come from their names and the `mtune` comments.

No memory daemon named `memoryd` was found on either machine: the tunable `memoryd_enabled` is read by `migr_control_init` on IP27 only and has no resolved read on IP35, and neither kernel's function inventory has a routine with that name. Inferred.

## Tunables

Defaults below are the 4-byte data words of the linked kernels at their tunable symbols (equal in `unix.IP35` and `unix.IP27`; 6.5.22 `mtune` agrees). An access site is a routine where the symbolic pass of [sysgen.md](sysgen.md) resolved a load or store of the word (`numa_migr_vehicle` in `migr_control_init` is a store), from [inventory/tunable-summary.tsv](inventory/tunable-summary.tsv).

| Tunable | Default in the IP35 kernel | IP35 access sites | IP27 access sites |
|---|---:|---|---|
| `coalesced_run_period` | 300 | _lpage_watermarks_sanity, lpage_init | _lpage_watermarks_sanity, lpage_init |
| `coalesced_precoalesce` | 0 | coalesced | coalesced |
| `coalesced_pri` | 90 | main | main |
| `numa_migr_default_mode` | 2 | migr_control_init | mem_tick_init_node, migr_control_init |
| `numa_migr_default_threshold` | 50 | none | migr_control_init |
| `numa_migr_threshold_reference` | 0 | none | migr_control_init |
| `numa_migr_vehicle` | 1 | migr_control_init, migr_pagemove | migr_control_init, migr_pagemove |
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
| `numa_zone_radius` | 1 | none | none |
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

Meaning of the values, from the comments of the 6.5.22 `mtune/numa` file (the vendor's wording, graded as a hint for the binaries above): `numa_migr_default_mode` 0 disables migration and overrides users, 1 enables it and overrides users, 2 and 3 leave the default off or on and let users override, and 4 chooses between those by the machine's maximum router radius against `numa_migr_min_maxradius`. The IP35 routine's bit pattern for the modes (0x10 for 1, 0x20 for 3, 0x30 for 2) is consistent with that list. `numa_migr_vehicle` carries the note "changing this has no effect". The threshold comment gives the rule: a request fires when the difference between a remote and the local counter reaches the default threshold as a percentage of the reference set by `numa_migr_threshold_reference`. Of the tunables, only `numa_migr_default_mode` and `numa_migr_min_maxradius` are used to build the IP35 control record.

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

The modules `xpc.o`, `xpc_part.o`, `partition.o`, `partition_part.o`, `cl_part.o` and `if_cl_part.o` are shipped in `6.5.30/usr/cpu/sysgen/IP35boot/` but their implementations are not linked into the shipped 6.5.30 `unix.IP35`: `part_activate`, `cl_open` and `ifcl_output` have no symbol there, and the `xpc_*` and `part_*` names that do exist (for example `xpc_connect`, 8 bytes at 0xc0000000004962f0) are the short routines described next. `xpc_part.o` and `partition_part.o` are byte-identical copies of `xpc.o` and `partition.o` (same hash); `IP27boot` has `xpc.o` and `partition.o` but no `xpc_part.o`, `cl_part.o`, `if_cl_part.o` or `xpmem.o`, and the IP30 and IP32 directories have only the stub archives. Established (file listing and function inventory).

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

## Unknowns

- The names and fields of the hub registers at offsets 0x30, 0x38 and 0x48 that IP27 uses for migration counting, and whether the IP35 store in `migr_effthreshold_diff_set` reaches an equivalent register. No names appear in the binaries; the next route is the published hub documentation and the chips repository's Bedrock notes.
- Whether any IP35 code outside `numa_init` connects a handler to hardwired level 3 ("Migration"), and whether any IP35 routine reads the IP27 counter offsets. The kernel-wide scan for accesses was not done.
- Which IP35 routines read the mode bits that `migr_control_init` stores at offset 0 of the per-node record, and whether `coalesce_leaf_chunk` or `migr_xface_migrate` consults them.
- What the service codes of the three `migr_policy_*_migr_srvc` dispatchers mean.
- The 6.5.30 system configuration, so which of `partition`, `xpc`, `cl`, `if_cl` and `xpmem` the shipped kernel is meant to be linked from, and how a site selects the real partition objects instead of the stubs. The 6.5.22 IP35 system file lists all of them as used (and also lists the stub modules), which the 6.5.30 kernel image contradicts for `partition` and `xpc`; the stub-and-real ordering rule is not modelled.
- The four hardware-graph path strings' order for the `cl` devices, and the record layouts copied by `part_operation`.
- The vendor user documentation for the migration controls: `numa(5)`, `migration(5)` and `mmci(5)`, and the `pmoctl` user library, none held.
