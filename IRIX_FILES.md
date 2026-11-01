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

**Not present.** The only 6.5.22 material held is an IP22 (Indy) disk image of 6.5.22m (not an IP27, IP30 or IP35 system, and a compressed disk image that has not been unpacked) and the 6.5.22 patch sets for 32-bit and 64-bit systems (patches, not a base install). Neither gives the IP27, IP30 or IP35 kernels or the `sysgen` object archives, so none of it is in `root/`.

The scaffold has the slot: `root/6.5.22/`, the paths of the `kernel-image` group in `scripts/scope.json` and the same `files` list as above. Fill `root/6.5.22/` from 6.5.22 media or an installation the same way as section 1, then run `scripts/gen-scope.py` and the inventory, and write the root manifest with `scripts/inventory.py --update-root-manifest`.

## Links and archives

```archives
```

```links
```
