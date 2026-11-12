# O2 CRIME as the shipped IRIX kernel and graphics libraries drive it: identity, interrupt inputs, renderer state and TLB formats

<!-- generated from defs/crime.toml by defs-gen - do not edit, change the source -->

Offsets, identity values, interrupt inputs and renderer field layouts that the O2 kernel, OpenGL library and X server use on the CRIME block. Prose and the reasoning are in docs/crime.md.

Unit `crime`, macro prefix `CRIME`.
Default source: shipped IRIX O2 graphics objects, per docs/crime.md.

Every item has a grade: established, inferred or unknown. A grade says how the item was derived from its source; it does not claim the item was measured on silicon.

## Registers

| Register | Offset | Access | Width | Count | Space | Reset | Grade | Source |
|---|---|---|---|---|---|---|---|---|
| `id` | 0x0 | r | 8 |  |  | unknown | established | shipped IRIX O2 graphics objects, per docs/crime.md |
| `texture_format` | 0x2118 | w | 64 |  |  | unknown | established | shipped IRIX O2 graphics objects, per docs/crime.md |

### Register `id`

Identity byte at the start of the CRIME block. The graphics probe reads its low byte and accepts the part only when bits 7:4 are the accepted family and the byte is not the refused value.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 7:4 | `family` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | family nibble; the probe accepts 0xA |

### Register `texture_format`

Texture format record, written by the OpenGL library for each texture. It carries the log2 sizes of the texture and the coordinate masks. The width of the register is taken from the top field, which reaches bit 47.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 47:44 | `width_inv` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | 10 minus log2 of the texture width |
| 43:40 | `height_inv` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | 10 minus log2 of the texture height |
| 39:36 | `width_log2` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | log2 of the texture width |
| 35:32 | `height_log2` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | log2 of the texture height |
| 31:16 | `width_mask` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | (1 << (log2 width + 4)) - 1 |
| 15:0 | `height_mask` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | (1 << (log2 height + 4)) - 1 |

## Words

| Word | Width | Grade | Source |
|---|---|---|---|
| `IB_ADDR` | 64 | established | shipped IRIX O2 graphics objects, per docs/crime.md |

### Word `IB_ADDR`

Interface-buffer address word, as the kernel decodes it in software. Bit 45 marks the stop entry.

| Bits | Field | Grade | Values | Source | Description |
|---|---|---|---|---|---|
| 45 | `stop` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | Stop marker, used by the kernel's decoder |
| 44:43 | `lane` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | Lane code |
| 42:40 | `group` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | Register group |
| 39:32 | `reg_index` | established |  | shipped IRIX O2 graphics objects, per docs/crime.md | Register index |

## Constants

| Constant | Value | Grade | Source | Description |
|---|---|---|---|---|
| `AA_LINE_OFF` | 0x2190 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Antialiased-line register: minor-axis slope in bits 31:16, start position with 8 fraction bits in bits 15:0 |
| `ALPHA_TEST_OFF` | 0x2198 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Alpha-test register |
| `BLEND_FUNC_OFF` | 0x21a8 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Blend function register |
| `BLOCK_ADDR` | 0x14000000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Physical address of the CRIME block, as the graphics probe reads it |
| `CID_TLB_REGS` | 0x4 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Registers in the CID tile-list TLB, four 16-bit entries each |
| `CONTROL_OFF` | 0x8 | established | shipped IRIX O2 graphics objects, per docs/crime.md | CONTROL word; the early-reset code writes bits 13:12 and 9 of it |
| `CPU_ERR_STATUS_OFF` | 0x48 | established | shipped IRIX O2 graphics objects, per docs/crime.md | CPU error status; the capture routine clears it by writing zero |
| `DEPTH_APERTURE_ADDR` | 0x12000000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Depth-buffer aperture, 32 MiB |
| `DEPTH_FUNC_OFF` | 0x21c0 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Depth function in bits 27:25, a stencil-only flag in bit 24 and the clear depth in bits 23:0 |
| `DMA_IOCTL` | 0x55f2 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Request number of the kernel pixel-DMA ioctl |
| `DMA_POLL_LIMIT` | 0x7a120 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Polls the kernel makes for idle before the pixel DMA gives up |
| `DMA_REQ_BYTES` | 0x28 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Size of the kernel pixel-DMA request |
| `DRAWMODE_OFF` | 0x2018 | established | shipped IRIX O2 graphics objects, per docs/crime.md | DrawMode; bit 9 is logic-op enable, bit 8 dither, bit 14 hardware fog, bit 12 antialiasing |
| `ENGINE_RESET_OFF` | 0x408 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Engine reset word; the kernel writes zero, waits 10 microseconds and rewrites the interface-buffer control word, named IB_CTL in the reading. Whether IB_CTL is the word at RENDER_CTL_OFF is inferred. The reset role of this write is inferred |
| `FB_TLB_REGS` | 0x40 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Registers in each frame-buffer TLB (A, B and C), each holding four 16-bit tile entries |
| `FOG_COLOR_OFF` | 0x2170 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Fog colour, three 8-bit components in bits 31:8 |
| `FRAME_APERTURE_ADDR` | 0x10000000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Frame-buffer aperture, 32 MiB |
| `FULL_MEM_ADDR` | 0x40000000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Full memory window, 1 GiB |
| `GBE_WINDOW_ADDR` | 0x16000000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | CPU window the specification gives for the GBE block. Not confirmed by the binaries; the GBE page keeps the GBE block base as unknown |
| `GL_UNBOUND_BIAS` | 0x21000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bias the kernel removes from coordinates for a caller that is not bound to the engine |
| `GL_VERTEX_BIAS` | 0x1000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bias the library applies to GL vertex halves in pixel DMA |
| `HIGH_ALIAS_ADDR` | 0x20000000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | High alias of low memory, 512 MiB; the specification's table marks its responder with a question mark |
| `ID_FAMILY_ACCEPTED` | 0xa | established | shipped IRIX O2 graphics objects, per docs/crime.md | Value of bits 7:4 of the identity byte that the graphics probe accepts |
| `ID_REFUSED_BYTE` | 0xa0 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Identity byte the graphics probe refuses even though its family is accepted |
| `ID_SPEC_15` | 0xb1 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Identity byte the specification gives for CRIME 1.5. The graphics probe refuses it |
| `INTMASK_OFF` | 0x18 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Interrupt mask word; the entry path saves it and installs a per-priority mask |
| `INTR_EDGE_OFF` | 0x28 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Interrupt edge word; the retrace and pre-blank handlers acknowledge by clearing bit 16 or 17 with a read-modify-write |
| `INTSTAT_OFF` | 0x10 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Interrupt status word; the dispatcher reads its low 32 bits ANDed with the saved mask |
| `IRQ_BUS_ERR_A` | 0x14 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bus-error input; one kernel handler serves this input and input 21. Which input the silicon raises for CPU errors is unknown |
| `IRQ_BUS_ERR_B` | 0x15 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bus-error input; see IRQ_BUS_ERR_A |
| `IRQ_FIFO_HIGH` | 0x1a | established | shipped IRIX O2 graphics objects, per docs/crime.md | Renderer interface buffer above its full level |
| `IRQ_FIFO_LOW` | 0x19 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Renderer interface buffer at or below its empty level |
| `IRQ_GBE_LINE_2` | 0x12 | established | shipped IRIX O2 graphics objects, per docs/crime.md | GBE line 2; IRIX does not use it |
| `IRQ_GBE_LINE_3` | 0x13 | established | shipped IRIX O2 graphics objects, per docs/crime.md | GBE line 3; IRIX does not use it |
| `IRQ_MACE_ISA_A` | 0x4 | established | shipped IRIX O2 graphics objects, per docs/crime.md | MACE ISA interrupt input; IRIX registers 4 or 5 |
| `IRQ_MACE_ISA_B` | 0x5 | established | shipped IRIX O2 graphics objects, per docs/crime.md | MACE ISA interrupt input; IRIX registers 4 or 5 |
| `IRQ_PREBLANK` | 0x11 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Pre-blank, from GBE line 1 |
| `IRQ_RETRACE` | 0x10 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Vertical retrace, from GBE line 0 |
| `IRQ_SOFT_A` | 0x1c | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Software interrupt registered by IRIX at priority 1. Its role is inferred from the software-interrupt word |
| `IRQ_SOFT_B` | 0x1d | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Software interrupt registered by IRIX at priority 2. Its role is inferred from the software-interrupt word |
| `LINEAR_WINDOW_MASK` | 0x1ffff | established | shipped IRIX O2 graphics objects, per docs/crime.md | Mask on the linear source address of a draw; the linear window is 128 KiB |
| `LINE_MATCH_LATCHES` | 0x4 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Line-match interrupts that latch in CRIME; only software clears them there |
| `LIN_TLB_MAX_PAGES` | 0x20 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Most pages a pinned user buffer may have when it is loaded into a linear TLB |
| `LIN_TLB_REGS` | 0x10 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Registers in each linear TLB (A and B), two 32-bit entries each |
| `LIN_TLB_VALID` | 0x80000000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit ORed into each linear TLB entry; the page frame number is in the low bits |
| `LOGIC_OP_OFF` | 0x21b0 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Logic operation, GL op minus GL_CLEAR, in 16 values |
| `MACE_WINDOW_ADDR` | 0x18000000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Start of the MACE window, which runs to 0x1fffffff |
| `MEM_CTL_OFF` | 0x200 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Memory control word; 1 enables and 0 disables. The kernel never writes its bank or refresh registers |
| `MEM_ERR_STATUS_OFF` | 0x250 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Memory error status; the capture routine clears it by writing zero |
| `MTE_HOST_CHUNK_BYTES` | 0x20000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Largest chunk of a kernel host-memory clear or copy |
| `MTE_MODE_HOST_CLEAR` | 0x11 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Kernel host-memory clear: clear, linear A, destination ECC |
| `MTE_MODE_HOST_COPY` | 0x897 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Kernel host-memory copy: copy, linear A to linear B, both ECC bits |
| `MTE_MODE_X_COPY` | 0x820 | established | shipped IRIX O2 graphics objects, per docs/crime.md | MTE mode the X server uses for a copy: copy, source TLB B, destination TLB A, 8-bit |
| `MTE_X_BLOCK_BYTES` | 0x100 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Width in bytes of the whole-row blocks the X server sends to the MTE |
| `MTE_X_ROWS_16` | 0x10 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Rows in a block at 16 bits per pixel |
| `MTE_X_ROWS_32` | 0x8 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Rows in a block at 32 bits per pixel |
| `NOECC_ALIAS_ADDR` | 0x80000000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | No-ECC alias of memory, 1 GiB |
| `PAGE_BYTES` | 0x1000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Size of a linear TLB page frame, from the frame numbering |
| `PIXFLUSH_OFF` | 0x21f8 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Pixel-pipeline flush; a zero write, after which the library polls the idle bit |
| `PIXPIPE_NULL_OFF` | 0x21f0 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Null pixel-pipeline register, which the kernel treats as global |
| `PIXXFER_POS_OFF` | 0x20a0 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Pixel-transfer screen position: x in bits 26:16, y in bits 10:0 |
| `PIXXFER_VTX_OFF` | 0x2070 | established | shipped IRIX O2 graphics objects, per docs/crime.md | 16-bit X vertex form of pixel DMA, written by a caller bound to the engine |
| `PIXXFER_YSTEP_OFF` | 0x20ac | established | shipped IRIX O2 graphics objects, per docs/crime.md | Pixel-transfer source Y step, written by an unbound caller as (zoom - 1) << 28 \| (stride + pixel size - width) |
| `PRIMITIVE_OFF` | 0x2060 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Primitive type and edge word |
| `PRIM_RECT` | 0x3020000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Primitive word for rectangles: depth clears and the X server's copies |
| `PRIM_TRIANGLE` | 0x2000000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Primitive word for triangles; the edge number is ORed in at bit 16 |
| `RENDER_COUNT_INIT` | 0xa | established | shipped IRIX O2 graphics objects, per docs/crime.md | Count field value in every control word the kernel writes |
| `RENDER_CTL_INIT` | 0xfefff0a | established | shipped IRIX O2 graphics objects, per docs/crime.md | First value the kernel writes to the control word: full 127, empty 63, stall 126, count 10 |
| `RENDER_CTL_OFF` | 0x400 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Renderer interface-buffer control word: count, stall, empty and full thresholds |
| `RENDER_EMPTY_SHIFT` | 0xe | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit position of the empty field |
| `RENDER_FIELD_MASK` | 0x7f | established | shipped IRIX O2 graphics objects, per docs/crime.md | Mask for one 7-bit threshold field |
| `RENDER_FIFO_DEPTH` | 0x40 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Interface-buffer depth. The kernel reports overflow above 64 and walks the buffer as a ring of 64; the specification prints 128-entry arrays |
| `RENDER_FLUSH_OFF` | 0x4008 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Written with zero after TLB loads, context switches and clip validation; the write discards entries after the start pointer |
| `RENDER_FULL_SHIFT` | 0x15 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit position of the full field |
| `RENDER_OCC_SHIFT` | 0x12 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit position of the occupancy field of the status word, 7 bits wide |
| `RENDER_R10K_EMPTY` | 0x0 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Empty threshold for R10000-family and RM7000 modules |
| `RENDER_R10K_FULL` | 0x14 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Full threshold the kernel writes for R10000-family and RM7000 modules |
| `RENDER_R10K_STALL` | 0x13 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Stall threshold for R10000-family and RM7000 modules |
| `RENDER_R5K_EMPTY` | 0x18 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Empty threshold for the other R5000-class modules |
| `RENDER_R5K_FULL` | 0x30 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Full threshold for the other R5000-class modules |
| `RENDER_R5K_STALL` | 0x2f | established | shipped IRIX O2 graphics objects, per docs/crime.md | Stall threshold for the other R5000-class modules |
| `RENDER_STALL_SHIFT` | 0x7 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit position of the stall field; each of the four fields is 7 bits wide |
| `RENDER_START_MASK` | 0x3f | established | shipped IRIX O2 graphics objects, per docs/crime.md | Start pointer in bits 5:0 of the status word, read during context save |
| `RENDER_STATUS_IDLE_BIT` | 0x1c | established | shipped IRIX O2 graphics objects, per docs/crime.md | Idle bit of the renderer status word |
| `RENDER_STATUS_OFF` | 0x4000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Renderer interface-buffer status word |
| `RENDER_WPTR_SHIFT` | 0x6 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Write pointer bit position in the status word, 6 bits wide |
| `SOFTINT_OFF` | 0x20 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Software interrupt word, which raises inputs 28 and 29 |
| `START_ALIAS_OFF` | 0x800 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Added to a register's offset to make a write commit the primitive. The final write of each primitive uses the alias of whichever register is written last |
| `STENCIL_MASK_OFF` | 0x21e8 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Stencil write mask |
| `STENCIL_MODE_OFF` | 0x21e0 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Stencil mode: reference, value mask, function and three operations |
| `TEXTURE_MODE_OFF` | 0x2110 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Texture mode: filter codes, wrap codes, environment function, level bits and the subtile flag |
| `TEX_LOG2_BYTES_SHIFT` | 0x12 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit position of log2 bytes per texel, 2 bits wide |
| `TEX_MAX_TEXELS` | 0x400 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Largest texture dimension the format record leaves room for, from the coordinate scale |
| `TEX_SUBTILE_BIT` | 0x18 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Texture.mode bit that selects subtiled texels |
| `TEX_SUBTILE_CELL_BYTES` | 0x20 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Cell size of a subtiled texture, from the library's address stepping |
| `TEX_TILE_FLAG` | 0x8000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit ORed into each texture tile entry |
| `TEX_TLB_REGS` | 0x1c | established | shipped IRIX O2 graphics objects, per docs/crime.md | Registers in the texture TLB, written as 28 doublewords on each texture bind |
| `TEX_TYPE_SHIFT` | 0x14 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Bit position of the texel type code, 4 bits wide |
| `TILE_BYTES` | 0x10000 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Frame-buffer tile size; the kernel's 16-page mapping agrees with it |
| `TILE_PAGES` | 0x10 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Page frames in one 64 KiB tile. The kernel maps tile entry v as frames v << 4 through (v << 4) + 15 |
| `TILE_ROWS` | 0x80 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Rows in one frame-buffer tile |
| `TILE_ROW_BYTES` | 0x200 | inferred | shipped IRIX O2 graphics objects, per docs/crime.md | Bytes in one row of a frame-buffer tile |
| `TIME_OFF` | 0x38 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Free-running time counter; IRIX reads its low word |
| `TIME_RESOLUTION` | 0x3a98 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Resolution IRIX states for the time counter. The unit is inferred to be picoseconds, a 66.67 MHz tick |
| `VTX_DEPTH_BASE` | 0x840000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Biased binary32 vertex word for depth clears; the same bias as triangles in bits 18:0 |
| `VTX_STEP` | 0x40 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Scale applied to a coordinate before it is ORed into a vertex word |
| `VTX_TRI_BASE` | 0x48040000 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Biased binary32 vertex word for triangles; each step adds 64 times the coordinate |
| `WATCHDOG_OFF` | 0x30 | established | shipped IRIX O2 graphics objects, per docs/crime.md | Watchdog word; IRIX writes zero at start-up and never arms it |
