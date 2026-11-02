# Open questions

Questions the binaries have not yet answered. Each entry names the routes already tried and the next route. When a question is answered, move the answer into the owning document and delete the entry; when every route is exhausted, label the fact "unknown: no shipped software or published source depends on it" there and delete the entry.

## Triage (2026-11-01)

Seven entries: 4 software-now, 2 missing-input, 1 hardware-only.

Route classes:

- **software-now**: the next route reads or runs something already held, in a 6.5.30 or 6.5.22 binary or a held file.
- **missing-input**: the next route needs a release, binary or configuration file that is not held. The entry names it.
- **hardware-only**: no shipped software or published source can answer it, so it is recorded and not scheduled. One entry is in this class (the SCSI CD-ROM section).

Premises corrected in place, with the check that corrected them:

- The 6.5.22 IP27, IP30 and IP35 linked kernels are held and in the inventory; [version-diffs.md](version-diffs.md) compares them with 6.5.30.
- Panic messages are resolved for 361 of the 890 call sites, not none (row 3).

Software-now, in order:

1. The gp slots read by the hardware-facing layout routines of the 6.5.22 and 6.5.30 kernels: resolve each slot to its variable in both kernels (see the 6.5.22 comparison section).
2. The panic messages: extend `scripts/panic_sites.py` to the 529 sites marked not resolved.
3. The `sysmp` and `syssgi` selector tables: run the extractor on the other selector functions. These are out of the current scope rules, so they come last.
4. The CD-ROM class table in the media daemon's device module: read the routines at `0x01cf2c88`, `0x01cf31d8` and `0x01cf32f8` to the end ([scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md)).

Missing input:

- The IP32 linked kernel (row 1): no IP32 configuration is held for either release.
- The Octane2, Origin 350, Tezro and Fuel base-I/O PROMs, the IP22 PROM and an installed 6.5.22 `mediad` with its device module: none held ([scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md)).

Hardware-only:

- What opcodes `0xc4` and `0xc9` do to a Toshiba or Sony drive. Routes tried: both PROM families, both kernels, the media daemon and `scsicontrol` send them but nothing reveals their effect. Next route: a vendor drive manual ([scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md)).

## Scaffold

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| The IP32 linked kernel (`unix.IP32`): none held, and no IP32 configuration is held for either release. | missing-input | [IRIX_FILES.md](../IRIX_FILES.md) | The IP32 object archives are present and inventoried (230 entries in the 6.5.30 IP32 boot directory). `root/6.5.30` has no `var/`. `root/6.5.22/var/` has `sysgen.IP27`, `sysgen.IP30` and `sysgen.IP35` only. | Extract the IP32 configuration (`master.d`, `system`, `mtune`) from the installation media named in IRIX_FILES.md if they carry it, then link. Otherwise locate an installed O2 system disk. IP32 has no kernel row in `scripts/scope.json`. |
| `sysmp` and other selector tables are not generated; `syssgi` selectors that never narrow to a single value (shared error paths and ranges) are not listed. | software-now | `docs/inventory/selectors.tsv` | `scripts/syssgi_selectors.py` on `syssgi`; compared with public lists in [syscall-selector-crosscheck.md](syscall-selector-crosscheck.md). Checked 2026-11-01: `sysmp` and `syssgi` are named-only, out-of-scope rows in the 6.5.22 `os.a` objects of all five platforms. | Run the extractor on the other selector functions, starting with `sysmp` in the 6.5.30 and 6.5.22 `os.a` objects. Explain the selectors that differ from the public lists. |
| Panic and assert message strings: 529 of the 890 call sites in [panics.md](panics.md) are `not resolved`. | software-now | `scripts/os_tables.py`, `scripts/panic_sites.py` | `scripts/panic_sites.py` resolves the format register at each call to a string in the file-backed data, which gives the 361 resolved sites. The 529 unresolved sites are the ones the scan cannot follow: two paths join before the call, or the pointer is loaded at run time (the Grades paragraph of panics.md). Checked 2026-11-01 by counting the table rows. | Extend the scan to follow the two-path joins and the loaded pointers, then recount. Add the 6.5.22 kernels to the table once row 1 lands. |

## 6.5.22 comparison

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| Whether the gp slots that the hardware-facing layout routines read name the same variables in 6.5.22 as in 6.5.30. The routines differ only in gp offsets (106 hardware-facing rows, for example `nasid_to_compact_nodeid`, `cnode_exists` and `hub_slot_to_widget`) or in gp offsets and jump targets (1,080 rows); see [version-diffs-queue.tsv](version-diffs-queue.tsv). | software-now | [version-diffs.md](version-diffs.md) | The bytes match apart from those fields, so the routines are classed as layout. The gp offset differs, which fits a moved data layout, but no slot was resolved to its variable in either kernel. | For each such row, resolve gp plus the offset to the data symbol in each kernel (the `_gp` symbol gives the base in each kernel) and compare the two names. |


## Hardware graph

| Question | Class | Owner | Routes tried | Next route |
|---|---|---|---|---|
| What the 6.5.30 IP35 crossbar vertex routine (`klhwg_add_xbow` at 0xc000000000072f34, 616 bytes) does beyond the port 8 to 15 scan it shares with IP27: it also calls an I/O-brick registration routine and two module-to-node routines that the IP27 routine lacks. | software-now | [hwgraph.md](hwgraph.md) | Disassembled for its port range, board-type search (type 66) and callee list; the body past the port scan was not read. | Read the whole routine with `scripts/facts.py`, name each callee, and add an IP35 entry to `hwgraph.md` beside the IP27 one. |
| How the kernels assign and persist device unit numbers through the ioconfig file (the "unit number %d from ioconfig is invalid" and Gigabit Ethernet unit-limit messages are in `unix.IP27` and `unix.IP35`). No page here describes the numbering. | software-now | none yet | Searched the kernel strings for `ioconfig`: four messages, all in network driver attach paths. | Find the readers of those messages in the 6.5.30 kernels, then the user-space `ioconfig` binary in `root/`, and write a page on how the file's numbers reach the hardware graph. |
