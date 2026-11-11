/*
 * NIC_DECODE.h - bit-serial access to the one-wire identity parts
 *
 * Publish-safe contract for how the shipped kernels drive the one-wire part on
 * a board through the microlan control register of an I/O controller, and
 * where the node interface reaches the same bus on the Origin 300 hub.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   FIELD: <WORD>.<field> <hi>:<lo> <grade>             a field of a 32-bit word
 *
 * The control register is one 32-bit word. The routine writes it once with
 * the pulse and sample bits, then reads it back until the done bit sets, with no
 * limit. The bit it returns is the result bit of the last read. The register's own
 * offset inside the controller is not described here.
 *
 * Derivation. Every field comes from reading the access routine of the
 * shipped kernel object for the platform. The hub access address comes from
 * reading the Origin 300 access routine. No device document was available,
 * so the bit names are roles read from the code and the pulse timing is not
 * described.
 */
#ifndef NIC_DECODE_H
#define NIC_DECODE_H

#include <stdint.h>

/* The Origin 300 hub reaches the bus through one register. Its physical
 * address is the low part below plus the node number shifted left by the
 * node shift. */
#define NIC_HUB_ACCESS_PHYS_LOW 0x1e04030u /* established: physical address of the register for node 0 */
#define NIC_HUB_ACCESS_NODE_SHIFT 33 /* established: node number shift */

/* MCR: the 32-bit microlan control word */
/* pulse: written, starts a bus pulse */
/* FIELD: MCR.pulse 10:10 established */
#define NIC_MCR_PULSE_SHIFT 10
#define NIC_MCR_PULSE_MASK  0x1u
/* sample: written, asks the controller to sample the bus */
/* FIELD: MCR.sample 2:2 established */
#define NIC_MCR_SAMPLE_SHIFT 2
#define NIC_MCR_SAMPLE_MASK  0x1u
/* done: read, the routine loops until it is set; the delay after it is the caller's timing */
/* FIELD: MCR.done 1:1 established */
#define NIC_MCR_DONE_SHIFT 1
#define NIC_MCR_DONE_MASK  0x1u
/* result: read, the bus level the routine returns from the last read, taken after done is set */
/* FIELD: MCR.result 0:0 established */
#define NIC_MCR_RESULT_SHIFT 0
#define NIC_MCR_RESULT_MASK  0x1u

#endif
