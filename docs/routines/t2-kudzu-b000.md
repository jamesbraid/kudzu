# Tier-2 routine readings, kudzu batch 0

Role statements for the first 25 routines of the tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Where the facts stop short of the meaning, the statement says so.
Addresses are in the binary named in each header.

- `arcs_to_dsk` in `6.5.22/unix.IP27` at 0xc00000000001951c
  **Inferred.** Parses a string at arg0 that carries numeric fields in parentheses and after commas: strchr looks for `(` (0x28), `)` (0x29) and `,` (0x2c), and atoi converts the fields. Four fixed prefixes are compared with strncmp at 0xc00000000001953c, 0xc0000000000195d8, 0xc0000000000195f4 and 0xc00000000001960c, and a matched form is formatted into the buffer passed as arg1 with sprintf at 0xc0000000000196dc. The meaning of each prefix and of the output format is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_reg_read` in `6.5.22/unix.IP27` at 0xc00000000001b114
  **Inferred.** Forwards a register read. It takes a record at arg0, reads its halfword at +0x2, doubleword at +0x18 and word at +0x20, and calls hub_vector_read at 0xc00000000001d25c with those values as a0, a1 and a2. It returns that call's result unchanged, so the register meaning is whatever hub_vector_read does.
  Reviewed: 2026-11-12 (sonnet)

- `router_reg_write` in `6.5.22/unix.IP27` at 0xc00000000001b144
  **Inferred.** Forwards a register write with the same record layout as the read path. It reads the halfword at +0x2, doubleword at +0x18 and word at +0x20 of arg0, calls hub_vector_write at 0xc00000000001d33c with them, and returns that result unchanged.
  Reviewed: 2026-11-12 (sonnet)

- `router_nic_access` in `6.5.22/unix.IP27` at 0xc00000000001b174
  **Inferred.** Role (from name): drives a NIC register access through the router. It calls nic_get_phase_bits at 0xc00000000006a6bc, writes a control word built from that result and the arguments (a1 shifted by 10 and a2 shifted by 2, ORed in) to register 0x38 with router_reg_write, then polls register 0x38 with router_reg_read. The poll loop exits when the read returns negative or when bit 0x2 of the word at +0x8 of its buffer is set; us_delay at 0xc000000000047274 is called once after the loop exits, not between polls. The retry bound and the success value are not visible in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_set_leds` in `6.5.22/unix.IP27` at 0xc00000000001b224
  **Inferred.** Role (from name): updates an LED value kept in arg0. It compares the byte at +0x6 with the byte at +0x7 and returns early when they are equal. When they differ it loads the byte at +0x7 and the doubleword at +0x10, stores a byte to +0x6 (sb at 0xc00000000001b260), and calls router_reg_write at 0xc00000000001b144 with a1 = 0. The facts do not show which value is stored at +0x6 or which argument slot takes the doubleword, and do not settle what the LED value encodes.
  Reviewed: 2026-11-12 (sonnet)

- `router_acquire_port_info` in `6.5.22/unix.IP27` at 0xc00000000001b2e4
  **Inferred.** Gathers port information for a router record in arg0. It reads port registers at offsets 0x10 and 0x88 through router_reg_read at 0xc00000000001b114 (a1 = port << 16 | offset) before it takes the lock at offset 404 of arg0 with mutex_spinlock at 0xc000000000287214; router_decode_port_errors at 0xc00000000001b278 is called after the lock is taken, and mutex_spinunlock at 0xc000000000287260 is called on two paths. It stores a byte to +0x7 and halfwords to table entries it reaches. The field meanings are inferred from the access pattern only.
  Reviewed: 2026-11-12 (sonnet)

- `router_acquire_info` in `6.5.22/unix.IP27` at 0xc00000000001b4d4
  **Inferred.** Role (from name): brings a router into the acquired state. It takes the lock at offset 404 of arg0 (mutex_spinlock at 0xc000000000287214), and calls router_acquire_port_info at 0xc00000000001b2e4 with the byte at +0x191 as its third argument. That call sits between a mutex_spinunlock and a second mutex_spinlock. It also calls router_set_leds at 0xc00000000001b224, writes register offset 0x18 twice with router_reg_write, and stores doublewords at +0x30 and +0x160 of the record. The source of those doublewords, the order of the writes and the reason for the two register writes are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_print_error` in `6.5.22/unix.IP27` at 0xc00000000001b6c4
  **Inferred.** Reports a router error. It loads the doubleword at +0x178 and the byte at +0x1af of arg0, and has an early return path that depends on a signed compare against 0x1f5. It converts a flag word to a type with error_flag_to_type at 0xc00000000001b67c and prints with cmn_err at 0xc0000000001f8090. The first cmn_err (level 4, at 0xc00000000001b758) runs only when the flag word ANDed with the byte at +0x1af is nonzero. A byte is stored to +0x1af after that, then error_flag_to_type and a second cmn_err (class 0x404, at 0xc00000000001b78c) run on the path that follows. The stored value and what the byte means are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_check_errors` in `6.5.22/unix.IP27` at 0xc00000000001b7e8
  **Inferred.** Role (from name): checks a router for errors. It calls router_check_port at 0xc00000000001b79c with arg0 passed through as a0, inside a loop whose back branch at 0xc00000000001b81c tests a counter against 7, so the call can run several times. The other arguments and the return value are not visible in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_port_init` in `6.5.22/unix.IP27` at 0xc00000000001b840
  **Inferred.** Role (from name): sets up one router port's register block. It reads the port's offset 0 register with router_reg_read at 0xc00000000001b114, calls router_reg_write at 0xc00000000001b144 for offset 0 (the constant 0x40000 appears in a3 of that call and a2 is not resolved) and for offset 0x10 (a2 = 0), and reads offset 0x88. The port index comes from arg1 (a1 = port << 16 | offset), and the halfword at +0x158 of arg0 is compared with 0x50 on a branch. The meaning of the 0x40000 constant and the purpose of the reads are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_port_hist_reselect` in `6.5.22/unix.IP27` at 0xc00000000001b904
  **Inferred.** Updates a port's registers on a node taken from the halfword at +0x2 of arg0. It brackets the accesses with setnoderun at 0xc0000000000243ac and restorenoderun at 0xc0000000000244a0. In between it reads offset 0 with router_reg_read, writes offset 0 with router_reg_write (a2 << 18 appears in a3 of that call, a2 is not resolved), and writes zero (a2 = 0) to offset 0x10. One return path yields 0x16. The history meaning of these writes is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_set_fences` in `6.5.22/unix.IP27` at 0xc00000000001baec
  **Inferred.** Role (from name): sets a router's fence state and records it. It loads the byte at +0x8 of arg0 (twice), reads registers through router_reg_read at 0xc00000000001b114 (first with a1 = 0x10, then with an unresolved offset), writes registers through router_reg_write at 0xc00000000001b144 (the last with a1 = 0x10), stores a doubleword at +0x170 of arg0, and returns the last router_reg_write result. Which fence bits it touches is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_init` in `6.5.22/unix.IP27` at 0xc00000000001be54
  **Inferred.** Creates and sets up the software record for one router. It allocates a 0x1b8-byte record with kmem_zalloc_node at 0xc0000000001823a0, registers it with hwgraph_info_add_LBL at 0xc0000000002960bc, initializes the lock at offset 404 with spinlock_init, reads register offsets 0x0 and 0x18 with router_reg_read and writes offset 0x18 with router_reg_write. It then sets up ports with router_port_init and router_set_fences, calls router_acquire_info, and passes router_nic_access to nic_vertex_info_set. Error paths print through net_errmsg and cmn_err, and one path frees the record with kmem_free at 0xc00000000001c284 and calls assfail at 0xc00000000001c29c and 0xc00000000001c2b4. The return is either an unresolved value or a router_reg_read result; which checks select the error path is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_state_empty` in `6.5.22/unix.IP27` at 0xc00000000001c40c
  **Inferred.** Returns 0 or 1. Role (from name): reports whether a router is empty. It reads register offset 0 with router_reg_read at 0xc00000000001b114, then reads again in a loop that tests a counter against 7 and ANDs the byte at +0x8 of arg0 with a mask; the byte also appears as a3 of the second read. The condition that yields 1 is not visible in these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_print_state_specific` in `6.5.22/unix.IP27` at 0xc00000000001c52c
  **Inferred.** Role (from name): prints the state of one router. It takes the lock at offset 404 of arg0 (mutex_spinlock at 0xc000000000287214) and releases it on two paths. When a flag byte reached through a per-node table is nonzero it calls timeout_poll at 0xc00000000001c384, with max at 0xc0000000001fb4b0 (a0 = a1 = 0x100) supplying its fourth argument. It calls router_state_empty at 0xc00000000001c40c, passes a slot name from get_slotname at 0xc00000000003c99c to an unresolved callee, and calls show_rr_status_rev_id at 0xc000000000031e8c and show_rr_error_status at 0xc000000000031d14. The meaning of the timeout and the printed fields are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `router_nic_probe` in `6.5.22/unix.IP27` at 0xc00000000001c7d4
  **Inferred.** Role (from name): probes the NIC behind a router. It calls nic_get_phase_bits at 0xc00000000006a6bc, writes the phase-bit control word to register 0x38 with vector_write at 0xc00000000001d02c, then reads register 0x38 with vector_read at 0xc00000000001cfe4 in a loop that exits when a read returns negative or bit 0x2 of the result is set. us_delay at 0xc000000000047274 is called once after the loop, not between reads. The return value is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_vector_read` in `6.5.22/unix.IP27` at 0xc00000000001d25c
  **Inferred.** Role (from name): reads a vector register on a hub. It calls in_panic_mode at 0xc0000000001e1c54, and on one path calls it a second time, both before the access. It takes the spinlock at offset 18768 (decimal) of the hub record selected by arg0 (mutex_spinlock at 0xc000000000287214, unlock at 0xc000000000287260), stores a byte to +0x4954 before and after the access, and calls vector_read_node at 0xc00000000001cbd8 with arg1 as a0, the shared table at 0xc0000000014d3800 as a1, and arg2 and arg3 passed through. The panic-mode branch and the register layout are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_vector_write` in `6.5.22/unix.IP27` at 0xc00000000001d33c
  **Inferred.** Role (from name): writes a vector register on a hub, with the same structure as hub_vector_read. It calls in_panic_mode at 0xc0000000001e1c54, and on one path calls it a second time, both before the access. It takes the spinlock at offset 18768 (decimal) of the hub record selected by arg0, stores a byte to +0x4954 before and after the access, and calls vector_write_node at 0xc00000000001c9a0 with arg1 as a0, the shared table at 0xc0000000014d3800 as a1, and arg2 and arg3 passed through. The panic-mode branch and the register layout are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `hub_widget_id` in `6.5.22/unix.IP27` at 0xc00000000001d41c
  **Inferred.** Role (from name): returns a widget id. It makes one load of a doubleword at a large constant offset (-0x6dfffffffe3fffe0) from a base register that the facts do not resolve, and returns. It takes no lock and makes no call. How the address depends on arg0 and which widget or register it selects are not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `find_component` in `6.5.22/unix.IP27` at 0xc00000000001d7fc
  **Inferred.** Searches the component list at arg0, whose entry count is the byte at +0x17, for an entry that matches the arguments, and returns the match or 0. The failure path prints a debug line with dprintf at 0xc00000000004cbe4. The field compared and the list layout are inferred from the loads and branches only.
  Reviewed: 2026-11-12 (sonnet)

- `find_first_component` in `6.5.22/unix.IP27` at 0xc00000000001d944
  **Inferred.** A wrapper that calls find_component at 0xc00000000001d7fc with arg0, zero as the second argument, arg1 as the third and arg3 as the fourth, then returns that result. The name suggests it returns the first match find_component reports, but the facts show only the forwarding.
  Reviewed: 2026-11-12 (sonnet)

- `is_master_baseio` in `6.5.22/unix.IP27` at 0xc00000000001dcb0
  **Inferred.** Role (from name): answers whether the calling board is the master base-IO board on its node. It loads a doubleword at +0x48 from a base the facts do not resolve, compares a halfword read through the table at 0xc0000000014d3600 with -1, calls find_lboard_modslot at 0xc00000000001d964 (twice, on separate paths), and when the second result is nonzero loads the flag byte at +0x9 of the slot record. It returns 0 or a value that is not resolved; the flag's meaning is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `get_cpu_cnode` in `6.5.22/unix.IP27` at 0xc00000000001e0d8
  **Inferred.** Role (from name): returns the current CPU's cnode. It makes one halfword load (lh +0x0, in the delay slot of the return) from the fixed address 0xc0000000014d3880, which the render labels entry(0xc0000000014d3880), and returns. No index is applied. Whether v0 holds the loaded halfword or the address is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)

- `get_actual_nasid` in `6.5.22/unix.IP27` at 0xc00000000001e210
  **Inferred.** Role (from name): returns the actual NASID. When arg0 is nonzero it calls find_first_component at 0xc00000000001d944 with a1 = 2, a2 = arg2 and a3 = -1; when arg0 is zero it returns -1. It loads the byte at +0x2 of the returned component and, then a halfword at +0x20 only when bit 0x1 of that byte is set, and a byte at +0x18 afterwards. The bases of those two loads are not resolved in the facts, and the return value is not shown for the no-match case. The NASID meaning is inferred from the routine name only.
  Reviewed: 2026-11-12 (sonnet)

- `nasid_slice_to_cpuinfo` in `6.5.22/unix.IP27` at 0xc00000000001e274
  **Inferred.** Role (from name): maps a node slice from arg0 to its CPU information. It loads a doubleword at +0x48 from a base the facts do not resolve, calls find_lboard at 0xc00000000001d6b4 with a1 = 0x11, then find_first_component (a1 = 1) and, in a loop, find_component, loading a byte at +0x18 after each. It returns 0 on the path where find_lboard finds nothing, otherwise a value that is not resolved; the exact output is not settled by these facts.
  Reviewed: 2026-11-12 (sonnet)
