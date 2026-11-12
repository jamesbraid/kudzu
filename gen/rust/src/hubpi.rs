// generated from defs/hubpi.toml by defs-gen - do not edit, change the source
// node hub interrupt pending and mask registers
#![allow(dead_code)]

/// (inferred)
pub mod int_pend_mod {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x90;
    pub mod set { // bits 8:8, inferred
        pub const SHIFT: u32 = 8;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod level { // bits 6:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0x7f;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod int_pend0 {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x98;
}
/// (established)
pub mod int_pend1 {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0xa0;
}
/// (established)
pub mod int_mask0_a {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0xa8;
}
/// (established)
pub mod int_mask1_a {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0xb0;
}
/// (established)
pub mod int_mask0_b {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0xb8;
}
/// (established)
pub mod int_mask1_b {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0xc0;
}
pub const INT_PEND_MOD_ALT_OFFSET: u32 = 0x200090; // established
pub const LOCAL_WINDOW_BASE: u32 = 0x1000000; // established
pub const NODE_SHIFT: u32 = 0x20; // established
pub const NODE_WINDOW_BASE: u32 = 0x1800000; // established
