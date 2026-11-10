// generated from kudzu cc47e37a230e VOLHDR_DECODE.h - do not edit, regenerate in kudzu
//!
//! SGI disk volume header: magic, drive parameters, volume
//! directory and partition table.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: bytes in the header.
pub const SIZE: u32 = 0x200;
/// established: bytes 0b e5 a9 41.
pub const MAGIC: u32 = 0xbe5a941;
/// established.
pub const BOOTFILE_OFF: u32 = 8;
/// established: drive parameters, 48 bytes.
pub const DP_OFF: u32 = 24;
/// established.
pub const DP_BYTES: u32 = 48;
/// inferred: set together with the tag depth.
pub const DP_FLAG_TAGGED: u32 = 64;
/// established: start of the volume directory.
pub const DIR_OFF: u32 = 72;
/// established: directory entries.
pub const DIR_COUNT: u32 = 15;
/// established: bytes per directory entry.
pub const DIR_STRIDE: u32 = 16;
/// established: name bytes in a directory entry.
pub const DIR_NAME_LEN: u32 = 8;
/// established: start of the partition table.
pub const PART_OFF: u32 = 0x138;
/// established: partition entries.
pub const PART_COUNT: u32 = 16;
/// established: bytes per partition entry.
pub const PART_STRIDE: u32 = 12;
/// established: checksum word.
pub const CSUM_OFF: u32 = 0x1f8;
/// established: conventional root partition slot.
pub const SLOT_ROOT: u32 = 0;
/// established: conventional swap partition slot.
pub const SLOT_SWAP: u32 = 1;
/// established: conventional volume-header partition slot.
pub const SLOT_VOLHDR: u32 = 8;
/// established: conventional whole-disk partition slot.
pub const SLOT_WHOLEDISK: u32 = 10;
/// established.
pub const HDR_SIZE: u32 = 0x200;
/// established: boot file name, NUL padded, first byte 0 means none.
pub const HDR_OFF_BOOTFILE: u32 = 8;
/// established.
pub const HDR_LEN_BOOTFILE: u32 = 16;
/// established.
pub const DIRENT_SIZE: u32 = 16;
/// established: file name, NUL padded, not terminated at 8 characters.
pub const DIRENT_OFF_NAME: u32 = 0;
/// established.
pub const DIRENT_LEN_NAME: u32 = 8;
/// established.
pub const PART_SIZE: u32 = 12;

/// Values of `type` in `PartW2` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum PartW2Type {
    /// established.
    Volhdr,
    /// inferred.
    Trkrepl,
    /// inferred.
    Secrepl,
    /// established.
    Raw,
    /// inferred.
    Bsd,
    /// inferred.
    Sysv,
    /// established.
    Volume,
    /// established.
    Efs,
    /// established.
    Lvol,
    /// established.
    Rlvol,
    /// established.
    Xfs,
    /// established.
    Xfslog,
    /// established.
    Xlv,
    /// established.
    Xvm,
    /// established.
    Vxvm,
    /// A value the notes do not name.
    Other(u32),
}

impl PartW2Type {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::Volhdr,
            0x1 => Self::Trkrepl,
            0x2 => Self::Secrepl,
            0x3 => Self::Raw,
            0x4 => Self::Bsd,
            0x5 => Self::Sysv,
            0x6 => Self::Volume,
            0x7 => Self::Efs,
            0x8 => Self::Lvol,
            0x9 => Self::Rlvol,
            0xa => Self::Xfs,
            0xb => Self::Xfslog,
            0xc => Self::Xlv,
            0xd => Self::Xvm,
            0xe => Self::Vxvm,
            _ => Self::Other(v as u32),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Volhdr => 0x0,
            Self::Trkrepl => 0x1,
            Self::Secrepl => 0x2,
            Self::Raw => 0x3,
            Self::Bsd => 0x4,
            Self::Sysv => 0x5,
            Self::Volume => 0x6,
            Self::Efs => 0x7,
            Self::Lvol => 0x8,
            Self::Rlvol => 0x9,
            Self::Xfs => 0xa,
            Self::Xfslog => 0xb,
            Self::Xlv => 0xc,
            Self::Xvm => 0xd,
            Self::Vxvm => 0xe,
            Self::Other(v) => v as u32,
        }
    }
}

/// The `HDR_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW0(u32);

impl HdrW0 {
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

/// The `HDR_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW1(u32);

impl HdrW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn root(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn swap(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `HDR_W7` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW7(u32);

impl HdrW7 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, inferred.
    pub const fn dp_cylinders(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, unknown.
    pub const fn dp_head_offset(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `HDR_W8` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW8(u32);

impl HdrW8 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, inferred.
    pub const fn dp_heads(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:8, established.
    pub const fn dp_tag_depth(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, unknown.
    pub const fn dp_cyl_high(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `HDR_W9` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW9(u32);

impl HdrW9 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 15:0, inferred.
    pub const fn dp_sectors(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `HDR_W10` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW10(u32);

impl HdrW10 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn dp_secbytes(self) -> u32 {
        self.0 >> 16
    }
}

/// The `HDR_W11` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW11(u32);

impl HdrW11 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, inferred.
    pub const fn dp_flags(self) -> u32 {
        self.0
    }
}

/// The `HDR_W17` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW17(u32);

impl HdrW17 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn dp_capacity(self) -> u32 {
        self.0
    }
}

/// The `HDR_W126` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct HdrW126(u32);

impl HdrW126 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn checksum(self) -> u32 {
        self.0
    }
}

/// The `DIRENT_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DirentW2(u32);

impl DirentW2 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn start_block(self) -> u32 {
        self.0
    }
}

/// The `DIRENT_W3` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DirentW3(u32);

impl DirentW3 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn length_bytes(self) -> u32 {
        self.0
    }
}

/// The `PART_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PartW0(u32);

impl PartW0 {
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

/// The `PART_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PartW1(u32);

impl PartW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn first(self) -> u32 {
        self.0
    }
}

/// The `PART_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PartW2(u32);

impl PartW2 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn type_(self) -> PartW2Type {
        PartW2Type::from_bits(self.0)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn testvec_part_w0_1000() {
        let w = PartW0::new(0x1000);
        assert_eq!(w.nblks(), 4096, "nblks");
    }

    #[test]
    fn testvec_part_w0_40000() {
        let w = PartW0::new(0x40000);
        assert_eq!(w.nblks(), 262144, "nblks");
    }

    #[test]
    fn testvec_part_w1_41000() {
        let w = PartW1::new(0x41000);
        assert_eq!(w.first(), 266240, "first");
    }

    #[test]
    fn testvec_dirent_w3_41400() {
        let w = DirentW3::new(0x41400);
        assert_eq!(w.length_bytes(), 267264, "length_bytes");
    }
}
