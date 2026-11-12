// generated from defs/gbe.toml by defs-gen - do not edit, change the source
// O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe
#![allow(dead_code)]

/// Vertical blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active height and 0xfff (established)
pub mod vt_vblank {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10010;
    pub mod hi { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod lo { // bits 11:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Horizontal blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active width and 0xff (established)
pub mod vt_hblank {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10014;
    pub mod hi { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod lo { // bits 11:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Timing flag word. The timing routine reads it, clears bits 1 and 7, and ORs in a value of 0 or 2 for the byte 132 set-up. What bit 1 does to the hardw (established)
pub mod timing_flags {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10018;
}
/// Flat-panel horizontal drive word. The value depends on the board's panel set-up byte and on whether a dual-channel option is fitted. The table of valu (established)
pub mod fp_hdrv {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10028;
    pub mod hi { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod lo { // bits 11:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Flat-panel vertical drive word. The value depends on the panel set-up byte and on the option, as the table in docs/gbe.md gives. (established)
pub mod fp_vdrv {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x1002c;
    pub mod hi { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod lo { // bits 11:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Flat-panel data-enable word. The timing routine writes it only for the default set-up and panel set-ups 130 and 131, and only when no dual-channel opt (established)
pub mod fp_de {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10030;
    pub mod hi { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod lo { // bits 11:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
pub const BOARD_INTERLACE_OFF: u32 = 0x2d; // established
pub const BOARD_PANEL_OFF: u32 = 0x2e; // established
pub const FIELD_MASK: u32 = 0xfff; // established
pub const FIELD_SHIFT: u32 = 0xc; // established
pub const OPT_CMD_CLOCK_KHZ: u32 = 0x186a0; // established
pub const OPT_CMD_ID_HIGH: u32 = 0xff; // established
pub const OPT_CMD_ID_LOW: u32 = 0x7f; // established
pub const OPT_DELAY_SMALL: u32 = 0x2; // established
pub const OPT_ID_MASK: u32 = 0x60; // established
pub const OPT_ID_RESTORE: u32 = 0x70; // established
pub const OPT_INSTALLED_LIMIT: u32 = 0x9; // established
pub const OPT_PORT_ADDR: u32 = 0x7e; // established
pub const PANEL_SETUP_A: u32 = 0x82; // established
pub const PANEL_SETUP_B: u32 = 0x83; // established
pub const PANEL_SETUP_C: u32 = 0x84; // established
pub const TWO_WIRE_FP_OFF: u32 = 0x10; // established
pub const TWO_WIRE_MON_OFF: u32 = 0x8; // established
pub const TWO_WIRE_WAIT_BIT: u32 = 0x1; // established
pub const TWO_WIRE_WAIT_LIMIT: u32 = 0x3c; // established
