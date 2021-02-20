### 【官方教程】使用APT安装MySQL指南

#### MySQL全新安装步骤

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

- 执行

  ```shell
  sudo apt-get update
  ```

##### 使用APT安装MySQL

```shell
sudo apt-get install mysql-server
```

![2](http://imgs.maopuyu.com/2.png)

输入密码，选择 **<ok>**就可以了





