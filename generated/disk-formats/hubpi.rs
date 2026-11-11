// generated from kudzu c64e3113d062 HUBPI_DECODE.h - do not edit, regenerate in kudzu
//!
//! Node hub interrupt pending and mask registers.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: physical base of the processor's own hub window; no node shift.
pub const LOCAL_WINDOW_BASE: u32 = 0x1000000;
/// established: physical base of a named node's window.
pub const NODE_WINDOW_BASE: u32 = 0x1800000;
/// established.
pub const INT_PEND_MOD_ALT_OFFSET: u32 = 0x200090;

/// Register `int_pend_mod`: write, 64-bit; reset unknown; inferred.
pub const REG_INT_PEND_MOD: u32 = 0x0090;
pub const REG_INT_PEND_MOD_WIDTH: u32 = 64;
/// Register `int_pend0`: read, 64-bit; reset unknown; established.
pub const REG_INT_PEND0: u32 = 0x0098;
pub const REG_INT_PEND0_WIDTH: u32 = 64;
/// Register `int_pend1`: read, 64-bit; reset unknown; established.
pub const REG_INT_PEND1: u32 = 0x00a0;
pub const REG_INT_PEND1_WIDTH: u32 = 64;
/// Register `int_mask0_a`: read/write, 64-bit; reset unknown; established.
pub const REG_INT_MASK0_A: u32 = 0x00a8;
pub const REG_INT_MASK0_A_WIDTH: u32 = 64;
/// Register `int_mask1_a`: read/write, 64-bit; reset unknown; established.
pub const REG_INT_MASK1_A: u32 = 0x00b0;
pub const REG_INT_MASK1_A_WIDTH: u32 = 64;
/// Register `int_mask0_b`: read/write, 64-bit; reset unknown; established.
pub const REG_INT_MASK0_B: u32 = 0x00b8;
pub const REG_INT_MASK0_B_WIDTH: u32 = 64;
/// Register `int_mask1_b`: read/write, 64-bit; reset unknown; established.
pub const REG_INT_MASK1_B: u32 = 0x00c0;
pub const REG_INT_MASK1_B_WIDTH: u32 = 64;

/// The `int_pend_mod` register word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct IntPendMod(u32);

impl IntPendMod {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 6:0, established.
    pub const fn level(self) -> u32 {
        self.0 & 0x7f
    }

    /// Bit 8, inferred.
    pub const fn set(self) -> bool {
        (self.0 >> 8) & 1 != 0
    }
}
