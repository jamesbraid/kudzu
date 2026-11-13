# Tier-2 reading batch 11 of kudzu

Routine role entries for batch 11 of the tier-2 reading round, from the `6.5.22/unix.IP27` build. Each
role is read from the rendered facts only: instruction addresses, callees and constants.

- `_elscuart_puts` in `6.5.22/unix.IP27` at 0xc00000000009bbc0
  **Inferred.** Loops over bytes loaded with `lbu` and sends each one through `_elscuart_putc` at 0xc00000000009bbec. It returns -1 when a put fails (the `bltz` at 0xc00000000009bbf4 tests the result) and 0 otherwise. The loop exit tests a1 against zero, so the facts do not show whether a1 is a count or a terminating character.
  Reviewed: 2026-11-12 (sonnet)

- `_elscuart_putc` in `6.5.22/unix.IP27` at 0xc00000000009bc30
  **Inferred.** Role (from name): writes one character to a serial port whose state block is at a0, using the fields at a0+0x10c, a0+0x110 and a0+0x1b4. It compares the character in a1 against 0xa and 0xd at 0xc00000000009bc48 and 0xc00000000009bc60, calls `sprintf` at 0xc00000000009bd08 with a buffer on the stack and the constant arguments 0x31 and 0x41 (the facts do not show what the format string says), and calls `_elscuart_flush` at 0xc00000000009bfec from four call sites. It returns -1 or 0.
  Reviewed: 2026-11-12 (sonnet)

- `_elscuart_gets` in `6.5.22/unix.IP27` at 0xc00000000009bf48
  **Inferred.** Role (from name): reads characters with `_elscuart_getc` at 0xc00000000009bb34 and echoes them through `_elscuart_putc` at 0xc00000000009bc30. In the loop, each character that is not 0xd (the test at 0xc00000000009bf78) is echoed with `_elscuart_putc` at 0xc00000000009bf84, followed by `_elscuart_flush` at 0xc00000000009bf8c, a byte store and the next `_elscuart_getc` at 0xc00000000009bf9c. When the character is 0xd, the routine echoes it with `_elscuart_putc` at 0xc00000000009bfb4 and then sends 0xa with `_elscuart_putc` at 0xc00000000009bfc4. No `_elscuart_flush` call follows on that path in the rendered facts. It returns the value in a1; what a1 holds on entry is not established.
  Reviewed: 2026-11-12 (sonnet)

- `_elscuart_flush` in `6.5.22/unix.IP27` at 0xc00000000009bfec
  **Inferred.** Compares the words at a0+0x10c and a0+0x110 (the branch at 0xc00000000009c03c is a signed less-than between them), and a0 holds the state block. It calls `get_myid` at 0xc00000000009c06c and passes the result to `sprintf` at 0xc00000000009c080 with a destination of a0+440. It calls `bcopy` three times (0xc00000000009c0b4, 0xc00000000009c0c8 and 0xc00000000009c0dc), then `elsc_command` at 0xc00000000009c0f0 with a1 = 0, `elsc_parse` at 0xc00000000009c10c, and `strtoull` (base 0x10) at 0xc00000000009c124. It returns one of the `elsc_command` result, the `elsc_parse` result or 0. The facts do not establish what the two words are or what the text is; the callee names suggest a system-controller command layer, which they do not confirm.
  Reviewed: 2026-11-12 (sonnet)

- `return_0` in `6.5.22/unix.IP27` at 0xc00000000009d088
  **Inferred.** A stub that does nothing but return 0 in v0. Its eight bytes are one `jr` with the value in the delay slot.
  Reviewed: 2026-11-12 (sonnet)

- `return_1` in `6.5.22/unix.IP27` at 0xc00000000009d090
  **Inferred.** A stub that does nothing but return 1 in v0, the same shape as `return_0` at 0xc00000000009d088.
  Reviewed: 2026-11-12 (sonnet)

- `enable_heartbeat` in `6.5.22/unix.IP27` at 0xc00000000009d1f4
  **Inferred.** Tests the word at a0+0x34 first (the `beq` at 0xc00000000009d204). When it is nonzero, the routine loads and stores the word at a0+0x38 and returns `([a0+56] | 0x1)`. When it is zero, it calls `dtimeout` at 0xc00000000009d234 with `heartbeat` at 0xc00000000009d280 as a0, a0 of this routine as a1 and 0x64 as a2, stores to a0+0x34 afterwards, and calls `cmn_err` at 0xc00000000009d254 with level 4 and a static word at abs 0xffffffffffffc750 as an argument. The return on the zero path is not rendered.
  Reviewed: 2026-11-12 (sonnet)

- `disable_heartbeat` in `6.5.22/unix.IP27` at 0xc00000000009d26c
  **Inferred.** Loads the word at a0+0x38, stores a word to a0+0x38 (in the delay slot of the return) and returns -2. The rendered facts do not show what value is stored, so which bit changes is not established. It uses the same field as `enable_heartbeat` at 0xc00000000009d1f4.
  Reviewed: 2026-11-12 (sonnet)

- `heartbeat` in `6.5.22/unix.IP27` at 0xc00000000009d280
  **Inferred.** Calls `sio_lock_port` at 0xc00000000009d290 and `sio_unlock_port` at 0xc00000000009d354 around its work, and returns the unlock result. When bit 0x1 of the word at a0+0x38 is set (the `beq` at 0xc00000000009d2a0), it calls `dtimeout` at 0xc00000000009d2b8 with `heartbeat` itself as the callback and a 0x64 tick delay, and stores to a0+0x34. It then, on every path, takes the spin mutex at `[a0+64]+680` with `mutex_spinlock_spl` at 0xc00000000009d2d8, calls `elsc_process` at 0xc00000000009d2e8 with `[a0+64]+48`, and releases the mutex with `mutex_spinunlock` at 0xc00000000009d2f8. Afterwards two indirect calls (0xc00000000009d320 and 0xc00000000009d348) depend on bit 0x2 of the word at a0+0x30 and on the pointer at a0+0x8; their targets are not resolved.
  Reviewed: 2026-11-12 (sonnet)

- `i2c_open` in `6.5.22/unix.IP27` at 0xc00000000009d36c
  **Inferred.** Tests bit 0x2 of the word at a0+0x38 and returns -1 or 0. The two return values are not separated by the rendered facts, so the function's effect beyond the status test is not established.
  Reviewed: 2026-11-12 (sonnet)

- `i2c_write` in `6.5.22/unix.IP27` at 0xc00000000009d388
  **Inferred.** Returns early (the return at 0xc00000000009d3c8 lists -1 among its values) when bit 0x2 of the word at a0+0x38 is clear. Otherwise it takes the spin mutex at `[a0+64]+680` with `mutex_spinlock_spl` at 0xc00000000009d3d8. If a2 is greater than 0 (the `blez` at 0xc00000000009d3e4 skips the loop otherwise), it loops over bytes loaded with `lbu`, calling `_elscuart_putc` at 0xc00000000009d3f4 on `[a0+64]+48` for each, with a `bgtz` at 0xc00000000009d404 as the continue test and a `bltz` at 0xc00000000009d3fc leaving the loop when a put returns a negative value. It unlocks at two sites (0xc00000000009d414 after the loop, 0xc00000000009d434 after a failed put) and stores a word to a0+0x38 at 0xc00000000009d450. The returned value is not fully established.
  Reviewed: 2026-11-12 (sonnet)

- `i2c_wrflush` in `6.5.22/unix.IP27` at 0xc00000000009d454
  **Inferred.** Returns early (the return at 0xc00000000009d474) when bit 0x2 of the word at a0+0x38 is clear. Otherwise it takes the mutex with `mutex_spinlock_spl` at 0xc00000000009d484 and calls `_elscuart_flush` at 0xc00000000009d494 on `[a0+64]+48`. When the flush result is not negative, it unlocks with `mutex_spinunlock` at 0xc00000000009d4a8. When the result is negative (the `bltz` at 0xc00000000009d49c), control goes to 0xc00000000009d4b8, where no unlock call is rendered. Both paths load and store the word at a0+0x38. The return values are not established by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `i2c_read` in `6.5.22/unix.IP27` at 0xc00000000009d4c8
  **Inferred.** Returns early (the return at 0xc00000000009d508 lists -1 among its values) when bit 0x2 of the word at a0+0x38 is clear. Otherwise it takes the lock with `mutex_spinlock` at 0xc00000000009d514 on `[a0+64]+680`. While a2 is positive, it polls with `_elscuart_poll` at 0xc00000000009d528 and reads a byte with `_elscuart_getc` at 0xc00000000009d53c, both on `[a0+64]+48`, storing each byte; the loop ends when the poll returns 0, when the read returns -1, or when the count is used up (the `bgtz` at 0xc00000000009d558). It then unlocks with `mutex_spinunlock` at 0xc00000000009d568. The facts do not show what value is returned after the loop.
  Reviewed: 2026-11-12 (sonnet)

- `i2c_notification` in `6.5.22/unix.IP27` at 0xc00000000009d580
  **Inferred.** Role (from name): a state-change handler for the same device. When a2 is nonzero it loads and stores the word at a0+0x30 (0xc00000000009d588 and 0xc00000000009d590), then calls `enable_heartbeat` at 0xc00000000009d5a0 if the low two bits of a4 are nonzero (the `beq` at 0xc00000000009d598) and returns 0 at 0xc00000000009d5b0. When a2 is zero it stores the word at a0+0x30 at 0xc00000000009d5c4 and calls `disable_heartbeat` at 0xc00000000009d5c8; the same call is reached from a nonzero a2 when the low two bits of a4 are zero. The return after the `disable_heartbeat` call is not rendered.
  Reviewed: 2026-11-12 (sonnet)

- `i2c_set_extclk` in `6.5.22/unix.IP27` at 0xc00000000009d5d8
  **Inferred.** A stub that returns its second argument, a1, in v0. It is eight bytes long, from 0xc00000000009d5d8 to 0xc00000000009d5e0, and makes no calls.
  Reviewed: 2026-11-12 (sonnet)

- `klm_lockctl` in `6.5.22/unix.IP27` at 0xc0000000000bc8d4
  **Inferred.** Role (from name): lock-control entry of a network lock manager. It calls `bhv_base_unlocked` at 0xc0000000000bc908 with `[[a0]+48]+104`, then compares the value at `[[ret]+280]` with 1 and 4 (the `beq` branches at 0xc0000000000bc920 and 0xc0000000000bc928). The 4 case tests bit 0x10 of the word at `[ret]+96` and calls `nlm4_call` at 0xc0000000000bc974 (with a4 = 0 and a6 = 0x16) or `nlm4_async_call` at 0xc0000000000bc9a0 and 0xc0000000000bca14. The 1 case tests bit 0x1 of the same word and calls `nlm1_call` at 0xc0000000000bc9cc, `nlm1_async_call` at 0xc0000000000bc9f8 and 0xc0000000000bca34. Some calls pass 0x16 in a6. After `nlm4_call` and `nlm1_call` return, results are compared with 0x59 and 0xb. It calls `bitlock_clr` at 0xc0000000000bca4c and 0xc0000000000bca64 on the word at `[ret]+96` with a1 = 0xffffffff80000000 and a2 = 0x10 or 0x1. It returns 0x16 or one of the call results.
  Reviewed: 2026-11-12 (sonnet)

- `lockd_dispatch` in `6.5.22/unix.IP27` at 0xc0000000000be800
  **Inferred.** Role (from name): request dispatcher of a lock daemon. It checks the word at a2+0x14 against 0 and 5 (0xc0000000000be870 and 0xc0000000000be87c) and the word at a2+0x1c against a global limit. It clears 0x60 bytes on the stack with `blkclr` at 0xc0000000000be8dc. It calls `checkauth` at 0xc0000000000be96c only when bit 0x100 of a loaded word is clear (the `bne` at 0xc0000000000be960). It calls `nlm_dup` at 0xc0000000000bead4 and `nlm_dupdone` at 0xc0000000000bec18 with the `nlm_dup` result, and `export_unlock` at 0xc0000000000bea88, 0xc0000000000beb9c, 0xc0000000000bec28 and 0xc0000000000beca0. Most other calls (including those at 0xc0000000000be8ec, 0xc0000000000be948 and 0xc0000000000be9a8) are indirect, so the facts do not map each request type to its handler. The `cmn_err` calls are on error-looking paths whose conditions are not worked out here.
  Reviewed: 2026-11-12 (sonnet)

- `record_remote_name` in `6.5.22/unix.IP27` at 0xc0000000000bfac0
  **Inferred.** Role (from name): records a remote client name in a status-monitor directory. When a1 is nonzero it searches it for `/` with `strchr` (a1 = 0x2f) at 0xc0000000000bfae4 and returns early (the return at 0xc0000000000bfb00 lists 0x16 and 0x2e among its values) when one is found. Otherwise it takes the lock at 0xc0000000014d1510 with `mutex_spinlock`, may call `vn_hold` and `vn_rele`, calls `add_direntry` at 0xc0000000000bfbc0 and 0xc0000000000bfbd8 for the two pointers at 0xc0000000014c9fb0 and 0xc0000000014c9fb8 when each is nonzero, and calls `open_statmon_dir` at 0xc0000000000bfc60. It also calls `bhv_version_read_lock` and `bhv_version_read_unlock` on some paths. Other return values are not fixed by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `ql_init_board` in `6.5.22/unix.IP27` at 0xc0000000000d3058
  **Inferred.** Reads PCI config values with `pciio_config_get` at 0xc0000000000d3094 (offset 0, 2 bytes), 0xc0000000000d30ac (offset 2, 2 bytes) and 0xc0000000000d30c0 (offset 8, 1 byte). It compares the first value with 0x1077 and the second with 0x1020, 0x1240, 0x1080, 0x1280 and 0x1216. On a match it allocates state with `kmem_zalloc` (0x210 bytes at 0xc0000000000d3130 and 0x2a8 bytes at 0xc0000000000d3294, the latter in a loop), calls `pciio_intr_alloc` and then `pciio_intr_connect` at 0xc0000000000d3394 with `qlintr` at 0xc0000000000d43dc as the handler, and then `ql_init_isp` at 0xc0000000000d33a0. Role (from name): probe of a QLogic SCSI adapter.
  Reviewed: 2026-11-12 (sonnet)

- `ql_init_isp` in `6.5.22/unix.IP27` at 0xc0000000000d34d8
  **Inferred.** Works on the adapter block at a0. It takes the mutex at a0+208 with `mutex_lock` at 0xc0000000000d34fc and releases it with `mutex_unlock` at 0xc0000000000d354c, both only when a1 is nonzero. It allocates buffers with `kvpalloc` (a1 = 0x8101) at 0xc0000000000d35b0 and 0xc0000000000d360c when the pointers at a0+96 and a0+112 are zero, and stores configuration fields at offsets 0x46 through 0x70. It calls `ql_download_fw` at 0xc0000000000d3534 once both pointers are set. When that returns 0, or the first allocation fails, control reaches the path that calls `ql_reset_interface` at 0xc0000000000d35f4. The return value is not established by the rendered facts.
  Reviewed: 2026-11-12 (sonnet)

- `qlintr` in `6.5.22/unix.IP27` at 0xc0000000000d43dc
  **Inferred.** Role (from name): interrupt handler for the QLogic adapter. It takes the adapter mutex at a0+208 with `mutex_lock` at 0xc0000000000d43fc. When bit 0x20 of the word at a0+0x28 is clear and bit 0x4 of a halfword loaded at +0x8 is set (0xc0000000000d440c and 0xc0000000000d444c), it calls `ql_service_interrupt` at 0xc0000000000d4454 with `([a0+40] | 0x40)` as a2. It releases the mutex at 0xc0000000000d4420 with `mutex_unlock` and returns that result.
  Reviewed: 2026-11-12 (sonnet)

- `ql_service_interrupt` in `6.5.22/unix.IP27` at 0xc0000000000d4464
  **Inferred.** Role (from name): completion path of the QLogic adapter. It calls `ql_notify_responses` at 0xc0000000000d44c8 and 0xc0000000000d4818, `ql_service_mbox_interrupt` at 0xc0000000000d44f8 when bit 0x1 of a halfword at +0xe is set, and `ql_start_scsi` at 0xc0000000000d4868 under further tests. Other paths call `ql_reset_interface` at 0xc0000000000d4780, `ql_flush_queue` at 0xc0000000000d47bc and `ql_sensepr` at 0xc0000000000d4a04. It calls `panic` at 0xc0000000000d4cb4 on the path taken when a register value is at least the halfword at a0+68 (the `beq` at 0xc0000000000d4624). The return is one of the `ql_notify_responses`, `ql_start_scsi` or `us_delay` results or an unresolved value. The rendered facts do not map each status bit to its branch.
  Reviewed: 2026-11-12 (sonnet)

- `ql_mbox_cmd` in `6.5.22/unix.IP27` at 0xc0000000000d5b80
  **Inferred.** Role (from name): sends a mailbox command to the QLogic adapter. It releases the mutex at a0+208 (0xc0000000000d5bd0), then takes the mutexes at a0+192 and a0+208, and stores a series of halfwords. Under flag tests on the word at a0+0x28 it calls `mutex_unlock` on a0+208, `psema` on a0+224 (0xc0000000000d5d28) and `mutex_lock` again. In a polling loop it calls `us_delay` with an argument of 0xa at 0xc0000000000d5e50 and `ql_service_mbox_interrupt` at 0xc0000000000d5f50, and logs with `ql_ppmsg` at 0xc0000000000d5ed4, 0xc0000000000d5f30 and 0xc0000000000d5f90. It releases a0+192 at 0xc0000000000d5ddc and returns 0 or 1.
  Reviewed: 2026-11-12 (sonnet)

- `ql_reset_board` in `6.5.22/unix.IP27` at 0xc0000000000d67fc
  **Inferred.** Role (from name): resets the QLogic adapter. It calls `vsema` on a0+224 at 0xc0000000000d6828 when bit 0x8 of the word at a0+0x28 is set. It then branches on bit 0x4000 of that word. When the bit is set, it calls `ql_ppmsg` at 0xc0000000000d6864 (a2 = 0x20, a3 = `([a0+40] | 0x4000)`), loops over the count at a0+68 calling `undo_quiesce` at 0xc0000000000d68a8 (only under a test on bit 0x40 of a loaded word) and `ql_flush_queue` at 0xc0000000000d68c0, and returns an unresolved value or the `ql_flush_queue` result. When the bit is clear, it calls `ql_ppmsg` at 0xc0000000000d6924 and `ql_reset_interface` at 0xc0000000000d6930 (a1 = 1), and then stores to a0+0x28.
  Reviewed: 2026-11-12 (sonnet)
