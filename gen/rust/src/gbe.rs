// generated from defs/gbe.toml by defs-gen - do not edit, change the source
// O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe
#![allow(dead_code)]

/// Dot-clock (PLL) word. The timing routine clears the run bit, writes the timing words, then writes this word with the run bit set and waits 11 millisec (established)
pub mod dotclk {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x4;
    pub mod run { // bits 20:20, established
        pub const SHIFT: u32 = 20;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod pll_p { // bits 15:14, established
        pub const SHIFT: u32 = 14;
        pub const MASK: u32 = 0x3;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod pll_n { // bits 13:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0x3f;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod pll_m { // bits 7:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Identity word the kernel reads first. It accepts the block only when the word reads 0x666; any other value means the kernel leaves the block untouched (established)
pub mod device_id {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x14;
}
/// Vertical timing position word, called VT_0 in the reading. The start-up sequence compares its whole unmasked value with the vertical blank start line, (inferred)
pub mod vt_xy {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10000;
}
/// Frame totals. The timing routine writes 0 here, and the start-up sequence writes the totals once the frame buffer is set up. The kernel stores the rec (established)
pub mod vt_1 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10004;
    pub mod total_lines { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod total_pixels { // bits 11:0, established
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
/// Vertical sync position, in lines. (established)
pub mod vt_2 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10008;
    pub mod vsync_start { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod vsync_end { // bits 11:0, established
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
/// Horizontal sync position, in pixels. (established)
pub mod vt_3 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x1000c;
    pub mod hsync_start { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod hsync_end { // bits 11:0, established
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
/// Vertical blank start line in the high field. The low field repeats it when the frame-lock flag bits of the timing record (mask 0x12) are set, and hold (established)
pub mod vt_7 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x1001c;
    pub mod blank_start { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod low { // bits 11:0, established
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
/// Parked holding 0xffffff before the frame buffer is set up and written last in the start-up sequence. The final value is ((active width + v - 2) mod to (established)
pub mod vt_13 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10034;
}
/// Parked holding 0xffffff and written last in the start-up sequence with the vertical blank start in the low field and the vertical blank end in the hig (established)
pub mod vt_14 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10038;
}
/// Horizontal blank window, in pixels. (established)
pub mod vt_15 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x1003c;
    pub mod hblank_start { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod hblank_end { // bits 11:0, established
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
/// Vertical blank window, in lines. (established)
pub mod vt_16 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10040;
    pub mod vblank_start { // bits 23:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod vblank_end { // bits 11:0, established
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
/// Two parts, with bit placement not stated in the reading. The vertical part is minus (vertical blank start minus vertical blank end minus 1) when that  (established)
pub mod vt_17 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10044;
}
/// The vertical part of VT_17 and the horizontal blank end minus 54, with total pixels added when that is negative, then 0x1000 added for one line. The r (established)
pub mod vt_18 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x10048;
}
/// The vertical part of VT_17 without the extra line that VT_18 adds, and the horizontal blank end minus 4. Bit placement is inferred as for VT_18. (established)
pub mod vt_19 {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x1004c;
}
/// Live readback of the overlay control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the overlay control. (established)
pub mod ovr_readback {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x20004;
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
/// Live readback of the frame control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the frame control. (established)
pub mod frm_readback {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x30008;
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
    pub mod table_hi { // bits 15:0, inferred
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
/// Window-ID table, one 32-bit word per window index. Indices of 32 or more are rejected by the kernel. The kernel writes entry 0 with 3 at text-port set (established)
pub mod wid {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x48000;
    pub const COUNT: u32 = 32;
    pub const STRIDE: u64 = 4;
    pub mod aux { // bits 12:11, inferred
        pub const SHIFT: u32 = 11;
        pub const MASK: u32 = 0x3;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
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
/// Frame entry of a display-ID table: one per horizontal band of lines. The X module packs it as block << 19 | offset << 11 | yend. (established)
pub mod did_frame {
    pub const WIDTH: u32 = 32;
    pub mod block { // bits 25:19, established
        pub const SHIFT: u32 = 19;
        pub const MASK: u32 = 0x7f;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod offset { // bits 18:11, established
        pub const SHIFT: u32 = 11;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod yend { // bits 10:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0x7ff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Line-table entry of a display-ID table: one per rectangle in a band, sorted by its last column. The X module packs it as index << 11 | xend. (established)
pub mod did_line {
    pub const WIDTH: u32 = 16;
    pub mod did { // bits 15:11, established
        pub const SHIFT: u32 = 11;
        pub const MASK: u32 = 0x1f;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod xend { // bits 10:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0x7ff;
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
pub const CMAP_ENTRIES_REV11: u32 = 0x1200; // inferred
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
pub const OVERLAY_TRANSPARENT_INDEX: u32 = 0x0; // inferred
pub const PANEL_SETUP_A: u32 = 0x82; // established
pub const PANEL_SETUP_B: u32 = 0x83; // established
pub const PANEL_SETUP_C: u32 = 0x84; // established
pub const REF_CLOCK_KHZ: u32 = 0x4e20; // inferred
pub const TILE_LIST_ALIGN_BYTES: u32 = 0x200; // inferred
pub const TILE_LIST_OUTSTANDING: u32 = 0x8; // inferred
pub const TILE_LIST_PAGE_BYTES: u32 = 0x2000; // inferred
pub const TILE_NUMBER_MASK: u32 = 0x1fff; // established
pub const TWO_WIRE_FP_OFF: u32 = 0x10; // established
pub const TWO_WIRE_MON_OFF: u32 = 0x8; // established
pub const TWO_WIRE_WAIT_BIT: u32 = 0x1; // established
pub const TWO_WIRE_WAIT_LIMIT: u32 = 0x3c; // established
pub const VT_XY_FREEZE: u32 = 0x80000000; // established
