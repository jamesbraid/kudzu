// generated from kudzu c64e3113d062 R10K_TIMER_DECODE.h - do not edit, regenerate in kudzu
//!
//! R10000 Count and Compare timer interrupt, from the published
//! manual.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: Count register number in coprocessor 0.
pub const CP0_COUNT: u32 = 9;
/// established: Compare register number in coprocessor 0.
pub const CP0_COMPARE: u32 = 11;

/// The `CAUSE` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Cause(u32);

impl Cause {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bit 15, established.
    pub const fn ip7(self) -> bool {
        (self.0 >> 15) & 1 != 0
    }
}
