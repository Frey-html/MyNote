[C++11新特性——decltype-CSDN博客](https://blog.csdn.net/qq_42956179/article/details/115410004)
[[C++：新特性]]

#review
`decltype` 是 C++11 引入的一种 **类型推导** 关键字，用于从表达式中推断类型。它非常强大，可以在编译时根据表达式的类型来决定变量或函数的类型。这在模板编程、自动类型推导和泛型编程中非常有用。

### `decltype` 的基本用法

`decltype` 可以用来获取变量、表达式或函数返回值的类型。最常见的形式如下：

```cpp
decltype(expression)
```

其中，`expression` 是任意的表达式，`decltype` 会返回该表达式的类型。

### 示例：推导变量类型

```cpp
int a = 5;
decltype(a) b = 10;  // b 的类型与 a 相同，都是 int
```

在这个例子中，`decltype(a)` 推导出 `a` 的类型是 `int`，因此 `b` 也被声明为 `int`。

### 主要特性和使用场景

#### 1. **推导变量类型**

你可以使用 `decltype` 来推导变量的类型：

```cpp
int x = 10;
decltype(x) y = 20;  // y 的类型与 x 相同，都是 int
```

这样可以避免重复编写类型声明，使代码更简洁，尤其是在使用复杂类型时非常有用。

#### 2. **推导表达式的类型**

`decltype` 可以推导表达式的类型，而不仅仅是变量：

```cpp
int a = 1;
double b = 2.0;
decltype(a + b) result;  // result 的类型是 double，因为 a + b 的类型是 double
```

在上面的例子中，`a + b` 的类型是 `double`，因为 `a` 被提升为 `double`，因此 `decltype(a + b)` 推导出 `result` 的类型是 `double`。

#### 3. **推导函数返回值的类型**

`decltype` 可以用来推导函数的返回值类型，尤其适用于函数模板或泛型编程。

```cpp
int add(int a, int b) {
    return a + b;
}

decltype(add(1, 2)) result;  // result 的类型是 int，因为 add(1, 2) 返回 int
```

更进一步的，`decltype` 可以在函数的返回值类型声明中使用：

```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```

这里，`decltype(a + b)` 用来推导函数 `add` 的返回类型。在模板编程中，由于参数类型可能是不同的，返回值类型可能不是参数类型之一，所以使用 `decltype` 来推导更加可靠。

#### 4. **推导类成员类型**

有时类的成员类型较为复杂，可以通过 `decltype` 来推导这些成员的类型：

```cpp
struct MyStruct {
    int x;
    double y;
};

MyStruct s;
decltype(s.x) a;  // a 的类型是 int
decltype(s.y) b;  // b 的类型是 double
```

#### 5. **与 `auto` 搭配使用**

虽然 `auto` 可以自动推导变量的类型，但在某些情况下，`decltype` 更加灵活。例如，如果你需要获取一个变量的类型而不是创建一个新变量，`decltype` 是更好的选择。

```cpp
int x = 42;
auto a = x;       // a 的类型是 int
decltype(x) b = a;  // b 的类型也是 int
```

#### 6. **处理引用和指针**

`decltype` 可以精确地保留表达式的引用类型和指针类型，而 `auto` 会自动忽略引用：

```cpp
int x = 10;
int& ref = x;

auto a = ref;      // a 的类型是 int，auto 会忽略引用
decltype(ref) b = ref;  // b 的类型是 int&，decltype 保留引用类型
```

在上面的例子中，`auto` 推导出 `a` 是 `int`，而 `decltype` 保留了 `ref` 的引用性质，所以 `b` 是 `int&`。

#### 7. **decltype(auto)**

C++14 引入了 `decltype(auto)`，它结合了 `decltype` 和 `auto` 的特点，用于更加精准地推导类型，尤其是能够保留表达式的引用性和指针性。

```cpp
int x = 10;
int& ref = x;

decltype(auto) a = ref;  // a 的类型是 int&，保留了引用
```

`decltype(auto)` 可以推导出表达式的完整类型，包括引用和指针，而普通的 `auto` 可能会丢失这些信息。

### `decltype` 推导规则

`decltype` 的推导规则可以分为两类，主要基于表达式的上下文：

1. **不加括号的变量**：`decltype` 直接返回变量的类型。
   - **普通变量**：返回普通类型（非引用）。
   - **引用变量**：返回引用类型。
   
   ```cpp
   int x = 10;
   int& ref = x;

   decltype(x) a;    // a 的类型是 int
   decltype(ref) b;  // b 的类型是 int&，因为 ref 是引用
   ```

2. **表达式**：如果是表达式，`decltype` 返回的是表达式结果的类型。
   - 如果表达式的结果是一个左值，`decltype` 会返回引用类型。
   - 如果表达式的结果是一个右值，`decltype` 返回普通类型。

   ```cpp
   int x = 10;
   decltype((x)) a = x;  // a 的类型是 int&，因为 (x) 是左值
   decltype(x + 1) b;    // b 的类型是 int，因为 x + 1 是右值
   ```

### `decltype` 与 `auto` 的区别

- **`auto`**：推导类型时会去掉引用和 `const`，并且不考虑表达式的左右值特性。
  
  ```cpp
  int x = 10;
  int& ref = x;

  auto a = ref;  // a 的类型是 int，auto 会忽略引用
  ```

- **`decltype`**：会保留表达式的引用性、`const` 等属性，推导出更精准的类型。

  ```cpp
  decltype(ref) b = ref;  // b 的类型是 int&，decltype 保留引用类型
  ```

### 常见的 `decltype` 使用场景

1. **模板编程**：
   - `decltype` 常用于模板中，推导复杂表达式的类型。
   
2. **泛型编程**：
   - 在泛型代码中，`decltype` 可以确定返回值类型或参数类型。

3. **返回值类型推导**：
   - 当你编写函数模板时，返回值的类型可能取决于参数的操作，`decltype` 可以帮你正确推导。

4. **避免类型重复**：
   - 如果一个变量的类型特别复杂，可以用 `decltype` 直接推导，避免重复书写类型定义。

### 总结

- **`decltype`** 是一个强大的工具，它能根据表达式推导出准确的类型，并且保留引用和 `const` 等信息。
- 与 `auto` 不同，`decltype` 可以用来推导出表达式的类型而不是变量，并且不会自动去除引用或 `const` 修饰符。
- 在模板编程、泛型编程、复杂表达式处理以及需要明确类型推导的场景中，`decltype` 是不可或缺的利器。

通过 `decltype`，你可以让 C++ 在编译时自动处理复杂的类型推导逻辑，让代码更加简洁和灵活。