/* generated from defs/crime.toml by defs-gen - do not edit, change the source */
/* O2 CRIME as the shipped IRIX kernel and graphics libraries drive it: identity, interrupt inputs, renderer state and TLB formats */
#ifndef CRIME_GEN_H
#define CRIME_GEN_H

#include <stdint.h>


/*
 * Identity byte at the start of the CRIME block. The graphics probe reads its low byte and accepts the part only when bits 7:4 are the accepted family and the byte is not the refused value.
 */
/* id: r8, reset unknown; shipped IRIX O2 graphics objects, per docs/crime.md */
#define CRIME_ID_OFFSET                              0x0u /* established */
#define CRIME_ID_FAMILY_SHIFT                        4 /* established */
#define CRIME_ID_FAMILY_MASK                         0xfu
#define CRIME_ID_FAMILY_INPLACE                      (CRIME_ID_FAMILY_MASK << CRIME_ID_FAMILY_SHIFT)

/*
 * Texture format record, written by the OpenGL library for each texture. It carries the log2 sizes of the texture and the coordinate masks. The width of the register is taken from the top field, which reaches bit 47.
 */
/* texture_format: w64, reset unknown; shipped IRIX O2 graphics objects, per docs/crime.md */
#define CRIME_TEXTURE_FORMAT_OFFSET                  0x2118u /* established */
#define CRIME_TEXTURE_FORMAT_WIDTH_INV_SHIFT         44 /* established */
#define CRIME_TEXTURE_FORMAT_WIDTH_INV_MASK          0xfull
#define CRIME_TEXTURE_FORMAT_WIDTH_INV_INPLACE       (CRIME_TEXTURE_FORMAT_WIDTH_INV_MASK << CRIME_TEXTURE_FORMAT_WIDTH_INV_SHIFT)
#define CRIME_TEXTURE_FORMAT_HEIGHT_INV_SHIFT        40 /* established */
#define CRIME_TEXTURE_FORMAT_HEIGHT_INV_MASK         0xfull
#define CRIME_TEXTURE_FORMAT_HEIGHT_INV_INPLACE      (CRIME_TEXTURE_FORMAT_HEIGHT_INV_MASK << CRIME_TEXTURE_FORMAT_HEIGHT_INV_SHIFT)
#define CRIME_TEXTURE_FORMAT_WIDTH_LOG2_SHIFT        36 /* established */
#define CRIME_TEXTURE_FORMAT_WIDTH_LOG2_MASK         0xfull
#define CRIME_TEXTURE_FORMAT_WIDTH_LOG2_INPLACE      (CRIME_TEXTURE_FORMAT_WIDTH_LOG2_MASK << CRIME_TEXTURE_FORMAT_WIDTH_LOG2_SHIFT)
#define CRIME_TEXTURE_FORMAT_HEIGHT_LOG2_SHIFT       32 /* established */
#define CRIME_TEXTURE_FORMAT_HEIGHT_LOG2_MASK        0xfull
#define CRIME_TEXTURE_FORMAT_HEIGHT_LOG2_INPLACE     (CRIME_TEXTURE_FORMAT_HEIGHT_LOG2_MASK << CRIME_TEXTURE_FORMAT_HEIGHT_LOG2_SHIFT)
#define CRIME_TEXTURE_FORMAT_WIDTH_MASK_SHIFT        16 /* established */
#define CRIME_TEXTURE_FORMAT_WIDTH_MASK_MASK         0xffffull
#define CRIME_TEXTURE_FORMAT_WIDTH_MASK_INPLACE      (CRIME_TEXTURE_FORMAT_WIDTH_MASK_MASK << CRIME_TEXTURE_FORMAT_WIDTH_MASK_SHIFT)
#define CRIME_TEXTURE_FORMAT_HEIGHT_MASK_SHIFT       0 /* established */
#define CRIME_TEXTURE_FORMAT_HEIGHT_MASK_MASK        0xffffull
#define CRIME_TEXTURE_FORMAT_HEIGHT_MASK_INPLACE     (CRIME_TEXTURE_FORMAT_HEIGHT_MASK_MASK << CRIME_TEXTURE_FORMAT_HEIGHT_MASK_SHIFT)

/*
 * Interface-buffer address word, as the kernel decodes it in software. Bit 45 marks the stop entry.
 */
/* word IB_ADDR: 64 bits; shipped IRIX O2 graphics objects, per docs/crime.md */
#define CRIME_IB_ADDR_WIDTH                          64 /* established */
#define CRIME_IB_ADDR_STOP_SHIFT                     45 /* established */
#define CRIME_IB_ADDR_STOP_MASK                      0x1ull
#define CRIME_IB_ADDR_STOP_INPLACE                   (CRIME_IB_ADDR_STOP_MASK << CRIME_IB_ADDR_STOP_SHIFT)
#define CRIME_IB_ADDR_LANE_SHIFT                     43 /* established */
#define CRIME_IB_ADDR_LANE_MASK                      0x3ull
#define CRIME_IB_ADDR_LANE_INPLACE                   (CRIME_IB_ADDR_LANE_MASK << CRIME_IB_ADDR_LANE_SHIFT)
#define CRIME_IB_ADDR_GROUP_SHIFT                    40 /* established */
#define CRIME_IB_ADDR_GROUP_MASK                     0x7ull
#define CRIME_IB_ADDR_GROUP_INPLACE                  (CRIME_IB_ADDR_GROUP_MASK << CRIME_IB_ADDR_GROUP_SHIFT)
#define CRIME_IB_ADDR_REG_INDEX_SHIFT                32 /* established */
#define CRIME_IB_ADDR_REG_INDEX_MASK                 0xffull
#define CRIME_IB_ADDR_REG_INDEX_INPLACE              (CRIME_IB_ADDR_REG_INDEX_MASK << CRIME_IB_ADDR_REG_INDEX_SHIFT)

#define CRIME_AA_LINE_OFF                            0x2190u /* established */
#define CRIME_ALPHA_TEST_OFF                         0x2198u /* established */
#define CRIME_BLEND_FUNC_OFF                         0x21a8u /* established */
#define CRIME_BLOCK_ADDR                             0x14000000u /* established */
#define CRIME_CID_TLB_REGS                           0x4u /* established */
#define CRIME_CONTROL_OFF                            0x8u /* established */
#define CRIME_CPU_ERR_STATUS_OFF                     0x48u /* established */
#define CRIME_DEPTH_APERTURE_ADDR                    0x12000000u /* inferred */
#define CRIME_DEPTH_FUNC_OFF                         0x21c0u /* established */
#define CRIME_DMA_IOCTL                              0x55f2u /* inferred */
#define CRIME_DMA_POLL_LIMIT                         0x7a120u /* established */
#define CRIME_DMA_REQ_BYTES                          0x28u /* established */
#define CRIME_DRAWMODE_OFF                           0x2018u /* established */
#define CRIME_ENGINE_RESET_OFF                       0x408u /* established */
#define CRIME_FB_TLB_REGS                            0x40u /* established */
#define CRIME_FOG_COLOR_OFF                          0x2170u /* established */
#define CRIME_FRAME_APERTURE_ADDR                    0x10000000u /* inferred */
#define CRIME_FULL_MEM_ADDR                          0x40000000u /* inferred */
#define CRIME_GBE_WINDOW_ADDR                        0x16000000u /* inferred */
#define CRIME_GL_UNBOUND_BIAS                        0x21000u /* established */
#define CRIME_GL_VERTEX_BIAS                         0x1000u /* established */
#define CRIME_HIGH_ALIAS_ADDR                        0x20000000u /* inferred */
#define CRIME_ID_FAMILY_ACCEPTED                     0xau /* established */
#define CRIME_ID_REFUSED_BYTE                        0xa0u /* established */
#define CRIME_ID_SPEC_15                             0xb1u /* inferred */
#define CRIME_INTMASK_OFF                            0x18u /* established */
#define CRIME_INTR_EDGE_OFF                          0x28u /* established */
#define CRIME_INTSTAT_OFF                            0x10u /* established */
#define CRIME_IRQ_BUS_ERR_A                          0x14u /* established */
#define CRIME_IRQ_BUS_ERR_B                          0x15u /* established */
#define CRIME_IRQ_FIFO_HIGH                          0x1au /* established */
#define CRIME_IRQ_FIFO_LOW                           0x19u /* established */
#define CRIME_IRQ_GBE_LINE_2                         0x12u /* established */
#define CRIME_IRQ_GBE_LINE_3                         0x13u /* established */
#define CRIME_IRQ_MACE_ISA_A                         0x4u /* established */
#define CRIME_IRQ_MACE_ISA_B                         0x5u /* established */
#define CRIME_IRQ_PREBLANK                           0x11u /* established */
#define CRIME_IRQ_RETRACE                            0x10u /* established */
#define CRIME_IRQ_SOFT_A                             0x1cu /* inferred */
#define CRIME_IRQ_SOFT_B                             0x1du /* inferred */
#define CRIME_LINEAR_WINDOW_MASK                     0x1ffffu /* established */
#define CRIME_LINE_MATCH_LATCHES                     0x4u /* inferred */
#define CRIME_LIN_TLB_MAX_PAGES                      0x20u /* established */
#define CRIME_LIN_TLB_REGS                           0x10u /* established */
#define CRIME_LIN_TLB_VALID                          0x80000000u /* established */
#define CRIME_LOGIC_OP_OFF                           0x21b0u /* established */
#define CRIME_MACE_WINDOW_ADDR                       0x18000000u /* inferred */
#define CRIME_MEM_CTL_OFF                            0x200u /* established */
#define CRIME_MEM_ERR_STATUS_OFF                     0x250u /* established */
#define CRIME_MTE_HOST_CHUNK_BYTES                   0x20000u /* established */
#define CRIME_MTE_MODE_HOST_CLEAR                    0x11u /* established */
#define CRIME_MTE_MODE_HOST_COPY                     0x897u /* established */
#define CRIME_MTE_MODE_X_COPY                        0x820u /* established */
#define CRIME_MTE_X_BLOCK_BYTES                      0x100u /* established */
#define CRIME_MTE_X_ROWS_16                          0x10u /* established */
#define CRIME_MTE_X_ROWS_32                          0x8u /* established */
#define CRIME_NOECC_ALIAS_ADDR                       0x80000000u /* inferred */
#define CRIME_PAGE_BYTES                             0x1000u /* inferred */
#define CRIME_PIXFLUSH_OFF                           0x21f8u /* established */
#define CRIME_PIXPIPE_NULL_OFF                       0x21f0u /* established */
#define CRIME_PIXXFER_POS_OFF                        0x20a0u /* established */
#define CRIME_PIXXFER_VTX_OFF                        0x2070u /* established */
#define CRIME_PIXXFER_YSTEP_OFF                      0x20acu /* established */
#define CRIME_PRIMITIVE_OFF                          0x2060u /* established */
#define CRIME_PRIM_RECT                              0x3020000u /* established */
#define CRIME_PRIM_TRIANGLE                          0x2000000u /* established */
#define CRIME_RENDER_COUNT_INIT                      0xau /* established */
#define CRIME_RENDER_CTL_INIT                        0xfefff0au /* established */
#define CRIME_RENDER_CTL_OFF                         0x400u /* established */
#define CRIME_RENDER_EMPTY_SHIFT                     0xeu /* established */
#define CRIME_RENDER_FIELD_MASK                      0x7fu /* established */
#define CRIME_RENDER_FIFO_DEPTH                      0x40u /* inferred */
#define CRIME_RENDER_FLUSH_OFF                       0x4008u /* established */
#define CRIME_RENDER_FULL_SHIFT                      0x15u /* established */
#define CRIME_RENDER_OCC_SHIFT                       0x12u /* established */
#define CRIME_RENDER_R10K_EMPTY                      0x0u /* established */
#define CRIME_RENDER_R10K_FULL                       0x14u /* established */
#define CRIME_RENDER_R10K_STALL                      0x13u /* established */
#define CRIME_RENDER_R5K_EMPTY                       0x18u /* established */
#define CRIME_RENDER_R5K_FULL                        0x30u /* established */
#define CRIME_RENDER_R5K_STALL                       0x2fu /* established */
#define CRIME_RENDER_STALL_SHIFT                     0x7u /* established */
#define CRIME_RENDER_START_MASK                      0x3fu /* established */
#define CRIME_RENDER_STATUS_IDLE_BIT                 0x1cu /* established */
#define CRIME_RENDER_STATUS_OFF                      0x4000u /* established */
#define CRIME_RENDER_WPTR_SHIFT                      0x6u /* established */
#define CRIME_SOFTINT_OFF                            0x20u /* inferred */
#define CRIME_START_ALIAS_OFF                        0x800u /* established */
#define CRIME_STENCIL_MASK_OFF                       0x21e8u /* established */
#define CRIME_STENCIL_MODE_OFF                       0x21e0u /* established */
#define CRIME_TEXTURE_MODE_OFF                       0x2110u /* established */
#define CRIME_TEX_LOG2_BYTES_SHIFT                   0x12u /* established */
#define CRIME_TEX_MAX_TEXELS                         0x400u /* inferred */
#define CRIME_TEX_SUBTILE_BIT                        0x18u /* established */
#define CRIME_TEX_SUBTILE_CELL_BYTES                 0x20u /* inferred */
#define CRIME_TEX_TILE_FLAG                          0x8000u /* established */
#define CRIME_TEX_TLB_REGS                           0x1cu /* established */
#define CRIME_TEX_TYPE_SHIFT                         0x14u /* established */
#define CRIME_TILE_BYTES                             0x10000u /* inferred */
#define CRIME_TILE_PAGES                             0x10u /* established */
#define CRIME_TILE_ROWS                              0x80u /* inferred */
#define CRIME_TILE_ROW_BYTES                         0x200u /* inferred */
#define CRIME_TIME_OFF                               0x38u /* established */
#define CRIME_TIME_RESOLUTION                        0x3a98u /* established */
#define CRIME_VTX_DEPTH_BASE                         0x840000u /* established */
#define CRIME_VTX_STEP                               0x40u /* established */
#define CRIME_VTX_TRI_BASE                           0x48040000u /* established */
#define CRIME_WATCHDOG_OFF                           0x30u /* established */

#endif /* CRIME_GEN_H */
