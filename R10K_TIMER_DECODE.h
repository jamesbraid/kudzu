/*
 * R10K_TIMER_DECODE.h - R10000 Count and Compare timer interrupt
 *
 * Publish-safe contract for the processor's own timer interrupt: the coprocessor
 * 0 registers and the pending bit. Not read from the kernels: the facts come
 * from the published R10000 user's manual and are what the notes grade
 * established for the manual text. Hardware behaviour for a compare value that
 * is already behind the count (a wait for the 32-bit wrap) is inferred, not
 * measured, and is not stated here.
 *
 * Grammar (one annotation per comment line, do not reformat):
 *   FIELD: <WORD>.<field> <hi>:<lo> <grade>             a field of a 32-bit word
 *
 * CAUSE is the 32-bit coprocessor 0 Cause register.
 */
#ifndef R10K_TIMER_DECODE_H
#define R10K_TIMER_DECODE_H

#include <stdint.h>

#define R10K_CP0_COUNT 9 /* established: Count register number in coprocessor 0 */
#define R10K_CP0_COMPARE 11 /* established: Compare register number in coprocessor 0 */

/* ip7: set when Count equals Compare; a write to Compare clears it */
/* FIELD: CAUSE.ip7 15:15 established */
#define R10K_TIMER_CAUSE_IP7_SHIFT 15
#define R10K_TIMER_CAUSE_IP7_MASK  0x1u

#endif
