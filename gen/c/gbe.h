/* generated from defs/gbe.toml by defs-gen - do not edit, change the source */
/* O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe */
#ifndef GBE_GEN_H
#define GBE_GEN_H

#include <stdint.h>


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

#define GBE_BOARD_INTERLACE_OFF                      0x2du /* established */
#define GBE_BOARD_PANEL_OFF                          0x2eu /* established */
#define GBE_FIELD_MASK                               0xfffu /* established */
#define GBE_FIELD_SHIFT                              0xcu /* established */
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
#define GBE_TWO_WIRE_FP_OFF                          0x10u /* established */
#define GBE_TWO_WIRE_MON_OFF                         0x8u /* established */
#define GBE_TWO_WIRE_WAIT_BIT                        0x1u /* established */
#define GBE_TWO_WIRE_WAIT_LIMIT                      0x3cu /* established */

#endif /* GBE_GEN_H */
