**课程要求**
* 自己写代码每五行要有详细的中文注释
* 实验报告要注重讨论心得（实验过程中遇到的问题及解决方法）

### Chapter1:Intro
* 计算机系统组成：
	* 软件：
	1. 应用软件
	2. 中间件 Middleware
	3. 系统软件：操作系统，其他系统软件（编译器，连接器，数据库等）
	* 硬件：
	1. IO设备
	2. 存储器（内存）
	3. 中央处理器（CPU）

* 操作系统提供的接口：
	* 命令级接口：鼠标或键盘等命令，手机的触控和声音接收
	* 程序级接口：提供一组系统调用 system calls，即操作系统服务供程序调用

#### 操作系统的执行
* 特权指令：
	不允许用户程序直接使用，如IO指令，设置时钟，控制寄存器

* Dual-mode operation：
	* 用户态（执行用户程序时）和内核态（执行操作系统程序时）（Linux）
	i386支持Ring0-Ring3
	RISC-V架构也提供四种模式

* 分时系统：多个用户（进程）分时共享同一台计算机
	* 响应时间：用户发出命令到系统响应的时间间隔
		$S=n*q$
		n:进程数（用户数） q：每个进程运行时间片
### Chapter2:Operating-System Structure

* Operating System Interface
	* Command interface
	* Program interface (system call)

* User interface 命令接口
	* Command Line interface(CLI)
	* Graphics user interface, Touch-Screen, Voice

* System calls 系统调用，程序接口
	* programming interface between processes and OS kernel

* API 
	* 应用编程接口是一组函数定义，而**系统调用**是通过软中断向内核发送请求，每个系统调用对应一个封装例程（wrapper routine, 唯一目的就是发送系统调用）
	* 系统调用的时显示在内核完成的，而用户态的函数在函数库实现

* ABI ： Application Binary Interface(应用程序二进制接口)
	* 描述应用程序与操作系统，应用与库或应用的组成部分之间的低接口
___
### Linux内核
* Linux是单内核（微内核），内核运行在单独的地址空间，实现了模块化设计，可以动态装载和卸载内核模块
* 内核文件夹内容: arch体系结构相关 mn内存管理 fs文件系统 driver驱动 include内核编译头文件 init内核初始化代码 kernel主要的内核代码......
* 内核开发特点：
	内核变成不能访问C库（如printf等库函数）
	必须使用GNU C ...
___
### Chapter3:Processes
进程：执行时的程序
jobs = user programs = tasks = process

* 进程状态
	* New **Running Ready Waiting** Terminated
	* 进程状态转换的条件
		Program action(system call)
		OS action(scheduling decision)
		External action(interrupts)

* 并发与并行
	并发针对单核 CPU 而言，它指的是 CPU 交替执行不同任务的能力；并行针对多核 CPU 而言，它指的是多个核心同时执行多个任务的能力

* PCB progress control block 操作系统存储的进程相关信息（进程状态，pc，寄存器，cpu schedule信息）

* windows中很少从父进程创建子进程一般创建线程，而linux多用fork函数创建子进程

* 线程vs进程 线程可以节省资源（享有统一内存空间），速度更快（多核并行）

* 线程：用户级线程，内核级线程，两者结合

* 线程共享的内容包括：  
	0、进程代码段  
	1、进程的公有数据(利用这些共享的数据，线程很容易的实现相互之间的通讯)  
	2、进程打开的文件描述符
	3、信号的处理器
	4、进程的当前目录  
	5、进程用户ID与进程组ID   
* 线程独有的内容包括：  
	1、线程ID  
	2、寄存器组的值  
	3、线程的**栈**  
	4、错误返回码  
	5、线程的信号屏蔽码

