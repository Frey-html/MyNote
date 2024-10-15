[【C++】多态-CSDN](https://blog.csdn.net/weixin_67596609/article/details/131839669)
[多态（举例+详解）](https://blog.csdn.net/weixin_67596609/article/details/131839669)
[C++进阶：详解多态](https://blog.csdn.net/qq_74415153/article/details/136761841)
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