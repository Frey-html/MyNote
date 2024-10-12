在 C++中，当对象作为函数参数时，默认情况下是**值传递**。这意味着传递的是对象的一个**副本**，函数内部对对象的任何修改不会影响原始对象。

### 1. **值传递（默认方式）**
   - 当你将一个对象作为参数传递给函数时，如果没有使用引用或指针，它会按值传递。
   - 也就是说，会调用对象的**拷贝构造函数**来创建一个该对象的副本，函数内部操作的就是这个副本。
   - 任何对副本的修改都不会影响原来的对象。

#### 示例：
```cpp
class MyClass {
public:
    int data;
    MyClass(int val) : data(val) {}
};

void modifyObject(MyClass obj) {  // 按值传递
    obj.data = 10;                // 修改的是副本
}

int main() {
    MyClass myObj(5);
    modifyObject(myObj);          // 传递对象
    std::cout << myObj.data << std::endl;  // 输出仍然是 5
}
```
在这个例子中，`modifyObject` 函数接收的是 `myObj` 的副本，所以 `myObj.data` 的值在函数内部的修改不会影响原来的对象，仍然是5。

### 2. **引用传递**
   - 如果希望在函数内修改传递的对象并影响到原始对象，或者避免不必要的拷贝开销，可以使用**引用传递**（使用引用或指针）。
   - 引用传递允许函数操作实际的对象而不是副本。

#### 引用传递示例：
```cpp
void modifyObjectByRef(MyClass& obj) {  // 引用传递
    obj.data = 10;                      // 修改的是原对象
}

int main() {
    MyClass myObj(5);
    modifyObjectByRef(myObj);           // 传递对象
    std::cout << myObj.data << std::endl;  // 输出 10
}
```
在这个例子中，`modifyObjectByRef` 函数接收的是 `myObj` 的引用，直接操作原始对象，因此修改后 `myObj.data` 的值变成了10。

### 3. **使用常量引用避免拷贝且保持对象不变**
   - 有时候你不需要修改对象，但又不想因为按值传递导致性能开销。此时，可以使用**常量引用**（`const &`）来避免拷贝并保证对象不被修改。
   
#### 示例：
```cpp
void printObject(const MyClass& obj) {  // 常量引用
    std::cout << obj.data << std::endl; // 不能修改 obj
}

int main() {
    MyClass myObj(5);
    printObject(myObj);                 // 传递对象
}
```
这里使用了常量引用，既避免了对象的拷贝，又保证了对象在函数内部不会被修改。

### 总结：
- **默认传递方式**：C++函数中，对象作为参数默认是**按值传递**，即传递对象的副本。
- **引用传递**：如果想避免拷贝或允许函数修改原始对象，需要显式使用引用（`&`）。
- **常量引用传递**：如果不需要修改对象，但想避免拷贝开销，可以使用 `const &` 传递对象。