# PCI bridge driver routines of the IP35 kernel

Routines of the PCI bridge driver (`pcibr`) and the crosstalk bridge attach code in the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`). The kernel is a linked 64-bit image with DWARF, so structure member names below come from its debug information; the same code, byte for byte, is in `usr/cpu/sysgen/IP35boot/pcibr.o`. "soft +N" is byte N of the per-bridge software state (`struct pcibr_soft_s`, 2752 bytes). Its members used here: +0 `bs_conn`, +4 `bs_vhdl`, +8 `bs_base` (the bridge register window), +16 `bs_name`, +24 `bs_asic_name`, +40 `bs_xid`, +72 `bs_busnum`, +114 `bs_bridge_type`, +116 `bs_bridge_mode`, +264 `bs_slot[]` (128-byte slots), +1648 `bs_intr[]`, +2104 `bs_errinfo`. `bs_bridge_type` is a halfword that the register accessors (`pcireg_*`) switch on: `xbridge_attach` stores 0 for a widget with part number 0xc002 and manufacturer 54 and 1 for part number 0xd002 and manufacturer 36; 2 is stored by another attach routine, not read here, and is inferred to be the PIC from the provider table named `pci_pic_provider`; bit 0 of the halfword `bs_bridge_mode` selects the PCI-X register set in the error code.

## Error reporting

- `pcibr_error_dump` (`6.5.30/unix.IP35`, address 0xc0000000000a5fdc, 4268 bytes): prints the bridge registers that
  explain the error bits currently set in the interrupt status. It reads the status with `pcireg_intr_status_get` and
  keeps bits 8 and up (mask 0xffffffffffffff00); with none set it returns. It then swaps that masked value into the
  64-bit location whose address is soft + 2120 (`bs_errinfo.bserr_intstat`) with `test_and_set_int64` (a load-linked
  and store-conditional exchange, address 0xc000000000069ea0) and returns when the old value equals the new one, so
  the same set of bits is dumped once. Otherwise it calls `cmn_err` (level 1) with the text "PCI BRIDGE ERROR:
  int_status is 0x%lx for %s / Dumping relevant %s registers for each bit set", the masked status, the bridge name
  (soft +16) and the ASIC name (address soft +24). It then walks bit numbers 8 to 63. For a set bit it prints, with
  `cmn_err` level 6, the line of the 64-entry string table whose base is the pointer in the pool at gp-20304
  (0xc000000001559400) plus 1920 ("08: GIO non-contiguous byte enable in crosstalk packet" through "45: PCI-X split
  completion message parity error"; bits 0 to 7 hold an empty string), and then dumps registers by group. Bits 8, 10,
  11, 12, 13,
  14 and 15 print the PCI error address pair (`pcireg_pci_bus_addr_get` and
  `pcireg_pci_bus_addr_addr_get`); bit 9 prints the response buffer error (`pcireg_resp_err_get`,
  `pcireg_resp_err_dev_get`, `pcireg_resp_err_buf_get`); bit 16 prints the SSRAM parity register
  (`pcireg_ssram_parity_get`) on type 0 only; bits 23 to 25 and 27 print the command word (`pcireg_cmdword_err_get`,
  through the helper `print_bridge_errcmd` at 0xc0000000000a5fa8) and the bridge error address (`pcireg_bus_err_get`
  twice), except that bit 22 and, on type 2, bit 23 print the link-side error word (`pcireg_linkside_err_get`) and
  address (`pcireg_linkside_err_addr_get`); bits 26 and 28 print the response buffer error when the mode bit is clear
  and the PCI-X request error pair (`pcireg_pcix_req_err_addr_get`, `pcireg_pcix_req_err_attr_get`) when it is set,
  and bit 26 also calls `pcibr_show_dir_state`; bit 29 prints the link-side word and, in PCI-X mode, the request error
  pair; bit 30 prints the SSRAM parity register on type 0 and `pcireg_map_fault_get` otherwise; bits 32 to 38 and 42
  print the PCI-X bus error address, attribute and data registers; bit 39 the PCI-X request error pair; bit 40
  `pcireg_arbitration_get`; bit 41 `pcireg_parity_err_get`; bits 43 to 45 the PCI-X split request address and
  attribute pair (`pcireg_pcix_pio_split_addr_get`, `pcireg_pcix_pio_split_attr_get`) followed by the bus error
  triple. On type 1 and 2 bits 8, 16 and 31 are skipped, and on type 2 with a nonzero `bs_busnum` bits 17 to 21 and 23
  are skipped. The bit-to-group assignment is read from the compare tree of the decompiled body and checked against
  the register accessors the instructions call, not stepped bit by bit through the object code. The routine returns
  nothing; its frame is 176 bytes. Inferred.

## Slot probing

- `pcibr_slot_info_init` (`6.5.30/unix.IP35`, address 0xc0000000000a9c68, 2868 bytes, 624-byte frame): discovers the
  card in one PCI slot, creates an info record for each of its functions and assigns bus address windows to every base
  address register. `a0` is the bridge vertex and `a1` the slot number. It takes the soft state from
  `hwgraph_fastinfo_get(a0)` and returns 22 when that is null (0xc0000000000a9c98) or when the slot is above the
  highest slot (byte soft +57; address 0xc0000000000a9cf0), 0 when the slot's `has_host` word (`bs_slot[slot]`, first
  word) is nonzero (0xc0000000000a9cc4), 1 when `pcibr_is_slot_sys_critical` says the slot belongs to the system
  (0xc0000000000a9d0c), and 19 when `pcibr_probe_slot` on the slot's configuration address (`pcibr_slot_config_addr`
  with function 0) fails (0xc0000000000a9d44) or the vendor halfword reads 0xffff (0xc0000000000aa53c). A present card
  sets bit 0x10 of the slot status word (`bs_slot[slot]` +12). It reads the header type byte (configuration offset
  0xe); with bit 7 set it probes functions 1 to 7 through `pcibr_func_config_addr` and `pcibr_probe_slot`, noting
  absent ones in a bitmask and counting up to the last present function. `kmem_zalloc` gives an 8-byte-per-function
  array that is stored in `bs_slot[slot]` +24 with the count in +16. For each present function it picks 6 base address
  registers for header type 0 and 2 otherwise (a header type other than 0 and
  1 goes to `cmn_err` level 4 and is treated as 2), and for the SGI vendor ID 0x10a9 with device ID 2 it calls
  `pciio_device_win_populate` once for the memory window map (soft +2160, flagged by bit 0 of the word at soft +2176)
  and once for the I/O window map (soft +2128, bit 1), each of 0x1000000 bytes based at the crosstalk widget number
  shifted left 24. When the latency timer (configuration offset 0xd) is zero, the slot's device register
  (`pcireg_device_get`) lacks bit 0x200000 and the card is not an SGI 0x10a9 device 2, 3 or
  5, it writes the timer using the minimum-grant byte (offset 0x3e) times a unit that depends on the halfword at soft
  +116 (0x20 for 7, 0x18 for 5,
  0x10 for 2 or 3, 8 otherwise), falling back to four units when the product would reach 0x100 or the grant is zero.
  When bit 0 of that same halfword is set (PCI-X) it sets a zero cache-line byte (offset 0xc) to 0x20 and looks for
  the PCI-X capability (identifier 7, `pcibr_find_capability`); a function without it is reported with `cmn_err` level
  4, given an empty record by `pcibr_device_info_new(soft, slot, 0xff, -1, -1)` and skipped, while one with it has its
  capability pointer stored at record +544 and adds to the PCI-X function count (soft +1296) and the split-transaction
  total (soft +1300, from a table indexed by three bits of the capability). Each live function gets its record from
  `pcibr_device_info_new(soft, slot, function or 0xff, vendor, device)` and `pciio_device_info_register`; the handle
  from function 0 is saved at `bs_slot[slot]` +8. Then each base address register is sized by writing all ones and
  reading back, a 64-bit one taking two registers and having its upper half zeroed, and a window is allocated with
  `pcibr_bus_addr_alloc` (space 2 for I/O, 4 for 32-bit memory, 5 for 64-bit memory) and recorded in the record's
  56-byte window entries at +72. SGI vendor, device 3, stops after its first window. The routine was read through its
  decompiled body and the fact sheet's callee list; the arithmetic of the latency choice and the window sizing was not
  re-derived from instructions one by one. Inferred.
- `pcibr_pioerror` (`6.5.30/unix.IP35`, address 0xc0000000000a7be4, 3408 bytes, 208-byte frame): handles a crosstalk
  programmed-I/O error that the bridge reported, by turning the failing crosstalk address back into a PCI bus access
  and offering it to the owning driver. `a0` is the bridge soft state, `a1` an opaque handler argument passed through
  unchanged, `a2` the error mode (0, 1 or
  2 are tested) and `a3` the error record, whose first word is a flag word and whose doubleword at +24 is the failing
  crosstalk offset. It logs the soft state and offset through `pcibr_debug` (mask 0x200), then decodes the offset by
  region, using one set of bounds when `bs_busnum` (soft +72) is zero and another when it is nonzero (on type 2 the
  first slot's window starts 0x1000 later): the 4-kilobyte configuration windows (slot is the offset above the window
  base divided by 0x1000, the remainder is the configuration offset, space 7) with a final window just above them
  taken as configuration space 7 whose offset is the distance from its base; the 1-megabyte device windows (index from
  bits 20 and up of the offset, with indexes 1 and 3 adding a further 0x100000 and the later ones moving down by one
  or two, then either the slot's mapped base and space from its window entry or, for an unmapped window, space 4 or 2
  depending on bit 0x1000 of the device word); and the large direct-map ranges (0x40000000 to 0x7fffffff, 0x80000000
  up to a limit held in the pool at gp+0x6dc0, and, for the other bus layout, 0x100000000 to 0x1ffffffff), each giving
  a PCI memory or I/O space number 2, 5 or 6 and an offset. When the offset decodes to no slot it walks slots from the
  first to the last (bytes soft +56 and +57), their functions (`bs_slot[n]` +280 count and +288 info array), and each
  function's six window entries at record +72, looking for a base and size that contain the decoded offset with a
  compatible space, and failing that walks each function's list of allocated pieces (record +528 and its `+56` chain)
  and counts a hit in the piece's word at +72. The result is stored into the error record: the space number at +32 and
  the offset at +40 (both written only when a space was found), the slot and function packed as `slot << 3 | function`
  into the halfword at +8 with flag bit 0x20000000 when a slot was identified, and flag bits 0xc00000 in the first
  word. With `a2` zero it calls `pcibr_error_cleanup` and, when a slot was identified, returns what
  `pciio_error_handler` returns for it, otherwise 0. With `a2` nonzero and no space found it logs "XIO Bus Error ...
  does not correspond to any PCI address" and returns 7. Otherwise it returns 0 when `pciio_error_handler` returns 0;
  when that returns 9 it logs "PIO Error on PCI Bus" at level 1, prints the decoded address, treats the result as 10,
  and (when the halfword at gp-32752 is nonzero) dumps the raw space, address, device and function through `cmn_err`
  and runs `pcibr_error_dump` if the interrupt status has bits in the mask at gp+0x6e18; if bit 0x20000000 of the
  first word was set it then calls `pcibr_device_disable` on the slot, and when `a2` is 2 it ends with
  `pcibr_error_cleanup`. Any other nonzero handler result is returned unchanged by the same tail. The decode
  arithmetic is read from the decompiled body; individual comparison constants were checked against the disassembly
  only where quoted. Inferred.
- `pcibr_slot_device_init` (`6.5.30/unix.IP35`, address 0xc0000000000ab2d8, 244 bytes, 48-byte frame): sets one slot's
  bridge device register to its start-up value and keeps a copy in the soft state. `a0` is the bridge vertex and `a1`
  the slot number. `hwgraph_fastinfo_get(a0)` (0xc000000000341af8) gives the soft state; a null result returns 22. The
  slot is accepted when it is below the byte at +57 plus one (a signed compare at 0xc0000000000ab308, so no lower
  bound); otherwise the routine also returns 22. `pcireg_device_get(soft, slot)` (0xc0000000000b3fc4) gives the current
  register value and the bit 0x08000000 is cleared from it. The halfword at soft +114 then selects the bits to add: when
  it equals 2 and the low four bits of the word at +120 equal 1 (the code tests bit 1 of 1 shifted left by that
  nibble), the value gets 0x00010000 only; for any other nibble with halfword 2, and for any other halfword value, it
  gets 0x02010000. The result is stored as a doubleword at soft + 320 + (slot << 7), written to the register with
  `pcireg_device_set(soft, slot, value)` (0xc0000000000b41e4) and logged with `pcibr_debug` (0xc0000000000a1978) using the mask
  0x4000000, the vertex, "pcibr_slot_device_init: Device(%d): %R\n" (0xc00000000152d168), the slot, the value and the
  table `device_bits` (0xc000000001559888, 760 bytes). Returns 0 (set at 0xc0000000000ab3a0); the store, the
  register write and the log happen in that order. Established.
- `pcibr_release_device` (`6.5.30/unix.IP35`, address 0xc00000000009db64, 176 bytes, 48-byte frame): drops one
  reference on a per-slot resource count in the bridge soft state, chosen by a resource code. `a0` is the soft state,
  `a1` the slot and `a2` the code. It takes the lock at soft +140 with `mutex_spinlock` (0xc000000000331f6c) and then
  tests the code in this order, with the slot's record at soft + 128 x slot: a code of 0x1100000 subtracts 1 from the
  word at record +332, 0x8f8000 subtracts 1 from the word at record +336, and 0x898000 subtracts 1 from the word at
  record +340. The subtraction is a plain load, add and store under the lock, and the checks are independent, so a
  code that matches none changes nothing. It releases the lock with `mutex_io_spinunlock(lock, the level
  `mutex_spinlock` returned)` (0xc000000000331e84) and returns what that call returns. The slot number is not range
  checked. Established.
- `pcibr_func_config_addr` (`6.5.30/unix.IP35`, address 0xc0000000000a8e30, 132 bytes, 32-byte frame): forms the
  address through which one PCI configuration register of one function can be reached, choosing the configuration
  cycle type from the bus number. `a0` is the bridge soft state, `a1` the bus number, `a2` the device (slot), `a3`
  the function and `a4` the register. The compare at 0xc0000000000a8e54 is signed: when `a1` is zero or negative it returns
  `pcireg_type0_cfg_addr(soft, device, function, register)` (0xc0000000000b5e5c). When `a1` is positive it first calls
  `pcireg_type1_cntr_set(soft, (bus << 16) | (device << 11))` (0xc0000000000b0b68) and then returns
  `pcireg_type1_cfg_addr(soft, function, register)` (0xc0000000000b610c). The callee's result is returned unchanged on
  both paths (single exit 0xc0000000000a8e94). The register layout behind the two callees is unknown here. Established.
- `pcibr_device_flags_set` (`6.5.30/unix.IP35`, address 0xc0000000000a16bc, 512 bytes, 80-byte frame): changes
  selected bits of one device's bridge device register according to a flag word, under the bridge lock, and keeps the
  copy of the register in the soft state. It always returns 1. `a0` is the connection vertex and `a1` the flag word
  (`s2`). The slot is the byte at +540 of the record from `pciio_hostinfo_get(a0)` (0xc0000000000c1124), and the soft
  state is `pciio_info_mfast_get` (0xc0000000000c122c) of that record. From the flag word it builds a mask of bits to
  set (`s0`) and a mask of bits to clear (`s1`): flag bit 16 sets 0x1800000 and bit 17 clears 0x1800000; bit 18 sets
  and bit 19 clears 0x40000; bit 20 sets and bit 21 clears 0x20000; bit 22 sets and bit 23 clears 0x8000; bit 26 sets
  and bit 27 clears 0x400000. When the halfword at soft +114 equals 2 and flag bit 26 is set it removes 0x400000 from
  the set mask; a further test of the word at +120 (1 shifted left by its low four bits) cannot be false, so the nibble
  has no effect. With both masks zero it returns 1 at once (the branch at 0xc0000000000a17cc). Otherwise it takes the lock at
  soft +140 with `mutex_spinlock` (0xc000000000331f6c), reads the doubleword at soft + 128 x slot + 320 and forms
  `(old & ~clear) | set`. Only when that differs from the old value does it call `pcireg_device_set(soft, slot,
  new)` (0xc0000000000b41e4) and `pcireg_tflush_get(soft)` (0xc0000000000ae584), storing the new value at soft + 128 x slot +
  320 in the delay slot of the second call. It releases the lock with `mutex_io_spinunlock` (0xc000000000331e84) and
  calls `pcibr_debug(0x4000000, vertex, "pcibr_device_flags_set: Device(%d): %R\n", slot, new value, device_bits)`
  (0xc0000000000a1978; the string is at 0xc00000000152a9f0 and the table at 0xc000000001559888), which runs even when the
  value did not change. What the register bits 0x1800000, 0x40000, 0x20000, 0x8000 and 0x400000 do in the bridge:
  unknown here. Established.
- `pcibr_endian_set` (`6.5.30/unix.IP35`, address 0xc0000000000a1430, 232 bytes, 80-byte frame): sets or clears the
  bit pair 0x180000 (bits 19 and 20) in one slot's bridge device register. `a0` is the PCI connection
  vertex and `a1` and `a2` are two values compared for equality; `pciio_endian_set` calls it. It returns `a2`
  unchanged on every path (single exit 0xc0000000000a14e8, `move v0,s2` at 0xc0000000000a14e0). It gets the PCI
  information record with `pciio_hostinfo_get(a0)`, takes the slot number from the byte at record +540, gets the
  bridge soft state with `pciio_info_mfast_get(record)` and takes the lock at soft +140 with `mutex_spinlock`, keeping
  the returned level at 56(sp). The current register copy is the doubleword at soft + 128 x slot + 320. When `a1`
  equals `a2` the new value is the copy with 0x180000 cleared; otherwise it is the copy with 0x180000 set. If the new
  value equals the copy nothing is written. If it differs, the routine calls `pcireg_device_set(soft, slot, new)`
  then `pcireg_tflush_get(soft)`, and stores the new value as the copy (the store sits in the delay slot of the
  `pcireg_tflush_get` call, so it runs before the callee starts). In both cases it then releases the lock with
  `mutex_io_spinunlock(soft + 140, level)` and last calls `pcibr_debug(0x4000000, a0, "pcibr_endian_set:
  Device(%d): %R\n", slot, new value, device_bits)` (string at 0xc00000000152a9c8, decode table at
  0xc000000001559888). Which device-register field the bit pair belongs to is unknown here; the routine only
  compares `a1` and `a2`. Established.

## Read response buffers

- `pcibr_alloc_all_rrbs` (`6.5.30/unix.IP35`, address 0xc0000000000a3b60, 980 bytes, 240-byte frame): sets up the read
  response buffers (RRBs) of one half of a bridge in a single register write, from a table of per-channel buffer
  counts. `a0` is the bridge vertex and `a1` selects the register half (only bit 0 is used). The four channel totals
  are `a2`, `a4`, `a6` and the ninth argument (the word at 244(sp) on entry); the part of each total that is counted
  separately is `a3`, `a5`, `a7` and the tenth argument (252(sp)). It finds the bridge soft state by
  `hwgraph_traverse(a0, "pci", ...)` (the string at 0xc00000000152b050), reading the child's fast info with
  `hwgraph_fastinfo_get`, dropping the reference with `hwgraph_vertex_unref` and taking the register window from soft
  +8; when the traverse fails or gives no window it calls `xtalk_piotrans_addr(a0, 0, 0, 8, 0)` for the widget
  registers and has no soft state. It reads the widget identification word at window +4 and accepts a part number
  (bits 27:12) of 0xc002 with manufacturer (bits 11:1) 54, or 0xd002 with manufacturer 36; for any other widget it
  goes on only when the third and fourth totals and their separate counts are all zero. It returns -1 (address
  0xc0000000000a3c4c, or 0xc0000000000a3ee0 and 0xc0000000000a3ee8) when that test fails, when the four totals add to
  more than 8, or when any total or any of the first four of `a2`, `a4`, `a6` and the ninth argument is negative;
  otherwise it returns 0 (0xc0000000000a3e80). It builds a 32-bit word of up to eight 4-bit fields, filled from bit 28
  downward: for channel c from 0 to 3, first as many fields as the channel's separate count, each valued 8 | c | (1 <<
  k), then (total minus separate count) fields valued 8 | c, where k is 2 for the 0xc002 and 0xd002 widgets and 1 for
  any other. It takes the soft state's lock at soft +140 with `mutex_spinlock` when there is soft state, calls
  `pcireg_rrb_set(window, a1 & 1, word)`, and under the lock ORs 0x55 shifted left by the half number into the word at
  soft +1316 (`bs_rrb_fixed`), stores 8 minus the grand total at soft +1320 + 4 x half (`bs_rrb_avail`), zeros the
  four words at soft +1328, +1336, +1344 and +1352 each plus 4 x half (`bs_rrb_res`), and writes, at +1392 + 16 x half +
  32 x channel, the pair (total minus separate count, separate count) for each channel (`bs_rrb_valid`; channel 3
  lands at +1488 and +1492). It releases with `mutex_io_spinunlock`. What the nibble bits mean in the bridge register
  is not named by this code: it shows only that bit 3 is always set, the channel number fills the low bits and the
  separately counted fields carry one more bit. Established.
- `pcibr_dmard_error` (`6.5.30/unix.IP35`, address 0xc0000000000a8934, 296 bytes, 80-byte frame): handles a DMA read
  error reported by the bridge: fills in the error record, passes it to the PCI error handler, disables the offending
  device when the handler asks for that, resets the read-response buffer that took the error and returns the handler's
  result. `a0` is the bridge soft state (the word at +4 is its vertex), `a1` and `a2` are passed through to the handler
  and `a3` is the error record. It reads the failing buffer number with `pcireg_resp_err_buf_get(soft)`
  (0xc0000000000aeb68, kept at 16(sp)) and the failing device with `pcireg_resp_err_dev_get(soft)` (0xc0000000000aecdc);
  it stores the device number shifted left by 3 as the halfword at record +8 and sets bit 0x20000000 in the word at record
  +0. It then stores `pcireg_resp_err_get(soft)` (0xc0000000000ae878) as the doubleword at record +40 and sets bit
  0x400000 in the word at +0, and calls `pciio_error_handler(vertex, a1, a2, record)` (0xc0000000000c0c48), keeping the
  result at 64(sp). A nonzero result calls `pcibr_device_disable(soft, (the halfword at record +8 >> 3) & 0x1f)`
  (0xc0000000000a7bdc). It always then calls `pcireg_intr_reset_set(soft, 16)` (0xc0000000000b16c8). With `b` the buffer
  number as a sign-extended word, it reads `pcireg_rrb_get(soft, b & 1)` (0xc0000000000b4ac4), keeps the value at 32(sp),
  clears in it the four bits starting at bit 4 x (b >> 1) (mask `~(15 << (4 x (b >> 1)))`) and writes that with
  `pcireg_rrb_set(soft, b & 1, value)` (0xc0000000000b4cd4), then writes the saved value back with a second
  `pcireg_rrb_set`. It returns the handler's result from 64(sp) (single exit 0xc0000000000a8a54). The hardware state of the
  read-response buffer that the clear and restore touches: unknown here. Established.

## Bridge attach

- `xbridge_attach` (`6.5.30/unix.IP35`, address 0xc00000000009c8ac, 3484 bytes, 432-byte frame): attaches one Bridge
  or XBridge widget: reads its identity, builds the soft state, resets and programs the bridge, probes and initialises
  every slot, and finally runs the attached drivers. `a0` is the widget vertex. It logs "pcibr_attach" lines through
  `pcibr_debug` (mask 4), gets the register window with `pcibr_bridge_ptr_get(a0, 0)` and, when the widget has part
  number 0xc002 and manufacturer 54, reads the status with `pcireg_stat_get` and returns -1 (addresses
  0xc00000000009c9f4 and 0xc00000000009d36c) when bit 0x20 is clear. It adds the child "pci" to the vertex with
  `hwgraph_path_add`, registers and starts a PCI provider on it (`pciio_provider_register` with the table address held
  at gp-20512, `pciio_provider_startup`), adds the character device "controller" (`hwgraph_char_device_add`) and reads
  any asynchronous-attach record with `async_attach_get_info`. It takes the identification EEPROM address from
  `pcireg_nic_addr`; when `is_iobrick` (arguments: that address bits 38:33 and bits 27:24) says it is an I/O brick it
  records the EEPROM through `eeprom_vertex_info_set(0x400, node, a0, 0, widget)`, otherwise it takes the string from
  `nic_bridge_vertex_info`. It asks `pcibr_hints_get(a0, 0)` for administrator hints and returns -1 when a hint record
  exists whose word at +12 is nonzero. It builds the revision code from the identification word (bits 31:28 then part
  number in bits 27:12 shifted left 4) and labels the vertex "_pcibr_asic_rev" with it. `kmem_zalloc(0xac0)` gives the
  2752-byte soft state, attached to the vertex with `hwgraph_fastinfo_set`; it stores the vertex name copy (soft +16),
  the vertex handles (+0, +4), the highest slot 7 (+57) and lowest slot 0 (+56), the window (+8), the bridge type and
  mode (+114 for ATE count 0x80 or 0x400 and +112 for the type), the revision code (+120) and the speed and mode
  halfword (+116 from `pcireg_mode_get` and `pcireg_speed_get`). Types differ: the 0xd002 XBridge gets 0x400 internal
  ATEs and the Bridge 0x80; for a revision below 0xc0024 the two words at +2724 and +2720 (`bs_pio_end_*`) are 0x2000.
  It pushes the soft state on a global list (a 24-byte node swapped in at gp+0x4010 with `swap_int64`), derives the
  ASIC name from the revision code by comparison (0xc0021 to 0xc0024 pick successive two-byte names, 0xd0021 and
  0xd0022 two more, anything else "unknown"), reads the widget numbers from `xwidget_info_*` (+40 id, +44 master, +48
  master id, +51 set to 3) and the ELSC (`get_elsc`, +128), the brick type (`iobrick_l1type_get_nasid`, +138; negative
  logs "bricktype was unknown by L1") and module (`iobrick_module_get_nasid`, +136). For a positive brick type 35 to
  89 it indexes a 55-entry jump table (at 0xc00000000049b090): types 37 and 80 set the first slot 1, last slot 2 and
  last reset 2; types 52, 65 and 73 do so only when the widget id is 15 (last slot 5,
  5 and 6; type 52 also stores 12 at +52) and otherwise use last slot 2 and last reset 2; types 35,
  38, 48, 55,
  56, 61, 78,
  81, 88 and 89 change nothing; all others log "Unknown bricktype" at level 4. It initialises the lock
  (`spinlock_init` at +140) and the bus lock (`mrlock_init` at +144) and, with hints, copies the hint record's bytes
  into the slot table (host slot and flags), then clears every slot entry (words +1288 relatives, such as the window
  bases and flags). It registers the error handler with `xwidget_error_register`, clears the timeout field for
  revisions below 0xc0024 with `pcireg_timeout_bit_clr(0x3ff)`, resets interrupts (`pcireg_intr_reset_set(0x7f)`,
  `pcireg_intr_device_set(0xffffffff)`), initialises the direct map (`pcibr_directmap_init`), sets control bit
  0x200000, zeroes every internal ATE (`pcireg_int_ate_set`), and, for the Bridge, initialises external ATE RAM
  (`pcibr_init_ext_ate_ram`); the ATE pools are made with `rmallocmap` and `pcibr_ate_free`. It allocates the error
  interrupt with `xtalk_intr_alloc` (name "Bridge error", flag 1 set on a duplicated device descriptor), connects it
  with `xtalk_intr_connect` and ORs the enable bits at gp+0x6ed0 into the interrupt enable (`pcireg_intr_enable_set`),
  sets interrupt mode 0 and reads the flush register (`pcireg_tflush_get`). It sets the DMA control word at +124 from
  the revision (0x80000 below 0xc0022, 0x800 below that threshold, with the word at gp+0x4010 merged in by an OR, plus
  0x20000 for the same condition). It creates the three window maps with `pciio_device_win_map_new` and
  `pciio_device_win_populate`: soft +2160 (0x10000000 bytes, populated 0xff00000 from 0x200000), +2128 (0x100000,
  populated 0xfc000 from 0x4000) and +2144 (0x40000000 bytes, populated 0x3ff00000 from 0x200000). It creates the
  bridge's own info record (`pcibr_device_info_new` with slot 0xff), registers it (`pciio_device_info_register`;
  handle at +248, record at +256) and then loops over the slots calling, in turn for all slots,
  `pcibr_slot_info_init`, `pcibr_slot_addr_space_init`, `pcibr_slot_device_init`, (for type 1
  `pcibr_bus_cnvlink` first) `pcibr_slot_guest_info_init`; then `pcibr_initial_rrb` with the first and last slot bytes
  and brick-type-specific `pcibr_rrb_alloc_init` calls (types 0x25 and 0x50: channels 1 and
  2 each with 8; types 0x34, 0x41, 0x49: channels 1 and
  2 with 8 when the widget id is 14, else channel 0 with 1; and channel 1 with 8 when the EEPROM string contains
  "030-1275-"). It runs `pcibr_slot_call_device_attach` for each slot. If any slot's first function is vendor 0x11c1
  with device 0x5801 or 0x5802 (a USB part) it logs, clears and sets the bridge timeout fields
  (`pcireg_timeout_bit_clr(0x1f0000)`, `pcireg_timeout_bit_set(0x40000)`) and rewrites the hub's IXTT field through
  `hubii_ixtt_get` and `hubii_ixtt_set` to 0x4e2 in bits 47:36 (the rest masked by gp+0x6ed8). It returns 0 after
  `pciio_device_attach(handle, 0)`, bracketing it with `async_attach_add_info` and `async_attach_del_info` when an
  asynchronous record existed. The order and constants were read from the decompiled body and the jump table; the
  string-pool addresses and slot structure offsets are from the DWARF. Inferred.
- `pcibr_directmap_init` (`6.5.30/unix.IP35`, address 0xc00000000009e740, 300 bytes, 288-byte frame): sets up the
  bridge's direct-mapped DMA window from a crosstalk DMA translation of one node's memory. `a0` is the bridge's soft
  state; it reads the word at +0 (the crosstalk vertex handed to the translation), the word at +4 (the device vertex) and
  the byte at +48 (the widget number), and writes the byte at +88 and the doubleword at +80. It starts with
  `device_admin_info_get(the word at +4, "PCIBUS_DMATRANS_NODE")` (0xc00000000021d350; the key string is at
  0xc00000000152a190, from the pool word 0xc00000000163e780). With no value, node 0 is used. With a value it calls
  `hwgraph_path_to_vertex` (0xc000000000341358) and, unless that returns -1, `nodevertex_to_cnodeid`
  (0xc0000000003428b4), whose low halfword is sign-extended to a node number. A -1 from either, meaning the path names
  no node, calls `vertex_to_name(device, buffer at 0(sp), 256)` (0xc00000000034259c) and then
  `cmn_err(4, "Invalid hwgraph node path specified:\n    DEVICE_ADMIN: %s %s=%s\n", device name, "PCIBUS_DMATRANS_NODE", value)`
  (0xc0000000002ad048), and carries on with node 0 (the instruction at 0xc00000000009e838 is not reachable). The node's
  NASID is the halfword at `compact_to_nasid_node` (0xc000000001647000, pool word 0xc00000000163e788) + 2 x node. It
  calls `xtalk_dmatrans_addr(the word at +0, 0, NASID << 33, 0x4000, 0)` (0xc0000000000972b8) and keeps the result.
  `pcireg_dirmap_diroff_set(soft, result >> 31)` (0xc0000000000af408) and `pcireg_dirmap_wid_set(soft, the byte at
  +48)` (0xc0000000000af26c) follow, and the byte at +48 is copied to +88. When the result equals 0x20000000 it calls
  `pcireg_dirmap_add512_set(soft)` (0xc0000000000af5f8) and stores the doubleword 0x20000000 at +80. For any other
  result it calls `pcireg_dirmap_add512_clr(soft)` (0xc0000000000af778) and stores the result ANDed with
  0xffffffff80000000 at +80. It has one exit, and the return value is whatever the last `pcireg_dirmap_add512_*` call
  returned (nothing the caller is known to read). Which bridge register the diroff and add512 setters write: unknown
  here. Established.
- `pcibr_init_ext_ate_ram` (`6.5.30/unix.IP35`, address 0xc0000000000a2158, 352 bytes, 64-byte frame): finds how much
  external address-translation RAM the bridge has, programs that size, clears the entries and returns their count (0
  when there is none). `a0` is the bridge soft state: the word at +4 is the vertex used by the debug call and the
  doubleword at +8 its first argument after the format. The sizes are the words of the table at 0xc0000000015593f0
  (pool word 0xc00000000163e7b8), whose four words are 0, 8192, 16384 and 65536. The probe pattern is
  0x0123456789abcdef (pool word 0xc000000001644488). For each size index k from 1 to 3 it calls
  `pcireg_ext_ate_set(soft, size[k] - 1, pattern)` (0xc0000000000b64ac); when k is at least 2 it then calls
  `pcireg_ext_ate_set(soft, size[j] - 1, 0)` for j from 1 to k-1, so that a smaller size's last entry is zero when the
  pattern's entry aliases it; it reads `pcireg_ext_ate_get(soft, size[k] - 1)` (0xc0000000000b62e0) and, when the value
  equals the pattern, records k (a conditional move at 0xc0000000000a21fc; the record starts at 0, so a failing probe
  leaves the earlier one and 3 wins when all succeed). It then calls `pcireg_ssram_size_set(soft, recorded index)`
  (0xc0000000000ad074) and takes the count as the table word for that index. When bit 0x10 of the word
  `pcibr_debug_mask` (address 0xc00000000163c850) is set it calls `pcibr_debug(16, the word at +4, ...)`
  (0xc0000000000a1978) with "bridge at 0x%lx: clearing %d external ATEs\n" (0xc00000000152ae50, arguments the doubleword
  at +8 and the count) for a nonzero count, or "bridge at 0x%x: no external ATE RAM found\n" (0xc00000000152ae80) for a
  count of 0. It then calls `pcireg_ext_ate_set(soft, i, 0)` for each i from 0 to count-1 (skipped when the count is
  not positive) and returns the count. It has one exit (0xc0000000000a2298). Established.
- `pcibr_bridge_ptr_get` (`6.5.30/unix.IP35`, address 0xc0000000000a1c94, 172 bytes, 16-byte frame): returns the address
  of the register block of the bridge on a crosstalk widget, adding an offset for the second bus of a two-bus part.
  `a0` is the widget vertex and `a1` selects the bus. It maps the widget with `xtalk_piotrans_addr(a0, 0, 0, 8, 0)`
  (0xc0000000000970e8; the result is not tested) and reads the word at the mapping's +4. The part number is its bits 27
  to 12 and the manufacturer number its bits 11 to 1. When the part is 0xd102 or 0xd112 and the manufacturer number is 36
  (0x24), and `a1` (saved at 8(sp)) equals 1, it returns the mapped address plus 0x800000 (the sum is formed in the
  delay slot at 0xc0000000000a1d3c). In every other case, including either of those parts with another manufacturer
  number or a different `a1`, it returns the mapped address unchanged. Which chips carry those two part numbers, and
  that `a1` of 1 picks the second bus, are inferred from the two-bus layout the offset suggests; no name in the
  binary says so. It has one exit, 0xc0000000000a1d10. Established.
- `pcibr_detach` (`6.5.30/unix.IP35`, address 0xc00000000009e5ac, 404 bytes, 64-byte frame): tears down one PCI
  bridge: disables its interrupts, detaches every slot, frees its soft state and removes the hardware-graph entries it
  made. It returns 1 when the bridge has no "pci" entry and 0 once it has finished. `a0` is the bridge vertex
  (`s1`). It starts with `pcibr_debug(8, a0, "pcibr_detach\n")` (0xc0000000000a1978; the string is at 0xc00000000152a158)
  and `hwgraph_traverse(a0, "pci", &vertex)` (0xc0000000003411fc; the string is at 0xc00000000152a188), whose third argument is the address
  sp + 0 where it leaves the controller vertex; a nonzero result returns 1 (0xc00000000009e5e8). Otherwise the soft state is
  `hwgraph_fastinfo_get(vertex)` (0xc000000000341af8). Under the lock at soft +140 (`mutex_spinlock`,
  0xc000000000331f6c, level kept at 16(sp)) it calls `pcireg_intr_enable_set(soft, 0)` (0xc0000000000b127c) and
  releases with `mutex_io_spinunlock` (0xc000000000331e84). It then runs `pcibr_slot_detach(vertex, slot, 0, 0, 0)`
  (0xc0000000000abcd8) for every slot from the byte at soft +56 up to and including the byte at soft +57 (a signed
  compare against the second byte plus one, with the counter masked to a byte). After the slots it calls
  `pciio_device_info_unregister(vertex, the doubleword at soft +256)` (0xc0000000000c1660), `spinlock_destroy` on the lock
  at soft +140 (0xc000000000331d58), and frees the name string at soft +16 with `kmem_free(pointer, strlen(pointer))`
  (0xc00000000022c288; `strlen` is 0xc000000000047e54, and the size passed is the length without the terminator). It
  calls `rmfreemap` (0xc0000000002514c8) on the doubleword at soft +96 when it is nonzero and then on the one at
  soft +104 when it is nonzero, calls `xtalk_intr_disconnect` (0xc0000000000973b4) and `xtalk_intr_free`
  (0xc00000000009737c) on the doubleword at soft +2096, and frees the soft state with `kmem_free(soft, 2752)`. Last it
  removes the graph entries: `hwgraph_info_remove_LBL(vertex, "_pcibr_asic_rev", 0)` (0xc000000000340d00; string at
  0xc00000000152a168), `hwgraph_edge_remove(vertex, "controller", 0)` (0xc000000000340954; string at 0xc00000000152a178) and
  `hwgraph_edge_remove(a0, "pci", 0)`, and returns 0 (0xc00000000009e73c). None of the results other than the
  traverse is tested. Established.
- `pciiox_init` (`6.5.30/unix.IP35`, occupies 32 bytes at address 0xc00000000009d648): registers one board-part number prefix with the NIC code.
  It takes no argument. It calls `nic_vmc_add(string, routine)`, where the string is the pool word at 0xc00000000163e770 (gp-20480), "030-1062-" at 0xc00000000049b248, and the routine is the pool word at gp-20472, 0xc00000000009d668, the address right after this routine (set in the delay slot). It returns what `nic_vmc_add` returns. The routine at 0xc00000000009d668 was not read for this entry. The frame is 16 bytes. No instruction is branch-likely. Established.

## Interrupt connections

- `pcibr_intr_alloc` (`6.5.30/unix.IP35`, address 0xc0000000000a4f8c, 1552 bytes, 448-byte frame): allocates a PCI
  interrupt object for one device and ties each of the device's interrupt pins to a shared bridge interrupt bit. `a0`
  is the device vertex, `a1` an optional device descriptor, `a2` a bit mask of the pins wanted (bit 0 INTA, bit 1
  INTB, bit 2 INTC, bit 3 INTD; the pins are named only in the debug line) and `a3` the connection vertex. It takes
  the device's info record from `pcibr_info_get` (its byte +540 is the slot, the pointer at +40 the bridge's soft
  state). It returns 0 when `kmem_zalloc(160)` fails (address 0xc0000000000a5480) and when a needed crosstalk
  interrupt cannot be allocated and none was installed by another caller (after `cmn_err` level 1,
  0xc0000000000a5580); otherwise the new object (a 160-byte record). With no descriptor, or with one whose flags lack
  bit 4, the object is the threaded kind (first word 0; `pcibr_intr_free` waits for its interrupt thread, whose name
  `pcibr_intrd` it prints): it records a target cpu of -1 or the one the descriptor names (`intr_target_from_desc`)
  and the switch level (`device_desc_intr_swlevel_get`, or the kernel default when there is no descriptor); with bit 4
  of the flags set it is first word 2 and has no thread and takes no cpu from the descriptor. The record gets the
  vertex at +4, the pin mask at +16, the soft state at +96 and its own lock (+104). The unnamed routine reached
  through the soft-state pointer at +1288 (`bs_intr_bits`, an indirect call with the info record, the pin mask and the
  slot count) returns the bridge interrupt bits to use; they are logged ("pcibr_intr_alloc: pcibr_int_bits"). For each
  of the 8 bridge bits set in that result it uses the per-bit record at soft +1648 + 56 x bit (`bs_intr[]`, eight
  56-byte records ending at +2096): if its first doubleword, the crosstalk interrupt, is null it calls
  `xtalk_intr_alloc_nothd(master, descriptor, a3)` and installs it with a compare-and-swap; the loser frees its copy
  (`xtalk_intr_free`) and uses the winner's; the winner sets the bit's field in the bridge's device-interrupt register
  (`pcireg_intr_device_bit_clr` of the three bits for that bit position, then `pcireg_intr_device_bit_set` with the
  slot number in those bits). For a threaded object it reads the cpu (`xtalk_intr_cpuid_get`), warns with `cmn_err`
  level 4 when the bits it uses sit on two different cpus or when the cpu requested differs from the one allocated
  (printing the vertex name), and keeps the requested one. It then ORs the bit into the object's bit mask (+88),
  allocates a 32-byte list entry and appends this object to the bit's lock-free list (the list head is the pointer at
  record +24, record +1672 + 56 x bit in soft state, and the entries are linked with repeated `compare_and_swap_ptr`;
  a word at record +36 is set to 1 when an erased entry was reused), logging whether it was the first, second or Nth
  user of the bit and whether it replaced an erased entry. When the loop is done and the object is a threaded one it
  calls an unnamed static routine that sits just before `pcibr_intr_alloc` in `pcibr.o` (it takes the object, the bit
  mask and the switch level; it is the likely creator of the interrupt thread, and its body was not read) and then
  logs completion with the interrupt name from the descriptor (+16) or an empty string. Several branches were
  summarised from the decompiled body, not from the instructions; the list-append loop was not stepped instruction by
  instruction. Inferred.
- `pcibr_intr_connect` (`6.5.30/unix.IP35`, address 0xc0000000000a5880, 420 bytes, 80-byte frame): attaches a handler to a
  PCI interrupt object made by `pcibr_intr_alloc`, wires each bridge interrupt bit the object covers to a crosstalk
  interrupt, and enables those bits at the bridge. `a0` is the object, `a1` the handler and `a2` its argument. The
  object supplies the bit mask (the word at +88, held in `s2`) and the bridge's soft state (the doubleword at +96,
  `s1`). A null object returns -1 at once (the branch at 0xc0000000000a58b0 reaches the return with the value set at
  0xc0000000000a5898). Otherwise it calls `pcibr_debug(4096, the word at object +4, "pcibr_intr_connect: intr_func=0x%lx,
  intr_arg=0x%lx\n", handler, argument)` (0xc0000000000a1978), stores the argument at object +32, the handler at +24
  and then sets bit 0 of the word at +0, in that order. It then visits the bits 0 to 7 of the mask. For a set bit n
  the per-bit record is at soft +1648 + 56 x n. When the word at record +40 (soft +1688 + 56 x n) is nonzero the bit
  is already connected and nothing is done for it. Otherwise it calls `pcireg_intr_addr_addr(soft, n)`
  (0xc0000000000b2bdc, result kept at 40(sp)), stores the word `(the word at soft +72) x 8 + n` at record +48 (soft
  +1696 + 56 x n) and calls `xtalk_intr_connect` (0xc000000000097398) with the crosstalk interrupt at record +0,
  `pcibr_intr_func` (0xc0000000000a5cfc, from the pool word 0xc00000000163e800) as the handler, the address record +8
  as its argument, `pcibr_setpciint` (0xc0000000000a57d8, pool word 0xc00000000163e808) as the hook, the address
  record +48 and 0. It then stores 1 in the word at record +40, calls `pcireg_intr_addr_get(soft, n)`
  (0xc0000000000b27c0) and logs "pcibr_setpciint: int_addr=0x%lx, *int_addr=0x%lx, pcibr_int_bit=0x%x\n" with the
  address that `pcireg_intr_addr_addr` returned, the value read and n. After the loop it takes the lock at soft +140
  with `mutex_spinlock` (0xc000000000331f6c), calls `pcireg_intr_enable_bit_set(soft, the mask zero-extended to 32
  bits)` (0xc0000000000b1554), reads the bridge's flush register with `pcireg_tflush_get` (0xc0000000000ae584),
  releases the lock with `mutex_io_spinunlock` (0xc000000000331e84) and returns 0 (set at 0xc0000000000a597c). The
  routine has no failure path other than the null object. Established.
- `pcibr_force_interrupt` (`6.5.30/unix.IP35`, address 0xc0000000000a4d38, 232 bytes, 32-byte frame): makes one bridge
  interrupt bit fire by software. `a0` points at a pair: the doubleword at +0 is the bridge soft state and the word at
  +8 is the bridge interrupt bit number n. When the halfword at soft +114 (the chip kind) is nonzero it calls
  `pcireg_force_intr_set(soft, n)` (0xc0000000000b3da4) and returns. When it is zero it reads
  `pcireg_intr_status_get(soft)` (0xc0000000000b0fb0) and returns without doing anything unless bit n of that status
  is set. With the bit set it takes the crosstalk interrupt, the doubleword at soft + 56 x n + 1648, and asks
  `xtalk_intr_vector_get` (0xc00000000009756c) and `xtalk_intr_cpuid_get` (0xc0000000000973ec) for its vector and cpu.
  The cpu selects the doubleword at `pdaindr` (0xc000000001647568) + 16 x cpu + 8, a per-cpu record whose byte at +1320
  (named `p_slice` in the klconfig notes) and halfword at +1068 (the cpu's node, used as a NASID) are read. The routine
  then writes one doubleword, the vector (sign-extended halfword) ORed with 0x100 and truncated to 32 bits, to the
  address `0x9200000001800000 + (that halfword << 33) + 0x90`, with 0x200000 added when bit 1 of the byte at +1320 is
  set (the pool word at 0xc000000001644268 is the negated base; the arithmetic is `dsubu` of it). So the write goes to
  one of two 64-bit locations in that node's Hub register block, chosen by that bit. Which Hub register these are:
  unknown here. It returns no value on any path (the exits at 0xc0000000000a4d70 leave `v0` as the callee left it).
  Established.
- `pcibr_intr_func` (`6.5.30/unix.IP35`, address 0xc0000000000a5cfc, 592 bytes, 96-byte frame): the handler that
  `pcibr_intr_connect` registers with the crosstalk interrupt for each bridge interrupt bit; it runs every PCI interrupt
  object attached to that bit. It returns no value. `a0` is the per-bit wrapper that `pcibr_intr_connect` passes as the
  argument (`s5`, the address `pcibr_intr_connect` forms as its per-bit record plus 8). The wrapper holds the bridge soft state as the doubleword at
  wrapper +0, the bit number n as the word at wrapper +8, the head of the list of attached objects as the doubleword at
  wrapper +16, a count of woken threads as the word at wrapper +24 and a flag as the word at wrapper +28. A list entry (`s0`)
  holds its next entry at 0(s0), its object at 8(s0), a doubleword at 16(s0) and a byte at 24(s0). An
  object (`s1`) has its flags in the word at 0(s1) (bit 0 set once connected, bit 1 set for the kind without a thread,
  which is called directly), its handler at 24(s1), its argument at 32(s1), and a lock at 104(s1) and a
  semaphore at 48(s1), both used as addresses.
  When the halfword at soft +114 equals 2 and the low four bits of the word at soft +120 equal 1, it first takes the lock
  at soft +140 (`mutex_spinlock`, 0xc000000000331f6c), calls `pcireg_intr_enable_bit_clr(soft, 1 << n)`
  (0xc0000000000b13d4) and `pcireg_intr_enable_bit_set(soft, 1 << n)` (0xc0000000000b1554) in that order, and releases the
  lock with `mutex_io_spinunlock` (0xc000000000331e84). Then, when the word at wrapper +24 is nonzero, it returns at
  once. Otherwise it walks the list; a second pass runs only when the first met an object with bit 1 set. Entries whose
  object is null or whose bit 0 is clear are skipped. In the first pass an object with bit 1 clear (the kind with a
  thread) is woken: when the word at wrapper +28 is nonzero it calls `pcireg_wrb_flush_get(the doubleword at 16(s0),
  the byte at 24(s0))` (0xc0000000000b4870), it adds 1 to the word at wrapper +24 with a load-linked and
  store-conditional loop (the retry branch at 0xc0000000000a5e44 is branch-likely), calls `pcibr_wrap_put(wrapper,
  s1 + 104)` (0xc0000000000a4ca4) and `vsema(s1 + 48)` (0xc00000000023559c). Objects with bit 1 set are only
  counted in the first pass. In the second pass the objects with bit 1 set are called: the same flush when the word at
  wrapper +28 is nonzero, then `jalr` to the doubleword at 24(s1) with the doubleword at 32(s1) as its
  argument. After the second pass, when the word at wrapper +24 is still zero, it calls `pcibr_force_interrupt(wrapper)`
  (0xc0000000000a4d38). When no entry was handled at all (the list is empty, or every entry was skipped) it takes the
  soft state's lock, calls `pcireg_intr_enable_bit_clr(soft, 1 << n)`, reads `pcireg_tflush_get(soft)`
  (0xc0000000000ae584) and releases the lock, which turns the bit off. Every path ends at 0xc0000000000a5ed8. The code the woken thread of the threaded
  kind then runs: unknown here. Established.
- `pcibr_close` (`6.5.30/unix.IP35`, address 0xc00000000009dc64, 8 bytes, no stack frame): close entry of the PCI bridge
  device. It reads nothing and returns 0 (`or v0,zero,zero` in the delay slot of `jr ra`). Established.
- `pcibr_provider_startup` (`6.5.30/unix.IP35`, address 0xc0000000000a1164, 8 bytes, no stack frame): the bridge's
  provider start-up routine, by its name. The body is `jr ra` with a `nop` delay slot: no argument is
  read, nothing is stored, `v0` is untouched. Established.
- `pcibr_provider_shutdown` (`6.5.30/unix.IP35`, address 0xc0000000000a116c, 8 bytes, no stack frame): the matching
  provider shutdown routine, with the same body (`jr ra`, `nop` delay slot, nothing read or stored, `v0` untouched). Established.

## Slot information export

- `pcibr_slot_info_return` (`6.5.30/unix.IP35`, address 0xc0000000000a9830, 592 bytes, 64-byte frame): builds a
  6640-byte report on one bridge slot and copies it to a user buffer. `a0` is the bridge soft state, `a1` the slot
  number and `a2` the user address. It allocates the report with `kmem_zalloc(0x19f0)` and returns 12 when that fails
  (address 0xc0000000000a986c). The report is a 432-byte header followed by eight 776-byte function records (432 + 8 x
  776 = 6640). Offsets below are bytes in the report, and "slot +N" is byte N of the slot's 128-byte entry in
  `bs_slot[]` (the debug type `pcibr_soft_slot_s`, which starts at soft +264, so the code's soft +0x108 + 128 x slot
  is slot +0). Header: +0 halfword from soft +114 (`bs_bridge_type`); +2 halfword from soft +116 (`bs_bridge_mode`);
  +4 word from slot +0 (`has_host`); +8 byte from slot +4 (`host_slot`); +16 the vertex name of the slot (the vertex
  at slot +8, `slot_conn`; `vertex_to_name`, 256 bytes); +272 word from slot +12 (`slot_status`) with bit 0x40 set
  when `is_sys_critical_vertex` accepts that vertex; +276 the bus number from `pcibr_widget_to_bus(soft +4)`; +280 the
  function count (slot +16, `bss_ninfo`); +284 a string from `sprintf("%s", pci_space[word at slot +32])`; +304
  doubleword from slot +40 (inside `bss_devio`, which starts at +32); +312 doubleword from slot +56 (`bss_device`);
  +320, +324, +328 words from slot +68, +72, +76 (`bss_pmu_uctr`, `bss_d32_uctr`, `bss_d64_uctr`); +336 doubleword
  from slot +80 (`bss_d64_base`); +344 word from slot +88 (`bss_d64_flags`); +352 doubleword from slot +96
  (`bss_d32_base`); +360 and +364 words from slot +104 and +108 (`bss_d32_flags`, `bss_ext_ates_active`); +368
  doubleword from slot +112 (`bss_cmd_pointer`); +376 word from slot +120 (`bss_cmd_shadow`); +380, +384, +388, +392
  the four words at soft +1392 + 16 x slot; +396 the word at soft +1328 + 4 x slot; and four doublewords from the
  registers: +400 `pcireg_rrb_get(base, slot & 1)`, +408 `pcireg_intr_device_get(base)`, +416
  `pcireg_intr_enable_get(base)` and +424 `pcireg_intr_addr_get(base, slot)`, where `base` is the register window
  pointer at soft +8. Each function record at 432 + 776 x i is filled by
  `pcibr_slot_func_info_return(info array at slot +24, i, record)` for every index i below the function count. It
  copies the report to `a2` with `copyout(report, a2, 0x19f0)`: on success it frees the report and returns 0, on
  failure it returns 14 without freeing it (address 0xc0000000000a9a68). Every slot-entry member name above is the
  `pcibr_soft_slot_s` member at that offset, except slot +32 and +40, which lie inside the unnamed structure
  `bss_devio`. What each report field holds beyond the member name: unknown here. Inferred.

## Error interrupt, slot attach and mappings

- `pcibr_error_intr_handler` (`6.5.30/unix.IP35`, address 0xc0000000000a7274, 1460 bytes, 128-byte frame): the
  interrupt handler for the bridge's own error interrupt. `a0` is the bridge soft state and the routine returns
  nothing. It first walks the global list `pcibr_list` (address 0xc00000000163c870; each node holds a next pointer and
  a soft-state pointer) for `a0` and panics with "pcibr_error_intr_handler: (0x%lx) is not a pcibr_soft!" when it is
  absent. If `hubii_check_widget_disabled(node, widget)` says the bridge's widget is disabled (node from bits 39:33 of
  the register window address at soft +8, widget from soft +40) it calls `timeout` to run this same handler again
  later, adds one to the word at soft +2104 (`bs_errinfo.bserr_toutcnt`) and returns. Otherwise it reads the interrupt
  status (`pcireg_intr_status_get`), logs "pcibr_error_intr_handler: int_status" with
  `pcibr_debug` (mask 0x100), and does nothing more unless some bit from 8 up is set. With such a bit, status bits in
  the mask 0x4400008800 (the pool word at 0xc000000001644490) first call the static routine `pcibr_pioerr_check`
  (address 0xc0000000000a710c). Let E be the status with bit 31 cleared (mask 0xffffffff7fffffff). For each set bit n
  of E from 8 to 63 it adds one to a 12-byte counter record in `bs_errintr_stat` (soft +2180 + 12 x n; the words are a
  total count, the tick count of the last check and the total at that check) and measures the rate against the kernel
  tick counter `lbolt`: for a bridge of type 0 with `bs_busnum` zero, bit 18 with a rate above the highest seen so far
  (the word at 0xc000000001644d20) and more than 99 events since the last check logs "%s: %s, Excessive error
  interrupts : %d/tick" at `cmn_err` level 4 with the bridge name (soft +16) and the bit's message from the 64-entry
  table at 0xc000000001559b80 and updates the record; any other bit logs "%s: %s, Error rate %d/tick" at level 5 when
  more than 100 ticks have passed and the rate is above 100; and on a type 2 bridge whose revision bit selects (bit
  (soft +120 & 0xf) of 2) and which does not have bit 40 set, a total above 10000 logs "%s Disabling error interrupt
  type %s. Error count %d" at level 5. When bit 24 of the raw status is set and the bus error register
  (`pcireg_bus_err_get`) masked with 0xffffffffffc00000 equals 0xc00000 and the command word
  (`pcireg_cmdword_err_get`) masked with 0xf07f00 equals 0x402000, bit 24 is cleared from E (a recognised benign
  error). It then adjusts the global `bridge_errors_to_dump` (address 0xc00000000163c880): when the global flag
  `pcibr_pioerr_dump` is zero it clears the bits of the mask 0xffffffbbffff77ff, otherwise it sets the bits
  0x4400008800; if E has any bit in the result it prints "BRIDGE ERR_STATUS 0x%lx" at level 6 and runs
  `pcibr_error_dump(a0)`. If E has any bit in the fatal mask 0x22133fc06000 (the pool word at 0xc0000000016444a8; for
  type 2 with revision bit 1 or 2 selected, bits 0x300000 also get the dump), it calls `machine_error_dump` and then
  `panic("PCI Bridge Error interrupt killed the system")`. Otherwise it clears the interrupt: for type 0 and 1
  `pcireg_intr_reset_set(a0, pcibr_errintr_group(status))` and for type 2 `pcireg_intr_reset_set(a0, status | 0x40)`;
  for type 2 with bit 40 set and the revision test above it also calls `pcireg_arbitration_bit_set(a0, 0xf00000)`. It
  ends by writing 0 to the 64-bit word at soft +2120 (`bserr_intstat`, with `test_and_set_int64`), which re-arms the
  dump in `pcibr_error_dump`. Which bridge revision the test `1 << (rev & 0xf) & 2` means is not stated by the code.
  Inferred.
- `pcibr_slot_attach` (`6.5.30/unix.IP35`, address 0xc0000000000aba1c, 700 bytes): powers up and attaches one bridge
  slot, such as for a hot-plugged card, and returns a status code. `a0` is the bridge vertex, `a1` the slot number,
  `a2` a value passed on to `pcibr_slot_call_device_attach`, `a3` a value passed to
  `pcibr_slot_pwr`, and `a4` an optional pointer that receives the underlying error code when a step fails. The soft
  state comes from `hwgraph_fastinfo_get(a0)`; the slot entry is at soft + 128 x slot and its status word is at
  slot-entry +0x114 in the code's offsets (the member `slot_status`). If bit 0x10 of that word (the slot is
  initialised) is clear, it runs the bring-up: `pcibr_slot_pwr(a0, slot, 0x2002, a3)` (address 0xc0000000000abe28; a
  nonzero result is stored through `a4` and 7 is returned), sets the status word's bits 5:4 to 01, and, for any bridge
  type other than 0, rebuilds the speed and mode halfword at soft +116 from `pcireg_mode_get` and
  `pcireg_speed_get << 1`. It then waits
  500,000,000 nanoseconds with
  `nano_delay` (a pair of words, 0 and 0x1dcd6500, on the stack). The remaining steps each store their error code
  through `a4` and return a fixed value on failure: `pcibr_slot_info_init(a0, slot)` (address 0xc0000000000a9c68)
  returns 18 (0x12); `pcibr_slot_addr_space_init` 20 (0x14). In PCI-X mode (bit 0 of the halfword at soft +116) it
  sets the word at soft +1312 to 16 and the word at soft +1308 to 0, stores `pcibr_pcix_rbars_calc(soft, ..., 0x10)`
  (address 0xc0000000000a18bc) in the word at soft +1304, runs `pcibr_slot_pcix_rbar_init` (address
  0xc0000000000aa8fc) for every other slot from the lowest to the highest slot (bytes soft +56 and +57) and then for
  this slot last, and returns 29 (0x1d) if that last call fails. Then `pcibr_slot_device_init` returns 21 (0x15),
  `pcibr_slot_guest_info_init` 23 (0x17) and `pcibr_slot_initial_rrb_alloc` 24 (0x18) on failure. Whether or not it
  just initialised the slot, it then looks at the slot's function count (slot-entry +0x118, the member `bss_ninfo`): a
  count of 2 or more stores 1 through `a4` and returns 28 (0x1c); otherwise it calls
  `pcibr_slot_call_device_attach(a0, slot, a2)` (address 0xc0000000000ab4e0), returning 0 on success, 13 (0xd) when
  that call fails with code 42 (0x2a) and 25 (0x19) for any other failure (the code is stored through `a4` in both
  cases). The step order and codes were read from the decompiled body; the delay's units were taken from the argument
  pair (0 and
  500,000,000) and the callee name. Inferred.
- `pcibr_piomap_alloc` (`6.5.30/unix.IP35`, address 0xc00000000009f1e4, 636 bytes): allocates a programmed-I/O map
  object that gives a processor a window onto a range of one device's PCI space, and returns the record or 0. The
  arguments are the device vertex (`a0`), a descriptor that is not used (`a1`), the PCI space number (`a2`), the PCI
  address (`a3`), the size wanted (`a4`), the largest size the caller can use (`a5`) and the flags (`a6`). It gets the
  device's info record with `pcibr_info_get(a0)` (slot byte at +540, the record's list head at +528) and the bridge
  soft state with `pciio_info_mfast_get`. A size of 0 in either `a4` or `a5` logs
  "pcibr_piomap_alloc: req_size | req_size_max < 1" with
  `pcibr_debug` (mask 0x8000000) and returns 0. It converts the PCI range to a crosstalk address with an unnamed
  static routine at 0xc00000000009e910 (arguments: `a0`, slot, space, PCI address, size, flags); an answer of -1 logs
  "xio_addr == XIO_NOWHERE" and returns 0. It then takes the soft state's lock (+140) and searches the info record's
  chain of 96-byte map records (next pointer at record +56) for one whose size doubleword at +24 is zero, which it
  reuses; when there is none it releases the lock, allocates a new record with `kmem_zalloc(0x60)` and, after filling
  it, links it at the head of the chain under the lock again. A record is filled with the flags at +0, the vertex at
  +4, the slot at +8 (0xff stays 0xff and a bridge of type 2 adds one), the space at +12, the PCI address at +16, the
  size at +24, the soft state at +64 and zero at +72 (and, for type 2, zero at +80 to +95). It then calls
  `xtalk_piomap_alloc(bridge vertex, 0, crosstalk address, a4, a5, a6 & 0x1f)` (address 0xc000000000097078): a null
  answer clears the size in the record (so it becomes free) and the routine returns 0; otherwise the crosstalk map
  goes in the record at +48 and the crosstalk address at +40. On a type 2 bridge with flag bit 0x10 set it calls
  `pic_pio_flush_new(record, info)` (address 0xc00000000009c550) and, if that returns 0, logs "pcibr_piomap_alloc:
  flush-on-pio setup failed" at `cmn_err` level 4 and carries on. The result is the record, or 0 on any earlier
  failure. The unnamed routine at 0xc00000000009e910 was not read. Inferred.
- `pcibr_priority_bits_set` (`6.5.30/unix.IP35`, address 0xc0000000000a1518, 344 bytes): raises or lowers a slot's DMA
  priority by counting users of the priority setting and changing one bit of the slot's device register when the count
  crosses 1 and 0. `a0` is the bridge soft state,
  `a1` the slot number and `a2` the request: 1 or
  2 take a reference (high priority; 2 differs from 1 only in being accepted by the same test), 0 drops one. The bit
  is 0x200000, except that when the bridge revision (the word at soft +120, `bs_rev_num`) equals 0xc0022 the bit mask
  is 0 and no register write is ever made for it. The soft state's lock at +140 is held throughout (`mutex_spinlock`,
  `mutex_io_spinunlock`). The slot's software copy of the device register is the doubleword at slot +56 (`bss_device`,
  soft + 264 + 128 x slot + 56) and the reference count is the word at slot +64 (`bss_pri_uctr`). Requests 1 and 2:
  the count goes up by one; when it becomes 1 and the bit mask is nonzero the bit is set in the new register value,
  and when it becomes 1 and the mask is 0 the return value is -1; the return value is otherwise 0. Request 0: when the
  count is above 0 it goes down by one and, if it reaches 0 and the mask is nonzero, the bit is cleared in the new
  value (return 0); when the count is already 0 the return value is -1 and nothing changes. Any other request value
  returns 0 and changes nothing. When the new register value differs from the old one it writes it with
  `pcireg_device_set(soft, slot, value)` (address 0xc0000000000b41e4), stores it in the software copy and reads the
  bridge's flush register with `pcireg_tflush_get` (address 0xc0000000000ae584) so the write is not left posted. The
  return value is 0 for success and -1 as described above. Established.
- `pcibr_rrb_flush` (`6.5.30/unix.IP35`, address 0xc0000000000a3470, 380 bytes, 96-byte frame): flushes every read
  response buffer (RRB) that is currently assigned to the slot of the device being worked on. It takes no arguments
  and returns nothing. It finds the current device with `pciio_hostinfo_get` (the slot byte at info +540 is `S`) and
  its bridge soft state with `pciio_info_mfast_get`, and takes the register window from soft +8. It reads the widget
  identification word at window +4 and sets a variable `k` to 2 when the part number (bits 27:12) is 0xc002 with
  manufacturer (bits 11:1) 54, or 0xd002 with manufacturer 36, and to 1 for any other widget. It reads the RRB
  assignment word for the bridge half `S & 1` with `pcireg_rrb_get(window, S & 1)` (address 0xc0000000000b4ac4), takes
  the soft state's lock at +140 (`mutex_spinlock`) and examines the eight 4-bit fields of that word from the lowest
  upward. A field matches when its value ANDed with the mask `(2 x k - 1) | 8` (9 for k = 1, 0xb for k = 2) equals
  `(S >> 1) | 8`, meaning the valid bit is set and the device bits equal the slot's device number. For each match it
  calls `do_pcibr_rrb_flush(window, n)` (address 0xc0000000000a30ac), where n starts at `S & 1` and rises by 2 per
  field (so it names the RRB of that half), and then goes on with the next field. After the eighth field it releases
  the lock with `mutex_io_spinunlock`. Which bridge register the assignment word is and what the nibble bits other
  than the valid bit mean: unknown here. Established.
- `pcibr_piotrans_addr` (`6.5.30/unix.IP35`, address 0xc00000000009f4f8, 204 bytes, 64-byte frame): turns a PCI
  address on a bridge into a processor-side address for programmed I/O without allocating a mapping. `a0` is the PCI
  connection vertex (`s2`), `a1` is not read, `a2` the address space, `a3` the PCI address, `a4` the size and `a5` the
  flags. `pciio_piotrans_addr` calls it. It gets the PCI information record with `pciio_hostinfo_get(a0)`
  (0xc0000000000c1124), keeps the byte at record +540 (the slot) at 56(sp), takes the bridge soft state from
  `pciio_info_mfast_get(record)` (0xc0000000000c122c) and keeps the word at soft +0 at 0(sp). It then calls the static
  routine at 0xc00000000009e910 with (`a0`, slot, space, PCI address, size, flags), which has no symbol and whose body this
  entry does not cover, and takes its result as the crosstalk address. A result of -1 calls `pcibr_debug(0x10000000, a0,
  "pcibr_piotrans_addr: xio_addr == XIO_NOWHERE\n")` (0xc0000000000a1978; the string is at 0xc00000000152a3f0) and
  returns 0. Otherwise it returns `xtalk_piotrans_addr(the word at soft +0, 0, crosstalk address, size, flags & 0x1f)`
  (0xc0000000000970e8). The routine has one exit, 0xc00000000009f5a4. Established.
- `pcibr_error_cleanup` (`6.5.30/unix.IP35`, occupies 84 bytes at address 0xc0000000000a7b10): clears the bridge's pending error state and reads back the flush register.
  `a0` is the bridge soft state, kept in `s0`. The value passed to `pcireg_intr_reset_set` depends on the halfword at soft +114 (the chip kind): for 0 and for 1 it is 65 (0x41); for any other kind it is the
  doubleword at 0xc0000000016444b0 (gp+3392), which holds 0x00003cff00000041. (The load of that doubleword is in the delay slot of the compare against 1, and the branch for kind 1 then overwrites it with 65.)
  It calls `pcireg_intr_reset_set(soft, value)` and then `pcireg_tflush_get(soft)`, and returns what the second call leaves in `v0`. The frame is 16 bytes. No instruction is branch-likely. Established.
- `pcibr_error_extract` (`6.5.30/unix.IP35`, occupies 120 bytes at address 0xc0000000000a7b64): finds the slot number of the bridge device that a bus address and size fall into, or 255.
  `a0` is a vertex, with `a1` and `a2` saved at sp+16 and sp+24 and `a0` at sp+32. It calls `hwgraph_info_get_LBL(a0, label, &sp+0)`; the label string is at 0xc00000000152bfc0 ("_pcibr_asic_rev"), pool word
  at gp-20320 (0xc00000000152b978) plus 1608. A nonzero result returns 255 (set in that branch's delay slot). Otherwise it calls `hwgraph_fastinfo_get(a0)`, kept at sp+8; a null result returns 255.
  With a record it calls `pcireg_pci_bus_addr_get(record)` and then `pcibr_addr_toslot(record, that value, a1, a2, 0)`, and returns the low byte of the result (`andi` 0xff). The frame is 48 bytes. No instruction is branch-likely. Established.
- `pcibr_piomap_probe` (`6.5.30/unix.IP35`, occupies 80 bytes at address 0xc0000000000ac0a4): checks that a probe range lies inside a bridge PIO map and hands it to a probing routine.
  `a0` is the map record, `a1` an offset, `a2` a length and `a3` a pointer that is passed through unchanged. It returns -1 when `a1 + a2` is above the doubleword at map +24 (unsigned). Otherwise it calls the unnamed static routine
  at 0xc0000000000ac0f4 with `a0` set to the doubleword at +8 of the soft state whose pointer is at map +64, `a1` set to `a1` plus the doubleword at map +32 (set in the delay slot) and `a2`, `a3` unchanged, and returns its result.
  That static routine returns -1 unless `a2` is 1, 2, 4 or 8 and `a1` is a multiple of `a2` (`a1 & (a2 - 1)` is zero). Otherwise it calls the unnamed static routine at 0xc0000000000ac208 with `a0`, keeps its result, calls
  `badaddr_val(a1, a2, a3)`, calls `pcireg_intr_reset_set(a0, 64)`, calls the static routine at 0xc0000000000ac25c when the kept result is nonzero, and returns 1 when the probe's result was nonzero and 0 when it was zero. The two static routines at 0xc0000000000ac208 and 0xc0000000000ac25c are the pair the `pcibr_disable_mst_timeout`/`pcibr_enable_mst_timeout` symbols fold together; only 0xc0000000000ac25c was read. The frame is 16 bytes. No instruction is branch-likely. Established.
- `pcibr_map` (`6.5.30/unix.IP35`, occupies 204 bytes at address 0xc00000000009dc6c): maps a window of the bridge's registers into a process, and switches on a bridge control bit for a large window.
  `a0` is a vertex, `a1` the first argument for `v_mapphys` (kept in `s7`), `a2` an offset (`s1`) and `a3` a length (`s8`). It calls `hwgraph_connectpt_get(a0)`, then `hwgraph_fastinfo_get` on that result (soft state `s2`), saves the
  doubleword at soft +8 (the bridge register window) at sp+40 and drops the vertex with `hwgraph_vertex_unref`. The length is rounded up to 16 kilobytes (`a3 + 16383` ANDed with -16384, kept at sp+0). It calls
  `v_mapphys(a1, offset + window base, rounded length)` and keeps the result. When the halfword at soft +114 is nonzero or the result is nonzero it returns the result. Otherwise, when the offset equals 0xc00000 or the rounded length is above 0xc00000, it calls `pcireg_control_bit_set(soft, 0x80000000)`.
  It returns the `v_mapphys` result on every path. The frame is 64 bytes. No instruction is branch-likely. Established.
- `pcibr_write_gather_flush` (`6.5.30/unix.IP35`, occupies 60 bytes at address 0xc00000000009e8d4): reads the write-response flush register for the device's slot and returns 0.
  `a0` goes unchanged to `pciio_hostinfo_get`; the result is kept in `s8`. `pciio_info_mfast_get(that record)` gives a pointer, which the routine passes unchanged as the first argument of the unnamed static routine at 0xc00000000009dc14, with the byte at record +540 (loaded in the delay slot)
  as the second. That static routine is a 20-byte wrapper that calls `pcireg_wrb_flush_get` with its two arguments unchanged. The result is discarded and `v0` is set to 0 after the call. The frame is 16 bytes. No instruction is branch-likely. Established.
- `pcibr_wrb_flush` (`6.5.30/unix.IP35`, occupies 60 bytes at address 0xc0000000000a35ec): the other caller of the write-response flush read; it passes the register window and the slot, and returns 0.
  `a0` goes unchanged to `pciio_hostinfo_get`. The byte at record +540 is loaded into `s8` in the delay slot of the next call, `pciio_info_mfast_get(record)`, whose result is a soft state; the routine passes the doubleword at +8 of it (the register window) and the slot byte to
  `pcireg_wrb_flush_get`, discards the result, and returns 0. The frame is 16 bytes. No instruction is branch-likely. Established.
- `pcibr_setpciint` (`6.5.30/unix.IP35`, occupies 168 bytes at address 0xc0000000000a57d8): writes a PCI interrupt's destination address and vector into the bridge.
  `a0` is a crosstalk interrupt record (`s0`). It calls, in order, `xtalk_intr_addr_get` (result kept at sp+24), `xtalk_intr_vector_get` (`s8`), `xtalk_intr_dev_get` (`s7`) and `xtalk_intr_sfarg_get` twice. The word at the first sfarg result gives the pin (its low three bits, `s1`); the word at the second, shifted right 3 and ANDed with 1, is the second argument of `pcibr_bridge_ptr_get` (the form `pcibr_intr_disconnect` stores is the bit number with the bus number shifted left 3).
  `pcibr_bridge_ptr_get(device, that bit)` gives the register block (sp+16). It then calls `pcireg_intr_addr_vect_set(block, pin, vector & 0xff)` and last `pcireg_intr_addr_addr_set(block, pin, address)`, whose result is returned. The frame is 64 bytes. No instruction is branch-likely. Established.
- `pcibr_setwidint` (`6.5.30/unix.IP35`, occupies 132 bytes at address 0xc0000000000a5c78): writes the bridge's own error-interrupt destination from a crosstalk interrupt record.
  `a0` is the crosstalk interrupt record (`s0`). It calls `xtalk_intr_target_get` (`s7`), `xtalk_intr_addr_get` (`s8`), `xtalk_intr_vector_get` (kept at sp+16) and `xtalk_intr_sfarg_get`, whose result (the bridge soft state) replaces `s0`. It then calls
  `pcireg_intr_dst_target_id_set(soft, target & 0xff)`, `pcireg_intr_dst_addr_set(soft, address)` and `pcireg_intr_host_err_set(soft, vector & 0xff)`, in that order, and returns the last result. The frame is 48 bytes. No instruction is branch-likely. Established.
- `pcibr_ate_addr` (`6.5.30/unix.IP35`, occupies 64 bytes at address 0xc0000000000a23fc): returns the address of one address-translation entry, from the internal or the external table.
  `a0` is the bridge soft state and `a1` the entry index. It compares `a1` (signed, 64-bit register) with the halfword at soft +112: below it, it calls `pcireg_int_ate_addr(a0, a1)`; otherwise `pcireg_ext_ate_addr(a0, a1)`. The arguments pass through unchanged, the delay slots are `nop`, and the result is returned. The frame is 16 bytes. No instruction is branch-likely. Established.
- `pcibr_unmap` (`6.5.30/unix.IP35`, occupies 120 bytes at address 0xc00000000009dd38): undoes the control bit that `pcibr_map` sets, and always returns 0.
  `a0` is a vertex. It calls `hwgraph_connectpt_get(a0)` (kept in `s7`), `hwgraph_fastinfo_get` on the result (soft state, `s0`) and `hwgraph_vertex_unref(connect point)`. When the halfword at soft +114 is nonzero it returns 0. Otherwise it calls `pcireg_control_get(soft)` and, when bit 31 (0x80000000) of the result is set, `pcireg_control_bit_clr(soft, 0x80000000)`. `v0` is 0 on every path. The frame is 32 bytes. No instruction is branch-likely. Established.
- `pcibr_enable_mst_timeout` (`6.5.30/unix.IP35`, occupies 36 bytes at address 0xc0000000000ac1e4): sets two bits in the bridge's interrupt enable register and reports whether the register changed.
  `a0` goes unchanged to `hwgraph_fastinfo_get`; the doubleword at +8 of the result (the register window, loaded in the delay slot of the next call) is the argument of the unnamed static routine at 0xc0000000000ac25c, whose result is returned. That routine reads `pcireg_intr_enable_get`, sets the bits 0x400000800 (the doubleword at gp+3408, 0xc0000000016444c0;
  the name suggests the master timeout, which the bits' layout here does not confirm) with `pcireg_intr_enable_bit_set` and reads the enable register again; it returns 1 when the value changed and 0 when it did not. The frame is 16 bytes. No instruction is branch-likely. Established.

## Crossbar performance counters

- `xbow_enable_perf_counter` (`6.5.30/unix.IP35`, address 0xc0000000000999f0, 460 bytes): starts, or stops, counting
  on one of the crossbar's two link performance counters. `a0` is the crossbar vertex, `a1` the port number (8 to 15),
  `a2` the counting mode (0, 1 or
  2) and `a3` the counter number (0 or 1). It returns -1 for a port outside 8 to 15, a mode outside 0 to 2 or a
  counter outside 0 to 1, and 0 on success. The crossbar soft state comes from `hwgraph_fastinfo_get(a0)` (the
  register window is the pointer at soft +16) and its lock is the one at soft +448 (`mutex_spinlock`,
  `mutex_spinunlock`). Each counter has a 16-byte record at soft + 16 x counter: the saved count at +0x20 (word), the
  port in use at +0x24 (byte, as port minus 8), the mode in use at +0x25 (byte), copies of the two bytes at +0x26 and
  +0x27, and the pointer to the counter register at +0x28. It refuses (returns -1, lock released) when this counter is
  already counting (+0x25 nonzero) and the new mode is nonzero, and when the other counter is already counting the
  same port. Otherwise it stores the new mode in +0x25 and +0x27 and the port number minus 8 in +0x24 and +0x26. It
  then sets bits 29:28 of the port's link register (the word at window + 64 x port - 0xf4, so +0x0c of each 64-byte
  port block) to the mode, and rewrites the counter register through its pointer with bits 22:20 set to the port
  number minus 8, and saves the counter's low 20 bits (the old count) at +0x20. It finishes by arming a repeating poll
  with `timeout(xbow_update_perf_counters, a0, 4)` (the routine at address 0xc0000000000998c8) and returns 0, even
  when the mode is 0 (stop). What the two modes 1 and 2 count: unknown here. Inferred.
- `xbow_ioctl` (`6.5.30/unix.IP35`, address 0xc000000000098434, 876 bytes): the control-request handler of the
  crossbar's device node. `a0` is the crossbar vertex, `a1` the request number, `a2` the user argument address,
  `a3` the open mode (bit 0 read, bit 1 write), `a4` the caller's credentials and `a6` a pointer to a result word that
  is set to 0 at once. It returns 0 for success or an error number, and its requests are: 0 and any request number it
  does not list return 0. Requests 1 and 2 (start and stop a counter): without the capability 0x2000000000
  (`cap_able_cred(a4, 0x2000000000)`) it returns 1; without write mode 9; it copies 12 bytes of three words from the
  user (`copyin`, error 14), forces the second word (the counting mode) to 0 for request 2, and calls
  `xbow_enable_perf_counter(a0, first, second, third)` (address 0xc0000000000999f0), returning 22 when that fails.
  Request 3 (read the counters): without read mode it returns 9; it calls `xbow_get_perf_counters` (address
  0xc0000000000999d0), `assfail("xbow_perf_cnt != NULL", file argument not copied, line 612)` when that gives null, and copies 0x100
  bytes to the user (14 on failure). Requests 4 and 5 (start and stop the link-level monitor): with the capability
  test (1 on failure) they call `xbow_enable_llp_monitor` (address 0xc0000000000999d38) or `xbow_disable_llp_monitor`
  (address 0xc000000000099cf0) and return 22 on a nonzero result. Request 6 (read the link status): without read mode
  9; `xbow_get_llp_status` (address 0xc000000000099bbc), `assfail("xbow_llp_status != NULL", file argument not copied, line 652)` on
  null, and a 0x80-byte copy to the user (14 on failure). Requests 0x6701 and 0x6702 (bandwidth allocation between two
  widgets) check the same capability with `cap_able` (return 1), copy 16 bytes from the user (14 on failure), turn the
  two words at the start into widget numbers with `xbow_widget_num_get` (address 0xc000000000098370) and call
  `xbow_prio_bw_alloc` (address 0xc000000000099fe8) once for each direction (widgets swapped): request 0x6701 passes 0
  as the fourth argument and the 64-bit value at user offset 8 as the fifth, and request 0x6702 passes the 64-bit
  value at offset 8 as the fourth argument and its negation as the fifth. Both return 0. What the 64-bit values mean
  (bytes or a priority amount): unknown here. Inferred.
- `pcibr_intr_disconnect` (`6.5.30/unix.IP35`, address 0xc0000000000a5a24, 476 bytes): disconnects a PCI interrupt
  object from the bridge interrupt bits it uses, leaving shared bits alone. `a0` is the interrupt object and the
  routine returns nothing. It zeroes the four words at object +24, +28, +32 and +36, clears bit 0 of the object's
  first word (the connected flag) and takes the object's bit mask (word at +88) and the bridge soft state (pointer at
  +96). It removes from the mask every bit whose per-bit record (soft +1648 + 56 x bit, `bs_intr[]`) has a nonzero
  word at record +36, which marks a bit that other objects also use. If bits remain, it takes the soft state's lock at
  +140, clears those bits from the bridge's interrupt enable with `pcireg_intr_enable_bit_clr` (address
  0xc0000000000b13d4), reads the flush register (`pcireg_tflush_get`) so the write is not left posted, releases the
  lock, and logs "pcibr_intr_disconnect: disabled int_bits=0x%x" with `pcibr_debug` (mask 0x1000). Then for each
  remaining bit it calls `xtalk_intr_disconnect` on the record's crosstalk interrupt (the doubleword at record +0;
  address 0xc0000000000973b4), clears the word at record +40, and logs "pcibr_intr_disconnect: disconnect
  int_bits=0x%x". After that it tests the record's word at +36 again and, when it is nonzero, reconnects the interrupt
  for the remaining users with
  `xtalk_intr_connect(record +0, pcibr_intr_func, record +8, pcibr_setpciint, record +48, 0)` (the routines at
  0xc0000000000a5cfc and 0xc0000000000a57d8) after storing `bit | bs_busnum << 3` at record +48, and logs "now-sharing
  int_bits"; this reconnect runs only when another processor sets the word between the two tests, since the bit was
  selected because the word was zero. With no bits left after the shared ones are removed the routine ends without
  touching the hardware. Inferred.
- `pcibr_intr_free` (`6.5.30/unix.IP35`, address 0xc0000000000a55e0, 504 bytes, 96-byte frame): releases a PCI
  interrupt object made by `pcibr_intr_alloc`, detaching it from every bridge interrupt bit it used and stopping its
  thread. `a0` is the object and the routine returns nothing. The object's bit mask is the word at +88 and the bridge
  soft state the pointer at +96. For each bit n from 0 to 7 that is set in the mask it takes the bit's record (soft
  +1648 + 56 x n, `bs_intr[]`) and walks the bit's list of registered users (head at record +24, next pointer at entry
  +0): for each entry it does `compare_and_swap_int64(entry +8, object, 0)`, which clears the entry's pointer when it
  still points at this object, and when that succeeded it logs "pcibr_intr_free: cleared hdlr from bit 0x%x" through
  `pcibr_debug` (mask 0x1000). Then, if the record's word at +36 is zero (no other user shares the bit) and its
  crosstalk interrupt (the doubleword at record +0) is nonzero, it frees that with `xtalk_intr_free` (address
  0xc00000000009737c), zeroes the doubleword and clears the bit's three-bit field in the bridge's interrupt device
  register with `pcireg_intr_device_bit_clr(soft, 7 << (3 x n))` (address 0xc0000000000b2380). When bit 1 of the
  object's first word is clear (the threaded kind), it waits for the interrupt thread to have started: it checks bit
  0x400 of the thread flag word at object +40 and, while that is clear, sleeps with `nano_delay` for
  10,000,000 nanoseconds and counts 10 more milliseconds, up to 1000. If the bit never appears it logs
  "pcibr_intr_free: Unable to terminate pcibr_intrd thread, xt=0x%lx" at `cmn_err` level 4 and goes on. It then sets
  the thread's function to `xthread_exit` (address 0xc00000000027cbf4) with
  `xthread_set_func(thread at +64, xthread_exit, 0)`, wakes it with `vsema(object +48)` (address 0xc00000000023559c)
  and logs "pcibr_intrd xt exiting, wait=%d msec" with the thread and the milliseconds waited. Both kinds end with
  `kmem_free(object, 0xa0)`. The record fields are named by offset only, because the debug type of `bs_intr[]` is
  anonymous. Inferred.
- `xbow_update_llp_status` (`6.5.30/unix.IP35`, occupies 276 bytes at address 0xc000000000099bdc): timer routine that folds the crossbar's per-link status words into running totals and reports a bad link status.
  `a0` is the crossbar vertex, kept at sp+0 and later passed on as a zero-extended 32-bit value. `hwgraph_fastinfo_get(a0)` gives the
  soft state (`s3`); the doubleword at +16 is the register block (`s5`), the word at +8 the crossbar's own widget vertex, and the address
  soft +320 starts a table of 16-byte total slots, one per link. The loop runs for link 0 to 7 with the widget number 8 to 15 in `s1`.
  Every pass first calls `xbow_widget_lookup(word at +8, widget)` and then `xwidget_name_get` on its result, used or not. It reads two
  words of the link's 64-byte slot in the register block, at slot +300 (the status) and +316. When bit 31 of the status is set it adds
  bits 23:16 of the +316 word to the second doubleword of the link's total slot (stored first) and its top byte to the first
  doubleword (the store is in the delay slot of the next branch), and when the status ANDed with 0x7fffffe7 is nonzero it calls
  `cmn_err(4, "link %d[%s]: bad status 0x%x\n", link, name, status)`; the format string is at 0xc00000000049a420 (pool word
  0xc000000000499cc8 plus 1880) and the status in `a4` is the register loaded for the test, not reloaded. That test runs only
  when bit 31 was set. After link 7, when the word at soft +452 is nonzero, it calls
  `timeout(itself, a0 as an unsigned 32-bit value, 100)`; the routine's own address is the pool word at 0xc00000000163e6e0 (gp-20624).
  The routine sets no value of its own: `v0` is the result of `timeout` when that call is made. The frame is 64 bytes. No instruction is branch-likely. Established.
- `xbow_update_perf_counters` (`6.5.30/unix.IP35`, occupies 264 bytes at address 0xc0000000000998c8): timer routine that adds each active crossbar link counter's change since the last visit to a running total, then re-arms itself.
  `a0` is the crossbar vertex, kept in `s8`; `hwgraph_fastinfo_get(a0)` gives the soft state (`s4`). The two 16-byte counter records start at
  soft +32 (`s0`), and the lock is the address soft +448. For counter 0 then 1 it skips a record whose byte at +5 is zero. For an active one it takes the
  lock with `mutex_spinlock`, reads the word the pointer at record +8 points to, keeps its low 20 bits as the new count and takes bits 22:20 of that word as a
  port index, subtracts the saved count at record +0 and masks the difference to 20 bits (zero-extended), adds it to the doubleword at
  soft + 64 + 32 x (port index) + 8 x (byte at record +7), stores the new count at record +0 (the store is in the delay slot of the unlock call) and releases the lock with
  `mutex_spinunlock(soft + 448, level)`. After both counters it calls `timeout(itself, a0 as an unsigned 32-bit value, 4)`, with the routine's own
  address from the pool word 0xc00000000163e6d8 (gp-20632), and returns what `timeout` leaves in `v0`. The rearm happens whether or not a counter was active. The frame is 64 bytes. No instruction is branch-likely. Established.
- `xbow_enable_llp_monitor` (`6.5.30/unix.IP35`, occupies 76 bytes at address 0xc000000000099d38): starts the crossbar link-status timer and sets the flag that lets it re-arm itself.
  `a0` is the crossbar vertex, kept in `s8`. `hwgraph_fastinfo_get(a0)` gives the soft state, kept at sp+16. It calls
  `timeout(function, a0 as an unsigned 32-bit value, 100)`; the function is the pool word at 0xc00000000163e6e0 (gp-20624), which holds 0xc000000000099bdc, the address of
  `xbow_update_llp_status`. It returns 0 and stores the word 1 at soft +452 in the delay slot of the final `jr`, so the flag is set after the first timer is queued; that is the
  word `xbow_update_llp_status` tests before re-arming. The frame is 32 bytes. No instruction is branch-likely. Established.
- `xbow_disable_llp_monitor` (`6.5.30/unix.IP35`, occupies 72 bytes at address 0xc000000000099cf0): stops the link-status timer from re-arming and clears the link totals.
  `a0` is passed unchanged to `hwgraph_fastinfo_get`; the result is the soft state (`a3`). A loop over link 0 to 7 stores a zero doubleword at soft + 16 x link + 328 and then a zero
  doubleword at soft + 16 x link + 320 (the second store is in the delay slot of the loop branch). These are the two fields of each total slot that
  `xbow_update_llp_status` adds to. The routine returns 0 and stores a zero word at soft +452 in the delay slot of the final `jr`. The timer already queued is not cancelled.
  The frame is 16 bytes. No instruction is branch-likely. Established.
- `xbow_widget_lookup` (`6.5.30/unix.IP35`, occupies 48 bytes at address 0xc00000000009881c): finds the vertex of a crossbar port from its widget number.
  `a0` is the crossbar vertex and `a1` the widget number, kept in `s8` (saved in the delay slot of the first call). It calls `xswitch_info_get(a0)`, then
  `xswitch_info_vhdl_get(that result, widget number & 0xff)` (the mask is in the delay slot) and returns that result. The frame is 16 bytes. No instruction is branch-likely. Established.

## Crossbar attach and links

- `xbow_attach` (`6.5.30/unix.IP35`, address 0xc000000000097fe0, 776 bytes, 320-byte frame): attaches the Crossbow
  widget: makes its control device, builds its 656-byte soft record, connects its error interrupt, registers its error
  handler and marks the live links of the switch. `a0` (kept in `s2`) is the widget vertex. It maps the widget's
  registers with `xtalk_piotrans_addr(vertex, 0, 0, 768, 0)` (0xc0000000000970e8; result `s1`, the register base), gets
  the connection-point vertex with `hwgraph_connectpt_get` (0xc00000000034232c; `s4`) and adds a character device
  with `hwgraph_char_device_add(vertex, "xbow", "xbow_", &new)` (0xc00000000034164c; the strings are at
  0xc000000000499d78 and 0xc000000000499cc8, the pool word 0xc00000000163e6b8), the new device vertex being the word at
  0(sp). It allocates the record with `kmem_zalloc(656, 0)` (0xc00000000022c21c, the result is not tested) and stores
  the widget vertex at +0, the register base at +16, the connection-point vertex at +8 and the device vertex at +4,
  then calls `hwgraph_fastinfo_set(device, record)` (0xc000000000341ac8). It takes the device's name with
  `dev_to_name` (0xc0000000003425cc, 256 bytes at 8(sp)), allocates strlen + 11 bytes with `kmem_alloc` and formats
  `sprintf(buffer, "%s[xbow# %d]", name, 0)`, keeping the buffer pointer at record +24. The revision code is built from the word at register base +4:
  bits 31 to 28 OR the 16-bit field in bits 27 to 12 shifted left by 4. Only when the code is below 2 (the branch at
  0xc0000000000980cc) does it print: code 1 selects the string "1.0" and 0 selects "unknown", and it then calls
  `cmn_err(4, "%sCrossbow ASIC: rev %s (code=%d) at %s%s", "DOWNREV ", string, code, record +24 name, "")`
  (0xc0000000002ad048). The compare chain at 0xc000000000098248 also tests 3, 4, 5, 0xd0001 and 0xd0002 (strings "1.2",
  "1.3", "2.0", "Xbridge 1.0", "Xbridge 2.0"), but it follows the below-2 test, so those arms cannot be reached.
  It then calls `spinlock_init` (0xc000000000331d4c) on record +448 ("xbow_perf_lock") and, after storing register
  base +108 at record +56 and register base +100 at record +40, on record +520 ("xbow_bw_alloc_lock"). For each of 8
  ports n it stores 0 at record +592 + 8n and then 800000000 (0x2faf0800) at +528 + 8n. It duplicates the device's
  descriptor with `device_desc_dup` (0xc00000000021cf48), sets bit 0 of its flags (`device_desc_flags_get`, then
  `device_desc_flags_set`), names its interrupt "Crossbow error" with `device_desc_intr_name_set`, allocates the
  interrupt with `xtalk_intr_alloc(vertex, descriptor, device)` (0xc000000000097344) and connects it with
  `xtalk_intr_connect(interrupt, 0xc000000000098c24, record, 0xc00000000009884c, register base, 0)` (0xc000000000097398;
  both routines are static with no symbol; the pool words are 0xc00000000163e6c0 and 0xc00000000163e6c8), frees the
  descriptor (`device_desc_free`, 0xc00000000021cf1c) and registers 0xc000000000099218 (pool word 0xc00000000163e6d0,
  also static) with `xwidget_error_register(vertex, 0xc000000000099218, record)` (0xc000000000097b38). It stores 36
  at register base +36 and creates the switch information with `xswitch_info_new(connection point)`
  (0xc000000000097e24). For ports 8 to 15 it calls the static routine at 0xc000000000098804, which returns bit 31 of
  the word at register base + 64 x (port & 7) + 276; when that is nonzero it calls `xbow_widget_present`
  (0xc0000000000987a0) and, when that is nonzero, `xswitch_info_link_is_ok(info, port & 0xff)` (0xc000000000097ee4) and
  replaces bits 8 to 4 and bit 0 of the word at register base + 64 x (port & 7) + 268 with 0x3002b (mask
  0xfffffe2f, read-modify-write). It ends with `xswitch_provider_register(connection point, xbow_provider)`
  (0xc000000000097ebc; `xbow_provider`, at 0xc00000000163c810, holds the address of `xbow_reset_link`) and
  returns 0 (set at 0xc000000000098234). It has a single exit and no failure path. Established.
- `xbow_port_io_enabled` (`6.5.30/unix.IP35`, address 0xc000000000045084, 228 bytes, 16-byte frame): reports whether
  the crossbar port `a1` of node `a0` (a NASID) is marked as usable for I/O in the node's configuration records. It
  returns 1 or 0. With `a0` shifted left 33 as the node offset, it reads the doubleword at 0x9000000034000000 (pool
  word 0xc000000001644040) plus the offset and tests whether its bits 15 to 12 (the word masked with 0xf000) equal
  0xc000. When they do, the first board is the result of `chk_for_pebrick` (0xc000000000046384), which the code calls
  with `a0` still holding the NASID, the same test `klhwg_add_all_xbows` makes. Otherwise it reads the doubleword at
  `0x9600000000000000 + offset + 0x2000 + 72`, keeps its low 40 bits, ORs them into `0x9600000000000000 + offset`
  and reads the word 20 bytes in; when that word is zero the first board is 0, else it is
  `0x9600000000000000 | ((the word + offset) & 0xffffffffff)`. With the first board it calls `find_lboard(first, 66)`
  (0xc000000000044370; 66 is 0x42, the Linux name `KLTYPE_PBRICK_XBOW`) and returns 0 when none is found. Otherwise
  it calls `find_component(board, 0, 4)` (0xc0000000000444b8) and returns 0 when there is none; with one it reads the
  byte at component + 8 x port - 6 and returns 1 only when both bit 0 and bit 2 (0x1 and 0x4) of that byte are set,
  otherwise 0. The port number is not range-checked. What the +72 doubleword, the word at +20 and the byte at 8 x
  port - 6 are in the configuration layout: inferred only from the Linux names for type 66 and component type 4.
  Inferred.
- `xbow_xmit_retry_error` (`6.5.30/unix.IP35`, address 0xc000000000098b20, 260 bytes, 32-byte frame): counts and
  acknowledges a transmit-retry error reported against one crossbar port, and returns 1 when the error was one it
  recognised or could not examine, 0 for any other device. `a0` is the crossbar soft record and `a1` the port. The
  port's mapped register window is cached as the doubleword at record + 8 x port + 392. When that is zero it calls
  `xswitch_info_get(the word at record +8)` (0xc000000000097cf8); a null result returns 1. It then calls
  `xswitch_info_vhdl_get(info, port & 0xff)` (0xc000000000097d68), returning 1 for -1, and maps the port's widget with
  `xtalk_piotrans_addr(vertex, 0, 0, 88, 0)` (0xc0000000000970e8), returning 1 for a null result and otherwise
  caching the pointer at record + 8 x port + 392. With a window it reads the word at +4 and compares its bits 27 to
  12 and bits 11 to 1. A part of 0xc002 with 54 (0x36) in the low field, or a part of 0xd002 with 36 (0x24),
  counts as recognised: it adds 1 to the word `xbow_xmit_retry_errors` (0xc00000000163c818), reads the word at +36 of
  the window and writes the same value back, reads the word at +36 again (the result is discarded) and returns 1. Any
  other identification returns 0 and changes nothing. The Linux names for these parts are the Bridge (0xc002) and
  Xbridge (0xd002); the write-back as an acknowledge is inferred. Established.
- `xbow_widget_num_get` (`6.5.30/unix.IP35`, address 0xc000000000098370, 196 bytes, 288-byte frame): returns the
  crosstalk widget number that a hardware-graph vertex belongs to, or 255 when none can be found. `a0` is the vertex
  (saved at 264(sp)). It writes the vertex's name into the 256-byte buffer at 0(sp) with `vertex_to_name`
  (0xc00000000034259c) and requires the name to contain "pci" and "controller" (the strings at 0xc000000000499db0 and
  0xc000000000499da0, from the pool word 0xc00000000163e6b8) with two `strstr` calls (0xc0000000001de44c), returning 255 when
  either is missing. Unlike the IP27 routine of the same name, it has no branch for a vertex that is a node. It then
  stores the vertex as the word at 256(sp) and follows the edge named ".." (0xc000000000499db4) twice with
  `hwgraph_edge_get(cursor, "..", &cursor)` (0xc000000000340a20), the register `s0` counting the passes (it is
  incremented in the branch delay slot at 0xc0000000000983d8, so a failing lookup also counts); a nonzero result
  from a lookup returns 255 (the value is set at 0xc0000000000983e0 and 0xc0000000000983ec). For the vertex reached it calls
  `xwidget_info_chk` (0xc0000000000975bc), returns 255 when that gives nothing, and otherwise returns
  `xwidget_info_id_get` (0xc0000000000976cc) of the result. Exits are 0xc0000000000983ec, where `v0` is the
  255 or the widget id placed in `a3`, and the end at 0xc0000000000983f8. Established.
- `xbow_reset_link` (`6.5.30/unix.IP35`, address 0xc000000000099d84, 260 bytes, 48-byte frame): clears the error state
  of one crossbar link and waits for it to come back, returning 0 when it does and -1 when it does not in time. `a0` is
  the vertex of a widget (it is used with `xwidget_info_get`); the routine is the only entry of the table `xbow_provider`. The time base
  is the doubleword at 0x9200000001030100 (pool word 0xc000000001644048), of which only the low 32 bits are used; its
  tick rate is calibrated on the first call, when the word at 0xc00000000163c820 is zero: it reads the counter, calls
  `us_delay(1000)` (0xc000000000069a4c), reads it again and stores the difference in that word, so the word is the
  number of ticks in one millisecond. The routine then takes the widget number from `xwidget_info_get(a0)` (0xc0000000000975e8) and
  `xwidget_info_id_get` (0xc0000000000976cc), finds the crossbar with `hwgraph_traverse(a0, ".master/xtalk/0/xbow", &vertex)`
  (0xc0000000003411fc; the string is at 0xc00000000049a440) and gets its record with `hwgraph_fastinfo_get` (0xc000000000341af8).
  The port block is the record's doubleword at +16 (the register base) plus 64 x (widget number & 7). It saves the word at port
  block +268, stores 0 in the word at +308, delays `xbow_reset_delay` microseconds (the word at 0xc00000000163c81c, which
  holds 20 initially) with `us_delay` and records the counter. It then polls the word at +276: when its bit 31 is set it
  writes the saved word back to +268 and returns 0 (exit 0xc000000000099e5c). Otherwise, when the elapsed ticks (the
  difference of the low 32 bits, since the counter read after the first delay) exceed the calibrated word, that is more
  than about a millisecond, it returns -1 without restoring +268; if not it delays
  `xbow_reset_delay` microseconds again and repeats. What the words at +268, +276 and +308 are in the crossbar's link
  registers: unknown here. Established.
