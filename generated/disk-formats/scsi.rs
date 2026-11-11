// generated from kudzu c64e3113d062 SCSI_DECODE.h - do not edit, regenerate in kudzu
//!
//! SCSI opcodes the CD-ROM driver picks by drive class.
//!
//! Grades on each item say what is known about it: `established` is what the
//! shipped driver software does, read in full; `inferred` is a reasoned
//! reading; `unknown` is not derived. None of them is a measurement of the
//! silicon.

#![allow(dead_code)]

/// established: sent by every class except the first three.
pub const START_STOP_UNIT: u32 = 27;
/// established: sent by classes 1 to 3 in place of START STOP UNIT; use as software eject is inferred.
pub const CDROM_VENDOR_C4: u32 = 196;
/// established: sent only by classes 1 to 3; use as a mode switch is inferred.
pub const CDROM_VENDOR_C9: u32 = 201;
