# O2 CRIME: identity, interrupts, renderer interface and pixel pipeline

CRIME is the O2's memory controller, interrupt controller and rendering engine. The IRIX kernel, the OpenGL library and the X server write its registers in a fixed order, and this page records what they write, in that order. The register offsets, identity values, interrupt inputs and field positions are in the crime repository's reference for the `crime`, `re` and `mte` units; the constants the kernel and library apply are listed with their grades in [kernel register use](../kernel/hardware-register-use.md#o2-crime). The kernel's pixel-DMA request constants are in [formats/crime_dma/format.toml](../../formats/crime_dma/format.toml). Offsets with no stated width are in the tables below instead.

Grades follow [gbe.md](gbe.md). **Established** means the whole routine, table or section was read in a shipped binary. **Inferred** means a reasoned reading, with the reasoning given. **Unknown** means no shipped software or published source settles it.

The source reading gave roles for each routine, not addresses, so the addresses are not repeated here. Re-checking each established item against its routine is open work. A later verification pass compared the page with the kernel objects of the IRIX 6.5.30 IP32 boot set (`crime.a` and `kernel.o`), the CRIME 1.5 specification, and the NetBSD and Linux sources pinned in `references/code/`. Where it confirmed or corrected a fact the text says so and gives the address or page. The objects that are not in the kernel boot set (the renderer, OpenGL, X server and video-control objects below) were not reachable in that pass. Every fact on this page whose only source is one of them is unchanged and unchecked.

Published sources are hints, not evidence. Facts taken only from them are graded inferred here, unless a shipped binary confirms them. They are the CRIME 1.5 specification (revision 0.3, 2 June 1997, page numbers from the PDF), the GBE ASIC specification revision 1.1 (18 July 1996), and the public NetBSD sgimips and Linux IP32 drivers.

## Sources

The binaries are the graphics objects of the IRIX 6.5.30 overlay disc `Instalation_Tools_and_Overlays1.image` (SHA-256 `0e8b0610955686f3be551b73f17b80c6748ed7e6ff8898dd66766898b4c8222b`):

| Object | Bytes | SHA-256 |
|---|---:|---|
| Graphics kernel archive | 228,420 | `bbea587ce476a475461f8d0147d651ffc4aec7d39e6da61d33435dc9650e182a` |
| Renderer context object | 61,572 | `1cc4da3459d921fe6c82feac5064ec8db4264902c9910fb309e75b2cc72afd07` |
| Renderer initialisation object | 19,972 | `8a1446b8d97db07547eeff22bbc7d72ed1daa98fed1c828ffc3db640f1c82d97` |
| Tile allocation object | 26,388 | `f422bfb8e7ac546418a30f9a676ffb3d52874cf1615d223601c3c685cc48e621` |
| Pixel-transfer object | 11,268 | `f66d63177831f79cacf818b98688cf07a69f7dec4ba26e70abd41cc487812423` |
| Retrace object | 32,916 | `1839b4af7754b7d853eccbcf9f3ca5531219f01ea5fe382b399b7b3d5f41190e` |
| Machine-dependent kernel object | 504,132 | `ebf09eeae80f3726d7b324e2d829983adee15f79af215d256b6d33ed24cef3b6` |
| OpenGL core library, N32 | 2,572,500 | `b33ef449a0ffa93afd9242bc331eeb81111e22e81e3d8b7f1b6045c666cc9492` |
| X server display module | 1,861,396 | `4d5bf5d105b6afbb94300385daebf2b72b84f4454190fc081652131c92615b7a` |
| X server executable | 3,120,928 | `68792b0377bde4be868d03618c34b0dbaa7696cb8b1b5fd9cf40bb09ebbc7166` |
| Video-control client library, N32 | 76,084 | `9feef3be85dbeac2e1cad3e4a150cbb455938179c7f9ed9f72ce1e800cf4c47f` |
| Base kernel archive (tile allocator member) | 2,860,500 | `9bd72a24c18d8ebc219a37594db140eb1cf7952a1cae9d79745b9db8ed3649f4` |

The graphics kernel archive, the machine-dependent kernel object, the base kernel archive and the X server executable were re-hashed in the verification pass and match the files of the same names in the 6.5.30 IP32 boot set and the generic X server directory. The renderer, OpenGL, X display module and video-control objects were not available then.

The binaries show what the software expects the hardware to do. They do not prove the silicon. Where a fact here is only a software expectation, the text says so.

## Identity and address windows

- **Identity** (established): the graphics probe reads the low byte of the CRIME block and accepts the part only when bits 7:4 are `0xA` and the byte is not `0xa0`. A part reporting the 1.5 specification's value `0xb1` (PDF page 114) is refused, so IRIX 6.5.30 does not start graphics on it. Checked: `crimeProbe` in `crime_init.o` at `0x44` loads the doubleword at `0xb4000000`, masks the low byte, requires `0xa0` in the top nibble and returns 0 when the byte equals `0xa0`.
- Inferred: shipping O2s report family `0xA`. This is the reading the accepted family implies, not a measured value. The specification's note on PDF page 114 says Petty Crime and CRIME 1.1 reported ID `0xA`, with revision numbers 0 and 1.
- **Address windows** (inferred, from the specification's map; Table 3-1 on PDF page 48 and the text on page 47, checked against the document): low memory alias at 0, frame and depth apertures at `0x10000000` and `0x12000000` (32 MiB each), CRIME at `0x14000000`, GBE at `0x16000000`, MACE from `0x18000000` to `0x1fffffff`, a high alias at `0x20000000` (512 MiB; the table marks its responder with a question mark), full memory at `0x40000000` (1 GiB), and a no-ECC alias at `0x80000000` (1 GiB). The decode is 40-bit. A location should be reached through only one of the low, high or full aliases.
- Established: no IRIX graphics object builds an address in either aperture. Inferred: IRIX renders and scans out only through the TLBs and tile lists.
- **Conflict** (established on both sides): the specification lists only `0x40000000` to `0x7fffffff` as GBE-visible memory (PDF page 49). The IRIX GBE tile lists use 13-bit tile numbers that address low memory, and the Linux driver builds `phys >> 16` of low memory. How GBE addresses below `0x40000000` resolve is unknown.

## Error and interrupt inputs

- **Inputs** (established): IRIX registers 4 or 5 (MACE ISA), 16 (retrace), 17 (pre-blank), 18 and 19 (further GBE lines, unused), 20 and 21 (bus errors), 25 (renderer FIFO at or below empty), 26 (renderer FIFO above full), and 28 and 29 at priorities 1 and 2. IRIX does not use 22 to 24 or 27. Checked: `crime_init` (`crime.o`, `0x5364`) installs `crimeRetraceHandler` on 16, `crimePreblankHandler` on 17, `crimeFifoEmptyHandler` on 25 and `crimeFifoFullHandler` on 26. `crimeInterrupt` (`crime.o`, `0x4af8`) serves 18 and 19. `mlreset` (`kernel.o`, `0x8738`) installs `synthclock` on 28 and `synthackkgclock` on 29 at priorities 1 and 2, `buserror_intr` on 20 and 21 at priority 3, and a power-switch handler on MACE ISA input 5. Input 4 was not found in these objects.
- Inferred: 28 and 29 are the software inputs, raised through the software-interrupt word. The NetBSD names are SOFT0 and SOFT1. The dispatcher sets them from two kernel flags. Their handlers are named `synthclock` and `synthackkgclock` in the kernel's symbol table, so they most likely carry a synthesised clock tick, but nothing read shows what the tick drives.
- **Bus-error inputs** (established): one handler is installed on both inputs 20 and 21 at the same priority. Which input the silicon raises for CPU errors and which for memory errors is **inferred**: CPU on 20 and memory on 21. NetBSD (`crimereg.h`: `CRIME_INT_CRMERR` is bit 20, `CRIME_INT_MEMERR` bit 21) and Linux (`ip32_ints.h` numbers `CRIME_CPUERR_IRQ` 20 and `CRIME_MEMERR_IRQ` 21) agree, and so does the specification's own interrupt register table, whose revision notes (PDF page 233) say the VHDL, the specification and the software defines all place the memory error at 21 and the CPU interface error at 20. Only the prose of section 5.7.2.2 (PDF page 109) says CPU on 21. Whether the silicon matches the table is not settled by a measurement, hence inferred.
- **Capture** (established): the handler reads the CPU error words at `0x40`, `0x48`, `0x58`, and the memory error words at `0x250`, `0x258`, `0x260` and `0x268`. It clears `0x48` and `0x250` by writing zero. It never reads `0x4048`, which the 1.5 draft uses.
- **Decoders** (established, decoder selection; inferred revision mapping): the kernel caches a revision code from the identity. The code is 17 when the low nibble is non-zero, then 19 or 20 from the upper word, and 0 otherwise. Zero selects the revision 0 decoder and any other value the revision 1 decoder. Inferred: 17, 19 and 20 are revisions 1.1, 1.3 and 1.4.
- **Acknowledgement** (established): the retrace and pre-blank handlers clear bit 16 or 17 of the edge word at `0x28` with a read-modify-write.
- **Entry and dispatch** (established): the interrupt entry path saves the mask word at `0x18` and installs a per-priority mask, restoring it on exit. Enable and disable edit software mask tables only. The dispatcher reads the status word at `0x10`, ANDs it with the saved mask (low 32 bits), and loops until nothing is pending.
- **Absent interrupts** (established): IRIX uses no MTE-done or engine-idle interrupt, only the buffer empty and full levels. Linux names idle interrupts at 24 and 27. Whether either covers the MTE is unknown.
- Inferred from the GBE rev 1.1 specification: four line-match interrupts latch in CRIME, and only software clears them there.

## Control, time and memory words

- **CONTROL `0x08`** (established offset and early-reset writes; bit names from the NetBSD driver, which names bit 12 `CRIME_SYSADC` and bit 13 `TRITON_SYSADC`, inferred): bits 13 and 12 are the SysAD bits, 11 hard reset, 10 soft reset, 9 watchdog enable, 8 big-endian, 7:4 and 3:0 queue high-water marks. Early reset clears bit 9 on every CPU. Bits 13:12 are cleared on R10000-family CPUs and set on the others, which the source states as implementation `0x23` and later. Checked: `mlreset` (`kernel.o`, `0x8738`) reads the doubleword at `0xb4000008`, clears bits 13, 12 and 9 when the processor implementation field is below `0x23`, and otherwise clears bit 9 and sets bits 13 and 12. The specification (PDF page 114) calls bit 13 the R5000 SysADC parity-generation control and bit 12 the CRIME SysADC check control, and marks both not used in CRIME 1.5. The electrical meaning of the SysAD bits is **unknown**.
- **Watchdog `0x30`** (established): written zero at start-up and never armed. Checked: `mlreset` stores zero at `0xb4000030`. The specification names `0x30` the watchdog timer and `0x38` the CRIME timer (PDF page 113).
- **Time `0x38`** (established): a free-running counter. IRIX reads its low word and states a resolution of 15000. The unit is inferred to be picoseconds, a 66.67 MHz tick.
- **Memory control `0x200`** (established): 1 enables and 0 disables. ECC checking on reads is inferred. The kernel never touches the bank registers or the refresh counter. The PROM sets them.
- **Interrupt mask `0x18` and status `0x10`**: see above.
- Unknown: the ECC enable. The word at `0x270` is named ECC replacement check bits in the specification's memory-control register list (inferred); no software read here uses it.

## Renderer interface buffer and flow control

- **Control word `0x400`** (established layout as the kernel writes it): `count | stall << 7 | empty << 14 | full << 21`, four 7-bit fields. The first write is `0x0fefff0a`: full 127, empty 63, stall 126, count 10. Then, per CPU, R10000-family and RM7000 modules get full 20, empty 0, stall 19; other R5000-class modules get full 48, empty 24, stall 47. The count stays 10. Checked: `crimeInitHW` (`crime_init.o`, `0xb90`) builds exactly these words and writes them to `0x400`. It picks full 20 (the kernel variables `r10kfifohigh` and `r7kfifohigh` both hold 20) for implementation below `0x23` and for `0x27`, and full 48 and empty 24 for the rest. The interrupt handlers later rewrite only the empty field (bits 20:14) or the full field (bits 27:21). Inferred: the fields drive the interrupts at the levels the specification's prose describes (PDF page 185).
- **Status `0x4000`** (established): occupancy in bits 24:18, masked with `0x7f`, and idle in bit 28. The context-save routine reads a start pointer in bits 5:0 and a write pointer in bits 11:6.
- **Depth**: inferred 64 entries, strongly. The full-level handler reports overflow at an occupancy of 65 or more, and the context-save routine walks the buffer as a ring of 64, with the index wrapping at 64. The specification prints 128-entry arrays (PDF page 163). See the conflicts table.
- **Flow control** (established): the library never reads occupancy. It writes without checks and relies on the hardware stall and on input 26. On input 26 the kernel raises the full threshold to the current level plus one, enables input 25 and deschedules the writer. Input 25 restores the thresholds and wakes the writer. The library's only status reads poll the idle bit after a zero write to the pixel-pipeline flush at `0x21f8`.
- **Flush word `0x4008`** (established): only the kernel writes it, always with zero. It is written after TLB loads, between batches of initialisation TLB writes (each batch preceded by a full drain), on context switch and clip validation, and at the end of context save. Context save drains committed work, copies each entry from the start pointer up to the write pointer (address array as 64-bit, data array as two 32-bit reads), then writes `0x4008`. Inferred: entries after the start pointer are parameters not yet committed by a START write, and the write discards them. **Unknown** until a capture shows the status pointers before and after the write.
- **Engine reset `0x408`** (established write; role inferred; initialisation also writes it: checked in `crimeInitHW` at `0xbe4`): after a drain, idle or empty-wait timeout, the kernel writes zero to `0x408`, waits 10 microseconds and rewrites the interface-buffer control word. It does not reload TLBs or shadows. Inferred: the write resets the engine and its interface buffer, returning the control word to its power-on value. No other writer or reader of `0x408` was found. The specification's table ends at `0x400`.
- **Address word of an interface-buffer entry** (established software decoding): register index in bits 39:32, group in bits 42:40, lane code in bits 44:43, and bit 45 as a stop marker. See `IB_ADDR` in the reference.
- **Commit** (established): the final write of each primitive goes through the `0x800` START alias of whichever register is written last, not a fixed one. Global registers are written twice, to the hardware and to a kernel-provided shadow page. Per-primitive registers are written only to the hardware.

## TLBs

| TLB | Registers | Entry | Value the kernel loads |
|---|---|---|---|
| Frame buffer A, B, C | 64 each | 16-bit, four per register, first in bits 63:48 | allocator frame >> 4 |
| Texture | 28 | 16-bit, four per register | tile number, with bit 15 set |
| CID | 4 | 16-bit, four per register | tile-list entry, unchanged |
| Linear A, B | 16 each | 32-bit, two per register | page frame, with bit 31 set |

- **Frame-buffer TLBs** (established from the kernel loader): tile column x, row y uses register `y * 4 + x / 4`, and the halfword at byte offset `2 * (x & 3)` of the big-endian doubleword. An entry v maps page frames `v << 4` through `(v << 4) + 15`. The loader sets no valid bit. Whether hardware has one is **unknown**. Sixty-four registers give a 16 by 16 tile grid, the 2048-pixel limit (PDF page 186).
- **Bit 15 of a tile entry** (inferred): the allocator can return frames ORed with `0x80000`, the no-ECC alias (PDF pages 47 to 48). So bit 15 is most likely physical address bit 31 of the alias, not a separate valid flag. Which branch frame-buffer allocations take is **unknown**.
- **Linear TLBs** (established): two 32-bit entries per register. The even page sits in bits 63:32, and the entry is the page frame ORed with `0x80000000`. The register index is `(page >> 1) & 15`, and each TLB covers 32 pages of 4 KiB. Inferred: the page value is a 4 KiB frame number.
- **Pinned buffers** (established): a user buffer of at most 32 pages is loaded by a graphics ioctl. Flag 4 selects linear A, otherwise B, and flag 8 marks hardware read-only access. A failure or a zero length clears all 16 registers and writes `0x4008`. Flag bit 5 is passed through to both loaders and neither tests it, so it has no effect (established over both loaders).
- **Texture TLB** (established): the kernel keeps a 28-register image per texture and writes all 28 doublewords on each bind.

## Pixel pipeline encodings

All of these are established from the OpenGL library's register setters, unless marked otherwise.

- **DrawMode `0x2018`**: logic-op enable is bit 9, dither is bit 8, hardware fog is bit 14 and the antialiasing stage is bit 12. This matches NetBSD and reverses the specification's table (PDF page 174). See the conflicts table.
- **Primitive `0x2060`**: triangles are `0x02000000 | edge << 16`. The depth clear is the rectangle `0x03020000`.
- **Vertex words**: triangles send biased binary32 words `0x48040000 + 64x`. The depth clear sends `0x00840000 + 64x` from the same bias, `0x21000` (established). The two agree only in bits 18:0. The rasteriser reads at most those bits, an unsigned 13.6 field, with bit 18 meaning +4096 (inferred). No capture shows which upper bits the silicon ignores.
- **Colour**: `Shade.fgColor` packs 8-bit R, G, B and A in bits 31:24 down to 7:0. Smooth-shading starts and slopes are 0 to 255 colour values times 4096, truncated toward zero, a 9.12 value. The 12-bit colour-index visual uses 256 in place of 255.
- **Depth**: window z spans 0 to `0x3fffffff`. Zs, dz/dx and dz/dy are 64-bit truncations of window z times 64, so a 24-bit buffer value carries 12 fraction bits. That fits the specification's 25.12 format (inferred). `Depth.func` at `0x21c0` holds the GL function minus GL_NEVER in bits 27:25, bit 24 on the stencil-only path (meaning **unknown**), and the clear depth in bits 23:0, computed as `0x3fffffff * clear >> 6`.
- **Stencil mode `0x21e0`** (the specification's table is empty, PDF page 182): reference in 31:24, value mask in 23:16, function in 14:12 (GL function minus GL_NEVER), stencil-fail operation in 11:8, depth-fail in 7:4 and depth-pass in 3:0. Operations: KEEP 0, ZERO 1, REPLACE 2, INCR 3, DECR 4, INVERT 5. The stencil write mask is at `0x21e8`.
- **LogicOp `0x21b0`** (table empty, PDF page 182): GL op minus GL_CLEAR, all 16 values.
- **Alpha test `0x2198`** and **blend function `0x21a8`** match PDF pages 180 to 181. When the drawable has no alpha, DST_ALPHA is sent as ONE, ONE_MINUS_DST_ALPHA and SRC_ALPHA_SATURATE as ZERO.
- **Texture mode `0x2110`**: the minification and magnification codes match PDF page 178. The wrap codes match too, except for the border clamp: the specification (PDF page 179) has CLAMP 0, REPEAT 1, CLAMP_TO_EDGE 2 and CLAMP_TO_BORDER 3, and IRIX sends REPEAT as 1, CLAMP as 0 and CLAMP_TO_EDGE as 2. CLAMP_TO_BORDER is sent as **1** with the border colour loaded. A library-wide override sends CLAMP and CLAMP_TO_BORDER as 2. Code 3, the specification's border clamp, is never written. Environment functions are MODULATE 0, DECAL 1, BLEND 2 (which loads the environment colour) and REPLACE 3. Map level bits 17:14 hold `10 - (level field)`, and bits 13:10 hold the maximum level. Bit 24 selects subtiled texels, and bits 23:20 and 19:18 hold the type code and log2 bytes per texel.
- **Texture format `0x2118`** (64-bit, see the reference): the coordinate scale puts a texture coordinate of 1.0 at 2^14 after the perspective divide, leaving 4 sub-texel fraction bits and a 1024-texel maximum (inferred).
- **Filter arithmetic** is **unknown**. No library code computes weights for the hardware path. A capture of a magnified two-texel texture would settle the weight precision and rounding.
- **Fog** (established): `Fog.color` at `0x2170` holds three 8-bit components in bits 31:8. The fog steppers at `0x2178`, `0x2180` and `0x2188` are 32-bit integers, the vertex fog factor (0 to 1) times 255 times 4096. The specification (PDF page 169) calls the format a 9.12 two's-complement value, which holds that range. Point gradients are zero, and triangles solve the same plane equation as the colour steppers. Unknown: the hardware blend formula and whether the stepper clamps.
- **Antialiased lines** (established): the line register at `0x2190` takes the minor-axis slope times 16384 in bits 31:16 and the starting minor coordinate with 8 fraction bits in bits 15:0. The specification disagrees with the second half: its register field table (PDF page 180) gives both fields as 2.14 two's-complement values, and its register map (PDF page 169) says 2.12 for both. The slope scale of 16384 fits 2.14. The 8-fraction-bit reading of the coordinate fits neither, and the library that supplied it was not reachable for the check, so treat it as unconfirmed. The assembly path commits it with a 64-bit store to `0x2990`, the START alias. Polygon antialiasing is done in software and never writes `0x2190` or `0x2194`. Unknown: the hardware weighting and how it interacts with blending.

### Texel formats

Every requested internal format collapses onto a stored 16-bit or 32-bit texel (established, from the library's level-size routine). The type code goes into Texture.mode bits 23:20 and log2 bytes into bits 19:18.

| Stored | Texel bits | Type code | Requested formats |
|---|---:|---:|---|
| ALPHA16 | 16 | 5 | ALPHA and ALPHA4 to ALPHA16 |
| LUMINANCE16 | 16 | 7 | LUMINANCE and LUMINANCE4 to LUMINANCE16 |
| LUMINANCE8_ALPHA8 | 16 | 8 | LUMINANCE_ALPHA and the sized pairs |
| INTENSITY16 | 16 | 6 | INTENSITY and INTENSITY4 to INTENSITY16 |
| RGBA8 | 32 | 1 | RGB8, RGB10, RGB12, RGB16 |
| RGB5_A1 | 16 | 1 | R3_G3_B2, RGB2, RGB4, RGB5 |
| RGBA8 or RGB5_A1 | 32 or 16 | 1 | RGB, chosen by a context count and a global flag |
| RGBA8 or RGBA4 | 32 or 16 | 2 or 4 | RGBA, by the same tests |
| RGBA8 | 32 | 2 | RGBA8, RGB10_A2, RGBA12, RGBA16 |
| RGBA4 | 16 | 4 | RGBA2, RGBA4 |
| RGB5_A1 | 16 | 2 | RGB5_A1 |

Type code 3 is never produced. The subtile flag is set when a global flag and the caller's argument are both zero. Inferred: a 16-bit-colour visual textures at 5-5-5-1 and a 24-bit one at 8-8-8-8. Unknown: the in-memory bit order of 16-bit RGB5_A1 and RGBA4 texels.

## Tiles, subtiles and the MTE

- **Frame-buffer tiles** are 64 KiB: 128 rows of 512 bytes (PDF page 186, inferred). Unsubtiled textures use the same layout (inferred from the row stepping of the library).
- **Subtiled textures** (inferred from the library's address stepping): 32-byte cells of 8-byte rows, 16 cells per 512-byte row, 16 rows per 8 KiB. At 16 bits a cell is 4 by 4 texels and an 8 KiB subtile is 64 by 64 texels. There are no 8-bit texels (PDF page 178). Unknown: whether 32-bit formats are ever subtiled, and the order of subtiles within a tile.
- Texels reach memory as CPU stores through a kernel mapping of the texture's tiles. No MTE or DMA transfer appears in the texture-image path (inferred).
- **MTE registers** (established, offsets not given): MODE, BYTEMASK, FG, DST Y step, DST0 and DST1 (the last two through the START alias), and FLUSH, which takes a zero write at the end of a sequence. Mode is `pixDepth << 8 | buffer << 2`. The X server copies with mode `0x820`, copy, source TLB B, destination TLB A, 8-bit.
- **Address word**: each SRC and DST word is a byte column in bits 31:16 and a row in bits 15:0. Columns count bytes whatever the pixel size. An end word names the last byte.
- **X server copy** (established): global set-up of BufMode, DrawMode `0x00200278`, LogicOp 3, transfer steps 1, Primitive `0x03020000`, MTE mode `0x820`, BYTEMASK and a Y step of +1. For each clip box, whole rows go to the MTE, with the remaining rows and edge columns going through the pixel-pipeline transfer path. The block is a multiple of 16 rows at 16 bits and 8 rows at 32 bits, and 256 bytes wide. The sequence ends with zero writes to MTE FLUSH and the pixel-pipeline flush. Inferred: the MTE moves only whole units of that size. The X server never writes a negative Y step. NetBSD uses -1 for overlapping downward copies.
- **Fills** (established): mode `depth << 8 | buffer << 2`, BYTEMASK, FG, the DST Y step, DST0, then DST1 through START. Stippled fills take a second pass with the stipple bit.
- **Kernel host-memory clear and copy** (established): mode `0x11` (clear, linear A, destination ECC) and `0x897` (copy, linear A to linear B, both ECC bits), in chunks of at most 128 KiB. The SRC and DST words are physical addresses that the linear TLBs map to themselves. Each pass waits until `64 - ((status >> 18) & 0x7f)` free slots reach the count it needs. The completion wait tests a constant that is always zero, so it never waits (established as code). The intended poll of the MTE-idle bit 25 is inferred.
- **Pixel DMA** (established): a kernel request of `0x28` bytes does the same transfer as the library path. It writes either 64-bit GL vertex pairs `(y << 6) | (x << 38)` with a `0x1000` bias, or the 16-bit X vertex form. It commits with an address write through START and waits for idle with a poll limit of 500,000. The form is chosen by whether the calling context is the one the engine is bound to. An unbound caller gets the GL pairs, with the coordinates first reduced by `0x21000` and then increased by `0x1000`. Its source Y step is written to `0x20ac` as `(zoom - 1) << 28 | (stride + pixel size - width)`, and `0x20a8` is not written. A bound caller gets the X form at `0x2070` and no step write.
- **Pixel-transfer addresses** (established): a draw's source X step is the pixel byte size and its Y step is the pixel byte size plus row padding, masked to 17 bits. The commit uses the source address masked with `0x1ffff`, a 128 KiB linear window, through the START alias of `0x20a0`. Reads place the screen position in `0x20a0`, x in bits 26:16 and y in bits 10:0. Depth images use byte mask `0x38` and stencil uses `0x40` with the colour mask in bits 31:24. So byte-mask bit 3 + i enables byte i of a 32-bit pixel, and stencil is byte 3. Inferred: linear addresses are 17-bit offsets into the window.

## Reset and initialisation

- **Init order** (established): `0x408` is written zero and the cursor control is written zero. The interface-buffer control word takes its boot value. Then the frame-buffer TLBs are cleared, draining every eight entries, followed by the linear and texture TLBs. Each clear is followed by a `0x4008` write. Last come the CPU-specific thresholds, after an idle wait.
- **Early reset** (established): CONTROL `0x08` is written with the bits described above. The interrupt mask word and the status word at `0x10` are written zero, and the CPU error and memory error status words are cleared. Unknown: the meaning of bits 13:12 and 9 at the electrical level.

## Context switching

- The engine has no hardware context. The kernel keeps a per-client shadow record: pixel-pipeline registers at `0x2000 + x` in the shadow, and MTE registers at `0x3000 + x`, with `0x200 + x` as the offset. Offset `0x1f0` holds a software flag word that selects which register groups reload.
- Outgoing: save uncommitted interface-buffer entries, read the stipple pattern back if one is used, then write `0x4008`. Incoming: wait for idle, revalidate the clips, load the frame-buffer TLBs A to C and the linear TLBs A and B (and the texture TLB if needed), each with a `0x4008` write, load the global registers and then the non-global ones from the shadow, write `0x4008`, and replay the saved entries.
- **Global class** (established): the kernel's global and non-global table agrees with the specification's column (PDF pages 164 to 170) except for the null pixel-pipeline register at `0x21f0`. The kernel treats it as global, and the specification lists it as non-global. Unknown: the hardware's class for `0x20d0` and `0x21f0`.

## Software geometry (not hardware)

Lighting, colour clamping and frustum clipping run on the CPU. The engine receives only clipped, lit primitives (established). Inferred: the rasteriser needs no frustum or guard-band clipping. Window and scissor clipping use its screen masks and scissor.

## Conflicts

| Topic | Specification | Public drivers | IRIX | Status |
|---|---|---|---|---|
| DrawMode bits 9 and 8 | bit 9 dither, bit 8 logic op (PDF page 174) | NetBSD: bit 9 logic op, bit 8 dither | bit 9 logic op, bit 8 dither | Drivers and IRIX agree. The specification is inferred wrong for shipped parts |
| Interface-buffer depth | 128-entry arrays (PDF page 163) | NetBSD: 6-bit pointers, maximum level `0x3f` | overflow above 64; ring of 64 in context save | 64, inferred strongly |
| Bus-error inputs 20 and 21 | prose of 5.7.2.2: CPU 21 (PDF page 109); register table and revision notes: CPU 20, memory 21 (PDF page 233) | CPU 20, memory 21 | one handler on both | CPU 20, memory 21, inferred. Only the prose differs |
| Texture border wrap | CLAMP_TO_BORDER = 3 (PDF page 179) | none | border clamp sent as 1 | Unknown which is right |
| Identity | 1.5 = `0xb1` (PDF page 114) | NetBSD names both families | refuses `0xb1` | Shipped family `0xA`, inferred |
| Null pixel-pipeline register class | non-global (PDF page 170) | none | kernel treats it as global | The kernel view is what context save assumes. Hardware unknown |
| GBE memory window | GBE sees `0x40000000` to `0x7fffffff` (PDF page 49) | Linux: `phys >> 16` of low memory | 13-bit (low-memory) tile numbers | How low addresses resolve is unknown |
| Tile entry bit 15 | not described | NetBSD sets `0x8000` on programmed tiles | allocator can return no-ECC-alias frames; texture entries set it | Inferred address bit 31 |

## Open items

| Item | What would settle it |
|---|---|
| Role of `0x408`; whether TLBs survive it | Queue primitives with a high stall level, write `0x408`, read status `0x4000` and the drawn frame; repeat after a TLB write |
| Reset values of the CRIME, renderer, MTE, CONTROL, `0x30` and `0x200` registers | Read each at power-on from the PROM monitor on a stock O2 |
| MTE ordering against primitives and linear-TLB changes | Queue a primitive writing memory, an MTE copy of it, then a linear TLB rewrite; compare the copy |
| MTE completion interrupt | Unmask input 27, run only an MTE fill, read the status word |
| MTE odd start columns, row unit and stipple period | Small MTE copies and fills at odd columns, 1 to 20 rows and a known stipple; read back |
| Subtile order and 32-bit subtiling | Dump a texture tile after downloading recognisable 16-bit and 32-bit images |
| Hardware class of `0x20d0` and `0x21f0`; address-word bit 45 | Dump interface-buffer entries after writing each register, and during a stalled queue |
| Mid-primitive context switch | Logic-analyser capture on the renderer bus across a forced switch |
| Error input numbering; edge-bit clearing | Provoke CPU and ECC errors separately; read the status word, read again, write `0x28` |
| CONTROL bits 13:12 and 9 | Write each bit alone and read back; watch bus parity with bits 13:12 toggled |
| ECC enable and `0x270` | Write through the no-ECC alias with a flipped check bit, read through the normal alias, read the error registers |
| Error-status bit meanings per decoder | Provoke each error type and read the CPU and memory status words |
| TLB validity bit | Draw through a frame-buffer entry with bit 15, then bit 31, cleared |
| Unused step bits, zero step, and the source walk of an overlapping copy | Transfers with step 2 at 16 and 32 bits and a zero step; overlapping copies in each direction |
| Overlay compositing and transparent value | Known overlay values over a known frame buffer; read back the video output |
| FIFO depth | Stall the engine, queue single writes, read the occupancy bits until writes are refused |
| Depth, vertex and blend precision | Two triangles whose z differs by one step; known blends; read back |
| Fog blend and stepper clamp | One-pixel span with known fog steppers over a known colour |
| Antialiased-line weighting and coverage with blending | One antialiased line with known register values over black, with and without blending |
| Texture format choice and 16-bit texel bit order | Read the format record after each texture format; dump a tile with known texels |
| Texel filter weights | Magnified two-texel texture; read intermediate pixels |
