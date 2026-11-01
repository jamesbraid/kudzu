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

- `_check_dbg` (`6.5.30/unix.IP35`, address 0xc000000000083c3c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d0c of unit 134. Role unknown.
- `tpusim_version` (`6.5.30/unix.IP35`, address 0xc000000000083c44, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d40 of unit 134. Role unknown.
- `tpusim_info_chk` (`6.5.30/unix.IP35`, address 0xc000000000083c4c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d78 of unit 134. Role unknown.
- `tpusim_piotrans_addr` (`6.5.30/unix.IP35`, address 0xc000000000083c54, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10db1 of unit 134. Role unknown.
- `tpusim_dmatrans_list` (`6.5.30/unix.IP35`, address 0xc000000000083c5c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10def of unit 134. Role unknown.
- `tpusim_intr_alloc` (`6.5.30/unix.IP35`, address 0xc000000000083c64, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e2d of unit 134. Role unknown.
- `tpusim_intr_connect` (`6.5.30/unix.IP35`, address 0xc000000000083c6c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e68 of unit 134. Role unknown.
- `tpuidbg_version` (`6.5.30/unix.IP35`, address 0xc000000000083c7c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10edd of unit 134. Role unknown.
- `tpu_statename` (`6.5.30/unix.IP35`, address 0xc000000000083c8c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f45 of unit 134. Role unknown.
- `klistio_init` (`6.5.30/unix.IP35`, address 0xc000000000083c94, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f7c of unit 134. Role unknown.
- `klistio32` (`6.5.30/unix.IP35`, address 0xc000000000083c9c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10fb2 of unit 134. Role unknown.
- `klistio64` (`6.5.30/unix.IP35`, address 0xc000000000083cb8, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10fe5 of unit 134. Role unknown.
- `klistio_installed` (`6.5.30/unix.IP35`, address 0xc000000000083cd4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11018 of unit 134. Role unknown.
- `klistio_getparams` (`6.5.30/unix.IP35`, address 0xc000000000083cdc, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11053 of unit 134. Role unknown.
- `pwexit` (`6.5.30/unix.IP35`, address 0xc000000000083cf8, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1108e of unit 134. Role unknown.
- `pwsysmips` (`6.5.30/unix.IP35`, address 0xc000000000083d00, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x110be of unit 134. Role unknown.
- `postwait_installed` (`6.5.30/unix.IP35`, address 0xc000000000083d1c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x110f1 of unit 134. Role unknown.
- `postwait_getparams` (`6.5.30/unix.IP35`, address 0xc000000000083d24, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1112d of unit 134. Role unknown.
- `swoutput` (`6.5.30/unix.IP35`, address 0xc000000000083d50, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x111ba of unit 134. Role unknown.
- `ip_mrouter_cmd` (`6.5.30/unix.IP35`, address 0xc000000000083d6c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x111ec of unit 134. Role unknown.
- `ip_mrouter_done` (`6.5.30/unix.IP35`, address 0xc000000000083d88, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11224 of unit 134. Role unknown.
- `ip_mforward` (`6.5.30/unix.IP35`, address 0xc000000000083d90, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1125d of unit 134. Role unknown.
- `mrt_ioctl` (`6.5.30/unix.IP35`, address 0xc000000000083d98, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11292 of unit 134. Role unknown.
- `legal_vif_num` (`6.5.30/unix.IP35`, address 0xc000000000083db4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x112c5 of unit 134. Role unknown.
- `ip_mroute_slowtimo` (`6.5.30/unix.IP35`, address 0xc000000000083dbc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x112fc of unit 134. Role unknown.
- `ip_mroute_init` (`6.5.30/unix.IP35`, address 0xc000000000083dc4, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11338 of unit 134. Role unknown.
- `ip_mrouter_get` (`6.5.30/unix.IP35`, address 0xc000000000083de0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11370 of unit 134. Role unknown.
- `ip_mrouter_set` (`6.5.30/unix.IP35`, address 0xc000000000083dfc, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x113a8 of unit 134. Role unknown.
- `ip_rsvp_vif_init` (`6.5.30/unix.IP35`, address 0xc000000000083e18, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x113e0 of unit 134. Role unknown.
- `ip_rsvp_vif_done` (`6.5.30/unix.IP35`, address 0xc000000000083e34, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1141a of unit 134. Role unknown.
- `ipip_input` (`6.5.30/unix.IP35`, address 0xc000000000083e50, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11454 of unit 134. Role unknown.
- `ip_rsvp_force_done` (`6.5.30/unix.IP35`, address 0xc000000000083e6c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x11488 of unit 134. Role unknown.
- `ipfilter_kernel` (`6.5.30/unix.IP35`, address 0xc000000000083e88, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x114c4 of unit 134. Role unknown.
- `rpc_gss_secget` (`6.5.30/unix.IP35`, address 0xc000000000083eb0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x115bc of unit 134. Role unknown.
- `__svcrpcsec_gss` (`6.5.30/unix.IP35`, address 0xc000000000083ecc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x115f4 of unit 134. Role unknown.
- `rpc_gss_set_svc_name` (`6.5.30/unix.IP35`, address 0xc000000000083edc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1165e of unit 134. Role unknown.
- `rpc_gss_getcred` (`6.5.30/unix.IP35`, address 0xc000000000083ee4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1169c of unit 134. Role unknown.
- `xlv_tab_subvol_size` (`6.5.30/unix.IP35`, address 0xc000000000083eec, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x116d5 of unit 134. Role unknown.
- `wd95intr` (`6.5.30/unix.IP35`, address 0xc000000000083ef4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11712 of unit 134. Role unknown.
- `wd95_earlyinit` (`6.5.30/unix.IP35`, address 0xc000000000083efc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x11744 of unit 134. Role unknown.

### `6.5.30/unix.IP30`

- `_check_dbg` (`6.5.30/unix.IP30`, address 0xa80000002002c0b0, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x101e0 of unit 99. Role unknown.
- `tpusim_version` (`6.5.30/unix.IP30`, address 0xa80000002002c0b8, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10214 of unit 99. Role unknown.
- `tpusim_info_chk` (`6.5.30/unix.IP30`, address 0xa80000002002c0c0, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1024c of unit 99. Role unknown.
- `tpusim_piotrans_addr` (`6.5.30/unix.IP30`, address 0xa80000002002c0c8, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10285 of unit 99. Role unknown.
- `tpusim_dmatrans_list` (`6.5.30/unix.IP30`, address 0xa80000002002c0d0, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x102c3 of unit 99. Role unknown.
- `tpusim_intr_alloc` (`6.5.30/unix.IP30`, address 0xa80000002002c0d8, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10301 of unit 99. Role unknown.
- `tpusim_intr_connect` (`6.5.30/unix.IP30`, address 0xa80000002002c0e0, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1033c of unit 99. Role unknown.
- `tpuidbg_version` (`6.5.30/unix.IP30`, address 0xa80000002002c0f0, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x103b1 of unit 99. Role unknown.
- `tpu_statename` (`6.5.30/unix.IP30`, address 0xa80000002002c100, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10419 of unit 99. Role unknown.
- `klistio_init` (`6.5.30/unix.IP30`, address 0xa80000002002c108, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10450 of unit 99. Role unknown.
- `klistio32` (`6.5.30/unix.IP30`, address 0xa80000002002c110, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10486 of unit 99. Role unknown.
- `klistio64` (`6.5.30/unix.IP30`, address 0xa80000002002c12c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x104b9 of unit 99. Role unknown.
- `klistio_installed` (`6.5.30/unix.IP30`, address 0xa80000002002c148, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x104ec of unit 99. Role unknown.
- `klistio_getparams` (`6.5.30/unix.IP30`, address 0xa80000002002c150, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10527 of unit 99. Role unknown.
- `pwexit` (`6.5.30/unix.IP30`, address 0xa80000002002c16c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10562 of unit 99. Role unknown.
- `pwsysmips` (`6.5.30/unix.IP30`, address 0xa80000002002c174, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10592 of unit 99. Role unknown.
- `postwait_installed` (`6.5.30/unix.IP30`, address 0xa80000002002c190, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x105c5 of unit 99. Role unknown.
- `postwait_getparams` (`6.5.30/unix.IP30`, address 0xa80000002002c198, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10601 of unit 99. Role unknown.
- `swoutput` (`6.5.30/unix.IP30`, address 0xa80000002002c1c4, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1068e of unit 99. Role unknown.
- `ip_mrouter_cmd` (`6.5.30/unix.IP30`, address 0xa80000002002c1e0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x106c0 of unit 99. Role unknown.
- `ip_mrouter_done` (`6.5.30/unix.IP30`, address 0xa80000002002c1fc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x106f8 of unit 99. Role unknown.
- `ip_mforward` (`6.5.30/unix.IP30`, address 0xa80000002002c204, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10731 of unit 99. Role unknown.
- `mrt_ioctl` (`6.5.30/unix.IP30`, address 0xa80000002002c20c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10766 of unit 99. Role unknown.
- `legal_vif_num` (`6.5.30/unix.IP30`, address 0xa80000002002c228, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10799 of unit 99. Role unknown.
- `ip_mroute_slowtimo` (`6.5.30/unix.IP30`, address 0xa80000002002c230, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x107d0 of unit 99. Role unknown.
- `ip_mroute_init` (`6.5.30/unix.IP30`, address 0xa80000002002c238, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1080c of unit 99. Role unknown.
- `ip_mrouter_get` (`6.5.30/unix.IP30`, address 0xa80000002002c254, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10844 of unit 99. Role unknown.
- `ip_mrouter_set` (`6.5.30/unix.IP30`, address 0xa80000002002c270, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1087c of unit 99. Role unknown.
- `ip_rsvp_vif_init` (`6.5.30/unix.IP30`, address 0xa80000002002c28c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x108b4 of unit 99. Role unknown.
- `ip_rsvp_vif_done` (`6.5.30/unix.IP30`, address 0xa80000002002c2a8, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x108ee of unit 99. Role unknown.
- `ipip_input` (`6.5.30/unix.IP30`, address 0xa80000002002c2c4, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10928 of unit 99. Role unknown.
- `ip_rsvp_force_done` (`6.5.30/unix.IP30`, address 0xa80000002002c2e0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1095c of unit 99. Role unknown.
- `ipfilter_kernel` (`6.5.30/unix.IP30`, address 0xa80000002002c2fc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10998 of unit 99. Role unknown.
- `rpc_gss_secget` (`6.5.30/unix.IP30`, address 0xa80000002002c324, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10a90 of unit 99. Role unknown.
- `__svcrpcsec_gss` (`6.5.30/unix.IP30`, address 0xa80000002002c340, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ac8 of unit 99. Role unknown.
- `rpc_gss_set_svc_name` (`6.5.30/unix.IP30`, address 0xa80000002002c350, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10b32 of unit 99. Role unknown.
- `rpc_gss_getcred` (`6.5.30/unix.IP30`, address 0xa80000002002c358, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10b70 of unit 99. Role unknown.
- `xlv_tab_subvol_size` (`6.5.30/unix.IP30`, address 0xa80000002002c360, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ba9 of unit 99. Role unknown.
- `wd95intr` (`6.5.30/unix.IP30`, address 0xa80000002002c368, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10be6 of unit 99. Role unknown.
- `wd95_earlyinit` (`6.5.30/unix.IP30`, address 0xa80000002002c370, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10c18 of unit 99. Role unknown.

### `6.5.30/unix.IP27`

- `_check_dbg` (`6.5.30/unix.IP27`, address 0xc00000000005bcac, 8 bytes, external).
  Returns `int`; no parameter records. Record 0xff5f of unit 132. Role unknown.
- `tpusim_version` (`6.5.30/unix.IP27`, address 0xc00000000005bcb4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0xff93 of unit 132. Role unknown.
- `tpusim_info_chk` (`6.5.30/unix.IP27`, address 0xc00000000005bcbc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0xffcb of unit 132. Role unknown.
- `tpusim_piotrans_addr` (`6.5.30/unix.IP27`, address 0xc00000000005bcc4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10004 of unit 132. Role unknown.
- `tpusim_dmatrans_list` (`6.5.30/unix.IP27`, address 0xc00000000005bccc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10042 of unit 132. Role unknown.
- `tpusim_intr_alloc` (`6.5.30/unix.IP27`, address 0xc00000000005bcd4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10080 of unit 132. Role unknown.
- `tpusim_intr_connect` (`6.5.30/unix.IP27`, address 0xc00000000005bcdc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x100bb of unit 132. Role unknown.
- `tpuidbg_version` (`6.5.30/unix.IP27`, address 0xc00000000005bcec, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10130 of unit 132. Role unknown.
- `tpu_statename` (`6.5.30/unix.IP27`, address 0xc00000000005bcfc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10198 of unit 132. Role unknown.
- `klistio_init` (`6.5.30/unix.IP27`, address 0xc00000000005bd04, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x101cf of unit 132. Role unknown.
- `klistio32` (`6.5.30/unix.IP27`, address 0xc00000000005bd0c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10205 of unit 132. Role unknown.
- `klistio64` (`6.5.30/unix.IP27`, address 0xc00000000005bd28, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10238 of unit 132. Role unknown.
- `klistio_installed` (`6.5.30/unix.IP27`, address 0xc00000000005bd44, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1026b of unit 132. Role unknown.
- `klistio_getparams` (`6.5.30/unix.IP27`, address 0xc00000000005bd4c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x102a6 of unit 132. Role unknown.
- `pwexit` (`6.5.30/unix.IP27`, address 0xc00000000005bd68, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x102e1 of unit 132. Role unknown.
- `pwsysmips` (`6.5.30/unix.IP27`, address 0xc00000000005bd70, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10311 of unit 132. Role unknown.
- `postwait_installed` (`6.5.30/unix.IP27`, address 0xc00000000005bd8c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10344 of unit 132. Role unknown.
- `postwait_getparams` (`6.5.30/unix.IP27`, address 0xc00000000005bd94, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10380 of unit 132. Role unknown.
- `swoutput` (`6.5.30/unix.IP27`, address 0xc00000000005bdc0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1040d of unit 132. Role unknown.
- `ip_mrouter_cmd` (`6.5.30/unix.IP27`, address 0xc00000000005bddc, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1043f of unit 132. Role unknown.
- `ip_mrouter_done` (`6.5.30/unix.IP27`, address 0xc00000000005bdf8, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10477 of unit 132. Role unknown.
- `ip_mforward` (`6.5.30/unix.IP27`, address 0xc00000000005be00, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x104b0 of unit 132. Role unknown.
- `mrt_ioctl` (`6.5.30/unix.IP27`, address 0xc00000000005be08, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x104e5 of unit 132. Role unknown.
- `legal_vif_num` (`6.5.30/unix.IP27`, address 0xc00000000005be24, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10518 of unit 132. Role unknown.
- `ip_mroute_slowtimo` (`6.5.30/unix.IP27`, address 0xc00000000005be2c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1054f of unit 132. Role unknown.
- `ip_mroute_init` (`6.5.30/unix.IP27`, address 0xc00000000005be34, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1058b of unit 132. Role unknown.
- `ip_mrouter_get` (`6.5.30/unix.IP27`, address 0xc00000000005be50, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x105c3 of unit 132. Role unknown.
- `ip_mrouter_set` (`6.5.30/unix.IP27`, address 0xc00000000005be6c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x105fb of unit 132. Role unknown.
- `ip_rsvp_vif_init` (`6.5.30/unix.IP27`, address 0xc00000000005be88, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10633 of unit 132. Role unknown.
- `ip_rsvp_vif_done` (`6.5.30/unix.IP27`, address 0xc00000000005bea4, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1066d of unit 132. Role unknown.
- `ipip_input` (`6.5.30/unix.IP27`, address 0xc00000000005bec0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x106a7 of unit 132. Role unknown.
- `ip_rsvp_force_done` (`6.5.30/unix.IP27`, address 0xc00000000005bedc, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x106db of unit 132. Role unknown.
- `ipfilter_kernel` (`6.5.30/unix.IP27`, address 0xc00000000005bef8, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10717 of unit 132. Role unknown.
- `rpc_gss_secget` (`6.5.30/unix.IP27`, address 0xc00000000005bf20, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x1080f of unit 132. Role unknown.
- `__svcrpcsec_gss` (`6.5.30/unix.IP27`, address 0xc00000000005bf3c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10847 of unit 132. Role unknown.
- `rpc_gss_set_svc_name` (`6.5.30/unix.IP27`, address 0xc00000000005bf4c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x108b1 of unit 132. Role unknown.
- `rpc_gss_getcred` (`6.5.30/unix.IP27`, address 0xc00000000005bf54, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x108ef of unit 132. Role unknown.
- `do_unmount` (`6.5.30/unix.IP27`, address 0xc00000000005bf5c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10928 of unit 132. Role unknown.
- `nfs3_getvnodeops` (`6.5.30/unix.IP27`, address 0xc00000000005bf78, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x1095c of unit 132. Role unknown.
- `__nfs3_get_readahead` (`6.5.30/unix.IP27`, address 0xc00000000005bf80, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10996 of unit 132. Role unknown.
- `lockd_dispatch` (`6.5.30/unix.IP27`, address 0xc00000000005bfa0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10a61 of unit 132. Role unknown.
- `klm_lockctl` (`6.5.30/unix.IP27`, address 0xc00000000005bfc4, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10ac7 of unit 132. Role unknown.
- `record_remote_name` (`6.5.30/unix.IP27`, address 0xc00000000005bfe0, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10afc of unit 132. Role unknown.
- `nsm_cntl` (`6.5.30/unix.IP27`, address 0xc00000000005bffc, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10b38 of unit 132. Role unknown.
- `rfs_dispatch` (`6.5.30/unix.IP27`, address 0xc00000000005c018, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10b6a of unit 132. Role unknown.
- `xattr_dispatch` (`6.5.30/unix.IP27`, address 0xc00000000005c034, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10ba0 of unit 132. Role unknown.
- `nfs_getfh` (`6.5.30/unix.IP27`, address 0xc00000000005c050, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10bd8 of unit 132. Role unknown.
- `exportfs` (`6.5.30/unix.IP27`, address 0xc00000000005c06c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10c0b of unit 132. Role unknown.
- `nfs_cnvt` (`6.5.30/unix.IP27`, address 0xc00000000005c088, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10c3d of unit 132. Role unknown.
- `nfs_cnvt3` (`6.5.30/unix.IP27`, address 0xc00000000005c0a4, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10c6f of unit 132. Role unknown.
- `nfs_purge_vfsp` (`6.5.30/unix.IP27`, address 0xc00000000005c0c0, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ca2 of unit 132. Role unknown.
- `nfs_generic_cmd` (`6.5.30/unix.IP27`, address 0xc00000000005c0c8, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10cda of unit 132. Role unknown.
- `findexport` (`6.5.30/unix.IP27`, address 0xc00000000005c0f4, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10d72 of unit 132. Role unknown.
- `findexport_path` (`6.5.30/unix.IP27`, address 0xc00000000005c0fc, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10da6 of unit 132. Role unknown.
- `fhtovp` (`6.5.30/unix.IP27`, address 0xc00000000005c104, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ddf of unit 132. Role unknown.
- `checkauth` (`6.5.30/unix.IP27`, address 0xc00000000005c10c, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e0f of unit 132. Role unknown.
- `kcdb_enabled` (`6.5.30/unix.IP27`, address 0xc00000000005c114, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10e42 of unit 132. Role unknown.
- `nfs_auth_strerror` (`6.5.30/unix.IP27`, address 0xc00000000005c134, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f0d of unit 132. Role unknown.
- `cnfs_stuff_rdplus_data` (`6.5.30/unix.IP27`, address 0xc00000000005c13c, 28 bytes, external).
  Returns `int`; no parameter records. Record 0x10f48 of unit 132. Role unknown.
- `xlv_tab_subvol_size` (`6.5.30/unix.IP27`, address 0xc00000000005c158, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10f88 of unit 132. Role unknown.
- `wd95intr` (`6.5.30/unix.IP27`, address 0xc00000000005c160, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10fc5 of unit 132. Role unknown.
- `wd95_earlyinit` (`6.5.30/unix.IP27`, address 0xc00000000005c168, 8 bytes, external).
  Returns `int`; no parameter records. Record 0x10ff7 of unit 132. Role unknown.
