`timescale 1ps/1ps

module HazardDetectionUnit(
    input clk,
    input Branch_ID, rs1use_ID, rs2use_ID,
    input[1:0] hazard_optype_ID,
    input[4:0] rd_EXE, rd_MEM, rs1_ID, rs2_ID, rs2_EXE,
    output PC_EN_IF, reg_FD_EN, reg_FD_stall, reg_FD_flush,
        reg_DE_EN, reg_DE_flush, reg_EM_EN, reg_EM_flush, reg_MW_EN,
    output forward_ctrl_ls,
    output[1:0] forward_ctrl_A, forward_ctrl_B
);
    reg TO_BE_FILLED = 0; //所有这个寄存器占的位置都需要填入正确的值
                          //可能是wire或reg的名称，可能是一个算式，也可能是某个常数

    parameter HAZARD_NO  = 2'b00 ;
    parameter HAZARD_EX  = 2'b01 ; //非S类型指令读、写寄存器引起hazard
    parameter HAZARD_MEM = 2'b10 ; //L类型指令读、写寄存器引起harzard
    parameter HAZARD_ST  = 2'b11 ; //S类型指令读寄存器引起harzard
    reg [1:0] hazard_optype_EX, hazard_optype_MEM;
    //modified
    always @(posedge clk) begin
        hazard_optype_MEM = hazard_optype_EX;
        if (reg_DE_flush) 
            hazard_optype_EX <= HAZARD_NO;  // Clear hazard on flush
        else 
            hazard_optype_EX <= hazard_optype_ID;//前有坑，所以接下来Flush很有用
    end

    wire A_EX_Forward,B_EX_Forward;
    wire A_MEM_Forward,B_MEM_Forward;
    assign A_EX_Forward = rs1use_ID && 
                        (rs1_ID == rd_EXE) && 
                        (rs1_ID != 0) &&
                        (hazard_optype_EX == HAZARD_EX);
    //modified
    assign B_EX_Forward = rs2use_ID && 
                        (rs2_ID == rd_EXE) && 
                        (rs2_ID != 0) &&
                        (hazard_optype_EX == HAZARD_EX);  

    assign A_MEM_Forward = rs1use_ID && 
                        (rs1_ID == rd_MEM) && 
                        (rs1_ID != 0) &&
                        (hazard_optype_MEM == HAZARD_EX || hazard_optype_MEM == HAZARD_MEM);

    assign B_MEM_Forward = rs2use_ID && 
                        (rs2_ID == rd_MEM) && 
                        (rs2_ID != 0) &&
                        (hazard_optype_MEM == HAZARD_EX || hazard_optype_MEM == HAZARD_MEM);   //这个空比较长，有五个判断条件同时成立时这个信号才会触发  

    assign forward_ctrl_A   = {2{A_EX_Forward}} & 2'b01|
                            {2{A_MEM_Forward&(hazard_optype_MEM==HAZARD_EX)}} & 2'b10 |
                            {2{A_MEM_Forward&(hazard_optype_MEM==HAZARD_MEM)}}& 2'b11 ;

    assign forward_ctrl_B   = {2{B_EX_Forward}} & 2'b01|
                            {2{B_MEM_Forward&(hazard_optype_MEM==HAZARD_EX)}} & 2'b10 |
                            {2{B_MEM_Forward&(hazard_optype_MEM==HAZARD_MEM)}}& 2'b11 ;

    wire ST_Forward;
    assign ST_Forward = (hazard_optype_EX==HAZARD_ST) & 
                        (hazard_optype_MEM==HAZARD_MEM)&
                        (rs2_EXE == rd_MEM) &
                        (|rs2_ID);
    assign forward_ctrl_ls = ST_Forward;

    assign reg_FD_EN = 1'b1;
    assign reg_DE_EN = 1'b1;
    assign reg_EM_EN = 1'b1;
    assign reg_EM_flush = 1'b0;
    assign reg_MW_EN = 1'b1;

    //modified
    // Detect load-use hazard for rs1 (i.e., rs1 is being used after a load instruction in EX stage)
    assign rs1_stall = rs1use_ID & 
                (rs1_ID == rd_EXE) & 
                (|rs1_ID) & 
                (hazard_optype_ID != HAZARD_ST) &  // Not for store instruction
                (hazard_optype_EX == HAZARD_MEM);  // EX stage is a load instruction

    // Detect load-use hazard for rs2 (i.e., rs2 is being used after a load instruction in EX stage)
    assign rs2_stall = rs2use_ID & 
                (rs2_ID == rd_EXE) & 
                (|rs2_ID) & 
                (hazard_optype_ID != HAZARD_ST) &  // Not for store instruction
                (hazard_optype_EX == HAZARD_MEM);  // EX stage is a load instruction

    // Stall Fetch-Decode stage when a load-use hazard is detected
    assign reg_FD_stall = rs1_stall | rs2_stall;//什么时候IF到ID阶段需要等待前面的指令执行完成。这个空比较长，有四个判断条件同时成立时这个信号才会触发  

    // Flush Decode-Execute stage when a stall occurs
    // and this type of hazard become solvable by MEM forwarding
    assign reg_DE_flush = reg_FD_stall;
    assign PC_EN_IF = ~reg_FD_stall;
    
    //modifiy
    assign reg_FD_flush = Branch_ID;//什么时候IF到ID的寄存器需要被清空



endmodule