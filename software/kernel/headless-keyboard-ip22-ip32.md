# Indy, Indigo2 and O2 kernels with no PS/2 keyboard or mouse

Question. On the IRIX 6.5 kernels for Indy and Indigo2 (IP22/IP24, HPC3 keyboard controller), Power Indigo2 (IP28) and O2 (IP32, MACE PS/2 ports): how does the kernel decide a keyboard or mouse is absent, and what does it do about the console? The PROM side is in chips (`platforms/ip22/prom-console-input.md`, `platforms/ip32/prom-console-input.md`).

Method. Read from the shipped 6.5.22 and 6.5.30 kernel objects (below), checked against the public 6.5.7 source tree (`irix/kern/io/pckm.c`, `irix/kern/io/mhpckm.c`, `irix/kern/io/cn.c`, `irix/kern/sys/pckm.h`), whose text is not repeated here. Nothing was run on hardware or in an emulator, so nothing is **observed**. Grades: **established** (the routine was read and the claim follows), **software expectation** (what the driver assumes a controller or device returns), **inferred**, **unknown**.

| Object | Release | sha256 begins |
|---|---|---|
| `IP22boot/pckm.o`, `cn.o`, `tport.a`, `tportpckbd.a` | 6.5.22 | `3a7442daa2e62a34`, `49c7352892f3003e`, `1591be27973c401e`, `237c3bf4c5191201` |
| `IP32boot/mhpckm.o`, `cn.o`, `tport.a`, `tportpckbd.a` | 6.5.22 | `7c9fdbbac5e53706`, `d650ec3124e8a38a`, `d425d0985a8bc389`, `1f0ddee0bd24984b` |
| the same four, IP32 | 6.5.30 | `fd0938e2ac2241d2`, `09f9df616ef65391`, `39ccfa3af4314324`, `2087fb7922f26c2f` |

- The IP32 objects of 6.5.22 and 6.5.30 disassemble to the same instructions (compared whole for `mhpckm.o`, `cn.o`, `tport.o` and `pckeyboard.o`; only the file name line differs). Established.
- No linked O2 kernel is held, and the 6.5.30 media carry one IP22 file, so the IP22 objects were read for 6.5.22 only. No IP28 kernel objects are held: IP28 rests on the source, which builds one controller driver for IP22, IP26 and IP28. Established for the source, unknown for the IP28 binary.
- Object addresses are offsets in `.text`.

## Console selection (all four machines)

- `cn_init` (`cn.o` `0x804`, same in IP22 and IP32) reads the kernel's copy of the `console` variable. First character `g` or `G`: graphics textport. `d` or `D` followed by exactly `2`: second serial port. Any other value, `d`, or a value like `d1`: first serial port. Variable not set: graphics. Established (binary matches source).
- For graphics it calls `tp_init` (`tport.o` IP22 `0xff8`, IP32 `0x1018`). Only when that returns 0 (no graphics driver in the table initialised) does it fall through to the first serial port. Established.
- `tp_init` calls `keyboard_init` (call at `0x11b4` IP22, `0x11d4` IP32) and then returns 1 whatever it returned: the result is never tested. So a missing keyboard does not move the console to serial. Established (IP22 6.5.22, IP32 6.5.22 and 6.5.30).
- `keyboard_init` (`tportpckbd.a` `pckeyboard.o`): runs the serial-line init, then the keyboard-port init, then tests whether the keyboard was found. IP22 prints `gfx: no keyboard` through the kernel's debug-print routine when it was not (the string is at the `keyboard_init` call to `dprintf`, `0x38`); IP32 prints nothing and caches the answer for `keyboard_check`. Established for the code; whether the IP22 line reaches a console on a production kernel is unknown.
- The kernel never reads ConsoleIn or ConsoleOut. If the PROM fell back to serial for lack of a keyboard but left `console` at `g`, the kernel still opens the graphics textport (inferred: the code path is established, that the PROM leaves `console` at `g` is established in the PROM page, and where the kernel's `console` copy is filled from was not read).

## IP22 / IP24 / IP28 (HPC3 keyboard and mouse controller)

- Registers: data port byte at physical `0x1fbd9843`, status and command port byte at `0x1fbd9847` (`pckm.o` `_pckm_pollcmd`, `lui 0xbfbd` / `ori 0x9843` and `0x9847`). Established. Every write is followed by a read of the boot ROM word at `0x1fc00000`, as a write barrier.
- Status bits the driver uses: `0x01` output full, `0x02` input full, `0x20` data is from the mouse, `0x40` timeout, `0x80` parity. Established. Command byte (command `0x60` to write, `0x20` to read): `0x01` keyboard interrupt, `0x02` mouse interrupt, `0x10` keyboard disabled, `0x20` mouse disabled. A byte written to the command port as `0xd4` sends the next data byte to the mouse. Established.
- `early_pckminit` (`pckm.o` `0x0`) does the whole probe once at boot. Keyboard: write command byte `0x20` (mouse off); flush input; send `0xf5`; if the reply is `0xfa`, set LEDs off, scan set 3, make/break (`0xed 0x00`, `0xf0 0x03`, `0xfa`, `0xfc 0x14`, `0xfc 0x76`), flush, send `0xf4`; a second `0xfa` marks the keyboard present. Anything else, including a timeout, leaves it absent. Established.
- Mouse: write command byte `0x10`, wait 500 us, flush, send `0xd4 0xf6`; reply `0xfa` marks it present and sets resolution (`0xe8 0x03`). 6.5.22 then calls `default_ms_setting` (`0x1300`; call site read, body read only in the IP32 object, `0x14f0`), which sends a plain or a wheel-mouse set-up depending on a mode variable: the wheel form is the sample-rate sequence `0xf3 0xc8`, `0xf3 0x64`, `0xf3 0x50` followed by an ID read (`0xf2`). The 6.5.7 source has no such step. Established for the IP32 body; inferred that the IP22 body is the same. The default mode value was not read.
- Last, it installs the interrupt vector and writes command byte `0x03` (both interrupts on). Established.
- One command (`_pckm_pollcmd`, `0xfc8`): wait up to 20834 x 2 us for input-full to clear, write, wait 5 us, then poll status up to 400 times, 50 us apart. A parity error flushes the data byte and retries; a reply `0xfe` retries; a reply `0xff` is ignored. **A timeout bit on the keyboard port returns -1 at once, with no retry.** After 400 polls it retries, 3 attempts in all, then returns -1. Established. About 20 ms per attempt (400 x 50 us) when no timeout bit ever appears; an inference from the delay constants.
- Keyboard-present flag: `pckm_kbdexists` (`0x112c`) reads it. Established.
- Hwgraph nodes `input/keyboard` and `input/mouse` are added unconditionally (`pckminit`, `0x1ec`), present or not. Opening either with no device sends its setup commands, ignores every reply and succeeds. Established for the code; what X does with a silent mouse was not read.
- Interrupt path: an input-full status with the timeout bit set discards the byte (marks the LED update as unneeded). A byte `0xaa` on the mouse stream or `0xaa`/`0xfc` on the keyboard stream is taken as a device plugged in and queues a re-init. Established.

## IP32 (MACE PS/2 ports)

- Registers: keyboard port at physical `0x1f320000`, mouse port `0x20` above it; each is four 64-bit registers: transmit buffer `+0x00`, receive buffer `+0x08`, control `+0x10`, status `+0x18` (`mhpckm.o`, `pckm_attach` `0x10`; the PROM uses the same). Established.
- Control bits: `0x01` inhibit clock after transmit, `0x02` transmit enable, `0x04` transmit interrupt, `0x08` receive interrupt, `0x10` assert clock. Status bits: `0x01` clock, `0x02` inhibit, `0x04` transmitting, `0x08` transmit buffer empty, `0x10` receive buffer full, `0x20` receiving, `0x40` parity error, `0x80` framing error. Established (header and binary agree).
- Send (`outb`, `0x1154`): wait while transmitting or receiving (up to 20834 x 2 us), write control 0, wait 100 us, write the byte to the transmit buffer, write control `0x12` (transmit enable and assert clock). Established.
- One command (`_pckm_pollcmd`, `0x11e4`): send, then poll status up to 400 times, 250 us apart, for receive-buffer-full; reply `0xfe` retries; `0xff` is ignored; on timeout retry, 3 attempts in all, then return -1. **There is no controller timeout bit, so an absent device is only ever found by the 400-poll timeout:** about 100 ms per attempt, 300 ms per command (inferred from the delay constants). On the older MACE revision (ID register at `0x1f310000`, bit `0x10` clear) the received byte is shifted right one bit with parity carrying bit 7; on the newer one a parity error flushes and retries. Established.
- `pckm_attach` (`0x10`) discovers which port is which, because either connector can hold either device. For port 0 then 1: flush; send `0xf5`; no reply means nothing there (a message `No kbd/mouse on port %d` prints only when the boot option `showconfig` is on, otherwise silent); on `0xfa` flush and send `0xf2`; if that is not acknowledged, `Can't issue ID command kbd/mouse port %d` prints unconditionally; otherwise wait 2.5 ms and read the receive buffer, wait 2.5 ms and read again: `0xab` then `0x83` is a keyboard, `0x00` then `0x00` a mouse. Wait 200 us between ports. Established.
- Assignment: neither found, keyboard is port 0 and mouse port 1; one found, the other gets the other port. Then the same keyboard and mouse init as IP22 above (`0xf5`, setup, `0xf4`; mouse `0xf6`, `0xe8 0x03`, `0xf4`), each present flag set only on `0xfa`. The nodes `pckb` and `pcms` (and `input/keyboard`, `input/mouse`) are created unconditionally. The receive interrupt is enabled on both ports. Established.
- 6.5.22 and 6.5.30 add the same wheel-mouse sample-rate sequence and ID check (`default_ms_setting`, `check_port`). Established.
- If both ports answer as keyboards or both as mice the second wins the role it matches; the odd cases were not worked through. Unknown.

## Differences between the source and the shipped binaries

- Both shipped kernels match the 6.5.7 source on every point above except the wheel-mouse step and a helper that tests the mouse ID. No other difference was found, but only the routines named here were compared. Established for those routines.

## Mouse without keyboard, keyboard without mouse

- Kernel, any of the three controllers: the two present flags are separate and nothing ties them together. A missing keyboard does not hide the mouse node; a missing mouse does not stop the textport. Only the keyboard's absence is ever reported, and then only by the IP22 debug line. Established.

## What stays unknown

- What the real HPC3 controller returns for a keyboard or mouse that is not connected (timeout bit, output-full with `0xff`, or nothing). The drivers cope with all three; no capture or manual in the repositories states it. Software expectation only: the kernel treats output-full with the timeout bit as a dead keyboard and drops the byte.
- What real MACE returns while a device is absent beyond "receive-buffer-full never sets" (software expectation: the MACE specification does not describe a timeout).
- IP28 binaries, and the 6.5.30 IP22 kernel.
