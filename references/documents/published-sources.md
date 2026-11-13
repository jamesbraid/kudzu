# Published sources

Public trees are **hints**, not evidence: every fact one suggests must be confirmed at an address in a 6.5.30 or 6.5.22 binary, or graded inferred or unknown ([AGENTS.md](../../AGENTS.md#2-evidence)). They are meant to be pinned submodules. Several are pinned at an old commit because the IRIX parts were later removed.

The trees below are pinned submodules under [references/code/](../code/) at the commits listed. A directory name carries the version or date it pins where the project has several pins (`linux-2.4.37`, `linux-2.5.40`, `linux-2008-07-15`, `netbsd-2011-04-26`, `xfsprogs-1.0.0`).

| Path | Source | Pin | Why this pin |
|---|---|---|---|
| `references/code/m2c` | techomancer/m2c, branch irixn32 | b3e0876a | Assembly-to-C decompiler with the IRIX n32 target |
| `references/code/spimdisasm` | Decompollaborate/spimdisasm | 4b5d4c68 | MIPS object disassembly and symbol recovery |
| `references/code/libdwarf` | davea42/libdwarf-code | fa6cb32 | dwarfdump that reads the SGI MIPS DWARF 2 |
| `references/code/ogl-sample` | jrmuizel/ogl-sample | b606627 | OpenGL sample implementation, for the later GL scope |
| `references/code/linux` | torvalds/linux | d89e1d18 | IP27 and IP30 platform code, crossbar and widget probing |
| `references/code/linux-2.5.40` | kernel.org history tree | 7570df54 | 2.5.40: IP27 code before rewrites; carries the first public Linux XFS import (merged in 2.5.36), the oldest SGI Linux XFS history reachable here |
| `references/code/linux-2.4.37` | xtreamerdev/linux-mips | e0dbf803 | linux-mips 2.4: IP27, IP30, IP32 in original form |
| `references/code/linux-ip30` | jaaron/linux-mips-ip30 | bff2b6da | Octane (IP30) port |
| `references/code/linux-2008-07-15` | torvalds/linux | 99728851 | Parent of the 2008-07-15 commit that removed IRIX compatibility: Linux's IRIX a.out/ELF loader, Linux's IRIX syscall-compat code and the irix signal and ioctl code are present (verified at this commit) |
| `references/code/netbsd` | NetBSD/src | 0747dc05 | sgimips, ARCS and mips support |
| `references/code/netbsd-2011-04-26` | NetBSD/src | eee31751 | Parent of the 2011-04-26 commit "Remove IRIX emulation": last tree with `sys/compat/irix` (45 entries verified). NetBSD's history is in one repository, so the removed compat code is reachable from this pin |
| `references/code/openbsd` | openbsd/src | 7011342f | sgi platform: IP27, IP30, IP35, ARCS, xbridge and xbow |
| `references/code/mame` | mamedev/mame | 803a05bb | SGI machine models |
| `references/code/gxemul` | retroprom/gxemul | 234a181a | SGI machine emulation and ARCS handling |
| `references/code/qemu-irix` | n64decomp/qemu-irix | 6d7d1fde | User-mode emulator for IRIX binaries; cross-check for the system call surface |
| `references/code/xiri` | salvogendut/xiri | ace611e3 | Another independent reading of the IRIX system call surface |
| `references/code/binutils-gdb` | sourceware.org/git/binutils-gdb | 28498c42 | Last tree with IRIX support: `gdb/solib-irix.c`, `mips-irix-tdep.c`, `irix5-nat.c` (verified at this commit) and bfd's ECOFF readers (`bfd/ecoff.c`, `ecofflink.c`) for the symbol and debug formats |
| `references/code/gcc` | gcc-mirror/gcc | 02f6574b | Parent of "Remove obsolete IRIX 6.5 support" (2012-03-14): carries `gcc/config/mips/iris6.h` (verified) |
| `references/code/xfsprogs-1.0.0` | kernel.org xfsprogs-dev | f91dde66 | Tag `Release-1_0_0`: the oldest release tag of the public xfsprogs history |
| `references/code/pcp` | performancecopilot/pcp | 407329f9 | Performance Co-Pilot began on IRIX: its IRIX metric agents and the kernel statistics they read |
| `references/code/fam` | saminigod/fam | 7c875b55 | File Alteration Monitor, imported from SGI's site; the kernel imon interface |
| `references/code/open64` | open64-compiler/open64 | d69db73f | Descended from SGI's Pro64/MIPSpro compiler: object ABI, debug and symbol formats |
| `references/code/bsd44-lite2` | sergev/4.4BSD-Lite2 | 50587b00 | Lineage of IRIX BSD networking and the NFS code |
| `references/code/illumos-gate` | illumos/illumos-gate | 336772c3 | SVR4-lineage kernel: vnodes and VFS, STREAMS, procfs |
| `references/code/irix-657m-src` | calmsacibis995, public IRIX 6.5.7m tree | 822f4fcb | A published source tree for an older release. Hint only: the targets are 6.5.30 and 6.5.22 |

Not pinned, with the routes tried:

- **Sun ONC RPC 4.0 source (rpcsrc) and TI-RPC.** The one GitHub repository named `rpcsrc` found is an unrelated Java project. The libtirpc upstream repositories refused anonymous access from this host (403 on `git.linux-nfs.org`, no answer from the SourceForge mirror). 4.4BSD-Lite2 carries the BSD descendant of the same RPC code, which is the pin that stands in until a trustworthy host for rpcsrc or libtirpc is found.
- **Older SGI Linux XFS history than the 2.5.36 import.** The kernel.org `xfs-linux` tree starts much later. Nothing earlier than the history-tree import and the xfsprogs release tag was reachable.
- Projects built on the leaked IRIX source (for example the irix7 project) are not used.
