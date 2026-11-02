# Hardware graph construction from KLCONFIG

The 6.5.30 IP27 kernel turns each node's KLCONFIG board list ([klconfig.md](klconfig.md)) into vertices of the hardware graph: one vertex per node board under a path built from the module and slot, with `memory`, `hub`, `cpu` and `cpubus` vertices below it. This page states which paths are created, from which record fields, which per-node fields are filled on the way, and what happens when a step fails. It covers the routines named `klhwg_*` and the helpers they call; the generic graph routines (`hwgraph_path_add`, `hwgraph_edge_add`, `hwgraph_info_add_LBL`) are named as callees only.

Grades follow [klconfig.md](klconfig.md): "Established" means the whole routine was read at the named address. Strings are quoted from the kernel's read-only data at the address given (each routine reaches them through one string-block address loaded from gp; the offsets into that block are in the entries). Per-node and per-processor fields are named from the kernel's own DWARF (`nodepda_s`, `pda_s`).

Two tables the routines use:

- the per-node data pointer for compact node c is the doubleword at index c of the table that `p_nodepda` (absolute `0xffffffffffffc400`) points through, as described for `get_xbow_peer` in [klconfig.md](klconfig.md); written below as "the node's `nodepda_s`";
- the per-processor data for processor number n is the doubleword at +8 of entry n of `pdaindr` (`0xc0000000014d5020`, 16-byte entries, address from gp-19672); written below as "the processor's `pda_s`".

## Path names

`board_to_path` names every board vertex as `<module>/slot/<slot name>/<kind>`, through `sprintf` with the format "%H/slot/%s/%s" (the kernel's module-number conversion is `%H`). The kind comes from the board class, the high four bits of the type byte at +5:

| Class | Type | Kind string |
|---|---|---|
| `0x10` | any | `node` |
| `0x20` or `0x80` | any | `io` |
| `0x30` | 51 (`0x33`) | `metarouter` |
| `0x30` | 52 (`0x34`) | `repeaterrouter` |
| `0x30` | other | `router` |
| `0x40` | any | `midplane` |
| other | | `unknown` |

Linux names class `0x10` node, `0x20` I/O, `0x30` router and `0x40` midplane, and type `0x33` a meta router; it has no class `0x80` and no type `0x34`.

Below a node board vertex the kernel creates `memory`, `hub`, `cpu/<slice letter>` (an edge to the processor's vertex), `cpubus/0` and the processor vertices `cpubus/0/<slice letter>`; a disabled processor becomes `disabled/cpu/<letter>`.

## Routines

- `klhwg_add_all_nodes` (`6.5.30/unix.IP27` `0xc000000000042ec8`, 216 bytes)
  builds the node part of the graph under the root vertex in a0. It finds the boot node's GDA (the
  doubleword at node offset `0x20d0`, through `get_nasid`), and for each compact node from 0 to
  `numnodes` minus 1 (the word at gp+7340, reloaded every pass; the loop branch at
  `0xc000000000042f2c` is a `bnezl`) calls `klhwg_add_node` with the root, the compact node and the
  GDA pointer. It then calls `klhwg_add_all_xbows` for each compact node, and finally
  `klhwg_add_all_routers`, `klhwg_connect_routers`, `klhwg_connect_hubs`, `router_guardians_set` and
  `klhwg_device_disable_hints_add`, each with the root vertex. Established.
- `klhwg_add_node` (`6.5.30/unix.IP27` `0xc000000000041a2c`, 980 bytes)
  takes the root vertex, a compact node and the GDA pointer (unused). It converts the compact node
  to a NASID through `compact_to_nasid_node` (address from gp-19680), finds that node's first board
  through the directory, and calls `find_lboard` for type 17. It then handles every type-17 board on
  the node, following the list with `find_lboard` from each board's next offset and stopping when
  the offset is zero or no further type-17 board is found. The first board found is used without a
  null check, so a node whose list holds no type-17 board is not handled safely. For each board: (1)
  `board_to_path` builds the board's path and `hwgraph_path_add` creates it under the root. On
  failure it calls `cmn_err` with level 0 and "Node vertex creation failed.  Path == %s" and carries
  on with the same, unset, vertex. (2) It looks up the hub with `find_first_component` (type 2).
  When the hub's byte at +2 has bit 0 set (enabled) it calls `mark_nodevertex_as_node` with the
  vertex and compact node, names the vertex with `dev_to_name` (100-byte buffer), copies the name
  into a `kmem_alloc` buffer stored in the node's `hwg_node_name` (+20800 of `nodepda_s`; `assfail`
  at line 1045 when the allocation returns null), adds the label "_node" with the node's `pdinfo`
  (+20824) through `hwgraph_info_add_LBL`, and stores the board's slot byte (+10) in `slotdesc`
  (+18856), its module halfword (+14) in `module_id` (+18858), and the result of `module_lookup` for
  that module in `module` (+18864). A disabled hub skips all of this. (3) It calls `klhwg_add_mem`
  with the vertex, the compact node, the first type-3 component (memory), the board's slot byte and
  a node number: the NASID for an enabled hub, the hub's byte at +24 (`physid`) for a disabled one.
  (4) It creates `cpubus` under the vertex; on failure it calls `cmn_err` with level 0 and
  "klhwg_add_node: Cannot create CPUBUS directory\n" and continues. When `cnode_subnode_to_cpu` for
  subnode 0 is not -1 it creates `cpubus/0` (the name from `sprintf` with "%d" and 0), marks it with
  `mark_subnodevertex_as_subnode` for subnode 0, and calls the unnamed routine at
  `0xc000000000041a24`, which returns 0 and does nothing else. (5) When the board has a processor
  component (type 1) it creates `cpu` under the vertex (on failure `cmn_err` level 0 with
  "klhwg_add_node: Cannot create CPU directory\n"), then for each processor component calls
  `klhwg_add_cpu` when its byte at +2 has bit 0 set and `klhwg_add_disabled_cpu` (with the board's
  slot) when it is clear. (6) With an enabled hub it calls `klhwg_add_hub` with the vertex, the hub
  component and the compact node. Established.
- `board_to_path` (`6.5.30/unix.IP27` `0xc00000000001e638`, 268 bytes)
  writes a board's graph path into the buffer in a1, as in [Path names](#path-names): it chooses the
  kind string from the board's type byte at +5, calls `get_slotname` for the slot byte at +10, and
  calls `sprintf` with "%H/slot/%s/%s", the module halfword at +14, the slot name and the kind.
  Types 51 and 52 also add one to `hasmetarouter` (the word at gp+2324). Strings are at
  `0xc000000001404ce8` (from gp-20768) plus 264 to 352. Established.
- `klhwg_add_mem` (`6.5.30/unix.IP27` `0xc0000000000417a8`, 104 bytes)
  creates `memory` under the node vertex in a0 with `hwgraph_path_add`; on failure it returns at
  once. Otherwise it calls `device_master_set` for the new vertex with the node vertex as master,
  then `klhwg_mem_invent_info` with the new vertex and its other four arguments (compact node,
  memory component, slot, node number). Established.
- `klhwg_add_hub` (`6.5.30/unix.IP27` `0xc000000000041214`, 208 bytes)
  creates `hub` under the node vertex in a0 (the result is not checked), calls `device_master_set`
  with the node vertex as master, and adds the label "_hubinfo" with the address of the node's
  `hubstats` (+19608 of `nodepda_s`). When that label call fails it calls `cmn_err` with level 4 and
  "klhwg_add_hub: Can't add hub info label %v, code %d" and goes on. It then calls
  `klhwg_hub_invent_info` (vertex, compact node, hub component), `init_hub_stats` (compact node,
  `nodepda_s` pointer) and `sndrv_attach` (vertex). Established.
- `klhwg_add_cpu` (`6.5.30/unix.IP27` `0xc000000000041384`, 440 bytes)
  takes the node vertex, the compact node and a processor component. The processor number is the
  signed halfword at +30 of the component; the slice letter is `'a'` plus the processor's `p_slice`
  byte (+1312 of `pda_s`). It creates `cpubus/0/<letter>` (format "%s/%d/%c" with "cpubus" and 0),
  marks it with `mark_cpuvertex_as_cpu` and the processor number, adds the label "_cpu" with the
  processor's `pdinfo` (+1848), and sets the node vertex as its master. When `hwgraph_edge_get`
  finds `cpu` under the node vertex it adds an edge named by the slice letter from there to the new
  vertex. It then calls `klhwg_cpu_invent_info`, formats the vertex name with "%v", copies it into a
  `kmem_alloc_node` buffer (flags 257, on the compact node) stored in the processor's `p_cpunamestr`
  (+1680), and calls `assfail` (line 546) when the allocation returns null. The later lookups index
  `pdaindr` by the full word at +28 of the component rather than the halfword at +30. Established.
- `klhwg_add_disabled_cpu` (`6.5.30/unix.IP27` `0xc0000000000412e4`, 160 bytes)
  takes the node vertex, compact node, processor component and slot. It returns at once when the
  component's halfword at +30 is -1; otherwise it creates `disabled/cpu/<letter>` with the letter
  `'a'` plus the component's byte at +24 (`physid`), marks it with `mark_cpuvertex_as_cpu` and the
  processor number, sets the node vertex as master and calls `klhwg_disabled_cpu_invent_info` with
  the vertex, compact node, component and slot. Established.
- `klhwg_add_all_xbows` (`6.5.30/unix.IP27` `0xc000000000041e00`, 248 bytes)
  takes a compact node, converts it to a NASID through `compact_to_nasid_node`, and walks that
  node's type-17 boards as `klhwg_add_node` does. For each one it calls `find_lboard` from that
  board for type 65 (`0x41`, the crossbow midplane; Linux `KLTYPE_MIDPLANE8`) and, when one is
  found, calls `klhwg_add_xbow` with the compact node, the node board's halfword at +32
  (`brd_nasid`) and the crossbow board. Established.
- `klhwg_add_xbow` (`6.5.30/unix.IP27` `0xc000000000041810`, 532 bytes)
  takes a compact node, a NASID and a crossbow board. It returns at once when the board's byte at +9
  has bit 2 set (`0x4`, Linux `DUPLICATE_BOARD`), so a crossbow seen from two nodes is built once,
  and when the board has no type-4 component (crossbow). Otherwise it creates an unnamed vertex with
  `hwgraph_vertex_create`, calls `xswitch_vertex_init` on it, and scans crossbow ports 8 to 15. The
  port records start at +56 of the crossbow component, 8 bytes each, indexed from port 8 (Linux
  `xbow_port_info[link - BASE_XBOW_PORT]`): the signed halfword at +0 of a record is the NASID
  behind the port and the byte at +2 its flags. For each port whose flag byte has bit 1 set (`0x2`,
  a hub; Linux `XBOW_PORT_HUB`): (1) when the port's NASID is -1 it calls `cmn_err` with level 4 and
  "hub widget %d, skipping xbow graph\n" and moves on; the value passed for `%d` is the port number
  times 8, because a2 still holds the scaled index; (2) it adds the label "_xswitch_id" to the
  crossbow vertex with the 32-bit result of `hub_slot_to_crossbow` for the calling node's `slotdesc`
  (+18856 of `nodepda_s`); (3) it converts the port's NASID to a compact node through
  `nasid_to_compact_node` (address from gp-19664) and calls `is_specified` with the address of
  `arg_maxnodes`. Only when that returns nonzero does a compact node of -1 skip the port; otherwise
  the -1 is used as an index; (4) it stores the crossbow vertex in that node's `xbow_vhdl` (+18828)
  and reads its `node_vertex` (+20816); (5) when the port's NASID differs from the NASID argument,
  the two hubs share the crossbow: it stores the argument NASID in the port node's `xbow_peer`
  (+18832) and the port's NASID in the `xbow_peer` of the argument's compact node; (6) it adds an
  edge named "xtalk" from the port node's vertex to the crossbow vertex with `hwgraph_edge_add`.
  When that fails with 1 it first calls `cmn_err` level 4 with "klhwg_add_xbow: Check for working
  routers and router links!"; for any failure it then calls `cmn_err` (level 4 after a failure of 1,
  else 0) with "klhwg_add_xbow: Failed to add edge: vertex 0x%x (%v) to vertex 0x%x (%v), error
  %d\n", the node vertex twice, the crossbow vertex twice and the error. Established.
- `klhwg_add_all_routers` (`6.5.30/unix.IP27` `0xc000000000041ef8`, 768 bytes)
  creates a vertex for every router board. For each compact node from 0 to `maxnodes` minus 1 (the
  word at gp+7348) it takes the NASID from `compact_to_nasid_node` without testing it for -1, finds
  the node's first board through the directory, and walks the boards of router class with
  `find_lboard_class` and 49 (`0x31`, compared as class `0x30`). A board whose byte at +9 has bit 2
  set (duplicate) is skipped. For the others: (1) it looks up the router component (type 9). Without
  one it goes straight to step (4); (2) it reads the manufacturing NIC string, whose offset is the
  word at +128 of the component (Linux `rou_mfg_nic`). With an offset of zero, or no "Part" in the
  string, it calls `cmn_err` level 4 with "Cannot read router nic in module %M, slot r%d\n", the
  module halfword at +14 and the slot byte at +10 masked with `0xf`, and treats the board as
  current. When "030-0841" follows "Part", the revision is `atoi` of the text 14 bytes after that
  match; otherwise the board is current (revision 3); (3) when the revision is below 3 it checks
  ports 1 to 6, whose records start at +72 of the component (8 bytes each, from index 0; Linux
  `rou_port`, signed NASID halfword at +0, board offset word at +4). A port with a NASID other than
  -1 whose board has type 17 and an enabled hub (type 2, bit 0 of the byte at +2) whose doubleword
  at +88 (Linux `hub_speed`) is 100000000 (`0x5f5e100`, 100 MHz) makes it call `cmn_err` level 4
  with "downrev router board in module %M slot %s\n", the module and the `get_slotname` text, once
  per such port; (4) it builds the board's path with `board_to_path` and creates it under the root
  vertex with `hwgraph_path_add`; on failure it calls `cmn_err` level 0 with "Router vertex creation
  failed.  Path == %s". Established.
- `xswitch_vertex_init` (`6.5.30/unix.IP27` `0xc0000000000240c4`, 88 bytes)
  calls `kmem_zalloc` for 32 zeroed bytes (a1 of 0), then calls `init_mutex` for a mutex at the start
  of them (a1 0, a3 -1, the name 24 bytes past "_xswitch_volunteer" in the same string
  block), and attaches the block to the vertex in a0 with `hwgraph_info_add_LBL` under the label
  "_xswitch_volunteer". The allocation is not checked for null. It returns the result of
  `hwgraph_info_add_LBL`. Established.
- `router_guardians_set` (`6.5.30/unix.IP27` `0xc00000000002eba8`, 304 bytes)
  takes a vertex in a0 and, for each compact node below `maxnodes` (the word at gp+7348), finds the
  node's first board through the directory and walks its router boards with `find_lboard_class` for
  type `0x31` (class `0x30`, Linux `KLCLASS_ROUTER`), continuing each walk from the board's next
  offset (+0) on the same node. For every router board it calls `sub_c00000000002ead8` with a0 and
  the board. It returns nothing useful. Established.
- `sub_c00000000002ead8` (`6.5.30/unix.IP27` `0xc00000000002ead8`, 208 bytes)
  is unsymbolled. It returns at once for a board whose byte at +9 has bit 2 set (`0x4`, Linux
  `DUPLICATE_BOARD`); otherwise it builds the board's path with `board_to_path` and looks it up from
  a0 with `hwgraph_traverse`. A failed lookup calls `cmn_err` with level 4, "Can't find router: %s"
  and the path. A router whose component count (+23) is not 1 calls `cmn_err` with level 0,
  "connect_one_router: %d cmpts on router\n" and the count. Otherwise it calls the unsymbolled
  routine at `0xc00000000002e798` with the address of a stack doubleword it cleared on entry and the
  router vertex, and unless that returns -1 calls `sub_c00000000002ea0c` with that result, the value
  now in the stack doubleword, the board and the router vertex. Established; what the routine at
  `0xc00000000002e798` chooses (it searches neighbouring node vertices and keeps the one with the
  fewest dependent routers) is inferred from a partial read.
- `sub_c00000000002ea0c` (`6.5.30/unix.IP27` `0xc00000000002ea0c`, 204 bytes)
  is unsymbolled. It looks up the compact node of its first argument with `nodevertex_to_cnodeid`,
  allocates a 40-byte record on that node with `kmem_zalloc_node` (a1 257), and calls `assfail`
  (line 1015) when that fails. It appends the record to the node's list through `npda_rip_last`
  (+18816 of `nodepda_s`; the record's link is at +32, cleared) and adds one to `dependent_routers`
  (+18824). It stores a1 at +16 of the record and a3 as the word at +0, and copies the board's
  (a2) slot byte (+10) to +10, module halfword (+14) to +8 and type byte (+5) to +11. Established.
- `klhwg_connect_hubs` (`6.5.30/unix.IP27` `0xc0000000000425bc`, 564 bytes)
  links each node vertex to whatever its hub port reaches. For each compact node from 0 to
  `maxnodes` minus 1 it takes the NASID from `compact_to_nasid_node`, finds the first type-17 board
  and its hub component (neither result is tested for null), and reads the hub's port record at +60
  (Linux `hub_port`: signed NASID halfword at +60, board offset word at +64). A port NASID of -1
  skips the node, and so does a port NASID whose `nasid_to_compact_node` entry is -1 when
  `is_specified` reports `arg_maxnodes` set. Otherwise it finds the node board's vertex by
  `board_to_path` and `hwgraph_traverse` from the root (on failure `cmn_err` level 4 with "Can't
  find hub: %s", then it carries on), forms the far board's pointer from the port NASID and offset,
  and finds that board's vertex the same way. When the far vertex is missing it is silent only if
  `arg_maxnodes` is set and the far board's byte at +9 has bit 2 set (duplicate); otherwise it calls
  `cmn_err` level 0 with "Can't find board: %s". With both vertices it adds an edge named "link"
  from the node vertex to the far vertex, and on failure calls `cmn_err` level 0 with "Can't create
  edge: %s/%s to vertex 0x%x, error 0x%x\n", both paths, the far vertex and the error. Established.
- `klhwg_connect_routers` (`6.5.30/unix.IP27` `0xc00000000004247c`, 320 bytes)
  calls `klhwg_connect_one_router` for every router-class board: for each compact node from 0 to
  `maxnodes` minus 1 it takes the NASID from `compact_to_nasid_node`, walks that node's boards with
  `find_lboard_class` and 49 (class `0x30`), and passes the root vertex, the board, the compact node
  and the NASID. Established.
- `klhwg_connect_one_router` (`6.5.30/unix.IP27` `0xc0000000000421f8`, 644 bytes)
  adds one edge per connected router port. It returns at once for a board whose byte at +9 has bit 2
  set (duplicate). It finds the router's vertex by `board_to_path` and `hwgraph_traverse` from the
  root; when that fails it returns silently if `is_specified` reports `arg_maxnodes` set, and
  otherwise calls `cmn_err` level 4 with "Can't find router: %s" and carries on. A board whose
  component count (byte at +23) is not exactly 1 gets `cmn_err` level 0 with
  "klhwg_connect_one_router: %d cmpts on router\n" and the count, and the routine returns. The
  router component is the first offset at +36, formed directly. For each port from 1 to 6 (records
  at +72 of the component, 8 bytes each, as in `klhwg_add_all_routers`) with a NASID other than -1,
  and, when `arg_maxnodes` is set, a compact node other than -1, it forms the far board from the
  port's NASID and the offset word at +76 of the record, and finds its vertex the same way. When
  that lookup fails it skips the port silently only if `arg_maxnodes` is set and the far board is a
  duplicate; otherwise it calls `cmn_err` level 0 with "Can't find router: %s" and the far path and
  still goes on to add the edge, with whatever the lookup left in its result slot. The edge runs
  from the router vertex to the far vertex and is named by the port number (`sprintf` with "%d"). A
  result of 0 or 1 from `hwgraph_edge_add` is accepted; any other result is reported with `cmn_err`
  level 0 and "Can't create edge: %s/%s to vertex 0x%x error 0x%x\n" (both paths, the far vertex and
  the error), unless `arg_maxnodes` is set. Established.

## Modules

The kernel keeps a table of module records, `modules` (`0xc0000000014de3f0`, one doubleword pointer per module), with the count in `nummodules` (the word at gp+2704). The record layout is not in the kernel's DWARF (`module_s` is declared with no members); the routines below read the module number as the signed halfword at +0 of a record and six signed halfwords from +8, which `cnodemodule_lookup` matches against its argument (compact node numbers, inferred from the routine's name).

- `klhwg_add_all_modules` (`6.5.30/unix.IP27` `0xc000000000042dc4`, 260 bytes)
  creates, for each module from 0 to `nummodules` minus 1, the path "module/%M" (the module number
  at +0 of the record) under the root vertex in a0 and stores the record pointer in that vertex with
  `hwgraph_fastinfo_set`; the first `hwgraph_path_add` result is not checked. It then creates
  "module/%M/elsc", calls `assfail` (line 1776) when that fails, adds the label "_elsc" with the
  value 1 and calls `sndrv_attach` on the new vertex. Established.
- `cnodemodule_lookup` (`6.5.30/unix.IP27` `0xc000000000040428`, 112 bytes)
  returns the first module record from `modules` (address at gp-19720) whose halfwords at +8, +10,
  up to +18 include a0 (each loaded at 0xc00000000004045c), scanning `nummodules` records, or 0 when
  none does. Established.
- `module_lookup` (`6.5.30/unix.IP27` `0xc000000000040370`, 72 bytes)
  returns the first record in `modules` (address at gp-19720) whose first signed halfword (loaded at
  0xc000000000040390) equals a0 (the module number), scanning `nummodules` records, or 0.
  `klhwg_add_node` stores its result in the node's `module`. Established.

## Disabled-device hints

- `klhwg_device_disable_hints_add` (`6.5.30/unix.IP27` `0xc0000000000427f0`, 504 bytes)
  calls `device_admin_table_init`, then for each compact node from 0 to `numnodes` minus 1 walks
  every board on the node (all types). It skips boards whose byte at +9 has bit 2 set (duplicate)
  and boards whose class (+5 masked with `0xf0`) is not `0x20` (I/O). For each component offset of
  an I/O board (count at +23, offsets from +36) whose component byte at +2 has bit 0 clear
  (disabled), it builds the component's canonical path with `device_component_canonical_name_get`
  and calls `device_admin_table_update` with that path, "DISABLE" and "yes". So every disabled I/O
  component the PROM records becomes an administrative "disable" hint keyed by its `/hw` path.
  Established.
- `device_component_canonical_name_get` (`6.5.30/unix.IP27` `0xc00000000001e878`, 172 bytes)
  takes a board, a component and an output buffer and writes "/hw/module/%M/slot/%s/%s/pci/%d" with
  the board's module halfword (+14), the `get_slotname` text for its slot byte (+10), a board name,
  and the component's byte at +24 (`physid`) with bit 4 cleared (`& ~0x10`). For a board whose byte
  at +6 (Linux `brd_sversion`) is 2 or more it calls `nic_name_convert` on `brd_name` (+153 of the
  board) for the name. The null-board test comes after the module and slot loads (at
  0xc00000000001e87c and 0xc00000000001e88c), so a null board is dereferenced before it is tested.
  For an older record version the code copies the 8 bytes "xwidget" into the output buffer and then
  formats over it, passing the stack buffer that `nic_name_convert` would have filled, which this
  path never writes; the name in the result is then whatever that stack buffer held. Established;
  the fault is read from the code (the copy targets the third argument, the format reads the local
  buffer).

## Inventory records and the checks made on the way

Each node, hub and processor vertex gets a "_detail_invent" label holding a small inventory record, exported with `hwgraph_info_export_LBL` so that user programs can read it. Building those records is where the 6.5.30 IP27 kernel warns about down-revision parts; none of these checks stops the boot.

- `klhwg_invent_alloc` (`6.5.30/unix.IP27` `0xc000000000040a58`, 116 bytes)
  takes a compact node (a0), an inventory class (a1) and a size (a2). It calls `kern_malloc` for the
  size and returns 0 when that fails. Otherwise it stores the node's `module_id` (+18858 of
  `nodepda_s`) as the halfword at +0 of the new record, the class as the word at +8, and the node's
  `slotdesc` byte (+18856) masked with `0xf` as the halfword at +2, and returns the record.
  Established.
- `klhwg_hub_invent_info` (`6.5.30/unix.IP27` `0xc000000000040e30`, 268 bytes)
  takes the hub vertex, compact node and hub component. It allocates a 32-byte record of class 14
  with `klhwg_invent_alloc` and returns when that fails. It sets the byte at +4 of the record to 1
  when the hub is enabled (bit 0 of the component's byte at +2), stores 4 as the word at +20, the
  hub revision (the component's byte at +3, Linux `revision`) as the word at +12 and the doubleword
  at +88 of the component (Linux `hub_speed`) at +24. A revision below 3 makes it call `cmn_err`
  level 4 with "INVALID CUSTOMER CONFIGURATION - DOWNREV Hub ASIC: rev %s (code=%d) at %v.", the
  revision's name from the table `hub_rev` (`0xc000000001417740`: "0.0", "1.0", "2.0", "2.1", "2.2",
  "2.3" for codes 0 to 5), the code and the vertex. A revision below 5 while `maxcpus` (the word at
  gp+2352) is 33 or more makes it call `cmn_err` level 4 with the message that all hubs must be at
  least revision 2.3 above 32 processors. It then adds the record under "_detail_invent" and exports
  32 bytes of it. Established.
- `klhwg_cpu_invent_info` (`6.5.30/unix.IP27` `0xc000000000040acc`, 384 bytes)
  takes the processor vertex, compact node and processor component, and allocates a 36-byte record
  of class 1. It sets the byte at +4 to 1 for an enabled processor and 0 for a disabled one, and
  copies the component's halfwords (Linux `klcpu_t` names): +56 (`cpu_prid`) to the record word at
  +12, +60 (`cpu_speed`) to +16, +64 (`cpu_scachespeed`) to +24, +62 (`cpu_scachesz`) to +20, and
  +58 (`cpu_fpirr`) to the halfword at +32. The record halfword at +28 takes the processor number
  (the word at +28 of the component) and +30 the processor's `p_slice`. The first processor seen
  records its speed in a static word (`0xc0000000014ca284`, gp-32492); for later ones it calls
  `ip27log_getenv` for the node with "BackPlane" and the default "000", and only when that returns a
  negative value compares the speeds, calling `cmn_err` level 0 with "Speed of the CPU %d (%v) is
  %dMHz, different from another CPU at %dMHz\n" when they differ. A `cpu_prid` below 2342 (`0x926`)
  makes it call `cmn_err` level 4 with "CPU %d (%v) is downrev (%x)", the processor number, the
  vertex and the PRID. It then adds and exports the record under "_detail_invent" (36 bytes).
  Established.
- `klhwg_disabled_cpu_invent_info` (`6.5.30/unix.IP27` `0xc000000000040c4c`, 344 bytes)
  takes the vertex, compact node, processor component and slot. It calls `klhwg_invent_alloc` twice,
  for a 36-byte record of class 1 and a 92-byte record of class 70, returning if either fails. The
  first is filled as by `klhwg_cpu_invent_info` with three differences: the flag byte is 0, the
  halfword at +2 is the slot argument, and the halfword at +30 is the component's `physid` (+24)
  rather than a slice. In the second it stores the component's halfword at +4 (Linux `diagval`) as
  the word at +80, writes at +0 the text the unnamed routine at `0xc000000000040a14` returns for
  that value (through `sprintf` with "%s"), and stores the processor number (the word at +28) at +88
  and `physid` at +84. The PRID check and warning are the same. It exports the first record as
  "_detail_invent" (36 bytes) and the second as "_diag_reason" (92 bytes). Established.
- `klhwg_mem_invent_info` (`6.5.30/unix.IP27` `0xc000000000040f3c`, 728 bytes)
  takes the memory vertex, compact node, memory component, slot and NASID, and allocates an 80-byte
  record of class 3. It stores the slot as the halfword at +2 and sets the byte at +4 to 1 for an
  enabled component. It reads the node's PROM log variables "DisableMemMask" into one stack buffer
  and, only when that succeeds, "DisableMemSz" into another, through `ip27log_getenv`. It reads the
  doubleword at +112 of the node's boot PROM window (`0x9000000030000000` from gp-24488, plus the
  NASID shifted left 32; Linux `NODE_RBOOT_BASE` is that window) and divides it by 1000000 (`ddivu`,
  with a trap on a zero divisor). For each of the eight banks i it fills 8 bytes of the record from
  offset 16 + 8i: the halfword there is the bank size, the component's halfword at +60 + 2i (Linux
  `membnk_bnksz`, in megabytes); the halfword at record offset 18 + 8i gets bit 0 from the
  component's halfword at +76 (Linux `membnk_attr`), bit 0 of that halfword for a record version
  (component byte at +1) below 2 and bit i from version 2 on; the word at record offset 20 + 8i gets
  bit 0 when the bank is enabled. For version 1 records "enabled" is the component's enable bit.
  From version 2 a bank is enabled unless its digit ("0" plus i) appears in "DisableMemMask"; a
  disabled bank's size halfword is replaced by 0 when its "DisableMemSz" character is "0", and
  otherwise by 4 shifted left by that character's digit value. A bank size of 256 while the PROM
  value is 180 or 195 after the division makes it call `cmn_err` level 4 with "Found 256 MB in bank
  %d on IP27 node in module %M slot %d\n", the bank, the module halfword and the slot. Finally it
  stores the sum of the component's eight bank sizes (before any "DisableMemSz" replacement) as the
  halfword at +12, 8 as the halfword at +14, and adds and exports the record as "_detail_invent" (80
  bytes). Established for the code; that the PROM value is a processor clock in hertz is inferred
  from the division and the values 180 and 195.
- `klhwg_baseio_inventory_add` (`6.5.30/unix.IP27` `0xc000000000040da4`, 140 bytes)
  takes a vertex (a0) and a compact node (a1), allocates a 32-byte record of class 21, clears the
  word at +20 (without testing the allocation), calls `nvram_prom_version_get` for two bytes on the
  stack, stores the second byte as the word at +12 and the first as the word at +16, and adds and
  exports the record as "_detail_invent" (32 bytes). Established.
- `klhwg_add_rps` (`6.5.30/unix.IP27` `0xc00000000004153c`, 172 bytes)
  takes a node vertex, a compact node and a flag. It returns at once when the compact node is -1, or
  when `hwgraph_path_add` fails to create "rps" under the vertex. Otherwise it calls
  `device_master_set` to make the node vertex master of the new vertex, calls `klhwg_invent_alloc`
  for a 16-byte record of class 23, clears its word at +12, sets the byte at +4 to 1 when the flag
  is nonzero and 0 otherwise, and adds and exports it as "_detail_invent" (16 bytes). Established.
- `klhwg_update_rps` (`6.5.30/unix.IP27` `0xc0000000000415e8`, 140 bytes)
  takes a compact node and a flag. Unless the node is -1, it calls `hwgraph_edge_get` for the "rps"
  edge under the node's `node_vertex` (+20816 of `nodepda_s`), calls `hwgraph_info_get_LBL` for its
  "_detail_invent" record, and sets that record's byte at +4 to 1 or 0 from the flag. It returns
  early when either lookup fails. Established.
- `klhwg_add_xbox_rps` (`6.5.30/unix.IP27` `0xc000000000041674`, 172 bytes)
  is `klhwg_add_rps` for the vertex "xbox_rps" (it calls `hwgraph_path_add`, `device_master_set` and
  `klhwg_invent_alloc` the same way), with one difference: the word at +12 of the record is set to
  1, not cleared. Established.
- `klhwg_update_xbox_rps` (`6.5.30/unix.IP27` `0xc000000000041720`, 136 bytes)
  is `klhwg_update_rps` for the "xbox_rps" edge: it calls `hwgraph_edge_get` and
  `hwgraph_info_get_LBL` the same way, and the instructions differ only in scheduling. Established.

## IP35 differences

The 6.5.30 IP35 kernel builds the same graph with the IP35 NASID position, its own table sizes and data offsets, and the PE-brick check of [klconfig.md](klconfig.md) before each directory read. The per-node and per-processor fields move: in `6.5.30/unix.IP35` `xbow_vhdl` is at +34300 of `nodepda_s`, `xbow_peer` +34304, `slotdesc` +34328, `module_id` +34330, `module` +34336, `hubstats` +34760, `hwg_node_name` +35952, `node_vertex` +35968 and `pdinfo` +35976; in `pda_s`, `p_slice` is at +1320, `p_cpunamestr` +1688 and `pdinfo` +1856 (all from the kernel's DWARF). The strings are reached from gp-21664 (`0xc000000001526690`), the board-name strings from gp-23392 (`0xc00000000151a460`).

- `board_to_path` (`6.5.30/unix.IP35` `0xc0000000000451f8`, 404 bytes)
  names boards by module only: it calls `sprintf` with "module/%M/%s", the module halfword at +14
  and a kind string, with no slot. The kinds by class (+5 masked with `0xf0`) are those of IP27
  (`node`, `io` for `0x20` and `0x80`, `metarouter` for type 51 and `repeaterrouter` for 52 with
  `hasmetarouter` at gp+5268 raised, `router`, `midplane`, `unknown`) plus class `0x70`, whose types
  name I/O bricks: 113 "Ibrick", 114 "Pbrick", 115 "Xbrick", 116 "Nbrick", 117 "PEbrick", 118
  "PXbrick", 119 "IXbrick", 120 "CGbrick", any other "iobrick". Linux's header names I-, P- and
  X-brick types in that order (`KLTYPE_IBRICK`, `KLTYPE_PBRICK`, `KLTYPE_XBRICK`) under a class it
  does not give a value. Established.
- `klhwg_add_all_nodes` (`6.5.30/unix.IP35` `0xc0000000000748ac`, 212 bytes)
  is the IP27 routine with the IP35 NASID shift for the GDA lookup and `numnodes` read from gp+10276
  (loop branch at `0xc000000000074910`, a `bnezl`); it calls `klhwg_add_node`,
  `klhwg_add_all_xbows`, `klhwg_add_all_routers`, `klhwg_connect_routers`, `klhwg_connect_hubs`,
  `router_guardians_set` and `klhwg_device_disable_hints_add` in the same order. Established.
- `klhwg_add_hub` (`6.5.30/unix.IP35` `0xc00000000007293c`, 212 bytes)
  matches the IP27 routine instruction for instruction except for the string block and the
  `hubstats` offset, +34760 (`0x87c8`) of `nodepda_s`; it calls `hwgraph_path_add`,
  `device_master_set`, `hwgraph_info_add_LBL`, `klhwg_hub_invent_info`, `init_hub_stats` and
  `sndrv_attach` as there. Established.
- `klhwg_add_mem` (`6.5.30/unix.IP35` `0xc000000000072c70`, 104 bytes)
  matches the IP27 routine except for the string address and one reordered load; it calls
  `hwgraph_path_add` for "memory", then `device_master_set` and `klhwg_mem_invent_info`.
  Established.
- `klhwg_add_disabled_cpu` (`6.5.30/unix.IP35` `0xc000000000072a10`, 160 bytes)
  matches the IP27 routine except for the string addresses: it creates `disabled/cpu/<letter>` from
  `physid` (+24) and calls `mark_cpuvertex_as_cpu`, `device_master_set` and
  `klhwg_disabled_cpu_invent_info`. Established.
- `klhwg_add_cpu` (`6.5.30/unix.IP35` `0xc000000000072ab0`, 448 bytes)
  differs from the IP27 routine in the processor vertex path: with the slice from `p_slice` (+1320
  of `pda_s`), it creates `cpubus/<n>/<letter>` where n is bit 1 of the slice and the letter is
  `'a'` plus bit 0, so the four processors of a node sit two to a bus. The edge under `cpu` is still
  named `'a'` plus the whole slice. The other steps, the `pda_s` offsets (+1856 `pdinfo`, +1688
  `p_cpunamestr`) and the `assfail` at line 546 are as on IP27; it calls `mark_cpuvertex_as_cpu`,
  `hwgraph_edge_get`, `hwgraph_edge_add`, `klhwg_cpu_invent_info` and `kmem_alloc_node`.
  Established.
- `klhwg_invent_alloc` (`6.5.30/unix.IP35` `0xc000000000072328`, 128 bytes)
  calls `kern_malloc` and fills the record as on IP27 from the IP35 `module_id` (+34330) and
  `slotdesc` (+34328) of `nodepda_s`. Established.
- `klhwg_add_all_xbows` (`6.5.30/unix.IP35` `0xc0000000000737b4`, 280 bytes)
  differs from the IP27 routine in three ways: it takes the NASID from the IP35
  `compact_to_nasid_node` (address from gp-21632); when the node's register at `0x9000000034000000`
  (plus the NASID shifted left 33) has `0xc` in bits 12 to 15 it calls `chk_for_pebrick` for the
  first board; and it searches for type 66 (`0x42`; Linux `KLTYPE_PBRICK_XBOW`) instead of 65. It
  calls `find_lboard` and `klhwg_add_xbow` as there. Established.
- `klhwg_add_xbow` (`6.5.30/unix.IP35` `0xc000000000072f34`, 692 bytes)
  takes three arguments and reads the second (a1, kept in s0 as a NASID) and the third (a2, the crossbow board); the
  first (the compact node on IP27) is never read. It returns at once when the board's byte at +9 has bit 2 set, and when
  `find_component` (type 4, crossbow) finds no component on the board. Otherwise it calls `hwgraph_vertex_create` and
  `xswitch_vertex_init` as the IP27 routine does, but it makes no "_xswitch_id" label and does not call
  `hub_slot_to_crossbow`. It then reads the doubleword at `0x9000000034000000` plus the NASID shifted left 33 (pool
  word at gp+2256; see [the IP35 node class nibble](klconfig.md#the-ip35-node-class-nibble)) and branches on bits 12 to
  15 being `0xc` (a PE-brick node). The two branches differ.
  Not `0xc` (an ordinary node): it calls `iobrick_module_get_nasid` with the NASID and 8, sign-extends the low halfword
  of the result and, unless that is -1, calls `iobrick_register` with it. Then it scans ports 8 to 15 as the IP27
  routine does (port record at the component +56 plus 8 times (port - 8): signed halfword NASID at +0, flag byte at +2,
  bit 1 set for a hub). A port whose NASID is -1 logs level 4 "hub widget %d, skipping xbow graph\n" with the port
  number times 8, as on IP27. The compact node of a NASID is the halfword at index NASID of the array
  `nasid_to_compact_node` (read directly through gp-21656; on IP35 this is a data array here, not a call).
  `is_specified` is called with `arg_maxnodes` (gp-21640); only when it returns nonzero does a compact node of -1 skip
  the port. The per-node data is the entry at index compact node of the array behind the doubleword at absolute address
  -15360. The routine stores the crossbow vertex in the word at +34300 of it (`xbow_vhdl`), reads the word at +35968
  (`node_vertex`) and, when the port NASID differs from the argument NASID, stores the argument NASID as a halfword at
  +34304 (`xbow_peer`) of the port's node and the port NASID at +34304 of the argument's node. It calls
  `hwgraph_edge_add` from the node vertex to the crossbow vertex with the name "xtalk" (string at
  `0xc0000000015268d8`). On a nonzero return it logs as IP27 does: level 4 "klhwg_add_xbow: Check for working routers
  and router links!" first when the return is 1, then "klhwg_add_xbow: Failed to add edge: ..." at level 4 for 1 and
  level 0 for any other error, with the node vertex twice, the crossbow vertex twice and the error. The loop goes on
  with the next port after a failure.
  `0xc` (a PE-brick node): it skips the port scan, the `is_specified` call and the `xbow_vhdl` and `xbow_peer` stores.
  It takes the compact node as the halfword at index NASID of the same array, reads `node_vertex` (+35968) of that
  node's per-node data and calls `hwgraph_edge_add` with the same "xtalk" name from the node vertex to the crossbow
  vertex. A nonzero return logs only the "Failed to add edge" message at level 4 (the "Check for working routers"
  message is not on this path) and the routine goes on. It then calls `elsc_module_get_nasid` with the NASID,
  sign-extends the low halfword of the result and, unless that is -1, calls `iobrick_register` with it.
  There is one return, and the routine sets no return value (v0 is whatever the last callee left). So the two module
  calls register the brick's module id in the table described under `iobrick_register`: on the ordinary-node path through
  `iobrick_module_get_nasid`, on the PE-brick path through `elsc_module_get_nasid`. Established for the control flow,
  callees and offsets; what the 4-bit class field means is unknown, as recorded in klconfig.md.
- `iobrick_module_get_nasid` (`6.5.30/unix.IP35` `0xc0000000000d084c`, 204 bytes)
  takes a NASID and a second value (a1, kept in s1) and returns a module id from one of three sources. It calls
  `get_elsc` first and keeps the result. It reads the node class doubleword for the NASID (pool word at gp+2256) and,
  when bits 12 to 15 are `0xc`, calls `peer_iobrick_rack_bay_get` with the NASID and the addresses of two stack words
  (rack at 0(sp), bay at 4(sp)); a nonzero return prints "Could not read rack and bay location of PEBrick at nasid
  %d\n" through `printf`; either way it then calls `peer_iobrick_module_get` with the `get_elsc` result and those two
  words. Otherwise it calls `get_nasid`: when that equals the argument NASID it calls `iobrick_module_get` with the
  `get_elsc` result and the second value; when it differs it calls `elsc_init` on a stack buffer for the argument NASID
  and then `iobrick_module_get` on that buffer with the second value. The return is the callee's result on all three
  paths. Established.
- `elsc_module_get_nasid` (`6.5.30/unix.IP35` `0xc0000000000cf1f4`, 224 bytes)
  takes a NASID and returns a module id. It reads the node class doubleword (gp+2256) first. When bits 12 to 15 are
  `0xc` it calls `peer_iobrick_rack_bay_get` (rack at 0(sp), bay at 4(sp); a nonzero return prints the PEBrick rack and
  bay message through `printf` as above), then `get_elsc` and `peer_iobrick_type_get` with the rack, the bay and the
  address of a third stack word. A negative return is returned as it is; otherwise it calls `sc_rbt_to_moduleid` with
  the rack, the bay and the type and returns its result. For any other class it calls `get_nasid`: when that equals the
  argument it calls `elsc_module_get` on the `get_elsc` result; otherwise it calls `elsc_init` on a stack buffer for the
  NASID and `elsc_module_get` on it. Established.
- `iobrick_register` (`6.5.30/unix.IP35` `0xc00000000006f6fc`, 36 bytes)
  passes its argument (a0, a module id) to the unlabelled routine at `0xc00000000006f5c0` (the symbol table labels
  the address `dprintf+0x33c`; it is its own routine), with the table at gp-21936 (`0xc000000001652498`), the count word
  at gp-32356 (`0xc00000000163b90c`) and the capacity 128. That routine returns 0 and prints a message through
  `printf` when the count is not below 128, or when the same id is already present (compared through a key derived by
  the routine at `0xc00000000006f4b4` and then the low 6 bits); otherwise it stores the id as a halfword at index count of
  the table, adds one to the count and returns 1. `rbrick_register` (`0xc00000000006f6b0`) makes the same call with the
  table plus 256 and the count at gp-32352. Established for the capacity, the append and the return values; the key
  derivation was read but its equivalence classes were not worked out, so what counts as "the same id" is inferred.
- `klhwg_hub_invent_info` (`6.5.30/unix.IP35` `0xc000000000072664`, 152 bytes)
  calls `klhwg_invent_alloc` for the same 32-byte class-14 record and fills it as on IP27 (enable
  flag at +4, 4 at +20, the component's revision byte (+3) as the word at +12, the doubleword at +88
  of the component at +24), then
  calls `hwgraph_info_add_LBL` and `hwgraph_info_export_LBL` for "_detail_invent". It makes neither
  revision check: the IP35 kernel issues no down-revision hub warning here. Established.
- `klhwg_cpu_invent_info` (`6.5.30/unix.IP35` `0xc0000000000723a8`, 276 bytes)
  calls `klhwg_invent_alloc` for the same 36-byte record and fills the same fields, taking the slice
  from `p_slice` at +1320 of `pda_s`. It records the first processor's speed in the static word at
  gp-32348 but never compares later speeds and never calls `ip27log_getenv`. The PRID check (below
  2342, `cmn_err` level 4 with "CPU %d (%v) is downrev (%x)") is kept. Established.
- `klhwg_mem_invent_info` (`6.5.30/unix.IP35` `0xc0000000000726fc`, 576 bytes)
  calls `klhwg_invent_alloc` for the same 80-byte record and fills the same per-bank fields from
  "DisableMemMask" and "DisableMemSz" through `ip27log_getenv`, with two differences: a disabled
  bank's recorded size is 32 shifted left by the "DisableMemSz" digit (IP27 uses 4), and there is no
  PROM read and no 256 MB warning. Established.
- `klhwg_add_node` (`6.5.30/unix.IP35` `0xc000000000073268`, 1356 bytes)
  follows the IP27 routine with the IP35 tables and offsets and adds PE-brick handling. It takes the
  NASID from `compact_to_nasid_node` (address from gp-21632) and calls `chk_for_pebrick` for the
  first board when the node's register at `0x9000000034000000` plus the NASID shifted left 33 has
  `0xc` in bits 12 to 15. For each type-17 board, after `board_to_path` and `hwgraph_path_add`
  (failure reported with `cmn_err` level 0 and "Node vertex creation failed.  Path == %s"), it calls
  `find_component` for type 38: (1) with such a component, it raises a static counter (the word at
  gp-32344) and skips the board once the counter exceeds `num_pebricks` (gp+10280); otherwise it
  calls `init_pebrick` with the board, the component and the vertex, takes the NASID from the
  component's halfword at +58 and the compact node from `nasid_to_compact_node` for it (address from
  gp-21656), and calls `eeprom_vertex_info_set` with 1056 (`0x420`), that NASID and the vertex. Such
  a board is treated as having an enabled hub even when its hub's enable bit is clear, gets no
  `module` lookup, no `cpubus` directory and no `hub` vertex; (2) without one, it calls
  `board_to_full_path`, cuts the result at its last "/" with `strrchr`, and calls `hwgraph_link_add`
  to link the board vertex's path under that directory with the last name. The node fields are then
  set as on IP27 at the IP35 offsets (`hwg_node_name` +35952, `pdinfo` +35976, `slotdesc` +34328,
  `module_id` +34330, `module` +34336), with `assfail` at line 1045 for a failed name allocation.
  `klhwg_add_mem` gets the NASID, or the hub's `physid` for a disabled hub. For an ordinary board it
  creates `cpubus` (on failure `cmn_err` level 0 with "klhwg_add_node: Cannot create CPUBUS
  directory\n") and, for subnodes 0 and 1 whose `cnode_subnode_to_cpu` is not -1, creates
  `cpubus/<n>` (`sprintf` with "%d"), marks it with `mark_subnodevertex_as_subnode`, and calls the
  unnamed routine at `0xc0000000000731e8` with the node vertex and NASID; when that returns nonzero
  it calls `eeprom_vertex_info_set` with `0x110` ORed with the subnode, the NASID and the subnode
  vertex. The processor and hub steps follow IP27 ("klhwg_add_node: Cannot create CPU directory\n"
  on failure; `klhwg_add_cpu`, `klhwg_add_disabled_cpu`, `klhwg_add_hub`). Established.
- `klhwg_add_all_modules` (`6.5.30/unix.IP35` `0xc000000000074750`, 348 bytes)
  walks `modules` (`0xc0000000016511a0`, address from gp-21648) up to `nummodules` (the word at
  gp+5544) as on IP27, but names the second vertex "module/%M/L1" instead of ".../elsc". After
  creating it (`assfail` at line 1776 on failure) and adding "_elsc" with the value 1, it calls
  `kern_malloc` for 32 bytes and `l1_mfg_string` with that buffer, the address 40 bytes into the
  module record and the module number; when that returns 0 it adds the buffer under "_nic", and
  otherwise it calls `kern_free` on it. It calls `sndrv_attach` on the vertex either way.
  Established.
- `cnodemodule_lookup` (`6.5.30/unix.IP35` `0xc000000000064d24`, 108 bytes)
  scans `modules` (address from gp-22528) up to `nummodules` (gp+5544) and returns the first record
  whose halfword at +8 (loaded at 0xc000000000064d58) equals a0; unlike IP27 it compares only that
  one halfword per record, or returns 0. Established.
- `module_lookup` (`6.5.30/unix.IP35` `0xc000000000064c70`, 72 bytes)
  is the IP27 routine over the IP35 `modules` (gp-22528) and `nummodules` (gp+5544): it returns the
  first record whose first halfword (loaded at 0xc000000000064c90) equals a0, or 0. Established.
- `klhwg_add_all_routers` (`6.5.30/unix.IP35` `0xc0000000000738cc`, 456 bytes)
  keeps only the vertex creation of the IP27 routine: for each compact node below `maxnodes`
  (gp+10284) it takes the NASID from `compact_to_nasid_node`, finds the first board through the
  directory (or `chk_for_pebrick` for a PE-brick node), and for each router-class board
  (`find_lboard_class` with 49) that is not a duplicate (bit 2 of the byte at +9) calls
  `board_to_path` and `hwgraph_path_add` under the root, reporting a failure with `cmn_err` level 0
  and "Router vertex creation failed.  Path == %s". It reads no router NIC and makes no
  down-revision check. Established.
- `klhwg_connect_routers` (`6.5.30/unix.IP35` `0xc000000000073d20`, 376 bytes)
  is the IP27 routine with the IP35 tables, `maxnodes` from gp+10284, the IP35 NASID position and a
  call to `chk_for_pebrick` instead of the directory read for a PE-brick node; it calls
  `find_lboard_class` with 49 and `klhwg_connect_one_router` for each router-class board.
  Established.
- `klhwg_connect_one_router` (`6.5.30/unix.IP35` `0xc000000000073a94`, 652 bytes)
  is the IP27 routine with three changes: once the board is known to have exactly one component it
  calls `rbrick_register` with the board's module halfword (+14); it scans router ports 1 to 8
  rather than 1 to 6 (records still at +72 of the component, offset word at +76, 8 bytes each); and
  it uses the IP35 tables and NASID position. The messages ("Can't find router: %s", the
  component-count message, "Can't create edge: ...") and the `arg_maxnodes` rules are as on IP27; it
  calls `board_to_path`, `hwgraph_traverse`, `is_specified`, `sprintf` and `hwgraph_edge_add` in the
  same way. Established.
- `klhwg_connect_hubs` (`6.5.30/unix.IP35` `0xc000000000073e98`, 600 bytes)
  is the IP27 routine with the IP35 tables, `maxnodes` from gp+10284, the IP35 NASID position,
  `arg_maxnodes` from gp-21640, and a call to `chk_for_pebrick` instead of the directory read for a
  PE-brick node. It reads the hub port record at +60 and +64 of the hub component as on IP27, calls
  `board_to_path` and `hwgraph_traverse` for both ends, and adds the "link" edge with
  `hwgraph_edge_add`, with the same three messages. Established.
- `klhwg_disabled_cpu_invent_info` (`6.5.30/unix.IP35` `0xc0000000000724bc`, 284 bytes)
  calls `klhwg_invent_alloc` for the same two records (36 bytes of class 1, 92 bytes of class 70)
  and fills the first as on IP27, with the same PRID warning, but leaves the second as allocated: no
  diagnostic value, text, processor number or `physid` is written into it before it is exported as
  "_diag_reason" (92 bytes). Established.
- `klhwg_baseio_inventory_add` (`6.5.30/unix.IP35` `0xc0000000000725d8`, 140 bytes)
  matches the IP27 routine except for the string block: it calls `klhwg_invent_alloc` (class 21, 32
  bytes) and `nvram_prom_version_get`, and adds and exports "_detail_invent". Established.
- `device_component_canonical_name_get` (`6.5.30/unix.IP35` `0xc0000000000454e8`, 144 bytes)
  writes "/hw/module/%M/slot/%s/pci/%d" with the module halfword (+14), a board name and `physid`
  with bit 4 cleared; there is no slot-name lookup. The board name comes from a call to
  `nic_name_convert` on the text at +153 for record version 2 or more; for older records it shows
  the same fault as IP27, copying "xwidget" into the output and then formatting with an unwritten
  stack buffer. As on IP27 the null-board test comes after the module load (at 0xc0000000000454e8),
  so it does not protect that load. Established.
- `xswitch_vertex_init` (`6.5.30/unix.IP35` `0xc00000000004b3ac`, 88 bytes)
  has the IP27 instructions: it calls `kmem_zalloc` for 32 zeroed bytes, `init_mutex` on them, and
  `hwgraph_info_add_LBL` to attach them to the vertex under "_xswitch_volunteer". Established.
- `router_guardians_set` (`6.5.30/unix.IP35` `0xc0000000000521e8`, 360 bytes)
  is the IP27 routine with the IP35 NASID position and the PE-brick check: for each compact node below
  `maxnodes` (gp+10284) it reads the doubleword at `0x9000000034000000` plus the NASID shifted left 33
  (address from gp+2256, the load at 0xc00000000005226c) and calls `chk_for_pebrick` for the start
  board when bits 12 to 15 equal `0xc`, and otherwise reads the node's directory. It walks the router
  boards with `find_lboard_class` for type `0x31` and calls `sub_c000000000052118` with a0 and each
  board. Established.
- `klhwg_device_disable_hints_add` (`6.5.30/unix.IP35` `0xc0000000000740f0`, 560 bytes)
  is the IP27 routine with `numnodes` from gp+10276, the IP35 tables and NASID position, and a call
  to `chk_for_pebrick` for a PE-brick node's first board. It calls `device_admin_table_init`, then
  `device_component_canonical_name_get` and `device_admin_table_update` ("DISABLE", "yes") for each
  disabled component of each non-duplicate class-`0x20` board. Established.

## Hub device callouts and PROM device records

Each node vertex carries the label "_node", whose value is the node's `pdinfo` record (+20824 of `nodepda_s`, added by `klhwg_add_node` above). Two fields of that record are read through the label: the compact node number as the signed halfword at +8 and the NASID as the signed halfword at +10. Linux has no equivalent record. The callout list lets other subsystems run a routine for each hub's crossbow widget when it is attached; the PROM device list maps a PROM character device vertex back to its hub vertex.

- `hubdev_register` (`6.5.30/unix.IP27` `0xc00000000004c06c`, 88 bytes)
  adds callout routine a0 to the head of `hubdev_callout_list` (the doubleword at gp-32344). It
  calls `kmem_zalloc` for a 16-byte record (flags 0), then under `hubdev_callout_mutex` (taken with
  `mutex_lock` and 25, released with `mutex_unlock`) stores the old list head at +8 of the record,
  the record as the new head, and a0 at +0 (the store at 0xc00000000004c0b0, in the delay slot of the
  unlock call). It returns nothing. Its one caller in the kernel is `hubspc_init`. Established.
- `hubdev_unregister` (`6.5.30/unix.IP27` `0xc00000000004c0c4`, 128 bytes)
  takes `hubdev_callout_mutex` and walks `hubdev_callout_list` through the doublewords at +8 for a
  record whose doubleword at +0 equals a0 (the compare at 0xc00000000004c0f0). On a match it unlinks
  the record, calls `kmem_free` on it with 16, releases the mutex and returns 0; when no record
  matches it releases the mutex and returns 2. Established.
- `hubdev_docallouts` (`6.5.30/unix.IP27` `0xc00000000004c144`, 140 bytes)
  takes `hubdev_callout_mutex` and calls the routine at +0 of each record of `hubdev_callout_list`,
  in list order (newest first), with a0, the hub vertex, as its only argument (the call at
  0xc00000000004c17c). The first nonzero result ends the walk: it releases the mutex and returns
  that result. When every routine returns 0 it releases the mutex and returns 0. Its one caller is
  `io_xswitch_widget_init`. Established.
- `hubdev_cnodeid_get` (`6.5.30/unix.IP27` `0xc00000000004c20c`, 48 bytes)
  calls `hwgraph_info_get_LBL` with the vertex in a0, the label "_node" (string block address from
  gp-19248, plus 8) and a stack slot cleared beforehand, and returns the signed halfword at +8 of
  the record found, the compact node (the load at 0xc00000000004c230). The result pointer is not
  tested, so a vertex without the label reads address 8. Established.
- `hubdev_prombase_get` (`6.5.30/unix.IP27` `0xc00000000004c1d0`, 60 bytes)
  looks up "_node" the same way and returns the signed halfword at +10 of the record, the NASID
  (the load at 0xc00000000004c1f4), shifted left 32 and added to `0x9000000030000000` (the
  doubleword at gp-24488), the node's hub special space at offset `0x30000000`. Linux names that
  address `NODE_RBOOT_BASE`, the remote view of a node's boot PROM. Its caller `cpuprom_map` maps
  the PROM character device from that base. The result pointer is not tested. Established.
- `prominfo_add` (`6.5.30/unix.IP27` `0xc00000000006b84c`, 100 bytes)
  records that PROM device vertex a1 belongs to hub vertex a0. It calls `kmem_alloc` for a 16-byte
  record (flags 0), stores a1 as the word at +0 and a0 as the word at +4, and under
  `cpuprom_spinlock` (`mutex_spinlock`, then `mutex_spinunlock` with the level it returned) links
  the record at the head of the list whose head is the doubleword at gp+2816 (`0xc0000000014d2c70`,
  which has no symbol), with the old head at +8. Its caller `cpuprom_attach` passes the hub vertex
  and the character device it has just created. Established.
- `prominfo_del` (`6.5.30/unix.IP27` `0xc00000000006b8b0`, 120 bytes)
  under `cpuprom_spinlock` unlinks the first record whose first word (the device vertex) equals a0
  (the compare at 0xc00000000006b8dc). The record is not freed. It returns nothing. Established.
- `prominfo_nodeget` (`6.5.30/unix.IP27` `0xc00000000006b928`, 112 bytes)
  under `cpuprom_spinlock` returns the word at +4 (the hub vertex) of the first record whose first
  word equals a0 (the load at 0xc00000000006b994, in the delay slot of the branch back to the return),
  or 0 when none does. Its caller is `cpuprom_map`. Established.

On IP35 these routines differ as follows.

- `hubdev_register` (`6.5.30/unix.IP35` `0xc00000000006d47c`, 88 bytes)
  has the IP27 instructions: it calls `kmem_zalloc` for 16 bytes and `mutex_lock` on
  `hubdev_callout_mutex`, and pushes the record holding a0 onto `hubdev_callout_list`
  (the doubleword at gp-32368). Established.
- `hubdev_unregister` (`6.5.30/unix.IP35` `0xc00000000006d4d4`, 128 bytes)
  has the IP27 instructions: it unlinks and calls `kmem_free` on the first record holding a0 and
  returns 0, or returns 2 when none does. Established.
- `hubdev_docallouts` (`6.5.30/unix.IP35` `0xc00000000006d554`, 140 bytes)
  has the IP27 instructions: it calls each callout with a0 under `hubdev_callout_mutex` and returns
  the first nonzero result, or 0. Established.
- `hubdev_cnodeid_get` (`6.5.30/unix.IP35` `0xc00000000006d61c`, 48 bytes)
  has the IP27 instructions: it calls `hwgraph_info_get_LBL` for "_node" and returns the halfword at
  +8. Established.
- `hubdev_prombase_get` (`6.5.30/unix.IP35` `0xc00000000006d5e0`, 60 bytes)
  calls `hwgraph_info_get_LBL` for "_node", shifts the NASID at +10 left 33 and adds
  `0x9000000038000000` (the doubleword at gp+2968), so the IP35 PROM window is at offset
  `0x38000000` of the node's uncached space. Established.
- `prominfo_add` (`6.5.30/unix.IP35` `0xc000000000096a90`, 100 bytes)
  has the IP27 instructions with the list head at `0xc000000001644db8` (gp+5704) and
  `cpuprom_spinlock` at gp+5712: it calls `kmem_alloc` for 16 bytes, stores a1 and a0 as the words
  at +0 and +4, and links the record at the head under `mutex_spinlock`. Established.
- `prominfo_del` (`6.5.30/unix.IP35` `0xc000000000096af4`, 120 bytes)
  has the IP27 instructions: it calls `mutex_spinlock`, unlinks the first record whose first word
  equals a0 without freeing it, and calls `mutex_spinunlock`. Established.
- `prominfo_nodeget` (`6.5.30/unix.IP35` `0xc000000000096b6c`, 112 bytes)
  has the IP27 instructions: it calls `mutex_spinlock` and returns the word at +4 of the first record
  whose first word equals a0, or 0. Established.

The `hubdev_*` routines are the same instruction for instruction in the 6.5.22 and 6.5.30 `kernel.o` objects of each platform, relocations aside. No 6.5.22 object defining the `prominfo_*` routines is in the inventory.
