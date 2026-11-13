# O2 MACE video: input and output channels, GBE capture, two-wire bus and timer snapshots

The O2's MACE peripheral chip carries the video input and output channels, and a two-wire bus that the video driver uses to program video-port chips. The IRIX video driver also keeps a timer snapshot in the MACE timer block and loads a capture block on the GBE. This page records what the driver reaches on those blocks and what the reading could say about each register.

The register offsets, widths and fields that have a stated width are in the crime repository's reference pages for the `mace_video_in` (input channels), `mace_video` (output channel and window map), `mace_i2c` (two-wire bus), `mace_timer` (timestamp snapshots) and `gbe` (capture block) units; what the driver does with them is listed in [kernel register use](kernel/hardware-register-use.md#o2-mace-video-and-two-wire-bus). Offsets with no stated width are in the tables below.

The reading this page draws on was marked proposed and pending independent evidence review by its author. It has not had that review. Its facts are carried here at the grades below, and nothing on this page clears them for implementation.

Grades follow [gbe.md](gbe.md). **Established** means the routine or table was read in full in a shipped binary, with an address. **Inferred** means a reasoned reading, with the reasoning given. **Unknown** means no shipped software or published source settles it. Facts that come only from the MACE 2.0 specification are graded inferred, as in [crime.md](crime.md), because a published document is a hint and not evidence.

## Sources

- The shipped IRIX 6.5.30 IP32 video driver object. The reading was made from its instructions and data only. It records text offsets inside that object as locators, and describes routines by role.
- The MACE 2.0 specification. The reading cites page numbers (36 and 37 for status, 149 to 151 for the two-wire bus, 181 for the peripheral sub-block map). They were not checked against the document.

## Window map

Every block is reached through an uncached pointer that the driver stores in its controller record. The window starts at physical `0x1f100000`.

| Block | Location | Uncached pointer | Grade |
|---|---|---|---|
| Video window | physical `0x1f100000` | `0xbf100000` (input channel 1) | established |
| Input channel 1 | window offset `0x00000` | `0xbf100000` | established |
| Input channel 2 | window offset `0x80000` | `0xbf180000` | established |
| Output channel | window offset `0x100000` | `0xbf200000` | established |
| GBE capture block | GBE register block offset `0x80000` | `0xb6080000` | established |
| Two-wire bus | physical `0x1f330000` | `0xbf330000` | established |
| Timer block | physical `0x1f340000` | `0xbf340000` | established |

The timer block is the MACE count and compare block. The reading places it at sub-block 4 of the MACE peripheral map, which is a specification citation and is inferred.

The register offsets for the channels are from the start of each channel. Input channel 1 and the video window share a base, so their offsets are the same.

## Channel registers

Input channels and the output channel share the first five words. Their offsets are from the start of the channel. The grades are inferred, because the routines that write them were read in a workspace and not re-read for the reading.

- **Control, `+0x00`, 64 bits.** The low nine bits hold one DMA enable and eight interrupt enables. The driver writes the whole word from a copy of its last value, changing only those nine bits. Bit 0 is the DMA enable as the output reading states it. Which cause each interrupt enable covers is not stated.
- **Status, `+0x08`, 64 bits, read-only.** Reading it clears the pending interrupt causes (MACE 2.0 specification, pages 36 and 37, as cited).
- **Configuration, `+0x10`, 64 bits.** Written at start-up. Its layout is not recorded.
- **Next descriptor, `+0x18`, 64 bits.** A zero-extended 32-bit physical address of a descriptor block. The driver ORs a flag byte into the word. Which bits carry the flag are not stated.
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

The field-size split is read from one stored value that the reading gives as 243 lines of 2880 bytes. The reading names both parts of that value, so the split is inferred.

### Output hardware configuration, `+0x58`

Inferred from the bit positions the reading gives, which follow the layout in the MACE 2.0 specification:

- Bit 0, Port AB reset, and bit 2, Port CD reset. They are released whenever an input or output is set up.
- Bit 1, Port AB select, and bit 3, Port CD select.
- Bits 6:5, audio sync source.
- Bit 9, GBE framelock reset. Bits 11:10, framelock source. Bit 12, framelock precision. Bit 13, framelock correction.
- Bits 17:14, start-of-field count for the odd field. Bits 22:19, start-of-field count for the even field.
- Bit 18, general-purpose output.
- Bits 35:32, identity read by the video probe. The value that identifies the part is not in the reading.

The reading does not assign bits 4, 7 and 8, or the audio-sync reset bit. Those are unknown.

## Two-wire bus

The two-wire bus is the MACE peripheral's bus for video-port chips. The audio driver reaches the same bus. All accesses are 64-bit.

- **Configuration, `+0x00`.** Established: the configuration helper reads the word and writes it back with its low four bits replaced under a mask. The reset routine writes 1 to request a bus reset. The reading does not assign meanings to the other values it writes (0 and 2), beyond their use in the rate set-up.
- **Control and status, `+0x10`.** Established, from the polling loops and the force-idle routine: bit 0 means not idle, bit 4 transfer busy, bit 5 acknowledge not received and bit 7 bus error. The force-idle routine stores its argument with bit 0 cleared when the bus was not idle.
- **Data, `+0x18`.** Established: the read and write routines store the chip address byte and the register number byte through it, and read it back after the transfer.

Inferred: the values written to the control word are commands. The reading gives 1 as a write of a last byte, 3 as the start of a read and 5 as a write with more bytes to follow. This comes from the register description, with bit 1 the direction and bit 2 the last-byte control.

Unknown: no routine tests bit 7 except the force-idle path, so what the bus does on an error beyond that path is not settled by the reading.

## Timer snapshots

The timer block holds three 64-bit snapshot words for the video channels, at its base plus `0x38`, `0x40` and `0x48`. Established: the driver's timestamp routine reads them in that order for input channel 1, input channel 2 and the output channel, which the reading numbers 0 to 2. The routine widens the high word with a wrap counter kept in software. That counter is driver code, not hardware.

Unknown: which word of each snapshot the caller treats as the time stamp and which as the field count. The reading did not read the caller.

## GBE capture block

The capture block sits at GBE register block offset `0x80000`. It holds nine words that the reading names `VC_0` to `VC_8`. The reading gives the first and last word offsets, `0x80000` and `0x80020`, so the 4-byte spacing between them is inferred.

- Established, from the capture-type case of the start-up routine: `VC_0` to `VC_3` are written as zero when a capture path of that type stops.
- Inferred: `VC_2` bit 0 is the DMA enable. The start routine sets it once, after `VC_3` is written.
- Inferred: `VC_3` holds a path byte. The byte supplies the field-select and descriptor-list enable bits, and the start routine combines it with the physical address of a descriptor block when the driver's descriptor word is zero.
- Inferred: `VC_4` to `VC_8` hold five words that the start routine copies from a descriptor block when the driver has a descriptor word set. Their layout is not recorded.

The word width of the block is inferred as 32 bits, from the word-sized copies the reading describes.

## Unknown

- Which cause each of the eight channel interrupt enables covers, and the input channel's bit positions beyond the nine-bit group.
- The layouts of the clip, alpha, line-width, padding, genlock-delay and configuration words, on both channels.
- Output hardware configuration bits 4, 7 and 8, and the audio-sync reset bit.
- The meaning of the two-wire configuration values 0 and 2, and what a bus error does beyond the force-idle path.
- Which word of each timer snapshot is the time stamp.
- Whether the silicon accepts an output configuration write while DMA is enabled. The reading shows the driver writes that word only at start-up.
- The odd framelock start-of-field count. The reading says a small per-source routine chooses it, and that routine was not recorded.
- Whether the genlock delay must be nonzero when a genlock source is selected. The driver does not test that.
