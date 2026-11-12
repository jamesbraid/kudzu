# DM2 (XT-DIGVID) kernel driver: the ioctl contract

Status: **draft, not independently reviewed.** Part of the DM2 scoping datasheet
(chips docs/xio-dmediapro-dm2.md (chips repository, docs/xio-dmediapro-dm2.md)); the host
software overview is [dmedia-video-drivers.md](dmedia-video-drivers.md). This page maps every command number that
`xtdigvid_ioctl` accepts to its argument block, its effect on the board and its return codes, and cross-checks each command against the call sites in
`libdigvid.so`. Grades: **E** established (read in disassembly or the object's own tables), **I** inferred, **U** unknown, each with what would settle it.
Names in the "Name" column come from strings in the objects where one is given and are otherwise my own labels. Field names and offsets are my own.
The Ghidra export was used only to find the function boundaries; every fact below was read from `objdump -d` output.

## Sources

| Tag | Object (DM2 1.1.3ml root) | sha256 |
|---|---|---|
| K30 | `usr/cpu/sysgen/IP30boot/xtdigvid.o` | `952c3d9ecca0864360619236b807e38c58f5025a90e85745b956ee7a275d772b` |
| K27 | `usr/cpu/sysgen/IP27boot/xtdigvid.o` | `138ab5488ed6263b14582c61cf324a79929459ba62f29f92113fed36630f38ae` |
| K35 | `usr/cpu/sysgen/IP35boot/xtdigvid.o` | `64c024a59b6b1dd7962af6d7857e83ff93ef29e498816b91c524a4d00fab1976` |
| L | `usr/lib32/mlmodule/libdigvid.so` (n32, dynsym only) | `aacf8d4a64dcd34b523c4b2b7bc66153104b710171d42f7e65a588bcb95bc2ca` |

K addresses are `.text` offsets in the relocatable object. L addresses are virtual addresses. `xtdigvid_ioctl` is K30 0xac88 (23,240 bytes, 0x5ac8),
K27 0xb174 and K35 0xab80 (23,316 bytes, 0x5b14). `xtdigvid_eval_ioctl` (the second-level dispatcher, 448 bytes) is K30 0x10750, K27 0x10c3c, K35 0x10694.
The jump table is in `.rodata` of each object: K30 0x1a38, K27 0x1ca8, K35 0x19f8, 32 eight-byte entries (one per command number 1 to 32).

## 1. Calling convention (E)

- **Entry.** `xtdigvid_ioctl(dev, cmd, arg, mode, cred, rvalp)` in the usual SVR4 shape: `a0` is the device vertex, `a1` the command, `a2` the user pointer, `a5` the `rvalp`
  (K30 0xac88 to 0xacc4: `s2 = a0`, `s3 = a1`, `a2` spilled to `896(sp)`, `s6 = a5`). The vertex goes through `hwgraph_fastinfo_get`, which yields a 16-byte info record: type word at +0, pointer at +8.
- **Return and `rvalp`.** On most error paths the handler stores the errno at `*rvalp` and also returns it. The success path returns 0 and leaves `*rvalp` untouched (K30 0xad10 and 0xaf3c). Three paths return -1 without touching `*rvalp`
  (a failed `xtdigvid_init_xg` or `xtdigvid_invalidate_mapram` or `xtdigvid_init_gif`, all in the reset command). A library caller therefore sees errno 11, 12, 14, 16, 19 or 22 through `oserror()`, or an unspecific -1.
- **The vertex type.** Type 2 is the board node (created by `xtdigvid_attach`, K30 0x524, with the info pointer set to the board). Type 1 is refused with errno 22 (K30 0xad04 to 0xad10). Type 3 is accepted like type 2 (K30 0xacf8). Any other value hits an assertion
  ("bad xthd vertex type", `../src/hd_ioctl.c` line 153) and then continues. `xtdigvid_open` treats type 1 as the pipe (it takes the pipe's open semaphore with `cpsema`, which is the "pipe already opened" check), so **I**: type 1 is the pipe node and **ioctls go to the board node, not the pipe nodes**. The library agrees: it opens two descriptors per path, the board node (`open` of the path string at path +8, kept at path +1056) for every ioctl and the mmap, and a second node named `<dir>p<N>` (format `%sp%d`, kept at path +1788) that it only `select`s on (L 0x4096f0 to 0x4097e0, 0x40cf0..0x40d034). Who creates a type-3 vertex is **U**; settled by listing `xtdigvid_vertex_create` call sites in K30 and the node list of a running board.
  A null info pointer logs "null info pointer", returns errno 19 (ENODEV).
- **The `'mhwg'` command.** If `cmd == 0x6d687767` the copy-in is skipped and the static routine at K30 text 0xa4c8 (the one that prints "ioconfig()" and "exit ioconfig()") runs; its value is returned and stored at `*rvalp`.
  It is not a `hdv` command. It names the unit: `device_controller_num_get` into board +32, then builds hardware-graph links named `<unit>p<pipe>` to `../pipe0` and `../pipe1` and the `irix_unit` attribute. Returns 22 for an invalid unit number ("unit number %d from ioconfig is invalid"),
  12 if `kern_malloc` fails and -1 if a graph link cannot be added. The `ioconfig` line `12 14 -1 -1 xtdigvid 0 0x6d687767` is the caller.
- **Copy-in.** For every other command exactly 168 bytes are copied from `arg` to the stack (`copyin`, K30 0xade0); failure returns errno 14 (EFAULT). **Word 0 of the 168-byte block must equal `cmd`**; otherwise errno 22 (printed "ioctl cmd mismatch" when the tunable `xtdigvid_debug_level` is 3 or more) (K30 0xae00 to 0xae3c).
  A caller must therefore pass a buffer of at least 168 bytes with the command repeated at offset 0. All argument offsets below are from the start of that block.
- **Dispatch.** `cmd - 0x68647601` below 32 indexes the jump table. Commands 0x02, 0x04, 0x0d, 0x0e, 0x10, 0x11 and 0x15, and everything outside 1 to 32, go to the default arm (K30 0xaeb4), which calls `xtdigvid_eval_ioctl(board, block)`.
  That routine handles only 0x02, 0x04 and 0x0e; any other number returns errno 22. K27 and K35 have the same set of seven default entries (checked from the three jump tables).
- **Copy-out.** A handler that returns data sets a flag and the common tail copies all 168 bytes back (K30 0xb20c); a failed `copyout` returns errno 14. Commands that return data are marked "out" below.
- **Pipe numbering.** Pipe index 0 is the output pipe (host to board) and 1 the input pipe, as in dma.md (chips repository, docs/dm2/dma.md).
  A pipe index of 2 or more is refused with errno 22 and a console line "xtdigvid_ioctl(%d): bad pipe %d specified" in every handler that has a pipe field except the two noted (0x16, 0x20).
- **Board state used.** `board` is the 0x14860-byte record (magic words at +0 and +0x14858). Offsets used by the handlers: +4 widget vertex, +24 PIO window base (16 MiB), +32 unit number (-1 until ioconfig),
  +0x50 + `pipe` * 0xa368 the two pipe records (the second argument of `xtdigvid_dma` is a pipe record), +0x14720 clink-ack wait variable, +0x14728 clink-ack lock, +0x14738 clink-ack-outstanding halfword, +0x14740 pid-table semaphore,
  +0x14750 to +0x1475c four pid words, +0x14760 and +0x14764 two resync words, +0x14768 jack-block semaphore, +0x14778 four 56-byte jack blocks (ending exactly at +0x14858).
  All E from the handlers below.

## 2. Command table

`W` is the pipe index unless stated. "out" means the 168-byte block is copied back. Return codes are errno values; 0 is success. The column "Lib" lists the exported library functions that issue it (section 6).

| Cmd | Name | Block (offsets from block start) | Effect on the board and driver state | Returns |
|---|---|---|---|---|
| 0x01 | DIAG_READ_WORD (L string `XTHD_IOCTL_DIAG_READ_WORD`) | +8 u64 byte offset in; +16 u32 value, out | `lw` of window +offset, aligned, below 0xc00000 | 0; 12 if offset >= 0xc00000; 22 if not a multiple of 4 |
| 0x02 | DIAG_WRITE_WORD (L `XTHD_IOCTL_DIAG_WRITE_WORD`) | +8 u64 offset; +16 u32 value | `sw` of value to window +offset, then `sync` | as 0x01 |
| 0x03 | START_DMA (K "start dma") | +8 u64 FIFO handle, must be nonzero; +24 u32 pipe; +32 u64 and +40 u64 and other words passed through to `xtdigvid_dma` | Sets the pipe's in-call word (pipe record +2088 = 1), calls `xtdigvid_dma(pipe record, block+8)`, clears it. **Blocks until the stream ends.** | the value returned by `xtdigvid_dma` (section 4.1) |
| 0x04 | RASTER_PARAMS (L `XTHD_IOCTL_RASTER_PARAMS`) | +8 u32 pipe; +12 u32 field_size; +16 u16 interleaved; +18 u16 (unnamed); +20 u16 line_order; +22 u16 line_length; +24 u16 field_length1; +26 u16 field_length2; +28 u16 (unnamed); +30 u16 progressive; +32 u16 msc_incr; +34 u16 (unnamed) | Copies 28 bytes from +8 into the pipe record and calls `xtdigvid_configure_raster(pipe record)`, which validates them and turns them into start-address and control words | 0; 22 if pipe >= 2; otherwise the status of `xtdigvid_configure_raster` (U which codes) |
| 0x05 | GET_EVENT (L "ioctl GET_EVENT") | +8 u32 pipe; +16 .. +167 the 152-byte event record, out | Pops one record from the pipe's 256-entry event ring and clears its valid word | 0; 11 (EAGAIN) if the ring is empty; 22 |
| 0x06 | SET_EVENT_MASK (K "set event mask", L `XTHD_IOCTL_SET_EVENT_MASK`) | +8 u32 pipe; +12 u32 mask | Stores the mask at pipe record +2896 (the interrupt code ANDs it with each posted event) and copies mask bit 8 into bit 16 of the DMA control shadow at pipe record +2568. No register write | 0; 22 |
| 0x07 | FLUSH_EVENTS (L `FLUSH_EVENTS`) | +8 u32 pipe | Under the pipe's event lock, zeroes the 0x9800-byte ring and both ring indexes | 0; 22 |
| 0x08 | (no name; unused by L) | +8 u32 value, out | Returns the constant 113 (0x71) | 0 |
| 0x09 | SET_DM_INDEX (K "set_dm_index", L `XTHD_IOCTL_SET_DM_INDEX`) | +8 u32 new dm_index | Finds the inventory entry (class 12, type 14, controller = board +32) and rewrites its unit field with `replace_in_inventory`; sets the hardware-graph attribute `dm_index` on the device vertex to the decimal string | 0; 22 if board +32 is -1 or no entry; 12 if `kern_malloc` fails |
| 0x0a | GET_UNITS (K "get_units") | +8 u32 controller number out; +12 u32 inventory unit (dm_index) out | Returns the board's controller number (board +32) and the unit field of its inventory entry (`find_inventory(0, 12, 14, controller, -1, -1)`) | 0; 22 if board +32 is -1 or no entry |
| 0x0b | RESET (K "ioctl_reset") | +8 u32 flags (section 4.2) | Per flags: clears pipe state, resets the XG widget and its map RAM, re-initialises the GIF | 0; -1 if XG init, map RAM invalidate or GIF init fails |
| 0x0c | SDMA_CONTROL (K "gif_sdma_control pipe %d: %s") | +8 u32 pipe; +12 u32 on/off | Sets or clears bit 15 (0x8000) of the pipe's DMA control shadow and writes the whole shadow to the pipe's DMA control word, window +0x800014 (pipe 0) or +0x80003c (pipe 1) | 0; 22 |
| 0x0d | none | | default arm | 22 |
| 0x0e | GIF_BYTE_WRITE (eval) | +8 u8 byte | Eight rounds of three stores of `byte | 0x200` then three of `byte | 0x300` to window +0x120000, each followed by `sync`. No read. Not used by L (fpga-load.md (chips repository, docs/dm2/fpga-load.md) section 6 has the same reading) | 0 |
| 0x0f | GET_NIC (L `XTDIGVID_IOCTL_GET_NIC`) | +8 u64 user buffer pointer; +16 u64 buffer size in, bytes copied out | Copies the board's NIC text (`nic_vertex_info_get` on the widget vertex; the string "NOT AVAILABLE" if none) to the user buffer, `min(size, strlen+1)` bytes | 0; 14 if the buffer copy fails |
| 0x10 | none | | default arm | 22 |
| 0x11 | none | | default arm | 22 |
| 0x12 | SET_MCS_INFO (L `SET_MCS_INFO`) | +8 u32 pipe; +12 u16; +14 u16; +16 u32; +20 u32 | Stores the 12 bytes at +12 at pipe record +2664 (board +2744 + pipe*0xa368). **I:** a record of which packer or unpacker image is loaded for that pipe; the library reads it back with 0x13, compares two words with its own path fields and loads `packer_%s.xla` or `unpacker_%s.xla` when they differ (L 0x40a3f8 to 0x40a590) | 0; 22 |
| 0x13 | GET_MCS_INFO (L `GET_MCS_INFO`) | as 0x12, out | Returns that record | 0; 22 |
| 0x14 | VBOB_WRITE (K "vbob_cmd") | +8 u32 pipe; +12 .. +35 the 24-byte message | Locks the pipe's clink-ack lock, writes twelve halfwords to window +0x803804 + 8k, rings window +0x8038ec, sleeps for the acknowledge (section 4.3) | 0; 11 if the lock is taken or the acknowledge times out; 22 |
| 0x15 | none | | default arm | 22 |
| 0x16 | SET_XFERPID_INFO (L `SET_XFERPID_INFO`) | +8 u32 pipe (1 selects pipe 1, anything else pipe 0); +12 u32 slot (1 or 0); +16, +20 u32 pid for pipe 1 slots 1 and 0; +24, +28 u32 pid for pipe 0 slots 1 and 0 | Claims or releases one of four pid words (section 4.4) | 0; 16 (EBUSY) if a claim collides |
| 0x17 | GET_XFERPID_INFO (mine; L `xhdIsPathBusy`, `xhdIsPipeBusy`) | +8 .. +20 four u32 pids, out | Under the pid-table semaphore, returns board +0x14750 .. +0x1475c | 0 |
| 0x18 | GET_JACK_CONTROLS (L `getJackControls`) | +8 u32 jack index 0..3; +16 .. +71 56-byte block, out | Reads jack block `jack` (board +0x14778 + 56*jack) under the jack semaphore | 0; 22 if jack >= 4 |
| 0x19 | SET_JACK_CONTROLS (L `SET_JACK_CONTROLS`) | +8 u32 jack; +16 .. +71 56-byte block | Writes that jack block; no register access | 0; 22 |
| 0x1a | SET_RESYNC_VALUE (L `SET_RESYNC_VALUE`) | +8 u32 low word; +12 u32 high word | Stores both at board +0x14760, +0x14764 | 0 |
| 0x1b | GET_RESYNC_VALUE (L `GET_RESYNC_VALUE`) | +8, +12 out | Returns those two words | 0 |
| 0x1c | SET_VIDEO_TIMING (L "can't ioctl SET_VIDEO_TIMING" after its `xhdInit` calls) | +8 u32 pipe; +12 u32 value | Stores the value at pipe record +2580 (board +2660 + pipe*0xa368). No register write | 0; 22 |
| 0x1d | GET_VIDEO_TIMING (mine) | +8 u32 pipe; +12 u32 value, out | Returns that word | 0; 22 |
| 0x1e | SET_CUSTOM_FORMAT_DEFS (L string) | +8 u32 pipe; +12 .. +23 12 bytes | Stores 12 bytes at pipe record +2584 (board +2664 + pipe*0xa368) | 0; 22 |
| 0x1f | GET_CUSTOM_FORMAT_DEFS (L string) | as 0x1e, out | Returns those 12 bytes at +12 | 0; 22 |
| 0x20 | LOG_WRITE (mine; L `xhdCreateDriverLog`) | +8 u32 pipe label; +12 u32 type 1, 2 or 3; +16 u32 number; +20 u32 length; +24 u32 user pointer | Adds an entry to the driver's event log (section 4.5) | 0; 22 on an unknown type (U which of the paths returns it) |
| outside 1 to 32 | | | default arm | 22 |

Pipe record offsets above are from the start of the record at board +0x50 + pipe*0xa368. The code addresses most of them as board +N + pipe*0xa368 with N 80 larger; both forms are given where they matter.

## 3. Argument details that the table abbreviates

### 3.1 Read and write word (0x01, 0x02) (E)

K30 0xaf44 (read) and K30 `xtdigvid_eval_ioctl` 0x1081c to 0x10868 (write). The offset is a 64-bit big-endian value at +8. Offsets of 0xc00000 or more return errno 12 (not 22), a misaligned offset returns 22.
The window is the 16 MiB mapping at board +24, so offsets 0xc00000 to 0xffffff are the part of the window the ioctl refuses. L `read_word` (0x4163e0) and `write_word` (0x416470) are the only callers' primitives;
`xhdReadWord`, `xhdDebugReadWord`, `xhdWriteWord`, `xhdDebugWriteWord` and `xhdSendPIO` build on them. `xhdSendPIO` (L 0x4087e0) calls 0x02 thirteen times.

### 3.2 Raster parameters (0x04) (E for offsets and names, I for the unnamed fields)

K30 `xtdigvid_eval_ioctl` 0x10848 to 0x108f8. The 28 bytes at block +8 are copied to pipe record +2632 (board +2712 + pipe*0xa368), so block +8 is also the pipe index stored in the record.
The field names are the ones the driver's own debug dump prints (`raster_params: interleaved %d, line_order %d, line_length %d`, `field_length1 %d, field_length2 %d, field_size %d`, `progressive %d, msc_incr %d`; K30 0x3e3b4 to 0x3e4a8);
the dump's board-relative offsets 2716, 2720, 2724, 2726, 2728, 2730, 2734 and 2736 are block +12, +16, +20, +22, +24, +26, +30 and +32. `xtdigvid_configure_raster` (K30 0x72cc, 0x2d28 bytes) also reads the halfwords at block +18, +28 and +34, which the dump never prints.
The library's builder (L `xhdCommitPathControls`, 0x40b9d8 to 0x40bab8) fills the block from the path structure: +12 is a product of two path fields scaled by a float, +22 is `(bytes + 3) >> 2`, +20 is `field != 0`, and +24 and +26 are two path fields. The units of the validated values are the subject of the DMA part.

### 3.3 The event record (0x05, 0x06, 0x07) (E for sizes, I for fields)

The ring holds 256 entries of 152 bytes at pipe record +2904, a read index at +2888 and a write index at +2892, a spin lock at +2900 (K30 0xcfe0 to 0xd0d8, `xtdigvid_post_event` 0x10d50 to 0x10dac).
An entry is valid when the word at entry +16 is nonzero; dequeue clears it and advances the read index modulo 256. The reply block is the 152 bytes from block +16 to +167, which fills the 168-byte block exactly.
Entry +0 is a 64-bit value copied from pipe record +2712 (I: the last media stream counter), entry +8 a 64-bit nanosecond time from `ust_to_nano`, entry +16 the event code, the posting routine's second argument, an event bit that is ANDed with the mask in 0x06.
The rest of the entry is event specific and written by `xtdigvid_post_event` (K30 0x10910, 0x3964 bytes), not read here (U; settled by reading that routine). `xtdigvid_poll` (K30 0x14274) reports readiness for the library's poll.
The event names the driver prints are TRANSFER_ERROR(BUFFERS_ABORTED), CLINK_DATA_RCVD, VBOB_CLINK_ACK, SYNC_GAINED, SYNC_LOST, XFER_FAILED, SEQUENCE_LOST, TRANSFER_COMPLETE(ust/msc), STREAM_STOP and STREAM_START (K30 .rodata 0x1cd8 to 0x1ea8).
The library's masks: open sets 0x200 (L 0x4099b8 to 0x4099cc) and a transfer sets 0xfffffeff, all but bit 8 (L 0x40c028 to 0x40c03c); the library's reader of events is the routine at L 0x40cec0 (no symbol), which `select`s on the pipe descriptor at path +1788 and issues 0x05 on the board descriptor at L 0x40d060 (**I** for the select, from the fd-set construction at L 0x40cffc to 0x40d02c).

## 4. Handlers that do more than store or load a word

### 4.1 START_DMA (0x03) (E for the wrapper, body not read here)

K30 0xd0dc to 0xd650. After the pipe check the handler logs "in/out: start dma", sets pipe record +2088 to 1, calls `xtdigvid_dma` (K30 0x15878, 97,484 bytes) with the pipe record and `block + 8`, clears +2088, logs "end dma" and returns the callee's value.
`xtdigvid_dma` reads from its second argument: +0 a 64-bit FIFO value (zero is refused: "dma missing fifo argument", errno 22), +16 a word (block +24, the pipe), +24 and +32 64-bit values (block +32 and +40), the last stored at pipe record +2720 and +2752.
The values the callee can return, collected from its error exits (K30 0x15e8c, 0x18790, 0x1c5a8, 0x21c44, 0x26b34, 0x2c140): 4 (EINTR, interrupted semaphore wait), 5 (EIO), 7 (E2BIG, "too-large transfer size"), 12 (ENOMEM), 16 (EBUSY), 22. That the list is complete is **I**; the assignment of each is for the DMA part.
The library issues 0x03 from L `xhdDoTransfer` (0x40bf00): +8 the first word of the fifo structure it keeps at path +1784, +16 -1, +24 the pipe, +28 a path word, +40 100 or 101. **The call blocks for the whole stream**, so the library runs it from a thread.

### 4.2 RESET (0x0b) (E, names I)

K30 0xc268 to 0xc5e8, 0xc72c to 0xc7a8, 0xd650 to 0xd7b8, 0xe2b8 to 0xe44c, 0x101f8 to 0x10228. `flags` at +8. Two bit tests and a third:

1. **bit 0 or bit 2 (mask 5)**: for both pipes clear the word at pipe record +2572 (board +2652 + pipe*0xa368).
2. **bit 0**: log "XTALK reset". If `xtdigvid_verify_gif_loaded` succeeds (window +0x120000 bit 0 set) clear the DMA control shadow (pipe record +2568, board +2648 + pipe*0xa368) and write 0 to both pipes' DMA control words (+0x800014, +0x80003c). In every case then clear the words at board +2180, +2184 and +2648 (each + pipe*0xa368) for both pipes, log "calling XG_RESET",
   call `xwidget_reset` on the widget (board +4) and `xtdigvid_init_xg(board)`, log "invalidate_mapram reset_ioctl" and call `xtdigvid_invalidate_mapram(board, 0x8000, 0)`. A nonzero result from either returns -1.
3. **bit 1 clear**: call `xtdigvid_init_gif(board, flags & 4 != 0)` and return -1 if it fails. Bit 1 set skips this call (K30 0xe428 `beq t9,zero,0x101f8`).

The library issues 3 after the GIF FPGA is loaded (resets the XG, map RAM and clears pipe state, keeps the GIF) and 4 after the last FPGA is loaded (clears the pipe words and re-initialises the GIF; L `xhdInit` 0x4079ec and 0x407ed4).
This settles two items in fpga-load.md (chips repository, docs/dm2/fpga-load.md) section 6: the argument-4 reset reaches `xtdigvid_init_gif` with the re-initialise flag set (it was **I**, now **E**), and an argument with bit 1 set returns 0 without reading or writing the GIF registers (it was "not read").
The library's `xhdOpenPath` never issues 0x0b.

### 4.3 VBOB_WRITE (0x14) (E)

Fully described in vbob.md (chips repository, docs/dm2/vbob.md) section 1; this page agrees with it on the block (+8 pipe, +12 .. +35 message), the TX registers (twelve halfwords at window +0x803804 + 8k, big-endian pairs of block bytes), the doorbell (+0x8038ec = 1), the 250-tick timer,
the lock failure (11) and the timeout path (zero the status page, zero window +0x800074 for pipe 0 or +0x800064 for pipe 1, return 11). The pipe check (22 for 2 or more) comes before the lock.
The state is board +0x14728 (lock), +0x14738 (outstanding halfword), +0x14720 (sleep variable), +0x1471c (timer id, cancelled by `untimeout` in the success path and by 0x16).

### 4.4 SET_XFERPID_INFO (0x16) and GET_XFERPID_INFO (0x17) (E for the table, I for the purpose)

K30 0xb464 to 0xb528 and 0xe6e0 to 0xe730, 0xfe44. The handler holds the semaphore at board +0x14740 (`psema`) and edits four pid words:

| Word | Address | Selected by |
|---|---|---|
| A | board +0x14750 | pipe 1 (+8 == 1), +12 == 1, value block +16 |
| B | board +0x14754 | pipe 1, +12 == 0, value block +20 |
| C | board +0x14758 | any other +8, +12 == 1, value block +24 |
| D | board +0x1475c | any other +8, +12 == 0, value block +28 |

A value of -1 releases (stores -1, no collision check). Any other value claims: it fails with errno 16 (EBUSY) unless **both** words of that pipe (A and B, or C and D) read -1, then stores the pid. A release of pipe 1 first cancels the clink-ack timer if one is armed (K30 0xfe44 to 0xfe6c).
The semaphore is released on every path (`vsema`). 0x17 returns the four words in block +8, +12, +16, +20 in the order A, B, C, D.
The library maps each of six path types to one of the four fields (L `xhdSetPids` 0x40d660 to 0x40d728; types 3 and 5 write +16, 0 and 2 write +20, 4 writes +24, 1 writes +28) and `xhdIsPathBusy` and `xhdIsPipeBusy` read them back; so a pid word is "the process holding that path".
Why a pipe's slots exclude each other (one transfer direction per pipe at a time) is **I**; settled by the ML layer's path semantics.

### 4.5 LOG_WRITE (0x20) (E for the shape, I for the effect)

K30 0xe4d4 to 0xe6e0 (type 1) and 0xf69c to 0xf890, 0xf890 to 0xfb30 (types 2 and 3). Type 1 adds a log entry with code 0x303 carrying the number at +16. Types 2 and 3 allocate `+20` bytes, `copyin` that many bytes from the user pointer at +24 and add an entry with code 0x30b or 0x313
(only when the driver's event log is enabled, `XTDV_log` nonzero). Any other type logs 0x317 "unknown log type request". The library issues it from `xhdCreateDriverLog` (L 0x40d8c0) with the pipe label from path byte +1044, a type, a number and the length of a string it formats on the stack.
The tunable `xtdigvid_enable_event_log` controls `XTDV_log`.
What the types mean beyond "number" and "string" is **U**; settled by the `xtdvlog` and `xtdvsdump` debugger commands' formatting code (K30 near 0x3f018).

## 5. Fields the library reads back or sets, grouped by purpose

| Purpose | Commands | Board state |
|---|---|---|
| Raw register access for FPGA programming, LUT download and diagnostics | 0x01, 0x02 | window +0 .. +0xbfffff |
| Bring-up | 0x0b, 0x0e (unused), 0x0f | XG, map RAM, GIF, NIC |
| Per-pipe configuration | 0x04, 0x06, 0x07, 0x0c, 0x12, 0x13, 0x1c, 0x1d, 0x1e, 0x1f | pipe records |
| Transfers | 0x03, 0x05 | `xtdigvid_dma`, event ring |
| Ownership | 0x16, 0x17 | four pid words |
| Jack and timing controls | 0x18, 0x19, 0x1a, 0x1b | jack blocks, resync words |
| Identity | 0x09, 0x0a, 0x0f | inventory entry, `dm_index` attribute, NIC text |
| VBOB messages | 0x14 | TX registers |
| Debug | 0x20, 0x08 | event log |

## 6. Cross-check against `libdigvid.so`

The scan looked for every `ori rX,rY,0x76NN` that completes `0x68647600 | NN` (the pair `lui 0x6864` then `ori`), 80 sites. L exports 76 text symbols; local routines have none, so a site inside one is attributed to the exported symbol that precedes it (marked "after"). The library issues **24 distinct commands**:

| Cmd | Library sites (address, routine) |
|---|---|
| 0x01 | 0x40dc14 `xhdReadWord`, 0x40dcac `xhdDebugReadWord`, 0x416414 `read_word` |
| 0x02 | `xhdSendPIO` x13 (0x4087f0 to 0x408b14), 0x40da24 `xhdWriteWord`, 0x40db2c `xhdDebugWriteWord`, 0x4164a4 `write_word` |
| 0x03 | 0x40bf8c and 0x40c0a0 `xhdDoTransfer` |
| 0x04 | 0x40b9f0 and 0x40ba98 `xhdCommitPathControls` |
| 0x05 | 0x40d060 in the unnamed event routine at 0x40cec0 (after `getSyncErrorStatus`) |
| 0x06 | 0x4099b8 `xhdOpenPath`, 0x40c028, 0x40c244, 0x40c3c0, 0x40c434, 0x40c578, 0x40c5a4 `xhdDoTransfer` |
| 0x07 | 0x40998c `xhdOpenPath`, 0x40a1c4 `xhdCommitPathControls`, 0x40c064 `xhdDoTransfer` |
| 0x09 | 0x41122c `ddInterrogate` |
| 0x0b | 0x4079ec and 0x407ed4 `xhdInit` |
| 0x0f | 0x4078e4 `xhdInit`, 0x409874 `xhdOpenPath` |
| 0x12 | 0x40807c, 0x4080b8 `xhdInit`; 0x40b6d4 `xhdCommitPathControls` |
| 0x13 | 0x40a368, 0x40a4cc `xhdCommitPathControls` |
| 0x14 | 0x410628, 0x410714, 0x410734 `xhdVbobWriteMessage` |
| 0x16 | `xhdInit` x4, `xhdOpenPath` x5, 0x40d6bc `xhdSetPids` |
| 0x17 | 0x40d73c `xhdIsPathBusy`, 0x40d81c `xhdIsPipeBusy` |
| 0x18 | 0x4101ec `xhdSetVbobOutputDefault`, 0x412460 `getJackControls` |
| 0x19 | `xhdInit` x4, 0x412538 `setJackControls` |
| 0x1a | 0x4081fc `xhdInit` |
| 0x1b | 0x409758 `xhdOpenPath` |
| 0x1c | 0x40823c, 0x408358 `xhdInit`; 0x40a154 `xhdCommitPathControls` |
| 0x1d | 0x4095b8 `xhdProgramHdocResyncRegs`, 0x40a0b8 `xhdCommitPathControls` |
| 0x1e | 0x40eafc, 0x40eb34 `xhdVerifyPathConfig` |
| 0x1f | 0x40f0cc `xhdGetCustFormatDefs` |
| 0x20 | 0x40d95c, 0x40d9a4 `xhdCreateDriverLog` |

The library never issues 0x08, 0x0a, 0x0c, 0x0e, nor the five numbers with no handler. The error texts at the library's failure branches (`SET_MCS_INFO failed for pipe %d`, `SET_XFERPID_INFO failed`, `SET_RESYNC_VALUE failed`, `SET_VIDEO_TIMING`, `SET_JACK_CONTROLS failed`, `FLUSH_EVENTS`,
`GET_EVENT`, `SET_DM_INDEX failed`, `GET_NIC failed`) agree with the roles assigned above, but the names are the library's, not necessarily the kernel's. The strings carry "XTHD" prefixes in three of them (`XTHD_IOCTL_SET_EVENT_MASK`, `RASTER_PARAMS`, `DM_INDEX`), so the kernel interface is called XTHD ("xt hd").

Argument shapes seen at library call sites and agreeing with the kernel reads:

- 0x01 and 0x02: `sd offset,8(sp)` and the result at `16(sp)` (L 0x416400 to 0x41645c).
- 0x0b: `+8` is the flag word, values 3 and 4 only.
- 0x0f: `+8` the buffer pointer (zero-extended to 64 bits), `+16` the size 1024 (L 0x40985c, 0x409870).
- 0x14: `+8` pipe, `+12` message of 24 bytes (L 0x4105f0).
- 0x16: the types-to-field mapping above; every call sends -1 in all four pid fields except the one in use, so a release is "-1 in the field".
- 0x19: `xhdInit` initialises four jack blocks, setting words at block +24 (0), +48 (2, 3 or 10), +52 (3) and +64 (2) and zero elsewhere. The meaning of the 56 bytes is **U**; settled by the library's `ddGetControls` handler for the jack controls (L 0x413fc0).
- 0x1a: the two words are the low 12 bits and bits 12 to 22 of the register read at window +0x2060 (`xhdReadWord(0x2060)` at L 0x40816c to 0x408214), so the board's HDOC resync value is a 23-bit quantity split across two words (I).
- 0x1f: the kernel copies 12 bytes but the library, after the call, reads twelve 32-bit words starting at block +12 (L 0x40f03c to 0x40f094, matching `XTDIGVID_CUSTOM_FORMAT_DEFS_SIZE 12` in `ml_xtdigvid.h`). Either the kernel's count is bytes for the first three entries and the library trusts stale stack contents for the other nine,
  or the copies are narrower than the interface. This is a **discrepancy** (E for both readings, I for which one is the contract); settled by running a custom-format control against a board and comparing the returned block, or by reading how the library fills the 12 words for 0x1e.

## 7. The other two platform builds

- **K27** (`IP27boot`): the instruction stream of `xtdigvid_ioctl` and of `xtdigvid_eval_ioctl` is identical to K30's in length and in every opcode, differing in 48 instructions, all of them `lh` (40) or `daddiu` (8) with a different address immediate (the `.rodata` base, 5088 versus 5712, and the log-flag halfword, -15456 versus -15328).
  The jump table has the same 32 targets at the same relative offsets (+0x2bc, 0x22c, 0x2454 and so on). **E** that the command set, argument layouts and return codes are the same.
- **K35** (`IP35boot`): 5,789 instructions against 5,770, the same dispatch prelude (the 168-byte copy-in at 0x158, the `'mhwg'` test at 0x134, the 32-entry test at 0x20c) and the same seven default entries. The handler bodies sit at different offsets and the register allocation differs (429 differing instruction runs by difflib after masking addresses; the ones I looked at are register renames).
  The handler bodies of K35 were **not** compared. **I** that the contract is the same, because `xtdigvid_eval_ioctl` is identical in the three objects (112 instructions, same hash of the instruction words) and the library is one build for all platforms; settled by comparing the K35 handlers' constants (window offsets, pipe record offsets, errno values) one handler at a time.

## 8. Unknowns, and what would settle each

| # | Unknown | Grade | What settles it |
|---|---|---|---|
| 1 | Which vertex is type 3 and what it is used for | U | list every `xtdigvid_vertex_create` call and node in a running board's `/hw/video/xt-digvid` |
| 2 | Meaning of the three unnamed halfwords of the raster block (+18, +28, +34) and the units of the validated lengths | U | `xtdigvid_configure_raster` (K30 0x72cc), which reads them; the DMA part |
| 3 | The event record beyond +16 (payload per event) | U | read `xtdigvid_post_event` (K30 0x10910, 0x3964 bytes) event by event |
| 4 | What 0x08's constant 113 means | U | no caller; possibly an interface version. No reader exists in the shipped set |
| 5 | The 56-byte jack block layout | U | `ddGetControls` and `ddSendControls` (L 0x413fc0, 0x414e80) |
| 6 | The pipe record words at +2572, +2580 (0x1c), +2584 (0x1e), +2744 (MCS info) and what the interrupt or DMA code does with them | U | the DMA part and the status-ring part read the users of those words |
| 7 | Which errno `xtdigvid_configure_raster` returns for each bad field | U | read it; its messages "start_addr1 out of range" and "start_addr2 out of range" locate the exits |
| 8 | The 0x1f block width (12 bytes against twelve words) | U | see section 6 |
| 9 | Whether K35's handlers equal K30's | I | compare them (section 7) |
| 10 | What bit 15 of the DMA control word ("sdma control", 0x0c) switches in the GIF | U | the GIF register semantics; the DMA part names the other bits |
| 11 | The effect of the mask bit 8 to control bit 16 copy (0x06) | U | the DMA control word's bit 16 is not described anywhere; settled by hardware or the FPGA |
| 12 | Whether 0x0e is reachable from the shipped tools | I | `xtdigvidtst` and `usr/diags` binaries reference a subset of the commands (see below); a negative needs the other diagnostics tools read |

The diagnostics binary `usr/diags/ml_xtdigvid/bin/xtdigvidtst` (sha256 `7c96c9a69f7d2378c5a0fcbbaf55fe4bfcb8b10f429ce1900a7a09f558b01234`) carries `ori` immediates for 0x01 to 0x07, 0x0b and 0x0f, and `vbob_reset`, `vbob_config` and `vbob_flash` carry 0x01, 0x02, 0x05, 0x06, 0x07, 0x0b and 0x14 (a grep of each disassembly for `ori ...,0x76NN`; not read further, so whether each is an ioctl number is **I**). None carries 0x0e, 0x08, 0x0a or 0x0c. Command 0x0e is therefore unused by the shipped programs I scanned (the other tools in `usr/diags` and `usr/dmedia/bin/vbob` were scanned; `mldaemon` and `libML.so.1` carry no `0x6864` immediates).
