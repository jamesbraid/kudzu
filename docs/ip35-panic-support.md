# Panic and assertion support in the IP35 kernel

Routines of the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF) that run when the kernel stops on an internal error.

- `assfail` (`6.5.30/unix.IP35`, address 0xc0000000000619f4, 1084 bytes): the assertion-failure entry the compiled
  `ASSERT` checks call; it keeps a one-time snapshot of the processor registers and then jumps to `_assfail` (address
  0xc0000000002af970) without returning to its caller. It stores `v0` into the doubleword at 0xc0000000016b7360 on
  every entry. It then loads the word at 0xc00000000163d384 and, when that is not -1 (an earlier assertion already
  took the snapshot), jumps straight to `_assfail` with `a0` to `a2` untouched. When the word is -1 it stores the
  sign-extended halfword at absolute address -15320 (0xffffffffffffc428, in the per-processor page) into that word,
  and copies the registers into the save area that starts at 0xc0000000016b7358, in this order: `gp` (+216), the
  Status register `mfc0 $12` (+248), `sp` (+224), `v1` (+16),
  `a3` to `t3` (+48 to +112, eight doublewords), `s0` to
  `s7` (+120 to +176), `t8` (+184), `t9` (+192),
  `k0` (+200), `k1` (+208), `s8` (+232),
  `ra` (+240), the low multiply register (+256), the high multiply register (+264), Cause `mfc0 $13` (+280), the
  bad-address register `dmfc0 $8` (+272), Status again (+248), EPC `dmfc0 $14` (+288 and again +352). `a0`,
  `a1`, `a2` and `at` are not saved because they carry the assertion text and are needed intact by
  `_assfail`. The routine uses no stack frame and makes one call, the final jump at 0xc000000000061e28. What the
  halfword at -15320 holds (it is the value later reported as the failing processor) is inferred from its use as a
  once-only marker. Inferred.

