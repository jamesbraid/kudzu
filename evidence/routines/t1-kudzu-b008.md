# Routine role statements for 24 IP35 kernel routines

These statements describe software routines in the identified binaries, taken from their disassembly facts (registers, calls, constants and branches). They do not describe hardware behavior. Where the facts do not settle a role, the statement says so and leaves it open.

Every entry below is graded inferred. The disassembly facts name the routines from the binary's symbols or the inventory, and none of them is confirmed by a second source.

- `_ecc_errorvec` in `6.5.22/unix.IP35` at 0xc00000000006e640
  **Inferred.** `_ecc_errorvec` stores three doublewords to fixed low addresses (0x4c8, 0x4d0, 0x400) on entry, then loads a doubleword from 0xfd8 and branches on it at 0xc00000000006e674; the branch is taken when the value is nonzero and skips only the instructions that set a register from a constant. Both paths reach an indirect jump at 0xc00000000006e688 that passes the four argument registers through, and the callee is not resolved. The name suggests an ECC error vector stub that saves state and hands off, but the target of the jump is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/unix.IP35` at 0xc00000000007779c
  **Inferred.** `earlybadaddr` stores a word to the fixed address 0xc00000000159b9e8 near its start, then compares a1 against 1, 2 and 4 and reads a byte, halfword or word from a0 to match. When a1 is none of these it calls `cmn_err` at 0xc000000000077830 with a0 set to 0 and a1 set to 0xc0000000014b10eb. After the read or the call it tests bit 0x4000 of a register value at 0xc000000000077844. If the bit is set, control goes to 0xc000000000077918 (`earlybaerror`). If it is clear, a second word store goes to the same fixed address and the routine returns 0 at 0xc000000000077874. The name suggests an early bad-address check, which fits the width dispatch, but the meaning of the probed value and of the 0x4000 bit is open.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/unix.IP35` at 0xc0000000000778e8
  **Inferred.** `baerror` loads a doubleword from the sign-extended low address 0xffffffffffffc020. When that value is nonzero it stores a halfword at offset 0xf6 from it; on either path it then stores a word to 0xffffffffffffc48c and returns the constant 1. The name suggests a bus-error flag update, but the facts do not show what the loaded value or the stored fields mean.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/unix.IP35` at 0xc000000000077918
  **Inferred.** `earlybaerror` is a short routine: one word store to the fixed address 0xc00000000159b9e8 and a return of 1 at 0xc000000000077944. It stores to the same address as `earlybadaddr`, so the name suggests it is the early bus-error counterpart of that routine. The 52-byte body holds nothing else the facts can name.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/unix.IP35` at 0xc000000000079600
  **Inferred.** `VEC_dbe` loads a halfword from 0xffffffffffffc428 and stores it as a doubleword to a0 at offset 0x140. It jumps to `VEC_trap` at 0xc000000000065b60 with the four argument registers unchanged. The name suggests a data-bus-error vector that records a little state and forwards to the common trap routine; the facts do not identify the halfword.
  Reviewed: 2026-11-12 (sonnet)

- `ecc_handler` in `6.5.22/unix.IP35` at 0xc00000000007bc5c
  **Inferred.** `ecc_handler` is a 1992-byte routine. It reads words from the record in a1 at offsets 0x18, 0x2b8 and 0x2bc, and most of its calls sit behind branches on those words. It calls `cer_to_hex` at 0xc00000000007bcb8 (only when a bit test on the word at offset 0x18 passes), `eccf_recoverable_threshold` at 0xc00000000007bd5c, `save_cache_error` at 0xc00000000007bf08, `memerror_handle_uce` at 0xc00000000007c2b0 and `cmn_err_tag` (called from 0xc00000000007c3a4). It also calls `ecc_cacheOp` and `ecc_force_cache_line_invalid` repeatedly, along with `clear_sline` in a loop at 0xc00000000007c224, so it both acts on cache lines and reports. The name and callees suggest it sorts recoverable from uncorrectable cache errors, but the branch conditions behind that sorting are only partly in the facts, so the exact policy stays open.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/unix.IP35` at 0xc00000000007d2ac
  **Inferred.** `set_leds` stores a0 to a fixed low address at entry, calls `get_slice` and `get_nasid`, and stores a doubleword to an address built from those results plus 0x9000000030000000. It then branches at 0xc00000000007d2f8 on a loaded global value against 0x4000. Depending on that and on a0 being zero, it calls `get_elsc` and then `elsc_led_set` with the codes 0x102, 0x201, 0x102, 0x200, 0x101 and 0x200 (six calls in the routine, spread over separate paths rather than made in one run). `get_chimera_type` is called only on the path where the global value is not 0x4000. A single `elsc_display_mesg` call with a message address at 0xc0000000014a66c0 sits on the path where `get_chimera_type` returns 3 and a0 is nonzero. The name suggests the routine drives status LEDs and a display message through the controller; what each code means is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `bte_init` in `6.5.22/unix.IP35` at 0xc00000000007d3fc
  **Inferred.** `bte_init` writes several fields of the structure at a0 (offsets 0x0, 0x18, 0x20, 0x28, 0x38, 0x4c and 0x68) and calls `spinlock_init` at 0xc00000000007d438 on the lock at a0+0x3c. A branch on a2 at 0xc00000000007d498 selects between two store sequences, both of which store a word at offset 0x4c. A signed compare of the global word at 0xc00000000159ab94 against 0x21 at 0xc00000000007d4fc decides whether the field at offset 0x68 is stored a second time: it is when the word is below 0x21. The routine returns a0 at 0xc00000000007d53c. The name suggests a block transfer engine initialiser, but the facts do not show which hardware the stores reach, so that identification stays open.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/unix.IP35` at 0xc00000000007edfc
  **Inferred.** `badaddr` sets up a 48-byte frame and calls `probe_common` at 0xc00000000007eecc with a0 set to the frame, a1 through a3 passed through, and returns its result. It is a thin wrapper. The name suggests a probe that tolerates a faulting access, but the probe behavior lives in `probe_common`, which this entry does not cover.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/unix.IP35` at 0xc00000000007ee2c
  **Inferred.** `badaddr_val` has the same frame setup and the same call to `probe_common` at 0xc00000000007eecc as `badaddr`, and returns that call's result. The name suggests it also gives back the value read, but the facts show no store of a value in this routine, so that part is not established here.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/unix.IP35` at 0xc00000000007ee5c
  **Inferred.** `wbadaddr` is the same frame and `probe_common` call as `badaddr`, and it returns the call's result. The name suggests a write probe, but this body only passes arguments to the shared probe, so the direction of the access is left to `probe_common`.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/unix.IP35` at 0xc000000000083b40
  **Inferred.** `VEC_ibe` loads two doublewords, branches on the sign of the first (ra) at 0xc000000000083b48, stores a doubleword at an address the facts leave unresolved, loads a halfword from 0xffffffffffffc428, stores a doubleword to a0 at offset 0x140, and jumps to `VEC_trap` at 0xc000000000065b60. a0, a1 and a3 reach `VEC_trap` unchanged; a2 is not shown to. The shared halfword load, the store at a0+0x140 and the `VEC_trap` tail jump, together with the name, suggest it is the instruction-side counterpart of `VEC_dbe`, but the exact recorded state is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `fake_a_part_number` in `6.5.22/unix.IP35` at 0xc0000000000944f4
  **Inferred.** `fake_a_part_number` writes bytes into the buffer at a0 and splits on a1 against 0x100 at 0xc000000000094528. On the a1 != 0x100 path it loops, calling `random_eeprom_ch` at 0xc000000000094538 with a0 set to 0 and storing each result as a byte, until a counter reaches 12; positions 3 and 8 are compared for separately and get a different store. The other path calls `get_nasid` and `get_chimera_type` four times each, compares the type results against 3, 1, 2 and 5 and a loaded global value against 0x4000 and 0x8000, and copies a 13-byte block from a table reached through a global pointer, with unaligned doubleword and word stores at offsets 0x0, 0x7, 0x8 and 0xb and a byte store at 0xc. Which block is copied depends on those comparisons. The name suggests a placeholder value used when no real part number is available, but the template contents are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `fake_a_product_name` in `6.5.22/unix.IP35` at 0xc00000000009476c
  **Inferred.** `fake_a_product_name` masks a code from a2 with 0xf00 and compares the result against 0x100, 0x200 and 0x400 (branches at 0xc0000000000947a4, 0xc0000000000947b4 and 0xc0000000000947bc). It stores into the buffers passed in a0 and a1 with byte stores (offsets 0x0, 0x4, 0x8, 0x9, 0xa and 0xc) and unaligned word and doubleword stores, copying from a table reached through a global pointer. It also compares a loaded global value against 0x4000 and 0x8000; on the path where it matches neither, it calls `get_nasid` at 0xc0000000000948b0 and `get_chimera_type` at 0xc0000000000948b8 once each and branches on whether the type is zero. The name suggests it is the product-name counterpart of `fake_a_part_number`, and the dispatch shape is similar, but the exact string it produces is not established by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `gbrick_eeprom_read` in `6.5.22/unix.IP35` at 0xc000000000095fe4
  **Inferred.** `gbrick_eeprom_read` calls `get_chimera_type` on a1 and then `get_nasid`, and branches on bit 0x800 of a2 and on a1 against the `get_nasid` result. When bit 0x800 of a2 is set and a1 differs from that result, it opens a controller channel with `sc_open` at 0xc0000000000d6bd4 using the constant 0xffff000, and a negative return branches away to another path. Otherwise, and when a loaded global value is not 0x4000, it reads board information with `read_board_ia` at 0xc000000000095534, passing the channel handle as the second argument, closes the channel with `sc_close` at 0xc0000000000d6d14, and returns at 0xc0000000000960dc a value that is 0, 1 or one the facts leave unresolved. `get_elsc` at 0xc0000000000960e4 is on a separate path, taken when a1 equals the `get_nasid` result. The name suggests an EEPROM read for a board, and the controller-channel calls agree with that, but the facts do not confirm which board type the name refers to.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_phase_bits` in `6.5.22/unix.IP35` at 0xc0000000000962c4
  **Inferred.** `nic_get_phase_bits` has one load and a return of a value the facts leave unresolved, with no stores or calls. The name suggests a read of phase bits for a NIC, but the facts show neither what is loaded nor how the returned value is formed.
  Reviewed: 2026-11-12 (sonnet)

- `cpuprom_detailed_inventory_info_add` in `6.5.22/unix.IP35` at 0xc000000000098ee4
  **Inferred.** `cpuprom_detailed_inventory_info_add` calls `hubdev_cnodeid_get` at 0xc000000000098ef4 on a1, then `klhwg_invent_alloc` at 0xc000000000098f08 with constant sizes 0x15 and 0x20. It writes the new record at offsets 0x4 (byte), 0xc, 0x10 and 0x14 (words). The facts resolve the two words passed on to the next call as the contents of 0x90000000180000a0 (a3) and 0x90000000180000a4 (a4), and the words stored at offsets 0xc and 0x10 are loaded from these two locations; the values stored at offsets 0x4 and 0x14 are not resolved in the facts. It then attaches the record with `hwgraph_info_add_LBL` at 0xc000000000098f40 and exports it with `hwgraph_info_export_LBL` at 0xc000000000098f50, returning that call's result. The name suggests a CPU prom entry in the inventory, which the allocate-fill-attach sequence supports, though the meaning of the two fixed words is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `pic_attach2` in `6.5.22/unix.IP35` at 0xc00000000009db24
  **Inferred.** `pic_attach2` is a 3176-byte attach routine with 100 calls. Early on it registers a character device with `hwgraph_char_device_add` (the third call), then reads identity with `pcireg_id_get`, `pcireg_nic_addr` (four calls over the whole routine) and `pcibr_hints_get`, and allocates its per-device record with `kmem_zalloc` (0xac0 bytes) before calling the `xwidget_info_*` getters. It goes on to set interrupt state (`pcireg_intr_*` setters and getters), direct-map and ATE state (`pcibr_directmap_init`, `pcireg_int_ate_set`, `rmallocmap`) and connects to the xtalk layer with `xtalk_intr_alloc` and `xtalk_intr_connect`. The name suggests the PIC bridge attach path, and the callees fit that. The disassembly facts give the call graph but not the conditions that pick between these paths, so the detailed behavior stays open.
  Reviewed: 2026-11-12 (sonnet)

- `xbridge_attach` in `6.5.22/unix.IP35` at 0xc00000000009eaa8
  **Inferred.** `xbridge_attach` is a 3484-byte attach routine with a shape close to `pic_attach2`. It registers a pciio provider with `pciio_provider_register` at 0xc00000000009eb48 and starts it with `pciio_provider_startup` at 0xc00000000009eb50, and it adds the path with `hwgraph_path_add`. Later it registers an error handler with `xwidget_error_register` at 0xc00000000009f000 and sets control bits with `pcireg_control_bit_set` at 0xc00000000009f040. The name suggests the Xbridge variant of the same attach path, and the shared callees support that, but the facts do not show how the two variants differ in their conditions.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_id_get` in `6.5.22/unix.IP35` at 0xc0000000000ae7d4
  **Inferred.** `pcireg_id_get` reads a halfword at a0 offset 0x72, a doubleword at a0 offset 0x8 and a word at a0 offset 0x4. It then runs a long chain of compares against small constants and against the table values 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002. The `panic` call at 0xc0000000000ae924 (callee 0xc0000000002700b0) is the target of the a0 == 0 branch and also of the branches where the halfword is -1 or the compare chain finds no match, so it is not only the null case. The name suggests it returns an identity value for a PCI register set, but the facts do not show what the compare chain produces.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_stat_get` in `6.5.22/unix.IP35` at 0xc0000000000ae934
  **Inferred.** `pcireg_stat_get` has the same argument reads and the same compare chain as `pcireg_id_get`, and its panic call uses a different message address (0xc0000000014a9e60). The shared structure suggests a sibling reader over the same record, and the name suggests a status read. Which field it returns is not established.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_nic_addr` in `6.5.22/unix.IP35` at 0xc0000000000b236c
  **Inferred.** `pcireg_nic_addr` follows the same pattern again: it reads the halfword at a0 offset 0x72, the doubleword at a0 offset 0x8 and a word at a0 offset 0x4, then compares against the same constant set. Its `panic` call at 0xc0000000000b24b4 is the target of the a0 == 0 branch and also of the branches where the halfword is -1 or the compare chain finds no match. The name suggests it returns an address for a network interface on the widget, but the return value is not established by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_vect_set` in `6.5.22/unix.IP35` at 0xc0000000000b51b8
  **Inferred.** `pcireg_intr_addr_vect_set` checks its index argument a1. Values below zero or at least 8 branch to 0xc0000000000b53c8, where an assertion call (`assfail` at 0xc0000000000b53d0, with the constant 0x85b) is made. A `panic` call at 0xc0000000000b53e4 is reached on a different branch, taken when the halfword read from a0 offset 0x72 equals -1. Past the index check it reads a doubleword at a0 offset 0x8 and that halfword, then runs a loop headed at 0xc0000000000b51fc whose compare chain tests a word loaded at offset 0x4 of a base register against 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002, with loads and stores whose addresses the facts do not resolve. The name suggests it sets the vector field of an interrupt register, which fits the bounds checks, but the facts do not show which register is written.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_addr_set` in `6.5.22/unix.IP35` at 0xc0000000000b5624
  **Inferred.** `pcireg_intr_addr_addr_set` has the same index check and the same loop shape as `pcireg_intr_addr_vect_set`. Its assertion call uses the constant 0x89a and its panic call sits at 0xc0000000000b5860. The name suggests it sets the address field rather than the vector field of the same kind of register, but the facts do not show the fields it writes.
  Reviewed: 2026-11-12 (sonnet)
