# IRIX 6.5.30 base user-space programs

One entry per shipped program: the installed path, the package it installs from, what kind of program it is, the kernel and device interfaces its imports and strings name, and a one-line role. The programs are the 6.5.30 system daemons and commands under `sbin/`, `usr/etc/`, `usr/sbin/` and `usr/cluster/bin/`. They are listed in the inventory as the `userland-base` group, out of scope for now (see [scope.tsv](inventory/scope.tsv)).

## How the entries were made

- The files are the installed 6.5.30 copies under `root/6.5.30/`. Each one was checked byte for byte against the copy on the 6.5.30 installation discs named in [IRIX_FILES.md](../IRIX_FILES.md), so the paths below are the install paths.
- The package is the subsystem and product that the disc's own installation records (the `.idb` manifests) assign to the file. A subsystem such as `eoe.sw.base` is a package inside a product such as `eoe_6530m`.
- Imports are the undefined dynamic symbols (`readelf --dyn-syms`); the `NEEDED` list is the dynamic section. Strings are read from the file and are quoted only where they name a path, a device, a message or a usage line.
- Grades. **Established**: read in the file (an import, a string, a usage line, a header field). **Inferred**: a role reached from the name and those facts, with the reasoning given. **Unknown**: a question no shipped file in hand answers.
- Not done in this pass: the `syssgi` selector numbers and the ioctl request numbers each program sends. Those need the call sites disassembled, and the entries say so where it matters.
- The shipped files carry build-machine path strings. They are not recorded here.
- ELF: every program below is a big-endian MIPS executable. None has a `.mdebug` section (checked with `readelf -S`), so no source-level debug records are present.

## Hardware inventory and controller tools

### `6.5.30/sbin/hinv`

Package `eoe.sw.base`, product `eoe_6530m`. Command. N32 ELF32, 77064 bytes, `sha256` begins `4f430d6c4da73656`. Established.

- Imports `syssgi` and `sysmp`, the inventory walk `scaninvent`, the device-name lookups `dev_to_devname` and `filename_to_devname`, and the tree walk `nftw`. Established.
- Its usage line is `usage: %s {-v -m -s -c class -t type -d dev -u unit -a file}`, and its strings include the device paths `/hw/net/%s%d`, `/hw/gm%d` and `/pci`. Established.
- Role: prints the machine's hardware inventory, from the kernel's inventory records and the device nodes under `/hw`. Inferred from the imports and usage line.
- Unknown: which `syssgi` selectors it sends, and which routine prints each line.

### `6.5.30/sbin/ioconfig`

Package `eoe.sw.base`, product `eoe_6530m`. Command. N32 ELF32, 41768 bytes. Established.

- Reads the configuration file `/etc/ioconfig.conf` and the directory `/var/sysgen/ioconfig`, and names the file `/etc/ioperms`. Established from the strings.
- Its error text gives the line format `<class> <type> <state> <case> <suffix> <pattern> <start_num> <ioctl>`, and a message prints `ioctl=0x%x` for each device vertex it processes. Established.
- Imports `ioctl` and `syssgi`, the tree walk `nftw`, `fnmatch` for the name patterns, `filename_to_devname`, `chown`, `chmod`, `getpwnam` and `getgrnam`. Established.
- The strings name the hardware-graph directories `/hw/scsi_ctlr`, `/hw/input*`, `/hw/xvm`, `/hw/.invent` and `/hw/hw`. Established.
- Role: reads one line per device class, and for each matching device vertex sends the configured ioctl and names the device from the suffix, pattern and start number. Inferred from the message formats and the imports.
- Unknown: the request value each class sends, which is in the configuration file and not in the program. The program's own numbering rule is not read.

### `6.5.30/usr/etc/mediad`

Package `eoe.sw.rm_media`, product `eoe_6530m`. Daemon. N32 ELF32, 222192 bytes, dynamic, with `.msym` and `.debug_frame`. Established.

- Its configuration file is `/etc/config/mediad.config`. Its messages include "same device monitored twice in /etc/config/mediad.config" and "same filesystem mounted twice". Established.
- Device nodes named in its strings: `/dev/scsi/sc%ud%ul%u`, `/dev/scsi/%s`, `/dev/pfd/pfd.2dd` and `/dev/pfd/pfd.2hd`. Established.
- Its socket is `/tmp/.mediad_socket`. It loads device modules from `/usr/lib/devicelib` with `dlopen` and `dlsym`. Established.
- Imports `filename_to_scsiname` and `path_to_partnum`, `ioctl`, the mount-table calls (`setmntent`, `getmntent`, `addmntent`, `hasmntopt`), `pmap_set` and `pmap_unset`, and the XDR routines. Established.
- Its strings include `/sbin/fuser`, `/etc/mount` and `/etc/umount`, which it runs. Established.
- Role: a daemon that watches the removable-media devices named in its configuration (CD-ROM and floppy), mounts and unmounts their filesystems, and registers its service with the RPC port mapper. Inferred from the strings and imports.
- Unknown: the device module it loads for a CD-ROM and the commands that module sends. The module was not read here; the CD-ROM question is in [scsi-cdrom-inquiry.md](scsi-cdrom-inquiry.md).

### `6.5.30/usr/etc/eventmond`

Package `eoe.sw.base`, product `eoe_6530m`. Daemon. N32 ELF32, 159128 bytes, with `.msym` and `.debug_frame`. Established.

- Imports the event-manager library `libemgrapi.so` (the `emgr*` routines, `emgrSendEvent`, `emgrReadEvent`, `emgrForwardEvent`), `libpthread.so`, and the System V shared-memory calls `shmget`, `shmat` and `shmctl`. Established.
- Its files are `/var/emgr/subscr.eventmond` and `/var/emgr/tasks.eventmond`, its temporary socket `/tmp/s.eventmond` and its listing `/dev/null`. Established from the strings.
- Imports `socket`, `bind`, `listen`, `accept`, `sendto` and `recvfrom`, and creates pthreads, so it serves network clients. Established.
- Its usage line is `-h -print "help" information and exit`. Established.
- Role: the event-manager daemon. It accepts events and subscriptions, keeps them in its files, and forwards them to subscribers. Inferred from the library names and the file names.
- Unknown: which hardware or kernel sources feed its events. Nothing in the imports reads a device.
- The disc holds no program named `eventmon`. Its start script is `etc/init.d/eventmonitor`, which was not read.

### `6.5.30/usr/sbin/numastatd`

Package `eoe.sw.base`, product `eoe_6530m`. Daemon. N32 ELF32, 19972 bytes. Established.

- Imports `syssgi`, `ioctl`, `schedctl`, `cap_acquire` and `cap_surrender`, and the signal calls. Established.
- Reads per-process records from `/proc/pinfo` and `/proc/pinfo/%s`. Established.
- Its strings hold shell commands that list the router monitor nodes, `/usr/bin/sh -c "/usr/bin/ls -1 /hw/module/*/router/mon"` and two variants (`/hw/module/*/slot/r*/*router/mon`, `/hw/module/*/L1/controller`). It imports `popen` and `pclose`, so it runs them. Established.
- Its usage line is `usage: %s [-w|-n nicevalue] [-i interval]`, and it reports `firmware too old` and `unknown or unsupported architecture`. It refuses a second instance. Established.
- Role: a periodic sampler. It reads the per-process records and the router monitor nodes at an interval it is given and logs them. Inferred from the strings. The name suggests NUMA statistics; the entry does not claim more.
- Unknown: the record fields it stores and the router registers it reads.

### `6.5.30/usr/sbin/sysmon`

Package `sysmon.sw.viewer`, product `sysmon`, from the Applications disc. Command, a Motif window program. N32 ELF32, 256620 bytes. Established.

- Its 6.5.30 package list has a second copy under product `sysmon_6530m` (271024 bytes, `sysmon.sw.viewer`, on the Installation Tools disc). That copy was not read; this entry is for the 256620-byte file held in `root/`.
- Imports the Motif toolkit (`libXm.so.1`, `XmCreate*`, `XmString*`), Xt and Xlib (`libXt`, `libX11.so.1`), and the Xpm and Sgm helpers. Established.
- Its strings name `/usr/lib/sysmon/notifier.config`, and the printing commands `/usr/bin/lp`, `/usr/bin/lpstat` and `/usr/bin/cancel`, and the spool directory `/var/spool/lp`. Established.
- Role: an X and Motif window that shows system error and status notices, with a menu that prints or cancels them. Inferred from the library and file names; the notifier configuration is read by its name only.

### `6.5.30/usr/sbin/l1cmd`

Package `field_diags_sysco.sw.stand`, from the Overlays (3 of 3) disc. Command. N32 ELF32, 251576 bytes. Established.

- Runs `/bin/ls -1 /hw/module/*c*/L1/controller` to find the L1 controller node, and prints `Could not find an L1 Controller (/hw/module/*c*/L1/controller) to communicate with!` when it cannot. Established.
- Imports `socket`, `connect`, `sendto`, `recvfrom` and `select`, and its strings hold the name pattern `/usr/tmp/UNIX_%04x`. Established.
- Its messages name the interface ioctls `SIOCGIFCONF`, `SIOCGIFFLAGS`, `SIOCGIFADDR` and `SIOCGIFDSTADDR` and report their errors. Established from the message text.
- Role: a command-line client for the L1 controller of a module, run from the system. Inferred from the node path and the messages.
- Unknown: the command set it accepts and the packets it sends.

### `6.5.30/usr/sbin/l2cmd`

Not in the inventory. Package `field_diags_sysco.sw.stand`, from the Overlays (3 of 3) disc. Command. N32 ELF32, 446880 bytes. Established.

- Its section header table starts at offset 446168 and has 39 entries of 40 bytes, so it ends at byte 447728, past the end of the 446880-byte file. `readelf -S` and `readelf --dyn-syms` both stop with "Reading 1560 bytes extends past end of file for section headers". This is the copy on the disc, which matches the one in `root/`. Established.
- The inventory parser stops on this file, so `scripts/inventory-config.json` lists it under `exclude`. The rest of the entry comes from the dynamic segment (`readelf -d`, which reads only program headers), which lists `libc.so.1` as the one `NEEDED` library, and from the strings. Established.
- Its strings include the L1 controller node `/bin/ls -1 /hw/module/*c*/L1/controller` and the same interface-ioctl messages as `l1cmd`. Established.
- Role: a command-line client for the L2 controller of a system, the router-side counterpart of `l1cmd`. Inferred from the name and the shared strings.
- Unknown: the import table (the section table is damaged), and therefore which system calls it makes beyond the strings.

### `6.5.30/usr/etc/mmscd`

Package `eoe.sw.base`, product `eoe_6530m`. Daemon. N32 ELF32, 41672 bytes. Established.

- Its usage line is `mmscd [-c cdev] [-f sdev] [-b baud] [-u freq]`, with options for an alternate MMSC console device (`-c`), a non-console device (`-f`), a baud rate (`-b`), a decay factor (`-D`) and `-g`, which shuts the system down gracefully. Established.
- Opens `/dev/console` by default, and its strings name `/hw/machdep/mmsc_control`, `/hw/machdep/ffsc_control`, `/hw/module` and `/hw/nodenum`. Established.
- Imports `sysmp` and `ioctl`, the serial-line calls `__new_tcgetattr` and `__new_tcsetattr`, and `cap_acquire`. Its messages include `load_graph: ncpus: %u ncpus_sysmp: %u nodemode=%u`. Established.
- Role: the daemon that holds the serial link to the MMSC, the module management controller, and passes it commands from the system. Inferred from the device names and the usage text.

### `6.5.30/usr/sbin/flashmmsc`

Package `eoe.sw.base`, product `eoe_6530m`. Command. N32 ELF32, 43824 bytes. Established.

- Usage is `flashmmsc {-a|-m|-p} [-f <filename>] [-v]`, with `-a` for automatic download, `-d` for direct download to the serial port and `-l` for the serial device name. Established.
- Its strings include the firmware path `/usr/cpu/firmware/mmscfw.bin`, the serial device `/dev/ttyd1`, `/dev/console`, `/hw/machdep/mmsc_control`, `/hw/machdep/ffsc_control`, and the start-up script call `/etc/init.d/sn0start stop`. Established.
- Imports `ioctl`, `select`, `read`, `write`, the serial-line calls, `alarm` and `system`. Established.
- Role: downloads the MMSC firmware image to the controller over a serial line, in the mode the options select. Inferred from the usage text and the strings. Its strings include the start-up script with the `stop` argument, and `system` is imported, but the order of that call and the download was not read.
- Unknown: the download protocol and the image format.
