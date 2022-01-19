#### 前情回顾

上一篇文章为大家介绍了python多线程下对下载和保存模块的性能提升。可以看出，多线程对下载、保存等高IO过程的性能提升是显著的。

为了广大同学能够跟着我一起码代码、一起学习和体验，我今天把代码整理了一下，发布在gitee上。获取完整代码有利于更好地理解和学习。

**获取方式如下**



##### 代码内容

- 下载模块（单线程、多线程、多进程）
- 写入模块，也叫保存模块（单线程、多线程、多进程）
- Hash计算模块（单线程、多线程、多进程）



#### 多进程核心代码

这里我们引入python官方提供的`ProcessPoolExecutor`来实现进程池。

```python
import requests
from modules.executors import process_pool_executor as pp
from modules.base import BaseModule


class Mp3Downloader(BaseModule):
    def __init__(self):
        super(Mp3Downloader, self).__init__()

    def process_multi_process(self, tasks):
        '''多进程下载模块实现'''
        response_list = []
        task_list = []
        for task in tasks:
            task = pp.submit(self.process, task)
            task_list.append(task)
        for task in task_list:
            content = task.result()
            response_list.append(content)
        return response_list
```

```python
from modules.base import BaseModule
from modules.executors import process_pool_executor as pp


class Storager(BaseModule):
    path = None

    def __init__(self):
        super(Storager, self).__init__()

    def process_multi_process(self, tasks):
         '''多进程写入模块实现'''
        task_list = []
        for index, item in enumerate(tasks):
            task = pp.submit(self.process, (index, item))
            task_list.append(task)
        for task in task_list:
            task.result()
```

```python
import hashlib

from modules.base import BaseModule
from modules.executors import process_pool_executor as pp


class Hasher(BaseModule):
    def process(self, task):
        return hashlib.md5(task).hexdigest()

    def process_multi_process(self, tasks):
        '''多进程Hash模块'''
        md5_list = []
        task_list = []
        for task in tasks:
            pp_task = pp.submit(self.process, task)
            task_list.append(pp_task)
        for task in task_list:
            md5 = task.result()
            md5_list.append(md5)
        return md5_list

```

#### 性能对比

> 此此数据是在多次运行之后求平均值得出的

| 下载模块           | 保存模块 | Hash模块 |模式|
| ------------------ | -------- | -------- |-------- |
| 2.852427s | 0.010917s | 0.001993s |单线程|
| 0.642283s | 0.004987s | 0.002993s |多线程|
| 0.947492s | 0.018924s | 0.249332s |多进程|

可以看出，在IO处理上  多线程  > 多进程  >  单线程

> 注意：例子中因为文件小，每个文件写入时间都很短。多进程需要初始化进程、进程切换等开销所以性能上就不如单线程。但是如果每个文件都比较大的情况下，处理单个文件写入的时间就会变长，到那时，多进程的性能就会优于单线程。
>
> 从这里也可以看出，处理的业务场景不同，所需要选择的技术也应该跟着变化。当然，做选择时还要考虑到程序的的复杂度，投入产出比等因素。

而在 **Hash**模块上，我们可以明显的看出，  单线程 > 多线程  > 多进程，可以看出并发并没有起到预期的效果，这是为什么呢？这一点我们在下一篇文章中会给出答案。





