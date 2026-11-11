// generated from kudzu d0569cd5ad8f HUBMDA_DECODE.h - do not edit, regenerate in kudzu
//!
//! Origin 200/2000 hub memory-directory migration registers.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: physical base of the block for node 0.
pub const WINDOW_BASE: u32 = 0x1a00000;
/// established: set to arm, cleared to disarm.
pub const MIG_DIFF_THRESH_VALID_BIT: u32 = 63;
/// established: set to arm, cleared to disarm.
pub const MIG_VALUE_THRESH_VALID_BIT: u32 = 63;
/// established: the loop ends when it is clear.
pub const MIG_CANDIDATE_VALID_BIT: u32 = 63;
/// established: eleven bits, standard memory.
pub const REFCNT_COUNT_MASK_STANDARD: u32 = 0x7ff;
/// established: nineteen bits, premium memory; which memory is premium is inferred.
pub const REFCNT_COUNT_MASK_PREMIUM: u32 = 0x7ffff;
/// established: bytes between the words of consecutive nodes for one page.
pub const REFCNT_NODE_STRIDE: u32 = 8;

/// Register `mig_diff_thresh`: read/write, 64-bit; reset unknown; established.
pub const REG_MIG_DIFF_THRESH: u32 = 0x0030;
pub const REG_MIG_DIFF_THRESH_WIDTH: u32 = 64;
/// Register `mig_value_thresh`: read/write, 64-bit; reset unknown; inferred.
pub const REG_MIG_VALUE_THRESH: u32 = 0x0038;
pub const REG_MIG_VALUE_THRESH_WIDTH: u32 = 64;
/// Register `mig_candidate`: read, 64-bit; reset unknown; established.
pub const REG_MIG_CANDIDATE: u32 = 0x0048;
pub const REG_MIG_CANDIDATE_WIDTH: u32 = 64;

/// The `mig_diff_thresh` register word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct MigDiffThresh(u32);

impl MigDiffThresh {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 19:0, inferred.
    pub const fn value(self) -> u32 {
        self.0 & 0xfffff
    }
}

/// The `mig_candidate` register word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct MigCandidate(u32);

impl MigCandidate {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 17:0, inferred.
    pub const fn page(self) -> u32 {
        self.0 & 0x3ffff
    }

    /// Bits 28:20, inferred.
    pub const fn node(self) -> u32 {
        (self.0 >> 20) & 0x1ff
    }

    /// Bit 30, inferred.
    pub const fn type_(self) -> bool {
        (self.0 >> 30) & 1 != 0
    }
}
