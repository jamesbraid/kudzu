# IP35 kernel: how a PCI interrupt is re-posted after its handlers finish

How the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, linked 64-bit image with a symbol table) makes sure a PCI
interrupt whose pin is still asserted is raised again once its handlers have run, and which hardware registers that
touches. The question came from a host-bridge model that sends one interrupt packet per rising edge: if the
processor clears the node's pending bit while the pin stays high, nothing raises it again. Every routine below was read
in full unless a line says otherwise; grades are as in [klconfig.md](klconfig.md). Method: the retools Capstone
decoder (`tools/hostfacts.py`) over the linked image. Every address is in the `root/6.5.30/unix.IP35` held in this
repository (13,104,040 bytes). An earlier version of this page quoted addresses
from the 6.5.30 miniroot kernel of the installation boot set, a different link of the same release whose routines hold
identical code at addresses 0x7b30 lower (0x7b34 lower for `pic_attach2` and `xbridge_attach`); they were re-derived
here from the held image, routine by routine.

## The re-post

- `pcibr_force_interrupt` (`6.5.30/unix.IP35`, address 0xc0000000000a4d38, 232 bytes) takes an interrupt record
  (the word at +0 is the bridge soft state, the word at +8 the pin number). It reads the halfword at soft +114 (call
  it the bridge kind). **Established.**
  - Kind nonzero: it calls `pcireg_force_intr_set` (0xc0000000000b3da4) with the soft state and the pin and returns.
    That routine, for kinds 2 and 3, stores the 64-bit value 1 at the bridge's register block + 448 + 8 x pin (offset
    0x1c0 + 8n); for kind 1 it stores a 32-bit 1 at + 452 + 8 x pin; kinds 0 and -1 go to two further arms that were
    not read. **Established** for kinds 1 to 3, unread for the rest.
  - Kind zero: it reads the bridge's interrupt status (`pcireg_intr_status_get`, 0xc0000000000b0fb0), and if the bit for
    this pin is set it asks the crosstalk layer for the interrupt's vector (`xtalk_intr_vector_get`) and target
    processor (`xtalk_intr_cpuid_get`) and stores `vector | 0x100` (bit 8 set: **set** the pending level) to the target
    node's processor-interface pending-modify register through the node window: 0x1800090 for interface 0 or
    0x1a00090 for interface 1, node number shifted left 33, interface chosen by bit 1 of a byte at +1320 of the
    target processor's per-processor data. The kernel sets the pending bit itself and does not go through the bridge.
    **Established.**
- The bridge kind is 2 for the PIC: the PIC attach routine `pic_attach2` (0xc00000000009b884) stores 2 at soft +114
  (0xc00000000009ba84) next to a 1024 at soft +112. The XBridge attach routine `xbridge_attach` (0xc00000000009c8ac)
  stores 0 there (0xc00000000009cb14) and 1 when the widget identification word has part 0xd002 and manufacturer
  code 36 (0xc00000000009d3e4). **Established.** Which part numbers a real Tezro PIC reports at widget 0 is in the
  chips repository's PIC page, not here.
- **When it runs.** The interrupt record's word at +24 counts threaded handler invocations in flight. The dispatch
  routine `pcibr_intr_func` (0xc0000000000a5cfc, 592 bytes) returns at once when that word is nonzero at entry. It
  walks the record's list of connected handlers (list head at +16; bit 0 of a handler's first word is "connected",
  bit 1 "runs in a thread"). A handler without the thread bit is called directly (0xc0000000000a5dbc to
  0xc0000000000a5dc8, after a read of the bridge write-flush register when the record's word at +28 is nonzero). For a
  threaded handler it bumps the in-flight word with load-linked and store-conditional (0xc0000000000a5e34 to
  0xc0000000000a5e40), calls `pcibr_wrap_put` and posts the handler's semaphore with `vsema` (0xc0000000000a5e54). The
  completion routine the handler thread runs (0xc0000000000a4e20, 120 bytes up to the next routine's prologue at
  0xc0000000000a4e98, no symbol covers it) calls the handler's function, drops the word the same way (0xc0000000000a4e4c to 0xc0000000000a4e58) and, when the word has reached zero,
  calls `pcibr_force_interrupt` (0xc0000000000a4e88). The dispatch routine makes the same call when the word is zero
  after its walk (0xc0000000000a5e68 to 0xc0000000000a5f3c). So the re-post happens when the last threaded handler in
  flight for that pin finishes, or at the end of a walk that started none. **Established.**
- **What it does when nothing handled the interrupt.** The dispatch routine sets a flag at the start and clears it when
  it calls or wakes a handler. When the flag is still set at the end it takes the bridge lock (soft +140), clears the pin's bit
  in the bridge interrupt-enable register (`pcireg_intr_enable_bit_clr`, 0xc0000000000b13d4), reads back the
  flush register (`pcireg_tflush_get`) and releases the lock (0xc0000000000a5e78 to 0xc0000000000a5eac): an
  interrupt nobody claimed is masked at the bridge. **Established.**
- For a kind-2 bridge whose word at soft +120 has 1 in its low nibble, the dispatch routine also clears and sets the
  pin's enable bit once under the lock before it starts (0xc0000000000a5ee0 to 0xc0000000000a5f34). What sets soft +120
  is the chip's revision: `pic_attach2` (`6.5.30/unix.IP35`, 0xc00000000009b884) reads the bridge's identification
  register with `pcireg_id_get` (call at 0xc00000000009b8f8), forms `((id >> 28) & 0xf) | (((id >> 12) & 0xffff) << 4)`
  (0xc00000000009b908 to 0xc00000000009b91c; the revision field, bits 31:28, in the low four bits and the part number,
  bits 27:12, above it), keeps it in a local slot (0xc00000000009b924), attaches it to the vertex as a label
  (`hwgraph_info_add_LBL`, call at 0xc00000000009b920) and stores it at soft +120 (load 0xc00000000009bab4, store
  0xc00000000009bab8). It is a local of `pic_attach2`, not a stack argument. So "1 in the low nibble" means the PIC
  reports revision 1. The dispatch routine (`pcibr_intr_func`, 0xc0000000000a5cfc in the same image) tests it as
  `(1 << (word & 0xf)) & 2` (0xc0000000000a5ee0 to 0xc0000000000a5eec), and `pic_attach2` makes the same test at
  0xc00000000009bdb4 to 0xc00000000009bde0 to choose between two values before `pcireg_control_set` (call at
  0xc00000000009bdf4; the control-register bits it changes were not read). **Established** for the stores and tests;
  that the identification register has its revision in bits 31:28 comes from the routine's own shifts and the label
  it builds, and is **inferred** for the hardware layout.
  Whether the hardware raises an interrupt on an enable-bit set while the pin
  is high is **unknown** from software. Settling test: the bridge chip's published programming description of its interrupt enable and status registers (look it up by the chip's name and revision first), which states whether the pending state is level or edge sensitive; if it does not say, only a bench measurement on a real Origin 350 base-I/O bridge answers it (write the enable bit with a slot's pin held asserted and watch for an interrupt packet), and that is a last resort because it needs the hardware.

## What follows for a model

- A PCI interrupt model for the PIC that sends a packet only on edges is compatible with IRIX 6.5.30 only if the
  force-interrupt register at +0x1c0 + 8n re-sends the pin's packet. With that register working, the kernel itself
  re-posts the level after the last handler returns and the node's pending register needs no re-assertion
  of its own. **Inferred**: the register's effect on the hardware (the kernel only writes 1; the accessor name is the
  other evidence); route: the PIC specification or a hardware trace of a write to +0x1c0.
- A model that re-sets the pending bit by itself after a clear while the pin stays high does so on behalf of guests
  that have no such re-post. IRIX 6.5.30 is not one of them for kind 1 or 2 bridges.
- The PROM's own interrupt-line test is a different thing and is in the chips repository's
  `docs/questions/ip35-prom-reset-barrier.md`.
