// generated from kudzu cc47e37a230e XFS_DECODE.h - do not edit, regenerate in kudzu
//!
//! XFS superblock and inode core as the shipped release
//! writes them.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: bytes 58 46 53 42.
pub const SB_MAGIC: u32 = 0x58465342;
/// established: bytes the kernel copies into its in-core superblock.
pub const SB_READ_BYTES: u32 = 208;
/// established: the format debugger names fields up to this end.
pub const SB_DESCRIBED_BYTES: u32 = 204;
/// established.
pub const INO_MAGIC: u32 = 0x494e;
/// established: start of the data fork.
pub const INO_FORK_OFFSET: u32 = 100;
/// established: forkoff counts units of this many bytes.
pub const INO_ATTR_FORK_UNIT: u32 = 8;
/// established: the 16-bit flags word.
pub const INO_FLAGS_OFFSET: u32 = 90;
/// established.
pub const SB_SIZE: u32 = 208;
/// established: opaque bytes.
pub const SB_OFF_UUID: u32 = 32;
/// established.
pub const SB_LEN_UUID: u32 = 16;
/// established: volume name, first half.
pub const SB_OFF_FNAME: u32 = 108;
/// established.
pub const SB_LEN_FNAME: u32 = 6;
/// established: volume name, second half; fname and fpack form one 12-byte name.
pub const SB_OFF_FPACK: u32 = 114;
/// established.
pub const SB_LEN_FPACK: u32 = 6;
/// established.
pub const INO_SIZE: u32 = 100;

/// Values of `format` in `InoW1` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum InoW1Format {
    /// established.
    Dev,
    /// established.
    Local,
    /// established.
    Extents,
    /// established.
    Btree,
    /// established.
    Uuid,
    /// A value the notes do not name.
    Other(u8),
}

impl InoW1Format {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::Dev,
            0x1 => Self::Local,
            0x2 => Self::Extents,
            0x3 => Self::Btree,
            0x4 => Self::Uuid,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Dev => 0x0,
            Self::Local => 0x1,
            Self::Extents => 0x2,
            Self::Btree => 0x3,
            Self::Uuid => 0x4,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `aformat` in `InoW20` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum InoW20Aformat {
    /// established.
    Local,
    /// established.
    Extents,
    /// established.
    Btree,
    /// A value the notes do not name.
    Other(u8),
}

impl InoW20Aformat {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x1 => Self::Local,
            0x2 => Self::Extents,
            0x3 => Self::Btree,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Local => 0x1,
            Self::Extents => 0x2,
            Self::Btree => 0x3,
            Self::Other(v) => v as u32,
        }
    }
}

/// The `SB_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW0(u32);

impl SbW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn magicnum(self) -> u32 {
        self.0
    }
}

/// The `SB_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW1(u32);

impl SbW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn blocksize(self) -> u32 {
        self.0
    }
}

/// The `SB_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW2(u32);

impl SbW2 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn dblocks_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W3` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW3(u32);

impl SbW3 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn dblocks_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W4` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW4(u32);

impl SbW4 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rblocks_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W5` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW5(u32);

impl SbW5 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rblocks_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W6` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW6(u32);

impl SbW6 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rextents_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W7` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW7(u32);

impl SbW7 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rextents_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W12` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW12(u32);

impl SbW12 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn logstart_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W13` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW13(u32);

impl SbW13 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn logstart_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW14(u32);

impl SbW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rootino_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W15` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW15(u32);

impl SbW15 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rootino_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W16` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW16(u32);

impl SbW16 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rbmino_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W17` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW17(u32);

impl SbW17 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rbmino_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W18` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW18(u32);

impl SbW18 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rsumino_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W19` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW19(u32);

impl SbW19 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rsumino_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W20` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW20(u32);

impl SbW20 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rextsize(self) -> u32 {
        self.0
    }
}

/// The `SB_W21` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW21(u32);

impl SbW21 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn agblocks(self) -> u32 {
        self.0
    }
}

/// The `SB_W22` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW22(u32);

impl SbW22 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn agcount(self) -> u32 {
        self.0
    }
}

/// The `SB_W23` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW23(u32);

impl SbW23 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn rbmblocks(self) -> u32 {
        self.0
    }
}

/// The `SB_W24` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW24(u32);

impl SbW24 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn logblocks(self) -> u32 {
        self.0
    }
}

/// The `SB_W25` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW25(u32);

impl SbW25 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bit 31, established.
    pub const fn versionnum_morebits(self) -> bool {
        (self.0 >> 31) & 1 != 0
    }

    /// Bit 30, inferred.
    pub const fn versionnum_ci_dirs(self) -> bool {
        (self.0 >> 30) & 1 != 0
    }

    /// Bit 29, established.
    pub const fn versionnum_dirv2(self) -> bool {
        (self.0 >> 29) & 1 != 0
    }

    /// Bit 28, established.
    pub const fn versionnum_extflg(self) -> bool {
        (self.0 >> 28) & 1 != 0
    }

    /// Bit 27, established.
    pub const fn versionnum_rejected(self) -> bool {
        (self.0 >> 27) & 1 != 0
    }

    /// Bit 26, established.
    pub const fn versionnum_logv2(self) -> bool {
        (self.0 >> 26) & 1 != 0
    }

    /// Bit 25, established.
    pub const fn versionnum_shared(self) -> bool {
        (self.0 >> 25) & 1 != 0
    }

    /// Bit 24, established.
    pub const fn versionnum_dalign(self) -> bool {
        (self.0 >> 24) & 1 != 0
    }

    /// Bit 23, established.
    pub const fn versionnum_align(self) -> bool {
        (self.0 >> 23) & 1 != 0
    }

    /// Bit 22, established.
    pub const fn versionnum_quota(self) -> bool {
        (self.0 >> 22) & 1 != 0
    }

    /// Bit 21, established.
    pub const fn versionnum_nlink(self) -> bool {
        (self.0 >> 21) & 1 != 0
    }

    /// Bit 20, established.
    pub const fn versionnum_attr(self) -> bool {
        (self.0 >> 20) & 1 != 0
    }

    /// Bits 19:16, established.
    pub const fn versionnum_vers(self) -> u32 {
        (self.0 >> 16) & 0xf
    }

    /// Bits 15:0, established.
    pub const fn sectsize(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `SB_W26` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW26(u32);

impl SbW26 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn inodesize(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn inopblock(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `SB_W30` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW30(u32);

impl SbW30 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn blocklog(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:16, established.
    pub const fn sectlog(self) -> u32 {
        (self.0 >> 16) & 0xff
    }

    /// Bits 15:8, established.
    pub const fn inodelog(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, established.
    pub const fn inopblog(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `SB_W31` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW31(u32);

impl SbW31 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn agblklog(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:16, established.
    pub const fn rextslog(self) -> u32 {
        (self.0 >> 16) & 0xff
    }

    /// Bits 15:8, established.
    pub const fn inprogress(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, established.
    pub const fn imax_pct(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `SB_W32` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW32(u32);

impl SbW32 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn icount_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W33` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW33(u32);

impl SbW33 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn icount_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W34` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW34(u32);

impl SbW34 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ifree_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W35` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW35(u32);

impl SbW35 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ifree_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W36` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW36(u32);

impl SbW36 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn fdblocks_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W37` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW37(u32);

impl SbW37 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn fdblocks_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W38` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW38(u32);

impl SbW38 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn frextents_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W39` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW39(u32);

impl SbW39 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn frextents_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W40` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW40(u32);

impl SbW40 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn uquotino_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W41` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW41(u32);

impl SbW41 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn uquotino_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W42` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW42(u32);

impl SbW42 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn pquotino_hi(self) -> u32 {
        self.0
    }
}

/// The `SB_W43` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW43(u32);

impl SbW43 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn pquotino_lo(self) -> u32 {
        self.0
    }
}

/// The `SB_W44` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW44(u32);

impl SbW44 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn qflags(self) -> u32 {
        self.0 >> 16
    }

    /// Bit 8, established.
    pub const fn flags_shared_ro(self) -> bool {
        (self.0 >> 8) & 1 != 0
    }

    /// Bits 7:0, established.
    pub const fn shared_vn(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `SB_W45` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW45(u32);

impl SbW45 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn inoalignmt(self) -> u32 {
        self.0
    }
}

/// The `SB_W46` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW46(u32);

impl SbW46 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn unit(self) -> u32 {
        self.0
    }
}

/// The `SB_W47` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW47(u32);

impl SbW47 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn width(self) -> u32 {
        self.0
    }
}

/// The `SB_W48` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW48(u32);

impl SbW48 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn dirblklog(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:16, established.
    pub const fn logsectlog(self) -> u32 {
        (self.0 >> 16) & 0xff
    }

    /// Bits 15:0, established.
    pub const fn logsectsize(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `SB_W49` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW49(u32);

impl SbW49 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn logsunit(self) -> u32 {
        self.0
    }
}

/// The `SB_W50` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW50(u32);

impl SbW50 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:5, unknown.
    pub const fn features2_high(self) -> u32 {
        self.0 >> 5
    }

    /// Bit 4, established.
    pub const fn features2_parent(self) -> bool {
        (self.0 >> 4) & 1 != 0
    }

    /// Bit 3, established.
    pub const fn features2_attr2(self) -> bool {
        (self.0 >> 3) & 1 != 0
    }

    /// Bit 2, unknown.
    pub const fn features2_bit2(self) -> bool {
        (self.0 >> 2) & 1 != 0
    }

    /// Bit 1, established.
    pub const fn features2_lazysbcount(self) -> bool {
        (self.0 >> 1) & 1 != 0
    }

    /// Bit 0, unknown.
    pub const fn features2_bit0(self) -> bool {
        self.0 & 1 != 0
    }
}

/// The `INO_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW0(u32);

impl InoW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn magic(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn mode(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `INO_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW1(u32);

impl InoW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn version(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:16, established.
    pub const fn format(self) -> InoW1Format {
        InoW1Format::from_bits((self.0 >> 16) & 0xff)
    }

    /// Bits 15:0, established.
    pub const fn onlink(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `INO_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW2(u32);

impl InoW2 {
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

/// The `INO_W3` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW3(u32);

impl InoW3 {
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

/// The `INO_W4` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW4(u32);

impl InoW4 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn nlinkv2(self) -> u32 {
        self.0
    }
}

/// The `INO_W5` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW5(u32);

impl InoW5 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn projid(self) -> u32 {
        self.0 >> 16
    }
}

/// The `INO_W7` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW7(u32);

impl InoW7 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 15:0, established.
    pub const fn flushiter(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `INO_W8` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW8(u32);

impl InoW8 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn atime_sec(self) -> u32 {
        self.0
    }
}

/// The `INO_W9` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW9(u32);

impl InoW9 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn atime_nsec(self) -> u32 {
        self.0
    }
}

/// The `INO_W10` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW10(u32);

impl InoW10 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn mtime_sec(self) -> u32 {
        self.0
    }
}

/// The `INO_W11` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW11(u32);

impl InoW11 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn mtime_nsec(self) -> u32 {
        self.0
    }
}

/// The `INO_W12` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW12(u32);

impl InoW12 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ctime_sec(self) -> u32 {
        self.0
    }
}

/// The `INO_W13` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW13(u32);

impl InoW13 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ctime_nsec(self) -> u32 {
        self.0
    }
}

/// The `INO_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW14(u32);

impl InoW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn size_hi(self) -> u32 {
        self.0
    }
}

/// The `INO_W15` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW15(u32);

impl InoW15 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn size_lo(self) -> u32 {
        self.0
    }
}

/// The `INO_W16` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW16(u32);

impl InoW16 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn nblocks_hi(self) -> u32 {
        self.0
    }
}

/// The `INO_W17` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW17(u32);

impl InoW17 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn nblocks_lo(self) -> u32 {
        self.0
    }
}

/// The `INO_W18` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW18(u32);

impl InoW18 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn extsize(self) -> u32 {
        self.0
    }
}

/// The `INO_W19` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW19(u32);

impl InoW19 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn nextents(self) -> u32 {
        self.0
    }
}

/// The `INO_W20` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW20(u32);

impl InoW20 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn naextents(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:8, established.
    pub const fn forkoff(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, established.
    pub const fn aformat(self) -> InoW20Aformat {
        InoW20Aformat::from_bits(self.0 & 0xff)
    }
}

/// The `INO_W21` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW21(u32);

impl InoW21 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn dmevmask(self) -> u32 {
        self.0
    }
}

/// The `INO_W22` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW22(u32);

impl InoW22 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn dmstate(self) -> u32 {
        self.0 >> 16
    }

    /// Bit 8, established.
    pub const fn flags_rtinherit(self) -> bool {
        (self.0 >> 8) & 1 != 0
    }

    /// Bit 7, established.
    pub const fn flags_nodump(self) -> bool {
        (self.0 >> 7) & 1 != 0
    }

    /// Bit 6, established.
    pub const fn flags_noatime(self) -> bool {
        (self.0 >> 6) & 1 != 0
    }

    /// Bit 5, established.
    pub const fn flags_sync(self) -> bool {
        (self.0 >> 5) & 1 != 0
    }

    /// Bit 4, established.
    pub const fn flags_append(self) -> bool {
        (self.0 >> 4) & 1 != 0
    }

    /// Bit 3, established.
    pub const fn flags_immutable(self) -> bool {
        (self.0 >> 3) & 1 != 0
    }

    /// Bit 2, established.
    pub const fn flags_newrtbm(self) -> bool {
        (self.0 >> 2) & 1 != 0
    }

    /// Bit 1, established.
    pub const fn flags_prealloc(self) -> bool {
        (self.0 >> 1) & 1 != 0
    }

    /// Bit 0, established.
    pub const fn flags_realtime(self) -> bool {
        self.0 & 1 != 0
    }
}

/// The `INO_W23` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW23(u32);

impl InoW23 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn gen(self) -> u32 {
        self.0
    }
}

/// The `INO_W24` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InoW24(u32);

impl InoW24 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn next_unlinked(self) -> u32 {
        self.0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn testvec_sb_w50_1b() {
        let w = SbW50::new(0x1b);
        assert_eq!(w.features2_bit0() as u32, 1, "features2_bit0");
        assert_eq!(w.features2_lazysbcount() as u32, 1, "features2_lazysbcount");
        assert_eq!(w.features2_attr2() as u32, 1, "features2_attr2");
        assert_eq!(w.features2_parent() as u32, 1, "features2_parent");
    }
}
