/*
 * HUBIO_DECODE.h - Origin 300 hub crosstalk-side register window
 *
 * Publish-safe contract for two parts of the hub's crosstalk-side window: the
 * one register the kernel's error path tests before it goes on to the widget
 * error handler, and the table of request-buffer entries its I/O error scan
 * reads. Register offsets are relative to the window. A node's copy is
 * reached at the window base plus the node number shifted left by the node
 * shift.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   REG:   <BLOCK>.<name> <offset> <acc><width> [count=<n>] reset=<v> <grade>
 *   FIELD: <BLOCK>.<name>.<field> <hi>:<lo> <grade>     a field of a register
 *
 * Derivation. The offset, width and the two tested bits come from reading the
 * hub error routine of the shipped Origin 300 kernel. The routine only tests
 * the bits and returns early when neither is set; what the register holds is
 * unknown, so the field names are positions, not roles. The request-buffer
 * entry words come from the shifts and masks of the scan routine that reads
 * them; a field is a position the routine tests, and the notes say what the
 * routine does with it. No field name here is a hardware name.
 */
#ifndef HUBIO_DECODE_H
#define HUBIO_DECODE_H

#include <stdint.h>

#define HUBIO_WINDOW_BASE 0x1c00000u /* established: physical base of the window for node 0 */
#define HUBIO_NODE_SHIFT 33 /* established: node number shift */

/* REG: HUBIO.flags 0x1e8 r64 reset=unknown established */
/* bit0: tested with bit 4; the error routine continues only when one of the two is set; role unknown */
/* FIELD: HUBIO.flags.bit0 0:0 established */
#define HUBIO_FLAGS_BIT0_SHIFT 0
#define HUBIO_FLAGS_BIT0_MASK  0x1u
/* bit4: see bit0; role unknown */
/* FIELD: HUBIO.flags.bit4 4:4 established */
#define HUBIO_FLAGS_BIT4_SHIFT 4
#define HUBIO_FLAGS_BIT4_MASK  0x1u

/*
 * Request-buffer table. Entry i (0 to 14) has three 64-bit words, crb_a, crb_b
 * and crb_c, at the register offsets below plus the stride times i; the scan
 * reads all fifteen entries in order.
 */
#define HUBIO_CRB_STRIDE 32 /* established: bytes between entries */
#define HUBIO_CRB_COUNT 15 /* established: entries the scan reads */

/* REG: HUBIO.crb_a 0x400 r64 reset=unknown established */
/* bit0: selects which of two forms of the error record the scan builds for a block-transfer error; role unknown */
/* FIELD: HUBIO.crb_a.bit0 0:0 established */
#define HUBIO_CRB_A_BIT0_SHIFT 0
#define HUBIO_CRB_A_BIT0_MASK  0x1u
/* bit49: reported as a crosstalk packet that arrived with its error bit set; tested only when bit55 is clear */
/* FIELD: HUBIO.crb_a.bit49 49:49 established */
#define HUBIO_CRB_A_BIT49_SHIFT 49
#define HUBIO_CRB_A_BIT49_MASK  0x1u
/* bit50: the scan skips the entry unless this bit is set */
/* FIELD: HUBIO.crb_a.bit50 50:50 established */
#define HUBIO_CRB_A_BIT50_SHIFT 50
#define HUBIO_CRB_A_BIT50_MASK  0x1u
/* bit51: reported as an uncorrectable data error on data from the network; tested only when bit55 is clear */
/* FIELD: HUBIO.crb_a.bit51 51:51 established */
#define HUBIO_CRB_A_BIT51_SHIFT 51
#define HUBIO_CRB_A_BIT51_MASK  0x1u
/* type: error type, 0 to 7; with bit55 set it picks one of eight handling cases */
/* FIELD: HUBIO.crb_a.type 54:52 established */
#define HUBIO_CRB_A_TYPE_SHIFT 52
#define HUBIO_CRB_A_TYPE_MASK  0x7u
/* bit55: with the bit set the type selects the handling case; with it clear bit49 and bit51 are tested */
/* FIELD: HUBIO.crb_a.bit55 55:55 established */
#define HUBIO_CRB_A_BIT55_SHIFT 55
#define HUBIO_CRB_A_BIT55_MASK  0x1u

/* REG: HUBIO.crb_b 0x408 r64 reset=unknown established */
/* kind: with the value 1 or 5 and sub 1 or 2, the entry is treated as a block-transfer-engine error; bit 32 alone also selects the engine when crb_c bit48 is clear */
/* FIELD: HUBIO.crb_b.kind 32:30 established */
#define HUBIO_CRB_B_KIND_SHIFT 30
#define HUBIO_CRB_B_KIND_MASK  0x7u
/* sub: see kind */
/* FIELD: HUBIO.crb_b.sub 42:41 established */
#define HUBIO_CRB_B_SUB_SHIFT 41
#define HUBIO_CRB_B_SUB_MASK  0x3u
/* engine_alt: selects the block-transfer engine when crb_c bit48 is set */
/* FIELD: HUBIO.crb_b.engine_alt 58:58 established */
#define HUBIO_CRB_B_ENGINE_ALT_SHIFT 58
#define HUBIO_CRB_B_ENGINE_ALT_MASK  0x1u

/* REG: HUBIO.crb_c 0x410 r64 reset=unknown established */
/* bit48: with the bit set the entry is treated as a block-transfer-engine error */
/* FIELD: HUBIO.crb_c.bit48 48:48 established */
#define HUBIO_CRB_C_BIT48_SHIFT 48
#define HUBIO_CRB_C_BIT48_MASK  0x1u

#endif
