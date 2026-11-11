/*
 * KLCONFIG_DECODE.h - hardware configuration board record
 *
 * Publish-safe contract for the board record the PROM leaves in each node's
 * configuration area and the kernel walks to find boards, slots, modules and
 * components. Records are in big-endian memory. Only the fields the shipped
 * kernels read are described; every other byte of the record is unknown here.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   FIELD: <WORD>.<field> <hi>:<lo> <grade>             a field of a 32-bit word
 *
 * A word is named BRD_W<n>, n being the byte offset divided by four from the
 * start of the board record. A byte at offset o is bits 31-8*(o%4) down to
 * 24-8*(o%4) of its word. The component offset array is not a field: it
 * appears only as an _OFF_ and _STRIDE_ constant pair.
 *
 * Derivation. Offsets and widths come from reading the board-list search
 * routines of the shipped kernels for two platforms. The field names are the
 * ones the public Linux port of the same platform uses and are labels only.
 */
#ifndef KLCONFIG_DECODE_H
#define KLCONFIG_DECODE_H

#include <stdint.h>

#define KLCONFIG_BRD_OFF_COMPTS 36 /* established: first component offset, one 32-bit word each */
#define KLCONFIG_BRD_STRIDE_COMPTS 4 /* established */

/* BRD_W0: the 32-bit big-endian word at byte 0 */
/* next: offset of the next board on the same node; zero ends the list */
/* FIELD: BRD_W0.next 31:0 established */
#define KLCONFIG_BRD_W0_NEXT_SHIFT 0
#define KLCONFIG_BRD_W0_NEXT_MASK  0xffffffffu

/* BRD_W1: the 32-bit big-endian word at byte 4 */
/* class: high four bits of the board type byte (byte 5) */
/* FIELD: BRD_W1.class 23:20 established */
#define KLCONFIG_BRD_W1_CLASS_SHIFT 20
#define KLCONFIG_BRD_W1_CLASS_MASK  0xfu
/* type_low: low four bits of the board type byte; the routines compare only the class */
/* FIELD: BRD_W1.type_low 19:16 inferred */
#define KLCONFIG_BRD_W1_TYPE_LOW_SHIFT 16
#define KLCONFIG_BRD_W1_TYPE_LOW_MASK  0xfu

/* BRD_W2: the 32-bit big-endian word at byte 8 */
/* flags_master_baseio: flags byte (byte 9) bit 5, marks the global master I/O board */
/* FIELD: BRD_W2.flags_master_baseio 21:21 established */
#define KLCONFIG_BRD_W2_FLAGS_MASTER_BASEIO_SHIFT 21
#define KLCONFIG_BRD_W2_FLAGS_MASTER_BASEIO_MASK  0x1u
/* slot: slot number, byte 10 */
/* FIELD: BRD_W2.slot 15:8 established */
#define KLCONFIG_BRD_W2_SLOT_SHIFT 8
#define KLCONFIG_BRD_W2_SLOT_MASK  0xffu

/* BRD_W3: the 32-bit big-endian word at byte 12 */
/* module: signed 16-bit module number, bytes 14 to 15 */
/* FIELD: BRD_W3.module 15:0 established */
#define KLCONFIG_BRD_W3_MODULE_SHIFT 0
#define KLCONFIG_BRD_W3_MODULE_MASK  0xffffu

/* BRD_W5: the 32-bit big-endian word at byte 20 */
/* numcompts: number of component offsets in use, byte 23 */
/* FIELD: BRD_W5.numcompts 7:0 established */
#define KLCONFIG_BRD_W5_NUMCOMPTS_SHIFT 0
#define KLCONFIG_BRD_W5_NUMCOMPTS_MASK  0xffu

/* BRD_W8: the 32-bit big-endian word at byte 32 */
/* nasid: signed 16-bit node number the PROM recorded, bytes 32 to 33 */
/* FIELD: BRD_W8.nasid 31:16 established */
#define KLCONFIG_BRD_W8_NASID_SHIFT 16
#define KLCONFIG_BRD_W8_NASID_MASK  0xffffu

#endif
