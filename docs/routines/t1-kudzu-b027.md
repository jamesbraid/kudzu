# IP30 kernel routines read from 6.5.30

Each entry says what its routine appears to do, based only on its instructions of that routine: the registers and memory it touches, the constants it compares against, and the routines it calls. Addresses name the location in `6.5.30/unix.IP30`. Where the facts stop short of a role, the entry says so and leaves the role partly open. Every entry is labelled inferred, because the names of callees and the shape of the code support the role but no fact here establishes it.

- `_ecc_errorvec` in `6.5.30/unix.IP30` at 0xa80000002001dd30
  **Inferred.** It loads a doubleword through the fixed uncached pointer at 0xffffffffaff50000, shifts it left by three and uses it to index a table, loads a doubleword at offset 0xff0 of that table slot, and stores two doublewords through that loaded pointer at offsets 0xc8 and 0x0. It ends with an indirect jump at 0xa80000002001dd90 to a constant target at 0x900000002001a3e0, passing the four argument registers through unchanged. The name suggests an ECC error vector, but the facts do not show what the stored words mean or what the target does.
  Reviewed: 2026-11-12 (sonnet)

- `dobuserr_common` in `6.5.30/unix.IP30` at 0xa800000020023f74
  **Inferred.** This is a large routine (9460 bytes), and this entry covers only its outline. It uses load-linked and store-conditional on the word at 0xa8000000204a68e0 (named heart_error_cpumap), reads a halfword at 0xffffffffffffc3a0, and calls cmn_err on many of its paths. It also calls routines with a heart_ or hem_ prefix (hem_add_sync, hem_add_ioe, heart_error_refine, heart_paddr_to_port, heart_imr_bits_rmw) and log_dimmerr, which suggests a bus error path that classifies and logs errors from one controller. The classification rules themselves are not established here.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_dbe` in `6.5.30/unix.IP30` at 0xa800000020026960
  **Inferred.** It reads from the uncached pointer at 0xffffffffaff50000 and from a table at offset -0x500f0000 indexed the same way, then stores a doubleword into the argument block at offset 0x140. It finishes with a direct jump at 0xa8000000200269b0 to VEC_trap at 0xa80000002001b9d0, passing the argument registers through. The name suggests a data bus error vector; the facts show the handoff to VEC_trap but not what the table or the stored value mean.
  Reviewed: 2026-11-12 (sonnet)

- `VEC_ibe` in `6.5.30/unix.IP30` at 0xa80000002002bf70
  **Inferred.** It follows the same pattern as VEC_dbe: a table update through the uncached pointer at 0xffffffffaff50000, a doubleword store to the argument block at offset 0x140, and a direct jump at 0xa80000002002bfd8 to VEC_trap at 0xa80000002001b9d0. It adds an early branch at 0xa80000002002bf78 on the sign of a register whose source the facts do not show. The name suggests the instruction-side counterpart of VEC_dbe, but the facts only show that the two share a shape.
  Reviewed: 2026-11-12 (sonnet)

- `heart_cpu_intr` in `6.5.30/unix.IP30` at 0xa80000002003ec64
  **Inferred.** It reads a halfword at 0xffffffffffffc3a0, stores a doubleword to 0xffffffffaff10028, then calls doacvec at 0xa8000000201d39c4 and doactions at 0xa8000000201d3684, returning the value from doactions. It looks like a CPU-level interrupt handler that runs two action routines. The facts do not show what the value stored to 0xffffffffaff10028 is or which interrupt source the routine serves.
  Reviewed: 2026-11-12 (sonnet)

- `heart_debug_intr` in `6.5.30/unix.IP30` at 0xa80000002003ec9c
  **Inferred.** It makes the same halfword read and the same store to 0xffffffffaff10028 as heart_cpu_intr at 0xa80000002003ec64, then calls debug at 0xa80000002001c9dc with a fixed first argument of 0xa8000000203afe98 and returns debug's value. It looks like the debug-interrupt form of the CPU interrupt entry. The facts do not tie it to a particular source.
  Reviewed: 2026-11-12 (sonnet)

- `chkdebug` in `6.5.30/unix.IP30` at 0xa80000002003ecd4
  **Inferred.** It makes the same halfword read as heart_debug_intr and loads a doubleword from 0xffffffffaff10030. A branch at 0xa80000002003ecf4 tests a register (a2, source not shown) ANDed with that doubleword. When the result is zero it skips both the doubleword store to 0xffffffffaff10028 at 0xa80000002003ed00 and the call at 0xa80000002003ed04 to debug at 0xa80000002001c9dc; otherwise it does the store and then calls debug. The meaning of the mask and the source of the compared register are not established.
  Reviewed: 2026-11-12 (sonnet)

- `clear_cpu_intr` in `6.5.30/unix.IP30` at 0xa80000002003ed18
  **Inferred.** It makes one doubleword store to 0xffffffffaff10028 at 0xa80000002003ed2c and returns 0xffffffffaff10000 in v0. The same store address appears in heart_cpu_intr, heart_debug_intr and chkdebug, so this is probably a clear or acknowledge write to that status word. The facts do not show the value stored, so what the write does is unknown.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_id_get` in `6.5.30/unix.IP30` at 0xa8000000200506b8
  **Inferred.** If its record pointer in a0 is zero, it jumps to a panic call at 0xa800000020050808 (panic at 0xa8000000201c7be8). Otherwise it reads a halfword at offset 0x72 of the record and compares it against a ladder of small values (3, 2, 0, 1 and -1, where -1 also reaches the panic), and it reads a word at offset 0x4 and a doubleword at offset 0x8. A second ladder compares a register whose source the facts do not show against 16-bit codes (0xe000, 0xe010, 0xd102, 0xd112, 0xd002, 0xc002) and against 0x36 and 0x24, and a mismatch with 0xc002 or 0x36 reaches the panic. The return value is 0 on one path and unknown on another. The codes look like PCI device or class identifiers, but the facts do not name them.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_stat_get` in `6.5.30/unix.IP30` at 0xa800000020050818
  **Inferred.** It has the same layout as pcireg_id_get: a null check on a0 that leads to a panic call at 0xa80000002005096c, a halfword read at offset 0x72, a word at offset 0x4 and a doubleword at offset 0x8. Its compares have the same shape: a ladder of small values on the halfword, then a second ladder of 16-bit codes (0xe000 to 0xc002) and 0x36 against a register whose source the facts do not show. Its name suggests it reports status rather than identity, but the facts do not show which fields it returns.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_nic_addr` in `6.5.30/unix.IP30` at 0xa800000020054250
  **Inferred.** If a0 is zero it jumps to a panic call at 0xa800000020054398 (panic at 0xa8000000201c7be8). Otherwise it reads the same halfword at offset 0x72, word at 0x4 and doubleword at 0x8, and runs the same code ladder as pcireg_id_get, with the first compare against 0x2 and then 0x3. The return value is 0 on one path and unknown on another. The name suggests it extracts a network interface address from the record, which the facts neither confirm nor rule out.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_vect_set` in `6.5.30/unix.IP30` at 0xa80000002005709c
  **Inferred.** It reads the doubleword at offset 0x8 and the halfword at offset 0x72 of its record in a0, and a negative value in a1 takes a branch at 0xa8000000200570e0 that leads to an assertion call at 0xa8000000200572b4 (assfail at 0xa80000002001bc94). Its other path updates several fields through pointers the facts do not resolve, and it ends in a panic call at 0xa8000000200572c8. The name suggests it sets an interrupt vector field in the record, but the facts show neither the field nor where the vector comes from.
  Reviewed: 2026-11-12 (sonnet)

- `pcireg_intr_addr_addr_set` in `6.5.30/unix.IP30` at 0xa800000020057508
  **Inferred.** It is a close sibling of pcireg_intr_addr_vect_set. It reads the doubleword at offset 0x8 and the halfword at offset 0x72 of the record, updates fields through unresolved pointers, and ends in either an assertion call at 0xa800000020057730 (assfail at 0xa80000002001bc94) or a panic call at 0xa800000020057744. The name suggests it sets an interrupt address field where the sibling sets a vector field, but the facts do not show which fields differ.
  Reviewed: 2026-11-12 (sonnet)

- `pckm_attach` in `6.5.30/unix.IP30` at 0xa800000020061aa4
  **Inferred.** It allocates three zeroed blocks with kmem_zalloc at 0xa8000000201486a4 (sizes 0x68, 0x58 and 0x58) and writes several of their fields. It calls ioc3_ helpers (ioc3_intr_connect, ioc3_write_ireg, ioc3_is_console, ioc3_mem_ptr, ioc3_subdev_enabled, ioc3_pckm_mlreset), probes the controller with pckm_pollcmd and pckm_flushinp, and registers the device through hwgraph_char_device_add, hwgraph_chmod, hwgraph_fastinfo_set and device_inventory_add. From the names, this attaches a keyboard and mouse controller that sits on an IOC3 chip, but the facts do not confirm the device.
  Reviewed: 2026-11-12 (sonnet)

- `cn_init` in `6.5.30/unix.IP30` at 0xa800000020069c20
  **Inferred.** It calls gfx_earlyinit at 0xa800000020358984 first, then tests a halfword at 0xa8000000204c8978 and, on various paths, stores to fields of the cn structure at offsets 8, 24, 32, 72 and 74. It calls is_specified at 0xa8000000201d7550 with 0xa800000020516120 as its first argument; when that call returns nonzero it compares a byte at 0xa800000020516120 against 0x67 and 0x47. Calls to tp_init at 0xa80000002035871c and du_init at 0xa800000020148134 sit on branches, and the return value is one of an unknown value, the result of tp_init or the result of du_init. The name suggests console setup, but which console types the two byte constants select is not shown.
  Reviewed: 2026-11-12 (sonnet)

- `ef_ioc3eaddr` in `6.5.30/unix.IP30` at 0xa8000000200be2f4
  **Inferred.** It reads a pointer at offset 0x398 and a word at offset 0x38c from its argument, then calls nic_get_eaddr at 0xa800000020039ec4 with two addresses built from a sub-record at offset 0x398. A nonzero return takes one branch at 0xa8000000200be314, and one path reaches a cmn_err report at 0xa8000000200be338 (format string at 0xa8000000203c4668) before returning. The return value is 0 or 5. The name points to reading an Ethernet address for an IOC3 network port, which the facts support only in part.
  Reviewed: 2026-11-12 (sonnet)

- `doacvec` in `6.5.30/unix.IP30` at 0xa8000000201d39c4
  **Inferred.** It reads flag bytes at 0xffffffffffffc080 and 0xffffffffffffc081 and writes back to 0xffffffffffffc04e and 0xffffffffffffc081, then reads a word at 0xffffffffffffc084 and moves a doubleword at 0xffffffffffffc0a0. Depending on those values it calls tlbflush_rand at 0xa80000002017b458 or tlbflush at 0xa80000002017b364, and it can call panicspin at 0xa8000000201c8bb8. heart_cpu_intr calls it as one of its two actions, so it is probably a cross-processor TLB flush request handler, but that reading rests on its callees, and its name is not explained by any fact here.
  Reviewed: 2026-11-12 (sonnet)

- `syssgi` in `6.5.30/unix.IP30` at 0xa8000000201eb9a8
  **Inferred.** This is the dispatcher for the syssgi system call. An indirect jump at 0xa8000000201eba08 uses the jump table at 0xa8000000203d7b10 to pick one of many handlers, and many of those handlers move data with copyin and copyout and check privileges with cap_able. Some of them call job-control helpers such as syssgi_makenewjob, syssgi_killjob and syssgi_setjlimit. This entry covers only the dispatch and the call graph, and the meaning of each subcommand is not established here.
  Reviewed: 2026-11-12 (sonnet)

- `dogetitimer` in `6.5.30/unix.IP30` at 0xa800000020212948
  **Inferred.** A branch at 0xa800000020212964 on a0 selects between two paths. With a0 zero it raises the interrupt level with spl65, takes a mutex with mutex_lock at 0xa8000000201535f4, reads timer state from a structure reached through a2 at offset 0x2f0 (offsets 0x80 and 0x840 through 0x858 of that structure), and calls chktimeout_tick at 0xa800000020015758 and tick_to_timeval at 0xa800000020213b54. With a0 nonzero it raises the level with spl1 and reads four doublewords at offsets 0x348 through 0x360 of the entry at a2 plus a0 shifted left by five. The routine stores doublewords to the output block at a1 at offsets 0x0, 0x8, 0x10 and 0x18. From the name this is the getitimer call, but the facts do not show which fields hold the interval and the current value.
  Reviewed: 2026-11-12 (sonnet)

- `gfP_OdsySaveHwShadow` in `6.5.30/unix.IP30` at 0xa800000020358bc4
  **Inferred.** It runs under spl1 and splx and checks whether its caller is attached to the graphics flow, using gfx_flow_am_i_attached at 0xa80000002038f874 and odsy_flow_am_i_attached at 0xa800000020371458. It reads and writes many fields of the context structures in a0 and a1, follows nested pointers, and reads the doubleword at 0xffffffffaff00080. It also calls timeout, untimeout_wait and us_delay; the facts do not show what any wait is for. From the name, it saves a shadow copy of the Odyssey hardware registers, but the facts do not show which registers it copies.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyInitialize` in `6.5.30/unix.IP30` at 0xa8000000203601e8
  **Inferred.** It takes a semaphore with psema at 0xa800000020150388 and checks attachment through gfx_flow_am_i_attached at 0xa80000002038f874. Its setup calls OdsyAlloc32kSlot at 0xa80000002036e248 twice, zeroes state with odsyZeroOut at 0xa8000000203600dc, and resets the device through odsyReset at 0xa800000020360b68. It gets an interrupt target from xtalk_intr_target_get at 0xa80000002003ac4c and inserts delays with odsy_usdelay and us_delay. The name fits device bring-up, but the facts show the call order and not the register values written.
  Reviewed: 2026-11-12 (sonnet)

- `odsyLowLevelReset` in `6.5.30/unix.IP30` at 0xa800000020360794
  **Inferred.** It performs a sequence of word reads and writes at fixed addresses in the 0x9000000010000000 range and in a table at 0x900000001000010c. It places delays between steps with odsy_usdelay (values 0xf4240, 0x3e8 and 0x186a0) and us_delay (0x30d40), calls odsyLLPReset at 0xa800000020360724 on one step, and reports through cmn_err at 0xa800000020360b38 on a later path. A compare against 0xd100 at 0xa8000000203607d4 decides one branch, and the facts do not show what that value is. From the name this is the low-level reset of the Odyssey board, but only part of the register sequence is visible.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyAttach` in `6.5.30/unix.IP30` at 0xa80000002036f8cc
  **Inferred.** It calls gfxdd_recycle at 0xa8000000201a9fa0 and gfxdd_mmap at 0xa8000000201a9e34, with 0xa8000000200c22b8 as the fourth argument (a3) of gfxdd_mmap; the gfxdd_recycle call comes first in address order. On one path it calls gf_OdsyUnMapGfx at 0xa800000020370c80 between spl1 and splx. It reads the word at offset 0x4 of its argument and the doubleword at 0xffffffffaff00080, and returns 0x0, 0xc or 0x16 in v0. The name and the callee names suggest the attach step for an Odyssey graphics context that maps a window, but the facts do not show the mapping range, the flags or why the unmap runs.
  Reviewed: 2026-11-12 (sonnet)

- `gf_OdsyDestroyDDRN` in `6.5.30/unix.IP30` at 0xa80000002038b748
  **Inferred.** Role (from name): the routine appears to tear down a per-context object; the facts do not explain the abbreviation in the name. In address order it calls gf_OdsyUnSchedSwapBuf at 0xa800000020366054, gf_OdsyUnSchedRetraceEvent at 0xa800000020366298, odsy_hpipe_cleanup at 0xa800000020388850, kmem_free at 0xa800000020148710, OdsyFree32kSlot, spl1, gfx_flow_detach_me and gfx_flow_dealloc, splx, odsyDMAFinish at 0xa80000002038e854, odsyMMdetach at 0xa800000020367ba0 and odsyUnpinAndUnmapGctxRegions at 0xa800000020370f50. Several of these calls sit behind branches, so which of them run on a given path is not stated here.
  Reviewed: 2026-11-12 (sonnet)

- `gfx_flow_delayed_hiwater_intr_prehandler` in `6.5.30/unix.IP30` at 0xa80000002038f1a0
  **Inferred.** It reads a pointer from the table at 0xa8000000204a8620, indexed by its first argument shifted left by three, and branches at 0xa80000002038f1b4 on whether that pointer is nonzero. It logs through cmn_err at 0xa8000000201c6b00, locks and unlocks the spin lock at 0xa8000000204b0024 with mutex_spinlock at 0xa80000002022d954 and mutex_spinunlock at 0xa80000002022d9a0, and ends in an indirect call (the jalr at 0xa80000002038f214) whose target the facts do not resolve. That call passes the word at offset 48 of the table entry as a0 and the word at offset 40 as a5. It looks like the dispatch point for a high-water event, but the facts do not show which target it calls.
  Reviewed: 2026-11-12 (sonnet)
