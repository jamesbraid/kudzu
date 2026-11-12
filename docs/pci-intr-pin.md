# PCI interrupt pins on the IP35 bridges

This page records how the 6.5.30 IP35 kernel turns a PCI slot and interrupt pin into
Bridge or PIC interrupt bits, what the kernel expects of the hardware, and what the
published material says about board wiring. The arithmetic below is the kernel's
software expectation. It does not show how a physical pin is wired on a machine.

The pin mask is the one the caller passes. Routine entries name the binary they were
read from. Each routine has one entry; the other places it is used are described inside it.

## The default mapper

- `pcibr_intr_bits` (`6.5.30/unix.IP35`, address 0xc0000000000a4bf4, 76 bytes, no frame).
  Inputs are the device info record in `a0`, the pin mask in `a1` and the slot count in `a2`.
  The slot `s` is the byte at record +540 (`lbu` at 0xc0000000000a4bf4). `A` is `a1 & 0x5`
  (INTA or INTC requested) and `B` is `a1 & 0xa` (INTB or INTD requested). If `s` is not
  below `a2` (`slt` at 0xc0000000000a4bfc, branch at 0xc0000000000a4c08) the result is 0.
  Otherwise the result is `(A ? 1 << s : 0) | (B ? 1 << (s ^ 4) : 0)`. The XOR is `xori`
  at 0xc0000000000a4c0c, the shifts are `sllv` at 0xc0000000000a4c14 and 0xc0000000000a4c1c,
  and the `B` term is applied by the conditional move `movn` at 0xc0000000000a4c24. So INTA
  and INTC on slot `s` use bridge bit `s`, INTB and INTD use bridge bit `s ^ 4`, and the
  mapper does not tell INTA from INTC or INTB from INTD. A request for both groups gets both
  bits. With the slot limit of 3 that `pic_attach2` sets (below), bits 0 to 3 then carry only
  INTA and INTC and bits 4 to 7 carry only INTB and INTD, so the two groups never share a bit
  on that path; that reading depends on the unreconciled slot limit. Every branch and delay
  slot was read. Established.

## Which mapper is installed, and the slot limit

- `pcibr_intr_alloc` (`6.5.30/unix.IP35`, address 0xc0000000000a4f8c, 1552 bytes) calls the
  mapper through the pointer at soft +1288 (`jalr` at 0xc0000000000a50f4), with its pin mask
  as `a1` and the highest slot at soft +57 plus one as `a2`. For each bit `n` in the result
  it installs the crosstalk interrupt for that bit when none exists yet:
  `compare_and_swap_int64` at 0xc0000000000a51ac guards the install and
  `xtalk_intr_alloc_nothd` at 0xc0000000000a52ac creates it. Only the installing caller
  changes the bridge's interrupt device register. The installer clears the three-bit field
  for bit `n` with `pcireg_intr_device_bit_clr` (`jal` at 0xc0000000000a5344, mask `7 << 3n`
  built in its delay slot) and sets it with `pcireg_intr_device_bit_set` (`jal` at
  0xc0000000000a5358). The value stored is the slot byte, reloaded from the stack copy that
  `lbu` at 0xc0000000000a4fd0 and `sd` at 0xc0000000000a4fd8 took from record +540. So field
  `n` names the slot whose device owns bit `n`. The per-bit user list lets two slots share a
  bit, but the field holds one slot. The sharing consequence is a reading of the installer
  logic; the list-append path was not stepped instruction by instruction. Established.
- `pic_attach2` (`6.5.30/unix.IP35`, address 0xc00000000009b884, 3276 bytes) stores the
  default mapper at soft +1288 (`sd` at 0xc00000000009bac4), from a gp pool word that
  resolves to `pcibr_intr_bits`. It sets the highest slot at soft +57 to 3 and the lowest at
  soft +56 to 0 (`sb` at 0xc00000000009ba98 and 0xc00000000009ba9c), so this path accepts
  slots 0 to 3. It also copies the word at +24 of a hint record into soft +1288
  (`sd` at 0xc00000000009c26c), skipping the copy when that word is zero (`beqz` at
  0xc00000000009c264). The bridge-type table in the attach path (see
  [ip35-pci-bridge.md](ip35-pci-bridge.md)) also writes slot limits, and its order against
  this routine was not read. Established for those stores.
- `xbridge_attach` (`6.5.30/unix.IP35`, address 0xc00000000009c8ac, 3484 bytes) stores the
  default mapper at soft +1288 (`sd` at 0xc00000000009caec, from a gp pool word that resolves
  to `pcibr_intr_bits`) and sets the highest slot at soft +57 to 7 (`sb` at
  0xc00000000009cad8), so it accepts slots 0 to 7. It also copies the hint word into soft
  +1288 (`sd` at 0xc00000000009d338), which runs only when the word is nonzero (branch `beqz`
  at 0xc00000000009d330).
  Established.
- `pcibr_hints_intr_bits` (`6.5.30/unix.IP35`, address 0xc0000000000a49e0, 76 bytes) stores
  its second argument at +24 of the record that `pcibr_hints_get` returns (`sd` at
  0xc0000000000a4a04), and logs through `pcibr_debug` when there is no record. So a driver
  hint can replace the default mapper. Established for the store. The hint record is the
  record the two attach copies above read: that link is inferred, because the call that
  produces the record in the attach routines was not resolved. Inferred.

## The 6.5.22 kernel

- `pcibr_intr_bits` (`6.5.22/unix.IP35`, address 0xc0000000000a6de0, 76 bytes): its 19
  instruction words match the 6.5.30 routine except for branch displacements. Established
  for the instruction bytes.
- `pic_attach2` (`6.5.22/unix.IP35`, address 0xc00000000009db24, 3176 bytes) stores to soft
  +0x508 at 0xc00000000009dd68 (default mapper) and at 0xc00000000009e4a8 (hint copy). The
  pool word behind the first store was not resolved for 6.5.22. Unverified.
- `xbridge_attach` (`6.5.22/unix.IP35`, address 0xc00000000009eaa8, 3484 bytes) stores to
  soft +0x508 at 0xc00000000009ece8 (default mapper) and at 0xc00000000009f534 (hint copy).
  The pool word behind the first store was not resolved for 6.5.22. Unverified.

## Published material

These are observations about the documents, not claims about the binaries, so they carry
no grade.

- The IRIX Device Driver Programmer's Guide, 007-0911-210 (the revision for 6.5.22, printed
  page 753), lets a driver ask for any combination of the four PCI interrupt lines when it
  allocates an interrupt, and its example asks for lines A and B together for one function. The guide
  says nothing about which bridge bit or board pin a line reaches.
- The Tezro Visual Workstation Hardware User's Guide (007-4643-002) and the Origin 300 User's
  Guide (007-4365-002) were read in full text. Neither gives an INTA to INTD routing, a slot
  to interrupt table or a PCI interrupt line. Their only interrupt text concerns the RT
  interrupt ports and the NMI switch. The Tezro guide labels its PCI slots by bus and slot
  (bus 1 and bus 2, slots 1 and 2 on each bus, rear labels PCI 1 to PCI 4).
- The Origin 350 User's Guide (007-4566-001, 274 pages) was searched on every page's text for
  "interrupt", "INTA", "IRQ", "IDSEL", "wiring", "jumper" and "PCI slot". It gives no INTA to
  INTD routing and no slot to interrupt table; its interrupt text is the real-time interrupt
  ports and the NMI switch (printed pages 45, 61, 69, 110, 111, 119, 123, 126 and 130). Its only
  slot facts are physical: a compute module has four PCI/PCI-X slots, the lowermost reserved for
  the IO9 card and the one above it limited to 66 MHz or slower cards; the MPX module has four;
  the PCI expansion module has six buses (1 to 6) of two slots each, labelled 1 and 2 (printed
  page 87). The pages were found by text search, not read in full.

## Captures

No `hinv -v`, `ioconfig` or interrupt statistics capture from an IP35 system with a
dual-function PCI card was found. Unknown.

## Summary

- The mask-bit names are a reading of the PCI pin order, not a check against the debug text
  the caller prints. Unknown whether bit 1 is INTB in the caller's own naming.
- Software expectation of the IP35 kernels: the mapper arithmetic in the routine entries
  above. INTA and INTC use the slot bit, and INTB and INTD use the slot bit XOR 4.
- Board wiring, meaning which physical pin reaches which bridge bit: unknown. No document or
  capture read here states it, and the Origin 350, Origin 300 and Tezro user's guides were all
  searched without finding it.
- The XOR-4 rule for pins B and D is the rule the question attributes to the OpenBSD and
  IA64 drivers. Those drivers were not read here. The Linux Origin 350 port's `d - 1` for
  every pin does not match the IRIX kernel for pins B and D.
- Device numbering: the 64-bit config-address layout adds one to the slot
  (`pcireg_type0_cfg_addr`, see [ip35-pcibr-registers.md](ip35-pcibr-registers.md)). If the
  device number is the slot plus one, then `d - 1` in the other drivers is the slot used
  here. Unknown until a device-number table is read.
