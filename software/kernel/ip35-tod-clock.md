# IP35: the kernel's time-of-day clock reader and writer

Scope. IRIX 6.5.30 `unix.IP35` (linked, symbols present): `rtodc` (0xc00000000006a438, 1324 bytes), `wtodc`
(0xc00000000006a964, 1060 bytes), `init_timebase` (0xc0000000000679e4). Read in Ghidra, with the decompiled body
checked against the disassembly. Grades: established (routine read in full), inferred, unknown.

## Established

- First use reads one fixed entry of the master node's configuration block, the first component, with no search for an
  IOC type, and keeps its register base. The id word is a PCI id (device in the high half, vendor `0x10a9` in the low
  half): `0x000310a9` (IOC3) probes the layout; `0x100a10a9` (IOC4) takes layout 1 without probing. Any other id
  leaves the cached layout word at 0, and the kernel then prints the `kgclock` message and carries on with unset
  values (the code after the call assumes `cmn_err` level 4 returns; that was not checked).
- Clock base is `0x80000` above the IOC register base. The probe is the one the IP35 PROM uses: 0 to byte `0xb`, `0xff` to byte 6,
  `0x80` to byte `0xb`, then a read of byte 6; `0xff` means layout 1, else layout 2.
- Layout 1: freeze byte `0x7ff8` (`0x40` to read, `0x80` to write, cleared afterwards), BCD seconds `0x7ff9` to year `0x7fff` (weekday
  `0x7ffc` is written as 0). Layout 2: seconds, minutes, hours, date, month, year at bytes 1, 2, 4, 8, 9, `0xa`, byte `0xb`
  cleared to read and set to `0x80` afterwards.
- Year base is **1968**: `rtodc` adds `0x7b0` to the BCD year; `wtodc` stores year minus `0x7b0` as BCD. Leap years
  use the 4/100/400 rule on the full year. Fields past sec 59, min 59, hour 23, date 31, month 12 print a warning and
  fall back to a fixed 1998 date, which `rtodc` stores in `time` and `wtodc` writes back.
- No century byte is read or written in either layout.

## Inferred

- Layout 1 is the M48T35 / DS1742 map; layout 2 matches the DS1286 register map (offsets only).

## Unknown

- The part on a real I-brick IO7. Neither the kernel nor the PROM names it. Route: a board photograph or a parts
  list. A read of byte 6 is not a safe route: the probe writes bytes `0xb` and 6, so it is not a read-only test.

## Review

Independent re-read of `unix.IP35` 6.5.30 in Ghidra (decompile of `rtodc` and `wtodc`) with a MIPS disassembly
cross-check. Confirmed: `rtodc` adds `0x7b0` (1968) and `wtodc` subtracts it, so both directions use 1968; the probe
sequence, the `0x80000` offset, both layouts' offsets, the IOC4 skip and the absence of a century byte all match.

Changed: `rtodc` is 1324 bytes (symbol size `0x52c`), not 1216; the first-use step reads one fixed component and does
not search for an IOC; the id words are PCI ids; the unset-layout and range-failure paths are described as the code
shows; the "read byte 6 on a real IO7" route removed because the probe writes.

- IOC3 or IOC4 on IO7: IOC3. established: 108-0264-004 printed p. 2-5 lists the IO7 as IOC3, USB and IEEE 1394 parts;
  007-4311-002 Figure 3-12 (printed p. 29) puts "TOD/NVRAM" with Ethernet and Serial on the I-brick board. The kernel
  therefore probes the layout on an I-brick; the IOC4 shortcut applies to the Origin 350 and Tezro boards.
- Layout on IO7: inferred, weak. The IO7 test's NVRAM range `0x7fd1` to `0x7ff7` (108-0264-004 printed p. 2-7) stops
  just below layout 1's freeze byte `0x7ff8`.
- Part: unknown. Library searches over techpubs, irix-mirrors, irix-artifacts and origami-docs for "IO7", "007-4653",
  "M48T35", "DS1742", "DS1286", "DS1687", "timekeeper", "030-1557" and I-brick FRU, battery and NVRAM wording found no
  source that names the part on IO7.
