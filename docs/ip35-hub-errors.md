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
  hub_err_count[i]", line 340, file argument not copied) when either is null. When `a0` is the node of the
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
  interface CRB (crosstalk request buffer) table for entries that carry an error and reports or clears each one. The bit positions it tests in each entry's three words (the shifts and masks were re-read in the instructions) are in the chips repository's `bedrock_ii` unit (registers `crb_a`, `crb_b` and `crb_c`); their roles stay as stated here. `a0`
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
- `hub_error_clear` (`6.5.30/unix.IP35`, address 0xc00000000005792c, 420 bytes, 32-byte frame): puts one node's Hub
  error registers back to a clean state; `hub_error_init` calls it first. `a0` is the NASID. It returns nothing
  (`v0` is left holding scratch) and has one exit, at 0xc000000000057ac8. Each address it touches is a Hub
  register-space base from the literal pool plus the NASID shifted left 33. A two-pass loop (0xc000000000057980 to
  0xc0000000000579e8) runs first on base 0x9200000001800000 (pool word 0xc000000001644060) and then on the same base
  plus 0x200000, chosen by `movn` on the pass count; each pass stores, in this order, all-ones at +0x400, +0x438 and
  +0x458 and zero at +0x4c8 and +0x4d0. A nine-pass loop (0xc000000000057a2c to 0xc000000000057a54) then rewrites the
  nine consecutive doublewords from 0x9200000001c00000 (pool word 0xc000000001644020) + 0x198: the old value is ANDed
  with 0xffff07ffffffffff (pool word 0xc000000001644150, clearing bits 43 to 47), ORed with 0x80000000000 (bit 43),
  ANDed with 0xffffffc000c00000 (pool word 0xc000000001644158) and ORed with 3. The result keeps old bits 63 to 48, 42
  to 38, 23 and 22, has bit 43 and bits 1 and 0 set, and has every other bit clear. Between the two loops (the
  instructions at 0xc0000000000579f0 to 0xc000000000057a28) the routine loads four doublewords that nothing later reads, from 0x9200000001fc0018, 0x9200000001fe0018,
  0x9200000001f80058 and 0x9200000001fc0028 (plus the shifted NASID), and a fifth, from 0x9200000001e80120, after
  the nine-pass loop; they are inferred to be reads for their effect on the hardware, since the values are dropped.
  After the nine-pass loop it stores all-ones at 0x9200000001c00000 + 0x1f8, clears bit 28 of the doubleword at
  0x9200000001c00000 + 0x138 (read, AND with 0xffffffffefffffff, write), stores all-ones at 0x9200000001e00000 (pool
  word 0xc000000001644038) + 0x4048 and zero at + 0x4050. The last loop (0xc000000000057a98 to 0xc000000000057ac0)
  zeroes fifteen doublewords from 0x9200000001f00000 (pool word 0xc000000001644168) + 0x120 to + 0x190 in ascending
  order, two per pass, the second store sitting in a branch delay slot; the first store of a pass always runs and the pass exits
  after it, skipping the second, once the incremented index reaches 113. What each register is, in hardware terms, is unknown here; the role (reset the node's error status,
  masks and counters) is inferred from the routine name and its use by `hub_error_init`. Inferred.
- `hub_xp_error_handler` (`6.5.30/unix.IP35`, address 0xc000000000077b7c, 232 bytes, 64-byte frame): forwards a
  crosstalk-side error that `hub_ioerror_handler` or `hub_ioerror_local` found on a node, but only when the Hub says
  the error belongs to the crosstalk port. `a0` is the node vertex, `a1` the NASID and `a2`, `a3` and `a4` the error
  kind, handler argument and error record, which it passes on unchanged. It first loads the doubleword at
  0x9200000001c00000 (pool word 0xc000000001644020) + NASID shifted left 33 + 0x1e8; when neither bit 0 nor bit 4 (mask
  0x11) is set it returns 7 (the `li a7,7` at 0xc000000000077ba4, exit through 0xc000000000077c44). Otherwise it calls
  `error_state_get()` and keeps the result in 0(sp). It finds the node's data block as the doubleword indexed by
  `nasid_to_compact_node[a1]` (a halfword table, address from gp-21528) in the table that the pointer at absolute
  address -15360 reaches, takes the word at block +0x8640 and calls `error_state_set(word, state)`. A result of 7
  returns 9 (the `li a7,9` at 0xc000000000077c08 sits before the compare). Otherwise, when the saved state equals 3,
  it first calls `error_state_set(a0, 1)`. In both remaining cases it returns the result of
  `xtalk_error_handler(word at block +0x8640, a2, a3, a4)`. What the Hub register at +0x1e8 holds: unknown here. Established.
- `hub_widget_reset` (`6.5.30/unix.IP35`, address 0xc000000000079180, 328 bytes, 80-byte frame): drops the Hub's
  pending request entries for one crosstalk widget and returns that widget's register to its idle setting. `a1` is
  the widget number; `a0` is never read. `xwidget_gfx_reset` is the caller. It calls `master_node_get()` and keeps the
  result as a sign-extended halfword; when that is -1 it returns at once, with `v0` still holding the
  `master_node_get` result (branch at 0xc0000000000791b8 to the epilogue at 0xc000000000079278). Otherwise it takes
  the NASID from the halfword table `compact_to_nasid_node` (address from gp-21544), raises the priority level with
  the routine at 0xc000000000061f6c (called here as `splhi`; the symbol table gives the same address the names `spl1`,
  `spl3`, `spl5`, `spl6`, `splvme`, `splnet` and others) and saves the old level at 0(sp). It then scans eight
  doublewords at 0x9200000001c00000 (pool word 0xc000000001644020) + NASID shifted left 33 + 0x308 + 8 x i, for i from 0
  to 7. An entry is acted on when bit 63 is set and bits 24 to 31 equal the widget number. For such an entry it stores
  zero in it, stores the value `i | 0x10` in the doubleword at base + 0x388 (the same address every time), reads that
  doubleword back and, if bit 4 (0x10) of it is still set, calls `assfail("! (REMOTE_HUB_L(nasid, IIO_IPDR) &
  IIO_IPDR_PND)", file argument not copied, line 1037)` (the strings at gp-21536 + 5440 and + 5496). That compare is
  `beqzl`, so the increment of `i` in its delay slot runs only when the bit is clear; on the assert path the `addiu` at
  0xc0000000000792b8, in the delay slot of the branch at 0xc0000000000792b4, supplies it, and the scan always visits all eight
  entries. After the scan it calls `hub_setup_prb(nasid, widget, 3, 2)`, restores the level with `splx` (the same
  address as `splxecc`, at 0xc000000000079270) and returns the `splx` result. What the entries and the register at
  +0x388 are in the hardware: unknown here; the string names the register as the kernel's `IIO_IPDR` pending test,
  which is inferred to be the same register. Established.
- `hub_ioerror_local` (`6.5.30/unix.IP35`, address 0xc000000000077a80, 252 bytes, 32-byte frame): reports an I/O error
  that a processor caused by touching a Hub register it is not allowed to use. `hub_ioerror_handler` calls it (two
  sites) for a widget number of 1, the Hub itself. `a2` is the error kind and `a3` the error record; `a0` and `a1` are
  not read. A kind of 0, 2 or 3 returns 0 (exits through 0xc000000000077ab4 and the fall-through at
  0xc000000000077aa4, which sets `v0` to zero in the delay slot). Any other kind never gets back normally. It reads
  two halfwords of the record, at +12 and +14, and translates each through the halfword table
  `compact_to_nasid_node` (address from gp-21544), so the first gives a NASID (here `v0`, kept at 8(sp)) and the second
  another (`a1`, kept at 0(sp)). The error address passed to the messages is the doubleword at record +16 when bit 25
  of the record's first word is set, otherwise 0 in the first message only. It reads the doubleword at 0x9200000001800000
  (pool word at gp+2288) plus the second NASID shifted left 33 and tests bit number (first NASID) of it. With that bit set
  it calls `cmn_err(4, ...)` with "Error accessing Hub Register %x on node (%d/%d) from (%d/%d), with access permission\n"
  (0xc000000001527a68) and then `kl_panic("Hub Register Access error")` (the string at 0xc000000001527ac0). In either case
  it then reaches the second report, which uses the record's address and halfwords again: `cmn_err(4,
  "Error in accessing hub register 0x%x on node (%d/%d) by cpu %d on Node (%d/%d): No permission\n")`
  (0xc000000001527ae0) and `kl_panic("Hub Register Access Permission error")` (0xc000000001527b40). Only registers a2
  to a6 are loaded for those calls; the format has six conversions. Neither `kl_panic` call is expected to return. If
  one does, the first path falls into the second report, and the routine returns the last `kl_panic` result at
  0xc000000000077aac. Which Hub register the bit at +0 of that doubleword guards: unknown here. Established. The offset is the first register of the processor interface in the public SN1 register header (`PI_CPU_PROTECT`); that this is the one the routine tests is inferred from the offset alone. A bus error on a doubleword load in the node-named register window reaches this routine through `dobuserre` and `handle_io_buserr`; docs/ip35-crash-dump.md follows that path for the crash dump's Hub register walk.
- `huberror_process_stack_entry` (`6.5.30/unix.IP35`, address 0xc00000000005a88c, 248 bytes, 48-byte frame): decodes
  one saved Hub request-buffer error record and hands it to the write-side or read-side error routine. Callers are
  `process_error_spool` and `get_matching_spool_addr`. `a0` is the record's first word, `a1` its second word, `a2` the
  record layout (1 or 2) and `a3` a pointer through which the decoded address is returned. The routine returns the
  result of `huberror_process_wrb_error` or `huberror_process_rrb_error`, stores the decoded address (`s0`) as a
  doubleword at 0(`a3`) after the call, and has one normal exit at 0xc00000000005a914. Layout 1 takes the address
  as bits 31 and up of `a0` shifted down and then left 7 (`dsrl a0,0x1f; dsll 7`), the write flag as bit 12 of `a0`,
  and a two-bit field `a0 >> 13 & 3` that is used only when bit 22 (value 0x200 after the `>> 13`) is set. Layout 2
  takes the address as bits 25 to 61 of `a0` (`>> 25 & 0x1fffffffff`) shifted left 3, the write flag as bit 42 of
  `a1`, and the two-bit field `a1 >> 43 & 3`, again used only when bit 53 (`a1 >> 43 & 0x200`) is set. When that
  bit is clear the field value passed on is the word at 0(sp), which this routine never stores, so it is whatever the
  caller left on the stack. In both layouts `a0 & 7` is the first argument.
  With the write flag set it calls `huberror_process_wrb_error(a0 & 7, address)`. With it clear it calls
  `huberror_process_rrb_error(a0 & 7, address, the 0x200 bit as a 32-bit value, field)`. Any layout other than 1 or 2
  calls `cmn_err(20, "huberror_process_stack_entry, bad type 0x%x")` (0xc000000001521ce0); the code after it loads
  four unset stack words and rejoins the flag test, but the level-20 report is expected not to return. Which hardware condition each
  record field reports: unknown here. Established.
- `hubiio_crb_free` (`6.5.30/unix.IP35`, address 0xc00000000005ac38, 160 bytes, 32-byte frame): releases one entry of
  the Hub's table of outstanding I/O requests and waits until the Hub reports the release done. `a0` is the node's
  error record (its halfword at +10 is the NASID) and `a1` the entry number. It returns nothing (single exit,
  0xc00000000005acd0, with `v0` left from whichever instruction ran last). Callers are `hubiio_crb_error_handler` (two
  sites) and `hub_widgetdev_shutdown`. Every address is the base 0x9200000001c00000 (pool word 0xc000000001644020) plus
  the NASID shifted left 33. It first rewrites the first word of the entry, the doubleword at base + 32 x entry + 0x400,
  masking it with 0xfffbfffffffffffd (pool word 0xc000000001644210), which clears bits 50 and 1 and keeps every
  other bit. It then stores `entry | 0x10` into the doubleword at base + 0x390, reads that doubleword back and, while
  bit 4 (0x10) is still set, calls `us_delay(1)` and reads it again; the loop has no bound. When the bit is clear it
  returns. Which hardware feature the register at +0x390 and bit 1 of the entry word belong to: unknown here. The
  routine's callers treat bit 50 of the same word as the marker that an entry is already free. Established.

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
- `hub_setup_prb` (`6.5.30/unix.IP35`, address 0xc000000000062edc, 148 bytes, no frame): rewrites one of the nine
  consecutive 64-bit per-widget registers of the Hub that `hub_error_clear` also reinitialises (same base and
  stride). `a0` is the NASID, `a1` the crosstalk widget number, `a2` the new low byte and `a3` a mode value. It
  returns nothing and has one exit, at 0xc000000000062f68, with the store in its delay slot. Callers are
  `hub_set_piomode`, `hub_widget_flags_set`, `hub_widget_reset` and `kona_attach`. First, when the kernel variable
  `force_fire_and_forget` (word at gp-28884, address 0xc00000000163c69c, initial value 1) is nonzero and
  `ignore_conveyor_override` (word at gp-28880, address 0xc00000000163c6a0, initial value 1) is zero, a mode of
  exactly 1 becomes 2 and other modes pass through; with either variable otherwise the mode is unchanged, so at the
  shipped initial values it is unchanged. The register index is `a1` for a widget below 8 and `a1 - 7` for 8 to 15,
  so widgets 8 to 15 use registers 1 to 8 (widgets 1 to 7 share registers 1 to 7 with widgets 8 to 14). The register
  is the doubleword at 0x9200000001c00000 (pool word 0xc000000001644020) + NASID shifted left 33 + 0x198 + 8 x index.
  The new value is built in this order: the old value ANDed with 0xffff07ffffffffff (pool word 0xc000000001644150,
  clearing bits 47 to 43), ORed with bit 43, ANDed with 0xfffffbc000c000ff (pool word 0xc000000001644280), ORed with
  bit 42 when the mode is not 1, ANDed with 0xffffffffffffff00 and ORed with `a2 & 0xff`. The result keeps old bits 63
  to 48, 41 to 38, 23 and 22, has bit 43 set, has bit 42 set exactly when the mode is not 1, has `a2`'s low byte in
  bits 7 to 0, and has bits 47 to 44, 37 to 24 and 21 to 8 clear. Which hardware feature the register and its bits belong to is
  unknown here. That each register serves one widget is inferred from the index mapping and the callers' widget
  arguments. Established.
- `hub_set_piomode` (`6.5.30/unix.IP35`, address 0xc000000000062f70, 240 bytes, 64-byte frame): reprograms the Hub's
  nine per-widget registers (through `hub_setup_prb`) for one node while the node's doubleword at +0x110 of its
  register block is held at zero. `a0` is the NASID and `a1` is passed through as the mode argument of every
  `hub_setup_prb` call. Callers are `io_xswitch_widget_init` (two sites) and `hubio_init`. The routine first calls
  `get_console_nasid`; when `a0` equals the result it takes the console print lock with `putbuf_lock` (and notes that
  in `s3`) before anything else, and releases it with `putbuf_unlock` at the end. The block is the doubleword base
  0x9200000001c00000 (pool word 0xc000000001644020) plus the NASID shifted left 33.
  It saves the doubleword at block +0x110 on the stack (24(sp)), stores zero there, and tests bit 17 of the doubleword
  at block +0x20 (the `dsll32 ...,0xe` shifts it to the sign bit). With the bit clear it calls `hub_setup_prb(a0, 0, 1,
  a1)`; with it set `hub_setup_prb(a0, 0, 3, a1)`. Then for widget numbers 8 to 15 it calls `hub_setup_prb(a0, n, 3,
  a1)`. It restores the saved value at block +0x110. The return value is the doubleword read back from +0x110 (the
  `ld` in the delay slot at 0xc000000000063008) when this is not the console node, and the result of `putbuf_unlock`
  when it is. Which hardware feature block +0x20 bit 17 and block +0x110 belong to: unknown here. Established.
- `hub_piomap_free` (`6.5.30/unix.IP35`, address 0xc00000000006252c, 236 bytes, 48-byte frame): gives back a window
  that `hub_piomap_alloc` made, freeing the Hub's window register when the last user lets go. `a0` is the window
  record: its word at +0x20 is the vertex, the halfword at +0x24 the use count, the byte at +0x26 the entry number and
  the word at +0x28 the flags. `xtalk_piomap_free` calls it. It returns nothing (single exit 0xc00000000006259c). A record
  whose flag bit 1 is clear is left alone and the routine returns at once (the branch at 0xc000000000062540); the code
  that sets that bit is not in this routine. Otherwise it looks up the node record with
  `hwgraph_info_get_LBL(vertex, "_node", &record)` (string at 0xc000000001524ef8), takes the NASID from the halfword at
  record +10, takes the window lock with `mutex_spinlock(record + 0x318)`, and stores the use count minus 1 back as a
  halfword (0xc000000000062580, in the delay slot). When the new count is not zero it releases the lock with
  `mutex_spinunlock(record + 0x318, level)` and returns. When the count reaches zero it writes the doubleword 0x300
  to the Hub window register at 0x9200000001c00000 (pool word 0xc000000001644020) + NASID shifted left 33 + 8 x entry
  number + 0x160, the register that `hub_piomap_alloc` fills; 0x300 is the layout's widget field set to 3 with the
  address bits clear, read as the idle value (inferred). It then updates the flags: when bit 2 is clear it clears
  bit 0; when bit 2 is set it clears bits 0 and 2 and decrements the word at record + 800 (the count of fixed
  windows, the counter `hub_piomap_alloc` limits to 5). It calls `sv_signal(record + 0x310)` to wake any process that
  waited for a free window, and then releases the lock as above. Established.
- `hub_check_pci_equiv` (`6.5.30/unix.IP35`, address 0xc000000000062e54, 136 bytes, 32-byte frame): decides whether
  two crosstalk-side addresses name the same location in a PCI window, allowing for the two NASIDs being aliases of
  one node. `a0` and `a1` are the two addresses. `ioc3_attach` and `ioc4_attach` call it (two sites each), and the IOC3
  attach text records its result when comparing a controller's windows with the console's. It returns 0 or the
  result of `check_nasid_equiv`; the single exit is 0xc000000000062ea0. It first calls `hub_check_window_equiv(a0, a1)`
  (0xc000000000062dec); a zero result returns 0. That routine returns 1 when `hub_check_is_widget0` holds for both
  addresses, otherwise 1 when bits 24 to 31 of the two addresses are equal, otherwise 0. Next the low 24 bits of the
  two addresses are compared (mask 0x00ffffff, from `lui` 0xff and `ori` 0xffff); a difference returns 0. When they
  match it takes bits 33 to 39 of each address (shift right 33, mask 0x7f, sign-extended as a 16-bit halfword) and
  returns `check_nasid_equiv(first, second)` (0xc000000000046c40). Established.
- `hub_piotrans_addr` (`6.5.30/unix.IP35`, address 0xc000000000062680, 196 bytes, 64-byte frame): the Hub's
  translation of a crosstalk address into a processor-side address for a widget that already has a small window; it
  never allocates one. `a0` is the widget's vertex, `a1` is not read, `a2` is the crosstalk address, `a3` the length
  and `a4` the flags (only bit 3 is tested). It returns the processor-side address, or 0 for a range that is not covered, or 0x3800000000000000
  (see below). The widget information record comes from `xwidget_info_get(a0)`; `xwidget_info_id_get` gives the widget
  number (kept at 8(sp)) and `xwidget_info_master_get` the master vertex, whose node record it reads with
  `hwgraph_info_get_LBL(master, "_node", &record)` (the string at 0xc000000001524ef8). When the sum
  `a2 + a3` is greater than 0x1000000 (sixteen megabytes, `sltu` against `lui` 0x100) it returns 0 (the branch at
  0xc0000000000626d8 skips to the epilogue with `v0` zero from its delay slot). Otherwise it calls
  `hub_piomap_addr(record + 0x30 x (widget number & 0xff) + 0x10, a2, a3)` (0xc000000000062618), which is the lookup
  into the widget's small-window record created by `hub_piomap_alloc`; that routine returns 0 for a range outside
  the record's base (+8) and size (+16), or when bit 0 of the record's word at +0x28 is clear, and otherwise the
  record's doubleword at +0x18 plus the crosstalk address modulo the size. When `a4` bit 3 is set the result is ORed with
  0x3800000000000000 (formed by `li` 7 and `dsll32` 27) whether or not it was zero, so a miss with that flag
  returns 0x3800000000000000. Established.
- `hubii_ixtt_get` (`6.5.30/unix.IP35`, address 0xc0000000000631b8, 124 bytes, 32-byte frame): reads one Hub register
  that belongs to the node a crosstalk widget is attached to, and stores it where the caller points. It always
  returns 0. `a0` is the widget vertex and `a1` the address that receives a doubleword (kept in `s8`). It gets the widget
  information with `xwidget_info_get(a0)` (0xc0000000000975e8) and the master vertex with `xwidget_info_master_get`
  (0xc000000000097700), clears the record slot at 0(sp) and calls `hwgraph_info_get_LBL(master, "_node", &record)`
  (0xc000000000340d70; the string is at 0xc000000001524ef8), whose result is not tested. It takes the lock at record +792
  with `mutex_spinlock` (0xc000000000331f6c), reads the doubleword at `0x9200000001c00000` + (the halfword at record
  +10 << 33) + 496 (the pool word 0xc000000001644288 holds the negated base, which the code subtracts), stores it as
  the doubleword at `a1`, and releases the lock with `mutex_spinunlock(record + 792, level)` (0xc000000000331fb8). Its
  callers are two sites in `xbridge_attach`. The name's "ixtt" and the register at offset 496: unknown here beyond this
  access. Established.
- `hub_piomap_done` (`6.5.30/unix.IP35`, address 0xc000000000062678, 8 bytes, no stack frame): ends a use of a window
  record from `hub_piomap_alloc`. The body is `jr ra` with a `nop` delay slot: no argument is read, nothing is stored
  and `v0` is untouched, so a use leaves no state to undo. Established.
- `hub_piomap_addr` (`6.5.30/unix.IP35`, address 0xc000000000062618, 96 bytes, no stack frame): looks an address up in a
  small-window record. `a0` is the record, `a1` the crosstalk address and `a2` the length; `hub_piotrans_addr` calls it.
  The record's doubleword at +8 is the base and the one at +16 the size. It returns 0 when `a1` is below the base
  (unsigned compare, the branch at 0xc000000000062620) and 0 when the base plus the size is below `a1 + a2` (unsigned,
  the branch at 0xc000000000062634), so a range may end exactly at the end of the window; the sum has no wrap-around
  test. It then returns 0 when bit 0 of the record's word at +40 is clear (the word `hub_piomap_free` calls the flags).
  Otherwise it divides `a1` by the size (`ddivu`, remainder taken with `mfhi`) and returns the record's doubleword at +24
  plus that remainder. It is the address itself, not its distance from the base, that is reduced modulo the size.
  A zero size reaches the `teq` at 0xc000000000062658 with trap code 7. Established.
- `hub_check_window_equiv` (`6.5.30/unix.IP35`, address 0xc000000000062dec, 104 bytes, 32-byte frame): says whether two
  crosstalk addresses can name the same window. `a0` is kept at sp+0 and `a1` in `s0`. It calls `hub_check_is_widget0`
  (0xc000000000062d98) with `a0` and, if that is nonzero, with `a1`; when both are nonzero it returns 1 (the `li` at
  0xc000000000062e20, in the delay slot of the unconditional branch before it). Otherwise it returns 1 when bits 24 to 31 of the two addresses (shift
  right 24, mask 0xff) are equal and 0 when they differ (the `or v0,zero,zero` at
  0xc000000000062e3c is the delay slot of the compare-and-branch before it). The instruction at 0xc000000000062e24 is never reached. Established.
- `hub_widget_flags_set` (`6.5.30/unix.IP35`, address 0xc000000000063060, 72 bytes, 16-byte frame): turns a flag word
  into a per-widget register update. `a0` is the NASID, `a1` the widget number and `a2` the flags. When bit 0 of `a2` is set it
  calls `hub_setup_prb(a0, a1, 3, 1)`. Otherwise, when bit 1 is set, it calls `hub_setup_prb(a0, a1, 3, 2)`. Bit 0 takes
  precedence and no call is made when both are clear. It always returns 1 (the `li` at 0xc00000000006309c). Established.
- `hub_device_flags_set` (`6.5.30/unix.IP35`, address 0xc0000000000630a8, 148 bytes, 48-byte frame): sets flags for the
  widget that a device vertex sits on. `a0` is the device vertex and `a1` the flags (saved at sp+16). It calls
  `xwidget_info_get(a0)`, then `xwidget_info_id_get` on the result (kept at sp+40) and `xwidget_info_master_get` on the
  same result, and `hwgraph_info_get_LBL(master, "_node", &record)` (the string at 0xc000000001524ef8, pool word gp-22568),
  after storing zero in the record slot at sp+0; the result is not tested. It takes
  `mutex_spinlock(record + 792)` (level kept at sp+32), calls `hub_widget_flags_set(halfword at record +10, widget id
  & 0xff, a1)` and releases the lock with `mutex_spinunlock(record + 792, level)`. It returns what
  `hub_widget_flags_set` returned. Established.
- `hubii_ixtt_set` (`6.5.30/unix.IP35`, address 0xc00000000006313c, 124 bytes, 32-byte frame): the writing counterpart of
  `hubii_ixtt_get`. `a0` is a widget vertex and `a1` points to a doubleword (kept in `s8`). It calls `xwidget_info_get(a0)`,
  `xwidget_info_master_get` on the result and `hwgraph_info_get_LBL(master, "_node", &record)` (slot at sp+0 cleared
  first). It takes `mutex_spinlock(record + 792)`, stores the doubleword that `a1` points to at the address 0x9200000001c00000
  plus the NASID (halfword at record +10) shifted left 33 plus 496 (the pool word at gp+2840 holds the negated base,
  which the code subtracts), releases the lock with `mutex_spinunlock(record + 792, level)` and returns 0. Established.
- `hub_widget_id` (`6.5.30/unix.IP35`, address 0xc00000000004141c, 24 bytes, no stack frame): reads a four-bit field of a Hub register
  that the name calls the widget id. `a0` is the NASID. It returns the low four bits (`andi` in the delay slot of the return) of the
  doubleword at 0x9200000001c00000 (pool word gp+2224) plus `a0` shifted left 33 plus 32. Which Hub register that is: unknown
  here. Established.
- `hubio_init` (`6.5.30/unix.IP35`, address 0xc000000000062098, 28 bytes, 16-byte frame): calls
  `early_brl1_port_init()` (0xc0000000002283b0) with no argument set and returns its result. Its 28 bytes end at
  0xc0000000000620b4, where the next routine begins. Established.

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
- `intrnorm` (`6.5.30/unix.IP35`, address 0xc00000000006d680, 620 bytes, no stack frame): the assembly entry for the
  normal processor interrupt. It runs with the interrupted registers live, uses only `k0` and `k1` until it has saved
  state, takes no arguments, and ends by jumping to `ecommon` (0xc00000000006d8f0) or `ecommon_allsaved` (0xc00000000006d940)
  or by returning from the exception. Addresses written as plain numbers are words of the per-processor page reached
  through the zero register. First it loads the word at -16312 into `k0` (kept for a test below). Stage one runs only
  when Cause bit 15 is set. It then reads coprocessor 0 register 25 with selects 1 and 3, and with selects 5 and 7
  unless the processor revision register's implementation byte (bits 15 to 8) equals 9; these appear as the raw words
  0x401bc801, 0x401bc803, 0x401bc805 and 0x401bc807 (the disassembler does not decode them). If any value has bit 31
  set it goes on to stage two. When none does it tests bit 6 of the saved word: when that is clear it goes on to stage
  two, when set it writes 0xfffffffff8000000 to coprocessor 0 register 11 and executes `eret` (0xc00000000006d6e4).
  That the registers are performance counters whose overflow shares Cause bit 15, and that the write acknowledges the
  timer, are inferred from the register numbers. Stage two reads Status: with bit 0 clear it returns with `eret`; with it
  set it ANDs Status bits 15 to 8 with Cause and returns with `eret` (0xc00000000006d708) when no enabled interrupt is
  pending. Otherwise it picks a stack. When the doubleword at -16344 is zero the signed byte at -16305 (the state)
  decides: 2 goes to 0xc00000000006d8d0, which makes a frame at (sp - 360) rounded down to a multiple of 8, stores sp at
  frame +224, sets `k0` to 256 and jumps to `ecommon` with sp set to the frame; 1 sets the state to 2, makes the same
  frame, stores sp at its +224, stores the frame address as the doubleword at +552 of the structure at -16352, loads sp
  from -16336 and jumps to `ecommon` with `k0` 2; 3 sets the state to 2, stores sp at +224 of the stack whose
  address is at -16336 and switches to it, stores 1 in the byte at -16301, and jumps to `ecommon_allsaved` when the byte
  at -16304 is nonzero and to `ecommon` otherwise (`k0` 2); any other state value sets the state to 2, takes the frame
  as the doubleword at +1008 of the structure at -16360 plus 360, stores sp at +224 and the kernel global pointer
  0xc000000001643770 at +216 of it, loads sp from -16336 and jumps to `ecommon` with `k0` 2. When the doubleword at
  -16344 is nonzero and the state is 2 it takes the 0xc00000000006d8d0 path above. Otherwise it takes the frame as that
  structure's doubleword at +136 minus 360 rounded down to a multiple of 8, stores gp at +216 and sp at +224, sets sp to the
  frame, sets the state to 2, stores `ra` at +240, a7 down to a0 at +80 to +24 and s7 down to s0 at +176 to +120, loads the
  doubleword at -16328 and stores it at +0 in the delay slot of `jal tfi_save` (0xc000000000061980), copies the
  structure's +144 doubleword to the word at -16360, calls `uli_setasids` (0xc00000000006589c) with the halfword at +168
  and jumps to `ecommon_allsaved`. The block at 0xc00000000006d774 to 0xc00000000006d7ac, which loads sp from -15288
  and jumps to the routine at 0xc0000000000619f4 with the strings "invalid kstk" (0xc000000001531cc8) and
  "the IRIX tree assembly unit 1" (0xc000000001531cd5) and 0, has no branch into it from inside the routine. Inferred.
- `sub_c000000000062980` (`6.5.30/unix.IP35`, address 0xc000000000062980, 520 bytes, 96-byte frame): the unnamed
  routine behind `hub_intr_alloc` and `hub_intr_alloc_nothd`; it picks a processor and an interrupt bit for a device and
  builds the 48-byte handle that the other `hub_intr_*` routines use. `a0` is the device vertex (kept in `s2`), `a1` a
  device descriptor or zero (`s0`), `a2` an owner vertex (`s1`) and `a3` a no-thread flag, saved at sp+8. With `a1` zero it
  calls `device_desc_default_get(a0)` and uses the result as the descriptor (zero stays zero). With no descriptor the name
  is zero, the level is the word `default_intr_pri` (0xc00000000163c1bc, gp-30132) and the flags are 8 when the flag is
  zero, or the word at sp+0, which this routine never stores, when it is not. With a descriptor the name comes from
  `device_desc_intr_name_get` and the level from `device_desc_intr_swlevel_get`, and the flags are 4 when bit 0 of the
  descriptor's word at +24 is set, otherwise 0 when the flag is nonzero or descriptor bit 2 is set, otherwise 8. It calls
  `intr_heuristic(a0, descriptor, allocate_my_bit, flags, a2, name, result pointer)`, the pointer being the address of a word in its own frame, where `allocate_my_bit` is the word at
  0xc00000000163b874 (gp-32508). The processor is the low 16 bits of the result, sign-extended. When it is -1 the
  routine calls `cmn_err(4, "%v hub_intr_alloc could not allocate interrupt\n", owner)` (the string at
  0xc000000001524f98) and returns 0. Otherwise it finds the processor's per-processor block through `pdaindr` (entry
  index shifted left 4, doubleword at +8), reads that block's halfword at +1066 (member `p_nodeid` in the debug
  information, the compact node) and byte at +1320 (`p_slice`), and forms an offset that is 0x200090 when bit 1 of the
  byte is set and 0x90 otherwise. The node's NASID is the halfword at `compact_to_nasid_node` plus twice the
  node. It calls `kmem_alloc_node(48, 1, node)`; a null result calls `assfail` with the expression text "intr_hdl", a
  source file name and line 788 and then goes on into the stores below with the null pointer in `s1`. The handle is filled in this order: word +0 the device
  vertex, byte +5 the low byte of the word that `intr_heuristic` stored through that pointer (the bit), doubleword +8 the offset plus the NASID
  shifted left 39 plus the pool word at gp+2824 (0x0000800001800000), then
  `xwidget_info_get(a0)` and, when that is nonzero, byte +4 from `xwidget_info_masterid_get`, then word +32 the level,
  halfword +36 the processor, word +44 the value 1 and word +40 the bit. It then calls the unnamed routine at
  0xc00000000006292c with the descriptor (it may be zero; the call is unconditional), the level and the processor, and returns the
  handle. Established.
- `sub_c00000000006292c` (`6.5.30/unix.IP35`, address 0xc00000000006292c, 84 bytes, 64-byte frame): records on a device
  descriptor which processor an interrupt went to. `a0` is the descriptor (saved at sp+48), `a1` a level and `a2` a
  processor number (kept in `s8`). It calls `device_desc_intr_swlevel_set(a0, a1)`, formats the path with
  `sprintf(&sp+0, "/hw/cpunum/%d", a2)` (the string at 0xc000000001524f88, the "_node" block address from gp-22568 plus
  144), looks it up with `hwgraph_path_to_dev` and calls `device_desc_intr_target_set(a0, result)`, returning its result.
  Established.
- `hub_intr_alloc` (`6.5.30/unix.IP35`, address 0xc000000000062b88, 28 bytes, 16-byte frame): calls the unnamed routine
  at 0xc000000000062980 with its three arguments unchanged and `a3` set to 0 (in the delay slot of the call), and
  returns its result. Established.
- `hub_intr_alloc_nothd` (`6.5.30/unix.IP35`, address 0xc000000000062ba4, 28 bytes, 16-byte frame): the same as
  `hub_intr_alloc` except that `a3` is set to 1. Established.
- `hub_intr_free` (`6.5.30/unix.IP35`, address 0xc000000000062bc0, 120 bytes, 32-byte frame): releases a handle from
  the allocation routine. `a0` is the handle (kept in `s0`). It first loads the handle's halfword at +36 (processor), word at
  +40 (bit) and word at +44 (flags), saving the first two at sp+8 and sp+16. When flag bit 2 is set it stores, in this
  order, zero in the byte at +5, zero in the doubleword at +8 and -1 in the word at +0, then calls `hub_intr_disconnect`
  with the handle and reloads the flags. When flag bit 0 is then set it calls `kmem_free(handle, 48)`. It finally calls
  `intr_unreserve_level(processor, bit)` with the saved values and returns its result. It does not clear the flag words
  before freeing. Established.
- `hub_intr_connect` (`6.5.30/unix.IP35`, address 0xc000000000062c38, 136 bytes, 32-byte frame): attaches a handler to a
  handle. `a0` is the handle, `a1` the handler, `a2` the handler's argument, `a3` a callback (kept in `s1`) and `a4` the
  callback's own value (saved at sp+0). It calls `intr_connect_level(processor from +36, bit from +40, word at +32,
  a1, a2, 0)`. A negative result is returned as it is. Otherwise it stores `a3` at +24 and then `a4` at +16 (the second
  store is in the delay slot of the test of `a3`), calls `a3(handle)` when `a3` is nonzero, sets bit 2 of the word at
  +44 and returns 0. Established.
- `hub_intr_disconnect` (`6.5.30/unix.IP35`, address 0xc000000000062cc0, 100 bytes, 32-byte frame): detaches the handler.
  `a0` is the handle. It loads the word at +40 (kept at sp+24), the halfword at +36 and the doubleword at +24. When the
  doubleword at +24 is nonzero it calls it with the handle as the only argument. It then calls
  `intr_disconnect_level(processor, bit)` and clears bit 2 of the word at +44 (the mask -5 is formed in `v0`). It sets
  no return value of its own: `v0` holds -5 on return. Established.
- `hub_intr_cpu_get` (`6.5.30/unix.IP35`, address 0xc000000000062d24, 28 bytes, no stack frame): returns the vertex
  recorded for the handle's processor. `a0` is the handle. The processor is the halfword at +36; the routine reads the
  doubleword at +8 of that processor's entry in `pdaindr` (pool word gp-22560, 16-byte entries) and returns the word
  at +1852 of that block (member `p_vertex` in the debug information), loaded in the delay slot of the return.
  Established.

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
  `assfail("hub_v != GRAPH_VERTEX_NONE", file argument not copied, line 526)` when that is -1. It reads the node record
  with `hwgraph_info_get_LBL(hub, "_node", ...)` and works in the Hub register block at the pool word
  0x9200000001c00000 plus the node's NASID (halfword at record +10) shifted left 33. When bit 13 of the register
  doubleword at block +0x128 is clear it clears bit 10 of that word (mask 0xfffffffffffffbff). It picks a processor
  and interrupt level with `intr_heuristic(hub, 0, -1, 4, hub, "HUB IO error interrupt", &level)` (address
  0xc00000000005fe04); a result of -1 calls `cmn_err(20, "hubii_eint_init: intr_reserve_level failed, cnode %d", a0)`
  at level 20, which does not return. It connects the handler `hubii_eint_handler` (address 0xc00000000005b17c) with
  `intr_connect_level(cpu, level, 0, handler, hub, 0)` (address 0xc00000000005f308); a negative result calls
  `assfail("rv >= 0", file argument not copied, line 556)`. It then writes the register doubleword at block +0x138 with
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
- `hub_dma_enabled` (`6.5.30/unix.IP35`, address 0xc0000000000790bc, 196 bytes, 48-byte frame): tells whether the
  Hub allows inbound DMA from a crosstalk widget, and reports to the console when it does not. It returns 1 when the
  widget's permission bit is set and 0 otherwise. `a0` is the widget vertex (saved at 8(sp)). It gets the widget
  information with `xwidget_info_get(a0)` (0xc0000000000975e8), the widget number with `xwidget_info_id_get`
  (0xc0000000000976cc; saved at 24(sp)) and the master vertex with `xwidget_info_master_get` (0xc000000000097700; saved
  at 16(sp)). It then calls `hwgraph_info_get_LBL(master, "_node", &record)` (0xc000000000340d70; the string is at
  0xc000000001527bc0) with the record slot at sp + 0, which the routine does not clear first, and reads that slot.
  A zero record returns 0 (the branch at 0xc000000000079110); so does a node number of -1 in the halfword at record +10
  (the branch at 0xc000000000079120). Otherwise it reads the doubleword at `0x9200000001c00000` (pool word
  0xc000000001644020) + (node number << 33) + 280 and tests bit (widget number & 0xff) of it. A set bit returns 1.
  A clear bit calls `cmn_err(6, "No Inbound access\n\tWidget %d(%v)\n\tNode  %d(%v)\n", widget number & 0xff, widget
  vertex, node number, master vertex)` (0xc0000000002ad048; the string is at 0xc000000001528f78) and returns 0. Which
  Hub register offset 280 is: unknown here. Established.
- `hub_error_devenable` (`6.5.30/unix.IP35`, address 0xc000000000078d4c, 192 bytes, 48-byte frame): when the
  error kind is 5, re-arms a widget's error reporting at the Hub if the widget's register says so. It always returns 0.
  `a0` is the widget vertex, `a1` is not read and `a2` is the error kind (`s0`). It gets the widget number with
  `xwidget_info_get(a0)` (0xc0000000000975e8) and `xwidget_info_id_get` (0xc0000000000976cc; saved at 8(sp)), and the
  master vertex with `xwidget_info_master_get` (0xc000000000097700), then calls
  `hwgraph_info_get_LBL(master, "_node", &record)` (0xc000000000340d70; the string is at 0xc000000001527bc0), with
  the record slot at sp + 0. The result is not tested, and the halfword at record +10 (the node number) is loaded
  before either kind test, so a missing record would be dereferenced. A kind of 6 returns 0 at once, as does any kind other
  than 5 and 6. For kind 5 it reads the doubleword at `0x9200000001c00000` (pool word 0xc000000001644020) + (node
  number << 33) + 8 x (widget number & 0xff) + 712 and tests its bit 63. When that is set it stores
  `(widget number & 0xff) | 0x10` as the doubleword at the same base + (node number << 33) + 904; when clear it
  stores nothing. Which two Hub registers these are, and what bit 63 and the 0x10 bit mean: unknown here. Established.
- `hub_widgetdev_enable` (`6.5.30/unix.IP35`, address 0xc000000000078e0c, 160 bytes, 48-byte frame): gives one device
  behind a crosstalk widget permission to use the Hub, by setting its bit in the node's permission word; it is the
  counterpart of `hub_widgetdev_shutdown`. It returns no value that a caller can use (`v0` is left holding whatever the last
  call or address computation produced). `a0` is the widget vertex and `a1` the device number (saved at 16(sp)). It gets the
  widget number with `xwidget_info_get(a0)` (0xc0000000000975e8) and `xwidget_info_id_get` (0xc0000000000976cc), the master
  vertex with `xwidget_info_master_get` (0xc000000000097700), and calls `hwgraph_info_get_LBL(master, "_node",
  &record)` (0xc000000000340d70; the string is at 0xc000000001527bc0) with the record slot at sp + 0. A zero record
  returns without doing anything (the branch at 0xc000000000078e68). Otherwise the register block is the doubleword at
  `0x9200000001c00000` (pool word 0xc000000001644020) + (the halfword at record +10 << 33), and the word it changes is the
  doubleword at +288 of it. The bit number is `a1 + 8 x (widget number & 0xff) - 64`; the bit is formed with a variable
  shift, which uses only the low six bits of that number. When the bit is already set nothing is written. When it is clear
  the routine stores the old value ORed with the bit (the store is in the delay slot of the branch at
  0xc000000000078ea4). The same bit numbering is the one `hub_widgetdev_shutdown` clears. Which Hub register +288 is:
  unknown here. Established.

## Network interface error interrupts

- `hubni_eint_init` (`6.5.30/unix.IP35`, address 0xc00000000005824c, 260 bytes, 32-byte frame): connects the Hub's
  network-interface error interrupts for one node. `a0` is the compact node number (kept in `s2`). It asks
  `cnodetocpu(a0)` (0xc00000000004e694) for a cpu of the node and returns that value (-1) when there is none. Otherwise it calls
  `intr_connect_level(cpu, 126, 0, hubni_eint_handler, 0, 0)` (0xc00000000005f308; the handler at 0xc00000000005909c comes from
  the pool word 0xc00000000163de40, the cpu is the halfword in `a0`). A nonzero result calls `cmn_err(20, "hub_error_init:
  Cannot connect NI error interrupt.")` (0xc0000000002ad048; the string is at 0xc000000001520f20), and the routine goes on
  after it. It then visits the four slices s of the node. Each slice's cpu is `cnode_slice_to_cpuid(node, s)`
  (0xc00000000004e980), taken as a sign-extended halfword; a slice is skipped when that number is negative, is not below
  the word `maxcpus` (0xc000000001644c14, a signed compare), or when `cpu_enabled(cpu)` (0xc00000000004e948) is zero.
  For each remaining slice it calls `intr_connect_level(cpu, 103 + (s & 1), 0, hubni_rmteint_handler, 0, 0)` (the handler at
  0xc0000000000590c0, pool word 0xc00000000163de48), so even slices use level 103 and odd slices level 104, and a nonzero
  result calls `cmn_err(20, ...)` with the same string. The slice counter is advanced in the delay slot of the
  branch-likely `beql` at 0xc000000000058300 for a successful connection and in the delay slot at 0xc00000000005834c
  after a failed one. It returns the -1 from `cnodetocpu` when the node has no cpu; on every other path `v0` is
  whatever the last callee left, and no caller's use of it was read (single exit 0xc000000000058320). Established.
- `hubii_eint_handler` (`6.5.30/unix.IP35`, address 0xc00000000005b17c, 416 bytes, 48-byte frame): the handler for
  the Hub's I/O-interface error interrupt, connected by `hubii_eint_init`. `a0` is the
  Hub vertex. It returns nothing a caller uses (`v0` is left from the last call, exit at 0xc00000000005b290 with the
  final store in its delay slot). It first calls `check_ni_errors()` (0xc000000000059040) and, when that is nonzero,
  `hubni_error_handler("II interrupt", 1)` (0xc000000000059064; the string is at 0xc000000001521eb0). It then reads the
  node record with `hwgraph_info_get_LBL(a0, "_node", &record)` (the string at 0xc000000001520f10) and takes the NASID from the
  halfword at record +10. It tests bit 62 of the doubleword at 0x9200000001800000 (pool word 0xc000000001644060) + NASID
  shifted left 33 + 0xa0. With the bit set it calls the unnamed static routine at 0xc000000000058ea0 with ("Remote NI
  interrupt" at 0xc000000001521800, NASID, 1) and then `cleanup_remote_pi(NASID, 126)` (0xc00000000005b4b0), and skips
  the other handling. With the bit clear it reads the doubleword at 0x9200000001c00000 (pool word 0xc000000001644020) +
  NASID shifted left 33 + 8. When bit 32 of that is clear it calls `hubiio_crb_error_handler(a0, record)` and then
  `hubiio_prb_error_handler(a0, record)`. When bit 32 is set it first calls `cmn_err(0, "Hub %d to Xtalk Link failed
  (II_ECRAZY) Reason: %s", halfword at record +8, reason)`, where the reason is chosen in this order: bit 9 of the doubleword
  gives "Micro Packet Retry Timeout", else bit 5 "Crosstalk Tail Timeout", else bit 4 "Crosstalk Credit Timeout", else the
  doubleword at the same base + 0x1e8 is read and bit 0 gives "Hub Widget 0 has been Shutdown", bit 4 "BTE 0 has been
  shutdown", bit 8 "BTE 1 has been shutdown", and none of them gives "Unknown". The listing's code after that
  `cmn_err` runs the same two handlers as above, so the disassembly continues even though the call may not return.
  Both paths then store 0xffffff as the doubleword at the 0x9200000001c00000 base + NASID shifted left 33 + 0x1f8
  and clear bit 28 of the doubleword at +0x138 of that base (read, AND with 0xffffffffefffffff, write). These are the
  same two addresses `hub_error_clear` writes, though that routine stores all-ones at the first one. Which hardware
  fields the tested bits are: unknown here, beyond the text of the messages that name them. Established.
- `hubiio_prb_error_handler` (`6.5.30/unix.IP35`, address 0xc00000000005b0b0, 204 bytes, 64-byte frame): looks
  at the Hub's nine per-widget registers (the ones `hub_setup_prb` writes) and reports each widget whose register has
  bit 48 set. `a0` is the Hub vertex and `a1` the node record, whose halfword at +10 is the NASID. It returns the number
  of widgets reported (single exit, 0xc00000000005b144; `s4` is the count). Its caller is `hubii_eint_handler`. Registers
  are the doublewords at 0x9200000001c00000 (pool word 0xc000000001644020) + NASID shifted left 33 + 0x198 + 8 x i. It
  first tests register 0 (bit 48 is the sign bit after `dsll 15`): with it set it calls the unnamed static routine at
  0xc00000000005b068 with (`a0`, `a1`, widget 0) and sets the count to 1 (the `addiu s4,zero,1` at 0xc00000000005b178, in the
  delay slot of the branch before it) before looking at the others. With it clear the count starts at 0. It then tests
  registers 1 to 8, which stand for widgets 8 to 15 (`s1` runs from 8 to 15 and `s0` from 1 to 8 together), and for
  each one with bit 48 set calls the same static routine with that widget number and adds 1 to the count. The static
  routine at 0xc00000000005b068 has no symbol of its own (it occupies the 72 bytes from 0xc00000000005b068 to this routine's start, just after
  `hubii_check_widget_disabled`); it reads the NASID from the halfword at +10 of its second argument, stores `1 << widget` as the doubleword at the same base + NASID shifted left 33 + 504 (in the delay slot of its
  `cmn_err` call, so before the call) and then calls `cmn_err(4, "Hub nasid %d got a PIO Write error from widget %d,
  cleaning up and continuing", NASID, widget)` (string at 0xc000000001521e60). Which hardware condition bit 48 of
  those registers reports: unknown here. Established.
- `hub_error_count_enable` (`6.5.30/unix.IP35`, address 0xc00000000005b564, 172 bytes, 32-byte frame): turns on the
  periodic error counting of one node, returning 0 when it starts it and -1 when it does not. `a0` is the node's NASID
  (a signed compare, so a negative NASID or one of 128 or more returns -1 at once). The compact node number is the halfword
  at `nasid_to_compact_node` (0xc000000001646f00) + 2 x NASID; -1 there returns -1. The counting record is the
  doubleword at `hub_err_count` (0xc000000001650990, which is 1024 bytes after `kl_error_log` at 0xc000000001650590,
  the pool word at 0xc00000000163de20) + 8 x node, a pointer; the word at the start of what it points to is the
  enabled flag, and when that is nonzero the routine returns -1. When it is zero it calls `blkclr(record, 64)` (0xc00000000004791c)
  and `timeout(hub_error_count_update, NASID, 100, 1, record)` (0xc000000000261ca0; `hub_error_count_update`,
  at 0xc00000000005b4f0, comes from the pool word 0xc00000000163de78; the fourth argument is the constant 1 and the
  fifth the record pointer). The store of 1
  into the enabled flag sits in the delay slot of that call. It then returns 0 (set at 0xc00000000005b60c). The `syssgi` handler calls it. What `hub_error_count_update` counts, and the unit of the 100: unknown here.
  Established.
- `hubii_check_widget_disabled` (`6.5.30/unix.IP35`, address 0xc00000000005b000, 104 bytes, no stack frame): says whether
  the Hub's I/O interface has shut down a crosstalk widget, either through the widget's own register or because the
  whole link to crosstalk has failed. It returns 1 or 0. `a0` is the NASID and `a1` the widget number. The register
  index is `a1` for a widget below 8 and `a1 - 7` otherwise (a signed compare at 0xc00000000005b000), the same
  mapping `hub_setup_prb` uses. It reads the doubleword at `0x9200000001c00000` (pool word 0xc000000001644020) + (NASID <<
  33) + 408 + 8 x index and returns 1 when any of its bits 51 to 48 (mask 15 << 48) is set. Otherwise it reads the
  doubleword at the same base + (NASID << 33) + 8 and returns 1 when bit 32 of it is set, and 0 when it is clear (the
  value 0 comes from the delay slot at 0xc00000000005b05c, the value 1 from the one at 0xc00000000005b064). The same
  bit 32 of the +8 doubleword is the one `hubii_eint_handler` reports as a failed link to crosstalk. Which fields
  bits 51 to 48 of the per-widget registers are: unknown here. Established.
- `hubni_error_handler` (`6.5.30/unix.IP35`, address 0xc000000000059064, 56 bytes, 32-byte frame): the common front end
  of the Hub network-interface error reports. `a0` is a description string (kept in `s8`) and `a1` a flag (saved at
  sp+16). It calls `get_nasid()` (0xc000000000041434), then the unnamed static routine at 0xc000000000058ea0 with
  `(a0, NASID, a1)` (the flag reloaded in the delay slot of the call), and returns its result. Established.
- `hubni_eint_handler` (`6.5.30/unix.IP35`, address 0xc00000000005909c, 36 bytes, 16-byte frame): the handler that
  `hubni_eint_init` connects at level 126. It takes no argument. It calls
  `hubni_error_handler("NI interrupt", 1)`; the string is at 0xc0000000015217f0, the pool word at gp-22872
  (0xc000000001520df0) plus 2560. It returns the result. Established.
- `hubni_rmteint_handler` (`6.5.30/unix.IP35`, address 0xc0000000000590c0, 36 bytes, 16-byte frame): the handler that
  `hubni_eint_init` connects at levels 103 and 104. It takes no argument. It calls
  `hubni_error_handler("Remote NI interrupt", 0)`; the string is at 0xc000000001521800, the same pool word plus 2576. It
  returns the result. Established.
- `kl_panic` (`6.5.30/unix.IP35`, occupies 48 bytes at address 0xc00000000005e6a8): prints a fatal message after putting the machine in panic mode.
  `a0` is the message, kept in `s8` (the move is in the delay slot of the first call). It calls `kl_panic_setup`, which runs `enter_panic_mode`,
  `check_ni_errors` (with `hubni_error_handler` when that finds an error) and `sn0_error_dump`, then calls `cmn_err(18, a0)`: the level 18 is set
  right after the first call and the message in the call's delay slot. Registers `a2` to `a7` are not reloaded between the calls,
  so any further format arguments are whatever `kl_panic_setup` leaves. After the call it restores `ra` and `s8` and returns what
  `cmn_err` leaves in `v0`. The frame is 16 bytes. No instruction is branch-likely. Established.

## What clears the NI error interrupt after a tolerated link reset

The question: when the NI interrupt handler prints "Detected a link reset due to other partition reset" (port error bit 52 set, reset enable zero), it writes nothing to the NI error word. Does the level-126 interrupt keep firing, or is it cleared or masked elsewhere? Every address is in `6.5.30/unix.IP35`. Register offsets are hub-relative; the kernel reaches them through the window at 0x9200000001e80000 (pool word 0xc000000001644160) plus the NASID shifted left 33, so the port error word is that base plus 0x100, the port error clear word plus 0x120, the reset enable word plus 0x10 and the port status word plus 0. Those four names are the Bedrock datasheet's; the kernel names no register. The routine names below are the ones in the kernel's debug information; none is in the symbol table.

- `intr_connect_level` (`6.5.30/unix.IP35`, address 0xc00000000005f308, 792 bytes): connects a handler to an interrupt level of one processor. It asks `intr_get_ptrs` (0xc00000000005f2ac, 92 bytes, read in full) to split the level: below 64 is bank 0 with the level as the bit, otherwise bank 1 with the level minus 64, so level 126 is bank 1, bit 62. It stores the handler and its argument in that bit's 72-byte slot of the per-processor vector table, writes the level (plus 64 for bank 1, bit 8 clear) to the pending-modify register of the processor's interface (hub offset 0x90, or 0x200090 when bit 1 of the processor's slice byte is set; store at 0xc00000000005f4b0) and reads offset 0x98 back, sets the bit in the processor's software mask word, and last copies that word to the bank-1 hardware mask register (store at 0xc00000000005f5e4; offset 0xb0 for the first processor of the first interface and 0xc0 for the second, from the `addiu` at 0xc00000000005f528 and the bank test at 0xc00000000005f540; the second interface's offset was not worked out). The mask bit for level 126 is therefore set once, at connect time; the write at connect time is a clear request for the pending bit, not a mask. Established.
- `intpend1` (`6.5.30/unix.IP35`, address 0xc0000000000603d0, 236 bytes): the processor-interrupt handler for the second pending bank. `cause_intr_tbl` (0xc000000001531518) slot 3, at 0xc000000001531548, holds it and slot 2 holds `intpend0` (0xc000000000060270, 352 bytes); `intr` maps Cause bit 0x800 to slot 3. It ANDs a per-processor word (absolute -0x3a58) with one read of the pending-1 register (offset 0xa0, pool word 0xc000000001644260); with nothing left it returns. Otherwise, for each set bit from the highest down, it writes the bit plus 64 to offset 0x90 (pool word 0xc000000001644258), the pending-modify register with the bank bit set and the value bit clear, so a clear; reads offset 0x98 (pool word 0xc000000001644250) and drops the result; calls the slot's handler with the slot's argument and the exception frame; and removes the bit from its local copy. A slot with no handler gets a level 0 `cmn_err` instead. It does not re-read the pending register inside the loop, never writes a mask register and never writes the NI error word. For bit 62 its only action is one clear request before the handler runs. That bank 1 raises Cause bit 0x800 is the datasheet's statement, not something this code shows. Established.
- `hubni_error_handler_nasid` (`6.5.30/unix.IP35`, address 0xc000000000058ea0, 416 bytes, no symbol-table entry): the routine behind `hubni_eint_handler` (0xc00000000005909c) and `hubni_error_handler` (0xc000000000059064). In the tolerated case (port error word masked by 0x001fffffff000000 nonzero, `part_id` zero, bit 52 set, reset enable word zero: the branch at 0xc000000000058f18) it runs `cmn_err(21, "Detected a link reset due to other partition reset")`, reads the port error clear word (stored to an 8-byte kernel bss word at 0xc000000001644c28 that no global-pointer-relative load reads back), calls `ni_port_down_reset` (which returns at once because `part_id` is zero), reads the clear word again (stored to the same word) and returns. It stores nothing to any NI register, PI register or mask. Its other loads are the port error word (0xc000000000058ebc) and the reset enable word (0xc000000000058f14). With bit 52 clear, or the reset enable nonzero, it takes the fatal path. Established.
- `check_ni_errors_nasid` (`6.5.30/unix.IP35`, address 0xc000000000058ba4, 764 bytes, no symbol-table entry): the worker behind `check_ni_errors`, reached from the processor-interface and I/O-interface error handlers, the bus-error path and `kl_panic_setup`. With `part_id` zero and bit 52 set, and the reset enable word zero, it prints the same notice (with the NASID), calls `ni_port_down_reset`, reads the clear word and then stores zero to the port error word (store at 0xc000000000058c8c), returning 0. So the claim that the kernel leaves the error word alone holds for the level-126 handler only. When no bit of 52:24 is latched it reads the port status word and tests `status & 3`: with both bits zero it reads the reset enable word, and nonzero prints "    NI PORT_STATUS 0x%x" and "    NI LLP error: Link is down, failed after reset" and returns 1, while zero prints the same notice, calls `ni_port_down_reset`, reads the clear word, stores zero to the port error word (0xc000000000058e50 to 0xc000000000058e60) and returns 0. With the status bits not both zero it tests bits 40 and 39 of the pending-1 register (3 shifted left 39, at 0xc000000000058c28) and, if either is set, prints "Another node detected a fatal NI error, NI_BRDCAST_ERR 0x%x" and returns 1. Established.
- `ni_reset_port` (`6.5.30/unix.IP35`, address 0xc000000000041534, 28 bytes) is the only store found to the reset enable word: it writes 1 there and then 6 to the port reset word at 0x680008. No store of zero to the word was found by scanning the users of the two local-window pool words (0xc000000001644010 and 0xc000000001644018) and of the remote-window base word (five routines). The word's three loads (0xc000000000058c60, 0xc000000000058e2c, 0xc000000000058f14) all test it for nonzero. Established for the routines scanned.
- **Views of the network-interface words (established for the instructions).** The offsets 0, 0x10, 0x100 and 0x120 are reached through the node-indexed alias (the window at 0x9200000001e80000 plus the NASID shifted left 33; `check_ni_errors_nasid` loads the error word at 0xc000000000058bc4 and stores zero to it at 0xc000000000058bfc, 0xc000000000058c8c and 0xc000000000058e54). The words at 0x108, 0x110 and 0x118 are read through the local alias by absolute address (0xc000000000058de8, 0xc000000000058dfc and 0xc000000000058e10) and passed to the link error report; their roles are not read. `hubni_error_handler_nasid` loads the error word (0xc000000000058ebc) and stores nothing to it, which is why the level-126 handler leaves it alone while `check_ni_errors_nasid` clears it. `defs/hubni.toml` states the two aliases; the chips repository lists the node-indexed words at a different offset from the local-alias words because it keeps an alias bit in the offset.
- `0x680010` (`BEDROCK-NI`, local alias): network interface reset enable word, the word the IP35 kernel reads before it treats a link reset as tolerated.
  The only store to it is the write of 1 in `ni_reset_port`, at 0xc000000000041534 in `6.5.30/unix.IP35`.
  Its loads at 0xc000000000058c60, 0xc000000000058e2c and 0xc000000000058f14 test it for nonzero.
  The tolerated-reset branch of `check_ni_errors_nasid` (0xc000000000058ba4) requires it to read zero.
  Established for the stores and loads. The bit-0 "reset ok" meaning is the datasheet name, inferred from that use.
- `0x680008` (`BEDROCK-NI`, local alias, port reset word): `ni_reset_port` (`6.5.30/unix.IP35`, address 0xc000000000041534) stores 6 to it, after the store of 1 to 0x680010 in the same routine. Established for the store. The effect on the link is not read in the instructions and rests on the Linux name.
- `show_ni_port_error` (`6.5.30/unix.IP35`, address 0xc00000000005600c, 156 bytes): prints one line per set bit of the port error word's bits 52:24 (it shifts the word right by 24 and walks 29 bits), using the string table `hub_ni_peerr` (0xc000000001530910, 232 bytes, 29 pointers; entry 28 is bit 52, the string "52: Detected a link reset", each string carrying its bit number). Established.

Grades for what the binary cannot settle:

Whether the level-126 interrupt keeps firing: inferred. The processor-interface datasheet says pending-1 bit 62 only reflects the network interface and cannot be changed by a pending-modify write, and the network interface datasheet says port error bits 52:24 assert that unit's interrupt. If both hold, the clear in `intpend1` does nothing, the line stays asserted until the NI error word has no bit in 52:24, and the processor takes the exception again once the handler returns. The only operation in the level-126 path that could end the assertion is the read of the port error clear word. The datasheet gives that word as read-to-clear and the port error word as non-destructive; both access classes come from public chip headers and a public port's use, not from a measurement or the chip specification, which is not held. `hub_error_clear` (0xc00000000005792c), `init_hub_stats` (0xc00000000006b38c) and `capture_hub_stats` (0xc00000000006b1cc) also read the clear word, which is the kernel's usual way to empty the error word. If the read did not clear, nothing in `intpend1`, the handler or `ni_port_down_reset` would stop the re-entry; no storm guard for level 126 was found (no counter, no mask write, no `intr_disconnect_level` call in the routines read). Why the handler reads the clear word twice: inferred. The two reads sit on either side of the call to `ni_port_down_reset`. With `part_id` zero that call returns at once, so in the shipped kernels the second read adds nothing the first did not do. With `part_id` positive (a build that links the partition code, not held) the call polls the link status, then reads the clear word at 0xc000000000058b30 and stores zero to the port error word at 0xc000000000058b40 (the same two operations again, under a test of the status word). The most direct reading is that the second read in the handler collects any error latched while the call waited for the link, and that the shipped build keeps the pair although it is idle. Unknown: whether the vendor's partitioned build behaves that way; it needs the partition-enabled kernel objects.

The 6.5.22 IP35 kernel has the same level-126 path. `hubni_eint_init` (0xc000000000058b4c), `hubni_eint_handler` (0xc00000000005999c), `hubni_error_handler` (0xc000000000059964), the handler worker (0xc0000000000597a0, 104 instructions) and the error-check worker (0xc0000000000594a4, 191 instructions) match the 6.5.30 routines instruction for instruction in opcode, register and immediate, apart from the global-pointer displacements and the call targets, and the 17 calls of the handler worker name the same routines in the same order (the tolerated-link-reset branch included, with both clear-word reads). Established by a normalised comparison of the two bodies; so the level-126 behaviour described above holds for 6.5.22 IP35.

What re-arms it: inferred, and nothing in software. The mask bit stays set from connect time, so after the clear a later link reset latches bit 52 again and asserts bit 62 again.

"Arrival permit": no register or string in the kernel carries that name. The nearest is the reset enable word (offset 0x680010, bit 0, "reset ok" in the datasheet, which says it lets a reset arriving over the link reset the chip). That mapping is inferred.

Does bit 52 latch when the reset is refused by that bit: IRIX's expectation is inferred, the hardware behaviour unknown. The kernel treats bit 52 set with the reset enable zero as a tolerated link reset and bit 52 set with it nonzero as fatal, so the tolerated branch on the interrupt path presupposes that a reset the enable word does not let through still latches bit 52. Nothing the kernel does shows what the chip latches; the datasheet names bit 52 "link reset in" and says nothing about a refusal. Test that settles it: on a running IP35 node, with the reset enable word (offset 0x680010) at zero, cause one link reset to arrive and read the port error word at the non-clearing address, then the clear word; bit 52 set means the chip latches a refused reset, clear means it does not. A document route is the node controller's network interface specification, which is not held; its table for bit 52 would answer it without a run.

Does the far node's link status drop while the other end resets: inferred, from two readings, and unmeasured. `check_ni_errors_nasid` accepts status bits 1:0 both zero, with no latched error and the reset enable zero, as a survivable partition reset, so IRIX expects that status in that situation. `ni_port_down_reset` (0xc000000000058af4), when `part_id` is positive, polls bit 1 of the status word up to 1,000 times with `us_delay(200)` between reads when `(status & 6) == 4` (bit 2 set, bit 1 clear). The datasheet gives bit 1 as link up and bit 2 as remote power, so that reads as link up dropping while the remote end resets with its power present. Neither routine says which end's status it reads or how long the link stays down. The held kernel's `part_id` always returns 0, so the second reading is structure only.

The Origin 200 hub datasheet records the 6.5.30 `unix.IP27` form of the same rule (link-up and down-reason bits both zero, protection word zero, same notice). It is a different component with a different register layout and supports the reading of IRIX's intent, nothing more.

The 6.5.22 kernels were not read for this section.


## DMA maps, error counters and the crosstalk provider

- `hub_dmamap_alloc` (`6.5.30/unix.IP35`, address 0xc00000000006274c, 128 bytes, 48-byte frame): makes the 16-byte
  record that the hub's DMA map calls use. `a0` is the device vertex (kept at sp+8) and `a3` the flags; `a1` and `a2` are
  not read. It calls `xwidget_info_get(a0)`, then `xwidget_info_id_get` on the result (saved at sp+24) and
  `xwidget_info_master_get` on the same widget info (saved at sp+16), and `kern_malloc(16)`. The memory is not cleared.
  It stores, in this order, the vertex as a word at +0, 1 as a word at +12, the master as a word at +8 and the widget
  id as a byte at +4. When bit 0 of `a3` is set it then overwrites the word at +12 with 5. It returns the record, with bytes +5 to +7 unwritten. Word +12 is a flag word: bit 0 is set here and cleared by
  `hub_dmamap_free`, bit 1 (value 2) is set by `hub_dmamap_addr` and `hub_dmamap_list` and cleared by
  `hub_dmamap_done`, and bit 2 (value 4), which makes those routines accept a second use, is set here only through the
  flag bit. Established.
- `hub_dmamap_free` (`6.5.30/unix.IP35`, address 0xc0000000000627cc, 40 bytes, 16-byte frame): releases a record from
  `hub_dmamap_alloc`. It clears bit 0 of the word at +12 of `a0` (the store is in the delay slot of the call) and
  calls `kern_free(a0)` with no size argument set. It returns what `kern_free` leaves in `v0`. Established.
- `hub_dmamap_addr` (`6.5.30/unix.IP35`, address 0xc0000000000627f4, 92 bytes, 16-byte frame): marks a DMA map record
  as in use and hands back the bus address unchanged. `a0` is the record and `a1` the address, which is returned in every
  case (it is copied to `v0` before the first branch). It reads the flag word at +12. When bit 1 (value 2) is clear it
  stores the word with that bit set (in the delay slot of the branch at 0xc000000000062828) and returns. When bit 1 is
  set and bit 2 (value 4) is also set it returns without a store. When bit 1 is set and bit 2 is clear it calls
  `cmn_err(4, "%v: hub_dmamap_addr re-uses dmamap.\n", word at +0)` (the string at 0xc000000001524f08, the pool word at
  gp-22568 plus 16) and returns. Established.
- `hub_dmamap_list` (`6.5.30/unix.IP35`, address 0xc000000000062850, 92 bytes, 16-byte frame): the same routine as
  `hub_dmamap_addr` with one difference: its message is "%v: hub_dmamap_list re-uses dmamap\n" (no full stop), the
  string at 0xc000000001524f30 (the pool word at gp-22568 plus 56). The flag handling, the register use and the return of
  `a1` are the same. Established.
- `hub_dmamap_done` (`6.5.30/unix.IP35`, address 0xc0000000000628ac, 80 bytes, 16-byte frame): ends a DMA map use.
  `a0` is the record. When bit 1 (value 2) of the flag word at +12 is set it stores the word with that bit cleared (mask
  -3) and returns. When bit 1 is clear and bit 2 (value 4) is set it returns without a store. When both are clear it
  calls `cmn_err(4, "%v: hub_dmamap_done already done with dmamap\n", word at +0)` (the string at 0xc000000001524f58,
  pool word at gp-22568 plus 96) and returns. It has no return value. Established.
- `hub_error_count_update` (`6.5.30/unix.IP35`, address 0xc00000000005b4f0, 116 bytes, 32-byte frame): the periodic
  timeout routine that samples one node's link error counters. `a0` is a NASID (kept at sp+0). The compact node is the
  halfword at `nasid_to_compact_node` + 2 x NASID (pool word at gp-22808, a 256-byte table), with no range test and no
  test for the -1 that means no node. The node's count block is the doubleword at `hub_err_count` (0xc000000001650990,
  which is `kl_error_log` (pool word at gp-22864) plus 1024) + 8 x compact node. It calls
  `ii_llp_error_monitor(block + 8)` and `ni_llp_error_monitor(block + 32)`. When the word at block +0 is nonzero it
  calls `timeout(hub_error_count_update, a0, 100)`, the function address being the pool word at gp-22776, so the
  routine reschedules itself after 100 ticks until the word is cleared. It returns nothing. Established.
- `hub_error_count_disable` (`6.5.30/unix.IP35`, address 0xc00000000005b610, 84 bytes, no stack frame): stops the
  counting. It applies the same range test and the same compact-node lookup as `hub_error_count_enable` and returns -1
  for a NASID of 128 or more, a negative one or an unmapped one. Otherwise it stores 0 in the word at +0 of the node's
  count block and returns 0; the running timeout then ends at its next pass, because
  `hub_error_count_update` reschedules only while that word is nonzero. It makes no calls. Established.
- `hub_error_count_get` (`6.5.30/unix.IP35`, address 0xc00000000005b664, 88 bytes, no stack frame): returns a node's
  count block. `a0` is a NASID and `a1` points to a word. For a negative or 128-or-more NASID, or an unmapped one, it
  returns 0 and leaves `*a1` alone. Otherwise it stores 64 in the word at `a1` and returns the doubleword in
  `hub_err_count` for the compact node, a pointer to the 64-byte block. Established.
- `hub_xtalk_link_up` (`6.5.30/unix.IP35`, address 0xc00000000006afd0, 40 bytes, no stack frame): says whether the
  crosstalk link of a node is up. `a0` is a NASID. It returns 1 when bit 13 (0x2000) of the doubleword at 0x9200000001c00000
  (pool word at gp+2224) plus the NASID shifted left 33 plus 296 is set, and 0 otherwise. Which hub register that is:
  unknown here. Established.
- `hub_provider_shutdown` (`6.5.30/unix.IP35`, address 0xc000000000062d7c, 28 bytes, 16-byte frame): calls
  `xtalk_provider_unregister` with its own argument unchanged and returns what that call leaves in `v0`. Established.
- `hub_check_is_widget0` (`6.5.30/unix.IP35`, address 0xc000000000062d98, 84 bytes, no stack frame): says whether the
  address `a0` lies in widget 0's window of a hub's uncached space. The NASID is bits 39:33 of the address (a shift right
  by 33 and a mask of 0x7f); the window is [0x9200000000000000 + NASID shifted left 33, the pool word 0x9200000001000000
  (gp+2480) + NASID shifted left 33), so 16 megabytes. The routine returns 1 only when the address is at or above the lower
  bound and below the upper one, and 0 otherwise. Established.
- `hub_dmatrans_addr` (`6.5.30/unix.IP35`, address 0xc0000000000628fc, 8 bytes, no stack frame): returns its third
  argument (`a2`), copied to `v0` in the delay slot of the return. Nothing else is read or written, so a direct-translation
  DMA address passes through as given. `hub_dmatrans_list` at 0xc000000000062904 is the same two instructions. Established.
- `hub_provider_startup` (`6.5.30/unix.IP35`, address 0xc000000000062d48, 52 bytes, 16-byte frame): runs the hub's
  start-up steps for a crosstalk provider. It keeps `a0` in `s0` and calls three unnamed static routines with it, in
  order: 0xc0000000000620b4, 0xc000000000062744 and 0xc000000000062924. The last two are each a bare `jr ra` with a
  `nop` delay slot. The first is a long routine that the symbol table folds into `hubio_init`; it reads the "_node"
  label of the vertex, fills per-node and per-widget tables, calls `hub_set_piomode(.., 1)`, then `init_sv` and
  `spinlock_init` on addresses 784 and 792 bytes into the node record. That routine was read only in part, so what the
  tables hold is not stated here. This routine returns what the last call leaves in `v0`. Inferred.
- `hub_dmamap_drain` (`6.5.30/unix.IP35`, address 0xc00000000006290c, 8 bytes, no stack frame): the hub's DMA map drain
  entry for a crosstalk provider. The body is `jr ra` with a `nop` delay slot. It reads no argument, writes nothing and
  leaves `v0` as the caller had it. Established.
- `hub_dmaaddr_drain` (`6.5.30/unix.IP35`, address 0xc000000000062914, 8 bytes, no stack frame): the address-range drain
  entry of the same provider, with the same body: `jr ra` and a `nop` delay slot, no argument read, no store, `v0`
  untouched. Established.
- `hub_dmalist_drain` (`6.5.30/unix.IP35`, address 0xc00000000006291c, 8 bytes, no stack frame): the list drain entry of
  the same provider, with the same body: `jr ra` and a `nop` delay slot, no argument read, no store, `v0` untouched.
  Established.
- `hub_dmatrans_list` (`6.5.30/unix.IP35`, address 0xc000000000062904, 8 bytes, no stack frame): the direct-translation
  list entry of the same provider. Its two instructions are `jr ra` and, in the delay slot, `or v0,a2,zero`, so it
  returns its third argument (`a2`) and reads and writes nothing else. It is the same code as `hub_dmatrans_addr`.
  Established.
- `xtalk_piotrans_addr` (`6.5.30/unix.IP35`, occupies 28 bytes at address 0xc0000000000970e8): the crosstalk entry point for translating a programmed-I/O address; it calls `hub_piotrans_addr` and nothing else.
  A 16-byte frame holds only `ra`. The arguments reach `hub_piotrans_addr` untouched, and its result comes back as this routine's. The only branch is the call, whose delay slot is a `nop`; no instruction is branch-likely. Established.
- `xtalk_pio_addr` (`6.5.30/unix.IP35`, occupies 248 bytes at address 0xc000000000097104): returns a processor-side address for a crosstalk range, using a direct translation when there is one and a map object otherwise.
  The six arguments are `a0` to `a3` (passed on), `a4` a pointer to a map-object slot that may be null (`s2`) and `a5` the flags (`s3`). When `a4` is nonzero it stores 0 there first. It calls
  `xtalk_piotrans_addr(a0, a1, a2, a3, a5)` and returns that result when it is nonzero. When it is zero it calls
  `xtalk_piomap_alloc(a0, a1, a2, a3, a3, a5)` (the length is passed twice) and, if that gives zero, returns 0. Otherwise it calls `xtalk_piomap_addr(map, a2, a3)`. A zero result there makes it call
  `xtalk_piomap_free(map)` and return 0; a nonzero result is stored in `*a4` when `a4` is nonzero and returned. A map object is never left in `*a4` unless the address was obtained. The frame is 64 bytes. No instruction is branch-likely. Established.

## Hub special-device entry points

- `hubspc_open` (`6.5.30/unix.IP35`, address 0xc000000000096e20, 116 bytes, 48-byte frame): open routine of the hub
  special device, which serves two kinds of node depending on the device information recorded for its vertex. `a0`
  points to a device number word, and `a1` to `a3` are passed on unchanged (all four are saved at sp+0 to sp+24).
  It calls `device_info_get` with the word at `a0`, truncates the result to 32 bits and acts on it. Value 0: it returns
  what `mem_refcnt_open(a0, a1, a2, a3)` returns. Value 1: it calls `cap_able` with the mask 1 shifted left 37
  (`dsll32` of 1 by 5, 0x2000000000) and returns 1 when that call returns 0 and 0 when it returns nonzero. Any other
  value returns 19. Established.
- `hubspc_close` (`6.5.30/unix.IP35`, address 0xc000000000096e94, 108 bytes, 48-byte frame): close routine of the same
  device. It calls `device_info_get` with the word at `a0` and truncates the result to 32 bits. Value 0: it returns what
  `mem_refcnt_close(a0, a1, a2, a3)` returns. Value 1: it returns 0 without any call. Any other value returns 19.
  Established.
- `hubspc_init` (`6.5.30/unix.IP35`, address 0xc000000000096de4, 60 bytes, 16-byte frame): start-up of the hub special
  device. It takes no argument. It calls `hubdev_register` three times, each time with a routine address that it loads
  from a pool word into `a0` in the call's delay slot, in this order: `mem_refcnt_attach` (0xc000000000329c60, pool word
  at gp-20720), `cpuprom_attach` (0xc000000000096c60, gp-20712) and `l1_attach` (0xc000000000228180, gp-20704). It then
  calls `spinlock_init` with the address gp+5712 (0xc000000001644dc0, the variable `cpuprom_spinlock`) in `a0` and, in
  `a1` the address 0xc000000000499a70, which is the block address from the pool word at gp-20736
  (0xc000000000499a50) plus 32 and holds "promlist" (the delay slot of the call does the addition). It returns what
  `spinlock_init` leaves in `v0`. It does not call the three routines itself; it only hands them to `hubdev_register`. Established.
- `hubspc_map` (`6.5.30/unix.IP35`, address 0xc000000000096f00, 156 bytes, 48-byte frame): map entry point of the same
  device, which serves two kinds of node. `a0` is passed to `device_info_get`, and `a1` to `a4` are saved at sp+8, sp+16,
  sp+24 and sp+0. When `a3` is zero it returns 6 at once, before any call (`v0` is set to 6 at 0xc000000000096f14, ahead
  of the branch). Otherwise it calls `device_info_get(a0)` twice, the first result discarded (the second call's
  delay slot copies `a0` from `s0`, where the routine kept it), and truncates the second result to 32 bits. Value 0: it calls
  `mem_refcnt_mmap(a0, a1, a2, a3, a4)` with the saved arguments and returns what it returns. Value 1: it calls
  `cpuprom_map(a0, a1, a2, a3)` (0xc000000000096d20) with the first four and returns what it returns (`a4` is not
  passed). Any other value returns 19. Established.
- `hubspc_unmap` (`6.5.30/unix.IP35`, address 0xc000000000096f9c, 96 bytes, 32-byte frame): unmap entry point of the
  same device. `a0` and `a1` are saved at sp+0 and sp+8. It calls `device_info_get(a0)` and truncates the result to 32
  bits. Value 0: it calls `mem_refcnt_unmap(a0, a1)` and returns its result. Value 1: it calls `cpuprom_unmap(a0, a1)`
  (0xc000000000096ddc) and returns its result. Any other value returns 19. Established.
- `hubspc_ioctl` (`6.5.30/unix.IP35`, address 0xc000000000096ffc, 124 bytes, 64-byte frame): control entry point of the
  same device. It saves `a0` to `a5` (a0 at sp+0, a5 at sp+8, a4 at sp+16, a1 at sp+24, a2 at sp+32, a3 at sp+40) and
  calls `device_info_get(a0)`, truncating the result to 32 bits. Value 0: it calls
  `mem_refcnt_ioctl(a0, a1, a2, a3, a4, a5)` with all six saved arguments and returns its result. Value 1: it
  returns 0 without calling anything (the branch at 0xc000000000097030 reaches the instruction that zeroes `a0`, which
  `v0` then copies). Any other value returns 19. Established.

### The same routines in the 6.5.22 builds

The 6.5.22 copies of the routines above. Each row is a routine whose bytes match a 6.5.30 routine on the same page; the note gives the SHA-256 prefix of the bytes (first 12 hex digits). A linked-kernel row is established when the whole body has no calls, address constants or gp accesses; the others are inferred, because the 6.5.22 target of each constant or call was not checked. An object row compares section bytes, not relocated code, so it is inferred. The comparison is in [version-diffs.md](version-diffs.md).

| Routine | Binary | Address | Size | Note | Label |
|---|---|---|---|---|---|
| `hub_dmatrans_addr` | `6.5.22/unix.IP35` | `0xc00000000006315c` | 8 | returns its third argument (`a2`), copied to `v0` in the delay slot of the return; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 0e2e26765928), at a different address; no calls, address constants or gp accesses in the body | Established |
