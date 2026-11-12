/* generated from defs/gbe.toml by defs-gen - do not edit, change the source */
/* O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe */
#ifndef GBE_GEN_H
#define GBE_GEN_H

#include <stdint.h>


/*
 * Identity word the kernel reads first. It accepts the block only when the word reads 0x666; any other value means the kernel leaves the block untouched.
 */
/* device_id: r32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_DEVICE_ID_OFFSET                         0x14u /* established */

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
 * Overlay control. The stop routine clears its enable bit and leaves the word otherwise unwritten.
 */
/* ovr_control: rw32, reset unknown; shipped IRIX kernel binaries, per docs/gbe.md */
#define GBE_OVR_CONTROL_OFFSET                       0x20008u /* established */
#define GBE_OVR_CONTROL_ENABLE_SHIFT                 0 /* established */
#define GBE_OVR_CONTROL_ENABLE_MASK                  0x1u
#define GBE_OVR_CONTROL_ENABLE_INPLACE               (GBE_OVR_CONTROL_ENABLE_MASK << GBE_OVR_CONTROL_ENABLE_SHIFT)

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

/*
 * Window-ID table, one 32-bit word per window index. Indices of 32 or more are rejected by the kernel. The kernel writes entry 0 with 3 at text-port set-up.
 */
/* wid: w32, reset unknown; shipped IRIX X display module, per docs/gbe.md */
#define GBE_WID_OFFSET                               0x48000u /* established */
#define GBE_WID_COUNT                                32
#define GBE_WID_STRIDE                               4
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

#define GBE_BOARD_INTERLACE_OFF                      0x2du /* established */
#define GBE_BOARD_PANEL_OFF                          0x2eu /* established */
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
#define GBE_PANEL_SETUP_A                            0x82u /* established */
#define GBE_PANEL_SETUP_B                            0x83u /* established */
#define GBE_PANEL_SETUP_C                            0x84u /* established */
#define GBE_REF_CLOCK_KHZ                            0x4e20u /* inferred */
#define GBE_TILE_NUMBER_MASK                         0x1fffu /* established */
#define GBE_TWO_WIRE_FP_OFF                          0x10u /* established */
#define GBE_TWO_WIRE_MON_OFF                         0x8u /* established */
#define GBE_TWO_WIRE_WAIT_BIT                        0x1u /* established */
#define GBE_TWO_WIRE_WAIT_LIMIT                      0x3cu /* established */
#define GBE_VT_XY_FREEZE                             0x80000000u /* established */

#endif /* GBE_GEN_H */
