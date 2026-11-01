# Open questions

Questions the binaries have not yet answered. Each entry names the routes already tried and the next route. When a question is answered, move the answer into the owning document and delete the entry; when every route is exhausted, label the fact "unknown: no shipped software or published source depends on it" there and delete the entry.

## Scaffold

| Question | Owner | Routes tried | Next route |
|---|---|---|---|
| IRIX 6.5.22 kernels and object archives: no 6.5.22 IP27, IP30 or IP35 material is held. | [IRIX_FILES.md](../IRIX_FILES.md) | Searched the held media, extracted trees and installed disk images: only an IP22 (Indy) 6.5.22m disk image and the 6.5.22 patch sets exist. | Unpack the IP22 image to see what it carries (it is not an IP27/IP30/IP35 system); look for 6.5.22 overlay media for the target platforms. |
| The IP32 linked kernel (`unix.IP32`): none held. | [IRIX_FILES.md](../IRIX_FILES.md) | The IP32 object archives are present and inventoried. | Locate an installed O2 system disk, or link the IP32 objects the way the installer does. |
| `sysmp` and other selector tables are not generated; `syssgi` selectors that never narrow to a single value (shared error paths and ranges) are not listed. | `docs/inventory/selectors.tsv` | `scripts/syssgi_selectors.py` on `syssgi`; compared with public lists in [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md) | Run the extractor on the other selector functions; explain the selectors that differ from the public lists. |
| The five routines that `sgi_mconfig` calls (command 1 to 5, at 0xc0000000001d07fc, 0xc0000000001d098c, 0xc0000000001d0e34, 0xc0000000001d1068 and 0xc0000000001d12c8 in the IP27 kernel) have no symbol, so `docs/syssgi.md` cannot say what they do. The method `vtopv` calls through the address-space object, and the registers behind the uncached addresses that `numa_tune`, `numalinkstats_up2date`, `set_leds` and `heart_rev` use, are also unidentified. | [syssgi.md](syssgi.md) | `facts.py` on the IP27 kernel found no function symbol at the five addresses; the callers were read in full. | Read the five routines by address with `facts.py --size`, and follow the address-space method table from the object `as_lookup_current` returns. |
| Panic and assert message strings are not recovered; `panics.tsv` lists call sites only. | `scripts/os_tables.py` | none yet | Resolve the argument that precedes each call to `panic`, `cmn_err` or `assfail` to a string. |

## KLCONFIG and node numbering

| Question | Owner | Routes tried | Next route |
|---|---|---|---|
| Which IP35 node-controller registers sit at `0x9000000034000000` and `0x9000000014000000` (plus the NASID shifted left 33), and what bits 12 to 15 encode. `get_module_id`, `is_master_baseio` and `do_cpumask` treat `0xc` at the first as a PE-brick; `cpu_node_probe` treats 8 at the second as a PROM without PE-brick support. | [klconfig.md](klconfig.md) | The 6.5.30 IP35 kernel's uses of both addresses; Linux has no IP35 support. | Find every other reader of the two addresses in `6.5.30/unix.IP35` and the IP35 PROM image, and compare with the published Origin 3000-family node-controller documentation. |

## Interrupt routing

| Question | Owner | Routes tried | Next route |
|---|---|---|---|
| Whether a write to the first IP35 processor interface's pending-modify register (`0x9200000001000090`) clears a level pending on the second interface (`0x200000` higher). `intr_clear_all` reads both interfaces' pending registers but clears every set bit through the first. | [interrupts.md](interrupts.md#ip35-differences) | Read `intr_clear_all` and its clear loop in the 6.5.30 IP35 kernel; Linux has no IP35 support. | Look for another IP35 routine (error handlers, the interrupt entry path) that clears a second-interface level, and for published Origin 300 node-controller register descriptions. |
