`timescale 1ns / 1ps


module CtrlUnit(
    input[31:0] inst,       //指令
    input cmp_res,          //cmp结果
    output Branch, ALUSrc_A, ALUSrc_B, DatatoReg, RegWrite, mem_w,
        MIO, rs1use, rs2use,
    output [1:0] hazard_optype,
    output [2:0] ImmSel, cmp_ctrl,
    output [3:0] ALUControl,
    output JALR
);

    wire[6:0] funct7 = inst[31:25];
    wire[2:0] funct3 = inst[14:12];
    wire[6:0] opcode = inst[6:0];

    wire Rop = opcode == 7'b0110011;
    wire Iop = opcode == 7'b0010011;
    wire Bop = opcode == 7'b1100011;
    wire Lop = opcode == 7'b0000011;
    wire Sop = opcode == 7'b0100011;

    wire funct7_0  = funct7 == 7'h0;
    wire funct7_32 = funct7 == 7'h20;

    wire funct3_0 = funct3 == 3'h0;
    wire funct3_1 = funct3 == 3'h1;
    wire funct3_2 = funct3 == 3'h2;
    wire funct3_3 = funct3 == 3'h3;
    wire funct3_4 = funct3 == 3'h4;
    wire funct3_5 = funct3 == 3'h5;
    wire funct3_6 = funct3 == 3'h6;
    wire funct3_7 = funct3 == 3'h7;

    wire ADD  = Rop & funct3_0 & funct7_0;
    //把寄存器 x[rs2]加到寄存器 x[rs1]上，结果写入 x[rd]。忽略算术溢出。
    wire SUB  = Rop & funct3_0 & funct7_32;
    //x[rs1]减去 x[rs2]，结果写入 x[rd]。忽略算术溢出。
    wire SLL  = Rop & funct3_1 & funct7_0;
    //把寄存器 x[rs1]左移 x[rs2]位，空出的位置填入 0，结果写入 x[rd]。x[rs2]的低 5 位（如果是RV64I 则是低 6 位）代表移动位数，其高位则被忽略。
    wire SLT  = Rop & funct3_2 & funct7_0;
    //比较 x[rs1]和 x[rs2]中的数，如果 x[rs1]更小，向 x[rd]写入 1，否则写入0
    wire SLTU = Rop & funct3_3 & funct7_0;
    //比较 x[rs1]和 x[rs2]，比较时视为无符号数。如果 x[rs1]更小，向 x[rd]写入 1，否则写入 0
    wire XOR  = Rop & funct3_4 & funct7_0;
    //x[rs1]和 x[rs2]按位异或，结果写入 x[rd]
    wire SRL  = Rop & funct3_5 & funct7_0;
    //把寄存器 x[rs1]右移 x[rs2]位，空出的位置填入 0，结果写入 x[rd]。x[rs2]的低 5 位（如果是RV64I 则是低 6 位）代表移动位数，其高位则被忽略
    wire SRA  = Rop & funct3_5 & funct7_32;
    //把寄存器 x[rs1]右移 x[rs2]位，空位用 x[rs1]的最高位填充，结果写入 x[rd]。x[rs2]的低 5 位（如果是 RV64I 则是低 6 位）为移动位数，高位则被忽略。
    wire OR   = Rop & funct3_6 & funct7_0;
    //把寄存器 x[rs1]和寄存器 x[rs2]按位取或，结果写入 x[rd]
    wire AND  = Rop & funct3_7 & funct7_0;
    //将寄存器 x[rs1]和寄存器 x[rs2]位与的结果写入 x[rd]

    wire ADDI  = Iop & funct3_0;	
    //把符号位扩展的立即数加到寄存器 x[rs1]上，结果写入 x[rd]。忽略算术溢出。
    wire SLTI  = Iop & funct3_2;
    //比较 x[rs1]和有符号扩展的 immediate，如果 x[rs1]更小，向 x[rd]写入 1，否则写入 0
    wire SLTIU = Iop & funct3_3;
    //比较 x[rs1]和有符号扩展的 immediate，比较时视为无符号数。如果 x[rs1]更小，向 x[rd]写入1，否则写入 0
    wire XORI  = Iop & funct3_4;
    //x[rs1]和有符号扩展的 immediate 按位异或，结果写入 x[rd]
    wire ORI   = Iop & funct3_6;
    //把寄存器 x[rs1]和有符号扩展的立即数 immediate 按位取或，结果写入 x[rd]
    wire ANDI  = Iop & funct3_7;
    //把符号位扩展的立即数和寄存器 x[rs1]上的值进行位与，结果写入 x[rd]
    wire SLLI  = Iop & funct3_1 & funct7_0;
    //把寄存器x[rs1]左移shamt位，空出的位置填入0，结果写入x[rd]。对于RV32I，仅当shamt[5]=0时，指令才是有效的。
    wire SRLI  = Iop & funct3_5 & funct7_0;
    //把寄存器x[rs1]右移shamt位，空出的位置填入0，结果写入x[rd]。对于RV32I，仅当shamt[5]=0时，指令才是有效的。
    wire SRAI  = Iop & funct3_5 & funct7_32;
    //把寄存器 x[rs1]右移 shamt 位，空位用 x[rs1]的最高位填充，结果写入 x[rd]。对于 RV32I，仅当 shamt[5]=0 时指令有效。

    wire BEQ = Bop & funct3_0;                            //to fill sth. in 
    //beq指令只有在操作数寄存器rs1中的数值与操作数寄存器rs2的数值相等时，才会跳转跳,转地址为offset的有符号扩展和最低位补0（即乘以2）的偏移量加上BEQ指令的地址。
    wire BNE = Bop & funct3_1;                            //to fill sth. in 
    //bne指令只有在操作数寄存器rs1中的数值与操作数寄存器rs2的数值不相等时，才会跳转,转地址为offset的有符号扩展和最低位补0（即乘以2）的偏移量加上指令的地址
    wire BLT = Bop & funct3_4;                            //to fill sth. in 
    //blt指令只有在操作数寄存器rs1中的数值小于操作数寄存器rs2的数值时（有符号数），才会跳转,转地址为offset的有符号扩展和最低位补0（即乘以2）的偏移量加上指令的地址
    wire BGE = Bop & funct3_5;                            //to fill sth. in 
    //bge指令只有在操作数寄存器rs1中的数值大于或等于操作数寄存器rs2的数值时（有符号数），才会跳转,转地址为offset的有符号扩展和最低位补0（即乘以2）的偏移量加上指令的地址
    wire BLTU = Bop & funct3_6;                           //to fill sth. in 
    //bltu指令只有在操作数寄存器rs1中的数值小于操作数寄存器rs2的数值时（无符号数），才会跳转,转地址为offset的有符号扩展和最低位补0（即乘以2）的偏移量加上指令的地址
    wire BGEU = Bop & funct3_7;                           //to fill sth. in 
    //bgeu指令只有在操作数寄存器rs1中的数值大于或等于操作数寄存器rs2的数值时（无符号数），才会跳转,转地址为offset的有符号扩展和最低位补0（即乘以2）的偏移量加上指令的地址

    wire LB =  Lop & funct3_0;                            //to fill sth. in 
    //从地址 x[rs1] + sign-extend(offset)读取一个字节，经符号位扩展后写入 x[rd]
    wire LH =  Lop & funct3_1;                            //to fill sth. in 
    //从地址 x[rs1] + sign-extend(offset)读取两个字节，经符号位扩展后写入 x[rd]
    wire LW =  Lop & funct3_2;                            //to fill sth. in 
    //从地址 x[rs1] + sign-extend(offset)读取四个字节，写入 x[rd]。对于 RV64I，结果要进行符号扩展。
    wire LBU = Lop & funct3_4;                            //to fill sth. in 
    //从地址 x[rs1] + sign-extend(offset)读取一个字节，经零扩展后写入 x[rd]
    wire LHU = Lop & funct3_5;                            //to fill sth. in 
    //从地址 x[rs1] + sign-extend(offset)读取两个字节，经零扩展后写入 x[rd]。

    wire SB = Sop & funct3_0;                             //to fill sth. in 
    //将 x[rs2]的低位字节存入内存地址 x[rs1]+sign-extend(offset)
    wire SH = Sop & funct3_1;                             //to fill sth. in 
    //将 x[rs2]的低位 2 个字节存入内存地址 x[rs1]+sign-extend(offset)。
    wire SW = Sop & funct3_2;                             //to fill sth. in 
    //将 x[rs2]的低位 4 个字节存入内存地址 x[rs1]+sign-extend(offset)。

    wire LUI   = opcode == 7'b0110111;                          //to fill sth. in 
    //LUI：将20位立即数的值左移12位（低12位补零）成为一个32位数，将其写回rd。这一指令主要是为了在寄存器中存入比较大的立即数
    wire AUIPC = opcode == 7'b0010111;                          //to fill sth. in 
    //AUIPC：将20位立即数的值左移12位（低12位补零）成为一个32位数，再加上该指令的pc值，再将结果写回rd。

    wire JAL  = opcode == 7'b1101111;                           //to fill sth. in 
    //PC+4 的结果送 rd 但不送入PC，然后计算下条指令地址。转移地址采用相对寻址，基准地址为当前指令地址(即 PC)，偏移量为立即数 imm20 经符扩展后的值的 2 倍。
    assign JALR = opcode == 7'b1100111;                        //to fill sth. in 
    //JALR:jalr 指令使用 12 位立即数（有符号数）作为偏移量，与操作数寄存器 rs1中的值相加,然后将结果的最低有效位置0。jalr指令将其下一条指令的 PC（即当前指令PC+4）的值写入其结果寄存器 rd。

    wire R_valid = AND | OR | ADD | XOR | SLL | SRL | SRA | SUB | SLT | SLTU;
    wire I_valid = ANDI | ORI | ADDI | XORI | SLLI | SRLI | SRAI | SLTI | SLTIU;
    wire B_valid = BEQ | BNE | BLT | BGE | BLTU | BGEU;
    wire L_valid = LW | LH | LB | LHU | LBU;
    wire S_valid = SW | SH | SB;


    assign Branch = BEQ | BNE | BLT | BGE | BLTU | BGEU | JAL | JALR;                       //to fill sth. in 

    parameter Imm_type_I = 3'b001;
    parameter Imm_type_B = 3'b010;
    parameter Imm_type_J = 3'b011;
    parameter Imm_type_S = 3'b100;
    parameter Imm_type_U = 3'b101;
    assign ImmSel = {3{I_valid | JALR | L_valid}} & Imm_type_I |
                    {3{B_valid}}                  & Imm_type_B |
                    {3{JAL}}                      & Imm_type_J |
                    {3{S_valid}}                  & Imm_type_S |
                    {3{LUI | AUIPC}}              & Imm_type_U ;


    assign cmp_ctrl = 3'b001 & {3{BEQ}} |
                      3'b010 & {3{BNE}} |
                      3'b011 & {3{BLT}} |
                      3'b100 & {3{BLTU}} |
                      3'b101 & {3{BGE}} |
                      3'b110 & {3{BGEU}};                         //to fill sth. in 

    assign ALUSrc_A = R_valid | I_valid | B_valid | L_valid | S_valid | LUI;                         //to fill sth. in 

    assign ALUSrc_B = I_valid | L_valid | S_valid | LUI | AUIPC;             //to fill sth. in 

    parameter ALU_ADD  = 4'b0001;
    parameter ALU_SUB  = 4'b0010;
    parameter ALU_AND  = 4'b0011;
    parameter ALU_OR   = 4'b0100;
    parameter ALU_XOR  = 4'b0101;
    parameter ALU_SLL  = 4'b0110;
    parameter ALU_SRL  = 4'b0111;
    parameter ALU_SLT  = 4'b1000;
    parameter ALU_SLTU = 4'b1001;
    parameter ALU_SRA  = 4'b1010;
    parameter ALU_Ap4  = 4'b1011;
    parameter ALU_Bout = 4'b1100;
    assign ALUControl = {4{ADD | ADDI | L_valid | S_valid | AUIPC}} & ALU_ADD  |
                        {4{SUB}}                                    & ALU_SUB  |
                        {4{AND | ANDI}}                             & ALU_AND  |
                        {4{OR | ORI}}                               & ALU_OR   |
                        {4{XOR | XORI}}                             & ALU_XOR  |
                        {4{SLL | SLLI}}                             & ALU_SLL  |
                        {4{SRL | SRLI}}                             & ALU_SRL  |
                        {4{SLT | SLTI}}                             & ALU_SLT  |
                        {4{SLTU | SLTIU}}                           & ALU_SLTU |
                        {4{SRA | SRAI}}                             & ALU_SRA  |
                        {4{JAL | JALR}}                             & ALU_Ap4  |
                        {4{LUI}}                                    & ALU_Bout ;

    assign DatatoReg = L_valid;

    assign RegWrite = R_valid | I_valid | JAL | JALR | L_valid | LUI | AUIPC;

    assign mem_w = S_valid;

    assign MIO = L_valid | S_valid;

    assign rs1use =  R_valid | I_valid | S_valid | B_valid | L_valid | JALR;                        //to fill sth. in 

    assign rs2use = R_valid | S_valid | B_valid;                         //to fill sth. in 


    //hazard_optype 
    assign hazard_optype =  2'b01 & {2{R_valid | I_valid | JAL | JALR | LUI | AUIPC}} |
                            2'b10 & {2{L_valid}}|
                            2'b11 & {2{S_valid}};                  //to fill sth. in 

endmodule