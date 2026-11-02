# Number-in-a-can reads

The kernels read board identity from Dallas 1-Wire memory parts (the "number in a can", NIC): the part number, serial number and revision text, and on an Ethernet controller the MAC address. The PROM copies the manufacturing text of the node, I/O, router and graphics boards into KLCONFIG ([klconfig.md](klconfig.md#board-serial-numbers)); the kernel reads the parts itself for the vertices it labels `_nic` and for the Ethernet address. This page states how the 6.5.30 kernels drive the bus, which commands and memory layout they expect from the part, what text they build, and what each failure leaves behind.

All the routines are in one object per platform, `usr/cpu/sysgen/IPnnboot/nic.o`. The routines that object exports keep their names in the linked kernels; its static routines (`crc16`, `nic_presence`, `nic_write`, `nic_read`, `nic_write_byte`, `nic_read_byte`, `nic_next_scan`, `nic_match_rom`, `char2hex`, `chars2hex`, `chars2print`, `nic_mfg_next`, `nic_access_hub`) have no address in the linked kernels' symbol tables, so they are cited by their offset in the 6.5.30 `IP27boot/nic.o`; in `6.5.30/unix.IP27` the object starts at `0xc000000000068d0c` (its `nic_setup`, at object offset `0x3f4`, is linked at `0xc000000000069100`). Grades follow [klconfig.md](klconfig.md): "Established" means the whole routine was read.

## Bus access

The routines never touch a register themselves. A bus is a pair: an access routine and an argument for it, kept in a 96-byte search state together with the 1-Wire search progress. Every bus operation calls the access routine with the argument, a pulse width, a sample time and a recovery delay, all in microseconds, and takes bit 0 of the result as the line level sampled. The kernel passes these values:

| Operation | Pulse | Sample | Delay after | Linux `ioc3-eth.c` |
|---|---:|---:|---:|---|
| reset and presence | 520 | 65 | 500 | `mcr_pack(500, 65)`, then `mcr_pack(0, 500)` |
| read a bit | 6 | 13 | 100 | `mcr_pack(6, 13)`, then `mcr_pack(0, 100)` |
| write a 1 | 6 | 110 | 0 | `mcr_pack(6, 110)` |
| write a 0 | 80 | 30 | 0 | `mcr_pack(80, 30)` |

The kernel waits after the operation with `us_delay`; Linux instead issues a second, pulse-free operation for the recovery. A presence result of 0 means a part pulled the line low.

Two access routines exist:

- `nic_access_mcr32` (`6.5.30/unix.IP27` `0xc000000000069d24`, 136 bytes)
  drives a 32-bit microlan control register whose address is the argument (a0). It writes the pulse
  shifted left 10 ORed with the sample shifted left 2 (Linux `mcr_pack`), then reads the word back
  until bit 1 (done) is set, calls `us_delay` with the delay (a3) and returns bit 0 of the last read.
  The poll has no limit: a register that never sets bit 1 hangs the caller. This is the IOC3 `mcr`
  register as Linux drives it. Established.
- `nic_access_hub` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x17c4`, 260 bytes)
  drives the hub's own NIC. It ignores its argument and works on the calling processor's node: the
  processor number is the halfword at absolute `0xffffffffffffc420` (`p_cpuid`), its node the
  halfword at +1058 of that processor's `pda_s` (`p_nodeid`, through `pdaindr`), and the NASID the
  halfword of `compact_to_nasid_node` for that node. It writes the 64-bit value `nic_get_phase_bits`
  returns ORed with pulse shifted left 10 and sample shifted left 2 to `0x9200000001a000a8` plus the
  NASID shifted left 32, which is Linux's `MD_MLAN_CTL` (hub offset `0x2000a8`) of that node, polls
  bit 1 of the same doubleword without limit, delays and returns bit 0. Established.
- `nic_get_phase_bits` (`6.5.30/unix.IP27` `0xc000000000068d74`, 52 bytes)
  reads the doubleword at `0x9000000010000078` (the 8-byte literal at `.lit8` of the object, offset
  0), divides it by 1,000,000, halves the quotient and subtracts one, giving p, and returns p shifted
  left 20 ORed with p shifted left 27. Linux names bits 20 and 27 of `MD_MLAN_CTL` `MLAN_PHI0` and
  `MLAN_PHI1` and resets both to `0x31` (49), which is this formula for 100,000,000. So the
  doubleword is a clock rate in hertz, read from offset `0x78` of the window Linux calls
  `LBOOT_BASE` (the local boot PROM space, `0x9000000010000000`). Mechanics established; the
  meaning of the doubleword inferred from the arithmetic and the Linux reset value. A rate below
  2,000,000 gives p of -1 and sets every bit from 20 up.

## 1-Wire primitives

The search state, as the routines use it: +0 the access routine, +8 its argument, +16 the last discrepancy of the search, +20 a flag set when the search has seen every part, +24 the current bit index (1 to 64), +28 the discrepancy found in this pass, and from +32 one byte per ROM bit (bit i at +31+i).

- `nic_setup` (`6.5.30/unix.IP27` `0xc000000000069100`, 40 bytes)
  fills a search state (a0) with the access routine (a1, stored at +0x0) and argument (a2, at +0x8),
  clears +0x10 and +0x14, and calls `nic_presence` for a reset, returning its result. Established.
- `nic_presence` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x9c`, 44 bytes)
  calls the access routine with 520, 65 and 500 and returns its result. Established.
- `nic_write` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0xc8`, 68 bytes)
  writes one bit: 6 and 110 for a nonzero bit, 80 and 30 for zero, delay 0. Established.
- `nic_read` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x10c`, 44 bytes)
  reads one bit: it calls the access routine at +0x0 of the state with the argument at +0x8 and
  6, 13 and 100, and returns the sampled level. Established.
- `nic_write_byte` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x138`, 116 bytes)
  writes the eight bits of a1 least significant first through `nic_write`, stopping and returning the
  first negative result; otherwise it returns 0. Established.
- `nic_read_byte` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x1ac`, 100 bytes)
  reads eight bits through `nic_read`, least significant first, and returns the byte, or the first
  negative result. Established.
- `crc16` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x0`, 104 bytes)
  folds one byte (a0) into the 16-bit CRC halfword at a1. It is the reflected CRC-16 with polynomial
  `0xA001` that 1-Wire memory parts use, computed a byte at a time with a 16-word parity table at
  `.data` offset `0x100` of the object (0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0: the parity of
  each nibble value). The callers check a block that ends in its own inverted CRC by testing for the
  residue `0xB001`. Established.
- `nic_next_scan` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x210`, 484 bytes)
  runs one pass of the 1-Wire ROM search. It returns 1 at once when the done flag (+20) is set. It
  sends a reset: a negative result is returned, a nonzero one returns 4 (no part answered). It
  clears +28, writes Search ROM (`0xF0`) and then, for bit index 1 to 64, reads a bit and its
  complement. Both 1 returns 2 (no part on the bus). When they differ the bit is the first one read.
  When both are 0 (parts disagree) the bit is 1 if the index equals the last discrepancy (+16), 0
  and a new discrepancy (+28) if the index is above it, and otherwise the bit chosen last pass, which
  also records the index in +28 when that bit is 0. Each chosen bit is stored at +31+index and
  written back. After bit 64 it copies +28 to +16, sets the done flag when that is 0, and returns 0.
  This is the search in Linux's `nic_find`, with its state kept between calls so that each call
  finds the next part. A negative result from any bit operation is returned. Established.
- `nic_next` (`6.5.30/unix.IP27` `0xc000000000069128`, 400 bytes)
  takes a state (a0), a 6-byte serial buffer (a1), a family byte pointer (a2) and a CRC byte pointer
  (a3), each optional. It calls `nic_next_scan` and returns any nonzero result. Otherwise it packs the 64
  stored bits, least significant first, into eight ROM bytes and runs the 1-Wire CRC-8 over them
  with the 256-byte table at `.data` offset 0 of the object (address `0xc0000000014377b8` in the
  linked kernel, loaded from gp-18096 at address 0xc0000000000691f8); a nonzero CRC returns 3. It then stores ROM byte 0 (the
  family code) at a2, ROM bytes 6 down to 1 in serial bytes 0 to 5 (most significant first), and ROM
  byte 7 (the CRC) at a3, and returns 0. Established.
- `nic_match_rom` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x5ac`, 316 bytes)
  addresses one part: it sends a reset, returning a negative result and, for a nonzero one, printing
  "nic_match_rom: couldn't see presence pulse during match rom command" and returning 4; then it
  writes Match ROM (`0x55`), the family byte (a1), serial bytes 5 down to 0 (a2) and the CRC byte
  (a3), so the part sees its ROM in wire order. A negative write result is returned; otherwise 0.
  Established.

## Memory reads

The kernel reads two families of 1-Wire memory: family `0x0B` (DS2505-class, 64 pages of 32 bytes with a page redirection map) and family `0x09` (DS2502-class, no redirection, one extra byte after the command).

- `nic_read_one_page` (`6.5.30/unix.IP27` `0xc0000000000693f4`, 400 bytes)
  reads one 32-byte page. Arguments: state, family pointer, serial, CRC pointer, page number (a4),
  a 64-byte redirection map (a5) and a 32-byte buffer (a6). It follows the map from the page: an
  entry of `0xFF` means the page is not redirected, and any other entry e redirects to page `~e`
  (low eight bits). After 64 steps it prints "nic_read_one_page: loop in NIC redirection map" and
  returns 5. It calls `nic_match_rom` to address the part (returning its nonzero result), writes Read
  Memory (`0xF0`) and the address page * 32 as two bytes, low first; for family 9 it reads and drops
  one byte (the part's command CRC). It reads 32 bytes into the buffer and runs `crc16` over all 32
  from zero; any result but `0xB001` returns 3. So each page ends in an inverted CRC-16 of its first
  30 bytes. Negative results from the bus are returned; success returns 0. Established.
- `nic_read_mfg` (`6.5.30/unix.IP27` `0xc000000000069584`, 788 bytes)
  reads the two manufacturing pages: page 0 into the buffer in a4 ("page A") and page 1 into the
  buffer in a5 ("page B"). For family 9 it fills the map with `0xFF` and calls `nic_read_one_page`
  for both pages, returning the first nonzero result. For any other family it addresses the part
  with `nic_match_rom`, writes Read Status (`0xAA`) and the address `0x0100` (bytes 0, 1), and reads
  the 64 redirection bytes as eight groups of eight bytes, each followed by two CRC bytes; the CRC-16
  of the first group covers the three command bytes as well. A group whose CRC-16 residue is not
  `0xB001` prints "nic_read_mfg: invalid crc16 reading redirection map page %d" with the group number
  and returns 3. It then reads pages 0 and 1 through the map. On this path only a negative page
  result is returned: a page that fails its CRC (3), loses presence (4) or loops (5) still gives the
  result 0, with whatever bytes were read in the buffer. Established.
- `nic_eaddr` (`6.5.30/unix.IP27` `0xc000000000069ab0`, 628 bytes)
  reads an Ethernet address. Arguments: access routine, argument, and a 6-byte result buffer. A null
  buffer prints "nic_eaddr: Called with NULL eaddr!" and returns 6. It calls `nic_setup`; a negative
  result is returned, and a positive one prints "nic_eaddr: No presence pulse from NIC" and
  "nic_eaddr: (check for an empty NIC socket)" and returns 4. It then walks the parts with `nic_next`
  (returning its first nonzero result, so a bus without a suitable part ends with 1) and takes the
  first whose family is `0x04`, `0x06`, `0x08`, `0x09`, `0x0A`, `0x0C`, `0x89` or `0x91`. With that
  part still selected by the search it writes Read Memory (`0xF0`) and address 0, reads 14 bytes,
  and accepts the part when byte 0 is `0x8D`, byte 1 is `0x0A` and the CRC-16 of bytes 1 to 13 leaves
  `0xB001`; otherwise it moves to the next part. The address is bytes 11 down to 6: result byte 0 is
  byte 11. Linux reads the same 14 bytes into an array in reverse and takes the same six, and accepts
  only family `0x91`. Established.
- `nic_get_eaddr` (`6.5.30/unix.IP27` `0xc000000000069dac`, 184 bytes)
  takes an IOC3 `mcr` address (a0), an optional IOC3 `gpcr_s` address (a1) and the result buffer
  (a2). With a1 it writes `0x200000` there (Linux `GPCR_MLAN_EN`, which connects the microlan to the
  pin); without it prints "nic_get_eaddr: bypassing gpcr_s enable in ioc3". It calls `nic_eaddr` with
  `nic_access_mcr32` and the `mcr` address up to twice, stopping at the first 0, and after two
  failures prints "nic_eaddr: Failed to find readable MAC NIC, rc %d". It returns the last result.
  Linux also tries twice. Established.

## Manufacturing text

The text the kernel builds is the same `name:value;` form the PROM leaves in KLCONFIG, which is where [board_serial_number_get](klconfig.md#board-serial-numbers) finds "Serial:".

- `nic_mfg_next` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0x1158`, 1184 bytes)
  appends the text for the next part on a bus to a buffer (a1; null returns 6). It calls `nic_next`
  and dispatches on its result through the 7-entry table at `.rodata` +0x368 of the object. Result
  0 (a part found): a part of family `0x0B` or `0x09` is read through a call to `nic_read_mfg`, and
  other families are passed over while the search continues. A negative read result is returned. A
  positive one appends "NIC: data not available;". When page A byte 0 is not 1 it appends "NIC:page A
  data not available". Otherwise it appends, in order, `Part:` and page A bytes 11 to 29 followed by
  page B bytes 0 to 5 (a 25-character part number), `;Name:` and page B bytes 16 to 29, `;Serial:`
  and page A bytes 1 to 10, `;Revision:` and page B bytes 6 to 9, `;Group:` and page B byte 10 in
  hex, `;Capability:` and page B bytes 11 to 14 in hex, and `;Variety:` and page B byte 15 in hex. In
  all three cases it then appends `;Laser:`, the six serial bytes in hex (the part's 48-bit 1-Wire
  number, most significant first) and `;`, and returns 0. Result 1 (the search is over) returns 1
  and appends nothing. Results 2 to 6 and anything above write one of these texts at the end of the
  buffer and return the result: 2 "NIC:Failure reading interface;Laser:unreadable;", 3 "NIC:CRC
  failure reading interface;Laser:unreadable;", 4 "NIC:Not present on interface;Laser:unreadable;", 5
  "NIC:Mfg part with redirection loop;Laser:unreadable", 6 "NIC:Called with invalid
  parameter;Laser:unreadable", 7 and above "NIC:Can't access part;Laser:unreadable;". The texts for 5
  and 6 have no closing `;`. Established.
- `chars2print` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0xc98`, 268 bytes)
  appends a2 bytes from a1 as text: `:` and `;` become `?` and `0xFF` becomes a space, written back
  into the source buffer; bytes 33 to 127 are copied, spaces are dropped, and control bytes and bytes
  of 128 and up become `.`. When nothing was appended it appends one space. Established.
- `chars2hex` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0xc0c`, 140 bytes)
  appends a2 bytes from a1 as two lowercase hex digits each, through `char2hex`, and terminates the
  string. Established.
- `char2hex` (`6.5.30/usr/cpu/sysgen/IP27boot/nic.o` `0xb8c`, 128 bytes)
  stores the two hex digits of a byte (high nibble first) from the 16-character table at `.rodata`
  offset `0xb8` ("0123456789abcdef"). Established.
- `nic_info_get` (`6.5.30/unix.IP27` `0xc00000000006a304`, 88 bytes)
  builds the text for every part on a bus: it empties the buffer (a2), calls `nic_setup` with the
  access routine (a0) and argument (a1), returns a negative result, and otherwise calls
  `nic_mfg_next` until it returns nonzero, returning that value (1 after a clean walk). So an empty
  bus gives "NIC:Not present on interface;Laser:unreadable;". The buffer is not bounded. Established.

## Vertex labels

- `nic_vertex_info_get` (`6.5.30/unix.IP27` `0xc00000000006a35c`, 44 bytes)
  returns the string a vertex (a0) carries under the label "_nic" (`.rodata` +0x3c8 of the
  object), or 0; it calls `hwgraph_info_get_LBL` with a zeroed result word. Established.
- `nic_vertex_info_set` (`6.5.30/unix.IP27` `0xc00000000006a388`, 268 bytes)
  takes an access routine, its argument and a vertex (a2). When the vertex already has a "_nic"
  string (it calls `nic_vertex_info_get`) it returns that. Otherwise it allocates a 2048-byte buffer
  with `kern_malloc` (returning 0
  when that fails), fills it with `nic_info_get`, copies the text into an allocation of its exact
  length (keeping the 2048-byte buffer when that allocation fails) and adds it with
  `hwgraph_info_add_LBL`. When a second look finds another string, added meanwhile, it frees its own
  and returns the other. Otherwise it exports the label with `hwgraph_info_export_LBL` and the
  string's length plus one, runs `nic_vmc_check` on the vertex and the text, and returns the text.
  Established.
- `nic_vertex_info_match` (`6.5.30/unix.IP27` `0xc00000000006a494`, 60 bytes)
  calls `nic_vertex_info_get` and then `strstr`, and returns 1 when the vertex (a0) has a "_nic"
  string containing a1, else 0. Established.
- `nic_hub_vertex_info` (`6.5.30/unix.IP27` `0xc00000000006a5d4`, 36 bytes)
  labels a vertex from the hub's NIC: it calls `nic_vertex_info_set` with `nic_access_hub` (from
  `.srdata` +0x28 of the object) and argument 0.
  Established.
- `nic_bridge_vertex_info` (`6.5.30/unix.IP27` `0xc00000000006a5f8`, 32 bytes)
  labels a Bridge vertex (a0) from the microlan register at the address in a1: it calls
  `nic_vertex_info_set` with `nic_access_mcr32` (from `.srdata` +0x10 of the object). Established.
- `nic_hq4_vertex_info` (`6.5.30/unix.IP27` `0xc00000000006a618`, 32 bytes)
  labels an HQ4 vertex (a0) the same way, instruction for instruction: it calls
  `nic_vertex_info_set` with `nic_access_mcr32` and the register address in a1. Established.
- `nic_ioc3_vertex_info` (`6.5.30/unix.IP27` `0xc00000000006a638`, 48 bytes)
  takes a vertex, an IOC3 `mcr` address and an optional `gpcr_s` address; it writes `0x200000` to the
  latter when given and then calls `nic_vertex_info_set` with `nic_access_mcr32` and the `mcr`
  address, as `nic_bridge_vertex_info` does. Established.

## Callbacks on NIC text

Drivers can ask to be called when a vertex gets NIC text containing a string. The records form a list appended at the tail: in `6.5.30/unix.IP27` the head doubleword is at `0xc0000000014cb080` and the doubleword after it, at `0xc0000000014cb088`, points at the last record's link (initially at the head itself). A record is 24 bytes: +0 the next record, +8 the string, +16 the routine.

- `nic_vmc_add` (`6.5.30/unix.IP27` `0xc00000000006a668`, 88 bytes)
  calls `kern_malloc` for a 24-byte record (the result is not checked), stores a zero link, the
  string (a0) and the routine (a1), calls `swap_ptr` to swap it into the tail pointer, stores it
  through the old tail, and returns the record. Established.
- `nic_vmc_del` (`6.5.30/unix.IP27` `0xc00000000006a6c0`, 12 bytes)
  clears the routine (+0x10) and string (+0x8) of a record. The record stays on the list and is never
  freed.
  Established.
- `nic_vmc_check` (`6.5.30/unix.IP27` `0xc00000000006a6cc`, 160 bytes)
  walks the list from the head (address `0xc0000000014cb080`, loaded from gp-28912) and, for each
  record with both a routine (+0x10) and a string (+0x8), calls `strstr` and calls the routine with
  the vertex (a0) when the string is in the text (a1). Established.

## Other builds

The IP30 object has no `nic_get_phase_bits`, `nic_access_hub` or `nic_hub_vertex_info` (the IP30 build has no hub to read), so its offsets differ (the table gives them). Two IP30 routines differ from IP27: `nic_mfg_next` reads only family `0x0B` parts and passes over family `0x09` (its tests at object offsets `0x11a4` and `0x11b0` branch back to the search on anything but 11, then on 9), and `nic_vertex_info_match` returns the same values with a different delay-slot arrangement. The IP35 `nic_access_hub` reads `p_cpuid` at `0xffffffffffffc428` and `p_nodeid` at +1066 of `pda_s`, shifts the NASID left 33, and writes the register at `0x9200000001e04030` plus that, and the IP35 object's clock literal is `0x9000000018000078`; the rest of its object has the IP27 code hashes. Every 6.5.22 copy has the code hash of the 6.5.30 copy of the same platform.

| Routine | Binary | Address | Bytes | Note | Grade |
|---|---|---|---:|---|---|
| `nic_setup` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x3f4` | 40 | stores the access routine and argument at +0x0 and +0x8, clears +0x10 and +0x14, calls `nic_presence` | Established |
| `nic_next` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x41c` | 400 | calls `nic_next_scan`, packs 64 bits from +0x20, CRC-8 must be 0 or it returns 3 | Established |
| `nic_read_one_page` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x6e8` | 400 | follows the map, calls `nic_match_rom`, reads 32 bytes, residue `0xB001` or 3 | Established |
| `nic_read_mfg` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x878` | 788 | reads the redirection map from status address `0x0100`, then calls `nic_read_one_page` for pages 0 and 1 | Established |
| `nic_eaddr` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0xda4` | 628 | calls `nic_next` until a MAC part answers with bytes `0x8D`, `0x0A`; address from bytes 11 to 6 | Established |
| `nic_access_mcr32` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x1018` | 136 | writes pulse << 10 and sample << 2, polls bit 1, calls `us_delay`, returns bit 0 | Established |
| `nic_get_eaddr` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x10a0` | 184 | writes `0x200000` to `gpcr_s`, calls `nic_eaddr` at most twice | Established |
| `nic_info_get` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x15f8` | 88 | calls `nic_setup`, then `nic_mfg_next` until it returns nonzero | Established |
| `nic_vertex_info_get` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x1650` | 44 | calls `hwgraph_info_get_LBL` for the "_nic" label (`.rodata` +0x3c8) and returns the string found there, or zero when the vertex has none | Established |
| `nic_vertex_info_set` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x167c` | 268 | 2048-byte `kern_malloc` buffer, calls `nic_info_get` and `hwgraph_info_add_LBL`, exports the label | Established |
| `nic_vertex_info_match` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x1788` | 60 | calls `nic_vertex_info_get` and `strstr`; returns 1 when the vertex label text contains the given string, otherwise zero | Established |
| `nic_hub_vertex_info` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x18c8` | 36 | labels the vertex from the local hub part: calls `nic_vertex_info_set` with `nic_access_hub` (`.srdata` +0x28) and a zero argument | Established |
| `nic_bridge_vertex_info` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x18ec` | 32 | calls `nic_vertex_info_set` with `nic_access_mcr32` (`.srdata` +0x10) and the register address | Established |
| `nic_hq4_vertex_info` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x190c` | 32 | labels an HQ4 vertex through its microlan register: calls `nic_vertex_info_set` with `nic_access_mcr32` (`.srdata` +0x10), the same instructions as the bridge routine | Established |
| `nic_ioc3_vertex_info` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x192c` | 48 | optional store of `0x200000` to `gpcr_s`, then calls `nic_vertex_info_set` with `nic_access_mcr32` | Established |
| `nic_vmc_add` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x195c` | 88 | 24-byte record from `kern_malloc`, appended through the tail pointer with `swap_ptr` | Established |
| `nic_vmc_del` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x19b4` | 12 | zeroes +0x8 (string) and +0x10 (routine) of a record and leaves it linked | Established |
| `nic_vmc_check` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x19c0` | 160 | walks the list, calls each routine at +0x10 whose string at +0x8 `strstr` finds | Established |
| `nic_get_phase_bits` | `6.5.30/usr/cpu/sysgen/IP27boot/nic.o` | `0x68` | 52 | divides the `.lit8` +0x0 doubleword by 1,000,000, halves, subtracts 1, places it at bits 20 and 27 | Established |
| `nic_mfg_next` | `6.5.30/usr/cpu/sysgen/IP30boot/nic.o` | `0x1124` | 1188 | IP30 copy: branches back to the search unless the family is `0x0B` (tests at +0x80 and +0x8c), so family 9 parts get no text | Established |
| `nic_vertex_info_match` | `6.5.30/usr/cpu/sysgen/IP30boot/nic.o` | `0x1758` | 60 | IP30 copy: calls `nic_vertex_info_get` and `strstr`; same results as IP27 with the result register moved in a delay slot | Established |
| `nic_bridge_vertex_info` | `6.5.30/usr/cpu/sysgen/IP30boot/nic.o` | `0x1794` | 32 | calls `nic_vertex_info_set` with the `.srdata` +0x10 word, `nic_access_mcr32` (the load is gp-30704 because the object's gp0 is 0x7800), as a0 (loaded in the call's delay slot), the register address still in a1 and the vertex moved to a2; returns the callee's result. Same eight words and relocations as the IP27 copy | Established |
| `nic_hq4_vertex_info` | `6.5.30/usr/cpu/sysgen/IP30boot/nic.o` | `0x17b4` | 32 | the same eight words and relocations as the bridge routine in this object, so the same call and the same gp-30704 load | Established |
| `nic_access_hub` | `6.5.30/usr/cpu/sysgen/IP35boot/nic.o` | `0x17c4` | 260 | IP35 copy: `p_cpuid` at absolute `0xffffffffffffc428`, `p_nodeid` at +0x42a of `pda_s`, NASID shifted left 33, register at `0x9200000001e04030` | Established |
| `nic_bridge_vertex_info` | `6.5.30/usr/cpu/sysgen/IP35boot/nic.o` | `0x18ec` | 32 | calls `nic_vertex_info_set` with the `.srdata` +0x10 word, `nic_access_mcr32` (the load is gp-30704 because the object's gp0 is 0x7800), as a0 (loaded in the call's delay slot), the register address still in a1 and the vertex moved to a2; returns the callee's result. Same eight words and relocations as the IP27 copy | Established |
| `nic_hq4_vertex_info` | `6.5.30/usr/cpu/sysgen/IP35boot/nic.o` | `0x190c` | 32 | the same eight words and relocations as the bridge routine in this object, so the same call and the same gp-30704 load | Established |

The linked IP30 and IP35 kernels carry these routines at the addresses below (from their symbol tables; the static routines have none). These rows are located by symbol only; their linked instructions were not compared with the objects.

| Routine | `6.5.30/unix.IP30` | `6.5.30/unix.IP35` |
|---|---|---|
| `nic_get_phase_bits` | none | `0xc000000000093fa8` |
| `nic_setup` | `0xa800000020039218` | `0xc000000000094334` |
| `nic_next` | `0xa800000020039240` | `0xc00000000009435c` |
| `nic_read_one_page` | `0xa80000002003950c` | `0xc000000000094628` |
| `nic_read_mfg` | `0xa80000002003969c` | `0xc0000000000947b8` |
| `nic_eaddr` | `0xa800000020039bc8` | `0xc000000000094ce4` |
| `nic_access_mcr32` | `0xa800000020039e3c` | `0xc000000000094f58` |
| `nic_get_eaddr` | `0xa800000020039ec4` | `0xc000000000094fe0` |
| `nic_info_get` | `0xa80000002003a420` | `0xc000000000095538` |
| `nic_vertex_info_get` | `0xa80000002003a478` | `0xc000000000095590` |
| `nic_vertex_info_set` | `0xa80000002003a4a4` | `0xc0000000000955bc` |
| `nic_vertex_info_match` | `0xa80000002003a5b0` | `0xc0000000000956c8` |
| `nic_hub_vertex_info` | none | `0xc000000000095808` |
| `nic_bridge_vertex_info` | `0xa80000002003a5ec` | `0xc00000000009582c` |
| `nic_hq4_vertex_info` | `0xa80000002003a60c` | `0xc00000000009584c` |
| `nic_ioc3_vertex_info` | `0xa80000002003a62c` | `0xc00000000009586c` |
| `nic_vmc_add` | `0xa80000002003a65c` | `0xc00000000009589c` |
| `nic_vmc_del` | `0xa80000002003a6b4` | `0xc0000000000958f4` |
| `nic_vmc_check` | `0xa80000002003a6c0` | `0xc000000000095900` |

### The same routines in the 6.5.22 builds

The 6.5.22 copies of the routines above. Each row is a routine whose bytes match a 6.5.30 routine on the same page; the note gives the SHA-256 prefix of the bytes (first 12 hex digits). A linked-kernel row is established when the whole body has no calls, address constants or gp accesses; the others are inferred, because the 6.5.22 target of each constant or call was not checked. An object row compares section bytes, not relocated code, so it is inferred unless its note says the relocations were compared too; then it is established, as the rows for the bridge and HQ4 routines are. The comparison is in [version-diffs.md](version-diffs.md).

| Routine | Binary | Address | Size | Note | Label |
|---|---|---|---|---|---|
| `nic_vmc_del` | `6.5.22/unix.IP27` | `0xc00000000006c008` | 12 | clears the routine (+0x10) and string (+0x8) of a record; same bytes as the 6.5.30 linked kernel (SHA-256 prefix ff9cc7fdb4a7), at a different address; no calls, address constants or gp accesses in the body | Established |
| `crc16` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x0` | 104 | folds one byte (a0) into the 16-bit CRC halfword at a1; same bytes as the 6.5.30 object (SHA-256 prefix 62a772086389) at the same offset; relocations not compared | Inferred |
| `nic_presence` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x9c` | 44 | calls the access routine with 520, 65 and 500 and returns its result; same bytes as the 6.5.30 object (SHA-256 prefix 6d6833ceca2f) at the same offset; relocations not compared | Inferred |
| `nic_write` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0xc8` | 68 | writes one bit: 6 and 110 for a nonzero bit, 80 and 30 for zero, delay 0; same bytes as the 6.5.30 object (SHA-256 prefix a6203f5b06f2) at the same offset; relocations not compared | Inferred |
| `nic_read` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x10c` | 44 | reads one bit: it calls the access routine at +0x0 of the state with the argument at +0x8 and 6, 13 and 100, and returns the sampled level; same bytes as the 6.5.30 object (SHA-256 prefix d48e6d01cac3) at the same offset; relocations not compared | Inferred |
| `nic_write_byte` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x138` | 116 | writes the eight bits of a1 least significant first through `nic_write`, stopping and returning the first negative result; otherwise it returns 0; same bytes as the 6.5.30 object (SHA-256 prefix a97ac3cdf271) at the same offset; relocations not compared | Inferred |
| `nic_read_byte` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x1ac` | 100 | reads eight bits through `nic_read`, least significant first, and returns the byte, or the first negative result; same bytes as the 6.5.30 object (SHA-256 prefix 47b4afd80f80) at the same offset; relocations not compared | Inferred |
| `nic_next_scan` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x210` | 484 | runs one pass of the 1-Wire ROM search; same bytes as the 6.5.30 object (SHA-256 prefix 71d15f651f53) at the same offset; relocations not compared | Inferred |
| `nic_setup` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x3f4` | 40 | stores the access routine and argument at +0x0 and +0x8, clears +0x10 and +0x14, calls `nic_presence`; same bytes as the 6.5.30 object (SHA-256 prefix 81a781f1088a) at the same offset; relocations not compared | Inferred |
| `nic_next` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x41c` | 400 | calls `nic_next_scan`, packs 64 bits from +0x20, CRC-8 must be 0 or it returns 3; same bytes as the 6.5.30 object (SHA-256 prefix db8237bd1100) at the same offset; relocations not compared | Inferred |
| `nic_match_rom` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x5ac` | 316 | addresses one part: it sends a reset, returning a negative result and, for a nonzero one, printing "nic_match_rom: couldn't see presence pulse during match rom command" and returning 4; then it writes Match ROM (`0x55`), the family byte (a1), serial bytes 5 down to 0 (a2) and the CRC byte (a3), so the part sees its ROM in wire order; same bytes as the 6.5.30 object (SHA-256 prefix 85e55a0d8b3a) at the same offset; relocations not compared | Inferred |
| `nic_read_one_page` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x6e8` | 400 | follows the map, calls `nic_match_rom`, reads 32 bytes, residue `0xB001` or 3; same bytes as the 6.5.30 object (SHA-256 prefix bf51fb9cd3d0) at the same offset; relocations not compared | Inferred |
| `nic_read_mfg` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x878` | 788 | reads the redirection map from status address `0x0100`, then calls `nic_read_one_page` for pages 0 and 1; same bytes as the 6.5.30 object (SHA-256 prefix e3ebaec03993) at the same offset; relocations not compared | Inferred |
| `char2hex` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0xb8c` | 128 | stores the two hex digits of a byte (high nibble first) from the 16-character table at `.rodata` offset `0xb8` ("0123456789abcdef"); same bytes as the 6.5.30 object (SHA-256 prefix 7bb5e4334828) at the same offset; relocations not compared | Inferred |
| `chars2hex` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0xc0c` | 140 | appends a2 bytes from a1 as two lowercase hex digits each, through `char2hex`, and terminates the string; same bytes as the 6.5.30 object (SHA-256 prefix 2fae8aedf6a3) at the same offset; relocations not compared | Inferred |
| `chars2print` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0xc98` | 268 | appends a2 bytes from a1 as text: `:` and `;` become `?` and `0xFF` becomes a space, written back into the source buffer; bytes 33 to 127 are copied, spaces are dropped, and control bytes and bytes of 128 and up become `.`. When nothing was appended it appends one space; same bytes as the 6.5.30 object (SHA-256 prefix e75d037be9f4) at the same offset; relocations not compared | Inferred |
| `nic_eaddr` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0xda4` | 628 | calls `nic_next` until a MAC part answers with bytes `0x8D`, `0x0A`; address from bytes 11 to 6; same bytes as the 6.5.30 object (SHA-256 prefix 7c661884830a) at the same offset; relocations not compared | Inferred |
| `nic_access_mcr32` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x1018` | 136 | writes pulse << 10 and sample << 2, polls bit 1, calls `us_delay`, returns bit 0; same bytes as the 6.5.30 object (SHA-256 prefix e51d91ee4c4f) at the same offset; relocations not compared | Inferred |
| `nic_get_eaddr` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x10a0` | 184 | writes `0x200000` to `gpcr_s`, calls `nic_eaddr` at most twice; same bytes as the 6.5.30 object (SHA-256 prefix 7befa8c40a9c) at the same offset; relocations not compared | Inferred |
| `nic_mfg_next` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x1158` | 1184 | appends the text for the next part on a bus to a buffer (a1; null returns 6); same bytes as the 6.5.30 object (SHA-256 prefix df4ad335765d) at the same offset; relocations not compared | Inferred |
| `nic_info_get` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x15f8` | 88 | calls `nic_setup`, then `nic_mfg_next` until it returns nonzero; same bytes as the 6.5.30 object (SHA-256 prefix 95a643a444b0) at the same offset; relocations not compared | Inferred |
| `nic_vertex_info_get` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x1650` | 44 | calls `hwgraph_info_get_LBL` for the "_nic" label (`.rodata` +0x3c8) and returns the string found there, or zero when the vertex has none; same bytes as the 6.5.30 object (SHA-256 prefix fc1dcc1547d4) at the same offset; relocations not compared | Inferred |
| `nic_vertex_info_set` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x167c` | 268 | 2048-byte `kern_malloc` buffer, calls `nic_info_get` and `hwgraph_info_add_LBL`, exports the label; same bytes as the 6.5.30 object (SHA-256 prefix 7e4b29ee3213) at the same offset; relocations not compared | Inferred |
| `nic_vertex_info_match` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x1788` | 60 | calls `nic_vertex_info_get` and `strstr`; returns 1 when the vertex label text contains the given string, otherwise zero; same bytes as the 6.5.30 object (SHA-256 prefix c036e62fa314) at the same offset; relocations not compared | Inferred |
| `nic_access_hub` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x17c4` | 260 | drives the hub's own NIC. It ignores its argument and works on the calling processor's node: the processor number is the halfword at absolute `0xffffffffffffc420` (`p_cpuid`), its node the halfword at +1058 of that processor's `pda_s` (`p_nodeid`, through `pdaindr`), and the NASID the halfword of `compact_to_nasid_node` for that node; same bytes as the 6.5.30 object (SHA-256 prefix 2b864caeda03) at the same offset; relocations not compared | Inferred |
| `nic_hub_vertex_info` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x18c8` | 36 | labels the vertex from the local hub part: calls `nic_vertex_info_set` with `nic_access_hub` (`.srdata` +0x28) and a zero argument; same bytes as the 6.5.30 object (SHA-256 prefix 3e0abafb70a1) at the same offset; relocations not compared | Inferred |
| `nic_bridge_vertex_info` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x18ec` | 32 | calls `nic_vertex_info_set` with `nic_access_mcr32` (`.srdata` +0x10) and the register address; same bytes as the 6.5.30 object (SHA-256 prefix a862019d0b09) at the same offset, with the same relocations (R_MIPS_26 to `nic_vertex_info_set`, R_MIPS_GPREL16 to `.srdata`) | Established |
| `nic_hq4_vertex_info` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x190c` | 32 | the same eight words and relocations as the bridge routine in this object, so the same call and the same gp-30704 load; same bytes as the 6.5.30 object (SHA-256 prefix a862019d0b09) at the same offset, with the same relocations | Established |
| `nic_ioc3_vertex_info` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x192c` | 48 | optional store of `0x200000` to `gpcr_s`, then calls `nic_vertex_info_set` with `nic_access_mcr32`; same bytes as the 6.5.30 object (SHA-256 prefix e199daca4083) at the same offset; relocations not compared | Inferred |
| `nic_vmc_add` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x195c` | 88 | 24-byte record from `kern_malloc`, appended through the tail pointer with `swap_ptr`; same bytes as the 6.5.30 object (SHA-256 prefix 31114cb60a87) at the same offset; relocations not compared | Inferred |
| `nic_vmc_del` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x19b4` | 12 | zeroes +0x8 (string) and +0x10 (routine) of a record and leaves it linked; same bytes as the 6.5.30 object (SHA-256 prefix ff9cc7fdb4a7) at the same offset; relocations not compared | Inferred |
| `nic_vmc_check` | `6.5.22/usr/cpu/sysgen/IP27boot/nic.o` | `0x19c0` | 160 | walks the list, calls each routine at +0x10 whose string at +0x8 `strstr` finds; same bytes as the 6.5.30 object (SHA-256 prefix 81fdec6a1784) at the same offset; relocations not compared | Inferred |
| `crc16` | `6.5.22/usr/cpu/sysgen/IP30boot/nic.o` | `0x0` | 104 | folds one byte (a0) into the 16-bit CRC halfword at a1; same bytes as the 6.5.30 object (SHA-256 prefix 62a772086389) at the same offset; relocations not compared | Inferred |
| `nic_mfg_next` | `6.5.22/usr/cpu/sysgen/IP30boot/nic.o` | `0x1124` | 1188 | IP30 copy: branches back to the search unless the family is `0x0B` (tests at +0x80 and +0x8c), so family 9 parts get no text; same bytes as the 6.5.30 object (SHA-256 prefix 8800eb0eb1be) at the same offset; relocations not compared | Inferred |
| `nic_vertex_info_match` | `6.5.22/usr/cpu/sysgen/IP30boot/nic.o` | `0x1758` | 60 | IP30 copy: calls `nic_vertex_info_get` and `strstr`; same results as IP27 with the result register moved in a delay slot; same bytes as the 6.5.30 object (SHA-256 prefix 2e9718e25267) at the same offset; relocations not compared | Inferred |
| `nic_bridge_vertex_info` | `6.5.22/usr/cpu/sysgen/IP30boot/nic.o` | `0x1794` | 32 | calls `nic_vertex_info_set` with the `.srdata` +0x10 word, `nic_access_mcr32` (the load is gp-30704 because the object's gp0 is 0x7800), as a0 (loaded in the call's delay slot), the register address still in a1 and the vertex moved to a2; same bytes as the 6.5.30 object (SHA-256 prefix a862019d0b09) at the same offset, with the same relocations | Established |
| `nic_hq4_vertex_info` | `6.5.22/usr/cpu/sysgen/IP30boot/nic.o` | `0x17b4` | 32 | the same eight words and relocations as the bridge routine in this object, so the same call and the same gp-30704 load; same bytes as the 6.5.30 object (SHA-256 prefix a862019d0b09) at the same offset, with the same relocations | Established |
| `crc16` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x0` | 104 | folds one byte (a0) into the 16-bit CRC halfword at a1; same bytes as the 6.5.30 object (SHA-256 prefix 62a772086389) at the same offset; relocations not compared | Inferred |
| `nic_presence` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x9c` | 44 | calls the access routine with 520, 65 and 500 and returns its result; same bytes as the 6.5.30 object (SHA-256 prefix 6d6833ceca2f) at the same offset; relocations not compared | Inferred |
| `nic_write` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0xc8` | 68 | writes one bit: 6 and 110 for a nonzero bit, 80 and 30 for zero, delay 0; same bytes as the 6.5.30 object (SHA-256 prefix a6203f5b06f2) at the same offset; relocations not compared | Inferred |
| `nic_read` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x10c` | 44 | reads one bit: it calls the access routine at +0x0 of the state with the argument at +0x8 and 6, 13 and 100, and returns the sampled level; same bytes as the 6.5.30 object (SHA-256 prefix d48e6d01cac3) at the same offset; relocations not compared | Inferred |
| `nic_write_byte` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x138` | 116 | writes the eight bits of a1 least significant first through `nic_write`, stopping and returning the first negative result; otherwise it returns 0; same bytes as the 6.5.30 object (SHA-256 prefix a97ac3cdf271) at the same offset; relocations not compared | Inferred |
| `nic_read_byte` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x1ac` | 100 | reads eight bits through `nic_read`, least significant first, and returns the byte, or the first negative result; same bytes as the 6.5.30 object (SHA-256 prefix 47b4afd80f80) at the same offset; relocations not compared | Inferred |
| `nic_next_scan` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x210` | 484 | runs one pass of the 1-Wire ROM search; same bytes as the 6.5.30 object (SHA-256 prefix 71d15f651f53) at the same offset; relocations not compared | Inferred |
| `nic_match_rom` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x5ac` | 316 | addresses one part: it sends a reset, returning a negative result and, for a nonzero one, printing "nic_match_rom: couldn't see presence pulse during match rom command" and returning 4; then it writes Match ROM (`0x55`), the family byte (a1), serial bytes 5 down to 0 (a2) and the CRC byte (a3), so the part sees its ROM in wire order; same bytes as the 6.5.30 object (SHA-256 prefix 85e55a0d8b3a) at the same offset; relocations not compared | Inferred |
| `char2hex` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0xb8c` | 128 | stores the two hex digits of a byte (high nibble first) from the 16-character table at `.rodata` offset `0xb8` ("0123456789abcdef"); same bytes as the 6.5.30 object (SHA-256 prefix 7bb5e4334828) at the same offset; relocations not compared | Inferred |
| `chars2hex` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0xc0c` | 140 | appends a2 bytes from a1 as two lowercase hex digits each, through `char2hex`, and terminates the string; same bytes as the 6.5.30 object (SHA-256 prefix 2fae8aedf6a3) at the same offset; relocations not compared | Inferred |
| `chars2print` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0xc98` | 268 | appends a2 bytes from a1 as text: `:` and `;` become `?` and `0xFF` becomes a space, written back into the source buffer; bytes 33 to 127 are copied, spaces are dropped, and control bytes and bytes of 128 and up become `.`. When nothing was appended it appends one space; same bytes as the 6.5.30 object (SHA-256 prefix e75d037be9f4) at the same offset; relocations not compared | Inferred |
| `nic_bridge_vertex_info` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x18ec` | 32 | calls `nic_vertex_info_set` with the `.srdata` +0x10 word, `nic_access_mcr32` (the load is gp-30704 because the object's gp0 is 0x7800), as a0 (loaded in the call's delay slot), the register address still in a1 and the vertex moved to a2; same bytes as the 6.5.30 object (SHA-256 prefix a862019d0b09) at the same offset, with the same relocations | Established |
| `nic_hq4_vertex_info` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x190c` | 32 | the same eight words and relocations as the bridge routine in this object, so the same call and the same gp-30704 load; same bytes as the 6.5.30 object (SHA-256 prefix a862019d0b09) at the same offset, with the same relocations | Established |
| `nic_mfg_next` | `6.5.22/usr/cpu/sysgen/IP35boot/nic.o` | `0x1158` | 1184 | appends the text for the next part on a bus to a buffer (a1; null returns 6); same bytes as the 6.5.30 object (SHA-256 prefix df4ad335765d) at the same offset; relocations not compared | Inferred |
