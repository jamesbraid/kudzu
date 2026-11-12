# IP35 kernel: the graphics board EEPROM label path (6.5.30)

Question. When the Odyssey driver runs on a chimera-type IP35 host, how does the kernel read the graphics board's EEPROM and build the `_nic` label? Grades: E established, I inferred, U unknown, M model choice. Source: `unix.IP35` and `IP35boot/odyssey.a(odsy_init.o)` of IRIX 6.5.30; the layout of the area is the public IPMI FRU board info area. Companion notes: chips `docs/questions/v12-nic.md`, sysco `docs/l1/ip35-graphics-eeprom-request.md`.



Source: IRIX 6.5.30 `unix.IP35` (symbols present, text addresses `0xc000000000...`) and
`IP35boot/odyssey.a(odsy_init.o)`. Grades: E established (read in the binary), I inferred, U unknown,
M model choice. Nothing here is hardware evidence for what a real V12 board holds. Disassembly was
done with capstone; addresses below are kernel text unless named.

### What the driver calls (E)

- `odsy_init.o` 0x8dc to 0x8fc: after the gate (board +4 bits 31:28 >= 2), if the platform word class is
  0x4000 or `get_chimera_type(nasid)` is nonzero, it calls
  `eeprom_vertex_info_set(0x800, nasid, vertex, 0, 0)`. Otherwise (0x960) `nic_vertex_info_set` on
  board +0x10dc. `nasid` is the node that masters the graphics widget (`xwidget_info_masterid_get`).
- A Tezro's L1 reports brick type 0x39. `set_sn12_type` (0x41550) maps 0x39 to chimera type 3, so a Tezro
  takes the EEPROM path. (E for the mapping; that a real Tezro L1 replies 0x39 is I, see
  `l1-eeprom-names.md`.)

### Q1. Which component and area (E for the kernel, U for the L1 side)

- `eeprom_vertex_info_set` (0x91fac) returns early if the vertex already
  has a `_nic` label, allocates 2048 bytes, and with flags 0x800 (no 0x200 bit) calls `eeprom_str`
  (0x9175c, which is the common routine at 0x915f8 with text mode 0). Flag nibble 0xf00 selects the
  reader: 0x100 C-brick, 0x400 I/O brick, 0x800 graphics; anything else returns 6.
- Graphics reader `gbrick_eeprom_read` (0x93cc8): it needs flag 0x800. It opens the L1 channel of the
  node (`get_elsc()` when the nasid is the local one, otherwise the node's own channel) and calls
  `read_board_ia(sc, fd, component, out)` (0x931ec). The component is a **fixed constant, not derived
  from widget or slot**: 2 on a chimera host (0x93d78 to 0x93d7c), 3 when the platform class is 0x4000
  (0x93dd8). The last non-chimera branch reads an uninitialised stack word (0x93d70); not relevant here.
- `read_board_ia` asks for **area 2** (the board area; `read_ia` 0x929b8 puts the component at message
  +4 and the area at +0x14 and reads 64-byte chunks at rising offsets until a chunk shorter than 64
  comes back). The same reader for the C-brick on a chimera host uses component 0 (0x9355c), which is the
  node board the `/node` hinv line comes from. I/O-brick reads use components 1, 2 or 4 depending on
  flag and peer case (0x93a30 to 0x93a9c); those are the I/O path, not graphics. (E for the numbers, I
  for which caller of the I/O path the iris model's refusals of 1 and 4 belong to.)
- So for the V12 the model must answer **component 2, area 2**. Its refusal of component 2 is exactly
  what stops attach. The refusals of 1 and 4 are not on the graphics path.

### Q2. Does a refusal leave `_nic` unset? (E)

- `sc_interpret_resp` (0xd4e0c) returns the reply's first word, the status, when it is nonzero. `read_ia`
  (0x92a6c to 0x92af4) turns any nonzero return or negative status into 1. `read_board_ia` returns that 1
  and `eeprom_str` passes it up (0x916f0).
- In `eeprom_vertex_info_set`, a nonzero result frees the 2 KB buffer (0x92060) and returns 0 **without adding any label** (the add is at 0x920a0,
  reached only when the read returned 0). So -104 never becomes a failure text; the label stays absent
  and `nic_vertex_info_get` yields NULL, which `odsy_set_gfx_mfg_nic_name` reports as the alert.
  This differs from the 1-Wire path, which stores "not present" text. (E)
- -104 is the L1's "EEPROM not present" (sysco `board-records.md`, the Origin 300 firmware analogue:
  an entry whose probe failed answers -104; a bad header or area checksum answers -107). (E for that
  firmware, I for the Tezro image.)

### Q3. Board area to `_nic` text (E)

Decode order, `eeprom_decode_ipmi_fields` 0x92d54 and `eeprom_decode_sgi_fields` 0x92e44, then the
formatter at 0x90af4 (called `fawlty_disk+0x90` in the symbol table; the symbol is wrong). Public layout:
Intel/DMTF IPMI Platform Management FRU Information Storage Definition, board info area. The kernel
reads it like this:

| bytes | field | goes to |
|---|---|---|
| 0, 1 | format version, length in 8-byte units | version < 2 selects the plain tail; length checked by checksum |
| 2, 3 to 5 | language, manufacture date | ignored by the label |
| next | manufacturer (type/length, data) | not in the label |
| next | product name | `Name` |
| next | serial number | `Serial` |
| next | part number | `Part` |
| next | **one byte skipped** (the FRU file ID slot) | none |
| next | SGI field 1 (type/length, data) | `Revision` |
| next | two raw bytes, then SGI field 3 (type/length, data) | read, not used by the label |
| next | for version < 2: `0xC1` ends it; anything else makes the kernel also decode crypt and net fields | |
| last | checksum: the whole area, length*8 bytes, must sum to 0 mod 256, else status 3 | |

- Type/length bytes: top two bits 0xc0 (ASCII), low six bits the length; `read_board_ia` checks
  nothing else. (E)
- Label text: `Part:<part>;Name:<name>;Serial:<serial>;Revision:<rev>;Group:ff;Capability:ffffffff;Variety:ff;Laser:<hex>;`
  (strings at the kernel string base 0xc000000000499098 + 0x178 to 0x190; group, capability and variety
  are constant text here, not record fields). (E)
- `copy_ascii_field` (0x912cc): `:` and `;` become `?`, bytes 0xff become a space, bytes below 0x20 or
  above 0x7e become `.`, and with the dash flag (set for part, serial, revision, not for name) `_`
  becomes `-`. So `ODY128VERSIONB` and `030-1611-001` pass unchanged. (E)
- `odsy_set_gfx_mfg_nic_name` takes the token after `Name:` (see above); not re-read.
- Which byte a real board has in the skipped slot, and what the two raw bytes and SGI field 3 hold:
  U. A real board's Revision may sit in a different field; the printed hinv rev C is consistent with
  SGI field 1 but that is I.

### Model choice (M, not hardware evidence)

Answer component 2, area 2 (board) on the local node's L1. Area bytes (56, version 1, length 7):

```
01 07 00 00 00 00 c3 53 47 49 ce 4f 44 59 31 32 38 56 45 52 53 49 4f 4e 42
c6 45 4d 55 30 30 31 cc 30 33 30 2d 31 36 31 31 2d 30 30 31 c0 c1 43 ff ff
c0 c1 00 00 00 01
```

Manufacturer `SGI` (placeholder), name `ODY128VERSIONB`, serial `EMU001` (placeholder), part
`030-1611-001`, skipped slot 0xc0, SGI field 1 `C`, raw bytes 0xff 0xff, empty field 3, 0xc1 end, three
pad zeros, checksum 0x01 (sum of the 56 bytes is 0 mod 256). Reply chunks as the kernel requests: at most
the 64 bytes asked, a short final chunk (the 56-byte area ends the loop). A decode of this record by an
independent port of the rules above yields part `030-1611-001`, name `ODY128VERSIONB`, rev `C`
(own script, not the kernel). Keep the name away from `ODY128B1_2`, `CHIM_ODY`, `CHIODY` unless the
DVI register path is wanted. Also still answer component 0 as today.
