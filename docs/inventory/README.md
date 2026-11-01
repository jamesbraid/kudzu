# Inventory

Every file in this directory is generated. Do not edit by hand: change the documents, the headers, `scripts/scope.json` or the configuration, then regenerate. Run the two scripts from the repository root, with `root/` populated, inside the toolchain container ([tools/README.md](../../tools/README.md)):

    tools/re-run python3 scripts/gen-scope.py        # only after scripts/scope.json changes
    tools/re-run python3 scripts/inventory.py
    tools/re-run python3 scripts/os_tables.py        # needs the functions table from inventory.py

`scripts/inventory.py` is shared with the sibling repositories. It writes `binaries.tsv`, `functions.tsv`, `ioctls.tsv` and `summary.tsv`, plus `mmio.tsv`, `tokens.tsv`, `ucode.tsv` and `spec-registers.tsv`, which are graphics-hardware tables that have no rows in this repository. The columns, the five statuses (`undocumented`, `named-only`, `located`, `understood`, `no-software-dependency`) and the rule that an entry must carry a label, a role and effect and a citation specific to its subject before a row counts as `understood` are the shared definition: they are the same as in the sibling repositories, whose inventory README holds the full column text. The reproducibility inputs are `scripts/inventory-root.sha256` (hash of every file read from `root/`) and `scripts/inventory-siblings.json` (empty here: no sibling workspace is compared).

## Scope

`scripts/scope.json` states, for each binary group, whether its functions are in scope (`in`), in scope only when the function name starts with one of the listed prefixes (`narrow`) or out of scope for now (`out`). `scripts/gen-scope.py` turns it into the `binaries` and `scope` sections of `scripts/inventory-config.json`. Out-of-scope rows stay in the tables with `scope` = `out:N`; `summary.tsv` reports `out_of_scope` per table and per binary, and [scope.tsv](scope.tsv) reports the rule per group with its counts. An out-of-scope row is not excluded for good: it counts when the rule changes.

## Tables from scripts/os_tables.py

| Table | One row per | Notes |
|---|---|---|
| `syscalls.tsv` | `sysent` entry, per kernel image | handler symbol, argument-count byte, flag bytes; the entry layout is inferred (see open questions) |
| `structs.tsv` | named structure in a kernel image's DWARF | byte size and member count; source file names are not recorded |
| `panics.tsv` | calling function and callee (`panic`, `cmn_err`, `assfail`, `nopanicdebugger`) | call-site count; strings not recovered |
| `selectors.tsv` | `syssgi`, `sysmp` and similar selector | not generated yet; header only |
| `formats.tsv` | container or section kind of the kernel images | ELF class and type, and every section name and type |
| `scope.tsv` | binary group | the in-scope rule and its counts |
| `summary-os.tsv` | table above | in-scope and out-of-scope counts and documentation status |

For these tables `status` is `documented` when the name appears in backticks in a page under `docs/`, and `undocumented` otherwise. The shared five-level grading applies to `binaries.tsv`, `functions.tsv` and `ioctls.tsv`; moving these tables to it is future work.
