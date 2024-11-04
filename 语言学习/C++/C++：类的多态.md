[C++进阶：详解多态](https://blog.csdn.net/qq_74415153/article/details/136761841)
[C++虚函数表剖析_c++ 虚函数表-CSDN博客](https://blog.csdn.net/lihao21/article/details/50688337)
[图说C++对象模型：对象内存布局详解 - melonstreet - 博客园 (cnblogs.com)](https://www.cnblogs.com/QG-whz/p/4909359.html)
[深度探索C++对象模型-CSDN博客](https://blog.csdn.net/zzxiaozhao/article/details/102527023)

#review
多态是 C++中一个非常重要的概念，下面将详细介绍与多态相关的所有内容，包括虚函数的使用、限制、纯虚函数和纯虚类、函数重写和重载等。

### 多态概述
多态是指同一个函数名可以根据调用对象的不同而具有不同的实现。它分为两种类型：编译时多态（静态多态）和运行时多态（动态多态）。
编译时多态： 通过函数重载和运算符重载实现，是在编译阶段确定函数调用。重载允许一个函数名有多个定义，编译器根据函数参数和上下文来选择正确的定义。
运行时多态： 通过虚函数和继承实现，是在运行阶段确定函数调用。运行时多态允许通过基类指针或引用来调用派生类的函数，实现了动态绑定。


#### 1. 运行时多态

运行时多态主要通过虚函数实现。当基类指针或引用指向派生类对象时，调用虚函数会根据实际对象的类型来决定调用哪个版本的函数。

#### 2. 虚函数

**定义和使用**：
- 在基类中使用关键字 `virtual` 声明虚函数。
- 在派生类中可以重写这个虚函数，使用关键字 `override`。

**示例**：

```cpp
class Base {
public:
    virtual void display() {
        cout << "Base display" << endl;
    }
};

class Derived : public Base {
public:
    void display() override { // 重写
        cout << "Derived display" << endl;
    }
};

Base* b = new Derived();
b->display(); // 输出: Derived display
```

**限制**：
- 虚函数不能是 `static`，因为静态方法在类的上下文中不依赖于对象实例。
- 虚函数不能是 `const`，但可以在虚函数中声明为 `const`。
- 如果一个类中有虚函数，则该类会被认为是一个多态类。

### 3. 纯虚函数和纯虚类

**纯虚函数**：
- 通过在函数声明末尾加 `= 0` 来定义纯虚函数，表示该函数没有实现，需要在派生类中实现。

**示例**：

```cpp
class AbstractBase {
public:
    virtual void pureVirtualFunction() = 0; // 纯虚函数
};

class ConcreteClass : public AbstractBase {
public:
    void pureVirtualFunction() override {
        cout << "Concrete implementation" << endl;
    }
};
```

**纯虚类**：
- 包含一个或多个纯虚函数的类称为纯虚类，不能实例化。

**示例**：

```cpp
AbstractBase* ab = new ConcreteClass();
ab->pureVirtualFunction(); // 输出: Concrete implementation
```

### 4. 函数的重写和重载

**函数重写（Overriding）**：
- 子类中重新实现基类的虚函数，必须保持函数签名（名称、参数类型和顺序）一致。

**示例**：

```cpp
class Animal {
public:
    virtual void sound() {
        cout << "Animal sound" << endl;
    }
};

class Dog : public Animal {
public:
    void sound() override { // 重写
        cout << "Bark" << endl;
    }
};
```

**函数重载（Overloading）**：
- 在同一个作用域内，可以定义多个同名但参数不同的函数，编译器通过参数类型和数量来区分。

**示例**：

```cpp
class Print {
public:
    void show(int i) {
        cout << "Integer: " << i << endl;
    }
    
    void show(double d) {
        cout << "Double: " << d << endl;
    }
    
    void show(string s) {
        cout << "String: " << s << endl;
    }
};
```

### 5. 虚构造与虚析构函数
[C++基类的析构函数为何要声明为虚函数 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/148290103)
[(超重要)构造函数为什么不能为虚函数？析构函数为什么要虚函数？](https://blog.csdn.net/salmonwilliam/article/details/114259314)
#review

如果不使用虚析构函数，当通过基类指针删除派生类对象时，只会调用基类的析构函数，而不会调用派生类的析构函数。这会导致以下问题：

1. **资源泄露**：派生类中分配的资源（如动态内存、文件句柄等）无法被正确释放，可能导致内存泄漏或其他资源问题。

2. **不完全析构**：派生类的清理代码不会执行，可能导致不一致的对象状态或其他潜在错误。

#### 示例

```cpp
class Base {
public:
    ~Base() {
        std::cout << "Base destructor" << std::endl;
    }
};

class Derived : public Base {
public:
    ~Derived() {
        std::cout << "Derived destructor" << std::endl;
    }
};

int main() {
    Base* obj = new Derived();
    delete obj; // 只会调用 Base 的析构函数
    return 0;
}
```

输出

```
Base destructor
```

在这种情况下，`Derived` 的析构函数不会被调用，可能会导致资源泄露。因此，为了确保派生类的析构函数能够正确执行，始终在基类中使用虚析构函数是良好的设计实践。

### 6.对象构造过程与虚函数
在C++中，对象的构造和析构过程涉及到一系列操作，尤其是在多层继承和多态情况下更加复杂。以下是详细的说明：

---

#### 1. 对象的构造过程

C++的对象构造过程是从基类到派生类逐层调用构造函数的，具体步骤如下：

##### 构造过程步骤

1. **分配内存**：
   - 编译器为对象分配内存，通常包含：
     - 数据成员的内存空间。
     - 虚函数表指针（如果存在虚函数）。
   
2. **调用基类构造函数**：
   - 从最顶层基类开始依次调用各层基类的构造函数，逐层初始化基类。
   - 每个基类的构造函数执行时，只能访问该基类的成员。
   
3. **派生类的成员初始化**：
   - 完成所有基类构造函数调用后，开始调用派生类构造函数，初始化派生类的成员。
   - 在派生类构造函数体执行前，派生类的虚函数表指针会指向派生类的虚函数表（如果有虚函数的话）。

4. **完成派生类构造函数**：
   - 在派生类构造函数执行完成后，对象才算真正初始化完成，至此该对象具有完整的类型。

##### 内存布局

- **普通数据成员**：每个类的数据成员存储在内存中连续的区域内。
- **虚函数表指针**：
  - 如果类中有虚函数，编译器会在对象内添加一个虚函数表指针（`vptr`），通常是一个指针，指向该类的虚函数表（`vtable`）。
  - 在构造过程的各个阶段，这个虚函数表指针会动态调整，确保正确的虚函数调用行为。

---

#### 2. 对象的析构过程

对象的析构过程是从派生类到基类逐层调用析构函数的，具体步骤如下：

##### 析构过程步骤

1. **调用派生类析构函数**：
   - 派生类析构函数首先被调用。在析构函数体中，虚函数表指针仍然指向派生类的虚函数表，允许派生类调用虚函数。
   
2. **调用基类析构函数**：
   - 派生类析构函数完成后，虚函数表指针会被调整为指向基类的虚函数表。
   - 然后逐层调用基类析构函数，从派生类到最顶层基类依次析构，直到整个对象析构完成。

3. **释放内存**：
   - 完成所有析构函数调用后，编译器会释放对象的内存空间。

---

#### 3. 对象内存布局的多态变化

对象的内存布局在多态场景下有一些变化，特别是虚函数和多层继承情况下：

##### 虚函数表指针（`vptr`）的变化

- **构造阶段**：在构造过程的每一层中，虚函数表指针会指向当前正在构造的类的虚函数表。
  - 当基类构造时，虚函数表指针指向基类的虚函数表，因此基类构造时调用虚函数会执行基类的版本。
  - 当派生类构造函数开始执行时，虚函数表指针才会指向派生类的虚函数表。

- **析构阶段**：在析构过程的每一层中，虚函数表指针逐层调整：
  - 派生类析构函数执行时，虚函数表指针仍然指向派生类的虚函数表。
  - 在派生类析构完成后，虚函数表指针调整为基类的虚函数表，因此在基类析构函数中调用虚函数会执行基类的版本。

---

#### 示例：构造和析构过程的多态变化

下面用代码示例展示对象的构造、析构以及虚函数表指针的变化。

```cpp
#include <iostream>
class Base {
public:
    Base() { std::cout << "Base constructor\n"; print(); } // 构造函数中调用虚函数
    virtual ~Base() { std::cout << "Base destructor\n"; print(); } // 析构函数中调用虚函数
    virtual void print() { std::cout << "Base::print()\n"; }
};

class Derived : public Base {
public:
    Derived() { std::cout << "Derived constructor\n"; print(); }
    ~Derived() { std::cout << "Derived destructor\n"; }
    void print() override { std::cout << "Derived::print()\n"; }
};

int main() {
    Base* obj = new Derived();
    delete obj;
    return 0;
}
```

##### 输出结果
```plaintext
Base constructor
Base::print()
Derived constructor
Derived::print()
Derived destructor
Base destructor
Base::print()
```

##### 解释

1. **构造过程**：
   - `Base`的构造函数调用`print()`，此时虚表指针指向`Base`的虚表，所以调用了`Base::print()`。
   - 然后调用`Derived`的构造函数，虚表指针切换为指向`Derived`的虚表，调用`print()`时，执行了`Derived::print()`。

2. **析构过程**：
   - 先调用`Derived`的析构函数，`print()`调用的是`Derived::print()`。
   - 然后进入`Base`的析构函数，虚表指针再次调整为指向`Base`的虚表，所以`print()`调用的是`Base::print()`。

---

#### 4. 总结

- **构造过程**：从基类到派生类逐层构造，每层的虚表指针动态调整，只有构造到对应层的类时，其虚表指针才指向该类的虚表。
- **析构过程**：从派生类到基类逐层析构，虚表指针逐步调整回基类的虚表。
- **虚函数行为**：在构造和析构中调用虚函数时，实际调用的是当前层的虚表中的函数，即构造基类时调用基类的虚函数版本，析构派生类时调用派生类的虚函数版本。

这种设计是为了保证对象在构造和析构过程中的一致性，同时避免使用不完整的对象调用未初始化的成员或方法。