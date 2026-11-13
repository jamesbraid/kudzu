# O2 MACE video: input and output channels, GBE capture, two-wire bus and timer snapshots

The O2's MACE peripheral chip carries the video input and output channels, and a two-wire bus that the video driver uses to program video-port chips. The IRIX video driver also keeps a timer snapshot in the MACE timer block and loads a capture block on the GBE. This page records what the driver reaches on those blocks and what the reading could say about each register.

The register offsets, widths and fields that have a stated width are in the crime repository's reference pages for the `mace_video_in` (input channels), `mace_video` (output channel and window map), `mace_i2c` (two-wire bus), `mace_timer` (timestamp snapshots) and `gbe` (capture block) units; what the driver does with them is listed in [kernel register use](../kernel/hardware-register-use.md#o2-mace-video-and-two-wire-bus). Offsets with no stated width are in the tables below.

The reading this page draws on was marked proposed and pending independent evidence review by its author. It has not had that review. Its facts are carried here at the grades below, and nothing on this page clears them for implementation.

A later verification pass compared the page with the IRIX 6.5.30 IP32 video driver object (`usr/cpu/sysgen/IP32boot/mvp.o`), the kernel object (`kernel.o`) and the MACE 2.0 and GBE 1.1 specifications. Where that pass confirmed or corrected a fact, the text says so and gives the address or the PDF page.

Grades follow [gbe.md](gbe.md). **Established** means the routine or table was read in full in a shipped binary, with an address. **Inferred** means a reasoned reading, with the reasoning given. **Unknown** means no shipped software or published source settles it. Facts that come only from the MACE 2.0 specification are graded inferred, as in [crime.md](crime.md), because a published document is a hint and not evidence.

## Sources

- The shipped IRIX 6.5.30 IP32 video driver object, `mvp.o` (about 168 KB). The reading was made from its instructions and data only. It records text offsets inside that object as locators, and describes routines by role.
- The MACE 2.0 specification (dated July 1996). The reading cites page numbers (36 and 37 for status, 149 to 151 for the two-wire bus, 181 for the peripheral sub-block map). The verification pass checked them: the numbers are PDF page numbers, and each page says what the reading attributes to it. The channel register map is on PDF page 33, the control register on page 34, the output hardware configuration on pages 53 and 54, the field size on page 45, and the timer snapshot registers on page 148.
- The GBE 1.1 ASIC specification (18 July 1996), section 3.2.8 (PDF pages 47 and 48), for the capture block.

## Window map

Every block except the timer is reached through an uncached pointer that the driver stores in its controller record. The window starts at physical `0x1f100000`. Checked: `mvpreg_init` (`mvp.o` text offset `0xbdb0`) stores `0xbf100000`, `0xbf180000`, `0xbf200000`, `0xb6080000` and `0xbf330000` at record offsets `0xc0` to `0xd0`. The MACE 2.0 primary address map (PDF page 181) puts video input 1, video input 2 and video output at `0x1f100000`, `0x1f180000` and `0x1f200000`, and its peripheral sub-map puts the two-wire bus at `0x1f330000`.

| Block | Location | Uncached pointer | Grade |
|---|---|---|---|
| Video window | physical `0x1f100000` | `0xbf100000` (input channel 1) | established |
| Input channel 1 | window offset `0x00000` | `0xbf100000` | established |
| Input channel 2 | window offset `0x80000` | `0xbf180000` | established |
| Output channel | window offset `0x100000` | `0xbf200000` | established |
| GBE capture block | GBE register block offset `0x80000` | `0xb6080000` | established |
| Two-wire bus | physical `0x1f330000` | `0xbf330000` | established |
| Timer block | physical `0x1f340000` | `0xbf340000` | established |

The timer block is the MACE count and compare block. Its base is the first word of the driver's initialised data (`mvp.o`, `.data` offset 0), not a controller-record field, and the kernel's own time routine (`update_ust` in `kernel.o`, offset `0x4250`) reads the same address. The MACE 2.0 peripheral sub-map puts the count and compare timers at sub-block 4, offset `0x40000` from `0x1f300000` (PDF page 181), which agrees. The grade of the placement stays inferred, since it rests on the specification.

The register offsets for the channels are from the start of each channel. Input channel 1 and the video window share a base, so their offsets are the same.

## Channel registers

Input channels and the output channel share the first five words. Their offsets are from the start of the channel. The grades are inferred, because the routines that write them were read in a workspace and not re-read for the reading.

- **Control, `+0x00`, 64 bits.** The low nine bits hold one DMA enable and eight interrupt enables. The driver writes the whole word from a copy of its last value, changing only those nine bits (checked: `mvptransfer_enablechannel`, text offset `0x17d8`, and `mvptransfer_stop_dma`, `0x87c`, both mask with `0x1ff`). Bit 0 is the DMA enable as the output reading states it. The specification (PDF page 34) lists the enables. For an input channel: bit 1 vertical sync, 2 DMA complete, 3 lost sync, 4 buffer field overflow, 5 horizontal overflow, 6 vertical overflow, 7 FIFO overflow, 8 CRIME memory error, 9 GPIB. For the output channel: bit 1 vertical sync, 2 DMA complete, 3 genlock lost, 4 buffer field overflow, 7 FIFO underflow, 8 CRIME memory error, and bits 5 and 6 unused. The specification gives the control register as ten bits wide for an input channel, so the driver's nine-bit mask leaves bit 9 (GPIB) alone.
- **Status, `+0x08`, 64 bits, read-only.** Reading it clears the pending interrupt causes, except bit 0 (DMA active) and, on an input channel, bit 10 (sync present), which reflect live state (MACE 2.0 specification, PDF page 36).
- **Configuration, `+0x10`, 64 bits.** Written at start-up. The driver's layout is not recorded. The specification lists the input layout on PDF page 37 (channel reset, D1 reset, source, precision, error correction, start-of-field count, pixel format, dither, memory mode, interleave) and an output layout on the following pages. Inferred, from the specification alone.
- **Next descriptor, `+0x18`, 64 bits.** A zero-extended 32-bit physical address of a descriptor block. The driver ORs a flag byte into the word. The specification (PDF page 33) puts the pointer in bits 31:6 and a flag field in bits 2:0, which fits a flag byte ORed into the low bits. Inferred.
- **Field offset, `+0x20`, 64 bits.** The driver writes the low halfword of a transfer-record word here.

Established, from the start-up routine: the input channel's clip, line-width and alpha words are 64-bit stores at these offsets. Input channel layout:

| Offset | Word | Layout |
|---:|---|---|
| `+0x28` | line width | not recorded |
| `+0x30` | horizontal clip, odd field | not recorded |
| `+0x38` | vertical clip, odd field | not recorded |
| `+0x40` | alpha, odd field | not recorded |
| `+0x48` | horizontal clip, even field | not recorded |
| `+0x50` | vertical clip, even field | not recorded |
| `+0x58` | alpha, even field | not recorded |

The output channel's words from `+0x28` onward are different:

| Offset | Word | Layout |
|---:|---|---|
| `+0x28` | field size | inferred: bits 21:12 are the line count and bits 11:0 the bytes per line |
| `+0x30` | horizontal padding, odd field | 64-bit word, layout not recorded |
| `+0x38` | vertical padding, odd field | 64-bit word, layout not recorded |
| `+0x40` | horizontal padding, even field | 64-bit word, layout not recorded |
| `+0x48` | vertical padding, even field | 64-bit word, layout not recorded |
| `+0x50` | genlock delay | 64-bit word, layout not recorded |
| `+0x58` | hardware configuration | see below |

The field-size split is read from one stored value that the reading gives as 243 lines of 2880 bytes. The reading names both parts of that value, so the split is inferred. The specification (PDF page 45) agrees: bits 21:12 are the number of lines and bits 11:3 the line width in bytes, a multiple of 8. A value of 2880 has its low three bits clear, so it fits.

### Output hardware configuration, `+0x58`

Inferred from the bit positions the reading gives, which follow the layout in the MACE 2.0 specification:

- Bit 0, Port AB reset, and bit 2, Port CD reset. They are released whenever an input or output is set up.
- Bit 1, Port AB select, and bit 3, Port CD select.
- Bit 4, audio sync reset, bit 7, audio sync D1 precision and bit 8, audio sync D1 error correction (named by the specification on PDF page 53 and not seen in any software read here).
- Bits 6:5, audio sync source.
- Bit 9, GBE framelock reset. Bits 11:10, framelock source. Bit 12, framelock precision. Bit 13, framelock correction.
- Bits 17:14, start-of-field count for the odd field. Bits 22:19, start-of-field count for the even field.
- Bit 18, general-purpose output.
- Bits 35:32, the MACE revision code (PDF page 54), which the video probe reads. The value that identifies the part is not in the reading.

Bits 23 to 31 are reserved in the specification.

## Two-wire bus

The two-wire bus is the MACE peripheral's bus for video-port chips. The audio driver reaches the same bus. All accesses are 64-bit.

- **Configuration, `+0x00`.** Established: the configuration helper reads the word and writes it back with its low four bits replaced under a mask (checked: `mvpi2c_config`, text offset `0x856c`, tests mask bits 1, 2, 4 and 8 and replaces the matching bit of the word). The reset routine writes 1 to request a bus reset. The reading does not assign meanings to the other values it writes (0 and 2), beyond their use in the rate set-up. The specification (PDF page 151) gives bit 0 as bus reset, bit 1 as fast mode (400 kHz, against 100 kHz by default), bit 2 as a data-pin override and bit 3 as a clock-pin override. Inferred: 0 selects the standard rate and 2 the fast rate.
- **Control and status, `+0x10`.** Established, from the polling loops and the force-idle routine: bit 0 means not idle, bit 4 transfer busy, bit 5 acknowledge not received and bit 7 bus error. The specification's register table (PDF page 150) gives the same four meanings, and adds bit 1 as the bus direction (1 reads) and bit 2 as the last-byte control (1 holds the bus for more bytes). The force-idle routine stores its argument with bit 0 cleared when the bus was not idle.
- **Data, `+0x18`.** Established: the read and write routines store the chip address byte and the register number byte through it, and read it back after the transfer.

Inferred: the values written to the control word are commands. The reading gives 1 as a write of a last byte, 3 as the start of a read and 5 as a write with more bytes to follow. This comes from the register description, with bit 1 the direction and bit 2 the last-byte control. The specification's table (PDF page 150) agrees: bit 1 is the direction and bit 2 holds the bus when set, so 1 is a last-byte write, 3 a last-byte read and 5 a write with more bytes to follow.

Unknown: no routine tests bit 7 except the force-idle path, so what the bus does on an error beyond that path is not settled by the reading.

## Timer snapshots

The timer block holds three 64-bit snapshot words for the video channels, at its base plus `0x38`, `0x40` and `0x48`. Established: the driver's timestamp routine reads them in that order for input channel 1, input channel 2 and the output channel, which the reading numbers 0 to 2 (checked: the routine at `mvp.o` text offset `0x1884` reads the 64-bit word at the data-table pointer plus `0x38`, indexed by channel number times 8). The routine widens the high word with a wrap counter kept in software. That counter is driver code, not hardware.

Inferred, from the MACE 2.0 specification (PDF pages 56 and 148): each 64-bit word is a 32-bit time-stamp snapshot in bits 63:32 and a 32-bit count in bits 31:0, with the field ID in bit 0 and the frame count in bits 31:1. The specification lists the three pairs as separate 32-bit registers, UST at `0x38`, `0x40` and `0x48` and MSC at `0x3c`, `0x44` and `0x4c`. The driver's wrap detection compares the high word, which fits the high word being the time stamp. Unknown: what the caller of the timestamp routine does with the low word.

## GBE capture block

The capture block sits at GBE register block offset `0x80000`. It holds nine words that the reading names `VC_0` to `VC_8`. The reading gives the first and last word offsets, `0x80000` and `0x80020`, so the 4-byte spacing between them is inferred.

- Established, from the capture-type case of the start-up routine: `VC_0` to `VC_3` are written as zero when a capture path of that type stops.
- Inferred: `VC_2` bit 0 is the DMA enable. The start routine sets it once, after `VC_3` is written. The GBE 1.1 specification (section 3.2.8) agrees: VC_2 bit 0 is `vc_dmavideo`, with bit 1 the flicker filter, bit 2 automatic field advance, bit 3 full-screen mode and bit 4 a colour-space bypass.
- Inferred: `VC_3` holds a path byte. The byte supplies the field-select and descriptor-list enable bits, and the start routine combines it with the physical address of a descriptor block when the driver's descriptor word is zero. The specification puts the field select in bit 0, the descriptor-list DMA enable in bit 1 and the descriptor pointer in bits 31:5.
- Inferred: `VC_4` to `VC_8` hold five words that the start routine copies from a descriptor block when the driver has a descriptor word set. The specification names them as ten 16-bit tile addresses, two per word, and notes that the descriptor-list DMA enable does not work on revision 1.1, so software must write the list by hand. That is why a driver copies them.
- Inferred: `VC_0` and `VC_1` are the capture rectangle (left and right edge, top and bottom edge, 12 bits each). The driver copies them, with the `VC_2` bits other than bit 0, from a control record of the capture path (`mvpreg_syncpath_controls`, text offset `0xbe74`).

The word width of the block is 32 bits and the nine words sit at offsets `0x80000` to `0x80020` in steps of 4, which the specification's register list confirms. Established: `VC_0` to `VC_3` are zero-stored as 32-bit words when a capture path stops (checked: `mvptransfer_stop_dma`, text offset `0x87c`).

## Unknown

- Whether the silicon behaves as the specification's interrupt-enable list says. No software read here sets any of those causes.
- The layouts of the clip, alpha, padding, genlock-delay and configuration words, on both channels, as the driver programs them. The specification gives layouts that this page does not copy.
- What a two-wire bus error does beyond the force-idle path.
- What the caller of the timestamp routine does with the low word of a snapshot.
- Whether the silicon accepts an output configuration write while DMA is enabled. The reading shows the driver writes that word only at start-up.
- The odd framelock start-of-field count. The reading says a small per-source routine chooses it, and that routine was not recorded.
- Whether the genlock delay must be nonzero when a genlock source is selected. The driver does not test that.
