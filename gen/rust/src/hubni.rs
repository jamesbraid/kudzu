// generated from defs/hubni.toml by defs-gen - do not edit, change the source
// node hub network interface registers
#![allow(dead_code)]

/// The low 32 bits of the Origin 200/2000 network status word. (established)
pub mod nsri {
    pub const WIDTH: u32 = 32;
    pub mod nodeid { // bits 16:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0x1ff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const O200_STATUS_OFFSET: u32 = 0x600000; // established
pub const O300_ERROR_CLEAR_OFFSET: u32 = 0x680120; // established
pub const O300_ERROR_LATCHED_HI: u32 = 0x34; // established
pub const O300_ERROR_LATCHED_LO: u32 = 0x18; // established
pub const O300_ERROR_LINK_RESET_BIT: u32 = 0x34; // established
pub const O300_ERROR_OFFSET: u32 = 0x680100; // established
pub const O300_ERROR_REPORT_A_OFFSET: u32 = 0x680108; // established
pub const O300_ERROR_REPORT_B_OFFSET: u32 = 0x680110; // established
pub const O300_ERROR_REPORT_C_OFFSET: u32 = 0x680118; // established
pub const O300_PORT_RESET_OFFSET: u32 = 0x680008; // established
pub const O300_RESET_ENABLE_OFFSET: u32 = 0x680010; // established
pub const O300_STATUS_OFFSET: u32 = 0x680000; // established
