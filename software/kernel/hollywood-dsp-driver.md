# SGI Hollywood DSP audio driver and its user interface

Not the Sonic Solutions HDSP PCI audio card, which is a different product.

Scope: the IRIX audio character device that the Indigo's HPC-attached DSP audio hardware used (`hdsp`), the control interface it shares with the
in-kernel audio framework (`kdsp`) that replaced it, and the audio library's view of both. The hardware side (registers, SRAM, DMA, interrupts)
is in chips (`docs/audio/hollywood-dsp.md` there). Labels as in [AGENTS.md](../../AGENTS.md): **established**, **inferred**, **unknown**.

## What is and is not held

- The `hdsp` driver object itself is **not held** in either release, for any platform. 6.5.22 carries only its configuration entry (`master.d/hdsp`
  under `var/sysgen.IP27`, `IP30`, `IP35`: character device, prefix `hdsp`, major 53, comment "Hollywood DSP"). **Established.**
- `kdsp` is held. Its master entry says "use instead of hdsp", gives the same major number 53 and flags `cso`, plus a thread class `audio`. The two
  drivers therefore cannot both own major 53: **established** (both master files read in full). `indigo_dd` (prefix `indigo_`, depends on `kdsp`)
  is named by its master file, with no object held. It is the likely kdsp-based driver for the same Indigo hardware: **inferred** from the
  dependency and the name.
- Everything about the ioctl numbers below is read from `kdspioctl` and its helpers in the 6.5.22 `IP22boot/kdsp.a`, member `kdsp_di.o` (N32
  relocatable, not stripped, with debug records), `.text` offset `0x2224`, size `0x160c`. Offsets in parentheses in the ioctl table are
  relative to the start of `kdspioctl`; for the other functions the figure after the name is the function's offset in that member's `.text`. The other platforms' `kdsp.a` members were not compared; the IP30 and
  IP27 builds are 64-bit objects with a different table location, so a reader must not assume the dispatch table is identical (**unknown**).
- The numbers and structure shapes also come from the two shipped driver headers (public interface and private header), which say the
  public header is shared by `hdsp` and `kdsp` and that the private header is only partly used by `kdsp`. That sharing is the reason the `kdsp`
  readings stand in for `hdsp`: **inferred**. Where `hdsp` differs (it handles one request that `kdsp` rejects, and has hardware-specific
  state), the difference is marked.
- The audio library (`libaudio.so.1`, all three ABIs) and `6.5.30/usr/sbin/audiostate` are held for 6.5.30 (a 6.5.22 copy was added to `root/6.5.22` at the same time as this reading and was not compared). They are read
  only for strings and imports so far ([What libaudio opens](#what-libaudio-opens)). Library-side claims about parameters are from the shipped
  `audio.h`, whose parameter and type numbers match the driver header's one for one wherever I compared them (three spot checks: the lock-state
  parameter 40, the input-port type 23, the input-ring type 25). **Inferred.**

## Device nodes and minors

Read in `kdspopen` (`0x1b08`), `kdspmap` (`0x1dfc`) and the top of `kdspioctl`: the minor number selects the role. **Established** for `kdsp`.

| Minor | Role | Notes |
|---|---|---|
| 0 | master | Gets the slot-allocation request. |
| 1 | control | The only minor that may issue the reset and activate requests. |
| 2 to 17 | client ports | A 16-entry state table; a port's state byte is -2 unopened, -1 opened but not mapped, 0 mapped. An open takes a free ring-buffer record (error 16, EBUSY, if none). |
| 256 and up | event queues | `open` calls the event-queue opener. Event requests use the queue index `minor - 257` (established as the argument; whether 256 is reserved is **unknown**). |

The header names the minors the same way (master 0, control 1, first client 2, first event minor 256) and says 16 user rings and 32 connections at most.

A client learns its minor with the "find minor" request (below), then opens that minor, maps it, then acquires its ring.

## mmap region (kdspmap, 0x1dfc)

**Established** for `kdsp`; **inferred** for `hdsp` (the private header's ring record has the same header/data split, `nbytes` and `maplen`).

- Only client minors (2 and up) may map; minors 0 and 1 get EBADF (9).
- One region per port. Length must be above 0 and at most `0x190000` bytes (1,638,400; the public header names this limit as the maximum ring length).
  Larger or non-positive lengths return EINVAL (22).
- The kernel allocates the pages itself (cache-colour-aligned when the machine needs virtual-cache-alias avoidance, with the shared `kvpalloc`
  call) and maps them into the caller with `v_mapphys`. The first 16 bytes (four 32-bit words) are the ring header and the data follows at `+16`.
  After the mapping succeeds the four header words are cleared and the port's state becomes "mapped" (0).
- On mapping failure the pages are freed and the error from the mapper is returned; ENOMEM (12) if the page allocation fails.
- There is no second region (no register window, no DSP memory window) in `kdsp`. Whether `hdsp` exposed DSP SRAM or registers through mmap is **unknown**.

## Ring buffer (host side)

- Header, four words in the mapped page, in order: a first index, a second index, a request word and a fill mark. The header's names are
  "head", "tail", "intreq", "fillpt". **Inferred** (header names); the first two words' arithmetic in the flush and status requests below is **established**.
- The ring is `nlocs` 32-bit locations including the four header words. `acquire_rb` (`0x7b10`) stores usable locations as `nlocs - 4` and
  `nbytes = nlocs * 4`, requires `nlocs * 4` not to exceed the mapped length (EFAULT, 14, otherwise) and records the data end as the data start plus
  `nbytes - 16`. **Established** for `kdsp`.
- Direction: the acquire request's direction byte 0 sets the ring type resource to `0x20000018` (output-port type), direction 1 sets it to
  `0x20000017` (input-port type). Any other value returns EFAULT. **Established.** (So the port's resource type is named from the system's
  side: a ring that carries data to the CPU is an "output port".)
- Folding and sign extension are ring-type flag bits chosen in the public header (to-CPU 0, from-CPU 1, folded 2, sign-extend 4). The private
  ring flags (user-space-mapped 8, split header 0x10, no-defer 0x20, master 0x40, direct 0x80, dormant 0x100, big samples 0x800, subcode 0x400)
  are visible to `kdsp` only as one flag word in the port record at `+0x1a4`; `kdspopen` sets the user-space bit and `acquire_rb` ORs in the
  direction. **Established** for the bits those two routines set (bit 8 at open and acquire; the direction bits).
- The second ring-buffer request (below) reports and sets the `location` fields that the DSP wrote on a ring error: a 48-bit sample position held
  as two 24-bit halves (low half first). **Established** (`kdspioctl` case 6, `0x1188`).
- The flush request (case 12, `0xf78`) shifts the caller's frame count by the port's frame shift, clamps it to the amount queued (`word1 - word0`,
  corrected by the ring size when negative) and pulls the second index back toward the first by that amount, wrapping at the ring size;
  returns the number of frames removed. **Established** (arithmetic); which side (producer or consumer) that makes it is **inferred**
  (the second word is the write position for rings the application fills).
- How the DSP and `hdsp` update head, tail and the request/fill words, and what the request and fill words mean to the DSP, are **unknown**:
  no `hdsp` object is held and `kdsp` leaves them to the device back ends (`rad_dd`, `a3_dd` and the others).

## ioctl requests

Dispatch: `kdspioctl` (frame `0xf10`) reads the minor, loads the port's state byte (or -3 for minors 0, 1 and 256 up), and jumps through a table
of 244 entries (cmd below 244; anything higher returns EINVAL). All requests not in the table below fall to EINVAL (22): the table has 38 live
entries. "Mapped" means the port's state byte is 0; a request that needs it returns EACCES (13) otherwise. Errors are the numeric
values from `errno.h` (EACCES 13, EFAULT 14, EINVAL 22, ENODEV 19, ENOENT 2, ENXIO 6, EPERM 1, EEXIST 17). **Established** for `kdsp` throughout,
unless a row says otherwise.

| # | Name | Argument at the user pointer | What it does (target in `kdspioctl`) |
|---|---|---|---|
| 0 | reset | none | Minor 1 only (else EACCES). Calls an optional per-subsystem hook at `+0x11c` of the subsystem table, without taking the framework lock (0xabc, hook call at 0xc98). **hdsp:** the hardware reset (clear microcode). |
| 1 | activate | none | Minor 1 only. Takes the framework lock and calls the hook at `+0x120` (0xaa8, 0x1088). **hdsp:** presumably starts the DSP after microcode load (**inferred**). |
| 2 | find free minor | out: one int | Scans the 16 client state bytes for -2; stores `index + 2` (so 2 to 17) as the result; none free stores -1 (0xa2c, 0xeb0). |
| 3 | acquire audio ring | in: 8 bytes `{int nlocs; char direction}` | Requires a client minor that is mapped (state byte 0; else EEXIST 17 when mapped state is wrong); calls `acquire_rb` (0x9d4). The return value is 0 or the helper's error. |
| 4 | set old-style parameters | in: array of ints, `[n, (param, value) x ((n-1)/2)]`, n up to 64 | `copyin` the length then the whole array; calls `kdsp_old_set_params` under the lock (0x934, set branch 0x122c). |
| 5 | get old-style parameters | in/out: same array | Fills the values; `copyout` n ints (0x934, 0x9a0). |
| 6 | get ring status | out: 5 ints | Error count, last error type, error length (in frames), then the 48-bit error position as two 24-bit ints (0x1188). Mapped only. |
| 7 | set ring status | in: 5 ints | Mapped only. Copies the 20 bytes in and does nothing with them (0x1068). **Established** (whole path read). |
| 8 | set ring info | in: 24 bytes `{char name[20]; int alversion}` | Mapped only. Copies at most 31 name bytes into the port record at `+0xa0` (NUL at `+0xbf`), stores the version at `+0x1dc`; sets a per-ring count word (`+0x160`) from another (`+0x1ac`) and, when the version is 5 or more and the big-sample flag is set, makes it six times that (0xfec; the meaning of the two words is **unknown**). |
| 9 | get all ring info | out: array of 56-byte records | Walks the allocated-ring list and fills one record per port: name (19 bytes plus NUL), owner pid (+0x14), type, error count, channels, head, tail, position count; copyout `count * 56` bytes; the return value is the count (0x800). No state check. |
| 10 | set ring channels | by value: channel count | Opened but not yet mapped only (state -1; else EACCES). Calls `kdsp_set_channels(port, n)`; zero from it gives ENXIO (6) (0x7ec, 0xfd4). |
| 11 | get ring position and time | out: 24 bytes `{u64 sample count; u32 head; u32 tail; u64 system time}` | Mapped only. Under the framework's raised priority, starts from the hardware record's 64-bit sample counter, adds or subtracts the queued frames depending on direction, and copies out head and tail. The last 8 bytes (system time) are not written by this path (0x7dc, 0x10f0). |
| 12 | flush | by value: frame count | Mapped only; see above (0x7cc, 0xf78). |
| 13 | port control | rejected, EINVAL | In the public header; **kdsp has no entry (0xbc)**. **hdsp:** unknown. |
| 14 | list old-style parameters | in/out: `{int *buf; int count}` | Copies up to `count` entries of the old-parameter table (pairs of param, kind) and returns the table size 34 (0x708). |
| 15 | old parameter limits | in: int parameter; out: two ints in place, min then max | Looks the parameter up in the 17-entry table (0x6a8). ENOENT (2) if absent. |
| 16 | old parameter name | in: `{int param; char *buf}` | Copies the name string and NUL; ENOENT if absent (0x644). |
| 17 | old parameter default | in: `{int param}` | Returns the initial value as the call's result (0x608). |
| 18 | acquire direct audio ring | rejected, EINVAL | **kdsp has no table entry**, yet the acquire path tests for number 18 and passes a direct-ring flag (0x80): the code is shared, the table entry was left out. **hdsp:** accepts it (**inferred**). |
| 19 | get ring system-time and sample count | out: 24 bytes | Mapped only. Calls the hardware record's position-and-time hook at `+0x158`, or computes the sample count and converts a tick count with `ust_to_nano` when the hook is absent (0xf20, 0x14d0). |
| 20 | get resource | out: one int | Returns the caller's resource id for this ring (the field at `+0x98`) (0x5e8, 0xefc). |
| 21 | set device | by value: resource id | Mapped only. Stores the id in the port record (`+0x1e4`), where `acquire_rb` later connects the ring to it (0x5d8, 0xed0). No validation here. |
| 22 | connect | in: count then `count * 24` bytes of 24-byte parameter/value triplets, count up to 64 | Walks the list; recognised parameters are the source (0x10000016), destination (0x10000017), association (0x10000027) and sample format (0x1000002d) tokens; finds or creates a connection; returns the connection id (0x4b0). Source and destination tokens are checked for resource subtype and ownership, with the capability bit 37 as the override (`cap_able`); refusal is EPERM (1), an unknown resource ENODEV (19), a bad count EFAULT (14). |
| 23 | disconnect | by value: connection id | Checks the resource is a connection and that the caller owns it or holds capability bit 37, then deletes it (0x48c, 0xd3c). EPERM (1) if refused, ENODEV (19) if unknown. |
| 24 | set parameters | in/out: count then `count * 24` bytes, count up to 64 (outside 0 to 64 is EFAULT, 14) | Applies the list through `kdsp_pv_list_op` (set operation); copies back the results; returns the number applied (0x3dc). |
| 25 | get parameters | same | Same call, get operation (0x3dc). |
| 26 | query value set | in/out: 56 bytes (see below) | Looks up the resource and parameter, fills the caller's set buffer with up to `setsize` 8-byte values, returns the element count (0x348). |
| 27 | parameter info | in/out: 168 bytes (see below) | Returns the type, element type, maximum element counts, name and initial/min/max values for one parameter of one resource (0x314, 0xd60). |
| 28 | resource by name | in/out: 116 bytes `{char name[80]; int ids[8]; int count}` | Resolves a name path to up to 8 resource ids and the depth (0x268). |
| 29 | set ring format | by value: format token | Opened but not yet mapped only (else EACCES). Looks the token up in an 8-entry table (first the 32-bit audio sample, then the seven tokens below); ENXIO (6) if absent. Stores the table index in the port record (`+0x1a8`), sets the subcode flag for any nonzero index and clears it for index 0, and sets the big-sample flag when the entry's fourth word is 2 or more (0x254, 0xcbc). |
| 101 | event get | out: 64-byte event | Removes one event from queue `minor - 257` (0x220). Negative result from the queue is returned as the error. |
| 104 | event check | in/out: 64 bytes | Looks for a matching event (0x1d8). |
| 110 | event queue allocate | none | Allocates the queue for this minor (0x1c8). |
| 112 | event source mask set | in: 24 bytes `{int src; int pad; i64 mask; void *event}` | (0x1a8, 0xb0c). |
| 113 | event source mask clear | in: same | (0x18c, 0xafc). |
| 116 | event queue flush | none | (0x17c). |
| 118 | event pending count | out: one int | (0x148). |
| 241 to 243 | internal | none, out 68 bytes for 242 | Debug and timing statistics: 241 clears the statistics flag, 242 copies out a 68-byte block, 243 initialises and sets the flag. **Established**; purpose inferred from the helper names. |

The public header also defines event requests 102 (put), 103 (peek), 111 (queue dump) and 117 (queue info), and request 13 and 18 as above: all
fall to EINVAL in `kdsp` (**established**). Whether `hdsp` implements them is **unknown**.

## Structures (sizes confirmed by the copy lengths above)

Layouts from the public header, field names mine. Offsets and sizes are for the N32 ABI; 64-bit pointers pad the same way.

- **Parameter/value triplet (24 bytes).** `+0` int parameter token; `+4` int reserved; `+8` 8-byte value (a union of int, pointer and 64-bit);
  `+16` short input length; `+18` short second-dimension input length; `+20` short output length; `+22` short second-dimension output length.
  Confirmed: the dispatcher copies `n * 24` bytes.
- **Ring status (20 bytes).** Error count, error type, error length, two position halves. Confirmed by cases 6 and 7.
- **Ring position and time (24 bytes).** 8-byte sample count, int head, int tail, 8-byte system time. Confirmed by the 24-byte copyout in cases 11 and 19.
- **Set ring info (24 bytes).** 20-byte name, int version. Confirmed (0x18 copy).
- **Get ring info (56 bytes).** 20-byte name, pid, type, ring id, version, error count, channels, head, tail, locations. Confirmed (0x38 stride in case 9).
- **Acquire request (8 bytes).** Int `nlocs`, byte direction, 3 bytes padding. Confirmed (8-byte copy; byte at `+4` read by `lbu`).
- **Event (64 bytes).** Int source, int resource, int parameter, 4 bytes padding, 8-byte time stamp, 8-byte value, 32-byte buffer. Confirmed
  (0x40 copies).
- **Event source mask (24 bytes).** Int source, 4 bytes padding, 8-byte mask, pointer (padded to 8). Confirmed (0x18 copy).
- **Value-set query (56 bytes).** 8-byte set buffer pointer, int version, int resource, int parameter, int maximum size, 8 spare ints.
  Confirmed (0x38 copy; resource at `+0xc`, parameter at `+0x10`, size at `+0x14`).
- **Parameter info (168 bytes).** Int resource, int parameter, int value type, int element type, int max elements, int second max, 32-byte
  name (at `+0x18`), five 8-byte values (initial, min, max, min delta, max delta), special-values word, operations word, 60 spare bytes.
  Confirmed (0xa8 copy; name at `+0x18`; the value-type and element-type words read from short fields of the internal record).
- **Resource-by-name (116 bytes).** 80-byte name, 8 ints, a count. Confirmed (0x74 copy; ids from `+0x50`, count at `+0x70`).

## Old-style parameter table (kdsp_di.o, `.data +0x4c0`, 17 entries of 24 bytes)

Each entry: parameter number, kind (1 enumerated, 2 range, -2 count, -1 digital rate), minimum, maximum, initial value, name. **Established**
(table read in full). Parameters: 0 input source (0 to 2, name "Line/MIC/AES"), 1 and 2 left and right input attenuation (0 to 255), 3 and 4 input
and output rate (48000 default, up to 48000), 5 and 6 left and right output gain (0 to 255), 7, 8 and 9 input, output and unused ring counts
(maximum 16), 12 monitor control (0 to 1), 15 system channel mode (2 to 4, default 2), 16 speaker mute (0 to 1), 17 microphone mode (1 to 2, default 1),
18 and 19 left and right second-input attenuation (0 to 255), 20 digital input rate (`0x7d00` to `0xbb80` for the
range; kind -1). The public header has parameters 13 and 14 (left and right monitor attenuation) and 21 (lock state) that this table lacks.
The order of the table is not the numeric order of the parameters.

A second 8-entry table (`.data +0x2d0`, 32-byte entries) holds the ring sample formats of request 29: format 0 (32-bit audio) and tokens
`0x1000002f` to `0x10000035`, which are the public header's channel-status, user and validity parameters 47 to 53. **Established** (read);
meaning of the remaining fields **unknown**.

## Interrupts

- `kdsp` has no hardware interrupt handler of its own: its init creates a timer callback (the symbol `kdsp_timercallback`) and the ring state is moved by
  the back ends' interrupt routines. **Inferred** (symbol names only; the callback was not read).
- `hdsp`, from the private header (**inferred**): three sources on the controller (end of DMA, DSP wrote the transmit word, DSP read the receive word), a
  per-ring "interrupt service routine" function pointer in the ring record, and eight named routines for moving data between rings and the DSP
  (to/from CPU, folded/unfolded, kernel/user ring). Priority is the highest (`splhi`). The register-level wiring is in chips.
- What the DSP-write interrupt does, whether the driver polls the handshake word, and how DMA is chained across rings are **unknown**.

## Firmware download

**Unknown.** Nothing held shows the microcode load. What is known: the public header has a reset request and an activate request; `kdsp.a`
exports only `open`, `close`, `map`, `unmap`, `poll` and `ioctl` (so no `read` or `write`); the driver's private header defines three DSP states (reset
with no microcode, microcode loading, running), a client lock state, a flag for kernel writes that must not sleep (for stream drivers), and shared
SRAM offsets. These suggest a microcode download through the DSP SRAM window by a reset/activate sequence from a control process, with the
image coming from a user-space file. All of this is **inferred**; the image name and location, its format and the sequence are open questions.

## What else uses the interface

- `kdsp` itself, and through it `rad_dd`, `mad_dd`, `a3_dd`, `usbaudio_dd`, `emu_dd`, `a2_dd` and `indigo_dd`: their master files depend on `kdsp` (`emu_dd`'s master was not checked; the audio driver set is listed on the chips repository's audio drivers page). The debug records of `rad_dd`, `mad_dd`, `a3_dd`, `usbaudio_dd` and `kdsp_di.o` name the public header as a source of types.
  **Established** for those five objects (strings read); **inferred** that the others (not held) do the same.
- The inventory type for the Indigo DSP system is 0 (`INV_AUDIO_HDSP`), which `hinv` prints as "Iris Audio Processor". **Established** (see the
  chips repository's audio drivers page).
- The audio library and its applications: `libaudio.so.1` opens the device (below). Of the other held programs, `audiostate` has no `hdsp` string, imports
  `open`, `ioctl` and the library's `al*` calls, and names `/dev/graphics` for its panel code; the IP22 NG1 `Xsgi` carries the same three device-node strings
  as the library (statically included library code, Inferred from the identical strings). Whether any other 6.5.30 program opens the nodes directly is **unknown**
  (only a small part of the userland is held).

## What libaudio opens

`usr/lib32/libaudio.so.1` (and the o32 `usr/lib/libaudio.so.1`) contain exactly three device-node names: `/dev/hdsp/hdsp0master`, `/dev/hdsp/hdsp0events` and `/dev/hdsp/hdsp0r%d` (the ring node, one per ring number); the 64-bit build contains the first two and no `hdsp0r%d` under the same search. The library has no `kdsp` string. Its undefined symbols are `open`, `close`, `ioctl`, `mmap`, `munmap`, `select`, `getenv` and string and memory helpers, and not `read`, `write` or any file-opening call other than `open`. The open routine `alOpenPort` (0xd008dc0, 2428 bytes) loads the master name from the object `_ALhdsp0master` (0xd01d140). Established for the strings and imports, `libaudio.so.1` n32.

What this settles. The library does not choose between `hdsp` and `kdsp`: it opens one fixed path, and whichever driver owns the major number 53 answers it, which agrees with the `kdsp` master file saying "use instead of hdsp" (Established for the path, Inferred for the agreement). The node roles (master, events, one node per ring) match the minor roles read in `kdspopen` above. The library moves sample data through `mmap`, not `read` or `write`, as the driver's missing read and write entries require. It does not itself load the DSP microcode from a file (no `read`, `fopen` or `open` of an image name; the only paths are the three above): the load, if it is done through the driver, would have to be an ioctl on one of those nodes, and which request, or which program, does it remains Unknown. The reset and activate requests were not located in the library.

## Open questions

[open-questions.md](../../questions/README.md), "Hollywood DSP audio driver".
