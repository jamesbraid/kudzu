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
#define NIC_HUB_ACCESS_OFFSET 0xe04030u /* established: offset of the register from the hub's local-window base */

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

/* The Ethernet-address part is recognised from the first bytes of a 14-byte
 * record read from it. The record's byte 0 and byte 1 must be these two values,
 * and a CRC-16 folded over bytes 1 to 13 must come out as the residue below. The
 * six address bytes are record bytes 11 down to 6. */
#define NIC_MAC_RECORD_LEN 14 /* established: bytes read from the part */
#define NIC_MAC_ID_BYTE0 0x8du /* established: required value of record byte 0 */
#define NIC_MAC_ID_BYTE1 0x0au /* established: required value of record byte 1 */
#define NIC_MAC_CRC16_RESIDUE 0xb001u /* established: CRC-16 residue over record bytes 1 to 13 */

/* GPCR_S: the I/O controller's general-purpose control word, in its set
 * alias. Before reading the address part the routine stores 0x200000 to the
 * register address its caller passes, when that address is not null; with a
 * null address it prints that it is bypassing the enable and goes on. The word
 * is written whole, so bit 21 is the only bit the routine sets. That the store
 * enables the one-wire path to the part is read from the routine's own message,
 * and that a set alias leaves other bits alone is inferred from its name. */
/* FIELD: GPCR_S.nic_enable 21:21 inferred */
#define NIC_GPCR_S_NIC_ENABLE_SHIFT 21
#define NIC_GPCR_S_NIC_ENABLE_MASK  0x1u
#define NIC_GPCR_S_NIC_ENABLE_WORD 0x200000u /* established: the value stored */

#endif
