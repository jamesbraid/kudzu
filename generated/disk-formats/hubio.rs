// generated from kudzu 113eeaa39a64 HUBIO_DECODE.h - do not edit, regenerate in kudzu
//!
//! Origin 300 hub crosstalk-side window: the register tested by
//! the error path and the request-buffer table the I/O error scan reads.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: physical base of the window for node 0.
pub const WINDOW_BASE: u32 = 0x1c00000;
/// established: bytes between entries.
pub const CRB_STRIDE: u32 = 32;
/// established: entries the scan reads.
pub const CRB_COUNT: u32 = 15;

/// Register `flags`: read, 64-bit; reset unknown; established.
pub const REG_FLAGS: u32 = 0x01e8;
pub const REG_FLAGS_WIDTH: u32 = 64;
/// Register `crb_a`: read, 64-bit; reset unknown; established.
pub const REG_CRB_A: u32 = 0x0400;
pub const REG_CRB_A_WIDTH: u32 = 64;
/// Register `crb_b`: read, 64-bit; reset unknown; established.
pub const REG_CRB_B: u32 = 0x0408;
pub const REG_CRB_B_WIDTH: u32 = 64;
/// Register `crb_c`: read, 64-bit; reset unknown; established.
pub const REG_CRB_C: u32 = 0x0410;
pub const REG_CRB_C_WIDTH: u32 = 64;

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

/// The `crb_a` register word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct CrbA(u64);

impl CrbA {
    pub const fn new(word: u64) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u64 {
        self.0
    }

    /// Bit 0, established.
    pub const fn bit0(self) -> bool {
        self.0 & 1 != 0
    }

    /// Bit 49, established.
    pub const fn bit49(self) -> bool {
        (self.0 >> 49) & 1 != 0
    }

    /// Bit 50, established.
    pub const fn bit50(self) -> bool {
        (self.0 >> 50) & 1 != 0
    }

    /// Bit 51, established.
    pub const fn bit51(self) -> bool {
        (self.0 >> 51) & 1 != 0
    }

    /// Bits 54:52, established.
    pub const fn type_(self) -> u64 {
        (self.0 >> 52) & 0x7
    }

    /// Bit 55, established.
    pub const fn bit55(self) -> bool {
        (self.0 >> 55) & 1 != 0
    }
}

/// The `crb_b` register word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct CrbB(u64);

impl CrbB {
    pub const fn new(word: u64) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u64 {
        self.0
    }

    /// Bits 32:30, established.
    pub const fn kind(self) -> u64 {
        (self.0 >> 30) & 0x7
    }

    /// Bits 42:41, established.
    pub const fn sub(self) -> u64 {
        (self.0 >> 41) & 0x3
    }

    /// Bit 58, established.
    pub const fn engine_alt(self) -> bool {
        (self.0 >> 58) & 1 != 0
    }
}

/// The `crb_c` register word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct CrbC(u64);

impl CrbC {
    pub const fn new(word: u64) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u64 {
        self.0
    }

    /// Bit 48, established.
    pub const fn bit48(self) -> bool {
        (self.0 >> 48) & 1 != 0
    }
}
