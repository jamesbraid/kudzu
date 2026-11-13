# Tier-2 routine readings, kudzu round 5 batch 2

Role statements for entries 50 to 74 of the round 5 ranked list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every grade
is inferred. Where the facts stop short of the meaning, the statement says so.
Addresses are in the binary named in each header.

- `crc16` in `6.5.22/unix.IP27` at 0xc00000000006a654
  **Inferred.** Loads a halfword through a1 [@0xc00000000006a654] and reads two table words [@0xc00000000006a684, @0xc00000000006a688]. It stores a halfword through a1 [@0xc00000000006a698]. Its branch on a value computed from the two table words tests for zero [@0xc00000000006a69c taken zero]. When that value is nonzero [@0xc00000000006a69c fall nonzero] it stores a second halfword through a1 [@0xc00000000006a6a8], and it ends with a last halfword store through a1 [@0xc00000000006a6b8]. The polynomial and the update arithmetic are not in the facts.
  Role (from name): one CRC-16 update step on the halfword at a1, as the name says; the table reads and the stores through a1 are the facts [@0xc00000000006a6b8].
  Reviewed: 2026-11-12

- `nic_get_phase_bits` in `6.5.22/unix.IP27` at 0xc00000000006a6bc
  **Inferred.** Loads eight bytes from the fixed address 0x9000000010000078 [@0xc00000000006a6c4]. The value left in v0 at return is not resolved by these facts.
  Role (from name): derives phase bits from that eight-byte load, as the name says; the single load [@0xc00000000006a6c4] is the only load the facts list.
  Reviewed: 2026-11-12

- `nic_next_scan` in `6.5.22/unix.IP27` at 0xc00000000006a864
  **Inferred.** When the word at a0+20 is zero [@0xc00000000006a878 taken zero], it calls nic_presence [call 1, @0xc00000000006a878 taken]. When nic_presence returns zero [@0xc00000000006a8a0 taken zero], it calls nic_write_byte with 0xf0 in a1 [call 2, @0xc00000000006a8a0 taken]. It calls nic_read [call 4] and nic_read again [call 5].
  Role (from name): steps to the next device on a NIC bus, as the name says; the facts show a presence test, byte writes and reads, and the fields at a0+16 and a0+24 compared at 0xc00000000006a9c4.
  Reviewed: 2026-11-12

- `nic_read_mfg` in `6.5.22/unix.IP27` at 0xc00000000006aecc
  **Inferred.** When the byte at a1 is 0x9 [call 1, @0xc00000000006af10 fall], it reads a page with nic_read_one_page. When the byte at a1 is not 0x9 [@0xc00000000006af10 taken], it checks the ROM image with nic_match_rom [call 2, @0xc00000000006af10 taken]. The facts do not show what the later writes and CRC calls do.
  Role (from name): reads the manufacturing block from a NIC's ROM and checks it, as the name says; the page reads and the ROM match are the facts.
  Reviewed: 2026-11-12

- `chars2print` in `6.5.22/unix.IP27` at 0xc00000000006b2ec
  **Inferred.** Measures the string at a0 with strlen [call 1]. It loads one byte from a1 [@0xc00000000006b3f4] and stores bytes to a destination it does not name [@0xc00000000006b358]. It proceeds only when a2 is positive [@0xc00000000006b334 fall], and it compares bytes against 0x3a, 0x3b and 0xff [@0xc00000000006b374 taken].
  Role (from name): appends up to a2 bytes of a1 to the string at a0 as printable characters, as the name says; the facts show the length scan and the byte compares but not the output format.
  Reviewed: 2026-11-12

- `nic_eaddr` in `6.5.22/unix.IP27` at 0xc00000000006b3f8
  **Inferred.** When a2 is nonzero [call 1, @0xc00000000006b40c fall], it sets up the NIC with nic_setup. When a2 is zero [call 4, @0xc00000000006b40c taken], it calls printf. It steps the device list with nic_next [call 5]. It reads bytes with nic_read_byte [call 9] and runs crc16 over them in two passes [call 10, call 11], testing the 16-bit result against 0xb001 after each pass [@0xc00000000006b584 fall] [@0xc00000000006b5c4 fall].
  Role (from name): reads the station Ethernet address, as the name says; the setup, the device walk and the CRC checks are the facts, but the address bytes are not identified.
  Reviewed: 2026-11-12

- `nic_access_mcr32` in `6.5.22/unix.IP27` at 0xc00000000006b66c
  **Inferred.** It stores a word through a0 [@0xc00000000006b680]. It loads the word at a0 [@0xc00000000006b688] and tests bit 0x2 of it [@0xc00000000006b690 taken]. When that bit is set, it waits with us_delay [call 1, @0xc00000000006b690 taken].
  Role (from facts): polls the register at a0 for bit 0x2 and delays when the bit is set [call 1, @0xc00000000006b690 taken].
  Reviewed: 2026-11-12

- `nic_info_get` in `6.5.22/unix.IP27` at 0xc00000000006bc4c
  **Inferred.** It stores one byte through a2 [@0xc00000000006bc54]. It calls nic_setup [call 1]. When nic_setup returns a nonnegative value, it calls nic_mfg_next [call 2, @0xc00000000006bc78 fall] and repeats that call while it returns zero [@0xc00000000006bc8c taken zero].
  Role (from facts): runs nic_setup and, when setup succeeds, calls nic_mfg_next until it returns nonzero [call 2, @0xc00000000006bc78 fall].
  Reviewed: 2026-11-12

- `nic_vertex_info_set` in `6.5.22/unix.IP27` at 0xc00000000006bcd0
  **Inferred.** It calls nic_vertex_info_get [call 1] and returns its result when that is nonzero [@0xc00000000006bcf0 fall nonzero]. When it is zero [@0xc00000000006bcf0 taken zero], it allocates a buffer with kern_malloc [call 2, @0xc00000000006bcf0 taken] and fills it with nic_info_get [call 3, @0xc00000000006bd18 taken]. It adds a label with hwgraph_info_add_LBL [call 6, @0xc00000000006bd18 taken]. It reads the label back with nic_vertex_info_get [call 7, @0xc00000000006bd18 taken]. When the read-back differs from the buffer [@0xc00000000006bd80 fall], it frees the buffer with kern_free [call 8, @0xc00000000006bd80 fall]. Otherwise it exports the label with hwgraph_info_export_LBL [call 9, @0xc00000000006bd80 taken] and calls nic_vmc_check [call 10, @0xc00000000006bd80 taken].
  Role (from facts): adds a label built from the NIC info through nic_info_get [call 3, @0xc00000000006bd18 taken] when nic_vertex_info_get finds none [call 1, @0xc00000000006bcf0 taken].
  Reviewed: 2026-11-12

- `nic_vertex_info_match` in `6.5.22/unix.IP27` at 0xc00000000006bddc
  **Inferred.** It looks up the vertex info with nic_vertex_info_get [call 1]. When that lookup returns nonzero [@0xc00000000006bdec taken], it searches the result for the string in a1 with strstr [call 2, @0xc00000000006bdec taken].
  Role (from facts): tests whether the vertex info contains the string in a1 [call 2, @0xc00000000006bdec taken].
  Reviewed: 2026-11-12

- `nic_hub_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf1c
  **Inferred.** It calls nic_vertex_info_set [call 1] with a0 = 0xc00000000006be18 and a1 = 0. It returns the result of nic_vertex_info_set [@0xc00000000006bf38].
  Role (from facts): a wrapper of nic_vertex_info_set [call 1] that fixes its first argument to 0xc00000000006be18.
  Reviewed: 2026-11-12

- `nic_bridge_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf40
  **Inferred.** It calls nic_vertex_info_set [call 1] with a0 = 0xc00000000006b66c. It returns the result of nic_vertex_info_set [@0xc00000000006bf58].
  Role (from facts): a wrapper of nic_vertex_info_set [call 1] that fixes its first argument to 0xc00000000006b66c.
  Reviewed: 2026-11-12

- `nic_hq4_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf60
  **Inferred.** It calls nic_vertex_info_set [call 1] with a0 = 0xc00000000006b66c. It returns the result of nic_vertex_info_set [@0xc00000000006bf78].
  Role (from facts): a wrapper of nic_vertex_info_set [call 1] that fixes its first argument to 0xc00000000006b66c.
  Reviewed: 2026-11-12

- `nic_ioc3_vertex_info` in `6.5.22/unix.IP27` at 0xc00000000006bf80
  **Inferred.** When a2 is nonzero [@0xc00000000006bf8c fall nonzero], it stores a word through a2 [@0xc00000000006bf94]. It calls nic_vertex_info_set [call 1] with a0 = 0xc00000000006b66c. It returns the result of nic_vertex_info_set [@0xc00000000006bfa8].
  Role (from facts): a wrapper of nic_vertex_info_set [call 1] that also writes a word through a2 when a2 is nonzero [@0xc00000000006bf8c fall nonzero].
  Reviewed: 2026-11-12

- `nic_vmc_check` in `6.5.22/unix.IP27` at 0xc00000000006c014
  **Inferred.** When the eight-byte word at 0xc0000000014c9da0 is nonzero [@0xc00000000006c038 taken nonzero], it walks a chain of records with eight-byte loads [@0xc00000000006c058]. It searches with strstr [call 1] and calls through a loaded pointer with a0 at entry [call 2].
  Role (from facts): walks a chain of records, searching with strstr [call 1] and calling through a loaded pointer [call 2].
  Reviewed: 2026-11-12

- `check_router` in `6.5.22/unix.IP27` at 0xc00000000006cc24
  **Inferred.** It gets the device info with device_info_get [call 1]. When the result is 1 [@0xc00000000006cc50 taken], it calls cap_able_cred [call 2, @0xc00000000006cc50 taken]. When cap_able_cred returns nonzero [@0xc00000000006cc80 fall nonzero], it looks up a label with hwgraph_info_get_LBL [call 3, @0xc00000000006cc80 fall]. It returns either -0x1 or 0x0 [@0xc00000000006cc68].
  Role (from name): checks that a router device is usable, as the name says; the device-info test, the capability check and the label lookup are the facts.
  Reviewed: 2026-11-12

- `do_router_op` in `6.5.22/unix.IP27` at 0xc00000000006cd98
  **Inferred.** It reads the halfword at a0 [@0xc00000000006cda8]. When that value equals 0x2 [call 3, @0xc00000000006cdc8 taken], it calls do_reg_op with a0 = 1. When it equals 0x4 [call 4, @0xc00000000006cdd0 taken], it calls do_reg_op with a0 = 2. When it equals 0x8 [call 5, @0xc00000000006cdd8 taken], it calls do_reg_op with a0 = 1. It then calls do_reg_op with a0 = 2 [call 6, @0xc00000000006cdd8 taken]. When it equals 0xa [call 1, @0xc00000000006cde0 taken], it calls do_reg_op with a0 = 1. It then calls do_reg_op with a0 = 2 [call 2, @0xc00000000006cde0 taken].
  Role (from facts): dispatches a register operation by the type code in the halfword at a0 [@0xc00000000006cda8].
  Reviewed: 2026-11-12

- `rou_fence_op` in `6.5.22/unix.IP27` at 0xc00000000006cea4
  **Inferred.** It calls do_router_op with a2 = 0x10 [call 1]. When that returns a nonnegative value [@0xc00000000006ced8 fall], it reads the byte at a1 +0x8 [@0xc00000000006cee8]. It calls do_router_op again [call 2]. It returns the last do_router_op result [@0xc00000000006cf60].
  Role (from name): issues a fence to the router, as the name says; the first call's status test, the byte read at a1 +0x8 and the per-bit call are the facts.
  Reviewed: 2026-11-12

- `prominfo_del` in `6.5.22/unix.IP27` at 0xc00000000006d1f8
  **Inferred.** It takes the lock at 0xc0000000014d1590 with mutex_spinlock [call 1]. When a loaded word equals a0 [@0xc00000000006d224 taken], it stores 8 bytes [@0xc00000000006d25c].
  Role (from name): removes a prominfo node under the lock, as the name says; the lock, the eight-byte test at 0xc0000000014d1588 [@0xc00000000006d214 taken zero], the match test and the store are the facts.
  Reviewed: 2026-11-12

- `prominfo_nodeget` in `6.5.22/unix.IP27` at 0xc00000000006d270
  **Inferred.** It takes the lock with mutex_spinlock [call 1]. When a loaded word equals a0 [@0xc00000000006d29c taken], it loads a word [@0xc00000000006d2dc].
  Role (from name): finds a prominfo node under the lock, as the name says; the lock calls, the eight-byte test at 0xc0000000014d1588 [@0xc00000000006d290 taken zero] and the match test are the facts.
  Reviewed: 2026-11-12

- `cpuprom_detailed_inventory_info_add` in `6.5.22/unix.IP27` at 0xc00000000006d2e0
  **Inferred.** It calls hubdev_cnodeid_get [call 1]. It allocates a record with klhwg_invent_alloc, passing 0x15 in a1 and 0x20 in a2 [call 2]. It stores one byte at +0x4 of that record [@0xc00000000006d318]. It registers the record with hwgraph_info_add_LBL [call 3] and exports it with hwgraph_info_export_LBL [call 4].
  Role (from name): records a CPU's detailed inventory entry, as the name says; the allocation, the field stores and the two hwgraph calls are the facts.
  Reviewed: 2026-11-12

- `cpuprom_map` in `6.5.22/unix.IP27` at 0xc00000000006d3e0
  **Inferred.** It looks up the PROM node with prominfo_nodeget [call 1]. When prominfo_nodeget returns nonzero [@0xc00000000006d3fc fall nonzero], it fetches the base with hubdev_prombase_get [call 2, @0xc00000000006d3fc fall]. It maps that base with v_mapphys [call 4, @0xc00000000006d3fc fall].
  Role (from name): maps a CPU's PROM window, as the name says; the node lookup, the base fetch and the mapping call are the facts.
  Reviewed: 2026-11-12

- `hubspc_init` in `6.5.22/unix.IP27` at 0xc00000000006d4b4
  **Inferred.** It calls hubdev_register with a0 = 0xc000000000279850, the entry of mem_refcnt_attach [call 1]. It calls hubdev_register again with a0 = 0xc00000000006d360, the entry of cpuprom_attach [call 2]. It sets up the lock at 0xc0000000014d1590 with spinlock_init [call 3].
  Role (from name): sets up hub support state, as the name says; the two registrations of attach routines and the lock setup are the facts.
  Reviewed: 2026-11-12

- `hubspc_map` in `6.5.22/unix.IP27` at 0xc00000000006d5c8
  **Inferred.** When a3 is nonzero [call 1, @0xc00000000006d5e8 fall nonzero], it gets device info with device_info_get. When that value is zero [call 3, @0xc00000000006d608 taken zero], it calls mem_refcnt_mmap. When that value is 1 [call 4, @0xc00000000006d610 taken], it calls cpuprom_map.
  Role (from name): maps hub device space, as the name says; the device lookup and the two mapping calls chosen by device kind are the facts.
  Reviewed: 2026-11-12

- `xtalk_dmamap_drain` in `6.5.22/unix.IP27` at 0xc00000000006d9b8
  **Inferred.** It calls hub_dmamap_drain [call 1] and returns its result [@0xc00000000006d9cc].
  Role (from facts): forwards to hub_dmamap_drain [call 1] and returns its result [@0xc00000000006d9cc].
  Reviewed: 2026-11-12
