# Tier-2 routine readings, kudzu round 2 batch 5

- `tpu_statename` in `6.5.22/unix.IP27` at 0xc00000000005b508
  **Inferred.** The routine is 8 bytes and makes no calls. It returns the constant 0 in v0 at 0xc00000000005b508, so the facts show a fixed answer and no string. The name suggests a lookup of a TPU state name, which a bare zero return would not fit, so this is likely a stub.

- `klistio_init` in `6.5.22/unix.IP27` at 0xc00000000005b510
  **Inferred.** The routine is 8 bytes, makes no calls and returns 0 in v0 at 0xc00000000005b510. The name suggests one-time setup for the klistio facility, but no stores or calls appear, so the facts show an empty initializer.

- `klistio32` in `6.5.22/unix.IP27` at 0xc00000000005b518
  **Inferred.** The routine is 28 bytes. Its jal at 0xc00000000005b520 forwards a0 through a3 unchanged to the shared callee at 0xc0000000001a8ca8, and the return at 0xc00000000005b52c hands that callee's value back in v0. The 32-bit name suggests the entry for 32-bit klistio requests, served by a common routine that these facts do not show.

- `klistio64` in `6.5.22/unix.IP27` at 0xc00000000005b534
  **Inferred.** A 28-byte wrapper whose jal at 0xc00000000005b53c sends its arguments on to the shared callee at 0xc0000000001a8ca8, and whose return at 0xc00000000005b548 gives that value back in v0. Its name points at the 64-bit klistio entry, the twin of klistio32 at 0xc00000000005b518, but the facts cannot separate the two bodies beyond their names.

- `klistio_installed` in `6.5.22/unix.IP27` at 0xc00000000005b550
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b550. The name suggests a presence query for klistio, and a fixed 0 would fit an answer of "not installed", but the facts do not show how a caller reads that value.

- `klistio_getparams` in `6.5.22/unix.IP27` at 0xc00000000005b558
  **Inferred.** The only call is the jal at 0xc00000000005b560, which relays a0 to a3 to the shared callee at 0xc0000000001a8ca8. The return at 0xc00000000005b56c yields that callee's result in v0. The name suggests a parameter fetch for klistio, but the parameters never appear in these facts.

- `pwexit` in `6.5.22/unix.IP27` at 0xc00000000005b574
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b574. The name suggests an exit hook for the process-wait facility. A bare zero return makes it look like a no-op stub, which the facts do not confirm.

- `pwsysmips` in `6.5.22/unix.IP27` at 0xc00000000005b57c
  **Inferred.** Its single call, the jal at 0xc00000000005b584, routes the four argument registers to the shared callee at 0xc0000000001a8ca8. The return at 0xc00000000005b590 gives back that callee's v0. The name points at a MIPS process-wait system call, though the facts show no more than the forwarding.

- `postwait_installed` in `6.5.22/unix.IP27` at 0xc00000000005b598
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b598. Its name suggests a presence check for the postwait facility, the same shape as klistio_installed at 0xc00000000005b550. The facts do not show how a caller interprets the zero.

- `postwait_getparams` in `6.5.22/unix.IP27` at 0xc00000000005b5a0
  **Inferred.** A jal at 0xc00000000005b5a8 hands the arguments to the shared callee at 0xc0000000001a8ca8, and the return at 0xc00000000005b5b4 passes that result back. The name suggests a postwait parameter query, which these facts confirm only as far as the forwarding.

- `swoutput` in `6.5.22/unix.IP27` at 0xc00000000005b5cc
  **Inferred.** Only one call appears: the jal at 0xc00000000005b5d4 sends the arguments to the shared callee at 0xc0000000001a8ca8. The return at 0xc00000000005b5e0 carries back the callee's v0. The name suggests output to a swap or serial device, but nothing here reads or writes that device.

- `ip_mrouter_cmd` in `6.5.22/unix.IP27` at 0xc00000000005b5e8
  **Inferred.** The jal at 0xc00000000005b5f0 is the wrapper's one call, and it hands its registers to the shared callee at 0xc0000000001a8ca8. Control returns at 0xc00000000005b5fc with that callee's v0. The name suggests a multicast router command dispatcher, which the facts do not show.

- `ip_mrouter_done` in `6.5.22/unix.IP27` at 0xc00000000005b604
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b604. The name suggests a teardown for the multicast router, but the facts show no work being done, so this is most likely a stub that succeeds.

- `ip_mforward` in `6.5.22/unix.IP27` at 0xc00000000005b60c
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b60c. The name suggests the multicast forwarding entry point, but the facts show no forwarding and no use of its arguments, so it looks like a stub.

- `mrt_ioctl` in `6.5.22/unix.IP27` at 0xc00000000005b614
  **Inferred.** A wrapper around the shared callee at 0xc0000000001a8ca8: the jal at 0xc00000000005b61c forwards a0 onward without change, and the return at 0xc00000000005b628 gives the callee's result back. The name points at a multicast routing ioctl, but no ioctl number is read here.

- `legal_vif_num` in `6.5.22/unix.IP27` at 0xc00000000005b630
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b630 without reading its arguments. The name suggests a range check on a virtual interface number, but a fixed 0 does not show what the caller takes from it.

- `ip_mroute_slowtimo` in `6.5.22/unix.IP27` at 0xc00000000005b638
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b638. The name suggests the periodic timer for multicast routing, but the facts show no timer work, so it reads as a stub.

- `ip_mroute_init` in `6.5.22/unix.IP27` at 0xc00000000005b640
  **Inferred.** The jal at 0xc00000000005b648 is the lone call and sends a0 through a3 to the shared callee at 0xc0000000001a8ca8. The return at 0xc00000000005b654 passes that callee's v0 out. The name suggests multicast routing setup, though no state is initialised in these facts.

- `ip_mrouter_get` in `6.5.22/unix.IP27` at 0xc00000000005b65c
  **Inferred.** Its jal at 0xc00000000005b664 hands the arguments to the shared callee at 0xc0000000001a8ca8 unchanged, and the return at 0xc00000000005b670 returns that value. The name suggests a read of multicast router options, but the option codes are not in the facts.

- `ip_mrouter_set` in `6.5.22/unix.IP27` at 0xc00000000005b678
  **Inferred.** A 28-byte forwarder: the jal at 0xc00000000005b680 relays a0 to a3 to the shared callee at 0xc0000000001a8ca8, and the return at 0xc00000000005b68c yields that callee's v0. The name suggests a write of multicast router options, the counterpart of ip_mrouter_get at 0xc00000000005b65c, but the facts show no option being set.

- `ip_rsvp_vif_init` in `6.5.22/unix.IP27` at 0xc00000000005b694
  **Inferred.** The jal at 0xc00000000005b69c forwards a0 to a3 to the shared callee at 0xc0000000001a8ca8, and the return at 0xc00000000005b6a8 gives back its value. The name suggests RSVP virtual interface setup, which no fact here shows.

- `ip_rsvp_vif_done` in `6.5.22/unix.IP27` at 0xc00000000005b6b0
  **Inferred.** A forwarder whose jal at 0xc00000000005b6b8 sends its arguments on to the shared callee at 0xc0000000001a8ca8. Its return at 0xc00000000005b6c4 passes that callee's value back. The name suggests teardown of RSVP virtual interfaces, the counterpart of ip_rsvp_vif_init at 0xc00000000005b694.

- `ipip_input` in `6.5.22/unix.IP27` at 0xc00000000005b6cc
  **Inferred.** The jal at 0xc00000000005b6d4 is the only call, and it passes the arguments to the shared callee at 0xc0000000001a8ca8; the return at 0xc00000000005b6e0 yields that callee's v0. The name points at the receive path for IP-in-IP packets, but no packet is read in these facts.

- `ip_rsvp_force_done` in `6.5.22/unix.IP27` at 0xc00000000005b6e8
  **Inferred.** Its jal at 0xc00000000005b6f0 relays the four argument registers to the shared callee at 0xc0000000001a8ca8, and the return at 0xc00000000005b6fc gives back that value. The name suggests a forced RSVP teardown, which the facts do not confirm.

- `ipfilter_kernel` in `6.5.22/unix.IP27` at 0xc00000000005b704
  **Inferred.** The routine is 8 bytes, makes no calls and returns the constant 0 in v0 at 0xc00000000005b704. The name suggests the kernel side of the IP filter, but a bare zero return shows no filtering, so this reads as a stub.
