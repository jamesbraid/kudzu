# Tier-1 routine readings, kudzu batch 30

Role statements for entries 750 through 774 of the ranked tier-1 list (1072 targets),
read from their instructions only. Three entries are left out because their statements cite nothing beyond the routine's own name. Each grade is inferred: the facts show what a routine loads, stores,
calls and branches on, but not what the fields mean. Where a routine's behavior is fully
established by its facts, the label says so and separates that from the inferred role.
Addresses are in the binary named in each header.

- `baerror` in `6.5.30/unix.IP35` at 0xc000000000077a08
  **Inferred.** `baerror` loads one doubleword from the fixed address 0xffffffffffffc020, stores a halfword through a pointer loaded from [-0x3fe0]:8 and a word to the fixed address 0xffffffffffffc48c, and returns 1. A branch tests the word at [-0x3fe0]:8 and skips the word store when it is zero. The name suggests a bus-error handler, but the instruction listing does not show what the stored values mean or what the tested word holds.

- `earlybaerror` in `6.5.30/unix.IP35` at 0xc000000000077a38
  **Inferred.** `earlybaerror` is a 52-byte stub with one word store to the fixed address 0xc000000001645ae8 and a return of 1. The instruction listing tags that store as nofault, and I have not confirmed what that tag guarantees. The name suggests the early variant of the handler at 0xc000000000077a08, but the facts show no difference between them beyond this store.

- `VEC_dbe` in `6.5.30/unix.IP35` at 0xc000000000079720
  **Inferred.** `VEC_dbe` loads a halfword from the fixed address 0xffffffffffffc428, stores eight bytes into the block at a0 +0x140, then jumps to `VEC_trap` at 0xc000000000065850 with its four argument registers unchanged. The name suggests a data-bus-error vector that records a state field and hands off to the common trap code. The instruction listing does not show the stored value, so what goes into +0x140 is open.

- `bte_init` in `6.5.30/unix.IP35` at 0xc00000000007d358
  **Inferred.** `bte_init` is a 344-byte initializer. It calls `spinlock_init` at 0xc000000000331d4c on a0 +0x3c, stores to the a0 fields at +0x0, +0x18, +0x20, +0x28, +0x38, +0x4c and +0x68, and stores two words to the uncached hub address 0x92000000014001f8. A loop runs from 0xc00000000007d40c back to the top and exits when a counter loaded from 0xc000000001644c14 reaches 0x21. The name suggests block-transfer engine setup, but the facts do not name the engine, and the stored values and the meaning of the loop count are open. It returns a0.

- `badaddr` in `6.5.30/unix.IP35` at 0xc00000000007ed1c
  **Inferred.** `badaddr` is a 48-byte wrapper. It calls `probe_common` at 0xc00000000007edec with a0 set to a stack buffer (sp-48), forwards a1 through a3, and returns the result. The name suggests a bad-address probe, a test read that reports whether the address faults. The fault handling itself is not in this routine; it must be in `probe_common`.

- `badaddr_val` in `6.5.30/unix.IP35` at 0xc00000000007ed4c
  **Inferred.** `badaddr_val` has the same 48-byte shape as `badaddr` at 0xc00000000007ed1c. It calls `probe_common` at 0xc00000000007edec with the same argument pattern and returns its result. The name suggests a variant that also hands back the value read, but this routine passes nothing extra, so any difference from `badaddr` lies in `probe_common` or in what the caller passes. The facts do not show which.

- `wbadaddr` in `6.5.30/unix.IP35` at 0xc00000000007ed7c
  **Inferred.** `wbadaddr` is a 56-byte wrapper with the same call to `probe_common` at 0xc00000000007edec and the same argument pattern as the two read probes before it. The name suggests a write probe. The routine stores nothing itself, so any write behavior must come from `probe_common` or from a mode value the instruction listing does not show.

- `VEC_ibe` in `6.5.30/unix.IP35` at 0xc000000000083b30
  **Inferred.** `VEC_ibe` is the instruction-bus-error counterpart of `VEC_dbe` at 0xc000000000079720. It loads two doublewords and a halfword, branches on the sign of one register, stores eight bytes at a0 +0x140, and jumps to `VEC_trap` at 0xc000000000065850 with its arguments forwarded. Two of the addresses it touches are unresolved in the instruction listing, so the exact record it writes is open.

- `fake_a_part_number` in `6.5.30/unix.IP35` at 0xc0000000000921ac
  **Inferred.** `fake_a_part_number` is a 500-byte routine that copies data from tables around 0xc0000000004993xx into the buffer at a0, using unaligned doubleword and word stores. It calls `random_eeprom_ch` at 0xc00000000009210c and `get_nasid` and `get_chimera_type` at 0xc000000000041434 and 0xc00000000004167c several times, then branches on the type result against 0x3, 0x1, 0x2 and 0x5. The name suggests a stand-in part number used when the real one cannot be read. The constant 0x2d303031 is one of its possible return values; it reads as ASCII "-001" if the bytes sit in memory in order, but that is a guess from the constant alone.

- `fake_a_product_name` in `6.5.30/unix.IP35` at 0xc000000000092424
  **Inferred.** `fake_a_product_name` is a 520-byte routine built like `fake_a_part_number` above it. It calls `get_nasid` and `get_chimera_type` at 0xc000000000092568 and 0xc000000000092570, compares values against 0x100, 0x200, 0x400, 0x4000 and 0x8000, and copies table data around 0xc0000000004993xx and 0xc0000000004994xx into the buffer at a1. The name suggests a stand-in product name. The visible return values are 0x200, 0x4000, the type result and an unresolved value, and the instruction listing does not show which one a caller uses.

- `gbrick_eeprom_read` in `6.5.30/unix.IP35` at 0xc000000000093cc8
  **Inferred.** `gbrick_eeprom_read` is a 300-byte routine that reads a board EEPROM through a serial-controller channel. It opens a channel with `sc_open` at 0xc0000000000d497c using 0xffff000 as an argument, reads board information with `read_board_ia` at 0xc0000000000931ec, closes the channel with `sc_close` at 0xc0000000000d4abc, and calls `get_elsc` at 0xc000000000065240 on a path that uses the table at 0xc000000001646f00. A negative `sc_open` result takes an error path at 0xc000000000093dec, and the routine returns 0 or 1. The name suggests a read of a graphics board's EEPROM, but the facts do not identify the board, and I cannot expand "elsc" or "sc" from the instruction listing.

- `nic_get_phase_bits` in `6.5.30/unix.IP35` at 0xc000000000093fa8
  **Inferred.** `nic_get_phase_bits` is a 52-byte leaf that loads one doubleword from the fixed address 0x9000000018000078 and returns it in v0. The name suggests phase bits from a network interface controller, but the instruction listing shows no shift or mask, so no particular bits are isolated. What the returned word encodes is open.

- `cpuprom_detailed_inventory_info_add` in `6.5.30/unix.IP35` at 0xc000000000096bdc
  **Inferred.** `cpuprom_detailed_inventory_info_add` is a 132-byte routine that adds an inventory record for a CPU board. It starts with a call to `hubdev_cnodeid_get` at 0xc00000000006d61c, allocates a record with `klhwg_invent_alloc` at 0xc000000000072328 using 0x15 and 0x20 as arguments, and fills three fields from two words read at 0x90000000180000a0 and 0x90000000180000a4. It then attaches the record under a label with `hwgraph_info_add_LBL` at 0xc000000000340ccc and exports it with `hwgraph_info_export_LBL` at 0xc000000000340e30. The callees fit the name, but the meaning of the two words and of the arguments 0x15 and 0x20 is not shown, so the field layout stays open.

- `pcireg_id_get` in `6.5.30/unix.IP35` at 0xc0000000000ac608
  **Inferred.** `pcireg_id_get` is a 352-byte routine. It reads the halfword at +0x72 of its argument structure and dispatches on it against 0x3, 0x2, 0x0, 0x1 and -1. For the matching cases it compares further values against 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002, and against 0x24 and 0x36 in several places. A null argument leads to a `panic` call at 0xc0000000002ae1e8. The name suggests a PCI register ID read, and the constants look like device or register codes, but the instruction listing does not say which device they belong to.

- `pcireg_stat_get` in `6.5.30/unix.IP35` at 0xc0000000000ac768
  **Inferred.** `pcireg_stat_get` is a 356-byte routine with the same dispatch as `pcireg_id_get` at 0xc0000000000ac608: the same halfword at +0x72, the same constant ladder and the same null check with a panic. The name suggests a status read rather than an ID read. The two differ in their final loads and in the panic text they pass, so this is a sibling of the ID reader. The instruction listing does not show which register it reads.

- `pcireg_nic_addr` in `6.5.30/unix.IP35` at 0xc0000000000b01a0
  **Inferred.** `pcireg_nic_addr` is a 344-byte routine with the same halfword dispatch on +0x72 and the same null-check panic. Its ladder is shorter, testing 0x2, 0x3, 0x0, 0x1 and -1, and its later constant chain (0xe000 through 0xc002, with the 0x24 and 0x36 compares) matches the two PCI register routines above it. The name suggests it returns a network interface address on the bus. The instruction listing shows it returns 0 or an unresolved value, and nothing more about the address.

- `ioc4_eiattach` in `6.5.30/unix.IP35` at 0xc0000000000c22f8
  **Inferred.** `ioc4_eiattach` is a 420-byte attach routine for one IOC4 subdevice, gated on `ioc4_subdev_enabled` at 0xc0000000000c3e84 with subdevice 6. When the subdevice is enabled, it finds the hardware-graph node and memory pointer, allocates a 0x38-byte record and a 0x80000-byte buffer with `kvpalloc`, adds a character device, sets the controller number to 1, and connects an interrupt with `ioc4_intr_connect` at 0xc0000000000c33ac. It calls `ioc4_is_console` at 0xc0000000000c3990, and when that returns zero it sets the controller number to -1. The name suggests an Ethernet-interface attach, but the facts do not show what protocol the subdevice speaks. It returns 0 or -1, and the facts do not show which path produces which value.

- `eiattach` in `6.5.30/unix.IP35` at 0xc0000000000c4454
  **Inferred.** `eiattach` is the IOC3 counterpart of `ioc4_eiattach` above it, gated on `ioc3_subdev_enabled` at 0xc0000000000c62a8 with subdevice 7. It follows the same steps: hardware-graph traversal, memory-pointer lookup, a 0x38-byte record, a 0x80000-byte buffer from `kvpalloc`, a character device, and an interrupt connection through `ioc3_intr_connect` at 0xc0000000000c57b4 with the argument 0x18000000. It calls one unnamed routine at 0xc0000000000c3fac before connecting the interrupt, and it does not call the node-creation helper that the IOC4 copy calls. The name suggests the same Ethernet-interface attach, this time for IOC3.

- `ioc4_pckm_attach` in `6.5.30/unix.IP35` at 0xc0000000000c9274
  **Inferred.** `ioc4_pckm_attach` is a 1424-byte attach routine for the keyboard and mouse ports on IOC4, gated on `ioc4_subdev_enabled` for subdevice 1. It writes IOC4 register offset 0x40 with 1 at the start and with 0 later, allocates three records with `kmem_zalloc`, and sets the keyboard type with `initkbdtype` at 0xc0000000004686a8. It sends the command bytes 0xf5, 0xf4 and 0xf6 through `pckm_pollcmd` at 0xc0000000000ca9a0 and checks the replies against 0xfa. Those byte values match the PS/2 commands for disable scanning, enable scanning and set defaults, with 0xfa as the acknowledge byte; that match is public protocol knowledge, not something the instruction listing shows. The routine also adds two character devices at mode 0666, connects an interrupt, and reads an option string through `kopt_find` at 0xc0000000002c4890 checked with `strstr`. The instruction listing does not show what that option selects.

- `pckm_attach` in `6.5.30/unix.IP35` at 0xc0000000000cb864
  **Inferred.** `pckm_attach` is the IOC3 counterpart of `ioc4_pckm_attach` above it, gated on `ioc3_subdev_enabled` for subdevice 3. It follows the same sequence: a register write of 0x400000 at the start and 0 later through `ioc3_write_ireg` at 0xc0000000000c4ff4, three allocations, the keyboard-type setup, the command polls, two character devices at mode 0666, an interrupt connection through `ioc3_intr_connect` at 0xc0000000000c57b4, and an option check. The polls call unnamed routines at 0xc0000000000ccf98 and 0xc0000000000cd3f8 with the same 0xf5, 0xf4, 0xf6 and 0xfa arguments as the IOC4 copy, so they are probably the same helpers under different names in this binary. The name adds nothing to what the IOC4 copy shows.

- `get_myid` in `6.5.30/unix.IP35` at 0xc0000000000d2c94
  **Inferred.** `get_myid` is a 16-byte routine that loads one doubleword from the fixed uncached address 0x9200000001000020 and returns it. The name suggests it returns a processor or node id, and the address sits in the hub register window, but the instruction listing does not split out any bits of the loaded word, so what it encodes is open. It has no branches and no calls.

- `cn_init` in `6.5.30/unix.IP35` at 0xc0000000000dc9cc
  **Inferred.** `cn_init` is a 384-byte console initializer. It reads and writes fields of the console record at `cn` (offsets +0x8, +0x18, +0x20, +0x48 and +0x4a) and writes bytes into `arg_console`. It calls `gfx_earlyinit` at 0xc000000000469c18, initializes a lock and a mutex, and checks the console option with `is_specified` at 0xc0000000002c4694 before calling `tp_init` at 0xc0000000004699b0 and `du_init` at 0xc000000000227c94. It compares the first byte of the console option with 0x67 and 0x47, the ASCII codes for lowercase and uppercase g, which suggests a graphics console choice. The return value is one of several unresolved values, the `tp_init` result, or the `du_init` result.
