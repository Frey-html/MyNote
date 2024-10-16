
[【C++】多线程编程图文详解（多角度详解，小白一看就懂！！）-CSDN博客](https://blog.csdn.net/weixin_45031801/article/details/142705048)
[C++多线程编程实践：从基础到实例的全面指南_c++多线程编程实战-CSDN博客](https://blog.csdn.net/u010349629/article/details/130569746)

### 一、C++中的线程库

C++中的多线程支持主要包含在`<thread>`库中，并通过其他辅助库来进行同步与互斥等操作。以下是几个常用的库：
- `<thread>`：提供创建、管理线程的类和函数。
- `<mutex>`：提供互斥量，用于防止多个线程同时访问共享数据。
- `<condition_variable>`：提供条件变量，用于线程间的同步。
- `<future>` 和 `<promise>`：用于线程间通信和异步任务处理。

### 二、线程的基本使用

1. **创建线程**

创建一个线程非常简单，可以通过`std::thread`类来启动一个新线程。线程的入口是一个可调用对象（如函数或lambda表达式）。

```cpp
#include <iostream>
#include <thread>

void threadFunction() {
    std::cout << "Hello from thread!\n";
}

int main() {
    // 创建并启动一个新线程
    std::thread t(threadFunction);

    // 主线程等待子线程完成
    t.join();  // 阻塞主线程，直到子线程执行完成
    return 0;
}
```

这里，`std::thread`对象`t`创建并启动了一个新线程，它执行`threadFunction`函数。`join()`用于等待线程执行完成。

2. **传递参数给线程**

可以给线程传递参数，传递参数时，`std::thread`会按值传递。如果需要传引用，需使用`std::ref()`包装引用参数。

```cpp
#include <iostream>
#include <thread>

void printNumbers(int n, const std::string& str) {
    for (int i = 0; i < n; ++i) {
        std::cout << str << " " << i << "\n";
    }
}

int main() {
    std::string message = "Thread";
    std::thread t(printNumbers, 5, std::ref(message));  // 按引用传递message
    t.join();
    return 0;
}
```

### 三、线程同步

多线程程序的一个重要挑战是**数据竞争（race condition）**，当多个线程同时访问共享数据且至少有一个线程修改该数据时，可能会出现不可预知的错误行为。为了解决数据竞争问题，C++提供了以下几种同步机制：

1. **互斥量 (`std::mutex`)**

互斥量用于保护共享资源，使得在同一时刻只有一个线程可以访问资源。

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;  // 全局互斥量

void printMessage(const std::string& message, int n) {
    // 上锁
    mtx.lock();
    for (int i = 0; i < n; ++i) {
        std::cout << message << " " << i << "\n";
    }
    // 解锁
    mtx.unlock();
}

int main() {
    std::thread t1(printMessage, "Thread 1", 5);
    std::thread t2(printMessage, "Thread 2", 5);

    t1.join();
    t2.join();
    return 0;
}
```

在上面的例子中，互斥量`mtx`保证了同一时刻只有一个线程可以执行`printMessage`中的代码块。C++11 还提供了`std::lock_guard`，可以自动管理互斥量的锁定和解锁，避免手动锁定时忘记解锁的情况：

```cpp
std::lock_guard<std::mutex> guard(mtx);
```

2. **条件变量 (`std::condition_variable`)**

条件变量允许线程等待某个条件的发生，典型用法是一个线程在某个条件达成时唤醒另一个线程。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void printMessage(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    // 等待ready为true
    cv.wait(lock, [] { return ready; });
    std::cout << "Thread " << id << " is running\n";
}

void setReady() {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟一些操作
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_all();  // 唤醒所有等待中的线程
}

int main() {
    std::thread t1(printMessage, 1);
    std::thread t2(printMessage, 2);

    setReady();

    t1.join();
    t2.join();
    return 0;
}
```

3. **读写锁 ( `std::shared_mutex` )**

C++17引入了`std::shared_mutex`，允许多个线程并发读取数据，但在写入数据时，只有一个线程可以进行写操作。

```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>

std::shared_mutex sharedMtx;
int data = 0;

void reader(int id) {
    std::shared_lock<std::shared_mutex> lock(sharedMtx);
    std::cout << "Reader " << id << " reads data: " << data << "\n";
}

void writer(int id, int value) {
    std::unique_lock<std::shared_mutex> lock(sharedMtx);
    data = value;
    std::cout << "Writer " << id << " writes data: " << data << "\n";
}

int main() {
    std::thread t1(reader, 1);
    std::thread t2(writer, 2, 10);
    std::thread t3(reader, 3);

    t1.join();
    t2.join();
    t3.join();
    return 0;
}
```

### 四、线程通信

线程通信可以通过`std::future`和`std::promise`实现。`std::promise`允许线程设置某个值，而`std::future`允许另一个线程读取该值。常用于异步任务的处理。

```cpp
#include <iostream>
#include <thread>
#include <future>

void computeValue(std::promise<int> promise) {
    int result = 42;  // 假设的计算结果
    promise.set_value(result);  // 设置结果
}

int main() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();  // 获取与promise关联的future

    std::thread t(computeValue, std::move(promise));

    // 获取计算结果
    int result = future.get();
    std::cout << "Result: " << result << "\n";

    t.join();
    return 0;
}
```

### 五、线程池（Thread Pool）

C++标准库没有直接提供线程池，但可以自己实现。线程池是管理一组工作线程的机制，避免频繁创建和销毁线程的开销。

简化的线程池实现思路如下：
- 使用一个线程队列（任务队列）来存储任务。
- 创建固定数量的线程，反复从队列中获取任务并执行。
- 通过互斥量、条件变量来保证任务队列的线程安全性。

### 六、C++中的异步编程

C++标准库提供了`std::async`用于异步任务处理，允许轻松创建异步任务并获得其结果：

```cpp
#include <iostream>
#include <future>

int compute() {
    return 42;  // 一些计算
}

int main() {
    std::future<int> result = std::async(std::launch::async, compute);
    std::cout << "Async result: " << result.get() << "\n";  // 获取结果
    return 0;
}
```

### 总结

C++的多线程编程从简单的线程创建到复杂的同步机制，提供了强大的工具来开发并行程序。为了避免数据竞争和死锁等问题，合理使用同步机制（如互斥量、条件变量）非常重要。