# Routine role reads, kudzu tier-2 batch 10

These entries say what 25 routines of the IP27 kernel image do, read from the instruction facts rendered for each one. Every label is Inferred: the facts give the control flow, loads, stores and callees, but not the meaning of the names or the hardware behind each address, so the roles below are a reading of those facts.

- `ioc3_intb_needed` in `6.5.22/unix.IP27` at 0xc000000000097ba0
  **Inferred.** Calls the function pointer at 0xc0000000000979e0 with a1 zero and a0, a2 and a3 passed through, then reads the eight bytes at offset 0 of the pointer it returns. A null return takes the early branch at 0xc000000000097bb0, and otherwise the byte at offset 8 is tested against the mask 0xfa at 0xc000000000097bc0. It returns 0 or 1 from 0xc000000000097bcc. The name suggests an interrupt-line check, but the facts show only the mask test, not what the bits mean.

- `ioc3_get_mem` in `6.5.22/unix.IP27` at 0xc000000000097be4
  **Inferred.** Passes its four arguments to the pointer at 0xc000000000296ee8 (`hwgraph_fastinfo_get`), then loads the eight-byte word at offset 0x18 of the record that call returns and gives that word back. The facts show only the field read, so whether that field is a memory base is the name's claim, not something the code shows.

- `ioc3_pckm_mlreset` in `6.5.22/unix.IP27` at 0xc000000000097c04
  **Inferred.** Calls `pckm_console_register` through the pointer at 0xc0000000000a37d8 with a0 set to a stack buffer at sp-64, a1 set to the constant address 0xc0000000000994f0, and a2 set to 0xc000000000097c50. That a2 value is the first byte past this routine, so it is probably the address of a neighbouring routine or data used as a callback, and the routine returns the call's result. The reading of the constants is an inference from their values.

- `get_myid` in `6.5.22/unix.IP27` at 0xc00000000009a1b8
  **Inferred.** Loads the eight-byte word at the fixed address 0x9200000001000020 and returns it unchanged, with no shift or mask beyond a shift by zero. The routine is a single load from one address. The name suggests it returns a node or processor identifier, but the facts do not show what the word holds.

- `dequeue_msg` in `6.5.22/unix.IP27` at 0xc00000000009a20c
  **Inferred.** Compares the two words at offsets 0x194 and 0x198 of its argument (decimal 404 and 408) at 0xc00000000009a224, and on equality returns -1 or 0 through 0xc00000000009a2bc, or -1 directly at 0xc00000000009a2c4. Otherwise it reads a byte at offset 0x114 of a buffer reached through the word at offset 0x198, and stores updated words back at 0x198 and 0x194. The index comparison and the -1 returns look like a queue pop, which matches the name, but the facts do not settle the queue layout.

- `elsc_nvram_magic` in `6.5.22/unix.IP27` at 0xc00000000009acd0
  **Inferred.** Calls `elsc_nvram_read` at 0xc00000000009a530 for NVRAM offset 0x700 into a stack buffer at sp-16, with a size argument of 1. A negative read result takes the branch at 0xc00000000009acec to a return of -0x7c, and otherwise the routine returns 0 or the read result. The name suggests a magic-word check, but the facts show only the read and the error code, not a comparison against a stored value.

- `elsc_debug_set` in `6.5.22/unix.IP27` at 0xc00000000009ad14
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 first and stops on a negative result, then writes two values through `elsc_nvram_write` at 0xc00000000009a434 at NVRAM offsets 0x705 and 0x706, each from a stack buffer (sp-17 and sp-9). Each write's result is checked for a negative value at 0xc00000000009ad48 and 0xc00000000009ad60. The name suggests a debug setting, and the pair of offsets fits, but the facts do not name the fields.

- `elsc_debug_get` in `6.5.22/unix.IP27` at 0xc00000000009ad8c
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0, then reads NVRAM offsets 0x705 and 0x706 through `elsc_nvram_read` at 0xc00000000009a530 into the caller's a1 and a2 arguments. Negative results on either path go to the shared return at 0xc00000000009ade8, which yields an unknown value or the second read's result. This is the read counterpart of `elsc_debug_set`, which writes the same two offsets.

- `elsc_module_set` in `6.5.22/unix.IP27` at 0xc00000000009ae04
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 and, on success, writes one value at NVRAM offset 0x708 with `elsc_nvram_write` at 0xc00000000009a434, from a stack buffer at sp-32. A negative write result is returned through 0xc00000000009ae40. The name suggests a module-id setting, which the offset 0x708 is consistent with alongside the neighbouring setters at 0x70a to 0x70d, but the facts do not name the field.

- `elsc_partition_set` in `6.5.22/unix.IP27` at 0xc00000000009ae98
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 and then writes one value at NVRAM offset 0x70a with `elsc_nvram_write` at 0xc00000000009a434, from a stack buffer at sp-32. The result is checked for a negative value and returned through 0xc00000000009aed4. The field is not named by the facts, and the setter's name is the only clue to its meaning.

- `elsc_domain_set` in `6.5.22/unix.IP27` at 0xc00000000009af2c
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 and then writes one value at NVRAM offset 0x70b with `elsc_nvram_write` at 0xc00000000009a434, from a stack buffer at sp-32. A negative write result goes to the return at 0xc00000000009af68. The same pattern as its neighbours at 0xc00000000009ae98 and 0xc00000000009afc0 suggests a family of setters, but the facts show only the offset.

- `elsc_cluster_set` in `6.5.22/unix.IP27` at 0xc00000000009afc0
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 and then writes one value at NVRAM offset 0x70c with `elsc_nvram_write` at 0xc00000000009a434, from a stack buffer at sp-32. A negative write result goes to the return at 0xc00000000009affc. It has the same shape as the other setters at offsets 0x70a, 0x70b and 0x70d, and the facts show nothing else about the field.

- `elsc_cell_set` in `6.5.22/unix.IP27` at 0xc00000000009b054
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 and then writes one value at NVRAM offset 0x70d with `elsc_nvram_write` at 0xc00000000009a434, from a stack buffer at sp-32. A negative write result goes to the return at 0xc00000000009b090. Offset 0x70d is the one that `elsc_cell_get` reads back, which ties the pair together, but the facts do not name the field.

- `elsc_cell_get` in `6.5.22/unix.IP27` at 0xc00000000009b0a0
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 and then reads NVRAM offset 0x70d into a stack buffer at sp-32 with `elsc_nvram_read` at 0xc00000000009a530. A negative read result goes to the return at 0xc00000000009b0d8, which yields an unknown value or the read result. It is the read side of `elsc_cell_set`.

- `elsc_bist_set` in `6.5.22/unix.IP27` at 0xc00000000009b0e8
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0, then writes one value at NVRAM offset 0x709 with `elsc_nvram_write` at 0xc00000000009a434, from a stack buffer at sp-1. A negative write result goes to the return at 0xc00000000009b124. The name suggests a built-in self-test setting, and the offset is read back by `elsc_bist_get`, but the facts do not show the field.

- `elsc_bist_get` in `6.5.22/unix.IP27` at 0xc00000000009b134
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0 and then reads NVRAM offset 0x709 into a stack buffer at sp-32 with `elsc_nvram_read` at 0xc00000000009a530. Negative results on either path go to the shared return at 0xc00000000009b16c, which yields an unknown value. It is the read side of `elsc_bist_set`.

- `elsc_password_set` in `6.5.22/unix.IP27` at 0xc00000000009b47c
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0, then runs a byte-copy loop with `lbu` at 0xc00000000009b4ac and `sb` stores at 0xc00000000009b4c0 and 0xc00000000009b4fc, and then writes a 4-byte block at NVRAM offset 0x701 through `elsc_nvram_write` at 0xc00000000009a434. The write passes a size of 4 and a stack buffer of 32 bytes at sp-32 (a6 is 0x20). The name suggests a password store, and the copy loop fits building that buffer, but the facts show neither the password format nor the copy's source.

- `elsc_password_get` in `6.5.22/unix.IP27` at 0xc00000000009b500
  **Inferred.** Calls `elsc_nvram_magic` at 0xc00000000009acd0, then reads a 4-byte block from NVRAM offset 0x701 into the caller's a1 buffer with `elsc_nvram_read` at 0xc00000000009a530, and stores one byte at a1 plus 4 at 0xc00000000009b534. It returns the magic result or the read result. This is the read side of `elsc_password_set`, which writes the same offset.

- `nic_access` in `6.5.22/unix.IP27` at 0xc00000000009b86c
  **Inferred.** Formats a command string into the structure at a0+440 with `sprintf` at 0xc0000000001f8e8c and the format at 0xc0000000003c8bc0, sends it through `elsc_command` at 0xc00000000009a61c, and on success parses the reply with `elsc_parse` at 0xc00000000009a7ac. It then converts the parsed text with `strtoull` at 0xc000000000146adc in base 16. The name suggests a NIC register access through the system controller, which the hex conversion fits, but the facts do not show the command text.

- `_elsc_hbt` in `6.5.22/unix.IP27` at 0xc00000000009b980
  **Inferred.** Loops from 0xc00000000009b9b8 back through 0xc00000000009ba08 and 0xc00000000009ba28. Each pass formats a command string into a0+440 with `sprintf` at 0xc0000000001f8e8c, sends it through `elsc_command` at 0xc00000000009a61c, and parses the reply with `elsc_parse` at 0xc00000000009a7ac. The formats are at 0xc0000000003c8bd0, 0xc0000000003c8be0 and 0xc0000000003c8be8. The name suggests a heartbeat exchange with the system controller, but the facts show the loop and the strings, not the protocol.

- `_elscuart_probe` in `6.5.22/unix.IP27` at 0xc00000000009ba30
  **Inferred.** Calls `elsc_version` at 0xc00000000009ac64 with a stack buffer at sp-112 and returns the call's value unchanged. The routine is a single call with no check of the result. The name suggests a probe of the UART behind the system controller, and the call fits that role, but the facts do not show what the probe tests.

- `_elscuart_init` in `6.5.22/unix.IP27` at 0xc00000000009ba54
  **Inferred.** Clears three blocks of 0x88 (136) bytes with `blkclr` at 0xc00000000001fadc, at offsets 4, 140 and 276 of its argument. The blocks are 136 bytes apart, which suggests three per-port buffers, but the facts show nothing of their contents. The return is the last `blkclr` result.

- `_elscuart_poll` in `6.5.22/unix.IP27` at 0xc00000000009ba98
  **Inferred.** Compares the words at a0+132 and a0+136 at 0xc00000000009bab0, and calls `elsc_process` at 0xc00000000009aa90 on the equal path. Otherwise it compares them again at 0xc00000000009bad8 and returns 1 or 0. The two words look like the head and tail of a receive ring, which fits a UART receive poll, but the facts do not show which word is written where.

- `_elscuart_readc` in `6.5.22/unix.IP27` at 0xc00000000009baf0
  **Inferred.** Compares the words at a0+132 and a0+136 at 0xc00000000009bb04. When they are equal it returns -1 at 0xc00000000009bb0c, and otherwise it reads a byte at a0+136 plus a0+4 at 0xc00000000009bb1c, stores the advanced word back at a0+136 at 0xc00000000009bb28, and returns that byte. The head and tail reading matches `_elscuart_poll`. The buffer size is an unknown sub-fact here and does not change the read path described above.

- `_elscuart_getc` in `6.5.22/unix.IP27` at 0xc00000000009bb34
  **Inferred.** Works on the same head and tail words as `_elscuart_readc`, and when they are equal it calls `elsc_process` at 0xc00000000009aa90 with a3 set to the word at a0+136. It retries while `elsc_process` returns a non-negative value or -0xa (the compare at 0xc00000000009bb78), and otherwise it returns -1 or the byte at ([a0+136] + a0) + 4, as `_elscuart_readc` does. The blocking-read reading is inferred from that loop shape, and the facts do not show what -0xa means.
