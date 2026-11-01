# IRIX files needed for this repository

This lists the proprietary SGI files a reader copies from their own IRIX media or install into a local `root/` directory. They are not tracked in source control. Paths only: this repository holds no file contents.

`root/` holds one directory per release, `root/6.5.30/` and `root/6.5.22/`, each mirroring the IRIX filesystem plus the linked kernels as `unix.IPnn`. `scripts/populate-root` makes the links that the per-area directories expect.

## 1. IRIX 6.5.30 (primary)

Present in the working `root/`:

- `usr/cpu/sysgen/IP27boot/`, `IP30boot/`, `IP32boot/`, `IP35boot/` and `IP22boot/`: the kernel object archives and objects, from the three-disc **IRIX 6.5.30 Installation Tools and Overlays (1 of 3)**, **Overlays (2 of 3)** and **Overlays (3 of 3)** media (files merged, no name appears on two discs). 292 files for IP27, 274 for IP30, 230 for IP32, 353 for IP35 and one for IP22.
- `unix.IP27`: the installed 6.5.30m kernel of an Onyx2/Origin 2000 class system (IP27), from a completed installation.
- `unix.IP30`: the installed 6.5.30m kernel of an Octane (IP30).
- `unix.IP35`: the installed 6.5.30m kernel of an Origin 300/350 class system (IP35).

Not present, wanted next:

- `unix.IP32` (O2): no installed O2 kernel is held. The IP32 objects above are present, so the object inventory covers IP32 without the linked image.
- `var/sysgen/` (`master.d`, `mtune`, `system`, `stune`, `sassign`): the kernel configuration files, from the same media.
- The base libraries and applications (`lib32`, `lib64`, `usr/bin`, `usr/sbin`, `sbin`) for the later, wider scope.

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
