# Tier-1 routine readings kept from t1/kudzu-b26-r2

Entries that were on this kept branch and not on main. The role text is unchanged; only the header line was rewritten to the canonical form, with the routine name and the install path that functions.tsv gives for the address.

- `silence_all_audio` in `6.5.30/unix.IP35` at 0xc000000000041dd8.
  The body is 8 bytes and does nothing but return, with v0 unset. The name suggests an audio mute, but the facts show no work that would do one. The routine may be a stub, and its role is unknown.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `page_local_calias` in `6.5.30/unix.IP35` at 0xc0000000000502a4.
  It loads a doubleword from abs 0x9200000001000028. If that value is nonzero, it compares a3 with v1 as unsigned values and returns 0 or 1. The facts do not say what the stored word means, so its role is unknown, and the name's page-local aliasing reading is not established.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `net_link_up` in `6.5.30/unix.IP35` at 0xc000000000051b04.
  It loads one doubleword from abs 0x9200000001680000, with no branch and no call. The facts give the return value only as unknown, so whether the loaded word is returned as is, or masked or shifted first, is not shown. That base is the block that ni_reset_port writes (0x9200000001680008 and 0x9200000001680010), so the value may be a network status word. The facts do not show its bit meanings or its callers, so the link-status reading is inferred from the name and the address only.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)

- `save_hub_err_ext` in `6.5.30/unix.IP35` at 0xc000000000053a8c.
  It zeroes a 0xc0-byte area with blkclr (0xc00000000004791c), passing its entry a1 as the area. After the call the facts show three doubleword load and store pairs (0xc000000000053ad8 with 0xc000000000053adc, 0xc000000000053b00 with 0xc000000000053b08, and 0xc000000000053b28 with 0xc000000000053b30), each guarded by a compare of a2: the branches at 0xc000000000053ae0 and 0xc000000000053b04 compare it with 0xf (signed) and the one at 0xc000000000053b2c compares it with 0x9. These are loop-shaped compares, so more than three doublewords may be copied; the facts do not give the source base, the destination or the trip counts. Beyond the extended-error name the role stays open.
  Grade: inferred.
  Reviewed: 2026-11-12 (sonnet)
