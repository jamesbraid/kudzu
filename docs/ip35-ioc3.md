# IOC3 driver routines of the IP35 kernel

Routines of the IOC3 PCI I/O controller code in the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF): the base driver (`ioc3.o`) and the serial port attach and shutdown code (`sduart.a(sio_ioc3.o)`). The IOC3 appears to the system as a PCI device with vendor ID 0x10a9 and device ID 3, which is the word 0x000310a9 that the console check below tests for. Its sub-devices are numbered; the serial attach code uses sub-devices 4 and 5 for the two serial ports.

- `ioc3_serial_attach` (`6.5.30/unix.IP35`, address 0xc000000000221394, 1300 bytes, 176-byte frame): attaches the two
  serial ports of an IOC3 and makes one of them the console. `a0` is the IOC3's vertex. It returns -1 (address
  0xc000000000221854) when `ioc3_subdev_enabled(a0, 4)` and `ioc3_subdev_enabled(a0, 5)` both say no, and 0 otherwise.
  The ports it attaches are 0 (sub-device 4) and 1 (sub-device 5), each only when enabled. It maps the controller
  registers with
  `pciio_piotrans_addr(a0, 0, 8, 0, 0x20180, 0)`
  (a window 0x20180 bytes long; the serial registers are 0x20000 above the base it returns). It asks
  `ioc3_is_console(a0)`; if so it also reads a word from the master node's uncached space, the doubleword at
  0x9600000000000000 plus `master_nasid` shifted left 33, plus 0x2000, offset 0x48, masked to 40 bits, and treats the
  controller as the console only if the word at the offset that the first word of that block gives plus 0x1c equals
  0x000310a9. It looks up two neighbouring vertices with `hwgraph_traverse` (the names ".guest" and "ioc3"; the second
  one's fast information, `hwgraph_fastinfo_get`, is kept in each port record at +160). `kmem_zalloc(0x150)` gives two
  168-byte port records. For each enabled port it adds a child vertex named by the pointer table at 0xc00000000158df40 +
  240 + 8 x port (`hwgraph_path_add`), stores the IOC3 vertex at record +48 and the new vertex at +52, attaches the
  record to the vertex (`hwgraph_fastinfo_set`), runs
  `sio_initport(record, vertex, 0x1ff, 1 when this is the console's port 0 else 0)` and allocates a 40-byte lock block
  (`kmem_zalloc(0x28)`, byte +29 set to 3 for the console port and 2 otherwise, `sio_init_locks`), whose pointer goes
  in record +32. The record's baud word at +128 starts at 0x2580 (9600); for the console it records the port's "d"
  edge in the `cons_devs` array (`hwgraph_edge_get`), and reads the kernel option "dbaud" (port 0) or "rbaud" (port 1)
  with `kopt_find` and `atoi`, replacing the baud word when the value is positive and, for port 1, printing "Setting
  rbaud to %d" with `printf`; the pointer `the_kdbx_port` is kept or set to this port.
  `device_inventory_add(vertex, 4, 0x10, -1, 0, 0)`
  registers the port, the register window goes in record +56 and the serial register address (base + 0x20000) in +80,
  and the operations table address 0xc00000000158df88 is stored at +0. For port 0 it then sets up the DMA rings: the
  port's transmit and receive register pointers (+72 and +64 from fields +80 and +56 plus 0x178 and 0xb8), a
  16-kilobyte buffer from
  `kvpalloc(1, 0x100, 0)`
  (a failure panics with "ioc3_uart driver cannot allocate page"), cleared with `blkclr(0x4000)`, receive and transmit
  ring pointers at buffer +0 and +0x1000, calls an unnamed static initialiser at 0xc0000000002210f0 with the record
  and the console flag, finds the "d" edge and connects the interrupt with
  `ioc3_intr_connect(a0, 0x1ff, handler, record, vertex, edge, 0)`, where the handler is the unnamed routine at
  0xc000000000220a6c; the console case also calls `device_controller_num_set(vertex, 1)`,
  `sio_make_hwgraph_nodes(record)` and stores the record in `the_console_port`. Port 1 does the same with ring offsets
  +0x2000 and +0x3000 in the same buffer, interrupt mask 0x3fe00, controller number 2 and no `the_console_port` store.
  Offsets for the record fields come from the decompiled body and the field names are not in the debug information, so
  they are stated as offsets only. Inferred.
- `ioc3_write_ireg` (`6.5.30/unix.IP35`, address 0xc0000000000c4ff4, 408 bytes, 64-byte frame): changes the
  IOC3's interrupt enable bits, keeping a software copy and switching part of the set to a timer poll. `a0` is the
  controller's soft state, `a1` a bit mask and `a2` the operation: 0 sets the bits and 1 clears them; any other value
  changes nothing. The soft state holds the register block pointer at +24 (`R`), a lock at +2876 taken with
  `mutex_spinlock_spl` and released with `mutex_spinunlock`, the software enable word at +2880 and a flag byte at
  +2884 (bit 0 mode on, bit 1 poll wanted, bit 2 timeout scheduled; these three names are inferred from use).
  Operation 0: ORs the mask into the word at +2880. If flag bit 0 is clear and the word has any bit of 0x18000000 set,
  it sets flag bit 0 and writes 0x7fffff to the register word at R +0x24 (this clears the hardware enables of the low
  23 bits). With flag bit 0 now set it checks flag bit 1: when that is clear and the soft word has any bit of 0x7fffff
  set it sets flag bit 1 and, if flag bit 2 was clear, sets flag bit 2 and calls
  `prtimeout_nothrd(master_procid, handler, a0, 1)` (the handler is the unnamed routine whose address is held at
  gp-19712; `master_procid` is the halfword at gp-25584; the last argument is 1). With flag bit 0 set it then writes
  the mask with its low 23 bits cleared (mask & 0xff800000) to the register word at R +0x20; with flag bit 0 still
  clear it writes the mask unchanged to R +0x20. Operation 1: clears the mask bits from the word at +2880. When flag
  bit 0 is set: if flag bit 1 is set and no bit of 0x7fffff remains it clears flag bit 1; if no bit of 0x18000000
  remains it clears flag bits 0 and 1 and writes the whole soft word to R +0x20. In every operation-1 case it then
  writes the mask to R +0x24. The routine returns nothing. Which registers the words at +0x20 and +0x24 are: unknown
  here (a set port and a clear port are likely from the use). Established.
- `ioc3_intr_connect` (`6.5.30/unix.IP35`, address 0xc0000000000c57b4, 632 bytes, 128-byte frame): registers a handler
  for a set of IOC3 interrupt bits and starts a thread to run it. `a0` is the IOC3 vertex, `a1` the interrupt bit
  mask, `a2` the handler, `a3` its argument, `a4` the connecting device's vertex,
  `a5` the device (a descriptor source) and `a6` a value stored unchanged in the slot. It finds the controller's soft
  state through `hwgraph_traverse(a0, "ioc3", ...)` and `hwgraph_fastinfo_get`; when the traverse fails it logs
  "ioc3_intr_connect(%v): ioc3_attach not yet called" at `cmn_err` level 1 and returns with no value
  (0xc0000000000c59e8). It picks a handler slot number from a pool in the soft state: the word at +2872 holds a
  bitmask of slots that were released, and the code atomically clears the lowest set bit there (load-linked and
  store-conditional loop at 0xc0000000000c582c to 0xc0000000000c5838, a branch-likely `beqzl`), looping to try again
  if another processor took that bit first, and uses the number of that bit (a binary search over the bit position
  starting from 15 or 31); if the word is zero it instead atomically adds 1 to the counter at +2864 (loop at
  0xc0000000000c5990 to 0xc0000000000c59a0, branch-likely) and uses the old value. The slot record is at soft + 88 x
  slot, with the 88-byte records starting at soft +48 (the bit mask at +48, the handler at +56, its argument at +64,
  the vertex at +72, a back pointer to the soft state at +80 and `a6` at +128, all relative to the record start plus 0
  for this layout). It then ORs the mask into the used-bits word at +2868 (loop at 0xc0000000000c58c8 to
  0xc0000000000c58d4, branch-likely); if any requested bit was already used it logs "%v: trying to share ioc3 intr
  bits 0x%X" at level 6 and panics with "ioc3_intr_connect: no IOC3 interrupt source sharing allowed". Otherwise it
  duplicates the device descriptor (`device_desc_dup(a5)`), takes its interrupt priority
  (`device_desc_intr_swlevel_get`, or the kernel's default `default_intr_pri` when it is negative), looks up the
  driver bound to the device (`device_driver_getbydev`) and builds the thread name "ioc3[%d]%s" with the slot and the
  driver's name (`device_driver_name_get`, 16 bytes) or just "ioc3[%d]" when no driver is found. It sets bits 0x300 in
  the record's flag word at record +40 (soft + 88 x slot + 88, with an atomic OR loop at 0xc0000000000c5954 to
  0xc0000000000c5960, branch-likely) and calls
  `xthread_setup(name, priority, flag word address, the handler wrapper at 0xc0000000000c6154, record + 0x30)`. The
  routine has no return value; the only returns are after the thread setup and after the first failure message. The
  record layout is read from the stores; no debug type names it. Established.
- `ioc3_attach` (`6.5.30/unix.IP35`, address 0xc0000000000c5278, 1340 bytes): attaches an
  IOC3 controller found on a PCI slot. `a0` is the IOC3's vertex. It maps the configuration space (space 7,
  0x48 bytes) and the register space (space 8, 0x20180 bytes) with two calls to `pciio_pio_addr`; a failed
  configuration mapping logs "%v/ioc3: unable to get PIO mapping for my CFG space" and a failed register mapping frees
  the first map and logs the corresponding "MEM space" message, both at `cmn_err` level 1. It finds the two
  neighbouring vertices with `hwgraph_traverse` (".host", then ".guest" below it, each defaulting to the previous
  vertex when missing), adds the child "ioc3" to `a0` and links it under both with `hwgraph_edge_add`. It allocates
  2888 bytes of soft state with `kmem_zalloc(0xb48)`, initialises the lock at +2876 ("is_ir_lock"), stores the new
  vertex at +0, the host at +4, the guest at +8, the configuration window at +16 and the register window at +24,
  clears the flag byte at +2885, and attaches the record with `hwgraph_fastinfo_set`. If both windows compare equal
  under `hub_check_pci_equiv` to the console's configuration and memory windows (the kernel variables `console_cfg`
  and `console_mem`), it records this controller in a global (the doubleword at 0xc000000001644dd8) as the console
  controller. It registers an error handler (the routine at 0xc0000000000c5e18) on both the host and the guest with
  `pciio_error_register`. It then programs the chip: it sets bits 0x8146 in the configuration word at +4, writes
  0xff00 to the configuration word at +12, writes 0x7ae5e to the register word at +0x28 and 0xc0 to +0x34, masks every
  interrupt with `ioc3_write_ireg(soft, -1, 1)` and writes 0xffffffff to the register word at +0x1c. If the vertex
  name contains "Ibrick/xtalk/15/pci/4" it sets the flag byte at +2885 to 1 and selects the sub-device set from bits
  15:12 of the doubleword at 0x9000000034000000 plus the NASID shifted left 33 (0x8000 gives 0xb1, 0x4000 gives 0x79,
  anything else 0x91) with `ioc3_subdev_enables`; if the name contains neither "baseio" nor "menet" it reads the
  serial number from the part with `nic_ioc3_vertex_info`. An unnamed static routine at 0xc0000000000c63c0 is called
  with `a0`; when it returns 0 the routine stores 0 in the interrupt-object doubleword at +32 and the current
  processor number (the halfword at absolute address -15320) in +40 and returns 0. Otherwise it duplicates the guest's
  device descriptor, sets flag bit 4 and priority 0 and the interrupt name "IOC3 UART", makes it the guest's default,
  allocates the interrupt with `pciio_intr_alloc` (a kind of 2 when the host is the guest and sub-device 0 is enabled
  or the NIC string of the guest contains "030-1657-" or "030-1664-", else 1; a failure logs "ioc3_attach: unable to
  allocate interrupt" and the routine goes no further), stores it at +32 and connects `ioc3_intr` (address
  0xc0000000000c5b90) with `pciio_intr_connect`. It reads the processor that took the interrupt into +40 and the
  processor the descriptor wanted into +42; if they differ it logs "Request to target IOC3 interrupts to CPU %d could
  not be satisfied" at level 4. Otherwise it calls `ioc3_serial_attach(guest)`, and when bits 15:12 of the doubleword
  at 0x9000000014000000 equal 0x4000 also `pckm_attach(guest)` and sets `console_device_attached` to 1 unless the
  console controller recorded above is this one. It returns 0. Which chip fields the bits written to the configuration
  and register words set: unknown here. Inferred.
- `ioc3_handle_intr` (`6.5.30/unix.IP35`, address 0xc0000000000c5bac, 468 bytes): services the pending interrupts of
  an IOC3 by calling the handlers registered with `ioc3_intr_connect`. `a0` is the controller's soft state, or 0 to
  use the console controller recorded in the doubleword at gp+0x1668 (0xc000000001644dd8), and `a1` selects which
  interrupt group is served: 0 clears bits 28:27 from the pending word (mask 0xe7ffffff), and a nonzero value clears
  the low 23 bits (mask 0xff800000) when bit 0 of the controller's flag byte at +2884 is set. With neither soft state
  it does nothing and returns. The pending word comes from an unnamed static routine at 0xc0000000000c518c, which
  takes the controller lock (+2876), ANDs the register word at R +0x1c with the software enable word at +2880 and
  releases the lock. The routine loops until that word is zero. In each pass it goes through the handler slots
  (88-byte records starting at soft +48; the count is the word at +2864; offsets in the record are the mask at +0, the
  handler at +8, the argument at +16, the flag word at +40, the semaphore at +48 and the deferral check at +80). A
  slot whose mask shares bits with the pending word is served: with flag bit 0x1000 of the flag word clear it calls
  the handler with the argument and the shared bits. With that bit set, a nonzero deferral-check pointer at record +80
  is called first with the argument, and a zero answer sends the slot down the direct path; otherwise the slot's mask
  is cleared from the hardware enables through `ioc3_write_ireg(soft, mask, 1)` and `vsema` (address
  0xc00000000023559c) wakes the slot's thread on the semaphore. The bits it served are removed from the working copy.
  After the pass it adds (served count minus one) to the word at +208 of the per-processor structure the doubleword at
  absolute address -15088 points to, and if any pending bit is still unserved it logs "unknown IOC3 interrupt 0x%x"
  with `cmn_err` level 1; then it reads the pending word again. It returns nothing. Inferred.
- `ioc3_serial_kill` (`6.5.30/unix.IP35`, address 0xc0000000002218a8, 280 bytes, 32-byte frame): shuts down one
  IOC3 serial port record, taking the firmware's console hook out of the way for the console port while it does so.
  `a0` is the port record (as built by `ioc3_serial_attach`). If the pointer at record +8 is nonzero it calls the
  routine at offset 40 of the table it points to, with no arguments. When this port is the console (the record equals
  the doubleword `the_console_port` at gp+8224), the doubleword at the system parameter block address
  0xa800000000001000 equals 0x53435241 (the ARC firmware signature word), the pointer stored at 0xa800000000001020 is
  nonzero, the word that pointer addresses equals 0xfeeddead (sign-extended) and the entry at +128 of that table is
  nonzero, it raises the interrupt level (`spl7`, address 0xc00000000006201c), saves the old level in the word at
  gp+8220 and calls the table entry at +128 with 0. It then stores 0 in the word whose address is held at record +64
  (the port's interrupt enable register, from how `ioc3_serial_attach` sets it up; that is inferred). For the console
  under the same test it calls the table entry at +128 with 1 and restores the interrupt level from the word at
  gp+8220 with `splx` (address 0xc000000000062054). It ends by storing 0 in the record word at +148 and returns
  nothing. The table entry at +128 has no name; the 0 and 1 arguments suggest it turns the firmware console off and
  on, which is inferred only from the two calls. Inferred.

## Keyboard and mouse aliases

- `ioc3_pckm_rmalias` (`6.5.30/unix.IP35`, address 0xc0000000000cdd3c, 196 bytes, 32-byte frame): removes the keyboard and
  mouse (`pckm`) alias registered under a given number and marks that device record as having no number. `a0` is the alias number and `a1` selects which of two slots of each controller is
  searched (0 uses the word at +0x2c of the controller, anything else the word at +0x28). It returns nothing. Its callers
  are `usb_pckm_init` and `ioc_pckm_add_hwgraph_alias`. It walks a linked list whose head is the doubleword at the address
  0xc00000000163c978 (initial value 0) and whose next pointer is the doubleword at +0x68 of each element; an
  empty list returns at once. For an element it takes the selected word as a slot index, skips the element when the
  index is 19 (0x13, no device), and otherwise takes the device record at element + 8 x index + 0x18. A record
  whose word at +0x54 differs from `a0` is skipped too. When bit 1 of the record's word at +4 is set the element is
  skipped. For the first matching record with that bit clear it calls `pckm_rmalias(a0, a1)` (0xc0000000000c8bbc), then
  `mutex_lock(element + 0x48, 25)`, stores -1 as the word at record +0x54, calls
  `device_controller_num_set(the first word of the record, -1)` and releases the lock with `mutex_unlock(element + 0x48)`, and
  returns; it stops after the first such record. The lock address is held at 16(sp) across the calls. Established.
- `ioc3_subdev_enabled` (`6.5.30/unix.IP35`, address 0xc0000000000c62a8, 96 bytes, 16-byte frame): says whether one
  of an IOC3 chip's subdevices (serial, keyboard and so on, numbered by bit) is enabled. `a0` is the PCI vertex and
  `a1` the subdevice number; it returns `mask & (1 << a1)` as a 32-bit value, zero or the bit (single exit,
  0xc0000000000c6300, the final `sll v0,v0,0` is the sign-extension). Its callers are `eiattach`, `ioc3_attach`, `pckm_attach`, `if_efattach` and `ioc3_serial_attach`. The mask
  comes from the static routine at 0xc0000000000c61ec (it has no symbol), called with the vertex and a second argument of 0. That routine finds the host vertex with
  `hwgraph_traverse(vertex, ".host", &host)` (string at 0xc00000000049d248; the pointer is initialised to the vertex, so a
  failed traverse keeps it) and then looks for the label "_subdevs" (0xc00000000049d598) with
  `hwgraph_info_get_LBL(host, "_subdevs", &record)`, where the record pointer starts at 0. With the second argument
  zero it never creates a record. The creation path is taken only for a nonzero second argument when the label lookup
  returned zero: it allocates 8 bytes with `kmem_zalloc`, stores 255 in them, adds them as the label with
  `hwgraph_info_add_LBL`, looks the label up again and frees the 8 bytes with `kmem_free` if the stored pointer
  is not the one it just allocated. It returns the record
  pointer. When the pointer is nonzero `ioc3_subdev_enabled` uses the doubleword at its start as the mask. When it is
  zero the mask is 0xdf, unless bits 12 to 15 of the doubleword read from the address 0x9000000014000000 (pool word
  0xc000000001644068) equal 4, in which case it is 0xff. Which hardware register the address 0x9000000014000000 is: unknown here. Established.
- `ioc3_intr_disconnect` (`6.5.30/unix.IP35`, address 0xc0000000000c5a2c, 356 bytes, 48-byte frame): undoes
  `ioc3_intr_connect` for a handler and a set of IOC3 interrupt bits, releasing each handler slot whose bits have all gone.
  `a0` is the IOC3 vertex, `a1` the bit mask (saved at 8(sp)), `a2` the handler, `a3` its argument and `a4` the connecting
  vertex. It returns no value. It finds the soft state with `hwgraph_traverse(a0, "ioc3", &vertex)`
  (0xc0000000003411fc; the string is at 0xc00000000049d258) and `hwgraph_fastinfo_get` (0xc000000000341af8). A failed
  traverse calls `cmn_err(1, "%v: ioc3_intr_connect: ioc3_attach not yet called", a4)` (0xc0000000002ad048; the string is at
  0xc00000000049d338, the name it carries is that of the connect routine) and returns. Otherwise it reads the slot count from the
  word at soft +2864 and scans slots 0 up to that count. Slot i is the 88-byte record at soft + 88 x i: the word at
  +48 holds its bit mask, the doubleword at +56 its handler, the doubleword at +64 its argument and the word at +72 the
  connecting vertex. A slot is taken when its handler, argument and vertex equal `a2`, `a3` and `a4` and the AND of its
  mask with `a1` is nonzero. For a taken slot it clears the overlapping bits in the slot's word at +48 and in the word at soft
  +2868, each with a load-linked and store-conditional loop (the retry branches at 0xc0000000000c5b00 and
  0xc0000000000c5b14 are branch-likely). When the slot's mask is then zero it frees the slot: it stores -1 in the word at +72,
  0 in the doubleword at +64 and 0 in the doubleword at +56, and sets bit i of the word at soft +2872 with a third
  load-linked and store-conditional loop (retry branch 0xc0000000000c5b68, branch-likely). The scan does not stop at the
  first taken slot. The word at soft +2872 is the free-slot bitmask that `ioc3_intr_connect` takes
  the lowest set bit from, and the word at +2864 is its slot counter. What else uses the word at soft +2868:
  unknown here. Established.
- `ioc3_console_vhdl_get` (`6.5.30/unix.IP35`, address 0xc0000000000c61d4, 24 bytes, no stack frame): returns the vertex
  of the console IOC3. It loads the doubleword at 0xc000000001644dd8 (gp+5736), the global that `ioc3_attach` sets to
  the console controller's soft state. When that is zero the branch at 0xc0000000000c61d8 skips the load and the
  routine returns -1, which the delay slot of that branch put in `v0`. Otherwise it returns the word at +0 of the soft
  state, the vertex that `ioc3_attach` stores there. It makes no calls. Established.
- `ioc3_mem_ptr` (`6.5.30/unix.IP35`, address 0xc0000000000c63fc, 8 bytes, no stack frame): returns the doubleword at
  +24 of `a0`, loaded in the delay slot of the return. `a0` is an IOC3 soft state, whose +24 holds the register window
  that `ioc3_attach` maps. There is no null test. Established.
- `ioc3_get_mem` (`6.5.30/unix.IP35`, address 0xc0000000000c6404, 32 bytes, 16-byte frame): the vertex form of
  `ioc3_mem_ptr`. It calls `hwgraph_fastinfo_get` with its own `a0` untouched, then returns the doubleword at +24 of the
  record that call returned (the soft state `ioc3_attach` attached to the vertex), with no null test. Established.
- `ioc3_intr_cpu_get` (`6.5.30/unix.IP35`, address 0xc0000000000c4eec, 8 bytes, no stack frame): returns the halfword
  at +40 of `a0`, sign-extended (`lh` in the delay slot of the return). In an IOC3 soft state that halfword is the
  processor that took the interrupt, which `ioc3_attach` reads into it after `pciio_intr_connect`. Established.
- `ioc3_sio_mlreset` (`6.5.30/unix.IP35`, address 0xc00000000022093c, 44 bytes, 32-byte frame): registers two routine
  addresses with the kernel console layer. It builds a two-doubleword block on the stack, the address
  0xc000000000223d88 at sp+0 (pool word at 0xc000000001640a50) and the address 0xc000000000223c34 at sp+8 (pool word at
  0xc000000001640a58), and calls `sio_console_register` with the block's address. That routine copies the two
  doublewords to +1160 and +1168 of the structure `cn` and returns nothing; this routine then returns whatever `v0`
  holds. Both addresses are unnamed static routines inside the address range of `ioc3_serial_kill`; their bodies were
  not read for this entry, so what the console layer uses them for is unknown here. Established.
- `ioc3_subdev_enable` (`6.5.30/unix.IP35`, address 0xc0000000000c6334, 68 bytes, 16-byte frame): turns on one
  sub-device bit in an IOC3's sub-device mask. `a0` is the IOC3 vertex and `a1` the bit number. It saves `a1` at sp+8
  and calls the unnamed static routine at 0xc0000000000c61ec with `a0` and 1. That routine finds the mask: it runs
  `hwgraph_traverse` with the string ".host" (at 0xc00000000049d248, the pool word at gp-19704 plus 456), leaving the
  result in the stack slot that first held `a0`, reads the label "_subdevs" (at 0xc00000000049d598, pool word plus 1304)
  of the vertex in that slot with `hwgraph_info_get_LBL`, and when the label is missing and its second argument is nonzero allocates 8 bytes with
  `kmem_zalloc`, stores 255 in them, adds them as that label with `hwgraph_info_add_LBL`, reads the label back and frees
  its own copy with `kmem_free` if the label now holds a different pointer (another caller won). It returns the pointer
  to the doubleword, or 0 when there is none. Back here, a zero result ends the routine. Otherwise the routine ORs
  `1 << a1` into that doubleword with a `lld`/`scd` loop, retrying through the branch-likely `beql` at
  0xc0000000000c6368. The shift is the 32-bit variable shift `sllv`, so only the low five bits of `a1` count. The
  routine leaves the helper's result in `v0` and has no other return value. Established.
- `ioc3_subdev_disable` (`6.5.30/unix.IP35`, address 0xc0000000000c6378, 72 bytes, 16-byte frame): the clearing twin
  of `ioc3_subdev_enable`, with the same arguments and the same call of the static routine at 0xc0000000000c61ec
  (second argument 1, so a missing mask is created as 255 first). A zero result ends the routine. Otherwise it forms
  `1 << a1` with `sllv`, inverts it with `nor` against zero and ANDs it into the doubleword with a `lld`/`scd` loop
  whose retry is the branch-likely `beql` at 0xc0000000000c63b0. The helper's result is left in `v0`. Established.
- `ioc3_is_console` (`6.5.30/unix.IP35`, address 0xc0000000000c5d80, 88 bytes, 16-byte frame): says whether the IOC3
  vertex `a0` is the console controller. It returns 0 when the doubleword at 0xc000000001644dd8 (gp+5736, the console
  controller's soft state, set by `ioc3_attach`) is zero. Otherwise it calls `hwgraph_traverse(a0, "ioc3", &sp+0)`, the
  string at 0xc00000000049d258 (pool word at gp-19704 plus 472), and returns 0 if that call returns nonzero. If it
  succeeds it calls `hwgraph_fastinfo_get` on the vertex found and returns 1 when the result equals the global and 0
  when it does not (`xor` then `sltiu` against 1). Established.
- `ioc3_is_io7` (`6.5.30/unix.IP35`, address 0xc0000000000c5dd8, 64 bytes, 16-byte frame): returns the flag byte that `ioc3_attach`
  keeps in an IOC3's soft state. It calls `hwgraph_traverse(a0, "ioc3", &sp+0)` (the string at 0xc00000000049d258, pool word at
  gp-19704 plus 472) and returns 0 when that returns nonzero. Otherwise it calls `hwgraph_fastinfo_get` on the vertex
  found and returns the byte at +2885 of the soft state (`lbu`), the flag `ioc3_attach` sets to 1 for the vertex name
  containing "Ibrick/xtalk/15/pci/4". What the flag stands for beyond that is the kernel's own naming. Established.
- `ioc3_mlreset` (`6.5.30/unix.IP35`, address 0xc0000000000c4ef4, 116 bytes, 16-byte frame): records the console
  IOC3's configuration and register windows and, unless the chip reports one of two other identifiers, registers the
  IOC3 serial port as a console device. `a1` is stored in the kernel variable `console_mem` (0xc00000000163c938) and
  `a0` in `console_cfg` (0xc00000000163c930); `ioc3_attach` later compares them with each controller's windows. The
  routine then forms an uncached address B from the halfword `master_nasid` (the pool slot at gp-32632): `master_nasid`
  shifted left 33 plus 0x9600000000000000. It reads the doubleword at (B | 0x2000) + 72, keeps its low 40 bits and ORs
  them into B to get X. It reads the word at X + 16, adds it to X and reads the word at +28 from the sum, the word that
  `ioc3_serial_attach` compares with 0x000310a9. When that word equals 0x0300fafa or 0x0300fafb the routine returns at
  once. Otherwise it calls `ioc3_sio_mlreset` and returns. What the two other identifiers belong to is unknown here.
  There is no return value. Established.
- `ioc3_ei_intr` (`6.5.30/unix.IP35`, address 0xc0000000000c42a0, 308 bytes, 48-byte frame): the handler that `eiattach`
  connects with `ioc3_intr_connect` for the interrupt mask 0x18000000, with its own state record as the argument. `a0`
  is that record (56 bytes, from `eiattach`'s `kmem_zalloc`). The offsets used are the register window pointer at +8
  (`eiattach` stores `ioc3_mem_ptr`'s result there), a word at +16 (the mask passed to `ioc3_write_ireg`; `eiattach`
  stores 0 there after connecting), a spin lock whose address is record + 20, the head of a list at +24, a pointer at +32 to a page that
  `eiattach` allocates with `kvpalloc`, a word at +44 (`eiattach` stores -1) and the IOC3 soft state at +48. The
  routine first stores 0x18000000 in the word at +0x1c of the register window, the word `ioc3_handle_intr` reads as the
  pending word; what a store there does to the chip is unknown here. It then reads the word at the absolute address
  0xffffffffffffc6a8 (-14680). When that is nonzero it calls `frs_handle_eintr`, a stub whose whole body is `jr ra`.
  When it is zero it reads the record word at +44: a nonnegative value is passed to `uli_callup`, and a negative value
  (the value `eiattach` leaves) takes the third path. That path takes the lock at the address record + 20 with `mutex_spinlock`,
  adds 1 to the word at the start of the page at record +32 (a plain load, add and store under the lock), and walks the
  list from record +24 through the link at +8 of each node. For each node it adds 1 to the word at node +16 with an
  `ll`/`sc` loop whose retry is the branch-likely `beql` at 0xc0000000000c435c. When the word 20 bytes into the node is
  nonzero it calls `sigtopid` with the node's first word, that same word 20 bytes in, 18 and three zeros. It then calls
  `nested_spinlock(node+24)`, `sv_signal(node+32)` and `nested_spinunlock(node+24)`. After the list it calls
  `mutex_spinunlock` for the record lock with the level `mutex_spinlock` returned. All three paths end by calling
  `ioc3_write_ireg` with the soft state from +48, the word at +16 and operation 0, and the routine returns nothing. What
  the kernel treats these external interrupts as: unknown here. Established.
- `ioc3_init` (`6.5.30/unix.IP35`, address 0xc0000000000c51e8, 88 bytes, 16-byte frame): registers the IOC3 driver
  with the PCI layer and three part-number prefixes with the NIC code. It calls `pciio_driver_register(4265, 3, S + 48,
  1)`: 4265 is 0x10a9, the vendor, and 3 the device that `ioc3_serial_attach` also tests; S is the pool word at gp-19704
  (0xc00000000049d080, the string "030-1155-") and S + 48 is the string "ioc3_" at 0xc00000000049d0b0. It then calls
  `nic_vmc_add` three times, each with a string and a routine address: S and 0xc0000000000c5240 (pool word at
  gp-19696), S + 16 ("030-1657-") and 0xc0000000000c525c (pool word at gp-19688), and S + 32 ("030-1664-") with the
  same 0xc0000000000c525c. `ioc3_attach` tests the NIC string of its guest against the last two prefixes. The return
  value is whatever the last `nic_vmc_add` leaves in `v0`. What the two routines do is not read here. Established.
- `ioc3_pckm_mlreset` (`6.5.30/unix.IP35`, address 0xc0000000000cb808, 76 bytes, 64-byte frame): registers six routine
  addresses for the console keyboard with the console layer. It loads six pool words and stores them in a block on the
  stack, in the order issued: the word at gp-19448, 0xc0000000000cb854, at sp+0; gp-19440, 0xc0000000000cd074, at
  sp+8; gp-19424, 0xc0000000000cd0f0, at sp+24; gp-19416, 0xc0000000000cd134, at sp+32; gp-19432, 0xc0000000000cd0b0,
  at sp+16; and gp-19408, 0xc0000000000cd160, at sp+40 (in the delay slot of the call). It calls
  `pckm_console_register` with the block's address; that routine copies the six doublewords in order to +1112, +1120,
  +1128, +1136, +1144 and +1152 of the structure `cn` and returns nothing, and this routine returns what `v0` holds. All
  six are unnamed static routines; the first, 0xc0000000000cb854, is only `jr ra` with a `nop` delay slot. The other
  five were not read for this entry. Established.

### The same routines in the 6.5.22 builds

The 6.5.22 copies of the routines above. Each row is a routine whose bytes match a 6.5.30 routine on the same page; the note gives the SHA-256 prefix of the bytes (first 12 hex digits). A linked-kernel row is established when the whole body has no calls, address constants or gp accesses; the others are inferred, because the 6.5.22 target of each constant or call was not checked. An object row compares section bytes, not relocated code, so it is inferred. The comparison is in [version-diffs.md](version-diffs.md).

| Routine | Binary | Address | Size | Note | Label |
|---|---|---|---|---|---|
| `ioc3_intr_cpu_get` | `6.5.22/unix.IP35` | `0xc0000000000c7294` | 8 | returns the halfword at +40 of `a0`, sign-extended (`lh` in the delay slot of the return); same bytes as the 6.5.30 linked kernel (SHA-256 prefix e03406f826de), at a different address; no calls, address constants or gp accesses in the body | Established |
| `ioc3_mem_ptr` | `6.5.22/unix.IP35` | `0xc0000000000c8788` | 8 | returns the doubleword at +24 of `a0`, loaded in the delay slot of the return; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 402de11c1126), at a different address; no calls, address constants or gp accesses in the body | Established |
