在Obsidian中，数学公式的语法主要基于LaTeX。因此，许多数学符号、函数、集合运算等都可以通过LaTeX语法编写。在这里，我总结了在Obsidian中使用的主要数学符号和语法，包括常见的数学运算、集合操作、逻辑符号等。

### 1. 基本数学符号

| 符号              | LaTeX代码         | 显示             |
| ----------------- | ----------------- | ---------------- |
| 加                | `+`               | $+$              |
| 减                | `-`               | $-$              |
| 乘                | `\times`          | $\times$         |
| 除                | `\div`            | $\div$           |
| 等于              | `=`               | $=$              |
| 不等于            | `\neq`            | $\neq$           |
| 大于              | `>`               | $>$              |
| 小于              | `<`               | $<$              |
| 大于等于          | `\geq`            | $\geq$           |
| 小于等于          | `\leq`            | $\leq$           |
| 绝对值            | `\lvert x \rvert` | $\lvert x \rvert$|
| 上标              | `x^2`             | $x^2$            |
| 下标              | `x_1`             | $x_1$            |

### 2. 希腊字母

| 字母      | LaTeX代码        | 显示         |
| --------- | ---------------- | ------------ |
| α (alpha) | `\alpha`         | $\alpha$     |
| β (beta)  | `\beta`          | $\beta$      |
| γ (gamma) | `\gamma`         | $\gamma$     |
| Δ (Delta) | `\Delta`         | $\Delta$     |
| π (pi)    | `\pi`            | $\pi$        |

### 3. 分数和根号

| 运算     | LaTeX代码              | 显示             |
| -------- | -----------------------| ---------------- |
| 分数     | `\frac{a}{b}`          | $\frac{a}{b}$    |
| 平方根   | `\sqrt{a}`             | $\sqrt{a}$       |
| n次方根  | `\sqrt[n]{a}`          | $\sqrt[n]{a}$    |

### 4. 集合运算

| 符号            | LaTeX代码               | 显示                |
| --------------- | ----------------------- | ------------------- |
| 空集            | `\emptyset`             | $\emptyset$         |
| 属于            | `\in`                   | $\in$               |
| 不属于          | `\notin`                | $\notin$            |
| 子集            | `\subset`               | $\subset$           |
| 真子集          | `\subsetneq`            | $\subsetneq$        |
| 包含            | `\supset`               | $\supset$           |
| 真包含          | `\supsetneq`            | $\supsetneq$        |
| 子集或等于      | `\subseteq`             | $\subseteq$         |
| 包含或等于      | `\supseteq`             | $\supseteq$         |
| 交集            | `\cap`                  | $\cap$              |
| 并集            | `\cup`                  | $\cup$              |
| 差集            | `\setminus`             | $\setminus$         |
| 笛卡尔积        | `\times`                | $\times$            |
| 幂集            | `\mathcal{P}(A)`        | $\mathcal{P}(A)$    |
| 常见集合        | `\mathbb{N}, \mathbb{Z}, \mathbb{Q}, \mathbb{R}, \mathbb{C}` | $\mathbb{N}, \mathbb{Z}, \mathbb{Q}, \mathbb{R}, \mathbb{C}$ |

### 5. 逻辑运算符

| 符号          | LaTeX代码          | 显示               |
| ------------- | ------------------ | ------------------ |
| 与            | `\land`            | $\land$           |
| 或            | `\lor`             | $\lor$            |
| 非            | `\neg`             | $\neg$            |
| 蕴含          | `\Rightarrow`      | $\Rightarrow$     |
| 等价          | `\Leftrightarrow`  | $\Leftrightarrow$ |
| 任意          | `\forall`          | $\forall$         |
| 存在          | `\exists`          | $\exists$         |

### 6. 求和与积分

| 符号         | LaTeX代码                   | 显示                   |
| ------------ | ---------------------------- | ---------------------- |
| 求和         | `\sum_{i=1}^n`               | $\sum_{i=1}^n$        |
| 求积         | `\prod_{i=1}^n`              | $\prod_{i=1}^n$       |
| 积分         | `\int_{a}^{b}`               | $\int_{a}^{b}$        |
| 连续积分     | `\iint`、`\iiint`            | $\iint$、$\iiint$     |

### 7. 方程组与矩阵

- **方程组**：
  ```latex
  \begin{cases}
    x + y = 1 \\
    x - y = 0
  \end{cases}
  ```
  显示为：
  \[
  \begin{cases}
    x + y = 1 \\
    x - y = 0
  \end{cases}
  \]

- **矩阵**：
  ```latex
  \begin{matrix} 
    a & b \\ 
    c & d 
  \end{matrix}
  ```
  显示为：
  \[
  \begin{matrix} 
    a & b \\ 
    c & d 
  \end{matrix}
  \]

### 8. 其他常用符号

| 符号          | LaTeX代码               | 显示              |
| ------------- | ----------------------- | ----------------- |
| 无限          | `\infty`                | $\infty$          |
| 部分导数      | `\partial`              | $\partial$        |
| 向量          | `\vec{v}`               | $\vec{v}$         |
| 点乘          | `\cdot`                 | $\cdot$           |
| 叉乘          | `\times`                | $\times$          |
| 模            | `\|x\|`                 | $\|x\|$           |

### 9. 使用示例

在Obsidian中，书写数学公式时，建议遵循以下格式：

- **内联公式**：`$公式内容$`，例如 `$a^2 + b^2 = c^2$`。
- **块级公式**：`$$公式内容$$`，例如：
  ```latex
  $$
  \sum_{i=1}^n i = \frac{n(n+1)}{2}
  $$
  ```

在块级公式中，公式会居中显示，更适合书写较复杂的数学表达式。