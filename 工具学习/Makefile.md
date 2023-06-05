#### 1、Makefile介绍 ####
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


#### 2、书写规则 ####
