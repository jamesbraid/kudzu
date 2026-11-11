# Tier-1 routine readings kept from t1/kudzu-b31

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

### `silence_all_audio` in `6.5.30/usr/cpu/sysgen/IP27boot/kernel.o` at 0x1898

`silence_all_audio` (8 bytes) has no loads, stores or calls in its rendering, only a return
whose value the facts leave unknown. The name suggests muting audio. At eight bytes it can
only be a stub or placeholder, and the facts can confirm neither reading.

Grade: inferred.
