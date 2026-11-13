# Routine role statements, tier-1 batch 18

These statements come from instruction facts for 25 routines picked from the ranked tier-1 target list, entries 451 through 475. Each one names the routine, its binary and its address. The statements use only the registers, calls, constants and addresses in those facts. Every grade is inferred: the facts do not fully establish any role, and the names are only a hint. Each binary is cited by its install path. The 6.5.22 build is cited where 6.5.30 has the same routine at the same address.

- `debug_stop_all_cpus` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x10f28
  **Inferred.** An 8-byte routine. Its facts contain only a return at 0x10f28 with v0 unresolved, and no calls, branches or memory accesses. The name suggests it stops the other processors, but nothing in these facts shows that, so the role is open.
  Reviewed: 2026-11-12 (sonnet)

- `cpu_waitforreset` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x11214
  **Inferred.** A 196-byte routine with a poll loop whose back branches at 0x112c0 and 0x112d0 return to 0x11264. It loads bytes from 0xffffffffbf3a0a07, 0xffffffffbf3a4a07 and 0xffffffffbf3a0c07, stores a byte to 0xffffffffbf3a0a07, and loads a doubleword from 0xffffffffbf310010. It calls `flash_write_env` at 0x11230 and `flushbus` at 0x1126c. It calls `cpu_powerdown` at 0x11284 only when bit 0x1 of the byte at 0xffffffffbf3a4a07 is set. An inner loop at 0x11298 calls `splx` on the result of `spl7`, calls `spl7` at 0x112a0, and repeats while bit 0x100 of the doubleword at 0xffffffffbf310010 is clear. It calls `cmn_err` at 0x112c8 with a0 = 0x4 when bit 0x80 of the byte at 0xffffffffbf3a0c07 is clear. The name fits a reset wait that powers down, but the facts do not show what any polled bit means.
  Reviewed: 2026-11-12 (sonnet)

- `_clock_func` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x129e8
  **Inferred.** A 576-byte routine that calls `splhi` at 0x129f8 and `splx` on its result at 0x12c00. It makes a run of byte-wide `pciio_pio_read8` and `pciio_pio_write8` calls on addresses from 0xffffffffbf3a0007 up to 0xffffffffbf3a4807. A branch on a `.sdata` word at 0x12a24 sends the zero case straight to the `rtc_init` call at 0x12c18. A branch on a0 at 0x12a50 picks one of two paths. When a0 is zero it reads eight of those registers and stores the bytes at a1+0 through a1+7. When a0 is nonzero it loads the bytes at a1+0 through a1+7 and writes them to eight of those registers with `pciio_pio_write8`. Role (from name): a clock tick handler; the facts do not give the meaning of any register.
  Reviewed: 2026-11-12 (sonnet)

- `_ticksper1024inst` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x140c0
  **Inferred.** A 56-byte loop that reads a doubleword from the fixed address 0xffffffffb4000038 at 0x140d0 and 0x140dc. The signed `bgtz` at 0x140d4 targets itself and the one at 0x140e8 targets 0x140d0, each tested on a register the facts leave unresolved. The return at 0x140f0 leaves v0 unresolved. The name suggests a count of ticks per 1024 instructions, but the facts show no arithmetic that would produce that unit.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14270
  **Inferred.** A 312-byte probe that dispatches on the width in a1 (tests against 1, 2, 4 and 8 from 0x142d4 to 0x1431c), then loads 1, 2, 4 or 8 bytes from a0. Before the load it stores doublewords to 0xffffffffb4000048 and 0xffffffffb4000250 and a word to 0xffffffffbf080004, and reads each back. It also loads the word at 0xffffffffffffa018 and stores a halfword at offset 0xc2 from that loaded value; it reloads the same word and repeats the halfword store at 0x1437c and 0x14380. The `cmn_err` call at 0x14338 is reached only when the width matches none of 1, 2, 4 or 8. After the load it reads the doubleword at 0xffffffffb4000028, and a set bit in mask 0x300080 (branch at 0x14374) sends control to `baerror` at 0x149a8; otherwise it returns 0. The name fits a probe that reports a bus error, but the facts do not show how the fault is caught.
  Reviewed: 2026-11-12 (sonnet)

- `badaddr_val` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x143a8
  **Inferred.** The same width-dispatched probe as `badaddr`, 380 bytes long. Each load from a0 is paired with a store of the same width to a2, from 0x1442c to 0x1448c, so it appears to copy the probed value out to the caller's buffer. It repeats the fixed-address stores, and it reaches `cmn_err` at 0x144a0 only when the width in a1 matches none of 1, 2, 4 or 8. A set bit in mask 0x300080 of the doubleword at 0xffffffffb4000028 sends control to `baerror` at 0x149a8; otherwise it returns 0. The facts do not show the buffer contract, so the value-returning interpretation stays unconfirmed.
  Reviewed: 2026-11-12 (sonnet)

- `wbadaddr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14524
  **Inferred.** A 356-byte probe with the same width dispatch and stores of 1, 2, 4 and 8 bytes to a0 from 0x14590 to 0x145d8. The facts do not name the register that supplies the stored value, so the write direction is clear and the data written is not. It shares the status word at 0xffffffffffffa018, the fixed-address stores, the `baerror` branch at 0x14654 and a `cmn_err` call at 0x145ec that the matched widths branch around. The name suggests a write probe, which the stores support.
  Reviewed: 2026-11-12 (sonnet)

- `earlybadaddr` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14808
  **Inferred.** A 296-byte probe like `badaddr`, but its width tests cover only 1, 2 and 4 (from 0x14870 to 0x148a0), so the facts show no 8-byte case. It loads 1, 2 or 4 bytes from a0 and writes a word to the `nofault` symbol at 0x14868 and 0x14908. A set bit at 0xffffffffb4000028 sends control to `earlybaerror` at 0x14a0c through the branch at 0x148f8. The name suggests a probe used before the normal fault path is set up, but the facts do not show that ordering.
  Reviewed: 2026-11-12 (sonnet)

- `baerror` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x149a8
  **Inferred.** A 100-byte routine that stores doublewords to 0xffffffffb4000048 and 0xffffffffb4000250 and reads them back, and does the same with a word at 0xffffffffbf080004. It loads the word at 0xffffffffffffa018 and, when that word is nonzero (branch at 0x149e4), stores a halfword at that word plus 0xc2. It then stores a word at 0xffffffffffffa1d8 and returns 1 at 0x14a04. The name fits a bus-error handler, but the facts do not show what the stores record or what a return of 1 tells the caller.
  Reviewed: 2026-11-12 (sonnet)

- `earlybaerror` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x14a0c
  **Inferred.** An 88-byte early variant of `baerror`. It does the same doubleword round trip through 0xffffffffb4000048 and 0xffffffffb4000250 and the same word access at 0xffffffffbf080004, then writes a word to the `nofault` symbol. It has no status-word branch, and it returns 1 at 0x14a5c.
  Reviewed: 2026-11-12 (sonnet)

- `_ecc_errorvec` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x15934
  **Inferred.** A 48-byte stub with one direct jump at 0x15944 to `ecc_exception`, which passes a0 through a3 unchanged. The instructions from 0x1594c to 0x15960 are not reached from the entry. The name points to an ECC error vector, and the jump fits a vector that only hands off to `ecc_exception`.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1c2d4
  **Inferred.** A 132-byte exception vector that copies error state into named records. It loads doublewords from 0xffffffffb4000048, 0xffffffffb4000040, 0xffffffffb4000250, 0xffffffffb4000258 and 0xffffffffb4000058, in that order, and stores each to its own named record; the record names say CPU error status, CPU error address, memory error status, memory error address and vice error address, in that order. It then stores doublewords back to 0xffffffffb4000250 and 0xffffffffb4000048, writes the word at 0xffffffffbf080004 and reads it back, stores a doubleword at a0+0x140, and jumps to `VEC_trap` at 0x1c350. The name points to a data bus error, and the record names agree, but the facts do not confirm which exception reaches this vector.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.22/usr/cpu/sysgen/IP32boot/kernel.o` at 0x20dc0
  **Inferred.** A 156-byte exception vector with the same capture pattern as `VEC_dbe`. Its first loads and one store go to addresses the facts leave unresolved, and a signed test at 0x20dc8 depends on a register the facts do not resolve. It stores at a0+0x140 and jumps to `VEC_trap` at 0x20e54. The name points to an instruction bus error, which the shared records fit, but the facts do not confirm the trigger.
  Reviewed: 2026-11-12 (sonnet)

- `_mraccess` in `6.5.22/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x35b8
  **Inferred.** A 240-byte metered-access routine that calls `wchanmeter` at 0x35d0 with the `mrinfolist` table. It calls `mr_access` at 0x35e0 whether `wchanmeter` returns zero or not. When the result is nonzero, a path through 0x3678 first calls `_get_timestamp` and then `mrmeter_entry` at 0x3690 with type code 0x5, and then reaches the `mr_access` call. When the result is zero it returns after `mr_access`. On the nonzero path it also calls `_get_timestamp` at 0x35f0 and `mrmeter_getentry` at 0x363c with a2 = 0x5, updates fields at offsets 0x160 through 0x170 of the record `wchanmeter` returns under further branches, and uses a load-linked and store-conditional pair on the word at offset 0x138 when `mrmeter_getentry` returns nonzero. The name fits a wait-channel access counter, which the calls support, but the facts do not give the meaning of any counter.
  Reviewed: 2026-11-12 (sonnet)

- `_mrupdate` in `6.5.22/usr/cpu/sysgen/IP32boot/ksync_metered.a(sema.o)` at 0x36e0
  **Inferred.** The update-side sibling of `_mraccess`, 220 bytes long. It calls `wchanmeter` at 0x36f8 and `mr_update` at 0x3708. When `wchanmeter` returns nonzero it also calls `mrmeter_entry` at 0x379c with type code 0x6 and `_get_timestamp` at 0x37a4, and then reaches the `mr_update` call. After `mr_update` it returns unless the `wchanmeter` result is nonzero, in which case it calls `_get_timestamp` at 0x3718. It compares a value loaded from the `wchanmeter` record at offset 0x14c with a register the facts leave unresolved at 0x3748, and it tests the mask 0x4 on a halfword whose address the facts leave unresolved at 0x3768. The shared record and the type code 0x6 fit an update path paired with the access path, but the facts show no more than that.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.22/usr/cpu/sysgen/IP32boot/mhpckm.o` at 0x10
  **Inferred.** A 1264-byte attach routine for the keyboard and mouse controller. It reads doublewords from 0xffffffffbf310000 and from the port in `kmports` plus 8 with `pciio_pio_read64`, calls `initkbdtype` at 0x38, and reads an option string with `kopt_find` at 0x70. It registers four character devices with `hwgraph_char_device_add`, and it sends a long run of `pckm_pollcmd` command bytes (0xf5, 0xf6, 0xf2, 0xed, 0xf0, 0xf4 and others) to the keyboard and mouse ports. The call to `setmaceisavector` at 0x318 passes 0x5, 0xa00 and `pckm_intr` as a0, a1 and a2. The command bytes match the usual keyboard and mouse controller set, which is an inference from the byte values, not something the facts state.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.22/usr/cpu/sysgen/IP32boot/os.a(pda.o)` at 0x1574
  **Inferred.** A 96-byte routine that loads bytes at 0xffffffffffffa070 and 0xffffffffffffa071, a word at 0xffffffffffffa074 and a doubleword at 0xffffffffffffa090, and stores bytes at 0xffffffffffffa03e and 0xffffffffffffa071 and a doubleword at 0xffffffffffffa090. It calls `panicspin` at 0x1584 when the byte at 0xffffffffffffa070 is nonzero, and it stores the two bytes when the byte at 0xffffffffffffa071 is nonzero. It calls `tlbflush_rand` at 0x15ac with a0 = 1 when the word at 0xffffffffffffa074 is nonzero. It calls `tlbflush` at 0x15c0 with the doubleword from 0xffffffffffffa090 when that value is nonzero. The name suggests a TLB flush after a vector update, but the facts show only flush calls and flag tests, so the vector role is open.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.22/usr/cpu/sysgen/IP32boot/os.a(syssgi.o)` at 0xcc
  **Inferred.** A 10956-byte request handler that reads a request code from the word at a0 and tests it against 0x4d7, branching to 0x130 at 0x11c when the code is at least that. It dispatches through a table at sym(.rodata)+112 and an indirect jump at 0x128 whose targets the facts leave unresolved, so the individual requests cannot be listed. It calls `get_current_abi` at 0xec and, when `sat_enabled` is nonzero, calls `sat_never` at 0x28fc and `sat_set_subsysnum` at 0x2904 with a halfword from a0+2. The name fits a system-call entry for SGI-specific requests, but the facts do not confirm it.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.22/usr/cpu/sysgen/IP32boot/os.a(time.o)` at 0xa8
  **Inferred.** A 396-byte routine that appears to fill a timeval-style buffer at a1, writing words at a1+0, +4, +8 and +0xc. A branch on a0 at 0xc4 picks one of two paths. When a0 is zero it calls `splprof` at 0xcc, takes the lock at [a2+488]+1480 with `mutex_lock` at 0xe0, copies four words from offsets 0x5b8 through 0x5c4 of [a2+488] into a1+0 through a1+0xc, and releases the lock with `mutex_unlock` at 0x108. It then branches on the word at a1+8 and other fields. Depending on those branches it calls `chktimeout_tick` at 0x160 (when the `fastclock` word is nonzero) and at 0x184, and it calls `tick_to_timeval` at 0x19c with a1+8 when the `chktimeout_tick` result is nonzero, then `splx` at 0x1a8 on the `splprof` result. When a0 is nonzero it calls `splhi` at 0x1b8, copies four words from a2+(a0<<4)+0x220 through +0x22c into a1+0 through a1+0xc, and calls `splx` at 0x1ec on the `splhi` result. Role (from name): the usual interval-timer read; the facts do not give the field meanings.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.22/usr/cpu/sysgen/IP35boot/cn.o` at 0xaac
  **Inferred.** A 284-byte console setup routine. It calls `gfx_earlyinit` at 0xac4 first. It then loads the halfword at `cn` plus 0x48. Only when that halfword is zero (branch at 0xad4) does it reach `spinlock_init` at 0xb2c and `init_mutex` at 0xb44 on the `cn` record, store halfwords and doublewords into `cn`, and call `is_specified` at 0xb5c on the console argument. Branches at 0xb74 and 0xb7c compare the first byte of the console argument with 0x67 and 0x47. `tp_init` at 0xb8c is reached when a byte in `.sbss` is zero (branch at 0xafc), and `du_init` at 0xba8 when a1 is zero (branch at 0xb04) or `tp_init` returns zero (branch at 0xb94). It stores halfwords at `cn` plus 0x48 and 0x4a and doublewords at `cn` plus 0x8, 0x18 and 0x20. The names fit console initialization, but the facts do not show which back end a given argument selects.
  Reviewed: 2026-11-12 (sonnet)

- `bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x1668
  **Inferred.** A 612-byte debug wrapper around bit-lock acquisition. It runs load-linked and store-conditional sequences on words at a2 plus 0xa8, 0xac, 0xb0, 0xb8 and 0xbc. It calls `try_bitlock` at 0x16cc and 0x17f8, reports through `cmn_err` at 0x18bc with the doubleword at 0x9200000001030100, and calls `splx` at 0x17e0 before an indirect `jalr` at 0x17e8 whose target is unknown. The name suggests a lock-debug check, which the report call fits, but the facts do not say what the bit-lock record holds.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitlockdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x1f0c
  **Inferred.** A 612-byte routine with the same layout as `bitlockdebug`, using `try_64bitlock` at 0x1f70 and 0x209c where that one uses `try_bitlock`. It runs the same load-linked and store-conditional sequences on words at a2 plus 0xa8 through 0xbc. It reports through `cmn_err` at 0x2160 with the same doubleword at 0x9200000001030100. The name suggests the 64-bit variant of the same check, and the matching layout supports that, but the facts do not show what the 64-bit records hold.
  Reviewed: 2026-11-12 (sonnet)

- `_64bitunlockdebug` in `6.5.22/usr/cpu/sysgen/IP35boot/dhardlocks.a(dhardlocks.o)` at 0x21d0
  **Inferred.** A 280-byte unlock routine. It compares the halfword at a2 plus 0x2a with the halfword at 0xffffffffffffc428 at 0x2208 and branches to 0x22a8 when they differ. It rewrites several words and doublewords in the record at a2, including offsets 0x40 through 0x80, and compares two registers the facts leave unresolved at 0x2244 and the word at a2+0x60 against an unresolved register at 0x2264. It has two `cmn_err` calls at 0x22c0 and 0x22d8, whose first arguments are 4 and 0. The name suggests a check at unlock time, which the compare fits, but the facts do not say what the halfword at 0xffffffffffffc428 identifies.
  Reviewed: 2026-11-12 (sonnet)

- `fake_a_part_number` in `6.5.22/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x170c
  **Inferred.** A 500-byte routine that writes a 13-byte value into the buffer at a0, using `sdl`, `sdr`, `swl`, `swr` and `sb` at offsets 0 through 0xc, with the bytes loaded from one of five fixed 13-byte constants in read-only data (at offsets 0x2f0, 0x300, 0x310, 0x320 and 0x330). A branch at 0x1740 tests a1 against 0x100, and a loop at 0x1750 through 0x1774 calls `random_eeprom_ch` with a0 = 0, stores a byte per pass to a base the facts leave unresolved, and compares its counter against 0xc, 0x3 and 0x8. Separately, the code from 0x17ac loads the doubleword at 0x9000000014000000, tests it against 0x4000 and 0x8000, calls `get_nasid` with that address as a0 at 0x17d0, and calls `get_chimera_type` on the result at 0x17d8. It repeats the `get_nasid` and `get_chimera_type` pair from 0x17ec to 0x182c with a0 unresolved. The chimera-type results are compared with 0x3, 0x1, 0x2 and 0x5 and branch to different constants. The name suggests a placeholder part number, but the facts do not show why the stored value is fake, so that stays open.
  Reviewed: 2026-11-12 (sonnet)

- `fake_a_product_name` in `6.5.22/usr/cpu/sysgen/IP35boot/eeprom.o` at 0x1984
  **Inferred.** A 520-byte routine that writes constant bytes into two buffers, one at a1 with the same mix of `sdl`, `sdr`, `swl`, `swr` and `sb` stores, and one at a0 with single-byte stores. It copies from read-only data at offsets 0x340 through 0x3b4. It calls `get_nasid` at 0x1ac8 on the base 0x9000000014000000 with a1 set to 0xc4 and a5 to 0x400, then `get_chimera_type` at 0x1ad0. It tests a4 against 0x100, 0x200 and 0x400, and tests values against 0x4000 and 0x8000 at 0x1ab0 and 0x1ac0. The name suggests a placeholder product string, like `fake_a_part_number`, but the facts do not show what the bytes mean or what the two buffers are.
  Reviewed: 2026-11-12 (sonnet)
