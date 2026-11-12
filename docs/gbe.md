# O2 GBE: two-wire buses, flat-panel timing words and the dual-channel option

The O2 kernel reaches the GBE block through two-wire buses. This page covers the flat-panel bus, the optional second display channel that the kernel probes at the far end of that bus, and the flat-panel timing words the timing routine writes. The register offsets and constants are in the generated reference [gen/docs/gbe.md](../gen/docs/gbe.md), which is built from `defs/gbe.toml`.

Grades follow [klconfig.md](klconfig.md): "Established" means the whole routine was read in the 6.5.30 object, "Inferred" means a reasoned reading with the reasoning given, and "Unknown" means no shipped software or published source settles it. Offsets are relative to the GBE register block. The block's physical base is not given with a source here, so it is unknown on this page.

## Sources and what was checked

- All routines are in the IP32 boot set of IRIX 6.5.30: `usr/cpu/sysgen/IP32boot/crime.a`, members `crime.o` (the start-up caller), `crime_init.o` (the timing routine) and `crime_i2c.o` (the two-wire and option routines).
- The entry offsets and sizes in the table below come from the 6.5.30 function inventory ([docs/inventory/functions.tsv](inventory/functions.tsv)). The statements in this page were taken from a routine-by-routine reading of these objects. The bytes were not re-read for this page, and the routines were matched to the table by role, so the offsets should be checked before anyone relies on them.
- The 6.5.22 readings in [instruction-readings-kudzu-g1.md](instruction-readings-kudzu-g1.md) list the same entry offsets and the same register stores, which is a second check that the routines match.

| Role | Routine (6.5.30 object) | Entry | Bytes |
|---|---|---|---|
| start-up caller that runs the option probe | `crime_earlyinit` (`crime.o`) | `0x4e64` | 1280 |
| timing routine | `gbeSetTimingRegs` (`crime_init.o`) | `0x498` | 1244 |
| start-up run of the timing routine | `gbeRun` (`crime_init.o`) | `0x1510` | 192 |
| option probe | `i2cfp_DualChannelProbe` (`crime_i2c.o`) | `0x19c0` | 196 |
| option command | `i2cfp_DualChannelCommand` (`crime_i2c.o`) | `0x1a84` | 32 |
| option port read and write | `i2cfp_PortRead` / `i2cfp_PortWrite` (`crime_i2c.o`) | `0x1870` / `0x18f8` | 136 / 128 |
| bus start condition | `i2_start` (`crime_i2c.o`) | `0x1aa4` | 328 |

## Two-wire buses

Established: the bit-banged bus routines take a bus selector. Selector 1 drives the GBE register at offset `0x10`, the flat-panel two-wire bus. Every other selector drives the register at offset `0x08`, the monitor bus.

Established: a start condition writes 0 to the bus register, then polls bit 1 of that register until it reads clear. The poll makes at most 60 delays. It then writes 1 and 3, with a delay after each write. The register's other bits are not described here.

Established: a byte read is a start with the address ORed with 1, one byte received, then a stop. A byte write is a start with the address, one byte sent, then a stop. Each helper returns 1 on success and 0 when the start or the byte transfer fails. It issues the stop either way.

## The option probe

Inferred: the option is a second output channel for flat-panel and dual-head configurations. The kernel talks to it through an 8-bit port on the flat-panel bus. The write address is `0x7e`, the read address is `0x7f`, and the seven-bit address is `0x3f`. The shipped software uses the port only to identify the option and to send a one-byte command. What the port's other pins connect to is unknown.

Established, in order:

1. Read the port into byte A. If the read fails, the probe returns 0 and stops, so no option is fitted. A later read or write that fails is not noticed. A failed later read leaves the stack byte it would have filled unchanged.
2. Write `0xff` and read the port. Keep bits 6 and 5 of the byte as the high two bits of a four-bit id.
3. Write `0x7f` and read the port. Keep bits 6 and 5 as the low two bits. The id is high shifted left 2, ORed with low.
4. Write byte A back with `0x70` ORed in. This is the byte from step 1, not a new read.
5. Store 1 in the kernel word that holds the current bus selector, and return the id plus one.

Established: the word that step 5 sets is the first argument that the monitor EDID reader passes to the bus start routine, and the one the bus reset compares with 1. Inferred from those uses: after a successful probe the selector is the flat-panel bus. The other routines that store to the word were not read for this page.

Established, in `crime_earlyinit`: the return value is stored in a kernel variable here called "installed". It is 0 without the option and the id plus one, from 1 to 16, with it. When installed is non-zero the same routine also sets the number of monitors to 2.

## Option command

Established: when installed is non-zero, the timing routine first sends a command byte to the option port through the command routine. The byte is `0x7f`, or `0xff` when the pixel clock in the video timing record is greater than 100000 kHz. A clock of exactly 100000 sends `0x7f`. This applies to the default set-up and to set-ups 130 and 131 below. It does not apply to set-up 132.

## Flat-panel timing words

Established: the timing routine chooses one of four flat-panel set-ups from the board description byte at offset 46. The values 130 and 131 each select a fixed panel, 132 selects a third, and any other value is the default. The layout of the board description is not described here.

In the table, `W` and `H` are the active width and height from the video timing record. `d` is 2 when installed is below 9 (option ids 0 to 7) and 0 otherwise. Each field is masked to 12 bits before it is shifted or stored. Values for 130 and 131 are fixed and do not depend on the record.

| Set-up | `+0x10028` FP_HDRV | `+0x1002c` FP_VDRV | `+0x10030` FP_DE |
|---|---|---|---|
| default, no option | `0xfff` | `0x1fff` | `0xffffff` |
| default, option fitted | `(W + d) << 12 \| d` | `(H + 1) << 12 \| 1` | not written |
| 130, no option | `1025` | `769 \| 0x1000` | `806 \| 771 << 12` |
| 131, no option | `1281` | `1025 \| 0x1000` | `1065 \| 1024 << 12` |
| 130 or 131, option fitted | as the default, option fitted | as the default, option fitted | not written |
| 132 | `1600` | `1025 \| 0x1000` | not written |

The 132 row is the only one for that set-up, with no split by installed value. These are the values the kernel writes. What the hardware does with them is not established here.

Established: the timing routine writes the packed words with the high field in bits 23 to 12 and the low field in bits 11 to 0. The meaning of each field is unknown.

## Interlace case and the timing flag word

Established, set-up 132 only: the routine rewrites the timing flag word at `+0x10018` as the old value with bits 1 and 7 cleared, ORed with `v`. It looks up one kernel option by name. The name is a string in the routine's read-only data and is not recorded here. If the option exists and its first character is `1`, `v` is 0. If it exists with any other first character, `v` is 2. If the option does not exist, the routine uses a stack word it never stores to, so `v` is undefined.

Established: when installed is below 9, which includes 0 for no option, and the board description byte at offset 45 is non-zero, the routine writes the vertical blank word at `+0x10010` as `H << 12 \| 0xfff` and the horizontal blank word at `+0x10014` as `W << 12 \| 0xfff`. The same byte halves the frame height that the routine writes to the frame-size register. Inferred: byte 45 is the interlace flag, from that use.

Inferred: the horizontal delay `d` differs by option id because two families of the option need different delays. That reasoning explains the `d` test only. The interlace rewrite runs when no option is fitted, so the same reasoning does not explain it.

## Unknown

- What the option port's other pins drive, and what the ids above the two delay classes mean. Unknown: no shipped software or published source depends on them. Routes tried: none recorded for this page.
- Whether the hardware uses the flat-panel words for anything beyond the values written above.
- The meaning of bit 1 of the timing flag word. The bit-level specification for that register was not read for this page.
- The value of `v` when the option lookup fails on set-up 132.
- The physical base of the GBE block. The offsets above are relative to it.
- The offset of the frame-size register that the interlace case halves.
- The purpose of the no-option rewrite of the blank words. Unknown: no shipped software or published source depends on it.
- The layout of the board description, beyond the two byte offsets above.
