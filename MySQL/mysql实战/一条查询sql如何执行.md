### 一条SQL语句如何执行

首先我们来看一个查询语句

```mysql
select * from T where ID=10;
```

你我都知道输入一个查询语句，会返回一个结果，却不知道在mysql中这条查询语句怎样运行。

今天我带你一起探索这一过程。

![image-20210220171249926](http://imgs.maopuyu.com/image-20210220171249926.png)

>  **连接器**：管理连接，权限验证
>
> **分析器**：词法分析
>
> **优化器**： 执行计划优化、确定（选择合适的索引等操作）
>
> **执行器**：操作引擎，返回结果
>
> **存储器**：顾名思义，存储数据，提供读写接口
>
> **查询缓存**: mysql8.0将查询缓存模块取消掉了

大体来说，MySQL 可以分为 Server 层和存储引擎层两部分。需要注意的是，MySQL5.5.5版本开始，**InnoDB**成为默认存储引擎。

#### 连接器

```bash
mysql -h$ip -P$port -u$user -p
```

输完命令之后，你就需要在交互对话里面输入密码。虽然密码也可以直接跟在 -p 后面写在命令行中，但这样可能会导致你的密码泄露。

> 如果用户名或密码不对，你就会收到"Access denied for user"的错误提示；

> 如果验证通过，连接器会到权限表里面查出你拥有的权限。之后，这个链接里面的权限判断逻辑，都将依赖于此时读到的权限。所以，在一个用户建立连接后，你用超级管理员修改该用户的权限，是对已建立的连接没有作用的。

在建立连接后，执行 **show processlist** 命令查看当前MySQL服务器所有的链接。文本中这个图是 show processlist 的结果，其中的 Command 列显示为“Sleep”的这一行，就表示现在系统里面有一个空闲连接。

```mysql
mysql> show processlist;
+----+-----------------+-----------+------+---------+------+------------------------+------------------+
| Id | User            | Host      | db   | Command | Time | State                  | Info             |
+----+-----------------+-----------+------+---------+------+------------------------+------------------+
|  5 | event_scheduler | localhost | NULL | Daemon  |  191 | Waiting on empty queue | NULL             |
| 10 | root            | localhost | NULL | Query   |    0 | init                   | show processlist |
+----+-----------------+-----------+------+---------+------+------------------------+-----------
```

客户端如果太长时间没动静，连接器就会自动将它断开。这个时间是由参数 wait_timeout 控制的，默认值是 8 小时。

```mysql
mysql> show global variables like '%mysqlx_wait_timeout';
+---------------------+-------+
| Variable_name       | Value |
+---------------------+-------+
| mysqlx_wait_timeout | 28800 |
+---------------------+-------+
```



如果链接断开后客户端再次发送请求，会提示：**Lost connection to MySQL server during query。**

> 这里提一嘴，在SpringBoot中，可以通过配置 **main\resources\application.yml** 文件解决MySQL连接器中断问题。这里略微展示一下配置，不做深入讨论。
>
> ```yaml
> # 流量不大的网站
> spring:
>   datasource:
>     hikari:
>       maximum-pool-size: 6
>       connectionTimeout: 120000
>       idleTimeout: 90000
>       max-lifetime: 120000
> ```

#### 查询缓存

MySQL-8.0已经删除这个模块，所以这里就不再讲了。

#### 分析器

分析器先会做“词法分析”。你输入的是由多个字符串和空格组成的一条 SQL 语句，MySQL 需要识别出里面的字符串分别是什么，代表什么。

MySQL 从你输入的"select"这个关键字识别出来，这是一个查询语句。它也要把字符串“T”识别成“表名 T”，把字符串“ID”识别成“列 ID”。

做完了这些识别以后，就要做“语法分析”。根据词法分析的结果，语法分析器会根据语法规则，判断你输入的这个 SQL 语句是否满足 MySQL 语法。

如果你的语句不对，就会收到“You have an error in your SQL syntax”的错误提醒，比如下面这个语句 select 少打了开头的字母“s”。

```mysql
mysql> elect * from t where ID=1;
 
ERROR 1064 (42000): You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near 'elect * from t where ID=1' at line 1
```

一般语法错误会提示第一个出现错误的位置，所以你要关注的是紧接“use near”的内容。

#### 优化器

经过了分析器，MySQL 就知道你要做什么了。而优化器是决定怎么执行这个动作。例如，表里有多个索引的时候，选择用哪个索引；或者在一个语句有多表关联（join）的时候，决定各个表的连接顺序。

例如：

```mysql
mysql> select * from t1 join t2 using(ID)  where t1.c=10 and t2.d=20;
```

- 既可以先从表 t1 里面取出 c=10 的记录的 ID 值，再根据 ID 值关联到表 t2，再判断 t2 里面 d 的值是否等于 20。
- 也可以先从表 t2 里面取出 d=20 的记录的 ID 值，再根据 ID 值关联到 t1，再判断 t1 里面 c 的值是否等于 10。

这两种执行方法的逻辑结果是一样的，但是执行的效率会有不同，而优化器的作用就是决定选择使用哪一个方案。

#### 执行器

举例：

```mysql
mysql> select * from T where ID=10;
```

对于没有ID索引的表执行顺序是

- 调用 InnoDB 引擎接口取这个表的第一行，判断 ID 值是不是 10，如果不是则跳过，如果是则将这行存在结果集中；
- 调用引擎接口取“下一行”，重复相同的判断逻辑，直到取到这个表的最后一行。
- 执行器将上述遍历过程中所有满足条件的行组成的记录集作为结果集返回给客户端。

InnoDB中非主键索引使用的是B-Tree数据结构，而主键索引使用的是B+Tree。对于有ID索引的表，则按照对应的树结构进行查询。这又是另外的课题，我们会在接下来的文章中涉及到。

**整理不容易点个再看吧**

![qrcode_for_gh_1f012feb4af9_258](http://imgs.maopuyu.com/qrcode_for_gh_1f012feb4af9_258.jpg)

