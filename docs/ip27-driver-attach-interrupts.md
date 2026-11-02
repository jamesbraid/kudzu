# IP27 kernel: driver attach and interrupt routines

Routine-by-routine notes on how the device drivers of the IRIX 6.5.30 IP27 kernel attach to their hardware and what their interrupt handlers read,
acknowledge and clear. The code is in the linked kernel `6.5.30/unix.IP27`; most of these routines are file-local and have no symbol in it, so each
entry gives the routine's address and size in that image. The name and the boundaries come from the debug information and from the kernel object files
they were linked from (`usr/cpu/sysgen/IP27boot/` of release 6.5.30), whose text sits at a fixed distance from the image: the QLogic driver (`ql.o`)
at 0xc0000000000c6d2c, the IOC3 base driver (`ioc3.o`) at 0xc000000000094ef0 and its external-interrupt helper (`ioc3ei.o`) at 0xc000000000093fb0,
each added to the object-relative address. The debug information of these objects carries routine names but no structure types, so "record +N" is byte
N of the driver's per-controller record, read from the instructions, and a register of the device is named by its byte offset in the block the driver
mapped. Meanings given to a record word or to a device register are inferred from how the code uses it unless the entry says otherwise. Labels follow
`AGENTS.md` section 2.

## QLogic SCSI controller

The driver (`ql.o`) registers for PCI vendor 0x1077 and the device numbers 0x1020, 0x1080, 0x1240, 0x1280 and 0x1216. The first doubleword of the
controller record (record +0) is the address of the chip's 256-byte register block, and the registers are 16-bit. Every halfword register the driver
touches sits at an offset whose bit 1 is the opposite of the register number in published QLogic ISP1020/1040-family documentation (for example the
interrupt status register is read at +8 where the documentation puts it at 0xa, and the host command and control register at +194 where the
documentation puts it at 0xc0). This is the signature of a 16-bit access through the PCI bridge's byte-swapping window, so the entries below give the
offsets used by the code and say which register the code treats them as; that mapping is **inferred**. The flags word at record +40 is used the same
way everywhere (bit 0x1 attached, 0x2 interrupts enabled, 0x4 a mailbox command has completed, 0x8 a mailbox waiter is sleeping on the semaphore at
record +224, 0x10 a chip reset ran, 0x20 the board is declared dead, 0x40 the interrupt handler is running, 0x100 new responses were seen); each bit's
meaning is inferred from the places that set and test it.

- `qlintr` (`6.5.30/unix.IP27`, address 0xc0000000000c8964, 136 bytes, 32-byte frame): **established.**
  The interrupt entry of the QLogic SCSI driver; `ql_init_board` connects it with `pciio_intr_connect`. `a0` is the controller record (kept in `s0`).
  It saves the doubleword at record +0, the register block address, at sp+0 and calls `mutex_lock(a0+208, 20)` (0xc0000000000c8984); the lock address
  is saved at sp+8 in the call's delay slot. It reads the flags word at +40. When bit 0x20 (board dead) is set it does no chip access at all.
  Otherwise it sets bit 0x40 in the word (store at 0xc0000000000c89c4), reads the halfword at register offset +8 (0xc0000000000c89c8), and only when
  bit 0x4 of it is set calls `ql_service_interrupt(s0)` (0xc0000000000c89dc); it then reloads the flags word. Both paths join at 0xc0000000000c899c:
  the flags word with bit 0x40 cleared (an AND with -65) is stored back in the delay slot of the `mutex_unlock` of the saved lock (0xc0000000000c89a8;
  the store at 0xc0000000000c89ac therefore runs before the unlock), and the result of `mutex_unlock` is the return value. The bit-0x20 path also
  stores the word back with bit 0x40 cleared. It writes nothing to the chip itself; acknowledgement is done by the routine it calls. Reading bit 0x4
  of the register at +8 as "the RISC processor has an interrupt pending" is inferred.

- `ql_service_interrupt` (`6.5.30/unix.IP27`, address 0xc0000000000c89ec, 2264 bytes, 272-byte frame): **inferred.**
  The body of the QLogic interrupt service. Every chip access was read; the bookkeeping that finishes a SCSI command (locks, counters, sense copy,
  result codes) was read once and is summarised, which is why it is graded inferred. `a0` is the controller record (`s0`); `s5` holds the
  register block address from record +0 and `s3` the base of the read-only strings. On entry it clears bits 0x10, 0x2000 and 0x4000 of the flags word
  at +40 (an AND with -24593, store at 0xc0000000000c8a3c) and enters the loop at 0xc0000000000c8a6c:
  Step 1: It reads the halfword at register +14 and, when its bit 0 is set, calls `ql_service_mbox_interrupt(s0)` (0xc0000000000c8a80), which clears
  that register.
  Step 2: It clears bit 0x100 of the flags word, reads the halfword at register +120 (the chip's response-queue in-pointer) into the doubleword at
  sp+208 and the halfword at record +76, and compares it with the driver's out-index, the halfword at record +74. When they are equal, or flag bit 0x1
  is clear, it goes to the parity check (step 6).
  Step 3: For each response entry between the out-index and the in-pointer (64 bytes each; the ring has 256 entries, the index wraps by reloading the
  base from record +112 after index 255 and the entry pointer is the doubleword at record +104) it stops at once, jumping to the early exit in step 5,
  if the entry's word at +4 is already 0xdead. Otherwise it byte-swaps the 64 bytes in place with `munge(entry, 64)` (reverses the bytes of each
  word), advances the out-index and the pointer, and tests the entry's byte 0 against 0xe: bit 0x2 prints `Detected busy condition`, bit 0x4
  `Detected a bad header` and bit 0x8 `Detected a bad payload` (`ql_ppmsg`), and the entry is dropped. With none of those bits, an entry whose byte 3
  is not 3 prints `Invalid driver command issued` with its bytes 2, 3, 0 and 1 and its word at +4 and is dropped. A status entry (byte 3 equal to 3)
  names its bus in bit 12 of the word at +4 (a bus number at or above the halfword at record +68 reaches
  `panic("port is greater or equal channel count")`), its target in bits 8 to 11 and a 16-byte slot, the logical unit, in bits 0 to 7. When that
  slot's command pointer is empty the entry is skipped. Otherwise it decrements the per-target count (word at bus +48+4*target) and the per-bus count
  (+392), stores 0xdead in the entry's word at +4 so it is not handled twice, copies the entry's status halfword at +10 (byte at command +100) and the
  word at +20 (command +104), maps the completion code, the halfword at +8, to a result in command +96 (codes below 22 through a jump table in
  `.rodata`; others print `HARD ERROR: completion status [0x%x] (%s) ...` through `ql_completion_status_msg` and `ql_tpmsg` and set result 2), copies
  sense data with `bcopy` when the entry's halfword at +14 has bit 0x2000 (printing `No place for sense data` when the command has no buffer), takes
  the logical unit's lock at +84 with `mutex_spinlock`, adjusts its state words at +16 and +24 and moves its deferred queue (head +32, tail +40) onto
  the bus record's list (head +400, tail +408, linked through +80), and links the command into a local list headed at sp+192 for completion.
  Step 4: When the out-index equals the in-pointer it writes the out-index to register +120 (0xc0000000000c8a40) and 0x7000 to the halfword at
  register +194 (0xc0000000000c8a48), then calls `ql_notify_responses(s0, list)` (0xc0000000000c8a50), which drops the driver lock and calls each
  finished command's completion function. It loops back to step 1 when the halfword at register +8 still has bit 0x4 set or flag bit 0x100 has been
  set meanwhile.
  Step 5: Early exit and finish: the 0xdead case writes the out-index to register +120 (0xc0000000000c8d98), calls `ql_notify_responses`, and, like
  the normal finish, then scans the buses: when any bus record whose status word (offset 8 of that record) has bit 0x8 clear has a deferred list
  (offset 400 or 32 nonempty) it calls `ql_start_scsi(s0)` to start the queued commands before returning.
  Step 6: Parity check (0xc0000000000c8ca0): when the halfword at record +44 is 0x1020 (the ISP1020) it reads the halfword at register +194. Bit 0x400
  of it means an SRAM parity error. With flag bit 0x200 already set it writes the value it read, with bits 0x8000, 0x2000 and 0x100 cleared (an AND
  with the 8-byte `.lit8` word at offset 0, 0xffffffffffff5eff), then 0x7000, 0xa100 and 0x3000 to register +194 (a `flushbus` call follows the last
  write; the store is in its delay slot) and waits 30 microseconds. With bit 0x200 clear it prints `Q-logic sram parity error detected`, then
  `... on bank 0 hccr=%x` when bit 0x100 is set and `... on bank 1` when bit 0x200 is set, calls `ql_reset_interface(s0, 1)`, and flushes every bus
  with `ql_flush_queue`: reason 2 when the reset returned zero, and when it failed it first sets flag bit 0x20 in the word at +40 and uses reason 1.
  The last chip write of this path is 0x7000 to register +194 (0xc0000000000c8d64). The routine returns no value. Reading register +8 as the interrupt
  status, +14 as the semaphore, +120 as the response in/out pointer and +194 as the host command and control register, with 0x7000 as "clear RISC
  interrupt", is inferred from the published ISP register map.

- `ql_service_mbox_interrupt` (`6.5.30/unix.IP27`, address 0xc0000000000ce490, 568 bytes, 48-byte frame): **established.**
  Handles a mailbox interrupt: either the completion of a command the host issued, or an asynchronous event from the chip. `a0` is the controller
  record (`s0`), `s3` the register block. It reads the halfword at register +114 (the first mailbox register; `a2`). Three cases: Value 0x4000: it
  copies eight mailbox registers into the record, reading in this order and storing as halfwords: register +124 to record +258, +126 to +256, +120 to
  +254, +122 to +252, +116 to +250, +118 to +248, +112 to +246 and +114 to +244 (0xc0000000000ce57c to 0xc0000000000ce5c0), and notes a completion. A
  value of 0x8000 or less other than 0x4000: it prints `mailbox_service_interrupt error.  Mailbox0=%x` through `ql_cpmsg` and notes a completion. A
  value at or above 0x800f prints the same line and notes no completion. A value from 0x8001 to 0x800e is an event (an index of value minus 0x8001
  into a 14-entry jump table in `.rodata`). It selects the bus record from bit 0 of the halfword at register +126 when the halfword at record +68 is 2
  or more, and otherwise bus 0. The table sends 0x8001 to 0xc0000000000ce6a0 (sets bit 0x10 in the bus record's word at +8 and prints
  `SCSI bus RESET seen.`), 0x8002 to 0xc0000000000ce5c4 (prints `Unrecoverable SCSI bus state/condition (mbox1=0x%x mbox2=0x%x)` with the registers at
  +112 and +118, calls `ql_reset_interface(s0, 1)`, and when that returns nonzero sets flag bit 0x20, then calls `ql_flush_queue` on every bus: reason
  5 after a successful reset, 1 after a failed one), 0x8006 to 0xc0000000000ce68c, 0x800a to 0xc0000000000ce678, 0x800c to 0xc0000000000ce664 and
  0x800d to 0xc0000000000ce654 (each one `ql_cpmsg` line: `Execution timeout and SCSI bus reset is asserted.`,
  `Extended message underrun and Device Reset is asserted.`, `SCSI bus hang due to data overrun -- SCSI bus reset needed.` and
  `SCSI bus reset by isp due to data overrun.`) and 0x800e to 0xc0000000000ce638 (stores the register at +112 in record +246 and bits 10 to 12 of it
  in the halfword at the bus record's +418; a bus mode report, inferred). The seven other values in the range (0x8003, 0x8004, 0x8005, 0x8007, 0x8008,
  0x8009 and 0x800b) take the error line above. An event does not set the completion note. The acknowledgement is the same for every case: the common
  exit stores a zero halfword in the register at +14 (the delay slot at 0xc0000000000ce53c, which runs for both outcomes). On a completion it then
  sets flag bit 0x4 in the word at record +40 and, when bit 0x8 is set, calls `vsema(record+224)` to wake the sleeping command issuer. It returns no
  value. Reading +14 as the host semaphore and +114 as mailbox 0 is inferred from the register map.

- `ql_enable_intrs` (`6.5.30/unix.IP27`, address 0xc0000000000c7bb0, 28 bytes, no frame): **established.**
  Turns on the chip's interrupt output. `a0` is the controller record. It loads the register block address from record +0, stores the halfword 6 at
  register offset +10 (0xc0000000000c7bb8) and sets bit 0x2 of the flags word at record +40 (store in the delay slot of the return). The value left in
  `v0` is the constant 6, not a result. Treating +10 as the interrupt control register and bits 0x2 and 0x4 as "enable PCI interrupt" and "enable RISC
  interrupt" is inferred.

- `ql_mbox_cmd` (`6.5.30/unix.IP27`, address 0xc0000000000ca108, 1056 bytes, 176-byte frame): **established.**
  Issues one mailbox command to the chip and collects the reply. Arguments: the controller record (`a0`, `s0`), a pointer to eight halfwords for the
  reply (`a1`, `s2`), the number of mailbox registers to load (`a2`, 0 to 8), the number of reply registers to return (`a3`, 0 to 8), the command code
  (`a4`) and then seven more values: three in `a5`, `a6` and `a7` (saved at sp+32, sp+24 and sp+16) and four taken from the caller's stack (the low
  halfwords of the doublewords at sp+182, sp+190, sp+198 and sp+206). It returns 0 on success and 1 on a timeout.
  Locks: it releases the record lock at +208 (held by the caller), takes the mailbox lock at +192 and then the record lock again; the mailbox lock is
  dropped before return, the record lock is left held. Staging: it stores zero in the eight reply halfwords, in the eight halfwords of record +244 to
  +258 (the staging area `ql_service_mbox_interrupt` fills) and then loads the mailbox registers through a nine-entry jump table indexed by `a2`. For
  `a2` of n the stores are made from the top down, each entry falling into the next: register +124 (stack word at sp+206), +126 (sp+198), +120
  (sp+190, skipped when the command is 56 or 9), +122 (sp+182, skipped when the command is 50, 83 or 56), +116 (`a7`), +118 (`a6`), +112 (`a5`) and
  finally +114 (the command code); an `a2` of 0 stores nothing and `a2` above 8 skips the table. Kick: when none of the bits 0x14c0 of the flags word
  at +40 is set (an interrupt-driven wait) it sets flag bit 0x8 and stores 20 in the word at record +240. In both cases it clears flag bit 0x4, then
  writes 0x5000 to the halfword at register +194 (0xc0000000000ca274), which hands the command to the chip (inferred: the host-interrupts-RISC
  command). Wait, interrupt mode (bits 0x14c0 clear): it drops the record lock, sleeps with `psema(record+224, 20)`, retakes the lock and clears flag
  bit 0x8. Wait, polling mode (any of the bits 0x14c0 set, which includes bit 0x40 that `qlintr` sets while the handler runs): it counts 100 retries
  (`s5`) and 100000 polls (`s3`) and, unless flag bit 0x4 is already set, loops with `us_delay(10)` (0xc0000000000ca3d8). Each pass reads the halfword
  at register +8. With bit 0x4 clear it reads the halfword at +194 and stops waiting, treating the command as finished, when both bits 0x80 and 0x20
  are set (printing `mailbox with host interrupt hccr 0x%x` when the global `showconfig` is set). With bit 0x4 set it reads the halfword at +14: bit 0
  set (a mailbox event) costs one retry, and after the hundredth the routine calls `ql_service_mbox_interrupt(s0)` itself and, when any of bits 0x1480
  is set in the flags, writes 0x7000 to +194; bit 0 clear (a response-queue event) reads the in-pointer at +120, and when it differs from the halfword
  at record +76 stores it there and sets flag bit 0x100, then writes 0x7000 to +194. A set flag bit 0x4 ends the loop. Timeout (after 100000 polls):
  it re-reads +114, +8, +14 and +10 (the value of +194 is the one from the last poll). When that last value had bit 0x80 and the status had bit 0x4 it
  prints `Mailbox timeout:  (command 0x%x, mbox0 0x%x, isr 0x%x, bus_sema 0x%x, hccr 0x%x) resetting ISR and retrying`, writes 0x7000 to +194 and
  starts the polling over with fresh counters. Otherwise it prints
  `Mailbox timeout: intr_retry_cnt %d, command 0x%x, mbox0 0x%x, isr 0x%x, bus_sema 0x%x, hccr 0x%x  icr 0x%x`, and returns 1 after the reply copy
  below. Reply: a second nine-entry jump table indexed by `a3` copies the staged halfwords at record +244 to +258 into the caller's array, element k
  from record +244+2k; element 4 is skipped when the command is 50 or 83 and element 5 when it is 9. The staged words are in mailbox-number order:
  element 0 is register +114, 1 is +112, 2 is +118, 3 is +116, 4 is +122, 5 is +120, 6 is +126 and 7 is +124, matching `ql_service_mbox_interrupt`.

- `ql_init_isp` (`6.5.30/unix.IP27`, address 0xc0000000000c7a60, 336 bytes, 32-byte frame): **established.**
  Brings the chip to a working state: queue memory, firmware, reset. `a0` is the controller record (`s0`); `a1` (`s2`), when nonzero, tells it to take
  the record lock at +208 itself (`mutex_lock(record+208, 20)`) and release it before returning. It returns 0 or a step number. If the doubleword at
  record +112 is empty it allocates a page (`kvpalloc(1, 0x8101, 0)`) for the response queue and, when that fails, returns 1; the same allocation
  fills record +96 (the request queue) and on failure frees the response queue, clears record +112 and returns 2. It then sets the request queue
  pointer at +88 to the base held at +96, the response pointer at +104 to the base held at +112, and zeroes the halfwords at +74, +70 and +72 and the
  word at +80. `ql_download_fw(s0)` loads the firmware; a nonzero return gives 3. On success the global `ql_bus_reset_on_boot` decides the argument of
  `ql_reset_interface(s0, flag)`: 0 when the global is -1, 1 otherwise; a nonzero result returns 4, and zero gives 0. When the return value is nonzero
  it prints `ql_init_isp failed (%d)` with the step number, through `ql_cpmsg` for the identifier at record +32 and, for a controller with two or more
  buses (halfword at +68), again with the identifier at +36. It does not touch the chip registers directly; all device access is in the two routines
  it calls.

- `ql_reset_board` (`6.5.30/unix.IP27`, address 0xc0000000000cad84, 356 bytes, 64-byte frame): **established.**
  Recovery path used when a bus reset or hang forces the whole controller to be reset. `a0` is the controller record (`s2`). First, when flag bit 0x8
  is set in the word at +40 it clears it and wakes the sleeper with `vsema(record+224)`. Then: when flag bit 0x4000 is already set (inferred: a
  reset's follow-up is still pending) it overwrites the flags word with 0x20, prints `PCI slot reset failed.  Declaring board dead` through `ql_ppmsg`
  and uses reason 1; when 0x4000 is clear and flag bit 0x10 is set it only sets 0x4000 (store at 0xc0000000000caea4) and uses reason 5, leaving the
  chip alone; when both are clear it prints `Resetting SCSI chip.` and calls `ql_reset_interface(s2, 1)`; a zero result sets flag bit 0x10 and uses
  reason 5, a nonzero result overwrites the flags word with 0x20 and uses reason 1. Then for each bus record (the pointers at +16, count in the
  halfword at +68) whose word at +8 has bit 0x40 set it calls `undo_quiesce(bus, 1)` and clears bits 0x40, 0x8 and 0x80 of that word (an AND with
  -201); it calls `ql_flush_queue(bus, reason)` and clears bit 0x100. It reads or writes no chip register itself. It returns no value.

- `qlinit` (`6.5.30/unix.IP27`, address 0xc0000000000cb2bc, 176 bytes, 16-byte frame): **established.**
  The driver's one-time initialisation, run at boot. It calls `pciio_driver_register(0x1077, id, "ql", 0)` for five PCI device numbers, in this order:
  0x1020 (0xc0000000000cb2c4 to 0xc0000000000cb2d4), 0x1080, 0x1240, 0x1280 and 0x1216, which makes `qlattach` run for each matching board. It then
  calls `init_mutex` on the driver-wide lock in `.bss` (name `qldriverlock`) and clears three variables, in this order: a counter in `.sbss`, the head
  of the controller list `ql_chip_list` (0xc0000000000cb35c) and a second `.sbss` word, the flag that tells `ql_init` whether the watchdog timer was
  started yet. It touches no hardware and returns no value.

- `ql_init_board` (`6.5.30/unix.IP27`, address 0xc0000000000c75e0, 1152 bytes, 96-byte frame): **established.**
  Creates the controller record for one QLogic board and starts the chip. `a0` is the PCI vertex (`s5`) and `a1` the mapped register block (saved at
  sp+16). It returns the controller number of the first bus, or -1.
  Identification: it reads config offset 0 (2 bytes, vendor), offset 2 (2 bytes, device) and offset 8 (1 byte, revision) with `pciio_config_get`. A
  vendor other than 0x1077, or a device other than 0x1020, 0x1240, 0x1080, 0x1280 or 0x1216, prints `ql_init_board failed (1)` (`cmn_err` level 6) and
  returns -1. The device sets two values: the bus count (1 for 0x1020 and 0x1080, 2 for 0x1240, 0x1280 and 0x1216) and a clock figure kept at sp+0 (60
  for 0x1020 and 0x1240, 100 for 0x1080 and 0x1280, 80 for 0x1216).
  Record: `kmem_zalloc(528, 0x901)`; a failure prints `ql_init_board failed (4)` and returns -1. It fills the name at +264 (`sprintf("%v", vertex)`),
  the halfword device at +44, the revision byte as a halfword at +46, the result of `pcibr_asic_rev(vertex)` at +48, the clock at +52, the bus count
  at +68, the vertex at +8 and the register block pointer at +0. The firmware image is chosen by device: 0x1020 the 1040 image, 0x1216 the 12160 image
  and every other device the 1080 image; the image pointer goes to +56, its length (a halfword read from the image's length variable) to +64 and its
  load address to +66.
  Locks: `init_mutex` on +208, +176 and +192 and `init_sema(record+224, 0, "ql_mboxsema", -1)`; `alenlist_create(0)` at +168 grown by the size the
  global `v` gives.
  Buses: for each of the bus-count buses it allocates a 680-byte bus record (`kmem_zalloc(680, 0x901)`; failure ends with message 4), stores the
  controller record at the bus record's +0, the bus record at controller +16+8*bus, the bus number as a halfword at +416, and calls
  `ql_ctlr_add(vertex, bus, count)`. Its result is the bus's controller number: stored at bus +12 and at controller +32+4*bus, printed into the name
  at bus +420, registered with `ql_ha_info_put`, and `ql_set_defaults(bus, number)` runs. The 16 words at bus +112 to +172 are set to 1.
  Registration and start: the controller is pushed on the global list `ql_chip_list` (the old head to +520) under the driver lock;
  `pciio_intr_alloc(vertex, device_desc_default_get(vertex), 1, vertex)` is stored at +160 and connected with
  `pciio_intr_connect(intr, qlintr, record, 0)`; then `ql_init_isp(record, 1)`.
  Result: when `ql_init_isp` fails it clears flag bit 0x8 (waking `vsema(record+224)` when it was set), sets the flags word at +40 to 0x20 (board
  dead), prints `ql_init_board failed (2)` through `ql_cpmsg` and returns -1. When it succeeds, each bus's byte at +304 must have bit 0x10 set,
  otherwise `ql_init_board failed (3)` is printed and -1 returned; with all buses accepted it returns the word at +32. The two clock figures and the
  byte at +304 are named by use only (inferred); the register block itself is not touched here.

- `ql_reset_interface` (`6.5.30/unix.IP27`, address 0xc0000000000cd340, 1792 bytes, 160-byte frame): **established.**
  The full restart of the chip's host interface: stop it, reset it, wait for its boot firmware, start the downloaded firmware, set up both queues and
  push every setting. `a0` (saved at sp+80) is the controller record; `a1` is saved at sp+64 and not used again. It returns 0 or a step number, and
  every nonzero step prints `ql_reset_interface failed (%d)` with it through `ql_ppmsg`.
  Software state first: when flag bit 0x8 is set it clears it and calls `vsema(record+224)`; it clears flag bit 0x1 of the word at +40. For each bus
  it sets bit 0x10 in the bus word at +8 and, for each of the 16 targets that `scsi_targ_vertex_exist` reports and each of the 8 logical units that
  `scsi_lun_vhdl_get` finds, takes the unit's lock at +84 and clears bits 0x1 and 0x8 in the unit's word at +16.
  Ring state: it zeroes the halfwords at +74, +72 and +70 and the word at +80, sets the request queue pointer +88 to the base at +96 and the response
  pointer +104 to the base at +112, and stores 0xdead in the word at +4 of each of the 256 response entries (64 bytes apart, a loop unrolled three
  times).
  Stop and reset (register offsets as used by the code): it writes 0 to +10, then 0x2000 to +194 with `flushbus`, polls +194 up to 10000 times with
  `us_delay(100)` for bit 0x20 (failure: step 1); writes 1 to +10, `flushbus`, waits 30 microseconds; writes 0x1000 to +194, then 0x3000 to +194 with
  `flushbus`; waits 30 microseconds and polls +194 up to 10000 times with `us_delay(100)` for bit 0x40 to clear (failure: step 2).
  Chip-specific set-up: for the 0x1020 chip it stores 14 in the halfwords at +32 and +64; for every other chip it reads the halfword at +4, writes it
  back with bits 0x300 set (`flushbus` after), stores 14 at +128, +160 and +192 (`flushbus` after the third), sets bit 0x2 of the halfword at +130
  (`flushbus` after) and of the halfword at +194 (`flushbus` after) and restores the original value of +4 (`flushbus` after).
  Boot wait and start: it polls the halfword at +114 up to 1000 times with `us_delay(1000)` while it equals 4, calls
  `ql_enable_sram_parity(record, 0)`, writes 0x44 to +4, 0 to +14 and 0x3000 to +194, waits 30 microseconds, calls `ql_enable_intrs`, and, when the
  process-page byte at absolute address 0xffffffffffffc523 is nonzero, writes 1 to +180 and 0 to +182 and waits 100 milliseconds. It then issues
  mailbox command 2 through `ql_mbox_cmd(record, &sp+40, 2, 1, 2, the halfword at +66, 0, 0, ...)` (execute firmware at the load address; failure:
  step 3).
  Settings: `ql_set_pci_control_parameters` (step 4) and `ql_set_clock_rate` (step 5). The response queue: when the doubleword at +144 is empty it
  builds the chip's bus address of the ring at +104 with `kvtophys` and `pciio_dmatrans_addr(device at +8, 0, address, 16384, 0x2001050)` and stores
  it at +144; it clears the 16384-byte ring at +112 with `bzero` and sends command 83 with all eight mailbox values: 256 (queue length), the address
  bits 16 to 31, bits 0 to 15, 0, the halfword at +74, address bits 48 to 63 and 32 to 47 (step 6). The request queue is the same with command 82, the
  address stored at +136, the base at +96 and the halfword at +70, and the fifth value 0 is also loaded (step 7). `ql_mbox_cmd` skips the register at
  +122 for command 83 but not for 82, so only the request queue command loads its fifth value. Then for each bus
  `ql_set_bus_reset(record, &sp+40, the halfword at bus +416)` (step 8), and in this order, with the step number in parentheses: `ql_set_scsi_id` (9),
  `ql_set_selection_timeout` (10), `ql_set_retry_count` (11), `ql_set_tag_age_limit` (12), `ql_set_negation_state` (13),
  `ql_set_asynchronous_data_setup_time` (14), `ql_set_data_over_recovery_mode` (15), `ql_set_reset_delay_parameters` (16), `ql_set_system_parameters`
  (17) and `ql_set_firmware_features` (18). On success it sets flag bit 0x1 again and returns 0. The meaning of the halfword-register names (+194 as
  the host command register, +10 as the interrupt control register, +114 as mailbox 0) and of the values 0x2000, 0x1000 and 0x3000 as pause, reset and
  release is inferred from the published register map.

- `ql_download_fw` (`6.5.30/unix.IP27`, address 0xc0000000000ccf8c, 948 bytes, 96-byte frame): **established.**
  Loads the chip's firmware and starts it. `a0` is the controller record (`s2`); `s1` is its register block. It returns 0 or a step number and prints
  `ql_download_fw failed (%d)` for a nonzero one. Order of events:
  It writes 5 to the register at +10, waits 30 microseconds, writes 0x1000 and then 0x3000 to +194 (`flushbus` after the second), waits 30
  microseconds and polls +194 up to 10000 times with `us_delay(100)` for bit 0x40 to clear; a timeout is step 11.
  It sets bits 0x200 and 0x800 in the flags word at +40 (the store is in the delay slot of the call) and calls `ql_enable_sram_parity(record, 1)`; a
  nonzero result is step 7. Then `ql_enable_intrs`.
  A mailbox register test: command 6 through `ql_mbox_cmd(record, &sp+32, 8, 8, 6, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777)`; a failure
  is step 1. It writes 0x2000 to +194, 0x44 to +4 and, after a 100-microsecond wait whose delay slot writes 0 to +14, 0x3000 to +194 with `flushbus`,
  then waits 30 microseconds.
  Loading: for the 0x1020 chip (halfword at record +44) it writes the image one 16-bit word at a time: mailbox command 4 with the load address (the
  halfword at +66) plus the word index and the word, for the number of words in the halfword at +64 (failure: step 4). For every other chip it
  allocates a buffer of twice that many bytes (`kmem_zalloc(2 * count, 0x8901)`; failure: step 7), copies the image from the pointer at +56, swaps
  each 16-bit word (`munge16`), converts the buffer with `kvtophys` and `pciio_dmatrans_addr(device at +8, 0, address, 2 * count, 0x2001060)` and
  sends command 9 with the load address, the count and the 64-bit bus address split over the mailbox values (failure: step 2), then frees the buffer.
  Start: command 2 (execute firmware at the load address); failure is step 5. Then command 8 through `ql_mbox_cmd(record, &sp+32, 1, 3, 8, ...)`;
  failure is step 6. When the global `showconfig` is set it prints `Firmware version: %d.%d.%d` with the reply halfwords 1, 2 and 3 of the array at
  sp+32; the command asks for only three replies, so halfword 3 is not set by it and holds what an earlier command left there. It clears flag bit
  0x200 (an AND with -513) and returns 0. The reading of the commands as register test, write word, load, execute and about-firmware, and of 0x200 as
  "firmware loading", is inferred; the numbers are those passed to `ql_mbox_cmd`.

- `qlattach` (`6.5.30/unix.IP27`, address 0xc0000000000cb36c, 2060 bytes, 192-byte frame): **inferred.**
  The PCI attach routine of the QLogic driver, registered for the five device numbers by `qlinit`. Every device access and every failure message was
  read; the part that probes each bus for disks (a series of hardware-graph lookups and `ql_probe_bus` calls) is summarised, which is why the grade is
  inferred. `a0` is the PCI vertex (`s3`). It returns 0 on success and otherwise a step number, printing `qlattach() failed (%d)` through `ql_ppmsg`
  for the failures of steps 1 to 4 below.
  Already attached: under the driver lock it walks `ql_chip_list` for a record whose vertex (+8) matches; when found it prints
  `%s is already attached` with the name at +264 (`cmn_err` level 6) and returns 0.
  Mapping and enabling: it maps the register block with `pciio_pio_addr(vertex, 0, 9, 0, 256, &sp+0, 0)`, reads config offset 4 (2 bytes) and writes
  it back with bit 0x2 set (memory space enable), duplicates the interrupt descriptor (`device_desc_dup`), names its interrupt `Qlogic SCSI`
  (`device_desc_intr_name_set`) and installs it as the vertex's default (`device_desc_default_set`).
  Settings by device: the device id (config offset 2) and revision (config offset 8) are read. It writes config offset 13 (1 byte) the value 64 and
  config offset 12 (1 byte) the value 128, then asks the bridge for read response buffers with `pcibr_rrb_alloc(vertex, &sp+8, &sp+12)`: the two
  counts are 0 and 2 for devices 0x1020 and 0x1080, 3 and 3 for 0x1216 and 2 and 2 otherwise. A negative result prints
  `ql: Unable to get Bridge RRB's` at level 0 and goes on.
  Controller: `ql_init(vertex, register block)` (which starts the watchdog timer on first use and calls `ql_init_board`); -1 counts a failure in the
  `.sbss` counter and returns 1. Otherwise `ql_ha_info_from_ctlr_get` gives the record, which is locked (+208); the map handle from `pciio_pio_addr`
  is stored at +152; for each bus (`device_inventory_add(controller number, 2, 1, -1, 0, type)`) an inventory entry is added whose type comes from the
  device and revision: for device 0x1020 revision 4 is 9, 2 is 7, 1 is 6, 3 is 8, 5 is 11 and anything else 12; device 0x1240 is 13, 0x1080 is 14,
  0x1280 is 15, 0x1216 is 17 and any other is 12. Type 7 (a 1020 revision 2) prints `INVALID CUSTOMER CONFIGURATION: Downrev SCSI QL1040A`.
  Settings: `ql_set_selection_timeout` (failure: step 1), `ql_set_retry_count` (step 2), flag bit 0x1 is set in the word at +40, each bus is probed
  (`Probing SCSI bus`, `Probing MSCSI/A [channel 0] internal SE bus.` and similar lines when `showconfig` is set), then `ql_set_selection_timeout`
  again (step 3) and, after storing two bytes at +260 and +261, `ql_set_retry_count` again (step 4). The lock is released and 0 returned. The bus
  probing selects a probe mode from the board's part-number text (`030-0872-`, `030-1243-`, `030-1281-`, found through the `_nic` hardware-graph label
  with `strstr`), the bus's own slot number (`pciio_info_slot_get`, with slot 1 setting bit 0x1 of the bus word at +8 and calling `ql_set_defaults`)
  and the differential-sense result of `ql_diffsense_sp`; those decisions are not traced to the end.

## IOC3 interrupt dispatch

The IOC3 base driver (`ioc3.o`) and its external-interrupt helper (`ioc3ei.o`). The chip's interrupt status register is the word at offset 0x1c of its
memory-space window (the doubleword at the soft record's +24 is that window's address); the enable bits are shadowed in the record's word at +2880
under the lock at +2876. Its handler slots are 0x58 bytes each starting at record +0, described with `ioc3_intr_connect` in
[ip27-kernel-io.md](ip27-kernel-io.md).

- `ioc3_pending_intrs` (`6.5.30/unix.IP27`, address 0xc00000000009513c, 92 bytes, 48-byte frame): **established.**
  Returns the interrupts that are both asserted and enabled. `a0` is the chip's soft record (`s8`). It takes the record's lock
  (`mutex_spinlock_spl(record+2876)`, 0xc00000000009515c), reads the word at register offset 0x1c (the interrupt status; nothing is written back, so
  this does not acknowledge anything), ANDs it with the software enable shadow at record +2880, releases the lock with `mutex_spinunlock` and returns
  the AND. The first instruction at 0xc00000000009513c, a load of a global pointer into `a1`, has no later use in the routine.

- `ioc3_intrd` (`6.5.30/unix.IP27`, address 0xc000000000096038, 72 bytes, 16-byte frame): **established.**
  Body of the per-handler interrupt thread that `ioc3_handle_intr` wakes for a threaded handler. `a0` (`s8`) points at the connection record's mask
  word, which is slot +48 of the soft record (the thread is started with that address); so the mask is the word at `a0`+0, the handler the doubleword
  at +8, its argument at +16, the soft record at +32 and a semaphore at +48. It reads the word at +0, loads the soft record from +32 and the register
  window from the soft record's +24, reads the interrupt status word at window +28, ANDs it with the slot's mask and calls the handler with the
  argument and that AND as its two arguments (the AND is formed in the delay slot of the call at 0xc000000000096060). It writes no register and does
  not acknowledge. When the handler returns it blocks in `ipsema(a0+48)` until the next wake-up; the return value is that of `ipsema`.

- `ioc3_intrd_setup` (`6.5.30/unix.IP27`, address 0xc000000000096080, 128 bytes, 32-byte frame): **established.**
  First code run by a new IOC3 interrupt thread. `a0` (`s0`) is the same slot address as in `ioc3_intrd` (slot +48). It pins the thread to a CPU with
  `setmustrun(halfword at soft record +42)`, where the soft record is the doubleword at `a0`+32; it installs `ioc3_intrd` as the thread's function
  with `a0` as its argument (`xthread_set_func(current thread, ioc3_intrd, a0)`); names the thread after the handler by copying at most 16 bytes of
  `fetch_kname(handler at a0+8)` into the thread record at +832 (`strncpy`); atomically sets bit 0x400 in the word at `a0`+40 with an `ll`/`sc` loop
  (branch-likely at 0xc0000000000960e0), which is the slot's flag word at +88; and ends in `ipsema(a0+48)`, the same wait `ioc3_intrd` ends in. It
  returns what `ipsema` returns. It reads and writes no chip register.

- `ioc3_intr` (`6.5.30/unix.IP27`, address 0xc000000000095abc, 28 bytes, 16-byte frame): **established.**
  The interrupt entry that `ioc3_attach` connects with `pciio_intr_connect`. It calls `ioc3_handle_intr(a0, 1)` with its own first argument unchanged
  and the second argument set to 1 (the delay slot at 0xc000000000095ac8), and returns the callee's result. The second argument means the call comes
  from the real interrupt, not from the poll timer; `ioc3_handle_intr` does the acknowledging.

- `ioc3_init` (`6.5.30/unix.IP27`, address 0xc000000000095198, 88 bytes, 16-byte frame): **established.**
  The IOC3 driver's boot-time registration. It calls `pciio_driver_register(0x10a9, 3, "ioc3_", 1)` for PCI vendor 0x10a9 and device 3 (the `ioc3_`
  prefix is the string at `.rodata` +48, passed after being added in the delay slot of the call at 0xc0000000000951b0), then three `nic_vmc_add` calls
  that bind a board part-number prefix to the table of what the board carries: `030-1155-` to the Ethernet table `pci_ioc3_enet_vmc`, and `030-1657-`
  and `030-1664-` to the serial table `pci_ioc3_sio_vmc` (the last two use the same table). It returns the result of the last `nic_vmc_add`. It
  touches no device register.

- `ioc3_ei_intr` (`6.5.30/unix.IP27`, address 0xc0000000000942a4, 308 bytes, 48-byte frame): **established.**
  The interrupt handler for the IOC3's two external-interrupt inputs. `a0` is the handler's own record (`s1`): the doubleword at +8 is the chip's
  register window, +16 the interrupt bit mask the driver registered, +20 a lock, +24 a list of waiters, +32 a record whose first word counts
  interrupts, +44 a user-level interrupt index, and +48 the chip's soft record. It first acknowledges by storing 0x18000000 (bits 27 and 28) to the
  word at window +28, before anything else (0xc0000000000942a8 to 0xc0000000000942ac). It then takes one of three paths, selected by the word at
  absolute address 0xffffffffffffc6a0 (a process-page word; the call it selects suggests it means the frame scheduler is in use, which is inferred)
  and the sign of the word at +44: frame scheduler on: calls `frs_handle_eintr` (no arguments); off, index non-negative: calls `uli_callup(index)`;
  off, index negative: takes `mutex_spinlock` on the lock at +20, adds 1 to the first word of the record at +32, and walks the list at +24 (link at
  node +8). For each node it adds 1 atomically to the word at +16 (`ll`/`sc`, branch-likely at 0xc000000000094360), calls
  `sigtopid(pid at +0, signal at +20, 18, 0, 0, 0)` when the signal word is nonzero, then `nested_spinlock` on node +24, `sv_signal(node+32)` and
  `nested_spinunlock`; finally it releases the lock with `mutex_spinunlock`. All three paths end by re-enabling the interrupt bits with
  `ioc3_write_ireg(soft record at +48, mask at +16, 0)` (operation 0 sets the enable bits), and the return value is that call's result. That the two
  bits are the external-interrupt inputs is inferred from the routine's name and from the path through the frame scheduler and user-level interrupts.

## IOC3 Ethernet (IP27 `ef` driver)

The Ethernet driver (`if_ef.o`) works on the IOC3's Ethernet register block, a window of 100 bytes at offset 0xf0 of the chip's memory space, held in
the doubleword at record +816; the registers are 32-bit words. Register names below are by offset. The meanings are inferred from the public Linux
IOC3 Ethernet driver's register map where it agrees with how the code uses them.

- `if_efattach` (`6.5.30/unix.IP27`, address 0xc0000000000e5bec, 792 bytes, 64-byte frame): **established.**
  The PCI attach of the IOC3 Ethernet function; `a0` is the PCI vertex (saved at sp+52). It returns -1 when the Ethernet sub-device is disabled, 12 on
  any later failure after calling `ef_detach(device vertex)`, and 0 on success. `ioc3_subdev_enabled(vertex, 0)` zero returns -1.
  `hwgraph_traverse(vertex, ".host", &sp+52)` replaces the vertex by its host (a failed lookup leaves it).
  `hwgraph_char_device_add(vertex, "ef", "if_ef", &sp+0)` makes the device vertex; a nonzero result prints
  `if_efattach: hwgraph_char_device_add error %d` and goes on. `kmem_zalloc(1072, 0)` gives the record (failure: 12); the vertex is stored at +908,
  the device vertex at +916 and `hwgraph_fastinfo_set` attaches the record. Four register windows are mapped with
  `pciio_piotrans_addr(vertex, 0, 8, offset, size, 0)`, any failure returning 12: offset 0xf0 and size 100 (Ethernet registers, stored at +816);
  offset 0 and size 0x20180 (the whole chip window, +920); offset 0x40000 and size 0x20000 (the external SSRAM window, passed to
  `ssram_discovery(record, window)`), and `pciio_piotrans_addr(vertex, 0, 7, 0, 72, 0)` for configuration space, from which the low byte of the word
  at offset 8 (the revision) is stored in the word at +928; the halfword at +26 is set to -1 (unit not yet assigned).
  `hwgraph_traverse(vertex, "ioc3", &sp+4)` finds the chip's vertex (stored at +912); a failure prints `if_efattach(%v): unable to find ioc3 vertex`
  and returns 12. A descriptor is duplicated, named `IOC3 Ethernet` and made the default (`device_desc_dup`, `device_desc_intr_name_set`,
  `device_desc_default_set`).
  Hardware: `ef_phyunreset(record)`, `ef_ioc3reset(record)`, then `ef_ioc3eaddr(record, &sp+8)` reads the board's Ethernet address from its NIC (a
  failure returns 12). The six bytes are copied to +288 and +240 (`bcopy`); with `showconfig` set it prints `%v: hardware ethernet address %s`.
  `ef_allocef(record)` nonzero returns 12.
  Interrupt: the words at +948, +972 and +968 are set to -1; `pciio_intr_alloc(vertex, descriptor, 1, device vertex)` is stored at +936 and connected
  with `pciio_intr_connect(intr, ef_intr, record, 0)`; `pciio_dmamap_alloc(vertex, descriptor, 4, 0x200061)` is stored at +832, and a zero result
  prints `%v: pciio_dmamap_alloc() failed!` and returns 12.
  Last: `device_inventory_add(device vertex, 8, 0, 19, -1, the revision at +928)`, `idbg_addfunc("ef_dump", ef_dump)` and
  `init_bitlock(record+856, 4096, "ef drv lock", 0)`.

- `ef_ifattach` (`6.5.30/unix.IP27`, address 0xc0000000000e5ad4, 236 bytes, 32-byte frame): **established.**
  Registers the interface with the network stack, called once the unit number is known. `a0` is the record and `a1` the unit number (stored as the
  halfword at +26, and saved at sp+8). It finds the processor the interrupt landed on (`pciio_intr_cpu_get(the interrupt object at +936)`, then
  `cpuvertex_to_cpuid`) and reads the word at +1856 of that processor's private record through the table `pdaindr` (16 bytes a processor, the pointer
  in the second doubleword). When that word is 126 or more and the global `ef_delaydiv` is nonzero it stores 1 in a `.sdata` word. If the device
  vertex at +916 is not -1 it builds `ef<unit>` with `sprintf` and adds it as an alias (`if_hwgraph_alias_add`). When the global `if_efdevflag` has
  bit 0x1000 set it stores 0x8000000 in the flags word at +32. It then calls
  `ether_attach(record, "ef", unit, record, the interface table, record+288, 19, the word at +928)`, sets bit 0x4000 of the word at +32, and returns
  the result of `ef_ps_register(record, 0)`. No device register is touched.

- `ef_ioc3reset` (`6.5.30/unix.IP27`, address 0xc0000000000e74dc, 208 bytes, 32-byte frame): **established.**
  Resets the Ethernet part of the chip and leaves its registers cleared. `a0` is the record; the register block is the doubleword at +816. It writes
  0x80000000 to the word at +0 and reads it back twice. When bit 0x200000 of the second read is clear it polls the register up to 1000001 times with
  `us_delay(1)` for that bit to become set, and on running out prints `ef%d: IOC3 reset timed out` (`cmn_err` level 1, unit from the halfword at +26).
  It then stores zero in the registers at +0, +64, +28, +68 and +32, in that order, the last store in the delay slot of a `flushbus`. It returns
  nothing a caller uses. Taking bit 31 of +0 as a reset request and bit 21 as "reset done" is inferred; the code names neither.

- `ef_ioc3init` (`6.5.30/unix.IP27`, address 0xc0000000000e6498, 776 bytes, 80-byte frame): **established.**
  Programs the Ethernet registers from the interface's settings and enables its interrupts. `a0` is the record (`s0`), `a1` (saved at sp+40) interface
  flags, `a2` (saved at sp+32) the argument for `ef_phyerr`; `s1` is the register block at +816. It returns 0, or what `ef_phyinit` returned when that
  is nonzero. It calls `ef_phyinit(record)` (a nonzero result is returned at once), then `ef_phyerr(record, a2)`.
  Buffer count and speed: the word at +968 (nonzero: 100 Mbit/s) picks the count `ef_nrbuf100` or `ef_nrbuf10`, or the `_promisc` variants when bit
  0x100 of `a1` is set; it stores the count and then the count plus 15 in the word at +864. It sets the low 28 bits of the word at +48 apart from its
  top four bits to 100000000 or 10000000 (the bit rate).
  Duplex: the word at +972 (nonzero: full) sets bit 0x10000 of the flags word at +32 (clears it when zero) and selects the gap table
  `ef_fullduplex_ipg` or `ef_halfduplex_ipg`.
  Register writes, in order: the register at +40 gets the three words of the gap table packed as word0 | word1 << 8 | word2 << 16; `microtime(&sp+0)`
  and its microsecond field goes to +44; 0x1000000 goes to +24; +12 gets the buffer count divided by `ef_nrbufthreshdiv` and masked to 9 bits (an
  unsigned divide with a trap on a zero divisor); +36 gets 0; +76 gets the first four bytes of the hardware address at +240 (byte 0 lowest) and +72
  the next two; +84 and +80 get the multicast filter, either -1 and -1 when bit 0x200 of `a1` is set or the bytes at +956 to +959 and +960 to +963
  packed the same way. `pcibr_rrb_alloc(the Bridge at +908, &sp+16 (0), &sp+20 (2))`; a negative result prints `ef%d: Unable to get bridge RRB's`
  (level 0). The bus address of the receive ring (`kvtophys` of the doubleword at +872, then `pciio_dmatrans_addr(+908, 0, address, 4, 0x200050)`)
  goes to +20 (low word) and +16 (high word); the transmit ring at +824 goes to +60 and +56, and bit 0x1 is set in +60.
  Last: +0 gets the word at +976 with bit 0x1 set when the word at +972 is nonzero, bit 0x2 set when `a1` has bit 0x100, and 0xac, then +8 gets
  0x7fe003f (the interrupt enable mask) in the delay slot of a final `flushbus` call. The reading of +40 as the transmit control and gap register, +44
  as the random seed, +12 as the receive threshold, +72 and +76 as the station address, +80 and +84 as the multicast hash, +16 to +20 and +56 to +60
  as ring base addresses, +0 as the MAC control register and +8 as the interrupt enable register is inferred from the Linux register map.

- `ef_intr` (`6.5.30/unix.IP27`, address 0xc0000000000e7704, 1360 bytes, 96-byte frame): **established.**
  The Ethernet interrupt handler. `a0` is the record (`s0`), `s6` the lock word at +856 and `s3` the unit halfword at +26. Every register access: the
  status word at +4 is read once and written back unchanged, which acknowledges exactly the bits that were read; the word at +32 is written once with
  the receive producer position; nothing else is touched on the chip directly (`ef_recv`, `ef_reclaim`, `ef_getcdc` and `ef_init` do their own).
  Entry: `mutex_bitlock(record+856, 4096)`. It adds 1 to the word at +904; when the result reaches 99 it clears the word and calls `randproc_input`
  with a 32-bit value read through a literal-pool pointer and 5 in the upper word (entropy collection; the purpose is inferred).
  Ack: it reads the word at +4 into `s2` and stores it back.
  Running check: when the transmit ring pointer at +824 is nonzero and `(word at +32) & 0x41` is 0x41 it continues; otherwise it releases the lock and
  returns. It writes `(word at +884) * 8 | 0x80000000` to the register at +32 (with `flushbus`); when the word at the address in +888 has bit 31 set
  it calls `ef_recv(record)` and keeps its result, a chain of received packets, in `s1`.
  Errors: when `s2 & 0x07fe003c` is zero it unlocks and delivers the packets. Otherwise, per status bit: 0x4 (print `dropped packets`), 0x8 (`receive
  overflow`), 0x10 (`receive PCI error`, `cmn_err` level 1) and 0x20 (`rx SSRAM hardware parity error`, level 1) add 1 to the word at +56 and to the
  counters at +1016, +1020, +1028 and +1032; bits 0x20000 (`excessive collisions`), 0x40000 (`excessive defers`), 0x80000 (`late collision`), 0x100000
  (`transmit giant packet error`) and 0x200000 (`transmit underflow`) add 1 to the word at +64 and to the counters at +1036, +1040, +1044, +1048 and
  +1024; bit 0x2000000 (`transmit PCI error`, level 4) and 0x4000000 (`tx SSRAM hardware parity error`, level 1) add 1 to +64 and to +1028 and +1032.
  The printf lines appear only when bit 0x4 of the flags word at +32 is set. Bit 0x400000 with bit 0x10 of the word at +856 set unlocks, runs
  `ef_reclaim(record)`, reports `ps_txq_stat(record, 512 minus the ring occupancy)`, frees the returned buffers with `m_freem` and takes the lock
  again; with that bit clear it only prints `stray TXEXPLICIT interrupt` (when the debug flag allows); bits 0x1800000 call `ef_getcdc(record)` and
  print `collision or defer count wrapped`. The bits 0x4, 0x8, 0x10, 0x20, 0x200000, 0x2000000 and 0x4000000 also mark the interface for restart: at
  the end `ef_init(record, the flags word at +32, saved lock state)` runs.
  Delivery: it releases the lock and walks the packet chain (link at +16). Each packet goes to
  `ether_input(record, 4096 while another packet follows else 0, packet)`; when that returns 132 the rest are passed to `network_drop` instead. The
  returned value carries no meaning. The mapping of the status bits to the names comes only from the strings printed; the register's real bit
  assignment is inferred.

## Bridge interrupt wiring and threads

The PCI Bridge interrupt code (`pcibr.o`) is described in [ip27-kernel-io.md](ip27-kernel-io.md) for the connect and the handler; these are the
helpers they use.

- `pcibr_setpciint` (`6.5.30/unix.IP27`, address 0xc000000000078ccc, 168 bytes, 64-byte frame): **established.**
  Programs one of the Bridge's PCI interrupt bits with where to send the interrupt, called back by the crosstalk layer when `pcibr_intr_connect`
  connects the bit. `a0` (`s0`) is the crosstalk interrupt object. It gets the target address (`xtalk_intr_addr_get`, kept at sp+24), the vector
  (`xtalk_intr_vector_get`, `s8`), the Bridge widget vertex (`xtalk_intr_dev_get`, `s7`) and the set-function argument (`xtalk_intr_sfarg_get`), whose
  first word holds the bit number in its low 3 bits (`s1`) and the bus number in bit 3. Then `pcibr_bridge_ptr_get(vertex, bus)` gives the Bridge's
  register accessor and the routine calls, in this order, `pcireg_intr_addr_vect_set(bridge, bit, vector & 0xff)` and
  `pcireg_intr_addr_addr_set(bridge, bit, address)`. It returns the second call's result. The registers those two accessors write are in the Bridge's
  per-bit interrupt address registers (not read here).

- `pcibr_setwidint` (`6.5.30/unix.IP27`, address 0xc00000000007916c, 132 bytes, 48-byte frame): **established.**
  The same for the Bridge's own error interrupt: sets its destination. `a0` (`s0`) is the crosstalk interrupt object. It reads the target
  (`xtalk_intr_target_get`, `s7`), the address (`xtalk_intr_addr_get`, `s8`), the vector (`xtalk_intr_vector_get`, saved at sp+16) and the
  set-function argument (`xtalk_intr_sfarg_get`, which replaces `s0`: here it is the Bridge's soft record). It then calls
  `pcireg_intr_dst_target_id_set(record, target & 0xff)`, `pcireg_intr_dst_addr_set(record, address)` and
  `pcireg_intr_host_err_set(record, vector & 0xff)`, in that order, and returns the last result. That it is installed as the hook of the Bridge's
  error interrupt is inferred from the accessor names.

- `pcibr_wrap_get` (`6.5.30/unix.IP27`, address 0xc000000000078154, 100 bytes, 16-byte frame): **established.**
  Takes the oldest pending handler request from a five-slot queue. `a0` is the queue: the word at +4 is the put index, the word at +8 the get index
  and the doublewords from +16 are the slots (the queue is embedded in the interrupt handle at +104). When the two indexes are equal it calls `panic`
  (the string pointer comes from the global slot, the argument is the queue) and then continues as if it had a request. It stores get + 1 in the word
  at +8, then replaces it with (get + 1) modulo 5 (two stores, the second after a signed divide) and returns the doubleword that was in slot `get`.

- `pcibr_wrap_put` (`6.5.30/unix.IP27`, address 0xc0000000000781b8, 148 bytes, 48-byte frame): **established.**
  Adds a request to that queue. `a0` is the wrapper to queue (`s8`) and `a1` the queue (`s0`). It takes the lock at the queue's address with
  `mutex_spinlock`, computes (put + 1) modulo 5, panics when that equals the get index (queue full; the message argument is the queue), stores `a0` in
  the slot at +16 + 8 * put, stores the new put index at +4, and releases the lock with `mutex_spinunlock`. It returns nothing.

- `pcibr_intrd_start` (`6.5.30/unix.IP27`, address 0xc00000000007838c, 124 bytes, 32-byte frame): **established.**
  First code of a Bridge interrupt thread. `a0` (`s0`) is the handle: the handler function is at +24, a semaphore at +48, a flag word at +40 and the
  preferred processor in the halfword at +80. It pins the thread (`setmustrun(halfword at +80)`), installs `pcibr_intrd` as the thread function with
  the handle as argument (`xthread_set_func`), names the thread after the handler (`fetch_kname(+24)` copied with at most 16 bytes by `strncpy` to the
  thread record at +832), sets bit 0x400 in the word at +40 atomically (`ll`/`sc`, branch-likely at 0xc0000000000783e8), and blocks in
  `ipsema(handle+48)`. No device register is touched.

- `pcibr_intrd` (`6.5.30/unix.IP27`, address 0xc000000000078314, 120 bytes, 16-byte frame): **established.**
  The loop body of a Bridge interrupt thread, run each time the thread is woken. `a0` (`s0`) is the handle. It calls the handler, the function at +24,
  with the argument at +32. It then takes the oldest wrapper from the queue embedded at +104 (`pcibr_wrap_get(handle+104)`), subtracts 1 atomically
  from the wrapper's request count at +24 (`ll`/`sc`, branch-likely at 0xc000000000078350), and when the count is then zero calls
  `pcibr_force_interrupt(wrapper)`. It ends in `ipsema(handle+48)` and returns that call's value. It touches no register itself.

## Graphics board interrupt (InfiniteReality, `kona`)

The graphics board driver's main interrupt handler is a global symbol of the linked kernel (object `kona_intr_hip2.o` of the `kona.a` archive). The
board record's doubleword at +2376 is the mapped register window and its word at +2368 is the interrupt enable mask; the interrupt status register is
the 32-bit word at window +0x800238 and is read and written with `gfx_pio_read_32` and `gfx_pio_write_32`. The printed prefix is the string at the
base held in the global slot at `gp`+1528 (the board name `IR`, unit number from the word at +2184).

- `kona_intr_handler` (`6.5.30/unix.IP27`, address 0xc0000000003a6fe4, 588 bytes, 48-byte frame): **established.**
  The main interrupt handler of the graphics board. `a0` is the board record (`s0`). It adds 1 to the word at +16 of the CPU's 32-byte record in the
  table behind the global slot at `gp`+3368 (the CPU number is the halfword at absolute address 0xffffffffffffc420) and 1 to the board's word at
  +5268, then returns at once when the word at +2368 equals 256 (0x100). Otherwise:
  It takes the lock at +2352 (`mutex_spinlock`, spl kept at sp+0) and reads the status register at window +0x800238. The pending set is that value
  ANDed with the word at +2368 and with ~0x20.
  The bits are handled in this order. 0x1: prints the "unexpected HIP2 graphics fifo hi-water interrupt. Ignored." line (`cmn_err` level 4). 0x2:
  `kona_lowater_intr(s0)`. 0x4: prints "graphics fifo overflow. Data lost. Resetting pipe." and calls `kona_die(s0, 10)`. 0x8: `kona_video_intr(s0)`.
  0x10: `kona_arm_intr(s0)`. 0x40: prints "odma overflow." through `cmn_err_tag` (tag 2120, level 4; no `kona_die`). 0x100: prints "parity error. Data
  lost. Resetting pipe." and calls `kona_die(s0, 22)`. 0x200: `kona_voc_intr(s0)`. 0x400: "PIO overflow. Data lost. Resetting pipe." and
  `kona_die(s0, 13)`. 0x800: "IDMA overflow. Data lost. Resetting pipe." and `kona_die(s0, 14)`.
  Acknowledge: when the pending set is nonzero it writes it back to window +0x800238; it then always writes 0xbadc0ffe to window +0x800228 (the value
  is formed by `lui` with 0xbadc and `ori` with 0xffe), releases the lock with the saved spl and returns. The status register is acknowledged only for
  the handled bits; bit 0x20 is left for the geometry handler. Reading the write-back as write-one-to-clear and the write to +0x800228 as the re-arm
  is inferred from the order. A return value carries no meaning. Bit names come from the strings and the callee names.

## IOC3 serial ports (IP27)

The two serial ports of an IOC3 chip are driven by the routines of the object `sio_ioc3.o` (in the archive `sduart.a` of release 6.5.30), whose text sits at
0xc000000000175dd4 plus the object-relative address. The attach routine `ioc3_serial_attach` is in [ip27-kernel-io.md](ip27-kernel-io.md); this section
covers what it hands to the interrupt connection and the routines the interrupt path and the open path use. Port records are 168 bytes
(0xa8) apart, port 0 first, in one block. The fields these routines use, read from the instructions (all meanings are inferred from use):

- +8: a pointer to a table of notification methods of the serial-line layer above (it is not stored by any routine in this section); the methods used
  here sit at +0, +8, +24 and +32 of that table.
- +48: the chip's PCI vertex. +56: the mapped register window of the chip. +64: the address of the port's control block, window +184 for port 0 and window
  +212 for port 1 (seven words: +0 control word, +4 and +16 two ring positions, +8 and +12 two more ring positions, +20 a timer, +24 a shadow status word).
  +72: the port's byte-wide UART registers, in a region 0x20000 above the window (+376 for port 0, +368 for port 1; 16550-style: +0 data or divisor low, +1
  interrupt enable or divisor high, +2 FIFO control, +3 line control, +4 modem control, +5 line status, +7 a prescaler).
- +88, +96, +104: the 16 KB ring buffer block (port 0 only) and the two 4 KB halves of it this port transmits and receives from (+96 and +104).
- +112: a pointer to a per-port table of nine words (the table for port 0 is at 0xc000000001463a48, the one for port 1 at 0xc000000001463a6c), read with
  `lw` or `lbu` from the `.data` of the image: word +0 (0x10 for port 0, 0x2000 for port 1), +4 (0x20, 0x4000), +8 (0x1, 0x200), +12 (0x8, 0x1000), +16
  (0x4, 0x800), +20 (0x80, 0x10000), +24 and +28 (0x1ff, 0x3fe00: every interrupt bit of the port), and the byte at +32 (7, 6). These are bits of the
  chip's interrupt status word at window +28; that the first is a modem-carrier change, the second a modem-clear-to-send change, the third a
  transmitter-empty, the fourth a receive timer, the fifth a receive-high-water, and the sixth an explicit transmit-done is **inferred** from the order
  the handler tests them in and the table of interrupt-bit positions a published Linux driver for the same chip uses.
- +120: a flag word (the handler sets and clears bits 1, 2, 4 and 8). +128: the baud rate. +132: set to 1 when the port is open. +136: the last receive
  timeout. +140: the notification bits the upper layer asked for (1 receive, 2 transmit, 0x40 carrier, 0x80 clear-to-send). +144: the driver's copy of
  this port's enabled interrupt bits. +148: the driver's copy of the control word. +152 and +156: saved ring positions. +160: the chip's soft record
  (the argument `ioc3_write_ireg` takes).

Several routines bracket one register access with a hook of the console firmware. The test, every time, is: the port record equals the doubleword at
`gp`+5096 (the console port), the doubleword at the address in the `gp`-23672 slot (0xa800000000001000) equals 0x53435241, the pointer at the address in
the `gp`-23664 slot (0xa800000000001020) is nonzero, the word it points at is 0xfeeddead and the doubleword at +80 in that table is nonzero. When it
holds, the routine raises the interrupt level (`spl7`, level kept in the word at `gp`+5092), calls that doubleword with argument 0, does the access,
and afterwards calls it with argument 1 and restores the level with `splx`. The bracket is only a firmware console lock; whatever the hook does is unknown.

- `ioc3_serial_intr` (`6.5.30/unix.IP27`, address 0xc000000000175f04, 1436 bytes, 112-byte frame): **established.**
  The interrupt handler of both serial ports, given to `ioc3_intr_connect` by `ioc3_serial_attach` with mask 0x1ff for port 0 and 0x3fe00 for port 1.
  `a0` is the port record, `a1` the interrupt bits that are both asserted and in the connection's mask. It calls `sio_lock_port(record, 0)`, works out
  the working set as `a1` with the port's word +8 removed (tab+8 is the transmitter-empty bit), and loops, each pass in six steps in this order. In
  every step the "ack" is a store of that bit to the interrupt status word at window +28, and the table words are those of the port record's +112 table.
  1. Word +0 set (carrier): ack; then reads the shadow word at port control block +24 (inside the console bracket). When record +140 has bit 0x40:
     if shadow bit 0x800000 is clear it sets bit 2 of record +120 and calls nothing; if set and record +8 is nonzero it calls the method at +24 of the
     table with the record and 1.
  2. Word +4 set (clear-to-send): ack; reads the shadow word the same way. When record +140 has bit 0x80 and record +8 is nonzero it calls the method at
     +32 with the record and 0 or 1, 1 when shadow bit 0x100000 is set.
  3. Word +12 set (receive timer): ack; when record +140 has bit 1 and record +8 is nonzero, calls the method at +0 with the record.
  4. Word +16 set (receive high water), only looked at when step 3's bit was clear: the method at +0 is called (same conditions) with the record and the bit value; it then re-reads window +28,
     ANDs it with record +144, and if the bit is still set and bit 8 of record +120 is clear it calls `mask_disable_intrs(record, bit)` and sets bit 1
     of record +120; if bit 8 is set it instead adds 1 to a pass counter (the doubleword at sp+48). The ack of this bit is not in this step.
  5. Word +20 set (explicit transmit done): clears bit 4 of record +120, acks, and when record +140 has bit 2 and record +8 is nonzero calls the method at
     +8 with the record.
  6. Word +8 set (transmitter empty), only looked at when step 5's bit was clear (the first pass never has it, the working set lacks it): when record +140 has bit 2 and record +8 is nonzero calls the method at +8 with the
     record and the bit; if the status word, re-read and ANDed with record +144, still has the bit, it calls `mask_disable_intrs(record, bit)`; if
     record +140 has neither bit 1 nor bit 0x40, it first clears bit 28 (0x10000000) of record +148 and writes record +148 to the control word.
  After each pass it re-reads window +28 (ANDed with record +144) and loops while any bit of table word +28 is set in it. When it ends it calls
  `sio_unlock_port(record)` and then `ioc3_write_ireg(record +160, record +144, 0)`, which sets the enable bits from the driver's copy, and returns the
  result of that call. It prints nothing and has no failure path. Which bit is acknowledged where, and the loop test, are read; the names of the
  bits are inferred as the section header says. There is no branch-likely instruction.

- `mask_disable_intrs` (`6.5.30/unix.IP27`, address 0xc000000000175e4c, 20 bytes, no frame): **established.**
  `a0` is the port record, `a1` a bit set. It clears those bits in the driver's copy of the enabled bits (the word at record +144) and stores it back
  in the delay slot of the return. No chip register is touched; the chip's own enable bits stay as they were. The return value is not set.

- `enable_intrs` (`6.5.30/unix.IP27`, address 0xc000000000175e60, 80 bytes, 32-byte frame): **established.**
  `a0` is the port record, `a1` a bit set. When all of the bits are already in the word at record +144 it returns at once. Otherwise it calls
  `ioc3_write_ireg(record +160, a1, 0)` (operation 0 sets the chip's enable bits) and then ORs `a1` into record +144. The return value is not set.

- `disable_intrs` (`6.5.30/unix.IP27`, address 0xc000000000175eb0, 84 bytes, 32-byte frame): **established.**
  `a0` is the port record, `a1` a bit set. When none of the bits is in the word at record +144 it returns at once. Otherwise it calls
  `ioc3_write_ireg(record +160, a1, 1)` (operation 1 clears the chip's enable bits) and then removes the bits from record +144. The return value is not set.

- `ring_dmatrans` (`6.5.30/unix.IP27`, address 0xc000000000175e00, 76 bytes, 16-byte frame): **established.**
  Turns the ring buffer's address into one the chip can use. `a0` is the chip's PCI vertex, `a1` the address. The address is ANDed with 0xffffffffff
  (the mask is formed by a shift of -1 right by 24). When `a0` is -1 that value is returned. Otherwise it returns the result of
  `pciio_dmatrans_addr(a0, 0, address, 0x4000, 0x40)`, the whole 16 KB ring with the flag value 0x40.

- `set_baud_ti` (`6.5.30/unix.IP27`, address 0xc0000000001764a0, 232 bytes, no frame): **established.**
  Programs the divisor and prescaler of a port's UART. `a0` is the port record, `a1` the wanted baud rate. It tries prescaler values `a4` from 6 up to
  63: with `q` = 44,000,000 / `a4` (the constant 0x29f6300), it takes the divisor (`q` + 8 * baud) / (16 * baud), skips this value when the divisor is
  zero, computes the baud rate that divisor gives as (`q` / 16, signed, rounded toward zero for a negative `q`) / divisor, and accepts the first value
  where that rate is not further from the wanted one than 1 percent (the test is: achieved >= 100 * |achieved - wanted|). When no prescaler up to 63
  is accepted it returns 1 and writes nothing. Otherwise it saves the line control byte (UART +3), sets bit 0x80 in it, writes the divisor low byte
  to UART +0, the divisor shifted right by 8 to +1 and the prescaler to +7, restores the line control byte and returns 0. The divide instructions are
  followed by `teq` traps on a zero divisor.

- `hardware_init` (`6.5.30/unix.IP27`, address 0xc000000000176588, 676 bytes, 48-byte frame): **established.**
  Resets one serial port and leaves its interrupts off. `a0` is the port record, `a1` nonzero for the console chip. It runs, in this order:
  1. With `a1` nonzero it polls the UART line status (+5) up to 4200 times for bit 0x20 and waits `us_delay(3)` between polls; it goes on whatever the result.
  2. Writes 0x80000000 to the port's control word (block +0). It then reads the word at window +40 until bit 22 (0x400000) is set or the field in bits 19 to 21
     holds 4 or more; there is no timeout. It then writes 0 to the control word.
  3. Reads block +8, keeps bits 3 to 11 (AND 0xff8), stores that in record +152 and in block +4; the same with block +12 into record +156 and block +16.
  4. UART +3 and +1 are zeroed; `set_baud_ti(record, the baud at +128)` is called (its result is not used); then UART +3 = 3, +2 = 1, +2 = 7 and +4 = 0.
  5. Block +24 is written 0.
  6. For port 0 only (the record's +112 table is the port 0 table): it calls `ring_dmatrans(record +48, ring base at +88)` and writes the high 32 bits of
     the result to window +176 and the low 32 bits with bit 0 set to window +180.
  7. Block +20 is written 0xa0, and record +148 is set to 0x100180.
  8. The control word at block +0 is written from record +148, inside the console bracket.
  9. `ioc3_write_ireg(record +160, table word +24, 1)` clears all of this port's enable bits in the chip, the same bits are cleared in record +144, and
     table word +24 is stored to window +28 (in the delay slot of the return) to acknowledge everything pending. It returns 0.
  It prints nothing and has no failure path.

- `ioc3_open` (`6.5.30/unix.IP27`, address 0xc000000000176dfc, 676 bytes, 48-byte frame): **established.**
  The open method, entry +0 of the port method table that `ioc3_serial_attach` stores at record +0 (the table is at 0xc000000001463a90). `a0` is the port
  record. It zeroes the byte at +124 and the word at +120. When bit 28 (0x10000000) of record +148 is set it writes record +148 with bit 29 (0x20000000)
  set to the control word (block +0, console bracket) and reads block +0 until bit 30 (0x40000000) is set, at most 1,000,000 times; when it never sets,
  `ioc3_open` returns -1 at once. In every other case it then calls `set_baud_ti(record, 9600)` (the rate is fixed here, not taken from +128), and the
  delay slot of that call writes UART +2 = 3; it writes UART +3 = 3, sets the low nine bits of record +148 to 1, writes record +148 to the control word
  (console bracket), writes block +20 = 0, stores 1 at record +132 and returns 0. It does not touch the chip's interrupt enables.

- `ioc3_enable_hfc` (`6.5.30/unix.IP27`, address 0xc000000000177434, 292 bytes, 32-byte frame): **established.**
  Method +16 of the port table. `a0` is the port record, `a1` nonzero to turn the bit on. It sets (`a1` nonzero) or clears bit 17 (0x20000) of record +148, writes
  record +148 to the control word (block +0) inside the console bracket, and returns 0. (Reading the bit as hardware flow control is inferred from the name.)

- `ioc3_notification` (`6.5.30/unix.IP27`, address 0xc000000000178500, 512 bytes, 48-byte frame): **established.**
  Method +80 of the port table. `a0` is the port record, `a1` a set of the notification bits (1, 2, 0x40, 0x80), `a2` nonzero to turn them on. It builds an
  interrupt mask from table words: bit 1 gives word +12 and +16 (receive timer and high water), bit 2 gives word +20, bit 0x40 gives word +0, bit 0x80 gives
  word +4. With `a2` nonzero it calls `enable_intrs(record, mask)`, sets bit 0x40000 in record +148 when `a1` has bit 0x40 and ORs `a1` into record
  +140. With `a2` zero it calls `disable_intrs(record, mask)`, clears that bit 0x40000 and clears `a1` in record +140. Then, when record +140 has bit 1 or bit 0x40,
  it sets bit 24 (0x1000000) of record +148; when it has neither, it clears that bit if record +144 has no bit of table word +8 and leaves it alone otherwise. In all
  cases it writes record +148 to the control word (block +0, console bracket) and returns 0.

- `ioc3_rx_timeout` (`6.5.30/unix.IP27`, address 0xc000000000178700, 440 bytes, 48-byte frame): **established.**
  Method +88 of the port table. `a0` is the port record, `a1` a timeout. It stores `a1` at record +136. The threshold is (baud at +128 * `a1` / 10 / 100),
  divided by 4 with signed rounding, or 1 when that is zero. When it is 512 or more the routine returns 1 and writes nothing else. Otherwise it puts it in
  the low nine bits of record +148, writes record +148 to the control word (console bracket), then writes the receive timer (block +20) with
  160 * `a1` limited to 4095 (the code computes `a1` * 16000 / 100), and returns 0.

- `ioc3_set_proto` (`6.5.30/unix.IP27`, address 0xc000000000178cd4, 84 bytes, no frame): **established.**
  Method +128 of the port table. `a0` is the port record, `a1` the mode, 0 or 1. It takes the byte at +32 of the record's +112 table (7 for port 0, 6 for
  port 1) and stores `a1` in the register word at window +64 + 4 * that byte, then returns 0; for any other `a1` it stores nothing and returns 1. That
  this word is a pin that selects the line standard for the port is inferred from the published Linux driver's use of the same pins.

## PCI configuration access and driver registration (Bridge)

A PCI driver's attach routine reaches its device's configuration registers, and registers itself to be called, through the routines below (objects
`pciio.o` and `pcibr.o` of release 6.5.30). The Bridge register base is the doubleword at +8 of the Bridge soft record. The address routines
`pcireg_type0_cfg_addr`, `pcireg_type1_cfg_addr` and `pcireg_type1_cntr_set` take either that soft record or a register base. A register base is told by
`(a0 & 0x9200000000000000) == 0x9200000000000000`; it is then identified by the widget identification word at base +4, whose bits 12 to 27 are the part
number and bits 1 to 11 the manufacturer: the accepted pairs are part 0xc002 with manufacturer 54 and parts 0xd002, 0xd102, 0xd112, 0xe000 and 0xe010 with
manufacturer 36. Which chip each part number is, beyond 0xc002 (Bridge) and 0xd002 (XBridge) from [ip27-kernel-io.md](ip27-kernel-io.md), is not
established. Otherwise the halfword at soft record +114 is a Bridge kind, 0 to 3 (-1 and zero `a0` are errors).

- `pciio_driver_register` (`6.5.30/unix.IP27`, address 0xc000000000093748, 116 bytes, 48-byte frame): **established.**
  Registers a PCI driver by vendor and device number. `a0` is the vendor, `a1` the device, `a2` the driver's name prefix string and `a3` its flags. When
  the global table handle at the `gp`-28664 slot is zero it first calls `pciio_init()` (no arguments). It then calls
  `cdl_add_driver(handle, a0, a1, a2, a3, 0xc0000000000937e0)`, the last argument being the address of a callback routine of this object, and returns that
  result. It touches no device register. The QLogic and IOC3 drivers call it from their start-up routines (`ioc3_init` above).

- `pciio_intr_alloc` (`6.5.30/unix.IP27`, address 0xc000000000092da8, 104 bytes, 48-byte frame): **established.**
  Allocates an interrupt for a PCI device. `a0` is the device vertex, `a1` a device descriptor, `a2` the interrupt line the driver asks for and `a3` the
  owner vertex. It calls `pciio_info_get(a0)`; when the words at +8 and +12 of the result differ (the device sits behind a PCI bridge chip) it replaces
  `a2` with `pciio_ppb_intr_map(info, a2)`. It returns `pcibr_intr_alloc(a0, a1, line, a3)`. No device register is touched.

- `pciio_config_get` (`6.5.30/unix.IP27`, address 0xc0000000000931ac, 184 bytes, 64-byte frame): **established.**
  Reads 1 to 8 bytes of configuration space. `a0` is the device vertex, `a1` the register (the low byte is the offset; see `pcibr_config_addr` for the upper
  bits), `a2` the byte count. It splits the read into pieces that never cross a 4-byte boundary (each is the smaller of the bytes left and 4 minus the
  offset's low two bits), calls `pcibr_config_get(a0, offset, piece)` for each, ORs each result in shifted left by the bits already read (a 64-bit
  shift), and returns the 64-bit value. A count of zero returns 0.

- `pciio_config_set` (`6.5.30/unix.IP27`, address 0xc00000000009331c, 152 bytes, 64-byte frame): **established.**
  Writes 1 to 8 bytes of configuration space with the same splitting. `a0` is the vertex, `a1` the register, `a2` the count, `a3` the value. For each piece it
  calls `pcibr_config_set(a0, offset, piece, value)`, then shifts the value right by the piece's bits (64-bit) and advances. It returns nothing.

- `pcibr_config_get` (`6.5.30/unix.IP27`, address 0xc00000000007c5c0, 56 bytes, 32-byte frame): **established.**
  Reads up to 4 bytes inside one word. It obtains the window address with `pcibr_config_addr(a0, a1)`, then returns `do_pcibr_config_get(address, a1 & 0xff, a2)`.

- `pcibr_config_set` (`6.5.30/unix.IP27`, address 0xc00000000007c6a4, 64 bytes, 32-byte frame): **established.**
  The write counterpart: `pcibr_config_addr(a0, a1)`, then `do_pcibr_config_set(address, a1 & 0xff, a2, a3)`. It returns that result.

- `pcibr_config_addr` (`6.5.30/unix.IP27`, address 0xc00000000007c4cc, 244 bytes, 48-byte frame): **established.**
  Works out the configuration window address of the device's function (register 0). `a0` is the device vertex, `a1` the register word. It calls
  `pciio_info_get(a0)` and `pcibr_info_get(a0)` and `pciio_info_type1_get(info)`. When the device is behind a PCI-to-PCI bridge (type 1 nonzero) it takes
  the bus from bits 16 to 23 of `a1`, the slot from bits 11 to 15 and the function from bits 8 to 10, and calls `pcibr_func_config_addr(register base, bus,
  slot, function, 0)`, where the register base is the doubleword at +8 of the record that the per-device record's +40 points to. Otherwise, when the device is not the host device (`pciio_info_hostdev_get(info) != a0`) it uses the bytes at info +16, +17 and +18
  as bus, slot and function (function 0xff becomes 0). For the host device it takes the slot from the byte at the Bridge's per-device record +540 (0xff
  becomes the slot field of `a1`) and the function from the byte at +18 (0xff becomes the function field of `a1`), with bus 0. In all cases it returns the
  address from `pcibr_func_config_addr` and writes nothing itself.

- `pcibr_func_config_addr` (`6.5.30/unix.IP27`, address 0xc00000000007c2ec, 132 bytes, 32-byte frame): **established.**
  `a0` is the Bridge register base (or soft record), `a1` the bus, `a2` the slot, `a3` the function, `a4` the register offset. For a bus above 0 (the
  test is `blez`) it first calls `pcireg_type1_cntr_set(a0, (bus << 16) | (slot << 11))`, then returns `pcireg_type1_cfg_addr(a0, function, offset)`.
  For bus 0 (or less) it returns `pcireg_type0_cfg_addr(a0, slot, function, offset)`.

- `pcibr_slot_config_addr` (`6.5.30/unix.IP27`, address 0xc00000000007c370, 44 bytes, 16-byte frame): **established.**
  `a0` is the base, `a1` the slot, `a2` the offset. It returns `pcibr_func_config_addr(a0, 0, slot, 0, offset)`: bus 0 and function 0.

- `pcireg_type0_cfg_addr` (`6.5.30/unix.IP27`, address 0xc000000000088480, 688 bytes, 48-byte frame): **established.**
  Builds the address of a configuration register of a device on the Bridge's own bus. `a0` is the soft record or base, `a1` the slot, `a2` the function, `a3`
  the offset. Part 0xc002, part 0xd002, and Bridge kinds 0 and 1 use one layout; parts 0xd102 and 0xd112 and kind 2 a second; parts 0xe000 and 0xe010 and kind 3
  a third. Layout one asserts `slot >= 0 && slot <= 7` (`assfail`, line 2933) and returns base + (slot << 12) + (function << 8) + (offset rounded down to a
  multiple of 4; a negative offset is first biased by 3) + 0x20000. The second and third use (slot + 1) & 0xff in place of the slot and assert it is in 1 to
  4 (`slot >= 1 && slot <= 4`; line 2928 for the second, 2920 for the third); the rest of the sum is the same. A zero `a0`, an unlisted part number or manufacturer,
  or kind -1 reaches `panic("pcireg_type0_cfg_addr(): unknown bridgetype")`; a kind above 3 returns 0. It reads the widget identification word (base +4) when it was
  given a base. No device register is written.

- `pcireg_type1_cfg_addr` (`6.5.30/unix.IP27`, address 0xc000000000088730, 468 bytes, 16-byte frame): **established.**
  The same dispatch for a device behind a PCI bridge chip. `a0` is the soft record or base, `a1` the function, `a2` the offset. Every accepted base or kind
  returns base + (function << 8) + (offset rounded down to a multiple of 4, a negative offset biased by 3 first) + 0x28000. A zero `a0`, an unlisted
  part number or manufacturer, or kind -1 reaches `panic("pcireg_type1_cfg_addr(): unknown bridgetype")`; a kind above 3 returns 0.

- `pcireg_type1_cntr_set` (`6.5.30/unix.IP27`, address 0xc00000000008318c, 344 bytes, 16-byte frame): **established.**
  Stores the bus and slot of the next type-1 configuration cycle in the Bridge. `a0` is the soft record or base, `a1` the value. Parts 0xd102, 0xd112, 0xe000,
  0xe010 and Bridge kinds 2 and 3 get a doubleword store at base +200; parts 0xc002 and 0xd002 and kinds 0 and 1 get a word store at base +204. A zero `a0`,
  an unlisted pair, or kind -1 calls `panic("pcireg_type1_cfg_set(): unknown bridgetype")`; a kind above 3 stores nothing. It sets no return value.

- `do_pcibr_config_get` (`6.5.30/unix.IP27`, address 0xc00000000007c658, 76 bytes, no frame): **established.**
  `a0` is the window address of the function, `a1` the register offset, `a2` the size. It loads the whole word at a0 + (offset >> 2) * 4, shifts it right by
  8 * (offset & 3) when that is nonzero, ANDs it with (1 << (8 * size)) - 1 when the size is under 4, and returns the 32-bit result zero-extended. The
  word is used as loaded; unlike the narrow stores in `do_pcibr_config_set`, no byte-lane swap is applied to the load.

- `do_pcibr_config_set` (`6.5.30/unix.IP27`, address 0xc00000000007c6e4, 312 bytes, no frame): **established.**
  `a0` is the window address, `a1` the offset, `a2` the size (1 to 4; any other value stores nothing), `a3` the value. Narrow stores swap the byte lane
  inside the word: size 1 is a byte store at a0 + (offset ^ 3). Size 2 with an even offset is a halfword store at a0 + 2 * ((offset >> 1) ^ 1); with an odd offset
  it is a byte at (offset ^ 3) and the value shifted right by 8 at ((offset + 1) ^ 3). Size 3 with an even offset is a halfword at the same halfword address and
  the value shifted right by 16 as a byte at ((offset + 2) ^ 3); with an odd offset it is a byte at (offset ^ 3) and the value shifted right by 8 as a
  halfword at a0 + 2 * ((((offset + 1) >> 1)) ^ 1). Size 4 is one word store of the value at a0 + (offset >> 2) * 4, unswapped. No return value is set.

- `kona_lowater_intr` (`6.5.30/unix.IP27`, address 0xc0000000003976c8, 216 bytes, 16-byte frame): **established.**
  The low-water step of the graphics board's main handler, called with the lock at +2352 held. `a0` is the board record (`s0`). It calls `gfx_waitf_on()` first
  and ends with `gfx_waitf_off()`, whose result it returns. It adds 1 to the word at +5264 and then does nothing more when the word at +104 is zero.
  Otherwise: when the doubleword at +2336 (a pending timeout) is nonzero it calls `kona_untimeout(record, that value)` and clears it; when it is zero it prints
  "pipe was backed up without timeout being set." (`cmn_err` level 4, prefixed with the board name and unit) and carries on. It removes bit 0x2 from the enable
  mask word at +2368 and writes the mask to window +0x800230 (the register that holds the enable mask: that reading is inferred from the order and the word it
  is stored from), clears the word at +2344, and then writes 3 to the status register at window +0x800238 (the high-water and low-water bits). When the word at +108 is
  zero it subtracts 1 from the word at +80. Finally it clears the words at +104 and +108. All device access goes through `gfx_pio_write_32`.

- `kona_xg_intr_handler` (`6.5.30/unix.IP27`, address 0xc0000000003a6f74, 112 bytes, 16-byte frame): **established.**
  The handler for an error interrupt from the board's crosstalk-side logic (the "XG" in the message); it is not called by `kona_intr_handler` and reads no device register.
  `a0` is the board record (`s0`). It adds 1 to the word at +16 of the CPU's 32-byte record, selected as in `kona_intr_handler` by the halfword at absolute address
  0xffffffffffffc420, and 1 to the board's word at +5268. It prints "XG error interrupt received." (`cmn_err` level 4, board name and unit from +2184),
  calls `kona_die(record, 12)` and returns the result of `kona_report_errs(record)`.

- `kona_die` (`6.5.30/unix.IP27`, address 0xc00000000039e4e8, 372 bytes, 64-byte frame): **established.**
  Stops the board after a fatal error. `a0` is the board record (`s0`), `a1` the reason code (kept at sp+24). It takes `mutex_spinlock(record+2168)`. When the word at
  +2172 is already nonzero (the board is dying) it only unlocks and returns the unlock's result. Otherwise, in this order: it sets the global word at `gp`+10100 to 1;
  stores the reason at +2172; reads the registers at window +0x800220 and +0x8003a8 into the words at +2176 and +2180; writes window +0x800220 with the word at
  +2364 with bit 15 (0x8000) cleared, window +0x8003a8 with 8, and window +0x800298 with 0xbadc0ffe; prints "Fatal error.  bdata = 0x%0lx death_reason = %ld"
  through `cmn_err_tag` (tag 2114, level 4; the record address is printed as bdata); reads window +0x800208 and prints it with "Graphics FIFO depth is %d words."
  (`cmn_err`, level 6); clears the words at +104, +108 and +120; and calls `gthread_scan(callback, block, 9)`, where the callback is the routine at
  0xc0000000000ea548 (from the `gp`+1272 slot) and the block on the stack holds the word 9, the board record and a zero doubleword. It then unlocks with the saved
  level and returns the unlock's result. What the three writes mean on the chip (a stop of the pipe, most likely) is not established.

- `kona_arm_intr` (`6.5.30/unix.IP27`, address 0xc0000000003a7288, 528 bytes, 48-byte frame): **established.**
  Handles the interrupt of the board's on-board processor (the "ARM"), reached from the main handler on status bit 0x10. `a0` is the board record (`s0`). It stores 1 at
  the word +5464, then reads one 32-bit value from window +0x800240 (the reason code) and chooses:
  - 0x100: `vsema(record+5176)`; when the doubleword at +5192 is nonzero, `kona_untimeout(record, that value)` and clear it.
  - 0x102: `kona_mem_swapin_complete(record, 0, 0)`.
  - 0x103: `mutex_spinlock(record+2576)`; when the doubleword at +5312 is nonzero, `vsema(record+5296)`, `kona_untimeout(record, that value)` and clear it; `mutex_spinunlock`.
  - 0: reads a text message the processor sends, one byte per read. Each pass writes 0x10 to the status register at window +0x800238, then polls that register (read,
    then `us_delay(20)`, at most 5001 reads) until bit 0x10 is set, stores 1 at +5464 again, reads window +0x800240 and keeps its low byte at record +2616 plus the number of bytes kept so far
    (only while fewer than 511 are kept; the zero-byte test still sees every byte), and stops at the first zero byte or after 5000 passes. It then writes a zero
    byte after the text, prints "ARM: %s" with the text (`cmn_err` level 6) and, when it ran 5000 passes, prints "ARM is spewing garbage (%d)" with the pass count (level 4).
  - anything else: prints "unknown reason for ARM interrupt (0x%x)" through `cmn_err_tag` (tag 2100, level 4, board name and unit, the value read).
  Every message is prefixed with the board name and the unit from +2184. Whether reading window +0x800240 pops a hardware queue is inferred from the byte-per-read loop.
