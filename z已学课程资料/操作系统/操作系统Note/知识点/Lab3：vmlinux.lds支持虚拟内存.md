在这个链接器脚本中，`ram` 和 `ramv` 是两个不同的内存区域，用于定义物理内存和虚拟内存的分配。
[RISC-V 页表相关 - 鹤翔万里的笔记本](https://note.tonycrane.cc/cs/pl/riscv/paging/)
[[Lab3 Report]]
### 1. `ram`:
- **定义**: 
  ```plaintext
  ram  (wxa!ri): ORIGIN = PHY_START + OPENSBI_SIZE, LENGTH = PHY_SIZE - OPENSBI_SIZE
  ```
- **用途**: 
  - `ram` 表示物理内存区域。
  - `ORIGIN` 指定了这个区域的起始地址，从物理内存的起始地址 `PHY_START` 加上 `OPENSBI_SIZE`（OpenSBI 的占用空间）。
  - `LENGTH` 表示这个区域的大小，等于物理内存总大小减去 OpenSBI 占用的大小。

### 2. `ramv`:
- **定义**:
  ```plaintext
  ramv (wxa!ri): ORIGIN = VM_START + OPENSBI_SIZE, LENGTH =  VM_SIZE - OPENSBI_SIZE
  ```
- **用途**:
  - `ramv` 表示虚拟内存区域。
  - `ORIGIN` 指定了虚拟内存的起始地址，从虚拟内存的起始地址 `VM_START` 加上 `OPENSBI_SIZE`。
  - `LENGTH` 是虚拟内存的大小，等于虚拟内存的总大小减去 OpenSBI 的大小。

### 为什么有两个区域？
- **物理内存 (`ram`)**: 用于实际的硬件内存，直接映射到设备的RAM中。
- **虚拟内存 (`ramv`)**: 用于操作系统的虚拟内存管理。程序在运行时使用虚拟地址，链接器在构建时需要将这些虚拟地址映射到实际的物理地址。

### `AT>` 指令的作用：
在每个段的定义中使用 `AT>ram` 和 `AT>ramv` 的指令表示：
- 该段的实际数据存储在物理内存 `ram` 中，但其虚拟地址对应于 `ramv` 区域。这种分离使得可以有效地实现虚拟内存的管理和映射。

在链接器脚本中，`(wxa!ri)` 和 `Memory { ramv: ram: }` 是特定的语法，用于定义内存区域及其属性。

### `(wxa!ri)` 的含义
- **`wxa`**: 表示该内存区域是可写（w）、可执行（x）和可读（a）的。这意味着在这个区域中的代码可以被执行，数据可以被读写。
- **`!ri`**: 表示禁止读取无效（invalid）和初始化（initial）数据。具体来说，它指的是在访问时要确保数据是有效的。

这种语法用于在定义内存区域时指定访问权限和特性，以确保正确的内存管理。

### `Memory { ramv: ram: }`
- **`Memory`**: 是一个保留字，用于定义内存区域。
- **`ramv:` 和 `ram:`**: 这是两个内存区域的名称，它们在之前的 `MEMORY` 语句中定义。用冒号将内存区域名与其属性区分开。

这种结构是 GNU 链接器（ld）脚本的一部分，用于为程序分配内存并指定这些内存区域的特性。通过这种方式，链接器能够生成合适的内存布局，确保程序在运行时能够正确访问所需的内存区域。

