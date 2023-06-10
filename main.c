#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int text_ars = 0x400000;
int word_ars = 0x10000000;
int PC_ars = 0x400000;
int register_arr[32] = {0,};
int object_array[100];
int branch_ars = 0;
int text_size = 0;
int data_size = 0;
int format = 0;
int IDtoEX_hazard = 0;
int EXtoMEM_hazard = 0;
int IF_stall = 0;
int ID_stall = 0;
int EX_stall = 0;
int IF_check = 0;
int ID_check = 0;
int EX_check = 0;
int MEM_check = 0;
int prediction_opt = 0;
int branch_prediction = 0;

struct data_arr{
    int binary;
    int adrress;
};

struct text_arr{
    int binary;
    int adrress;
};

struct R_format{
    int op, rs, rt, rd,shamt, funct;
};

struct I_format{
    int op, rs, rt, imm;
};

struct J_format{
    int op, jump_target;
};

struct IF_ID{
    int Instr;
    int NPC;
    int PC;
};

struct ID_EX{
    int binary;
    int rs;
    int rt;
    int imm;
    int rd;
    int shamt;
    int jump_target;
    int forward;
    int NPC;
    char *inst_type;
    int PC;
    int target_address;
};

struct EX_MEM{
    int alu_out;
    int br_target;
    int rs;
    int rt;
    int rd;
    char *inst_type;
    int check_inst;
    int up;
    int left;
    int PC;
};

struct MEM_WB{
    int rs;
    int rt;
    int rd;
    char *inst_type;
    int check_inst;
    int alu_out;
    int mem_out;
    int PC;
};

struct pipeline_PC{
    int if_pc;
    int id_pc;
    int ex_pc;
    int mem_pc;
    int wb_pc;
};


struct IF_ID if_id = {.PC = 0};
struct ID_EX id_ex = {.binary = 0, .PC = 0};
struct EX_MEM ex_mem = {.PC = 0};
struct MEM_WB mem_wb = {.PC = 0};
struct pipeline_PC pipeline_pc = {0};
struct R_format R_F;
struct I_format I_F;
struct J_format J_f;

int sign_extend(int i){
    if(i >> 15 == 1){
        i = i + 0XFFFF0000;
    }
    return i;
}

int hazard_check_1(){
    if(strcmp(ex_mem.inst_type, "I_TYPE") == 0){
        if(ex_mem.rt == id_ex.rs && ex_mem.rt != id_ex.rt){
            id_ex.forward = ex_mem.alu_out;
            return 1;
        }
        else if(ex_mem.rt != id_ex.rs && ex_mem.rt == id_ex.rt){
            id_ex.forward = ex_mem.alu_out;
            return 2;
        }
        else if(ex_mem.rt == id_ex.rs && ex_mem.rt == id_ex.rt){
            id_ex.forward = ex_mem.alu_out;
            return 3;
        }
    }
    else if(strcmp(ex_mem.inst_type, "R_TYPE") == 0 && ex_mem.rd != 0){
        if(ex_mem.rd == id_ex.rs && ex_mem.rd != id_ex.rt){
            id_ex.forward = ex_mem.alu_out;
            return 1;
        }
        else if(ex_mem.rd != id_ex.rs && ex_mem.rd == id_ex.rt){
            id_ex.forward = ex_mem.alu_out;
            return 2;
        }
        else if(ex_mem.rd == id_ex.rs && ex_mem.rd == id_ex.rt){
            id_ex.forward = ex_mem.alu_out;
            return 3;
        }
    }
    return 0;
}



void ADDU(struct R_format *R, int check){
    if(check == 0){
        ex_mem.alu_out = register_arr[R -> rt] + register_arr[R -> rs];
    }
    else if(check == 1){
        ex_mem.alu_out = register_arr[R -> rt] + id_ex.forward;
    }
    else if(check == 2){
        ex_mem.alu_out = id_ex.forward + register_arr[R -> rs];
    }else{
        ex_mem.alu_out = id_ex.forward + id_ex.forward;
    }
}

void AND(struct R_format *R, int check){
    if(check == 0){
        ex_mem.alu_out = register_arr[R -> rt] & register_arr[R -> rs];
    }
    else if(check == 1){
        ex_mem.alu_out = register_arr[R -> rt] & id_ex.forward;
    }
    else if(check == 2){
        ex_mem.alu_out = id_ex.forward & register_arr[R -> rs];
    }else{
        ex_mem.alu_out = id_ex.forward & id_ex.forward;
    }
}

void NOR(struct R_format *R, int check){
    if(check == 0){
        ex_mem.alu_out = ~(register_arr[R -> rt] | register_arr[R -> rs]);
    }
    else if(check == 1){
        ex_mem.alu_out = ~(register_arr[R -> rt] | id_ex.forward);
    }
    else if(check == 2){
        ex_mem.alu_out = ~(id_ex.forward | register_arr[R -> rs]);
    }else{
        ex_mem.alu_out = ~(id_ex.forward | id_ex.forward);
    }
}

void OR(struct R_format *R, int check){
    if(check == 0){
        ex_mem.alu_out = (register_arr[R -> rt] | register_arr[R -> rs]);
    }
    else if(check == 1){
        ex_mem.alu_out = (register_arr[R -> rt] | id_ex.forward);
    }
    else if(check == 2){
        ex_mem.alu_out = (id_ex.forward | register_arr[R -> rs]);
    }else{
        ex_mem.alu_out = (id_ex.forward | id_ex.forward);
    }
}

void SLTU(struct R_format *R, int check){
    if(check == 0){
        if(register_arr[R -> rs] < register_arr[R -> rt]){
            ex_mem.alu_out = 1;
        }else{
            ex_mem.alu_out = 0;
        }
    }
    else if(check == 1){
        if(id_ex.forward < register_arr[R -> rt]){
            ex_mem.alu_out = 1;
        }else{
            ex_mem.alu_out = 0;
        }
    }
    else if(check == 2){
        if(register_arr[R -> rs] < id_ex.forward){
            ex_mem.alu_out = 1;
        }else{
            ex_mem.alu_out = 0;
        }
    }else{
        if(id_ex.forward < id_ex.forward){
            ex_mem.alu_out = 1;
        }else{
            ex_mem.alu_out = 0;
        }
    }

}

void SLL(struct R_format *R, int check){
    if(check == 2 || check == 3){
        ex_mem.alu_out = id_ex.forward << (R -> shamt);
    }else{
        ex_mem.alu_out = register_arr[R -> rt] << (R -> shamt);
    }
    
}

void SRL(struct R_format *R, int check){
    if(check == 1 || check == 3){
        ex_mem.alu_out = id_ex.forward >> (R -> shamt);
    }else{
        ex_mem.alu_out = register_arr[R -> rt] >> (R -> shamt);
    }
}

void SUBU(struct R_format *R, int check){
    if(check == 0){
        ex_mem.alu_out = register_arr[R -> rs] - register_arr[R -> rt]; 
    }
    else if(check ==1){
        ex_mem.alu_out = id_ex.forward - register_arr[R -> rt];
    }
    else if(check == 2){
        ex_mem.alu_out = register_arr[R -> rs] - id_ex.forward;
    }else{
        ex_mem.alu_out = id_ex.forward - id_ex.forward;
    }
}

void JR(struct R_format *R, struct text_arr *Text_arr, int check){
    if(check == 1 || check == 3){
        ex_mem.br_target = id_ex.forward;    
    }else{
        ex_mem.br_target = register_arr[R -> rs];
    }
    
    /*
    for(int i = 0; i < text_size; i++){
        if(Text_arr[i].adrress == branch_ars){
            PC_ars = branch_ars;
        }
    }
    */
}
void ADDIU(struct I_format *I, int check){
    if(check == 0){
        ex_mem.alu_out = register_arr[I -> rs] + sign_extend((I -> imm)); // sign-extention 시키기
    }
    else if(check == 1){
        ex_mem.alu_out = id_ex.forward + sign_extend((I -> imm)); 
    }
    else if(check ==2){
        ex_mem.alu_out = register_arr[I -> rs] + sign_extend((I -> imm));
    }else{
        ex_mem.alu_out = id_ex.forward + sign_extend((I -> imm));
    }
    
    
}

void ANDI(struct I_format *I, int check){
    if(check == 0){
        ex_mem.alu_out = register_arr[I -> rs] & (I -> imm);
    }
    else if(check == 1){
        ex_mem.alu_out = id_ex.forward & (I -> imm); 
    }
    else if(check ==2){
        ex_mem.alu_out = register_arr[I -> rs] & (I -> imm);
    }else{
        ex_mem.alu_out = id_ex.forward & (I -> imm); 
    }
}

void BEQ(struct I_format *I, struct text_arr *Text_arr, struct data_arr *Data_arr, int check){
    if(check == 0){
        if(register_arr[I -> rs] == register_arr[I -> rt]){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        }
    }
    else if(check == 1){
        if(id_ex.forward == register_arr[I -> rt]){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        } 
    }
    else if(check ==2){
        if(register_arr[I -> rs] == id_ex.forward){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        }
    }else{
        if(id_ex.forward == id_ex.forward){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        } 
    }
}

void BNE(struct I_format *I, struct text_arr *Text_arr, struct data_arr *Data_arr, int check){
    if(check == 0){
        if(register_arr[I -> rs] != register_arr[I -> rt]){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        }
    }
    else if(check == 1){
        if(id_ex.forward != register_arr[I -> rt]){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        } 
    }
    else if(check ==2){
        if(register_arr[I -> rs] != id_ex.forward){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        }
    }else{
        if(id_ex.forward != id_ex.forward){
            ex_mem.br_target = ex_mem.PC + (I -> imm)*4 + 4;
            branch_prediction = 1;
        } 
    }
}

void LUI(struct I_format *I, int check){
    ex_mem.alu_out = 0XFFFFFFFF & ((I -> imm) << 16);
}

void LW(struct I_format *I, struct data_arr *Data_arr, int check){
    ex_mem.up = (I -> imm) /4;

}

void LB(struct I_format *I, struct data_arr *Data_arr, int check){
    ex_mem.up = (I -> imm) / 4;
    ex_mem.left = (I -> imm) % 4;
}

void ORI(struct I_format *I, int check){
    if(check ==0 || check == 2){
        ex_mem.alu_out = register_arr[I -> rs] | (I -> imm); //zero_extended 필요
    }else{
        ex_mem.alu_out = id_ex.forward | (I -> imm);
    }
}

void SLTIU(struct I_format *I, int check){
    if(check == 0 || check == 2){
        if(register_arr[I -> rs] < sign_extend(I -> imm)){
            ex_mem.alu_out = 1;
        }else{
            ex_mem.alu_out = 0;
        }
    }else{
        if(id_ex.forward < sign_extend(I -> imm)){
            ex_mem.alu_out = 1;
        }else{
            ex_mem.alu_out = 0;
        }        
    }
}
void SW(struct I_format *I, struct data_arr *Data_arr, int check){
    ex_mem.up = (I -> imm) / 4;
    
}

void SB(struct I_format *I, struct data_arr *Data_arr, int check){
    ex_mem.up = (I -> imm) / 4;
    ex_mem.left = (I -> imm) % 4;
    
}

void J(struct J_format *J_f, struct text_arr *Text_arr, struct data_arr *Data_arr, int check){
    branch_ars = (J_f -> jump_target) * 4;
    PC_ars = branch_ars;

}

void JAL(struct J_format *J_f, struct text_arr *Text_arr, struct data_arr *Data_arr, int check){
    register_arr[31] = PC_ars+4;
    branch_ars = (J_f -> jump_target) * 4;
    PC_ars = branch_ars;
}

int execution(int binary, struct text_arr *Text_arr, struct data_arr *Data_arr, int check){
    int op = (binary & 0XFF000000)>> 26;

    if(op == 0){
        

        if(R_F.funct == 0X21){
            ADDU(&R_F,check);
            return 0;
        }
        else if(R_F.funct == 0X24){
            AND(&R_F,check);
            return 0;
        } 
        else if(R_F.funct == 0X27){
            NOR(&R_F,check);
            return 0;
        }
        else if(R_F.funct == 0X25){
            OR(&R_F,check);
            return 0;
        }
        else if(R_F.funct == 0X2B){
            SLTU(&R_F,check);
            return 0;
        }
        else if(R_F.funct == 0){
            SLL(&R_F,check);
            return 0;
        }
        else if(R_F.funct == 2){
            SRL(&R_F,check);
            return 0;
        }
        else if(R_F.funct == 0X23){
            SUBU(&R_F,check);
            return 0;
        }
        else if(R_F.funct == 8){
            JR(&R_F, Text_arr,check);
            PC_ars = PC_ars-4;
            return 0;
        }
    }

    else if(op==2 | op == 3){

        if(op == 2){
            J_f.op = op;
            J(&J_f, Text_arr, Data_arr,check);
            PC_ars = PC_ars-4;
            return 0;
        }else{
            J_f.op = op;
            JAL(&J_f, Text_arr, Data_arr,check);
            PC_ars = PC_ars-4;
            return 0;
        }
    }else{

        if(I_F.op == 9){
            ADDIU(&I_F,check);
            return 0;
        }
        else if(I_F.op == 0XC){
            ANDI(&I_F,check);
            return 0;
        }
        else if(I_F.op == 4){
            BEQ(&I_F, Text_arr, Data_arr,check);
            ex_mem.check_inst = 5;
            return 0;
        }
        else if(I_F.op == 5){
            BNE(&I_F, Text_arr, Data_arr,check);
            ex_mem.check_inst = 6;
            return 0;
        }
        else if(I_F.op == 0XF){
            LUI(&I_F,check);
            return 0;
        }
        else if(I_F.op == 0X23){
            LW(&I_F,Data_arr,check);
            ex_mem.check_inst = 1;
            return 1;
        }
        else if(I_F.op == 0X20){
            LB(&I_F,Data_arr,check);
            ex_mem.check_inst = 2;
            return 1;
        }
        else if(I_F.op == 0XD){
            ORI(&I_F,check);
            return 0;
        }
        else if(I_F.op == 0XB){
            SLTIU(&I_F,check);
            return 0;
        }
        else if(I_F.op == 0X2B){
            SW(&I_F, Data_arr,check);
            ex_mem.check_inst = 3;
            return 1;
        }
        else if(I_F.op == 0X28){
            SB(&I_F, Data_arr,check);
            ex_mem.check_inst = 4;
            return 1;
        }
    }
}


int select_op(int binary, int* check_branch){
    int op = (binary & 0XFF000000)>> 26; 

    if(op == 0){
        
        R_F.funct = binary & 0X3F;
        R_F.op = 0;
        R_F.rs = (binary & 0X3E00000)>>21;
        R_F.rt = (binary & 0X1F0000)>>16;
        R_F.rd = (binary & 0XF800)>>11;
        R_F.shamt = (binary & 0X7C0) >>6;
        format = 1;
        return 1;
    
    }

    else if(op==2 | op == 3){
        
        J_f.jump_target = (binary & 0X3FFFFF);
        format = 3;
        return 2;

    }else{

        I_F.op = op;
        I_F.rs = (binary & 0X3E00000)>>21;
        I_F.rt = (binary & 0X1F0000)>>16;
        I_F.imm = binary & 0XFFFF;
        format = 2;
        if(I_F.op == 4 || I_F.op == 5){
            check_branch = 1;
        }
        return 3;
        
    }
}

void show_register(){
    printf("Current register values:\n");
    printf("-------------------------------------------\n");
    printf("PC: 0x%x\n",PC_ars);
    printf("Registers:\n");
    for(int i=0; i<32; i++){
        printf("R%d: 0x%x\n", i, register_arr[i]);
    }
}

void IF(int binary, struct text_arr *Text_arr, struct data_arr *Data_arr){
    if(IF_stall == 1){
        IF_stall = 0;
        return;
    }
    if_id.Instr = binary;
    if_id.PC = PC_ars;
    if_id.NPC = PC_ars + 4;  
    pipeline_pc.if_pc = PC_ars;
    PC_ars = PC_ars + 4;
    IF_check = 1;
    
}

void ID(struct text_arr *Text_arr, struct data_arr *Data_arr){
    if(ID_stall == 1){
        ID_stall = 0;
        return;
    }
    id_ex.binary = if_id.Instr;
    pipeline_pc.id_pc = if_id.PC;
    int check_branch = 0;
    id_ex.PC = if_id.PC;

    int operation_type = select_op(if_id.Instr, &check_branch);
    id_ex.NPC =  if_id.NPC;
    if(operation_type == 1){
        id_ex.rs = R_F.rs;
        id_ex.rt = R_F.rt;
        id_ex.rd = R_F.rd;
        id_ex.shamt = R_F.shamt;
        id_ex.inst_type = "R_TYPE";
    }
    else if(operation_type == 3){
        id_ex.rs = I_F.rs;
        id_ex.rt = I_F.rt;
        id_ex.imm = I_F.imm;
        id_ex.inst_type = "I_TYPE";
    }else{
        id_ex.jump_target = J_f.jump_target;
        id_ex.inst_type = "J_TYPE";
    }

    if(check_branch == 1){
        if(prediction_opt == 1){
            id_ex.target_address = id_ex.NPC + 4 + (I_F.imm)*4;
            if_id.NPC = id_ex.target_address;
        }
    }
    ID_check = 1;
}

void EX(struct text_arr *Text_arr, struct data_arr *Data_arr){
    if(EX_stall == 1){
        EX_stall = 0;
        return;
    }
    int hazard_1 = 0;
    if(ex_mem.PC !=0){
        hazard_1 = hazard_check_1();
    }
    

    pipeline_pc.ex_pc = id_ex.PC;
    ex_mem.PC = id_ex.PC;
    
    int check_load_store = execution(id_ex.binary, Text_arr, Data_arr, hazard_1);

    if(strcmp(id_ex.inst_type,"R_TYPE") == 0){
        ex_mem.rd = id_ex.rd;
        ex_mem.rs = id_ex.rt;
        ex_mem.rt = id_ex.rt;
        ex_mem.inst_type = id_ex.inst_type;
    }
    else if(strcmp(id_ex.inst_type,"I_TYPE") == 0){
        if(check_load_store == 1){
        }else{
            ex_mem.rs = id_ex.rs;
            ex_mem.rt = id_ex.rt;
            ex_mem.inst_type = id_ex.inst_type;
        }
    }
    EX_check = 1;
}

void MEM(struct data_arr *Data_arr){
    pipeline_pc.mem_pc = ex_mem.PC;
    mem_wb.PC = ex_mem.PC;

    if(strcmp(ex_mem.inst_type,"R_TYPE") == 0){
        mem_wb.rd = ex_mem.rd;
        mem_wb.rs = ex_mem.rs;
        mem_wb.rt = ex_mem.rt;
    }
    else if(strcmp(ex_mem.inst_type,"I_TYPE") == 0){
        mem_wb.rs = ex_mem.rs;
        mem_wb.rt = ex_mem.rt;
    }

    if(ex_mem.check_inst == 1){
        for(int i=0; i < data_size; i++){
            if(Data_arr[i].adrress == register_arr[I_F.rs]){
                mem_wb.alu_out = Data_arr[i+ex_mem.up].binary;
            }
        } 
        mem_wb.check_inst = ex_mem.check_inst;      
    }
    else if(ex_mem.check_inst == 2){
        for(int i=0; i < data_size; i++){
            if(Data_arr[i].adrress == register_arr[I_F.rs]){
                if(ex_mem.left == 0){
                    mem_wb.alu_out = (Data_arr[i+ex_mem.up].binary & 0XFF000000) >> 24;
                }
                else if(ex_mem.left == 1){
                    mem_wb.alu_out = (Data_arr[i+ex_mem.up].binary & 0X00FF0000) >> 16;
                }
                else if(ex_mem.left == 2){
                    mem_wb.alu_out = (Data_arr[i+ex_mem.up].binary & 0X0000FF00) >> 8;
                }else{
                    mem_wb.alu_out = (Data_arr[i+ex_mem.up].binary & 0X000000FF);
                }
            
            }
        }
        mem_wb.check_inst = ex_mem.check_inst;
    }
    else if(ex_mem.check_inst == 3){
        for(int i=0; i < data_size; i++){
            if(Data_arr[i].adrress == register_arr[I_F.rs]){
                Data_arr[i+ex_mem.up].binary = register_arr[I_F.rt];
            }
        }
        mem_wb.check_inst = ex_mem.check_inst;
    }else if(ex_mem.check_inst == 4){
        for(int i=0; i < data_size; i++){
            if(Data_arr[i].adrress == register_arr[I_F.rs]){
                if(ex_mem.left == 0){
                    Data_arr[i+ex_mem.up].binary = (register_arr[I_F.rt] & 0XFF000000) >> 24;
                }
                else if(ex_mem.left == 1){
                    Data_arr[i+ex_mem.up].binary = (register_arr[I_F.rt] & 0X00FF0000) >> 16;
                }
                else if(ex_mem.left == 2){
                    Data_arr[i+ex_mem.up].binary = (register_arr[I_F.rt] & 0X0000FF00) >> 8;
                }else{
                    Data_arr[i+ex_mem.up].binary = (register_arr[I_F.rt] & 0X000000FF);
                }
            
            }
        }
        mem_wb.check_inst = ex_mem.check_inst;
    }
    else{
        if(branch_prediction == 1 && prediction_opt == 1){
            branch_prediction = 0;

        }
        else if(prediction_opt == 1 && branch_prediction == 0){
            IF_stall = 1;
            ID_stall = 1;
            EX_stall = 1;
        }
        else if(prediction_opt == 2 && branch_prediction == 1){
            branch_prediction = 0;
            IF_stall = 1;
            ID_stall = 1;
            EX_stall = 1;
        }
    }
    mem_wb.alu_out = ex_mem.alu_out;
    mem_wb.inst_type = ex_mem.inst_type;
    MEM_check = 1;
}

void WB(){
    pipeline_pc.wb_pc =mem_wb.PC;
    if(strcmp(mem_wb.inst_type,"R_TYPE") == 0 ){
        register_arr[mem_wb.rd] = mem_wb.alu_out;
    }
    else if(strcmp(mem_wb.inst_type,"I_TYPE") == 0){
        if(mem_wb.check_inst == 0){
            register_arr[mem_wb.rt] = mem_wb.alu_out;
        }
        else if(mem_wb.check_inst == 1 || mem_wb.check_inst == 2){
            register_arr[mem_wb.rt] = mem_wb.alu_out;
        }    
    }
}

int find_binary(int PC,struct text_arr *Text_arr){
    for(int i=0; i< text_size; i++){
        if(PC == Text_arr[i].adrress){
            return Text_arr[i].binary;
        }
    }
    return 0;
}

void print_pc(int cycle){
    if(pipeline_pc.if_pc == 0 && pipeline_pc.id_pc == 0 && pipeline_pc.ex_pc == 0 && pipeline_pc.mem_pc == 0 && pipeline_pc.wb_pc == 0){
        printf("===== Completion Cycle: %d =====\n",(cycle-1));
    }else{
        printf("===== Completion Cycle: %d =====\n",(cycle));    
    }
    char if_pc[100] = "";
    char id_pc[100] = "";
    char ex_pc[100] = "";
    char mem_pc[100] = "";
    char wb_pc[100] = "";
    if(pipeline_pc.if_pc != 0){
        sprintf(if_pc, "0x%x", pipeline_pc.if_pc);
    }

    if(pipeline_pc.id_pc != 0){
        sprintf(id_pc, "0x%x",pipeline_pc.id_pc);
    }
    
    if(pipeline_pc.ex_pc != 0){
        sprintf(ex_pc, "0x%x",pipeline_pc.ex_pc);
    }
    
    if(pipeline_pc.mem_pc != 0){
        sprintf(mem_pc, "0x%x",pipeline_pc.mem_pc);
    }

    if(pipeline_pc.wb_pc != 0){
        sprintf(wb_pc, "0x%x",pipeline_pc.wb_pc);
    }

    printf("Current pipeline PC state:\n");
    printf("{%s|%s|%s|%s|%s}\n",if_pc,id_pc,ex_pc,mem_pc,wb_pc);
}

void main(int argc, char* argv[]){
    char *readfile;
    char line[200];
    int linecount =0;
    int num_instuction=0;
    char* m_start;
    char* m_end;
    char* temp;
    FILE *fp;
    int trigger_m=0;
    int trigger_d=0;
    int trigger_n=0;
    int trigger_p=0;
    int m_start_int;
    int m_end_int;
    int trigger = 0;
    int show_count=0;
    int inst_count=0;
    int cycle = 0;

    fp = fopen(argv[argc-1], "r");
    while(!feof(fp)){
        readfile = fgets(line, sizeof(line), fp);
        readfile = strtok(readfile,"\n");

        if(readfile == NULL){
            break;
        } 
        object_array[linecount] = strtol(readfile,NULL,16);
        if(linecount == 0){
            text_size = object_array[linecount]/4;
        }
        else if(linecount == 1){
            data_size = object_array[linecount]/4;
        }else{
            linecount++;
            continue;
        }
        linecount++;
    }

    struct data_arr Data_arr[data_size];
    struct text_arr Text_arr[text_size];

    for(int i=2; i < text_size+2; i++){
        Text_arr[i-2].adrress = text_ars;
        Text_arr[i-2].binary = object_array[i];
        text_ars = text_ars + 4;
    }

    for(int i=text_size+2; i< linecount; i++){
        Data_arr[i-text_size-2].adrress = word_ars;
        Data_arr[i-text_size-2].binary = object_array[i];
        word_ars = word_ars + 4;
    }

    for(int i=1; i<argc-1; i++){
        if(strstr(argv[i], "-m") != NULL){
            m_start = strtok(argv[i+1],":");
            m_end = strtok(NULL,"\0");
            m_start_int = strtol(m_start,NULL,16);
            m_end_int = strtol(m_end, NULL, 16);
            trigger_m = 1;
        }
        else if(strstr(argv[i], "-d") != NULL){
            trigger_d = 1;
        }
        else if(strstr(argv[i], "-n") != NULL){
            num_instuction = atoi(argv[i+1]);
            trigger_n = 1;
        }
        else if(strstr(argv[i], "-p") != NULL){
            trigger_p = 1;
        }
        else if(strstr(argv[i],"-atp") != NULL){
            prediction_opt = 1;
        }
        else if(strstr(argv[i],"-antp") != NULL){
            prediction_opt = 2;
        }
    }
    
    if(trigger_n == 1){
         
        while(1){
            
            if(MEM_check == 1){
                WB();
                MEM_check = 0;
            }
            if(EX_check == 1){
                MEM(Data_arr);
                EX_check = 0;
            }
            if(ID_check == 1){
                EX(Text_arr,Data_arr);
                ID_check = 0;
            }    
            if(IF_check == 1){
                ID(Text_arr, Data_arr);
                IF_check = 0;
            }   
            
            int binary = 0;
            if(cycle == 0){
                binary = find_binary(PC_ars,Text_arr);
            }else{
                binary = find_binary(if_id.NPC,Text_arr);
            }

            if(binary != 0){
                IF(binary,Text_arr, Data_arr);
            }
            cycle++;


        
            inst_count++;
            if(trigger_d == 1){
                show_count++;
                if(trigger_p == 1){
                    print_pc(cycle);
                }
                show_register();
                if(trigger_m == 1){
                    printf("\nMemory content [0x%x..0x%x]:\n", m_start_int, m_end_int);
                    printf("----------------------------\n");
                    for(int j = 0; j < text_size; j++){
                        if(Text_arr[j].adrress == m_start_int){    
                            for(int k = j; k< j + ((m_end_int - m_start_int)/4)+1; k++){
                                printf("0x%x: 0x%x\n", Text_arr[k].adrress, Text_arr[k].binary);
                            }
                            trigger = 1;
                        }
                    }
                    if(trigger == 0){
                        for(int j = 0; j< data_size; j++){
                            if(Data_arr[j].adrress == m_start_int){
                                for(int k = j; k< j + ((m_end_int - m_start_int)/4)+1; k++){
                                    printf("0x%x: 0x%x\n", Data_arr[k].adrress, Data_arr[k].binary);
                                }
                                trigger = 1;
                            }
                        }
                    }
                    if(trigger == 0){
                        for(int j = m_start_int; j< m_end_int+1; j=j+4){
                            printf("0x%x: 0x%x\n", j, 0);
                        }
                    }
                }
            }
            if(pipeline_pc.wb_pc == 0x400000+(num_instuction-1)*4){
                break;
            }
        } 
    }
    else{
        
        while(1){
            if(MEM_check == 1){
                WB();
                MEM_check = 0;
            }
            if(EX_check == 1){
                MEM(Data_arr);
                EX_check = 0;
            }
            if(ID_check == 1){
                EX(Text_arr,Data_arr);
                ID_check = 0;
            }    
            if(IF_check == 1){
                ID(Text_arr, Data_arr);
                IF_check = 0;
            }   
            int binary = 0;

            if(cycle == 0){
                binary = find_binary(PC_ars,Text_arr);
            }else{
                binary = find_binary(if_id.NPC,Text_arr);
            }

            if(binary != 0){
                IF(binary,Text_arr, Data_arr);
            }


            cycle++;

            
        
            if(trigger_d == 1){
                show_count++;
                if(trigger_p == 1){
                    print_pc(cycle);
                }
                show_register();
                if(trigger_m == 1){
                    printf("\nMemory content [0x%x..0x%x]:\n", m_start_int, m_end_int);
                    printf("----------------------------\n");
                    for(int j = 0; j < text_size; j++){
                        if(Text_arr[j].adrress == m_start_int){    
                            for(int k = j; k< j + ((m_end_int - m_start_int)/4)+1; k++){
                                printf("0x%x: 0x%x\n", Text_arr[k].adrress, Text_arr[k].binary);
                            }
                            trigger = 1;
                        }
                    }
                    if(trigger == 0){
                        for(int j = 0; j< data_size; j++){
                            if(Data_arr[j].adrress == m_start_int){
                                for(int k = j; k< j + ((m_end_int - m_start_int)/4)+1; k++){
                                    printf("0x%x: 0x%x\n", Data_arr[k].adrress, Data_arr[k].binary);
                                }
                                trigger = 1;
                            }
                        }
                    }
                    if(trigger == 0){
                        for(int j = m_start_int; j< m_end_int+1; j=j+4){
                            printf("0x%x: 0x%x\n", j, 0);
                        }
                    }
                }
            }
            if(pipeline_pc.if_pc == 0 && pipeline_pc.id_pc == 0 && pipeline_pc.ex_pc == 0 && pipeline_pc.mem_pc == 0 && pipeline_pc.wb_pc == 0){
                break;
            }
            pipeline_pc.if_pc =0;
            pipeline_pc.id_pc =0;
            pipeline_pc.ex_pc =0;
            pipeline_pc.mem_pc =0;
            pipeline_pc.wb_pc =0;
        }    
    }
    if(trigger_p == 0 || show_count == 0){
        print_pc(cycle);
    }
    if(trigger_d == 0 || show_count == 0){
        show_register();
    }
    
    if(trigger_d == 0 && trigger_m == 1){
        printf("\nMemory content [0x%x..0x%x]:\n", m_start_int, m_end_int);
        printf("----------------------------\n");
        for(int j = 0; j < text_size; j++){
            if(Text_arr[j].adrress == m_start_int){    
                for(int k = j; k< j + ((m_end_int - m_start_int)/4)+1; k++){
                    printf("0x%x: 0x%x\n", Text_arr[k].adrress, Text_arr[k].binary);
                }
                trigger = 1;
            }
        }
        if(trigger == 0){
            for(int j = 0; j< data_size; j++){
                if(Data_arr[j].adrress == m_start_int){
                    for(int k = j; k< j + ((m_end_int - m_start_int)/4)+1; k++){
                        printf("0x%x: 0x%x\n", Data_arr[k].adrress, Data_arr[k].binary);
                    }
                    trigger = 1;
                }
            }
        }
        if(trigger == 0){
            for(int j = m_start_int; j< m_end_int+1; j=j+4){
                printf("0x%x: 0x%x\n", j, 0);
            }
        }
    }
    
    fclose(fp);
}