# Kernel probe points: a function reference table

A mechanical reference table of candidate kernel functions from the linked
IRIX 6.5.30 kernel, read as a reference list. This page is
descriptive only: the symbol, address, size, stack-frame use and first four
instruction words of each routine, plus the kernel text mapping and the
instruction-cache flush routines. It makes no claim about which routine is
safe to alter, how a module would reach kernel text, or how to stop other
processors; those are a consumer's own question against a running kernel, not
a fact this binary settles, and are left as an open question below.

Evidence is `6.5.30/unix.IP35` (Origin 300/350, Onyx 300/350 class), read with
`retools/scripts/hostfacts.py` and a capstone-based disassembler script
written for this page. Grades follow [AGENTS.md](../../AGENTS.md): **established**
is read in the binary; **inferred** says what it rests on; **unknown** says
what would settle it. Register names are the kernel's own n64 ABI names
(`$8`-`$15` are `a4`-`a7`, `t0`-`t3`; `$24`-`$25` are `t8`-`t9`); the kernel
and every routine below are n64, ISA mips4, confirmed by the ELF header
(`e_flags` architecture bits, `EI_CLASS` 64-bit). "Absolute-address load" below
means a load whose base register is `$zero`, so the effective address is the
small signed immediate itself (a fixed high-kernel address, not a register- or
PC-relative one); it is distinct from a branch, jump or delay slot.

Related pages: [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md)
(interrupt-context writer `log_tstamp_event`, the runtime-code-patching
section, `cache_operation` and its callers), [module-interfaces.md](module-interfaces.md)
(what a loadable module can call, the relocation kinds the loader accepts,
interrupt-context and locking rules), [syscalls.md](syscalls.md) (the syscall
table entries for `getpid`, `gtime`, `gettimeofday`, `getuid`, `umask`).

## Symbol-table caveat

Not every routine named in prose elsewhere in this repository has a global
symbol-table entry. `cpuintr`, the interrupt handler that `sendintr` and
`doactions` are read against in [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md#runtime-code-patching),
is one: it is known only from DWARF debug information (one of the 5029
functions the symbol table carries no global name for, per that page), not
from `.symtab`. A lookup of `cpuintr` by name in `unix.IP35`'s `.symtab`
fails. Established, checked directly against this binary's symbol table.
`install_cpuintr`, `counter_intr` and `clock`, used below, are ordinary
global `.symtab` entries.

## Function entries

One entry per routine, all in `6.5.30/unix.IP35`. "Frame" is the stack
adjustment, if any, in a `daddiu $sp, $sp, -N` or `addiu $sp, $sp, -N` among
the routine's first few instructions; a routine with none is a leaf that
touches no stack slot there. "First four words" are the raw 32-bit
big-endian instruction words at the routine's entry address. Each entry was
disassembled from its full function body at the cited address; all are
established.

- `getpid` (`6.5.30/unix.IP35`, 0xc0000000002b54dc, 104 bytes, 16-byte frame).
  First four words: `dc06c018 67bdfff0 ffb00000 00a08025`. Word 1
  (`ld $a2, -0x3fe8($zero)`) is an absolute-address load (the current
  thread's record pointer, chained at `+0x308` for the `vproc` pointer, per
  the existing [syscalls.md](syscalls.md) entry); none of the four is a
  branch, jump or delay slot. Established.
- `gtime` (`6.5.30/unix.IP35`, 0xc0000000002b4ff4, 52 bytes, 32-byte frame).
  First four words: `67bdffe0 ffbe0018 00a0f025 ffbf0010`. None of the four
  is a branch, jump, delay slot or absolute-address load. Established.
- `gettimeofday` (`6.5.30/unix.IP35`, 0xc0000000002632bc, 104 bytes,
  32-byte frame). First four words: `67bdffe0 ffa00000 ffbf0010 ffbe0018`.
  None of the four is a branch, jump, delay slot or absolute-address load.
  Established.
- `getuid` (`6.5.30/unix.IP35`, 0xc0000000002b51b0, 56 bytes, 16-byte frame).
  First four words: `67bdfff0 ffbe0008 ffbf0000 0c0b47f0`. Word 4
  (`jal get_current_cred`) is a jump; its delay slot (`move $fp, $a1`) is the
  instruction immediately after these four, so none of the four words itself
  sits in a delay slot. Established.
- `umask` (`6.5.30/unix.IP35`, 0xc0000000002b67c4, 116 bytes, 32-byte frame).
  First four words: `dc01c018 67bdffe0 842a034a a7aa0000`. Word 1
  (`ld $at, -0x3fe8($zero)`) is an absolute-address load; none of the four is
  a branch, jump or delay slot. Established.
- `microtime` (`6.5.30/unix.IP35`, 0xc0000000000695e0, 68 bytes, 32-byte
  frame). First four words: `67bdffe0 ffbe0018 0080f025 ffbf0010`. None of
  the four is a branch, jump, delay slot or absolute-address load.
  Established.
- `counter_intr` (`6.5.30/unix.IP35`, 0xc000000000067478, 824 bytes, 48-byte
  frame). First four words: `dc01c280 67bdffd0 30210020 ffb00010`. Word 1
  (`ld $at, -0x3d80($zero)`) is an absolute-address load; none of the four is
  a branch, jump or delay slot. Established.
- `clock` (`6.5.30/unix.IP35`, 0xc000000000260abc, 2528 bytes, 144-byte
  frame). First four words: `dc8100f8 67bdff70 ffa10048 ffb00068`. Word 1
  (`ld $at, 0xf8($a0)`) loads through argument register `a0`, not `$zero`,
  so it is an ordinary register-relative load, not an absolute-address one;
  none of the four is a branch, jump or delay slot. Established.
- `mutex_spinlock` (`6.5.30/unix.IP35`, 0xc000000000331f6c, 76 bytes, no
  frame: a leaf with no `sp` adjustment). First four words:
  `40026000 344c0001 398c0001 344e9700`. Word 1 (`mfc0 $v0, $12`) is a
  coprocessor-0 register move, not a memory load, branch or jump; none of
  the four is a branch, jump, delay slot or absolute-address load.
  Established.
- `mutex_spinunlock` (`6.5.30/unix.IP35`, 0xc000000000331fb8, 32 bytes, no
  frame: a leaf with no `sp` adjustment). First four words:
  `8c8c0000 358c0001 398c0001 0000000f`. Word 1 (`lw $t4, ($a0)`) loads
  through argument register `a0`, not `$zero`; none of the four is a branch,
  jump, delay slot or absolute-address load. Established.

Quick-view table of the same ten entries (not an independent claim; see the
prose above for the citations and grades):

| Symbol | Address | Size | Frame |
| --- | --- | --- | --- |
| `getpid` | 0xc0000000002b54dc | 104 bytes | 16 bytes |
| `gtime` | 0xc0000000002b4ff4 | 52 bytes | 32 bytes |
| `gettimeofday` | 0xc0000000002632bc | 104 bytes | 32 bytes |
| `getuid` | 0xc0000000002b51b0 | 56 bytes | 16 bytes |
| `umask` | 0xc0000000002b67c4 | 116 bytes | 32 bytes |
| `microtime` | 0xc0000000000695e0 | 68 bytes | 32 bytes |
| `counter_intr` | 0xc000000000067478 | 824 bytes | 48 bytes |
| `clock` | 0xc000000000260abc | 2528 bytes | 144 bytes |
| `mutex_spinlock` | 0xc000000000331f6c | 76 bytes | none (leaf) |
| `mutex_spinunlock` | 0xc000000000331fb8 | 32 bytes | none (leaf) |

## Kernel text mapping

Kernel code and read-only data are mapped by one wired TLB entry, read in
`mapped_kernel_setup_tlb` (`6.5.30/unix.IP35`, 0xc000000000065538, 168 bytes;
already read in full in
[kernel-tracing-and-modules.md](kernel-tracing-and-modules.md#runtime-code-patching)).
That entry's EntryHi is in the xkseg virtual range at 0xc000000000000000, with
a 16 MB page covering 0xc000000000000000 to 0xc000000000ffffff. The ELF
section headers of `6.5.30/unix.IP35` place `.text` at 0xc000000000040180,
size 0x457d08 (established, read directly from the section header table),
inside that first 16 MB page alongside the first `.rodata`; the load segment
that holds them runs 0xc000000000040000 to 0xc0000000004d9ebe (`p_vaddr`
0xc000000000040000, `p_filesz`/`p_memsz` 0x4d9ebf). The second load segment,
holding `.data`, `.sdata`, `.srdata`, `.lit8`, `.sbss` and `.bss`, starts at
0xc000000001519ec0, in the TLB entry's second (writable) page. Established,
from the ELF program and section headers read directly.

Whether that mapped text is writable, or needs a protection or TLB change
first, is already answered in
[kernel-tracing-and-modules.md](kernel-tracing-and-modules.md#runtime-code-patching):
the first page's EntryLo clears the dirty/write-enable bit, so a store to the
0xc000... text address is inferred to take a TLB-modified exception, and the
kernel's own code-patching path (`_hook_exceptions`) writes through the
cached XKPHYS alias of the physical page instead, not through that virtual
address. This page repeats the citation rather than re-deriving it.

## Cache-flush routines

Read directly in `6.5.30/unix.IP35` (also covered, for the call graph, in
[kernel-tracing-and-modules.md](kernel-tracing-and-modules.md#runtime-code-patching)):

- `cache_operation` (`6.5.30/unix.IP35`, 0xc0000000002b08f0, 956 bytes)
  dispatches on a flag word. Three small wrappers each add one flag bit and
  tail-call it: `icache_inval` (0xc0000000002b0ea0, 32 bytes; takes an
  address and length, ORs `0x8001` into the flag word), `inst_cache_operation`
  (0xc0000000002b0e80, 32 bytes; ORs `0x1`) and `data_cache_operation`
  (0xc0000000002b0e60, 32 bytes; ORs `0x2`). Established.
- `flush_cache` (`6.5.30/unix.IP35`, 0xc000000000050b84, 40 bytes, no
  arguments) loads the global word `picache_size` (through the same
  absolute-`$zero` addressing pattern noted above) into `$a1`, sets `$a0` to
  a fixed operation code (0x15, shifted into the high bits of a 64-bit value
  with `dsll32`), and calls `__cache_wb_inval` (0xc000000000050638, 188
  bytes) directly with those two values. Established.
- `__icache_inval` (`6.5.30/unix.IP35`, 0xc0000000000505b0, 136 bytes) and
  `__picache_inval` (0xc000000000050c74, 196 bytes) both use the MIPS
  `cache` instruction in a loop. In `__icache_inval`'s primary-instruction-
  cache path the loop steps by 128 bytes per `cache 0x17` (Hit Invalidate)
  instruction; in `__picache_inval`'s secondary-cache path the loop steps by
  64 bytes per pair of `cache` instructions. Established as a fact about the
  loop stride in this code.

**Cache line size: unknown.** No kernel global in this binary is named
`linesize`, `line_size` or similar for the primary instruction or data
cache; only `picache_size`, `pdcache_size` and `scache_linemask`, which size
the caches rather than giving their line length, were found by name. The
128-byte and 64-byte loop strides above are established as code facts, but
reading them as the literal hardware cache-line size is not confirmed
against a cited manual: no R10000-family datasheet was read for this page,
so a step of 128 bytes could equally be two 64-byte lines handled per loop
body. Settling this needs the published R10000/R12000/R14000 primary-cache-
line specification, cited by document number, matched against the stride;
it is not attempted here. This repository's own practice elsewhere
([kernel-tracing-and-modules.md](kernel-tracing-and-modules.md)) is the
same: statements about R10000 cache behaviour beyond the instruction
constants actually read are graded inferred or left unknown, not asserted
from general knowledge.

## Open questions

Which of the routines above (or any other) is safe to hook, how a module's
code would reach kernel text or vice versa, and how one CPU would stop or
synchronise with another for a patch, are a consumer's own design questions
against a running kernel, not facts this static binary settles by itself.
The row is recorded in [open-questions.md](../../questions/README.md) under
"Tracing and modules" rather than repeated here.
