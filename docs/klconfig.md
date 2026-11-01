# KLCONFIG as the kernel reads it

The IP27 and IP35 PROMs leave a description of each node's hardware in that node's memory: a directory at a fixed offset, a configuration header, and a linked list of board records, each with an array of component records. The 6.5.30 IP27 and IP35 kernels never copy this description; they walk it in place through uncached addresses. This page states what the kernels expect to find there: the addresses they form, the offsets they read, the values they compare against, and what each lookup routine returns. IP30 has no KLCONFIG reader: none of the routines below is linked into `6.5.30/unix.IP30`.

Every fact is graded. "Established" means the whole routine body was read in the named build at the named address. Public Linux (`arch/mips/include/asm/sn/klconfig.h`, `arch/mips/sgi-ip27/`) and OpenBSD (`sys/arch/sgi/sgi/ip27_machdep.c`, `sys/arch/sgi/include/mnode.h`) read the same records; where their offsets agree with the kernel's loads that is noted, and their field names are used as labels. The linked kernels' DWARF declares `lboard_s` with no members; the board record's full definition is in the DWARF of the `idbg.o` objects ([structs/lboard_s.md](structs/lboard_s.md)), and every board offset below agrees with it. No shipped DWARF defines the component, header, directory or GDA records, so those offsets come from the instructions alone.

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

The directory therefore sits at offset `0x2000` of each node and the configuration entry is the second 64-byte entry, with its offset field at +8 of the entry. Linux reads the same: `KLDIR_OFFSET` is `0x2000`, `KLI_KLCONFIG` is entry 1, `kldir_ent_t.offset` is at +8, and `kl_config_hdr_t.ch_board_info` is at +20. Established.

On IP35 this path is taken only after a check: `get_module_id` and `is_master_baseio` first read the doubleword at `0x9000000034000000` plus the NASID shifted left 33, and when bits 12 to 15 of it equal `0xc` they call `chk_for_pebrick` instead of reading the node's own directory (see [IP35 differences](#ip35-differences)). Which register that address names is not settled; it is listed in [open-questions.md](open-questions.md).

## Board record

Offsets as the 6.5.30 IP27 and IP35 kernels read them. "Readers" names the routines on this page that load the field.

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
