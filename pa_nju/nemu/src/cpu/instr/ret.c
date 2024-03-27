#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.

IF instruction = near RET
THEN;
 IF OperandSize = 16
 THEN
 IP ← Pop();
 EIP ← EIP AND 0000FFFFH;
 ELSE (* OperandSize = 32 *)
 EIP ← Pop();
 FI;
 IF instruction has immediate operand THEN eSP ← eSP + imm16; FI;
FI;

*/

make_instr_func(ret_near){
   
	OPERAND mem;
	mem.sreg=SREG_DS;
	mem.addr=cpu.esp;
	mem.data_size=data_size;
	mem.type=OPR_MEM;

	operand_read(&mem);
	cpu.eip=mem.val;
	cpu.esp+=data_size/8;

    print_asm_0("ret", "", 1);
	return 0;
    
}



make_instr_func(ret_near_imm16){
    OPERAND i;
    i.type=OPR_IMM;
    i.data_size=16;
    i.addr=cpu.eip+1;
    operand_read(&i);
    
    OPERAND stackTop;
    stackTop.type=OPR_MEM;
    stackTop.data_size=data_size;
    stackTop.addr=cpu.esp;
    operand_read(&stackTop);
   
//IF instruction has immediate operand THEN eSP ← eSP + imm16; FI;
   
    cpu.esp+=stackTop.data_size/8;
    cpu.eip=stackTop.val;
    if(data_size==16)
        cpu.eip&=0x0000ffff;
    
    cpu.esp+=i.val;
    print_asm_1("ret","",4,&i);
    return 0;
    
}
