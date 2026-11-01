# Open questions

Questions the binaries have not yet answered. Each entry names the routes already tried and the next route. When a question is answered, move the answer into the owning document and delete the entry; when every route is exhausted, label the fact "unknown: no shipped software or published source depends on it" there and delete the entry.

## Triage (2026-11-01)

Seven entries: 6 software-now, 1 missing-input, 0 hardware-only.

Route classes:

- **software-now**: the next route reads or runs something already held, in a 6.5.30 or 6.5.22 binary or a held file.
- **missing-input**: the next route needs a release, binary or configuration file that is not held. The entry names it.
- **hardware-only**: no shipped software or published source can answer it, so it is recorded and not scheduled. No entry is in this class.

Premises corrected in place, with the check that corrected them:

- The 6.5.22 IP27, IP30 and IP35 linked kernels are held (row 1).
- Panic messages are resolved for 361 of the 890 call sites, not none (row 5).

Software-now, in order:

1. The 6.5.22 linked kernels: regenerate the inventory and `scripts/inventory-root.sha256` so the three kernels that `scripts/scope.json` already names enter the tables.
2. The IP35 node-controller registers at the two KLCONFIG addresses: read every other reader of both addresses in the 6.5.30 and 6.5.22 IP35 kernels.
3. The IP35 second-interface pending clear: read the other IP35 writers of a second-interface level in both IP35 kernels.
4. The panic and assert messages: extend `scripts/panic_sites.py` to the 529 sites marked not resolved.
5. The five `sgi_mconfig` routines: read them by address in the 6.5.30 and 6.5.22 IP27 kernels.
6. The `sysmp` and `syssgi` selector tables: run the extractor on the other selector functions. These are out of the current scope rules, so they come last.

Missing input:

- The IP32 linked kernel (row 2): no IP32 configuration is held for either release.

## Scaffold

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| IRIX 6.5.22 linked kernels: `unix.IP27`, `unix.IP30` and `unix.IP35` are held, but no inventory row or root hash covers them. The 6.5.22 IP32 kernel is not held. | software-now | [IRIX_FILES.md](../IRIX_FILES.md) | Checked 2026-11-01: the three kernels are ELF executables that contain the release string `6.5.22m` (7978 times in IP27, 8188 in IP35). `docs/inventory/functions.tsv` has no `6.5.22/unix` rows, and `scripts/inventory-root.sha256` has no 6.5.22 kernel line. Earlier: only an IP22 (Indy) 6.5.22m disk image and the 6.5.22 patch sets were found. | Regenerate the inventory and the root hash so the three kernels enter the rows `scripts/scope.json` already names. Then correct IRIX_FILES.md section 2 and retire the IP22 image route. |
| The IP32 linked kernel (`unix.IP32`): none held, and no IP32 configuration is held for either release. | missing-input | [IRIX_FILES.md](../IRIX_FILES.md) | The IP32 object archives are present and inventoried (230 entries in the 6.5.30 IP32 boot directory). `root/6.5.30` has no `var/`. `root/6.5.22/var/` has `sysgen.IP27`, `sysgen.IP30` and `sysgen.IP35` only. | Extract the IP32 configuration (`master.d`, `system`, `mtune`) from the installation media named in IRIX_FILES.md if they carry it, then link. Otherwise locate an installed O2 system disk. IP32 has no kernel row in `scripts/scope.json`. |
| `sysmp` and other selector tables are not generated; `syssgi` selectors that never narrow to a single value (shared error paths and ranges) are not listed. | software-now | `docs/inventory/selectors.tsv` | `scripts/syssgi_selectors.py` on `syssgi`; compared with public lists in [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md). Checked 2026-11-01: `sysmp` and `syssgi` are named-only, out-of-scope rows in the 6.5.22 `os.a` objects of all five platforms. | Run the extractor on the other selector functions, starting with `sysmp` in the 6.5.30 and 6.5.22 `os.a` objects. Explain the selectors that differ from the public lists. |
| The five routines that `sgi_mconfig` calls (command 1 to 5, at 0xc0000000001d07fc, 0xc0000000001d098c, 0xc0000000001d0e34, 0xc0000000001d1068 and 0xc0000000001d12c8 in the IP27 kernel) have no symbol, so `docs/syssgi.md` cannot say what they do. The method `vtopv` calls through the address-space object, and the registers behind the uncached addresses that `numa_tune`, `numalinkstats_up2date`, `set_leds` and `heart_rev` use, are also unidentified. | software-now | [syssgi.md](syssgi.md) | `facts.py` on the IP27 kernel found no function symbol at the five addresses; the callers were read in full. Checked 2026-11-01: none of the five addresses falls inside a symbol range in the 6.5.30 IP27 inventory. They lie in the unnamed gap that starts where `mload_init` ends (0xc0000000001d07fc) and ends where the next symbol, `mload_driver_load`, begins (0xc0000000001d2d78). | Read the five routines by address with `facts.py --size`, and follow the address-space method table from the object `as_lookup_current` returns. Compare the same routines in the 6.5.22 IP27 kernel. |
| Panic and assert message strings: 529 of the 890 call sites in [panics.md](panics.md) are `not resolved`. | software-now | `scripts/os_tables.py`, `scripts/panic_sites.py` | `scripts/panic_sites.py` resolves the format register at each call to a string in the file-backed data, which gives the 361 resolved sites. The 529 unresolved sites are the ones the scan cannot follow: two paths join before the call, or the pointer is loaded at run time (the Grades paragraph of panics.md). Checked 2026-11-01 by counting the table rows. | Extend the scan to follow the two-path joins and the loaded pointers, then recount. Add the 6.5.22 kernels to the table once row 1 lands. |

## KLCONFIG and node numbering

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| Which IP35 node-controller registers sit at `0x9000000034000000` and `0x9000000014000000` (plus the NASID shifted left 33), and what bits 12 to 15 encode. `get_module_id`, `is_master_baseio` and `do_cpumask` treat `0xc` at the first as a PE-brick; `cpu_node_probe` treats 8 at the second as a PROM without PE-brick support. | software-now | [klconfig.md](klconfig.md) | The 6.5.30 IP35 kernel's uses of both addresses. Checked 2026-11-01: `get_module_id`, `is_master_baseio`, `chk_for_pebrick`, `cpu_node_probe` and `do_cpumask` are in the 6.5.30 IP35 inventory as understood. No IP35 PROM image is held under `root/`. "Linux has no IP35 support" is not checked: the Linux submodule is not initialised in this clone. | Find every other reader of the two addresses in `6.5.30/unix.IP35` and `6.5.22/unix.IP35` (row 1 of the scaffold table). The PROM image route is missing-input until a PROM image is held. The published Origin 3000-family node-controller documentation is a hint only. |

## Interrupt routing

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| Whether a write to the first IP35 processor interface's pending-modify register (`0x9200000001000090`) clears a level pending on the second interface (`0x200000` higher). `intr_clear_all` passes both interfaces' pending registers to a helper that clears every set bit through the first interface's modify register. | software-now | [interrupts.md](interrupts.md#ip35-differences) | Read `intr_clear_all` and its clear loop in the 6.5.30 IP35 kernel; Linux has no IP35 support. Checked 2026-11-01: `intr_clear_all` (0xc0000000000610d8, 316 bytes) is in the 6.5.30 IP35 inventory as understood. The kernel-side accesses are established in interrupts.md. The node controller's side is not in any binary. | Look for other IP35 routines (error handlers, the interrupt entry path) that write a second-interface level, in `6.5.30` and `6.5.22`. If the controller's behaviour stays unsettled, label that half "unknown: no shipped software or published source depends on it". Published Origin 300 node-controller register descriptions are a hint only. |

## Hardware graph

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| What the 6.5.30 IP35 crossbar vertex routine (`klhwg_add_xbow` at 0xc000000000072f34, 616 bytes) does beyond the port 8 to 15 scan it shares with IP27: it also calls an I/O-brick registration routine and two module-to-node routines that the IP27 routine lacks. | software-now | [hwgraph.md](hwgraph.md) | Disassembled for its port range, board-type search (type 66) and callee list; the body past the port scan was not read. | Read the whole routine with `scripts/facts.py`, name each callee, and add an IP35 entry to `hwgraph.md` beside the IP27 one. |
| How the kernels assign and persist device unit numbers through the ioconfig file (the "unit number %d from ioconfig is invalid" and Gigabit Ethernet unit-limit messages are in `unix.IP27` and `unix.IP35`). No page here describes the numbering. | software-now | none yet | Searched the kernel strings for `ioconfig`: four messages, all in network driver attach paths. | Find the readers of those messages in the 6.5.30 kernels, then the user-space `ioconfig` binary in `root/`, and write a page on how the file's numbers reach the hardware graph. |
