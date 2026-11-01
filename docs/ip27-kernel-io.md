# IP27 kernel: I/O bridge, hub and device routines

Routine-by-routine notes on the IRIX 6.5.30 IP27 kernel image (`6.5.30/unix.IP27`, a linked 64-bit MIPS IV big-endian ELF whose text lives at 0xc000000000000000 and above). Every address is a virtual address in that image. "Record +N" is byte N of the structure a routine works on; structure and member names come from the kernel's own debug information and the offsets are checked against the instructions. The decompiler's output was used to navigate, never as the evidence: each claim was read from the disassembly. Labels follow `AGENTS.md` section 2.

## Kernel assertion failure

- `assfail` (`6.5.30/unix.IP27`, address 0xc00000000003a5c4): **established.** A 1084-byte routine with no stack frame that runs before `_assfail` and keeps the first
failing CPU's register state in the global `_assregs`. It stores the incoming `v0` at `_assregs`+8 unconditionally (0x3a5dc), then reads the word `panic_cpu` and, only
when it equals -1 (no CPU has failed yet), stores the CPU number into it (a halfword read from absolute address 0xffffffffffffc420, sign-extended, written by `sw` at
0x3a620) and saves the registers: `v1` at +16, `a3` to `a7` at +48 to +80,
`t0` to `t3` at +88 to +112, `s0` to
`s7` at +120 to +176, `t8` +184, `t9` +192,
`k0` +200, `k1` +208, `gp` +216,
`sp` +224, `s8` +232, `ra` +240, the
CP0 Status register (`mfc0 $12`) +248 (stored twice, at 0x3a65c and again at 0x3a9b8), the low multiply register +256, the high multiply register +264, the BadVAddr
register (`dmfc0 $8`) +272, the Cause register (`mfc0 $13`) +280, and the EPC register (`dmfc0 $14`) +288 and again +352. A register slot is the register number minus one
times eight, so `a0`, `a1` and `a2` (+24 to +40) are never stored: they carry the assertion text, file and line on to the next routine unchanged. Both paths end at the
same `j _assfail` at 0x3a9f8 (a jump, not a call; delay slot `nop`), so `assfail` never returns itself and
`_assfail` returns to the caller's caller.

## PCI bridge error reporting

- `pcibr_error_dump` (`6.5.30/unix.IP27`, address 0xc0000000000794d0): **inferred.** Prints the bridge's interrupt status to the console once per distinct status value
and, for each set bit from 8 to 63, a description line and the bridge registers that explain that bit. The argument (`a0`, kept in `s1`) is the per-bridge soft record
(`pcibr_soft_s`). It calls `pcireg_intr_status_get(soft)` and masks off the low eight bits (and with -256, 0x794e8 to 0x794f0); a zero result returns at once. Otherwise
it calls `test_and_set_int64(soft+0x840, masked)`, an atomic swap that returns the old word (`lld`/`scd` loop at 0xc0000000000480c0), and returns without printing when
the old word equals the new, so the same status is not repeated. Otherwise `cmn_err(1, ...)` prints `int_status is 0x%lx for %s` and `Dumping relevant %s registers` with
the masked status, the name at record +16 (`bs_name`) and the ASIC name stored at record +24 (`bs_asic_name`; passed as the address soft+24). It then scans bits 8 to 63
of the status saved at sp+72. Two filters depend on the halfword at record +114 (`bs_bridge_type`): for a nonzero type, bits 8, 16 and 31 are skipped; for type 2 and a
nonzero word at record +72 (`bs_busnum`), bits 17, 18, 19, 20, 21 and 23 are skipped as well. For each remaining set bit it calls `cmn_err(6, "\t%s\n", text)` where
`text` is the pointer at 1920+8*bit in the bit-description table that the table whose address is held in the global slot at `gp-17672` (0xc000000001437a88 in this image)
points to (bits 8 to 45 have texts, 38 in all, such as bit 9 "PCI to Crosstalk read request timeout", bit 16 "SSRAM parity error", bit 41 "internal RAM parity error", bit
45 "PCI-X split completion message parity error"), and then a compare tree on the one-bit value chooses the extra register dump with `cmn_err(6, ...)`. Read by running
that dispatch for every bit value and every combination of the type, the word at record +72 and the PCI-X flag (the low bit of the halfword at record +116): bits 8 and 10
to 15 print the PCI bus address pair (`pcireg_pci_bus_addr_get`, `pcireg_pci_bus_addr_addr_get`), and bit 8 only for type 0; bit 9 prints the response-buffer error
registers (`pcireg_resp_err_get`, `pcireg_resp_err_dev_get`, `pcireg_resp_err_buf_get`, then `pcireg_resp_err_get` again for the address); bit 16 prints the SSRAM parity
register (`pcireg_ssram_parity_get`, type 0 only); bit 22 prints, for type 2 only, the link-side error word through `print_bridge_errcmd(word, "Aux ")` and the link-side
address (`pcireg_linkside_err_addr_get`); bit 23 prints the command-word error through `print_bridge_errcmd(word, "")` and the bridge bus error address pair
(`pcireg_bus_err_get` twice) for types 0 and 1, and the link-side word and address as for bit 22 for type 2; bits 24, 25 and 27 print the command-word error and the bus
error pair for every type; bits 26 and 28 print the response-buffer registers (device, buffer and address) or, when the PCI-X flag is set, the PCI-X DMA request error
address and attribute registers (`pcireg_pcix_req_err_addr_get`, `pcireg_pcix_req_err_attr_get`), with the link-side word first for type 2, and bit 26 in the non-PCI-X
case also calls `pcibr_show_dir_state`; bit 29 prints the link-side word and, with the PCI-X flag set, the PCI-X request error pair; bit 30 prints the SSRAM parity
register for type 0 and the map-fault address (`pcireg_map_fault_get`) for types 1 and 2; bits 32 to 38 and 42 print the PCI-X bus error address, attribute and data
registers; bit 39 the PCI-X request error pair; bit 40 the arbitration register (`pcireg_arbitration_get`); bit 41 the internal RAM parity register
(`pcireg_parity_err_get`); bits 43 to 45 the PCI-X split-request address and attribute pair (`pcireg_pcix_pio_split_addr_get`, `pcireg_pcix_pio_split_attr_get`) and then
the PCI-X bus error triple; bits 17 to 21 and 31, and bit 22 for types 0 and 1, print only the description line, and the table has no description pointers from bit 46 up
(the words there are not addresses of text), so a set bit 46 to 63 would be passed to the print as a bad pointer; that these bits are never set is **inferred** from the
unused table words. After the scan it calls `pcireg_intr_multiple_get(soft)`; with bridge type 0 or no bits above 7 it returns, otherwise it prints
`%s Multiple Interrupt Register is 0x%lx` (level 6, ASIC name and value) and then a description line for each set bit 8 to 63 of that register. It returns no value.

- `pcibr_pioerror` (`6.5.30/unix.IP27`, address 0xc00000000007b0a0): **inferred.** The 3404-byte handler for a programmed-I/O error that the crossbar reports against a
PCI bridge. Its arguments are the bridge soft record (`a0`, kept in `s4`), a value passed through to the lower handlers (`a1`), an error mode
(`a2`: 0, 1 or
2) and the error record (`a3`, `s1`), whose doubleword at +24 is the failing crossbar address. It first calls `pcibr_debug(0x200, ...)` with the soft record and that
address. It then turns the address into a PCI space code, a device slot and an offset by comparing it with window limits chosen by the halfword at soft +114
(`bs_bridge_type`, tested against 2) and the word at +72 (`bs_busnum`, which moves the config-space window from 0x20000 or
0x21000 up to 0x820000 or 0x821000 and the per-device windows from 0x200000 to 0xa00000), with PCI memory ranges 0x40000000 to 0x7fffffff and
0x80000000 up (the upper bound is read from the global slots at `gp`), and then walks the soft record's per-slot arrays (slot records start at soft +256, 128 bytes each;
the slot range is the bytes at +56 and +57) looking for a mapping record whose base and size contain the address, counting a hit in the record's word at +72. It writes
the result into the error record: the word at +0 gains 0x20000000 when a slot was found and always 0xc00000, the halfword at +8 gets the slot and function, the
doublewords at +32 and +40 get the space code and the offset. A second `pcibr_debug(0x200, ...)` reports space, offset, slot and function. With mode 0 it calls
`pcibr_error_cleanup(soft, a1)`, returns 0 when no slot was found and otherwise returns `pciio_error_handler(soft+4's vertex, a1, 0, record)`. With a nonzero mode and a
space code of 0 it prints `XIO Bus Error at %s ... does not correspond to any PCI address` (`cmn_err`, level 6) and returns 7 (the `li v0,7` in the delay slot at
0x7bb8c). With a nonzero mode and a space found it returns `pciio_error_handler(vertex, a1, mode, record)`: 0 is returned as it is; 9 first prints
`PIO Error on PCI Bus %s` at `cmn_err` level 1; any other nonzero result with the word at `kdebug` set prints a register dump
(`pcireg_intr_status_get`, `pcibr_error_dump` when any status bit in the mask held at `gp-23392` is set, then
`raw_space`, `raw_paddr`, `device`,
`func` and `device_reg` lines at level 6); a result with bit 29 of the record's flag word set calls `pcibr_device_disable(soft, slot)`; mode 2 ends with
`pcibr_error_cleanup(soft, a1)`; the result of the handler is returned. The window arithmetic and the slot search were followed from the control flow, not traced
instruction by instruction, hence the grade.

- `pcibr_alloc_all_rrbs` (`6.5.30/unix.IP27`, address 0xc000000000077074): **established.** Programs the response read buffer (RRB) assignment of one half of a bridge's
eight buffers. Arguments: the bridge's widget vertex (`a0`), the half to program (`a1`, only its low bit is used), then the buffer counts for devices 0 to 3 as the pairs
(`a2`, `a3`), (`a4`,
`a5`), (`a6`, `a7`) and the two stack words at 244(sp) and 252(sp) of the caller's frame (the ninth and tenth arguments). In each pair the first number is the device's
total count and the second is how many of those are virtual-channel buffers. It finds the bridge registers by calling `hwgraph_traverse(vertex, "pci", &out)` and, when
that succeeds and `hwgraph_fastinfo_get(out)` returns a soft record (`pcibr_soft_s`, kept in a local at 8(sp)), takes the register base from the record's doubleword at +8
(`bs_base`) after `hwgraph_vertex_unref(out)`; if the traversal fails or no base is found it uses
`xtalk_piotrans_addr(vertex, 0, 0, 8, 0)`
with no soft record. It refuses (returns -1) unless the widget identification word at base +4 names a Bridge (part 0xc002, manufacturer 54) or an XBridge (part 0xd002,
manufacturer 36), or devices 2 and 3 are given no buffers at all (the two counts of the third pair and the two stack words all zero); it also returns -1 when the sum of
the four device totals exceeds 8 or any of the four totals or the first, second and third virtual counts is negative. It builds a 64-bit image of eight 4-bit entries
filled from bit 32 downwards: for each device in turn, first one entry per virtual buffer (the device number or 8, with the virtual marker 1 shifted left by 2 for a
Bridge or XBridge, by 1 otherwise), then one entry per remaining buffer (the device number or 8), after subtracting the virtual count from the device total. With a soft
record it takes the lock at record +128 (`mutex_spinlock`) around the update; it calls `pcireg_rrb_set(base, a1 & 1, image)` and, with a record, sets 0x55 shifted by the
half in the word at +1308, zeroes four words at +1320, +1328, +1336 and +1344 (plus 4 times the half), stores 8 minus the sum of the totals at +1312 (plus 4 times the
half), and for each device the normal count (total minus virtual) and the virtual count into the words at +1384, +1388, +1416, +1420, +1448, +1452, +1480 and +1484 plus
16 times the half, then `mutex_io_spinunlock`. It returns 0 on success.

- `pcibr_slot_info_init` (`6.5.30/unix.IP27`, address 0xc00000000007c81c): **inferred.** Probes one device slot of a PCI bridge and builds the driver's description of
every function found there. Arguments: the bridge vertex (`a0`) and a slot number (`a1`). It gets the soft record from `hwgraph_fastinfo_get` and returns 0x16 (22) when
there is none or the slot is above the byte at record +57 (`bs_max_slot`); returns 0 when the slot record (at record +256 plus 128 times the slot) already holds a nonzero
word at its +256 offset; returns 1 when `pcibr_is_slot_sys_critical` is nonzero; returns 0x13 (19) when `pcibr_probe_slot` on the slot's configuration address fails or
the vendor halfword reads 0xffff. Otherwise it sets bit 4 in the slot's word at +268, reads the header-type byte (configuration offset 0xe) and, when its multifunction
bit 0x80 is set, probes functions 1 to 7 with `pcibr_func_config_addr` and `pcibr_probe_slot`, remembering which are empty in a mask; it allocates one 8-byte pointer per
function found (`kmem_zalloc`) into the slot's doubleword at +280 with the count at +272, and for each function: maps the header type (0 gives 6 base address registers, 1
gives 2; any other value is reported with `cmn_err(4, "...strange header type...")`), writes the cache-line size (configuration offset 0xc) as 0x20 when it is zero and
the bridge mode halfword at record +116 has bit 0 set, writes the latency timer (offset 0xd) from a table of 8, 16, 24 or
32 times the header's minimum-grant byte (offset 0x3e) according to the halfword at record +116 (values 7, 5, 2 or
3, otherwise 8) when it is zero, finds the PCI-X capability (`pcibr_find_capability(cfg, 7)`, a failure being reported with `cmn_err(4, ...)` because the bus runs in
PCI-X mode) and counts it in the words at +1288 and +1292, creates the function's info record with `pcibr_device_info_new` and registers it with
`pciio_device_info_register`, and for each base address register writes all ones, reads back the size, and reserves the bus address window with `pcibr_bus_addr_alloc`,
recording kind (2 for I/O, 4 or 5 for memory), base and size in the info record's 56-byte window entries at +72. Vendor 0x10a9 with class 2 or
3 devices get the two 16 MB device windows at record +2168 populated (`pciio_device_win_populate`) once, tracked by bits 0 and 1 of the word at +2168. The 624-byte frame
and the function-level loops were followed from the control flow and the calls, not instruction by instruction, hence the grade. It returns 0 after the last function.
- `pcibr_dmard_error` (`6.5.30/unix.IP27`, address 0xc00000000007bdec, 296 bytes, 80-byte frame): **established.**
Handles a DMA read-response error from the bridge: fills in the error record, passes it to the PCI I/O error handler, disables the offending device when the handler
reports a failure, clears the bridge's error interrupt and resets the failing read response buffer. Arguments: `a0` (`s0`) the bridge soft record (the word at +4 is its
vertex, saved at sp+72), `a1` (`s5`) and `a2` (`s8`) two values passed on to `pciio_error_handler`, `a3` (`s1`) the error record. It reads the failing buffer number
with `pcireg_resp_err_buf_get` (0xc00000000008118c; saved at sp+16) and the device with `pcireg_resp_err_dev_get` (0xc000000000081300). The record's halfword at +8 gets
the device number times 8, bit 25 (0x2000000) is ORed into the word at +0, and `pcireg_resp_err_get` (0xc000000000080e9c) stores its result in the doubleword at +40
before bit 22 (0x400000) is ORed into the word at +0. It then calls `pciio_error_handler(vertex, a1, a2, record)` (0xc000000000092fa0) and keeps the result at sp+64. A
nonzero result calls `pcibr_device_disable(soft, (halfword at +8 shifted right 3) & 0x1f)` (0xc00000000007b098). Whatever the result, it calls
`pcireg_intr_reset_set(soft, 16)` (0xc000000000083cec), then resets the response buffer: with the buffer number `b` (low word of the value at sp+16, kept at sp+24) it
reads `pcireg_rrb_get(soft, b & 1)` (0xc0000000000870e8), writes back that value with the 4-bit field number `b` / 2 (bits 4 * (b / 2) up, signed halving) cleared
(`pcireg_rrb_set`, 0xc0000000000872f8), and then writes the original value back with a second `pcireg_rrb_set`. The routine returns the `pciio_error_handler` result
(exit 0xc00000000007bf0c). The bit 25 and bit 22 flags and the meaning of the register fields are not named by the code; reading the buffer-register write sequence as a
reset of that buffer's entry is inferred.

## Cache error handling

- `ecc_handler` (`6.5.30/unix.IP27`, address 0xc00000000005614c): **inferred.** The R10000 cache-error exception worker: it classifies the error in a saved CP0
cache-error word, repairs what can be repaired and returns either a null pointer (recovered) or a pointer to a text that names the unrecoverable cause. Arguments: the
per-CPU register save area (`a0`) and the per-CPU error record (`a1`, `s`-saved), whose word at +24 holds the cache-error register image; the image's bit 30 (tested with
`<<0x22`) means several errors hit the same unit and returns at once the fixed text `CER=           Multiple cache errors in same unit detected` after `cer_to_hex` has
filled in the register value (the one return path with a known value, 0xc000000001410ad8). Otherwise it logs the image and the first doubleword of the save area into a
16-entry ring in the record (index word at +696 against count word at +700, wrapping with `& 0xf`), asks `eccf_recoverable_threshold(a0, a1, image)` whether the
recoverable-error budget is spent, and clears the record's doublewords at +8 and +16. It then branches on the two top bits of the image (mask 0xc0000000): 0 is a primary
instruction-cache error (counts it in the word at +40, reads the tag or data of each failing way through the cache-operation routine at 0xc0000000000559a0 with operation
codes 4 and 0, invalidates the line with `ecc_force_cache_line_invalid` for both ways, and returns the text
`too many primary instruction cache error exceptions` only when the threshold is spent and `failsoft_panic_check` agrees); 0x40000000 is a primary data-cache error
(counts it in the word at +44; with no error bits in the check masks 0x5500000 and 0xaa00000 it returns `unrecoverable dcache error, neither way` or
`unrecoverable - dcache tag on inconsistent block` by bit 29 of the image; otherwise it reads each failing way's tag with operation code 5 and returns
`unrecoverable - dcache tag mod`, `unrecoverable - dcache tag` or `unrecoverable - dcache data` for the tag states it finds, or invalidates the line with operation code
1); 0x80000000 is a secondary-cache error (counts it in the word at +48 and returns `unrecoverable secondary cache error`); 0xc0000000 is a system-interface error (counts
it in the word at +52; bits in mask 0x3800000 return `unrecoverable SIE`, no way bit returns `unrecoverable SIE, neither way`, otherwise each failing way's secondary tag
is read with operation code 7 and `memerror_handle_uce` is asked whether the memory error is recoverable: when it is, eight `clear_sline` calls (16 bytes apart) scrub the
line, when not and the state is 3 it returns `unrecoverable SIE - dirty scache`, and otherwise the line is invalidated through `ecc_force_cache_line_invalid`; with the
threshold spent it prints `Unrecoverable Interface error: Suspect memory address 0x%x At %s Bank %d` through
`cmn_err_tag(0xb96, 4, ...)` naming the node from
`nasid_to_compact_node` or `a remote partition`, and returns `too many interface error exceptions.`). Any other value returns
`unrecoverable - unknown cache error`. On the way out it stores the first two saved doublewords into the ring entry, sets a byte in the processor-private page, and when a
reason text exists and the record's byte at +38 is not 2 it sets it to 2 and copies 44 doubleword pairs of the save area to offset 1080. It calls
`save_cache_error(image, record+8, record+16)` after every path that did not return early. The cache-operation encodings and the threshold arithmetic were not traced
instruction by instruction, hence the grade.
- `ecc_exception` (`6.5.30/unix.IP27`, address 0xc0000000000236d0, 432 bytes, no frame): **established.**
The kernel's cache-error exception body: it saves the interrupted state into a fixed save area at absolute address 1152 (0x480), runs the cache-error handler, panics if
the handler reports something it cannot recover, and otherwise restores everything and resumes with `eret`. It has no `jr ra` return: its only exit is the `eret` at
0xc00000000002387c. `k1` is set to 1152 and the stores that follow use it as the base (the addresses given are the absolute ones): `s0` to `s7` at +120 to +176, `a0` to
`a7` at +24 to +80 (all as doublewords), `gp` at +216, `sp` at +224 and `ra` at +240. It then forms the kernel global pointer 0xc0000000014d2170 in `gp` (the
instruction sequence at 0xc000000000023720 to 0xc000000000023734) and calls `tfi_save` (0xc00000000003a550). Coprocessor 0 reads are stored in the area: Status (`mfc0`
register 12) at +248, Cause (13) at +280, BadVAddr (`dmfc0` 8) at +272 and EPC (`dmfc0` 14) at +288 and again at +352. `s0` is kept as the area pointer and `s1` is the
address area + 360 (0x5e8), a second record: the byte at its +38 (0x60e) is set to 1 if it was zero, ErrorEPC (`dmfc0` 30) is stored at its +0, TagHi (29) in the word
at +32, TagLo (28) in the word at +28 and CacheErr (27) in the word at +24. The stack pointer is the doubleword at absolute address 4064 or, when that is zero, the
constant 4048 (the `li` at 0xc0000000000237a8 runs only when the branch at 0xc0000000000237a0 is not taken). It calls `ecc_handler` (0xc00000000005614c) with the area
as the first argument and the second record as the second. A zero result takes the resume path. A nonzero result (kept in `v0` and passed on as the first argument)
first loads `sp` from the doubleword at absolute address 0xffffffffffffc440, rebuilds `gp` and clears Status bits 3 and 4 (mask -25), then calls `ecc_panic`
(0xc000000000056924) with that result, the area and the second record; the code after it is the resume path, which would run only if `ecc_panic` returned. The resume
path writes TagHi and TagLo back from the saved words, calls `tfi_restore` (0xc00000000003a588) with `k1` set to the area pointer in its delay slot, writes ErrorEPC
back from the second record, reloads `a0` to `a7`, `s0` to `s7`, `ra`, the `at` register from +0, `sp` and `gp`, clears the byte at second record +38 (through `k0`),
loads `k0` from +200 and `k1` from +208 and executes `eret`. The registers `k0`, `k1` and `at` are not stored by this routine; the area's +0, +200 and +208 slots are
only loaded here.
- `eccf_recoverable_threshold` (`6.5.30/unix.IP27`, address 0xc000000000055f98, 188 bytes, no frame): **established.**
Rate-limits recoverable ECC reports: it counts events of four kinds inside a time window and returns 0 once one kind passes 100 events in the window, and 1 otherwise.
`a1` is a record and `a2` a 32-bit value (sign-extended) whose top two bits select the kind; `a0` is not used. The kind is `a2 & 0xc0000000`: zero uses the halfword
counter at record +712, 0x40000000 the one at +714, 0x80000000 the one at +716 and 0xc0000000 the one at +718. Any other masked value returns 1 with nothing changed
(the `bne` at 0xc000000000055fe0 reaches the `jr ra` at 0xc000000000056024 with `v0` still 1 from the first instruction). It reads the 64-bit value at the absolute
address 0x9200000001030100 (the `lui` 0x9200 shifted left 32 ORed with 0x1030100) and loads the doubleword at record +704. When that doubleword is zero or the new
reading minus it is not below 0x3938700 (60,000,000) the window starts again: the reading is stored at +704, the selected counter is set to 0 and the routine returns 1
(0xc00000000005602c to 0xc000000000056038, the counter store in the delay slot of the `jr ra`). Otherwise the selected halfword counter is incremented (kept to 16 bits)
and stored, and the routine returns 1 while the counter is at most 100 and 0 once it is 101 or more (a `movz` clears `v0`). No branch-likely instruction. That the
register at 0x1030100 is a free-running counter is inferred from its use as a difference against a stored copy; the code does not name it.

## Interrupt allocation and hub error handling

- `pcibr_intr_alloc` (`6.5.30/unix.IP27`, address 0xc000000000078480): **inferred.** Builds the 160-byte per-request interrupt record of a PCI bridge device. Arguments:
the device's vertex (`a0`, its soft record from `pcibr_info_get`), a device descriptor (`a1`, may be 0), the INTA to INTD bit mask
(`a2`, low four bits) and the crossbar interrupt vertex (`a3`). It names the pins for a debug line (`pcibr_debug(0x1000, ...)`, the words
` INTA`, ` INTB`, ` INTC`,
` INTD` or ` No INTs?`), allocates the record with
`kmem_zalloc(0xa0, 0)` and returns 0 when that fails. With no descriptor it takes the global
`default_intr_pri` and no target CPU; with a descriptor whose flags (`device_desc_flags_get`) have bit 2 set it skips the CPU work, and otherwise it reads the target
(`intr_target_from_desc`) and the software level (`device_desc_intr_swlevel_get`). It fills the record (+4 vertex, +16 mask, +24 soft record, +20 target halfword preset
to 0xffff, +104 the lock set up with `spinlock_init`), asks the soft record's method at +640 (`*(soft+0x140)`, a function pointer) which of the eight bridge interrupt
bits the pins map to, and for each bit set: gets or creates the bit's crossbar interrupt with `xtalk_intr_alloc_nothd` (stored with `compare_and_swap_int64` in the soft
record's per-bit array at +1640+16*bit; a failure when nothing was stored is reported with `cmn_err(1, ...)`, and a lost race frees the new one with `xtalk_intr_free` and
moves the device bits with `pcireg_intr_device_bit_clr` and `pcireg_intr_device_bit_set`), checks with `xtalk_intr_cpuid_get` that all bits interrupt the same CPU or the
target requested (a mismatch is reported with `cmn_err(4, "Conflict on where to schedule interrupts ...")` or
`Request to target PCI interrupts to CPU %d could not be satisfied`, using `vertex_to_name`), sets the bit in the record's word at +88, and appends a 32-byte list node to
the bit's handler list by a chain of `compare_and_swap_int64` calls, freeing it when an equal one is already present. It ends with a debug line naming the record,
descriptor, CPU and target and returns the record. The list-insertion races and the final scheduling helper at 0xc000000000078408 were followed from the calls and
constants only.
- `hub_ioerror_handler` (`6.5.30/unix.IP27`, address 0xc000000000027c50): **inferred.** The hub-chip entry for an I/O error against a node: arguments are the node vertex
(`a0`), the error kind (`a1`: 5,
6, 9 or 10), a pass-through argument
(`a2`) and the error record (`a3`). It looks up the node's label `_node` with
`hwgraph_info_get_LBL`; when that fails it prints `Unable to handle IO error: hardware graph not setup` through `printf` (a result of 8 from the branches below leads to
the same print). Kind 9 returns 9 and any kind other than 5, 6, 9 and
10 returns 4. Kind 5 (the read path) first translates the error address in the record's doubleword at +48 into a widget and offset: a large-window address is split by the
node ID at +10 of the label and compared with window limits from `gp`, a small-window address uses the doubleword at +16 and the table at +352, writing the widget into
the halfword at +6 and the offset into the doubleword at +24 and setting 0x41000000 in the record's flags; a record without bit 25 set returns 5. A widget number of 1
calls `hub_ioerror_local(vertex, kind, a2, record)`; otherwise, with `is_widget_pio_enabled(record)` zero, it returns 0 after dumping the record through `ioerror_dump`
when `error_state_get` is 3 (`No outbound widget access - `), and with PIO enabled it calls `hub_xp_error_handler(vertex, node, kind,
a2, record)`. Kind 6 and 10 skip the address translation: kind 6 follows the same local, disabled or crossbar split (local when the record's halfword at +6 is 1), kind 10
goes straight to `hub_xp_error_handler`. It returns the callee's result. Return values read from the exits: 4, 5,
9, 0 and the callees' results. The address-window arithmetic uses limits held in `gp` slots that were not resolved here.
- `hubiio_crb_error_handler` (`6.5.30/unix.IP27`, address 0xc000000000034a30): **inferred.** Walks the 15 crossbar request buffer (CRB) slots of one hub's I/O interface
and reports every slot that holds an error. The arguments are an opaque value for the block-transfer handler (`a0`, kept at 144(sp)) and the node's hub record (`a1`, kept
at 136(sp)), whose halfword at +10 is the node ID and whose lock at +796 is taken around the whole walk (`mutex_spinlock_spl`, released by `mutex_spinunlock`). Slot n has
its registers at the node's register page plus 0x400 + 32*n (first word), +8 and +16; a slot is looked at when bit 50 of the first word (`dsll ...,0xd` going negative at
0x34b28) is set. For such a slot the routine decodes the message type, the widget number (bits 20 to 22 of the second word), the source widget and the error address (the
first word shifted down by 2 and masked to 38 bits, then times 4), builds a local error record on the stack (flags 0x80000000, or 0xc1000000 plus 0x20000000 when the
slot's bit 0 is set; the second-word test `<<0xf` selects a simpler reading from bit 58), and, depending on the third word's bits, either hands the error to
`hub_ioerror_handler(opaque, 10, 1 or 3, record)`,
calls the handler table entry for the widget number (an indirect call through eight slots at 2152 bytes past a global table, selected by a 3-bit widget code: values 0 to
7 are used, other values print through `sn0_error_dump` and `cmn_err(0, "Fatal error (code : %d) on hub : %d")`), or ends in one of the fatal reports
`Fatal %s on hub : %d`, `Xtalk Packet with error bit set to hub %d`, `Uncorrectable data error detected on data from Craylink to node %d` and
`Unable to handle %s on hub %d`; a slot number outside 16 to 23 with the same bit set calls `assfail` with the failed condition that the transaction number lies between
the bridge DMA write request limits (assert at line 1804, file argument not copied). Every slot that was reported goes to `bte_crb_error_handler(opaque, flag, slot, record)`
and is freed with `hubiio_crb_free(hub, slot)`, and a counter at 152(sp) is incremented. It returns that count (the `ld v0,152(sp)` before the epilogue). The per-widget
handler table and the record layout were followed from the control flow only, hence the grade.
- `hub_dmatimeout_kick` (`6.5.30/unix.IP27`, address 0xc000000000029144): **inferred.** Recovers a hub whose DMA stalled by stopping the other CPUs, draining
block-transfer engines, resetting the hub's request buffers and fixing the flow-control register, run under the global lock `global_kick_lock`. Arguments: a compact node
number (`a0`) and a wait flag (`a1`): 0 uses `mutex_spintrylock` (and gives up with the lock unheld when it fails), nonzero uses
`mutex_spinlock`. It converts the node to a NASID through `compact_to_nasid_node`; on the first call for node 0 while the global at 0xc0000000014ca218 is nonzero and
`graph_done` is zero it fills in the node's hub register pointer from the master NASID's widget table. It clears `release`, records the kicker CPU from the
processor-private page (halfword at absolute address 0xffffffffffffc420), zeroes the 128-bit `entermask`, sets a bit for every other CPU whose `pdaindr` entry is not -1,
and calls `cpuaction(cpu, kick_action, 1)` for each (storing the CPU in `kicking`). It then waits for the block-transfer engines (`bte_wait_for_xfer_completion`), and
spins until `entermask` is empty or 30000000 ticks of the clock at `DAT_c0000000014cc150` have passed. If the timeout expires it reports
`CPU %d didn't arrive in kick_action.` for each missing CPU and `Timeout/IFDR kick failed. CPUs didn't all arrive.` (both `cmn_err`, the second at level 0). Otherwise it
raises the interrupt level (`splerr`), for each of 16 buffers saves and clears two words of the node's register page (+0x10c and +0xa4, the latter ORed with 0x40) in
per-node save areas at +0x4c38 and +0x4bf8, waits 5 microseconds (`us_delay`), calls `hub_merge_clean(nasid)`, waits 2, calls `hub_ifdr_fix_value(node)` (its result
chooses between the two console messages `HUB II_IFDR (0x%x) fixed. Value was 0x%x` and `HUB II_IFDR (0x%x) overflowed. Fatal`; a result of 0 or less goes to the fatal
text, via `cmn_err` levels 0x15 and 0x10), restores the saved words, sets
`release` to 1, resets `kicker` to 0xffff, waits 2 more microseconds, restores the level with `splxecc` and releases the lock. Return values read from the exits: 0 when
the try-lock fails, 1 after a complete kick and 0 on the other return paths. The `entermask` bit handling and the hub register offsets were followed from the control
flow, not instruction by instruction.

## Bridge attach

- `xbridge_attach` (`6.5.30/unix.IP27`, address 0xc0000000000703ac): **inferred.** Attaches a PCI bridge (Bridge or XBridge) found on a crossbar widget: argument `a0` is
the widget vertex. It logs with `pcibr_debug(4, ...)`, calls `hub_device_flags_set(vertex, 1)` and gets the register base from `pcibr_bridge_ptr_get(vertex, 0)`. It
returns -1 without attaching when the widget identification word at base +4 names a Bridge (part 0xc002, manufacturer 54) and bit 5 of `pcireg_stat_get` is clear, or when
`pcibr_hints_get(vertex, 0)` returns a hint record whose word at +12 is nonzero. Otherwise it adds the `pci` vertex (`hwgraph_path_add`), registers and starts the
`pci_xbridge_provider` (`pciio_provider_register`, `pciio_provider_startup`), adds the bridge's control device, reads the NIC address
(`pcireg_nic_addr`, `nic_bridge_vertex_info`), builds the label from the ID register (`pcireg_id_get`: the revision nibble from bits 31:28 and the part number from bits
27:12 shifted left by 4; 0xc0021 to 0xc0024 are Bridge revisions and 0xd0021 and
0xd0022 XBridge revisions in the strings it selects) and stores it with `hwgraph_info_add_LBL`. It then allocates a 0xab8-byte soft record (`kmem_zalloc`, `memset`,
attached with `hwgraph_fastinfo_set`) and fills it: widget vertex at +0, bridge vertex at +4, register base at +8, a copy of the vertex name at +16 (`vertex_to_name`,
`strlen`, `kmem_alloc`, `strcpy`), the ID word at +120, the highest slot byte at +57 set to 7 and the first slot at +56 to 0, the interrupt-bit method pointer at +640
(`pcibr_intr_bits` or the hint's), bridge type at +114 (1 for an XBridge, 0 for a Bridge), the internal ATE count at +112 (0x400 for XBridge, 0x80 for Bridge, with two
constant ASIC-name strings stored at +24), the mode and speed halfword at +116 (`pcireg_mode_get`, `pcireg_speed_get`), per-slot defaults from the hint record, per-slot
and per-pin tables (slot stride 128 bytes from +256, pin stride 56 bytes from +1640), and widget identity from `xwidget_info_get` and its accessors; it initialises the
lock at +128. It then registers the error handler (`xwidget_error_register(vertex, pcibr_error_handler, soft)`), clears the timeout register bits
(`pcireg_timeout_bit_clr(soft, 0x3ff)`, before revision 0xc0024), resets and re-arms the interrupt registers (`pcireg_intr_reset_set` 0x7f, `pcireg_intr_device_set`
0xffffffff), sets up direct mapping (`pcibr_directmap_init`), sets control bit 0x200000 and zeroes every internal ATE (`pcireg_int_ate_set`), builds the ATE allocation
maps (`rmallocmap`, `pcibr_ate_free`, and for a Bridge the external ATE RAM through `pcibr_init_ext_ate_ram`), allocates the bridge's own error interrupt
(`device_desc_dup`, `xtalk_intr_alloc`) and connects `pcibr_error_intr_handler` to it with
`pcibr_setwidint` (`xtalk_intr_connect`), enables the interrupt sources (`pcireg_intr_enable_set` with extra bits from a global mask, trimmed by revision), sets interrupt
mode 0 and flushes (`pcireg_tflush_get`), chooses the bridge control flags at +124 by revision (0x80000, 0x800 or
0x20000), creates three device windows (`pciio_device_win_map_new` and `pciio_device_win_populate`: 0x10000000 bytes with 0x100000 granule, a 0x100000 window with 0x4000
and a 0x40000000 window), registers a bridge-level info record (`pcibr_device_info_new`, `pciio_device_info_register`) and then runs, for every slot from the first to the
last, `pcibr_slot_info_init`, `pcibr_slot_addr_space_init`, `pcibr_slot_device_init`,
`pcibr_slot_guest_info_init`, then `pcibr_initial_rrb`, and (when the NIC string contains the marker string at 0x1b8 of the string block)
`pcibr_rrb_alloc_init(soft, 1, 0, 8)`,
then `pcibr_slot_call_device_attach` for each slot and `pciio_device_attach`. It returns 0 after attach. The ordering of those calls was checked against the call
addresses; the per-revision constants and the layout of the soft record beyond the offsets above were taken from the decompiler and not traced by hand, hence the grade.

## IOC3 serial and router set-up

- `ioc3_serial_attach` (`6.5.30/unix.IP27`, address 0xc00000000017682c): **inferred.** Sets up the one or two serial ports of an IOC3 I/O chip. Argument `a0` is the
chip's PCI vertex. It returns -1 unless `ioc3_subdev_enabled(vertex, 4)` or `ioc3_subdev_enabled(vertex, 5)` is true (the two serial sub-devices). Otherwise it maps the
chip's registers with
`pciio_piotrans_addr(vertex, 0, 8, 0, 0x20180, 0)`,
asks `ioc3_is_console(vertex)`, looks up two child vertices of the chip by name (`hwgraph_traverse` twice, the second giving the pointer
`hwgraph_fastinfo_get` returns) and allocates one 0x150-byte block (`kmem_zalloc`) holding two 0xa8-byte port records (port 0 at +0, port 1 at +0xa8). For each enabled
port it adds the port's hwgraph node (`hwgraph_path_add`, then `hwgraph_fastinfo_set`), initialises the port (`sio_initport(record, vertex, 0x1ff, console flag)`, the
flag being 1 only for port 0 of the console chip), allocates a 0x28-byte lock record (`kmem_zalloc`, `sio_init_locks`) whose byte at +0x1d is 3 for the console port and 2
otherwise, and sets the default baud word at record +128 to 0x2580 (9600). For the console chip it also records the port's `hwgraph_edge_get` target in a global array,
looks up a kernel option (`kopt_find`, `atoi`) and when it is positive stores it as the port's baud rate (and, for port 1, prints it through `printf`), and registers the
port as a console device (`cons_devs` and the other globals through `gp`). Each port also gets an inventory entry (`device_inventory_add(vertex, 4,
0x10, -1, 0, 0)`), the register-base fields at +56, +80 (base plus 0x20000) and the pointer at +0 to a method table. Port 0, when enabled, gets a 16 KB ring buffer
(`kvpalloc(1, 0x100,
0)`, `panic` when it fails, `blkclr` over 0x4000 bytes) split into receive at +0x68 and transmit at +0x60 halves 0x1000 apart, an interrupt connection
(`ioc3_intr_connect(vertex, 0x1ff, handler, port, ...)`), controller number 1 (`device_controller_num_set`) and `sio_make_hwgraph_nodes`; port 1 does the same with mask
0x3fe00 and controller number 2, using the ring's upper halves at +0x2000 and +0x3000. It returns 0. The remaining port-record fields and the routine at
0xc000000000176588 that is called for each port are unknown here.
- `router_init` (`6.5.30/unix.IP27`, address 0xc00000000001c084): **inferred.** Brings up one Craylink router node: arguments are the compact node number (`a0`), a value
kept in `s3` and an interface record (`a2`) whose word at +0 is the router's vertex and doubleword at +16 an address printed in errors. It allocates the 440-byte router
record on the node's own memory (`kmem_zalloc_node(440, 0x101, node)`), stores it at interface +24 and as the vertex label
`_router` (`hwgraph_info_add_LBL`), initialises the lock `routerinfolock` at record +404, fills in node, NASID (from
`compact_to_nasid_node`), a not-yet-set port byte 0xff at +6 and two timestamps from the clock at `DAT_c0000000014cc150`, and reads the router's status register at vector
0 into record +16 and the global-parameters register at vector 0x18 into record +352 (`router_reg_read`; a failure of either is reported with `net_errmsg` and
`cmn_err(1, "Router init read failed: ...")`). It forces the register's field (bits 28 to 35) to 0x20 when it is not, sets or clears bit 0 according to the kernel
variables `craylnk_bypass_disable` and `hasmetarouter`, masks with a global, ORs in 0xafe000000000 and writes it back with `router_reg_write(record, 0x18, ...)`. The
router's port count is taken from status bits 46 to 48 into the interface halfword at +2, and a status low nibble other than 1 ("Not a router") is a `cmn_err(4, ...)`
panic. It allocates a pointer table of one doubleword per node (`kmem_alloc_node`, failure goes to `assfail("rip->ri_vecarray", file argument not copied, line 1131)`) and the path-name
buffer (`kern_malloc_node(0x50)`, `assfail("rip->ri_name", ..., 1136)`), finds the router's board (`vertex_to_router_board`; a missing board is
`Couldn't locate board structure for %v`, and a board whose type nibble (byte 0x15a) is not 1 is `Incorrectly jumpered router board in module %M`), builds the path name
(`board_to_path`) and then initialises each of the six router ports 1 to 6 (`router_port_init`, which on failure gives
`Router port init failed, cnode %d, %v, p 0x%x, err %d <%s>` through `cmn_err(4, ...)`), building the port-presence byte at +8 from the status bits at 29+3*port. After
the ports it calls `router_set_fences`, `nic_vertex_info_set`, `router_acquire_info`, sets the record's state byte at +0 to 6, calls `build_router_invent_info` and
`sndrv_attach(vertex)`, stores `router_poll_ticks` in
`p_routertick` and returns the result of `router_reg_write` (the value `ld s0,32(sp)` restores). Both error returns of the exits (0x1c46c,
0x1c494) are `cmn_err` calls that do not return. The names of the register fields are inferred from how the values are masked.
- `ioc3_write_ireg` (`6.5.30/unix.IP27`, address 0xc000000000094fa4): **established.** Changes the interrupt-enable state of an IOC3 chip and keeps a software copy of it.
Arguments: the chip's soft record (`a0`, `s0`), an interrupt bit mask (`a1`) and an operation
(`a2`): 0 sets the bits, 1 clears them, any other value does nothing but take and release the lock. The record's doubleword at +24 is the register base (`s3`), its lock
at +2876 (`mutex_spinlock_spl` with the level in `splerr`, released by `mutex_spinunlock`), the shadow of the enable word at +2880 and a byte of flags at +2884 (bit 0
"poll mode on", bit 1 "poll timer wanted", bit 2 "timer queued"). Clear: it removes the mask from the shadow; if flag bit 0 is on, then it drops flag bit 1 when the
shadow has no bit left in 0x7fffff and, when the shadow has no bit in 0x18000000, drops flags 0 and 1 and writes the whole shadow to the register at base +32; in every
clear case it then writes the mask itself to base +36. Set: it ORs the mask into the shadow; if flag bit 0 is off and the shadow has a bit in 0x18000000 it turns bit 0 on
and writes 0x7fffff to base +36 (which clears those enable bits in the chip: they are then driven by software polling); if bit 0 is off after that, it writes the mask to
base +32; otherwise (poll mode) it writes the mask with its low 23 bits removed (`& 0xff800000`) to base +32, and when flag bit 1 is off and the shadow has a bit in
0x7fffff it sets bit 1 and, when bit 2 is off, bit 2 as well and calls `prtimeout_nothrd(master_procid, handler, record, 1)` to queue the poll. It returns the result of
`mutex_spinunlock`. Which of +32 and +36 is the chip's set and which its clear register is inferred from the values written.
- `router_set_fences` (`6.5.30/unix.IP27`, address 0xc00000000001bd1c, 276 bytes, 64-byte frame): **established.**
Clears, for every router port that is absent, its bit in the router's global register 16 and in each present port's register 0x18, then keeps the register 16 value in
the router record and writes it back. `a0` (`s0`) is the router record; the byte at its +8 is the port-presence mask (bit n for port n + 1). It reads register 16 with
`router_reg_read(record, 16, &sp+0)` (0xc00000000001b344). For ports 1 to 6 (`s3`, with `s2` the bit number): when the port's presence bit is clear it clears bit `s2`
of the value at sp+0 (an AND with the complement); when it is set it reads the port's register with `router_reg_read(record, (port << 16) | 0x18, &sp+8)`, clears in
that value the bit of every absent port (an inner loop over bits 0 to 5 testing the presence byte, again at sp+8 through `a2`) and writes it back with
`router_reg_write` (0xc00000000001b374) at the same register number. After the six ports it stores the value at sp+0 in the doubleword at record +368, writes it to
register 16 with `router_reg_write(record, 16, value)` and returns that call's result (exit 0xc00000000001be08). The name suggests the registers hold fence masks, one
bit per port; that, and that bit `n` is port `n` + 1, are inferred from how the presence byte is used.
- `router_port_hist_reselect` (`6.5.30/unix.IP27`, address 0xc00000000001bb34, 224 bytes, 80-byte frame): **established.**
Changes a two-bit field in one router port's first register and then writes zero to that port's register 0x10, running on the node the router is attached to. `a0`
(`s0`) is the router record, `a1` (saved at sp+24) a port number and `a2` (saved at sp+32) the new field value. The node is the halfword at record +2, and its record is
the doubleword at that index of the table behind `p_nodepda` (absolute 0xffffffffffffc400); when the halfword at +18774 of that node record is -1 the routine returns 22
at once (the 22 is set in the delay slot of the branch at 0xc00000000001bb6c). Otherwise it calls `setnoderun(node)` (0xc0000000000245dc) and keeps its result at sp+0,
with the upper 32 bits stored as a word at sp+8 and the low 16 bits (sign-extended) as a halfword at sp+12. It then calls `router_reg_read(record, port << 16, &sp+16)`
(0xc00000000001b344), clears bits 19 and 18 of the value (an AND with the complement of 0xc0000, formed with `lui` 0xc), ORs in `a2` shifted left 18 (`a2` is not
masked, so a value above 3 also sets higher bits) and writes the result back with `router_reg_write(record, port << 16, value)` (0xc00000000001b374). A second
`router_reg_write(record, (port << 16) | 0x10, 0)` follows; its result is kept at sp+48. The routine calls `restorenoderun` (0xc0000000000246d0) with the doubleword at
sp+8 and returns the result of that second write (exit 0xc00000000001bc0c; the early exit at 0xc00000000001bc08 is the one with 22). No branch-likely instruction. That
the field selects a history source and that register 0x10 holds the history are inferred from the routine's name; the code does not name them.
- `router_port_init` (`6.5.30/unix.IP27`, address 0xc00000000001ba70, 196 bytes, 48-byte frame): **established.**
Programs the first register of one router port, then clears its register 0x10 and reads back its register 0x88. `a0` (`s0`) is the router record and `a1` (saved at
sp+8) the port number; `s1` is the register number of the port's first register, the port number shifted left by 16. It calls
`router_reg_read(record, port << 16, &sp+0)` (0xc00000000001b344), clears bits 19 and 18 of the value (an AND with the complement of 0xc0000, formed with `lui` 0xc) and
sets bit 18 (0x40000), the same two-bit field that `router_port_hist_reselect` rewrites from a caller's value; the result is kept at sp+0. When the global word
`hasmetarouter` (gp+2324) is nonzero and the port is 2 and the halfword at record +344 is below 80 (signed compare), it also replaces the low 10 bits of the value (an
AND with -1024 then an OR) with the global word `rpparm_maxburst_val` (gp-29312), which is not masked. It then calls `router_reg_write(record, port << 16, value)`
(0xc00000000001b374), keeps that call's result at sp+40, calls `router_reg_write(record, (port << 16) | 0x10, 0)` and finally
`router_reg_read(record, (port << 16) | 0x88, &sp+0)`, whose value is dropped. The routine returns the result of the first write (the one `jr ra` is at
0xc00000000001bb2c). No branch-likely instruction. That the low bits are a burst limit is inferred from the global's name; the code does not name the fields.

## Node driver control calls

- `sn0drv_ioctl` (`6.5.30/unix.IP27`, address 0xc00000000006aa10): **inferred.** The control-call entry of the node driver. Arguments: the device vertex (`a0`), the
command (`a1`), a user buffer (`a2`) and a result word pointer (`a5`). Commands 8 to 22 index a 15-entry jump table at 0xc0000000003c52e0 (`a1 -
8`); commands 14, 20 and 21 and every command outside 8 to 22 go to
`sndrv_ioctl_common(vertex, cmd, arg, a3, a4, a5)` and return its result. The others, with the status codes read from the stores to the local result word (1, 5,
14 and 22; 0 on success): 8 (needs the capability bit tested by
`cap_able(1 << 37)`, else 1; device type from `device_info_get` must be 2, else 22) copies in 4 bytes, finds the vertex label `_mdperf` and calls `md_perf_node_enable`; 9
does the same checks and calls `md_perf_node_disable`; 10 and 11 (type 2) call `md_perf_get_node_count` and `md_perf_get_node_ctrl` through the same label and copy out
288 and 4 bytes; 12 (capability, type 2) finds the node's compact number through `hwgraph_connectpt_get` twice and
`nodevertex_to_cnodeid`, calls `memerror_get_stats` and copies out 32 bytes; 13 (capability, type 3) copies in 12 bytes, pins the thread (`setmustrun`), takes the lock at
the node record +680, runs `elsc_nvram_read` or `elsc_nvram_write` as flag bit 0 or 1 of the request says, and copies the data out for a read; 15 (capability, type 1)
copies in 112 bytes, finds the label `_rou_fence`, runs `rou_fence_op` on the node's CPU (`cnodetocpu`,
`setmustrun`, `restoremustrun`) and copies the 112 bytes out; 16,
17 and 18 call `check_router` and then `router_port_map_get` (6 bytes out),
`router_port_mfg_info_get` (8 bytes in, 8 out) and, from the `_nic` label when its text contains `Laser:`, a hexadecimal number
(`strtoull`) copied out as 8 bytes; 19 (capability, type 4) copies in 96 bytes to the node record at +488, runs `elsc_command` under the lock at +680 and copies 96 bytes
out from +584; 22 (type 2) reads the `_mon` label and, when the node's NIC string contains
`IP31`, stores bits 7:4 of the doubleword at +72 of the hub register page in the result word and otherwise stores 15. Whether a command that is not handled by its table
entry (14, 20, 21) is a real, rejected command was taken from the table; the layout of each request buffer beyond the lengths above is unknown.
- `ioc3_intr_connect` (`6.5.30/unix.IP27`, address 0xc0000000000956e0): **established.** Registers a handler for a set of IOC3 interrupt bits and starts a thread to run
it. Arguments: the chip's PCI vertex (`a0`), the interrupt bit mask (`a1`), the handler (`a2`), its argument
(`a3`), the vertex the thread belongs to (`a4`, kept in `s2`), a device descriptor
(`a5`) and a seventh value (`a6`). It looks up the chip's child vertex named `ioc3` with
`hwgraph_traverse`; if that fails it stops with `cmn_err(1, "ioc3_intr_connect(%v): ioc3_attach not yet called")`. The child's fastinfo (`hwgraph_fastinfo_get`) is the
chip's soft record. It picks a free handler slot: the word at soft +2872 is a bitmap of released slots; when it is nonzero the routine atomically (`ll`/`sc`,
branch-likely at 0x95764) clears its lowest set bit and uses that bit's index (found by a binary search on 16, 8, 4,
2 and 1 bits), repeating if another CPU took the bit first; when it is zero the routine atomically increments the counter at +2864 and uses the old value. Slot records
are 0x58 bytes each starting at soft +0: the routine stores `a6` at slot +128, the soft record at +80, `a4` as a word at +72, `a3` at +64,
`a2` at +56 and the mask at +48 (as a word). It then ORs the mask atomically into the claimed-bits word at +2868 (branch-likely at 0x95800); if any bit of the mask was
already claimed it prints `%v: trying to share ioc3 intr bits 0x%X` at `cmn_err` level 6 and calls `panic` with a fixed string. Otherwise it duplicates the descriptor
(`device_desc_dup`), takes its software level from `device_desc_intr_swlevel_get` (falling back to the global `default_intr_pri` when negative), names the thread
`ioc3[<slot>]` or, when `device_driver_getbydev` finds a driver for the device,
`ioc3[<slot>]<driver name>` (`device_driver_name_get`, `sprintf`), sets 0x300 in the slot's word at +88 atomically (branch-likely at 0x9588c), and calls
`xthread_setup(name, level, slot+88, thread function, slot+48)`; its result is the return value. The two other `cmn_err`/`panic` exits do not return.
- `hub_error_init` (`6.5.30/unix.IP27`, address 0xc0000000000324f4): **inferred.** Sets up error handling for one node's hub chip; the argument (`a0`) is the compact node
number and the routine returns no value that a caller uses. It starts with `hub_error_clear`. On node 0 it allocates, for every node, a 0x1938-byte error log
(`kmem_zalloc_node(0x1938, 1, node)`, stored in
`kl_error_log[node]`) and a 0x40-byte count block (`kmem_zalloc_node(0x40,
0x101, node)`, in `hub_err_count[node]`); a failure of either is
`assfail("kl_error_log[i] && hub_err_count[i]", file argument not copied, line 155)`. When the node is the one the running CPU belongs to (`get_compact_nodeid`), it asserts both blocks
exist (lines 164 and 165), clears the log's first word, initialises two spooling areas with `recover_error_init` at log +0x1810 and +0x1890, programs the hub's
error-spool registers (through global register pointers) with one of three bit patterns (0x1aaaaaa, 0x1555555, or 0x16555aa and
0x9aaa55) chosen by two kernel flags, clears bit 21 and bit 20 in the two words, sets the spool start and end pointers in the log (+0x1910 to +0x1928) when the kernel
variables for them are nonzero, stores a spool size of 0x1000 shifted left by a global minus one (or 0) at +0x1930, and connects the error interrupt with
`cause_intr_connect(6, hubpi_eint_handler, 1)`, a nonzero result being `cmn_err(0x14, "hub_error_init: cause_intr_connect failed on %d")`. For any other node it only
zeroes the two hub registers at +0x408 and +0x410 of that node's register page. For every node it then calls `hubii_eint_init` and `hubni_eint_init`; on node 0 it also
sets the log's halfword at +0x404 to 0xffff, its word at +0x400 to 0 and initialises the lock `error_dump_lock`; finally it stores 12000 into the per-CPU word at +0x4d1c.
The register-pointer globals and the meaning of the three bit patterns were not resolved.
- `hubpi_spool_get_next` (`6.5.30/unix.IP27`, address 0xc000000000033dac): **inferred.** Takes the next 8-byte entry from the hub's error spool for the CPU that is
running. Arguments: two result pointers (`a0` in `s0`, `a1` saved at 0(sp)). It gets the NASID
(`get_nasid`) and raises the interrupt level (`spl7`); the node's log block comes from
`kl_error_log[node]` and the per-processor-slot read and write positions are the doublewords at log +0x1910 and +0x1920 plus 8 times the processor-slot byte at +0x520 of
the CPU's private data. It returns 0 (after restoring the level with `splx`) when there is no log, no read position (word at +0x1910 is zero), or after a correction of an
overrun it finds nothing more to read. When the read position equals the write position it first refills: it loads the two spool register values from global register
pointers (selected by slot 0 or other), stores them through the two result pointers, clears a hub register unless bit 1 of the halfword at absolute address
0xffffffffffffc524 is set, stores a new write position, and calls `klsave_spool_registers(first, second, 2)`, returning 2. Otherwise it reads the doubleword at the hub
register page offset given by the read position into `*a0` (the page is `0x9600000000000000` plus the NASID shifted left by 32), advances the position by 8, wraps it back
by the spool size at +0x1930 when the advanced value is aligned to that size, restores the level, and returns 1. The register-pointer globals were not resolved, and the
exact refill and wrap arithmetic follows the control flow, not each instruction, hence the grade.
- `router_acquire_info` (`6.5.30/unix.IP27`, address 0xc00000000001b704): **inferred.** Refreshes what the kernel knows about one Craylink router (argument `a0`, the
router record) under the record's lock at +404 (`mutex_spinlock`, `mutex_spinunlock`). It remembers the byte at +401 (the "already acquired" flag). When that flag is 0 it
clears the byte at +7 (the set of ports found up), stamps the time from the clock at `DAT_c0000000014cc150` into +48, and, when bit 0 of the global-parameters image at
+352 is set, writes the image back with bit 0 cleared (`router_reg_write(record, 0x18, ...)`). For ports 1 to 6 whose bit is set in the presence byte at +8 it releases
the lock, calls `router_acquire_port_info(record, port, flag)`, adds the result to a running total and takes the lock again. It then ANDs the byte at +7 with the presence
byte, updates the LEDs (`router_set_leds`), sets or clears bit 0 of the image at +352 according to `craylnk_bypass_disable` and `hasmetarouter` (set only when both are
zero), and, when bit 0 is set, the flag was 0 and the total over all ports is 0, writes the image back to register 0x18. It releases the lock and returns nothing. Which
fields hold ports and flags is inferred from the access pattern.
- `hub_piomap_alloc` (`6.5.30/unix.IP27`, address 0xc00000000003b5a0): **inferred.** Allocates a window through which a widget's registers are reached from this node.
Arguments: the widget vertex (`a0`), a device argument (`a1`, unused), the crossbar address (`a2`), the size
(`a3`), the minimum size (`a4`) and flags (`a5`; bit 0 "may wait for a window", bit 3 "uncached/cache-coherent bit" 0x3800000000000000, low two bits a no-sleep mode). It
gets the widget number and its master from `xwidget_info_get` and its accessors and the node record from the vertex label `_node`. It returns 0 when `a3` is smaller than
`a4`. For an offset-plus-size no larger than 16 MB it uses the node's fixed per-widget window record (the record at the node label plus 0x30 times the widget number,
+0x10 is the returned handle) and, when flag bit 3 is set, ORs 0x3800000000000000 into the doubleword at +0x28. For larger requests (up to 512 MB when the offset is taken
modulo 0x20000000) it takes the lock at node label +0x318 and scans the six big-window records (0x30 bytes each from +0x328, flag word at +0x350): a record already
mapping the same 512 MB block of the same widget (base doubleword at +0x330, widget byte at +0x32c) just gets its use count at +0x34c incremented and is returned;
otherwise it takes the first free record, and if flag bit 0 is set it refuses with 0 once five waiters (word at +800) are queued, programs the hub's big-window register
for that record (page `DAT_c0000000014cc198`, slot offset 0x160 plus 8 times the record number, value from the widget number and address bits 33 to 29) and fills the
record (base, widget, vertex, address `DAT_c0000000014cc220` plus record times 0x20000000 plus 0x3800000000000000 when flag bit 3, use count 1, flags |1 or |5). With no
free record and no-wait set it returns 0; otherwise it sleeps on the semaphore at +0x310 (`sv_wait`) and retries. The return value is the address of the record (+0x328
style) or 0. The exits list shows 0 and record pointers.
- `hubni_error_handler` (`6.5.30/unix.IP27`, address 0xc00000000003319c): **inferred.** Reports a fatal network-interface error of the local hub and halts or resets the
node. Argument `a0` is the name of the detector (printed with `%s`); `a1` selects reset (nonzero) or spin (zero). If the global flag word at
`DAT_c0000000014cc348` has bit 37 set and the word at `DAT_c0000000014cc350` is zero it calls `cmn_err(0x15, ...)`. It raises the interrupt level (`splerr`), stores 0x9e
into the hub register page doubleword at +0x50 plus 8 times the slot byte when the per-CPU byte at absolute address 0xffffffffffffc523 is zero, sets the node panic flag,
prints the hub's NI state to the PROM log (`hubni_print_state(nasid, 0, ip27log_printf, 2)`), enters panic mode, and when `a1` is nonzero logs `Resetting the NI link` and
sets the global at `DAT_c0000000014cc1b0` to 2; it prints the router state, logs `Fatal Craylink error: NI error detected by %s`, switches to the PROM log, prints the NI
and router states again with `cmn_err` as the printer (level 4) and then, when `a1` is zero, calls `panicspin`; with `a1` nonzero it falls through to
`cmn_err(0, "Detected a link reset due to other partition reset")`. It returns nothing that a caller can use.
- `pcibr_error_intr_handler` (`6.5.30/unix.IP27`, address 0xc00000000007a768): **inferred.** The interrupt handler for a PCI bridge's own error interrupt; the argument
(`a0`) is the bridge soft record. It reads the interrupt status (`pcireg_intr_status_get`), logs it with `pcibr_debug(0x100, ...)` and does nothing more unless some bit
from 8 up is set. It then (1) calls a helper at 0xc00000000007a600 when the status has a bit in the mask held at `DAT_c0000000014cc610`; (2) for the Bridge revision
0xc0022 with status bit 21 set it reports the rev B data-correction erratum with `cmn_err(4, ...)` when the kernel variable `bridge_rev_b_data_check_disable` is nonzero,
and otherwise runs `ql_bridge_rev_b_war` and calls `panic`; (3) for each set bit from 8 to 63 in the status masked by `DAT_c0000000014cc618` it counts the occurrence in a
24-byte per-bit record at soft +0x8dc (count, time of first count, count at that time), compares the rate in events per clock tick (`lbolt`) with limits (more than 100
per tick on an XBridge, and for bit 18 more than 99 events at a higher rate than the best seen so far) and prints the rate warnings `%s: %s, Error rate %d/tick`,
`%s Disabling error interrupt type %s. Error count %d` and `%s: %s, Excessive error interrupts : %d/tick` (levels 4,
5 and 5); (4) clears bit 24 of the working status when the bus error register shows the address pattern 0xc00000 and the command-word error register 0x402000; (5) prints
the status and calls `pcibr_error_dump` when any bit of the working status is in `bridge_errors_to_dump` (kept in step with the variable `pcibr_pioerr_dump`) or, on an
XBridge of revisions whose low nibble is 1 or 2, bits 20 or 21 are set, and then ends with `panic("PCI Bridge Error interrupt killed the system")` after
`machine_error_dump`, an exit that does not return; (6) if the status has a bit in the fatal mask at `DAT_c0000000014cc620` it does the same machine dump and panic.
Otherwise it clears the handled interrupt bits, using the group computed by `pcibr_errintr_group` for a Bridge (`pcireg_intr_reset_set`) or the status plus 0x40 for an
XBridge, sets arbitration bits 0xf00000 for one XBridge case (`pcireg_arbitration_bit_set`) and releases the error-interrupt interlock with
`test_and_set_int64(soft+0x840, 0)`. It returns nothing. The per-bit rate thresholds and the two branch-likely tests (0x7a8c4 and
0x7a944) were not followed in detail.
- `pcibr_reset` (`6.5.30/unix.IP27`, address 0xc000000000074688): **inferred.** Resets the PCI device in one slot of a bridge by pulsing the bridge's reset line for that
slot and restoring the device's configuration afterwards. The argument (`a0`, saved at 64(sp)) is the device's vertex. It gets the device's host info
(`pciio_hostinfo_get`) and the bridge soft record (`pciio_info_mfast_get`), takes the slot number from the byte at host info +540, and, when the slot record's word at
+256+128*slot is nonzero, replaces it with the byte at +260 of that record. A slot below the soft record's first slot (byte +49) or above its last (byte +51) is refused:
it logs `pcibr_reset unimplemented for slot %d` with `pcibr_debug(8, vertex, ...)` and returns -1 (the `li v0,-1` at 0x748dc). Otherwise it takes the bridge lock at +128
(`mutex_spinlock`) and for every function of the slot (count at slot +272, pointer array at +280) with an info record reads configuration register 4
(`pcibr_func_config_get`, 4 bytes) into a stack array; then pulses the slot's reset bit, `(1 << slot) << 24` in the bridge control register: `pcireg_control_bit_clr`,
`nano_delay` (a
1,000,000 ns delay),
`pcireg_control_bit_set`, `nano_delay`; then for every function writes back, through `pcibr_func_config_set`, the base address registers (configuration offsets 0x10 to
0x24, at most six per function, from the 56-byte window entries' base at +0x50, skipping zero ones) and finally the saved command register at offset 4. It releases the
lock with `mutex_io_spinunlock` and returns 0. The only values returned are 0 and -1.

## Interrupt entry

- `intr` (`6.5.30/unix.IP27`, address 0xc0000000000392d8): **established.** Picks the highest-priority pending processor interrupt and runs its registered handler.
Arguments: the saved-register frame (`a0`, kept in `s1`, passed on to the handler), a second value stored at 8(sp) (`a2`, the Status image) and the pending-interrupt mask
(`a3`, `s0`, a Cause image). A 636-byte routine with a 48-byte frame. If bits 4 and 5 of the word at absolute address 0xffffffffffffc048 are both set it calls
`check_delay_tlbflush(0)` and notes that in 0(sp); if bit 5 of the doubleword at absolute address 0xffffffffffffc280 is set it logs
`log_tstamp_event(102, 0, 0, 0, 0)`.
If `a2 & a3 & 0xff00` is zero it calls the routine at 0xc00000000003907c (the symbol table places it at `cause_intr_disconnect+0x40`) and the result is 0. Otherwise it
increments the word at +208 of the record whose pointer is at absolute address 0xffffffffffffc508, then tests the pending bits in this order: 0x4000, 0x800, 0x2000,
0x8000, 0x1000, 0x100,
0x400, 0x200. The table `cause_intr_tbl` (through the
`gp-19984` slot) has one 16-byte entry per bit position, entry n for bit 0x100<<n: the handler pointer at +0, the interrupt level at +8 and a result word at +12. For the
first bit found it calls `splx(level)`, takes the entry's result word as the return value and calls `handler(frame)`. When none of the eight bits is set it calls
`intr_stray(frame)` and the result is 0. On the way out it calls `check_delay_tlbflush(1)` if the first call was made and bit 4 of the word at 0xffffffffffffc048 is still
set, logs
`log_tstamp_event(101, 102, 0, 0, 0)`
when the 0x20 bit of the doubleword at 0xffffffffffffc280 is set, and returns the result word.
- `intrnorm` (`6.5.30/unix.IP27`, address 0xc00000000004c3a4): **inferred.** The general interrupt entry stub (1344 bytes, no stack frame, uses `k0` and `k1` and ends in
`eret` or a jump to `ecommon`); it is not a C function. Read as three parts. First, a guard at 0x4c3a4: when the byte at absolute address 0xffffffffffffc7d0 is not 1, the
doubleword at 0xffffffffffffc4e0 is nonzero and equals the one at 0xffffffffffffc018, and the exception PC (`dmfc0 k1,$14`) has its top bit clear (a user address), it
goes to 0x4c64c, a re-execution workaround: it probes the TLB for the page of EPC-4, reads the entry, checks the page is valid, loads the instruction word at EPC-4, and
when its primary opcode is 0 and its function bits 5:3 are 3 (the multiply and divide group) it stores that instruction into a patch slot (the page at the doubleword at
0xffffffffffffc7f0), flushes it with cache operations 0x15 and 0x10, runs it by `jalr`, compares the resulting multiply registers with the live ones and, depending on the
opcode of the instruction before (a jump, a branch or neither), moves EPC back by 4 or 8 and counts it in the words at 0xffffffffffffc7d8 and
0xffffffffffffc7d4 (the 8-byte case); the same test is made again for EPC-8 when EPC-4 is at the start of a 16 KB page. Second, at 0x4c3d0 a timer special case: when
Cause bit 15 (0x8000) is set, the counter-overflow bits of the performance counters read by the four `.word` coprocessor-0 reads are all clear (CP0 register 25 selects 1,
3, and for processors other than the one whose PRId field is 0x900 also 5 and 7) and bit 6 of the word at 0xffffffffffffc048 is set, it writes `Compare`
(CP0 register 11) with 0xfffffffff8000000 and returns with `eret`. Third, from 0x4c438: when Status.IE is clear, or no enabled interrupt (Status bits 15:8 and Cause) is
pending, it returns with `eret`; otherwise it chooses a stack and entry path by the state byte at absolute address 0xffffffffffffc04f (the byte at 0xffffffffffffc050 is
also read): when a thread context pointer at 0xffffffffffffc028 is nonzero it builds a register frame below the pointer at +136 of that context (saving `a0` to `a7`, `s0`
to `s7`, `ra`, `gp` and
`sp`, calling `tfi_save` and `uli_setasids`) and jumps to `ecommon_allsaved`; with state 2 it builds a 360-byte frame on the current stack, `k0` = 256, and jumps to
`ecommon`; with state 1 it switches to the interrupt stack held at 0xffffffffffffc030, records the old stack pointer at frame +224 and jumps to `ecommon` with `k0` = 2;
with state 3 it takes the interrupt stack, sets the byte at 0xffffffffffffc053 to 1 and goes to `ecommon_allsaved` when the byte at 0xffffffffffffc050 is nonzero and to
`ecommon` otherwise; with any other state it sets the state byte to 2 and takes a frame from the current process record, setting its `gp` to the kernel global pointer
(0xc0000000014d2170) before the jump to `ecommon`. The block at 0x4c4d4 (an `assfail` call) is never reached. The multiply and divide re-execution is inferred to be a
processor erratum workaround from its shape; no text states that. The addresses of the per-CPU words were read from the sign-extended 16-bit offsets in the instructions.
- `ioc3_intr_disconnect` (`6.5.30/unix.IP27`, address 0xc000000000095958, 356 bytes, 48-byte frame): **established.**
Removes the interrupt connections that match a caller's description from one IOC3's table and marks emptied entries free. `a0` is the PCI device vertex, `a1` (saved at
sp+8) a bit mask, `a2` (`s0`) and `a3` (`s1`) two doublewords that identify the connection and `a4` (`s2`) a word that does too. It calls
`hwgraph_traverse(a0, "ioc3", &sp+0)` (0xc00000000029579c). A nonzero result calls `cmn_err(1, "%v: ioc3_intr_connect: ioc3_attach not yet called", a4)`
(0xc0000000001fcbd4; the text names the connect routine) and returns. Otherwise `hwgraph_fastinfo_get` of the vertex at sp+0 (0xc000000000296098) gives the soft record.
The word at record +2864 is the number of table entries; a count of zero or less ends the routine. Entries are 88 bytes apart (the index times 88, built from shifts and
subtractions) starting at record +0, with the doubleword at entry +56 compared with `a2`, the doubleword at +64 with `a3` and the word at +72 with `a4`. When all three
match and the word at entry +48 ANDed with `a1` is nonzero, that AND result is cleared (`ll`/`sc` loops at 0xc000000000095a20, 0xc000000000095a34 and, below,
0xc000000000095a88, each retried by a branch-likely instruction) from the word at entry +48 and from the word at record +2868. When the word at entry +48 is then zero
it stores -1 in the word at entry +72, zeroes the doublewords at +64 and +56, and sets bit `index` in the word at record +2872. The scan does not stop at a match: it
continues to the last entry. The routine returns what `hwgraph_fastinfo_get` returned (the record pointer, left in `v0`; the error path leaves the `cmn_err` result),
which carries no meaning. That the record's three words at +2864 to +2872 are the entry count, an in-use mask and a free mask is inferred from how they are used.

## Crossbar, PCI window and router state helpers

- `xbow_get_bridge_pointers` (`6.5.30/unix.IP27`, address 0xc00000000006eb84): **inferred.** Fills a table of the register addresses of the I/O widgets that hang off one
node's crossbar (Xbow). Arguments: the compact node number (`a0`), a 16-entry table of doublewords (`a1`, kept in `s0`) and a pointer to a count
(`a2`, saved at 288(sp)). It converts the node to a NASID through the halfword table whose pointer is in the `gp-17944` slot, zeroes all 16 table entries, then looks up
the node's I/O vertex (`hwgraph_traverse(node's vertex at +20816 of its record, "xtalk", &v1)` and then `hwgraph_traverse(v1, "0", &v2)`); if either lookup fails it
returns 0 with the table empty. It gets the widget info of the second vertex (`xwidget_info_get`) and reads its part number at +16 and manufacturer at +24. Case 1: part
and manufacturer both 0 (no crossbar identified), or part 0xd000 or 0xd100 with manufacturer 0: it walks every edge of the first vertex (`hwgraph_edge_get_next`, starting
with a cursor of 2); for each edge's widget whose info says part 0xc002 and manufacturer 54 (a Bridge) or part 0xd002 and manufacturer 36 (an XBridge) it takes the widget
number from the byte at info +12 and stores `table[widget]` = the widget's register address, which is the NASID shifted left by 32 plus a global base (`gp-24504`,
0x92000000e0000000 in this image) when the widget number is 0 and otherwise the NASID shifted left by 32 plus 0x1000000 times the widget number plus 0x9200000000000000;
it also increments the count. Case 2: a single Bridge or XBridge directly as the vertex: `table[0]` gets the same computed address for its widget number and the count is
incremented. The function returns 0 in every path.
- `pciio_pio_addr` (`6.5.30/unix.IP27`, address 0xc0000000000929b4): **established.** The PCI layer's one-call way to get a host address for a device's PIO window.
Arguments: `a0` to `a4` are the device vertex, the device number, the space type, the offset and the size, `a5` points to a map slot (or is 0) and
`a6` is flags. It saves any existing map pointer from `*a5` and clears the slot. It calls `pciio_piotrans_addr(a0, a1, a2, a3, a4, a6)`; a nonzero result is returned at
once. Otherwise it needs a map: the saved one, or, when there was none, a new one from `pciio_piomap_alloc(a0, a1, a2, a3, a4, a4, a6)` (note that the size is passed
twice, as the size and the maximum), and a failed allocation returns 0. With a map it calls `pciio_piomap_addr(map, a3, a4)`; a nonzero address is returned and, when the
caller supplied a slot, the map is stored in `*a5`; a zero address frees the map only if this call allocated it (`pciio_piomap_free`) and returns 0.
- `router_print_state_specific` (`6.5.30/unix.IP27`, address 0xc00000000001c75c): **inferred.** Prints a router's error state once per reason. Arguments: the router
record (`a0`, `s0`), a first argument for the printer (`a1`), the printer function
(`a2`, called with `a1` and a format) and a one-bit reason mask (`a3`). Under the record's lock at +404 it tests the byte at +400: when any bit of the mask is already set
it unlocks and returns; otherwise it sets the bits, unlocks and carries on. If the node's byte at +18772 (reached through the node table at absolute address
0xffffffffffffc400) is nonzero it prints `vector unit busy for cnode %d.Waiting....`, calls `timeout_poll(nasid, 0x600300, 0x8000000000000000, max(256, 256), 1)` and
clears the byte. It then calls the routine at 0xc00000000001c63c (the symbol table places it at `timeout_poll+0x88`) and returns at once if that returns nonzero;
otherwise it prints `ROUTER ERROR STATE:`, the line
`/hw/module/%M/slot/%s`
with the module number (halfword +344) and slot name (`get_slotname` of byte +346), reads the router's register 0 into a local (`router_reg_read`) and prints it with
`show_rr_status_rev_id(value, presence byte at +8, a1, a2)`, and for every port 1 to 6 whose bit is set in the presence byte reads register `(port << 16) | 8` and prints
`show_rr_error_status(value & 0x3ffffffff, port, a1, a2, 0)`. It returns nothing a caller uses. What the routine at 0xc00000000001c63c checks is unknown here.
- `ioc3_handle_intr` (`6.5.30/unix.IP27`, address 0xc000000000095ad8): **established.** Dispatches the pending interrupts of an IOC3 chip to the handlers that
`ioc3_intr_connect` registered. Arguments: the chip's soft record (`a0`, or 0 to use the console chip's record held at `gp`+2840) and a flag
(`a1`). With no record at all it returns. It reads the handler count from the word at record +2864 and asks the routine at 0xc00000000009513c (it sits straight after
`ioc3_write_ireg` and has no symbol of its own; it takes the soft record and returns a mask, so it is read here as the pending-interrupt reader, which is inferred) for
the pending mask; a zero mask ends the routine. For a nonzero mask: with `a1` zero it clears bits 27 and 28 (and with 0xe7ffffff); with `a1` nonzero and bit 0 of the flag
byte at +2884 set (poll mode) it keeps only bits 23 and up (and with 0xff800000); a mask that is then zero also ends the routine. For each handler slot in turn (0x58-byte
records, mask word at +48): when the slot's mask meets the pending mask it either calls the handler directly with `handler(arg at +64, matching bits)` (the pointer at
+56) when bit 0x1000 of the slot's flag word at +88 is clear, or, when that bit is set (a threaded handler), calls the optional filter function at +128 with the argument
at +64 and, when there is no filter or it returns zero, calls the direct handler too, and otherwise masks the slot's bits off in the chip with
`ioc3_write_ireg(soft, slot mask, 1)` and wakes the thread with `vsema(slot+96)`; it counts each matched slot and removes the matching bits from the pending mask. After
the slots it adds the count minus one atomically (`ll`/`sc`, branch-likely at 0x95b68) to the word at +208 of the record whose pointer is at absolute address
0xffffffffffffc508 (the interrupt statistics), then, when the pending mask still has bits no slot claimed, prints `unknown IOC3 interrupt 0x%x` (`cmn_err` level 1), and
reads the pending mask again to loop. It returns nothing.
- `ioc3_attach` (`6.5.30/unix.IP27`, address 0xc000000000095228): **inferred.** Attaches an IOC3 I/O chip found on a PCI bus; argument `a0` is the PCI vertex. It maps the
chip's configuration space (`pciio_pio_addr(vertex, 0, 7,
0, 0x48, &map1, 0)`) and its memory space
(`pciio_pio_addr(vertex, 0, 8,
0, 0x20180, &map2, 0)`); a failure prints
`%v/ioc3: unable to get PIO mapping for my CFG space` or `... MEM space` (`cmn_err` level 1, freeing the first map with
`pciio_piomap_free` in the second case) and returns -1. It finds the `.host` and `.guest` parents of the vertex (falling back to the vertex itself) and creates an
`ioc3` vertex with edges from both. It allocates the 0xb48-byte soft record (`kmem_zalloc`), initialises the lock `is_ir_lock` and fills it (new vertex +0, host +4, guest
+8, config map +16, memory map +24), attaches it as fastinfo and makes it the console chip (the global at 0xc0000000014d2c88) when both `hub_check_pci_equiv` calls match
the console configuration and memory values. It registers the error handler with `pciio_error_register` on both parents, writes the PCI command register (config offset 4
ORed with 0x8146, bits for I/O, memory, bus master, parity and SERR), sets chip registers at memory base +0x28 to 0x7ae5e and +0x34 to 0xc0 and config +0xc to 0xff00,
clears all interrupt enables with `ioc3_write_ireg(soft, -1, 1)`, sets the register at +0x1c to 0xffffffff, and tests the vertex name with
`strstr` for `Ibrick/xtalk/15/pci/4` (the "console" chip: sets the byte at +2885 to 1 and calls `ioc3_subdev_enables(vertex, 0x91)`), `baseio` and `menet`, otherwise
reading the board NIC with `nic_ioc3_vertex_info`. It sets the interrupt target (+32 doubleword to 0 and the halfword at +40 to the running CPU) when the helper at
0xc0000000000962d4 returns 0, builds a device descriptor for the interrupt (`device_desc_dup`, flag bit 4 added, software level 0, name `IOC3 UART`), allocates the
interrupt with `pciio_intr_alloc` (class 2 when the host is the guest and sub-device 0 is enabled, or when the NIC string contains `030-1657-` or `030-1664-`, otherwise
1; a failure is `cmn_err(0, "%v: ioc3_attach: unable to allocate interrupt")`), connects it to `ioc3_intr` (`pciio_intr_connect`), records the CPU it landed on
(`pciio_intr_cpu_get`, `cpuvertex_to_cpuid`, halfwords at +40 and +42), warns with `Request to target IOC3 interrupts to CPU %d could not be satisfied` (level 4) when
they differ and then calls `ioc3_serial_attach(guest)` and `pckm_attach(guest)`. For the console chip it sets the global `console_device_attached` to 1. It returns 0 on
success and -1 on the mapping failures. The condition on which a requested CPU is honoured was decoded from the decompiler output and checked against the compare at
0x95540 only.

## Hub memory error classification

- `huberror_process_rrb_error` (`6.5.30/unix.IP27`, address 0xc000000000034054): **inferred.** Decides what to do about a failed read that the hub's request buffer
reported. Arguments: the error type (`a0`: 1 uncached read, 2 cached read with a directory error, 3 read timeout) and the physical address (`a1`, whose bits 32 to 39 are
the NASID). For each type it asks which partition owns the address (`nasid_to_partid`) and compares with `part_id()`, then returns a small disposition code and never
changes memory itself: type 3 on the local partition goes to `error_check_part_page` (zero means the page is not a partition-shared one and ends in
`cmn_err(0x14, "Read timeout, physaddr 0x%x")`) and `partition_handle_error(0, addr, 1)`, giving 2 when that returns 3 and
0 otherwise; type 3 on another partition gives 1 or 0 by the same test. Type 2 on the local partition checks `page_isdiscarded` (a page that is not discarded prints
`Cached read directory error, physaddr 0x%x in %s` with the memory bank name from `membank_pathname_get`, at `cmn_err` level 0x14), then
`error_check_part_page` (zero gives 0) and `partition_handle_error(0, addr, 4)` giving 6 or
0; type 2 on another partition gives 4 or 0. Type 1 on the local partition checks `error_check_part_page` (zero ends in
`Uncached read error, physaddr 0x%x`) and `partition_handle_error(0, addr, 10)` giving 8 or
0; on another partition 7 or 0. Any other type is `Invalid RRB error type: 0x%x`. The
`cmn_err` level 0x14 calls do not return. A call to `error_page_discard` at 0x3428c is present and its condition is unknown.
- `huberror_process_wrb_error` (`6.5.30/unix.IP27`, address 0xc0000000000342ac): **inferred.** The write-side counterpart: arguments are the error type (`a0`: 0 cached
write, 1 uncached write, 3 write timeout) and the physical address (`a1`). Type 1 calls `kl_ioerror_handler(cnode, cnode, -1, 6, addr, 0, 1, 0)` for the node the CPU is
on and returns 0 when it returns 0, otherwise `cmn_err(0x14, "Uncached Write error, physaddr 0x%x node: %d")`. For types 0 and 3, with the owning partition looked up as
in the read case: type 3 on the local partition with `error_check_part_page` nonzero gives 0xc when `partition_handle_error(0, addr, 3)` returns 3 and
0 otherwise, with it zero returns 0 unless `error_check_poisoned_state(addr, 0, 1)` returns 3, which is
`Write timeout, paddr 0x%x` at level 0x14; type 3 on another partition gives 0xb or 0. Type 0 on the local partition with
`error_check_part_page` nonzero gives 0xf or 0, with it zero calls `error_check_memory_access(addr, 1, 0)`: zero is
`Cached write error, physaddr 0x%x` at level 0x14, 3 gives 0x11 and any other value 0; type 0 on another partition gives 0xe or 0.
Any other type is `Invalid WRB error type: 0x%x`. Returned values per path match the exits' "one of 0x0, 0xc", "0x0,
0xf", "0x0, 0xb" and "0x0,
0xe" entries.

## PCI bridge PIO maps and device settings

- `pcibr_piomap_alloc` (`6.5.30/unix.IP27`, address 0xc000000000072714): **inferred.** Allocates a PIO map for a device behind a PCI bridge. Arguments: the device vertex
(`a0`), the device number (`a1`, unused), the space type (`a2`), the offset
(`a3`), the size (`a4`), the maximum size (`a5`) and flags
(`a6`). It reads the slot byte at the device's info record +540 and the soft record through `pciio_info_mfast_get`; when `a4` or
`a5` is 0 it logs `pcibr_piomap_alloc: req_size | req_size_max < 1` through `pcibr_debug(0x8000000, ...)` and returns 0. It converts space, offset, size and flags to a
crossbar address with the routine at 0xc000000000071e44 and, when that gives -1, logs `xio_addr == XIO_NOWHERE` and returns 0. It reuses a record from the device's free
list at info +528 (a record whose size doubleword at +24 is zero) or allocates 0x60 bytes (`kmem_zalloc`) and links it into that list under the bridge lock at soft +128,
then fills it: vertex +4, slot at +8 (the slot, plus 1 for a bridge whose type halfword at +114 is 2, 0xff for slot 0xff), soft record +64, size +24, offset +16, flags
+0, space +12, and for type 2 zeroes +80 to +95. It then calls `xtalk_piomap_alloc(widget vertex, 0, xio address, size, max size, flags & 0x1f)`; on failure it marks the
record free again (size 0) and returns 0; on success it stores that map at +48 and the crossbar address at +40 and returns the record. The reuse scan and the list update
were read from the decompiler output; the instruction-level order of the list update was not checked.
- `pcibr_priority_bits_set` (`6.5.30/unix.IP27`, address 0xc000000000074a2c): **established.** Raises or lowers, with a use count, the high-priority bit of one PCI slot's
bridge device register. Arguments: the bridge soft record (`a0`), the slot (`a1`) and a mode (`a2`: 1 or
2 take a reference, 0 drops one, any other value changes nothing). The bit is 0x200000, except that for a bridge whose revision word at +120 is 0xc0022 the mask is 0, so
the bit is never changed. It takes the lock at +128 (`mutex_spinlock`) and reads the slot's shadow of the device register, the doubleword at +312 plus 128 times the slot,
with a use count in the word at +320. Take (modes 1 and 2): it increments the count; when the count becomes 1 it returns -1 if the mask is 0 (the `li s0,-1` at 0x74b80)
and otherwise ORs the mask into the new value; a later reference only counts. Drop (mode 0): a count that is already 0 or less returns -1 and changes nothing; otherwise
it decrements, and when the count reaches 0 with a nonzero mask it clears the bit. When the new value differs from the shadow it writes it with
`pcireg_device_set(soft, slot, value)`, stores it back at +312 and flushes with `pcireg_tflush_get`. It releases the lock (`mutex_io_spinunlock`) and returns 0, or -1 in
the two cases above.
- `pcibr_device_flags_set` (`6.5.30/unix.IP27`, address 0xc000000000074bd0): **established.** Turns the driver-level flags of a PCI device into set and clear masks for
its bridge device register and applies them. Arguments: the device vertex (`a0`, saved at 16(sp)) and a flag word (`a1`, `s2`); the slot is the byte at the device's host
info +540 and the soft record comes from `pciio_info_mfast_get`. Each flag pair maps to one register bit as set (the low bit of the pair) and clear (the high bit of the
pair): flags 0x10000 and 0x20000 set and clear 0x1000000, and also set (0x10000) and clear (0x20000) 0x800000; the pair at bits 18 and 19 sets and clears 0x40000; the
pair at bits 20 and 21 sets and clears 0x20000; flag bit 22 sets 0x8000 and flag bit 23 (0x800000) clears 0x8000; flag bits 26 and 27 set and clear 0x400000. For a bridge
whose type halfword at +114 is 2 with flag bit 26 (0x4000000) set it also removes 0x400000 from the set mask (the shift `1 << (revision & 0xf)` guarding it is always
nonzero for the nibble's range, so the guard is always true). With both masks zero it returns 1 without touching the bridge. Otherwise it takes the lock at soft +128,
computes `new = set | (shadow & ~clear)` from the slot's shadow doubleword at +312 plus 128 times the slot, and when that differs writes it with `pcireg_device_set`,
stores it back and flushes (`pcireg_tflush_get`), then releases the lock and logs `pcibr_device_flags_set: Device(%d): %R` with `pcibr_debug(0x4000000, ...)` (the
`%R` argument is the table `device_bits`). It returns 1 in every case.
- `ioc3_serial_kill` (`6.5.30/unix.IP27`, address 0xc000000000176ce8): **inferred.** Quiets one IOC3 serial port. Argument `a0` is the port record. It calls the method at
offset 40 of the table the record's doubleword at +8 points to, when that pointer is nonzero. If the port is the console port (the record equals the global at `gp`+5096)
and an external callback table is present (a doubleword at the address in the `gp-23672` slot equals 0x53435241 and the table whose pointer is in the `gp-23664` slot is
nonzero, has the marker word 0xfeeddead in its first word and a nonzero entry at +80), it raises the interrupt level with `spl7`, calls that entry at +80 with argument 0
and keeps the saved level in the global at `gp`+5092. It then stores 0 to the word the record's pointer at +64 holds (the port's interrupt-enable register), repeats the
same table test and, when it holds, calls the entry with argument 1 and restores the level (`splx`). It finally sets the record's word at +148 to 0. It returns nothing.
What the external table is (the marker 0x53435241 and its +80 entry look like a firmware console hook) is inferred.
- `pcibr_rrb_flush` (`6.5.30/unix.IP27`, address 0xc000000000076984): **established.** Flushes the response read buffers (RRBs) that belong to the device that is calling
it. It takes no argument of its own: its `a0` is passed unchanged to `pciio_hostinfo_get`, whose result (kept in `s0`) holds the device slot as the byte at +540;
`pciio_info_mfast_get` of it gives the bridge soft record (`s2`), and the doubleword at soft +8 is the bridge register base (`s5`). It tests the widget identification
word at base +4: part number (bits 27:12) 0xc002 with manufacturer (bits 11:1) 54, or part 0xd002 with manufacturer 36, sets `s7` to 2 (a Bridge or XBridge); any other
part leaves `s7` at 1. It reads the register of RRB assignments for the slot's group, `pcireg_rrb_get(base, slot & 1)`, takes the lock at soft +128 (`mutex_spinlock`),
and compares, for eight nibbles of that value from the low end (the register is shifted right by 4 each step), the nibble masked with `((s7 << 1) - 1) | 8` (0x9 for 1,
0xb for 2) against `((slot >> 1) | 8) & 0xffff` (the arithmetic shift of the slot byte); each match calls `do_pcibr_rrb_flush(base, n)` with `n` starting at `slot & 1`
and growing by 2 per nibble. After the eight nibbles it releases the lock with `mutex_io_spinunlock(soft+128, saved level)` and returns; the return value is whatever that
call leaves in `v0`, not a defined result. The meaning of the nibble layout (a valid bit 8 plus the owning device) is inferred from the masks.
- `pcibr_intr_free` (`6.5.30/unix.IP27`, address 0xc000000000078ad4): **inferred.** Releases a PCI bridge interrupt record made by `pcibr_intr_alloc`. Argument `a0` is
the record (`s` registers hold it). For each of the eight bridge interrupt bits set in the record's mask word at +88, it walks the singly linked list of handler nodes at
soft +0x680 plus 56 times the bit (the soft record is the pointer at record +96), calls `compare_and_swap_int64(node+8, record, 0)` to take this record out of the node
and, when that call returns nonzero, logs `pcibr_intr_free: cleared hdlr from bit 0x%x` with `pcibr_debug(0x1000, ...)`; and when the bit's handler count (word at +0x68c)
is 0 and its crossbar interrupt pointer (at +0x668) is nonzero, frees it with `xtalk_intr_free`, zeroes the pointer and clears the bit's three device-bit field with
`pcireg_intr_device_bit_clr(soft, 7 << (3 * bit))`. Then, when bit 1 of the record's first word is clear (it has a thread), it waits for bit 0x400 of the record's word at
+40 (thread exited-ready), polling every 10 ms with `nano_delay` for up to 1000 ms; if it never sets it prints
`pcibr_intr_free: Unable to terminate pcibr_intrd thread, xt=0x%lx` with `cmn_err(4, ...)` (which does not return); otherwise it points the thread at `xthread_exit`
(`xthread_set_func(thread, xthread_exit, 0)`), wakes it with `vsema(record+48)` and logs the wait. It ends with
`kmem_free(record, 0xa0)`, whose result is the return value. Which result of the swap means success was read from the decompiler output only.
- `pcibr_intr_disconnect` (`6.5.30/unix.IP27`, address 0xc000000000078f18): **inferred.** Disconnects a PCI bridge interrupt record from the hardware without freeing it.
Argument `a0` is the record. It zeroes its words +24 to +39, clears bit 0 of its first word, takes the soft record from +96 and its bit mask from +88, then drops from the
mask every bit whose wrapper (soft +56 times the bit, word +0x68c is the handler count) still has handlers. With bits left it takes the lock at soft +128 and clears them
in the bridge's interrupt enable register (`pcireg_intr_enable_bit_clr`), flushes (`pcireg_tflush_get`), releases the lock and logs
`pcibr_intr_disconnect: disabled int_bits=0x%x`; then for each remaining bit with a handler count of 0 it calls `xtalk_intr_disconnect` on the wrapper's crossbar
interrupt (pointer at +0x668), zeroes the wrapper's word at +0x690 and logs `disconnect int_bits=0x%x`; a path that reconnects a wrapper whose count has become nonzero
meanwhile (storing `bit | (soft word at +72 << 3)` at +0x698 and calling `xtalk_intr_connect(..., pcibr_intr_func, wrapper+0x670, pcibr_setpciint, wrapper+0x698, 0)` and
logging `now-sharing int_bits=0x%x`) is in the code but the count is tested twice with nothing in between to change it, so whether it can run is unknown. It returns
nothing.
- `router_acquire_port_info` (`6.5.30/unix.IP27`, address 0xc00000000001b514): **established.** Reads and checks one Craylink router port. Arguments: the router record
(`a0`), the port number 1 to 6 (`a1`) and a flag (`a2`; 0 means this call does the reading and takes the lock itself, nonzero means the caller already did and holds it).
The port's data block is at record +48 times the port (the doubleword at +8 of the block holds the decoded error counters, +16 the second register, +40 the error summary
word). With `a2` zero it reads router register `(port << 16) | 0x10` into block +8 and `(port << 16) | 0x88` into block +16
(`router_reg_read`), takes the record lock at +404 (`mutex_spinlock`) and calls `router_decode_port_errors(block+8)`. If the summary word at block +40 is nonzero it sets
bit `port - 1` of the byte at +7, either as `byte7 |= bit & ~byte6` (with `a2` zero, releasing the lock and returning 1) or as
`byte7 ^= bit` (with `a2` nonzero, returning 1 with the lock untouched). If it is zero: when the byte at +384 is 1 it takes the count in bits 63:48 of block +8 into the
halfword at block +50 and, when that count is nonzero, stores three rates (halfwords at block +44, +46 and +48), each being a 16-bit field of the doubleword (bits 15:0,
31:16, 47:32) shifted left by 10 and divided by the count, and sets bit `port - 1` in the byte at +7 when the second or third rate is 20 or more; when the byte at +384 is
not 1 it copies the four 16-bit fields of block +8 into the halfwords at block +44, +46, +48 and +50 unchanged. With `a2` zero it releases the lock and in every no-error
case returns 0.
- `pcibr_release_device` (`6.5.30/unix.IP27`, address 0xc0000000000712e4, 176 bytes, 48-byte frame): **established.**
Drops one use of a slot's Device-register setting: it decrements the use counter that matches a flag value, under the soft record's lock. `a0` (`s8`) is the bridge soft
record, `a1` (saved at sp+32) the slot number and `a2` (`s0`) a flag value. It takes the lock at soft +128 (`mutex_spinlock`, 0xc0000000002863b4). The three counters it
adjusts are the words at soft + 128 * slot + 324, +328 and +332 (in the kernel's structure definition the slot array starts at soft +256 with a 128-byte stride, which
puts them at +68, +72 and +76 of a slot structure). For flag value 0x1100000 it decrements the first of those words (`bss_pmu_uctr` in the slot structure), for 0x8f8000
the second (`bss_d32_uctr`) and for 0x898000 the third (`bss_d64_uctr`); any other value changes nothing. Each decrement is a plain load, subtract and store under the
lock. It then releases the lock with `mutex_io_spinunlock` (0xc0000000002862c4), passing the state `mutex_spinlock` returned, and returns that call's result (exit
0xc000000000071380 onwards). The counter names come from the kernel's own structure definition for the slot block.
- `pcibr_func_config_addr` (`6.5.30/unix.IP27`, address 0xc00000000007c2ec, 132 bytes, 32-byte frame): **established.**
Returns the address through which one PCI configuration register can be read or written, choosing the type 0 or the type 1 form by bus number. Arguments: `a0` (`s0`)
the bridge soft record, `a1` the bus number (signed), `a2` the slot, `a3` (`s1`) the function and `a4` (`s2`) the register offset. When the bus is zero or negative (the
branch is `blez`) it calls `pcireg_type0_cfg_addr(soft, slot, function, register)` (0xc000000000088480) and returns its result (exit through 0xc00000000007c368).
Otherwise it first calls `pcireg_type1_cntr_set(soft, (bus << 16) | (slot << 11))` (0xc00000000008318c), then `pcireg_type1_cfg_addr(soft, function, register)`
(0xc000000000088730) and returns that result (0xc00000000007c338). From the call order, a type 1 access takes its bus and slot from the bridge's type 1 control register
and only the function and register offset from the address (inferred).
- `pcibr_slot_device_init` (`6.5.30/unix.IP27`, address 0xc00000000007de88, 244 bytes, 48-byte frame): **established.**
Writes the start-up value of one slot's Device register: reads it, forces the bits the kernel wants and caches and writes the result. Arguments: `a0` the bridge vertex
(saved at sp+0), `a1` (`s1`) the slot number. It gets the soft record with `hwgraph_fastinfo_get` (0xc000000000296098; `s0`) and returns 22 when there is none
(0xc00000000007dea8) or when the slot number is not below the byte at soft +57 plus 1 (signed compare; 0xc00000000007debc). It reads the register with
`pcireg_device_get(soft, slot)` (0xc0000000000865e8) and clears bit 27 (mask 0x8000000). For a bridge whose type halfword at soft +114 is not 2, it sets bits 25 and 16
together (0x2010000). For type 2 it tests the low nibble of the word at soft +120 (the test is `(1 << nibble) & 2`): when it is 1 only bit 16 is set (0x10000),
otherwise bits 25 and 16 (0x2010000), as for the other types. It stores the value in the doubleword at soft + 128 * slot + 312 first and then calls
`pcireg_device_set(soft, slot, value)` (0xc000000000086808), and logs
`pcibr_debug(0x4000000, vertex, "pcibr_slot_device_init: Device(%d): %R\n", slot, value, table at 0xc000000001437f10)` (0xc000000000074e8c). It returns 0
(0xc00000000007df4c). It does not take the soft lock. The code does not name the bits.
- `pcibr_endian_set` (`6.5.30/unix.IP27`, address 0xc000000000074944, 232 bytes, 80-byte frame): **established.**
Sets or clears the byte-swap bits of one slot's Device register and caches the new value. Arguments: `a0` the device's vertex (saved at sp+8 and passed on unchanged),
`a1` (`s0`) and `a2` (`s2`) two values compared with each other. It calls `pciio_hostinfo_get(a0)` (0xc0000000000934d4) and reads the slot from the byte at +540 of the
information it returns (`s1`), then `pciio_info_mfast_get` (0xc0000000000935dc) for the bridge soft record (`s3`) and takes the soft lock at +128 (`mutex_spinlock`,
0xc0000000002863b4). The cached Device register is the doubleword at soft + 128 * slot + 312 (the `bss_device` member of the slot block). The new value is the cached
one with bits 20 and 19 (the constant 0x180000) cleared when `a1` equals `a2`, and set when they differ. When the new value is unchanged it does not touch the hardware;
otherwise it calls `pcireg_device_set(soft, slot, new)` (0xc000000000086808), stores the new value back in the cache and reads the flush register with
`pcireg_tflush_get` (0xc000000000080ba8). It releases the lock (`mutex_io_spinunlock`, 0xc0000000002862c4) and logs through
`pcibr_debug(0x4000000, vertex, "pcibr_endian_set: Device(%d): %R\n", slot, new value, the flag-name table at 0xc000000001437f10)` (0xc000000000074e8c). It returns
`a2`. That bits 20 and 19 are the two byte-swap bits of the Device register is inferred from the published Linux header, where `BRIDGE_DEV_SWAP_PMU` is 0x100000 and
`BRIDGE_DEV_SWAP_DIR` is 0x80000; the code itself does not name them.
- `pcibr_bridge_ptr_get` (`6.5.30/unix.IP27`, address 0xc00000000007512c, 172 bytes, 16-byte frame): **established.**
Maps a bridge widget's registers and returns their address, moving to the second bus's block for the two-bus part. `a0` is the widget vertex; `a1` (saved at sp+8)
selects the interface, 0 or 1. It calls `xtalk_piotrans_addr(vertex, 0, 0, 8, 0)` (0xc00000000006be68) and reads the widget identification word at +4 of the result:
part number in bits 27 to 12, manufacturer in bits 11 to 1. When the part is 0xd102 or 0xd112 and the manufacturer is 36, and `a1` is 1, it returns the mapped address
plus 0x800000 (0xc0000000000751d0); in every other case, including a part or manufacturer that does not match and an `a1` other than 1, it returns the mapped address
unchanged (0xc0000000000751a4). The result of a failed mapping is not tested: the word at +4 is read from whatever came back. That parts 0xd102 and 0xd112 are the two
bus parts of one bridge chip is inferred from the published Linux headers that give those numbers.
- `pcibr_piotrans_addr` (`6.5.30/unix.IP27`, address 0xc0000000000729f4, 204 bytes, 64-byte frame): **established.**
Turns a PCI address window of one device into a processor-visible PIO address through the Bridge's crosstalk widget, or returns 0 when the window has no crosstalk
address. `a0` (`s2`) is the device vertex, `a2` (`s7`), `a3` (`s8`), `a4` (`s1`) and `a5` (`s0`) are four values handed on, and `a1` is not used. It calls
`pciio_hostinfo_get(a0)` (0xc0000000000934d4), keeps the byte at +540 of the result as the slot (stored at sp+56) and calls `pciio_info_mfast_get` (0xc0000000000935dc)
on it for the Bridge soft record. It then calls the routine at 0xc000000000071e44 with `a0` the device vertex, `a1` the slot, `a2` to `a5` the four saved values; the
word at soft +0 is loaded just before the call and stored at sp+0 in the call's delay slot. That routine is not a separate symbol (the symbol table puts it under
`pcibr_write_gather_flush`, 60 bytes in) and is not read here; it takes a PCI address and gives a crosstalk address, and its result of -1 means no address (inferred
from the use made of it below and the log text). A result of -1 (the 64-bit compare with -1, 0xc000000000072a60) calls
`pcibr_debug(0x10000000, device vertex, "pcibr_piotrans_addr: xio_addr == XIO_NOWHERE\n")` (0xc000000000074e8c) and returns 0 (the `move a0,zero` at 0xc000000000072abc,
the delay slot of the branch back to the epilogue at 0xc000000000072ab8, sets the value that is returned). Any other result goes to
`xtalk_piotrans_addr(the word from soft +0, 0, that result, a4, a5 & 0x1f)` (0xc00000000006be68), and its result is returned. No branch-likely instruction.

## Hub widget and error helpers

- `hub_widgetdev_shutdown` (`6.5.30/unix.IP27`, address 0xc0000000000278ac, 472 bytes, 80-byte frame): **established.**
Detaches one crosstalk widget's device from the hub that masters it and frees any hub I/O request-tracking entries (CRBs) that still belong to it. `a0` is the widget
vertex, passed straight through; `a1` (kept in `s4`) is a small number used both as a bit index and as a match value (a device number within the widget, going by the
bit index 8 * widget id + `a1`). It calls `xwidget_info_get` (0xc00000000006c368), then `xwidget_info_id_get` (0xc00000000006c44c; the widget id is kept in `s5`) and
`xwidget_info_master_get` (0xc00000000006c480), and looks up the master's `_node` label with `hwgraph_info_get_LBL` (0xc000000000295310), the result going to the
doubleword at sp+0; the return value is not tested. It returns with nothing done when that doubleword is 0, or when the halfword at +10 of the record it points to (the
node's NASID, kept in `s7`) is -1. Otherwise it first clears one bit in a hub register: the doubleword at +288 of the node's hub register page (the page is the pool
constant 0x9200000001c00000, held at 0xc0000000014cc198, plus the NASID shifted left by 32, `sd` at 0xc00000000002795c) loses bit number `a1` + 8 * (widget id & 0xff)
minus 64. It then walks 15 CRB slots, index 0 to 14 (`s1`), reading the doubleword at +1024 + 32 * index of the same page. A slot is taken when bit 50 of that word is
set, or, only while the global word `free_error_crbs` is nonzero, when bit 55 is set. For a taken slot it extracts the 5-bit field at bits 44 to 40 and acts only when
the field equals `a1` + 16 or `a1` + 24: it calls `hubiio_crb_free` (0xc000000000034990) with the doubleword at sp+0 and the slot index, and counts it in `s6`. When the
global word `free_error_crbs_print` is nonzero it also calls `cmn_err(6, "Releasing %s CRB 0x%x[Freed so far %d]\n", ...)` with the text "Data" when the field was
`a1` + 16 and "Interrupt" when it was `a1` + 24, the index and the count. After slot 14, if `free_error_crbs_print` is nonzero it calls
`cmn_err(6, "Freed %d CRBS on nasid %d \n", count, nasid)`. Both exits (0xc000000000027a1c, reached from 0xc0000000000279f8 on the early returns and from
0xc000000000027a70 after the summary) leave `v0` as the last call left it: the routine has no meaningful result.
- `hub_device_inquiry` (`6.5.30/unix.IP27`, address 0xc00000000003c59c, 436 bytes, 96-byte frame): **established.**
Prints, through `cmn_err` at level 6, which of a widget's eight devices are shut down and whether the widget may access the hub and be accessed by it. `a0` (`s3`) is a
hardware-graph vertex and `a1` (saved at sp+48) a widget number. It formats the number with `sprintf` ("%d", into sp+0, 0xc0000000001fd9d0), looks up the child of `a0`
with that name (`hwgraph_traverse`, 0xc00000000029579c, vertex into the word at sp+8) and returns at once if the lookup fails (`v0` nonzero). It then asks
`device_master_get` (0xc000000000296bec) for the vertex's master, returns when that is -1, and reads the master's `_node` label (`hwgraph_info_get_LBL`,
0xc000000000295310, into sp+16), returning when it is null. The halfword at +10 of the label's record is the NASID; the hub register page is the pool constant
0x9200000001c00000 plus the NASID shifted left by 32. Three doublewords of that page are read and saved: +272 at sp+32, +280 at sp+24 and +288 at sp+40. Output:
"Inquiry Info for %v" with the child vertex; "\tDevices shutdown [ "; then for devices 0 to 7 the number (" %d") of each device whose bit is clear in the doubleword at
+288, the bit index being 8 * widget number minus 64 + device (the bit that `hub_widgetdev_shutdown` clears); then "]\n"; then "\tInbound access ? %s" with "yes" when
bit (1 << widget number) of the doubleword at +280 is set and "no" otherwise, and "\tOutbound access ? %s" with the same test on the doubleword at +272. The three
return points are 0xc00000000003c5d4 (lookup failed, `v0` is the `hwgraph_traverse` result), 0xc00000000003c5f4 and 0xc00000000003c610 (no useful value) and
0xc00000000003c73c after the last print.
- `hub_widget_reset` (`6.5.30/unix.IP27`, address 0xc000000000029660, 424 bytes, 80-byte frame): **established.**
Releases the hub's eight pending PIO read buffers (PRBs) that belong to one widget, then sets the hub's buffer up again for that widget. `a0` is a vertex handed to
`master_node_get` (0xc000000000296c44), which gives the compact node number; `a1` (`s4`) is the widget number. When the node is -1 (halfword compare) the routine
returns with `v0` still holding the `master_node_get` result (0xc000000000029694). Otherwise the NASID is the halfword at that node's slot in the table whose pointer is
in the gp-20496 slot (0xc0000000014cd160 pointing at 0xc0000000014d4e80), and the hub register page is 0x9200000001c00000 plus the NASID shifted left by 32. It raises
the interrupt level with `splhi` (0xc00000000003b30c; the old level is saved at sp+0) and scans entries 0 to 7 of the doubleword table at page +776 (stride 8; the loop
is unrolled by two, with identical bodies at 0xc0000000000296ec and 0xc000000000029714). An entry is taken when bit 56 is set and the byte at bits 31 to 24 equals the
widget number. For a taken entry it stores 0 in the entry and writes the value (index | 0x10) to the doubleword at page +904, then reads that doubleword back and, when
bit 4 is still set, calls `assfail` (line 1378, assertion text "! (REMOTE_HUB_L(nasid, IIO_IPDR) & IIO_IPDR_PND)"; the test is the only read-back, with no wait) and
goes on with the next entry; the branch at 0xc00000000002974c is branch-likely (it executes the entry-counter increment in its delay slot only when taken), the others
are plain branches. After the scan it calls `hub_setup_prb(nasid, widget number, 3, 2)` (0xc00000000003c338), restores the level with `splx` (0xc00000000003b3f4) and
returns; `v0` is the `splx` result and carries no meaning.
- `hubni_print_state` (`6.5.30/unix.IP27`, address 0xc000000000032e00, 320 bytes, 192-byte frame): **established.**
Prints the network-interface error state of a node's hub at most once per state bit, through a caller-supplied print function. Arguments: `a0` (saved at sp+128) the
NASID whose register is shown, `a1` (`s1`) the first argument handed to the print function, `a2` (`s2`) the print function and `a3` (`s7`) the state bits. It finds the
executing processor's per-node data: the doubleword at absolute address 0xffffffffffffc400 (`p_nodepda`) points to a table, indexed by the halfword at absolute address
0xffffffffffffc422 (taken to be the processor's compact node number), of node records (`s0`). When `a3` is 2 it first spends up to 1000000 rounds on the node record's
lock at +18916: each round is `mutex_spintrylock` (0xc00000000028662c), and a failed try is followed by `us_delay(1)` (0xc000000000047c0c); on success or when the
rounds run out it calls `mutex_spinunlock` (0xc000000000286400) on the same lock (the unlock happens either way, with the last try result, 0 after a timeout, as the
state argument; the test `at == 0` at 0xc000000000032e9c is never taken). It then sets the bits in the word at record +21464 with an `ll`/`sc` loop (the retry branch at
0xc000000000032e58 is branch-likely) and returns without printing when any of the bits was already set (0xc000000000032e6c). Otherwise it calls `get_slotname`
(0xc00000000003cc9c) with the slot byte at record +18856 and a buffer at sp+8, prints "NI ERROR STATE:\n" and "/module/%d/slot/%s/node\n" (module halfword at record
+18858 and the slot name) by calling the print function with `a1` as its first argument, and finally calls `show_ni_port_error` (0xc000000000031570) with the doubleword
at (0x9200000001e10000 + NASID shifted left by 32 - 32760), `a1` and the print function; the result is the return value (0xc000000000032f18). The early return leaves
`v0` undefined. The register at that address is the hub's network-interface port error register (inferred from the callee's name; the register's fields are left open).
- `hubii_eint_init` (`6.5.30/unix.IP27`, address 0xc000000000032894, 388 bytes, 48-byte frame): **established.**
Sets up one node's I/O error interrupt: reserves an interrupt level, connects the I/O error handler to it and points the hub's error-interrupt destination register at
that level and node. `a0` (`s1`) is the compact node number. It reads the node's hub vertex from the word at +20816 of its node record (the record is reached through
the table behind `p_nodepda`, absolute 0xffffffffffffc400); a vertex of -1 runs `assfail` (line 309, text "hub_v != GRAPH_VERTEX_NONE") and continues anyway. It reads
the vertex's `_node` label (`hwgraph_info_get_LBL`, 0xc000000000295310, result at sp+0); the halfword at +10 of the record it points to is the NASID, and the register
page is the pool constant 0x9200000001c00000 plus the NASID shifted left by 32. It loads the doubleword at page +296: when its bit 13 is clear it stores the value back
with bit 10 cleared and returns without connecting anything (0xc0000000000329bc to 0xc0000000000329c4). Otherwise it calls `intr_heuristic` (0xc000000000038db8) with
the vertex, 0, -1, 4, the vertex and the text "HUB IO error interrupt", the level coming back in the word at sp+8 and a processor number as the result; a result of -1
(halfword) runs `cmn_err(20, "hubii_eint_init: intr_reserve_level failed, cnode %d")` and returns. It then calls `intr_connect_level` (0xc0000000000383ec) with that
processor, the level, 0, `hubii_eint_handler` (0xc000000000034ed4), the vertex zero-extended and 0; a negative result runs `assfail` (line 339, text "rv >= 0") and
continues. Finally it builds the doubleword stored at page +312: the low 7 bits of the level, bit 24 set, bits 16 to 8 replaced by the node's own NASID (from the
halfword table behind the gp-20240 slot at 0xc0000000014cd260, indexed by the compact node, masked to 9 bits and shifted left by 8; the mask word 0xfffffffffffe00ff is
the pool constant at 0xc0000000014cc338). The routine returns no value (exits at 0xc0000000000329a8 and 0xc0000000000329f4).
- `hubpi_eint_handler` (`6.5.30/unix.IP27`, address 0xc000000000032b28, 728 bytes, 80-byte frame): **established.**
Handles the processor-interface error interrupt on the executing processor: checks the network interface, panics on fatal causes, drains the error spools and
acknowledges each cause it handled. It starts with `check_ni_errors` (0xc000000000032f40) and, if that returns nonzero, calls `hubni_error_handler` (0xc00000000003319c)
with the text "PI interrupt" and 1. The processor number is the halfword at absolute address 0xffffffffffffc420; the table whose address is in the gp-20184 slot
(0xc0000000014cd298, pointing at 0xc0000000014d5020) has 16 bytes per processor and its doubleword at +8 points at the processor's private data, whose byte at +1312 is
the slice (0 or 1). The pending word is the doubleword at 0x9200000001000400 ANDed with the mask doubleword at 0x9200000001000408 for slice 0 or at 0x9200000001000410
for any other slice (so `s0` = pending & mask). If any bit of 0xcfff00 is set it calls `enter_panic_mode` (0xc0000000001e59b4),
`cmn_err(20, "Fatal PI error interrupt received")` and `kl_panic` (0xc0000000000377b4) with the same text, and, if `kl_panic` ever returned, would carry on. If bit 24
is set it calls `log_md_errors` (0xc000000000033570) with the result of `get_nasid` (0xc00000000001d664). It then tests five cause pairs, each pair having one bit for
slice 0 and one for slice 1, in this order: (2, 1), (128, 64), (32, 16), (0x200000, 0x100000), (8, 4), the first number being the slice-0 bit. The first three pairs
each call `process_error_spool` (0xc000000000034598) when their bit is set; the fourth, when set, prints
`cmn_err(20, "Unexpected UCE error interrupt. int pend 0x%x\n", pending)`; the fifth prints `cmn_err(20, "Spurious message intr, likely partition reboot")`. After
handling a pair it stores that pair's bit for the slice alone into the doubleword at 0x9200000001000400, which acknowledges the cause (inferred: the code only shows the
store of the single bit back to the pending register). The routine returns no value (the exit at 0xc000000000032d38; 0xc000000000032df8 is the path after the last
print). The identification of the three registers as the pending word and the two slice masks is inferred from how they are combined.
- `hub_ioerror_local` (`6.5.30/unix.IP27`, address 0xc000000000027670, 252 bytes, 32-byte frame): **established.**
Decides what to do about an I/O error that was taken on this node: it ignores error kinds 0, 2 and 3, and for any other kind panics after checking whether the accessing
node had permission to reach the target node's hub. `a2` is the error kind and `a3` (`s0`) a pointer to the error record. For kinds 0, 2 and 3 it returns 0 at once
(0xc000000000027690 for 3, 0xc0000000000276a4 for 0 and 2). For other kinds it reads from the record the word at +0, the halfword at +12 (node A) and the halfword at
+14 (node B), turns both halfwords into NASIDs through the halfword table behind the gp-20496 slot (0xc0000000014cd160, pointing at 0xc0000000014d4e80), and takes `a2`
as the doubleword at record +16 when bit 25 of the word at +0 is set (the word shifted left 6 is negative) and 0 otherwise. It loads the doubleword at
0x9200000001800000 (the pool constant at 0xc0000000014cc168) plus node B's NASID shifted left by 32 and tests bit number (node A's NASID, taken modulo 64) of it. When
that bit is set it calls `cmn_err(4, "Error accessing Hub Register %x on node (%d/%d) from (%d/%d), with access permission\n", ...)` with `a2`, node B's NASID and
halfword, node A's NASID and halfword, and then `kl_panic` (0xc0000000000377b4) with "Hub Register Access error". When the bit is clear the branch at 0xc00000000002770c
skips both of those calls; the routine does not expect `kl_panic` to return, but if it did the code falls into the next step. That step calls
`cmn_err(4, "Error in accessing hub register 0x%x on node (%d/%d) by cpu %d on Node (%d/%d): No permission\n", ...)` with the doubleword at record +16 as the register,
node B's NASID and halfword, node A's NASID in the place of the cpu number and node A's halfword; the format has six conversions and the routine sets arguments a2 to a6
only, so the last conversion takes whatever `a7` held. It then calls `kl_panic` with "Hub Register Access Permission error". The final return (0xc000000000027764)
leaves `v0` as `kl_panic` left it. The reading of the permission doubleword as one bit per accessing node is inferred from how the bit is selected and the two messages.
- `hub_setup_prb` (`6.5.30/unix.IP27`, address 0xc00000000003c338, 148 bytes, no frame): **established.**
Programs one of the hub's PIO read buffer (PRB) registers: it sets the buffer's low byte and two mode bits and clears the fields around them. Arguments: `a0` the NASID,
`a1` the widget number (0 to 15), `a2` a byte value and `a3` a mode. No routine is called. When the global word `force_fire_and_forget` (gp-29212, 0xc0000000014caf54)
is nonzero and the global word `ignore_conveyor_override` (the next word, gp-29208) is zero, a mode of 1 is changed to 2 and any other mode is kept. The register index
is `a1` for widgets below 8 and `a1` minus 7 otherwise (0 to 7, and 1 to 8). The register is the doubleword at the hub register page (the pool constant
0x9200000001c00000 at 0xc0000000014cc198, plus the NASID shifted left by 32) + 408 + 8 * index. It is rewritten as: bits 47 to 43 cleared (mask 0xffff07ffffffffff, pool
at 0xc0000000014cc440) and bit 43 set; then bits 8 to 21, 24 to 37 and bit 42 cleared (mask 0xfffffbc000c000ff, pool at 0xc0000000014cc448); then bit 42 set when the
(possibly changed) mode is not 1; then the low byte replaced by `a2` & 0xff. Bits 22, 23, 38 to 41 and 48 to 63 keep their value. The store is the delay slot of the
`jr ra` (0xc00000000003c3c4). Nothing is returned. The meaning of the fields is not named by the code.
- `hubni_eint_init` (`6.5.30/unix.IP27`, address 0xc000000000032a18, 272 bytes, 48-byte frame): **established.**
Connects the hub's network-interface error interrupts for one node. `a0` (`s3`) is the compact node number. `cnodetocpu` (0xc000000000026fc8) gives the first processor
of the node; a result of -1 (64-bit compare) returns with nothing connected. Otherwise `intr_connect_level` (0xc0000000000383ec) is called with that processor (as a
halfword), level 126, 0, `hubni_eint_handler` (0xc000000000033324), 0 and 0, and a nonzero result prints
`cmn_err(20, "hub_error_init: Cannot connect NI error interrupt.")` and carries on. It then tries the node's two processor slices, 0 and 1, with the level 103 for slice
0 and 104 for slice 1: `cnode_slice_to_cpuid` (0xc000000000027260) gives the processor (halfword); a negative number, a number not below the word `maxcpus` (the gp+2352
slot, signed compare) or a processor for which `cpu_enabled` (0xc000000000027228) is zero is skipped. For the others it calls `intr_connect_level` with the processor,
the level, 0, `hubni_rmteint_handler` (0xc000000000033348), 0 and 0; a nonzero result prints the same message, and the success test at 0xc000000000032ad0 is
branch-likely, so the counter increment in its delay slot runs only on success, and the failure path does the increment itself in the delay slot of the branch at
0xc000000000032b20. The early return (branch at 0xc000000000032a38) leaves `v0` as `cnodetocpu` returned it, -1; the other exit (loop end, branch at 0xc000000000032a78)
leaves it undefined.
- `hub_init_ifdr_war` (`6.5.30/unix.IP27`, address 0xc000000000028dd0, 252 bytes, 32-byte frame): **established.**
Resets the bookkeeping for one node's IFDR workaround and lowers the hub's IFDR field to 40 when the PROM left it higher. `a0` is the compact node number (the register
page is the pool constant 0x9200000001c00000 plus the node's NASID shifted left by 32; the NASID is the halfword of the table behind the gp-20496 slot,
0xc0000000014cd160, indexed by `a0`). The node record comes from the table behind `p_nodepda` (absolute 0xffffffffffffc400), the doubleword at index `a0`. Three stores
into the record come first: the halfword at +19586 gets 0, the halfword at +19588 gets 40 and the word at +19580 gets the global word `hub_ifdr_war_freq` (gp-32604). It
then reads the doubleword at page +920 and extracts the 7-bit field in bits 14 to 8; the halfword at record +19588 (read back, so 40) is compared (unsigned) with the
field. When the field is not above 40 the routine returns (the `beqz` at 0xc000000000028e58; the frame is allocated in that branch's delay slot and the return releases
it). Otherwise it saves a5, a4, a3 and `ra` on the frame; for node 0 only it calls `cmn_err(5, "HUB IFDR value not correctly set. Please upgrade your prom")`
(0xc0000000001fcbd4) and reloads the saved values. For every node that reaches this point it stores the register value with bits 14 to 8 replaced by the low 7 bits of
the halfword at record +19588 (AND with 0xffffffffffff80ff, then OR with the field shifted left by 8) back at page +920. `v0` is not set to a meaningful value on either
return (0xc000000000028e8c is the one `jr ra`). What the register at +920 is called is inferred from the routine's own name; the code does not name it.
- `huberror_process_stack_entry` (`6.5.30/unix.IP27`, address 0xc0000000000344d8, 192 bytes, 48-byte frame): **established.**
Routes one entry of the hub's error stack to the write-buffer or read-buffer error routine according to the entry type, and hands the decoded address back through a
pointer. `a0` (also copied to `v0`) is the packed entry word, `a1` a second word, `a2` the entry type and `a3` (`s8`) a pointer that receives one doubleword. Type 1
(the `beq` at 0xc0000000000344f4): the doubleword is the entry word shifted right 31 and then left 7 (the shift left is in the delay slot of the `bnez` at
0xc000000000034560); when bit 12 of the entry word is set it calls `huberror_process_wrb_error` (0xc0000000000342ac), otherwise `huberror_process_rrb_error`
(0xc000000000034054). Type 2: the doubleword is the entry word shifted right 25, masked to its low 37 bits (the constant -1 shifted right 27) and shifted left 3; bit 42
of `a1` set selects `huberror_process_wrb_error`, clear selects `huberror_process_rrb_error`. Both callees receive the entry word's low 3 bits in `a0` and that
doubleword in `a1`; `a2` and `a3` pass through unchanged. After the call the doubleword is stored at `*a3` (the store at 0xc000000000034538 is common to both paths) and
the callee's result is returned (the one `jr ra` is at 0xc000000000034548). Any other type calls `cmn_err(20, "huberror_process_stack_entry, bad type 0x%x", type)`
(0xc0000000001fcbd4), then reloads `s0` from sp+8, `a2` from sp+4 and `a0` from sp+0, none of which this routine ever stores, and joins the type 2 path at the bit 42
test; its outcome depends on whatever the stack held. No branch-likely instruction. That the two callees handle the write-buffer and read-buffer error cases is inferred
from their symbol names.
- `hub_set_piomode` (`6.5.30/unix.IP27`, address 0xc00000000003c3cc, 240 bytes, 64-byte frame): **established.**
Puts all of one node's PIO read buffers into a given mode, with a register quiesced around the update. `a0` (`s1`) is the NASID and `a1` (`s2`) the mode. When the NASID
is the console node (`get_console_nasid`, 0xc00000000001f8f0) it takes the console buffer lock first (`putbuf_lock`, 0xc0000000001ff7d8; `s3` records that) and releases
it at the end (`putbuf_unlock`, 0xc0000000001ff94c). The hub register page is the pool constant 0x9200000001c00000 plus the NASID shifted left by 32. It saves the
doubleword at page +272 (at sp+24) and zeroes it, then tests bit 17 of the doubleword at page +32. With that bit set it calls `hub_setup_prb(nasid, 0, 3, mode)`
(0xc00000000003c338) for buffer 0; with it clear it calls `hub_setup_prb(nasid, 0, 1, 2)`. It then calls `hub_setup_prb(nasid, widget, 3, mode)` for widgets 8 to 15 in
turn, writes the saved doubleword back to page +272 and reads it again. The return value is that read-back when the node is not the console node, and the
`putbuf_unlock` result when it is. The code does not say what the register at +272 or bit 17 of the one at +32 is for; the read-back after the write is probably there
to flush it (inferred).
- `hubiio_crb_free` (`6.5.30/unix.IP27`, address 0xc000000000034990, 160 bytes, 32-byte frame): **established.**
Frees one of a hub's I/O request-tracking entries (CRBs) and waits until the hub has finished with it. `a0` (`s1`) is the node's hub record, whose halfword at +10 is
the NASID, and `a1` is the entry index. The hub register page is the pool constant 0x9200000001c00000 (held at 0xc0000000014cc198) plus the NASID shifted left by 32. It
clears bit 50 and bit 1 of the doubleword at page + 1024 + 32 * index (an AND with the pool constant 0xfffbfffffffffffd held at 0xc0000000014cc390), then stores
`index | 0x10` in the doubleword at page + 912 and reads that doubleword back. While bit 4 of the value read is still set it calls `us_delay(1)` (0xc000000000047c0c)
and reads it again, so the wait has no bound. The bit 4 in the register at +912 is the pending flag of the free request (inferred by comparison with the neighbouring
register at +904, which the widget reset routine writes the same way and whose assertion text names it a pending bit). There is one `jr ra` (0xc000000000034a28); the
early branch at 0xc0000000000349f0 (bit 4 clear on the first read) reaches it with `v0` holding `index | 0x10`, and the loop exit leaves `v0` as `us_delay` returned it.
- `hub_ifdr_fix_value` (`6.5.30/unix.IP27`, address 0xc000000000028f3c, 236 bytes, no frame): **established.**
Waits for two hub queue-depth fields to reach zero and then rewrites the IFDR field with the node's stored value. `a0` is the compact node number; the NASID is the
halfword of the table behind the gp-20496 slot (0xc0000000014cd160). It reads the doubleword at page +920 (page = the pool constant 0x9200000001c00000 plus the NASID
shifted left by 32) and, when its 7-bit field in bits 14 to 8 is not below 65 (`sltiu` by 65), returns -1 at once (0xc000000000028f6c is the delay slot of the branch to
the return). Otherwise it adds 1 to the halfword at +19586 of the node record (the doubleword at the node's slot of the table behind `p_nodepda`, absolute
0xffffffffffffc400) and then polls, with no delay and no call, up to 99 rounds. Each round loads the doublewords at +208 and +216 of a second page (the pool constant
0x9200000001a00000, held at 0xc0000000014cc1f8, plus the NASID shifted left by 32; the second load is in the delay slot of the first test, so it happens every round). A
round in which bits 23 to 17 of the first are nonzero, or else those of the second are nonzero, counts down the budget (the second test is branch-likely, so its
decrement in the delay slot runs only when taken). When both fields are zero the loop is left to the rewrite below. When the budget reaches zero (0xc000000000028fdc) it
stores the second doubleword in the global `last_repqd_val` (gp+2408), the first in `last_reqqd_val` (gp+2400) and adds 1 to the word `wait_exceeded` (gp+2392). In both
cases it then writes the register at page +920 with bits 14 to 8 replaced by the low 7 bits of the halfword at record +19588 (the AND uses 0xffffffffffff80ff) and
returns 1 (the one `jr ra`, 0xc000000000029020; the -1 return is the first branch). That the fields at +208 and +216 are queue depths is inferred from the global names;
the code does not name them.
- `hubiio_prb_error_handler` (`6.5.30/unix.IP27`, address 0xc000000000034e08, 204 bytes, 64-byte frame): **established.**
Scans a hub's PIO read buffer registers for the ones whose bit 48 is set and calls a clean-up routine for each. `a0` (saved at sp+0) is passed on unchanged and `a1`
(saved at sp+8) is a hub record whose halfword at +10 is the NASID; the register page is the pool constant 0x9200000001c00000 (held at 0xc0000000014cc198) plus the
NASID shifted left by 32. It tests the doubleword at page +408 (index 0): when bit 48 is set (the value shifted left 15 is negative) it calls the routine at
0xc000000000034db0 with widget number 0 and sets the count `s4` to 1; otherwise the count starts at 0. It then loops over widgets 8 to 15 (`s1`) with register index 1
to 8 (`s0`), reading the doubleword at page + 408 + 8 * index; each one with bit 48 set adds 1 to the count and calls the routine at 0xc000000000034db0 with the same
`a0` and `a1` and the widget number in `a2`. The routine returns the count (the one `jr ra` is at 0xc000000000034e9c). The routine at 0xc000000000034db0 is not a
separate symbol (the symbol table puts it under `hubiio_crb_error_handler`, 896 bytes in); it ignores `a0`, calls
`cmn_err(4, "Hub nasid %d got a PIO Write error from widget %d, cleaning up and continuing", the halfword at +10 of a1, the widget)` (0xc0000000001fcbd4) and stores
`1 << widget` in the doubleword at page + 504, returning with that store in the delay slot of its `jr ra`. The code does not say what bit 48 of the register at +408
means; register +408 is the one `hub_setup_prb` rewrites for widget 0. No branch-likely instruction.
- `hub_xp_error_handler` (`6.5.30/unix.IP27`, address 0xc00000000002776c, 124 bytes, 16-byte frame): **established.**
Passes a crosstalk error to the crosstalk error handler when the hub says the widget interface has something pending, and otherwise answers 7. `a1` is the NASID, `a2`,
`a3` and `a4` are three values handed on, and `a0` is not used. It reads the doubleword at +488 of the hub register page (the pool constant 0x9200000001c00000 plus the
NASID shifted left by 32) and tests bits 0 and 4 (mask 0x11). With both clear it returns 7 (`a6` is set to 7 by the `li` at 0xc000000000027790, just before the `beqz`
at 0xc000000000027794, and the return takes `a6`). Otherwise it turns the NASID into a compact node number with the halfword table behind the gp-20480 slot
(0xc0000000014cd170, pointing at 0xc0000000014d4c80), reaches the node record through the table behind `p_nodepda` (absolute 0xffffffffffffc400) and calls
`xtalk_error_handler(the word at record +18912, a2, a3, a4)` (0xc00000000006c188). It returns that call's result (the one `jr ra` is at 0xc0000000000277e0). No
branch-likely instruction. What bits 0 and 4 of the register at +488 mean is not named by the code.
- `hub_merge_clean` (`6.5.30/unix.IP27`, address 0xc000000000028d44, 140 bytes, 16-byte frame): **established.**
For each of the hub's 15 request-tracking entries (CRBs) that the hub marks, reads one word from the memory address held in the entry and drops the value. `a0` is the
NASID; the register page is the pool constant 0x9200000001c00000 plus the NASID shifted left by 32. It loads the doubleword at page +936 and shifts it right 36, so bit
`i` of the result is bit 36 + `i` of the register. For `i` from 0 to 14 (the loop test is `slti` of the already incremented counter with 15) it tests that bit, using
`andi` with 0x7fff. When the bit is set it loads the doubleword at page + 1024 + 32 * `i` (the entry, which it keeps in a temporary on the stack), shifts it right 2,
keeps the low 38 bits (the constant -1 shifted right 26), shifts that left 3, ORs in 0xa800000000000000 (-11 shifted left 59) and loads the word at that address with
`lw`. The loaded value is not used. Nothing is returned, and the only stores are to that temporary. There are no calls and no branch-likely instructions. The code does
not say why the read is done; the entry's address field is read as a physical address with the cache attribute in bits 61 to 59, and a read through it would pull the
line in; that purpose is inferred from the routine's name.
- `hubii_eint_handler` (`6.5.30/unix.IP27`, address 0xc000000000034ed4, 352 bytes, 32-byte frame): **established.**
Handles the hub's I/O error interrupt: checks the network interface, prints why the crosstalk link failed when the hub reports it, runs the CRB and PRB error scans and
clears the interrupt. `a0` (`s0`, kept at sp+8 and read back as a 32-bit value) is the hub's vertex, the argument `hubii_eint_init` connected the handler with. It calls
`check_ni_errors` (0xc000000000032f40) and, when that is nonzero, `hubni_error_handler("II interrupt", 1)` (0xc00000000003319c). It reads the vertex's `_node` label
(`hwgraph_info_get_LBL`, 0xc000000000295310, into sp+0); the halfword at +10 of the record is the NASID and the register page is the pool constant 0x9200000001c00000
plus the NASID shifted left by 32. When bit 32 of the doubleword at page +8 is set it prints
`cmn_err(0, "Hub %d to Xtalk Link failed (II_ECRAZY) Reason: %s", the halfword at record +8, reason)` (0xc0000000001fcbd4), the reason being the first match of this
ladder: bit 9 of that doubleword "Micro Packet Retry Timeout", bit 6 "Crosstalk Tail Timeout", bit 5 "Crosstalk Credit Timeout", then in the doubleword at page +488 bit
0 "Hub Widget 0 has been Shutdown", bit 4 "BTE 0 has been shutdown", bit 8 "BTE 1 has been shutdown", and otherwise "Unknown". Then, whether or not it printed, it calls
`hubiio_crb_error_handler(vertex, record)` (0xc000000000034a30) and `hubiio_prb_error_handler(vertex, record)` (0xc000000000034e08), stores 0xffffff in the doubleword
at page +504 and clears bit 28 (the constant 0x10000000 inverted) of the doubleword at page +312, the register `hubii_eint_init` filled in. It returns with the result
of `hubiio_prb_error_handler` still in `v0` (the one `jr ra` is at 0xc000000000034fa8, the store to page +312 in its delay slot). No branch-likely instruction. The code
does not name the registers at +8, +312, +488 and +504 or the bits tested.

## Crossbar widget control

- `xbow_widget_num_get` (`6.5.30/unix.IP27`, address 0xc00000000006d104, 252 bytes, 288-byte frame): **established.**
Returns the crosstalk widget number that a hardware-graph vertex stands for, or 0xff when none can be found. `a0` is the vertex (kept in `s0`). It writes the vertex's
name into a 256-byte buffer at sp+0 (`vertex_to_name`, 0xc000000000296b3c) and asks `nodevertex_to_cnodeid` (0xc000000000296e54) for a compact node number. When that is
not -1 the vertex is a node: the halfword at the node's slot in the table whose pointer is in the gp-17944 slot (0xc0000000014cdb58, pointing at 0xc0000000014d4e80) is
the NASID, and the routine returns `hub_widget_id` (0xc00000000001d64c) of it. Otherwise it needs the name to contain "pci" and also "controller" (`strstr`,
0xc0000000001426e0, twice) and returns 0xff when either is missing. It then follows the edge named ".." twice from the vertex with `hwgraph_edge_get`
(0xc000000000294fc0), the cursor being the word at sp+256 and the doubleword at sp+264 counting passes; a failing edge lookup returns 0xff. For the vertex reached it
calls `xwidget_info_chk` (0xc00000000006c33c), returns 0xff when that gives nothing and otherwise returns `xwidget_info_id_get` (0xc00000000006c44c) of the result. Exit
points: 0xc00000000006d14c (`hub_widget_id`), 0xc00000000006d1b4, 0xc00000000006d1bc and 0xc00000000006d1e8 (0xff) and 0xc00000000006d1f8 (the widget id).
- `xbow_enable_perf_counter` (`6.5.30/unix.IP27`, address 0xc00000000006e7f0, 460 bytes, 80-byte frame): **established.**
Programs one of the crossbar's two performance counters to watch one link port, or releases it. Arguments: `a0` the crossbar vertex (saved at sp+8), `a1` (`s3`) the
port number, `a2` (`s2`) the mode, `a3` (`s0`) the counter number. It fetches the crossbar's record with `hwgraph_fastinfo_get` (0xc000000000296098; kept in `s1`) and
saves the doubleword at +16 of it (the register base) at sp+0. It returns -1 at once, before touching anything, unless the port is 8 to 15, the mode 0 to 2 and the
counter 0 or 1. It then takes the spin lock at record +448 (`mutex_spinlock`, 0xc0000000002863b4; the lock address is saved at sp+24 and the returned state at sp+16).
Each counter has a 16-byte slot in the record, slot n at record + 16n, holding the port in the byte at +36 and the mode in the byte at +37 (and copies of them at +38
and +39), a word at +32 and a doubleword pointer to the counter register at +40. It unlocks (`mutex_spinunlock`, 0xc000000000286400) and returns -1 when the requested
slot's mode byte is nonzero and the new mode is nonzero, or when the other slot already watches the same port (equal port byte) with a nonzero mode byte. Otherwise it
stores the mode at +37 and +39 and the port number minus 8 at +36 and +38 of the requested slot; rewrites bits 29 and 28 of the word at (register base + 64 * port -
244, which is the 64-byte per-port block's word at +268) with the low two bits of the mode; replaces bits 22 to 20 of the word the slot's pointer addresses with the
port number minus 8 and stores the same word masked to 20 bits in the slot's word at +32. It then calls `timeout` (0xc0000000001b3498) with the routine at
0xc00000000006e6c8 (`xbow_update_perf_counters`), the vertex zero-extended to 32 bits and 4 ticks, unlocks and returns 0. Return values by path: -1 for bad arguments
(0xc00000000006e834, 0xc00000000006e83c, 0xc00000000006e864, 0xc00000000006e888), for a busy slot (0xc00000000006e908, 0xc00000000006e9b4); 0 on success
(0xc00000000006e9a4).
- `xbow_ioctl` (`6.5.30/unix.IP27`, address 0xc00000000006d200, 876 bytes, 96-byte frame): **established.**
The control-call entry of the crossbar's character device. Its first instruction (0xc00000000006d200) zeroes the word that the fifth-argument pointer `a5` addresses,
before the frame is built. Arguments: `a0` (`s1`) the crossbar vertex, `a1` (`s0`) the command, `a2` (`s2`) a user pointer, `a3` (`s3`) the open flags (bit 0 read, bit
1 write as tested here) and `a4` the caller's credentials. Commands and their paths: 0 and any value not listed return 0. Command 1 and command 2 need the capability
bit 1 << 37 (`cap_able_cred`, 0xc000000000287350; failure returns 1), then the write flag (absent returns 9), then `copyin` (0xc00000000001f990) of 12 bytes from the
user pointer to sp+32 (failure returns 14); they call `xbow_enable_perf_counter` (0xc00000000006e7f0) with the vertex, the word at sp+32, the word at sp+36 (replaced by
0 for command 2) and the word at sp+40, and return 0 when it returns 0 and 22 otherwise. Command 3 needs the read flag (absent returns 9), calls
`xbow_get_perf_counters` (0xc00000000006e7d0), runs `assfail` (line 612, text "xbow_perf_cnt != NULL") when it returns null and `copyout` (0xc00000000001fa1c) of 256
bytes to the user pointer: 0 or 14. Command 4 needs the capability (else 1) and calls `xbow_enable_llp_monitor` (0xc00000000006eb38); command 5 needs it and calls
`xbow_disable_llp_monitor` (0xc00000000006eaf0); both return 0 or 22 from the helper's result. Command 6 needs the read flag (else 9), calls `xbow_get_llp_status`
(0xc00000000006e9bc), runs `assfail` (line 652, text "xbow_llp_status != NULL") on null and copies 128 bytes out: 0 or 14. Commands 26369 (0x6701) and 26370 (0x6702)
call `cap_able` (0xc00000000028741c) with the same capability (failure returns 1), `copyin` 16 bytes to sp+0 (26369) or sp+16 (26370) (failure returns 14), turn the
first two words into widget numbers with `xbow_widget_num_get` (0xc00000000006d104, masked to a byte) and call `xbow_prio_bw_alloc` (0xc00000000006f160) twice with the
two widgets swapped: for 26369 the fourth argument is 0 and the fifth the doubleword at sp+8; for 26370 the fourth is the doubleword at sp+24 and the fifth its
negation. The result is 0 either way. Error numbers are 1 (permission), 9 (wrong open mode), 14 (bad user address) and 22 (helper failed).
- `xbow_attach` (`6.5.30/unix.IP27`, address 0xc00000000006cd60, 796 bytes, 336-byte frame): **established.**
Attaches the crossbar (Xbow) widget: makes its control device, builds its soft record, hooks its error interrupt and error handler, and marks the live links of the
switch. `a0` (`s2`) is the widget vertex. It maps the widget's registers with `xtalk_piotrans_addr(vertex, 0, 0, 0x300, 0)` (0xc00000000006be68; result `s1`, the
register base), gets the connection-point vertex with `hwgraph_connectpt_get` (0xc0000000002968cc; `s3`) and adds a character device named "xbow" with prefix "xbow_"
(`hwgraph_char_device_add`, 0xc000000000295bec), the new device vertex being the word at sp+0. It allocates the 656-byte soft record with `kmem_zalloc`
(0xc00000000017e344) and stores the widget vertex at +0, the device vertex at +4, the connection-point vertex at +8 and the register base at +16, then
`hwgraph_fastinfo_set` (0xc000000000296068) on the device vertex. It reads the switch id (`xswitch_id_get`, 0xc00000000006ccbc), takes the device's name (`dev_to_name`,
256 bytes at sp+8), allocates strlen + 11 bytes (`kmem_alloc`, 0xc00000000017e220), stores the pointer at record +24 and formats it with `sprintf` as "%s[xbow# %d]". It
forms a revision code from the word at register base +4: the high nibble (bits 31 to 28) OR the 16-bit part number (bits 27 to 12) shifted left by 4. Only when that
code is below 2 does it select a revision string and print `cmn_err(4, "%sCrossbow ASIC: rev %s (code=%d) at %s%s", "DOWNREV ", string, code, record name, "")`: code 1
gives "1.0" and 0 gives "unknown" (the compare chain at 0xc00000000006cfdc also tests 3, 4, 5, 0xd0001 and 0xd0002, giving "1.2", "1.3", "2.0", "Xbridge 1.0" and
"Xbridge 2.0", but those tests follow the below-2 test and cannot be true); it then carries on. Next, `spinlock_init` (0xc00000000028618c) on record +448
("xbow_perf_lock") and +520 ("xbow_bw_alloc_lock"); record +40 and +56 receive register base +100 and +108; for each of 8 ports the doubleword at +528 + 8n is set to
800000000 and the one at +592 + 8n to 0. It duplicates the device descriptor of the device vertex (`device_desc_dup`, 0xc0000000001723e0), sets flag bit 0
(`device_desc_flags_get`/`_set`), names the interrupt "Crossbow error" (`device_desc_intr_name_set`), allocates the interrupt with `xtalk_intr_alloc`
(0xc00000000006c0c4) and connects it with `xtalk_intr_connect` (0xc00000000006c118) to `xbow_errintr_handler` (0xc00000000006da24, argument the record) with
`xbow_setwidint` (0xc00000000006d64c) as the set-widget-interrupt hook and the register base as the sixth value, frees the descriptor, registers `xbow_error_handler`
(0xc00000000006e018) with `xwidget_error_register` (0xc00000000006c8b8), stores 36 (0x24) in the word at register base +36, creates the switch information with
`xswitch_info_new` (0xc00000000006cba4) and, for ports 8 to 15, calls `xbow_link_alive` (0xc00000000006d5d0) and `xbow_widget_present` (0xc00000000006d56c) on the
register base; for each port where both are nonzero it calls `xswitch_info_link_is_ok` (0xc00000000006cc64) and replaces bits 8 to 4 and bit 0 of the word at register
base + 64 * (port & 7) + 268 with the constant 0x3002b (read-modify-write with mask 0xfffffe2f). It ends with
`xswitch_provider_register(connection point, xbow_provider)` (0xc00000000006cc3c; the provider table at 0xc0000000014cb0b8) and returns 0.
- `xbow_xmit_retry_error` (`6.5.30/unix.IP27`, address 0xc00000000006d920, 260 bytes, 32-byte frame): **established.**
Counts a transmit-retry error on one crossbar port when the widget behind the port is a Bridge or XBridge, and returns whether it was handled. `a0` is the crossbar soft
record and `a1` the port number (saved at sp+8). The record keeps one cached register pointer per port in a table of doublewords at +392 (the slot's address is saved at
sp+0). When the slot is empty it asks `xswitch_info_get` (0xc00000000006ca78) for the switch information of the connection-point vertex (the word at record +8), then
`xswitch_info_vhdl_get` (0xc00000000006cae8) for the port's vertex, and maps that vertex with `xtalk_piotrans_addr(vertex, 0, 0, 88, 0)` (0xc00000000006be68), storing
the result in the slot; a null switch information, a vertex of -1 or a failed mapping returns 1 (0xc00000000006d9c8, 0xc00000000006da14, 0xc00000000006da1c). With the
pointer in hand it reads the widget identification word at +4: the part number is bits 27 to 12 and the manufacturer bits 11 to 1. A part of 0xc002 with manufacturer 54
or a part of 0xd002 with manufacturer 36 is accepted; anything else returns 0. For an accepted widget it increments the global word `xbow_xmit_retry_errors` (at
0xc0000000014cb0c0, the gp-28848 slot), reads the word at +36 of the widget's registers and writes the same value back unchanged, reads it once more (the value is not
used) and returns 1. Why the value is written back, which matters only if the register reacts to a write, is unknown.
- `xbow_reset_link` (`6.5.30/unix.IP27`, address 0xc00000000006eefc, 260 bytes, 48-byte frame): **established.**
Resets one crossbar link after a widget error and waits for the link to come back, for a bounded time. `a0` (`s2`) is the vertex of the widget on the failing port. Two
timing words are involved: `xbow_reset_delay` (the word at gp-28844, the pause between polls in microseconds) and a calibration word at 0xc0000000014cb0c8 (the gp-28840
slot, 4 bytes after `xbow_reset_delay`). A pointer slot at 0xc0000000014cc150 holds 0x9200000001030100; the routine reads the low 32 bits of the doubleword there only
to difference two readings, so it is a free-running counter (inferred from that use). While the calibration word is zero it takes a reading, calls `us_delay(1000)`
(0xc000000000047c0c) and stores the counter difference, the number of counter ticks in one millisecond. It then takes the widget id through `xwidget_info_get`
(0xc00000000006c368) and `xwidget_info_id_get` (0xc00000000006c44c), finds the crossbar with `hwgraph_traverse(vertex, ".master/xtalk/0/xbow", &sp+0)`
(0xc00000000029579c) and its soft record with `hwgraph_fastinfo_get` (0xc000000000296098); the port block is the doubleword at record +16 plus 64 * (widget id & 7). It
saves the word at block +268 in the doubleword at sp+8, stores 0 in the word at block +308, delays `xbow_reset_delay` microseconds and then polls: if bit 31 of the word
at block +276 is set it writes the saved word back to block +268 and returns 0; otherwise, if more counter ticks than the calibration word have passed since the reading
taken after the store, it returns -1 (0xc00000000006efb4); otherwise it delays `xbow_reset_delay` microseconds and polls again. The meaning of bit 31 (link alive, going
by the name of the crossbar's own link-alive helper, which tests the same word and bit) is inferred.
- `xbow_port_io_enabled` (`6.5.30/unix.IP27`, address 0xc00000000001e574, 196 bytes, 16-byte frame): **established.**
Answers whether the KLCONFIG record of a node's crossbar marks one port as usable for I/O: 1 when two flag bits of the port's byte are both set, 0 when they are not or
when the component search finds nothing. `a0` is the NASID and `a1` (saved at sp+0) the port number. It forms the node's uncached base, -53 shifted left by 57
(0x9600000000000000) plus the NASID shifted left by 32, reads the doubleword at base + 0x2048, masks it to its low 40 bits, ORs that with the base and loads the word at
+20 of the result (that this is a header offset and the first board record's offset is inferred from how the value is used); a zero offset sends a null board (a0 is
cleared before the branch at 0xc00000000001e5b4) to the same `find_lboard` call. The board address is the 40-bit-masked sum of that offset and the NASID shifted left by
32, ORed with the uncached base. It calls `find_lboard(board, 65)` (0xc00000000001d8e4) and returns 0 when it returns null, then `find_component(board, 0, 4)`
(0xc00000000001da2c) and returns 0 when that returns null (0xc00000000001e628). The result is the byte at component + 8 * port - 6: it returns 1 (0xc00000000001e618)
when both bit 0 and bit 2 of that byte are set, and 0 (0xc00000000001e630) otherwise. The board type 65 and component type 4 are the values the code passes; that they
denote the crossbar board and its port list, and that the two bits mean "port present" and "I/O enabled", are inferred from the routine's name and use.

## PCI bridge interrupts and address translation

- `pcibr_intr_connect` (`6.5.30/unix.IP27`, address 0xc000000000078d74, 420 bytes, 80-byte frame): **established.**
Connects a PCI interrupt object to its handler: records the handler in the object, wires each of the bridge interrupt bits it covers to the crosstalk interrupt, and
enables the bits at the bridge. `a0` (`s5`) is the interrupt object (`pcibr_intr_s`), `a1` (`s4`) the handler function and `a2` (`s3`) its argument. The object supplies
the bit mask (word at +88, `s2`) and the bridge soft record (doubleword at +96, `s1`). A null object returns -1 at once (0xc000000000078dac). Otherwise it logs with
`pcibr_debug(4096, the word at object +4, "pcibr_intr_connect: intr_func=0x%lx, intr_arg=0x%lx\n", handler, argument)` (0xc000000000074e8c), stores the argument at +32
and the handler at +24 (in that order) and sets bit 0 of the flags word at +0. It then visits bits 0 to 7 of the mask. For each set bit n the per-bit record is soft +
1640 + 56 * n; if the word at its +40 (soft +1680 + 56n) is nonzero, the bit is already connected and only the doubleword at its +0 is loaded. If it is zero, the
routine calls `pcireg_intr_addr_addr(soft, n)` (0xc000000000085200; result kept at sp+40), stores `(word at soft +72) * 8 + n` in the word at record +48 (soft +1688 +
56n), calls `xtalk_intr_connect` (0xc00000000006c118) with the crosstalk interrupt doubleword at record +0, `pcibr_intr_func` (0xc0000000000791f0) as the handler, the
address record +8 as its argument, `pcibr_setpciint` (0xc000000000078ccc) as the set-interrupt hook, the address record +48 and 0, sets the word at record +40 to 1,
calls `pcireg_intr_addr_get(soft, n)` (0xc000000000084de4) and logs "pcibr_setpciint: int_addr=0x%lx, *int_addr=0x%lx, pcibr_int_bit=0x%x\n" with the address, the value
and the bit. After the loop it takes the soft record's lock at +128 (`mutex_spinlock`, 0xc0000000002863b4), calls `pcireg_intr_enable_bit_set(soft, mask zero-extended)`
(0xc000000000083b78), reads the bridge's flush register with `pcireg_tflush_get` (0xc000000000080ba8) and releases the lock with `mutex_io_spinunlock`
(0xc0000000002862c4), then returns 0 (0xc000000000078e74). The per-bit records are the `bs_intr` array of the soft record: crosstalk interrupt at +0, an embedded
40-byte wrapper at +8 whose +32 word is its connected flag, and the bridge interrupt bit at +48.
- `pcibr_init_ext_ate_ram` (`6.5.30/unix.IP27`, address 0xc00000000007566c, 352 bytes, 64-byte frame): **established.**
Finds how much external address-translation RAM (ATE RAM) a Bridge has, tells the bridge the size, clears it and returns the number of entries (0 when there is none).
`a0` (`s1`) is the bridge soft record. The candidate sizes are the words of the table whose pointer is in the gp-17776 slot (0xc0000000014cdc00, pointing at
0xc000000001437a78): 0, 8192, 16384 and 65536 entries, index 0 to 3. The loop (`s0`, 1 to 3) writes the doubleword 0x0123456789abcdef (the pool constant at
0xc0000000014cc608) into the last entry of the candidate size (index size - 1) with `pcireg_ext_ate_set` (0xc000000000088ad0); from the second candidate tested on it
also writes 0 into the last entry of each smaller candidate (inner loop `s4` from 1 below `s0`); it reads the candidate's last entry back with `pcireg_ext_ate_get`
(0xc000000000088904) and, when the value equals the pattern, records that candidate index in `s3`, so the largest candidate that reads back correctly wins (the loop
does not stop at the first failure). It calls `pcireg_ssram_size_set(soft, s3)` (0xc00000000007f698) and takes the entry count from the table at that index. When the
word `pcibr_debug_mask` (the gp-28800 slot) has bit 0x10 set it logs through `pcibr_debug(16, the word at soft +4, ...)` (0xc000000000074e8c): "bridge at 0x%lx:
clearing %d external ATEs\n" with the doubleword at +8 and the count, or, for a zero count, "bridge at 0x%x: no external ATE RAM found\n". It finally zeroes every entry
from 0 to count - 1 with `pcireg_ext_ate_set` and returns the count. Exits: 0xc000000000075768 (count zero, returns `s2`) and 0xc000000000075788 (after the clear loop).
- `pcibr_directmap_init` (`6.5.30/unix.IP27`, address 0xc000000000071c74, 300 bytes, 288-byte frame): **established.**
Sets up the bridge's direct-mapped DMA window so that it points at the memory of one node. `a0` (`s0`) is the bridge soft record. It reads the device-administration
string "PCIBUS_DMATRANS_NODE" for the bridge's vertex (the word at soft +4) with `device_admin_info_get` (0xc0000000001727e8; the string pointer is saved at sp+256).
With no string the node is compact node 0. With a string it resolves the path with `hwgraph_path_to_vertex` (0xc0000000002958f8) and the vertex with
`nodevertex_to_cnodeid` (0xc000000000296e54); a path that does not resolve, or a vertex that is not a node (halfword -1), prints
`cmn_err(4, "Invalid hwgraph node path specified:\n    DEVICE_ADMIN: %s %s=%s\n", bridge name, "PCIBUS_DMATRANS_NODE", the string)` (the name comes from
`vertex_to_name` into sp+0) and falls back to node 0. The NASID is the halfword at the node's slot of the table behind the gp-17824 slot (0xc0000000014cdbd0). It calls
`xtalk_dmatrans_addr(soft's connection vertex, 0, NASID shifted left by 32, 16384, 0)` (0xc00000000006c038) for the crosstalk address of that node's memory, sets the
direct-map offset with `pcireg_dirmap_diroff_set(soft, address >> 31)` (0xc000000000081a2c) and the direct-map widget with
`pcireg_dirmap_wid_set(soft, the byte at +48)` (0xc000000000081890), and copies the byte at +48 to +88. When the crosstalk address is exactly 0x20000000 it calls
`pcireg_dirmap_add512_set` (0xc000000000081c1c) and stores 0x20000000 in the doubleword at +80; otherwise it calls `pcireg_dirmap_add512_clr` (0xc000000000081d9c) and
stores the address ANDed with 0xffffffff80000000 at +80. It returns the result of that last call, which carries no meaning. The instruction at 0xc000000000071d6c is
never reached. The reading of the +48 and +88 bytes as the master widget id and the direct-map widget is from the structure's member names.
- `pcibr_force_interrupt` (`6.5.30/unix.IP27`, address 0xc00000000007824c, 200 bytes, 32-byte frame): **established.**
Makes the bridge raise one of its PCI interrupt bits by software, in the way the bridge type allows. `a0` points to an interrupt wrapper whose doubleword at +0 is the
bridge soft record (`s0`) and whose word at +8 is the interrupt bit number (`s1`). When the halfword at soft +114 (the bridge type) is nonzero it just calls
`pcireg_force_intr_set(soft, bit)` (0xc0000000000863c8) and returns its result (0xc000000000078278 onwards). For type 0 it reads the status with
`pcireg_intr_status_get` (0xc0000000000835d4) and returns when the bit is not set in it (`v0` is then the status AND the bit mask, zero). Otherwise it takes the
crosstalk interrupt object of that bit, the doubleword at soft + 1640 + 56 * bit, asks for its vector with `xtalk_intr_vector_get` (0xc00000000006c2ec; kept as `s0`)
and its processor with `xtalk_intr_cpuid_get` (0xc00000000006c16c), and stores the value `vector | 0x100` (the vector as a sign-extended halfword, then truncated to 32
bits) in the doubleword at (the pool constant 0x9200000001800000 at 0xc0000000014cc168, plus the NASID of that processor shifted left by 32) + 144. The NASID is the
halfword at +1060 of the processor's private-data record, reached through the 16-byte-per-processor table whose pointer is in the gp-17736 slot (0xc0000000014cdc28,
pointing at 0xc0000000014d5020), the second doubleword of each entry being the pointer to that record. That the store raises the interrupt on that processor (as a software-generated interrupt for the vector) is inferred
from the address and value; the code does not name the register.
- `pcibr_intr_func` (`6.5.30/unix.IP27`, address 0xc0000000000791f0, 592 bytes, 96-byte frame): **established.**
The interrupt handler that `pcibr_intr_connect` registers for each bridge interrupt bit: it runs the handlers connected to the bit, either directly or through their
interrupt threads, and turns the bit off when nothing is connected. `a0` (`s5`) is the per-bit wrapper (`pcibr_intr_wrap_s`): the doubleword at +0 is the bridge soft
record (`s7`), the word at +8 the bit number, the doubleword at +16 the head of a list of connected handles, the word at +24 a count of queued thread requests and the
word at +28 (`iw_shared` in the kernel's structure definition) a flag that, when set, makes each handler be preceded by a write-buffer flush read. Each list node has
the next link at +0, the handle (`pcibr_intr_s`) at +8, a soft record at +16 and a slot number held in a byte at node offset 24. Step 1: when the halfword at soft +114 (the bridge type) is 2 and
the low nibble of the word at soft +120 is 1 (the test is `(1 << nibble) & 2`), it takes the soft lock at +128 (`mutex_spinlock`, 0xc0000000002863b4), clears and sets
the bit's enable bit with `pcireg_intr_enable_bit_clr` (0xc0000000000839f8) and `pcireg_intr_enable_bit_set` (0xc000000000083b78), and releases the lock with
`mutex_io_spinunlock` (0xc0000000002862c4), so a level that is still asserted is seen again. Step 2: when the word at wrapper +24 is nonzero it returns at once
(0xc0000000000793a4). Step 3: one or two passes over the list. A node counts only when bit 0 of its handle's flags word is set. Pass 1: a handle whose flags bit 1 is
clear is queued for its thread: with the wrapper's +28 flag set it first calls `pcireg_wrb_flush_get(node soft, node slot)` (0xc000000000086e94), then adds 1 to the
word at wrapper +24 with an `ll`/`sc` loop (the retry branch at 0xc000000000079338 is branch-likely), calls `pcibr_wrap_put(wrapper, handle + 104)` (0xc0000000000781b8)
and `vsema(handle + 48)` (0xc000000000187618). A handle with bit 1 set is only counted in `s2` in pass 1. A second pass (with `s3` zero) runs only if pass 1 counted
such a handle: it calls each bit-1-set handle's function, the pointer at handle +24, with its argument from handle +32 (an indirect call, `jalr`, after the same
optional flush); handles with bit 1 clear are skipped in pass 2. The register `s6` starts at 1 and is cleared (several times, partly in branch delay slots at
0xc00000000007928c and 0xc000000000079294) whenever a node with bit 0 set is reached. Step 4: at the end of pass 2, if the word at wrapper +24 is zero it calls
`pcibr_force_interrupt(wrapper)` (0xc00000000007824c); this test is not reached after a single pass. Step 5: if `s6` is still 1, no connected handle was seen, and it
takes the soft lock, clears the bit's enable bit and reads `pcireg_tflush_get` (0xc000000000080ba8) before unlocking. It returns nothing: every path ends in the
epilogue at 0xc0000000000793a8 to 0xc0000000000793cc (step 2 enters it at 0xc0000000000793a4). That flags bit 1 marks handlers that run in interrupt context is inferred
from which handles are called directly.
- `pcibr_detach` (`6.5.30/unix.IP27`, address 0xc000000000071ae4, 400 bytes, 64-byte frame): **established.**
Tears down a Bridge: masks its interrupts, detaches every slot, frees its records and removes its hardware-graph entries. `a0` (`s1`) is the Bridge's widget vertex. It
logs `pcibr_debug(8, vertex, "pcibr_detach\n")` (0xc000000000074e8c), then calls `hwgraph_traverse(vertex, "pci", &sp+0)` (0xc00000000029579c); a nonzero result returns
1 (0xc000000000071b28; the `li v0,1` is in the branch's delay slot). Otherwise the child vertex at sp+0 gives the soft record through `hwgraph_fastinfo_get`
(0xc000000000296098, `s0`). It takes the soft lock at +128 (`mutex_spinlock`, 0xc0000000002863b4), calls `pcireg_intr_enable_set(soft, 0)` (0xc0000000000838a0), which
writes an empty interrupt enable mask, and releases the lock with `mutex_io_spinunlock` (0xc0000000002862c4) and the saved state. It then calls
`pcibr_slot_detach(child, slot, 0, 0, 0)` (0xc00000000007e52c) for each slot from the byte at soft +56 up to and including the byte at soft +57 (a signed compare of the
slot, masked to 8 bits, with the last plus 1). Next, `pciio_device_info_unregister(child, the doubleword at soft +248)` (0xc000000000093a44), `spinlock_destroy` of the
soft lock (0xc000000000286198) and `strlen` (0xc000000000020244) of the string pointed to by the doubleword at +16 followed by `kmem_free` (0xc00000000017e3b0) of that
pointer with the length as the size. Each of the doublewords at +96 and +104, when nonzero, goes to `rmfreemap` (0xc0000000001a2a64). The crosstalk interrupt object,
the doubleword at +2088, is passed to `xtalk_intr_disconnect` (0xc00000000006c134) and then to `xtalk_intr_free` (0xc00000000006c0fc). `kmem_free(soft, 2744)` frees the
record. Last, `hwgraph_info_remove_LBL(child, "_pcibr_asic_rev", 0)` (0xc0000000002952a0), `hwgraph_edge_remove(child, "controller", 0)` and
`hwgraph_edge_remove(vertex, "pci", 0)` (0xc000000000294ef4) remove the entries, and the routine returns 0 (0xc000000000071c70). The meaning of the doublewords at +96,
+104 and +248 is not read here; the first two are freed with `rmfreemap`, so they are resource maps (inferred from the callee). No branch-likely instruction.

## Hub vector access and router state

- `hub_vector_read` (`6.5.30/unix.IP27`, address 0xc00000000001d48c, 224 bytes, 80-byte frame): **established.**
Runs `vector_read_node` (0xc00000000001ce08) on a given node under that node's own lock, but only when the caller is running on that node; otherwise it fails with -10.
The five arguments `a0` to `a4` are a node number (`a0`, `s3`), then four values that are passed on untouched (`a1` saved at sp+16, `a2` at sp+40, `a3` at sp+8, `a4` at
sp+24). It reads the NASID of the node from the halfword table behind the gp-20792 slot (0xc0000000014cd038, pointing at 0xc0000000014d4e80), indexed by `a0` and saved
at sp+32, and finds the node's record through the table behind `p_nodepda` (absolute address 0xffffffffffffc400, the doubleword at index `a0`). The sequence is:
`in_panic_mode` (0xc0000000001e5a94); when it returns zero, `mutex_spinlock` (0xc0000000002863b4) on the lock at record +18768, the lock state being the word at sp+0
(when the machine is in panic mode no lock is taken). Then the halfword at absolute address 0xffffffffffffc422 (the number of the node the processor runs on) is
compared with `a0`. Equal: the byte at record +18772 is set to 1, `vector_read_node` is called with the saved `a1`, the NASID, the saved `a2`, `a3` and `a4`, its result
is kept in `s1` and the byte is cleared again. Not equal: `s1` is -10 and nothing is called. `in_panic_mode` is tested a second time and, when it returns zero,
`mutex_spinunlock` (0xc000000000286400) releases the lock with the saved state. The routine returns `s1`; the panic-mode path and the unlock path meet at the epilogue
whose `jr ra` is at 0xc00000000001d514.
- `hub_vector_write` (`6.5.30/unix.IP27`, address 0xc00000000001d56c, 224 bytes, 80-byte frame): **established.**
The write counterpart of `hub_vector_read`: runs `vector_write_node` (0xc00000000001cbd0) on a given node under that node's lock, only when the caller runs on that
node, and otherwise fails with -10. Its structure is the same as the read routine apart from the callee and the position of the load of the saved lock state, which here
sits in the delay slot of the second panic-mode test. Arguments: a node number (`a0`, `s3`) and four values passed on (`a1` at sp+16, `a2` at sp+40, `a3` at sp+8, `a4`
at sp+24). The NASID comes from the halfword table behind the gp-20792 slot (0xc0000000014cd038) indexed by `a0` (saved at sp+32); the node record comes from the table
behind `p_nodepda` (absolute 0xffffffffffffc400). When `in_panic_mode` (0xc0000000001e5a94) returns zero it takes `mutex_spinlock` (0xc0000000002863b4) on the lock at
record +18768 (state at sp+0). If the halfword at absolute address 0xffffffffffffc422 equals `a0` it sets the byte at record +18772 to 1, calls `vector_write_node` with
the saved `a1`, the NASID and the saved `a2`, `a3`, `a4`, keeps the result in `s1` and clears the byte; otherwise `s1` is -10. A second `in_panic_mode` test decides
whether `mutex_spinunlock` (0xc000000000286400) runs on the lock at +18768. It returns `s1`; the panic-mode path and the unlock path meet at the epilogue whose `jr ra`
is at 0xc00000000001d5f8.

## Hub PIO maps

- `hub_piotrans_addr` (`6.5.30/unix.IP27`, address 0xc00000000003ba0c, 196 bytes, 64-byte frame): **established.**
Translates a window of a crosstalk widget's address space (an offset and a length) into a processor-visible PIO address through the master hub, or returns 0 when the
window is too large. `a0` is the widget vertex (passed straight to `xwidget_info_get`, 0xc00000000006c368), `a1` is not used, `a2` (`s1`) is the offset, `a3` (`s0`) the
length and `a4` (saved at sp+16) the flags. It takes the widget id from `xwidget_info_id_get` (0xc00000000006c44c; kept at sp+8) and the master from
`xwidget_info_master_get` (0xc00000000006c480), and reads the master's `_node` label with `hwgraph_info_get_LBL` (0xc000000000295310) into the doubleword at sp+0; the
result of that lookup is not tested. When offset + length is above 0x1000000 it returns 0 (0xc00000000003ba64). Otherwise it calls `hub_piomap_addr`
(0xc00000000003b9a4) with the address (the doubleword at sp+0) + 16 + 48 * (widget id & 0xff) (a 48-byte record per widget), the offset and the length. If bit 3 of the
flags is set the result has bits 61 to 59 set (7 shifted left by 59). The result is returned (0xc00000000003bab4 falls to the epilogue).
- `hub_piomap_free` (`6.5.30/unix.IP27`, address 0xc00000000003b8b8, 236 bytes, 48-byte frame): **established.**
Drops one use of a hub PIO map and, when it was the last, releases the hub window register it held and wakes a waiter. `a0` (`s0`) is the map. A map whose flags word at
+40 lacks bit 1 is ignored (the routine returns with `v0` unset, 0xc00000000003b8cc). Otherwise it reads the `_node` label of the vertex held in the word at map +32
(`hwgraph_info_get_LBL`, 0xc000000000295310, into sp+0), takes the node record's lock at record +792 (`mutex_spinlock`, 0xc0000000002863b4), after saving the NASID (the
halfword at record +10) at sp+8, and decrements the halfword use count at map +36. If the count is not zero it just unlocks (`mutex_spinunlock`, 0xc000000000286400).
When it reaches zero it stores 768 in the doubleword at the hub register page (the pool constant 0x9200000001c00000 plus the NASID shifted left by 32) + 352 + 8 * (the
byte at map +38), then updates the flags: if bit 2 is set it clears bits 0 and 2 and decrements the word at record +800; if bit 2 is clear it clears bit 0 only (and
leaves the record's word alone). Either way it calls `sv_signal` (0xc00000000018bf30) on the record at +784 and then unlocks the lock at +792 with the saved state. The
return value is that of `mutex_spinunlock`. The reading of the register as a window register and of +784 as a wait queue is inferred from the callee names.
- `hub_dma_enabled` (`6.5.30/unix.IP27`, address 0xc0000000000277e8, 196 bytes, 48-byte frame): **established.**
Says whether a crosstalk widget may send DMA to its master hub, and prints a message when it may not. `a0` (saved at sp+8) is the widget vertex. It takes
`xwidget_info_get(a0)` (0xc00000000006c368), the widget id from `xwidget_info_id_get` (0xc00000000006c44c; saved at sp+24) and the master from `xwidget_info_master_get`
(0xc00000000006c480; saved at sp+16), and reads the master's `_node` label with `hwgraph_info_get_LBL` (0xc000000000295310) into the doubleword at sp+0 (the call's own
result is not tested). It returns 0 when that doubleword is zero (0xc000000000027880) or when the halfword at +10 of the record it points to, the NASID, is -1
(0xc000000000027888). Otherwise it tests bit (widget id & 0xff) of the doubleword at +280 of the node's register page (the pool constant 0x9200000001c00000 plus the
NASID shifted left by 32) and returns 1 when it is set. When it is clear it calls
`cmn_err(6, ...)` with a three-line message that starts "No Inbound access" and then gives the widget id and vertex and the node's NASID and master vertex, in that order (0xc0000000001fcbd4) and returns 0. No
branch-likely instruction. `hub_device_inquiry` reads the same register and bit and prints them as "Inbound access".
- `hub_error_devenable` (`6.5.30/unix.IP27`, address 0xc000000000027b24, 196 bytes, 48-byte frame): **established.**
Acts on one widget's entry in the hub's table when the command in `a2` is 5; with command 6 or any other value it does nothing. `a0` is the widget vertex, `a1` is not
used and `a2` (`s0`) is the command. It takes `xwidget_info_get` (0xc00000000006c368), the widget id from `xwidget_info_id_get` (0xc00000000006c44c; saved at sp+8) and
the master from `xwidget_info_master_get` (0xc00000000006c480), and reads the master's `_node` label with `hwgraph_info_get_LBL` (0xc000000000295310) into sp+0. The
record pointer is never tested for null: the halfword at +10 (the NASID) is loaded in the delay slot of the first command comparison at 0xc000000000027b7c, before
either command is looked at. For command 5 the entry is the doubleword at (the pool constant 0x9200000001c00000 plus the NASID shifted left by 32) + 712 + 8 * (widget
id & 0xff); when its bit 56 is set (the value shifted left 7 is negative) the routine stores `(widget id & 0xff) | 0x10` in the doubleword at page +904 of the same
page. The routine always returns 0 (exits 0xc000000000027b90, 0xc000000000027bc4 and 0xc000000000027bd0). `hub_widget_reset` writes `index | 0x10` to the same +904
register after clearing an entry of the neighbouring table at +776. No branch-likely instruction.

## Hub address checks, error counters and kick set-up

- `hub_check_is_widget0` (`6.5.30/unix.IP27`, address 0xc00000000003c1cc, 124 bytes, no frame): **established.**
Says whether an address lies in one of two windows of the node it names, returning 1 inside either and 0 otherwise. `a0` is the address; no other argument is read. The node number
is the byte in bits 39 to 32 of `a0` (`dsrl32` then `andi` 0xff, then a 16-bit sign extension that cannot change a value of 0 to 255), moved back up to bit 32 and added to four constants
to give four limits: 0x9200000000000000 (`li` -55 shifted left by 57), and three doublewords loaded through `gp`, 0x9200000001000000 (the word at 0xc0000000014cc2b0), 0x92000000e0000000
(0xc0000000014cc1b8) and 0x9200000100000000 (0xc0000000014cc228). The routine returns 1 when the address is at or above the first limit and below the second (a window of 0x1000000 bytes
at the start of the node's range), and also when it is at or above the third and below the fourth (the top 512 MB of the node's 4 GB, ending where the next node's range begins).
`v0` is 1 from the start; the `move v0,zero` at 0xc00000000003c228 is in the delay slot of the `bnez` at 0xc00000000003c224, so it always runs, and a hit on the first window leaves through
0xc00000000003c240 before it. The exits are 0xc00000000003c238 (1, both bounds of the second window hold) and 0xc00000000003c240 (1 when the first window matched, otherwise 0). The one caller is
`hub_check_window_equiv`. No branch-likely instruction. That the windows belong to widget 0 is inferred, from the symbol's spelling alone; the code names neither.
- `hub_check_window_equiv` (`6.5.30/unix.IP27`, address 0xc00000000003c248, 104 bytes, 32-byte frame): **established.**
Decides whether two addresses name the same window position, returning 1 when both are in the widget-0 windows or when their bits 31 to 24 match. `a0` is saved at sp+0 and
`a1` is kept in `s0`. It calls `hub_check_is_widget0(a0)` (0xc00000000003c1cc); a zero result goes to the comparison. A nonzero result calls it again on `a1`, and a second
nonzero result returns 1 (the `li v0,1` at 0xc00000000003c27c, in the delay slot of the unconditional branch at 0xc00000000003c278). In every other case it reloads `a0`, takes the byte in bits 31 to 24 of each
(`dsrl` by 24, `andi` 0xff) and returns 1 when the two bytes are equal and 0 when they differ (the `move v0,zero` at 0xc00000000003c298, in the delay slot of the compare-and-branch at 0xc00000000003c294, is
overwritten by the `li v0,1` that follows when the bytes match). The `ld at,0(sp)` at 0xc00000000003c280 follows an unconditional `b` and nothing branches to it. Its one caller
is `hub_check_pci_equiv`. No branch-likely instruction.
- `hub_check_pci_equiv` (`6.5.30/unix.IP27`, address 0xc00000000003c2b0, 136 bytes, 32-byte frame): **established.**
Decides whether two crosstalk-side addresses name the same PCI location, allowing for the two node numbers being aliases of one node. `a0` (`s0`) and `a1` (`s1`) are the two
addresses; the routine has one exit, 0xc00000000003c2fc, and returns 0 or the result of `check_nasid_equiv`. It calls `hub_check_window_equiv(a0, a1)` (0xc00000000003c248); a
zero result returns 0. It then compares the low 24 bits of the two addresses (mask 0x00ffffff from `lui` 0xff and `ori` 0xffff) and returns 0 when they differ. When they match
it takes bits 39 to 32 of each address (`dsrl32`, `andi` 0xff, then a 16-bit sign extension that cannot change a value of 0 to 255) and returns
`check_nasid_equiv(first, second, 0)` (0xc00000000001f900, 64 bytes, described in `docs/klconfig.md`; it reads only `a0` and `a1`), whose result is left in `a2` by the `move a2,v0` at 0xc00000000003c328, in the
delay slot of the unconditional branch at 0xc00000000003c324, and moved to `v0` at the exit. `ioc3_attach` calls this routine twice (0xc000000000095380 and 0xc000000000095390). The IP35 kernel's routine of the same name takes
the node from bits 39 to 33 (see `docs/ip35-hub-errors.md`); this build takes eight bits. No branch-likely instruction.
- `elsc_nvram_magic` (`6.5.30/unix.IP27`, address 0xc000000000099404, 68 bytes, 16-byte frame): **established.**
Checks that the system controller's non-volatile memory carries the expected marker byte. `a0` is passed on unchanged to `elsc_nvram_read(a0, 1792, &sp+0, 1)`
(0xc000000000098c64): one byte from offset 0x700 into the byte at sp+0. A negative result is returned as it is (the `bltz` at 0xc000000000099420 branches to the `b` at
0xc000000000099440 with `v0` untouched). Otherwise the byte is loaded (`lbu`, in the branch delay slot) and the routine returns 0 when it equals 0x37 (the ASCII digit
'7') and -124 when it does not (`li v0,-124`, then a `movn` of zero on equality). Sixteen routines call it before reading or writing a field: `elsc_debug_set` and `_get`,
`elsc_module_set` and `_get`, `elsc_partition_set` and `_get`, `elsc_domain_set` and `_get`, `elsc_bist_set` and `_get`, `elsc_cell_set` and `_get`, `elsc_cluster_set` and `_get`,
and `elsc_password_set` and `_get`. That the byte is a validity marker for the area is inferred from the use; the code does not name it. No branch-likely instruction.
- `hub_error_count_enable` (`6.5.30/unix.IP27`, address 0xc000000000035238, 172 bytes, 32-byte frame): **established.**
Starts the periodic hub error counting for one node; returns 0 on success and -1 when the node is out of range, unknown, or already counting. `a0` (`s0`) is the NASID. It returns -1
when `a0` is negative or not below 256 (`slti`), reads the halfword `nasid_to_compact_node[a0]` (the array reached through the global pointer slot at 0xc0000000014cd2b0) and returns -1
when that is -1. The record pointer is the doubleword at `hub_err_count` + 8 x compact node, where `hub_err_count` is `kl_error_log` (the slot at 0xc0000000014cd268) plus 512
(the address is saved at sp+0). When the first word of that 64-byte block is already nonzero the routine returns -1. Otherwise it calls `blkclr(block, 64)` (0xc00000000001fd0c; the `li a1,64` is in
the delay slot), then `timeout(hub_error_count_update, a0, 100, 1)` (0xc0000000001b3498; the function is the slot at 0xc0000000014cd2c8, the delay slot of the call stores 1 in the block's
first word, so the flag is set before `timeout` runs) and returns 0. The timed routine (`hub_error_count_update`, 0xc0000000000351c0) passes block +8 to the routine at 0xc000000000035094 and block +32 to the one at 0xc0000000000350fc
and, while the first word is nonzero, schedules itself again 100 ticks later. The only caller is the `syssgi` routine (0xc0000000002317e0), at 0xc000000000235474. No branch-likely instruction.
- `hub_error_count_disable` (`6.5.30/unix.IP27`, address 0xc0000000000352e4, 84 bytes, no frame): **established.**
Stops the counting that `hub_error_count_enable` started. `a0` is the NASID. It returns -1 (exit 0xc000000000035330, `li v0,-1` in the delay slot) when `a0` is negative or not below 256, and -1
at 0xc000000000035328 when `nasid_to_compact_node[a0]` is -1. Otherwise it loads the block pointer from `hub_err_count[compact node]` (the slot at 0xc0000000014cd268 plus 512 plus 8 times the
node), stores zero in the block's first word (`sw zero,0(a1)`) and returns 0; nothing else in the block is cleared, and the pending `timeout` runs once more and then stops itself because the word is zero.
The only caller is the `syssgi` routine (0xc0000000002317e0), at 0xc000000000235414. No branch-likely instruction.
- `hub_error_count_get` (`6.5.30/unix.IP27`, address 0xc000000000035338, 88 bytes, no frame): **established.**
Gives a caller the count block of one node. `a0` is the NASID and `a1` points to a word. It returns 0 when `a0` is negative, not below 256, or when `nasid_to_compact_node[a0]` is -1 (the three
early returns at 0xc000000000035380 and 0xc000000000035388, `move v0,zero` in their delay slots), and nothing is stored in those cases. Otherwise it stores 64 in the word at `a1`
and returns the doubleword at `hub_err_count` + 8 x compact node (`kl_error_log` plus 512), the pointer to the 64-byte block (`v0` at exit 0xc000000000035378). The block is not read or
copied here. The only caller is the `syssgi` routine (0xc0000000002317e0), at 0xc000000000235128. No branch-likely instruction.
- `ioc3_subdev_enabled` (`6.5.30/unix.IP27`, address 0xc0000000000961d4, 72 bytes, 16-byte frame): **established.**
Says whether one sub-device of an IOC3 is enabled. `a0` is the PCI device vertex and `a1` (`s8`) the sub-device number. It calls the unlabelled routine at 0xc000000000096118 (not a symbol; the
symbol table puts it inside `ioc3_console_vhdl_get`) with `a0` and 0. That routine does `hwgraph_traverse(a0, ".host", &slot)`, then `hwgraph_info_get_LBL(slot, "_subdevs", &value)` and, with
its second argument zero, returns the label's value (zero when none exists); with a nonzero second argument it would allocate 8 bytes, store 255, add the label and free the block if another caller's
block won, a path this routine never takes. A zero answer makes the mask 255 (`li a2,255`); otherwise the mask is the doubleword the answer points to (`ld a2,0(v0)`). The result is the mask ANDed with 1
shifted left by `a1` (`sllv`, so only the low five bits of `a1` count), cut to 32 bits with `sll v0,v0,0`: nonzero exactly when the bit is set. Callers are `eiattach`, `ioc3_attach`,
`pckm_attach`, `if_efattach` and several sites in `ioc3_serial_attach`. No branch-likely instruction.
- `hub_widgetdev_enable` (`6.5.30/unix.IP27`, address 0xc000000000027a84, 160 bytes, 48-byte frame): **established.**
Sets one bit in a hub register to allow a crosstalk widget's device to act, unless it is already set. `a0` is the widget vertex and `a1` (saved at sp+16) a device number. It calls
`xwidget_info_get(a0)` (0xc00000000006c368), then `xwidget_info_id_get` (0xc00000000006c44c) on the result (saved at sp+24) and `xwidget_info_master_get` (0xc00000000006c480), and reads the master's
`_node` label with `hwgraph_info_get_LBL` into the doubleword at sp+0; a zero there returns at once with nothing stored. Otherwise the NASID is the halfword at the node record +10, and the register is
the doubleword at the page 0x9200000001c00000 (the word at 0xc0000000014cc198) plus the NASID shifted left by 32, offset 288. The bit number is `a1` plus 8 times the widget id (`andi` 0xff) minus 64,
added as 32-bit values and used by `dsllv`, which keeps only the low six bits; for widget ids 8 to 15 that is `a1` + 8 x (id - 8), and ids 0 to 7 wrap onto the same positions. When the bit is
already set (the `and` of the shifted 1 with the old value is nonzero) nothing is stored; when clear, the old value ORed with the bit is stored back (`sd` at 0xc000000000027b20, in the delay slot of the
`b`). No value is returned on purpose (`v0` is whatever the last call or the table address left). `hub_dma_enabled` tests the doubleword 8 bytes lower in the same page. The one caller is `xtalk_widgetdev_enable`.
No branch-likely instruction.
- `hub_setup_kick` (`6.5.30/unix.IP27`, address 0xc000000000029028, 156 bytes, 32-byte frame): **established.**
Initialises the locks and flags the DMA-timeout recovery uses for one node. `a0` (saved at sp+8) is the compact node number. When it is zero, the routine first calls
`spinlock_init(0xc0000000014d2ae0, "global_kick_lock")` and `spinlock_init(0xc0000000014d2ae4, "kick_mask_lock")` (0xc00000000028618c; the two addresses are the symbols `global_kick_lock` and `kick_mask_lock`, formed as `gp` plus a constant; the
names are the strings 5632 and 5656 bytes into the same string area that holds "_node"). For every node it then takes the node record from the table whose pointer is at absolute address
0xffffffffffffc400 (doubleword 0, indexed by 8 times the node number) and calls `spinlock_init(record + 19312, "dma_kick_lock")`, stores 1 in the word at record +19308 and then 0 in the word at +19304 (that last
store is in the delay slot of the final `jr ra`). `hub_dmatimeout_kick` is the routine that uses these locks. The only caller is `per_hub_init` (0xc000000000026de8). No value is set for the caller. No
branch-likely instruction.
- `intrnorm_rsasave` (`6.5.30/unix.IP27`, address 0xc00000000004c270, 308 bytes, no stack frame): **established.**
A side path of the interrupt entry that saves the interrupted registers into a reserved area chosen by a table lookup instead of the usual frame, then enters the common interrupt code.
It is not a C function and has no symbol-table size of its own; the only branch into it is the `bnez` at 0xc00000000004c494 in `intrnorm`, taken when the byte at the per-processor area +1492
is nonzero. `k1` holds the per-processor area (the doubleword at absolute address 0xffffffffffffc018, loaded at 0xc00000000004c48c). The routine takes the doubleword at area +1480 into `k0`; zero jumps to
0xc00000000004c4a0, the tail of `intrnorm`. Otherwise it takes the doubleword at area +832 (zero goes to 0xc00000000004c498) and the word at +3660 of that structure, which must be 1 to 1024 (the `blez` and the
`addi`/`bgtz` pair at 0xc00000000004c28c to 0xc00000000004c298 reject anything else). It reads the halfword at (the +1480 table) + 256 + 2 x that word; zero is rejected, and so is a value not below the
unsigned halfword at area +1488. A rejection goes to 0xc00000000004c498, which stores 2 in the byte at area +1490 and continues at 0xc00000000004c4a0. Note that `k1` is overwritten by the +1488 load and compare
(0xc00000000004c2b4, 0xc00000000004c2b8), so on the last rejection (0xc00000000004c2bc) `k1` is 0 rather than the area pointer: the stores and loads at 0xc00000000004c498 and 0xc00000000004c4a0 then
address the absolute locations 1490 and 1000. That path is read from the code, not observed. On acceptance the halfword is an index into an array of 640-byte save frames starting at table + 2432 (index shifted left 7, plus the same value shifted left 9, added to the base); the byte at area +1490 is set to 1 (the
store at 0xc00000000004c2dc). With `k1` pointing at the chosen frame the routine stores the old `sp` at +232 and the old `gp` at +224, loads `sp` from absolute address 0xffffffffffffc030, builds `gp` as the kernel global
pointer 0xc0000000014d2170 (`lui` 0xc000, shifts and adds of 0, 333 and 8560), and stores `a0` to `a7` at +32 to +88, `t0` at +96, `t1` at +104, `t2` and `t3` at +112 and +120, `s0` to `s7` at +128 to +184, `t8` and `t9` at +192 and +200,
`s8` at +240, `ra` at +248, `v0` at +16 and `v1` at +24. The multiply LO and HI values go to +256 and +264 (`mflo` and `mfhi` are taken after `t0` and `t1` are saved), the exception PC (`dmfc0` of CP0 register 14) to +280, and the
doubleword at absolute address 0xffffffffffffc038 to +8 (it is loaded into `at` at 0xc00000000004c2f4, so the interrupted `at` is not saved). `k0` and `k1` are not saved. It finally sets `k0` to 2, takes `k1` as the doubleword at area +1000 plus 360 and jumps to
`ecommon_allsaved` (0xc00000000004c940). No branch-likely instruction.
- `ioc3_ei_intr` (`6.5.30/unix.IP27`, address 0xc0000000000942a4, 308 bytes, 48-byte frame): **established.**
Interrupt handler that `eiattach` connects for IOC3 interrupt bits 0x18000000 (the `lui` 0x1800 at 0xc0000000000945b8 before the `ioc3_intr_connect` call; the handler is the pointer in the word at 0xc0000000014cddf0, loaded
at 0xc0000000000945b4). It counts the interrupt, wakes waiters and re-enables the bits. `a0` (`s1`) is its record: the doubleword at +8 is the chip's register block, +16 a mask word, the record address plus 20 a lock, +24 the head of a list of
waiters, +32 a pointer to a counter word, +44 a word, +48 the chip's soft record. First it stores 0x18000000 in the word at register block +28. When the word at absolute address 0xffffffffffffc6a0 is nonzero it calls
`frs_handle_eintr` (0xc0000000003c38bc). Otherwise, when the word at +44 is not negative, it calls `uli_callup(word at +44)` (0xc000000000211c70). When it is negative it takes the lock at the record address plus 20 with `mutex_spinlock`
(0xc0000000002863b4), increments the word that +32 points to (an ordinary load, add and store), and for each element of the list (next pointer at element +8) it increments the word at element +16 atomically (`ll`/`sc`, the retry is the branch-likely
`beqzl` at 0xc000000000094360), calls `sigtopid(the element's first word, the element's word at offset 20, 18, 0, 0, 0)` (0xc0000000001bacc8) when that offset-20 word is nonzero, then takes the element's lock at +24 (`nested_spinlock`, 0xc000000000286420),
calls `sv_signal` on element +32 (0xc00000000018bf30) and then releases the lock (`nested_spinunlock`, 0xc000000000286444); afterwards it releases the first lock (`mutex_spinunlock`, 0xc000000000286400). All three paths end with
`ioc3_write_ireg(doubleword at +48, word at +16, 0)` (0xc000000000094fa4, operation 0, which ORs the mask into the enable shadow) and return its result. What the word at +44 means is inferred from its sign test; the code does not name it.
- `xbow_widget_lookup` (`6.5.30/unix.IP27`, address 0xc00000000006d5e8, 100 bytes, 32-byte frame): **established.**
Gives the vertex that the crossbar's switch information holds for one widget port. `a0` is the crossbar vertex and `a1` (`s0`) the widget number. It calls `xswitch_info_get(a0)` (0xc00000000006ca78) and keeps the result at sp+0,
then tests the number: below 8 (`slti` at 0xc00000000006d5fc) or not below 16 (0xc00000000006d608) calls `assfail` (0xc00000000003a5c4) with the expression text "(widgetnum >= HUB_WIDGET_ID_MIN) && (widgetnum <= HUB_WIDGET_ID_MAX)" and line 775 (the
`li a2,775` at 0xc00000000006d634; the file argument is not copied here). The assertion is not fatal in this build: the routine goes on in both cases to `xswitch_info_vhdl_get(info, widget & 0xff)` (0xc00000000006cae8), whose result is the return value (the only exit is
0xc00000000006d628). That routine returns the word at +12 + 4 x widget of the switch record, or -1 for a number of 16 or more; its numeric bounds 8 and 16 are the compare constants, the macro names come from the assertion text only. The callers are
`xbow_xmit_retry_error` (two sites) and `xbow_update_llp_status`. No branch-likely instruction.
- `hub_widget_flags_set` (`6.5.30/unix.IP27`, address 0xc00000000003c4bc, 76 bytes, 16-byte frame): **established.**
Applies a widget's flag bits to the hub's per-widget register through `hub_setup_prb` and always returns 1. `a0` is the NASID, `a1` the widget id and `a2` the flags. When flag bit 0 is set it calls `hub_setup_prb(a0, a1, 3, 1)` (0xc00000000003c338);
when bit 0 is clear and bit 1 is set it calls `hub_setup_prb(a0, a1, 3, 2)`; with neither bit set it calls nothing. Bit 1 is not looked at when bit 0 is set. Every path returns 1 (the `li v0,1` at 0xc00000000003c4fc), not the result of the call.
The callers are `hub_device_flags_set` and `kona_tport_reset_adaptor`. The meaning of the two modes is not read here. No branch-likely instruction.
- `hub_device_flags_set` (`6.5.30/unix.IP27`, address 0xc00000000003c508, 148 bytes, 48-byte frame): **established.**
Applies flag bits to one crosstalk widget's hub register under the node's lock and returns 1. `a0` is the widget vertex and `a1` (saved at sp+16) the flags. It calls `xwidget_info_get(a0)` (0xc00000000006c368), takes the widget id from
`xwidget_info_id_get` (0xc00000000006c44c; saved at sp+40) and the master from `xwidget_info_master_get` (0xc00000000006c480), clears sp+0 and reads the master's `_node` label with `hwgraph_info_get_LBL` (0xc000000000295310) into it; the
result of that lookup is not tested. It then takes the node record's lock at record +792 (`mutex_spinlock`, 0xc0000000002863b4; the level is saved at sp+32), calls `hub_widget_flags_set(halfword at record +10, widget id & 0xff, flags)`
(0xc00000000003c4bc; the halfword load is in the delay slot of the call) and releases the lock with `mutex_spinunlock` (0xc000000000286400). It returns the value from `hub_widget_flags_set`, which is 1. The only caller is `xbridge_attach`.
The record pointer is not tested for null. No branch-likely instruction.
- `ioc3_is_console` (`6.5.30/unix.IP27`, address 0xc000000000095cac, 88 bytes, 16-byte frame): **established.**
Says whether an IOC3 chip is the console chip. `a0` is the chip's PCI vertex. It returns 0 when the global doubleword at 0xc0000000014d2c88 (the console chip's record) is zero. Otherwise it calls
`hwgraph_traverse(a0, "ioc3", &sp+0)` (0xc00000000029579c); a nonzero result returns 0. Otherwise it calls `hwgraph_fastinfo_get` (0xc000000000296098) on the word at sp+0 and returns 1 when the result equals the global and 0 when it
differs (`xor` then `sltiu` 1, in the delay slot of the closing `b`). The callers are `eiattach`, `pckm_attach` and `ioc3_serial_attach`. No branch-likely instruction.
- `xbow_update_llp_status` (`6.5.30/unix.IP27`, address 0xc00000000006e9dc, 276 bytes, 64-byte frame): **established.**
Timer routine that adds each crossbar link's error counts into running totals and reports a link whose status word has unexpected bits. `a0` is the crossbar vertex (saved at sp+0). It takes the crossbar's record with
`hwgraph_fastinfo_get` (0xc000000000296098; kept in `s3`), the register base from the doubleword at record +16 (`s5`) and the totals array at record +320 (`s2`, 16 bytes per link). For each of eight links, with link index `s0` from 0 and widget number
`s1` from 8, it calls `xbow_widget_lookup(word at record +8, widget)` (0xc00000000006d5e8) and `xwidget_name_get` (0xc00000000006c980) on the result; the name is held in `v0`. In link `s0`'s 64-byte register block it reads the word at block +300 and the word at block
+316. When bit 31 of the +300 word is set it adds bits 23 to 16 of the +316 word to the doubleword at totals +8 (stored first, 0xc00000000006ea68) and bits 31 to 24 of the +316 word to the doubleword at totals +0 (stored in the delay slot of
the `bnez` at 0xc00000000006ea70). In that same bit-31 case, when the +300 word ANDed with 0x7fffffe7 (`lui` 0x7fff, `ori` 0xffe7) is also nonzero, it calls `cmn_err(4, "link %d[%s]: bad status 0x%x\n", link index, name, +300 word)` (0xc0000000001fcbd4) and goes
on; with bit 31 clear the link is skipped without any add or message. The format string is the one at 0xc0000000003c5648 + 1952, read through the gp slot at 0xc0000000014cdb38. After the eighth link, when the word at record +452 is nonzero it calls
`timeout(xbow_update_llp_status, vertex, 100)` (0xc0000000001b3498; the first argument is the gp slot at 0xc0000000014cdb68, the vertex is zero-extended to 32 bits in the delay slot), so the routine rearms itself every 100 ticks; with the word zero
it simply returns. The only exit is 0xc00000000006eac8, and no value is returned on purpose. `xbow_enable_llp_monitor` stores 1 in the word at +452 after calling `timeout` once with this routine, and `xbow_disable_llp_monitor` clears
the totals array and that word. Which register is the link status and which the counts is inferred from the use; the code does not name them. No branch-likely instruction.
- `xbow_update_perf_counters` (`6.5.30/unix.IP27`, address 0xc00000000006e6c8, 264 bytes, 64-byte frame): **established.**
Timer routine that folds the crossbar's two 20-bit performance counters into running 64-bit totals. `a0` (`s8`) is the crossbar vertex. It takes the crossbar's record from `hwgraph_fastinfo_get` (0xc000000000296098; `s4`) and looks at two counter slots, slot `s1` at record +32 + 16 x `s1`.
A slot whose byte at slot +5 is zero is skipped. For an active slot it takes the record's lock at +448 (`mutex_spinlock`, 0xc0000000002863b4) and reads the word that the doubleword at slot +8 points to. The new reading is that word masked with 0xfffff (`lui` 0xf, `ori` 0xffff);
the difference from the word at slot +0, masked with 0xfffff and zero-extended, is added to the doubleword at record + 64 + 32 x (bits 22 to 20 of the register word) + 8 x (byte at slot +7), and the new reading is stored into the word at slot +0 in the delay slot of the
`mutex_spinunlock` call (0xc000000000286400), before the lock is released. A counter that wrapped therefore adds the modulo-2^20 difference. After both slots it calls `timeout(xbow_update_perf_counters, vertex, 4)` (0xc0000000001b3498; the first argument is the gp slot at
0xc0000000014cdb60, the vertex is zero-extended in the delay slot), so it rearms every 4 ticks unconditionally. The only exit is 0xc00000000006e7c8 and no value is returned on purpose. `xbow_enable_perf_counter` fills the slots (see its entry). That the totals are indexed by port
and mode is inferred from the shifts. No branch-likely instruction.
- `hub_dmamap_alloc` (`6.5.30/unix.IP27`, address 0xc00000000003bad8, 128 bytes, 48-byte frame): **established.**
Makes a 16-byte record that tracks one DMA mapping through the hub and returns it. `a0` (saved at sp+8) is the widget vertex and `a3` (saved at sp+32) the flags; `a1` and `a2` are not read. It calls `xwidget_info_get(a0)` (0xc00000000006c368),
`xwidget_info_id_get` on the result (0xc00000000006c44c; saved at sp+24) and `xwidget_info_master_get` (0xc00000000006c480; saved at sp+16), then `kern_malloc(16)` (0xc00000000017e408). The result is not tested for null. It stores into the new
record, in this order: the vertex in the word at +0, 1 in the word at +12, the master in the word at +8 and the widget id in the byte at +4; when bit 0 of the flags is set it then stores 5 in the word at +12. The record is returned (`v0` is the
`kern_malloc` result at both exits, 0xc00000000003bb40 falling through to 0xc00000000003bb50). Bytes +5 to +7 are not written. The word at +12 is used by `hub_dmamap_addr`, `hub_dmamap_list` and `hub_dmamap_done` as a set of bits (2 for in use, 4 for
reusable); that bit 0 is "allocated" is inferred. The routine sits in the hub's table of crosstalk operations (the doubleword at 0xc000000001416780) and its one direct caller is `xtalk_dmamap_alloc`. No branch-likely instruction.
- `hub_dmamap_addr` (`6.5.30/unix.IP27`, address 0xc00000000003bb80, 92 bytes, 16-byte frame): **established.**
Marks a hub DMA mapping as in use and hands back its address argument unchanged. `a0` is the record `hub_dmamap_alloc` made and `a1` the address (kept in `v0` from the start). It reads the word at record +12. With bit 1 clear it stores the word ORed with 2
(the `sw` at 0xc00000000003bbb8, in the delay slot of the `b` at 0xc00000000003bbb4) and returns `a1`. With bit 1 set and bit 2 set it returns `a1` with nothing changed. With bit 1 set and bit 2 clear it calls
`cmn_err(4, "%v: hub_dmamap_addr re-uses dmamap.\n", the word at record +0)` (0xc0000000001fcbd4; the text is 16 bytes into the string area that starts with "_node") and then returns `a1` (reloaded from sp+0). No path changes `a1`, the translation is the identity. The routine is
the doubleword at 0xc000000001416790 in the hub's table of crosstalk operations; its one direct caller is `xtalk_dmamap_addr`. No branch-likely instruction.
- `hub_dmamap_list` (`6.5.30/unix.IP27`, address 0xc00000000003bbdc, 92 bytes, 16-byte frame): **established.**
Same body as `hub_dmamap_addr`, for a list of regions instead of one address: `a0` is the record and `a1` the value it hands back. With record +12 bit 1 clear it sets that bit (the `sw` at 0xc00000000003bc14, in the delay slot of the `b` at 0xc00000000003bc10) and returns `a1`;
with bit 1 set and bit 2 set it returns `a1`; with bit 1 set and bit 2 clear it calls `cmn_err(4, "%v: hub_dmamap_list re-uses dmamap\n", the word at record +0)` (0xc0000000001fcbd4; the text is 56 bytes into the same string area, and unlike the other message it has no full stop)
and returns `a1`. It translates nothing. It is the doubleword at 0xc000000001416798 in the hub's table of crosstalk operations; its one direct caller is `xtalk_dmamap_list`. No branch-likely instruction.
- `hub_dmamap_done` (`6.5.30/unix.IP27`, address 0xc00000000003bc38, 80 bytes, 16-byte frame): **established.**
Releases the in-use mark of a hub DMA mapping. `a0` is the record. With the word at record +12 having bit 1 set, it clears that bit (an AND with -3) and returns (the store at 0xc00000000003bc58); with bit 1 clear and bit 2 set it returns without a change; with both clear it calls
`cmn_err(4, "%v: hub_dmamap_done already done with dmamap\n", the word at record +0)` (0xc0000000001fcbd4; the text is 96 bytes into the string area) and returns. No return value is set on purpose. It is the doubleword at 0xc0000000014167a0 in the hub's table of crosstalk operations; its one direct
caller is `xtalk_dmamap_done`. No branch-likely instruction.

## SCSI host, PCI bridge and hub control calls

- `scsiha_ioctl` (`6.5.30/unix.IP27`, address 0xc0000000000bc638, 556 bytes, 112-byte frame): **established.** The control-call entry of the SCSI host adapter. `a0`
(`s1`) is the device vertex, `a1` (`s0`) the command, `a2` a user pointer (kept at sp+64) and `a4` the caller's credentials (kept at sp+56). Bits 18 to 26 of `a0`
are tested first: any nonzero value returns 6 and nothing else runs. Otherwise `scsi_ctlr_info_get` (0xc0000000000c2a78) is called on `a0` and its result, the
controller record, is kept at sp+48 without a null test. The commands are compared in this order:
  - 29446 (0x7306) jumps to the path code. It takes the connection point (`hwgraph_connectpt_get`) and the controller number (`device_controller_num_get`); a
    negative number releases the handles and returns 0. Otherwise it calls `hwgraph_path_to_dev` on the doubleword at gp-15600; on -1 it calls `hwgraph_path_add`, and in
    both cases it formats a path with `sprintf` and calls `hwgraph_traverse`. A traverse result of 2 also adds an edge with `hwgraph_edge_add`. Then it stores the
    controller number at record +76 and calls the record's routine pointer (the doubleword at +40) with the word at record +0, the command and the controller
    number. The return value of that call is discarded and the routine returns 0. No user copy happens on this path.
  - 29473 (0x7321) is reached only after the 16-byte copy and the ABI step below. It needs the capability `1 << 37` (`cap_able_cred`, 0xc000000000287350, on the
    credentials in `a4`). Without it the routine returns 1. With it, the command is passed to the record's routine pointer as for the general case below.
  - Commands below 11 (a signed compare with `slti`) go straight to the record's routine pointer. They copy nothing in: the third argument is a 16-byte buffer from
    `kmem_zalloc` (0xc00000000017e344) that is not copied into.
  - Commands 11 and up, other than 0x7306, copy 16 bytes from the user pointer into that buffer with `copyin` (0xc00000000001f990). A failed copy returns 14.
    After a good copy the routine calls `get_current_abi` (0xc0000000003c3cac). When bit 2 (value 4) of its result is clear, the doubleword at buffer +8 is
    replaced by its upper 32 bits, zero-extended. The command 0x7321 check and the capability check then run as above, and everything else goes to the record's
    routine pointer.
  - The routine pointer call is `(word at record +0, command, buffer)`, and its result is the routine's result. Every route after the allocation, which is every
    command except 0x7306, frees the buffer with `kern_free` (0xc00000000017e590) before it returns. The early return of 6 and the 0x7306 path allocate nothing.
  Error numbers: 6 (bits 18 to 26 of `a0` set), 1 (capability missing for 0x7321), 14 (failed copy).
- `pcibr_ioctl` (`6.5.30/unix.IP27`, address 0xc0000000000715b8, 368 bytes, 80-byte frame): **established.** The control-call entry of the PCI bridge. `a0` is the
vertex, `a1` (`s0`) the command and `a2` a user pointer kept at sp+40. Before any command is compared, the routine takes the connection point of `a0`
(`hwgraph_connectpt_get`), its fast-info pointer (`hwgraph_fastinfo_get`, kept at sp+32) and releases the connection point. Commands:
  - 26369 (0x6701) needs `cap_able` on `1 << 37` (0xc00000000028741c; failure returns 1). It copies 16 bytes from the user pointer to sp+16 (failure returns 14).
    The word at sp+16 goes to `pcibr_device_slot_get` (0xc000000000071530). A result of 255 returns 5. Otherwise, if the doubleword at sp+24 is zero the routine
    returns 0. If it is nonzero, it calls `pcibr_priority_bits_set` (0xc000000000074a2c) with the fast-info pointer, the slot masked to a byte and `a2` = 1, leaving
    `a3` as it arrived, and returns 0.
  - 26370 (0x6702) has the same capability and copy, into sp+0. The word at sp+0 goes to `pcibr_device_slot_get`; 255 returns 5. If the doubleword at sp+8 is
    zero the routine returns 0; otherwise it calls `pcibr_priority_bits_set` with the fast-info pointer, the slot masked to a byte, `a2` = 0 and `a3` = 255, and
    returns 0.
  - 28673 (0x7001), 28674 (0x7002) and 28675 (0x7003) make no further call and return 1.
  - Any other command returns 0.
  Error numbers: 1 (capability missing), 14 (failed copy), 5 (slot 255).
- `hubspc_ioctl` (`6.5.30/unix.IP27`, address 0xc00000000006bd7c, 124 bytes, 64-byte frame): **established.** This routine compares no command number. It saves
the six argument registers, calls `device_info_get` (0xc0000000001734d0) on the vertex in `a0` and branches on its result as a 32-bit value. Result 0 restores the six
registers and calls `mem_refcnt_ioctl` (0xc00000000027a210) with them, then returns that call's result. Result 1 returns 0. Any other result returns 19 and calls
nothing.
