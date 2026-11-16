# IP35: the kernel's time-of-day clock reader and writer

Scope. IRIX 6.5.30 `unix.IP35` (linked, symbols present): `rtodc` (0xc00000000006a438, 1216 bytes), `wtodc`
(0xc00000000006a964, 1060 bytes), `init_timebase` (0xc0000000000679e4). Read in Ghidra, with the decompiled body
checked against the disassembly. Grades: established (routine read in full), inferred, unknown.

## Established

- First use walks the master node's configuration block for the first IOC-type component and keeps its register
  base. The component id word `0x000310a9` (IOC3) probes the layout; `0x100a10a9` (IOC4) takes layout 1 without
  probing. Any other id leaves the earlier cached layout word, and if that is neither 1 nor 2 the kernel raises a
  `kgclock` error.
- Clock base is `0x80000` above the IOC register base. The probe is the one the IP35 PROM uses (bytes `0xb`, 6, `0xb`;
  byte 6 reading `0xff` means layout 1, else layout 2).
- Layout 1: freeze byte `0x7ff8` (`0x40` to read, `0x80` to write), BCD seconds `0x7ff9` to year `0x7fff` (weekday
  `0x7ffc` is written as 0). Layout 2: seconds, minutes, hours, date, month, year at bytes 1, 2, 4, 8, 9, `0xa`, byte `0xb`
  cleared to read and set to `0x80` afterwards.
- Year base is **1968**: `rtodc` adds `0x7b0` to the BCD year; `wtodc` stores year minus `0x7b0` as BCD. Leap years
  use the 4/100/400 rule on the full year. Fields past sec 59, min 59, hour 23, date 31, month 12 print a warning and
  fall back to a fixed 1998 date, which `wtodc` writes back.
- No century byte is read or written in either layout.

## Inferred

- Layout 1 is the M48T35 / DS1742 map; layout 2 matches the DS1286 register map (offsets only).

## Unknown

- Which layout and part a real I-brick IO7 presents. Neither the kernel nor the PROM names the part. Route: a read
  of byte 6 after the probe on a real IO7, or a board photograph.
