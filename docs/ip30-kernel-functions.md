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
- `pcibr_reset` (`6.5.30/unix.IP30`, occupies 700 bytes at address 0xa800000020046188): pulses the PCI reset line of the
slot that holds one device, saving and restoring the configuration state of every function on that slot around the
pulse. The argument a0 is the device vertex, also used in the debug message. The slot comes from the byte at +540 of the
record `pciio_hostinfo_get` returns, replaced by the byte at +260 of that slot's 0x80-byte record when the word at +256
is nonzero; the bridge soft state is `pciio_info_mfast_get` of that record, with the register base at its doubleword +8.
A slot outside the range from the byte at +49 to the byte at +51 logs "pcibr_reset unimplemented for slot %d" through
`pcibr_debug` (mask 8) and returns -1. Otherwise it takes the soft-state lock at +128 with `mutex_spinlock` and reads
the command register of every present function (the function table has the count at slot +272 and the pointer at +280;
config offset 4, 4 bytes, `pcibr_func_config_get`) into a stack array. Next it clears and then sets the bits `1 <<
slot` shifted left 24 of the Bridge control register with `pcireg_control_bit_clr` and
`pcireg_control_bit_set`, calling `nano_delay` after each step on a time record of 0 seconds and 1000000 nanoseconds, so
the line is held low for 1 ms and given 1 ms to recover. Afterwards, for each present function and each of the six base
registers (config offsets 0x10 to 0x24, step 4), it writes back the saved window base from the function's record (the
doubleword at +0x50 of the 0x38-byte window entry, skipped when zero) with `pcibr_func_config_set`; a second pass writes
the saved command registers back at offset 4. It releases the lock with `mutex_io_spinunlock` and returns 0. The
instruction at 0xa8000000200463c0 is never reached. **Established.**
- `intr` (`6.5.30/unix.IP30`, occupies 696 bytes at address 0xa800000020013b98): top-level hardware-interrupt dispatcher;
it repeatedly picks the highest-priority pending interrupt, runs its handler at that interrupt's priority level, and
returns 1 when a handler whose level word equals 0xe001 ran, else 0. Arguments: a0 is the exception frame, passed
unchanged to each handler; a2 is the interrupt-enable mask and a3 the pending cause value, both with the interrupt bits
in 0xff00. At entry, when the word at absolute address 0xffffffffffffc048 has both bits 0x30 set, it calls
`check_delay_tlbflush(0)` and remembers that; when the doubleword at 0xffffffffffffc280 has bit 0x20 set it logs
`log_tstamp_event(0x66, 0,
0, 0, 0)`. The dispatch table pointer is loaded from gp-21992 (the table at 0xa800000020420630). Each pass of the loop
first services two counted software events: when the word at 0xffffffffffffc560 is nonzero and a2 bit 0x1000 is set, or
the word at 0xffffffffffffc564 is nonzero and a2 bit 0x2000 is set, it raises the level with `spl65`, and for each such
event not yet taken sets bit 0x10000 (first) or 0x20000 (second) in its pending set and decrements the matching counter;
if neither was taken it restores the level with `splx`. The pending set is those bits or'd with a2 and a3 and 0xff00;
when it is zero the loop ends. Otherwise the table index is a byte read from the table at +0x118 plus the set shifted
right 13 when that is nonzero, else from +0x138 plus the set shifted right 8, and the entry is 0x18 bytes from the table
start: the handler at +0x10, its level at +0x18, the mask of the cause bits it covers at +0x1c and a software-event bit
at +0x20. The routine calls `splx` with the level, then the handler with a0 (a register call). When the level equals
0xe001 the return value becomes 1. When the entry's software-event bit is zero it increments, with a load-linked and
store-conditional pair starting at 0xa800000020013d18 (retried through the branch-likely test at 0xa800000020013d28),
the word at offset 0xd0 of the structure whose pointer is at 0xffffffffffffc488; otherwise it clears that bit from the
pending set. It then reads `getcause` and ANDs it with the entry's mask to form the next pass's cause. After the loop it
calls `ip30_intr_maint`, calls `check_delay_tlbflush(1)` when the first test passed and bit 0x10 of the word at
0xffffffffffffc048 is set, and logs `log_tstamp_event(0x65, 0x66,
0, 0, 0)` when the tracing bit is set. **Established.**
- `pcibr_error_intr_handler` (`6.5.30/unix.IP30`, occupies 1380 bytes at address 0xa80000002004c214): interrupt handler
for the Bridge's own error interrupt; it counts each error bit, throttles or disables floods, panics on the fatal ones
and acknowledges the rest. The argument a0 is the bridge soft-state record (word +0 is logged with `pcibr_debug`; the
byte at +40 is the widget number; the halfword at +114 is the chip kind; the doubleword at +16 is the name used in
messages). It reads the status with `pcireg_intr_status_get` and logs "pcibr_error_intr_handler: int_status=0x%lx" at
mask 0x100. For widget 15 with bit 0x80 set it clears that enable bit with `pcireg_intr_enable_bit_clr` and calls
`powerfail(0)`. When any bit above the low eight is set the main path runs: if the status contains any bit of the mask
0x4400008800 (a stored constant at 0xa8000000204a89b0) it first calls the unnamed routine at 0xa80000002004c0ac, which
was not read. It then walks bits 8 to 63 that are set in the status masked with 0xffffffff7fffffff, keeping for each bit
a 12-byte counter record at soft state +0x8dc (count, tick of the last check, count at the last check): the count is
incremented, and with the tick counter `lbolt` the number of errors per tick since the last check is computed by integer
division. A rate above 100 per tick (with more than 100 ticks elapsed) logs "Error rate %d/tick" at level 5; bit 18
(0x40000) on chip kinds other than 0 and 1 has its own limit, and on chip kind 0 or 1
it logs "Excessive error interrupts : %d/tick" at level 4 and raises a stored high-water mark when more than 100 errors
were seen in the interval. For chip kind 2, when the soft state's word at +0x78 masked with 0xf selects the bit pattern 2
and a bit other than 40 has been seen more than 10000 times, it logs "Disabling error interrupt type %s. Error count %d"
at level 5. When bit 24 (an unsupported-command report) is set, `pcireg_bus_err_get` has the signature 0xc00000 in its
upper bits and `pcireg_cmdword_err_get` masked with 0xf07f00 equals 0x402000, bit 24 is dropped from the working set.
The dump mask `bridge_errors_to_dump` is widened by the mask at 0xa8000000204a89b0 when `pcibr_pioerr_dump` is set and
narrowed by the mask 0xffffffbbffff77ff when not; a working set that intersects it logs "BRIDGE ERR_STATUS 0x%lx" at
level 6, as does chip kind 2 with the word +0x78 pattern 6 and bits 0x300000. A working set that intersects the fatal
mask 0x22133fc06000 calls `machine_error_dump` and `panic("PCI Bridge Error interrupt killed the system")`. Otherwise
the status is acknowledged: for chip kinds 0 and 1 with `pcibr_errintr_group` of the status written by
`pcireg_intr_reset_set`, for kind 2 with the status or 0x40; kind 2 with bit 40 set and the same pattern also sets
0xf00000 in the arbitration register with `pcireg_arbitration_bit_set`. It ends by storing 0 with `test_and_set_int64`
at the address soft state +0x840, which resets the one-dump-per-error-set guard that `pcibr_error_dump` uses. The
flood-rate arithmetic was read through the decompiler listing and the constants and callees checked against the
instructions; the unnamed routine at 0xa80000002004c0ac was not read. **Inferred.**
- `pciio_pio_addr` (`6.5.30/unix.IP30`, occupies 300 bytes at address 0xa80000002005f23c): returns a processor address for
a programmed-I/O window of a PCI device, using the direct translation when one exists and otherwise a mapping object
that the caller may keep. Arguments: a0 and a1 the device and its window selector, a2 the space,
a3 and a4 the base and size, a5 a pointer to the caller's mapping slot (may be null) and
a6 flags, passed on as the last argument of each call. If a5 is non-null the routine takes the mapping saved there and
writes 0 to the slot. It then calls `pciio_piotrans_addr(a0, a1,
a2, a3, a4,
a6)`; a nonzero result is returned at once and the saved mapping, if any, is dropped without being freed. When it
returns 0 the routine uses the saved mapping, or else allocates one with `pciio_piomap_alloc(a0, a1,
a2, a3, a4, a4,
a6)` (a zero result returns 0). It then calls `pciio_piomap_addr(map,
a3, a4)`. If that returns 0 and the mapping was newly allocated it is released with
`pciio_piomap_free` (a saved one is kept); in every case the address from `pciio_piomap_addr` is returned. When it is
nonzero and a5 is non-null, the mapping is stored back through a5 for reuse. **Established.**
- `heart_startup` (`6.5.30/unix.IP30`, occupies 488 bytes at address 0xa80000002003ee44): brings up the Heart I/O switch
at boot by creating its hardware-graph vertices, registering the crosstalk provider and registering every widget that
reports a link. It takes no arguments. It first stores a value into the word at 0x900000001800002c: 40000 (0x9c40) when
the halfword `kdebug` is zero and 0x80 otherwise. It reads the Heart status doubleword at 0xffffffffaff00080, whose low
four bits are the master widget number, and builds the vertices "node" (`hwgraph_path_add` on the argument-less root)
with the crosstalk provider table `heart_provider` registered by `xtalk_provider_register` and `xtalk_provider_startup`,
then "xtalk" below it with `xswitch_id_set(vertex, 0)`, then "0" below that, recorded in `the_error_vhdl`, joined back
to the node by the edge "io" through `hwgraph_edge_add` and given the node as its master with `device_master_set`.
Widget 0 (the Heart itself) is registered with `xwidget_register` using the part number (bits 12 to 27) and manufacturer
number (bits 1 to 11) of the identification word at 0x9000000010000004. For widget numbers 15 down to 0 it asks
`xswitch_info_link_ok` whether the link is up and, if so, creates a vertex named with the decimal number via `sprintf`
and `hwgraph_path_add`, records it with `xswitch_info_vhdl_set`, reads the widget's identification word at
0x9000000010000000 plus widget number times 0x1000000, plus 4, and registers it with `xwidget_register` under the same
master widget number. If `xswitch_info_get` finds no switch information the loop is skipped. The routine returns
nothing. **Established.**
- `ioc3_attach` (`6.5.30/unix.IP30`, occupies 1196 bytes at address 0xa8000000200609bc): attach routine of the IOC3 I/O
controller; it maps the chip's PCI configuration and memory windows, builds the driver state, programs a few chip
registers, connects the chip interrupt and starts the serial and keyboard attach routines. The argument a0 is the PCI
device vertex. It maps configuration space with `pciio_pio_addr(a0, 0,
7, 0, 0x48, slot, 0)`; a failure logs
"%v/ioc3: unable to get PIO mapping for my CFG space" through `cmn_err` level 1, and a second map of the memory space
(space 8, size 0x20180) failing logs the MEM-space message after freeing the first mapping with `pciio_piomap_free`;
both of those return -1. It looks up the ".host" and ".guest" vertices with `hwgraph_traverse` (falling back to a0 for
either), adds the vertex "ioc3" with `hwgraph_path_add` and links it under both with `hwgraph_edge_add`. A 0xb48-byte
state block comes from `kmem_zalloc`, with a lock named "is_ir_lock" at +0xb3c (`spinlock_init`) and the vertices at +0,
+4 and +8, the CFG window at +16 and the MEM window at +24; it is attached with `hwgraph_fastinfo_set`, saved in a
static when the windows match `console_cfg` and `console_mem`, and registered with `pciio_error_register` for both
vertices using the handler at 0xa800000020061468. It ORs 0x8146 into the CFG window's word at +4 (the command register),
writes 0x7ae5e to the MEM window's word at +0x28, 0x2800 to the CFG word at +0xc,
0xc0 to the MEM word at +0x34 and 0xffffffff to +0x1c, and clears the interrupt bits with `ioc3_write_ireg(state, -1,
1)`. The vertex name from `vertex_to_name` decides the board kind: a name that contains "Ibrick/xtalk/15/pci/4" sets the
byte at +0xb40 and enables subdevices with `ioc3_subdev_enables(a0, 0x91)`; names containing "baseio" or "menet" skip
the nameplate read; otherwise `nic_ioc3_vertex_info` reads it. The unnamed helper at 0xa8000000200619e4 decides whether
the chip uses interrupts; when it returns 0 the routine stores a zero interrupt handle, saves the processor identity
(the halfword at absolute address 0xffffffffffffc3a0) at +40 and returns 0. Otherwise it copies the device descriptor
with `device_desc_dup`, sets flag bit 2, software level 0 and the name "IOC3 UART", makes it the default with
`device_desc_default_set`, allocates the interrupt with `pciio_intr_alloc` (pin set 2 when `ioc3_subdev_enabled` says
subdevice 0 is on, else decided by the nameplate strings "030-1155-", "030-1657-" and "030-1664-"; failure logs "%v:
ioc3_attach: unable to allocate interrupt" at level 0) and connects it with `pciio_intr_connect` to `ioc3_intr`. The
processor chosen (`pciio_intr_cpu_get` and `cpuvertex_to_cpuid`) is stored at +40 and the requested one at +42; a
mismatch logs "Request to target IOC3 interrupts to CPU %d could not be satisfied" at level 4. Finally it calls
`ioc3_serial_attach` and `pckm_attach`, sets `console_device_attached` to 1 when this state is the console one, and
returns 0. **Inferred.**
- `pcibr_priority_bits_set` (`6.5.30/unix.IP30`, occupies 348 bytes at address 0xa80000002004652c): turns on or off, with
a reference count, the priority bits of one slot's Bridge device register. Arguments: a0 is the bridge soft state, a1
the slot and a2 the request (1 or 2 raise, 0 lowers, any other value changes nothing). The bits are 0x200000, replaced
by 0 when the revision word at +0x78 equals 0xc0022, and 0x4000 is added to them unless the request is 2. With the
soft-state lock at +128 held (`mutex_spinlock`), it reads the slot's shadow of the device register from the doubleword
at soft state + slot*0x80 + 0x138 and the use count from the word at +0x140 of the same record. A raise adds one to the
count; only when the count becomes 1 does it or the bits into the shadow, and when the bits are 0 in that case it
returns -1. A lower with a count above zero subtracts one and, when the count reaches 0 and the bits are nonzero, clears
the bits in the shadow; a lower with the count already at 0 or less returns -1 and changes nothing. All other results
are 0. If the shadow changed it writes the new value with `pcireg_device_set(soft, slot, value)`, stores it back into
the shadow and flushes with `pcireg_tflush_get`; the lock is released with `mutex_io_spinunlock` and the result
returned. **Established.**
- `pcibr_device_flags_set` (`6.5.30/unix.IP30`, occupies 512 bytes at address 0xa8000000200466d4): changes the
device-register options of the slot that holds the calling device, from a request word of paired set and clear flags,
and returns 1. The argument a1 is the request (a0 is the vertex, used only in the debug message). The slot is the byte
at +540 of the record from `pciio_hostinfo_get`, and the bridge soft state is `pciio_info_mfast_get` of that record. Two
masks are built from the request bits: bit 16 sets 0x1000000 and 0x800000, bit 17 clears both, bits 18 and 19
set and clear 0x40000, bits 20 and 21 set and clear 0x20000, bit 22 sets and bit 23 clears 0x8000, bits 26 and 27
set and clear 0x400000; for chip kind 2 (halfword +0x72) a request with bit 26 set also removes 0x400000 from the set
mask. When both masks are zero nothing else happens. Otherwise, with the lock at +128 held, the new value is the set
mask or the old shadow (the doubleword at slot record +0x138, slot records 0x80 bytes apart) with the clear mask
removed; if it differs it is written with `pcireg_device_set`, stored back and flushed with `pcireg_tflush_get`. The
lock is released and the change is logged through `pcibr_debug` (mask 0x4000000, "pcibr_device_flags_set: Device(%d):
%R") whether or not the value changed. **Established.**
- `pcibr_piomap_alloc` (`6.5.30/unix.IP30`, occupies 584 bytes at address 0xa80000002004437c): allocates a programmed-I/O
mapping record for a PCI device window, bound to a crosstalk mapping, and returns it or 0. Arguments: a0 is the device
vertex, a1 unused, a2 the window space, a3 the offset,
a4 the base address and a5 the requested size, a6 the flags. A zero size or zero base (a4 or a5 when the maximum size is
also zero) logs "pcibr_piomap_alloc: req_size | req_size_max < 1" at mask 0x8000000 and returns 0. The slot byte is read
from the device record (+540) and the soft state from `pciio_info_mfast_get`; the unnamed helper at 0xa800000020043aac
turns slot, space, offset, base and flags into a crosstalk address, and a result of -1 logs "pcibr_piomap_alloc:
xio_addr == XIO_NOWHERE" and returns 0. Otherwise it scans the free list that starts at device record +0x210 (next link
at +0x38) for a record whose doubleword at +0x18 is zero, reusing it, or allocates a new 0x60-byte record with
`kmem_zalloc` under the soft-state lock at +128 and links it at the head of the list. It fills in the vertex, the slot
(plus 1 for chip kind 2 when the slot is not 255), the soft state, the space, the base and size and the flags, and for
chip kind 2 clears three more fields. Then it calls `xtalk_piomap_alloc(master vertex, 0, crosstalk address, base, size,
flags & 0x1f)`: a zero result releases the record's size field and returns 0, otherwise the crosstalk map is stored at
+0x30 and the address at +0x28 of the record, which is returned. The helper was not read, so its translation is inferred
from how its result is used. **Inferred.**
- `pcibr_intr_free` (`6.5.30/unix.IP30`, occupies 504 bytes at address 0xa80000002004a584): releases an interrupt handle
made by `pcibr_intr_alloc`, detaching it from every bit's handler list and from the crosstalk interrupt when it was the
last user. The argument a0 is the handle: the pin-bit set is the word at +0x58, the soft-state pointer the doubleword at
+0x60, the thread record the doubleword at +0x40 and the flag word at +0. For each of the eight bits in the set it walks
the bit's handler list (head at soft state +0x680 plus the bit's slot, next link at offset 0) and clears, with
`compare_and_swap_int64`, each node that points at this handle, logging "pcibr_intr_free: cleared hdlr from bit 0x%x" at
mask 0x1000 for any node that was not clear; when the bit's use count (+0x68c) is zero and its crosstalk interrupt is
set it calls `xtalk_intr_free` on it, zeroes the pointer and clears the bit's three device bits with
`pcireg_intr_device_bit_clr(soft, 7 shifted by 3 times the bit)`. When flag bit 1 of the word at +0 is clear (the handle
has an interrupt thread) it waits up to 1000 ms in 10 ms steps (`nano_delay` of 10000000 ns) for bit 0x400 of the word
at +0x28 to be set, ending in `cmn_err` level 4 "Unable to terminate pcibr_intrd thread" if it never is. It then points
the thread at `xthread_exit` with `xthread_set_func`, wakes it with `vsema` on the semaphore at +0x30 and logs the wait.
Finally it frees the 0xa0-byte handle with `kmem_free` and returns that call's result. **Inferred.**
- `heart_addr_phys_to_xio` (`6.5.30/unix.IP30`, occupies 288 bytes at address 0xa80000002003f0cc): translates a physical
address seen by a device into the crosstalk address form that the Heart provider uses, or -1 when no widget window
covers it. The argument a0 is the address and the result is returned in v0. An address below 0x4000 first has 0x20000000
added; every address that is then 0x20000000 or above is returned unchanged (the branch at 0xa80000002003f0d8). A value
from 0x10000000 up to 0x20000000 is a widget window: the widget number is (address minus 0x10000000) shifted right 24
and masked with 0xff, the result is the address masked with 0xffffff, with the widget number shifted left 60 or'd in,
except that widget 8 is returned as the masked address alone. Everything from 0x4000 up to 0x10000000 returns -1. The
code after the 0x10000000 test also compares against 0x800000000, 0x1000000000 and 0x10000000000 and forms larger-window
results (widget number from address shifted right 31 or 36, with the same widget-8 exception), but every route to it
holds a value below 0x10000000, which returns -1 first, so those instructions never run for an address that reaches
them. **Established.**
- `pcibr_rrb_flush` (`6.5.30/unix.IP30`, occupies 380 bytes at address 0xa800000020048458): flushes every read response
buffer that is assigned to the slot of the calling device. It takes no arguments of its own: the slot is the byte at
+540 of the record from `pciio_hostinfo_get`, the bridge soft state is `pciio_info_mfast_get` of it and the register
base is the soft state's doubleword +8. The width of a buffer's owner field depends on the chip: the identification word
at register +4 with bits 12 to 27 equal to 0xc002 and bits 1 to 11 equal to 0x36, or 0xd002 with 0x24, selects 2-bit
slot numbers, anything else 1-bit. It reads the buffer assignment word for the slot's half (slot bit 0) with
`pcireg_rrb_get` and, with the lock at +128 held (`mutex_spinlock`), looks at its eight nibbles in turn; a nibble whose
enable bit (8) and owner field equal the slot number shifted right 1 triggers `do_pcibr_rrb_flush(registers, half + 2
times the nibble index)`. It releases the lock with `mutex_io_spinunlock` and returns nothing. **Established.**
- `pcibr_intr_disconnect` (`6.5.30/unix.IP30`, occupies 476 bytes at address 0xa80000002004a9c8): detaches an interrupt
handle from the Bridge's interrupt bits and from their crosstalk connections, keeping the connections that other handles
still share. The argument a0 is the handle (+0x58 holds its bit set, +0x60 the soft state, +4 the vertex for messages).
It zeroes the four words at +0x18 to +0x24 and clears bit 0 of the flag word at +0. From the bit set it removes every
bit whose shared count (the word at soft state +0x68c plus 56 times the bit number) is nonzero; if bits remain it
disables them with `pcireg_intr_enable_bit_clr` under the lock at +128 (`mutex_spinlock` and `mutex_io_spinunlock`),
flushes with `pcireg_tflush_get` and logs "pcibr_intr_disconnect: disabled int_bits=0x%x" at mask 0x1000. For each
remaining bit whose shared count is still zero it calls `xtalk_intr_disconnect` on the bit's crosstalk interrupt (the
doubleword at soft state +0x668 plus 56 times the bit), clears the word at +0x690 and logs "disconnect int_bits". If the
shared count has become nonzero meanwhile, it sets the connection word at +0x698 to the bit number or'd with the
soft-state word +0x48 shifted left 3 and reconnects with `xtalk_intr_connect(intr, pcibr_intr_func, ...,
0xa80000002004a77c, ..., 0)`, logging "now-sharing int_bits". It returns nothing meaningful. **Inferred.**
- `heart_dmamap_list` (`6.5.30/unix.IP30`, occupies 364 bytes at address 0xa80000002003f4b4): rewrites every physical
address in a DMA address list into the form the Heart provider uses, either into a copy or in place, and returns the
resulting list or 0 on failure. Arguments: a0 is not read, a1 is the source list and a2 the flags (bit 1 passes an
allocation hint on, bit 2 means rewrite in place). It resets the source's cursor with `alenlist_cursor_init(list, 0,
0)`, and without the in-place flag creates the destination list with `alenlist_create` (a null result returns 0). Each
element is read with `alenlist_get` into an address and a length on the stack; the address is converted by
`heart_addr_phys_to_xio(address, length)`. In copy mode the converted address and the length are added with
`alenlist_append`; in place mode `alenlist_replace` overwrites the element, but only when the converted address differs
from the original. A failure in either call ends the loop; in copy mode the new list is released with `alenlist_destroy`
and 0 is returned, in place mode the loop simply stops and the same list is returned as it stands. When all elements
have been read, copy mode calls `alenlist_done` on the source and returns the new list; in place mode returns the source
list. **Established.**
- `heart_dmatrans_list` (`6.5.30/unix.IP30`, occupies 364 bytes at address 0xa80000002003f648): the same translation of an
address list as `heart_dmamap_list`, with the list in the third argument and the flags in the fourth. Arguments a0 and
a1 are not read; a2 is the source list and a3 the flags (bit 1 passes the allocation hint, bit 2 selects in-place
rewriting). The body is instruction for instruction the body of `heart_dmamap_list` with the two argument registers
shifted by one: `alenlist_cursor_init(list, 0, 0)`, a new list from
`alenlist_create` unless in place, a loop over `alenlist_get` that converts each address with `heart_addr_phys_to_xio`
and then either `alenlist_append` or, when the converted address differs, `alenlist_replace`, `alenlist_done` on the
source in copy mode and `alenlist_destroy` of the new list on a failed copy, returning the list or 0. **Established.**
- `heart_intr` (`6.5.30/unix.IP30`, occupies 364 bytes at address 0xa80000002003fe98): Heart interrupt dispatcher. It
loops over the pending interrupt vectors that the caller's mask allows, runs each vector's handler or wakes its
interrupt thread, and finally credits the handled count to a statistics word. Arguments: a0 is the exception frame
(handed to `buserror_intr`) and a1 the mask of vectors this level may service. The pending set is the doubleword at
0xffffffffaff10038 ANDed with a1; if it is zero the routine goes straight to the statistics update. Each pass takes the
lowest vector number from `ffintr64` and uses the 64-byte entry of `heart_ivec` (handler pointer +0, its argument +8,
flags +0x18, wake semaphore +0x20). Vectors 59 and above call `buserror_intr(a0)` and nothing else. For a lower vector,
when the vector is 3 or above its bit is written to the clear doubleword at 0xffffffffaff10028 before the handler runs;
vectors 0 to 2 are cleared after it instead. If bit 0x1000 of the entry's flags is set the interrupt belongs to a
thread: `valusema` reads the semaphore, `vsema` posts it when the value is below 28673 and `cvsema` is called when it is
not. Without that flag the handler is called with the entry's argument (a register call). After every vector the pending
doubleword is read again and ANDed with a1; the loop ends when it is zero. The routine counts the vectors it handled
minus one and adds that count to the word at offset 0xd0 of the structure whose pointer is at absolute address
0xffffffffffffc488, with an `ll` and `sc` pair (the branch-likely retry at 0xa80000002003ffec), so an empty pass adds
-1. **Established.**
- `heart_piomap_alloc` (`6.5.30/unix.IP30`, occupies 192 bytes at address 0xa80000002003f2c8): allocates a programmed-I/O
mapping record for a window of a Heart widget and returns it, or 0 when the window has no kernel address or the
allocation fails. Arguments: a0 is the widget vertex, a2 the crosstalk address and a3 the size,
a5 the flags; a1 and a4 are not read. A small unnamed routine at 0xa80000002003f0a4 gives the widget number (the
routine's own callees are the widget-information getters for the vertex, and it masks the id with 0xff), which
`heart_addr_xio_to_kva` turns, with the address and size, into a kernel address (the result of `heart_addr_xio_to_phys`
with 0x9000000000000000 or'd in, or 0). A zero address returns 0. The record comes from `kmem_zalloc(0x28, flag)` where
flag is 1 when bit 1 of a5 is set; a null result returns 0. When bit 3 of a5 is set the kernel address additionally gets
the bits 7 shifted left 59. The record gets the vertex at +0, the widget number byte at +4, the crosstalk address at +8,
the size at +16, the kernel address at +24 and the flags word at +32, and is returned. **Established.**
- `heart_paddr_to_port` (`6.5.30/unix.IP30`, occupies 284 bytes at address 0xa80000002003fc54): classifies a physical
address as belonging to one of the Heart's crosstalk widget windows and returns the widget number, 0xfe when the address
is not in a widget window, or 0xff when it is in none. Arguments: a0 is the address, a1 a pointer that receives the
address itself in the 0xfe case, a2 a pointer that receives the offset inside the widget window; either pointer may be
null and is then skipped. An address below 0x4000, or from 0x20004000 up to (not including) 0x800000000, takes the 0xfe
route: a1 gets a0 and the routine returns 0xfe. For 0x10000000 up to 0x20000000 the widget number is (address minus
0x10000000) shifted right 24, masked with 0xff, and a2 gets the address masked with 0xffffff. For 0x800000000 up to 1
shifted left 36 the number is (address minus 0x800000000) shifted right 31 and a2 gets the address masked with
0x7fffffff. For 1 shifted left 36 up to 1 shifted left 40 the number is the address shifted right 36 and a2 gets the
address masked with 0xfffffffff. An address from 0x4000 up to 0x10000000, and any address of 1 shifted left 40 or above,
returns 0xff without writing either pointer. **Established.**
- `heart_error_refine` (`6.5.30/unix.IP30`, occupies 244 bytes at address 0xa80000002003fd70): fills in the widget and
offset of a bus-error record when the failing address is a physical address, then passes the record up to the crosstalk
error handler. The argument a3 is the error record (a1 and a2 are passed through unchanged to the handler;
a0 is not read). The record's flag word at +0 is tested: when bit 25 is set, and it is not the case that bits 30 and 24
are both set, the routine calls `heart_paddr_to_port(record's doubleword at +16, &x, &y)`. A port number below 16 sets
bits 30 (0x40000000) and 24 (0x1000000) of the flags, stores the port in the halfword at +6 and the in-window offset y
in the doubleword at +24; the value 0xfe sets bit 20 (0x100000) and stores x, the physical address, in the doubleword at
+56; any other port leaves the record alone. Then, if bit 30 of the flags is set and the byte at +7 is below 16, it
calls `xtalk_error_handler` with the global vertex `the_error_vhdl` (word at gp-28832) and a1, a2 and the record, and
returns its result; in every other case it returns 9. **Established.**
- `xbow_widget_num_get` (`6.5.30/unix.IP30`, occupies 236 bytes at address 0xa80000002003b9f0): finds the crosstalk widget
number of the bridge that sits at, or above, a hardware-graph vertex; returns 8 for a vertex that has a ".connection"
edge, 0xff when no widget can be found, and otherwise the widget id. The argument a0 is the vertex. It forms its name
with `vertex_to_name`, then looks for the edge ".connection" below it with `hwgraph_edge_get`; when the edge exists it
returns 8. Otherwise it requires the name to contain both "pci" and "controller" (`strstr` twice) and returns 0xff when
either is missing. It then follows the edge ".." twice with `hwgraph_edge_get` (any failure returns 0xff), asks
`xwidget_info_chk` for the widget information of the vertex reached and returns `xwidget_info_id_get` of it, or 0xff
when there is none. The instruction at 0xa80000002003ba80 is never reached. **Established.**
- `xbow_enable_perf_counter` (`6.5.30/unix.IP30`, occupies 460 bytes at address 0xa80000002003d09c): starts one of the
crossbar's two link performance counters on a given port and returns 0, or -1 when the arguments are invalid or the
request conflicts. Arguments: a0 the crossbar vertex (also the argument of the periodic update), a1 the port, which must
be from 8 to 15, a2 the mode, from 0 to 2, and a3 the counter number, 0 or 1.
The crossbar soft state comes from `hwgraph_fastinfo_get`, with the register base as the doubleword at +16 and the lock
at +448 (`mutex_spinlock` and `mutex_spinunlock`). The counter's record sits 0x10 bytes apart from the soft state start:
a flag byte at +0x25 (the mode now running) and a port byte at +0x24. A running counter cannot be given a nonzero mode,
and the other counter must not already be counting the same port; either case unlocks and returns -1. Otherwise the
routine stores the mode in the bytes at +0x25 and +0x27 and the port minus 8 in the bytes at +0x24 and +0x26, replaces
bits 28 to 29 of the word at port register address (register base + port*0x40 - 0xf4) with the mode, replaces bits 20 to 22
of the counter register that the doubleword at +0x28 points to with the port minus 8, saves the register's low 20 bits
in the word at +0x20 and schedules `xbow_update_perf_counters` with `timeout(routine, a0, 4)`. It releases the lock and
returns 0. **Established.**
- `xbow_reset_link` (`6.5.30/unix.IP30`, occupies 236 bytes at address 0xa80000002003d430): resets the link of one
crossbar port and waits up to about a millisecond for it to come back. The argument a0 is the widget vertex whose widget
number (low three bits of the id from `xwidget_info_id_get`) selects the port register block, 0x40 bytes apiece from the
base held at gp-24576. The first call measures how many counts of the free-running counter at 0xffffffffaff20000 pass in
`us_delay(1000)` and stores the 32-bit difference in the word at gp-28872; later calls reuse it. The routine saves the
port's word at block +268, writes 0 to the word at +308, waits `us_delay` of the delay stored at gp-28876 and then polls
the port's word at +276: it returns as soon as bit 31 is set, restoring the saved word at +268 and returning 0. If the
counter shows that more than the measured millisecond has elapsed first it returns -1 without restoring the saved word.
**Established.**
- `pcibr_dmard_error` (`6.5.30/unix.IP30`, occupies 296 bytes at address 0xa80000002004d878): handles a DMA read error
reported by the Bridge by filling in the error record, running the PCI error handler, disabling the offending device if
the handler asks and then disabling and re-enabling the failed read response buffer. Arguments: a0 is the bridge soft
state (its word +4 is the handler's first argument), a1 and a2 are forwarded unchanged to `pciio_error_handler`, and
a3 is the error record. It reads the failing buffer number with `pcireg_resp_err_buf_get` (kept on the stack) and the
device number with `pcireg_resp_err_dev_get`; the device number shifted left 3 goes into the record's halfword at +8,
the flag word at +0 gets 0x20000000 (device valid), the doubleword at +40 gets `pcireg_resp_err_get` and the flag word
gets 0x400000. Then `pciio_error_handler(soft word +4, a1, a2, record)` runs; a nonzero result calls
`pcibr_device_disable(soft, device number from the halfword at +8 shifted right 3 and masked with 0x1f)`. The error is
cleared with `pcireg_intr_reset_set(soft, 0x10)`. For the buffer found earlier (the half is its bit 0 and the nibble its
number shifted right 1) it reads the half's assignment word with `pcireg_rrb_get`, writes it back with that nibble's
four bits cleared using `pcireg_rrb_set`, and then writes the unmodified word again with a second `pcireg_rrb_set`. It
returns the result of `pciio_error_handler`. **Established.**
- `pcibr_intr_connect` (`6.5.30/unix.IP30`, occupies 420 bytes at address 0xa80000002004a824): connects the handler of an
interrupt handle to the Bridge's interrupt bits, making the crosstalk connection for each bit that is not yet connected,
and enables the bits. Arguments: a0 is the handle (+4 the vertex for messages, +0x58 its bit set, +0x60 the soft state),
a1 the handler function and a2 its argument. A null handle returns -1. It logs the pair through `pcibr_debug` (mask
0x1000), stores a2 at +0x20 and a1 at +0x18 and sets bit 0 of the flag word at +0. For each bit in the set whose
connected word (soft state +0x690 plus 56 per bit) is zero it takes the old interrupt address with
`pcireg_intr_addr_addr`, sets the connection word at +0x698 to the bit number or'd with the soft-state word +0x48
shifted left 3, calls `xtalk_intr_connect(crosstalk interrupt at +0x668, pcibr_intr_func, node +0x670,
0xa80000002004a77c, node +0x698, 0)`, sets the connected word to 1 and logs the new value read with
`pcireg_intr_addr_get`. It then enables the whole bit set with `pcireg_intr_enable_bit_set` under the soft-state lock at
+128 (`mutex_spinlock` and `mutex_io_spinunlock`), flushing with `pcireg_tflush_get`, and returns 0. **Established.**
- `ioc3_write_ireg` (`6.5.30/unix.IP30`, occupies 124 bytes at address 0xa800000020060860): writes one of the two IOC3
interrupt registers under the driver lock. Arguments: a0 is the IOC3 driver state (the memory-window pointer is its
doubleword at +24 and the lock is at +2876), a1 the 32-bit value and a2 selects the register: 0 stores the value in the
word at window +0x20, 1 in the word at window +0x24 and any other value writes nothing. The lock is taken with
`mutex_spinlock_spl(lock, splerr)` before the store and released with `mutex_spinunlock` after it. The routine returns
nothing. **Established.**
- `ioc3_intr` (`6.5.30/unix.IP30`, occupies 396 bytes at address 0xa800000020061244): interrupt handler for the IOC3 chip;
it reads which interrupt causes are pending and calls the handler that claimed each. The argument a0 is the IOC3 driver
state, replaced by the console state stored at gp-696 when a0 is null; a null result there returns. The number of
claimed slots is the word at state +2864 and each slot is 0x58 bytes from the state start (cause mask +0x30, handler
+0x38, handler argument +0x40, flags +0x58, thread-wake semaphore +0x60, direct handler +0x80). The pending set comes
from an unnamed routine at 0xa8000000200608dc (the instructions at 0xa8000000200608dc to 0xa800000020060928: under the
lock at +2876 it reads the words at window +0x1c and +0x20 and returns their bitwise AND). The loop looks at each slot
in turn: a slot whose cause mask intersects the pending set has those bits taken from the set; when its flag word has
bit 0x1000 set it first calls the direct handler at +0x80 if it is nonzero, and when that handler returns nonzero (or is
missing) the cause bits are written back with `ioc3_write_ireg(state, mask, 1)` and the slot's semaphore is posted with
`vsema` to wake its interrupt thread; without bit 0x1000 the handler at +0x38 is called with its argument and the cause
bits. Bits left over after all slots log "unknown IOC3 interrupt 0x%x" through `cmn_err` level 1, after which the
pending set is read again. When no pending bits remain it adds the number of handled slots minus 1 to the word at offset
0xd0 of the structure whose pointer is at absolute address 0xffffffffffffc488, with an `ll` and `sc` pair (the
branch-likely retry at 0xa800000020061280). **Inferred.**
- `pcibr_intr_func` (`6.5.30/unix.IP30`, occupies 588 bytes at address 0xa80000002004ad50): common handler for one Bridge
interrupt bit; it walks the list of handles connected to the bit, runs the handlers that run in interrupt context, wakes
the interrupt threads of the others and re-enables the bit. The argument a0 is the per-bit record (the soft state is its
doubleword +0, the bit number its word +8, the handle list head the doubleword at +16 and the count of threads woken the
word at +24). First, for chip kind 2 when the soft state's word +0x78 masked with 0xf selects the pattern 2, it clears
and then sets that bit's enable bit (`pcireg_intr_enable_bit_clr` and `pcireg_intr_enable_bit_set` under the lock at
+128) so a stuck level is seen again. If the count at +24 is nonzero it does nothing more. Otherwise it makes up to two
passes over the list (the first handles the interrupt-context handlers, and only a first pass that finds none leaves a
second); each list node holds a next link, the handle at +8, the crosstalk interrupt owner at +16 and, as a fourth field
one byte wide, the line to flush. A handle counts only when bit 0 of its flag word is set; bit 1 of that word marks a
handler to call in interrupt context. In interrupt context it optionally flushes the write buffer with
`pcireg_wrb_flush_get` when the word +0x1c of the per-bit record is nonzero and calls the handle's handler (the pointer
at +0x18, argument at +0x20) in a register call; the other handles on the first pass get the same flush, the count at
+24 incremented, a request queued with `pcibr_wrap_put(record, handle +0x68)` and the handle's semaphore at +0x30 posted
with `vsema` to run its thread. After the passes, if nothing was woken it calls `pcibr_force_interrupt` on the record,
and if the last pass ran no handler at all it disables the bit (`pcireg_intr_enable_bit_clr`, `pcireg_tflush_get`, under
the same lock). The routine returns nothing. The list-walking state machine was read through the decompiler listing and
checked against the instructions for callees and offsets; the order of the two passes was not stepped instruction by
instruction. **Inferred.**
- `intrnorm` (`6.5.30/unix.IP30`, occupies 1188 bytes at address 0xa80000002001cd40): assembly entry for the normal
interrupt exception; it first works around a multiply or divide that was cut off by the interrupt, then switches to the
kernel stack and jumps to the common exception code. It runs with the interrupted registers live and takes no arguments;
addresses written as numbers are process-page words reached through the zero register. Stage one runs only when the byte
at -14584 is not 1, the doubleword at -15264 is nonzero and equals the doubleword at -16360, and the saved exception
program counter (coprocessor 0 register 14) has its top bit clear. It probes the translation buffer for the page that
holds the instruction before that counter (the translation-buffer probe, then read, instructions), uses the even or odd entry by the page-size bit, sets the
page mask to 0x6000 and gives up unless the entry is valid. It then reads the instruction word at the counter minus 4
and gives up unless its opcode field is 0 and its function field bits 3 to 5 equal 3 (a multiply or divide). Otherwise
it stores that word at +4 of the scratch routine whose pointer is at -14552 (given up when null), flushes it with cache
operations 0x15 and 0x10, saves HI and LO at -14568 and -14560, calls the scratch routine through `jalr`, and compares
the LO it left with the saved one; a difference restores HI and ends the workaround, equal LO records at -14568 whether
HI changed. Next it decides how many bytes to back the counter up: when the counter minus 4 is on a 16 KB boundary the
earlier page is probed the same way; the word at counter minus 8 decides between 8 (a branch or jump precedes, so the
instruction sits in a delay slot) and 4. The counter is lowered by that amount, the word at -14576 is incremented and,
for 8, the word at -14580 as well. Stage two selects the kernel stack. When the doubleword at -16344 is zero the byte at
-16305 is the state: 2 goes to the path at 0xa80000002001cf30, which carves a 360-byte save frame below the current
stack pointer, aligned down to 8, stores the old stack pointer at +224 and jumps to `ecommon` with k0 = 256; state 1
builds the frame on the current stack, stores the frame pointer at +552 of the structure at -16352, switches to the
stack at -16336 and jumps to `ecommon` with k0 = 2; state 3 sets the byte at -16301, switches to the stack at -16336 and
jumps to `ecommon_allsaved` when the byte at -16304 is nonzero and to `ecommon` otherwise; any other state sets the byte
to 2, builds the frame at the top of the structure at +992 of the doubleword at -16360, switches to the stack at -16336
and the kernel global pointer, and jumps to `ecommon`. When the doubleword at -16344 is nonzero it sets the state byte
to 2 (unless it already is, which takes the 256 path), builds the frame from the +136 doubleword of that structure,
saves ra, a0 to a7, s0 to
s7, gp, sp and the saved value of at, calls `tfi_save`, copies the +144 doubleword into the word at -16360, calls
`uli_setasids` with the halfword at +168 and jumps to `ecommon_allsaved`. The block at +0x94, a tail jump to `assfail`
with "invalid kstk", "intrnorm.s" and 0, is not reached from inside the routine. The workaround stage was read
instruction by instruction but its purpose (a cut-off multiply or divide) is inferred from the instruction pattern.
**Inferred.**
- `xbow_attach` (`6.5.30/unix.IP30`, occupies 796 bytes at address 0xa80000002003b64c): attach routine of the Crossbow
switch widget; it maps the chip, builds its soft state, hooks its error interrupt and brings up every port that has a
live link. The argument a0 is the widget vertex. It maps 0x300 bytes of the widget's registers with
`xtalk_piotrans_addr(a0, 0,
0, 0x300, 0)`, takes the parent with
`hwgraph_connectpt_get`, adds the character device "xbow" with `hwgraph_char_device_add` and allocates a 0x290-byte
state block with `kmem_zalloc`, filled with the widget vertex (+0), the new vertex (+4), the parent (+8) and the
register pointer (+16) and attached with `hwgraph_fastinfo_set`. A name of the form "%s[xbow# %d]" is built from
`dev_to_name` and `xswitch_id_get` and stored at +24. The revision key is the register word at +4 with bits 28 to 31
merged with bits 12 to 27 shifted left 4. A key of 0 or 1, or any key not matched below, goes to a `cmn_err` level 4
report "Crossbow ASIC: rev %s (code=%d)" with "DOWNREV " prefix, which does not return; the names it prints are "1.0"
for key 1, "1.2" for 3, "1.3" for 4, "2.0" for 5, "Xbridge 1.0" for 0xd0001, "Xbridge 2.0" for 0xd0002 and "unknown"
otherwise. For a higher key it initialises two locks, "xbow_perf_lock" at +0x1c0 and "xbow_bw_alloc_lock" at +0x208,
with `spinlock_init`, stores the performance-counter register addresses (+0x6c and +0x64 from the register base) at +56
and +40, clears eight per-port words at +0x250 and sets eight words at +0x210 to 800000000, allocates the error
interrupt with `xtalk_intr_alloc` (descriptor duplicated, flag bit 0 set, name "Crossbow error") and connects it to the
handler at 0xa80000002003c2cc with `xtalk_intr_connect`, registers `xwidget_error_register` with the handler at
0xa80000002003c8c0, writes 0x24 to the register word at +0x24 and creates the switch description with
`xswitch_info_new`. For ports 8 to 15 it asks the unnamed routine at 0xa80000002003beac whether the port is usable and
`xbow_widget_present` whether a widget answers; for a present widget it calls `xswitch_info_link_is_ok` and rewrites the
port's word at register +0x10c (port block stride 0x40) with bits 0x1d0 cleared and 0x3002b set. It finishes with
`xswitch_provider_register(parent, xbow_provider)` and returns 0. **Established.**
- `xbow_xmit_retry_error` (`6.5.30/unix.IP30`, occupies 260 bytes at address 0xa80000002003c1c8): counts and clears a
transmit-retry error on one crossbow port and tells the caller whether it was one it recognised. Arguments: a0 is the
crossbow soft state and a1 the port number. The mapped registers of the widget on that port are cached in the doubleword
at soft state + 8*port + 392. When the cache is empty the routine asks `xswitch_info_get` for the switch description of
the vertex at +8, `xswitch_info_vhdl_get` for the port's vertex and maps 0x58 bytes of it with
`xtalk_piotrans_addr(vertex, 0, 0,
0x58, 0)`, caching the pointer; if any of the three comes back null or -1 it returns 1 without recording anything. With
registers in hand it reads the word at +4: when the part number (bits 12 to 27) is 0xc002 and the manufacturer field
(bits 1 to 11) is 54, or the part is 0xd002 and the manufacturer 36, it adds 1 to the global counter
`xbow_xmit_retry_errors`, reads and writes back the word at +0x24 (which acknowledges the error) and returns 1. For any
other identification it returns 0 and touches nothing. **Established.**
- `pcibr_init_ext_ate_ram` (`6.5.30/unix.IP30`, occupies 352 bytes at address 0xa800000020047140): finds out how much
external address-translation RAM the Bridge has, programs that size and clears the entries, returning the number of
entries (0 when there is none). The argument a0 is the bridge soft state (+4 the vertex for messages, +8 the register
pointer). The candidate sizes are the words of the table at 0xa80000002043eac0, which hold 0, 8192, 16384 and 65536.
For each size index 1 to 3 it writes the pattern 0x0123456789abcdef into the last entry of that size
(`pcireg_ext_ate_set(soft, size - 1, pattern)`), writes 0 into the last entry of each smaller size (indices 1 up to the
current one, so that a wrap-around is not misread), and reads the entry back with `pcireg_ext_ate_get`; when the read
equals the pattern the index is recorded as the largest size that works. The chosen index is written with
`pcireg_ssram_size_set` and the count is that index's table word. When debug mask 0x10 is on it logs "bridge at 0x%x: no
external ATE RAM found" for a count of 0 and "bridge at 0x%lx: clearing %d external ATEs" otherwise. The first count
entries are then set to 0 with `pcireg_ext_ate_set`, and the count is returned. **Established.**
- `pcibr_release_device` (`6.5.30/unix.IP30`, occupies 176 bytes at address 0xa800000020042fe4): drops one reference on a
per-slot resource of the Bridge, picked by a resource code. Arguments: a0 is the bridge soft state, a1 the slot and a2
the code. With the soft-state lock at +128 held (`mutex_spinlock`), it subtracts 1 from one of three counters in the
slot's 0x80-byte record, chosen by the code: 0x1100000 selects the word at slot record +0x144, 0x8f8000 the word at
+0x148 and 0x898000 the word at +0x14c; any other code changes nothing. It releases the lock with `mutex_io_spinunlock`
and returns nothing. **Established.**
- `pcibr_func_config_addr` (`6.5.30/unix.IP30`, occupies 132 bytes at address 0xa80000002004dd78): forms the address
through which one PCI configuration register of one function can be accessed, for a device on the Bridge's own bus or
behind a PCI bridge. Arguments: a0 is the bridge's register state, a1 the bus number below the Bridge (0 is the primary
bus), a2 the device number, a3 the function and a4 the register. For bus 0
(a1 not positive) the result is `pcireg_type0_cfg_addr(a0,
a2, a3, a4)`. For a positive bus number it first programs the type-1 control register with
`pcireg_type1_cntr_set(a0, (a1 << 16) |
(a2 << 11))`, then returns
`pcireg_type1_cfg_addr(a0, a3, a4)`. The return value is whatever the chosen address routine returns. **Established.**
- `heart_invalidate_war` (`6.5.30/unix.IP30`, occupies 148 bytes at address 0xa8000000200140a4): wrapper that runs the
cache-invalidate workaround routine with interrupts or preemption held off, and does nothing when the workaround is off.
Arguments: a0 and a1 are passed unchanged to `__heart_invalidate_war`. When the word
`_heart_invalidate_war` is zero the routine returns at once. Otherwise, if the doubleword at absolute address
0xffffffffffffc0a0 is zero or the byte at 0xffffffffffffc0cf is not 2, it raises the interrupt level with `spl1(2)`
before the call and restores it with `splx` afterwards; if that doubleword is nonzero and the byte is 2 it instead pins
the thread with `rt_pin_thread(2)` before the call and releases it with `rt_unpin_thread` afterwards. It returns
nothing. **Established.**
- `ecc_init` (`6.5.30/unix.IP30`, occupies 204 bytes at address 0xa800000020027e10): allocates the per-processor save area
that the cache-error exception writes to and publishes its uncached address. It takes no arguments. It reads the
processor identity (the halfword at absolute address 0xffffffffffffc3a0), allocates 0x1000 bytes with
`kmem_zalloc(0x1000, 0x900)` and calls the unnamed routine at 0xa800000020027dc8 with the block and 0x1000. The block's
uncached address is its address masked with 0xffffffffff and or'd into 0x9000000000000000; it is stored in
`cacheErr_frames` at 8 times the identity and also at the uncached address 0x9000000000000ff0 plus 8 times the identity,
where the cache-error handler finds it. When the allocation fails it logs "Unable to allocate ECCF" through `cmn_err`
level 0 and then continues into the same call with a null block pointer. It returns nothing. **Inferred.**
- `xbow_ioctl` (`6.5.30/unix.IP30`, occupies 876 bytes at address 0xa80000002003badc): handles the control requests of the
crossbow character device and returns an error number. Arguments: a0 is the crossbow vertex, a1 the request, a2 the user
argument pointer, a3 the open-mode flags (bit 0 read, bit 1 write), a4 the caller's credentials and a5 a pointer to the
call's result word, which is set to 0 first. A privileged request first asks the capability check `cap_able_cred` (or
`cap_able`) for the capability 0x2000000000 and returns 1 when it is denied. Request 1 and request 2 start a performance
counter: they need the write flag (else 9), copy 12 bytes from the user with `copyin` (14 on a fault) and call
`xbow_enable_perf_counter(vertex, port, mode, counter)`; request 2 forces the mode word to 0. A zero result gives 0 and
any other 22. Request 3 reads the counters: it needs the read flag (else 9), takes the 0x100-byte block from
`xbow_get_perf_counters` and copies it out with `copyout`, giving 0 or 14.
Requests 4 and 5 call `xbow_enable_llp_monitor` and `xbow_disable_llp_monitor` after the capability check and return 0
or 22. Request 6 reads the link status: read flag needed, the 0x80-byte block from `xbow_get_llp_status` copied out, 0
or 14. Requests 0x6701 and 0x6702 reserve bandwidth between two widgets: after the capability check they copy 16 bytes
in (14 on a fault), turn the first two words into widget numbers with `xbow_widget_num_get` and call
`xbow_prio_bw_alloc` twice, once in each direction; 0x6701 passes bandwidth 0 and the fourth word, 0x6702 passes the
third word and its negative. Both give 0. A null block from either getter ends in a failed assertion through `assfail`
(the checks named "xbow_perf_cnt != NULL" and "xbow_llp_status != NULL" in its strings). Any other request returns 0.
**Established.**

## Heart, Bridge, GIO bridge and cache-error routines

- `giobr_piomap_alloc` (`6.5.30/unix.IP30`, occupies 244 bytes at address 0xa800000020040e68): builds a programmed-I/O
map for a window of the GIO bridge. The arguments are a0 (the bridge's vertex, passed unchanged to `gioio_info_get` and stored in the record), a1
(not read), a2 (a window index),
a3 (an offset into the window), a4 (a length), a5 (forwarded as the fifth argument of the crosstalk allocator) and a6
(a flags word, saved at sp+0 and stored in the record). The routine calls `gioio_info_get` and `gioio_info_mfast_get`
to reach the bridge's information record and loads the word at +20 of it. It then calls the static routine at
0xa800000020040dc4 (inside the range of `giobr_attach`, with no symbol of its own) with a0 and, as its second to fourth arguments, a2, a3 and a4. That helper
repeats the two lookups and returns 0 unless the window index is 0 to 4, the byte at +64 plus the index in the record
is non-zero and offset plus length does not exceed the word at +8 of the 16-byte entry (the table is the one `gp`
reaches at 0xa8000000204a9760, which holds 0xa80000002043e100; entries start 16 bytes into it); otherwise it returns
the 64-bit value stored first in the entry, plus the offset. A zero result is returned as zero at once. Otherwise
`kmem_alloc(56, 0)` makes the record; if that fails the routine returns zero. The record is then filled by a call to
`xtalk_piomap_alloc` with the record word loaded from +20 as a0, 0 as a1, the helper's result as a2, the length as a3,
a5 as a4 and 0 as a5. A zero from that call frees the record with `kmem_free(record, 56)` and returns zero. On success
the stores, in address order of issue, are +48 the crosstalk map, +40 the helper's result, +24 the length, +16 the
offset, +4 the vertex word and +0 the flags word, and the record address is returned. There is no branch-likely
instruction. **Established.**
- `pcibr_slot_device_init` (`6.5.30/unix.IP30`, occupies 244 bytes at address 0xa80000002004f914): rewrites one slot's
Bridge device register to its start-up value and keeps a copy. The arguments are a0 (the bridge vertex) and a1 (the slot
number). `hwgraph_fastinfo_get(a0)` gives the soft-state record; a null record returns 22. The slot is accepted when it
is below the byte at +57 of the record plus one, compared as signed numbers, so there is no lower bound; a larger slot
also returns 22. `pcireg_device_get(record, slot)` then yields the current register value, and the halfword at +114 (the
chip kind) selects the bits. The value is first cleared of 0x08000000. For kind 2 the low four bits of the word at +120
choose between two results: when they equal 1 the routine ORs in 0x00010000 only; otherwise it ORs in 0x02010000. Any
other kind ORs in 0x02010000. The result is stored as a doubleword at record + 312 + (slot << 7), written to the chip with
`pcireg_device_set(record, slot, value)` and logged by `pcibr_debug` (mask 0x4000000, the vertex, the string "pcibr_slot_device_init:
Device(%d): %R", the slot, the value and a register-description table). The routine returns 0. No branch-likely
instruction is present. **Established.**
- `pcibr_endian_set` (`6.5.30/unix.IP30`, occupies 232 bytes at address 0xa800000020046444): sets or clears the pair of
byte-swap bits 0x180000 in a slot's Bridge device register under the bridge lock. The arguments are a0 (a connection
vertex), a1 and a2 (two values compared for equality) and the result is a2 unchanged on every path. `pciio_hostinfo_get(a0)`
gives a record whose byte at +540 is the slot number; `pciio_info_mfast_get` of that gives the bridge soft-state record, and the lock at +128 of it is taken with `mutex_spinlock`. The
current register copy is the doubleword at record + 312 + (slot << 7). When a1 equals a2 the new value is the copy with 0x180000 cleared; when
they differ it is the copy with 0x180000 set. If the new value equals the old one nothing is written. Otherwise
`pcireg_device_set(record, slot, new)` runs, then the copy is replaced (the store sits in the delay slot of the call to
`pcireg_tflush_get`, whose result is discarded). The lock is released with `mutex_io_spinunlock` and the level that
`mutex_spinlock` returned (reloaded from sp+56 on the changed path). Last it calls `pcibr_debug` (mask 0x4000000, a0, the string
"pcibr_endian_set: Device(%d): %R", the slot, the new value) whether or not anything changed. No branch-likely
instruction. **Established.**
- `ecc_exception` (`6.5.30/unix.IP30`, occupies 460 bytes at address 0xa80000002001a3e0): the cache-error exception body
that saves the interrupted state, asks `ecc_handler` what happened and returns with `eret`. It has no stack frame and no
arguments; it takes the register-save area from k1, which it never sets, so a stub before it has to. It first merges
coprocessor-0 registers 19, 18 and 17 into one doubleword in k0 (register 19's low three bits shifted to the top, register 18's
low word shifted left by 29, register 17's low word zero-extended) and stores it at +208 of the area; the exit path reloads
k1 from that slot, so this is where the stub parked the old k1 (inferred from the reload). It then stores s0 to s7 at +120 to +176
in steps of 8, a0 to a7 at +24 to +80 in steps of 8, gp at +216, sp at +224 and ra at +240, builds gp as 0xa8000000204ae7f0
and calls `tfi_save` (the call at 0xa80000002001a470, target 0xa80000002001bc20).
After the call it stores the Status register at +248, Cause at +280, register 8 at +272 and register 14 at +288 and +352,
the same layout as the save in `assfail`. s0 is set to the area and s1 to the area plus 360. If the byte at +38 of s1 is
zero it is set to 1, and the routine then stores ErrorEPC (register 30) at +0 of s1 and registers 29, 28 and 27 as words at +32, +28
and +24 of s1. The stack becomes the area plus 4096 and `ecc_handler(area, s1)` is called (the call at 0xa80000002001a4dc, target 0xa800000020027fd4). A zero result skips to the restore. A non-zero
result loads sp from the absolute address 0xffffffffffffc3c0, rebuilds gp, clears bits 3 and 4 of Status (mask -25) and
calls `ecc_panic` with the handler's result, the area and s1 as arguments; if that returns it falls into the restore. The restore
writes registers 29 and 28 back from the words at +32 and +28 of s1, calls `tfi_restore` (with k1 set to the area in its delay
slot), writes ErrorEPC from +0 of s1, reloads a0 to a7, s0 to s7, ra, at (from +0 of the area, which this routine never
stores, so the stub does), sp and gp, clears the byte at +38 of s1, reloads k0 from +200 (also not stored here) and k1 from
+208, and executes `eret`. Register 27 is saved but not written back. There are no branch-likely instructions and no
return value. **Established.**
- `ecc_panic` (`6.5.30/unix.IP30`, occupies 112 bytes at address 0xa8000000200286c4): reports an unrecoverable cache
error and passes it to `cmn_err`. The arguments are a0 (a text naming the error, as returned by `ecc_handler`), a1 (the
register-save area) and a2 (the per-processor error record, which `ecc_exception` sets to the area plus 360). The routine
first calls an unlabelled routine at 0xa8000000200027a10 (the symbol table has no entry there; its nearest earlier symbol,
`ecc_cacheop_get`, is only 36 bytes long and ends at that address) with 20, a0, the word at record +24 (the cache-error value),
the doubleword at record +0 and the address record +8. It then calls `cmn_err` with level 16, the format
"Cache Error (%s) Eframe = 0x%x" at 0xa8000000203ada48, a0 for the %s, the address a1 + 952 for the %x and a fifth argument,
0x203ad7e0, that the format does not use. The value `cmn_err` returns is the routine's own result (v0 is not
touched after the call). It saves s8 and ra and uses a 32-byte frame; no branch-likely instruction. **Established.**
- `ecc_printPending` (`6.5.30/unix.IP30`, occupies 304 bytes at address 0xa80000002002873c): walks the per-processor
cache-error records and prints each one's counts and queued errors through a caller-supplied print routine. The only
argument, a0, is a function pointer kept in s5 and called with `jalr` (a0 of each call is a format string). The routine
visits indexes 0 to 127 in order and, for each, loads the pointer at that index of the eight-byte array `cacheErr_frames`
(0xa8000000204b0e10); a null pointer is skipped. For a non-null pointer p it calls the printer with the format
"Total Errors [cpu %d] ic(%d) dc(%d) sc(%d) in(%d)" (0xa8000000203ada68), the index, then the words at p +400, +404, +408
and +412. It then takes the record at p + 360 and compares its words at +572 and +568; if they are equal the queue is empty
and it goes to the next index. Otherwise it starts at the value of the word at +572 and, for each queue slot (32 bytes apart,
from the record's +56) prints "\tcer=0x%x errorEPC=0x%x tag [0]=0x%x [1]=0x%x" (0xa8000000203adaa0) with the unsigned word at
slot +56, the doubleword at +80, the doubleword at +64 and the doubleword at +72 (the order the arguments are loaded in).
The slot number advances with an absolute-value sequence that gives (n + 1) AND 15 for a non-negative n, and the loop stops
when it equals the word at record +568 (reloaded each time). No result is set and there are no branch-likely
instructions. **Established.**
- `pcibr_bridge_ptr_get` (`6.5.30/unix.IP30`, occupies 172 bytes at address 0xa800000020046c00): returns the address of
a bridge widget's register block, moved to the second bus's block when asked. a0 is passed unchanged as the first argument
of `xtalk_piotrans_addr`, which is called with 0, 0, 8 and 0 as the rest (an 8-byte window at offset 0); a1 is saved at sp+8.
There is no check for a null result. The routine reads the word at +4 of the window, takes bits 12 to 27 as a part number
and bits 1 to 11 as a manufacturer number. If the part number is 0xd102 or 0xd112 (0xd102 is tested first, 0xd112 second)
and the manufacturer number is 36, and a1 equals 1, it returns the window address plus 0x800000 (the add is in the delay slot
of the jump at 0xa800000020046ca4). Every other path, including a1 other than 1, a part that matches but whose manufacturer
is not 36 and any other part number, returns the window address unchanged. The routine does not say what the part numbers
name; that 0x800000 is the second bus's register block of a two-bus bridge is an inference from the constant alone. No
branch-likely instruction. **Established.**
- `heart_intr_connect` (`6.5.30/unix.IP30`, occupies 212 bytes at address 0xa80000002003fac8): installs an interrupt
handler in the Heart vector table and unmasks its level. a0 is an interrupt descriptor: the byte at +5 is the level (s1), the halfword at
+36 is the first argument of `heart_imr_bits_rmw` and `heart_ithread_setup` (s0) and the word at +32 is the third argument of
the latter. a1 and a2 are the handler and its argument, a3 an optional hook (s2) and a4 a further value. The table is `heart_ivec`
(0xa80000002043cfd8, reached through the pointer at 0xa8000000204a9720), entries 64 bytes apart indexed by the level. The
stores, in issue order, are a2 at entry +8, a1 at entry +0, a3 at descriptor +24 and a4 at descriptor +16. When bit 0x100 of
the 32-bit field at entry offset 24 is set it stores the halfword from descriptor +36 at entry +16, sets bit 0x200 of the entry word
with a load-linked/store-conditional loop (retried by the branch-likely `beql` at 0xa80000002003fb30) and, when the
kernel word `ithreads_enabled` (0xa8000000204a7768) is non-zero, calls `heart_ithread_setup(halfword, level, word at +32)`.
Whatever the flag, if a3 is non-zero it then calls a3 with the descriptor as its argument. Last it calls
`heart_imr_bits_rmw(halfword, 0, 1 << level)` and returns 0. **Established.**
- `pcibr_force_interrupt` (`6.5.30/unix.IP30`, occupies 136 bytes at address 0xa800000020049d20): raises one Bridge
interrupt bit by software. a0 points to a request: the doubleword at +0 is the bridge soft-state record (s0) and the
word at +8 is the interrupt bit number (s1). When the halfword at record +114 (the chip kind) is non-zero it calls
`pcireg_force_intr_set(record, bit)` and returns that call's result. For kind 0 it reads the status with
`pcireg_intr_status_get(record)` and ANDs it with 1 shifted left by the bit number; a zero result returns 0. When the
bit is set it loads the doubleword at record + 1640 + 56 x bit, passes it to `xtalk_intr_vector_get`, sign-extends the
low 16 bits of the answer (`dsll32` by 16 then `dsra32` by 16, a 48-bit shift each way) and passes that to
`heart_set_cpu_intr`, whose result it returns. No branch-likely instruction. **Established.**
- `pcibr_piotrans_addr` (`6.5.30/unix.IP30`, occupies 204 bytes at address 0xa80000002004465c): the Bridge's
translate-a-window entry. The arguments are a0 (a connection vertex), a2, a3, a4 and a5; a1 is never read. `pciio_hostinfo_get(a0)`
supplies a record whose byte at +540 is the slot, and `pciio_info_mfast_get` of that gives the bridge soft-state record;
the word at +0 of it is saved at sp+0 and later used as the first argument of `xtalk_piotrans_addr`. The routine then calls the
static routine at 0xa800000020043aac (no symbol of its own; the symbol table puts it under `pcibr_write_gather_flush`, and its body is not
read for this entry) with a0, the slot, a2, a3, a4 and a5, in that order. A result of -1 sends it to a message
through `pcibr_debug` (mask 0x10000000, a0, the text "pcibr_piotrans_addr: xio_addr == XIO_NOWHERE") and the routine returns 0.
Any other result goes to `xtalk_piotrans_addr(saved word, 0, result, a4, a5 AND 0x1f)` and that call's result is returned. The
helper is taken to produce a crosstalk address, from its -1 failure value and the message text. No branch-likely
instruction. **Established.**
- `pcibr_detach` (`6.5.30/unix.IP30`, occupies 400 bytes at address 0xa8000000200437e4): takes a Bridge down when its
hardware-graph vertex is removed. a0 is the vertex that has the "pci" edge; a0 is kept in s1. After a `pcibr_debug` call (mask 8,
a0, the text "pcibr_detach") it calls `hwgraph_traverse(a0, "pci", &result)`; a non-zero return is returned at once as 1.
Otherwise the vertex that call wrote through its stack-address argument (a2) is the Bridge's and `hwgraph_fastinfo_get` gives its soft-state record (s0). Under the
spinlock at record +128 (`mutex_spinlock`, released by `mutex_io_spinunlock` with the saved level) it calls
`pcireg_intr_enable_set(record, 0)`, which clears every interrupt enable. It then calls
`pcibr_slot_detach(vertex, slot, 0, 0, 0)` for each slot from the byte at record +56 up to and including the byte at +57 (the slot
counter is masked to 8 bits and the bound is compared as signed numbers; a first slot above the last skips the loop).
Next, in order: `pciio_device_info_unregister(vertex, doubleword at +248)`, `spinlock_destroy` on the record's lock, `strlen` and
`kmem_free` of the string pointed to at +16, `rmfreemap` of the map pointed to at +96 and then of the one at +104 (each only when its
pointer is non-zero), `xtalk_intr_disconnect` and `xtalk_intr_free` on the pointer at +2088 (no null test), and `kmem_free(record,
2744)`. It finishes by removing the label "_pcibr_asic_rev" from the vertex with `hwgraph_info_remove_LBL`, the edge
"controller" from the same vertex, and the edge "pci" from a0 with `hwgraph_edge_remove` (the third argument of each is 0). It
returns 0. No branch-likely instruction. **Established.**
- `giobr_rrb_call_all` (`6.5.30/unix.IP30`, occupies 180 bytes at address 0xa8000000200406ac): runs six small helper
routines in a fixed order on the GIO bridge record and returns the sum of four of their results. The helpers have no
symbols (the symbol table puts them under `heart_device_inquiry`); they are named here by address. The arguments are a0
(the record, kept in s1), a1 (a selector, s0), a2 (s8) and a3 (saved at sp+24). Every helper picks one of two packed
words in the record by bit 0 of a1, the word at +652 when it is set and the word at +644 when it is clear, and treats
the word as eight 4-bit fields; the device number it looks for is a1 divided by two (signed), and a field whose bit 3
is set is "enabled". The helpers, in call order: (1) 0xa80000002004039c with a0 and a1 returns how many fields are enabled
and hold that device number; (2) 0xa80000002004041c with a0 and a1 returns how many fields have bit 3 clear; (3)
0xa800000020040470 with a0, a1 and a2 gives the lowest-numbered a2 clear-bit-3 fields the value (device number OR 8), one at a
time, and returns 0; when there are fewer free fields it stores the partly updated word and returns -1, and a count of
zero or less stores the word unchanged and returns 0; (4) 0xa800000020040528 with a0 and a1 gives every clear-bit-3
field that value, stores the word and returns the new word; (5) 0xa80000002004058c with a0, a1 and a3 clears bit 3 of
the lowest-numbered a3 fields that are enabled for the device, with the same -1 and zero-count behaviour as (3); (6)
0xa800000020040634 with a0 and a1 clears bit 3 of every field enabled for the device, stores the word and returns it.
The routine saves the results of (1), (2), (3) and (5) at sp+32, sp+64, sp+56 and sp+48 and returns their sum; the
results of (4) and (6) are discarded. No branch-likely instruction. **Established.**
- `heart_attach` (`6.5.30/unix.IP30`, occupies 120 bytes at address 0xa80000002003f02c): attach routine for the Heart
widget vertex. a0 is the vertex, kept in s0. `xwidget_info_get(a0)` returns the widget record (saved at sp+16) and
`xwidget_info_master_get` of it gives the master vertex (s8). It adds two hardware-graph edges with `hwgraph_edge_add`:
first from a0 to the master with the name "heart", then from the master back to a0 with the name ".connection". Last it
calls `xwidget_error_register(a0, 0xa80000002003fe64, record)`, where the second argument is the pointer stored at
0xa8000000204a9730 (the address has no symbol; the nearest symbol below it is `heart_error_refine` at
0xa80000002003fd70). The routine returns 0 and does not use the result of that call. No branch-likely instruction.
**Established.**
- `ioc3_intr_disconnect` (`6.5.30/unix.IP30`, occupies 356 bytes at address 0xa8000000200610e0): releases the interrupt
claims that match one connection in an IOC3's table of interrupt connections. The arguments are a0 (the PCI device vertex,
passed unchanged to `hwgraph_traverse`), a1 (a mask of interrupt bits, saved at sp+8) and a2, a3 and a4 (kept in s0, s1 and s2;
a4 is also the argument of the failure message). The routine calls `hwgraph_traverse(a0, "ioc3", &result)`. A non-zero return
makes it call `cmn_err(1, "%v: ioc3_intr_connect: ioc3_attach not yet called", a4)` and return what `cmn_err` returns.
Otherwise `hwgraph_fastinfo_get` of the vertex written to the result gives the IOC3 soft-state record, and the word at record
+2864 is the number of table entries. Entries are 88 bytes apart and start at the record itself. An entry matches when its
doubleword at +56 equals a2, its doubleword at +64 equals a3, its word at +72 equals a4 and its word at +48 shares a bit with
a1; any other entry is skipped. For a match, the shared bits are cleared from the entry word at +48 and from the word at record
+2868, each with its own load-linked/store-conditional loop. If the entry word at +48 is then zero, the entry is released: -1 is
stored as a word at +72, zero as doublewords at +64 and then +56, and the bit numbered by the entry's index is set in the word at record
+2872 with a third such loop. The scan continues through every entry (it does not stop at the first match). The retry
loops end in the branch-likely `beql` instructions at 0xa8000000200611b4, 0xa8000000200611c8 and 0xa80000002006121c. v0 is never
rewritten on the normal paths, so they return the record pointer that `hwgraph_fastinfo_get` gave (and so the pointer is
returned even when nothing matched). **Established.**
- `heart_intr_prealloc` (`6.5.30/unix.IP30`, occupies 92 bytes at address 0xa80000002003f834): marks one Heart interrupt
level as taken and then hands the work to a caller-supplied routine. a0 is not read. a1 is the level (kept in s8), a2 is a function pointer and a3
and a4 are two values passed on. It first calls the unlabelled routine at 0xa80000002003e9a8 (reached from the end of
`heart_imr_bits_rmw`'s symbol range), with a0 equal to 1 shifted left by the level (a variable 64-bit shift in the delay slot). That
routine clears those bits atomically with a load-linked/store-conditional doubleword loop in the word `heart_ivec_free`
(0xa8000000204a7758), repeating until the conditional store succeeds. This routine itself holds no branch-likely instruction. It then reads the doubleword at
the absolute address 0xffffffffaff00080 and calls a2 through `jalr` with a0 = a3, a1 = a4, a2 = the low four bits of that
doubleword (the AND is in the delay slot), a3 = 128, a4 = the level and a5 = the function pointer. Its return value is the
callee's. **Established.**
- `heart_intr_preconn` (`6.5.30/unix.IP30`, occupies 104 bytes at address 0xa80000002003f7cc): installs a handler for a
Heart interrupt level that was reserved in advance, and unmasks the level on the executing processor. a0 is not read; a1 is the level (s8), a2 the handler and a3 its
argument. It calls the unlabelled routine at 0xa80000002003e9a8 with 1 shifted left by the level, which atomically clears those bits in
`heart_ivec_free` (0xa8000000204a7758). It then takes the entry of `heart_ivec` for the level (64 bytes apart, base reached
through the pointer at 0xa8000000204a9720) and stores a3 at entry +8 and a2 at entry +0, in that order (the second store is in the delay slot
of the `getcpuid` call). It calls `heart_imr_bits_rmw(getcpuid(), 0, 1 << level)` to set the level's mask bit on the processor
and returns that call's value, the register's previous contents. No other entry field is touched, unlike `heart_intr_connect`.
**Established.**
- `pcibr_errintr_group` (`6.5.30/unix.IP30`, occupies 132 bytes at address 0xa80000002004c028): turns a Bridge interrupt-status
word (a0) into a small group code. It is a leaf with no frame, no memory access and no branch-likely instruction. The result
starts at 65 when any bit of 0xfc00 is set in a0 and at 64 otherwise (`movz` in the delay slot of the first branch), and then
gains one bit per class: 0x02 when a0 has a bit of 0x40010000, 0x04 for a bit of 0x3e0000, 0x08 for a bit of 0xb80000, 0x10 for a bit of 0x14000200
and 0x20 for a bit of 0x20400000. The classes are tested in that order and every class that matches adds its bit. What the
classes mean is not read from this routine. **Established.**
- `heart_addr_xio_to_phys` (`6.5.30/unix.IP30`, occupies 164 bytes at address 0xa80000002003f1ec): converts a widget-relative
crosstalk range into a physical address by the size of the range. a0 is a widget number, a1 an offset and a2 a length;
a3 to a7 are scratch and no memory is touched. If the top four bits of a1 are non-zero, a0 is replaced by that nibble and
a1 is cut to its low 48 bits. The end is a1 plus a2 and all comparisons are unsigned. For an end up to 0x1000000 the result is
0x10000000 + (a0 << 24) + a1; for an end above that and up to 0x80000000 it is 0x800000000 + (a0 << 31) + a1; for an end above that and up to
2^36 it is (a0 << 36) + a1; for an end above 2^36 it is 0. Those are the four exits. No branch-likely instruction.
**Established.**
- `heart_mlreset` (`6.5.30/unix.IP30`, occupies 164 bytes at address 0xa80000002003ed30): puts the Heart interrupt hardware
for the executing processor back to a quiet state. It takes no arguments. `getcpuid` returns the processor number (sign-extended from 16 bits). The
stores, in issue order, are: a doubleword 0 at 0x9000000018000068; a read of the doubleword at 0x9000000018000060 written straight
back; a word 0 at 0x9000000018000044; a read of the doubleword at 0xffffffffaff10040 written straight back; the doubleword
0x3ff62c07 at 0x9000000018000068; doublewords 0 at 0xffffffffaff10000, +8, +16 and +24; and last the doubleword at 0xffffffffaff10000
plus 8 times the processor number, which receives bit 63 plus bits (cpu+42), (cpu+46) and (cpu+59). The three 0x9000000018xxxxxx
addresses come from pointer cells in the small-data area (gp-24376, gp-24264 and gp-24384); the other four registers are the mask
registers `heart_imr_bits_rmw` indexes. The last store is in the delay slot of the return, so v0 on return is the address of that mask register. The routine
does not say what the 0x9000000018xxxxxx registers are. No branch-likely instruction. **Established.**
- `ioc3_subdev_enabled` (`6.5.30/unix.IP30`, occupies 72 bytes at address 0xa8000000200618e4): says whether one subdevice
of an IOC3 is enabled. a0 is the PCI device vertex and a1 the subdevice number (s8). It calls the unlabelled routine at 0xa800000020061824 with a0 and 0; a zero answer means no mask was recorded and the mask is 255 (bits 0 to 7), otherwise the mask is the doubleword the
answer points to. The result is the mask ANDed with 1 shifted left by a1, cut to 32 bits (`sll v0,v0,0`), so it is
non-zero exactly when that bit is set. The helper does `hwgraph_traverse(a0, ".host", &slot)` (the slot keeps a0 when the
traverse fails), then `hwgraph_info_get_LBL(slot vertex, "_subdevs", &value)`; with its second argument zero it returns the label value (zero
when nothing was recorded) and never creates one. With a non-zero second argument it would allocate 8 bytes with
`kmem_zalloc`, store 255, add the label with `hwgraph_info_add_LBL`, read it back and free its block with `kmem_free(block, 8)`
if another caller's block won; this routine never takes that path. No branch-likely instruction. **Established.**
- `pcibr_directmap_init` (`6.5.30/unix.IP30`, occupies 148 bytes at address 0xa800000020043974): sets up the Bridge's
direct-mapped DMA window. a0 is the bridge soft-state record (s0). It calls `xtalk_dmatrans_addr(word at +0, 0, 0, 0x4000, 0)`, keeps the result (s1),
passes it shifted right by 31 to `pcireg_dirmap_diroff_set` (the shift is in the delay slot) and passes the byte at +48 to `pcireg_dirmap_wid_set`.
It copies that byte to +88 as well. When the result equals 0x20000000 it calls `pcireg_dirmap_add512_set(a0)`, stores the doubleword
0x20000000 at +80 and returns that call's result. Otherwise it calls `pcireg_dirmap_add512_clr(a0)`, stores the result ANDed with
0xffffffff80000000 (the `lui` sign-extends, so the upper 32 bits are kept) as the doubleword at +80, and returns the value of
that call. No branch-likely instruction. **Established.**
- `giobr_piotrans_addr` (`6.5.30/unix.IP30`, occupies 140 bytes at address 0xa800000020040fc4): translates a window of the GIO
bridge to a processor address. a0 is the bridge vertex (s2), a1 is not read, a2 is a window index, a3 an offset (saved at sp+40) and a4 a length
(s0). `gioio_info_get(a0)` and `gioio_info_mfast_get` give the bridge record, and its word at +20 is saved at sp+0. The static
routine at 0xa800000020040dc4 (the same lookup `giobr_piomap_alloc` uses, inside the range of `giobr_attach`) is called with a0, the window
index, the offset and the length; a zero result returns 0. Otherwise `xtalk_piotrans_addr(saved word, 0, helper result, a4, 0)`
is called and its result returned. No branch-likely instruction. **Established.**
- `heart_ithread_setup` (`6.5.30/unix.IP30`, occupies 108 bytes at address 0xa8000000200400bc): creates the interrupt thread
for one Heart level. a0 is not read; a1 is the level and a2 a priority. The entry is `heart_ivec` (through the pointer at 0xa8000000204a9720) plus 64 times the level, and it is
saved at sp+48. The priority used is the word `graphics_intr_pri` (0xa8000000204a70fc) when bit 0 of the entry word at +24 is set and a2 otherwise (`movz`). The routine
formats the thread name "heart_ivec(%d)" with `sprintf` into a buffer at sp+0 and calls `xthread_setup(buffer, priority, entry + 24,
0xa800000020040040, entry)`; the fourth argument is the pointer at 0xa8000000204a9750, which is `heart_intrd_start`. Its result is returned.
No branch-likely instruction. **Established.**
- `giobr_bridge_config` (`6.5.30/unix.IP30`, occupies 92 bytes at address 0xa800000020041d98): applies the GIO bridge
configuration. a0 is the bridge vertex (saved at sp+0) and a1 is passed on (saved at sp+8). `gioio_info_get(a0)` and `gioio_info_mfast_get` give
the bridge record. When the kernel word `showconfig` (0xa8000000204af7f0, read as the word at gp+4096) is non-zero it first calls `cmn_err(7,
"%v: Setting GIO bridge configuration\n", a0)`. In every case it then calls the static routine at 0xa800000020041df4 (the symbol table includes it in this routine's range; its
body is not read for this entry) with the doubleword at record +8 and a1, and returns that call's result. No branch-likely
instruction. **Established.**
- `xbow_update_llp_status` (`6.5.30/unix.IP30`, occupies 276 bytes at address 0xa80000002003d288): timer routine that folds
the crossbow's per-link status words into running totals and reports bad link status. Its argument a0 is a crossbow vertex,
saved at sp+0 and later passed on as a zero-extended 32-bit value. `hwgraph_fastinfo_get(a0)` gives the soft-state record (s3); the
doubleword at +16 is the register block (s5), the word at +8 the crossbow's own widget vertex and the address record + 320 the
start of a table of 16-byte total slots, one per link. The loop runs for link 0 to 7 with widget number 8 to 15 in s1. Every pass
first calls `xbow_widget_lookup(word at record +8, widget)` and then `xwidget_name_get` on its result, whether or not the link is used.
It reads two words of the link's 64-byte slot in the register block, at slot +300 (call it the status) and +316. If bit 31 of the
status is set, it adds the top byte of the +316 word to the first eight-byte field of the link's total slot and bits 16 to 23 of it to
the second eight-byte field, then (in that order: the +8 store first, then the +0 store in the delay slot of the next branch) stores both. When
the status AND 0x7fffffe7 is non-zero (checked only when bit 31 was set) it calls `cmn_err(4, "link %d[%s]: bad status 0x%x\n", link, name, status)`. After
the eighth link, if the word at record +452 is non-zero the routine reschedules itself with `timeout(self, a0 as an unsigned 32-bit
value, 100)`; the function pointer comes from the cell at 0xa8000000204a9700, which holds the routine's own address. With that word zero the
routine ends without rescheduling. The return value is not set by the routine: it is whatever the last call returned. No branch-likely
instruction. **Established.**
- `xbow_update_perf_counters` (`6.5.30/unix.IP30`, occupies 264 bytes at address 0xa80000002003cf74): timer routine that
accumulates the crossbow's two hardware event counters into 64-bit totals. a0 is the crossbow vertex (s8), kept to
reschedule. `hwgraph_fastinfo_get(a0)` gives the soft-state record (s4); the address record + 32 starts two 16-byte counter descriptors, and
the address record + 448 is a spinlock. For descriptor 0 then 1, a descriptor whose byte at +5 is zero is skipped. Otherwise the lock is
taken with `mutex_spinlock`, the counter register is read as the word the doubleword at descriptor +8 points to, and the
new count is that word AND 0xfffff. The delta is (new count minus the word stored at descriptor +0) AND 0xfffff, zero-extended
to 32 bits; it is added to the doubleword at record + 64 + 32 x (bits 20 to 22 of the register word) + 8 x (byte at descriptor +7). The new
count is stored at descriptor +0 in the delay slot of the `mutex_spinunlock` call, which releases the lock with the saved level. Afterwards
the routine calls `timeout(self, a0 as an unsigned 32-bit value, 4)` (the function pointer is the cell at 0xa8000000204a96f8, holding the routine's own
address) and returns its result. No branch-likely instruction. **Established.**
- `xbow_widget_present` (`6.5.30/unix.IP30`, occupies 100 bytes at address 0xa80000002003be48): says whether a widget's link
is up. It is a leaf with no frame. a0 is the crossbow register block and a1 a widget number. The link slot is the 64-byte slot
number (a1 AND 7) in the block. The routine loads the identification word at +4. A value 1 shifted left by its top nibble is formed and tested for zero; it never
is zero, so that branch cannot be taken. If bits 12 to 27 of the word equal 0xd100 and bits 1 to 11 are zero and a1 equals 15 it returns 1.
In every other case it returns the word at slot +316 ANDed with 0x20, that is 0 or 32. No branch-likely instruction. **Established.**
- `heart_flow_control_connect` (`6.5.30/unix.IP30`, occupies 132 bytes at address 0xa800000020040208): connects the handler of
a flow-control interrupt and leaves its level masked on every processor. a0 is a number n (kept as n + 1 in s7) and a1 the handler. It
calls `heart_intr_preconn(0, (n + 1) AND 0xff, a1, a0)`, so the level is n + 1, the handler is a1 and the handler's argument is the original a0. It then calls
`heart_imr_bits_rmw(register, 1 << (n + 1), 0)` (the shift is a 32-bit `sllv`, set up in the delay slot of the first call; a2 is zero) for the mask
registers 0, 1, 2 and 3 in that order, clearing that bit in each. It returns the last call's value, the previous contents of register 3. No branch-likely
instruction. **Established.**
- `heart_flow_control_disconnect` (`6.5.30/unix.IP30`, occupies 104 bytes at address 0xa80000002004028c): masks one flow-control
interrupt level on every processor. a0 is a number n; the bit is 1 << (n + 1) (a 32-bit `sllv`). It calls
`heart_imr_bits_rmw(register, bit, 0)` for registers 0, 1, 2 and 3 in that order, clearing the bit in each, and returns the last call's value (register 3's previous contents). It does not
touch the interrupt vector table. No branch-likely instruction. **Established.**
- `ioc3_serial_kill` (`6.5.30/unix.IP30`, occupies 64 bytes at address 0xa800000020142f18): stops one IOC3 serial port.
a0 is the port record (s0). When the doubleword at +8 (a2) is non-zero, the routine calls the function pointer stored at +40
of the structure that doubleword points to, through `jalr`, with a0 unchanged and a2 holding that doubleword (the result
is dropped; the delay slot is a nop). Whether or not it called, it then loads the doubleword at +64, stores a zero word at
its address (offset 0) and a zero word at record +148, and returns the +64 doubleword. Both exits return it: the
`ld v0,64(s0)` at 0xa800000020142f3c is reached by the taken branch and by the fall-through after the call. No
branch-likely instruction. **Established.**
- `pcibr_error_cleanup` (`6.5.30/unix.IP30`, occupies 84 bytes at address 0xa80000002004ca5c): clears the Bridge's pending error
state. a0 is the bridge soft-state record (s0). The halfword at +114 is the chip kind. For kind 0 or 1 the second argument is 65; for any other kind it is the
doubleword in the literal pool at 0xa8000000204a89d0, 0x00003cff00000041 (the load, from gp-24096, runs in the delay slot of the second compare and is overwritten by 65 when the
kind is 1). It calls `pcireg_intr_reset_set(record, that value, a2 unset, 1, kind)` and then `pcireg_tflush_get(record)`, whose result is returned. a2 is not set by this
routine, so it is whatever the caller left. No branch-likely instruction. **Established.**
- `xtalk_pio_addr` (`6.5.30/unix.IP30`, occupies 248 bytes at address 0xa80000002003a7ec): gets a processor-visible address for a
crosstalk window, using the cheap path when it can and a map otherwise. The arguments a0, a1, a2, a3, a4 and a5 are saved as s5, s4, s1, s0, s2 and s3. a4 is a pointer for an optional map result and a5 a flags word. A non-null a4 first
has a zero doubleword stored through it. The routine calls `xtalk_piotrans_addr(a0, a1, a2, a3, a5)`; a non-zero result is returned at once. Otherwise it calls
`xtalk_piomap_alloc(a0, a1, a2, a3, a3, a5)` (the fifth argument is a copy of a3), and a zero result returns zero. It then calls `xtalk_piomap_addr(map, a2, a3)`. A zero result calls `xtalk_piomap_free(map)` and
returns zero. A non-zero result is stored through a4 as the map when a4 is non-null (as a doubleword), and the routine returns the address. When a4 is null the map is neither
freed nor returned. No branch-likely instruction. **Established.**
- `pcibr_error_extract` (`6.5.30/unix.IP30`, occupies 120 bytes at address 0xa80000002004cab0): finds the slot a Bridge
error address belongs to. a0 is a vertex, and a1 and a2 are two values saved at sp+16 and sp+24 and passed on.
It calls `hwgraph_info_get_LBL(a0, "_pcibr_asic_rev", &value)`. A non-zero return gives 255 (the `addiu` is in the
delay slot, so 255 is set on both paths and the fall-through returns it). Otherwise `hwgraph_fastinfo_get(a0)` gives
the soft-state record, and a null record returns 255. Otherwise `pcireg_pci_bus_addr_get(record)` is called and
its result goes to `pcibr_addr_toslot` as the second argument, with the record first, a1 and a2 third and
fourth and 0 fifth; the answer ANDed with 0xff is returned. No branch-likely instruction. **Established.**
- `ioc3_is_io7` (`6.5.30/unix.IP30`, occupies 64 bytes at address 0xa800000020061428): reads one flag byte from an IOC3's
soft-state record. a0 is the PCI device vertex. `hwgraph_traverse(a0, "ioc3", &result)` is called; a non-zero return makes the routine return 0 (the
zero is set in the delay slot of the branch). Otherwise `hwgraph_fastinfo_get` of the vertex written to the result gives the record, and the
routine returns the byte at record +2880, zero-extended (the load is in the delay slot of the final jump to the exit). No branch-likely instruction. **Established.**
- `ioc3_is_console` (`6.5.30/unix.IP30`, occupies 88 bytes at address 0xa8000000200613d0): says whether an IOC3 is the one that
carries the console. a0 is the PCI device vertex. It first loads the doubleword at 0xa8000000204ae538 (a variable with no symbol of its
own; the nearest symbol below it is `usrpci_lock`); zero returns 0. Otherwise `hwgraph_traverse(a0, "ioc3", &result)` runs and a non-zero
return gives 0. Otherwise `hwgraph_fastinfo_get` of the vertex written to the result gives the soft-state record, which is compared (64-bit xor, then `sltiu` with 1)
with the doubleword at 0xa8000000204ae538, read again. The routine returns 1 when they are equal and 0 otherwise. No branch-likely instruction. **Established.**
- `heart_intr_disconnect` (`6.5.30/unix.IP30`, occupies 112 bytes at address 0xa80000002003fb9c): the reverse of
`heart_intr_connect`. a0 is the interrupt descriptor, saved at sp+0: the doubleword at +24 is the hook (s0), the byte at +5 the level (s3) and the
halfword at +36 the first argument of the mask routine. It calls `heart_imr_bits_rmw(halfword, 1 << level, 0)` (a variable 64-bit shift in the delay slot), which clears the
level's mask bit. It then rewrites the `heart_ivec` entry for the level (64 bytes apart, base through the pointer at 0xa8000000204a9720): the level is stored at entry +8 and the pointer at 0xa8000000204a9738,
which holds the address of `heart_stray_intr` (0xa80000002003ec1c), is stored at entry +0 (the second store is in the delay slot of the
test of the hook). If the hook is non-zero it is called with the descriptor as its argument and its result is returned; otherwise the result of `heart_imr_bits_rmw` is returned. No branch-likely
instruction. **Established.**
- `heart_intr_swlevel_to_hwlevel` (`6.5.30/unix.IP30`, occupies 84 bytes at address 0xa80000002003ea74): classifies a software
interrupt level into one of three hardware levels. The only argument is a0, compared as a signed number with two kernel words,
`swlevel_thread_hi` (0xa8000000204a776c) and `swlevel_thread_med` (0xa8000000204a7770). A negative a0 calls `cmn_err(0, "heart: unable to allocate level %d
interrupt\n", a0)` and returns -1; the fourth argument register holds the value of `swlevel_thread_med`, which the text does not use. Otherwise it returns 2 when a0 is not below the first word, 0 when a0 is below the second word and 1
otherwise (so values from the second word up to, not including, the first give 1). No branch-likely instruction. **Established.**
- `heart_addr_xio_to_kva` (`6.5.30/unix.IP30`, occupies 56 bytes at address 0xa80000002003f290): the same as
`heart_addr_xio_to_phys`, with the result placed in the uncached kernel window. It calls that routine with a0, a1 and a2 unchanged. A zero result is returned as zero. Any other result is ORed with
0x9000000000000000 (the constant is -7 shifted left by 60 with `dsll32`) and returned. No branch-likely instruction. **Established.**
- `pcibr_piomap_probe` (`6.5.30/unix.IP30`, occupies 80 bytes at address 0xa8000000200501c8): tries a read through a
Bridge PIO map and says whether it faulted. a0 is the map record, a1 an offset, a2 a length and a3 an output pointer. The doubleword at +24 is the map's
size; when offset plus length is above it (unsigned compare) the routine returns -1. Otherwise it calls the static routine at 0xa800000020050218
(the symbol table puts it under `pcibr_piomap_probe`) with a0 = the doubleword at +8 of the structure the doubleword at map +64 points to, a1 = the doubleword at map +32 plus the offset, a2
and a3 unchanged and a6 and a7 left as -1 and the map. The result is returned. That helper returns -1 for a length other than 1, 2, 4 or 8, or for an address that is not aligned to the length; otherwise it
calls the unlabelled routine at 0xa80000002005032c, then `badaddr_val(address, length, a3)`, then `pcireg_intr_reset_set(a0, 64)`, then the unlabelled routine at 0xa800000020050380 when the first call returned non-zero, and returns 1 when
`badaddr_val` reported a fault and 0 when it did not (`sltu` of zero below the value). No branch-likely instruction in either. **Established.**
- `pcibr_map` (`6.5.30/unix.IP30`, occupies 204 bytes at address 0xa8000000200430ec): maps a window of a Bridge's PCI space into the
caller's address space. a0 is a PCI vertex, a1 a map argument, a2 an offset and a3 a length. It gets the connection point with `hwgraph_connectpt_get(a0)`, the Bridge soft-state record with `hwgraph_fastinfo_get`,
loads the doubleword at +8 of the record (the window base) and drops the vertex reference with `hwgraph_vertex_unref`. The length is rounded up to a multiple of 16 KB ((a3 + 16383) AND -16384, saved at sp+0). It then calls
`v_mapphys(a1, a2 + base, rounded length, a3)` where the fourth argument register still holds the mask -16384. The result is kept in s0. When the halfword at record +114 (the chip kind) is non-zero, or when the result is non-zero, the
routine returns the result. For kind 0 with a zero result, it calls `pcireg_control_bit_set(record, 0x80000000, kind)` (0x8000 shifted left by 16 in the delay slot) when the offset a2 equals 0xc00000 or the rounded length is
above 0xc00000, and in every case it returns the `v_mapphys` result. No branch-likely instruction. **Established.**
- `xbow_bytes_to_gbr` (`6.5.30/unix.IP30`, occupies 100 bytes at address 0xa80000002003d600): returns the difference of two rounded-up quotients, one for a span's start and one for its end.
a0 is the start and a1 the length; there is no stack frame. The divisor is built by `lui` 0x3d and `ori` 0x900, which is 4,000,000. The start is divided by it with a signed 64-bit `ddiv` and the quotient is cut to 32 bits (a3).
When a3 times the divisor (a 32-bit `mult`, low word) is below a0 as a signed compare, a4 is a3 plus 1; otherwise a4 is a3 (the copy is in the delay slot of the branch at 0xa80000002003d628).
The same steps run on a0 plus a1 (a5): the quotient is a3 again, and a0 becomes a3 plus 1 when the product is below a5 (the `bne` at 0xa80000002003d650 jumps over the copy, with the add in its delay slot) and a3 otherwise.
The return value is the 32-bit difference a0 minus a4, formed in the delay slot of the `jr`. A zero length gives 0. There is one exit and no branch-likely instruction. **Established.**
- `ecc_compute_scdata_ecc` (`6.5.30/unix.IP30`, occupies 196 bytes at address 0xa800000020028a24): builds the ten-bit check code for a 128-bit secondary-cache data word.
a0 and a1 are the two doublewords of the word; they are kept in s3 and s4. The table `scache_data_ecc` (0xa800000020424228, 24-byte entries) is addressed from 0xa800000020423e38, so the loads at +1016 and +1024 read the doublewords at entry +8 and +16 and never the one at +0.
Nine passes run for entry 0 to 8. Each pass shifts s1 left one place (a 32-bit `sll`), ANDs entry +8 with a0 and entry +16 with a1, XORs the two, calls `ecc_bitcount` on the result and, when the count is odd, sets bit 0 of s1.
After the ninth pass `ecc_bitcount` is called again on s1 (cleared to 32 bits with `dsll32` and `dsrl32`) and an odd count sets bit 9 (0x200). So bits 0 to 8 are the nine row parities, the first row in bit 8 and the last in bit 0, and bit 9 is the parity of those nine bits.
s1 is returned. `ecc_bitcount` (0xa800000020028ae8) returns the number of one bits by clearing the lowest set bit until none is left. There is one exit and no branch-likely instruction. **Established.**
- `heart_dmamap_alloc` (`6.5.30/unix.IP30`, occupies 92 bytes at address 0xa80000002003f41c): allocates the 12-byte record that describes a Heart DMA mapping.
a0 is the widget vertex, saved at sp+0, and a3 the flags, saved at sp+8; a1 and a2 are not used. It calls `kmem_zalloc(12, f)` where f is 1 when bit 1 of a3 is set (`sltu` of zero below the masked bit, in the delay slot of the call). A null result is returned unchanged.
Otherwise the vertex is stored as a word at record +0 (the store is in the delay slot of the next call), then the unnamed routine at 0xa80000002003f0a4 is called with the vertex and its low byte, the widget number, is stored at +4. The flags a3 are stored as a word at +8 (in the delay slot of the branch to the exit).
The record is returned. That routine calls `xwidget_info_get` and `xwidget_info_id_get` and keeps the low byte of the id. No branch-likely instruction. **Established.**
- `heart_piotrans_addr` (`6.5.30/unix.IP30`, occupies 92 bytes at address 0xa80000002003f3c0): gives the kernel address of a programmed-I/O window of a Heart widget without making a map.
a0 is the widget vertex, a2 the crosstalk address, a3 the size and a4 the flags; a1 is not used. The unnamed routine at 0xa80000002003f0a4 turns the vertex into the widget number; `heart_addr_xio_to_kva(widget, a2, a3)` then gives the address (a2 is saved in s8 and a3 at sp+24, and a3 is reloaded in the delay slot of the call).
When bit 3 of a4 is set the address is ORed with 7 shifted left 59 (`dsll32` by 27), 0x3800000000000000. Unlike `heart_piomap_alloc`, the routine has no test for a zero address, so the bits are ORed into a zero result too. The address is returned. No branch-likely instruction. **Established.**
- `heart_flow_control_disable` (`6.5.30/unix.IP30`, occupies 60 bytes at address 0xa800000020040330): masks one flow-control interrupt level on the executing processor.
a0 is a number n and the bit is 1 shifted left by n plus 1 (a 32-bit `sllv`, kept in s8). The routine calls `heart_imr_bits_rmw(h, bit, 0)` where h is the halfword at absolute address 0xffffffffffffc3a0, the processor identity, so the bit is cleared in that one register.
The return value is the bit ANDed with the register's previous contents, so it is non-zero when the level was unmasked before. `heart_flow_control_enable` is the same routine with the set operand equal to the clear operand. No branch-likely instruction. **Established.**
- `pcibr_write_gather_flush` (`6.5.30/unix.IP30`, occupies 60 bytes at address 0xa800000020043a70): asks the Bridge for a write-buffer flush on behalf of the calling device, and returns 0.
It takes no argument of its own; a0 passes through to the first call unchanged. `pciio_hostinfo_get` gives a record (s8), `pciio_info_mfast_get` of it gives a value, and the unnamed routine at 0xa800000020043094 is called with that value and the byte at record +540 (loaded in the delay slot).
That routine's only call is to the register accessor pcireg_wrb_flush_get (at 0xa800000020058920), with the same two arguments. The result is dropped and 0 is returned. `pcibr_wrb_flush` differs: it passes the doubleword at +8 of the `pciio_info_mfast_get` result, not the result itself. No branch-likely instruction. **Established.**
- `pcibr_xintr_preset` (`6.5.30/unix.IP30`, occupies 176 bytes at address 0xa80000002004aca0): programs the Bridge interrupt destination, either the global one or a single pin's.
a0 is the Bridge soft-state record (s0), a1 a pin number, and a2, a3 and a4 are values for the destination. When a1 is -1 it calls, in this order, `pcireg_intr_dst_target_id_set(record, a2)`, `pcireg_intr_dst_addr_set(record, a3)`, `pcireg_intr_host_err_set(record, a4)` and `pcireg_intr_enable_bit_set(record, -256)`.
Otherwise it calls `pcireg_intr_addr_addr_set(record, a1, a3)`, `pcireg_intr_addr_vect_set(record, a1, a4)` and `pcireg_intr_enable_bit_set(record, 1 << a4)` (the 32-bit `sllv` at 0xa80000002004ad28 sits in the delay slot of the call). Both paths end with `pcireg_tflush_get(record)`, whose result is returned. No branch-likely instruction. **Established.**
- `pcibr_setpciint` (`6.5.30/unix.IP30`, occupies 168 bytes at address 0xa80000002004a77c): writes a PCI interrupt's destination address and vector into the Bridge.
a0 is a crosstalk interrupt record (s0). It calls, in order, `xtalk_intr_addr_get` (result saved at sp+24), `xtalk_intr_vector_get` (s8), `xtalk_intr_dev_get` (s7) and `xtalk_intr_sfarg_get` twice. The first sfarg result is read as a word whose low three bits are the pin (s1); the second is read again, shifted right 3 and ANDed with 1.
`pcibr_bridge_ptr_get(device, that bit)` gives the register block (sp+16). Then `pcireg_intr_addr_vect_set(block, pin, vector AND 0xff)` is called, and last `pcireg_intr_addr_addr_set(block, pin, address)`, whose result is returned. No branch-likely instruction. **Established.**
- `ioc3_pckm_mlreset` (`6.5.30/unix.IP30`, occupies 76 bytes at address 0xa800000020061a48): registers the IOC3 keyboard and mouse port with the console layer.
The routine takes no argument. It copies six code pointers from the literal pool into a 48-byte table on its stack, in the order issued: sp+0 gets the word at 0xa8000000204a9968, which is 0xa800000020061a94 (the first byte after this routine);
sp+8 gets 0xa800000020063334 (pool 0xa8000000204a9970); sp+16 0xa800000020063370 (0xa8000000204a9978); sp+24 0xa8000000200633ac (0xa8000000204a9980); sp+32 0xa8000000200633f0 (0xa8000000204a9988); sp+40 0xa80000002006341c (0xa8000000204a9990, stored in the delay slot of the call).
None of the six has a symbol of its own; the nearest symbol below each of the last five is `pckm_attach`. It then calls `pckm_console_register` with a0 set to the table's address (sp+0) and returns that call's result. No branch-likely instruction. **Established.**
- `xbow_enable_llp_monitor` (`6.5.30/unix.IP30`, occupies 76 bytes at address 0xa80000002003d3e4): starts the crossbow link-status timer and turns on its self-rescheduling.
a0 is the crossbow vertex (s8). `hwgraph_fastinfo_get(a0)` gives the soft-state record, kept at sp+16. It then calls `timeout(function, a0 as an unsigned 32-bit value, 100)`; the function pointer is the cell at 0xa8000000204a9700, which holds 0xa80000002003d288, the address of `xbow_update_llp_status`.
After the call it returns 0, and stores 1 as a word at record +452 in the delay slot of the `jr`, so the flag is set after the first timer is queued. That is the word `xbow_update_llp_status` tests before rescheduling itself. No branch-likely instruction. **Established.**
- `xbow_disable_llp_monitor` (`6.5.30/unix.IP30`, occupies 72 bytes at address 0xa80000002003d39c): stops the crossbow link-status timer from rescheduling and clears the link totals.
a0 is passed unchanged to `hwgraph_fastinfo_get` and the result is the soft-state record (a3). A loop runs for link 0 to 7; for each link it stores a zero doubleword at record + 16 x link + 328, then a zero doubleword at record + 16 x link + 320 (that store is in the delay slot of the loop branch).
These are the two fields of each 16-byte total slot that `xbow_update_llp_status` adds to. After the loop it returns 0 and stores a zero word at record +452 in the delay slot of the `jr`, which is the flag the timer routine tests. The timer already queued is not cancelled. No branch-likely instruction. **Established.**
- `xtalk_piotrans_addr` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003a7d0): the crosstalk entry point for translating a programmed-I/O address; it calls `heart_piotrans_addr` and nothing else.
A 16-byte frame holds only ra. The five arguments a0 to a4 reach `heart_piotrans_addr` untouched, and its return value comes back as this routine's. The only branch is the jump to the callee, whose delay slot is a nop; no branch-likely instruction. **Established.**
- `pciio_piotrans_addr` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002005f220): the generic PCI entry point for translating a programmed-I/O address; it calls `pcibr_piotrans_addr` and nothing else.
A 16-byte frame holds only ra. The arguments reach `pcibr_piotrans_addr` untouched, and its return value comes back as this routine's. The only branch is the jump to the callee, whose delay slot is a nop; no branch-likely instruction. **Established.**
- `xbow_widget_lookup` (`6.5.30/unix.IP30`, occupies 48 bytes at address 0xa80000002003bec4): finds the vertex of a crossbow port from its widget number.
a0 is the crossbow vertex and a1 the widget number, saved in s8 in the delay slot of the first call. `xswitch_info_get(a0)` gives the switch record; `xswitch_info_vhdl_get(record, a1 AND 0xff)` (the `andi` is in the delay slot) gives the port's vertex, which is returned. No branch-likely instruction. **Established.**
- `ecc_compute_sctag_ecc` (`6.5.30/unix.IP30`, occupies 144 bytes at address 0xa80000002002886c): builds the seven-bit check code for a secondary-cache tag word.
a0 is the tag word (s3). The table `sctag_eccm` (0xa8000000204241d0) is addressed from 0xa800000020423e38 with the load at +920, one doubleword per entry at eight-byte steps. Seven passes run for entry 0 to 6.
Each pass shifts s1 left one place (a 32-bit `sll`), ANDs the entry with a0, calls `ecc_bitcount` and sets bit 0 of s1 when the count is odd, so the first entry's parity ends in bit 6 and the last in bit 0. s1 is returned. There is one exit and no branch-likely instruction. **Established.**
- `heart_mp_intr_init` (`6.5.30/unix.IP30`, occupies 68 bytes at address 0xa80000002003edd4): writes the initial value of one Heart interrupt-mask register.
a0 selects the register, n. The routine stores one doubleword at 0xffffffffaff10000 + 8n, the same mask register array `heart_imr_bits_rmw` reaches. The value is the OR of 1 shifted left by n + 42, by n + 46 and by n + 59 (a variable 64-bit left shift), and bit 63 (-1 shifted left 32 and again 31 places).
The store is in the delay slot of the `jr`, and nothing else is stored. v0 is left holding the register's address; the routine sets no return value. There is no call, no branch other than the return and no branch-likely instruction. **Established.**
- `xbow_intr_preset` (`6.5.30/unix.IP30`, occupies 52 bytes at address 0xa80000002003bf60): fills two words of a record from a target and an address.
a0 is the record, a2 a four-bit value, a3 a 64-bit address and a4 a byte value; a1 is not read. The word at +52 is formed first: a2 shifted left 16 and masked with 0xf0000, ORed with a4 shifted left 24 and masked with 0xff000000, ORed with bits 32 to 47 of a3.
The word at +60 gets the low 32 bits of a3 (that store is in the delay slot of the `jr`). No value is returned and nothing is called. No branch-likely instruction. **Established.**
- `ioc3_subdev_disable` (`6.5.30/unix.IP30`, occupies 72 bytes at address 0xa80000002006199c): clears one bit of an IOC3's enabled-subdevice mask, atomically.
a0 is the PCI device vertex and a1 a bit number, saved at sp+8. The unlabelled routine at 0xa800000020061824 is called with a0 and 1; it returns the address of the doubleword that holds the mask, creating it when the vertex has none, or 0.
A zero result ends the routine. Otherwise the clear mask is the complement of 1 shifted left by a1 (a 32-bit `sllv`, then `nor`; bit 31 gives 0x000000007fffffff, which also clears bits 32 to 63), and a load-linked and store-conditional pair on the doubleword (`lld`, `and`, `scd`) repeats until the store succeeds.
The one branch-likely instruction is the retry at 0xa8000000200619d4, taken when `scd` leaves zero. The routine returns the address that helper gave, and 0 when it gave none. **Established.**
- `ioc3_subdev_enables` (`6.5.30/unix.IP30`, occupies 44 bytes at address 0xa80000002006192c): records the mask of enabled subdevices for an IOC3.
a0 is the PCI device vertex and a1 the mask, saved at sp+8. The unlabelled routine at 0xa800000020061824 is called with a0 and 1; it returns the address of the doubleword that holds the mask (created when absent), or 0.
A zero result ends the routine. Otherwise a1 is stored as a doubleword at that address. The routine returns the helper's result, the address or 0. No branch-likely instruction. **Established.**
- `heart_rev` (`6.5.30/unix.IP30`, occupies 40 bytes at address 0xa8000000200401e0): returns the Heart chip revision, reading it from the chip once and caching it.
There is no argument. It loads the word `the_heart_rev` (0xa8000000204a7754, gp - 28828), whose stored initial value is -1. When it is not negative (`bltz` at 0xa8000000200401e4) it is returned as it is.
Otherwise it takes the pointer from the literal-pool word at 0xa8000000204a8938, which holds 0x9000000018000004, and loads a 32-bit word from that address. It shifts the word right 28 places (a logical `srl`, so the value lies between 0 and 15) and stores it in `the_heart_rev`, in the delay slot of the final `jr`, and returns the same value.
Nothing is called. No branch-likely instruction. **Established.**
- `pcibr_setwidint` (`6.5.30/unix.IP30`, occupies 132 bytes at address 0xa80000002004ac1c): programs the Bridge's interrupt destination from a crosstalk interrupt record.
a0 is the record (s0). It calls, in order, `xtalk_intr_target_get` (s7), `xtalk_intr_addr_get` (s8), `xtalk_intr_vector_get` (saved at sp+16) and `xtalk_intr_sfarg_get`, whose result (s0) is the soft-state record of the Bridge.
It then calls `pcireg_intr_dst_target_id_set(block, target AND 0xff)`, `pcireg_intr_dst_addr_set(block, address)` and `pcireg_intr_host_err_set(block, vector AND 0xff)`, with the Bridge record as the first argument each time, and returns the last result. No branch-likely instruction. **Established.**
- `xtalk_intr_connect` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003aa80): the crosstalk entry point for installing an interrupt handler; it calls `heart_intr_connect` and nothing else.
Only ra is saved, in a 16-byte frame. Nothing in a0 to a7 is changed before the jump to `heart_intr_connect`, whose delay slot is a nop, and whatever it returns is returned. No other branch and no branch-likely instruction. **Established.**
- `xtalk_intr_alloc` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003aa2c): the crosstalk entry point for allocating an interrupt descriptor; it calls `heart_intr_alloc` and nothing else.
Only ra is saved, in a 16-byte frame. Nothing in a0 to a7 is changed before the jump to `heart_intr_alloc`, whose delay slot is a nop, and whatever it returns is returned. No other branch and no branch-likely instruction. **Established.**
- `pcibr_ate_addr` (`6.5.30/unix.IP30`, occupies 64 bytes at address 0xa8000000200473e4): returns the address of one address-translation entry of a Bridge.
a0 is the soft-state record and a1 the entry number. The halfword at record +112 is compared with a1 as a signed number (the `lh` is the first instruction). When a1 is below it the routine calls `pcireg_int_ate_addr` and otherwise `pcireg_ext_ate_addr`; both calls get a0 and a1 unchanged (their delay slots are nops).
The accessor's result is returned. No branch-likely instruction. **Established.**
- `pciiox_init` (`6.5.30/unix.IP30`, occupies 32 bytes at address 0xa800000020042ac4): registers the handler for one numbered board family with `nic_vmc_add`.
The routine has no argument. It calls `nic_vmc_add` with the string at 0xa8000000203b09c8, read through the pool word at 0xa8000000204a97b8 (the text is "030-1062-"), and the code address 0xa800000020042ae4 read through the pool word at 0xa8000000204a97c0; that address is the first byte after this routine and has no symbol of its own.
The result is returned. No branch-likely instruction. **Established.**
- `heart_intrd_start` (`6.5.30/unix.IP30`, occupies 124 bytes at address 0xa800000020040040): entry code of a Heart interrupt thread that pins it to a processor and names it.
a0 is the thread's descriptor (s0). It calls, in order, `setmustrun` with the halfword at descriptor +16 (the load is in the delay slot); `xthread_set_func(t, 0xa80000002004000c, descriptor)` where t is the doubleword at absolute address 0xffffffffffffc020 and the function is `heart_intrd`, read through the pool word at 0xa8000000204a9748;
`fetch_kname(doubleword at descriptor +0, sp+0)`; and `strncpy(t + 832, name, 16)` with t loaded again. It then ORs 0x400 into the word at descriptor +24 with a load-linked and store-conditional pair (`ll`, `or`, `sc`), repeating through the branch-likely instruction at 0xa80000002004009c when the store fails.
Last it calls `ipsema(descriptor + 32)` and returns that call's result. **Established.**
- `heart_flow_control_enable` (`6.5.30/unix.IP30`, occupies 60 bytes at address 0xa8000000200402f4): unmasks one flow-control interrupt level on the executing processor.
a0 is a number n and the bit is 1 shifted left by n plus 1 (a 32-bit `sllv`, kept in s8). It calls `heart_imr_bits_rmw(h, bit, bit)` where h is the halfword at absolute address 0xffffffffffffc3a0, the processor identity, so the bit is cleared and set again in that one register, which leaves it set.
The return value is the bit ANDed with the register's previous contents, so it is non-zero when the level was already unmasked. `heart_flow_control_disable` passes 0 as the third argument instead. No branch-likely instruction. **Established.**
- `heart_intr_free` (`6.5.30/unix.IP30`, occupies 60 bytes at address 0xa80000002003fa8c): gives back an interrupt descriptor and its level.
a0 is the descriptor (s8), 48 bytes long, and the byte at +5 is its level. The unnamed routine at 0xa80000002003e988 is called with 1 shifted left by the level (a variable 64-bit left shift, in the delay slot); it ORs that bit into the doubleword `heart_ivec_free` (0xa8000000204a7758) with a load-linked and store-conditional loop inside that routine.
Then `kmem_free(descriptor, 48)` is called and its result is returned. This routine itself holds no branch-likely instruction. **Established.**
- `pcibr_unmap` (`6.5.30/unix.IP30`, occupies 120 bytes at address 0xa8000000200431b8): undoes `pcibr_map` for a Bridge, dropping the extra control bit when it is set, and always returns 0.
a0 is a PCI vertex. `hwgraph_connectpt_get(a0)` gives the connection point (s7; `sd s7,16(sp)` is in the delay slot), `hwgraph_fastinfo_get` of it gives the soft-state record (s0), and `hwgraph_vertex_unref` drops the vertex reference.
When the halfword at record +114 (the chip kind) is non-zero the routine returns 0 at once. For kind 0 it calls `pcireg_control_get(record)` and ANDs the result with 0x80000000 (0x8000 shifted left 16, a 64-bit result); a zero result returns 0.
A non-zero result calls `pcireg_control_bit_clr(record, 0x80000000)`, the mask still held in a1 from the AND, and then returns 0. This is the opposite of the `pcireg_control_bit_set` call in `pcibr_map`. No branch-likely instruction. **Established.**
- `pcibr_wrb_flush` (`6.5.30/unix.IP30`, occupies 60 bytes at address 0xa8000000200485d4): asks the Bridge for a write-buffer flush on behalf of the calling device, and returns 0.
It takes no argument of its own; a0 passes through to the first call unchanged. `pciio_hostinfo_get` gives a record whose byte at +540 is loaded into s8 (in the delay slot of the next call). `pciio_info_mfast_get` of the record gives a second record.
`pcireg_wrb_flush_get(doubleword at +8 of the second record, the byte)` is then called, its result is dropped and 0 is returned. `pcibr_write_gather_flush` passes the second record itself, through a wrapper at 0xa800000020043094, and not its +8 word. No branch-likely instruction. **Established.**
- `pciio_piomap_addr` (`6.5.30/unix.IP30`, occupies 40 bytes at address 0xa80000002005f1dc): translates a PCI programmed-I/O map to an address and caches the answer in the map.
a0 is the map (s8, saved in the delay slot of the call); the other argument registers pass through unchanged to `pcibr_piomap_addr`. Its result is stored as a doubleword at map +32 and returned. No branch-likely instruction. **Established.**
- `pcibr_enable_mst_timeout` (`6.5.30/unix.IP30`, occupies 36 bytes at address 0xa800000020050308): turns on the Bridge's master-timeout interrupt enable and says whether the enable register changed.
a0 is a vertex. `hwgraph_fastinfo_get(a0)` gives the record, and the unlabelled routine at 0xa800000020050380 is called with the doubleword at record +8 (the load is in the delay slot); its result is returned.
That routine reads the interrupt-enable register through pcireg_intr_enable_get, sets the bits 0x0000000400000800 (a literal-pool constant at 0xa8000000204a89e0) with pcireg_intr_enable_bit_set, reads the register again and returns 1 when the two readings differ and 0 when they are equal. No branch-likely instruction in either. **Established.**
- `ecc_cacheop_get` (`6.5.30/unix.IP30`, occupies 36 bytes at address 0xa8000000200279ec): fills in a cache-operation request and performs it.
a0 is a 32-bit code, a1 a 64-bit value and a2 the request block. Before the stack frame is made it stores a1 as a doubleword at block +0, then a0 as a word at +8. It then calls `cacheOP` with a0 set to the block (the move is in the delay slot) and returns that call's result. No branch-likely instruction. **Established.**
- `xtalk_dmamap_free` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003a930): the crosstalk entry point for freeing a DMA map; it calls `heart_dmamap_free` and nothing else.
The frame is 16 bytes and keeps only ra. The incoming arguments are not touched before `heart_dmamap_free` is called (nop in the delay slot), and its value is the one returned. There are no other branches and no branch-likely instruction. **Established.**
- `xtalk_dmatrans_addr` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003a9a0): the crosstalk entry point for translating a DMA address; it calls `heart_dmatrans_addr` and nothing else.
The frame is 16 bytes and keeps only ra. The incoming arguments are not touched before `heart_dmatrans_addr` is called (nop in the delay slot), and its value is the one returned. There are no other branches and no branch-likely instruction. **Established.**
- `xtalk_intr_free` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003aa64): the crosstalk entry point for freeing an interrupt descriptor; it calls `heart_intr_free` and nothing else.
This is a bare forwarder: ra is saved in a 16-byte frame, `heart_intr_free` is called with the arguments exactly as received (nop in the delay slot), and its result is returned. Nothing else is executed and no branch-likely instruction is present. **Established.**
- `giobr_piomap_addr` (`6.5.30/unix.IP30`, occupies 44 bytes at address 0xa800000020040f90): translates an offset in a GIO bridge's programmed-I/O map by rebasing it onto the crosstalk map.
a0 is the map and a1 an offset. The routine loads the doubleword at +40 (a4), the doubleword at +16 (a3) and then the doubleword at +48 (a0, the crosstalk map). The new offset is a4 plus a1 minus a3: the add comes first, the subtract is in the delay slot of the call.
It calls `xtalk_piomap_addr(crosstalk map, new offset)` with a2 unchanged, and returns its result. No branch-likely instruction. **Established.**
- `ioc3_init` (`6.5.30/unix.IP30`, occupies 88 bytes at address 0xa80000002006092c): registers the IOC3 PCI driver and three board-part handlers.
The routine has no argument. It calls `pciio_driver_register(4265, 3, name, 1)`, where 4265 is 0x10a9 and name is the pool-word string at 0xa8000000204a9930 plus 48, which is "ioc3_" (the add is in the delay slot).
Then it calls `nic_vmc_add` three times with the string at 0xa8000000203b5bd8 ("030-1155-") and the code address 0xa800000020060984 from the pool word at 0xa8000000204a9938; with that string plus 16 ("030-1657-") and 0xa8000000200609a0 from the pool word at 0xa8000000204a9940; and with the string plus 32 ("030-1664-") and the same 0xa8000000200609a0. The last result is returned. No branch-likely instruction. **Established.**

## Heart interrupt glue, board revision and cache stubs

- `heart_stray_intr` (`6.5.30/unix.IP30`, occupies 72 bytes at address 0xa80000002003ec1c): masks off an interrupt vector that has no owner and logs it.
a0 is the vector number; the routine copies it to s8 as a sign-extended word (`sll s8,a0,0`). It loads the executing processor's identity, the halfword at absolute address 0xffffffffffffc3a0, as the first argument and calls `heart_imr_bits_rmw(identity, 1 shifted left by the vector, 0)` (the shift, a variable 64-bit left shift, is in the delay slot), so the vector's bit is cleared from that processor's mask register and nothing is set.
Then it calls `cmn_err(6, string, vector)`, where the string is the rodata text "heart: stray interrupt at vector %d.\n" at 0xa8000000203afe70 (the pool word at 0xa8000000204a9728 plus 48), and returns the `cmn_err` result. The frame is 16 bytes and keeps ra and s8. There is no branch other than the return and no branch-likely instruction. **Established.**
- `heart_init` (`6.5.30/unix.IP30`, occupies 44 bytes at address 0xa80000002003ee18): registers the Heart's crosstalk widget driver.
The routine has no argument. It calls `xwidget_driver_register(0xc001, 0x36, name, 0)`, where name is the pool-word string at 0xa8000000203afea0 ("heart_", the pool word at 0xa8000000204a9728 plus 96; the add is in the delay slot), and returns that call's result. It makes no other call and has no branch other than the return and no branch-likely instruction. **Established.**
- `heart_intrd` (`6.5.30/unix.IP30`, occupies 52 bytes at address 0xa80000002004000c): body of a Heart interrupt thread: run the vector's handler once, then call `ipsema` on the descriptor's semaphore word.
a0 is the descriptor, kept in s8. It loads the handler pointer from the doubleword at descriptor +0 into at and calls it through `jalr at` with the doubleword at descriptor +8 as its only argument (the load is in the delay slot). Then it calls `ipsema(descriptor + 32)` (the address is formed in the delay slot) and returns that call's result. The frame is 16 bytes and keeps ra and s8. No loop, no other branch and no branch-likely instruction. The descriptor layout matches the one `heart_intr` and `heart_intrd_start` use. **Established.**
- `heart_set_cpu_intr` (`6.5.30/unix.IP30`, occupies 20 bytes at address 0xa800000020013f10): writes one bit to a Heart register at offset 32 from the Heart register base.
a0 is a bit number n. The routine builds the base 0xffffffffaff10000 (`lui v0,0xaff1`) and the value 1 shifted left by n with the word-sized variable shift `sllv`, then stores the value as a doubleword at base +32 (address 0xffffffffaff10020), the store being in the delay slot of the `jr`. v0 is left holding the base address; the routine sets no return value. No call, no frame, no branch other than the return and no branch-likely instruction. **Established.**
- `heartclock_intr` (`6.5.30/unix.IP30`, occupies 20 bytes at address 0xa80000002001823c): writes bit 50 to the Heart register at offset 40 from the base.
There is no argument. The routine forms 0xffffffffaff10000 in v0, builds 1 shifted left by 50 (`addiu at,zero,1` then `dsll32 at,at,18`) and stores that doubleword at base +40 (address 0xffffffffaff10028), in the delay slot of the `jr`. That is the same doubleword that `heart_intr` writes the cleared vector's bit to, so it clears the pending bit for vector 50; none of the four level masks of the `heart_intr_*` entry stubs includes bit 50 (this exclusion was read from the stubs below). v0 holds the base address on return. No call, no frame and no branch-likely instruction. **Established.**
- `ip30_intr_maint` (`6.5.30/unix.IP30`, occupies 48 bytes at address 0xa800000020013b68): raises software interrupt 0x200 when either of two per-processor flag words is non-zero.
There is no argument. It loads the word at absolute address 0xffffffffffffc560 (-15008) and, when it is non-zero, branches to the call (at 0xa800000020013b70; the `sd ra` in the delay slot has already run). Otherwise it loads the word at 0xffffffffffffc564 (-15004) into v0 and, when that is zero, returns at once with v0 zero. A non-zero second word falls through to the same call: `siron(0x200)` at 0xa800000020013b84, with the argument set in the delay slot, and the result of `siron` is returned. The frame is 16 bytes and keeps ra. What the two words mean is unknown: no entry here reads their writers. No branch-likely instruction. **Established.**
- `ip30_rad_halt` (`6.5.30/unix.IP30`, occupies 36 bytes at address 0xa800000020014264): calls the function pointer `rad_halt_func` when one is set.
There is no argument. It loads the doubleword `rad_halt_func` (0xa8000000204ae3a8) into a1; when it is zero the routine returns without a call and v0 is not written. Otherwise it calls it with `jalr a1` (nop delay slot) with no argument set by this routine and returns whatever the hook leaves in v0. The frame is 16 bytes and keeps ra. No branch-likely instruction. What the hook does is not read here. **Established.**
- `ip30_board_rev` (`6.5.30/unix.IP30`, occupies 8 bytes at address 0xa8000000200130c8): returns the board revision word.
The routine has no frame and no call: `jr ra` with `lw v0,-1104(gp)` in its delay slot, which loads the word `ip30_rev` (0xa8000000204ae3a0) sign-extended. No branch-likely instruction. **Established.**
- `heart_need_flush` (`6.5.30/unix.IP30`, occupies 8 bytes at address 0xa800000020011320): says that no cache flush is needed; it always returns 0.
`jr ra` with `or v0,zero,zero` in the delay slot. No argument is read and nothing else runs. **Established.**
- `heart_dcache_inval` (`6.5.30/unix.IP30`, symbol size 0, address 0xa800000020011300): empty routine, a bare `jr ra` followed by a nop in the delay slot. It reads no argument and leaves v0 as it was. `heart_dcache_wb_inval`, `heart_write_dcache_inval` and `heart_write_dcache_wb_inval` are further names of the same two instructions at the same address. No branch-likely instruction. **Established.**
- `heart_intr_low` (`6.5.30/unix.IP30`, occupies 48 bytes at address 0xa80000002001167c): entry stub for the lowest Heart interrupt level; it tail-jumps to `heart_intr` with the mask of vectors 0 to 15.
The first two instructions are `j heart_intr` and, in its delay slot, `dli a1,0xffff`. The jump leaves a0 and ra untouched, so the exception frame passes through and `heart_intr` returns to this stub's caller. The symbol's recorded size of 48 bytes also covers the three stubs that follow (`heart_intr_med` at 0xa800000020011684, `heart_intr_hi` at 0xa800000020011690, `heart_intr_err` at 0xa8000000200116a0), which are not reached from this one. No branch-likely instruction. **Established.**
- `heart_intr_med` (`6.5.30/unix.IP30`, occupies 12 bytes at address 0xa800000020011684): entry stub for the second Heart interrupt level; it tail-jumps to `heart_intr` with the mask of vectors 16 to 31.
`dli a1,0xffff`, then `j heart_intr` with `dsll a1,a1,16` in its delay slot, which shifts the 0xffff left by 16, so the mask has bits 16 to 31. a0 and ra are untouched. No branch-likely instruction. **Established.**
- `heart_intr_hi` (`6.5.30/unix.IP30`, occupies 16 bytes at address 0xa800000020011690): entry stub for the third Heart interrupt level; it tail-jumps to `heart_intr` with the mask of vectors 32 to 49.
`addiu a1,zero,-1` and `dsll32 a1,a1,14` leave all ones shifted left 46 places; the delay slot of `j heart_intr` holds `dsrl a1,a1,14`, a logical right shift, which gives 0x0003ffff00000000. a0 and ra are untouched. No branch-likely instruction. **Established.**
- `heart_intr_err` (`6.5.30/unix.IP30`, occupies 12 bytes at address 0xa8000000200116a0): entry stub for the highest Heart interrupt level; it tail-jumps to `heart_intr` with the mask of vectors 51 to 63.
`addiu a1,zero,8191` then `j heart_intr` with `dsll32 a1,a1,19` in the delay slot, which gives 0xfff8000000000000 (13 ones shifted left 51 places). The four masks cover 0 to 15, 16 to 31, 32 to 49 and 51 to 63, so bit 50 is in no level (see `heartclock_intr`). a0 and ra are untouched. No branch-likely instruction. **Established.**

## IOC3 bring-up, Xbridge and crossbar registration, and Bridge helpers

- `ioc3_mlreset` (`6.5.30/unix.IP30`, occupies 32 bytes at address 0xa800000020060840): remembers the console IOC3's two window addresses and registers the IOC3 console.
a0 and a1 are stored as doublewords, a0 at the global `console_cfg` (0xa8000000204a77f8) before the frame is made and a1 at `console_mem` (0xa8000000204a7800) in the delay slot of the call. The only call is `ioc3_sio_mlreset`, made with no argument set here, and its result is returned. The frame is 16 bytes and keeps ra. These are the two globals that `ioc3_attach` compares its windows with. No branch-likely instruction. **Established.**
- `ioc3_sio_mlreset` (`6.5.30/unix.IP30`, occupies 44 bytes at address 0xa800000020142310): hands the serial driver's console hooks to `sio_console_register`.
The routine builds a two-doubleword record at sp+0. The first doubleword (stored at 0xa80000002014231c) is the code address 0xa800000020144208, loaded from the pool word at 0xa8000000204ab328. The second (stored at +8 in the delay slot of the call) is the code address 0xa8000000201440b4, loaded from the pool word at 0xa8000000204ab330. Neither target has a function symbol of its own. It then calls `sio_console_register(sp)` and returns its result. The frame is 32 bytes and keeps ra at +16. No branch-likely instruction. What the two hooks do is not read here. **Established.**
- `ioc3_subdev_enable` (`6.5.30/unix.IP30`, occupies 68 bytes at address 0xa800000020061958): sets one bit of an IOC3's enabled-subdevice mask, atomically.
a0 is the PCI device vertex and a1 a bit number, saved at sp+8. The unlabelled routine at 0xa800000020061824 is called with a0 and 1 and returns the address of the doubleword that holds the mask, or 0; a zero result ends the routine. Otherwise the bit is 1 shifted left by a1 with the word-sized `sllv` (bit 31 would sign-extend into a doubleword with bits 31 to 63 set), and `lld`, `or`, `scd` on the mask doubleword repeat until the store succeeds. The one branch-likely instruction is the retry `beql` at 0xa80000002006198c; its delay slot is a nop. The return value is the helper's: the address, or 0. It is the opposite of `ioc3_subdev_disable`. **Established.**
- `ioc3_get_mem` (`6.5.30/unix.IP30`, occupies 32 bytes at address 0xa800000020061a28): returns the IOC3 memory-window pointer of a vertex.
a0 is the vertex, passed unchanged to `hwgraph_fastinfo_get`. The routine loads the doubleword at +24 of the record it returns and returns that, which matches the MEM window address that `ioc3_attach` stores at +24 of its state block. A null record would fault: there is no test. The frame is 16 bytes and keeps ra. No branch-likely instruction. **Established.**
- `xbridge_init` (`6.5.30/unix.IP30`, occupies 84 bytes at address 0xa8000000200420b0): registers the Bridge widget driver for the two Xbridge identities.
The routine has no argument. It calls `pcibr_debug(1, 0, string)` with the rodata text "pcibr_init()\n" at 0xa8000000203b0800 (the pool word at 0xa8000000204a9788; a1 is cleared in the delay slot of the call). Then it calls `xwidget_driver_register(0xd002, 36, name, 0)` and `xwidget_driver_register(0xc002, 54, name, 0)`, where name is the rodata text "xbridge_" at 0xa8000000203b0810 (the pool word plus 16, added in each delay slot). The pairs 0xd002 with 36 (0x24) and 0xc002 with 54 (0x36) are the same pairs that the `xbridge_attach` and `pcibr_alloc_all_rrbs` entries split into the widget id register's part-number field (bits 12 to 27) and its 11-bit field at bits 1 to 11, so the first argument is the part number and the second the value of that 11-bit field (inferred to be the manufacturer number); the second call's result is returned. The frame is 16 bytes and keeps ra. No branch-likely instruction. **Established.**
- `xbow_init` (`6.5.30/unix.IP30`, occupies 80 bytes at address 0xa80000002003b5fc): registers the crossbar widget driver three times.
The routine has no argument. Each call is `xwidget_driver_register(code, 0, name, 1)` with name the rodata text "xbow_" at 0xa8000000203af4e8 (loaded from the pool word at 0xa8000000204a96d8 with no added offset) and the fourth argument 1. The codes, in call order, are 0xd100, 0xd000 (set in the delay slot of the second `jal`) and 0 (set by an `or` before the third). The last call's result is returned. The frame is 16 bytes and keeps ra. No branch-likely instruction. The meaning of the three codes is not read here. **Established.**
- `xbow_open` (`6.5.30/unix.IP30`, occupies 52 bytes at address 0xa80000002003b968): permission check for opening the crossbar device; returns 0 when the caller has the capability and 1 when not.
a3 is passed as the first argument to `cap_able_cred`, and the second argument is 0x2000000000 (1 shifted left by 37, formed with `dsll32 a1,a1,5` in the delay slot of the call); a0 to a2 are not read. A zero result takes the branch at 0xa80000002003b980 to the path that returns 1 (that path ends with `addiu v0,zero,1` in the delay slot of a branch back to the return); a non-zero result falls through to the path that returns 0. The frame is 16 bytes and keeps ra. No branch-likely instruction. **Established.**
- `xbow_map` (`6.5.30/unix.IP30`, occupies 68 bytes at address 0xa80000002003b9a4): maps the crossbar's register block into a caller's address space.
a0 is the vertex, passed unchanged to `hwgraph_fastinfo_get`; a1 is saved in s8 and a3 at sp+16 (the store is in the delay slot of that call); a2 is overwritten and not read. The length is a3 plus 16383 ANDed with -16384, so it is rounded up to a multiple of 16384 (the AND is in the delay slot of the next call). The routine calls `v_mapphys(a1, doubleword at record +16, rounded length)` and returns its result. The frame is 32 bytes and keeps ra and s8. Nothing is checked before the call. No branch-likely instruction. **Established.**
- `xbow_gbr_to_bytes` (`6.5.30/unix.IP30`, occupies 24 bytes at address 0xa80000002003d664): converts a guaranteed-rate count to bytes by multiplying by 4,000,000.
a0 is the count. The multiplier is formed by `lui at,0x3d` and `ori at,at,0x900`, which is 4,000,000, the same divisor `xbow_bytes_to_gbr` uses. `mult a0,at` is the 32-bit signed multiply, and `mflo v0` returns its low word, so the product wraps at 32 bits. No call, no frame, no branch other than the return and no branch-likely instruction. **Established.**
- `xbow_get_llp_status` (`6.5.30/unix.IP30`, occupies 32 bytes at address 0xa80000002003d268): returns the address of the link-status area inside the crossbar's soft state.
a0 is the vertex, passed unchanged to `hwgraph_fastinfo_get`. The routine adds 320 to the returned record address (an `addiu`-style add, so the result is an address and not a loaded word) and returns it. The frame is 16 bytes and keeps ra. There is no null test. No branch-likely instruction. **Established.**
- `pcibr_asic_rev` (`6.5.30/unix.IP30`, occupies 104 bytes at address 0xa800000020043a08): returns the Bridge revision number stored as a label on a vertex, or -1.
a0 is the vertex. It calls `hwgraph_traverse(a0, ".master", sp+0)` (the string is the pool word at 0xa8000000204a97c8 plus 56) and returns -1 if that returns non-zero. Otherwise it calls `hwgraph_info_get_LBL(handle, "_pcibr_asic_rev", sp+8)` with the vertex handle read from sp+0 and the string at the pool word plus 16, keeps the result at sp+16, and drops the handle with `hwgraph_vertex_unref`. A non-zero label result gives -1; a zero result returns the word at sp+12, the low half of the doubleword the label call wrote (the machine is big-endian). The frame is 32 bytes and keeps ra. The label is the one `xbridge_attach` creates. No branch-likely instruction. **Established.**
- `pcibr_intr_bits` (`6.5.30/unix.IP30`, occupies 76 bytes at address 0xa800000020049bdc): turns a PCI interrupt pin mask into the Bridge interrupt-bit mask for one slot, or 0.
a0 is the device record, whose byte at +540 is the slot number (a6, loaded with `lbu`). a1 is a pin mask and a2 an upper bound. When the slot is not below a2 as a signed compare, it returns 0 (that exit is `jr ra` with `or v0,zero,zero` after it). Otherwise the result starts at 1 shifted left by the slot when a1 has bit 0 or bit 2 set (a1 AND 5 is non-zero), and at 0 when not. When a1 has bit 1 or bit 3 set (a1 AND 0xa), the bit 1 shifted left by (slot XOR 4) is ORed in (the `movn` at 0xa800000020049c0c). Reading the four pin bits as interrupt pins A to D, A and C map to the slot's own bit and B and D to its partner slot; that reading is inferred. No call, no frame and no branch-likely instruction. **Established.**

## Graphics board (Odyssey) attach as the kernel sees it

- `odsy_init` (`6.5.30/unix.IP30`, occupies 56 bytes at address 0xa80000002035f2fc): kernel start-up for the Odyssey graphics driver; it allocates the board table, resets the board counter and registers the widget driver.
The routine has no argument. It calls `odsyCheckBoardPointers` (nop delay slot), zeroes the word `OdsyBoardEnum` (0xa8000000204a85c4) with `sw zero,-25132(gp)` before the next call, and calls `xwidget_driver_register(0xc013, 35, name, 0)` with the name "odsy_" at 0xa800000020417a60 (the pool word at 0xa8000000204adf10 plus 192, added in the delay slot). The first two arguments follow the part-number and 11-bit-field split described for `xbridge_init`. The register call's result is returned. The frame is 16 bytes and keeps ra. No branch-likely instruction. **Established.**
- `odsyCheckBoardPointer` (`6.5.30/unix.IP30`, occupies 164 bytes at address 0xa80000002035f088): makes sure board number n has a state record, creating it when absent, and marks it.
a0 is the board number n (kept in s0; n shifted left 3 is saved at sp+0). The table is the pointer `OdsyBoardspp` (0xa8000000204a85b8, gp - 25144); its entry n is the doubleword at 8n. When the entry is non-null the routine skips to the marking. Otherwise it calls `kmem_zalloc(0xb4e0, 0)` (46304 bytes), stores the result in entry n (the store is in the delay slot of the zero test), and on a non-zero result stores n as a halfword at record +790 and reloads the entry. A zero result calls `assfail` with the expression text "0", the file-name string the assertion carries and line number 238 (the `daddiu` forming the first argument is in the delay slot); `assfail` ends in a jump to `_assfail`, so the marking is not expected to see a null entry.
Marking stores n as a word at record +240 and the word 1 at record +0xb468 (46184; this store is in the delay slot of the final `jr`). The routine sets no return value. The frame is 32 bytes and keeps ra and s0. No branch-likely instruction. **Established.**
- `odsyFindBoardAddr` (`6.5.30/unix.IP30`, occupies 196 bytes at address 0xa80000002035f7a4): returns the crosstalk window address of Odyssey board n, finding it through the early-mapping path when the board has no record yet.
a0 is the board number n (s0). It first calls `odsyCheckBoardPointers`, then loads entry n of `OdsyBoardspp`; when that is non-null it returns the doubleword at record +824 straight away. Otherwise it calls `xtalk_early_piotrans_addr(0xc013, 35, n, 0, 0x401000, 0)` (the 0x401000 is `lui` 0x40 and `ori` 0x1000, set in the delay slot) and returns 0 when the result is 0. A non-zero result is kept at sp+8 and the word at result +36 at sp+0 (stored in the delay slot of the next call). `odsyCheckBoardPointer(n)` runs, then the record is reloaded and receives the saved word ANDed with 15 as a byte at +793, the window address as a doubleword at +824 and n as a halfword at +790. The window address is returned (the move into v0 is in the delay slot of the jump to the exit). The frame is 48 bytes and keeps ra and s0. No branch-likely instruction. **Established.**
- `odsy_attach` (`6.5.30/unix.IP30`, occupies 360 bytes at address 0xa80000002035f488): attach routine of an Odyssey graphics board; it claims the next board number, creates the device vertex, maps the board's window and records its identity in the board record.
a0 is the widget vertex (s0). It calls `psema` on the semaphore at 0xa8000000204e73b8 (`GfxBoardInitSema`, priority 25), reads `OdsyBoardEnum` (0xa8000000204a85c4) into s1 as the board number, stores s1 plus 1 back (the store is in the delay slot of the next call) and releases the semaphore with `vsema`. It then calls `hwgraph_char_device_add(a0, "odsy", "odsy_", sp+0)`, with the strings at 0xa800000020417ad8 and 0xa800000020417a60, and ignores the result. `xtalk_piotrans_addr(a0, 0, 0, 0x401000, 0)` gives the window (s2). `odsyCheckBoardPointer(s1)` makes the record (s3 is entry s1 of `OdsyBoardspp`). The record receives the window as a doubleword at +824, the vertex as a word at +0xb438 and the halfword 2 at +788 (in the delay slot of the next call). `xwidget_info_get(vertex)` then `xwidget_info_id_get` gives a byte stored at +793; a second `xwidget_info_get` then `xwidget_info_masterid_get` gives a byte stored at +792.
The word at window +4 is shifted right 28 places; below 2 the routine goes straight to the reset test. At 2 or above it first calls `nic_vertex_info_set(0xa800000020039e3c, window + 4316, vertex)` (that address is `nic_access_mcr32`) and the unnamed routine at 0xa80000002035f334 with the record; a non-zero result from it logs `cmn_err(1, "odsy_attach: odsy_set_gfx_mfg_nic_name failed\n")`, whose text names what that unnamed routine is for (inferred). Then the word at 0xa8000000204a85c0 (gp - 25136), which `odsy_earlyinit` sets to 0, is compared with s1: when they differ the routine calls `odsyReset(s1)` first (a board other than the one that routine already reset, inferred). Both cases then call `odsyInitBoardGfx(s1)` and `odsyInitIntr(s1, vertex, vertex)` and return 0. There is no null test on the window. The frame is 48 bytes and keeps ra, s0, s1, s2 and s3. No branch-likely instruction. **Established.**
- `pcibr_dma_enabled` (`6.5.30/unix.IP30`, occupies 44 bytes at address 0xa800000020046964): asks the crosstalk layer whether DMA is enabled for the widget that backs a PCI device, always getting 1.
a0 is not touched before the first call: `pciio_hostinfo_get` (0xa80000002005fd5c) receives the caller's a0 and returns a record. That record becomes a0 of `pciio_info_mfast_get` (0xa80000002005fe64; the `or a0,v0,zero` is in the delay slot of its call), which is a two-instruction routine that returns the doubleword at its argument +40. The routine then calls `xtalk_dma_enabled` with a0 set to the 32-bit word at the address that call returned (`lw a0,0(v0)`, in the delay slot), and returns its result. `xtalk_dma_enabled` forwards to `heart_dma_enabled` (0xa80000002004037c), whose whole body is two instructions, `jr ra` and then `addiu v0,zero,1`, so the result is the constant 1. No conditional branch, no branch-likely instruction. **Established.**
- `pcibr_piomap_addr` (`6.5.30/unix.IP30`, occupies 44 bytes at address 0xa800000020044614): turns an offset within a Bridge programmed-I/O map into a bus address by rebasing it on the underlying crosstalk map.
a0 is the map record and a1 the offset. Three doublewords are loaded from the record: +40 (a4), +16 (a3) and +48, which replaces a0. a1 becomes (a4 + a1) - a3 (the `dsubu` is in the delay slot of the call), and a2 is passed on as it came. The routine calls `xtalk_piomap_addr(record+48, a1', a2)` with the record's +16 value also left in a3 and +40 in a4, and returns its result. `xtalk_piomap_addr` forwards to `heart_piomap_addr`, a five-instruction routine that returns the doubleword at its first argument +24, plus a1, minus the doubleword at +8. No branch-likely instruction. **Established.**
- `pciio_piomap_alloc` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002005f1a4): the generic PCI entry point for allocating a programmed-I/O map; it calls `pcibr_piomap_alloc` (0xa80000002004437c) and nothing else.
Only ra is saved. The call's delay slot is a nop, so every argument register reaches the callee unchanged, and its result is returned. There is no conditional branch and no branch-likely instruction. **Established.**
- `xtalk_intr_disconnect` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003aa9c): the crosstalk entry point for removing an interrupt handler; it calls `heart_intr_disconnect` (0xa80000002003fb9c) and nothing else.
All argument registers pass through unchanged (the delay slot of the call is a nop) and the result is returned. There is no conditional branch and no branch-likely instruction. **Established.**
- `xtalk_piomap_addr` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003a798): the crosstalk entry point for translating an offset in a programmed-I/O map; it calls `heart_piomap_addr` (0xa80000002003f3a4) and nothing else.
The delay slot of the call is a nop, so the arguments pass through unchanged and the result is returned. There is no conditional branch and no branch-likely instruction. **Established.**
- `xtalk_piomap_alloc` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003a760): the crosstalk entry point for allocating a programmed-I/O map; it calls `heart_piomap_alloc` (0xa80000002003f2c8) and nothing else.
The delay slot of the call is a nop, so the arguments pass through unchanged and the result is returned. There is no conditional branch and no branch-likely instruction. **Established.**
- `xtalk_piomap_free` (`6.5.30/unix.IP30`, occupies 28 bytes at address 0xa80000002003a77c): the crosstalk entry point for releasing a programmed-I/O map; it calls `heart_piomap_free` (0xa80000002003f388) and nothing else.
The delay slot of the call is a nop, so the arguments pass through unchanged and the result is returned. There is no conditional branch and no branch-likely instruction. **Established.**

### The same routines in the 6.5.22 builds

The 6.5.22 copies of the routines above. Each row is a routine whose bytes match a 6.5.30 routine on the same page; the note gives the SHA-256 prefix of the bytes (first 12 hex digits). A linked-kernel row is established when the whole body has no calls, address constants or gp accesses; the others are inferred, because the 6.5.22 target of each constant or call was not checked. An object row compares section bytes, not relocated code, so it is inferred. The comparison is in [version-diffs.md](version-diffs.md).

| Routine | Binary | Address | Size | Note | Label |
|---|---|---|---|---|---|
| `xbow_widget_present` | `6.5.22/unix.IP27` | `0xc00000000006eeb4` | 100 | says whether a widget's link is up, from the identification word at +4 of the crossbow block; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 26d2fae0aee3), at a different address; no calls, address constants or gp accesses in the body | Established |
| `xbow_bytes_to_gbr` | `6.5.22/unix.IP27` | `0xc000000000070a2c` | 100 | returns the difference of two rounded-up quotients of a span by 4,000,000; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 8cee067198e7), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `pcibr_errintr_group` | `6.5.22/unix.IP27` | `0xc00000000007be98` | 132 | turns a Bridge interrupt-status word into a small group code; same bytes as the 6.5.30 linked kernel (SHA-256 prefix ecd63d84c398), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `xbow_widget_present` | `6.5.22/unix.IP30` | `0xa800000020031b7c` | 100 | says whether a widget's link is up, from the identification word at +4 of the crossbow block; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 26d2fae0aee3), at a different address; no calls, address constants or gp accesses in the body | Established |
| `xbow_bytes_to_gbr` | `6.5.22/unix.IP30` | `0xa800000020033334` | 100 | returns the difference of two rounded-up quotients of a span by 4,000,000; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 8cee067198e7), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `heart_addr_phys_to_xio` | `6.5.22/unix.IP30` | `0xa800000020034e00` | 288 | translates a physical address into the Heart crosstalk form, or returns -1; same bytes as the 6.5.30 linked kernel (SHA-256 prefix f70aa1489f9f), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `heart_addr_xio_to_phys` | `6.5.22/unix.IP30` | `0xa800000020034f20` | 164 | converts a widget-relative crosstalk range into a physical address; same bytes as the 6.5.30 linked kernel (SHA-256 prefix b190aaed481d), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `heart_paddr_to_port` | `6.5.22/unix.IP30` | `0xa800000020035988` | 284 | classifies a physical address as a widget window and returns its widget number (0xfe or 0xff when none); same bytes as the 6.5.30 linked kernel (SHA-256 prefix 985d51392a50), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `pcibr_errintr_group` | `6.5.22/unix.IP30` | `0xa800000020041d08` | 132 | turns a Bridge interrupt-status word into a small group code; same bytes as the 6.5.30 linked kernel (SHA-256 prefix ecd63d84c398), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `ioc3_serial_kill` | `6.5.22/unix.IP30` | `0xa80000002012b038` | 64 | stops one IOC3 serial port record, calling the hook at +40 of the structure at +8 when that pointer is set; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 0d7c76339956), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `xbow_widget_present` | `6.5.22/unix.IP35` | `0xc00000000009aaa8` | 100 | says whether a widget's link is up, from the identification word at +4 of the crossbow block; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 26d2fae0aee3), at a different address; no calls, address constants or gp accesses in the body | Established |
| `xbow_bytes_to_gbr` | `6.5.22/unix.IP35` | `0xc00000000009c274` | 100 | returns the difference of two rounded-up quotients of a span by 4,000,000; same bytes as the 6.5.30 linked kernel (SHA-256 prefix 8cee067198e7), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `pcibr_errintr_group` | `6.5.22/unix.IP35` | `0xc0000000000a9254` | 132 | turns a Bridge interrupt-status word into a small group code; same bytes as the 6.5.30 linked kernel (SHA-256 prefix ecd63d84c398), at a different address; the body has a call, an address constant or a gp access, and the 6.5.22 target of each is not checked | Inferred |
| `pcibr_errintr_group` | `6.5.22/usr/cpu/sysgen/IP27boot/pcibr.o` | `0x9714` | 132 | turns a Bridge interrupt-status word into a small group code; same bytes as the 6.5.30 object (SHA-256 prefix ecd63d84c398) at a different offset; relocations not compared | Inferred |
| `xbow_widget_present` | `6.5.22/usr/cpu/sysgen/IP27boot/xbow.o` | `0x864` | 100 | says whether a widget's link is up, from the identification word at +4 of the crossbow block; same bytes as the 6.5.30 object (SHA-256 prefix 26d2fae0aee3) at the same offset; relocations not compared | Inferred |
| `xbow_bytes_to_gbr` | `6.5.22/usr/cpu/sysgen/IP27boot/xbow.o` | `0x23dc` | 100 | returns the difference of two rounded-up quotients of a span by 4,000,000; same bytes as the 6.5.30 object (SHA-256 prefix 8cee067198e7) at the same offset; relocations not compared | Inferred |
| `heart_addr_phys_to_xio` | `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` | `0x864` | 288 | translates a physical address into the Heart crosstalk form, or returns -1; same bytes as the 6.5.30 object (SHA-256 prefix f70aa1489f9f) at the same offset; relocations not compared | Inferred |
| `heart_addr_xio_to_phys` | `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` | `0x984` | 164 | converts a widget-relative crosstalk range into a physical address; same bytes as the 6.5.30 object (SHA-256 prefix b190aaed481d) at the same offset; relocations not compared | Inferred |
| `heart_paddr_to_port` | `6.5.22/usr/cpu/sysgen/IP30boot/heart.o` | `0x13ec` | 284 | classifies a physical address as a widget window and returns its widget number (0xfe or 0xff when none); same bytes as the 6.5.30 object (SHA-256 prefix 985d51392a50) at the same offset; relocations not compared | Inferred |
| `pcibr_errintr_group` | `6.5.22/usr/cpu/sysgen/IP30boot/pcibr.o` | `0x94c0` | 132 | turns a Bridge interrupt-status word into a small group code; same bytes as the 6.5.30 object (SHA-256 prefix ecd63d84c398) at a different offset; relocations not compared | Inferred |
| `ioc3_serial_kill` | `6.5.22/usr/cpu/sysgen/IP30boot/sduart.a(sio_ioc3.o)` | `0xc08` | 64 | stops one IOC3 serial port record, calling the hook at +40 of the structure at +8 when that pointer is set; same bytes as the 6.5.30 object (SHA-256 prefix 0d7c76339956) at the same offset; relocations not compared | Inferred |
| `xbow_widget_present` | `6.5.22/usr/cpu/sysgen/IP30boot/xbow.o` | `0x854` | 100 | says whether a widget's link is up, from the identification word at +4 of the crossbow block; same bytes as the 6.5.30 object (SHA-256 prefix 26d2fae0aee3) at the same offset; relocations not compared | Inferred |
| `xbow_bytes_to_gbr` | `6.5.22/usr/cpu/sysgen/IP30boot/xbow.o` | `0x200c` | 100 | returns the difference of two rounded-up quotients of a span by 4,000,000; same bytes as the 6.5.30 object (SHA-256 prefix 8cee067198e7) at the same offset; relocations not compared | Inferred |
| `pcibr_errintr_group` | `6.5.22/usr/cpu/sysgen/IP35boot/pcibr.o` | `0x99a0` | 132 | turns a Bridge interrupt-status word into a small group code; same bytes as the 6.5.30 object (SHA-256 prefix ecd63d84c398) at a different offset; relocations not compared | Inferred |
| `xbow_widget_present` | `6.5.22/usr/cpu/sysgen/IP35boot/xbow.o` | `0x818` | 100 | says whether a widget's link is up, from the identification word at +4 of the crossbow block; same bytes as the 6.5.30 object (SHA-256 prefix 26d2fae0aee3) at the same offset; relocations not compared | Inferred |
| `xbow_bytes_to_gbr` | `6.5.22/usr/cpu/sysgen/IP35boot/xbow.o` | `0x1fe4` | 100 | returns the difference of two rounded-up quotients of a span by 4,000,000; same bytes as the 6.5.30 object (SHA-256 prefix 8cee067198e7) at the same offset; relocations not compared | Inferred |
