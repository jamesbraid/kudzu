/*
 * SCSI_DECODE.h - SCSI command opcodes the CD-ROM driver selects by drive class
 *
 * Publish-safe contract for the three command opcodes whose use depends on the
 * drive class the CD-ROM driver assigns from the inquiry data. The class
 * numbers are the driver's own and are not restated here: the notes list which
 * drives fall in each.
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

#endif
