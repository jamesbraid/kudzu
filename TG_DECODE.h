/*
 * TG_DECODE.h - gigabit Ethernet controller match table of the Origin 300/350 driver
 *
 * Publish-safe contract for which PCI controllers the shipped gigabit Ethernet
 * driver of the Origin 300/350 kernels accepts at attach. The driver reads four
 * configuration halfwords of function 0 and walks a table of five rows; a
 * controller is accepted only when all four halfwords equal one row, so the
 * subsystem pair is mandatory and a controller with the chip vendor's own
 * subsystem ids is refused. No row matching means no interface is created.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   FIELD: <WORD>.<field> <hi>:<lo> <grade>             a field of a 32-bit word
 *
 * A table row is 16 bytes in big-endian memory: two 32-bit words that hold the
 * four match halfwords, then eight bytes of flag halfwords whose meaning is
 * not decoded here. The device numbers are those the public Linux driver for
 * the same chip family uses; the chip names are labels only.
 *
 * Derivation. The table and the attach routine come from reading the driver
 * object of one release; the older release's driver was not read. Which of the
 * three rows for
 * device 0x1645 a Tezro's controller carries is unknown.
 */
#ifndef TG_DECODE_H
#define TG_DECODE_H

#include <stdint.h>

#define TG_MATCH_ROW_STRIDE 16 /* established: bytes between table rows */
#define TG_MATCH_ROW_COUNT 5 /* established: rows the attach loop walks */

/* Configuration halfwords the attach routine reads: offsets 0 (device), 2 (vendor), 0x2c (subsystem vendor) and 0x2e (subsystem device) */
#define TG_CFG_OFF_DEVICE 0x00 /* established */
#define TG_CFG_OFF_VENDOR 0x02 /* established */
#define TG_CFG_OFF_SUBSYS_VENDOR 0x2c /* established */
#define TG_CFG_OFF_SUBSYS_DEVICE 0x2e /* established */

#define TG_PCI_VENDOR 0x14e4u /* established: the vendor id of every row */
#define TG_PCI_SUBSYS_VENDOR 0x10a9u /* established: the subsystem vendor of every row */
#define TG_PCI_DEVICE_1645 0x1645u /* established: three rows (subsystem 0x8010, 0x8011, 0x8012) */
#define TG_PCI_DEVICE_1648 0x1648u /* established: one row (subsystem 0x8013) */
#define TG_PCI_DEVICE_16A8 0x16a8u /* established: one row (subsystem 0x8014) */
#define TG_PCI_SUBSYS_DEVICE_FIRST 0x8010u /* established: subsystem device of the first row; the rows run consecutively to 0x8014 */

/* ROW_W0: the 32-bit big-endian word at byte 0 of a table row */
/* vendor: PCI vendor id to match */
/* FIELD: ROW_W0.vendor 31:16 established */
#define TG_ROW_W0_VENDOR_SHIFT 16
#define TG_ROW_W0_VENDOR_MASK  0xffffu
/* device: PCI device id to match */
/* FIELD: ROW_W0.device 15:0 established */
#define TG_ROW_W0_DEVICE_SHIFT 0
#define TG_ROW_W0_DEVICE_MASK  0xffffu

/* ROW_W1: the 32-bit big-endian word at byte 4 of a table row */
/* subsys_vendor: PCI subsystem vendor id to match */
/* FIELD: ROW_W1.subsys_vendor 31:16 established */
#define TG_ROW_W1_SUBSYS_VENDOR_SHIFT 16
#define TG_ROW_W1_SUBSYS_VENDOR_MASK  0xffffu
/* subsys_device: PCI subsystem device id to match */
/* FIELD: ROW_W1.subsys_device 15:0 established */
#define TG_ROW_W1_SUBSYS_DEVICE_SHIFT 0
#define TG_ROW_W1_SUBSYS_DEVICE_MASK  0xffffu

#endif
