这段代码实现了一个用于 RISC-V 架构的简单内核虚拟内存的初始设置。具体来说，它是在 RISC-V 的 RV39 虚拟内存模式下进行的等值映射（即虚拟地址和物理地址相同），并为早期的内核启动过程提供 1GB 的地址空间映射。

[[Lab3 Report]]

### 1. `early_pgtbl` 页表的初始化

```c
unsigned long early_pgtbl[512] __attribute__((__aligned__(0x1000)));
```

这里定义了一个名为 `early_pgtbl` 的页表，包含 512 个 `unsigned long` 类型的条目，并且使用 `__aligned__(0x1000)` 来确保它按 4KB（0x1000 字节）对齐，这是页表的基本要求。

### 2. `setup_vm` 函数的作用

```c
void setup_vm(void) {
```

`setup_vm` 函数的任务是在早期启动阶段设置一个简单的虚拟内存映射。其核心目的是使用 1GB 的大页来映射部分内核区域，从而可以快速建立一个简单的等值映射内存环境。以下是代码的逐行解释：

### 3. 页表映射的结构说明

```c
/*
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12，
        即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
*/
```

在 RISC-V 的 RV39 虚拟内存模式下，虚拟地址的最高位用于符号扩展，9 位用于根页表的索引，低 30 位用于指定页内偏移。每个页表条目指向一个 1GB 的物理内存区域。

- **高位**：在 64 位虚拟地址中，最高位可以忽略。
- **中间 9 位**：作为根页表的索引。即在 `early_pgtbl` 中的某个条目位置。
- **低 30 位**：作为页内偏移，因此每个条目对应 1GB 的内存区域（\(2^{30}\) 字节 = 1GB）。

### 4. 设置具体的映射条目

代码通过设置 `early_pgtbl` 的条目 `early_pgtbl[2]` 和 `early_pgtbl[384]` 来实现两个等值映射：

#### (1) 等值映射 `0x80000000` 地址

```c
// virtual address = 0x80000000 => VPN[2] = 2
early_pgtbl[2] = (uint64)(0 | 0x20000000U | 15U);
```

- `0x80000000` 是 RISC-V 内核的起始虚拟地址，它会被映射到物理地址 `0x80000000`。
- `VPN[2] = 2` 表示在 `early_pgtbl` 的第 2 个条目中设置这个映射。
- 条目值为 `0x20000000 | 15`：
  - `0x20000000`：实际的物理地址（这里等于 `0x80000000` >> 2）。
  - `15U`：表示页表条目的权限位设置为 `V`（有效）、`R`（可读）、`W`（可写）、`X`（可执行），即有效且支持读写和执行。

#### (2) 等值映射 `0xffffffe000000000` 地址

```c
// virtual address = 0xffffffe000000000 => VPN[2] = 384
early_pgtbl[384] = (uint64)(0 | 0x20000000U | 15U);
```

- `0xffffffe000000000` 是一个高位虚拟地址（通常为内核的备用高地址），映射到相同的物理地址 `0x80000000`。
- `VPN[2] = 384` 表示在 `early_pgtbl` 的第 384 个条目中设置这个映射。
- 条目值同样设置为 `0x20000000 | 15`，代表映射到物理地址 `0x80000000`，并开启读、写、执行权限。

### 5. 打印映射完成信息

```c
printk("...setup_vm done!\n");
```

在内核启动时打印一条信息，表示虚拟内存设置完成。

### 总结

- 该代码建立了两个 1GB 的等值映射，分别从 `0x80000000` 和 `0xffffffe000000000` 映射到物理地址 `0x80000000`。
- 权限位 `V | R | W | X` 全部开启，使这些页表条目在内核启动阶段可读、可写、可执行。

```cpp
// arch/riscv/kernel/vm.c
#include "defs.h"
#include "types.h"
#include "vm.h"
#include "mm.h"
#include "string.h"
#include "printk.h"

/* 
early_pgtbl：用于 setup_vm 函数进行早期内核启动阶段的 1GB 等值映射。 
这是一个简单的根页表，大小为 512 个条目（每个条目占 8 字节），
因此可以表示 512 * 1GB 的虚拟地址空间。
*/
unsigned long early_pgtbl[512] __attribute__((__aligned__(0x1000)));

void setup_vm(void) {
    /* 
    虚拟内存设置说明：
    1. 因为这里只需要 1GB 等值映射，所以不需要多级页表。
    2. 将虚拟地址（va）的 64 位按如下方式划分：
        | high bit | 9 bit | 30 bit |
        - high bit（最高位）：用于符号扩展，这里可以忽略。
        - 中间 9 bit：用于索引 early_pgtbl 中的条目。
        - 低 30 bit：作为页内偏移，这 30 位再细分为 9 + 9 + 12 位，
          每个根页表的条目可以表示 1GB 的内存区域（2^30 字节）。
    3. 为了完成内核等值映射，将 Page Table Entry（PTE）的权限位
       设置为 V | R | W | X，表示条目有效，且支持读、写、执行权限。
    */

    // 将虚拟地址 0x80000000 等值映射到物理地址 0x80000000
    // virtual address = 0x80000000 => VPN[2] = 2
    // 即虚拟地址 0x80000000 对应 early_pgtbl 的第 2 个条目
    // 这里的 (0 | 0x20000000U | 15U) 设置了 PTE 的值
    early_pgtbl[2] = (uint64)(0 | 0x20000000U | 15U);

    /*
    PTE 解释：
    - 0x20000000U：表示实际物理地址 0x80000000 >> 2，用于指向目标物理页。
    - 15U：设置 PTE 权限位 V | R | W | X，其中：
      - V：有效位，表示此页表条目有效
      - R：可读位，允许读取
      - W：可写位，允许写入
      - X：可执行位，允许执行
    */

    // 将虚拟地址 0xffffffe000000000 等值映射到物理地址 0x80000000
    // virtual address = 0xffffffe000000000 => VPN[2] = 384
    // 即虚拟地址 0xffffffe000000000 对应 early_pgtbl 的第 384 个条目
    // PTE 值 (0 | 0x20000000U | 15U) 保持不变
    early_pgtbl[384] = (uint64)(0 | 0x20000000U | 15U);

    /*
    此条目使得虚拟地址 0xffffffe000000000 等值映射到物理地址 0x80000000。
    这是一个备用的高位内核地址，用于某些 RISC-V 系统的高地址访问。
    */

    // 输出调试信息，表示 setup_vm 函数执行完成
    printk("...setup_vm done!\n");
}

```

### 补充
在 `setup_vm` 函数中，仅仅设置了页表的第 2 项和第 384 项，而中间的项并没有进行映射。这个选择与 RISC-V 的虚拟内存管理策略和内核启动需求密切相关，原因主要包括以下几点：

#### 内核等值映射仅需两个关键地址段

在内核启动时，两个关键的虚拟地址区域需要等值映射到物理地址：

- **低地址段 (`0x80000000`)**：这是内核的实际加载地址，映射到物理内存的起始位置 `0x80000000`，用于内核代码和数据的访问。
- **高地址段 (`0xffffffe000000000`)**：这是内核的高位虚拟地址，常用于在不同段内进行虚拟地址访问，并避免与用户空间地址产生冲突。

通过这两个地址的映射，内核能够既在低地址段（实际物理地址位置）访问自己，又可以通过高地址段来操作虚拟地址。其他中间地址在这个启动阶段是多余的，因此没有映射需求。

##### 中间区域未使用，节省内存资源

在 RISC-V 的虚拟地址空间中，虚拟地址并不总是要连续映射到物理内存。将中间未使用的虚拟地址区域留空，即不映射页表条目，有助于：

- **节省页表内存**：不为未用区域创建页表项减少了内存消耗。
- **增强安全性**：未映射的地址会触发页故障，防止非法访问。
- **减少管理复杂性**：只需要关注当前内核实际需要的映射段，而不是在页表中配置整个虚拟地址空间。

##### 分段映射的需求

RISC-V 的内核虚拟地址布局往往按需分段映射，如此内核在 `setup_vm` 初始化时只需在页表中设置必要的虚拟地址段。在内核启动之后，通过其他页表管理函数可以根据需要进一步管理和分配地址空间。因此，其他中间区域不需要提前映射。

##### 内核启动流程中的最小映射

在最小映射方案下，只有与内核加载和启动相关的内存区域需要映射，因此没有必要设置整个地址空间。后续的内存区域（如堆栈、堆等）会由内核进一步初始化和映射，这样确保内核在内存管理上更灵活高效。

##### 结论

仅映射第 2 项和第 384 项是出于启动需求的考虑，为了提供最小的启动映射支持内核加载和运行。中间的页表项在启动时没有被使用，因此没有必要去映射。

是的，在 `setup_vm` 设置页表之前，内核的指令地址是物理地址。在 `setup_vm` 设置页表并切换到虚拟内存模式之后，指令地址才会转换为虚拟地址。

#### 内核启动流程中的地址模式切换

1. **启动初期（物理地址模式）**：
   - 在内核启动的最初阶段，CPU 还没有开启虚拟内存支持，指令地址使用的是物理地址模式。在 RISC-V 架构中，这意味着 PC 指针指向的是实际的物理地址。例如，内核一般被加载在物理地址 `0x80000000` 处，所以在启动时，CPU 会从 `0x80000000` 物理地址开始执行。
   - 这段时间，内核可以直接通过物理地址访问自身的数据和代码。

2. **设置页表并切换到虚拟地址模式**：
   - 当 `setup_vm` 函数设置好页表（如`early_pgtbl`）后，CPU 切换到虚拟内存模式。此时，通过设置 `satp` 寄存器并启用分页，CPU 开始使用页表进行地址转换。
   - 切换后，内核使用虚拟地址访问自身，这样的设计便于内核和用户进程各自拥有独立的虚拟地址空间，并简化了权限管理。

3. **虚拟地址映射**：
   - `setup_vm` 中配置的页表建立了特定的虚拟地址到物理地址的映射（如 `0x80000000` 虚拟地址映射到 `0x80000000` 物理地址，`0xffffffe000000000` 虚拟地址也映射到 `0x80000000` 物理地址）。
   - 切换到虚拟地址模式后，内核可以使用这两个虚拟地址来访问物理地址 `0x80000000` 处的代码和数据。

##### 总结

在 `setup_vm` 设置并启用页表之前，内核运行在物理地址模式，直接使用物理地址访问内存。`setup_vm` 设置页表并切换到虚拟地址模式后，内核指令地址转换为虚拟地址模式，以便后续访问虚拟地址空间。