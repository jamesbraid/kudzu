# Tier-2 routine readings, kudzu round 2 batch 6

- `xlv_tab_subvol_size` in `6.5.22/unix.IP27` at 0xc00000000005b70c
  **Established.** It is an 8-byte stub. Its only instruction is the return, with v0 = 0x0, and it makes no calls and no memory accesses. The name suggests a size table lookup, but this build does no lookup.
  Reviewed: 2026-11-12 (sonnet)

- `wd95intr` in `6.5.22/unix.IP27` at 0xc00000000005b714
  **Established.** It is an 8-byte stub that returns the constant 0x1 in v0, with no calls and no memory accesses. The name suggests an interrupt handler for a WD95 device, but in this build it does no work.
  Reviewed: 2026-11-12 (sonnet)

- `wd95_earlyinit` in `6.5.22/unix.IP27` at 0xc00000000005b71c
  **Established.** It is an 8-byte stub that returns the constant 0x1 in v0, with no calls and no memory accesses. The name suggests an early initialization step for the same device, and this build does nothing there.
  Reviewed: 2026-11-12 (sonnet)

- `crc16` in `6.5.22/unix.IP27` at 0xc00000000006a654
  **Inferred.** It loads a 16-bit value through a1 (the `lhu` at 0xc00000000006a654), looks up two table words (the `lw` pair at 0xc00000000006a684 and 0xc00000000006a688) at addresses derived from the word at 0xc000000001436d28 (the facts tie the return value to that address, not to the table contents), and stores updated halfwords back through a1 (the `sh` stores at 0xc00000000006a698, 0xc00000000006a6a8 and 0xc00000000006a6b8). A branch at 0xc00000000006a69c jumps to 0xc00000000006a6ac when a value the facts do not identify is zero, skipping the store at 0xc00000000006a6a8; the return is at 0xc00000000006a6b4. The facts contain no backward branch, so they do not show a loop. Role (from name): the name suggests a table-driven CRC-16, but the polynomial and the table contents are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_phase_bits` in `6.5.22/unix.IP27` at 0xc00000000006a6bc
  **Inferred.** It reads one 64-bit word (the `ld` at 0xc00000000006a6c4) and returns a value the facts do not resolve (the return at 0xc00000000006a6e8 has v0 = ?). The facts do not show the address of that load. `nic_access_hub` at 0xc00000000006be18 calls it. Role (from name): the name suggests it returns the phase bits of a serial bus, but the bit meanings are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_write_byte` in `6.5.22/unix.IP27` at 0xc00000000006a78c
  **Inferred.** It writes one byte by calling `nic_write` at 0xc00000000006a71c (the `jal` at 0xc00000000006a7c0) once per bit, with the loop counter s0 running up to 8 (the `bne` at 0xc00000000006a7d4). It checks each result for a negative value (the `bltz` at 0xc00000000006a7c8) and returns 0 or that first negative result. The bit timing and encoding come from the callee, which the facts do not show.
  Reviewed: 2026-11-12 (sonnet)

- `nic_read_byte` in `6.5.22/unix.IP27` at 0xc00000000006a800
  **Inferred.** It reads one byte by calling `nic_read` at 0xc00000000006a760 eight times, with the loop bound 8 at the `bne` at 0xc00000000006a838, and checks each result for a negative value (the `bltz` at 0xc00000000006a828). The return is one of an unresolved value or the `nic_read` result. The facts do not show how the bits are assembled into the byte.
  Reviewed: 2026-11-12 (sonnet)

- `nic_next_scan` in `6.5.22/unix.IP27` at 0xc00000000006a864
  **Inferred.** The branch at 0xc00000000006a878 skips the rest of the routine when the word at [a0+20] is nonzero. When it is zero, the routine calls `nic_presence` at 0xc00000000006a890. A negative `nic_presence` result goes to a separate exit (the `bltz` at 0xc00000000006a898). When the result is zero (the `beq` at 0xc00000000006a8a0) it stores to [a0+0x1c] and [a0+0x18] and calls `nic_write_byte` with the byte 0xf0 at 0xc00000000006a8bc, which matches the 1-Wire SEARCH ROM command code (outside knowledge). After that it makes `nic_write` and `nic_read` calls (0xc00000000006a8ec, 0xc00000000006a910, 0xc00000000006a924, 0xc00000000006a970) under a counter at [a0+24] that is compared with 0x40, and stores into the record at a0 (offsets 0x10 through 0x1f). The return is one of 0x0, 0x1, 0x2, 0x4 or a callee result. Role (from name): the name suggests one step of a ROM-ID search; the exact algorithm is not settled by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_setup` in `6.5.22/unix.IP27` at 0xc00000000006aa48
  **Established.** It stores two 8-byte words at a0+0 and a0+8 and two 4-byte words at a0+0x10 and a0+0x14 (the `sd` and `sw` at 0xc00000000006aa48 through 0xc00000000006aa60), then calls `nic_presence` at 0xc00000000006aa5c with its own arguments and returns that result. The stored values are not resolved, so the facts settle the shape of the code, not what each field means.
  Reviewed: 2026-11-12 (sonnet)

- `nic_next` in `6.5.22/unix.IP27` at 0xc00000000006aa70
  **Inferred.** It calls `nic_next_scan` at 0xc00000000006aa90 and branches on the result (the `beq` at 0xc00000000006aa9c); the return at 0xc00000000006aab0 is one of an unresolved value or 0x3. On the other branch it stores bytes and runs loops under counters a6 and a7 (branches against the bounds 0x8, 0x40 and 0x8 in the facts), then stores single bytes through a1 (offsets 0 through 5, the `sb` run from 0xc00000000006aba8 to 0xc00000000006abd0), through a2 (the `beq` at 0xc00000000006abe8 tests it against zero) and through a3 (tested at 0xc00000000006abd4 and 0xc00000000006abf0). The facts show a1, a2 and a3 are each compared with zero before the stores, and a3 is a pointer, not a counter. The buffer layouts are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_match_rom` in `6.5.22/unix.IP27` at 0xc00000000006ac00
  **Inferred.** It calls `nic_presence` at 0xc00000000006ac1c. A negative result (the `bltz` at 0xc00000000006ac24) goes to a separate exit; a zero result (the `beq` at 0xc00000000006ac2c) goes straight to the write sequence; a positive result reaches `printf` at 0xc00000000006ac34. It then sends the byte 0x55 through `nic_write_byte` at 0xc00000000006ac54, which matches the 1-Wire MATCH ROM command code (outside knowledge), followed by eight more `nic_write_byte` calls: the byte at a1, the bytes at a2+5 down to a2+0, and the byte at a3 (the `jal`s from 0xc00000000006ac68 to 0xc00000000006acdc). The layout matches a 1-Wire ROM ID of family code, serial and CRC, but the facts only show the byte sources. Each write result is checked for a negative value.
  Reviewed: 2026-11-12 (sonnet)

- `nic_read_one_page` in `6.5.22/unix.IP27` at 0xc00000000006ad3c
  **Inferred.** It calls `nic_match_rom` at 0xc00000000006ada0 and, when that returns zero (the `beq` at 0xc00000000006ada8), sends 0xf0 through `nic_write_byte` at 0xc00000000006adcc, followed by two more `nic_write_byte` calls with values the facts do not resolve. It compares the byte at a1 with 0x9 (the `beq` at 0xc00000000006ae10), then reads bytes through `nic_read_byte` (at 0xc00000000006ae18 and 0xc00000000006ae9c) in a loop bounded by 0x20. It calls `crc16` at 0xc00000000006ae48 in a second loop bounded by 0x20, with a1 pointing at the stack. It compares a value the facts do not identify with 0xb001 at 0xc00000000006ae68, and calls `printf` at 0xc00000000006ae7c. The facts do not say what the 0xb001 comparison marks.
  Reviewed: 2026-11-12 (sonnet)

- `nic_read_mfg` in `6.5.22/unix.IP27` at 0xc00000000006aecc
  **Inferred.** It calls `nic_read_one_page` four times, at 0xc00000000006af7c and 0xc00000000006afec (fifth argument a4 = 0x0 and 0x1) and at 0xc00000000006b188 and 0xc00000000006b1b0 (a4 = 0x0 and 0x1 again), and calls `nic_match_rom` at 0xc00000000006afbc. It writes the bytes 0xaa, 0x00 and 0x01 with `nic_write_byte` (at 0xc00000000006b004, 0xc00000000006b018 and 0xc00000000006b02c), checking each result for a negative value. It calls `crc16` four times on the same stack buffer (at 0xc00000000006b050, 0xc00000000006b05c, 0xc00000000006b070 and 0xc00000000006b130, with a0 = 0x0, 0x1, an unresolved value and 0xaa), and calls `printf` at 0xc00000000006b160 with a2 = 0xb001. Role (from name): the name suggests it reads the manufacturing data area. The facts do not give the record layout, and they do not settle which path reaches each call.
  Reviewed: 2026-11-12 (sonnet)

- `char2hex` in `6.5.22/unix.IP27` at 0xc00000000006b1e0
  **Inferred.** It writes two bytes through a0 (at a0+0 and a0+1, the `sb` at 0xc00000000006b23c and 0xc00000000006b25c), after loading from the constant addresses 0xc0000000003c5948, 0xc0000000003c5950 and 0xc0000000003c5958 and two words at 0xc00000000006b21c and 0xc00000000006b244. The pair of bytes fits the two ASCII hex digits of one input byte, which the name suggests, but the digit table is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `chars2hex` in `6.5.22/unix.IP27` at 0xc00000000006b260
  **Inferred.** It appends hex text to a string. It finds the current length with `strlen` at 0xc00000000006b27c and calls `char2hex` at 0xc00000000006b2a8 in a loop bounded by the count in a2 (the `blez` at 0xc00000000006b290 and the `bne` at 0xc00000000006b2b8). It writes one byte at strlen(a0) + 2 x a2 (the `sb` at 0xc00000000006b2e8), which is consistent with a terminator after the last pair of digits.
  Reviewed: 2026-11-12 (sonnet)

- `chars2print` in `6.5.22/unix.IP27` at 0xc00000000006b2ec
  **Inferred.** It appends a copy of a2 bytes from a1 to the string at a0, found with `strlen` at 0xc00000000006b308. Bytes are tested against the constants 0x3a, 0x3b and 0xff (the `beq` branches at 0xc00000000006b374, 0xc00000000006b37c and 0xc00000000006b384). A match on 0x3a or 0x3b goes to the store at 0xc00000000006b3ac, and a match on 0xff goes to the store at 0xc00000000006b3b8; other bytes reach the stores at 0xc00000000006b358 and 0xc00000000006b39c. Role (from name): the name suggests printable-character conversion, but the replacement byte and the reason for each constant are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `nic_eaddr` in `6.5.22/unix.IP27` at 0xc00000000006b3f8
  **Inferred.** When a2 is nonzero (the `beq` at 0xc00000000006b40c), it initializes the device with `nic_setup` at 0xc00000000006b41c; a positive result prints with `printf` at 0xc00000000006b434 and 0xc00000000006b43c, and a2 equal to zero reaches `printf` at 0xc00000000006b460. It steps through ROM IDs with `nic_next` at 0xc00000000006b47c, sends 0xf0, 0x0 and 0x0 through `nic_write_byte` at 0xc00000000006b4ec, 0xc00000000006b4fc and 0xc00000000006b50c, and reads bytes through `nic_read_byte` at 0xc00000000006b51c. It calls `crc16` twice on the same stack buffer (at 0xc00000000006b554 and 0xc00000000006b5a8). It compares values with the constants 0x91, 0x8d, 0xa and 0xb001 (for example at 0xc00000000006b4bc and 0xc00000000006b584). Role (from name): the name suggests reading a station address from the device. The facts do not show where the address bytes are stored, and they do not identify the device family for 0x91.
  Reviewed: 2026-11-12 (sonnet)

- `nic_access_mcr32` in `6.5.22/unix.IP27` at 0xc00000000006b66c
  **Inferred.** It writes one 4-byte word (the `sw` at 0xc00000000006b680; the stored value is not resolved). It then polls the word at a0 for the bit mask 0x2 (the `bne` and `beq` branches from 0xc00000000006b690 to 0xc00000000006b6d0). The poll loop makes no call: `us_delay` at 0xc00000000006b6d8 is where the `bne` branches once the bit is set, and its argument is a3 at entry. The return value is not resolved. Role (from name): the name suggests a register access that waits for a ready bit; `nic_access_hub` polls the same way.
  Reviewed: 2026-11-12 (sonnet)

- `nic_get_eaddr` in `6.5.22/unix.IP27` at 0xc00000000006b6f4
  **Inferred.** When a1 is nonzero (the `beq` at 0xc00000000006b71c) it stores a 4-byte word through a1 (the `sw` at 0xc00000000006b724). It calls `nic_eaddr` at 0xc00000000006b73c with `nic_access_mcr32` at 0xc00000000006b66c as its first argument, so the access routine is passed in as a callback. The `bne` at 0xc00000000006b750 branches back to 0xc00000000006b738, repeating the call, while a counter is not the constant -1. It prints the `nic_eaddr` result with `printf` at 0xc00000000006b760 and returns it at 0xc00000000006b788. The other `printf` at 0xc00000000006b794 is passed the entry arguments, not that result.
  Reviewed: 2026-11-12 (sonnet)

- `nic_mfg_next` in `6.5.22/unix.IP27` at 0xc00000000006b7ac
  **Inferred.** It returns early when a1 is zero (the `bne` at 0xc00000000006b7c0). Otherwise it calls `strlen` on a1 at 0xc00000000006b7e0 and `nic_next` at 0xc00000000006b7f8, and dispatches on the `nic_next` result through a 7-entry jump table at 0xc0000000003c5bf8 (the indirect jump at 0xc00000000006b81c, with an unsigned bound check at 0xc00000000006b810). `nic_read_mfg` at 0xc00000000006b850 is reached when a byte at sp+6 equals 0xb or 0x9 (the branches at 0xc00000000006b82c and 0xc00000000006b834). Other paths copy words with unaligned stores (the `sdl` and `sdr` runs), and append text with `strcat` at 0xc000000000146450, `chars2hex` and `chars2print`. The constant 0x50617274 at 0xc00000000006bb64 is the ASCII text "Part". The facts do not settle the full field list, which cases do what, or the return convention.
  Reviewed: 2026-11-12 (sonnet)

- `nic_info_get` in `6.5.22/unix.IP27` at 0xc00000000006bc4c
  **Inferred.** It stores one byte through a2 (the `sb` at 0xc00000000006bc54) and calls `nic_setup` at 0xc00000000006bc70 on a local record. When the `nic_setup` result is not negative (the `bltz` at 0xc00000000006bc78) it calls `nic_mfg_next` at 0xc00000000006bc84, repeating that call while it returns zero (the `beq` at 0xc00000000006bc8c). It returns the result of `nic_setup` or `nic_mfg_next`. Role (from name): the name suggests it collects NIC information into a text buffer, which fits the `nic_mfg_next` call, but the facts do not show the buffer.
  Reviewed: 2026-11-12 (sonnet)

- `nic_vertex_info_get` in `6.5.22/unix.IP27` at 0xc00000000006bca4
  **Inferred.** It makes one call, to `hwgraph_info_get_LBL` at 0xc000000000296160 (the `jal` at 0xc00000000006bcb8), passing the vertex from a0, the label constant at 0xc0000000003c5c58 and a pointer to a stack slot as a2. The return at 0xc00000000006bcc8 has v0 unresolved in the facts. The facts do not show that the return is the call's own result; the instruction after the call loads v0 from that stack slot. The meaning of the label is not settled.
  Reviewed: 2026-11-12 (sonnet)

- `nic_vertex_info_set` in `6.5.22/unix.IP27` at 0xc00000000006bcd0
  **Inferred.** It first calls `nic_vertex_info_get` at 0xc00000000006bce8 and branches on the result (the `beq` at 0xc00000000006bcf0): a nonzero result returns at 0xc00000000006bd04. When the result is zero it allocates a 0x800-byte buffer with `kern_malloc` at 0xc00000000006bd10, fills it through `nic_info_get` at 0xc00000000006bd34 and measures the text with `strlen`. It allocates a second buffer with `kern_malloc` at 0xc00000000006bd4c; when that succeeds (the `bne` at 0xc00000000006bd54) it copies the text with `strcpy` at 0xc00000000006bdc4 and frees the first buffer with `kern_free` at 0xc00000000006bdcc. It calls `hwgraph_info_add_LBL` at 0xc00000000006bd70 and re-reads the label with `nic_vertex_info_get` at 0xc00000000006bd78. When the re-read value differs from the buffer it frees the buffer with `kern_free` at 0xc00000000006bd88; when it matches it calls `hwgraph_info_export_LBL` at 0xc00000000006bda4 and `nic_vmc_check` at 0xc00000000006bdb0. Role (from name): the name suggests it attaches NIC information to a hardware graph vertex. The facts do not show every branch.
  Reviewed: 2026-11-12 (sonnet)

- `nic_vertex_info_match` in `6.5.22/unix.IP27` at 0xc00000000006bddc
  **Inferred.** It looks up the NIC information for a vertex with `nic_vertex_info_get` at 0xc00000000006bde4. If the lookup returns a nonzero value (the `bne` at 0xc00000000006bdec), it searches that text with `strstr` at 0xc00000000006be08 for the string in a1. When the lookup returns zero it reaches the return at 0xc00000000006bdfc, whose v0 is one of 0x0 or an unresolved value. Role (from name): the name suggests a match test on the vertex information text.
  Reviewed: 2026-11-12 (sonnet)

- `nic_access_hub` in `6.5.22/unix.IP27` at 0xc00000000006be18
  **Inferred.** It calls `nic_get_phase_bits` at 0xc00000000006be34, then loads 16-bit values from the absolute address 0xffffffffffffc420 (the `lh` at 0xc00000000006be3c) and through pointer tables. It builds a register address from a table entry, plus the constant 0x9200000001a00000 (seen in a6 at the `us_delay` call), stores a doubleword there (the `sd` at 0xc00000000006be80), and polls that doubleword for the bit mask 0x2 (the `bne` and `beq` branches from 0xc00000000006beb4 to 0xc00000000006bef4). The poll loop makes no call: `us_delay` at 0xc00000000006befc is where the `bne` branches once the bit is set, with a3 at entry as its argument. This is the same poll pattern as `nic_access_mcr32`. The facts do not show which hub register it reaches, so that part is a reading from the address and the poll pattern.
  Reviewed: 2026-11-12 (sonnet)
