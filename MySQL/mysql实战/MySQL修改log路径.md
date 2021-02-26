#### 实验环境

- Ubuntu 18.04
- MySQL 8.0

MySQL有其默认的日志保存位置：

```mysql
mysql> show variables like "log_%";
+---------------------------------+-------------------------+
| Variable_name                   | Value                   |
+---------------------------------+-------------------------+
| log_bin                         | ON                      |
| log_bin_basename                | /var/lib/mysql/binlog   |
| log_bin_index                   | /var/lib/mysql/binlog.index |
| log_bin_trust_function_creators | OFF                     |
| log_bin_use_v1_row_events       | OFF                     |
+---------------------------------+-------------------------+
```

但是，某些情况下，需要更高某些日志的位置，便于管理。接下来我就告诉大家如何变更MySQL日志的位置。

#### 查看默认路径的权限

```shell
$ sudo ls -ld /var/lib/mysql/
drwxr-x--- 7 mysql mysql 4096 Feb 25 07:37 /var/lib/mysql/
```

#### 创建新的日志文件夹，并修改权限

```shell
$ sudo mkdir -p /log/msyql

# sudo chown -R {用户名}:{用户组} /log/
$ sudo chown -R mysql:mysql /log/
```

#### 【注意】修改强制访问系统配置

```shell
# 在文件末尾添加一些内容
$ sudo vim /etc/apparmor.d/usr.sbin.mysqld
```

使mysqld进程可以读写log/mysql及其所属下的文件

```conf
/log/mysql/ r,
/log/mysql/** rwk,
```



#### 修改配置文件

```shell
$ sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf
```

将文件修改为下面的样子

```
# /etc/mysql/mysql.conf.d/mysqld.cnf
[mysqld]
pid-file        = /var/run/mysqld/mysqld.pid
socket          = /var/run/mysqld/mysqld.sock
datadir         = /var/lib/mysql
log-error       = /var/log/mysql/error.log

log-bin         = /log/mysql/binlog
```

我们接下来重启mysql

```shell
$ sudo service mysql restart
```

支持我们成功修改了MySQL Binlog的保存位置。你可以看到 **/log/mysql** 文件夹下已经生成了log文件

接下来我们登录到mysql验证一下

```mysql
mysql> show variables like "log_bin%";
+---------------------------------+-------------------------+
| Variable_name                   | Value                   |
+---------------------------------+-------------------------+
| log_bin                         | ON                      |
| log_bin_basename                | /log/mysql/binlog       |
| log_bin_index                   | /log/mysql/binlog.index |
| log_bin_trust_function_creators | OFF                     |
| log_bin_use_v1_row_events       | OFF                     |
+---------------------------------+-------------------------+
```

可以看出，MySQL的**log_bin_basename**已由 **/var/lib/mysql/binlog** 改编为 **/log/mysql/binlog **，**log_bin_index**也有相应的改变

