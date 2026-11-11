/*
 * HUBNI_DECODE.h - node hub network interface registers
 *
 * Publish-safe contract for the network-interface words of a node hub that the
 * kernels read for the node number, the link state and the link reset. Every
 * offset is relative to the hub's local window and the same word is reached on
 * another node through that node's window. All words are 64-bit.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   FIELD: <WORD>.<field> <hi>:<lo> <grade>             a field of a 32-bit word
 *
 * Offsets are plain constants because the register overlap check treats every
 * header as one address space and the two hub generations place their network
 * interface blocks at different bases. The generator reads only the low 32
 * bits of a field, so the node-number field, which sits in the low half, is a
 * word and the 64-bit error word's latched-error range is a mask constant.
 *
 * Derivation. Offsets, widths and bit ranges come from reading the node-number,
 * link-check, link-reset and error-report routines of the shipped kernels. The
 * Origin 200/2000 offset is the node-number register only. Register roles are
 * read from use; the datasheet names for the Origin 300 words are not
 * restated here. What the status bits read on a surviving node while the far
 * node resets its link is unknown.
 */
#ifndef HUBNI_DECODE_H
#define HUBNI_DECODE_H

#include <stdint.h>

#define HUBNI_O200_STATUS_OFFSET 0x600000u /* established: Origin 200/2000 network status word, carries the node number */
#define HUBNI_O300_STATUS_OFFSET 0x680000u /* established: Origin 300 network status word; bits 1:0 are tested as link state, bits 2:1 equal to 2 starts a link-reset wait */
#define HUBNI_O300_PORT_RESET_OFFSET 0x680008u /* established: Origin 300 port reset word; the kernel writes the value 6 */
#define HUBNI_O300_RESET_ENABLE_OFFSET 0x680010u /* established: Origin 300 reset enable word; the kernel writes 1; bit 0 is the one that matters (datasheet "reset ok" is inferred) */
#define HUBNI_O300_ERROR_OFFSET 0x680100u /* established: Origin 300 port error word; bit 52 set with the enable word zero is treated as a tolerated link reset */
#define HUBNI_O300_ERROR_CLEAR_OFFSET 0x680120u /* established: Origin 300 port error clear word; the kernel reads it, then stores zero to the error word; whether the read clears anything is unknown */

#define HUBNI_O300_ERROR_LATCHED_HI 52 /* established: top bit of the range of latched error bits the kernel tests */
#define HUBNI_O300_ERROR_LATCHED_LO 24 /* established: bottom bit of that range */
#define HUBNI_O300_ERROR_LINK_RESET_BIT 52 /* established: link reset caused by another partition, with the enable word zero; that the chip latches it when a reset is refused is unknown */

/* NSRI: the low 32 bits of the Origin 200/2000 network status word */
/* nodeid: node number, the kernel reads it to find its own node; the 9-bit field is read in full */
/* FIELD: NSRI.nodeid 16:8 established */
#define HUBNI_NSRI_NODEID_SHIFT 8
#define HUBNI_NSRI_NODEID_MASK  0x1ffu

#endif
