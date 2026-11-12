# Routine readings, kudzu reading round 1, batch 26

Role statements for 25 routines at positions 650 to 674 of the ranked round-1 list for kudzu (`6.5.30/unix.IP30`, the IP30 kernel image). Every statement comes from the disassembly for that routine: registers, call sites, constants and branches, cited by address. None of the 25 entries was dropped. Where a name suggests a role the facts do not show, the entry says so and leaves the point open.

- `cpu_exists` in `6.5.30/unix.IP30` at 0xa800000020012a34
  **Inferred.** The routine is a leaf with no calls and no branches. It loads a doubleword from the fixed address 0xffffffffaff00080 at 0xa800000020012a40 and returns a v0 the disassembler cannot track. The name suggests a CPU presence test, but the facts do not show the loaded value being compared, so the role is open.
  Reviewed: 2026-11-12 (sonnet)

- `ip30_chips_mlreset` in `6.5.30/unix.IP30` at 0xa800000020013560
  **Inferred.** Role (from name): a board-level reset sequence named `ip30_chips_mlreset`; the facts show only the calls and stores. It calls xtalk_set_early_piotrans_addr at 0xa800000020013568, heart_mlreset at 0xa800000020013570, xbow_mlreset at 0xa800000020013578 and ioc3_mlreset at 0xa800000020013588. Between 0xa8000000200135d8 and 0xa80000002001369c it calls heart_intr_prealloc six times and heart_intr_preconn three times. Three word stores sit at 0xa8000000200135c4, 0xa8000000200135d0 and 0xa8000000200135dc, around the first heart_intr_prealloc call, and the facts do not show their targets or values.
  Reviewed: 2026-11-12 (sonnet)

- `ip30_early_piotrans_addr` in `6.5.30/unix.IP30` at 0xa8000000200136b4
  **Inferred.** The routine makes three calls to heart_addr_xio_to_kva, at 0xa80000002001379c, 0xa8000000200137e0 and 0xa8000000200137f8, and its name suggests a conversion from an XIO address to a kernel address. Before those calls it loads a word at offset +4 from a base the facts do not resolve at 0xa8000000200136d8, and a word at 0xa800000020013714 whose address the facts do not show. A chain of compare-and-branch sites picks the path. The return is either the heart_addr_xio_to_kva result or 0. The meaning of the loaded words and of the branch tests is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `sendintr` in `6.5.30/unix.IP30` at 0xa800000020013a2c
  **Inferred.** This is a leaf with one doubleword store to the fixed address 0xffffffffaff10020 at 0xa800000020013a3c, followed by a return of 0. The neighbouring routine debug_stop_all_cpus stores to the same address, which weakly fits a name suggesting an inter-processor interrupt send. The facts do not show what the stored value does, so the role stays inferred.
  Reviewed: 2026-11-12 (sonnet)

- `debug_stop_all_cpus` in `6.5.30/unix.IP30` at 0xa800000020013a48
  **Inferred.** The routine makes no calls. It loops against a bound read from 0xa8000000204a6814 (the blez at 0xa800000020013a5c and the beq at 0xa800000020013a78 compare with it), reads halfwords addressed as [-0x3c60]:2, and stores a doubleword to 0xffffffffaff10020 at 0xa800000020013a8c and 0xa800000020013ab4. The name suggests it halts every CPU, but the facts do not show what the stored value means or what each pass of the loop does, so the role is open.
  Reviewed: 2026-11-12 (sonnet)

- `stop_bell` in `6.5.30/unix.IP30` at 0xa800000020014288
  **Inferred.** The routine calls spl1 at 0xa800000020014290 and splx at 0xa8000000200142a8, and returns the splx result. It stores one word through a pointer it loads from 0xa8000000204a6840; that store sits at 0xa8000000200142ac, in the delay slot of the splx call, so it executes before splx runs. pckbd_bell passes this routine's address as the first argument of timeout at 0xa800000020014334, which suggests it is used as a delayed callback. The name suggests it silences the bell, but the stored value and the pointer's target are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `pckbd_bell` in `6.5.30/unix.IP30` at 0xa8000000200142bc
  **Inferred.** The routine calls spl1 at 0xa8000000200142d0 and splx at 0xa8000000200142ec. When the argument a2 is zero or negative, the branch at 0xa8000000200142c8 goes to the return path at 0xa800000020014304. When a2 is positive, the code calls spl1 and then tests the word at 0xa8000000204ae3b0 at 0xa8000000200142e4. If that word is nonzero it calls splx and returns 1 (0xa8000000200142fc). If it is zero it reaches a word store at 0xa800000020014330 and a timeout call at 0xa800000020014334 that passes stop_bell's address as a0. The name fits a keyboard bell, but the meanings of the arguments and of that word are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `counter_intr` in `6.5.30/unix.IP30` at 0xa800000020014370
  **Inferred.** Role (from name): a counter interrupt handler named `counter_intr`; the facts show only calls, loads, stores and branches. It calls frs_handle_counter_intr at 0xa80000002001439c, hwperf_overflowed_intr at 0xa8000000200143c4 and hwperf_intr at 0xa8000000200143d4, the last only when the hwperf_overflowed_intr result is nonzero. It also calls resetcounter, first_timein, _set_timer_intr and resetlatency, calls _get_count and _get_compare, calls log_tstamp_event four times and calls cmn_err once at 0xa800000020014a0c. Which of these calls run depends on many compare-and-branch sites that the facts do not resolve, so the flow between the calls is inferred.
  Reviewed: 2026-11-12 (sonnet)

- `doadjtime` in `6.5.30/unix.IP30` at 0xa800000020015e3c
  **Inferred.** The routine has two copies of one sequence, one releasing the lock at 0xa800000020015f44 and one at 0xa800000020016018. Each raises the level with spl65 (a1 0xf0000), reads time with __nanotime, takes the lock at 0xa8000000204ae3d4 with nested_spinlock, and releases it with mutex_spinunlock. The second release passes 0x3e8 in register a4, whose meaning the facts do not show. The name suggests a clock adjustment, but the adjustment arithmetic is not in the facts, so the role beyond locked timekeeping is open.
  Reviewed: 2026-11-12 (sonnet)

- `__nanotime` in `6.5.30/unix.IP30` at 0xa800000020016030
  **Inferred.** The name suggests a nanosecond time read. The routine takes the lock at 0xa8000000204ae3d4 with mutex_spinlock_spl at 0xa80000002001603c and releases it with mutex_spinunlock at 0xa8000000200161cc and 0xa800000020016244. It reads the doubleword at 0xffffffffaff20000, the same address counter_intr reads, and keeps 64-bit and 32-bit state at 0xa8000000204b03a8 and 0xa8000000204b03a0, but the facts do not show how the counter value becomes the returned time.
  Reviewed: 2026-11-12 (sonnet)

- `findcpufreq_cycles` in `6.5.30/unix.IP30` at 0xa800000020016768
  **Inferred.** The routine loads a word from 0xffffffffffffc6f4 at 0xa800000020016768. If the word is nonzero, the branch at 0xa800000020016770 is not taken and the routine returns that word. If it is zero, the routine calls findcpufreq_hz at 0xa800000020016784 with the entry arguments a0 and a1, the loaded word (zero on this path) as a2, and the entry a3, and then stores a word to 0xffffffffffffc6f4 at 0xa800000020016798. The facts show the return after the call as an untracked value, not as the findcpufreq_hz result itself, and they do not show how the stored word is derived. The name suggests a frequency converted into cycles, but the units are not in the facts, so that reading is inferred from the name alone.
  Reviewed: 2026-11-12 (sonnet)

- `startkgclock` in `6.5.30/unix.IP30` at 0xa80000002001829c
  **Inferred.** The routine calls fasthzerr at 0xa8000000200182fc, 0xa800000020018384 and 0xa8000000200183f8 with 0x3e8, 0x1f4 and 0x9c4 as its third argument. Between them it calls spl65 at 0xa80000002001833c, which takes 0xf4240 in a4, calls set_timer_intr at 0xa800000020018360, and loads the word at 0xffffffffffffc6f4 that findcpufreq_cycles also uses (0xa8000000200182e0). It stores to 0xffffffffffffc56c and 0xffffffffffffc550, locations that counter_intr also touches, so it shares clock state with that routine, but the arithmetic that fixes the rate and the meaning of that state are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `slowkgclock` in `6.5.30/unix.IP30` at 0xa800000020018434
  **Inferred.** The routine raises the level with spl65 at 0xa80000002001843c, stores a doubleword to 0xffffffffffffc550 at 0xa80000002001844c, and restores the level with splx at 0xa800000020018448. The store goes to the location startkgclock writes, so the routine plausibly resets that slow-clock state, but the stored value is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.30/unix.IP30` at 0xa800000020018800
  **Inferred.** The routine polls the word at 0x900000001f000104: the load at 0xa800000020018810 is followed by a branch at 0xa800000020018818 that returns to the load while bit 0x40 of the word is clear. When bit 0x40 is set it falls through to a call to cpu_soft_powerdown at 0xa800000020018820. The name suggests a wait for a CPU reset, which fits a poll loop, but the facts do not say what bit 0x40 of that word means.
  Reviewed: 2026-11-12 (sonnet)

- `_clock_func` in `6.5.30/unix.IP30` at 0xa800000020018a64
  **Inferred.** The routine reads and writes 16-bit fields through its argument a1 at offsets 0x0 to 0xc, and it stores bytes to the two registers at 0x900000001f6a0000 and 0x900000001f6c0000, which cpu_clearpower and cpu_soft_powerdown also write. It runs under the lock at 0xa8000000204ae448 and brackets the register work with slow_ioc3_sio at 0xa800000020018a8c and restore_ioc3_sio at 0xa800000020018b70 and 0xa800000020018bd0. The name suggests a clock or timer routine, but the register meanings are not in the facts, so that role is open.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_clearpower` in `6.5.30/unix.IP30` at 0xa800000020018ff0
  **Inferred.** The routine writes byte values to 0x900000001f6a0000 and 0x900000001f6c0000 under the lock at 0xa8000000204ae448. It brackets that work with slow_ioc3_sio at 0xa800000020019008 and restore_ioc3_sio at 0xa80000002001903c, and calls flushbus at 0xa800000020019034 with a0 0x30 and a1 0xa. The name suggests clearing a power-control latch, but the stored values are not in the facts, so the role is inferred from the name and the shared addresses.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_soft_powerdown` in `6.5.30/unix.IP30` at 0xa800000020019060
  **Inferred.** The routine loads the pointer at [0xa8000000204ae450] and, only when it is nonzero, calls through it at 0xa800000020019070; the disassembler cannot name that callee. It then calls conbuf_flush at 0xa800000020019078 with a0 3 and heart_imr_bits_rmw at 0xa80000002001908c with the mask 0x200000000000000 in a1 and 0 in a2, so whether the call sets or clears the bit is not shown. It takes the lock at 0xa8000000204ae448, calls slow_ioc3_sio at 0xa8000000200190b8, stores bytes to and reads a byte from the same two byte registers that cpu_clearpower uses, and calls us_delay at 0xa800000020019118 with 0x989680 (10000000). Those register accesses all sit at lower addresses than the us_delay call. The sequence fits a soft power-off by name, but what the mask bit controls is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `slow_ioc3_sio` in `6.5.30/unix.IP30` at 0xa800000020019128
  **Inferred.** This is a leaf that reads the word at 0x900000001f600028 at 0xa800000020019130, stores to the same address at 0xa800000020019144, and returns the value it read. restore_ioc3_sio stores to the same address at 0xa800000020019164, so the two look like a save and restore pair. The name fits, but the facts do not show which device bits the word holds.
  Reviewed: 2026-11-12 (sonnet)

- `restore_ioc3_sio` in `6.5.30/unix.IP30` at 0xa800000020019148
  **Inferred.** The routine writes one byte to 0x900000001f6a0000 at 0xa800000020019158 and a word to 0x900000001f600028 at 0xa800000020019164, the address slow_ioc3_sio reads. It reads one byte from 0x900000001f6c0000 and returns it, with no calls. The callers in this batch, _clock_func and cpu_clearpower, use the pair around their own register work, which fits the name, but the value written and the meaning of the returned byte are not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `nvchecksum` in `6.5.30/unix.IP30` at 0xa8000000200191a0
  **Inferred.** The routine makes no calls. It stores bytes to 0x900000001f6a0000 and 0x900000001f6c0000 and has two byte loads from the second register, at 0xa8000000200191d8 and 0xa800000020019214, each inside a compare-and-branch loop bounded at 0x80 and 0x100 (the bne sites at 0xa8000000200191f4 and 0xa800000020019234). The name suggests a checksum over a stored region, but the facts do not show which memory the loops walk, so the role is inferred from the name.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.30/unix.IP30` at 0xa80000002001c200
  **Inferred.** The routine probes the address in a0, and its argument a1 selects a width of 1, 2, 4 or 8 for the matching load at 0xa80000002001c258, 0xa80000002001c270, 0xa80000002001c288 or 0xa80000002001c2a0. It reads and rewrites several fixed locations around the probe. It calls cmn_err at 0xa80000002001c2c4 with a0 0 only when a1 is none of 1, 2, 4 or 8, because the four width cases branch past that call. After the load it branches to baerror at 0xa80000002001c908 when a mask test with 0x4000 is nonzero or when bits of the word at 0xffffffffaff10040 are set (the bne at 0xa80000002001c304 tests mask 0x10000); otherwise it returns 0 at 0xa80000002001c320. The name suggests a bus-error-safe address probe, which these pieces fit, but the error mechanism is not in the facts.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.30/unix.IP30` at 0xa80000002001c328
  **Inferred.** This variant loads a value of width 1, 2, 4 or 8 from a0 at 0xa80000002001c390, 0xa80000002001c3b0, 0xa80000002001c3d0 and 0xa80000002001c3f0, and stores the same width at 0xa80000002001c398, 0xa80000002001c3b8, 0xa80000002001c3d8 and 0xa80000002001c3f8 to an address the facts do not resolve. It shares badaddr's fixed-location traffic. It calls cmn_err at 0xa80000002001c41c only when a1 is none of 1, 2, 4 or 8. After the loads it branches to baerror at 0xa80000002001c908 on the same two tests as badaddr (a 0x4000 mask test and the 0x10000 test of the word at 0xffffffffaff10040); otherwise it returns 0 at 0xa80000002001c48c. The stores suggest an output value, and the name suggests a probe that also returns the value, but neither is settled by the facts.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.30/unix.IP30` at 0xa80000002001c494
  **Inferred.** The name suggests a write-side probe. It stores a byte, halfword, word or doubleword to a0 at 0xa80000002001c4ec, 0xa80000002001c504, 0xa80000002001c51c and 0xa80000002001c534, after the same fixed-location traffic as badaddr. It calls cmn_err at 0xa80000002001c558 only when a1 is none of 1, 2, 4 or 8. It reads the word at 0x9000000000000000 at 0xa80000002001c590, after the stores, then branches to baerror at 0xa80000002001c908 on the same kind of test as badaddr; otherwise it returns 0 at 0xa80000002001c5d8. The name suggests a write probe that tolerates a bus error, but the facts do not show that error path.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.30/unix.IP30` at 0xa80000002001c748
  **Inferred.** This probe reads a byte, halfword or word from a0 at 0xa80000002001c7b4, 0xa80000002001c7cc and 0xa80000002001c7e4, and it has no doubleword case. It stores to 0xa8000000204af10c at 0xa80000002001c7a4 and 0xa80000002001c868, an address the disassembler tags as nofault. It calls cmn_err at 0xa80000002001c808 only when a1 is none of 1, 2 or 4. Its two error-bit tests (0xa80000002001c82c and 0xa80000002001c848) branch to 0xa80000002001c964, not to baerror, and it returns 0 at 0xa80000002001c878 when neither branch is taken. The name suggests a probe for use before the ordinary fault path is set up, which the narrower width set fits, but the facts do not show that setup.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.30/unix.IP30` at 0xa80000002001c908
  **Inferred.** The routine makes no calls. It reads the fixed locations 0xffffffffb8000060, 0xffffffffb8000000, 0xffffffffaff10040, 0xffffffffaff50000 and 0xffffffffffffc020, and writes some of them back. When the pointer loaded from 0xffffffffffffc020 ([-0x3fe0]:8) is nonzero, it stores a halfword at +0xf6 of it at 0xa80000002001c944. It stores a word to 0xffffffffffffc404 at 0xa80000002001c950 and returns 1 at 0xa80000002001c95c, where the probes above return 0. The name suggests the bus-error handler that the probes branch to, but the facts show no decoding of error bits, so the role is inferred.
  Reviewed: 2026-11-12 (sonnet)
