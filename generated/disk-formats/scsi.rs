// generated from kudzu 113eeaa39a64 SCSI_DECODE.h - do not edit, regenerate in kudzu
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
/// established: device type byte; the PROMs test it for 0 and the daemon accepts only 5.
pub const INQ_OFF_TYPE: u32 = 0;
/// established: the PROMs test for this value before comparing the text.
pub const INQ_TYPE_DISK: u32 = 0;
/// established: named by the O2 PROM's probe; no other PROM tests it.
pub const INQ_TYPE_TAPE: u32 = 1;
/// established: the daemon accepts it; the PROMs never compare the text of a type 5 unit.
pub const INQ_TYPE_CDROM: u32 = 5;
/// established: version byte; the daemon masks it with 7 and tests for 2 to refine one drive family.
pub const INQ_OFF_VERSION: u32 = 2;
/// established.
pub const INQ_VERSION_REFINE: u32 = 2;
/// established: first byte of the vendor text, the start of the text the daemon matches.
pub const INQ_OFF_VENDOR: u32 = 8;
/// established: the notes and both readers treat vendor and product as one 24-byte text.
pub const INQ_LEN_VENDOR: u32 = 8;
/// inferred: follows the vendor field in that text.
pub const INQ_OFF_PRODUCT: u32 = 16;
/// inferred.
pub const INQ_LEN_PRODUCT: u32 = 16;
/// established: bytes of the trimmed vendor, separator and product that are compared.
pub const PROM_PREFIX_LEN: u32 = 10;
