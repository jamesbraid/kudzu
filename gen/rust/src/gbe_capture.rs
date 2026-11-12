// generated from defs/gbe_capture.toml by defs-gen - do not edit, change the source
// O2 GBE capture block as the IRIX video driver programs it
#![allow(dead_code)]

/// Capture control words VC_0 to VC_8, as the reading names them. The reading gives the first and last word offsets, 0x80000 and 0x80020, so the 4-byte s (inferred)
pub mod vc {
    pub const WIDTH: u32 = 32;
    pub const OFFSET: u64 = 0x0;
    pub const COUNT: u32 = 9;
    pub const STRIDE: u64 = 4;
}
