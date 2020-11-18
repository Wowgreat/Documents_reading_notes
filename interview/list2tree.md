#### 要求

```python
将
industry_list = [
    {
        "parent_ind": "女装",
        "name": "连衣裙"
    },
    {
        "name": "女装"
    },
    {
        "parent_ind": "女装",
        "name": "半身裙"
    },
    {
        "parent_ind": "女装",
        "name": "A字裙"
    },
    {
        "name": "数码"
    },
    {
        "parent_ind": "数码",
        "name": "电脑配件"
    },
    {
        "parent_ind": "电脑配件",
        "name": "内存"
    },
    {
        "parent_ind": "内存",
        "name": "金士顿"
    },
]
变换为
{
   "数码": {
        "电脑配件": {
            "内存" : {}
        }
    },
   "女装" : {
        "连衣裙": {},
        "半身裙": {},
        "A字裙": {}
    }
}
```



#### 正文（编程语言为 python）

以上是一道面试题。乍一看，感觉不难，仔细看，感觉有点意思。做一做，发现，这题还是蛮难的。

##### 难点：

- 树的深度不确定。
- 所给的list是无须的

基于以上两点，我们无法较为快速的将一个 **parent_ind** 节点放到 **树** 中合适的节点中。

但是，我发现了python以下特性

```python
In [45]: r={}

In [46]: d={}
    
In [47]: d['a']={}

In [48]: d['b']={}

In [49]: d['c']={}

In [50]: d['d']={}

In [51]: d['e']={}

In [52]: r['a'] = d['a']

In [53]: r
Out[53]: {'a': {}}

In [54]: d['a']['b']=d['b']

In [55]: r
Out[55]: {'a': {'b': {}}}

In [56]: d['b']['c']=d['c']

In [57]: r
Out[57]: {'a': {'b': {'c': {}}}}

In [58]: d['d']['e']=d['e']

In [59]: r
Out[59]: {'a': {'b': {'c': {}}}}

In [60]: d['c']['d']=d['d']

In [61]: r
Out[61]: {'a': {'b': {'c': {'d': {'e': {}}}}}}
    
In [62]: id(r['a'])
Out[62]: 1348618724480

In [63]: id(d['a'])
Out[63]: 1348618724480
```

由以上演示结果，我们可以看到。当变量 **A** 指向另一个 **dict变量B** 时，其变量的指针与该dict变量始终保持不变。也就是说**A**会随着**B**的变化而变化。

> #### python变量分为可变变量，和，不可变变量。当A = B（b为可变变量，如list，dict）时，A会随着B的变化而变化

这个特性可以让我们不用顾虑以上两个难点。我们只需要改变**B变量(此B变量随着每一步执行而指向不同的对象)** ，使其逐渐呈现出要求所需要的样子，那么A变量亦会达到最终的要求。

```python
# 例如 我们需要  r={'a': {'b': {'c': {}}}}
# 给出  d = {'a': {}, 'b': {}, 'c': {}}

In [10]: d['b']['c']=d['c']
    
In [13]: d['a']['b']=d['b']

# 那么得到 d['a']['b']=d['b']={'c': {}}
# 故 d['a'] = {'b': {'c': {}}}
# 执行
In [20]: r['a'] = d['a']
    
In [21]: r
Out[21]: {'a': {'b': {'c': {}}}}
```

由此我们得到此题的解决代码，如下

#### 解题代码

```python
def convert_format():
    d = dict()
    root = dict()
    for ele in industry_list:
        if ele['name'] not in d:
            d[ele['name']] = {}

    for ele in industry_list:
        if 'parent_ind' not in ele:
            root[ele['name']] = d[ele['name']]
        else:
            d[ele['parent_ind']][ele['name']] = d[ele['name']]
    print(root)
```



####  题后思考，python对象（变量）的指针是如何变化的

想起了在抖音上的一段视频中的代码

```
In [23]: a = 'hello@'

In [24]: b = 'hello@'

In [25]: a==b
Out[25]: True

In [26]: a is b
Out[26]: False

In [27]: a = 1024

In [28]: b = 1024

In [29]: a is b
Out[29]: False

In [30]: a = 1

In [31]: b = 1

In [32]: a is b
Out[32]: True

In [33]: a = 'hello'

In [34]: b = 'hello'

In [35]: a is b
Out[35]: True

In [36]: a = []

In [37]: b = []

In [38]: a is b
Out[38]: False

In [39]: a = b

In [40]: a is b
Out[40]: True

In [41]: b.append(1)

In [42]: a
Out[42]: [1]
```

> - is对比的时变量的指针是否一致，== 对比的是变量的值
>
> - 整数在程序中的使用非常广泛，Python为了优化速度，使用了小整数对象池， 避免为整数频繁申请和销毁内存空间。
>
>   Python 对小整数的定义是 [-5, 256] 这些整数对象是提前建立好的，不会被垃圾回收。在一个 Python 的程序中，无论这个整数处于LEGB中的哪个位置，
>
>   所有位于这个范围内的整数使用的都是同一个对象。同理，单个字母也是这样的。



