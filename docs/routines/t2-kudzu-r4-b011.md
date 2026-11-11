# Tier-2 reading batch 11 of kudzu

Routine role entries for batch 11 of the tier-2 reading round, from the `6.5.22/unix.IP27` build. Each
role is read from the rendered facts only: instruction addresses, callees and constants.

- `_elscuart_puts` in `6.5.22/unix.IP27` at 0xc00000000009bbc0
  **Inferred.** Loops over bytes loaded with `lbu` and sends each one through `_elscuart_putc` at 0xc00000000009bbec. It returns -1 when a put fails (the `bltz` at 0xc00000000009bbf4 tests the result) and 0 otherwise. The loop exit tests a1 against zero, so the facts do not show whether a1 is a count or a terminating character.

- `_elscuart_putc` in `6.5.22/unix.IP27` at 0xc00000000009bc30
  **Inferred.** Writes one character to the serial device whose state block is at a0, using the fields at a0+0x10c, a0+0x110 and a0+0x1b4. It compares the character against 0xa and 0xd at 0xc00000000009bc48 and 0xc00000000009bc60, builds a 0x31-byte line with `sprintf` at 0xc00000000009bd08, and drains through `_elscuart_flush` at 0xc00000000009bfec at four call sites. It returns -1 or 0.

- `_elscuart_gets` in `6.5.22/unix.IP27` at 0xc00000000009bf48
  **Inferred.** Reads characters with `_elscuart_getc` at 0xc00000000009bb34 and echoes each one through `_elscuart_putc` at 0xc00000000009bc30. A carriage return (0xd, tested at 0xc00000000009bf78) is echoed as a newline (0xa at 0xc00000000009bfc4), then the routine flushes through `_elscuart_flush` and returns a1. What a1 holds on entry is not established by the rendered facts.

- `_elscuart_flush` in `6.5.22/unix.IP27` at 0xc00000000009bfec
  **Inferred.** Works on a ring of queued bytes whose head and tail indexes are the words at a0+0x10c and a0+0x110 (the loop test at 0xc00000000009c03c compares them). It formats a name with `get_myid` at 0xc00000000009c06c and `sprintf` at 0xc00000000009c080, copies buffers with `bcopy` at 0xc00000000009c0b4 and 0xc00000000009c0dc, then hands the text to `elsc_command` at 0xc00000000009c0f0 and `elsc_parse` at 0xc00000000009c10c, and reads a hex value with `strtoull` (base 0x10) at 0xc00000000009c124. The callee names suggest a system-controller command layer, which the facts alone do not confirm.

- `return_0` in `6.5.22/unix.IP27` at 0xc00000000009d088
  **Inferred.** A stub that does nothing but return 0 in v0. Its eight bytes are one `jr` with the value in the delay slot.

- `return_1` in `6.5.22/unix.IP27` at 0xc00000000009d090
  **Inferred.** A stub that does nothing but return 1 in v0, the same shape as `return_0` at 0xc00000000009d088.

- `enable_heartbeat` in `6.5.22/unix.IP27` at 0xc00000000009d1f4
  **Inferred.** Sets a flag bit in the word at a0+0x38 and returns the result as `([a0+56] | 0x1)`. It schedules the `heartbeat` routine at 0xc00000000009d280 through `dtimeout` at 0xc00000000009d234 with a 0x64 (100) tick delay. The condition that guards the `dtimeout` call is not settled by the rendered facts, and the `cmn_err` call at 0xc00000000009d254 reports a value from a static word at abs 0xffffffffffffc750.

- `disable_heartbeat` in `6.5.22/unix.IP27` at 0xc00000000009d26c
  **Inferred.** Loads the word at a0+0x38 and stores it back at a0+0x38, then returns -2. The rendered facts do not show which bit the store clears. It is the counterpart of `enable_heartbeat` at 0xc00000000009d1f4, which uses the same field.

- `heartbeat` in `6.5.22/unix.IP27` at 0xc00000000009d280
  **Inferred.** A periodic callback that reschedules itself: `dtimeout` at 0xc00000000009d2b8 is given `heartbeat` as its callback with a 0x64 tick delay. Between the lock calls `sio_lock_port` at 0xc00000000009d290 and `sio_unlock_port` at 0xc00000000009d354, it takes the spin mutex at `[a0+64]+680` and calls `elsc_process` at 0xc00000000009aa90. The guards on the word at a0+0x38 and the bit test at a0+0x30 decide which of those steps run.

- `i2c_open` in `6.5.22/unix.IP27` at 0xc00000000009d36c
  **Inferred.** Tests bit 0x2 of the word at a0+0x38 and returns -1 or 0. The two return values are not separated by the rendered facts, so the function's effect beyond the status test is not established.

- `i2c_write` in `6.5.22/unix.IP27` at 0xc00000000009d388
  **Inferred.** The write side of a byte-at-a-time bus path. It takes the spin mutex at `[a0+64]+680` with `mutex_spinlock_spl` at 0xc00000000009d3d8, sends the bytes one at a time through `_elscuart_putc` at 0xc00000000009d3f4 in a loop over a2 (the `blez` at 0xc00000000009d3e4 is the loop test), and unlocks at two sites. It stores the status word back at a0+0x38 at 0xc00000000009d450.

- `i2c_wrflush` in `6.5.22/unix.IP27` at 0xc00000000009d454
  **Inferred.** Drains the write queue of the same bus path as `i2c_write` at 0xc00000000009d388. It takes the mutex with `mutex_spinlock_spl` at 0xc00000000009d484, calls `_elscuart_flush` at 0xc00000000009d494 on the state at `[a0+64]+48`, and unlocks at 0xc00000000009d4a8. It returns -1 when the flush fails (the `bltz` at 0xc00000000009d49c tests the result).

- `i2c_read` in `6.5.22/unix.IP27` at 0xc00000000009d4c8
  **Inferred.** The read side of the same bus path. Under the mutex from `mutex_spinlock` at 0xc00000000009d514, it polls with `_elscuart_poll` at 0xc00000000009d528, takes a byte with `_elscuart_getc` at 0xc00000000009d53c, and loops while s1 is positive (the `bgtz` at 0xc00000000009d558). It returns -1 when the poll or the read fails, and unlocks at 0xc00000000009d568.

- `i2c_notification` in `6.5.22/unix.IP27` at 0xc00000000009d580
  **Inferred.** A state-change handler that chooses between the two heartbeat routines. It calls `enable_heartbeat` at 0xc00000000009d5a0 or `disable_heartbeat` at 0xc00000000009d5c8, based on whether a2 is zero (the `beq` at 0xc00000000009d584) and on the low two bits of a4 (the `beq` at 0xc00000000009d598). It stores the word at a0+0x30 and returns 0.

- `i2c_set_extclk` in `6.5.22/unix.IP27` at 0xc00000000009d5d8
  **Inferred.** A stub that returns its second argument, a1, in v0. It is eight bytes long, from 0xc00000000009d5d8 to 0xc00000000009d5e0, and makes no calls.

- `klm_lockctl` in `6.5.22/unix.IP27` at 0xc0000000000bc8d4
  **Inferred.** Dispatches lock-control requests for the network lock manager. It looks up a record with `bhv_base_unlocked` at 0xc0000000000bc908, then calls `nlm4_call` at 0xc0000000000bc974, `nlm4_async_call` at 0xc0000000000bc9a0, `nlm1_call` at 0xc0000000000bc9cc and `nlm1_async_call` at 0xc0000000000bc9f8, with procedure 0x16 on some paths. It clears the 0x80000000 bit of the word at `[ret(bhv_base_unlocked)]+96` with `bitlock_clr` at 0xc0000000000bca4c and 0xc0000000000bca64, and returns 0x16 or a call result.

- `lockd_dispatch` in `6.5.22/unix.IP27` at 0xc0000000000be800
  **Inferred.** The request dispatcher for the lock daemon. It clears a 0x60-byte request block with `blkclr` at 0xc0000000000be8dc, runs `checkauth` at 0xc0000000000be96c, duplicates the request with `nlm_dup` at 0xc0000000000bead4 and completes it with `nlm_dupdone` at 0xc0000000000bec18, and brackets the work with `export_unlock` calls. Most of its per-procedure calls are indirect, so the facts do not map each request type to its handler.

- `record_remote_name` in `6.5.22/unix.IP27` at 0xc0000000000bfac0
  **Inferred.** Adds a remote client name to the status-monitor directory. It looks for a `/` with `strchr` at 0xc0000000000bfae4, takes the mutex at 0xc0000000014d1510, calls `add_direntry` at 0xc0000000000bfcd8 for each of two stored names, and opens the directory with `open_statmon_dir` at 0xc0000000000bfc60. Version locks around the directory use `bhv_version_read_lock` and `bhv_version_read_unlock`. It returns 0x16, or a value the facts do not fix.

- `ql_init_board` in `6.5.22/unix.IP27` at 0xc0000000000d3058
  **Inferred.** Probes a QLogic SCSI adapter on the PCI bus. It reads config words with `pciio_config_get` at 0xc0000000000d3094, 0xc0000000000d30ac and 0xc0000000000d30c0, compares the device id with 0x1077 and the chip ids 0x1020, 0x1240, 0x1080, 0x1280 and 0x1216, and allocates its state with `kmem_zalloc` (0x210 bytes at 0xc0000000000d3130, 0x2a8 bytes at 0xc0000000000d3294). It connects the interrupt with `pciio_intr_connect` at 0xc0000000000d3394 using `qlintr` at 0xc0000000000d43dc, and then runs `ql_init_isp` at 0xc0000000000d33a0.

- `ql_init_isp` in `6.5.22/unix.IP27` at 0xc0000000000d34d8
  **Inferred.** Brings up the chip state in the adapter block at a0. It takes the mutex at a0+208, loads firmware with `ql_download_fw` at 0xc0000000000d3534 when a1 is nonzero, and allocates two buffers with `kvpalloc` (flags 0x8101) at 0xc0000000000d35b0 and 0xc0000000000d360c. It then resets the interface with `ql_reset_interface` at 0xc0000000000d35f4 and stores configuration fields at offsets 0x46 through 0x70. The return value is not established by the rendered facts.

- `qlintr` in `6.5.22/unix.IP27` at 0xc0000000000d43dc
  **Inferred.** The interrupt handler for the QLogic adapter. It takes the adapter mutex at a0+208 with `mutex_lock`, calls `ql_service_interrupt` at 0xc0000000000d4454 with the flag word `([a0+40] | 0x40)`, and releases the mutex at 0xc0000000000d4420. It returns the result of that unlock.

- `ql_service_interrupt` in `6.5.22/unix.IP27` at 0xc0000000000d4464
  **Inferred.** The body of the QLogic completion path. It calls `ql_notify_responses` at 0xc0000000000d44c8 and 0xc0000000000d4818, `ql_service_mbox_interrupt` at 0xc0000000000d44f8 for mailbox completions, and `ql_start_scsi` at 0xc0000000000d4868 to restart queued commands. Error paths use `ql_reset_interface` at 0xc0000000000d4780, `ql_flush_queue` at 0xc0000000000d47bc and `ql_sensepr` at 0xc0000000000d4a04, and it ends in `panic` at 0xc0000000000d4cb4 on an unrecoverable case. The rendered facts do not map each status bit to its branch.

- `ql_mbox_cmd` in `6.5.22/unix.IP27` at 0xc0000000000d5b80
  **Inferred.** Sends a mailbox command to the QLogic adapter. It takes the mutexes at a0+192 and a0+208, waits on the semaphore at a0+224 with `psema` at 0xc0000000000d5d28, and polls with `us_delay` (10 ticks) at 0xc0000000000d5e50. While it waits it calls `ql_service_mbox_interrupt` at 0xc0000000000d5f50 and logs through `ql_ppmsg` at 0xc0000000000d5ed4, 0xc0000000000d5f30 and 0xc0000000000d5f90. It returns 0 or 1.

- `ql_reset_board` in `6.5.22/unix.IP27` at 0xc0000000000d67fc
  **Inferred.** Resets the QLogic adapter. It signals the semaphore at a0+224 with `vsema` at 0xc0000000000d6828, logs twice through `ql_ppmsg` at 0xc0000000000d6864 and 0xc0000000000d6924 with the 0x4000 flag ORed into `[a0+40]`, and calls `undo_quiesce` at 0xc0000000000d68a8 followed by `ql_flush_queue` at 0xc0000000000d68c0. It finishes with `ql_reset_interface` at 0xc0000000000d6930, and returns the result of `ql_flush_queue` or a value the facts do not fix.
