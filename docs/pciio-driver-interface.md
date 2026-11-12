# PCI driver interface: space codes, flag bits, endianness, interrupt target, id registry

What a third-party IRIX PCI driver observes when it calls the generic PCI layer (`pciio`) and the Bridge-family provider (`pcibr`) on IP30 (Octane), IP27 (Origin 200/2000, Onyx2) and IP35 (Origin 300/350, Onyx 300/350). Read for a driver that maps a window, sets endianness, makes a DMA map, binds an interrupt to a named CPU, and is looked for under the hardware graph `.id` tree.

Evidence: the IRIX 6.5.30 kernel objects `IP30boot/pcibr.o`, `IP30boot/pciio.o`, `IP30boot/cdl.o`, `IP30boot/xbridge.o`, `IP30boot/heart.o` and the linked `unix.IP30`; the IP27 and IP35 objects for the comparisons named below. Addresses are `.text` offsets in the object unless a kernel address is given. Constant names (`PCIIO_SPACE_WIN`, `PCIIO_BYTE_STREAM` and the rest) are the names in the public Linux SN headers, which are a hint for naming only; every value below is read at an address in the shipped objects. The IP35 side of the same code is in [ip35-pciio.md](ip35-pciio.md) and [ip35-pci-bridge.md](ip35-pci-bridge.md); where they overlap this page only adds what they do not say.

## Space codes of the PIO calls

`pciio_piomap_alloc` and `pciio_piotrans_addr` (and `pciio_pio_addr`, which calls both) pass the space number to the provider. In `pcibr` both provider routines (`pcibr_piomap_alloc` 0x183c, call at 0x18c4; `pcibr_piotrans_addr` 0x1b1c, call at 0x1b7c) call one unlabelled helper at 0xf6c that turns a space and a PCI address into a crosstalk offset. Its dispatch on the space number (0xf6c to 0x1018, then the per-space blocks) is:

| Space | Name | What the helper does | Grade |
|---|---|---|---|
| 0 | none | returns -1 (failure) | established |
| 1 | expansion ROM | the range must lie inside the function's ROM window (base at function record +416, size at +424); the space is then treated as 4 with the ROM base added (0x1378) | established |
| 2 | I/O | crosstalk offset = 0x1_0000_0000 + PCI I/O address (block at 0x149c). Fails when the address plus length exceeds 4 GiB, and fails when the bridge revision word (soft +120) is not above 0xc0023 | established |
| 4, 5 | memory, 32-bit memory | crosstalk offset = 0x4000_0000 + PCI address; the range must end by 0x4000_0000 (block at 0x12a8, bridge types 0 and 1; the type 2 branch at 0x153c was not read) | established |
| 6 | 64-bit memory | crosstalk offset = 0x8000_0000 + PCI address (block at 0x1468); fails above the limit the constant at gp-32736 holds, whose value was not read | established for the offset, unknown for the limit |
| 7 | configuration | only for a range ending within the first 0x100 bytes; offset is the slot window: slot * 0x1000 + 0x20000 (0x820000 when soft +72, the bus number, is nonzero), block at 0x14f8; the type 2 branch at 0x17f4 was not read | established |
| 8 to 13 | window 0 to 5 | space - 8 indexes the function's window table, six 56-byte records from function record +72 (type word at +0, base at +8, size at +16). A zero type word fails. Otherwise the range is checked against the record's size, the record's base is added to the address and the helper continues as if the call had named the record's type, which must be 2, 4, 5 or 6 to succeed (0x1024 to 0x1098) | established |

So **space 8 selects window 0 and space 9 window 1**: `PCIIO_SPACE_WIN(n)` is `8 + n`, as the public header says. Window n is the n-th base-address register of the function (config offset 0x10 + 4n): `pcibr_slot_info_init` (0xb768) counts the registers with one index, and when a memory register is 64-bit (type bits 10) it adds a second increment to the index and a second step of 4 to the config offset before the next register (0xbb98 to 0xbbbc). The window therefore skips the upper half of a 64-bit register. The mapping from "window n" to "register n" is read in that loop and stated here as established for the index walk; the exact record fields (+0 type, +8 base, +16 size) are established from the stores at 0xbb78 to 0xbb84.

IP27 (`IP27boot/pcibr.o`, helper at 0x1004) and IP35 (`IP35boot/pcibr.o`, helper at 0x1250) have the same dispatch: zero fails, 7 and 1 are tested first, then `space - 8 < 6`. Established for the dispatch shape on both, not read line by line past it.

A driver that calls with space 9 is therefore asking for the second register's window, not the first. When the second register is the only one populated (the first register is 64-bit or absent) the table entry for window 1 is the one the call needs; a call for a window whose register reads as unimplemented fails.

## Endianness: `pciio_endian_set`

`pciio_endian_set(dev, device_end, desired_end)` calls the provider's `pcibr_endian_set` (IP30 0x3904, IP27 0x3b04, IP35 0x3d70, the same instructions; IP35 also in [ip35-pci-bridge.md](ip35-pci-bridge.md)). The two arguments are enum values, big endian = 0 and little endian = 1 (public header). The routine:

- compares the two arguments. **Equal: clears bits 19 and 20 (0x180000) of the slot's Device register; unequal: sets them.** Bit 20 is the swap bit for DMA through the address-translation entries, bit 19 the swap bit for direct-mapped DMA (public Linux bridge header names `SWAP_PMU` and `SWAP_DIR`); the names are a hint, the bit positions are read at 0x394c to 0x3968;
- writes the Device register only when the value changes, and always returns the second argument (`desired_end`), so the caller sees "achieved = desired" on every path;
- touches no other bit. It does **not** set the PIO byte-swap: PIO swapping on these bridges is chosen per mapping, see the next section.

So a driver that calls `pciio_endian_set(dev, 1, 0)` (device little endian, host wants big) gets hardware byte swapping on both DMA paths for that slot; `(dev, 1, 1)` or `(dev, 0, 0)` gets none. The call "with argument 1" is one of those, depending on which position the 1 is in. Established for the register effect on IP30, IP27 and IP35; which of those the third-party driver's call is cannot be read here (the driver binary is not in this repository's `root/`).

## PIO byte swap is per mapping

The helper above (0xf6c) reads bit 0x1000 of the call's flags (`PCIIO_BYTE_STREAM`). For the big memory and I/O windows (spaces 2, 4, 5, 6) the first mapping decides a bridge-wide mode kept in soft +2712 (I/O) or +2716 (memory) (0x12ec to 0x1304, 0x16a4): with the flag set it sets bit 23 (I/O) or 22 (memory) of the bridge Control register through `pcireg_control_bit_set`, without it it clears that bit through `pcireg_control_bit_clr` (0x16e0, 0x17fc). A later mapping that asks for the other mode fails with -1. The same flag in the small-window path (0x1580 to 0x15a8) sets bit 13 (0x2000) of the slot's Device register. Established (IP30). The consequence: with `PCIIO_BYTE_STREAM` absent (the usual case for a driver that swaps in software) the CPU sees the device's registers with no hardware swap.

## DMA map flags

The flag word of `pciio_dmamap_alloc` and `pciio_dmatrans_addr` has these bits in the objects (value, effect where read):

| Bit | Public name | Effect | Where |
|---|---|---|---|
| 0x2 | no-sleep | the map record allocation uses no-sleep | `pcibr_dmamap_alloc` 0x2114 |
| 0x10 | `PCIIO_DMA_CMD` | command stream: ATE bits `V|CO` plus barrier, no prefetch; Device register: barrier on, prefetch off | `pcibr_flags_to_ate` 0x47f0, 0x0 helper |
| 0x20 | `PCIIO_DMA_DATA` | data stream: ATE bits `V|CO|PREF` (prefetch on); Device register: prefetch on, barrier off | same |
| 0x40 | `PCIIO_DMA_A64` | 64-bit PCI addresses; without it the address must be below 4 GiB | `pcibr_dmatrans_addr` 0x2edc |
| 0x100, 0x200 | write gather on, off | Device register bits 23 and 24 | helper at .text+0 |
| 0x400, 0x800 | prefetch on, off | prefetch bit | 0x47f0 |
| 0x1000 | `PCIIO_BYTE_STREAM` | request byte swap for byte-stream data | see below |
| 0x2000 | `PCIIO_WORD_VALUES` | request byte swap for 32-bit word values from a little-endian device | see below |

How the two swap flags act depends on the bridge type, the halfword at soft +114: 0 for part 0xc002 (Bridge), 1 for part 0xd002 (XBridge), 2 for the PIC parts (`xbridge_attach` sets 1 when the manufacturer field is 36, 0x94c to 0x950; the part-to-type table for all four parts is in the sibling chips repository's Bridge routine notes). Types 0 and 1 are the IP30 hardware; type 2 is IP35 only and its branches below were read only where named:

- **Bridge (0)**, direct-map and translation-entry DMA: `0x1000` sets Device bits 19 and 20 together, `0x2000` clears them (the helper at .text+0, 0x14c to 0x198). There is no address-bit swap on this type: `pcibr_dmamap_alloc` skips the address block when the type is 0 (0x243c). The effect equals `pciio_endian_set` with unequal arguments.
- **XBridge (1) and PIC (2)**: for translation-entry maps `pcibr_dmamap_alloc` sets bit 29 (0x2000_0000) of the PCI address it hands back when `0x1000` is set or the Device register has bit 20 set, and clears it when `0x2000` is set (0x2448 to 0x2498). For direct-map DMA `0x1000` sets Device bit 19 only and `0x2000` clears it (0x154 to 0x198). For 64-bit addresses the swap attribute is bit 55 of the 64-bit PCI address (helper at 0x1f0c, bit set at 0x2078, cleared by `0x2000`).
- Established on IP30; IP27 and IP35 use the same helper shapes (inferred, not compared line by line).

Decoding **0x1020 = `PCIIO_BYTE_STREAM | PCIIO_DMA_DATA`**: 32-bit DMA (no 0x40), a data stream (prefetch on, barrier off), byte swap requested, no command bit, no word-values bit. On a Bridge this sets the same two Device bits as the endian call; on an XBridge or PIC it sets bit 29 in the returned translation-entry address and bit 19 in the Device register.

## Interrupt target `/hw/cpunum/<n>`

`pcibr_intr_alloc` (0x73d4) reads the target from the device descriptor for its interrupt-bit sharing decisions (`device_desc_intr_target_get`, only when descriptor flag 0x4 is clear, 0x74b0 to 0x74c8): the value -1 means "no preference", anything else goes through `cpuvertex_to_cpuid` (0x792c to 0x7958). The placement itself is the Heart's, below. `cpuvertex_to_cpuid` (`unix.IP30` 0xa80000002023e298) reads the vertex's CPU-id label and returns -1 when the label is absent. On IP30 the interrupt is placed by `intr_spray_heuristic` (`unix.IP30` 0xa800000020013e50), called from `do_heart_intr_alloc` (`IP30boot/heart.o` 0x1188):

- target given and its CPU enabled (`cpu_enabled`): that CPU;
- target given but that CPU is not enabled: a global default CPU (the halfword at gp-26016; its role, the boot CPU, is inferred);
- no target: CPU 1 when its count in the spray counters at gp-32668 is below CPU 0's at gp-32672, otherwise CPU 0; again the default is the fallback when the pick is not enabled, and also when that CPU's flag word has 0x80 set (what that flag means was not read). The chosen CPU's counter is incremented.

Nothing in these paths fails or panics: attach continues and the interrupt runs on a CPU that exists. On a uniprocessor Octane an unenabled CPU 1 therefore lands on the default CPU (established for the fall-through code, inferred that the default is CPU 0). Whether `/hw/cpunum/1` exists on a uniprocessor is not read here; the kernel itself builds this path with `sprintf("/hw/cpunum/%d")`, `hwgraph_path_to_dev` and `device_desc_intr_target_set` on IP27 ([interrupts.md](interrupts.md)), so a driver that does the same sees a failed lookup as an unset target. How the third-party driver treats a failed lookup is unknown (driver not held).

## The `/hw/.id/pci/<vendor><device>` entries

The connection-point registry `cdl` (`IP30boot/cdl.o`) creates them in `cdl_add_connpt_reg_locked` (0xca8). The PCI registry is created by `pciio_init` (`IP30boot/pciio.o` 0xd58) with the names "pci", "vendor" and "device" (`cdl_new` 0x590 stores them at +0, +8, +16).

Order of events for a PCI function:

1. The Bridge or XBridge widget attaches (`xbridge_attach`, `IP30boot/xbridge.o`; registered by `xbridge_init` for widget parts 0xc002 and 0xd002). Per slot it runs `pcibr_slot_info_init` (reads the function's identity, sizes the registers), `pcibr_slot_addr_space_init`, `pcibr_slot_device_init`, `pcibr_slot_guest_info_init`, and then `pcibr_slot_call_device_attach` for each slot (0x7e8).
2. That reaches `pciio_device_attach` (`pciio.o` 0x121c), which calls `cdl_add_connpt(pciio_registry, vendor, device, connection vertex, flags)` (call at 0x1250; vendor from info record +20, device from +24).
3. `cdl_add_connpt_reg_locked` first runs each driver registered for that vendor and device (`cdl_call_attach`) and counts the ones that returned success (0xd5c to 0xd94). With a count of zero it creates `/hw/unknown/pci/vendor/0x<vendor>/device/0x<device>/instance` with an edge to the connection vertex (0xfec to 0x1028, 0x1134 to 0x116c). Both outcomes rejoin at 0xdcc, where the vertex is added to the entry's list and, when the entry has no `.id` vertex yet and neither number is -1, the name is built from eight uppercase hex digits, vendor then device (digits taken from the table "0123456789ABCDEF" at rodata 0xd8; 0xf3c to 0xfc8) and `/hw/.id/pci/<VVVVDDDD>` is created with `hwgraph_path_add` (the parents `.id` and `pci` are made on first use, 0x1174 to 0x11b0), then an edge to the connection vertex named by the vertex handle in decimal is added (0xe50 to 0xe80). For 0x1263 and 0x05DC that is `/hw/.id/pci/126305DC`.

Correction, from the multi-function scan note: an earlier version of this page said the `.id` entry needs at least one successful driver attach and that a zero count creates the `unknown` path "instead". The zero-count branch at 0xfec rejoins the main path at 0xdcc and nothing after that reads the count, so the `.id` entry is made for every function whose vendor and device are both real, and a function with no successful attach gets the `unknown` path as well. See [pcibr-multifunction-scan.md](pcibr-multifunction-scan.md#3-does-a-driver-registered-for-126305dc-attach-to-function-1).

The consequences: the `.id` entry does not depend on a driver. What a driver registered for the exact pair (`pciio_driver_register` to `cdl_add_driver`) changes is whether its attach runs and whether the `unknown` entry is made; a driver registered after the slot scan is attached to the existing connection vertices by `cdl_add_driver` (call to `cdl_call_attach` at 0x868). The wildcard registrations (-1 for vendor or device) are attached by the three recursive calls at 0xeb4 to 0xee4 and do not make a name themselves (the name needs both numbers). `cdl_call_attach` (0x2bc) can defer the attach to a thread (`io_init_async`); a deferred attach returns 0 at once and so counts as a success (see the multi-function scan note).

Open: the second argument of attach for the IP27 and IP35 builds of `cdl.o` was not compared; the routine shape is the same name and size class, so the same behaviour is inferred there.
