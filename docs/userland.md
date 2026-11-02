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

## NFS, RPC and NIS services

### `6.5.30/usr/etc/nfsd`

Package `nfs.sw.nfs`, product `nfs_6530m`. Daemon. N32 ELF32, 69304 bytes. Established.

- Usage is `usage: %s [-a] [-p protocol] [-c #conn] [nservers]`. Established.
- Imports `nfssvc`, the kernel's NFS service call, and `syssgi`, `sysmp` and `tsix_on`. It also imports the socket calls `socket`, `bind`, `setsockopt` and `getsockname`, and the RPC port-map calls `pmap_set` and `pmap_unset2`, and `chroot`. Established.
- Role: starts the NFS server threads, which then run in the kernel through `nfssvc`, and registers the NFS RPC programs. Inferred from the imports and usage line.

### `6.5.30/usr/etc/nfsdctl`

Package `nfs.sw.nfs`, product `nfs_6530m`. Command. N32 ELF32, 67572 bytes. Established.

- Usage is `usage: %s [-e|-d] [programs...]` and `%s -t <address>`. Its messages read `cannot %sactivate NFS program %s`, `%s is unknown RPC program` and `cannot disconnect clients at %s`. Established.
- Imports `syssgi`, `getrpcbyname` and `strtoul`. Established.
- Role: enables or disables the NFS RPC programs in the kernel, and disconnects the clients of one address. Inferred from the usage and messages; the `syssgi` selector used for each action was not read.

### `6.5.30/usr/etc/exportfs`

Package `nfs.sw.nfs`, product `nfs_6530m`. Command. N32 ELF32, 73192 bytes. Established.

- Usage is `usage: %s [-aiuvtfR] [-o options] [dir] ...`. Established.
- Its strings name `/etc/exports`, `/etc/xtab` and `/etc/nfssec.conf`, and the message `Removing %s from /etc/xtab`. Established.
- Imports the export-table calls `getexportent`, `addexportent`, `remexportent`, `getexportopt` and the call `exportfs`, plus `syssgi` and the netgroup calls `innetgr` and `getnetgrent`. Established.
- Role: adds, removes and lists the exported directories, keeping the active-export file in step with `/etc/exports`. Inferred from the file names and the export-table calls.

### `6.5.30/usr/etc/automount`

Package `nfs.sw.nfs`, product `nfs_6530m`. Daemon. N32 ELF32, 81780 bytes. Established.

- Usage is `Usage: automount`. Its strings name `/tmp_mnt` and `/etc/mtab`. Established.
- Imports the RPC client calls `callrpc`, `clntudp_create` and `authunix_create_default`, the export-option call `getexportopt`, the mount-table calls `addmntent` and `endmntent`, and `fork`. Established.
- Role: an NFS automounter. It mounts remote directories when they are first used, under `/tmp_mnt`, and records them in the mount table. Inferred from the strings and the RPC and mount calls.

### `6.5.30/usr/etc/rpc.mountd`

Package `nfs.sw.nfs`, product `nfs_6530m`. Daemon. N32 ELF32, 74092 bytes. Established.

- Usage is `usage: rpc.mountd [-l lifetime] [-m maxthreads] [-nr]`. Established.
- Its strings name `/etc/xtab`, `/etc/rmtab` and `/etc/rpcports`, and the messages `Out of memory while processing /etc/rmtab` and `cannot allocate %s port from the range for %d in /etc/rpcports`. Established.
- Imports `getfh`, `getexportent`, `getexportopt`, `innetgr`, `syssgi`, `ioctl` and `cap_acquire`. Established.
- Role: the NFS mount-protocol service. It answers mount requests from the export table, records the mounted clients in `/etc/rmtab`, and limits its worker threads by its maxthreads option. Inferred from the usage line and the file names.

### `6.5.30/usr/etc/rpc.statd`

Package `nfs.sw.nfs`, product `nfs_6530m`. Daemon. N32 ELF32, 39564 bytes. Established.

- Its state directory is `/var/statmon`. It imports `pmap_unset`, `rpc_control`, `opendir`, `readdir`, `rename`, `mkdir` and `gethostbyname`, and `syssgi`. Established.
- Role: the lock-status monitor for NFS. It records the hosts that hold locks in `/var/statmon` and notifies them after a restart. Inferred from the directory name and the RPC calls.

### `6.5.30/usr/etc/rpc.lockd`

Package `nfs.sw.nfs`, product `nfs_6530m`. Daemon. N32 ELF32, 18924 bytes. Established.

- Usage is `usage: rpc.lockd -l -g [grace_period] [servers]`. Established.
- Imports `nfssvc`, `syssgi`, `tsix_on`, `chroot`, `pmap_set`, `pmap_unset2` and the socket calls. Its strings include the device `/dev/tty`. Established.
- Role: the NFS lock manager. It serves the lock calls of NFS clients through the kernel's `nfssvc` path, with a grace period on start-up. Inferred from the usage line and the imports.

### `6.5.30/usr/etc/rpc.bootparamd`

Package `nfs.sw.nfs`, product `nfs_6530m`. Daemon. N32 ELF32, 20440 bytes. Established.

- Usage is `usage: %s [-d]`. Its strings name `/etc/bootparams` and the error `getfilekey: open /etc/bootparams failed`. Established.
- Imports the bootparams XDR routines `xdr_bp_whoami_arg`, `xdr_bp_getfile_arg`, `svc_register`, `svcudp_create` and `svc_run`. Established.
- Role: the boot-parameter service. It answers clients that ask for their own name and for a file path, from `/etc/bootparams`. Inferred from the strings and the XDR routine names.

### `6.5.30/usr/etc/rpc.espd`

Package `eoe.sw.base`, product `eoe_6530m`. Daemon. N32 ELF32, 19212 bytes. Established.

- Its `NEEDED` libraries are `libssdb.so`, `libeventmon.so`, `libsgmtask.so`, `libemgrapi.so` and `libc.so.1`. Established.
- It imports the SGM session and frame routines `sgm_init_session`, `sgm_execute_request`, `sgm_check_frame_auth`, `sgm_check_session_auth`, `sgm_calc_hmac_auth`, `sgm_make_reply_frame`, `sgm_make_error_frame`, `sgm_write_session_data`, `sgm_load_default_conf`, `sgm_open_ssdb` and `sgm_close_ssdb`, the security-database calls `ssdbInit` and `ssdbDone`, and the logging calls `InitDbLog` and `DbLog`. It also imports `xdr_sgmframe` and the pthread calls. Established.
- Its messages include `Error initializing RPC Service for SGM`. Established.
- Role: a daemon that runs authenticated request sessions for the SGM product. It checks each request frame's authentication, runs the request, and sends a reply or an error frame. Inferred from the routine names and the library list. The letters in `espd` are not expanded in the file.

### `6.5.30/usr/etc/rpc.passwd`

Package `nfs.sw.nis`, product `nfs_6530m`. Daemon. N32 ELF32, 72588 bytes. Established.

- Its strings name `/etc/passwd`, `/etc/shadow`, `/etc/shells`, `/etc/.pwd.lock`, the shells `/bin/sh`, `/bin/csh`, `/bin/ksh` and their `/sbin` copies, and the advice `It should match the setting of PWDIR in /etc/config/ypmaster.options`. Established.
- Imports `fgetpwent`, `fgetspent`, `getspnam`, `putpwent`, `putspent`, `crypt`, `execvp`, `fork` and `svc_register`. Established.
- Role: the NIS password-change service. It checks a password change from a client and rewrites the password and shadow records, with a lock file. Inferred from the file names and the calls.

### `6.5.30/usr/etc/rpc.rexd`

Package `nfs.sw.nfs`, product `nfs_6530m`. Daemon. N32 ELF32, 38476 bytes. Established.

- Its `NEEDED` library is `librpcsvc.so`, with `libc.so.1`. It imports `_getpty`, `execvp`, `fork`, `mount`, `mkdir`, `mktemp`, `chmod`, `chown`, `addmntent`, `getmntent`, `pututline` and the RPC client calls `callrpc` and `clntudp_create`. Established.
- Its strings name `/var/tmp_rex/rexdXXXXXX`, `/etc/utmp`, `/etc/wtmp`, `/usr/adm/wtmp` and `/etc/mtab`, and the temporary mount-table names `/etc/nmtabXXXXXX` and `/etc/omtabXXXXXX`. Established.
- Role: the remote-execution service. It runs a command that a client sends, on a pseudo-terminal, under a temporary directory, and records the session in the utmp and wtmp files. Inferred from the strings and imports.

### `6.5.30/usr/etc/rpc.rquotad`

Package `nfs.sw.nis`, product `nfs_6530m`. Daemon. N32 ELF32, 68124 bytes. Established.

- Imports `quotactl`, the mount-table calls `getmntent`, `setmntent`, `endmntent` and `hasmntopt`, `gethostbyaddr`, `inet_ntoa`, and the RPC service calls `svc_register`, `svcudp_create` and `svc_sendreply`, with the quota XDR routines `xdr_getquota_args` and `xdr_getquota_rslt`. Established.
- Its strings name `/etc/mtab`. Established.
- Role: the remote quota service. It answers a client's quota request for a filesystem with the `quotactl` call. Inferred from the imports.

### `6.5.30/usr/etc/rpc.rstatd`

Package `eoe.sw.base`, product `eoe_6530m`. Daemon. This copy is an N64 ELF64 file, 77400 bytes, flags `mips3` without the N32 `abi2` bit. Established.

- The media index lists a second 6.5.30 copy of this path (134368 bytes, same product, same disc). It was not read. This entry is for the 77400-byte copy in `root/`.
- Its `NEEDED` libraries are `librpcsvc.so`, `libdisk.so` and `libc.so.1`. It imports `sysmp`, `sgi_disk_setup`, `dkiotime_get`, the inventory walk `getinvent`, `setinvent` and `endinvent`, `recvfrom`, `svcudp_bufcreate`, and the RPC calls `svc_register`, `svc_run` and `svc_sendreply`, with the statistics XDR routines `xdr_stats`, `xdr_statsswtch` and `xdr_stat`. Established.
- Its strings name `/dev/kmem` and `/unix`, and the messages `/unix: bad namelist: %m`, `minfo: sysmp: %m` and `sysinfo: sysmp: %m`. Established.
- Role: the performance-statistics service. It reads the kernel's counters through `/dev/kmem`, the namelist of `/unix`, and `sysmp`, and returns CPU, disk and paging figures to rstat clients. Inferred from the imports and strings.

### `6.5.30/usr/etc/rpc.rusersd`

Package `nfs.sw.nis`, product `nfs_6530m`. Daemon. N32 ELF32, 67804 bytes. Established.

- Imports the utmp calls `getutent`, `getutxid`, `setutent`, `setutxent`, the XDR routines `xdr_utmparr` and `xdr_utmpidlearr`, and `getsockname`, `svcudp_create`, `svc_register`, `svc_run` and `rpc_control`. Established.
- Role: the logged-in-users service. It returns the utmp records to remote `rusers` clients. Inferred from the imports.

### `6.5.30/usr/etc/rpc.rwalld`

Package `nfs.sw.nis`, product `nfs_6530m`. Daemon. N32 ELF32, 67476 bytes. Established.

- Its strings name `/sbin/wall`, which it starts with `popen` and `pclose`. Established.
- Imports `svc_register`, `svc_run`, `svc_sendreply`, `svcudp_create`, `svcerr_decode` and `xdr_wrapstring`. Established.
- Role: the remote-wall service. It takes a broadcast message from a client and passes it to the local `wall` command. Inferred from the strings and imports.

### `6.5.30/usr/etc/rpc.sprayd`

Package `nfs.sw.nis`, product `nfs_6530m`. Daemon. N32 ELF32, 67476 bytes. Established.

- Imports `xdr_spraycumul`, `BSDgettimeofday`, `svc_register`, `svc_run`, `svc_sendreply`, `svcudp_create` and `rpc_control`. Established.
- Role: the spray service, which counts the packets that a spray client sends to it and returns the count. Inferred from the XDR routine name and the RPC calls.

### `6.5.30/usr/etc/rpc.ypupdated`

Package `nfs.sw.nis`, product `nfs_6530m`. Daemon. N32 ELF32, 67836 bytes. Established.

- Its strings name `/var/yp` and `/dev/null`. It imports `_openchild`, `fork`, `wait3`, `chdir`, `host2netname`, `user2netname`, `gethostbyaddr`, the XDR routine `xdr_ypupdate_args`, `svcerr_weakauth`, and the RPC calls `svctcp_create`, `svcudp_create` and `svc_register`. Established.
- Role: the NIS map-update service. It takes an update request from a client, checks its authentication, and runs the update in the `/var/yp` directory in a child process. Inferred from the imports and strings.

### `6.5.30/usr/etc/cfsadmin`

Package `nfs.sw.cachefs`, product `nfs_6530m`. Command. N32 ELF32, 39928 bytes. Established.

- Usage is `usage: cfsadmin -[cu] [-o parameter-list] cachedir`, and it refuses `-o` without `-c` or `-u`. Established.
- Imports `nftw`, `mkdir`, `rmdir`, `rename`, `getmntany`, `setmntent`, `syssgi` and `fsync`. Its strings name `/etc/mtab`. Established.
- Role: creates (`-c`) or updates (`-u`) a CacheFS cache directory and its parameters. Inferred from the usage line and the file calls. The `syssgi` selector it sends was not read.

### `6.5.30/usr/etc/cfsstat`

Package `nfs.sw.cachefs`, product `nfs_6530m`. Command. N32 ELF32, 37648 bytes. Established.

- Imports the curses calls `initscr32`, `printw`, `wgetch`, `keypad`, `raw`, `noecho` and `endwin`, and `sysmp`, `ioctl`, `kill`, `cftime` and `getuid`. It needs `libcurses.so`. Established.
- Role: a full-screen display of CacheFS statistics that refreshes until it is stopped. Inferred from the name and the curses calls. The `sysmp` and `ioctl` requests were not read.

## Cluster services

These programs belong to the cluster products, which hold a cluster database (`cdb`) that the libraries read and write. The cluster database default is `/var/cluster/cdb/cdb.db`, named in the `cms_status` usage text.

### `6.5.30/usr/cluster/bin/cmond`

Package `cluster_admin.sw.base`, from the Overlays (3 of 3) disc. Daemon. N32 ELF32, 219644 bytes. Established.

- Usage is `Usage: %s [-s] [-d] [-L log_level]`. Its `NEEDED` libraries are `libpthread.so`, `libcmon.so`, `libcdb.so`, `libgen.so` and `libc.so.1`. Established.
- Its strings name `/etc/config/cmond.options`, `/etc/pcp.conf`, the socket `/tmp/.cmond_cdb.uds`, and the command `/sbin/chkconfig -f %s %s`. It imports the `cdb_*` handle, node and value calls, and `cap_acquire`. Established.
- Role: the cluster monitor daemon on each node. It reads its options file, keeps a connection to the cluster database, and runs the configuration checks through `chkconfig`. Inferred from the library names and strings.

### `6.5.30/usr/cluster/bin/cmon_ctrl`

Package `cluster_admin.sw.base`, from the Overlays (3 of 3) disc. Command. N32 ELF32, 30396 bytes. Established.

- Usage is `Usage: %s action [-f] [-r] [-g process_group] [-l level]`. Its `NEEDED` libraries are `libcmon.so`, `libgen.so` and `libc.so.1`. Established.
- Imports `cmon_request`, `cmon_actions_stoc` and `cmon_err_ctos`, and the thread calls. Established.
- Role: the command-line control for the cluster monitor daemon, which sends it one action and a log level. Inferred from the usage line and the library calls.

### `6.5.30/usr/cluster/bin/cms_status`

Package `cluster_services.sw.base`, from the Overlays (3 of 3) disc. Command. N32 ELF32, 28444 bytes. Established.

- Usage is `Usage: %s [-h] [-l] [-f dbfile] [-c cluster] [-m machine]`, and its help text names the default database `/var/cluster/cdb/cdb.db`. Established.
- Its `NEEDED` libraries are `libhacms.so`, `libgen.so`, `libci.so`, `libcdb.so`, `libpthread.so` and `libc.so.1`. It imports the `cicdb_*` calls for the cluster, machine and membership status. Established.
- Role: prints the cluster membership status from the database, for a cluster or a machine. Inferred from the usage line and the calls.

### `6.5.30/usr/cluster/bin/cms_failconf`

Package `cluster_control.sw.base`, from the Overlays (3 of 3) disc. Command. N32 ELF32, 35164 bytes. Established.

- Usage is `cms_failconf [options ...]`, and `-h` prints the usage. Established.
- Its `NEEDED` libraries are `libcrf.so`, `libcdb.so`, `libci.so`, `libpthread.so`, `libgen.so` and `libc.so.1`. It imports `crf_get_fail_actions`, `crf_set_fail_actions`, `crf_set_fail_actions_old`, `crf_action_str`, `crf_cellid_to_node`, `cicdb_open` and `cicdb_close`, and `cdb_lock_node` and `cdb_unlock_node`. Established.
- Role: reads and sets the fail actions that the cluster runs when a node fails, through the cluster database. Inferred from the routine names.

### `6.5.30/usr/cluster/bin/crsd`

Package `cluster_control.sw.base`, from the Overlays (3 of 3) disc. Daemon. N32 ELF32, 763160 bytes. Established.

- Usage is `Usage: %s [-c cdb-file] [-l] [-n node_name] [-s log_level]`. Its `NEEDED` libraries are `libcrs.so`, `libcdb.so`, `libci.so`, `libpthread.so`, `libgen.so`, `libcmon.so` and `libc.so.1`. Established.
- It imports twelve `__ci_nlist_*` list calls (create, add, next, search, delete, destroy and others), `bind`, `calloc` and `cap_acquire`. Its strings include the error names `CI_CRFERR_NODEV` and `CI_GCSLIBERR_BADID`. Established.
- Role: the cluster resource service. It keeps lists of cluster resources and answers resource requests for the local node. Inferred from the library name and the list calls.

### `6.5.30/usr/cluster/bin/ha_cmsd`

Package `cluster_services.sw.base`, from the Overlays (3 of 3) disc. Daemon. N32 ELF32, 1219980 bytes, the largest program in this set. Established.

- Usage is `Usage: %s [options]`. Its `NEEDED` libraries are `libhacms.so`, `libcrs.so`, `libci.so`, `libpthread.so`, `libcdb.so`, `libgen.so`, `libcmon.so` and `libc.so.1`. Established.
- It imports thirteen symbols of the membership library, the timing and path settings `__cms_node_timeout`, `__cms_reset_timeout`, `__cms_linger_timeout`, `__cms_new_membership_timeout`, `__cms_waitforall_timeout`, `__cms_init_timeout`, `__cms_send_timeperiod`, `__cms_tie_wait_period`, `__cms_dir`, `__cms_ipc_file`, `__cms_incarnation_file`, `__cms_experimental` and `__cmsd_service_name`. Established.
- Its strings include the administration command structure name `cmsd_admin_cmd_t` and the error name `CI_CMSERR_NOCLIENT`. Established.
- Role: the cluster membership daemon. It forms and tracks the membership of the nodes, using the timeouts above. Inferred from the library and tunable names.

### `6.5.30/usr/cluster/bin/ha_gcd`

Package `cluster_services.sw.base`, from the Overlays (3 of 3) disc. Daemon. N32 ELF32, 702432 bytes. Established.

- Its `NEEDED` libraries are `libhacms.so`, `libci.so`, `libpthread.so`, `libcdb.so`, `libgen.so`, `libcmon.so` and `libc.so.1`. It imports `cdb_get_value_at_key`, `cdb_set_value_at_key`, `cdb_notify_register` and the lock calls. Established.
- Its strings include the group names `gcd_groups`, `new_group`, `group_membership`, `send_msg_to_all_clients_in_group` and the messages `GCSMSG_DOWN_MEMBERSHIP` and `GCSCHG_NODE_FAILURE`. Established.
- Role: the group-communication daemon. It keeps the membership of each named group and sends membership-change messages to the group's clients. Inferred from the group names and messages.

## XFS tools

### `6.5.30/usr/sbin/xfs_db`

Package `eoe.sw.base`, product `eoe_6530m`. Command. N32 ELF32, 217348 bytes. Established.

- Its `NEEDED` libraries are `libdisk.so` and `libc.so.1`. It imports `findrawpath`, `findblockpath` and `findsize` from the disk library, `ioctl`, `fstat64`, `lseek64` and `ftruncate64`. Established.
- Its strings include the device path format `/hw/major/%d/minor/%d` and three simulation-file templates `/tmp/xfs_simdXXXXXX`, `/tmp/xfs_simlXXXXXX` and `/tmp/xfs_simrXXXXXX`. Established.
- Role: an interactive debugger for the on-disk structures of an XFS filesystem. The templates suggest it can also work on a simulated image of the filesystem. Inferred from the strings and imports.

### `6.5.30/usr/sbin/xfs_repair`

Package `eoe.sw.base`, product `eoe_6530m`. Command. N32 ELF32, 639436 bytes. Established.

- Usage is `usage: xfs_repair [-n] [-o subopt[=value]] fs-device`. Its `NEEDED` libraries are `libdisk.so` and `libc.so.1`. Established.
- It imports `findrawpath`, `findblockpath`, `findsize`, `acl_valid` and `mac_valid`, and its strings include the same `/hw/major/%d/minor/%d` path format and simulation templates as `xfs_db`. Established.
- Role: checks an XFS filesystem and repairs it. The usage line lists `-n` first. Reading it as report-only, without repairs, is inferred from the option's letter and was not checked against the code.

## Generated table

`docs/inventory/userland.tsv` has one row per binary listed in [scripts/userland-roles.json](../scripts/userland-roles.json), generated by `scripts/userland_tables.py`. The ELF facts in each row (class, ABI, interpreter, section count, undefined dynamic symbols, size and hash) are read from the file under `root/`; the package, type, interfaces, role and citation are the hand-written fields of the roles file.

Thirty-one of the install's 1,150 distinct ELF files under `sbin`, `usr/sbin`, `usr/etc` and `usr/bin` are in the table: the hardware inventory and controller commands, the NFS and RPC daemons, the cluster commands and daemons, `cfsadmin`, `sysmon` and `xfs_repair`. Each row is `located` with an `inferred` grade. No row is `understood`.

Limits the table adds to the entries above:

- The `usr/etc/rpc.rstatd` row is the 64-bit build. The 32-bit build at the same path under a mode suffix is not classified.
- `usr/etc/rpc.espd` has grade `unknown`: the binary does not say what its sessions are for.

Regenerate with `tools/re-run python3 scripts/userland_tables.py`. A file missing from `root/6.5.30/` stops the run and names the path.
