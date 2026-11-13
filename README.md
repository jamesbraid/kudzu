# kudzu

Reverse-engineering and specification repository for SGI IRIX 6.5.30 and 6.5.22.

It starts with the hardware-facing and boot-critical kernel: the PROM interface and KLCONFIG parsing, hardware-graph construction, interrupt routing, NUMA and memory initialisation, the topology checks that panic or skip nodes, the device attach framework, and a debugging map of structure layouts, entry points and panic and assert sites. The long-term scope is all of IRIX and its add-ons: the full kernel, CXFS, ONC RPC and NFS, the base applications and libraries.

There is no reimplementation here. The findings are specifications that datasheets, drivers and debuggers elsewhere can use.

The layout follows the sibling repositories for graphics and chip reverse engineering. Proprietary IRIX binaries are not tracked: copy them from your own media into the ignored `root/` directory as [evidence/irix-files.md](evidence/irix-files.md) describes. [AGENTS.md](AGENTS.md) has the scope rule, the evidence rules and the finish line; [evidence/coverage.md](evidence/coverage.md) is the map of what is covered; [questions/README.md](questions/README.md) lists what the binaries have not yet answered; [references/documents/published-sources.md](references/documents/published-sources.md) says which public trees are pinned and why.

## Start here

- To use the findings in code (tool authors and driver writers), take the generated C headers and the Rust crate from [generated/c](generated/c) and [generated/rust](generated/rust).
- To understand a disk format, a record or a kernel table, read its directory under [formats/](formats/): `README.md` explains it, `format.toml` holds every graded field and `format.md` is the generated table.
- How IRIX drives the hardware and what the kernel and the base system do (interrupts, the hardware graph, the device drivers, system calls, the userland) is under [software/](software/): [software/kernel/](software/kernel/), [software/video/](software/video/) and [software/userland/](software/userland/).
- What is covered is [evidence/coverage.md](evidence/coverage.md). The routine readings, the generated inventory tables and the cross-checks are in [evidence/](evidence/), the open questions in [questions/README.md](questions/README.md), the published sources in [references/documents/](references/documents/) and the pinned public source trees, as submodules, in [references/code/](references/code/).
- The repository's own generators and their settings are in [tools/](tools/); the shared toolkit is the `retools/` submodule.

## Document index

Every hand-written document under `software/`, `formats/`, `evidence/`, `questions/` and `references/`, and the top-level Markdown files, grouped by area. Generated pages are not listed one by one; the lines at the end say which they are.

### Project files

- [AGENTS.md](AGENTS.md): Scope, evidence rules, layout and checks for the IRIX 6.5 specification
- [evidence/irix-files.md](evidence/irix-files.md): IRIX files needed for this repository
- [evidence/coverage.md](evidence/coverage.md): Coverage map and measured totals for the inventory tables
- [questions/README.md](questions/README.md): Questions the binaries have not yet answered, with routes tried
- [references/documents/published-sources.md](references/documents/published-sources.md): Public source trees pinned as hints, not evidence, with reasons
- [evidence/version-diffs.md](evidence/version-diffs.md): IRIX 6.5.22 against 6.5.30: routine-level differences

### Hardware, boot and device layers

- [formats/klconfig/README.md](formats/klconfig/README.md): KLCONFIG as the kernel reads it
- [software/kernel/hwgraph.md](software/kernel/hwgraph.md): Hardware graph construction from KLCONFIG board lists
- [software/kernel/hwgraph-paths.md](software/kernel/hwgraph-paths.md): Hardware-graph path and edge names in the 6.5.30 kernels
- [software/kernel/interrupts.md](software/kernel/interrupts.md): Hub interrupt routing as the kernel programs it
- [software/video/gbe.md](software/video/gbe.md): O2 GBE two-wire buses, flat-panel timing words and the dual-channel option probe
- [software/video/crime.md](software/video/crime.md): O2 CRIME: identity, interrupt inputs, renderer interface buffer, TLBs and the pixel pipeline as the shipped IRIX graphics code drives them
- [software/video/mace-video.md](software/video/mace-video.md): O2 MACE video: input and output channels, the GBE capture block, the two-wire bus and timer snapshots as the IRIX video driver uses them
- [software/kernel/ioconfig.md](software/kernel/ioconfig.md): Device unit numbers and the ioconfig file
- [software/kernel/input-path.md](software/kernel/input-path.md): Input events from a serial tablet module through the shared-memory queue to Xsgi
- [formats/nic_record/README.md](formats/nic_record/README.md): Board identity read from Dallas 1-Wire number-in-a-can parts
- [software/kernel/pci-intr-pin.md](software/kernel/pci-intr-pin.md): PCI interrupt pins on the IP35 bridges
- [software/kernel/pciio-driver-interface.md](software/kernel/pciio-driver-interface.md): PCI driver interface: space codes, flag bits, endianness, interrupt target, id registry
- [software/kernel/pcibr-multifunction-scan.md](software/kernel/pcibr-multifunction-scan.md): PCI slot scan: multi-function cards, vertex names, PCI-to-PCI bridge functions
- [software/kernel/sysgen.md](software/kernel/sysgen.md): Kernel configuration tables: driver files, tunables and link directives

### Generic kernel

- [software/kernel/checkpoint-restart.md](software/kernel/checkpoint-restart.md): Checkpoint and restart in the IRIX kernels
- [software/kernel/driver-ioctls.md](software/kernel/driver-ioctls.md): Driver control requests: IP30 kernel dispatch
- [software/kernel/errnos.md](software/kernel/errnos.md): Error numbers returned by each system call handler and selector
- [software/kernel/kernel-debug-signatures.md](software/kernel/kernel-debug-signatures.md): Kernel routine signatures from debug information
- [software/kernel/kernel-debugger-commands.md](software/kernel/kernel-debugger-commands.md): Commands the optional kernel debugger registers in its objects
- [software/kernel/kernel-optional-subsystems.md](software/kernel/kernel-optional-subsystems.md): Kernel features that are stubbed, empty, absent or ship separately
- [software/kernel/kernel-probe-points.md](software/kernel/kernel-probe-points.md): Kernel probe points: a function reference table
- [software/kernel/kernel-tracing-and-modules.md](software/kernel/kernel-tracing-and-modules.md): Kernel tracing, loadable modules and runtime code patching
- [software/kernel/module-interfaces.md](software/kernel/module-interfaces.md): Loadable kernel modules and performance counters
- [software/kernel/namespace-dispatch.md](software/kernel/namespace-dispatch.md): Namespace dispatch: pointer tables and where the code lives
- [software/kernel/network-and-snapshots.md](software/kernel/network-and-snapshots.md): Network addressing and filtering; XVM and XFS copy-on-write primitives
- [software/kernel/network-dataplane-and-vfs.md](software/kernel/network-dataplane-and-vfs.md): Network dataplane and virtual file system attachment points
- [software/kernel/page-migration.md](software/kernel/page-migration.md): Page migration and cross-partition communication in the 6.5.30 kernels
- [software/kernel/numa-text-replication.md](software/kernel/numa-text-replication.md): Kernel text replication, user page replication and node numbering after start-up (IP27 and IP35)
- [formats/r10k_tlb/README.md](formats/r10k_tlb/README.md): R10000 TLB geometry and how IRIX 6.5.7 on IP27 uses its wired entries, refills, probes and flushes
- [software/kernel/process-model.md](software/kernel/process-model.md): Process model, groups, root directories and privilege checks
- [software/kernel/resource-control.md](software/kernel/resource-control.md): Resource control and NUMA placement: cpusets, miser, limits
- [formats/scsi/README.md](formats/scsi/README.md): The INQUIRY vendor and product text of a SCSI CD-ROM
- [software/kernel/syscalls.md](software/kernel/syscalls.md): System call handlers, one entry per native table handler
- [evidence/syscall-selector-crosscheck.md](evidence/syscall-selector-crosscheck.md): Syscall and syssgi tables cross-checked against public lists
- [software/kernel/syssgi.md](software/kernel/syssgi.md): syssgi selectors, one at a time, starting with NUMA and memory
- [software/kernel/trusted-irix.md](software/kernel/trusted-irix.md): Trusted IRIX (CMW, B1, MLS): what shipped and what it could isolate
- [software/kernel/vfs-layering.md](software/kernel/vfs-layering.md): Loopback file system and the layering interfaces of the VFS

### IP27 kernel

- [software/kernel/ip27-driver-attach-interrupts.md](software/kernel/ip27-driver-attach-interrupts.md): IP27 kernel: driver attach and interrupt routines
- [software/kernel/ip27-kernel-io.md](software/kernel/ip27-kernel-io.md): IP27 kernel: I/O bridge, hub and device routines

### IP30 kernel

- [software/kernel/ip30-kernel-functions.md](software/kernel/ip30-kernel-functions.md): IP30 kernel: one entry per routine
- [software/kernel/ip30-timer-compare-rearm.md](software/kernel/ip30-timer-compare-rearm.md): IP30 scheduler clock: CP0 Compare re-arm and a pull-in defect

### IP35 kernel

- [software/kernel/ip35-cache-error.md](software/kernel/ip35-cache-error.md): Cache error handling in the IP35 kernel
- [software/kernel/ip35-crash-dump.md](software/kernel/ip35-crash-dump.md): Crash dump of the IP35 kernel: the Hub information pass
- [software/kernel/ip35-halt-and-reset.md](software/kernel/ip35-halt-and-reset.md): IP35 kernel halt, power-off and reboot paths
- [software/kernel/ip35-hub-errors.md](software/kernel/ip35-hub-errors.md): Hub error handling in the IP35 kernel
- [software/kernel/ip35-ioc3.md](software/kernel/ip35-ioc3.md): IOC3 driver routines of the IP35 kernel
- [software/kernel/ip35-memory-error-address-decode.md](software/kernel/ip35-memory-error-address-decode.md): IP35 kernel readers of the memory-controller configuration word
- [software/kernel/ip35-node-class-and-link-checks.md](software/kernel/ip35-node-class-and-link-checks.md): IP35 node class register, flash configuration block and link-up bit
- [software/kernel/ip35-panic-support.md](software/kernel/ip35-panic-support.md): Panic and assertion support in the IP35 kernel
- [software/kernel/ip35-pci-bridge.md](software/kernel/ip35-pci-bridge.md): PCI bridge driver routines of the IP35 kernel
- [software/kernel/ip35-pci-interrupt-repost.md](software/kernel/ip35-pci-interrupt-repost.md): How a PCI interrupt is re-posted after its handlers finish
- [software/kernel/ip35-pcibr-registers.md](software/kernel/ip35-pcibr-registers.md): Bridge register accessors in the IP35 pcibr object
- [software/kernel/ip35-pciio.md](software/kernel/ip35-pciio.md): Generic PCI layer routines of the IP35 kernel
- [software/kernel/ip35-router.md](software/kernel/ip35-router.md): Router driver routines of the IP35 kernel
- [software/kernel/ip35-rtc-and-cycle-counter.md](software/kernel/ip35-rtc-and-cycle-counter.md): How the IP35 kernel reads the real-time counter's period and exposes the cycle counter
- [software/kernel/ip35-tg-driver.md](software/kernel/ip35-tg-driver.md): What the gigabit Ethernet tg driver expects of its controller
- [software/kernel/ip35-xtalk.md](software/kernel/ip35-xtalk.md): Crosstalk layer entry points of the IP35 kernel

### Kernel topic notes

- [software/kernel/2106-core-time.md](software/kernel/2106-core-time.md): IRIX 6.5.30 core kernel time at 2038 and 2106
- [software/kernel/boot-time-nsd-getpwuid.md](software/kernel/boot-time-nsd-getpwuid.md): getpwuid failing at boot once the clock passes 2^31
- [software/kernel/hardware-register-use.md](software/kernel/hardware-register-use.md): What the IRIX kernel, libraries and X modules do with registers whose maps live in the chips and crime repositories
- [software/kernel/idle-and-wait-loops.md](software/kernel/idle-and-wait-loops.md): Idle loop and look-alike wait loops in the IRIX kernels
- [software/kernel/xfs-mount-and-block-io.md](software/kernel/xfs-mount-and-block-io.md): IRIX 6.5.30 XFS mount path, superblock checks and block I/O
- [software/kernel/xfs-timestamps.md](software/kernel/xfs-timestamps.md): XFS inode timestamps: reads, writes, widths and signedness
- [software/kernel/xtalk-software.md](software/kernel/xtalk-software.md): Software side of the crosstalk link in IRIX
- [software/kernel/y2038-signed-time-scan.md](software/kernel/y2038-signed-time-scan.md): Signed 32-bit time scan across the shipped 6.5.30 binaries

### Userland

- [software/userland/userland.md](software/userland/userland.md): IRIX 6.5.30 base user-space programs, one entry each
- [software/video/dmedia-video-drivers.md](software/video/dmedia-video-drivers.md): host software for the DMediaPro DM2 (XT-DIGVID) and DIVO video boards: drivers, ML and VL modules, daemons, firmware download
- [software/userland/userland-abi-and-loader.md](software/userland/userland-abi-and-loader.md): Userland ABI and the dynamic loader
- [software/userland/userland-libraries-and-isa.md](software/userland/userland-libraries-and-isa.md): Userland libraries and the instruction-set level

### On-disk formats (formats/)

- [formats/efs/README.md](formats/efs/README.md): EFS on-disk format: superblock, bitmaps, inodes and extents
- [formats/volhdr/README.md](formats/volhdr/README.md): SGI disk volume header (dvh) on-disk format
- [formats/xfs/README.md](formats/xfs/README.md): XFS on-disk format as written by IRIX 6.5.30
- [formats/xlv/README.md](formats/xlv/README.md): XLV on-disk label format, recovered from the 6.5.30 binaries
- [formats/xvm/README.md](formats/xvm/README.md): XVM volume label and metadata, recovered from the binaries

### Not indexed one by one

- Generated, not indexed one by one: the 46 layout pages in `software/kernel/structs/` (each says it is generated by `scripts/struct_fields.py`) and `software/kernel/panics.md`.
- Instruction-reading notes `evidence/instruction-readings-kudzu-g1.md` and `evidence/instruction-readings-kudzu-g2.md`, not indexed.

## Hardware definitions

The hardware structures this repository specifies are described once, in TOML: `formats/<name>/format.toml`. Each unit file gives its constants, registers and fields with a grade (established, inferred or unknown) and a source. `generated/` holds what is generated from them: C headers, a Rust crate, Python decode helpers, flat JSON and a genxml export, and the reference page of each unit is `format.md` beside its source. Do not edit `generated/` or `format.md`; edit the TOML and run `retools/tools/defs/defs-gen`. Register maps of the hardware IRIX drives (hub, PCI bridge, one-wire parts, R10000 timer, O2 CRIME, GBE and MACE video) are in the chips and crime repositories; what the kernel does with them is in [software/kernel/hardware-register-use.md](software/kernel/hardware-register-use.md).

## Running the tools

After cloning, run `git submodule update --init retools`: the container definitions and shared scripts are in it, under `retools/tools/` (shared) and `retools/tools/kudzu/` (this repository's). Run them from the repository root by that path. The other submodules are public reference trees and are large; initialise them only when you want them ([tools/README.md](tools/README.md) describes `tools/setup-clone.sh`, which links `root/` and fetches the public-source submodules).

The host needs Podman, git and python3. Everything else runs in a container; [tools/README.md](tools/README.md) has the commands:

    retools/tools/re-run python3 retools/tools/inventory.py             # binaries, functions, ioctls, scope
    retools/tools/re-run python3 retools/tools/kudzu/os_tables.py     # system calls, structures, panic sites, formats
    tools/check-os-tables                                     # fail if the committed tables are stale

The generated tables are in [evidence/inventory/](evidence/inventory/README.md). `git status --short evidence` shows nothing after a run when your `root/` matches `tools/config/inventory-root.sha256`.

Licence: BSD-3-Clause, see [LICENSE](LICENSE). The scripts descend from gr4's and keep its notice in [retools/tools/LICENSE-gr4](retools/tools/LICENSE-gr4).
