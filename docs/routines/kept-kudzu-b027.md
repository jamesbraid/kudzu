# Tier-1 routine readings kept from t1/kudzu-b27-ip35

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

- `cpu_waitforreset` in `6.5.30/unix.IP35` at 0xc000000000077a78
  **Established.** The whole body is 8 bytes, 0xc000000000077a78 to 0xc000000000077a80: one branch at 0xc000000000077a78 that targets itself, with its delay slot. It touches no memory and makes no call, so the routine never returns. The name suggests it waits for a CPU reset, which these facts do not show.
