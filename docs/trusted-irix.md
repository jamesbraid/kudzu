# Trusted IRIX (CMW / B1 / MLS): what shipped, what we hold, what it could isolate

IRIX 6.5.30 and 6.5.22; IP27 (Origin 200/2000, Onyx2) and IP35 (Origin 300/350), with the IP28-only and other-board pieces noted. This page locates and identifies the product. It reimplements nothing. Evidence is the media manifests (inst product descriptions), the shipped kernel and configuration files, and the object files on the one Trusted IRIX 6.5 disc we hold. SGI manuals, an SGI engineer's newsgroup posts and release guides were used to find names and to date things; they are cited by title and part number and are hints, not binary evidence. Grades are **established** (read in a manifest, binary or configuration file), **inferred** (reasoned, reasoning given) or **unknown**.

## Short answer

- The product is **Trusted IRIX/CMW**, inst product `trix_eoe` ("Trusted IRIX Execution Environment"). The 6.5.30 platform file calls it the "Trix B1/CMW add-on package". It is an optional, separately obtained add-on laid over a normal IRIX install, not a fork of the OS and not part of the base discs. (Read from the manifests and the platform file.)
- The last Trusted IRIX/CMW guide revision supports **6.5.22** (Security Administration Guide 007-3299-009 and Security Features User's Guide 007-3300-005, both November 2003). No later revision exists in the techpubs record. Guides for 6.5.30-era products (Resource Administration, August 2006; CXFS Administration, January 2007) still document Trusted IRIX, so it was still supported then, but no document names a "Trusted IRIX/CMW 6.5.30" distribution. The 6.5.22 point is documented; a distinct 6.5.30 title is unknown.
- We hold **one** Trusted IRIX disc for the 6.5 line: *Trusted IRIX CMW 6.5.3 Overlays, March 1999*. It carries 3 kernel objects per board for 10 boards (IP19, IP20, IP21, IP22, IP25, IP26, IP27, IP28, IP30, IP32), plus the labelling tools. It has **no IP35 build** and **no 6.5.22 or 6.5.30 trusted object**. `mac.a`, the other `mac_*` hook modules, `cap_mac_access` and an IP27/IP35 `ip_sec.a` are on no held medium for any 6.5 release, by a search of every held disc manifest.
- The multilevel ("moldy") directory view has its call site in the shipped 6.5.30 IP35 kernel: `lookuppn` calls `mac_moldy_path`, a bare-return stub, behind the global `mac_enabled`. It is a direct call, not an interposable slot, and the body that does the work (`mac.a`) is not held. Details and what a container id could do with it: "The multilevel-directory hook" below.
- Shipped Trusted IRIX gives **label-based** separation of files, devices, System V IPC, sockets and processes (a MAC subject/object model with multilevel directories). It does not give container-style namespaces (separate process-id space, mount table, host name, network stack). See "What it could isolate".

## What it was called and how it was delivered

| Fact | Evidence | Grade |
|---|---|---|
| Product name "Trusted IRIX/CMW" (Compartmented Mode Workstation); B1-class feature set; also written "Trix B1/CMW" and "TRIX" | 007-3299-009 chapter 1; platform file `irix.sm` comment, 6.5 base, 6.5.22 and 6.5.30 (6.5.30 IP35 lines 508-509) | established |
| inst product `trix_eoe`; subsystems `trix_eoe.sw.mac` and `trix_eoe.sw.tsix` on the 6.5 CMW disc (earlier IRIX 5.3 disc: `trix_eoe.trix_sw.mac`, `.cipso`, `.eag`, `.trix_man.*`) | manifests of the held discs | established |
| An installed system shows it with `versions` (product image `trix_eoe`) and `sysconf MAC` returns 1 | IRIX Admin: Backup, Security, and Accounting, 007-2862-008 (October 2005, IRIX 6.5.25) | established (manual) |
| Optional and extra-cost: "MAC is available only in the optional Trusted IRIX operating system"; standard IRIX 6.5 moved ACLs and capabilities into the base but left MAC out | 007-2862-008; a 1997 newsgroup post by an SGI engineer says MAC "stays with Trusted IRIX/CMW 6.5 only" and Trusted IRIX "is now a pretty pure addon package vs. a separate release"; a 2002 post by an SGI engineer says no-superuser mode needs "the Trusted Irix product" at extra cost | established (manual); the posts are hints |
| Installed with inst on top of IRIX; the install runs an exit operation, `etc/trix.config`, which runs `attrinit` with the MAC and capability attribute scripts (`irix.mac`, `irix.cap`) to stamp labels on every installed file. This needs XFS extended attributes, so a Trusted IRIX system is XFS-only | `trix_eoe.sw.mac` idb entry (`exitop("chroot $rbase /bin/sh /etc/trix.config")`) and the script text; 007-3299-009 "Installation Notes" says installation is "substantially similar to that of standard IRIX" and points to the release notes | established |
| Its kernel modules are enabled by `USE:` lines under that "add-on package" comment, with `macstubs`, `sesmgrstubs` and `swipestubs` linked instead when the objects are absent | `irix.sm` of 6.5 base (IP27 line 443), 6.5.22 (IP27 453, IP35 451, IP32 321), 6.5.30 (IP35 508-530, stub block 562-589) | established |
| No Trusted IRIX CD appears in the IRIX 6.5.22 update kit's list of discs (three Installation Tools and Overlays CDs, two Applications CDs) | IRIX 6.5.22 Update Guide, 007-3897-022 | established: it was separate media |
| Release cadence: guide revisions follow IRIX releases 6.5.8 (007-3299-003, May 2000), 6.5.11 (-004), 6.5.13 (-005), 6.5.19 (-006), 6.5.20 (-007), 6.5.21 (-008), 6.5.22 (-009); the 6.5.22 guide says it "supports the Trusted IRIX/CMW operating system as of release 6.5.22" | Record of Revision, 007-3299-009 | established |
| One CD set per release (a base set plus per-release overlay discs, as for IRIX itself) | the held 6.5.3 disc is an overlay: it carries only `trix_eoe`, `x_eoe`, `desktop_base`, `desktop_eoe` and `print` overlays (each in an `f` and an `m` stream, the two stream suffixes IRIX overlay products use), so the base objects were on another disc | inferred; the 6.5.22 packaging is not shown by any held file |
| Evaluation: supports the requirements for EAL3 and the Labelled Security Protection Profile; "will be evaluated" (the Common Criteria successor of the B1 rating of the IRIX 4.0.5 product, which had a published NCSC evaluation) | 007-3299-009 chapter 1 | the claim is the vendor's; no certificate is held |
| Hardware: the guide gives no board list ("contact your local SGI office" for the evaluated hardware list). Whether IP35 was a supported or evaluated platform is not stated; the 6.5.22 and 6.5.30 IP35 `irix.sm` carry the same add-on module block as IP27, IP30 and IP32 | 007-3299-009 chapter 10; platform files | IP35 build exists in the platform file: established; IP35 support: unknown |

Part numbers known: Security Administration Guide 007-3299-009; Security Features User's Guide 007-3300-005 (its "accompanying documentation" names a Release Notes document). A "Trusted IRIX Read Me First Notice" is cited by the CXFS Administration Guide (007-4016-025) without a part number. **The part number or order code of the 6.5.22 or 6.5.30 media is unknown**; no held catalogue gives one.

Related products that are not Trusted IRIX: *Commercial Security Pak* (1997, IRIX 6.2: ACLs, capabilities, Kerberos; manuals 007-3266-001 and 007-3267-001) was carved out of Trusted IRIX/CMW 6.2; in 6.5 those parts are in the base (`eoe.sw.acl`, `capability.a`). *swIPe* (`if_sw`) is an IP-layer encryption driver from the Gauntlet firewall product, not part of Trusted IRIX.

## Bill of materials

Legend: **real** = working object held; **stub-sized** = archive of the stub size; **master** = only the `master.d` module description is shipped; **-** = no copy on any held medium. All inst names are `product.subsystem`.

### The trusted-only objects

| Object | What it is | Product.subsystem | Held copy | Release | Boards |
|---|---|---|---|---|---|
| `mac.a` | MAC core (labels, dominance, label checks) | `trix_eoe.sw.mac` (inferred from the 6.5.3 CMW product, the IRIX 5.3 product has it in `trix_eoe.trix_sw.mac`) | **none for 6.5**. Real `mac.a` only in *Trusted IRIX 5.3* (IRIX 5.3 kernel) | 5.3 only | IP4, IP5, IP6, IP12, IP17, IP19, IP20, IP22 |
| `mac_autofs.o`, `mac_proc.o` | label hooks for the autofs attribute interface; process attribute interface (`mac_proc_attr_get`) | `trix_eoe.sw.mac` | real | 6.5.3 CMW overlays (both streams) | IP19, IP20, IP21, IP22, IP25, IP26, IP27, IP28, IP30, IP32 |
| `mac_xfs.o`, `mac_nfs3.o`, `mac_ipc.o`, `mac_specfs.o`, `mac_pipe.o`, `mac_fifo.o`, `mac_fd.o`, `mac_efs.o`, `mac_hwg.o`, `mac_udf.o`, `cap_mac_access.o` | label hooks for XFS, NFS3, System V IPC, device (spec) files, pipes, FIFOs, file descriptors, EFS, hwgraph, UDF; capability-aware MAC access policy | not named by any held 6.5 manifest | **-** for 6.5. 5.3 disc has `mac_efs.o`, `mac_ipc.o`, `mac_specfs.o`, `mac_access.a`; `mac_nfs.o` also in the IRIX 6.2 Unbundled Products patch for Trusted IRIX | n/a | none |
| `master.d/mac`, `mac_access`, `mac_autofs`, `mac_efs`, `mac_fd`, `mac_fifo`, `mac_hwg`, `mac_ipc`, `mac_nfs3`, `mac_pipe`, `mac_proc`, `mac_specfs`, `mac_xfs`, `cap_mac_access` | module descriptions, with dependencies (`mac` needs `eag`; `mac_proc` needs `mac,proc`; `mac_xfs` needs `mac,xfs,xfs_attr_fetch`; `mac_ipc` needs `ipc`) | `eoe.sw.base` | master (all of them in the 6.5 base) | 6.5 base | all |
| `master.d/mac_udf` | UDF label hook description | `eoe.sw.base` | master | first in the 6.5.18 overlays; present in 6.5.22, 6.5.30 | all |
| `sesmgr.a` | TSIX network session manager: label and attribute exchange (SAMP, SATMP), socket attributes | `trix_eoe.sw.tsix` | real (162,980 bytes, 83 global functions on IP27) | 6.5.3 CMW overlays | IP19, IP20, IP21, IP22, IP25, IP26, IP27, IP28, IP30, IP32 |
| `sesmgr.a` of IRIX 6.2, 6.3, 6.4 | same file name, but a 15-function stub (9,844 bytes on IP27) in `eoe.sw.unix` | `eoe` | stub-sized, not the trusted code | 6.2-6.4 | various |
| `ip_sec.a` | CIPSO IP security options ("labelled sockets") | `eoe.sw.ipgate` on the 6.5 base (Foundation 1, and the beta); IRIX 5.3 product has it in `trix_eoe.trix_sw.cipso` | real, **IP28 only** for 6.5 | 6.5 base | IP28 (5.3 disc: IP4-IP22) |
| `master.d/sesmgr`, `master.d/ip_sec` | module descriptions (`sesmgr` needs `ip_sec`) | `eoe.sw.base` | master | 6.5 base, unchanged in 6.5.22 and 6.5.30 | all |
| `macstubs.a`, `sesmgrstubs.a` | the stand-ins linked into every shipped kernel | `eoe.sw.base` | stub-sized, the objects that are linked | 6.5 base, 6.5.22, 6.5.30 | IP27 and IP35 in all three; every other board in the 6.5 base |
| `swipestubs.a`, `master.d/if_sw`, `master.d/swipestubs` | stand-ins for the swIPe driver | `eoe.sw.base` | stub | 6.5 base, 6.5.22 (IP35), 6.5.30 (IP35); IP27 6.5 base | |
| `if_sw.o`, `master.d/swipe` | swIPe driver | `gauntlet_encrypt` (Gauntlet 3.0 CD) | real but for old boards only | Gauntlet 3.0 | IP5, IP17, IP19, IP20, IP22 |

### The user-space half of the 6.5.3 CMW disc

`trix_eoe.sw.mac`: `etc/irix.mac` (the label script, 39,704 bytes), `etc/trix.config` (the exit operation), `sbin/chlabel` (setuid). `trix_eoe.sw.tsix`: `usr/etc/satmpd`. The disc also carries trusted overlays of `x_eoe` (X server subsystem `x_eoe.sw.Server` with generic and IP22NG1 servers, `xwsh`), `desktop_base`, `desktop_eoe` and `print` (`print.sw.spooler`). Nothing of this exists for IP35 or for 6.5.22 or 6.5.30.

### What the base and the overlays do ship (held)

| Item | Product.subsystem | Held in |
|---|---|---|
| MAC and label library API (`mac_get_proc`, `mac_set_proc`, `mac_get_file`, `mac_set_file`, `mac_to_text`, `mac_from_text`, `mac_valid` in libc), man pages for `mac_*`, `msen_*`, `mint_*` | libc / `eoe.man.base` | 6.5 base, 6.5.22, 6.5.30 (libc symbols in `docs/userland-symbols`) |
| `libsatmp.so` (three ABIs) and nine public headers (MAC labels, the session manager and its interface, SAMP, the IP security options, the TSIX remote-host database, attributes and SATMP) | `eoe.sw.base`, `eoe.sw64.lib`, `irix_dev.sw.headers` | 6.5 base to 6.5.30 |
| a CMW device configuration file under `etc`, the PAM service file `cmwdialog` (CMW login dialog), `etc/openssh/irix.mac` (label script for OpenSSH files) | `eoe.sw.base`, `eoe.sw.pam`, `openssh.sw.common` | the device file throughout; `cmwdialog` 6.5.22 and 6.5.30; `irix.mac` 6.5.21 on |
| `/usr/sbin/sesmgr` and `sesdaemon` | `eoe.sw.base` | 6.5.22 and 6.5.30 (documented from 6.5.9). **These are the SCSI enclosure (SES) client and daemon, not the session manager.** The IRIX 6.5.9 Update Guide (007-3897-008) introduces `sesmgr` as the "client application" of the new `sesdaemon` for Fibre Channel drive enclosures, and the 6.5.30 binary's strings are LED, bypass and alarm operations. A row of `open-questions.md` had treated this binary as the trusted session daemon; that was wrong and is corrected there. |
| `capability.a` (privilege sets), `eag.a` (extended attribute glue the MAC and ACL code uses) | `eoe.sw.base` | real, linked in 6.5.22 and 6.5.30 IP27 and IP35 |
| `acl.a`, `acl_hwg.o`, `acl_xfs.o` | `eoe.sw.acl` | real, **not linked** in the shipped kernels |
| `acl_nfs3.o` | `nfs.sw.acl_nfs` | same |
| `sat.a` (audit) | `eoe.sw.audit` | same |

### Per release and board

For the objects that only Trusted IRIX supplies. "6.5" is the 6.5 base (Foundation 1, June 1998).

| Release | IP27 | IP35 | IP28 |
|---|---|---|---|
| 6.5 base | stubs, masters, headers | no board | stubs, masters, `ip_sec.a` real |
| 6.5.3 Trusted IRIX CMW overlays | `mac_autofs.o`, `mac_proc.o`, `sesmgr.a` real | no board | same three real |
| 6.5.22 | stubs, masters (incl. `mac_udf`), `cmwdialog`; no trusted object | stubs; none | stubs; none |
| 6.5.30 | same as 6.5.22 | same as 6.5.22 | not in the sets read |

The 6.5.22 and 6.5.30 IP35 platform files carry the add-on `USE:` block (14 `mac*` modules, `sesmgr`, `ip_sec`) and the matching stub block, so the objects were selectable for IP35; that SGI shipped them for IP35 is unknown.

## Searched and held, by string

All 813 disc images reachable (the SGI CD mirror, the 6.5.30 install-tree images, the 6.5 base discs, the IP28 media) were opened; the 693 that carry an inst distribution had every product manifest read for the strings below, by file name and by subsystem. The other 120 are plain ISO9660 discs (documentation, Linux, third-party) with no inst manifests and were not searched by file list. Nothing else held (the mirrors of other sites, the artifacts and extracts trees, the 6.5.29 tarballs) has a trusted-IRIX name; the filename search of those trees found only the discs above. The only hits outside the discs were directory names in a source tree, which was not opened and is not used here. The techpubs mirror and the newsgroup archive were searched by title and string.

| String | Result |
|---|---|
| `trix` | `trix_eoe`: Trusted IRIX 5.3 disc (IRIX 5.3 kernel), Trusted IRIX CMW 6.5.3 Overlays, and patch products for 5.3 and 6.2 (`patchSG0000920`, `0001102`, `0001158`, `0001477`, `0001567`, `0002562`; subsystem `trix_eoe_trix_sw`). Absent for 6.5.22, 6.5.30 and every other 6.5.x |
| `mac` | `mac.a` and `mac_access.a` only on the 5.3 disc; six `mac_*` objects total (above); `master.d/mac*` in the 6.5 base discs; `macstubs.a` everywhere 6.5 |
| `sesmgr` | `sesmgr.a` real on the 6.5.3 CMW disc; stub-sized on 6.2, 6.3, 6.4; the `/usr/sbin/sesmgr` binary is the SES client (above) |
| `tsix` | `satmpd` (6.5.3 CMW disc); `libsatmp.so`, `t6*.h`, man pages `tsix`, `satmp` in the base |
| `cmw` | the 6.5.3 CMW disc; a CMW device configuration file and `cmwdialog` in base and overlays |
| `b1` | no trusted-IRIX hit (only the `irix.sm` comment, text inside a file) |
| `mls`, `msen`, `mint` | no object or product; only man pages (`msen_*`, `mint_*`, `mac_from_msen_mint`) in the base documentation |
| `sat` | `sat.a`, `satstubs.a`, `satd` in base and every overlay set: audit is separate from Trusted IRIX and is held |
| not trusted-related | `swipestubs.a`, `if_sw` (Gauntlet) |

Held versus absent, by release and board:

| Release | Held | Absent |
|---|---|---|
| 6.5.30 IP27, IP35 | stubs, masters, headers, libc API, `cmwdialog`, the CMW device file | every trusted kernel object, user tool and product (`trix_eoe`) |
| 6.5.22 IP27, IP35 | same | same |
| 6.5.3 IP27 (not IP35) | `mac_autofs.o`, `mac_proc.o`, `sesmgr.a`, tools | `mac.a` and the rest of the hooks |
| 6.5.3 other boards | the same three objects | the same |
| any release, IP35 | nothing | everything |
| 6.5 base IP28 | `ip_sec.a` | `mac.a`, hooks, `sesmgr.a` |

## Acquisition rows

**This exact thing (preferred)**: the *Trusted IRIX/CMW 6.5.22* distribution (the release documented by 007-3299-009), the CD or CD set that carries inst product `trix_eoe` (names modelled on the held 6.5.3 disc: `trix_eoe_6522m`, `trix_eoe_6522f`), with the CMW base set it layers on and its Release Notes and Read Me First Notice. Needed files, per board directory under `usr/cpu/sysgen/IP35boot/` and `IP27boot/`:

- `mac.a`, `mac_autofs.o`, `mac_proc.o`, `mac_xfs.o`, `mac_nfs3.o`, `mac_ipc.o`, `mac_specfs.o`, `mac_pipe.o`, `mac_fifo.o`, `mac_fd.o`, `mac_efs.o`, `mac_hwg.o`, `mac_udf.o`, `cap_mac_access.o`;
- `sesmgr.a`, `ip_sec.a`;
- the product's `.idb` files and `.sw` images (so the subsystem names, the board lists and the exit operations are read from the manifest), its `master.d` additions and the platform-file additions;
- `etc/irix.mac`, `etc/irix.cap`, `etc/trix.config`, `sbin/chlabel`, `usr/etc/satmpd` and the other trusted tools.

**Also useful**: the *Trusted IRIX/CMW* distribution for 6.5.30, if one exists (its title and part number are unknown; ask for anything titled "Trusted IRIX" dated after November 2003).

**Compatible substitute**: any Trusted IRIX/CMW 6.5.x CD set from 6.5.8 on (guide revisions exist for 6.5.8, 6.5.11, 6.5.13, 6.5.19, 6.5.20, 6.5.21) containing IP27 objects, and an IP35 build if any release has one. IP27 objects are enough to read the label code, because the IP27 and IP35 kernels share the hook call sites (documented in `process-model.md`); an IP35 object proves nothing a 6.5.x IP27 object does not, except board-specific differences. A 6.5.x object does not replace the 6.5.22 or 6.5.30 object for any claim stated for those releases; the claim stays release-specific. The Trusted IRIX 5.3 and 4.0.5 discs we hold are another kernel and are not substitutes.

**Also helpful, any release**: the Trusted IRIX Release Notes and Read Me First Notice, and the trusted `x_eoe` overlay.

## What it could isolate

Only the shipped kernel, the shipped configuration and the vendor manuals were used; no Trusted IRIX kernel past 6.5.3 is held, so nothing below is read from a 6.5.22 or 6.5.30 trusted object.

| Question | Answer | Grade |
|---|---|---|
| Does every process, file, device, pipe and socket carry a label, and does the kernel check it on access? | Yes by design: each "process, user, file, or device" has a label and "Based on the relationship between the labels ... Trusted IRIX/CMW makes access control decisions." A label has a sensitivity part (256 levels, 65,000 categories) and an integrity part (256 grades, 65,000 divisions). Subject needs to dominate the object. | established (manual); the code is not held |
| Is the hook surface in the shipped kernel? | Yes. The IP35 kernel links the `macstubs` archive (83 functions, 88 symbols) and the `sesmgrstubs` archive (59 functions). Among them are `mac_getplabel`/`mac_setplabel` (process label), `mac_vaccess`/`mac_vsetlabel` (files), `mac_msg_access`, `mac_sem_access`, `mac_shm_access` (System V IPC), `mac_hwg_*` (hwgraph), `mac_proc_attr_get`, `mac_pipe_attr_*`, `mac_fifo_attr_get`, `mac_mount`/`mac_mountroot`, and, for sockets, `sesmgr_*`, `ip_sec_*` and `siocgetlabel`/`siocsetlabel`. Each is a bare return in the shipped kernel and the credential has a `cr_mac` field. The System V IPC paths already reach the label hooks. | established (hook names and call sites: `process-model.md`) |
| Per-label view of the file system? | Partly (the call site is in the shipped kernel; see "The multilevel-directory hook" below). Multilevel directories ("moldy" directories, `mac_moldy_path`, `mac_is_moldy`, `mac_demld`, `mac_set_moldy`) put each label's files in hidden subdirectories, so two processes at different labels see different contents of the same path. Only for directories made multilevel; the rest of the namespace is one tree with dominance rules, and a subject may still see that a non-dominated subdirectory exists. | established (manual and hook names) |
| Per-label view of processes (what `ps`, signals and `/proc` show) | The guides do not say. The `mac_proc_attr_get` hook and `pcred_access` imply `/proc` entries carry labels; whether a lower-label process can see or signal a higher one is not documented in any held guide. | unknown |
| Separate System V IPC key space, separate PID space, mount table, host name or network stack per compartment? | Not in the design. IPC objects get labels, but the key space and the id tables stay global (`process-model.md`: one `msgque`, one `vshmtab`, one `vsemtab`); one hostname and one network stack; sockets get labels through CIPSO/TSIX. | inferred from the manual and the kernel tables |
| Does it stack with other shipped containment pieces? | Yes without kernel work: cpusets have a Trusted IRIX mode (`cpusetGetTrustPerm` returns the MAC label and ACLs of a cpuset's permission file; the Resource Administration guide, August 2006, covers it for 6.5.30), and capabilities and ACLs are in the base. These give per-group CPU and memory placement and privilege narrowing, but not views. | established (manual) |

**Verdict.** Without new kernel code, shipped Trusted IRIX could give **label-based compartments** (isolation of what a process may read, write and connect to, at the points the hooks cover), plus multilevel-directory views of chosen directories, plus the cpuset and capability controls. It does **not** give container-style isolation: no namespaces for process ids, mounts, IPC keys, host name or network. And it is **not obtainable today** for 6.5.22 or 6.5.30 IP35/IP27 because the kernel objects are not held. So: **no** for container-style isolation; **unknown** for label-based compartments until a trusted 6.5.x object is read.

What would settle the unknowns: the 6.5.22 `mac_proc.o`, `mac.a`, `mac_ipc.o` (what label check `ps`, signals and IPC get), and a run of a trusted kernel. Neither is possible from held media.

## The multilevel-directory hook: in the shipped kernel, and what a container id could drive with it

Question: is the per-label directory view ("moldy" directories) done by a hook that the shipped `6.5.30/unix.IP35` already calls, so that an added object could drive it, or only by a Trusted kernel? Evidence: the two held 6.5.3 IP27 objects (`mac_autofs.o`, `mac_proc.o`, disassembled with their relocations), the shipped `6.5.30/unix.IP35` and its `macstubs.a`. The real moldy code is in `mac.a` (the `mac_moldy_path` body), which is on no held medium for any 6.5 release, so the hook's inside is not read. Addresses are `6.5.30/unix.IP35`.

**Short answer.** The call site is in the shipped kernel: `lookuppn` (0xc0000000002c1698) calls `mac_moldy_path` (0xc000000000497250, a bare return in `macstubs.o`) after every path component, behind the global `mac_enabled`. It is a direct call to a link-resolved symbol, not a pointer slot. An added object could drive it only by being linked into the kernel in place of `macstubs.o`; nothing can be installed at run time through a slot. The 6.5.3 objects we hold contain no moldy logic at all.

### What the held 6.5.3 objects are (question 1)

Established from the disassembly, IP27 objects, release 6.5.3.

- `mac_autofs.o` (7,056 bytes) exports `mac_autofs_attr_get` (0xb8 bytes), `mac_autofs_attr_set` (0x90) and `mac_autofs_attr_list` (0x48). They are the extended-attribute accessors for autofs nodes and nothing else. `get` accepts only the 12-byte name `SGI_MAC_FILE` and returns a constant 8-byte label (the word `mac_low_high_lp`, the system low-high label); `set` accepts the same name and succeeds only if the supplied label, passed through `mac_add_label`, equals `mac_low_high_lp` (else error 22); `list` copies a 32-byte name table. It calls only `strlen`, `bcmp`, `bcopy` and `mac_add_label`. It installs nothing, mounts nothing and reads no process state. In the shipped kernel `lnode_flush` is the only caller of the three stubs (0xc000000000497148, 0xc000000000497150, 0xc000000000497158). So mechanism (b), an autofs-style indirect mount, is **excluded**: autofs only gets a constant label.
- `mac_proc.o` (6,896 bytes) exports `mac_proc_attr_get` (0x2a4 bytes), the `/proc` attribute reader. It accepts the names `SGI_MAC_FILE` (12 bytes) and `SGI_MAC_PROCESS` (15 bytes), takes the target process's credential through `pcred_access` under `prlock`, checks the reader with `mac_access` (mode 0x100 against the target's `cr_mac`), and returns the label (`mac_size` bytes, copied with `bcopy`; error 7 when the buffer is short). It reads the label at credential +0x20, the `cr_mac` field of `process-model.md`. No lookup or directory logic.
- Neither object contains `mac_moldy_path`, `mac_is_moldy`, `mac_demld`, `mac_unmold`, `mac_set_moldy` or `mac_chkmld_vsetlabel`; those six stubs of `macstubs.o` (0xc000000000497250, 0xc000000000497240, 0xc000000000497320, 0xc0000000004972e0, 0xc000000000497340, 0xc000000000497288) are each `jr ra; nop` (two instructions, `v0` not written). Read in the binary. Their real bodies are in `mac.a` or the file system hook modules: unknown, not held.

So mechanism (c), replacing the directory's own vnode operations with MAC versions, is **not supported** by anything held: the held hook modules carry no vnode operation vector, and the file system lookup operations of the shipped kernel (the indirect call at `lookuppn`+0x1b4, 0xc0000000002c184c) contain no `mac_*` call that touches views (`udf_dirlookup` calls `mac_udf_iaccess`, an access test). Inferred, because the `mac_xfs.o` body is not held.

The mechanism the kernel itself shows is **(a), a hook called from the generic path-name walker**. Established at the call site:

- `lookuppn` resolves a component through the vnode lookup operation (`jalr` at 0xc0000000002c184c), crosses a mount with `traverse` (call at 0xc0000000002c1894), then loads `mac_enabled` (0xc0000000002c18a8, `lw` at gp+11552) and, when it is non-zero, branches at 0xc0000000002c18ac to an out-of-line block at 0xc0000000002c1e5c. The block calls `mac_moldy_path` (call at 0xc0000000002c1e64) with: `a0` = the vnode the component resolved to, `a1` = the component name buffer, `a2` = the path-name record being consumed, `a3` = the caller's credential (from `get_current_cred`, kept at sp+312). It is not a per-directory test: it runs for every component of every walk once `mac_enabled` is set.
- The return value is three-way (0xc0000000002c1e6c to 0xc0000000002c1e80): **-1** goes to 0xc0000000002c1f24, which releases the parent vnode, loads `/` (47) and re-enters the loop top at 0xc0000000002c1738 with the resolved vnode as the current directory; **non-zero** is an error return of the walk (0xc0000000002c1774); **0** carries on (0xc0000000002c18b8: symlink and mount-point checks). The loop top re-reads the path-name record (its length at +16 and first character through the pointer at +8) and requires the current vnode to be a directory (type 2, else error 20). Reasoned from that loop, not read: the hook may rewrite the remaining path-name record and return -1 so that the walk continues inside the vnode it was given; the return code and the loop top fit that reading and no other use of -1 is visible. A rewritten record that starts with `/` would restart from the root (0xc0000000002c1748 to 0xc0000000002c174c, branch to 0xc0000000002c1940, not read further). What the real hook writes into the record is unknown.
- The same call exists in a second, unlabelled copy of the walk loop in the NFS client code (call at 0xc0000000003dd578; the symbol table attributes the address to `get_class`, whose symbol is only 0x144 bytes at 0xc0000000003dca74, so the code is a routine without its own symbol, which calls `makenfsroot`, `inet_ntop` and `KCLNT_CALL` and so looks like the NFS root mount code). `symlink` calls `mac_initial_path` (0xc000000000497258) once (call at 0xc0000000002a6c68, also behind the `mac_enabled` test at 0xc0000000002a6b78) with a pointer loaded from offset 8 of its argument structure. There is **no other caller** of any moldy stub in the kernel text: `mac_is_moldy`, `mac_demld`, `mac_unmold`, `mac_set_moldy` and `mac_chkmld_vsetlabel` have no call site (they are reached only from inside `mac.a` and the hook modules, or from the user-library calls through the `syssgi` label selectors). No `getdents`, directory-read or `getcwd` path calls a `mac_*` hook.
- `..` is a separate branch of the walk (0xc0000000002c198c to 0xc0000000002c1c2c) that compares the current directory with the thread's root (thread +0x478) and the global root; at the root the walk stays put and jumps to 0xc0000000002c18c8, **past** the hook (the hook is skipped for that component). Elsewhere `..` goes through the same lookup operation and the hook. Read for the branches; the hook's own `..` behaviour is unknown.

Grade: call site, arguments, return handling: **established**. That the hook rewrites the path-name record: **inferred**. The hook's body (how it decides a directory is moldy, how it names the per-label child): **unknown, not held**.

### The per-process key (question 2)

The hook is handed the credential, so the kernel side keys on whatever the hook reads from it; the walker itself has no label logic (no `cr_mac` read in `lookuppn`; the label comparison would be inside `mac_moldy_path`). `cr_mac` is the credential field at +0x20 (an 8-byte word that `mac_proc_attr_get` uses as the label pointer: `mac_size` and `bcopy` on it, `mac_access` on it). Inferred: `mac_moldy_path` reads the label from `cr_mac` of the credential it was given; the kernel passes nothing else per process. There is no general "pick a view by per-process tag" facility in the kernel: the kernel-side contract is "given vnode, component, path record and credential, optionally rewrite the path record and ask for a restart". The label-specific part is wholly in the hook. Established for the contract; inferred for the label use. `crdup` (0xc0000000002d1d60) and `crcopy` copy the whole credential with `bcopy` (call at 0xc0000000002d1d88) and `crfree` frees it with no `mac_*` call, so `cr_mac` is inherited as a plain word and nothing reference-counts the label (established from the three routines: none of them calls a `mac_*` function). A tag placed there would follow `fork` and `exec` like the user ids do.

### Is the hook present in the shipped kernel (question 3)

**Yes, as a bare-return stub behind a direct call. Present, gated and dormant.** Established.

- `mac_moldy_path` and `mac_initial_path` exist as 8-byte `jr ra; nop` routines in `macstubs.o` (the only member of the 6.5.30 `IP35boot/macstubs.a`) and the three call sites above are in the linked kernel. It is not compiled out.
- The gate is the one global word `mac_enabled` (0xc000000001646490, small-common data, zero in the file image). The kernel has **155 loads of it in 83 code regions (66 named routines and 17 stretches without a symbol) and no store to it**: nothing in the shipped kernel can set it, so it is set (if ever) by code in `mac.a` or the hook modules, which is where `mac_init` (listed in the kernel's init-routine table at 0xc000000001598260) lives in a Trusted build. Who sets it is reasoned, not read.
- The same word gates every other label hook, so the moldy hook cannot be switched on alone. The routines that test it include the ones for `access`, `readlink`, `symlink`, `ptrace`, `setpgid`, `cap_get`, `cap_set`, `xfs_iaccess`, `xfs_create`, `nfs_getattr`, `msgget`, `vshm_*`, `fs_cover`, `vfs_mountroot`, `syssgi` (label selectors 70 to 73, 87, 90, 91), `sysconf` (0xc0000000002b7444, which reports MAC) and `xpmem_init`, which refuses to start ("Trix system detected", `page-migration.md`). With the flag set, the stubs are called and, because they are bare `jr ra; nop`, return whatever `v0` held; a build that sets the flag has to supply working definitions for every stub those regions reach, not only the moldy ones.

### Can an added object install into it (question 4)

**Neither a function-pointer slot nor an operations vector. A direct call to a symbol resolved at link time (the `macstubs.o` member of `macstubs.a`), so an added object could take it over only by replacing that member in the kernel link.** Established that no slot exists; inferred how the link would work.

- A scan of every allocated data section of `6.5.30/unix.IP35` for 8-byte words equal to any stub address in the macstubs and sesmgrstubs range (0xc000000000496bb8 to 0xc0000000004973a8) finds seven words: `sgi_sesmgr` in the system call tables at 0xc000000001595480 (`sysent`) and 0xc000000001596ab8 (`irix5_64_sysent`); `sat_init`, `mac_init`, `acl_init`, `sesmgr_init` at 0xc000000001598210, 0xc000000001598260, 0xc0000000015983e8 and 0xc0000000015983f8 (entries of the kernel's init-routine table `sgt_ksym_static`); and `sesmgr_soreceive` at 0xc0000000016404f0. None is a hook slot: there is no word for `mac_moldy_path`, nor for any other `mac_` access routine. No interposition without relinking.
- `macstubs.a` has one member, `macstubs.o`, which defines all the stub functions and the five label words (`mac_low_high_lp`, `mac_high_low_lp`, `mac_admin_high_lp`, `mac_equal_equal_lp`, `mac_process_cred`). The master file of `macstubs` is flagged `oxu` with no stub block ("always loaded after all other modules"; `master.d/README`), and `irix.sm` lists `USE: macstubs` unconditionally. Reasoned, from archive linking and the existing note in `kernel-optional-subsystems.md`: a replacement object that defines `mac_moldy_path` ahead of the archive wins that symbol, but if it does not define every other symbol the kernel still references from `macstubs.o`, the linker pulls the whole member and the link fails with a duplicate definition. So the replacement has to be the full set (all 83 functions and the five words), either as a drop-in `macstubs.a` or as a `mac.a` that lets `lboot` leave the stubs out. Which of those `lboot` accepts was not tested (the `lboot` rule is itself only reasoned from its strings, per `kernel-optional-subsystems.md`).
- The `mac*` modules are not loadable modules: their master files carry `ox`, not the `d` flag of dynamically loadable modules. So no run-time module load reaches the call. A new kernel has to be linked (with `lboot`/`autoconfig` and the objects added to the boot directory); that is "no run-time patching" but it is not "drop a module in".
- Setting `mac_enabled` is a separate step: it has no store in the shipped kernel (above), so the replacement object's `mac_init` (in the init-routine table, already called at boot because the stub `mac_init` is in the table) would have to write it. Whether a non-trusted kernel boots cleanly with the flag set and all other hooks as inert definitions is unknown.

### Could a container id drive it (question 5)

**It is entry redirection inside one shared tree, not a private root. A container id could pick between named alternatives that already exist in the tree; it cannot hide the host's files from paths that do not go through the walker.** Graded below.

- Read: the redirection point is `lookuppn`, after each component is resolved and after any mount crossing, with the credential as input. Whatever the hook decides is applied to that walk only (the restart continues in the vnode the hook was given).
- Read: the root vnode of a walk is not itself offered to the hook: a walk starts at the thread's root or current directory and the hook runs on component results. The root directory's own entries can therefore not be hidden by the hook; only the first component down. The `..` branch at the thread's root skips the hook.
- Reasoned: a hook that, for vnodes it recognises (for example a directory carrying a marker), appends `<container id>` to the remaining path and returns -1 gives each container its own subdirectory under each marked directory, the way a moldy directory gives each label one. Under that scheme the container sees only the files under its id for those directories. That shows container files at the same path and hides the sibling container directories and the host files **that sit beside it in the marked directory**, provided the container reaches them by a path walk.
- Read by absence: no `mac_*` call in the directory-read routines or `getcwd` (the caller scan above), and a descriptor or `fchdir` target is a vnode that no walk resolves. Not redirected: an open file descriptor, `fchdir`, or the `/proc` entries. Calls that reach a vnode by file handle (the NFS server) were not traced and are unknown. A process that already holds a descriptor, or reaches a vnode other than by a path walk, is not redirected. The hook does not make a vnode unreachable; it changes where one walk goes. The host tree stays one tree with one set of vnodes and one mount list (`process-model.md`, "Namespace scope").
- Reasoned: a "private root" needs the redirect to apply to every walk from `/` and for every mount to be invisible; the hook cannot do the first (the root vnode is never offered) and does not touch the second. `chroot` (which the container tooling would use anyway) gives the tree cut, the hook can add per-container versions of chosen directories (`/tmp`, `/var`, `/etc`) inside it.
- Cost: turning the hook on needs a relinked kernel, a full definition of the 83 `macstubs` functions, and `mac_enabled` set, which turns on the hook calls of about 80 other routines (a Trusted-like side effect set: `sysconf` reports MAC, XPMEM declines to start, `syssgi` label selectors stop returning 89).

Verdict: **yes, the hook point is in the shipped kernel and its calling contract is fixed enough to build to; no, it is not a slot an added object can fill at run time, and as a mechanism it gives per-container versions of marked directories, not a hidden host or a private root.** Whether it is worth the kernel link and the global flag, compared with `chroot` plus a static tree built by a supervisor (`process-model.md`, "Mount table"), is a design choice for the container plan; the facts above are what it can and cannot do.

## Related

[kernel-optional-subsystems.md](kernel-optional-subsystems.md) (rows 2 and 3 and the gaps list), [process-model.md](process-model.md) (credential, IPC call sites and the hook stubs), [resource-control.md](resource-control.md) (cpusets and job limits), [open-questions.md](open-questions.md).
