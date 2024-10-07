### 1、Makefile介绍
应用参考：[[Lab1：工程makefile]] [[makefile通配符]] [[makefile静态模式]]

**参考**：
[概述 — 跟我一起写Makefile 1.0 文档 (seisman.github.io)](https://seisman.github.io/how-to-write-makefile/overview.html)
* make命令执行时需要makefile文件告诉make命令如何去编译链接
* **构成**：
	1. 显式规则。显式规则说明了如何生成一个或多个目标文件。
	2. 隐晦规则。由于我们的make有自动推导的功能，所以隐晦的规则可以让我们比较简略地书写 Makefile
	3. 变量的定义。变量一般都是字符串，类似c的宏，当Makefile被执行时，其中的变量都会被扩展到相应的引用位置上。
	4. 文件指示。其包括了三个部分，一个是在一个Makefile中引用另一个Makefile，就像C语言中的include一样；另一个是指根据某些情况指定Makefile中的有效部分，就像C语言中的预编译#if一样；还有就是定义一个多行的命令。
	5. 注释。Makefile中只有行注释，使用#。
	
* **规则**：
	* 如果工程没有编译过，所有c文件都需编译链接
	* 如果某几个c文件被修改过，只需要编译修改过的c文件并链接目标程序
	* 如果工程某几个头文件被修改过，只需要编译引用这些头文件的c文件，并重新链接
* **语法**
```
//target: 一个目标文件，执行文件或者标签
//prerequisites: 生成该target所依赖的文件
target ... : prerequisites ... 
    command  #命令以Tab为间隔
    ...
    ...
```
* **使用变量**
	* 变量可以替代复杂的prerequisites, 使用时用$(变量名)
```
objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o

edit : $(objects)
    cc -o edit $(objects)
```
* **自动推导**
	* makefile可以通过.o自动添加同名.c，并自动推导编译命令
```
//原
main.o : main.c defs.h
    cc -c main.c
//自动推导
main.o : defs.h
```
* **清空目标文件**（放在makefile文件最后）
```
clean:
    rm edit $(objects)
//或者
.PHONY : clean
clean :
    -rm edit $(objects)
```
* **makefile文件名**
	一般使用makefile或Makefile，make命令直接可以找到
	或者使用make -f filename命令
* **工作方式**
	1. 读入所有的Makefile。
	2. 读入被include的其它Makefile。
	3. 初始化文件中的变量。
    1. 推导隐晦规则，并分析所有规则。
    2. 为所有的目标文件创建依赖关系链。
    3. 根据依赖关系，决定哪些目标要重新生成。
    4. 执行生成命令。


### 补充：指定目标
在 Makefile 中，`make` 的默认行为是只编译与指定目标（可执行文件或其他目标）相关的依赖。如果你定义了多个可执行文件的规则，但这些规则之间没有依赖关系，`make` 只会编译用户指定的目标及其依赖项，**不会编译与目标无关的部分**。

#### 场景解释

假设你的 Makefile 中有三个可执行文件的构建规则，但它们之间没有依赖关系：

```makefile
# 定义三个可执行文件的构建规则
app1: app1.o
	$(CC) -o app1 app1.o

app2: app2.o
	$(CC) -o app2 app2.o

app3: app3.o
	$(CC) -o app3 app3.o

# 各个目标的依赖项
app1.o: app1.c
	$(CC) -c app1.c

app2.o: app2.c
	$(CC) -c app2.c

app3.o: app3.c
	$(CC) -c app3.c
```

##### 1. 如果你只运行 `make app1`：

- `make` 只会编译 `app1` 及其依赖项（如 `app1.o`），不会编译 `app2` 和 `app3`。
- 输出：
  ```bash
  cc -c app1.c
  cc -o app1 app1.o
  ```

##### 2. 如果你运行 `make` 不指定目标：

- `make` 会编译默认目标，默认目标是 Makefile 中**第一个出现的目标**，在这个例子中是 `app1`。它会编译 `app1` 及其依赖项。
- 输出：
  ```bash
  cc -c app1.c
  cc -o app1 app1.o
  ```

##### 3. 如果你运行 `make app2` 或 `make app3`：

- 分别编译 `app2` 或 `app3`，它们的依赖项会被正确处理。
- `make app2` 输出：
  ```bash
  cc -c app2.c
  cc -o app2 app2.o
  ```
- `make app3` 输出：
  ```bash
  cc -c app3.c
  cc -o app3 app3.o
  ```

#### 总结

- **Make 只编译与目标相关的文件**。如果你不显式指定 `app2` 和 `app3`，并且它们没有作为 `app1` 的依赖，它们不会被编译。
- **默认目标**：如果不指定目标，`make` 会编译 Makefile 中的第一个目标。如果第一个目标是 `app1`，`make` 只编译 `app1` 及其依赖项。

#### 如何强制编译多个目标？

如果你想在一次 `make` 执行中编译多个目标文件，可以定义一个伪目标（如 `all`），并让它依赖多个目标：

```makefile
# 定义伪目标 all，依赖于 app1、app2 和 app3
all: app1 app2 app3

app1: app1.o
	$(CC) -o app1 app1.o

app2: app2.o
	$(CC) -o app2 app2.o

app3: app3.o
	$(CC) -o app3 app3.o

app1.o: app1.c
	$(CC) -c app1.c

app2.o: app2.c
	$(CC) -c app2.c

app3.o: app3.c
	$(CC) -c app3.c
```

- 现在运行 `make all` 或仅 `make`（因为 `all` 是第一个目标），将会编译 `app1`、`app2` 和 `app3`：
  ```bash
  make all
  cc -c app1.c
  cc -o app1 app1.o
  cc -c app2.c
  cc -o app2 app2.o
  cc -c app3.c
  cc -o app3 app3.o
  ```
