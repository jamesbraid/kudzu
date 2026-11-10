// generated from kudzu 5468a0cc81b8 EFS_DECODE.h - do not edit, regenerate in kudzu
//!
//! Extent File System on-disk structures: superblock, inode,
//! extent and directory block.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: block size in bytes.
pub const BBSIZE: u32 = 0x200;
/// established: block 0 is an unused boot block.
pub const SUPER_BB: u32 = 1;
/// established: bitmap start when the superblock bitmap block is 0.
pub const BITMAP_BB: u32 = 2;
/// established: carried by every held image.
pub const MAGIC_OLD: u32 = 0x72959;
/// established: bitmap and size placed past the last group.
pub const MAGIC_NEW: u32 = 0x7295a;
/// established: inodes 0 and 1 are never allocated.
pub const ROOT_INO: u32 = 2;
/// established.
pub const INODE_SIZE: u32 = 128;
/// established.
pub const INODES_PER_BB: u32 = 4;
/// established: extents that fit in the inode data area.
pub const DIRECTEXTENTS: u32 = 12;
/// established: longest run in blocks.
pub const EX_MAXLEN: u32 = 248;
/// established: inode data area, from byte 0x20 to 0x7f.
pub const INODE_DATA_BYTES: u32 = 96;
/// established: short device value that selects the long form.
pub const DEV_LONG_FORM: u32 = 0xffff;
/// established.
pub const DIRBLK_MAGIC: u32 = 0xbeef;
/// established.
pub const DIRBLK_HEADERSIZE: u32 = 4;
/// established.
pub const DIRBLK_MAXSLOTS: u32 = 72;
/// established: inode number plus name length byte.
pub const DENT_HEADER_BYTES: u32 = 5;
/// established.
pub const SB_SIZE: u32 = 92;
/// established: filesystem name, not NUL terminated.
pub const SB_OFF_FNAME: u32 = 32;
/// established.
pub const SB_LEN_FNAME: u32 = 6;
/// established: pack name, not NUL terminated.
pub const SB_OFF_FPACK: u32 = 38;
/// established.
pub const SB_LEN_FPACK: u32 = 6;
/// inferred: zero in every image.
pub const SB_OFF_SPARE: u32 = 68;
/// inferred.
pub const SB_LEN_SPARE: u32 = 20;
/// established.
pub const EXT_SIZE: u32 = 8;
/// established.
pub const DI_SIZE: u32 = 128;
/// established.
pub const DB_SIZE: u32 = 0x200;

/// Values of `dirty` in `SbW5` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum SbW5Dirty {
    /// established.
    Clean,
    /// established.
    Mounted,
    /// established.
    Dirty,
    /// established.
    Needsck,
    /// A value the notes do not name.
    Other(u32),
}

impl SbW5Dirty {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x0 => Self::Clean,
            0x7777 => Self::Mounted,
            0xbad => Self::Dirty,
            0x1234 => Self::Needsck,
            _ => Self::Other(v as u32),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Clean => 0x0,
            Self::Mounted => 0x7777,
            Self::Dirty => 0xbad,
            Self::Needsck => 0x1234,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `magic` in `SbW7` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum SbW7Magic {
    /// established.
    Old,
    /// established.
    New,
    /// A value the notes do not name.
    Other(u32),
}

impl SbW7Magic {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x72959 => Self::Old,
            0x7295a => Self::New,
            _ => Self::Other(v as u32),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Old => 0x72959,
            Self::New => 0x7295a,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `mode_type` in `DiW0` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum DiW0ModeType {
    /// established.
    Fifo,
    /// established.
    Chr,
    /// established.
    X3,
    /// established.
    Dir,
    /// established.
    Blk,
    /// established.
    X7,
    /// established.
    Reg,
    /// established.
    Lnk,
    /// established.
    Sock,
    /// A value the notes do not name.
    Other(u8),
}

impl DiW0ModeType {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x1 => Self::Fifo,
            0x2 => Self::Chr,
            0x3 => Self::X3,
            0x4 => Self::Dir,
            0x6 => Self::Blk,
            0x7 => Self::X7,
            0x8 => Self::Reg,
            0xa => Self::Lnk,
            0xc => Self::Sock,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Fifo => 0x1,
            Self::Chr => 0x2,
            Self::X3 => 0x3,
            Self::Dir => 0x4,
            Self::Blk => 0x6,
            Self::X7 => 0x7,
            Self::Reg => 0x8,
            Self::Lnk => 0xa,
            Self::Sock => 0xc,
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
    pub const fn size(self) -> u32 {
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
    pub const fn firstcg(self) -> u32 {
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
    pub const fn cgfsize(self) -> u32 {
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

    /// Bits 31:16, established.
    pub const fn cgisize(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn sectors(self) -> u32 {
        self.0 & 0xffff
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

    /// Bits 31:16, established.
    pub const fn heads(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn ncg(self) -> u32 {
        self.0 & 0xffff
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

    /// Bits 31:16, established.
    pub const fn dirty(self) -> SbW5Dirty {
        SbW5Dirty::from_bits(self.0 >> 16)
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
    pub const fn time(self) -> u32 {
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
    pub const fn magic(self) -> SbW7Magic {
        SbW7Magic::from_bits(self.0)
    }
}

/// The `SB_W11` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SbW11(u32);

impl SbW11 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn bmsize(self) -> u32 {
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
    pub const fn tfree(self) -> u32 {
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
    pub const fn tinode(self) -> u32 {
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
    pub const fn bmblock(self) -> u32 {
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
    pub const fn replsb(self) -> u32 {
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

    /// Bits 31:0, inferred.
    pub const fn lastialloc(self) -> u32 {
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
    pub const fn checksum(self) -> u32 {
        self.0
    }
}

/// The `EXT_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ExtW0(u32);

impl ExtW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn magic(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:0, established.
    pub const fn bn(self) -> u32 {
        self.0 & 0xffffff
    }
}

/// The `EXT_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ExtW1(u32);

impl ExtW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn length(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:0, established.
    pub const fn offset(self) -> u32 {
        self.0 & 0xffffff
    }
}

/// The `DI_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW0(u32);

impl DiW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:28, established.
    pub const fn mode_type(self) -> DiW0ModeType {
        DiW0ModeType::from_bits(self.0 >> 28)
    }

    /// Bits 27:16, established.
    pub const fn mode_perm(self) -> u32 {
        (self.0 >> 16) & 0xfff
    }

    /// Bits 15:0, established.
    pub const fn nlink(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `DI_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW1(u32);

impl DiW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn uid(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn gid(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `DI_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW2(u32);

impl DiW2 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn size(self) -> u32 {
        self.0
    }
}

/// The `DI_W3` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW3(u32);

impl DiW3 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn atime(self) -> u32 {
        self.0
    }
}

/// The `DI_W4` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW4(u32);

impl DiW4 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn mtime(self) -> u32 {
        self.0
    }
}

/// The `DI_W5` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW5(u32);

impl DiW5 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ctime(self) -> u32 {
        self.0
    }
}

/// The `DI_W6` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW6(u32);

impl DiW6 {
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

/// The `DI_W7` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DiW7(u32);

impl DiW7 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn numextents(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:8, established.
    pub const fn version(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, inferred.
    pub const fn spare(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `DEV_W8` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DevW8(u32);

impl DevW8 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn odev(self) -> u32 {
        self.0 >> 16
    }
}

/// The `DEV_W9` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DevW9(u32);

impl DevW9 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn ndev(self) -> u32 {
        self.0
    }
}

/// The `DB_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct DbW0(u32);

impl DbW0 {
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

    /// Bits 15:8, established.
    pub const fn firstused(self) -> u32 {
        (self.0 >> 8) & 0xff
    }

    /// Bits 7:0, established.
    pub const fn slots(self) -> u32 {
        self.0 & 0xff
    }
}
