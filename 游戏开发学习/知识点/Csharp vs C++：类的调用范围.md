在 C++ 和 C# 中，调用其他文件中的类有些不同。以下是两者的具体说明：

### C++

#### 1. **调用 `public` 类**

- **规则**：在 C++ 中，`public` 类可以在其他文件中访问，前提是要包含相应的头文件。

##### 示例：

**文件 1：`MyClass.h`**

```cpp
#ifndef MYCLASS_H
#define MYCLASS_H

class MyClass {
public:
    void display() {
        std::cout << "Hello from MyClass!" << std::endl;
    }
};

#endif
```

**文件 2：`main.cpp`**

```cpp
#include <iostream>
#include "MyClass.h" // 包含头文件

int main() {
    MyClass myClass; // 创建 MyClass 实例
    myClass.display(); // 调用方法
    return 0;
}
```

#### 2. **调用非 `public` 类**

- **规则**：非  `public` 类（例如，默认可见性）在同一文件或命名空间内可访问，但通常不建议跨文件使用。

#### 3. **调用 `private` 类**

- **规则**：`private` 类不能被其他类访问，除非在同一类内部。

---

### Csharp

#### 1. **调用 `public` 类**

- **规则**：在 C# 中，`public` 类可以在任何文件中访问，只要在使用的文件中添加适当的命名空间。

##### 示例：

**文件 1：`MyClass.cs`**

```csharp
public class MyClass {
    public void Display() {
        Console.WriteLine("Hello from MyClass!");
    }
}
```

**文件 2：`Program.cs`**

```csharp
using System;

class Program {
    static void Main() {
        MyClass myClass = new MyClass(); // 创建 MyClass 实例
        myClass.Display(); // 调用方法
    }
}
```

#### 2. **调用非 `public` 类**

- **规则**：非 `public` 类（如 `internal` 类）只能在同一程序集内访问。

##### 示例：

**文件 1：`AnotherClass.cs`**

```csharp
internal class AnotherClass {
    public void Show() {
        Console.WriteLine("Hello from AnotherClass!");
    }
}
```

**文件 2：`Program.cs`**（同一程序集）

```csharp
class Program {
    static void Main() {
        AnotherClass anotherClass = new AnotherClass(); // 可以访问
        anotherClass.Show(); // 调用方法
    }
}
```

#### 3. **调用 `private` 类**

- **规则**：`private` 类只能在其定义的类内部访问。

##### 示例：

**文件 1：`OuterClass.cs`**

```csharp
public class OuterClass {
    private class InnerClass {
        public void Display() {
            Console.WriteLine("Hello from InnerClass!");
        }
    }

    public void CallInner() {
        InnerClass inner = new InnerClass(); // 可以访问
        inner.Display();
    }
}
```

**文件 2：`Program.cs`**

```csharp
class Program {
    static void Main() {
        OuterClass outer = new OuterClass();
        outer.CallInner(); // 通过公共方法访问 private 类
    }
}
```

### 总结

- **C++**：通过头文件和包含关系访问 `public` 类；非 `public` 类在同一命名空间内访问；`private` 类仅在同一类内访问。
- **C#**：通过命名空间访问 `public` 类；非 `public` 类在同一程序集内访问；`private` 类仅在定义它的类内访问。

如果你有更多具体问题或需要进一步的示例，请告诉我！