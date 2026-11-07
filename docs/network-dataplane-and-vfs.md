# Network dataplane and virtual file system attachment points

Where a packet filter or a tun-style device could attach to the IRIX 6.5.30 network stack, how STREAMS modules are pushed, which network pseudo-interfaces the release carries, and what the virtual file system (VFS) layer looks like to someone judging a user-space file system layer. The IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF) was read first; section 5 says where the IP27 and IP30 kernels differ. The page reuses the behaviour-chain conventions of [syscalls.md](syscalls.md) and the loadable-module selector of [syssgi.md](syssgi.md) instead of repeating them. [network-and-snapshots.md](network-and-snapshots.md) has the filter matching rules, interface aliases and virtual-interface tunables, and [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md) has the module loader; this page cites them where they settle a point and does not repeat their content.

Evidence is the three linked 6.5.30 kernels, the kernel object archives in `usr/cpu/sysgen/IP35boot` (`bsd.a`, `snif.a`, `ipv6.a`, `umfs.a`, `if_sl.o`, `if_ppp.o`, `ppp_fram.o`, `ipfilter.o`), the userland program `automount` in `usr/etc`, and the 6.5.22 IP35 system-generation files for configuration. No published manual page is cited: none was retrieved for this page, so every behaviour below is read from a binary and the public names (STREAMS command numbers, address families) are hints only. Routine names and sizes come from the kernel's own symbol and debug records. Grades follow [AGENTS.md](../AGENTS.md): Established means read in the named routine or table, Inferred names its basis, Unknown names what would settle it. Nothing here was run: no IRIX guest, no packet, no mount was observed.

## 1. Network packet path

### 1.1 What the stack is

The IP35 kernel carries a BSD-style socket and protocol stack in the kernel proper (`socreate` at 0xc0000000001ae494, 768 bytes; `sosend` at 0xc0000000001af024, 1808 bytes; `soreceive` at 0xc0000000001af734, 3040 bytes), built from the archives `bsd.a` and `socket.a`. Interfaces are `ifnet` records, not STREAMS queues. STREAMS exists alongside it for terminals, pipes, the sockets-over-TLI helper `timod`, and a data-link provider driver (`snif`, section 1.5). The TLI-based socket drivers `tpitcp`, `tpiudp`, `tpirawip` and `tpiicmp` are not in the installed kernel: `tpisockd_init` (0xc00000000049759c) is an 8-byte stub, the form `tpisocketstubs.a` supplies. Established.

### 1.2 The interface record

`ifnet` is 240 bytes, identical in all three 6.5.30 kernels (debug record). Established.

| Offset | Size | Member | Note |
|---:|---:|---|---|
| 0 | 8 | `if_name` | pointer to the name string, for example "lo" |
| 8 | 8 | `if_next` | list of all interfaces; the list head is the global `ifnet` (0xc000000001646d10) |
| 16 | 8 | `in_ifaddr` | |
| 24 / 26 / 28 | 2 / 2 / 2 | `if_index`, `if_unit`, `if_timer` | `ifindex2ifnet` (0xc00000000163ce60) maps an index back |
| 32 | 4 | `if_flags` | |
| 36 | 68 | `if_data` | type byte at +0, header length +2, MTU +4, metric +8, speed +12, then counters: in/out packets, errors, collisions, bytes, multicasts, input queue drops, output drops, unknown-protocol drops, last change |
| 104 | 8 | `if_output` | transmit entry |
| 112 | 8 | `if_ioctl` | |
| 120 / 128 / 136 / 144 | 8 each | `if_reset`, `if_watchdog`, `if_rtrequest`, `if_findaddr` | |
| 152 | 32 | `if_snd` | an `ifqueue`: head, tail, length, maximum length, drops, lock |
| 184 | 8 | `if_sec` | security attributes |
| 192 / 200 | 8 each | `if_sendspace`, `if_recvspace` | |
| 208 | 16 | `if_mutex` | |
| 224 | 4 | `ipalias_count` | |
| 232 | 8 | `if_ext` | a 104-byte extension record with the IPv6 state |

Driver records embed the `ifnet` at offset 0 and are larger: `ether_attach` (0xc0000000001c2778, 432 bytes) writes the station address at +0xf0 to +0xf5 and two driver callbacks at +0x110 and +0x118 of the record it is given. Established for the stores; that +0x118 is the driver's transmit-start routine rests on `ether_output` calling through it (section 1.4). Inferred.

`if_attach` (0xc0000000001b1234, 364 bytes) initialises the send-queue lock (+0xb4) and the interface mutex (+0xd0), takes the interface-list update lock (`mrupdatef`), links the record into the global list (the walk tests flag bit 0x8 of each entry), and allocates the extension record (`kmem_zalloc`, stored at +0xe8 of the record, whose first fields get small constants including a hop limit of 0x40). Established. Where the interface index is assigned was not found in this routine.

The mbuf is 256 bytes: `m_next` +0, `m_off` +8 (the data address is the mbuf address plus `m_off`), `m_act` +16 (the packet-queue link), `m_len` +24, `m_flags` +26, `m_type` +27, `m_index` +28, then a 224-byte union. Established; the offsets come from the debug record and the data-address rule from `network_input` and `netproc` below.

### 1.3 Receive path

A driver interrupt reaches the stack through `ether_input` (0xc0000000001c29f8, 1136 bytes), called by the Ethernet drivers' receive routines (`ef_recv`, `ef_intr`, `eg_recv`, `tg_recv`, `xframe_receive_complete`, found by scanning every direct call in the kernel text). In order, `ether_input` does the following. Established for the calls and constants; the pairing of each Ethernet type with its queue is Inferred from the compares being adjacent.

1. Calls `snoop_input2` (twice, 0xc0000000001c2aac and 0xc0000000001c2c10) and `snoop_input` (0xc0000000001c2c80 and 0xc0000000001c2dac) with the interface's raw-interface record at +400 of the interface record: the raw-socket capture hook, section 1.5.
2. Calls `drain_input` (0xc0000000001a6540, 352 bytes) with the same record, which builds an address of family 0x12 on the stack and calls `raw_input`: the second raw-socket delivery.
3. Calls `dlsap_find` (0xc0000000001c3b88, 112 bytes) with a class of 1, 2 or 4 and, when it returns a record, calls the function pointer at +8 of that record: the DLPI upcall, section 1.5.
4. Compares the Ethernet type with 0x800, 0x806, 0x86dd and 0x8183 and hands the packet to `network_input` (0xc0000000001bfcf8, 340 bytes) with queue number 2 (type 0x800), 0x19 (0x806), 0x18 (0x86dd), or to `st_et_input` (0xc000000000496714, 28 bytes, the Scheduled Transfer entry, a stub in this kernel) for 0x8183.

`network_input(m, queue, flags)` does not run the protocol. It writes the queue number into the halfword at +10 of the mbuf's data area, picks the record of the receiving CPU from the table `netproc_data` (0xc000000001646d58) with the CPU number read from the per-CPU page, and under that record's spin lock appends the mbuf by its `m_act` link and counts it. It returns 132 and drops the packet when the count has reached the limit, and wakes the CPU's protocol thread with a semaphore post unless bit 0x1000 of its third argument is set. Established.

The thread routine `netproc` (0xc0000000001bff44, 268 bytes) removes mbufs from its record, reads the queue number back from data+10, looks that number up in a table of handlers, and calls the handler as `handler(m, record + queue * 40 + 32)`. `network_input_unqueued` (0xc0000000001bfe4c, 108 bytes) makes the same call directly in the caller's context. `network_input_setup(n, handler)` (0xc0000000001bfc9c, 92 bytes) stores a handler for a queue number below 29 and prints a message for a larger one. `netisr_init` (0xc0000000001c011c, 344 bytes) allocates the per-thread records of 0x4b8 bytes and starts the threads; `max_netprocs` (0xc000000001646d60) bounds their number. Established; one thread per enabled CPU is Inferred from the `cpu_enabled` and `xthread_setup` calls in `netisr_init`.

The registered handlers in this kernel are, from the three callers of `network_input_setup`: `ip_init` (0xc0000000001bc3bc, 412 bytes) registers queue 2 with `ip_input`; `arp_init` (0xc0000000001ba5b8, 32 bytes) registers queue 0x19 with `netproc_arpinput` (0xc0000000001b9360, 40 bytes); `raw_init` (0xc0000000001ab244, 136 bytes) registers queue 0x12 with `raw_protocol_input` (0xc0000000001ab3ac, 792 bytes). Queue 0x18, which receives Ethernet type 0x86dd, has no registrar in the installed kernel; the IPv6 archive is not linked (section 1.7). Established for the absence, Inferred for the reason.

`ip_input` (0xc0000000001bc79c, 2740 bytes) receives the interface pointer as its first argument from the first doubleword of the mbuf's data area. `arpinput` takes its first argument the same way, from the call in `netproc_arpinput`. So a record that a driver queues starts with a small header that holds the interface pointer at +0 and, after `network_input`, the queue number at +10. Where the packet bytes begin after that header, and who removes the Ethernet header, was not followed. Established for the two loads, Unknown for the layout (section 6).

### 1.4 Transmit path

`ip_output` (0xc0000000001a2a54, 3664 bytes) reaches the interface by loading the pointer at +0x68 of the `ifnet` (the `if_output` slot) and calling it with the interface, the mbuf, the destination address and the route (the calls at 0xc0000000001a2e00 and 0xc0000000001a31b4 load the slot at 0xc0000000001a2df0 and 0xc0000000001a31a4; two further indirect calls, at 0xc0000000001a341c and 0xc0000000001a3690, were not traced). Established for the two traced calls.

For Ethernet, `ether_attach` stores `ether_output` (0xc0000000001c2e68, 1140 bytes) in `if_output` and `ether_ioctl` (0xc0000000001c33e4, 1028 bytes) in `if_ioctl`, sets the type byte to 6, the MTU to 0x5dc and ORs 0x400802 into `if_flags`. `ether_output` resolves the destination with `arpresolve` (0xc0000000001b8abc, 1560 bytes) for IPv4 or `ndp6_resolve` for IPv6, loops back frames for the local station with `looutput`, copies broadcast frames with `m_copy`, and finally calls through the pointer at +0x118 of the driver record (the call at 0xc0000000001c2fd4). The drivers call `ether_selfsnoop` (0xc0000000001c32dc, 264 bytes; from `eg_selfsnoop`, `tg_selfsnoop` and `xframe_selfsnoop`) so that the raw-socket hook also sees transmitted frames. Established.

The loopback interface `loif` (0xc00000000171a538, 240 bytes) is set up by `loattach` (0xc0000000001c2368, 148 bytes): `if_output` is `looutput` (0xc0000000001c23fc, 756 bytes), `if_ioctl` is `loioctl` (0xc0000000001c26f0, 136 bytes), the type byte is 0x18, `if_flags` gets 0x8001808, and it calls `if_attach` and `rawif_attach`. `looutput` calls `snoop_input`, `drain_input`, `m_datacopy` and `network_input`. Established.

### 1.5 Attachment points that already exist

There are four hooks, three of them for observing or filtering and one for adding an interface.

**Raw-socket capture (`rawif` and `snoop`).** `rawif_attach` (0xc0000000001a4b20, 328 bytes) is called by `ether_attach`, `loattach`, and the objects `if_sl.o`, `if_ppp.o` and `if_ptr.o` (it is an undefined symbol of each). It fills a record with the family value 0x12, the interface pointer and the interface name, links it into a list under a lock, and `rawif_lookup` (0xc0000000001a4e38, 128 bytes) finds it by name. `snoop_input` (0xc0000000001a5fe0, 44 bytes) takes and clears the word at +0x188 of the record it is given and passes it on to `snoop_input2` (0xc0000000001a600c, 1208 bytes), which matches listeners with `snoop_match` and the filter routine `sfmatch`, copies the packet with `m_copy`, and delivers it with `raw_input`; `snoop_drop` and `snoop_drop2` deliver a drop notice. So a driver that calls `rawif_attach` once and `snoop_input` on each receive and transmit becomes visible to the capture tools. Established. That the userland capture tool binds through this family is Inferred from the family value 0x12 matching the raw-socket queue registered by `raw_init`.

**DLPI service (`snif`, `llc2`).** `ether_input` looks up a registered data-link service access point with `dlsap_find` and calls the record's function at +8. `snif.o` (in `snif.a`) imports `register_dlsap` (0xc0000000001c3bf8, 84 bytes) and `unregister_dlsap`, `rawif_lookup`, `ifunit`, `mbuf_to_mblk` and `mblk_to_mbuf`: it converts between the two buffer kinds and exposes interfaces as STREAMS minors named by the table `iftab`. The IP35 kernel has `snif_devcnt` equal to 5; the 6.5.22 IP35 `master.d/snif` names the five as "fv0", "ef0", "rns0", "eg0" and "tg0", so the Xframe interface has no minor. Established for the imports and count, Inferred for the Xframe consequence (the 6.5.30 `master.d` is not held).

**IP packet filter (`ipfilter`).** The kernel calls `ipfilter_kernel` once, from `ip_input` at 0xc0000000001bc998, only when the word `ipfilterflag` (0xc000000001646d50) is nonzero and the packet's version nibble is 4; a result of 2 takes the drop path. No other direct call to it exists anywhere in the kernel text. The installed kernel links the stub (0xc000000000083e88, 8 bytes, returns 0); the real routine, the character device, the verdict cache and the packet queue are in `ipfilter.o`, and [network-and-snapshots.md](network-and-snapshots.md#packet-filtering) reads them: what the key is, what the verdict kinds return, the one ioctl request code, and that the daemon is not held. The output side has a separate optional hook: `ip_output` tests the pointer `ipf_process` (0xc000000001646c10) at 0xc0000000001a2dd4 and again at 0xc0000000001a31e4, and `ip_input` passes it to `ipfilter_kernel` as a trailing argument. `ip_init` sets it to zero and no shipped object other than the two IP routines names it (the symbol search is in that page), so on the shipped objects it stays unset. Established for the call sites and the pointer; what code a loaded module would put there is Unknown.
4. **A new interface.** Section 1.7.

### 1.6 STREAMS versus BSD network drivers

The cdevsw table (`cdevsw`, 0xc000000001542f78, 103 slots of 152 bytes, `d_str` at +128) separates the two kinds. Established.

| Driver | Kind | Evidence |
|---|---|---|
| `if_ef` (IOC3 Ethernet), `if_eg` (Gigabit), `if_tg` (Tigon), `if_xframe` (10 Gigabit) | BSD `ifnet`; character device with open routine, no stream table | slots 66, 65, 64, 63 have `d_str` zero and open routines `if_efopen`, `if_egopen`, `if_tgopen`, `if_xframeopen`; each attach calls `ether_attach` |
| `if_ptr` (token ring), `if_rns` (FDDI), `if_cl` (cluster link) | BSD `ifnet` | each of `if_ptr.o`, `if_rns.o` and `if_cl.o` imports `if_attach`, `rawif_attach` and `snoop_input`, and none imports a STREAMS routine |
| `snif`, `llc2` | STREAMS drivers (DLPI) | slots 53 and 52 point `d_str` at `snifinfo` and `llc2info` |
| `if_sl`, `if_ppp`, `ppp_fram` | STREAMS modules that also hold an `ifnet` (hybrid) | section 1.7 |
| `clone`, `pts`/`ptc`, `sad`, `log`, `tp` and similar | STREAMS, not network | slots 2, 3, 39, 50, 51, 70 |

`if_ptr.o` also imports `ipintrq` and `schednetisr`, which none of the three linked kernels defines; how that object resolves them was not followed. The three objects are not linked into the installed kernel.

### 1.7 Pseudo-interfaces and point-to-point links in 6.5.30

| Interface | Where | In the installed IP35 kernel? | Grade |
|---|---|---|---|
| `lo0` loopback | `if_loop.o` in `bsd.a` | yes | Established |
| `is` striping interface | `if_is.o` in `bsd.a` | yes (`is_add_stripe` 0xc0000000001c75a4, 512 bytes) | Established |
| `gif` generic tunnel | `if_gif.o`, `in_gif.o`, `in6_gif.o`, `ip_encap.o`, `if_sit6.o` in `ipv6.a` | no: `gifattach` (0xc000000000495e9c) and `ipip_input` are 8- and 28-byte stubs | Established |
| `sl` serial line IP | `if_sl.o` | no: no `sl_` symbol in any linked kernel | Established |
| `ppp` | `if_ppp.o` with `ppp_fram.o` | no: no `ppp` symbol in any linked kernel | Established |
| `sw` (swIPe) | named in the system file | no object ships; its master file is stubs only | Established |
| `tun`, `tap`, `bpf`, VLAN, bridge | none | no function or data symbol of that kind in any IP35 boot object or archive | Established |

The `is` interface is the one general-purpose pseudo-interface in the installed kernel. `is_add_stripe` allocates its record with `kmem_zalloc`, stores 0x5090 in `if_flags`, 0x18 in the type byte, 0x5bc in the MTU and a name pointer to the string "is", sets `if_output` to `is_output` (0xc0000000001c5260, 1576 bytes) and `if_ioctl` to `is_ioctl` (0xc0000000001c847c, 392 bytes), starts a reorder thread, and reserves a UDP port with `is_reserve_udp_port` (0xc0000000001c74d0, 212 bytes, which calls `vsocreate_internal`); its strings say that member links are striped. It calls `snoop_input` from `is_input_snoop` and `is_output_snoop`. That it aggregates member interfaces is Inferred from those strings and routine names; the ioctl protocol was not read.

The best structural template for a tun-style device is `if_gif.o`, read from the object. `gifattach` allocates the per-unit records, sets the name and unit, the type byte 0x39, the MTU 0x500, `if_flags` 0x8000010, `if_output` to `gif_output` and `if_ioctl` to `gif_ioctl`, registers an encapsulation handler with `encap_attach_func`, and calls `if_attach`. The object imports `network_input` and `cap_able`. Whether bit 0x10 of the flags is the point-to-point flag is Inferred from the BSD flag value.

The serial-line objects show the STREAMS-plus-`ifnet` form: `if_sl.o` defines `sl_open`, `sl_wput`, `sl_rput`, `sl_wsrv`, `sl_rsrv`, `sl_output`, `sl_ioctl` and imports `if_attach`, `rawif_attach`, `snoop_input`, `network_input`, `in_rtinit`, `if_down` and `if_dropall` next to `putnext`, `qreply`, `allocb`, `canput`. `if_ppp.o` has the same shape plus `ppp_output4` and `ppp_output6` and imports `ndp6_ifinit`. A module of this kind creates its interface when it is opened on a stream and receives packets by calling `network_input`. Established for the symbols; the sequence is Inferred.

Why `if_sl` and `if_ppp` are absent from the installed kernels: the 6.5.22 IP35 system file lists `USE: if_sl` and `USE: if_ppp`, yet neither the 6.5.22 nor the 6.5.30 linked kernels contain their code, while the objects ship in the boot directory. The master files give both the flags `ofms` with no dynamic-load flag. Unknown which condition leaves them out (section 6).

### 1.8 How STREAMS modules are pushed

`fmodsw` (0xc00000000154a3b8, 992 bytes) is an array of 31 slots of 32 bytes: a 9-byte name, a pointer to a stream table (`streamtab`: read and write `qinit`, mux read and write `qinit`) and a pointer to a flag word. The global `fmodmax` is 31 and `fmodsw_extra` is 20: eleven slots are filled by the link step (`stty_ld`, `cn`, `alp`, `connld`, `pipemod`, `ptem`, `ldterm`, `tirdwr`, `timod`, `keyboard`, `mouse`) and twenty are free. A `qinit` is 56 bytes: put, service, open, close, admin routines, then `module_info` and `module_stat` pointers. Established.

`fmfind` (0xc000000000216978, 180 bytes) scans the `fmodmax` slots comparing names byte by byte, with reference counting through `fmhold` and `fmrele`. `strdrv_push` (0xc0000000002134c0, 264 bytes) calls `fmfind` and then `qattach` (0xc000000000213fb8, 756 bytes), which allocates a queue pair (`allocq`), initialises it (`setq`), and calls the module's open routine through the pointer at +16 of the queue's `qinit` (the load at 0xc000000000214144). `strioctl` (0xc00000000020c3f4, 21172 bytes) calls `fmfind` at 0xc00000000020f01c and 0xc00000000020f480 and `qattach` at 0xc000000000210c0c, and compares its command with 0x5302 at 0xc00000000020c850; the numbering is the System V STREAMS one, so the push command is that path (Inferred from the public numbering). `strdrv_push` is also the in-kernel route used by the console, pseudo-terminal and keyboard code. Established for the calls, Inferred for the command name.

STREAMS modules can also be loaded at run time: the loader has a STREAMS kind (kind 2, `str_load`, `str_reg`) reached through selector 69 ([kernel-tracing-and-modules.md](kernel-tracing-and-modules.md#module-kinds)), and the 6.5.22 master files give the dynamic-load flag to the STREAMS modules `kbd` and `ptrmap`. A loaded module fills a free `fmodsw` slot (twenty are free); that `str_reg` is what writes the slot was not read. Established for the kinds and flags, Inferred for the slot.

## 2. Judging a tun-style device

What the binaries support, and no more. The kernel has no tun or tap device, but it has everything such a device is built from: an `ifnet` with `if_output` and `if_ioctl` slots, `if_attach`, an `rawif_attach` plus `snoop_input` pair for capture, a `network_input` entry that any driver may call, and two complete examples of pseudo-interfaces (`is` linked, `gif` shipped). None of these pieces is private to one driver: `ether_attach`, `loattach`, `is_add_stripe` and `gifattach` all use the same calls. A driver can be linked in through a `cdevsw` slot and a master file (103 slots, 80 in use, the rest zero) or loaded as a driver module (loader kind 1, [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md#module-kinds)), which fills the `cdevsw` entry itself; either can call `if_attach` when it initialises. That a driver module can create an interface this way is Inferred: the driver loader and `if_attach` were both read, their combination was not.

The input contract is the part that would need more reading: the mbuf header that precedes the packet (section 1.3) is set by the driver and consumed by `ip_input`, and its exact layout is not yet documented.

## 3. File system type switch and the VFS tables

### 3.1 The switch

`vfssw` (0xc00000000154a798, 1008 bytes) is an array of 21 records of 48 bytes: name pointer, init routine, `vfsops` table, `vnodeops` table, a fill pointer and a flag. The global `nfstype` (0xc00000000163bdb4) is 16 and `vfssw_extra` (0xc00000000163c378) is 5. Slot 0 is the placeholder "BADFS" (`vfs_strayops`, `dead_vnodeops`); slots 1 to 15 hold, in order, `xfs`, `umfs`, `lofs`, `autofs`, `nfs`, `nfs3`, `udf`, `efs`, `namefs`, `fd`, `hwgfs`, `fifofs`, `proc`, `pipefs` and `specfs`; slots 16 to 20 are empty. Established.

`vfs_getvfssw` (0xc00000000028c2f4, 276 bytes) finds a type by name: after a fixed-string check it scans the slots below `nfstype` under the lock `vfsswlock` (0xc000000001645cbc). The `sysfs` call (call 1084, [syscalls.md](syscalls.md)) returns the index for a name (function 1) and, as function 3, `nfstype` minus 1. `nfstype` here is 16, the number of filled slots including slot 0. Established.

A file system type can also be added at run time. The module loader has a file system kind (kind 5, [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md#module-kinds)). `fsys_load` (0xc000000000284d70, 120 bytes) takes the loader lock, calls `filesys_check`, loads the object with `mloadfile` and calls `filesys_init` (0xc000000000284b6c, 280 bytes). `filesys_init` runs `mversion_chk`, copies the module's name into a 17-byte buffer, looks up four symbols in the module with `mfindname` ("init", "vfsops", "vnodeops" and "unload"), calls the module's init routine with a stack record, and calls `filesys_insert` (0xc000000000284adc, 144 bytes). `filesys_check` (0xc000000000284c84, 236 bytes) compares the new name with every filled slot (`strcmp`, up to the global `vfsmax`, 0xc00000000163bdb8, which is 21) and calls `fsys_alloc` (0xc0000000002849a8, 200 bytes), which scans `vfssw` under a spin lock for a free slot. `fsys_unload` (0xc000000000284de8, 184 bytes) refuses on a mounted file system (it reads the list `rootvfs`, calls `bhv_lookup_unlocked`) and calls `fsys_free`. So a file system module is a relocatable ELF object that defines `init`, `vfsops`, `vnodeops` and `unload` symbols, loaded through selector 69 into one of the five free slots (21 minus the 16 filled). The conditions from the loader apply: the kernel must have been booted from a disk kernel with a symbol table (not over the network), the caller needs the capability the selector checks, and the object must carry the matching `mversion` string. Established for the call structure and the capacity; that the record is copied into the free slot by `filesys_insert` is Inferred (the `bcopy` destination was not traced). The 6.5.22 master files give the dynamic-load flag to no file system module, so none of the shipped file systems is loadable that way.

### 3.2 `vfsops`

`vfsops` is 152 bytes (debug record). Established.

| Offset | Slot | `xfs` | `nfs3` | `lofs` | `autofs` |
|---:|---|---|---|---|---|
| 0 | position record (4 bytes, padded) | 0x00040101 | 0x00000101 | 0x00000101 | 0x00000101 |
| 8 | `vfs_mount` | `xfs_vfsmount` | `nfs3_mount` | `lofs_mount` | `auto_vfsmount` |
| 16 | `vfs_rootinit` | `xfs_rootinit` | `fs_nosys` | `fs_nosys` | `fs_nosys` |
| 24 | `vfs_mntupdate` | `xfs_mntupdate` | `fs_nosys` | `fs_nosys` | `auto_mntupdate` |
| 32 | `vfs_dounmount` | `fs_dounmount` | `fs_dounmount` | `fs_dounmount` | `fs_dounmount` |
| 40 | `vfs_unmount` | `xfs_unmount` | `nfs3_unmount` | `lofs_unmount` | `auto_unmount` |
| 48 | `vfs_root` | `xfs_root` | `nfs3_root` | `lofs_root` | `auto_root` |
| 56 | `vfs_statvfs` | `xfs_statvfs` | `nfs3_statvfs` | `lofs_statvfs` | `auto_statvfs` |
| 64 | `vfs_sync` | `xfs_sync` | `nfs_sync` | `lofs_sync` | `fs_noerr` |
| 72 | `vfs_vget` | `xfs_vget` | `nfs3_vget` | `fs_nosys` | `fs_nosys` |
| 80 | `vfs_mountroot` | `xfs_vfsmountroot` | `fs_nosys` | `fs_nosys` | `fs_nosys` |
| 88 | `vfs_realvfsops` | `fs_realvfsops` | `fs_noerr` | `fs_noerr` | `fs_realvfsops` |
| 96 | `vfs_import` | `fs_noerr` | `fs_noerr` | `fs_noerr` | `fs_noerr` |
| 104 | `vfs_quotactl` | `xfs_quotactl` | `fs_noerr` | `fs_noerr` | `fs_noerr` |
| 112 | `vfs_force_pinned` | `xfs_force_pinned` | 0 | 0 | 0 |
| 120 | `vfs_ops_magic` (int) | 0x18732739 | 0 | 0 | 0 |
| 128 / 136 | `vfs_ops_version`, `vfs_ops_flags` | 1, 2 | 0 | 0 | 0 |
| 144 | `vfs_init_vnode` | 0 | 0 | 0 | 0 |

The first word reads as the `bhv_identity` record (a 2-byte id, a version byte and a position byte; id 4 for `xfs`, version 1, position 1). Inferred from the matching debug record. `vfs` itself is 184 bytes and carries a behaviour head (`vfs_bh`) at +104; the covered vnode is at +16, the flag word at +28, the device at +32, the block size at +48, the type index at +52 and the id at +56. Established.

### 3.3 `vnodeops`: 56 operations

The debug records declare `vnodeops` only as an incomplete type, so the layout is read from the tables themselves: every table is 456 bytes, a position word at +0 and 56 function pointers at +8 to +448. The order is that of the pass-through table `vn_passthru_ops` (0xc00000000158f898), whose entries are the 56 routines `vn_passthru_vop_NAME`; the `xfs_vnodeops` table (0xc0000000015ea818) agrees with the slots [syscalls.md](syscalls.md) names (+24 read, +32 write, +56 getattr, +72 access, +208 seek). Established; the slot names are the routine names.

| Slot | Operation | Slot | Operation | Slot | Operation |
|---:|---|---:|---|---:|---|
| +8 | open | +160 | fsync | +312 | fcntl |
| +16 | close | +168 | inactive | +320 | reclaim |
| +24 | read | +176 | fid | +328 | attr_get |
| +32 | write | +184 | fid2 | +336 | attr_set |
| +40 | ioctl | +192 | rwlock | +344 | attr_remove |
| +48 | setfl | +200 | rwunlock | +352 | attr_list |
| +56 | getattr | +208 | seek | +360 | cover |
| +64 | setattr | +216 | cmp | +368 | link removed |
| +72 | access | +224 | frlock | +376 | vnode change |
| +80 | lookup | +232 | realvp | +384 | tosspages |
| +88 | create | +240 | bmap | +392 | flushinval_pages |
| +96 | remove | +248 | strategy | +400 | flush_pages |
| +104 | link | +256 | map | +408 | invalfree_pages |
| +112 | rename | +264 | addmap | +416 | pages_sethole |
| +120 | mkdir | +272 | delmap | +424 | commit |
| +128 | rmdir | +280 | poll | +432 | readbuf |
| +136 | readdir | +288 | dump | +440 | strgetmsg |
| +144 | symlink | +296 | pathconf | +448 | strputmsg |
| +152 | readlink | +304 | allocstore | | |

The slot names are the pass-through routine names without their prefix ("link removed" and "vnode change" are spelled with underscores there). The tables of IP27 and IP30 have the same 56 routine names in the same order. Established.

Dispatch goes through the behaviour chain. The vnode is 256 bytes with the behaviour head `v_bh` at +96, whose first field points at the first behaviour descriptor; a descriptor is 32 bytes: private data +0, the object +8, the operation table +16, the next descriptor +24 (debug records). A pass-through stub such as `vn_passthru_vop_open` (0xc000000000287100, 68 bytes) loads the next descriptor from +24 of its argument, takes the table at +16 and the slot at +8, skips successors whose slot holds the value kept in the global area at gp-9368 (the pass-through marker), and calls the slot with that descriptor as first argument. Established for the code, Inferred for the marker's role.

### 3.4 Which operations a file system really fills

The shared default routines in the kernel are `fs_nosys` (0xc000000000289244, 8 bytes, returns the error 89), `fs_noerr` (0xc00000000028923c, returns 0), `fs_noval` (0xc000000000289254, returns without setting a value), `fs_nodev`, `fs_cmp`, `fs_poll`, `fs_cover`, `fs_vnode_change`, `fs_setfl`, `fs_frlock`, `fs_tosspages`, `fs_flushinval_pages`, `fs_flush_pages`, `fs_invalfree_pages`, `fs_pages_sethole`, `fs_strgetmsg` and `fs_strputmsg`. The tables fill unused slots with these. Established.

`autofs_vnodeops` fills open, close, getattr, access, lookup, create, remove, rename, mkdir, rmdir, readdir, symlink, readlink, fsync, inactive, rwlock, rwunlock, seek and the attr_get, attr_set and attr_list slots; it leaves read, write, ioctl, setattr, link, fid, bmap, map and the page slots at defaults. It is the smallest real implementation. Established.

`lofs_vnodeops` fills every slot except addmap, delmap, allocstore, fcntl, commit and readbuf (`fs_nosys`), cover, link removed and the page-cache slots (defaults); its routines forward to the underlying vnode (Inferred from the names `lofs_*` and `lofs_realvp`). Established.

`nfs3_vnodeops` fills the page-cache-related slots too: bmap, strategy and `nfs3_async_commit` at commit, and takes the `fs_noerr` default for map, addmap and delmap. Established.

The set an implementation must supply to behave like a disk file system is therefore: lookup and the directory operations, getattr and setattr, open and close, read and write (or the page path: bmap, strategy, the page-flush slots), readdir, inactive and reclaim, rwlock and rwunlock, fid, and the vfs side mount, unmount, root, statvfs, sync, vget. Inferred from which slots the existing file systems use; no operation was read in full beyond the stubs.

### 3.5 `umfs`

A file system type named `umfs`, whose master file calls it the User Mode File System, occupies slot 2 of `vfssw` in every kernel and registers a system call `umfscall` (call 1228, the entry in [inventory/syscalls.tsv](inventory/syscalls.tsv)). It has no implementation. `umfs_vnodeops` (0xc0000000015eb538) points 47 of its slots at 8-byte routines, 41 of which return 89, five return 0 (rwlock, rwunlock, strategy, link removed and vnode change) and one (map) returns 19; the rest are `fs_*` defaults. `umfs_vfsops` (0xc0000000015eb700) points mount, rootinit, mntupdate, dounmount, unmount, root, statvfs, sync, vget, mountroot and quotactl at 8-byte routines that return 89; `umfs_init` returns 0. `umfscall` (0xc0000000003b64d0, 28 bytes) calls `nopkg`, which returns 65. `umfs.a` contains exactly these three objects (`umfs_entry.o`, `umfs_vfsops.o`, `umfs_vnodeops.o`) and the same code; `umfsstubs.a` is the build without the name. The facility is a reserved name with no behaviour in this release. Established. What it was meant to do is Unknown: no shipped software or published source depends on it, and no user-space part is held.

## 4. User-space file system routes

### 4.1 `automount`: a user-level NFS server that mounts itself

`usr/etc/automount` (N32, dynamically linked, symbols exported) carries an NFS version 2 server and a client for mounting. Its `main` (0x10006ae4, 2664 bytes in the file's virtual layout) does the following. Established.

1. `svcudp_create(-1)` makes a UDP RPC service on any socket and `svc_register(xprt, 100003, 2, nfs_program_2, 0)` registers it as the NFS program, version 2, with protocol 0, so it is not announced to the port mapper. The service functions are `nfsproc_getattr_2`, `setattr`, `lookup`, `readlink`, `read`, `write`, `create`, `remove`, `rename`, `link`, `symlink`, `mkdir`, `rmdir`, `readdir`, `statfs`, `root` and the write-cache procedure.
2. It looks up the file system type index with `sysfs` (function 1, the string "nfs"), forks, and then fills a 16-byte socket address at 0x10042370: family 2, the port from the transport (also saved in `myport`) and the address word 0x7f000001. A pointer to it goes into the argument block on the stack, together with the values 5, 0x4038 and 0x6038 and two timeouts.
3. It calls `mount` with the same path as the first two arguments, the flags value 5, the index as file system type, a data block of 0x84 (132) bytes and that length. The option text it records for the mount table is `ro,intr,port=%d,map=%s,%s`.

So the kernel NFS client (`nfs_vfsops`, version 2) mounts a directory from a server in a user process at the loopback address and an explicit port. The mount path in the kernel copies the 132-byte block (`nfs_mount`, 0xc0000000003caa3c, 652 bytes, calls `bsd_copyin`, `nfs_loadargs` and `nfs_processargs`; `nfs_loadargs` itself calls `bsd_copyin` three times and `copyinstr`). The per-name directories the program serves come from maps read by `loadmaster_file`, `loadmaster_yp`, `getmapent` and its host and password variants. For mounts of real remote servers the same program speaks the mount protocol with `clntudp_create` (programs 391004 and 100005) and `pingmount`, `nfs_cast` and `nfsmount`. The meaning of the flags value 5 (the mount call's flag bits) is Inferred from the published `mount(2)` description (read-only and data-pointer bits); the data block's field layout was not read. That the mount succeeds at run time was not observed: the guarantee is only that the shipped program is built to do it.

`nfs_processargs` (0xc0000000003ca674, 968 bytes) calls `ifa_ifwithaddr` on the server address, so the client tests whether the server is one of the host's own addresses; one branch ORs bit 0x10000 into the word at +0x130 of the mount record when a fourth argument equals 0x801. What the bit does is Unknown (section 6).

### 4.2 Other server-side and loopback pieces

The kernel NFS server is started by `nfs_svc` (call 1119) and the program `nfsd`; `exportfs` (call 1122) feeds the export list; `rpc.mountd`, `rpc.lockd` and `rpc.statd` are held in `usr/etc`. Established for presence; behaviour is in [syscalls.md](syscalls.md).

`lofs` is the kernel loopback file system: it mounts another path of the same host over a directory and forwards every operation to the underlying vnode. It is in the kernel (slot 3 of `vfssw`), is not a user-space server, and needs no daemon. Established.
- `autofs` exists in the kernel (slot 4 of `vfssw`, section 3.4) with no daemon in `root`; the string "autofs" does not occur in the `automount` program above. The user-space part of the kernel `autofs` is Unknown (section 6).

### 4.3 What this means for a FUSE-like layer

One route works with the shipped kernel and a second needs code in the kernel, which the module loader can load without a relink (section 3.1).

**User-level NFS version 2 server on loopback (exists).** The `automount` pattern is built to work this way: a process serves version 2 over UDP, and `mount` with the `nfs` type to 127.0.0.1 and an explicit port attaches it. Limits that follow from the protocol and from the program, not from the kernel: version 2 handles of 32 bytes, 32-bit offsets and sizes, and the client's caching. Inferred; only the mount call and the server registration were read.

**Kernel file system module that forwards to a user daemon (needs new kernel code).** The 56-operation `vnodeops` table, the 19-slot `vfsops` and the behaviour chain are what a type must supply, and the loader has a file system kind that takes an ELF object defining `init`, `vfsops`, `vnodeops` and `unload`, with five free `vfssw` slots (section 3.1). The new code forwards each operation to a daemon through a character device or a system call. `umfs` is an empty slot of that kind. Inferred; a module was not built or loaded.

**The page cache side.** The page slots (tosspages, flushinval_pages, flush_pages, invalfree_pages, pages_sethole, bmap, strategy, map, addmap, delmap) have `fs_*` defaults, so a file system that serves only name, attribute and read/write operations could leave them at defaults, as `autofs` does for all of them; memory-mapped files, block-mapped I/O and read-ahead would need real versions. Inferred.

## 5. IP27 and IP30

The `ifnet`, `mbuf`, `vfs`, `vnode`, `vfsops`, `vfssw`, `fmodsw`, `streamtab` and `qinit` debug records are byte-identical in the three kernels. The sizes of `ip_output`, `network_input`, `netproc`, `ether_attach`, `if_attach`, `rawif_attach`, `snoop_input2`, `looutput`, `loattach`, `strdrv_push`, `fmfind`, `vfs_getvfssw`, `umfscall` and the stubs `ipfilter_kernel` and `gifattach` are equal in all three. `ether_input` has the same size (1136) on IP27 and IP30 as on IP35. Established.

IP27 has no Xframe driver (no `if_xframeopen` symbol and no `xframe_open` debug record); IP30 has none either. The cdevsw open routines `if_tgopen`, `if_egopen` and `if_efopen` are present on all three. Established.

IP27's `vfssw` has 18 slots (864 bytes) with `nfstype` 13: it lacks `lofs`, `autofs` and `nfs3`. IP30 has 21 slots, `nfstype` 16, and the list read from its table is the same as IP35's. `vfssw_extra` is 5 on all three, so IP27 has five spare slots too. Established.

`fmodsw` is identical in the three kernels (31 slots, 11 filled, `fmodsw_extra` 20). Established.
- Address spaces differ: IP27 and IP35 kernels are at 0xc000... and IP30's at 0xa800.... The tables above are by name and offset.

## 6. Unknowns

Each is recorded in [open-questions.md](open-questions.md) with the route tried and the next route.

- Which condition leaves `if_sl`, `if_ppp`, `ppp_fram` and `ipfilter` out of the installed kernels, and whether a stock configuration links them.
- The layout of the header a driver leaves in front of a packet before `network_input` and `ip_input`.
- The code a module would install in `ipf_process`, and the filter daemon's rule syntax (the latter is recorded in [network-and-snapshots.md](network-and-snapshots.md)).
- How a pseudo-interface is created from user space (the case list of `ifioctl`, 0xc0000000001b2ac0, 2672 bytes).
- What the file system module's `init` routine is called with, and what `fsys_load` requires of the `vfsops` and `vnodeops` symbols beyond their presence.
- The effect of the local-server flag in `nfs_processargs`.
- The kernel `autofs` daemon and the user-space half of `umfs`.

## 7. Routine index

The routines of the sections above that are checked mechanically against the kernel, all in `6.5.30/unix.IP35`.

- `ether_input` (`6.5.30/unix.IP35`, 0xc0000000001c29f8, 1136 bytes) receives a frame from a driver; calls `snoop_input2`, `snoop_input`, `drain_input`, `dlsap_find`, `mfethermatch` and `network_input`. Established.
- `network_input` (`6.5.30/unix.IP35`, 0xc0000000001bfcf8, 340 bytes) queues a packet on the receiving CPU's protocol thread; calls `mutex_spinlock`, `mutex_spinunlock` and `m_freem`; returns 0, or 132 when the queue is full. Established.
- `network_input_unqueued` (`6.5.30/unix.IP35`, 0xc0000000001bfe4c, 108 bytes) calls the queue's handler in the caller's context; returns 0, or 124 when no handler is set. Established.
- `network_input_setup` (`6.5.30/unix.IP35`, 0xc0000000001bfc9c, 92 bytes) stores a handler for a queue number; calls `cmn_err_tag` for a number out of range. Established.
- `netproc` (`6.5.30/unix.IP35`, 0xc0000000001bff44, 268 bytes) the protocol thread body; calls `process_network_callouts`, `mutex_spinlock`, `mutex_spinunlock` and `m_freem`. Established.
- `netisr_init` (`6.5.30/unix.IP35`, 0xc0000000001c011c, 344 bytes) creates the protocol threads; calls `init_max_netprocs`, `kmem_zalloc` and `xthread_setup`. Established.
- `ip_init` (`6.5.30/unix.IP35`, 0xc0000000001bc3bc, 412 bytes) registers `ip_input`; calls `network_input_setup`. Established.
- `arp_init` (`6.5.30/unix.IP35`, 0xc0000000001ba5b8, 32 bytes) registers `netproc_arpinput`; calls `network_input_setup`. Established.
- `raw_init` (`6.5.30/unix.IP35`, 0xc0000000001ab244, 136 bytes) registers `raw_protocol_input`; calls `network_input_setup`. Established.
- `ip_input` (`6.5.30/unix.IP35`, 0xc0000000001bc79c, 2740 bytes) calls `ipfilter_kernel`, `ip_dooptions`, `ip_reass` and `ip_forward`. Established.
- `ip_output` (`6.5.30/unix.IP35`, 0xc0000000001a2a54, 3664 bytes) sends through the interface's `if_output` slot at +0x68. Established.
- `ether_attach` (`6.5.30/unix.IP35`, 0xc0000000001c2778, 432 bytes) sets up an Ethernet interface; calls `if_attach`, `rawif_attach`, `mfnew` and `st_et_attach`. Established.
- `ether_output` (`6.5.30/unix.IP35`, 0xc0000000001c2e68, 1140 bytes) calls `arpresolve`, `ndp6_resolve`, `looutput`, `m_copy` and `route_valid`. Established.
- `looutput` (`6.5.30/unix.IP35`, 0xc0000000001c23fc, 756 bytes) the loopback transmit routine; calls `snoop_input`, `drain_input`, `m_datacopy` and `network_input`. Established.
- `loattach` (`6.5.30/unix.IP35`, 0xc0000000001c2368, 148 bytes) sets up `loif`; calls `if_attach` and `rawif_attach`. Established.
- `if_attach` (`6.5.30/unix.IP35`, 0xc0000000001b1234, 364 bytes) links an interface into the global list; calls `init_mutex`, `spinlock_init` and `mrupdatef`. Established.
- `rawif_attach` (`6.5.30/unix.IP35`, 0xc0000000001a4b20, 328 bytes) registers an interface for raw-socket capture; calls `rawif_fixlink`, `mrupdatef` and `mrunlock`. Established.
- `rawif_lookup` (`6.5.30/unix.IP35`, 0xc0000000001a4e38, 128 bytes) finds a raw-interface record by name; calls `strncmp` and `mraccessf`. Established.
- `snoop_input` (`6.5.30/unix.IP35`, 0xc0000000001a5fe0, 44 bytes) calls `snoop_input2`. Established.
- `snoop_input2` (`6.5.30/unix.IP35`, 0xc0000000001a600c, 1208 bytes) calls `snoop_match`, `sfmatch`, `m_copy` and `raw_input`; returns 0 or 1. Established.
- `drain_input` (`6.5.30/unix.IP35`, 0xc0000000001a6540, 352 bytes) calls `raw_input`; returns 0 or 1. Established.
- `dlsap_find` (`6.5.30/unix.IP35`, 0xc0000000001c3b88, 112 bytes) looks up a registered data-link service access point. Established.
- `register_dlsap` (`6.5.30/unix.IP35`, 0xc0000000001c3bf8, 84 bytes) adds a data-link service access point; returns 0, or -2 on failure. Established.
- `is_add_stripe` (`6.5.30/unix.IP35`, 0xc0000000001c75a4, 512 bytes) creates the `is` interface; calls `is_reserve_udp_port`, `is_extract_unit` and `is_reorder_thread_start`. Established.
- `fmfind` (`6.5.30/unix.IP35`, 0xc000000000216978, 180 bytes) looks up a STREAMS module by name; calls `fmhold` and `fmrele`. Established.
- `strdrv_push` (`6.5.30/unix.IP35`, 0xc0000000002134c0, 264 bytes) pushes a named module on a stream; calls `fmfind`, `qattach` and `fmrele`. Established.
- `qattach` (`6.5.30/unix.IP35`, 0xc000000000213fb8, 756 bytes) attaches a module's queues; calls `allocq`, `setq`, `qdetach` and `fmhold`. Established.
- `vfs_getvfssw` (`6.5.30/unix.IP35`, 0xc00000000028c2f4, 276 bytes) finds a file system type by name; calls `strcmp`, `mutex_spinlock` and `mutex_spinunlock`. Established.
- `nfs_mount` (`6.5.30/unix.IP35`, 0xc0000000003caa3c, 652 bytes) calls `nfs_loadargs`, `nfs_processargs` and `bsd_copyin`. Established.
- `nfs_processargs` (`6.5.30/unix.IP35`, 0xc0000000003ca674, 968 bytes) calls `ifa_ifwithaddr`. Established.
- `umfscall` (`6.5.30/unix.IP35`, 0xc0000000003b64d0, 28 bytes) the call 1228 body; calls `nopkg`. Established.
- `fsys_load` (`6.5.30/unix.IP35`, 0xc000000000284d70, 120 bytes) loads a file system module; calls `mutex_lock`, `filesys_check`, `mloadfile` and `filesys_init`. Established.
- `filesys_init` (`6.5.30/unix.IP35`, 0xc000000000284b6c, 280 bytes) looks up a module's symbols; calls `mversion_chk`, `kern_malloc`, `strncpy`, `mfindname` and `filesys_insert`. Established.
- `filesys_check` (`6.5.30/unix.IP35`, 0xc000000000284c84, 236 bytes) checks a new type name against the table; calls `strcmp`, `fsys_alloc` and `mutex_spinlock`; returns 0, or one of the error numbers 50, 94 and 95. Established.
- `fsys_alloc` (`6.5.30/unix.IP35`, 0xc0000000002849a8, 200 bytes) finds a free `vfssw` slot; calls `mutex_spinlock` and `mutex_spinunlock`. Established.
