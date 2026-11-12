// generated from defs/crime.toml by defs-gen - do not edit, change the source
// O2 CRIME as the shipped IRIX kernel and graphics libraries drive it: identity, interrupt inputs, renderer state and TLB formats
#![allow(dead_code)]

/// Identity byte at the start of the CRIME block. The graphics probe reads its low byte and accepts the part only when bits 7:4 are the accepted family a (established)
pub mod id {
    pub const WIDTH: u32 = 8;
    pub const OFFSET: u64 = 0x0;
    pub mod family { // bits 7:4, established
        pub const SHIFT: u32 = 4;
        pub const MASK: u32 = 0xf;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Texture format record, written by the OpenGL library for each texture. It carries the log2 sizes of the texture and the coordinate masks. The width of (established)
pub mod texture_format {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x2118;
    pub mod width_inv { // bits 47:44, established
        pub const SHIFT: u32 = 44;
        pub const MASK: u64 = 0xf;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod height_inv { // bits 43:40, established
        pub const SHIFT: u32 = 40;
        pub const MASK: u64 = 0xf;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod width_log2 { // bits 39:36, established
        pub const SHIFT: u32 = 36;
        pub const MASK: u64 = 0xf;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod height_log2 { // bits 35:32, established
        pub const SHIFT: u32 = 32;
        pub const MASK: u64 = 0xf;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod width_mask { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u64 = 0xffff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod height_mask { // bits 15:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0xffff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Interface-buffer address word, as the kernel decodes it in software. Bit 45 marks the stop entry. (established)
pub mod ib_addr {
    pub const WIDTH: u32 = 64;
    pub mod stop { // bits 45:45, established
        pub const SHIFT: u32 = 45;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod lane { // bits 44:43, established
        pub const SHIFT: u32 = 43;
        pub const MASK: u64 = 0x3;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod group { // bits 42:40, established
        pub const SHIFT: u32 = 40;
        pub const MASK: u64 = 0x7;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod reg_index { // bits 39:32, established
        pub const SHIFT: u32 = 32;
        pub const MASK: u64 = 0xff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const AA_LINE_OFF: u32 = 0x2190; // established
pub const ALPHA_TEST_OFF: u32 = 0x2198; // established
pub const BLEND_FUNC_OFF: u32 = 0x21a8; // established
pub const BLOCK_ADDR: u32 = 0x14000000; // established
pub const CID_TLB_REGS: u32 = 0x4; // established
pub const CONTROL_OFF: u32 = 0x8; // established
pub const CPU_ERR_STATUS_OFF: u32 = 0x48; // established
pub const DEPTH_APERTURE_ADDR: u32 = 0x12000000; // inferred
pub const DEPTH_FUNC_OFF: u32 = 0x21c0; // established
pub const DMA_IOCTL: u32 = 0x55f2; // inferred
pub const DMA_POLL_LIMIT: u32 = 0x7a120; // established
pub const DMA_REQ_BYTES: u32 = 0x28; // established
pub const DRAWMODE_OFF: u32 = 0x2018; // established
pub const ENGINE_RESET_OFF: u32 = 0x408; // established
pub const FB_TLB_REGS: u32 = 0x40; // established
pub const FOG_COLOR_OFF: u32 = 0x2170; // established
pub const FRAME_APERTURE_ADDR: u32 = 0x10000000; // inferred
pub const FULL_MEM_ADDR: u32 = 0x40000000; // inferred
pub const GBE_WINDOW_ADDR: u32 = 0x16000000; // inferred
pub const GL_UNBOUND_BIAS: u32 = 0x21000; // established
pub const GL_VERTEX_BIAS: u32 = 0x1000; // established
pub const HIGH_ALIAS_ADDR: u32 = 0x20000000; // inferred
pub const ID_FAMILY_ACCEPTED: u32 = 0xa; // established
pub const ID_REFUSED_BYTE: u32 = 0xa0; // established
pub const ID_SPEC_15: u32 = 0xb1; // inferred
pub const INTMASK_OFF: u32 = 0x18; // established
pub const INTR_EDGE_OFF: u32 = 0x28; // established
pub const INTSTAT_OFF: u32 = 0x10; // established
pub const IRQ_BUS_ERR_A: u32 = 0x14; // established
pub const IRQ_BUS_ERR_B: u32 = 0x15; // established
pub const IRQ_FIFO_HIGH: u32 = 0x1a; // established
pub const IRQ_FIFO_LOW: u32 = 0x19; // established
pub const IRQ_GBE_LINE_2: u32 = 0x12; // established
pub const IRQ_GBE_LINE_3: u32 = 0x13; // established
pub const IRQ_MACE_ISA_A: u32 = 0x4; // established
pub const IRQ_MACE_ISA_B: u32 = 0x5; // established
pub const IRQ_PREBLANK: u32 = 0x11; // established
pub const IRQ_RETRACE: u32 = 0x10; // established
pub const IRQ_SOFT_A: u32 = 0x1c; // inferred
pub const IRQ_SOFT_B: u32 = 0x1d; // inferred
pub const LINEAR_WINDOW_MASK: u32 = 0x1ffff; // established
pub const LINE_MATCH_LATCHES: u32 = 0x4; // inferred
pub const LIN_TLB_MAX_PAGES: u32 = 0x20; // established
pub const LIN_TLB_REGS: u32 = 0x10; // established
pub const LIN_TLB_VALID: u32 = 0x80000000; // established
pub const LOGIC_OP_OFF: u32 = 0x21b0; // established
pub const MACE_WINDOW_ADDR: u32 = 0x18000000; // inferred
pub const MEM_CTL_OFF: u32 = 0x200; // established
pub const MEM_ERR_STATUS_OFF: u32 = 0x250; // established
pub const MTE_HOST_CHUNK_BYTES: u32 = 0x20000; // established
pub const MTE_MODE_HOST_CLEAR: u32 = 0x11; // established
pub const MTE_MODE_HOST_COPY: u32 = 0x897; // established
pub const MTE_MODE_X_COPY: u32 = 0x820; // established
pub const MTE_X_BLOCK_BYTES: u32 = 0x100; // established
pub const MTE_X_ROWS_16: u32 = 0x10; // established
pub const MTE_X_ROWS_32: u32 = 0x8; // established
pub const NOECC_ALIAS_ADDR: u32 = 0x80000000; // inferred
pub const PAGE_BYTES: u32 = 0x1000; // inferred
pub const PIXFLUSH_OFF: u32 = 0x21f8; // established
pub const PIXPIPE_NULL_OFF: u32 = 0x21f0; // established
pub const PIXXFER_POS_OFF: u32 = 0x20a0; // established
pub const PIXXFER_VTX_OFF: u32 = 0x2070; // established
pub const PIXXFER_YSTEP_OFF: u32 = 0x20ac; // established
pub const PRIMITIVE_OFF: u32 = 0x2060; // established
pub const PRIM_RECT: u32 = 0x3020000; // established
pub const PRIM_TRIANGLE: u32 = 0x2000000; // established
pub const RENDER_COUNT_INIT: u32 = 0xa; // established
pub const RENDER_CTL_INIT: u32 = 0xfefff0a; // established
pub const RENDER_CTL_OFF: u32 = 0x400; // established
pub const RENDER_EMPTY_SHIFT: u32 = 0xe; // established
pub const RENDER_FIELD_MASK: u32 = 0x7f; // established
pub const RENDER_FIFO_DEPTH: u32 = 0x40; // inferred
pub const RENDER_FLUSH_OFF: u32 = 0x4008; // established
pub const RENDER_FULL_SHIFT: u32 = 0x15; // established
pub const RENDER_OCC_SHIFT: u32 = 0x12; // established
pub const RENDER_R10K_EMPTY: u32 = 0x0; // established
pub const RENDER_R10K_FULL: u32 = 0x14; // established
pub const RENDER_R10K_STALL: u32 = 0x13; // established
pub const RENDER_R5K_EMPTY: u32 = 0x18; // established
pub const RENDER_R5K_FULL: u32 = 0x30; // established
pub const RENDER_R5K_STALL: u32 = 0x2f; // established
pub const RENDER_STALL_SHIFT: u32 = 0x7; // established
pub const RENDER_START_MASK: u32 = 0x3f; // established
pub const RENDER_STATUS_IDLE_BIT: u32 = 0x1c; // established
pub const RENDER_STATUS_OFF: u32 = 0x4000; // established
pub const RENDER_WPTR_SHIFT: u32 = 0x6; // established
pub const SOFTINT_OFF: u32 = 0x20; // inferred
pub const START_ALIAS_OFF: u32 = 0x800; // established
pub const STENCIL_MASK_OFF: u32 = 0x21e8; // established
pub const STENCIL_MODE_OFF: u32 = 0x21e0; // established
pub const TEXTURE_MODE_OFF: u32 = 0x2110; // established
pub const TEX_LOG2_BYTES_SHIFT: u32 = 0x12; // established
pub const TEX_MAX_TEXELS: u32 = 0x400; // inferred
pub const TEX_SUBTILE_BIT: u32 = 0x18; // established
pub const TEX_SUBTILE_CELL_BYTES: u32 = 0x20; // inferred
pub const TEX_TILE_FLAG: u32 = 0x8000; // established
pub const TEX_TLB_REGS: u32 = 0x1c; // established
pub const TEX_TYPE_SHIFT: u32 = 0x14; // established
pub const TILE_BYTES: u32 = 0x10000; // inferred
pub const TILE_PAGES: u32 = 0x10; // established
pub const TILE_ROWS: u32 = 0x80; // inferred
pub const TILE_ROW_BYTES: u32 = 0x200; // inferred
pub const TIME_OFF: u32 = 0x38; // established
pub const TIME_RESOLUTION: u32 = 0x3a98; // established
pub const VTX_DEPTH_BASE: u32 = 0x840000; // established
pub const VTX_STEP: u32 = 0x40; // established
pub const VTX_TRI_BASE: u32 = 0x48040000; // established
pub const WATCHDOG_OFF: u32 = 0x30; // established
