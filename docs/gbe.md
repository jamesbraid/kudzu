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

## Display set-up outside the timing words

The sections below cover what the O2 kernel and the O2 X server display module do to the GBE block apart from the flat-panel words: identity and clock select, the stop and blank sequences, the window-ID table, the display-ID tables that describe scan-out bands, the tile lists, colour-map writes, line interrupts, dot-clock coefficients and the hardware cursor.

The facts come from one reader's decompilation of the shipped IRIX 6.5.30 IP32 kernel objects and the X server display module, with the public GBE 1.1 ASIC specification cited as far as the reading gives it. Routines are described by role; no routine name is given. The reading has not had a second check. Register offsets for CTRLSTAT, the VT_INTR words and CMFIFO are not recorded in the reading, so those registers appear here by name only. The dot-clock, vertical position and live readback offsets come from the timing and start-up stores and are in the register table. The offsets above remain relative to the GBE register block.

### Identity, clock select and blanking

Established (probe routine): the kernel accepts a GBE only when the identity word at offset `0x14` reads `0x666`. It then writes CTRLSTAT as `0x30000000`: clock source 3, which is the internal PLL, with every pin pair set to output driving 0. It writes 0 to VT_XY only when that word's bit 31 reads 1. A device with any other identity is left alone.

Established (board-information routine): the kernel rewrites CTRLSTAT as its old value with the clock source kept, plus `0x000aa000`. Bits 13, 15, 17 and 19 set make IO_3 to IO_6 inputs, and their data bits are 12, 14, 16 and 18. The kernel reads those four data bits, with the bit at 12 as bit 0, as a board strap. When the strap reads 0, the kernel selects the alternative gamma table; otherwise it does not. CTRLSTAT bits 3:0 read back the chip revision: 1 is the GBE 1.1 part.

Established (blank and unblank): blanking writes `0x000aa000`, which selects clock source 0, the external TTL input. The board does not drive that input. Unblanking writes `0x300aa000`. Inferred: blanking deselects the clock and does not change any timing register.

Unknown: the meaning of the other strap codes, and what IO_0 to IO_2 and IO_7 to IO_9 drive. Routes tried: the specification's pin table, the stand-alone diagnostics, the kernel, public Linux and NetBSD drivers. No shipped software depends on either.

### Stopping scan-out

The three controls sit at `0x3000c` (frame), `0x20008` (overlay) and `0x40004` (display ID). Their DMA enables are frame bit 0, overlay bit 0 and display-ID bit 16.

Established (stop routine): the kernel saves the three controls and clears their enables. If the saved overlay enable was clear, it stops there. Otherwise it polls the live readback of the three enables, up to 20,000 times at 10 microseconds, until all three read clear. It then writes the frame control back with its enable clear and `0x20` added. It polls the live frame control, up to 20,000 more times, until it equals the value written. The overlay and display-ID controls stay cleared and are not written back.

Inferred: the pointer field of the frame control starts at bit 5, so adding `0x20` moves the pointer by one unit. A changed live value shows that a blanking edge has passed.

Established (early stop, used before the kernel returns to the PROM): the same three enables are cleared, each write followed by a bus flush. Polling is capped at 400,000 reads. Each control gets `0x20` added and the live readbacks are polled until they equal the new values. VT_XY is written `0x80000000`, which freezes it, and bit 20 of DOTCLK is cleared with the other bits kept. The sequence is skipped when the live overlay and frame enables and the display-ID enable are all clear at entry.

### Window-ID table

Established: the window-ID table is at `0x48000`, one 32-bit word per window index, `0x48000 + 4 * index`. An index of 32 or more is rejected. The kernel writes entry 0 with 3 at text-port set-up.

Established: the X module writes one of four tables at start-up, chosen by the chip revision (a revision below 2 takes the first pair, 2 or more the second) and by whether the frame buffer is 16 bits deep. Each entry's value is below. Every entry has buffer code 3.

| Table | Entries (index: value) |
|---|---|
| revision 1, 16-bit (8) | 0 `0x203`, 1 `0x1e3`, 2 `0x1c3`, 3 `0x1a3`, 4 `0x183` (indexed 8-bit, map groups 16 to 12); 5 `0x00b` (RG3B2); 6 `0x007` (indexed 12-bit); 7 `0x013` (RGB5) |
| revision 1, 24-bit (10) | 0 to 7 as above; 8 `0x577` (RGB8, group 11, gamma disabled); 9 `0x017` (RGB8, group 0) |
| revision 2, 16-bit (9) | 0 to 5: indexed 8-bit with groups 18 to 23 (`0x243`, `0x263`, `0x283`, `0x2a3`, `0x2c3`, `0x2e3`); 6 `0x00b`; 7 `0x007`; 8 `0x013` |
| revision 2, 24-bit (11) | 0 to 6 as the revision 2 16-bit table; 7 `0x007`; 8 `0x013`; 9 `0x577` (RGB8, group 11, gamma disabled); 10 `0x017` |

Inferred, from the values and a 32-bit compare mask of `0x7fc` that the X module uses: bits 1:0 are the buffer code, bits 4:2 the pixel type (0 indexed 8-bit, 1 indexed 12-bit, 2 RG3B2, 4 RGB5, 5 RGB8), bits 9:5 the colour-map group and bit 10 the gamma disable. Bit 10 is established from the kernel, which clears it before a write when the alternative gamma table is in use.

Established (retrace handler): an entry with buffer code 0 is skipped by the buffer flip, and entries with code 1 or 2 are exchanged on a swap. Inferred: this is a front-back flip with no change to the frame pointer.

### Display-ID tables as the X module fills them

Established (table builder): the X module keeps up to three 64 KiB display-ID tables. The 32 windows are merged into horizontal bands of lines that cross the same rectangles.

Each band has one frame entry, a 32-bit word `block << 19 | offset << 11 | yend`, where `yend` is the band's last line, counted from 0 at the first active line. Each band also has one line table, with one 16-bit entry per rectangle, `index << 11 | xend`, where `xend` is the rectangle's last column. Entries are sorted by `xend` when a band has two or more rectangles. When the screen is narrower than the display, the list is cut after the entry that reaches the last display column and clamped to it.

Established (layout): the frame table starts at byte 0 and holds up to 2048 entries in blocks 0 to 15, each block being 512 bytes. Line tables start at byte `0x2000` (block 16) and are packed back to back in 16-bit units inside 512-byte blocks. When a table would pass entry 256 of a block, packing restarts at the next block. Block 128 ends the table; on reaching it the X module prints a warning and ends the table early.

Established: the last band covers every line below the screen. Its line entry has `xend` 2047 and display ID 5 (value `0x2fff`) on revision 1, or 6 (`0x37ff`) on revision 2. Its frame entry has `yend` 2047. Those display IDs are the RG3B2 window-ID entries above.

Established: the X module ends each visible line's last entry at the last active column (width minus 1), not at 2047. Inferred: the display stops reading a line table at the end of the active line, so an `xend` at or past the last active pixel ends the line either way. The reading gives no source for the comparison the hardware makes. Unknown: whether the hardware also accepts an `xend` short of the active width when no later entry follows.

### Tile lists

Established (list builder): the kernel keeps one 0x600-byte block per GBE. A tile number is the physical tile address masked to 13 bits (`0x1fff`), so tile bases lie below 512 MiB of bus address.

- Overlay lists (`+0x120` and `+0x160` in the kernel block): contiguous 16-bit tile numbers, two copies, the second starting half way down for interlaced modes.
- Frame-buffer lists (`+0x200` and `+0x300`): `ceil(width / (512 / bytes per pixel))` tile numbers per row, one row per 128-line tile row, with the second list starting half way for interlaced modes.
- `+0x0a0`: 64 entries, each one tile number in the high halfword of a word.
- `+0x400` and `+0x500` are not filled by the two routines read. Unknown: whether another path fills them.

Inferred: which list serves which plane, from the sizes and sources. Established: the list loader sets bit 0 of each of the seven pointer words before the words are used as the frame, overlay and display-ID control values. Inferred: bit 0 is the DMA enable of those controls. Unknown: the meaning of tile-number bit 15. The kernel masks it, and no shipped software sets it.

### Colour-map writes and the FIFO

Established (X module): a colour is one 32-bit store to `0x50000 + 4 * index`, the word `R << 24 | G << 16 | B << 8`. The index is the global entry number, so a window's group base is part of it. The first entry of a group is the group value times 256. Revision 1 group lists start at `0x1000`, `0xf00`, `0xe00`, `0xd00`, `0xc00`, `0xb00` and `0x1100`, which are groups 16 to 12, 11 and 17. Revision 2 lists start at `0x1200` to `0x1700` (groups 18 to 23) and at `0xb00` and `0x1100`.

Established: after 33 writes the X module polls CMFIFO (bits 5:0) until it is at least 48 on revision 2, or 0 or at least 48 on revision 1. It also polls at the start of a list. The stand-alone diagnostics wait for room until the field reads at least 17 on revision 2 (0 or at least 17 on revision 1), and wait for an empty queue until it reads 63 on revision 2 and 0 on revision 1.

Inferred: both revisions count free entries. Revision 2 queues at most 63, so 0 means full and an empty queue reads 63. Revision 1 queues 64, wraps to 0 when empty and also reads 0 when full, which is why its room test accepts 0. Thirty-three writes after a count of at least 48 free cannot overrun 64 entries, by arithmetic.

Established: the X module skips the hardware write of entry `0x1102` (group `0x11`, index 2) while one of its screen flags is non-zero, and keeps only its software copy. The text port draws colour `c` as pixel value `c + 1` and loads it at entry `0x1100 + c + 1`, so entry `0x1102` is the text port's colour 1. Inferred: the skip protects that colour. What sets the flag was not traced.

Unknown: whether the hardware drains writes in strict order. No shipped software reads a colour-map entry back.

### Line interrupts

Established (mode set-up): for a mode with vertical blank start `B` and total lines `T`, the kernel computes line 1 as `B - 32` and line 0 as `B + 4`, reduced by `T` when it reaches `T`. It writes `VT_INTR01 = line1 | line0 << 12`, rewrites it from the interrupt handler on every frame, and parks VT_INTR23 and both lines of an unused display at `0xfff` or `0xffffff`. Inferred: each line number is 12 bits, bits 11:0 for line 1 and 23:12 for line 0, from the shift.

Established: the handler clears CRIME interrupt bit 16 first and then rewrites the register. CRIME bit `16 + n` is GBE line `n`, so lines 2 and 3 are CRIME inputs 18 and 19. The reading found scheduler code that uses lines 2 and 3, but no shipped graphics object calls it, so a stock session leaves both lines parked at `0xfff`.

Unknown: whether a line interrupt fires when its line is written equal to the line the beam is on, or when it is written after the beam has passed it. No shipped caller does either.

### Dot-clock coefficients

Established (dot-clock write): the kernel clears DOTCLK bit 20, writes the new value with bit 20 set, and waits 11 milliseconds. Inferred: bit 20 runs the PLL or its output. The specification leaves it undefined.

Established (format files): the 26 shipped video format files each name three PLL scalars, M, N and P, in their schema. The values below were read at the offsets the schema gives, as big-endian words. The two-channel and `p` variants repeat their base format's values. Inferred: the output is `20 MHz * M / (N * 2^P)`. The kernel states no reference clock. The 20 MHz reference is taken from a public Linux driver in the reading, which was not checked here. The output and difference columns below were recomputed from M, N and P.

| File | M | N | P | VCO MHz | Output MHz | File clock MHz | Difference |
|---|---:|---:|---:|---:|---:|---:|---:|
| 640x480_60 | 5 | 1 | 2 | 100.000 | 25.000 | 25.175 | -0.70 % |
| 800x600_60 | 8 | 1 | 2 | 160.000 | 40.000 | 40.000 | 0.00 % |
| 800x600_72 | 5 | 1 | 1 | 100.000 | 50.000 | 50.000 | 0.00 % |
| 1024x768_60 | 13 | 2 | 1 | 130.000 | 65.000 | 65.000 | 0.00 % |
| 1024x768_70 | 15 | 2 | 1 | 150.000 | 75.000 | 75.000 | 0.00 % |
| 1024x768_75 | 39 | 5 | 1 | 156.000 | 78.000 | 78.750 | -0.95 % |
| 1024x768_96s | 31 | 3 | 1 | 206.667 | 103.333 | 103.503 | -0.16 % |
| 1280x1024_48 | 30 | 7 | 0 | 85.714 | 85.714 | 85.882 | -0.20 % |
| 1280x1024_50 | 9 | 1 | 1 | 180.000 | 90.000 | 89.460 | +0.60 % |
| 1280x1024_60 | 16 | 3 | 0 | 106.667 | 106.667 | 107.245 | -0.54 % |
| 1280x1024_72 | 13 | 2 | 0 | 130.000 | 130.000 | 129.468 | +0.41 % |
| 1280x1024_75 | 27 | 4 | 0 | 135.000 | 135.000 | 135.001 | 0.00 % |
| 1280x492_120s | 16 | 3 | 0 | 106.667 | 106.667 | 107.144 | -0.45 % |
| 1600x1024_48p | 34 | 7 | 0 | 97.143 | 97.143 | 97.219 | -0.08 % |
| 1600x1024_50p | 5 | 1 | 0 | 100.000 | 100.000 | 101.270 | -1.25 % |
| 1600x1024_60p | 38 | 7 | 0 | 108.571 | 108.571 | 108.732 | -0.15 % |

Established (kernel built-in table): 13 populated records carry the same M, N and P as their files, with two exceptions. The stereo record (1280 by 492) carries 27, 5, 0 where its file has 16, 3, 0. The 1600 by 1024 record carries 5, 1, 0, the values of the 50 Hz file, and no record carries the 48 or 60 Hz values. The pixel clock fields, in slot order and in kHz, are 25,175; 40,000; 50,000; 65,000; 75,000; 78,750; 85,882; 89,544; 107,245; 129,468; 135,000; 107,072 for the stereo record; and 101,270.

Unknown: why the stereo record's PLL values differ from its file, and why its clock is 107,072 kHz rather than the 108,000 kHz those values give.

Established: the shipped formats use small integer coefficients, and the highest shipped format is 135 MHz. Shipped VCO frequencies run from 85.7 to 206.7 MHz. Unknown: whether the hardware locks above 140 MHz, the ceiling the reading attributes to the GBE 1.1 specification (not checked here). The reading also gives the specification's worked example for 25.175 MHz as M 146, N 29, P 4. That example does not fit the output formula above (it gives about 6.29 MHz), so either the specification uses a different formula or the example belongs to another reference. This is unresolved.

### Start-up order and frame-size words

Established (timing routine): the kernel keeps bits 15:0 of the dot-clock word, which clears the run bit, writes the timing words, then writes the dot-clock word with the run bit set and waits 11 milliseconds. The timing words are written before the dot-clock value.

Established (start-up sequence, in the order the routine runs):

1. Clear bit 16 of the word at physical `0x14000028`. The reading does not identify that word or confirm that bit 16 is the GBE line interrupt.
2. Park VT_13 and VT_14 holding `0xffffff`.
3. Write the frame totals, VT_1.
4. Pulse bit 15 of the frame-size word and bit 13 of the overlay-width word. Each pulse resets that FIFO. The reading gives neither word's offset.
5. Write the frame, overlay and display-ID controls at `0x3000c`, `0x20008` and `0x40004`.
6. Poll bit 0 of the live readbacks at `0x30008` and `0x20004` until each equals bit 0 of the control just written.
7. Compare the vertical position word, VT_XY, whole and unmasked, with the vertical blank start line. Wait until it is below that line, then until it is above it.
8. Write VT_13 and VT_14 with their final values.

Established (frame size): the frame-size word takes the width in whole tiles at bits 12:5 and the remainder in 32-byte units at bits 4:0. Its depth code sits at bits 14:13: 1 for 16-bit pixels, 2 otherwise. A tile is 256 pixels at 16 bits and 128 at 32 bits. The frame-size pixel word is the height shifted left 16 bits, halved for an interlaced record. The overlay-width word uses 512-pixel tiles for the 8-bit overlay.

Established (frame-lock flags): the low field of VT_7 takes the blank start line when the record's frame-lock flag bits (mask `0x12`) are set, and `0xfff` otherwise. The reading shows only that the choice tests those bits. What the bits do in the hardware is unknown.

Established (VT_17 to VT_19): the vertical and horizontal parts are built as the register table describes. Where each part sits in its word is inferred from the `0x1000` step that VT_18 adds for one line.

### Conflicting CRT timing readings

A second reading of the CRT-format register stores disagrees with three entries above. Neither is resolved here, and the grades above are unchanged.

- The word at `0x10010` (vt_vblank). The CRT reading stores the vertical blank end in the low field, where that entry says the low field is `0xfff` in the interlace case. For the 640 by 480 record the high field agrees at 480.
- The word at `0x10014` (vt_hblank). The CRT reading stores `(hblank end - 3)` in the low field and `(hblank start + total pixels - 5) mod total pixels` in the high field. For the 640 by 480 record that high field is 635, where that entry records the active width, 640.
- The word at `0x10018` (timing_flags). The CRT reading writes 0. That entry says the routine reads the word, clears bits 1 and 7, and ORs in 0 or 2.

The 640 by 480 example has no dual-channel option, and the entries above say the vt_vblank and vt_hblank stores run when no option is fitted. So the two readings disagree on the same branch as far as either shows. Settling it needs the branch conditions of the timing routine read again for a CRT record with no option, and then an independent review of this page.

### Hardware cursor

Established (text-port initialiser): before the screen is blanked the kernel loads the hardware cursor. Colour 1 at `0x70008` is `0xffffff00`, colour 2 at `0x7000c` is `0xff000000`, and the position word at `0x70000` is 0. Glyph words 0 to 31 at `0x78000` to `0x7807c` come from a 32-word table, and words 32 to 63 at `0x78080` to `0x780fc` are 0. The control word at `0x70004` is then 1. Colours use the byte layout red, green, blue, zero, as in the colour map.

Inferred: the 64 words form a 32 by 32 cursor at two bits per pixel, and the text port draws with the first 32 words only.

## Revision 1.1 specification

The GBE ASIC specification for revision 1.1 (18 July 1996) describes revision 1.1 only. Every fact here is from that document and is graded inferred, because a published document is a hint, not a shipped binary. Where the kernel's use agrees with it, the agreement is noted but does not change the grade.

- **Reset values** (inferred): CTRLSTAT chip ID 1, every pin pair an input, clock select 0. DOTCLK 0, the two-wire lines released, DEVICE_ID `0x666`, VT_XY freeze set, and the other timing, plane, display-ID and cursor registers 0. CMFIFO reads 0. The kernel unfreezes VT_XY only when the freeze bit reads set, which matches.
- **Shadow latching** (inferred): each control register is copied into its live twin at the start of blanking. The tile-list pointer is in bits 31:5, so the kernel's stop routine adds `0x20` to see a changed pointer arrive, as the stop section above describes. The display-ID control holds the upper address bits of a 64 KiB table, and bit 16 enables it. It does not hold a list pointer. See `table_hi` in the reference.
- **Tile lists** (inferred): 16-bit entries of 64 KiB tile numbers, read whole into on-chip RAM at vertical blanking. A rewrite during active video takes effect at the next frame. Requests are 512-byte aligned, do not cross an 8 KiB page, and up to eight may be outstanding. The constants are in the reference.
- **Overlay and cursor compositing** (inferred): an opaque cursor pixel wins, then a non-zero overlay pixel, then the normal planes. An overlay value of 0 is transparent. The overlay always indexes the last 256 colour-map entries and always passes gamma. No register carries a colour key, and the hardware has no underlay. This fits the kernel's use: the only overlay code changes a drawable's rendering state, and no shipped code writes a transparent value or an overlay-to-window map.
- **Window-ID entry** (inferred): buffer code 1 is the lower buffer, 2 the upper and 3 both. A change between buffer codes is a buffer swap with no frame-pointer change. The auxiliary code is in bits 12:11 and the colour-map group in bits 9:5. The kernel's compare mask of `0x7fc` leaves out bits 1:0 and 12:11, which fits the buffer and auxiliary codes being excluded from the type comparison.
- **Colour map** (inferred): 4608 entries in revision 1.1, against 6144 in the revision 2 tables the X module uses and in the public Linux driver. The larger map belongs to a later revision.
- **FIFO count** (inferred): the specification counts free entries at both revisions. That matches the reading above, and the public Linux driver's occupancy reading is the outlier.
- **Counter period** (inferred): the counters reset when they equal their maxima, so a stored value N gives a period of N + 1. The kernel writes the exact totals, which would make the programmed frame one clock and one line longer than its format. This points to the totals minus one, but needs a capture to settle. See the note on `vt_1` in the reference.
- **Display-ID runs** (inferred): a line table is a list of 16-bit entries with `xend` in bits 10:0 and the display ID in bits 15:11. The specification asks for the last run to end at 2047, while the X module ends visible lines at the last active column. The reading gives no source for the hardware comparison. See `DID_LINE` in the reference.

## Unknown

- What the option port's other pins drive, and what the ids above the two delay classes mean. Unknown: no shipped software or published source depends on them. Routes tried: none recorded for this page.
- Whether the hardware uses the flat-panel words for anything beyond the values written above.
- The meaning of bit 1 of the timing flag word. The bit-level specification for that register was not read for this page.
- The value of `v` when the option lookup fails on set-up 132.
- The physical base of the GBE block. The offsets above are relative to it.
- The offsets of the frame-size word, the frame-size pixel word and the overlay-width word. The interlace case halves the frame-size pixel word, and its offset is among these.
- Whether VT_1 takes the totals or the totals minus one. The kernel writes the totals, and the reading gives no further evidence. The revision 1.1 section gives a reason to expect minus one, which is inferred and needs a capture.
- The gamma write path. The source reading gives a linear ramp, where entry i is `i * 0x01010100`, written to offset `0x60000`, or a fixed table. It gives no grade for this, so it is recorded as unknown until the routine is checked.
- Whether the unmasked vertical position comparison in the start-up sequence is the intended line test, or works only because the line counter is zero early in the frame.
- Which of the two CRT readings in the conflict section is right for a record with no dual-channel option.
- The purpose of the no-option rewrite of the blank words. Unknown: no shipped software or published source depends on it.
- The layout of the board description, beyond the two byte offsets above.
- Register offsets of CTRLSTAT, the VT_INTR words and CMFIFO. The reading gives their use but not their offsets.
- The layout of CTRLSTAT and the IO pin pairs beyond IO_3 to IO_6, and the meaning of the strap codes other than 0.
- The reset values of revision 2. No shipped software depends on them, and no source gives them; the revision 1 values are the only ones with a source.
- Tile-number bit 15, and the contents of the kernel block at `+0x400` and `+0x500`.
- The `xend` comparison, the firing rule for line interrupts, and the write ordering of the colour map (see the sections above).
- Whether the hardware locks above 140 MHz, and the cause of the 25.175 MHz example's mismatch with the output formula.
