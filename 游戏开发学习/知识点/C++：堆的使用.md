[std::make_heap - cppreference.com](https://en.cppreference.com/w/cpp/algorithm/make_heap)

在 C++中，堆是一种特殊的二叉树结构，主要用来管理优先级队列。堆可以分为两种类型：**最大堆（Max-Heap）**和**最小堆（Min-Heap）**。在最大堆中，父节点的值总是大于或等于其子节点的值，根节点是堆中最大的元素。而在最小堆中，父节点的值总是小于或等于其子节点的值，根节点是堆中最小的元素。

C++标准模板库（STL）提供了几个用于操作堆的函数，允许我们在任何符合条件的容器（如 `vector`）上使用堆的特性。C++中的堆操作是基于数组的，可以通过 `push_heap`、`pop_heap` 等函数来维持堆的性质。

### C++ 堆的基本用法

C++的堆操作主要依赖于以下几个 STL 函数：
- **`std::make_heap()`**：将一个范围内的元素转换成堆结构。
- **`std::push_heap()`**：将新添加的元素插入到堆中，并重新调整堆的顺序。
- **`std::pop_heap()`**：将堆顶元素与最后一个元素交换位置，并将剩下的部分重新调整为堆。
- **`std::sort_heap()`**：对堆进行排序。
- **`std::is_heap()`**：判断一个序列是否满足堆的性质。

### 1. **`std::make_heap()`**
`make_heap` 将一个现有的序列转换为一个堆。默认情况下，C++ 使用的是最大堆（大顶堆），如果你需要最小堆（小顶堆），可以通过自定义比较函数来实现。

**示例：**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9};
    
    // 将向量 v 转换为堆
    std::make_heap(v.begin(), v.end());
    
    // 输出堆顶元素（最大元素）
    std::cout << "堆顶元素: " << v.front() << std::endl; // 输出：9
    return 0;
}
```
`make_heap()` 会重新排列 `v` 中的元素，使之满足堆的性质。在最大堆中，`v.front()` 总是返回最大的元素。

### 2. **`std::push_heap()`**
`push_heap()` 用于在已经是堆的容器中插入新元素。通常的步骤是先将新元素添加到容器的末尾，然后使用 `push_heap()` 来维护堆的性质。

**示例：**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5};
    
    // 将向量 v 转换为堆
    std::make_heap(v.begin(), v.end());
    
    // 插入新元素
    v.push_back(9);   // 先将新元素放入向量末尾
    std::push_heap(v.begin(), v.end());  // 重新调整为堆
    
    // 输出堆顶元素（最大元素）
    std::cout << "堆顶元素: " << v.front() << std::endl; // 输出：9
    return 0;
}
```
`push_heap()` 保证新插入的元素也遵循堆的性质。

### 3. **`std::pop_heap()`**
`pop_heap()` 将堆顶元素与容器的最后一个元素交换，然后调整剩下的元素保持堆的性质。注意，这个操作并不会删除元素，它只是将堆顶元素移到容器的最后，并保持前面的元素为有效堆。

**示例：**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5};
    
    // 将向量 v 转换为堆
    std::make_heap(v.begin(), v.end());
    
    // 弹出堆顶元素
    std::pop_heap(v.begin(), v.end());  // 堆顶元素移动到末尾
    v.pop_back();  // 删除堆顶元素
    
    // 输出新的堆顶元素
    std::cout << "新的堆顶元素: " << v.front() << std::endl; // 输出：4
    return 0;
}
```
`pop_heap()` 的操作使得最大（或最小）的元素位于容器末尾，之后可以删除它。

### 4. **`std::sort_heap()`**
`sort_heap()` 会对堆进行排序。这个函数只能在一个有效的堆上使用，并且在排序之后，容器将不再具有堆的性质。

**示例：**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5};
    
    // 将向量 v 转换为堆
    std::make_heap(v.begin(), v.end());
    
    // 对堆进行排序
    std::sort_heap(v.begin(), v.end());
    
    // 输出排序后的元素
    for (int i : v) {
        std::cout << i << " ";  // 输出：1 1 3 4 5
    }
    return 0;
}
```
`sort_heap()` 对堆排序，结果是一个从小到大的有序数组。

### 5. **`std::is_heap()`**
`is_heap()` 用来判断一个序列是否是一个堆。

**示例：**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {9, 5, 4, 3, 1};
    
    // 检查 v 是否是一个堆
    if (std::is_heap(v.begin(), v.end())) {
        std::cout << "v 是一个堆" << std::endl;
    } else {
        std::cout << "v 不是一个堆" << std::endl;
    }
    
    return 0;
}
```

### **最大堆 vs 最小堆**

默认情况下，C++ 的堆操作（如 `make_heap()` 和 `push_heap()`）会生成一个 **最大堆**。如果需要最小堆，可以通过传递一个自定义的比较函数，如 `greater<int>`，来将堆转为最小堆。

**示例：最小堆实现**
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5};
    
    // 将向量 v 转换为最小堆
    std::make_heap(v.begin(), v.end(), std::greater<int>());
    
    // 输出堆顶元素（最小元素）
    std::cout << "最小堆顶元素: " << v.front() << std::endl;  // 输出：1
    
    return 0;
}
```

### **堆的原理**
堆通常使用 **完全二叉树** 实现，节点按照层序存储在一个数组中。假设当前节点的索引为 `i`，则：
- 父节点的索引为 `(i - 1) / 2`。
- 左子节点的索引为 `2 * i + 1`。
- 右子节点的索引为 `2 * i + 2`。

在插入和删除元素时，通过不断地比较并交换父子节点的位置，维护堆的性质。调整堆的操作时间复杂度是 **O (log n)**，因为堆的高度为 log (n)。

### **总结**
C++ 提供了一套高效的堆操作函数，支持对容器进行堆操作。常见的堆操作有 `make_heap`（构建堆）、`push_heap`（插入元素）、`pop_heap`（移除堆顶元素）、`sort_heap`（堆排序），以及判断堆性质的 `is_heap`。这些操作基于完全二叉树的特性，具有良好的性能，适用于优先级队列等场景。