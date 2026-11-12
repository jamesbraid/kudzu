# IRIX software for the DM2 (XT-DIGVID) and DIVO video boards

What the shipped IRIX software for two XIO video options does on the host side: the kernel driver
modules, the user-space modules and daemons, the ioctl surface, the configuration files and the
firmware download path. The hardware facts (widget identity, registers, interrupts, DMA) are in the
sibling chips repository's scoping datasheet for the DM2 and DIVO; this page keeps the software.
Grades follow [AGENTS.md](../AGENTS.md): **established** (read in the object, with address),
**inferred**, **unknown**.

## What was read

| Release / set | Files | Notes |
|---|---|---|
| DMediaPro DM2 and DM3 Software 1.1.3ml (CD set, tar sha256 `fb1d0c17…66ef`), products `ml`, `ml_xtdigvid`, `ml_xtdigvid_dev`, `vbob`, patch `SG0005254` | `usr/cpu/sysgen/IP27boot/xtdigvid.o` (sha256 `138ab548…38ae`), `IP30boot/xtdigvid.o` (`952c3d9e…772b`), `IP35boot/xtdigvid.o` (`64c024a5…1976`), `usr/lib32/mlmodule/libdigvid.so` (`aacf8d4a…c2ca`), `usr/lib32/libML.so.1`, `libMLU`, `libMLddk`, `usr/sbin/mldaemon`, `usr/bin/mlquery`, `var/sysgen/{master.d,ioconfig,mtune,system}/xtdigvid`, `usr/dmedia/bin/vbob/*`, `usr/diags/ml_xtdigvid/bin/*` | the "1.1.3ml" set is the last DM2/DM3 release held; the kernel objects were built for IRIX 6.5.21 and later. Whether the DM2 release that ships on an older Discreet Octane2 media differs is open (below) |
| DIVO and DIVO-DVC software 2.4, products `divo`, `divo_6511m` (tardist sha256 `9f2b273a…643f`) | `usr/cpu/sysgen/IP{27,30,35}boot/divo.a`, `divo_al.o`, `mdivo.o`, `usr/lib32/dmedia/video/DIVO.so` (`0d506518…6eb02`), `usr/dmedia/bin/DIVO/*`, `usr/dmedia/ucode/DIVO/*`, `etc/init.d/initdivo`, `var/sysgen/{master.d,ioconfig,system}/divo*` | the 6.5.11m overlay is what is read for the objects; the base product's configuration files for the rest |
| IRIX 6.5.30 overlays | `usr/etc/videod`, `usr/lib32/dmedia/arch/{directvl,videodvl}/libvl.so.1`, `usr/include/dmedia/vl_divo.h` | the VL stack on the base media |

The files are in `root/dmediapro-dm2-1.1.3ml/`, `root/divodvc-2.4-6.5.11m/` and `root/vl-6.5.30m/`. They were taken out
of the CD images with the repository owner's media extractor. The new binaries are outside the configured inventory scope, so the inventory tables are not regenerated.

## Two different stacks

| | DM2 / DM3 (XT-DIGVID) | DIVO / DIVO-DVC |
|---|---|---|
| API | OpenML (`libML`, ML modules in `/usr/lib32/mlmodule`) | Video Library (`libvl`, `videod`, device module `DIVO.so`) |
| Daemon | `mldaemon` (no `dmdaemon` exists in either set or on the 6.5.30 base media) | `videod`; the boot script `initdivo` starts it by running `divo_cmd` |
| Kernel driver prefix | `xtdigvid_` (`master.d` flags `ocsn`) | `divo_` (`ocsn`) and `divo_al_` (`cso`, needs `kdsp` and `divo`) |
| ioconfig line | `12 14 -1 -1 xtdigvid 0 0x6d687767` | `12 10 -1 -1 divo 0 0x6d687767` |
| Device nodes | `/hw/video/xt-digvid/` (`pipe0`, `pipe1`, and the board node `xtdigvid`) | under the board's hwgraph vertex |
| Firmware | eight FPGA files, downloaded by user space over PIO | an ELF for an on-board MIPS, FPGA files, all burned into on-board flash by user space |

The ioconfig fields are class 12 (`INV_VIDEO`, established in [hinv-codes.tsv](hinv-codes.tsv)), type 14 or 10, and the ioctl number `'mhwg'`; the type names
`INV_VIDEO_XTDIGVID` and `INV_VIDEO_DIVO` are the comment in each shipped ioconfig file. Established.

## DM2 kernel driver (`xtdigvid.o`)

Established unless marked. ELF64 big-endian relocatable, n32 mips4, with a symbol table and line tables, 35 sections, text 0x40678 bytes.

- Registered as a Crosstalk widget driver for part `0xc102`, manufacturer 682, prefix `xtdigvid_` (`xtdigvid_reg`, text 0x0; same immediates in all three platform objects). `xtdigvid_reg` also creates the hardware-graph path
  `video/xt-digvid` and validates the tunable `xtdigvid_hub_io_crb_cnt` (0 to 15).
- Entry points: `xtdigvid_attach`, `xtdigvid_detach`, `xtdigvid_open`, `xtdigvid_close`, `xtdigvid_map`, `xtdigvid_unmap`, `xtdigvid_ioctl`, `xtdigvid_poll`, `xtdigvid_dma`, `xtdigvid_dma_abort`, `xtdigvid_post_event`, `xtdigvid_unload`, `XTDV_restart`,
  three interrupt handlers, `xtdigvid_idbg_register` (debugger commands `xtdv_brd`, `xtdv_brds`, `xtdvlog`, `xtdvslog`, `xtdvsdump`, `sortxtdvlogs`, `resetxtdvlogs`). The idbg routines dump the whole per-board state.
- Board state is 0x14860 bytes with a magic word `0x6d616731` ("mag1") at the start and `0x6d616732` ("mag2") at offset 0x14858; each of two pipes is a 0xa368-byte record starting at board offset 0x50, with the same magics.
- Device nodes: `pipe0` and `pipe1` character devices under the board vertex, mode 0666, plus `xtdigvid` (board node); the open, close, map and poll routines test a vertex-type word (1 pipe, 2 board).
  A pipe can be opened once at a time ("pipe already opened"). The ML module maps the board's 16 MiB window through the board node.
- ioctl commands: family `0x68647601` to at least `0x68647620` (bytes "hdv" and a number; the user module issues 24 distinct values: 0x01 to 0x07, 0x09, 0x0b, 0x0f, 0x12 to 0x14, 0x16 to 0x20; the library's `read_word` uses 0x01 and `write_word` 0x02).
  `0x14` is the VBOB message write. Names the driver prints for its cases: get units, set `dm_index`, set event mask, start dma, end dma, ioctl reset ("XTALK reset", "calling XG_RESET"), invalidate map RAM, `vbob_cmd`, clink-ack lock/unlock,
  reset "sdma ring ptr", log requests, set and get MCS info. Individual command numbers to case names are not mapped (**unknown**: the dispatch is 23,240 bytes with a jump table; not read).
- Attributes exported to the hardware graph: `irix_unit` (from ioconfig) and `dm_index`; the NIC text is attached to the board vertex and read by `usr/diags/ml_xtdigvid/bin/xtdigvid_hinv` with `attr -g _nic`.
- Events: a per-pipe event queue with a mask, posted by the interrupt code and read through `xtdigvid_poll`: TRANSFER_ERROR(BUFFERS_ABORTED), CLINK_DATA_RCVD, VBOB_CLINK_ACK, SYNC_GAINED, SYNC_LOST, XFER_FAILED,
  SEQUENCE_LOST, TRANSFER_COMPLETE(ust/msc), STREAM_STOP, STREAM_START (the strings the post routine prints).
- Tunables (`var/sysgen/mtune/xtdigvid`): static `xtdigvid_hub_io_crb_enable` 1, `xtdigvid_hub_io_crb_cnt` `0xf 0x4 0xf`, `xtdigvid_xbow_arb_reg_enable` 1, `xtdigvid_siglost_timer` 175; run-time `xtdigvid_enable_event_log` 1,
  `xtdigvid_debug_level` 0. The `system` fragment is `USE: xtdigvid`, with a comment that the board is for XIO-based systems only.
- The driver uses the DMS fifo interface (`dms_open`, `dms_fifo_attach`, `dms_fifo_deq`, `dms_fifo_get_user`, `dms_set_callback`), `userdma`/`undma`, `uvaddr_to_alenlist` and `xtalk_dmamap_*` for user-buffer DMA, and kernel UST (`get_ust`,
  `update_ust`, `ust_to_nano`).
- The platform objects differ in the Origin builds' use of the hub CRB tunables; the IP30 object hard-codes the Xbow arbitration register addresses (chips datasheet section 3).

## DM2 user side

- **`libdigvid.so`** is an ML device-dependent module (the ML "DDK": `ddConnect`, `ddOpen`/`ddClose`, `ddGetCapabilities`, `ddIdentifyJack/Path/Param`, `ddInterrogate`...). It exposes the jacks and paths of two pipes, the custom controls (LUTs, edge-enhance and
  frame-flip modes, loopback, genlock state and error status, colour-converter filter, VBOB reinit, custom video format) and wraps the `hdv` ioctls (`read_word`, `write_word`, VBOB message write, set and get MCS info).
- **Firmware download is here, not in a daemon** (details and register values in the chips datasheet, section 6): `xthd_program_xilinx` (n32, virtual address 0x416570 in the shipped build) maps the 16 MiB window, builds the file
  name `%s/usr/dmedia/ucode/xtdigvid/%s.mcs` from `_XTDIGVID_MCS_ROOT` (default root `/`), reads the Intel HEX records with `xilinx_load_intel`, and calls one of two start/byte/stop routines chosen by the kind field of the per-FPGA record in `xthd_fpga_info` (28 bytes each, eight records).
  The failure messages name the FPGAs GIF, CLINK, HDOC, OXCC, UNPACKER, VIF, PACKER, IXCC; the load order and the order inside the table were not established (**unknown**); the GIF must be first because the others are configured through it.
- **`mldaemon`** (`/usr/sbin/mldaemon`, started by `etc/init.d/mldaemon`, switched by `chkconfig mldaemon`, links `rc2.d/S98mldaemon`, `rc0.d/K02mldaemon`) daemonises, and creates and manages a shared segment
  (`/tmp/.ml32DaemonSharedCapabilites`, symbols `CreateSharedSegment`, `_mlOSDiscoverLocalSystem`, `_mlOSDiscoverLocalDevices`) holding the capabilities `libML` collected from the modules in `/usr/lib32/mlmodule` (`_MLMODULE_ROOT` overrides the directory).
  It never opens a board. `libML` complains "daemon not ready yet" and "daemon mismatch with library" when the segment is missing or stale.
- **`mlquery -i`** prints the inventory line `XT-DIGVID Multi-standard Digital Video: controller %d, unit %d, version 0x%x`. `usr/diags/ml_xtdigvid/bin/xtdigvid_hinv` reads the NIC attribute and prints name, part number, serial number, unit and `dm_index` for each
  board found under `/hw` (`find /hw -type c -name xtdigvid`).
- **VBOB tools** (`usr/dmedia/bin/vbob/`): `vbob_config`, `vbob_flash` (the patch ships a newer one, 65,856 bytes), `vbob_update1`/`vbob_update2` (shell scripts), plus the diagnostics `vbob_reset`. The firmware set is under `usr/dmedia/ucode/vbob/`.
  The flash tools talk to the box over the LVDS control channel through the same driver ioctls (**inferred** from the `libvboblvds` example path recorded in the install manifest and the shared message ioctl; not read).
- Diagnostics: `xtdigvid_confidence`, `xtdigvidtst`, `dv_memtovid`, `dv_vidtomem`, `dv_vidtogfx`, `dv_vidtocmp`, `dv_flapper`, `check_ustmsc`, `diff_images`, and test patterns; their environment switches
  (`_XTDIGVID_NO_VBOB_SUPPORT`, `XTDV_IGNORE_SEQLOST`, `_XTDIGVID_TEST_INPUT`, `_XTDIGVID_TEST_OUTPUT`, `XTDIGVID_DEBUG`) are in the Discreet repository's switch tables.

## DIVO software

- **Kernel**: `divo.a` has seven members (`divo.o`, `divo_init.o`, `divo_ioctl.o`, `divo_intr.o`, `divo_dma.o`, `divo_flash.o`, `divo_idbg.o`) plus `divo_al.o` (audio, depends on `kdsp`) and `mdivo.o` (diagnostics). `divo_init` registers a **PCI** driver,
  `pciio_driver_register(0x10a9, 2, "divo_", 0)`, adds NIC entries, creates the hardware-graph path, and adds idbg commands. Attach maps the board through `pciio_piomap_alloc`, initialises the Bridge for the board, allocates two interrupts
  (`divo_intr_0`, `divo_intr_1`) and registers two error handlers. Established.
- **User**: `DIVO.so` is the VL device-dependent module (`usr/lib32/dmedia/video/DIVO.so`, loaded by `videod`). It owns the flash and firmware logic: `divo_read_firmware` (reads an ELF), `divo_flash_get_section_info`,
  `divo_flash_write_{elf,rpc,rpc2,vidif,vidif28,vpacker,vpacker2,apacker,bootinfo}`, `divo_flash_verify_checksum`, `divo_flash_erase`, `divoCheckFlash` (run at start, `_DIVO_CHECK_FLASH`), `divoDumpFirmware`, `divoLoadDVPacker`,
  `_divoLoadPackerUcode` and `divo_xilinx_load_intel`. Flash is updated only when the disk and PROM sections differ ("flash not updated (disk and prom sections are same)"). Established (strings and symbols).
- **Boot**: `etc/init.d/initdivo` (switched by `chkconfig initdivo`) runs `divo_cmd` in the background, which "just contacts the server" so that `videod` checks the flash and initialises the boards; the script's comment says this is required for DIVO audio and for `hinv` to be correct. Established (readable script).
- **Tools** (`usr/dmedia/bin/DIVO/`): `divo_cmd`, `divo_reset`, `divo_ee`, `divo_memtovid`, `divo_vidtomem`, `divohinv`, `flashdivo`, `flashalldivo` (and a `28` variant), `dvparse`; diagnostics under `usr/diags/DIVO/bin` including per-block dump tools (`csc_dump`, `genlock_dump`, `rpc_dump`, `sdti_dump`, `vidif_dump`, `vip_dump`).
- **Firmware files** (`usr/dmedia/ucode/DIVO/`): `divofw`, `divofw.2925`, `divofw.dvc` (ELF, big-endian MIPS, mips2), `lincprom` (boot PROM image), `divolog.sym`, `rpc*.mcs`, `vidif*.mcs`, and about 50 1344-byte packer files (`.pb`, `.ub`).
- The generic `videod` contains no DIVO-specific strings: the device-dependent code is in `DIVO.so`.

## The VL server (`videod`), and what a graphics-to-video output needs of it

Read from the 6.5.30 overlay builds `usr/etc/videod` (sha256 `48b7a41bb90989fa1385f57652fcf384c47e3dd52912013fc42840b6629bf131`, N32 with a symbol table), `usr/lib32/dmedia/arch/videodvl/libvl.so.1` (`2d559bbdccf5c90a94eff2f4a74382472e93e708b31709abe29cc09e4f4ccadb`) and `usr/lib32/dmedia/video/mvp.so` (`f27c4fb092655b855155dbd87e4002e021282f9fc6d8167d88e4ea2a5c23ec2c`), the IP27 kernel objects and `unix.IP27`, and `usr/gfx/gfxinit`. The start-up script is held only from the 6.5 base media (`etc/init.d/videod`); no later copy is in the files read.

### Start-up

- **Established** (the base-media script). `etc/init.d/videod start` runs the video part of `MAKEDEV` (to link `/dev` names to the hardware-graph video nodes), then starts `/usr/etc/videod` in the background if `chkconfig videod` is on and the file is executable. With the window system on it sets `DAEMON=1` for the daemon.
- **Established** (`main`, 0x1000cb40-0x1000cf48). The daemon sets up its tables (`OsInit` 0x100211f0, `InitClientResources` 0x100102b0, `InitControls` 0x100086c0, `ResetPathPrivates` 0x1000f870), then calls `InitDevices` (0x1001bf00, call at 0x1000ccf8). It reads the device count the scan left at +4 of the device table (0x1000cd00) and, if it is not above zero, calls `FatalError` (0x10021010) with "no devices found" (string 0x100242e8, call at 0x1000ced8), which ends the daemon. Only after that does it load the factory and system control defaults (`restoreFactoryControls` 0x10009460, `restoreSystemControls` 0x100093d0, which reads `/usr/etc/video/videod.defaults`), start the window-system side (`InitWindowSystem` 0x1001b940) and the connection block, and enter `Dispatch` (0x1000a980).
- **Established** (`InitDevices`, 0x1001bf00-0x1001c4f4, and the loader at 0x1001c730). Devices come only from shared objects. The scan opens each directory of a search list, takes the entries whose names end in `.so` and calls the loader for each. The default list is `/usr/lib32/dmedia/video` (string 0x100246b0); the environment variables `_VL_ROOT` and `_RLD_ROOT` (0x100246d8, 0x100246e8) add to or replace it (**inferred** from the order of the `getenv` calls; the list handling is a colon-split at 0x1001bf8c-0x1001c068), and `_VL_DEBUG` (0x100246c8) turns on a "Loading DSO" message. The loader builds `<dir>/<name>.so` (0x10024740), `dlopen`s it, then looks up symbols whose names it builds from the module name with the formats `%sExtensions` (0x100247a0), `%sDeviceInfo` (0x100247b0) and a third format held in the data segment. A failed `dlopen`, a missing symbol and a version mismatch each print their own message ("loadDSO: Cannot load", "Cannot find symbol", "is not the same version as videod"). `AddDevice` (0x1001cbf0) registers what the module returns; the messages "Unable to open device %s for %s." (0x100246f8) and "Unable to add device %s" (0x10024720) show that opening the hardware is the module's job. **Unknown:** which symbol of a module the three formats name; `mvp.so` exports neither `mvpExtensions` nor `mvpDeviceInfo` but `ddLoadDriver` and `mvpdevices`.
- **Established** (the dynamic symbol table of `videod`). The daemon imports no inventory routine (no `getinvent`, `setinvent` or `scaninvent`); its device knowledge is entirely in the modules it loads.

### What the held media offers a graphics-to-video output

- **Established** (all `usr/lib32/dmedia/video` files in the 6.5.30 and 6.5 base extractions). The only VL device modules on the base and 6.5.30 media are `mvp.so` (6.5.30 and base; it opens `/dev/mvp`), `vino.so` (base only) and a debug build of `impactcomp.so` (base only). The DIVO and DM2 modules come from the separate option-board products described above. **No VL module for a GVO exists in the files held.**
- **Established** (text search for GVO and DGO over `usr/gfx`, `usr/lib/X11/dyDDX`, `usr/lib32/dmedia`, `usr/etc`, `usr/sbin`, `usr/bin`, `etc`, the 6.5.30 `IP27boot` objects and `unix.IP27`). The strings occur only in `gfxinit` ("GVO option detected"), `ircombine`, `setmon`, the four `kona*.so` X modules and the display firmware. The kernel objects and the linked IP27 kernel have no GVO text, so no separate GVO or video kernel driver exists in this release: the option is attached by the display driver and the display firmware.
- **Established** (`kona_inventory_board`, `kona_edtinit.o` in `IP27boot/kona.a`, text 0x2ec-0x380). The display driver makes one inventory call per pipe: class 7 (graphics), with the type, controller, unit and state read from the board record at +0x88c, +0x890, +0x888 and +0x894. No other member of `kona.a` calls the inventory routine, so there is no call for a video option and none in class 12 (video). The `INV_VIDEO` entries come from the video drivers (the DIVO and XT-DIGVID types in [hinv-codes.tsv](hinv-codes.tsv)). **Inferred:** a GVO does not appear in `hinv -c video`; Discreet's installer lists that class only to look for a Sirius (read from the installer script's own logic, not from a binary here).
- **Unknown:** what a GVO VL device module would probe (ioctls, inventory entries, option fields), because none is held. What the display side offers is described in the InfiniteReality repository (`docs/dg.md`, the option mask and the GVO physical port 0xc6); the Xsgi video-control extension, not the VL, is the interface the `gvo(7)` page names for configuring it.

### Why the open fails when `videod` is not serving

- **Established** (`libvl.so.1`, 6.5.30). `vlOpenVideo` (0xc40c810) connects with `_vlConnectServer` (0xc406de0), using the socket path prefix `/tmp/.vs-unix/vs` (0xc4140e8) and the environment variable `VLSERVER` (0xc414cf0); it never starts the daemon. If the connection call fails it frees its record and sets error 11 (0xc40cf5c-0xc40cf70). After the connection it installs an error handler with `setjmp`, sends the client prefix and reads the 8-byte setup reply (0xc40c920-0xc40ca1c); a transport failure there takes the `longjmp` path and sets error 10 (0xc40c978). A reply from a server with a different protocol revision prints "client is protocol rev %d, server is rev %d" (0xc414d48), and a server that refuses the client prints "connection to \"%s\" refused by server" (0xc414d88).
- **Established** (the message table at 0xc414df0, indexed by error number): 10 is "VL: Bad access to the library", 11 "VL: Failed allocation of resource or memory", 20 "VL: Bad server passed as parameter". Every later `vl*` call checks its server with `_vlServerTagValid` (0xc40c380), which returns zero for a null server or one without the library's tag, and the check reports error 20. So a program that ignores a failed `vlOpenVideo` and carries on prints "Bad server passed as parameter" for each following call.
- **Inferred:** with no `videod` running, or one that ended on "no devices found" before it served a connection, the open fails and every later call reports error 20.

## Unknown, with the route

| Unknown | Route |
|---|---|
| Case-by-case ioctl map of `xtdigvid_ioctl` and its argument structures | read the dispatch table and each case; the matching user-space callers in `libdigvid.so` give the argument shapes |
| The body of `xtdigvid_dma` and the interrupt service | split the 97 KB symbol and read |
| `xthd_fpga_info` contents | dump from the library's data section |
| Whether the DM2 release shipped on a Discreet Octane2 media (older `libdigvid.so`, a different build from this set) uses the same ioctl set and kernel object | compare the two builds' hashes and strings; the Discreet repository holds the other build's hash |
| DIVO `divo_ioctl`, `divo_dma`, `divo_intr` bodies, the firmware mailbox, and `divofw` | read the objects in hand; `divofw` is a 0x37240-byte MIPS program |
| `hinv` itself (how the inventory entry turns into the printed line) | the hinv program is not held in `root/6.5.30`; the format is established only in `mlquery` and in published captures |
