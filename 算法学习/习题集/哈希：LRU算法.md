[lru 算法实现（C++） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/452513200)

```cpp
#include <iostream>
#include <unordered_map>
#include <list>

// 定义LRU缓存类
class LRUCache {
public:
    // 构造函数，初始化缓存容量
    LRUCache(int capacity) : capacity(capacity) {}

    // 获取缓存中某个键的值
    int get(int key) {
        auto it = cache.find(key); // 在哈希表中查找键
        if (it == cache.end()) return -1; // 如果键不存在，返回-1
        use(it); // 更新使用顺序
        return it->second.first; // 返回键对应的值
    }

    // 向缓存中插入或更新一个键值对
    void put(int key, int value) {
        auto it = cache.find(key); // 在哈希表中查找键
        if (it != cache.end()) use(it); // 如果键已存在，更新使用顺序
        else {
            if (cache.size() == capacity) { // 如果缓存已满
                cache.erase(usage.back()); // 删除使用最少的键
                usage.pop_back(); // 移除使用顺序列表中的最后一个元素
            }
            usage.push_front(key); // 将新键插入使用顺序列表的头部
        }
        cache[key] = { value, usage.begin() }; // 插入或更新键值对
    }

private:
    typedef std::list<int>::iterator ListIt; // 定义使用顺序列表迭代器类型
    int capacity; // 缓存容量
    std::list<int> usage; // 使用顺序列表
    std::unordered_map<int, std::pair<int, ListIt>> cache; // 缓存存储结构

    // 更新键的使用顺序
    void use(std::unordered_map<int, std::pair<int, ListIt>>::iterator& it) {
        usage.erase(it->second.second); // 删除旧的使用位置
        usage.push_front(it->first); // 将键移动到使用顺序列表的头部
        it->second.second = usage.begin(); // 更新迭代器指向新的位置
    }
};

int main() {
    LRUCache cache(2); // 创建一个容量为2的LRU缓存
    cache.put(1, 1); // 插入键值对 (1, 1)
    cache.put(2, 2); // 插入键值对 (2, 2)
    std::cout << cache.get(1) << std::endl; // 输出 1
    cache.put(3, 3); // 插入键值对 (3, 3), 由于缓存已满，移除键2
    std::cout << cache.get(2) << std::endl; // 输出 -1 (键2已被移除)
    cache.put(4, 4); // 插入键值对 (4, 4), 由于缓存已满，移除键1
    std::cout << cache.get(1) << std::endl; // 输出 -1 (键1已被移除)
    std::cout << cache.get(3) << std::endl; // 输出 3
    std::cout << cache.get(4) << std::endl; // 输出 4
    return 0;
}

```

这个实现使用 `unordered_map` 存储缓存的数据，以便能快速查找；使用 `list` 记录使用的顺序，实现了双向链表，便于快速移除最久未使用的元素。每次访问一个元素时，将其移动到链表头部；每次插入新元素时，如果缓存已满，则移除链表尾部的元素。这样便实现了LRU缓存机制。