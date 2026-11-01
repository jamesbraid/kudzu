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
