// generated from kudzu c64e3113d062 HUBIO_DECODE.h - do not edit, regenerate in kudzu
//!
//! Origin 300 hub crosstalk-side window register tested by
//! the error path.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: physical base of the window for node 0.
pub const WINDOW_BASE: u32 = 0x1c00000;

/// Register `flags`: read, 64-bit; reset unknown; established.
pub const REG_FLAGS: u32 = 0x01e8;
pub const REG_FLAGS_WIDTH: u32 = 64;

/// The `flags` register word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Flags(u32);

impl Flags {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bit 0, established.
    pub const fn bit0(self) -> bool {
        self.0 & 1 != 0
    }

    /// Bit 4, established.
    pub const fn bit4(self) -> bool {
        (self.0 >> 4) & 1 != 0
    }
}
