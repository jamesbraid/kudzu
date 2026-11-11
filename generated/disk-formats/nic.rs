// generated from kudzu 460c8a2ecf06 NIC_DECODE.h - do not edit, regenerate in kudzu
//!
//! Bit-serial control word for the one-wire identity parts and
//! the hub address that reaches the same bus.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: physical address of the register for node 0.
pub const HUB_ACCESS_PHYS_LOW: u32 = 0x1e04030;

/// The `MCR` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Mcr(u32);

impl Mcr {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bit 10, established.
    pub const fn pulse(self) -> bool {
        (self.0 >> 10) & 1 != 0
    }

    /// Bit 2, established.
    pub const fn sample(self) -> bool {
        (self.0 >> 2) & 1 != 0
    }

    /// Bit 1, established.
    pub const fn done(self) -> bool {
        (self.0 >> 1) & 1 != 0
    }

    /// Bit 0, established.
    pub const fn result(self) -> bool {
        self.0 & 1 != 0
    }
}
