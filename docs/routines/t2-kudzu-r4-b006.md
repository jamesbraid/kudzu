# Tier-2 routine readings, kudzu round 4, batch 6

Role statements for routines 151 to 175 of the tier-2 list, written from the rendered
instructions only: loads, stores, calls, branches and constants. Every grade is inferred.
Where the facts stop short of the meaning, the statement says so. All 25 routines are in
the `6.5.22/unix.IP27` build and every address is in that binary.

Shared shape across the family: arg0 is a record, and a null arg0 leads to a panic call.
Each routine loads the halfword at +0x72, the word at +0x4 and the doubleword at +0x8 of
that record. The halfword is compared with 0 to 3. Each routine also compares values the
facts do not resolve with 0xe000, 0xe010, 0xd102, 0xd112, 0xd002, 0xc002 and 0x36, so the
meaning of those constants is not settled here. The `_get` routines only load. The
`_set` and `_bit_` routines also store. The rendered facts do not list arithmetic, so the
bit operations in the `_bit_` routines are not shown. The `addr_` and `force_` routines
also range-check arg1 and send a negative value to an assertion call.

- `pcireg_intr_reset_set` in `6.5.22/unix.IP27` at 0xc000000000085608
  **Inferred.** Writes a record at arg0 after the shared record checks, with a null arg0 going to a panic call at 0xc000000000085750. It compares the halfword at +0x72 with -1, 0, 1, 2 and 3 to choose a path, then stores a word at 0xc0000000000856cc and doublewords at 0xc0000000000856e8 and 0xc000000000085704. The stored values are not resolved by these facts, and the field it targets is not settled here.

- `pcireg_intr_reset_bit_set` in `6.5.22/unix.IP27` at 0xc000000000085760
  **Inferred.** Follows the same record checks as its set sibling, with the panic call at 0xc0000000000858c4. It loads through pointers the facts do not resolve, then stores a word at 0xc000000000085830 and doublewords at 0xc000000000085854 and 0xc000000000085878. The bit operation itself is not shown in the rendered facts, so which bits change is unknown.

- `pcireg_intr_mode_get` in `6.5.22/unix.IP27` at 0xc0000000000858d4
  **Inferred.** Reads a record at arg0 and returns a value without storing to it, with a null arg0 going to a panic call at 0xc000000000085a2c. It loads the halfword at +0x72, word at +0x4 and two doublewords from unresolved pointers, and returns either 0 or an unresolved value. The facts do not say which mode field is read or what the returned value means.

- `pcireg_intr_mode_set` in `6.5.22/unix.IP27` at 0xc000000000085a3c
  **Inferred.** Writes a record at arg0 using the same halfword dispatch as the mode getter at 0xc0000000000858d4, with a null arg0 going to a panic call at 0xc000000000085b88. It stores a word at 0xc000000000085b04 and doublewords at 0xc000000000085b20 and 0xc000000000085b3c. The value written and the mode field it lands in are not resolved by these facts.

- `pcireg_intr_mode_bit_set` in `6.5.22/unix.IP27` at 0xc000000000085b98
  **Inferred.** Reads the same record fields as the mode setter and stores a word at 0xc000000000085c68 and doublewords at 0xc000000000085c8c and 0xc000000000085cb0 through unresolved pointers. Its panic call is at 0xc000000000085cfc and it returns an unresolved value or the panic result. The bit operation is not shown in the rendered facts.

- `pcireg_intr_mode_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000085d0c
  **Inferred.** Clears a bit in a mode field of the record at arg0, using the same record reads and unresolved stores as the mode bit setter at 0xc000000000085b98. Its panic call is at 0xc000000000085e78 and it can return the constant 0x3 on one path. The bit operation itself is not shown in the rendered facts, so the cleared bit is unknown.

- `pcireg_intr_device_get` in `6.5.22/unix.IP27` at 0xc000000000085e88
  **Inferred.** Reads a device field from the record at arg0 and stores nothing: it loads the halfword at +0x72, word at +0x4 and doubleword at +0x8, plus two doublewords through unresolved pointers. It returns 0 or an unresolved value, with a panic call at 0xc000000000085fe0 on the bad-record path. The facts do not say which device field is read.

- `pcireg_intr_device_set` in `6.5.22/unix.IP27` at 0xc000000000085ff0
  **Inferred.** Writes a device field in the record at arg0 using the same record reads as the device getter at 0xc000000000085e88. It stores a word at 0xc0000000000860b8 and doublewords at 0xc0000000000860d4 and 0xc0000000000860f0, with a panic call at 0xc00000000008613c. The stored values and the target field are not resolved by these facts.

- `pcireg_intr_device_bit_set` in `6.5.22/unix.IP27` at 0xc00000000008614c
  **Inferred.** Reads the record at arg0 with the same field reads as the device setter and stores a word at 0xc00000000008621c and doublewords at 0xc000000000086240 and 0xc000000000086264. The panic call is at 0xc0000000000862b0 and it returns an unresolved value or the panic result. The bit operation is not shown in the rendered facts.

- `pcireg_intr_device_bit_clr` in `6.5.22/unix.IP27` at 0xc0000000000862c0
  **Inferred.** Clears a bit in a device field of the record at arg0, with the same reads and unresolved stores as the device bit setter at 0xc00000000008614c. Its panic call is at 0xc00000000008642c and it can return the constant 0x3 on one path. The bit operation itself is not shown in the rendered facts, so the cleared bit is unknown.

- `pcireg_intr_host_err_get` in `6.5.22/unix.IP27` at 0xc00000000008643c
  **Inferred.** Reads a host-error field from the record at arg0 and stores nothing: it loads the halfword at +0x72, word at +0x4 and doubleword at +0x8, and two doublewords through unresolved pointers. It returns 0 or an unresolved value, with a panic call at 0xc000000000086594 on the bad-record path. The facts do not say which field is read.

- `pcireg_intr_host_err_set` in `6.5.22/unix.IP27` at 0xc0000000000865a4
  **Inferred.** Writes a host-error field in the record at arg0 with the same reads as the host-error getter at 0xc00000000008643c. It stores a word at 0xc00000000008666c and doublewords at 0xc000000000086688 and 0xc0000000000866a4, and it panics at 0xc0000000000866f0 on the bad-record path. The stored values are not resolved by these facts.

- `pcireg_intr_addr_get` in `6.5.22/unix.IP27` at 0xc000000000086700
  **Inferred.** Reads an address field from the record at arg0 and stores nothing. A negative arg1 goes to an assertion call at 0xc0000000000868ec, and the halfword at +0x72 is compared with 0 to 3 to choose among paths. It returns 0 or an unresolved value, and the facts do not show what arg1 indexes or which address is returned.

- `pcireg_intr_addr_set` in `6.5.22/unix.IP27` at 0xc000000000086910
  **Inferred.** Writes an address field in the record at arg0. A negative arg1 or one of 8 or more goes to an assertion call at 0xc000000000086af8, and the halfword at +0x72 selects among store paths. It stores a word at 0xc0000000000869cc and doublewords at 0xc000000000086a38 and 0xc000000000086a44, with a panic call at 0xc000000000086b0c on the failure path. Whether the store is indexed by arg1 is not shown.

- `pcireg_intr_addr_addr` in `6.5.22/unix.IP27` at 0xc000000000086b1c
  **Inferred.** Reads an address-of-address field from the record at arg0 and stores nothing. A negative arg1 or one of 8 or more goes to an assertion call at 0xc000000000086d04, and the halfword at +0x72 is compared with 0 to 3 to choose a path. It returns 0 or an unresolved value, and the facts do not settle what the field holds.

- `pcireg_intr_addr_vect_get` in `6.5.22/unix.IP27` at 0xc000000000086d28
  **Inferred.** Reads a vector field from the record at arg0 and stores nothing. A negative arg1 or one of 8 or more goes to an assertion call at 0xc000000000086f08, and the halfword at +0x72 is compared with 0 to 3 to choose a path. It returns 0 or an unresolved value, and the vector's meaning is not settled by these facts.

- `pcireg_intr_addr_addr_get` in `6.5.22/unix.IP27` at 0xc000000000087168
  **Inferred.** Reads an address-of-address field from the record at arg0 and stores nothing, loading three words and doublewords through unresolved pointers. A negative arg1 goes to an assertion call at 0xc000000000087374, and the halfword at +0x72 is compared with 0 to 3 to choose a path. It returns 0 or an unresolved value, and the facts do not settle which field it reads.

- `pcireg_intr_view_get` in `6.5.22/unix.IP27` at 0xc0000000000875e4
  **Inferred.** Reads a view field from the record at arg0 and stores nothing, loading the halfword at +0x72, doubleword at +0x8 and word at +0x4, plus further unresolved loads. Its panic call at 0xc00000000008773c passes a fourth argument that is one of -1, 0x24 or 0xe010, and it returns 0 or an unresolved value. The view that is selected is not settled by these facts.

- `pcireg_intr_multiple_get` in `6.5.22/unix.IP27` at 0xc00000000008774c
  **Inferred.** Reads a multiple-interrupt field from the record at arg0 and stores nothing, with the same halfword, doubleword and word reads as the view getter at 0xc0000000000875e4. Its panic call at 0xc0000000000878a0 uses the same fourth-argument set, and it returns 0 or an unresolved value. The facts do not settle what the field counts or selects.

- `pcireg_force_always_set` in `6.5.22/unix.IP27` at 0xc0000000000878b0
  **Inferred.** Writes a forced-interrupt field in the record at arg0. A negative arg1 or one of 8 or more goes to an assertion call at 0xc000000000087aa4, and the halfword at +0x72 selects among store paths. It stores doublewords at 0xc0000000000879cc and 0xc0000000000879d8 and a word at 0xc0000000000879e4, with panic calls at 0xc000000000087abc and 0xc000000000087ac8 on other paths. The stored values and the forced condition are not resolved by these facts.

- `pcireg_force_always_addr_get` in `6.5.22/unix.IP27` at 0xc000000000087ad8
  **Inferred.** Reads a forced-interrupt address from the record at arg0 and stores nothing. A negative arg1 goes to an assertion call at 0xc000000000087cb4, and the halfword at +0x72 is compared with 0 to 3 to choose a path. Panic calls at 0xc000000000087cc8 and 0xc000000000087cd4 cover failure paths, and the routine returns 0 or an unresolved value. The facts do not say which address is returned.

- `pcireg_force_intr_set` in `6.5.22/unix.IP27` at 0xc000000000087ce4
  **Inferred.** Writes a forced-interrupt request into the record at arg0. A negative arg1 goes to an assertion call at 0xc000000000087ed0, and the halfword at +0x72 is compared with 0 to 3 to pick a store path. It stores doublewords at 0xc000000000087dfc and 0xc000000000087e08 and a word at 0xc000000000087e14, with panics at 0xc000000000087ee8 and 0xc000000000087ef4. The stored request value is not resolved by these facts.

- `pcireg_device_get` in `6.5.22/unix.IP27` at 0xc000000000087f04
  **Inferred.** Reads a device field from the record at arg0 and stores nothing, loading the halfword at +0x72, doubleword at +0x8 and word at +0x4, plus several unresolved loads. It makes one panic call at 0xc0000000000880c0 and three assertion calls at 0xc0000000000880dc, 0xc0000000000880f8 and 0xc000000000088114, and returns 0 or an unresolved value. The facts do not say which device field is read.

- `pcireg_device_set` in `6.5.22/unix.IP27` at 0xc000000000088124
  **Inferred.** Writes a device field in the record at arg0 with the same reads as the device getter at 0xc000000000087f04. It stores a word at 0xc000000000088224 and doublewords at 0xc000000000088258 and 0xc000000000088290, with a panic call at 0xc0000000000882dc and assertion calls at 0xc0000000000882f8, 0xc000000000088314 and 0xc000000000088330. The stored values are not resolved by these facts.

- `pcireg_device_bit_set` in `6.5.22/unix.IP27` at 0xc000000000088340
  **Inferred.** Reads the device record at arg0 with the same field reads as the device setter and stores a word at 0xc000000000088444 and doublewords at 0xc000000000088484 and 0xc0000000000884c8 through unresolved pointers. Its panic call is at 0xc000000000088510 with assertion calls at 0xc00000000008852c, 0xc000000000088548 and 0xc000000000088564. The bit operation is not shown in the rendered facts, so which bits change is unknown.
