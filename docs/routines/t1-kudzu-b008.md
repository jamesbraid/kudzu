# Routine role statements for 24 IP35 kernel routines

These statements describe software routines in the identified binaries, taken from their disassembly facts (registers, calls, constants and branches). They do not describe hardware behavior. Where the facts do not settle a role, the statement says so and leaves it open.

Every entry below is graded inferred. The disassembly facts name the routines from the binary's symbols or the inventory, and none of them is confirmed by a second source.

- `_ecc_errorvec` in `6.5.22/unix.IP35` at 0xc00000000006e640
  **Inferred.** `_ecc_errorvec` stores three doublewords to fixed low addresses (0x4c8, 0x4d0, 0x400) on entry, then loads a doubleword from 0xfd8 and branches on it at 0xc00000000006e674. It leaves through an indirect jump at 0xc00000000006e688 that passes the four argument registers through, and the callee is not resolved. The name suggests an ECC error vector stub that saves state and hands off, but the target of the jump is not in the facts. The last few instructions at 0xc00000000006e690 are not reached from the entry.

- `earlybadaddr` in `6.5.22/unix.IP35` at 0xc00000000007779c
  **Inferred.** `earlybadaddr` dispatches on the value in a1 against 1, 2 and 4, and reads a byte, halfword or word from a0 to match. It calls `cmn_err` at 0xc000000000077830 with a0 set to 0 and a message address at 0xc0000000014b10eb, then returns 0 at 0xc000000000077874. Two word stores go to the no-fault address 0xc00000000159b9e8, and a bit test on 0x4000 at 0xc000000000077844 can send control to `earlybaerror` at 0xc000000000077918. The name suggests an early bad-address check, which fits the width dispatch, but the meaning of the probed value is open.

- `baerror` in `6.5.22/unix.IP35` at 0xc0000000000778e8
  **Inferred.** `baerror` loads a doubleword from the sign-extended low address 0xffffffffffffc020, then conditionally stores a word to 0xffffffffffffc48c and a halfword through a base at offset -0x3fe0. It returns the constant 1. The name suggests a bus-error flag update, but the facts do not show which condition the flags record.

- `earlybaerror` in `6.5.22/unix.IP35` at 0xc000000000077918
  **Inferred.** `earlybaerror` is a short routine: one word store to the no-fault address 0xc00000000159b9e8 and a return of 1 at 0xc000000000077944. It is the same no-fault store that `earlybadaddr` makes, so it reads as the early bus-error counterpart of that routine. The 52-byte body holds nothing else the facts can name.

- `VEC_dbe` in `6.5.22/unix.IP35` at 0xc000000000079600
  **Inferred.** `VEC_dbe` loads a halfword from 0xffffffffffffc428, stores a doubleword to a0 at offset 0x140, and jumps to `VEC_trap` at 0xc000000000065b60 with the four argument registers unchanged. It is a thin data-bus-error vector that records a little state and forwards to the common trap routine. The recorded halfword is not identified by the facts.

- `ecc_handler` in `6.5.22/unix.IP35` at 0xc00000000007bc5c
  **Inferred.** `ecc_handler` is the largest cache-error routine in this batch at 1992 bytes. It reads words from the record in a1 at offsets 0x18, 0x2b8 and 0x2bc, and calls `cer_to_hex` at 0xc00000000007bcb8, `eccf_recoverable_threshold` at 0xc00000000007bd5c, `save_cache_error` at 0xc00000000007bf08, `memerror_handle_uce` at 0xc00000000007c2b0 and `cmn_err_tag` (called from 0xc00000000007c3a4). It also calls `ecc_cacheOp` and `ecc_force_cache_line_invalid` repeatedly, along with `clear_sline` at 0xc00000000007c224, so it both acts on cache lines and reports. The name and callees suggest it sorts recoverable from uncorrectable cache errors, but the branch conditions behind that sorting are only partly in the facts, so the exact policy stays open.

- `set_leds` in `6.5.22/unix.IP35` at 0xc00000000007d2ac
  **Inferred.** `set_leds` works out a node-specific address from `get_slice` and `get_nasid`, then uses `get_chimera_type` and `get_elsc` to pick a path. It makes six `elsc_led_set` calls with the codes 0x102, 0x201, 0x102, 0x200, 0x101 and 0x200, and one `elsc_display_mesg` call with a message address at 0xc0000000014a66c0. The name fits: the routine drives status LEDs and a display message through the controller. What each code means is not in the facts.

- `bte_init` in `6.5.22/unix.IP35` at 0xc00000000007d3fc
  **Inferred.** `bte_init` writes several fields of the structure at a0 (offsets 0x0, 0x18, 0x20, 0x28, 0x38, 0x4c and 0x68) and calls `spinlock_init` at 0xc00000000007d438 on the lock at a0+0x3c. It then loops over a table at 0xc00000000159cf00, comparing a count to 0x21 at 0xc00000000007d4fc, and writes into fixed addresses in the 0x92... range. The name suggests a block transfer engine, but the facts do not show which hardware the stores reach, so that identification stays open.

- `badaddr` in `6.5.22/unix.IP35` at 0xc00000000007edfc
  **Inferred.** `badaddr` sets up a 48-byte frame and calls `probe_common` at 0xc00000000007eecc with a0 set to the frame, a1 through a3 passed through, and returns its result. It is a thin wrapper. The name suggests a probe that tolerates a faulting access, but the probe behavior lives in `probe_common`, which this entry does not cover.

- `badaddr_val` in `6.5.22/unix.IP35` at 0xc00000000007ee2c
  **Inferred.** `badaddr_val` has the same frame setup and the same call to `probe_common` at 0xc00000000007eecc as `badaddr`, and returns that call's result. The name suggests it also gives back the value read, but the facts show no store of a value in this routine, so that part is not established here.

- `wbadaddr` in `6.5.22/unix.IP35` at 0xc00000000007ee5c
  **Inferred.** `wbadaddr` is the same frame and `probe_common` call as `badaddr`, and it returns the call's result. The name suggests a write probe, but this body only passes arguments to the shared probe, so the direction of the access is left to `probe_common`.

- `VEC_ibe` in `6.5.22/unix.IP35` at 0xc000000000083b40
  **Inferred.** `VEC_ibe` is the instruction-side counterpart of `VEC_dbe`. It loads two doublewords, a halfword from 0xffffffffffffc428, stores a doubleword at a0 offset 0x140, branches on the sign of ra at 0xc000000000083b48, and jumps to `VEC_trap` at 0xc000000000065b60. Some of the load and store addresses are unresolved in the facts, so the exact recorded state is unknown.

- `fake_a_part_number` in `6.5.22/unix.IP35` at 0xc0000000000944f4
  **Inferred.** `fake_a_part_number` builds a part-number string in the buffer at a0. It calls `random_eeprom_ch` at 0xc000000000094538, then `get_nasid` and `get_chimera_type` several times, and copies a block of fixed bytes from addresses 0xc000000000446428 through 0xc000000000446478 with unaligned doubleword and word stores at offsets 0x0, 0x7, 0x8, 0xb and 0xc. The source block appears to depend on the node type, since the routine branches on `get_chimera_type` results. The name suggests a placeholder value used when no real part number is available, but the template contents are not in the facts.

- `fake_a_product_name` in `6.5.22/unix.IP35` at 0xc00000000009476c
  **Inferred.** `fake_a_product_name` writes single bytes into the buffer at a1 at offsets 0x9, 0xa and 0xc. The bytes come from fixed addresses in the 0xc000000000446478 to 0xc000000000446494 range, and the routine calls `get_nasid` at 0xc0000000000948b0 and `get_chimera_type` at 0xc0000000000948b8 first. It is the product-name counterpart of `fake_a_part_number`, and the node-type dispatch is similar. The exact string it produces is not established by the facts.

- `gbrick_eeprom_read` in `6.5.22/unix.IP35` at 0xc000000000095fe4
  **Inferred.** `gbrick_eeprom_read` picks a path with `get_chimera_type` and `get_nasid`, then opens a controller channel with `sc_open` at 0xc0000000000d6bd4 using the constant 0xffff000. It reads board information with `read_board_ia` at 0xc000000000095534, closes the channel with `sc_close` at 0xc0000000000d6d14, and calls `get_elsc` at 0xc0000000000960e4. The result is 0 or 1 by the return at 0xc0000000000960dc. The name suggests an EEPROM read for a board, and the controller-channel calls agree with that, but the facts do not confirm which board type the name refers to.

- `nic_get_phase_bits` in `6.5.22/unix.IP35` at 0xc0000000000962c4
  **Inferred.** `nic_get_phase_bits` is a single load from 0x9000000018000078 followed by a return. The return value is unknown in the facts. The name suggests a bit field read for a network interface, but the body alone does not show what the loaded word means.

- `cpuprom_detailed_inventory_info_add` in `6.5.22/unix.IP35` at 0xc000000000098ee4
  **Inferred.** `cpuprom_detailed_inventory_info_add` calls `hubdev_cnodeid_get` at 0xc000000000098ef4, then `klhwg_invent_alloc` at 0xc000000000098f08 with constant sizes 0x15 and 0x20. It fills the new record at offsets 0x4, 0xc, 0x10 and 0x14 with values from two fixed words at 0x90000000180000a0 and 0x90000000180000a4. It then attaches the record with `hwgraph_info_add_LBL` at 0xc000000000098f40 and exports it with `hwgraph_info_export_LBL` at 0xc000000000098f50. The name suggests a CPU prom entry in the inventory, which the allocate-fill-attach sequence supports, though the meaning of the two fixed words is not in the facts.

- `pic_attach2` in `6.5.22/unix.IP35` at 0xc00000000009db24
  **Inferred.** `pic_attach2` is a 3176-byte attach routine with roughly a hundred calls. It reads widget and bridge identity with `pcireg_id_get`, `pcireg_nic_addr` (four calls), `xwidget_info_*` getters and `pcibr_hints_get`, then allocates its per-device record with `kmem_zalloc` and registers the device with `hwgraph_char_device_add`. It goes on to set interrupt state (`pcireg_intr_*` setters and getters), direct-map and ATE state (`pcibr_directmap_init`, `pcireg_int_ate_set`, `rmallocmap`) and connects to the xtalk layer with `xtalk_intr_alloc` and `xtalk_intr_connect`. The name suggests the PIC bridge attach path, and the callees fit that. The disassembly facts give the call graph but not the conditions that pick between these paths, so the detailed behavior stays open.

- `xbridge_attach` in `6.5.22/unix.IP35` at 0xc00000000009eaa8
  **Inferred.** `xbridge_attach` is a 3484-byte attach routine with a shape close to `pic_attach2`. It registers a pciio provider with `pciio_provider_register` at 0xc00000000009eb48 and starts it with `pciio_provider_startup` at 0xc00000000009eb50, and it adds the path with `hwgraph_path_add`. Later it registers an error handler with `xwidget_error_register` at 0xc00000000009f000 and sets control bits with `pcireg_control_bit_set` at 0xc00000000009f040. The name suggests the Xbridge variant of the same attach path, and the shared callees support that, but the facts do not show how the two variants differ in their conditions.

- `pcireg_id_get` in `6.5.22/unix.IP35` at 0xc0000000000ae7d4
  **Inferred.** `pcireg_id_get` reads a halfword at a0 offset 0x72, a doubleword at a0 offset 0x8 and a word at a0 offset 0x4. It then runs a long chain of compares against small constants and against the table values 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002. It calls `panic` at 0xc0000000002700b0 when a0 is zero. The name suggests it returns an identity value for a PCI register set, but the facts do not show what the compare chain produces.

- `pcireg_stat_get` in `6.5.22/unix.IP35` at 0xc0000000000ae934
  **Inferred.** `pcireg_stat_get` has the same argument reads and the same compare chain as `pcireg_id_get`, and its panic call uses a different message address (0xc0000000014a9e60). The shared structure suggests a sibling reader over the same record, and the name suggests a status read. Which field it returns is not established.

- `pcireg_nic_addr` in `6.5.22/unix.IP35` at 0xc0000000000b236c
  **Inferred.** `pcireg_nic_addr` follows the same pattern again: it reads the halfword at a0 offset 0x72, the doubleword at a0 offset 0x8 and a word at a0 offset 0x4, then compares against the same constant set. It calls `panic` at 0xc0000000000b24b4 when a0 is zero. The name suggests it returns an address for a network interface on the widget, but the return value is not established by the facts.

- `pcireg_intr_addr_vect_set` in `6.5.22/unix.IP35` at 0xc0000000000b51b8
  **Inferred.** `pcireg_intr_addr_vect_set` checks its index argument a1. It rejects values below zero or at least 8 (sending control to 0xc0000000000b53c8), and it has an assertion call at 0xc0000000000b53d0 with the constant 0x85b and a `panic` call at 0xc0000000000b53e4 for the out-of-range case. Past that check it runs a loop headed at 0xc0000000000b51fc that reads and writes fields at a0 offset 0x8 and 0x72, comparing values against constants. The name suggests it sets the vector field of an interrupt register, which fits the bounds checks, but the facts do not show which register is written.

- `pcireg_intr_addr_addr_set` in `6.5.22/unix.IP35` at 0xc0000000000b5624
  **Inferred.** `pcireg_intr_addr_addr_set` has the same index check and the same loop shape as `pcireg_intr_addr_vect_set`. Its assertion call uses the constant 0x89a and its panic call sits at 0xc0000000000b5860. The name suggests it sets the address field rather than the vector field of the same kind of register, but the facts do not show the fields it writes.
