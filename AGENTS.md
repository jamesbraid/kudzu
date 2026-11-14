# IRIX 6.5 reverse engineering and specification

This repository documents how IRIX 6.5.30 and IRIX 6.5.22 work, from the shipped binaries, as specifications that other projects (datasheets, drivers, debuggers) can build on. It holds no reimplementation. The layout and working style follow the sibling repositories for the graphics and chip work: a gitignored `root/` of IRIX files, an `evidence/irix-files.md` that says what goes in it, generated inventory tables that measure what is covered, and a claim checker that compares written facts with the binaries.

## 1. Scope

The long-term scope is all of IRIX and its add-ons: the full kernel, CXFS, ONC RPC and NFS, the base applications and libraries. The inventory has to be able to grow to that, so every binary of the releases may be listed. What is **in scope now** is the whole kernel: the linked kernels (`unix.IP27`, `unix.IP30`, `unix.IP35`) and every object of the platform boot directories, with the hardware-facing subsystems (PROM interface and KLCONFIG parsing, hardware graph, interrupt routing, NUMA and memory initialisation, topology checks, the device attach framework) and the generic kernel (processes, scheduler, virtual memory, file systems, networking, STREAMS, IPC, system calls). The rule for each binary group is in [tools/config/scope.json](tools/config/scope.json) and reported in [evidence/inventory/scope.tsv](evidence/inventory/scope.tsv) and `summary.tsv`.

User-space binaries and the optional subsystems are out of scope **for now**, not excluded: they stay in the inventory with an `out` scope, and move in when a rule in `tools/config/scope.json` changes. A function, structure or call site that matches an in-scope rule counts toward the finish line in section 6; one that does not is still listed so the totals stay honest.

Platforms: IP27 (Origin 200/2000, Onyx2), IP30 (Octane), IP35 (Origin 300/350, Onyx 300/350) and IP32 (O2) where the media has them. Releases: 6.5.30 is the primary release; 6.5.22 has its kernel object archives, its linked IP27, IP30 and IP35 kernels, and the userland set that [evidence/irix-files.md](evidence/irix-files.md) lists, in `root/` (see [evidence/irix-files.md](evidence/irix-files.md) and [evidence/version-diffs.md](evidence/version-diffs.md)). A fact from one release or platform does not fill a gap in another.

## 2. Evidence

The **shipped 6.5.30 and 6.5.22 binaries are the target.** Each fact is taken from the highest source that states it, in the [evidence order](#evidence-order) at the end of this file, and graded by that source. A fact the IRIX source tree defines is established, and is cited by path in that tree. Where a shipped 6.5.30 or 6.5.22 binary confirms it, cite the binary too, at an address, offset or symbol. The trees differ from the shipped releases, so cite the release a source fact was read from.

The public IRIX 6.5.7m tree (`references/code/irix-657m-src`) and the other trees in [references/documents/published-sources.md](references/documents/published-sources.md) are sources under that order. Published SGI documents and man pages, public drivers and emulators, and other reverse-engineering projects rank below them. A note from another project may confirm or contradict a fact, but never supplies one alone.

Rules for what is written down:

- Findings go in your own words, graded **established** (read in full in a source or a binary: the whole function body, table or section, with a citation that names its binary and address), **inferred** (a reasoned reading; say the reasoning) or **unknown**. Anything unlabelled is unknown. Never reword an entry to change its grade.
- No source text, comments or internal identifiers from any source tree go into the repository. A source file path is cited as the evidence for a fact, not copied as a layout of the tree. Symbol names that are present in the shipped binaries' own symbol tables or debug information are fine, and so are names in published headers and manuals.
- A fact that no software reveals is labelled "unknown: no shipped software or published source depends on it", with the routes tried. A reimplementer then chooses a value and documents the choice.
- There is no exposure or provenance record in this repository. The guard is the check described in section 5.
- Do not paste proprietary binaries, disassembly of whole files, or source into the repository. Short quoted instruction sequences and tables derived from the binaries are fine.
- No commit trailers, and no names of the people or tools involved, in commits or files.

## 3. Layout

- `root/` is gitignored. It mirrors the install of each release under `root/<release>/` (for example `root/6.5.30/usr/cpu/sysgen/IP27boot/kernel.o`) and holds the linked kernels as `root/<release>/unix.IPnn`. [evidence/irix-files.md](evidence/irix-files.md) lists the paths and which media each came from.
- `software/` holds the findings about IRIX itself: `software/kernel/` (one page per subsystem, platform or topic, with `structs/` for the generated structure layouts), `software/video/` (the O2 graphics and video drivers and the DM2 video boards) and `software/userland/` (the base programs, the ABI and the libraries).
- `evidence/` holds what backs the findings: [evidence/coverage.md](evidence/coverage.md) (the map and the measured totals), the generated [evidence/inventory/](evidence/inventory/README.md), `evidence/routines/` (the routine readings), the instruction readings, the cross-checks and comparison tables, and [evidence/irix-files.md](evidence/irix-files.md). [questions/README.md](questions/README.md) lists the open questions; `references/documents/` says which public trees are pinned and `references/code/` holds them.
- `include/` and per-area directories hold C headers that describe structure layouts and constants, each field commented with its grade and derivation.
- `formats/<name>/` holds the facts for the disk, hub and board structures (the volume header, the EFS, XFS, XLV and XVM labels, the KLCONFIG records and the SCSI constants) and for a few kernel software tables (the gigabit Ethernet match table, the Ethernet address record read from a one-wire part, the pixel-DMA request, the GBE board description bytes and the R10000 TLB wired-entry layout): `format.toml` is the source, one per unit, `README.md` is the prose that explains it and `format.md` is the generated reference page. The register maps of the hardware IRIX drives (the hub, PCI bridge, one-wire parts, R10000 timer, and the O2's CRIME, GBE and MACE video blocks) live in the chips and crime repositories; [software/kernel/hardware-register-use.md](software/kernel/hardware-register-use.md) keeps what the kernel does with them. Each item carries a grade and a source. `generated/` is generated from the `format.toml` files and is never edited by hand: `generated/c`, `generated/rust`, `generated/python`, `generated/json` and `generated/xml` are the machine outputs. To change a fact, edit the TOML, run `retools/tools/defs/defs-gen`, and commit both.
- `tools/` holds this repository's own generators (`errno_table.py`, `gen-hw-windows.py` and the like), `check-os-tables` and `setup-clone.sh`; `tools/config/` holds their data and settings (`scope.json`, `inventory-config.json` and the like). The shared tooling is the `retools/` submodule (sgi-retools): `retools/tools/` has the inventory, fact sheet, container definitions and wrappers, and `retools/tools/kudzu/` has the tables specific to this repository (`os_tables.py`, `gen-scope.py`, `panic_sites.py`, `struct_fields.py`). Run them from the repository root; they find this repository from the current directory.
- Public sources are pinned submodules under `references/code/` ([references/documents/published-sources.md](references/documents/published-sources.md) says why each pin).

## 4. Tools

Everything runs in a container built from `retools/tools/Containerfile` (MIPS binutils, dwarfdump, pyelftools, capstone, unicorn, spimdisasm). `tools/README.md` has the commands: `retools/tools/re-run`, the Ghidra image and `--ghidra-export`, the inventory and the copy check.

Tables (all generated; never edit them by hand):

- `binaries.tsv`, `functions.tsv`, `ioctls.tsv` from `retools/tools/inventory.py` (shared with the sibling repositories; configuration in `tools/config/inventory-config.json`, which `retools/tools/kudzu/gen-scope.py` partly writes from `tools/config/scope.json` and `tools/gen-hw-windows.py` partly writes from `tools/config/hw-windows.json`); The prose pages it reads are the `docs.prose` list of that file, in reading order: when two pages describe the same item, the first one read is the one a table row cites. A new page under `software/kernel/`, `software/userland/` or `software/video/` is picked up by the globs at the end of the list.
- `syscalls.tsv`, `structs.tsv`, `panics.tsv`, `selectors.tsv`, `formats.tsv`, `summary-os.tsv` from `retools/tools/kudzu/os_tables.py`;
- `scope.tsv` and `summary.tsv`, which report the in-scope rule per group and the in-scope and out-of-scope counts.

## 5. Checks before every push

- The copy check in `tools/README.md`: a fingerprint comparison of the C in this repository and an identifier comparison of the prose against a stored index of a reference tree. It never prints reference source. Read every lead; a lead is judged by opening the pair, not by the score.
- `retools/tools/kudzu/gen-scope.py --check` and `tools/gen-hw-windows.py --check`.
- `retools/tools/defs/ci-check`, the same check CI runs. After a change to a `format.toml`, `retools/tools/defs/defs-gen --check` must pass and `retools/tools/defs/defs-check --build` must print `build: ok`.

## 6. When work is finished

Every in-scope entry in every inventory table is `understood` (a label, a role and effect, and a citation specific to the item: release, binary, address or offset, callee) or `no-software-dependency` (the section 2 label, with the routes tried). `located` and `named-only` are partial. The repository is not finished while any table has an in-scope row below that, and the long-term finish moves with the scope. Regenerate the inventory in the same commit as any document or header that explains an item.

An open question is not a stopping point: record it in `questions/README.md` with the routes tried and the next route, then carry on.

## 7. Working here

Commit small and often, straight to `main`. Subjects are lowercase, component-prefixed imperatives (`kernel: document the klconfig board walk`); bodies say why. Take a cluster of related rows, take each to `understood`, regenerate, commit.

## Evidence order

For a register or behaviour fact, take it from the highest source that states it and grade it by that source:

1. IRIX source (public `irix-657m-src` tree): read it, write the fact in your own words, never copy source text, cite the path in the tree. A fact it defines is `established`.
2. Shipped binaries (PROMs, kernels, libraries): confirm the source matches what shipped, and the release differences (6.5.7 source, 6.5.22 and 6.5.30 binaries).
3. Public Linux, OpenBSD and NetBSD headers and drivers: `established` once the IRIX source confirms them.
4. Published SGI and vendor manuals.
5. Notes from other projects (Origami datasheets, QEMU manual pages, emulator code): may confirm or contradict, never supply a fact. A fact resting only on one stays `unknown`, with the note named.

Reset values: `established` when the source states a power-on default or PROM code relies on a reset state; `inferred` when the PROM depends on a value before its first write (cite the PROM address); a read after firmware initialisation is not a reset value and can only rule values out; otherwise `unknown`. A physical hardware read is a last resort, needs the repository owner's approval, and counts as a reset value only for a register firmware provably never writes. Emulator runs are model behaviour, never hardware evidence.

Full text and the checks that enforce it: `retools/tools/README-analysis.md#evidence-order`.
