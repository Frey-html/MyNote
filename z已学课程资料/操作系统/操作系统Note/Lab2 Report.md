### 浙江大学实验报告
---
课程名称：操作系统
实验项目名称：RV64 内核线程调度
学生姓名：展翼飞  学号：3190102196
电子邮件地址：1007921963@qq.com
实验日期： 2024年 10 月 11 日

### 一、实验内容
#### 1. 准备工程
* 从lab1中拷贝lab1完成内容至lab2中，将如下宏定义添加进`defs.h` 
	![[Pasted image 20241018152902.png]]
	尝试编译，提示找不到`memset`函数定义，进行检查，发现`\lib`中除了`printk.c`均未编译，原因为makefile文件目标仅为`printk.o`，进行修改：![[Pasted image 20241018155115.png]]
	修改完成后工程正确编译

##### 1.1  `proc.h` 数据结构定义
* 阅读添加的`proc.h`，里面内置了线程状态数据结构与线程调度函数的头文件以及相关宏定义

#### 2. 线程调度功能实现
##### 2.1 线程初始化
* 在`proc.c`中按照要求步骤实现线程的初始化，包含分配物理页和初始化对应线程的 `task_struct`
	![[Pasted image 20241021095534.png]]![[Pasted image 20241021095605.png]]
	其中每个线程的`task_struct`放在物理页的起始地址，而其子结构`thread_struct`紧接着`task_struct`

##### 2.2 `__dummy` 与 `dummy` 的实现
1. `dummy`分析
	`dummy()`是除了`idle`线程外所有线程运行的代码，它记录线程被调度前的剩余运行时间`last_counter`，并在时钟中断导致线程被调度导致剩余运行时间`counter`减少时增大局部变量`auto_inc_local_var`标志线程工作进行，并使用`printk`打印线程`pid`等信息。
2.  `__dummy`的实现
	当线程在运行时，由于时钟中断的触发，会将当前运行线程的上下文环境保存在栈上；当线程再次被调度时，会将上下文从栈上恢复，但是当我们创建一个新的线程，此时线程的栈为空，当这个线程被调度时，是没有上下文需要被恢复的，所以我们需要为线程**第一次调度**提供一个特殊的返回函数 `__dummy`，在 `__dummy` 中将 sepc 设置为 `dummy()` 的地址，并使用 `sret` 从 S 模式中返回![[Pasted image 20241023091132.png]]

##### 2.3 实现线程切换
1. `proc.c`中`switch_to`实现
	判断下一个执行的线程 `next` 与当前的线程 `current` 是否为同一个线程，如果是同一个线程，则无需做任何处理，否则调用 `__switch_to` 进行线程切换：![[Pasted image 20241023102722.png]]
2. `entry.S`中`__switch_to`实现
	在 `entry.S` 中实现线程上下文切换 `__switch_to`：
	- `__switch_to` 接受两个 `task_struct` 指针作为参数；
	- 保存当前线程的 `ra`，`sp`，`s0~s11` 到当前线程的 `thread_struct` 中；
	- 将下一个线程的 `thread_struct` 中的相关数据载入到 `ra`，`sp`，`s0~s11` 中进行恢复：
	![[Pasted image 20241023102804.png]]
	此处`__switch_to`由`proc.c`的`switch_to`调用，`a0 a1`两个参数分别为当前线程和下一线程的物理页起始地址，而线程寄存器保存在物理页的`thread_struct`中，`thread_struct`是放在物理页起始地址的`task_struct`的成员变量，而它的偏移了四个`uint64_t`即32Bytes，因此偏移地址为32，由此可以依次存取`thread_struct` 中保存的线程寄存器

##### 2.4 实现调度入口函数
实现 `do_timer()` 函数，并在 `trap.c` 时钟中断处理函数中调用：
在`proc.c`中新增`do_timer`函数入下：
![[Pasted image 20241023103431.png]]
并在`trap.c`的`trap_handler`函数调用它，实现时钟中断发生时减少当前运行线程的剩余运行时间：
![[Pasted image 20241023103526.png]]

##### 2.5 线程调度算法实现
在`proc.c`中实现线程优先级调度算法`schedule`如下:
- `task_init` 的时候随机为各个线程赋予了优先级
- 调度时选择 `counter` 最大的线程运行
- 如果所有线程 `counter` 都为 0，则令所有线程 `counter = priority`
    - 即优先级越高，运行的时间越长，且越先运行
    - 设置完后需要重新进行调度
- 最后通过 `switch_to` 切换到下一个线程
![[Pasted image 20241023103727.png]]
每次调度时遍历`task`选择剩余运行时间最大的线程运行，并在所有线程没有剩余运行时间时重新根据优先级分配时间片。在线程切换与重新分配时间片时打印相应调试信息。

#### 3. 编译及测试
在根目录`Makefile`中添加如下变量
![[Pasted image 20241023104050.png]]
其中编译选项`-DTEST_SCHED`会在编译时添加相应宏定义`TEST_SCHED`
运行测试`make TEST_SCHED=1 run`:
![[Pasted image 20241023104336.png]]

测试顺利通过

### 二、思考题
1. **在 RV64 中一共有 32 个通用寄存器，为什么 `__switch_to` 中只保存了 14 个**？
	因为`__switch_to`实际上需要保存恢复的不是当前线程的所有寄存器状态，而是该线程在进入时钟中断已经将所有寄存器保存在栈上后，继续调用时钟中断处理函数直至调用`__switch_to`的`switch_to`函数的状态，因为`switch_to`没有参数返回值没有使用临时寄存器，因此只需要保存从时钟中断`__traps`保存后发生变化的`ra`,`sp`以及`s0-s11`
2. **阅读并理解 `arch/riscv/kernel/mm.c` 代码，尝试说明 `mm_init` 函数都做了什么，以及在 `kalloc` 和 `kfree` 的时候内存是如何被管理的。**
	1. `mm_init` 
		 `mm_init`调用`kfreerange`函数，将物理内存地址从`_ekernel`到`PHY_END`的空间，按页大小遍历打包成页（起始地址需要`Roundup`到下一个页起始地址保证页对齐），并将每一页内容清空，插入空闲页链表中。
	2. `kalloc`
		  `kalloc`在空闲页链表中取出第一个空闲页并清空，返回页地址，如果空闲页链表为空则返回0.
	3. `kfree`
		 `kfree`用于释放一个 4 KiB 的物理页，首先判断传入地址是否是页对齐，如果传入的地址不是按 PGSIZE 对齐则将其`Rounddown`。然后用0清空这一页，再将该清空后的空闲页插入空闲页链表的头部。
3. **当线程第一次调用时，其 `ra` 所代表的返回点是 `__dummy`，那么在之后的线程调用中 `__switch_to` 中，`ra` 保存 / 恢复的函数返回点是什么呢？请同学用 gdb 尝试追踪一次完整的线程切换流程，并关注每一次 `ra` 的变换（需要截图）。**
	* 使用GDB在`__switch_to`处打上断点，跟踪到Test中第一次线程切换时，使用`bt`打印函数栈帧，发现此时`ra`处在`proc.c`的`switch_to`函数中，所以第一次调用之后保存/恢复的函数返回点是`schedule`中调用的`switch_to`函数，之后再逐层函数栈返回至`_traps`，由`sret`返回`dummy`![[Pasted image 20241023135120.png]]![[Pasted image 20241023135203.png]]

4. **请尝试分析并画图说明 kernel 运行到输出第二次 `switch to [PID ...` 的时候内存中存在的全部函数帧栈布局。**
    - 可通过 gdb 调试使用 `backtrace` 等指令辅助分析，注意分析第一次时钟中断触发后的 `pc` 和 `sp` 的变化。
    在第一次以及第二次打印`switch to [PID..`时，使用`backtrace`命令打印函数栈帧，均有`task[0]`与`task[2]`(`PID = 2`的线程)栈帧如下，再加上进入中断前的函数，则有全部函数栈帧布局如图二![[Pasted image 20241023190249.png]]
	![[c10e948ee0527449fec9b1713d2c3ed.jpg]]


### 三、问题与心得
1. 在完成`do_timer`后，在`trap_handler`中调用它，如果将`do_timer`放置在时钟中断处理`clock_set_next_event`前，会导致线程在第一轮调度时所有线程第一次运行`dummy`前就进行一次`counter`自减，线程实际运行时间片比预期少1![[Pasted image 20241023101300.png]]![[Pasted image 20241023101906.png]]
	而在第二轮调度重新分配时间片后恢复正常![[Pasted image 20241023102411.png]]

2. 在`dummy`中存在如下代码![[Pasted image 20241023104700.png]]该代码会使`counter`在等于1时在`dummy`中自减至0，如果不注释掉，则需要在`do_timer`的`counter`自减前多加一条`counter==0`的判断（或者小于0同样进行调度），否则`counter`会变为负值且不能正常进行线程调度![[Pasted image 20241023104857.png]]![[Pasted image 20241023104911.png]]

