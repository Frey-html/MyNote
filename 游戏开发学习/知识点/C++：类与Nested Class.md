[Nested classes - cppreference.com](https://en.cppreference.com/w/cpp/language/nested_types)
在 C++中，嵌套类（nested class）是一种非常有用的特性，它允许你在一个类的内部定义另一个类。嵌套类可以分为两种类型：**内嵌类（nested class）** 和 **成员类（member class）**。

嵌套类的名字存在于外围类的作用域中，并且从嵌套类的成员函数中进行名称查找时，会在检查完嵌套类的作用域之后访问外围类的作用域。就像外围类的任何成员一样，嵌套类可以访问外围类所能访问的所有名称（私有、受保护等）。除此之外，嵌套类是独立的，并没有特别的途径访问外围类的 `this` 指针。嵌套类中的声明可以使用外围类的任何成员，遵循非静态成员使用的常规规则。
这段话的主要内容可以总结如下：
1. **作用域**：嵌套类的名字存在于外围类的作用域中，因此在访问嵌套类时需要通过外围类的名称来限定。
2. **名称查找**：从嵌套类的成员函数中进行名称查找时，会先检查嵌套类的作用域，然后检查外围类的作用域。
3. **访问权限**：嵌套类可以访问外围类的所有成员（包括私有和受保护成员），前提是这些成员对外围类本身是可见的。
4. **独立性**：除了可以访问外围类的成员之外，嵌套类是独立的，它并没有特别的途径访问外围类的 `this` 指针。
5. **成员使用规则**：嵌套类中的声明可以使用外围类的任何成员，但需要遵循非静态成员的一般使用规则。

### 内嵌类（Nested Class）
内嵌类是定义在一个外层类的内部的类。内嵌类与外层类之间没有直接的继承关系，但是内嵌类可以访问外层类的私有成员。内嵌类的定义通常是为了组织相关的类，或者实现某些封装的需求。

#### 示例：
```cpp
class OuterClass {
public:
    // 外部类的公共成员
    void publicMethod() {}

private:
    // 外部类的私有成员
    int privateData;

    // 内嵌类定义在外层类的内部
    class InnerClass {
    public:
        // 内嵌类可以访问外部类的私有成员
        void accessPrivateDataOfOuter() {
            int data = outerInstance.privateData; // 访问外部类的私有成员
        }

        // 创建内嵌类的实例需要外部类的实例
        InnerClass(OuterClass& outer) : outerInstance(outer) {}

    private:
        OuterClass& outerInstance; // 引用外部类的实例
    };

    // 成员函数可以返回内嵌类的实例
    InnerClass getInnerClassInstance() {
        return InnerClass(*this);
    }
};

int main() {
    OuterClass outer;
    OuterClass::InnerClass inner = outer.getInnerClassInstance();

    // 注意：内嵌类的访问必须通过外层类的实例
    return 0;
}
```

### 成员类（Member Class）
成员类是一种特殊的内嵌类，它不仅定义在另一个类的内部，而且还是那个类的一个成员。成员类的定义方式与内嵌类类似，但它具有成员属性，如公有（public）、私有（private）或受保护（protected）。

#### 示例：
```cpp
class OuterClass {
public:
    // 成员类定义在外层类的内部
    class MemberClass {
    public:
        // 成员类的公共成员
        void publicMethod() {}

    private:
        // 成员类的私有成员
        int privateData;

        // 成员类可以访问外部类的私有成员
        void accessPrivateDataOfOuter() {
            int data = outerInstance.privateData; // 访问外部类的私有成员
        }

    protected:
        OuterClass& outerInstance; // 引用外部类的实例
    };

private:
    // 外部类的私有成员
    int privateData;

    // 创建成员类的实例需要外部类的实例
    MemberClass createMemberClassInstance() {
        return MemberClass(*this);
    }
};

int main() {
    OuterClass outer;
    OuterClass::MemberClass member = outer.createMemberClassInstance();

    // 注意：成员类的访问必须通过外层类的实例
    return 0;
}
```

### 嵌套类的特点
1. **访问控制**：嵌套类可以访问外层类的所有成员，包括私有的。
2. **作用域**：嵌套类的作用域是在外层类内部定义的，它对外界是不可见的，除非通过外层类的实例来访问。
3. **实例化**：通常情况下，创建嵌套类的实例需要外层类的实例，除非嵌套类的构造函数是公开的。
4. **名称解析**：访问嵌套类时，需要使用外层类的名称作为前缀，如 `OuterClass::InnerClass`。
5. **封装**：嵌套类可以用来实现更高级别的封装，使代码更加模块化。

### 嵌套类的应用场景
1. **封装相关功能**：将一组紧密相关的类放在一起，便于管理和使用。
2. **实现内部细节**：隐藏实现细节，使外部代码看不到内部的具体实现。
3. **辅助类**：创建辅助类来帮助实现外层类的功能。

### 注意事项
1. **命名冲突**：嵌套类的名字与外层类的其他成员名字可能会产生冲突。
2. **依赖关系**：嵌套类通常依赖于外层类的存在，因此它们的生命周期通常与外层类绑定。

