# Laboratory Work No. 7: Three Small Programs (Linux Process Management)

**Course:** Operating Systems  
**Institution:** Technical University of Moldova (UTM), FCIM / FAF  
**Academic Year:** 2026–2027  

---

## Part 1: fork + exec + wait (`run.c`)

### Answers to Observe Questions:
1. **The line `perror("exec")` is in the child, right after `execlp`. Why is it printed only with `nosuchprog`?**  
   When `execlp` succeeds (as with `ls` or `date`), the operating system completely overwrites the child's address space, text, data, and stack segments with the new binary image. Execution restarts at the new program's entry point (`_start`), so the lines following `execlp` are destroyed and never executed. However, when `execlp` fails (as with `nosuchprog`), the system call returns `-1`, setting `errno` (`ENOENT`), and execution continues immediately to `perror("exec")` and `exit(127)`.

2. **Which number does the child print as "my parent", and whose PID is it?**  
   The child prints its parent PID via `getppid()`, which is identical to the PID of the original parent process (`run`), printed as `parent: my PID is <PID>`.

3. **What status does the parent see with `ls`, and what status with `nosuchprog`?**  
   - With `ls`, the child exits normally with return code `0`, and the parent reports: `status 0`.
   - With `nosuchprog`, `execlp` fails, executing `exit(127)`, and the parent reports: `status 127` (standard Unix exit code indicating command not found).

---

## Part 2: A Zombie, on Purpose (`zombie.c`)

### Answers to Observe Questions:
1. **Which letter does `ls` have in the `STAT` column, and what does it mean?**  
   In `ps -o pid,ppid,stat,cmd`, the `ls` process shows the status letter **`Z`** (marked as `[ls] <defunct>`). It means the process has terminated its execution (all memory and file descriptors released), but its process control block (task struct) remains in the kernel process table because its parent has not yet collected its exit status via `wait()` / `waitpid()`.

2. **After 20 seconds, is the `[ls] <defunct>` line still there? Which line of `zombie.c` made it disappear?**  
   After 20 seconds, `[ls] <defunct>` is completely gone. The line in `zombie.c` that reaped it is:
   ```c
   waitpid(pid, &status, 0);
   ```
   Executing `waitpid()` reads the termination status, allowing the kernel to deallocate the entry from the process table.

---

## Part 3: Redirection: `ls -l > out.txt` (`redirect.c`)

### Answers to Observe Questions:
1. **Which number did `open` give, and why exactly that one?**  
   `open()` returned file descriptor **`1`**. Under POSIX specifications, the kernel is guaranteed to allocate the lowest-numbered file descriptor currently unused by the process. Because the child called `close(1)` immediately beforehand, file descriptor 1 (standard output) was the lowest available descriptor slot.

2. **`ls` was not changed at all. Why does its listing not appear on the screen?**  
   File descriptor tables are preserved across `exec()` (unless `O_CLOEXEC` is set). The compiled `ls` binary simply writes standard output to descriptor `1`. Because descriptor `1` was redirected to point to `out.txt`, `ls` unwittingly writes its entire output buffer into the file instead of the terminal display.

3. **To redirect the errors instead (like `2> err.txt`), which descriptor would you close before `open`?**  
   Descriptor **`2`** (`STDERR_FILENO`, standard error). Calling `close(2)` before `open("err.txt", ...)` would bind file descriptor 2 to the error log file.

---

## Part 4: A Pipe: `ls /etc | wc -l` (`pipeline.c`)

### Answers to Observe Questions:
1. **Did `./pipeline` and `ls /etc | wc -l` give the same number?**  
   Yes, both `./pipeline` and `ls /etc | wc -l` yielded the exact same line count: `215`.

2. **After the change (commenting out `close(p[1])` in the parent), which of the two children never ends, `ls` or `wc`?**  
   The second child, **`wc`**, never ends and hangs indefinitely.

3. **Why, in one sentence: what is `wc` waiting for?**  
   `wc` is blocked waiting for EOF (End-of-File) on descriptor 0 (`p[0]`), which will never arrive because the parent process still holds an open write reference to `p[1]`.
