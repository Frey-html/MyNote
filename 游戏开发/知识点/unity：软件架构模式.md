#review
[[常用设计模式]]
与设计模式区别：
- **软件架构模式**：
    - 关注的是整个系统的设计，包括各个主要组件如何交互以及它们之间如何组织。
    - 涉及到系统的高层次结构和决策。
- **设计模式**：
    - 更加具体地解决类、对象或函数等小范围的问题。
    - 提供了解决特定问题的模板，通常是在编码层面。
    - 例子包括 Singleton、Observer、Factory Method 等。

### 1. MVC (Model-View-Controller)
- **实现**: 
  - **Model**: 存储应用程序的数据和业务逻辑。可以通过数据库访问对象（DAO）或其他数据访问技术来实现。
  - **View**: 展示数据，通常由 HTML/CSS/JavaScript 构成。在后端，可以使用模板引擎（如 Thymeleaf、Handlebars）来动态渲染页面。
  - **Controller**: 处理用户输入，更新 Model 和 View。在 Web 应用中，这通常是通过处理 HTTP 请求来实现的，例如使用 Spring MVC 或 Django。

### 2. MVVM (Model-View-ViewModel)
- **实现**:
  - **Model**: 同 MVC 的 Model。
  - **View**: 使用数据绑定机制来展示 ViewModel 提供的数据。现代前端框架如 Angular、Vue. Js 和 React 都支持 MVVM 模式。
  - **ViewModel**: 实现业务逻辑，并提供数据绑定所需的数据和命令。通常通过观察者模式或其他机制来同步 ViewModel 和 View。

### 3. Layered (N-tier) Architecture
- **实现**:
  - 通常分为表示层、业务逻辑层、数据访问层等。每一层都有明确的责任和接口定义。
  - 表示层处理用户交互，业务逻辑层处理核心业务流程，数据访问层处理数据库操作。
  - 层间通信通常通过接口或 API 进行，确保各层的松耦合。

### 4. Microservices Architecture
- **实现**:
  - 将应用程序拆分为多个小型服务，每个服务运行在其自己的进程中，并通过轻量级通信机制（通常是 HTTP API）进行交互。
  - 每个微服务都是独立部署、升级和扩展的。
  - 微服务之间需要使用服务发现机制（如 Eureka、Consul）来动态查找其他服务的位置。

### 5. Event-driven Architecture (EDA)
- **实现**:
  - 应用程序的不同组件通过发布和订阅消息来进行通信。
  - 通常使用消息队列（如 RabbitMQ、Kafka）或事件总线来传递事件。
  - 组件不需要直接调用彼此，而是通过发布事件来通知其他组件。

### 6. Service-oriented Architecture (SOA)
- **实现**:
  - 服务之间通过标准的通信协议（如 SOAP、REST）进行交互。
  - 每个服务提供一组接口（通常是 Web Services），客户端通过这些接口来调用服务。
  - SOA 强调服务的重用性和松耦合性。

### 7. Client-server Architecture
- **实现**:
  - 客户端向服务器发送请求，服务器处理请求并返回结果。
  - 在 Web 应用中，客户端通常是浏览器，服务器则处理 HTTP 请求并返回 HTML 页面或其他资源。
  - 可以使用多种协议（如 HTTP、FTP）和语言（如 Java、Python、Node. Js）来实现服务器端逻辑。

### 深入：MVC 与 MVVM 对比
MVC（Model-View-Controller）和 MVVM（Model-View-ViewModel）虽然都是软件架构模式，但它们之间有几个关键的区别：
#### MVC (Model-View-Controller)

##### 组成部分：
- **Model**: 负责数据管理和业务逻辑。
- **View**: 负责展示数据给用户，不包含业务逻辑。
- **Controller**: 处理用户的输入，并控制 Model 和 View 之间的交互。

##### 特点：
- **耦合度**: Controller 通常会直接与 View 交互，处理用户输入，并更新 Model。
- **适用性**: 适用于 Web 应用、桌面应用以及其他需要处理用户输入和显示数据的应用程序。
- **数据绑定**: 数据绑定通常是手动完成的，即 Controller 需要显式地更新 View。

##### 实现方式：
- 在 MVC 中，用户与 View 发生交互，Controller 捕获这些交互并相应地更新 Model 和 View。
- 例如，在 Web 开发中，Controller 通常是处理 HTTP 请求的后端逻辑，它从 Model 获取数据并将其传递给 View 进行渲染。

#### MVVM (Model-View-ViewModel)

##### 组成部分：
- **Model**: 同 MVC 的 Model，负责数据管理和业务逻辑。
- **View**: 负责展示数据给用户，但不直接与 Model 通信。
- **ViewModel**: 作为 Model 和 View 之间的桥梁，负责提供数据绑定所需的属性和命令，处理一些用户交互逻辑。

##### 特点：
- **耦合度**: ViewModel 与 View 之间的耦合度较低，因为它们通过数据绑定机制连接。
- **适用性**: 适用于需要声明式编程的环境，特别是那些支持数据绑定的语言和框架（如 WPF、Xamarin. Forms、AngularJS、Vue. Js）。
- **数据绑定**: 数据绑定通常是自动的，ViewModel 中的数据变化会自动反映在 View 上，反之亦然。

##### 实现方式：
- 在 MVVM 中，View 与 ViewModel 通过数据绑定机制进行交互，ViewModel 负责与 Model 通信。
- 用户与 View 的交互不会直接触发 Model 的变化，而是通过 ViewModel 来协调。

#### 总结：

- **MVC** 更加关注用户交互和状态的直接管理，适合需要频繁用户交互的应用。
- **MVVM** 强调数据绑定和声明式编程，更适合需要复杂数据模型和动态 UI 的应用。

两种模式的选择取决于具体的应用需求和技术栈。例如，在需要高度动态 UI 和数据驱动的应用中，MVVM 可能是更好的选择；而在需要清晰地分离业务逻辑和用户界面的 Web 应用中，MVC 可能更加合适。