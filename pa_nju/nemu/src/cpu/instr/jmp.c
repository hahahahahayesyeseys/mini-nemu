#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp_near", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}
//Ap A类似立即数，而p是指针变量，指针一般是远转移
/*
IF instruction = near indirect JMP
 (* i.e. operand is r/m16 or r/m32 *)
THEN
 IF OperandSize = 16
 THEN
 EIP ← [r/m16] AND 0000FFFFH;
 ELSE (* OperandSize = 32 *)
 EIP ← [r/m32];
 FI;
FI

*/
make_instr_func(jmp_near_indirect){
        
        int len=1;
        len+=modrm_rm(eip + 1, &opr_src);
        operand_read(&opr_src);
        if(data_size==32){
            cpu.eip=opr_src.val;
        }
        else if(data_size==16){
            cpu.eip=opr_src.val&0x0000ffff;
        }


        
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp_near_indirect", "", 1 + data_size / 8, &opr_src);

       

    
    
    
    return 0;
}





make_instr_func(jmp_b){
    
      OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = 8;//相当于把变长指令v改成定长b
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, rel.data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp_short", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 2;
}
make_instr_func(ljmp){
    OPERAND sreg,offset;
    
    offset.addr=eip+1;
    offset.type=OPR_IMM;
    offset.data_size=32;
    operand_read(&offset);
    
    sreg.data_size=16;
    sreg.addr=eip+1+4;
    sreg.type=OPR_IMM;
    operand_read(&sreg);
    
    cpu.eip=offset.val;
    cpu.cs.val=sreg.val;

    return 0;
}




    