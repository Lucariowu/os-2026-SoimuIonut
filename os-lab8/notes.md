# Laboratory Work No. 8: The Same Calls Inside Your Own OS (xv6)

**Course:** Operating Systems  
**Institution:** Technical University of Moldova (UTM), FCIM / FAF  
**Academic Year:** 2026–2027  

---

## Part 1: `run`, inside xv6 (`user/run.c`)

### Answers to Observe Questions:

1. **Write the three calls that `run.c` uses in xv6 and the three calls `run.c` used on Linux in Lab 7. What is the same?**
   - **Calls in xv6 (`user/run.c`):**
     1. `fork()`
     2. `exec(path, argv)`
     3. `wait(&status)`
   - **Calls in Linux (`run.c`):**
     1. `fork()`
     2. `execlp("ls", ...)` (part of the `exec` family wrapping `execve`)
     3. `waitpid(pid, &status, 0)` (or `wait(&status)`)
   - **What is the same:**  
     The fundamental process abstraction, lifecycle model, and Unix system call interface are identical:
     - `fork()` duplicates the calling process, returning the child's PID to the parent and `0` to the child.
     - `exec()` replaces the process image in memory with a new binary executable without changing the PID.
     - `wait()` suspends the parent until a child process terminates and retrieves its exit status.
     Both Unix (Linux) and xv6 adhere to this separation of process creation (`fork`) from program invocation (`exec`).

2. **Who prints "hello from the child": the code of `run`, or `echo`?**  
   The program **`echo`** prints "hello from the child". When `exec("echo", args)` succeeds, the address space, machine instructions, and registers of `run` in the child process are completely replaced by the `echo` binary. The text printed is the command-line arguments passed to `echo`, executed by `echo`'s own `main()` function in xv6.

3. **Why is the status 1 here and 127 on Linux? (Look at the exit line after exec in both programs.)**  
   - In xv6's `run.c`: If `exec()` fails, the fallback code executes:
     ```c
     printf("exec failed\n");
     exit(1);
     ```
     Hence, the child exits with explicit status code `1`, which the parent reads through `wait(&status)`.
   - In Linux's `run.c`: The error handling executes:
     ```c
     perror("exec");
     exit(127);
     ```
     Linux adheres to standard POSIX shell conventions where exit code `127` specifically denotes "command not found" / "exec failed". In both operating systems, the status seen by the parent is whatever integer argument was passed to `exit()` in the child after `exec` failed.

---

## Part 2: `pingpong` (`user/pingpong.c`)

### Answers to Observe Questions:

1. **Why do you need TWO pipes, and not one for both directions?**  
   A standard Unix/xv6 pipe is strictly **unidirectional** (one read end and one write end). If only a single pipe were used for bidirectional communication:
   - When the parent writes a byte into the pipe and immediately tries to read the reply, the parent could read its own byte back before the child ever wakes up, or both processes could read simultaneously resulting in a race condition.
   - Using two dedicated pipes (`p2c` for parent-to-child and `c2p` for child-to-parent) ensures clear data flow and avoids race conditions, deadlocks, and self-consumption of messages.

2. **Nothing in your code says "wait until the child has replied". Which call makes the parent wait?**  
   The system call **`read(c2p[0], &b, 1)`** makes the parent wait. Reading from an empty pipe in Unix and xv6 blocks the calling process until at least one byte of data is written by the other end (or until all write descriptors to that pipe are closed, yielding EOF). Since the child only writes to `c2p[1]` after receiving its ping, the parent's `read()` automatically suspends execution until the child has received, processed, and replied with the pong byte.
