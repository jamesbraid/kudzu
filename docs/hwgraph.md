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
