# Inventory

Every file in this directory is generated. Do not edit by hand: change the documents, the headers, `scripts/scope.json` or the configuration, then regenerate. Run the two scripts from the repository root, with `root/` populated, inside the toolchain container ([tools/README.md](../../tools/README.md)):

    tools/re-run python3 scripts/gen-scope.py        # only after scripts/scope.json changes
    tools/re-run python3 scripts/inventory.py
    tools/re-run python3 scripts/os_tables.py --release 6.5.30   # needs the functions table from inventory.py

`scripts/inventory.py` is shared with the sibling repositories. It writes `binaries.tsv`, `functions.tsv`, `ioctls.tsv` and `summary.tsv`, plus `mmio.tsv`, `tokens.tsv`, `ucode.tsv` and `spec-registers.tsv`, which are graphics-hardware tables that have no rows in this repository. The columns, the five statuses (`undocumented`, `named-only`, `located`, `understood`, `no-software-dependency`) and the rule that an entry must carry a label, a role and effect and a citation specific to its subject before a row counts as `understood` are the shared definition: they are the same as in the sibling repositories, whose inventory README holds the full column text. The reproducibility inputs are `scripts/inventory-root.sha256` (hash of every file read from `root/`) and `scripts/inventory-siblings.json` (empty here: no sibling workspace is compared).

## Scope

`scripts/scope.json` states, for each binary group, whether its functions are in scope (`in`), in scope only when the function name starts with one of the listed prefixes (`narrow`) or out of scope for now (`out`). `scripts/gen-scope.py` turns it into the `binaries` and `scope` sections of `scripts/inventory-config.json`. Out-of-scope rows stay in the tables with `scope` = `out:N`; `summary.tsv` reports `out_of_scope` per table and per binary, and [scope.tsv](scope.tsv) reports the rule per group with its counts. An out-of-scope row is not excluded for good: it counts when the rule changes.

## Tables from scripts/os_tables.py

| Table | One row per | Notes |
|---|---|---|
| `syscalls.tsv` | entry of a system call table (`sysent` or `irix5_64_sysent`, the `table` column), per kernel image | call number, handler symbol, argument-count byte, flag bytes; from `scripts/syscall_table.py`, layout confirmed against the dispatcher |
| `structs.tsv` | named structure in a kernel image's DWARF | byte size and member count; source file names are not recorded |
| `panics.tsv` | calling function and callee (`panic`, `cmn_err`, `assfail`, `nopanicdebugger`) | call-site count; strings not recovered |
| `selectors.tsv` | `syssgi` selector that reaches code of its own, per kernel image | jump table or compare tree (`group`), code address, `first_call`, `shared`; from `scripts/syssgi_selectors.py`. `first_call` is a heuristic: the first routine the code calls before it branches, blank when it branches first, and sometimes a locking or helper routine rather than the handler. `sysmp` and other selector functions are not generated yet |
| `formats.tsv` | container or section kind of the kernel images | ELF class and type, and every section name and type |
| `scope.tsv` | binary group | the in-scope rule and its counts |
| `summary-os.tsv` | table above | in-scope and out-of-scope counts and documentation status |

For these tables `status` is `documented` when the name appears in backticks in a page under `docs/`, and `undocumented` otherwise. The shared five-level grading applies to `binaries.tsv`, `functions.tsv` and `ioctls.tsv`; moving these tables to it is future work.

## Tables from scripts/sysgen_tables.py

`sysgen-drivers.tsv` (one row per driver master file) and `sysgen-tunables.tsv` (one row per `mtune` line) come from the configuration files of one release, read per machine. Run `tools/re-run python3 scripts/sysgen_tables.py --release <release>`. The method, the release in use and the open disagreements are in [sysgen.md](../sysgen.md). Driver roles are hand-written in `scripts/sysgen-roles.json`; a row is `understood` only with a grade, a role and a citation.

## Table from scripts/errno_table.py

`errnos.tsv` has one row per nonzero constant, callee-propagated value or unresolved value that a 6.5.30 system call handler or `syssgi` case returns, and per such value of the first-level callee whose return it passes on. It needs `syscalls.tsv` and `selectors.tsv` from `scripts/os_tables.py`. Its columns, the grades and the known limits are in [errnos.md](../errnos.md). Run `tools/re-run python3 scripts/errno_table.py` (about four minutes for the three kernels).

## Tables from scripts/tunable_sites.py

`tunable-summary.tsv` (one row per tunable and kernel) and `tunable-sites.tsv` (one row per read or store of a tunable's data symbol) map each `mtune` name in `sysgen-tunables.tsv` to its data symbol in the linked 6.5.30 kernels `unix.IP27`, `unix.IP30` and `unix.IP35`. Run `tools/re-run python3 scripts/tunable_sites.py` (about ten minutes for the three kernels); `--roles-only` re-applies `scripts/tunable-roles.json` to the last scan without rescanning. A site is an access whose resolved absolute address lies in the symbol, found by the symbolic pass of `scripts/facts.py`. Roles are in [sysgen.md](../sysgen.md#tunable-read-sites); a row is `understood` only for the kernels its role lists.

The numbering of `syscalls.tsv` and `selectors.tsv` is compared with public lists in [syscall-selector-crosscheck.md](../syscall-selector-crosscheck.md).
