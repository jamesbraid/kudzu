// @generated from formats/r10k_tlb/format.toml by defs-gen - do not edit, change the source
// defs-gen 4
// input sha256 d7b6d0493f2d5ba86caa56678e4ca4a5206dd452b65a3dc96839d93e7773954d
// The wired-entry layout IRIX uses in the R10000 TLB
#![allow(dead_code)]
#![allow(clippy::doc_markdown)]
#![cfg_attr(rustfmt, rustfmt_skip)]

pub const ASID_USABLE: u32 = 0xff; // inferred
pub const BASE_PAGE_BYTES: u32 = 0x4000; // inferred
pub const BROADCAST_SHARERS: u32 = 0x10; // inferred
pub const IO_MAP_BYTES: u32 = 0x1000000; // inferred
pub const KTEXT_BYTES: u32 = 0x1000000; // inferred
pub const LARGE_PAGE_MIN_BYTES: u32 = 0x10000; // inferred
pub const WIRED_INDEX_CPU: u32 = 0x0; // inferred
pub const WIRED_INDEX_DROPIN_FIRST: u32 = 0x2; // inferred
pub const WIRED_INDEX_DROPIN_LAST: u32 = 0x6; // inferred
pub const WIRED_INDEX_KTEXT: u32 = 0x1; // inferred
pub const WIRED_MAPPED: u32 = 0x7; // inferred
