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
int stall_for_load = 0;
int stall_for_jump = 0;

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
};

struct ID_EX{
    char* instruction;
    int rs;
    int rt;
    int imm;
    int rd;
    int shamt;
    int jump_target;
};

struct EX_MEM{
    int alu_out;
    int br_target;
};

struct MEM_WB{
    int alu_out;
    int mem_out;
};

struct instruction{
    int binary;
    char *op;
    char *op_type;
}

struct IF_ID if_id;
struct ID_EX id_ex;
struct EX_MEM ex_mem;
struct MEM_WB mem_wb;

int sign_extend(int i){
    if(i >> 15 == 1){
        i = i + 0XFFFF0000;
    }
    return i;
}

int hazard_type_IDtoEX(struct ID_EX *id_ex, struct EX_MEM *ex_mem){
    

}

int hazard_type_EXtoMEM(struct ID_EX *id_ex, struct MEM_WB *mem_wb){


}

int branch_hazard(struct ID_EX *id_ex, struct EX_MEM *ex_mem){
    if(id_ex->rs == ex_mem->)

}

void IF(int binary, char* op){
    if(strcmp(id_ex.instruction, "lw") == 0 || strcmp(id_ex.instruction, "lb") == 0 || strcmp(id_ex.instruction, "sw") == 0 || strcmp(id_ex.instruction, "sb") == 0){
        stall_for_load = 1;  
    }

    if(stall_for_load == 0 && stall_for_jump == 0){
        if_id.NPC = PC_ars + 4;
        if_id.Instr = binary; 
    }

    if(strcmp(op,"j") == 0 || strcmp(op,"jal")==0 || strcmp(op,"jr") == 0){
        stall_for_jump = 1;
    }

}

void ID(int binary, char* op_type, char* op){
    if(stall_for_jump == 0){
        if(strcmp(op_type,R)==0){
            id_ex.rd = R.rd;
            id_ex.rs = R.rs;
            id_ex.rt = R.rt;
            id_ex.shamt = R.shamt;
        } 
        else if(strcmp(op_type,I)==0){
            id_ex.rs = I.rs;
            id_ex.rt = I.rt;
            id_ex.imm = I.imm;
        }else{
            id_ex.jump_target = J_f.jump_target;
        }
    }


}

void EX(){

}

void MEM(char *op){
    if(strcmp(op, "lw") == 0){

    }
    else if(strcmp(op, "lb") == 0){

    }
    else if(strcmp(op,"sw") == 0){

    }
    else if(strcmp(op,"sb") == 0){

    }
}

void WB(char* op_type, char* op){
    if(strcmp(op_type, "I") == 0){
        if(strcmp(op, "lb") == 0 || strcmp(op, "lw") == 0){
            register_arr[I.rt] = mem_wb.mem_out;
        }
        else if(strcmp(op,"sb") != 0 && strcmp(op,"sw") !=0){
            register_arr[I.rt] = mem_wb.alu_out;
        }
    }
    else if(strcmp(op_type, "R") == 0){
        register_arr[R.rd] = mem_wb.alu_out
    }
    else if(strcmp(op, "jal") == 0){
        register_arr[31] = mem_wb.alu_out
    }
}

void pipeline(){
    for(int i = 2; i< text_size+2; i++){
        
    }
}

void ADDU(struct R_format *R){
    register_arr[R -> rd] = register_arr[R -> rt] + register_arr[R -> rs];
}

void AND(struct R_format *R){
    register_arr[R -> rd] = register_arr[R -> rt] & register_arr[R -> rs];
}

void NOR(struct R_format *R){
    register_arr[R -> rd] = ~(register_arr[R -> rt] | register_arr[R -> rs]);
}

void OR(struct R_format *R){
    register_arr[R -> rd] = register_arr[R -> rt] | register_arr[R -> rs];
}

void SLTU(struct R_format *R){
    if(register_arr[R -> rs] < register_arr[R -> rt]){
        register_arr[R -> rd] = 1;
    }else{
        register_arr[R -> rd] = 0;
    }
}

void SLL(struct R_format *R){
    register_arr[R -> rd] = register_arr[R -> rt] << (R -> shamt);
}

void SRL(struct R_format *R){
    register_arr[R -> rd] = register_arr[R -> rt] >> (R -> shamt);
}

void SUBU(struct R_format *R){
    register_arr[R -> rd] = register_arr[R -> rs] - register_arr[R -> rt]; 
}

void JR(struct R_format *R, struct text_arr *Text_arr){
    branch_ars = register_arr[R -> rs];
    for(int i = 0; i < text_size; i++){
        if(Text_arr[i].adrress == branch_ars){
            PC_ars = branch_ars;
        }
    }
}
void ADDIU(struct I_format *I){
    register_arr[I -> rt] = register_arr[I -> rs] + sign_extend((I -> imm)); // sign-extention 시키기
    
}

void ANDI(struct I_format *I){
    register_arr[I -> rt] = register_arr[I -> rs] & (I -> imm); // zero-extention 시키기
}

void BEQ(struct I_format *I, struct text_arr *Text_arr, struct data_arr *Data_arr){
    if(register_arr[I -> rs] == register_arr[I -> rt]){
        branch_ars = PC_ars + (I -> imm)*4 + 4;
    }
    for(int i = 0; i < text_size; i++){
        if(Text_arr[i].adrress == branch_ars){
            PC_ars = branch_ars - 4;
        }
    }
}

void BNE(struct I_format *I, struct text_arr *Text_arr, struct data_arr *Data_arr){
    if(register_arr[I -> rs] != register_arr[I -> rt]){
        branch_ars = PC_ars + (I -> imm)*4 + 4;
        for(int i = 0; i < text_size; i++){
            if(Text_arr[i].adrress == branch_ars){
                PC_ars = branch_ars - 4;
            }
        }
    }    
}

void LUI(struct I_format *I){
    register_arr[I -> rt] = 0XFFFFFFFF & ((I -> imm) << 16);
}

void LW(struct I_format *I, struct data_arr *Data_arr){
    int up = (I -> imm) /4;
    for(int i=0; i < data_size; i++){
        if(Data_arr[i].adrress == register_arr[I -> rs]){
            register_arr[I -> rt] = Data_arr[i+up].binary;
        }
    }
}

void LB(struct I_format *I, struct data_arr *Data_arr){
    int up = (I -> imm) / 4;
    int left = (I -> imm) % 4;
    for(int i=0; i < data_size; i++){
        if(Data_arr[i].adrress == register_arr[I -> rs]){
            if(left == 0){
                register_arr[I -> rt] = (Data_arr[i+up].binary & 0XFF000000) >> 24;
            }
            else if(left == 1){
                register_arr[I -> rt] = (Data_arr[i+up].binary & 0X00FF0000) >> 16;
            }
            else if(left == 2){
                register_arr[I -> rt] = (Data_arr[i+up].binary & 0X0000FF00) >> 8;
            }else{
                register_arr[I -> rt] = (Data_arr[i+up].binary & 0X000000FF);
            }
            
        }
    }
}

void ORI(struct I_format *I){
    register_arr[I -> rt] = register_arr[I -> rs] | (I -> imm); //zero_extended 필요
}

void SLTIU(struct I_format *I){
    if(register_arr[I -> rs] < sign_extend(I -> imm)){
        register_arr[I -> rt] = 1;
    }else{
        register_arr[I -> rt] = 0;
    }
}

void SW(struct I_format *I, struct data_arr *Data_arr){
    int up = (I -> imm) / 4;
    for(int i=0; i < data_size; i++){
        if(Data_arr[i].adrress == register_arr[I -> rs]){
            Data_arr[i+up].binary = register_arr[I -> rt];
        }
    }
}

void SB(struct I_format *I, struct data_arr *Data_arr){
    int up = (I -> imm) / 4;
    int left = (I -> imm) % 4;
    for(int i=0; i < data_size; i++){
        if(Data_arr[i].adrress == register_arr[I -> rs]){
            if(left == 0){
                Data_arr[i+up].binary = (register_arr[I -> rt] & 0XFF000000) >> 24;
            }
            else if(left == 1){
                Data_arr[i+up].binary = (register_arr[I -> rt] & 0X00FF0000) >> 16;
            }
            else if(left == 2){
                Data_arr[i+up].binary = (register_arr[I -> rt] & 0X0000FF00) >> 8;
            }else{
                Data_arr[i+up].binary = (register_arr[I -> rt] & 0X000000FF);
            }
            
        }
    }
}

void J(struct J_format *J_f, struct text_arr *Text_arr, struct data_arr *Data_arr){
    branch_ars = (J_f -> jump_target) * 4;
    PC_ars = branch_ars;

}

void JAL(struct J_format *J_f, struct text_arr *Text_arr, struct data_arr *Data_arr){
    register_arr[31] = PC_ars+4;
    branch_ars = (J_f -> jump_target) * 4;
    PC_ars = branch_ars;
}

struct R_format R;
struct J_format J_f;
struct I_format I;

int select_op(int binary, struct text_arr *Text_arr, struct data_arr *Data_arr){
    int op = (binary & 0XFF000000)>> 26;
    if(op == 0){

        R.funct = binary & 0X3F;
        R.op = 0;
        R.rs = (binary & 0X3E00000)>>21;
        R.rt = (binary & 0X1F0000)>>16;
        R.rd = (binary & 0XF800)>>11;
        R.shamt = (binary & 0X7C0) >>6;
        format = 1;

        if(R.funct == 0X21){
            ADDU(&R);
        }
        else if(R.funct == 0X24){
            AND(&R);
        } 
        else if(R.funct == 0X27){
            NOR(&R);
        }
        else if(R.funct == 0X25){
            OR(&R);
        }
        else if(R.funct == 0X2B){
            SLTU(&R);
        }
        else if(R.funct == 0){
            SLL(&R);
        }
        else if(R.funct == 2){
            SRL(&R);
        }
        else if(R.funct == 0X23){
            SUBU(&R);
        }
        else if(R.funct == 8){
            JR(&R, Text_arr);
            PC_ars = PC_ars-4;
        }
    }

    else if(op==2 | op == 3){
        
        J_f.jump_target = (binary & 0X3FFFFF);
        format = 3;

        if(op == 2){
            J_f.op = op;
            J(&J_f, Text_arr, Data_arr);
            PC_ars = PC_ars-4;
        }else{
            J_f.op = op;
            JAL(&J_f, Text_arr, Data_arr);
            PC_ars = PC_ars-4;
        }
    }else{

        I.op = op;
        I.rs = (binary & 0X3E00000)>>21;
        I.rt = (binary & 0X1F0000)>>16;
        I.imm = binary & 0XFFFF;
        format = 2;

        if(I.op == 9){
            ADDIU(&I);
        }
        else if(I.op == 0XC){
            ANDI(&I);
        }
        else if(I.op == 4){
            BEQ(&I, Text_arr, Data_arr);
        }
        else if(I.op == 5){
            BNE(&I, Text_arr, Data_arr);
        }
        else if(I.op == 0XF){
            LUI(&I);
        }
        else if(I.op == 0X23){
            LW(&I,Data_arr);
        }
        else if(I.op == 0X20){
            LB(&I,Data_arr);
        }
        else if(I.op == 0XD){
            ORI(&I);
        }
        else if(I.op == 0XB){
            SLTIU(&I);
        }
        else if(I.op == 0X2B){
            SW(&I, Data_arr);
        }
        else if(I.op == 0X28){
            SB(&I, Data_arr);
        }
    }
    PC_ars = PC_ars+4;
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
    int m_start_int;
    int m_end_int;
    int trigger = 0;
    int show_count=0;
    int inst_count=0;
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
    }
    
    if(trigger_n == 1){
        for(int i=2; i<num_instuction+2; i++){
            if(inst_count == num_instuction){
                break;
            }else{
                if(branch_ars != 0){
                    for(int j=0; j < text_size; j++){
                        if(branch_ars == Text_arr[j].adrress){
                            i = (branch_ars - 0X400000)/4 + 2;
                        }
                    }
                    branch_ars = 0;
                }
                select_op(object_array[i],Text_arr, Data_arr);
                inst_count++;
                if(trigger_d == 1){
                    show_register();
                    show_count++;
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
            }
        }
    }else{
        int check=1;
        int i =2;
        while(check){
            for(int k=0; k< text_size; k++){
                if(Text_arr[k].adrress == PC_ars){
                    check = 1;
                    break;
                }else{
                    check = 0;
                }
            }
            if(check == 0){
                break;
            }
            if(branch_ars != 0){
                for(int j=0; j < text_size; j++){
                    if(branch_ars == Text_arr[j].adrress){
                        i = (branch_ars - 0X400000)/4 + 2;
                    }
                }
                branch_ars = 0;
            }
            select_op(object_array[i],Text_arr, Data_arr);
            inst_count++;
            i++;
            if(trigger_d == 1){
                show_register();
                show_count++;
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
        }
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