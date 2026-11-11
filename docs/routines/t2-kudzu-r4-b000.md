# Tier-2 routine readings, kudzu batch 0

Routines from the first 25 targets of the tier-2 list for kudzu, read from the rendered `6.5.22/unix.IP27` facts. Each role names the addresses, callees and constants it comes from. Entries that only forward their arguments to another routine are marked established, since the rendered facts settle that behavior.

- `net_link_up` in `6.5.22/unix.IP27` at 0xc00000000002e2f0
  **Inferred.** Named `net_link_up`, this 20-byte leaf makes no calls. It loads one doubleword from the uncached address 0x9200000001600000 at 0xc00000000002e2f4 and returns at 0xc00000000002e2fc, with v0 not tied to that load in the rendered facts. The name points to a network link-status read, which a single device-register load would fit.

- `hub_piomap_free` in `6.5.22/unix.IP27` at 0xc00000000003b5ac
  **Inferred.** Named `hub_piomap_free`, this 232-byte routine releases a PIO map record. It reads fields from the record at a0+0x20 through a0+0x28 and tests bits 0x2 and 0x4 of the word at a0+0x28. It takes a spinlock through mutex_spinlock at 0xc00000000003b5e4, stores a doubleword into the table at 0x9200000001c00160 at 0xc00000000003b640, releases through mutex_spinunlock at 0xc00000000003b60c, and wakes waiters with sv_signal at 0xc00000000003b674. The lock, table store, unlock and wakeup sequence is the shape of a release.

- `hub_dmamap_alloc` in `6.5.22/unix.IP27` at 0xc00000000003b7c8
  **Inferred.** Named `hub_dmamap_alloc`, this 128-byte routine allocates a 16-byte descriptor with kern_malloc at 0xc00000000003b800 and fills it. It looks up widget data through xwidget_info_get at 0xc00000000003b7d8, xwidget_info_id_get at 0xc00000000003b7e4 and xwidget_info_master_get at 0xc00000000003b7f0, then stores words at offsets 0x0, 0x8 and 0xc and a byte at offset 0x4. It returns the descriptor pointer, and a bit-0 test on a3 at 0xc00000000003b830 selects one of two paths.

- `hub_intr_cpu_get` in `6.5.22/unix.IP27` at 0xc00000000003be48
  **Inferred.** Named `hub_intr_cpu_get`, this 28-byte routine reads a halfword at a0+0x24 at 0xc00000000003be48. It loads a pointer from the table at 0xc0000000014d39a8 and returns the word at offset 0x734 (1844) of the structure it points to, at 0xc00000000003be60 and 0xc00000000003be5c. The halfword does not feed the return expression, so the lookup reads a fixed global table, not one indexed by the argument.

- `hub_setup_prb` in `6.5.22/unix.IP27` at 0xc00000000003c028
  **Inferred.** Named `hub_setup_prb`, this 148-byte routine tests the word at 0xc0000000014c9c9c (branch at 0xc00000000003c030) and a second condition on a5 at 0xc00000000003c054. Under those tests it does a read-modify-write of one doubleword in the table at 0x9200000001c00198, loading at 0xc00000000003c078 and storing at 0xc00000000003c0b8. It rewrites one fixed table entry under two flag tests, which fits a one-time setup step.

- `get_my_slotid` in `6.5.22/unix.IP27` at 0xc00000000003ca64
  **Inferred.** Named `get_my_slotid`, this 36-byte routine loads a doubleword from the uncached address 0x9200000001220048 at 0xc00000000003ca68. It passes its arguments to hub_slotbits_to_slot at 0xc00000000003c7ec and returns that result at 0xc00000000003ca80. The register at 0x9200000001220048 therefore supplies the slot bits that get converted.

- `intrgroup_init` in `6.5.22/unix.IP27` at 0xc000000000049fc0
  **Inferred.** Named `intrgroup_init`, this 112-byte routine stores doublewords to 0xc0000000014dd2a0 and 0xc0000000014dd2a8 at 0xc000000000049fd8 and 0xc000000000049fe4. It calls spinlock_init at 0xc000000000049fe0 and again at 0xc00000000004a008 on each pass of a loop. The loop continues while the counter s0 is below 0x10 (branch at 0xc00000000004a018), so it initializes a set of interrupt-group locks, going by the name.

- `_check_dbg` in `6.5.22/unix.IP27` at 0xc00000000005b4b8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4b8. It makes no calls and no memory accesses, so it is a no-op that always reports zero, and the name suggests a debug-check hook.

- `tpusim_version` in `6.5.22/unix.IP27` at 0xc00000000005b4c0
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4c0 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator version query.

- `tpusim_info_chk` in `6.5.22/unix.IP27` at 0xc00000000005b4c8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4c8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator info check.

- `tpusim_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000005b4d0
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4d0 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator PIO translation hook.

- `tpusim_dmatrans_list` in `6.5.22/unix.IP27` at 0xc00000000005b4d8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4d8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator DMA translation hook.

- `tpusim_intr_alloc` in `6.5.22/unix.IP27` at 0xc00000000005b4e0
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4e0 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator interrupt allocation hook.

- `tpusim_intr_connect` in `6.5.22/unix.IP27` at 0xc00000000005b4e8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4e8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a simulator interrupt connect hook.

- `tpuidbg_version` in `6.5.22/unix.IP27` at 0xc00000000005b4f8
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000005b4f8 with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a debug-interface version query.

- `xtalk_piomap_alloc` in `6.5.22/unix.IP27` at 0xc00000000006d740
  **Established.** Its only action is a call to hub_piomap_alloc at 0xc00000000003b280, reached by the jal at 0xc00000000006d748 with a0 through a3 as they arrived. The v0 returned at 0xc00000000006d754 is that callee's result, so the routine is a pass-through to the hub implementation.

- `xtalk_piomap_free` in `6.5.22/unix.IP27` at 0xc00000000006d75c
  **Established.** The body is one jal, to hub_piomap_free at 0xc00000000003b5ac, at 0xc00000000006d764. Arguments a0 to a3 reach the callee untouched, and the value returned at 0xc00000000006d770 is the callee's v0.

- `xtalk_piomap_addr` in `6.5.22/unix.IP27` at 0xc00000000006d778
  **Established.** A single call at 0xc00000000006d780 reaches hub_piomap_addr at 0xc00000000003b694. The return at 0xc00000000006d78c hands back whatever that callee left in v0, with no extra handling of its arguments.

- `xtalk_piomap_done` in `6.5.22/unix.IP27` at 0xc00000000006d794
  **Established.** The one call, at 0xc00000000006d79c, goes to hub_piomap_done at 0xc00000000003b6f4. Nothing else in the 28 bytes touches memory, and v0 at 0xc00000000006d7a8 carries the callee result out.

- `xtalk_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000006d7b0
  **Established.** The jal at 0xc00000000006d7b8 targets hub_piotrans_addr at 0xc00000000003b6fc, receiving a0 through a3 as passed in. The value returned at 0xc00000000006d7c4 comes from that routine's v0, so this is a direct alias for the hub translation.

- `xtalk_pio_addr` in `6.5.22/unix.IP27` at 0xc00000000006d7cc
  **Inferred.** This 248-byte routine chains the translation and mapping calls. It first calls xtalk_piotrans_addr at 0xc00000000006d820, and when that result is zero it falls back to xtalk_piomap_alloc at 0xc00000000006d86c. It then calls xtalk_piomap_addr at 0xc00000000006d88c and, when that returns zero, releases the map with xtalk_piomap_free at 0xc00000000006d8b4. It stores doublewords through the pointer in a4 at 0xc00000000006d808 and 0xc00000000006d8a4. The order is translate first, and map on failure.

- `null_xtalk_early_piotrans_addr` in `6.5.22/unix.IP27` at 0xc00000000006d8ec
  **Established.** This 8-byte stub returns the constant 0 at 0xc00000000006d8ec with no calls or memory accesses. It is a no-op that always reports zero, and the name suggests a placeholder for the piotrans path used before the translation layer is up.

- `xtalk_dmamap_alloc` in `6.5.22/unix.IP27` at 0xc00000000006d8f4
  **Established.** At 0xc00000000006d8fc it calls hub_dmamap_alloc at 0xc00000000003b7c8 with the entry arguments, then returns its v0 at 0xc00000000006d908. There is no other work in the routine.

- `xtalk_dmamap_free` in `6.5.22/unix.IP27` at 0xc00000000006d910
  **Established.** The call at 0xc00000000006d918 goes to hub_dmamap_free at 0xc00000000003b848 with the entry registers, and v0 from it is returned at 0xc00000000006d924. The routine has no other calls.

- `xtalk_dmamap_addr` in `6.5.22/unix.IP27` at 0xc00000000006d92c
  **Established.** A call at 0xc00000000006d934 reaches hub_dmamap_addr at 0xc00000000003b870, passing a0 through a3 as they entered. The v0 returned at 0xc00000000006d940 is the callee's, with nothing else done in the routine.
