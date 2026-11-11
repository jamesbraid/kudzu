# Tier-2 routine readings, kudzu batch 4

Twenty-five `pcireg_*` accessors from the 6.5.22 IP27 unix build, read from their rendered facts. The name and the shape of each routine (a record pointer in a0, a kind halfword at +0x72, a selector chain, and a read-modify-write under an interrupt-level bracket) support a reading of PCI bridge register access. The facts never name the registers, so each role is inferred.

- `pcireg_control_bit_clr` in `6.5.22/unix.IP27` at 0xc000000000080c6c
  **Inferred.** Takes a record pointer in a0 and branches to the panic path at 0xc000000000080e04 when a0 is zero (`beq` at 0xc000000000080c8c). The kind halfword at +0x72 is loaded at 0xc000000000080ca8, and the code then branches on kind values 3, 2, 1, 0 and -1 (`beq` at 0xc000000000080cb0 through 0xc000000000080cd4). Inside the read-modify-write, `spl1` at 0xc000000000080c80 and `splx` at 0xc000000000080ce0 bracket the access, and the stores are `sw` at 0xc000000000080d64 and `sd` at 0xc000000000080d8c. The bit cleared is unknown, and so is the register each selector constant addresses.

- `pcireg_control_bit_set` in `6.5.22/unix.IP27` at 0xc000000000080e18
  **Inferred.** The `lh` at 0xc000000000080e54 reads the kind halfword at +0x72, and the `lw` at 0xc000000000080ea4 reads a word at +0x4. The store `sw` at 0xc000000000080f08 writes a word, and the `sd` pair at 0xc000000000080f2c and 0xc000000000080f50 writes doublewords. `spl1` at 0xc000000000080e2c and `splx` at 0xc000000000080e8c frame the update, and `panic` at 0xc000000000080fa4 is the failure exit on one path. The bit it sets is unknown.

- `pcireg_ssram_size_set` in `6.5.22/unix.IP27` at 0xc000000000080fb4
  **Inferred.** Writes two words, `sw` at 0xc0000000000810c8 and `sw` at 0xc0000000000810d8, after reading the kind halfword at +0x72 (`lh` at 0xc000000000080ff0), an eight-byte field at +0x8 (`ld` at 0xc000000000080ffc) and a word at +0x4 (`lw` at 0xc000000000081048). The update sits between `spl1` at 0xc000000000080fc8 and `splx` at 0xc000000000081030. Three failure calls leave the routine: `panic` at 0xc000000000081158, `panic` at 0xc00000000008116c and `assfail` at 0xc000000000081184. The size encoding is unknown.

- `pcireg_speed_get` in `6.5.22/unix.IP27` at 0xc0000000000811a4
  **Inferred.** Reads a word at `lw` 0xc0000000000812cc in addition to the kind halfword (`lh` at 0xc0000000000811c8), the eight-byte field at +0x8 (`ld` at 0xc0000000000811d4) and the word at +0x4 (`lw` at 0xc000000000081210). The return at 0xc000000000081204 hands back 0 or a value that is unknown. The `jal` at 0xc00000000008130c calls `panic` with a4 set to -1 or 0xe010. The unit of the returned value is unknown.

- `pcireg_mode_get` in `6.5.22/unix.IP27` at 0xc00000000008131c
  **Inferred.** A read path with no stores: the kind halfword sits at +0x72 (`lh` at 0xc000000000081340), the eight-byte field at +0x8 (`ld` at 0xc00000000008134c) and the word at +0x4 (`lw` at 0xc000000000081384). The return at 0xc000000000081378 gives 0 or a value that is unknown. `panic` at 0xc000000000081478 is reached when a4 is -1, 0x24 or 0xe010. The mode value's bits are unknown.

- `pcireg_req_timeout_get` in `6.5.22/unix.IP27` at 0xc000000000081488
  **Inferred.** Three field reads open the routine: `lh` at 0xc0000000000814ac for the kind halfword at +0x72, `ld` at 0xc0000000000814b8 for +0x8 and `lw` at 0xc0000000000814f0 for +0x4. A second word load at 0xc00000000008154c comes from an address that is unknown. The only call is `panic` at 0xc0000000000815dc. The timeout's unit is unknown.

- `pcireg_req_timeout_set` in `6.5.22/unix.IP27` at 0xc0000000000815ec
  **Inferred.** Reads the kind halfword (`lh` at 0xc000000000081610) and the word at +0x4 (`lw` at 0xc000000000081654), then writes a word with `sw` at 0xc0000000000816b4 and a doubleword with `sd` at 0xc0000000000816d0 or 0xc0000000000816ec. The `panic` at 0xc000000000081738 receives a1 as its second argument, so the caller's second argument is carried into the diagnostic. The value written is unknown.

- `pcireg_intr_dst_get` in `6.5.22/unix.IP27` at 0xc000000000081748
  **Inferred.** Pulls the kind halfword from +0x72 (`lh` at 0xc00000000008176c), the eight-byte field at +0x8 (`ld` at 0xc000000000081778) and the word at +0x4 (`lw` at 0xc0000000000817b4). The return at 0xc0000000000817a8 yields 0 or an unknown value, and `panic` at 0xc0000000000818ac is the only call. The encoding of the destination is unknown.

- `pcireg_intr_dst_set` in `6.5.22/unix.IP27` at 0xc0000000000818bc
  **Inferred.** Loads the kind halfword (`lh` at 0xc0000000000818e0), the eight-byte field at +0x8 (`ld` at 0xc0000000000818ec) and the word at +0x4 (`lw` at 0xc000000000081924). It stores words at 0xc000000000081984 and 0xc00000000008198c (`sw`) and doublewords at 0xc0000000000819a8 and 0xc0000000000819c4 (`sd`). The `panic` at 0xc000000000081a10 receives a5 as one of -1 or 0xe010. The destination value is unknown.

- `pcireg_intr_dst_target_id_get` in `6.5.22/unix.IP27` at 0xc000000000081a20
  **Inferred.** Starts with the kind halfword at +0x72 (`lh` at 0xc000000000081a44), then the eight-byte field at +0x8 (`ld` at 0xc000000000081a50) and the word at +0x4 (`lw` at 0xc000000000081a8c). Further loads come from `lw` at 0xc000000000081ae8 and `ld` at 0xc000000000081b0c, and the return at 0xc000000000081a80 gives 0 or an unknown value. `panic` at 0xc000000000081b7c is the failure path. The identifier's bit layout is unknown.

- `pcireg_intr_dst_target_id_set` in `6.5.22/unix.IP27` at 0xc000000000081b8c
  **Inferred.** Reads the kind halfword (`lh` at 0xc000000000081bb0), the eight-byte field at +0x8 (`ld` at 0xc000000000081bbc) and the word at +0x4 (`lw` at 0xc000000000081bf4). Writes go out as `sw` at 0xc000000000081c60 and 0xc000000000081c78 and as `sd` at 0xc000000000081c9c and 0xc000000000081cb4. A `panic` at 0xc000000000081d18 takes a1 as its second argument. The identifier value is unknown.

- `pcireg_intr_dst_addr_get` in `6.5.22/unix.IP27` at 0xc000000000081d28
  **Inferred.** The kind halfword at +0x72 is loaded at 0xc000000000081d4c, the eight-byte field at +0x8 at 0xc000000000081d58 and the word at +0x4 at 0xc000000000081d94. A doubleword load at 0xc000000000081e24 reads from an address that is unknown. The `panic` at 0xc000000000081e98 is the only call. The address format is unknown.

- `pcireg_intr_dst_addr_set` in `6.5.22/unix.IP27` at 0xc000000000081ea8
  **Inferred.** After three field reads (`lh` at 0xc000000000081ecc, `ld` at 0xc000000000081ed8, `lw` at 0xc000000000081f10), the routine stores three words (`sw` at 0xc000000000081f78, 0xc000000000081f8c and 0xc000000000081f94) and two doublewords (`sd` at 0xc000000000081fc0 and 0xc000000000081fd8). The unmatched path calls `panic` at 0xc00000000008203c. The address written is unknown.

- `pcireg_cmdword_err_get` in `6.5.22/unix.IP27` at 0xc00000000008204c
  **Inferred.** Opens with the kind halfword (`lh` at 0xc000000000082070), the eight-byte field at +0x8 (`ld` at 0xc00000000008207c) and the word at +0x4 (`lw` at 0xc0000000000820b8). A later word load at 0xc000000000082114 supplies the rest, and the return at 0xc0000000000820ac yields 0 or an unknown value. `panic` at 0xc0000000000821a4 closes the failure side. What the command word's error bits mean is unknown.

- `pcireg_llp_cfg_get` in `6.5.22/unix.IP27` at 0xc0000000000821b4
  **Inferred.** Reads the kind halfword at 0xc0000000000821d4 (+0x72), the eight-byte field at 0xc0000000000821e0 (+0x8) and the word at 0xc00000000008221c (+0x4). Two more loads, a word at 0xc000000000082278 and a doubleword at 0xc0000000000822a0, come from addresses that are unknown. The return at 0xc000000000082210 yields 0 or an unknown value, and the calls at 0xc000000000082304 and 0xc000000000082318 both reach `panic`, the second with a5 set to 0xd102. The configuration fields are unknown.

- `pcireg_llp_cfg_set` in `6.5.22/unix.IP27` at 0xc000000000082328
  **Inferred.** Brackets its update with `spl1` at 0xc000000000082338 and `splx` at 0xc000000000082398. It reads the kind halfword (`lh` at 0xc000000000082360), the eight-byte field at +0x8 (`ld` at 0xc00000000008236c) and the word at +0x4 (`lw` at 0xc0000000000823b0). Its write is `sw` at 0xc000000000082410 and `sd` at 0xc000000000082434, followed by `lw` at 0xc000000000082418 and `ld` at 0xc00000000008243c, which load from addresses that are unknown. Two `panic` calls at 0xc0000000000824a0 and 0xc0000000000824b4 are the failure exits. The fields written are unknown.

- `pcireg_tflush_get` in `6.5.22/unix.IP27` at 0xc0000000000824c4
  **Inferred.** Begins with the kind halfword at 0xc0000000000824e8 (`lh`, +0x72), the eight-byte field at 0xc0000000000824f4 (`ld`, +0x8) and the word at 0xc000000000082528 (`lw`, +0x4). A further word at 0xc000000000082584 is loaded from an unknown address. Failure goes to `panic` at 0xc000000000082628, and `assfail` at 0xc000000000082644 receives a2 set to 0x30f. The value the return at 0xc0000000000825a0 hands back is unknown.

- `pcireg_linkside_err_get` in `6.5.22/unix.IP27` at 0xc000000000082654
  **Inferred.** Only reads from the record in a0 and stores nothing: it loads the kind halfword (`lh` at 0xc000000000082678), the eight-byte field at +0x8 (`ld` at 0xc000000000082684) and the word at +0x4 (`lw` at 0xc0000000000826bc), plus a second word at `lw` 0xc000000000082718 whose source the facts do not resolve. The return at 0xc0000000000826b0 yields 0 or an unknown value. The single call is `panic` at 0xc0000000000827a8.

- `pcireg_resp_err_get` in `6.5.22/unix.IP27` at 0xc0000000000827b8
  **Inferred.** Its three field reads are `lh` at 0xc0000000000827dc (kind halfword, +0x72), `ld` at 0xc0000000000827e8 (+0x8) and `lw` at 0xc000000000082824 (+0x4). The return at 0xc000000000082818 gives 0 or an unknown value. The one call is `panic` at 0xc00000000008291c.

- `pcireg_resp_err_addr_get` in `6.5.22/unix.IP27` at 0xc00000000008292c
  **Inferred.** Reads the kind halfword at 0xc000000000082950, the eight-byte field at 0xc00000000008295c and the word at 0xc000000000082994. A doubleword from an unknown address is loaded at `ld` 0xc000000000082a24, the return at 0xc000000000082988 gives 0 or an unknown value, and `panic` at 0xc000000000082a98 is the failure call. The address held is unknown.

- `pcireg_resp_err_buf_get` in `6.5.22/unix.IP27` at 0xc000000000082aa8
  **Inferred.** `lh` at 0xc000000000082acc reads the kind halfword, `ld` at 0xc000000000082ad8 the eight-byte field at +0x8, and `lw` at 0xc000000000082b14 the word at +0x4. Two later loads, `lw` at 0xc000000000082b70 and `ld` at 0xc000000000082b9c, come from unknown addresses. The return at 0xc000000000082b08 gives 0 or an unknown value, and `panic` at 0xc000000000082c0c is the exit on failure. The buffer contents are unknown.

- `pcireg_resp_err_dev_get` in `6.5.22/unix.IP27` at 0xc000000000082c1c
  **Inferred.** Kind halfword from `lh` at 0xc000000000082c40, eight-byte field from `ld` at 0xc000000000082c4c, word from `lw` at 0xc000000000082c88. Loads at `lw` 0xc000000000082ce4 and `ld` 0xc000000000082d10 read unknown addresses. The return at 0xc000000000082c7c hands back 0 or an unknown value, and the `panic` at 0xc000000000082d80 is the only call. Which device the value names is unknown.

- `pcireg_linkside_err_addr_get` in `6.5.22/unix.IP27` at 0xc000000000082d90
  **Inferred.** Only reads from the record in a0 and stores nothing. After `lh` at 0xc000000000082db4 (kind halfword), `ld` at 0xc000000000082dc0 (+0x8) and `lw` at 0xc000000000082dfc (+0x4), the return at 0xc000000000082df0 yields 0 or an unknown value. Two `panic` calls sit at 0xc000000000082ebc and 0xc000000000082edc, and the second sets a5 to one of 0x24 or 0xd102. The address field's layout is unknown.

- `pcireg_dirmap_get` in `6.5.22/unix.IP27` at 0xc000000000082eec
  **Inferred.** Loads the kind halfword at 0xc000000000082f10, the eight-byte field at 0xc000000000082f1c and the word at 0xc000000000082f58. A second word at 0xc000000000082fb4 is read from an unknown address. The return at 0xc000000000082f4c yields 0 or an unknown value, and the sole call, `panic` at 0xc000000000083044, guards the failure exit. The mapping's fields are unknown.

- `pcireg_dirmap_set` in `6.5.22/unix.IP27` at 0xc000000000083054
  **Inferred.** Reads the kind halfword (`lh` at 0xc000000000083078), the eight-byte field (`ld` at 0xc000000000083084) and the word at +0x4 (`lw` at 0xc0000000000830b8). It stores with `sw` at 0xc000000000083118 and `sd` at 0xc000000000083134 and 0xc000000000083150. The return at 0xc0000000000830ac yields 0x3, a panic result or an unknown value, and `panic` at 0xc00000000008319c takes a1 as its second argument. The direct-mapping fields written are unknown.
