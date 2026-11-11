/*
 * PCIBR_DECODE.h - PCI bridge widget identification and window layouts
 *
 * Publish-safe contract for how the shipped Origin 300 kernel tells the PCI
 * bridge widgets apart and where it finds a bridge's configuration space and
 * interrupt address registers. The bridge registers are reached through a
 * register window; every offset is relative to that window.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   FIELD: <WORD>.<field> <hi>:<lo> <grade>             a field of a 32-bit word
 *   ENUM:  <target> <value> <label> <grade>             target is a FIELD key
 *
 * The widget identification word is the 32-bit word at byte 4 of the window.
 * View: that is the low half of the 8-byte register slot at byte 0. The
 * machine is big-endian, so a 32-bit load at +4 reads bits 31:0 of the 64-bit
 * register at 0. A register list that counts slots shows it as a 4-byte access
 * at 0x0; it is the same word. The chip registers of the 32-bit layout are all
 * reached this way, each one slot apart, and only the two translation arrays
 * are used as doublewords.
 * The kernel selects one of three register layouts from the part number and
 * the 11-bit field together: a 32-bit layout with eight device slots and two
 * 64-bit layouts with four. Any other pair makes the register accessors panic.
 * The names of the layouts are positions, not product names.
 *
 * Derivation. Every value comes from reading the bridge register accessors and
 * the bridge driver registration routine of the shipped kernel. The kernel
 * never names the 11-bit field; only the values that accompany each part
 * number are read. The public Linux crosstalk widget header (v5.3) puts a
 * manufacturer number in exactly bits 11:1 and gives 36 to the later bridge
 * and 54 to the first one, which matches the pairs, so the field is the
 * manufacturer number (inferred). Another widget uses the same field with
 * another value (the Odyssey graphics board is 35 in the sibling decode
 * header). Whether the same pairs are what the widgets themselves report is
 * unknown here.
 */
#ifndef PCIBR_DECODE_H
#define PCIBR_DECODE_H

#include <stdint.h>

#define PCIBR_WID_OFFSET 4 /* established: byte offset of the widget identification word in the window */

/* A pointer is the register window itself, not the driver's soft state, when all three of these bits are set */
#define PCIBR_WINDOW_BIT_A 63 /* established */
#define PCIBR_WINDOW_BIT_B 60 /* established */
#define PCIBR_WINDOW_BIT_C 57 /* established */

/* WID: the widget identification word */
/* part: part number */
/* FIELD: WID.part 27:12 established */
#define PCIBR_WID_PART_SHIFT 12
#define PCIBR_WID_PART_MASK  0xffffu
/* ENUM: WID.part 0xc002 layout32 established */
#define PCIBR_WID_PART_LAYOUT32 0xc002u
/* ENUM: WID.part 0xd002 layout32b established */
#define PCIBR_WID_PART_LAYOUT32B 0xd002u
/* ENUM: WID.part 0xd102 layout64b established */
#define PCIBR_WID_PART_LAYOUT64B 0xd102u
/* ENUM: WID.part 0xd112 layout64bx established */
#define PCIBR_WID_PART_LAYOUT64BX 0xd112u
/* ENUM: WID.part 0xe000 layout64a established */
#define PCIBR_WID_PART_LAYOUT64A 0xe000u
/* ENUM: WID.part 0xe010 layout64ax established */
#define PCIBR_WID_PART_LAYOUT64AX 0xe010u
/* field11: manufacturer number, bits 11:1 of the identification word; inferred from the public Linux widget header, the kernel does not name it */
/* FIELD: WID.field11 11:1 established */
#define PCIBR_WID_FIELD11_SHIFT 1
#define PCIBR_WID_FIELD11_MASK  0x7ffu
/* ENUM: WID.field11 36 value36 established */
#define PCIBR_WID_FIELD11_VALUE36 36
/* ENUM: WID.field11 54 value54 established */
#define PCIBR_WID_FIELD11_VALUE54 54

/* Pairs the kernel accepts: part 0xe000 or 0xe010 with 36 (first 64-bit layout), 0xd102 or 0xd112 with 36 (second 64-bit layout), 0xd002 with 36 or 0xc002 with 54 (32-bit layout). */

#define PCIBR_CFG_SLOT_SHIFT 12 /* established: configuration address is window plus slot shifted left by this */
#define PCIBR_CFG_FUNC_SHIFT 8 /* established: function shifted left by this */
#define PCIBR_CFG_WINDOW32_OFFSET 0x20000u /* established: added on the 32-bit layout only, with the register offset first rounded down to a multiple of four */
#define PCIBR_CFG_SLOTS_32 8 /* established: slots 0 to 7 on the 32-bit layout */
#define PCIBR_CFG_SLOT_BIAS_64 1 /* established: on the 64-bit layouts the slot is incremented before use; the accepted range is then 1 to 4 */

#define PCIBR_INTR_COUNT 8 /* established: interrupt numbers 0 to 7 */
#define PCIBR_INTR_STRIDE 8 /* established: byte stride of the interrupt address registers on every layout */
#define PCIBR_INTR_ADDR64_OFFSET 304 /* established: first interrupt address register, 64-bit layouts; its low 48 bits hold the address */
#define PCIBR_INTR_ADDR32_OFFSET 308 /* established: first interrupt address register, 32-bit layout; a 32-bit word */
/* View: both are the same slot of eight bytes at 304 + 8n. The 64-bit layouts
 * use the doubleword at the slot; the 32-bit layout uses its low half at +4,
 * which a slot-counting register list shows as a 4-byte access at 304. */
#define PCIBR_INTR_ADDR32_HI 17 /* established: the 32-bit layout replaces bits 17:8 of the word */
#define PCIBR_INTR_ADDR32_LO 8 /* established */
#define PCIBR_INTR_ADDR32_SRC_HI 47 /* established: the 32-bit layout keeps source address bits 47:38 */
#define PCIBR_INTR_ADDR32_SRC_LO 38 /* established */

#endif
