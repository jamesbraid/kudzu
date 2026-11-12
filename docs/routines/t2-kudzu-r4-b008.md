# Tier-2 routine readings, kudzu batch 8

Role statements for 25 routines of the 6.5.22 IP27 kernel image, written from the
rendered instructions only: loads, stores, calls, branches and constants. Every
grade is inferred. The facts show what each routine touches and calls, not what the
fields and constants mean, so each statement leaves that open where the facts stop.
Addresses are in the binary named in each header.

- `ppb_scan` in `6.5.22/unix.IP27` at 0xc000000000092490
  **Inferred.** Takes a semaphore at 0xc0000000014f58a0 (psema with 0x19), calls do_ppb_scan (0xc000000000092768) while holding it, then releases it with vsema (0xc00000000018ac88). Only after the release, and only when do_ppb_scan's result is zero, it goes on to read the pointer at +0x1e8 of the pciio_info_get record and several doublewords behind it, round sizes with ppb_roundup_log2 (0xc000000000094168), and allocate and free I/O space through pciio_piospace_alloc (0xc0000000000943c8) and pciio_piospace_free (0xc0000000000943ec). The return is one of do_ppb_scan's result, -1 or 0; the facts do not show what the I/O ranges stand for.
  Reviewed: 2026-11-12 (sonnet)

- `do_ppb_scan` in `6.5.22/unix.IP27` at 0xc000000000092768
  **Inferred.** Reads the record from pciio_info_get and the bus record from pciio_info_bus_get, then calls ppb_bus_alloc (0xc000000000093d64); Role (from name): the call returns a bus number, which the facts do not show. It allocates two records with kmem_zalloc (0x80 and 0x200 bytes), writes config bytes 0x18, 0x19 and 0x1a through pciio_config_set (0xc000000000094c04), and passes a name formatted by sprintf to hwgraph_path_add. A nonzero result from hwgraph_path_add takes an error exit that logs with cmn_err, frees the first record with kmem_free and releases the bus with ppb_bus_free (0xc000000000093e74); the routine returns -1 or 0. On the other path it calls ppb_device_init (0xc0000000000929c0), inside a loop and skipped when 0xffff equals a compared register that follows a pciio_config_get_safe call. The constants and callees fit a bus scan on a PCI-to-PCI bridge, but the facts alone do not settle what the scan finds.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_device_init` in `6.5.22/unix.IP27` at 0xc0000000000929c0
  **Inferred.** Loads a byte at +0x19 and a word at +0x0, then reads the master record through pciio_info_master_get and several values through pciio_config_get (0xc000000000094a94, sizes 1 and 2 in the facts) and pciio_config_get_safe (size 2). Inside a loop, and only when 0xffff differs from a register compared after a pciio_config_get_safe call, it creates a device record with pciio_device_info_new (0xc0000000000951a0), registers it with pciio_device_info_register, links it through ppb_link (0xc000000000093b98) and then runs ppb_function_init (0xc000000000092b50) on the new record. The only return, at 0xc000000000092b48, yields 0. The config offsets are passed in registers the facts do not resolve.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_function_init` in `6.5.22/unix.IP27` at 0xc000000000092b50
  **Inferred.** Reads the slot, function, vendor and device identifiers of a function through the pciio_info_*_get accessors, then runs a long sequence of pciio_config_get and pciio_config_set calls with constant offsets such as 0xd, 0xe, 0x4, 0x30 and 0xb. The call at 0xc000000000092e40 writes 0xfffff800 to offset 0x30 (size 4) through pciio_config_set; the code compares the vendor id with 0x10a9 around it, but the vendor id and 0x10a9 are not arguments of that call. It calls do_ppb_scan (0xc000000000092768) at 0xc000000000092fc8 and logs once through cmn_err (0xc0000000001f8090) at 0xc000000000093134. The only return, at 0xc000000000092f40, yields 0; the facts do not show which registers the constants select.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_addr_space_assign` in `6.5.22/unix.IP27` at 0xc000000000093144
  **Inferred.** First loads and stores fields of the device record (pointers at +0x1e8 and +0x10 of it among them). When the pointer at +0x10 of the record behind +0x1e8 is nonzero, it then calls function_addr_space_assign (0xc00000000009338c) in a loop; the call is not the first thing the routine does. It then reads the device record with pciio_info_dev_get and writes config offsets: 0x1c and 0x1d as bytes, then 0x20, 0x22, 0x24, 0x26, 0x30 and 0x32 as halfwords and 0x28 and 0x2c as 4-byte values, some of them behind branches. The offsets are consistent with the window registers of a PCI-to-PCI bridge, but the values written are not shown. Two cmn_err (0xc0000000001f8090) calls, at 0xc000000000093364 and 0xc00000000009337c, are reached by branches off the main path. The return at 0xc000000000093354 yields 0.
  Reviewed: 2026-11-12 (sonnet)

- `function_addr_space_assign` in `6.5.22/unix.IP27` at 0xc00000000009338c
  **Inferred.** Reads config offsets 0xb and 0xa through pciio_config_get (0xc000000000094a94), then writes config offsets 0x4 and 0x30 through pciio_config_set (0xc000000000094c04). The value passed at 0x30 is the doubleword at +0x48 of the pointer stored at +0x1f0 of the device record; the doubleword at +0x1a8 is loaded and combined with it (sum minus one) in registers that are not arguments of that call. On one path it calls ppb_addr_space_assign (0xc000000000093144) with 0x4 at 0xc00000000009364c, so the two routines call each other. The return at 0xc0000000000935f4 yields 0. The facts do not show what the two offsets 0xa and 0xb hold.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_function_print` in `6.5.22/unix.IP27` at 0xc0000000000936b0
  **Inferred.** Gets the bus, slot, function and device records, then reads values through pciio_config_get at offsets that include 0x0, 0x2, 0x9, 0xa, 0xb, 0xd, 0xe, 0x3f, 0x1c, 0x1d, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2c, 0x30 and 0x32. It builds text with strcat (0xc000000000146450) and sprintf (0xc0000000001f8e8c) and emits it with ten printf (0xc0000000001f8f2c) calls, the first at 0xc000000000093744. It calls itself at 0xc000000000093b6c with a buffer at sp-144, in a loop that runs while the pointer at +0x10 of the record behind +0x1e8 is nonzero. The return is one of the config_get, sprintf, printf or recursive results; what the printed field names mean is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_link` in `6.5.22/unix.IP27` at 0xc000000000093b98
  **Inferred.** Writes a register value to +0x1f0 of its first argument and reads the pointer at +0x10 of its second argument. It compares the byte fields at +0x11 and +0x12 of the first record with values loaded elsewhere, in a loop that follows a pointer chain. Along its paths it stores pointer-sized values at +0x10, +0x1e0 and +0x1d8 and at offsets the facts leave unnamed; the base register of those stores is not shown. It makes no calls, and each of its four returns yields an unresolved value, so the facts do not show whether the link succeeds.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_bus_alloc` in `6.5.22/unix.IP27` at 0xc000000000093d64
  **Inferred.** Searches the bit word at 0xc0000000014f58b0 for a free bit, testing bits in a loop bounded by a3 < 8, and stores the updated word back at 0xc000000000093e18. It returns 0xff when no bit is free and an unresolved value otherwise. The facts show a bit index from 0 to 7 but not what each bit stands for.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_bus_reserve` in `6.5.22/unix.IP27` at 0xc000000000093e24
  **Inferred.** Loads the word at 0xc0000000014f58b0 and returns it; the store of a value back to the word at 0xc000000000093e70 follows the return instruction at 0xc000000000093e6c (its delay slot), so the returned word is the one read before that store. The value it writes is not shown in the rendered facts, so what the reservation changes is open. It shares its word with ppb_bus_alloc (0xc000000000093d64) and ppb_bus_free (0xc000000000093e74).
  Reviewed: 2026-11-12 (sonnet)

- `ppb_bus_free` in `6.5.22/unix.IP27` at 0xc000000000093e74
  **Inferred.** Loads the word at 0xc0000000014f58b0 and returns it at 0xc000000000093ec0; the store of a value back to it at 0xc000000000093ec4 follows the return instruction (its delay slot), so the returned word is the one read before the store. As with ppb_bus_reserve, the stored value is not shown, so the bit it clears is not settled by these facts. Its shared word ties it to ppb_bus_alloc (0xc000000000093d64).
  Reviewed: 2026-11-12 (sonnet)

- `ppb_attach_thread` in `6.5.22/unix.IP27` at 0xc00000000009405c
  **Inferred.** Passes its four argument registers unchanged to ppb_attach_function (0xc000000000094080), then calls sthread_exit (0xc0000000001f0090) at 0xc00000000009406c. The return at 0xc000000000094078 is sthread_exit's result. The facts show only this forward and exit, so the thread role rests on the callee names.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_attach_function` in `6.5.22/unix.IP27` at 0xc000000000094080
  **Inferred.** Follows a chain of records through the pointer at +0x1e8 of its argument and the pointer at +0x10 of each record after it, calling itself at 0xc0000000000940a8 while the next pointer is nonzero. At the end it gets the device with pciio_info_dev_get (0xc000000000094e58) and attaches it with pciio_device_attach (0xc000000000095420), passing 0 as the second argument. The facts do not show what the chain represents, and the return value is unresolved.
  Reviewed: 2026-11-12 (sonnet)

- `ppb_roundup_log2` in `6.5.22/unix.IP27` at 0xc000000000094168
  **Inferred.** Returns 0 for a zero argument and returns 1 on one of its paths, with a third path that returns an unresolved value. Its branches test the sign bit (0x8000000000000000) and a flag in a3. The name suggests a round-up to a power of two, but the rendered facts list only branches and no arithmetic, so the rounding is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_piomap_alloc` in `6.5.22/unix.IP27` at 0xc000000000094204
  **Inferred.** Forwards its four arguments unchanged to pcibr_piomap_alloc (0xc000000000074058) and returns that call's result at 0xc000000000094218. The facts show no logic of its own, so the mapping rules come from the callee.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_piomap_addr` in `6.5.22/unix.IP27` at 0xc00000000009423c
  **Inferred.** Forwards its four arguments to pcibr_piomap_addr (0xc0000000000742f0), then stores a value at +0x20 at 0xc000000000094254, after the call, and returns that call's result at 0xc00000000009425c. The stored value and the store's base register are not shown in the facts, so what the field caches is open.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_piotrans_addr` in `6.5.22/unix.IP27` at 0xc000000000094280
  **Inferred.** Forwards its four arguments unchanged to pcibr_piotrans_addr (0xc000000000074338) and returns that call's result at 0xc000000000094294. The facts show no logic of its own, so the translation rules come from the callee.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_pio_addr` in `6.5.22/unix.IP27` at 0xc00000000009429c
  **Inferred.** Tries the translated path first: it calls pciio_piotrans_addr (0xc000000000094280), and only when that result is zero and a register (s0) is nonzero does it fall back to pciio_piomap_addr (0xc00000000009423c). When that fallback returns zero and s4 is nonzero it calls pciio_piomap_free (0xc000000000094220). Another path calls pciio_piomap_alloc (0xc000000000094204). When its a5 argument is nonzero it stores through it at 0xc0000000000942ec, and a second store at 0xc000000000094358 is guarded by a register copied from a5. The return at 0xc000000000094384 is one of the piotrans or piomap results; the facts do not show what the address means.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_intr_alloc` in `6.5.22/unix.IP27` at 0xc000000000094690
  **Inferred.** Looks up the device record with pciio_info_get (0xc000000000094d64) and compares its words at +0x8 and +0xc. It calls pciio_ppb_intr_map (0xc000000000093c84) only when they differ, then calls pcibr_intr_alloc (0xc000000000079d9c) with a0, a1 and a3 unchanged (the a2 value is not resolved) and returns that result at 0xc0000000000946f0. The facts do not show what the two fields mean, so the condition is inferred from the branch alone.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_cardinfo_get` in `6.5.22/unix.IP27` at 0xc0000000000947e8
  **Inferred.** Formats a name from a slot and function with pciio_slot_func_to_name (0xc000000000094784) and looks it up with hwgraph_traverse (0xc0000000002965ec). When hwgraph_traverse returns nonzero and the a2 argument is zero it formats again with the function argument 0xff and traverses a second time; the facts do not say which result means success. When the traverse result checked at 0xc00000000009482c is zero, it calls pciio_info_chk (0xc000000000094d38) and drops a reference with hwgraph_vertex_unref (0xc000000000295c58). The return is 0 or pciio_info_chk's result. The facts do not show what the card information contains.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_config_get` in `6.5.22/unix.IP27` at 0xc000000000094a94
  **Inferred.** Reads a PCI config value through the bridge by calling pcibr_config_get (0xc00000000007dedc) with its first argument. Its branches on the size-like argument a2 (zero at 0xc000000000094acc, which takes another path) and on s1 (0xc000000000094b18) choose the path. It returns 0 on one path and otherwise an unresolved value; the facts do not show the register offset that reaches the bridge.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_config_set` in `6.5.22/unix.IP27` at 0xc000000000094c04
  **Inferred.** Writes a PCI config value through the bridge by calling pcibr_config_set (0xc00000000007dfc0) with its first argument. Branches on the size-like argument a2 (zero at 0xc000000000094c38) and on s2 (0xc000000000094c70) choose the path. It returns either an unresolved value or pcibr_config_set's result at 0xc000000000094c94; the facts do not show the written value.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_driver_register` in `6.5.22/unix.IP27` at 0xc000000000095030
  **Inferred.** Calls cdl_add_driver (0xc0000000000d15bc) with the doubleword stored at 0xc0000000014c9e98 as its first argument and its own arguments after it. When that doubleword is zero, a branch at 0xc000000000095058 skips to pciio_init (0xc000000000094f5c) instead. The facts list no loop or exit condition. The return at 0xc000000000095080 is cdl_add_driver's result.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_device_inventory_add` in `6.5.22/unix.IP27` at 0xc0000000000953a8
  **Inferred.** Looks up the device record with pciio_info_get (0xc000000000094d64). Unless its words at +0x14 and +0x18 are -1, it adds an inventory entry through device_inventory_add (0xc0000000001c1a18) with the constants 0xb and 0x4, those two words and the byte at +0x11. The return at 0xc0000000000953ec is the record or device_inventory_add's result. The constants appear to be the inventory class and type, but the facts do not name them.
  Reviewed: 2026-11-12 (sonnet)

- `pciio_device_inventory_remove` in `6.5.22/unix.IP27` at 0xc0000000000953f4
  **Inferred.** Removes the device from the hardware inventory by calling hwgraph_inventory_remove (0xc000000000297164) with its first argument and -1 for each of the five remaining arguments, then returns that call's result at 0xc000000000095418. The -1 arguments look like wildcards for the inventory fields, but the facts do not name those fields.
  Reviewed: 2026-11-12 (sonnet)
