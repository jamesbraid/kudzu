# Tier-2 routine readings, kudzu round 2 batch 6

- `xlv_tab_subvol_size` in `6.5.22/unix.IP27` at 0xc00000000005b70c
  **Established.** It is an 8-byte stub. Its only instruction is the return, with v0 = 0x0, and it makes no calls and no memory accesses. The name suggests a size table lookup, but this build does no lookup.

- `wd95intr` in `6.5.22/unix.IP27` at 0xc00000000005b714
  **Established.** It is an 8-byte stub that returns the constant 0x1 in v0, with no calls and no memory accesses. The name suggests an interrupt handler for a WD95 device, but in this build it does no work.

- `wd95_earlyinit` in `6.5.22/unix.IP27` at 0xc00000000005b71c
  **Established.** It is an 8-byte stub that returns the constant 0x1 in v0, with no calls and no memory accesses. The name suggests an early initialization step for the same device, and this build does nothing there.

- `crc16` in `6.5.22/unix.IP27` at 0xc00000000006a654
  **Inferred.** It loads 16-bit values from the buffer at a1 (the `lhu` at 0xc00000000006a654), looks up table words at 0xc000000001436d28 (the `lw` pairs at 0xc00000000006a684 and 0xc00000000006a688), and stores updated halfwords back through a1 (the `sh` stores from 0xc00000000006a698). A branch at 0xc00000000006a69c jumps to 0xc00000000006a6ac when a value the facts do not identify is zero, before the return at 0xc00000000006a6b4. The facts contain no backward branch, so they do not show a loop. The name suggests a table-driven CRC-16, but the polynomial and the table contents are not in the facts.

- `nic_get_phase_bits` in `6.5.22/unix.IP27` at 0xc00000000006a6bc
  **Inferred.** It reads one 64-bit word from the fixed address 0x9000000010000078 (the `ld` at 0xc00000000006a6c4) and returns a value the facts do not resolve (the return at 0xc00000000006a6e8 has v0 = ?). `nic_access_hub` at 0xc00000000006be18 calls it. The name suggests it returns the phase bits of a serial bus, but the bit meanings are not in the facts.

- `nic_write_byte` in `6.5.22/unix.IP27` at 0xc00000000006a78c
  **Inferred.** It writes one byte by calling `nic_write` at 0xc00000000006a71c (the `jal` at 0xc00000000006a7c0) once per bit, with the loop counter s0 running up to 8 (the `bne` at 0xc00000000006a7d4). It checks each result for a negative value (the `bltz` at 0xc00000000006a7c8) and returns 0 or that first negative result. The bit timing and encoding come from the callee, which the facts do not show.

- `nic_read_byte` in `6.5.22/unix.IP27` at 0xc00000000006a800
  **Inferred.** It reads one byte by calling `nic_read` at 0xc00000000006a760 eight times, with the loop bound 8 at the `bne` at 0xc00000000006a838, and checks each result for a negative value (the `bltz` at 0xc00000000006a828). The return is one of an unresolved value or the `nic_read` result. The facts do not show how the bits are assembled into the byte.

- `nic_next_scan` in `6.5.22/unix.IP27` at 0xc00000000006a864
  **Inferred.** It appears to run one step of a ROM-ID search over a device. It tests presence through `nic_presence` at 0xc00000000006a890, then sends the byte 0xf0 through `nic_write_byte` at 0xc00000000006a8bc, which matches the 1-Wire SEARCH ROM command code. Bit-by-bit branches on the counter at [a0+24] and the stores into the record at a0 (offsets 0x10 through 0x1f) form the search state, and the return is one of 0x0, 0x1, 0x2, 0x4 or a callee result. The exact search algorithm is not settled by the facts.

- `nic_setup` in `6.5.22/unix.IP27` at 0xc00000000006aa48
  **Established.** It stores two 8-byte words at a0+0 and a0+8 and two 4-byte words at a0+0x10 and a0+0x14 (the `sd` and `sw` at 0xc00000000006aa48 through 0xc00000000006aa60), then calls `nic_presence` at 0xc00000000006aa5c with its own arguments and returns that result. The stored values are not resolved, so the facts settle the shape of the code, not what each field means.

- `nic_next` in `6.5.22/unix.IP27` at 0xc00000000006aa70
  **Inferred.** It calls `nic_next_scan` at 0xc00000000006aa90 and branches on the result (the `beq` at 0xc00000000006aa9c). It then stores single bytes into the buffer at a1 (offsets 0 through 5, the `sb` run from 0xc00000000006abac to 0xc00000000006abd0) and into the locations at a2 and a3, under loops on the counters a6, a7 and s1. The return is 0x3 on one path or an unresolved value on another. The buffer layouts are not in the facts, so the role is read from the scan results it passes on.

- `nic_match_rom` in `6.5.22/unix.IP27` at 0xc00000000006ac00
  **Inferred.** It checks presence through `nic_presence` at 0xc00000000006ac1c and prints a diagnostic with `printf` at 0xc00000000006ac34 when that fails. It then sends the byte 0x55 at 0xc00000000006ac54, which matches the 1-Wire MATCH ROM command code, followed by eight bytes through `nic_write_byte`: the byte at a1, six bytes at a2+0 through a2+5, and the byte at a3 (the `jal`s from 0xc00000000006ac68 to 0xc00000000006acdc). The layout matches a 1-Wire ROM ID of family code, serial and CRC, but the facts only show the byte sources. Each write result is checked for a negative value.

- `nic_read_one_page` in `6.5.22/unix.IP27` at 0xc00000000006ad3c
  **Inferred.** It selects a device through `nic_match_rom` at 0xc00000000006ada0 and sends 0xf0 at 0xc00000000006adcc, then reads bytes through `nic_read_byte` (at 0xc00000000006ae18 and 0xc00000000006ae9c) and compares the byte count with 0x9. It computes a CRC over the bytes with `crc16` at 0xc00000000006ae48 and prints with `printf` at 0xc00000000006ae7c. The constant 0xb001 at 0xc00000000006ae68 is compared with a read value, but the facts do not say what that value marks.

- `nic_read_mfg` in `6.5.22/unix.IP27` at 0xc00000000006aecc
  **Inferred.** It reads two pages through `nic_read_one_page` at 0xc00000000006af7c and 0xc00000000006afec, with the page flag set to 0 and then 1, after selecting the device through `nic_match_rom` at 0xc00000000006afbc. It writes the bytes 0xaa, 0x00 and 0x01 with `nic_write_byte` (at 0xc00000000006b004, 0xc00000000006b018 and 0xc00000000006b02c) and checks several buffers with `crc16` (at 0xc00000000006b050, 0xc00000000006b05c, 0xc00000000006b070 and 0xc00000000006b130). It reports a failure with `printf` at 0xc00000000006b160. The name suggests it reads the manufacturing data area, but the record layout is not in the facts.

- `char2hex` in `6.5.22/unix.IP27` at 0xc00000000006b1e0
  **Inferred.** It writes two bytes through a0 (at a0+0 and a0+1, the `sb` at 0xc00000000006b23c and 0xc00000000006b25c), after loading from the constant addresses 0xc0000000003c5948, 0xc0000000003c5950 and 0xc0000000003c5958 and two words at 0xc00000000006b21c and 0xc00000000006b244. The pair of bytes fits the two ASCII hex digits of one input byte, which the name suggests, but the digit table is not in the facts.

- `chars2hex` in `6.5.22/unix.IP27` at 0xc00000000006b260
  **Inferred.** It appends hex text to a string. It finds the current length with `strlen` at 0xc00000000006b27c and calls `char2hex` at 0xc00000000006b2a8 in a loop bounded by the count in a2 (the `blez` at 0xc00000000006b290 and the `bne` at 0xc00000000006b2b8). It writes one byte at strlen(a0) + 2 x a2 (the `sb` at 0xc00000000006b2e8), which is consistent with a terminator after the last pair of digits.

- `chars2print` in `6.5.22/unix.IP27` at 0xc00000000006b2ec
  **Inferred.** It appends a copy of a2 bytes from a1 to the string at a0, found with `strlen` at 0xc00000000006b308. Bytes are tested against the constants 0x3a, 0x3b and 0xff (the `beq` branches at 0xc00000000006b374, 0xc00000000006b37c and 0xc00000000006b384), and each test selects a stored byte (the `sb` stores at 0xc00000000006b358, 0xc00000000006b39c and 0xc00000000006b3ac). The name suggests printable-character conversion, but the replacement byte and the reason for each constant are not in the facts.

- `nic_eaddr` in `6.5.22/unix.IP27` at 0xc00000000006b3f8
  **Inferred.** It initializes the device with `nic_setup` at 0xc00000000006b41c, steps through ROM IDs with `nic_next` at 0xc00000000006b47c, and sends 0xf0 through `nic_write_byte` at 0xc00000000006b4ec. It reads bytes through `nic_read_byte` at 0xc00000000006b51c and checks two buffers with `crc16` at 0xc00000000006b554 and 0xc00000000006b5a8. It compares read values with the constants 0x91, 0x8d, 0xa and 0xb001 (for example at 0xc00000000006b4bc and 0xc00000000006b584), and prints with `printf` at 0xc00000000006b434, 0xc00000000006b43c and 0xc00000000006b460. The name suggests reading a station address from the device. The facts do not show where the address bytes are stored, and they do not identify the device family for 0x91.

- `nic_access_mcr32` in `6.5.22/unix.IP27` at 0xc00000000006b66c
  **Inferred.** It writes one 4-byte word to a0 (the `sw` at 0xc00000000006b680; the stored value is not resolved). It then polls the word at a0 for the bit mask 0x2, calling `us_delay` at 0xc00000000006b6d8 between polls (the `bne` and `beq` branches from 0xc00000000006b690 to 0xc00000000006b6d0). The return value is not resolved. The name suggests a register access that waits for a ready bit, and `nic_access_hub` polls the same way.

- `nic_get_eaddr` in `6.5.22/unix.IP27` at 0xc00000000006b6f4
  **Inferred.** It stores a 4-byte word through a1 (the `sw` at 0xc00000000006b724), then calls `nic_eaddr` at 0xc00000000006b73c with `nic_access_mcr32` at 0xc00000000006b66c as its first argument, so the access routine is passed in as a callback. It prints the result with `printf` at 0xc00000000006b760 and 0xc00000000006b794. It loops while the value is not the constant -1 (the `bne` at 0xc00000000006b750), and it returns the `nic_eaddr` result at 0xc00000000006b788.

- `nic_mfg_next` in `6.5.22/unix.IP27` at 0xc00000000006b7ac
  **Inferred.** It works through a list of manufacturing-data fields. It dispatches on the value from `nic_next` at 0xc00000000006b7f8 through a 7-entry jump table at 0xc0000000003c5bf8 (the indirect jump at 0xc00000000006b81c, with an unsigned bound check at 0xc00000000006b810). Each case copies 8-byte table words with unaligned stores (the `sdl` and `sdr` runs) and appends labels and values to a text buffer with `strcat` at 0xc000000000146450, using `chars2hex` and `chars2print`. The constant 0x50617274 at 0xc00000000006bb64 is the ASCII text "Part". `nic_read_mfg` at 0xc00000000006b850 supplies the data. The facts do not settle the full field list or the return convention.

- `nic_info_get` in `6.5.22/unix.IP27` at 0xc00000000006bc4c
  **Inferred.** It stores one byte through a2 (the `sb` at 0xc00000000006bc54), calls `nic_setup` at 0xc00000000006bc70 on a local record, and then calls `nic_mfg_next` at 0xc00000000006bc84, repeating that call while it returns zero (the `beq` at 0xc00000000006bc8c). It returns the result of `nic_setup` or `nic_mfg_next`. The name suggests it collects NIC information into a text buffer, which fits the `nic_mfg_next` call, but the facts do not show the buffer.

- `nic_vertex_info_get` in `6.5.22/unix.IP27` at 0xc00000000006bca4
  **Established.** It makes one call, to `hwgraph_info_get_LBL` at 0xc000000000296160 (the `jal` at 0xc00000000006bcb8), passing the vertex from a0 and the label constant at 0xc0000000003c5c58. It returns that call's result. The facts show the forwarding only, so the meaning of the label is not settled.

- `nic_vertex_info_set` in `6.5.22/unix.IP27` at 0xc00000000006bcd0
  **Inferred.** It first calls `nic_vertex_info_get` at 0xc00000000006bce8 and branches on the result (the `beq` at 0xc00000000006bcf0). Its path allocates a 0x800-byte buffer with `kern_malloc` at 0xc00000000006bd10, fills it through `nic_info_get` at 0xc00000000006bd34, and measures the text with `strlen`. It copies the text into a second allocation with `strcpy` at 0xc00000000006bdc4, and then calls `hwgraph_info_add_LBL` at 0xc00000000006bd70 and `hwgraph_info_export_LBL` at 0xc00000000006bda4. It frees its temporary buffers with `kern_free` at 0xc00000000006bd88 and 0xc00000000006bdcc and checks the vertex with `nic_vmc_check` at 0xc00000000006bdb0. The name suggests it attaches NIC information to a hardware graph vertex, and those calls fit that reading, but the facts do not show every branch.

- `nic_vertex_info_match` in `6.5.22/unix.IP27` at 0xc00000000006bddc
  **Inferred.** It looks up the NIC information for a vertex with `nic_vertex_info_get` at 0xc00000000006bde4. If the lookup returns a value, it searches that text with `strstr` at 0xc00000000006be08 for the string in a1 (the `bne` at 0xc00000000006bdec). Otherwise it returns 0, and the return at 0xc00000000006bdfc is one of 0x0 or an unresolved value. The name suggests a match test on the vertex information text.

- `nic_access_hub` in `6.5.22/unix.IP27` at 0xc00000000006be18
  **Inferred.** It loads 16-bit values from the absolute address 0xffffffffffffc420 (the `lh` at 0xc00000000006be3c) and from two pointer tables at 0xc0000000014d39a8 and 0xc0000000014d3800. It builds a register address from a table entry, with the constant 0x9200000001a00000 at the `us_delay` call at 0xc00000000006befc. It calls `nic_get_phase_bits` at 0xc00000000006be34 and polls a word for the bit mask 0x2, delaying with `us_delay` between polls (the `bne` and `beq` branches from 0xc00000000006beb4 to 0xc00000000006bef4). This is the same poll pattern as `nic_access_mcr32`. The facts do not show which hub register it reaches, so that part is a reading from the address and the poll pattern.
