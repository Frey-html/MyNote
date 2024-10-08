是的，在 Java 中，一个文件中的所有类都属于该文件所在的包。

具体解释如下：

1. **包声明**:
   - 在 Java 文件的顶部，通常会有一个 `package` 语句，比如：
     ```java
     package com.example.myapp;
     ```
   - 这个声明指定了该文件中的所有类都属于 `com.example.myapp` 这个包。

2. **文件中的所有类**:
   - Java 中，一个 `.java` 文件可以包含多个类（但通常只有一个公共类 `public class`）。所有这些类，无论是 `public`、`default` 访问修饰符的类，或者是嵌套类，都会被视为属于该文件声明的包。例如：
     ```java
     package com.example.myapp;

     public class MainClass {
         // 主类的内容
     }

     class HelperClass {
         // 辅助类的内容
     }
     ```
   - 在上面的例子中，`MainClass` 和 `HelperClass` 都属于 `com.example.myapp` 包。

3. **默认包**:
   - 如果在 `.java` 文件中没有显式的 `package` 声明，那么该文件中的类属于**默认包**。这种情况下，类不属于任何命名的包，而是处于一种默认的全局包中（不推荐在实际项目中使用默认包，因为默认包中的类不能被其他带有包声明的类访问）。

### 总结

Java 文件中的所有类，无论是公共类还是非公共类，都属于该文件声明的包。如果没有 `package` 声明，它们将位于默认包中。