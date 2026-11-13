/* generated from formats/scsi/format.toml by defs-gen - do not edit, change the source */
/* SCSI command opcodes the CD-ROM driver selects by drive class */
#ifndef SCSI_GEN_H
#define SCSI_GEN_H

#include <stdint.h>


#define SCSI_CDROM_VENDOR_C4                         0xc4u /* established */
#define SCSI_CDROM_VENDOR_C9                         0xc9u /* established */
#define SCSI_INQ_LEN_PRODUCT                         0x10u /* inferred */
#define SCSI_INQ_LEN_VENDOR                          0x8u /* established */
#define SCSI_INQ_OFF_PRODUCT                         0x10u /* inferred */
#define SCSI_INQ_OFF_TYPE                            0x0u /* established */
#define SCSI_INQ_OFF_VENDOR                          0x8u /* established */
#define SCSI_INQ_OFF_VERSION                         0x2u /* established */
#define SCSI_INQ_TYPE_CDROM                          0x5u /* established */
#define SCSI_INQ_TYPE_DISK                           0x0u /* established */
#define SCSI_INQ_TYPE_TAPE                           0x1u /* established */
#define SCSI_INQ_VERSION_MASK                        0x7u /* established */
#define SCSI_INQ_VERSION_REFINE                      0x2u /* established */
#define SCSI_PROM_PREFIX_LEN                         0xau /* established */
#define SCSI_START_STOP_UNIT                         0x1bu /* established */

#endif /* SCSI_GEN_H */
