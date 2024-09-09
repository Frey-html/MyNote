RISC-V 是一种简洁且模块化的指令集架构（ISA），它具有多种标准和扩展指令集。在这里，我将按照不同的类别为你介绍尽可能多的 RISC-V 基础指令集和一些常见的扩展指令。
![[Pasted image 20240909112336.png]]
单周期 cpu 数据通路：
![[Pasted image 20240909112748.png]]
五级流水线 CPU（Lab 1 实现forwarding）原理图
![[Pasted image 20240909152719.png]]


### 1. **整数计算指令集（I 指令集，RV 32 I/64 I/128 I）**
这些是 RISC-V 的基本整数指令，适用于 32 位、64 位和 128 位的 RISC-V 架构。
#### 算术指令：
- **ADD rd, rs 1, rs 2**：`rd = rs1 + rs2`（加法）
- **SUB rd, rs 1, rs 2**：`rd = rs1 - rs2`（减法）
- **LUI rd, imm**：将 20 位立即数 imm 左移 12 位，并放入 `rd`，即 `rd = imm << 12`（加载上半部分立即数）
- **AUIPC rd, imm**：`rd = PC + (imm << 12)`（相对 PC 加载上半部分立即数）
  
#### 逻辑指令：
- **AND rd, rs 1, rs 2**：`rd = rs1 & rs2`（按位与）
- **OR rd, rs 1, rs 2**：`rd = rs1 | rs2`（按位或）
- **XOR rd, rs 1, rs 2**：`rd = rs1 ^ rs2`（按位异或）

#### 移位指令：
- **SLL rd, rs 1, rs 2**：`rd = rs1 << rs2`（逻辑左移）
- **SRL rd, rs 1, rs 2**：`rd = rs1 >> rs2`（逻辑右移）
- **SRA rd, rs 1, rs 2**：`rd = rs1 >>> rs2`（算术右移）
  
#### 比较指令：
- **SLT rd, rs 1, rs 2**：`rd = (rs1 < rs2)`（有符号小于）
- **SLTU rd, rs 1, rs 2**：`rd = (rs1 < rs2)`（无符号小于）

#### 立即数运算：
- **ADDI rd, rs 1, imm**：`rd = rs1 + imm`（加立即数）
- **ANDI rd, rs 1, imm**：`rd = rs1 & imm`（与立即数）
- **ORI rd, rs 1, imm**：`rd = rs1 | imm`（或立即数）
- **XORI rd, rs 1, imm**：`rd = rs1 ^ imm`（异或立即数）
- **SLLI rd, rs 1, shamt**：`rd = rs1 << shamt`（逻辑左移立即数）
- **SRLI rd, rs 1, shamt**：`rd = rs1 >> shamt`（逻辑右移立即数）
- **SRAI rd, rs 1, shamt**：`rd = rs1 >>> shamt`（算术右移立即数）
- **SLTI rd, rs 1, imm**：`rd = (rs1 < imm)`（有符号立即数比较）
- **SLTIU rd, rs 1, imm**：`rd = (rs1 < imm)`（无符号立即数比较）

### 2. **分支和跳转指令**

#### 条件分支：
- **BEQ rs 1, rs 2, offset**：如果 `rs1 == rs2`，跳转到 `PC + offset`
- **BNE rs 1, rs 2, offset**：如果 `rs1 != rs2`，跳转到 `PC + offset`
- **BLT rs 1, rs 2, offset**：如果 `rs1 < rs2`（有符号），跳转到 `PC + offset`
- **BGE rs 1, rs 2, offset**：如果 `rs1 >= rs2`（有符号），跳转到 `PC + offset`
- **BLTU rs 1, rs 2, offset**：如果 `rs1 < rs2`（无符号），跳转到 `PC + offset`
- **BGEU rs 1, rs 2, offset**：如果 `rs1 >= rs2`（无符号），跳转到 `PC + offset`

#### 无条件跳转：
- **JAL rd, offset**：跳转到 `PC + offset` 并将 `PC + 4` 保存到 `rd` 中（用于函数调用）
- **JALR rd, rs 1, offset**：跳转到 `(rs1 + offset) & ~1`，并将 `PC + 4` 保存到 `rd` 中（寄存器间接跳转）

### 3. **内存访问指令**

#### 加载指令：
- **LB rd, offset (rs 1)**：从 `rs1 + offset` 的内存地址处加载一个字节，符号扩展后存入 `rd`
- **LBU rd, offset (rs 1)**：从 `rs1 + offset` 的内存地址处加载一个字节，零扩展后存入 `rd`
- **LH rd, offset (rs 1)**：从 `rs 1 + offset` 的内存地址处加载半字，符号扩展后存入 `rd`
- **LHU rd, offset (rs 1)**：从 `rs 1 + offset` 的内存地址处加载半字，零扩展后存入 `rd`
- **LW rd, offset (rs 1)**：从 `rs 1 + offset` 的内存地址处加载一个字（32 位），存入 `rd`
- **LD rd, offset (rs 1)**：从 `rs 1 + offset` 的内存地址处加载双字（64 位），存入 `rd`（64 位架构）

#### 存储指令：
- **SB rs 2, offset (rs 1)**：将寄存器 `rs 2` 的最低 8 位存储到 `rs 1 + offset` 的内存地址处
- **SH rs 2, offset (rs 1)**：将寄存器 `rs 2` 的最低 16 位存储到 `rs 1 + offset` 的内存地址处
- **SW rs 2, offset (rs 1)**：将寄存器 `rs 2` 的 32 位存储到 `rs 1 + offset` 的内存地址处
- **SD rs 2, offset (rs 1)**：将寄存器 `rs 2` 的 64 位存储到 `rs 1 + offset` 的内存地址处（64 位架构）

### 4. **原子性指令（A 扩展，RV 32 A/64 A）**

原子操作指令用于多线程环境中的同步操作。

- **LR. W rd, rs 1**：加载并保留 32 位字（load-reserved）
- **SC. W rd, rs 2, rs 1**：尝试存储条件（store-conditional），如果成功则将 `rs 2` 存储到 `rs 1`，并将结果状态存储到 `rd`
- **AMOSWAP. W rd, rs 2, rs 1**：交换 `rs 2` 和内存中 `rs 1` 地址处的 32 位字
- **AMOADD. W rd, rs 2, rs 1**：将 `rs 2` 加到 `rs 1` 地址处的 32 位字，并将结果存储到 `rd`

### 5. **乘除法指令（M 扩展，RV 32 M/64 M）**

RISC-V M 扩展用于整数乘法和除法操作。

- **MUL rd, rs 1, rs 2**：`rd = rs 1 * rs 2`（乘法）
- **MULH rd, rs 1, rs 2**：`rd = (rs 1 * rs 2)` 高 32 位（有符号乘法）
- **MULHU rd, rs 1, rs 2**：`rd = (rs 1 * rs 2)` 高 32 位（无符号乘法）
- **DIV rd, rs 1, rs 2**：`rd = rs 1 / rs 2`（有符号除法）
- **DIVU rd, rs 1, rs 2**：`rd = rs 1 / rs 2`（无符号除法）
- **REM rd, rs 1, rs 2**：`rd = rs 1 % rs 2`（有符号取余）
- **REMU rd, rs 1, rs 2**：`rd = rs 1 % rs 2`（无符号取余）

### 6. **浮点运算指令（F 和 D 扩展，RV 32 F/64 F/32 D/64 D）**

浮点扩展支持单精度（32 位）和双精度（64 位）浮点数运算。

- **FADD. S rd, rs 1, rs 2**：浮点加法（单精度）
- **FSUB. S rd, rs 1, rs 2**：浮点减法（单精度）
- **FMUL. S rd, rs 1, rs 2**：浮点乘法（单精度）
- **FDIV. S rd, rs 1, rs2** ：浮点除法（单精度）
- **FADD. D rd, rs 1, rs 2**：浮点加法（双精度）
- **FSUB. D rd, rs 1, rs 2**：浮点减法（双精度）

### 7. **系统控制指令**

- **ECALL**：系统调用
- **EBREAK**：调试断点
- **FENCE**：内存同步屏障，用于确保内存操作的顺序
