# IOC3 keyboard and mouse driver with nothing attached (IP27 and IP30 kernels)

What the IRIX 6.5 kernel's IOC3 keyboard and mouse driver (`ioc3_pckm.o`) does when a port has no device: the probe, the retry budget, what it
records, and what it leaves for the console code. The consumer is an emulator that wants a headless Octane (IP30) or Origin (IP27) to behave as the
real kernel expects. The PROM side (which decides the console before the kernel runs) is in chips `platforms/ip27/prom-console-input.md` and
`platforms/ip30/prom-console-input.md`.

Ratings: **established** (public IRIX 6.5.7 source and the shipped objects agree), **software expectation** (one of them states it, or the
behaviour depends on code not read), **inferred**, **unknown**. Nothing here is an observation of real hardware.

## Evidence

| What | Where | sha256 begins |
|---|---|---|
| IOC3 keyboard/mouse driver, IP30 | `6.5.22/usr/cpu/sysgen/IP30boot/ioc3_pckm.o` | `d09d9f074579c1e9` |
| the same, IP27 | `6.5.22/usr/cpu/sysgen/IP27boot/ioc3_pckm.o`, `6.5.30/usr/cpu/sysgen/IP27boot/ioc3_pckm.o` | `b406416cf8894d73`, `159a1304d5d86fd0` |
| textport keyboard glue | `6.5.22/usr/cpu/sysgen/IP30boot/tportpckbd.a(pckeyboard.o)` | not hashed |

The three driver objects disassemble to the same code: IP30 against IP27 differs in three gp-relative offsets only, and the 6.5.22 and 6.5.30 IP27
copies are identical. Addresses below are object offsets in `.text` (same in all three). Public IRIX 6.5.7 source (`irix/kern/io/ioc3_pckm.c`,
`irix/kern/sys/PCI/ioc3.h`) was read for the structure and agrees with the objects at every routine listed.

## Controller registers the driver uses

Offsets in the IOC3 register block (see chips `devices/ioc3/README.md`).

| Offset | Role | Bits the driver uses |
|---|---|---|
| `0x9c` | keyboard and mouse control and status | attach stores `0xc00000` (clamp-after-three-characters, both ports). Bit 0 and bit 1: transmit pending, keyboard and mouse. Bits 16 and 17: port timeout. Bits 18 and 19: timeout interrupt enable. Bits 7 and 11: pull clock low |
| `0xa0`, `0xa4` | keyboard, mouse receive | three byte slots, oldest in slot 0. Bits 31, 30, 29 mark slots 0, 1, 2 (bytes at 23:16, 15:8, 7:0) valid; bits 26, 25, 24 are the matching framing or parity error; bit 28 overflow; bit 27 source (0 keyboard, 1 mouse). A read clears the slots |
| `0xa8`, `0xac` | keyboard, mouse transmit | low byte is the byte to send |

Established (source and `pckm_attach` at `0x5c`, `pckm_pollcmd` at `0x1810`, `pckm_sendok` at `0x1ba8`).

## What an absent device looks like to the driver

| Step | Driver action | Absent device | Rating |
|---|---|---|---|
| Wait for transmit idle (`pckm_sendok`) | read `0x9c`, loop while the port's transmit-pending bit is set: at most 250 reads, 50 us apart | the loop is bounded, so a pending bit that never clears costs 12.5 ms and is then ignored | established (loop); unknown (whether real IOC3 leaves the bit set with no device) |
| Send (`pckm_pollcmd`) | write the command byte to `0xa8` or `0xac`, then up to 250 times: wait 50 us, read `0xa0` or `0xa4`, stop at the first read with any of bits 31:29 set | no valid bit ever sets; the routine returns the last read after 12.5 ms | established |
| Identify (`check_port`, `0x1fe4`) | flush with one read, send identify (`0xf2`), then up to 3 passes: classify every valid byte, and if nothing identified the device, either (acknowledge seen) wait 300 ms and read once more, or (no acknowledge) flush and send identify again | no byte: three more identify sends, so four identify writes and about 50 ms of timeouts per port; returns ENODEV (19) | established |
| `set_ports` (`0x1e84`) | run `check_port` on port 0, then port 1. Identify answers: `0x83` after acknowledge `0xfa` and identity-response `0xab`, or `0x47` then `0x53` after an acknowledge, mark the keyboard port; `0x00` after an acknowledge marks the mouse port | both ports stay ENODEV; the board records "default keyboard and mouse" (keyboard on port 0, mouse on port 1) for later hot plug | established |
| Keyboard probe in `pckm_attach` | only if `keyboard_port` is set: send `0xf5` and expect `0xfa`, set defaults, send `0xf4` and expect `0xfa`, then mark the port existing | skipped; the port's existing flag stays clear | established |
| Mouse probe | only if `mouse_port` is set: send `0xf6`, expect `0xfa`, then set the sample rate (`0xf3`, `0x28`) | skipped | established |
| Interface vertices | `pckb` and `pcms` character-device vertices are added in every case, on whichever port the other device did not claim (keyboard falls on port 1 if the mouse is on port 0, otherwise port 0) | both exist; neither is marked existing | established (source); software expectation (binary: the calls were not traced) |
| Interrupt | the keyboard/mouse interrupt is connected and enabled, but the per-port timeout enable (`0x9c` bits 18, 19) is set only for a port that exists | no timeout interrupts from an absent port | established |

Retry arithmetic for the model: each identify costs a send plus up to 250 x 50 us of polling, so an emulator that returns "no valid bit" at once makes
the whole probe finish early rather than wrong; nothing in the kernel measures the elapsed time.

## Hot plug

`valid_data` (`0x1ce8`), called from the interrupt handler `pckm_intr_lock` (`0x16cc`) for both ports, reads each receive register until no valid bit is
set. A valid byte on a port whose existing flag is clear calls `pckm_restart` (`0x2320`), which runs `check_port` again and, if it finds a keyboard or
mouse, installs it (keyboard: defaults, existing flag, timeout enable). If the board came up with neither device, a keyboard is accepted only on port 0
and a mouse only on port 1; the other way round raises an alert and the device is not installed. A device that is plugged in later also needs an IOC3
keyboard interrupt to arrive (bit 22 of the IOC3 interrupt register, per chips `devices/ioc3/registers-extra.md`); how the real part raises it on a connect is not known. Established (code); unknown
(hardware trigger).

## Mouse without keyboard, keyboard without mouse

The driver treats the two ports independently, so there is no ordering effect in the probe itself. The differences are in the callers:

- Keyboard missing: `pckm_kbdexists` (`0x18ec`, exported as `pckm_kbdexists`) returns 0 when the console board's keyboard port is ENODEV or its
  existing flag is clear. Set LEDs, bell and the textport port-handler routines return without doing anything when `keyboard_port` is ENODEV.
  Established.
- Mouse missing: nothing queries it. The `pcms` vertex exists and reads never produce data. Established.
- The textport keyboard glue (`keyboard_check`, in `pckeyboard.o`) calls `pckm_kbdexists`, and once it has seen a keyboard it never asks again.
  `keyboard_init` calls it once at start. Which graphics or console code consumes `keyboard_check` was not found in the IP30 boot objects.
  Software expectation.

## Console fallback

Nothing in `ioc3_pckm.o` chooses a console or falls back to serial. The kernel console objects take their choice from the PROM: `cn.o` imports the
boot argument `arg_console`, and `ioc3_is_console` (`ioc3.o` at `0xb98`) compares the IOC3 against the console IOC3 handle. So a headless boot in which
the PROM switched to the serial console stays serial in the kernel, and a boot in which the PROM kept graphics goes on with a graphics textport whose
keyboard simply never answers. Software expectation: the gfx and textport code (`gfx.o`, `cn.o` internals) was not read. Unknown whether the textport
refuses to open or falls back to serial when `pckm_kbdexists` returns 0.

## Unknowns

1. Real IOC3 values at `0x9c`, `0xa0` and `0xa4` with a port unplugged (every read here is a software test, not a register image).
2. Whether the transmit-pending bit clears, and whether a timeout bit (`0x9c` bit 16 or 17) sets, when nothing drives the clock.
3. Which kernel code consumes `keyboard_check` and what it does when the keyboard is absent.
