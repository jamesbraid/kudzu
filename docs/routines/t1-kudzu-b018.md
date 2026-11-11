# Routine role statements, tier-1 batch 18

These statements come from instruction facts for 25 routines picked from the ranked tier-1 target list, entries 451 through 475. Each one names the routine, its binary and its address. The statements use only the registers, calls, constants and addresses in those facts. Every grade is inferred: the facts do not fully establish any role, and the names are only a hint. Each binary is cited by its install path. The 6.5.22 build is cited where 6.5.30 has the same routine at the same address.

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10f28
  **Inferred.** An 8-byte routine. Its facts contain only a return at 0x10f28 with v0 unresolved, and no calls, branches or memory accesses. The name suggests it stops the other processors, but nothing in these facts shows that, so the role is open.

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x11214
  **Inferred.** A 196-byte routine with a poll loop between 0x11264 and 0x112d0 that reads bytes at fixed addresses from 0xffffffffbf3a0a07 to 0xffffffffbf3a4a07 and a doubleword at 0xffffffffbf310010. It also calls `flash_write_env` at 0x11230, `flushbus` at 0x1126c, `cpu_powerdown` at 0x11284 and `cmn_err` at 0x112c8, with `spl7` and `splx` around the wait. The name fits a reset wait that powers down, which the calls support, but the facts do not show which bits the poll tests.

- `_clock_func` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x129e8
  **Inferred.** A 576-byte routine that runs under `splhi` at 0x129f8 and `splx` at 0x12c00. It makes a long run of byte-wide `pciio_pio_read8` and `pciio_pio_write8` calls to registers between 0xffffffffbf3a0007 and 0xffffffffbf3a4a07, and it moves eight bytes in both directions between the buffer at a1 and that register window. A branch on a `.sdata` word at 0x12a24 sends zero values straight to the `rtc_init` call at 0x12c18. The name suggests a clock tick handler, and the register window fits a real-time clock, but the facts do not give the meaning of any register.

- `_ticksper1024inst` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x140c0
  **Inferred.** A 56-byte loop that reads a doubleword from the fixed address 0xffffffffb4000038 at 0x140d0 and 0x140dc. Signed `bgtz` tests at 0x140d4 and 0x140e8 close the loop, and the return at 0x140f0 leaves v0 unresolved. The name suggests a count of ticks per 1024 instructions, but the facts show no arithmetic that would produce that unit.

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14270
  **Inferred.** A 312-byte probe that dispatches on the width in a1 (tests against 1, 2, 4 and 8 from 0x142d4 to 0x1431c), then loads 1, 2, 4 or 8 bytes from a0. Around the load it stores to and reads back the fixed addresses 0xffffffffb4000048, 0xffffffffb4000250 and 0xffffffffbf080004, and it tests the word at 0xffffffffffffa018 for zero. A set bit in the doubleword at 0xffffffffb4000028 (mask 0x300080, branch at 0x14374) sends control to `baerror` at 0x149a8, and the `cmn_err` call at 0x14338 is the failure report. The name fits a probe that reports a bus error, but the facts do not show how the fault is caught.

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x143a8
  **Inferred.** The same width-dispatched probe as `badaddr`, 380 bytes long. Each load is paired with a store of the same width to a2, from 0x1442c to 0x1448c, so it appears to copy the probed value out to the caller's buffer. It repeats the fixed-address stores, calls `cmn_err` at 0x144a0 on failure, and sends a set bit in the doubleword at 0xffffffffb4000028 to `baerror` at 0x149a8. The facts do not show the buffer contract, so the value-returning interpretation stays unconfirmed.

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14524
  **Inferred.** A 356-byte probe with the same width dispatch and stores of 1, 2, 4 and 8 bytes to a0 from 0x14590 to 0x145d8. The facts do not name the register that supplies the stored value, so the write direction is clear and the data written is not. It shares the status word at 0xffffffffffffa018, the fixed-address stores, the `cmn_err` report at 0x145ec and the `baerror` branch at 0x14654. The name suggests a write probe, which the stores support.

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14808
  **Inferred.** A 296-byte probe like `badaddr`, but its width tests cover only 1, 2 and 4 (from 0x14870 to 0x148a0), so the facts show no 8-byte case. It loads 1, 2 or 4 bytes from a0 and writes a word to the `nofault` symbol at 0x14868 and 0x14908. A set bit at 0xffffffffb4000028 sends control to `earlybaerror` at 0x14a0c through the branch at 0x148f8. The name suggests a probe used before the normal fault path is set up, but the facts do not show that ordering.

- `baerror` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x149a8
  **Inferred.** A 100-byte routine that round-trips doublewords through 0xffffffffb4000048 and 0xffffffffb4000250 and a word through 0xffffffffbf080004. It reads the word at 0xffffffffffffa018, branches on whether that word is zero at 0x149e4, and writes a halfword at that word plus 0xc2 and a word at 0xffffffffffffa1d8. It returns 1 at 0x14a04. The name fits a bus-error handler and the facts show status capture, but the facts do not show what a return of 1 tells the caller.

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14a0c
  **Inferred.** An 88-byte early variant of `baerror`. It does the same doubleword round trip through 0xffffffffb4000048 and 0xffffffffb4000250 and the same word access at 0xffffffffbf080004, then writes a word to the `nofault` symbol. It has no status-word branch, and it returns 1 at 0x14a5c.

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x15934
  **Inferred.** A 48-byte stub with one direct jump at 0x15944 to `ecc_exception`, which passes a0 through a3 unchanged. The instructions from 0x1594c to 0x15960 are not reached from the entry. The name points to an ECC error vector, and the jump fits a vector that only hands off to `ecc_exception`.

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1c2d4
  **Inferred.** A 132-byte exception vector that copies error state into named records. It loads CPU error status and address from 0xffffffffb4000040 through 0xffffffffb4000250, memory error status and address from 0xffffffffb4000258 and 0xffffffffb4000058, and it stores the vice error address. It also writes the word at 0xffffffffbf080004, stores a doubleword at a0+0x140, and jumps to `VEC_trap` at 0x1c350. The name points to a data bus error, and the record names agree, but the facts do not confirm which exception reaches this vector.

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x20dc0
  **Inferred.** A 156-byte exception vector with the same capture pattern as `VEC_dbe`. Its first loads and one store go to addresses the facts leave unresolved, and a signed test at 0x20dc8 depends on a register the facts do not resolve. It stores at a0+0x140 and jumps to `VEC_trap` at 0x20e54. The name points to an instruction bus error, which the shared records fit, but the facts do not confirm the trigger.

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x35b8
  **Inferred.** A 240-byte metered-access routine that calls `wchanmeter` at 0x35d0 with the `mrinfolist` table. It then calls either `mr_access` at 0x35e0 or `mrmeter_getentry` at 0x363c, stamps times with `_get_timestamp` at 0x35f0 and 0x3678, and writes an entry through `mrmeter_entry` at 0x3690 with type code 0x5. It updates counters in the record `wchanmeter` returns at offsets 0x138 through 0x170, using a load-linked and store-conditional pair on the word at 0x138. The name fits a wait-channel access counter, which the calls support, but the facts do not give the meaning of any counter.

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x36e0
  **Inferred.** The update-side sibling of `_mraccess`, 220 bytes long. It calls `wchanmeter` at 0x36f8, `mr_update` at 0x3708 and `mrmeter_entry` at 0x379c with type code 0x6. It compares the word at `wchanmeter` plus 0x14c with a0 at 0x3748, and it tests the mask 0x4 at 0x3768. The shared record and the type code 0x6 fit an update path paired with the access path, but the facts show no more than that.

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP32boot/mhpckm.o` at 0x10
  **Inferred.** A 1264-byte attach routine for the keyboard and mouse controller. It reads doublewords from 0xffffffffbf310000 and from the port in `kmports` plus 8, calls `initkbdtype` at 0x38, and reads an option string with `kopt_find` at 0x70. It registers four character devices with `hwgraph_char_device_add`, and it sends a long run of `pckm_pollcmd` command bytes (0xf5, 0xf6, 0xf2, 0xed, 0xf0, 0xf4 and others) to the keyboard and mouse ports. The call to `setmaceisavector` at 0x318 passes `pckm_intr` and vector 0xa00. The command bytes match the usual keyboard and mouse controller set, which is an inference from the byte values, not something the facts state.

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP32boot/os.a(pda.o)` at 0x1574
  **Inferred.** A 96-byte routine that reads and writes bytes and a word at fixed addresses from 0xffffffffffffa03e to 0xffffffffffffa090, and a doubleword at 0xffffffffffffa090. It calls `panicspin` at 0x1584 when the byte at -0x5f90 is nonzero, calls `tlbflush_rand` at 0x15ac with a0=1, and calls `tlbflush` at 0x15c0 with a value loaded from -0x5f70. The name suggests a TLB flush after a vector update, but the facts show only flush calls and flag tests, so the vector role is open.

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP32boot/os.a(syssgi.o)` at 0xcc
  **Inferred.** A 10956-byte request handler that reads a request code from the word at a0 and tests it against 0x4d7, branching to 0x130 at 0x11c when the code is at least that. It dispatches through a table at sym(.rodata)+112 and an indirect jump at 0x128 whose targets the facts leave unresolved, so the individual requests cannot be listed. It calls `get_current_abi` at 0xec and, when `sat_enabled` is nonzero, calls `sat_never` at 0x28fc and `sat_set_subsysnum` at 0x2904 with a halfword from a0+2. The name fits a system-call entry for SGI-specific requests, but the facts do not confirm it.

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP32boot/os.a(time.o)` at 0xa8
  **Inferred.** A 396-byte routine that appears to fill a timeval-style buffer at a1, writing words at a1+0, +4, +8 and +0xc from per-process timer state reached through [a2+488]. It takes the lock at [a2+488]+1480 with `mutex_lock` at 0xe0 and releases it at 0x108. It calls `chktimeout_tick` at 0x160 and 0x184 and `tick_to_timeval` at 0x19c, and raises and drops priority with `splprof`, `splhi` and `splx`. The name is the usual interval-timer read, which the lock and conversion calls support, but the facts do not give the field meanings.

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP35boot/cn.o` at 0xaac
  **Inferred.** A 284-byte console setup routine. It calls `gfx_earlyinit` at 0xac4, then `spinlock_init` at 0xb2c and `init_mutex` at 0xb44 on the `cn` record. It calls `is_specified` at 0xb5c on the console argument, then `tp_init` at 0xb8c and `du_init` at 0xba8, and it stores halfwords at `cn` plus 0x48 and 0x4a and doublewords at `cn` plus 0x8, 0x18 and 0x20. Branches at 0xb74 and 0xb7c compare the first byte of the console argument with 0x67 and 0x47. The names fit console initialization, but the facts do not show which back end a given argument selects.

- `bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** A 612-byte debug wrapper around bit-lock acquisition. It runs load-linked and store-conditional sequences on words at a2 plus 0xa8, 0xac, 0xb0, 0xb8 and 0xbc. It calls `try_bitlock` at 0x16cc and 0x17f8, reports through `cmn_err` at 0x18bc with the doubleword at 0x9200000001030100, and calls `splx` at 0x17e0 before an indirect `jalr` at 0x17e8 whose target is unknown. The name suggests a lock-debug check, which the report call fits, but the facts do not say what the bit-lock record holds.

- `_64bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** A 612-byte routine with the same layout as `bitlockdebug`, using `try_64bitlock` at 0x1f70 and 0x209c where that one uses `try_bitlock`. It runs the same load-linked and store-conditional sequences on words at a2 plus 0xa8 through 0xbc. It reports through `cmn_err` at 0x2160 with the same doubleword at 0x9200000001030100. The name suggests the 64-bit variant of the same check, and the matching layout supports that, but the facts do not show what the 64-bit records hold.

- `_64bitunlockdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** A 280-byte unlock routine. It compares the halfword at a2 plus 0x2a with the halfword at 0xffffffffffffc428 at 0x2208 and branches to 0x22a8 when they differ. It rewrites several words and doublewords in the record at a2, including offsets 0x40 through 0x80, and tests a counter against a limit at 0x2244 and 0x2264. It has two `cmn_err` calls at 0x22c0 and 0x22d8, whose first arguments are 4 and 0. The name suggests a check at unlock time, which the compare fits, but the facts do not say what the halfword at 0xffffffffffffc428 identifies.

- `fake_a_part_number` in `6.5.22/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x170c
  **Inferred.** A 500-byte routine that writes a 13-byte value into the buffer at a0, using `sdl`, `sdr`, `swl`, `swr` and `sb` at offsets 0 through 0xc. It calls `random_eeprom_ch` at 0x1750 with 0, then `get_nasid` on the fixed base 0x9000000014000000 and `get_chimera_type` on the result. It repeats the `get_nasid` and `get_chimera_type` pair with other arguments from 0x17ec to 0x182c, and copies one of several fixed 13-byte constants from read-only data. The choice depends on the chimera type compared with 0x3, 0x1, 0x2 and 0x5 and on the values 0x4000 and 0x8000. The name suggests a placeholder part number, but the facts do not show why the stored value is fake, so that stays open.

- `fake_a_product_name` in `6.5.22/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x1984
  **Inferred.** A 520-byte routine that writes constant text into two buffers, one at a1 with the same mix of `sdl`, `sdr`, `swl`, `swr` and `sb` stores, and one at a0 with single-byte stores. It copies from read-only data at offsets 0x340 through 0x3b4. It calls `get_nasid` at 0x1ac8 on the base 0x9000000014000000 with a1 set to 0xc4 and a5 to 0x400, then `get_chimera_type` at 0x1ad0. It tests a4 against 0x100, 0x200 and 0x400, and tests values against 0x4000 and 0x8000 at 0x1ab0 and 0x1ac0. The name suggests a placeholder product string, like `fake_a_part_number`, but the facts do not show what the two buffers are.
