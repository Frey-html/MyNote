在 C++ 中，**比较函数**（普通函数）和 **比较仿函数**（函数对象或 functor）的本质区别在于它们的使用方式和语法要求。为了更好地理解为什么不能直接在 `priority_queue` 中使用普通的比较函数，而需要使用仿函数，下面将详细解释它们的区别以及 C++ 容器对仿函数的要求。
[C++11——仿函数原理及使用场景_c++ 仿函数应用场景-CSDN博客](https://blog.csdn.net/qq_42956179/article/details/115462144)

### 比较函数与仿函数的区别

1. **普通比较函数**：
   - 一个普通函数是一个独立的实体，不能携带状态（例如，不能记住之前的操作或存储一些额外信息）。
   - 它的形式是：`bool compare(const T& a, const T& b)`，返回布尔值表示比较结果。
   - 它可以像普通函数那样被调用，但无法直接用于某些模板类中，因为模板类需要的是 **一个类型**，而不是 **函数指针**。

   **示例**：
   ```cpp
   bool compare(const int& a, const int& b) {
       return a > b;
   }
   ```

2. **比较仿函数（函数对象）**：
   - 仿函数本质上是一个定义了 `operator()` 的类或结构体的对象，它可以像函数一样调用。
   - 仿函数比普通函数更灵活，因为它可以携带状态（即可以保存类成员变量），还能被当作 **类型** 使用（这正是容器模板如 `priority_queue` 需要的）。
   - 当传递仿函数到模板类时，传递的是 **类型**（即一个类或结构体），而不是一个单独的函数指针。这符合模板的语法要求。

   **示例**：
   ```cpp
   struct Compare {
       bool operator()(const int& a, const int& b) const {
           return a > b;  // 定义比较逻辑
       }
   };
   ```

### 为什么不能直接用普通的比较函数？

`priority_queue` 作为一个模板类，要求 **比较器必须是一个类型**，而不是一个函数指针。模板参数中的 `Compare` 期望的是一个可以 **实例化的类型**，也就是说，它期望接收到的比较器是一个类或结构体，这个类或结构体中必须定义 `operator()` 以便进行函数调用。

- 普通函数是不能作为 **类型** 的，只能作为 **指针** 使用。
- 而仿函数是类或结构体，可以作为模板参数使用，并且通过重载 `operator()`，它可以像函数一样调用。

### 仿函数的优势：
1. **更灵活**：仿函数可以携带状态。例如，你可以在仿函数内部存储一些额外的信息，如统计信息、某个标志位等。
2. **可以用作模板参数**：仿函数是一个类型，因此可以用在 STL 容器（如 `priority_queue`）中，作为模板的第三个参数。普通函数指针则不具备这种能力。
3. **性能优化**：仿函数可能会比普通函数指针稍微高效，因为编译器在编译时就已经知道仿函数的类型，可以更好地进行优化。而函数指针需要在运行时动态解析函数地址。

### `priority_queue` 中使用仿函数

`priority_queue` 是一个模板类，它的第三个参数（比较器）需要传入一个 **类型**，这个类型定义了如何比较元素。默认情况下，它使用的是 `std::less<T>`，这是一个标准的仿函数。

例如，`std::less<T>` 定义了这样一个仿函数，它的作用就是返回 `a < b`：

```cpp
template<class T>
struct less {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};
```

类似地，你可以自定义自己的仿函数：

```cpp
struct Compare {
    bool operator()(const int& a, const int& b) const {
        return a > b;  // 改为 a > b 的比较逻辑
    }
};

int main() {
    // 使用自定义仿函数 Compare 作为第三个参数
    priority_queue<int, vector<int>, Compare> pq;

    pq.push(10);
    pq.push(5);
    pq.push(20);

    cout << pq.top();  // 输出 5，最小堆行为
}
```

### 为什么 `priority_queue` 不能直接使用普通比较函数？

`priority_queue` 要求的比较器是一个 **类型**，即模板的第三个参数（`Compare`）需要是类型，而不是普通函数。模板参数是编译时确定的，编译器需要知道如何实例化一个类型，并通过 `operator()` 来进行比较。

直接使用普通函数会导致模板实例化失败，因为普通函数指针不能被当作类型使用，模板不能使用函数指针进行实例化：

```cpp
bool compare(const int& a, const int& b) {
    return a > b;
}

// 这会产生编译错误，因为 compare 不是一个类型
priority_queue<int, vector<int>, compare> pq;  // 错误！
```

相反，仿函数（如 `std::less<T>` 或自定义的 `Compare`）是一个 **类型**，符合模板的要求，可以在模板实例化时正确使用：

```cpp
struct Compare {
    bool operator()(const int& a, const int& b) const {
        return a > b;
    }
};

// 这是正确的，Compare 是一个类型
priority_queue<int, vector<int>, Compare> pq;  // 正确！
```

### 总结：
1. **普通函数**：不能直接用于模板类的比较器参数，因为模板类要求的是一个类型，而普通函数只是函数指针，不是类型。
2. **仿函数**：是一种带有 `operator()` 的类或结构体，它可以像函数一样调用，同时也可以作为模板类的参数，因为它是一个类型。
3. **STL 容器**（如 `priority_queue`）需要仿函数来进行比较操作，因为模板参数要求类型。

仿函数比普通函数灵活，能够携带状态，并且可以在模板类中使用，因此在 `priority_queue` 以及其他容器适配器中，仿函数是定义自定义比较逻辑的标准方式。



### 补充：为什么sort可以接受普通函数指针而priority_queue不行：
在 C++ 中，`std::sort` 函数确实可以直接接受 **函数指针** 作为自定义的比较器，这是因为 `std::sort` 的设计方式与 `priority_queue` 的模板参数要求不同。下面详细解释为什么 `std::sort` 可以直接使用函数指针，而 `priority_queue` 需要一个仿函数（或函数对象）。

#### 1. `std::sort` 的函数签名

`std::sort` 是一个模板函数，接受三个参数：

```cpp
template<class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare comp);
```

- `first` 和 `last`：是迭代器，指定排序的范围。
- `comp`：是一个 **可调用对象**（Callable object），用于定义排序规则。这个 `comp` 参数可以是：
  - **函数指针**（普通函数）
  - **仿函数**（函数对象）
  - **lambda 表达式**
  - **std::function**

`std::sort` 对比较器 `comp` 的要求是，它必须是 **可调用对象**。这意味着任何可以像函数一样调用的东西都可以传递给 `std::sort`，包括函数指针。由于普通函数可以通过函数指针来调用，它自然满足这个条件。

##### 使用函数指针的例子：

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

bool compare(int a, int b) {
    return a > b;  // 降序排序
}

int main() {
    vector<int> v = {4, 2, 5, 1, 3};
    
    // 使用函数指针作为比较器
    sort(v.begin(), v.end(), compare);
    
    for (int i : v) {
        cout << i << " ";  // 输出: 5 4 3 2 1
    }
}
```

在上面的例子中，`compare` 是一个普通函数，通过函数指针的形式传递给 `std::sort` 来进行排序。

#### 2. 为什么 `std::sort` 能接受函数指针？

**模板函数**（如 `std::sort`）可以接受 **函数指针** 作为模板参数，因为 C++ 的模板机制允许将函数指针作为模板参数的类型传递给函数。

在调用 `std::sort` 时，模板会自动推断 `Compare` 的类型。如果你传递的是一个普通的比较函数，编译器会推断 `Compare` 为函数指针类型 `bool (*)(int, int)`，并正确调用它。

**总结**：
- `std::sort` 只需要一个 **可调用对象**，它可以是函数指针、仿函数或 lambda 表达式。
- 因为普通函数可以通过指针来调用，所以它满足了 `std::sort` 对 `Compare` 的要求。

#### 3. `priority_queue` 与 `std::sort` 的区别

虽然 `std::sort` 可以接受函数指针，但 `priority_queue` 要求的是一个 **类型**，这和它的模板设计有关。

#### `priority_queue` 的模板签名：

```cpp
template <class T, class Container = vector<T>, class Compare = less<typename Container::value_type>>
class priority_queue;
```

- `T`：队列中元素的类型。
- `Container`：存储数据的底层容器，默认是 `std::vector`。
- `Compare`：比较器，用来定义元素的优先级。默认是 `std::less<T>`，这是一个仿函数，创建最大堆。

这里的 `Compare` **不是一个函数指针**，而是一个 **类型**。这意味着 `priority_queue` 期望的是一个类型，可以是 `std::less<T>` 这种仿函数类型，或者你自定义的比较对象类型。

##### 为什么 `priority_queue` 需要类型而不是函数指针？

`priority_queue` 是一个 **容器适配器**，它依赖底层容器（比如 `std::vector`）来存储数据，内部维护了一个堆结构。这个堆的操作（如 `push`、`pop` 等）需要不断地比较元素，而这些操作是在 **编译时** 确定的。为了支持这种编译时优化，`priority_queue` 要求传入的比较器是一个类型，方便在编译时进行实例化和优化。

- 如果使用仿函数（或其他类型），编译器可以在编译时就知道该如何调用比较操作，能进行更好的优化。
- 函数指针则是在运行时解析的，会导致一些性能上的损耗。

#### 4. 可调用对象 vs 类型

- **`std::sort`** 只需要传递一个 **可调用对象**，无论是函数指针、lambda、还是仿函数都可以，因为它只在执行排序时调用这个比较器。
- **`priority_queue`** 需要传递一个 **类型**，它会在容器生命周期中多次使用这个类型来比较元素，所以它要求的是一个仿函数类型，而不是一个运行时才确定的函数指针。

#### 5. 仿函数与函数指针的适用场景总结

- **函数指针**：可以传递给任何需要 **可调用对象** 的地方，例如 `std::sort`。在这些场景中，传递一个普通函数是可以的，因为只要求这个参数能被调用。
- **仿函数**：更常用于需要 **类型** 的模板类或容器，如 `priority_queue`、`std::set`、`std::map` 等。仿函数是一个类型，在编译时可以被实例化，并在容器的生命周期中反复使用。

#### 总结

- **`std::sort`**：接受 **可调用对象**，所以它可以接受函数指针、lambda 或仿函数作为参数，因为它只需要在排序时调用这个比较器。
- **`priority_queue`**：要求的是一个 **类型**，因此它需要仿函数（或函数对象）作为比较器，因为模板类需要在编译时实例化这个比较器，并在整个容器的操作中使用它。

这两者的设计思路不同，导致它们对比较器的要求也不同。