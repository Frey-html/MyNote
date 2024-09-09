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
    //according to the diagram, design the Hazard Detection Unit
    reg[1:0] hazard_optype_EXE, hazard_optype_MEM;
    always@(posedge clk) begin
        hazard_optype_MEM <= hazard_optype_EXE;
        hazard_optype_EXE <= hazard_optype_ID & {2{~reg_DE_flush}};
    end

    //1.���ݾ���   01 ��L������Ҫ��д�Ĵ���ָ�� 10 L���� 11 S���� 
    //EXE�׶�forward��ID�׶�
    wire rs1_forward_1     = rs1use_ID && rs1_ID == rd_EXE && rd_EXE && hazard_optype_EXE == 2'b01;
    //���ʱL����ָ�����ӷ�S���Ͷ���Ӧ�Ĵ���ָ���Ҫstall 1��cycle
    wire rs1_forward_stall = rs1use_ID && rs1_ID == rd_EXE && rd_EXE && hazard_optype_EXE == 2'b10
                             && hazard_optype_ID != 2'b11;
    //��һ�����͵�hazard��Ӷ���Ӧ�Ĵ���ָ�MEM�׶�forward��ID�׶�     
    wire rs1_forward_2     = rs1use_ID && rs1_ID == rd_MEM && rd_MEM && hazard_optype_MEM == 2'b01;
    //L����ָ��һ���ں�Ӷ���Ӧ�Ĵ���ָ���MEMforward��ID
    wire rs1_forward_3     = rs1use_ID && rs1_ID == rd_MEM && rd_MEM && hazard_optype_MEM == 2'b10;

    wire rs2_forward_1     = rs2use_ID && rs2_ID == rd_EXE && rd_EXE && hazard_optype_EXE == 2'b01;
    wire rs2_forward_stall = rs2use_ID && rs2_ID == rd_EXE && rd_EXE && hazard_optype_EXE == 2'b10
                             && hazard_optype_ID != 2'b11;
    wire rs2_forward_2     = rs2use_ID && rs2_ID == rd_MEM && rd_MEM && hazard_optype_MEM == 2'b01;
    wire rs2_forward_3     = rs2use_ID && rs2_ID == rd_MEM && rd_MEM && hazard_optype_MEM == 2'b10;

    //L����ָ������S����ָ��
    assign forward_ctrl_ls = rs2_EXE == rd_MEM && hazard_optype_EXE == 2'b11
                            && hazard_optype_MEM == 2'b10;

    assign forward_ctrl_A = {2{rs1_forward_1}} & 2'd1 |
                            {2{rs1_forward_2}} & 2'd2 |
                            {2{rs1_forward_3}} & 2'd3 ;

    assign forward_ctrl_B = {2{rs2_forward_1}} & 2'd1 |
                            {2{rs2_forward_2}} & 2'd2 |
                            {2{rs2_forward_3}} & 2'd3 ;


    wire load_stall = rs1_forward_stall | rs2_forward_stall;

    assign PC_EN_IF = ~load_stall;
    assign reg_FD_EN = ~load_stall;
    assign reg_FD_stall = load_stall;
    assign reg_FD_flush = Branch_ID;
    assign reg_DE_EN = ~load_stall;
    assign reg_DE_flush = load_stall;    
    assign reg_EM_EN = 1;
    assign reg_EM_flush = 0;
    assign reg_MW_EN = 1;

    



endmodule