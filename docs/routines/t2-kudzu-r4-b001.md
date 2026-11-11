# kudzu routine roles, read batch 1

Role statements for routines of the shipped IP27 kernel `6.5.22/unix.IP27`. Each role comes from the rendered instruction facts: calls, argument registers, fixed-offset loads and stores, branch conditions and return values. Every role is inferred. The facts show control flow and callees, but not the meaning of the constants, the stored values or the callees' contracts.

- `xtalk_dmamap_list` in `6.5.22/unix.IP27` at 0xc00000000006d948
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmamap_list` (target 0xc00000000003b8cc) through the call at 0xc00000000006d950, and returns that call's v0 at 0xc00000000006d95c. It adds no checks or state of its own, so the operation is whatever `hub_dmamap_list` does, which these facts do not show.

- `xtalk_dmamap_done` in `6.5.22/unix.IP27` at 0xc00000000006d964
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmamap_done` (target 0xc00000000003b928) through the call at 0xc00000000006d96c, and returns that call's v0 at 0xc00000000006d978. Like its neighbours at 0xc00000000006d948, it is a pass-through stub whose meaning comes from the callee's name and contract, not from this body.

- `xtalk_dmatrans_addr` in `6.5.22/unix.IP27` at 0xc00000000006d980
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmatrans_addr` (target 0xc00000000003b978) through the call at 0xc00000000006d988, and returns that call's v0 at 0xc00000000006d994. It is a pass-through stub, so the translation it names is the callee's job, and the callee's body is not part of these facts.

- `xtalk_dmatrans_list` in `6.5.22/unix.IP27` at 0xc00000000006d99c
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmatrans_list` (target 0xc00000000003b980) through the call at 0xc00000000006d9a4, and returns that call's v0 at 0xc00000000006d9b0. It adds nothing of its own, so the list translation is the callee's behaviour, not something this body shows.

- `xtalk_dmamap_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9b8
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmamap_drain` (target 0xc00000000003b988) through the call at 0xc00000000006d9c0, and returns that call's v0 at 0xc00000000006d9cc. It is a pass-through stub, so the drain's effect is the callee's and is not shown by this body.

- `xtalk_dmaaddr_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9d4
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmaaddr_drain` (target 0xc00000000003b990) through the call at 0xc00000000006d9dc, and returns that call's v0 at 0xc00000000006d9e8. It is a pass-through stub, so the drain's effect is the callee's and is not shown by this body.

- `xtalk_dmalist_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9f0
  **Inferred.** Forwards its four argument registers unchanged to `hub_dmalist_drain` (target 0xc00000000003b998) through the call at 0xc00000000006d9f8, and returns that call's v0 at 0xc00000000006da04. It is a pass-through stub, so the drain's effect is the callee's and is not shown by this body.

- `xbow_init` in `6.5.22/unix.IP27` at 0xc00000000006e658
  **Inferred.** Makes three calls to `xwidget_driver_register` (target 0xc00000000006df78) at 0xc00000000006e66c, 0xc00000000006e680 and 0xc00000000006e694, each with a2 = 0xc0000000003c6098 and a3 = 1, and first arguments 0xd100, 0xd000 and 0. The routine reads as a setup step that registers three crossbar widget drivers, with the first argument selecting each one; the meaning of those three constants is not shown by these facts. Its return at 0xc00000000006e6a0 is the last registration call's v0.

- `xbow_widget_num_get` in `6.5.22/unix.IP27` at 0xc00000000006ea4c
  **Inferred.** Reads a halfword from table(0xc0000000014d3800) at 0xc00000000006ea90, then turns a crossbar vertex into a widget number by name matching: it calls `vertex_to_name` (0xc00000000029798c) into a 256-byte stack buffer, `strstr` (0xc0000000001464f8) twice against the strings at 0xc0000000003c6180 and 0xc0000000003c6170, and `hwgraph_edge_get` (0xc000000000295e10). The fallbacks are `hub_widget_id` (0xc00000000001d41c) and `xwidget_info_id_get` (0xc00000000006dd94), and the function returns 0xff when none matches. The name-matching rule is inferred from the call pattern and the strings' use, not from a stated contract.

- `xbow_ioctl` in `6.5.22/unix.IP27` at 0xc00000000006eb48
  **Inferred.** Dispatches on the command in a1, comparing it against 0x3, 0x4, 0x5, 0x6 and 0x6702 at 0xc00000000006eb74 through 0xc00000000006ebd4 and 0xc00000000006eb9c. The arms call `xbow_get_perf_counters`, `xbow_get_llp_status`, `xbow_disable_llp_monitor`, `xbow_enable_perf_counter`, `xbow_enable_llp_monitor` and `xbow_prio_bw_alloc`, and move argument blocks of 0xc, 0x10, 0x80 and 0x100 bytes with `copyin` and `copyout`. Capability checks (`cap_able_cred`, `cap_able`) guard those arms, and two paths call `assfail`. The command-to-meaning mapping is inferred from the callee names and the constants, not from a stated interface.

- `xbow_get_perf_counters` in `6.5.22/unix.IP27` at 0xc000000000070118
  **Inferred.** Calls `hwgraph_fastinfo_get` (0xc000000000296ee8) with its own four argument registers at 0xc000000000070120, and returns that result plus 0x40 at 0xc000000000070130. It hands back a pointer to a block at offset 0x40 inside the vertex's fast-info record, which the name suggests is the performance-counter area; the layout of that block is not shown.

- `xbow_get_llp_status` in `6.5.22/unix.IP27` at 0xc000000000070304
  **Inferred.** Calls `hwgraph_fastinfo_get` (0xc000000000296ee8) at 0xc00000000007030c, and returns that result plus 0x140 at 0xc00000000007031c. It hands back a pointer to a block at offset 0x140 inside the vertex's fast-info record, which the name suggests is the link-protocol status area; the layout is not shown.

- `xbow_disable_llp_monitor` in `6.5.22/unix.IP27` at 0xc000000000070438
  **Inferred.** Calls `hwgraph_fastinfo_get` (0xc000000000296ee8) at 0xc000000000070440, then stores 8-byte values at 0xc000000000070464 and 0xc00000000007046c and a 4-byte value at offset 0x1c4 of the returned record at 0xc000000000070474, and returns 0. The stored values are not shown, so the clear-to-disable reading comes from the name only.

- `pciiox_vmc` in `6.5.22/unix.IP27` at 0xc000000000072734
  **Inferred.** Calls `pcibr_hints_fix_rrbs` (0xc0000000000797b8) at 0xc000000000072740, then maps a 0x1000000-byte (16 MiB) window with `xtalk_piotrans_addr` (0xc00000000006d7b0) at 0xc000000000072758, using a1 = 0, a2 = 0 and a4 = 0. It writes 4-byte values at offsets 0x284 and 0x28c of the mapped window at 0xc000000000072774 and 0xc000000000072780, and returns the mapping's address. The purpose of the two written words is not shown.

- `pcibr_release_device` in `6.5.22/unix.IP27` at 0xc000000000072c28
  **Inferred.** Under the bridge lock at a0 + 0x80, taken with `mutex_spinlock` (0xc000000000287214) at 0xc000000000072c48 and released with `mutex_io_spinunlock` (0xc000000000287120) at 0xc000000000072cc4, it reads and writes three 4-byte fields at offsets 0x144, 0x148 and 0x14c of the slot record at a0 + (a1 << 7). The name suggests it clears a device's slot state; the stored values are not shown.

- `pcibr_piomap_alloc` in `6.5.22/unix.IP27` at 0xc000000000074058
  **Inferred.** Resolves the bridge info with `pcibr_info_get` (0xc00000000007306c) and `pciio_info_mfast_get` (0xc000000000094ec4), converts the PCI address with `pcibr_addr_pci_to_xio` (0xc000000000073788), and allocates a 0x60-byte record with `kmem_zalloc` at 0xc000000000074144. It then takes the device lock, calls `xtalk_piomap_alloc` (0xc00000000006d740) at 0xc0000000000741ec, and releases the lock. Two `pcibr_debug` calls at 0xc000000000074274 and 0xc000000000074290 run on the error or trace paths, and the return is 0 or a value from the allocation.

- `pcibr_reset` in `6.5.22/unix.IP27` at 0xc000000000075fcc
  **Inferred.** Reads the device and bridge info through `pciio_hostinfo_get` and `pciio_info_mfast_get`, then under the lock at 0xc00000000007604c it reads the config word with `pcibr_func_config_get` (0xc00000000007dcf4) and clears and sets a control bit with `pcireg_control_bit_clr` (0xc000000000080c6c) and `pcireg_control_bit_set` (0xc000000000080e18). `nano_delay` (0xc0000000001b5a98) runs between the two steps, and `pcibr_func_config_set` (0xc00000000007dd8c) writes the config word back twice. The return is 0 or -1 at 0xc000000000076244. The reset sequence is inferred from the call order.

- `pcibr_endian_set` in `6.5.22/unix.IP27` at 0xc000000000076288
  **Inferred.** Gets the device and bridge info (`pciio_hostinfo_get` at 0xc0000000000762a4, `pciio_info_mfast_get` at 0xc0000000000762b4) and takes the slot lock at 0xc0000000000762c4. It loads and stores the 8-byte word at offset 0x138 of the slot record at 0xc0000000000762e0 and 0xc000000000076364, and programs the device register with `pcireg_device_set` (0xc000000000088124) using mask 0x180000 at 0xc000000000076350, then flushes with `pcireg_tflush_get` (0xc0000000000824c4). The byte-order meaning comes from the name; the mask bits are not decoded here.

- `pcibr_priority_bits_set` in `6.5.22/unix.IP27` at 0xc000000000076370
  **Inferred.** Under the slot lock (`mutex_spinlock` at 0xc0000000000763c0, `mutex_io_spinunlock` at 0xc000000000076418), it reads and writes the 8-byte word at offset 0x138 and the 4-byte word at offset 0x140 of the slot record at a0 + (a1 << 7), with the write at 0xc00000000007644c and 0xc000000000076490 depending on a bit test. It then calls `pcireg_device_set` (0xc00000000007645c) and `pcireg_tflush_get` (0xc000000000076468). The priority-bit encoding is inferred from the name; the bits are not decoded here.

- `pcibr_device_flags_set` in `6.5.22/unix.IP27` at 0xc000000000076514
  **Inferred.** Gets the device and bridge info, then under the slot lock (`mutex_spinlock` at 0xc000000000076630, with arguments built from shifts of a1 by 0x2c, 0x2b and 0x25) reads the halfword at offset 0x72 and the 8-byte word at offset 0x138, and writes the 8-byte word back at 0xc0000000000766d0. It programs the device register with `pcireg_device_set` (0xc0000000000766bc), flushes with `pcireg_tflush_get`, and returns 1 at 0xc0000000000766ac. The flags reading comes from the shift pattern and the name; the bit mapping is not shown.

- `pcibr_dma_enabled` in `6.5.22/unix.IP27` at 0xc0000000000767a4
  **Inferred.** Gets the device and bridge info with `pciio_hostinfo_get` (0xc000000000094dbc) and `pciio_info_mfast_get` (0xc000000000094ec4), then passes the first word of the info record (offset 0) to `xtalk_dma_enabled` (0xc00000000006dc08) and returns that call's result. The check is a pass-through to the crossbar layer, so the criterion for "enabled" lives in `xtalk_dma_enabled`, which these facts do not show.

- `pcibr_bridge_ptr_get` in `6.5.22/unix.IP27` at 0xc000000000076a54
  **Inferred.** Maps an 8-byte-window PIO address with `xtalk_piotrans_addr` (0xc00000000006d7b0) at 0xc000000000076a6c, using a3 = 8 and a1 = a2 = a4 = 0, then reads four 4-byte words at offset 0x4 of the mapping. It returns the mapping address, or that address plus 0x800000 (8 MiB), depending on one of those words at 0xc000000000076ad0. The meaning of the word test is not shown.

- `pcibr_init_ext_ate_ram` in `6.5.22/unix.IP27` at 0xc000000000076f94
  **Inferred.** Walks a table whose count is read from table(0xc000000001436ee8), calling `pcireg_ext_ate_set` (0xc00000000008a3ec) with the constant 0x0123456789abcdef and then with 0, and reading back with `pcireg_ext_ate_get` (0xc00000000008a220). It also calls `pcireg_ssram_size_set` (0xc000000000080fb4), and skips the test write when bit 0x10 of the word at 0xc0000000014c9e10 is set. The walk reads as a probe and init of the bridge's external address-translation RAM; whether the read-back is compared is not shown.

- `pcibr_flags_to_ate` in `6.5.22/unix.IP27` at 0xc000000000077184
  **Inferred.** Tests single bits of its argument: bit 0x10 at 0xc000000000077198 and bit 0x400 at 0xc0000000000771b4, then the sign bits of the argument shifted left by 0x29, 0x28, 0x2d, 0x2c, 0x2b and 0x2a at 0xc0000000000771d4 through 0xc000000000077224. It looks like a flags-to-translation-attribute mapping, but the output bits and the return value are not established by these facts.
