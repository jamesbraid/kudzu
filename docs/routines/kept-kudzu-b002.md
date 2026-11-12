# Tier-1 routine readings kept from t1/kudzu-b2

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

- `prof_intr` in `6.5.22/unix.IP27` at 0xc0000000000489d0
  Role (from name): an interrupt handler named `prof_intr`. It calls `fastick_maint` at 0xc0000000000489e8, `cnode_subnode_to_cpu` at 0xc000000000048a38 and `log_tstamp_event` at 0xc000000000048aa0 (a0=0x6e) and 0xc000000000048abc (a0=0x65, a1=0x6e). It tests bit 0x20 of the doubleword at [-0x3d80] at 0xc0000000000489e0 and again at 0xc000000000048a08, and the `cnode_subnode_to_cpu` call sits on a path that depends on other comparisons. The facts do not say what that flag or the event codes mean.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_mem` in `6.5.22/unix.IP27` at 0xc000000000048fe4
  Named `mdir_xlate_addr_mem`, this routine calls `sprintf` at four sites (0xc000000000049080, 0xc00000000004910c, 0xc000000000049124, 0xc00000000004914c), each with a different fixed buffer address as a1. It reads a word at abs 0x9000000010000090 and branches on it at 0xc000000000049024, and loads a doubleword at 0xc000000000049000 whose address the facts do not show. The facts show no stores, so what it is translating is not established.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_pdir` in `6.5.22/unix.IP27` at 0xc00000000004915c
  Named `mdir_xlate_addr_pdir`, the sibling of the memory variant. It loads a doubleword at 0xc000000000049184 (address not shown in the facts) and calls `sprintf` at 0xc0000000000491e0, 0xc000000000049204 and 0xc000000000049250. A sign test on a2 at 0xc00000000004919c branches to 0xc000000000049214, and a test of a3 at 0xc0000000000491a4 branches to 0xc0000000000491f4. Whether it means a page-directory level comes from the name only.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `mdir_xlate_addr_sdir` in `6.5.22/unix.IP27` at 0xc000000000049260
  Named `mdir_xlate_addr_sdir`, the third member of the group. It loads a doubleword at 0xc00000000004926c (address not shown in the facts), reads a word at abs 0x9000000010000090 at 0xc000000000049290 and branches on it at 0xc0000000000492ac, and calls `sprintf` at four sites from 0xc0000000000492c8 to 0xc000000000049330. The facts show formatting only, and the level name is inferred.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/unix.IP27` at 0xc00000000004cb30
  An exception-entry stub, read as `_ecc_errorvec`. It stores doublewords to abs 0x548, 0x550 and 0x480 at 0xc00000000004cb30 to 0xc00000000004cb38, loads abs 0xfd8 at 0xc00000000004cb60, and branches on that value at 0xc00000000004cb64. It ends in an indirect jump through a register at 0xc00000000004cb78 whose target the facts cannot resolve, so the real handler is unknown and this reading is partial.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/unix.IP27` at 0xc000000000050c4c
  A boot-time probe, read as `earlybadaddr`, of the address passed in a0. It stores a word at abs 0xc0000000014d217c at 0xc000000000050c7c, then reads 1, 2 or 4 bytes from a0, chosen by a1 at 0xc000000000050c84, 0xc000000000050c9c and 0xc000000000050cb4. It calls `cmn_err` at 0xc000000000050ce0 with a0=0 (the format string is not decoded). A test of bit 0x4000 of t1 at 0xc000000000050cf4 branches to 0xc000000000050dc8 when set. It stores a word at 0xc000000000050d14 and returns 0 at 0xc000000000050d24. How t1 is set is not shown, and the facts do not say which condition leads to the `cmn_err` call.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/unix.IP27` at 0xc000000000050d98
  A small routine, read as `baerror`. It loads a doubleword from abs 0xffffffffffffc020 at 0xc000000000050d9c and branches on whether that value is zero at 0xc000000000050da0. On the nonzero path it stores a halfword at offset +0xf6 at 0xc000000000050da8; when the value is zero it branches to 0xc000000000050db4 and stores a word there instead. It returns 1 at 0xc000000000050dc0 and makes no calls. The facts do not say what the stored values record.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/unix.IP27` at 0xc000000000053150
  A data-bus-error vector stub, read as `VEC_dbe`. It loads a halfword from abs 0xffffffffffffc420 at 0xc000000000053150, stores a doubleword at a0+0x140 at 0xc000000000053158, and jumps to `VEC_trap` at 0xc0000000000432c0 with its arguments unchanged (0xc000000000053154). The facts do not show what the halfword holds.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `set_leds` in `6.5.22/unix.IP27` at 0xc000000000056d5c
  Named `set_leds`. It stores a word at 0xc000000000056d60 and tests the byte at [-0x3add] at 0xc000000000056d68. When the byte is nonzero it calls `get_slice` at 0xc000000000056d70 and `get_nasid` at 0xc000000000056d78, loads a word at 0xc000000000056d9c, returns `get_nasid`'s v0 at 0xc000000000056da4 and stores a doubleword at 0xc000000000056da8. When the byte is zero it branches to 0xc000000000056dac and calls `get_slice` there and `get_nasid` at 0xc000000000056db4. The facts do not show the address or the value of the doubleword store, so the name's front-panel LEDs are not borne out.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/unix.IP27` at 0xc000000000056e9c
  A wrapper, read as `badaddr`. It calls `probe_common` at 0xc000000000056eb8 with a stack buffer at sp-48, passes a1 to a3 through, and returns `probe_common`'s v0 at 0xc000000000056ec4. It adds no logic of its own, so the meaning of its result is unknown here: it depends on `probe_common`, which this entry does not read.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/unix.IP27` at 0xc000000000056ecc
  A wrapper, read as `badaddr_val`, with the same shape as `badaddr`. It calls `probe_common` at 0xc000000000056ee8 with the same stack buffer and pass-through arguments, and returns its v0 at 0xc000000000056ef4. The facts show no difference from `badaddr`, so the value-carrying part of the name is not borne out here.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/unix.IP27` at 0xc000000000056efc
  A third wrapper, read as `wbadaddr`, that calls `probe_common` at 0xc000000000056f20 with the same stack buffer and pass-through arguments, then returns v0 at 0xc000000000056f2c. It is 56 bytes against 48 for its two siblings, and the extra bytes contain nothing the facts describe. The write-side name is not visible in the facts.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/unix.IP27` at 0xc00000000005b3b0
  The instruction-bus-error counterpart of `VEC_dbe`, read as `VEC_ibe`. It loads a halfword from abs 0xffffffffffffc420 at 0xc00000000005b3c8, stores a doubleword at a0+0x140 at 0xc00000000005b3d0, and jumps to `VEC_trap` at 0xc0000000000432c0 from 0xc00000000005b3cc. Before that it loads two doublewords at 0xc00000000005b3b0 and 0xc00000000005b3b4, tests the sign of ra at 0xc00000000005b3b8 and branches to the doubleword store at 0xc00000000005b3c4. The facts do not show what the instructions between the test and that store do, or what the test is for.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_id_get` in `6.5.22/unix.IP27` at 0xc000000000080548
  A register accessor, read as `pcireg_id_get`. It branches to the `panic` call at 0xc000000000080698 when a0 is 0 (0xc000000000080550). It loads a halfword at offset +0x72 at 0xc00000000008056c, a doubleword at +0x8 at 0xc000000000080578 and a word at +0x4 at 0xc0000000000805b0 (base registers of the last two are not shown), and compares the constant 0x9200000000000000 with v0 at 0xc000000000080564. The halfword is compared with 3, 2, 0, 1 and -1 and the word with 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002; most mismatches lead to the `panic`, and v0 is returned at 0xc0000000000805a4. The facts do not decode the offsets or the ID field, so its role is unknown.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_stat_get` in `6.5.22/unix.IP27` at 0xc0000000000806a8
  A register accessor, read as `pcireg_stat_get`, with the same shape as `pcireg_id_get`. It branches to the `panic` call at 0xc0000000000807fc when a0 is 0 (0xc0000000000806b0). It loads a halfword at offset +0x72 at 0xc0000000000806cc, a doubleword at +0x8 and a word at +0x4 (base registers of the last two are not shown), and compares the constant 0x9200000000000000 with v0 at 0xc0000000000806c4. The name points to a status register, but the facts show no status bit being tested or returned.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_nic_addr` in `6.5.22/unix.IP27` at 0xc0000000000840e0
  A register accessor, read as `pcireg_nic_addr`, with a shape close to its siblings. It branches to the `panic` call at 0xc000000000084228 when a0 is 0 (0xc0000000000840e8). It loads a halfword at offset +0x72 at 0xc000000000084104, a doubleword at +0x8 and a word at +0x4 (base registers of the last two are not shown), and compares the constant 0x9200000000000000 with v0 at 0xc0000000000840fc. The halfword comparisons use 2 and 3 where the siblings use 3 and 2. The name suggests a network interface address, but the facts show no field and that reading is inferred from the name.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_vect_set` in `6.5.22/unix.IP27` at 0xc000000000086f2c
  An interrupt vector setter by name, read as `pcireg_intr_addr_vect_set`. It compares the constant 0x9200000000000000 with v0 at 0xc000000000086f5c and branches to 0xc000000000086fc8 when a0 is 0. It branches to 0xc00000000008713c when a1 is below 0 or at least 8. It loads a halfword at offset +0x72 at 0xc000000000086f6c and compares a value with 2, 3, 0, 1 and -1. The facts show four stores (two doublewords at 0xc000000000086ff0 and 0xc000000000087008, two words at 0xc000000000087024 and 0xc000000000087038, bases not shown). They also show a word load at 0xc00000000008703c and comparisons with 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002. It calls `assfail` at 0xc000000000087144 with a2=0x85b and `panic` at 0xc000000000087158 with a2 and a4 equal to a1 shifted by 3. What the stores write is not shown.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_addr_set` in `6.5.22/unix.IP27` at 0xc000000000087398
  The sibling setter for the interrupt address field, read as `pcireg_intr_addr_addr_set`. It makes the same checks as `pcireg_intr_addr_vect_set`: the constant 0x9200000000000000 against v0 at 0xc0000000000873c8, a0 equal to 0 at 0xc0000000000873b4, and a1 below 0 or at least 8 at 0xc0000000000873dc and 0xc0000000000873e8. It calls `assfail` at 0xc0000000000875c0 with a2=0x89a and `panic` at 0xc0000000000875d4. The facts show four stores (two doublewords at 0xc000000000087460 and 0xc000000000087478, two words at 0xc000000000087498 and 0xc0000000000874b4, bases not shown), and do not show what they write.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `eiattach` in `6.5.22/unix.IP27` at 0xc000000000095d40
  An attach routine, read as `eiattach`, for an IOC3 sub-device. It calls `ioc3_subdev_enabled` at 0xc000000000095d50 with a1=0x7 and branches to 0xc000000000095ecc when the result is 0. Otherwise it calls `hwgraph_traverse` twice, `hwgraph_fastinfo_get` and `ioc3_mem_ptr`, then `kmem_zalloc` with a0=0x38 and a1=0x1, `spinlock_init`, and `kvpalloc` at 0xc000000000095de0 with a0=0x1, a1=0x80000 and a2=0 (which argument is the size is not decoded). It stores words at offsets +0x2c, +0x34 and +0x38 and other fields at 0xc000000000095db4 to 0xc000000000095e40 (base registers not shown), then calls `hwgraph_char_device_add`, `hwgraph_chmod` with a1=0x124, `hwgraph_fastinfo_set` and `device_inventory_add` with a1=0xe and a2=0x2. It then calls `ioc3_is_console`. When the result is nonzero it calls `device_controller_num_set` with a1=0x1, `ei_make_hwgraph_nodes` and `ioc3_intr_connect` at 0xc000000000095ea8 with a1=0x18000000, and returns 0 or -1 at 0xc000000000095ec4. When the result is 0 it branches to 0xc000000000095ed4 and calls `device_controller_num_set` with a1=-0x1. The name suggests the Ethernet side of the chip, but the facts do not show the sub-device kind, so that is inferred.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)
