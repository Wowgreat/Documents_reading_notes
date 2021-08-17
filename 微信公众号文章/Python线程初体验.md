#### 前言

> 这将是一个系列，一个关于**进程、线程和 协程**的系列。
>
> 主要用于：**回顾和复习以往所学的知识** 以及 **希望这点经验能够帮助正在学习编程的你**

#### 查看线程ID

创建文件 0809.py

```python
import time
import threading

def loop():
    while True:
        print('thread id is {}'.format(threading.get_native_id()))
        time.sleep(3)
        
if __name__ == '__main__':
    loop()
```

在第一个终端窗口中执行

```shell
$ python 0809.py 
thread id is 3344
thread id is 3344
thread id is 3344
······
```

在第二个终端窗口中执行

```shell
ps -ef | grep 'python 0809.py'
vagrant   3344  3117  0 16:26 pts/1    00:00:00 python 0809.py
vagrant   3662  3451  0 16:30 pts/0    00:00:00 grep --color=auto python 0809.py
```

你会发现其进程ID也是 3344和线程ID一致。这是因为Linux中规定，当一个进程中只有一个线程的情况下，线程ID等于进程ID。或则说，进程的第一个线程（主线程）的ID等于进程ID。 

#### 经典的生产者/消费者模型（也有人称之为，发布/订阅模型）

```python
# 0809.py 
import time
import threading

count = 0

def consumer():
    global count
    while True:
        if count <= 0:
            continue
        count = count - 1
        print(f'count is {count}, consumer thread id is {threading.get_native_id()}')
        time.sleep(2)

def producer():
    global count
    while True:
        count = count + 1
        print(f'count is {count}, producer thread id is {threading.get_native_id()}')
        time.sleep(1)
        
if __name__ == '__main__':
    tp = threading.Thread(target=producer)
    tc = threading.Thread(target=consumer)
    tp.start()
    tc.start()

```

执行命令 *python 0809.py* 

```shell
$ python 0809.py 
count is 1, producer thread id is 3785
count is 0, consumer thread id is 3786
count is 1, producer thread id is 3785
count is 0, consumer thread id is 3786
count is 1, producer thread id is 3785
count is 2, producer thread id is 3785
count is 1, consumer thread id is 3786
count is 2, producer thread id is 3785
```

可以发现，两个线程并非严格交替执行，而是随机执行。

我们再来查看一下相关的进程和线程

```shell
$ ps -ef | grep 'python 0809.py'
vagrant   3784  3117  0 17:24 pts/1    00:00:00 python 0809.py
vagrant   3789  3451  0 17:24 pts/0    00:00:00 grep --color=auto python 0809.py

$ ps -T -p 3784
  PID  SPID TTY          TIME CMD
 3784  3784 pts/1    00:00:00 python
 3784  3785 pts/1    00:00:00 python
 3784  3786 pts/1    00:00:00 python
```

可以看出该进程中有三个线程，分别是主线程 3784 和两个子线程 3785（producer）、3786（consumer）

今天我们就先讲到这里，重点掌握：

1、如何在python代码中和shell终端中查看线程id 进程ID 以及进程中包含的线程。

2、理解生产/消费者模型，因为这个模型会在接下来的学习中被多次提到

接下来的文章我们将介绍 **python单线程下载脚本、单线程hash处理脚本**，这两个脚本分别帮助我们理解**IO密集型程序**和**CPU密集型程序** 