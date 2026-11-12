/* generated from defs/hubio.toml by defs-gen - do not edit, change the source */
/* Origin 300 hub crosstalk-side register window */
#ifndef HUBIO_GEN_H
#define HUBIO_GEN_H

#include <stdint.h>


/*
 * Link control and status doubleword that the early widget probe reads. Only bit 13
 * is described. With it clear, the probe returns -1 for the part, revision and
 * manufacturer without touching the widget. Whether the bit means link up is
 * inferred from how the I/O error interrupt set-up uses it.
 */
/* llp_csr: r64, reset unknown; shipped IRIX kernel binaries, per docs/ip35-hub-errors.md */
#define HUBIO_LLP_CSR_OFFSET                         0x128u /* established */
#define HUBIO_LLP_CSR_BIT13_SHIFT                    13 /* established */
#define HUBIO_LLP_CSR_BIT13_MASK                     0x1ull
#define HUBIO_LLP_CSR_BIT13_INPLACE                  (HUBIO_LLP_CSR_BIT13_MASK << HUBIO_LLP_CSR_BIT13_SHIFT)

/*
 * The register the kernel's error path tests before it goes on to the widget error handler.
 */
/* flags: r64, reset unknown; shipped IRIX kernel binaries, per docs/ip35-hub-errors.md */
#define HUBIO_FLAGS_OFFSET                           0x1e8u /* established */
#define HUBIO_FLAGS_BIT4_SHIFT                       4 /* established */
#define HUBIO_FLAGS_BIT4_MASK                        0x1ull
#define HUBIO_FLAGS_BIT4_INPLACE                     (HUBIO_FLAGS_BIT4_MASK << HUBIO_FLAGS_BIT4_SHIFT)
#define HUBIO_FLAGS_BIT0_SHIFT                       0 /* established */
#define HUBIO_FLAGS_BIT0_MASK                        0x1ull
#define HUBIO_FLAGS_BIT0_INPLACE                     (HUBIO_FLAGS_BIT0_MASK << HUBIO_FLAGS_BIT0_SHIFT)

/*
 * Request-buffer table. Entry i (0 to 14) has three 64-bit words, crb_a, crb_b
 * and crb_c, at the register offsets below plus the stride times i; the scan
 * reads all fifteen entries in order. See CRB_STRIDE and CRB_COUNT.
 */
/* crb_a: r64, reset unknown; shipped IRIX kernel binaries, per docs/ip35-hub-errors.md */
#define HUBIO_CRB_A_OFFSET                           0x400u /* established */
#define HUBIO_CRB_A_BIT55_SHIFT                      55 /* established */
#define HUBIO_CRB_A_BIT55_MASK                       0x1ull
#define HUBIO_CRB_A_BIT55_INPLACE                    (HUBIO_CRB_A_BIT55_MASK << HUBIO_CRB_A_BIT55_SHIFT)
#define HUBIO_CRB_A_TYPE_SHIFT                       52 /* established */
#define HUBIO_CRB_A_TYPE_MASK                        0x7ull
#define HUBIO_CRB_A_TYPE_INPLACE                     (HUBIO_CRB_A_TYPE_MASK << HUBIO_CRB_A_TYPE_SHIFT)
#define HUBIO_CRB_A_BIT51_SHIFT                      51 /* established */
#define HUBIO_CRB_A_BIT51_MASK                       0x1ull
#define HUBIO_CRB_A_BIT51_INPLACE                    (HUBIO_CRB_A_BIT51_MASK << HUBIO_CRB_A_BIT51_SHIFT)
#define HUBIO_CRB_A_BIT50_SHIFT                      50 /* established */
#define HUBIO_CRB_A_BIT50_MASK                       0x1ull
#define HUBIO_CRB_A_BIT50_INPLACE                    (HUBIO_CRB_A_BIT50_MASK << HUBIO_CRB_A_BIT50_SHIFT)
#define HUBIO_CRB_A_BIT49_SHIFT                      49 /* established */
#define HUBIO_CRB_A_BIT49_MASK                       0x1ull
#define HUBIO_CRB_A_BIT49_INPLACE                    (HUBIO_CRB_A_BIT49_MASK << HUBIO_CRB_A_BIT49_SHIFT)
#define HUBIO_CRB_A_BIT0_SHIFT                       0 /* established */
#define HUBIO_CRB_A_BIT0_MASK                        0x1ull
#define HUBIO_CRB_A_BIT0_INPLACE                     (HUBIO_CRB_A_BIT0_MASK << HUBIO_CRB_A_BIT0_SHIFT)

/* crb_b: r64, reset unknown; shipped IRIX kernel binaries, per docs/ip35-hub-errors.md */
#define HUBIO_CRB_B_OFFSET                           0x408u /* established */
#define HUBIO_CRB_B_ENGINE_ALT_SHIFT                 58 /* established */
#define HUBIO_CRB_B_ENGINE_ALT_MASK                  0x1ull
#define HUBIO_CRB_B_ENGINE_ALT_INPLACE               (HUBIO_CRB_B_ENGINE_ALT_MASK << HUBIO_CRB_B_ENGINE_ALT_SHIFT)
#define HUBIO_CRB_B_SUB_SHIFT                        41 /* established */
#define HUBIO_CRB_B_SUB_MASK                         0x3ull
#define HUBIO_CRB_B_SUB_INPLACE                      (HUBIO_CRB_B_SUB_MASK << HUBIO_CRB_B_SUB_SHIFT)
#define HUBIO_CRB_B_KIND_SHIFT                       30 /* established */
#define HUBIO_CRB_B_KIND_MASK                        0x7ull
#define HUBIO_CRB_B_KIND_INPLACE                     (HUBIO_CRB_B_KIND_MASK << HUBIO_CRB_B_KIND_SHIFT)

/* crb_c: r64, reset unknown; shipped IRIX kernel binaries, per docs/ip35-hub-errors.md */
#define HUBIO_CRB_C_OFFSET                           0x410u /* established */
#define HUBIO_CRB_C_BIT48_SHIFT                      48 /* established */
#define HUBIO_CRB_C_BIT48_MASK                       0x1ull
#define HUBIO_CRB_C_BIT48_INPLACE                    (HUBIO_CRB_C_BIT48_MASK << HUBIO_CRB_C_BIT48_SHIFT)

#define HUBIO_CRB_COUNT                              0xfu /* established */
#define HUBIO_CRB_STRIDE                             0x20u /* established */
#define HUBIO_NODE_SHIFT                             0x21u /* established */
#define HUBIO_WIDGET_PART_C104                       0xc104u /* established */
#define HUBIO_WIDGET_PART_C105                       0xc105u /* established */
#define HUBIO_WINDOW_BASE                            0x1c00000u /* established */

#endif /* HUBIO_GEN_H */
