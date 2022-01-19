### 【官方教程】使用APT安装MySQL指南

##### MySQL全新安装步骤

以下步骤假设没有任何版本的MySQL安装在你的系统上。

##### 增加MySQL APT仓库

- 下载 MySQL APT仓库，[点击](https://dev.mysql.com/downloads/repo/apt/)

  > 注意，目前下载MySQL APT仓库需要登录Oracel帐号，如果你没有可以在本公众号内回复mysqlapt获取

- 安装下载的MySQL APT仓库

  ```shell
  sudo dpkg -i /PATH/version-specific-package-name.deb
  //例如我这里就是
  sudo dpkg -i mysql-apt-config_0.8.16-1_all.deb
  ```

  ![1](http://imgs.maopuyu.com/1.png)

  上图表示三个配置（默认配置）

  - MySQL Server & Cluster 当前选择的是 mysql-8.0
  - MySQL Tool & Connetor 当前的选择是 Enable（激活）
  - MysSQL Preview Packages 当前的选择是 Disabled（禁用）

  如果你想更改默认配置，可以挨个点选进去，进行配置

- 执行更新apt仓库的操作，执行后，MySQL-8.0的安装包将会被加入到apt仓库中

  ```shell
  sudo apt-get update
  ```

##### 使用APT安装MySQL

```shell
sudo apt-get install mysql-server
```

![2](http://imgs.maopuyu.com/2.png)

输入密码，选择 **<ok>**，重复两次就可以了。

![image-20210221161127948](https://gitee.com/dashugan/article-imgs/raw/master/image-20210221161127948.png)

这里，我们选择 **Use Strong Password Encryption（强密码加密）**，到此，MySQL安装就算完成了。

我们来执行一下mysql链接

```shell
#mysql -h$ip -P$port -u$user -p
#由于我们的MySQLServer和MySQLClient都在一台虚拟机中，且使用默认端口3306，所以我们可以省略 ip和port
vagrant@ubuntu-bionic:~$ mysql -uroot -p
Enter password: 
mysql> 
```

##### 修改MySQL配置，使其允许远程访问

默认情况下，MySQLServer仅允许本机访问。

但在一些情况下我们需要MySQLServer允许其他计算机访问，例如内网中的其他机器，亦或者是开发环境中，为了方便开允许所有的机器连接。

那这些操作怎么做呢，我们继续看

```shell
vagrant@ubuntu-bionic:~$ mysql -uroot -p
Enter password: 
mysql> use mysql;
mysql> CREATE USER 'remoteuser'@'%' IDENTIFIED BY 'iamsuperman';
mysql> GRANT ALL ON *.* TO 'remoteuser'@'%';
mysql> ALTER USER 'remoteuser'@'%' IDENTIFIED WITH mysql_native_password BY 'iamsuperman';
#如果你想只允许特定的Ip连接MySQLServer，那么可以这么写
#mysql> ALTER USER 'remoteuser'@'{指定ip}' IDENTIFIED WITH mysql_native_password BY 'iamsuperman';
mysql> FLUSH PRIVILEGES; 
mysql> exit
(base) vagrant@ubuntu-bionic:~$ sudo service mysql restart
```

重启MySQLServer后，我们发现下图情况，表示我们可以使用remoteuser进行远程连接。

> 由于我们我们执行了 GRANT ALL ON *.* TO 'remoteuser'@'%'; 语句，所以remoteuser等同于root用户，拥有所有权限

![image-20210221163440865](http://imgs.maopuyu.com/image-20210221163440865.png)

##### 忘记root密码怎么办

一不小心，忘了密码怎么办呢？肯定不是凉拌就咸菜。

不要着急咱们继续往下讲

- 第一步

  ```shell
  $ sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf
  ```

  ```cnf
  #/etc/mysql/mysql.conf.d/mysqld.cnf
  [mysqld]
  pid-file        = /var/run/mysqld/mysqld.pid
  socket          = /var/run/mysqld/mysqld.sock
  datadir         = /var/lib/mysql
  log-error       = /var/log/mysql/error.log
  skip-grant-tables
  ```

  在文件末尾添加 **skip-grant-tables**，用于使MySQLServer重启后，不在检查用户权限

- 第二步，重启MySQL服务

  ```shell
  $ sudo service mysql restart
  ```

- 第三步，输入mysql直接登录MySQL服务

  ```shell
  $ mysql
  mysql> use mysql;
  mysql> select authentication_string  from user where user='root';
  # 我们会看到
  +------------------------------------------------------------------------+
  | authentication_string                                                  |
  +------------------------------------------------------------------------+
  | $A$005$xQ\I&%/b/]=h5BM8FODO3pX6gSlaFCv9tJkUgt9bMFAZLVDYJqAAcGeaLA |
  +------------------------------------------------------------------------+
  # authentication_string就是加密后的密码，但是却不能直接修改成我们想要的密码。
  # 这里我们先将该值置空，然后取消skip-grant-tables，再重启，登陆，修改密码
  
  mysql> update user set authentication_string = '' where user = 'root';
  mysql> quit
  $ sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf
  # 删除之前加在末尾的 skip-grant-tables
  $ sudo service mysql restart
  $ mysql -u root -p //提示输入密码时直接敲回车。
  mysql> use mysql;
  mysql> ALTER USER 'root'@'localhost' IDENTIFIED BY '新密码';
  # 退出，重启MySQL即可。至此MySQL密码重置为新密码
  ```

##### 总结

实验环境：

- vagrant构建的 ubuntu18.04虚拟机

##### 文中涉及的软件包获取

![](http://imgs.maopuyu.com/image-20210223164949346.png)

长按扫码关注

- 回复 mysqlapt 获取 mysql-apt-config_0.8.16-1_all.deb
- 回复 navicat 获取 navicat 安装包和注册机，激活教程  https://www.bilibili.com/video/BV12K41157iR

#### sudo mysql 强制登录

```mysql
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'insert_password';
```

