

[英文原文](https://man7.org/linux/man-pages/man2/ptrace.2.html)是**process trace**的简写

#### 原型

```c
#include <sys/ptrace.h>
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data)
```

#### 描述

**ptrace()**系统调用提供了**一个能让一个进程观察和控制另一个进程执行，并且可以检查和修改被跟踪进程的内存和寄存器的手段**。该系统调用主要用来实现断点调试和系统调用跟踪。

被跟踪进程首先需要**被附着(attached)**在执行跟踪的进程上。附着操作和后续的命令是按线程为粒度进行的：在一个多线程的进程中，每个线程可以被单独地附着在跟踪者进程上（有可能是不同的跟踪者），或者没有附着因此没有被调试。所以，被跟踪者永远指一个线程，绝不会指一个可能是多线程的进程。**Ptrace**命令总是通过下面的形式被分配给**被跟踪者（tracee）**

```c
ptrace(PTRACE_foo, pid, ...)
```

**pid**是对应的Linux线程的线程ID。
