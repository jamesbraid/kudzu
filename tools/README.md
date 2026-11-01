# Tools

Everything runs in a container built from `Containerfile`, so the host needs only Podman, git and python3.

    tools/setup-clone.sh --root /path/to/filled/root   # root/ link and the public-source submodules
    tools/re-run python3 scripts/inventory.py          # the first run builds the image
    tools/re-run python3 scripts/os_tables.py
    tools/re-run python3 scripts/gen-scope.py --check
    tools/re-run python3 scripts/check_claims.py --status all
    scripts/check-leaks.sh

`tools/re-run COMMAND` runs COMMAND in the image with this repository mounted at `/work`, `root/` included. Set `RE_ENGINE` to the container command to use instead of `podman` (a wrapper that selects a particular Podman service works). `tools/re-run --ghidra COMMAND` uses the optional Ghidra 12.1.3 image from `Containerfile.ghidra`, which has `analyzeHeadless` and the BSim support; `tools/re-run --ghidra-export OBJECT` decompiles an ELF object or kernel to JSON under `.cache/ghidra-export/`. Decompiler output is navigation, not evidence. The Ghidra scripts are in `ghidra-scripts/`.

`tools/check-shared` verifies that the container definitions, wrappers, Ghidra helpers and the shared scripts (inventory, fact sheet, claim checker, pre-push hook, leak check) are byte-identical to the copies in the sibling reverse-engineering repositories. Edit them in one repository, run `tools/check-shared --update`, copy the files and `shared.sha256` everywhere, and run it in each. `scripts/os_tables.py`, `scripts/gen-scope.py` and `scripts/scope.json` are specific to this repository and are not shared.

## Checking for copied reference source before a push

The leak check (`scripts/check-leaks.sh`, run by the pre-push hook) looks for forbidden names and trailers. A second check looks for text copied from a reference source tree. It is a fingerprint comparison against a stored index of the reference tree, which holds fingerprints and file paths and never source, and it prints no reference text. It is a lead generator: open each pair it reports and judge it.

The check is `tools/fingerprint` in the project that owns the stored reference index (the emulator tooling repository), not here. Run it from that checkout, against this repository, and let it write its reports under its own scratch run directory, never in this working tree:

    # copied spans, over the C and headers in this repository (include/, decomp/ and any other .c/.h)
    TOOLING/tools/fingerprint --ours /path/to/this/repository

    # a lone distinctive name lifted from the reference; open-source trees named with --baseline drop common names
    TOOLING/tools/fingerprint --ours /path/to/this/repository --identifiers \
        --baseline INDEX --baseline INDEX

    # only the files a branch changed
    TOOLING/tools/fingerprint --ours /path/to/this/repository --changed-only --base main

`tools/fingerprint --help` (there) lists the options. On the scaffold commit both runs reported nothing, but that is vacuous: the repository holds no C or header files yet.

Limit: the tool reads only `.c` and `.h` files. This repository's findings are mostly Markdown and TSV, which it does not read, so it does not cover prose. Extending it to read Markdown belongs in that tool; until then, review prose by hand against the evidence rules in [AGENTS.md](../AGENTS.md#2-evidence) and run `scripts/check-leaks.sh`.
