[书写规则 — 跟我一起写Makefile 1.0 文档 (seisman.github.io)](https://seisman.github.io/how-to-write-makefile/rules.html#id7)
[[makefile规则模式]]
### 静态模式规则简介

`Makefile` 中的**静态模式规则**（Static Pattern Rules）是一种简化编写重复规则的机制，用于指定多个目标的构建方式。它的语法类似于普通的模式规则（Pattern Rules），但允许你显式列出目标文件，并为这些目标文件定义通用的构建规则。

你的示例代码中展示了静态模式规则的使用：

```makefile
objects = foo.o bar.o 

all: $(objects)

$(objects): %.o: %.c
    $(CC) -c $(CFLAGS) $< -o $@
```

### 结构解释

1. **`objects = foo.o bar.o`**
   - 这定义了一个变量 `objects`，它包含目标文件 `foo.o` 和 `bar.o`。
   - `objects` 变量的值可以在 Makefile 其他部分通过 `$(objects)` 来引用。

2. **`all: $(objects)`**
   - 这是一个**伪目标**（phony target）。目标 `all` 依赖于 `$(objects)`，即 `foo.o` 和 `bar.o`。
   - 当你运行 `make` 或 `make all` 时，`make` 将尝试生成 `foo.o` 和 `bar.o`。

3. **静态模式规则 `$(objects): %.o: %.c`**
   - **静态模式规则**的语法是：
     ```makefile
     TARGETS: TARGET_PATTERN: DEPENDENCY_PATTERN
     ```
   - 这里的 `TARGETS` 是 `$(objects)`，即 `foo.o` 和 `bar.o`，表示这些目标文件需要通过静态模式规则来生成。
   - `TARGET_PATTERN` 是 `%.o`，表示匹配 `.o` 文件（目标）。
   - `DEPENDENCY_PATTERN` 是 `%.c`，表示这些 `.o` 文件依赖于同名的 `.c` 文件（如 `foo.c` 和 `bar.c`）。
   
   **说明**：
   - `%.o` 和 `%.c` 中的 `%` 是通配符，它会匹配文件名的任意部分。在这个例子中，`foo.o` 对应 `foo.c`，`bar.o` 对应 `bar.c`。
   - 这个静态模式规则告诉 `make`，生成 `foo.o` 需要 `foo.c`，生成 `bar.o` 需要 `bar.c`，并且生成方式是通过编译这些 `.c` 文件。

4. **`$(CC) -c $(CFLAGS) $< -o $@`**
   - 这是生成目标文件的命令，它解释了如何将 `.c` 文件编译为 `.o` 文件。

   **变量解释**：
   - **`$(CC)`**：编译器，通常是 `gcc` 或其他指定的 C 编译器。
   - **`$(CFLAGS)`**：编译选项，如 `-O2`、`-Wall` 等。可以在 Makefile 其他部分定义。
   - **`$<`**：依赖文件的第一个文件（即这里的 `.c` 文件）。例如，当编译 `foo.o` 时，`$<` 会是 `foo.c`。
   - **`$@`**：当前目标文件（即这里的 `.o` 文件）。例如，当目标是 `foo.o` 时，`$@` 会是 `foo.o`。

   **实际效果**：
   ```bash
   gcc -c $(CFLAGS) foo.c -o foo.o
   gcc -c $(CFLAGS) bar.c -o bar.o
   ```

### 静态模式规则 vs 普通模式规则

- **普通模式规则**（Pattern Rules）：
  ```makefile
  %.o: %.c
      $(CC) -c $(CFLAGS) $< -o $@
  ```
  这种规则适用于所有符合模式的文件，`%.o` 和 `%.c` 的作用范围是全局的，意味着只要有 `.o` 文件，它都会自动寻找同名 `.c` 文件。

- **静态模式规则**：
  ```makefile
  $(objects): %.o: %.c
      $(CC) -c $(CFLAGS) $< -o $@
  ```
  这种规则只适用于你明确列出的目标（即 `$(objects)` 中列出的文件），而不会影响其他 `.o` 文件的生成。这种方式更加精确，适合控制特定文件的编译方式。

### `$` 符号及其结合含义

在 Makefile 中，`$` 符号用于表示**变量引用**和**自动化变量**，如下是常用的几种 `$` 符号及其结合的含义：

#### 1. **变量引用**：
   - **`$(VAR)`**：用于引用变量 `VAR` 的值。变量可以是自定义的，也可以是 Makefile 中的内置变量。
     ```makefile
     objects = foo.o bar.o
     all: $(objects)
     ```
     这里 `$(objects)` 会被替换为 `foo.o bar.o`。

#### 2. **自动化变量**：
   自动化变量在命令中使用，表示当前规则的目标或依赖文件。

   - **`$@`**：当前目标文件。例如，在编译 `foo.o` 时，`$@` 就是 `foo.o`。
     ```makefile
     %.o: %.c
         $(CC) -c $(CFLAGS) $< -o $@
     ```
     `$@` 在这里被替换为 `.o` 文件。

   - **`$<`**：依赖文件中的第一个文件。通常用于获取规则中的源文件。例如，在编译 `foo.o` 时，`$<` 是 `foo.c`。
     ```makefile
     %.o: %.c
         $(CC) -c $(CFLAGS) $< -o $@
     ```
     `$<` 在这里被替换为 `.c` 文件。

   - **`$^`**：所有依赖文件，列表中的所有依赖项都会包含在内，用空格分隔。如果有多个依赖项，它们都会被包括。
     ```makefile
     foo: foo.o lib.o
         $(CC) -o $@ $^
     ```
     `$^` 被替换为 `foo.o lib.o`。

   - **`$?`**：比目标文件更新的依赖文件。适用于增量编译，只编译那些比目标更新的依赖文件。
     ```makefile
     foo: foo.o lib.o
         $(CC) -o $@ $?
     ```
     如果 `foo.o` 和 `lib.o` 有一个更新，那么 `$?` 就会是那个更新的文件。

#### 3. **其他 `$` 相关语法**：
   - **`$$`**：如果你需要在 Makefile 的命令中引用一个 Shell 中的 `$` 符号（如在命令行中的环境变量），需要用双 `$`。
     ```makefile
     echo:
         @echo $$PATH
     ```
     这个命令会打印 Shell 的 `PATH` 环境变量值。

### 例子解释

在你的例子中：

```makefile
objects = foo.o bar.o 

all: $(objects)

$(objects): %.o: %.c
    $(CC) -c $(CFLAGS) $< -o $@
```

- `objects` 变量定义了两个 `.o` 文件（`foo.o` 和 `bar.o`）。
- 目标 `all` 依赖于这些 `.o` 文件，因此 `make all` 会触发对 `foo.o` 和 `bar.o` 的构建。
- 静态模式规则 `$(objects): %.o: %.c` 定义了如何从 `.c` 文件生成对应的 `.o` 文件。`foo.o` 从 `foo.c` 编译，`bar.o` 从 `bar.c` 编译。
- 编译命令中的 `$<` 引用了 `.c` 文件，`$@` 引用了生成的 `.o` 文件。