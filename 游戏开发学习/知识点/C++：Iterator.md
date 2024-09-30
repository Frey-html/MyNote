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