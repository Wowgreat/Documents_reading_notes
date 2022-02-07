### 使用Ptrace进行进程跟踪

原文链接：[ptrace(2) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man2/ptrace.2.html#DESCRIPTION)

**Ptrace**对于调试类程序例如**gdb**是非常关键的。但目前为止它的特性都没有被很好的记录，除非你认为最好的文档是内核（一般指linux kernel或者unix kernel）文档本身。现在我将尝试演示**ptrace**是如何被用来实现一些在类似GDB工具中的功能的。

#### 介绍

**ptrace()**是一个能够使一个进程能够控制另一个进程的执行的系统调用。他还能使一个进程能够改变另一个进程的**核心镜像（core image of another process）**。

**被跟踪的进程( traced process)**执行正常，直到捕获到信号。当这种情况发生时，进程进入停止状态并通过 wait() 调用通知**跟踪进程( tracing process)**。跟踪进程决定被跟踪进程应该如何响应。只有一种异常——SIGKILL信号可以杀死进程。

被跟踪的进程也可能在其执行过程中响应某些特定事件而进入停止状态。仅当跟踪进程在被跟踪进程的上下文中设置了任何事件标志时才会发生这种情况。跟踪进程甚至可以通过设置被跟踪进程的退出码来杀死被跟踪进程。 跟踪后，跟踪器进程可能会杀死被跟踪的进程或离开以继续执行。

> **Ptrace()**高度依赖底层硬件的架构。程序使用**Ptrace**通常不能轻易地跨不同的架构

#### 更多细节

Ptrace() 的原型如下。

```c
# include <sys/ptrace.h>
long int ptrace(enum __ptrace_request request, pid_t pid, void * addr, void * data)
```

四个参数中，**request**的值决定要做什么。**Pid**就是进程ID。**Addr**是被追踪进程用户空间中的偏移量，当程序被指示写入数据时，**Data**会被写入的位置。它是被追踪进程的用户空间中的一个偏移量，从这个偏移量中读取一个字，并作为调用的结果返回。

父进程可以分支一个子进程，并且通过设置 **request** 的值为 *PTRACE_TRACEME*来跟踪这个子进程。父进程也可以通过*PTRACE_ATTACH*来跟踪一个已经存在的进程。关于**request**不同的值我们将在下面进行讨论。

#### ptrace 如何工作的

一旦ptrace被调用，它做的第一件事就是锁内核。然后在其返回之前解锁内核。让我们来看一下在不同的**request**值之间，**ptrace**的工作。

- ##### PTRACE_TRACEME

  当子进程被父进程跟踪时使用这个调用。就像上面说的一样，任何信号（除**SIGKILL**外），不论是从外部传递过来，还是进程通过**exec**调用产生，都会导致子进程**停止（stop）**并且让父进程决定怎么处理。ptrace()内部，唯一要检查的事情是当前进程的ptrace flag有没有被设置。如果没有，则授予权限设置flag。除**request**外，其他的参数都将被忽略。

- ##### PTRACE_ATTACH

  

- ##### PTRACE_DETACH

  

- ##### PTRACE_PEEKTEX, PTRACE_PEEKDATA, PTRACE_PEEKUSER

  

- ##### PTRACE_POKETEXT, PTRACE_POKEDATA, PTRACE_POKEUSER

- ##### PTRACE_SYSCALL, PTRACE_CONT

- ##### PTRACE_SINGLESTEP

- ##### PTRACE_KILL

#### Ptrace的返回值

一个成功的**Ptrace()**执行返回0。错误会返回-1，并设置**errno**。由于一个成功的PEEKDATA/PEEKTEXT的返回值可能是-1，最好检查一下**errno**。

- EPERM：无法跟踪请求的进程。 没有权限。
- ESRCH：请求的进程不存在或正在被跟踪。
- EIO：请求无效或从/向无效的内存区域进行读/写。
- EFAULT：读/写是从/向未真正映射的内存进行的。

真的很难区分 EIO 和 EFAULT 的原因。 这些返回几乎相同的错误。

#### 一个小例子（小栗子）



#### 结论

**Preace()**被大量用于调试工作。也用于系统调用跟踪。调试器创建新一个子进程，并通过父进程身份跟踪它。被调试的程序以子进程身份执行，并且，每个指令执行之后，父进程都可以检查正在运行的程序中寄存器的值。我将在本系列的下一部分演示利用 ptrace 多功能性的程序。 到那时再见。
