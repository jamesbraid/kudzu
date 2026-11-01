# IRIX 6.5 reverse engineering and specification

This repository documents how IRIX 6.5.30 and IRIX 6.5.22 work, from the shipped binaries, as specifications that other projects (emulator datasheets, drivers, debuggers) can build on. It holds no reimplementation. The layout and working style follow the sibling repositories for the graphics and chip work: a gitignored `root/` of IRIX files, an `IRIX_FILES.md` that says what goes in it, generated inventory tables that measure what is covered, and a claim checker that compares written facts with the binaries.

## 1. Scope

The long-term scope is all of IRIX and its add-ons: the full kernel, CXFS, ONC RPC and NFS, the base applications and libraries. The inventory has to be able to grow to that, so every binary of the releases may be listed. What is **in scope now** is narrow, and the rule for each binary group is in [scripts/scope.json](scripts/scope.json) and reported in [docs/inventory/scope.tsv](docs/inventory/scope.tsv) and `summary.tsv`:

- the PROM interface and KLCONFIG parsing;
- hardware-graph construction;
- interrupt routing;
- NUMA and memory initialisation;
- the topology checks that panic or skip nodes;
- the device attach framework;
- a kernel debugging map: structure layouts, subsystem entry points, panic and assert sites.

Everything else is out of scope **for now**, not excluded: it stays in the inventory with an `out` scope, and moves in when a rule in `scope.json` changes. A function, structure or call site that matches an in-scope rule counts toward the finish line in section 6; one that does not is still listed so the totals stay honest.

Platforms: IP27 (Origin 200/2000, Onyx2), IP30 (Octane), IP35 (Origin 300/350, Onyx 300/350) and IP32 (O2) where the media has them. Releases: 6.5.30 is the primary release; 6.5.22 has its kernel object archives and linked IP27, IP30 and IP35 kernels in `root/` (see [IRIX_FILES.md](IRIX_FILES.md) and [docs/version-diffs.md](docs/version-diffs.md)). A fact from one release or platform does not fill a gap in another.

## 2. Evidence

The **shipped 6.5.30 and 6.5.22 binaries are the evidence.** A fact is established only at an address, offset or symbol in a binary of one of those releases.

Readers may use anything else as a **hint** to find where to look: published SGI documents and man pages, public drivers and emulators, other reverse-engineering projects, and even old leaked source, including the public IRIX 6.5.7m tree (`irix-657m-src`) and the other trees in [docs/published-sources.md](docs/published-sources.md). A hint is never evidence. The targets are 6.5.22 and 6.5.30, which differ from every source tree, so every fact a hint suggests must be confirmed at an address in a 6.5.30 or 6.5.22 binary, or graded inferred or unknown.

Rules for what is written down:

- Findings go in your own words, graded **established** (read in full in a binary: the whole function body, table or section, with address), **inferred** (a reasoned reading; say the reasoning) or **unknown**. Anything unlabelled is unknown. Never reword an entry to change its grade.
- No source text, file structure, comments or internal identifiers from any source tree go into the repository. That includes source file paths and directory layouts: a DWARF record that names the file a structure was declared in is not copied. Symbol names that are present in the shipped binaries' own symbol tables or debug information are fine, and so are names in published headers and manuals.
- A fact that no software reveals is labelled "unknown: no shipped software or published source depends on it", with the routes tried. An emulator author then chooses a value and documents the choice.
- There is no exposure or provenance record in this repository. The guard is the check described in section 5.
- Do not paste proprietary binaries, disassembly of whole files, or source into the repository. Short quoted instruction sequences and tables derived from the binaries are fine.
- No commit trailers, and no names of the people or tools involved, in commits or files.

## 3. Layout

- `root/` is gitignored. It mirrors the install of each release under `root/<release>/` (for example `root/6.5.30/usr/cpu/sysgen/IP27boot/kernel.o`) and holds the linked kernels as `root/<release>/unix.IPnn`. [IRIX_FILES.md](IRIX_FILES.md) lists the paths and which media each came from. `scripts/populate-root` links per-area directories into it.
- `docs/` holds the findings (`docs/<area>.md`), [docs/coverage.md](docs/coverage.md) (the map and the measured totals), [docs/open-questions.md](docs/open-questions.md) and the generated [docs/inventory/](docs/inventory/README.md).
- `include/` and per-area directories hold C headers that describe structure layouts and constants, each field commented with its grade and derivation.
- `scripts/` holds the shared inventory, fact sheet and claim checker, and the tables specific to this repository (`os_tables.py`, `gen-scope.py`). `tools/` holds the container definitions and wrappers shared with the sibling repositories; `tools/check-shared` keeps them identical.
- Public sources are pinned submodules ([docs/published-sources.md](docs/published-sources.md) says why each pin).

## 4. Tools

Everything runs in a container built from `tools/Containerfile` (MIPS binutils, dwarfdump, pyelftools, capstone, unicorn, spimdisasm). `tools/README.md` has the commands: `tools/re-run`, the Ghidra image and `--ghidra-export`, the inventory, the claim checker and the pre-push checks.

Tables (all generated; never edit them by hand):

- `binaries.tsv`, `functions.tsv`, `ioctls.tsv` from `scripts/inventory.py` (shared with the sibling repositories; configuration in `scripts/inventory-config.json`, which `scripts/gen-scope.py` partly writes from `scripts/scope.json`);
- `syscalls.tsv`, `structs.tsv`, `panics.tsv`, `selectors.tsv`, `formats.tsv`, `summary-os.tsv` from `scripts/os_tables.py`;
- `scope.tsv` and `summary.tsv`, which report the in-scope rule per group and the in-scope and out-of-scope counts.

## 5. Checks before every push

- `scripts/check-leaks.sh` (also run by the `scripts/pre-push` hook: install it with `ln -s ../../scripts/pre-push .git/hooks/pre-push`): no forbidden names, private paths or commit trailers.
- The copy check in `tools/README.md`: a fingerprint comparison of the C in this repository and an identifier comparison of the prose against a stored index of a reference tree. It never prints reference source. Read every lead; a lead is judged by opening the pair, not by the score.
- `scripts/check_claims.py`: compares the mechanical claims of routine entries (addresses, sizes, callees, frames) with the binaries. The hook refuses a push on a blocking mismatch.
- `tools/check-shared` and `scripts/gen-scope.py --check`.

## 6. When work is finished

Every in-scope entry in every inventory table is `understood` (a label, a role and effect, and a citation specific to the item: release, binary, address or offset, callee) or `no-software-dependency` (the section 2 label, with the routes tried). `located` and `named-only` are partial. The repository is not finished while any table has an in-scope row below that, and the long-term finish moves with the scope. Regenerate the inventory in the same commit as any document or header that explains an item.

An open question is not a stopping point: record it in `docs/open-questions.md` with the routes tried and the next route, then carry on.

## 7. Working here

Commit small and often, straight to `main`. Subjects are lowercase, component-prefixed imperatives (`kernel: document the klconfig board walk`); bodies say why. Take a cluster of related rows, take each to `understood`, regenerate, commit.
