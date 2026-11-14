# IRIX 6.5 reverse engineering and specification

This repository documents how IRIX 6.5.30 and 6.5.22 work, from the shipped binaries, as specifications that other projects (datasheets, drivers, debuggers) can build on. It holds no reimplementation. The layout follows the sibling graphics and chip repositories: a gitignored `root/` of IRIX files, generated inventory tables that measure coverage, and a claim checker that compares written facts with the binaries.

## 1. Scope

The long-term scope is all of IRIX and its add-ons: the full kernel, CXFS, ONC RPC and NFS, the base applications and libraries. The inventory may list every binary of the releases so it can grow to that. **In scope now** is the whole kernel: the linked kernels (`unix.IP27`, `unix.IP30`, `unix.IP35`) and every object of the platform boot directories, hardware-facing subsystems first (PROM interface, KLCONFIG, hardware graph, interrupts, NUMA and memory, topology checks, device attach), then the generic kernel. The rule for each binary group is in [tools/config/scope.json](tools/config/scope.json), reported in [evidence/inventory/scope.tsv](evidence/inventory/scope.tsv).

User-space binaries and optional subsystems are out of scope **for now**: they stay in the inventory as `out` and move in when a rule in `scope.json` changes. A function, structure or call site that matches an in-scope rule counts toward the finish line in section 6; one that does not is still listed so the totals stay honest. Platforms: IP27 (Origin 200/2000, Onyx2), IP30 (Octane), IP35 (Origin 300/350, Onyx 300/350) and IP32 (O2) where the media has them. 6.5.30 is the primary release; 6.5.22 has its kernel object archives, its linked IP27, IP30 and IP35 kernels and the userland set that the file list lists, in `root/` too ([evidence/irix-files.md](evidence/irix-files.md), [evidence/version-diffs.md](evidence/version-diffs.md)). A fact from one release or platform does not fill a gap in another.

## 2. Evidence

The shipped binaries are the target. The public IRIX 6.5.7m tree (`references/code/irix-657m-src`) and the other trees in [references/documents/published-sources.md](references/documents/published-sources.md) rank under the order at the end of this file. A fact the source tree defines is established and cited by path; where a shipped binary confirms it, cite the binary too, at an address, offset or symbol. The trees differ from the shipped releases, so cite the release a source fact was read from.

- Grade every finding **established** (read in full: the whole function body, table or section, cited by binary and address), **inferred** (say the reasoning) or **unknown**. Unlabelled means unknown. Never reword an entry to change its grade.
- Write findings in your own words. No source text, comments or internal identifiers from any source tree go into the repository. Cite a source path as evidence; do not copy the tree's layout. Symbol names in the shipped binaries' own tables or debug information, and names in published headers and manuals, are fine.
- There is no exposure or provenance record here. The guard is the copy check in section 5.
- A fact that no software reveals is labelled "unknown: no shipped software or published source depends on it", with the routes tried.
- Do not commit proprietary binaries, whole-file disassembly or source. Short instruction sequences and tables derived from binaries are fine.
- No commit trailers, and no names of people or tools, in commits or files.

## 3. Layout

- `root/` is gitignored: `root/<release>/` mirrors each install, with the linked kernels as `root/<release>/unix.IPnn`.
- `software/` holds findings about IRIX: `kernel/` (one page per subsystem, platform or topic; `structs/` is generated), `video/` and `userland/`.
- `evidence/` holds what backs them: [coverage.md](evidence/coverage.md), the generated [inventory](evidence/inventory/README.md), routine readings, instruction readings and cross-checks. [questions/README.md](questions/README.md) lists open questions.
- `formats/<name>/` holds one unit per disk, board or kernel table: `format.toml` is the source, `README.md` the prose, `format.md` the generated page. Each item carries a grade and a source. Register maps of hardware IRIX drives live in the chips and crime repositories; [hardware-register-use.md](software/kernel/hardware-register-use.md) keeps what the kernel does with them.
- `generated/` (C, Rust, Python, JSON, XML) comes from the TOML and is never edited. Change the TOML, run `retools/tools/defs/defs-gen`, commit both.
- `tools/` has this repository's generators (`errno_table.py`, `gen-hw-windows.py`, `check-os-tables`, `setup-clone.sh`) and `tools/config/` their settings (`scope.json`, `inventory-config.json`). The shared toolkit is the `retools/` submodule; `retools/tools/kudzu/` has the tables specific to this repository. Run everything from the repository root.
- `references/code/` holds pinned public source submodules.

## 4. Tools

Everything runs in the container built from `retools/tools/Containerfile`; [tools/README.md](tools/README.md) has the commands: `retools/tools/re-run`, the Ghidra image and `--ghidra-export`, the inventory and the copy check. The image holds MIPS binutils, dwarfdump, pyelftools, capstone, unicorn and spimdisasm. Tables are generated and never edited by hand: `binaries.tsv`, `functions.tsv`, `ioctls.tsv` from `retools/tools/inventory.py` (settings in `tools/config/inventory-config.json`; its `docs.prose` list is the reading order, and when two pages describe one item the first read is the one a row cites); `syscalls.tsv`, `structs.tsv`, `panics.tsv`, `selectors.tsv`, `formats.tsv`, `summary-os.tsv` from `retools/tools/kudzu/os_tables.py`; `scope.tsv` and `summary.tsv` from the scope rules, reporting the in-scope rule per group and the in-scope and out-of-scope counts. `gen-scope.py` writes part of `inventory-config.json` from `scope.json`, and `tools/gen-hw-windows.py` writes part of it from `hw-windows.json`. A new page under `software/kernel/`, `software/userland/` or `software/video/` is picked up by the globs at the end of `docs.prose`.

## 5. Checks before every push

- The copy check in [tools/README.md](tools/README.md): a fingerprint comparison of the C here and an identifier comparison of the prose against a stored index of a reference tree. It never prints reference source. Judge each lead by opening the pair, not by the score.
- `retools/tools/kudzu/gen-scope.py --check` and `tools/gen-hw-windows.py --check`.
- `retools/tools/defs/ci-check`, the check CI runs. After a `format.toml` change, `defs-gen --check` must pass and `defs-check --build` must print `build: ok`.

## 6. Finish line

Every in-scope row of every inventory table is `understood` (label, role and effect, and a citation specific to the item) or `no-software-dependency` (with the routes tried). `located` and `named-only` are partial. The repository is not finished while any table has an in-scope row below that, and the finish line moves with the scope. An open question is not a stopping point: record it in `questions/README.md` with the routes tried and the next route, then carry on. Regenerate the inventory in the same commit as any document or header that explains an item.

## 7. Working here

Commit small and often, straight to `main`. Subjects are lowercase, component-prefixed imperatives (`kernel: document the klconfig board walk`); bodies say why. Take a cluster of related rows to `understood`, regenerate, commit.

## Evidence order

For a register or behaviour fact, take it from the highest source that states it and grade it by that source:

1. IRIX source (public `irix-657m-src` tree): read it, write the fact in your own words, never copy source text, cite the path in the tree. A fact it defines is `established`.
2. Shipped binaries (PROMs, kernels, libraries): confirm the source matches what shipped, and the release differences (6.5.7 source, 6.5.22 and 6.5.30 binaries).
3. Public Linux, OpenBSD and NetBSD headers and drivers: `established` once the IRIX source confirms them.
4. Published SGI and vendor manuals.
5. Notes from other projects (other projects' datasheets, manual pages and emulator code): may confirm or contradict, never supply a fact. A fact resting only on one stays `unknown`, with the note named.

Reset values: `established` when the source states a power-on default or PROM code relies on a reset state; `inferred` when the PROM depends on a value before its first write (cite the PROM address); a read after firmware initialisation is not a reset value and can only rule values out; otherwise `unknown`. A physical hardware read is a last resort, needs the repository owner's approval, and counts as a reset value only for a register firmware provably never writes. Emulator runs are model behaviour, never hardware evidence.

Full text and the checks that enforce it: `retools/tools/README-analysis.md#evidence-order`.
