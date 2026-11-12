/* generated from defs/r10k_timer.toml by defs-gen - do not edit, change the source */
/* R10000 Count and Compare timer interrupt */
#ifndef R10K_TIMER_GEN_H
#define R10K_TIMER_GEN_H

#include <stdint.h>


/* word CAUSE: 32 bits; published R10000 user's manual (the header gives no document number) */
#define R10K_TIMER_CAUSE_WIDTH                       32 /* established */
#define R10K_TIMER_CAUSE_IP7_SHIFT                   15 /* established */
#define R10K_TIMER_CAUSE_IP7_MASK                    0x1u
#define R10K_TIMER_CAUSE_IP7_INPLACE                 (R10K_TIMER_CAUSE_IP7_MASK << R10K_TIMER_CAUSE_IP7_SHIFT)

#define R10K_CP0_COMPARE                             0xbu /* established */
#define R10K_CP0_COUNT                               0x9u /* established */

#endif /* R10K_TIMER_GEN_H */
