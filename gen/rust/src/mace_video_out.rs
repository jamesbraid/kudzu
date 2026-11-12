// generated from defs/mace_video_out.toml by defs-gen - do not edit, change the source
// O2 MACE video output channel registers as the IRIX O2 video driver reaches them
#![allow(dead_code)]

/// Channel control word. The driver writes the whole word, changing only its low nine bits. The output recovery routines pulse bit 0. The reading read th (inferred)
pub mod out_control {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x0;
    pub mod intr_enable { // bits 8:1, inferred
        pub const SHIFT: u32 = 1;
        pub const MASK: u64 = 0xff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod dma_enable { // bits 0:0, inferred
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
/// Status word. Reading it clears the pending interrupt causes. The output recovery routines read it after their last write and discard the value. The pa (inferred)
pub mod out_status {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x8;
}
/// Configuration word, written at start-up. Its field layout is not recorded here; the hardware configuration register at at 0x58 is a separate word. (established)
pub mod out_config {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x10;
}
/// Physical address of the next descriptor block, held as a zero-extended 32-bit word. The recovery routines write it as well. Which bits carry the flag  (inferred)
pub mod out_next_descriptor {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x18;
}
/// Field offset. The driver writes the low halfword of a transfer-record word here. (inferred)
pub mod out_field_offset {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x20;
    pub mod offset { // bits 15:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0xffff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Field size. The reading takes the line count and the bytes per line from one stored value, which it gives as 243 lines of 2880 bytes in one set and 28 (inferred)
pub mod out_field_size {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x28;
    pub mod lines { // bits 21:12, inferred
        pub const SHIFT: u32 = 12;
        pub const MASK: u64 = 0x3ff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod line_bytes { // bits 11:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u64 = 0xfff;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// Horizontal padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded. (established)
pub mod out_h_pad_odd {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x30;
}
/// Vertical padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded. (established)
pub mod out_v_pad_odd {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x38;
}
/// Horizontal padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded. (established)
pub mod out_h_pad_even {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x40;
}
/// Vertical padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded. (established)
pub mod out_v_pad_even {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x48;
}
/// Genlock delay word, written at start-up with a 64-bit store. Its layout is not recorded. (established)
pub mod out_genlock_delay {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x50;
}
/// Hardware configuration word for the output channel: port reset and select bits, audio and genlock sync settings, the general-purpose output bit and th (inferred)
pub mod out_hw_config {
    pub const WIDTH: u32 = 64;
    pub const OFFSET: u64 = 0x58;
    pub mod ident { // bits 35:32, inferred
        pub const SHIFT: u32 = 32;
        pub const MASK: u64 = 0xf;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod sof_count_even { // bits 22:19, inferred
        pub const SHIFT: u32 = 19;
        pub const MASK: u64 = 0xf;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod general_output { // bits 18:18, inferred
        pub const SHIFT: u32 = 18;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod sof_count_odd { // bits 17:14, inferred
        pub const SHIFT: u32 = 14;
        pub const MASK: u64 = 0xf;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod framelock_correction { // bits 13:13, inferred
        pub const SHIFT: u32 = 13;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod framelock_precision { // bits 12:12, inferred
        pub const SHIFT: u32 = 12;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod framelock_source { // bits 11:10, inferred
        pub const SHIFT: u32 = 10;
        pub const MASK: u64 = 0x3;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod framelock_reset { // bits 9:9, inferred
        pub const SHIFT: u32 = 9;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod audio_sync_source { // bits 6:5, inferred
        pub const SHIFT: u32 = 5;
        pub const MASK: u64 = 0x3;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod port_cd_select { // bits 3:3, inferred
        pub const SHIFT: u32 = 3;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod port_cd_reset { // bits 2:2, inferred
        pub const SHIFT: u32 = 2;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod port_ab_select { // bits 1:1, inferred
        pub const SHIFT: u32 = 1;
        pub const MASK: u64 = 0x1;
        pub const fn get(v: u64) -> u64 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u64, x: u64) -> u64 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod port_ab_reset { // bits 0:0, inferred
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
