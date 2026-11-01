# Cache error handling in the IP35 kernel

How the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF) classifies a processor cache error exception. The routines come from the kernel's cache error source file, which the debug information names; addresses are kernel virtual addresses. The messages a handler can return are in one string pool that starts at 0xc0000000015294f0; "msg +N" below is that base plus N. "info +N" is byte N of the per-processor cache error record that `ecc_handler` receives as its second argument.

- `ecc_handler` (`6.5.30/unix.IP35`, address 0xc00000000007be10, 2016 bytes, 192-byte frame): decides from the saved
  cache error register whether a cache error exception was repaired. It returns 0 when it was, or the address of a
  message string when the error cannot be recovered (the caller panics with it). `a0` is the saved exception frame and
  `a1` the cache error record: the word at info +24 is the register value (loaded unsigned), the doublewords at +8 and
  +16 receive tag words, and the byte at +38 is a state flag. If bit 29 of the register is set it calls `cer_to_hex`
  (address 0xc00000000007bd6c) with the text at msg +244 and returns msg +240 ("CER=" followed by "Multiple cache
  errors in same unit detected") at once, logging nothing else and skipping the steps after the dispatch. Otherwise it
  appends to a ring of 16 entries of 40 bytes starting at info +56 (the word at +696 is the next slot, the word at
  +700 the oldest; if the next slot would equal the oldest the ring is full and nothing is stored): the register at
  entry +0 (info +56), the doubleword at info +0 into entry +24 (info +80), and a doubleword read from the uncached
  address 0x9200000001030100 into entry +32 (info +88); the next-slot word advances modulo 16. It calls
  `eccf_recoverable_threshold` (address 0xc00000000007bc5c) with the frame, the record and the register; call its
  result T, and the routine keeps a flag that is 1 when T is 0. It zeroes the tag doublewords at info +8 and +16 and
  dispatches on bits 31:30 of the register. The unnamed helper at address 0xc00000000007b678, called below with a
  small operation number as its first argument and a cache address as its second, is the routine that touches the
  cache arrays (it has no name in the symbol table; the operation numbers 0, 1, 3,
  4, 5 and 7 are used as follows). The cache index address used throughout is bits 13:3 of the register plus
  0xa800000000000000 for the first two classes, and bits 22:6 plus the same base for the interface class. Bits 31:30 =
  0 (the messages say primary instruction cache): adds one to the word at info +40. For array 0 then array 1, when the
  register has any of the bits 0x5500000 (array 0) or 0xaa00000 (array 1), it calls the helper with operation 4 and
  the next index address, and stores the result at info +8 or +16. It then calls the helper with operation 0 on the
  index and on the index with bit 0 flipped, and `ecc_force_cache_line_invalid` (address 0xc00000000007bd18) for way 0
  and way 1 with third argument 0. When T was 0 it calls `failsoft_panic_check` (address 0xc00000000024cb8c) with the
  frame; a nonzero result there makes the return value msg +840 ("too many primary instruction cache error
  exceptions"), otherwise (or when T was not 0) the return value is 0. The save step below runs. Bits 31:30 = 1
  (primary data cache): adds one to the word at info +44. With none of the 0x5500000 or 0xaa00000 bits set the answer
  is msg +600 ("unrecoverable - dcache tag on inconsistent block") when bit 28 of the register is set and msg +656
  ("unrecoverable dcache error, neither way") when it is clear. With some set, for each array in turn whose bits are
  set it reads the tag with helper operation 5 into info +8 or +16 and checks, in order: a register bit in 0x300000
  together with bits 7:6 of the tag both set gives msg +696 ("unrecoverable - dcache tag mod"); a register bit in
  0x3c00000 together with bits 63:61 of the tag equal to 2 gives msg +728 ("unrecoverable - dcache tag"); a register
  bit in 0xc000000 together with bits 7:6 equal to 3 and bits 63:61 equal to 2 gives msg +760 ("unrecoverable - dcache
  data"); otherwise it repairs the line with helper operation 1 and goes on to the next array; a hit ends the loop.
  When the loop ends with no hit the answer is 0. Then, in both cases, the answer is replaced by msg +600 when
  register bit 28 is set, else by msg +792 ("too many primary data cache error exceptions") when T was 0, else it is
  left as it stands. Bits 31:30 = 2 (secondary cache): adds one to the word at info +48 and returns msg +560
  ("unrecoverable secondary cache error"). Bits 31:30 = 3 (system interface): adds one to the word at info +52. A
  register bit in 0x3800000 returns msg +344 ("unrecoverable SIE") at once. Otherwise, with bits 26 and 27 both clear,
  it returns msg +368 ("unrecoverable SIE, neither way"). With either set, for bit 26 then bit 27 it reads the
  interface error word with helper operation 7 into info +8 or +16 and takes bits 11:10 of it; when they are nonzero
  it calls `memerror_handle_uce` (address 0xc00000000006b4d8) with the register, the word, the frame and the record. A
  nonzero result there makes it call `clear_sline` (address 0xc0000000000644d4) eight times at 16-byte steps from the
  index and then helper operation 3; a zero result with bits 11:10 equal to 3 ends the loop with msg +400
  ("unrecoverable SIE - dirty scache"); any other zero result uses helper operation 3 and
  `ecc_force_cache_line_invalid` with third argument 3. A loop that ends with no stop gives 0. In both cases, when T
  was 0, the answer is replaced after reporting through `cmn_err_tag` (address 0xc0000000002ad094; arguments 2966, 4,
  msg +440 "Unrecoverable Interface error: Suspect memory address 0x%x At %s Bank %d", the index address, a node name,
  and bits 28:26 of the error word) by msg +520 ("too many interface error exceptions."). The node name is msg +0 ("a
  remote partition") when the halfword at 0xc000000001646f00 + 2 x (index address bits 39:33) is -1, and otherwise a
  string pointer reached through the table whose address is the doubleword at absolute address -15360, indexed by that
  halfword, at offset 0x8c70. Any other value of the field is not reachable from a 2-bit field; the code for it
  returns msg +304 ("unrecoverable - unknown cache error"), skips `save_cache_error` but still does the ring and frame
  steps below. Afterward, when a ring slot was taken, the tag words at info +8 and +16 are copied into that slot
  (entry +8 and +16, info +64 and +72 plus 40 times the slot). The byte at absolute address -14536 is set to 1. When
  the return value is nonzero and the byte at info +38 is not already 2, it is set to 2 and the frame's first 45
  doublewords are copied to frame +1080 onward. Finally, for every class except that last one, `save_cache_error`
  (address 0xc000000000052a08) is called with the register and the two tag words. What the cache-op operations and the
  doubleword read from 0x9200000001030100 are: unknown here, labelled by use only. Established.

## Cache-error exception

- `ecc_exception` (`6.5.30/unix.IP35`, address 0xc00000000004a810, 432 bytes, no stack frame): the cache-error
  exception body: it saves the interrupted state, asks `ecc_handler` what happened, calls `ecc_panic` when the answer
  is nonzero and otherwise returns with `eret`. It takes no arguments and never returns normally to a caller; its
  one exit is the `eret` at 0xc00000000004a9bc. It does not use a stack to start with: the first instruction sets
  `k1` to the absolute address 1024 (`li k1,1024`), and that is the register-save area. It stores `s0` to `s7` at +120 to
  +176, `a0` to `a7` at +24 to +80, `gp` at +216, `sp` at +224 and `ra` at +240 (all doublewords, in that order), then
  builds `gp` as 0xc000000001643770 and calls `tfi_save` (0xc000000000061980). After the call it stores coprocessor-0
  register 12 at +248, register 13 at +280, register 8 at +272 and register 14 at +288 and again at +352 (in the architecture's
  numbering these are the status, cause, faulting-address and return-address registers). `s0` is the area and `s1` the area plus 360. When the byte at `s1` +38 is
  zero it is set to 1. It stores register 30 (ErrorEPC) as a doubleword at `s1` +0 and registers 29, 28 and 27 as words at
  +32, +28 and +24. The stack pointer is then the doubleword at absolute address 4064, or 4048 when that is zero.
  `ecc_handler(s0, s1)` (0xc00000000007be10) runs; a zero result jumps to the restore at 0xc00000000004a93c. A nonzero
  result loads `sp` from the doubleword at absolute address 0xffffffffffffc448, rebuilds `gp`, clears bits 3 and 4 of
  Status (AND with -25, the `li` at 0xc00000000004a920) and calls `ecc_panic(result, s0, s1)` (0xc00000000007c5f0), which is
  not expected to return; if it does the code falls into the restore. The restore writes the words at `s1` +32 and +28
  back to registers 29 and 28, calls `tfi_restore` (0xc0000000000619b8) with `k1` = `s0` set in its delay slot, writes the
  doubleword at `s1` +0 back to register 30, reloads `a0` to `a7`, `s0` to `s7`, `ra`, `at` (from area +0), `sp` and `gp`,
  clears the byte at `s1` +38 (through `k0`), loads `k0` from area +200 and `k1` from +208, and does `eret`. Established.
