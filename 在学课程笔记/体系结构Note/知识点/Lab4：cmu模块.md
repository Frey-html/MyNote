### **补全并注释后的代码：**

```verilog
module cmu (
        // CPU side
		input clk,
		input rst,
		input [31:0] addr_rw,          // CPU的读写地址
		input en_r,                    // 读使能
		input en_w,                    // 写使能
        input [2:0] u_b_h_w,           // 表示访问的字节宽度（字节、半字或字）
		input [31:0] data_w,           // 写入的数据
		output [31:0] data_r,          // 读取的数据
		output stall,                  // 是否需要暂停

        // mem side
		output reg mem_cs_o = 0,       // 与外部存储器的片选信号
		output reg mem_we_o = 0,       // 与外部存储器的写使能信号
		output reg [31:0] mem_addr_o = 0,  // 发送给外部存储器的地址
		input [31:0] mem_data_i,       // 从外部存储器读取的数据
		output [31:0] mem_data_o,      // 发送给外部存储器的数据
		input mem_ack_i,               // 存储器操作的确认信号

        // debug info
        output [2:0] cmu_state         // 当前状态
	);

    `include "addr_define.vh"           // 引入地址相关的常量定义
    reg TO_BE_FILLED = 0;               // 占位变量
    reg [ADDR_BITS-1:0] cache_addr = 0; // 缓存操作的地址
    reg cache_load = 0;                 // 缓存的加载操作信号
    reg cache_replace = 0;              // 缓存的替换操作信号
    reg cache_store = 0;                // 缓存的写入操作信号
    reg [2:0] cache_u_b_h_w = 0;        // 传递给缓存的字节宽度选择
    reg [WORD_BITS-1:0] cache_din = 0;  // 写入缓存的数据
    wire cache_hit;                     // 缓存命中信号
    wire [WORD_BITS-1:0] cache_dout;    // 从缓存读取的数据
    wire cache_valid;                   // 缓存有效标志位
    wire cache_dirty;                   // 缓存脏位标志位
    wire [TAG_BITS-1:0] cache_tag;      // 缓存的标签位

    // 缓存模块实例化
    cache CACHE (
        .clk(clk),
        .rst(rst),
        .addr(cache_addr),
        .load(cache_load),
        .replace(cache_replace),
        .store(cache_store),
        .invalid(1'b0),
        .u_b_h_w(cache_u_b_h_w),
        .din(cache_din),
        .hit(cache_hit),
        .dout(cache_dout),
        .valid(cache_valid),
        .dirty(cache_dirty),
        .tag(cache_tag)
    );

    // 状态定义
    localparam
        S_IDLE = 0,    // 空闲状态
        S_BACK = 2,    // 写回内存状态
        S_FILL = 3,    // 从内存填充缓存状态
        S_WAIT = 4;    // 等待状态

    reg [2:0] state = 0;                // 当前状态
    reg [2:0] next_state = 0;           // 下一状态
    reg [ELEMENT_WORDS_WIDTH-1:0] word_count = 0;      // 当前处理的字数计数
    reg [ELEMENT_WORDS_WIDTH-1:0] next_word_count = 0; // 下一状态的字数计数
    assign cmu_state = state;            // 将当前状态输出到调试信号
    reg sstall;                          // 暂停信号
    
    // 状态机的时序逻辑
    always @ (posedge clk) begin
        if (rst) begin
            state <= S_IDLE;             // 复位时进入空闲状态
            word_count <= 2'b00;         // 字数计数器清零
        end
        else begin
            state <= next_state;         // 更新当前状态
            word_count <= next_word_count; // 更新字数计数器
        end
    end

    // 状态转移逻辑
    always @ (*) begin
        case (state)
            S_IDLE: begin
                if (en_r || en_w) begin  // 如果CPU请求读取或写入
                    if (cache_hit)       // 缓存命中
                        next_state = S_WAIT;
                    else if (cache_valid && cache_dirty) // 未命中且缓存块为脏
                        next_state = S_BACK;   // 进入写回状态
                    else
                        next_state = S_FILL;   // 进入填充缓存状态
                end
                else begin
                    next_state = S_IDLE;       // 没有读写请求，保持空闲状态
                end
                next_word_count = 2'b00;       // 字数计数器清零
            end

            S_BACK: begin                      // 写回状态
                if (mem_ack_i && word_count == {ELEMENT_WORDS_WIDTH{1'b1}}) // 确认写回完成
                    next_state = S_FILL;       // 写回完成后，进入填充状态
                else
                    next_state = S_BACK;       // 否则继续写回

                if (mem_ack_i)
                    next_word_count = word_count+1; // 更新字数计数器
                else
                    next_word_count = word_count;
            end

            S_FILL: begin                      // 填充状态
                if (mem_ack_i && word_count == {ELEMENT_WORDS_WIDTH{1'b1}}) // 确认填充完成
                    next_state = S_WAIT;       // 填充完成后，进入等待状态
                else
                    next_state = S_FILL;       // 否则继续填充

                if (mem_ack_i)
                    next_word_count = word_count+1; // 更新字数计数器
                else
                    next_word_count = word_count;
            end

            S_WAIT: begin                      // 等待状态
                next_state = S_IDLE;           // 返回空闲状态
                next_word_count = 2'b00;       // 字数计数器清零
            end

            default: begin                     // 默认情况，保持不确定状态
                next_state = 3'hx;
                next_word_count = 2'bxx;
            end
        endcase
    end

    // 缓存控制逻辑
    always @ (*) begin
        case(state)
            S_IDLE, S_WAIT: begin              // 空闲或等待状态
                cache_addr = addr_rw;          // 将CPU的地址传递给缓存
                cache_load = en_r;             // 读操作信号传递给缓存
                cache_store = en_w;            // 写操作信号传递给缓存
                cache_replace = 1'b0;          // 替换信号关闭
                cache_u_b_h_w = u_b_h_w;       // 字节宽度信息传递给缓存
                cache_din = data_w;            // 数据写入缓存
            end
            S_BACK: begin                      // 写回状态
                cache_addr = addr_rw;          // 写回时保持当前地址
                cache_load = 1'b0;             // 停止读取
                cache_store = 1'b0;            // 停止写入
                cache_replace = 1'b0;          // 替换信号关闭
                cache_u_b_h_w = 3'b010;        // 默认字宽设置
                cache_din = 32'b0;             // 数据清零
            end
            S_FILL: begin                      // 填充状态
                cache_addr = addr_rw;          // 从内存填充时的地址
                cache_load = 1'b0;             // 停止读取
                cache_store = 1'b0;            // 停止写入
                cache_replace = mem_ack_i;     // 如果收到确认信号，执行替换
                cache_u_b_h_w = 3'b010;        // 默认字宽设置
                cache_din = mem_data_i;        // 将从内存读出的数据填充到缓存
            end

            default: begin
                cache_addr = 32'hxxxxxxxx;     // 默认不确定的地址
                cache_load = 1'hx;
                cache_store = 1'hx;
                cache_replace = 1'bx;
                cache_u_b_h_w = 3'hx;
                cache_din = 32'hxxxxxxxx;
            end
        endcase
    end
    assign data_r = cache_dout;                // 输出缓存读出的数据

    // 内存控制逻辑
    always @ (*) begin
        case (state)
            S_IDLE, S_WAIT: begin              // 空闲或等待状态
                mem_cs_o = 1'b0;               // 关闭片选信号
                mem_we_o = 1'b0;               // 停止

写入
                mem_addr_o = 32'b0;            // 地址清零
            end

            S_BACK: begin                      // 写回状态
                mem_addr_o = {cache_tag, addr_rw[ADDR_BITS-TAG_BITS-1:BLOCK_WIDTH], word_count, {ELEMENT_WORDS_WIDTH{1'b0}}}; // 写回地址
                mem_cs_o = 1'b1;               // 使能片选信号
                mem_we_o = 1'b1;               // 写回数据
            end

            S_FILL: begin                      // 填充状态
                mem_addr_o = {addr_rw[ADDR_BITS-1:BLOCK_WIDTH], word_count, {ELEMENT_WORDS_WIDTH{1'b0}}}; // 填充地址
                mem_cs_o = 1'b1;               // 使能片选信号
                mem_we_o = 1'b0;               // 读取数据
            end

            default: begin
                mem_addr_o = 32'hxxxxxxxx;     // 不确定的默认值
                mem_cs_o = 1'bx;
                mem_we_o = 1'bx;
            end
        endcase
    end

    // 将缓存的数据输出到内存侧
    assign mem_data_o = cache_dout;
    // 暂停信号输出（根据需要补充）
    assign stall = (state != S_IDLE);

endmodule
```

### **补全部分的解释：**

1. **状态机逻辑：**
    - 状态机根据当前状态和输入信号决定下一步动作。主要的状态有：
        - `S_IDLE`: 处理空闲状态，检测是否有读写请求，并判断是否命中缓存。
        - `S_BACK`: 处理缓存未命中且脏位有效的情况，需要将数据写回到外部存储器。
        - `S_FILL`: 处理缓存未命中的情况，从外部存储器读取数据并填充缓存。
        - `S_WAIT`: 等待操作完成，返回到空闲状态。

2. **缓存控制逻辑：**
    - 缓存控制主要是根据当前状态决定是否加载、写入或替换缓存内容。关键部分：
        - 在 `S_IDLE` 或 `S_WAIT` 状态下，根据 CPU 请求决定是否执行读取或写入。
        - 在 `S_BACK` 和 `S_FILL` 状态下，处理缓存的写回或填充操作。

3. **内存控制逻辑：**
    - 根据状态决定对外部存储器的操作：
        - 在 `S_BACK` 状态下将脏数据写回到外部存储器。
        - 在 `S_FILL` 状态下从外部存储器读取数据，填充到缓存。

4. **补全的变量和逻辑：**
    - `mem_cs_o`, `mem_we_o` 等信号的控制在状态机中补全。
    - 增加了对 `stall` 信号的处理，在非空闲状态时挂起 CPU 的后续操作。