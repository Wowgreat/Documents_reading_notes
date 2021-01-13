## Gradle插件简介

一个Gradle插件就是打包`可重复使用`的构建逻辑片段。这些片段是可以跨平台和项目使用的。Gradle允许你实现自己的插件。

你可以使用任意你喜欢的语言实现一个gradle插件，只要实现最终被编译为JVM字节码。在我们的示例中，我们将使用Java作为独立插件项目的实现语言，而在buildscript插件示例中使用Groovy或Kotlin。通常来讲，Java 或 Kotlin 这类静态语言比groovy性能要好。

### 打包插件

有几个地方你可以放置你的插件源代码

- ##### Build script，你可以直接将代码写在 `build.gradle`脚本中，好处是，插件会被自动编译和加入到build script的`classpath`中，缺点是，这个插件对外部的build script是不可见的，你也不能在此build script之外使用。

- ##### `buildSrc` project， 您可以将插件的源代码放在`rootProjectDir/buildSrc/src/main/java`目录中(或者`rootProjectDir/buildSrc/src/main/groovy`，或者`rootProjectDir/buildSrc/src/main/kotlin`，具体取决于您喜欢哪种语言)。Gradle将负责编译和测试插件，并使其在构建脚本的类路径上可用。该插件对构建所使用的每个构建脚本都是可见的。但是，它在构建之外是不可见的，因此您不能在其定义的构建之外重用插件。

- ##### Standalone project, 你可以为你的插件创建一个单独的项目。该项目生成并发布一个JAR，然后您可以在多个构建中使用它并与其他人共享。通常，这个JAR可能包含一些插件，或者将几个相关的任务类捆绑到一个库中。或者两者的结合。

### 使插件可配置

- **需求：为插件提供build和work过程中定制化配置的操作。**

- **解决方法：通过使用extension对象实现这一点。**Gradle项目有一个关联的ExtensionContainer对象，该对象包含了已经应用到该项目的插件的所有设置和属性。故，你可以通过增加一个扩展对象为你的插件提供扩展。一个对象是一个简单的带有属性的Java bean 独享， 它的属性代表配置。

  ```groovy
  // example
  class GreetingPluginExtension {
      String message = 'Hello from GreetingPlugin'
  }
  
  class GreetingPlugin implements Plugin<Project> {
      void apply(Project project) {
          // Add the 'greeting' extension object
          def extension = project.extensions.create('greeting', GreetingPluginExtension)
          // Add a task that uses configuration from the extension object
          project.task('hello') {
              doLast {
                  println extension.message
              }
          }
      }
  }
  
  apply plugin: GreetingPlugin
  
  // Configure the extension
  greeting.message = 'Hi from Gradle'
  ```

- 













