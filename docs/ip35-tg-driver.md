# IP35: what the gigabit Ethernet driver (`tg`) expects of its controller at attach and open

Scope. IRIX 6.5.30 `usr/cpu/sysgen/IP35boot/if_tg.o` (relocatable, n64, symbols present), read routine by routine with
the host-side fact tool. Offsets are offsets in that object. The 6.5.22 object was not read. Register names are the
public Linux `tg3` names for the offsets the code reaches. Grades: established (routine read in full), inferred
(reasoned reading), unknown (with the read that would settle it).

## Which controllers it accepts

- Established: `if_tgattach` (0x9110, 1480 bytes) reads four configuration halfwords through `tg_pci_cfg_rd`
  (offsets 2, 0, 0x2e, 0x2c, in the driver's big-endian view, where the halfword at +2 is the vendor id and the one at
  +0 the device id) and walks the five rows of the table `tg_devids` (`.data` +0x170, 16 bytes a row, loop at 0x91bc
  to 0x91f4). A row is vendor, device, subsystem vendor, subsystem device, then eight bytes of flags that were not
  decoded. The rows:

  | vendor | device | subsystem vendor | subsystem device | flag halfwords (undecoded) |
  |---|---|---|---|---|
  | 0x14e4 | 0x1645 | 0x10a9 | 0x8010 | 0x0001 0x0100 0 0 |
  | 0x14e4 | 0x1645 | 0x10a9 | 0x8011 | 0x0001 0x0100 0 0 |
  | 0x14e4 | 0x1645 | 0x10a9 | 0x8012 | 0x0100 0 0 0 |
  | 0x14e4 | 0x1648 | 0x10a9 | 0x8013 | 0x0001 0 0 0 |
  | 0x14e4 | 0x16a8 | 0x10a9 | 0x8014 | 0x0100 0 0 0 |

  No row matches means no interface (the loop falls to 0x9494). The subsystem pair is therefore mandatory: a controller
  with Broadcom's own ids and no SGI subsystem pair is refused. Unknown: which of the three 0x1645 rows a Tezro's
  controller carries; a configuration dump of a real board's function 0 offsets 0x2c to 0x2f would settle it.

## What attach does

- Established: maps a 64 KiB window (`pciio_pio_addr` with size 0x10000, 0x9288), reads configuration dwords 0x68
  and 0x70 (0x92bc, 0x92d4) into the record (the high half of 0x68 is kept as a halfword at record +0x38), allocates
  interrupt line A (line B only when `pciio_info_function_get` returns 1, `movn` at 0x9348), allocates a DMA map, resets
  the card with `tg_reset_card` (0x93a0), then `tg_alloctg`, `device_inventory_add` and finally `tg_read_vpd`
  (0x9484, after `tg_reset_info`).
- Established: for the device ids 5701 (0x1645) and 3 (the halfword at record +0x22) the byte at record +0x3f is set to
  0 without reading BAR + 0x7014 (0x95e8, 0x95ec). For every other id the driver reads the word at BAR + 0x7014
  (NVRAM_CFG1) and keeps its low byte there (0x93dc, 0x93e0). So a 5701 never consults NVRAM_CFG1; no strapping read
  through that register matters for it. Inferred: bit 0 of that byte selects the flash or NVRAM interface in
  `tg_read_eeprom` over the serial-EEPROM registers (see below).
- Established: for ids 5704 (0x1648) and 5800 (0x16a8) the DMA flag word at record +0x7c depends on bit 2 of the
  configuration dword at 0x70 (PCI state) and on the function number (0x95b0 to 0x95e4). For the 5701 the flag word is
  the constant 0x2000000 (0x9380) and the PCI state dword is read but not used in attach.

## Reset

- Established: `tg_reset_card` (0x9f08, 1592 bytes) writes configuration dword 0x68 (0xbb), reads configuration byte
  0xf, halfwords 0x2e and 0x40 and byte 0xe, sets bit 1 of the saved 0x68 value and writes it, reads and writes the
  halfword at 0x6, writes 0x40 back, waits (delays of 100, 400 and 1000 microseconds appear), then restores bytes
  0xf and 0xe and halfword 0x2e. The 0x2e restore (subsystem id) is the only place a subsystem register is written;
  inferred: a reset can clear it. Inferred: the halfword at 0x40 is the PCI-X command register of the public layout.
  Unknown: whether the 0x40 save and restore is conditional; no gating test was found in the calls but the routine's
  branches were not all followed.
- Established: the constant 0x4b657654 appears in the reset path's trace calls (0xa3e8). Inferred: it is the public
  firmware-mailbox magic, and the driver waits for the on-chip bootcode to leave that value (or its complement) in
  on-chip SRAM. The routine ends in three `cmn_err` calls (0xa4fc, 0xa518, 0xa530) that print the controller's id;
  which condition reaches each, and whether attach then fails, was not followed. A capture of a real attach failure
  would settle it.

## Vital product data (every attach)

- Established: `tg_read_vpd` (0x0, 756 bytes) reads 256 bytes of VPD through the controller's PCI VPD capability, using
  the alias of configuration space at the start of the BAR: it stores an address (0 to 252 in steps of 4) as a halfword
  at BAR + 0x50, polls bit 15 of that halfword with a 100 microsecond delay and at most 1000 tries (0x98 to 0xb8), then
  reads the dword at BAR + 0x54. It then parses a resource whose tag has low seven bits 0x10 (the public read-only data tag 0x90) for the keywords PN, EC, SN, MN and RV
  (compared with `strcmp` at 0x200 to 0x250), copying fields of up to 15, 29 and 15 bytes (0x27c to 0x2ac). A poll that
  runs out its tries moves on to the next address; nothing aborts the attach.
- Inferred arithmetic, not measured: with a controller whose bit 15 never sets, the 64 addresses cost up to
  64 x 1000 x 100 microseconds, about 6.4 seconds of delay, at every attach.
- Unknown: the VPD contents of a real board. A dump of the 256 bytes would settle it.

## Station address and EEPROM

- Established: `tg_get_macaddr` (0xc674, 244 bytes), called from `tg_open` (0x990c), copies 8 bytes from BAR + 0x410
  (the first address-filter entry, high then low) and uses the last 6. The driver does not read an EEPROM for the
  station address. Inferred: the controller's on-chip bootcode loads the address from its configuration memory before the
  driver reads the register.
- Established: `tg_read_eeprom` (0xe024) has one caller, the ioctl handler `tg_ioctl` (0x56f0). It drives the serial
  EEPROM registers (BAR + 0x6838 address, 0x683c data) when bit 0 of the byte at record +0x3f is clear, and the NVRAM
  registers (0x7000 command with 0x198, 0x700c address, 0x7010 data, arbitration at 0x7020) when set. It is not on the
  attach or open path.
