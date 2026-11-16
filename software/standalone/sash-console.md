# sash64 install dialog with console=g and no graphics board (Octane, 6.5.30)

Question. On an Octane2 with no graphics board and the factory `console=g`, "Install System Software" loads `sash64`, which then takes a
read TLB miss on address 0x38 before "Copying installation program to disk". Which instruction, which null pointer, which firmware call
produced it, and would a real machine do the same.

Image: `6.5.30/sash64` from the 6.5.30 bootable media (ELF64 MIPS relocatable, 266,768 bytes, sha256 `7b86941c14cb8679...ec4a17142`, banner
"SGI Version 6.5 ARCS Jul 20, 2006"). Offsets below are `.text` offsets in that file. The firmware loads `.text` at `0x2ff8c000` when
the machine has 256 MiB of memory (it moves with memory size; the 512 MiB run is at `0x3ff8c000`). The base was fitted, not read from the
loader: 193 of the 209 distinct basic-block starts in the run's execution trace (`exec-after-vh-read.collapsed.txt`) land on function
starts, branch targets or branch fall-throughs at that base and no other base scores above 75. The register dump independently agrees
(see below). Ratings: **established** (whole routine read), **observed** (in a model run), **software expectation** (public 6.5.7 source),
**inferred**, **unknown**. The 6.5.22 `sash64` was not available in this workspace; the panic was reported on both releases and only the
6.5.30 file is read here.

Run evidence: octane2 model run set of 2026-11-16 (console logs, gdb registers at the fault, exec trace); stock IP30 PROM 6.5.30.

## The fault

| Item | Value | Rating |
|---|---|---|
| Faulting instruction | `textSize+0x94` = `.text` `0x10874`, `ld t0,8(t0)` (run address `0x2ff9c874`) | established; run PC observed |
| Register at fault | `t0 = 0x30`, so address `0x38`; `t0` came from `fonts + 24*font` with `font = 2` | observed (`t3 = 0x30`) |
| Table base | `*(htp + 0x10)` is zero; `htp = *(gfxgui + 0x10) = 0x20f29d28` (a PROM-owned address, non-null) | observed (`t9` at the fault); established (`textSize` at `0x107e0`-`0x10874`) |
| Return address | `0x2ff9a49c` = `createDialog+0x90` (`.text` `0xe49c`, after its call to `textSize(msg, &w, &h, 2)`) | established |
| Caller of `createDialog` | the install-start code at `.text` `0xd1a8` (static, inside the range after `do_custom_boot`), call at `0xd1f8`-`0xd200` | established; trace passes `0x2ff99200` (`.text` `0xd200`) just before |

`textSize` is the text-measuring routine of sash's built-in dialog code. Given a string and a font number it looks the font up in the
graphics state's font table (24-byte entries, field at +8 read) and sums glyph widths. The table pointer is a field of the console
graphics state object that the firmware owns and `sash64` stores in its global `gfxgui` at `+0x10`. Here that object exists but its
font table pointer is null. The faulting load is therefore a font-table dereference, not a console or ARCS component structure.

## Where the null comes from

1. `main` calls `initGfxGui(0)` (`.text` `0xae04`; call is in `main`, "we use gui for install" per the 6.5.7 source
   `stand/arcs/sash/main.c`). With argument 0 it fetches the graphics state from the firmware, as established by `0xaecc`-`0xaedc`:
   it loads the ARCS system parameter block at physical `0x1000` (`0xa800...00001000`), takes the private vector pointer at SPB `+0x50`,
   and calls entry 8 (`+0x40`). **Entry 8 is `gethtp`**, the firmware's "get the textport state" call (private vector order read
   from 6.5.7 `irix/kern/sys/arcs/pvector.h`: ioctl, nvram table, load, invoke, exec, fs register, fs unregister, signal, gethtp).
   The result goes to `gfxgui+0x10` unchecked-for-use; only a null result returns early. Established (code), software expectation (name).
2. In the firmware's library the object returned by `gethtp` exists regardless of whether a board was found, but its font table
   is filled in only by the textport start-up that runs after a graphics board was found (public source
   `stand/arcs/lib/libsk/graphics/gfx.c` and `htport.c`: board probe first; on failure no textport start-up, no font setup). Software
   expectation. Observed here: object non-null (`0x20f29d28`), font table null; PROM log "Cannot open video() for output".
3. `sash64` reaches the dialog because it asks the wrong question. `doGui` (`.text` `0x7108`) is true when the environment variable
   `nogui` is not `y`, the firmware version call succeeds and `isgraphic(1)` is true. `isgraphic` (`0x375c`) has two paths: when
   the library flag `stdio_init` is non-zero it asks the firmware with a control request (private vector entry 0, request `0x7409`);
   when zero it only reads the `console` environment variable and answers yes if the first character is `g` or `G`. Nothing in
   `sash64` writes `stdio_init` (only `isgraphic` reads it), so the second path is the one taken. Established for the code;
   that the flag is zero at run time is inferred (not read from memory).
4. With `console=g` set, `doGui` is true although the firmware has opened `serial(0)` as ConsoleOut after "CONSOLE OPEN FAILED."
   (the environment variable is left as `g`). The install start code then sets an environment variable, enters GUI mode
   (`setGuiMode`) and calls `createDialog`, which measures its message with font 2 and faults. Established (code); the run's
   `printenv` shows `console=g`, `NoAutoLoad=CONSOLE OPEN FAILED.`, `ConsoleOut=serial(0)` (observed).
5. `console=d` makes `isgraphic(1)` false, so the text path (`p_printf`) is used. With a V12 board the textport start-up runs, fonts exist,
   and the dialog draws. Observed (both runs worked).

The `ESC[35h` the serial port receives just before the fault is the string at `.rodata+0x1248` (file `0x1c720`). It is in the same
`.rodata` run as `nogui` and the other GUI strings; which routine prints it was not traced. Unknown.

## Answers

1. The faulting instruction is `ld t0,8(t0)` in `textSize` at `.text` `0x10874`. The null is the font table inside the firmware's
   textport/graphics state object, returned non-null by private-vector entry 8 (`gethtp`) and left unfilled because no graphics board
   was opened. It is not an ARCS ConsoleOut component, `GetComponent` result or a null `htp` pointer. Established (code) with observed
   registers. The cause is `sash64` trusting `console=g` (via `isgraphic`) over what the firmware actually opened.
2. Real hardware: by this code, yes. Nothing in the chain is hardware specific: the environment variable, the firmware's private
   vector and the font setup that only graphics start-up performs. The model does not synthesise any of it; the PROM is the stock
   image. A real Octane with no graphics board and `console=g` (the NVRAM default) should fault identically. Inferred: the PROM's own
   allocation of the graphics state was not read in the PROM binary, so "non-null on real hardware" rests on the model run
   plus the library structure. A 2007 comp.sys.sgi.hardware report on a real O2 (IP32 PROM 4.4) with `console=g`,
   `NoAutoLoad=CONSOLE OPEN FAILED.` and `ConsoleOut=serial(0)` shows the same family of failure: a read TLB miss when installing from the CD,
   and the diagnostics CD program fails the same way (message id `1171529859.028540.47790@p10g2000cwp.googlegroups.com`). Its PC and
   firmware differ and it was never resolved, so it supports the symptom only. Observed (report), inferred (same cause).
3. Model changes that avoid the fault: none short of presenting a graphics board. Making `gethtp` return null would not help: `initGfxGui`
   then leaves `gfxgui+0x10` null and `textSize` faults on address `0x10` instead (inferred from `0x107f4`-`0x10824`). ARCS
   `GetComponent` and `ConsoleOut` are not read on this path. A guest-side avoidance that keeps `console=g` is the PROM environment
   variable `nogui` set to a value starting with `y`: `doGui` then returns false (`0x7150`-`0x7158`), sending the dialog to the text path.
   Software expectation from the code; not run. `setenv console d` is observed to work.

## Open

- The 6.5.22 `sash64` was not read; its panic address in the evidence is the same relocated PC, which is consistent but not shown.
- Which routine writes `ESC[35h`.
- Whether `nogui=y` actually completes the install: not run.
