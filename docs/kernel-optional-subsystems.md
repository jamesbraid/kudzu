# Kernel features that are stubbed, empty, absent or ship separately

IRIX 6.5.30 and 6.5.22, IP35 (Origin 300/350) and IP27 (Origin 200/2000). Evidence is the shipped objects, the installation manifests of the media and the linked kernels; manuals and the vendor's product catalogues were used only to find names. Grades are **established** (read in a binary or manifest), **inferred** (reasoned, reasoning given) or **unknown**.

## How a feature becomes a stub

The platform configuration file (`irix.sm`) names a module on a `USE` line and, further down, a companion `<name>stubs` module. `lboot` links a `USE`d module only if its object (`<name>.o` or `<name>.a`) is in the boot directory (`var/sysgen/boot`, a link to `usr/cpu/sysgen/IP35boot` or `IP27boot`); otherwise the stubs go in. A module on an `EXCLUDE` line is never linked. Some master files carry a short stub block of their own instead of a stubs archive. Stub bodies that return error 65 are `ENOPKG`, "package not installed". Sources: [sysgen.md](sysgen.md), the `master.d` README and the `lboot` strings (strings only, not disassembled). The behaviour is inferred from those plus the linked kernels; it fits every case measured below.

So "stubbed in the shipped kernel" splits into four different facts, which the table keeps apart:

1. **Optional subsystem not installed.** The object is on the media in a non-default subsystem of `eoe` (or `nfs`), and the machine that linked the kernel lacked it.
2. **Excluded by configuration.** The object may be on the media; `irix.sm` says `EXCLUDE`.
3. **Built empty.** The object exists with the right name and holds no code.
4. **Not on any disc we hold for these releases.** It ships in a different product, or in another release.

## Method

Scripted, not sampled. For each of 6.5.22 and 6.5.30 and each of IP27 and IP35:

- Parse every `*.sm` of the platform (the 6.5 base overlaid by the release's own overlay) and every master file (same overlay order) for `USE`, `INCLUDE`, `EXCLUDE` and stub blocks.
- Take every module that has a `<name>stubs` companion, is `EXCLUDE`d, is `USE`d with no object in the boot directory, has a stub block in its master file and is `USE`d, or whose object lives in an optional subsystem. That gave 183 module names over the four combinations.
- Read the object's subsystem from the manifests (`eoe_6530m`, `eoe_6522m`, the 6.5 base discs; links such as `partition.o` are resolved to their target).
- Dump all defined symbols with sizes from every boot object and from the four linked kernels. A module is "real" if a global function of the object (size over 8 bytes, name not shared with another object) is in the kernel at the same size; "stub" if the stubs archive's functions are in the kernel at the stub sizes.
- Dump the manifests of every other disc image we can reach (about 670 discs of the large third-party mirror, all 6.5.x overlay sets from 6.5.2 to 6.5.30, the Trusted IRIX, REACT, ATM and database discs) and look up every missing object by name; extract and symbol-check the hits against the stub archives.

"Held" means a copy is on a medium we can read. Row counts differ by platform and release only where the table says so.

## The table

Columns: where the stub or absence comes from; the real code and the product and subsystem that installs it; whether we hold it; whether it is linked in the shipped `unix.IP27`/`unix.IP35` (22 = 6.5.22, 30 = 6.5.30; abbreviations: **real**, **stub**, **excl** excluded, **none** neither object nor stubs, **nl** object not linked and no stubs archive). "Cfg" cites the 6.5.30 IP35 `irix.sm` line unless said.

| # | Module(s) and what they provide | Cfg | Real code, product.subsystem | Held | Linked 22 IP27/IP35, 30 IP27/IP35 | Grade |
|---|---|---|---|---|---|---|
| 1 | `cell`, `cfs`, `cxfs`, `xvmcell` (stubs only), `cxfsarr`, `grio2cell`: cellular kernel layer, cell file system, CXFS and its XVM and GRIO2 cell variants | 329, 585-586, 565, 563 | `cell.a`, `cfs.a`, `cxfs.a`, `xvm_cell.a`, debug `cfsidbg.a`, `cxfsidbg.o`, `dp_idbg.a`; product `cxfs`, subsystems `cxfs.sw.cxfs`, `cxfs.sw.xvm_cell`, selected by its own `cxfs.sm`; **6.5.6 to 6.5.21 overlay discs only, none in 6.5.22 or 6.5.30** | Not for 6.5.22/6.5.30. 6.5.21 IP35 and IP27 copies held (the mirror). `cxfs.a`/`cell.a` define 33 of 38 and 26 of 27 stub symbols; `xvm_cell.a` 12 of 12; `grio2cell` 0 of 6 (unknown definer) | stub/stub, stub/stub | established (stub symbols, manifests) |
| 2 | `mac` and 13 hook modules (`mac_nfs3`, `mac_xfs`, `mac_ipc`, `mac_specfs`, `mac_autofs`, `mac_proc`, `mac_pipe`, `mac_fifo`, `mac_fd`, `mac_efs`, `mac_hwg`, `mac_udf`, `cap_mac_access`): mandatory access control | 513-526, 567 | Product `trix_eoe` (Trusted IRIX/CMW), subsystem `trix_eoe.sw.mac`. Only `mac_autofs.o`, `mac_proc.o` (IP27) and `chlabel`, `irix.mac` survive on the 6.5.3 CMW overlay disc; `mac.a` and the other hooks are not on any disc held | Partly, 6.5.3 only. Defines 6 of 88 `macstubs` symbols | stub (`mac` only; hooks: none) in all four | established |
| 3 | `sesmgr`: labelled-network session manager; `ip_sec`: IP security option hooks | 530-531, 568 | `sesmgr.a`: `trix_eoe.sw.tsix` (6.5.3 CMW overlay, IP27 held; 39 of 59 stub symbols). `ip_sec.a`: IP28boot only, `eoe.sw.ipgate` of the 6.5 base (Foundation 1); defines label and CIPSO routines; no IP27/IP35 build anywhere | `sesmgr.a` 6.5.3 only; `ip_sec.a` IP28 6.5 base only | `sesmgr`: stub; `ip_sec`: none (all four) | established |
| 4 | `swipe`, `if_sw`: swIPe packet encryption and its interface | 451, 575 | `swipe.a`, `if_sw.o`, product `gauntlet_encrypt`, subsystem `gauntlet_encrypt.sw.gauntlet` (Gauntlet 3.0 disc: IP5, IP17, IP19, IP20, IP22 only) | IRIX 5/6.x boards only; nothing for IP27 or IP35 | IP35 stub, IP27 none; `if_sw` uses the stub block in its master file | established |
| 5 | `kaio`: kernel asynchronous I/O for database servers | 536, 573 | `kaio.o`; `eoe.sw.database_accel` (IRIX 6.2, 6.4; IP27 18416 bytes) and Database Accelerator 3.0 `dba.sw.dba` (IP19, IP25, IP27); defines all 11 `kaiostubs` symbols | IP27 copy held (6.4 and 3.0); no IP35 build | IP27 none, IP35 stub (both releases) | established |
| 6 | `klistio`: kernel list I/O | 544, 574 | `klistio.a`; `dba.sw.oracle` of Database Accelerator 3.0 (IP27 17828); 5 of 5 stub symbols | IP27 held; no IP35 | IP27 none, IP35 stub | established |
| 7 | `postwait`: post/wait driver for database servers | 540, 574 | `postwait.o`; `dba.sw.oracle` (IP27 21616); 4 of 4 stub symbols | IP27 held; no IP35 | IP27 none, IP35 stub | established |
| 8 | `frs`: frame scheduler (real-time) | 389, 563 | `frs.a`; product `react`, subsystem `react.sw.frs` (REACT 2.0 and the 6.2 unbundled discs; IP19, IP21, IP25 only) | Old releases only; no IP27, IP35 | stub, stub, stub, stub | established (manifests); symbol match not run |
| 9 | `atm`: ATM ARP domain and ioctl | 576 | `atm.a`, product `atm`, `atm.sw.eoe` (IP19, IP21, IP25; ATM-OC3c 2.2 "for IRIX 6.4 for Origin2000", 6.5 beta); neither held `atm.a` defines the two stub symbols (`atmarpdomain`, `new_atmarp_ioctl`) | Other releases only; real definer unknown | IP27 none, IP35 stub | unknown (definer) |
| 10 | `dfs`: file system call entry (`afs_syscall`) | 574 | No kernel object in any manifest. The DCE 1.1B disc (IRIX 5.3/6.2/6.3) and a 6.3 patch set whose product is named `DFS_sw` (not opened) are the only DFS material; the disc has user-space files only | No | IP27 none, IP35 stub | unknown |
| 11 | `unc` stubs (`uc_fork_notify`, `uc_exit_notify`, `caienfk`) | 574 | `uncintf.o` in `eoe.sw.unc` is held for IP27 and IP35 but does not define them; definer unknown | n/a | IP27 none, IP35 stub | unknown |
| 12 | `partition`, `xpc`, `if_cl`, `cl`: partitioning, cross-partition communication, cluster interface | 554-557, 584, 588; IP27 513-516 `EXCLUDE` | IP35: `partition_part.o`, `xpc_part.o`, `if_cl_part.o`, `cl_part.o`, `usr/sbin/partmgr`; `eoe.sw.partition`. The names `partition.o` etc. are inst links to them. IP27: full `partition.o`, `xpc.o`, `cl.o`, `if_cl.o` in `eoe.sw.base` | Yes, both | IP35: `partition`, `xpc` stub, `cl`, `if_cl` nl; IP27: excl (all four combos) | established |
| 13 | `xpmem`: cross-partition memory attach | 558, 588 | `xpmem.o`, `eoe.sw.base`, IP35 only (no IP27 object, no IP27 `USE`) | Yes (IP35) | IP35 real; IP27 none | established |
| 14 | `jlimits`: job IDs and job limits | 548, 580 | `jlimits.a`, `eoe.sw.jlimits` | Yes, both | stub in all four | established |
| 15 | `ipfilter`, `ip_mroute`: packet-filter hook for `ipfilterd`; multicast routing | 435, 439 | `ipfilter.o`, `ip_mroute.o`, `eoe.sw.ipgate` (also `usr/etc/ipfilterd`). Inline stub block in `master.d/ipfilter` (no stubs archive); `ipfilter_kernel` is the 8-byte stub in the kernels | Yes | inline stub in all four | established |
| 16 | `ipv6`: IPv6, gif tunnels, helpers | 463, 590 | `ipv6.a`, `eoe.sw.ipv6` | Yes | stub in all four | established |
| 17 | `sat`: security audit trail; `acl`, `acl_hwg`, `acl_xfs`, `acl_nfs3`: access control lists | 495, 503-506, 567-568 | `sat.a`: `eoe.sw.audit`. ACL: `eoe.sw.acl`, `acl_nfs3.o` in `nfs.sw.acl_nfs` | Yes | `sat`, `acl`: stub; the three ACL hook modules: nl | established |
| 18 | `xvm`, `tsv`: XVM volume manager | 326-327, 565 | `xvm.a`, `tsv.a`, `eoe.sw.xvm` (user side `sysadm_xvm`, `/sbin/xvm`) | Yes | `xvm` stub; `tsv` (6.5.30) nl | established |
| 19 | `xlv`, `xlv_plex`: XLV volume manager and plexes | 306, 310, 564, 569 | `xlv.a`: `eoe.sw.xlv`; `xlv_plex.a`: `eoe.sw.xlvplex` | Yes | IP35 stub; IP27 nl (no stubs file) | established |
| 20 | `quotas`, `xfsquotas`; `dmi`, `xfsdmapi`; `grio`, `grio2`, `xfsrt` | 368-376, 566 | `eoe.sw.quotas`; `eoe.sw.dmi`; `grio.a`, `xfsrt.a`: `eoe.sw.xfsrt`; `grio2.a`: `eoe.sw.grio2` | Yes | stub (IP27 quotas: nl) | established |
| 21 | `stp`, `tpisocket`, `sockmod`, `ticlts`, `ticots`, `ticotsord`, `tpitcp`, `tpiudp`, `tpiicmp`, `tpirawip`, `tpitcp6`, `tpiudp6`: STP transport, TPI sockets, TLI helpers | 393-402, 455 | `eoe.sw.stp`, `eoe.sw.svr4net` | Yes | `stp`, `tpisocket` stub; the others nl | established |
| 22 | `if_sl`, `if_ppp`: SLIP and PPP | 443, 447 | `eoe.sw.slip`, `eoe.sw.ppp` | Yes | not testable (only local symbols); inferred nl | inferred |
| 23 | `cachefs`, `umfs`: cache file system; reserved user-mode file system | 361, 374, 566, 572 | `cachefs.a`: `nfs.sw.cachefs`; `umfs.a` (three objects) is in `eoe.sw.base` with a stubs archive | Yes | stub in all four | established |
| 24 | `optinput` devices: `calcomp`, `dial`, `dialbox`, `hitachi`, `imp`, `magellan`, `sball`, `tablet`, `wacom` | no stubs | `eoe.sw.optinput` | Yes | nl | established |
| 25 | NFS pieces: `nlm` (stubs), `lockd`, `lofs`, `autofs`, `snfs`, `nfs3`, `rpcsec` | 357-364, 572 | `nfs.sw.nfs`, `nfs.sw.autofs`, `nfs.sw.rpcsec` | Yes | 22 and 30/IP35 real except `nlm` stub; **30/IP27 links none of them** (stubs or nl): the IP27 6.5.30 kernel came from a build without the NFS client | established; cause inferred |
| 26 | `idbg` and 36 `*idbg` debug modules, `kmem_debug`, `ksync_statistics` | 804-821, 595-602, 670-691 | `idbg.o` and companions in `eoe.sw.kdebug` (some in `eoe.sw.base`, `nfs`, `xlv`, `xvm`); lock statistics are `ksync_statistics.a` in `eoe.sw.base`, chosen by editing `irix.sm` | Yes | excl | established |
| 27 | Hardware stubs: `crime`, `gr2`, `ng1`, `mgras`, `mgv`, `vid`, `dn`, `gsn` (IP27), `tport`, `gfx`, `odyssey`, `vga`, `vme` | 562-573 | Objects live in other board directories (IP30, IP32) or in `eoe.sw.gfx`; `dn` (DECnet) and `vid` (video) have no definer | Partly | stub or none | established |
| 28 | `shareII`, `kitchen` | master only | `ShareII` product (`ShareII.sw.base`, ShareII 2.4 disc and 6.2 unbundled): scheduler, not a 6.5.22/30 kernel object | Old releases | none | inferred |
| 29 | `device` stubs (`setnoderun`, `restorenoderun`) | 573 | defined in `kernel.o` (linked real) | Yes | n/a | established |

`cpr`, `crypto`, `fru`, `kona`, `efs`, `failover` and the other stub-carrying modules not listed are linked real in all four kernels.

### Built empty

Relocatable objects with the same name as a working IP27 object but no code (1.2 to 1.4 KB, no `.text`, no defined symbols), 6.5.30 IP35 `numa.a`: `migr_bounce.o`, `migr_interrupt.o`, `migr_manager.o`, `migr_queue.o`, `migr_refcnt.o`, `migr_states.o`, `migr_unpegging.o` and `pfms.o`. IP35 also has empty `fcidbg.o`, `umfsidbg.o`, `odyssey.a(odsy_idbg.o, odsy_sim.o)`, `vga.a(vga_idbg.o)`, `frustubs.o`, and `xvm.a(xvm_palio_noncell.o)` in 6.5.30 (empty on both platforms). Empty on both platforms in both releases: `io.a(eisa.o, idbgactlog.o, vdma.o)`, `os.a(mtext.o, tile.o)`, `xfs.a(xfs_dir2_trace.o, xfs_macros.o)`, `udf.a(udf_idbg.o)`, `qlfc.a(ql2100_fw.o)`, `gfxs.a(gfx_debug.o)`. In 6.5.22 also empty: `grio2.a(grio2_misc.o)`, `numa.a(mem_profiler.o, repldebug.o)`. IP27's `mgras.a(mgras_pipeend.o)` is empty. Established, from `ar` and `nm`. Automatic page migration is therefore not built on IP35 ([page-migration.md](page-migration.md)); IP27 builds it.

## Gaps and where the real code would come from

Everything marked "held" above is extractable now. The remaining gaps, each with the nearest copy we can reach. "The mirror" below is the public SGI CD collection kept as part of the held software (about 790 images); discs are named as they are titled there.

- **CXFS (rows 1).** Nearest: the `IRIX 6.5.21 Overlays (4 of 4)` disc, product `cxfs` (kernel), `cxfs_client`, with cluster services `cluster_admin`, `cluster_control`, `cluster_services`, `sysadm_cluster`, `sysadm_cxfs`. The cluster user-space products are also on the held 6.5.22 and 6.5.30 Overlays 3, so only the kernel product is missing for those releases; the 6.5.22 and 6.5.30 discs ship no `cxfs` product at all. Request: the CXFS CD or CD set distributed for IRIX 6.5.22 and for 6.5.30 (product `cxfs`, subsystems `cxfs.sw.cxfs` and `cxfs.sw.xvm_cell`; `cxfs_client.sw.base` for the client), or the 6.5.22 and 6.5.30 CXFS release patch set. The 6.5.21 IP35 objects are 6.5.21, not 6.5.22: sizes and symbols may differ (not compared, because no 6.5.22 copy exists). No held file shows the name of the later separate distribution.
- **Trusted IRIX (rows 2-3).** Request: Trusted IRIX/CMW for 6.5.22 and 6.5.30, product `trix_eoe`, subsystems `trix_eoe.sw.mac` and `trix_eoe.sw.tsix`, including the base kernel objects `mac.a`, `mac_xfs.o`, `mac_efs.o`, `mac_nfs3.o`, `mac_ipc.o`, `mac_specfs.o`, `mac_pipe.o`, `mac_fifo.o`, `mac_fd.o`, `mac_hwg.o`, `mac_udf.o`, `cap_mac_access.o`, `sesmgr.a` and `ip_sec.a` for IP27 and IP35. Nearest held: the 6.5.3 CMW overlay disc (IP27: `mac_autofs.o`, `mac_proc.o`, `sesmgr.a`; user `satmpd`, `chlabel`, `irix.mac`) and the IP28 `ip_sec.a` of the 6.5 base. The CMW base discs for 6.5.3 (needed for `mac.a`) are not mirrored. Older Trusted IRIX/B 4.0.5 and Trusted IRIX 5.3 discs are held but are other kernels.
- **swIPe (row 4).** Request: a Gauntlet (product `gauntlet_encrypt`) release for IRIX 6.5 that builds for IP27 and IP35. Nearest: Gauntlet 3.0, IP5 to IP22 only.
- **kaio, klistio, postwait (rows 5-7).** Request: Database Accelerator for IRIX 6.5.22/6.5.30 on IP35 (product `dba`, subsystems `dba.sw.dba` and `dba.sw.oracle`), or the later `eoe` overlay that carried `eoe.sw.database_accel` for IP35. Nearest: Database Accelerator 3.0 "for IRIX 6.5" with IP27 objects of all three, and the 6.4 `eoe` IP27 `kaio.o`.
- **frs (row 8).** Request: REACT/Pro for IRIX 6.5 (product `react`, `react.sw.frs`) with IP27 and IP35 objects. Nearest: REACT 2.0 (IP19, IP21) and the 6.2 REACT build (IP25). The marketing name of the 6.5 release is not shown by any held file.
- **atm (row 9).** Request: the ATM driver product for IRIX 6.5 on Origin (product `atm`, `atm.sw.eoe`), release newer than 2.2. The two held builds do not define the stub symbols, so even the right release is unconfirmed.
- **dfs (row 10).** Request: DCE/DFS for IRIX 6.5 including the kernel DFS client module (product `dce`); none of the held DCE material has a kernel object.
- **unc, grio2cell, vid, dn (rows 11, 1, 27).** No object in any held manifest defines their stub symbols. Source product unknown; nothing to request until a named product is found.
- **umfs (row 23).** The three objects are in `eoe.sw.base` and the stub is the only alternative; no other object defines its symbols on any disc. Nothing missing.

## Searched and not found

Every manifest dumped from the 6.5.2 to 6.5.30 overlay sets, the 6.5 base and beta discs, and about 670 other SGI discs was searched by object name for each missing module. The other held software trees (the 6.5.29 tarballs, the 6.5.30 install tree, the mirrors, the artifact and extract directories) hold no other copy: the 6.5.30 install tree duplicates the discs already read, and the source-tree directories were not opened. The techpubs mirror and release-note directories had no product catalogue naming a later CXFS, Trusted IRIX or REACT release for these two. The Library search services were not reachable from this session (connection refused on the default URL, empty results through the LAN URL), so no negative is claimed from them.

## Related

[sysgen.md](sysgen.md), [process-model.md](process-model.md), [resource-control.md](resource-control.md), [network-and-snapshots.md](network-and-snapshots.md), [network-dataplane-and-vfs.md](network-dataplane-and-vfs.md), [page-migration.md](page-migration.md), [kernel-tracing-and-modules.md](kernel-tracing-and-modules.md), [checkpoint-restart.md](checkpoint-restart.md), [open-questions.md](open-questions.md).
