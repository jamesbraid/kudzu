# kudzu

Reverse-engineering and specification repository for SGI IRIX 6.5.30 and 6.5.22.

It starts with the hardware-facing and boot-critical kernel: the PROM interface and KLCONFIG parsing, hardware-graph construction, interrupt routing, NUMA and memory initialisation, the topology checks that panic or skip nodes, the device attach framework, and a debugging map of structure layouts, entry points and panic and assert sites. The long-term scope is all of IRIX and its add-ons: the full kernel, CXFS, ONC RPC and NFS, the base applications and libraries.

There is no reimplementation here. The findings are specifications that emulator datasheets, drivers and debuggers elsewhere can use.

The layout follows the sibling repositories for graphics and chip reverse engineering. Proprietary IRIX binaries are not tracked: copy them from your own media into the ignored `root/` directory as [IRIX_FILES.md](IRIX_FILES.md) describes. [AGENTS.md](AGENTS.md) has the scope rule, the evidence rules and the finish line; [docs/coverage.md](docs/coverage.md) is the map of what is covered; [docs/open-questions.md](docs/open-questions.md) lists what the binaries have not yet answered; [docs/published-sources.md](docs/published-sources.md) says which public trees are pinned and why.

## Document index

Every hand-written document under `docs/` and the top-level Markdown files, grouped by area. Generated pages are not listed one by one; the lines at the end say which they are.

### Start here

- [AGENTS.md](AGENTS.md): Scope, evidence rules, layout and checks for the IRIX 6.5 specification
- [IRIX_FILES.md](IRIX_FILES.md): IRIX files needed for this repository
- [docs/coverage.md](docs/coverage.md): Coverage map and measured totals for the inventory tables
- [docs/open-questions.md](docs/open-questions.md): Questions the binaries have not yet answered, with routes tried
- [docs/published-sources.md](docs/published-sources.md): Public source trees pinned as hints, not evidence, with reasons
- [docs/version-diffs.md](docs/version-diffs.md): IRIX 6.5.22 against 6.5.30: routine-level differences

### Hardware, boot and device layers

- [docs/klconfig.md](docs/klconfig.md): KLCONFIG as the kernel reads it
- [docs/hwgraph.md](docs/hwgraph.md): Hardware graph construction from KLCONFIG board lists
- [docs/hwgraph-paths.md](docs/hwgraph-paths.md): Hardware-graph path and edge names in the 6.5.30 kernels
- [docs/interrupts.md](docs/interrupts.md): Hub interrupt routing as the kernel programs it
- [docs/ioconfig.md](docs/ioconfig.md): Device unit numbers and the ioconfig file
- [docs/input-path.md](docs/input-path.md): Input events from a serial tablet module through the shared-memory queue to Xsgi
- [docs/nic.md](docs/nic.md): Board identity read from Dallas 1-Wire number-in-a-can parts
- [docs/pci-intr-pin.md](docs/pci-intr-pin.md): PCI interrupt pins on the IP35 bridges
- [docs/pciio-driver-interface.md](docs/pciio-driver-interface.md): PCI driver interface: space codes, flag bits, endianness, interrupt target, id registry
- [docs/sysgen.md](docs/sysgen.md): Kernel configuration tables: driver files, tunables and link directives

### Generic kernel

- [docs/checkpoint-restart.md](docs/checkpoint-restart.md): Checkpoint and restart in the IRIX kernels
- [docs/driver-ioctls.md](docs/driver-ioctls.md): Driver control requests: IP30 kernel dispatch
- [docs/errnos.md](docs/errnos.md): Error numbers returned by each system call handler and selector
- [docs/kernel-debug-signatures.md](docs/kernel-debug-signatures.md): Kernel routine signatures from debug information
- [docs/kernel-debugger-commands.md](docs/kernel-debugger-commands.md): Commands the optional kernel debugger registers in its objects
- [docs/kernel-optional-subsystems.md](docs/kernel-optional-subsystems.md): Kernel features that are stubbed, empty, absent or ship separately
- [docs/kernel-probe-points.md](docs/kernel-probe-points.md): Kernel probe points: a function reference table
- [docs/kernel-tracing-and-modules.md](docs/kernel-tracing-and-modules.md): Kernel tracing, loadable modules and runtime code patching
- [docs/module-interfaces.md](docs/module-interfaces.md): Loadable kernel modules and performance counters
- [docs/namespace-dispatch.md](docs/namespace-dispatch.md): Namespace dispatch: pointer tables and where the code lives
- [docs/network-and-snapshots.md](docs/network-and-snapshots.md): Network addressing and filtering; XVM and XFS copy-on-write primitives
- [docs/network-dataplane-and-vfs.md](docs/network-dataplane-and-vfs.md): Network dataplane and virtual file system attachment points
- [docs/page-migration.md](docs/page-migration.md): Page migration and cross-partition communication in the 6.5.30 kernels
- [docs/numa-text-replication.md](docs/numa-text-replication.md): Kernel text replication, user page replication and node numbering after start-up (IP27 and IP35)
- [docs/process-model.md](docs/process-model.md): Process model, groups, root directories and privilege checks
- [docs/resource-control.md](docs/resource-control.md): Resource control and NUMA placement: cpusets, miser, limits
- [docs/scsi-cdrom-inquiry.md](docs/scsi-cdrom-inquiry.md): The INQUIRY vendor and product text of a SCSI CD-ROM
- [docs/syscalls.md](docs/syscalls.md): System call handlers, one entry per native table handler
- [docs/syscall-selector-crosscheck.md](docs/syscall-selector-crosscheck.md): Syscall and syssgi tables cross-checked against public lists
- [docs/syssgi.md](docs/syssgi.md): syssgi selectors, one at a time, starting with NUMA and memory
- [docs/trusted-irix.md](docs/trusted-irix.md): Trusted IRIX (CMW, B1, MLS): what shipped and what it could isolate
- [docs/vfs-layering.md](docs/vfs-layering.md): Loopback file system and the layering interfaces of the VFS

### IP27 kernel

- [docs/ip27-driver-attach-interrupts.md](docs/ip27-driver-attach-interrupts.md): IP27 kernel: driver attach and interrupt routines
- [docs/ip27-kernel-io.md](docs/ip27-kernel-io.md): IP27 kernel: I/O bridge, hub and device routines

### IP30 kernel

- [docs/ip30-kernel-functions.md](docs/ip30-kernel-functions.md): IP30 kernel: one entry per routine
- [docs/ip30-timer-compare-rearm.md](docs/ip30-timer-compare-rearm.md): IP30 scheduler clock: CP0 Compare re-arm and a pull-in defect

### IP35 kernel

- [docs/ip35-cache-error.md](docs/ip35-cache-error.md): Cache error handling in the IP35 kernel
- [docs/ip35-crash-dump.md](docs/ip35-crash-dump.md): Crash dump of the IP35 kernel: the Hub information pass
- [docs/ip35-halt-and-reset.md](docs/ip35-halt-and-reset.md): IP35 kernel halt, power-off and reboot paths
- [docs/ip35-hub-errors.md](docs/ip35-hub-errors.md): Hub error handling in the IP35 kernel
- [docs/ip35-ioc3.md](docs/ip35-ioc3.md): IOC3 driver routines of the IP35 kernel
- [docs/ip35-memory-error-address-decode.md](docs/ip35-memory-error-address-decode.md): IP35 kernel readers of the memory-controller configuration word
- [docs/ip35-node-class-and-link-checks.md](docs/ip35-node-class-and-link-checks.md): IP35 node class register, flash configuration block and link-up bit
- [docs/ip35-panic-support.md](docs/ip35-panic-support.md): Panic and assertion support in the IP35 kernel
- [docs/ip35-pci-bridge.md](docs/ip35-pci-bridge.md): PCI bridge driver routines of the IP35 kernel
- [docs/ip35-pci-interrupt-repost.md](docs/ip35-pci-interrupt-repost.md): How a PCI interrupt is re-posted after its handlers finish
- [docs/ip35-pcibr-registers.md](docs/ip35-pcibr-registers.md): Bridge register accessors in the IP35 pcibr object
- [docs/ip35-pciio.md](docs/ip35-pciio.md): Generic PCI layer routines of the IP35 kernel
- [docs/ip35-router.md](docs/ip35-router.md): Router driver routines of the IP35 kernel
- [docs/ip35-rtc-and-cycle-counter.md](docs/ip35-rtc-and-cycle-counter.md): How the IP35 kernel reads the real-time counter's period and exposes the cycle counter
- [docs/ip35-tg-driver.md](docs/ip35-tg-driver.md): What the gigabit Ethernet tg driver expects of its controller
- [docs/ip35-xtalk.md](docs/ip35-xtalk.md): Crosstalk layer entry points of the IP35 kernel

### Kernel topic notes (docs/kernel/)

- [docs/kernel/2106-core-time.md](docs/kernel/2106-core-time.md): IRIX 6.5.30 core kernel time at 2038 and 2106
- [docs/kernel/boot-time-nsd-getpwuid.md](docs/kernel/boot-time-nsd-getpwuid.md): getpwuid failing at boot once the clock passes 2^31
- [docs/kernel/idle-and-wait-loops.md](docs/kernel/idle-and-wait-loops.md): Idle loop and look-alike wait loops in the IRIX kernels
- [docs/kernel/xfs-mount-and-block-io.md](docs/kernel/xfs-mount-and-block-io.md): IRIX 6.5.30 XFS mount path, superblock checks and block I/O
- [docs/kernel/xfs-timestamps.md](docs/kernel/xfs-timestamps.md): XFS inode timestamps: reads, writes, widths and signedness
- [docs/kernel/xtalk-software.md](docs/kernel/xtalk-software.md): Software side of the crosstalk link in IRIX
- [docs/kernel/y2038-signed-time-scan.md](docs/kernel/y2038-signed-time-scan.md): Signed 32-bit time scan across the shipped 6.5.30 binaries

### Userland

- [docs/userland.md](docs/userland.md): IRIX 6.5.30 base user-space programs, one entry each
- [docs/userland-abi-and-loader.md](docs/userland-abi-and-loader.md): Userland ABI and the dynamic loader
- [docs/userland-libraries-and-isa.md](docs/userland-libraries-and-isa.md): Userland libraries and the instruction-set level

### On-disk formats (docs/formats/)

- [docs/formats/efs.md](docs/formats/efs.md): EFS on-disk format: superblock, bitmaps, inodes and extents
- [docs/formats/volume-header.md](docs/formats/volume-header.md): SGI disk volume header (dvh) on-disk format
- [docs/formats/xfs.md](docs/formats/xfs.md): XFS on-disk format as written by IRIX 6.5.30
- [docs/formats/xlv.md](docs/formats/xlv.md): XLV on-disk label format, recovered from the 6.5.30 binaries
- [docs/formats/xvm.md](docs/formats/xvm.md): XVM volume label and metadata, recovered from the binaries

### Not indexed one by one

- Generated, not indexed one by one: the 46 layout pages in `docs/structs/` (each says it is generated by `scripts/struct_fields.py`) and `docs/panics.md`.
- Instruction-reading notes `docs/instruction-readings-kudzu-g1.md` and `docs/instruction-readings-kudzu-g2.md`, not indexed.

## Hardware definitions

The hardware structures this repository specifies are described once, in TOML under `defs/`. Each unit file gives its constants, registers and fields with a grade (established, inferred or unknown) and a source. `gen/` holds what is generated from them: C headers, a Rust crate, Python decode helpers, flat JSON, a genxml export and one reference page per unit under `gen/docs/`. Do not edit `gen/`; edit the TOML and run `retools/tools/defs/defs-gen`. The `*_DECODE.h` files at the root are forwarding headers for the old include paths.

## Running the tools

After cloning, run `git submodule update --init retools`: the container definitions and shared scripts are in it, under `retools/tools/` (shared) and `retools/tools/kudzu/` (this repository's). Run them from the repository root by that path. The other submodules are public reference trees and are large; initialise them only when you want them ([tools/README.md](tools/README.md) describes `tools/setup-clone.sh`, which links `root/` and fetches the public-source submodules).

The host needs Podman, git and python3. Everything else runs in a container; [tools/README.md](tools/README.md) has the commands:

    retools/tools/re-run python3 retools/tools/inventory.py             # binaries, functions, ioctls, scope
    retools/tools/re-run python3 retools/tools/kudzu/os_tables.py     # system calls, structures, panic sites, formats
    tools/check-os-tables                                     # fail if the committed tables are stale

The generated tables are in [docs/inventory/](docs/inventory/README.md). `git status --short docs` shows nothing after a run when your `root/` matches `scripts/inventory-root.sha256`.

Licence: BSD-3-Clause, see [LICENSE](LICENSE). The scripts descend from gr4's and keep its notice in [retools/tools/LICENSE-gr4](retools/tools/LICENSE-gr4).
