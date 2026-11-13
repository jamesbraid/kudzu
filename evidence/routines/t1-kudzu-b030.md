# Tier-1 routine readings, kudzu batch 30

Role statements for entries 750 through 774 of the ranked tier-1 list (1072 targets),
read from their instructions only. Three entries are left out because their statements cite nothing beyond the routine's own name. Each grade is inferred: the facts show what a routine loads, stores,
calls and branches on, but not what the fields mean. Where a routine's behavior is fully
established by its facts, the label says so and separates that from the inferred role.
Addresses are in the binary named in each header.

- `baerror` in `6.5.30/unix.IP35` at 0xc000000000077a08
  **Inferred.** `baerror` loads one doubleword from the fixed address 0xffffffffffffc020, stores a halfword at +0xf6 from the pointer it loaded, stores a word to the fixed address 0xffffffffffffc48c, and returns 1. A branch tests the loaded pointer: when it is zero the branch jumps to the word store at 0xc000000000077a24 and skips the halfword store; when it is nonzero the halfword store runs first. The name suggests a bus-error handler, but the instruction listing does not show what the stored values mean or what the tested pointer refers to.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.30/unix.IP35` at 0xc000000000077a38
  **Inferred.** `earlybaerror` is a 52-byte stub with one word store to the fixed address 0xc000000001645ae8 and a return of 1. It has no loads, branches or calls in the facts. The name suggests the early variant of the handler at 0xc000000000077a08; unlike that routine it makes no load or conditional store, and the facts do not show what the store at 0xc000000001645ae8 means.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.30/unix.IP35` at 0xc000000000079720
  **Inferred.** `VEC_dbe` loads a halfword from the fixed address 0xffffffffffffc428, stores eight bytes into the block at a0 +0x140, then jumps to `VEC_trap` at 0xc000000000065850 with its four argument registers unchanged. The name suggests a data-bus-error vector that records a state field and hands off to the common trap code. The instruction listing does not show the stored value, so what goes into +0x140 is open.
  Reviewed: 2026-11-12 (sonnet)

- `bte_init` in `6.5.30/unix.IP35` at 0xc00000000007d358
  **Inferred.** `bte_init` is a 344-byte routine. It calls `spinlock_init` at 0xc000000000331d4c on a0 +0x3c (a1 is 0xc000000001529be0). It stores at offsets +0x20, +0x28, +0x38 and +0x18 (the +0x38 store is a halfword), then stores at +0x0 and +0x4c and later at +0x68. The facts give offsets, not the base registers or the stored values. A branch on a2 (a2 == 0) selects a different store sequence that ends with a +0x4c store. A word loaded from 0xc000000001644c14 is compared with 0x21 (signed); when it is 0x21 or more the routine skips a second +0x68 store and goes to its return, so this is a forward skip and the facts show no loop. It returns a0. Role (from name): setup for a block-transfer engine; the facts do not name the engine or show what the stored values mean.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.30/unix.IP35` at 0xc00000000007ed1c
  **Inferred.** `badaddr` is a 48-byte wrapper. It calls `probe_common` at 0xc00000000007edec with a0 set to a stack address (sp-48), forwards a1 through a3 unchanged, and returns the result. Role (from name): a bad-address probe, a test read that reports whether the address faults. This routine contains no fault handling of its own, so any such handling is not shown here and may be in `probe_common`.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.30/unix.IP35` at 0xc00000000007ed4c
  **Inferred.** `badaddr_val` has the same 48-byte shape as `badaddr` at 0xc00000000007ed1c. It calls `probe_common` at 0xc00000000007edec with the same argument pattern and returns its result. The name suggests a variant that also hands back the value read, but this routine passes nothing extra, so any difference from `badaddr` lies in `probe_common` or in what the caller passes. The facts do not show which.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.30/unix.IP35` at 0xc00000000007ed7c
  **Inferred.** `wbadaddr` is a 56-byte wrapper with the same call to `probe_common` at 0xc00000000007edec and the same argument pattern as the two read probes before it. The name suggests a write probe. The routine stores nothing itself, so any write behavior must come from `probe_common` or from a mode value the instruction listing does not show.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.30/unix.IP35` at 0xc000000000083b30
  **Inferred.** `VEC_ibe` loads two doublewords and a halfword, branches on the sign of one loaded register (taken when it is zero or positive), stores a doubleword at an unresolved offset and another eight bytes at a0 +0x140, and jumps to `VEC_trap` at 0xc000000000065850. The jump forwards a0, a1 and a3 unchanged; a2 is unresolved in the facts, so it may have been altered. Role (from name): the instruction-bus-error counterpart of `VEC_dbe` at 0xc000000000079720. Two of the addresses it touches are unresolved in the instruction listing, so the exact record it writes is open.
  Reviewed: 2026-11-12 (sonnet)

- `fake_a_part_number` in `6.5.30/unix.IP35` at 0xc0000000000921ac
  **Inferred.** `fake_a_part_number` is a 500-byte routine that writes into the buffer at a0. Branch on a1 against 0x100: on the path where a1 equals 0x100 it calls `random_eeprom_ch` at 0xc00000000009210c (a0 = 0) in a loop, storing each returned byte with a byte store until a counter reaches 0xc, with two special positions that take different branches (counter equal to 3 and to 8). On the other path it compares a masked value against 0x4000 and 0x8000, then calls `get_nasid` at 0xc000000000041434 and `get_chimera_type` at 0xc00000000004167c in four pairs, comparing the type result against 0x3, 0x1, 0x2 and 0x5; each outcome copies a fixed-size block with unaligned doubleword and word stores plus a byte store. The return value is one of an unresolved value, the type result, or 0x2d303031; that constant reads as ASCII "-001" if the bytes sit in memory in order, but that is a guess from the constant alone. Role (from name): a stand-in part number used when the real one cannot be read.
  Reviewed: 2026-11-12 (sonnet)

- `fake_a_product_name` in `6.5.30/unix.IP35` at 0xc000000000092424
  **Inferred.** `fake_a_product_name` is a 520-byte routine built like `fake_a_part_number` above it. Its calls to `get_nasid` (callee 0xc000000000041434) and `get_chimera_type` (callee 0xc00000000004167c) sit at call sites 0xc000000000092568 and 0xc000000000092570, on the path where the masked value matches neither 0x4000 nor 0x8000. It compares values against 0x100, 0x200, 0x400, 0x4000 and 0x8000, and copies table data into the buffer at a1 with unaligned stores. Role (from name): a stand-in product name. The visible return values are 0x200, 0x4000, the type result and an unresolved value, and the instruction listing does not show which one a caller uses.
  Reviewed: 2026-11-12 (sonnet)

- `gbrick_eeprom_read` in `6.5.30/unix.IP35` at 0xc000000000093cc8
  **Inferred.** `gbrick_eeprom_read` is a 300-byte routine. It starts with `get_chimera_type` at 0xc00000000004167c and `get_nasid` at 0xc000000000041434, tests bit 0x800 of a2 and compares a1 with the `get_nasid` result. On one path it calls `get_elsc` at 0xc000000000065240 with the table at 0xc000000001646f00. On another it calls `sc_open` at 0xc0000000000d497c with 0xffff000 as its second argument; a negative result takes an error path at 0xc000000000093dec. After a successful open it calls `read_board_ia` at 0xc0000000000931ec and then `sc_close` at 0xc0000000000d4abc, and tests the `read_board_ia` result against zero. It returns 0, 1 or an unresolved value. Role (from name): a read of a graphics board EEPROM through a serial-controller channel; the facts do not identify the board, and I cannot expand "elsc" or "sc" from the instruction listing.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_phase_bits` in `6.5.30/unix.IP35` at 0xc000000000093fa8
  **Inferred.** `nic_get_phase_bits` is a 52-byte leaf with no calls. It loads one doubleword and returns an unresolved value, so the result is not shown to be the loaded word itself and the facts do not rule out arithmetic on it. The facts do not resolve the load address. Role (from name): phase bits from a network interface controller; what the returned value encodes is open.
  Reviewed: 2026-11-12 (sonnet)

- `cpuprom_detailed_inventory_info_add` in `6.5.30/unix.IP35` at 0xc000000000096bdc
  **Inferred.** `cpuprom_detailed_inventory_info_add` is a 132-byte routine. It starts with a call to `hubdev_cnodeid_get` at 0xc00000000006d61c, allocates a record with `klhwg_invent_alloc` at 0xc000000000072328 using 0x15 and 0x20 as arguments, and writes four fields of the returned record: a word at +0xc and a word at +0x10 from two words read at 0x90000000180000a0 and 0x90000000180000a4, plus a word at +0x14 and a byte at +0x4 whose values the facts do not show. It then attaches the record with `hwgraph_info_add_LBL` at 0xc000000000340ccc and exports it with `hwgraph_info_export_LBL` at 0xc000000000340e30 (second argument 0x20). Role (from name): adds an inventory record for a CPU board. The callees fit the name, but the meaning of the two words and of the arguments 0x15 and 0x20 is not shown, so the field layout stays open.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_id_get` in `6.5.30/unix.IP35` at 0xc0000000000ac608
  **Inferred.** `pcireg_id_get` is a 352-byte routine. It reads the halfword at +0x72 of its argument structure and dispatches on it against 0x3, 0x2, 0x0, 0x1 and -1. For the matching cases it compares further values against 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002, and against 0x24 and 0x36 in several places. A null argument, a halfword of -1, or a value that matches none of the constants leads to a `panic` call at 0xc0000000002ae1e8; the other exits return 0 or an unresolved value. Role (from name): a PCI register ID read, and the constants look like device or register codes, but the instruction listing does not say which device they belong to.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_stat_get` in `6.5.30/unix.IP35` at 0xc0000000000ac768
  **Inferred.** `pcireg_stat_get` is a 356-byte routine with the same dispatch as `pcireg_id_get` at 0xc0000000000ac608: the same halfword at +0x72, the same constant ladder and the same null check with a panic. The facts differ only in addresses, including the panic argument (0xc00000000152d370 here); they do not show how its final loads differ from the ID reader's. Role (from name): a status read rather than an ID read, a sibling of the ID reader. The instruction listing does not show which register it reads.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_nic_addr` in `6.5.30/unix.IP35` at 0xc0000000000b01a0
  **Inferred.** `pcireg_nic_addr` is a 344-byte routine with the same halfword dispatch on +0x72 and the same null-check panic. Its ladder has the same five entries as the two PCI register routines above, in a different order (0x2, 0x3, 0x0, 0x1 and -1), and its later constant chain (0xe000 through 0xc002, with the 0x24 and 0x36 compares) matches theirs. The name suggests it returns a network interface address on the bus. The instruction listing shows it returns 0 or an unresolved value, and nothing more about the address.
  Reviewed: 2026-11-12 (sonnet)

- `ioc4_eiattach` in `6.5.30/unix.IP35` at 0xc0000000000c22f8
  **Inferred.** `ioc4_eiattach` is a 420-byte routine, gated on `ioc4_subdev_enabled` at 0xc0000000000c3e84 with subdevice 6. When the subdevice is enabled, it finds the hardware-graph node and memory pointer, allocates a 0x38-byte record and a 0x80000-byte buffer with `kvpalloc`, adds a character device, and calls `ioc4_is_console` at 0xc0000000000c3990. When that returns nonzero it sets the controller number to 1, calls `ei_make_hwgraph_nodes` at 0xc0000000000c1e44 and connects an interrupt with `ioc4_intr_connect` at 0xc0000000000c33ac; when it returns zero it sets the controller number to -1 instead. It returns 0 or -1, and the facts do not show which path produces which value. Role (from name): attach for an Ethernet interface; the facts do not show what protocol the subdevice speaks.
  Reviewed: 2026-11-12 (sonnet)

- `eiattach` in `6.5.30/unix.IP35` at 0xc0000000000c4454
  **Inferred.** `eiattach` is the IOC3 counterpart of `ioc4_eiattach` above it, gated on `ioc3_subdev_enabled` at 0xc0000000000c62a8 with subdevice 7. It follows the same steps: hardware-graph traversal (two traversals here), memory-pointer lookup, a 0x38-byte record, a 0x80000-byte buffer from `kvpalloc`, a character device, and a test of `ioc3_is_console`. When that returns nonzero it sets the controller number to 1, calls one unnamed routine at 0xc0000000000c3fac and connects the interrupt through `ioc3_intr_connect` at 0xc0000000000c57b4 with the argument 0x18000000; when it returns zero it sets the controller number to -1. It does not call the node-creation helper that the IOC4 copy calls. Role (from name): the same Ethernet-interface attach, for IOC3.
  Reviewed: 2026-11-12 (sonnet)

- `ioc4_pckm_attach` in `6.5.30/unix.IP35` at 0xc0000000000c9274
  **Inferred.** `ioc4_pckm_attach` is a 1424-byte routine, gated on `ioc4_subdev_enabled` for subdevice 1. It calls `ioc4_write_ireg` at 0xc0000000000c2e3c with arguments 0x40, 1, 1 at the start and 0x40, 0, 1 near the end (the facts do not say which argument is the register offset), allocates three records with `kmem_zalloc`, and sets the keyboard type with `initkbdtype` at 0xc0000000004686a8. It sends the command bytes 0xf5, 0xf4 and 0xf6 through `pckm_pollcmd` at 0xc0000000000ca9a0 and checks the replies against 0xfa with `data_check`. Those byte values match the PS/2 commands for disable scanning, enable scanning and set defaults, with 0xfa as the acknowledge byte; that match is public protocol knowledge, not something the instruction listing shows. The routine also adds two character devices at mode 0666, connects an interrupt with `ioc4_intr_connect`, and reads an option string through `kopt_find` at 0xc0000000002c4890 checked with `strstr`. Role (from name): attach for keyboard and mouse ports on IOC4. The instruction listing does not show what the option selects.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.30/unix.IP35` at 0xc0000000000cb864
  **Inferred.** `pckm_attach` is the IOC3 counterpart of `ioc4_pckm_attach` above it, gated on `ioc3_subdev_enabled` for subdevice 3. It follows the same sequence: a call to `ioc3_write_ireg` at 0xc0000000000c4ff4 with arguments 0x400000, 1 at the start and 0x400000, 0 near the end, three allocations, the keyboard-type setup, the command polls, two character devices at mode 0666, an interrupt connection through `ioc3_intr_connect` at 0xc0000000000c57b4, and an option check. The polls call unnamed routines at 0xc0000000000ccf98 and 0xc0000000000cd3f8 with the same 0xf5, 0xf4, 0xf6 and 0xfa arguments as the IOC4 copy, so they are probably this binary's counterparts of `pckm_pollcmd` and `data_check`. Role (from name): the IOC3 keyboard and mouse attach; the name adds nothing to what the IOC4 copy shows.
  Reviewed: 2026-11-12 (sonnet)

- `get_myid` in `6.5.30/unix.IP35` at 0xc0000000000d2c94
  **Inferred.** `get_myid` is a 16-byte routine that loads one doubleword from the fixed uncached address 0x9200000001000020 and returns it after a shift left by 0, which the facts show as the return expression. It has no branches and no calls. Role (from name): returns a processor or node id; the instruction listing does not split out any bits of the loaded word, so what it encodes is open.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.30/unix.IP35` at 0xc0000000000dc9cc
  **Inferred.** `cn_init` is a 384-byte routine. It makes halfword, doubleword and byte stores and loads whose bases the facts do not resolve. It calls `gfx_earlyinit` at 0xc000000000469c18 on some paths. When the halfword at 0xc00000000166dce8 is zero it initializes a lock with `spinlock_init` and a mutex with `init_mutex`, then checks the console option with `is_specified` at 0xc0000000002c4694; when that returns nonzero it compares the first byte of the option at 0xc0000000016b87c8 with 0x67 and 0x47, the ASCII codes for lowercase and uppercase g, which suggests a graphics console choice. `tp_init` at 0xc0000000004699b0 is called when the byte at 0xc000000001644e50 is zero, and `du_init` at 0xc000000000227c94 when that byte is nonzero and a1 is zero. The return value is one of an unresolved value, the `tp_init` result or the `du_init` result. Role (from name): console initializer.
  Reviewed: 2026-11-12 (sonnet)
