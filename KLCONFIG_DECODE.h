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
 *   ENUM: <target> <value> <label> <grade>
 *
 * A word is named <RECORD>_W<n>, n being the byte offset divided by four from
 * the start of that record: BRD for the board record, COMP for the header every
 * component record starts with, CONS for the console record, MEM for the memory
 * component and GDA for the global data area. A byte at offset o is bits
 * 31-8*(o%4) down to 24-8*(o%4) of its word. A doubleword is two words (name_hi,
 * name_lo). An array is not a field: it appears only as _OFF_, _STRIDE_ and
 * _COUNT_ constants.
 *
 * Derivation. Offsets and widths come from reading the board-list search
 * routines of the shipped kernels for two platforms. The field names are the
 * ones the public Linux port of the same platform uses and are labels only.
 * The component, console, memory and global-data records are described only at
 * the offsets the same routines read, in the shipped kernels for both platforms.
 */
#ifndef KLCONFIG_DECODE_H
#define KLCONFIG_DECODE_H

#include <stdint.h>

#define KLCONFIG_BRD_OFF_COMPTS 36 /* established: first component offset, one 32-bit word each */
#define KLCONFIG_BRD_STRIDE_COMPTS 4 /* established */

/*
 * Per-node directory. The board list is reached through a directory at a fixed
 * offset of each node's memory. The kernel reads the doubleword at byte 72 of
 * that directory (the second 64-byte entry, offset field 8 bytes in), masks it
 * to 40 bits, and adds the result to the node base to find the configuration
 * header; the board list offset is the word at byte 20 of that header (zero
 * means no boards). The 64-byte entry size is not read as a constant: it is
 * the sum 64 + 8 = 72 that the routines load.
 */
#define KLCONFIG_DIR_OFF 0x2000u /* established: directory offset from the node base */
#define KLCONFIG_DIR_OFF_CONFIG_OFFSET 72 /* established: byte of the doubleword that gives the configuration header offset */
#define KLCONFIG_DIR_CONFIG_OFFSET_MASK 0xffffffffffull /* established: 40 bits are kept */
#define KLCONFIG_HDR_OFF_BOARD_INFO 20 /* established: word giving the offset of the first board record */

/*
 * Board class and component. The serial-number routine of both platforms picks
 * the component to search from the class: node searches component type 2, I/O
 * type 5, router type 9 and graphics type 10 (established). Only the class value
 * and the component it selects are read from the binaries; the labels node, io,
 * router and graphics describe that component's role, and only node is tied to a
 * board type the kernels name (the node board, type 0x11, whose hub component is
 * type 2). Class 4 is not seen.
 */

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
/* ENUM: BRD_W1.class 1 node established */
#define KLCONFIG_BRD_W1_CLASS_NODE 1
/* ENUM: BRD_W1.class 2 io inferred */
#define KLCONFIG_BRD_W1_CLASS_IO 2
/* ENUM: BRD_W1.class 3 router inferred */
#define KLCONFIG_BRD_W1_CLASS_ROUTER 3
/* ENUM: BRD_W1.class 5 graphics inferred */
#define KLCONFIG_BRD_W1_CLASS_GRAPHICS 5
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

/* ---- component record header (every component record starts with it) ---- */

/* COMP_W0: the 32-bit big-endian word at byte 0 */
/* struct_type: component kind, byte 0, matched exactly by the component search */
/* FIELD: COMP_W0.struct_type 31:24 established */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_SHIFT 24
#define KLCONFIG_COMP_W0_STRUCT_TYPE_MASK  0xffu
/* ENUM: COMP_W0.struct_type 2 hub established */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_HUB 2
/* ENUM: COMP_W0.struct_type 3 memory inferred */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_MEMORY 3
/* ENUM: COMP_W0.struct_type 5 io inferred */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_IO 5
/* ENUM: COMP_W0.struct_type 9 router inferred */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_ROUTER 9
/* ENUM: COMP_W0.struct_type 10 graphics inferred */
#define KLCONFIG_COMP_W0_STRUCT_TYPE_GRAPHICS 10
/* flags_enabled: bit 0 of the flags byte (byte 2); set means the component is enabled */
/* FIELD: COMP_W0.flags_enabled 8:8 established */
#define KLCONFIG_COMP_W0_FLAGS_ENABLED_SHIFT 8
#define KLCONFIG_COMP_W0_FLAGS_ENABLED_MASK  0x1u

/* COMP_W6: the 32-bit big-endian word at byte 24 */
/* physid: physical identifier, byte 24; for a disabled hub it is returned as the node number */
/* FIELD: COMP_W6.physid 31:24 established */
#define KLCONFIG_COMP_W6_PHYSID_SHIFT 24
#define KLCONFIG_COMP_W6_PHYSID_MASK  0xffu

/* ---- console record ---- */

/* CONS_W2: the 32-bit big-endian word at byte 8 */
/* config_base_hi: doubleword at byte 8, passed as the first argument of the console reset call, bytes 8 to 11 */
/* FIELD: CONS_W2.config_base_hi 31:0 established */
#define KLCONFIG_CONS_W2_CONFIG_BASE_HI_SHIFT 0
#define KLCONFIG_CONS_W2_CONFIG_BASE_HI_MASK  0xffffffffu

/* CONS_W3: the 32-bit big-endian word at byte 12 */
/* config_base_lo: low word of the same doubleword, bytes 12 to 15 */
/* FIELD: CONS_W3.config_base_lo 31:0 established */
#define KLCONFIG_CONS_W3_CONFIG_BASE_LO_SHIFT 0
#define KLCONFIG_CONS_W3_CONFIG_BASE_LO_MASK  0xffffffffu

/* CONS_W4: the 32-bit big-endian word at byte 16 */
/* memory_base_hi: doubleword at byte 16, passed as the second argument of the console reset call, bytes 16 to 19 */
/* FIELD: CONS_W4.memory_base_hi 31:0 established */
#define KLCONFIG_CONS_W4_MEMORY_BASE_HI_SHIFT 0
#define KLCONFIG_CONS_W4_MEMORY_BASE_HI_MASK  0xffffffffu

/* CONS_W5: the 32-bit big-endian word at byte 20 */
/* memory_base_widget: bits 31:24 of the doubleword's low word (doubleword bits 31:24), taken as the console's widget number */
/* FIELD: CONS_W5.memory_base_widget 31:24 established */
#define KLCONFIG_CONS_W5_MEMORY_BASE_WIDGET_SHIFT 24
#define KLCONFIG_CONS_W5_MEMORY_BASE_WIDGET_MASK  0xffu
/* memory_base_rest: bits 23:0 of the same low word, not read separately */
/* FIELD: CONS_W5.memory_base_rest 23:0 established */
#define KLCONFIG_CONS_W5_MEMORY_BASE_REST_SHIFT 0
#define KLCONFIG_CONS_W5_MEMORY_BASE_REST_MASK  0xffffffu

/* CONS_W8: the 32-bit big-endian word at byte 32 */
/* nasid: signed 16-bit node number of the console, bytes 32 to 33 */
/* FIELD: CONS_W8.nasid 31:16 established */
#define KLCONFIG_CONS_W8_NASID_SHIFT 16
#define KLCONFIG_CONS_W8_NASID_MASK  0xffffu
/* npci: the console's PCI slot, byte 35 */
/* FIELD: CONS_W8.npci 7:0 established */
#define KLCONFIG_CONS_W8_NPCI_SHIFT 0
#define KLCONFIG_CONS_W8_NPCI_MASK  0xffu

/* ---- memory component (after the component header) ---- */
#define KLCONFIG_MEM_OFF_BNKSZ 60 /* established: bank sizes in megabytes, signed halfwords */
#define KLCONFIG_MEM_STRIDE_BNKSZ 2 /* established */
#define KLCONFIG_MEM_COUNT_BNKSZ 8 /* established */

/* MEM_W14: the 32-bit big-endian word at byte 56 */
/* dimm_select: signed halfword, bytes 58 to 59; halved and XORed into the bank index on the Origin 200/2000 platform only when an NVRAM byte allows it, always XORed on the Origin 300/350 platform */
/* FIELD: MEM_W14.dimm_select 15:0 established */
#define KLCONFIG_MEM_W14_DIMM_SELECT_SHIFT 0
#define KLCONFIG_MEM_W14_DIMM_SELECT_MASK  0xffffu

/* ---- global data area (reached through the fourth directory entry) ---- */
#define KLCONFIG_GDA_OFF_NASIDTABLE 128 /* established: signed halfword node number of each compact node; the first -1 ends the list */
#define KLCONFIG_GDA_STRIDE_NASIDTABLE 2 /* established */
#define KLCONFIG_GDA_COUNT_NASIDTABLE_IP27 64 /* established: entries the Origin 200/2000 kernel reads at most */
#define KLCONFIG_GDA_COUNT_NASIDTABLE_IP35 128 /* established: entries the Origin 300/350 kernel reads at most */
#define KLCONFIG_GDA_OFF_PEBRICK 392 /* established: Origin 300/350 only, pairs of signed halfwords; a first halfword of -1 ends the table */
#define KLCONFIG_GDA_STRIDE_PEBRICK 4 /* established */
#define KLCONFIG_GDA_COUNT_PEBRICK 4 /* established: pairs read at most */
#define KLCONFIG_GDA_PEBRICK_MAGIC 0x5045626bu /* established: the word at byte 388 equals this when the pair table follows */

/* GDA_W97: the 32-bit big-endian word at byte 388 */
/* pebrick_marker: Origin 300/350 only, equals the magic above when a PE-brick table follows */
/* FIELD: GDA_W97.pebrick_marker 31:0 established */
#define KLCONFIG_GDA_W97_PEBRICK_MARKER_SHIFT 0
#define KLCONFIG_GDA_W97_PEBRICK_MARKER_MASK  0xffffffffu

/* PEPAIR_W0: the 32-bit big-endian word of one PE-brick pair (pair i at byte 392 + 4 * i) */
/* pe_nasid: signed halfword node number of the PE-brick, bytes 0 to 1 of the pair; -1 ends the table */
/* FIELD: PEPAIR_W0.pe_nasid 31:16 established */
#define KLCONFIG_PEPAIR_W0_PE_NASID_SHIFT 16
#define KLCONFIG_PEPAIR_W0_PE_NASID_MASK  0xffffu
/* host_nasid: signed halfword node number of its host node, bytes 2 to 3 of the pair; -1 means no host node */
/* FIELD: PEPAIR_W0.host_nasid 15:0 established */
#define KLCONFIG_PEPAIR_W0_HOST_NASID_SHIFT 0
#define KLCONFIG_PEPAIR_W0_HOST_NASID_MASK  0xffffu

/*
 * Node class register (Origin 300/350 kernels). One doubleword of the node's
 * local register space, read uncached with a 64-bit load. The kernel keeps
 * only bits 15:12 of it. Two aliases of the same register are read: the local
 * one, and the node-indexed one, which adds the node number shifted left 33.
 * The offset below is relative to a node's register space; the two space
 * constants are the bases of the local and the node-indexed alias.
 */
#define KLCONFIG_NODECLASS_OFF 0x4000000u /* established: offset of the register in the node's local register space */
#define KLCONFIG_NODECLASS_SPACE_LOCAL 0x10000000u /* established: added to the offset for the local alias */
#define KLCONFIG_NODECLASS_SPACE_REMOTE 0x30000000u /* established: added to the offset, with the node number, for the node-indexed alias */

/* NODECLASS_W1: the 32-bit big-endian word at byte 4 of the doubleword (its low half) */
/* class: the kernel branches on 4, 8 and 12 and never examines another bit; what drives the bits on hardware is unknown */
/* FIELD: NODECLASS_W1.class 15:12 established */
#define KLCONFIG_NODECLASS_W1_CLASS_SHIFT 12
#define KLCONFIG_NODECLASS_W1_CLASS_MASK  0xfu
/* ENUM: NODECLASS_W1.class 12 pebrick established */
#define KLCONFIG_NODECLASS_W1_CLASS_PEBRICK 12

#endif
