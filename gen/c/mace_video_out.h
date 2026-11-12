/* generated from defs/mace_video_out.toml by defs-gen - do not edit, change the source */
/* O2 MACE video output channel registers as the IRIX O2 video driver reaches them */
#ifndef MACE_VIDEO_OUT_GEN_H
#define MACE_VIDEO_OUT_GEN_H

#include <stdint.h>


/*
 * Channel control word. The driver writes the whole word, changing only its low nine bits. The output recovery routines pulse bit 0. The reading read the routines that write it in a workspace and did not re-read them, so the grade is inferred.
 */
/* out_control: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_CONTROL_OFFSET            0x0u /* inferred */
#define MACE_VIDEO_OUT_OUT_CONTROL_INTR_ENABLE_SHIFT 1 /* inferred */
#define MACE_VIDEO_OUT_OUT_CONTROL_INTR_ENABLE_MASK  0xffull
#define MACE_VIDEO_OUT_OUT_CONTROL_INTR_ENABLE_INPLACE (MACE_VIDEO_OUT_OUT_CONTROL_INTR_ENABLE_MASK << MACE_VIDEO_OUT_OUT_CONTROL_INTR_ENABLE_SHIFT)
#define MACE_VIDEO_OUT_OUT_CONTROL_DMA_ENABLE_SHIFT  0 /* inferred */
#define MACE_VIDEO_OUT_OUT_CONTROL_DMA_ENABLE_MASK   0x1ull
#define MACE_VIDEO_OUT_OUT_CONTROL_DMA_ENABLE_INPLACE (MACE_VIDEO_OUT_OUT_CONTROL_DMA_ENABLE_MASK << MACE_VIDEO_OUT_OUT_CONTROL_DMA_ENABLE_SHIFT)

/*
 * Status word. Reading it clears the pending interrupt causes. The output recovery routines read it after their last write and discard the value. The page numbers were not checked against the document.
 */
/* out_status: r64, reset unknown; MACE 2.0 specification, pages 36 and 37 as the reading cites them */
#define MACE_VIDEO_OUT_OUT_STATUS_OFFSET             0x8u /* inferred */

/*
 * Configuration word, written at start-up. Its field layout is not recorded here; the hardware configuration register at at 0x58 is a separate word.
 */
/* out_config: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_CONFIG_OFFSET             0x10u /* established */

/*
 * Physical address of the next descriptor block, held as a zero-extended 32-bit word. The recovery routines write it as well. Which bits carry the flag byte the driver ORs in is not stated.
 */
/* out_next_descriptor: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_NEXT_DESCRIPTOR_OFFSET    0x18u /* inferred */

/*
 * Field offset. The driver writes the low halfword of a transfer-record word here.
 */
/* out_field_offset: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_FIELD_OFFSET_OFFSET       0x20u /* inferred */
#define MACE_VIDEO_OUT_OUT_FIELD_OFFSET_OFFSET_SHIFT 0 /* inferred */
#define MACE_VIDEO_OUT_OUT_FIELD_OFFSET_OFFSET_MASK  0xffffull
#define MACE_VIDEO_OUT_OUT_FIELD_OFFSET_OFFSET_INPLACE (MACE_VIDEO_OUT_OUT_FIELD_OFFSET_OFFSET_MASK << MACE_VIDEO_OUT_OUT_FIELD_OFFSET_OFFSET_SHIFT)

/*
 * Field size. The reading takes the line count and the bytes per line from one stored value, which it gives as 243 lines of 2880 bytes in one set and 288 lines in the other.
 */
/* out_field_size: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_FIELD_SIZE_OFFSET         0x28u /* inferred */
#define MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINES_SHIFT    12 /* inferred */
#define MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINES_MASK     0x3ffull
#define MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINES_INPLACE  (MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINES_MASK << MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINES_SHIFT)
#define MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINE_BYTES_SHIFT 0 /* inferred */
#define MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINE_BYTES_MASK 0xfffull
#define MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINE_BYTES_INPLACE (MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINE_BYTES_MASK << MACE_VIDEO_OUT_OUT_FIELD_SIZE_LINE_BYTES_SHIFT)

/*
 * Horizontal padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded.
 */
/* out_h_pad_odd: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_H_PAD_ODD_OFFSET          0x30u /* established */

/*
 * Vertical padding word for the odd field. The reading states it is a 64-bit value. Its layout is not recorded.
 */
/* out_v_pad_odd: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_V_PAD_ODD_OFFSET          0x38u /* established */

/*
 * Horizontal padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded.
 */
/* out_h_pad_even: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_H_PAD_EVEN_OFFSET         0x40u /* established */

/*
 * Vertical padding word for the even field. The reading states it is a 64-bit value. Its layout is not recorded.
 */
/* out_v_pad_even: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_V_PAD_EVEN_OFFSET         0x48u /* established */

/*
 * Genlock delay word, written at start-up with a 64-bit store. Its layout is not recorded.
 */
/* out_genlock_delay: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_GENLOCK_DELAY_OFFSET      0x50u /* established */

/*
 * Hardware configuration word for the output channel: port reset and select bits, audio and genlock sync settings, the general-purpose output bit and the framelock start-of-field counts. Bits 4, 7 and 8 and the audio-sync reset bit are not assigned by the reading.
 */
/* out_hw_config: rw64, reset unknown; MACE 2.0 specification layout, as the shipped IRIX O2 video driver object uses it, per docs/mace-video.md */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_OFFSET          0x58u /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_IDENT_SHIFT     32 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_IDENT_MASK      0xfull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_IDENT_INPLACE   (MACE_VIDEO_OUT_OUT_HW_CONFIG_IDENT_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_IDENT_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_EVEN_SHIFT 19 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_EVEN_MASK 0xfull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_EVEN_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_EVEN_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_EVEN_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_GENERAL_OUTPUT_SHIFT 18 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_GENERAL_OUTPUT_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_GENERAL_OUTPUT_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_GENERAL_OUTPUT_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_GENERAL_OUTPUT_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_ODD_SHIFT 14 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_ODD_MASK 0xfull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_ODD_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_ODD_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_SOF_COUNT_ODD_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_CORRECTION_SHIFT 13 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_CORRECTION_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_CORRECTION_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_CORRECTION_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_CORRECTION_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_PRECISION_SHIFT 12 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_PRECISION_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_PRECISION_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_PRECISION_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_PRECISION_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_SOURCE_SHIFT 10 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_SOURCE_MASK 0x3ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_SOURCE_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_SOURCE_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_SOURCE_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_RESET_SHIFT 9 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_RESET_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_RESET_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_RESET_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_FRAMELOCK_RESET_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_AUDIO_SYNC_SOURCE_SHIFT 5 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_AUDIO_SYNC_SOURCE_MASK 0x3ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_AUDIO_SYNC_SOURCE_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_AUDIO_SYNC_SOURCE_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_AUDIO_SYNC_SOURCE_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_SELECT_SHIFT 3 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_SELECT_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_SELECT_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_SELECT_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_SELECT_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_RESET_SHIFT 2 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_RESET_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_RESET_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_RESET_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_CD_RESET_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_SELECT_SHIFT 1 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_SELECT_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_SELECT_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_SELECT_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_SELECT_SHIFT)
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_RESET_SHIFT 0 /* inferred */
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_RESET_MASK 0x1ull
#define MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_RESET_INPLACE (MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_RESET_MASK << MACE_VIDEO_OUT_OUT_HW_CONFIG_PORT_AB_RESET_SHIFT)

#endif /* MACE_VIDEO_OUT_GEN_H */
