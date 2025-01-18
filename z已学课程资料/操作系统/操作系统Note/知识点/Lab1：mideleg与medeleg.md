[RISC-V架构——中断委托和中断注入_mideleg-CSDN博客](https://blog.csdn.net/weixin_42031299/article/details/134018259)
统称 machine trap delegation registers，mideleg（machind interrupt delegation register）用于保存哪些中断要委托给 S 模式进行处理。medeleg（machine exception delegation register）用于保存哪些异常要委托给 S 模式进行处理。

![[Pasted image 20241013144445.png]]

中断码：

- 1: Supervisor Software Interrupt
- 3: Machine Software Interrupt
- 5: Supervisor Timer Interrupt
- 7: Machine Timer Interrupt
- 9: Supervisor External Interrupt
- 11: Machine External Interrupt

异常码：

- 0: Instruction Address Misaligned
- 1: Instruction Access Fault
- 2: Illegal Instruction
- 3: Breakpoint
- 4: Load Address Misaligned
- 5: Load Access Fault
- 6: Store/AMO Address Misaligned
- 7: Store/AMO Access Fault
- 8: Environment Call from U-mode
- 9: Environment Call from S-mode
- 11: Environment Call from M-mode
- 12: Instruction Page Fault
- 13: Load Page Fault
- 15: Store/AMO Page Fault

需要委托哪些中断 / 异常，就将对应寄存器的对应位设置为 1，不需要委托就设置为 0。