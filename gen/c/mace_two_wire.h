/* generated from defs/mace_two_wire.toml by defs-gen - do not edit, change the source */
/* MACE peripheral two-wire bus block as the IRIX video driver drives it */
#ifndef MACE_TWO_WIRE_GEN_H
#define MACE_TWO_WIRE_GEN_H

#include <stdint.h>


/*
 * Bus configuration word. The configuration helper reads it and writes it back with the bits selected by its mask replaced. The driver writes 1 to request a bus reset. Meanings of the other values written are not recorded.
 */
/* tw_config: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_TWO_WIRE_TW_CONFIG_OFFSET               0x0u /* established */
#define MACE_TWO_WIRE_TW_CONFIG_LOW_CONFIG_SHIFT     0 /* established */
#define MACE_TWO_WIRE_TW_CONFIG_LOW_CONFIG_MASK      0xfull
#define MACE_TWO_WIRE_TW_CONFIG_LOW_CONFIG_INPLACE   (MACE_TWO_WIRE_TW_CONFIG_LOW_CONFIG_MASK << MACE_TWO_WIRE_TW_CONFIG_LOW_CONFIG_SHIFT)

/*
 * Bus control and status word. Transfer routines poll the status bits below. Writes of 1, 3 and 5 are commands, and their meanings are inferred in docs/mace-video.md.
 */
/* tw_control: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_TWO_WIRE_TW_CONTROL_OFFSET              0x10u /* established */
#define MACE_TWO_WIRE_TW_CONTROL_BUS_ERROR_SHIFT     7 /* established */
#define MACE_TWO_WIRE_TW_CONTROL_BUS_ERROR_MASK      0x1ull
#define MACE_TWO_WIRE_TW_CONTROL_BUS_ERROR_INPLACE   (MACE_TWO_WIRE_TW_CONTROL_BUS_ERROR_MASK << MACE_TWO_WIRE_TW_CONTROL_BUS_ERROR_SHIFT)
#define MACE_TWO_WIRE_TW_CONTROL_ACK_MISSING_SHIFT   5 /* established */
#define MACE_TWO_WIRE_TW_CONTROL_ACK_MISSING_MASK    0x1ull
#define MACE_TWO_WIRE_TW_CONTROL_ACK_MISSING_INPLACE (MACE_TWO_WIRE_TW_CONTROL_ACK_MISSING_MASK << MACE_TWO_WIRE_TW_CONTROL_ACK_MISSING_SHIFT)
#define MACE_TWO_WIRE_TW_CONTROL_TRANSFER_BUSY_SHIFT 4 /* established */
#define MACE_TWO_WIRE_TW_CONTROL_TRANSFER_BUSY_MASK  0x1ull
#define MACE_TWO_WIRE_TW_CONTROL_TRANSFER_BUSY_INPLACE (MACE_TWO_WIRE_TW_CONTROL_TRANSFER_BUSY_MASK << MACE_TWO_WIRE_TW_CONTROL_TRANSFER_BUSY_SHIFT)
#define MACE_TWO_WIRE_TW_CONTROL_NOT_IDLE_SHIFT      0 /* established */
#define MACE_TWO_WIRE_TW_CONTROL_NOT_IDLE_MASK       0x1ull
#define MACE_TWO_WIRE_TW_CONTROL_NOT_IDLE_INPLACE    (MACE_TWO_WIRE_TW_CONTROL_NOT_IDLE_MASK << MACE_TWO_WIRE_TW_CONTROL_NOT_IDLE_SHIFT)

/*
 * Data word. The read and write routines store the chip address byte and the register number byte through it, and read it back after the transfer. How its bytes map to bits is not recorded.
 */
/* tw_data: rw64, reset unknown; shipped IRIX O2 video driver object, per docs/mace-video.md */
#define MACE_TWO_WIRE_TW_DATA_OFFSET                 0x18u /* established */

#endif /* MACE_TWO_WIRE_GEN_H */
