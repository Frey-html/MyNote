[C++之继承详解](https://blog.csdn.net/be_a_struggler/article/details/127780411)
[c++：继承（超详解）](https://blog.csdn.net/qq_62718027/article/details/125922249)
### 1. 继承的基本概念

**继承的定义**：
继承是一个类从另一个类获取属性和方法的机制。基类（父类）提供了基本的功能，派生类（子类）可以扩展或修改这些功能。

#### 基本语法

```cpp
class Base {
public:
    void baseFunction() {}
};

class Derived : public Base {
public:
    void derivedFunction() {}
};
```

### 2. 继承的类型

C++支持三种类型的继承：

- **公有继承（public inheritance）**：
  - 基类的公有成员在派生类中仍为公有。
  - 基类的保护成员在派生类中仍为保护。
  - 基类的私有成员在派生类中不可访问。

```cpp
class Base {
public:
    void publicFunc() {}
protected:
    void protectedFunc() {}
private:
    void privateFunc() {}
};

class Derived : public Base {
public:
    void access() {
        publicFunc();       // 可访问
        protectedFunc();    // 可访问
        // privateFunc();   // 不可访问
    }
};
```

- **保护继承（protected inheritance）**：
  - 基类的公有和保护成员在派生类中变为保护成员。

```cpp
class Derived : protected Base {
public:
    void access() {
        publicFunc();      // 可访问
        protectedFunc();   // 可访问
        // privateFunc();   // 不可访问
    }
};
```

- **私有继承（private inheritance）**：
  - 基类的公有和保护成员在派生类中变为私有成员。

```cpp
class Derived : private Base {
public:
    void access() {
        publicFunc();      // 可访问
        protectedFunc();   // 可访问
        // privateFunc();   // 不可访问
    }
};
```

### 3. C++支持多继承

C++允许一个类从多个基类继承，这种方式称为多继承。虽然多继承增加了灵活性，但也可能导致复杂性和二义性问题。

#### 示例

```cpp
class Base1 {
public:
    void function1() {}
};

class Base2 {
public:
    void function2() {}
};

class Derived : public Base1, public Base2 {
public:
    void derivedFunction() {}
};
```

### 4. 菱形继承

菱形继承是指一个类通过两个不同的路径继承自同一个基类。它可能导致二义性。为了解决这个问题，C++引入了虚继承。

#### 示例

```cpp
class Base {
public:
    void display() {}
};

class Derived1 : virtual public Base {
public:
    void show() {}
};

class Derived2 : virtual public Base {
public:
    void show() {}
};

class FinalDerived : public Derived1, public Derived2 {
public:
    void finalShow() {
        display(); // 调用唯一的Base实例
    }
};
```

### 5. 访问基类成员

在派生类中，可以访问基类的公有和保护成员，但无法直接访问私有成员。可以通过公有或保护的成员函数间接访问。

### 6. 构造函数和析构函数

在继承中，构造函数和析构函数的调用顺序如下：

- **构造顺序**：
  1. 基类构造函数
  2. 派生类构造函数

- **析构顺序**：
  1. 派生类析构函数
  2. 基类析构函数

#### 示例

```cpp
class Base {
public:
    Base() { cout << "Base constructed" << endl; }
    ~Base() { cout << "Base destructed" << endl; }
};

class Derived : public Base {
public:
    Derived() { cout << "Derived constructed" << endl; }
    ~Derived() { cout << "Derived destructed" << endl; }
};

int main() {
    Derived d; // 输出: Base constructed -> Derived constructed
    return 0;  // 输出: Derived destructed -> Base destructed
}
```

### 7. 重写与隐藏

- **重写（Overriding）**：派生类重写基类的虚函数，以提供特定的实现。重写时需要保持函数签名一致，并可以使用 `override` 关键字进行标记。

- **隐藏（Hiding）**：如果派生类定义了一个与基类同名的非虚函数，则基类的函数会被隐藏，而不是重写。

```cpp
class Base {
public:
    void func() { cout << "Base func" << endl; }
};

class Derived : public Base {
public:
    void func(int) { cout << "Derived func" << endl; } // 隐藏Base::func
};
```

### 8. 继承的好处和注意事项

#### 好处

- **代码重用**：通过继承，子类可以重用父类的代码，减少代码重复。
- **层次结构**：可以清晰地组织类之间的关系，形成层次结构。
- **多态**：通过虚函数实现动态绑定，提供更大的灵活性。

#### 注意事项

- **二义性**：在多继承和菱形继承中，可能会出现二义性，使用虚继承可以解决。
- **设计复杂性**：多继承可能导致类设计变得复杂，需谨慎使用。
- **资源管理**：确保在使用继承时正确管理资源，特别是在有虚构造和析构的情况下。

### 9. 总结

C++中的继承是实现代码重用和创建复杂层次结构的重要工具。理解继承的各种特性，包括公有、保护和私有继承，多继承和菱形继承，构造和析构的调用顺序，以及重写和隐藏等，是高效使用 C++进行面向对象编程的基础。

如果你有其他具体的问题或需要更多示例，随时问我！