下面是一个实现上下文切换的汇编代码示例，符合你提到的要求。代码会在 `arch/riscv/kernel/entry.S` 文件中实现 trap 处理机制。

### 代码实现

```assembly
    .extern trap_handler
    .section .text.entry
    .align 2
    .globl _traps 
_traps:
    # 1. Save the context (32 registers and sepc)
    # Assuming sp points to the top of the stack
    # Stack frame layout:
    # |-------------------|
    # |     x31 (ra)      | <- sp + 0
    # |     ...           |
    # |     x1 (ra)       | <- sp + 124
    # |     sepc          | <- sp + 128
    # |-------------------|

    addi sp, sp, -128      # Allocate space for 32 registers (4 bytes each)
    
    # Save registers x0 to x31 (x0 is zero, so we skip it)
    sw ra, 124(sp)         # Save return address (x1)
    sw t0, 120(sp)         # Save t0 (x5)
    sw t1, 116(sp)         # Save t1 (x6)
    sw t2, 112(sp)         # Save t2 (x7)
    sw t3, 108(sp)         # Save t3 (x28)
    sw t4, 104(sp)         # Save t4 (x29)
    sw t5, 100(sp)         # Save t5 (x30)
    sw t6, 96(sp)          # Save t6 (x31)
    sw a0, 92(sp)          # Save a0 (x10)
    sw a1, 88(sp)          # Save a1 (x11)
    sw a2, 84(sp)          # Save a2 (x12)
    sw a3, 80(sp)          # Save a3 (x13)
    sw a4, 76(sp)          # Save a4 (x14)
    sw a5, 72(sp)          # Save a5 (x15)
    sw a6, 68(sp)          # Save a6 (x16)
    sw a7, 64(sp)          # Save a7 (x17)
    sw s0, 60(sp)          # Save s0 (x8)
    sw s1, 56(sp)          # Save s1 (x9)
    sw s2, 52(sp)          # Save s2 (x18)
    sw s3, 48(sp)          # Save s3 (x19)
    sw s4, 44(sp)          # Save s4 (x20)
    sw s5, 40(sp)          # Save s5 (x21)
    sw s6, 36(sp)          # Save s6 (x22)
    sw s7, 32(sp)          # Save s7 (x23)
    sw s8, 28(sp)          # Save s8 (x24)
    sw s9, 24(sp)          # Save s9 (x25)
    sw s10, 20(sp)        # Save s10 (x26)
    sw s11, 16(sp)        # Save s11 (x27)
    
    csrr a1, sepc          # Load sepc to a0
    csrr a0, scause        # Load scause to a1
    call trap_handler       # Call the trap handler

    # 3. Restore context
    lw ra, 124(sp)         # Restore return address
    lw t0, 120(sp)         # Restore t0
    lw t1, 116(sp)         # Restore t1
    lw t2, 112(sp)         # Restore t2
    lw t3, 108(sp)         # Restore t3
    lw t4, 104(sp)         # Restore t4
    lw t5, 100(sp)         # Restore t5
    lw t6, 96(sp)          # Restore t6
    lw a0, 92(sp)          # Restore a0
    lw a1, 88(sp)          # Restore a1
    lw a2, 84(sp)          # Restore a2
    lw a3, 80(sp)          # Restore a3
    lw a4, 76(sp)          # Restore a4
    lw a5, 72(sp)          # Restore a5
    lw a6, 68(sp)          # Restore a6
    lw a7, 64(sp)          # Restore a7
    lw s0, 60(sp)          # Restore s0
    lw s1, 56(sp)          # Restore s1
    lw s2, 52(sp)          # Restore s2
    lw s3, 48(sp)          # Restore s3
    lw s4, 44(sp)          # Restore s4
    lw s5, 40(sp)          # Restore s5
    lw s6, 36(sp)          # Restore s6
    lw s7, 32(sp)          # Restore s7
    lw s8, 28(sp)          # Restore s8
    lw s9, 24(sp)          # Restore s9
    lw s10, 20(sp)        # Restore s10
    lw s11, 16(sp)        # Restore s11

    addi sp, sp, 128       # Deallocate stack space

    mret                    # Return from trap (restore PC and status)
```

### 代码解释

1. **保存上下文**：
   - 使用 `sw` 指令将 CPU 的寄存器（包括 `ra` 和通用寄存器 `t0` 到 `t6`、`a0` 到 `a7`、`s0` 到 `s11`）存储到栈上。
   - 栈指针 `sp` 在保存寄存器前先向下调整，以分配足够的空间。

2. **调用 Trap 处理函数**：
   - 使用 `csrr` 指令读取 `sepc`（保存程序计数器）和 `scause`（保存异常原因），并将其传递给 `trap_handler` 函数。

3. **恢复上下文**：
   - 使用 `lw` 指令从栈中恢复先前保存的寄存器值，确保 CPU 恢复到异常发生前的状态。

4. **返回**：
   - 最后使用 `mret` 指令从 trap 中返回，恢复程序计数器和中断状态。

### 总结

- 该实现展示了 RISC-V 中的上下文切换机制，涉及寄存器保存、调用处理函数以及恢复状态的步骤。
- 这种机制对于实现操作系统的异常处理和任务切换至关重要。

### 补充：Scause 与 Sepc 变化时机
#### Scause
`scause` 寄存器的值在 RISC-V 中断和异常发生时会被更新。具体来说，它在以下情况下改变：

1. **发生中断或异常时**：
   - 当系统发生中断（如外部中断、时钟中断）或异常（如非法指令、越界访问等）时，CPU 会自动将 `scause` 的值设置为导致异常或中断的原因。这包括中断的类型和异常的原因码。

2. **处理完异常后**：
   - 在返回处理完的异常时，`scause` 的值不会被重置，除非系统重启或显式地通过软件进行清除。它保持在发生异常时的状态，以便进行后续调试或处理。

##### 例子

- **外部中断**：当一个外部设备产生中断时，`scause` 将会记录中断的类型，例如外部中断的原因码。
- **系统调用**：在发生系统调用时，`scause` 会被设置为表示系统调用的原因。
- **异常**：如果程序遇到非法指令或其他异常，`scause` 会被设置为相应的异常原因码。

#### Sepc
`sepc` 寄存器（Supervisor Exception Program Counter）在 RISC-V 架构中主要用于保存最后一次发生异常或中断时的程序计数器（PC）的值。它的值在以下情况下改变：

1. **发生中断或异常时**：
   - 当 CPU 检测到中断或异常发生时，当前的程序计数器（PC）会被保存到 `sepc` 中。这个值表示的是导致异常发生的指令地址。

2. **在异常处理返回时**：
   - 在处理完异常后，使用 `mret` 或其他相关指令返回时，系统会从 `sepc` 中读取值，确保程序能够从异常发生的位置继续执行。

##### 详细流程

- **保存 PC**：
  - 当发生异常（例如，外部中断、非法指令、访问权限错误等），CPU 会将此时的 PC 值（指向出错指令的地址）保存到 `sepc`。

- **处理异常**：
  - 操作系统的异常处理程序会使用 `sepc` 中的值来确定出错的位置，并进行相应的处理。

- **恢复执行**：
  - 处理完异常后，使用 `mret` 指令时，CPU 会读取 `sepc` 的值，将程序计数器恢复到异常发生前的状态，继续执行。