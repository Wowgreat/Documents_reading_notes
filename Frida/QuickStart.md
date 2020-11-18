#### 有必要一记的单词

| English         | Wiktiondary                                                  | Chinese    |
| --------------- | ------------------------------------------------------------ | ---------- |
| Instrumentation | [释义](https://en.wiktionary.org/wiki/instrumentation)<br />On a vehicle, dashboard gauges <br />monitoring engine functions and<br /> performance, along with other <br />essential functions | 监控仪器   |
| small-but-handy |                                                              | 小巧且方便 |
| hazardous       |                                                              | 危险的     |
| dodge           |                                                              | 避开       |



#### 欢迎你

本文档主要翻译自 [frida官网](https://frida.re/docs/home/)，这将会是一片Frida综合指导的中文版。在这里将会覆盖 **在命令行钟交互式方法追踪、在Frida的API上构建自己的工具 和 给你一些在参与frida自身发展上的建议**

#### 啥是FRIDA

你可以理解为本地app版本的**油猴**，或者更正式一点说，它是一个动态代码指令监控仪器工具包（原文作：instrumentation toolkit）。 它可以让你将自己的**JavaScript**片段或者其他的库 **注入** 到本地APP（Windows，macOS，IOS android，Linux等系统的本地应用）。Frida本身也提供了一些基于Frida API的简单工具。你可以直接使用，也可用用学习研究示例。

#### 为啥需要

。。。。。。

#### 为啥Python API，但是要用Javascript调试逻辑

Frida内核使用C编写，并且将Google的V8引擎注入目标进程钟，在该进程中你的js代码有**完全的权限访问内存**，Hook 函数 甚至是调用进程内的本地方法。在app和js（运行在目标进程内的js）之间的交流是双向的。使用Python和Js可以使用无风险的API快速开发。





#### 操作模式

- 注入
- 嵌入式
- 预加载











































