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

菱形继承是指一个类通过两个不同的路径继承自同一个基类。它可能导致二义性。为了解决这个问题，C++引入了虚继承。（**尽量不要使用菱形继承和多继承，为了避免二义性大多数语言不像 C++一样支持多继承**）

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

### 6.1 构造函数详解
在 C++ 中，构造函数是一种特殊的成员函数，用于初始化对象。构造函数的类型主要有以下几种，每种类型的作用和调用时机也有所不同。**构造函数不能被 virtual 修饰作为虚函数**。
[[C++：类的初始化]]

#### 1. **默认构造函数**

- **定义**：不带参数的构造函数。
- **作用**：用于初始化对象的基本属性，通常用于为数据成员赋默认值。
- **调用时机**：当创建对象时，如果没有提供任何参数，默认构造函数将被调用。

```cpp
class MyClass {
public:
    MyClass() {
        // 默认构造函数
    }
};

MyClass obj; // 调用默认构造函数
```

#### 2. **带参数构造函数**

- **定义**：接受参数的构造函数。
- **作用**：允许在创建对象时直接初始化对象的属性。
- **调用时机**：当创建对象并提供参数时，带参数构造函数将被调用。

```cpp
class MyClass {
public:
    int x;
    MyClass(int value) {
        x = value; // 带参数构造函数
    }
};

MyClass obj(10); // 调用带参数构造函数
```

#### 3. **复制构造函数**

- **定义**：接受同类型对象引用作为参数的构造函数。
- **作用**：用于复制对象的属性，通常在需要复制对象时调用。
- **调用时机**：在以下情况下会调用复制构造函数：
  - 通过值传递对象作为函数参数。
  - 从函数返回对象。
  - 明确地使用复制构造函数。

```cpp
class MyClass {
public:
    int x;
    MyClass(int value) : x(value) {}

    // 复制构造函数
    MyClass(const MyClass& other) {
        x = other.x;
    }
};

MyClass obj1(10);
MyClass obj2 = obj1; // 调用复制构造函数
```

#### 4. **移动构造函数**

- **定义**：接受右值引用作为参数的构造函数。
- **作用**：用于高效地移动资源（如动态分配的内存），避免不必要的复制。
- **调用时机**：在需要转移对象所有权时，例如通过 `std::move()`。

```cpp
class MyClass {
public:
    int* data;

    MyClass(int value) {
        data = new int(value);
    }

    // 移动构造函数
    MyClass(MyClass&& other) noexcept {
        data = other.data; // 资源转移
        other.data = nullptr; // 防止其他析构
    }

    ~MyClass() {
        delete data; // 释放资源
    }
};

MyClass obj1(10);
MyClass obj2 = std::move(obj1); // 调用移动构造函数
```

#### 5. **委托构造函数**

- **定义**：一个构造函数调用另一个构造函数。
- **作用**：减少代码重复，使构造函数更清晰。
- **调用时机**：在初始化对象时，由一个构造函数委托给另一个构造函数。

```cpp
class MyClass {
public:
    int x, y;

    MyClass(int value) : MyClass(value, value) { } // 委托构造函数

    MyClass(int x, int y) : x(x), y(y) { }
};

MyClass obj(10); // 调用委托构造函数
```

#### 6. 复制构造函数与=运算符重载

##### 1. **目的**

- **= 运算符重载**：
  - 允许你定义如何处理对象之间的赋值操作。
  - 用于在**已存在**的对象之间进行赋值。

- **复制构造函数**：
  - 用于创建一个新对象，并初始化为已存在对象的副本。
  - 在**对象创建**时调用，例如通过值传递、返回对象或显式调用复制构造函数。

##### 2. **语法**

- **=运算符重载**：
  - 通过重载 `operator=` 函数实现。
  - 示例：

    ```cpp
    class MyClass {
    public:
        int* data;

        MyClass(int value) {
            data = new int(value);
        }

        // 赋值运算符重载
        MyClass& operator=(const MyClass& other) {
            if (this != &other) {
                delete data; // 释放旧资源
                data = new int(*other.data); // 深拷贝
            }
            return *this; // 返回自身
        }

        ~MyClass() {
            delete data; // 释放资源
        }
    };
    ```

- **复制构造函数**：
  - 通过定义一个接受同类引用参数的构造函数实现。
  - 示例：

    ```cpp
    class MyClass {
    public:
        int* data;

        MyClass(int value) {
            data = new int(value);
        }

        // 复制构造函数
        MyClass(const MyClass& other) {
            data = new int(*other.data); // 深拷贝
        }

        ~MyClass() {
            delete data; // 释放资源
        }
    };
    ```

##### 3. **调用时机**

- **=运算符重载**：
  - 在已经存在的对象之间进行赋值时调用。
  - 例如：
  
    ```cpp
    MyClass obj1(10);
    MyClass obj2(20);
    obj1 = obj2; // 调用 `=` 运算符重载
    ```

- **复制构造函数**：
  - 在创建新对象时调用。
  - 例如：
  
    ```cpp
    MyClass obj1(10);
    MyClass obj2 = obj1; // 调用复制构造函数
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



### 补充：内存布局与构造函数调用虚函数
在 C++ 中，子类对象的构造顺序、内存布局以及在父类构造函数中调用虚函数时的行为是比较复杂的，涉及到类的继承、构造函数的调用顺序、虚函数的调用机制等。我们逐一进行分析。

#### 1. 子类对象构造顺序

当我们创建一个子类对象时，C++ 的构造过程是按以下顺序执行的：

- **首先调用父类的构造函数**。如果父类有多个构造函数，编译器会选择一个合适的构造函数。
- **然后调用子类的构造函数**，即子类的构造函数中的代码会执行。

##### 详细构造顺序：
1. 父类的非静态数据成员（如果有）会在父类构造函数执行之前进行初始化。
2. 父类的构造函数被调用，初始化父类的成员变量。
3. 然后执行子类的构造函数。
4. 子类中的非静态数据成员会按照声明顺序进行初始化。
5. 子类的构造函数体中的代码会被执行。

> 需要注意的是，如果父类没有显式调用构造函数，编译器会调用父类的默认构造函数。如果父类没有默认构造函数（比如父类只有带参数的构造函数），那么在子类的构造函数中必须显式调用父类的构造函数。

#### 2. 内存布局

C++ 中的对象内存布局会按照类成员变量在类中的声明顺序排列，且子类对象的内存布局会包含父类的数据成员。

##### 典型的内存布局：
1. **父类的非静态成员变量**：这些成员变量会位于子类对象内存的前部分。
2. **子类的非静态成员变量**：这些成员变量会位于内存的后部分。

假设有如下代码：

```cpp
class Base {
public:
    int baseValue;
    Base(int val) : baseValue(val) {}
};

class Derived : public Base {
public:
    int derivedValue;
    Derived(int baseVal, int derivedVal) : Base(baseVal), derivedValue(derivedVal) {}
};
```

`Derived` 类对象的内存布局将类似于：

```
[ baseValue (Base) ] [ derivedValue (Derived) ]
```

其中 `baseValue` 是父类 `Base` 的成员变量，`derivedValue` 是子类 `Derived` 的成员变量。

#### 3. 父类构造函数中调用虚函数的行为

在父类构造函数中调用虚函数时，**动态绑定**（即虚函数的派发）会发生不同的行为，这一点是很多 C++ 程序员常犯的错误。

**关键点：** 当父类构造函数调用虚函数时，**调用的是父类的版本**，而不是子类的版本。这是因为在父类构造函数执行时，子类部分还没有构造完成，因此虚函数的派发机制还没有“初始化”。

##### 举个例子：

```cpp
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base constructor\n";
        call();  // 在父类构造函数中调用虚函数
    }
    
    virtual void call() {
        std::cout << "Base class call\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived constructor\n";
    }
    
    void call() override {
        std::cout << "Derived class call\n";
    }
};

int main() {
    Derived d(10);
}
```

##### 输出结果：
```
Base constructor
Base class call
Derived constructor
```

##### 解释：
- 当 `Base` 类的构造函数执行时，虚函数 `call()` 被调用。由于此时对象还没有构造完成（还不是完整的 `Derived` 对象），因此调用的是 `Base` 类的 `call()` 函数，而不是 `Derived` 类的。
- 在父类构造函数执行完毕后，才会继续执行子类的构造函数。

#### 结论

- **构造顺序**：子类对象的构造是从父类构造函数开始，父类构造完成后再执行子类构造函数。
- **内存布局**：子类对象的内存布局包括父类的成员变量，并且按照声明顺序排列。
- **虚函数调用**：在父类构造函数中调用虚函数时，调用的是父类的虚函数版本，而不是子类的版本。这是因为子类对象的构造尚未完成，虚函数的派发机制还没有生效。

通过理解这些细节，可以更好地掌控 C++ 中对象的构造过程及内存管理。
