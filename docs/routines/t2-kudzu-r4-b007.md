# Tier-2 routine readings, kudzu round 4, batch 7

Role statements for targets 175 to 199 of the ranked tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Names come from the binary's symbol table and are not checked
against any other source. Offsets such as +0x72 are byte offsets from the pointer
named in the sentence.

- `pcireg_device_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000088574
  **Inferred.** Role (from name): the name suggests it clears bits in a bridge device register for the record passed in a0; the facts do not show which register or what is cleared. It loads the halfword at +0x72 of a0 and branches on the values 0x3, 0x2, 0, 1 and -1; the -1 case reaches panic at 0xc00000000008874c. It also loads the word at +0x4 of a0 (0xc0000000000885f4) and compares that word, not the halfword, with 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002 to pick a path. The paths that load and store do so through pointers the facts do not resolve: a word load at 0xc000000000088670 with a store at 0xc000000000088680 on one path, and doubleword loads and stores at 0xc0000000000886ac/0xc0000000000886bc and 0xc0000000000886f0/0xc000000000088700 on others. The operation applied to the loaded value is an unknown sub-fact. Three assfail calls (0xc000000000088768, 0xc000000000088784, 0xc0000000000887a0) are further exits.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_wrb_flush_get` in `6.5.22/unix.IP27` at 0xc0000000000887b0
  **Inferred.** Role (from name): the name suggests a read-only accessor for write-buffer flush state of the record in a0; the facts show no store on any path. It loads the halfword at +0x72 of a0, a doubleword at +0x8 and a word at +0x4, then loads through computed pointers at 0xc000000000088894 (word), 0xc000000000088900 and 0xc00000000008893c (doublewords). The assfail calls at 0xc0000000000889a8, 0xc0000000000889bc, 0xc0000000000889d8 and 0xc0000000000889f4 and the panic at 0xc00000000008898c are the failure exits; the facts do not say what each check tests beyond the visible comparisons. The normal return at 0xc0000000000888c0 yields a value the facts do not resolve.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_rrb_get` in `6.5.22/unix.IP27` at 0xc000000000088a04
  **Inferred.** Role (from name): the name suggests it reads a read-response-buffer setting from the record in a0; the facts do not show which setting. It loads a doubleword at +0x8 and the halfword at +0x72 of a0, a word at +0x4, and then loads through pointers at 0xc000000000088ab4 (word), 0xc000000000088b30 and 0xc000000000088b40 (doublewords). The failure path calls assfail at 0xc000000000088bf0 and panics at 0xc000000000088c04; -0x1 and 0x3 left in a4 and a5 there are leftover comparison constants, not arguments of either call. The normal return at 0xc000000000088a94 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_rrb_set` in `6.5.22/unix.IP27` at 0xc000000000088c14
  **Inferred.** Role (from name): the name suggests the write side of pcireg_rrb_get, setting a read-response-buffer value for the record in a0; the facts do not show what is set. It loads the halfword at +0x72 and doubleword at +0x8 of a0, then, depending on the word at +0x4, stores a word at 0xc000000000088ccc or doublewords at 0xc000000000088d38 or 0xc000000000088d44; the facts do not resolve the store addresses or values. Its failure path calls assfail at 0xc000000000088df8 and panic at 0xc000000000088e0c, and the return at 0xc000000000088ca8 yields an unresolved value or the result of panic.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_rrb_bit_set` in `6.5.22/unix.IP27` at 0xc000000000088e1c
  **Inferred.** Role (from name): the name suggests it sets bits in a read-response-buffer register for the record in a0; the facts do not show the register or the bits. It loads the halfword at +0x72 and word at +0x4 of a0. Depending on the word at +0x4, one path loads a word at 0xc000000000088eec and stores a word at 0xc000000000088ef8, and others load and store doublewords (0xc000000000088f18/0xc000000000088f24 and 0xc000000000088f44/0xc000000000088f50) through computed pointers; the facts do not show that the stored value is derived from the loaded one, and the combining operation is an unknown sub-fact. The panic at 0xc000000000088f9c passes a1 and a2 through unchanged; the -0x1 left in a6 there is a leftover comparison constant. The return at 0xc000000000088e7c yields an unresolved value or the result of panic.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_rrb_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000088fac
  **Inferred.** Role (from name): the name suggests the clear counterpart of pcireg_rrb_bit_set; the facts show the same record layout, a halfword at +0x72, a doubleword at +0x8 and a word at +0x4 of a0, but not which bits are cleared. Depending on the word at +0x4, one path loads a word at 0xc000000000089078 and stores a word at 0xc000000000089088, and others load and store doublewords (0xc0000000000890a8/0xc0000000000890b8 and 0xc0000000000890d8/0xc0000000000890e8); the facts do not show that the stored value is derived from the loaded one. The panic at 0xc000000000089134 passes a1 and a2 through unchanged; the -0x1 left in a6 there is a leftover comparison constant. The return at 0xc000000000089008 yields 0x3, an unresolved value or the result of panic.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_rrb_status_get` in `6.5.22/unix.IP27` at 0xc000000000089144
  **Inferred.** Role (from name): the name suggests a read-only status accessor for a read-response buffer of the record in a0. It loads the halfword at +0x72, a doubleword at +0x8 and a word at +0x4 of a0, and loads through pointers at 0xc000000000089208 (word), 0xc000000000089230 and 0xc00000000008924c (doublewords). The facts show no store on any path. The return at 0xc0000000000891a0 yields 0 or an unresolved value, and the panic at 0xc000000000089298 is the failure exit.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_rrb_clear_set` in `6.5.22/unix.IP27` at 0xc0000000000892a8
  **Inferred.** Role (from name): the name suggests it clears and then sets a field of the record in a0; the facts show a single store on each path, so they do not show a clear followed by a set. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, and, depending on the word at +0x4, stores a word at 0xc000000000089370 or doublewords at 0xc00000000008938c or 0xc0000000000893a8; the stored values are not in the facts. The panic at 0xc0000000000893f4 passes a1 through, and the return at 0xc000000000089304 yields an unresolved value or the result of panic.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_pcix_bus_err_addr_get` in `6.5.22/unix.IP27` at 0xc000000000089404
  **Inferred.** Role (from name): the name suggests a getter for the PCI-X bus error address of the record in a0; the facts show no store on any path. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads doublewords at 0xc0000000000894c0 and 0xc0000000000894dc. Two panic calls at 0xc000000000089548 and 0xc000000000089554 are the failure exits, and the return at 0xc000000000089464 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_pcix_bus_err_attr_get` in `6.5.22/unix.IP27` at 0xc000000000089564
  **Inferred.** Role (from name): the name suggests a getter for the PCI-X bus error attribute word of the record in a0; the facts show no store on any path. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads doublewords at 0xc000000000089624 and 0xc000000000089640. Two panic calls at 0xc0000000000896ac and 0xc0000000000896b8 are the failure exits, and the return at 0xc0000000000895c8 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_pcix_bus_err_data_get` in `6.5.22/unix.IP27` at 0xc0000000000896c8
  **Inferred.** Role (from name): the name suggests a getter for the PCI-X bus error data field of the record in a0; the facts show no store on any path. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads doublewords at 0xc000000000089784 and 0xc0000000000897a0. Two panic calls at 0xc000000000089808 and 0xc000000000089814 are the failure exits, and the return at 0xc000000000089728 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_pcix_pio_split_addr_get` in `6.5.22/unix.IP27` at 0xc000000000089824
  **Inferred.** Role (from name): the name suggests a getter for the address of a PCI-X split PIO transaction in the record in a0; the facts show no store on any path. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads doublewords at 0xc0000000000898e0 and 0xc0000000000898fc. Two panic calls at 0xc000000000089968 and 0xc000000000089974 are the failure exits, and the return at 0xc000000000089884 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_pcix_pio_split_attr_get` in `6.5.22/unix.IP27` at 0xc000000000089984
  **Inferred.** Role (from name): the name suggests a getter for the attribute word of a PCI-X split PIO transaction in the record in a0, paired by name with pcireg_pcix_pio_split_addr_get; the facts show no store on any path. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads doublewords at 0xc000000000089a3c and 0xc000000000089a58. Two panic calls at 0xc000000000089ac8 and 0xc000000000089ad4 are the failure exits, and the return at 0xc0000000000899e0 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_pcix_req_err_attr_get` in `6.5.22/unix.IP27` at 0xc000000000089ae4
  **Inferred.** Role (from name): the name suggests a getter for the attribute word of the PCI-X request error record in a0; the facts show no store on any path. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads doublewords at 0xc000000000089b9c and 0xc000000000089bb8. Two panic calls at 0xc000000000089c20 and 0xc000000000089c2c are the failure exits, and the return at 0xc000000000089b40 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_pcix_req_err_addr_get` in `6.5.22/unix.IP27` at 0xc000000000089c3c
  **Inferred.** Role (from name): the name suggests a getter for the address word of the PCI-X request error record in a0, paired by name with pcireg_pcix_req_err_attr_get; the facts show no store on any path. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads doublewords at 0xc000000000089cf8 and 0xc000000000089d14. Two panic calls at 0xc000000000089d80 and 0xc000000000089d8c are the failure exits, and the return at 0xc000000000089c9c gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_type0_cfg_addr` in `6.5.22/unix.IP27` at 0xc000000000089d9c
  **Inferred.** Role (from name): the name suggests it computes a type-0 configuration address for the record in a0; the facts do not show the computation. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0. Its checks call assfail at 0xc00000000008a004, 0xc00000000008a020 and 0xc00000000008a03c, passing the constants 0xb75, 0xb68 and 0xb70 in a2; the panic at 0xc000000000089fe8 is the other exit. The normal return at 0xc000000000089e20 yields 0 or the value of a table entry labelled 0x20000.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_type1_cfg_addr` in `6.5.22/unix.IP27` at 0xc00000000008a04c
  **Inferred.** Role (from name): the name suggests it computes a type-1 configuration address for the record in a0, the companion of pcireg_type0_cfg_addr; the facts do not show the computation. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, and the failure exit calls panic at 0xc00000000008a210 passing a1 and a2 through. The normal return at 0xc00000000008a0a8 yields 0 or the table entry labelled 0x28000; that this differs from the type-0 label does not show what either window is.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_ext_ate_get` in `6.5.22/unix.IP27` at 0xc00000000008a220
  **Inferred.** Role (from name): the name suggests it reads an external address translation entry (ATE) for the record in a0; the facts do not show what the entry is. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, loads a doubleword at 0xc00000000008a324 through a computed pointer, and its failure exits are panic at 0xc00000000008a3a0 and 0xc00000000008a3b4 and assfail at 0xc00000000008a3cc. The return at 0xc00000000008a290 yields 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_ext_ate_set` in `6.5.22/unix.IP27` at 0xc00000000008a3ec
  **Inferred.** Role (from name): the name suggests it writes an external address translation entry for the record in a0, the set side of pcireg_ext_ate_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and stores a doubleword at 0xc00000000008a4f0 through a pointer the facts do not resolve; the stored value is not in the facts. Failure exits are panic at 0xc00000000008a56c and 0xc00000000008a580 and assfail at 0xc00000000008a598; a4 there holds 0xc000000001413578, a leftover value rather than a documented argument. The return at 0xc00000000008a454 yields 1, an unresolved value or the result of panic.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_ext_ate_addr` in `6.5.22/unix.IP27` at 0xc00000000008a5b8
  **Inferred.** Role (from name): the name suggests it returns the address of an external address translation entry for the record in a0; the facts do not show what the returned value is. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and has no store. Failure exits are panic at 0xc00000000008a738 and 0xc00000000008a74c and assfail at 0xc00000000008a764, and the return at 0xc00000000008a62c gives 0 or the table entry labelled 0x80000.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_int_ate_get` in `6.5.22/unix.IP27` at 0xc00000000008a784
  **Inferred.** Role (from name): the name suggests it reads an internal address translation entry (ATE) for the record in a0, the internal counterpart of pcireg_ext_ate_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then loads four doublewords through computed pointers at 0xc00000000008a874, 0xc00000000008a8b0, 0xc00000000008a8f0 and 0xc00000000008a934. Failure exits are panic at 0xc00000000008a970 and assfail calls at 0xc00000000008a98c, 0xc00000000008a9a8, 0xc00000000008a9c4 and 0xc00000000008a9e0; the return at 0xc00000000008a7f0 gives 0 or an unresolved value.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_int_ate_set` in `6.5.22/unix.IP27` at 0xc00000000008a9f0
  **Inferred.** Role (from name): the name suggests it writes an internal address translation entry for the record in a0, the set side of pcireg_int_ate_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and stores doublewords at 0xc00000000008aae4, 0xc00000000008ab24, 0xc00000000008ab64 and 0xc00000000008aba0 through pointers the facts do not resolve; the stored values are not in the facts. Failure exits are panic at 0xc00000000008abdc and assfail calls at 0xc00000000008abf8, 0xc00000000008ac14, 0xc00000000008ac30 and 0xc00000000008ac4c; the return at 0xc00000000008aa64 gives 3, an unresolved value or the result of panic.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_int_ate_addr` in `6.5.22/unix.IP27` at 0xc00000000008ac5c
  **Inferred.** Role (from name): the name suggests it returns the address of an internal address translation entry for the record in a0, the address side of pcireg_int_ate_set. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and has no store. Failure exits are panic at 0xc00000000008ae30 and assfail calls at 0xc00000000008ae4c, 0xc00000000008ae68, 0xc00000000008ae84 and 0xc00000000008aea0; the return at 0xc00000000008acc8 gives 0 or the table entry labelled 0x10000.
  Reviewed: 2026-11-12 (sonnet)

- `io6_init` in `6.5.22/unix.IP27` at 0xc00000000008b8a8
  **Inferred.** It calls nic_vmc_add [0xc00000000006bfb0] three times, at 0xc00000000008b8b4, 0xc00000000008b8c4 and 0xc00000000008b8d4. The first arguments are the fixed addresses 0xc0000000003c6d58, 0xc0000000003c6d68 and 0xc0000000003c6d78, and the second argument is the address of io6_vmc (0xc00000000008b8e8) each time. The return at 0xc00000000008b8e0 passes through the result of the last call. Role (from name): the name suggests these are IO6 vertex records registered with the NIC VMC; what each record holds is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `io6_vmc` in `6.5.22/unix.IP27` at 0xc00000000008b8e8
  **Inferred.** On entry it calls nic_vertex_info_match at 0xc00000000008b8f8 with the fixed address 0xc0000000003c6d58 (the first argument of the first nic_vmc_add call in io6_init) as a1, before anything else. It then reads a byte from the fixed address 0xffffffffffffc523 at 0xc00000000008b900; a nonzero byte branches at 0xc00000000008b904 to 0xc00000000008b9d8, which calls panic (0xc00000000008b9dc) and does not reach the return. When the byte is zero it calls pcibr_hints_dualslot at 0xc00000000008b914 with a1=0x2 and a2=0x4, pcibr_hints_subdevs at 0xc00000000008b930, xtalk_piotrans_addr at 0xc00000000008b948 with a3=0x1000000, pcibr_hints_fix_rrbs at 0xc00000000008b954, hwgraph_traverse at 0xc00000000008b9a4 and device_inventory_add at 0xc00000000008b9c4 with a1=0x15. The return at 0xc00000000008b9d0 passes the result of device_inventory_add. Role (from name): the names of the callees suggest it probes one IO6 vertex and sets PCI-bridge hints; the facts do not confirm that.
  Reviewed: 2026-11-12 (sonnet)
