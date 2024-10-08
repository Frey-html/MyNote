[使用变量 — 跟我一起写Makefile 1.0 文档 (seisman.github.io)](https://seisman.github.io/how-to-write-makefile/variables.html#id3)
在 `Makefile` 中，查看包含追加参数的变量、以及调用这些变量的方式，主要依赖于两部分：**变量的定义**和**变量的调用**。下面详细解释如何查看追加参数的变量、如何调用，以及如何验证变量中的追加参数。

### 1. 查看变量的追加参数

要查看变量的追加参数，首先我们需要理解 `Makefile` 中如何定义和追加变量。

#### 追加参数的定义
```makefile
CFLAGS = -Wall              # 初始定义
CFLAGS += -O2               # 追加参数 -O2
CFLAGS += -g                # 追加参数 -g
```

在上面的例子中，`CFLAGS` 变量最初被定义为 `-Wall`，之后通过 `+=` 操作符，分别追加了 `-O2` 和 `-g`。这意味着 `CFLAGS` 的最终值将是 `-Wall -O2 -g`。

### 2. 调用含有追加参数的变量

在 `Makefile` 中，变量通常通过 `$()` 或 `${}` 来调用。

#### 调用例子：
```makefile
all:
    @echo $(CFLAGS)
```

- **`$(CFLAGS)`** 是对 `CFLAGS` 变量的调用。
- 在执行 `make` 时，这行代码将打印出 `CFLAGS` 变量的值，包括所有的追加参数。

#### 输出结果：
```bash
-Wall -O2 -g
```

### 3. 打印变量值以验证追加参数

有时，你可能需要查看变量的内容，尤其是在复杂的 `Makefile` 中，变量经过多次追加后，最终值可能不直观。你可以通过 `echo` 或 `$(info)` 来输出变量的值。

#### 使用 `echo` 打印变量：
```makefile
all:
    @echo CFLAGS is: $(CFLAGS)
```

- `@` 符号是为了不让 `make` 输出命令本身，只输出结果。
- 这会在执行 `make all` 时，打印 `CFLAGS` 的值，包括所有追加的参数。

#### 使用 `info` 打印变量：
```makefile
all:
    $(info CFLAGS is: $(CFLAGS))
```

- **`$(info ...)`** 是 `Makefile` 的内置函数，用于在构建过程中输出信息。
- 它会在 `make` 运行时打印出 `CFLAGS` 的值，显示所有追加的参数。

### 4. 含有追加参数的变量的实际调用

在实际使用中，带有追加参数的变量通常用于传递给编译器或链接器。例如：

```makefile
CFLAGS = -Wall
CFLAGS += -O2
CFLAGS += -g

CC = gcc

all: program

program: main.o util.o
    $(CC) $(CFLAGS) main.o util.o -o program
```

- **`$(CFLAGS)`** 在这个规则中被传递给 `gcc`，用作编译选项。
- 当执行 `make program` 时，编译器 `gcc` 会使用 `CFLAGS` 中包含的所有选项（`-Wall -O2 -g`）来编译程序。

### 5. 验证追加参数的存在

你可以通过手动检查变量的内容来确认追加参数是否已经生效。例如：

```makefile
CFLAGS = -Wall
CFLAGS += -O2

all:
    @echo Initial CFLAGS: $(CFLAGS)
    @$(eval CFLAGS += -g)   # 动态追加
    @echo Final CFLAGS: $(CFLAGS)
```

在这个例子中，`$(eval)` 允许我们在运行时追加参数。通过 `echo` 输出，我们可以查看追加前后的变量值：

#### 输出结果：
```bash
Initial CFLAGS: -Wall -O2
Final CFLAGS: -Wall -O2 -g
```

### 6. 多个变量的追加使用

有时我们会从多个变量中追加内容，如下：

```makefile
CFLAGS = -Wall
EXTRA_FLAGS = -O2 -g
CFLAGS += $(EXTRA_FLAGS)

all:
    @echo CFLAGS is: $(CFLAGS)
```

- **`$(EXTRA_FLAGS)`** 的值会被追加到 `CFLAGS` 中。
- 在执行 `make all` 时，`CFLAGS` 的最终值是 `-Wall -O2 -g`。

#### 输出结果：
```bash
CFLAGS is: -Wall -O2 -g
```

### 7. 总结

- **定义变量并追加参数**：使用 `+=` 可以逐步将新值追加到现有变量中。
- **调用含有追加参数的变量**：通过 `$(VARIABLE)` 或 `${VARIABLE}` 调用，确保最终值包含所有追加的内容。
- **验证变量的最终值**：可以通过 `@echo` 或 `$(info)` 输出变量的内容，检查追加参数是否生效。
- **实际使用**：在 `Makefile` 中，变量（如 `CFLAGS`）通常用于传递编译器或链接器的选项，追加参数可以灵活地调整这些选项。