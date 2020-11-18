> LineageOS是android一个免费且开源的发行版，就如同Ubuntu与Linux的关系一样

#### 为啥要安装LineageOS？

1、纯净，没有其他的捆绑软件

2、由第一点就决定了，这款系统比其他发行版本更小巧，更快捷，可以让你的老旧机器焕发第二春

3、**生命在于折腾**

#### 废话不多说，开始安装吧

- ##### STEP 1、准备材料

  - 小米 4 LIT（代号：cancro）一台，自用的老机器，据传说，这机型是爱好刷机者的必备机型

  - 小米4 适用的**USB数据线**一条

  - **twrp.img** 用于recovery image（将系统镜像刷入手机中），这里小米 4 LIT对应的版本是 **twrp-3.3.1-0-cancro.img**，关注 **直截了当** 公众号回复 cancro 即可获取该资源

  - **lineage-17.1-20200416-UNOFFICIAL-cancro.zip** 适用于小米4的LineageOS镜像，关注 **直截了当** 公众号回复 cancro 即可获取该资源

    > 重点提示：针对不同型号的手机需要不同的twrp，LineageOS版本，如果你找不到你的机型对应的这两个软件的版本，可以在#直截了当#公众号内回复你的机型，可以帮助你寻找

  - **platform-tools_r30.0.1-windows.zip** 该压缩包内包含 **adb、fastboot** 等android调试工具，同样在公众号内回复 **ptools** 获取改包

- ##### STEP2、安装步骤

  - STEP1中各软件下载下来

  - 解压**platform-tools_r30.0.1-windows.zip**，并将 **twrp-3.3.1-0-cancro.img、lineage-17.1-20200416-UNOFFICIAL-cancro.zip**  等软件包移动到该**platform-tools**文件夹(platform-tools_r30.0.1-windows.zip解压后的文件夹里)，是这些软件和 adb fastboot等软件在同目录下，方便操作

    > 可忽略的提示：当然你也可以将adb fastboot等加入环境变量

  **以下步骤目前支持小米系列，其他系列手机需要稍微变化。如果您想看其他型号手机的教程可以联系我们**

  - 关闭手机

  - （此步骤执行前需进行**解锁BootLoader**，由于这不是本文重点关注的内容，所以这里不多做介绍，如果很多同学不知道，以后可以出一篇文章）待手机关闭后，同时按住**电源键+音量 - **进入**fastboot**模式

    ![](https://s1.ax1x.com/2020/05/18/YfrA78.png)

  - 此时执行,在**platform-tools**目录下执行

    ```powe
    > adb push lineage-17.1-20200416-UNOFFICIAL-cancro.zip /sdcard
    lineage-17.1-20200416-UNOFFICIAL-cancro.zip: 1 file pushed, 0 skipped. 23.3 MB/s (452194265 bytes in 18.504s)
    > fastboot reboot
    ```

  - 此时手机会重启，这时同时按住 **电源键+音量 +**即可进入 **recovery** 模式，（本文选择的是**英文版**，你也可以选择中文版）

    ![](https://s1.ax1x.com/2020/05/18/YfrehQ.jpg)

    ![](https://s1.ax1x.com/2020/05/18/YfrZtg.jpg)

  

  - 此时点击 **Wipe** -> **Advanced Wipe** ->  **选择需要Wipe的分区，如下图** -> 滑动 并执行Wipe

    ![](https://s1.ax1x.com/2020/05/18/YfrVAS.jpg)

    ![](https://s1.ax1x.com/2020/05/18/Yfrk0f.jpg)

    ![](https://s1.ax1x.com/2020/05/18/Yfrnpj.jpg)

  - 此时我们退回到菜单栏，并连接 usb数据线

    ```powershell
    > adb devices
    List of devices attached
    ba80cd5 recovery   # 说明手机已连接电脑
    
    > adb push lineage-17.1-20200416-UNOFFICIAL-cancro.zip /sdcard
    lineage-17.1-20200416-UNOFFICIAL-cancro.zip: 1 file pushed, 0 skipped. 25.9 MB/s (452194265 bytes in 16.666s) # 系统安装包推送到手机/sdcard目录下
    ```

    

  - 此时点击 Install -> 选择 安装包 -> 执行安装系统

    ![](https://s1.ax1x.com/2020/05/18/YfrKcn.jpg)

  - 待完成之后执行重启命令就可以了，重启后是以下的样子

    ![](https://s1.ax1x.com/2020/05/18/Yfru1s.jpg)

  - 最后会进入系统初始化设置流程，类似于你买了个新的小米，经过一系列设置，就进入系统了，这个系统目前是比较简陋的，仅有android远程系统的部分软件（APP）
    ![](https://s1.ax1x.com/2020/05/18/YfrMXq.jpg)

- 总结：此时系统比较简陋，接下来将带领大家为这个操作系统安装 google套件（google play，google服务框架，google服务等）。但限于文章篇幅就不在本文重详解，留到下一篇文章中为大家呈现。关注 **直截了当**公众号，及时获取 为android lineageos刷入google套件的文章。

  > 原创不易，如想转载请联系本号，再不济，也请著名出处： 微信公众号，直截了当  谢谢