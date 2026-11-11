# Spin-lock word and the cpuset record lock (IP27 kernel)

How the kernel's plain spin lock works at the instruction level, which lock `exitrunq` takes, and what a processor that spins on it can and cannot do. Written to explain a four-processor hang in which three processors spin at the entry of the lock routine and the lock word reads 3.

Binary: the IP27 `unix` of IRIX 6.5.21f taken from an installed system disk (11,547,368 bytes, SHA-256 `68e73b5832e09665bdebe3fb3c5aa3a73ddc1f7034d16424482e4003003b7e00`, linked 64-bit, symbol table and DWARF present). This is not one of the two releases the rest of this repository targets, so nothing here transfers to 6.5.22 or 6.5.30 until the same addresses are read there. All addresses below are in that image. The global pointer is 0xc0000000015087f0 (from the options section). Grades are as in the repository rules: established is read in the instructions, inferred is a stated reading, unknown has the route that was tried.

## The lock word

Established, reading the whole bodies:

- `nested_spinlock` (0xc0000000002bc080) is a load-linked loop: load-linked the word, test bit 0, branch back to the load when it is set, otherwise set bit 0 and store-conditional, retry on failure. It never touches the status register. `nested_spintrylock` (0xc0000000002bc0b8) is the same loop that returns failure instead of looping.
- `nested_spinunlock` (0xc0000000002bc0a4) is an ordinary load, an exclusive-or with 1, a `sync`, and an ordinary store. It is not load-linked and it toggles bit 0, it does not clear it.
- `mutex_spinlock` (0xc0000000002bc014) saves the status register, clears its interrupt-enable bit (bit 0), then runs the same loop; when the lock is held it restores the saved status register, so interrupts are enabled again while it waits, and retries. `mutex_spinunlock` (0xc0000000002bc060) is the toggle store followed by restoring the saved status. `mutex_spinlock_spl` (0xc0000000002bbfc0) takes the lock after raising the interrupt priority level through the routine passed in `a1`, and drops back to the old level while it waits. `mutex_bitlock_ra` (0xc0000000002bc154) is the same shape for an arbitrary bit mask: it masks interrupts (status bit 0 cleared), takes the bit, and returns the old status, which `mutex_bitunlock` (0xc0000000002bc1a0) restores.
- `spinlock_init` (0xc0000000002bbde0) and `init_spinlock` (0xc0000000002bbdd4) store the value 2. `spinlock_initialized` (0xc0000000002bbdf4) returns bit 1 of the word and `spinlock_islocked` (0xc0000000002bbe00) returns bit 0.

So bit 0 is the held flag and bit 1 is an "initialised" marker that is set at creation and never cleared. It is not an owner, nesting or interrupt-state bit. A healthy free lock reads 2 and a held lock reads 3. There is no owner field anywhere in the word, so the word cannot name the holder.

Inferred consequence: because unlock toggles, an unlock of a lock that is not held turns a free word (2) into a held word (3) with no holder. Two processors that both believed they held the lock would end the same way (the first unlock frees it, the second re-sets it). A word that reads 3 with no processor inside a critical section is therefore the signature of an unbalanced unlock or of broken mutual exclusion, as well as of a plain holder that is stuck. The word alone does not tell these apart.

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

Not established: that this is what happened in the hang. The hang shows no sign of `cpuset_destroy` (the non-spinning processor was seen in `tlbsync` wait, `idle` and `local_idle`, and also in user code), and no routine other than `cpuset_destroy` was found that waits for another processor while holding a record lock. The alternatives that fit the observation equally well and need no waiter at all are: (a) the record lock word was left at 3 with no holder by an unbalanced toggle-unlock or by two simultaneous holders (first section); (b) a holder that the sampling missed. Unknown which. What would settle it, read from the hung system: the `cpusets` pointer at 0xc000000001509ca8 (gives the record index), the word at record +0x10 (member count), each processor's saved return address chain and status register (which are masked), and the group counters at gp-0x2470 + group * 0x80 + 0x800 (non-zero means a `tlbsync` is waiting).

## What this says about the machine

Established: unlock is a plain store after `sync`, and acquisition is load-linked/store-conditional on the same word. A machine that is correct for this lock must (1) make a store-conditional fail if any other processor's store or store-conditional hit the word since the load-linked, including the plain store that unlocks, and (2) make the critical-section stores visible before the unlocking store (the `sync` at 0xc0000000002bc0ac and 0xc0000000002bc06c orders them). Failing either lets two processors hold the lock, and the toggle unlock then leaves the word at 3 with no holder, which is the observed state.

Established: a processor with the status interrupt-enable bit clear must not take the shootdown interrupt, on hardware either. Delivering an inter-processor interrupt into a masked, spinning processor is not what the hardware does, and would not be a fix; it would hide the ordering above.
