// generated from defs/r10k_timer.toml by defs-gen - do not edit, change the source
// R10000 Count and Compare timer interrupt
#![allow(dead_code)]

/// (established)
pub mod cause {
    pub const WIDTH: u32 = 32;
    pub mod ip7 { // bits 15:15, established
        pub const SHIFT: u32 = 15;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const R10K_CP0_COMPARE: u32 = 0xb; // established
pub const R10K_CP0_COUNT: u32 = 0x9; // established
