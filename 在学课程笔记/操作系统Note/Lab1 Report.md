### 浙江大学实验报告
---
课程名称：操作系统
实验项目名称：RV64 内核引导与时钟中断处理
学生姓名：展翼飞  学号：3190102196
电子邮件地址：1007921963@qq.com
实验日期： 2024年 10 月 7 日

#### 一、实验内容

##### 1. 准备工程
* 进入os24fall文件夹并使用git命令拉去最新实验工程文件，并检查
```bash
cd os24fall   #进入工程文件夹
git pull      #拉取更新
cd src/lab1   #进入相应文件夹检查
```

##### 2. RV64内核引导
###### 2.1 完善 Makefile 脚本
![[Pasted image 20241008165525.png]]
在根目录的 Makefile 中存在 all 任务为目标任务，该任务依次进入 lib 等子文件夹运行相应目录下 Makfile 文件进行工程编译。在 lib 子文件夹中，仅需使用根目录 Makefile export 的编译变量对 printk c 文件进行编译打包。
###### 2.2 编写 head. S 
为即将运行的第一个 C 函数设置程序栈（栈的大小可以设置为 4 KB），并将该栈放置在 `.bss.stack` 段
![[Pasted image 20241008174241.png]]
在内核引导过程中，OpenSBI 会将 Linux 内核映像加载到内存中。这包括 `head.s`，这是内核启动的汇编代码，负责完成初始设置。它为即将运行的第一个 C 函数设置程序栈，并将该栈放置在 `.bss.stack` 段。接下来我们只需要通过跳转指令，跳转至 main.c 中的 `start_kernel` 函数

###### 2.3 补充sbi.c
![[Pasted image 20241008161830.png]]
使用内联汇编与命名占位符格式编写 sbi_ecall，完成以下内容：
1. 将 `eid`（Extension ID）放入寄存器 `a7` 中，`fid`（Function ID）放入寄存器 `a6` 中，将 `arg[0-5]` 放入寄存器 `a[0-5]` 中。
2. 使用 `ecall` 指令。`ecall` 之后系统会进入 M 模式，之后 OpenSBI 会完成相关操作。
3. OpenSBI 的返回结果会存放在寄存器 `a0`，`a1` 中，其中 `a0` 为 error code，`a1` 为返回值，我们用 `sbiret` 来接受这两个返回值。

完成 makefile 与sbi_ecall 编写后，尝试在根目录进行 make，出现以下错误信息：
![[Pasted image 20241008164612.png]]
第一个错误表明链接过程中，尝试将使用双精度浮点数的模块与使用软浮点的模块链接在一起。这通常表明编译选项不一致。需要检查 `Makefile` 中是否有关于浮点 ABI 的设置，并确保所有模块都使用相同的设置。
第二个错误表明在 `printk.c` 中调用了一些函数，但链接器找不到这些函数的定义。通常，`__stack_chk_guard` 是与栈保护机制相关的符号，可能与编译选项或库的链接有关。解决这个问题可以尝试：确保启用了栈保护机制，并且链接了适当的库。

经检查，lib 中的 makefile 编译命令选项 `{CFLAG}` 被写成了 `{CFLAGS}`，修改后编译完成
![[Pasted image 20241008165722.png]]

###### 2.4 修改defs
![[Pasted image 20241008171000.png]]
使用 `#csr` 的目的是为了将宏参数 `csr` 转换为字符串，使得内联汇编能够使用合适的 CSR 名称，确保生成正确的汇编指令。
如当我们调用 `csr_read(mstatus)` 时（此时宏的参数为直接的 csr 寄存器名字字符串没有加双引号），宏会展开为：
```C
({
  uint64_t __v;
  asm volatile("csrr %0, " "mstatus" : "=r"(__v));
  __v;
})

```


##### 3. RV64时钟中断处理
###### 3.0 准备工作

###### 3.1 开启trap处理

###### 3.2 实现上下文切换

###### 3.3 实现trap处理函数

###### 3.4 实现时钟中断处理函数

###### 3.5 修改 test 函数

###### 3.6 编译及测试

#### 二、思考题
1. 请总结一下 RISC-V 的 calling convention，并解释 Caller / Callee Saved Register 有什么区别？


2. 编译之后，通过 System.map 查看 vmlinux.lds 中自定义符号的值并截图。


3. 用 `csr_read` 宏读取 `sstatus` 寄存器的值，对照 RISC-V 手册解释其含义并截图。


4. 用 `csr_write` 宏向 `sscratch` 寄存器写入数据，并验证是否写入成功并截图。


5. 详细描述你可以通过什么步骤来得到 `arch/arm64/kernel/sys.i`，给出过程以及截图。


6. 寻找 Linux v6.0 中 ARM32 RV32 RV64 x86_64 架构的系统调用表；
    - 请列出源代码文件，展示完整的系统调用表（宏展开后），每一步都需要截图。



7. 阐述什么是 ELF 文件？尝试使用 readelf 和 objdump 来查看 ELF 文件，并给出解释和截图。
    - 运行一个 ELF 文件，然后通过 `cat /proc/PID/maps` 来给出其内存布局并截图。
- ELF 文件：（Executable and Linkable Format）文件是一种可执行文件格式，它平台无关，可动态链接并可以包含调试信息。ELF文件将其内容分为多个部分，主要包括：
	- **程序头表（Program Header Table）**：定义了程序的执行结构，如加载到内存的地址、大小等。
	- **节头表（Section Header Table）**：包含文件各个节的描述信息，包括代码段、数据段、符号表等
	- **节（Sections）**： `.text`：代码段 `.data`：数据段等

8. 在我们使用 make run 时，OpenSBI 会产生如下输出：
    - 通过查看 [RISC-V Privileged Spec](https://github.com/riscv/riscv-isa-manual/releases/download/20240411/priv-isa-asciidoc.pdf) 中的 `medeleg` 和 `mideleg` 部分，解释上面 `MIDELEG` 和 `MEDELEG` 值的含义。。

#### 三、讨论心得
1. 使用 makefile 时需要注意编译命令的可选项，比如包含路径；在本实验中子文件夹 lib 中的 makefile 编译 printk 需要在编译命令加入包含路径· `${INCLUDE}`
2. 