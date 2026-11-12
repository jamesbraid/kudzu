// generated from defs/efs.toml by defs-gen - do not edit, change the source
// Extent File System on-disk structures
#![allow(dead_code)]

/// (established)
pub mod db_w0 {
    pub const WIDTH: u32 = 32;
    pub mod magic { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod firstused { // bits 15:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod slots { // bits 7:0, established
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
/// (established)
pub mod dev_w8 {
    pub const WIDTH: u32 = 32;
    pub mod odev { // bits 31:16, established
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
/// (established)
pub mod dev_w9 {
    pub const WIDTH: u32 = 32;
    pub mod ndev { // bits 31:0, established
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
pub mod di_w0 {
    pub const WIDTH: u32 = 32;
    pub mod mode_type { // bits 31:28, established
        pub const SHIFT: u32 = 28;
        pub const MASK: u32 = 0xf;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const FIFO: u32 = 1; // established
        pub const CHR: u32 = 2; // established
        pub const X3: u32 = 3; // established
        pub const DIR: u32 = 4; // established
        pub const BLK: u32 = 6; // established
        pub const X7: u32 = 7; // established
        pub const REG: u32 = 8; // established
        pub const LNK: u32 = 10; // established
        pub const SOCK: u32 = 12; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Fifo,
            Chr,
            X3,
            Dir,
            Blk,
            X7,
            Reg,
            Lnk,
            Sock,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    1 => Self::Fifo,
                    2 => Self::Chr,
                    3 => Self::X3,
                    4 => Self::Dir,
                    6 => Self::Blk,
                    7 => Self::X7,
                    8 => Self::Reg,
                    10 => Self::Lnk,
                    12 => Self::Sock,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Fifo => 1,
                    Self::Chr => 2,
                    Self::X3 => 3,
                    Self::Dir => 4,
                    Self::Blk => 6,
                    Self::X7 => 7,
                    Self::Reg => 8,
                    Self::Lnk => 10,
                    Self::Sock => 12,
                    Self::Other(v) => v,
                }
            }
        }
    }
    pub mod mode_perm { // bits 27:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xfff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod nlink { // bits 15:0, established
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
pub mod di_w1 {
    pub const WIDTH: u32 = 32;
    pub mod uid { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod gid { // bits 15:0, established
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
pub mod di_w2 {
    pub const WIDTH: u32 = 32;
    pub mod size { // bits 31:0, established
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
pub mod di_w3 {
    pub const WIDTH: u32 = 32;
    pub mod atime { // bits 31:0, established
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
pub mod di_w4 {
    pub const WIDTH: u32 = 32;
    pub mod mtime { // bits 31:0, established
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
pub mod di_w5 {
    pub const WIDTH: u32 = 32;
    pub mod ctime { // bits 31:0, established
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
pub mod di_w6 {
    pub const WIDTH: u32 = 32;
    pub mod gen { // bits 31:0, established
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
pub mod di_w7 {
    pub const WIDTH: u32 = 32;
    pub mod numextents { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod version { // bits 15:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod spare { // bits 7:0, inferred
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
/// (established)
pub mod ext_w0 {
    pub const WIDTH: u32 = 32;
    pub mod magic { // bits 31:24, established
        pub const SHIFT: u32 = 24;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod bn { // bits 23:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod ext_w1 {
    pub const WIDTH: u32 = 32;
    pub mod length { // bits 31:24, established
        pub const SHIFT: u32 = 24;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod offset { // bits 23:0, established
        pub const SHIFT: u32 = 0;
        pub const MASK: u32 = 0xffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod sb_w0 {
    pub const WIDTH: u32 = 32;
    pub mod size { // bits 31:0, established
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
pub mod sb_w1 {
    pub const WIDTH: u32 = 32;
    pub mod firstcg { // bits 31:0, established
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
pub mod sb_w11 {
    pub const WIDTH: u32 = 32;
    pub mod bmsize { // bits 31:0, established
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
pub mod sb_w12 {
    pub const WIDTH: u32 = 32;
    pub mod tfree { // bits 31:0, established
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
pub mod sb_w13 {
    pub const WIDTH: u32 = 32;
    pub mod tinode { // bits 31:0, established
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
pub mod sb_w14 {
    pub const WIDTH: u32 = 32;
    pub mod bmblock { // bits 31:0, established
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
pub mod sb_w15 {
    pub const WIDTH: u32 = 32;
    pub mod replsb { // bits 31:0, established
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
pub mod sb_w16 {
    pub const WIDTH: u32 = 32;
    pub mod lastialloc { // bits 31:0, inferred
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
pub mod sb_w2 {
    pub const WIDTH: u32 = 32;
    pub mod cgfsize { // bits 31:0, established
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
pub mod sb_w22 {
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
pub mod sb_w3 {
    pub const WIDTH: u32 = 32;
    pub mod cgisize { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod sectors { // bits 15:0, established
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
pub mod sb_w4 {
    pub const WIDTH: u32 = 32;
    pub mod heads { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod ncg { // bits 15:0, established
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
pub mod sb_w5 {
    pub const WIDTH: u32 = 32;
    pub mod dirty { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const CLEAN: u32 = 0; // established
        pub const DIRTY: u32 = 2989; // established
        pub const NEEDSCK: u32 = 4660; // established
        pub const MOUNTED: u32 = 30583; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Clean,
            Dirty,
            Needsck,
            Mounted,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    0 => Self::Clean,
                    2989 => Self::Dirty,
                    4660 => Self::Needsck,
                    30583 => Self::Mounted,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Clean => 0,
                    Self::Dirty => 2989,
                    Self::Needsck => 4660,
                    Self::Mounted => 30583,
                    Self::Other(v) => v,
                }
            }
        }
    }
}
/// (established)
pub mod sb_w6 {
    pub const WIDTH: u32 = 32;
    pub mod time { // bits 31:0, established
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
pub mod sb_w7 {
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
        pub const OLD: u32 = 469337; // established
        pub const NEW: u32 = 469338; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Old,
            New,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    469337 => Self::Old,
                    469338 => Self::New,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Old => 469337,
                    Self::New => 469338,
                    Self::Other(v) => v,
                }
            }
        }
    }
}
pub const BBSIZE: u32 = 0x200; // established
pub const BITMAP_BB: u32 = 0x2; // established
pub const DB_SIZE: u32 = 0x200; // established
pub const DENT_HEADER_BYTES: u32 = 0x5; // established
pub const DEV_LONG_FORM: u32 = 0xffff; // established
pub const DIRBLK_HEADERSIZE: u32 = 0x4; // established
pub const DIRBLK_MAGIC: u32 = 0xbeef; // established
pub const DIRBLK_MAXSLOTS: u32 = 0x48; // established
pub const DIRECTEXTENTS: u32 = 0xc; // established
pub const DI_SIZE: u32 = 0x80; // established
pub const EXT_SIZE: u32 = 0x8; // established
pub const EX_MAXLEN: u32 = 0xf8; // established
pub const INODES_PER_BB: u32 = 0x4; // established
pub const INODE_DATA_BYTES: u32 = 0x60; // established
pub const INODE_SIZE: u32 = 0x80; // established
pub const MAGIC_NEW: u32 = 0x7295a; // established
pub const MAGIC_OLD: u32 = 0x72959; // established
pub const ROOT_INO: u32 = 0x2; // established
pub const SB_LEN_FNAME: u32 = 0x6; // established
pub const SB_LEN_FPACK: u32 = 0x6; // established
pub const SB_LEN_SPARE: u32 = 0x14; // inferred
pub const SB_OFF_FNAME: u32 = 0x20; // established
pub const SB_OFF_FPACK: u32 = 0x26; // established
pub const SB_OFF_SPARE: u32 = 0x44; // inferred
pub const SB_SIZE: u32 = 0x5c; // established
pub const SUPER_BB: u32 = 0x1; // established
