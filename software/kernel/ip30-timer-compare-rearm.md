# IP30 scheduler clock: how the kernel re-arms CP0 Compare, and the 6.5.13f pull-in defect

The IP30 kernel keeps the scheduler clock on CP0 Count and Compare. Two of its routines write Compare after a tick or a new timer request: `resetcounter` (the tick re-arm) and `trimcompare` (pull the next deadline earlier). This note reads both from two releases and explains a reported 44-second gap. The report is not a hardware capture, so whether the gap occurs on real hardware is unknown (see the questions at the end).

Releases read:

| Release | Binary | SHA-256 | Note |
|---|---|---|---|
| 6.5.30 | `usr/cpu/sysgen/IP30boot/kernel.o`, `.text` | `32d900592e14a5572a4ce1ec9947ef4b6c7730491303aa8f0d3d8ec9a0fbde31` | Relocatable object; offsets are `.text` offsets. |
| 6.5.13f | installed `unix.IP30` (the `/unix` of an Octane install, version string 6.5.13f) | `ca22f8eb9addfe4bab1b1517825341d10ebb1facd2b916ae10498c323078e3fa` | Linked; addresses are virtual. A second `unix.IP30` image on the same disk has identical routines at identical addresses. |

6.5.13f is outside this repository's usual release scope; facts from it are graded on that binary only and do not fill gaps for 6.5.30 or 6.5.22. The routine symbols (`resetcounter`, `trimcompare`, `initcounter`, `resetlatency`) are in both binaries' own symbol tables.

## The two routines

Both mask interrupts around the whole read-modify-write: they save Status, write 0x80 (only KX set, so IE is clear and every interrupt mask bit is clear), and restore Status in the delay slot of the return. Neither touches memory. The kernel text sits in the cached direct-mapped window (virtual `0xa800000020005700` is physical `0x20005700`), so a TLB miss cannot occur inside either routine. Interrupts are therefore masked across the window; only NMI, soft reset and cache-error exceptions, which ignore IE, could intervene.

### `resetcounter(a0 = interval)` (6.5.13f `0xa800000020005700`, 6.5.30 `.text+0x1800`; 112 bytes)

Reads Compare, reads Count, writes Count to zero, then:

- **Compare already reached or passed** (Count not below Compare): overshoot = Count - Compare. If the interval is smaller than the overshoot, Compare is written with the 1000-tick minimum; otherwise Compare = interval - overshoot, raised to 1000 if smaller. The Compare write is eight register instructions after the Count write (6.5.13f `0x...571c` to `0x...573c`).
- **Compare not yet reached**: the Count value just read is written back (undoing the reset), and Compare is rewritten, to the interval (at least 1000), only if interval + 1000 or more ticks remain; otherwise it is left alone.

Same bytes in both releases. Established, both releases.

### `trimcompare(a0 = ticks to pull in)` (6.5.13f `0xa800000020005780`, 6.5.30 `.text+0x1880`; 96 bytes)

Intended behaviour (6.5.30): read Count and Compare; if Count has reached Compare, or fewer than 1000 ticks remain, return unchanged; if the pull-in is at least the remaining time, write Count + 1000; otherwise compute a1 = Compare - a0 (32-bit), and if a1 is not at least 1000 ticks past Count write Count + 1000, else write a1. Net effect: the new Compare is never closer than 1000 ticks to the Count that was read.

The two releases differ in exactly one instruction word, at routine offset 0x20 (6.5.13f `0xa8000000200057a0`, 6.5.30 `.text+0x18a0`):

| Release | Word | Meaning |
|---|---|---|
| 6.5.30 | `0x01cc4023` | remaining = Compare - Count |
| 6.5.13f | `0x018e4023` | t0 = Count - Compare |

In 6.5.13f, with Count below Compare, that difference is negative as a 64-bit value and so large when compared unsigned. The "fewer than 1000 remain" guard and the "pull-in is at least the remaining time" guard therefore never fire. Only the two later guards survive: they compare a1 with Count (`sltu`) and a1 - Count with 1000, both as 64-bit unsigned values of sign-extended 32-bit results. When a1 is non-negative they clamp correctly. When Compare - a0 wraps below zero, the sign-extended a1 looks like a distant future target and both guards pass, so the wrapped value is written to Compare. Established (read in full, both releases).

## Caller

`trimcompare` has one caller in 6.5.13f: the routine labelled `set_timer_intr` in the symbol table, at `0xa800000020009204`. It gets an absolute deadline in HEART counter units (the HEART counter read at physical `0x0ff20000` is taken on entry at `0xa800000020009048`), rounds it up to a multiple of 120 units, and compares it with the currently programmed deadline held at absolute address `0xffffffffffffc558`. When the new deadline is earlier it calls `trimcompare` with a0 = Count rate x (old deadline - new deadline) / 125, where the rate is the word at gp-0x1218 (inferred to be the Count rate in units of 100 kHz, 975 for 97.5 MHz, because 975/125 equals 97.5/12.5), then stores the new deadline. When the new deadline is later it calls `resetcounter` (`0xa80000002000912c`, `0xa80000002000916c`). The tick handler (`counter_intr`) calls `resetcounter` at `0xa8000000200086b8` and `0xa800000020008150`, with the interval taken from the programmed deadline minus the current HEART value rounded down to a multiple of the tick period (word at gp-0x1214). Established for the call structure; the unit of the rate word is inferred.

## The reported trace, and what explains it

A trace of 6.5.13f on an Octane, taken without a hardware capture and therefore only inferred to match hardware, records a Compare write of `0xfffff0b0` at `0xa8000000200057d4` (the Compare write at the end of `trimcompare`) with Count = `0x10dd`; the preceding `resetcounter` had reset Count to zero 4317 ticks earlier and had written Compare = `0xb1510`.

- The value comes from the pull-in path, not from a stall. `0xfffff0b0` is not Count + 1000, so it is Compare - a0 with the Compare read being `0xb1510` (the most recent Compare write): a0 = `0xb2460` = 730,208 ticks. **Inferred** (the arithmetic is exact; that the Compare read equals the previous write rests on no other write between them, as in the trace).
- a1 = -3920: the requested deadline lies 3920 ticks (about 40 microseconds at 97.5 MHz) before the instant the previous `resetcounter` set Count to zero. In 6.5.30 the same inputs hit the "pull-in at least the remaining time" guard and write Count + 1000. **Established** for the 6.5.30 behaviour (instruction-level read); the 13f outcome is **established** given the inputs above.
- The 4317 ticks are not the exposure. They span the return from `resetcounter`, the rest of the tick handler and the call into `set_timer_intr`. The window inside `trimcompare` from its Count read to its Compare write is under sixteen register instructions, and a1 does not depend on Count at all. **Established.**
- Within `resetcounter`, Count reset to Compare write is eight register instructions with interrupts masked and no memory access. On an R10000 that is a few tens of cycles at most, plus an instruction-cache miss on first use; **inferred** from the instruction count, not measured. The trace of this window reports 434 to 870 Count ticks, far longer than that inference allows; a trace that is not a hardware capture cannot say whether real hardware shows it, so that gap is unknown.

## Why the requested deadline falls before the Count reset (inferred)

`resetcounter` measures Compare from the instant of the Count reset, and subtracts the overshoot (Count - old Compare). The interval it is given is measured from the HEART time rounded down to a tick-period multiple, not from the instant the old Compare matched. These agree only while the old Compare matched at a tick-period boundary or later than the HEART time rounded down. If a period boundary falls between an off-boundary Compare match and the HEART read in the handler, the rounded-down time is later than the real match and the new Compare lands early in Count terms by the difference. A following pull-in to a deadline just ahead of the handler's HEART time then computes a negative a1. A long delay between the interrupt and the handler's HEART read makes this likely; a few microseconds of interrupt latency makes it rare but not impossible. Not traced to a concrete HEART value; **inferred**.

## Comparator rule

The R10000 sets IP[7] when Count equals Compare, and a Compare write clears it (R10000 Microprocessor User's Manual, version 2.0, "Timer Interrupt" in chapter 6, manual page 106; section 14.8 "Count and Compare Registers (9 and 11)", page 244). The manual states equality and gives no rule for a target already behind Count. A target that Count has already passed therefore waits for the 32-bit wrap: 2^32 / 97.5 MHz = 44.05 s (Count 0x10dd to 0xfffff0b0 is 4,294,959,059 ticks). The kernel's "no scheduler clock interrupt" alert at 44 s matches. **Established** for the manual text; that a real R10000 also waits a full wrap is **inferred** from the equality wording (no measurement on hardware).

So the gap does not need a stall the hardware could not have, and does not need a comparator rule different from the R10000's. With the same inputs the same 6.5.13f instruction sequence arms a full wrap on real hardware. What differs is how often the inputs occur.

## Open items

| Question | Class | Test that would settle it |
|---|---|---|
| Whether the 6.5.13f inputs (a0 greater than Compare) occur on real hardware, and how often. | hardware-only | A trace of Count, Compare, the programmed deadline at `0xffffffffffffc558` and the HEART counter at every `trimcompare` entry on a running 6.5.13f Octane; count entries where a0 exceeds Compare. |
| Whether the early-Compare mechanism above is the source in the reported trace. | software-now | In a trace of the same release, read the HEART value stored by the tick handler, the tick period (gp-0x1214), the programmed deadline and the Compare at `0xa800000020005700` return, and check that the Count-domain time of Compare is earlier than the deadline by the observed amount. |
| In which release between 6.5.13f and 6.5.30 the subtraction operand was corrected, and whether 6.5.22 has it. | missing-input | The `IP30boot/kernel.o` of 6.5.22 and of the intermediate maintenance releases: compare the word at `trimcompare+0x20`. |
| Whether the Count rate word (gp-0x1218) is in units of 100 kHz. | software-now | Read the word from a running kernel on a 195 MHz CPU (expect 975) and the code that stores it. |

## The trimcompare operand by release

The word at `trimcompare+0x20` in each IP30 kernel that could be read. `13f` is `018e4023` (`subu t0,Count,Compare`, the operand order that arms the wrap). `fixed` is `01cc4023` (`subu t0,Compare,Count`). The sha256 prefix is of the file named in the row; for linked images it is of the whole `/unix` file.

| release | file | sha256 prefix | word | form |
|---|---|---|---|---|
| 6.5 base (June 1998 media) | `refs/irix-media/6.5-base/usr/cpu/sysgen/IP30boot/kernel.o` | `5d6eba62f3e0` | `018e4023` | 13f |
| 6.5.22 | `root/6.5.22/usr/cpu/sysgen/IP30boot/kernel.o` (a working copy; no disc or manifest recorded alongside it) | `05ed66e3d9d4` | `01cc4023` | fixed |
| 6.5.30 | `refs/irix-media/6.5.30/usr/cpu/sysgen/IP30boot/kernel.o` (same bytes as the kudzu and chips copies) | `32d900592e14` | `01cc4023` | fixed |
| 6.5.30m | `/unix` on a scratch copy of `octane2-vpro.qcow2` (banner 6.5.30m; same bytes as the `octane2-vpro-sdk-recovered-20261002.qcow2` copy) | `3ac07f94bd51` | `01cc4023` | fixed |
| 6.5.30m | `/unix` on a scratch copy of `octane-mgras-base-install-20261003.qcow2` | `c2414ec0a4b0` | `01cc4023` | fixed |
| 6.5.30m | `/unix` on a scratch copy of `root.qcow2` | `1b89ea691fd0` | `01cc4023` | fixed |
| 6.5.21f | `/unix` on a scratch copy of `octane-6521f-flint834-mxi.qcow2` (banner 6.5.21f) | `c5cb45216232` | `01cc4023` | fixed |
| 6.5.23m | `/unix` on a scratch copy of `o2-octane2-v12-irix6523m-base-pristine.qcow2` (banner 6.5.23m) | `6ec53d8c3c66` | `01cc4023` | fixed |
| 6.5.23f | `/unix` on a scratch copy of `o2-octane2-v12-irix6523f-flame9514-ok.qcow2` (banner 6.5.23f) | `9a7f97c0ce63` | `01cc4023` | fixed |

Not held, so not checked: 6.5.13f, 6.5.14 through 6.5.20, 6.5.21 maintenance, 6.5.24 through 6.5.29, the 6.5.29 tarballs listed in the media inventory, and any other maintenance or feature release not in the table. The 6.5.13f Octane disks (rung 5 and rung 7 of the discreet disk set) exist but the disk reader used here refuses their version 1 directories, so their `/unix` was not read. The extracted release trees carry only the IP30 `gfx.o` and `odyssey.a` overlays, with no `kernel.o` or `unix`. The disk search covered only the Octane- and IP30-named images, so an installed IP30 disk on another release may exist unrecorded.
