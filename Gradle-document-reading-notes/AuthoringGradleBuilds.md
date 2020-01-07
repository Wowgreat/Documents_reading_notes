## Learning The Basics

- ### Introducing the Basics of Build Scripts

- ### Working with Tasks

  #### Defining tasks

  - **Method 1**

    ```groovy
    build.gradle
    
    task('hello'){
        doLast {
            print "hello"
        }
    }
    task('copy', type: Copy) {
        from(file('srcDir'))
        into(buildDir)
    }
    ```

  - **Method 2**

    ```groovy
    build.gradle
    
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

  - **Method 3**

    ```groovy
    build.gradle
    
    tasks.create('hello') {
        doLast {
            println "hello"
        }
    }
    
    tasks.create('copy', Copy) {
        from(file('srcDir'))
        into(buildDir)
    }
    ```

  #### Locating tasks

  #### Configuring tasks

  - *Creating a copy task*
  -  *Configuring a task using the API*
  -  *Configuring a task using a DSL specific syntax*
  - *Defining a task with a configuration block*

- ### Learning More About Build Scripts

- ### Working with Files

- ### Using Gradle Plugins

- ### Understanding the Build Lifycle

- ### Working with Logging

- ### Configuring Multi-Project

