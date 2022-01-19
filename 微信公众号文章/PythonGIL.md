上一篇文章我们留下了一个问题：

- 在进行hash计算中，Python的多线程为什么比单线程还要慢？换句话说，为什么**Python的多线程程对hash运算没有效果**？

那就是因为**Python的多线程是伪多线程**。这篇文章，我就带领大家**验证**Python的多线程是为多线程。以及在什么情况下，为多线程也能起到提高程序性能的作用。

### GIL介绍

 **Global interpreter lock (GIL)，全局解释器锁** ，要解释GIL我们首先得了解什么是锁。

**锁机制：**并发访问共享资源，如果不加锁，可能会导致数据不一致问题，通常为了解决并发访问问题，我们都会在访问共享资源之前加锁，保证同一时刻只有一个线程访问。

举个例子，线程A和线程B同时对一个变量C进行加1操作。在某一个时刻，A读取了C的值6，紧接着被挂起。CPU被B占用，此时B也读了C的值，也为6并执行完加1操作。那此时C的值为7。这时CPU流转到线程A，A被唤醒，执行完加1操作，由于在被挂起前已经读取了C的值为6，那么此时A线程A也将C的值变为7。这就背离了我们使用两个线程的初衷。为了避免这种情况，就必须引进锁的概念。

- 扩展阅读

  ```c
  /*https://github.com/python/cpython/blob/main/Python/ceval_gil.h */
  
  static void _gil_initialize(struct _gil_runtime_state *gil)
  {
      _Py_atomic_int uninitialized = {-1};
      gil->locked = uninitialized;
      gil->interval = DEFAULT_INTERVAL;
  }
  
  static int gil_created(struct _gil_runtime_state *gil)
  {
      return (_Py_atomic_load_explicit(&gil->locked, _Py_memory_order_acquire) >= 0);
  }
  
  static void create_gil(struct _gil_runtime_state *gil)
  {
      MUTEX_INIT(gil->mutex);
  #ifdef FORCE_SWITCHING
      MUTEX_INIT(gil->switch_mutex);
  #endif
      COND_INIT(gil->cond);
  #ifdef FORCE_SWITCHING
      COND_INIT(gil->switch_cond);
  #endif
      _Py_atomic_store_relaxed(&gil->last_holder, 0);
      _Py_ANNOTATE_RWLOCK_CREATE(&gil->locked);
      _Py_atomic_store_explicit(&gil->locked, 0, _Py_memory_order_release);
  }
  ```

从GIL锁的源码中，我们大致可以看出其本质是互斥锁。而且是**全局的互斥锁**。所以，当Python多线程被执行的时候，实际上只有一个线程可以使用CPU。



**为什么网络请求和保存文件的操作使用Python多线程会快很多呢？**

这是因为这两者都属于**IO密集型操作**，而IO操作是不占用CPU的，交由**IO总线**完成。所以在线程调用完读写命令后，紧接着被挂起，也是不影响IO总线继续执行读写的命令的。



### 体验GIL

这里我们使用两个程序验证GIL锁的存在

```python
# countdown.py
import threading
from utils import Timer

cnt = 10000000

def cuntdown():
    global cnt
    while cnt > 0:
        cnt -= 1


def single_thread():
    tick = Timer()
    cuntdown()
    print(f'single_thread 共耗时 {tick.tick()}s')


def multi_thread():
    tick = Timer()
    t1 = threading.Thread(target=cuntdown)
    t2 = threading.Thread(target=cuntdown)
    t1.start()
    t2.start()
    t1.join()
    t2.join()  # 等待此线程结束
    print(f'multi_thread 共耗时 {tick.tick()}s')


if __name__ == '__main__':
    # single_thread()
    multi_thread()

```

- **猜想**

  单线程是一个线程在使用CPU运算**循环减1**的操作，多线程是2个线程使用CPU运算**循环减1**的操作。如果是真并发编程的话，多线程肯定用的时间会比单线程少。

- **实验结果**(计5次运行结果的平均值)

  | 单线程 | 多线程 |
  | ------ | ------ |
  | 1.04s  | 1.55s  |

由实验结果可以看出，多线程模式下并没有达到我们预期的效果。反而由于线程间切换耗时，降低了程序的性能。间接可以判断python的多线程是伪多线程。

接下来我们再通过一个更直观的例子来证明python是伪多线程。（实验环境为 **2核2线程Ubuntu18.04 虚拟机**）

```python
import threading

cnt = 0


def consumer():
    global cnt
    while True:
        cnt -= 1


def producer():
    global cnt
    while True:
        cnt += 1


if __name__ == '__main__':
    t1 = threading.Thread(target=producer)
    t2 = threading.Thread(target=consumer)
    t1.start()
    t2.start()

```

如果python的多线程是真并发，那么运行上面的程序，消费者线程和生产者线程同时运行，将会使2核2线程的cpu完全被使用。那么我们运行一下看看结果吧。

```bash
(base) vagrant@ubuntu-bionic:~/code/pythonCoroutine$ sar -u 1 100
Linux 4.15.0-162-generic (ubuntu-bionic) 	11/24/21 	_x86_64_	(2 CPU)

02:49:34        CPU     %user     %nice   %system   %iowait    %steal     %idle
02:49:35        all     50.00      0.00      0.00      0.00      0.00     50.00
02:49:36        all     50.25      0.00      0.00      0.00      0.00     49.75
02:49:37        all     49.75      0.00      0.00      0.00      0.00     50.25
02:49:38        all     50.25      0.00      0.00      0.00      0.00     49.75
02:49:39        all     50.00      0.00      0.00      0.00      0.00     50.00
02:49:40        all     49.75      0.00      0.00      0.00      0.00     50.25
02:49:41        all     50.00      0.00      0.00      0.00      0.00     50.00
02:49:42        all     50.25      0.00      0.00      0.00      0.00     49.75
02:49:43        all     50.00      0.00      0.00      0.00      0.00     50.00
```

可以很清楚的发现 **%idle（CPU空置率）**是50%左右，也就是说在同义时刻，只有一个核心被使用。从这个例子，我们可以直观的判断，Python的多线程并不是真正的多线程。
