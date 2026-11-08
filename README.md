# kudzu

Reverse-engineering and specification repository for SGI IRIX 6.5.30 and 6.5.22.

It starts with the hardware-facing and boot-critical kernel: the PROM interface and KLCONFIG parsing, hardware-graph construction, interrupt routing, NUMA and memory initialisation, the topology checks that panic or skip nodes, the device attach framework, and a debugging map of structure layouts, entry points and panic and assert sites. The long-term scope is all of IRIX and its add-ons: the full kernel, CXFS, ONC RPC and NFS, the base applications and libraries.

There is no reimplementation here. The findings are specifications that emulator datasheets, drivers and debuggers elsewhere can use.

The layout follows the sibling repositories for graphics and chip reverse engineering. Proprietary IRIX binaries are not tracked: copy them from your own media into the ignored `root/` directory as [IRIX_FILES.md](IRIX_FILES.md) describes. [AGENTS.md](AGENTS.md) has the scope rule, the evidence rules and the finish line; [docs/coverage.md](docs/coverage.md) is the map of what is covered; [docs/open-questions.md](docs/open-questions.md) lists what the binaries have not yet answered; [docs/published-sources.md](docs/published-sources.md) says which public trees are pinned and why.

## Running the tools

After cloning, run `git submodule update --init retools`: the container definitions and shared scripts under `tools/` and `scripts/` are links into it. The other submodules are public reference trees and are large; initialise them only when you want them ([tools/README.md](tools/README.md) describes `tools/setup-clone.sh`, which links `root/` and fetches the public-source submodules).

The host needs Podman, git and python3. Everything else runs in a container; [tools/README.md](tools/README.md) has the commands:

    tools/re-run python3 scripts/inventory.py     # binaries, functions, ioctls, scope
    tools/re-run python3 scripts/os_tables.py     # system calls, structures, panic sites, formats
    tools/check-os-tables                         # fail if the committed tables are stale

The generated tables are in [docs/inventory/](docs/inventory/README.md). `git status --short docs` shows nothing after a run when your `root/` matches `scripts/inventory-root.sha256`.

Licence: BSD-3-Clause, see [LICENSE](LICENSE). The scripts descend from gr4's and keep its notice in [scripts/LICENSE-gr4](scripts/LICENSE-gr4).
