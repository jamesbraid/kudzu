# Generic PCI layer routines of the IP35 kernel

Routines of the machine-independent PCI layer (`pciio`) in the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, a linked 64-bit image with DWARF); the same code is in `usr/cpu/sysgen/IP35boot/pciio.o`. These routines dispatch through the provider table that a bridge driver registered, so they carry no register knowledge of their own.

- `pciio_pio_addr` (`6.5.30/unix.IP35`, address 0xc0000000000c065c, 300 bytes, 80-byte frame): returns a kernel
  address through which a driver can reach a range of a device's PCI space, using a direct translation when the bridge
  offers one and a mapping object when it does not. The seven arguments are the device vertex (`a0`), a device
  descriptor (`a1`), the space number (`a2`), the PCI address (`a3`), the byte count
  (`a4`), a pointer to a map-object slot (`a5`) that may be null, and the flags (`a6`). It first saves the map object
  the caller left in `*a5` and clears `*a5` (when `a5` is not null). It then calls
  `pciio_piotrans_addr(a0, a1, a2, a3, a4, a6)` and returns its result when that is nonzero (address
  0xc0000000000c06cc). When it is zero it uses the saved map object, or, if there was none, allocates one with
  `pciio_piomap_alloc(a0, a1, a2, a3, a4, a4, a6)` (the byte count is passed twice, as both the size and the largest
  size) and returns 0 if that fails (the path through 0xc0000000000c0780 returns the zero stored at 0(sp)). It maps
  the range with `pciio_piomap_addr(map, a3, a4)`. When that gives zero and the map object was newly allocated here,
  it releases it with `pciio_piomap_free` and returns 0; when it gives an address, it stores the map object in `*a5`
  (when `a5` is not null) and returns the address. A map object passed in by the caller is never freed here, and `*a5`
  is left zero when the mapping fails. Established.
- `pciio_piotrans_addr` (`6.5.30/unix.IP35`, occupies 28 bytes at address 0xc0000000000c0640): the generic PCI entry point for translating a programmed-I/O address; it calls `pcibr_piotrans_addr` and nothing else.
  A 16-byte frame holds only `ra`. The arguments reach `pcibr_piotrans_addr` untouched and its result comes back as this routine's. The only branch is the call, whose delay slot is a `nop`; no instruction is branch-likely. Established.
- `pciio_piomap_addr` (`6.5.30/unix.IP35`, occupies 40 bytes at address 0xc0000000000c05fc): maps a range through a PCI map object and records the resulting address in the object.
  `a0` is the map object, kept in `s8`. The other arguments go unchanged to `pcibr_piomap_addr`. After the call it stores the result as the doubleword at +32 of the map object (a null result is stored too) and returns it in `v0`. The frame is 16 bytes. No instruction is branch-likely. Established.
