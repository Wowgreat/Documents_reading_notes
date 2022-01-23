### 使用Ptrace进行进程跟踪

[原文地址](https://tldp.org/LDP/LG/issue81/sandeep.html)

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

四个参数中，**request**的值决定要做什么。**Pid**就是进程ID。**Addr**是被追踪进程用户空间中的偏移量，是程序被指示写入**Data**的位置。
