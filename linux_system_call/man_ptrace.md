**prtrace**是**process trace**的缩写。

### 概要

```c
#include <sys/ptrace.h>
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
```

### 描述

ptrace() 系统调用提供了一个途径，让一个进程（tracer）可以对另一个进程（tracee）的执行进行观察和操控。并且可以检查和改变tracee的内存和寄存器。这个系统调用主要用来实现断点调试和系统调用跟踪。

被跟踪进程需要依附于跟踪者进程。依附和后续操作都是按每个线程进行的。

