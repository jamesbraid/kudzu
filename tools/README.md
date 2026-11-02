# Tools

Everything runs in a container built from `Containerfile`, so the host needs only Podman, git and python3.

    tools/setup-clone.sh --root /path/to/filled/root   # root/ link and the public-source submodules
    tools/re-run python3 scripts/inventory.py          # the first run builds the image
    tools/re-run python3 scripts/os_tables.py --release 6.5.30
    tools/re-run python3 scripts/gen-scope.py --check
    tools/re-run python3 scripts/check_claims.py --status all
    scripts/check-leaks.sh

`tools/re-run COMMAND` runs COMMAND in the image with this repository mounted at `/work`, `root/` included. Set `RE_ENGINE` to the container command to use instead of `podman` (a wrapper that selects a particular Podman service works). `tools/re-run --ghidra COMMAND` uses the optional Ghidra 12.1.3 image from `Containerfile.ghidra`, which has `analyzeHeadless` and the BSim support; `tools/re-run --ghidra-export OBJECT` decompiles an ELF object or kernel to JSON under `.cache/ghidra-export/`. Decompiler output is navigation, not evidence. The Ghidra scripts are in `ghidra-scripts/`. [README-analysis.md](README-analysis.md) describes the `.mdebug` importer and the syscall and syssgi extractors.

The container definitions, wrappers, Ghidra helpers and the shared scripts (inventory, fact sheet, claim checker, pre-push hook, leak check) are the `retools/` submodule (sgi-retools); the paths here and in `scripts/` link into it. `tools/check-shared` verifies the pin. `scripts/os_tables.py`, `scripts/gen-scope.py` and `scripts/scope.json` are specific to this repository and are not shared.

## Checking for copied reference source before a push

The leak check (`scripts/check-leaks.sh`, run by the pre-push hook) looks for forbidden names and trailers. A second check looks for text copied from a reference source tree. It is a fingerprint comparison against a stored index of the reference tree, which holds fingerprints and file paths and never source, and it prints no reference text. It is a lead generator: open each pair it reports and judge it.

The check is `tools/fingerprint` in the project that owns the stored reference index (the emulator tooling repository), not here. Run it from that checkout, against this repository, and let it write its reports under its own scratch run directory, never in this working tree:

    # copied spans and prose, over what git tracks here: Markdown and text, listings, tables, Python, C and headers
    TOOLING/tools/fingerprint --ours /path/to/this/repository --docs

    # lone distinctive names too; the names the shipped binaries in root/ carry, and the open-source names, are not reported
    TOOLING/tools/fingerprint --ours /path/to/this/repository --docs --identifiers \
        --symbols-baseline /path/to/this/repository

    # only the files a branch changed
    TOOLING/tools/fingerprint --ours /path/to/this/repository --docs --changed-only --base main

`tools/fingerprint --help` (there) lists the options. A prose lead is about a dozen words in a row shared with a comment or string of the reference; a name lead is ranked by how few reference files hold the name, rarest first. Read each one.

The pre-push hook (`scripts/pre-push`) runs the same tool on the files a push changes, once for spans and prose and once for names, when it can find it. Name the tool with `COPY_CHECK=/path/to/TOOLING/tools/fingerprint`, or put that path on the first line of `~/.config/re-copy-check`; add options such as `--reference DIR` or `--cpus N` with `COPY_CHECK_ARGS`. The tool also needs its prebuilt indexes and container image; when any is missing it reports so, the hook prints a notice and the push goes on. A lead refuses the push (exit 4 from the tool). Read the lead and rewrite the passage in your own words. A name you have read and judged benign goes in `scripts/copy-check-allow.txt`, one per line. `COPY_CHECK=off` skips the stage.

The tool does not catch text reworded or code renamed after copying, and it only compares with the reference tree it has indexed.
