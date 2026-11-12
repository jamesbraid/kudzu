# Tools

Everything runs in a container built from `retools/tools/Containerfile`, so the host needs only Podman, git and python3.

    tools/setup-clone.sh --root /path/to/filled/root   # root/ link and the public-source submodules
    retools/tools/re-run python3 retools/tools/inventory.py          # the first run builds the image
    retools/tools/re-run python3 retools/tools/kudzu/os_tables.py          # both releases under root/; run it after inventory.py
    tools/check-os-tables                              # the committed tables are what the two scripts generate
    retools/tools/re-run python3 retools/tools/kudzu/gen-scope.py --check

`retools/tools/re-run COMMAND` runs COMMAND in the image with this repository mounted at `/work`, `root/` included. Set `RE_ENGINE` to the container command to use instead of `podman` (a wrapper that selects a particular Podman service works). `retools/tools/re-run --ghidra COMMAND` uses the optional Ghidra 12.1.3 image from `retools/tools/Containerfile.ghidra`, which has `analyzeHeadless` and the BSim support; `retools/tools/re-run --ghidra-export OBJECT` decompiles an ELF object or kernel to JSON under `.cache/ghidra-export/`. Decompiler output is navigation, not evidence. The Ghidra scripts are in `retools/ghidra/scripts/`. [README-analysis.md](../retools/tools/README-analysis.md) describes the `.mdebug` importer and the syscall and syssgi extractors.

The container definitions, wrappers, Ghidra helpers and the shared scripts (inventory, fact sheet, syscall extractors) are the `retools/` submodule (sgi-retools), run by their `retools/tools/` path from the repository root. `retools/tools/kudzu/os_tables.py`, `retools/tools/kudzu/gen-scope.py` and `scripts/scope.json` are specific to this repository and are not shared. This directory holds only `check-os-tables` and `setup-clone.sh`.

## Checking for copied reference source before a push

A check looks for text copied from a reference source tree. It is a fingerprint comparison against a stored index of the reference tree, which holds fingerprints and file paths and never source, and it prints no reference text. It is a lead generator: open each pair it reports and judge it.

The check is `tools/fingerprint` in the project that owns the stored reference index (a separate tooling repository), not here. Run it from that checkout, against this repository, and let it write its reports under its own scratch run directory, never in this working tree:

    # copied spans and prose, over what git tracks here: Markdown and text, listings, tables, Python, C and headers
    TOOLING/tools/fingerprint --ours /path/to/this/repository --docs

    # lone distinctive names too; the names the shipped binaries in root/ carry, and the open-source names, are not reported
    TOOLING/tools/fingerprint --ours /path/to/this/repository --docs --identifiers \
        --symbols-baseline /path/to/this/repository

    # only the files a branch changed
    TOOLING/tools/fingerprint --ours /path/to/this/repository --docs --changed-only --base main

`tools/fingerprint --help` (there) lists the options. A prose lead is about a dozen words in a row shared with a comment or string of the reference; a name lead is ranked by how few reference files hold the name, rarest first. Read each one.

Add a name you have read and judged benign to `scripts/copy-check-allow.txt`, one per line.

The tool does not catch text reworded or code renamed after copying, and it only compares with the reference tree it has indexed.
