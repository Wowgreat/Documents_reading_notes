## Configuring the Build Environment

### Build Environment

##### There are four methods to configure Gradle behavior(arrange by priority):

- **Command-line flags**
- **System properties**
  (1) Using the `-D` command-line option, you can pass a system property to the JVM which runs Gradle.(gradle -D == java -D)
  (2)**Specifying system properties in** `gradle.properties`

- **Gradle properties**
  Setting up a consistent environment for your build is as simple as placing these settings into a `gradle.properties` file. The configuration is a combination of all your `gradle.properties` files, but if an option is configured in multiple locations, the *first one wins*:
- **Environment Variables**
  - GRADLE_OPTS
  - GRADLE_USER_HOME
  - JAVA_HOME

### Project properties

### Configuring JVM memory



## Configuring The Gradle Daemon

### The Gradle Daemon

- ### Running Daemon Status

- ### Disabling the Daeamon

- ### Stopping an existing Daemon



## Initialization Scripts

略

## Executing Multi-Project Builds

> **Note: ** Build files might not be called `build.gradle`.





## Testing Java & JVM Projects



## Managing Dependencies



## JVM Plugins

