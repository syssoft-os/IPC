# IPC
A collection of example programs using inter-process communication (IPC) techniques within a process or between processes on one computer.
Examples within a process have an explicit prefix `intra_`, any programs without this prefix focus on the more general case of communication between threads in different processes on a single host.

List of implemented examples:

1. `ipc_signal`: Receiver registers a signal handler and loops
2. `intra_semaphore`: using unnamed semaphores within a process; not working on MacOS
3. `ipc_named_semaphore`: Named semaphore to be used between two independent processes
4. `ipc_pipe`: Unnamed pipe
5. `ipc_named_pipe`: Named pipe to be used between two independent processes