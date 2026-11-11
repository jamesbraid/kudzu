// generated from kudzu 393674ed2650 NIC_DECODE.h - do not edit, regenerate in kudzu
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
/// established: offset of the register from the hub's local-window base.
pub const HUB_ACCESS_OFFSET: u32 = 0xe04030;
/// established: bytes read from the part.
pub const MAC_RECORD_LEN: u32 = 14;
/// established: required value of record byte 0.
pub const MAC_ID_BYTE0: u32 = 141;
/// established: required value of record byte 1.
pub const MAC_ID_BYTE1: u32 = 10;
/// established: CRC-16 residue over record bytes 1 to 13.
pub const MAC_CRC16_RESIDUE: u32 = 0xb001;
/// established: the value stored.
pub const GPCR_S_NIC_ENABLE_WORD: u32 = 0x200000;

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

/// The `GPCR_S` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct GpcrS(u32);

impl GpcrS {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bit 21, inferred.
    pub const fn nic_enable(self) -> bool {
        (self.0 >> 21) & 1 != 0
    }
}
