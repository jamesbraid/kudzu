# Tier-2 routine readings, kudzu batch 4

Twenty-five `pcireg_*` accessors from the 6.5.22 IP27 unix build, read from their rendered facts. The name and the shape of each routine (a record pointer in a0, a kind halfword at +0x72, a selector chain, and a read-modify-write under an interrupt-level bracket) support a reading of PCI bridge register access. The facts never name the registers, so each role is inferred.

- `pcireg_control_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000080c6c
  **Inferred.** Takes a record pointer in a0 and branches to the panic path at 0xc000000000080e04 when a0 is zero (`beq` at 0xc000000000080c8c). The kind halfword at +0x72 is loaded at 0xc000000000080ca8, and the code then branches on kind values 3, 2, 1, 0 and -1 (`beq` at 0xc000000000080cb0 through 0xc000000000080cd4). The calls are `spl1` at 0xc000000000080c80, `splx` at 0xc000000000080ce0 (followed by the return at 0xc000000000080cec) and `panic` at 0xc000000000080e08; the facts do not show which loads and stores lie between `spl1` and `splx`. The stores are `sw` at 0xc000000000080d64 and `sd` at 0xc000000000080d8c and 0xc000000000080db4. The bit cleared is unknown, and so is the register each selector constant addresses.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_control_bit_set` in `6.5.22/unix.IP27` at 0xc000000000080e18
  **Inferred.** The `lh` at 0xc000000000080e54 reads the kind halfword at +0x72, and the `lw` at 0xc000000000080ea4 reads a word at +0x4. The store `sw` at 0xc000000000080f08 writes a word, and the `sd` pair at 0xc000000000080f2c and 0xc000000000080f50 writes doublewords. The calls are `spl1` at 0xc000000000080e2c, `splx` at 0xc000000000080e8c and `panic` at 0xc000000000080fa4, which is the failure exit on one path; the facts do not show which stores lie between `spl1` and `splx`. The bit it sets is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_ssram_size_set` in `6.5.22/unix.IP27` at 0xc000000000080fb4
  **Inferred.** Writes two words, `sw` at 0xc0000000000810c8 and `sw` at 0xc0000000000810d8, and the facts also list the kind halfword at +0x72 (`lh` at 0xc000000000080ff0), an eight-byte field at +0x8 (`ld` at 0xc000000000080ffc) and a word at +0x4 (`lw` at 0xc000000000081048) as loads. The calls are `spl1` at 0xc000000000080fc8 and `splx` at 0xc000000000081030; the facts do not show that the stores lie between them. The signed tests of a1 at 0xc0000000000810a8 and 0xc0000000000810b4 branch to 0xc00000000008117c when a1 is below 0 or at least 4, and the two stores follow them in address order. Three calls are failure calls: `panic` at 0xc000000000081158, `panic` at 0xc00000000008116c and `assfail` at 0xc000000000081184. The size encoding is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_speed_get` in `6.5.22/unix.IP27` at 0xc0000000000811a4
  **Inferred.** Reads a word at `lw` 0xc0000000000812cc in addition to the kind halfword (`lh` at 0xc0000000000811c8), the eight-byte field at +0x8 (`ld` at 0xc0000000000811d4) and the word at +0x4 (`lw` at 0xc000000000081210). The return at 0xc000000000081204 hands back 0 or a value that is unknown. The `jal` at 0xc00000000008130c calls `panic`; the facts list a4 as -1 at that call and show no other value. The unit of the returned value is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_mode_get` in `6.5.22/unix.IP27` at 0xc00000000008131c
  **Inferred.** A read path with no stores: the kind halfword sits at +0x72 (`lh` at 0xc000000000081340), the eight-byte field at +0x8 (`ld` at 0xc00000000008134c) and the word at +0x4 (`lw` at 0xc000000000081384). The return at 0xc000000000081378 gives 0 or a value that is unknown. `panic` at 0xc000000000081478 is the only call; the facts list a4 as -1 at that call and show no other value. The mode value's bits are unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_req_timeout_get` in `6.5.22/unix.IP27` at 0xc000000000081488
  **Inferred.** Three field reads open the routine: `lh` at 0xc0000000000814ac for the kind halfword at +0x72, `ld` at 0xc0000000000814b8 for +0x8 and `lw` at 0xc0000000000814f0 for +0x4. A second word load at 0xc00000000008154c comes from an address that is unknown. The only call is `panic` at 0xc0000000000815dc. The timeout's unit is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_req_timeout_set` in `6.5.22/unix.IP27` at 0xc0000000000815ec
  **Inferred.** Reads the kind halfword (`lh` at 0xc000000000081610) and the word at +0x4 (`lw` at 0xc000000000081654). The stores are a word with `sw` at 0xc0000000000816b4 and doublewords with `sd` at 0xc0000000000816d0 and 0xc0000000000816ec; the facts do not show which paths reach each. The `panic` at 0xc000000000081738 receives a1 as its second argument. The value written is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_dst_get` in `6.5.22/unix.IP27` at 0xc000000000081748
  **Inferred.** Pulls the kind halfword from +0x72 (`lh` at 0xc00000000008176c), the eight-byte field at +0x8 (`ld` at 0xc000000000081778) and the word at +0x4 (`lw` at 0xc0000000000817b4). The return at 0xc0000000000817a8 yields 0 or an unknown value, and `panic` at 0xc0000000000818ac is the only call. The encoding of the destination is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_dst_set` in `6.5.22/unix.IP27` at 0xc0000000000818bc
  **Inferred.** Loads the kind halfword (`lh` at 0xc0000000000818e0), the eight-byte field at +0x8 (`ld` at 0xc0000000000818ec) and the word at +0x4 (`lw` at 0xc000000000081924). It stores words at 0xc000000000081984 and 0xc00000000008198c (`sw`) and doublewords at 0xc0000000000819a8 and 0xc0000000000819c4 (`sd`). The `panic` at 0xc000000000081a10 is the only call; the facts list a5 as -1 at that call and show no other value. The destination value is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_dst_target_id_get` in `6.5.22/unix.IP27` at 0xc000000000081a20
  **Inferred.** Starts with the kind halfword at +0x72 (`lh` at 0xc000000000081a44), then the eight-byte field at +0x8 (`ld` at 0xc000000000081a50) and the word at +0x4 (`lw` at 0xc000000000081a8c). Further loads come from `lw` at 0xc000000000081ae8 and `ld` at 0xc000000000081b0c, and the return at 0xc000000000081a80 gives 0 or an unknown value. `panic` at 0xc000000000081b7c is the failure path. The identifier's bit layout is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_dst_target_id_set` in `6.5.22/unix.IP27` at 0xc000000000081b8c
  **Inferred.** Reads the kind halfword (`lh` at 0xc000000000081bb0), the eight-byte field at +0x8 (`ld` at 0xc000000000081bbc) and the word at +0x4 (`lw` at 0xc000000000081bf4). Writes go out as `sw` at 0xc000000000081c60 and 0xc000000000081c78 and as `sd` at 0xc000000000081c9c and 0xc000000000081cb4. A `panic` at 0xc000000000081d18 takes a1 as its second argument. The identifier value is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_dst_addr_get` in `6.5.22/unix.IP27` at 0xc000000000081d28
  **Inferred.** The kind halfword at +0x72 is loaded at 0xc000000000081d4c, the eight-byte field at +0x8 at 0xc000000000081d58 and the word at +0x4 at 0xc000000000081d94. A doubleword load at 0xc000000000081e24 reads from an address that is unknown. The `panic` at 0xc000000000081e98 is the only call. The address format is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_dst_addr_set` in `6.5.22/unix.IP27` at 0xc000000000081ea8
  **Inferred.** After three field reads (`lh` at 0xc000000000081ecc, `ld` at 0xc000000000081ed8, `lw` at 0xc000000000081f10), the routine stores three words (`sw` at 0xc000000000081f78, 0xc000000000081f8c and 0xc000000000081f94) and two doublewords (`sd` at 0xc000000000081fc0 and 0xc000000000081fd8). The only call is `panic` at 0xc00000000008203c, reached from the a0 == 0 test, the kind == -1 test and the end of the word-value tests. The address written is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_cmdword_err_get` in `6.5.22/unix.IP27` at 0xc00000000008204c
  **Inferred.** Opens with the kind halfword (`lh` at 0xc000000000082070), the eight-byte field at +0x8 (`ld` at 0xc00000000008207c) and the word at +0x4 (`lw` at 0xc0000000000820b8). Further loads are a word at 0xc000000000082114 and doublewords at 0xc00000000008213c and 0xc000000000082158, and the return at 0xc0000000000820ac yields 0 or an unknown value. The only call is `panic` at 0xc0000000000821a4. What the command word's error bits mean is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_llp_cfg_get` in `6.5.22/unix.IP27` at 0xc0000000000821b4
  **Inferred.** Reads the kind halfword at 0xc0000000000821d4 (+0x72), the eight-byte field at 0xc0000000000821e0 (+0x8) and the word at 0xc00000000008221c (+0x4). Two more loads, a word at 0xc000000000082278 and a doubleword at 0xc0000000000822a0, come from addresses that are unknown. The return at 0xc000000000082210 yields 0 or an unknown value, and the calls at 0xc000000000082304 and 0xc000000000082318 both reach `panic`, the second with a5 set to 0xd102. The configuration fields are unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_llp_cfg_set` in `6.5.22/unix.IP27` at 0xc000000000082328
  **Inferred.** Calls `spl1` at 0xc000000000082338 and `splx` at 0xc000000000082398; the facts do not show which loads and stores lie between them. It reads the kind halfword (`lh` at 0xc000000000082360), the eight-byte field at +0x8 (`ld` at 0xc00000000008236c) and the word at +0x4 (`lw` at 0xc0000000000823b0). Its write is `sw` at 0xc000000000082410 and `sd` at 0xc000000000082434, followed by `lw` at 0xc000000000082418 and `ld` at 0xc00000000008243c, which load from addresses that are unknown. Two `panic` calls at 0xc0000000000824a0 and 0xc0000000000824b4 are the failure exits. The fields written are unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_tflush_get` in `6.5.22/unix.IP27` at 0xc0000000000824c4
  **Inferred.** Begins with the kind halfword at 0xc0000000000824e8 (`lh`, +0x72), the eight-byte field at 0xc0000000000824f4 (`ld`, +0x8) and the word at 0xc000000000082528 (`lw`, +0x4). A further word at 0xc000000000082584 is loaded from an unknown address. Failure goes to `panic` at 0xc000000000082628, and `assfail` at 0xc000000000082644 receives a2 set to 0x30f. The value the return at 0xc0000000000825a0 hands back is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_linkside_err_get` in `6.5.22/unix.IP27` at 0xc000000000082654
  **Inferred.** Stores nothing. It loads the kind halfword (`lh` at 0xc000000000082678), the eight-byte field at +0x8 (`ld` at 0xc000000000082684) and the word at +0x4 (`lw` at 0xc0000000000826bc), plus a second word at `lw` 0xc000000000082718 and doublewords at 0xc000000000082740 and 0xc00000000008275c whose sources the facts do not resolve. The return at 0xc0000000000826b0 yields 0 or an unknown value. The single call is `panic` at 0xc0000000000827a8.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_resp_err_get` in `6.5.22/unix.IP27` at 0xc0000000000827b8
  **Inferred.** Its three field reads are `lh` at 0xc0000000000827dc (kind halfword, +0x72), `ld` at 0xc0000000000827e8 (+0x8) and `lw` at 0xc000000000082824 (+0x4). The return at 0xc000000000082818 gives 0 or an unknown value. The one call is `panic` at 0xc00000000008291c.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_resp_err_addr_get` in `6.5.22/unix.IP27` at 0xc00000000008292c
  **Inferred.** Reads the kind halfword at 0xc000000000082950, the eight-byte field at 0xc00000000008295c and the word at 0xc000000000082994. A doubleword from an unknown address is loaded at `ld` 0xc000000000082a24, the return at 0xc000000000082988 gives 0 or an unknown value, and `panic` at 0xc000000000082a98 is the failure call. The address held is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_resp_err_buf_get` in `6.5.22/unix.IP27` at 0xc000000000082aa8
  **Inferred.** `lh` at 0xc000000000082acc reads the kind halfword, `ld` at 0xc000000000082ad8 the eight-byte field at +0x8, and `lw` at 0xc000000000082b14 the word at +0x4. Two later loads, `lw` at 0xc000000000082b70 and `ld` at 0xc000000000082b9c, come from unknown addresses. The return at 0xc000000000082b08 gives 0 or an unknown value, and `panic` at 0xc000000000082c0c is the exit on failure. The buffer contents are unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_resp_err_dev_get` in `6.5.22/unix.IP27` at 0xc000000000082c1c
  **Inferred.** Kind halfword from `lh` at 0xc000000000082c40, eight-byte field from `ld` at 0xc000000000082c4c, word from `lw` at 0xc000000000082c88. Loads at `lw` 0xc000000000082ce4 and `ld` 0xc000000000082d10 read unknown addresses. The return at 0xc000000000082c7c hands back 0 or an unknown value, and the `panic` at 0xc000000000082d80 is the only call. Which device the value names is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_linkside_err_addr_get` in `6.5.22/unix.IP27` at 0xc000000000082d90
  **Inferred.** Stores nothing. After `lh` at 0xc000000000082db4 (kind halfword), `ld` at 0xc000000000082dc0 (+0x8) and `lw` at 0xc000000000082dfc (+0x4), the return at 0xc000000000082df0 yields 0 or an unknown value. Further loads at 0xc000000000082e58 and 0xc000000000082e74 are doublewords from addresses the facts do not resolve. Two `panic` calls sit at 0xc000000000082ebc and 0xc000000000082edc, and the second has a5 as 0x24 in the facts. The address field's layout is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_dirmap_get` in `6.5.22/unix.IP27` at 0xc000000000082eec
  **Inferred.** Loads the kind halfword at 0xc000000000082f10, the eight-byte field at 0xc000000000082f1c and the word at 0xc000000000082f58. A second word at 0xc000000000082fb4 is read from an unknown address. The return at 0xc000000000082f4c yields 0 or an unknown value, and the only call is `panic` at 0xc000000000083044. The mapping's fields are unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_dirmap_set` in `6.5.22/unix.IP27` at 0xc000000000083054
  **Inferred.** Reads the kind halfword (`lh` at 0xc000000000083078), the eight-byte field (`ld` at 0xc000000000083084) and the word at +0x4 (`lw` at 0xc0000000000830b8). It stores with `sw` at 0xc000000000083118 and `sd` at 0xc000000000083134 and 0xc000000000083150. The return at 0xc0000000000830ac yields 0x3, a panic result or an unknown value, and `panic` at 0xc00000000008319c takes a1 as its second argument. The direct-mapping fields written are unknown.
  Reviewed: 2026-11-12 (sonnet)
