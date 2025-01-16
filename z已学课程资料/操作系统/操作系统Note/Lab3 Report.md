### 浙江大学实验报告
---
课程名称：操作系统
实验项目名称：RV64 虚拟内存管理
学生姓名：展翼飞  学号：3190102196
电子邮件地址：1007921963@qq.com
实验日期： 2024年 10 月 30 日
### 一、实验内容
#### 1. 准备工程
* 使用`git fetch`与`git merge`从仓库同步`lab3 vmlinux.lds`代码，并从`lab2`拷贝之前完成的代码![[Pasted image 20241029164503.png]]
* 在`defs.h`中添加本实验虚拟内存相应的宏定义![[Pasted image 20241029164655.png]]
* 修改根目录`Makefile`的`ISA`，支持指令屏障，使用刷新缓存的指令扩展![[Pasted image 20241029165112.png]]
#### 2.PIE
* 因为 GOT 表里的地址都是最终的虚拟地址，所以在 kernel 启用虚拟地址之前，一切从 GOT 表取出的地址都是错的，这种情况下需要手动给 `la` 得到的地址减去 `PA2VA_OFFSET` 才能得到正确的物理地址。
	为了避免这种情况，需要在 Makefile 的 `CF` 中加一个 `-fno-pie` 强制不编译出 PIE 的代码![[Pasted image 20241029165522.png]]

#### 3.开启虚拟内存映射

##### 3.1 `setup_vm` 的实现
1. **setup_vm:** 将 0x80000000 开始的 1GB 区域进行两次映射，其中一次是等值映射（PA == VA），另一次是将其映射到 `direct mapping area`（使得 `PA + PV2VA_OFFSET == VA`）。
	![[Pasted image 20241114103214.png]]
	 因为每个页表项进行1GiB的映射，所以对需要映射的虚拟地址右移30位可以得到该1GIB映射的页表项编号Index，等值映射和 `direct mapping area`分别对应根页表2和384项。
	 而它们映射到的物理页都是OpenSBI的起始地址页，也即物理地址0x8000000，将它右移12位得到SV39模式的4KiB物理页号0x8000，由于PTE中物理页号存储从第十位开始，放入PTE中仍需左移10位，得到需要放入PTE中的PPN。
	 完成这些后，我们可以对根页表中的第2和384项进行设置，完成虚拟地址的映射，只需要将PPN与0xf即15进行按位或，将页表项VRWX位置1，完成页表项设置。
2. **relocate:** 完成`relocate`编写实现设置satp寄存器与跳转到虚拟地址，在`head.s`中`_start`起始处调用`setup_vm`与`relocate`
	![[Pasted image 20241114105218.png]]
	首先需要将ra与sp，也即pc返回值与堆栈指针加上`PA2VA_OFFSET`，使其从`relocate`返回后运行在虚拟地址之上，并使用`sfence.vma`指令确保虚拟地址转换启用。
	随后将根页表物理地址右移12位得到根页表物理页号，而0x8000000000000000代表64位寄存器最高四位为8，在`satp`中代表使用SV39分页模式，二者相加并设置`satp`寄存器。
3. **修改mm_init:**
	修改 `arch/riscv/kernel/mm.c` 的 `mm_init` 函数，将结束地址调整为虚拟地址，保证`kalloc`正常运行![[Pasted image 20241114110416.png]]

##### 3.2 `setup_vm_final` 的实现
1. **`setup_vm_final` 的实现**
	在映射部分中，我们需要通过`system.map`寻找对应段的大小，可知`kernel`的`text` 和`rodata`分别需要映射2页和1页，而剩余的内存经计算共32768-（2 + 1）=32765页，调用`create_mapping`函数创建对应页表项的虚拟内存映射，随后以SV39模式通过内联汇编将根页表物理页号写入`satp`：
	![[Pasted image 20241115082706.png]]
	在`head.s`中调用`setup_vm_final`：![[Pasted image 20241115083536.png]]
2. `create_mapping`的实现
	![[Pasted image 20241124133553.png]]![[Pasted image 20241124133602.png]]

#### 4.编译与测试
尚未debug完成


### 二、思考题
1. **验证 `.text`，`.rodata` 段的属性是否成功设置，给出截图。**
	完成`setup_vm`后，编译并查看`system.map`，可知段成功设置在虚拟地址上![[Pasted image 20241114111748.png]]
2. **为什么我们在 `setup_vm` 中需要做等值映射？在 Linux 中，是不需要做等值映射的，请探索一下不在 `setup_vm` 中做等值映射的方法。你需要回答以下问题：**
    - **本次实验中如果不做等值映射，会出现什么问题，原因是什么；**
		如果不进行等值映射，那么在 `setup_vm` 函数完成设置页表并切换页表后，将开启虚拟地址，但程序运行的代码仍是物理地址，被视为虚拟地址后此段可能在页表中不存在有意义的物理地址映射。
    - **简要分析 [Linux v5.2.21](https://elixir.bootlin.com/linux/v5.2.21/source) 或之后的版本中的内核启动部分（直至 `init/main.c` 中 `start_kernel` 开始之前），特别是设置 satp 切换页表附近的逻辑；**
    - **回答 Linux 为什么可以不进行等值映射，它是如何在无等值映射的情况下让 pc 从物理地址跳到虚拟地址；**
    - **Linux v5.2.21 中的 `trampoline_pg_dir` 和 `swapper_pg_dir` 有什么区别，它们分别是在哪里通过 satp 设为所使用的页表的；**
    - **尝试修改你的 kernel，使得其可以像 Linux 一样不需要等值映射。**



### 三、问题与心得
1. 调试时在运行到`relocate`设置好 `satp` 寄存器之前，只可以使用**物理地址**来打断点。因为符号表、`vmlinux.lds` 里面记录的函数名的地址都是虚拟地址。如果使用符号的虚拟地址设置断点会导致程序不会遇到设置到的断点(虚拟地址相比物理地址高出offset)。可以在目录下编译生成的 `vmlinux.asm` 中找到所有代码的虚拟地址，然后将其转换成物理地址，再使用 `b *<addr>` 命令设置断点，设置 satp 之后，才可以使用虚拟地址打断点，同时之前设置的物理地址断点也会失效，需要删除。

2. 为什么setupvm_final对于kernel 的text rodata段与剩余内存要分开调用create mapping？
	因为他们映射的物理内存页需要设置的访问权限不同。

