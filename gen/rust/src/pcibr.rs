// generated from defs/pcibr.toml by defs-gen - do not edit, change the source
// PCI bridge widget identification and window layouts
#![allow(dead_code)]

/// The widget identification word. (established)
pub mod wid {
    pub const WIDTH: u32 = 32;
    pub mod part { // bits 27:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const LAYOUT32: u32 = 49154; // established
        pub const LAYOUT32B: u32 = 53250; // established
        pub const LAYOUT64B: u32 = 53506; // established
        pub const LAYOUT64BX: u32 = 53522; // established
        pub const LAYOUT64A: u32 = 57344; // established
        pub const LAYOUT64AX: u32 = 57360; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Layout32,
            Layout32b,
            Layout64b,
            Layout64bx,
            Layout64a,
            Layout64ax,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    49154 => Self::Layout32,
                    53250 => Self::Layout32b,
                    53506 => Self::Layout64b,
                    53522 => Self::Layout64bx,
                    57344 => Self::Layout64a,
                    57360 => Self::Layout64ax,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Layout32 => 49154,
                    Self::Layout32b => 53250,
                    Self::Layout64b => 53506,
                    Self::Layout64bx => 53522,
                    Self::Layout64a => 57344,
                    Self::Layout64ax => 57360,
                    Self::Other(v) => v,
                }
            }
        }
    }
    pub mod field11 { // bits 11:1, established
        pub const SHIFT: u32 = 1;
        pub const MASK: u32 = 0x7ff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const VALUE36: u32 = 36; // established
        pub const VALUE54: u32 = 54; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Value36,
            Value54,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    36 => Self::Value36,
                    54 => Self::Value54,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Value36 => 36,
                    Self::Value54 => 54,
                    Self::Other(v) => v,
                }
            }
        }
    }
}
pub const CFG_FUNC_SHIFT: u32 = 0x8; // established
pub const CFG_SLOTS_32: u32 = 0x8; // established
pub const CFG_SLOT_BIAS_64: u32 = 0x1; // established
pub const CFG_SLOT_SHIFT: u32 = 0xc; // established
pub const CFG_WINDOW32_OFFSET: u32 = 0x20000; // established
pub const INTR_ADDR32_HI: u32 = 0x11; // established
pub const INTR_ADDR32_LO: u32 = 0x8; // established
pub const INTR_ADDR32_OFFSET: u32 = 0x134; // established
pub const INTR_ADDR32_SRC_HI: u32 = 0x2f; // established
pub const INTR_ADDR32_SRC_LO: u32 = 0x26; // established
pub const INTR_ADDR64_OFFSET: u32 = 0x130; // established
pub const INTR_COUNT: u32 = 0x8; // established
pub const INTR_STRIDE: u32 = 0x8; // established
pub const WID_OFFSET: u32 = 0x4; // established
pub const WINDOW_BIT_A: u32 = 0x3f; // established
pub const WINDOW_BIT_B: u32 = 0x3c; // established
pub const WINDOW_BIT_C: u32 = 0x39; // established
