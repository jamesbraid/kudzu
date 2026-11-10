// generated from kudzu 5468a0cc81b8 XLV_DECODE.h - do not edit, regenerate in kudzu
//!
//! XLV logical volume manager on-disk label: master block,
//! label slot header and per-partition entry.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: bytes 11 20 19 89.
pub const LABEL_MAGIC: u32 = 0x11201989;
/// established: the value writers store; no reader checks it.
pub const LABEL_VERSION: u32 = 2;
/// established: used bytes of one label slot.
pub const LABEL_BYTES: u32 = 0x12a0;
/// established: master block size before rounding to the sector size.
pub const MASTER_BYTES: u32 = 0x200;
/// established: entries per label, one per partition.
pub const NPART: u32 = 16;
/// established: bytes in a volume, plex or element name.
pub const NAME_LEN: u32 = 32;
/// established: bytes reserved for the owning host name.
pub const NODENAME_LEN: u32 = 0x108;
/// established: bytes the writers copy into the host name.
pub const NODENAME_COPY: u32 = 0x102;
/// established.
pub const ENTRY_SIZE: u32 = 0x118;
/// established.
pub const HDR_SIZE: u32 = 0x120;
/// established: offset of entry 0 in a slot.
pub const ENTRY_OFF: u32 = 0x120;
/// established: volume-header partition type of the header partition.
pub const PTYPE_VOLHDR: u32 = 0;
/// established.
pub const PTYPE_RAW: u32 = 3;
/// established: type 5 is mapped to the name efs by the shipped name table.
pub const PTYPE_EFS_LEGACY: u32 = 5;
/// established: whole disk.
pub const PTYPE_VOLUME: u32 = 6;
/// established.
pub const PTYPE_EFS: u32 = 7;
/// established.
pub const PTYPE_XFS: u32 = 10;
/// established: accepted for a log subvolume.
pub const PTYPE_XFSLOG: u32 = 11;
/// established: accepted for a log, data or real-time subvolume.
pub const PTYPE_XLV: u32 = 12;
/// established.
pub const PTYPE_XVM: u32 = 13;
/// established.
pub const PTYPE_VXVM: u32 = 14;
/// established.
pub const LBL_SIZE: u32 = 0x120;
/// established: owning host name; writers copy 0x102 bytes.
pub const LBL_OFF_NODENAME: u32 = 24;
/// established.
pub const LBL_LEN_NODENAME: u32 = 0x108;
/// established.
pub const E_SIZE: u32 = 0x118;
/// established: volume UUID, opaque bytes.
pub const E_OFF_VOL_UUID: u32 = 4;
/// established.
pub const E_LEN_VOL_UUID: u32 = 16;
/// established: nil if the volume has no log subvolume.
pub const E_OFF_LOG_SV_UUID: u32 = 20;
/// established.
pub const E_LEN_LOG_SV_UUID: u32 = 16;
/// established.
pub const E_OFF_DATA_SV_UUID: u32 = 36;
/// established.
pub const E_LEN_DATA_SV_UUID: u32 = 16;
/// established.
pub const E_OFF_RT_SV_UUID: u32 = 52;
/// established.
pub const E_LEN_RT_SV_UUID: u32 = 16;
/// established: volume name, NUL padded, every byte may be used.
pub const E_OFF_VOL_NAME: u32 = 68;
/// established.
pub const E_LEN_VOL_NAME: u32 = 32;
/// established: opaque bytes.
pub const E_OFF_PLEX_UUID: u32 = 120;
/// established.
pub const E_LEN_PLEX_UUID: u32 = 16;
/// established.
pub const E_OFF_PLEX_NAME: u32 = 136;
/// established.
pub const E_LEN_PLEX_NAME: u32 = 32;
/// established: opaque bytes.
pub const E_OFF_VE_UUID: u32 = 176;
/// established.
pub const E_LEN_VE_UUID: u32 = 16;
/// established.
pub const E_OFF_VE_NAME: u32 = 192;
/// established.
pub const E_LEN_VE_NAME: u32 = 32;
/// established: nil means the entry is unused, opaque bytes.
pub const E_OFF_PART_UUID: u32 = 248;
/// established.
pub const E_LEN_PART_UUID: u32 = 16;

/// Values of `sv_type` in `EW29` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum EW29SvType {
    /// established.
    Log,
    /// established.
    Data,
    /// established.
    Rt,
    /// A value the notes do not name.
    Other(u8),
}

impl EW29SvType {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x1 => Self::Log,
            0x2 => Self::Data,
            0x3 => Self::Rt,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Log => 0x1,
            Self::Data => 0x2,
            Self::Rt => 0x3,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `ve_state` in `EW56` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum EW56VeState {
    /// established.
    Empty,
    /// established.
    Clean,
    /// established.
    Active,
    /// established.
    Stale,
    /// established.
    Offline,
    /// established.
    Incomplete,
    /// A value the notes do not name.
    Other(u8),
}

impl EW56VeState {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::Empty,
            0x1 => Self::Clean,
            0x2 => Self::Active,
            0x3 => Self::Stale,
            0x4 => Self::Offline,
            0x5 => Self::Incomplete,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Empty => 0x0,
            Self::Clean => 0x1,
            Self::Active => 0x2,
            Self::Stale => 0x3,
            Self::Offline => 0x4,
            Self::Incomplete => 0x5,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `ve_type` in `EW56` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum EW56VeType {
    /// established.
    Stripe,
    /// established.
    Concat,
    /// A value the notes do not name.
    Other(u8),
}

impl EW56VeType {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x1 => Self::Stripe,
            0x2 => Self::Concat,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Stripe => 0x1,
            Self::Concat => 0x2,
            Self::Other(v) => v as u32,
        }
    }
}

/// The `MASTER_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct MasterW0(u32);

impl MasterW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn seq_hi(self) -> u32 {
        self.0
    }
}

/// The `MASTER_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct MasterW1(u32);

impl MasterW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:1, established.
    pub const fn seq_lo_rest(self) -> u32 {
        self.0 >> 1
    }

    /// Bit 0, established.
    pub const fn seq_lo_slot(self) -> bool {
        self.0 & 1 != 0
    }
}

/// The `LBL_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LblW0(u32);

impl LblW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn magic(self) -> u32 {
        self.0
    }
}

/// The `LBL_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LblW1(u32);

impl LblW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn version(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:8, established.
    pub const fn nparts(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, unknown.
    pub const fn reserved(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `LBL_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LblW2(u32);

impl LblW2 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, unknown.
    pub const fn timestamp_hi(self) -> u32 {
        self.0
    }
}

/// The `LBL_W3` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LblW3(u32);

impl LblW3 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, unknown.
    pub const fn timestamp_lo(self) -> u32 {
        self.0
    }
}

/// The `LBL_W4` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LblW4(u32);

impl LblW4 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn trans_id_hi(self) -> u32 {
        self.0
    }
}

/// The `LBL_W5` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LblW5(u32);

impl LblW5 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn trans_id_lo(self) -> u32 {
        self.0
    }
}

/// The `E_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW0(u32);

impl EW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn vol_flags(self) -> u32 {
        self.0
    }
}

/// The `E_W25` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW25(u32);

impl EW25 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bit 31, established.
    pub const fn sv_flags_hi_no_err_retry(self) -> bool {
        (self.0 >> 31) & 1 != 0
    }

    /// Bits 30:16, unknown.
    pub const fn sv_flags_hi_other(self) -> u32 {
        (self.0 >> 16) & 0x7fff
    }

    /// Bits 15:0, established.
    pub const fn sv_flags_hi_low(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `E_W27` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW27(u32);

impl EW27 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn time(self) -> u32 {
        self.0
    }
}

/// The `E_W28` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW28(u32);

impl EW28 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn vol_attr16(self) -> u32 {
        self.0
    }
}

/// The `E_W29` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW29(u32);

impl EW29 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn sv_type(self) -> EW29SvType {
        EW29SvType::from_bits(self.0 >> 24)
    }

    /// Bits 15:0, established.
    pub const fn sv_depth(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `E_W42` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW42(u32);

impl EW42 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn sv_nplex(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:16, established.
    pub const fn plex_index(self) -> u32 {
        (self.0 >> 16) & 0xff
    }

    /// Bits 15:0, established.
    pub const fn plex_nve(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `E_W43` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW43(u32);

impl EW43 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn plex_flags(self) -> u32 {
        self.0 >> 16
    }
}

/// The `E_W56` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW56(u32);

impl EW56 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn ve_state(self) -> EW56VeState {
        EW56VeState::from_bits(self.0 >> 24)
    }

    /// Bits 23:16, established.
    pub const fn ve_type(self) -> EW56VeType {
        EW56VeType::from_bits((self.0 >> 16) & 0xff)
    }

    /// Bits 15:8, established.
    pub const fn ve_nparts(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, established.
    pub const fn ve_index(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `E_W57` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW57(u32);

impl EW57 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn ve_stripe_unit(self) -> u32 {
        self.0 >> 16
    }
}

/// The `E_W58` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW58(u32);

impl EW58 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ve_start_hi(self) -> u32 {
        self.0
    }
}

/// The `E_W59` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW59(u32);

impl EW59 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ve_start_lo(self) -> u32 {
        self.0
    }
}

/// The `E_W60` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW60(u32);

impl EW60 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ve_end_hi(self) -> u32 {
        self.0
    }
}

/// The `E_W61` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW61(u32);

impl EW61 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ve_end_lo(self) -> u32 {
        self.0
    }
}

/// The `E_W66` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW66(u32);

impl EW66 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn part_dev(self) -> u32 {
        self.0
    }
}

/// The `E_W67` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW67(u32);

impl EW67 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn part_size(self) -> u32 {
        self.0
    }
}

/// The `E_W68` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct EW68(u32);

impl EW68 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn part_index(self) -> u32 {
        self.0 >> 24
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn testvec_e_w25_80000000() {
        let w = EW25::new(0x80000000);
        assert_eq!(w.sv_flags_hi_no_err_retry() as u32, 1, "sv_flags_hi_no_err_retry");
        assert_eq!(w.sv_flags_hi_other(), 0, "sv_flags_hi_other");
        assert_eq!(w.sv_flags_hi_low(), 0, "sv_flags_hi_low");
    }
}
