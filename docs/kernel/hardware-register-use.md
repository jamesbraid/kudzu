# How the IRIX kernel uses registers owned by other repositories

The register maps for the hardware that IRIX drives (the hub, the PCI bridge, the one-wire parts, the R10000 timer, and the O2's CRIME, GBE and MACE video blocks) live in the repositories that describe that hardware. This page keeps the other half: what the shipped kernels, libraries and X modules do with those registers. Each section names the repository and unit that now holds the register facts. Grades are the ones the facts carry in those repositories; this page repeats them as the kernel readings stated them.

The reasoning and the binary addresses are in the topic notes this page links to ([interrupts](../interrupts.md), [page migration](../page-migration.md), [IP35 hub errors](../ip35-hub-errors.md), [IP35 PCI bridge registers](../ip35-pcibr-registers.md), [number-in-a-can reads](../nic.md), [CRIME](../crime.md), [GBE](../gbe.md), [MACE video](../mace-video.md)). Offsets in the notes below are the kernel's own and are relative to the block each section names.

## Hub interrupt registers (Origin 200/2000)

Register facts: chips repository, units `hub` (census registers) and `hub_pi_extra` (layouts and constants).

Node hub interrupt pending and mask registers.

| Item | grade | kernel use |
|---|---|---|
| `INT_PEND_MOD_ALT_OFFSET` | established | a processor whose slice number has bit 1 set reaches int_pend_mod at this offset in place of 0x90 |
| `LOCAL_WINDOW_BASE` | established | physical base of the processor's own hub window; no node shift |
| `NODE_SHIFT` | established | physical node number shift |
| `NODE_WINDOW_BASE` | established | physical base of a named node's window |

## Hub memory and directory migration (Origin 200/2000)

Register facts: chips repository, units `hub` and `hub_md_extra`.

Origin 200/2000 hub memory-directory migration registers.

| Item | grade | kernel use |
|---|---|---|
| `mig_diff_thresh` | established | Bit 63 is the valid bit: set when the kernel arms the threshold, cleared to disarm it while a candidate is handled. See MIG_DIFF_THRESH_VALID_BIT. |
| `mig_value_thresh` | inferred | Bit 63 is the valid bit, set to arm and cleared to disarm. See MIG_VALUE_THRESH_VALID_BIT. |
| `mig_candidate` | established | Read-to-clear alias of the candidate register. The kernel reads it in a loop until the valid bit (bit 63) is clear. The public Linux hub header calls 0x40 the candidate register and 0x48 its read-to-clear alias. The kernel reads only 0x48, so this is that alias; that a read consumes the candidate is inferred from the name and the loop. Nothing accesses 0x40. See MIG_CANDIDATE_VALID_BIT. |
| `MIG_CANDIDATE_VALID_BIT` | established | the loop ends when it is clear |
| `MIG_DIFF_THRESH_VALID_BIT` | established | set to arm, cleared to disarm |
| `MIG_VALUE_THRESH_VALID_BIT` | established | set to arm, cleared to disarm |
| `NODE_SHIFT` | established | physical node number shift |
| `REFCNT_COUNT_MASK_PREMIUM` | established | nineteen bits, premium memory; which memory is premium is inferred |
| `REFCNT_COUNT_MASK_STANDARD` | established | eleven bits, standard memory |
| `REFCNT_NODE_STRIDE` | established | bytes between the words of consecutive nodes for one page |
| `WINDOW_BASE` | established | physical base of the block for node 0 |

## Hub memory and directory migration (Origin 300/350)

Register facts: chips repository, unit `bedrock_md`, with `bedrock_extra` for the node shift.

Origin 300 hub memory-directory migration registers.

| Item | grade | kernel use |
|---|---|---|
| `mig_diff_thresh` | established | The mode control sits in a different window of the hub, not in the block above: its offset is the local register base of the public vendor header subtracted from the address the notes record. No shipped kernel reads or writes it, so only the offset is stated. See MIG_CONFIG_LOCAL_OFFSET. |
| `MIG_CANDIDATE_CLR_OFFSET` | unknown | access and width; the public vendor header names it the read-to-clear candidate register, no shipped kernel accesses it |
| `MIG_CANDIDATE_OFFSET` | unknown | access and width; named only by a public vendor header, no shipped kernel accesses it |
| `MIG_CONFIG_LOCAL_OFFSET` | inferred | offset from the hub-local register base, by subtraction against a public vendor header; unknown: access, width and fields, no shipped kernel accesses it |
| `MIG_DIFF_THRESH_VALID_BIT` | established | set to arm, cleared to disarm |
| `NODE_SHIFT` | established | physical node number shift |
| `WINDOW_BASE_UNITS` | established | physical base of the block for node 0, in units of 1 << HUBMDB_WINDOW_BASE_UNIT_SHIFT bytes; split because the full value is also a boot-region address the publish check refuses |

## Hub network interface

Register facts: chips repository, units `hub_ni_extra` (Origin 200/2000) and `bedrock_ni` (Origin 300/350).

Node hub network interface words: node number, link state, link reset and port error.

| Item | grade | kernel use |
|---|---|---|
| `NSRI` | established | The low 32 bits of the Origin 200/2000 network status word. |
| `O200_STATUS_OFFSET` | established | Origin 200/2000 network status word, carries the node number |
| `O300_ERROR_CLEAR_OFFSET` | established | Origin 300 port error clear word; the kernel reads it, then stores zero to the error word; whether the read clears anything is unknown |
| `O300_ERROR_LATCHED_HI` | established | top bit of the range of latched error bits the kernel tests |
| `O300_ERROR_LATCHED_LO` | established | bottom bit of that range |
| `O300_ERROR_LINK_RESET_BIT` | established | link reset caused by another partition, with the enable word zero; that the chip latches it when a reset is refused is unknown |
| `O300_ERROR_REPORT_A_OFFSET` | established | read through the local alias and printed in the link error report; role unknown |
| `O300_ERROR_REPORT_B_OFFSET` | established | as report word A |
| `O300_ERROR_REPORT_C_OFFSET` | established | as report word A |
| `O300_PORT_RESET_OFFSET` | established | Origin 300 port reset word; the kernel writes the value 6 |
| `O300_RESET_ENABLE_OFFSET` | established | Origin 300 reset enable word; the kernel writes 1; bit 0 is the one that matters (datasheet "reset ok" is inferred) |
| `O300_STATUS_OFFSET` | established | Origin 300 network status word; bits 1:0 are tested as link state, bits 2:1 equal to 2 starts a link-reset wait |

## Hub crosstalk-side window (Origin 300/350)

Register facts: chips repository, unit `bedrock_ii`.

Origin 300 hub crosstalk-side window: the link status doubleword, the register tested by the error path, the request-buffer table the I/O error scan reads and the widget part numbers the kernel compares.

| Item | grade | kernel use |
|---|---|---|
| `llp_csr` | established | Link control and status doubleword that the early widget probe reads. Only bit 13 is described. With it clear, the probe returns -1 for the part, revision and manufacturer without touching the widget. Whether the bit means link up is inferred from how the I/O error interrupt set-up uses it. Field `bit13` (bits 13:13, established): the early widget probe tests this bit; the I/O error interrupt set-up tests it before choosing a handler path |
| `flags` | established | The register the kernel's error path tests before it goes on to the widget error handler. Field `bit4` (bits 4:4, established): see bit0; role unknown Field `bit0` (bits 0:0, established): tested with bit 4; the error routine continues only when one of the two is set; role unknown |
| `crb_a` | established | Request-buffer table. Entry i (0 to 14) has three 64-bit words, crb_a, crb_b and crb_c, at the register offsets below plus the stride times i; the scan reads all fifteen entries in order. See CRB_STRIDE and CRB_COUNT. Field `bit55` (bits 55:55, established): with the bit set the type selects the handling case; with it clear bit49 and bit51 are tested Field `type` (bits 54:52, established): error type, 0 to 7; with bit55 set it picks one of eight handling cases Field `bit51` (bits 51:51, established): reported as an uncorrectable data error on data from the network; tested only when bit55 is clear Field `bit50` (bits 50:50, established): the scan skips the entry unless this bit is set Field `bit49` (bits 49:49, established): reported as a crosstalk packet that arrived with its error bit set; tested only when bit55 is clear Field `bit0` (bits 0:0, established): selects which of two forms of the error record the scan builds for a block-transfer error; role unknown |
| `crb_b` | established | Field `engine_alt` (bits 58:58, established): selects the block-transfer engine when crb_c bit48 is set Field `sub` (bits 42:41, established): see kind Field `kind` (bits 32:30, established): with the value 1 or 5 and sub 1 or 2, the entry is treated as a block-transfer-engine error; bit 32 alone also selects the engine when crb_c bit48 is clear |
| `crb_c` | established | Field `bit48` (bits 48:48, established): with the bit set the entry is treated as a block-transfer-engine error |
| `CRB_COUNT` | established | entries the scan reads |
| `CRB_STRIDE` | established | bytes between entries |
| `NODE_SHIFT` | established | node number shift |
| `WIDGET_PART_C104` | established | widget part number the Origin 300/350 kernels compare by value, read from the widget identification word; compared in both releases read and in no Origin 200/2000 kernel; role unknown |
| `WIDGET_PART_C105` | established | widget part number, compared like C104; the kernel's per-widget set-up takes the same programmed-I/O mode call for both as for its graphics parts; no driver registers either part |
| `WINDOW_BASE` | established | physical base of the window for node 0 |

## PCI bridge widget words

Register facts: chips repository, unit `bridge_extra`; the registers themselves are in `bridge` and `xbridge`.

PCI bridge widget identification word, configuration address layout and interrupt address registers.

| Item | grade | kernel use |
|---|---|---|
| `WID` | established | The widget identification word. |
| `CFG_FUNC_SHIFT` | established | function shifted left by this |
| `CFG_SLOTS_32` | established | slots 0 to 7 on the 32-bit layout |
| `CFG_SLOT_BIAS_64` | established | on the 64-bit layouts the slot is incremented before use; the accepted range is then 1 to 4 |
| `CFG_SLOT_SHIFT` | established | configuration address is window plus slot shifted left by this |
| `CFG_WINDOW32_OFFSET` | established | added on the 32-bit layout only, with the register offset first rounded down to a multiple of four |
| `INTR_ADDR32_HI` | established | the 32-bit layout replaces bits 17:8 of the word |
| `INTR_ADDR32_OFFSET` | established | first interrupt address register, 32-bit layout; a 32-bit word |
| `INTR_ADDR32_SRC_HI` | established | the 32-bit layout keeps source address bits 47:38 |
| `INTR_ADDR64_OFFSET` | established | first interrupt address register, 64-bit layouts; its low 48 bits hold the address |
| `INTR_COUNT` | established | interrupt numbers 0 to 7 |
| `INTR_STRIDE` | established | byte stride of the interrupt address registers on every layout |
| `WID_OFFSET` | established | byte offset of the widget identification word in the window |

## One-wire access to the identity parts

Register facts: chips repository, units `ioc3_extra`, `bedrock_extra` and `hub_md_extra`; the Ethernet address record layout stays here as `defs/nic_record.toml`.

Bit-serial control word for the one-wire identity parts and the hub address that reaches the same bus.

| Item | grade | kernel use |
|---|---|---|
| `GPCR_S` | inferred | The I/O controller's general-purpose control word, written through its set alias. Before reading the address part the routine stores 0x200000 to the register address its caller passes, when that address is not null; with a null address it prints that it is bypassing the enable and goes on. The word is written whole, so bit 21 is the only bit the routine sets. That the store enables the one-wire path to the part is read from the routine's own message, and that a set alias leaves other bits alone is inferred from its name. |
| `MCR` | established | The 32-bit microlan control word. |
| `GPCR_S_NIC_ENABLE_WORD` | established | the value stored |
| `HUB_ACCESS_NODE_SHIFT` | established | node number shift |
| `HUB_ACCESS_OFFSET` | established | offset of the register from the hub's local-window base |
| `HUB_ACCESS_PHYS_LOW` | established | physical address of the register for node 0 |
| `MAC_CRC16_RESIDUE` | established | CRC-16 residue over record bytes 1 to 13 |
| `MAC_ID_BYTE0` | established | required value of record byte 0 |
| `MAC_ID_BYTE1` | established | required value of record byte 1 |
| `MAC_RECORD_LEN` | established | bytes read from the part |

## R10000 Count and Compare timer

Register facts: chips repository, unit `r10000_cp0`.

R10000 Count and Compare timer interrupt, from the published manual.

| Item | grade | kernel use |
|---|---|---|
| `R10K_CP0_COMPARE` | established | established: Compare register number in coprocessor 0 |
| `R10K_CP0_COUNT` | established | established: Count register number in coprocessor 0 |

## O2 CRIME

Register facts: crime repository, units `crime`, `re` and `mte`; the kernel's pixel-DMA request constants stay here as `defs/crime_dma.toml`.

Offsets, identity values, interrupt inputs and renderer field layouts that the O2 kernel, OpenGL library and X server use on the CRIME block. Prose and the reasoning are in docs/crime.md.

| Item | grade | kernel use |
|---|---|---|
| `id` | established | Identity byte at the start of the CRIME block. The graphics probe reads its low byte and accepts the part only when bits 7:4 are the accepted family and the byte is not the refused value. Field `family` (bits 7:4, established): family nibble; the probe accepts 0xA |
| `texture_format` | established | Texture format record, written by the OpenGL library for each texture. It carries the log2 sizes of the texture and the coordinate masks. The width of the register is taken from the top field, which reaches bit 47. Field `width_inv` (bits 47:44, established): 10 minus log2 of the texture width Field `height_inv` (bits 43:40, established): 10 minus log2 of the texture height Field `width_log2` (bits 39:36, established): log2 of the texture width Field `height_log2` (bits 35:32, established): log2 of the texture height Field `width_mask` (bits 31:16, established): (1 << (log2 width + 4)) - 1 Field `height_mask` (bits 15:0, established): (1 << (log2 height + 4)) - 1 |
| `IB_ADDR` | established | Interface-buffer address word, as the kernel decodes it in software. Bit 45 marks the stop entry. Field `reg_index` (bits 39:32, established): Register index Field `group` (bits 42:40, established): Register group Field `lane` (bits 44:43, established): Lane code Field `stop` (bits 45:45, established): Stop marker, used by the kernel's decoder |
| `BLOCK_ADDR` | established | Physical address of the CRIME block, as the graphics probe reads it |
| `GBE_WINDOW_ADDR` | inferred | CPU window the specification gives for the GBE block. Not confirmed by the binaries; the GBE page keeps the GBE block base as unknown |
| `FRAME_APERTURE_ADDR` | inferred | Frame-buffer aperture, 32 MiB |
| `DEPTH_APERTURE_ADDR` | inferred | Depth-buffer aperture, 32 MiB |
| `MACE_WINDOW_ADDR` | inferred | Start of the MACE window, which runs to 0x1fffffff |
| `HIGH_ALIAS_ADDR` | inferred | High alias of low memory, 512 MiB; the specification's table marks its responder with a question mark |
| `FULL_MEM_ADDR` | inferred | Full memory window, 1 GiB |
| `NOECC_ALIAS_ADDR` | inferred | No-ECC alias of memory, 1 GiB |
| `ID_FAMILY_ACCEPTED` | established | Value of bits 7:4 of the identity byte that the graphics probe accepts |
| `ID_REFUSED_BYTE` | established | Identity byte the graphics probe refuses even though its family is accepted |
| `ID_SPEC_15` | inferred | Identity byte the specification gives for CRIME 1.5. The graphics probe refuses it |
| `CONTROL_OFF` | established | CONTROL word; the early-reset code writes bits 13:12 and 9 of it |
| `INTSTAT_OFF` | established | Interrupt status word; the dispatcher reads its low 32 bits ANDed with the saved mask |
| `INTMASK_OFF` | established | Interrupt mask word; the entry path saves it and installs a per-priority mask |
| `SOFTINT_OFF` | inferred | Software interrupt word, which raises inputs 28 and 29 |
| `INTR_EDGE_OFF` | established | Interrupt edge word; the retrace and pre-blank handlers acknowledge by clearing bit 16 or 17 with a read-modify-write |
| `WATCHDOG_OFF` | established | Watchdog word; IRIX writes zero at start-up and never arms it |
| `TIME_OFF` | established | Free-running time counter; IRIX reads its low word |
| `TIME_RESOLUTION` | established | Resolution IRIX states for the time counter. The unit is inferred to be picoseconds, a 66.67 MHz tick |
| `CPU_ERR_STATUS_OFF` | established | CPU error status; the capture routine clears it by writing zero |
| `MEM_CTL_OFF` | established | Memory control word; 1 enables and 0 disables. The kernel never writes its bank or refresh registers |
| `MEM_ERR_STATUS_OFF` | established | Memory error status; the capture routine clears it by writing zero |
| `ENGINE_RESET_OFF` | established | Engine reset word; the kernel writes zero, waits 10 microseconds and rewrites the interface-buffer control word, named IB_CTL in the reading. Whether IB_CTL is the word at RENDER_CTL_OFF is inferred. The reset role of this write is inferred |
| `RENDER_STATUS_OFF` | established | Renderer interface-buffer status word |
| `RENDER_CTL_OFF` | established | Renderer interface-buffer control word: count, stall, empty and full thresholds |
| `RENDER_FLUSH_OFF` | established | Written with zero after TLB loads, context switches and clip validation; the write discards entries after the start pointer |
| `START_ALIAS_OFF` | established | Added to a register's offset to make a write commit the primitive. The final write of each primitive uses the alias of whichever register is written last |
| `RENDER_CTL_INIT` | established | First value the kernel writes to the control word: full 127, empty 63, stall 126, count 10 |
| `RENDER_COUNT_INIT` | established | Count field value in every control word the kernel writes |
| `RENDER_STALL_SHIFT` | established | Bit position of the stall field; each of the four fields is 7 bits wide |
| `RENDER_EMPTY_SHIFT` | established | Bit position of the empty field |
| `RENDER_FULL_SHIFT` | established | Bit position of the full field |
| `RENDER_FIELD_MASK` | established | Mask for one 7-bit threshold field |
| `RENDER_R10K_FULL` | established | Full threshold the kernel writes for R10000-family and RM7000 modules |
| `RENDER_R10K_EMPTY` | established | Empty threshold for R10000-family and RM7000 modules |
| `RENDER_R10K_STALL` | established | Stall threshold for R10000-family and RM7000 modules |
| `RENDER_R5K_FULL` | established | Full threshold for the other R5000-class modules |
| `RENDER_R5K_EMPTY` | established | Empty threshold for the other R5000-class modules |
| `RENDER_R5K_STALL` | established | Stall threshold for the other R5000-class modules |
| `RENDER_STATUS_IDLE_BIT` | established | Idle bit of the renderer status word |
| `RENDER_OCC_SHIFT` | established | Bit position of the occupancy field of the status word, 7 bits wide |
| `RENDER_START_MASK` | established | Start pointer in bits 5:0 of the status word, read during context save |
| `RENDER_WPTR_SHIFT` | established | Write pointer bit position in the status word, 6 bits wide |
| `RENDER_FIFO_DEPTH` | inferred | Interface-buffer depth. The kernel reports overflow above 64 and walks the buffer as a ring of 64; the specification prints 128-entry arrays |
| `IRQ_MACE_ISA_A` | established | MACE ISA interrupt input; IRIX registers 4 or 5 |
| `IRQ_MACE_ISA_B` | established | MACE ISA interrupt input; IRIX registers 4 or 5 |
| `IRQ_RETRACE` | established | Vertical retrace, from GBE line 0 |
| `IRQ_PREBLANK` | established | Pre-blank, from GBE line 1 |
| `IRQ_GBE_LINE_2` | established | GBE line 2; IRIX does not use it |
| `IRQ_GBE_LINE_3` | established | GBE line 3; IRIX does not use it |
| `IRQ_BUS_ERR_A` | established | Bus-error input; one kernel handler serves this input and input 21. Which input the silicon raises for CPU errors is unknown |
| `IRQ_BUS_ERR_B` | established | Bus-error input; see IRQ_BUS_ERR_A |
| `IRQ_FIFO_LOW` | established | Renderer interface buffer at or below its empty level |
| `IRQ_FIFO_HIGH` | established | Renderer interface buffer above its full level |
| `IRQ_SOFT_A` | inferred | Software interrupt registered by IRIX at priority 1. Its role is inferred from the software-interrupt word |
| `IRQ_SOFT_B` | inferred | Software interrupt registered by IRIX at priority 2. Its role is inferred from the software-interrupt word |
| `LINE_MATCH_LATCHES` | inferred | Line-match interrupts that latch in CRIME; only software clears them there |
| `FB_TLB_REGS` | established | Registers in each frame-buffer TLB (A, B and C), each holding four 16-bit tile entries |
| `TEX_TLB_REGS` | established | Registers in the texture TLB, written as 28 doublewords on each texture bind |
| `CID_TLB_REGS` | established | Registers in the CID tile-list TLB, four 16-bit entries each |
| `LIN_TLB_REGS` | established | Registers in each linear TLB (A and B), two 32-bit entries each |
| `LIN_TLB_MAX_PAGES` | established | Most pages a pinned user buffer may have when it is loaded into a linear TLB |
| `LIN_TLB_VALID` | established | Bit ORed into each linear TLB entry; the page frame number is in the low bits |
| `TEX_TILE_FLAG` | established | Bit ORed into each texture tile entry |
| `TILE_PAGES` | established | Page frames in one 64 KiB tile. The kernel maps tile entry v as frames v << 4 through (v << 4) + 15 |
| `PAGE_BYTES` | inferred | Size of a linear TLB page frame, from the frame numbering |
| `TILE_BYTES` | inferred | Frame-buffer tile size; the kernel's 16-page mapping agrees with it |
| `TILE_ROWS` | inferred | Rows in one frame-buffer tile |
| `TILE_ROW_BYTES` | inferred | Bytes in one row of a frame-buffer tile |
| `TEX_MAX_TEXELS` | inferred | Largest texture dimension the format record leaves room for, from the coordinate scale |
| `DRAWMODE_OFF` | established | DrawMode; bit 9 is logic-op enable, bit 8 dither, bit 14 hardware fog, bit 12 antialiasing |
| `PRIMITIVE_OFF` | established | Primitive type and edge word |
| `PIXPIPE_NULL_OFF` | established | Null pixel-pipeline register, which the kernel treats as global |
| `PIXFLUSH_OFF` | established | Pixel-pipeline flush; a zero write, after which the library polls the idle bit |
| `TEXTURE_MODE_OFF` | established | Texture mode: filter codes, wrap codes, environment function, level bits and the subtile flag |
| `FOG_COLOR_OFF` | established | Fog colour, three 8-bit components in bits 31:8 |
| `AA_LINE_OFF` | established | Antialiased-line register: minor-axis slope in bits 31:16, start position with 8 fraction bits in bits 15:0 |
| `ALPHA_TEST_OFF` | established | Alpha-test register |
| `BLEND_FUNC_OFF` | established | Blend function register |
| `LOGIC_OP_OFF` | established | Logic operation, GL op minus GL_CLEAR, in 16 values |
| `DEPTH_FUNC_OFF` | established | Depth function in bits 27:25, a stencil-only flag in bit 24 and the clear depth in bits 23:0 |
| `STENCIL_MODE_OFF` | established | Stencil mode: reference, value mask, function and three operations |
| `STENCIL_MASK_OFF` | established | Stencil write mask |
| `PIXXFER_POS_OFF` | established | Pixel-transfer screen position: x in bits 26:16, y in bits 10:0 |
| `PIXXFER_YSTEP_OFF` | established | Pixel-transfer source Y step, written by an unbound caller as (zoom - 1) << 28 \| (stride + pixel size - width) |
| `PIXXFER_VTX_OFF` | established | 16-bit X vertex form of pixel DMA, written by a caller bound to the engine |
| `LINEAR_WINDOW_MASK` | established | Mask on the linear source address of a draw; the linear window is 128 KiB |
| `PRIM_TRIANGLE` | established | Primitive word for triangles; the edge number is ORed in at bit 16 |
| `PRIM_RECT` | established | Primitive word for rectangles: depth clears and the X server's copies |
| `VTX_TRI_BASE` | established | Biased binary32 vertex word for triangles; each step adds 64 times the coordinate |
| `VTX_DEPTH_BASE` | established | Biased binary32 vertex word for depth clears; the same bias as triangles in bits 18:0 |
| `VTX_STEP` | established | Scale applied to a coordinate before it is ORed into a vertex word |
| `GL_VERTEX_BIAS` | established | Bias the library applies to GL vertex halves in pixel DMA |
| `GL_UNBOUND_BIAS` | established | Bias the kernel removes from coordinates for a caller that is not bound to the engine |
| `DMA_REQ_BYTES` | established | Size of the kernel pixel-DMA request |
| `DMA_POLL_LIMIT` | established | Polls the kernel makes for idle before the pixel DMA gives up |
| `DMA_IOCTL` | inferred | Request number of the kernel pixel-DMA ioctl |
| `MTE_MODE_X_COPY` | established | MTE mode the X server uses for a copy: copy, source TLB B, destination TLB A, 8-bit |
| `MTE_MODE_HOST_CLEAR` | established | Kernel host-memory clear: clear, linear A, destination ECC |
| `MTE_MODE_HOST_COPY` | established | Kernel host-memory copy: copy, linear A to linear B, both ECC bits |
| `MTE_HOST_CHUNK_BYTES` | established | Largest chunk of a kernel host-memory clear or copy |
| `MTE_X_BLOCK_BYTES` | established | Width in bytes of the whole-row blocks the X server sends to the MTE |
| `MTE_X_ROWS_16` | established | Rows in a block at 16 bits per pixel |
| `MTE_X_ROWS_32` | established | Rows in a block at 32 bits per pixel |
| `TEX_SUBTILE_BIT` | established | Texture.mode bit that selects subtiled texels |
| `TEX_TYPE_SHIFT` | established | Bit position of the texel type code, 4 bits wide |
| `TEX_LOG2_BYTES_SHIFT` | established | Bit position of log2 bytes per texel, 2 bits wide |
| `TEX_SUBTILE_CELL_BYTES` | inferred | Cell size of a subtiled texture, from the library's address stepping |

## O2 GBE display back end

Register facts: crime repository, unit `gbe`; the board description bytes stay here as `defs/gbe_board.toml`.

Register offsets, packed timing words and option-probe constants that the O2 kernel writes on the GBE block.

| Item (gbe) | grade | kernel use |
|---|---|---|
| `vt_vblank` | established | Vertical blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active height and 0xfff. Other stores to this offset are outside what this unit records. Field `hi` (bits 23:12, established): active height Field `lo` (bits 11:0, established): 0xfff in the interlace case |
| `vt_hblank` | established | Horizontal blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active width and 0xfff. Other stores to this offset are outside what this unit records. Field `hi` (bits 23:12, established): active width Field `lo` (bits 11:0, established): 0xfff in the interlace case |
| `timing_flags` | established | Timing flag word. The timing routine reads it, clears bits 1 and 7, and ORs in a value of 0 or 2 for the byte 132 set-up. What bit 1 does to the hardware is unknown. The value is undefined when the option lookup that feeds it fails. |
| `fp_hdrv` | established | Flat-panel horizontal drive word. The value depends on the board's panel set-up byte and on whether a dual-channel option is fitted. The table of values is in docs/gbe.md. Field `hi` (bits 23:12, established): 12-bit field; its meaning is unknown Field `lo` (bits 11:0, established): 12-bit field; its meaning is unknown |
| `fp_vdrv` | established | Flat-panel vertical drive word. The value depends on the panel set-up byte and on the option, as the table in docs/gbe.md gives. Field `hi` (bits 23:12, established): 12-bit field; its meaning is unknown Field `lo` (bits 11:0, established): 12-bit field; its meaning is unknown |
| `fp_de` | established | Flat-panel data-enable word. The timing routine writes it only for the default set-up and panel set-ups 130 and 131, and only when no dual-channel option is fitted. It is not written otherwise. Field `hi` (bits 23:12, established): 12-bit field; its meaning is unknown Field `lo` (bits 11:0, established): 12-bit field; its meaning is unknown |
| `dotclk` | established | Dot-clock (PLL) word. The timing routine clears the run bit, writes the timing words, then writes this word with the run bit set and waits 11 milliseconds. The coefficient fields hold M minus one, N minus one and P, as the kernel stores them. Field `pll_m` (bits 7:0, established): PLL multiplier M, stored as M minus one Field `pll_n` (bits 13:8, established): PLL divider N, stored as N minus one Field `pll_p` (bits 15:14, established): PLL post-divider P, stored unchanged Field `run` (bits 20:20, established): cleared before a new value and set with it; what it does in the hardware is inferred (see DOTCLK_RUN_BIT) |
| `vt_xy` | inferred | Vertical timing position word, called VT_0 in the reading. The start-up sequence compares its whole unmasked value with the vertical blank start line, and the stop sequence freezes it with the value in VT_XY_FREEZE. The offset follows from the VT_n numbering of the other timing words; the reading does not state it directly. |
| `vt_1` | established | Frame totals. The timing routine writes 0 here, and the start-up sequence writes the totals once the frame buffer is set up. The kernel stores the record's total pixels and total lines, not one less than them; whether the hardware expects one less is unknown. Field `total_pixels` (bits 11:0, established): total pixels per line, as stored Field `total_lines` (bits 23:12, established): total lines per frame, as stored |
| `vt_2` | established | Vertical sync position, in lines. Field `vsync_end` (bits 11:0, established): line where vertical sync ends Field `vsync_start` (bits 23:12, established): line where vertical sync starts |
| `vt_3` | established | Horizontal sync position, in pixels. Field `hsync_end` (bits 11:0, established): pixel where horizontal sync ends Field `hsync_start` (bits 23:12, established): pixel where horizontal sync starts |
| `vt_7` | established | Vertical blank start line in the high field. The low field repeats it when the frame-lock flag bits of the timing record (mask 0x12) are set, and holds 0xfff otherwise. Field `blank_start` (bits 23:12, established): vertical blank start line Field `low` (bits 11:0, established): the blank start line when a frame-lock flag is set, else 0xfff |
| `vt_13` | established | Parked holding 0xffffff before the frame buffer is set up and written last in the start-up sequence. The final value is ((active width + v - 2) mod total pixels) in the low field and v in the high field, where v is the horizontal blank end minus 0x13, with total pixels added first when that is negative. |
| `vt_14` | established | Parked holding 0xffffff and written last in the start-up sequence with the vertical blank start in the low field and the vertical blank end in the high field. |
| `vt_15` | established | Horizontal blank window, in pixels. Field `hblank_end` (bits 11:0, established): pixel where horizontal blank ends Field `hblank_start` (bits 23:12, established): pixel where horizontal blank starts |
| `vt_16` | established | Vertical blank window, in lines. Field `vblank_end` (bits 11:0, established): line where vertical blank ends Field `vblank_start` (bits 23:12, established): line where vertical blank starts |
| `vt_17` | established | Two parts, with bit placement not stated in the reading. The vertical part is minus (vertical blank start minus vertical blank end minus 1) when that difference is positive, else the difference, masked to 12 bits. The horizontal part is the horizontal blank end minus 20, with total pixels added when the blank end is below 20. |
| `vt_18` | established | The vertical part of VT_17 and the horizontal blank end minus 54, with total pixels added when that is negative, then 0x1000 added for one line. The reading gives the 54 as a kernel constant. The 0x1000 step places the line part in bits 23:12 (inferred), so the horizontal part is in bits 11:0 (inferred). |
| `vt_19` | established | The vertical part of VT_17 without the extra line that VT_18 adds, and the horizontal blank end minus 4. Bit placement is inferred as for VT_18. |
| `frm_readback` | established | Live readback of the frame control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the frame control. |
| `ovr_readback` | established | Live readback of the overlay control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the overlay control. |
| `device_id` | established | Identity word the kernel reads first. It accepts the block only when the word reads 0x666; any other value means the kernel leaves the block untouched. |
| `frm_control` | established | Frame-buffer scan-out control. The stop routine saves it, clears its enable bit, and later writes it back with its pointer field bumped by one unit. The pointer field starts at bit 5 (inferred). Field `enable` (bits 0:0, established): cleared by the stop routine; the meaning as a DMA enable is inferred |
| `ovr_control` | established | Overlay control. The stop routine clears its enable bit and leaves the word otherwise unwritten. Field `enable` (bits 0:0, established): cleared by the stop routine; the meaning as a DMA enable is inferred |
| `did_control` | established | Display-ID table control. The stop routine clears its enable bit and leaves the word otherwise unwritten. Field `enable` (bits 16:16, established): cleared by the stop routine; the stop routine also tests it at entry Field `table_hi` (bits 15:0, inferred): Upper address bits of the 64 KiB display-ID table. The specification gives this field; the kernel never writes it |
| `wid` | established | Window-ID table, one 32-bit word per window index. Indices of 32 or more are rejected by the kernel. The kernel writes entry 0 with 3 at text-port set-up. Field `buf` (bits 1:0, inferred): buffer code: 3 buffers both, 0 marks the window to be skipped by the buffer flip, 1 and 2 exchange on a swap. The bit position is inferred from the table values. Field `type` (bits 4:2, inferred): pixel type: 0 indexed 8-bit, 1 indexed 12-bit, 2 RG3B2, 4 RGB5, 5 RGB8. The bit position is inferred from the table values and the compare mask. Field `map_group` (bits 9:5, inferred): colour-map group; the group's first entry is this value times 256. The bit position is inferred from the table values. Field `gamma_disable` (bits 10:10, established): gamma disable. The kernel clears it before a write when the alternative gamma table is in use. Field `aux` (bits 12:11, inferred): Auxiliary code. The kernel's compare mask of 0x7fc leaves it out. The position is from the specification |
| `cursor_pos` | established | Hardware cursor position word. The text port writes 0 at set-up. |
| `cursor_ctrl` | established | Cursor control word. The text port writes 1 after the glyph and colours are loaded. What the bits mean beyond that is unknown. |
| `cursor_color1` | established | Cursor colour 1 in the byte layout red, green, blue, zero. The text port writes 0xffffff00. |
| `cursor_color2` | established | Cursor colour 2 in the byte layout red, green, blue, zero. The text port writes 0xff000000. |
| `cursor_glyph` | established | Cursor bitmap, 64 words. The text port loads words 0 to 31 from a 32-word table and writes 0 to words 32 to 63. Reading the words as a 32 by 32 bitmap at two bits per pixel is inferred; the text port draws with the first 32 words only (inferred). |
| `DID_FRAME` | established | Frame entry of a display-ID table: one per horizontal band of lines. The X module packs it as block << 19 \| offset << 11 \| yend. Field `yend` (bits 10:0, established): last line of the band, counted from the first active line Field `offset` (bits 18:11, established): offset of the band's line table within its block Field `block` (bits 25:19, established): block that holds the band's line table |
| `DID_LINE` | established | Line-table entry of a display-ID table: one per rectangle in a band, sorted by its last column. The X module packs it as index << 11 \| xend. Field `xend` (bits 10:0, established): last column of the rectangle; the last visible entry of a line ends at width minus 1 Field `did` (bits 15:11, established): display (window) ID of the rectangle |
| `DEVICE_ID_VALUE` | established | Value the identity word at offset 0x14 must read for the kernel to accept the block |
| `VT_XY_FREEZE` | established | Value the kernel writes to the vertical timing position word to freeze it; the word's bit 31 is read first as the freeze state |
| `DOTCLK_RUN_BIT` | inferred | Bit of the dot-clock word that the kernel clears before a new value and sets with it. Inferred to run the PLL or its output; the specification leaves it undefined. |
| `DOTCLK_SETTLE_MS` | established | Milliseconds the kernel waits after it writes the dot-clock word with the run bit set |
| `REF_CLOCK_KHZ` | inferred | PLL reference clock behind the dot-clock table. The kernel states none; the source reading takes 20 MHz from a public Linux driver, not re-checked here. |
| `CMFIFO_MASK` | established | Colour-map FIFO occupancy field, bits 5:0. The register that holds it is not recorded here. |
| `CMFIFO_FREE_MIN` | established | Free colour-map FIFO entries the X module waits for after each batch of 33 writes |
| `CMFIFO_BATCH` | established | Colour-map writes the X module issues between FIFO checks |
| `CMAP_ENTRY_BASE` | established | Colour-map entry i is the word at this offset plus 4 times i; the word is red << 24 \| green << 16 \| blue << 8 |
| `TILE_NUMBER_MASK` | established | Mask the kernel applies to a physical tile address before it is written into a tile list |
| `DID_TABLE_BYTES` | established | Size of one display-ID table as the X module fills it. The frame table starts at byte 0 and the line tables start at DID_LINE_TABLE_OFFSET. |
| `DID_LINE_TABLE_OFFSET` | established | Byte offset of the first line table in a display-ID table |
| `DID_BLOCK_BYTES` | established | Size of one block of a display-ID table; line tables are packed in 16-bit units inside a block |
| `DID_BLOCK_ENTRIES` | established | Entries after which packing restarts in the next block |
| `DID_END_BLOCK` | established | Block number that ends a display-ID table; the X module stops filling it there |
| `DID_LAST_XEND` | established | Right-hand column of the last band's line entry, which covers every line below the screen |
| `GBE_LINE_IRQ_BASE` | established | CRIME interrupt bit of GBE line 0; GBE line n raises bit 16 + n. Lines 2 and 3 are the CRIME inputs 18 and 19. |
| `TWO_WIRE_FP_OFF` | established | Register offset of the flat-panel two-wire bus. Bus selector 1 drives it. |
| `TWO_WIRE_MON_OFF` | established | Register offset of the monitor two-wire bus. Every other bus selector drives it. |
| `TWO_WIRE_WAIT_BIT` | established | Bit of the bus register that a start condition waits to read clear. |
| `TWO_WIRE_WAIT_LIMIT` | established | Most delays a start condition polls the bus bit before it gives up. |
| `OPT_PORT_ADDR` | established | Write address of the option port on the flat-panel bus. A read uses the same address with bit 0 set. |
| `OPT_CMD_ID_HIGH` | established | Command written before the first id read. Its id bits come from bits 6 and 5 of the byte read. |
| `OPT_CMD_ID_LOW` | established | Command written before the second id read. Its id bits come from bits 6 and 5 of the byte read. |
| `OPT_ID_MASK` | established | Bits 6 and 5 of an id read. |
| `OPT_ID_RESTORE` | established | Bits ORed into the first byte read when the probe writes that byte back. |
| `OPT_CMD_CLOCK_KHZ` | established | Pixel clock in kHz. Above this the timing routine sends 0xff to the option port; at or below it, 0x7f. |
| `OPT_INSTALLED_LIMIT` | established | Installed values below this use the small horizontal delay. The reason for the split is inferred. |
| `OPT_DELAY_SMALL` | established | Horizontal delay used when the installed value is below the limit. It is zero otherwise. |
| `FIELD_SHIFT` | established | Bit position of the high 12-bit field in the packed timing words. |
| `FIELD_MASK` | established | Mask applied to each 12-bit field before it is shifted or stored. |
| `BOARD_INTERLACE_OFF` | established | Board description byte that selects the interlace case when non-zero. The layout of the board description is not established here. |
| `BOARD_PANEL_OFF` | established | Board description byte that selects a flat-panel set-up. |
| `PANEL_SETUP_A` | established | Panel set-up byte: fixed values for the first panel. |
| `PANEL_SETUP_B` | established | Panel set-up byte: fixed values for the second panel. |
| `PANEL_SETUP_C` | established | Panel set-up byte: fixed horizontal value, and the timing flag rewrite. |
| `TILE_LIST_ALIGN_BYTES` | inferred | Tile-list requests start on this boundary |
| `TILE_LIST_PAGE_BYTES` | inferred | A tile-list request does not cross a page of this size |
| `TILE_LIST_OUTSTANDING` | inferred | Tile-list requests that may be outstanding at once |
| `CMAP_ENTRIES_REV11` | inferred | Colour-map entries in revision 1.1. The IRIX revision-2 tables and the public Linux driver use 6144 |
| `OVERLAY_TRANSPARENT_INDEX` | inferred | Overlay pixel value that is transparent, so the normal planes show through |

The nine capture control words the video driver clears and loads for a capture path. Offsets are from the capture block base, which is a fixed displacement in the GBE register block. Prose and the reasoning are in docs/mace-video.md.

| Item (gbe_capture) | grade | kernel use |
|---|---|---|
| `vc` | inferred | Capture control words VC_0 to VC_8, as the reading names them. The reading gives the first and last word offsets, 0x80000 and 0x80020, so the 4-byte stride is inferred. The reading established that VC_0 to VC_3 are written as zero when a capture path stops. Its word width is inferred from the word-sized copies the start-up makes. VC_2 bit 0 enables DMA and VC_3 holds a path byte, both inferred from the start routine. |

## O2 MACE video and two-wire bus

Register facts: crime repository, units `mace_video_in`, `mace_video`, `mace_i2c` and `mace_timer`.

Base addresses of the video window and its blocks, the offsets of the two input channels and their register layouts. Offsets in the input registers are from the start of an input channel. Prose and the reasoning are in docs/mace-video.md.

| Item (mace_video) | grade | kernel use |
|---|---|---|
| `in_control` | inferred | Channel control word. The driver writes the whole word, changing only its low nine bits. The reading read the routines that write it in a workspace and did not re-read them, so the grade is inferred. Field `dma_enable` (bits 0:0, inferred): DMA enable. Bit 0 is stated for the output channel; the input reading gives only the nine-bit group. Field `intr_enable` (bits 8:1, inferred): Eight interrupt enables. Which cause each bit enables is not stated. |
| `in_status` | inferred | Status word. Reading it clears the pending interrupt causes. The page numbers are the reading's citation and were not checked against the document. |
| `in_config` | established | Configuration word, written at start-up. Its field layout is not recorded. |
| `in_next_descriptor` | inferred | Physical address of the next descriptor block, held as a zero-extended 32-bit word. The driver ORs a flag byte into the word. Which bits carry the flag is not stated. |
| `in_field_offset` | inferred | Field offset. The driver writes the low halfword of a transfer-record word here. Field `offset` (bits 15:0, inferred): Field offset in the low halfword |
| `in_line_width` | inferred | Line width word. The name is the reading's label; the start-up writes it with a 64-bit store. |
| `in_hclip_odd` | established | Horizontal clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded. |
| `in_vclip_odd` | established | Vertical clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded. |
| `in_alpha_odd` | established | Alpha word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded. |
| `in_hclip_even` | established | Horizontal clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded. |
| `in_vclip_even` | established | Vertical clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded. |
| `in_alpha_even` | established | Alpha word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded. |
| `WINDOW_PHYS_ADDR` | established | Physical base of the video window. The driver reaches it through the uncached address 0xbf100000 and stores that pointer as input channel 1. |
| `IN2_OFF` | established | Offset of input channel 2 from the start of the video window |
| `OUT_OFF` | established | Offset of the output channel from the start of the video window |
| `TWO_WIRE_PHYS_ADDR` | established | Physical base of the MACE peripheral two-wire bus block |
| `TIMER_PHYS_ADDR` | established | Physical base of the MACE count and compare timer block, read by the driver's timestamp routine. Its place as sub-block 4 of the MACE peripheral map is the reading's citation of the specification, not checked. |
| `GBE_CAPTURE_OFF` | established | Offset of the GBE capture block from the GBE register block base. The driver stores the uncached pointer 0xb6080000 for it. |
| `TIMER_SNAP_IN1_OFF` | established | Timer snapshot doubleword for input channel 1, from the timer block base |
| `TIMER_SNAP_IN2_OFF` | established | Timer snapshot doubleword for input channel 2, from the timer block base |
| `TIMER_SNAP_OUT_OFF` | established | Timer snapshot doubleword for the output channel, from the timer block base |

Register offsets, widths and the fields the reading could state for the output channel. Offsets are from the output channel base, which is a fixed displacement in the video window. Prose and the reasoning are in docs/mace-video.md.

| Item (mace_video_out) | grade | kernel use |
|---|---|---|
| `out_control` | inferred | Channel control word. The driver writes the whole word, changing only its low nine bits. The output recovery routines pulse bit 0. The reading read the routines that write it in a workspace and did not re-read them, so the grade is inferred. Field `dma_enable` (bits 0:0, inferred): DMA enable Field `intr_enable` (bits 8:1, inferred): Eight interrupt enables. Which cause each bit enables is not stated. |
| `out_status` | inferred | Status word. Reading it clears the pending interrupt causes. The output recovery routines read it after their last write and discard the value. The page numbers were not checked against the document. |
| `out_config` | established | Configuration word, written at start-up. Its field layout is not recorded here; the hardware configuration register at at 0x58 is a separate word. |
| `out_next_descriptor` | inferred | Physical address of the next descriptor block, held as a zero-extended 32-bit word. The recovery routines write it as well. Which bits carry the flag byte the driver ORs in is not stated. |
| `out_field_offset` | inferred | Field offset. The driver writes the low halfword of a transfer-record word here. Field `offset` (bits 15:0, inferred): Field offset in the low halfword |
| `out_field_size` | inferred | Field size. The reading takes the line count and the bytes per line from one stored value, which it gives as 243 lines of 2880 bytes in one set and 288 lines in the other. Field `lines` (bits 21:12, inferred): Lines in the field Field `line_bytes` (bits 11:0, inferred): Bytes per line |
| `out_h_pad_odd` | established | Horizontal padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded. |
| `out_v_pad_odd` | established | Vertical padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded. |
| `out_h_pad_even` | established | Horizontal padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded. |
| `out_v_pad_even` | established | Vertical padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded. |
| `out_genlock_delay` | established | Genlock delay word, written at start-up with a 64-bit store. Its layout is not recorded. |
| `out_hw_config` | inferred | Hardware configuration word for the output channel: port reset and select bits, audio and genlock sync settings, the general-purpose output bit and the framelock start-of-field counts. Bits 4, 7 and 8 and the audio-sync reset bit are not assigned by the reading. Field `port_ab_reset` (bits 0:0, inferred): Port AB reset. Released whenever an input or output is set up. Field `port_ab_select` (bits 1:1, inferred): Port AB select Field `port_cd_reset` (bits 2:2, inferred): Port CD reset. Released whenever an input or output is set up. Field `port_cd_select` (bits 3:3, inferred): Port CD select Field `audio_sync_source` (bits 6:5, inferred): Audio sync source Field `framelock_reset` (bits 9:9, inferred): GBE framelock reset Field `framelock_source` (bits 11:10, inferred): GBE framelock source Field `framelock_precision` (bits 12:12, inferred): GBE framelock precision Field `framelock_correction` (bits 13:13, inferred): GBE framelock correction Field `sof_count_odd` (bits 17:14, inferred): Start-of-field count for the odd field Field `general_output` (bits 18:18, inferred): General-purpose output bit Field `sof_count_even` (bits 22:19, inferred): Start-of-field count for the even field Field `ident` (bits 35:32, inferred): Identity read by the video probe. The value that identifies the part is not in the reading. |

Configuration, control and data words of the MACE peripheral's two-wire bus, which the video driver uses to program video-port chips. The bus is base 0x1f330000. Prose and the reasoning are in docs/mace-video.md.

| Item (mace_two_wire) | grade | kernel use |
|---|---|---|
| `tw_config` | established | Bus configuration word. The configuration helper reads it and writes it back with the bits selected by its mask replaced. The driver writes 1 to request a bus reset. Meanings of the other values written are not recorded. Field `low_config` (bits 3:0, established): Low four bits, the only bits the configuration helper changes |
| `tw_control` | established | Bus control and status word. Transfer routines poll the status bits below. Writes of 1, 3 and 5 are commands, and their meanings are inferred in docs/mace-video.md. Field `not_idle` (bits 0:0, established): Bus not idle. Also set by the force-idle command, which stores its argument with this bit cleared. Field `transfer_busy` (bits 4:4, established): Transfer busy Field `ack_missing` (bits 5:5, established): Acknowledge not received from the addressed chip Field `bus_error` (bits 7:7, established): Bus error. Only the force-idle path tests it in the reading. |
| `tw_data` | established | Data word. The read and write routines store the chip address byte and the register number byte through it, and read it back after the transfer. How its bytes map to bits is not recorded. |
