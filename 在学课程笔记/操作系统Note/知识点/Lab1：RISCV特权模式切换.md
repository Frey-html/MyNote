在RISC-V架构中，特权模式的进入和退出是通过特定的指令来完成的。RISC-V定义了三种主要的特权模式：用户模式（User Mode）、监管模式（Supervisor Mode）和机器模式（Machine Mode）。每种模式的权限级别不同，适用于不同的操作场景。
[RISC-V架构的三种特权模式如何切换_riscv m和u mode-CSDN博客](https://blog.csdn.net/weixin_42031299/article/details/136843807)
[[Lab：中断处理与CSR]]

![[Pasted image 20241011175056.png]]
![[Pasted image 20241013144236.png]]


### 特权模式的进入和退出

1. **用户模式（User Mode）**
    
    - **进入**：通常通过系统调用（`ecall`指令）从用户模式进入更高特权模式（监管模式或机器模式）。
    - **退出**：从更高特权模式返回到用户模式时，使用`mret`或`sret`指令。
2. **监管模式（Supervisor Mode）**
    
    - **进入**：可以通过系统调用（`ecall`指令）或异常从用户模式进入监管模式。
    - **退出**：使用`sret`指令从监管模式返回到用户模式。
3. **机器模式（Machine Mode）**
    
    - **进入**：通常在系统启动时默认进入机器模式，或者通过异常和中断从其他模式进入机器模式。
    - **退出**：使用`mret`指令从机器模式返回到之前的模式（监管模式或用户模式）。

![[Pasted image 20241013151853.png]]
![[Pasted image 20241013151942.png]]

### 关键指令

- [**`mret`（Machine Return）**：用于从机器模式返回到先前的模式。执行`mret`指令时，处理器会恢复之前保存的状态，并跳转到保存的程序计数器（PC）地址](https://blog.csdn.net/zyhse/article/details/136322722)[1](https://blog.csdn.net/zyhse/article/details/136322722)。
- [**`sret`（Supervisor Return）**：用于从监管模式返回到先前的模式。类似于`mret`，`sret`指令会恢复监管模式下的状态，并跳转到保存的PC地址](https://blog.csdn.net/weixin_42031299/article/details/136843807)[2](https://blog.csdn.net/weixin_42031299/article/details/136843807)。
- [**`ecall`（Environment Call）**：用于从用户模式进入更高特权模式（监管模式或机器模式）。处理器会根据当前模式和异常委托寄存器（`medeleg`）的设置，决定进入哪个特权模式](https://blog.csdn.net/weixin_42031299/article/details/136843807)[2](https://blog.csdn.net/weixin_42031299/article/details/136843807)。

在 RISC-V 架构中，确实有一个 CSR（控制和状态寄存器）专门用于存储当前的特权模式。这是 **`mstatus`**（机器状态寄存器），其中包含了系统的许多状态信息，包括当前的特权模式。

### 关键寄存器：`mstatus` 和 `sstatus`

1. **`mstatus`（Machine Status Register）**:
   - 这是机器模式下的状态寄存器，其中存储了与特权级别相关的信息。
   - `mstatus` 寄存器的 **MPP（Machine Previous Privilege）字段** 用于存储之前的特权模式，在处理异常和中断时用于保存并恢复特权模式。
   - MPP 的值决定了下一个特权模式，取值如下：
     - `00`: 用户模式（User mode）
     - `01`: 保留
     - `10`: 超级模式（Supervisor mode）
     - `11`: 机器模式（Machine mode）
   - 当前的特权模式也可能通过 MPP 的值来推断。

2. **`sstatus`（Supervisor Status Register）**:
   - 当 RISC-V 支持超级模式（Supervisor mode）时，`sstatus` 寄存器保存了超级模式的状态。
   - `sstatus` 寄存器中的 **SPP（Supervisor Previous Privilege）字段** 用于存储进入中断或异常处理前的特权模式（用于超级模式的异常处理）。

### 如何确定当前特权模式？
[[Lab2：CSRRegs-MPP位]]

虽然 `mstatus` 和 `sstatus` 保存的是**上一个特权模式**的信息（例如 MPP 和 SPP），要明确当前的特权模式可以通过以下方式：

- 在进入异常处理时，系统会保存当前特权模式到 `mstatus.MPP` 或 `sstatus.SPP` 中，并进入机器模式或超级模式。
- 系统可以根据当前所处的特权级别（机器模式、超级模式或用户模式）来判断。例如，当执行指令时，处理器在机器模式下运行的代码不会改变 `mstatus.MPP`，而是利用 `mstatus` 的其他字段。

因此，`mstatus` 和 `sstatus` 寄存器虽然保存的是**之前**的特权模式信息，但间接地也能帮助确定当前的特权模式。在实际的运行时上下文中，当前特权模式是由硬件根据指令执行环境确定的。