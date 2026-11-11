# Tier-1 routine readings kept from t1/kudzu-b20

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

- `save_hub_err_ext` in `6.5.22/usr/cpu/sysgen/IP35boot/kernel.o` at 0x1420c
  It zeroes a 0xc0-byte block at a1 with `bzero` (call at 0x14228), then runs a short run of loads and stores whose base addresses the renderer could not resolve. Branches at 0x14260, 0x14284 and 0x142ac compare a2 against 0xf and 0x9 to pick among the stores. The name suggests an extended error record, but the facts do not show which structure is copied.
  Grade: unknown.
