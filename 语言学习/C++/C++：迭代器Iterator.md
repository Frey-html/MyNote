[C++迭代器iterator详解-CSDN博客](https://blog.csdn.net/QIANGWEIYUAN/article/details/89184546)
[std::allocator - cppreference.com](https://en.cppreference.com/w/cpp/memory/allocator)

This code provides a simplified implementation of a custom vector container, `MyVector`, along with a **nested class** `iterator` to iterate over the elements. It mimics the behavior of the C++ STL `std::vector`, with essential methods for managing dynamic arrays, adding/removing elements, and iterating over them.


---

### **Overview of `MyVector` Class:**
- The class `MyVector` is templated to hold elements of type `T` (like `std::vector<T>`).
- It uses an allocator (default is `std::allocator<T>`) for memory management.
- Key methods include:
  - `push_back()`: Adds an element to the end.
  - `pop_back()`: Removes the last element.
  - `resize()`: Expands the storage when needed.
  - An internal iterator class is provided for traversing the container.
  
### **Code Breakdown:**

#### 1. **Class Declaration and Constructor**:
```cpp
template<typename T, typename Alloc = std::allocator<T>>
class MyVector {
public:
    MyVector(const Alloc &alloc = Alloc())
        : _allocator(alloc) {
        _first._ptr = _last._ptr = _end._ptr = nullptr;
    }
    ...
};
```

- `template<typename T, typename Alloc = std::allocator<T>>`: The class is templated to support any type `T`. It also takes an optional allocator (`Alloc`) to manage memory, defaulting to `std::allocator<T>`.
- `MyVector(const Alloc &alloc = Alloc())`: The constructor accepts a custom allocator. By default, it uses the standard allocator.
  - `_first`, `_last`, and `_end` are iterators initialized to `nullptr` (signifying an empty vector).
    - `_first`: Points to the first element.
    - `_last`: Points to one past the last valid element (i.e., the end of the current data).
    - `_end`: Points to the end of the allocated memory.

#### 2. **`push_back()` Method**:
```cpp
template<typename T>
void push_back(T &&val) {
    if (full())
        resize();  // Expand the container if needed
    _allocator.construct(_last._ptr, std::forward<T>(val));
    _last._ptr++;  // Move the end pointer forward
}
```
- This method adds an element to the vector.
- **Template Parameter**: `T &&val` is a perfect forwarding mechanism (accepts both lvalues and rvalues).
- **Steps**:
  1. If the vector is full (`full()`), it calls `resize()` to allocate more memory.
  2. It constructs the new element at the location `_last._ptr` using `std::allocator::construct`.
  3. `_last._ptr++` advances the iterator to the next position, reflecting that a new element has been added.

#### 3. **`pop_back()` Method**:
```cpp
void pop_back() {
    if (empty())
        return;  // Do nothing if empty
    _last._ptr--;  // Move back to the last element
    _allocator.destroy(_last._ptr);  // Destroy the last element
}
```
- This method removes the last element from the vector.
  - If the vector is empty, it does nothing.
  - Otherwise, it decrements `_last._ptr` (to remove the last element) and calls `destroy()` to destruct the element.

#### 4. **`full()` and `empty()` Methods**:
```cpp
bool full() const { return _last._ptr == _end._ptr; }
bool empty() const { return _first._ptr == _last._ptr; }
```
- **`full()`**: Checks if the vector is full by comparing `_last` (the current end of valid elements) and `_end` (the allocated end of memory).
- **`empty()`**: Checks if the vector is empty by comparing `_first` (beginning) and `_last` (current end). If they point to the same location, the vector is empty.

---

### **Iterator Class Implementation**:
```cpp
class iterator {
public:
    friend class MyVector;
    iterator(T *ptr = nullptr) : _ptr(ptr) {}
    
    void operator++() { ++_ptr; }  // Prefix increment
    bool operator!=(const iterator &it) const { return _ptr != it._ptr; }
    T& operator*() { return *_ptr; }  // Dereference operator
    T* operator->() { return _ptr; }  // Arrow operator

private:
    T *_ptr;
};
```
- The `iterator` class is nested inside `MyVector` and allows traversal through the elements.
- **Key components**:
  1. **Constructor**: The iterator is initialized with a raw pointer (`_ptr`), which points to the elements inside `MyVector`.
  2. **Operator `++`**: Advances the iterator to the next element by incrementing the pointer.
  3. **Operator `!=`**: Compares two iterators by comparing their pointers.
  4. **Operator `*`**: Dereferences the iterator, returning a reference to the element pointed to by `_ptr`.
  5. **Operator `->`**: Provides pointer-like access to the object (for smart pointers or classes with members).

#### 5. **`begin ()` and `end ()` Methods**:
```cpp
Iterator begin () { return iterator (_first._ptr); }
Iterator end () { return iterator (_last._ptr); }
```
- **`begin ()`**: Returns an iterator pointing to the first element (`_first`).
- **`end ()`**: Returns an iterator pointing to the position after the last valid element (`_last`). This is consistent with how STL `end ()` iterators work.

---

### **Memory Management and `resize ()` Function**:
```cpp
Void resize () {
    If (_first._ptr == nullptr) {
        _first._ptr = _allocator.Allocate (1);
        _last._ptr = _first._ptr;
        _end._ptr = _first._ptr + 1;
    } else {
        Int size = _last._ptr - _first._ptr;  // Calculate the number of valid elements
        T *ptmp = _allocator.Allocate (2 * size);  // Allocate twice the size
        For (int i = 0; i < size; ++i) {
            _allocator.Construct (ptmp + i, _first._ptr[i]);  // Copy elements to new memory
            _allocator.Destroy (_first._ptr + i);  // Destroy old elements
        }
        _allocator.Deallocate (_first._ptr, size);  // Deallocate old memory
        _first._ptr = ptmp;  // Update the new pointers
        _last._ptr = _first._ptr + size;
        _end._ptr = _first._ptr + 2 * size;
    }
}
```
- **`resize ()`** expands the vector when it’s full.
- If the vector is empty (`_first._ptr == nullptr`), it allocates an initial block of size 1.
- If the vector contains elements:
  1. The function calculates the number of elements.
  2. Allocates new memory (`2 * size`) to expand the capacity.
  3. Copies the old elements to the new memory.
  4. Destroys the old elements.
  5. Updates `_first`, `_last`, and `_end` pointers to point to the new memory.

---

### **Summary**:
This code implements a simplified version of a dynamic array (like `std::vector`) with basic functionalities:
- **Dynamic memory management**: It uses an allocator (`std::allocator`) to manage memory dynamically. When the vector is full, it resizes and reallocates memory, copying existing elements to new locations.
- **Element access and modification**: Supports `push_back ()`, `pop_back ()`, and dynamic resizing.
- **Iterators**: Provides a nested `iterator` class to allow traversing elements, supporting `begin ()` and `end ()` functions, consistent with how standard C++ iterators work.

The implementation focuses on teaching the core concepts of dynamic arrays, memory management, and iterators.

### 示例：简易 Vector 实现

```cpp
#include <iostream>
#include <memory> // 引入标准库中的内存管理功能

// 简单的 vector 容器实现，主要查看其嵌套类 iterator 迭代器的实现
template<typename T, typename Alloc = std::allocator<T>>
class MyVector {
public:
    // 构造函数，使用分配器 Alloc 初始化 _allocator
    MyVector(const Alloc &alloc = Alloc())
        : _allocator(alloc) // 初始化分配器
    {
        // 初始化指针，指向空
        _first._ptr = _last._ptr = _end._ptr = nullptr;
    }

    // 在容器末尾添加新元素
    template<typename T>
    void push_back(T &&val) {
        // 如果容器已满，进行扩容
        if (full())
            resize();
        // 在 _last 位置构造新元素
        _allocator.construct(_last._ptr, std::forward<T>(val));
        // 移动 _last 指针到下一个位置
        _last._ptr++;
    }

    // 移除末尾元素
    void pop_back() {
        // 如果容器为空，直接返回
        if (empty())
            return;
        // 移动 _last 指针到前一个位置
        _last._ptr--;
        // 销毁 _last 指向的元素
        _allocator.destroy(_last._ptr);
    }

    // 检查容器是否已满
    bool full() const { return _last._ptr == _end._ptr; }
    // 检查容器是否为空
    bool empty() const { return _first._ptr == _last._ptr; }

    // 容器迭代器的实现
    class iterator {
    public:
        // 使 MyVector 类成为 iterator 的朋友类，以访问其私有成员
        friend class MyVector;

        // 构造函数，接受一个指针参数
        iterator(T *ptr = nullptr) : _ptr(ptr) {}

        // 递增操作符重载
        void operator++() { ++_ptr; }
        
        // 不等于操作符重载
        bool operator!=(const iterator &it) { return _ptr != it._ptr; }

        // 解引用操作符重载
        T& operator*() { return *_ptr; }
        
        // 箭头操作符重载
        T* operator->() { return _ptr; }
    
    private:
        T *_ptr; // 指向容器元素的指针
    };

    // 返回指向容器第一个元素的迭代器
    iterator begin() { return iterator(_first._ptr); }
    
    // 返回指向容器最后一个元素后继位置的迭代器
    iterator end() { return iterator(_last._ptr); }

private:
    iterator _first; // 指向数组的起始地址
    iterator _last;  // 指向最后一个有效元素的后继位置
    iterator _end;   // 指向数据空间的末尾元素的后继位置
    Alloc _allocator; // 容器底层的空间分配器

    // 容器的扩容函数
    void resize() {
        // 如果当前没有分配空间
        if (_first._ptr == nullptr) {
            // 分配一个元素的空间
            _first._ptr = _allocator.allocate(1);
            _last._ptr = _first._ptr; // _last 也指向新分配的空间
            _end._ptr = _first._ptr + 1; // _end 指向下一个位置
        } else {
            // 计算当前存储的元素个数
            int size = _last._ptr - _first._ptr;
            // 分配双倍大小的新空间
            T *ptmp = _allocator.allocate(2 * size);
            
            // 复制当前元素到新空间
            for (int i = 0; i < size; ++i) {
                _allocator.construct(ptmp + i, _first._ptr[i]); // 在新位置构造元素
                _allocator.destroy(_first._ptr + i); // 销毁旧位置的元素
            }
            // 释放旧空间
            _allocator.deallocate(_first._ptr, size);
            // 更新指针
            _first._ptr = ptmp; // 更新指向新空间
            _last._ptr = _first._ptr + size; // _last 更新到最后一个有效元素后继位置
            _end._ptr = _first._ptr + 2 * size; // _end 更新到新空间的末尾
        }
    }
};
```

#### 注释说明

1. **类模板声明**:
   - `template<typename T, typename Alloc = std::allocator<T>>` 声明了一个类模板，`T` 是存储元素的类型，`Alloc` 是用来管理内存的分配器，默认使用 `std::allocator<T>`。

2. **构造函数**:
   - `MyVector(const Alloc &alloc = Alloc())` 初始化分配器，并将 `_first`、`_last` 和 `_end` 的指针初始化为 `nullptr`，表示当前没有分配任何空间。

3. **`push_back` 函数**:
   - 添加元素到 `vector` 的末尾。如果当前空间已满，则调用 `resize` 扩容。
   - 使用 `_allocator.construct` 在 `_last` 指向的地址上构造新元素，并更新 `_last` 指针。

4. **`pop_back` 函数**:
   - 移除 `vector` 末尾的元素。先检查是否为空，如果不为空，更新 `_last` 指针并销毁相应的元素。

5. **`full` 和 `empty` 函数**:
   - 检查 `vector` 是否已满或为空。

6. **嵌套的 `iterator` 类**:
   - `iterator` 是一个嵌套类，封装了一个指向 `T` 类型的指针。提供了迭代器的基本操作，如递增、解引用和比较。
   - 使用 `friend class MyVector;` 允许 `MyVector` 访问 `iterator` 的私有成员。

7. **`begin` 和 `end` 方法**:
   - `begin()` 返回指向第一个元素的迭代器，`end()` 返回指向最后一个元素后继位置的迭代器。

8. **`resize` 函数**:
   - 处理 `vector` 的扩容。首先判断是否有空间分配，如果没有则分配一个元素的空间。
   - 如果有空间，则计算当前存储的元素数量，分配一个双倍大小的新空间，将旧元素复制到新空间，并释放旧空间。更新 `_first`、`_last` 和 `_end` 指针。
