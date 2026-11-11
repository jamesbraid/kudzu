/*
 * SCSI_DECODE.h - SCSI command opcodes the CD-ROM driver selects by drive class
 *
 * Publish-safe contract for the three command opcodes whose use depends on the
 * drive class the CD-ROM driver assigns from the inquiry data. The class
 * numbers are the driver's own and are not restated here: the notes list which
 * drives fall in each.
 *
 * The INQUIRY data the classes and the firmware's drive test are chosen from is
 * described by byte position only: the standard command's response, as the
 * shipped firmware and the removable-media daemon read it.
 *
 * Derivation. Opcodes and which classes send them come from reading the
 * driver's class-dependent routines in the shipped kernel. START STOP UNIT is
 * the standard opcode. The two vendor opcodes are not in any published command
 * set the notes found, so what they do is inferred from their position in the
 * driver (software eject, mode switch).
 */
#ifndef SCSI_DECODE_H
#define SCSI_DECODE_H

#include <stdint.h>

#define SCSI_START_STOP_UNIT 0x1b /* established: sent by every class except the first three */
#define SCSI_CDROM_VENDOR_C4 0xc4 /* established: sent by classes 1 to 3 in place of START STOP UNIT; use as software eject is inferred */
#define SCSI_CDROM_VENDOR_C9 0xc9 /* established: sent only by classes 1 to 3; use as a mode switch is inferred */

/* ---- INQUIRY response bytes the shipped software reads ---- */
#define SCSI_INQ_OFF_TYPE 0 /* established: device type byte; the PROMs test it for 0 and the daemon accepts only 5 */
#define SCSI_INQ_TYPE_DISK 0 /* established: the PROMs test for this value before comparing the text */
#define SCSI_INQ_TYPE_TAPE 1 /* established: named by the O2 PROM's probe; no other PROM tests it */
#define SCSI_INQ_TYPE_CDROM 5 /* established: the daemon accepts it; the PROMs never compare the text of a type 5 unit */
#define SCSI_INQ_OFF_VERSION 2 /* established: version byte; the daemon masks it with 7 and tests for 2 to refine one drive family */
#define SCSI_INQ_VERSION_MASK 0x7 /* established */
#define SCSI_INQ_VERSION_REFINE 2 /* established */
#define SCSI_INQ_OFF_VENDOR 8 /* established: first byte of the vendor text, the start of the text the daemon matches */
#define SCSI_INQ_LEN_VENDOR 8 /* established: the notes and both readers treat vendor and product as one 24-byte text */
#define SCSI_INQ_OFF_PRODUCT 16 /* inferred: follows the vendor field in that text */
#define SCSI_INQ_LEN_PRODUCT 16 /* inferred */

/* ---- the text the PROMs compare for a type 0 unit ---- */
#define SCSI_PROM_PREFIX_LEN 10 /* established: bytes of the trimmed vendor, separator and product that are compared */
#define SCSI_PROM_PREFIX_TOSHIBA "TOSHIBA CD" /* established: a match makes the PROM send the 0xc9 command */
#define SCSI_PROM_PREFIX_SONY "SONY    CD" /* established: compared; what a match does is not recorded in the notes */
#define SCSI_PROM_PREFIX_LMS "LMS     CD" /* established: compared; what a match does is not recorded in the notes */

#endif
