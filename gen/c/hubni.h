/* generated from defs/hubni.toml by defs-gen - do not edit, change the source */
/* node hub network interface registers */
#ifndef HUBNI_GEN_H
#define HUBNI_GEN_H

#include <stdint.h>


/*
 * The low 32 bits of the Origin 200/2000 network status word.
 */
/* word NSRI: 32 bits; shipped IRIX kernel binaries, per docs/ip35-hub-errors.md */
#define HUBNI_NSRI_WIDTH                             32 /* established */
#define HUBNI_NSRI_NODEID_SHIFT                      8 /* established */
#define HUBNI_NSRI_NODEID_MASK                       0x1ffu
#define HUBNI_NSRI_NODEID_INPLACE                    (HUBNI_NSRI_NODEID_MASK << HUBNI_NSRI_NODEID_SHIFT)

#define HUBNI_O200_STATUS_OFFSET                     0x600000u /* established */
#define HUBNI_O300_ERROR_CLEAR_OFFSET                0x680120u /* established */
#define HUBNI_O300_ERROR_LATCHED_HI                  0x34u /* established */
#define HUBNI_O300_ERROR_LATCHED_LO                  0x18u /* established */
#define HUBNI_O300_ERROR_LINK_RESET_BIT              0x34u /* established */
#define HUBNI_O300_ERROR_OFFSET                      0x680100u /* established */
#define HUBNI_O300_ERROR_REPORT_A_OFFSET             0x680108u /* established */
#define HUBNI_O300_ERROR_REPORT_B_OFFSET             0x680110u /* established */
#define HUBNI_O300_ERROR_REPORT_C_OFFSET             0x680118u /* established */
#define HUBNI_O300_PORT_RESET_OFFSET                 0x680008u /* established */
#define HUBNI_O300_RESET_ENABLE_OFFSET               0x680010u /* established */
#define HUBNI_O300_STATUS_OFFSET                     0x680000u /* established */

#endif /* HUBNI_GEN_H */
