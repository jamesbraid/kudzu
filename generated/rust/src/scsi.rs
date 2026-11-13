// generated from formats/scsi/format.toml by defs-gen - do not edit, change the source
// SCSI command opcodes the CD-ROM driver selects by drive class
#![allow(dead_code)]

pub const CDROM_VENDOR_C4: u32 = 0xc4; // established
pub const CDROM_VENDOR_C9: u32 = 0xc9; // established
pub const INQ_LEN_PRODUCT: u32 = 0x10; // inferred
pub const INQ_LEN_VENDOR: u32 = 0x8; // established
pub const INQ_OFF_PRODUCT: u32 = 0x10; // inferred
pub const INQ_OFF_TYPE: u32 = 0x0; // established
pub const INQ_OFF_VENDOR: u32 = 0x8; // established
pub const INQ_OFF_VERSION: u32 = 0x2; // established
pub const INQ_TYPE_CDROM: u32 = 0x5; // established
pub const INQ_TYPE_DISK: u32 = 0x0; // established
pub const INQ_TYPE_TAPE: u32 = 0x1; // established
pub const INQ_VERSION_MASK: u32 = 0x7; // established
pub const INQ_VERSION_REFINE: u32 = 0x2; // established
pub const PROM_PREFIX_LEN: u32 = 0xa; // established
pub const START_STOP_UNIT: u32 = 0x1b; // established
