# Hub error handling in the IP35 kernel

Routines of the Hub (node controller) error code in the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF). The routines come from the kernel's node error source files, named in the debug information. Constants held in the global pointer's literal pool are given with the pool word's address.

- `hub_ioerror_handler` (`6.5.30/unix.IP35`, address 0xc000000000077ccc, 1028 bytes, 96-byte frame): the Hub's handler
  for an I/O error reported against a node. `a0` is the node's vertex, `a1` the error kind, `a2` a handler argument
  passed through, and `a3` the error record. It fetches the node record with
  `hwgraph_info_get_LBL(a0, "_node", &record)` (the string at 0xc000000001527bc0). When no record comes back it goes
  on with the stack word at 16(sp), which this call never stores, prints "Unable to handle IO error: hardware graph
  not setup" if that word equals 8 and returns it. With a record, its halfword at +10 is the node number. Kinds other
  than 5, 6, 9 and
  10 return 4 (at 0xc000000000077d3c), and 9 returns 9. Kind 10 calls `hub_xp_error_handler(a0, node, 10, a2, a3)` and
  keeps its result. Kind 6 calls `hub_ioerror_local(a0, 6, a2, a3)` when the halfword at record-a3 +6 (the widget
  number) is 1 and otherwise asks `is_widget_pio_enabled(a3)`; if the widget is not enabled it returns 0 after, when
  `error_state_get(a0)` is 3, dumping the record with `ioerror_dump("No outbound widget access - ", 6, a2, a3)`, and
  if it is enabled it calls `hub_xp_error_handler(a0, node, 6, a2, a3)`. Kind 5 first works out which widget and
  offset the failing access names. The error address is the doubleword at a3 +48, masked with 0xf9ffffffffffffff (pool
  word at 0xc000000001644350). A value inside the uncached window from 0x9000000000000000 up to 0x9000010000000000
  (the pool word at 0xc000000001644358) is compared with bounds formed as the pool words at 0xc000000001644070,
  0xc000000001644270, 0xc000000001644360 and 0xc000000001644368 plus the node number shifted left 33. The first range
  takes the widget from bits 27:24 of the doubleword at a3 +16 and the offset from its low 24 bits; the second range
  finds the node's small-window entry among six 48-byte entries starting at node record +808, matching the byte 38
  bytes into the entry with the window number (bits 31:29 of the a3 +16 word, minus one), and asks
  `xtalk_pio_target_get` and `xtalk_pio_xtalk_addr_get` for the widget and base, adding the low 29 bits; an address in
  neither takes the widget halfword the record already has. Anything outside the 0x9000... window goes on only when
  bit 25 of the record's first word is set (otherwise it returns 5 at 0xc000000000077f0c), reading the window number
  from bits 31:29 of the a3 +16 word: 0 gives widget bits 27:24 and offset bits 23:0, any other number is looked up in
  the table at pool word 0xc000000001644020 plus node x 2^33 plus 8 x (number-1) + 352, whose low five bits become
  offset bits 33:29 (added to the low 29 bits of the address) and whose bits 11:8 the widget. All decoded paths store
  the offset at a3 +24 and the widget at a3 +6 and OR the bits 0x40000000 and 0x01000000 into the first word of the
  record. Then a widget of 1 (the Hub itself) goes to `hub_ioerror_local(a0, 5, a2, a3)`; other widgets go through the
  same enabled test, dump and `hub_xp_error_handler` as kind 6, with kind 5. A callee result of 8 prints a second
  message through `printf` (the pool string at 0xc000000001527b88 is the same "hardware graph not setup" text, shared)
  before the result is returned. The bounds arithmetic is read from the instructions at 0xc000000000077d74 to
  0xc000000000077fe8 and the pool words, but the meaning of the 0x9... windows: unknown here. Inferred.
- `hub_error_init` (`6.5.30/unix.IP35`, address 0xc000000000057ad0, 1464 bytes, 128-byte frame): sets up error
  reporting for one node at boot. `a0` is the compact node number. The register block it works in is the node's Hub
  register space, the pool word 0x9200000001800000 (at 0xc000000001644060) plus the node's NASID shifted left 33; the
  NASID comes from the halfword table `compact_to_nasid_node` indexed by `a0`. It starts with
  `hub_error_clear(nasid)` (address 0xc00000000005792c). On node 0 only it then allocates, for every node up to
  `numnodes`, a 0x2258-byte error log with
  `kmem_zalloc_node(0x2258, 1, node)` into
  `kl_error_log[node]` and a 64-byte counter record with
  `kmem_zalloc_node(0x40, 0x101, node)` into
  `hub_err_count[node]` (the second array is at `kl_error_log` + 0x400), and calls `assfail` ("kl_error_log[i] &&
  hub_err_count[i]", file "../ml/SN/SN1/huberror.c", line 340) when either is null. When `a0` is the node of the
  processor running the routine (`get_compact_nodeid`), it re-asserts both pointers (lines 354 and 355), zeroes the
  first word of the log and calls `recover_error_init` on four 128-byte slots at log +0x2010, +0x2090, +0x2110 and
  +0x2190. Then, for each of the two processor register banks (the second is 0x200000 above the first), it reads the
  Hub words at +0x40, +0x48, +0x50 and +0x58 and combines the masks held in the kernel variables `pi_fatal_err_cpu_a`
  (0x5408aaa00), `pi_fatal_err_cpu_b` (0x2a0455500), `pi_misc_err_cpu_a` (0x142000aa) and `pi_misc_err_cpu_b`
  (0xa100055) into stores to the words at +0x408 and +0x410 of that bank, which of them depending on which of the
  pairs (+0x40 and +0x50) and (+0x48 and +0x58) share any bit; the first bank's value also ORs in 0x1000000. After
  that it clears bit 21 of the +0x408 words and bit 20 of the +0x410 words, zeroes +0x448 and +0x468, copies the
  +0x418 words (when the matching +0x40 word is nonzero) into log +0x2230 and +0x2210 (first bank) or +0x2240 and
  +0x2220 (second), and the +0x420 words (when +0x48 is nonzero) into log +0x2238 and +0x2218 or +0x2248 and +0x2228,
  and stores into log +0x2250 the value 0x1000 shifted left by (the doubleword at +0x428 minus one) or 0 when it is 0.
  It connects the node's error interrupt with `cause_intr_connect(6, hubpi_eint_handler, 1)` and never returns from
  `cmn_err(20, "hub_error_init: cause_intr_connect failed on %d", a0)` when that fails. When `a0` is some other node
  it only zeroes the +0x408 and +0x410 words of both banks and sets bit 4 of the word at +0xf0. In both cases it ends
  with `hubii_eint_init(a0)` and `hubni_eint_init(a0)`; on node 0 it also stores 0xffff and
  0 at +0x804 and +0x800 of the global block that follows `kl_error_log` and runs `spinlock_init` there with the name
  "error_dump_lock"; and it stores 12000 into the word at +0x884c of the per-processor structure the pointer at
  absolute address -15360 reaches. Which register each offset is (the names `pi_*err*` are only the kernel's variable
  names) is unknown here. Inferred.
- `huberror_process_rrb_error` (`6.5.30/unix.IP35`, address 0xc000000000059ed0, 1344 bytes, 336-byte frame): turns one
  read-response error type into a recovery decision for the memory page that was read. `a0` is the error type and `a1`
  the physical address of the access; the node number is the 7-bit field in bits 39:33 of the address (`dsrl32
  a0,a1,1` in the jump's delay slot, then a 7-bit mask in each case). A type of 8 or above calls
  `cmn_err(20, "Invalid RRB error type: 0x%x", type)` (level 20) and returns 25 (0x19, the only exit the fact sheet
  reaches). Types 0 to 7 jump through the eight-entry table at 0xc000000001521af0, whose targets are
  0xc00000000005a1a4, 0xc00000000005a148, 0xc00000000005a0e0,
  0xc000000000059f08, 0xc00000000005a088, 0xc00000000005a030,
  0xc000000000059fd8 and 0xc000000000059f68. Every case calls `nasid_to_partid(node)` and `part_id()` (addresses
  0xc000000000496418 and 0xc000000000496400) and compares them. Different partitions:
  `partition_handle_error(0, a1, kind)` (address 0xc00000000005ecfc), result `base` when that gives 3 and
  0 otherwise. Same partition: `error_check_part_page(a1)` (address 0xc00000000005ec7c); a nonzero answer calls
  `partition_handle_error(0, a1, kind)` and gives `base + 1` when that is 3, else 0; a zero answer reports with
  `cmn_err(20, ...)` (a message that names `a1`) and gives `base + 2`. Per type, kind and base: type 0 kind 6 base 10;
  type 1 kind 10 base 7; type 2 kind 4 base 4; type 3 kind 1 base 1; type 4 kind 5 base 13; type 5 kind 2 base 16;
  type 6 kind 7 base 19; type 7 kind 8 base 22. Exceptions: in the same-partition zero case type 0 calls
  `memory_present(a1)` (address 0xc00000000004f4f0) and gives 0 with no message when it returns 0, and otherwise
  reports and gives 12; type 4 gives 15 with no message; type 2 first calls `page_isdiscarded(a1)` (address
  0xc00000000025f08c): for a page not yet discarded it prints the memory bank name (`membank_pathname_get`), reports
  with `cmn_err(20, ...)`, calls `error_page_discard(a1, 1, 1)` (address 0xc00000000005ebbc) and gives 6 unless that
  returned 0 or 1, in which case it goes on to `error_check_part_page`, and a zero answer from `error_check_part_page`
  there gives 0 with no message. What the kinds passed to `partition_handle_error` stand for: unknown here. The fact
  sheet reaches only the invalid-type exit, because it does not follow the indirect jump, so the values of the eight
  cases were read from the disassembly alone, so the checker cannot confirm them. Inferred.
- `huberror_process_wrb_error` (`6.5.30/unix.IP35`, address 0xc00000000005a410, 1148 bytes, 48-byte frame): turns one
  write-response error type into a recovery decision for the memory page that was written. `a0` is the error type,
  `a1` the physical address of the access, `a2` a partial-write flag (kept at 8(sp)) and `a3` the address-space number
  of the write (kept in `s2`). The node is bits 39:33 of the address. As in the read-response routine, each handled
  type compares `nasid_to_partid(node)` with `part_id()` (addresses 0xc000000000496418 and
  0xc000000000496400). When the partitions differ it calls `partition_handle_error(0, a1, kind)` (address
  0xc00000000005ecfc) and returns a code when that gives 3 and 0 otherwise. When they are the same it first calls
  `error_check_part_page(a1)` (address 0xc00000000005ec7c); a nonzero answer calls
  `partition_handle_error(0, a1, kind)` and returns a code if the result is 3, else 0. The codes and kinds are: type 0
  (message "Cached write error, physaddr 0x%x") kind 3, different partition 29, same partition 30; type 1 (partial
  write) kind 10, 33 and 34; type 3 ("Write timeout, paddr 0x%x") kind 3,
  26 and 27; type 6 kind 7,
  36 and 37; type 7 kind 8,
  39 and 40. A zero from `error_check_part_page` takes one of these paths. Type 0:
  `error_check_memory_access(a1, 1, 0)` (address 0xc00000000005c8b8); nonzero returns 32 when it equals 3 and
  0 otherwise, zero reports "Cached write error" with `cmn_err(20, ...)` and returns 31. Type 1: the same
  `error_check_memory_access` test (32 or 0); when it is zero and the flag in
  `a2` is zero it reports "Uncached Partial Write error without partial flag, physaddr 0x%x node %d" with `cmn_err`
  level 4 (the node being the halfword at absolute address -15318 in the per-processor page) and returns 35; when the
  flag is set it picks a space name from `a3`: 0 "Hspec space", 2 "Mspec space", 3 "uncached memory space", 1 "IO
  space" and anything else "unknown space", and reports "Uncached Partial Write error in %s, physaddr 0x%x node: %d"
  with `cmn_err(20, ...)` and returns 35, except that space 1 first calls `kl_ioerror_handler` (address
  0xc00000000005e4dc, with the node, node, -1, 6, the address, 0,
  1, 0) and returns 0 with no message when that returns 0. Type 3:
  `error_check_poisoned_state(a1, 0, 1)` (address 0xc00000000005c9a4); a result of 3 reports "Write timeout, paddr
  0x%x" with `cmn_err(20, ...)` and returns 28, any other result returns 0. Type 6: reports "Write Response Data
  Error, physaddr 0x%x" and returns 38. Type 7: reports "Write Packet Length Error, physaddr 0x%x" and returns 41. Any
  other type reports "Invalid WRB error type: 0x%x" at level 20 and returns 42. The two-argument calls with the
  constants 3, 7, 8 and
  10 give the third argument of `partition_handle_error`; what those kinds stand for: unknown here. Established.
- `hubiio_crb_error_handler` (`6.5.30/unix.IP35`, address 0xc00000000005acd8, 808 bytes): scans the Hub's I/O
  interface CRB (crosstalk request buffer) table for entries that carry an error and reports or clears each one. `a0`
  is the Hub vertex and `a1` the Hub I/O soft state, whose halfword at +10 is the NASID and whose lock at +796 is held
  for the whole scan (`mutex_spinlock_spl` with the spl-error routine `splerr`, then `mutex_spinunlock`). The register
  block is the pool word 0x9200000001c00000 (at 0xc000000001644020) plus the NASID shifted left 33. For each of the 15
  CRBs (0 to 14, 32 bytes apart, three doublewords read from +0x400, +0x408 and +0x410 of that entry) it skips the
  entry unless bit 50 of the first word is set. For a set entry it takes the type from bits 54:52 of the first word.
  If bit 48 of the third word is set, or if bits 32:30 of the second word are 1 or 5 and bits 42:41 are 1 or
  2, the entry is treated as a block-transfer-engine error: it builds an error record on the stack (flag word
  0x80000000 at +0 with the type in the halfword at +4, or 0xc1000000/0xe1020000 forms when the first word's bit 0 is
  set), calls `bte_crb_error_handler(a0, engine, crb, record)` (address 0xc00000000007f2cc; the engine is bit 58 of
  the second word when bit 48 of the third word is set and bit 32 of it otherwise), frees the entry with
  `hubiio_crb_free(a1, crb)` (address 0xc00000000005ac38) and adds one to the count that is the return value. Any
  other valid entry first tests bit 55: when set the type picks one of eight cases through the table at
  0xc000000001521e18. Types 0, 5, 6 and
  7 dump the error log with `sn0_error_dump` (address 0xc00000000005c0ac) and report "Fatal %s on hub : %d" with
  `cmn_err` level 0 and the name from the table `hub_rw_err_info`, then continue as for type 8 or more: another
  `sn0_error_dump` and "Fatal error (code : %d) on hub : %d"; none of these frees the entry or counts it. Types 1 to 4
  report "%s on hub cnodeid: %d" at level 5, call `hub_ioerror_handler(a0, 10, 1, record)` (address
  0xc000000000077ccc) with bit 31 set in the record's flag word and the type at +4, and, when that returns nonzero,
  dump the log and report "Unable to handle %s on hub %d"; in either case they then call
  `hub_ioerror_handler(a0, 10, 3, record)`, free the entry with
  `hubiio_crb_free` and move to the next entry without counting. An entry whose bit 55 is clear is checked for bit 49
  ("Xtalk Packet with error bit set to hub %d") and bit 51 ("Uncorrectable data error detected on data from Craylink
  to node %d"), each reported at level 0, and no entry state is changed. After the 15th entry it releases the lock and
  returns the count of entries handled by the block-transfer path (0 when none). The record fields built for the
  non-BTE cases (halfwords at +6 and +8 and the doubleword at +24) were read from the stores in the decompiled body.
  Inferred.
- `hubpi_eint_handler` (`6.5.30/unix.IP35`, address 0xc000000000058350, 1624 bytes, 80-byte frame): the handler for
  the processor-interface (PI) error interrupt of a Bedrock Hub; the messages in its body say "BEDROCK PI". It takes
  no arguments and returns nothing. It first calls `check_ni_errors` (address 0xc000000000059040) and, if that finds
  any, `hubni_error_handler("PI interrupt", 1)` (address 0xc000000000059064). It then reads the pending word at
  0x9200000001000400 and masks it with the mask word at 0x9200000001000408 or, when bit 0 of the byte at +0x528 of the
  current processor's per-processor block (reached through `pdaindr`, indexed by the halfword at absolute address
  -15320) is set, the one at 0x9200000001000410. Call the result P and the bit-0 flag S. Fatal bits: P masked with the
  kernel variables `pi_fatal_err_cpu_a` and `pi_fatal_err_cpu_b` (0x5408aaa00 and
  0x2a0455500). If any are set it logs "Fatal PI error interrupt 0x%x (from cpu A)" with `cmn_err` level 20, then the
  same with "cpu B" for the second variable, and if the call returns it prints " bit 0x%x %s" for each entry of the
  table `hub_rw_err_info` (16-byte entries from +0x148; value, name) whose bit is set in the fatal bits until an entry
  above 0x400000000, then calls `kl_panic_setup` (address 0xc00000000005e660) and logs the same two "Fatal PI error
  interrupt" messages at level 18. Bit 32 of P (`dsll32 ... 7` sign test) calls `log_md_errors(get_nasid())` (address
  0xc000000000059348) and `md_int_block` (address 0xc00000000005bb4c) and stores -1 into the word at +0x884c of the
  per-processor structure the pointer at absolute address -15360 reaches. Then, for three bit pairs, it tests the bit
  for this side (the first of each pair when S is set, the second when it is clear): bits 1 and 2, bits 0x40 and 0x80,
  bits 0x10 and 0x20. A hit calls `process_error_spool` (address 0xc00000000005a984) and writes that same bit value
  back to the pending word (to acknowledge it). Bits 0x100000 and 0x200000 log "Unexpected UCE error interrupt. int
  pend 0x%x" at level 20. Bits 4 and 8 mean a spurious message: it reads two header words at 0x92000000010004c8 and
  0x92000000010004d0 and the bit 1 of the word at 0x9200000001000020; if the first header is valid (bit 63) and its
  field in bits 24:17 is 0x90, 0x92 or 0x89 it clears both headers and acknowledges the bit and carries on; otherwise
  it prints both header words (the field layouts "00 error status B valid", "01 error status A valid", "16 to 06
  message supplemental", "24 to 17 message command", "61 to 25 error address", "62 tail bit", "63 spurious header
  valid", "63 to 53 message source" are in the strings), calls `part_id` (address 0xc000000000496400), and logs
  "Spurious message intr, likely partition reboot" at level 20 and "PANIC: unexpected spurious message" at level 16.
  Bits 0x8000000 and 0x10000000 log "UE_CACHED error interrupt. int pend 0x%x" and bits 0x2000000 and 0x4000000 log
  "SYSAD_BAD_DATA (Data Quality Bad) error interrupt. int pend 0x%x", both at level 20. The bit order of the checks
  and the side selection come from the instructions at 0xc000000000058464 onward; what each bit of the pending word
  stands for beyond these message names: unknown here. Inferred.

## Hub programmed-I/O windows

- `hub_piomap_alloc` (`6.5.30/unix.IP35`, address 0xc000000000062214, 792 bytes, 112-byte frame): allocates the
  Hub-side window that lets a processor reach a range of a crosstalk widget's address space, and returns the window
  record or 0. `a0` is the requesting vertex (stored in the record), `a1` a descriptor that is not used, `a2` the
  crosstalk address, `a3` the length, `a4` the smallest length that is acceptable, and `a5` the flags (bit 0 and bit 1
  tested together as 3 for the wait rule below, bit 0 alone for "fixed", bit 3 for the 0x3800000000000000 attribute
  bits). It gets the widget from `xwidget_info_get`, its number from `xwidget_info_id_get` and its master vertex from
  `xwidget_info_master_get`, and returns 0 at once (address 0xc000000000062278) when `a3` is smaller than `a4`. It
  then reads the node record of the master through `hwgraph_info_get_LBL(master, "_node", ...)` and works in two ways.
  A range that ends within the first 16 megabytes (`a2` + `a3` below 0x1000001) uses the widget's own small-window
  record at node record + 0x30 x (widget number & 0xff) + 0x10 and returns its address; with flag bit 3 set it first
  ORs 0x3800000000000000 into the doubleword at that record's +0x28. A larger range is accepted only when `a3` plus
  the low 29 bits of `a2` is at most 0x20000000, otherwise the result is 0 (address 0xc000000000062328). For an
  accepted large range it takes the node's window lock (`mutex_spinlock` at node record +0x318) and searches the six
  48-byte window entries at node record +0x328: an entry that is in use (bit 0 of its word at +0x28), is marked fixed
  (bit 2), has base (+8) equal to `a2` with its low 29 bits cleared and widget byte (+4) equal to the widget number is
  shared: its use count (halfword at +0x24) is incremented, the lock is released and the entry returned. Otherwise it
  remembers the last entry whose in-use bit is clear (searching all six). When one is free: with flag bit 0 set it
  counts the request against the node's fixed-window counter (the word at node record +0x320, limited to 5; at the
  limit the result is 0), then fills the entry: it writes the Hub's window register at the pool word
  0x9200000001c00000 plus 8 x entry plus 0x160 plus node x 2^33 with `((widget & 0xf) << 8) | (a2 >> 29 & 0x1f)`,
  stores the vertex at +0, the widget at +4, the base `a2` with the low 29 bits cleared at +8, the window's
  processor-side address (the pool word 0x9200000020000000 plus node x 2^33 plus entry x 0x20000000, with the
  attribute bits when flag bit 3 is set) at +0x18, the entry number as a byte at +0x26, increments the use count, and
  sets bit 0 of the word at +0x28 (bits 0 and 2 when flag bit 0 was set). When none is free it releases the lock and
  either returns 0 (when `a5` & 3 is nonzero, meaning the caller does not want to wait) or releases the lock and
  sleeps on `sv_wait(node record +0x310, 25, node record +0x318, ...)`, and then searches again. The routine returns
  the entry address in every successful case. Which Hub register the window register at +0x160 is, and what the
  entry's word at +0x28 holds: unknown here. Inferred.
- `hubpi_spool_get_next` (`6.5.30/unix.IP35`, address 0xc000000000059ca0, 560 bytes): takes the next entry from the
  Hub's error spool for the current processor side. `a0` points to the doubleword that receives the entry, and `a1` to
  a second doubleword that receives a companion word in one case; the return value is 0, 1 or 2. It gets the node's
  NASID with `get_nasid`, raises the interrupt level with `spl7` (the symbol `splerr` is the same address,
  0xc00000000006201c) and lowers it with `splx` (address 0xc000000000062054) on every exit. The side S is bit 0 of the
  byte at +0x528 of the processor's per-processor block, reached through `pdaindr` and the halfword at absolute
  address -15320; the node index is the halfword at absolute address -15318. It works on the node's error log
  `kl_error_log[node]` (the log `hub_error_init` allocated): the read position is the doubleword at log + 8 x S +
  0x2210 and the write position the doubleword at log + 8 x S + 0x2230. With no log, or a read position of 0, it
  returns 0. When the two positions differ it reads one doubleword from the uncached address (read position OR
  0x9600000000000000 plus NASID shifted left 33) into `*a0`, adds 8 to the read position, subtracts the log's spool
  size (the doubleword at log +0x2250) from it when the new position is aligned to that size, and returns 1. When the
  positions are equal it reads the Hub's spool header registers for side S into `*a0` and `*a1` (the words at
  0x9200000001000430 and 0x9200000001000440 for side 0,
  0x9200000001000450 and 0x9200000001000460 for side 1); if the first word is negative (bit 63 set) it writes 0 to the
  register at 0x9200000001000448 or 0x9200000001000468, sets the write position to the doubleword read from the
  register at 0x9200000001000418 or 0x9200000001000420, calls `klsave_spool_registers(*a0, *a1, 2)` (address
  0xc000000000059acc) and returns 2; otherwise it returns 0. What the registers at those addresses are called is
  unknown here. Inferred.

## Interrupt dispatch

- `intr` (`6.5.30/unix.IP35`, address 0xc0000000000604bc, 636 bytes, 48-byte frame): the kernel's top-level processor
  interrupt dispatcher. `a0` is the exception frame (passed on to the handler), `a2` the saved Status register value
  and `a3` the Cause register value; the pending lines are the bits 15:8 of the bitwise AND of `a2` and `a3`. It
  returns a word from the dispatch table, or 0. On entry it checks bits 5:4 of the per-processor word at absolute
  address -16312: if both are set it calls `check_delay_tlbflush(0)` (address 0xc000000000266ed8), and after the
  handler it calls `check_delay_tlbflush(1)` when bit 4 is still set. When bit 5 of the doubleword at absolute address
  -15744 is set it brackets the dispatch with
  `log_tstamp_event(0x66, 0, 0, 0, 0)`
  on entry and
  `log_tstamp_event(0x65, 0x66, 0, 0, 0)`
  on exit. If no line is both enabled and pending it calls an unnamed static routine at 0xc000000000060260 and returns 0.
  Otherwise it adds one to the word at +208 of the per-processor structure the doubleword at absolute address -15088
  points to, and tests the Cause bits in this fixed order: 0x4000, 0x800, 0x2000,
  0x8000, 0x1000, 0x100,
  0x400, 0x200, using the first one set. For the line n (bit n+8 of Cause) it reads the 16-byte entry of the table
  `cause_intr_tbl` (address 0xc000000001531518) at 16 x n: the handler pointer at +0, the interrupt level at +8 and a
  word at +12. It loads the word at +12 as the return value, calls `splx` (address 0xc000000000062054) with the level
  and then calls the handler with `a0`. The line whose bit is 0x100 uses entry 0, 0x200 entry 1,
  0x400 entry 2, 0x800 entry 3,
  0x1000 entry 4, 0x2000 entry 5,
  0x4000 entry 6 and 0x8000 entry 7. When none of the eight bits is set it calls `intr_stray(frame)` (address
  0xc00000000005ee4c) and returns 0. What the entries' +12 words hold is unknown here. Established.

## Hub widget shutdown

- `hub_widgetdev_shutdown` (`6.5.30/unix.IP35`, address 0xc000000000078eac, 528 bytes): when a device behind a
  crosstalk widget is shut down, stops the Hub from using it and releases the request buffers (CRBs) still waiting on
  it. `a0` is the widget vertex and `a1` the device number on that widget (0 to 7, or 4 and above for the interrupt
  kind of request, as the code uses it). It gets the widget from `xwidget_info_get`, its number from
  `xwidget_info_id_get` and its master vertex from `xwidget_info_master_get`, and reads the master's node record with
  `hwgraph_info_get_LBL(master, "_node", ...)`. With no record, or a node number (halfword at record +10) of -1, it
  does nothing. Otherwise the Hub register block is the pool word 0x9200000001c00000 plus the node number shifted left 33.
  It clears one bit of the doubleword at +0x120 of that block: bit number (`a1` + 8 x (widget number & 0xff) - 64), so
  each (widget, device) pair owns a bit in a 64-bit permission word. It then walks the 15 CRBs (32 bytes apart from
  block +0x400) and picks the ones that belong to the device. With the kernel flag `free_error_crbs` zero, a CRB
  counts when bit 50 of its first word is set; with it nonzero, it counts unless bit 55 is set or bit 50 is clear (the
  test in the code is: valid, or not already flagged as error). A counted CRB belongs to this device when, for a
  device number below 4, the 2-bit field in bits 41:40 of the first word equals `a1` (the routine tests for inequality
  and frees on a zero), and for a device number of 4 or more when the 3-bit field in bits 42:40 is zero. Such a CRB is
  released with `hubiio_crb_free(record, crb)` (address 0xc00000000005ac38) and counted. When the kernel flag
  `free_error_crbs_print` is nonzero each release prints "Releasing %s CRB 0x%x[Freed so far %d]" ("Interrupt" or
  "Data" as the name) and, after the walk, "Freed %d CRBS on nasid %d" with `cmn_err` level 6. The routine returns
  nothing. Which Hub register the doubleword at +0x120 is, and what the bits 41:40 and 42:40 of a CRB word stand for,
  are unknown here. Inferred.
- `hubii_eint_init` (`6.5.30/unix.IP35`, address 0xc000000000058088, 452 bytes, 48-byte frame): connects the Hub's I/O
  interface error interrupt for one node and programs where the Hub sends it. `a0` is the compact node number and the
  routine returns nothing. It reads the node's Hub vertex from the word at +0x8c80 of the node's per-node block
  (reached through the pointer at absolute address -15360, indexed by `a0`) and calls
  `assfail("hub_v != GRAPH_VERTEX_NONE", "../ml/SN/SN1/huberror.c", 526)` when that is -1. It reads the node record
  with `hwgraph_info_get_LBL(hub, "_node", ...)` and works in the Hub register block at the pool word
  0x9200000001c00000 plus the node's NASID (halfword at record +10) shifted left 33. When bit 13 of the register
  doubleword at block +0x128 is clear it clears bit 10 of that word (mask 0xfffffffffffffbff). It picks a processor
  and interrupt level with `intr_heuristic(hub, 0, -1, 4, hub, "HUB IO error interrupt", &level)` (address
  0xc00000000005fe04); a result of -1 calls `cmn_err(20, "hubii_eint_init: intr_reserve_level failed, cnode %d", a0)`
  at level 20, which does not return. It connects the handler `hubii_eint_handler` (address 0xc00000000005b17c) with
  `intr_connect_level(cpu, level, 0, handler, hub, 0)` (address 0xc00000000005f308); a negative result calls
  `assfail("rv >= 0", "../ml/SN/SN1/huberror.c", 556)`. It then writes the register doubleword at block +0x138 with
  the target: the 7-bit level plus bit 24 (0x1000000), masked with the pool word 0xfffffffffffe01ff
  (0xc000000001644170), with the NASID of the chosen processor's node (`get_cpu_cnode` and `compact_to_nasid_node`, 8
  bits) shifted left 9, bit 8 cleared and then set from bit 1 of the byte at +0x528 of that processor's per-processor
  block (`pdaindr` indexed by the processor number). Which register fields these are (target NASID, processor slice
  and vector) follows from the values, not from names in the code, and is inferred. Inferred.
- `hub_device_inquiry` (`6.5.30/unix.IP35`, address 0xc000000000063234, 436 bytes): prints the Hub's access state for
  one crosstalk widget. `a0` is a crossbar vertex and `a1` the widget number; the routine returns nothing. It formats
  the widget number as text with `sprintf("%d")`, finds that child with `hwgraph_traverse(a0, text, &vertex)`, takes
  the child's master with `device_master_get` and reads the master's node record with
  `hwgraph_info_get_LBL(master, "_node", ...)`; if any of these fails it prints nothing. With a record, the Hub
  register block is the pool word 0x9200000001c00000 plus the node number (record +10, halfword) shifted left 33, and
  it reads three doublewords from it: +0x110 (outbound), +0x118 (inbound) and +0x120 (the per-device permission word,
  the same word `hub_widgetdev_shutdown` clears bits of). It prints, all at `cmn_err` level 6: "Inquiry Info for %v"
  with the widget vertex; "\tDevices shutdown [ " followed by " %d" for each device number i from 0 to 7 whose bit
  number (8 x widget - 64 + i) is clear in the permission word, and "]"; "\tInbound access ? %s" with "yes" when bit
  number `widget` of the inbound word is set and "no" otherwise; and "\tOutbound access ? %s" the same way with the
  outbound word. The labels (outbound, inbound) are those of the printed text; which Hub registers the three
  doublewords are: unknown here. Established.
- `hubni_print_state` (`6.5.30/unix.IP35`, address 0xc0000000000589a8, 332 bytes, 80-byte frame): prints the Hub's
  network-interface error state for one node, once for each requester. `a0` is the node's NASID, `a1` the opaque first
  argument of the print routine, `a2` the print routine (called as `a2(a1, format, ...)`) and `a3` a requester mask.
  The node's data block is reached from `nasid_to_compact_node[a0]` through the per-node table whose pointer is at
  absolute address -15360. When `a3` is 2 it first waits for the block's lock at +0x8644 to be free: it calls
  `mutex_spintrylock` (address 0xc000000000332174) up to
  1,000,000 times with
  `us_delay(1)` between tries, and when a try succeeds it releases the lock again at once with `mutex_spinunlock` (it
  never holds the lock while printing). It then ORs `a3` into the word at block +0x8f08 with a load-linked and
  store-conditional loop (the retry branch at 0xc000000000058a10 is branch-likely, `beqzl`). If any bit of `a3` was
  already set in the old value it returns, so each requester prints at most once. Otherwise it prints "NI ERROR
  STATE:" and the line "/module/%M/node" with the halfword at block +0x861a, and calls `show_ni_port_error` (address
  0xc00000000005600c) with the doubleword read from the Hub register address 0x9200000001e80000 plus the NASID shifted
  left 33 plus 0x100, `a1` and `a2`. It returns nothing. Which register that doubleword is, beyond the routine name
  that decodes it: unknown here. Established.
