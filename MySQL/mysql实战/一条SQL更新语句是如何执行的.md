- 首先我们创建一个数据库用于实验

  ```mysql
  mysql> CREATE DATABASE  `test` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
  ```

- 创建一个表**T**

  ```mysql
  mysql> use test;
  mysql> create table T(ID int primary key, c int);
  ```

- 插入一条数据

  ```mysql
  INSERT INTO T (ID, C) VALUES (1, 1);
  ```

#### 接下来，我们来看如何更新SQL语句的执行流程

```mysql
mysql> update T set c=c+1 where ID=1;
```

- 首先查询流程肯定要先运行一遍；
- 接下来，分析器会通过词法和语法解析知道这是一条更新语句。优化器决定要使用 ID 这个索引。然后，执行器负责具体执行，找到这一行，然后更新。

与查询流程不一样的是，更新流程还涉及两个重要的日志模块，它们正是我们今天要讨论的主角：redo log（重做日志）和 binlog（归档日志）。

#### Redo log

首先我们先来 **Redo log**，在更新过程中，是这样子的流程

![image-20210223155812843](http://imgs.maopuyu.com/image-20210223155812843.png)

**InnoDB** 并不会直接将更新写道磁盘中，而是先将更新操作记录写道redo log中，并更新内存，然后在合适的时间（空闲时间），再将记录同步到磁盘中。这就是WAL技术，**Write-Ahead Logging**，先写日志，再写磁盘。这么做的目的是为了减少IO操作时间。

如果操作记录特别多，将redolog区域占满，那么，InnoDB引擎会将一部分记录更新到磁盘，然后将此部分记录从redolog中删除。

InnoDB 的 redo log 是固定大小的，比如可以配置为一组 4 个文件，每个文件的大小是 1GB，那么这块“粉板”总共就可以记录 4GB 的操作。从头开始写，写到末尾就又回到开头循环写，如下面这个图所示。

![image-20210223155230296](http://imgs.maopuyu.com/image-20210223155230296.png)

write pos 是当前记录的位置，一边写一边后移，写到第 3 号文件末尾后就回到 0 号文件开头。checkpoint 是当前要擦除的位置，也是往后推移并且循环的，擦除记录前要把记录更新到数据文件。

write pos 和 checkpoint 之间的是**redo log**上还空着的部分，可以用来记录新的操作。当write pos 追上 checkpoint，表示**redo log**没有剩余空间了。得停下来先擦掉一些记录，把 checkpoint 推进一下。

有了 redo log，InnoDB 就可以保证即使数据库发生异常重启，之前提交的记录都不会丢失，这个能力称为**crash-safe**。

> nnodb_flush_log_at_trx_commit 这个参数设置成 1 的时候，表示每次事务的 redo log 都直接持久化到磁盘。这个参数我建议你设置成 1，这样可以保证 MySQL 异常重启之后数据不丢失。

#### binlog

上面涉及到的 **redo log**是InnoDB引擎特有的日志。之前我们讲过MySQL分会两层： Server 层&引擎层。Binlog是Server层实现的。这两种log有以下3点不同

1. redo log 是 InnoDB 引擎特有的；binlog 是 MySQL 的 Server 层实现的，所有引擎都可以使用。
2. redo log 是物理日志，记录的是“在某个数据页上做了什么修改”；binlog 是逻辑日志，记录的是这个语句的原始逻辑，比如“给 ID=1 这一行的 c 字段加 1 ”。
3. redo log 是循环写的，空间固定会用完；binlog 是可以追加写入的。“追加写”是指 binlog 文件写到一定大小后会切换到下一个，并不会覆盖以前的日志。

> 最开始 MySQL 里并没有 InnoDB 引擎。MySQL 自带的引擎是 MyISAM，但是 MyISAM 没有 crash-safe 的能力，binlog 日志只能用于归档。而 InnoDB 是另一个公司以插件形式引入 MySQL 的，既然只依靠 binlog 是没有 crash-safe 能力的，所以 InnoDB 使用另外一套日志系统——也就是 redo log 来实现 crash-safe 能力。

OK，我们再来看**update**语句的执行流程：

1. 执行器先找引擎取 ID=2 这一行。ID 是主键，引擎直接用树搜索找到这一行。如果 ID=2 这一行所在的数据页本来就在内存中，就直接返回给执行器；否则，需要先从磁盘读入内存，然后再返回。
2. 执行器拿到引擎给的行数据，把这个值加上 1，比如原来是 N，现在就是 N+1，得到新的一行数据，再调用引擎接口写入这行新数据。
3. 引擎将这行新数据更新到内存中，同时将这个更新操作记录到 redo log 里面，此时 redo log 处于 prepare 状态。然后告知执行器执行完成了，随时可以提交事务。
4. 执行器生成这个操作的 binlog，并把 binlog 写入磁盘。
5. 执行器调用引擎的提交事务接口，引擎把刚刚写入的 redo log 改成提交（commit）状态，更新完成。

> sync_binlog 这个参数设置成 1 的时候，表示每次事务的 binlog 都持久化到磁盘。这个参数我也建议你设置成 1，这样可以保证 MySQL 异常重启之后 binlog 不丢失。

#### 学以致用，我们来看如何利用binlog进行数据库恢复

首先我们来看Binlog相关的命令

```mysql
# 查询 BINLOG 格式
>mysql show VARIABLES like 'binlog_format';

# 查询 BINLOG 位置
>mysql show VARIABLES like 'datadir';

# 查询当前数据库中 BINLOG 名称及大小
>mysql show binary logs;

# 查看 master 正在写入的 BINLOG 信息
>mysql show master status\G;

# 通过 offset 查看 BINLOG 信息
>mysql show BINLOG events in 'mysql-bin.000034' limit 9000,  10;

# 通过 position 查看 binlog 信息
>mysql show BINLOG events in 'mysql-bin.000034' from 1742635 limit 10;
```

使用 `show BINLOG events` 的问题：

- 使用该命令时，如果当前 binlog 文件很大，而且没有指定 `limit`，会引发对资源的过度消耗。因为 MySQL 客户端需要将 binlog 的全部内容处理，返回并显示出来。为了防止这种情况，mysqlbinlog 工具是一个很好的选择。

  



![](http://imgs.maopuyu.com/image-20210223164949346.png)