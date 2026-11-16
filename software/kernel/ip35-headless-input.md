# IP35 kernel with no PS/2 keyboard or mouse: probe, console choice, USB

How the IRIX 6.5.30 IP35 kernel (`6.5.30/unix.IP35`, sha256 begins `412a1aae553a3483`) finds that a PS/2 keyboard or mouse is missing, and whether that moves the console to serial. For the consumer: an emulated IP35 machine with no input device attached. The PROM side is in chips `platforms/ip35/prom-console-input.md`; the PROM, not the kernel, takes the fallback to serial.

Grades: **established** (routine read in full from the shipped binary or from the public IRIX 6.5.7 source), **observed**, **software expectation**, **inferred**, **unknown**. IP35 support postdates 6.5.7, so the public source covers only the shared PS/2 driver logic (`irix/kern/io/ioc3_pckm.c` and `irix/kern/sys/pckm.h`, `stand/arcs/lib/libsk/io/ioc3_pckm.c`), not the IOC4 variant or USB. Binary addresses are for 6.5.30; 6.5.22 was not compared.

## Probe and attach

| Fact | Grade | Where |
|---|---|---|
| The IOC4 attach (`ioc4_pckm_attach`, 0xc0000000000c9274) returns -1 at once if the chip's keyboard/mouse subdevice is not enabled for connectors. Otherwise it writes `0xc00000` to the control register (IOC4 `+0x200`), runs the port scan, then confirms each device with a command. | established | `unix.IP35` |
| Port scan (0xc0000000000cb014): each of the two ports is asked for its ID with `0xf2`. A keyboard answers `0x83` or `0x47`; a mouse answers `0x00`, `0x03` or `0x04` (plain, 3-button wheel and 5-button wheel IDs; the source knows only `0x00`). A second device of a kind prints a message (the severity level was not decoded). If neither port answers, a "default ports" flag is set (keyboard port 0, mouse port 1). | established (IDs read from the code; naming the 3 and 4 IDs is inferred) | `unix.IP35`, public source `set_ports` |
| Keyboard confirmation: send `0xf5` and require `0xfa`; configure scan mode, LEDs and make/break; send `0xf4` and require `0xfa`. Only then is the keyboard marked present, the `pckb` hwgraph device created, the console keyboard registered (once), and its timeout bit set in the control register. Mouse: send `0xf6` and require `0xfa`, then configure and create `pcms`. | established | `unix.IP35` |
| Timing in this kernel: after each write, 250 reads of the receive register at 50 microseconds until bits 31:29 are non-zero (about 12.5 ms), and the ID probe makes 3 rounds, with a 300 ms wait when a bare `0xfa` was seen. The PROM uses 25 reads at 1 ms. | established | `unix.IP35` (0xc0000000000ca9a0, 0xc0000000000cb174 in the scan), public source |
| With no device, the keyboard and mouse port indices stay 19 (none), no `pckb` or `pcms` node is made, no console keyboard is registered, and with `showconfig` the kernel prints `no keyboard found` and `no mouse found` for the console IOC only. The interrupt is still connected, so a keyboard plugged in later is detected by the restart path (public source `pckm_restart`). | established (attach); the IOC4 restart path was not read | `unix.IP35`, source |

## Console choice

| Fact | Grade | Where |
|---|---|---|
| `cn_init` (0xc0000000000dc9cc) reads the ARCS `console` argument. Unset, `g` or `G`: textport (`tp_init`). Any other value: serial (`du_init`). It sets `console_is_tport` to 1 when the textport comes up. No keyboard test is part of the decision. | established | `unix.IP35` |
| If `tp_init` returns 0 (no usable graphics), the code falls to the serial branch instead. | established | `unix.IP35` |
| `pckm_kbdexists` returns 0 until a console keyboard registered itself (`ioc3_pckm_mlreset` or the IOC4 equivalent copies six routines into the `cn` structure). With no PS/2 keyboard nothing registers, so it stays 0. Who calls it was not read. | established (return value); callers unknown | `unix.IP35` (0xc0000000000dd05c, 0xc0000000000dcffc) |
| A graphics console with no keyboard therefore stays the textport console, output only. The serial fallback seen on a Fuel is the PROM's own and lasts for that PROM session: it changes ConsoleIn and ConsoleOut but not the `console` variable that `cn_init` reads. Whether the kernel also reads `arg_consolein` and `arg_consoleout` was not read. | inferred from the two read halves; no kernel boot without a keyboard is held | PROM page, `cn_init` |

## USB keyboards

`usb_early_pckminit` is an empty routine. A USB HID keyboard is recognised by `usb_hid_iskbd` (0xc0000000002037ec), which looks through the interface descriptors for a boot-class keyboard entry; the exact field it tests is not decoded. The ARCS-side USB keyboard path is the PROM's (`/dev/input/usb/keyboard`). How the kernel's HID driver registers a console keyboard and whether it competes with PS/2 for the console was not read. Unknown.

## Mouse without keyboard, keyboard without mouse

Established: the two ports are probed and confirmed independently (`0xf5/0xf4` for the keyboard, `0xf6` for the mouse), and a failure on one leaves the other's `pckb` or `pcms` node. Only the keyboard registers with the console layer, so a mouse without a keyboard does not make `pckm_kbdexists` true. The console choice never reads either.

## Unknown

- The IOC4 interrupt-restart behaviour after hot-plug on 6.5.30.
- Who calls `pckm_kbdexists`, and what the textport does with no keyboard (it was not read).
- The USB HID console path in the kernel.
- A booted IRIX run with no keyboard on any IP35 machine: none is held.
