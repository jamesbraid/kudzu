// generated from kudzu 460c8a2ecf06 KLCONFIG_DECODE.h - do not edit, regenerate in kudzu
//!
//! Hardware configuration board record: the fields the
//! kernel reads to find boards, slots, modules and components.
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
    pub const fn class(self) -> u32 {
        (self.0 >> 20) & 0xf
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
