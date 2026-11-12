/* generated from defs/mace_video.toml by defs-gen - do not edit, change the source */
/* O2 MACE video window map and input channel registers as the IRIX O2 video driver reaches them */
#ifndef MACE_VIDEO_GEN_H
#define MACE_VIDEO_GEN_H

#include <stdint.h>


/*
 * Channel control word. The driver writes the whole word, changing only its low nine bits. The reading read the routines that write it in a workspace and did not re-read them, so the grade is inferred.
 */
/* in_control: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_CONTROL_OFFSET                 0x0u /* inferred */
#define MACE_VIDEO_IN_CONTROL_INTR_ENABLE_SHIFT      1 /* inferred */
#define MACE_VIDEO_IN_CONTROL_INTR_ENABLE_MASK       0xffull
#define MACE_VIDEO_IN_CONTROL_INTR_ENABLE_INPLACE    (MACE_VIDEO_IN_CONTROL_INTR_ENABLE_MASK << MACE_VIDEO_IN_CONTROL_INTR_ENABLE_SHIFT)
#define MACE_VIDEO_IN_CONTROL_DMA_ENABLE_SHIFT       0 /* inferred */
#define MACE_VIDEO_IN_CONTROL_DMA_ENABLE_MASK        0x1ull
#define MACE_VIDEO_IN_CONTROL_DMA_ENABLE_INPLACE     (MACE_VIDEO_IN_CONTROL_DMA_ENABLE_MASK << MACE_VIDEO_IN_CONTROL_DMA_ENABLE_SHIFT)

/*
 * Status word. Reading it clears the pending interrupt causes. The page numbers are the reading's citation and were not checked against the document.
 */
/* in_status: r64, reset unknown; MACE 2.0 specification, pages 36 and 37 as the reading cites them */
#define MACE_VIDEO_IN_STATUS_OFFSET                  0x8u /* inferred */

/*
 * Configuration word, written at start-up. Its field layout is not recorded.
 */
/* in_config: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_CONFIG_OFFSET                  0x10u /* established */

/*
 * Physical address of the next descriptor block, held as a zero-extended 32-bit word. The driver ORs a flag byte into the word. Which bits carry the flag is not stated.
 */
/* in_next_descriptor: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_NEXT_DESCRIPTOR_OFFSET         0x18u /* inferred */

/*
 * Field offset. The driver writes the low halfword of a transfer-record word here.
 */
/* in_field_offset: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_FIELD_OFFSET_OFFSET            0x20u /* inferred */
#define MACE_VIDEO_IN_FIELD_OFFSET_OFFSET_SHIFT      0 /* inferred */
#define MACE_VIDEO_IN_FIELD_OFFSET_OFFSET_MASK       0xffffull
#define MACE_VIDEO_IN_FIELD_OFFSET_OFFSET_INPLACE    (MACE_VIDEO_IN_FIELD_OFFSET_OFFSET_MASK << MACE_VIDEO_IN_FIELD_OFFSET_OFFSET_SHIFT)

/*
 * Line width word. The name is the reading's label; the start-up writes it with a 64-bit store.
 */
/* in_line_width: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_LINE_WIDTH_OFFSET              0x28u /* inferred */

/*
 * Horizontal clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded.
 */
/* in_hclip_odd: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_HCLIP_ODD_OFFSET               0x30u /* established */

/*
 * Vertical clip word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded.
 */
/* in_vclip_odd: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_VCLIP_ODD_OFFSET               0x38u /* established */

/*
 * Alpha word for the odd field. The start-up writes it with a 64-bit store. Its layout is not recorded.
 */
/* in_alpha_odd: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_ALPHA_ODD_OFFSET               0x40u /* established */

/*
 * Horizontal clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded.
 */
/* in_hclip_even: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_HCLIP_EVEN_OFFSET              0x48u /* established */

/*
 * Vertical clip word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded.
 */
/* in_vclip_even: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_VCLIP_EVEN_OFFSET              0x50u /* established */

/*
 * Alpha word for the even field. The start-up writes it with a 64-bit store. Its layout is not recorded.
 */
/* in_alpha_even: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_IN_ALPHA_EVEN_OFFSET              0x58u /* established */

#define MACE_VIDEO_GBE_CAPTURE_OFF                   0x80000u /* established */
#define MACE_VIDEO_IN2_OFF                           0x80000u /* established */
#define MACE_VIDEO_OUT_OFF                           0x100000u /* established */
#define MACE_VIDEO_TIMER_PHYS_ADDR                   0x1f340000u /* established */
#define MACE_VIDEO_TIMER_SNAP_IN1_OFF                0x38u /* established */
#define MACE_VIDEO_TIMER_SNAP_IN2_OFF                0x40u /* established */
#define MACE_VIDEO_TIMER_SNAP_OUT_OFF                0x48u /* established */
#define MACE_VIDEO_TWO_WIRE_PHYS_ADDR                0x1f330000u /* established */
#define MACE_VIDEO_WINDOW_PHYS_ADDR                  0x1f100000u /* established */

#endif /* MACE_VIDEO_GEN_H */
