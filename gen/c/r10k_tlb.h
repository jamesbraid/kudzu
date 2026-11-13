/* generated from defs/r10k_tlb.toml by defs-gen - do not edit, change the source */
/* The wired-entry layout IRIX uses in the R10000 TLB */
#ifndef R10K_TLB_GEN_H
#define R10K_TLB_GEN_H

#include <stdint.h>


#define R10K_TLB_ASID_USABLE                         0xffu /* inferred */
#define R10K_TLB_BASE_PAGE_BYTES                     0x4000u /* inferred */
#define R10K_TLB_BROADCAST_SHARERS                   0x10u /* inferred */
#define R10K_TLB_IO_MAP_BYTES                        0x1000000u /* inferred */
#define R10K_TLB_KTEXT_BYTES                         0x1000000u /* inferred */
#define R10K_TLB_LARGE_PAGE_MIN_BYTES                0x10000u /* inferred */
#define R10K_TLB_RECYCLE_TICKS_DEFAULT               0x64u /* inferred */
#define R10K_TLB_WIRED_INDEX_CPU                     0x0u /* inferred */
#define R10K_TLB_WIRED_INDEX_DROPIN_FIRST            0x2u /* inferred */
#define R10K_TLB_WIRED_INDEX_DROPIN_LAST             0x6u /* inferred */
#define R10K_TLB_WIRED_INDEX_KTEXT                   0x1u /* inferred */
#define R10K_TLB_WIRED_MAPPED                        0x7u /* inferred */
#define R10K_TLB_WIRED_UNMAPPED                      0x6u /* inferred */

#endif /* R10K_TLB_GEN_H */
