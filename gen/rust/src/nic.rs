// generated from defs/nic.toml by defs-gen - do not edit, change the source
// bit-serial access to the one-wire identity parts
#![allow(dead_code)]

/// The I/O controller's general-purpose control word, written through its set alias. Before reading the address part the routine stores 0x200000 to the r (inferred)
pub mod gpcr_s {
    pub const WIDTH: u32 = 32;
    pub mod nic_enable { // bits 21:21, inferred
        pub const SHIFT: u32 = 21;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// The 32-bit microlan control word. (established)
pub mod mcr {
    pub const WIDTH: u32 = 32;
    pub mod pulse { // bits 10:10, established
        pub const SHIFT: u32 = 10;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod sample { // bits 2:2, established
        pub const SHIFT: u32 = 2;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod done { // bits 1:1, established
        pub const SHIFT: u32 = 1;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod result { // bits 0:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const GPCR_S_NIC_ENABLE_WORD: u32 = 0x200000; // established
pub const HUB_ACCESS_NODE_SHIFT: u32 = 0x21; // established
pub const HUB_ACCESS_OFFSET: u32 = 0xe04030; // established
pub const HUB_ACCESS_PHYS_LOW: u32 = 0x1e04030; // established
pub const MAC_CRC16_RESIDUE: u32 = 0xb001; // established
pub const MAC_ID_BYTE0: u32 = 0x8d; // established
pub const MAC_ID_BYTE1: u32 = 0xa; // established
pub const MAC_RECORD_LEN: u32 = 0xe; // established
