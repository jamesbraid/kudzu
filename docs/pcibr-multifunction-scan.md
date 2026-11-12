# PCI slot scan: multi-function cards, vertex names and PCI-to-PCI bridge functions

How the IRIX 6.5.22 and 6.5.30 Bridge-family provider (`pcibr`) and generic PCI layer (`pciio`) treat a card whose function 0 is a PCI-to-PCI bridge (class 0x0604) and whose function 1 is an ordinary device, on IP30 (Bridge or XBridge), IP27 (Bridge or XBridge) and IP35 (PIC). The case that prompted it is a card with an Intel i960 Rx: function 0 a bridge, function 1 the address translation unit with a base address register 0 and vendor and device 0x1263 and 0x05DC. The page says what the kernel does for any such card; it does not depend on that card's contents.

Evidence: the kernel objects `IP30boot`, `IP27boot` and `IP35boot` `pcibr.o`, `pciio.o`, `pciio_ppb.o` and `cdl.o` of 6.5.22 and 6.5.30 under `root/`, and the linked `unix.IP30`, `unix.IP27` and `unix.IP35` of both releases for function addresses. Addresses are `.text` offsets in the 6.5.30 IP30 object unless a platform is named; the table at the end gives each function's start for every release and platform. Disassembly was read with the MIPS binutils in the retools image; no source tree was consulted. The companion page [pciio-driver-interface.md](pciio-driver-interface.md) covers the driver call interface and the `.id` registry; [ip35-pci-bridge.md](ip35-pci-bridge.md) reads the IP35 slot probe from the linked kernel. The hardware side (configuration address layout by function) is in the sibling chips repository, `docs/bridge-pci-driver-windows.md`.

Grades: **E** established (read in the instructions at the address given), **I** inferred (reasoned from what was read, reasoning stated), **U** unknown.

Independent check of the `.id` and deferred-attach claims below: re-read from `cdl.o` (IP30 6.5.30 and the compared builds, which are instruction-identical after normalising gp displacements) and the linked kernels. Read `cdl_add_connpt` 0xc0c to 0xc8c, `cdl_add_connpt_reg_locked` 0xd98 to 0xdc8, 0xe34 to 0xe38, 0xf20 to 0xfe8 and 0xfec to 0x102c, and `cdl_call_attach` 0x368 to 0x398 and 0x480 to 0x494; the call sites in `unix.IP30` 6.5.30 (`xbridge_attach` calls at 0xa800000020042898, `pciio_device_attach` at 0xa8000000200428c0) agree. E for the flags-0 path: the zero-count branch rejoins at 0xdcc and the `.id` entry is made. One scope limit found: that holds when the attach flags argument is 0, which is every IP30 and IP27 caller and the IP35 boot-time callers. With a nonzero flags argument (IP35 `pcibr_slot_startup` calls `pcibr_slot_attach` with 0x200, passed on by `pcibr_slot_call_device_attach`) `cdl_add_connpt` first calls `cdl_chk_driver` and returns its value without registering when that is nonzero (0xc30 to 0xc40), and `cdl_add_connpt_reg_locked` returns at 0xef0 with a real vendor and device when there is no driver on the entry or the last attach returned nonzero (0xd98 to 0xdb0, 0x110c to 0x1124), so neither the `.id` nor the `unknown` entry is made on that path. What `cdl_chk_driver` returns for which case was not worked through (U). E for the branch structure; not run.

## What is the same everywhere

Compared as instruction streams with only the global-pointer displacements normalised:

- 6.5.22 and 6.5.30 of the same platform have identical `pcibr_slot_info_init`, `pcibr_device_info_new`, `pcibr_slot_call_device_attach`, `pcibr_func_config_addr`, `pcibr_probe_slot`, `pciio_slot_func_to_name`, `pciio_device_info_register`, `pciio_device_attach`, `pciio_device_info_new`, `cdl_call_attach`, `cdl_add_connpt` and `cdl_add_connpt_reg_locked`. E.
- IP30 and IP27 6.5.30 have identical `pcibr_slot_info_init`, `pcibr_slot_call_device_attach`, `pcibr_device_info_new`, `pcireg_type0_cfg_addr` (up to six differing displacement lines) and `pciio_device_info_register`. E.
- IP35 has the same callee sequence in `pcibr_slot_info_init` (39 calls, same order) and the same constants for every step quoted below (0x80 test, bound 8, mask 0x7f, 6 or 2 registers). The instruction differences are register allocation and the slot-record field offsets (+264 and +276 where IP30 has +256 and +268). E for the sequence and constants, I that nothing else differs.
- The PPB module differs between releases in two routines, `pciio_ppb_attach` and `ppb_function_init`; see the last section. `ppb_scan`, `do_ppb_scan`, `ppb_device_init`, `ppb_addr_space_assign`, `ppb_bus_alloc`, `ppb_attach_function` and `pciio_ppb_init` are identical across all six objects. E.

## 1. Does the slot scan probe functions above 0

**Yes, when function 0's header type has bit 7 set, and up to function 7.** All in `pcibr_slot_info_init` (0xb768):

1. Function 0 is probed through `pcibr_slot_config_addr` (0xb81c) and `pcibr_probe_slot` (0xb830). A failed probe returns 19 (0xb848). The identity word is kept, and a vendor halfword of 0xffff returns 19 (0xb8a4 to 0xc034). E.
2. The header type byte (config offset 0xe) is read with `do_pcibr_config_get(addr, 14, 1)` (0xb8b0 to 0xb8b4) and tested with `andi 0x80` (0xb8c0). Bit 7 clear: one function, the count word is set to 1 and the function selector to 0xff (0xc054 to 0xc068). E.
3. Bit 7 set: functions 1 to 7 are probed in a loop (0xb90c to 0xb96c). The loop counter is compared with 8 (`slti 8`, 0xb900), so the bound is 7 and nothing above it is tried. Each pass calls `pcibr_func_config_addr(soft, bus 0, slot, function, 0)` (0xb92c) then `pcibr_probe_slot` (0xb93c). A failed probe sets bit `function` of an absent mask (0xb8f4 to 0xb8f8). A good probe loads the identity word directly from the returned address (0xb94c); a vendor halfword of 0xffff is also marked absent (0xb950 to 0xb958, 0xc06c); anything else records the count as `function + 1` (0xb960; the register holds function + 1 from the loop entry at 0xb8ec and the increment at 0xb908). **The count is therefore the highest present function plus one, and a gap is allowed**: absent functions inside the range are skipped later. E.
4. The function selector word is cleared to 0 when at least one function above 0 answered (0xb968) and stays 0xff when none did. A card with bit 7 set and only function 0 present is therefore treated as a single-function card for naming. E.
5. `kmem_zalloc(count * 8)` gives the per-slot array of function records (0xb990, stored in the slot record at +280 with the count at +272, 0xb9a4 and 0xb9b0). E.
6. The second loop (0xba00 to 0xb9f8) walks functions 0 to count-1. A function whose absent bit is set is skipped (0xba14 to 0xba24). For function above 0 the identity and header type are read again and the selector word is set to the function number (0xbfd4 to 0xc02c, store at 0xc024). E.
7. The header type is masked with 0x7f (0xba2c). Type 0 sizes 6 base registers (0xbed0); type 1 sizes 2 (0xba48); any other type goes to `cmn_err` level 4 with the "strange header type" message (0xc238 to 0xc250) and is treated as 2 registers. So **a PCI-to-PCI bridge function (header type 1, usually read as 0x81) has its two base registers sized, given bus address windows and written back like any other function**. The ROM base register is not part of that loop. E.

What stops the scan: the bound of 8 functions; an unanswered or all-ones function is skipped, not an error; header type bit 7 clear on function 0. On a bus in PCI-X mode (halfword at soft +116, bit 0) a function without the PCI-X capability gets a warning and an empty record (0xc25c to 0xc290) and is skipped. That code is present in the Bridge and XBridge objects as well; whether the bit is ever set for a Bridge or XBridge was not read (U). Other per-function effects: the latency timer is written when it reads 0, from the minimum-grant byte (0xbaa0 onward), and the cache-line byte only in PCI-X mode. E for the paths, the arithmetic not re-derived.

IP35 differences: none in the scan logic. The only slot difference is that the info record's slot number is the PIC device number, one higher than the loop's slot index (see below). E (IP35 `pcibr.o` 0xc704 holds the 0x80 test, 0xc744 the bound).

Each present function's configuration is reached at `base + 0x20000 + device * 0x1000 + function * 0x100 + register` for Bridge and XBridge (`pcireg_type0_cfg_addr`, 0x173cc, block at 0x174bc to 0x174fc). Hardware address layout is in the chips repository page named above.

## 2. The connection vertex of function 1

**One hwgraph vertex per function, a sibling under the bridge's `pci` vertex, named `<slot>` plus a letter.** The chain:

- `xbridge_attach` creates the bridge's `pci` vertex with `hwgraph_path_add(widget vertex, "pci")` (`xbridge.o` 0xe0, the string is at rodata 408). E.
- `pcibr_slot_info_init` calls `pcibr_device_info_new(soft, slot, selector, vendor, device)` (0xbe0c) then `pciio_device_info_register(pci vertex, record)` (0xbe5c). E.
- `pcibr_device_info_new` (0x904) calls `pciio_device_info_new` (0x970) with the slot number, adding 1 when the bridge type halfword (soft +114) is 2, that is a PIC (0xa14 to 0xa18, 0x954 to 0x958), and with the function selector. It stores the function in the record byte at +540 (0x990) and the four interrupt-pin entries `(slot + k) & 7` for k from 0 to 3 at +512 (0x99c to 0x9b8) from the slot, not the function: **all functions of a slot share one pin map**. E. The record goes into the slot's array at the function index, or index 0 when the selector is 0xff (0x9bc to 0x9ec). E.
- `pciio_device_info_register` (0x1070) calls `pciio_slot_func_to_name` (0x1090) and then `hwgraph_path_add(pci vertex, name)` (0x10a0), stores the vertex in the record (+8 and +12), sets the info (0x10d8) and the master (0x10e4). E.
- `pciio_slot_func_to_name` (0x580): slot 0xff gives the fixed string at rodata 0 ("direct", 0x5d4); selector 0xff gives `"%d"` of the slot (0x5bc); otherwise `"%d%c"` with the character `'a' + function` (0x59c to 0x5a8, 0x61 is 97). E.

So on IP30 and IP27 the path shape is `<xtalk widget vertex>/pci/<slot>a` for function 0 and `<xtalk widget vertex>/pci/<slot>b` for function 1 of a multi-function card, and plain `<xtalk widget vertex>/pci/<slot>` for a single-function card, a card with bit 7 set and no second function included. The widget vertex is the same one single-function cards use, for example the existing note's `xtalk/15/pci/4` form. On IP35 the number is the info record's slot, which is the PIC device number: slot index plus 1. Which widget number and prefix a given cage has was not read. The `<func>` directory shape `.../pci/<slot>/<func>` does not exist: functions are siblings, not children. E.

One more registration in the same routine: for a record whose bus number is 0 and slot is not 0xff, `usrpci_device_register(vertex, master)` is called (0x10f8 to 0x1118). It is in the IP30 and IP27 objects and not in the IP35 object (IP35 `pciio.o` 0x1018 has no call). Devices behind a PCI-to-PCI bridge have a nonzero bus number and never get it. E.

## 3. Does a driver registered for 1263:05DC attach to function 1

**Yes, and nothing in the path looks at the function number.** After the per-slot setup `xbridge_attach` calls `pcibr_slot_call_device_attach(soft, slot, 0)` for each slot (`xbridge.o` 0x7e8). That routine (0xcfdc):

- takes the slot's function array (+280) and count (+272) and walks it in index order, 0 up to the count (0xd0d4 to 0xd0f0). A null entry or a record whose vendor field (+20) is -1 is skipped (0xd0f8 to 0xd104). E.
- skips a function whose vertex has the administrative attribute "DISABLE" (`device_admin_info_get`, 0xd118; string at rodata 12056) with a level 4 message "<name> disabled" (0xd1e4 to 0xd1fc). E for the path and string; how the attribute gets set was not read.
- calls `pciio_device_attach(function vertex, 0)` (0xd140), which calls `cdl_add_connpt(pciio_registry, vendor +20, device +24, vertex, flags)` (`pciio.o` 0x1250). E.

`cdl_add_connpt_reg_locked` (`cdl.o` 0xca8) finds or creates the registry entry for the exact vendor and device pair (`cdl_ent`, 0xcdc; it allocates one when none exists), calls every driver on that entry (`cdl_call_attach`, 0xd7c) and counts those that returned 0 (0xd84 to 0xd94). The count only decides the first step. With a count of zero and both numbers real (not -1) it also creates `/hw/unknown/pci/vendor/0x<vendor>/device/0x<device>/instance` with an edge to the connection vertex (0xfec to 0x1028, 0x1134 to 0x116c). Both outcomes then continue at 0xdcc: the connection vertex is added to the entry's list (0xdd4 to 0xe2c), and **when the entry has no `.id` vertex yet (word +20 is -1, 0xe34 to 0xe38) and neither number is -1, the path `/hw/.id/pci/<VVVVDDDD>` is created (0xf20 to 0xfe8) and an edge named by the decimal vertex handle is added to it (0xe48 to 0xe80)**. Nothing between the count test and 0xe34 reads the count again. So `/hw/.id/pci/126305DC` is made for function 1 as soon as function 1 is attached to the registry, with or without a driver for 1263:05DC, and a card with no driver additionally gets the `unknown` path. The three wildcard registrations (vendor, device or both -1) are tried afterwards by recursive calls (0xeb4 to 0xee4), with their results discarded, and never make a name themselves. The routine is identical in all six `cdl.o` objects compared. E for the control flow; not run.

**This corrects [pciio-driver-interface.md](pciio-driver-interface.md#the-hwidpcivendordevice-entries) as first written, which said the `.id` entry needs a successful driver attach.** That reading took the count test at 0xdc4 as a condition on the name; the zero-count branch at 0xfec rejoins the main path at 0xdcc, so the condition does not exist. The same page's "driver registered afterwards" paragraph is unchanged in effect: the entry is made at attach time either way.

What a driver registered for the pair does change: its attach runs (once per connection vertex, in `cdl_call_attach` 0x2bc, or from `cdl_add_driver` when it registers after the scan, `cdl.o` 0x868), and a pair with at least one successful attach gets no `unknown` entry.

A reading that settles the old open question about deferred attach: in `cdl_call_attach` the thread path (`sthread_create` at 0x480, `async_attach_signal_start` at 0x488) returns 0 (0x494, `move s1,zero` in the branch delay slot) before the driver's attach has run, so a deferred attach is counted as a success and suppresses the `unknown` entry even if the attach later fails. E for the return value. The condition for taking that path is `io_init_async` set (0x368 to 0x374), an async info record for the vertex (0x37c) and bit 0x100 of the driver's flag word (0x384 to 0x38c), with the attach flag bit 0x200 clear (0x378, 0x398); `pcibr_slot_call_device_attach` passes flags 0 on the boot-time paths (the IP35 slot-startup path passes 0x200, which stops this threading). What bit 0x100 of the driver's flags means was not read, so whether a particular driver takes this path is U.

A driver registered after the scan is attached to the existing vertices by `cdl_add_driver` (call to `cdl_call_attach` at 0x868). The `.id` entry already exists by then.

A new reading, answering the open question about deferred attach: in `cdl_call_attach` (0x2bc) the thread path (`sthread_create` at 0x480, `async_attach_signal_start` at 0x488) returns 0 (0x494, `move s1,zero` in the branch delay slot) before the driver's attach has run, so a deferred attach is counted as a success and the `.id` entry is created even if the attach later fails. E for the return value. The condition for taking that path is `io_init_async` set (0x368 to 0x374), an async info record for the vertex (0x37c) and bit 0x100 of the driver's flag word (0x384 to 0x38c), with the attach flag bit 0x200 clear (0x378, 0x398); `pcibr_slot_call_device_attach` passes flags 0 on the boot-time paths (the IP35 slot-startup path passes 0x200, which stops this threading). What bit 0x100 of the driver's flags means was not read, so whether a particular driver takes this path is U.

Does the vendor and device pair have to be exact? Yes: 0x1263 and 0x05DC are matched as two 32-bit halves of the identity word as read at function 1's config offset 0 (vendor from the low halfword, device from the high one, `pcibr_slot_info_init` 0xb85c to 0xb86c). E.

## 4. What happens to a function 0 of class 0x0604

**`pcibr` itself ignores the class.** `pcibr_slot_info_init`, `pcibr_slot_addr_space_init`, `pcibr_slot_device_init` and `pcibr_slot_call_device_attach` contain no class-code read, and `pcibr.o` references no `ppb` symbol. The bridge function is probed, sized (two base registers), given windows, given a hwgraph vertex (`<slot>a`) and offered to the registry like any other function, and it does not block function 1: the attach loop continues to the next array entry whatever the previous attach returned (0xd0d4 to 0xd0e0; the result only sets the slot status at 0xd16c to 0xd1b4). E.

The class is looked at by a separate generic module, `pciio_ppb.o`. Its init routine `pciio_ppb_init` (0x1acc) reserves bus numbers 0 to 15 and 255 in a 256-bit allocation map (loop at 0x1af4 to 0x1b08, 0x1b10) and registers a driver with vendor -1 and device -1 (`pciio_driver_register`, 0x1b2c). It is in the linked kernels' I/O init table next to `pciio_init` in all three platforms (checked in `unix.IP30` 6.5.30 at 0xa8000000204268e8, `unix.IP35` 6.5.30 at 0xc00000000153fea8, and the 6.5.22 and IP27 equivalents); `master.d/pciio_ppb` lists it with prefix `pciio_ppb_`. E. Being a wildcard driver, it is called for every function through the recursive wildcard calls above.

`pciio_ppb_attach` (0x1b40):

- returns 0 at once when the record's slot byte is 0xff (0x1b6c) or when the class is not a bridge: it reads config offset 0x0b (base class) and 0x0a (sub-class) (0x1b78, 0x1b8c) and continues only for base class 6 with sub-class 0 (host bridge) or 4 (PCI-to-PCI) (0x1ba8 to 0x1bb8). Return 0 counts as success in the registry, so **the wildcard driver "succeeds" for every non-bridge function, function 1 included, without doing anything**. E.
- for a bridge calls `ppb_scan` (0x1bdc). A non-zero result is returned and the bridge is left unconfigured.
- then `ppb_addr_space_assign` (0x1bf4), optionally `ppb_function_print` when `showconfig` is set (0x1c0c) and `ppb_attach_function` (0x1c14), and returns 0. E.

`do_ppb_scan` (0x2d8), run under a semaphore by `ppb_scan` (0x0 to 0x48), is what scans behind the bridge:

- allocates a bus number with `ppb_bus_alloc` (0x304; the first free bit after the reserved 0 to 15, so 16 for the first bridge; the map is filled from the most significant bit down, so reservations 0 to 15 are the top half of the first word; read at 0x1968 to 0x1a20, inferred for the first result), allocates a 128-byte bridge record and writes the primary bus number to config offset 0x18 (0x334) and the new secondary to 0x19 (0x350). The subordinate register 0x1a is written with the new bus number for this bridge and for each enclosing bridge in the chain (0x414 to 0x43c). E for the writes, I that the chain is the parent bridge list.
- creates a vertex `ppb_<secondary bus>` under the bridge function's vertex (sprintf at 0x36c, `hwgraph_path_add` at 0x37c), so the path is `<xtalk widget>/pci/<slot>a/ppb_<n>`. E.
- copies the 512-byte info record into a new one for that vertex with bus, slot and function set to 0xff (0x3a0 to 0x3f4) and flag +504 set to 1, which makes configuration accesses through it be type 1 cycles carrying the bus, device and function from the offset word (`pcibr_config_addr` 0xb418; `pcibr_func_config_addr` 0xb238 calls `pcireg_type1_cntr_set` then `pcireg_type1_cfg_addr` when the bus argument is above 0). E.
- then reads the vendor halfword of devices 0 to 31 on the new bus with `pciio_config_get_safe` (offset word `bus << 16 | device << 11`, loop 0x45c to 0x490, 0x468). A halfword of 0xffff is "nothing there"; the safe read returns all ones when the access faults (`pcibr_config_get_safe` 0xb544 uses `pcibr_probe_slot`). Any other value goes to `ppb_device_init` (0x47c). E.

`ppb_device_init` (0x530) reads header type bit 7 (0x5b8, 0x5c8) and walks 1 or 8 functions (0x5cc to 0x5dc), calling `pciio_device_info_new` with the device number as slot and the function, or 0xff for a single-function device (0x63c), registering each under the `ppb_<n>` vertex (0x648) and sizing its registers in `ppb_function_init` (0x6c0), which has the same type 0 (6 registers) and type 1 (2) split. The path of a device behind the bridge is therefore `<xtalk widget>/pci/<slot>a/ppb_<n>/<device>[a-h]`. E.

`ppb_scan` also enforces a bus-master rule. `ppb_function_init` counts the functions behind the bridge whose command register has bus-master enable (bit 2) and whose class is not a bridge (0xa8c to 0xabc) into halfwords at +106 and +104 of the bridge record, and a bridge child adds its own +106 count to its parent's (0xbbc to 0xbd4). The meaning of the two halfwords is inferred from the arithmetic. When two or more were counted, `ppb_scan` requires the host slot to allow multiple masters (`pciio_businfo_multi_master_get`, 0x140) or the tunable `pciio_multimaster_override` (0x14c); otherwise it logs "Host PCI slot %v does not support having multiple dma masters downstream" and returns -1, with the message that no devices behind the slot are initialised (0x278 to 0x2b4). A bridge with no device behind it, or one master, is unaffected. Which value `pciio_businfo_multi_master_get` returns for a Bridge, XBridge or PIC slot was not read (U).

Window programming: `ppb_addr_space_assign` (0xcf8) calls `function_addr_space_assign` on every child and then writes the bridge's I/O, memory and prefetchable memory window registers (reads of 0x1c and 0x24 at 0xda8 and 0xe98; writes of 0x30, 0x32, 0x28, 0x2c and others at 0xddc to 0xee8). It prints "*** ppb IO base/limit conflict" style messages and takes no other action when windows conflict (0xf18, 0xf30). What it writes when nothing sits behind the bridge was not traced (U).

No panic or error path was found for a bridge-class function that has no driver of its own or no device behind it. A bridge function without an exact driver appears under `/hw/unknown/pci/...` (companion page) while the wildcard driver still configures its bus numbers; both are normal outcomes. E for the absence of a failure branch in the paths read; the code under `ppb_function_init` beyond the lines quoted was read for its calls and counters only (I).

## Differences between IP30, IP27 and IP35, and between releases

| Item | IP30, IP27 | IP35 (PIC) | Grade |
|---|---|---|---|
| Slot number in the record and vertex name | slot index | slot index + 1 (`pcibr_device_info_new` IP35 object 0xa20 tests soft +114 against 2, 0xaec adds 1; same shape at 0x954 and 0xa18 in the IP30 object) | E |
| Type 0 configuration address of a function | `base + 0x20000 + slot*0x1000 + function*0x100 + reg` | same form with `(slot+1)`, which the code asserts is 1 to 4; the address mask constant differs | E (IP30 object 0x17514 to 0x1757c holds both branches; IP35 object compared, the mask is the only difference) |
| `usrpci_device_register` for bus 0 | called | not called | E |
| PCI-X capability check | present in code | present, effective on a PIC bus in PCI-X mode | E for the code, U for Bridge reachability |
| Scan, naming, attach loop | identical | identical | E |

6.5.22 against 6.5.30 for the PPB module:

- 6.5.22 `pciio_ppb_attach` (IP30, IP27 and IP35 all at object 0x1aac) does the scan and window assignment synchronously and then names a thread `ppb_attach%d` and starts it with `sthread_create` (0x1b8c to 0x1bbc); the thread runs `ppb_attach_function` (`ppb_attach_thread` 0x1bcc), which attaches the devices behind the bridge. The 6.5.30 version (object 0x1b40) calls `ppb_attach_function` directly (0x1c14). So in 6.5.22 the devices behind a bridge are attached after `pciio_ppb_attach` has already returned, in 6.5.30 before. E.
- `ppb_function_init` differs between releases in its frame layout; the register and bus-master logic read above was read in 6.5.30 only. U for 6.5.22.
- Everything else in sections 1 to 3 is byte-for-byte the same sequence in both.

## Function addresses

Object offset (in the named `.o` of that release and platform) and linked-kernel address. The `ppb_` routines are local symbols, absent from the kernel symbol tables, so only object offsets are given.

| Function | 6.5.22 IP30 | 6.5.22 IP27 | 6.5.22 IP35 | 6.5.30 IP30 | 6.5.30 IP27 | 6.5.30 IP35 |
|---|---|---|---|---|---|---|
| `pcibr_slot_info_init` | 0xb740, 0xa800000020043f88 | 0xb9b4, 0xc00000000007e138 | 0xc580, 0xc0000000000abe34 | 0xb768, 0xa80000002004e2a8 | 0xb9dc, 0xc00000000007c81c | 0xc5a8, 0xc0000000000a9c68 |
| `pcibr_slot_call_device_attach` | 0xcfb4, 0xa8000000200457fc | 0xd228, 0xc00000000007f9ac | 0xddf8, 0xc0000000000ad6ac | 0xcfdc, 0xa80000002004fb1c | 0xd250, 0xc00000000007e090 | 0xde20, 0xc0000000000ab4e0 |
| `pcibr_device_info_new` | 0x904, 0xa80000002003914c | 0x904, 0xc000000000073088 | 0x9d0, 0xc0000000000a0284 | 0x904, 0xa800000020043444 | 0x904, 0xc000000000071744 | 0x9d0, 0xc00000000009e090 |
| `pcibr_func_config_addr` | 0xb210, 0xa800000020043a58 | 0xb484, 0xc00000000007dc08 | 0xb748, 0xc0000000000aaffc | 0xb238, 0xa80000002004dd78 | 0xb4ac, 0xc00000000007c2ec | 0xb770, 0xc0000000000a8e30 |
| `pcireg_type0_cfg_addr` | 0x173a4, 0xa80000002004fbec | 0x17618, 0xc000000000089d9c | 0x18774, 0xc0000000000b8028 | 0x173cc, 0xa800000020059f0c | 0x17640, 0xc000000000088480 | 0x1879c, 0xc0000000000b5e5c |
| `pciio_slot_func_to_name` | 0x580, 0xa8000000200553d0 | 0x580, 0xc000000000094784 | 0x580, 0xc0000000000c3180 | 0x580, 0xa80000002005f724 | 0x580, 0xc000000000092e9c | 0x580, 0xc0000000000c0b44 |
| `pciio_device_info_register` | 0x1070, 0xa800000020055ec0 | 0x1070, 0xc000000000095274 | 0x1018, 0xc0000000000c3c18 | 0x1070, 0xa800000020060214 | 0x1070, 0xc00000000009398c | 0x1018, 0xc0000000000c15dc |
| `pciio_device_attach` | 0x121c, 0xa80000002005606c | 0x121c, 0xc000000000095420 | 0x1190, 0xc0000000000c3d90 | 0x121c, 0xa8000000200603c0 | 0x121c, 0xc000000000093b38 | 0x1190, 0xc0000000000c1754 |
| `pciio_ppb_attach` | 0x1aac, 0xa800000020054b88 | 0x1aac, 0xc000000000093f3c | 0x1aac, 0xc0000000000c2938 | 0x1b40, 0xa80000002005ef3c | 0x1b40, 0xc0000000000926b4 | 0x1b40, 0xc0000000000c035c |
| `pciio_ppb_init` | 0x1a38, 0xa800000020054b14 | 0x1a38, 0xc000000000093ec8 | 0x1a38, 0xc0000000000c28c4 | 0x1acc, 0xa80000002005eec8 | 0x1acc, 0xc000000000092640 | 0x1acc, 0xc0000000000c02e8 |
| `cdl_add_connpt_reg_locked` | 0xca8, 0xa80000002008e100 | 0xca8, 0xc0000000000d1bc4 | 0xca8, 0xc000000000114e2c | 0xca8, 0xa800000020097674 | 0xca8, 0xc0000000000c614c | 0xca8, 0xc000000000112094 |

Object offsets of the local `pciio_ppb.o` routines, 6.5.30 (6.5.22 in brackets where different): `ppb_scan` 0x0, `do_ppb_scan` 0x2d8, `ppb_device_init` 0x530, `ppb_function_init` 0x6c0, `ppb_addr_space_assign` 0xcf8 (0xcb4), `ppb_bus_alloc` 0x1968 (0x18d4), `ppb_attach_function` 0x1c24 (0x1bf0). The same in IP30, IP27 and IP35.

## Open points

- What `pciio_businfo_multi_master_get` returns per bridge type, and so whether a bridge with two bus masters behind it is initialised on a Bridge, XBridge or PIC slot.
- What `ppb_addr_space_assign` writes to the bridge window registers when no device sits behind the bridge.
- Whether the PCI-X mode bit at soft +116 is ever set on a Bridge or XBridge.
- Which devices (if any) the HDSP's i960 puts behind its function 0 bridge: nothing in the kernel says. The kernel's behaviour for an empty secondary bus is the scan above: 32 safe reads of offset 0 on the new bus number, each expected to return 0xffff in the vendor halfword.
