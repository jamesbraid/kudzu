# Tier-2 routine readings, kudzu round 4 batch 5

- `pcireg_dirmap_wid_set` in `6.5.22/unix.IP27` at 0xc0000000000831ac
  **Inferred.** The name says it sets the widget field of a PCI direct-mapping register. The facts show a null check on a0 that branches to the panic call at 0xc000000000083338, a halfword read at +0x72 that drives a dispatch chain (0xc0000000000831c8 to 0xc0000000000831fc), and a second compare chain on 0xe000, 0xe010, 0xd102, 0xd112 and 0xd002 (0xc000000000083234 to 0xc000000000083254). The stores at 0xc00000000008327c to 0xc0000000000832d4 go to addresses the facts do not name, so which field each one writes is not derived here.

- `pcireg_dirmap_diroff_set` in `6.5.22/unix.IP27` at 0xc000000000083348
  **Inferred.** The name suggests it sets the offset field of a PCI direct mapping, the sibling of the widget setter at 0xc0000000000831ac. The routine has the same null check and the same halfword dispatch at +0x72, with a six-way doubleword store group (0xc00000000008345c to 0xc000000000083484) and a word store at 0xc00000000008341c and 0xc000000000083434. Its error path calls the panic routine at 0xc000000000083528, and it returns an unlabeled value or the panic result at 0xc0000000000833a4.

- `pcireg_dirmap_add512_set` in `6.5.22/unix.IP27` at 0xc000000000083538
  **Inferred.** The name suggests it turns on the 512-unit add bit of a PCI direct mapping. It null-checks a0, reads the halfword at +0x72 and dispatches on it, then performs one word store at 0xc00000000008360c and doubleword stores at 0xc000000000083634 and 0xc00000000008365c. The panic call at 0xc0000000000836a8 is the only error exit. The stores are to unlabeled addresses, so the exact bit written is not derived from these facts.

- `pcireg_dirmap_add512_clr` in `6.5.22/unix.IP27` at 0xc0000000000836b8
  **Inferred.** The name suggests it clears the same 512-unit add bit that the setter at 0xc000000000083538 sets. It reads the halfword at +0x72 and the word at +0x4 of a0, then does a word load at 0xc000000000083778 and a word store at 0xc00000000008378c, which looks like a read-modify-write of one 32-bit register. It returns 0x3 or an unlabeled value at 0xc000000000083710, and its panic call is at 0xc000000000083830.

- `pcireg_ssram_parity_get` in `6.5.22/unix.IP27` at 0xc000000000083840
  **Inferred.** The name says it reads the SSRAM parity state. The routine checks its argument, reads the halfword at +0x72, the word at +0x4 and the word at 0xc000000000083908, and returns 0x0 or an unlabeled value at 0xc0000000000838a0. It has two panic call sites, at 0xc000000000083990 and 0xc0000000000839a4, and the second passes the constant 0xd102 or 0x24 to the panic path. It writes nothing.

- `pcireg_map_fault_get` in `6.5.22/unix.IP27` at 0xc0000000000839bc
  **Inferred.** The name suggests it reads the map-fault status. It is a read-only accessor: it loads the halfword at +0x72, the word at +0x4 and two doublewords from unlabeled addresses (0xc000000000083aa8, 0xc000000000083ac4), then returns 0x0 or the loaded value at 0xc000000000083a1c. Two panic call sites at 0xc000000000083b14 and 0xc000000000083b20 handle the invalid cases, and the first passes a5 as either 0xc002 or 0xd102.

- `pcireg_arbitration_get` in `6.5.22/unix.IP27` at 0xc000000000083b30
  **Inferred.** The name suggests it reads the bridge arbitration register. It null-checks a0, reads the halfword at +0x72, the word at +0x4 and two doublewords at 0xc000000000083c20 and 0xc000000000083c3c, and returns 0x0 or a loaded value at 0xc000000000083b90. It has a single panic call at 0xc000000000083c88 and writes nothing.

- `pcireg_arbitration_set` in `6.5.22/unix.IP27` at 0xc000000000083c98
  **Inferred.** The name suggests it writes the arbitration register. It reads the same argument fields as the getter at 0xc000000000083b30 and then makes one word store at 0xc000000000083d60 and two doubleword stores at 0xc000000000083d7c and 0xc000000000083d98. The caller's value enters as arg1, and the only error exit is the panic call at 0xc000000000083de4. It returns through 0xc000000000083cf4.

- `pcireg_arbitration_bit_set` in `6.5.22/unix.IP27` at 0xc000000000083df4
  **Inferred.** The name suggests it sets one bit in the arbitration register. It loads a word from an unlabeled address at 0xc000000000083eb4 and stores a word back at 0xc000000000083ec0, so it reads, modifies and writes one 32-bit register, with the doubleword accesses at 0xc000000000083ed8 to 0xc000000000083f08 handling the other fields. The bit itself comes from arg1, which the panic call at 0xc000000000083f54 checks. It returns 0x3 or an unlabeled value at 0xc000000000083e4c.

- `pcireg_arbitration_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000083f64
  **Inferred.** The name suggests it clears one bit in the arbitration register, the mirror of the setter at 0xc000000000083df4. The shape matches: a word load at 0xc000000000084024, a word store at 0xc000000000084034 for the read-modify-write, two doubleword loads and two stores in the same group, and a panic call at 0xc0000000000840d0 with arg1 as the bit selector. It returns 0x3 or an unlabeled value at 0xc000000000083fbc.

- `pcireg_nic_addr` in `6.5.22/unix.IP27` at 0xc0000000000840e0
  **Inferred.** The name suggests it returns the NIC address held in the bridge. It is a read-only accessor: it reads the halfword at +0x72, the doubleword at +0x8 and the word at +0x4 of a0 and returns 0x0 or an unlabeled value at 0xc00000000008413c. Its only error exit is the panic call at 0xc000000000084228. The NIC address field itself is not located in these facts.

- `pcireg_parity_err_get` in `6.5.22/unix.IP27` at 0xc000000000084238
  **Inferred.** The name suggests it reads the parity error state. It reads the halfword at +0x72, the word at +0x4 and two doublewords from unlabeled addresses (0xc0000000000842f0 and 0xc00000000008430c), and returns 0x0 or an unlabeled value at 0xc000000000084294. Two panic call sites at 0xc000000000084378 and 0xc000000000084384 cover the error cases, and the first passes a4 as either -0x1 or 0xd002. It writes nothing.

- `pcireg_timeout_get` in `6.5.22/unix.IP27` at 0xc000000000084394
  **Inferred.** The name suggests it reads the bridge timeout register. It is a plain read path: the halfword at +0x72, the word at +0x4 and two doublewords at 0xc000000000084480 and 0xc00000000008449c feed a return of 0x0 or an unlabeled value at 0xc0000000000843f0. The panic call at 0xc0000000000844e8 is the error exit.

- `pcireg_timeout_set` in `6.5.22/unix.IP27` at 0xc0000000000844f8
  **Inferred.** The name suggests it writes the bridge timeout register, mirroring the getter at 0xc000000000084394. It makes one word store at 0xc0000000000845bc and two doubleword stores at 0xc0000000000845d8 and 0xc0000000000845f4, with arg1 as the value, and returns 0x3 or an unlabeled value at 0xc000000000084550. The only error exit is the panic call at 0xc000000000084640.

- `pcireg_timeout_bit_set` in `6.5.22/unix.IP27` at 0xc000000000084650
  **Inferred.** The name suggests it sets one bit in the timeout register. It performs a word load at 0xc000000000084714 and a word store at 0xc000000000084720, the read-modify-write of a 32-bit register, plus two doubleword loads and two doubleword stores at 0xc000000000084738 to 0xc000000000084768. The panic call at 0xc0000000000847b4 takes arg1 as the bit, and the routine returns through 0xc0000000000846ac.

- `pcireg_timeout_bit_clr` in `6.5.22/unix.IP27` at 0xc0000000000847c4
  **Inferred.** The name suggests it clears one bit in the timeout register, the counterpart of the setter at 0xc000000000084650. It has the same read-modify-write shape, a word load at 0xc000000000084884 and a word store at 0xc000000000084894, with doubleword accesses at 0xc0000000000848ac to 0xc0000000000848e4. Its panic call is at 0xc000000000084930, and it returns 0x3 or an unlabeled value at 0xc00000000008481c.

- `pcireg_type1_cntr_get` in `6.5.22/unix.IP27` at 0xc000000000084940
  **Inferred.** The name suggests it reads a type-1 configuration counter. It is read-only: the halfword at +0x72, the word at +0x4, a word load at 0xc000000000084a08 and two doubleword loads at 0xc000000000084a30 and 0xc000000000084a4c feed a return of 0x0 or a loaded value at 0xc0000000000849a0. The panic call at 0xc000000000084a98 is the only error exit.

- `pcireg_type1_cntr_set` in `6.5.22/unix.IP27` at 0xc000000000084aa8
  **Inferred.** The name suggests it writes the same type-1 counter that the getter at 0xc000000000084940 reads. It makes one word store at 0xc000000000084b6c and two doubleword stores at 0xc000000000084b88 and 0xc000000000084ba4, with arg1 as the value, and returns 0x3 or an unlabeled value at 0xc000000000084b00. The panic call at 0xc000000000084bf0 is the error exit.

- `pcireg_pci_bus_addr_get` in `6.5.22/unix.IP27` at 0xc000000000084c00
  **Inferred.** The name suggests it reads the PCI bus address held by the bridge. It loads the halfword at +0x72, the doubleword at +0x8 and the word at +0x4 of a0, then two word loads at 0xc000000000084cc4 and 0xc000000000084cc8 and two doubleword loads at 0xc000000000084cf8 and 0xc000000000084d14. It returns 0x0 or an unlabeled value at 0xc000000000084c5c, and the panic call at 0xc000000000084d60 is the error exit.

- `pcireg_pci_bus_addr_addr_get` in `6.5.22/unix.IP27` at 0xc000000000084d70
  **Inferred.** The name suggests it reads an address-register companion of the PCI bus address getter at 0xc000000000084c00. It loads the same argument fields and then two word loads at 0xc000000000084e38 and 0xc000000000084e3c plus a doubleword load at 0xc000000000084e6c, and returns 0x0 or an unlabeled value at 0xc000000000084dd0. The panic call at 0xc000000000084ee0 is the error exit.

- `pcireg_intr_status_get` in `6.5.22/unix.IP27` at 0xc000000000084ef0
  **Inferred.** The name suggests it reads the bridge interrupt status register. It loads the halfword at +0x72, the doubleword at +0x8 and the word at +0x4 of a0, plus a word load at 0xc000000000084fb8 and two doubleword loads at 0xc000000000084fe0 and 0xc000000000084ffc, and returns 0x0 or an unlabeled value at 0xc000000000084f50. The panic call at 0xc000000000085048 is the error exit. It writes nothing.

- `pcireg_intr_enable_get` in `6.5.22/unix.IP27` at 0xc000000000085058
  **Inferred.** The name suggests it reads the bridge interrupt-enable register, the reader for the setter at 0xc0000000000851bc. It loads the same argument fields, a word load at 0xc00000000008511c and two doubleword loads at 0xc000000000085144 and 0xc000000000085160, and returns 0x0 or an unlabeled value at 0xc0000000000850b4. The only error exit is the panic call at 0xc0000000000851ac.

- `pcireg_intr_enable_set` in `6.5.22/unix.IP27` at 0xc0000000000851bc
  **Inferred.** The name suggests it writes the interrupt-enable register read by the getter at 0xc000000000085058. It makes one word store at 0xc000000000085280 and two doubleword stores at 0xc00000000008529c and 0xc0000000000852b8, with arg1 as the value, and returns 0x3 or an unlabeled value at 0xc000000000085214. The panic call at 0xc000000000085304 is the error exit.

- `pcireg_intr_enable_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000085314
  **Inferred.** The name suggests it clears one bit in the interrupt-enable register written by the setter at 0xc0000000000851bc. The read-modify-write is a word load at 0xc0000000000853d8 and a word store at 0xc0000000000853e8, with doubleword loads and stores at 0xc000000000085400 to 0xc000000000085438. The panic call at 0xc000000000085484 takes arg1 as the bit, and it returns through 0xc000000000085370.

- `pcireg_intr_enable_bit_set` in `6.5.22/unix.IP27` at 0xc000000000085494
  **Inferred.** The name suggests it sets one bit in the interrupt-enable register, the counterpart of the clearer at 0xc000000000085314. It does a word load at 0xc000000000085558 and a word store at 0xc000000000085564 for the read-modify-write, with doubleword accesses at 0xc00000000008557c to 0xc0000000000855ac. The panic call at 0xc0000000000855f8 takes arg1 as the bit, and it returns through 0xc0000000000854f0.
