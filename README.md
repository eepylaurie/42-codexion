*This project has been created as part of the 42 curriculum by lmatthes.*

# Codexion

A concurrent simulation of coders competing for shared USB dongles, implemented in C using POSIX threads. Each coder cycles through compiling, debugging, and refactoring. Compiling requires holding two adjacent dongles simultaneously; missing the burnout deadline ends the simulation.

## Description

Codexion is a variant of the classic Dining Philosophers problem with three additional layers of complexity: a three-phase work cycle, dongle cooldown enforcement, and configurable scheduling (FIFO or EDF). Each coder runs as a separate POSIX thread. A dedicated monitor thread detects burnout within 10 ms of the actual missed deadline.

The simulation halts under one of two conditions: a coder burns out (missing their deadline), or every coder has completed the required number of compile cycles. Logs are serialized through a mutex so messages never interleave.

The program takes exactly 8 arguments:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

All time arguments are in milliseconds. The scheduler argument must be exactly `fifo` or `edf`.

## Instructions

### Build

From the project root:

```sh
cd coders
make
```

This produces the executable `codexion` in the same directory.

### Run

```sh
./codexion 5 800 200 200 200 7 50 fifo
./codexion 5 2000 200 200 200 7 50 edf
./codexion 1 800 200 200 200 7 50 fifo    # single coder will burn out
```

### Clean

```sh
make clean    # remove object files
make fclean   # remove object files and executable
make re       # full rebuild
```

### Verifying correctness

On Linux, check for memory leaks and data races:

```sh
valgrind --leak-check=full --show-leak-kinds=all ./codexion 5 2000 200 200 200 7 50 fifo
valgrind --tool=helgrind ./codexion 5 2000 200 200 200 7 50 fifo
```

On macOS, the built-in `leaks` tool works for leak detection:

```sh
leaks --atExit -- ./codexion 5 2000 200 200 200 7 50 fifo
```

## Blocking cases handled

### Deadlock prevention

Coffman's four conditions are addressed by breaking **circular wait**. Every coder acquires dongles in ascending index order: the lower-numbered dongle first, then the higher-numbered one. With this total ordering, no cycle can form in the wait-for graph. A coder waiting for dongle *N* cannot already hold a higher-numbered dongle, so no chain of waits can close.

The other Coffman conditions are inherent to the problem and cannot be removed: dongles are mutually exclusive resources (hardware), a coder must hold two dongles simultaneously (hold-and-wait), and one coder cannot rip a dongle from another (no preemption).

### Starvation prevention

When multiple coders are waiting for the same dongle, the dongle's internal priority queue (binary min-heap) chooses who proceeds next. Under FIFO, the queue is ordered by a monotonic sequence number assigned at the moment of the request, so requests are honored in arrival order. Under EDF, the queue is ordered by burnout deadline (`last_compile_start + time_to_burnout`), giving priority to whoever is closest to failing.

`pthread_cond_broadcast` is used to wake all waiters on release; each waiter rechecks its eligibility (top of the queue + dongle free + cooldown elapsed) and only the chosen coder proceeds.

### Cooldown handling

After a dongle is released, it remains unavailable for `dongle_cooldown` ms. The release function records a timestamp; the acquire function checks elapsed time against the cooldown before granting the dongle. While waiting on a cooldown specifically (no other coder holds the dongle), waiters use `pthread_cond_timedwait` so they sleep for exactly the remaining cooldown rather than busy-polling.

A sentinel value `release_time == 0` distinguishes "never released" from "just released at t=0", preventing a startup deadlock in which every dongle initially appears to be in cooldown.

### Precise burnout detection

A dedicated monitor thread polls every 1 ms. On each iteration it computes elapsed time since each coder's last compile start; if any coder's elapsed time meets or exceeds `time_to_burnout`, the monitor logs the burnout and sets the simulation stop flag. The 1 ms polling granularity is well within the 10 ms precision required by the subject.

The monitor also checks whether all coders have hit `number_of_compiles_required` and stops the simulation cleanly if so (no log message in this case).

### Log serialization

All output passes through `log_msg` or `log_burnout`, both of which hold a dedicated log mutex while calling `printf`. The timestamp is captured inside the mutex, ensuring strict monotonic ordering in the printed output even under heavy contention (verified with 200-coder stress tests).

A stop-flag check inside `log_msg` suppresses post-burnout state messages from coder threads; `log_burnout` bypasses this check so the burnout line itself always prints.

## Thread synchronization mechanisms

### Mutexes

The simulation uses three categories of mutex:

- **Per-dongle mutex (`t_dongle::mutex`)**: protects each dongle's state (taken flag, release time, wait queue). Acquired during `acquire_dongle` and `release_dongle`. Always paired with the dongle's condition variable.
- **`stop_mutex`**: protects the `stopped` flag, the `seq_counter`, and shared coder state (`last_compile_start`, `compile_count`). Read by the monitor thread, written by coders. Kept lightweight to minimize contention.
- **`log_mutex`**: protects `printf` calls so log lines never interleave.

### Condition variables

Each dongle has its own condition variable (`t_dongle::cond`). Coders wait on it when the dongle is unavailable. `release_dongle` calls `pthread_cond_broadcast` to wake all waiters; each waiter rechecks its predicate in a `while` loop, so spurious wakeups and stale conditions are handled correctly. For cooldown-only waits (dongle is free but cooling down), `pthread_cond_timedwait` is used so the waiter sleeps exactly the remaining cooldown.

### Thread-safe communication between coders and monitor

The monitor reads `last_compile_start` and `compile_count` from each coder while the coders write them. These accesses are serialized through `stop_mutex`: writes happen under the lock during `do_compile_cycle`, and reads happen under the lock inside helper functions in `sim_utils.c`. This prevents data races without introducing per-coder mutexes.

The stop flag is the primary one-way channel: the monitor sets it, every other thread polls it. Coder threads check the flag at each phase boundary inside `precise_sleep`, allowing them to exit promptly when burnout is detected (well within the 10 ms requirement).

### Race condition prevention

Every shared field has a designated mutex. The patterns used:

1. **Read-modify-write under lock**: `seq_counter++` in `next_seq_num`.
2. **Read under lock, return local copy**: `simulation_stopped`, `get_coder_deadline`.
3. **Wait-loop with predicate**: every `pthread_cond_wait` is wrapped in a `while` loop that re-checks the condition after waking, handling both spurious wakeups and state changes between threads.

## Resources

### Classic references

- Dijkstra, E. W. (1965). *Hierarchical Ordering of Sequential Processes* — original Dining Philosophers problem.
- Coffman, E. G., Elphick, M. J., Shoshani, A. (1971). *System Deadlocks* — the four necessary conditions for deadlock.
- Stevens, W. R., Rago, S. A. *Advanced Programming in the UNIX Environment* — chapters on threads and synchronization.
- Liu, C. L., Layland, J. W. (1973). *Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment* — original EDF paper.

### Online references

- POSIX manual pages: `pthread_create(3)`, `pthread_mutex_init(3)`, `pthread_cond_wait(3)`, `pthread_cond_timedwait(3)`, `gettimeofday(2)`, `usleep(3)`.
- The Linux Programming Interface, Michael Kerrisk — chapter on thread synchronization.

### How AI was used

AI was used as an interactive tutor during the development of this project. Specific roles:

- **Conceptual explanations**: walking through the Dining Philosophers problem, Coffman's conditions, condition variable semantics, and the EDF scheduling algorithm before writing code.
- **Architectural guidance**: suggested the file split (init/init_dongles, heap/heap_utils, sim_utils), the static helper patterns, and the lower-index-first acquisition order for deadlock prevention.
- **Code review and debugging**: identified the startup deadlock caused by `release_time == 0` being interpreted as "in cooldown," and the log timestamp ordering issue under high contention.
- **42 Norm guidance**: helped refactor functions exceeding 25 lines and files exceeding 5 functions.

All code was typed, tested, and committed by me. AI was not used to generate complete functions blindly; every implementation was reviewed line by line, modified to fit the project's specific structure, and tested before commit. Defense-readiness was a constant priority: anything I could not explain in my own words was rewritten until I could.
