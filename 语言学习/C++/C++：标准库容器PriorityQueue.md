
[[C++：语法仿函数]]

`priority_queue` 是 C++ 标准库中的一个容器适配器，提供了 **优先队列** 的功能。它的底层通常实现为一个 **堆**（默认是最大堆，根节点的值总是最大）。优先队列支持快速地插入和取出具有最高优先级的元素。
### 基本语法与用法

```cpp
#include <queue>  // priority_queue 的头文件

std::priority_queue<Type, Container, Compare> pq;
```

#### 参数说明：
1. **Type**：优先队列中存储的元素类型。
2. **Container**（可选）：底层容器类型，默认是 `std::vector`，也可以使用 `std::deque` 等。该容器必须支持随机访问以及堆操作。
3. **Compare**（可选）：比较函数或函数对象，用于定义元素的优先级顺序。默认使用 `std::less<Type>`，这会使优先队列成为 **最大堆**，即优先返回最大值。

### 默认用法（最大堆）

默认情况下，`priority_queue` 是最大堆，意味着它总是让堆顶元素是当前最大值：

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    priority_queue<int> pq;  // 默认是最大堆

    pq.push(10);
    pq.push(5);
    pq.push(20);

    cout << "Top element: " << pq.top() << endl; // 输出 20，最大堆
    pq.pop();  // 移除堆顶元素 20

    cout << "Top element after pop: " << pq.top() << endl;  // 输出 10
}
```

#### 函数方法：
- `push(const Type& val)`：插入元素到优先队列。
- `pop()`：移除堆顶元素（但不会返回该元素，返回顶元素用 `top()`）。
- `top()`：返回堆顶元素，但不移除。
- `empty()`：检查队列是否为空。
- `size()`：返回优先队列的大小。

### 自定义比较（最小堆）

如果你想使用 **最小堆**（即优先返回最小值），可以通过指定比较函数来实现。例如：

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    // 使用 greater<int> 将其变为最小堆
    priority_queue<int, vector<int>, greater<int>> minHeap;

    minHeap.push(10);
    minHeap.push(5);
    minHeap.push(20);

    cout << "Top element (min heap): " << minHeap.top() << endl;  // 输出 5
    minHeap.pop();

    cout << "Top element after pop: " << minHeap.top() << endl;  // 输出 10
}
```

在这个例子中，`greater<int>` 将优先队列转换为 **最小堆**，即堆顶元素为最小值。

### 自定义比较函数或仿函数

如果你想要更加复杂的比较逻辑，可以通过自定义比较函数或使用仿函数。例如，对于一个存储自定义结构体的优先队列，可以根据结构体的某个字段进行排序。

#### 示例：使用仿函数比较

```cpp
#include <iostream>
#include <queue>
using namespace std;

// 定义一个结构体
struct Node {
    int value;
    int priority;

    Node(int v, int p) : value(v), priority(p) {}
};

// 自定义比较仿函数
struct CompareNode {
    bool operator()(const Node& a, const Node& b) {
        // 按优先级从高到低排序，优先级大的优先
        return a.priority < b.priority;
    }
};

int main() {
    priority_queue<Node, vector<Node>, CompareNode> pq;

    pq.push(Node(1, 2));  // value = 1, priority = 2
    pq.push(Node(2, 5));  // value = 2, priority = 5
    pq.push(Node(3, 1));  // value = 3, priority = 1

    cout << "Top element: value = " << pq.top().value << ", priority = " << pq.top().priority << endl;  // 输出 value = 2, priority = 5
}
```

#### 示例：使用 lambda 表达式

C++11 及以上版本支持使用 lambda 表达式定义比较函数：

```cpp
#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int value;
    int priority;

    Node(int v, int p) : value(v), priority(p) {}
};

int main() {
    auto cmp = [](const Node& a, const Node& b) {
        return a.priority > b.priority;  // 按优先级从低到高排列
    };

    priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);

    pq.push(Node(1, 2));  // value = 1, priority = 2
    pq.push(Node(2, 5));  // value = 2, priority = 5
    pq.push(Node(3, 1));  // value = 3, priority = 1

    cout << "Top element: value = " << pq.top().value << ", priority = " << pq.top().priority << endl;  // 输出 value = 3, priority = 1
}
```

### 优先队列的常见应用
- **任务调度**：基于任务的优先级调度任务执行顺序。
- **路径搜索算法**：如 Dijkstra 算法、A* 算法中，常用优先队列来维护未处理的节点。
- **合并 k 个有序链表**：将所有链表的节点按顺序合并。

### 小结

1. **默认情况下**，`priority_queue` 是一个最大堆，返回队列中的最大值。
2. 通过指定 `greater<Type>` 或自定义比较器，可以将 `priority_queue` 转换为 **最小堆** 或其他任意排序顺序。
3. 自定义排序可以使用仿函数、lambda 表达式等。

使用 `priority_queue` 可以方便地实现基于优先级的队列操作，在算法中具有广泛的应用场景。