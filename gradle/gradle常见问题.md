### gradle代理

修改 $HOME/.gradle/gradle.properties 文件，如下图开启gradle全局代理

```
org.gradle.jvmargs=-DsocksProxyHost=127.0.0.1 -DsocksProxyPort=10808
```

