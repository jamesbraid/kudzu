# Tier-1 routine readings kept from t1/kudzu-b30

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

### `pcireg_intr_addr_vect_set` in `6.5.30/unix.IP35` at 0xc0000000000b2fec

`pcireg_intr_addr_vect_set` is a 572-byte routine. It branches to a different block (0xc0000000000b3088) when its
first argument is null (branch at 0xc0000000000b3008) and compares a register against the constant
0x9200000000000000 (0xc0000000000b301c). It bounds its second argument: a value below 0 or at or
above 8 goes to the assertion-failure call at 0xc0000000000b3204 (branches at 0xc0000000000b3030 and
0xc0000000000b303c). It then dispatches on a register, whose source the facts do not resolve, against
2, 3, 0, 1 and -1; the -1 case leads to the `panic` call at 0xc0000000000b3218 [call 2]. Separately it
loads a word at offset 4 and compares it against 0xe000, 0xe010, 0xd102, 0xd112, 0xd002 and 0xc002; a
value that matches none of them reaches `assfail` [call 1]. Each dispatch arm does loads and stores
(sd, sd, sw, sw), but the facts do not resolve what they address.

Role (from name): it may set an interrupt address vector, but the facts do not show that, so its role
is unknown. The 0-to-7 bound may be a slot or line number.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)


### `pcireg_intr_addr_addr_set` in `6.5.30/unix.IP35` at 0xc0000000000b3458

`pcireg_intr_addr_addr_set` is a 588-byte routine whose branch layout matches
`pcireg_intr_addr_vect_set` at 0xc0000000000b2fec: a null test on its first argument (0xc0000000000b3474),
a compare against 0x9200000000000000 (0xc0000000000b3488), a bound of 0 to 7 on its second argument
(0xc0000000000b349c and 0xc0000000000b34a8), a dispatch against 2, 3, 0, 1 and -1 (the -1 case reaches
`panic` [call 2]), and a match of a word at offset 4 against the same constants 0xe000 to 0xc002, with
no match reaching `assfail` [call 1]. The dispatch arms perform the same sequence of loads and stores
(sd, sd, sw, sw); the facts leave their addresses unresolved.

Role (from name): it may set an interrupt address, as against the vector setter, but the facts do not
show what those stores write, so its role is unknown.

Grade: inferred.

Reviewed: 2026-11-12 (sonnet)
