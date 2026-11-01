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

## Interrupt connections

- `pcibr_intr_alloc` (`6.5.30/unix.IP35`, address 0xc0000000000a4f8c, 1552 bytes, 448-byte frame): allocates a PCI
  interrupt object for one device and ties each of the device's interrupt pins to a shared bridge interrupt bit. `a0`
  is the device vertex, `a1` an optional device descriptor, `a2` a bit mask of the pins wanted (bit 0 INTA, bit 1
  INTB, bit 2 INTC, bit 3 INTD; the pins are named only in the debug line) and `a3` the connection vertex. It takes
  the device's info record from `pcibr_info_get` (its byte +540 is the slot, the pointer at +40 the bridge's soft
  state). It returns 0 when `kmem_zalloc(160)` fails (address 0xc0000000000a5480) and when a needed crosstalk
  interrupt cannot be allocated and none was installed by another caller (after `cmn_err` level 1,
  0xc0000000000a5580); otherwise the new object (a 160-byte record). With no descriptor, or with one whose flags lack
  bit 2, the object records a target cpu of -1 or the one the descriptor names (`intr_target_from_desc`, switch level
  `device_desc_intr_swlevel_get`) and is marked as a thread-less direct interrupt (first word 0); with bit 2 set it is
  first word 2 and takes no cpu from the descriptor. The record gets the vertex at +4, the pin mask at +16, the soft
  state at +96 and its own lock (+104). The unnamed routine reached through the soft-state pointer at +1288
  (`bs_intr_bits`, an indirect call with the info record, the pin mask and the slot count) returns the bridge
  interrupt bits to use; they are logged ("pcibr_intr_alloc: pcibr_int_bits"). For each of the 8 bridge bits set in
  that result it uses the per-bit record at soft +1648 + 56 x bit (`bs_intr[]`, eight 56-byte records ending at
  +2096): if its first doubleword, the crosstalk interrupt, is null it calls
  `xtalk_intr_alloc_nothd(master, descriptor, a3)` and installs it with a compare-and-swap; the loser frees its copy
  (`xtalk_intr_free`) and uses the winner's; the winner sets the bit's field in the bridge's device-interrupt register
  (`pcireg_intr_device_bit_clr` of the three bits for that bit position, then `pcireg_intr_device_bit_set` with the
  slot number in those bits). For a direct object it reads the cpu (`xtalk_intr_cpuid_get`), warns with `cmn_err`
  level 4 when the bits it uses sit on two different cpus or when the cpu requested differs from the one allocated
  (printing the vertex name), and keeps the requested one. It then ORs the bit into the object's bit mask (+88),
  allocates a 32-byte list entry and appends this object to the bit's lock-free list (the list head is the pointer at
  record +24, record +1672 + 56 x bit in soft state, and the entries are linked with repeated `compare_and_swap_ptr`;
  a word at record +36 is set to 1 when an erased entry was reused), logging whether it was the first, second or Nth
  user of the bit and whether it replaced an erased entry. When the loop is done and the object is a direct one it
  calls an unnamed static routine that sits just before `pcibr_intr_alloc` in `pcibr.o` (it takes the object, the bit
  mask and the switch level; its body was not read) and then logs completion with the interrupt name from the
  descriptor (+16) or an empty string. Several branches were summarised from the decompiled body, not from the
  instructions; the list-append loop was not stepped instruction by instruction. Inferred.

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
