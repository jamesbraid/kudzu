# kudzu routine roles, read batch 1

Role statements for routines of the shipped IP27 kernel `6.5.22/unix.IP27`. Each role comes from the rendered instruction facts: calls, argument registers, fixed-offset loads and stores, branch conditions and return values. Every role is inferred. The facts show control flow and callees, but not the meaning of the constants, the stored values or the callees' contracts.

- `xtalk_dmamap_list` in `6.5.22/unix.IP27` at 0xc00000000006d948
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmamap_list` (target 0xc00000000003b8cc) through the call at 0xc00000000006d950, and returns that call's v0 at 0xc00000000006d95c. It adds no checks or state of its own, so the operation is whatever `hub_dmamap_list` does, which these facts do not show.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmamap_done` in `6.5.22/unix.IP27` at 0xc00000000006d964
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmamap_done` (target 0xc00000000003b928) through the call at 0xc00000000006d96c, and returns that call's v0 at 0xc00000000006d978. Like its neighbours at 0xc00000000006d948, it is a pass-through stub whose meaning comes from the callee's name and contract, not from this body.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmatrans_addr` in `6.5.22/unix.IP27` at 0xc00000000006d980
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmatrans_addr` (target 0xc00000000003b978) through the call at 0xc00000000006d988, and returns that call's v0 at 0xc00000000006d994. It is a pass-through stub, so the translation it names is the callee's job, and the callee's body is not part of these facts.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmatrans_list` in `6.5.22/unix.IP27` at 0xc00000000006d99c
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmatrans_list` (target 0xc00000000003b980) through the call at 0xc00000000006d9a4, and returns that call's v0 at 0xc00000000006d9b0. It adds nothing of its own, so the list translation is the callee's behaviour, not something this body shows.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmamap_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9b8
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmamap_drain` (target 0xc00000000003b988) through the call at 0xc00000000006d9c0, and returns that call's v0 at 0xc00000000006d9cc. It is a pass-through stub, so the drain's effect is the callee's and is not shown by this body.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmaaddr_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9d4
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmaaddr_drain` (target 0xc00000000003b990) through the call at 0xc00000000006d9dc, and returns that call's v0 at 0xc00000000006d9e8. It is a pass-through stub, so the drain's effect is the callee's and is not shown by this body.
  Reviewed: 2026-11-12 (sonnet)

- `xtalk_dmalist_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9f0
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmalist_drain` (target 0xc00000000003b998) through the call at 0xc00000000006d9f8, and returns that call's v0 at 0xc00000000006da04. It is a pass-through stub, so the drain's effect is the callee's and is not shown by this body.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_init` in `6.5.22/unix.IP27` at 0xc00000000006e658
  **Inferred.** Makes three calls to `xwidget_driver_register` (target 0xc00000000006df78) at 0xc00000000006e66c, 0xc00000000006e680 and 0xc00000000006e694, each with a2 = 0xc0000000003c6098 and a3 = 1, and first arguments 0xd100, 0xd000 and 0. The routine reads as a setup step that registers three crossbar widget drivers, with the first argument selecting each one; the meaning of those three constants is not shown by these facts. Its return at 0xc00000000006e6a0 is the last registration call's v0.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_widget_num_get` in `6.5.22/unix.IP27` at 0xc00000000006ea4c
  **Inferred.** Calls `vertex_to_name` (0xc00000000029798c) at 0xc00000000006ea60 with a1 = a stack buffer and a2 = 0x100, then `nodevertex_to_cnodeid` (0xc000000000297ca4) at 0xc00000000006ea68. When that result is not -1 the path calls `hub_widget_id` (0xc00000000001d41c) at 0xc00000000006ea8c with a0 = table(0xc0000000014d3800), followed by a halfword load at offset 0 at 0xc00000000006ea90. Otherwise it calls `strstr` (0xc0000000001464f8) on the buffer at 0xc00000000006eab4 and 0xc00000000006eac8 against the strings at 0xc0000000003c6180 and 0xc0000000003c6170; a zero result from either branches away to 0xc00000000006eb04, and when both are nonzero it calls `hwgraph_edge_get` (0xc000000000295e10) at 0xc00000000006eaec with a1 = 0xc0000000003c6184. It also calls `xwidget_info_chk` (0xc00000000006dc84) at 0xc00000000006eb20 and, when that result is nonzero, `xwidget_info_id_get` (0xc00000000006dd94) at 0xc00000000006eb38. The return is one of the `hub_widget_id` result, 0xff and the `xwidget_info_id_get` result; the facts do not show which condition yields 0xff. Role (from name): turns a crossbar vertex into a widget number, with the name matching inferred from the call pattern.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006eb48
  **Inferred.** Dispatches on the command in a1, comparing it against 0x3, 0x4, 0x5, 0x6 and 0x6702 at 0xc00000000006eb74 through 0xc00000000006ebd4 and 0xc00000000006eb9c. The arms call `xbow_get_perf_counters`, `xbow_get_llp_status`, `xbow_disable_llp_monitor`, `xbow_enable_perf_counter`, `xbow_enable_llp_monitor` and `xbow_prio_bw_alloc`, and move argument blocks of 0xc, 0x10, 0x80 and 0x100 bytes with `copyin` and `copyout`. Capability checks (`cap_able_cred`, `cap_able`) guard those arms, and two paths call `assfail`. The command-to-meaning mapping is inferred from the callee names and the constants, not from a stated interface.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_get_perf_counters` in `6.5.22/unix.IP27` at 0xc000000000070118
  **Inferred.** Calls `hwgraph_fastinfo_get` (0xc000000000296ee8) with its own four argument registers at 0xc000000000070120, and returns that result plus 0x40 at 0xc000000000070130. It hands back a pointer to a block at offset 0x40 inside the vertex's fast-info record, which the name suggests is the performance-counter area; the layout of that block is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_get_llp_status` in `6.5.22/unix.IP27` at 0xc000000000070304
  **Inferred.** Calls `hwgraph_fastinfo_get` (0xc000000000296ee8) at 0xc00000000007030c, and returns that result plus 0x140 at 0xc00000000007031c. It hands back a pointer to a block at offset 0x140 inside the vertex's fast-info record, which the name suggests is the link-protocol status area; the layout is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `xbow_disable_llp_monitor` in `6.5.22/unix.IP27` at 0xc000000000070438
  **Inferred.** Calls `hwgraph_fastinfo_get` (0xc000000000296ee8) at 0xc000000000070440 with its own four argument registers, then stores 8-byte values at 0xc000000000070464 and 0xc00000000007046c, where the branch at 0xc000000000070468 goes back to 0xc000000000070454 while a counter is below 8, so those stores are part of a loop. A 4-byte store at offset 0x1c4 follows at 0xc000000000070474, and the routine returns 0 at 0xc000000000070478. The facts do not name the base register of the stores or show the stored values, so the clear-to-disable reading comes from the name only.
  Reviewed: 2026-11-12 (sonnet)

- `pciiox_vmc` in `6.5.22/unix.IP27` at 0xc000000000072734
  **Inferred.** Calls `pcibr_hints_fix_rrbs` (0xc0000000000797b8) at 0xc000000000072740, then calls `xtalk_piotrans_addr` (0xc00000000006d7b0) at 0xc000000000072758 with a1 = 0, a2 = 0, a3 = 0x1000000 (16 MiB) and a4 = 0. It makes 4-byte stores at offsets 0x284 and 0x28c at 0xc000000000072774 and 0xc000000000072780; the facts do not name their base register, so that it is the returned mapping is not shown. It returns the `xtalk_piotrans_addr` result at 0xc00000000007277c. The purpose of the two written words is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_release_device` in `6.5.22/unix.IP27` at 0xc000000000072c28
  **Inferred.** Takes the lock at a0 + 0x80 with `mutex_spinlock` (0xc000000000287214) at 0xc000000000072c48 and releases it with `mutex_io_spinunlock` (0xc000000000287120) at 0xc000000000072cc4. In between, each of three 4-byte fields at offsets 0x144, 0x148 and 0x14c of the slot record at a0 + (a1 << 7) is loaded and stored only under a condition on a2: a2 equal to 0x1100000 for offset 0x144, 0x8f8000 for 0x148 and 0x898000 for 0x14c (branches at 0xc000000000072c7c, 0xc000000000072c90 and 0xc000000000072ca4). The name suggests it clears a device's slot state; the stored values are not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_piomap_alloc` in `6.5.22/unix.IP27` at 0xc000000000074058
  **Inferred.** Calls `pcibr_info_get` (0xc00000000007306c) at 0xc000000000074094 and `pciio_info_mfast_get` (0xc000000000094ec4) at 0xc0000000000740a8; zero results at the branches at 0xc0000000000740b4 and 0xc0000000000740bc go to 0xc000000000074268. Otherwise it calls `pcibr_addr_pci_to_xio` (0xc000000000073788) at 0xc0000000000740e0, and a result of -1 branches to 0xc000000000074284. It then takes the device lock with `mutex_spinlock` at 0xc000000000074104, loads the doubleword at offset 0x210 of the bridge info and loops over further doubleword loads, and releases the lock at 0xc000000000074138, before allocating a 0x60-byte record with `kmem_zalloc` (0xc000000000181ce0) at 0xc000000000074144. Further `mutex_spinlock` and `mutex_io_spinunlock` calls sit at 0xc0000000000741a8 and 0xc0000000000741cc, and the call to `xtalk_piomap_alloc` (0xc00000000006d740) at 0xc0000000000741ec is not shown to be made under the first lock, which was released at 0xc000000000074138. The two `pcibr_debug` calls at 0xc000000000074274 and 0xc000000000074290 lie after the branch targets 0xc000000000074268 and 0xc000000000074284, and the facts do not show which condition reaches them. The return is 0 or an unshown value.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_reset` in `6.5.22/unix.IP27` at 0xc000000000075fcc
  **Inferred.** Calls `pciio_hostinfo_get` at 0xc000000000075fd8 and `pciio_info_mfast_get` at 0xc000000000075fec, then compares bounds from the info record (branches at 0xc000000000076028 and 0xc000000000076038, which can skip ahead to 0xc000000000076208). On the main path it takes the lock at the info record + 0x80 with `mutex_spinlock` at 0xc00000000007604c, calls `pcibr_func_config_get` (0xc00000000007dcf4) with a3 = 4 at 0xc0000000000760b0, then `pcireg_control_bit_clr` (0xc000000000080c6c) at 0xc0000000000760e0, `nano_delay` (0xc0000000001b5a98) at 0xc0000000000760e8, `pcireg_control_bit_set` (0xc000000000080e18) at 0xc0000000000760f4 and `nano_delay` again at 0xc0000000000760fc. It has call sites for `pcibr_func_config_set` (0xc00000000007dd8c) at 0xc000000000076120 and 0xc0000000000761d4, the first inside a loop, so how many times the config word is written back is not shown. It releases the lock with `mutex_io_spinunlock` at 0xc0000000000761f4 and calls `pcibr_debug` at 0xc000000000076218. The return at 0xc000000000076244 is -1 or 0. The reset sequence is inferred from the call order.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_endian_set` in `6.5.22/unix.IP27` at 0xc000000000076288
  **Inferred.** Gets the device and bridge info (`pciio_hostinfo_get` at 0xc0000000000762a4, `pciio_info_mfast_get` at 0xc0000000000762b4) and takes the slot lock at 0xc0000000000762c4. It loads the 8-byte word at offset 0x138 of the slot record at 0xc0000000000762e0 and compares it with a value held in s0. When they match it unlocks with `mutex_io_spinunlock` at 0xc000000000076304, calls `pcibr_debug` at 0xc000000000076324 and returns a2 at 0xc000000000076340. When they differ it calls `pcireg_device_set` (0xc000000000088124) at 0xc000000000076350 with a4 = 0x180000, then `pcireg_tflush_get` (0xc0000000000824c4), and stores the 8-byte word at offset 0x138 at 0xc000000000076364. The byte-order meaning comes from the name; the bits of 0x180000 are not decoded here.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_priority_bits_set` in `6.5.22/unix.IP27` at 0xc000000000076370
  **Inferred.** Under the slot lock (`mutex_spinlock` at 0xc0000000000763c0, `mutex_io_spinunlock` at 0xc000000000076418), it reads and writes the 8-byte word at offset 0x138 and the 4-byte word at offset 0x140 of the slot record at a0 + (a1 << 7), with the writes at 0xc00000000007644c and 0xc000000000076490 reached through compares of a2 with 1, 2 and 0 and through compares on the word at offset 0x140. The facts do not show a bit test. It then calls `pcireg_device_set` (0xc00000000007645c) and `pcireg_tflush_get` (0xc000000000076468). The priority-bit encoding is inferred from the name; the bits are not decoded here.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_device_flags_set` in `6.5.22/unix.IP27` at 0xc000000000076514
  **Inferred.** Gets the device and bridge info (`pciio_hostinfo_get` at 0xc000000000076528, `pciio_info_mfast_get` at 0xc000000000076540), then tests the sign bits of a1 shifted left by 0x2f through 0x29, 0x25 and 0x24 and the bit 0x800000 of a1 in a chain of branches. It loads the halfword at offset 0x72 at 0xc00000000007660c, before the slot lock is taken with `mutex_spinlock` at 0xc000000000076630. Under the lock it loads the 8-byte word at offset 0x138 at 0xc000000000076644 and compares it with a value in s2. When they match it unlocks with `mutex_io_spinunlock` at 0xc000000000076668, calls `pcibr_debug` at 0xc000000000076688 and returns 1 at 0xc0000000000766ac. When they differ it calls `pcireg_device_set` (0xc0000000000766bc), `pcireg_tflush_get` at 0xc0000000000766cc and stores the 8-byte word at 0xc0000000000766d0. The flags reading comes from the shift pattern and the name; the bit mapping is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_dma_enabled` in `6.5.22/unix.IP27` at 0xc0000000000767a4
  **Inferred.** Gets the device and bridge info with `pciio_hostinfo_get` (0xc000000000094dbc) and `pciio_info_mfast_get` (0xc000000000094ec4), then passes the first word of the info record (offset 0) to `xtalk_dma_enabled` (0xc00000000006dc08) and returns that call's result. The check is a pass-through to the crossbar layer, so the criterion for "enabled" lives in `xtalk_dma_enabled`, which these facts do not show.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_bridge_ptr_get` in `6.5.22/unix.IP27` at 0xc000000000076a54
  **Inferred.** Calls `xtalk_piotrans_addr` (0xc00000000006d7b0) at 0xc000000000076a6c with a1 = a2 = a4 = 0 and a3 = 8, then loads 4-byte words at offset 0x4 at four sites (0xc000000000076a74, 0xc000000000076a94, 0xc000000000076aa8 and 0xc000000000076ad8), with a branch at 0xc000000000076ae8 looping back, and compares the values against 0xd102, 0xd112 and 0x24 and a flag against 1. It returns either the `xtalk_piotrans_addr` result or that result plus 0x800000 (8 MiB) at 0xc000000000076ad0; the facts do not show which comparison selects the offset, or what the compared words mean.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_init_ext_ate_ram` in `6.5.22/unix.IP27` at 0xc000000000076f94
  **Inferred.** In loops (branches at 0xc000000000076fec, 0xc000000000077018 and 0xc000000000077044) it calls `pcireg_ext_ate_set` (0xc00000000008a3ec) at 0xc000000000076fe0 with a2 = 0x0123456789abcdef, calls it again at 0xc000000000077008 with a2 = 0, and `pcireg_ext_ate_get` (0xc00000000008a220) at 0xc000000000077028; the facts do not show whether the read-back is compared. It then calls `pcireg_ssram_size_set` (0xc000000000080fb4) at 0xc00000000007704c. When bit 0x10 of the word at 0xc0000000014c9e10 is set it falls through to a `pcibr_debug` call at 0xc000000000077080; when clear it branches to 0xc000000000077088 past that call. A loop bounded by table(0xc000000001436ee8) then calls `pcireg_ext_ate_set` with a2 = 0 at 0xc0000000000770a0, and the routine returns table(0xc000000001436ee8) at 0xc0000000000770d4. A second `pcibr_debug` call at 0xc0000000000770e4 is reached when that value is 0. The walk reads as a probe and init of the bridge's external address-translation RAM.
  Reviewed: 2026-11-12 (sonnet)

- `pcibr_flags_to_ate` in `6.5.22/unix.IP27` at 0xc000000000077184
  **Inferred.** Tests single bits of its argument: bit 0x10 at 0xc000000000077198 and bit 0x400 at 0xc0000000000771b4, then the sign bits of the argument shifted left by 0x29, 0x28, 0x2d, 0x2c, 0x2b and 0x2a at 0xc0000000000771d4 through 0xc000000000077224. It looks like a flags-to-translation-attribute mapping, but the output bits and the return value are not established by these facts.
  Reviewed: 2026-11-12 (sonnet)
