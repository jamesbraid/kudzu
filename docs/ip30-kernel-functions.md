# IP30 kernel: one entry per routine

Each entry reads one routine of the linked IRIX 6.5.30 IP30 kernel (`6.5.30/unix.IP30`, the Octane kernel) from its disassembly and its fact sheet. Offsets written `+N` are byte offsets from the first argument unless the entry says another base. Entries closing with **Established.** read the whole body; **Inferred.** says the entry rests on a partial reading, and the entry names what it did not trace.

## Interrupt-mask and assertion support: `heart.o` and the kernel core

- `heart_imr_bits_rmw` (`6.5.30/unix.IP30`, occupies 288 bytes at address 0xa80000002003e868): changes selected bits of
one of four 64-bit interrupt-mask registers and returns the register's previous value, without letting two processors
interleave the read-modify-write. The first argument (a0) is reduced to its low two bits and picks the register at
`0xffffffffaff10000` plus eight times that index; the second argument (a1) holds the bits to clear and the third (a2)
the bits to set, so the new value is a2 OR (old AND NOT a1). A table of four ownership words, `heart_imr_own`, is
indexed the same way (the address formed at 0xa80000002003e88c). The routine loads the halfword at absolute address
0xffffffffffffc3a0, the executing processor's identity, and compares it with the ownership word. When they are equal the
caller already owns that register, so it reads, combines and stores the register at once (the store sits in the delay
slot of the `flushbus` call at 0xa80000002003e944) and returns the old value. Otherwise it raises the interrupt level
with `spl7`, then calls `compare_and_swap_int64` with the ownership word's address, -1 and the identity as arguments; a
zero result sends it to the retry loop at 0xa80000002003e954, which restores the level with `splx`, calls `spl7` again
and repeats the swap, so interrupts stay open between attempts. Once the swap succeeds it does the same read, combine
and store (the store is again in the delay slot of `flushbus`, at 0xa80000002003e8f0), writes -1 back to the ownership
word to release it and calls `splx` with the level `spl7` returned. Both exits leave in v0 the register value read
before the change, saved at sp+40 in each path. No branch-likely instruction or error call is present. **Established.**
- `assfail` (`6.5.30/unix.IP30`, occupies 1084 bytes at address 0xa80000002001bc94): front end of the kernel's
assertion-failure path; the first failing processor snapshots its register state into a fixed save area, then every
caller falls through to a tail jump to `_assfail` at 0xa8000000201c9204 without touching the argument registers a0, a1
and a2. The save area is the static object `_assregs` at 0xa8000000204fe608. The routine first stores v0 at +8 of it,
then loads the word `panic_cpu` (0xa8000000204a8254). The single conditional branch, at 0xa80000002001bcd0, goes to the
tail jump when that word is not -1, so a processor that fails an assertion after another has claimed the panic skips
every remaining save. When the word is -1 the routine writes into it the executing processor's identity, the halfword at
absolute address 0xffffffffffffc3a0, and then stores each general register at byte offset 8 times (register number minus
1), skipping the assembler temporary and a0 to a2: v1 at +16,
a3 at +48, s0 at +120, t8 at +184, gp at +216, sp at +224,
s8 at +232 and ra at +240. It also stores the status register (coprocessor 0 register 12) at +248, the multiply-low
register at +256, multiply-high at +264, coprocessor 0 register 8 at +272, the cause register at +280 and register 14
(the exception program counter) at +288 and again at +352. The routine never returns to its caller. **Established.**
- `pcibr_error_dump` (`6.5.30/unix.IP30`, occupies 4188 bytes at address 0xa80000002004afcc): prints, for every error bit
set in the Bridge interrupt-status register, the registers that explain that bit, through `cmn_err` at level 6. Its
argument a0 is the bridge soft-state record: the halfword at +114 is the chip kind (0, 1 or
2), bit 0 of the halfword at +116 selects the PCI-X readings of some request-error bits, the word at +72 changes which
kind-2 bits print, the doubleword at +16 is a name string and the address a0+24 is a second name buffer. The routine
reads the status with `pcireg_intr_status_get`, clears the low eight bits and returns at once if nothing is left.
Otherwise it stores the masked value at the address a0+2112 with `test_and_set_int64` and returns silently when the
value that held before is the same, so one error set is dumped once. A new value logs, at level 1, the header "PCI
BRIDGE ERROR: int_status is 0x%lx for %s ... Dumping relevant %s registers for each bit set" with the status, the +16
name and the a0+24 name. It then walks bits 8 to 63. For each set bit it first prints the bit's description from a table
of string pointers (the table pointer is loaded through gp-20392, the entry for a bit is at table offset 1920 plus 8
times the bit number; the strings start with the bit number, for example "30: PMU Access Fault"), then reads registers
by bit. Kind 0: bits 8 and 10 to 15 print the PCI bus error address pair (`pcireg_pci_bus_addr_get`,
`pcireg_pci_bus_addr_addr_get`); bit 9 prints the response buffer error with device, buffer and a second
`pcireg_resp_err_get` read; bits 16 and 30 print
`pcireg_ssram_parity_get`; bits 23 to 25 and 27 print the command word with
`print_bridge_errcmd` plus two `pcireg_bus_err_get` reads; bits 26 and 28
print `pcireg_resp_err_get`, its device and buffer fields and `pcireg_resp_err_addr_get`; bit 29 prints
`pcireg_linkside_err_get` through `print_bridge_errcmd` with the label "Aux "; bits 32 to 38 and 42
print the PCI-X bus error address, attribute and data; bit 39 prints the PCI-X request error address and attribute; bit 40
prints `pcireg_arbitration_get`; bit 41 prints `pcireg_parity_err_get`; bits 43 to 45 print the PCI-X split request pair
and then the bus error triple; bits 17 to 22, 31 and 46 to 63 print only the description. When the +116 flag is set,
bits 26, 28 and 29 print the PCI-X request pair instead of the response fields. Kind 1 differs by skipping bits 8, 16
and 31 entirely (not even the description) and printing `pcireg_map_fault_get` for bit 30. Kind 2 skips the same bits,
and prints the link-side error word and its address for bits 22 and 23; bits 26 and 28
are preceded by the link-side word; when the word at +72 is nonzero it also skips bits 16 to 21 and bit 23. After the
loop it reads `pcireg_intr_multiple_get`; for kinds 1 and 2 (the kind-0 path returns at once, the branch-likely test at
0xa80000002004b204 skips the print when no such bit is set) with any bit above the low eight set it prints that register
and the description of each of its bits 8 to 63 the same way. The routine was read by following the compare chains and
checking each bit's route by stepping the branch conditions for every kind and flag value, not by tracing every register
by eye. **Inferred.**

## Bridge error handling: `pcibr.o`

- `pcibr_pioerror` (`6.5.30/unix.IP30`, occupies 3400 bytes at address 0xa80000002004cb30): decides which PCI space, slot
and offset a failed programmed-I/O access hit, writes that into the caller's error record and asks the PCI layer's error
handler what to do. Arguments: a0 is the bridge soft-state record (the word at +4 is passed on as the handler's first
argument; the halfword at +114 is the chip kind; the word at +72 chooses between two window plans; bytes +56 and +57 are
the first and last bus index), a1 is an opaque value that is only forwarded, a2 is the mode (0, 2
or other) and a3 is the error record whose doubleword at +24 is the failing crosstalk address. It logs that address with
`pcibr_debug` (mask 0x200), then classifies it. Fixed windows, with the second constant used when the word at +72 is
nonzero and chip kind 2 adds the same 0x800000 to the first pair: 0x20000 to 0x28000 (0x820000 to 0x828000) is
configuration space, code 7, with device number (address minus base) shifted right 12 and masked with 0xff and the
offset in the low 12 bits; 0x28000 up to 0x29000 (0x828000 to 0x829000) is also code 7 with no device, offset address
minus 0x28000. Addresses from 0x200000 (0xa00000) to 0xc00000 (0x1400000) use a 1 MB step index; steps 1 and 3 add
0x100000, the step minus one or two picks a 128-byte window record at +288, whose word gives the space code and
doubleword at +296 the base, or, when the word is zero, code 4 or 2 chosen by bit 12 of the doubleword at +312 and the
low 12 bits of it shifted left 20 added to the offset. For chip kind 2 two address plans exist by the word at +72
(0x40000000 to 0x7fffffff and 0x80000000 up to a stored limit, or 0xc0000000 up and 0x100000000 up to a second limit,
giving codes 5 and 6 with the base subtracted or, for 0xc0000000 and up, a stored constant added); other chips use
0x40000000 (code 5), 0x80000000 up to the first stored limit (code 6) and 0x100000000 to 0x1ffffffff (code 2, I/O). When
no device was named, a first walk goes over the buses from byte +56 to byte +57 and, per device, over six window records
(0x38 bytes each from device +0x48: space, base, size); the first window that contains the offset and has a compatible
space (equal, or both memory codes 4 to 6) gives the device, function and window number, code 8 plus the window index,
and the offset inside the window. A second walk over the same buses follows each device's resource list (head at device
+0x210, next pointer at +0x38, type word at +0xc, base at +0x10, size at +0x18) and adds 1 to the counter at +0x48 of
every record that contains the address, filling in the device if the first walk found none. The result is logged with
`pcibr_debug` (mask 0x200, "space=%d, offset=0x%lx, dev=0x%x, func=0x%x"). When the space is nonzero it writes into the
record: flag word +0 |= 0xc00000 (and |= 0x20000000 when a device was found), the halfword at +8 gets the device number
(plus 1 for chip kind 2) shifted left by 3 plus the function index, +32 the space code and +40 the offset. Mode 0 calls
`pcibr_error_cleanup` and then returns 0, or the result of `pciio_error_handler` when a device was found. Other modes
with space 0 print "XIO Bus Error ... does not correspond to any PCI address" at level 6 and return 7. Otherwise
`pciio_error_handler(a0 word +4, a1, mode, record)` runs: a result of 0 is returned unchanged; 9 prints "PIO Error on
PCI Bus %s" at level 1, then, when bit 23 of the record's flag word is set, one line naming the space (I/O, memory,
configuration or window) with slot, function and offset, and the result becomes 10; any nonzero result, when the debug
halfword at gp-26390 is nonzero, prints the raw interrupt status, calls `pcibr_error_dump` if the status has a bit of
the mask at gp-24128, and lists space, physical address, device, function and the device register. Finally, when bit 29
of the record's flag word is set it calls `pcibr_device_disable` with the slot (minus 1 for chip kind 2), and for mode 2
calls `pcibr_error_cleanup`; the handler's (or 10) result is returned. The window arithmetic was read through the
decompiler listing and checked against the instructions for constants, callees and record offsets; the nested walks were
not stepped instruction by instruction. **Inferred.**
- `pcibr_slot_info_init` (`6.5.30/unix.IP30`, occupies 2864 bytes at address 0xa80000002004e2a8): probes one PCI slot of a
Bridge, allocates bus address space for each base register of each function it finds and registers the function with the
PCI layer. Arguments: a0 is the vertex handle (passed through to `pcibr_debug`, `pciio_device_info_register` and
`hwgraph_fastinfo_get`) and a1 the slot number (255 means the wildcard slot); the bridge soft-state record comes from
`hwgraph_fastinfo_get(a0)`, and the slot's own record is at soft-state +slot*0x80 (word +0x100 is the "already
initialised" mark, +0x108 the registered handle, +0x10c a flag word, +0x110 the function count and +0x118 the function
table). Returns: 22 (EINVAL) when the vertex has no soft state or the slot is above the last bus byte at +57; 0 when the
slot's mark at +0x100 is set; 1 when `pcibr_is_slot_sys_critical` says the slot must not be probed; 19 (ENODEV) when
`pcibr_probe_slot` on the configuration address from `pcibr_slot_config_addr` fails or the vendor id half of the first
word is 0xffff; otherwise 0 after all functions are done. It reads the header-type byte (config offset 0xe with
`do_pcibr_config_get`); when bit 7 (multifunction) is set it probes functions 1 to 7 with `pcibr_func_config_addr` and
`pcibr_probe_slot`, collecting a bitmask of absent functions and the highest present function. It then allocates a table
of function-count doublewords with `kmem_zalloc`, and for each function: takes the header type (low 7 bits; 0 means six
base registers, 1 means two, anything else is a `cmn_err` at level 4), logs through `pcibr_debug` (mask 0x400),
special-cases vendor 0x10a9 devices whose class half is 2 by calling `pciio_device_win_populate` once per soft-state
flag bit (+0x878 bits 0 and 1) with the bus byte at +40 shifted left 24 and size 0x1000000, and sets the latency timer
(config offset 0xd) from the soft-state halfword at +0x74 and the maximum-latency byte when the existing value is zero
and the device register does not forbid it. When bit 0 of the +0x74 halfword is set it also writes 0x20 to the
cache-line size (offset 0xc) if zero and looks up PCI-X capability 7 with `pcibr_find_capability`, calling `cmn_err`
level 4 when the capability is missing and counting PCI-X functions at soft state +0x508 and adding a split-transaction
buffer count from a table indexed by bits 24 to 26 of the capability word into +0x50c. A device-info record from
`pcibr_device_info_new` is registered with `pciio_device_info_register`. Each base address register is probed by writing
all ones and reading it back; the lowest set bit of the masked value gives the size, memory registers carry type bits
(bit 0 of the value: I/O is code 2, memory code 4, or 5 for a 64-bit register whose upper word is nonzero), and
`pcibr_bus_addr_alloc` hands out the space; the base, size and code go into the six 0x38-byte window records at device
+0x48/+0x50/+0x58. A 64-bit register consumes two slots and zeroes its upper register. The body was read from the
decompiler listing with constants, callees and record offsets checked against the instructions; the register-sizing
arithmetic was not stepped instruction by instruction. **Inferred.**
- `ecc_handler` (`6.5.30/unix.IP30`, occupies 1776 bytes at address 0xa800000020027fd4): decodes the R10000 cache-error
register after a cache or system-interface error, repairs what can be repaired and returns a text describing an
unrecoverable error, or 0 when the error was cleared. Arguments: a0 is the saved register frame (its first doubleword is
copied to +0x3b8 and the 0x2c-iteration copy loop at 0xa800000020028204 saves the frame to +952 on the first fatal
error), a1 is the per-processor error record: the word at +24 is the cache-error value, +568 and +572 are a 16-entry
ring's head and tail, the entry for a head index sits 32 bytes apart from +56 (value), +64/+72 (the two tag reads) and
+80 (first frame doubleword), words +40, +44 and +52 count primary-instruction, primary-data and secondary errors, +0x26
holds a state byte, and +8/+16 receive the tag words read back. It first tests bit 29 of the value: set means several
errors in one unit, and it returns the string "CER= Multiple cache errors in same unit detected" after `cer_to_hex`
formats the value into it. Otherwise it appends value and frame word to the ring unless the ring is full (head plus one,
masked with 0xf, equals the tail; the slot index is then -1). The error source is bits 31 to 30 of the value: 0 is an
instruction-cache error, where for each way whose bit is set in 0x5500000 (way 0) or 0xaa00000 (way 1) it calls the
unnamed cache-operation helper at 0xa8000000200279b8 with operation 4 and the index taken from bits 13 to 3, and then
with operation 0 on both ways and `ecc_force_cache_line_invalid` for both ways, returning 0; 0x40000000 is a primary
data-cache error, counted at +44, which returns "unrecoverable dcache error, neither way" when no way bit is set in the
masks, or reads each flagged way's tag with operation 5 and returns "unrecoverable - dcache tag mod", "unrecoverable -
dcache tag" or "unrecoverable - dcache data" for dirty or modified lines, otherwise invalidates it with operation 1 (a
flag at bit 28 turns any result into "unrecoverable - dcache tag on inconsistent block"); 0x80000000 is a
secondary-cache error and always returns "unrecoverable secondary cache error"; 0xc0000000 is a system-interface error,
counted at +52: bits 25 or 28 set give "unrecoverable SIE" when bits 23 to 25 also say so, a multi-bit ECC error is
reported with the failing physical address read from the doubleword at 0xffffffffaff00098 and `maddr_to_dimm` when the
Heart status doubleword at 0xffffffffaff10040 has bit 1 set and the address bits match the value, and secondary tags are
read with operation 7 and either invalidated with operation 3 or reported as "unrecoverable SIE - dirty scache". An
unrecognised source returns "unrecoverable - unknown cache error". Afterwards it stores the tag words into the ring
slot, sets the byte at absolute address 0xffffffffffffc6e0 to 1, and when a message exists and state byte +0x26 was not
yet 2 it sets it to 2 and copies the frame. Finally it walks the 4096-byte record in 128-byte steps calling the helper
with operation 0x13. The cache-state decoding was read from the decompiler listing and checked for constants, strings
and callees against the instructions; the helper at 0xa8000000200279b8 was not read, so what its operation codes do is
inferred from the way the results are used. **Inferred.**
- `pcibr_alloc_all_rrbs` (`6.5.30/unix.IP30`, occupies 980 bytes at address 0xa800000020048b48): gives the read response
buffers of one Bridge half to the four device slots it serves, by building one 16-bit-per-field assignment word and
writing it with `pcireg_rrb_set`. Arguments: a0 is the vertex handle, a1 selects the half (only bit 0 is used), and the
eight counts come as a2, a4, a6 and the ninth argument (virtual-channel-0 buffers for slots 0 to 3 in that order) and
a3, a5, a7 and the tenth argument (the matching counts for the second channel). It finds the bridge soft state by
`hwgraph_traverse` on the name "pci" followed by `hwgraph_fastinfo_get` (the soft-state doubleword at +8 is the register
base) and `hwgraph_vertex_unref`; if either step finds nothing it falls back to `xtalk_piotrans_addr(a0, 0,
0, 8, 0)`. The part and revision are read from the word at register base +4: the pair of values (bits 12 to 27 equal to
0xc002 with bits 1 to 11 equal to 0x36, or 0xd002 with 0x24) marks a chip that takes the two-channel layout; for other
chips all of a6, a7, the ninth and tenth arguments must be zero. It returns -1 when the sum of the four first-channel
counts exceeds 8, when any of them is negative or when the chip check fails. The assignment word is built nibble by
nibble from the top, one nibble per buffer: for each slot the first-channel buffers are written with the slot number
or'd with 8 (the valid bit) plus a channel bit for the two-channel chip; the second-channel buffers use only the slot
number or 8. With the soft state's lock (`mutex_spinlock` on soft state +0x80) held, it calls `pcireg_rrb_set(registers,
half, word)`, ORs 0x55 shifted by the half index into the doubleword-sized flag word at +0x51c, zeroes the four per-slot
virtual counts at +0x528 to +0x540 (stride 8 by half), stores 8 minus the total at +0x520, records each slot's counts
and differences at +0x568 to +0x5cc with stride 16 by half, and releases with `mutex_io_spinunlock`. Returns 0.
**Established.**
- `pcibr_intr_alloc` (`6.5.30/unix.IP30`, occupies 1580 bytes at address 0xa800000020049f14): creates the interrupt-handle
record for a PCI device's interrupt lines and binds each line to a crosstalk interrupt resource, returning the record or 0.
Arguments: a0 is the device vertex, a1 an optional device descriptor, a2 a bit set naming the interrupt pins in use
(bits 0 to 3, written as a text of pin letters in the first `pcibr_debug`, mask 0x1000) and a3 the target vertex. The
device record comes from `pcibr_info_get` and the bridge soft state from its doubleword at +40. The record is 0xa0 bytes
from `kmem_zalloc` (0 returned if that fails). With no descriptor the interrupt level is `default_intr_pri` and the
target is unconstrained; otherwise `device_desc_flags_get` bit 2 (non-threaded) clears the level logic, and the
descriptor's target (`device_desc_intr_target_get` and `cpuvertex_to_cpuid`) and software level
(`device_desc_intr_swlevel_get`) are used. It fills in pin set, vertex and soft state, a 0xffff cpu field, and sets the
lock named "intr_cbuf" with `spinlock_init`. A bridge method (the function pointer at soft state +1280, called with the
device record, the pin set and the highest bus index plus 1) returns the wired interrupt-bit mask. For each of the eight
bits set it: takes the crosstalk interrupt for that bit from the soft-state array starting at +1640 (stride 8),
allocating one with `xtalk_intr_alloc_nothd` when empty and publishing it with `compare_and_swap_int64` (an interrupt
lost in the race is released with `xtalk_intr_free`; the winner has the device-bit registers rewritten with
`pcireg_intr_device_bit_clr` and `pcireg_intr_device_bit_set`); checks through `xtalk_intr_cpuid_get` that every bit
lands on one cpu, and against the descriptor's cpu when one was requested, ending in `cmn_err` level 4 (with
`vertex_to_name` for the second message) when they differ; adds 1 shifted by the bit to the record's mask; and appends a
0x20-byte list node (next, record, soft state, line byte) to the per-bit lock-free list at +1664 using
`compare_and_swap_int64`, freeing the node with `kmem_free` and logging when another thread already holds the slot. The
tail calls the helper at 0xa800000020049e9c to place the interrupt on its processor for non-threaded handles. A missing
crosstalk resource ends in `cmn_err` level 1. Returns the record. The list-publishing steps and the placement helper
were read through the decompiler listing and checked against the instructions for callees, sizes and offsets; the helper
itself was not read. **Inferred.**
- `xbridge_attach` (`6.5.30/unix.IP30`, occupies 2496 bytes at address 0xa800000020042104): attach routine of the
crosstalk-to-PCI bridge on a widget; it creates the "pci" vertex, builds the soft-state record, resets and maps the
bridge, hooks the error interrupt and attaches every slot. The argument a0 is the widget vertex. It logs "pcibr_attach"
through `pcibr_debug` (mask 4), takes the register base from `pcibr_bridge_ptr_get(a0, 0)` and reads the identification
word at base +4. When bits 12 to 27 equal 0xc002 and bits 1 to 11 equal 0x36 and bit 5 of `pcireg_stat_get` is clear, it
returns -1 without attaching. Otherwise it adds the vertex "pci" with `hwgraph_path_add`, registers
`pci_xbridge_provider` with `pciio_provider_register` and `pciio_provider_startup`, adds the character device
"controller" with `hwgraph_char_device_add`, reads the board information string with `nic_bridge_vertex_info` and
fetches platform hints with `pcibr_hints_get`; a hints record whose word at +12 is nonzero ends in a return of -1. It
then reads the full id with `pcireg_id_get`, forms the revision key (bits 28 to 31 of the id merged with bits 12 to 27
shifted left 4) and attaches it as the label `_pcibr_asic_rev`; allocates and zeroes the 0xab8-byte soft state with
`kmem_zalloc` and `memset`, stores its pointer with `hwgraph_fastinfo_set`, saves a copy of the vertex name, and fills
the fields: widget vertex at +0, vertex handle +4, register base +8, last bus byte +0x39 (7), chip kind halfword +0x72
(1 when the id word has bits 12 to 27 equal 0xd002 and bits 1 to 11 equal 0x24, else 0), the slot-count halfword at
+0x70 (0x400 for kind 1, 0x80 otherwise) and a short chip name string copied from the string table at +0x1a8 or +0x1b0
("BRIDGE" for the 0x80 case), the interrupt-bit table pointer at +0x500, and the mode and speed halfword at +0x74 (mode
from `pcireg_mode_get`, speed from `pcireg_speed_get` shifted left 1). The revision key picks the letter "A", "B", "C"
or "D" for the revision log, or "unknown". It stores the widget's id, master and master id from the `xwidget_info_*`
getters, initialises the lock at +0x80 with `spinlock_init`, applies the hints' bus mapping when present, clears per-bus
arrays and eight per-interrupt entries, and registers `pcibr_error_handler` with `xwidget_error_register`. It resets the
bridge: for key below 0xc0024 `pcireg_timeout_bit_clr` with 0x3ff, `pcireg_intr_reset_set` 0x7f,
`pcireg_intr_device_set` 0xffffffff, `pcibr_directmap_init`, `pcireg_control_bit_set` 0x200000, then zeroes every
internal address-translation entry with `pcireg_int_ate_set`. For chip kind 0 it sizes external ATE RAM with
`pcibr_init_ext_ate_ram`; it builds the translation resource maps with `rmallocmap` and `pcibr_ate_free`. It allocates
the error interrupt with `xtalk_intr_alloc` (descriptor duplicated with `device_desc_dup`, flag bit 0 set, name "Bridge
error"), connects it to `pcibr_error_intr_handler` with `xtalk_intr_connect` and `pcibr_setwidint`, enables the mask
bits in a stored constant with `pcireg_intr_enable_set`, sets interrupt mode 0 and flushes with `pcireg_tflush_get`.
Revision-dependent flag bits go into +0x7c (0x80000 below key 0xc0022; 0x800 below the prefetch-enable key; 0x20000
below the write-gather key). It creates three resource windows with `pciio_device_win_map_new` and
`pciio_device_win_populate` (0x10000000 bytes at 0x100000, 0x100000 at 0x4000,
0x40000000 at 0x100000), registers a wildcard device record, and loops over the slots calling `pcibr_slot_info_init`,
`pcibr_slot_addr_space_init`, `pcibr_slot_device_init` and `pcibr_slot_guest_info_init` in turn, then
`pcibr_initial_rrb`, `pcibr_rrb_alloc_init(soft, 1,
0, 8)` when the board string contains "030-1275-",
`pcibr_slot_call_device_attach` for each slot and finally `pciio_device_attach`. It returns 0 on success. The routine
was read through the decompiler listing and checked against the instructions for callees, constants and offsets; the
inner arithmetic of the hint-driven bus map was not stepped instruction by instruction. **Inferred.**

- `mlreset` (`6.5.30/unix.IP30`, occupies 864 bytes at address 0xa800000020012450): early machine reset hook, run once on
each processor, that zeroes the performance counters and, on the boot processor only, discovers which processors are
enabled and sets up several kernel globals. The argument a0 is nonzero on a secondary processor. On every processor it
first clears performance control registers 0 and 1 and data registers 0 and 1
with `r1nkperf_control_register_set` and `r1nkperf_data_register_set`, then reads the processor revision with
`get_cpu_irr`; when that value shifted right 8 is 14 or 15 it clears counters 2 and 3
as well. A secondary processor then calls `heart_mp_intr_init(getcpuid())`, `flush_tlb(1)` and `flush_cache` and
returns. The boot processor stores 0 in the halfword `kdebug` when it holds -1, stores `getcpuid()` in `master_procid`,
and reads the doublewords at 0xffffffffaff00000 and 0xffffffffaff00080: for processor 0 and
1, `processor_enabled[i]` becomes 0 when bit 60+i of the first doubleword is set, else 1 when bit 4+i of the second is
set, else 0. It calls three unnamed routines (at 0xa8000000200131d0, 0xa800000020013560 and 0xa80000002001234c, none
read here) and `load_nvram_tab`; sets `reboot_on_panic` to 0 when it is -1; clears bits 12 and 14
in the two words `valid_icache_reasons` and `valid_dcache_reasons` (gp-26596 and gp-26592); sets `softpowerup_ok` to 1
and `cachewrback` to 0; and derives `cachecolormask` as (primary data cache size rounded up to a multiple of 32 KB,
shifted right 15) minus 1, or 0 if that quotient is below 1. The memory-copy workaround is chosen next: when the boot
argument "heart_invalidate_war" is present, `atoi` of it becomes `_heart_invalidate_war`; when absent, the flag is set
to 1 if any enabled processor (tested with `cpu_enabled` for processors 0 and 1) has the word at 0xa800000000000604 plus
0x80 per processor below 0x927 or equal to 0x930. With the flag set it writes zero over the first word of `bcopy_pref1`,
`bcopy_pref2` and `bzero_pref`; with the flag clear it zeroes, with `blkclr`, the 0x28 bytes from
`ovbcopy` to `bcopy_nopage` and the 0x1c bytes from `blkclr` to
`bzero_nopage`. It then calls `flush_cache`, initialises the locks `ip30clocklock` ("ip30clock") and
`ip30ledlock` ("ip30led") with `spinlock_init`, sets
`maxcpus` to one more than the highest enabled processor among 1 and 0 (unchanged when none), sets
`is_octane_lx` to 1 when the word at 0x900000001f600048 is zero and in that case writes 0x100000 to the word at
0x900000001f600038 and 0x7001a to the word at 0x900000001f60002c, sets `_prom_cleared` to 1 and returns.
**Established.**

## IOC3 serial ports: the `ioc3` driver

- `ioc3_serial_attach` (`6.5.30/unix.IP30`, occupies 1212 bytes at address 0xa800000020142a5c): sets up the one or two
UART ports of an IOC3 (subdevices 4 and 5 of the PCI function) and connects their interrupts. The argument
a0 is the PCI device vertex. It returns -1 when `ioc3_subdev_enabled` reports neither subdevice 4 nor 5 enabled, and 0
otherwise (both the paths that finish, at 0xa800000020142c68 and 0xa800000020142d10, return 0). The first and last port
index are chosen from the two subdevice tests (4 enables port 0, 5 enables port 1). The register window comes from
`pciio_piotrans_addr(a0, 0,
8, 0, 0x20180,
0)`, and `ioc3_is_console` says whether this IOC3 carries the console. Two
`hwgraph_traverse` calls look up the vertices named ".guest" and "ioc3", and `hwgraph_fastinfo_get` of the second gives
a driver-wide doubleword stored in each port at +160. One zeroed 0x150-byte block (`kmem_zalloc`) holds both 0xa8-byte
port records. For each port it adds the vertex "tty/1" (port 0) or "tty/2" (port 1) with `hwgraph_path_add`, stores the
PCI vertex at +48 and the new vertex at +52 of the record, attaches the record to it with `hwgraph_fastinfo_set`, calls
`sio_initport` with mask 0x1ff and a flag that is 1 only for port 0 on the console IOC3, allocates a 0x28-byte state
block, sets its byte at +29 to 3 for the console port and 2 for the other, calls `sio_init_locks`, and sets the baud
word at +128 to 9600 (0x2580). On the console IOC3 it also finds the edge "d" below the new vertex with
`hwgraph_edge_get` (stored in a static array at 0xa8000000204aeda0 indexed by port), then lets the kernel option "dbaud"
(port 0) or "rbaud" (port 1) found with `kopt_find` and `atoi` replace the baud when positive; port 1 prints "Setting
rbaud to %d" with `printf`. `the_kdbx_port` is set to port 0 on the console IOC3 and to port 1 when an rbaud value was
given. Each port gets `device_inventory_add(vertex, 4, 0x10, -1,
0, 0)`, the register base at +56, a driver table pointer at +0 and a register address 0x20000 above the window at +80.
Port 0 (when subdevice 4 is enabled) then gets its ring pointers: a 16 KB buffer from `kvpalloc` if none exists (a null
result ends in `panic` "ioc3_uart driver cannot allocate page"), cleared with `blkclr`, with transmit and receive halves
at +0x1000 offsets and the descriptor addresses at +0x178 and +0xb8; the unnamed routine at 0xa80000002014288c
initialises the DMA setup, `hwgraph_edge_get` picks the vertex for the interrupt, and `ioc3_intr_connect(a0, 0x1ff,
handler, record, vertex, vertex, 0)` connects it. On the console IOC3 it calls `device_controller_num_set(vertex, 1)`,
`sio_make_hwgraph_nodes` and stores the record in `the_console_port`. Port 1 does the same with mask 0x3fe00 and
controller number 2, sharing the buffer page with offsets 0x2000 and 0x3000. The port-array indexing and the placement
of the buffers were read through the decompiler listing and checked against the instructions for callees, constants and
stores; the unnamed setup routine was not read. **Inferred.**
- `giobr_attach` (`6.5.30/unix.IP30`, occupies 740 bytes at address 0xa800000020040ae0): attach routine for the GIO bus
bridge widget, which builds its state, probes four GIO slots and registers the bus with the GIO layer. The argument a0
is the widget vertex. The routine first raises the word at 0x900000001800002c to 0x80 when it holds less. It maps the
bridge's 16 MB register window with `xtalk_piotrans_addr(a0, 0,
0, 0x1000000, 0)` and returns -1 when bit 5 of the word at window +0xc is set. Otherwise it adds the vertex "gio" with
`hwgraph_path_add`, reads the board nameplate with `nic_bridge_vertex_info` at window +0xb4, adds the character device
"controller" under the prefix "giobr_", and allocates and clears a 0x108-byte state record with `kmem_alloc` and
`blkclr`, attached with `hwgraph_fastinfo_set`. The state gets the vertex at +0, the window pointer at +8, the widget id
byte at +16 and the master vertex and master id from the `xwidget_info_*` getters, plus a bus-address word at +32 made
of the master id shifted left 20. Eight 16-byte per-slot entries from +0x50 are zeroed. The DMA window comes from
`physmem` shifted left 14, capped at 0x80000000, stored at +40, and `xtalk_dmatrans_addr(a0, 0,
0, size, 0)` decides the base at +48 (0, 0x20000000, or the translated address with its high bits kept) and the matching
bits in +32. The unnamed helper at 0xa800000020040958 is called with the window, the state and 1. For each of four table
entries (the table at 0xa80000002043e100 plus 0x10, stride 16, with values 0x200000, 0x600000, 0x700000 and
0x800000 as the first field) it maps the entry's register at +4 with `xtalk_piotrans_addr` and calls the unnamed probe
at 0xa800000020040760; a nonzero result is shifted left by the low five bits of the word at entry +0xc and or'd into the
state word at +0x44, and the byte at state +0x40+index is set to 1. When no slot answered it calls `cmn_err` level 7
with "unknown device on gio bridge". Then the unnamed helper at 0xa800000020041ff0 runs, `gioio_provider_register` and
`gioio_provider_startup` register the table at offset 0x7c8 of the slot table, the word at +0x48 is cleared,
`gioio_device_register(vertex, vertex, 0, state+0x44, 0)` registers the bus and
`gioio_device_attach` attaches it. Returns 0 on success. The routine was read through the decompiler listing and checked
against the instructions for callees, constants and stores; the three unnamed helpers were not read. **Inferred.**
- `ioc3_intr_connect` (`6.5.30/unix.IP30`, occupies 632 bytes at address 0xa800000020060e68): connects a handler to a set
of IOC3 interrupt bits by claiming a slot in the IOC3 driver state and starting an interrupt thread for it. Arguments:
a0 is the PCI device vertex, a1 the mask of interrupt bits to claim, a2 the handler,
a3 its argument, a4 the vertex reported in messages and stored in the slot, a5 the device vertex used for the descriptor
and driver name, a6 an extra doubleword stored in the slot. The IOC3 driver state comes from
`hwgraph_traverse(a0, "ioc3")` and `hwgraph_fastinfo_get`; when the traverse fails the routine logs
"ioc3_intr_connect(%v): ioc3_attach not yet called" through `cmn_err` level 1. The slot comes from a free-slot bit mask
at state +0xb38: the lowest set bit is cleared with a load-linked and store-conditional pair (`ll` and `sc` at
0xa800000020060ee0, retried with the branch-likely test at 0xa800000020060eec when the store fails) and its bit number
found by a halving search starting from 15 or 31; when the mask is empty the slot number is the post-incremented counter
at +0xb30. Each slot is 0x58 bytes from the state start: +0x30 gets the mask (a1), +0x38 the handler, +0x40 the
argument, +0x48 a4, +0x50 the state pointer and +0x80 the a6 value. The claimed mask is or'd into the in-use word at
+0xb34 with another `ll` and `sc` pair; an overlap with a previous owner logs "%v: trying to share ioc3 intr bits 0x%X"
at level 6 and the routine returns the value from `cmn_err`. Otherwise it duplicates the descriptor with
`device_desc_dup(a5)`, reads its software level with `device_desc_intr_swlevel_get` (the level is used when nonnegative,
else the global `default_intr_pri`), looks up the driver with `device_driver_getbydev` and forms the thread name
"ioc3[slot]" or "ioc3[slot]driver" with `sprintf` and `device_driver_name_get`. It sets 0x300 in the slot's flag word at
+0x58 and returns the result of `xthread_setup(name, level, slot+0x58, 0xa8000000200617a4, slot+0x30)`, which sets up
the interrupt thread; the fourth argument is the routine at 0xa8000000200617a4. **Established.**
