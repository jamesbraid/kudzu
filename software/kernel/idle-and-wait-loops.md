# Idle loop and look-alike wait loops in the IRIX kernels

How a CPU with nothing to run waits in the IRIX 6.5 kernels, how it is woken, and which other loops in the kernel look the same from outside. Every address is a linked address in the shipped kernel named in the heading, read from the symbol table and the disassembly. Names are the kernel's own symbol names. Grades follow [AGENTS.md](../../AGENTS.md): **established** means read in full at an address in the binary, **inferred** means a reasoned reading (the reasoning is stated), **unknown** means no binary shows it.

## What this covers and what exists already

Kernels read (SHA-256 prefixes are those in `tools/config/inventory-root.sha256`):

| Build | Image | SHA-256 | Text segment as linked |
|---|---|---|---|
| 6.5.30 IP35 | `unix.IP35` | `412a1aae553a` | `0xc000000000040000`, size 0x4d9ebf (read-execute) |
| 6.5.30 IP27 | `unix.IP27` | `567474674eaa` | `0xc000000000019000`, size 0x3eb64f |
| 6.5.30 IP30 | `unix.IP30` | `607a432f441d` | `0xa800000020010000`, size 0x4a037c |
| 6.5.22 IP35 | `unix.IP35` | `87563dbc0cbc` | `0xc000000000040000`, size 0x456af3 |
| 6.5.22 IP27 | `unix.IP27` | `40e3f6246b58` | `0xc000000000019000`, size 0x3ea82b |
| 6.5.22 IP30 | `unix.IP30` | `e913b7022abb` | `0xa800000020004000`, size 0x3df304 |

The IP27 and IP35 text lies in the 64-bit mapped kernel segment (`0xc000...`); the IP30 text lies in the 64-bit cached direct window (`0xa8000000...`, physical `0x20010000` upward). Linked IP32 (O2), IP22 and IP28 kernels are not held in either release, and no 5.3 media is held. For those boards only the unlinked objects of the platform boot directories exist, so their facts below carry `.text` offsets inside `kernel.o`, not linked addresses.

**The per-release address catalogue already exists.** [evidence/inventory/binaries.tsv](../../evidence/inventory/binaries.tsv) lists the six images above and [evidence/inventory/functions.tsv](../../evidence/inventory/functions.tsv) has one row per function with its address, size and code hash, for the linked kernels and for every object of `IP22boot`, `IP27boot`, `IP30boot`, `IP32boot` and `IP35boot`. It already carries rows for `idle`, `idlerunq`, `idler`, `local_idle`, `resumeidle`, `toidlestk`, `sendintr`, `us_delay`, `panicspin` and, from debug information, for the static `wait_for_interrupt` in `machdep.o`; the addresses in this note agree with those rows. Those tables are generated, so no row was added here. The one thing the tables do not show is that the linked `wait_for_interrupt` has no symbol-table entry: its address is the `functions.tsv` row from debug information, and the symbol table folds it into the preceding `clkreld`.

## Short answer

* Every linked kernel idles in C code, not with a power-saving instruction. The idle function `idle` runs on a per-CPU idle stack with interrupts enabled at priority level zero and sits in a small polling loop (`wait_for_interrupt`, 12 instructions, calling `local_idle` and `idler`). No linked IP27, IP30 or IP35 kernel contains a WAIT instruction at all (established by a scan of all executable words of all six images).
* The polled state is ordinary cached memory of the per-CPU data area: the "next thread" pointer, the local run-queue head and the "queued for me" flag. The loop reads no Count, Cause or Status register, issues no CACHE operation and no SYNC.
* A sleeping CPU is woken either by a store to that polled memory by another CPU (no interrupt needed) or by an interrupt, which on return restarts idle through `resumeidle`. A wakeup can therefore arrive with no interrupt.
* The only kernels with a WAIT instruction are the IP22 (6.5.22) and IP32 (both releases) objects, and it is reached only when the processor revision register names one of a short list of implementations.
* Nothing in the kernels checksums or verifies the kernel text at run time that was found (inferred, see section 4).

## 1. The idle loops

### Structure (established on 6.5.30 IP35, same shape in all six images)

Four routines make up idle. In the order they run:

* `idle` (about 119 instructions, 0x1dc bytes on 6.5.30, 0x1d0 on 6.5.22) is entered from `resumeidle`, and from the thread-exit switch routines `utexitswtch` and `istexitswtch`, with no thread on the stack. It returns the next thread to run. Its outer loop is: read the per-CPU next-thread pointer; if empty, call `spl0` (interrupts on, level zero), `flush_console`, `delay_for_intr` (a two-instruction stub that just returns on all six images) and, when a graphics-wait flag is set and a back-off variable is nonzero, `us_delay(1)`; set the per-CPU "resume idle on interrupt" byte to 1; then loop `while (idlerunq()) wait_for_interrupt();`; clear that byte (in the delay slot of the `splhi` call, so the byte is clear before `splhi` runs); call `splhi`; call `getrunq`; if it returned a thread, return it, otherwise go round again.
* `idlerunq` returns nonzero to mean "stay idle". It returns zero at once when `local_idle` says there is work. For a CPU that is not the designated idler it returns 1 straight after `idler()` returns 0. For the designated idler it walks every CPU's per-CPU record (up to the CPU count) and returns zero if any of them has the "queued" flag set, if the real-time global queue is nonempty or if the batch queue is nonempty.
* `wait_for_interrupt` is the polling loop proper: call `local_idle`; if it returns nonzero call `idler`; loop while `idler` returned zero; return when either condition changes. Because the function is static and its debug-information address has no symbol-table entry, a disassembler that goes by the symbol table shows it as the tail of `clkreld`.
* `local_idle` (20 instructions) returns 1 when the CPU has nothing local to do: it clears the CPU's "queued" byte if set, and returns "idle" only if the next-thread pointer is zero and the local run-queue head is zero and (on the builds that have the real-time per-CPU table) that table's entry for this CPU is zero.
* `idler` (about 104 instructions) maintains the cpuset-idler bookkeeping: it returns 0 at once on a CPU marked restricted; otherwise it can claim the global "primary idle" word with a load-linked/store-conditional compare-and-swap (`compare_and_swap_int`) and record the claim in the per-CPU idler field. Its return value is that per-CPU idler field. So on exactly one unrestricted idle CPU at a time (the primary idler) `idler` returns nonzero, `wait_for_interrupt` returns after one pass, and the CPU spins in the larger `idlerunq` scan instead.

Instruction counts per iteration:

* Non-primary CPU, inner loop (`local_idle` plus `idler`, both called each pass): **68 instructions per pass** on 6.5.30 IP35 (including the call and return glue and the delay slots). Inferred: obtained by running the routines' own instructions through a small interpreter with a synthetic per-CPU state (empty queues, one cpuset, another CPU holding the primary-idle word); only the control flow, not the memory contents, comes from the binary. The other five images have the same routines with 104 to 107 instructions in `idler` and 20 in `local_idle`, so their pass is expected to be within a few instructions of 68; this was not measured.
* Primary idler: one pass of `idler` (the same ~35 instructions that return nonzero) plus the `idlerunq` scan, which costs a roughly fixed amount per CPU slot (a `local_idle` call and about 15 more instructions each). Not measured.

Interrupts: **enabled inside the loop.** `idle` calls `spl0` before it, and nothing between that call and the `splhi` after the loop changes the interrupt mask. Established by the call sequence at `idle`.

### What the loops contain (established; searched all of `idle`, `idlerunq`, `idler`, `local_idle`, the polling loop and `resumeidle` on all six images)

* CP0 reads: none of Count, Cause or Status. (`resumeidle` and `toidlestk` read and write Status, once each, to mask interrupts while the stack is switched; they are not in the loop.)
* CACHE operations: none.
* SYNC: one, in `idle`, inside a short `us_delay(1)` retry loop that waits for the thread-being-left to release its stack (`k_sqself` byte at offset 0x72 of the kthread). It is reached only when the CPU has just been handed a thread, not during idle polling.
* WAIT instruction: none in the linked images.
* Uncached loads: none that are visible as such. All the polled words are addressed through the per-CPU data window at the top of the 64-bit address space (negative displacements from `$zero`, the window starting at `0xffffffffffffc000`; for example `0xffffffffffffc438` is the next-thread pointer on 6.5.30 IP35). Whether that window is mapped cached is set by a TLB entry the loop does not show: unknown here, inferred cached because `idlerunq` reads other CPUs' copies of the same words through ordinary pointers.
* Atomic operations: `compare_and_swap_int` (`ll`/`sc`) in `idler` only, on the idler claim, not on every pass.

### Addresses per build

Linked virtual addresses, size in bytes. `loop` is the polling routine (no symbol entry), start to end inclusive of its delay slot.

| Routine | 6.5.30 IP35 | 6.5.30 IP27 | 6.5.30 IP30 |
|---|---|---|---|
| `idle` | 0xc0000000002b0024, 0x1dc | 0xc0000000001ffbf8, 0x1dc | 0xa8000000201863f8, 0x1dc |
| loop (`wait_for_interrupt`) | 0xc0000000002affc0..0xc0000000002affef | 0xc0000000001ffb94..0xc0000000001ffbc3 | 0xa800000020186394..0xa8000000201863c3 |
| `idlerunq` | 0xc00000000023fae0, 0x270 | 0xc000000000191c78, 0x264 | 0xa80000002015a928, 0x1d4 |
| `idler` | 0xc00000000023f7b0, 0x19c | 0xc00000000019195c, 0x19c | 0xa80000002015a738, 0x1a0 |
| `local_idle` | 0xc00000000023f94c, 0x50 | 0xc000000000191af8, 0x50 | 0xa80000002015a8d8, 0x50 |
| `resumeidle` | 0xc00000000004113c, 0x44 | 0xc00000000001a0fc, 0x44 | 0xa8000000200111ac, 0x44 |
| `toidlestk` | 0xc000000000041180, 0x3c | 0xc00000000001a140, 0x3c | 0xa8000000200111f0, 0x3c |
| `getrunq` | 0xc00000000023fde8, 0x134 | 0xc000000000191f74, 0x134 | 0xa80000002015ab94, 0x134 |
| interrupt-return jump to `resumeidle` | 0xc000000000040318 | 0xc000000000019318 | 0xa800000020010318 |

| Routine | 6.5.22 IP35 | 6.5.22 IP27 | 6.5.22 IP30 |
|---|---|---|---|
| `idle` | 0xc000000000271edc, 0x1d0 | 0xc0000000001fb0b4, 0x1d0 | 0xa800000020169ca0, 0x1d0 |
| loop (`wait_for_interrupt`) | 0xc000000000271e78..0xc000000000271ea7 | 0xc0000000001fb050..0xc0000000001fb07f | 0xa800000020169c3c..0xa800000020169c6b |
| `idlerunq` | 0xc00000000020a314, 0x25c | 0xc00000000019508c, 0x254 | 0xa8000000201424fc, 0x1e4 |
| `idler` | 0xc000000000209fd4, 0x1a8 | 0xc000000000194d60, 0x1a8 | 0xa800000020142304, 0x1a8 |
| `local_idle` | 0xc00000000020a17c, 0x50 | 0xc000000000194f08, 0x50 | 0xa8000000201424ac, 0x50 |
| `resumeidle` | 0xc000000000040f0c, 0x44 | 0xc000000000019ecc, 0x44 | 0xa800000020004f7c, 0x44 |
| `toidlestk` | 0xc000000000040f50, 0x3c | 0xc000000000019f10, 0x3c | 0xa800000020004fc0, 0x3c |
| `getrunq` | 0xc00000000020a608, 0x134 | 0xc000000000195378, 0x134 | 0xa800000020142778, 0x134 |
| interrupt-return jump to `resumeidle` | 0xc000000000040318 | 0xc000000000019318 | 0xa800000020004318 |

(The disassembler symbol for the interrupt-return jump is `tfp_special_int_exit+0x168`; it is the tail of the shared interrupt-return code, not that function. Established.)

### Differences between 6.5.22 and 6.5.30 (established)

`idle` is 0x1d0 bytes in 6.5.22 and 0x1dc bytes in 6.5.30. The extra instructions are in the "found a next thread already" check: 6.5.30 additionally tests a flag byte in the per-CPU area (offset `-0x3faf`, bit 0, i.e. a flag word next to the kernel-stack flag) and treats the thread as unrunnable when it is set. `idler`, `local_idle` and `idlerunq` differ only in layout and in the sizes shown above. The polling loop is byte-for-byte the same shape in both releases (12 instructions). The reason for the extra flag test is not visible in the binary: unknown.

### Per-CPU data offsets used (negative displacements from `$zero`; field names are from the type and symbol names present in the images and the usage; offset-to-name mapping is inferred)

| Field | 6.5.30 IP35 | 6.5.30 IP27 | 6.5.30 IP30 |
|---|---|---|---|
| CPU number (halfword) | -0x3bd8 | -0x3be0 | -0x3c60 |
| next-thread pointer | -0x3bc8 | -0x3bd0 | -0x3c50 |
| local run-queue head | -0x3be8 | -0x3bf0 | -0x3c70 |
| "queued for this CPU" byte | -0x3bde | -0x3be6 | -0x3c66 |
| "resume idle on interrupt" byte | -0x3fb0 | -0x3fb0 | -0x3fb0 |
| "reschedule requested" byte (stored by `force_resched`) | -0x3fb2 | -0x3fb2 | -0x3fb2 |
| idler field (halfword) | -0x3fac | -0x3fac | -0x3fac |

The 6.5.22 images use the same offsets as the same platform in 6.5.30.

## Stores in one pass of the polling loop (IP35)

Question: does one pass of `wait_for_interrupt` with its `local_idle` and `idler` calls write anything besides loads? Read from the disassembly of every store instruction in `idler` and `local_idle`, then confirmed by running the loop through a small interpreter with synthetic per-CPU state (empty queues, one cpuset, another CPU holding the primary-idle word, so `idler` returns 0 and the loop repeats). The interpreter's memory contents are invented; the control flow and the store list come from the binary. Grade: established for the store sites; inferred for which of them a real idle CPU reaches, because that depends on live state.

**6.5.30 IP35 steady pass (non-primary idle CPU): four stores, all to the stack, none to shared or per-CPU data.**

| Store | Address | Target |
|---|---|---|
| `sd` | 0xc00000000023f824 (`idler`) | stack slot sp+8, the CPU's cpuset index |
| `sd` | 0xc00000000023f830 (`idler`) | stack slot sp+0, the CPU number |
| `sd` | 0xc00000000023f83c (`idler`) | stack slot sp+0x18, return address |
| `sd` | 0xc00000000023f8b4 (`idler`, in a branch delay slot, so it runs whether or not the branch is taken) | stack slot sp+0x10, the CPU's cpuset record offset |

`wait_for_interrupt` itself stores one more value, the return address at sp+0, in its prologue at 0xc0000000002affc4. That is once per entry to the function, not per pass: the loop branches back to 0xc0000000002affc8, after the prologue. `local_idle` stores nothing in this pass.

Not reached in this pass, but present as stores in the two routines:

* `local_idle` 0xc00000000023f958: `sb zero` to the per-CPU "queued for this CPU" byte (offset -0x3bde). It runs only when that byte was set; the same call then clears it, so the next pass loads zero and skips the store. A one-shot, not a cycle.
* `idler` claim and release paths: byte stores to the "resume idle on interrupt" byte (-0x3fb0) at 0xc00000000023f86c, 0xc00000000023f884, 0xc00000000023f8b8, 0xc00000000023f8e4, 0xc00000000023f908 and 0xc00000000023f938; halfword stores to the idler field (-0x3fac) at 0xc00000000023f87c, 0xc00000000023f8dc and 0xc00000000023f928; a word store of -1 into the cpuset's idler slot at 0xc00000000023f878; and two calls to `compare_and_swap_int` (an `ll`/`sc` pair) at 0xc00000000023f8c8 and 0xc00000000023f918. They run when a CPU gives up or takes the primary-idle or cpuset-idler role (conditions are the compares in `idler`), not on every pass. In the synthetic run none executed.
* No spl change, counter, statistic, lock take or release, or Count read occurs in the loop.

**Fixed point or cycle.** In the steady pass every one of the four stores writes the same value each time, because each is a copy of CPU-number-derived values or the return address. No store depends on a counter, so memory reaches a fixed point after the first pass: period 1. The instruction trace is also periodic with period 1 at 68 instructions per pass (the figure from section 1). The only way the trace changes is when the loaded state changes (a wakeup, or the idler role moving), not by anything the loop writes.

**6.5.22 IP35.** Same result by the same method (loop at 0xc000000000271e78, `idler` 0xc000000000209fd4, `local_idle` 0xc00000000020a17c). The steady pass stores only to the stack: `idler` 0xc000000000209fe4 (s1), 0xc00000000020a004 (s0), 0xc00000000020a024 (CPU number), and the return address at 0xc00000000020a070 or 0xc00000000020a120 depending on the exit taken. The `local_idle` one-shot clear of the queued byte is at 0xc00000000020a188. The claim and release stores are byte stores to the resume-idle byte (-0x3fb0) and to a byte at -0x3a98, which in 6.5.22 holds the idler field (a byte there, a halfword at -0x3fac in 6.5.30), plus two `compare_and_swap_int` calls at 0xc00000000020a0ec and 0xc00000000020a144. None ran in the synthetic pass; all repeated stack values were identical, so it is also a fixed point of period 1. The saved values of the callee-saved registers on the stack are whatever the caller held, and the loop does not change them. The memory offsets used for the synthetic state were read from the disassembly and are not a check against live data.

## 2. How an idle CPU is woken

Three routes, all established unless marked.

**A. A remote store into the polled memory (no interrupt).** The run-queue insert routine `cpu_insert_localq` (0xc00000000024ee98 on 6.5.30 IP35, reached from `putrunq`) stores into the target CPU's per-CPU record directly: it links the thread on the local run-queue head and sets the "queued" byte, between a `nested_spinlock` and `nested_spinunlock` of the target's record; its only calls are those two, so it sends no interrupt (established on 6.5.30 IP35). The target is polling exactly those words in `local_idle` (run-queue head, next-thread pointer, "queued" byte), and the primary idler additionally scans every CPU's "queued" byte in `idlerunq`. So the wakeup is the cache-coherence traffic from that store; on the Hub/Bedrock systems that is ordinary coherent memory traffic. **A wakeup can arrive with no interrupt.** Whether a particular wakeup is delivered this way or with route B depends on whether the new thread outranks the one running (below).

**B. An interprocessor interrupt.** `sendintr(cpu, 0xab)` (0xab is the "do actions" argument) is called from the run-queue code at the preemption tests in the routine that `putrunq` calls (disassembler label `_condRunchecks+0x2f0` and `+0x330` on 6.5.30 IP35; the label is the symbol the table gives for that address, the call sites are in the code reached from `putrunq`), from `evpostRunq`, `tlbsync`, `nested_cpuaction`, `failsoft_avoid`, `mp_mustrun`, `uthread_sched_setscheduler` and `pproc_prnode`. Its body is a single 64-bit store:
* IP35 (`sendintr` 0xc000000000060824): the target CPU's record gives a slice bit and a node number; the 8-byte store goes to a node-window address built from those (inferred to be the Bedrock interrupt-pending register of that slice) and the value is `0x100` ORed with 7 plus the slice number for "do actions", or `0x6b` plus the slice number for any other argument. The register is identified only by address arithmetic here; its name was not checked against [ip35-hub-errors.md](ip35-hub-errors.md).
* IP27 (`sendintr` 0xc000000000039640): same shape, store at doubleword offset 0x90 of the target node's Hub window, value `0x100` ORed with 7 plus the slice number (or `0x6b` plus it).
* IP30 (`sendintr` 0xa800000020013a2c): one store of a single set bit (bit number the target CPU plus 0x2e) to a HEART register at `0xffffffffaff10020` (the 32-bit uncached compatibility window).

In the receiving CPU the interrupt-return code tests the "resume idle on interrupt" byte. Established: the only code in each linked image that reads the byte and acts on it is a short tail at the addresses in the table above (symbol `tfp_special_int_exit+0x140`, on 6.5.30 IP35 the byte load is at 0xc0000000000402f0): it loads the byte, clears it, and jumps to `resumeidle` instead of returning into the interrupted instruction. Inferred: that this tail is the interrupt-return path of the R10000-family boards. The symbol name suggests the R8000 path, but the same code sits at the same offset in the IP27, IP30 and IP35 images, and no other reader of the byte exists apart from the fatal-error path and the profiler; I did not trace the entry code through to it. `resumeidle` masks interrupts, switches to the idle stack (`toidlestk` is the variant that does not reset the thread slot), marks the kernel-stack flag as "on the idle stack", optionally clears the stack-in-use byte of the thread being left, clears the current-thread slots, restores the status register, calls `idle`, and passes its result to `resumenewthread`. So after any interrupt the idle loop is restarted from the top and the thread-pick is retried. Established (both the jump site and `resumeidle` body are read in full).

**C. Timer ticks.** The clock interrupt arrives like any interrupt, so route B's return path applies. Mechanism: `startkgclock` / `ackkgclock` / `startrtclock` / `ackrtclock` program and acknowledge per-CPU clock interrupts. On IP27 and IP35 they store to registers of the local Hub/Bedrock (`ackkgclock` at 0xc00000000006a140 on 6.5.30 IP35 does one 64-bit store of zero to one of two registers, chosen by the CPU's slice bit); the compare value derives from the node's real-time-counter period (a field read at `(*0x8e0($gp)) >> 4 & 0x3ff` times ten plus ten divides 10^9 to give nanoseconds per tick). On IP30 the tick is a HEART timer interrupt (`heartclock_intr` 0xa80000002001823c, `ackkgclock` 0xa800000020011300). That the tick is delivered as an interrupt, not polled by the idle loop, is established for the idle loop (it never reads a clock); that the source is a counter-compare interrupt rather than something else is inferred from the stores in these routines. The R10000 CP0 Count register is read only by `_get_count` and `resetcounter` (and `trimcompare` on IP30), none of which the idle loop calls.

## 3. Other loops that look like idle

None of these is idle; each can be told apart by its address and by what it reads.

| Loop | What it spins on | 6.5.30 IP35 | 6.5.30 IP27 | 6.5.30 IP30 |
|---|---|---|---|---|
| `nested_spinlock` (9 instructions, `ll`/`sc` on bit 0 of the lock word, no back-off) | the lock word | 0xc000000000331fd8 | 0xc000000000286420 | 0xa80000002022d9c0 |
| `nested_bitlock` (9 instructions, `ll`/`sc` on a caller-chosen bit) | the lock word | 0xc0000000003320a4 | 0xc00000000028655c | 0xa80000002022dafc |
| `us_delay` | IP27, IP35: a free-running 64-bit counter register read through a pointer in the global data area (not CP0 Count), deadline computed from a period field; IP30: a counted software loop calibrated by a variable, no counter read | 0xc000000000069a4c | 0xc000000000047c0c | 0xa800000020011640 |
| `drv_usecwait` | calls `us_delay` | 0xc00000000021eec0 | 0xc000000000174358 | 0xa8000000201404e8 |
| `cpu_waitforreset` | IP30: polls bit 0x40 of a word through a pointer in the global data area, calls `cpu_soft_powerdown` when it sets; IP27 and IP35: an 8-byte stub that just returns | 0xc000000000077a78 | 0xc000000000053678 | 0xa800000020018800 |
| `panicspin` | after `clkreld`, `spl7`, a TLB dump and a console flush, loops forever calling `chkdebug`, `doactions` and `us_delay(500)` (interrupts masked) | 0xc0000000002af314 | 0xc0000000001feefc | 0xa8000000201c8bb8 |
| `bte_wait_for_xfer_completion` (block-transfer engine status poll; IP27 only, no symbol of that name in the IP35 and IP30 images) | a Hub register | absent | 0xc00000000002d9e4 | absent |
| stack-release wait inside `idle` (`us_delay(1)` plus `sync`, repeat while the thread-left byte at +0x72 is set; no symbol of its own) | the kthread byte at +0x72 | 0xc0000000002b00b4..0xc0000000002b00c8 | 0xc0000000001ffc88..0xc0000000001ffc9c | 0xa800000020186488..0xa80000002018649c |

6.5.22 counterparts: `nested_spinlock` 0xc0000000002fab78 (IP35), 0xc000000000287280 (IP27), 0xa800000020210fe0 (IP30); `nested_bitlock` 0xc0000000002fac44, 0xc0000000002873bc, 0xa80000002021111c; `us_delay` 0xc000000000069c48, 0xc000000000047274, 0xa8000000200053c0; `cpu_waitforreset` 0xc000000000077958, 0xc000000000052cf8, 0xa80000002000c470; `panicspin` 0xc0000000002711dc, 0xc0000000001fa3b8, 0xa8000000201a846c.

Reading Count: in all six images the only reads of CP0 register 9 (Count) are `_get_count` and `resetcounter` (and `trimcompare` on IP30), which are called for profiling and time-keeping, never from the idle path. The CP0 Cause register is read by the exception and interrupt entry code, `getcause`, `siron`/`siroff` and the bad-address probes. So a loop that reads Count in the kernel is not the idle loop.

Telling idle from `panicspin` in a trace: `panicspin` runs with interrupts masked (`spl7`) and calls `us_delay(500)`, `doactions` and `chkdebug`; idle runs at level zero and calls none of these in its inner loop. Telling idle from a lock spin: a lock spin is the 9-instruction `ll`/`sc` loop at the addresses above, entered from a caller that holds a raised priority level.

## 4. Does the kernel checksum or verify its text at run time?

Not found. Inferred, with these checks behind it (established):

* No symbol with a name referring to a kernel-text checksum, hash or verify exists in the symbol tables of the six images apart from unrelated ones: `compute_crc32` (callers: the `hash_*` table routines and `kcdb_free`, not text), `update_checksum` (NVRAM dictionary, callers `nvram_dict_*` and, on IP27, `wtodc`), `vh_checksum` (disk volume header), the MD5 routines (`_BSD_MD5Init`, `_BSD_MD5Update`, `_BSD_MD5Final`, used by the network code), `verify_snchip_rev` and `fprom_verify` (hardware revision and flash).
* The idle path touches no memory in the text segment at all.
* The one place kernel text is rewritten rather than verified is the IP32 object, which has a symbol `wait_for_interrupt_fix_loc` immediately before the WAIT instruction (section 5). Its purpose, from the name and layout, is to let start-up code slide the final status-register write and WAIT apart from a cache-line boundary; this is inferred. No verification of the result was found.

An exhaustive proof of absence would need a read of every loop in the kernel; this was a search by name and by callers, so the grade stays inferred.

## 5. WAIT, and the processor-type switch

**Linked IP27, IP30, IP35 (both releases): no WAIT, no processor-type switch.** A scan of every executable word of all six images for the encoding `0x42000020` found none. The idle path never reads the processor revision register either. These are the R10000-family boards; the polling loop above is what runs there.

**Unlinked IP22 (6.5.22) and IP32 (6.5.30 and 6.5.22): WAIT is present and gated by processor type.** The routine `wait_for_interrupt` in `kernel.o` (not the C version above) reads the processor revision register, masks the implementation field, and compares it with a list by successive subtraction. If it matches, it masks interrupts (writes zero to Status), reads the first byte of the word the caller passed (the next-thread pointer), and if that byte is zero restores Status and, as the next instruction after the restore, executes WAIT, followed by four no-operations and a return. If it does not match, or the byte is nonzero, it restores Status and returns at once; the caller loops. The status-restore and the WAIT are adjacent so an interrupt cannot slip between them (established from the instruction order; the race rationale is a comment-level inference).

| Object | `wait_for_interrupt` (.text offset) | `wait_for_interrupt_fix_loc` | WAIT instruction | Implementations that take the WAIT path (implementation field of the revision register) |
|---|---|---|---|---|
| 6.5.30 IP32boot `kernel.o` | 0x1534, 160 bytes | 0x15ac | 0x15b0 | 0x20, 0x21, 0x23, 0x27, 0x28 |
| 6.5.22 IP32boot `kernel.o` | 0x152c, 160 bytes | 0x15a4 | 0x15a8 | 0x20, 0x21, 0x23, 0x27, 0x28 |
| 6.5.22 IP22boot `kernel.o` | 0x16a0, 148 bytes | 0x170c | 0x1710 | 0x20, 0x21, 0x23, 0x28 |

Implementation numbers are the values in the installed `sys/sbd.h`: 0x20 is the R4600, 0x21 the R4700, 0x23 the R5000 (also named TRITON there), 0x27 the RM7000, 0x28 the RM5271. **The R4600/R5000-only statement is correct in spirit but incomplete:** the list also contains the R4700 and RM5271 on both boards and the RM7000 on IP32; the R4650 (0x22) is not in either list. An R10000 or R12000 (the IP28 and the O2 R10000 parts) falls through and does not wait; this follows from the arithmetic on the list, not from a run. The IP28 6.5-base object (`IP28boot/os.a`, `machdep.o`) has only a local C `wait_for_interrupt` and no WAIT (scan of that archive and of `IP28boot/kernel.o`). No other `kernel.o`, `os.a` or linked image of the held IP30, IP27 and IP35 contains a WAIT. The IP22 6.5.30 and any 5.3 objects are not held, so nothing is said about them.

Where it is called: the `idle` routine of IP22 and IP32 (`machdep.o`, `idle` at .text+0x2b8 on 6.5.30 IP32, +0x2cc on 6.5.22 IP22) refers to `wait_for_interrupt` as an external symbol of `kernel.o`; the other platforms define a local C function of that name inside `machdep.o` (0x180, 48 bytes, the loop in section 1), which is the one linked into the IP27, IP30 and IP35 kernels.

## Unknowns

* Whether the per-CPU data window is mapped cached in each kernel (no loop touches the TLB entry that sets it).
* The reason 6.5.30 added a flag test to `idle` (section 1).
* How many instructions one pass of the primary idler's `idlerunq` scan takes on each build (not measured).
* Which interprocessor-interrupt register a given `sendintr` targets, by hardware name, on IP35: the address arithmetic is read in full, the register names are not rechecked against the Bedrock notes.
* Linked IP32, IP22, IP28 and 5.3 behaviour: only the unlinked objects are held.

## How this was read

Capstone (MIPS64, big-endian) over the ELF segments and symbol tables, a scan of every executable word for the WAIT encoding and for coprocessor-0 reads of Count and Cause with symbol attribution, a call-site scan for calls and jumps to `resumeidle`, `idle`, `sendintr` and the idle routines, and a small instruction interpreter for the instruction count. The IRIX 6.5.7 source tree was used as a hint for where to look and for names; every fact above is from the binaries.
