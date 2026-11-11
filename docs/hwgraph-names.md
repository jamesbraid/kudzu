# Hardware-graph names in the 6.5.30 kernels

This page lists the names that the 6.5.30 IP27, IP30 and IP35 kernels put into the hardware graph
(the `/hw` tree): path components, edge names and the path formats built from them. It also
records what `hinv` prints from that tree and what the `ioconfig` configuration files say about
device numbering. The generated tables are [hwgraph-sites.tsv](hwgraph-sites.tsv), one row per
call that adds a name, and [hwgraph-edges.tsv](hwgraph-edges.tsv), one row per distinct name.
[scripts/hwgraph-sites.py](../scripts/hwgraph-sites.py) makes both from the kernel binaries.

Grades. "established" means the call was read in a routine the fact sheet covers, and the name it
passes is a literal string or a format that a `sprintf` in the same routine builds. "inferred"
means the name sits in a stack buffer whose writer is in another routine, or the callee was not
read. "unknown" means the name is a register or a pointer the routine does not fix. The per-machine
path shapes below cite the table rows they come from.

## The graph-add primitives

These bodies were read in `6.5.30/unix.IP27`. The IP30 and IP35 kernels carry the same primitive
names; their bodies were not compared line by line.

- `hwgraph_path_add` (`6.5.30/unix.IP27` `0xc00000000029561c`, 384 bytes) takes a start vertex, a path and an output
  pointer. It calls `graph_path_get_component` with a stack buffer (`sp-352`) that the later calls
  use as the name, then
  `hwgraph_traverse`, `hwgraph_vertex_create` (whose first argument is the output vertex pointer,
  not a name), `hwgraph_edge_add` with that component buffer as the name, and the unref and destroy
  calls. Every component therefore becomes a vertex and an edge named by the component. The order of
  the calls is established from the body; that a vertex is created only when the traverse misses is
  inferred from that order. Established for the call list.
- `hwgraph_edge_add` (`6.5.30/unix.IP27` `0xc000000000294e28`, 204 bytes) takes the from vertex in a0, the to vertex in
  a1 and the edge name in a2. It passes the name on to `graph_edge_add`. Established.
- `hwgraph_char_device_add` (`6.5.30/unix.IP27` `0xc000000000295bec`, 344 bytes) takes a start vertex (a0), a path (a1),
  a driver prefix (a2) and an output pointer (a3). It looks up the driver with `device_driver_get`
  on a2, then, when a1 is set, calls `hwgraph_path_add` with a1 as the path. Established.
- `hwgraph_block_device_add` has one site per kernel. Its body was not read, so its rows are graded
  inferred.
- `hwgraph_device_add`, `hwgraph_link_add`, `graph_vertex_create` and `graph_edge_add` (the
  lower-level graph calls) appear in the table with role "unread". Their names do not feed the edge
  table.

## Names that the kernels add

The table holds 615 call sites: 205 in IP27, 163 in IP30 and 247 in IP35. Of these, 167, 122 and
193 are established; 27, 28 and 41 are unknown because the name is a pointer the routine does not
fix; the rest are inferred. Each site is in one of two forms: a path (`hwgraph_path_add` and the
char and block device adds, where each component becomes a vertex and an edge), or an edge name
(`hwgraph_edge_add`).

[hwgraph-edges.tsv](hwgraph-edges.tsv) has 146 distinct names: 127 literals and 19 formats. Names
used on one kernel only:

- IP27 only: `elsc`, `machdep`, `mgras`, `mmsc_control`, `nvram`, `rad1.%d`, `rad1_b`, `rps`,
  `slot`, `ttyrd%d`, `xbox_rps`.
- IP30 only: `.connection`, `15`, `flash`, `gio`, `heart`, `odsy`.
- IP35 only: `.nullkeyboard`, `.nullmouse`, `1`, `L1`, `agp`, `console`, `emu_subsys`,
  `if_xframe`, `input`, `ioc4`, `keyboard`, `mouse`, `pci-x`, `spi`, `tablet`, `target`, `usb`,
  `usbaudio_subsys`, `xframe`, `xpmem`.

Names from the rest of the list are shared by at least two kernels. Format names keep their
placeholders as written (`%d`, `%s`, `%M`, `%llx`, `%c`).

## Path shapes by machine

Each shape names the routine and the call that builds it.

### IP27 (Onyx2, Origin 2000 class)

- `module/%M` and `module/%M/elsc` for each module, from `klhwg_add_all_modules`. Established.
- `module/%M/slot/%s/%s`, the board path for an I/O board, from `io_xswitch_widget_init`.
  Established. The node-board path (`%H/slot/%s/%s`, from `board_to_path`) is described in
  [hwgraph.md](hwgraph.md), which read that routine. It is not re-read here: the call that passes
  the board's path (`klhwg_add_node`, `0xc000000000041bcc`) takes a stack buffer, and the table
  grades it inferred.
- `module/%d/elsc/tty`, from `i2cinit`. Established.
- `cpubus` (literal) and a `%d` child under it for the subnode, from `klhwg_add_node`; `cpu`
  (literal) from the same routine; `cpubus/<n>/<letter>` (format `%s/%d/%c`) from `klhwg_add_cpu`;
  and `disabled/cpu/<letter>` (format `%s/%s/%c`) from `klhwg_add_disabled_cpu`. Established.
- `hub`, `memory`, `rps` and `xbox_rps` under the node vertex, from `klhwg_add_hub`,
  `klhwg_add_mem`, `klhwg_add_rps` and `klhwg_add_xbox_rps`. Established.
- Crossbow and router links are edges: `xtalk` (`klhwg_add_xbow`) and `link` (`klhwg_connect_hubs`),
  and per-port router edges named `%d` (`klhwg_connect_one_router`). Established.
- Device paths: `scsi_ctlr/0`, `lun/%d`, `node/%llx/port/%llx`, `disk`, `rdisk`, `net`, `pci`,
  `vme`, `usrvme`, `ttys`, and the char-device nodes `mon`, `nvram`, `controller`, `xbow`,
  `machdep/mmsc_control`, `net/if_tg`. Established.

### IP30 (Octane)

- No path begins with `module/`. Every IP30 site in the table uses a node or crossbow form.
- `node` (`heart_startup`, `init_all_devices`), `xtalk` (`heart_startup`) and `xtalk/15`
  (`init_all_devices`). Established.
- `node/%llx` (`fire_finish_probe`) and `node/%llx/port/%llx` (`scsi_node_port_vertex_add`).
  Established.
- `cpu` and `memory` (`init_all_devices`), `ohci/0` (`fire_attach`), `scsi_ctlr/0` (`fcadp_ctlr_add`),
  and edges `heart` (`heart_attach`), `io` (`heart_startup`) and `.connection` (`heart_attach`).
  Established.

### IP35 (Origin 300/350, Onyx 300/350)

- `module/%M` and `module/%M/L1` (`klhwg_add_all_modules`), and `module/%M/L1/tty` (`l1_attach`).
  Established.
- `module/%M/%s` (`pic_bus1_redist`), `module/%M/%s/L1` and `module/%M/%s/%s/%d`
  (`io_xswitch_widget_init`). Established.
- `hub`, `memory`, `cpu`, `cpubus`, and `%s/%d/%c` and `%s/%s/%c` for enabled and disabled
  processors (`klhwg_add_hub`, `klhwg_add_mem`, `klhwg_add_node`, `klhwg_add_cpu`,
  `klhwg_add_disabled_cpu`). Established.
- Bus paths without a module: `agp/0`, `agp/1`, `pci-x/0`, `pci-x/1` (`pic_attach`), `vme`
  (`universe_attach`), `usb` (`usb_attach`). Established.
- Edges: `link` and `xtalk` as on IP27, and `console` (`l1_attach`). Established.

## Format strings that hinv prints

`hinv` (`6.5.30/sbin/hinv`, stripped, 32-bit) has about 73 format strings that mention a path, a
module or a slot. Only the following were traced, from the call sites in the binary. Routine starts
are the addresses after the preceding return, because the binary has no symbols.

- `/hw/net/%s%d` (`0x1000c0e0`). Established. The routine at `0x100021cc` builds it with `sprintf`
  (`0x100021e4`) into a stack buffer, then passes that buffer to `filename_to_devname`
  (`0x100021f8`). So `hinv` turns the graph path of a network interface into its device name.
- `, slot %s` (`0x1000c1b0`). Established at its two `printf` calls, `0x10002ae8` and `0x10002cb8`.
- `/hw/net/ef%d` (`0x1000c190`, referenced at `0x100029b0`), `/hw/gm%d` (`0x1000c1d0`, referenced at
  `0x10002b78`), `, module %s` (`0x1000c1a0`, referenced at `0x10002b1c` and `0x10002ce8`) and
  `module %s ` (`0x1000c7e8`, referenced at three sites). Located only. The call that consumes each
  one was not traced; the call at `0x10003ae4` and the one at `0x10005cd8` reach their function
  through a computed value. Unknown which routine prints them.
- The other format strings in the binary (for example the cache sizes, the SCSI and tape lines and
  the audio lines) are not path formats. They are not traced here.

## ioconfig numbering

Neither `6.5.30` nor `6.5.22` has an `ioconfig` binary in `root/`. The 6.5.22 configuration
directories (`root/6.5.22/var/sysgen.IP27/ioconfig`, `sysgen.IP30/ioconfig` and
`sysgen.IP35/ioconfig`) hold text files only. The 6.5.30 tree has no `var/sysgen` at all. So the
numbering code is unread.

What the 6.5.22 files state (established as file content; applied by a binary not held):

- The `README` in each directory gives seven fields: `<class> <type> <state> <suffix> <pattern>
  <start_num> <ioctl_num>`. A value of -1 means the field is ignored; for example a `<pattern>` of
  -1 means the basename of the device's canonical name is used. The more generic entry for a class
  must come before the more specific one for it to take effect.
- `start_num` is the first number given to the devices that match the entry. The README's example
  uses 2 for `tty`. How later devices of the same entry are numbered is not stated in the file.
- Entries in the directories (6.5.22 only):
  - `vme` (IP27 and IP35): class 13, type 0, start 1, ioctl `0x20007363`.
  - `ifcl` (IP27 and IP35): class 0, type 0, state 0, suffix -1, pattern `xplink`, start 0.
  - `dmedia` (IP27, IP30 and IP35): class 10, type 7, start 1, ioctl `0x6d687767` (`mhwg`).
  - `usb` (IP35 only): class 26 with types 769, 770 and 771 (keyboard, mouse, tablet), start 0, and
    class 10 type 6 (USB audio), start 0; the ioctl is `0x7501` for the HID entries.

Open: the increment rule and any per-machine numbering come from the `ioconfig` binary. No shipped
binary in the held trees reads these files, so the rule is unknown.

## Open ends

- `hwgraph_block_device_add`, `hwgraph_device_add`, `hwgraph_link_add` and the lower-level
  `graph_*` routines were not read. Their sites are in the table with that grade.
- 15 path sites are graded inferred: 12 take their name from a stack buffer filled by a routine
  outside the window (for example `board_to_path`), and 3 are `block` names passed to
  `hwgraph_block_device_add`, whose body was not read. Reading the filling routine would move the
  first group.
- The `ioconfig` binary is not in any held tree. The numbering rule is unknown until it is found.
- hinv consumers for the located format strings above.
