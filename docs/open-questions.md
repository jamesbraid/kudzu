# Open questions

Questions the binaries have not yet answered. Each entry names the routes already tried and the next route. When a question is answered, move the answer into the owning document and delete the entry; when every route is exhausted, label the fact "unknown: no shipped software or published source depends on it" there and delete the entry.

## Scaffold

| Question | Owner | Routes tried | Next route |
|---|---|---|---|
| IRIX 6.5.22 kernels and object archives: no 6.5.22 IP27, IP30 or IP35 material is held. | [IRIX_FILES.md](../IRIX_FILES.md) | Searched the held media, extracted trees and installed disk images: only an IP22 (Indy) 6.5.22m disk image and the 6.5.22 patch sets exist. | Unpack the IP22 image to see what it carries (it is not an IP27/IP30/IP35 system); look for 6.5.22 overlay media for the target platforms. |
| The IP32 linked kernel (`unix.IP32`): none held. | [IRIX_FILES.md](../IRIX_FILES.md) | The IP32 object archives are present and inventoried. | Locate an installed O2 system disk, or link the IP32 objects the way the installer does. |
| `sysmp` and other selector tables are not generated; `syssgi` selectors that never narrow to a single value (shared error paths and ranges) are not listed. | `docs/inventory/selectors.tsv` | `scripts/syssgi_selectors.py` on `syssgi`; compared with public lists in [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md) | Run the extractor on the other selector functions; explain the selectors that differ from the public lists. |
| Panic and assert message strings are not recovered; `panics.tsv` lists call sites only. | `scripts/os_tables.py` | none yet | Resolve the argument that precedes each call to `panic`, `cmn_err` or `assfail` to a string. |

## KLCONFIG and node numbering

| Question | Owner | Routes tried | Next route |
|---|---|---|---|
| Which IP35 node-controller registers sit at `0x9000000034000000` and `0x9000000014000000` (plus the NASID shifted left 33), and what bits 12 to 15 encode. `get_module_id`, `is_master_baseio` and `do_cpumask` treat `0xc` at the first as a PE-brick; `cpu_node_probe` treats 8 at the second as a PROM without PE-brick support. | [klconfig.md](klconfig.md) | The 6.5.30 IP35 kernel's uses of both addresses; Linux has no IP35 support. | Find every other reader of the two addresses in `6.5.30/unix.IP35` and the IP35 PROM image, and compare with the published Origin 3000-family node-controller documentation. |
