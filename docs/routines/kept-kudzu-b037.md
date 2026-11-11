# Tier-1 routine readings kept from t1/kudzu-b37

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

- `Mgras_FC_Disable` in `6.5.30/usr/cpu/sysgen/IP30boot/mgras.a(mgras_init.o)` at 0x1230.
  The 24-byte body loads the doubleword at 0xffffffffaff00040 (`ld` at 0x1234) and stores 8 bytes back to that address (`sd` at 0x1244). It makes no calls and returns the constant 0xffffffffaff00000 at 0x1240. The rendered facts do not show the operation between the load and the store, so the bit change is unknown. The name suggests a flow-control disable, and the same doubleword is read by MgrasGeneralInterrupt at 0x81dc, but the facts do not name the register, so the role is unknown.
  Evidence grade: unknown.

- `silence_all_audio` in `6.5.30/usr/cpu/sysgen/IP32boot/kernel.o` at 0x1be8.
  The 8-byte body has no loads, stores or calls, and its return value is not known from the facts. The name suggests muting audio, but nothing in the rendered facts touches audio hardware, so the role is unknown.
  Evidence grade: unknown.
