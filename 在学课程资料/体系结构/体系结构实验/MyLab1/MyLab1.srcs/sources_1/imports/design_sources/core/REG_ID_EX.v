`timescale 1ns / 1ps

module    REG_ID_EX(input clk,
                    input rst,
                    input EN, 
                    input flush,                                  
                    input [31:0] IR_ID,                            
                    input [31:0] PCurrent_ID,                      
                    input [4:0] rs1_addr,                            
                    input [4:0] rs2_addr,                             
                    input [31:0] rs1_data,                           
                    input [31:0] rs2_data,                           
                    input [31:0] Imm32,                                
                    input [4:0]  rd_addr,                           
                    input ALUSrc_A,                            
                    input ALUSrc_B,                            
                    input [3:0]  ALUC,                        
                    input DatatoReg,                        
                    input RegWrite,                            
                    input WR,                                     
                    input [2:0] u_b_h_w,
                    input MIO,

                    output reg[31:0] PCurrent_EX,          
                    output reg[31:0] IR_EX,                     
                    output reg[4:0]  rs1_EX,
                    output reg[4:0]  rs2_EX,
                    output reg[31:0] A_EX,                          
                    output reg[31:0] B_EX,                    
                    output reg[31:0] Imm32_EX,                    
                    output reg[4:0]  rd_EX,                    
                    output reg       ALUSrc_A_EX,              
                    output reg       ALUSrc_B_EX,                  
                    output reg[3:0]  ALUC_EX,                     
                    output reg       DatatoReg_EX,            
                    output reg       RegWrite_EX,              
                    output reg       WR_EX,                   
                    output reg[2:0]  u_b_h_w_EX,
                    output reg       MIO_EX
                );

    always @(posedge clk or posedge rst) begin   
    if(rst) begin
        rd_EX        <= 0;
        RegWrite_EX  <= 0;
        WR_EX        <= 0;
        IR_EX        <= 32'h00000000;
        PCurrent_EX  <= 32'h00000000 ;
        rs1_EX       <= 0;
        rs2_EX       <= 0;
        MIO_EX       <= 0;
    end
    else if(EN)begin
        //modified
            if(flush)begin                          
                //PCurrent_EX <= PCurrent_ID;
                // TO_BE_FILLED，Flush时应该进行什么操作，哪些reg的值需要被重置
                PCurrent_EX  <= 32'h00000000;      // Reset PC
                IR_EX        <= 32'h00000000;      // Invalidate the instruction
                rd_EX        <= 0;                 // Reset destination register
                A_EX         <= 32'h00000000;      // Reset operand A
                B_EX         <= 32'h00000000;      // Reset operand B
                Imm32_EX     <= 32'h00000000;      // Reset immediate value
                rs1_EX       <= 0;                 // Reset source registers
                rs2_EX       <= 0;
                ALUSrc_A_EX  <= 0;                 // Reset ALU source control
                ALUSrc_B_EX  <= 0;
                ALUC_EX      <= 4'b0000;           // Reset ALU control
                DatatoReg_EX <= 0;                 // Prevent writing back data
                RegWrite_EX  <= 0;                 // Disable register write
                WR_EX        <= 0;                 // Disable memory write
                u_b_h_w_EX   <= 3'b000;            // Reset memory width control
                MIO_EX       <= 0;                 // Disable memory I/O
            end
            else begin                             
                PCurrent_EX <= PCurrent_ID;        
                IR_EX       <= IR_ID;              
                A_EX        <= rs1_data;            
                B_EX        <= rs2_data;            
                Imm32_EX    <= Imm32;           
                rd_EX       <= rd_addr;          
                rs1_EX      <= rs1_addr;
                rs2_EX      <= rs2_addr;
                ALUSrc_A_EX <= ALUSrc_A;           
                ALUSrc_B_EX <= ALUSrc_B;        
                ALUC_EX     <= ALUC;                
                DatatoReg_EX<= DatatoReg;            
                RegWrite_EX <= RegWrite;            
                WR_EX       <= WR;             
                u_b_h_w_EX  <= u_b_h_w;
                MIO_EX      <= MIO;

                end
        end
    end

endmodule