# Tier-2 routine readings, kudzu round 4, batch 7

Role statements for targets 175 to 199 of the ranked tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Names come from the binary's symbol table and are not checked
against any other source. Offsets such as +0x72 are byte offsets from the pointer
named in the sentence.

- `pcireg_device_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000088574
  **Inferred.** Clears bits in a bridge register for the record passed in a0, with a1 as the second argument. It loads the halfword at +0x72 of a0 and branches on the values 0x3, 0x2, 0, 1 and -1; the -1 case reaches panic at 0xc00000000008874c. The halfword is then compared with 0xe000, 0xe010, 0xd102, 0xd112 and 0xd002 to pick one register path, and that path does a word load at 0xc000000000088670 and store at 0xc000000000088680 through a pointer from the record. The operation applied to the loaded word is an unknown sub-fact.

- `pcireg_wrb_flush_get` in `6.5.22/unix.IP27` at 0xc0000000000887b0
  **Inferred.** A read-only accessor for the write-buffer flush state of the record in a0. It loads the halfword at +0x72 of a0, a doubleword at +0x8 and a word at +0x4, then reads through computed pointers at 0xc000000000088894, 0xc000000000088900 and 0xc00000000008893c. Invalid selector paths call assfail at 0xc0000000000889a8, 0xc0000000000889bc, 0xc0000000000889d8 and 0xc0000000000889f4 and panic at 0xc00000000008898c; the normal return at 0xc0000000000888c0 yields the value in v0, which is an unknown sub-fact.

- `pcireg_rrb_get` in `6.5.22/unix.IP27` at 0xc000000000088a04
  **Inferred.** Reads a read-response-buffer setting from the record in a0. It loads a doubleword at +0x8 and the halfword at +0x72 of a0, a word at +0x4, and then reads through pointers at 0xc000000000088ab4, 0xc000000000088b30 and 0xc000000000088b40. The bad-argument path calls assfail at 0xc000000000088bf0 with -0x1 and 0x3 in a4 and a5 and panics at 0xc000000000088c04; the normal return at 0xc000000000088a94 gives 0 or an unknown value.

- `pcireg_rrb_set` in `6.5.22/unix.IP27` at 0xc000000000088c14
  **Inferred.** The write side of pcireg_rrb_get: it sets a read-response-buffer setting for the record in a0. It loads the halfword at +0x72 and doubleword at +0x8 of a0, then stores a word at 0xc000000000088ccc and doublewords at 0xc000000000088d38 and 0xc000000000088d44 through pointers from the record. Its failure path calls assfail at 0xc000000000088df8 and panic at 0xc000000000088e0c, and the return at 0xc000000000088ca8 yields an unknown value.

- `pcireg_rrb_bit_set` in `6.5.22/unix.IP27` at 0xc000000000088e1c
  **Inferred.** Sets bits in a read-response-buffer register for the record in a0. It loads the halfword at +0x72 and word at +0x4 of a0, reads a word at 0xc000000000088eec, stores it at 0xc000000000088ef8, and writes doublewords at 0xc000000000088f24 and 0xc000000000088f50 through computed pointers. The panic at 0xc000000000088f9c passes a1 and a2 through with -0x1 or 0xe010 in a6, and the return at 0xc000000000088e7c yields an unknown value. The combining operation on the loaded word is an unknown sub-fact.

- `pcireg_rrb_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000088fac
  **Inferred.** The clear counterpart of pcireg_rrb_bit_set, with the same record layout: a halfword at +0x72, a doubleword at +0x8 and a word at +0x4 of a0. It reads a word at 0xc000000000089078, stores it at 0xc000000000089088, and writes doublewords at 0xc0000000000890b8 and 0xc0000000000890e8. The panic at 0xc000000000089134 passes a1 and a2 through with -0x1 or 0xe010 in a6, and the return at 0xc000000000089008 yields 0x3 or an unknown value.

- `pcireg_rrb_status_get` in `6.5.22/unix.IP27` at 0xc000000000089144
  **Inferred.** A read-only status accessor for a read-response buffer of the record in a0. It loads the halfword at +0x72, a doubleword at +0x8 and a word at +0x4 of a0, and reads through pointers at 0xc000000000089208, 0xc000000000089230 and 0xc00000000008924c. It has no store on any path; the return at 0xc0000000000891a0 yields 0 or an unknown value, and the panic at 0xc000000000089298 is the failure exit.

- `pcireg_rrb_clear_set` in `6.5.22/unix.IP27` at 0xc0000000000892a8
  **Inferred.** Clears and then sets a read-response-buffer field in the record in a0, as the name suggests. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, and stores a word at 0xc000000000089370 and doublewords at 0xc00000000008938c and 0xc0000000000893a8. The panic at 0xc0000000000893f4 passes a1 through, and the return at 0xc000000000089304 yields an unknown value.

- `pcireg_pcix_bus_err_addr_get` in `6.5.22/unix.IP27` at 0xc000000000089404
  **Inferred.** A read-only getter for the PCI-X bus error address held in the record in a0. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads doublewords at 0xc0000000000894c0 and 0xc0000000000894dc. Two panic calls at 0xc000000000089548 and 0xc000000000089554 are the failure exits, one with -0x1 or 0xd002 in a4, and the return at 0xc000000000089464 gives 0 or an unknown value.

- `pcireg_pcix_bus_err_attr_get` in `6.5.22/unix.IP27` at 0xc000000000089564
  **Inferred.** A read-only getter for the PCI-X bus error attribute word of the record in a0. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads doublewords at 0xc000000000089624 and 0xc000000000089640. Failure exits panic at 0xc0000000000896ac (with -0x1 or 0xd002 in a4) and 0xc0000000000896b8, and the return at 0xc0000000000895c8 gives 0 or an unknown value.

- `pcireg_pcix_bus_err_data_get` in `6.5.22/unix.IP27` at 0xc0000000000896c8
  **Inferred.** A read-only getter for the PCI-X bus error data field of the record in a0. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads doublewords at 0xc000000000089784 and 0xc0000000000897a0. Failure exits panic at 0xc000000000089808 (with -0x1 or 0xd002 in a4) and 0xc000000000089814, and the return at 0xc000000000089728 gives 0 or an unknown value.

- `pcireg_pcix_pio_split_addr_get` in `6.5.22/unix.IP27` at 0xc000000000089824
  **Inferred.** A read-only getter for the address of a PCI-X split PIO transaction in the record in a0. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads doublewords at 0xc0000000000898e0 and 0xc0000000000898fc. Failure exits panic at 0xc000000000089968 (with -0x1 or 0xd002 in a4) and 0xc000000000089974, and the return at 0xc000000000089884 gives 0 or an unknown value.

- `pcireg_pcix_pio_split_attr_get` in `6.5.22/unix.IP27` at 0xc000000000089984
  **Inferred.** A read-only getter for the attribute word of a PCI-X split PIO transaction in the record in a0, paired with pcireg_pcix_pio_split_addr_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads doublewords at 0xc000000000089a3c and 0xc000000000089a58. Failure exits panic at 0xc000000000089ac8 (with -0x1 or 0xd002 in a4) and 0xc000000000089ad4, and the return at 0xc0000000000899e0 gives 0 or an unknown value.

- `pcireg_pcix_req_err_attr_get` in `6.5.22/unix.IP27` at 0xc000000000089ae4
  **Inferred.** A read-only getter for the attribute word of the PCI-X request error record in a0. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads doublewords at 0xc000000000089b9c and 0xc000000000089bb8. Failure exits panic at 0xc000000000089c20 (with -0x1 or 0xd002 in a4) and 0xc000000000089c2c, and the return at 0xc000000000089b40 gives 0 or an unknown value.

- `pcireg_pcix_req_err_addr_get` in `6.5.22/unix.IP27` at 0xc000000000089c3c
  **Inferred.** A read-only getter for the address word of the PCI-X request error record in a0, paired with pcireg_pcix_req_err_attr_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads doublewords at 0xc000000000089cf8 and 0xc000000000089d14. Failure exits panic at 0xc000000000089d80 (with -0x1 or 0xd002 in a4) and 0xc000000000089d8c, and the return at 0xc000000000089c9c gives 0 or an unknown value.

- `pcireg_type0_cfg_addr` in `6.5.22/unix.IP27` at 0xc000000000089d9c
  **Inferred.** Computes a type-0 configuration address for the record in a0. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, and its checks call assfail at 0xc00000000008a004, 0xc00000000008a020 and 0xc00000000008a03c, passing the constants 0xb75, 0xb68 and 0xb70 in a2; the panic at 0xc000000000089fe8 is the other exit. The normal return at 0xc000000000089e20 yields 0 or the value of a table entry labelled 0x20000.

- `pcireg_type1_cfg_addr` in `6.5.22/unix.IP27` at 0xc00000000008a04c
  **Inferred.** Computes a type-1 configuration address for the record in a0, the companion of pcireg_type0_cfg_addr. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, and the failure exit calls panic at 0xc00000000008a210 passing a1 and a2 through. The normal return at 0xc00000000008a0a8 yields 0 or the table entry labelled 0x28000, which differs from the type-0 table label and so suggests a different window.

- `pcireg_ext_ate_get` in `6.5.22/unix.IP27` at 0xc00000000008a220
  **Inferred.** Reads an external address translation entry (ATE) for the record in a0; the name gives the ATE reading. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, reads a doubleword at 0xc00000000008a324 through a computed pointer, and its failure exits are panic at 0xc00000000008a3a0 and 0xc00000000008a3b4 and assfail at 0xc00000000008a3cc. The return at 0xc00000000008a290 yields 0 or an unknown value.

- `pcireg_ext_ate_set` in `6.5.22/unix.IP27` at 0xc00000000008a3ec
  **Inferred.** Writes an external address translation entry for the record in a0, the set side of pcireg_ext_ate_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and stores a doubleword at 0xc00000000008a4f0 through a pointer from the record. Failure exits panic at 0xc00000000008a56c and 0xc00000000008a580 and call assfail at 0xc00000000008a598 with 0xc000000001413578 in a4; the return at 0xc00000000008a454 yields 1 or an unknown value.

- `pcireg_ext_ate_addr` in `6.5.22/unix.IP27` at 0xc00000000008a5b8
  **Inferred.** Returns the address of an external address translation entry for the record in a0. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and has no store. Failure exits are panic at 0xc00000000008a738 and 0xc00000000008a74c and assfail at 0xc00000000008a764, and the return at 0xc00000000008a62c gives 0 or the table entry labelled 0x80000.

- `pcireg_int_ate_get` in `6.5.22/unix.IP27` at 0xc00000000008a784
  **Inferred.** Reads an internal address translation entry (ATE) for the record in a0, the internal counterpart of pcireg_ext_ate_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0, then reads four doublewords through computed pointers at 0xc00000000008a874, 0xc00000000008a8b0 and 0xc00000000008a8f0 among them. Failure exits are panic at 0xc00000000008a970 and assfail calls at 0xc00000000008a98c, 0xc00000000008a9a8, 0xc00000000008a9c4 and 0xc00000000008a9e0; the return at 0xc00000000008a7f0 gives 0 or an unknown value.

- `pcireg_int_ate_set` in `6.5.22/unix.IP27` at 0xc00000000008a9f0
  **Inferred.** Writes an internal address translation entry for the record in a0, the set side of pcireg_int_ate_get. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and stores four doublewords at 0xc00000000008aae4, 0xc00000000008ab24 and 0xc00000000008ab64 among them through pointers from the record. Failure exits are panic at 0xc00000000008abdc and assfail calls at 0xc00000000008abf8, 0xc00000000008ac14, 0xc00000000008ac30 and 0xc00000000008ac4c; the return at 0xc00000000008aa64 gives 3 or an unknown value.

- `pcireg_int_ate_addr` in `6.5.22/unix.IP27` at 0xc00000000008ac5c
  **Inferred.** Returns the address of an internal address translation entry for the record in a0, the address side of pcireg_int_ate_set. It loads the halfword at +0x72, doubleword at +0x8 and word at +0x4 of a0 and has no store. Failure exits are panic at 0xc00000000008ae30 and assfail calls at 0xc00000000008ae4c, 0xc00000000008ae68, 0xc00000000008ae84 and 0xc00000000008aea0; the return at 0xc00000000008acc8 gives 0 or the table entry labelled 0x10000.

- `io6_init` in `6.5.22/unix.IP27` at 0xc00000000008b8a8
  **Inferred.** Registers three IO6 vertex records with the NIC VMC by calling nic_vmc_add [0xc00000000006bfb0] at 0xc00000000008b8b4, 0xc00000000008b8c4 and 0xc00000000008b8d4. The three first arguments are the fixed addresses 0xc0000000003c6d58, 0xc0000000003c6d68 and 0xc0000000003c6d78, and the second argument is the address of io6_vmc (0xc00000000008b8e8) each time. The return at 0xc00000000008b8e0 passes through the result of the last call, and what each record holds is an unknown sub-fact.

- `io6_vmc` in `6.5.22/unix.IP27` at 0xc00000000008b8e8
  **Inferred.** Probes one IO6 vertex and sets up its PCI-bridge hints, using the record at 0xc0000000003c6d58 that io6_init registers. It first reads a byte from the fixed address 0xffffffffffffc523 at 0xc00000000008b900, and a nonzero value branches to 0xc00000000008b9d8 at 0xc00000000008b904. Otherwise it calls nic_vertex_info_match at 0xc00000000008b8f8, pcibr_hints_dualslot at 0xc00000000008b914 with a1=0x2 and a2=0x4, pcibr_hints_subdevs at 0xc00000000008b930, xtalk_piotrans_addr at 0xc00000000008b948 with a3=0x1000000, and pcibr_hints_fix_rrbs at 0xc00000000008b954, then hwgraph_traverse at 0xc00000000008b9a4 and device_inventory_add at 0xc00000000008b9c4 with a1=0x15. The return at 0xc00000000008b9d0 passes the result of device_inventory_add.
