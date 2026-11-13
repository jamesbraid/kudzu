// generated from formats/tg/format.toml by defs-gen - do not edit, change the source
// gigabit Ethernet controller match table of the Origin 300/350 driver
#![allow(dead_code)]

/// (established)
pub mod row_w0 {
    pub const WIDTH: u32 = 32;
    pub mod vendor { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod device { // bits 15:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod row_w1 {
    pub const WIDTH: u32 = 32;
    pub mod subsys_vendor { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod subsys_device { // bits 15:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const CFG_OFF_DEVICE: u32 = 0x0; // established
pub const CFG_OFF_SUBSYS_DEVICE: u32 = 0x2e; // established
pub const CFG_OFF_SUBSYS_VENDOR: u32 = 0x2c; // established
pub const CFG_OFF_VENDOR: u32 = 0x2; // established
pub const MATCH_ROW_COUNT: u32 = 0x5; // established
pub const MATCH_ROW_STRIDE: u32 = 0x10; // established
pub const PCI_DEVICE_1645: u32 = 0x1645; // established
pub const PCI_DEVICE_1648: u32 = 0x1648; // established
pub const PCI_DEVICE_16A8: u32 = 0x16a8; // established
pub const PCI_SUBSYS_DEVICE_FIRST: u32 = 0x8010; // established
pub const PCI_SUBSYS_VENDOR: u32 = 0x10a9; // established
pub const PCI_VENDOR: u32 = 0x14e4; // established
