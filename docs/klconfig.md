# KLCONFIG as the kernel reads it

The IP27 and IP35 PROMs leave a description of each node's hardware in that node's memory: a directory at a fixed offset, a configuration header, and a linked list of board records, each with an array of component records. The 6.5.30 IP27 and IP35 kernels never copy this description; they walk it in place through uncached addresses. This page states what the kernels expect to find there: the addresses they form, the offsets they read, the values they compare against, and what each lookup routine returns. IP30 has no KLCONFIG reader: none of the routines below is linked into `6.5.30/unix.IP30`.

Every fact is graded. "Established" means the whole routine body was read in the named build at the named address. The public Linux and OpenBSD SN headers corroborate the same records; where their offsets agree with the kernel's loads that is noted, and their field names are used as labels. The linked kernels' DWARF declares `lboard_s` with no members; the board record's full definition is in the DWARF of the `idbg.o` objects ([structs/lboard_s.md](structs/lboard_s.md)), and every board offset below agrees with it. No shipped DWARF defines the component, header, directory or GDA records, so those offsets come from the instructions alone.

## Addresses

A KLCONFIG pointer is a 32-bit offset from the start of the owning node's memory. The kernels turn an offset into a pointer the same way everywhere:

- the node number (NASID) comes from the record that holds the offset: bits 32 to 39 of its address on IP27, bits 33 to 39 on IP35;
- the NASID is shifted back into place (left 32 on IP27, left 33 on IP35) and added to the offset;
- the sum is masked to its low 40 bits (`0x000000ffffffffff`, formed as -1 shifted right 24) and ORed with `0x9600000000000000` (formed as -53 shifted left 57), the uncached window;
- an offset of zero means "no record" and yields a null pointer, never `0x9600000000000000`.

So a board list never leaves the node it starts on: the next record's NASID is always the current record's. Linux forms the same address (`NODE_OFFSET_TO_K1`, `TO_NODE_UNCAC`) and also treats a zero offset as the end (`KLCF_NEXT`). Established on both kernels from the lookup routines below.

## Finding a node's first board

The kernels reach a node's board list in three loads, inlined in each caller (`get_module_id` and `is_master_baseio` below are the shortest examples):

| Step | Address read | Width | Use | Grade |
|---|---|---|---|---|
| directory entry | node uncached base + `0x2048` (the base ORed with `0x2000`, then +72) | doubleword | offset of the configuration header from the node base; masked to 40 bits | established |
| header | node uncached base + that offset, then +20 | word | offset of the first board record; zero means the node has no boards | established |

The directory therefore sits at offset `0x2000` of each node and the configuration entry is the second 64-byte entry, with its offset field at +8 of the entry. Linux reads the same: `KLDIR_OFFSET` is `0x2000`, `KLI_KLCONFIG` is entry 1, `kldir_ent_t.offset` is at +8, and `kl_config_hdr_t.ch_board_info` is at +20. Established. The constants are in `KLCONFIG_DECODE.h` (`KLCONFIG_DIR_OFF`, `KLCONFIG_DIR_OFF_CONFIG_OFFSET`, `KLCONFIG_DIR_CONFIG_OFFSET_MASK`, `KLCONFIG_HDR_OFF_BOARD_INFO`). Re-read in `get_module_id` of `6.5.30/unix.IP35` (sha256 `412a1aae553a348376bc30cacbaf65044de1745d30222f02ec5f88297dcb1dfa`, 0xc00000000004538c): the node base ORed with 0x2000, a doubleword load at +0x48 masked with 0xffffffffff, then a word load at +0x14. The 64-byte entry size is not a constant the code uses; only the sum 0x48 is read.

On IP35 this path is taken only after a check: `get_module_id` and `is_master_baseio` first read the doubleword at `0x9000000034000000` plus the NASID shifted left 33, and when bits 12 to 15 of it equal `0xc` they call `chk_for_pebrick` instead of reading the node's own directory (see [IP35 differences](#ip35-differences)). What the value means is in [the IP35 node class nibble](#the-ip35-node-class-nibble).

## Board record

Offsets as the 6.5.30 IP27 and IP35 kernels read them. "Readers" names the routines on this page that load the field. `KLCONFIG_DECODE.h` carries the words of this table and of the component header, global data area, console and memory component tables below, and the board-class values from the serial-number table.

| Offset | Width | Linux name | Kernel use | Readers | Grade |
|---|---|---|---|---|---|
| +0 | word | `brd_next` | offset of the next board on the same node; zero ends the list | every `find_lboard*` routine | established |
| +5 | byte | `brd_type` | board type; the high four bits (`& 0xf0`) are the class | `find_lboard`, `find_lboard_class`, `find_lboard_slot_class`, `find_lboard_module_class` | established |
| +9 | byte | `brd_flags` | bit `0x20` marks the global master I/O board | `is_master_baseio` | established |
| +10 | byte | `brd_slot` | slot number | `find_lboard_slot_class`, `find_lboard_modslot` | established |
| +14 | signed halfword | `brd_module` | module number | `find_lboard_modslot`, `find_lboard_module`, `find_lboard_module_class`, `get_module_id` | established |
| +23 | byte | `brd_numcompts` | number of component offsets in use | `find_component` | established |
| +32 | signed halfword | `brd_nasid` | node number recorded by the PROM | `get_actual_nasid` | established |
| +36 | word array | `brd_compts` | component offsets, one word each, from index 0 | `find_component` | established |

The offsets agree with Linux's `lboard_t` and with the `idbg.o` definition, which also names +153 `brd_name` (32 bytes). Type values the kernels pass: 17 (`0x11`) for the node board in `get_module_id`; class compares take the caller's value masked with `0xf0`.

## Component record header

Every component record starts with the same header (Linux `klinfo_t`). Offsets read by the routines on this page:

| Offset | Width | Linux name | Kernel use | Grade |
|---|---|---|---|---|
| +0 | byte | `struct_type` | component kind; `find_component` matches it exactly. 2 is the hub (`get_actual_nasid`) | established |
| +2 | byte | `flags` | bit 0 set means the component is enabled (`get_actual_nasid`) | established |
| +24 | byte | `physid` | physical identifier; for a disabled hub, `get_actual_nasid` returns it as the node number | established |

## Lookup routines

All of these leaf routines are the same, instruction for instruction, in the 6.5.30 and 6.5.22 `kernel.o` objects and in the linked 6.5.30 kernel of the same platform (relocations aside); the only IP35 difference is the NASID position given under [Addresses](#addresses). The entries name the linked IP27 kernel; the table at the end lists the other builds.

- `find_lboard` (`6.5.30/unix.IP27` `0xc00000000001d8e4`, 100 bytes)
  takes a board pointer in a0 and a type in a1 and walks the list from that board: it returns the
  first board whose byte at +5 equals a1 exactly, or 0 when it reaches a zero next-offset (the word
  at +0) or is given a null pointer. The type byte is loaded at 0xc00000000001d90c and the next
  offset at 0xc00000000001d920. Each next pointer is formed as in [Addresses](#addresses) from the
  current record's NASID. Established.
- `find_lboard_class` (`6.5.30/unix.IP27` `0xc00000000001d948`, 108 bytes)
  makes the same walk as `find_lboard` but compares the byte at +5 masked with `0xf0` against a1
  masked with `0xf0`, so it returns the first board of the class (type load at 0xc00000000001d974,
  masked at 0xc00000000001d978). Returns 0 at the end of the list or for a null start. Established.
- `find_lboard_slot_class` (`6.5.30/unix.IP27` `0xc00000000001d9b4`, 120 bytes)
  walks from a0 and returns the first board whose class (byte at +5 masked with `0xf0`) equals a1
  masked with `0xf0` and whose byte at +10 equals a2. The slot compare (load at 0xc00000000001da0c)
  runs only when the class matches (load at 0xc00000000001d9f4). Returns 0 at the end of the list or
  for a null start. Established.
- `find_lboard_modslot` (`6.5.30/unix.IP27` `0xc00000000001db94`, 124 bytes)
  walks from a0 and returns the first board whose signed halfword at +14 equals a1 (compared as a
  32-bit difference) and whose byte at +10 equals a2; the slot is tested (load at
  0xc00000000001dbf0) only for a module match (load at 0xc00000000001dbbc). Returns 0 at the end of
  the list or for a null start. Established.
- `find_lboard_module` (`6.5.30/unix.IP27` `0xc00000000001dc10`, 104 bytes)
  walks from a0 and returns the first board whose signed halfword at +14 (loaded at
  0xc00000000001dc38) equals a1. Returns 0 at the end of the list or for a null start. Established.
- `find_lboard_module_class` (`6.5.30/unix.IP27` `0xc00000000001dc78`, 132 bytes)
  walks from a0 and returns the first board whose halfword at +14 equals a1 and whose byte at +5
  masked with `0xf0` equals a2 masked with `0xf0` (module load at 0xc00000000001dca4, type load at
  0xc00000000001dcd8). Returns 0 at the end of the list or for a null start. Established.
- `find_component` (`6.5.30/unix.IP27` `0xc00000000001da2c`, 328 bytes)
  takes a board in a0, a previous component pointer in a1 and a component type in a2, and returns
  the next component of that type after a1, or the first one when a1 is 0. With a1 of 0 it returns 0
  when the byte at +23 (component count) is zero or negative, and otherwise scans the word offsets
  at +36, +40, and so on, forming each pointer on the board's node, and returns the first whose
  first byte (`struct_type`) equals a2. With a nonzero a1 it first searches the offsets for a
  pointer equal to a1; when none matches (including a board with a count of zero) it calls `dprintf`
  with the string at `0xc000000001404ce8` ("find_component: Bad pointer: 0x%x\n", loaded from
  gp-20768) and a1, and returns 0. When a1 is found it scans the remaining offsets after it the same
  way. A count read as negative with a nonzero a1 returns 0 without the message. Established.
- `find_first_component` (`6.5.30/unix.IP27` `0xc00000000001db74`, 32 bytes)
  calls `find_component` with the board in a0, a1 of 0 and the type moved from a1 to a2, and returns
  its result. Established.
- `get_actual_nasid` (`6.5.30/unix.IP27` `0xc00000000001e440`, 100 bytes)
  returns the node number a board really answers on. It returns -1 for a null board, calls
  `find_first_component` for type 2 (the hub), and returns -1 when the board has no hub. When the
  hub's byte at +2 has bit 0 set (enabled) it returns the board's signed halfword at +32
  (`brd_nasid`); when bit 0 is clear it returns the hub's byte at +24 (`physid`). So a disabled
  hub's node is named by its physical identifier, not by the PROM-assigned number. Established.
- `get_module_id` (`6.5.30/unix.IP27` `0xc00000000001e744`, 120 bytes)
  takes a NASID in a0, finds the node's first board through the directory at node offset `0x2048`
  and the header word at +20 (see [Finding a node's first board](#finding-a-nodes-first-board)),
  calls `find_lboard` for type 17, and returns the found board's signed halfword at +14 (the module
  number), or -1 when there is no such board. A node with no board list passes a null start to
  `find_lboard`, which returns 0, so the result is -1. Established.
- `is_master_baseio` (`6.5.30/unix.IP27` `0xc00000000001dee0`, 280 bytes)
  takes a NASID, a module and a slot, and reports whether the board in that module and slot is the
  system's global master I/O board. It calls `find_lboard_modslot` on the node's own board list;
  when that finds nothing it looks up the node's crossbow peer (the compact node from
  `nasid_to_compact_node`, then `xbow_peer` at +18832 of that node's `nodepda_s`, the same path as
  `get_xbow_peer`) and, unless the peer is -1, searches the peer node's list with the same module
  and slot. It returns the found board's byte at +9 masked with `0x20` (nonzero for the global
  master), or 0 when neither list has the board. Established.
- `get_xbow_peer` (`6.5.30/unix.IP27` `0xc00000000001e7bc`, 44 bytes)
  takes a NASID, converts it to a compact node number through the halfword table
  `nasid_to_compact_node` (address loaded from gp-20752), and returns the signed halfword
  `xbow_peer` at +18832 of that node's `nodepda_s` (the load at 0xc00000000001e7e4). The node data
  pointer comes from the per-processor word `p_nodepda` (absolute `0xffffffffffffc400`, offset 1024
  of `pda_s`), whose first doubleword (`pernode_pdaindr`) points at the table of node data pointers
  indexed by compact node. The member names and offsets are from the kernel's DWARF. Established.

### IP35 lookups

- `find_component` (`6.5.30/unix.IP35` `0xc0000000000444b8`, 328 bytes)
  has the IP27 body with the IP35 NASID position for the component pointers (bits 33 to 39, shifted
  back left 33). It scans the word offsets from +36 for the first component whose type byte equals
  a2, after the one in a1 when a1 is nonzero, and calls `dprintf` with "find_component: Bad pointer:
  0x%x\n" when a1 is not among them. Established.
- `find_first_component` (`6.5.30/unix.IP35` `0xc000000000044600`, 32 bytes)
  has the same instructions as the IP27 routine: it calls `find_component` with a1 of 0 and the type
  moved to a2, and returns its result. Established.
- `get_actual_nasid` (`6.5.30/unix.IP35` `0xc000000000044f28`, 100 bytes)
  has the same instructions as the IP27 routine: it calls `find_first_component` for type 2 and
  returns the board's halfword at +32 for an enabled hub, the hub's byte at +24 for a disabled one,
  and -1 for a null board or a board without a hub. Established.
- `get_module_id` (`6.5.30/unix.IP35` `0xc00000000004538c`, 152 bytes)
  is the IP27 routine with the IP35 NASID shift and the PE-brick check: when bits 12 to 15 of the
  doubleword at `0x9000000034000000` plus the NASID shifted left 33 (address from gp+2256, the load
  at 0xc000000000045398) equal `0xc`, it calls `chk_for_pebrick` for the start board instead of
  reading the directory. It then calls `find_lboard` for type 17 and returns the module halfword at
  +14, or -1. Established.
- `is_master_baseio` (`6.5.30/unix.IP35` `0xc00000000004493c`, 352 bytes)
  differs from the IP27 routine in its search: it calls `find_lboard_module` with the module alone
  and ignores the slot argument. It searches the node's own list (or the `chk_for_pebrick` board for
  a PE-brick node), then, when nothing is found, the list of the node's `xbow_peer` (+34304 of
  `nodepda_s`, `0x8600`) unless that is -1, with the same PE-brick check. It returns the found
  board's byte at +9 masked with `0x20`, or 0. Established.
- `get_xbow_peer` (`6.5.30/unix.IP35` `0xc000000000045424`, 52 bytes)
  is the IP27 routine over the IP35 `nasid_to_compact_node` (address from gp-23376) with `xbow_peer`
  at +34304 (`0x8600`) of `nodepda_s`, loaded at 0xc000000000045454. Established.

### The same routines in the other builds

The 6.5.22 and 6.5.30 `kernel.o` objects were compared with the linked 6.5.30 kernel by instruction text, with addresses and relocation targets removed; IP35 rows were read in full for the NASID change.

| Routine | Binary | Address | Size | Note | Label |
|---|---|---|---|---|---|
| `find_lboard` | `6.5.30/unix.IP35` | `0xc000000000044370` | 100 | board walk by exact type at +5; NASID taken from bits 33 to 39 (`dsrl32` by 1, `andi 0x7f`, `dsll32` by 1) | Established |
| `find_lboard_class` | `6.5.30/unix.IP35` | `0xc0000000000443d4` | 108 | board walk by class (+5 masked `0xf0`); IP35 NASID position | Established |
| `find_lboard_slot_class` | `6.5.30/unix.IP35` | `0xc000000000044440` | 120 | board walk by class and slot byte at +10; IP35 NASID position | Established |
| `find_lboard_modslot` | `6.5.30/unix.IP35` | `0xc000000000044620` | 124 | board walk by module halfword at +14 and slot byte at +10; IP35 NASID position | Established |
| `find_lboard_module` | `6.5.30/unix.IP35` | `0xc00000000004469c` | 104 | board walk by module halfword at +14; IP35 NASID position | Established |
| `find_lboard_module_class` | `6.5.30/unix.IP35` | `0xc000000000044704` | 132 | board walk by module at +14 and class at +5; IP35 NASID position | Established |
| `find_lboard` | `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4764` | 100 | board walk by exact type at +5; same instructions as the IP27 linked kernel | Established |
| `find_lboard` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4534` | 100 | board walk by exact type at +5; unchanged from 6.5.30 | Established |
| `find_lboard` | `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` | `0x41f0` | 100 | board walk by exact type at +5; same instructions as the IP35 linked kernel | Established |
| `find_lboard` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0x3fc0` | 100 | board walk by exact type at +5; unchanged from 6.5.30 | Established |
| `find_lboard_class` | `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` | `0x47c8` | 108 | board walk by class at +5; same instructions as the IP27 linked kernel | Established |
| `find_lboard_class` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4598` | 108 | board walk by class at +5; unchanged from 6.5.30 | Established |
| `find_lboard_class` | `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` | `0x4254` | 108 | board walk by class at +5; same instructions as the IP35 linked kernel | Established |
| `find_lboard_class` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0x4024` | 108 | board walk by class at +5; unchanged from 6.5.30 | Established |
| `find_lboard_slot_class` | `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4834` | 120 | board walk by class at +5 and slot at +10; same instructions as the IP27 linked kernel | Established |
| `find_lboard_slot_class` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4604` | 120 | board walk by class and slot; unchanged from 6.5.30 | Established |
| `find_lboard_slot_class` | `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` | `0x42c0` | 120 | board walk by class and slot; same instructions as the IP35 linked kernel | Established |
| `find_lboard_slot_class` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0x4090` | 120 | board walk by class and slot; unchanged from 6.5.30 | Established |
| `find_lboard_modslot` | `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4a14` | 124 | board walk by module at +14 and slot at +10; same instructions as the IP27 linked kernel | Established |
| `find_lboard_modslot` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x47e4` | 124 | board walk by module and slot; unchanged from 6.5.30 | Established |
| `find_lboard_modslot` | `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` | `0x44a0` | 124 | board walk by module and slot; same instructions as the IP35 linked kernel | Established |
| `find_lboard_modslot` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0x4270` | 124 | board walk by module and slot; unchanged from 6.5.30 | Established |
| `find_lboard_module` | `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4a90` | 104 | board walk by module at +14; same instructions as the IP27 linked kernel | Established |
| `find_lboard_module` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4860` | 104 | board walk by module at +14; unchanged from 6.5.30 | Established |
| `find_lboard_module` | `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` | `0x451c` | 104 | board walk by module at +14; same instructions as the IP35 linked kernel | Established |
| `find_lboard_module` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0x42ec` | 104 | board walk by module at +14; unchanged from 6.5.30 | Established |
| `find_lboard_module_class` | `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` | `0x4af8` | 132 | board walk by module at +14 and class at +5; same instructions as the IP27 linked kernel | Established |
| `find_lboard_module_class` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x48c8` | 132 | board walk by module and class; unchanged from 6.5.30 | Established |
| `find_lboard_module_class` | `6.5.30/usr/cpu/sysgen/IP35boot/kernel.o` | `0x4584` | 132 | board walk by module and class; same instructions as the IP35 linked kernel | Established |
| `find_lboard_module_class` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0x4354` | 132 | board walk by module and class; unchanged from 6.5.30 | Established |
| `find_lboard` | `6.5.22/unix.IP27` | `0xc00000000001d6b4` | 100 | board walk by exact type at +5; same bytes as the 6.5.30 linked kernel (SHA-256 prefix ee11644838de), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_class` | `6.5.22/unix.IP27` | `0xc00000000001d718` | 108 | board walk by class (+5 masked with 0xf0); same bytes as the 6.5.30 linked kernel (SHA-256 prefix 6bcb70b8a999), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_slot_class` | `6.5.22/unix.IP27` | `0xc00000000001d784` | 120 | board walk by class and slot byte at +10; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 1a6a04b9ac5e), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_modslot` | `6.5.22/unix.IP27` | `0xc00000000001d964` | 124 | board walk by module halfword at +14 and slot byte at +10; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 1a1e492680c1), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_module` | `6.5.22/unix.IP27` | `0xc00000000001d9e0` | 104 | board walk by module halfword at +14; same bytes as the 6.5.30 linked kernel (SHA-256 prefix add2287b5acd), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_module_class` | `6.5.22/unix.IP27` | `0xc00000000001da48` | 132 | board walk by module halfword at +14 and class at +5; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 1cd5e0490f89), at a different address; no calls, address constants or gp accesses in the body | Established |
| `get_master_bridge_base` | `6.5.22/unix.IP27` | `0xc00000000001f74c` | 8 | returns the doubleword at gp-32688, the master bridge base; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 13856258530b), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `nasid_to_region` | `6.5.22/unix.IP27` | `0xc00000000002734c` | 24 | returns a0 when a gp-relative flag word is nonzero, and a0 shifted right 3 (arithmetic) when it is zero; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 5bea195aac3f), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `hub_slot_to_crossbow` | `6.5.22/unix.IP27` | `0xc00000000003c8c0` | 36 | returns 0 for hub slot class 0x50, otherwise (a0 - 1) masked with 1; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 11261d777a5d), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard` | `6.5.22/unix.IP35` | `0xc000000000044140` | 100 | board walk by exact type at +5; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 67280654a1a1), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_class` | `6.5.22/unix.IP35` | `0xc0000000000441a4` | 108 | board walk by class (+5 masked with 0xf0); same bytes as the 6.5.30 linked kernel (SHA-256 prefix 818af0d59444), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_slot_class` | `6.5.22/unix.IP35` | `0xc000000000044210` | 120 | board walk by class and slot byte at +10; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 3eefb13db501), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_modslot` | `6.5.22/unix.IP35` | `0xc0000000000443f0` | 124 | board walk by module halfword at +14 and slot byte at +10; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 049110a0a765), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_module` | `6.5.22/unix.IP35` | `0xc00000000004446c` | 104 | board walk by module halfword at +14; same bytes as the 6.5.30 linked kernel (SHA-256 prefix eef758bfb48e), at a different address; no calls, address constants or gp accesses in the body | Established |
| `find_lboard_module_class` | `6.5.22/unix.IP35` | `0xc0000000000444d4` | 132 | board walk by module halfword at +14 and class at +5; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 450484c16a35), at a different address; no calls, address constants or gp accesses in the body | Established |
| `nasid_to_region` | `6.5.22/unix.IP35` | `0xc00000000004f628` | 24 | returns a0 when a gp-relative flag word is nonzero, and a0 shifted right 1 (arithmetic) when it is zero; same bytes as the 6.5.30 linked kernel (SHA-256 prefix f1b245a1dd4c), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `get_master_bridge_base` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x65cc` | 8 | returns the doubleword in a gp-relative slot, the master bridge base; same bytes as the 6.5.30 object (SHA-256 prefix 13856258530b) at a different offset; relocations not compared | Inferred |
| `nasid_to_region` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0xe1cc` | 24 | returns a0 when a gp-relative flag word is nonzero, and a0 shifted right 3 (arithmetic) when it is zero; same bytes as the 6.5.30 object (SHA-256 prefix 5bea195aac3f) at a different offset; relocations not compared | Inferred |
| `hub_slot_to_crossbow` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x23740` | 36 | returns 0 for hub slot class 0x50, otherwise (a0 - 1) masked with 1; same bytes as the 6.5.30 object (SHA-256 prefix 11261d777a5d) at a different offset; relocations not compared | Inferred |
| `nasid_to_region` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0xf4a8` | 24 | returns a0 when a gp-relative flag word is nonzero, and a0 shifted right 1 (arithmetic) when it is zero; same bytes as the 6.5.30 object (SHA-256 prefix f1b245a1dd4c) at a different offset; relocations not compared | Inferred |

## Node numbering at boot

The kernels number nodes from the global data area (GDA) the PROM leaves on the boot node, then fill three halfword tables. In `6.5.30/unix.IP27` they are `nasid_to_compact_node` (`0xc0000000014d4c80`, 256 entries), `compact_to_nasid_node` (`0xc0000000014d4e80`, 512 bytes after it, 64 entries) and `cpuid_to_compact_node` (`0xc0000000014d4f00`, 640 bytes after it, 128 entries); the routines reach all three from the address of the first, held at gp-20728. In `6.5.30/unix.IP35` the same three tables sit at `0xc000000001646f00`, `0xc000000001647000` and `0xc000000001647100` (offsets 0, 256 and 512 from the first), with 128, 128 and 512 entries. An entry of -1 means "none".

The GDA is found through the fourth directory entry: its pointer field is the doubleword at node offset `0x20d0` (the directory base ORed with `0x2000`, then +208). The kernels use that doubleword as an address without masking it, unlike the configuration entry's offset. Fields read:

| Offset | Width | Linux name | Kernel use | Grade |
|---|---|---|---|---|
| +128 | signed halfword array | `g_nasidtable` | NASID of each compact node in order; the first -1 ends the list. IP27 reads at most 64 entries, IP35 at most 128 | established |
| +388 | word | none | IP35 only: `0x5045626b` (ASCII "PEbk") says a PE-brick table follows | established |
| +392 | four pairs of signed halfwords | none | IP35 only: per pair, the PE-brick NASID at +0 and its host NASID at +2; a first halfword of -1 ends the table | established |

Linux's `gda_t` places `g_nasidtable` at +128 too and uses directory entry 3 (`KLI_GDA`) with its `pointer` field at +16. Linux has no IP35 support, so the +388 and +392 fields have no public name.

- `cpu_node_probe` (`6.5.30/unix.IP27` `0xc00000000001f2f8`, 556 bytes)
  takes a pointer to the boot CPU mask (a0) and a pointer to the node count (a1). It sets every
  entry of the three tables to -1, reads its own NASID with `get_nasid`, finds the GDA through the
  directory as above and clears the word at a1. For each GDA NASID entry until the first -1 (at most
  64) it stores the NASID as compact node i in `compact_to_nasid_node`, stores i at that NASID in
  `nasid_to_compact_node`, adds one to the node count and calls `do_cpumask` with the compact node,
  the NASID, the mask and a stack word that collects the highest processor number. It then checks
  that processor numbers rise with compact node: for each processor number up to the highest, a node
  number below the current one is accepted only when the node count equals the current node plus
  one, or when that processor's bit in the boot mask is clear; otherwise it calls `assfail`
  (condition string at `0xc000000001404ff0` from gp-20720, line 185) and carries on. It returns the
  highest processor number plus one, sign-extended from 16 bits. Established.
- `do_cpumask` (`6.5.30/unix.IP27` `0xc00000000001f524`, 520 bytes)
  takes a compact node (a0), a NASID (a1), the CPU mask (a2) and a pointer to the highest processor
  number (a3). It finds the NASID's first board (see [Finding a node's first
  board](#finding-a-nodes-first-board)) and calls `find_lboard` for type 17; when the node has no
  such board it calls `assfail` (line 227). For each type-17 board on the node, and for each
  component of type 1 (a processor, from `find_first_component` then `find_component`), it reads the
  processor number as the signed halfword at +30 of the component, calls `assfail` (line 236) when
  that is 128 or more, and records the compact node at that number in `cpuid_to_compact_node` only
  when the board's halfword at +32 equals what `get_actual_nasid` returns for the board, so
  processors behind a disabled hub are not mapped. It raises the highest-number word when the number
  is larger. When the component's byte at +2 has bit 0 set (enabled) it sets the processor's bit in
  the mask, calling `assfail` (line 250) first if the bit was already set, and counts it. It returns
  the count of enabled processors. Established.
- `get_nasid` (`6.5.30/unix.IP27` `0xc00000000001d664`, 28 bytes)
  loads the address `0x9200000001600000` from gp-24528, reads the doubleword at that address (the
  load at 0xc00000000001d668) and returns bits 8 to 16 (shift right 8, mask `0x1ff`), sign-extended
  from 16 bits. Linux reads the node identifier from the same hub register (`NI_STATUS_REV_ID` at
  hub offset `0x600000`, `NSRI_NODEID_SHFT` 8, a 9-bit mask). Established.
- `get_slice` (`6.5.30/unix.IP27` `0xc00000000001d680`, 16 bytes)
  loads the address `0x9200000001000020` from gp-24520, reads the doubleword at that address (the
  load at 0xc00000000001d684) and returns its low 32 bits sign-extended. That is offset `0x20` of
  the local hub's processor interface (Linux `PI_CPU_NUM`), so the result is the calling processor's
  slice on its node. Established for the access; the register name is from Linux.
- `nasid_to_compact_nodeid` (`6.5.30/unix.IP27` `0xc00000000001f734`, 20 bytes)
  returns the signed halfword at index a0 of `nasid_to_compact_node` (address from gp-20728, the
  load at 0xc00000000001f744), with no bounds check. Established.
- `compact_to_nasid_nodeid` (`6.5.30/unix.IP27` `0xc00000000001f748`, 24 bytes)
  returns the signed halfword at index a0 of `compact_to_nasid_node` (512 bytes past the address at
  gp-20728; the load at 0xc00000000001f75c), with no bounds check. Established.
- `cnode_exists` (`6.5.30/unix.IP27` `0xc00000000001f7c0`, 60 bytes)
  returns 1 when a0 is from 0 to 63 and `compact_to_nasid_node` at that index (the load at
  0xc00000000001f7e0) is not -1, and 0 otherwise. Established.
- `get_lowest_nasid` (`6.5.30/unix.IP27` `0xc00000000001f760`, 96 bytes)
  returns the smallest entry of `compact_to_nasid_node` over the first `maxnodes` entries (the word
  at gp+7348, loaded at 0xc00000000001f760; entries loaded at 0xc00000000001f78c). With `maxnodes`
  below 2 it returns entry 0 unchanged. Entries of -1 are not skipped, so it relies on `maxnodes`
  covering only filled entries. Established.
- `get_cpu_cnode` (`6.5.30/unix.IP27` `0xc00000000001e308`, 20 bytes)
  returns the signed halfword at index a0 of `cpuid_to_compact_node` (address from gp-20744, the
  load at 0xc00000000001e318), with no bounds check. Established.
- `get_compact_nodeid` (`6.5.30/unix.IP27` `0xc000000000027144`, 52 bytes)
  calls `get_nasid` and returns `nasid_to_compact_node` at that NASID (address from gp-20528): the
  calling processor's compact node. Established.

### IP35 differences

- `cpu_node_probe` (`6.5.30/unix.IP35` `0xc00000000004648c`, 768 bytes)
  does what the IP27 routine does with the IP35 table sizes (128 nodes, 512 processors) and NASID
  position, then handles PE-bricks. When the GDA word at +388 is `0x5045626b`, it reads up to four
  pairs at +392. A pair whose first halfword is -1 ends the walk. A pair whose host halfword is -1
  makes it call `printf` with "No host node for pebrick at nasid %d\n" and the PE-brick NASID, and
  go on to the next pair. Otherwise it appends an entry to `kl_pebricks` (the address from gp-23336;
  each entry is 8 bytes: the word 1 at +0, the PE-brick NASID at +4, the host NASID at +6), adds one
  to `num_pebricks` (the word at gp+10280), and gives the PE-brick a compact node number of the node
  count plus the pair index in both directions of the table pair. The node count itself is not
  raised, so PE-bricks get compact numbers past the counted nodes. When the magic is absent it reads
  the doubleword at `0x9000000014000000` (address from gp+2296), and when bits 12 to 15 of it equal
  8 it prints "IP35 prom is out of sync with Irix kernel.\n" and "PE-Brick support not available.\n"
  and carries on. The processor-order check and its `assfail` (line 185) follow as on IP27, and it
  returns the highest processor number plus one. Established.
- `do_cpumask` (`6.5.30/unix.IP35` `0xc00000000004678c`, 556 bytes)
  is the IP27 routine with three changes: it first reads the doubleword at `0x9000000034000000` plus
  the NASID shifted left 33 (address from gp+2256, the load at 0xc0000000000467b8) and, when bits 12
  to 15 equal `0xc`, calls `chk_for_pebrick` for its first board instead of reading the node's own
  directory; it allows processor numbers up to 511; and it records them in the IP35
  `cpuid_to_compact_node`. The `assfail` lines are 227, 236 and 250 as on IP27. Established.
- `chk_for_pebrick` (`6.5.30/unix.IP35` `0xc000000000046384`, 264 bytes)
  takes a NASID and scans the first `num_pebricks` entries of `kl_pebricks` for one whose PE-brick
  halfword (entry offset 4) equals it. For a match it walks the host node's board list (the host
  NASID, entry offset 6, through that node's directory) and returns the first board whose byte at +4
  (`struct_type`) has bit 0 clear; Linux names bit 0 of that byte a local board, so the result is
  the first board the host lists as remote. A match whose host has no boards, or whose walk finds
  none, moves on to the next entry. When no entry gives a board it calls `cmn_err` with level 0 and
  "chk_for_pebrick: invalid klconfig\n" and returns 0. Established; "remote" is inferred from the
  Linux names `LOCAL_BOARD` and `REMOTE_BOARD`.
- `get_nasid` (`6.5.30/unix.IP35` `0xc000000000041434`, 28 bytes)
  loads the address `0x9200000001600000` from gp+2232 and returns bits 32 to 39 of the doubleword at
  that address (the load at 0xc000000000041438), sign-extended from 16 bits. The IP35 node
  controller keeps the node number higher in that register than the IP27 hub. Established for the
  access.
- `get_slice` (`6.5.30/unix.IP35` `0xc000000000041450`, 16 bytes)
  reads the doubleword at `0x9200000001000020` (address from gp+2240, the load at
  0xc000000000041454) and returns its low 32 bits, as on IP27. Established.
- `nasid_to_compact_nodeid` (`6.5.30/unix.IP35` `0xc0000000000469c0`, 20 bytes)
  returns `nasid_to_compact_node` (address from gp-23344, the load at 0xc0000000000469d0) at index
  a0, with no bounds check. Established.
- `compact_to_nasid_nodeid` (`6.5.30/unix.IP35` `0xc0000000000469d4`, 24 bytes)
  returns `compact_to_nasid_node` (256 bytes past the address at gp-23344; the load at
  0xc0000000000469e8) at index a0, with no bounds check. Established.
- `cnode_exists` (`6.5.30/unix.IP35` `0xc000000000046a4c`, 64 bytes)
  returns 1 when a0 is from 0 to 127 and `compact_to_nasid_node` at that index (the load at
  0xc000000000046a70) is not -1, and 0 otherwise. Established.
- `get_lowest_nasid` (`6.5.30/unix.IP35` `0xc0000000000469ec`, 96 bytes)
  is the IP27 routine over the IP35 table (entries at offset 256 from the address at gp-23344,
  loaded at 0xc000000000046a18), bounded by `maxnodes` at gp+10284. Established.
- `get_cpu_cnode` (`6.5.30/unix.IP35` `0xc000000000044df0`, 20 bytes)
  returns `cpuid_to_compact_node` (address from gp-23368, the load at 0xc000000000044e00) at index
  a0. Established.
- `get_compact_nodeid` (`6.5.30/unix.IP35` `0xc00000000004e864`, 52 bytes)
  calls `get_nasid` and returns `nasid_to_compact_node` (address from gp-23080) at that NASID.
  Established.

The 6.5.22 objects carry the same instructions for all of these routines in both platforms; the only difference found is in the 6.5.22 IP35 `chk_for_pebrick`, whose message string sits 856 bytes into its string block instead of 864.

## The IP35 node class nibble

Both IP35 kernels read one 4-bit field, bits 12 to 15 of a doubleword, from two uncached addresses, and branch on its value. The two addresses differ only in address bit 29: `0x9000000034000000` is indexed by node (the NASID shifted left 33 is added), `0x9000000014000000` is read for the local node with no NASID added. The pool words are at `0xc000000001644040` (gp+2256) and `0xc000000001644068` (gp+2296) in `6.5.30/unix.IP35`, and at `0xc00000000159a0d8` (gp+232) and `0xc00000000159a100` (gp+272) in `6.5.22/unix.IP35`.

Every load of either pool word was found by scanning the whole disassembly of each kernel for loads from the gp slot. In `6.5.30` the node-indexed address has 75 load sites in 60 routines, and each is within reach of a shift left 32 by 1 of the NASID. The local address has 24 load sites in 18 routines. `6.5.22` has the same 60 routines for the node-indexed address (74 sites) and the same 18 for the local one, except that `init_all_devices` takes the place of `cn_init`. The only values any site compares the field with are `0x4000`, `0x8000` and `0xc000` (as `addiu 16384`, `dli 0x8000` and `dli 0xc000`, or an `xori` with `0x4000`); every other value takes the default path. Established for the reader census and the compared values.

What the kernel does for each value (all in `6.5.30/unix.IP35`, read in full unless stated):

| Field | Where it is read | Effect |
|---|---|---|
| `0xc` | the node-indexed address, in `get_module_id`, `is_master_baseio`, `do_cpumask`, `get_board_name`, `slot_getsize`, `klhwg_add_node` and the other routines that start a board walk | the first board comes from `chk_for_pebrick` instead of the node's own directory. This is the PE-brick case already described above |
| `0x8` | the local address, in `cpu_node_probe` (`0xc000000000046704`), when the PE-brick table magic is absent | prints "IP35 prom is out of sync with Irix kernel.\n" and "PE-Brick support not available.\n" |
| `0x8`, `0x4` or `0xc` | the node-indexed address, in `set_sn12_type` (`0xc000000000041550`, the load at `0xc00000000004157c`) | the routine returns at once when the byte at +128 of its per-node table (base from gp-23440) is already non-zero; otherwise it stores 1 in that byte (the store sits in the delay slot of the first compare, so it happens for every value of the field) and then returns without asking the system controller. For any other value it calls `elsc_cbrick_type_get` and stores a type byte at +0 of the table entry: 55 gives 1, 56 gives 2, 57 gives 3, 48 gives 4, 81 gives 5; a negative return logs through `cmn_err` at level 4 and leaves the type alone; other returns leave it alone. `get_chimera_type` (`0xc00000000004167c`) returns that type byte, calling `cmn_err` and then `set_sn12_type` first when the flag byte is zero. So the type stays 0 on a node whose field is `0x4`, `0x8` or `0xc` |
| `0x4`, `0x8` | the node-indexed address, in `sc_command_interp` (loads at `0xc0000000000d0024`, `0xc0000000000d0064` and `0xc0000000000d00d8`) | only when the second argument is 4; the routine then sets that argument to 0 before it joins the shared tail (`0xc0000000000cff44`), which shifts it left 28 into the word handed to `sc_open`, when any of these holds: the field is `0x4`; `get_chimera_type` returns 3; the field is `0x8` and the sixth argument is 0, 14 or 15; the sixth argument is 0 and the chimera type is non-zero; the sixth argument is 12 or 15 and the chimera type is non-zero. Otherwise it keeps the 4 (a sixth argument of 1 never triggers the `0x8` or chimera tests). The tests were read at the three loads; the meaning of the arguments is not named here |
| `0x8`, `0x4` | the node-indexed address, in `ioc3_attach` | with the vertex name "Ibrick/xtalk/15/pci/4", the sub-device enable argument is `0xb1` for `0x8`, `0x79` for `0x4` and `0x91` otherwise (ip35-ioc3.md has the surrounding routine) |
| `0x4` | the local address, in `ioc3_subdev_enabled` (`0xc0000000000c62c4`) | the default enable mask is `0xff`; for any other value it is `0xdf` |
| `0x4` | the local address, in `ioc3_attach` (`0xc0000000000c55d4`) | calls `pckm_attach`, the PC keyboard and mouse attach |
| `0x4` | the local address, in `cn_init` (`0xc0000000000dca00`) | when the word at gp-32476 is non-zero and the field is `0x4`, the call to `gfx_earlyinit` is skipped; otherwise it is made |
| `0x4` | the local address, in `rad_init` (`0xc00000000019cd60`) | stores 1 when the field is `0x4` and 0 otherwise as the word at gp+6524, then registers the PCI driver |
| `0x4` | the local address, in `scsi_ctlr_nums_add` (first load, `0xc00000000004cadc`) | stores 2 as the word at gp-25004 when the field is `0x4` and 6 otherwise |

The other readers (the iobrick EEPROM and system-controller helpers, `mactoh`, `get_host_id`, `main`, `syssgi`, `fake_a_part_number`, `fake_a_product_name`, `set_leds`, the error-save and hardware-graph routines) were found by the scan and compare the field with the same three constants; their individual effects are not described here. Their loads are in `docs/inventory/functions.tsv` under the routines named.

Reading: the field sorts a node into a class before any system-controller or directory access. `0xc` is a PE-brick node (established by the routines above). `0x8` is the value a PROM that lacks the PE-brick table leaves (inferred from the `cpu_node_probe` message, which names the PROM). `0x4` is the value on a platform where `set_sn12_type` does not query the system controller, the PC keyboard and mouse attach, and the graphics console start is skipped (inferred from those effects; the code does not name the platform). Which register the doublewords are: the node's class register at offset `0x4000000` of its local register space, which the IP35 CPU PROM reads at the same two addresses (established; [ip35-node-class-and-link-checks.md](ip35-node-class-and-link-checks.md) has the PROM side, the class values and the reasons for naming `0x4` the Fuel class, which are inferred). What drives the four bits on the hardware, and what class `0xc` is beyond its PE-brick use: unknown. The IP53 and IP59 node boards are outside classes `0x4`, `0x8` and `0xc` ([ip35-node-class-and-link-checks.md](ip35-node-class-and-link-checks.md)). The routes tried were the 6.5.30 and 6.5.22 IP35 kernels (every reader, above), the IP35 CPU PROM 6.210 and the OpenBSD SGI port's reader of the same register.

## Node and processor lookups

Small routines the rest of the kernel uses to turn compact nodes, NASIDs and slices into processor numbers and KLCONFIG records. The per-node fields they read are members of `nodepda_s` in the IP27 kernel's DWARF: `node_first_cpu` (signed halfword at +18774) and `node_num_cpus` (unsigned halfword at +18776). A node's `nodepda_s` is found as in `get_xbow_peer`: the doubleword at absolute `0xffffffffffffc400` (`p_nodepda` of the running processor's `pda_s`), then its first doubleword (the table of node data pointers), indexed by compact node times 8.

- `cpu_enabled` (`6.5.30/unix.IP27` `0xc000000000027228`, 56 bytes)
  returns 0 for processor -1, and otherwise 1 when bit a0 of `boot_cpumask` (address from gp-20560;
  bit a0 mod 64 of the doubleword at a0 shifted right 6, times 8, loaded at 0xc00000000002724c) is
  set and 0 when it is clear.
  Established.
- `cnodetocpu` (`6.5.30/unix.IP27` `0xc000000000026fc8`, 176 bytes)
  returns the first enabled processor of compact node a0. It reads `node_first_cpu` and returns -1
  when that is -1; otherwise it tries `node_first_cpu`, `node_first_cpu` + 1 and so on,
  `node_num_cpus` times (re-reading the count each pass), calling `cpu_enabled` on each, and returns
  the first number `cpu_enabled` accepts (sign-extended from 16 bits). It returns -1 when the count
  is zero or no processor is enabled. Established.
- `cnode_slice_to_cpuid` (`6.5.30/unix.IP27` `0xc000000000027260`, 180 bytes)
  returns the processor of compact node a0 that sits on slice a1. It returns -1 when
  `node_first_cpu` (loaded at 0xc000000000027284) is negative or `node_num_cpus` is zero. Otherwise,
  for at most `node_num_cpus` processors from `node_first_cpu`, it calls `cpu_enabled` on each, skips
  the ones it rejects and, for an enabled one,
  reads its `pda_s` through `pdaindr` (the doubleword at +8 of the 16-byte entry for that processor,
  `pdaindr` being 16 bytes past `boot_cpumask`) and compares the byte `p_slice` at +1312 (loaded at
  0xc0000000000272c4) with a1,
  returning the processor number on a match. A processor number that turns negative as a halfword, or
  running out of processors, returns -1. Established.
- `cnode_subnode_to_cpu` (`6.5.30/unix.IP27` `0xc000000000027078`, 204 bytes)
  takes a compact node (a0) and a subnode (a1). It returns -1 when `node_first_cpu` is -1 or
  `node_num_cpus` is zero (`node_first_cpu` loaded at 0xc00000000002709c). Otherwise it walks the
  node's processors as `cnodetocpu` does and calls `cpu_enabled` on each, returning the first enabled
  one only when a1 is 0. For a nonzero a1 the walk never returns a processor (a1 is
  kept in s4 and never changed), so the result is -1. The IP27 `klhwg_add_node` calls it only for
  subnode 0. Established.
- `nasid_slice_to_cpuinfo` (`6.5.30/unix.IP27` `0xc00000000001e4a4`, 208 bytes)
  returns the KLCONFIG processor component of NASID a0 on slice a1. It finds the node's first board
  through the directory (see [Finding a node's first board](#finding-a-nodes-first-board)), calls
  `find_lboard` for type 17, and returns 0 when there is none. It then takes components of type 1
  (`find_first_component`, then `find_component`) and returns the first whose byte at +24 (`physid`)
  equals a1, or 0 when none does. Only the first type-17 board is searched. Established.
- `nasid_to_region` (`6.5.30/unix.IP27` `0xc000000000027600`, 24 bytes)
  returns a0 unchanged when the word at gp-32628 (the `is_fine_dirmode` result `mlreset` stores,
  loaded at 0xc000000000027600) is nonzero, and a0 shifted right 3 (arithmetic) when it is zero. So in fine directory mode each node is
  its own region and in coarse mode eight nodes share one, as Linux's `NASID_TO_FINEREG` and
  `NASID_TO_COARSEREG` also compute. Established.
- `memory_present` (`6.5.30/unix.IP27` `0xc00000000002a160`, 36 bytes)
  shifts the physical address in a0 right 14 (the 16 KB page number), truncates it to 32 bits, calls
  `ckphyspnum`, and returns 1 when that returns 0 and 0 otherwise. Established for the mechanics;
  that a zero return from `ckphyspnum` means a valid page is inferred from this use.
- `hub_slot_to_crossbow` (`6.5.30/unix.IP27` `0xc00000000003cbc0`, 36 bytes)
  takes a hub slot number and returns 0 when its class (a0 masked with `0xf0` at 0xc00000000003cbc4)
  is `0x50`, and
  otherwise (a0 - 1) masked with 1. So for slots outside class `0x50` odd slot numbers map to crossbow
  0 and even ones to crossbow 1. Established.

### IP35 node and processor lookups

On IP35 `node_first_cpu` is at +34198 and `node_num_cpus` at +34200 of `nodepda_s`, `p_slice` at +1320 of `pda_s`, and `pdaindr` sits 64 bytes past `boot_cpumask` (`0xc000000001647568` and `0xc000000001647528`), because the processor mask is 512 bits.

- `cpu_enabled` (`6.5.30/unix.IP35` `0xc00000000004e948`, 56 bytes)
  has the IP27 instructions over the IP35 `boot_cpumask` (address from gp-23120, the load at
  0xc00000000004e96c): 0 for processor -1, otherwise bit a0 of the mask. Established.
- `cnodetocpu` (`6.5.30/unix.IP35` `0xc00000000004e694`, 192 bytes)
  is the IP27 routine with the IP35 offsets (`node_first_cpu` loaded at 0xc00000000004e6bc): it
  calls `cpu_enabled` on up to `node_num_cpus` processors from `node_first_cpu` and returns the first
  enabled one, or -1. Established.
- `cnode_slice_to_cpuid` (`6.5.30/unix.IP35` `0xc00000000004e980`, 188 bytes)
  is the IP27 routine with the IP35 offsets: it calls `cpu_enabled` on each of the node's processors
  and returns the first enabled one whose `p_slice` byte (+1320 of its `pda_s`, loaded at
  0xc00000000004e9ec) equals a1, or -1. Established.
- `cnode_subnode_to_cpu` (`6.5.30/unix.IP35` `0xc00000000004e754`, 272 bytes)
  differs from the IP27 routine: for each processor of the node from `node_first_cpu` (loaded at
  0xc00000000004e78c) it calls `cpu_enabled`, and for an enabled one compares bit 1 of its `p_slice`
  byte (loaded at 0xc00000000004e810, shifted right 1 and masked with 1) with a1, returning the first
  processor that matches. So slices 0 and 1 form subnode 0 and slices 2 and 3 subnode 1. It returns
  -1 when `node_first_cpu` is -1, `node_num_cpus` is zero or no processor matches. Established.
- `nasid_slice_to_cpuinfo` (`6.5.30/unix.IP35` `0xc000000000044f8c`, 248 bytes)
  is the IP27 routine with the IP35 NASID shift and the PE-brick check: when bits 12 to 15 of the
  doubleword at `0x9000000034000000` plus the NASID shifted left 33 (address from gp+2256, the load at
  0xc000000000044f9c) equal `0xc`, it calls `chk_for_pebrick` for the start board instead of reading
  the directory. It then calls `find_lboard` for type 17 and returns the first type-1 component whose
  byte at +24 equals a1, or 0. Established.
- `nasid_to_region` (`6.5.30/unix.IP35` `0xc00000000004ed30`, 24 bytes)
  returns a0 when the word at gp-32636 (loaded at 0xc00000000004ed30; the IP35 `cputype` object sits just below it)
  is nonzero, and a0 shifted right 1 (arithmetic) when it is zero. So in coarse mode two IP35 NASIDs
  share a region, where IP27 groups eight. Established.
- `memory_present` (`6.5.30/unix.IP35` `0xc00000000004f4f0`, 36 bytes)
  has the IP27 instructions: it calls `ckphyspnum` with the address shifted right 14 and returns 1
  when that returns 0. Established.

## Console and master I/O

The configuration header also locates the console record: the word at +16 of the header (Linux `ch_cons_off`) is an offset that the kernel adds to the header's own address, not to the node base. Fields read from the console record (Linux `console_t`):

| Offset | Width | Linux name | Kernel use | Grade |
|---|---|---|---|---|
| +8 | doubleword | `config_base` | passed to `ioc3_mlreset` as its first argument | established |
| +16 | doubleword | `memory_base` | passed to `ioc3_mlreset` as its second argument; bits 24 to 31 are taken as the console's widget number | established |
| +32 | signed halfword | `nasid` | the console's node | established |
| +35 | byte | `npci` | the console's PCI slot | established |

- `set_master_bridge_base` (`6.5.30/unix.IP27` `0xc00000000001f868`, 136 bytes)
  reads the console record of `master_nasid` (the halfword at gp-32620) through that node's
  directory and header (the header's console offset is loaded at 0xc00000000001f894). It saves the
  record's NASID (+32) in the halfword at gp+2328, its PCI slot byte (+35) in the byte at gp+2331,
  and bits 24 to 31 of `memory_base` (+16) in the byte at gp+2330 as the widget number. It then
  stores the master bridge's base address in the doubleword at gp-32688: `0x9200000000000000` plus
  the NASID shifted left 32 plus the widget shifted left 24 for a nonzero widget, or the constant
  `0x92000000e0000000` (from gp-24504) plus the NASID shifted left 32 when the widget is 0.
  Established.
- `get_console_nasid` (`6.5.30/unix.IP27` `0xc00000000001f8f0`, 8 bytes)
  returns the halfword at gp+2328 that `set_master_bridge_base` saved (loaded at
  0xc00000000001f8f4). Established.
- `get_console_pcislot` (`6.5.30/unix.IP27` `0xc00000000001f8f8`, 8 bytes)
  returns the byte at gp+2331 (loaded at 0xc00000000001f8fc), the console's PCI slot. Established.
- `get_master_bridge_base` (`6.5.30/unix.IP27` `0xc00000000001f97c`, 8 bytes)
  returns the doubleword at gp-32688 (loaded at 0xc00000000001f980), the master bridge base.
  Established.
- `check_nasid_equiv` (`6.5.30/unix.IP27` `0xc00000000001f900`, 64 bytes)
  returns 1 when its two NASIDs are equal, or when the first equals the `xbow_peer` (+18832 of
  `nodepda_s`, loaded at 0xc00000000001f928) of the second's compact node, and 0 otherwise.
  Established.
- `is_master_nasid_widget` (`6.5.30/unix.IP27` `0xc00000000001f940`, 60 bytes)
  returns 1 when its widget argument (a1) equals the console widget byte at gp+2330 and a call to
  `check_nasid_equiv` accepts its NASID (a0) against the console NASID, and 0 otherwise.
  Established.

## Early platform setup

`mlreset` is the IP27 routine that ties the reads above together at boot: it numbers the nodes, programs each hub's region registers and starts memory and I/O discovery.

- `mlreset` (`6.5.30/unix.IP27` `0xc0000000000260e0`, 1056 bytes)
  takes one argument; 0 selects the full boot-processor path, any other value only the per-processor
  steps. Every processor clears processor performance counter control and data registers 0 and 1
  (`r1nkperf_control_register_set`, `r1nkperf_data_register_set`), and 2 and 3 as well when
  `get_cpu_irr` shifted right 8 is 14 or 15. The boot-processor path then runs in this order: (1)
  stores `get_nasid` in `master_nasid` (the halfword at gp-32620) and calls
  `set_master_bridge_base`; (2) calls `ioc3_mlreset` with the console record's doublewords at +8 and
  +16 (see [Console and master I/O](#console-and-master-io)), then `nvram_baseinit`; (3) stores the
  result of `is_fine_dirmode` in the word at gp-32628 (`0xc0000000014ca1fc`, which has no symbol: the 2-byte `cputype` object ends before it); when `is_specified` reports
  `arg_maxnodes` set, passes `atoi` of it to the unnamed routine at `0xc000000000027618`; (4) clears
  the two doublewords of `boot_cpumask` and calls `cpu_node_probe` with it and the address of
  `numnodes`, storing the result in `maxcpus` (the word at gp+2352); `assfail` (line 281) when that
  is 0 or less. It copies `numnodes` to `maxnodes`, with `assfail` (line 284) when it is 0 or less,
  and sets `boot_cnodemask` (the doubleword at gp+7352) to the low `numnodes` bits (all ones shifted
  right by 64 minus `numnodes`); (5) calls the unnamed routine at `0xc00000000002741c` with the
  address of `region_mask` (gp-32640), stores `getcpuid` in `master_procid` (the halfword at
  gp-26608), sets `master_nasid` again from `compact_to_nasid_node` at `get_cpu_cnode` of that
  processor, and calls `setup_replication_mask` with `maxnodes`. When `master_nasid` then differs
  from `get_nasid` it calls `assfail` (line 302); (6) for each compact node below `maxnodes`, writes
  three registers of that node's hub through the remote hub window: `region_mask` ORed with 1 to hub
  offset `0x18` (address `0x9200000001800018` plus the NASID shifted left 32; Linux
  `PI_REGION_PRESENT`), 2 to hub offset `0x28` (Linux `PI_CALIAS_SIZE`), and 0 to hub offset
  `0x400190` (no Linux name); (7) clears two doublewords at gp+2360 and gp+2368, stores 1 in the
  word at gp+2356, and initialises the lock at gp+2376 with `spinlock_init` and the name
  "hub_mask_lock"; (8) when `master_procid` is not 0, moves the per-processor data pointer of
  `pdaindr` entry 0 (the doubleword at +24 from `boot_cpumask`, which is +8 of the first `pdaindr`
  entry) to the boot processor's entry and clears entry 0; (9) calls `iograph_early_init`,
  `mem_init`, `hubdev_init` and the unnamed routine at `0xc000000000027510`; (10) clears bits 12 and
  14 of `valid_dcache_reasons` and `valid_icache_reasons` (mask `0xffffafff`), turns a `kdebug`
  halfword of -1 into 0, clears `cachewrback`, and sets `cachecolormask` to `pdcache_size` divided
  by 32768 (signed) minus 1, or 0 when the quotient is below 1; (11) calls `verify_snchip_rev`;
  replaces the halfword at gp-32618 with `warbits_override` unless that is -1; and calls
  `he_arcs_set_vectors` and `intrgroup_init`. Both paths then apply `r10k_bpmode` (the word at
  gp-30484): 0 calls `branch_prediction_on`, 1 `branch_prediction_fwd_not_taken`, 2
  `branch_prediction_off`, 3 `branch_prediction_taken`; any other value makes the boot processor
  call `printf` with "WARNING: Illegal value for r10k_bpmode (see mtune/kernel)\n". Finally a
  processor whose `get_cpu_irr` shifted right 8 is 9 adds one to `any_r10k_cpus`, and every
  processor calls `flush_tlb` and `flush_cache`. Established.

The routines `mlreset` calls that only set up locks and masks:

- `mem_init` (`6.5.30/unix.IP27` `0xc0000000000299b8`, 40 bytes)
  calls `init_mutex` on `probe_sema` with a1 of 0, the name "probe" and a3 of -1, and returns its
  result. Established.
- `hubdev_init` (`6.5.30/unix.IP27` `0xc00000000004c040`, 44 bytes)
  calls `init_mutex` on `hubdev_callout_mutex` with the name "hubdev" (a1 0, a3 -1) and clears the
  doubleword `hubdev_callout_list` (gp-32344). Established.
- `intrgroup_init` (`6.5.30/unix.IP27` `0xc00000000004a950`, 112 bytes)
  sets up the unsymbolled table at `0xc0000000014de720` (address from gp-19336): it clears the
  doublewords at +512 and +520, initialises the lock at +528 with `spinlock_init` and the name
  "IGRPMAIN", then for each of 16 entries of 32 bytes from +0 stores the entry index as the word at
  +16, clears the doublewords at +0 and +8, and calls `spinlock_init` for the lock at +20 with the
  name 16 bytes past "IGRPMAIN" in the same string block. Established.
- `setup_replication_mask` (`6.5.30/unix.IP27` `0xc0000000000507b8`, 188 bytes)
  builds `ktext_repmask`, the 128-bit mask of nodes that hold a copy of the kernel text, from the node
  count in a0. It stores 1 in the first doubleword and 0 in the second, so node 0 always has a copy.
  When a0 is 2 or more, for each node i from 1 to a0 - 1 it sets bit i when
  `numa_kernel_replication_ratio` (the word at gp-31020, loaded at 0xc0000000000507e4) is nonzero and
  divides i exactly. It then
  calls `get_nasid` and stores the address of `ktext_repmask` as a doubleword at +64 of the calling
  node's GDA (the store at 0xc000000000050858; the GDA found
  through the directory doubleword at node offset `0x20d0`, as in [Node numbering at
  boot](#node-numbering-at-boot)). Linux's `gda_t` names +64 `g_ktext_repmask`. Established.
- `hub_rtc_init` (`6.5.30/unix.IP27` `0xc0000000000484f8`, 88 bytes)
  does nothing unless compact node a0 is the caller's own node (`get_compact_nodeid`). It then writes
  the local hub of that node through the remote hub window (`0x9200000001800000`, from gp-24584, plus
  the NASID shifted left 32): 1 to offsets `0x140` and `0x148` and 0 to `0x150` and `0x158`, in that
  order. Linux names these `PI_RT_EN_A`, `PI_RT_EN_B`, `PI_PROF_EN_A` and `PI_PROF_EN_B`, so both
  slices take the real-time clock interrupt and neither takes the profiling interrupt. Established
  for the stores; the register names are from Linux.

On IP35 the same routines differ as follows.

- `mem_init` (`6.5.30/unix.IP35` `0xc00000000004eda0`, 40 bytes)
  has the IP27 instructions: it calls `init_mutex` on `probe_sema` with the name "probe". Established.
- `hubdev_init` (`6.5.30/unix.IP35` `0xc00000000006d450`, 44 bytes)
  has the IP27 instructions: it calls `init_mutex` on `hubdev_callout_mutex` with the name "hubdev"
  and clears `hubdev_callout_list` (gp-32368). Established.
- `intrgroup_init` (`6.5.30/unix.IP35` `0xc00000000006ca58`, 176 bytes)
  works on the unsymbolled table at `0xc000000001651e40` (address from gp-22184) with larger entries:
  it clears the eight doublewords at +1280 to +1336, calls `spinlock_init` for the lock at +1344 with
  "IGRPMAIN", and then for each of 16 entries of 80 bytes clears its eight doublewords at +0 to +56,
  stores the entry index as the word at +64 and calls `spinlock_init` for the lock at +68 with the
  name 16 bytes past "IGRPMAIN". Established.
- `setup_replication_mask` (`6.5.30/unix.IP35` `0xc000000000074c38`, 220 bytes)
  clears all eight doublewords of the 512-bit `ktext_repmask`, sets bit 0, and sets bit i for each
  node i from 1 to a0 - 1 that `numa_kernel_replication_ratio` (the word at gp-30692, loaded at
  0xc000000000074c7c) divides exactly, as on IP27. It then calls `get_nasid` and stores the address of
  the mask at +64 of the node's GDA with the IP35 NASID shift (the store at 0xc000000000074cf8).
  Established.
- `hub_rtc_init` (`6.5.30/unix.IP35` `0xc00000000006a03c`, 200 bytes)
  writes two register blocks instead of one when a0 is the caller's own compact node
  (`get_compact_nodeid`): base `0x9200000001800000` (from gp+2288) plus the NASID shifted left 33, then
  1 to offsets `0x140` and `0x148` and 0 to `0x150` and `0x158`, and the same four values to
  `0x200140`, `0x200148`, `0x200150` and `0x200158`. Linux has no IP35 support; the second block
  `0x200000` higher is inferred to be the second processor interface of the IP35 node controller,
  since IP35 nodes carry up to four processors. Established for the stores.

## Memory bank checks

- `membank_check_mixed_hidensity` (`6.5.30/unix.IP27` `0xc00000000001ecc0`, 180 bytes)
  takes a NASID, finds the node's first board through the directory, calls `find_lboard` for type
  17 and `find_first_component` for type 3 (memory), neither result tested for null. It reads the
  eight bank-size halfwords at +60 to +74 of the memory component (Linux `membnk_bnksz`, megabytes;
  the load at 0xc00000000001ed34) and keeps the largest (starting from 0) and the smallest
  (starting from 1024). It returns 1 when the largest is 512 and the smallest is not, and 0
  otherwise. An empty bank has size 0, so a node with one 512 MB bank and an empty bank counts
  as mixed. Established.
- `mem_mixed_hidensity_banks` (`6.5.30/unix.IP27` `0xc00000000001ed74`, 168 bytes)
  returns 1 as soon as a call to `membank_check_mixed_hidensity` returns nonzero for a node, scanning compact
  nodes below `maxnodes` (the word at gp+7348) and skipping entries of `compact_to_nasid_node`
  (address from gp-20736, loaded at 0xc00000000001eda4) that are -1; it returns 0 when no node
  qualifies. Established.
- `membank_check_mixed_hidensity` (`6.5.30/unix.IP35` `0xc000000000045958`, 220 bytes)
  is the IP27 routine with the IP35 NASID shift and a call to `chk_for_pebrick` for the start board
  when the node's register at `0x9000000034000000` (plus the NASID shifted left 33) has `0xc` in
  bits 12 to 15. The bank sizes and the 512 MB test are as on IP27: it returns 1 when the largest
  bank is 512 and the smallest is not, and 0 otherwise. Established.
- `mem_mixed_hidensity_banks` (`6.5.30/unix.IP35` `0xc000000000045a34`, 168 bytes)
  is the IP27 routine over the IP35 `compact_to_nasid_node` (address from gp-23360) and `maxnodes`
  (gp+10284; the table load at 0xc000000000045a64); it calls `membank_check_mixed_hidensity` for each
  node that has a NASID, and returns 1 at the first nonzero result and 0 otherwise. Established.

## Memory sizing

The kernels size each node's memory from the bank-size halfwords of the node board's memory component, not from any hub register. IP27 divides a node's address space into 32 slots and IP35 into 8; the routines below fill three tables indexed by node and slot, and the rest of the kernel asks those tables for page counts and page numbers. A page here is 16 KB: every byte count is turned into pages by adding 16383 and shifting right 14.

| Table | IP27 | IP35 | Contents |
|---|---|---|---|
| `slot_lastfilled_cache` | halfword per node, 64 nodes | halfword per node, 128 nodes | last slot of the node with a nonzero size |
| `slot_psize_cache` (`slot_lastfilled_cache` + 128 on IP27, + 256 on IP35) | halfword per slot, node × 32 + slot | word per slot, node × 8 + slot | pages in the slot |
| `page_ordinal` (`slot_lastfilled_cache` + 4224 on IP27, + 4352 on IP35) | word per slot | word per slot | pages in all earlier slots, in node then slot order |

A page number for slot s of node n is the NASID shifted left 18 ORed with s shifted left 13 on IP27 (4 GB of address space per node, 128 MB per slot), and the NASID shifted left 19 ORed with s shifted left 16 on IP35 (8 GB per node, 1 GB per slot). Linux forms the IP27 page number the same way (`slot_getbasepfn`, `SLOT_SHIFT` 27). Established from `node_getmaxclick` below.

The memory component fields read, beyond the [component header](#component-record-header):

| Offset | Width | Linux name | Kernel use | Grade |
|---|---|---|---|---|
| +58 | signed halfword | `membnk_dimm_select` | IP27: halved (signed) and XORed into the bank index, only when the NVRAM byte at offset 14468 is `y`; IP35: XORed into the bank index always | established |
| +60 | signed halfword array | `membnk_bnksz` | bank sizes in megabytes, eight entries | established |

- `szmem` (`6.5.30/unix.IP27` `0xc0000000000299e0`, 312 bytes)
  fills the three tables for every node and returns the total page count. It calls `assfail` with
  "numnodes > 0" and line 106 when `numnodes` (the word at gp+7340) is 0 or less. For each node n
  below `numnodes` and each slot s from 0 to 31 it stores the running total in the `page_ordinal`
  word for (n, s) (the store at 0xc000000000029a54, in the delay slot of the call), calls the
  unnamed routine at `0xc00000000002a024` with n (sign-extended from 16 bits) and s, stores the
  result as the `slot_psize_cache` halfword (the store at 0xc000000000029a68) and adds it to the
  total. `low_maxclick` (the word at gp+7876) takes the first nonzero slot size it sees and keeps it.
  When the slot size is nonzero it stores s as the node's `slot_lastfilled_cache` halfword. It
  returns the total, or a1 when a1 is nonzero and smaller than the total; a0 is not read.
  Linux's `szmem` walks the same slots with the same per-slot size but feeds them to its own
  allocator and adds a check that the node's page structures fit in slot 0, which IRIX lacks.
  Established.
- `sub_c00000000002a024` (`6.5.30/unix.IP27` `0xc00000000002a024`, 316 bytes)
  is unsymbolled in the linked kernel; the `kernel.o` DWARF names it `slot_psize_compute`. It
  returns the page count of slot a1 on compact node a0. It finds the node's first board through
  the directory as in [Finding a node's first board](#finding-a-nodes-first-board), calls
  `find_lboard` for type 17 and `find_first_component` for type 3, and returns 0 when either finds
  nothing. It then calls `get_nvreg` with 14468; when the low byte of the result is 121 (`y`) the
  bank swap value is the signed halfword at +58 of the memory component divided by 2 (signed),
  otherwise 0. The bank read is the signed halfword at +60 plus twice (a1 / 4 XOR the swap value).
  A bank of 128 MB or less gives all its pages to the first slot of its four (a1 % 4 of 0) and 0 to
  the other three; a larger bank gives a quarter of its pages to each of its four slots. Linux's
  `slot_psize_compute` makes the same choice (its "hack for 128 dimm banks") but has no NVRAM test
  and no swap. `get_nvreg` (`0xc00000000004af50`) reads a byte at that offset from the base
  `nvram_baseinit` stores, and gives 0 when no base is set. Established.
- `slot_getsize` (`6.5.30/unix.IP27` `0xc00000000002a004`, 32 bytes)
  returns the signed `slot_psize_cache` halfword for node a0 and slot a1 (index a0 × 32 + a1), the
  size `szmem` stored; the load is at 0xc00000000002a020, in the delay slot of the return.
  Established.
- `node_getlastslot` (`6.5.30/unix.IP27` `0xc000000000029ff0`, 20 bytes)
  returns the signed `slot_lastfilled_cache` halfword for node a0, the last slot `szmem` found
  memory in (the load at 0xc00000000002a000). Established.
- `node_getmaxclick` (`6.5.30/unix.IP27` `0xc000000000029f78`, 120 bytes)
  returns the last page number of node a0: it calls `slot_getsize` for slots 31 down to 0 and, at
  the first nonzero size, returns the slot's page number (the NASID from `compact_to_nasid_node`
  shifted left 18, ORed with the slot shifted left 13) plus the size minus 1, as a 32-bit sum.
  It returns 0 when every slot is empty. Established.
- `node_getfirstfree` (`6.5.30/unix.IP27` `0xc000000000029f50`, 40 bytes)
  calls `get_freemem_start` with a0 and returns the result converted to a page number (plus 16383,
  shifted right 14, truncated to 32 bits). Established.
- `get_freemem_start` (`6.5.30/unix.IP27` `0xc0000000000506bc`, 252 bytes)
  returns the first free address of compact node a0. For node 0 it returns the doubleword at
  +18760 of the caller's own node data (the per-node table entry indexed by the halfword at
  absolute `0xffffffffffffc422`) ORed with the low 24 bits of `0xc0000000015c6250`, the end of
  `.bss`. For another node whose bit is set in `ktext_repmask` (it holds a copy of the kernel
  text) it returns the doubleword at +18752 of that same node data ORed with the low 24 bits of
  `0xc00000000040464f`, the end of the first `.rodata` section, and with the node's NASID shifted
  left 32 (the load at 0xc00000000005071c; the node-0 load is at 0xc0000000000507a0). For any other node it reads the node's KLCONFIG directory: the offset doubleword of entry
  5 (node offset `0x2148`, masked to 40 bits) is added to the node's base and returned as a 40-bit
  physical address (the load at 0xc000000000050754); when `kdebug` (the halfword at gp-26518) is
  nonzero the size doubleword of the same entry (node offset `0x2158`, the load at
  0xc00000000005076c) is added first, so the debugger's area is skipped. The entry
  layout (offset 8 bytes and size 24 bytes into a 64-byte entry) is Linux's `kldir_ent_t`; Linux's IP27
  headers do not say which area entry 5 describes. Established.

On IP35 the same routines differ only as follows.

- `szmem` (`6.5.30/unix.IP35` `0xc00000000004edc8`, 308 bytes)
  walks slots 0 to 7 of each node, calls the unnamed routine at `0xc00000000004f408` for each, and
  stores the result as a `slot_psize_cache` word (the store at 0xc00000000004ee4c) and the running
  total as the `page_ordinal` word (the store at 0xc00000000004ee3c). The `numnodes` check (the
  word at gp+10276, line 106), `low_maxclick` (gp+10812), `slot_lastfilled_cache` and the return
  value are as on IP27. Established.
- `sub_c00000000004f408` (`6.5.30/unix.IP35` `0xc00000000004f408`, 232 bytes)
  (`slot_psize_compute` in the `kernel.o` DWARF) finds the node board through the directory, or through `chk_for_pebrick` when the node's
  register at `0x9000000034000000` (plus the NASID shifted left 33) has `0xc` in bits 12 to 15,
  then `find_lboard` type 17 and `find_first_component` type 3, returning 0 when either finds
  nothing. It returns the pages of one bank: the signed halfword at +60 plus twice (a1 XOR the
  signed halfword at +58), so each slot is one bank and the swap value at +58 always applies.
  There is no NVRAM test and no 128 MB rule. Established.
- `slot_getsize` (`6.5.30/unix.IP35` `0xc00000000004f3e8`, 32 bytes)
  returns the `slot_psize_cache` word for node a0 and slot a1 (index a0 × 8 + a1), the page count
  `szmem` stored; the load is at 0xc00000000004f404, in the delay slot of the return. Established.
- `node_getlastslot` (`6.5.30/unix.IP35` `0xc00000000004f3d4`, 20 bytes)
  returns the signed `slot_lastfilled_cache` halfword for node a0, the last slot `szmem` found
  memory in (the load at 0xc00000000004f3e4). Established.
- `node_getmaxclick` (`6.5.30/unix.IP35` `0xc00000000004f35c`, 120 bytes)
  calls `slot_getsize` for slots 7 down to 0 (the call at 0xc00000000004f378) and, at the first
  nonzero size, returns the slot's page number (the NASID shifted left 19, ORed with the slot shifted
  left 16) plus the size minus 1, as a 32-bit sum; it returns 0 when every slot is empty.
  Established.
- `node_getfirstfree` (`6.5.30/unix.IP35` `0xc00000000004f334`, 40 bytes)
  calls `get_freemem_start` and converts to a page number as on IP27. Established.
- `get_freemem_start` (`6.5.30/unix.IP35` `0xc000000000074b2c`, 268 bytes)
  takes the node data fields at +34176 (node 0) and +34184 (replicated-text nodes) instead of
  +18760 and +18752, indexes the node table by the halfword at absolute `0xffffffffffffc42a`,
  shifts the NASID left 33, and uses the end of `.bss` (`0xc00000000173a950`) and of the first
  `.rodata` section (`0xc000000000519ebf`). The directory entry 5 reads at node offsets
  `0x2148` and `0x2158` (the loads at 0xc000000000074bcc and 0xc000000000074be4) and the `kdebug`
  test (the halfword load at 0xc000000000074bb8) are as on IP27. Established.

The 6.5.22 and 6.5.30 `kernel.o` objects for IP27 and for IP35 hold these routines with the same instructions, relocations aside.

## Memory and NUMA start-up

- `meminit` (`6.5.30/unix.IP27` `0xc0000000001ac1b4`, 480 bytes)
  sets up the global page pools and per-node page locks. In order it calls `tune_sanity` on
  `tune`; calls `global_freemem_init` and stores its result F in `relaxed_global_freemem`; calls
  `init_global_pool` with F minus the word at +36 of `tune` minus F/1024, F minus the word at +32
  minus F/1024 (both as 32-bit values) and `maxmem`; initialises `memory_lock` ("memlock") and
  `sxbrk_lock` ("sxbrklock") with `spinlock_init`; for each index below `numnodes` plus
  `num_pebricks` copies 8 × (`numnodes` + `num_pebricks`) bytes from `Nodepdaindr` to the table that
  the first doubleword of that node's data points at (`bcopy`), so every node holds its own copy
  of the node index; initialises `private_list_lock` ("private_list_lock"); for each node below
  `numnodes` calls `init_spinlock` on +1552 of the node data with "freelst" and the node number;
  calls `init_sv` on the 64 entries of `pfdwtsv` with "pwt"; calls `lpage_init`; when `numnodes` is
  2 or more, copies 136 × `numnodes` bytes from the table at +1544 of node 0's data to the table at
  +1544 of each other node's data; and finally calls `init_sv` on `sv_sxbrk` with "sxbrk". The node
  data are reached through the per-processor pointer at absolute `0xffffffffffffc400`.
  Established.
- `meminit` (`6.5.30/unix.IP35` `0xc00000000025a888`, 480 bytes)
  is the IP27 routine with the per-node lock at +1520 (address formed at 0xc00000000025a994) and the
  per-node table at +1512 (loads at 0xc00000000025aa20 and 0xc00000000025aa28) instead of +1552 and
  +1544: it calls `tune_sanity`, `global_freemem_init`, `init_global_pool`, `spinlock_init`, `bcopy`,
  `init_spinlock`, `init_sv` and `lpage_init` in the IP27 order with the same arguments. The 6.5.22 and 6.5.30 `os.a(page.o)` objects carry the same code hash in the
  inventory for each platform. Established.
- `numa_init` (`6.5.30/unix.IP27` `0xc000000000277a1c`, 96 bytes)
  runs the start-up table `numa_init_table`: 16-byte entries of a routine pointer at +0 and a name
  pointer at +8, ending at a null routine pointer. It calls each routine in turn with no
  arguments; a nonzero result makes it call `cmn_err` with 0, "[numa_init]: Module <%s>
  initialization failed (%d)", the entry's name and the result, and it carries on with the next
  entry. It returns nothing. The table (at `0xc0000000014bec48`) holds, in order: `mld_init` (Mld),
  `raff_init` (Raff), `mldset_init` (MldSet), `pmolist_init` (PmoList), `topology_init` (Topology),
  `migr_init` (Migr), the six placement policies `plac_policy_default_init`,
  `plac_policy_fixed_init`, `plac_policy_firsttouch_init`, `plac_policy_thread_init`,
  `plac_policy_roundrobin_init` and `plac_policy_cachecolor_init`, `fbck_policy_default_init`,
  `fbck_policy_local_init`, `migr_policy_default_init`, `migr_policy_control_init`,
  `migr_policy_refcnt_init`, `mem_tick_init` (MemTickInit) and `assign_wards` (AssignWards).
  Established.
- `numa_init` (`6.5.30/unix.IP35` `0xc0000000003272f0`, 96 bytes)
  has the IP27 instructions: it calls each routine of its table through a register (the call at
  0xc00000000032730c) and calls `cmn_err` with the module name and result on a failure (the call at
  0xc000000000327340). Its table at `0xc0000000015e95c8` lists the same 19 routines with the same
  names in the same order. Established.
- `mem_adjust` (`6.5.30/unix.IP27` `0xc00000000014b3bc`, 252 bytes)
  trims the buffer cache when memory is short. It calls `bfrag_shake` with 0 and a node mask of
  the low `numnodes` bits (all ones shifted right by 64 minus `numnodes`). It then forms the
  surplus S: `chunkpages` minus `min_file_pages`, floored at 0, plus `bufmem` minus `min_bufmem`
  when that is positive. It reads `get_availrmem` and `get_availsmem` of `global_pool` and returns
  when S is 0 or less. The want W is `min_free_pages` minus `relaxed_global_freemem`, floored at 0;
  when the smaller of the two available counts is below `chunkpages` plus `bufmem`, W becomes that
  shortfall if it is larger. When W is positive it calls `free_buffer_mem` with the smaller of S
  and W. Established.
- `mem_adjust` (`6.5.30/unix.IP35` `0xc0000000001e739c`, 376 bytes)
  builds the node mask as two doublewords on the stack, setting bits 0 to `numnodes` - 1 one at a
  time, and passes them to `bfrag_shake` in a1 and a2; the rest is the IP27 computation with
  other registers. The 6.5.22 IP35 `sgi.a(chunkio.o)` copy (388 bytes) clears the two mask
  doublewords with a two-pass loop instead of two stores and is otherwise the same instruction for
  instruction; the IP27 copies of both releases carry the same code hash. Established.

## Board serial numbers

The kernels find a board's serial number in the manufacturing NIC string the PROM copies into KLCONFIG: a text of `name:value;` pairs. The kernel builds the same text itself when it reads a part directly; [nic.md](nic.md#manufacturing-text) gives the fields and the page bytes they come from. Each board class keeps the offset of that string in its main component; Linux names those fields `hub_mfg_nic`, `bri_mfg_nic`, `rou_mfg_nic` and `gfx_mfg_nic`, and component types 2, 5, 9 and 10 `KLSTRUCT_HUB`, `KLSTRUCT_BRI`, `KLSTRUCT_ROU` and `KLSTRUCT_GFX`.

| Board class | Component type | NIC string offset (IP27) | NIC string offset (IP35) | Part names tried first | Grade |
|---|---|---|---|---|---|
| `0x10` node | 2 | word at +80 | word at +80 | IP27: "IP27", then "IP31"; IP35: "IP35", "IP39", "IP45", "IP34", "IP53", "IP59" | established |
| `0x20` I/O | 5 | word at +136 | word at +136 | none | established |
| `0x30` router | 9 | word at +128 | word at +144 | none | established |
| `0x50` graphics | 10 | word at +92 | word at +92 | none | established |

Grades. The class values, the component type each one selects, the NIC string offsets and the part-name strings were read from `board_serial_number_get` in the linked 6.5.30 IP27 and IP35 kernels: the class byte is compared against `0x10`, `0x20`, `0x30` and `0x50`; each branch calls `find_first_component` with 2, 5, 9 and 10; the loads are at +80, +136, +128 (IP27) or +144 (IP35) and +92; and the strings at the pointers passed to the serial-number routine read "IP27", "IP31" on IP27 and "IP35", "IP39", "IP45", "IP34", "IP53", "IP59" on IP35. The words "I/O", "router" and "graphics" are labels for the role of the component each class selects, taken from the table's use and Linux's class names, and are inferred; "node" is tied to the node board type 17 that `get_module_id` passes. Class `0x40` is not handled by either routine.

- `sub_c00000000001e924` (`6.5.30/unix.IP27` `0xc00000000001e924`, 316 bytes)
  is unsymbolled in the linked kernel; the `kernel.o` DWARF names it `component_serial_number_get`.
  It takes a board pointer (a0, used only for its NASID), a NIC string offset (a1), an output buffer
  (a2) and a part name (a3). It returns 1 when a1 is 0. It forms the string's address on the board's
  node as in [Addresses](#addresses); when the part name is not empty (`strcmp` against the empty
  string at `0xc000000001404e70`) it moves to the first match of the name with `strstr` and returns
  1 when there is none. It then calls `strstr` for "Serial:" and returns 1 when that is absent, and
  otherwise copies the bytes after "Serial:" into a2 up to the first `;` (59), adds a terminating
  zero and returns 0. The copy has no length limit and stops only at `;`. Established.
- `board_serial_number_get` (`6.5.30/unix.IP27` `0xc00000000001ea60`, 424 bytes)
  takes a board (a0) and a buffer (a1), returns 1 when either is null, and first stores a zero byte
  in the buffer. It selects the component from the class (byte at +5 masked with `0xf0`) as in the
  table, calls `find_first_component` for it, and returns 1 when there is none. For a node board it
  calls `sub_c00000000001e924` with the hub's NIC offset and "IP27" (string block from gp-20768,
  plus 408) and, when that fails, again with "IP31" (plus 416); for the other classes it passes the
  empty name. It returns 0 when a search succeeds and 1 otherwise. A board of any other class gets
  an empty buffer and the result 0. Established.
- `sub_c000000000045578` (`6.5.30/unix.IP35` `0xc000000000045578`, 316 bytes)
  (`component_serial_number_get` in the `kernel.o` DWARF) is the IP27 routine with the NASID taken
  from bits 33 to 39 of a0 (the shift at 0xc0000000000455a8) and shifted left 33. Established.
- `board_serial_number_get` (`6.5.30/unix.IP35` `0xc0000000000456b4`, 536 bytes)
  is the IP27 routine with six part names for a node board, tried in the order of the table through
  `sub_c000000000045578` (string block from gp-23392, plus 480 to 520), and the router NIC offset
  read from +144 of the router component (the load at 0xc0000000000458b0) instead of +128. Established.

## Hub slot numbers (IP27)

An IP27 hub reads its own slot from the low byte of the hub register at offset `0x220048` (Linux `MD_SLOTID_USTAT`; the kernel reads it at `0x9200000001220048`, the calling processor's own hub). Linux gives the slot in bits 0 to 2 on an Origin 2000 class module and in bit 7 on an Origin 200 (`MSU_SN0_SLOTID_MASK`, `MSU_SN00_SLOTID_MASK`), which is the split the kernel makes on `p_sn00` (+1315 of `pda_s`, the byte at absolute `0xffffffffffffc523`). These routines have no IP35 counterparts in the linked IP35 kernel.

- `hub_slotbits_to_slot` (`6.5.30/unix.IP27` `0xc00000000003caec`, 108 bytes)
  turns slot bits (a0) into a slot number. When `p_sn00` is nonzero it returns 1 when bit 7 of a0 is
  set and 2 when it is clear. Otherwise it takes bits 0 to 2 as an index (the bound test against 8
  is never taken) and reads the low byte of word index of one of two tables, chosen by bit 0 of the
  word at `0x90000000100000a8` (the load at 0xc00000000003cb2c; Linux puts the local boot PROM
  space, `LBOOT_BASE`, at `0x9000000010000000`): with the bit set the table at `0xc0000000014168c0`
  gives 240, 240, 6, 5, 4, 3, 2, 1 for indexes 0 to 7, and with it clear the table at
  `0xc0000000014168a0` gives 240, 240, 82, 81, 4, 3, 2, 1. So index 2 and 3 name slots `0x52` and
  `0x51` in one layout and 6 and 5 in the other. Established.
- `get_my_slotid` (`6.5.30/unix.IP27` `0xc00000000003cd64`, 36 bytes)
  reads the doubleword at `0x9200000001220048` (address from gp-23824), keeps its low byte and calls
  `hub_slotbits_to_slot` with it (the call at 0xc00000000003cd74), returning that slot number.
  Established.
- `hub_slot_get` (`6.5.30/unix.IP27` `0xc00000000003cee0`, 36 bytes)
  calls `get_my_slotid` (the call at 0xc00000000003cee8) and returns its result masked with `0xf`.
  Established.
- `hub_slot_to_widget` (`6.5.30/unix.IP27` `0xc00000000003cb58`, 104 bytes)
  maps a slot number (a0) to a crossbow widget number. It returns 255 when the low four bits are 8
  or more. Otherwise, with i the low four bits, it returns the low byte of word i - 1 of a table:
  when the slot's high four bits are 5 (`0x50`), the table at `0xc0000000014168f0` (10, 9, 255, 255
  for i of 1 to 4); else, when the word at `0x9000000010000090` (read at 0xc00000000003cb70, in the
  boot PROM space) is 0, the table at `0xc0000000014168e0` (9, 10, 10, 9); otherwise the table at
  `0xc0000000014ca270` (10, 10, then 0). A slot number of 0 in the low bits reads the word before
  each table. Established.
