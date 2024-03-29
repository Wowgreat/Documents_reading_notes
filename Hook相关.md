#### Hook相关的知识点

#### 术语

- 劫持函数调用
- 动态二进制插桩（DBI）



- 注入进程: 

  - ptrace
  - dlopen

- hook 目标函数: 

  - Java Hook
    - Static Field Hook：静态成员hook
    - Method Hook：函数hook
  - Native So Hook
    - GOT Hook：全局偏移表hook
    - SYM Hook：符号表hook
    - Inline Hook：函数内联hook

- 执行自身代码

  - 获取敏感信息

  - 修改返回值

#### 本质

Hook技术无论对安全软件还是恶意软件都是十分关键的一项技术，其本质就是劫持函数调用。但是由于处于Linux 用户态，每个进程都有自己独立的进程空间，所以必须先注入到所要Hook 的进程空间，修改其内存中的进程代码，替换其过程表的符号地址。

#### Hook分类

![](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/20180806142903509.png)

#### 步骤

- 注入进程
- hook目标函数
- 执行自身代码





