# Tier-1 routine readings kept from t1/kudzu-b2

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

- `prof_intr` in `6.5.22/unix.IP27` at 0xc0000000000489d0
  A periodic timer-style handler, read as `prof_intr`. It calls `fastick_maint` at 0xc0000000000489e8, `cnode_subnode_to_cpu` at 0xc000000000048a38 and `log_tstamp_event` at 0xc000000000048aa0 and 0xc000000000048abc, with event codes 0x6e and 0x65. The facts do not say what the flag byte at [-0x3d80] or the event codes mean.
  Grade: inferred.

- `mdir_xlate_addr_mem` in `6.5.22/unix.IP27` at 0xc000000000048fe4
  Named `mdir_xlate_addr_mem`, this routine formats text. It calls `sprintf` at four sites (0xc000000000049080, 0xc00000000004910c, 0xc000000000049124, 0xc00000000004914c), each into a different fixed buffer. It reads a word at abs 0x9000000010000090 and a doubleword at abs 0x9200000001200018, and the facts show no stores, so what it is translating is not established.
  Grade: inferred.

- `mdir_xlate_addr_pdir` in `6.5.22/unix.IP27` at 0xc00000000004915c
  Named `mdir_xlate_addr_pdir`, the sibling of the memory variant. It loads from abs 0x9200000001200018 at 0xc000000000049184 and calls `sprintf` at 0xc0000000000491e0, 0xc000000000049204 and 0xc000000000049250. A sign test on a2 at 0xc00000000004919c chooses between two formatting paths. Whether it means a page-directory level comes from the name only.
  Grade: inferred.

- `mdir_xlate_addr_sdir` in `6.5.22/unix.IP27` at 0xc000000000049260
  Named `mdir_xlate_addr_sdir`, the third member of the group. It loads the same abs 0x9200000001200018 at 0xc00000000004926c, reads abs 0x9000000010000090 at 0xc000000000049290, and calls `sprintf` at four sites from 0xc0000000000492c8 to 0xc000000000049330. The facts show formatting only, and the level name is inferred.
  Grade: inferred.

- `_ecc_errorvec` in `6.5.22/unix.IP27` at 0xc00000000004cb30
  An exception-entry stub, read as `_ecc_errorvec`. It stores doublewords to abs 0x548, 0x550 and 0x480 at 0xc00000000004cb30 to 0xc00000000004cb38, loads abs 0xfd8 at 0xc00000000004cb60, and branches on that value at 0xc00000000004cb64. It ends in an indirect jump through a register at 0xc00000000004cb78 whose target the facts cannot resolve, so the real handler is unknown and this reading is partial.
  Grade: inferred.

- `earlybadaddr` in `6.5.22/unix.IP27` at 0xc000000000050c4c
  A boot-time probe, read as `earlybadaddr`, of the address passed in a0. It reads 1, 2 or 4 bytes from it, chosen by a1 at 0xc000000000050c84, 0xc000000000050c9c and 0xc000000000050cb4. It stores a word to the no-fault slot at abs 0xc0000000014d217c and, on the error path, calls `cmn_err` at 0xc000000000050ce0 and can branch to `earlybaerror` at 0xc000000000050dc8 when bit 0x4000 of t1 is set. How t1 is set, and what the `cmn_err` format says, are not shown.
  Grade: inferred.

- `baerror` in `6.5.22/unix.IP27` at 0xc000000000050d98
  A small error-bookkeeping routine, read as `baerror`. It loads a doubleword from abs 0xffffffffffffc020 at 0xc000000000050d9c, branches on a field loaded through the pointer at [-0x3fe0]:8 at 0xc000000000050da0, stores a halfword and a word to fixed locations at 0xc000000000050da8 and 0xc000000000050db4, and returns 1 at 0xc000000000050dc0. It makes no calls, and the facts do not say what the stored values record.
  Grade: inferred.

- `VEC_dbe` in `6.5.22/unix.IP27` at 0xc000000000053150
  A data-bus-error vector stub, read as `VEC_dbe`. It loads a halfword from abs 0xffffffffffffc420 at 0xc000000000053150, stores a doubleword at a0+0x140 at 0xc000000000053158, and jumps to `VEC_trap` at 0xc0000000000432c0 with its arguments unchanged (0xc000000000053154). The facts do not show what the halfword holds.
  Grade: inferred.

- `set_leds` in `6.5.22/unix.IP27` at 0xc000000000056d5c
  Named `set_leds`, this routine writes one doubleword to an address built from a slice and a node number. It calls `get_slice` at 0xc000000000056d70 and `get_nasid` at 0xc000000000056d78, then stores at 0xc000000000056da8 to an address made from those values plus one of the constants 0x9200000001a20050 or 0x9200000001a20080. A test of the byte at [-0x3add] at 0xc000000000056d68 decides which of the two call sites runs. The name suggests front-panel LEDs, but the facts do not show what value is stored or which device it reaches.
  Grade: inferred.

- `badaddr` in `6.5.22/unix.IP27` at 0xc000000000056e9c
  A wrapper, read as `badaddr`. It calls `probe_common` at 0xc000000000056eb8 with a stack buffer at sp-48, passes a1 to a3 through, and returns `probe_common`'s v0 at 0xc000000000056ec4. It adds no logic of its own, so the meaning of its result is unknown here: it depends on `probe_common`, which this entry does not read.
  Grade: inferred.

- `badaddr_val` in `6.5.22/unix.IP27` at 0xc000000000056ecc
  A wrapper, read as `badaddr_val`, with the same shape as `badaddr`. It calls `probe_common` at 0xc000000000056ee8 with the same stack buffer and pass-through arguments, and returns its v0 at 0xc000000000056ef4. The facts show no difference from `badaddr`, so the value-carrying part of the name is not borne out here.
  Grade: inferred.

- `wbadaddr` in `6.5.22/unix.IP27` at 0xc000000000056efc
  A third wrapper, read as `wbadaddr`, that calls `probe_common` at 0xc000000000056f20 with the same stack buffer and pass-through arguments, then returns v0 at 0xc000000000056f2c. It is 56 bytes against 48 for its two siblings, and the extra bytes contain nothing the facts describe. The write-side name is not visible in the facts.
  Grade: inferred.

- `VEC_ibe` in `6.5.22/unix.IP27` at 0xc00000000005b3b0
  The instruction-bus-error counterpart of `VEC_dbe`, read as `VEC_ibe`. It loads a halfword from the same abs 0xffffffffffffc420 at 0xc00000000005b3c8, stores a doubleword at a0+0x140 at 0xc00000000005b3d0, and jumps to `VEC_trap` at 0xc0000000000432c0 from 0xc00000000005b3cc. It adds a sign test on ra at 0xc00000000005b3b8 that skips one store, and the facts do not say what that test is for.
  Grade: inferred.

- `pcireg_id_get` in `6.5.22/unix.IP27` at 0xc000000000080548
  A register accessor, read as `pcireg_id_get`. It loads a halfword from a0+0x72, a doubleword from a0+0x8 and a word from a0+0x4, and compares a doubleword at abs 0x9200000000000000 against v0 at 0xc000000000080564. It has a `panic` call at 0xc000000000080698 on one path and returns v0 at 0xc0000000000805a4. The facts do not decode the offsets or the ID field, so its role is unknown.
  Grade: inferred.

- `pcireg_stat_get` in `6.5.22/unix.IP27` at 0xc0000000000806a8
  A register accessor, read as `pcireg_stat_get`, with the same shape as `pcireg_id_get`. It reads a0+0x72, a0+0x8 and a0+0x4 and checks the value at abs 0x9200000000000000 at 0xc0000000000806c4, with a `panic` call at 0xc0000000000807fc. The name points to a status register, but the facts show no status bit being tested or returned.
  Grade: inferred.

- `pcireg_nic_addr` in `6.5.22/unix.IP27` at 0xc0000000000840e0
  A register accessor, read as `pcireg_nic_addr`, with the same shape as its siblings. It reads a0+0x72, a0+0x8 and a0+0x4 and makes the same check against abs 0x9200000000000000 at 0xc0000000000840fc, then panics at 0xc000000000084228 on the other path. The name suggests a network interface address, but the facts show no field and that reading is inferred from the name.
  Grade: inferred.

- `pcireg_intr_addr_vect_set` in `6.5.22/unix.IP27` at 0xc000000000086f2c
  An interrupt vector setter by name, read as `pcireg_intr_addr_vect_set`. It checks the value at abs 0x9200000000000000 at 0xc000000000086f5c, asserts with the constant 0x85b through `assfail` at 0xc000000000087144, and panics at 0xc000000000087158 with an index built from a1 shifted by 3. The facts show loads from a0 and no stores, so the setting itself is not in this routine.
  Grade: inferred.

- `pcireg_intr_addr_addr_set` in `6.5.22/unix.IP27` at 0xc000000000087398
  The sibling setter for the interrupt address field, read as `pcireg_intr_addr_addr_set`. It makes the same check against abs 0x9200000000000000 at 0xc0000000000873c8, asserts with the constant 0x89a through `assfail` at 0xc0000000000875c0, and panics at 0xc0000000000875d4. It loads from a0 and does not store, so the same question about where the write happens applies.
  Grade: inferred.

- `eiattach` in `6.5.22/unix.IP27` at 0xc000000000095d40
  An attach routine for an IOC3 sub-device, read as `eiattach`. It checks the sub-device with `ioc3_subdev_enabled` at 0xc000000000095d50 (argument 0x7), looks up its hardware-graph vertices and memory pointer, and allocates a 0x38-byte state block and a 0x80000-byte region through `kvpalloc`. It writes fields into the new block and into the mapped IOC3 block at offsets 0x2c to 0x38, then creates a character device and connects the chip interrupt at 0xc000000000095ea8 with address 0x18000000. The name suggests the Ethernet side of the chip, but the facts do not show the sub-device kind, so that is inferred.
  Grade: inferred.
