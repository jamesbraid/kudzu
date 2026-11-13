// generated from formats/xlv/format.toml by defs-gen - do not edit, change the source
// XLV logical volume manager on-disk label structures
#![allow(dead_code)]

/// (established)
pub mod e_w0 {
    pub const WIDTH: u32 = 32;
    pub mod vol_flags { // bits 31:0, established
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
/// (unknown)
pub mod e_w25 {
    pub const WIDTH: u32 = 32;
    pub mod sv_flags_hi_no_err_retry { // bits 31:31, established
        pub const SHIFT: u32 = 31;
        pub const MASK: u32 = 0x1;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod sv_flags_hi_other { // bits 30:16, unknown
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0x7fff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod sv_flags_hi_low { // bits 15:0, established
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
pub mod e_w27 {
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
pub mod e_w28 {
    pub const WIDTH: u32 = 32;
    pub mod vol_attr16 { // bits 31:0, established
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
pub mod e_w29 {
    pub const WIDTH: u32 = 32;
    pub mod sv_type { // bits 31:24, established
        pub const SHIFT: u32 = 24;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const LOG: u32 = 1; // established
        pub const DATA: u32 = 2; // established
        pub const RT: u32 = 3; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Log,
            Data,
            Rt,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    1 => Self::Log,
                    2 => Self::Data,
                    3 => Self::Rt,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Log => 1,
                    Self::Data => 2,
                    Self::Rt => 3,
                    Self::Other(v) => v,
                }
            }
        }
    }
    pub mod sv_depth { // bits 15:0, established
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
pub mod e_w42 {
    pub const WIDTH: u32 = 32;
    pub mod sv_nplex { // bits 31:24, established
        pub const SHIFT: u32 = 24;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod plex_index { // bits 23:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod plex_nve { // bits 15:0, established
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
pub mod e_w43 {
    pub const WIDTH: u32 = 32;
    pub mod plex_flags { // bits 31:16, established
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
pub mod e_w56 {
    pub const WIDTH: u32 = 32;
    pub mod ve_state { // bits 31:24, established
        pub const SHIFT: u32 = 24;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const EMPTY: u32 = 0; // established
        pub const CLEAN: u32 = 1; // established
        pub const ACTIVE: u32 = 2; // established
        pub const STALE: u32 = 3; // established
        pub const OFFLINE: u32 = 4; // established
        pub const INCOMPLETE: u32 = 5; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Empty,
            Clean,
            Active,
            Stale,
            Offline,
            Incomplete,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    0 => Self::Empty,
                    1 => Self::Clean,
                    2 => Self::Active,
                    3 => Self::Stale,
                    4 => Self::Offline,
                    5 => Self::Incomplete,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Empty => 0,
                    Self::Clean => 1,
                    Self::Active => 2,
                    Self::Stale => 3,
                    Self::Offline => 4,
                    Self::Incomplete => 5,
                    Self::Other(v) => v,
                }
            }
        }
    }
    pub mod ve_type { // bits 23:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
        pub const STRIPE: u32 = 1; // established
        pub const CONCAT: u32 = 2; // established
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub enum Value {
            Stripe,
            Concat,
            Other(u32),
        }
        impl Value {
            pub const fn from_raw(v: u32) -> Self {
                match v {
                    1 => Self::Stripe,
                    2 => Self::Concat,
                    _ => Self::Other(v),
                }
            }
            pub const fn raw(self) -> u32 {
                match self {
                    Self::Stripe => 1,
                    Self::Concat => 2,
                    Self::Other(v) => v,
                }
            }
        }
    }
    pub mod ve_nparts { // bits 15:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod ve_index { // bits 7:0, established
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
pub mod e_w57 {
    pub const WIDTH: u32 = 32;
    pub mod ve_stripe_unit { // bits 31:16, established
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
pub mod e_w58 {
    pub const WIDTH: u32 = 32;
    pub mod ve_start_hi { // bits 31:0, established
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
pub mod e_w59 {
    pub const WIDTH: u32 = 32;
    pub mod ve_start_lo { // bits 31:0, established
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
pub mod e_w60 {
    pub const WIDTH: u32 = 32;
    pub mod ve_end_hi { // bits 31:0, established
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
pub mod e_w61 {
    pub const WIDTH: u32 = 32;
    pub mod ve_end_lo { // bits 31:0, established
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
pub mod e_w66 {
    pub const WIDTH: u32 = 32;
    pub mod part_dev { // bits 31:0, established
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
pub mod e_w67 {
    pub const WIDTH: u32 = 32;
    pub mod part_size { // bits 31:0, established
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
pub mod e_w68 {
    pub const WIDTH: u32 = 32;
    pub mod part_index { // bits 31:24, established
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
pub mod lbl_w0 {
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
/// (unknown)
pub mod lbl_w1 {
    pub const WIDTH: u32 = 32;
    pub mod version { // bits 31:16, established
        pub const SHIFT: u32 = 16;
        pub const MASK: u32 = 0xffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod nparts { // bits 15:8, established
        pub const SHIFT: u32 = 8;
        pub const MASK: u32 = 0xff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod reserved { // bits 7:0, unknown
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
/// (unknown)
pub mod lbl_w2 {
    pub const WIDTH: u32 = 32;
    pub mod timestamp_hi { // bits 31:0, unknown
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
/// (unknown)
pub mod lbl_w3 {
    pub const WIDTH: u32 = 32;
    pub mod timestamp_lo { // bits 31:0, unknown
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
pub mod lbl_w4 {
    pub const WIDTH: u32 = 32;
    pub mod trans_id_hi { // bits 31:0, established
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
pub mod lbl_w5 {
    pub const WIDTH: u32 = 32;
    pub mod trans_id_lo { // bits 31:0, established
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
pub mod master_w0 {
    pub const WIDTH: u32 = 32;
    pub mod seq_hi { // bits 31:0, established
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
pub mod master_w1 {
    pub const WIDTH: u32 = 32;
    pub mod seq_lo_rest { // bits 31:1, established
        pub const SHIFT: u32 = 1;
        pub const MASK: u32 = 0x7fffffff;
        pub const fn get(v: u32) -> u32 {
            (v >> SHIFT) & MASK
        }
        pub const fn set(v: u32, x: u32) -> u32 {
            (v & !(MASK << SHIFT)) | ((x & MASK) << SHIFT)
        }
    }
    pub mod seq_lo_slot { // bits 0:0, established
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
pub const ENTRY_OFF: u32 = 0x120; // established
pub const ENTRY_SIZE: u32 = 0x118; // established
pub const E_LEN_DATA_SV_UUID: u32 = 0x10; // established
pub const E_LEN_LOG_SV_UUID: u32 = 0x10; // established
pub const E_LEN_PART_UUID: u32 = 0x10; // established
pub const E_LEN_PLEX_NAME: u32 = 0x20; // established
pub const E_LEN_PLEX_UUID: u32 = 0x10; // established
pub const E_LEN_RT_SV_UUID: u32 = 0x10; // established
pub const E_LEN_VE_NAME: u32 = 0x20; // established
pub const E_LEN_VE_UUID: u32 = 0x10; // established
pub const E_LEN_VOL_NAME: u32 = 0x20; // established
pub const E_LEN_VOL_UUID: u32 = 0x10; // established
pub const E_OFF_DATA_SV_UUID: u32 = 0x24; // established
pub const E_OFF_LOG_SV_UUID: u32 = 0x14; // established
pub const E_OFF_PART_UUID: u32 = 0xf8; // established
pub const E_OFF_PLEX_NAME: u32 = 0x88; // established
pub const E_OFF_PLEX_UUID: u32 = 0x78; // established
pub const E_OFF_RT_SV_UUID: u32 = 0x34; // established
pub const E_OFF_VE_NAME: u32 = 0xc0; // established
pub const E_OFF_VE_UUID: u32 = 0xb0; // established
pub const E_OFF_VOL_NAME: u32 = 0x44; // established
pub const E_OFF_VOL_UUID: u32 = 0x4; // established
pub const E_SIZE: u32 = 0x118; // established
pub const HDR_SIZE: u32 = 0x120; // established
pub const LABEL_BYTES: u32 = 0x12a0; // established
pub const LABEL_MAGIC: u32 = 0x11201989; // established
pub const LABEL_VERSION: u32 = 0x2; // established
pub const LBL_LEN_NODENAME: u32 = 0x108; // established
pub const LBL_OFF_NODENAME: u32 = 0x18; // established
pub const LBL_SIZE: u32 = 0x120; // established
pub const MASTER_BYTES: u32 = 0x200; // established
pub const NAME_LEN: u32 = 0x20; // established
pub const NODENAME_COPY: u32 = 0x102; // established
pub const NODENAME_LEN: u32 = 0x108; // established
pub const NPART: u32 = 0x10; // established
pub const PTYPE_EFS: u32 = 0x7; // established
pub const PTYPE_EFS_LEGACY: u32 = 0x5; // established
pub const PTYPE_RAW: u32 = 0x3; // established
pub const PTYPE_VOLHDR: u32 = 0x0; // established
pub const PTYPE_VOLUME: u32 = 0x6; // established
pub const PTYPE_VXVM: u32 = 0xe; // established
pub const PTYPE_XFS: u32 = 0xa; // established
pub const PTYPE_XFSLOG: u32 = 0xb; // established
pub const PTYPE_XLV: u32 = 0xc; // established
pub const PTYPE_XVM: u32 = 0xd; // established

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn e_w25_80000000() {
        assert_eq!(e_w25::sv_flags_hi_no_err_retry::get(0x80000000), 1);
        assert_eq!(e_w25::sv_flags_hi_other::get(0x80000000), 0);
        assert_eq!(e_w25::sv_flags_hi_low::get(0x80000000), 0);
    }
}
