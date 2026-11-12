// generated from defs/klconfig.toml by defs-gen - do not edit, change the source
// hardware configuration board record
#![allow(dead_code)]

/// (established)
pub mod brd_w0 {
    pub const WIDTH: u32 = 32;
    pub mod next { // bits 31:0, established
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
pub mod brd_w1 {
    pub const WIDTH: u32 = 32;
    pub mod class { // bits 23:20, established
        pub const SHIFT: u32 = 20;
        pub const MASK: u32 = 0xf;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const NODE: u32 = 1; // established
        pub const IO: u32 = 2; // inferred
        pub const ROUTER: u32 = 3; // inferred
        pub const GRAPHICS: u32 = 5; // inferred
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Node,
            Io,
            Router,
            Graphics,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    1 => Self::Node,
                    2 => Self::Io,
                    3 => Self::Router,
                    5 => Self::Graphics,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Node => 1,
                    Self::Io => 2,
                    Self::Router => 3,
                    Self::Graphics => 5,
                    Self::Other(v) => v,
                }
            }
        }
    }
    pub mod type_low { // bits 19:16, inferred
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xf;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod brd_w2 {
    pub const WIDTH: u32 = 32;
    pub mod flags_master_baseio { // bits 21:21, established
        pub const SHIFT: u32 = 21;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod slot { // bits 15:8, established
        pub const SHIFT: u32 = 8;
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
pub mod brd_w3 {
    pub const WIDTH: u32 = 32;
    pub mod module { // bits 15:0, established
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
pub mod brd_w5 {
    pub const WIDTH: u32 = 32;
    pub mod numcompts { // bits 7:0, established
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
pub mod brd_w8 {
    pub const WIDTH: u32 = 32;
    pub mod nasid { // bits 31:16, established
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
pub mod comp_w0 {
    pub const WIDTH: u32 = 32;
    pub mod struct_type { // bits 31:24, established
        pub const SHIFT: u32 = 24;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const HUB: u32 = 2; // established
        pub const MEMORY: u32 = 3; // inferred
        pub const IO: u32 = 5; // inferred
        pub const ROUTER: u32 = 9; // inferred
        pub const GRAPHICS: u32 = 10; // inferred
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Hub,
            Memory,
            Io,
            Router,
            Graphics,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    2 => Self::Hub,
                    3 => Self::Memory,
                    5 => Self::Io,
                    9 => Self::Router,
                    10 => Self::Graphics,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Hub => 2,
                    Self::Memory => 3,
                    Self::Io => 5,
                    Self::Router => 9,
                    Self::Graphics => 10,
                    Self::Other(v) => v,
                }
            }
        }
    }
    pub mod flags_enabled { // bits 8:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
}
/// (established)
pub mod comp_w6 {
    pub const WIDTH: u32 = 32;
    pub mod physid { // bits 31:24, established
        pub const SHIFT: u32 = 24;
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
pub mod cons_w2 {
    pub const WIDTH: u32 = 32;
    pub mod config_base_hi { // bits 31:0, established
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
pub mod cons_w3 {
    pub const WIDTH: u32 = 32;
    pub mod config_base_lo { // bits 31:0, established
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
pub mod cons_w4 {
    pub const WIDTH: u32 = 32;
    pub mod memory_base_hi { // bits 31:0, established
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
pub mod cons_w5 {
    pub const WIDTH: u32 = 32;
    pub mod memory_base_widget { // bits 31:24, established
        pub const SHIFT: u32 = 24;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod memory_base_rest { // bits 23:0, established
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
pub mod cons_w8 {
    pub const WIDTH: u32 = 32;
    pub mod nasid { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod npci { // bits 7:0, established
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
pub mod gda_w97 {
    pub const WIDTH: u32 = 32;
    pub mod pebrick_marker { // bits 31:0, established
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
pub mod mem_w14 {
    pub const WIDTH: u32 = 32;
    pub mod dimm_select { // bits 15:0, established
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
pub mod nodeclass_w1 {
    pub const WIDTH: u32 = 32;
    pub mod class { // bits 15:12, established
        pub const SHIFT: u32 = 12;
        pub const MASK: u32 = 0xf;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const PEBRICK: u32 = 12; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Pebrick,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    12 => Self::Pebrick,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Pebrick => 12,
                    Self::Other(v) => v,
                }
            }
        }
    }
}
/// (established)
pub mod pepair_w0 {
    pub const WIDTH: u32 = 32;
    pub mod pe_nasid { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod host_nasid { // bits 15:0, established
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
pub const BRD_OFF_COMPTS: u32 = 0x24; // established
pub const BRD_STRIDE_COMPTS: u32 = 0x4; // established
pub const DIR_CONFIG_OFFSET_MASK: u64 = 0xffffffffff; // established
pub const DIR_OFF: u32 = 0x2000; // established
pub const DIR_OFF_CONFIG_OFFSET: u32 = 0x48; // established
pub const GDA_COUNT_NASIDTABLE_IP27: u32 = 0x40; // established
pub const GDA_COUNT_NASIDTABLE_IP35: u32 = 0x80; // established
pub const GDA_COUNT_PEBRICK: u32 = 0x4; // established
pub const GDA_OFF_NASIDTABLE: u32 = 0x80; // established
pub const GDA_OFF_PEBRICK: u32 = 0x188; // established
pub const GDA_PEBRICK_MAGIC: u32 = 0x5045626b; // established
pub const GDA_STRIDE_NASIDTABLE: u32 = 0x2; // established
pub const GDA_STRIDE_PEBRICK: u32 = 0x4; // established
pub const HDR_OFF_BOARD_INFO: u32 = 0x14; // established
pub const MEM_COUNT_BNKSZ: u32 = 0x8; // established
pub const MEM_OFF_BNKSZ: u32 = 0x3c; // established
pub const MEM_STRIDE_BNKSZ: u32 = 0x2; // established
pub const NODECLASS_OFF: u32 = 0x4000000; // established
pub const NODECLASS_SPACE_LOCAL: u32 = 0x10000000; // established
pub const NODECLASS_SPACE_REMOTE: u32 = 0x30000000; // established
