/* generated from defs/gbe.toml by defs-gen - do not edit, change the source */
/* O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe */
#ifndef GBE_GEN_H
#define GBE_GEN_H

#include <stdint.h>


/*
 * Dot-clock (PLL) word. The timing routine clears the run bit, writes the timing words, then writes this word with the run bit set and waits 11 milliseconds. The coefficient fields hold M minus one, N minus one and P, as the kernel stores them.
 */
/* dotclk: rw32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_DOTCLK_OFFSET                            0x4u /* established */
#define GBE_DOTCLK_RUN_SHIFT                         20 /* established */
#define GBE_DOTCLK_RUN_MASK                          0x1u
#define GBE_DOTCLK_RUN_INPLACE                       (GBE_DOTCLK_RUN_MASK << GBE_DOTCLK_RUN_SHIFT)
#define GBE_DOTCLK_PLL_P_SHIFT                       14 /* established */
#define GBE_DOTCLK_PLL_P_MASK                        0x3u
#define GBE_DOTCLK_PLL_P_INPLACE                     (GBE_DOTCLK_PLL_P_MASK << GBE_DOTCLK_PLL_P_SHIFT)
#define GBE_DOTCLK_PLL_N_SHIFT                       8 /* established */
#define GBE_DOTCLK_PLL_N_MASK                        0x3fu
#define GBE_DOTCLK_PLL_N_INPLACE                     (GBE_DOTCLK_PLL_N_MASK << GBE_DOTCLK_PLL_N_SHIFT)
#define GBE_DOTCLK_PLL_M_SHIFT                       0 /* established */
#define GBE_DOTCLK_PLL_M_MASK                        0xffu
#define GBE_DOTCLK_PLL_M_INPLACE                     (GBE_DOTCLK_PLL_M_MASK << GBE_DOTCLK_PLL_M_SHIFT)

/*
 * Identity word the kernel reads first. It accepts the block only when the word reads 0x666; any other value means the kernel leaves the block untouched.
 */
/* device_id: r32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_DEVICE_ID_OFFSET                         0x14u /* established */

/*
 * Vertical timing position word, called VT_0 in the reading. The start-up sequence compares its whole unmasked value with the vertical blank start line, and the stop sequence freezes it with the value in VT_XY_FREEZE. The offset follows from the VT_n numbering of the other timing words; the reading does not state it directly.
 */
/* vt_xy: rw32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_XY_OFFSET                             0x10000u /* inferred */

/*
 * Frame totals. The timing routine writes 0 here, and the start-up sequence writes the totals once the frame buffer is set up. The kernel stores the record's total pixels and total lines, not one less than them; whether the hardware expects one less is unknown.
 */
/* vt_1: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_1_OFFSET                              0x10004u /* established */
#define GBE_VT_1_TOTAL_LINES_SHIFT                   12 /* established */
#define GBE_VT_1_TOTAL_LINES_MASK                    0xfffu
#define GBE_VT_1_TOTAL_LINES_INPLACE                 (GBE_VT_1_TOTAL_LINES_MASK << GBE_VT_1_TOTAL_LINES_SHIFT)
#define GBE_VT_1_TOTAL_PIXELS_SHIFT                  0 /* established */
#define GBE_VT_1_TOTAL_PIXELS_MASK                   0xfffu
#define GBE_VT_1_TOTAL_PIXELS_INPLACE                (GBE_VT_1_TOTAL_PIXELS_MASK << GBE_VT_1_TOTAL_PIXELS_SHIFT)

/*
 * Vertical sync position, in lines.
 */
/* vt_2: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_2_OFFSET                              0x10008u /* established */
#define GBE_VT_2_VSYNC_START_SHIFT                   12 /* established */
#define GBE_VT_2_VSYNC_START_MASK                    0xfffu
#define GBE_VT_2_VSYNC_START_INPLACE                 (GBE_VT_2_VSYNC_START_MASK << GBE_VT_2_VSYNC_START_SHIFT)
#define GBE_VT_2_VSYNC_END_SHIFT                     0 /* established */
#define GBE_VT_2_VSYNC_END_MASK                      0xfffu
#define GBE_VT_2_VSYNC_END_INPLACE                   (GBE_VT_2_VSYNC_END_MASK << GBE_VT_2_VSYNC_END_SHIFT)

/*
 * Horizontal sync position, in pixels.
 */
/* vt_3: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_3_OFFSET                              0x1000cu /* established */
#define GBE_VT_3_HSYNC_START_SHIFT                   12 /* established */
#define GBE_VT_3_HSYNC_START_MASK                    0xfffu
#define GBE_VT_3_HSYNC_START_INPLACE                 (GBE_VT_3_HSYNC_START_MASK << GBE_VT_3_HSYNC_START_SHIFT)
#define GBE_VT_3_HSYNC_END_SHIFT                     0 /* established */
#define GBE_VT_3_HSYNC_END_MASK                      0xfffu
#define GBE_VT_3_HSYNC_END_INPLACE                   (GBE_VT_3_HSYNC_END_MASK << GBE_VT_3_HSYNC_END_SHIFT)

/*
 * Vertical blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active height and 0xfff. Other stores to this offset are outside what this unit records.
 */
/* vt_vblank: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_VBLANK_OFFSET                         0x10010u /* established */
#define GBE_VT_VBLANK_HI_SHIFT                       12 /* established */
#define GBE_VT_VBLANK_HI_MASK                        0xfffu
#define GBE_VT_VBLANK_HI_INPLACE                     (GBE_VT_VBLANK_HI_MASK << GBE_VT_VBLANK_HI_SHIFT)
#define GBE_VT_VBLANK_LO_SHIFT                       0 /* established */
#define GBE_VT_VBLANK_LO_MASK                        0xfffu
#define GBE_VT_VBLANK_LO_INPLACE                     (GBE_VT_VBLANK_LO_MASK << GBE_VT_VBLANK_LO_SHIFT)

/*
 * Horizontal blank word. The timing routine stores it in the interlace case and when no dual-channel option is fitted, packing the active width and 0xfff. Other stores to this offset are outside what this unit records.
 */
/* vt_hblank: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_HBLANK_OFFSET                         0x10014u /* established */
#define GBE_VT_HBLANK_HI_SHIFT                       12 /* established */
#define GBE_VT_HBLANK_HI_MASK                        0xfffu
#define GBE_VT_HBLANK_HI_INPLACE                     (GBE_VT_HBLANK_HI_MASK << GBE_VT_HBLANK_HI_SHIFT)
#define GBE_VT_HBLANK_LO_SHIFT                       0 /* established */
#define GBE_VT_HBLANK_LO_MASK                        0xfffu
#define GBE_VT_HBLANK_LO_INPLACE                     (GBE_VT_HBLANK_LO_MASK << GBE_VT_HBLANK_LO_SHIFT)

/*
 * Timing flag word. The timing routine reads it, clears bits 1 and 7, and ORs in a value of 0 or 2 for the byte 132 set-up. What bit 1 does to the hardware is unknown. The value is undefined when the option lookup that feeds it fails.
 */
/* timing_flags: rw32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_TIMING_FLAGS_OFFSET                      0x10018u /* established */

/*
 * Vertical blank start line in the high field. The low field repeats it when the frame-lock flag bits of the timing record (mask 0x12) are set, and holds 0xfff otherwise.
 */
/* vt_7: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_7_OFFSET                              0x1001cu /* established */
#define GBE_VT_7_BLANK_START_SHIFT                   12 /* established */
#define GBE_VT_7_BLANK_START_MASK                    0xfffu
#define GBE_VT_7_BLANK_START_INPLACE                 (GBE_VT_7_BLANK_START_MASK << GBE_VT_7_BLANK_START_SHIFT)
#define GBE_VT_7_LOW_SHIFT                           0 /* established */
#define GBE_VT_7_LOW_MASK                            0xfffu
#define GBE_VT_7_LOW_INPLACE                         (GBE_VT_7_LOW_MASK << GBE_VT_7_LOW_SHIFT)

/*
 * Flat-panel horizontal drive word. The value depends on the board's panel set-up byte and on whether a dual-channel option is fitted. The table of values is in docs/gbe.md.
 */
/* fp_hdrv: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_FP_HDRV_OFFSET                           0x10028u /* established */
#define GBE_FP_HDRV_HI_SHIFT                         12 /* established */
#define GBE_FP_HDRV_HI_MASK                          0xfffu
#define GBE_FP_HDRV_HI_INPLACE                       (GBE_FP_HDRV_HI_MASK << GBE_FP_HDRV_HI_SHIFT)
#define GBE_FP_HDRV_LO_SHIFT                         0 /* established */
#define GBE_FP_HDRV_LO_MASK                          0xfffu
#define GBE_FP_HDRV_LO_INPLACE                       (GBE_FP_HDRV_LO_MASK << GBE_FP_HDRV_LO_SHIFT)

/*
 * Flat-panel vertical drive word. The value depends on the panel set-up byte and on the option, as the table in docs/gbe.md gives.
 */
/* fp_vdrv: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_FP_VDRV_OFFSET                           0x1002cu /* established */
#define GBE_FP_VDRV_HI_SHIFT                         12 /* established */
#define GBE_FP_VDRV_HI_MASK                          0xfffu
#define GBE_FP_VDRV_HI_INPLACE                       (GBE_FP_VDRV_HI_MASK << GBE_FP_VDRV_HI_SHIFT)
#define GBE_FP_VDRV_LO_SHIFT                         0 /* established */
#define GBE_FP_VDRV_LO_MASK                          0xfffu
#define GBE_FP_VDRV_LO_INPLACE                       (GBE_FP_VDRV_LO_MASK << GBE_FP_VDRV_LO_SHIFT)

/*
 * Flat-panel data-enable word. The timing routine writes it only for the default set-up and panel set-ups 130 and 131, and only when no dual-channel option is fitted. It is not written otherwise.
 */
/* fp_de: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_FP_DE_OFFSET                             0x10030u /* established */
#define GBE_FP_DE_HI_SHIFT                           12 /* established */
#define GBE_FP_DE_HI_MASK                            0xfffu
#define GBE_FP_DE_HI_INPLACE                         (GBE_FP_DE_HI_MASK << GBE_FP_DE_HI_SHIFT)
#define GBE_FP_DE_LO_SHIFT                           0 /* established */
#define GBE_FP_DE_LO_MASK                            0xfffu
#define GBE_FP_DE_LO_INPLACE                         (GBE_FP_DE_LO_MASK << GBE_FP_DE_LO_SHIFT)

/*
 * Parked holding 0xffffff before the frame buffer is set up and written last in the start-up sequence. The final value is ((active width + v - 2) mod total pixels) in the low field and v in the high field, where v is the horizontal blank end minus 0x13, with total pixels added first when that is negative.
 */
/* vt_13: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_13_OFFSET                             0x10034u /* established */

/*
 * Parked holding 0xffffff and written last in the start-up sequence with the vertical blank start in the low field and the vertical blank end in the high field.
 */
/* vt_14: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_14_OFFSET                             0x10038u /* established */

/*
 * Horizontal blank window, in pixels.
 */
/* vt_15: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_15_OFFSET                             0x1003cu /* established */
#define GBE_VT_15_HBLANK_START_SHIFT                 12 /* established */
#define GBE_VT_15_HBLANK_START_MASK                  0xfffu
#define GBE_VT_15_HBLANK_START_INPLACE               (GBE_VT_15_HBLANK_START_MASK << GBE_VT_15_HBLANK_START_SHIFT)
#define GBE_VT_15_HBLANK_END_SHIFT                   0 /* established */
#define GBE_VT_15_HBLANK_END_MASK                    0xfffu
#define GBE_VT_15_HBLANK_END_INPLACE                 (GBE_VT_15_HBLANK_END_MASK << GBE_VT_15_HBLANK_END_SHIFT)

/*
 * Vertical blank window, in lines.
 */
/* vt_16: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_16_OFFSET                             0x10040u /* established */
#define GBE_VT_16_VBLANK_START_SHIFT                 12 /* established */
#define GBE_VT_16_VBLANK_START_MASK                  0xfffu
#define GBE_VT_16_VBLANK_START_INPLACE               (GBE_VT_16_VBLANK_START_MASK << GBE_VT_16_VBLANK_START_SHIFT)
#define GBE_VT_16_VBLANK_END_SHIFT                   0 /* established */
#define GBE_VT_16_VBLANK_END_MASK                    0xfffu
#define GBE_VT_16_VBLANK_END_INPLACE                 (GBE_VT_16_VBLANK_END_MASK << GBE_VT_16_VBLANK_END_SHIFT)

/*
 * Two parts, with bit placement not stated in the reading. The vertical part is minus (vertical blank start minus vertical blank end minus 1) when that difference is positive, else the difference, masked to 12 bits. The horizontal part is the horizontal blank end minus 20, with total pixels added when the blank end is below 20.
 */
/* vt_17: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_17_OFFSET                             0x10044u /* established */

/*
 * The vertical part of VT_17 and the horizontal blank end minus 54, with total pixels added when that is negative, then 0x1000 added for one line. The reading gives the 54 as a kernel constant. The 0x1000 step places the line part in bits 23:12 (inferred), so the horizontal part is in bits 11:0 (inferred).
 */
/* vt_18: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_18_OFFSET                             0x10048u /* established */

/*
 * The vertical part of VT_17 without the extra line that VT_18 adds, and the horizontal blank end minus 4. Bit placement is inferred as for VT_18.
 */
/* vt_19: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_VT_19_OFFSET                             0x1004cu /* established */

/*
 * Live readback of the overlay control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the overlay control.
 */
/* ovr_readback: r32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_OVR_READBACK_OFFSET                      0x20004u /* established */

/*
 * Overlay control. The stop routine clears its enable bit and leaves the word otherwise unwritten.
 */
/* ovr_control: rw32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_OVR_CONTROL_OFFSET                       0x20008u /* established */
#define GBE_OVR_CONTROL_ENABLE_SHIFT                 0 /* established */
#define GBE_OVR_CONTROL_ENABLE_MASK                  0x1u
#define GBE_OVR_CONTROL_ENABLE_INPLACE               (GBE_OVR_CONTROL_ENABLE_MASK << GBE_OVR_CONTROL_ENABLE_SHIFT)

/*
 * Live readback of the frame control. The start-up sequence polls bit 0 until it equals bit 0 of the value it has just written to the frame control.
 */
/* frm_readback: r32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_FRM_READBACK_OFFSET                      0x30008u /* established */

/*
 * Frame-buffer scan-out control. The stop routine saves it, clears its enable bit, and later writes it back with its pointer field bumped by one unit. The pointer field starts at bit 5 (inferred).
 */
/* frm_control: rw32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_FRM_CONTROL_OFFSET                       0x3000cu /* established */
#define GBE_FRM_CONTROL_ENABLE_SHIFT                 0 /* established */
#define GBE_FRM_CONTROL_ENABLE_MASK                  0x1u
#define GBE_FRM_CONTROL_ENABLE_INPLACE               (GBE_FRM_CONTROL_ENABLE_MASK << GBE_FRM_CONTROL_ENABLE_SHIFT)

/*
 * Display-ID table control. The stop routine clears its enable bit and leaves the word otherwise unwritten.
 */
/* did_control: rw32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_DID_CONTROL_OFFSET                       0x40004u /* established */
#define GBE_DID_CONTROL_ENABLE_SHIFT                 16 /* established */
#define GBE_DID_CONTROL_ENABLE_MASK                  0x1u
#define GBE_DID_CONTROL_ENABLE_INPLACE               (GBE_DID_CONTROL_ENABLE_MASK << GBE_DID_CONTROL_ENABLE_SHIFT)
#define GBE_DID_CONTROL_TABLE_HI_SHIFT               0 /* inferred */
#define GBE_DID_CONTROL_TABLE_HI_MASK                0xffffu
#define GBE_DID_CONTROL_TABLE_HI_INPLACE             (GBE_DID_CONTROL_TABLE_HI_MASK << GBE_DID_CONTROL_TABLE_HI_SHIFT)

/*
 * Window-ID table, one 32-bit word per window index. Indices of 32 or more are rejected by the kernel. The kernel writes entry 0 with 3 at text-port set-up.
 */
/* wid: w32, reset unknown; shipped IRIX X display module, per docs/gbe.md */
#define GBE_WID_OFFSET                               0x48000u /* established */
#define GBE_WID_COUNT                                32
#define GBE_WID_STRIDE                               4
#define GBE_WID_AUX_SHIFT                            11 /* inferred */
#define GBE_WID_AUX_MASK                             0x3u
#define GBE_WID_AUX_INPLACE                          (GBE_WID_AUX_MASK << GBE_WID_AUX_SHIFT)
#define GBE_WID_GAMMA_DISABLE_SHIFT                  10 /* established */
#define GBE_WID_GAMMA_DISABLE_MASK                   0x1u
#define GBE_WID_GAMMA_DISABLE_INPLACE                (GBE_WID_GAMMA_DISABLE_MASK << GBE_WID_GAMMA_DISABLE_SHIFT)
#define GBE_WID_MAP_GROUP_SHIFT                      5 /* inferred */
#define GBE_WID_MAP_GROUP_MASK                       0x1fu
#define GBE_WID_MAP_GROUP_INPLACE                    (GBE_WID_MAP_GROUP_MASK << GBE_WID_MAP_GROUP_SHIFT)
#define GBE_WID_TYPE_SHIFT                           2 /* inferred */
#define GBE_WID_TYPE_MASK                            0x7u
#define GBE_WID_TYPE_INPLACE                         (GBE_WID_TYPE_MASK << GBE_WID_TYPE_SHIFT)
#define GBE_WID_BUF_SHIFT                            0 /* inferred */
#define GBE_WID_BUF_MASK                             0x3u
#define GBE_WID_BUF_INPLACE                          (GBE_WID_BUF_MASK << GBE_WID_BUF_SHIFT)

/*
 * Hardware cursor position word. The text port writes 0 at set-up.
 */
/* cursor_pos: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_CURSOR_POS_OFFSET                        0x70000u /* established */

/*
 * Cursor control word. The text port writes 1 after the glyph and colours are loaded. What the bits mean beyond that is unknown.
 */
/* cursor_ctrl: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_CURSOR_CTRL_OFFSET                       0x70004u /* established */

/*
 * Cursor colour 1 in the byte layout red, green, blue, zero. The text port writes 0xffffff00.
 */
/* cursor_color1: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_CURSOR_COLOR1_OFFSET                     0x70008u /* established */

/*
 * Cursor colour 2 in the byte layout red, green, blue, zero. The text port writes 0xff000000.
 */
/* cursor_color2: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_CURSOR_COLOR2_OFFSET                     0x7000cu /* established */

/*
 * Cursor bitmap, 64 words. The text port loads words 0 to 31 from a 32-word table and writes 0 to words 32 to 63. Reading the words as a 32 by 32 bitmap at two bits per pixel is inferred; the text port draws with the first 32 words only (inferred).
 */
/* cursor_glyph: w32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_CURSOR_GLYPH_OFFSET                      0x78000u /* established */
#define GBE_CURSOR_GLYPH_COUNT                       64
#define GBE_CURSOR_GLYPH_STRIDE                      4

/*
 * Frame entry of a display-ID table: one per horizontal band of lines. The X module packs it as block << 19 | offset << 11 | yend.
 */
/* word DID_FRAME: 32 bits; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_DID_FRAME_WIDTH                          32 /* established */
#define GBE_DID_FRAME_BLOCK_SHIFT                    19 /* established */
#define GBE_DID_FRAME_BLOCK_MASK                     0x7fu
#define GBE_DID_FRAME_BLOCK_INPLACE                  (GBE_DID_FRAME_BLOCK_MASK << GBE_DID_FRAME_BLOCK_SHIFT)
#define GBE_DID_FRAME_OFFSET_SHIFT                   11 /* established */
#define GBE_DID_FRAME_OFFSET_MASK                    0xffu
#define GBE_DID_FRAME_OFFSET_INPLACE                 (GBE_DID_FRAME_OFFSET_MASK << GBE_DID_FRAME_OFFSET_SHIFT)
#define GBE_DID_FRAME_YEND_SHIFT                     0 /* established */
#define GBE_DID_FRAME_YEND_MASK                      0x7ffu
#define GBE_DID_FRAME_YEND_INPLACE                   (GBE_DID_FRAME_YEND_MASK << GBE_DID_FRAME_YEND_SHIFT)

/*
 * Line-table entry of a display-ID table: one per rectangle in a band, sorted by its last column. The X module packs it as index << 11 | xend.
 */
/* word DID_LINE: 16 bits; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_DID_LINE_WIDTH                           16 /* established */
#define GBE_DID_LINE_DID_SHIFT                       11 /* established */
#define GBE_DID_LINE_DID_MASK                        0x1fu
#define GBE_DID_LINE_DID_INPLACE                     (GBE_DID_LINE_DID_MASK << GBE_DID_LINE_DID_SHIFT)
#define GBE_DID_LINE_XEND_SHIFT                      0 /* established */
#define GBE_DID_LINE_XEND_MASK                       0x7ffu
#define GBE_DID_LINE_XEND_INPLACE                    (GBE_DID_LINE_XEND_MASK << GBE_DID_LINE_XEND_SHIFT)

#define GBE_BOARD_INTERLACE_OFF                      0x2du /* established */
#define GBE_BOARD_PANEL_OFF                          0x2eu /* established */
#define GBE_CMAP_ENTRIES_REV11                       0x1200u /* inferred */
#define GBE_CMAP_ENTRY_BASE                          0x50000u /* established */
#define GBE_CMFIFO_BATCH                             0x21u /* established */
#define GBE_CMFIFO_FREE_MIN                          0x30u /* established */
#define GBE_CMFIFO_MASK                              0x3fu /* established */
#define GBE_DEVICE_ID_VALUE                          0x666u /* established */
#define GBE_DID_BLOCK_BYTES                          0x200u /* established */
#define GBE_DID_BLOCK_ENTRIES                        0x100u /* established */
#define GBE_DID_END_BLOCK                            0x80u /* established */
#define GBE_DID_LAST_XEND                            0x7ffu /* established */
#define GBE_DID_LINE_TABLE_OFFSET                    0x2000u /* established */
#define GBE_DID_TABLE_BYTES                          0x10000u /* established */
#define GBE_DOTCLK_RUN_BIT                           0x14u /* inferred */
#define GBE_DOTCLK_SETTLE_MS                         0xbu /* established */
#define GBE_FIELD_MASK                               0xfffu /* established */
#define GBE_FIELD_SHIFT                              0xcu /* established */
#define GBE_GBE_LINE_IRQ_BASE                        0x10u /* established */
#define GBE_OPT_CMD_CLOCK_KHZ                        0x186a0u /* established */
#define GBE_OPT_CMD_ID_HIGH                          0xffu /* established */
#define GBE_OPT_CMD_ID_LOW                           0x7fu /* established */
#define GBE_OPT_DELAY_SMALL                          0x2u /* established */
#define GBE_OPT_ID_MASK                              0x60u /* established */
#define GBE_OPT_ID_RESTORE                           0x70u /* established */
#define GBE_OPT_INSTALLED_LIMIT                      0x9u /* established */
#define GBE_OPT_PORT_ADDR                            0x7eu /* established */
#define GBE_OVERLAY_TRANSPARENT_INDEX                0x0u /* inferred */
#define GBE_PANEL_SETUP_A                            0x82u /* established */
#define GBE_PANEL_SETUP_B                            0x83u /* established */
#define GBE_PANEL_SETUP_C                            0x84u /* established */
#define GBE_REF_CLOCK_KHZ                            0x4e20u /* inferred */
#define GBE_TILE_LIST_ALIGN_BYTES                    0x200u /* inferred */
#define GBE_TILE_LIST_OUTSTANDING                    0x8u /* inferred */
#define GBE_TILE_LIST_PAGE_BYTES                     0x2000u /* inferred */
#define GBE_TILE_NUMBER_MASK                         0x1fffu /* established */
#define GBE_TWO_WIRE_FP_OFF                          0x10u /* established */
#define GBE_TWO_WIRE_MON_OFF                         0x8u /* established */
#define GBE_TWO_WIRE_WAIT_BIT                        0x1u /* established */
#define GBE_TWO_WIRE_WAIT_LIMIT                      0x3cu /* established */
#define GBE_VT_XY_FREEZE                             0x80000000u /* established */

#endif /* GBE_GEN_H */
