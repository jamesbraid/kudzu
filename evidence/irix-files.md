# IRIX files needed for this repository

This lists the proprietary SGI files a reader copies from their own IRIX media or install into a local `root/` directory. They are not tracked in source control. Paths only: this repository holds no file contents.

`root/` holds one directory per release, `root/6.5.30/` and `root/6.5.22/`, each mirroring the IRIX filesystem plus the linked kernels as `unix.IPnn`.

## 1. IRIX 6.5.30 (primary)

Present in the working `root/`:

- `usr/cpu/sysgen/IP27boot/`, `IP30boot/`, `IP32boot/`, `IP35boot/` and `IP22boot/`: the kernel object archives and objects, from the three-disc **IRIX 6.5.30 Installation Tools and Overlays (1 of 3)**, **Overlays (2 of 3)** and **Overlays (3 of 3)** media (files merged, no name appears on two discs). 292 files for IP27, 274 for IP30, 230 for IP32, 353 for IP35 and one for IP22.
- `unix.IP27`: the installed 6.5.30m kernel of an Onyx2/Origin 2000 class system (IP27), from a completed installation.
- `unix.IP30`: the installed 6.5.30m kernel of an Octane (IP30).
- `unix.IP35`: the installed 6.5.30m kernel of an Origin 300/350 class system (IP35).

Not present, wanted next:

- `unix.IP32` (O2): no installed O2 kernel is held. The IP32 objects above are present, so the object inventory covers IP32 without the linked image.
- `var/sysgen/` (`master.d`, `mtune`, `system`, `stune`, `sassign`): the kernel configuration files, from the same media.
- The rest of the base libraries and applications (`lib32`, `lib64`, `usr/bin`, `usr/sbin`, `sbin`) for the later, wider scope. Only the programs and libraries that [questions/README.md](../questions/README.md) names are held, listed next.

Userland held in `root/6.5.30/`, copied from the extracted 6.5.30 overlay set (five discs) at the same install paths (products `eoe`, `x_eoe`, `dmedia_eoe`, `nfs` and `compiler_eoe`, from the media's file index). The whole userland is not held, to keep the inventory small; each file's hash is in `tools/config/inventory-root.sha256`.

- Loader and C library: `6.5.30/lib/rld`, `6.5.30/lib32/rld` and `6.5.30/lib64/rld`; `libc.so.1` as `6.5.30/lib/libc.so.1` (o32), `6.5.30/lib32/libc.so.1`, `6.5.30/usr/lib32/mips3/libc.so.1` and `6.5.30/usr/lib32/mips4/libc.so.1` (n32, MIPS III and MIPS IV builds), and `6.5.30/usr/lib64/mips3/libc.so.1` and `6.5.30/usr/lib64/mips4/libc.so.1` (64-bit); `libm.so` in the same builds; `libpthread.so` (three ABIs), `6.5.30/usr/lib/libexc.so`, `6.5.30/usr/lib32/libCsup.so` and `6.5.30/usr/lib32/libC.so.2`. The media installs `6.5.30/lib32/libc.so.1` under a CPU-architecture selector as well (`libc.so.1.CPUARCH-R8000-CPUARCH-R10000-CPUARCH-R5000`); that copy has the content of the MIPS IV build and is not held separately.
- X and audio: `6.5.30/usr/gfx/arch/GENERIC/Xsgi` and `6.5.30/usr/gfx/arch/IP22NG1/Xsgi` (the only two server builds on the media), `libX11.so.1` and `libXi.so` in the three ABIs, `libaudio.so.1` in the three ABIs and `6.5.30/usr/sbin/audiostate`.
- Programs and libraries behind the optional-subsystem questions: `6.5.30/usr/sbin/cpr`, `6.5.30/usr/sbin/cview` and `libcpr.so` (three ABIs); `6.5.30/usr/etc/ipfilterd`; `6.5.30/usr/sbin/cpuset`, `6.5.30/usr/sbin/runon`, `6.5.30/usr/sbin/dplace`, `6.5.30/usr/sbin/dprof`, `6.5.30/usr/etc/miser`, the six `usr/sbin/miser_*` commands, `6.5.30/usr/bin/jlimit`, `libcpuset.so` (n32 and 64-bit), `6.5.30/usr/lib32/mips4/libdplace.so` and `6.5.30/usr/lib32/mips4/libdprof.so`; `6.5.30/usr/bin/perfex` and `libperfex.so` (three ABIs); `6.5.30/usr/etc/autofs`, `6.5.30/usr/etc/autofsd` and `6.5.30/usr/etc/mount_autofs`; `6.5.30/usr/lib/devicelib/dev_cdrom.so` and `6.5.30/usr/sbin/scsicontrol`; `6.5.30/sbin/xvm`, `6.5.30/sbin/mkfs_xfs` and `6.5.30/usr/sbin/xfs_bmap`, `6.5.30/usr/sbin/xfs_freeze`, `6.5.30/usr/sbin/xfs_growfs`, `xfs_io` and `xfs_quota`.
- The 6.5.22 userland is held as well, listed in section 2.

```files
/unix
/usr/cpu/sysgen/IP22boot
/usr/cpu/sysgen/IP27boot
/usr/cpu/sysgen/IP30boot
/usr/cpu/sysgen/IP32boot
/usr/cpu/sysgen/IP35boot
/var/sysgen
```

The kernel on a running system is built from the object files at install time, so `/unix` differs between systems of one platform in the modules it links; the three `unix.*` files here are the ones this repository's tables were measured on (see `tools/config/inventory-root.sha256`).

## 2. IRIX 6.5.22

Present in the working `root/`:

- `usr/cpu/sysgen/IP22boot/`, `IP27boot/`, `IP30boot/`, `IP32boot/` and `IP35boot/`: the kernel object archives and objects, from the three-disc **IRIX 6.5.22 Installation Tools and Overlays (1 of 3)**, **Overlays (2 of 3)** and **Overlays (3 of 3)** media (files merged, no name appears on two discs). 210 files for IP22, 288 for IP27, 266 for IP30, 222 for IP32 and 346 for IP35. Each directory holds a few names that the media installs as links to another member (`socket.a` to `bsd.a`, and for IP35 `cl.o`, `if_cl.o`, `partition.o`, `pts.a` and `xpc.o`); they are kept as links.

Disc image SHA-256 (the files these were read from):

```
c36d43431569902a9bdf868cb9f98ea44519a9a99e2bb68dbf1be76be0eec299  IRIX 6.5.22 Installation Tools and Overlays (1 of 3).iso
9064b95ef1c4589c9799a58bbcd7b9f99577d6516555ddc8ed02b703655b58cc  IRIX 6.5.22 Overlays (2 of 3).iso
d2899e9aac3f6f62a446d881ccc2f214afc181c195272ab0e658ca974528557f  IRIX 6.5.22 Overlays (3 of 3).iso
```

The hash of every extracted file is in `tools/config/inventory-root.sha256` under `6.5.22/`.

Also present:

- `unix.IP27`, `unix.IP30` and `unix.IP35`: the linked 6.5.22 kernels, in `root/6.5.22/`. This repository does not record which installed system they came from. Their SHA-256 hashes are in `tools/config/inventory-root.sha256`, and [evidence/version-diffs.md](version-diffs.md) compares them with the 6.5.30 kernels.
- `var/sysgen.IP27`, `var/sysgen.IP30` and `var/sysgen.IP35`: kernel configuration files (`master.d`, `mtune`, `system`, `stune`, `ioconfig`), with no object code.

Userland held in `root/6.5.22/`: the same set as `root/6.5.30/`, at the same install paths. It covers the loader and C library, the two Xsgi builds with libX11, libXi and libaudio, the programs and libraries behind the optional-subsystem questions (checkpoint and restart, cpusets and miser, performance counters, the IP filter daemon, the autofs tools, the CD-ROM and SCSI control tools, `xvm`, `mkfs_xfs`), plus `6.5.22/usr/etc/automount`, `6.5.22/usr/sbin/xfs_db` and `6.5.22/usr/sbin/xfs_repair`. The files were read from the 6.5.22 overlay discs above: 56 from overlays 1 and 15 from overlays 2. The 6.5 foundation, NFS and development discs that the 6.5.22 install recipe names (`foundation1`, `foundation2`, `nfs`, `devlibs`, `devfoundation`) carry older copies of most of these paths; the overlays update them, so the overlay copy is held. Each file's size matches its installation record, and its hash is in `tools/config/inventory-root.sha256` under `6.5.22/`.

- Where one path has two CPU-specific builds on the media, the first build in the disc's order is held. For `6.5.22/lib32/libc.so.1` that is the copy that also sits at `6.5.22/usr/lib32/mips3/libc.so.1`, as in 6.5.30. The other build is the one at `6.5.22/usr/lib32/mips4/libc.so.1`. The second builds of `6.5.22/usr/sbin/cpr` (32-bit; the held build is 64-bit, as in 6.5.30), `6.5.22/usr/lib32/mips4/libm.so` and `6.5.22/usr/lib64/mips4/libm.so` are not held.
- Not held: `usr/sbin/xfs_io` and `usr/sbin/xfs_quota`. Neither is on the 6.5.22 discs above or on the 6.5 installation tools disc. The 6.5.30 root holds both.

Not present, wanted next:

- `unix.IP32` (O2): no installed 6.5.22 O2 kernel is held.
- The rest of the base libraries and applications, as for 6.5.30. Only the userland listed above is held.

The 6.5.22 patch sets for 32-bit and 64-bit systems are patches, not a base install, and are not in `root/`.

## Links and archives

```archives
```

```links
```
