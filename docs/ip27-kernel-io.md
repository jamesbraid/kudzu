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
the bridge DMA write request limits (source file name `huberror.c`, line 1804). Every slot that was reported goes to `bte_crb_error_handler(opaque, flag, slot, record)`
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
