### 浙江大学实验报告
---
课程名称：操作系统
实验项目名称：RV64 内核引导与时钟中断处理
学生姓名：展翼飞  学号：3190102196
电子邮件地址：1007921963@qq.com
实验日期： 2024年 10 月 11 日

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
![[Pasted image 20241010163146.png]]

使用内联汇编与命名占位符格式编写 sbi_ecall，完成以下内容：
1. 将 `eid`（Extension ID）放入寄存器 `a7` 中，`fid`（Function ID）放入寄存器 `a6` 中，将 `arg[0-5]` 放入寄存器 `a[0-5]` 中。
2. 使用 `ecall` 指令。`ecall` 之后系统会进入 M 模式，之后 OpenSBI 会完成相关操作。
3. OpenSBI 的返回结果会存放在寄存器 `a0`，`a1` 中，其中 `a0` 为 error code，`a1` 为返回值，我们用 `sbiret` 来接受这两个返回值。
 完成 sbi_ecall 后，其余 sbi 调用均可以通过 sbi_ecall 实现
 

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

重新编译并 make run，qemu 中可以正确得到字符输出并退出：
![[Pasted image 20241009162511.png]]

##### 3. RV64时钟中断处理
###### 3.0 准备工作
修改 `vmlinux.lds` 以及 `head.S`
![[Pasted image 20241009163536.png]]
![[Pasted image 20241009163447.png]]
修改 head. S 中 text 段段名，并在 lds 文件中添加相应的段排布

###### 3.1 开启trap处理
![[Pasted image 20241010152656.png]]
补充代码，在 `head.s` 中对 CSR 进行初始化，并调用 `sbi_set_timer` 调用时钟中断，开启 trap 处理
* 在 `start_kernel` 处打下短点，查看相关CSR 寄存器设置情况：
	 ![[Pasted image 20241010164155.png]]![[Pasted image 20241010163922.png]]
	可见 `sstatus` 第一位置为 1（即 ` sstatus[sie]` = 1），`sie[STIE]` 即 S 态时钟中断，以及 `stvec` 也成功设置，


###### 3.2 实现上下文切换

其中 `sepc` 的值不能通过 lw 和 sw 直接保存在栈上，故而保存时先存 `t0` 再通过 `t0` 间接 sw，恢复寄存器则先通过 `t0` 恢复 `sepc`
![[Pasted image 20241011133821.png]]
![[Pasted image 20241011133836.png]]
需要注意的是此处从 `_traps` 返回应该使用 `sret` 指令，它会读取 `mepc` 并置 `pc`，返回中断发生前状态，如果使用 `mret` 则会发生错误。

###### 3.3 实现trap处理函数
查询手册中中断 ID 相关内容，得到结果如下：
![[Pasted image 20241010190530.png]]
即 `S Mode` 中时钟中断 id 号为 5，编写如下的 `trap` 处理函数： ![[Pasted image 20241011134651.png]]
###### 3.4 实现时钟中断处理函数
实现 `clock.h` 头文件与 `clock.c`，方便其他库函数调用与顺利编译如下：
![[Pasted image 20241011135342.png]]
![[Pasted image 20241011135418.png]]


###### 3.5 修改 test 函数
修改 `test` 函数如下，与 `_trap` 的中断处理函数相呼应，每过一秒钟时间输出 `Kernel is running`，与每秒调用一次的时钟中断匹配
![[Pasted image 20241011134903.png]]


###### 3.6 编译及测试
经检查，`Makefile` 文件中的依赖与目标文件模式匹配能够支持新增的 `.c .s` 文件正常编译链接，进行 `make run` 结果如下：
![[Pasted image 20241011134601.png]]


#### 二、思考题
1. 请总结一下 RISC-V 的 calling convention，并解释 Caller / Callee Saved Register 有什么区别？
	RISC-V 中寄存器关于函数调用的，`ra` 保存函数返回的地址，`sp` 指向栈顶管理函数调用过程中的局部变量和上下文保存。`a0 - a7` 一般作为函数调用参数和返回值 `a0 a1` 常用于返回值，`t0-t6` 为临时寄存器，他们为 `Caller saved register`。而保存寄存器 `s0-s11` 为帧指针局部变量等函数上下文，为 `callee saved register`
	 常见的调用过程为：保存 `Caller saved register`，传递参数到 `a0-a7`，使用跳转指令调用函数，保存 `Callee saved register`，函数执行，恢复 `Callee saved register`，传递返回值到 `a0-a1`，使用跳转指令返回，读取返回值，恢复 `Caller saved register`
	 这两种寄存器主要区别在于功能不同（前者作为函数参数和临时寄存器，后者存储函数上下文信息）与保存时机不同（前者调用者保存，后者被调用者保存）

2. 编译之后，通过 System.map 查看 vmlinux.lds 中自定义符号的值并截图。
![[Pasted image 20241011135509.png]]


3. 用 `csr_read` 宏读取 `sstatus` 寄存器的值，对照 RISC-V 手册解释其含义并截图。
	在 `main.c` 中使用 `csr_read` 宏读取 `sstatus` 得到结果如下：
	![[Pasted image 20241011143224.png]]![[Pasted image 20241011143124.png]]
		查询手册：![[Pasted image 20241011144857.png]]
		`csr_read` 结果说明 ：
			`UIE` = 0, user模式中断使能关闭
			`SIE` = 1, supervisor 模式中断使能开启
			`UPIE = SPIE = 0`，陷入中断或异常前，两个模式的中断使能关闭 
			`SPP = 0` 陷入异常时 S 模式前一个模式为U

4. 用 `csr_write` 宏向 `sscratch` 寄存器写入数据，并验证是否写入成功并截图。
	`sscratch` 用于 s 模式的上下文保存和恢复，在 `main.c` 中使用 `csr_write` 宏写入 `sscratch` 得到结果如下：![[Pasted image 20241011154717.png]] ![[Pasted image 20241011154736.png]]

5. 详细描述你可以通过什么步骤来得到 `arch/arm64/kernel/sys.i`，给出过程以及截图。
	1. 安装交叉编译工具链
		![[Pasted image 20241011163522.png]]![[Pasted image 20241011163351.png]]
	2. 进入 linux 内核文件夹，设置编译配置为 `arm64` 默认配置:
		![[Pasted image 20241011164026.png]]
	3. 指定需要生成的中间文件：`make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- ./arch/arm64/kernel/sys.i`
		![[Pasted image 20241011164301.png]]


6. 寻找 Linux v6.0 中 ARM32 RV32 RV64 x86_64 架构的系统调用表；
    - 请列出源代码文件，展示完整的系统调用表（宏展开后），每一步都需要截图。
	相应架构的系统调用表在 `./arch/*` 路径下，不需要编译对应内核即可查看，但每一种架构路径有所不同
	* ARM 32: `arch/arm/tools/syscall.tbl`
		![[Pasted image 20241011165626.png]]
	* RV 32/64: `arch/riscv/kernel/syscall_table.c`:
		![[Pasted image 20241011170003.png]]
	* x 86_64: `arch/x86/entry/syscalls/syscall_64.tbl`
		![[Pasted image 20241011170250.png]]

7. 阐述什么是 ELF 文件？尝试使用 readelf 和 objdump 来查看 ELF 文件，并给出解释和截图。
    - 运行一个 ELF 文件，然后通过 `cat /proc/PID/maps` 来给出其内存布局并截图。
	- ELF 文件：（Executable and Linkable Format）文件是一种可执行文件格式，它平台无关，可动态链接并可以包含调试信息。ELF文件将其内容分为多个部分，主要包括：
		- **程序头表（Program Header Table）**：定义了程序的执行结构，如加载到内存的地址、大小等。
		- **节头表（Section Header Table）**：包含文件各个节的描述信息，包括代码段、数据段、符号表等
		- **节（Sections）**： `.text`：代码段 `.data`：数据段等
		- **程序表头(Program Header Table)**：文件在运行时需要加载的段
		- **符号表 (Symbol Table)**：包含了程序中所有符号的信息，如函数名、变量名
		- **重定位表 (Relocation Table)**: 包含需要在加载或链接时修改的地址信息

	 1. 创建一个简单的 c 文件，并编译它，得到 elf 文件：
		![[Pasted image 20241011170629.png]]![[Pasted image 20241011170747.png]]
	 2. 使用 readelf 查看 elf 文件:
		* `readelf` 是一个专门用于显示ELF文件信息的工具。它可以显示ELF文件的各种头信息、节信息、符号表等。
		* 查看 elf 头部信息： ![[Pasted image 20241011171012.png]]
		* 查看节表头：![[Pasted image 20241011171210.png]]
		* 查看程序头表：![[Pasted image 20241011171313.png]]
		* 查看符号表：![[Pasted image 20241011171431.png]]
		* 查看重定位表：![[Pasted image 20241011171520.png]]
	3. 使用 objdump 查看 elf 文件：
		* `objdump` 是一个功能更为广泛的二进制文件分析工具。除了显示ELF文件的信息外，它还可以反汇编代码段，显示调试信息等。
		* 显示文件头信息（节表头）：![[Pasted image 20241011172120.png]]
		* 查看所有节的内容：![[Pasted image 20241011172226.png]]
		* 反汇编代码段：![[Pasted image 20241011172301.png]]


1. 在我们使用 make run 时，OpenSBI 会产生如下输出：
    - 通过查看 [RISC-V Privileged Spec](https://github.com/riscv/riscv-isa-manual/releases/download/20240411/priv-isa-asciidoc.pdf) 中的 `medeleg` 和 `mideleg` 部分，解释上面 `MIDELEG` 和 `MEDELEG` 值的含义。
    - Boot HART MIDELEG : 0x0000000000000222
    - Boot HART MEDELEG : 0x000000000000b109
    这两个寄存器统称 machine trap delegation registers，mideleg（machind interrupt delegation register）用于保存哪些中断要委托给 S 模式进行处理。medeleg（machine exception delegation register）用于保存哪些异常要委托给 S 模式进行处理。
	 上图中 `MIDELEG` 位 1,5,9 置 1代表委托 `Supervisor Software Interrupt`、`Supervisor Timer Interrupt` 、`Supervisor External Interrupt` 给 S 模式，其余给 M 模式处理。
	 而 ``MEDELEG`` 表示位 0,3,9,12 置 1 代表委托了 `Instruction Address Misaligned`、`Breakpoint`、`Environment Call from S-mode`、`Instruction Page Fault` 给 S 模式，其余给 M 模式处理。

#### 三、讨论心得
1. 使用 makefile 时需要注意编译命令的可选项，比如包含路径；在本实验中子文件夹 lib 中的 makefile 编译 printk 需要在编译命令加入包含路径· `${INCLUDE}`。
2. 对于每一个子文件的完成，可以先注释掉对于后面未完成函数的调用，先测试本文件基本功能是否正常，保证每一个子文件 debug 完成，减少最后整体工程调试工作量与出错可能性。
3. 需要注意函数返回使用 mret 还是 sret 或者 jalr，特权等级与指令不匹配会导致报错