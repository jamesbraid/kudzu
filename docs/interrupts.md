# Hub interrupt routing as the kernel programs it

Each IP27 hub has two 64-bit pending registers and, per processor slice, two mask registers. The 6.5.30 kernel numbers hub interrupt levels 0 to 127: levels 0 to 63 are bits of the first pending register and levels 64 to 127 bits of the second. This page states how the kernel reserves levels, connects handlers, masks levels and clears what is pending, and which levels it reserves at boot, first for IP27 and then for the IP35 differences.

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

## Hub interrupt handles

Drivers reach the routines above through the crosstalk interrupt calls, which the IP27 hub provider implements with a 48-byte handle allocated on the target processor's node. No shipped DWARF defines the handle; offsets are from the instructions:

| Offset | Width | Contents | Grade |
|---|---|---|---|
| +0 | word | the device vertex (set to -1 on free) | established |
| +4 | byte | the device's crosstalk master widget, from `xwidget_info_masterid_get` | established |
| +5 | byte | the bit within its block | established |
| +8 | doubleword | the NASID shifted left 38, plus `0x800001800090` | established |
| +16 | doubleword | the set-function argument given to connect | established |
| +24 | doubleword | the set function given to connect, called on connect and disconnect | established |
| +32 | word | the software level, used as the thread priority on connect | established |
| +36 | halfword | the target processor | established |
| +40 | word | the bit within its block, as returned by the reservation | established |
| +44 | word | flags: bit 0 allocated, bit 2 connected | established |

The doubleword at +8 is the node's `PI_INT_PEND_MOD` (`0x90` in the hub window) at the crosstalk address a widget writes to raise the level; that reading is inferred from the offset and the Linux name, since the kernel only stores it here.

- `sub_c00000000003bd0c` (`6.5.30/unix.IP27` `0xc00000000003bd0c`, 688 bytes)
  is unsymbolled; `hub_intr_alloc` and `hub_intr_alloc_nothd` call it with a device vertex (a0), a
  device descriptor (a1), an owner vertex (a2) and a no-thread flag (a3). With no descriptor it calls
  `device_desc_default_get`; when that also returns nothing it uses no name, `default_intr_pri` (gp-30460)
  as the level, and reservation flags of 8 (threaded) when a3 is 0, or the word at sp+0, which the
  routine never writes, when a3 is nonzero. With a descriptor it calls `device_desc_intr_name_get`
  and `device_desc_intr_swlevel_get`, and takes flags 4 (second block) when bit 0 of the descriptor
  word at +24 is set, else 0 when a3 is nonzero or the descriptor word has bit 2, else 8. It then calls
  `hwgraph_edge_get` for the device's "pci" edge. When that succeeds, the edge's `hwgraph_fastinfo_get`
  record has `0xc0022` at +120 and a nonzero pointer at +2088, `bridge_rev_b_data_check_disable`
  (gp-29420) is 0, a descriptor was given and its name equals "Qlogic SCSI" (`strcmp`), it targets the
  processor in the halfword at +36 of the record that pointer names and calls `intr_reserve_level`
  for it with `allocate_my_bit` (gp-32516), calling `cmn_err` level 4 with "Could not reserve an
  interrupt bit for cpu  %d and dev %v\n" when that fails. Otherwise it calls `intr_heuristic` with
  the device, the descriptor, `allocate_my_bit`, the flags, the owner, the name and a result pointer.
  A processor of -1 calls `cmn_err` level 4 with "%v hub_intr_alloc could not allocate interrupt\n"
  and returns 0. On success it calls `kmem_alloc_node` for 48 bytes (a1 1) on the processor's node,
  calling `assfail` (line 788) when that returns null, fills the handle as in the table (flags 1),
  calls `xwidget_info_get` and, for a widget, `xwidget_info_masterid_get`, then calls
  `sub_c00000000003bcb8` and returns the handle. Established; that the "Qlogic SCSI" path keeps the
  SCSI interrupt on the processor of an earlier Bridge interrupt is inferred from the fields it reads.
- `sub_c00000000003bcb8` (`6.5.30/unix.IP27` `0xc00000000003bcb8`, 84 bytes)
  is unsymbolled. It calls `device_desc_intr_swlevel_set` on the descriptor (a0) with a1, formats
  "/hw/cpunum/%d" with the processor (a2) by `sprintf`, looks the path up with
  `hwgraph_path_to_dev` and calls `device_desc_intr_target_set` with the result, so the descriptor
  records the processor vertex the interrupt went to. Established.
- `hub_intr_alloc` (`6.5.30/unix.IP27` `0xc00000000003bfbc`, 28 bytes)
  calls `sub_c00000000003bd0c` with its three arguments and a3 of 0 (threaded handlers allowed).
  Established.
- `hub_intr_alloc_nothd` (`6.5.30/unix.IP27` `0xc00000000003bfd8`, 28 bytes)
  calls `sub_c00000000003bd0c` with its three arguments and a3 of 1. Established.
- `hub_intr_connect` (`6.5.30/unix.IP27` `0xc00000000003c06c`, 136 bytes)
  takes a handle (a0), a handler (a1), its argument (a2), a set function (a3) and its argument (a4),
  and calls `intr_connect_level` with the handle's processor (+36), bit (+40) and level (+32) as the
  priority, the handler, the argument and no pre-handler. A negative result is returned as is.
  Otherwise it stores a3 at +24 and a4 at +16, calls a3 with the handle when a3 is not null, sets bit
  2 of the flags word (+44) and returns 0. Established.
- `hub_intr_disconnect` (`6.5.30/unix.IP27` `0xc00000000003c0f4`, 100 bytes)
  calls the handle's set function (+24) with the handle and its bit when it is not null, then calls
  `intr_disconnect_level` with the handle's processor (+36) and bit (+40) and clears bit 2 of the
  flags word (+44, mask -5). It does not set a return value of its own: v0 holds -5, the mask, on
  return. Established.
- `hub_intr_free` (`6.5.30/unix.IP27` `0xc00000000003bff4`, 120 bytes)
  reads the handle's processor (+36), bit (+40) and flags (+44). When the flags word has bit 2
  (connected) it clears the byte at +5 and the doubleword at +8, stores -1 at +0 and calls
  `hub_intr_disconnect`. When the flags word has bit 0 it calls `kmem_free` for the 48 bytes. It
  then calls `intr_unreserve_level` with the saved processor and bit. Established.
- `hub_intr_cpuid_get` (`6.5.30/unix.IP27` `0xc00000000003c174`, 8 bytes)
  returns the handle's processor, the halfword at +36, loaded in the return's delay slot at
  0xc00000000003c178. Established.
- `hub_intr_cpu_get` (`6.5.30/unix.IP27` `0xc00000000003c158`, 28 bytes)
  returns the processor's vertex: `p_vertex`, the word at +1844 of the `pda_s` of the processor at +36
  of the handle (through `pdaindr`, address from gp-19880; the load at 0xc00000000003c170).
  Established.

## Choosing a target processor

When a driver allocates an interrupt without naming a processor, `intr_heuristic` picks one: the processor the device's descriptor names, else the least loaded processor of the device's own node, else of the nearest node in rising distance, else of the boot processor's node. Load is two per-slice counters in the node's `nodepda_s`. Five unsymbolled routines do the work in the linked kernel; the 6.5.30 IP27 `kernel.o` DWARF gives each a name and an address in that object, quoted in its entry.

- `sub_c0000000000389d8` (`6.5.30/unix.IP27` `0xc0000000000389d8`, 296 bytes)
  is `do_intr_cpu_choose` (at 0x1f858 in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` by its DWARF).
  It takes a compact node in a0 and a selector in a1, and returns the least loaded usable processor
  of the node, or -1. For slices 0 and 1 it calls `cnode_slice_to_cpuid`; it skips a slice that
  gives -1, one for which `cpu_enabled` returns 0, and one whose `pda_s` (through `pdaindr`, loaded
  from gp-20008) has bit `0x80` set in the word at +72 (the test at 0xc000000000038a70). It also
  skips every slice when a1 is neither -1 nor 0. The load is the sum of the words at +10992 and
  +18712 plus 4 × slice in node a0's `nodepda_s` (the loads at 0xc000000000038aa8 and
  0xc000000000038aac); the first processor whose load is below the best so far, starting from 1000,
  wins, so slice 0 wins a tie. Established.
- `sub_c000000000038b00` (`6.5.30/unix.IP27` `0xc000000000038b00`, 28 bytes)
  is `intr_cpu_choose_from_node` (kernel.o DWARF, 0x1f980): it calls `sub_c0000000000389d8` with its
  own a0 and a1 (the call at 0xc000000000038b08) and returns the result. Established.
- `sub_c000000000038b1c` (`6.5.30/unix.IP27` `0xc000000000038b1c`, 120 bytes)
  is `subnodevertex_to_subnode` (kernel.o DWARF, 0x1f99c). It calls `hwgraph_info_get_LBL` on vertex
  a0 for the label "_cpubus" (string block from gp-20040, plus 56) and returns -1 when the vertex has
  no such label. Otherwise it calls `master_node_get` on the vertex and returns -1 when that gives
  -1; else it stores the node as a halfword through a1 and the label's value as a word through a2
  (the stores at 0xc000000000038b80 and 0xc000000000038b7c) and returns 0. Established.
- `intr_target_from_desc` (`6.5.30/unix.IP27` `0xc000000000038c18`, 184 bytes)
  turns a device descriptor (a0) into a processor number. It calls `device_desc_intr_target_get`
  and returns -1 when the descriptor names no target vertex. A target that `cpuvertex_to_cpuid`
  maps to a processor gives that processor. Otherwise, when `sub_c000000000038b1c` finds the target
  is a CPU bus vertex, it returns `sub_c000000000038b00` for that node and subnode (the call at
  0xc000000000038cbc); else it calls `master_node_get` on the target, returns -1 for -1, and returns
  `sub_c000000000038b00` for that node with the caller's a1 as the selector (the call at
  0xc000000000038ca8). Results are sign-extended from 16 bits. Established.
- `sub_c000000000038cd0` (`6.5.30/unix.IP27` `0xc000000000038cd0`, 64 bytes)
  is `intr_cnode_seen` (kernel.o DWARF, 0x1fb50), the filter `intr_heuristic` hands to
  `physmem_select_neighbor_node`. It scans the signed halfwords of array a1 up to the count in the
  word at a2 (the load at 0xc000000000038cd0) and returns 0 when one equals a0; otherwise it returns
  a1 itself, a nonzero value, including when the count is 0 or less. Established.
- `sub_c000000000038d10` (`6.5.30/unix.IP27` `0xc000000000038d10`, 168 bytes)
  is `intr_bit_reserve_test` (kernel.o DWARF, 0x1fb90). It takes a processor in a0, a selector in
  a1, a node in a2, then the level, flags, owner, name and a result pointer in a3 to a7. When a2 is
  not -1 it replaces the processor with `sub_c000000000038b00` for that node and selector (the call
  at 0xc000000000038d48). It returns -1 when it has no processor. Otherwise it calls
  `sub_c00000000003810c` (the level reservation above) with the processor, level, flags, 1, owner
  and name (the call at 0xc000000000038d84), stores that result as a word through a7 (in the delay
  slot of the following branch, so on every path), and returns -1 when the result is negative and
  the processor otherwise. Established.
- `intr_heuristic` (`6.5.30/unix.IP27` `0xc000000000038db8`, 572 bytes)
  takes the device vertex in a0, a descriptor in a1, and the level, flags, owner, name and result
  pointer in a2 to a6, which it passes to `sub_c000000000038d10` at every step. It first calls
  `physmem_maxradius`. (1) With a descriptor it tries the processor `intr_target_from_desc` names
  (selector -1). (2) It tries the node `master_node_get` gives for the vertex (the call at
  0xc000000000038e74). On failure it calls `cmn_err` with 4, "!Cannot target interrupts to closest
  node: %v (0x%x)\n" and the owner twice. (3) For each radius from 1 to the maximum it calls
  `physmem_select_neighbor_node` with the caller's node (the halfword at absolute
  `0xffffffffffffc422`), the radius, a result slot, `sub_c000000000038cd0` as the filter, a
  list of halfword nodes at sp+8 and its count at sp+0. Each node found is added to the list and tried; after a failure
  the same radius is asked again, so every node at that distance is tried before the next. When none
  succeeds it calls `cmn_err` with 4 and "!Cannot target interrupts to any close node: %v (0x%x)\n".
  (4) It tries the node of the boot processor (`p_nodeid`, +1058 of the `pda_s` of
  `master_procid`). When that fails too it calls `cmn_err` with 4 and "Cannot target interrupts: %v
  (0x%x)\n" and returns -1. It returns the first processor that `sub_c000000000038d10` accepts.
  Established.

## IP35 differences

An IP35 node has up to four processors behind two processor interfaces. The kernel keeps the IP27 register offsets within each interface and adds `0x200000` for the second one: a processor whose `p_slice` (+1320 of the IP35 `pda_s`) has bit 1 set uses the second interface, and bit 0 of `p_slice` picks the A or B mask registers within it (16 bytes apart). Node registers are at `0x9200000001800000` (from gp+2288) plus the NASID shifted left 33. In the IP35 `pda_s`, `p_intmasks` is at +1440 (masks at +1440 and +1448, `dispatch0` at +1456, `dispatch1` at +1464), `p_cpuid` at +1064 and `p_nodeid` at +1066; `snpda` (+3256 of `nodepda_s`) holds two subnode records of 15456 bytes.

- `intr_init_vecblk` (`6.5.30/unix.IP35` `0xc00000000005ee74`, 436 bytes)
  has the IP27 body with the vector blocks at +3256 and +10976 of the node data plus 15456 times the
  subnode. It fills every vector with `intr_stray` (gp-22648), the level and -1, names each record
  "Unused", stores `splvme` or `splerr` as `vector_spl`, and calls `init_spinlock` with "ivecb". Its
  `cpu_count` clear has the same two branches that are never taken, at 0xc00000000005efb4 and
  0xc00000000005efc8. Established.
- `intr_stray` (`6.5.30/unix.IP35` `0xc00000000005ee4c`, 40 bytes)
  calls `cmn_err` with level 4, "Stray Interrupt - level %d to cpu %d", the level and the halfword at
  absolute `0xffffffffffffc428` (`p_cpuid` at +1064 of the IP35 `pda_s`). Established.
- `sub_c00000000005f2ac` (`6.5.30/unix.IP35` `0xc00000000005f2ac`, 92 bytes)
  is the IP27 pointer routine with the IP35 offsets: below level 64 it returns the address +1440,
  `dispatch0` (+1456), block 0 and the level; otherwise +1448, `dispatch1` (+1464), block 1 and the
  level minus 64. Established.
- `sub_c00000000005f028` (`6.5.30/unix.IP35` `0xc00000000005f028`, 564 bytes)
  is the IP27 reserve and release routine with `dispatch0` at +1456 and `dispatch1` at +1464 of the
  IP35 `pda_s`. It calls `mutex_spinlock_spl`, `strlen`, `strncpy` and `mutex_spinunlock` as on IP27
  and returns the level or -1 under the same conditions. Established.
- `intr_reserve_level` (`6.5.30/unix.IP35` `0xc00000000005f25c`, 40 bytes)
  calls `sub_c00000000005f028` with a3 of 1 and its a3 and a4 moved to a4 and a5. Established.
- `intr_unreserve_level` (`6.5.30/unix.IP35` `0xc00000000005f284`, 40 bytes)
  calls `sub_c00000000005f028` with its a0 and a1 and zero for the rest. Established.
- `intr_connect_level` (`6.5.30/unix.IP35` `0xc00000000005f308`, 792 bytes)
  makes the IP27 checks and vector stores, then picks the interface from bit 1 of `p_slice` (loaded
  at 0xc00000000005f464): it writes the level to offset `0x90` or `0x200090` and reads `0x98` or
  `0x200098` back, adds one to `cpu_count` indexed by bit 0 of `p_slice`, and writes the software mask
  to `0xa8` or `0xb0` (by block), plus 16 times bit 0 of `p_slice`, plus `0x200000` for the second
  interface. The threaded-handler path is as on IP27 (branch-likely retry at 0xc00000000005f574) and
  calls the unsymbolled routine at `0xc00000000006191c`. It returns -1 for a level that is not
  reserved or already connected, and 0 after connecting. Established.
- `intr_disconnect_level` (`6.5.30/unix.IP35` `0xc00000000005f620`, 468 bytes)
  is the IP27 routine with the IP35 register choice. It calls `sub_c00000000005f2ac`,
  `mutex_spinlock_spl` and `mutex_spinunlock` as on IP27; after clearing the vector and bit 1 of
  `ii_flags` it subtracts one from `cpu_count` indexed by bit 0 of `p_slice` and writes the software
  mask to `0xa8` or `0xb0`, plus 16 times bit 0 of `p_slice`, plus `0x200000` when bit 1 of `p_slice`
  is set. It returns -1 unless the level is reserved and connected, and 0 otherwise. Established.
- `do_intr_block_bit` (`6.5.30/unix.IP35` `0xc00000000005f7f4`, 448 bytes)
  updates the software mask as on IP27 and writes it to `0xa8`, `0xb0`, `0xb8` or `0xc0` (by bit 0 of
  `p_slice` and the block), plus `0x200000` when bit 1 of `p_slice` (loaded at 0xc00000000005f874) is
  set; when blocking it reads the register back until it matches. Established.
- `intr_block_bit` (`6.5.30/unix.IP35` `0xc00000000005f9b4`, 28 bytes)
  masks a level on a processor: it calls `do_intr_block_bit` with its processor and level and a2 of
  1. Established.
- `intr_unblock_bit` (`6.5.30/unix.IP35` `0xc00000000005f9d0`, 28 bytes)
  unmasks a level on a processor: it calls `do_intr_block_bit` with its processor and level and a2
  of 0. Established.
- `intr_clear_all` (`6.5.30/unix.IP35` `0xc0000000000610d8`, 316 bytes)
  does the IP27 work once per interface: for the first and then the second (`0x200000` higher) it
  writes 0 to the mask registers in the order `0xa8`, `0xb8`, `0xb0`, `0xc0` and calls
  `sub_c000000000061060` with that interface's `0x98` register and level 0, then with its `0xa0`
  register and level 64. Established.
- `sub_c000000000061060` (`6.5.30/unix.IP35` `0xc000000000061060`, 120 bytes)
  has the IP27 body: for each set bit of the pending register at a1 it writes the level to
  `0x9200000001000090` and reads `0x9200000001000098`. Those are the first interface of the caller's
  own node, so bits pending on the second interface are cleared through the first. Established for
  the accesses. Whether the node controller lets the first interface's modify register clear the
  second interface's pending bits is unknown: no shipped software depends on it (see below).

  Every other user of a modify register was found by scanning both IP35 kernels. Six routines load the first
  interface's modify-register pool word (gp+2792 in 6.5.30, gp+768 in 6.5.22), the same six in both:
  `sub_c000000000061060` (the symbol table gives it as `intr_reserve_hardwired+0x15c`, above),
  `cause_intr_disconnect`, `install_cpuintr`, `install_dbgintr`, `gfx_flow_detach_me` and `sendintr`.
  The only routines that build an offset of `0x200000` or more with `lui 0x20` and a register offset of `0x90`
  or `0x98` are `sendintr` (0xc00000000006083c) and `intr_connect_level` (0xc00000000005f468); the five
  others contain no such `lui`. Each of the two picks `0x200090` or `0x90` by bit 1 of the processor's
  `p_slice`. `sendintr` writes `0x100` ORed with a level (7 or 8 when its second argument is 171, else 107
  or 108, by bit 0 of `p_slice`) to the target processor's interface, the form that sets a level. `intr_connect_level` writes the level alone, which clears it, to the modify register of the interface the connecting
  processor sits behind (`0x90` or `0x200090`) and reads back that interface's `0x98` or `0x200098`. The `sendintr` and
  `intr_connect_level` accesses were read in the instructions around those two addresses, and the other five by their pool-word loads
  only.
  So the only clear of a level on the second interface that goes through the second interface's own
  modify register is the one in `intr_connect_level`; `intr_clear_all` is the one place that clears second-interface
  pending bits through the first. Its only direct call (`jal` at 0xc000000000065f54, inside the range the symbol
  table gives `VEC_fpe`) is in a loop over the compact nodes, run right after `init_platform_nodepda` for the same node. Nothing
  in these routines shows software relying on the first-interface write to clear a second-interface level.
  Inferred, from the accesses listed. The hardware behaviour itself: unknown: no shipped software or published
  source depends on it. Routes tried: every user of the pool words in both IP35 kernels; the node controller's
  own description is in no binary.
- `intr_reserve_hardwired` (`6.5.30/unix.IP35` `0xc000000000060f04`, 348 bytes)
  differs from the IP27 routine in whom it reserves for: it keeps a two-byte flag on the stack, one per
  interface, and walks the node's processors from `cnodetocpu` while they are below `maxcpus`
  (gp+5284), enabled and on the node. For each processor whose interface (bit 1 of `p_slice`, loaded
  at 0xc000000000060fe0) has no flag yet it sets the flag and calls `intr_reserve_level` for every
  entry of `hardwired_intr` (`0xc000000001531598`, 128 bytes into `cause_intr_tbl`, from gp-22616),
  with `cmn_err` level 0 and "intr_reserve_hardwired: Can't reserve level %d." for a failure. So the
  first enabled processor on each interface gets the hardwired levels. The walk ends, rather than
  skips on, the first processor that is disabled (the `cpu_enabled` test at 0xc000000000060fb8), on
  another node (the `p_nodeid` compare at 0xc000000000060fd8) or at or above `maxcpus`, so a node
  whose first processor is disabled reserves nothing, and the second interface's table is reserved
  only when every processor before its first one passes those tests. Established.

The IP35 `hardwired_intr` table (`.data` at `0xc000000001531598`) has the IP27 entries with three changes and four additions: level 4 is named "Bedrock/L1" and has flag `0x8`, the IO error moves from level 121 to 102, and the table adds 120 "CPU A Nack count == NACK_CMP", 121 "CPU B Nack count == NACK_CMP", 119 "Local Block Error" and 118 "Local XBar Error", all with flags 0. Linux has no IP35 support, so these levels have no public cross-check. Established for the table.
- `intr_target_from_desc` (`6.5.30/unix.IP35` `0xc00000000005fc64`, 184 bytes)
  is the IP27 routine with one instruction moved: it calls `device_desc_intr_target_get`, then
  `cpuvertex_to_cpuid`, then the CPU bus lookup at `0xc00000000005fb68` (the call at
  0xc00000000005fcc4) or `master_node_get`, and picks the processor through the node chooser at
  `0xc00000000005fb4c` (calls at 0xc00000000005fcf4 and 0xc00000000005fd08), returning -1 where the
  IP27 routine does. Established.

### The same routines in the 6.5.22 builds

The 6.5.22 copies of the routines above. Each row is a routine whose bytes match a 6.5.30 routine on the same page; the note gives the SHA-256 prefix of the bytes (first 12 hex digits). A linked-kernel row is established when the whole body has no calls, address constants or gp accesses; the others are inferred, because the 6.5.22 target of each constant or call was not checked. An object row compares section bytes, not relocated code, so it is inferred. The comparison is in [version-diffs.md](version-diffs.md).

| Routine | Binary | Address | Size | Note | Label |
|---|---|---|---|---|---|
| `hub_intr_cpuid_get` | `6.5.22/unix.IP27` | `0xc00000000003be64` | 8 | returns the interrupt handle's processor, the halfword at +36; same bytes as the 6.5.30 linked kernel (SHA-256 prefix acb766709b99), at a different address; no calls, address constants or gp accesses in the body | Established |
| `hub_intr_cpuid_get` | `6.5.22/unix.IP35` | `0xc0000000000635a0` | 8 | returns the interrupt handle's processor, the halfword at +36; same bytes as the 6.5.30 linked kernel (SHA-256 prefix acb766709b99), at a different address; no calls, address constants or gp accesses in the body | Established |
| `hub_intr_cpuid_get` | `6.5.22/usr/cpu/sysgen/IP27boot/kernel.o` | `0x22ce4` | 8 | returns the interrupt handle's processor, the halfword at +36; same bytes as the 6.5.30 object (SHA-256 prefix acb766709b99) at a different offset; relocations not compared | Inferred |
| `hub_intr_cpuid_get` | `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` | `0x23420` | 8 | returns the interrupt handle's processor, the halfword at +36; same bytes as the 6.5.30 object (SHA-256 prefix acb766709b99) at a different offset; relocations not compared | Inferred |
