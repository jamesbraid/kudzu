# Crash dump of the IP35 kernel: the Hub information pass

Routines of the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF, 13,104,040 bytes, sha256
`412a1aae553a348376bc30cacbaf65044de1745d30222f02ec5f88297dcb1dfa`) that run after the kernel panics and writes a core
to the dump device. The register-level reading of the Hub offsets this pass touches is in chips/bedrock/README.md ("Register
walk of the crash dump"); this page is the software side. A second linked 6.5.30 IP35 kernel (14,238,624 bytes, sha256
`8404e226a649c622ebefe452a234566d74cbb81e802d793f22acdcbab518b9a8`) has the same table and routines, with
`dump_hub_registers` at 0xc0000000002e70dc.

- `dump_hub_info` (`6.5.30/unix.IP35`, data, address 0xc00000000163c64c, 4 bytes, initial value 2): a tunable word. The next
  word, `dump_level` (0xc00000000163c650), starts at 4. The dump driver (the routine that contains the call at
  0xc0000000002c7d6c) loads `dump_hub_info`; when it is zero the driver skips the pass, otherwise it prints "Dumping hub
  information ..." and calls `dump_hub_data`. Established.
- `dump_hub_data` (`6.5.30/unix.IP35`, address 0xc0000000002c90e4, 96 bytes): reads `dump_hub_info`. When it is above 0 it calls
  `dump_hub_registers`; a nonzero result there ends the pass and is returned. When the value is 2 or more it then calls
  `dump_hub_dir_mem` and returns its result. So 0 skips both, 1 dumps registers only, 2 (the shipped value) dumps registers
  and directory memory. Established.
- `dump_hub_registers` (`6.5.30/unix.IP35`, address 0xc0000000002c956c, 944 bytes, 0x44a0-byte frame): see below.
- `dump_hub_dir_mem` (`6.5.30/unix.IP35`, address 0xc0000000002c9144, 520 bytes), `write_hub_dir_mem` (0xc0000000002c934c, 404
  bytes) and `flush_hub_dir_mem` (0xc0000000002c94e0, 140 bytes): for each node, walk the node's memory pages that
  `page_dumpable` accepts. `write_hub_dir_mem` skips discarded, poisoned and cache-alias pages and pages `page_read_accessible`
  refuses. For each page it reads the doubleword at 0x9200000001fc0000 (pool word gp+0x948) plus the node number shifted
  left 33, and keeps bit 0 as a format flag; then it copies 128 pairs of doublewords, two consecutive doublewords per 128 bytes
  of the page, from the address 0x90000001c0000000 (pool word gp+0x950) ORed with an index built from the page address, into
  a 16 KB buffer (0xc0000000016b8c60) with a fill of 0x5555555555555555 elsewhere. A full buffer is handed to
  `compress_and_write_block` with a length of 0x4000 and a word that is 0x1100 when the format flag is set and 0x100 when it is clear. Established; what
  the first register and the copied words are in the hardware: see the chips page.

## The register walk

`dump_hub_registers` takes no arguments. It walks a table of `{first, last}` offset pairs at 0xc000000001597388 in the
data section: 16 bytes per pair, `last` inclusive, ended by a pair whose first word is negative. There are 67 pairs
(521 doubleword registers); the list, with counts, is in the chips page. Its node set is the word at gp+0x2824 as the
count, increased by 4 when the word at +0x184 of the per-node structure the routine fetches equals 0x5045626b; the NASID of
index `i` comes from the halfword table at gp-0x1cc8 below the count and from the halfword at +0x188 of that structure
(four bytes per entry) above it. For each node and each pair it forms the address 0x9200000001800000 (pool word gp+0x8f0)
plus the NASID shifted left 33 plus the offset, and loads one doubleword (`ld`, no `nofault` wrapper, no value test). It
stores the value into slot `(offset >> 3) & 0x7ff` of the 16 KB staging buffer in its frame, which it first fills with
0x5555555555555555 (the doubleword at gp+0x1298). When the 16 KB page number (offset >> 14) or the bits 23:20 of the offset
change it flushes the buffer through `compress_and_write_block` (0xc0000000002c8f6c, length 0x4000, 0x200 in the last
argument) and refills it. A flush that returns nonzero ends the walk with that value. Established for the shape; the
meaning of 0x5045626b, which the code takes as selecting four extra nodes, is unknown here. The pair that follows the
real-time counter (0x30100) is offsets 0x40200 to 0x40210 (three doublewords), the first register of which is where the
observed failure in a crash dump of a guest kernel stops.

## A failed load in the walk

The loads are plain, so a load that gets a bus error is taken as the processor's data bus error exception, not returned as a
status. The path to the message, all established by reading the routines:

1. `dobuserre` (0xc00000000005c2a0, 1352 bytes) takes the exception, reads the exception code field of the saved Cause (masked value 0x1c for a
   data bus error, 0x18 for an instruction bus error), and asks `vtop_attr_get` (0xc00000000005e814) to classify the bad address. For an address of the form
   0x92xxxxxxxxxxxxxx the result is cache attribute 2 (bits 61:59 give 2) and space number 1 (bits 58:57), and the address
   is reduced to its low 40 bits (`vphys_get_pattr`, 0xc00000000005e6d8). Attribute 2 with space 1 goes to
   `handle_io_buserr` (call at 0xc00000000005c6bc).
2. `handle_io_buserr` (0xc00000000005cedc, 592 bytes) turns its third argument into a mode (0 becomes 1, 1 becomes 0,
   everything else 2) and calls `get_matching_spool_addr` (0xc00000000005aa14) for the address rounded down to 8 bytes. For a
   result of 0, 1 or 2 it calls `kl_ioerror_handler` (0xc00000000005e4dc) with error kind 5, the address and the mode;
   for -1 it logs and returns the saved status word.
3. `kl_ioerror_handler` builds an error record (a flags word, the node halfwords at +12 and +14, the address at +16 with flag
   bit 25 set to say so) and calls `hub_ioerror_handler` (0xc000000000077ccc). For kind 5 and a widget number of 1
   (the Hub's own registers) that routine calls `hub_ioerror_local` (0xc000000000077a80, described in ip35-hub-errors.md).
4. `hub_ioerror_local` returns 0 for modes 0, 2 and 3. For mode 1 it prints through `cmn_err` and calls `kl_panic`,
   whichever way the permission test goes. The address in the "Error accessing Hub Register %x" message is the record's
   address, the low 40 bits of the faulting address: for node 0, 0x1800000 plus the register offset. A load at
   0x9200000001840200 prints "Hub Register 1840200".
5. `kl_panic` is entered while the dump is already running under the first panic, so it prints "DOUBLE PANIC" and restarts
   the machine; the walk and the rest of the dump are abandoned.

The permission test in step 4 reads the doubleword at +0 of the faulting node's register window and tests one bit of it
(established). Inferred from the message wording: the bit-set path means the access was permitted and the error is a real
hardware error rather than a refused one. Which register is at +0 is not named in the binary; the public SN1 register header
names it as the processor-interface CPU protection register (inferred). The consequence: the dump pass only reaches the
message when a register the walk reads is not answered; if every register in the table answers with a doubleword, the pass
completes silently and the dump continues. Anything that stands in for the Hub and must keep crash dumps alive therefore has to answer every
register in the table (including the performance-counter block at 0x40200 to 0x40210 and 0x50200 to 0x50210 on each
processor interface) or set `dump_hub_info` to 0.

Open: the reset values and fields of the performance-counter registers (names only in the public header); the meaning of
the 0x5045626b word; which of the spool-match results a never-answered uncached load produces on hardware. Route: a Bedrock
hardware reference, or a capture of a real crash dump with the Hub pass enabled.
