# Instruction readings from hostfacts

These entries record instruction facts rendered from the routines below. They describe software behavior, not hardware behavior, and each locator names an address in the identified binary.

- `start` in `6.5.22/unix.IP27` at 0xc00000000001a520 (n64).
  Evidence grade: inferred.
  Exact binary: `6.5.22/unix.IP27`.
  Range: 0xc00000000001a520-0xc00000000001a740 (544 bytes).
  It occupies 544 bytes.
  The `ld` at 0xc00000000001a550 loads 8 bytes from abs 0x9200000001600000.
  The `jal` at 0xc00000000001a568 calls mapped_kernel_setup_tlb [0xc000000000042fa8] with a0=?, a1=?, a2=a2 at entry (arg2), a3=a3 at entry (arg3).
  Calls start+112 [0xc00000000001a590] via register-constant with a0=?, a1=?, a2=?, a3=? (`jr` at 0xc00000000001a588).
  The instructions from 0xc00000000001a590 to 0xc00000000001a73c are not reached by any path from the entry, and their branches are not listed.

Skipped no routines.
