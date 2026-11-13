# IP35 kernel: the readers of the memory-controller configuration word

The IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, linked 64-bit image with a symbol table) sizes memory from the node
board's KLCONFIG memory component, not from a hub register ([klconfig.md](../../formats/klconfig/README.md), "Memory sizing"). This page
records where the kernel does read the memory controller's configuration word: the routines that turn a memory error's
address into a DIMM or bank name (local form), and the two accessibility checks that test one bit of it (node-alias
form). Grades as in [klconfig.md](../../formats/klconfig/README.md); method: the retools Capstone decoder (`tools/hostfacts.py`) over the
linked image.

## What is read

A scan of the literal pool for addresses of the memory controller's blocks finds two forms of the word:
`0x9200000001780008` (local form, pool address 0xc0000000016c9be0, `gp` + 1776), whose only three readers are the
decode routines below, and the node-alias base `0x9200000001f80000` (pool address 0xc0000000016c99b0, `gp` + 1216), whose
seven loads are in `memory_read_accessible`, `memory_write_accessible`, `fetchop_flush`, `hub_error_clear` and
`log_md_errors`. (Checked by searching every load from each pool slot in the kernel text.) Of the node-alias users only
the first two read the offset +8 word; the others use +0x20, +0x28, +0x40 and +0x58. **Established.**

## The two accessibility checks

`memory_read_accessible` (0xc00000000004f58c, 384 bytes) and `memory_write_accessible` (0xc00000000004f760, 372 bytes) take
an address, take the node number from address bits 39:33 (or the local node, from `0xc00000000004ed04`, when the
address names none), load the configuration word at node alias + 8 and test **bit 44** (0x100000000000). When it is
set they read the doubleword at node alias + 0 and test the bit selected by a small number that the helper at 0xc00000000004ed04 returns (inferred to be the calling
processor's region); a set bit makes the address accessible at once. When bit 44 is clear, or the region bit is clear, they go on to read one entry
of the per-page protection table in the directory memory (the address is built from the node bits, address bits 32:3
and table bases at `gp` + 1224 and `gp` + 1200) and treat a low byte of 0xff or 0x0f as accessible. **Established** for
the path; the meaning of bit 44 as a protection enable is **inferred** (the kernel only tests it and no kernel text
names it).

## The three routines

Each loads the word, takes bits 33:32 as S, and forms A = (address & 0x1ffffffff) XOR (S << 30) from its second
argument. **Established** for all arithmetic below (all three read in full).

- `mdir_xlate_addr_mem` (0xc00000000006b9ec, 164 bytes): the pair number is bits 32:31 of A. With a negative third
  argument it reports both positions of the pair (2 x pair and 2 x pair + 1). Otherwise it reports 2 x pair plus
  bit k of a constant, where k is the third argument divided by 8 and the constant is 0x1aa when address bit 3 is set
  and 0x1e when it is clear: the failing byte lane picks which of the pair's two positions is named. Inferred
  that "position" is a DIMM slot (the callee at 0xc0000000002cab4c formats a name from a table at `gp` - 22520 + 128).
- `mdir_xlate_addr_pdir` (0xc00000000006ba90, 128 bytes): 2 x pair plus one when bit 4 of the third argument is set
  (zero when the third argument is negative, which reports both positions as above).
- `mdir_xlate_addr_sdir` (0xc00000000006bb10, 80 bytes): reports bits 32:30 of A, the bank number.

## What follows

- The kernel's own view of the bank-select field is the same XOR at address bits 31:30 (S is two bits, so bit 32 is
  untouched): the eight 1 GB slots of a node are permuted only within each group of four that share bit 32.
  Established for these three routines.
- None of the loads above is followed by a store back to the word. The kernel's IP35 slot-size routine (0xc00000000004f408) XORs the slot number with a halfword at +58 of the memory
  component ([klconfig.md](../../formats/klconfig/README.md)); that this halfword equals S is **inferred**, and the PROM store that fills it
  was not found (see the chips repository's `docs/questions/ip35-prom-memory-bank-config.md` item 4).
- Software that never reads bits 33:32 sees no effect of S except in this error decode, so an implementation that gets S wrong
  mislabels error reports and nothing else, provided the access path itself applies the XOR.
