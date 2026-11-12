/* generated from defs/gbe_capture.toml by defs-gen - do not edit, change the source */
/* O2 GBE capture block as the IRIX video driver programs it */
#ifndef GBE_CAPTURE_GEN_H
#define GBE_CAPTURE_GEN_H

#include <stdint.h>


/*
 * Capture control words VC_0 to VC_8, as the reading names them. The reading gives the first and last word offsets, 0x80000 and 0x80020, so the 4-byte stride is inferred. The reading established that VC_0 to VC_3 are written as zero when a capture path stops. Its word width is inferred from the word-sized copies the start-up makes. VC_2 bit 0 enables DMA and VC_3 holds a path byte, both inferred from the start routine.
 */
/* vc: rw32, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define GBE_CAPTURE_VC_OFFSET                        0x0u /* inferred */
#define GBE_CAPTURE_VC_COUNT                         9
#define GBE_CAPTURE_VC_STRIDE                        4

#endif /* GBE_CAPTURE_GEN_H */
