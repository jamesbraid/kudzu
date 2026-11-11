// generated from kudzu d0569cd5ad8f HUBMDB_DECODE.h - do not edit, regenerate in kudzu
//!
//! Origin 300 hub memory-directory migration registers.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: physical base of the block for node 0, in units of 1 << HUBMDB_WINDOW_BASE_UNIT_SHIFT bytes; split because the full value is also a boot-region address the publish check refuses.
pub const WINDOW_BASE_UNITS: u32 = 0x1fc;
/// unknown: access and width; named only by a public vendor header, no shipped kernel accesses it.
pub const MIG_CANDIDATE_OFFSET: u32 = 48;
/// unknown: access and width; the public vendor header names it the read-to-clear candidate register, no shipped kernel accesses it.
pub const MIG_CANDIDATE_CLR_OFFSET: u32 = 56;
/// unknown: access and width; named only by a public vendor header, no shipped kernel accesses it. The Origin 200/2000 block reads its candidate register at this offset in a loop; the two layouts differ, so that does not carry over.
pub const MIG_VALUE_THRESH_OFFSET: u32 = 72;
/// established: set to arm, cleared to disarm.
pub const MIG_DIFF_THRESH_VALID_BIT: u32 = 63;

/// Register `mig_diff_thresh`: write, 64-bit; reset unknown; established.
pub const REG_MIG_DIFF_THRESH: u32 = 0x0040;
pub const REG_MIG_DIFF_THRESH_WIDTH: u32 = 64;

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

    /// Bits 14:0, inferred.
    pub const fn value(self) -> u32 {
        self.0 & 0x7fff
    }
}
