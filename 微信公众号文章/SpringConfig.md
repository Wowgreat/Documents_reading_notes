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

##### 编写Properties类

```java
import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@Data
@Component
@ConfigurationProperties(prefix = "my")
public class MyEntityProperties {
    private String name;
}
```

##### 在application.properties中配置

```properties
my.name=I am from application.properties
```

##### 编写Properties测试类

```java
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

@SpringBootTest
public class TestMyEntityProperties {
    @Autowired
    private MyEntityProperties myEntityProperties;

    @Test
    void initEntity() {
        System.out.println(myEntityProperties.getName());
    }
}
```

##### 执行结果

```shell
  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::                (v2.6.3)

2022-02-17 16:29:13.615  INFO 10452 --- [    Test worker] c.e.d.properties.TestMyEntityProperties  : Starting TestMyEntityProperties using Java 1.8.0_301 on DESKTOP-6POMD94 with PID 10452 (started by neo in D:\code\springbootdemo)
2022-02-17 16:29:13.616  INFO 10452 --- [    Test worker] c.e.d.properties.TestMyEntityProperties  : No active profile set, falling back to default profiles: default
2022-02-17 16:29:13.997  INFO 10452 --- [    Test worker] c.e.d.properties.TestMyEntityProperties  : Started TestMyEntityProperties in 0.573 seconds (JVM running for 1.663)
I am from application.properties # 这里便是执行的结果输出
BUILD SUCCESSFUL in 2s
4 actionable tasks: 2 executed, 2 up-to-date
16:29:14: Task execution finished ':test --tests "com.example.demo.properties.TestMyEntityProperties"'.
```

##### 中文乱码解决

我们更改一下name的配置

```properties
my.name = my.name=我来自application.properties
```

执行的结果会变成

```shell
???application.properties
```

这是由于IDEA编码的问题这里我们设置一下 ， 然后重新启动IDEA。

![image-20220217165303375](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217165303375.png)

IDEA冲洗启动后，application.properties会变成下面的样子，这里我们将修正一下

![image-20220217165525558](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217165525558.png)

改后的结果如下

![image-20220217165718932](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217165718932.png)

最周执行下来的结果是

![image-20220217165754577](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217165754577.png)

当然我们也可以将application.properties修改为application.yml

```yaml
spring:
  messages:
    encoding: UTF-8
my:
  name: 我来自application.yml
```

同样可以正确执行。

##### 自定义配置文件的名称

有些情况我们并不希望使用系统自带的**application.properties**或**application.yml**配置文件，而是指定**xxxx.properties**或者**xxx.yml**作为配置文件。

- 我们在**resources**文件夹下新增**my.properties**

  ![image-20220217170430388](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217170430388.png)

- 在**MyEntityProperties**实体类中指定使用这个文件作为配置文件

  ```java
  package com.example.demo.properties;
  
  import lombok.Data;
  import org.springframework.boot.context.properties.ConfigurationProperties;
  import org.springframework.context.annotation.PropertySource;
  import org.springframework.stereotype.Component;
  
  @Data
  @Component
  @ConfigurationProperties(prefix = "my")
  @PropertySource(value = "classpath:my.properties")
  public class MyEntityProperties {
      private String name;
  }
  ```

  执行结果

  ![image-20220217172300486](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217172300486.png)

  可以发现配置并没有生效，这是因为**application.yml或application.properties**的优先级比**my.properties**高。所以这里我们清除掉application.yml中重复的配置再执行一次

  ![image-20220217172447643](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217172447643.png)

  但是**my.properties**并不能像**application.yml或application.properties**自动提示。这里需要设置一下

  - 同时按下**Ctrl+Alt+Shift+S**
  
  - ![image-20220217173147089](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217173147089.png)
  
  - ![image-20220217173203484](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217173203484.png)
  
  - ![image-20220217173229417](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217173229417.png)
  
  - 操作完成后你会发现
  
    ![image-20220217173312408](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217173312408.png)
  
    此时你会发现**my.properties**也可以自动提示了。
  
    #### 自定义yaml文件
  
    SpringBoot官方目前仅支持自定义的properties文件，如果想使用自定义的yaml文件需要额外编写解析工具。
  
    ```java
    import org.springframework.beans.factory.config.YamlPropertiesFactoryBean;
    import org.springframework.core.env.PropertiesPropertySource;
    import org.springframework.core.env.PropertySource;
    import org.springframework.core.io.support.EncodedResource;
    import org.springframework.core.io.support.PropertySourceFactory;
    
    import java.util.Objects;
    import java.util.Properties;
    
    public class YamlPropertySourceUtil implements PropertySourceFactory {
        @Override
        public PropertySource<?> createPropertySource(String name, EncodedResource resource) {
            YamlPropertiesFactoryBean factory = new YamlPropertiesFactoryBean();
            factory.setResources(resource.getResource());
    
            Properties properties = factory.getObject();
    
            assert properties != null;
            return new PropertiesPropertySource(Objects.requireNonNull(resource.getResource().getFilename()), properties);
        }
    }
    ```
  
    然后在实体类中引入这个工具
  
    ```java
    import com.example.demo.utils.YamlPropertySourceUtil;
    import lombok.Data;
    import org.springframework.boot.context.properties.ConfigurationProperties;
    import org.springframework.context.annotation.PropertySource;
    import org.springframework.stereotype.Component;
    
    @Data
    @Component
    @ConfigurationProperties(prefix = "my")
    @PropertySource(value = "classpath:my.properties", factory = YamlPropertySourceUtil.class)
    public class MyEntityProperties {
        private String name;
    }
    ```
  
    最后执行获取
  
    ![image-20220217174841161](https://maopuyufile-1304875256.cos.ap-shanghai.myqcloud.com/markdown/image-20220217174841161.png)
  
    至此SpringBoot配置方面的知识就算带大家入门了。
