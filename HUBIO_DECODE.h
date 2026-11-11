/*
 * HUBIO_DECODE.h - Origin 300 hub crosstalk-side register window
 *
 * Publish-safe contract for the one register of the hub's crosstalk-side
 * window that the kernel's error path tests before it goes on to the widget
 * error handler. Register offsets are relative to the window. A node's copy is
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
 * unknown, so the field names are positions, not roles.
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

#endif
