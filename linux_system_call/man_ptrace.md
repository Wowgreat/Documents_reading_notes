

[英文a原文](https://man7.org/linux/man-pages/man2/ptrace.2.html)

**ptrace**是**process trace**的简写

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

> 注意：在本文中，一个多线程的进程，值得是一个线程组（由使用clone(2) **CLONE_THREAD**标志创建的线程组成的线程组）。

一个进程可以通过调用fork(2)并让产生的子进程执行*PTRACE_TRACEME*来启动跟踪，随后（通常）跟着*execve(2)*方法。或者说，一个进程使用**PTRACE_ATTACH**或**PTRACE_SEIZE**来监控另一个进程。

在被跟踪期间，被跟踪者每次收到信号都会停下来，即使这个信号被忽略（**SIGKILL**除外，该信号依旧有正常的作用——杀死进程）。追踪者将在下次调用waitpid(2)时被通知（或相关的 "等待 "系统调用之一）；调用回返回一个状态值，这个状态值包含着导致被跟踪者停止的原因。当被跟踪者被停止，跟踪者可以使用多样的**ptrace requests**检查和修改被跟踪者。跟踪者接下来可以使被跟踪者继续执行，或者忽略接收到的信号亦或者是发送一个不同的信号。

如果**PTRACE_0_TRACEEXEC**选项不生效，所有成功的*execve(2)*调用(通过被跟踪进程的调用)，都会导致此调用发出一个**SIGTRAP**信号，给父进程一个在新的程序开始执行前获得控制权的机会。

当跟踪者完成跟踪，它可以让被跟踪者按照正常情况继续执行，通过**PTRACE_DETACH**解除跟踪。

*request*选项的值决定了执行的动作：

#### PTRACE_TRACEME

意味着这个进程将会被自己的父进程跟踪。如果一个进程的父进程不期望跟踪它，那么它不会执行这个request选项。（pid，addr，和 data等参数会被忽略）

**PTRACE_TRACEME**选项只有被跟踪者可是使用；剩余的request选项由跟踪者使用。在接下来的request选项中，*pid*指的是被跟踪的线程ID。对于**PTRACE_ATTACH**，**PTRACE_SEIZE**，**PTRACE_INTERRUPT**和**PTRACEP_KILL**以外的request选项，被跟踪者必须停止。

#### PTRACE_PEEKTEXT，PTRACE_PEEKDATA

从被跟踪者的内存地址*addr*处读一个字(word)，然后将这个字作为**ptrace()**调用的记过返回。Linux没有独立的文本和数据地址空间，所以这两个请求目前是等价的。（*data*参数被忽略，详见**NOTES**）

#### PTRACE_PEEKUSER

从被跟踪者用户空间中偏移量为*addr*处读一个字(word)，该区域存放着寄存器和该进程的其他信息（详见<sys/user.h>）。这个字作为**ptrace()**调用的结果返回。通常这个偏移地址必须是字对齐的，尽管因架构不同出现多种情况（*data*参数被忽略，详见**NOTES**）。

#### PTRACE_POKETEXT, PTRACE_POKDATA

复制*data*到被跟踪者内存偏移地址*addr*处。这两个选项目前等价。

#### PTRACE_POKEUSER

复制*data*到被跟踪者用户空间的偏移量*addr*处。对于**PTRACE_PEEKUSER**来讲，偏移量必须是字对齐的。为了保持内核的完整性，一些对用户空间的修改操作是不被允许的。

#### PTRACE_GETREGS, PTRACE_GETFPREGS

复制被跟踪者的通用寄存器或者浮点寄存器分别到跟踪者的地址*data*。 关于这个数据的格式，请看<sys/user.h>的信息。请注意，SPARC系统的*data*和*addr*的含义是相反的；也就是说，数据被忽略，寄存器被复制到地址*addr*。PTRACE_GETREGS和PTRACE_GETFPREGS并不存在于所有的架构上。



