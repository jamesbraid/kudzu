// generated from defs/hubio.toml by defs-gen - do not edit, change the source
// Origin 300 hub crosstalk-side register window
#![allow(dead_code)]

/// Link control and status doubleword that the early widget probe reads. Only bit 13 is described. With it clear, the probe returns -1 for the part, revi (established)
pub mod llp_csr {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x128;
    pub mod bit13 { // bits 13:13, established
        pub const SHIFT: u32 = 13;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// The register the kernel's error path tests before it goes on to the widget error handler. (established)
pub mod flags {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x1e8;
    pub mod bit4 { // bits 4:4, established
        pub const SHIFT: u32 = 4;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod bit0 { // bits 0:0, established
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
/// Request-buffer table. Entry i (0 to 14) has three 64-bit words, crb_a, crb_b and crb_c, at the register offsets below plus the stride times i; the sca (established)
pub mod crb_a {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x400;
    pub mod bit55 { // bits 55:55, established
        pub const SHIFT: u32 = 55;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod r#type { // bits 54:52, established
        pub const SHIFT: u32 = 52;
        pub const MASK: u64 = 0x7;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod bit51 { // bits 51:51, established
        pub const SHIFT: u32 = 51;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod bit50 { // bits 50:50, established
        pub const SHIFT: u32 = 50;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod bit49 { // bits 49:49, established
        pub const SHIFT: u32 = 49;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod bit0 { // bits 0:0, established
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
/// (established)
pub mod crb_b {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x408;
    pub mod engine_alt { // bits 58:58, established
        pub const SHIFT: u32 = 58;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod sub { // bits 42:41, established
        pub const SHIFT: u32 = 41;
        pub const MASK: u64 = 0x3;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod kind { // bits 32:30, established
        pub const SHIFT: u32 = 30;
        pub const MASK: u64 = 0x7;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod crb_c {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x410;
    pub mod bit48 { // bits 48:48, established
        pub const SHIFT: u32 = 48;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const CRB_COUNT: u32 = 0xf; // established
pub const CRB_STRIDE: u32 = 0x20; // established
pub const NODE_SHIFT: u32 = 0x21; // established
pub const WIDGET_PART_C104: u32 = 0xc104; // established
pub const WIDGET_PART_C105: u32 = 0xc105; // established
pub const WINDOW_BASE: u32 = 0x1c00000; // established
