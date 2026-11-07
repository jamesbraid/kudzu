# Inventory

Every file in this directory is generated. Do not edit by hand: change the documents, the headers, `scripts/scope.json` or the configuration, then regenerate. Run the two scripts from the repository root, with `root/` populated, inside the toolchain container ([tools/README.md](../../tools/README.md)):

    tools/re-run python3 scripts/gen-scope.py        # only after scripts/scope.json changes
    python3 scripts/gen-hw-windows.py                # only after scripts/hw-windows.json changes
    tools/re-run python3 scripts/inventory.py
    tools/re-run python3 scripts/os_tables.py                    # needs the functions table from inventory.py; both releases
    tools/check-os-tables                                         # fails when the committed tables are not what these produce

`scripts/inventory.py` is shared with the sibling repositories. It writes `binaries.tsv`, `functions.tsv`, `ioctls.tsv`, `mmio.tsv` and `summary.tsv`, plus `tokens.tsv`, `ucode.tsv` and `spec-registers.tsv`, which are graphics-hardware tables that have no rows in this repository. `mmio.tsv` has one row per register offset the kernels reach in the windows listed under Register windows below. The columns, the five statuses (`undocumented`, `named-only`, `located`, `understood`, `no-software-dependency`) and the rule that an entry must carry a label, a role and effect and a citation specific to its subject before a row counts as `understood` are the shared definition: they are the same as in the sibling repositories, whose inventory README holds the full column text. The reproducibility inputs are `scripts/inventory-root.sha256` (hash of every file read from `root/`) and `scripts/inventory-siblings.json` (empty here: no sibling workspace is compared).

## Scope

`scripts/scope.json` states, for each binary group, whether its functions are in scope (`in`), in scope only when the function name starts with one of the listed prefixes (`narrow`) or out of scope for now (`out`). `scripts/gen-scope.py` turns it into the `binaries` and `scope` sections of `scripts/inventory-config.json`. Out-of-scope rows stay in the tables with `scope` = `out:N`; `summary.tsv` reports `out_of_scope` per table and per binary, and [scope.tsv](scope.tsv) reports the rule per group with its counts. An out-of-scope row is not excluded for good: it counts when the rule changes.

## Register windows

`scripts/hw-windows.json` says which hardware register windows the scan looks for in the kernels, and `scripts/gen-hw-windows.py` writes them into the `windows`, `bases` and `gp_literals` sections of `scripts/inventory-config.json` (`--check` fails when the config is stale). Every base carries a `why` that names its source. The addresses, pointer arguments and pointer-returning routines that the system-chip workspace (chips) established are read from its `tools/configs/<chip>.json` at the commit pinned in `hw-windows.json`, with `git show`, from a clone at `.cache/siblings/chips`; they are not copied, so the two workspaces cannot drift. The rest comes from the reviewed Bedrock address-map datasheet (Hub register space and Hub special space) and the IP30 machine and HEART datasheets, and from this repository's own documents; the window's `docs` list is where a bare offset counts as a mention.

| Platform | Windows | How code reaches them |
|---|---|---|
| IP27 | `HUB` (regions `HUB-PI`, `HUB-MD`, `HUB-II`, `HUB-NI`), `XBOW` | the uncached I/O alias of physical `0x01000000` for the calling processor's own Hub; the node alias `0x9200000001800000` plus the node number shifted left 32 (unresolved: see below); the `0x02000000` bus-error alias |
| IP30 | `HEART` (regions `HEART-PIU`, `HEART-W8`), `XIO-IP30` (regions `XIO-W0` to `XIO-W15`), `XBOW`, `BRIDGE` | literal addresses in the 64-bit uncached and the 32-bit compatibility windows; widget n at `0x10000000 + n * 0x1000000` |
| IP35 | `BEDROCK` (regions `BEDROCK-PI0`, `-PI1`, `-IO`, `-LB`, `-NI`, `-XB`, `-MD`), `BEDROCK-SPECIAL` (regions `BEDROCK-L1UART`, `BEDROCK-LED`), `BEDROCK-BOOT`, `XBOW` | the local alias `0x9200000001000000`; the node alias `0x9200000001800000` plus the node number shifted left 33 (unresolved); the Hub special space windows at `0x9000000010000000` and `0x9000000030000000` (plus the node number shifted left 33); the boot window at `0x9000000018000000` |
| all three | `BRIDGE` (Bridge, XBridge, PIC), `IOC3`, `IOC4` (IP35 only), `XTALK-PIO`, `PCI-PIO` | pointers: the return of `pcibr_bridge_ptr_get`, `ioc3_get_mem`, `ioc3_mem_ptr`, `ioc4_get_mem`, `ioc4_mem_ptr`, and arguments the callers pass on |

The linked kernels have no GOT: the compiler keeps 64-bit addresses in the small-data sections and loads them through gp. The `gp_literals` rule makes the scan read `.sdata`, `.lit8` and `.srdata` of `unix.IPnn` for those loads; without it a linked kernel resolves only the addresses it builds with `lui`.

Reading the tables:

- A `window` column value is a window or one of its regions. Offsets are relative to the start of the window's alias, so the local and node aliases of a Hub fall on the same offsets.
- `XTALK-PIO` and `PCI-PIO` are pointers returned by the crosstalk and PCI mapping routines at an offset their caller chose. The scan does not follow that argument, so an access through such a pointer is an unresolved site of the window, never a register row. The same holds for the node aliases of `HUB` and `BEDROCK` and the L1 UART window, whose node number is computed: the offset is known and the node is not, and the scan reports the site as unresolved rather than filing it at node 0.
- `summary.tsv` and the last line of the run report the unresolved sites by window. A routine with only unresolved sites still counts as reaching the window, which is what stops `scripts/scope.json` from excluding it.
- Known artefact: the Bridge-family accessors `pcireg_*` take either the chip window or a driver record, and the routine decides which at run time. The scan gives the argument the window the callers pass, so the record's fields appear as `BRIDGE` rows `0x8` (doubleword) and `0x72` (halfword). They are not registers (`0x72` is not a register slot, and the registers are read as words at slot + 4).
- Register names are blank: no register table is wired into the config yet (`spec-registers.tsv` is empty). `status` comes from the documents that mention the offset.
- `scripts/scope.json` lists, under `keep`, the routines of out-of-scope objects that the scan found reaching a window (mostly readers of the free-running real-time counter). They stay in scope because the run refuses to exclude a routine that reaches a window.

## Function addresses

A function's address comes from one source per name, and `address_source` in `binaries.tsv` lists the sources a binary used. The ELF symbol tables come first (`symtab`, then `dynsym`). `mdebug` supplies names they lack, and `dwarf` supplies names that neither has.

DWARF can be out of date. If the library was relinked after its debug information was written, the code moved and DWARF's addresses are wrong (a `libGL.so` has `glXQueryVersion` at 0xd833278 in `.dynsym` and at 0xd80f0a0 in DWARF). The script treats a binary's DWARF as stale only when some name has DWARF addresses that share none with the addresses the symbol tables give that same name. A static function that is defined in several compilation units, where the symbol table keeps only one of them, does not show this: its DWARF addresses overlap the table's, and the file's other DWARF addresses stay in use.

When DWARF is stale, its addresses are dropped and the label says so with a suffix such as `dynsym+dwarf (dwarf addresses stale, dropped except sections ...)`, whose last words, in full in `binaries.tsv`, name the exception below. The exception is a function that fills a section by itself, such as `_init` in `.init`. Relinking did not move those sections, so DWARF's address, which is the section start, is kept (hence `+dwarf` in the label). Without that exception the label reads `dynsym (dwarf addresses stale, dropped)`. A name left with no address after this is covered below.

A name defined at several addresses in the chosen source takes the lowest and is rejected (see the sibling repositories' inventory README for the `rejected` column).

## Rows with no address

Some static functions have code in the binary, and a name in its debug information, but no source gives an address for them: the symbol tables omit them and DWARF is stale. The `functions.tsv` row for such a function has address 0x0 and `rejected` = `address-unknown` (none in this repository's current binaries). Nothing can credit such a row by address, and a reading round cannot render it, so it is not open work. `summary.tsv` and `coverage.md` leave these rows out of every status count and the denominator, as they do out-of-scope rows, and report them in the `address_unknown` column of `summary.tsv` and a line under the coverage table. Reading round target selection skips them. They become ordinary rows again when a source supplies an address.

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

## Table from scripts/userland_tables.py

`userland.tsv` has one row per base user-space binary listed in `scripts/userland-roles.json` (commands, daemons and their RPC and cluster companions of the 6.5.30 install). The ELF facts (ABI from the header flags, interpreter, section headers, undefined dynamic symbols) come from the file under `root/`; the package, type, interfaces, role, grade and citation come from the roles file. Run `tools/re-run python3 scripts/userland_tables.py`. The rows are described in [userland.md](../userland.md).

## Table from scripts/errno_table.py

`errnos.tsv` has one row per nonzero constant, callee-propagated value or unresolved value that a 6.5.30 system call handler or `syssgi` case returns, and per such value of the first-level callee whose return it passes on. It needs `syscalls.tsv` and `selectors.tsv` from `scripts/os_tables.py`. Its columns, the grades and the known limits are in [errnos.md](../errnos.md). Run `tools/re-run python3 scripts/errno_table.py` (about four minutes for the three kernels).

## Tables from scripts/tunable_sites.py

`tunable-summary.tsv` (one row per tunable and kernel) and `tunable-sites.tsv` (one row per read or store of a tunable's data symbol) map each `mtune` name in `sysgen-tunables.tsv` to its data symbol in the linked 6.5.30 kernels `unix.IP27`, `unix.IP30` and `unix.IP35`. Run `tools/re-run python3 scripts/tunable_sites.py` (about ten minutes for the three kernels); `--roles-only` re-applies `scripts/tunable-roles.json` to the last scan without rescanning. A site is an access whose resolved absolute address lies in the symbol, found by the symbolic pass of `scripts/facts.py`. Roles are in [sysgen.md](../sysgen.md#tunable-read-sites); a row is `understood` only for the kernels its role lists.

The numbering of `syscalls.tsv` and `selectors.tsv` is compared with public lists in [syscall-selector-crosscheck.md](../syscall-selector-crosscheck.md).
