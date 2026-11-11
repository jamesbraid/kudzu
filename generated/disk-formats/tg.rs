// generated from kudzu fdb059d4cbe7 TG_DECODE.h - do not edit, regenerate in kudzu
//!
//! Gigabit Ethernet controller match table: the PCI vendor, device
//! and subsystem ids the Origin 300/350 driver accepts.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: bytes between table rows.
pub const MATCH_ROW_STRIDE: u32 = 16;
/// established: rows the attach loop walks.
pub const MATCH_ROW_COUNT: u32 = 5;
/// established.
pub const CFG_OFF_DEVICE: u32 = 0;
/// established.
pub const CFG_OFF_VENDOR: u32 = 2;
/// established.
pub const CFG_OFF_SUBSYS_VENDOR: u32 = 44;
/// established.
pub const CFG_OFF_SUBSYS_DEVICE: u32 = 46;
/// established: the vendor id of every row.
pub const PCI_VENDOR: u32 = 0x14e4;
/// established: the subsystem vendor of every row.
pub const PCI_SUBSYS_VENDOR: u32 = 0x10a9;
/// established: three rows (subsystem 0x8010, 0x8011, 0x8012).
pub const PCI_DEVICE_1645: u32 = 0x1645;
/// established: one row (subsystem 0x8013).
pub const PCI_DEVICE_1648: u32 = 0x1648;
/// established: one row (subsystem 0x8014).
pub const PCI_DEVICE_16A8: u32 = 0x16a8;
/// established: subsystem device of the first row; the rows run consecutively to 0x8014.
pub const PCI_SUBSYS_DEVICE_FIRST: u32 = 0x8010;

/// The `ROW_W0` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct RowW0(u32);

impl RowW0 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn vendor(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn device(self) -> u32 {
        self.0 & 0xffff
    }
}

/// The `ROW_W1` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct RowW1(u32);

impl RowW1 {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 31:16, established.
    pub const fn subsys_vendor(self) -> u32 {
        self.0 >> 16
    }

    /// Bits 15:0, established.
    pub const fn subsys_device(self) -> u32 {
        self.0 & 0xffff
    }
}
