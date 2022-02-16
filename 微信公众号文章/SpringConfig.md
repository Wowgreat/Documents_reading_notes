### 有趣的Spring Boot Config

##### 实验环境

- IntelliJ IDEA 2021.2
- SpringBoot 2.6.3
- Lombok
- spring-boot-configuration-processor

##### 初始化一个SpringBoot项目

我这里是直接从[Spring Initializr](https://start.spring.io/)网站初始化并下载的。然后用IDEA打开，具体过程蜗居不再赘述了。要注意的是，IDEA初次打开一个初始化的SpringBoot项目时，由于某些原因（你懂的），需要花点时间将依赖下载下来。

##### 安装Lombok

Lombok可以简化Java实体类，免去编写 **get set**方法的困扰，使实体类更加清爽。安装Lombok分为两步。

- 在IDEA中安装Lombok插件（不过我看新出的IDEA自带了该插件）

  File->Settings->Plugins

  ![image-20220216220109456](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/image-20220216220109456.png)

- 在项目中引入Lombok包

  ```groovy
  dependencies {
  	compileOnly 'org.projectlombok:lombok:1.18.22'
  	annotationProcessor 'org.projectlombok:lombok:1.18.22'
  	
  	testCompileOnly 'org.projectlombok:lombok:1.18.22'
  	testAnnotationProcessor 'org.projectlombok:lombok:1.18.22'
  }
  ```

  ```xml
  <dependencies>
  	<dependency>
  		<groupId>org.projectlombok</groupId>
  		<artifactId>lombok</artifactId>
  		<version>1.18.22</version>
  		<scope>provided</scope>
  	</dependency>
  </dependencies>
  ```

##### 安装 spring-boot-configuration-processor

```groovy
dependencies {
    annotationProcessor "org.springframework.boot:spring-boot-configuration-processor"
}
```

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-configuration-processor</artifactId>
    <optional>true</optional>
</dependency>
```

