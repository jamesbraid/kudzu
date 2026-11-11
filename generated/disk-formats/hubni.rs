// generated from kudzu aa90cb88b76b HUBNI_DECODE.h - do not edit, regenerate in kudzu
//!
//! Node hub network interface words: node number, link state,
//! link reset and port error.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: Origin 200/2000 network status word, carries the node number.
pub const O200_STATUS_OFFSET: u32 = 0x600000;
/// established: Origin 300 network status word; bits 1:0 are tested as link state, bits 2:1 equal to 2 starts a link-reset wait.
pub const O300_STATUS_OFFSET: u32 = 0x680000;
/// established: Origin 300 port reset word; the kernel writes the value 6.
pub const O300_PORT_RESET_OFFSET: u32 = 0x680008;
/// established: Origin 300 reset enable word; the kernel writes 1; bit 0 is the one that matters (datasheet "reset ok" is inferred).
pub const O300_RESET_ENABLE_OFFSET: u32 = 0x680010;
/// established: Origin 300 port error word; bit 52 set with the enable word zero is treated as a tolerated link reset. The node-wide error check reads the clear word and then stores zero here, at three sites; the link interrupt handler only reads it and stores nothing.
pub const O300_ERROR_OFFSET: u32 = 0x680100;
/// established: Origin 300 port error clear word; the kernel reads it, then stores zero to the error word; whether the read clears anything is unknown.
pub const O300_ERROR_CLEAR_OFFSET: u32 = 0x680120;
/// established: read through the local alias and printed in the link error report; role unknown.
pub const O300_ERROR_REPORT_A_OFFSET: u32 = 0x680108;
/// established: as report word A.
pub const O300_ERROR_REPORT_B_OFFSET: u32 = 0x680110;
/// established: as report word A.
pub const O300_ERROR_REPORT_C_OFFSET: u32 = 0x680118;
/// established: top bit of the range of latched error bits the kernel tests.
pub const O300_ERROR_LATCHED_HI: u32 = 52;
/// established: bottom bit of that range.
pub const O300_ERROR_LATCHED_LO: u32 = 24;
/// established: link reset caused by another partition, with the enable word zero; that the chip latches it when a reset is refused is unknown.
pub const O300_ERROR_LINK_RESET_BIT: u32 = 52;

/// The `NSRI` word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Nsri(u32);

impl Nsri {
    pub const fn new(word: u32) -> Self {
        Self(word)
    }

    pub const fn raw(self) -> u32 {
        self.0
    }

    /// Bits 16:8, established.
    pub const fn nodeid(self) -> u32 {
        (self.0 >> 8) & 0x1ff
    }
}
