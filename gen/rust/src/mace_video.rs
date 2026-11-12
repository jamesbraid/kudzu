// generated from defs/mace_video.toml by defs-gen - do not edit, change the source
// O2 MACE video window map and input channel registers as the IRIX O2 video driver reaches them
#![allow(dead_code)]

/// Channel control word. The driver writes the whole word, changing only its low nine bits. The reading read the routines that write it in a workspace an (inferred)
pub mod in_control {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x0;
    pub mod intr_enable { // bits 8:1, inferred
        pub const SHIFT: u32 = 1;
        pub const MASK: u64 = 0xff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod dma_enable { // bits 0:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Status word. Reading it clears the pending interrupt causes. The page numbers are the reading's citation and were not checked against the document. (inferred)
pub mod in_status {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x8;
}
/// Configuration word, written at start-up. Its field layout is not recorded. (established)
pub mod in_config {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x10;
}
/// Physical address of the next descriptor block, held as a zero-extended 32-bit word. The driver ORs a flag byte into the word. Which bits carry the fla (inferred)
pub mod in_next_descriptor {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x18;
}
/// Field offset. The driver writes the low halfword of a transfer-record word here. (inferred)
pub mod in_field_offset {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x20;
    pub mod offset { // bits 15:0, inferred
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
/// Line width word. The name is the reading's label; the start-up writes it with a 64-bit store. (inferred)
pub mod in_line_width {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x28;
}
/// Horizontal clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded. (established)
pub mod in_hclip_odd {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x30;
}
/// Vertical clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded. (established)
pub mod in_vclip_odd {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x38;
}
/// Alpha word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded. (established)
pub mod in_alpha_odd {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x40;
}
/// Horizontal clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded. (established)
pub mod in_hclip_even {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x48;
}
/// Vertical clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded. (established)
pub mod in_vclip_even {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x50;
}
/// Alpha word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded. (established)
pub mod in_alpha_even {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x58;
}
pub const GBE_CAPTURE_OFF: u32 = 0x80000; // established
pub const IN2_OFF: u32 = 0x80000; // established
pub const OUT_OFF: u32 = 0x100000; // established
pub const TIMER_PHYS_ADDR: u32 = 0x1f340000; // established
pub const TIMER_SNAP_IN1_OFF: u32 = 0x38; // established
pub const TIMER_SNAP_IN2_OFF: u32 = 0x40; // established
pub const TIMER_SNAP_OUT_OFF: u32 = 0x48; // established
pub const TWO_WIRE_PHYS_ADDR: u32 = 0x1f330000; // established
pub const WINDOW_PHYS_ADDR: u32 = 0x1f100000; // established
