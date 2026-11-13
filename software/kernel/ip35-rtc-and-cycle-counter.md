# IP35: how the kernel reads the real-time counter's period and exposes the cycle counter

Scope. IRIX 6.5.30 `unix.IP35` (linked kernel, symbols present). Addresses are kernel virtual addresses. The 6.5.22 kernel was not
read. Grades: established (routine read in full), inferred (reasoned reading), unknown (with the read that would settle it).

## Established

- `rtc_resolution` (0xc0000000000680e8, 36 bytes) loads the doubleword at the local-block register +0x604200 (the pointer
  0x9200000001604200 comes from the global word at gp + 0x8e0), takes bits 13:4 (call it F), and returns `10 * (F + 1)`: the tick period in
  nanoseconds. That is `(2F + 2)` periods of a 5 ns clock, a 200 MHz clock built into the kernel.
- `startrtclock` (0xc0000000000677b0, 152 bytes) computes the same `10 * (F + 1)` inline (0xc0000000000677ec to 0xc000000000067810),
  divides 1,000,000,000 by it (0xc000000000067814, the `ddiv` result is the tick rate in hertz), and stores ten times
  the rate as the counter-interrupt warning interval in cycles (`counter_intr_warn_cycles`, 0xc00000000163b8c8). It then calls `setrtvector` (0xc000000000060738) once.
- `query_cyclecntr` (0xc000000000067c24, 36 bytes) is the kernel side of the cycle-counter query the system-call selector table names
  `SGI_QUERY_CYCLECNTR`: it returns the fixed address 0x9200000001030100 (the processor interface's real-time counter, local form,
  taken from the word at gp + 0x8d8) and stores `timer_unit * 1000` (picoseconds per count; `timer_unit` is at 0xc000000001644c98,
  the multiply is `((v << 5) - v) << 2` plus `v`, then `<< 3`) at the caller's pointer. `query_cyclecntr_size` (0xc000000000067c6c) is
  the companion that gives the width. A user process therefore reads the counter that the PROM programs, by an uncached load of
  the processor interface register mapped into its address space.
- `hub_rtc_init` (0xc00000000006a03c, 200 bytes) stores 1, 1, 0 and 0 to the node-window registers at +0x140, +0x148, +0x150 and +0x158 of
  the processor interface of the calling node (the real-time interrupt enables and the profiling enables, by position in the register
  map): it never writes local-block +0x4200. The kernel only reads F; the period is the PROM's to set.

## Established agreement with the PROM

The IP35 PROM writes bits 13:4 of the same register from two flash configuration words, `F = (hub clock / RTC rate) / 2 - 1`
(chips `docs/questions/ip35-prom-poll-timing.md`). In both shipped PROM payloads the words are 200,000,000 and 1,250,000, so F = 79,
the period is 800 ns and the counter rate is 1.25 MHz. The PROM's microsecond conversion is `ticks * (2F + 2) / 200`: the same
arithmetic as this kernel's `10 * (F + 1)` nanoseconds. Grade: the arithmetic agreement is established; that the hardware divides its
clock as both readers assume is inferred.

## Unknown

- What a running Bedrock reads at local-block +0x604200 (prediction from the PROM's write: low 16 bits 0x04f1, bit 32 set). Route: one
  64-bit read on a running IP35 system.
- The latency of a user-level load of the counter (an uncached load to a processor interface register). Route: a user program that
  maps the counter as the system-call documentation describes and times many reads against the counter itself; read-only, about 10^6 loads
  give microsecond-level resolution against an 800 ns tick.
