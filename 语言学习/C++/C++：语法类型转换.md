[《深入理解C++类型转换：探索 static_cast、reinterpret_cast、dynamic_cast 和 const_cast》 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/679500619)

C++中提供了四种类型转换（cast）的方式，分别是：`static_cast`、`dynamic_cast`、`const_cast` 和 `reinterpret_cast`。这些cast操作符提供了比C语言中强制类型转换更安全、精确的类型转换方式。下面我将分别介绍它们的用法和区别。
#review

### 1. `static_cast`
`static_cast`主要用于**隐式类型转换**（implicit conversion）和一些相关的显式转换。它是一种**编译时**的类型转换，编译器会在编译阶段进行检查，但不会做运行时的类型检查。

**使用场景**：
- 基本数据类型之间的转换（如`int`转为`float`）。
- 类层次结构中，**父类与子类之间的转换**（上行转换和下行转换）。但下行转换时需要开发者确保转换是安全的，因为编译器不会做额外的检查。
- 将`void*`指针转换为具体类型的指针。

**示例**：
```cpp
int a = 10;
float b = static_cast<float>(a);  // int转为float

class Base {};
class Derived : public Base {};

Base* base = new Derived();
Derived* derived = static_cast<Derived*>(base);  // 基类指针转换为派生类指针（下行转换）
```

**特点**：
- 编译时完成。
- 不涉及任何运行时开销。
- 编译器不进行类型检查，开发者需确保转换安全。

---

### 2. `dynamic_cast`
`dynamic_cast`用于在类层次结构中进行**安全的类型转换**，特别是在**多态**情况下。它依赖于运行时类型信息（RTTI），因此可以在运行时对转换进行安全性检查。如果类型不匹配，`dynamic_cast`会返回`nullptr`（当转换为指针时）或抛出`std::bad_cast`异常（当转换为引用时）。

**使用场景**：
- 将基类指针或引用**向下转换**为派生类类型。
- 用于多态情况下，检查一个基类指针是否实际指向某个派生类对象。

**示例**：
```cpp
class Base {
    virtual void func() {}  // 必须有虚函数，才能使用dynamic_cast
};
class Derived : public Base {};

Base* base = new Derived();
Derived* derived = dynamic_cast<Derived*>(base);  // 成功转换
if (derived == nullptr) {
    std::cout << "转换失败，base并非指向Derived类型" << std::endl;
}
```

**特点**：
- 需要类有虚函数（意味着类需要启用RTTI）。
- 转换是**安全的**，因为会在运行时检查类型。
- 转换失败时返回`nullptr`或抛出异常。

---

### 3. `const_cast`
`const_cast`用于**去除或添加`const`属性**，即可以将`const`对象转换为非`const`对象，或反过来。它是唯一一个能够修改对象`const`或`volatile`属性的cast。

**使用场景**：
- 在不改变对象实际内容的前提下，去除对象的`const`属性（例如有些API参数不接受`const`类型时）。
- 在特定场景下添加`const`属性。

**示例**：
```cpp
const int a = 10;
int* b = const_cast<int*>(&a);  // 去除const属性
*b = 20;  // 未定义行为，小心使用

void func(const int* x) {
    int* y = const_cast<int*>(x);  // 去掉const属性
    *y = 5;  // 如果x指向的是const对象，这样做会有未定义行为
}
```

**特点**：
- 只能用于改变`const`或`volatile`属性。
- 如果试图修改真正的`const`对象，会导致**未定义行为**。因此，务必确保去除`const`的对象在逻辑上允许修改。

---

### 4. `reinterpret_cast`
`reinterpret_cast`是最危险的类型转换，它允许**任意类型之间的转换**，包括指针和整数之间的转换，指针类型之间的转换。它只进行**二进制位**的重新解释，不会对数据内容进行任何修改或检查，因此需要非常小心使用。

**使用场景**：
- 将一个类型的位模式重新解释为另一种不相关的类型。
- 指针和整数之间的转换（如将一个指针转换为`uintptr_t`类型，用于保存地址）。

**示例**：
```cpp
int a = 42;
void* p = &a;
int* ip = reinterpret_cast<int*>(p);  // void*转为int*

uintptr_t addr = reinterpret_cast<uintptr_t>(p);  // 指针转换为整数
void* p2 = reinterpret_cast<void*>(addr);  // 整数转换回指针
```

**特点**：
- 不进行任何类型检查或安全性检查。
- 可以用于在非常底层的场景下进行内存操作，如与硬件或操作系统交互。
- 使用不当会导致未定义行为，特别是在不同类型的数据之间强制转换时。

---

### 四种cast的区别总结
| 类型转换方式     | 转换检查    | 使用场景                                       | 特点                     |
|------------------|-------------|------------------------------------------------|--------------------------|
| `static_cast`     | 编译时检查  | 基本类型转换、类层次结构中上下转换、void*转换 | 安全但开发者需确保类型匹配 |
| `dynamic_cast`    | 运行时检查  | 类的多态转换，特别是基类到派生类的转换         | 安全但需启用RTTI，速度较慢 |
| `const_cast`      | 无           | 添加/去除`const`或`volatile`属性              | 仅用于改变`const`属性       |
| `reinterpret_cast`| 无           | 任意类型之间的位模式转换                      | 危险，慎用               |

每种类型转换操作符都有其特定的用途和限制。选择使用哪种转换取决于你是否需要类型检查、是否涉及多态，以及是否涉及底层内存操作。