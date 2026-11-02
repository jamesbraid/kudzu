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
