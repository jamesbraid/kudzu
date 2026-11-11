# Spin-lock word and the cpuset record lock (IP27 kernel)

How the kernel's plain spin lock works at the instruction level, which lock `exitrunq` takes, and what a processor that spins on it can and cannot do. Written to explain a four-processor hang in which three processors spin at the entry of the lock routine and the lock word reads 3. The cause is a lock leak in `cpuset_destroy` of the 6.5.21f kernel, set out in the next section; the rest of the page is the background that the leak reading rests on.

Binary: the IP27 `unix` of IRIX 6.5.21f taken from an installed system disk (11,547,368 bytes, SHA-256 `68e73b5832e09665bdebe3fb3c5aa3a73ddc1f7034d16424482e4003003b7e00`, linked 64-bit, symbol table and DWARF present). This is not one of the two releases the rest of this repository targets, so nothing here transfers to 6.5.22 or 6.5.30 except where a section says it read those kernels (only `cpuset_destroy`, in the per-build table of the cause section). All other addresses below are in the 6.5.21f image. The global pointer is 0xc0000000015087f0 (from the options section). Grades are as in the repository rules: established is read in the instructions, inferred is a stated reading, unknown has the route that was tried.

## Cause of the hang: `cpuset_destroy` leaks the record 1 lock when its lookup fails

Established, reading `cpuset_destroy` (0xc0000000001d2b18) to its end in the 6.5.21f image above:

- The routine takes the lock word of record 1 (`cpusets` + 0x114, so record offset 0x14) with `nested_spinlock` at +0x74 (0xc0000000001d2b8c). It does this once the first doubleword of the argument block (the id to look up) is non-zero, and it takes it whether or not the permission-check flag (bit 0x200 of the second doubleword) is set. The checks that can fail first (the capability test at +0x44, the zero id at +0x6c and the range check at +0x2e8 to +0x2fc) return before this call, so they are not the problem.
- It then calls the lookup helper at `cpuset_init`+0x224 (0xc0000000001d12ac, call at +0xd4). The helper walks records 2 and up, takes each record's own lock (offset 0x14) with `mutex_spinlock`, compares the record's id word at offset 8 with the argument, and returns the record with its lock held, or zero having dropped each lock it took.
- When the helper returns zero the branch at +0xdc goes to +0x268. That code sets the return value (3 when the flag is clear; when the flag is set, a test of one bit in the mask at `cpusets` + 0x120 + (id / 64) * 8, giving 1 or 0) and jumps to +0xc0 with the loop flags clear. +0xc0 branches to +0x2a4 (0xc0000000001d2dbc), which is the function epilogue: it reloads the saved registers and returns at +0x2c8 (0xc0000000001d2de0). Nothing on that path calls an unlock.
- The only `nested_spinunlock` of the record 1 lock is at +0x314 (0xc0000000001d2e2c). It is reached only through the success path at +0x304, after the found record's own lock has been released at +0x308. A failed lookup therefore leaves the record 1 lock word at 3 with nobody holding it. It is not a toggle misuse or a missing mutual exclusion; the routine simply returns with the lock held.

Established by the same read: the `sysmp` case that ends in the call at `sysmp`+0x124c (0xc000000000274208) clears an argument block, stores the processor number from the request as the first doubleword, or's 0x200 into the second and calls `cpuset_destroy`. The request is the one `mpadmin -u` issues (sysmp EMPOWER). Inferred: a processor number is not a cpuset id, so unless the processor has a record the lookup finds none, and the not-found path above is the ordinary path for this request on an unrestricted processor, not a rare error.

Observed, reproduced under emulation: a stock `mpadmin -u2` on a processor that is not restricted makes `sysmp` return EPERM and leaves the record 1 lock word at 3 (it reads 2 beforehand). The next thread exit runs `exitrunq`, which takes the same lock with `nested_spinlock` and spins at its entry with interrupts masked, which is the original four-processor signature. Restricting the processors first with `mpadmin -r` avoided the hang in the reproduction (inferred reason: a restricted processor has a record for the lookup to find). The return value 1 on an unrestricted processor is consistent with the bit test above reading a set bit; what that mask means is unknown (see the open question).

Established, per build. "Fixed" means the failed-lookup return reaches an unlock of the record 1 lock. Addresses are in each image; offsets are from `cpuset_destroy`.

| Build (SHA-256 prefix, size) | `cpuset_destroy` | Lock call | Failed-lookup return | Unlock of the record 1 lock | Verdict |
| --- | --- | --- | --- | --- | --- |
| 6.5.21f IP27, installed `/unix` (`68e73b58`, 11,547,368) | 0xc0000000001d2b18 | +0x74, `nested_spinlock` | +0x2a4, epilogue, no unlock | +0x314 (0xc0000000001d2e2c), success path only | leaks |
| 6.5.21f IP27, second image (`97426d87`, 11,047,720) | 0xc00000000018db58 | +0x74 (0xc00000000018dbcc), `nested_spinlock` | +0x2a4 (0xc00000000018ddfc), epilogue, no unlock | +0x314 (0xc00000000018de6c), success path only | leaks, same shape |
| 6.5.22 IP27 (`40e3f624`, 11,237,256) | 0xc00000000019d8c8 | +0x64 (0xc00000000019d92c), `nested_spinlock` | +0x2a4 (0xc00000000019db6c) | +0x2a8 (0xc00000000019db70), in the shared exit, then return at +0x2d4 | fixed |
| 6.5.30 IP27 (`56747467`, 11,101,184) | 0xc00000000019a108 | +0x60 (0xc00000000019a168), `mutex_spinlock`, saved status kept at sp+0x30 | +0x2b0, joining the exit at +0x264 or +0x268 | +0x268 (0xc00000000019a370), `mutex_spinunlock` with the saved status, then return at +0x28c | fixed |
| 6.5.22 IP35 (`87563dbc`, 12,168,592) | 0xc00000000021351c | +0x84 (0xc0000000002135a0), `nested_spinlock` | +0x498 | +0x49c (0xc0000000002139b8), then return at +0x4d0 | fixed |
| 6.5.30 IP35 (`412a1aae`, 13,104,040) | 0xc000000000248794 | +0x90 (0xc000000000248824), `mutex_spinlock` | +0x49c, joining the exit at +0x454 | +0x458 (0xc000000000248bec), `mutex_spinunlock`, then return at +0x478 | fixed |

In 6.5.22 the unlock moved into the single exit that every post-lock path reaches; the pre-lock error returns jump past it (+0x2c4 on IP27). In 6.5.30 the lock became `mutex_spinlock`, so interrupts are masked while it is held and the saved status is restored by the unlock. Also changed in 6.5.30 on both boards, inferred and not otherwise checked: the final bit test is inverted (`sltiu` against 1, giving 1 when the bit is clear and 0 when it is set, where 6.5.21f and 6.5.22 give 1 when it is set), so the not-found return value of a successful empower request differs between 6.5.22 and 6.5.30. Which sense matches the hardware-independent intent is unknown.

Not examined: the intermediate releases (6.5.14 to 6.5.20 and 6.5.23 to 6.5.29), so where between 6.5.21f and 6.5.22 the fix landed is unknown; the IP30 kernels of any release (their `cpuset_destroy` has a different layout and was not read to the end); the single 6.5.13f Octane disk was seen but its kernel was not extracted. The page's other addresses (`exitrunq`, the lock routines, `tlbsync`) are read only in the 6.5.21f image.

## The lock word

Established, reading the whole bodies:

- `nested_spinlock` (0xc0000000002bc080) is a load-linked loop: load-linked the word, test bit 0, branch back to the load when it is set, otherwise set bit 0 and store-conditional, retry on failure. It never touches the status register. `nested_spintrylock` (0xc0000000002bc0b8) is the same loop that returns failure instead of looping.
- `nested_spinunlock` (0xc0000000002bc0a4) is an ordinary load, an exclusive-or with 1, a `sync`, and an ordinary store. It is not load-linked and it toggles bit 0, it does not clear it.
- `mutex_spinlock` (0xc0000000002bc014) saves the status register, clears its interrupt-enable bit (bit 0), then runs the same loop; when the lock is held it restores the saved status register, so interrupts are enabled again while it waits, and retries. `mutex_spinunlock` (0xc0000000002bc060) is the toggle store followed by restoring the saved status. `mutex_spinlock_spl` (0xc0000000002bbfc0) takes the lock after raising the interrupt priority level through the routine passed in `a1`, and drops back to the old level while it waits. `mutex_bitlock_ra` (0xc0000000002bc154) is the same shape for an arbitrary bit mask: it masks interrupts (status bit 0 cleared), takes the bit, and returns the old status, which `mutex_bitunlock` (0xc0000000002bc1a0) restores.
- `spinlock_init` (0xc0000000002bbde0) and `init_spinlock` (0xc0000000002bbdd4) store the value 2. `spinlock_initialized` (0xc0000000002bbdf4) returns bit 1 of the word and `spinlock_islocked` (0xc0000000002bbe00) returns bit 0.

So bit 0 is the held flag and bit 1 is an "initialised" marker that is set at creation and never cleared. It is not an owner, nesting or interrupt-state bit. A healthy free lock reads 2 and a held lock reads 3. There is no owner field anywhere in the word, so the word cannot name the holder.

Inferred consequence (a reading of the routines in isolation; the leak above is the cause that actually occurred): because unlock toggles, an unlock of a lock that is not held turns a free word (2) into a held word (3) with no holder. Two processors that both believed they held the lock would end the same way (the first unlock frees it, the second re-sets it). A word that reads 3 with no processor inside a critical section is therefore the signature of an unbalanced unlock or of broken mutual exclusion, as well as of a plain holder that is stuck. The word alone does not tell these apart.

## Which lock `exitrunq` takes

Established. `exitrunq` (0xc0000000001ca78c) first takes bit mask 4 of the thread word at thread offset 0x84 with `mutex_bitlock_ra`, so interrupts are masked from there to the end of the routine. It then reads the halfword at thread offset 0xf4, scales it by 256, adds it to the pointer held in the kernel variable `cpusets` (0xc000000001509ca8, reached as gp+0x14b8) and takes the spin lock at record offset 0x14 with `nested_spinlock` (call at 0xc0000000001ca7c0, return address 0xc0000000001ca7c8). If that halfword is 2 or more it decrements the word at record offset 0x10. It releases the lock with `nested_spinunlock` at 0xc0000000001ca7fc and carries on with no call between lock and unlock.

So the lock is per cpuset record, not the global run queue and not per processor. The thread's halfword at +0xf4 is a cpuset index. The critical section in `exitrunq` is five instructions and makes no calls, so a thread that spins at 0xc0000000001ca7c0 is waiting for somebody else's critical section. It is waiting with interrupts masked (status bit 0 clear) and with the thread's own bit lock held.

The `cpusets` array is created by `cpuset_init` (0xc0000000001d1088 region): `kmem_zalloc` of (`maxcpus` * 256 + 512) bytes, `max_cpusets` (0xc000000001509cc0) set to `maxcpus` + 2, 256-byte records. Record 0 and record 1 get their lock word (offset 0x14) from `spinlock_init` at fixed offsets; records 2 and up are filled in a loop. Every processor is placed in cpuset 1 initially (the store of 1 into each `cpu_to_cpuset` entry, 0xc0000000001d11d0). Inferred: record 1 is the default set that holds all processors, so threads that never use cpusets presumably all carry index 1 and all contend on record 1's lock. Established only that the lock is at `cpusets` + 256 * index + 0x14.

The word at physical 0x6ec914 (kernel address 0xa8000000006ec914) is therefore inside the record that starts at 0x6ec900. It has no symbol of its own. Which index that is depends on the runtime value of the `cpusets` pointer (the doubleword at 0xc000000001509ca8): index = (0x6ec900 minus that pointer) divided by 256. Unknown until that pointer is read from the hung system.

## Who takes record locks

Established by call sites: a scan of every `nested_spinlock`, `mutex_spinlock` and trylock call that follows a load of `cpusets` finds record locks taken in `exitrunq`, `uthreadrunq` (0xc0000000001ca9f0, `mutex_spinlock`), `rt_thread_select`, `rt_remove_q`, `rt_add_gq`, `cpuset_init`, `cpuset_create`, `cpuset_destroy`, `cpuset_attach`, `cpuset_attach_pid` and `cpuset_move_op`. The scan is partial, because other callers receive the record pointer in a register. Inferred from names and call shapes only, not checked: the batch routines (`batch_unset_binding` and others), `gang_link`, `gang_startrun`, `gang_sched_tick`, `cpu_insert_localq` and `cpuset_process` also take a record lock. The idle-scan helper that follows `local_idle` (0xc0000000001ca198) reads the record flag word at +0x18 without taking the lock (established).

## Can the lock be held across a wait on another processor

Established for one path. `cpuset_destroy` (0xc0000000001d2b18) takes the lock of record 1 (`cpusets` + 0x114) with `nested_spinlock` at 0xc0000000001d2b8c and releases it with `nested_spinunlock` at 0xc0000000001d2e2c. Between them, inside its loop, it calls `cpuset_destroy_memsys` (0xc0000000002a2710, call at 0xc0000000001d2c28). By direct calls that routine reaches `cpuset_mem_init`, then `tosspages`, then `tlbsync`. `cpuset_destroy` is reached from `sysmp` (0xc000000000274208), `mp_isolate` (0xc000000000274ee4), `mp_unisolate` (0xc0000000002751bc), `cpuset_process`, `cpuset_attach`, `cpuset_attach_pid` and `cpuset_move_op`. Inferred (static direct calls only; indirect calls were not followed, and no conditional on the memory-set flag was evaluated): a cpuset destroy can wait in `tlbsync` while holding the record 1 lock.

Not found: any path from the routines that touch `gfx_flow_attach_me` (0xc0000000003d8dac), `KONAresume` (0xc0000000003c70c4) or `poll` into a record lock. A direct-call search from those routines was not run to completion; this is unknown, not a negative result. `exitrunq` itself holds the lock over no calls. `batch_unset_binding` (0xc0000000001cb834) holds its lock (inferred to be a record lock) across `batch_to_weightless`, which reaches `putrunq` and `sendintr` (0xc000000000039358); `sendintr` posts an interrupt and, as far as the instructions around its call sites show, does not wait (inferred).

## What `tlbsync` waits for

`tlbsync` (0xc00000000020dae4), body read from 0xc00000000020dae4 to 0xc00000000020df80. Established:

- It raises the interrupt level with `splvme` (0xc00000000020db6c), builds the set of target processors from two bit masks, and for each group of eight processors adds to a counter at group base + 0x800 (base is the pointer at gp-0x2470; `atomicAddUintHot2`, call at 0xc00000000020dc3c).
- For each target processor it either posts an interrupt with `sendintr(cpu, 0xab)` (0xc00000000020dc94), sends vector 0xbb after incrementing a counter in the target's structure (0xc00000000020dda4 to 0xc00000000020ddc8), or queues a `nested_cpuaction` (0xc00000000020dd44, 0xc00000000020de1c). Which of these each target gets depends on per-target flags; the choice was not decoded in full (unknown).
- It flushes the local TLB with `tlbflush` (0xc00000000020decc), restores the caller's interrupt level with `splxecc` (0xc00000000020ded4), then loops: sum the group counters (loop at 0xc00000000020dee0 to 0xc00000000020df18) and repeat while the sum is non-zero. The branches at 0xc00000000020df18 and 0xc00000000020dee0 are the two program counters that show a processor stuck in the wait, `tlbsync`+0x3fc and +0x434.

Inferred: each counter is brought back to zero by the target processors in their interrupt handlers (the handler was not located), so the caller can only finish when every target has taken its interrupt. The caller's own wait runs at whatever interrupt level the caller had on entry, so if its caller had interrupts masked, the wait does too (established that the level restored is the saved one; the callers' levels were not checked).

## Deadlock cycle

Established ingredients, from the instructions above:

1. A processor in `nested_spinlock` does not re-enable interrupts. A processor that entered `exitrunq` has them masked (`mutex_bitlock_ra`, 0xc0000000002bc154). Masked means the status register interrupt-enable bit is clear; the architecture does not deliver any external or inter-processor interrupt to such a processor.
2. `tlbsync` returns only after the target processors have acknowledged, and an acknowledgement needs the interrupt to be taken.
3. `cpuset_destroy` can reach `tlbsync` with the record 1 lock held.

Inferred cycle (all three parts needed): a processor A in a cpuset destroy holds the record lock and waits in `tlbsync` for acknowledgements; processors B and C are in `exitrunq` waiting for that lock with interrupts masked and cannot acknowledge. That is a genuine lock-ordering deadlock in the kernel's design and would hang the same way on real hardware if the timing occurred.

Not what happened in the observed hang: the leak above explains it with no waiter at all, and a `cpuset_destroy` that is still running would not fit the sampling (the non-spinning processor was seen in `tlbsync` wait, `idle` and `local_idle`, and also in user code). The cycle above stays a real hazard of the 6.5.21f design (inferred, static direct calls only) that nobody has observed.

## Ruled out: broken load-linked/store-conditional or store ordering

An earlier reading of this hang was that the emulated machine let two processors hold the lock at once (a store-conditional that did not fail after another processor's store, or an unlock store that became visible before the critical-section stores), and that the toggle unlock then left the word at 3 with no holder. That is ruled out as the cause. The same hang, with the same lock word and the same `exitrunq` spin, reproduces from a stock `mpadmin -u2` under an emulation run driven by an instruction count in which only one processor executes at a time, so it does not depend on cross-processor store-conditional or store-ordering behaviour, and the instructions of the failed-lookup path contain no unlock at all (section above).

What survives from the original analysis, established: the lock is load-linked/store-conditional on acquire and a plain store after `sync` on release, so a machine must make a store-conditional fail after any other processor's store to the word, including the unlocking store, and make the critical-section stores visible before the unlocking store. Nothing observed so far shows the emulation violating this. Also established: a processor with the status interrupt-enable bit clear does not take the shootdown interrupt on hardware either, so delivering an inter-processor interrupt into a masked, spinning processor would hide an ordering problem and is not a fix.
