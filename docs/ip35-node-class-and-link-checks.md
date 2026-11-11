# IP35 node class register, the flash configuration block and the link-up bit

What three small reads in the IP35 kernel (`6.5.30/unix.IP35`) are, now that the IP35 CPU PROM (version 6.210) has
been read. The PROM side, with the full table and the evidence for the hardware, is in the chips repository,
`docs/questions/ip35-board-word-and-config-block.md`. This page records what that settles about the kernel, and
what it does not.

## The class register

[klconfig.md](klconfig.md) ("The IP35 node class nibble") found that the kernel reads bits 15:12 of a doubleword at
`0x9000000014000000` (local) or `0x9000000034000000` plus the NASID shifted left 33 (node-indexed), and branches on
`0x4`, `0x8` and `0xc`. It left the register unidentified. The PROM reads the same two addresses with the same mask
(`0xf000`) and the same three comparisons, plus `0x2000`, so the register is the same.

- **Established.** The address is offset `0x4000000` of the node's local register space, under the local-versus-node
  alias that the other hub registers use (the register space is at `+0x10000000`, the remote alias at `+0x30000000`). The offset,
  both alias bases and the bits 15:12 field are in `KLCONFIG_DECODE.h` (`KLCONFIG_NODECLASS_*`). Re-read in `get_module_id` and
  `cpu_node_probe` of `6.5.30/unix.IP35` (sha256 `412a1aae553a348376bc30cacbaf65044de1745d30222f02ec5f88297dcb1dfa`): the node-indexed
  load from the 0x9000000034000000 pool word, the local load from 0x9000000014000000, and the 0xf000 mask with a compare against 0xc000 and 0x8000.
  The PROM's own use of it is in the chips page above: only bits 15:12 select a class, and bits 3:0 (bits 7:0 for class
  `0x8`) then pick a row of a processor-configuration table.
- **Established, by the shipped kernel's own census.** The kernel never examines any other bit of it (klconfig.md).
- **Inferred.** `0x4` is the Fuel class, `0x8` the Origin 300 class and `0xc` the PE-brick class. Reasons: the PROM
  table's `0x4` rows include each of the shipped Fuel processor options (500, 600, 700, 800 and 900 MHz, with the cache
  sizes and tap values a public Fuel inventory lists); OpenBSD's `sys/arch/sgi/sgi/ip27.h` names 4 for Fuel and 8
  for the Origin 300 and reads bits 15:12 of the same register; and the kernel's `0x4` effects (keyboard and mouse
  attach, graphics console start skipped, the narrower IOC3 sub-device mask) fit a one-node workstation. The kernel
  itself never names the platform.
- **Established.** The kernel's Chimera brick-type path is gated by the class. `set_sn12_type` (`0xc000000000041550`) asks the system
  controller for the brick-type character only when the class is none of `0x4`, `0x8`, `0xc`, and maps '7' to 1, '8' to 2, '9' to 3,
  '0' to 4 and 'Q' to 5; `get_chimera_type` returns that number. The IP35 CPU PROM has the same gate and the same mapping (chips page
  `docs/questions/ip35-board-class-and-flash-record.md`, routine at outer `0x1fc68ba0`). So a node for which the kernel can ever see
  Chimera type 3 (the workstation branch in `sc_command_interp`) is not class `0x4`, `0x8` or `0xc`: the IP53 and IP59 node boards, whose
  controller reports the Chimera server, blade and workstation brick types, belong to the remaining classes.
  The Fuel-only effects listed in [klconfig.md](klconfig.md) (keyboard and mouse attach through IOC3, graphics console start skipped,
  SCSI controller numbering, the narrower IOC3 sub-device mask) are therefore not taken on those boards.
- **Inferred.** The class of IP53 boards is `0x2`: OpenBSD's IP35 subtype list names 2 for the Origin 350 (and the PROM's pin report labels
  class `0x2000` with the IP53-family codename). The kernel has no compare with `0x2`, so for the kernel class 2 is just "default".
- **Unknown.** What drives the four bits on the hardware (the PROM's table treats bits 3:0 as the processor module's
  own code), the IP59 class and the low bits of an IP53 or IP59 word (route: one raw 64-bit read of `0x9000000014000000`), and class
  `0xc` beyond the kernel's PE-brick use.

## The 80-byte block that `syssgi` copies out

Selector 1200 (HW_CPU_CONFREG; [syssgi.md](syssgi.md)) copies 80 bytes from `0x9000000038000060` plus the node
shifted left 33 when its fourth argument is non-zero, and leaves their meaning open.

- **Established.** The 80 bytes are the node's flash configuration block, at offset `0x60` of the boot flash. The
  PROM's own routine that fills a record of this shape stores its fields at these offsets: `0x15` at `+0x00`, the
  processor mode word at `+0x04`, the ASCII bytes `ip27conf` at `+0x08`, the processor and bus clocks in Hz at `+0x10`
  and `+0x18`, `0x1312d0` at `+0x20`, 1 at `+0x28`, a flash-timing byte at `+0x2c`, the machine type at `+0x30`, a
  zero checksum adjustment at `+0x34` and a second timing byte at `+0x3c`. The public SN1 header
  `ip27config.h` (Linux 2.5.40, `include/asm-ia64/sn/sn1/`) lays out the same fields at the same offsets.
- **Established.** The PROM compares the flash copy with a reference record at offsets `0x00`, `0x04`, `0x08`, `0x10`,
  `0x18`, `0x28`, `0x2c`, `0x30` and `0x3c`, and prints a "values did not match" notice when they differ.
- **Established.** The kernel does not interpret the block: the only loads of the node-addressed pool word for
  `0x9000000038000060` (gp+4800 in `6.5.30/unix.IP35`; address `0xc000000001644a30`) are in `syssgi`
  (the copy-out at `0xc0000000002e2f28`) and `hubdev_prombase_get` (`0xc00000000006d608`, which loads the
  `0x9000000038000000` base). There is no pool word for the local `0x...18000060` address in the kernel.
- **Unknown.** Whether the boot message that names a machine variant (the string "Selecting SN11", in the second
  read-only data section of `6.5.30/unix.IP35` at file offset `0x4db4f0`, next to the strings for the baseio and the
  default server) is chosen from the machine-type field at `+0x30`. The reference to the string is not a plain
  address build: no `lui`/`daddiu` pair, no pool word and no data pointer to it was found in `unix.IP35` or in the
  relocations of `IP35boot/kernel.o`. The user-space flash utility that takes the machine type is not held.

## The link-up bit

- **Established.** `0xc000000000051b04` (a static helper next to `probe_routers`, without its own symbol) loads the
  doubleword at `0x9200000001680000` (gp+2456 in `6.5.30/unix.IP35`; address `0xc000000001644108`), shifts it right 1
  and returns bit 0 of the result, so it returns bit 1 of the register. That register is NI port status
  (offset `0x680000`; chips `chips/bedrock/README.md`). `probe_routers` (`0xc000000000051bb4`) calls the helper
  first. When it returns zero the routine logs a message through `ip27log_printf` (string at gp-22984 plus
  `0x260`) and returns 0; otherwise it continues to the router reads (documented in [ip35-router.md](ip35-router.md)).
  This is the only load of that pool word in the kernel.
- **Agreement.** The public SN1 header (`hubni_next.h`) defines bit 1 of the register as the link-up mask and
  `hubni.h` gives bits 1:0 as a two-bit "port status" field.
- **Unknown.** What bit 0 and bit 2 mean, and the value of the whole register on a node with no router. The kernel
  needs only bit 1.
