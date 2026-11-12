// generated from defs/gbe.toml by defs-gen - do not edit, change the source
// O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe
#![allow(dead_code)]

/// Identity word the kernel reads first. It accepts the block only when the word reads 0x666; any other value means the kernel leaves the block untouched (established)
pub mod device_id {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x14;
}
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
/// Overlay control. The stop routine clears its enable bit and leaves the word otherwise unwritten. (established)
pub mod ovr_control {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x20008;
    pub mod enable { // bits 0:0, established
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
/// Frame-buffer scan-out control. The stop routine saves it, clears its enable bit, and later writes it back with its pointer field bumped by one unit. T (established)
pub mod frm_control {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x3000c;
    pub mod enable { // bits 0:0, established
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
/// Display-ID table control. The stop routine clears its enable bit and leaves the word otherwise unwritten. (established)
pub mod did_control {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x40004;
    pub mod enable { // bits 16:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Window-ID table, one 32-bit word per window index. Indices of 32 or more are rejected by the kernel. The kernel writes entry 0 with 3 at text-port set (established)
pub mod wid {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x48000;
    pub const COUNT: u32 = 32;
    pub const STRIDE: u64 = 4;
    pub mod gamma_disable { // bits 10:10, established
        pub const SHIFT: u32 = 10;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod map_group { // bits 9:5, inferred
        pub const SHIFT: u32 = 5;
        pub const MASK: u32 = 0x1f;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod r#type { // bits 4:2, inferred
        pub const SHIFT: u32 = 2;
        pub const MASK: u32 = 0x7;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod buf { // bits 1:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0x3;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Hardware cursor position word. The text port writes 0 at set-up. (established)
pub mod cursor_pos {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x70000;
}
/// Cursor control word. The text port writes 1 after the glyph and colours are loaded. What the bits mean beyond that is unknown. (established)
pub mod cursor_ctrl {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x70004;
}
/// Cursor colour 1 in the byte layout red, green, blue, zero. The text port writes 0xffffff00. (established)
pub mod cursor_color1 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x70008;
}
/// Cursor colour 2 in the byte layout red, green, blue, zero. The text port writes 0xff000000. (established)
pub mod cursor_color2 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x7000c;
}
/// Cursor bitmap, 64 words. The text port loads words 0 to 31 from a 32-word table and writes 0 to words 32 to 63. Reading the words as a 32 by 32 bitmap (established)
pub mod cursor_glyph {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x78000;
    pub const COUNT: u32 = 64;
    pub const STRIDE: u64 = 4;
}
pub const BOARD_INTERLACE_OFF: u32 = 0x2d; // established
pub const BOARD_PANEL_OFF: u32 = 0x2e; // established
pub const CMAP_ENTRY_BASE: u32 = 0x50000; // established
pub const CMFIFO_BATCH: u32 = 0x21; // established
pub const CMFIFO_FREE_MIN: u32 = 0x30; // established
pub const CMFIFO_MASK: u32 = 0x3f; // established
pub const DEVICE_ID_VALUE: u32 = 0x666; // established
pub const DID_BLOCK_BYTES: u32 = 0x200; // established
pub const DID_BLOCK_ENTRIES: u32 = 0x100; // established
pub const DID_END_BLOCK: u32 = 0x80; // established
pub const DID_LAST_XEND: u32 = 0x7ff; // established
pub const DID_LINE_TABLE_OFFSET: u32 = 0x2000; // established
pub const DID_TABLE_BYTES: u32 = 0x10000; // established
pub const DOTCLK_RUN_BIT: u32 = 0x14; // inferred
pub const DOTCLK_SETTLE_MS: u32 = 0xb; // established
pub const FIELD_MASK: u32 = 0xfff; // established
pub const FIELD_SHIFT: u32 = 0xc; // established
pub const GBE_LINE_IRQ_BASE: u32 = 0x10; // established
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
pub const REF_CLOCK_KHZ: u32 = 0x4e20; // inferred
pub const TILE_NUMBER_MASK: u32 = 0x1fff; // established
pub const TWO_WIRE_FP_OFF: u32 = 0x10; // established
pub const TWO_WIRE_MON_OFF: u32 = 0x8; // established
pub const TWO_WIRE_WAIT_BIT: u32 = 0x1; // established
pub const TWO_WIRE_WAIT_LIMIT: u32 = 0x3c; // established
pub const VT_XY_FREEZE: u32 = 0x80000000; // established
