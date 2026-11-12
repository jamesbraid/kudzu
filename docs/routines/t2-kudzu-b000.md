# Tier-2 routine readings, kudzu batch 0

Role statements for the first 25 routines of the tier-2 list, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. Where the facts stop short of the meaning, the statement says so.
Addresses are in the binary named in each header.

- `arcs_to_dsk` in `6.5.22/unix.IP27` at 0xc00000000001951c
  **Inferred.** Parses a string at arg0 that carries numeric fields in parentheses and after commas: strchr looks for `(` (0x28), `)` (0x29) and `,` (0x2c), and atoi converts the fields. Four fixed prefixes are compared with strncmp at 0xc00000000001953c, 0xc0000000000195d8, 0xc0000000000195f4 and 0xc00000000001960c, and a matched form is formatted into the buffer passed as arg1 with sprintf at 0xc0000000000196dc. The meaning of each prefix and of the output format is not settled by these facts.

- `router_reg_read` in `6.5.22/unix.IP27` at 0xc00000000001b114
  **Inferred.** Forwards a register read. It takes a record at arg0, reads its halfword at +0x2, doubleword at +0x18 and word at +0x20, and calls hub_vector_read at 0xc00000000001d25c with those values as a0, a1 and a2. It returns that call's result unchanged, so the register meaning is whatever hub_vector_read does.

- `router_reg_write` in `6.5.22/unix.IP27` at 0xc00000000001b144
  **Inferred.** Forwards a register write with the same record layout as the read path. It reads the halfword at +0x2, doubleword at +0x18 and word at +0x20 of arg0, calls hub_vector_write at 0xc00000000001d33c with them, and returns that result unchanged.

- `router_nic_access` in `6.5.22/unix.IP27` at 0xc00000000001b174
  **Inferred.** Drives a NIC register access through the router. It calls nic_get_phase_bits at 0xc00000000006a6bc, writes a control word built from that result and the arguments (shifted by 10 and 2 bits) to register 0x38 with router_reg_write, then polls register 0x38 with router_reg_read. The poll tests bit 0x2 of the status word at +0x8 of its buffer and a negative return, and calls us_delay at 0xc000000000047274 between polls. The retry bound and the success value are not visible in these facts.

- `router_set_leds` in `6.5.22/unix.IP27` at 0xc00000000001b224
  **Inferred.** Updates an LED value kept in arg0. It compares the byte at +0x6 with the byte at +0x7, stores the new value at +0x6 (sb at 0xc00000000001b260), and calls router_reg_write at 0xc00000000001b144 with a1 = 0, using the doubleword at +0x10 as the other argument. The condition that triggers the write and what the LED value encodes are not settled by these facts.

- `router_acquire_port_info` in `6.5.22/unix.IP27` at 0xc00000000001b2e4
  **Inferred.** Reads a port's status under the lock at offset 404 of arg0 (mutex_spinlock at 0xc000000000287214, mutex_spinunlock at 0xc000000000287260). It reads port registers at offsets 0x10 and 0x88 through router_reg_read at 0xc00000000001b114, calls router_decode_port_errors at 0xc00000000001b278, and updates the state bytes at +0x7 and the table entries it reaches. The field meanings are inferred from the access pattern only.

- `router_acquire_info` in `6.5.22/unix.IP27` at 0xc00000000001b4d4
  **Inferred.** Brings a router into the acquired state under the lock at offset 404 of arg0. It calls router_acquire_port_info at 0xc00000000001b2e4 with the byte at +0x191, sets the LEDs through router_set_leds at 0xc00000000001b224, and writes register offset 0x18 twice with router_reg_write. It also copies a doubleword from the hub-space address 0x9200000001030100 into the record at +0x30 and +0x160. The order of the writes and the reason for the two register writes are not settled by these facts.

- `router_print_error` in `6.5.22/unix.IP27` at 0xc00000000001b6c4
  **Inferred.** Reports a router error. It reads the doubleword at +0x178 and the byte at +0x1af of arg0, converts a flag word to a type with error_flag_to_type at 0xc00000000001b67c, and prints the result with cmn_err at 0xc0000000001f8090, at level 4 on one path and with class 0x404 on the other. It then stores back the byte at +0x1af. Which path runs and what the stored byte means are not settled by these facts.

- `router_check_errors` in `6.5.22/unix.IP27` at 0xc00000000001b7e8
  **Inferred.** A one-call wrapper. It passes arg0 unchanged to router_check_port at 0xc00000000001b79c and returns that routine's result. What is checked is whatever router_check_port does, which is outside this routine.

- `router_port_init` in `6.5.22/unix.IP27` at 0xc00000000001b840
  **Inferred.** Writes one router port's register block. It reads the port's offset 0 register with router_reg_read at 0xc00000000001b114, writes 0x40000 to offset 0 and zero to offset 0x10 with router_reg_write at 0xc00000000001b144, and reads offset 0x88. The port index comes from arg1, and the halfword at +0x158 of arg0 is also read. The meaning of the 0x40000 bit and the purpose of the reads are not settled by these facts.

- `router_port_hist_reselect` in `6.5.22/unix.IP27` at 0xc00000000001b904
  **Inferred.** Updates a port's registers on the node that owns the port. It takes the node from the halfword at +0x2 of arg0 and brackets the accesses with setnoderun at 0xc0000000000243ac and restorenoderun at 0xc0000000000244a0. In between it reads offset 0 with router_reg_read, writes a field shifted by 18 bits to offset 0, and writes zero to offset 0x10. One return path yields 0x16. The history meaning of these writes is not settled by these facts.

- `router_set_fences` in `6.5.22/unix.IP27` at 0xc00000000001baec
  **Inferred.** Sets a router's fence state and records it. It reads two bytes at +0x8 of arg0, reads registers at offsets 0x10 and a second offset through router_reg_read at 0xc00000000001b114, writes registers through router_reg_write at 0xc00000000001b144, and stores a doubleword at +0x170 of arg0. Which fence bits it touches is not settled by these facts.

- `router_init` in `6.5.22/unix.IP27` at 0xc00000000001be54
  **Inferred.** Creates and sets up the software record for one router. It allocates a 0x1b8-byte record with kmem_zalloc_node at 0xc0000000001823a0, registers it with hwgraph_info_add_LBL at 0xc0000000002960bc, initializes the lock at offset 404, and reads and writes register offsets 0x0, 0x10 and 0x18 through router_reg_read and router_reg_write. It then sets up ports with router_port_init and router_set_fences, acquires state with router_acquire_info, passes router_nic_access to nic_vertex_info_set, and on the error path frees the record with kmem_free at 0xc00000000001c284 and calls assfail at 0xc00000000001c29c and 0xc00000000001c2b4. The return is 0 or a register value; which checks select the error path is not settled by these facts.

- `router_state_empty` in `6.5.22/unix.IP27` at 0xc00000000001c40c
  **Inferred.** Returns 0 or 1, apparently reporting whether a router is empty. It reads register offset 0 with router_reg_read at 0xc00000000001b114, then reads again with the byte at +0x8 of arg0 as one of the arguments. The condition that yields 1 is not visible in these facts.

- `router_print_state_specific` in `6.5.22/unix.IP27` at 0xc00000000001c52c
  **Inferred.** Prints the state of one router. It takes the lock at offset 404 of arg0 (mutex_spinlock at 0xc000000000287214), polls with timeout_poll at 0xc00000000001c384 using max at 0xc0000000001fb4b0 for the limit, and calls router_state_empty at 0xc00000000001c40c. It then prints names and status through get_slotname at 0xc00000000003c99c, show_rr_status_rev_id at 0xc000000000031e8c and show_rr_error_status at 0xc000000000031d14. The timeout meaning and the printed fields are not settled by these facts.

- `router_nic_probe` in `6.5.22/unix.IP27` at 0xc00000000001c7d4
  **Inferred.** Probes the NIC behind a router. It calls nic_get_phase_bits at 0xc00000000006a6bc, writes the phase-bit control word to register 0x38 with vector_write at 0xc00000000001d02c, then reads register 0x38 twice with vector_read at 0xc00000000001cfe4, with us_delay at 0xc000000000047274 between the reads. The return value is not settled by these facts.

- `hub_vector_read` in `6.5.22/unix.IP27` at 0xc00000000001d25c
  **Inferred.** Reads a vector register on a hub. It calls in_panic_mode at 0xc0000000001e1c54 before and after the access, takes the spinlock at offset 18768 (decimal) of the hub record selected by arg0 (mutex_spinlock at 0xc000000000287214, unlock at 0xc000000000287260), and calls vector_read_node at 0xc00000000001cbd8 with arg1 and the shared table at 0xc0000000014d3800. The panic-mode branch and the register layout are not settled by these facts.

- `hub_vector_write` in `6.5.22/unix.IP27` at 0xc00000000001d33c
  **Inferred.** Writes a vector register on a hub, mirroring hub_vector_read. It calls in_panic_mode at 0xc0000000001e1c54 before and after the access, takes the spinlock at offset 18768 (decimal) of the hub record selected by arg0, and calls vector_write_node at 0xc00000000001c9a0 with arg1 and the shared table at 0xc0000000014d3800. The panic-mode branch and the register layout are not settled by these facts.

- `hub_widget_id` in `6.5.22/unix.IP27` at 0xc00000000001d41c
  **Inferred.** Returns a doubleword loaded from a hub-space address built from arg0 shifted into bits 32 and up, plus a fixed constant. It takes no lock and makes no call. Which widget or register that address selects is not settled by these facts.

- `find_component` in `6.5.22/unix.IP27` at 0xc00000000001d7fc
  **Inferred.** Searches the component list at arg0, whose entry count is the byte at +0x17, for an entry that matches the arguments, and returns the match or 0. The failure path prints a debug line with dprintf at 0xc00000000004cbe4. The field compared and the list layout are inferred from the loads and branches only.

- `find_first_component` in `6.5.22/unix.IP27` at 0xc00000000001d944
  **Inferred.** A wrapper that calls find_component at 0xc00000000001d7fc with arg0, zero as the second argument, arg1 as the third and arg3 as the fourth, then returns that result. The name suggests it returns the first match find_component reports, but the facts show only the forwarding.

- `is_master_baseio` in `6.5.22/unix.IP27` at 0xc00000000001dcb0
  **Inferred.** Answers whether the calling board is the master base-IO board on its node. It reads the node record through the table at 0xc0000000014d3600 and the hub-space address built from arg0 at offset 0x48, finds the module slot with find_lboard_modslot at 0xc00000000001d964 (called twice), and reads the flag byte at +0x9 of the slot record. It returns 0 or a flag value; the flag's meaning is not settled by these facts.

- `get_cpu_cnode` in `6.5.22/unix.IP27` at 0xc00000000001e0d8
  **Inferred.** Returns a halfword from the fixed table at 0xc0000000014d3880, probably the calling CPU's cnode. The routine does not index the table, so any per-CPU selection is not visible here. The render shows v0 as the table address rather than the loaded value, so what is returned is not settled by these facts.

- `get_actual_nasid` in `6.5.22/unix.IP27` at 0xc00000000001e210
  **Inferred.** Finds the first component of class 2 under arg0 with find_first_component at 0xc00000000001d944. It reads bytes at +0x2 and +0x18 of that component and the halfword at +0x20 of arg0, and returns a value built from those fields, or -1 when no component matches. The NASID meaning is inferred from the routine name and the -1 return only.

- `nasid_slice_to_cpuinfo` in `6.5.22/unix.IP27` at 0xc00000000001e274
  **Inferred.** Maps a node slice from arg0 to its CPU information. It reads the node record at the hub-space address built from arg0 at offset 0x48, looks up the board with find_lboard at 0xc00000000001d6b4 for class 0x11, then finds components with find_first_component and find_component. It reads bytes at +0x18 of the component results and returns 0 or a value; the exact output is not settled by these facts.
