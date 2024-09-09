### 浙江大学实验报告
---
课程名称：操作系统
实验项目名称：RV64 内核引导与时钟中断处理
学生姓名：展翼飞  学号：3190102196
电子邮件地址：1007921963@qq.com
实验日期： 2023年 10 月 7 日

#### 一、实验内容

##### 1. 准备工程
* 进入os23fall文件夹并使用git命令拉去最新实验工程文件，并检查
```bash
cd os23fall   #进入工程文件夹
git pull      #拉取更新
cd src/lab1   #进入相应文件夹检查
ls
```
![[Pasted image 20231010161922.png]]

##### 2. RV64内核引导
###### 2.1 编写head.S
为即将运行的第一个 C 函数设置程序栈（栈的大小可以设置为 4KB），并将该栈放置在`.bss.stack` 段

###### 2.1 完善Makefile脚本

###### 2.1 补充sbi.c

###### 2.1 修改defs


##### 3. RV64时钟中断处理
###### 3.0 准备工作

###### 3.1 开启trap处理

###### 3.2 实现上下文切换

###### 3.3 实现trap处理函数

###### 3.4 实现时钟中断处理函数

###### 3.5 编译及测试

#### 二、思考题
1. 请总结一下 RISC-V 的 calling convention，并解释 Caller / Callee Saved Register 有什么区别？
2. 编译之后，通过 System.map 查看 vmlinux.lds 中自定义符号的值（截图）。
3. 用 `csr_read` 宏读取 `sstatus` 寄存器的值，对照 RISC-V 手册解释其含义（截图）。
4. 用 `csr_write` 宏向 `sscratch` 寄存器写入数据，并验证是否写入成功（截图）。
5. Detail your steps about how to get `arch/arm64/kernel/sys.i`  
6. Find system call table of Linux v6.0 for `ARM32`, `RISC-V(32 bit)`, `RISC-V(64 bit)`, `x86(32 bit)`, `x86_64` List source code file, the whole system call table with macro expanded, screenshot every step.
7. Explain what is ELF file? Try readelf and objdump command on an ELF file, give screenshot of the output. Run an ELF file and cat `/proc/PID/maps` to give its memory layout.
8. 在我们使用 make run 时， OpenSBI 会产生如下输出 :通过查看 `RISC-V Privileged Spec` 中的 `medeleg` 和 `mideleg` ，解释上面 `MIDELEG` 值的含义。

#### 三、讨论心得