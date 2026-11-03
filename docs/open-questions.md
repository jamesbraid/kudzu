# Open questions

Questions the binaries have not yet answered. Each entry names the routes already tried and the next route. When a question is answered, move the answer into the owning document and delete the entry; when every route is exhausted, label the fact "unknown: no shipped software or published source depends on it" there and delete the entry.

## Triage (2026-11-01)

Six entries: 2 software-now, 3 missing-input, 1 hardware-only.

Route classes:

- **software-now**: the next route reads or runs something already held, in a 6.5.30 or 6.5.22 binary or a held file.
- **missing-input**: the next route needs a release, binary or configuration file that is not held. The entry names it.
- **hardware-only**: no shipped software or published source can answer it, so it is recorded and not scheduled. One entry is in this class (the SCSI CD-ROM section).

Premises corrected in place, with the check that corrected them:

- The 6.5.22 IP27, IP30 and IP35 linked kernels are held and in the inventory; [version-diffs.md](version-diffs.md) compares them with 6.5.30.
- Panic messages are resolved for 361 of the 890 call sites, not none (row 3).

Software-now, in order:

1. The panic messages: extend `scripts/panic_sites.py` to the 529 sites marked not resolved.
2. The `sysmp` and `syssgi` selector tables: run the extractor on the other selector functions. These are out of the current scope rules, so they come last.

Missing input:

- The IP32 linked kernel (row 1): no IP32 configuration is held for either release.
- The user-space `ioconfig` program and an `/etc/ioconfig.conf` file: neither is held for either release; the kernel side is in [ioconfig.md](ioconfig.md). Next route: extract `ioconfig` from the installation media named in [IRIX_FILES.md](../IRIX_FILES.md), then read how it assigns numbers and which control request sets the controller number from user space.
- The Octane2, Origin 350, Tezro and Fuel base-I/O PROMs, the IP22 PROM and an installed 6.5.22 `mediad` with its device module: none held ([scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md)). The CD-ROM device module (`dev_cdrom.so`) is not in `root/` for 6.5.30 either, so the routines at `0x01cf2c88`, `0x01cf31d8` and `0x01cf32f8` cannot be read to the end until it is extracted (checked 2026-11-01: no `dev_cdrom.so` or `mediad` file under `root/`).

Hardware-only:

- What opcodes `0xc4` and `0xc9` do to a Toshiba or Sony drive. Routes tried: both PROM families, both kernels, the media daemon and `scsicontrol` send them but nothing reveals their effect. Next route: a vendor drive manual ([scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md)).

## Scaffold

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| The IP32 linked kernel (`unix.IP32`): none held, and no IP32 configuration is held for either release. | missing-input | [IRIX_FILES.md](../IRIX_FILES.md) | The IP32 object archives are present and inventoried (230 entries in the 6.5.30 IP32 boot directory). `root/6.5.30` has no `var/`. `root/6.5.22/var/` has `sysgen.IP27`, `sysgen.IP30` and `sysgen.IP35` only. | Extract the IP32 configuration (`master.d`, `system`, `mtune`) from the installation media named in IRIX_FILES.md if they carry it, then link. Otherwise locate an installed O2 system disk. IP32 has no kernel row in `scripts/scope.json`. |
| `sysmp` and other selector tables are not generated; `syssgi` selectors that never narrow to a single value (shared error paths and ranges) are not listed. | software-now | `docs/inventory/selectors.tsv` | `scripts/syssgi_selectors.py` on `syssgi`; compared with public lists in [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md). Checked 2026-11-01: `sysmp` and `syssgi` are named-only, out-of-scope rows in the 6.5.22 `os.a` objects of all five platforms. | Run the extractor on the other selector functions, starting with `sysmp` in the 6.5.30 and 6.5.22 `os.a` objects. Explain the selectors that differ from the public lists. |
| Panic and assert message strings: 529 of the 890 call sites in [panics.md](panics.md) are `not resolved`. | software-now | `scripts/os_tables.py`, `scripts/panic_sites.py` | `scripts/panic_sites.py` resolves the format register at each call to a string in the file-backed data, which gives the 361 resolved sites. The 529 unresolved sites are the ones the scan cannot follow: two paths join before the call, or the pointer is loaded at run time (the Grades paragraph of panics.md). Checked 2026-11-01 by counting the table rows. | Extend the scan to follow the two-path joins and the loaded pointers, then recount. Add the 6.5.22 kernels to the table once row 1 lands. |

## Hardware graph

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| How the kernels assign and persist device unit numbers through the ioconfig file, and which control request sets a controller number from user space. | missing-input | [ioconfig.md](ioconfig.md) | The kernel side is read: where the number lives (an inventory record word), the getter and setter, the Ethernet and audio readers and both messages, and the shipped configuration fragments. The setter calls found by a whole-kernel scan are driver attach and ioctl routines, not a user-space entry; only three were read. The user-space program is not held. | Extract `ioconfig` from the installation media, read how it assigns numbers and saves them, and read the remaining 20 setter call sites to find the one reachable from user space. |
| Which physical PCI pin (INTA to INTD) of each slot reaches which Bridge or PIC interrupt bit on IP35 (Origin 350, Tezro, Origin 300). The software side is settled in [pci-intr-pin.md](pci-intr-pin.md). | missing-input | [pci-intr-pin.md](pci-intr-pin.md) | IRIX software expectation read from the 6.5.30 and 6.5.22 IP35 kernels. Full text of the Tezro hardware guide (007-4643-002) and the Origin 300 user's guide (007-4365-002): no pin routing. 007-0911-210 driver guide: pin lines as a set, no wiring. Web search for hinv or ioconfig captures: none found. The Library search backends were refused connections, so no Library-wide negative is claimed. | Read the Origin 350 user's guide 007-4566-001 for interrupt or slot tables; search for an IP35 hinv -v or ioconfig capture with a dual-function PCI card; check the IDSEL-to-slot table when the Library is back. |
