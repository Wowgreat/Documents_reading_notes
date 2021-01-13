| English                       | Chinese                                                      |
| ----------------------------- | ------------------------------------------------------------ |
| In a nutshell                 | 简单地说                                                     |
| authoring                     | 编辑                                                         |
| authorized / authorise / auth | 授权的, 认证                                                 |
| authentic                     | 自生的                                                       |
| expensive                     | 1、贵的<br />2、（计算机）花费大量系统时间和资源；一种不必要的昂贵算法选择 |
| readily                       | 容易的                                                       |
| apparently                    | 显然的                                                       |



#### The Gradle Daemon

Gradle它运行在JVM上，并且需要用多个支持库，这要求一段不短的初始化时间。作为结果就是，gradle启动非常的慢。解决这个问题的办法就是**Gradle Daemon**。长期驻留在后台的进程执行你的构建，要比其他情况快很多。我们通过避免不必要的昂贵算法启动进程和大量的缓存（将你项目相关的数据保存在内存中）。运行Gradle，通过守护进程构建和非通过守护进程构建没有区别。简单的配置就可以决定你是否使用它，其他所有的事情都由Gradle自动掌控。







#### 禁用Daemon

在 `«USER_HOME»/.gradle/gradle.properties`，加上这样一行

```properties
org.gradle.daemon=false
```

- `C:\Users\<username>` (Windows Vista & 7+)
- `/Users/<username>` (macOS)
- `/home/<username>` (Linux)













#### gradle-wrapper

建议在Gradle Wrapper的帮助下执行gradle build。这样做有以下好处：

- 通过一个给定的gradle版本，标准化一个项目。这样乐意使项目构建更加的可靠、稳健。
- 可以通过简单的修改Wrapper的设定轻而易举地向不同的用户提供新的Gradle版本。

#### Command-Line Interface

强烈建议使用**Gradle Wrapper**，你应该使用 `gradlew`替换`gradle`



#### 初始化Gradle Wrapper

 

#### 编辑任务(三种方式)

```groovy
# build.gradle
# NO.1
task('hello'){
    doLast {
        println "hello"
    }
}
task('copy', type: Copy) {
    from(file('srcDir'))
    into(buildDir)
}
# NO.2
tasks.create('hello') {
    doLast {
        println "hello"
    }
}

tasks.create('copy', Copy) {
    from(file('srcDir'))
    into(buildDir)
}

# NO.3
task(hello) {
    doLast {
        println "hello"
    }
}

task(copy, type: Copy) {
    from(file('srcDir'))
    into(buildDir)
}
```

#### 定位任务



#### 声明依赖



#### 自定义Plugins

```groovy
apply plugin: GreetingPlugin
class GreetingPlugin implements Plugin<Project> {
   void apply(Project project) {
      project.task('hello') << {
         println "Hello from the GreetingPlugin"
      }
   }
}
```





#### Gradle Plugins介绍

使用一个插件可以扩展项目的能力。它可以做以下事情：

- 扩展Gradle模型，例如增加新的DSL元素
- 通过约定配置项目
- 应用指定的配置



