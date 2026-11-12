// generated from defs/hubmda.toml by defs-gen - do not edit, change the source
// Origin 200/2000 hub memory-directory migration registers
#![allow(dead_code)]

/// Bit 63 is the valid bit: set when the kernel arms the threshold, cleared to disarm it while a candidate is handled. See MIG_DIFF_THRESH_VALID_BIT. (established)
pub mod mig_diff_thresh {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x30;
    pub mod value { // bits 19:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0xfffff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Bit 63 is the valid bit, set to arm and cleared to disarm. See MIG_VALUE_THRESH_VALID_BIT. (inferred)
pub mod mig_value_thresh {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x38;
}
/// Read-to-clear alias of the candidate register. The kernel reads it in a loop until the valid bit (bit 63) is clear. The public Linux hub header calls  (established)
pub mod mig_candidate {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x48;
    pub mod r#type { // bits 30:30, inferred
        pub const SHIFT: u32 = 30;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod node { // bits 28:20, inferred
        pub const SHIFT: u32 = 20;
        pub const MASK: u64 = 0x1ff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod page { // bits 17:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0x3ffff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const MIG_CANDIDATE_VALID_BIT: u32 = 0x3f; // established
pub const MIG_DIFF_THRESH_VALID_BIT: u32 = 0x3f; // established
pub const MIG_VALUE_THRESH_VALID_BIT: u32 = 0x3f; // established
pub const NODE_SHIFT: u32 = 0x20; // established
pub const REFCNT_COUNT_MASK_PREMIUM: u32 = 0x7ffff; // established
pub const REFCNT_COUNT_MASK_STANDARD: u32 = 0x7ff; // established
pub const REFCNT_COUNT_SHIFT: u32 = 0x5; // established
pub const REFCNT_NODE_STRIDE: u32 = 0x8; // established
pub const WINDOW_BASE: u32 = 0x1a00000; // established
