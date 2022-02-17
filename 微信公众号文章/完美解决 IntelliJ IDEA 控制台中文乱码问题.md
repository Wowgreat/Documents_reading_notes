#### 完美解决 IntelliJ IDEA 控制台中文乱码问题

有时候在使用Intellij Idea的时候会遇到下面的问题

![image-20220208134416361](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220208134416361.png)

- 首先查看IDEA的全局编码设置，发现均为UTF-8

  **File -> Settings -> File Encoding**

![image-20220208135529563](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220208135529563.png)

- 编辑**Custom VM Options**

  **Help -> Edit Custom VM Options**，在文件末尾加上 **-Dfile.encoding=UTF-8**，然后重启**IDEA**

![image-20220208135745446](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220208135745446.png)

- 最后运行项目发现，乱码消失了，至此问题解决

  ![image-20220208135917218](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220208135917218.png)