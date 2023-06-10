#include <stdio.h>
#include <stdlib.h>

struct IF_ID{
    int Instr;
    int NPC;
};

struct ID_EX{
    int rs;
    int rt;
    int imm;
    int rd;
};

struct EX_MEM{
    int alu_out;
    int br_target;
};

struct MEM_WB{
    int alu_out;
    int mem_out;
};

struct IF_ID if_id;
struct ID_EX id_ex;
struct EX_MEM ex_mem;
struct MEM_WB mem_wb;


int hazard_type_IDtoEX(struct ID_EX *id_ex, struct EX_MEM *ex_mem){


}

int hazard_type_EXtoMEM(struct ID_EX *id_ex, struct MEM_WB *mem_wb){


}

int branch_hazard(struct ID_EX *id_ex, struct EX_MEM *ex_mem){


}

void IF(){

}

void ID(){

}

void EX(){

}

void MEM(){

}

void WB(){
    
}

void pipeline(){
    
}