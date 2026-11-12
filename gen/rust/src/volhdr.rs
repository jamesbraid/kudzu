// generated from defs/volhdr.toml by defs-gen - do not edit, change the source
// SGI disk volume header
#![allow(dead_code)]

/// (established)
pub mod dirent_w2 {
    pub const WIDTH: u32 = 32;
    pub mod start_block { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod dirent_w3 {
    pub const WIDTH: u32 = 32;
    pub mod length_bytes { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod hdr_w0 {
    pub const WIDTH: u32 = 32;
    pub mod magic { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod hdr_w1 {
    pub const WIDTH: u32 = 32;
    pub mod root { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod swap { // bits 15:0, established
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
pub mod hdr_w10 {
    pub const WIDTH: u32 = 32;
    pub mod dp_secbytes { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (inferred)
pub mod hdr_w11 {
    pub const WIDTH: u32 = 32;
    pub mod dp_flags { // bits 31:0, inferred
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod hdr_w126 {
    pub const WIDTH: u32 = 32;
    pub mod checksum { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod hdr_w17 {
    pub const WIDTH: u32 = 32;
    pub mod dp_capacity { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (inferred)
pub mod hdr_w7 {
    pub const WIDTH: u32 = 32;
    pub mod dp_cylinders { // bits 31:16, inferred
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod dp_head_offset { // bits 15:0, unknown
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
pub mod hdr_w8 {
    pub const WIDTH: u32 = 32;
    pub mod dp_heads { // bits 31:16, inferred
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod dp_tag_depth { // bits 15:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod dp_cyl_high { // bits 7:0, unknown
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
/// (inferred)
pub mod hdr_w9 {
    pub const WIDTH: u32 = 32;
    pub mod dp_sectors { // bits 15:0, inferred
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
pub mod part_w0 {
    pub const WIDTH: u32 = 32;
    pub mod nblks { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod part_w1 {
    pub const WIDTH: u32 = 32;
    pub mod first { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod part_w2 {
    pub const WIDTH: u32 = 32;
    pub mod r#type { // bits 31:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const VOLHDR: u32 = 0; // established
        pub const TRKREPL: u32 = 1; // inferred
        pub const SECREPL: u32 = 2; // inferred
        pub const RAW: u32 = 3; // established
        pub const BSD: u32 = 4; // inferred
        pub const SYSV: u32 = 5; // inferred
        pub const VOLUME: u32 = 6; // established
        pub const EFS: u32 = 7; // established
        pub const LVOL: u32 = 8; // established
        pub const RLVOL: u32 = 9; // established
        pub const XFS: u32 = 10; // established
        pub const XFSLOG: u32 = 11; // established
        pub const XLV: u32 = 12; // established
        pub const XVM: u32 = 13; // established
        pub const VXVM: u32 = 14; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Volhdr,
            Trkrepl,
            Secrepl,
            Raw,
            Bsd,
            Sysv,
            Volume,
            Efs,
            Lvol,
            Rlvol,
            Xfs,
            Xfslog,
            Xlv,
            Xvm,
            Vxvm,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    0 => Self::Volhdr,
                    1 => Self::Trkrepl,
                    2 => Self::Secrepl,
                    3 => Self::Raw,
                    4 => Self::Bsd,
                    5 => Self::Sysv,
                    6 => Self::Volume,
                    7 => Self::Efs,
                    8 => Self::Lvol,
                    9 => Self::Rlvol,
                    10 => Self::Xfs,
                    11 => Self::Xfslog,
                    12 => Self::Xlv,
                    13 => Self::Xvm,
                    14 => Self::Vxvm,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Volhdr => 0,
                    Self::Trkrepl => 1,
                    Self::Secrepl => 2,
                    Self::Raw => 3,
                    Self::Bsd => 4,
                    Self::Sysv => 5,
                    Self::Volume => 6,
                    Self::Efs => 7,
                    Self::Lvol => 8,
                    Self::Rlvol => 9,
                    Self::Xfs => 10,
                    Self::Xfslog => 11,
                    Self::Xlv => 12,
                    Self::Xvm => 13,
                    Self::Vxvm => 14,
                    Self::Other(v) => v,
                }
            }
        }
    }
}
pub const BOOTFILE_OFF: u32 = 0x8; // established
pub const CSUM_OFF: u32 = 0x1f8; // established
pub const DIRENT_LEN_NAME: u32 = 0x8; // established
pub const DIRENT_OFF_NAME: u32 = 0x0; // established
pub const DIRENT_SIZE: u32 = 0x10; // established
pub const DIR_COUNT: u32 = 0xf; // established
pub const DIR_NAME_LEN: u32 = 0x8; // established
pub const DIR_OFF: u32 = 0x48; // established
pub const DIR_STRIDE: u32 = 0x10; // established
pub const DP_BYTES: u32 = 0x30; // established
pub const DP_FLAG_TAGGED: u32 = 0x40; // inferred
pub const DP_OFF: u32 = 0x18; // established
pub const HDR_LEN_BOOTFILE: u32 = 0x10; // established
pub const HDR_OFF_BOOTFILE: u32 = 0x8; // established
pub const HDR_SIZE: u32 = 0x200; // established
pub const MAGIC: u32 = 0xbe5a941; // established
pub const PART_COUNT: u32 = 0x10; // established
pub const PART_OFF: u32 = 0x138; // established
pub const PART_SIZE: u32 = 0xc; // established
pub const PART_STRIDE: u32 = 0xc; // established
pub const SIZE: u32 = 0x200; // established
pub const SLOT_ROOT: u32 = 0x0; // established
pub const SLOT_SWAP: u32 = 0x1; // established
pub const SLOT_VOLHDR: u32 = 0x8; // established
pub const SLOT_WHOLEDISK: u32 = 0xa; // established

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn dirent_w3_41400() {
        assert_eq!(dirent_w3::length_bytes::get(0x41400), 267264);
    }

    #[test]
    fn part_w0_1000() {
        assert_eq!(part_w0::nblks::get(0x1000), 4096);
    }

    #[test]
    fn part_w0_40000() {
        assert_eq!(part_w0::nblks::get(0x40000), 262144);
    }

    #[test]
    fn part_w1_41000() {
        assert_eq!(part_w1::first::get(0x41000), 266240);
    }
}
