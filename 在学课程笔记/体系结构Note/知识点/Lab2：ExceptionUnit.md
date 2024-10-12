Control unit 中的 "exception vector" 在 RISC-V 中用于处理不同类型的异常，例如非法指令、系统调用等。通常会根据指令执行时是否发生了异常（如非法指令、系统调用等）来产生中断或者进入异常处理流程。

对于这个二位的 `exp_vector`，可以这样定义：

1. **00**：表示无异常。
2. **01**：表示系统调用 (`ECALL`) 异常。
3. **10**：表示非法指令 (`illegal_inst`) 异常。
4. **11**：保留或扩展为其他异常类型。

在代码中，`exp_vector` 的值取决于是否检测到 `ECALL` 或 `illegal_inst` 异常。你可以根据这两类异常的检测结果设置 `exp_vector`。以下是补全后的代码：

```verilog
assign exp_vector = illegal_inst ? 2'b10 : 
                    ECALL       ? 2'b01 : 
                                  2'b00;  // 默认无异常
```


要补全和解释你的异常处理单元（`ExceptionUnit`）的功能，我们需要依赖于 RISC-V 架构中的异常和中断处理流程，尤其是与 CSR（控制和状态寄存器）相关的操作。具体来说，我们要处理的是：

1. **异常类型识别**（根据 `illegal_inst`、`l_access_fault`、`s_access_fault` 和 `ecall_m` 等信号）。
2. **异常向量计算**（从 `mcause` 寄存器获取异常原因）。
3. **中断和异常处理的交互**。
4. **PC 重定向**（异常处理结束后，需要重定向 PC 到异常处理地址）。

### 1. **异常编码 (`exception_index`)**
RISC-V 中 `mcause` 寄存器的编码用于表示不同的异常或中断原因。我们可以根据这些信号为 `mcause` 分配不同的值。参考 RISC-V 手册，常见的异常编码如下：

- **非法指令（illegal instruction）**: 2
- **存储读取访问错误（load access fault）**: 5
- **存储写入访问错误（store access fault）**: 7
- **机器模式调用（ecall_m）**: 11

你需要将这些值映射到 `exception_index`：

```verilog
assign exception_index = (illegal_inst)      ? 4'd2 :
                         (l_access_fault)    ? 4'd5 :
                         (s_access_fault)    ? 4'd7 :
                         (ecall_m)           ? 4'd11 :
                         4'd0;  // 默认值
```

### 2. **中断或异常标志 (`interrupt_or_exception`)**
在 RISC-V 中，中断和异常会触发不同的处理路径，但可以通过统一的信号来处理。我们需要判断是中断还是异常发生。如果发生中断或异常，控制逻辑需要进行 PC 的重定向。假设你有一个 `interrupt` 信号输入来标记中断，`interrupt_or_exception` 可以如下计算：

```verilog
assign interrupt_or_exception = interrupt | illegal_inst | l_access_fault | s_access_fault | ecall_m;
```

### 3. **重定向 PC (`PC_redirect`)**
异常和中断通常会导致 PC 重定向到一个特定的异常处理地址。在 RISC-V 中，这个地址存储在 `mtvec` 寄存器中。因此，当发生异常时，PC 需要被重定向到 `mtvec`：

```verilog
assign PC_redirect = (interrupt_or_exception) ? mtvec : epc_next;
```

- 如果发生异常或中断，PC 会重定向到 `mtvec`（异常处理地址）。
- 否则，PC 会继续到下一个 PC 地址 `epc_next`。

### 4. **寄存器刷新和取消写回 (`flush` 和 `RegWrite_cancel`)**
当发生异常时，流水线中的指令需要被清除，并停止任何潜在的写回操作。因此，`flush` 信号应该在发生中断或异常时置高，`RegWrite_cancel` 用于取消写回操作：

```verilog
assign reg_FD_flush = interrupt_or_exception | mret;
assign reg_DE_flush = interrupt_or_exception;
assign reg_EM_flush = interrupt_or_exception;
assign reg_MW_flush = interrupt_or_exception;
assign RegWrite_cancel = interrupt_or_exception;
```

### 5. **重定向选择 (`redirect_mux`)**
这个信号控制是否应该执行重定向，通常与中断或异常发生时相关。此时，流水线会跳转到异常处理程序：

```verilog
assign redirect_mux = interrupt_or_exception;
```

### 6. **`mepc_w` 和 `mcause_w` 寄存器写回**
当发生异常时，当前的 PC 值（`epc_cur`）应该写入 `mepc`，用于保存异常发生时的 PC，稍后可以通过 `mret` 返回。而 `mcause` 保存了异常的原因（即 `exception_index` 或者中断信息）。

```verilog
assign mepc_w = epc_cur;  // 保存当前PC
assign mcause_w = {interrupt, {27{1'b0}}, exception_index};  // mcause包含中断标志和异常原因
assign mtval_w = epc_cur;  // 一般来说，异常时会保存PC
```

### 补全后的代码：
```verilog
`timescale 1ns / 1ps

module ExceptionUnit(
    input clk, rst,
    input csr_rw_in,
    input[1:0] csr_wsc_mode_in,
    input csr_w_imm_mux,
    input[11:0] csr_rw_addr_in,
    input[31:0] csr_w_data_reg,
    input[4:0] csr_w_data_imm,
    output[31:0] csr_r_data_out,

    input interrupt,
    input illegal_inst,
    input l_access_fault,
    input s_access_fault,
    input ecall_m,

    input mret,

    input[31:0] epc_cur,
    input[31:0] epc_next,
    output[31:0] PC_redirect,
    output redirect_mux,

    output reg_FD_flush, reg_DE_flush, reg_EM_flush, reg_MW_flush, 
    output RegWrite_cancel
);
    reg TO_BE_FILLED = 0;

    reg[11:0] csr_waddr;
    reg[31:0] csr_wdata;
    reg csr_w;
    reg[1:0] csr_wsc;
    wire[11:0] csr_raddr;
    wire[31:0] csr_rdata;

    wire[31:0] mstatus;
    wire[31:0] mepc;
    wire[31:0] mtvec;
    wire[31:0] mie;

    wire interrupt_or_exception;
    wire[31:0] mepc_w;
    wire[31:0] mcause_w;
    wire[31:0] mtval_w;

    wire[3:0] waddr_map;
    wire enable_exception;

    wire [3:0] exception_index;

    // exception encoding
    assign exception_index = (illegal_inst)      ? 4'd2 :
                             (l_access_fault)    ? 4'd5 :
                             (s_access_fault)    ? 4'd7 :
                             (ecall_m)           ? 4'd11 :
                             4'd0;  // default value

    assign enable_exception = mstatus[3];

    // combine interrupt and exception signals
    assign interrupt_or_exception = interrupt | illegal_inst | l_access_fault | s_access_fault | ecall_m;
    
    // flush registers on interrupt or exception
    assign reg_FD_flush = interrupt_or_exception | mret;
    assign reg_DE_flush = interrupt_or_exception;
    assign reg_EM_flush = interrupt_or_exception;
    assign reg_MW_flush = interrupt_or_exception;
    assign RegWrite_cancel = interrupt_or_exception;

    // redirect PC based on exception or next PC
    assign PC_redirect = (interrupt_or_exception) ? mtvec : epc_next;
    assign redirect_mux = interrupt_or_exception;

    // write to mepc and mcause on exception
    assign mepc_w = epc_cur;
    assign mcause_w = {interrupt, {27{1'b0}}, exception_index};
    assign mtval_w = epc_cur;  // example, depending on the fault type
    assign csr_raddr = csr_rw_addr_in;
    assign csr_r_data_out = csr_rdata;

    CSRRegs csr(.clk(clk), .rst(rst), .csr_w(csr_w), .raddr(csr_raddr), .waddr(csr_waddr),
        .wdata(csr_wdata), .rdata(csr_rdata), .mstatus(mstatus), .csr_wsc_mode(csr_wsc),
        .mtvec(mtvec), .mepc(mepc), .interrupt(interrupt_or_exception), .mepc_w(mepc_w),
        .mcause_w(mcause_w), .mtval_w(mtval_w), .mret(mret), .waddr_map(waddr_map), .mie(mie));

    always @ (negedge clk or posedge rst) begin
        if(rst) begin
          csr_waddr<=0;
          csr_wdata<=0;
          csr_w<=0;
          csr_wsc<=0;
        end
        else begin
            if(csr_rw_in) begin
                csr_waddr <= csr_rw_addr_in;
                csr_wdata <= csr_w_imm_mux ? {{27{1'b0}}, csr_w_data_imm} : csr_w_data_reg;
                csr_w <= 1;
                csr_wsc <= csr_wsc_mode_in;
            end 
            else begin
                csr_waddr<=0;
                csr_wdata<=0;
                csr_w<=0;
                csr_wsc<=0;
            end            
        end
    end
endmodule
```

### 解释：
- `exception_index` 负责编码不同的异常类型（非法指令、访问故障、系统调用等），并将其写入 `mcause`。
- `interrupt_or_exception` 是中断和异常的组合信号，用于触发 PC 重定向、流水线刷新和写回取消等操作。
- `PC_redirect` 在发生异常

或中断时，将 PC 重定向到 `mtvec`，否则跳转到 `epc_next`。
- `reg_FD_flush` 等信号用于刷新流水线中的各个寄存器，以清除不合法指令或正在执行的指令。