// generated from kudzu c64e3113d062 PCIBR_DECODE.h - do not edit, regenerate in kudzu
//!
//! PCI bridge widget identification word, configuration address
//! layout and interrupt address registers.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: byte offset of the widget identification word in the window.
pub const WID_OFFSET: u32 = 4;
/// established.
pub const WINDOW_BIT_A: u32 = 63;
/// established.
pub const WINDOW_BIT_B: u32 = 60;
/// established.
pub const WINDOW_BIT_C: u32 = 57;
/// established: added on the 32-bit layout only, with the register offset first rounded down to a multiple of four.
pub const CFG_WINDOW32_OFFSET: u32 = 0x20000;
/// established: slots 0 to 7 on the 32-bit layout.
pub const CFG_SLOTS_32: u32 = 8;
/// established: on the 64-bit layouts the slot is incremented before use; the accepted range is then 1 to 4.
pub const CFG_SLOT_BIAS_64: u32 = 1;
/// established: interrupt numbers 0 to 7.
pub const INTR_COUNT: u32 = 8;
/// established: byte stride of the interrupt address registers on every layout.
pub const INTR_STRIDE: u32 = 8;
/// established: first interrupt address register, 64-bit layouts; its low 48 bits hold the address.
pub const INTR_ADDR64_OFFSET: u32 = 0x130;
/// established: first interrupt address register, 32-bit layout; a 32-bit word.
pub const INTR_ADDR32_OFFSET: u32 = 0x134;
/// established: the 32-bit layout replaces bits 17:8 of the word.
pub const INTR_ADDR32_HI: u32 = 17;
/// established.
pub const INTR_ADDR32_LO: u32 = 8;
/// established: the 32-bit layout keeps source address bits 47:38.
pub const INTR_ADDR32_SRC_HI: u32 = 47;
/// established.
pub const INTR_ADDR32_SRC_LO: u32 = 38;

/// Values of `part` in `Wid` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum WidPart {
    /// established.
    Layout32,
    /// established.
    Layout32b,
    /// established.
    Layout64b,
    /// established.
    Layout64bx,
    /// established.
    Layout64a,
    /// established.
    Layout64ax,
    /// A value the notes do not name.
    Other(u32),
}

impl WidPart {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0xc002 => Self::Layout32,
            0xd002 => Self::Layout32b,
            0xd102 => Self::Layout64b,
            0xd112 => Self::Layout64bx,
            0xe000 => Self::Layout64a,
            0xe010 => Self::Layout64ax,
            _ => Self::Other(v as u32),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Layout32 => 0xc002,
            Self::Layout32b => 0xd002,
            Self::Layout64b => 0xd102,
            Self::Layout64bx => 0xd112,
            Self::Layout64a => 0xe000,
            Self::Layout64ax => 0xe010,
            Self::Other(v) => v as u32,
        }
    }
}

/// Values of `field11` in `Wid` that the header names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum WidField11 {
    /// established.
    Value36,
    /// established.
    Value54,
    /// A value the notes do not name.
    Other(u32),
}

impl WidField11 {
    pub const fn from_bits(v: u32) -> Self {
        match v {
            0x24 => Self::Value36,
            0x36 => Self::Value54,
            _ => Self::Other(v as u32),
        }
    }

    pub const fn bits(self) -> u32 {
        match self {
            Self::Value36 => 0x24,
            Self::Value54 => 0x36,
            Self::Other(v) => v as u32,
        }
    }
}

/// The `WID` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Wid(u32);

impl Wid {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 27:12, established.
    pub const fn part(self) -> WidPart {
        WidPart::from_bits((self.0 >> 12) & 0xffff)
    }

    /// Bits 11:1, established.
    pub const fn field11(self) -> WidField11 {
        WidField11::from_bits((self.0 >> 1) & 0x7ff)
    }
}
