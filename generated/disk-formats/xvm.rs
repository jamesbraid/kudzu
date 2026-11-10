// generated from kudzu 5468a0cc81b8 XVM_DECODE.h - do not edit, regenerate in kudzu
//!
//! XVM volume manager on-disk label structures: the
//! physical label, label sector header, element and attribute entries.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: bytes 78 6c 61 62.
pub const PHYSLAB_MAGIC: u32 = 0x786c6162;
/// established.
pub const PHYSLAB_VERSION: u32 = 2;
/// established: converted on read.
pub const PHYSLAB_VERSION_LEGACY: u32 = 1;
/// established: device block holding the label on a disk with an SGI volume header.
pub const PHYSLAB_START_BLOCK: u32 = 1;
/// established: bytes 6c 64 68 72.
pub const LABELHDR_MAGIC: u32 = 0x6c646872;
/// established.
pub const LABELHDR_VERSION: u32 = 2;
/// established: accepted for element sectors only.
pub const LABELHDR_VERSION_LEGACY: u32 = 1;
/// established: bytes per entry in a label sector.
pub const LABENT_SIZE: u32 = 72;
/// established: bytes in an object name.
pub const NAME_MAX: u32 = 48;
/// established.
pub const HOSTNAME_MAX: u32 = 32;
/// established.
pub const DISKNAME_MAX: u32 = 48;
/// established.
pub const PL_SIZE: u32 = 124;
/// established: owning host in the private domain; a version 1 label held the disk name here.
pub const PL_OFF_HOSTNAME: u32 = 8;
/// established.
pub const PL_LEN_HOSTNAME: u32 = 32;
/// established: physical volume UUID, opaque bytes.
pub const PL_OFF_UUID: u32 = 40;
/// established.
pub const PL_LEN_UUID: u32 = 16;
/// established: physical volume name, NUL padded.
pub const PL_OFF_DISKNAME: u32 = 72;
/// established.
pub const PL_LEN_DISKNAME: u32 = 48;
/// established.
pub const LH_SIZE: u32 = 8;
/// established.
pub const VE_SIZE: u32 = 72;
/// established: this object, opaque bytes.
pub const VE_OFF_UUID: u32 = 8;
/// established.
pub const VE_LEN_UUID: u32 = 16;
/// established: the child at the piece position, opaque bytes.
pub const VE_OFF_CHILDUUID: u32 = 24;
/// established.
pub const VE_LEN_CHILDUUID: u32 = 16;
/// established: repository object UUID, opaque bytes; no word fields.
pub const VE_COW_OFF_RUUID: u32 = 56;
/// established.
pub const VE_COW_LEN_RUUID: u32 = 16;
/// established.
pub const ATTR_SIZE: u32 = 72;
/// established: the object named or protected, opaque bytes.
pub const ATTR_OFF_UUID: u32 = 8;
/// established.
pub const ATTR_LEN_UUID: u32 = 16;
/// established: for a name entry: NUL padded string.
pub const ATTR_OFF_NAME: u32 = 24;
/// established.
pub const ATTR_LEN_NAME: u32 = 48;

/// Values of `type` in `LhW1` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum LhW1Type {
    /// established.
    Free,
    /// unknown.
    Reserved,
    /// established.
    Ve,
    /// established.
    Attr,
    /// A value the notes do not name.
    Other(u8),
}

impl LhW1Type {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::Free,
            0x1 => Self::Reserved,
            0x2 => Self::Ve,
            0x3 => Self::Attr,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Free => 0x0,
            Self::Reserved => 0x1,
            Self::Ve => 0x2,
            Self::Attr => 0x3,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `type` in `VeW0` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum VeW0Type {
    /// established.
    Free,
    /// established.
    Slice,
    /// established.
    Concat,
    /// established.
    Stripe,
    /// established.
    Raid,
    /// established.
    Mirror,
    /// established.
    Subvol,
    /// established.
    Volume,
    /// established.
    Cow,
    /// established.
    Snapshot,
    /// A value the notes do not name.
    Other(u8),
}

impl VeW0Type {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::Free,
            0x1 => Self::Slice,
            0x2 => Self::Concat,
            0x3 => Self::Stripe,
            0x4 => Self::Raid,
            0x5 => Self::Mirror,
            0x6 => Self::Subvol,
            0x7 => Self::Volume,
            0xb => Self::Cow,
            0xc => Self::Snapshot,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Free => 0x0,
            Self::Slice => 0x1,
            Self::Concat => 0x2,
            Self::Stripe => 0x3,
            Self::Raid => 0x4,
            Self::Mirror => 0x5,
            Self::Subvol => 0x6,
            Self::Volume => 0x7,
            Self::Cow => 0xb,
            Self::Snapshot => 0xc,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `policy` in `VeMirrorW16` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum VeMirrorW16Policy {
    /// established.
    RoundRobin,
    /// established.
    Sequential,
    /// A value the notes do not name.
    Other(u8),
}

impl VeMirrorW16Policy {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::RoundRobin,
            0x1 => Self::Sequential,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::RoundRobin => 0x0,
            Self::Sequential => 0x1,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `svtype` in `VeSubvolW14` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum VeSubvolW14Svtype {
    /// established.
    All,
    /// established.
    Log,
    /// established.
    Data,
    /// established.
    Rt,
    /// established.
    Swap,
    /// established.
    User1,
    /// A value the notes do not name.
    Other(u8),
}

impl VeSubvolW14Svtype {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::All,
            0x1 => Self::Log,
            0x2 => Self::Data,
            0x3 => Self::Rt,
            0x4 => Self::Swap,
            0x10 => Self::User1,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::All => 0x0,
            Self::Log => 0x1,
            Self::Data => 0x2,
            Self::Rt => 0x3,
            Self::Swap => 0x4,
            Self::User1 => 0x10,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `type` in `AttrW0` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum AttrW0Type {
    /// established.
    Free,
    /// established.
    Name,
    /// established.
    Perm,
    /// A value the notes do not name.
    Other(u8),
}

impl AttrW0Type {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::Free,
            0x1 => Self::Name,
            0x2 => Self::Perm,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Free => 0x0,
            Self::Name => 0x1,
            Self::Perm => 0x2,
            Self::Other(v) => v as u32,
        }
    }
}

/// The `PL_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PlW0(u32);

impl PlW0 {
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

/// The `PL_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PlW1(u32);

impl PlW1 {
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

    /// Bit 14, inferred.
    pub const fn state_draining(self) -> bool {
        (self.0 >> 14) & 1 != 0
    }

    /// Bit 13, established.
    pub const fn state_cluster(self) -> bool {
        (self.0 >> 13) & 1 != 0
    }

    /// Bit 12, established.
    pub const fn state_private(self) -> bool {
        (self.0 >> 12) & 1 != 0
    }

    /// Bit 11, inferred.
    pub const fn state_recovering(self) -> bool {
        (self.0 >> 11) & 1 != 0
    }

    /// Bit 10, inferred.
    pub const fn state_mediaerror(self) -> bool {
        (self.0 >> 10) & 1 != 0
    }

    /// Bit 9, inferred.
    pub const fn state_spare(self) -> bool {
        (self.0 >> 9) & 1 != 0
    }

    /// Bit 8, inferred.
    pub const fn state_online(self) -> bool {
        (self.0 >> 8) & 1 != 0
    }

    /// Bit 3, established.
    pub const fn flags_sync(self) -> bool {
        (self.0 >> 3) & 1 != 0
    }

    /// Bit 2, established.
    pub const fn flags_duplicate(self) -> bool {
        (self.0 >> 2) & 1 != 0
    }

    /// Bit 1, established.
    pub const fn flags_bootable(self) -> bool {
        (self.0 >> 1) & 1 != 0
    }
}

/// The `PL_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PlW14(u32);

impl PlW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn secsize(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn xvmlabel(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `PL_W15` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PlW15(u32);

impl PlW15 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn xvmlabelblks(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn dbn(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `PL_W16` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PlW16(u32);

impl PlW16 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn nblks(self) -> u32 {
        self.0
    }
}

/// The `PL_W17` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PlW17(u32);

impl PlW17 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn timestamp(self) -> u32 {
        self.0
    }
}

/// The `PL_W30` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PlW30(u32);

impl PlW30 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn clusterid(self) -> u32 {
        self.0
    }
}

/// The `LH_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LhW0(u32);

impl LhW0 {
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

/// The `LH_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LhW1(u32);

impl LhW1 {
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
    pub const fn type_(self) -> LhW1Type {
        LhW1Type::from_bits((self.0 >> 8) & 0xff)
    }

    /// Bits 7:0, established.
    pub const fn size(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `VE_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeW0(u32);

impl VeW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn type_(self) -> VeW0Type {
        VeW0Type::from_bits(self.0 >> 24)
    }

    /// Bit 20, established.
    pub const fn state_disabled(self) -> bool {
        (self.0 >> 20) & 1 != 0
    }

    /// Bit 17, established.
    pub const fn state_valid(self) -> bool {
        (self.0 >> 17) & 1 != 0
    }

    /// Bit 16, established.
    pub const fn state_clean(self) -> bool {
        (self.0 >> 16) & 1 != 0
    }

    /// Bits 15:0, established.
    pub const fn pieces(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `VE_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeW1(u32);

impl VeW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn tid(self) -> u32 {
        self.0
    }
}

/// The `VE_W10` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeW10(u32);

impl VeW10 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn length_hi(self) -> u32 {
        self.0
    }
}

/// The `VE_W11` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeW11(u32);

impl VeW11 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn length_lo(self) -> u32 {
        self.0
    }
}

/// The `VE_W12` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeW12(u32);

impl VeW12 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn pos(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn cowfirst(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `VE_W13` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeW13(u32);

impl VeW13 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn cownext(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn cowregsz(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `VE_SLICE_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeSliceW14(u32);

impl VeSliceW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn startblk_hi(self) -> u32 {
        self.0
    }
}

/// The `VE_SLICE_W15` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeSliceW15(u32);

impl VeSliceW15 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn startblk_lo(self) -> u32 {
        self.0
    }
}

/// The `VE_SLICE_W16` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeSliceW16(u32);

impl VeSliceW16 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn seq(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:8, established.
    pub const fn flags(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, established.
    pub const fn partition(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `VE_STRIPE_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeStripeW14(u32);

impl VeStripeW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn susize(self) -> u32 {
        self.0
    }
}

/// The `VE_MIRROR_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeMirrorW14(u32);

impl VeMirrorW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn oldlength_hi(self) -> u32 {
        self.0
    }
}

/// The `VE_MIRROR_W15` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeMirrorW15(u32);

impl VeMirrorW15 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn oldlength_lo(self) -> u32 {
        self.0
    }
}

/// The `VE_MIRROR_W16` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeMirrorW16(u32);

impl VeMirrorW16 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn logfreq(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:8, established.
    pub const fn policy(self) -> VeMirrorW16Policy {
        VeMirrorW16Policy::from_bits((self.0 >> 8) & 0xff)
    }

    /// Bits 7:0, inferred.
    pub const fn config(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `VE_RAID_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeRaidW14(u32);

impl VeRaidW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, inferred.
    pub const fn susize(self) -> u32 {
        self.0
    }
}

/// The `VE_RAID_W15` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeRaidW15(u32);

impl VeRaidW15 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, inferred.
    pub const fn layout(self) -> u32 {
        self.0 >> 24
    }
}

/// The `VE_SUBVOL_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeSubvolW14(u32);

impl VeSubvolW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn svtype(self) -> VeSubvolW14Svtype {
        VeSubvolW14Svtype::from_bits(self.0 >> 24)
    }
}

/// The `VE_VOL_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct VeVolW14(u32);

impl VeVolW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn repfull(self) -> u32 {
        self.0 >> 24
    }
}

/// The `ATTR_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct AttrW0(u32);

impl AttrW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn type_(self) -> AttrW0Type {
        AttrW0Type::from_bits(self.0 >> 24)
    }

    /// Bit 16, inferred.
    pub const fn flag_persistent(self) -> bool {
        (self.0 >> 16) & 1 != 0
    }
}

/// The `ATTR_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct AttrW1(u32);

impl AttrW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn tid(self) -> u32 {
        self.0
    }
}

/// The `ATTR_PERM_W6` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct AttrPermW6(u32);

impl AttrPermW6 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn uid(self) -> u32 {
        self.0
    }
}

/// The `ATTR_PERM_W7` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct AttrPermW7(u32);

impl AttrPermW7 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn gid(self) -> u32 {
        self.0
    }
}

/// The `ATTR_PERM_W8` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct AttrPermW8(u32);

impl AttrPermW8 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn mode(self) -> u32 {
        self.0
    }
}
