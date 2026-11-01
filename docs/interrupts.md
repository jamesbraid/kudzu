# Hub interrupt routing as the kernel programs it

Each IP27 hub has two 64-bit pending registers and, per processor slice, two mask registers. The 6.5.30 kernel numbers hub interrupt levels 0 to 127: levels 0 to 63 are bits of the first pending register and levels 64 to 127 bits of the second. This page states how the kernel reserves levels, connects handlers, masks levels and clears what is pending, and which levels it reserves at boot. IP35 is not yet covered here.

Every fact is graded as in [klconfig.md](klconfig.md). Register offsets are from the hub's register window: the kernel writes a node's registers at `0x9200000001800000` plus the NASID shifted left 32, and the calling processor's own hub at `0x9200000001000000`. Public Linux names the same offsets in `arch/mips/include/asm/sn/sn0/hubpi.h`; those names are used as labels.

| Offset | Linux name | Kernel use | Grade |
|---|---|---|---|
| `0x90` | `PI_INT_PEND_MOD` | a level number written here with bit 8 clear clears that pending bit | established for the writes; the effect is from Linux |
| `0x98` | `PI_INT_PEND0` | pending levels 0 to 63; read back after a clear | established |
| `0xa0` | `PI_INT_PEND1` | pending levels 64 to 127 | established |
| `0xa8`, `0xb0` | `PI_INT_MASK0_A`, `PI_INT_MASK1_A` | masks of slice 0 for the two pending registers | established |
| `0xb8`, `0xc0` | `PI_INT_MASK0_B`, `PI_INT_MASK1_B` | masks of slice 1; the kernel adds 16 times `p_slice` to the slice-0 offset | established |

## Software state

The kernel keeps its own copy of each processor's two masks in `p_intmasks` (+1432 of `pda_s`): `intpend0_masks` at +1432, `intpend1_masks` at +1440, and pointers to the two vector blocks, `dispatch0` at +1448 and `dispatch1` at +1456. A vector block (`intr_vecblk_s`, 7720 bytes, [structs/intr_vecblk_s.md](structs/intr_vecblk_s.md)) has 64 vectors of 72 bytes from +0 and 64 information records of 48 bytes from +4608. The IP27 node keeps two vector blocks per subnode in `snpda` (+3288 of `nodepda_s`): one at +0 and one at +7720 of the subnode record. In each information record the word `ii_flags` (+44) carries bit 0 for a reserved level and bit 1 for a connected one; in each vector `thd_flags` (+24, the start of `iv_tinfo`) carries `0x100` for a level whose handler runs as a thread and `0x200` once that thread is set up.

## Routines

- `intr_init_vecblk` (`6.5.30/unix.IP27` `0xc000000000037f58`, 436 bytes)
  takes a `nodepda_s` pointer (a0), a compact node (a1) and a subnode (a2), and fills the two vector
  blocks at +3288 and +11008 of that node data plus 15456 times the subnode. For each of the 64
  vectors it stores `intr_stray` (address from gp-20024) in `iv_func`, 0 in `iv_prefunc`, the level
  number (0 to 63 in the first block, 64 to 127 in the second) in `iv_arg`, 0 in `thd_flags` and -1 in
  `iv_mustruncpu` (+64). For each information record it clears `ii_owner_dev` (+16) and `ii_flags`
  (+44) and copies the 7 bytes of "Unused" with its terminator into `ii_name` (+20). It then stores
  `splvme` (first block) or `splerr` (second) as `vector_spl` (+7688), calls `init_spinlock` for
  `vector_lock` (+7680) with "ivecb" and the number (a1 shifted left 8) + 2 times a2 + the block
  index, and clears `vector_count` (+7700), both words of `cpu_count` (+7704) and `vector_state`
  (+7696). The `cpu_count` clear is an unrolled two-pass loop whose further-pass branches at
  0xc000000000038098 and 0xc0000000000380ac are never taken. Established.
- `intr_stray` (`6.5.30/unix.IP27` `0xc000000000037f30`, 40 bytes)
  is the handler every unconnected vector holds: it calls `cmn_err` with level 4, "Stray Interrupt -
  level %d to cpu %d", its argument (the level) and the halfword at absolute `0xffffffffffffc420`
  (the running processor's `p_cpuid`, +1056 of `pda_s`). Established.
- `sub_c000000000038390` (`6.5.30/unix.IP27` `0xc000000000038390`, 92 bytes)
  is unsymbolled (the DWARF names a routine `intr_get_ptrs` without an address). It takes a processor
  (a0) and a level (a1) and fills four result pointers: for a level below 64, the address of
  `intpend0_masks` (+1432 of that processor's `pda_s`, found through `pdaindr`) into a3, `dispatch0`
  (+1448) into a4, block 0 into a5 and the level into a2; for 64 or more, `intpend1_masks` (+1440),
  `dispatch1` (+1456), block 1 and the level minus 64. Established.
- `sub_c00000000003810c` (`6.5.30/unix.IP27` `0xc00000000003810c`, 564 bytes)
  is unsymbolled. It reserves (a3 nonzero) or releases (a3 zero) a level of processor a0. It returns
  -1 when the processor has no `pda_s`. A level a1 below 64 selects `dispatch0` unless flag `0x4` is
  set in a2; otherwise it selects `dispatch1` and subtracts 64, so with flag `0x4` any level below 64
  becomes a request for any free level of the second block. It calls `mutex_spinlock_spl` with the
  block's `vector_lock` and `vector_spl`. For a bit of 0 to 63 it fails (-1) when bit 0 of `ii_flags`
  already equals a3 (reserving a reserved level or releasing a free one); for a negative bit it takes
  the first record whose bit 0 is clear and fails when all 64 are taken. To reserve it stores a4 in
  `ii_owner_dev`, ORs a2 and 1 into `ii_flags`, calls `strncpy` to copy at most 23 bytes of the name
  in a5 into `ii_name` (calling `strlen` first when a5 is not null) and clears the last byte, adds one
  to `vector_count`, and sets `0x100` in `thd_flags` when a2 has `0x8` or clears it otherwise. To
  release it clears the first byte of `ii_name`, `ii_owner_dev` and `ii_flags` and subtracts one from
  `vector_count`. It calls `mutex_spinunlock` and returns the level (bit plus 64 for the second
  block), or -1. Established.
- `intr_reserve_level` (`6.5.30/unix.IP27` `0xc000000000038340`, 40 bytes)
  calls `sub_c00000000003810c` with its own a0, a1 and a2, 1 as a3, its a3 as the owner (a4) and its
  a4 as the name (a5), and returns the result. Established.
- `intr_unreserve_level` (`6.5.30/unix.IP27` `0xc000000000038368`, 40 bytes)
  calls `sub_c00000000003810c` with its a0 and a1 and zero for every other argument, releasing the
  level. Established.
- `intr_connect_level` (`6.5.30/unix.IP27` `0xc0000000000383ec`, 668 bytes)
  takes a processor (a0), a level (a1), a thread priority (a2), a handler (a3), an argument (a4) and
  a pre-handler (a5). It calls `sub_c000000000038390` and `mutex_spinlock_spl` on the block. It
  returns -1 when the level is already connected (bit 1 of `ii_flags`) or not reserved (bit 0 clear).
  Otherwise it stores the handler in `iv_func`, the pre-handler in `iv_prefunc`, the argument in
  `iv_arg` and a2 as the halfword `thd_pri` (+56 of the vector), and sets bit 1 of `ii_flags`. On the
  processor's node (NASID from `compact_to_nasid_node` at `p_nodeid`, +1058 of `pda_s`) it writes the
  level (bit plus 64 times the block) to hub offset `0x90` and reads `0x98` back, sets the bit in the
  software mask, and adds one to `cpu_count` for the processor's `p_slice`. When `thd_flags` has
  `0x100` it sets `0x200` there with a load-linked/store-conditional loop (branch-likely retry at
  0xc000000000038608), stores the processor in `iv_mustruncpu`, and when `ithreads_enabled` (+7712)
  is set calls the unsymbolled routine at `0xc00000000003a4e8` with the vector, `p_nodeid`, the bit
  and a2. Finally it writes the whole software mask to the slice's mask register (`0xa8` or `0xb0`,
  plus 16 for slice 1), unlocks and returns 0. Established.
- `intr_disconnect_level` (`6.5.30/unix.IP27` `0xc000000000038688`, 428 bytes)
  takes a processor and a level, calls `sub_c000000000038390` and locks the block. It returns -1
  unless both bits 0 and 1 of `ii_flags` are set (`& 3` equals 3). Otherwise it clears `iv_func`,
  `iv_prefunc` and `iv_arg`, clears bit 1 of `ii_flags` (mask -3), clears the bit in the software mask,
  subtracts one from `cpu_count` for the processor's slice, writes the software mask to the slice's
  mask register (`0xa8` or `0xb0`, plus 16 times `p_slice`), unlocks and returns 0. The level stays
  reserved. Established.
- `do_intr_block_bit` (`6.5.30/unix.IP27` `0xc000000000038834`, 364 bytes)
  takes a processor (a0), a level (a1) and a2, 1 to block and 0 to unblock. It calls
  `sub_c000000000038390` and `mutex_spinlock_spl`, clears (a2 nonzero) or sets the level's bit in the
  software mask, and writes the mask to the processor's mask register on its node: `0xa8` or `0xb0`
  for slice 0 and `0xb8` or `0xc0` for any other slice. When blocking it then reads the register back
  until it equals the software mask, so the bit is known to be off before it returns; unblocking does
  not read back. It calls `mutex_spinunlock`. Established.
- `intr_block_bit` (`6.5.30/unix.IP27` `0xc0000000000389a0`, 28 bytes)
  calls `do_intr_block_bit` with its a0 and a1 and 1 in a2. Established.
- `intr_unblock_bit` (`6.5.30/unix.IP27` `0xc0000000000389bc`, 28 bytes)
  calls `do_intr_block_bit` with its a0 and a1 and 0 in a2. Established.
- `intr_clear_all` (`6.5.30/unix.IP27` `0xc000000000039ea0`, 120 bytes)
  takes a NASID and writes 0 to that node's four mask registers in the order `0xa8`, `0xb8`, `0xb0`,
  `0xc0`. It then calls `sub_c000000000039e28` with the node's `0x98` register and first level 0, and
  again with `0xa0` and first level 64. Established.
- `sub_c000000000039e28` (`6.5.30/unix.IP27` `0xc000000000039e28`, 120 bytes)
  is unsymbolled. It reads the pending register at a1 once and, for each set bit i from 0 to 63,
  writes level a2 + i to offset `0x90` of the calling processor's own hub (`0x9200000001000090`) and
  reads that hub's `0x98` back. It reads the remote register but clears through the local hub, so it
  clears the right bits only when the NASID passed to `intr_clear_all` is the caller's own node.
  Established.
- `intr_reserve_hardwired` (`6.5.30/unix.IP27` `0xc000000000039d04`, 292 bytes)
  takes a compact node, calls `cnodetocpu` for its first enabled processor and returns when there is
  none. For that processor it walks `hardwired_intr` (`0xc0000000014165f8`, 128 bytes into
  `cause_intr_tbl`, address from gp-19984), entries of 16 bytes ended by a level byte of -1, and calls
  `intr_reserve_level` with the processor, the entry's level byte (+0), its flags word (+4), no owner
  and its name (+8). A result other than the level calls `cmn_err` with level 0 and
  "intr_reserve_hardwired: Can't reserve level %d." It then steps through the following processor
  numbers while they are below `maxcpus` (gp+2352), enabled (`cpu_enabled`) and on the same node
  (`p_nodeid`), but reserves nothing for them: the flag that would restart the table walk is never
  cleared. So the hardwired levels are reserved on the node's first enabled processor only.
  Established.

## Levels reserved at boot

The `hardwired_intr` table in `6.5.30/unix.IP27` (read from `.data` at `0xc0000000014165f8`). Flag `0x4` selects the second block; flag `0x8` marks a threaded handler.

| Level | Flags | Name in the table |
|---:|---|---|
| 0 | 0 | "Reserved" |
| 1 | 0 | "Gfx A" |
| 2 | 0 | "Gfx B" |
| 3 | `0x8` | "Migration" |
| 4 | 0 | "Hub I2C" |
| 5 | 0 | "Crosscall A" |
| 6 | 0 | "Crosscall B" |
| 9 | `0x8` | "MSC Message" |
| 7 | 0 | "CPU Action A" |
| 8 | 0 | "CPU Action B" |
| 121 | `0x4` | "IO Error" |
| 122 | `0x4` | "Clock Error" |
| 123 | `0x4` | "Correctable Error A" |
| 124 | `0x4` | "Correctable Error B" |
| 125 | `0x4` | "MD Correct. Error" |
| 126 | `0x4` | "NI Error" |
| 103 | `0x4` | "Remote NI Error" |
| 104 | `0x4` | "Remote NI Error" |
| 127 | `0x4` | "MSC Panic" |
| 105 | `0x4` | "LLP Pfail WAR" |
| 106 | `0x4` | "LLP Pfail WAR" |

The second-block levels agree with the hardware bit list in Linux 2.4's `include/asm-mips64/sn/intr.h` (IO error 57, clock error 58, correctable errors 59 and 60, memory correctable error 61, NI error 62, MSC panic 63 of `INT_PEND1`), and so do levels 0 to 3 and the crosscalls at 5 and 6 (Linux `CC_PEND_A` and `CC_PEND_B`). Linux names level 4 `UART_INTR` where the IRIX table says "Hub I2C". The software-chosen levels differ: IRIX 6.5.30 puts the processor actions at 7 and 8 and the MSC message at 9, where Linux uses 10, 11 and 13. Established for the table.
