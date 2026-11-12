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
- The rest of the base libraries and applications (`lib32`, `lib64`, `usr/bin`, `usr/sbin`, `sbin`) for the later, wider scope. Only the programs and libraries that [docs/open-questions.md](docs/open-questions.md) names are held, listed next.

Userland held in `root/6.5.30/`, copied from the extracted 6.5.30 overlay set (five discs) at the same install paths (products `eoe`, `x_eoe`, `dmedia_eoe`, `nfs` and `compiler_eoe`, from the media's file index). The whole userland is not held, to keep the inventory small; each file's hash is in `scripts/inventory-root.sha256`.

- Loader and C library: `lib/rld`, `lib32/rld` and `lib64/rld`; `libc.so.1` as `lib/libc.so.1` (o32), `lib32/libc.so.1`, `usr/lib32/mips3/libc.so.1` and `usr/lib32/mips4/libc.so.1` (n32, MIPS III and MIPS IV builds), and `usr/lib64/mips3/libc.so.1` and `usr/lib64/mips4/libc.so.1` (64-bit); `libm.so` in the same builds; `libpthread.so` (three ABIs), `usr/lib/libexc.so`, `usr/lib32/libCsup.so` and `usr/lib32/libC.so.2`. The media installs `lib32/libc.so.1` under a CPU-architecture selector as well (`libc.so.1.CPUARCH-R8000-CPUARCH-R10000-CPUARCH-R5000`); that copy has the content of the MIPS IV build and is not held separately.
- X and audio: `usr/gfx/arch/GENERIC/Xsgi` and `usr/gfx/arch/IP22NG1/Xsgi` (the only two server builds on the media), `libX11.so.1` and `libXi.so` in the three ABIs, `libaudio.so.1` in the three ABIs and `usr/sbin/audiostate`.
- Programs and libraries behind the optional-subsystem questions: `usr/sbin/cpr`, `usr/sbin/cview` and `libcpr.so` (three ABIs); `usr/etc/ipfilterd`; `usr/sbin/cpuset`, `runon`, `dplace`, `dprof`, `usr/etc/miser`, the six `usr/sbin/miser_*` commands, `usr/bin/jlimit`, `libcpuset.so` (n32 and 64-bit), `usr/lib32/mips4/libdplace.so` and `libdprof.so`; `usr/bin/perfex` and `libperfex.so` (three ABIs); `usr/etc/autofs`, `autofsd` and `mount_autofs`; `usr/lib/devicelib/dev_cdrom.so` and `usr/sbin/scsicontrol`; `sbin/xvm`, `sbin/mkfs_xfs` and `usr/sbin/xfs_bmap`, `xfs_freeze`, `xfs_growfs`, `xfs_io` and `xfs_quota`.
- Not held: any 6.5.22 userland. Only the 6.5.22 kernel archives and linked kernels are in `root/6.5.22/`.

```files
/unix
/usr/cpu/sysgen/IP22boot
/usr/cpu/sysgen/IP27boot
/usr/cpu/sysgen/IP30boot
/usr/cpu/sysgen/IP32boot
/usr/cpu/sysgen/IP35boot
/var/sysgen
```

The kernel on a running system is built from the object files at install time, so `/unix` differs between systems of one platform in the modules it links; the three `unix.*` files here are the ones this repository's tables were measured on (see `scripts/inventory-root.sha256`).

## 2. IRIX 6.5.22

Present in the working `root/`:

- `usr/cpu/sysgen/IP22boot/`, `IP27boot/`, `IP30boot/`, `IP32boot/` and `IP35boot/`: the kernel object archives and objects, from the three-disc **IRIX 6.5.22 Installation Tools and Overlays (1 of 3)**, **Overlays (2 of 3)** and **Overlays (3 of 3)** media (files merged, no name appears on two discs). 210 files for IP22, 288 for IP27, 266 for IP30, 222 for IP32 and 346 for IP35. Each directory holds a few names that the media installs as links to another member (`socket.a` to `bsd.a`, and for IP35 `cl.o`, `if_cl.o`, `partition.o`, `pts.a` and `xpc.o`); they are kept as links.

Disc image SHA-256 (the files these were read from):

```
c36d43431569902a9bdf868cb9f98ea44519a9a99e2bb68dbf1be76be0eec299  IRIX 6.5.22 Installation Tools and Overlays (1 of 3).iso
9064b95ef1c4589c9799a58bbcd7b9f99577d6516555ddc8ed02b703655b58cc  IRIX 6.5.22 Overlays (2 of 3).iso
d2899e9aac3f6f62a446d881ccc2f214afc181c195272ab0e658ca974528557f  IRIX 6.5.22 Overlays (3 of 3).iso
```

The hash of every extracted file is in `scripts/inventory-root.sha256` under `6.5.22/`.

Also present:

- `unix.IP27`, `unix.IP30` and `unix.IP35`: the linked 6.5.22 kernels, in `root/6.5.22/`. This repository does not record which installed system they came from. Their SHA-256 hashes are in `scripts/inventory-root.sha256`, and [docs/version-diffs.md](docs/version-diffs.md) compares them with the 6.5.30 kernels.
- `var/sysgen.IP27`, `var/sysgen.IP30` and `var/sysgen.IP35`: kernel configuration files (`master.d`, `mtune`, `system`, `stune`, `ioconfig`), with no object code.

Not present, wanted next:

- `unix.IP32` (O2): no installed 6.5.22 O2 kernel is held.
- The base libraries and applications, as for 6.5.30.

The 6.5.22 patch sets for 32-bit and 64-bit systems are patches, not a base install, and are not in `root/`.

## Links and archives

```archives
```

```links
```
