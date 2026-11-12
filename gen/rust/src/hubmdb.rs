// generated from defs/hubmdb.toml by defs-gen - do not edit, change the source
// Origin 300 hub memory-directory migration registers
#![allow(dead_code)]

/// The mode control sits in a different window of the hub, not in the block above: its offset is the local register base of the public vendor header subt (established)
pub mod mig_diff_thresh {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x40;
    pub mod value { // bits 14:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0x7fff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const MIG_CANDIDATE_CLR_OFFSET: u32 = 0x38; // unknown
pub const MIG_CANDIDATE_OFFSET: u32 = 0x30; // unknown
pub const MIG_CONFIG_LOCAL_OFFSET: u32 = 0x780018; // inferred
pub const MIG_DIFF_THRESH_VALID_BIT: u32 = 0x3f; // established
pub const MIG_VALUE_THRESH_OFFSET: u32 = 0x48; // unknown
pub const NODE_SHIFT: u32 = 0x21; // established
pub const WINDOW_BASE_UNITS: u32 = 0x1fc; // established
pub const WINDOW_BASE_UNIT_SHIFT: u32 = 0x10; // established
