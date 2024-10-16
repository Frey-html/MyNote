[C++网络编程+线程（全详解入门） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/650758773)
[Windows上的C++网络编程保姆级教学-CSDN博客](https://blog.csdn.net/weixin_74027669/article/details/138437305)

[VS code 配置std::thread调试环境（主要是Mingw更新）](https://blog.csdn.net/weixin_44816102/article/details/123975430)

C++网络编程是指通过网络协议进行通信的编程技术，常用于构建客户端/服务器应用程序。C++作为一种系统级语言，提供了高效的网络编程能力，但它本身不包含网络库。通常，C++开发者依赖于一些第三方库和操作系统提供的网络API来实现网络功能。

在C++中进行网络编程，常用的库和API包括：
1. **Berkeley Sockets API**：也称为POSIX sockets，适用于UNIX、Linux和Windows系统。
2. **Boost.Asio**：一个跨平台的异步网络编程库，支持同步和异步操作。
3. **POCO C++ Libraries**：另一个广泛使用的C++网络库，提供高级的网络抽象。
4. **Qt Network Module**：Qt库中的网络模块，提供了丰富的网络编程功能。
![[Pasted image 20241016150432.png]]
### 一、网络编程的基础：Socket

**Socket（套接字** 是网络编程中用于通信的基础抽象。它是通信的端点，通常包含IP地址和端口号。Socket是跨平台的概念，可以在不同操作系统中使用。通过Socket，客户端和服务器之间可以传递数据。

Socket的通信模型一般有以下几种：
- **TCP（Transmission Control Protocol）**：面向连接的协议，保证数据可靠传输。
- **UDP（User Datagram Protocol）**：无连接的协议，数据不保证可靠传输，但速度较快。

#### 1.1 Socket编程步骤

无论是客户端还是服务器，使用Socket进行通信的基本步骤如下：

1. **创建Socket**：调用系统API创建Socket对象。
2. **绑定Socket**：将Socket与特定的IP地址和端口绑定（通常用于服务器）。
3. **监听和连接**：服务器监听客户端的连接请求，客户端主动发起连接。
4. **发送和接收数据**：通过Socket进行数据的读写操作。
5. **关闭Socket**：通信结束后关闭Socket，释放资源。

### 二、使用POSIX Sockets进行网络编程

POSIX Sockets是C++中网络编程的基础API，几乎所有操作系统都支持。以TCP协议的客户端-服务器模型为例，以下是如何用POSIX Sockets实现的。

#### 2.1 TCP服务器代码

```cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 创建服务器端的Socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    // 设置Socket选项
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 设置服务器地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有网络接口
    address.sin_port = htons(8080); // 监听端口8080

    // 绑定Socket到指定的IP地址和端口
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return -1;
    }

    // 监听客户端的连接
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        return -1;
    }

    std::cout << "Waiting for connections...\n";

    // 接受客户端的连接
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed\n";
        return -1;
    }

    std::cout << "Connection established!\n";

    // 发送数据给客户端
    const char* message = "Hello from server";
    send(new_socket, message, strlen(message), 0);

    // 关闭Socket
    close(new_socket);
    close(server_fd);

    return 0;
}
```

#### 2.2 TCP客户端代码

```cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // 创建客户端Socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    // 设置服务器地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // 将IP地址转换为二进制格式
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return -1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed\n";
        return -1;
    }

    // 接收服务器消息
    read(sock, buffer, 1024);
    std::cout << "Message from server: " << buffer << "\n";

    // 关闭Socket
    close(sock);

    return 0;
}
```

在这个简单的示例中：
- 服务器使用`socket()`函数创建一个TCP套接字，然后使用`bind()`将其绑定到特定的IP地址和端口，接着使用`listen()`进入监听状态，等待客户端的连接。
- 客户端使用`socket()`创建一个TCP套接字，并使用`connect()`连接到服务器。

### 三、Boost.Asio：C++的高级网络库

[Boost.Asio](https://www.boost.org/doc/libs/release/doc/html/boost_asio.html) 是C++中一个广泛使用的跨平台网络编程库，它支持同步和异步操作。Boost.Asio不仅支持TCP和UDP，还支持串口、文件描述符等多种I/O设备。Boost.Asio采用异步事件驱动模式，非常适合处理大量并发连接的服务器。

#### 3.1 Boost.Asio TCP同步服务器

```cpp
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // 创建服务器端的接受器，监听端口8080
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        std::cout << "Waiting for connection...\n";

        while (true) {
            // 创建套接字并等待客户端连接
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Client connected\n";

            // 发送消息给客户端
            std::string message = "Hello from Boost.Asio server\n";
            boost::asio::write(socket, boost::asio::buffer(message));
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

#### 3.2 Boost.Asio TCP同步客户端

```cpp
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // 创建客户端的套接字
        tcp::socket socket(io_context);
        
        // 连接服务器
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "8080"));

        // 接收服务器消息
        char reply[1024];
        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, 1024));

        std::cout << "Message from server: " << std::string(reply, reply_length) << "\n";
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
```

Boost.Asio支持异步操作，例如可以使用`async_accept`和`async_read`等方法来实现异步I/O，避免阻塞。异步编程需要定义回调函数，这些函数将在I/O操作完成后被调用。通过异步编程，服务器可以处理大量并发连接，而不必为每个连接创建单独的线程。

### 四、POCO C++ Libraries

POCO提供了一组强大的C++库，包括网络编程、线程、文件系统等模块。它的网络模块支持HTTP、TCP、UDP等协议，使用起来更加抽象化，比POSIX Sockets和Boost.Asio更容易上手。

### 五、Qt Network Module

Qt的网络模块提供了高层次的网络编程接口，尤其适合GUI应用程序。它支持TCP、UDP、HTTP等协议，并且与Qt的信号/槽机制完美结合，非常适合编写事件驱动的网络程序。

###

 六、总结

C++网络编程为构建高性能网络应用程序提供了灵活的工具。初学者可以从POSIX Sockets入手，了解网络编程的基础；对于需要跨平台支持和异步编程的应用，Boost.Asio是非常好的选择。如果需要更多的功能和高级抽象，可以考虑使用POCO或Qt等更高级的库。