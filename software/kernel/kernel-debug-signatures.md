# Kernel routine signatures from debug information

Each entry gives what the IRIX 6.5.30 debug information records for one routine: its return type and its formal parameters. The address and size come from the symbol table of the same binary. The entry names the debug record it was read from, by unit and offset, so a reader can find it in the binary.

These entries are located facts only. The record names a signature but no role, so every entry says "Role unknown". A routine's role needs a body read.

## What the debug information holds

- Return types and parameter lists come from the `.debug_info` section (DWARF) of each binary. A return type is the `DW_AT_type` of a subprogram record. A parameter is a formal-parameter child of one.
- Almost no routine has parameter records. The 6.5.30 kernel objects and linked kernels carry parameter records only for the `idbg` debugger objects and the `mloadidbg` object of each platform. No 6.5.30 object or linked kernel has a `.mdebug` section, so there is no ECOFF symbol data to read in its place.
- The linked kernels carry return types for a few routines. The IP35 kernel has 40, the IP30 kernel 40 and the IP27 kernel 62. Most of them are eight-byte routines that return `int`.
- A routine with neither a return type nor parameter records has no signature here. The record does not say that such a routine takes no arguments or returns nothing.
- A parameter type of `function` marks a pointer to a subroutine type, whose own signature the record does not give.

## Linked kernels

### `6.5.30/unix.IP35`

- `_check_dbg` (`6.5.30/unix.IP35` `0xc000000000083c3c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d0c of unit 134. Role unknown.
- `tpusim_version` (`6.5.30/unix.IP35` `0xc000000000083c44`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d40 of unit 134. Role unknown.
- `tpusim_info_chk` (`6.5.30/unix.IP35` `0xc000000000083c4c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d78 of unit 134. Role unknown.
- `tpusim_piotrans_addr` (`6.5.30/unix.IP35` `0xc000000000083c54`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10db1 of unit 134. Role unknown.
- `tpusim_dmatrans_list` (`6.5.30/unix.IP35` `0xc000000000083c5c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10def of unit 134. Role unknown.
- `tpusim_intr_alloc` (`6.5.30/unix.IP35` `0xc000000000083c64`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e2d of unit 134. Role unknown.
- `tpusim_intr_connect` (`6.5.30/unix.IP35` `0xc000000000083c6c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e68 of unit 134. Role unknown.
- `tpuidbg_version` (`6.5.30/unix.IP35` `0xc000000000083c7c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10edd of unit 134. Role unknown.
- `tpu_statename` (`6.5.30/unix.IP35` `0xc000000000083c8c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f45 of unit 134. Role unknown.
- `klistio_init` (`6.5.30/unix.IP35` `0xc000000000083c94`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f7c of unit 134. Role unknown.
- `klistio32` (`6.5.30/unix.IP35` `0xc000000000083c9c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10fb2 of unit 134. Role unknown.
- `klistio64` (`6.5.30/unix.IP35` `0xc000000000083cb8`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10fe5 of unit 134. Role unknown.
- `klistio_installed` (`6.5.30/unix.IP35` `0xc000000000083cd4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11018 of unit 134. Role unknown.
- `klistio_getparams` (`6.5.30/unix.IP35` `0xc000000000083cdc`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11053 of unit 134. Role unknown.
- `pwexit` (`6.5.30/unix.IP35` `0xc000000000083cf8`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1108e of unit 134. Role unknown.
- `pwsysmips` (`6.5.30/unix.IP35` `0xc000000000083d00`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x110be of unit 134. Role unknown.
- `postwait_installed` (`6.5.30/unix.IP35` `0xc000000000083d1c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x110f1 of unit 134. Role unknown.
- `postwait_getparams` (`6.5.30/unix.IP35` `0xc000000000083d24`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1112d of unit 134. Role unknown.
- `swoutput` (`6.5.30/unix.IP35` `0xc000000000083d50`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x111ba of unit 134. Role unknown.
- `ip_mrouter_cmd` (`6.5.30/unix.IP35` `0xc000000000083d6c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x111ec of unit 134. Role unknown.
- `ip_mrouter_done` (`6.5.30/unix.IP35` `0xc000000000083d88`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11224 of unit 134. Role unknown.
- `ip_mforward` (`6.5.30/unix.IP35` `0xc000000000083d90`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1125d of unit 134. Role unknown.
- `mrt_ioctl` (`6.5.30/unix.IP35` `0xc000000000083d98`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11292 of unit 134. Role unknown.
- `legal_vif_num` (`6.5.30/unix.IP35` `0xc000000000083db4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x112c5 of unit 134. Role unknown.
- `ip_mroute_slowtimo` (`6.5.30/unix.IP35` `0xc000000000083dbc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x112fc of unit 134. Role unknown.
- `ip_mroute_init` (`6.5.30/unix.IP35` `0xc000000000083dc4`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11338 of unit 134. Role unknown.
- `ip_mrouter_get` (`6.5.30/unix.IP35` `0xc000000000083de0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11370 of unit 134. Role unknown.
- `ip_mrouter_set` (`6.5.30/unix.IP35` `0xc000000000083dfc`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x113a8 of unit 134. Role unknown.
- `ip_rsvp_vif_init` (`6.5.30/unix.IP35` `0xc000000000083e18`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x113e0 of unit 134. Role unknown.
- `ip_rsvp_vif_done` (`6.5.30/unix.IP35` `0xc000000000083e34`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1141a of unit 134. Role unknown.
- `ipip_input` (`6.5.30/unix.IP35` `0xc000000000083e50`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11454 of unit 134. Role unknown.
- `ip_rsvp_force_done` (`6.5.30/unix.IP35` `0xc000000000083e6c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11488 of unit 134. Role unknown.
- `ipfilter_kernel` (`6.5.30/unix.IP35` `0xc000000000083e88`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x114c4 of unit 134. Role unknown.
- `rpc_gss_secget` (`6.5.30/unix.IP35` `0xc000000000083eb0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x115bc of unit 134. Role unknown.
- `__svcrpcsec_gss` (`6.5.30/unix.IP35` `0xc000000000083ecc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x115f4 of unit 134. Role unknown.
- `rpc_gss_set_svc_name` (`6.5.30/unix.IP35` `0xc000000000083edc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1165e of unit 134. Role unknown.
- `rpc_gss_getcred` (`6.5.30/unix.IP35` `0xc000000000083ee4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1169c of unit 134. Role unknown.
- `xlv_tab_subvol_size` (`6.5.30/unix.IP35` `0xc000000000083eec`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x116d5 of unit 134. Role unknown.
- `wd95intr` (`6.5.30/unix.IP35` `0xc000000000083ef4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11712 of unit 134. Role unknown.
- `wd95_earlyinit` (`6.5.30/unix.IP35` `0xc000000000083efc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11744 of unit 134. Role unknown.

### `6.5.30/unix.IP30`

- `_check_dbg` (`6.5.30/unix.IP30` `0xa80000002002c0b0`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x101e0 of unit 99. Role unknown.
- `tpusim_version` (`6.5.30/unix.IP30` `0xa80000002002c0b8`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10214 of unit 99. Role unknown.
- `tpusim_info_chk` (`6.5.30/unix.IP30` `0xa80000002002c0c0`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1024c of unit 99. Role unknown.
- `tpusim_piotrans_addr` (`6.5.30/unix.IP30` `0xa80000002002c0c8`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10285 of unit 99. Role unknown.
- `tpusim_dmatrans_list` (`6.5.30/unix.IP30` `0xa80000002002c0d0`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x102c3 of unit 99. Role unknown.
- `tpusim_intr_alloc` (`6.5.30/unix.IP30` `0xa80000002002c0d8`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10301 of unit 99. Role unknown.
- `tpusim_intr_connect` (`6.5.30/unix.IP30` `0xa80000002002c0e0`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1033c of unit 99. Role unknown.
- `tpuidbg_version` (`6.5.30/unix.IP30` `0xa80000002002c0f0`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x103b1 of unit 99. Role unknown.
- `tpu_statename` (`6.5.30/unix.IP30` `0xa80000002002c100`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10419 of unit 99. Role unknown.
- `klistio_init` (`6.5.30/unix.IP30` `0xa80000002002c108`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10450 of unit 99. Role unknown.
- `klistio32` (`6.5.30/unix.IP30` `0xa80000002002c110`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10486 of unit 99. Role unknown.
- `klistio64` (`6.5.30/unix.IP30` `0xa80000002002c12c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x104b9 of unit 99. Role unknown.
- `klistio_installed` (`6.5.30/unix.IP30` `0xa80000002002c148`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x104ec of unit 99. Role unknown.
- `klistio_getparams` (`6.5.30/unix.IP30` `0xa80000002002c150`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10527 of unit 99. Role unknown.
- `pwexit` (`6.5.30/unix.IP30` `0xa80000002002c16c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10562 of unit 99. Role unknown.
- `pwsysmips` (`6.5.30/unix.IP30` `0xa80000002002c174`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10592 of unit 99. Role unknown.
- `postwait_installed` (`6.5.30/unix.IP30` `0xa80000002002c190`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x105c5 of unit 99. Role unknown.
- `postwait_getparams` (`6.5.30/unix.IP30` `0xa80000002002c198`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10601 of unit 99. Role unknown.
- `swoutput` (`6.5.30/unix.IP30` `0xa80000002002c1c4`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1068e of unit 99. Role unknown.
- `ip_mrouter_cmd` (`6.5.30/unix.IP30` `0xa80000002002c1e0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x106c0 of unit 99. Role unknown.
- `ip_mrouter_done` (`6.5.30/unix.IP30` `0xa80000002002c1fc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x106f8 of unit 99. Role unknown.
- `ip_mforward` (`6.5.30/unix.IP30` `0xa80000002002c204`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10731 of unit 99. Role unknown.
- `mrt_ioctl` (`6.5.30/unix.IP30` `0xa80000002002c20c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10766 of unit 99. Role unknown.
- `legal_vif_num` (`6.5.30/unix.IP30` `0xa80000002002c228`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10799 of unit 99. Role unknown.
- `ip_mroute_slowtimo` (`6.5.30/unix.IP30` `0xa80000002002c230`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x107d0 of unit 99. Role unknown.
- `ip_mroute_init` (`6.5.30/unix.IP30` `0xa80000002002c238`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1080c of unit 99. Role unknown.
- `ip_mrouter_get` (`6.5.30/unix.IP30` `0xa80000002002c254`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10844 of unit 99. Role unknown.
- `ip_mrouter_set` (`6.5.30/unix.IP30` `0xa80000002002c270`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1087c of unit 99. Role unknown.
- `ip_rsvp_vif_init` (`6.5.30/unix.IP30` `0xa80000002002c28c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x108b4 of unit 99. Role unknown.
- `ip_rsvp_vif_done` (`6.5.30/unix.IP30` `0xa80000002002c2a8`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x108ee of unit 99. Role unknown.
- `ipip_input` (`6.5.30/unix.IP30` `0xa80000002002c2c4`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10928 of unit 99. Role unknown.
- `ip_rsvp_force_done` (`6.5.30/unix.IP30` `0xa80000002002c2e0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1095c of unit 99. Role unknown.
- `ipfilter_kernel` (`6.5.30/unix.IP30` `0xa80000002002c2fc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10998 of unit 99. Role unknown.
- `rpc_gss_secget` (`6.5.30/unix.IP30` `0xa80000002002c324`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10a90 of unit 99. Role unknown.
- `__svcrpcsec_gss` (`6.5.30/unix.IP30` `0xa80000002002c340`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ac8 of unit 99. Role unknown.
- `rpc_gss_set_svc_name` (`6.5.30/unix.IP30` `0xa80000002002c350`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10b32 of unit 99. Role unknown.
- `rpc_gss_getcred` (`6.5.30/unix.IP30` `0xa80000002002c358`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10b70 of unit 99. Role unknown.
- `xlv_tab_subvol_size` (`6.5.30/unix.IP30` `0xa80000002002c360`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ba9 of unit 99. Role unknown.
- `wd95intr` (`6.5.30/unix.IP30` `0xa80000002002c368`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10be6 of unit 99. Role unknown.
- `wd95_earlyinit` (`6.5.30/unix.IP30` `0xa80000002002c370`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10c18 of unit 99. Role unknown.

### `6.5.30/unix.IP27`

- `_check_dbg` (`6.5.30/unix.IP27` `0xc00000000005bcac`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0xff5f of unit 132. Role unknown.
- `tpusim_version` (`6.5.30/unix.IP27` `0xc00000000005bcb4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0xff93 of unit 132. Role unknown.
- `tpusim_info_chk` (`6.5.30/unix.IP27` `0xc00000000005bcbc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0xffcb of unit 132. Role unknown.
- `tpusim_piotrans_addr` (`6.5.30/unix.IP27` `0xc00000000005bcc4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10004 of unit 132. Role unknown.
- `tpusim_dmatrans_list` (`6.5.30/unix.IP27` `0xc00000000005bccc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10042 of unit 132. Role unknown.
- `tpusim_intr_alloc` (`6.5.30/unix.IP27` `0xc00000000005bcd4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10080 of unit 132. Role unknown.
- `tpusim_intr_connect` (`6.5.30/unix.IP27` `0xc00000000005bcdc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x100bb of unit 132. Role unknown.
- `tpuidbg_version` (`6.5.30/unix.IP27` `0xc00000000005bcec`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10130 of unit 132. Role unknown.
- `tpu_statename` (`6.5.30/unix.IP27` `0xc00000000005bcfc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10198 of unit 132. Role unknown.
- `klistio_init` (`6.5.30/unix.IP27` `0xc00000000005bd04`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x101cf of unit 132. Role unknown.
- `klistio32` (`6.5.30/unix.IP27` `0xc00000000005bd0c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10205 of unit 132. Role unknown.
- `klistio64` (`6.5.30/unix.IP27` `0xc00000000005bd28`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10238 of unit 132. Role unknown.
- `klistio_installed` (`6.5.30/unix.IP27` `0xc00000000005bd44`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1026b of unit 132. Role unknown.
- `klistio_getparams` (`6.5.30/unix.IP27` `0xc00000000005bd4c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x102a6 of unit 132. Role unknown.
- `pwexit` (`6.5.30/unix.IP27` `0xc00000000005bd68`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x102e1 of unit 132. Role unknown.
- `pwsysmips` (`6.5.30/unix.IP27` `0xc00000000005bd70`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10311 of unit 132. Role unknown.
- `postwait_installed` (`6.5.30/unix.IP27` `0xc00000000005bd8c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10344 of unit 132. Role unknown.
- `postwait_getparams` (`6.5.30/unix.IP27` `0xc00000000005bd94`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10380 of unit 132. Role unknown.
- `swoutput` (`6.5.30/unix.IP27` `0xc00000000005bdc0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1040d of unit 132. Role unknown.
- `ip_mrouter_cmd` (`6.5.30/unix.IP27` `0xc00000000005bddc`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1043f of unit 132. Role unknown.
- `ip_mrouter_done` (`6.5.30/unix.IP27` `0xc00000000005bdf8`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10477 of unit 132. Role unknown.
- `ip_mforward` (`6.5.30/unix.IP27` `0xc00000000005be00`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x104b0 of unit 132. Role unknown.
- `mrt_ioctl` (`6.5.30/unix.IP27` `0xc00000000005be08`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x104e5 of unit 132. Role unknown.
- `legal_vif_num` (`6.5.30/unix.IP27` `0xc00000000005be24`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10518 of unit 132. Role unknown.
- `ip_mroute_slowtimo` (`6.5.30/unix.IP27` `0xc00000000005be2c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1054f of unit 132. Role unknown.
- `ip_mroute_init` (`6.5.30/unix.IP27` `0xc00000000005be34`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1058b of unit 132. Role unknown.
- `ip_mrouter_get` (`6.5.30/unix.IP27` `0xc00000000005be50`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x105c3 of unit 132. Role unknown.
- `ip_mrouter_set` (`6.5.30/unix.IP27` `0xc00000000005be6c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x105fb of unit 132. Role unknown.
- `ip_rsvp_vif_init` (`6.5.30/unix.IP27` `0xc00000000005be88`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10633 of unit 132. Role unknown.
- `ip_rsvp_vif_done` (`6.5.30/unix.IP27` `0xc00000000005bea4`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1066d of unit 132. Role unknown.
- `ipip_input` (`6.5.30/unix.IP27` `0xc00000000005bec0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x106a7 of unit 132. Role unknown.
- `ip_rsvp_force_done` (`6.5.30/unix.IP27` `0xc00000000005bedc`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x106db of unit 132. Role unknown.
- `ipfilter_kernel` (`6.5.30/unix.IP27` `0xc00000000005bef8`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10717 of unit 132. Role unknown.
- `rpc_gss_secget` (`6.5.30/unix.IP27` `0xc00000000005bf20`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1080f of unit 132. Role unknown.
- `__svcrpcsec_gss` (`6.5.30/unix.IP27` `0xc00000000005bf3c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10847 of unit 132. Role unknown.
- `rpc_gss_set_svc_name` (`6.5.30/unix.IP27` `0xc00000000005bf4c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x108b1 of unit 132. Role unknown.
- `rpc_gss_getcred` (`6.5.30/unix.IP27` `0xc00000000005bf54`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x108ef of unit 132. Role unknown.
- `do_unmount` (`6.5.30/unix.IP27` `0xc00000000005bf5c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10928 of unit 132. Role unknown.
- `nfs3_getvnodeops` (`6.5.30/unix.IP27` `0xc00000000005bf78`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1095c of unit 132. Role unknown.
- `__nfs3_get_readahead` (`6.5.30/unix.IP27` `0xc00000000005bf80`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10996 of unit 132. Role unknown.
- `lockd_dispatch` (`6.5.30/unix.IP27` `0xc00000000005bfa0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10a61 of unit 132. Role unknown.
- `klm_lockctl` (`6.5.30/unix.IP27` `0xc00000000005bfc4`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10ac7 of unit 132. Role unknown.
- `record_remote_name` (`6.5.30/unix.IP27` `0xc00000000005bfe0`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10afc of unit 132. Role unknown.
- `nsm_cntl` (`6.5.30/unix.IP27` `0xc00000000005bffc`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10b38 of unit 132. Role unknown.
- `rfs_dispatch` (`6.5.30/unix.IP27` `0xc00000000005c018`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10b6a of unit 132. Role unknown.
- `xattr_dispatch` (`6.5.30/unix.IP27` `0xc00000000005c034`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10ba0 of unit 132. Role unknown.
- `nfs_getfh` (`6.5.30/unix.IP27` `0xc00000000005c050`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10bd8 of unit 132. Role unknown.
- `exportfs` (`6.5.30/unix.IP27` `0xc00000000005c06c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10c0b of unit 132. Role unknown.
- `nfs_cnvt` (`6.5.30/unix.IP27` `0xc00000000005c088`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10c3d of unit 132. Role unknown.
- `nfs_cnvt3` (`6.5.30/unix.IP27` `0xc00000000005c0a4`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10c6f of unit 132. Role unknown.
- `nfs_purge_vfsp` (`6.5.30/unix.IP27` `0xc00000000005c0c0`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ca2 of unit 132. Role unknown.
- `nfs_generic_cmd` (`6.5.30/unix.IP27` `0xc00000000005c0c8`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10cda of unit 132. Role unknown.
- `findexport` (`6.5.30/unix.IP27` `0xc00000000005c0f4`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d72 of unit 132. Role unknown.
- `findexport_path` (`6.5.30/unix.IP27` `0xc00000000005c0fc`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10da6 of unit 132. Role unknown.
- `fhtovp` (`6.5.30/unix.IP27` `0xc00000000005c104`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ddf of unit 132. Role unknown.
- `checkauth` (`6.5.30/unix.IP27` `0xc00000000005c10c`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e0f of unit 132. Role unknown.
- `kcdb_enabled` (`6.5.30/unix.IP27` `0xc00000000005c114`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e42 of unit 132. Role unknown.
- `nfs_auth_strerror` (`6.5.30/unix.IP27` `0xc00000000005c134`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f0d of unit 132. Role unknown.
- `cnfs_stuff_rdplus_data` (`6.5.30/unix.IP27` `0xc00000000005c13c`, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10f48 of unit 132. Role unknown.
- `xlv_tab_subvol_size` (`6.5.30/unix.IP27` `0xc00000000005c158`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f88 of unit 132. Role unknown.
- `wd95intr` (`6.5.30/unix.IP27` `0xc00000000005c160`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10fc5 of unit 132. Role unknown.
- `wd95_earlyinit` (`6.5.30/unix.IP27` `0xc00000000005c168`, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ff7 of unit 132. Role unknown.

## Debugger objects

### `6.5.30/usr/cpu/sysgen/IP35boot/idbg.o`

- `kqueue_first` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x0`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1f130 of unit 1. Role unknown.
- `kqueue_next` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1c`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1f183 of unit 1. Role unknown.
- `kqueue_end` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x38`, 24 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1f1d5 of unit 1. Role unknown.
- `VPAG_GETPRID` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x58`, 80 bytes, static).
  Returns `prid_t`; Parameters `vpagg_t *vpag`. Record 0x1f251 of unit 1. Role unknown.
- `VPAG_EXTRACT_ARSESS_INFO` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xa8`, 112 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag, arsess_t *arsess_info`. Record 0x1f2da of unit 1. Role unknown.
- `mutexmeter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x118`, 88 bytes, static).
  Returns `mmeter_t *`; Parameters `mutex_t *mp`. Record 0x1f363 of unit 1. Role unknown.
- `svmeter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x170`, 88 bytes, static).
  Returns `svmeter_t *`; Parameters `sv_t *svp`. Record 0x1f3b3 of unit 1. Role unknown.
- `smeter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1c8`, 88 bytes, static).
  Returns `k_semameter_t *`; Parameters `sema_t *sp`. Record 0x1f401 of unit 1. Role unknown.
- `mrmeter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x220`, 88 bytes, static).
  Returns `k_mrmeter_t *`; Parameters `mrlock_t *mrp`. Record 0x1f44d of unit 1. Role unknown.
- `_idbg_unload` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x740`, 88 bytes, external).
  Returns `int`; no parameter records. Record 0x1f589 of unit 1. Role unknown.
- `_idbg_addfssw` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x798`, 88 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x1f5c0 of unit 1. Role unknown.
- `_idbg_delfssw` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x7f0`, 156 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x1f60b of unit 1. Role unknown.
- `idbg_findfssw_vfs` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x88c`, 200 bytes, external).
  Returns `idbgfssw_t *`; Parameters `vfs_t *vfsp, bhv_desc_t * *bdp`. Record 0x1f673 of unit 1. Role unknown.
- `idbg_findfssw_vnbhv` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x954`, 148 bytes, external).
  Returns `idbgfssw_t *`; Parameters `bhv_desc_t *bdp`. Record 0x1f6f6 of unit 1. Role unknown.
- `_idbg_addfunc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x9e8`, 1584 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a subroutine type)`. Record 0x1f766 of unit 1. Role unknown.
- `idbg_findfunc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1018`, 780 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a pointer to a subroutine type)`. Record 0x1f817 of unit 1. Role unknown.
- `_idbg_delfunc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1324`, 432 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type)`. Record 0x1f8bb of unit 1. Role unknown.
- `_idbg_copytab` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x14d4`, 792 bytes, external).
  Returns `int`; Parameters `caddr_t *uaddr, int len, rval_t *rvp`. Record 0x1f946 of unit 1. Role unknown.
- `_idbg_tablesize` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x17ec`, 128 bytes, external).
  No return type in the record; Parameters `rval_t *rvp`. Record 0x1fa35 of unit 1. Role unknown.
- `_idbg_dofunc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x186c`, 2376 bytes, external).
  Returns `int`; Parameters `struct idbgcomm *s, rval_t *rvp`. Record 0x1fa84 of unit 1. Role unknown.
- `_qprintf` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x21b4`, 788 bytes, external).
  No return type in the record; Parameters `char *fmt, va_list ap`. Record 0x1fc6e of unit 1. Role unknown.
- `_idbg_error` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x24c8`, 104 bytes, external).
  Returns `int`; no parameter records. Record 0x1fd01 of unit 1. Role unknown.
- `_idbg_switch` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2530`, 488 bytes, external).
  No return type in the record; Parameters `int snum, int onoff`. Record 0x1fd37 of unit 1. Role unknown.
- `idbg_string` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2718`, 92 bytes, external).
  No return type in the record; Parameters `int addr`. Record 0x1fdac of unit 1. Role unknown.
- `_printflags` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2774`, 484 bytes, external).
  No return type in the record; Parameters `uint64_t flags, char * *strings, char *name`. Record 0x1fdf7 of unit 1. Role unknown.
- `idbg_hd` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2958`, 464 bytes, external).
  No return type in the record; Parameters `unsigned int *addr, __psint_t len`. Record 0x1fe9d of unit 1. Role unknown.
- `ktrace_nentries` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2b28`, 116 bytes, external).
  Returns `int`; Parameters `ktrace_t *ktp`. Record 0x1ff1d of unit 1. Role unknown.
- `ktrace_first` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2b9c`, 236 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x1ff74 of unit 1. Role unknown.
- `ktrace_next` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2c88`, 192 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x20021 of unit 1. Role unknown.
- `ktrace_skip` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2d48`, 348 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, int count, ktrace_snap_t *ktsp`. Record 0x200b4 of unit 1. Role unknown.
- `printtimers` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2ea4`, 820 bytes, external).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x20191 of unit 1. Role unknown.
- `idbg_kthreadname` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x31d8`, 188 bytes, external).
  Returns `char *`; Parameters `kthread_t *kt`. Record 0x2020b of unit 1. Role unknown.
- `idbg_polldat` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x3294`, 264 bytes, external).
  No return type in the record; Parameters `struct polldat *p`. Record 0x20277 of unit 1. Role unknown.
- `idbg_pollhead` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x339c`, 328 bytes, external).
  No return type in the record; Parameters `struct pollhead *p`. Record 0x202c1 of unit 1. Role unknown.
- `idbg_proxy` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x3750`, 1080 bytes, external).
  No return type in the record; Parameters `proc_proxy_t *prxy`. Record 0x203b0 of unit 1. Role unknown.
- `pcred` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x3b88`, 636 bytes, external).
  No return type in the record; Parameters `cred_t *crp`. Record 0x203fb of unit 1. Role unknown.
- `idbg_doto_uthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x3e04`, 572 bytes, external).
  Returns `int`; Parameters `__psint_t p1, pfunc (pointer to a subroutine type), int all, int must`. Record 0x20452 of unit 1. Role unknown.
- `idbg_do_uthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x4040`, 3984 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x2051b of unit 1. Role unknown.
- `idbg_uthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x4fd0`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x205e4 of unit 1. Role unknown.
- `idbg_proc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x5030`, 2692 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x2062f of unit 1. Role unknown.
- `sleepon` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x5abc`, 1424 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x2070f of unit 1. Role unknown.
- `dumpssleep` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x604c`, 128 bytes, static).
  No return type in the record; Parameters `ssleep_t *st`. Record 0x2081c of unit 1. Role unknown.
- `idbg_pchain` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x60cc`, 616 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x20864 of unit 1. Role unknown.
- `issthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x6334`, 188 bytes, static).
  Returns `int`; Parameters `sthread_t *x`. Record 0x208d7 of unit 1. Role unknown.
- `isxthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x63f0`, 188 bytes, static).
  Returns `int`; Parameters `xthread_t *x`. Record 0x20938 of unit 1. Role unknown.
- `kstack` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x64ac`, 576 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x20999 of unit 1. Role unknown.
- `idbg_slist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x66ec`, 548 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20a1f of unit 1. Role unknown.
- `idbg_xlist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x6910`, 548 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20a8d of unit 1. Role unknown.
- `idbg_klist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x6b34`, 244 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20afb of unit 1. Role unknown.
- `kid_is_uthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x6c28`, 172 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x20b59 of unit 1. Role unknown.
- `find_kthreadid` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x6cd4`, 880 bytes, static).
  Returns `int`; Parameters `uint64_t id, int verbose, __psunsigned_t *result`. Record 0x20c09 of unit 1. Role unknown.
- `idbg_kthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x7044`, 2964 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20cef of unit 1. Role unknown.
- `idbg_sthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x7bd8`, 588 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20dc7 of unit 1. Role unknown.
- `idbg_xthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x7e24`, 848 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20e37 of unit 1. Role unknown.
- `print_proc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x8174`, 96 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x20ea7 of unit 1. Role unknown.
- `idbg_plist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x81d4`, 360 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20f1f of unit 1. Role unknown.
- `print_proc_fdt` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x833c`, 360 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x20f7a of unit 1. Role unknown.
- `idbg_plfdt` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x84a4`, 336 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20ff6 of unit 1. Role unknown.
- `showptree` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x85f4`, 332 bytes, static).
  No return type in the record; Parameters `proc_t *parent, int level`. Record 0x21062 of unit 1. Role unknown.
- `idbg_ptree` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x8740`, 308 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x21100 of unit 1. Role unknown.
- `idbg_pgrp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x8874`, 456 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2115c of unit 1. Role unknown.
- `gdbg_proc_print` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x8a3c`, 356 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x211d4 of unit 1. Role unknown.
- `gdbg_owner_print` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x8ba0`, 212 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x212a1 of unit 1. Role unknown.
- `idbg_pagg` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x8c74`, 2588 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x2135b of unit 1. Role unknown.
- `showpagtree` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x9690`, 360 bytes, static).
  No return type in the record; Parameters `proc_t *root, proc_t *parent, int level`. Record 0x21423 of unit 1. Role unknown.
- `idbg_pagtree` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x97f8`, 564 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x214d9 of unit 1. Role unknown.
- `idbg_paglist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x9a2c`, 432 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x2157c of unit 1. Role unknown.
- `proc_pagfind` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x9bdc`, 120 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x21601 of unit 1. Role unknown.
- `idbg_pagfind` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x9c54`, 308 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x21690 of unit 1. Role unknown.
- `shortuthread` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x9d88`, 560 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x21706 of unit 1. Role unknown.
- `shortproc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x9fb8`, 760 bytes, external).
  No return type in the record; Parameters `proc_t *pp, int ohboy`. Record 0x21750 of unit 1. Role unknown.
- `tinyproc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xa2b0`, 172 bytes, static).
  No return type in the record; Parameters `proc_t *p`. Record 0x217d2 of unit 1. Role unknown.
- `shortvpag` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xa35c`, 340 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag`. Record 0x21817 of unit 1. Role unknown.
- `prksigset` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xa4b0`, 468 bytes, static).
  No return type in the record; Parameters `char *str, k_sigset_t *ks`. Record 0x21860 of unit 1. Role unknown.
- `idbg_do_signal` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xa684`, 1576 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x218df of unit 1. Role unknown.
- `idbg_signal` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xacac`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21966 of unit 1. Role unknown.
- `idbg_ptrace` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xad0c`, 444 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x219b0 of unit 1. Role unknown.
- `idbg_prnode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xaec8`, 768 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21a24 of unit 1. Role unknown.
- `idbg_prnodefree` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xb1c8`, 1256 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x21aae of unit 1. Role unknown.
- `idbg_watch` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xb6b0`, 964 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21b26 of unit 1. Role unknown.
- `print_sleepers` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xba74`, 808 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x21bb4 of unit 1. Role unknown.
- `idbg_slpproc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xbd9c`, 128 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x21cb4 of unit 1. Role unknown.
- `idbg_priscan` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xbeac`, 232 bytes, static).
  No return type in the record; Parameters `prilist_t *mp`. Record 0x21d42 of unit 1. Role unknown.
- `idbg_printputbuf` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xbf94`, 224 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x21d8c of unit 1. Role unknown.
- `dumpminfostats` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xc074`, 824 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x21dda of unit 1. Role unknown.
- `idbg_minfo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xc948`, 232 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x21fc2 of unit 1. Role unknown.
- `dumpsysinfostats` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xca30`, 1096 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x2200a of unit 1. Role unknown.
- `idbg_sysinfo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xce78`, 88 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x22076 of unit 1. Role unknown.
- `idbg_gbuf` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xced0`, 2772 bytes, static).
  No return type in the record; Parameters `buf_t *bp, int idx, buf_t *bufhead`. Record 0x220c0 of unit 1. Role unknown.
- `dumppdabstats` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xdda4`, 1008 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x221a3 of unit 1. Role unknown.
- `idbg_fbuf` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xe194`, 1312 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x221ee of unit 1. Role unknown.
- `idbg_vbuf` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xe6b4`, 536 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x222cd of unit 1. Role unknown.
- `idbg_findbuf` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xe8cc`, 416 bytes, external).
  No return type in the record; Parameters `int blkno`. Record 0x22395 of unit 1. Role unknown.
- `idbg_dport` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xea6c`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x22422 of unit 1. Role unknown.
- `idbg_dportrset` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xeaac`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x2246b of unit 1. Role unknown.
- `idbg_pty` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xeaec`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x224b8 of unit 1. Role unknown.
- `idbg_do_shaddr` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xeb2c`, 1816 bytes, external).
  No return type in the record; Parameters `shaddr_t *sa`. Record 0x224fe of unit 1. Role unknown.
- `idbg_do_ut_shaddr` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xf244`, 176 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x2256f of unit 1. Role unknown.
- `idbg_shaddr` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xf2f4`, 168 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x225d2 of unit 1. Role unknown.
- `idbg_vnode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xf39c`, 1684 bytes, external).
  Returns `vnode_t *`; Parameters `__psint_t p1`. Record 0x2261c of unit 1. Role unknown.
- `vnops_to_vfstype` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xfa30`, 220 bytes, static).
  Returns `int`; Parameters `vnodeops_t *vnops`. Record 0x22728 of unit 1. Role unknown.
- `idbg_bhvh` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xfb0c`, 352 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22793 of unit 1. Role unknown.
- `idbg_bhv` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xfc6c`, 300 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x227ef of unit 1. Role unknown.
- `idbg_vnbhv` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0xfd98`, 2016 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22891 of unit 1. Role unknown.
- `idbg_extents` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x10578`, 1024 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x229c0 of unit 1. Role unknown.
- `idbg_prfifo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x10978`, 2180 bytes, external).
  No return type in the record; Parameters `struct fifonode *fnp, int header`. Record 0x22aaf of unit 1. Role unknown.
- `idbg_prinode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x111fc`, 1392 bytes, external).
  No return type in the record; Parameters `struct inode *ip, vtype_t vtype, int header`. Record 0x22b12 of unit 1. Role unknown.
- `idbg_prnamenode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1176c`, 720 bytes, external).
  No return type in the record; Parameters `struct namenode *nmp, int header`. Record 0x22b8b of unit 1. Role unknown.
- `idbg_prlsnode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x11a3c`, 2028 bytes, external).
  No return type in the record; Parameters `struct lsnode *lsp`. Record 0x22bf1 of unit 1. Role unknown.
- `idbg_prcsnode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x12228`, 1996 bytes, external).
  No return type in the record; Parameters `struct csnode *csp`. Record 0x22c53 of unit 1. Role unknown.
- `prvn` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x129f4`, 616 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x22cb5 of unit 1. Role unknown.
- `__prvn` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x12c5c`, 72 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x22d22 of unit 1. Role unknown.
- `prvfsvn` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x12ca4`, 1208 bytes, static).
  No return type in the record; Parameters `struct vfs *vfsp, int all`. Record 0x22d7b of unit 1. Role unknown.
- `idbg_fifo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1315c`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22e70 of unit 1. Role unknown.
- `idbg_fifolist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x131bc`, 516 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22ec8 of unit 1. Role unknown.
- `idbg_namelist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x133c0`, 516 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22f3a of unit 1. Role unknown.
- `idbg_vlist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x135c4`, 1908 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22fac of unit 1. Role unknown.
- `dumppdaistats` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x13d38`, 744 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x23084 of unit 1. Role unknown.
- `do_efs` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x14020`, 1944 bytes, static).
  No return type in the record; Parameters `struct efs *fs, int summary`. Record 0x230cf of unit 1. Role unknown.
- `idbg_efs` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x147b8`, 68 bytes, external).
  No return type in the record; Parameters `struct efs *fs`. Record 0x231b2 of unit 1. Role unknown.
- `idbg_quota` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x147fc`, 1216 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x231f9 of unit 1. Role unknown.
- `idbg_dqb` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x14cbc`, 1364 bytes, external).
  No return type in the record; Parameters `struct dquot *dqp`. Record 0x232e5 of unit 1. Role unknown.
- `idbg_vfs` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x15210`, 392 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x233a1 of unit 1. Role unknown.
- `prvfs` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x15398`, 1764 bytes, external).
  No return type in the record; Parameters `struct vfs *vfsp, int summary`. Record 0x2340d of unit 1. Role unknown.
- `idbg_vnmap` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x15a7c`, 400 bytes, external).
  No return type in the record; Parameters `__psunsigned_t addr`. Record 0x23505 of unit 1. Role unknown.
- `print_fmode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x15c0c`, 240 bytes, static).
  No return type in the record; Parameters `int fmode`. Record 0x235b1 of unit 1. Role unknown.
- `idbg_bmapval` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x15cfc`, 360 bytes, external).
  No return type in the record; Parameters `struct bmapval *bmapp`. Record 0x23628 of unit 1. Role unknown.
- `idbg_uio` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x15e9c`, 756 bytes, external).
  No return type in the record; Parameters `uio_t *uiop`. Record 0x236d3 of unit 1. Role unknown.
- `idbg_open` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16190`, 144 bytes, external).
  No return type in the record; Parameters `struct opena *uap`. Record 0x23758 of unit 1. Role unknown.
- `idbg_creat` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16220`, 104 bytes, external).
  No return type in the record; Parameters `struct creata *uap`. Record 0x237a1 of unit 1. Role unknown.
- `idbg_mkdir` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16288`, 104 bytes, external).
  No return type in the record; Parameters `struct mkdira *uap`. Record 0x237eb of unit 1. Role unknown.
- `idbg_link` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x162f0`, 104 bytes, external).
  No return type in the record; Parameters `struct linka *uap`. Record 0x23835 of unit 1. Role unknown.
- `idbg_rename` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16358`, 104 bytes, external).
  No return type in the record; Parameters `struct renamea *uap`. Record 0x2387e of unit 1. Role unknown.
- `idbg_symlink` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x163c0`, 104 bytes, external).
  No return type in the record; Parameters `struct symlinka *uap`. Record 0x238c9 of unit 1. Role unknown.
- `idbg_unlink` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16428`, 96 bytes, external).
  No return type in the record; Parameters `struct unlinka *uap`. Record 0x23915 of unit 1. Role unknown.
- `idbg_rmdir` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16488`, 96 bytes, external).
  No return type in the record; Parameters `struct rmdira *uap`. Record 0x23960 of unit 1. Role unknown.
- `idbg_xstat` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x164e8`, 112 bytes, external).
  No return type in the record; Parameters `struct xstatarg *uap`. Record 0x239aa of unit 1. Role unknown.
- `idbg_dumpsym` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16558`, 368 bytes, external).
  No return type in the record; Parameters `char * *addr`. Record 0x239f4 of unit 1. Role unknown.
- `idbg_callo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x166c8`, 656 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23a65 of unit 1. Role unknown.
- `dump_callo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16958`, 472 bytes, static).
  No return type in the record; Parameters `struct callout *p1`. Record 0x23abf of unit 1. Role unknown.
- `idbg_calloinfo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16b30`, 476 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23b1d of unit 1. Role unknown.
- `dump_callout_info` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16d0c`, 620 bytes, static).
  No return type in the record; Parameters `callout_info_t *cip`. Record 0x23b88 of unit 1. Role unknown.
- `idbg_ptimer` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x16f78`, 532 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23c00 of unit 1. Role unknown.
- `idbg_file` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1718c`, 708 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23ca6 of unit 1. Role unknown.
- `print_procfdt` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x17450`, 400 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x23d4f of unit 1. Role unknown.
- `idbg_vfile` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x175e0`, 300 bytes, external).
  No return type in the record; Parameters `__psint_t ptr`. Record 0x23e19 of unit 1. Role unknown.
- `ltype_to_str` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1770c`, 464 bytes, external).
  Returns `char *`; Parameters `int ltype`. Record 0x23e76 of unit 1. Role unknown.
- `idbg_filock` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x178dc`, 232 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x23f12 of unit 1. Role unknown.
- `idbg_sleeplcks` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x179c4`, 344 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x23f84 of unit 1. Role unknown.
- `idbg_flock` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x17b1c`, 188 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x23ff9 of unit 1. Role unknown.
- `printfp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x17bd8`, 1124 bytes, static).
  Returns `int`; Parameters `vfile_t *fp, int slot, int flag`. Record 0x24063 of unit 1. Role unknown.
- `idbg_map` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1803c`, 244 bytes, external).
  No return type in the record; Parameters `struct map *p`. Record 0x24129 of unit 1. Role unknown.
- `idbg_dobitmap` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x18130`, 560 bytes, external).
  No return type in the record; Parameters `struct bitmap *bp, char *name`. Record 0x24180 of unit 1. Role unknown.
- `idbg_chbtmp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x18360`, 332 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x241f3 of unit 1. Role unknown.
- `idbg_bitmap` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x184ac`, 928 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x242a2 of unit 1. Role unknown.
- `idbg_svdump` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1884c`, 1612 bytes, external).
  No return type in the record; Parameters `sv_t *svp`. Record 0x24326 of unit 1. Role unknown.
- `idbg_pwait` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x18e98`, 472 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x243ad of unit 1. Role unknown.
- `idbg_page_counter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x19070`, 20 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x2440c of unit 1. Role unknown.
- `idbg_shdump` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1908c`, 1060 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x24495 of unit 1. Role unknown.
- `idbg_semchain` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x194b0`, 816 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x24519 of unit 1. Role unknown.
- `idbg_nschain` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x197e0`, 1132 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x24588 of unit 1. Role unknown.
- `scanchain` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x19c4c`, 512 bytes, static).
  No return type in the record; Parameters `int x`. Record 0x24630 of unit 1. Role unknown.
- `idbgplog` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x19e4c`, 792 bytes, external).
  No return type in the record; Parameters `semahist_t *hp`. Record 0x2469d of unit 1. Role unknown.
- `idbg_semctl` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1a164`, 576 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x24711 of unit 1. Role unknown.
- `semctl_apply` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1a3a4`, 836 bytes, static).
  Returns `int`; Parameters `char *str, func (pointer to a subroutine type), int ac, char * *av`. Record 0x247bd of unit 1. Role unknown.
- `semctl_describe` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1a6e8`, 232 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x2492a of unit 1. Role unknown.
- `semctl_ctlmeter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1a7d0`, 3256 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2499d of unit 1. Role unknown.
- `semctl_ctlmeter_aux` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1b488`, 2532 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x24b00 of unit 1. Role unknown.
- `semctl_defhist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1be6c`, 828 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24d05 of unit 1. Role unknown.
- `semctl_defkhist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1c1a8`, 760 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24d64 of unit 1. Role unknown.
- `semctl_defkhlen` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1c4a0`, 388 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24dc4 of unit 1. Role unknown.
- `semctl_defmeter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1c624`, 760 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24e4a of unit 1. Role unknown.
- `semctl_help` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1c91c`, 224 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24eaa of unit 1. Role unknown.
- `semctl_histmax` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1c9fc`, 360 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24f1b of unit 1. Role unknown.
- `semctl_sema` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1cb64`, 188 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24fa3 of unit 1. Role unknown.
- `semctl_semlog` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1cc20`, 3200 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24fff of unit 1. Role unknown.
- `semctl_what` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1d8a0`, 188 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x251b7 of unit 1. Role unknown.
- `printid` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1d95c`, 552 bytes, static).
  No return type in the record; Parameters `kthread_t *kt, uint64_t id, char *str`. Record 0x25213 of unit 1. Role unknown.
- `idbg_semdump` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1db84`, 1620 bytes, external).
  No return type in the record; Parameters `sema_t *sp`. Record 0x252a7 of unit 1. Role unknown.
- `idbg_mutexdump` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1e1d8`, 1316 bytes, external).
  No return type in the record; Parameters `mutex_t *mp`. Record 0x2532e of unit 1. Role unknown.
- `idbg_dumphisto` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1e6fc`, 1656 bytes, static).
  No return type in the record; Parameters `k_histelem_t *khp, char *str, int timed`. Record 0x253b7 of unit 1. Role unknown.
- `idbg_semalist` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1ed74`, 1436 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x254a9 of unit 1. Role unknown.
- `idbg_chklock` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1f310`, 1608 bytes, external).
  No return type in the record; Parameters `void *l`. Record 0x2556a of unit 1. Role unknown.
- `idbg_avlnode` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1f958`, 344 bytes, external).
  No return type in the record; Parameters `avlnode_t *avl`. Record 0x255c9 of unit 1. Role unknown.
- `idbg_lockmeter` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x1fab0`, 1588 bytes, external).
  No return type in the record; Parameters `k_splockmeter_t *spm`. Record 0x25615 of unit 1. Role unknown.
- `islock` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x200e4`, 360 bytes, static).
  Returns `char *`; Parameters `void *lck`. Record 0x25663 of unit 1. Role unknown.
- `idbg_hotlocks` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2024c`, 72 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x256d5 of unit 1. Role unknown.
- `dumphotlock` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x20294`, 2876 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x25733 of unit 1. Role unknown.
- `idbg_coldlocks` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x20dd0`, 528 bytes, external).
  No return type in the record; Parameters `int n`. Record 0x2582c of unit 1. Role unknown.
- `idbg_mrlock` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x20fe0`, 3844 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x258ed of unit 1. Role unknown.
- `idbg_mslock` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x21ee4`, 408 bytes, external).
  No return type in the record; Parameters `mslock_t *msp`. Record 0x25a52 of unit 1. Role unknown.
- `idbg_mrlockfull` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2207c`, 4956 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x25a9d of unit 1. Role unknown.
- `idbg_pda` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x233d8`, 148 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x25b8f of unit 1. Role unknown.
- `do_pdas` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2346c`, 480 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type), int which`. Record 0x25bd5 of unit 1. Role unknown.
- `dumppda` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2364c`, 4328 bytes, external).
  No return type in the record; Parameters `pda_t *pd`. Record 0x25c45 of unit 1. Role unknown.
- `idbg_donodepda` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x24734`, 1824 bytes, external).
  No return type in the record; Parameters `nodepda_t *npda`. Record 0x25ce0 of unit 1. Role unknown.
- `idbg_nodepda` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x24e54`, 332 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x25da5 of unit 1. Role unknown.
- `idbg_uarea` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x24fa0`, 540 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x25e01 of unit 1. Role unknown.
- `idbg_upage` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x251bc`, 624 bytes, external).
  No return type in the record; Parameters `proc_t *pp`. Record 0x25e5c of unit 1. Role unknown.
- `idbg_fdt` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2542c`, 464 bytes, external).
  No return type in the record; Parameters `fdt_t *fdt`. Record 0x25ea5 of unit 1. Role unknown.
- `idbg_runq` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x255fc`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t howmany`. Record 0x25f1e of unit 1. Role unknown.
- `dumpRunq` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2563c`, 1900 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x25f6b of unit 1. Role unknown.
- `idbg_dopglst` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x25da8`, 288 bytes, static).
  No return type in the record; Parameters `pglst_t *pglptr`. Record 0x25fed of unit 1. Role unknown.
- `idbg_swap` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x25ec8`, 592 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x26050 of unit 1. Role unknown.
- `idbg_pglst` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x26118`, 540 bytes, external).
  No return type in the record; Parameters `__psint_t i`. Record 0x260bc of unit 1. Role unknown.
- `idbg_kaddr` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x26334`, 324 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x26121 of unit 1. Role unknown.
- `idbg_do_eframe` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x26478`, 536 bytes, external).
  No return type in the record; Parameters `eframe_t *ep`. Record 0x26196 of unit 1. Role unknown.
- `idbg_do_ut_eframe` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x26690`, 160 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x261f8 of unit 1. Role unknown.
- `idbg_eframe` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x26730`, 204 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2625b of unit 1. Role unknown.
- `idbg_ubtrace` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x267fc`, 1912 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x262a4 of unit 1. Role unknown.
- `convsp_start` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x26f74`, 96 bytes, static).
  Returns `int`; Parameters `struct uthread_s *ut`. Record 0x263b2 of unit 1. Role unknown.
- `convsp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x26ff8`, 128 bytes, static).
  Returns `unsigned int *`; Parameters `unsigned int *sp, unsigned int *physsp`. Record 0x26430 of unit 1. Role unknown.
- `idbg_get_stack_bytes` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x27078`, 304 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut, void *sp, void *dp, int n`. Record 0x264a7 of unit 1. Role unknown.
- `fetch_kaddr` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x271a8`, 472 bytes, external).
  Returns `__psunsigned_t`; Parameters `char *s`. Record 0x2655b of unit 1. Role unknown.
- `lookup_kname` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x27380`, 416 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x26631 of unit 1. Role unknown.
- `fetch_kname` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x27520`, 140 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x266c5 of unit 1. Role unknown.
- `_prsymoff` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x275ac`, 468 bytes, external).
  No return type in the record; Parameters `void *addr, char *pre, char *post`. Record 0x26760 of unit 1. Role unknown.
- `idbg_btrace` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x27780`, 148 bytes, external).
  No return type in the record; Parameters `int maxfr`. Record 0x267ff of unit 1. Role unknown.
- `btrace` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x27814`, 9356 bytes, static).
  No return type in the record; Parameters `unsigned int *pc, unsigned int *sp, unsigned int *lra, int maxfr, unsigned int *lbound, kthread_t *kt, int myself`. Record 0x2684c of unit 1. Role unknown.
- `txtsrch` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x29ca0`, 1152 bytes, static).
  Returns `inst_t *`; Parameters `inst_t *pc, unsigned int inst1mask, unsigned int inst2mask, int *which, enum sign im1sign, enum sign im2sign, int *im_ptr, inst_t *llimit, struct spec_funcs * *specf, enum direction dir, unsigned int check1mask, unsigned int check2mask`. Record 0x26c24 of unit 1. Role unknown.
- `is_jal` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a2a8`, 168 bytes, static).
  Returns `int`; Parameters `inst_t inst`. Record 0x26eaa of unit 1. Role unknown.
- `branch_target` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a350`, 240 bytes, static).
  Returns `inst_t *`; Parameters `inst_t inst, inst_t *pc`. Record 0x26f0a of unit 1. Role unknown.
- `fetch_text` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a440`, 308 bytes, static).
  Returns `inst_t`; Parameters `inst_t *pc`. Record 0x26f84 of unit 1. Role unknown.
- `ilastframe` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a574`, 76 bytes, static).
  Returns `int`; Parameters `k_machreg_t *sp`. Record 0x26ffc of unit 1. Role unknown.
- `idbg_symtab` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a5c0`, 88 bytes, external).
  No return type in the record; Parameters `struct dbstbl * *db, char * *name`. Record 0x2704c of unit 1. Role unknown.
- `idbg_symsize` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a618`, 56 bytes, external).
  No return type in the record; Parameters `int *size`. Record 0x270ab of unit 1. Role unknown.
- `idbg_ksymrange` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a650`, 88 bytes, external).
  No return type in the record; Parameters `void * *_kftext, void * *_kend`. Record 0x270f8 of unit 1. Role unknown.
- `idbg_scsi` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2a6a8`, 892 bytes, external).
  No return type in the record; Parameters `struct scsi_request *req`. Record 0x27160 of unit 1. Role unknown.
- `idbg_foi` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2aa24`, 1372 bytes, external).
  No return type in the record; Parameters `vertex_hdl_t lun_vhdl`. Record 0x271bb of unit 1. Role unknown.
- `dkactive` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2af80`, 164 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x272a8 of unit 1. Role unknown.
- `dkqueue` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2b024`, 564 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x27302 of unit 1. Role unknown.
- `idbg_dksoftc` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2b258`, 1164 bytes, external).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x2735b of unit 1. Role unknown.
- `idbg_xpr` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2b734`, 116 bytes, external).
  No return type in the record; Parameters `void *xpr`. Record 0x273d8 of unit 1. Role unknown.
- `idbg_xpm` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2b814`, 120 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2744a of unit 1. Role unknown.
- `intvec_tinfo` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2b88c`, 640 bytes, external).
  No return type in the record; Parameters `thd_int_t *tip`. Record 0x27490 of unit 1. Role unknown.
- `idbg_call` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2bb0c`, 368 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x274dc of unit 1. Role unknown.
- `idbg_kill` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2bc7c`, 508 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x27523 of unit 1. Role unknown.
- `idbg_qbuf` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2be78`, 772 bytes, external).
  No return type in the record; Parameters `__psint_t dev`. Record 0x27591 of unit 1. Role unknown.
- `idbg_dozone` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2c3bc`, 2084 bytes, static).
  No return type in the record; Parameters `zone_t *zp, int all`. Record 0x276ab of unit 1. Role unknown.
- `idbg_zone` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2cbe0`, 912 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x2775f of unit 1. Role unknown.
- `idbg_tracemasks` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2cf70`, 364 bytes, external).
  No return type in the record; Parameters `struct pr_tracemasks *t`. Record 0x27823 of unit 1. Role unknown.
- `idbg_tstamp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2d0dc`, 364 bytes, external).
  No return type in the record; Parameters `const tstamp_obj_t *objp`. Record 0x27871 of unit 1. Role unknown.
- `utrace_dump` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2d248`, 2740 bytes, static).
  No return type in the record; Parameters `tstamp_event_entry_t *event, int cpu, tstamp_event_entry_t *limit, __psint_t sval`. Record 0x278bd of unit 1. Role unknown.
- `_idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2dcfc`, 3872 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2, __psint_t sval`. Record 0x279dd of unit 1. Role unknown.
- `idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2ec1c`, 76 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x27cca of unit 1. Role unknown.
- `idbg_utrace_find` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2ec68`, 136 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x27d2b of unit 1. Role unknown.
- `idbg_utrace_mask` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2ecf0`, 200 bytes, external).
  No return type in the record; Parameters `uint64_t arg1`. Record 0x27d91 of unit 1. Role unknown.
- `idbg_do_softfp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2edb8`, 204 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x27de2 of unit 1. Role unknown.
- `idbg_softfp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2ee84`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x27e2e of unit 1. Role unknown.
- `print_maclabel` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2eee4`, 1004 bytes, external).
  No return type in the record; Parameters `char *cp, mac_label *lp`. Record 0x27e78 of unit 1. Role unknown.
- `idbg_stop` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2f2d0`, 64 bytes, external).
  No return type in the record; Parameters `void *stoplist`. Record 0x27f02 of unit 1. Role unknown.
- `idbg_ioerror` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2f3a0`, 1552 bytes, static).
  No return type in the record; Parameters `__psunsigned_t arg`. Record 0x27f7e of unit 1. Role unknown.
- `re_comp` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x2f9b0`, 2328 bytes, external).
  Returns `char *`; Parameters `const char *sp`. Record 0x27fec of unit 1. Role unknown.
- `re_exec` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x302c8`, 592 bytes, external).
  Returns `int`; Parameters `const char *p1`. Record 0x2811a of unit 1. Role unknown.
- `advance` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x30518`, 1936 bytes, static).
  Returns `int`; Parameters `const char *lp, char *ep`. Record 0x281a1 of unit 1. Role unknown.
- `backref` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x30ca8`, 272 bytes, static).
  Returns `int`; Parameters `int i, const char *lp`. Record 0x28260 of unit 1. Role unknown.
- `cclass` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x30db8`, 204 bytes, static).
  Returns `int`; Parameters `char *set, char c, int af`. Record 0x282d2 of unit 1. Role unknown.
- `glob2regex` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x30e84`, 412 bytes, static).
  No return type in the record; Parameters `char *out, const char *in`. Record 0x28357 of unit 1. Role unknown.
- `strtoargs` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x31020`, 700 bytes, static).
  No return type in the record; Parameters `char *buf, int *ac, char * *ap`. Record 0x283c5 of unit 1. Role unknown.
- `_padstr` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x312dc`, 476 bytes, external).
  Returns `char *`; Parameters `char *buf, int len`. Record 0x2846d of unit 1. Role unknown.
- `_padstrn` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x314b8`, 560 bytes, external).
  Returns `char *`; Parameters `char *buf, int buflen, int padlen`. Record 0x2850a of unit 1. Role unknown.
- `padnum` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x316e8`, 880 bytes, external).
  Returns `char *`; Parameters `__psint_t num, int base, int len`. Record 0x285c3 of unit 1. Role unknown.
- `padnum64` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x31a58`, 880 bytes, external).
  Returns `char *`; Parameters `__int64_t num, int base, int len`. Record 0x286ad of unit 1. Role unknown.
- `pad_utrace_qual` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x31dc8`, 288 bytes, static).
  Returns `char *`; Parameters `__uint64_t qual`. Record 0x28799 of unit 1. Role unknown.
- `_mcount` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x31ee8`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x28817 of unit 1. Role unknown.
- `dumptraplog_entry` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x31ef4`, 2248 bytes, external).
  No return type in the record; Parameters `__uint64_t *entry, __uint32_t pda_traplog_mask, __uint32_t pda_traplog_strace_size`. Record 0x28849 of unit 1. Role unknown.
- `dumptraplog` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x327bc`, 316 bytes, external).
  No return type in the record; Parameters `char *pdapage, __uint32_t pda_traplog_mask, __uint32_t pda_traplog_dumpcnt, __uint32_t pda_traplog_strace_size, __uint32_t pda_traplog_entry_size`. Record 0x2893e of unit 1. Role unknown.
- `idbg_traplog` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x328f8`, 1484 bytes, external).
  No return type in the record; Parameters `__psint_t id`. Record 0x28a51 of unit 1. Role unknown.
- `idbg_print_cpumon` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x32ec4`, 576 bytes, external).
  No return type in the record; Parameters `cpu_mon_t *cmp`. Record 0x28b69 of unit 1. Role unknown.
- `idbg_cpumon` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x33104`, 1036 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x28bcc of unit 1. Role unknown.
- `start_event_counters` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x33548`, 1636 bytes, external).
  No return type in the record; Parameters `int mode, short event0, short event1, short event2, short event3`. Record 0x28c7f of unit 1. Role unknown.
- `stop_event_counters` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x340a4`, 868 bytes, external).
  No return type in the record; Parameters `int counters`. Record 0x28e52 of unit 1. Role unknown.
- `idbg_setpri` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34458`, 572 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x28edd of unit 1. Role unknown.
- `ec0_down` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34694`, 716 bytes, external).
  Returns `int`; no parameter records. Record 0x28f9b of unit 1. Role unknown.
- `ec0_up` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34960`, 516 bytes, external).
  Returns `int`; no parameter records. Record 0x29011 of unit 1. Role unknown.
- `getrbase` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34b64`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x2909c of unit 1. Role unknown.
- `idbg_etrace` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34b70`, 116 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x290cf of unit 1. Role unknown.
- `idbg_nmidump` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34eb4`, 64 bytes, external).
  No return type in the record; Parameters `int cmd`. Record 0x29174 of unit 1. Role unknown.
- `uthreadp_is_valid` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34ef4`, 244 bytes, external).
  Returns `int`; Parameters `uthread_t *ut`. Record 0x291c0 of unit 1. Role unknown.
- `procp_is_valid` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x34fe8`, 260 bytes, external).
  Returns `int`; Parameters `proc_t *p`. Record 0x29218 of unit 1. Role unknown.
- `idbg_patch_instruction` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x350ec`, 724 bytes, external).
  No return type in the record; Parameters `__int32_t *p, __psint_t a2, int argc, char * *argv`. Record 0x2926c of unit 1. Role unknown.
- `idbg_dstat` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x353c0`, 364 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x29382 of unit 1. Role unknown.
- `idbg_dstats` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x3552c`, 1060 bytes, external).
  No return type in the record; Parameters `uint64_t flag`. Record 0x293fd of unit 1. Role unknown.
- `idbg_retarget` (`6.5.30/usr/cpu/sysgen/IP35boot/idbg.o` `0x35950`, 1532 bytes, external).
  No return type in the record; Parameters `void *p, void *a2, int argc, char * *argv`. Record 0x294e1 of unit 1. Role unknown.

### `6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o`

- `melflkaddr` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0x0`, 780 bytes, static).
  No return type in the record; Parameters `ml_info_t *m, __psint_t addr`. Record 0x18af of unit 1. Role unknown.
- `idbg_mlist` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0x30c`, 2796 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x1990 of unit 1. Role unknown.
- `idbg_msyms` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0xdf8`, 488 bytes, external).
  No return type in the record; Parameters `int id`. Record 0x1a59 of unit 1. Role unknown.
- `idbg_mlkup` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0xfe0`, 360 bytes, external).
  No return type in the record; Parameters `char *name`. Record 0x1aea of unit 1. Role unknown.
- `idbg_mlkaddr` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0x1148`, 156 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x1b6b of unit 1. Role unknown.
- `mflags` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0x11e4`, 636 bytes, static).
  No return type in the record; Parameters `int flags`. Record 0x1bc8 of unit 1. Role unknown.
- `mtype` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0x1460`, 832 bytes, static).
  No return type in the record; Parameters `int type`. Record 0x1c0f of unit 1. Role unknown.
- `idbg_mlinfo` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0x17a0`, 1316 bytes, external).
  No return type in the record; Parameters `ml_info_t *m`. Record 0x1c54 of unit 1. Role unknown.
- `mloadidbgunload` (`6.5.30/usr/cpu/sysgen/IP35boot/mloadidbg.o` `0x1f28`, 420 bytes, external).
  Returns `int`; no parameter records. Record 0x1ce2 of unit 1. Role unknown.
### `6.5.30/usr/cpu/sysgen/IP27boot/idbg.o`

- `kqueue_first` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x0`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1f4bd of unit 1. Role unknown.
- `kqueue_next` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1c`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1f510 of unit 1. Role unknown.
- `kqueue_end` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x38`, 24 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1f562 of unit 1. Role unknown.
- `VPAG_GETPRID` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x58`, 80 bytes, static).
  Returns `prid_t`; Parameters `vpagg_t *vpag`. Record 0x1f5de of unit 1. Role unknown.
- `VPAG_EXTRACT_ARSESS_INFO` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xa8`, 112 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag, arsess_t *arsess_info`. Record 0x1f667 of unit 1. Role unknown.
- `mutexmeter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x118`, 88 bytes, static).
  Returns `mmeter_t *`; Parameters `mutex_t *mp`. Record 0x1f6f0 of unit 1. Role unknown.
- `svmeter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x170`, 88 bytes, static).
  Returns `svmeter_t *`; Parameters `sv_t *svp`. Record 0x1f740 of unit 1. Role unknown.
- `smeter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1c8`, 88 bytes, static).
  Returns `k_semameter_t *`; Parameters `sema_t *sp`. Record 0x1f78e of unit 1. Role unknown.
- `mrmeter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x220`, 88 bytes, static).
  Returns `k_mrmeter_t *`; Parameters `mrlock_t *mrp`. Record 0x1f7da of unit 1. Role unknown.
- `_idbg_unload` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x740`, 88 bytes, external).
  Returns `int`; no parameter records. Record 0x1f916 of unit 1. Role unknown.
- `_idbg_addfssw` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x798`, 88 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x1f94d of unit 1. Role unknown.
- `_idbg_delfssw` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x7f0`, 156 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x1f998 of unit 1. Role unknown.
- `idbg_findfssw_vfs` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x88c`, 200 bytes, external).
  Returns `idbgfssw_t *`; Parameters `vfs_t *vfsp, bhv_desc_t * *bdp`. Record 0x1fa00 of unit 1. Role unknown.
- `idbg_findfssw_vnbhv` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x954`, 148 bytes, external).
  Returns `idbgfssw_t *`; Parameters `bhv_desc_t *bdp`. Record 0x1fa83 of unit 1. Role unknown.
- `_idbg_addfunc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x9e8`, 1584 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a subroutine type)`. Record 0x1faf3 of unit 1. Role unknown.
- `idbg_findfunc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1018`, 780 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a pointer to a subroutine type)`. Record 0x1fba4 of unit 1. Role unknown.
- `_idbg_delfunc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1324`, 432 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type)`. Record 0x1fc48 of unit 1. Role unknown.
- `_idbg_copytab` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x14d4`, 792 bytes, external).
  Returns `int`; Parameters `caddr_t *uaddr, int len, rval_t *rvp`. Record 0x1fcd3 of unit 1. Role unknown.
- `_idbg_tablesize` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x17ec`, 128 bytes, external).
  No return type in the record; Parameters `rval_t *rvp`. Record 0x1fdc2 of unit 1. Role unknown.
- `_idbg_dofunc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x186c`, 2376 bytes, external).
  Returns `int`; Parameters `struct idbgcomm *s, rval_t *rvp`. Record 0x1fe11 of unit 1. Role unknown.
- `_qprintf` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x21b4`, 788 bytes, external).
  No return type in the record; Parameters `char *fmt, va_list ap`. Record 0x1fffb of unit 1. Role unknown.
- `_idbg_error` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x24c8`, 104 bytes, external).
  Returns `int`; no parameter records. Record 0x2008e of unit 1. Role unknown.
- `_idbg_switch` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2530`, 488 bytes, external).
  No return type in the record; Parameters `int snum, int onoff`. Record 0x200c4 of unit 1. Role unknown.
- `idbg_string` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2718`, 92 bytes, external).
  No return type in the record; Parameters `int addr`. Record 0x20139 of unit 1. Role unknown.
- `_printflags` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2774`, 484 bytes, external).
  No return type in the record; Parameters `uint64_t flags, char * *strings, char *name`. Record 0x20184 of unit 1. Role unknown.
- `idbg_hd` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2958`, 464 bytes, external).
  No return type in the record; Parameters `unsigned int *addr, __psint_t len`. Record 0x2022a of unit 1. Role unknown.
- `ktrace_nentries` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2b28`, 116 bytes, external).
  Returns `int`; Parameters `ktrace_t *ktp`. Record 0x202aa of unit 1. Role unknown.
- `ktrace_first` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2b9c`, 236 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x20301 of unit 1. Role unknown.
- `ktrace_next` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2c88`, 192 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x203ae of unit 1. Role unknown.
- `ktrace_skip` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2d48`, 348 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, int count, ktrace_snap_t *ktsp`. Record 0x20441 of unit 1. Role unknown.
- `printtimers` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2ea4`, 820 bytes, external).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x2051e of unit 1. Role unknown.
- `idbg_kthreadname` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x31d8`, 188 bytes, external).
  Returns `char *`; Parameters `kthread_t *kt`. Record 0x20598 of unit 1. Role unknown.
- `idbg_polldat` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x3294`, 264 bytes, external).
  No return type in the record; Parameters `struct polldat *p`. Record 0x20604 of unit 1. Role unknown.
- `idbg_pollhead` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x339c`, 328 bytes, external).
  No return type in the record; Parameters `struct pollhead *p`. Record 0x2064e of unit 1. Role unknown.
- `idbg_proxy` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x3750`, 1080 bytes, external).
  No return type in the record; Parameters `proc_proxy_t *prxy`. Record 0x2073d of unit 1. Role unknown.
- `pcred` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x3b88`, 636 bytes, external).
  No return type in the record; Parameters `cred_t *crp`. Record 0x20788 of unit 1. Role unknown.
- `idbg_doto_uthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x3e04`, 572 bytes, external).
  Returns `int`; Parameters `__psint_t p1, pfunc (pointer to a subroutine type), int all, int must`. Record 0x207df of unit 1. Role unknown.
- `idbg_do_uthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x4040`, 3840 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x208a8 of unit 1. Role unknown.
- `idbg_uthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x4f40`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x2096e of unit 1. Role unknown.
- `idbg_proc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x4fa0`, 2692 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x209b9 of unit 1. Role unknown.
- `sleepon` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x5a2c`, 1424 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x20a99 of unit 1. Role unknown.
- `dumpssleep` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x5fbc`, 128 bytes, static).
  No return type in the record; Parameters `ssleep_t *st`. Record 0x20ba6 of unit 1. Role unknown.
- `idbg_pchain` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x603c`, 616 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x20bee of unit 1. Role unknown.
- `issthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x62a4`, 188 bytes, static).
  Returns `int`; Parameters `sthread_t *x`. Record 0x20c61 of unit 1. Role unknown.
- `isxthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x6360`, 188 bytes, static).
  Returns `int`; Parameters `xthread_t *x`. Record 0x20cc2 of unit 1. Role unknown.
- `kstack` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x641c`, 576 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x20d23 of unit 1. Role unknown.
- `idbg_slist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x665c`, 548 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20da9 of unit 1. Role unknown.
- `idbg_xlist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x6880`, 548 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20e17 of unit 1. Role unknown.
- `idbg_klist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x6aa4`, 244 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20e85 of unit 1. Role unknown.
- `kid_is_uthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x6b98`, 172 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x20ee3 of unit 1. Role unknown.
- `find_kthreadid` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x6c44`, 880 bytes, static).
  Returns `int`; Parameters `uint64_t id, int verbose, __psunsigned_t *result`. Record 0x20f93 of unit 1. Role unknown.
- `idbg_kthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x6fb4`, 2820 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x21079 of unit 1. Role unknown.
- `idbg_sthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x7ab8`, 588 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2114f of unit 1. Role unknown.
- `idbg_xthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x7d04`, 848 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x211bf of unit 1. Role unknown.
- `print_proc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x8054`, 96 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x2122f of unit 1. Role unknown.
- `idbg_plist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x80b4`, 360 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x212a7 of unit 1. Role unknown.
- `print_proc_fdt` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x821c`, 360 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x21302 of unit 1. Role unknown.
- `idbg_plfdt` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x8384`, 336 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2137e of unit 1. Role unknown.
- `showptree` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x84d4`, 332 bytes, static).
  No return type in the record; Parameters `proc_t *parent, int level`. Record 0x213ea of unit 1. Role unknown.
- `idbg_ptree` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x8620`, 308 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x21488 of unit 1. Role unknown.
- `idbg_pgrp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x8754`, 456 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x214e4 of unit 1. Role unknown.
- `gdbg_proc_print` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x891c`, 356 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x2155c of unit 1. Role unknown.
- `gdbg_owner_print` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x8a80`, 212 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x21629 of unit 1. Role unknown.
- `idbg_pagg` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x8b54`, 2588 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x216e3 of unit 1. Role unknown.
- `showpagtree` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x9570`, 360 bytes, static).
  No return type in the record; Parameters `proc_t *root, proc_t *parent, int level`. Record 0x217ab of unit 1. Role unknown.
- `idbg_pagtree` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x96d8`, 564 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x21861 of unit 1. Role unknown.
- `idbg_paglist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x990c`, 432 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x21904 of unit 1. Role unknown.
- `proc_pagfind` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x9abc`, 120 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x21989 of unit 1. Role unknown.
- `idbg_pagfind` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x9b34`, 308 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x21a18 of unit 1. Role unknown.
- `shortuthread` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x9c68`, 560 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x21a8e of unit 1. Role unknown.
- `shortproc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x9e98`, 760 bytes, external).
  No return type in the record; Parameters `proc_t *pp, int ohboy`. Record 0x21ad8 of unit 1. Role unknown.
- `tinyproc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xa190`, 172 bytes, static).
  No return type in the record; Parameters `proc_t *p`. Record 0x21b5a of unit 1. Role unknown.
- `shortvpag` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xa23c`, 340 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag`. Record 0x21b9f of unit 1. Role unknown.
- `prksigset` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xa390`, 468 bytes, static).
  No return type in the record; Parameters `char *str, k_sigset_t *ks`. Record 0x21be8 of unit 1. Role unknown.
- `idbg_do_signal` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xa564`, 1576 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x21c67 of unit 1. Role unknown.
- `idbg_signal` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xab8c`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21cee of unit 1. Role unknown.
- `idbg_ptrace` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xabec`, 444 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21d38 of unit 1. Role unknown.
- `idbg_prnode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xada8`, 768 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21dac of unit 1. Role unknown.
- `idbg_prnodefree` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xb0a8`, 1256 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x21e36 of unit 1. Role unknown.
- `idbg_watch` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xb590`, 964 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21eae of unit 1. Role unknown.
- `print_sleepers` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xb954`, 808 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x21f3c of unit 1. Role unknown.
- `idbg_slpproc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xbc7c`, 128 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2203c of unit 1. Role unknown.
- `idbg_priscan` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xbd8c`, 232 bytes, static).
  No return type in the record; Parameters `prilist_t *mp`. Record 0x220ca of unit 1. Role unknown.
- `idbg_printputbuf` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xbe74`, 224 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22114 of unit 1. Role unknown.
- `dumpminfostats` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xbf54`, 824 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x22162 of unit 1. Role unknown.
- `idbg_minfo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xc828`, 232 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x2234a of unit 1. Role unknown.
- `dumpsysinfostats` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xc910`, 1096 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x22392 of unit 1. Role unknown.
- `idbg_sysinfo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xcd58`, 88 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x223fe of unit 1. Role unknown.
- `idbg_gbuf` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xcdb0`, 2772 bytes, static).
  No return type in the record; Parameters `buf_t *bp, int idx, buf_t *bufhead`. Record 0x22448 of unit 1. Role unknown.
- `dumppdabstats` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xdc84`, 1008 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x2252b of unit 1. Role unknown.
- `idbg_fbuf` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xe074`, 1312 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x22576 of unit 1. Role unknown.
- `idbg_vbuf` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xe594`, 536 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x22655 of unit 1. Role unknown.
- `idbg_findbuf` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xe7ac`, 416 bytes, external).
  No return type in the record; Parameters `int blkno`. Record 0x2271d of unit 1. Role unknown.
- `idbg_dport` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xe94c`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x227aa of unit 1. Role unknown.
- `idbg_dportrset` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xe98c`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x227f3 of unit 1. Role unknown.
- `idbg_pty` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xe9cc`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x22840 of unit 1. Role unknown.
- `idbg_do_shaddr` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xea0c`, 1816 bytes, external).
  No return type in the record; Parameters `shaddr_t *sa`. Record 0x22886 of unit 1. Role unknown.
- `idbg_do_ut_shaddr` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xf124`, 176 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x228f7 of unit 1. Role unknown.
- `idbg_shaddr` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xf1d4`, 168 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x2295a of unit 1. Role unknown.
- `idbg_vnode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xf27c`, 1684 bytes, external).
  Returns `vnode_t *`; Parameters `__psint_t p1`. Record 0x229a4 of unit 1. Role unknown.
- `vnops_to_vfstype` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xf910`, 220 bytes, static).
  Returns `int`; Parameters `vnodeops_t *vnops`. Record 0x22ab0 of unit 1. Role unknown.
- `idbg_bhvh` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xf9ec`, 352 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22b1b of unit 1. Role unknown.
- `idbg_bhv` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xfb4c`, 300 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22b77 of unit 1. Role unknown.
- `idbg_vnbhv` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0xfc78`, 2016 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22c19 of unit 1. Role unknown.
- `idbg_extents` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x10458`, 1024 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x22d48 of unit 1. Role unknown.
- `idbg_prfifo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x10858`, 2180 bytes, external).
  No return type in the record; Parameters `struct fifonode *fnp, int header`. Record 0x22e37 of unit 1. Role unknown.
- `idbg_prinode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x110dc`, 1392 bytes, external).
  No return type in the record; Parameters `struct inode *ip, vtype_t vtype, int header`. Record 0x22e9a of unit 1. Role unknown.
- `idbg_prnamenode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1164c`, 720 bytes, external).
  No return type in the record; Parameters `struct namenode *nmp, int header`. Record 0x22f13 of unit 1. Role unknown.
- `idbg_prlsnode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1191c`, 2028 bytes, external).
  No return type in the record; Parameters `struct lsnode *lsp`. Record 0x22f79 of unit 1. Role unknown.
- `idbg_prcsnode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x12108`, 1996 bytes, external).
  No return type in the record; Parameters `struct csnode *csp`. Record 0x22fdb of unit 1. Role unknown.
- `prvn` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x128d4`, 616 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x2303d of unit 1. Role unknown.
- `__prvn` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x12b3c`, 72 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x230aa of unit 1. Role unknown.
- `prvfsvn` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x12b84`, 1208 bytes, static).
  No return type in the record; Parameters `struct vfs *vfsp, int all`. Record 0x23103 of unit 1. Role unknown.
- `idbg_fifo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1303c`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x231f8 of unit 1. Role unknown.
- `idbg_fifolist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1309c`, 516 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23250 of unit 1. Role unknown.
- `idbg_namelist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x132a0`, 516 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x232c2 of unit 1. Role unknown.
- `idbg_vlist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x134a4`, 1908 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23334 of unit 1. Role unknown.
- `dumppdaistats` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x13c18`, 744 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x2340c of unit 1. Role unknown.
- `do_efs` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x13f00`, 1944 bytes, static).
  No return type in the record; Parameters `struct efs *fs, int summary`. Record 0x23457 of unit 1. Role unknown.
- `idbg_efs` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x14698`, 68 bytes, external).
  No return type in the record; Parameters `struct efs *fs`. Record 0x2353a of unit 1. Role unknown.
- `idbg_quota` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x146dc`, 1216 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23581 of unit 1. Role unknown.
- `idbg_dqb` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x14b9c`, 1364 bytes, external).
  No return type in the record; Parameters `struct dquot *dqp`. Record 0x2366d of unit 1. Role unknown.
- `idbg_vfs` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x150f0`, 392 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23729 of unit 1. Role unknown.
- `prvfs` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x15278`, 1764 bytes, external).
  No return type in the record; Parameters `struct vfs *vfsp, int summary`. Record 0x23795 of unit 1. Role unknown.
- `idbg_vnmap` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1595c`, 400 bytes, external).
  No return type in the record; Parameters `__psunsigned_t addr`. Record 0x2388d of unit 1. Role unknown.
- `print_fmode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x15aec`, 240 bytes, static).
  No return type in the record; Parameters `int fmode`. Record 0x23939 of unit 1. Role unknown.
- `idbg_bmapval` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x15bdc`, 360 bytes, external).
  No return type in the record; Parameters `struct bmapval *bmapp`. Record 0x239b0 of unit 1. Role unknown.
- `idbg_uio` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x15d7c`, 756 bytes, external).
  No return type in the record; Parameters `uio_t *uiop`. Record 0x23a5b of unit 1. Role unknown.
- `idbg_open` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16070`, 144 bytes, external).
  No return type in the record; Parameters `struct opena *uap`. Record 0x23ae0 of unit 1. Role unknown.
- `idbg_creat` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16100`, 104 bytes, external).
  No return type in the record; Parameters `struct creata *uap`. Record 0x23b29 of unit 1. Role unknown.
- `idbg_mkdir` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16168`, 104 bytes, external).
  No return type in the record; Parameters `struct mkdira *uap`. Record 0x23b73 of unit 1. Role unknown.
- `idbg_link` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x161d0`, 104 bytes, external).
  No return type in the record; Parameters `struct linka *uap`. Record 0x23bbd of unit 1. Role unknown.
- `idbg_rename` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16238`, 104 bytes, external).
  No return type in the record; Parameters `struct renamea *uap`. Record 0x23c06 of unit 1. Role unknown.
- `idbg_symlink` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x162a0`, 104 bytes, external).
  No return type in the record; Parameters `struct symlinka *uap`. Record 0x23c51 of unit 1. Role unknown.
- `idbg_unlink` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16308`, 96 bytes, external).
  No return type in the record; Parameters `struct unlinka *uap`. Record 0x23c9d of unit 1. Role unknown.
- `idbg_rmdir` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16368`, 96 bytes, external).
  No return type in the record; Parameters `struct rmdira *uap`. Record 0x23ce8 of unit 1. Role unknown.
- `idbg_xstat` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x163c8`, 112 bytes, external).
  No return type in the record; Parameters `struct xstatarg *uap`. Record 0x23d32 of unit 1. Role unknown.
- `idbg_dumpsym` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16438`, 368 bytes, external).
  No return type in the record; Parameters `char * *addr`. Record 0x23d7c of unit 1. Role unknown.
- `idbg_callo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x165a8`, 656 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23ded of unit 1. Role unknown.
- `dump_callo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16838`, 472 bytes, static).
  No return type in the record; Parameters `struct callout *p1`. Record 0x23e47 of unit 1. Role unknown.
- `idbg_calloinfo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16a10`, 476 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23ea5 of unit 1. Role unknown.
- `dump_callout_info` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16bec`, 620 bytes, static).
  No return type in the record; Parameters `callout_info_t *cip`. Record 0x23f10 of unit 1. Role unknown.
- `idbg_ptimer` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x16e58`, 532 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23f88 of unit 1. Role unknown.
- `idbg_file` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1706c`, 708 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2402e of unit 1. Role unknown.
- `print_procfdt` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x17330`, 400 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x240d7 of unit 1. Role unknown.
- `idbg_vfile` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x174c0`, 300 bytes, external).
  No return type in the record; Parameters `__psint_t ptr`. Record 0x241a1 of unit 1. Role unknown.
- `ltype_to_str` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x175ec`, 464 bytes, external).
  Returns `char *`; Parameters `int ltype`. Record 0x241fe of unit 1. Role unknown.
- `idbg_filock` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x177bc`, 232 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x2429a of unit 1. Role unknown.
- `idbg_sleeplcks` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x178a4`, 344 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x2430c of unit 1. Role unknown.
- `idbg_flock` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x179fc`, 188 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x24381 of unit 1. Role unknown.
- `printfp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x17ab8`, 1124 bytes, static).
  Returns `int`; Parameters `vfile_t *fp, int slot, int flag`. Record 0x243eb of unit 1. Role unknown.
- `idbg_map` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x17f1c`, 244 bytes, external).
  No return type in the record; Parameters `struct map *p`. Record 0x244b1 of unit 1. Role unknown.
- `idbg_dobitmap` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x18010`, 560 bytes, external).
  No return type in the record; Parameters `struct bitmap *bp, char *name`. Record 0x24508 of unit 1. Role unknown.
- `idbg_chbtmp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x18240`, 332 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2457b of unit 1. Role unknown.
- `idbg_bitmap` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1838c`, 928 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2462a of unit 1. Role unknown.
- `idbg_svdump` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1872c`, 1612 bytes, external).
  No return type in the record; Parameters `sv_t *svp`. Record 0x246ae of unit 1. Role unknown.
- `idbg_pwait` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x18d78`, 472 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x24735 of unit 1. Role unknown.
- `idbg_page_counter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x18f50`, 20 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x24794 of unit 1. Role unknown.
- `idbg_shdump` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x18f6c`, 1060 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x2481d of unit 1. Role unknown.
- `idbg_semchain` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x19390`, 816 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x248a1 of unit 1. Role unknown.
- `idbg_nschain` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x196c0`, 1132 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x24910 of unit 1. Role unknown.
- `scanchain` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x19b2c`, 512 bytes, static).
  No return type in the record; Parameters `int x`. Record 0x249b8 of unit 1. Role unknown.
- `idbgplog` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x19d2c`, 792 bytes, external).
  No return type in the record; Parameters `semahist_t *hp`. Record 0x24a25 of unit 1. Role unknown.
- `idbg_semctl` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1a044`, 576 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x24a99 of unit 1. Role unknown.
- `semctl_apply` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1a284`, 836 bytes, static).
  Returns `int`; Parameters `char *str, func (pointer to a subroutine type), int ac, char * *av`. Record 0x24b45 of unit 1. Role unknown.
- `semctl_describe` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1a5c8`, 232 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x24cb2 of unit 1. Role unknown.
- `semctl_ctlmeter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1a6b0`, 3256 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x24d25 of unit 1. Role unknown.
- `semctl_ctlmeter_aux` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1b368`, 2532 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x24e88 of unit 1. Role unknown.
- `semctl_defhist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1bd4c`, 828 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2508d of unit 1. Role unknown.
- `semctl_defkhist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1c088`, 760 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x250ec of unit 1. Role unknown.
- `semctl_defkhlen` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1c380`, 388 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2514c of unit 1. Role unknown.
- `semctl_defmeter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1c504`, 760 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x251d2 of unit 1. Role unknown.
- `semctl_help` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1c7fc`, 224 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x25232 of unit 1. Role unknown.
- `semctl_histmax` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1c8dc`, 360 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x252a3 of unit 1. Role unknown.
- `semctl_sema` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1ca44`, 188 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2532b of unit 1. Role unknown.
- `semctl_semlog` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1cb00`, 3200 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x25387 of unit 1. Role unknown.
- `semctl_what` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1d780`, 188 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2553f of unit 1. Role unknown.
- `printid` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1d83c`, 552 bytes, static).
  No return type in the record; Parameters `kthread_t *kt, uint64_t id, char *str`. Record 0x2559b of unit 1. Role unknown.
- `idbg_semdump` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1da64`, 1620 bytes, external).
  No return type in the record; Parameters `sema_t *sp`. Record 0x2562f of unit 1. Role unknown.
- `idbg_mutexdump` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1e0b8`, 1316 bytes, external).
  No return type in the record; Parameters `mutex_t *mp`. Record 0x256b6 of unit 1. Role unknown.
- `idbg_dumphisto` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1e5dc`, 1656 bytes, static).
  No return type in the record; Parameters `k_histelem_t *khp, char *str, int timed`. Record 0x2573f of unit 1. Role unknown.
- `idbg_semalist` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1ec54`, 1436 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x25831 of unit 1. Role unknown.
- `idbg_chklock` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1f1f0`, 1608 bytes, external).
  No return type in the record; Parameters `void *l`. Record 0x258f2 of unit 1. Role unknown.
- `idbg_avlnode` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1f838`, 344 bytes, external).
  No return type in the record; Parameters `avlnode_t *avl`. Record 0x25951 of unit 1. Role unknown.
- `idbg_lockmeter` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1f990`, 1588 bytes, external).
  No return type in the record; Parameters `k_splockmeter_t *spm`. Record 0x2599d of unit 1. Role unknown.
- `islock` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x1ffc4`, 360 bytes, static).
  Returns `char *`; Parameters `void *lck`. Record 0x259eb of unit 1. Role unknown.
- `idbg_hotlocks` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2012c`, 72 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x25a5d of unit 1. Role unknown.
- `dumphotlock` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x20174`, 2876 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x25abb of unit 1. Role unknown.
- `idbg_coldlocks` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x20cb0`, 528 bytes, external).
  No return type in the record; Parameters `int n`. Record 0x25bb4 of unit 1. Role unknown.
- `idbg_mrlock` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x20ec0`, 3844 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x25c75 of unit 1. Role unknown.
- `idbg_mslock` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x21dc4`, 408 bytes, external).
  No return type in the record; Parameters `mslock_t *msp`. Record 0x25dda of unit 1. Role unknown.
- `idbg_mrlockfull` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x21f5c`, 4956 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x25e25 of unit 1. Role unknown.
- `idbg_pda` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x232b8`, 148 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x25f17 of unit 1. Role unknown.
- `do_pdas` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2334c`, 480 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type), int which`. Record 0x25f5d of unit 1. Role unknown.
- `dumppda` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2352c`, 4764 bytes, external).
  No return type in the record; Parameters `pda_t *pd`. Record 0x25fcd of unit 1. Role unknown.
- `idbg_donodepda` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x247c8`, 2064 bytes, external).
  No return type in the record; Parameters `nodepda_t *npda`. Record 0x26068 of unit 1. Role unknown.
- `idbg_nodepda` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x24fd8`, 332 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2618f of unit 1. Role unknown.
- `idbg_uarea` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x25124`, 540 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x261eb of unit 1. Role unknown.
- `idbg_upage` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x25340`, 624 bytes, external).
  No return type in the record; Parameters `proc_t *pp`. Record 0x26246 of unit 1. Role unknown.
- `idbg_fdt` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x255b0`, 464 bytes, external).
  No return type in the record; Parameters `fdt_t *fdt`. Record 0x2628f of unit 1. Role unknown.
- `idbg_runq` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x25780`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t howmany`. Record 0x26308 of unit 1. Role unknown.
- `dumpRunq` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x257c0`, 1900 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x26355 of unit 1. Role unknown.
- `idbg_dopglst` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x25f2c`, 288 bytes, static).
  No return type in the record; Parameters `pglst_t *pglptr`. Record 0x263d7 of unit 1. Role unknown.
- `idbg_swap` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2604c`, 592 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2643a of unit 1. Role unknown.
- `idbg_pglst` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2629c`, 540 bytes, external).
  No return type in the record; Parameters `__psint_t i`. Record 0x264a6 of unit 1. Role unknown.
- `idbg_kaddr` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x264b8`, 324 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x2650b of unit 1. Role unknown.
- `idbg_do_eframe` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x265fc`, 536 bytes, external).
  No return type in the record; Parameters `eframe_t *ep`. Record 0x26580 of unit 1. Role unknown.
- `idbg_do_ut_eframe` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x26814`, 160 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x265e2 of unit 1. Role unknown.
- `idbg_eframe` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x268b4`, 204 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x26645 of unit 1. Role unknown.
- `idbg_ubtrace` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x26980`, 1912 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x2668e of unit 1. Role unknown.
- `convsp_start` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x270f8`, 96 bytes, static).
  Returns `int`; Parameters `struct uthread_s *ut`. Record 0x2679c of unit 1. Role unknown.
- `convsp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2717c`, 128 bytes, static).
  Returns `unsigned int *`; Parameters `unsigned int *sp, unsigned int *physsp`. Record 0x2681a of unit 1. Role unknown.
- `idbg_get_stack_bytes` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x271fc`, 304 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut, void *sp, void *dp, int n`. Record 0x26891 of unit 1. Role unknown.
- `fetch_kaddr` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2732c`, 472 bytes, external).
  Returns `__psunsigned_t`; Parameters `char *s`. Record 0x26945 of unit 1. Role unknown.
- `lookup_kname` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x27504`, 416 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x26a1b of unit 1. Role unknown.
- `fetch_kname` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x276a4`, 140 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x26aaf of unit 1. Role unknown.
- `_prsymoff` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x27730`, 468 bytes, external).
  No return type in the record; Parameters `void *addr, char *pre, char *post`. Record 0x26b4a of unit 1. Role unknown.
- `idbg_btrace` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x27904`, 148 bytes, external).
  No return type in the record; Parameters `int maxfr`. Record 0x26be9 of unit 1. Role unknown.
- `btrace` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x27998`, 9364 bytes, static).
  No return type in the record; Parameters `unsigned int *pc, unsigned int *sp, unsigned int *lra, int maxfr, unsigned int *lbound, kthread_t *kt, int myself`. Record 0x26c36 of unit 1. Role unknown.
- `txtsrch` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x29e2c`, 1152 bytes, static).
  Returns `inst_t *`; Parameters `inst_t *pc, unsigned int inst1mask, unsigned int inst2mask, int *which, enum sign im1sign, enum sign im2sign, int *im_ptr, inst_t *llimit, struct spec_funcs * *specf, enum direction dir, unsigned int check1mask, unsigned int check2mask`. Record 0x2700e of unit 1. Role unknown.
- `is_jal` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a434`, 168 bytes, static).
  Returns `int`; Parameters `inst_t inst`. Record 0x27294 of unit 1. Role unknown.
- `branch_target` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a4dc`, 240 bytes, static).
  Returns `inst_t *`; Parameters `inst_t inst, inst_t *pc`. Record 0x272f4 of unit 1. Role unknown.
- `fetch_text` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a5cc`, 308 bytes, static).
  Returns `inst_t`; Parameters `inst_t *pc`. Record 0x2736e of unit 1. Role unknown.
- `ilastframe` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a700`, 76 bytes, static).
  Returns `int`; Parameters `k_machreg_t *sp`. Record 0x273e6 of unit 1. Role unknown.
- `idbg_symtab` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a74c`, 88 bytes, external).
  No return type in the record; Parameters `struct dbstbl * *db, char * *name`. Record 0x27436 of unit 1. Role unknown.
- `idbg_symsize` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a7a4`, 56 bytes, external).
  No return type in the record; Parameters `int *size`. Record 0x27495 of unit 1. Role unknown.
- `idbg_ksymrange` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a7dc`, 88 bytes, external).
  No return type in the record; Parameters `void * *_kftext, void * *_kend`. Record 0x274e2 of unit 1. Role unknown.
- `idbg_scsi` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2a834`, 892 bytes, external).
  No return type in the record; Parameters `struct scsi_request *req`. Record 0x2754a of unit 1. Role unknown.
- `idbg_foi` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2abb0`, 1372 bytes, external).
  No return type in the record; Parameters `vertex_hdl_t lun_vhdl`. Record 0x275a5 of unit 1. Role unknown.
- `dkactive` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2b10c`, 164 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x27692 of unit 1. Role unknown.
- `dkqueue` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2b1b0`, 564 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x276ec of unit 1. Role unknown.
- `idbg_dksoftc` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2b3e4`, 1164 bytes, external).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x27745 of unit 1. Role unknown.
- `idbg_xpr` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2b8c0`, 116 bytes, external).
  No return type in the record; Parameters `void *xpr`. Record 0x277c2 of unit 1. Role unknown.
- `idbg_xpm` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2b9a0`, 120 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x27834 of unit 1. Role unknown.
- `intvec_tinfo` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2ba18`, 640 bytes, external).
  No return type in the record; Parameters `thd_int_t *tip`. Record 0x2787a of unit 1. Role unknown.
- `idbg_call` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2bc98`, 368 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x278c6 of unit 1. Role unknown.
- `idbg_kill` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2be08`, 508 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x2790d of unit 1. Role unknown.
- `idbg_qbuf` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2c004`, 772 bytes, external).
  No return type in the record; Parameters `__psint_t dev`. Record 0x2797b of unit 1. Role unknown.
- `idbg_dozone` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2c548`, 2084 bytes, static).
  No return type in the record; Parameters `zone_t *zp, int all`. Record 0x27a95 of unit 1. Role unknown.
- `idbg_zone` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2cd6c`, 912 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x27b49 of unit 1. Role unknown.
- `idbg_tracemasks` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2d0fc`, 364 bytes, external).
  No return type in the record; Parameters `struct pr_tracemasks *t`. Record 0x27c0d of unit 1. Role unknown.
- `idbg_tstamp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2d268`, 364 bytes, external).
  No return type in the record; Parameters `const tstamp_obj_t *objp`. Record 0x27c5b of unit 1. Role unknown.
- `utrace_dump` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2d3d4`, 2740 bytes, static).
  No return type in the record; Parameters `tstamp_event_entry_t *event, int cpu, tstamp_event_entry_t *limit, __psint_t sval`. Record 0x27ca7 of unit 1. Role unknown.
- `_idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2de88`, 3872 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2, __psint_t sval`. Record 0x27dc7 of unit 1. Role unknown.
- `idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2eda8`, 76 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x280b3 of unit 1. Role unknown.
- `idbg_utrace_find` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2edf4`, 136 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x28114 of unit 1. Role unknown.
- `idbg_utrace_mask` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2ee7c`, 200 bytes, external).
  No return type in the record; Parameters `uint64_t arg1`. Record 0x2817a of unit 1. Role unknown.
- `idbg_do_softfp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2ef44`, 204 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x281cb of unit 1. Role unknown.
- `idbg_softfp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2f010`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x28217 of unit 1. Role unknown.
- `print_maclabel` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2f070`, 1004 bytes, external).
  No return type in the record; Parameters `char *cp, mac_label *lp`. Record 0x28261 of unit 1. Role unknown.
- `idbg_stop` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2f45c`, 64 bytes, external).
  No return type in the record; Parameters `void *stoplist`. Record 0x282eb of unit 1. Role unknown.
- `idbg_ioerror` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2f52c`, 1552 bytes, static).
  No return type in the record; Parameters `__psunsigned_t arg`. Record 0x28367 of unit 1. Role unknown.
- `re_comp` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x2fb3c`, 2328 bytes, external).
  Returns `char *`; Parameters `const char *sp`. Record 0x283d5 of unit 1. Role unknown.
- `re_exec` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x30454`, 592 bytes, external).
  Returns `int`; Parameters `const char *p1`. Record 0x28503 of unit 1. Role unknown.
- `advance` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x306a4`, 1936 bytes, static).
  Returns `int`; Parameters `const char *lp, char *ep`. Record 0x2858a of unit 1. Role unknown.
- `backref` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x30e34`, 272 bytes, static).
  Returns `int`; Parameters `int i, const char *lp`. Record 0x28649 of unit 1. Role unknown.
- `cclass` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x30f44`, 204 bytes, static).
  Returns `int`; Parameters `char *set, char c, int af`. Record 0x286bb of unit 1. Role unknown.
- `glob2regex` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x31010`, 412 bytes, static).
  No return type in the record; Parameters `char *out, const char *in`. Record 0x28740 of unit 1. Role unknown.
- `strtoargs` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x311ac`, 700 bytes, static).
  No return type in the record; Parameters `char *buf, int *ac, char * *ap`. Record 0x287ae of unit 1. Role unknown.
- `_padstr` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x31468`, 476 bytes, external).
  Returns `char *`; Parameters `char *buf, int len`. Record 0x28856 of unit 1. Role unknown.
- `_padstrn` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x31644`, 560 bytes, external).
  Returns `char *`; Parameters `char *buf, int buflen, int padlen`. Record 0x288f3 of unit 1. Role unknown.
- `padnum` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x31874`, 880 bytes, external).
  Returns `char *`; Parameters `__psint_t num, int base, int len`. Record 0x289ac of unit 1. Role unknown.
- `padnum64` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x31be4`, 880 bytes, external).
  Returns `char *`; Parameters `__int64_t num, int base, int len`. Record 0x28a96 of unit 1. Role unknown.
- `pad_utrace_qual` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x31f54`, 288 bytes, static).
  Returns `char *`; Parameters `__uint64_t qual`. Record 0x28b82 of unit 1. Role unknown.
- `_mcount` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x32074`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x28c00 of unit 1. Role unknown.
- `dumptraplog_entry` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x32080`, 2248 bytes, external).
  No return type in the record; Parameters `__uint64_t *entry, __uint32_t pda_traplog_mask, __uint32_t pda_traplog_strace_size`. Record 0x28c32 of unit 1. Role unknown.
- `dumptraplog` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x32948`, 316 bytes, external).
  No return type in the record; Parameters `char *pdapage, __uint32_t pda_traplog_mask, __uint32_t pda_traplog_dumpcnt, __uint32_t pda_traplog_strace_size, __uint32_t pda_traplog_entry_size`. Record 0x28d27 of unit 1. Role unknown.
- `idbg_traplog` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x32a84`, 1484 bytes, external).
  No return type in the record; Parameters `__psint_t id`. Record 0x28e3a of unit 1. Role unknown.
- `idbg_print_cpumon` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x33050`, 576 bytes, external).
  No return type in the record; Parameters `cpu_mon_t *cmp`. Record 0x28f52 of unit 1. Role unknown.
- `idbg_cpumon` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x33290`, 1036 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x28fb5 of unit 1. Role unknown.
- `start_event_counters` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x336d4`, 1636 bytes, external).
  No return type in the record; Parameters `int mode, short event0, short event1, short event2, short event3`. Record 0x29068 of unit 1. Role unknown.
- `stop_event_counters` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x34230`, 868 bytes, external).
  No return type in the record; Parameters `int counters`. Record 0x2923b of unit 1. Role unknown.
- `idbg_setpri` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x345e4`, 572 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x292c6 of unit 1. Role unknown.
- `ec0_down` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x34820`, 716 bytes, external).
  Returns `int`; no parameter records. Record 0x29384 of unit 1. Role unknown.
- `ec0_up` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x34aec`, 516 bytes, external).
  Returns `int`; no parameter records. Record 0x293fa of unit 1. Role unknown.
- `getrbase` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x34cf0`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x29485 of unit 1. Role unknown.
- `idbg_etrace` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x34cfc`, 116 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x294b8 of unit 1. Role unknown.
- `idbg_nmidump` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x35040`, 64 bytes, external).
  No return type in the record; Parameters `int cmd`. Record 0x2955d of unit 1. Role unknown.
- `uthreadp_is_valid` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x35080`, 244 bytes, external).
  Returns `int`; Parameters `uthread_t *ut`. Record 0x295a9 of unit 1. Role unknown.
- `procp_is_valid` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x35174`, 260 bytes, external).
  Returns `int`; Parameters `proc_t *p`. Record 0x29601 of unit 1. Role unknown.
- `idbg_patch_instruction` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x35278`, 724 bytes, external).
  No return type in the record; Parameters `__int32_t *p, __psint_t a2, int argc, char * *argv`. Record 0x29655 of unit 1. Role unknown.
- `idbg_dstat` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x3554c`, 364 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x2976b of unit 1. Role unknown.
- `idbg_dstats` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x356b8`, 1060 bytes, external).
  No return type in the record; Parameters `uint64_t flag`. Record 0x297e6 of unit 1. Role unknown.
- `idbg_retarget` (`6.5.30/usr/cpu/sysgen/IP27boot/idbg.o` `0x35adc`, 1532 bytes, external).
  No return type in the record; Parameters `void *p, void *a2, int argc, char * *argv`. Record 0x298ca of unit 1. Role unknown.

### `6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o`

- `melflkaddr` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0x0`, 780 bytes, static).
  No return type in the record; Parameters `ml_info_t *m, __psint_t addr`. Record 0x19c4 of unit 1. Role unknown.
- `idbg_mlist` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0x30c`, 2796 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x1aa5 of unit 1. Role unknown.
- `idbg_msyms` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0xdf8`, 488 bytes, external).
  No return type in the record; Parameters `int id`. Record 0x1b6e of unit 1. Role unknown.
- `idbg_mlkup` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0xfe0`, 360 bytes, external).
  No return type in the record; Parameters `char *name`. Record 0x1bff of unit 1. Role unknown.
- `idbg_mlkaddr` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0x1148`, 156 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x1c80 of unit 1. Role unknown.
- `mflags` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0x11e4`, 636 bytes, static).
  No return type in the record; Parameters `int flags`. Record 0x1cdd of unit 1. Role unknown.
- `mtype` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0x1460`, 832 bytes, static).
  No return type in the record; Parameters `int type`. Record 0x1d24 of unit 1. Role unknown.
- `idbg_mlinfo` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0x17a0`, 1316 bytes, external).
  No return type in the record; Parameters `ml_info_t *m`. Record 0x1d69 of unit 1. Role unknown.
- `mloadidbgunload` (`6.5.30/usr/cpu/sysgen/IP27boot/mloadidbg.o` `0x1f28`, 420 bytes, external).
  Returns `int`; no parameter records. Record 0x1df7 of unit 1. Role unknown.
### `6.5.30/usr/cpu/sysgen/IP30boot/idbg.o`

- `kqueue_first` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x0`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1c827 of unit 1. Role unknown.
- `kqueue_next` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1c`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1c87a of unit 1. Role unknown.
- `kqueue_end` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x38`, 24 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1c8cc of unit 1. Role unknown.
- `VPAG_GETPRID` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x58`, 80 bytes, static).
  Returns `prid_t`; Parameters `vpagg_t *vpag`. Record 0x1c948 of unit 1. Role unknown.
- `VPAG_EXTRACT_ARSESS_INFO` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xa8`, 112 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag, arsess_t *arsess_info`. Record 0x1c9d1 of unit 1. Role unknown.
- `mutexmeter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x118`, 88 bytes, static).
  Returns `mmeter_t *`; Parameters `mutex_t *mp`. Record 0x1ca5a of unit 1. Role unknown.
- `svmeter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x170`, 88 bytes, static).
  Returns `svmeter_t *`; Parameters `sv_t *svp`. Record 0x1caaa of unit 1. Role unknown.
- `smeter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1c8`, 88 bytes, static).
  Returns `k_semameter_t *`; Parameters `sema_t *sp`. Record 0x1caf8 of unit 1. Role unknown.
- `mrmeter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x220`, 88 bytes, static).
  Returns `k_mrmeter_t *`; Parameters `mrlock_t *mrp`. Record 0x1cb44 of unit 1. Role unknown.
- `_idbg_unload` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x740`, 88 bytes, external).
  Returns `int`; no parameter records. Record 0x1cc80 of unit 1. Role unknown.
- `_idbg_addfssw` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x798`, 88 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x1ccb7 of unit 1. Role unknown.
- `_idbg_delfssw` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x7f0`, 156 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x1cd02 of unit 1. Role unknown.
- `idbg_findfssw_vfs` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x88c`, 200 bytes, external).
  Returns `idbgfssw_t *`; Parameters `vfs_t *vfsp, bhv_desc_t * *bdp`. Record 0x1cd6a of unit 1. Role unknown.
- `idbg_findfssw_vnbhv` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x954`, 148 bytes, external).
  Returns `idbgfssw_t *`; Parameters `bhv_desc_t *bdp`. Record 0x1cded of unit 1. Role unknown.
- `_idbg_addfunc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x9e8`, 1584 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a subroutine type)`. Record 0x1ce5d of unit 1. Role unknown.
- `idbg_findfunc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1018`, 780 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a pointer to a subroutine type)`. Record 0x1cf0e of unit 1. Role unknown.
- `_idbg_delfunc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1324`, 432 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type)`. Record 0x1cfb2 of unit 1. Role unknown.
- `_idbg_copytab` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x14d4`, 792 bytes, external).
  Returns `int`; Parameters `caddr_t *uaddr, int len, rval_t *rvp`. Record 0x1d03d of unit 1. Role unknown.
- `_idbg_tablesize` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17ec`, 128 bytes, external).
  No return type in the record; Parameters `rval_t *rvp`. Record 0x1d12c of unit 1. Role unknown.
- `_idbg_dofunc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x186c`, 2376 bytes, external).
  Returns `int`; Parameters `struct idbgcomm *s, rval_t *rvp`. Record 0x1d17b of unit 1. Role unknown.
- `_qprintf` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x21b4`, 788 bytes, external).
  No return type in the record; Parameters `char *fmt, va_list ap`. Record 0x1d365 of unit 1. Role unknown.
- `_idbg_error` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x24c8`, 104 bytes, external).
  Returns `int`; no parameter records. Record 0x1d3f8 of unit 1. Role unknown.
- `_idbg_switch` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2530`, 488 bytes, external).
  No return type in the record; Parameters `int snum, int onoff`. Record 0x1d42e of unit 1. Role unknown.
- `idbg_string` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2718`, 92 bytes, external).
  No return type in the record; Parameters `int addr`. Record 0x1d4a3 of unit 1. Role unknown.
- `_printflags` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2774`, 484 bytes, external).
  No return type in the record; Parameters `uint64_t flags, char * *strings, char *name`. Record 0x1d4ee of unit 1. Role unknown.
- `idbg_hd` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2958`, 464 bytes, external).
  No return type in the record; Parameters `unsigned int *addr, __psint_t len`. Record 0x1d594 of unit 1. Role unknown.
- `ktrace_nentries` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2b28`, 116 bytes, external).
  Returns `int`; Parameters `ktrace_t *ktp`. Record 0x1d614 of unit 1. Role unknown.
- `ktrace_first` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2b9c`, 236 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x1d66b of unit 1. Role unknown.
- `ktrace_next` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2c88`, 192 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x1d718 of unit 1. Role unknown.
- `ktrace_skip` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2d48`, 348 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, int count, ktrace_snap_t *ktsp`. Record 0x1d7ab of unit 1. Role unknown.
- `printtimers` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2ea4`, 820 bytes, external).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x1d888 of unit 1. Role unknown.
- `idbg_kthreadname` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x31d8`, 188 bytes, external).
  Returns `char *`; Parameters `kthread_t *kt`. Record 0x1d902 of unit 1. Role unknown.
- `idbg_polldat` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x3294`, 264 bytes, external).
  No return type in the record; Parameters `struct polldat *p`. Record 0x1d96e of unit 1. Role unknown.
- `idbg_pollhead` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x339c`, 328 bytes, external).
  No return type in the record; Parameters `struct pollhead *p`. Record 0x1d9b8 of unit 1. Role unknown.
- `idbg_proxy` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x3750`, 1080 bytes, external).
  No return type in the record; Parameters `proc_proxy_t *prxy`. Record 0x1daa7 of unit 1. Role unknown.
- `pcred` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x3b88`, 636 bytes, external).
  No return type in the record; Parameters `cred_t *crp`. Record 0x1daf2 of unit 1. Role unknown.
- `idbg_doto_uthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x3e04`, 572 bytes, external).
  Returns `int`; Parameters `__psint_t p1, pfunc (pointer to a subroutine type), int all, int must`. Record 0x1db49 of unit 1. Role unknown.
- `idbg_do_uthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x4040`, 3360 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x1dc12 of unit 1. Role unknown.
- `idbg_uthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x4d60`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1dcc6 of unit 1. Role unknown.
- `idbg_proc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x4dc0`, 2692 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1dd11 of unit 1. Role unknown.
- `sleepon` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x584c`, 1424 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x1ddf1 of unit 1. Role unknown.
- `dumpssleep` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x5ddc`, 128 bytes, static).
  No return type in the record; Parameters `ssleep_t *st`. Record 0x1defe of unit 1. Role unknown.
- `idbg_pchain` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x5e5c`, 616 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1df46 of unit 1. Role unknown.
- `issthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x60c4`, 188 bytes, static).
  Returns `int`; Parameters `sthread_t *x`. Record 0x1dfb9 of unit 1. Role unknown.
- `isxthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x6180`, 188 bytes, static).
  Returns `int`; Parameters `xthread_t *x`. Record 0x1e01a of unit 1. Role unknown.
- `kstack` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x623c`, 576 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x1e07b of unit 1. Role unknown.
- `idbg_slist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x647c`, 548 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e101 of unit 1. Role unknown.
- `idbg_xlist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x66a0`, 548 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e16f of unit 1. Role unknown.
- `idbg_klist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x68c4`, 244 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e1dd of unit 1. Role unknown.
- `kid_is_uthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x69b8`, 172 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1e23b of unit 1. Role unknown.
- `find_kthreadid` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x6a64`, 880 bytes, static).
  Returns `int`; Parameters `uint64_t id, int verbose, __psunsigned_t *result`. Record 0x1e2eb of unit 1. Role unknown.
- `idbg_kthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x6dd4`, 2340 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e3d1 of unit 1. Role unknown.
- `idbg_sthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x76f8`, 588 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e483 of unit 1. Role unknown.
- `idbg_xthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x7944`, 848 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e4f3 of unit 1. Role unknown.
- `print_proc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x7c94`, 96 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1e563 of unit 1. Role unknown.
- `idbg_plist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x7cf4`, 360 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e5db of unit 1. Role unknown.
- `print_proc_fdt` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x7e5c`, 360 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1e636 of unit 1. Role unknown.
- `idbg_plfdt` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x7fc4`, 336 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e6b2 of unit 1. Role unknown.
- `showptree` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x8114`, 332 bytes, static).
  No return type in the record; Parameters `proc_t *parent, int level`. Record 0x1e71e of unit 1. Role unknown.
- `idbg_ptree` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x8260`, 308 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x1e7bc of unit 1. Role unknown.
- `idbg_pgrp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x8394`, 456 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1e818 of unit 1. Role unknown.
- `gdbg_proc_print` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x855c`, 356 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1e890 of unit 1. Role unknown.
- `gdbg_owner_print` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x86c0`, 212 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1e95d of unit 1. Role unknown.
- `idbg_pagg` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x8794`, 2588 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x1ea17 of unit 1. Role unknown.
- `showpagtree` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x91b0`, 360 bytes, static).
  No return type in the record; Parameters `proc_t *root, proc_t *parent, int level`. Record 0x1eadf of unit 1. Role unknown.
- `idbg_pagtree` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x9318`, 564 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x1eb95 of unit 1. Role unknown.
- `idbg_paglist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x954c`, 432 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x1ec38 of unit 1. Role unknown.
- `proc_pagfind` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x96fc`, 120 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1ecbd of unit 1. Role unknown.
- `idbg_pagfind` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x9774`, 308 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x1ed4c of unit 1. Role unknown.
- `shortuthread` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x98a8`, 484 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x1edc2 of unit 1. Role unknown.
- `shortproc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x9a8c`, 760 bytes, external).
  No return type in the record; Parameters `proc_t *pp, int ohboy`. Record 0x1ee0c of unit 1. Role unknown.
- `tinyproc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x9d84`, 172 bytes, static).
  No return type in the record; Parameters `proc_t *p`. Record 0x1ee8e of unit 1. Role unknown.
- `shortvpag` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x9e30`, 340 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag`. Record 0x1eed3 of unit 1. Role unknown.
- `prksigset` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x9f84`, 468 bytes, static).
  No return type in the record; Parameters `char *str, k_sigset_t *ks`. Record 0x1ef1c of unit 1. Role unknown.
- `idbg_do_signal` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xa158`, 1576 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x1ef9b of unit 1. Role unknown.
- `idbg_signal` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xa780`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1f022 of unit 1. Role unknown.
- `idbg_ptrace` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xa7e0`, 444 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1f06c of unit 1. Role unknown.
- `idbg_prnode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xa99c`, 768 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1f0e0 of unit 1. Role unknown.
- `idbg_prnodefree` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xac9c`, 1256 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x1f16a of unit 1. Role unknown.
- `idbg_watch` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xb184`, 964 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1f1e2 of unit 1. Role unknown.
- `print_sleepers` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xb548`, 808 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1f270 of unit 1. Role unknown.
- `idbg_slpproc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xb870`, 128 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1f370 of unit 1. Role unknown.
- `idbg_priscan` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xb980`, 232 bytes, static).
  No return type in the record; Parameters `prilist_t *mp`. Record 0x1f3fe of unit 1. Role unknown.
- `idbg_printputbuf` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xba68`, 224 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1f448 of unit 1. Role unknown.
- `dumpminfostats` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xbb48`, 824 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x1f496 of unit 1. Role unknown.
- `idbg_minfo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xc3dc`, 232 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x1f67c of unit 1. Role unknown.
- `dumpsysinfostats` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xc4c4`, 1096 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x1f6c4 of unit 1. Role unknown.
- `idbg_sysinfo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xc90c`, 88 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x1f730 of unit 1. Role unknown.
- `idbg_gbuf` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xc964`, 2772 bytes, static).
  No return type in the record; Parameters `buf_t *bp, int idx, buf_t *bufhead`. Record 0x1f77a of unit 1. Role unknown.
- `dumppdabstats` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xd838`, 1008 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x1f85d of unit 1. Role unknown.
- `idbg_fbuf` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xdc28`, 1312 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1f8a8 of unit 1. Role unknown.
- `idbg_vbuf` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xe148`, 536 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1f987 of unit 1. Role unknown.
- `idbg_findbuf` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xe360`, 416 bytes, external).
  No return type in the record; Parameters `int blkno`. Record 0x1fa4f of unit 1. Role unknown.
- `idbg_dport` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xe500`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1fadc of unit 1. Role unknown.
- `idbg_dportrset` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xe540`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1fb25 of unit 1. Role unknown.
- `idbg_pty` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xe580`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x1fb72 of unit 1. Role unknown.
- `idbg_do_shaddr` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xe5c0`, 1816 bytes, external).
  No return type in the record; Parameters `shaddr_t *sa`. Record 0x1fbb8 of unit 1. Role unknown.
- `idbg_do_ut_shaddr` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xecd8`, 176 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x1fc29 of unit 1. Role unknown.
- `idbg_shaddr` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xed88`, 168 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1fc8c of unit 1. Role unknown.
- `idbg_vnode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xee30`, 1684 bytes, external).
  Returns `vnode_t *`; Parameters `__psint_t p1`. Record 0x1fcd6 of unit 1. Role unknown.
- `vnops_to_vfstype` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xf4c4`, 220 bytes, static).
  Returns `int`; Parameters `vnodeops_t *vnops`. Record 0x1fde2 of unit 1. Role unknown.
- `idbg_bhvh` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xf5a0`, 352 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1fe4d of unit 1. Role unknown.
- `idbg_bhv` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xf700`, 300 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1fea9 of unit 1. Role unknown.
- `idbg_vnbhv` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0xf82c`, 2016 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1ff4b of unit 1. Role unknown.
- `idbg_extents` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1000c`, 1024 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2007a of unit 1. Role unknown.
- `idbg_prfifo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1040c`, 2180 bytes, external).
  No return type in the record; Parameters `struct fifonode *fnp, int header`. Record 0x20169 of unit 1. Role unknown.
- `idbg_prinode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x10c90`, 1392 bytes, external).
  No return type in the record; Parameters `struct inode *ip, vtype_t vtype, int header`. Record 0x201cc of unit 1. Role unknown.
- `idbg_prnamenode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x11200`, 720 bytes, external).
  No return type in the record; Parameters `struct namenode *nmp, int header`. Record 0x20245 of unit 1. Role unknown.
- `idbg_prlsnode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x114d0`, 2028 bytes, external).
  No return type in the record; Parameters `struct lsnode *lsp`. Record 0x202ab of unit 1. Role unknown.
- `idbg_prcsnode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x11cbc`, 1996 bytes, external).
  No return type in the record; Parameters `struct csnode *csp`. Record 0x2030d of unit 1. Role unknown.
- `prvn` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x12488`, 616 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x2036f of unit 1. Role unknown.
- `__prvn` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x126f0`, 72 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x203dc of unit 1. Role unknown.
- `prvfsvn` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x12738`, 1208 bytes, static).
  No return type in the record; Parameters `struct vfs *vfsp, int all`. Record 0x20435 of unit 1. Role unknown.
- `idbg_fifo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x12bf0`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2052a of unit 1. Role unknown.
- `idbg_fifolist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x12c50`, 516 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20582 of unit 1. Role unknown.
- `idbg_namelist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x12e54`, 516 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x205f4 of unit 1. Role unknown.
- `idbg_vlist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x13058`, 1908 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20666 of unit 1. Role unknown.
- `dumppdaistats` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x137cc`, 744 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x2073e of unit 1. Role unknown.
- `do_efs` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x13ab4`, 1944 bytes, static).
  No return type in the record; Parameters `struct efs *fs, int summary`. Record 0x20789 of unit 1. Role unknown.
- `idbg_efs` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1424c`, 68 bytes, external).
  No return type in the record; Parameters `struct efs *fs`. Record 0x2086c of unit 1. Role unknown.
- `idbg_quota` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x14290`, 1216 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x208b3 of unit 1. Role unknown.
- `idbg_dqb` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x14750`, 1364 bytes, external).
  No return type in the record; Parameters `struct dquot *dqp`. Record 0x2099f of unit 1. Role unknown.
- `idbg_vfs` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x14ca4`, 392 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x20a5b of unit 1. Role unknown.
- `prvfs` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x14e2c`, 1764 bytes, external).
  No return type in the record; Parameters `struct vfs *vfsp, int summary`. Record 0x20ac7 of unit 1. Role unknown.
- `idbg_vnmap` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15510`, 400 bytes, external).
  No return type in the record; Parameters `__psunsigned_t addr`. Record 0x20bbf of unit 1. Role unknown.
- `print_fmode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x156a0`, 240 bytes, static).
  No return type in the record; Parameters `int fmode`. Record 0x20c6b of unit 1. Role unknown.
- `idbg_bmapval` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15790`, 360 bytes, external).
  No return type in the record; Parameters `struct bmapval *bmapp`. Record 0x20ce2 of unit 1. Role unknown.
- `idbg_uio` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15930`, 756 bytes, external).
  No return type in the record; Parameters `uio_t *uiop`. Record 0x20d8d of unit 1. Role unknown.
- `idbg_open` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15c24`, 144 bytes, external).
  No return type in the record; Parameters `struct opena *uap`. Record 0x20e12 of unit 1. Role unknown.
- `idbg_creat` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15cb4`, 104 bytes, external).
  No return type in the record; Parameters `struct creata *uap`. Record 0x20e5b of unit 1. Role unknown.
- `idbg_mkdir` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15d1c`, 104 bytes, external).
  No return type in the record; Parameters `struct mkdira *uap`. Record 0x20ea5 of unit 1. Role unknown.
- `idbg_link` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15d84`, 104 bytes, external).
  No return type in the record; Parameters `struct linka *uap`. Record 0x20eef of unit 1. Role unknown.
- `idbg_rename` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15dec`, 104 bytes, external).
  No return type in the record; Parameters `struct renamea *uap`. Record 0x20f38 of unit 1. Role unknown.
- `idbg_symlink` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15e54`, 104 bytes, external).
  No return type in the record; Parameters `struct symlinka *uap`. Record 0x20f83 of unit 1. Role unknown.
- `idbg_unlink` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15ebc`, 96 bytes, external).
  No return type in the record; Parameters `struct unlinka *uap`. Record 0x20fcf of unit 1. Role unknown.
- `idbg_rmdir` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15f1c`, 96 bytes, external).
  No return type in the record; Parameters `struct rmdira *uap`. Record 0x2101a of unit 1. Role unknown.
- `idbg_xstat` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15f7c`, 112 bytes, external).
  No return type in the record; Parameters `struct xstatarg *uap`. Record 0x21064 of unit 1. Role unknown.
- `idbg_dumpsym` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x15fec`, 368 bytes, external).
  No return type in the record; Parameters `char * *addr`. Record 0x210ae of unit 1. Role unknown.
- `idbg_callo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1615c`, 640 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2111f of unit 1. Role unknown.
- `dump_callo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x163dc`, 472 bytes, static).
  No return type in the record; Parameters `struct callout *p1`. Record 0x21179 of unit 1. Role unknown.
- `idbg_calloinfo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x165b4`, 476 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x211d7 of unit 1. Role unknown.
- `dump_callout_info` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x16790`, 620 bytes, static).
  No return type in the record; Parameters `callout_info_t *cip`. Record 0x21242 of unit 1. Role unknown.
- `idbg_ptimer` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x169fc`, 532 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x212ba of unit 1. Role unknown.
- `idbg_file` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x16c10`, 708 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x21360 of unit 1. Role unknown.
- `print_procfdt` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x16ed4`, 400 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x21409 of unit 1. Role unknown.
- `idbg_vfile` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17064`, 300 bytes, external).
  No return type in the record; Parameters `__psint_t ptr`. Record 0x214d3 of unit 1. Role unknown.
- `ltype_to_str` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17190`, 464 bytes, external).
  Returns `char *`; Parameters `int ltype`. Record 0x21530 of unit 1. Role unknown.
- `idbg_filock` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17360`, 232 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x215cc of unit 1. Role unknown.
- `idbg_sleeplcks` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17448`, 344 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x2163e of unit 1. Role unknown.
- `idbg_flock` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x175a0`, 188 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x216b3 of unit 1. Role unknown.
- `printfp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1765c`, 1124 bytes, static).
  Returns `int`; Parameters `vfile_t *fp, int slot, int flag`. Record 0x2171d of unit 1. Role unknown.
- `idbg_map` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17ac0`, 244 bytes, external).
  No return type in the record; Parameters `struct map *p`. Record 0x217e3 of unit 1. Role unknown.
- `idbg_dobitmap` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17bb4`, 560 bytes, external).
  No return type in the record; Parameters `struct bitmap *bp, char *name`. Record 0x2183a of unit 1. Role unknown.
- `idbg_chbtmp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17de4`, 332 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x218ad of unit 1. Role unknown.
- `idbg_bitmap` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x17f30`, 928 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2195c of unit 1. Role unknown.
- `idbg_svdump` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x182d0`, 1612 bytes, external).
  No return type in the record; Parameters `sv_t *svp`. Record 0x219e0 of unit 1. Role unknown.
- `idbg_pwait` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1891c`, 472 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21a67 of unit 1. Role unknown.
- `idbg_page_counter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x18af4`, 20 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x21ac6 of unit 1. Role unknown.
- `idbg_shdump` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x18b10`, 1060 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x21b4f of unit 1. Role unknown.
- `idbg_semchain` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x18f34`, 816 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x21bd3 of unit 1. Role unknown.
- `idbg_nschain` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x19264`, 1132 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x21c42 of unit 1. Role unknown.
- `scanchain` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x196d0`, 512 bytes, static).
  No return type in the record; Parameters `int x`. Record 0x21cea of unit 1. Role unknown.
- `idbgplog` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x198d0`, 792 bytes, external).
  No return type in the record; Parameters `semahist_t *hp`. Record 0x21d57 of unit 1. Role unknown.
- `idbg_semctl` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x19be8`, 576 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x21dcb of unit 1. Role unknown.
- `semctl_apply` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x19e28`, 836 bytes, static).
  Returns `int`; Parameters `char *str, func (pointer to a subroutine type), int ac, char * *av`. Record 0x21e77 of unit 1. Role unknown.
- `semctl_describe` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1a16c`, 232 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x21fe4 of unit 1. Role unknown.
- `semctl_ctlmeter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1a254`, 3256 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x22057 of unit 1. Role unknown.
- `semctl_ctlmeter_aux` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1af0c`, 2532 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x221ba of unit 1. Role unknown.
- `semctl_defhist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1b8f0`, 828 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x223bf of unit 1. Role unknown.
- `semctl_defkhist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1bc2c`, 760 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2241e of unit 1. Role unknown.
- `semctl_defkhlen` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1bf24`, 388 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2247e of unit 1. Role unknown.
- `semctl_defmeter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1c0a8`, 760 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x22504 of unit 1. Role unknown.
- `semctl_help` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1c3a0`, 224 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x22564 of unit 1. Role unknown.
- `semctl_histmax` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1c480`, 360 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x225d5 of unit 1. Role unknown.
- `semctl_sema` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1c5e8`, 188 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x2265d of unit 1. Role unknown.
- `semctl_semlog` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1c6a4`, 3200 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x226b9 of unit 1. Role unknown.
- `semctl_what` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1d324`, 188 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x22871 of unit 1. Role unknown.
- `printid` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1d3e0`, 552 bytes, static).
  No return type in the record; Parameters `kthread_t *kt, uint64_t id, char *str`. Record 0x228cd of unit 1. Role unknown.
- `idbg_semdump` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1d608`, 1620 bytes, external).
  No return type in the record; Parameters `sema_t *sp`. Record 0x22961 of unit 1. Role unknown.
- `idbg_mutexdump` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1dc5c`, 1316 bytes, external).
  No return type in the record; Parameters `mutex_t *mp`. Record 0x229e8 of unit 1. Role unknown.
- `idbg_dumphisto` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1e180`, 1656 bytes, static).
  No return type in the record; Parameters `k_histelem_t *khp, char *str, int timed`. Record 0x22a71 of unit 1. Role unknown.
- `idbg_semalist` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1e7f8`, 1436 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x22b63 of unit 1. Role unknown.
- `idbg_chklock` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1ed94`, 1608 bytes, external).
  No return type in the record; Parameters `void *l`. Record 0x22c24 of unit 1. Role unknown.
- `idbg_avlnode` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1f3dc`, 344 bytes, external).
  No return type in the record; Parameters `avlnode_t *avl`. Record 0x22c83 of unit 1. Role unknown.
- `idbg_lockmeter` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1f534`, 1588 bytes, external).
  No return type in the record; Parameters `k_splockmeter_t *spm`. Record 0x22ccf of unit 1. Role unknown.
- `islock` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1fb68`, 360 bytes, static).
  Returns `char *`; Parameters `void *lck`. Record 0x22d1d of unit 1. Role unknown.
- `idbg_hotlocks` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1fcd0`, 72 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x22d8f of unit 1. Role unknown.
- `dumphotlock` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x1fd18`, 2876 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x22ded of unit 1. Role unknown.
- `idbg_coldlocks` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x20854`, 528 bytes, external).
  No return type in the record; Parameters `int n`. Record 0x22ee6 of unit 1. Role unknown.
- `idbg_mrlock` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x20a64`, 3844 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x22fa7 of unit 1. Role unknown.
- `idbg_mslock` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x21968`, 408 bytes, external).
  No return type in the record; Parameters `mslock_t *msp`. Record 0x2310c of unit 1. Role unknown.
- `idbg_mrlockfull` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x21b00`, 4956 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x23157 of unit 1. Role unknown.
- `idbg_pda` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x22e5c`, 148 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23249 of unit 1. Role unknown.
- `do_pdas` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x22ef0`, 480 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type), int which`. Record 0x2328f of unit 1. Role unknown.
- `dumppda` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x230d0`, 3920 bytes, external).
  No return type in the record; Parameters `pda_t *pd`. Record 0x232ff of unit 1. Role unknown.
- `idbg_donodepda` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x24020`, 504 bytes, external).
  No return type in the record; Parameters `nodepda_t *npda`. Record 0x23384 of unit 1. Role unknown.
- `idbg_nodepda` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x24218`, 332 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x233d3 of unit 1. Role unknown.
- `idbg_uarea` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x24364`, 540 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2342f of unit 1. Role unknown.
- `idbg_upage` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x24580`, 624 bytes, external).
  No return type in the record; Parameters `proc_t *pp`. Record 0x2348a of unit 1. Role unknown.
- `idbg_fdt` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x247f0`, 464 bytes, external).
  No return type in the record; Parameters `fdt_t *fdt`. Record 0x234d3 of unit 1. Role unknown.
- `idbg_runq` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x249c0`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t howmany`. Record 0x2354c of unit 1. Role unknown.
- `dumpRunq` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x24a00`, 1900 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x23599 of unit 1. Role unknown.
- `idbg_dopglst` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2516c`, 288 bytes, static).
  No return type in the record; Parameters `pglst_t *pglptr`. Record 0x2361b of unit 1. Role unknown.
- `idbg_swap` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2528c`, 592 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x2367e of unit 1. Role unknown.
- `idbg_pglst` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x254dc`, 540 bytes, external).
  No return type in the record; Parameters `__psint_t i`. Record 0x236ea of unit 1. Role unknown.
- `idbg_kaddr` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x256f8`, 324 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x2374f of unit 1. Role unknown.
- `idbg_do_eframe` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2583c`, 536 bytes, external).
  No return type in the record; Parameters `eframe_t *ep`. Record 0x237c4 of unit 1. Role unknown.
- `idbg_do_ut_eframe` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x25a54`, 160 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x23826 of unit 1. Role unknown.
- `idbg_eframe` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x25af4`, 204 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x23889 of unit 1. Role unknown.
- `idbg_ubtrace` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x25bc0`, 1912 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x238d2 of unit 1. Role unknown.
- `convsp_start` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x26338`, 96 bytes, static).
  Returns `int`; Parameters `struct uthread_s *ut`. Record 0x239e0 of unit 1. Role unknown.
- `convsp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x263bc`, 128 bytes, static).
  Returns `unsigned int *`; Parameters `unsigned int *sp, unsigned int *physsp`. Record 0x23a5e of unit 1. Role unknown.
- `idbg_get_stack_bytes` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2643c`, 304 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut, void *sp, void *dp, int n`. Record 0x23ad5 of unit 1. Role unknown.
- `fetch_kaddr` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2656c`, 472 bytes, external).
  Returns `__psunsigned_t`; Parameters `char *s`. Record 0x23b89 of unit 1. Role unknown.
- `lookup_kname` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x26744`, 416 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x23c5f of unit 1. Role unknown.
- `fetch_kname` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x268e4`, 140 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x23cf3 of unit 1. Role unknown.
- `_prsymoff` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x26970`, 468 bytes, external).
  No return type in the record; Parameters `void *addr, char *pre, char *post`. Record 0x23d8e of unit 1. Role unknown.
- `idbg_btrace` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x26b44`, 148 bytes, external).
  No return type in the record; Parameters `int maxfr`. Record 0x23e2d of unit 1. Role unknown.
- `btrace` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x26bd8`, 9332 bytes, static).
  No return type in the record; Parameters `unsigned int *pc, unsigned int *sp, unsigned int *lra, int maxfr, unsigned int *lbound, kthread_t *kt, int myself`. Record 0x23e7a of unit 1. Role unknown.
- `txtsrch` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2904c`, 1152 bytes, static).
  Returns `inst_t *`; Parameters `inst_t *pc, unsigned int inst1mask, unsigned int inst2mask, int *which, enum sign im1sign, enum sign im2sign, int *im_ptr, inst_t *llimit, struct spec_funcs * *specf, enum direction dir, unsigned int check1mask, unsigned int check2mask`. Record 0x24252 of unit 1. Role unknown.
- `is_jal` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x29654`, 168 bytes, static).
  Returns `int`; Parameters `inst_t inst`. Record 0x244d8 of unit 1. Role unknown.
- `branch_target` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x296fc`, 240 bytes, static).
  Returns `inst_t *`; Parameters `inst_t inst, inst_t *pc`. Record 0x24538 of unit 1. Role unknown.
- `fetch_text` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x297ec`, 308 bytes, static).
  Returns `inst_t`; Parameters `inst_t *pc`. Record 0x245b2 of unit 1. Role unknown.
- `ilastframe` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x29920`, 76 bytes, static).
  Returns `int`; Parameters `k_machreg_t *sp`. Record 0x2462a of unit 1. Role unknown.
- `idbg_symtab` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2996c`, 88 bytes, external).
  No return type in the record; Parameters `struct dbstbl * *db, char * *name`. Record 0x2467a of unit 1. Role unknown.
- `idbg_symsize` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x299c4`, 56 bytes, external).
  No return type in the record; Parameters `int *size`. Record 0x246d9 of unit 1. Role unknown.
- `idbg_ksymrange` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x299fc`, 88 bytes, external).
  No return type in the record; Parameters `void * *_kftext, void * *_kend`. Record 0x24726 of unit 1. Role unknown.
- `idbg_scsi` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x29a54`, 892 bytes, external).
  No return type in the record; Parameters `struct scsi_request *req`. Record 0x2478e of unit 1. Role unknown.
- `idbg_foi` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x29dd0`, 1372 bytes, external).
  No return type in the record; Parameters `vertex_hdl_t lun_vhdl`. Record 0x247e9 of unit 1. Role unknown.
- `dkactive` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2a32c`, 164 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x248d6 of unit 1. Role unknown.
- `dkqueue` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2a3d0`, 564 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x24930 of unit 1. Role unknown.
- `idbg_dksoftc` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2a604`, 1164 bytes, external).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x24989 of unit 1. Role unknown.
- `intvec_tinfo` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2aa90`, 640 bytes, external).
  No return type in the record; Parameters `thd_int_t *tip`. Record 0x249d5 of unit 1. Role unknown.
- `idbg_intvec` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2ad10`, 580 bytes, external).
  No return type in the record; Parameters `__psint_t l`. Record 0x24a21 of unit 1. Role unknown.
- `idbg_call` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2af54`, 336 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x24ab3 of unit 1. Role unknown.
- `idbg_kill` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2b0a4`, 508 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x24afa of unit 1. Role unknown.
- `idbg_qbuf` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2b2a0`, 772 bytes, external).
  No return type in the record; Parameters `__psint_t dev`. Record 0x24b68 of unit 1. Role unknown.
- `idbg_dozone` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2b7e4`, 2084 bytes, static).
  No return type in the record; Parameters `zone_t *zp, int all`. Record 0x24c82 of unit 1. Role unknown.
- `idbg_zone` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2c008`, 916 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x24d36 of unit 1. Role unknown.
- `idbg_tracemasks` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2c39c`, 364 bytes, external).
  No return type in the record; Parameters `struct pr_tracemasks *t`. Record 0x24dfa of unit 1. Role unknown.
- `idbg_tstamp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2c508`, 364 bytes, external).
  No return type in the record; Parameters `const tstamp_obj_t *objp`. Record 0x24e48 of unit 1. Role unknown.
- `utrace_dump` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2c674`, 2740 bytes, static).
  No return type in the record; Parameters `tstamp_event_entry_t *event, int cpu, tstamp_event_entry_t *limit, __psint_t sval`. Record 0x24e94 of unit 1. Role unknown.
- `_idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2d128`, 3872 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2, __psint_t sval`. Record 0x24fb4 of unit 1. Role unknown.
- `idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e048`, 76 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x252a0 of unit 1. Role unknown.
- `idbg_utrace_find` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e094`, 136 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x25301 of unit 1. Role unknown.
- `idbg_utrace_mask` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e11c`, 200 bytes, external).
  No return type in the record; Parameters `uint64_t arg1`. Record 0x25367 of unit 1. Role unknown.
- `idbg_do_softfp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e1e4`, 204 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x253b8 of unit 1. Role unknown.
- `idbg_softfp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e2b0`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x25404 of unit 1. Role unknown.
- `print_maclabel` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e310`, 1004 bytes, external).
  No return type in the record; Parameters `char *cp, mac_label *lp`. Record 0x2544e of unit 1. Role unknown.
- `idbg_stop` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e6fc`, 64 bytes, external).
  No return type in the record; Parameters `void *stoplist`. Record 0x254d8 of unit 1. Role unknown.
- `idbg_ioerror` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2e73c`, 1552 bytes, static).
  No return type in the record; Parameters `__psunsigned_t arg`. Record 0x25526 of unit 1. Role unknown.
- `re_comp` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2ed4c`, 2328 bytes, external).
  Returns `char *`; Parameters `const char *sp`. Record 0x25594 of unit 1. Role unknown.
- `re_exec` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2f664`, 592 bytes, external).
  Returns `int`; Parameters `const char *p1`. Record 0x256c2 of unit 1. Role unknown.
- `advance` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x2f8b4`, 1936 bytes, static).
  Returns `int`; Parameters `const char *lp, char *ep`. Record 0x25749 of unit 1. Role unknown.
- `backref` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x30044`, 272 bytes, static).
  Returns `int`; Parameters `int i, const char *lp`. Record 0x25808 of unit 1. Role unknown.
- `cclass` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x30154`, 204 bytes, static).
  Returns `int`; Parameters `char *set, char c, int af`. Record 0x2587a of unit 1. Role unknown.
- `glob2regex` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x30220`, 412 bytes, static).
  No return type in the record; Parameters `char *out, const char *in`. Record 0x258ff of unit 1. Role unknown.
- `strtoargs` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x303bc`, 700 bytes, static).
  No return type in the record; Parameters `char *buf, int *ac, char * *ap`. Record 0x2596d of unit 1. Role unknown.
- `_padstr` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x30678`, 476 bytes, external).
  Returns `char *`; Parameters `char *buf, int len`. Record 0x25a15 of unit 1. Role unknown.
- `_padstrn` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x30854`, 560 bytes, external).
  Returns `char *`; Parameters `char *buf, int buflen, int padlen`. Record 0x25ab2 of unit 1. Role unknown.
- `padnum` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x30a84`, 880 bytes, external).
  Returns `char *`; Parameters `__psint_t num, int base, int len`. Record 0x25b6b of unit 1. Role unknown.
- `padnum64` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x30df4`, 880 bytes, external).
  Returns `char *`; Parameters `__int64_t num, int base, int len`. Record 0x25c55 of unit 1. Role unknown.
- `pad_utrace_qual` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x31164`, 288 bytes, static).
  Returns `char *`; Parameters `__uint64_t qual`. Record 0x25d41 of unit 1. Role unknown.
- `_mcount` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x31284`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x25dbf of unit 1. Role unknown.
- `dumptraplog_entry` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x31290`, 2248 bytes, external).
  No return type in the record; Parameters `__uint64_t *entry, __uint32_t pda_traplog_mask, __uint32_t pda_traplog_strace_size`. Record 0x25df1 of unit 1. Role unknown.
- `dumptraplog` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x31b58`, 316 bytes, external).
  No return type in the record; Parameters `char *pdapage, __uint32_t pda_traplog_mask, __uint32_t pda_traplog_dumpcnt, __uint32_t pda_traplog_strace_size, __uint32_t pda_traplog_entry_size`. Record 0x25ee6 of unit 1. Role unknown.
- `idbg_traplog` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x31c94`, 1484 bytes, external).
  No return type in the record; Parameters `__psint_t id`. Record 0x25ff9 of unit 1. Role unknown.
- `idbg_print_cpumon` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x32260`, 576 bytes, external).
  No return type in the record; Parameters `cpu_mon_t *cmp`. Record 0x26111 of unit 1. Role unknown.
- `idbg_cpumon` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x324a0`, 1036 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x26174 of unit 1. Role unknown.
- `start_event_counters` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x328e4`, 1636 bytes, external).
  No return type in the record; Parameters `int mode, short event0, short event1, short event2, short event3`. Record 0x26227 of unit 1. Role unknown.
- `stop_event_counters` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x33440`, 868 bytes, external).
  No return type in the record; Parameters `int counters`. Record 0x263fa of unit 1. Role unknown.
- `idbg_setpri` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x337f4`, 572 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x26485 of unit 1. Role unknown.
- `ec0_down` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x33a30`, 716 bytes, external).
  Returns `int`; no parameter records. Record 0x26543 of unit 1. Role unknown.
- `ec0_up` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x33cfc`, 516 bytes, external).
  Returns `int`; no parameter records. Record 0x265b9 of unit 1. Role unknown.
- `getrbase` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x33f00`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x26644 of unit 1. Role unknown.
- `idbg_etrace` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x33f0c`, 116 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x26677 of unit 1. Role unknown.
- `uthreadp_is_valid` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x34250`, 492 bytes, external).
  Returns `int`; Parameters `uthread_t *ut`. Record 0x2671c of unit 1. Role unknown.
- `procp_is_valid` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x3443c`, 508 bytes, external).
  Returns `int`; Parameters `proc_t *p`. Record 0x26774 of unit 1. Role unknown.
- `idbg_dstat` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x34638`, 364 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x267c8 of unit 1. Role unknown.
- `idbg_dstats` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x347a4`, 1060 bytes, external).
  No return type in the record; Parameters `uint64_t flag`. Record 0x26843 of unit 1. Role unknown.
- `idbg_retarget` (`6.5.30/usr/cpu/sysgen/IP30boot/idbg.o` `0x34bc8`, 1532 bytes, external).
  No return type in the record; Parameters `void *p, void *a2, int argc, char * *argv`. Record 0x26927 of unit 1. Role unknown.

### `6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o`

- `melflkaddr` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0x0`, 780 bytes, static).
  No return type in the record; Parameters `ml_info_t *m, __psint_t addr`. Record 0x1844 of unit 1. Role unknown.
- `idbg_mlist` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0x30c`, 2796 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x1925 of unit 1. Role unknown.
- `idbg_msyms` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0xdf8`, 488 bytes, external).
  No return type in the record; Parameters `int id`. Record 0x19ee of unit 1. Role unknown.
- `idbg_mlkup` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0xfe0`, 360 bytes, external).
  No return type in the record; Parameters `char *name`. Record 0x1a7f of unit 1. Role unknown.
- `idbg_mlkaddr` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0x1148`, 156 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x1b00 of unit 1. Role unknown.
- `mflags` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0x11e4`, 636 bytes, static).
  No return type in the record; Parameters `int flags`. Record 0x1b5d of unit 1. Role unknown.
- `mtype` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0x1460`, 832 bytes, static).
  No return type in the record; Parameters `int type`. Record 0x1ba4 of unit 1. Role unknown.
- `idbg_mlinfo` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0x17a0`, 1316 bytes, external).
  No return type in the record; Parameters `ml_info_t *m`. Record 0x1be9 of unit 1. Role unknown.
- `mloadidbgunload` (`6.5.30/usr/cpu/sysgen/IP30boot/mloadidbg.o` `0x1f28`, 420 bytes, external).
  Returns `int`; no parameter records. Record 0x1c77 of unit 1. Role unknown.
### `6.5.30/usr/cpu/sysgen/IP32boot/idbg.o`

- `kqueue_first` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x0`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x1696b of unit 1. Role unknown.
- `kqueue_next` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1c`, 28 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x169a6 of unit 1. Role unknown.
- `kqueue_end` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x38`, 24 bytes, static).
  Returns `kqueue_t *`; Parameters `kqueue_t *queue`. Record 0x169e0 of unit 1. Role unknown.
- `VPAG_GETPRID` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x50`, 80 bytes, static).
  Returns `prid_t`; Parameters `vpagg_t *vpag`. Record 0x16a19 of unit 1. Role unknown.
- `VPAG_EXTRACT_ARSESS_INFO` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xa0`, 112 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag, arsess_t *arsess_info`. Record 0x16a7e of unit 1. Role unknown.
- `mutexmeter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x110`, 56 bytes, static).
  Returns `mmeter_t *`; Parameters `mutex_t *mp`. Record 0x16aeb of unit 1. Role unknown.
- `svmeter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x148`, 56 bytes, static).
  Returns `svmeter_t *`; Parameters `sv_t *svp`. Record 0x16b23 of unit 1. Role unknown.
- `smeter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x180`, 56 bytes, static).
  Returns `k_semameter_t *`; Parameters `sema_t *sp`. Record 0x16b59 of unit 1. Role unknown.
- `mrmeter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1b8`, 56 bytes, static).
  Returns `k_mrmeter_t *`; Parameters `mrlock_t *mrp`. Record 0x16b8d of unit 1. Role unknown.
- `_idbg_unload` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x54c`, 156 bytes, external).
  Returns `int`; no parameter records. Record 0x16c71 of unit 1. Role unknown.
- `_idbg_addfssw` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5e8`, 48 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x16c98 of unit 1. Role unknown.
- `_idbg_delfssw` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x618`, 136 bytes, external).
  No return type in the record; Parameters `idbgfssw_t *i`. Record 0x16ccf of unit 1. Role unknown.
- `idbg_findfssw_vfs` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6a0`, 164 bytes, external).
  Returns `idbgfssw_t *`; Parameters `vfs_t *vfsp, bhv_desc_t * *bdp`. Record 0x16d1b of unit 1. Role unknown.
- `idbg_findfssw_vnbhv` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x744`, 128 bytes, external).
  Returns `idbgfssw_t *`; Parameters `bhv_desc_t *bdp`. Record 0x16d7e of unit 1. Role unknown.
- `_idbg_addfunc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7c4`, 872 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a subroutine type)`. Record 0x16dd2 of unit 1. Role unknown.
- `idbg_findfunc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xb2c`, 452 bytes, external).
  Returns `int`; Parameters `char *name, func (pointer to a pointer to a subroutine type)`. Record 0x16e55 of unit 1. Role unknown.
- `_idbg_delfunc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xcf0`, 252 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type)`. Record 0x16ed1 of unit 1. Role unknown.
- `_idbg_copytab` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdec`, 644 bytes, external).
  Returns `int`; Parameters `caddr_t *uaddr, int len, rval_t *rvp`. Record 0x16f3c of unit 1. Role unknown.
- `_idbg_tablesize` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1070`, 76 bytes, external).
  No return type in the record; Parameters `rval_t *rvp`. Record 0x16ff7 of unit 1. Role unknown.
- `_idbg_dofunc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x10bc`, 1688 bytes, external).
  Returns `int`; Parameters `struct idbgcomm *s, rval_t *rvp`. Record 0x17032 of unit 1. Role unknown.
- `_qprintf` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1754`, 464 bytes, external).
  No return type in the record; Parameters `char *fmt, va_list ap`. Record 0x171b8 of unit 1. Role unknown.
- `_idbg_error` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1924`, 68 bytes, external).
  Returns `int`; no parameter records. Record 0x17227 of unit 1. Role unknown.
- `_idbg_switch` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1968`, 300 bytes, external).
  No return type in the record; Parameters `int snum, int onoff`. Record 0x1724d of unit 1. Role unknown.
- `idbg_string` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a94`, 56 bytes, external).
  No return type in the record; Parameters `int addr`. Record 0x172a6 of unit 1. Role unknown.
- `_printflags` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1acc`, 336 bytes, external).
  No return type in the record; Parameters `uint64_t flags, char * *strings, char *name`. Record 0x172dd of unit 1. Role unknown.
- `idbg_hd` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1c1c`, 296 bytes, external).
  No return type in the record; Parameters `unsigned int *addr, __psint_t len`. Record 0x1735f of unit 1. Role unknown.
- `ktrace_nentries` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1d44`, 116 bytes, external).
  Returns `int`; Parameters `ktrace_t *ktp`. Record 0x173bf of unit 1. Role unknown.
- `ktrace_first` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1db8`, 220 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x173fe of unit 1. Role unknown.
- `ktrace_next` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1e94`, 176 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, ktrace_snap_t *ktsp`. Record 0x17483 of unit 1. Role unknown.
- `ktrace_skip` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f44`, 332 bytes, external).
  Returns `ktrace_entry_t *`; Parameters `ktrace_t *ktp, int count, ktrace_snap_t *ktsp`. Record 0x174f2 of unit 1. Role unknown.
- `printtimers` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2090`, 472 bytes, external).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x1759f of unit 1. Role unknown.
- `idbg_kthreadname` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2268`, 168 bytes, external).
  Returns `char *`; Parameters `kthread_t *kt`. Record 0x175f9 of unit 1. Role unknown.
- `idbg_polldat` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2310`, 136 bytes, external).
  No return type in the record; Parameters `struct polldat *p`. Record 0x17649 of unit 1. Role unknown.
- `idbg_pollhead` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2398`, 224 bytes, external).
  No return type in the record; Parameters `struct pollhead *p`. Record 0x1767f of unit 1. Role unknown.
- `idbg_proxy` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x260c`, 612 bytes, external).
  No return type in the record; Parameters `proc_proxy_t *prxy`. Record 0x17732 of unit 1. Role unknown.
- `pcred` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2870`, 400 bytes, external).
  No return type in the record; Parameters `cred_t *crp`. Record 0x17769 of unit 1. Role unknown.
- `idbg_doto_uthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2a00`, 440 bytes, external).
  Returns `int`; Parameters `__psint_t p1, pfunc (pointer to a subroutine type), int all, int must`. Record 0x177a8 of unit 1. Role unknown.
- `idbg_do_uthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2bb8`, 1936 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x17841 of unit 1. Role unknown.
- `idbg_uthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x3348`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x178c0 of unit 1. Role unknown.
- `idbg_proc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x3388`, 1620 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x178f7 of unit 1. Role unknown.
- `sleepon` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x39e4`, 864 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x1799b of unit 1. Role unknown.
- `dumpssleep` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x3d44`, 92 bytes, static).
  No return type in the record; Parameters `ssleep_t *st`. Record 0x17a60 of unit 1. Role unknown.
- `idbg_pchain` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x3da0`, 420 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x17a94 of unit 1. Role unknown.
- `issthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x3f44`, 136 bytes, static).
  Returns `int`; Parameters `sthread_t *x`. Record 0x17aeb of unit 1. Role unknown.
- `isxthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x3fcc`, 136 bytes, static).
  Returns `int`; Parameters `xthread_t *x`. Record 0x17b30 of unit 1. Role unknown.
- `kstack` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x4054`, 360 bytes, static).
  No return type in the record; Parameters `kthread_t *kt`. Record 0x17b75 of unit 1. Role unknown.
- `idbg_slist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x41bc`, 356 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x17bdb of unit 1. Role unknown.
- `idbg_xlist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x4320`, 356 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x17c2d of unit 1. Role unknown.
- `idbg_klist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x4484`, 160 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x17c7f of unit 1. Role unknown.
- `kid_is_uthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x4524`, 172 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x17cc5 of unit 1. Role unknown.
- `find_kthreadid` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x45d0`, 564 bytes, static).
  Returns `int`; Parameters `uint64_t id, int verbose, __psunsigned_t *result`. Record 0x17d49 of unit 1. Role unknown.
- `idbg_kthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x4804`, 1408 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x17df9 of unit 1. Role unknown.
- `idbg_sthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x4d84`, 376 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x17e7e of unit 1. Role unknown.
- `idbg_xthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x4efc`, 444 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x17ed2 of unit 1. Role unknown.
- `print_proc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x50b8`, 80 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x17f26 of unit 1. Role unknown.
- `idbg_plist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5108`, 204 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x17f7e of unit 1. Role unknown.
- `print_proc_fdt` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x51d4`, 216 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x17fc1 of unit 1. Role unknown.
- `idbg_plfdt` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x52ac`, 200 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1801d of unit 1. Role unknown.
- `showptree` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5374`, 248 bytes, static).
  No return type in the record; Parameters `proc_t *parent, int level`. Record 0x18069 of unit 1. Role unknown.
- `idbg_ptree` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x546c`, 204 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x180e3 of unit 1. Role unknown.
- `idbg_pgrp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5538`, 280 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x18127 of unit 1. Role unknown.
- `gdbg_proc_print` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5650`, 268 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1817f of unit 1. Role unknown.
- `gdbg_owner_print` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x575c`, 196 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x18218 of unit 1. Role unknown.
- `idbg_pagg` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5820`, 1436 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x182a2 of unit 1. Role unknown.
- `showpagtree` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5dbc`, 276 bytes, static).
  No return type in the record; Parameters `proc_t *root, proc_t *parent, int level`. Record 0x18331 of unit 1. Role unknown.
- `idbg_pagtree` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x5ed0`, 332 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x183bf of unit 1. Role unknown.
- `idbg_paglist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x601c`, 292 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x18439 of unit 1. Role unknown.
- `proc_pagfind` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6140`, 104 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1849a of unit 1. Role unknown.
- `idbg_pagfind` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x61a8`, 208 bytes, external).
  No return type in the record; Parameters `__psint_t arg`. Record 0x18505 of unit 1. Role unknown.
- `shortuthread` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6278`, 252 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x1855f of unit 1. Role unknown.
- `shortproc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6374`, 484 bytes, external).
  No return type in the record; Parameters `proc_t *pp, int ohboy`. Record 0x18595 of unit 1. Role unknown.
- `tinyproc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6558`, 100 bytes, static).
  No return type in the record; Parameters `proc_t *p`. Record 0x185f3 of unit 1. Role unknown.
- `shortvpag` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x65bc`, 180 bytes, static).
  No return type in the record; Parameters `vpagg_t *vpag`. Record 0x18624 of unit 1. Role unknown.
- `prksigset` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6670`, 304 bytes, static).
  No return type in the record; Parameters `char *str, k_sigset_t *ks`. Record 0x18659 of unit 1. Role unknown.
- `idbg_do_signal` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x67a0`, 820 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x186b8 of unit 1. Role unknown.
- `idbg_signal` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6ad4`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1871b of unit 1. Role unknown.
- `idbg_ptrace` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6b14`, 292 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x18751 of unit 1. Role unknown.
- `idbg_prnode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6c38`, 472 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x187a9 of unit 1. Role unknown.
- `idbg_prnodefree` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x6e10`, 720 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x18813 of unit 1. Role unknown.
- `idbg_watch` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x70e0`, 624 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1886f of unit 1. Role unknown.
- `print_sleepers` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7350`, 576 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x188d9 of unit 1. Role unknown.
- `idbg_slpproc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7590`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1899c of unit 1. Role unknown.
- `idbg_priscan` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7638`, 164 bytes, static).
  No return type in the record; Parameters `prilist_t *mp`. Record 0x18a06 of unit 1. Role unknown.
- `idbg_printputbuf` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x76dc`, 116 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x18a3c of unit 1. Role unknown.
- `dumpminfostats` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7750`, 500 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x18a76 of unit 1. Role unknown.
- `idbg_minfo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7c38`, 148 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x18bfd of unit 1. Role unknown.
- `dumpsysinfostats` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7ccc`, 664 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x18c31 of unit 1. Role unknown.
- `idbg_sysinfo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7f64`, 56 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x18c81 of unit 1. Role unknown.
- `idbg_gbuf` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x7f9c`, 1872 bytes, static).
  No return type in the record; Parameters `buf_t *bp, int idx, buf_t *bufhead`. Record 0x18cb7 of unit 1. Role unknown.
- `dumppdabstats` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x88e0`, 684 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x18d62 of unit 1. Role unknown.
- `idbg_fbuf` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x8b8c`, 812 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x18d99 of unit 1. Role unknown.
- `idbg_vbuf` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x8eb8`, 500 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x18e3c of unit 1. Role unknown.
- `idbg_findbuf` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x90ac`, 248 bytes, external).
  No return type in the record; Parameters `int blkno`. Record 0x18ed9 of unit 1. Role unknown.
- `idbg_dport` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x91a4`, 48 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x18f46 of unit 1. Role unknown.
- `idbg_dportrset` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x91d4`, 48 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x18f7b of unit 1. Role unknown.
- `idbg_pty` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x9204`, 48 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x18fb4 of unit 1. Role unknown.
- `idbg_do_shaddr` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x9234`, 1080 bytes, external).
  No return type in the record; Parameters `shaddr_t *sa`. Record 0x18fe6 of unit 1. Role unknown.
- `idbg_do_ut_shaddr` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x966c`, 124 bytes, external).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x1903b of unit 1. Role unknown.
- `idbg_shaddr` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x96e8`, 120 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x19086 of unit 1. Role unknown.
- `idbg_vnode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x9760`, 1136 bytes, external).
  Returns `vnode_t *`; Parameters `__psint_t p1`. Record 0x190bc of unit 1. Role unknown.
- `vnops_to_vfstype` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x9bd0`, 164 bytes, static).
  Returns `int`; Parameters `vnodeops_t *vnops`. Record 0x19182 of unit 1. Role unknown.
- `idbg_bhvh` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x9c74`, 148 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x191d1 of unit 1. Role unknown.
- `idbg_bhv` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x9d08`, 212 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x19215 of unit 1. Role unknown.
- `idbg_vnbhv` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x9ddc`, 1108 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1928b of unit 1. Role unknown.
- `idbg_extents` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xa230`, 724 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x19368 of unit 1. Role unknown.
- `idbg_prfifo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xa504`, 1324 bytes, external).
  No return type in the record; Parameters `struct fifonode *fnp, int header`. Record 0x1941d of unit 1. Role unknown.
- `idbg_prinode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xaa30`, 844 bytes, external).
  No return type in the record; Parameters `struct inode *ip, vtype_t vtype, int header`. Record 0x19468 of unit 1. Role unknown.
- `idbg_prnamenode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xad7c`, 452 bytes, external).
  No return type in the record; Parameters `struct namenode *nmp, int header`. Record 0x194c5 of unit 1. Role unknown.
- `idbg_prlsnode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xaf40`, 1196 bytes, external).
  No return type in the record; Parameters `struct lsnode *lsp`. Record 0x19513 of unit 1. Role unknown.
- `idbg_prcsnode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xb3ec`, 1156 bytes, external).
  No return type in the record; Parameters `struct csnode *csp`. Record 0x1955d of unit 1. Role unknown.
- `prvn` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xb870`, 360 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x195a7 of unit 1. Role unknown.
- `__prvn` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xb9d8`, 56 bytes, external).
  No return type in the record; Parameters `vnode_t *vp, int all`. Record 0x195f8 of unit 1. Role unknown.
- `prvfsvn` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xba10`, 784 bytes, static).
  No return type in the record; Parameters `struct vfs *vfsp, int all`. Record 0x19639 of unit 1. Role unknown.
- `idbg_fifo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xbd20`, 80 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x196ec of unit 1. Role unknown.
- `idbg_fifolist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xbd70`, 296 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x19728 of unit 1. Role unknown.
- `idbg_namelist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xbe98`, 296 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1977a of unit 1. Role unknown.
- `idbg_vlist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xbfc0`, 1172 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x197cc of unit 1. Role unknown.
- `dumppdaistats` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xc454`, 492 bytes, static).
  No return type in the record; Parameters `pda_t *pd`. Record 0x19862 of unit 1. Role unknown.
- `do_efs` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xc640`, 1256 bytes, static).
  No return type in the record; Parameters `struct efs *fs, int summary`. Record 0x19899 of unit 1. Role unknown.
- `idbg_efs` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xcb28`, 52 bytes, external).
  No return type in the record; Parameters `struct efs *fs`. Record 0x19948 of unit 1. Role unknown.
- `idbg_quota` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xcb5c`, 772 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1997b of unit 1. Role unknown.
- `idbg_dqb` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xce60`, 836 bytes, external).
  No return type in the record; Parameters `struct dquot *dqp`. Record 0x19a29 of unit 1. Role unknown.
- `idbg_vfs` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xd1a4`, 268 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x19aaf of unit 1. Role unknown.
- `prvfs` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xd2b0`, 888 bytes, external).
  No return type in the record; Parameters `struct vfs *vfsp, int summary`. Record 0x19afb of unit 1. Role unknown.
- `idbg_vnmap` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xd628`, 248 bytes, external).
  No return type in the record; Parameters `__psunsigned_t addr`. Record 0x19baf of unit 1. Role unknown.
- `print_fmode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xd720`, 112 bytes, static).
  No return type in the record; Parameters `int fmode`. Record 0x19c2f of unit 1. Role unknown.
- `idbg_bmapval` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xd790`, 196 bytes, external).
  No return type in the record; Parameters `struct bmapval *bmapp`. Record 0x19c86 of unit 1. Role unknown.
- `idbg_uio` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xd87c`, 496 bytes, external).
  No return type in the record; Parameters `uio_t *uiop`. Record 0x19d05 of unit 1. Role unknown.
- `idbg_open` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xda6c`, 92 bytes, external).
  No return type in the record; Parameters `struct opena *uap`. Record 0x19d69 of unit 1. Role unknown.
- `idbg_creat` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdac8`, 68 bytes, external).
  No return type in the record; Parameters `struct creata *uap`. Record 0x19d9e of unit 1. Role unknown.
- `idbg_mkdir` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdb0c`, 68 bytes, external).
  No return type in the record; Parameters `struct mkdira *uap`. Record 0x19dd4 of unit 1. Role unknown.
- `idbg_link` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdb50`, 68 bytes, external).
  No return type in the record; Parameters `struct linka *uap`. Record 0x19e0a of unit 1. Role unknown.
- `idbg_rename` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdb94`, 68 bytes, external).
  No return type in the record; Parameters `struct renamea *uap`. Record 0x19e3f of unit 1. Role unknown.
- `idbg_symlink` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdbd8`, 68 bytes, external).
  No return type in the record; Parameters `struct symlinka *uap`. Record 0x19e76 of unit 1. Role unknown.
- `idbg_unlink` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdc1c`, 60 bytes, external).
  No return type in the record; Parameters `struct unlinka *uap`. Record 0x19eae of unit 1. Role unknown.
- `idbg_rmdir` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdc58`, 60 bytes, external).
  No return type in the record; Parameters `struct rmdira *uap`. Record 0x19ee5 of unit 1. Role unknown.
- `idbg_xstat` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdc94`, 76 bytes, external).
  No return type in the record; Parameters `struct xstatarg *uap`. Record 0x19f1b of unit 1. Role unknown.
- `idbg_dumpsym` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdce0`, 244 bytes, external).
  No return type in the record; Parameters `char * *addr`. Record 0x19f51 of unit 1. Role unknown.
- `idbg_callo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xddd4`, 424 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x19fa6 of unit 1. Role unknown.
- `dump_callo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xdf7c`, 312 bytes, static).
  No return type in the record; Parameters `struct callout *p1`. Record 0x19fe8 of unit 1. Role unknown.
- `idbg_calloinfo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe0b4`, 356 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1a02e of unit 1. Role unknown.
- `dump_callout_info` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe218`, 404 bytes, static).
  No return type in the record; Parameters `callout_info_t *cip`. Record 0x1a076 of unit 1. Role unknown.
- `idbg_ptimer` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe3ac`, 372 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1a0d2 of unit 1. Role unknown.
- `idbg_file` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe520`, 500 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1a150 of unit 1. Role unknown.
- `print_procfdt` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe714`, 332 bytes, external).
  Returns `int`; Parameters `proc_t *p, void *arg, int ctl`. Record 0x1a1cc of unit 1. Role unknown.
- `idbg_vfile` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe860`, 160 bytes, external).
  No return type in the record; Parameters `__psint_t ptr`. Record 0x1a264 of unit 1. Role unknown.
- `ltype_to_str` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe900`, 228 bytes, external).
  Returns `char *`; Parameters `int ltype`. Record 0x1a2a9 of unit 1. Role unknown.
- `idbg_filock` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xe9e4`, 180 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x1a319 of unit 1. Role unknown.
- `idbg_sleeplcks` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xea98`, 252 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x1a36b of unit 1. Role unknown.
- `idbg_flock` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xeb94`, 136 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x1a3c0 of unit 1. Role unknown.
- `printfp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xec1c`, 672 bytes, static).
  Returns `int`; Parameters `vfile_t *fp, int slot, int flag`. Record 0x1a40e of unit 1. Role unknown.
- `idbg_map` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xeebc`, 172 bytes, external).
  No return type in the record; Parameters `struct map *p`. Record 0x1a49c of unit 1. Role unknown.
- `idbg_dobitmap` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xef68`, 344 bytes, external).
  No return type in the record; Parameters `struct bitmap *bp, char *name`. Record 0x1a4db of unit 1. Role unknown.
- `idbg_chbtmp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xf0c0`, 208 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1a532 of unit 1. Role unknown.
- `idbg_bitmap` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xf190`, 844 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1a5b5 of unit 1. Role unknown.
- `idbg_svdump` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xf4dc`, 932 bytes, external).
  No return type in the record; Parameters `sv_t *svp`. Record 0x1a63d of unit 1. Role unknown.
- `idbg_pwait` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xf880`, 280 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1a6a4 of unit 1. Role unknown.
- `idbg_page_counter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xf998`, 20 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1a6eb of unit 1. Role unknown.
- `idbg_shdump` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xf9b4`, 684 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x1a754 of unit 1. Role unknown.
- `idbg_semchain` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xfc60`, 528 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1a7b6 of unit 1. Role unknown.
- `idbg_nschain` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0xfe70`, 856 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x1a809 of unit 1. Role unknown.
- `scanchain` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x101c8`, 320 bytes, static).
  No return type in the record; Parameters `int x`. Record 0x1a889 of unit 1. Role unknown.
- `idbgplog` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x10308`, 440 bytes, external).
  No return type in the record; Parameters `semahist_t *hp`. Record 0x1a8da of unit 1. Role unknown.
- `idbg_semctl` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x104c0`, 360 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x1a932 of unit 1. Role unknown.
- `semctl_apply` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x10628`, 600 bytes, static).
  Returns `int`; Parameters `char *str, func (pointer to a subroutine type), int ac, char * *av`. Record 0x1a9b6 of unit 1. Role unknown.
- `semctl_describe` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x10880`, 148 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x1aac7 of unit 1. Role unknown.
- `semctl_ctlmeter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x10914`, 2056 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1ab1e of unit 1. Role unknown.
- `semctl_ctlmeter_aux` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1111c`, 1876 bytes, static).
  No return type in the record; Parameters `sema_t *sp, int ac, char * *av`. Record 0x1ac3d of unit 1. Role unknown.
- `semctl_defhist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x11870`, 436 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1adc5 of unit 1. Role unknown.
- `semctl_defkhist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x11a24`, 408 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1ae0c of unit 1. Role unknown.
- `semctl_defkhlen` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x11bbc`, 224 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1ae54 of unit 1. Role unknown.
- `semctl_defmeter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x11c9c`, 408 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1aeb6 of unit 1. Role unknown.
- `semctl_help` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x11e34`, 152 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1aefe of unit 1. Role unknown.
- `semctl_histmax` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x11ecc`, 220 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1af53 of unit 1. Role unknown.
- `semctl_sema` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x11fa8`, 120 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1afbb of unit 1. Role unknown.
- `semctl_semlog` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x12020`, 2124 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1afff of unit 1. Role unknown.
- `semctl_what` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1286c`, 120 bytes, static).
  No return type in the record; Parameters `int ac, char * *av`. Record 0x1b153 of unit 1. Role unknown.
- `printid` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x128e4`, 320 bytes, static).
  No return type in the record; Parameters `kthread_t *kt, uint64_t id, char *str`. Record 0x1b197 of unit 1. Role unknown.
- `idbg_semdump` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x12a24`, 940 bytes, external).
  No return type in the record; Parameters `sema_t *sp`. Record 0x1b206 of unit 1. Role unknown.
- `idbg_mutexdump` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x12dd0`, 764 bytes, external).
  No return type in the record; Parameters `mutex_t *mp`. Record 0x1b26c of unit 1. Role unknown.
- `idbg_dumphisto` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x130cc`, 1104 bytes, static).
  No return type in the record; Parameters `k_histelem_t *khp, char *str, int timed`. Record 0x1b2d5 of unit 1. Role unknown.
- `idbg_semalist` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1351c`, 688 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x1b38b of unit 1. Role unknown.
- `idbg_chklock` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x137cc`, 936 bytes, external).
  No return type in the record; Parameters `void *l`. Record 0x1b416 of unit 1. Role unknown.
- `idbg_avlnode` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x13b74`, 212 bytes, external).
  No return type in the record; Parameters `avlnode_t *avl`. Record 0x1b45d of unit 1. Role unknown.
- `idbg_lockmeter` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x13c48`, 916 bytes, external).
  No return type in the record; Parameters `k_splockmeter_t *spm`. Record 0x1b495 of unit 1. Role unknown.
- `islock` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x13fdc`, 244 bytes, static).
  Returns `char *`; Parameters `void *lck`. Record 0x1b4cf of unit 1. Role unknown.
- `idbg_hotlocks` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x140d0`, 56 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x1b521 of unit 1. Role unknown.
- `dumphotlock` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x14108`, 1960 bytes, external).
  No return type in the record; Parameters `int op, int n`. Record 0x1b567 of unit 1. Role unknown.
- `idbg_coldlocks` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x148b0`, 376 bytes, external).
  No return type in the record; Parameters `int n`. Record 0x1b62b of unit 1. Role unknown.
- `idbg_mrlock` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x14a28`, 2420 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x1b6c3 of unit 1. Role unknown.
- `idbg_mslock` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1539c`, 228 bytes, external).
  No return type in the record; Parameters `mslock_t *msp`. Record 0x1b7cc of unit 1. Role unknown.
- `idbg_mrlockfull` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x15480`, 2908 bytes, external).
  No return type in the record; Parameters `mrlock_t *mrp`. Record 0x1b803 of unit 1. Role unknown.
- `idbg_pda` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x15fdc`, 80 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1b8bd of unit 1. Role unknown.
- `do_pdas` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1602c`, 320 bytes, external).
  No return type in the record; Parameters `func (pointer to a subroutine type), int which`. Record 0x1b8ef of unit 1. Role unknown.
- `dumppda` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1616c`, 2264 bytes, external).
  No return type in the record; Parameters `pda_t *pd`. Record 0x1b943 of unit 1. Role unknown.
- `idbg_donodepda` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x16a44`, 256 bytes, external).
  No return type in the record; Parameters `nodepda_t *npda`. Record 0x1b9b6 of unit 1. Role unknown.
- `idbg_nodepda` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x16b44`, 204 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1b9f1 of unit 1. Role unknown.
- `idbg_uarea` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x16c10`, 356 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1ba35 of unit 1. Role unknown.
- `idbg_upage` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x16d74`, 336 bytes, external).
  No return type in the record; Parameters `proc_t *pp`. Record 0x1ba78 of unit 1. Role unknown.
- `idbg_fdt` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x16ec4`, 304 bytes, external).
  No return type in the record; Parameters `fdt_t *fdt`. Record 0x1baad of unit 1. Role unknown.
- `idbg_runq` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x16ff4`, 48 bytes, external).
  No return type in the record; Parameters `__psint_t howmany`. Record 0x1bb06 of unit 1. Role unknown.
- `dumpRunq` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x17024`, 1148 bytes, external).
  No return type in the record; Parameters `__psint_t n`. Record 0x1bb3f of unit 1. Role unknown.
- `idbg_dopglst` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x174a0`, 180 bytes, static).
  No return type in the record; Parameters `pglst_t *pglptr`. Record 0x1bb9f of unit 1. Role unknown.
- `idbg_swap` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x17554`, 376 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1bbea of unit 1. Role unknown.
- `idbg_pglst` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x176cc`, 344 bytes, external).
  No return type in the record; Parameters `__psint_t i`. Record 0x1bc3a of unit 1. Role unknown.
- `idbg_kaddr` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x17824`, 180 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x1bc83 of unit 1. Role unknown.
- `idbg_do_eframe` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x178d8`, 368 bytes, external).
  No return type in the record; Parameters `eframe_t *ep`. Record 0x1bcdc of unit 1. Role unknown.
- `idbg_do_ut_eframe` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x17a48`, 108 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut`. Record 0x1bd26 of unit 1. Role unknown.
- `idbg_eframe` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x17ab4`, 184 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1bd71 of unit 1. Role unknown.
- `idbg_ubtrace` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x17b6c`, 1404 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x1bda6 of unit 1. Role unknown.
- `convsp_start` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x180e8`, 508 bytes, static).
  Returns `int`; Parameters `struct uthread_s *ut`. Record 0x1be72 of unit 1. Role unknown.
- `convsp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x18378`, 196 bytes, static).
  Returns `unsigned int *`; Parameters `unsigned int *sp, unsigned int *physsp`. Record 0x1becc of unit 1. Role unknown.
- `idbg_get_stack_bytes` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1843c`, 220 bytes, external).
  No return type in the record; Parameters `struct uthread_s *ut, void *sp, void *dp, int n`. Record 0x1bf23 of unit 1. Role unknown.
- `fetch_kaddr` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x18518`, 368 bytes, external).
  Returns `__psunsigned_t`; Parameters `char *s`. Record 0x1bfae of unit 1. Role unknown.
- `lookup_kname` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x18688`, 276 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x1c052 of unit 1. Role unknown.
- `fetch_kname` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1879c`, 104 bytes, external).
  Returns `char *`; Parameters `void *addr, void *offst`. Record 0x1c0c2 of unit 1. Role unknown.
- `_prsymoff` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x18804`, 272 bytes, external).
  No return type in the record; Parameters `void *addr, char *pre, char *post`. Record 0x1c135 of unit 1. Role unknown.
- `idbg_btrace` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x18914`, 96 bytes, external).
  No return type in the record; Parameters `int maxfr`. Record 0x1c1af of unit 1. Role unknown.
- `btrace` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x18974`, 6236 bytes, static).
  No return type in the record; Parameters `unsigned int *pc, unsigned int *sp, unsigned int *lra, int maxfr, unsigned int *lbound, kthread_t *kt, int myself`. Record 0x1c1e8 of unit 1. Role unknown.
- `txtsrch` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a1d0`, 912 bytes, static).
  Returns `inst_t *`; Parameters `inst_t *pc, unsigned int inst1mask, unsigned int inst2mask, int *which, enum sign im1sign, enum sign im2sign, int *im_ptr, inst_t *llimit, struct spec_funcs * *specf, enum direction dir, unsigned int check1mask, unsigned int check2mask`. Record 0x1c4fc of unit 1. Role unknown.
- `is_jal` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a568`, 168 bytes, static).
  Returns `int`; Parameters `inst_t inst`. Record 0x1c6ca of unit 1. Role unknown.
- `branch_target` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a610`, 232 bytes, static).
  Returns `inst_t *`; Parameters `inst_t inst, inst_t *pc`. Record 0x1c70e of unit 1. Role unknown.
- `fetch_text` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a6f8`, 228 bytes, static).
  Returns `inst_t`; Parameters `inst_t *pc`. Record 0x1c768 of unit 1. Role unknown.
- `ilastframe` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a7dc`, 76 bytes, static).
  Returns `int`; Parameters `k_machreg_t *sp`. Record 0x1c7c0 of unit 1. Role unknown.
- `idbg_symtab` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a828`, 56 bytes, external).
  No return type in the record; Parameters `struct dbstbl * *db, char * *name`. Record 0x1c7f8 of unit 1. Role unknown.
- `idbg_symsize` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a860`, 36 bytes, external).
  No return type in the record; Parameters `int *size`. Record 0x1c83f of unit 1. Role unknown.
- `idbg_ksymrange` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a884`, 56 bytes, external).
  No return type in the record; Parameters `void * *_kftext, void * *_kend`. Record 0x1c878 of unit 1. Role unknown.
- `idbg_scsi` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1a8bc`, 568 bytes, external).
  No return type in the record; Parameters `struct scsi_request *req`. Record 0x1c8c8 of unit 1. Role unknown.
- `idbg_foi` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1aaf4`, 868 bytes, external).
  No return type in the record; Parameters `vertex_hdl_t lun_vhdl`. Record 0x1c90b of unit 1. Role unknown.
- `dkactive` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1ae58`, 120 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x1c9bd of unit 1. Role unknown.
- `dkqueue` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1aed0`, 360 bytes, static).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x1c9ff of unit 1. Role unknown.
- `idbg_dksoftc` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1b038`, 664 bytes, external).
  No return type in the record; Parameters `struct dksoftc *dkp`. Record 0x1ca40 of unit 1. Role unknown.
- `intvec_tinfo` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1b2d0`, 352 bytes, external).
  No return type in the record; Parameters `thd_int_t *tip`. Record 0x1ca78 of unit 1. Role unknown.
- `idbg_call` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1b430`, 208 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x1cab0 of unit 1. Role unknown.
- `idbg_kill` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1b500`, 340 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1cae3 of unit 1. Role unknown.
- `idbg_qbuf` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1b654`, 452 bytes, external).
  No return type in the record; Parameters `__psint_t dev`. Record 0x1cb35 of unit 1. Role unknown.
- `idbg_dozone` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1b964`, 1240 bytes, static).
  No return type in the record; Parameters `zone_t *zp, int all`. Record 0x1cc02 of unit 1. Role unknown.
- `idbg_zone` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1be3c`, 624 bytes, external).
  No return type in the record; Parameters `__psint_t p`. Record 0x1cc8c of unit 1. Role unknown.
- `idbg_tracemasks` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1c0ac`, 256 bytes, external).
  No return type in the record; Parameters `struct pr_tracemasks *t`. Record 0x1cd1c of unit 1. Role unknown.
- `idbg_tstamp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1c1ac`, 220 bytes, external).
  No return type in the record; Parameters `const tstamp_obj_t *objp`. Record 0x1cd56 of unit 1. Role unknown.
- `utrace_dump` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1c288`, 1976 bytes, static).
  No return type in the record; Parameters `tstamp_event_entry_t *event, int cpu, tstamp_event_entry_t *limit, __psint_t sval`. Record 0x1cd8e of unit 1. Role unknown.
- `_idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1ca40`, 3080 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2, __psint_t sval`. Record 0x1ce6e of unit 1. Role unknown.
- `idbg_utrace` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1d648`, 60 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x1d0ba of unit 1. Role unknown.
- `idbg_utrace_find` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1d684`, 84 bytes, external).
  No return type in the record; Parameters `__psint_t arg1, __psint_t arg2`. Record 0x1d103 of unit 1. Role unknown.
- `idbg_utrace_mask` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1d6d8`, 112 bytes, external).
  No return type in the record; Parameters `uint64_t arg1`. Record 0x1d151 of unit 1. Role unknown.
- `idbg_do_softfp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1d748`, 132 bytes, static).
  No return type in the record; Parameters `uthread_t *ut`. Record 0x1d18e of unit 1. Role unknown.
- `idbg_softfp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1d7cc`, 64 bytes, external).
  No return type in the record; Parameters `__psint_t p1`. Record 0x1d1c6 of unit 1. Role unknown.
- `print_maclabel` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1d80c`, 628 bytes, external).
  No return type in the record; Parameters `char *cp, mac_label *lp`. Record 0x1d1fc of unit 1. Role unknown.
- `re_comp` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1da80`, 1904 bytes, external).
  Returns `char *`; Parameters `const char *sp`. Record 0x1d266 of unit 1. Role unknown.
- `re_exec` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1e1f0`, 464 bytes, external).
  Returns `int`; Parameters `const char *p1`. Record 0x1d34f of unit 1. Role unknown.
- `advance` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1e3c0`, 1612 bytes, static).
  Returns `int`; Parameters `const char *lp, char *ep`. Record 0x1d3b2 of unit 1. Role unknown.
- `backref` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1ea0c`, 232 bytes, static).
  Returns `int`; Parameters `int i, const char *lp`. Record 0x1d443 of unit 1. Role unknown.
- `cclass` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1eaf4`, 204 bytes, static).
  Returns `int`; Parameters `char *set, char c, int af`. Record 0x1d495 of unit 1. Role unknown.
- `glob2regex` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1ebc0`, 392 bytes, static).
  No return type in the record; Parameters `char *out, const char *in`. Record 0x1d4f6 of unit 1. Role unknown.
- `strtoargs` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1ed48`, 636 bytes, static).
  No return type in the record; Parameters `char *buf, int *ac, char * *ap`. Record 0x1d548 of unit 1. Role unknown.
- `_padstr` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1efc4`, 324 bytes, external).
  Returns `char *`; Parameters `char *buf, int len`. Record 0x1d5c8 of unit 1. Role unknown.
- `_padstrn` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f108`, 408 bytes, external).
  Returns `char *`; Parameters `char *buf, int buflen, int padlen`. Record 0x1d63d of unit 1. Role unknown.
- `padnum` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f2a0`, 556 bytes, external).
  Returns `char *`; Parameters `__psint_t num, int base, int len`. Record 0x1d6ca of unit 1. Role unknown.
- `padnum64` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f4cc`, 556 bytes, external).
  Returns `char *`; Parameters `__int64_t num, int base, int len`. Record 0x1d77a of unit 1. Role unknown.
- `pad_utrace_qual` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f6f8`, 240 bytes, static).
  Returns `char *`; Parameters `__uint64_t qual`. Record 0x1d82c of unit 1. Role unknown.
- `_mcount` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f7e8`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x1d889 of unit 1. Role unknown.
- `idbg_print_cpumon` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f7f4`, 432 bytes, external).
  No return type in the record; Parameters `cpu_mon_t *cmp`. Record 0x1d8ab of unit 1. Role unknown.
- `idbg_cpumon` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1f9a4`, 652 bytes, external).
  No return type in the record; Parameters `__psint_t x`. Record 0x1d8f6 of unit 1. Role unknown.
- `start_event_counters` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x1fc58`, 1160 bytes, external).
  No return type in the record; Parameters `int mode, short event0, short event1, short event2, short event3`. Record 0x1d97c of unit 1. Role unknown.
- `stop_event_counters` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x203f0`, 636 bytes, external).
  No return type in the record; Parameters `int counters`. Record 0x1daf3 of unit 1. Role unknown.
- `idbg_setpri` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x2069c`, 360 bytes, external).
  No return type in the record; Parameters `__psint_t arg0, char *arg1`. Record 0x1db5e of unit 1. Role unknown.
- `ec0_down` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x20804`, 448 bytes, external).
  Returns `int`; no parameter records. Record 0x1dbf0 of unit 1. Role unknown.
- `ec0_up` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x209c4`, 368 bytes, external).
  Returns `int`; no parameter records. Record 0x1dc42 of unit 1. Role unknown.
- `getrbase` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x20b34`, 12 bytes, external).
  Returns `int`; no parameter records. Record 0x1dca5 of unit 1. Role unknown.
- `idbg_etrace` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x20b40`, 96 bytes, external).
  No return type in the record; Parameters `__psint_t *x`. Record 0x1dcc8 of unit 1. Role unknown.
- `uthreadp_is_valid` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x20cec`, 1192 bytes, external).
  Returns `int`; Parameters `uthread_t *ut`. Record 0x1dd41 of unit 1. Role unknown.
- `procp_is_valid` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x21194`, 1216 bytes, external).
  Returns `int`; Parameters `proc_t *p`. Record 0x1dd82 of unit 1. Role unknown.
- `idbg_conpoll` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x21654`, 224 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x1ddbf of unit 1. Role unknown.
- `idbg_dstat` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x21734`, 236 bytes, external).
  No return type in the record; Parameters `char *s`. Record 0x1ddf5 of unit 1. Role unknown.
- `idbg_dstats` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x21820`, 868 bytes, external).
  No return type in the record; Parameters `uint64_t flag`. Record 0x1de50 of unit 1. Role unknown.
- `idbg_retarget` (`6.5.30/usr/cpu/sysgen/IP32boot/idbg.o` `0x21b84`, 1092 bytes, external).
  No return type in the record; Parameters `void *p, void *a2, int argc, char * *argv`. Record 0x1df00 of unit 1. Role unknown.

### `6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o`

- `melflkaddr` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0x0`, 724 bytes, static).
  No return type in the record; Parameters `ml_info_t *m, __psint_t addr`. Record 0x14a0 of unit 1. Role unknown.
- `idbg_mlist` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0x2d4`, 1676 bytes, external).
  No return type in the record; Parameters `int x`. Record 0x154f of unit 1. Role unknown.
- `idbg_msyms` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0x960`, 360 bytes, external).
  No return type in the record; Parameters `int id`. Record 0x15db of unit 1. Role unknown.
- `idbg_mlkup` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0xac8`, 292 bytes, external).
  No return type in the record; Parameters `char *name`. Record 0x1644 of unit 1. Role unknown.
- `idbg_mlkaddr` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0xbec`, 120 bytes, external).
  No return type in the record; Parameters `__psint_t addr`. Record 0x16a5 of unit 1. Role unknown.
- `mflags` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0xc64`, 348 bytes, static).
  No return type in the record; Parameters `int flags`. Record 0x16ea of unit 1. Role unknown.
- `mtype` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0xdc0`, 544 bytes, static).
  No return type in the record; Parameters `int type`. Record 0x171d of unit 1. Role unknown.
- `idbg_mlinfo` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0xfe0`, 760 bytes, external).
  No return type in the record; Parameters `ml_info_t *m`. Record 0x174e of unit 1. Role unknown.
- `mloadidbgunload` (`6.5.30/usr/cpu/sysgen/IP32boot/mloadidbg.o` `0x13d0`, 196 bytes, external).
  Returns `int`; no parameter records. Record 0x17b8 of unit 1. Role unknown.
