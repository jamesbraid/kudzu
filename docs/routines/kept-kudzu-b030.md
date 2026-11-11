# Tier-1 routine readings kept from t1/kudzu-b30

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

### `pcireg_intr_addr_vect_set` in `6.5.30/unix.IP35` at 0xc0000000000b2fec

`pcireg_intr_addr_vect_set` is a 572-byte routine. It checks its first argument for null and
the hub base, bounds its second argument to the range 0 to 7, and dispatches on a value loaded
from memory against 2, 3, 0, 1 and -1. It then matches a device code against the same constant
set as the register readers, and a bad match reaches an assertion-failure call and a panic. The
name suggests it sets an interrupt vector, but the facts do not show that, so its role is
unknown. The 0-to-7 bound may be a slot or line number.

Grade: inferred.


### `pcireg_intr_addr_addr_set` in `6.5.30/unix.IP35` at 0xc0000000000b3458

`pcireg_intr_addr_addr_set` is a 588-byte routine that follows the same structure as
`pcireg_intr_addr_vect_set` above it: the same null and hub-base checks, the same 0-to-7 bound,
the same dispatch, and the same assertion and panic path. The name suggests it sets the
interrupt address register rather than the vector. The field writes are of the same kinds, so
the difference from the vector setter lies in which fields those writes hit, and the rendering
leaves those pointers unresolved.

Grade: inferred.
