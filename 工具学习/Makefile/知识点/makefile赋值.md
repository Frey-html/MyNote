[使用变量 — 跟我一起写Makefile 1.0 文档 (seisman.github.io)](https://seisman.github.io/how-to-write-makefile/variables.html)

在 `Makefile` 中，有几种不同的赋值方式，每种方式在处理变量时的行为有所不同。下面是 `Makefile` 中常用的赋值方式的详细介绍：

### 1. 简单赋值（=）

- **语法**：
  ```makefile
  VARIABLE = value
  ```

- **特性**：
  - 立即计算并赋值。
  - 在赋值时，右侧的内容会被完全计算一次。
  - 后续对 `VARIABLE` 的引用会使用计算后的值。

- **示例**：
  ```makefile
  VAR = hello
  all:
      @echo $(VAR)   # 输出: hello
  ```

### 2. 懒惰赋值（`:=`）

- **语法**：
  ```makefile
  VARIABLE := value
  ```

- **特性**：
  - 立即计算并赋值。
  - 与简单赋值相似，但右侧的内容在赋值时被求值，而不是在引用时求值。
  - 对于动态计算内容时，特别有用。

- **示例**：
  ```makefile
  VAR := $(shell date)
  all:
      @echo $(VAR)   # 输出当前日期
  ```

### 3. 条件赋值（`?=`）

- **语法**：
  ```makefile
  VARIABLE ?= value
  ```

- **特性**：
  - 仅在 `VARIABLE` 尚未定义时赋值。
  - 如果 `VARIABLE` 已经被定义，这个赋值将被忽略。
  - 适用于提供默认值的情况。

- **示例**：
  ```makefile
  VAR ?= default_value
  all:
      @echo $(VAR)   # 如果 VAR 未定义，输出 default_value
  ```

### 4. 追加赋值（`+=`）

- **语法**：
  ```makefile
  VARIABLE += value
  ```

- **特性**：
  - 将 `value` 追加到已有的 `VARIABLE` 中。
  - 适用于构建文件列表、编译选项等场景。

- **示例**：
  ```makefile
  VAR = item1
  VAR += item2
  all:
      @echo $(VAR)   # 输出: item1 item2
  ```

### 5. 递归赋值（= 与 `:=` 的混用）

- 在使用 `=` 赋值的情况下，可以在值中包含 `$(VARIABLE)`，这会导致对 `VARIABLE` 的引用被延迟到实际使用时。这种方式称为递归赋值。

- **示例**：
  ```makefile
  VAR = $(shell echo Hello)
  all:
      @echo $(VAR)   # 输出: 运行时确定的值
  ```

- 如果使用 `:=`，则在赋值时就会计算并保存值。
  
  ```makefile
  VAR := $(shell echo Hello)
  all:
      @echo $(VAR)   # 输出: Hello
  ```

### 6. 变量的特定扩展

- `Makefile` 还允许某些特定的扩展和引用方式，例如，使用 `$(VARIABLE)` 引用变量，或 `$(VARIABLE:pattern=replacement)` 来替换变量中的某些部分。

### 7. 总结

| **赋值方式** | **语法**        | **行为**                                        | **应用场景**                                       |
|--------------|-----------------|-------------------------------------------------|----------------------------------------------------|
| 简单赋值     | `VAR = value`   | 立即计算并赋值，右侧值在引用时被求值。        | 定义常量或简单的变量。                              |
| 懒惰赋值     | `VAR := value`  | 立即计算并赋值，右侧值在赋值时被求值。        | 动态计算值，需立即评估的变量。                      |
| 条件赋值     | `VAR ?= value`  | 仅在未定义时赋值。                             | 提供默认值。                                       |
| 追加赋值     | `VAR += value`  | 追加内容到已有变量中。                         | 构建列表或累积编译选项。                            |
| 递归赋值     | `VAR = $(shell...)` | 根据上下文动态确定值。                        | 需要延迟计算的动态内容。                            |

每种赋值方式在不同的场景下具有不同的应用效果，理解它们的差异可以帮助你更有效地编写和维护 `Makefile`。