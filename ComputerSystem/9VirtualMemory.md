单词

| English      | 中文                 |
| ------------ | -------------------- |
| poses        | 姿势（n）；提出（v） |
| demand       | 需求                 |
| out of space | 空间不足             |
| vulnerable   | 脆弱的               |
| corruption   | 破坏；破败；腐败     |
| inadvertent  | 疏忽的               |
| bewilder     | 迷惑                 |
|              |                      |
|              |                      |



### 简介

进程之间共享CPU和主内存。共享主内存存在一些特殊的挑战。

1、随着对CPU的需求增加，进程会合理地平滑地降低速度。但是过多的进程需要太多的主内存，将导致一些进程不能够运行。当一个程序遇上空间不足时，那它运气不好。

2、主内存是非常容易受到破坏的。假设一个进程不经意写了一些数据到另一个进程用过的内存里。另一个进程可能会以与程序逻辑无关的令人困惑的方式失败。

为了结局以上两个问题，现代系统提供了一个

### 9.1 物理/虚拟寻址


