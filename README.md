# kudzu

How IRIX 6.5.30 and 6.5.22 boot and drive SGI hardware, from the kernel's side. If you want to know what the
kernel expects to find before it will attach a driver, this is where to look.

Worked out from the shipped kernels (`unix.IP27`, `unix.IP30`, `unix.IP35` and the boot objects), the public
IRIX 6.5.7m source, SGI manuals and the Linux and BSD drivers. Nothing here probes a machine. Every fact notes
where it came from and whether it's established, inferred or still unknown.

## What's covered

- **Boot and hardware discovery:** KLCONFIG as the kernels walk it, building the hardware graph, Hub interrupt
  routing and the crosstalk layer.
- **PCI and I/O bridges:** the PCI layer, the Bridge, XBridge and PIC provider, slot scans and driver attach.
- **Platform kernels:** IP27, IP30 and IP35 errors, halt and reset, IOC3, routers and timers.
- **O2 video:** CRIME, GBE and MACE video as the drivers use them, plus the DM2 and DIVO boards.
- **On-disk and board formats:** EFS, XFS, XLV, XVM, the volume header, KLCONFIG, one-wire board IDs and more.
- **System interface:** system calls, `syssgi` selectors, panic sites, and what changed between 6.5.22 and
  6.5.30 ([evidence/version-diffs.md](evidence/version-diffs.md)).
- **Userland:** base programs, the ABI and the loader, one line each for now.

The kernel pages are under [software/kernel/](software/kernel/), the formats under [formats/](formats/README.md).

## Using it

The headers and the crate live in [generated/](generated/). Don't edit them; they're built from the
`format.toml` files under `formats/`.

To run the checks yourself:

```sh
git clone https://github.com/jamesbraid/kudzu && cd kudzu
tools/setup-clone.sh --root /path/to/your/irix/files
retools/tools/re-run retools/tools/defs/ci-check
```

You'll need Podman, git, python3 and your own IRIX files. [evidence/irix-files.md](evidence/irix-files.md) lists
which ones.

## How far along

Still a long way to go on the functions; most of the kernel outside the paths above isn't read yet. The open questions are in [questions/](questions/README.md).
If you spot a mistake or have a manual we don't, [open an issue](https://github.com/jamesbraid/kudzu/issues).

<!-- coverage:begin (copied from evidence/coverage.md; regenerate with: retools/tools/re-run python3 retools/tools/inventory.py && retools/tools/re-run python3 retools/tools/kudzu/os_tables.py) -->
| Table | Rows | Understood | Located or better |
|---|---:|---:|---:|
| binaries | 7737 | 0.0% | 0.0% |
| functions | 242521 | 1.3% | 13.6% |
| mmio | 176 | 2.3% | 2.3% |
| ioctls | 7 | 28.6% | 100.0% |

| Kernel table (in scope) | Rows | Documented |
|---|---:|---:|
| syscalls | 2844 | 2418 |
| structs | 343 | 277 |
| panics | 6987 | 5140 |
| selectors | 18 | 18 |
| formats | 29 | 15 |
<!-- coverage:end -->

## License

[BSD 3-Clause](LICENSE). The `retools/` scripts started life as gr4's and keep its notice. No SGI binaries in
here; bring your own.

## Credits

James Braid, plus a lot of Claude and Codex reading MIPS kernel disassembly so I didn't have to.

Thanks to -

* Dominik Behr ([@techomancer](https://github.com/techomancer)) for [gr2](https://github.com/techomancer/gr2)
  and [gr4](https://github.com/techomancer/gr4), where the layout and working style come from
* The Linux-MIPS, NetBSD and OpenBSD folks, whose SGI ports get checked against constantly
* The XFS developers, whose xfsprogs still documents the on-disk format
* The binutils, GCC and Open64 people, whose tools make sense of IRIX's ELF and DWARF
* Everyone keeping SGI manuals, `hinv` dumps and IRIX media online
