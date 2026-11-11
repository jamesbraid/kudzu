// generated from kudzu fdb059d4cbe7 KLCONFIG_DECODE.h - do not edit, regenerate in kudzu
//!
//! Hardware configuration records: the board, component, console,
//! memory and global-data fields the kernel reads to find boards, slots, modules and components.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: first component offset, one 32-bit word each.
pub const BRD_OFF_COMPTS: u32 = 36;
/// established.
pub const BRD_STRIDE_COMPTS: u32 = 4;
/// established: directory offset from the node base.
pub const DIR_OFF: u32 = 0x2000;
/// established: byte of the doubleword that gives the configuration header offset.
pub const DIR_OFF_CONFIG_OFFSET: u32 = 72;
/// established: word giving the offset of the first board record.
pub const HDR_OFF_BOARD_INFO: u32 = 20;
/// established: bank sizes in megabytes, signed halfwords.
pub const MEM_OFF_BNKSZ: u32 = 60;
/// established.
pub const MEM_STRIDE_BNKSZ: u32 = 2;
/// established.
pub const MEM_COUNT_BNKSZ: u32 = 8;
/// established: signed halfword node number of each compact node; the first -1 ends the list.
pub const GDA_OFF_NASIDTABLE: u32 = 128;
/// established.
pub const GDA_STRIDE_NASIDTABLE: u32 = 2;
/// established: entries the Origin 200/2000 kernel reads at most.
pub const GDA_COUNT_NASIDTABLE_IP27: u32 = 64;
/// established: entries the Origin 300/350 kernel reads at most.
pub const GDA_COUNT_NASIDTABLE_IP35: u32 = 128;
/// established: Origin 300/350 only, pairs of signed halfwords; a first halfword of -1 ends the table.
pub const GDA_OFF_PEBRICK: u32 = 0x188;
/// established.
pub const GDA_STRIDE_PEBRICK: u32 = 4;
/// established: pairs read at most.
pub const GDA_COUNT_PEBRICK: u32 = 4;
/// established: the word at byte 388 equals this when the pair table follows.
pub const GDA_PEBRICK_MAGIC: u32 = 0x5045626b;
/// established: offset of the register in the node's local register space.
pub const NODECLASS_OFF: u32 = 0x4000000;
/// established: added to the offset for the local alias.
pub const NODECLASS_SPACE_LOCAL: u32 = 0x10000000;
/// established: added to the offset, with the node number, for the node-indexed alias.
pub const NODECLASS_SPACE_REMOTE: u32 = 0x30000000;

/// Values of `class` in `BrdW1` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum BrdW1Class {
    /// established.
    Node,
    /// inferred.
    Io,
    /// inferred.
    Router,
    /// inferred.
    Graphics,
    /// A value the notes do not name.
    Other(u8),
}

impl BrdW1Class {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x1 => Self::Node,
            0x2 => Self::Io,
            0x3 => Self::Router,
            0x5 => Self::Graphics,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Node => 0x1,
            Self::Io => 0x2,
            Self::Router => 0x3,
            Self::Graphics => 0x5,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `struct_type` in `CompW0` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum CompW0StructType {
    /// established.
    Hub,
    /// inferred.
    Memory,
    /// inferred.
    Io,
    /// inferred.
    Router,
    /// inferred.
    Graphics,
    /// A value the notes do not name.
    Other(u8),
}

impl CompW0StructType {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x2 => Self::Hub,
            0x3 => Self::Memory,
            0x5 => Self::Io,
            0x9 => Self::Router,
            0xa => Self::Graphics,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Hub => 0x2,
            Self::Memory => 0x3,
            Self::Io => 0x5,
            Self::Router => 0x9,
            Self::Graphics => 0xa,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `class` in `NodeclassW1` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum NodeclassW1Class {
    /// established.
    Pebrick,
    /// A value the notes do not name.
    Other(u8),
}

impl NodeclassW1Class {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0xc => Self::Pebrick,
            _ => Self::Other(v as u8),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Pebrick => 0xc,
            Self::Other(v) => v as u32,
        }
    }
}

/// The `BRD_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct BrdW0(u32);

impl BrdW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn next(self) -> u32 {
        self.0
    }
}

/// The `BRD_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct BrdW1(u32);

impl BrdW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 23:20, established.
    pub const fn class(self) -> BrdW1Class {
        BrdW1Class::from_bits((self.0 >> 20) & 0xf)
    }

    /// Bits 19:16, inferred.
    pub const fn type_low(self) -> u32 {
        (self.0 >> 16) & 0xf
    }
}

/// The `BRD_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct BrdW2(u32);

impl BrdW2 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bit 21, established.
    pub const fn flags_master_baseio(self) -> bool {
        (self.0 >> 21) & 1 != 0
    }

    /// Bits 15:8, established.
    pub const fn slot(self) -> u32 {
        (self.0 >> 8) & 0xff
    }
}

/// The `BRD_W3` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct BrdW3(u32);

impl BrdW3 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 15:0, established.
    pub const fn module(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `BRD_W5` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct BrdW5(u32);

impl BrdW5 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 7:0, established.
    pub const fn numcompts(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `BRD_W8` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct BrdW8(u32);

impl BrdW8 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn nasid(self) -> u32 {
        self.0 >> 16
    }
}

/// The `COMP_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct CompW0(u32);

impl CompW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn struct_type(self) -> CompW0StructType {
        CompW0StructType::from_bits(self.0 >> 24)
    }

    /// Bit 8, established.
    pub const fn flags_enabled(self) -> bool {
        (self.0 >> 8) & 1 != 0
    }
}

/// The `COMP_W6` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct CompW6(u32);

impl CompW6 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn physid(self) -> u32 {
        self.0 >> 24
    }
}

/// The `CONS_W2` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ConsW2(u32);

impl ConsW2 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn config_base_hi(self) -> u32 {
        self.0
    }
}

/// The `CONS_W3` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ConsW3(u32);

impl ConsW3 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn config_base_lo(self) -> u32 {
        self.0
    }
}

/// The `CONS_W4` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ConsW4(u32);

impl ConsW4 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn memory_base_hi(self) -> u32 {
        self.0
    }
}

/// The `CONS_W5` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ConsW5(u32);

impl ConsW5 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:24, established.
    pub const fn memory_base_widget(self) -> u32 {
        self.0 >> 24
    }

    /// Bits 23:0, established.
    pub const fn memory_base_rest(self) -> u32 {
        self.0 & 0xffffff
    }
}

/// The `CONS_W8` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ConsW8(u32);

impl ConsW8 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn nasid(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 7:0, established.
    pub const fn npci(self) -> u32 {
        self.0 & 0xff
    }
}

/// The `MEM_W14` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct MemW14(u32);

impl MemW14 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 15:0, established.
    pub const fn dimm_select(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `GDA_W97` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct GdaW97(u32);

impl GdaW97 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:0, established.
    pub const fn pebrick_marker(self) -> u32 {
        self.0
    }
}

/// The `PEPAIR_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PepairW0(u32);

impl PepairW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn pe_nasid(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn host_nasid(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `NODECLASS_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct NodeclassW1(u32);

impl NodeclassW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 15:12, established.
    pub const fn class(self) -> NodeclassW1Class {
        NodeclassW1Class::from_bits((self.0 >> 12) & 0xf)
    }
}
