#### Python生成器概述

- 什么是生成器

  要了解生成器，得先了解三个概念

  - 迭代：重复执行一系列运算步骤，待满足预设条件后，停止

  - 迭代器：在可迭代对象上进行数据遍历访问的接口

  - 可迭代对象： 可以通过迭代器**迭代**出对象内数据的对象，例如，*List*、*Set*、*Dict*等


😉，理解了上面三个概念，我们接着来了解什么是**生成器**，生成器也是**可迭代对象**，有点类似与List、Set等对象，可以使用**for循环（迭代）**，但又有区别。跟着我咱们一起来看看吧。

```python
In [1]: [i for i in range(10)]
Out[1]: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9] # List

In [2]: (i for i in range(10))
Out[2]: <generator object <genexpr> at 0x0000024E738E7EB0> # generator
```

那么两者到底有什么差别呢。

```python
# 对列表连续迭代， 可以发现两次结果是一致的
In [3]: l = [i for i in range(10)]

In [4]: l
Out[4]: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

In [5]: for i in l:
   ...:     print(i)
   ...:
0
.
.
.
9

In [6]: for i in l:
   ...:     print(i)
   ...:
0
.
.
.
9
```

```python
# 对生成器进行两次迭代，可以发现，第二次输出结果为空
In [7]: g = (i for i in range(10))

In [8]: for i in g:
   ...:     print(i)
   ...:
0
.
.
.
9

In [9]: for i in g:
   ...:     print(i)
   ...:

In [10]:
```

```python
# 比较生成器和List所占内存的大小，可以看出生成器的大小始终保持不变
In [15]: sys.getsizeof([i for i in range(10)])
Out[15]: 184

In [16]: sys.getsizeof((i for i in range(10)))
Out[16]: 112

In [17]: sys.getsizeof([i for i in range(1000)])
Out[17]: 9016

In [18]: sys.getsizeof((i for i in range(1000)))
Out[18]: 112
```

通过生成器和List进行**for循环**和**所占内存空间大小的比较**，可以看出两者的差异。

**总结一下：生成器也是一种迭代器，但是你只能对其迭代一次  。这是因为它们并没有把所有的值存在内存中，而是在运行时生成值。**

#### Python关键字 yield/yield from

yield可以将函数转变为生成器。我们依旧用实际例子来理解

```python
def a():
    for i in range(10):
        print('我来自方法a')
        yield i

if __name__ == '__main__':
    for n in a():
        print('我是来自主函数的', n)

# 输出
我来自方法a
我是来自主函数的 0
.
.
.
我来自方法a
我是来自主函数的 9
```

- 当函数含有yield，会被系统默认为是一个生成器
- 执行到yield i， 返回**i**值以及整个**生成器处于暂停**的状态，并跳出当前函数，执行到调用返回值**i**的语句
- 当再次执行到这个含有yield的生成器函数时，会自动**立即执行**到**上次暂停的位置**继续执行，也就是从yield i这个语句继续执行

yield from，在一个生成器函数中调用另一个生成器函数时使用。

#### next、send函数

**next**函数控制执行步骤，启动生成器

**send**函数接受外部调用者的赋值，并将CPU交还给生成器函数

```python
def func():
    while True:
        num = yield
        print(num)


if __name__ == '__main__':
    g = func()
    next(g) # 启动生成器
    g.send(2)
# 输出 2
```

#### StopIteration异常

```python
# 触发 StopIteration 异常
In [28]: g = (i for i in range(1))

In [29]: next(g)
Out[29]: 0

In [30]: next(g)
---------------------------------------------------------------------------
StopIteration                             Traceback (most recent call last)
<ipython-input-30-e734f8aca5ac> in <module>
----> 1 next(g)

StopIteration:
```

StopIteration 异常表示生成器所有的数据已经被迭代完毕

#### yield生成器版本的 生产者-消费者模型

```python
import time


def consumer():
    cnt = yield
    while True:
        if cnt <= 0:
            cnt = yield cnt
        cnt -= 1
        print('consumer', cnt)


def producer(cnt):
    gen = consumer()
    next(gen)
    gen.send(cnt)
    while True:
        cnt += 1
        print('producer', cnt)
        cnt = gen.send(cnt)


if __name__ == '__main__':
    producer(0)

```
