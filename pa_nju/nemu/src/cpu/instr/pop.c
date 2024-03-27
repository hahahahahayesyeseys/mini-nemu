#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
IF StackAddrSize = 16
THEN
 IF OperandSize = 16
 THEN
 DEST ← (SS:SP); (* copy a word *)
 SP ← SP + 2;
 ELSE (* OperandSize = 32 *)
 DEST ← (SS:SP); (* copy a dword *)
 SP ← SP + 4;
 FI;
ELSE (* StackAddrSize = 32 * )
 IF OperandSize = 16
 THEN
 DEST ← (SS:ESP); (* copy a word *)
 ESP ← ESP + 2;
 ELSE (* OperandSize = 32 *)
 DEST ← (SS:ESP); (* copy a dword *)
 ESP ← ESP + 4;
 FI;
FI;
*/
static void instr_execute_1op(){
    //所有pop指令共享的执行方法
    
      OPERAND temp;
    temp.data_size = data_size;
    temp.type = OPR_MEM;
    temp.sreg = SREG_DS;
    temp.addr = cpu.esp;
    operand_read(&temp);
    opr_src.val = temp.val;
	operand_write(&opr_src);
	cpu.esp += data_size / 8;
    
}








make_instr_impl_1op(pop,r,v)

make_instr_func(popa){
    
	opr_src.type = OPR_MEM;
	opr_src.sreg = SREG_DS;
    opr_src.data_size = 32;

    opr_src.addr=cpu.esp;
    operand_read(&opr_src);
    cpu.edi=opr_src.val;
    cpu.esp+=data_size/8;

    opr_src.addr=cpu.esp;
    operand_read(&opr_src);
    cpu.esi=opr_src.val;
    cpu.esp+=data_size/8;

    opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.ebp = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.esp = opr_src.val;
	cpu.esp += data_size / 8;

    opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.ebx = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.edx = opr_src.val;
	cpu.esp += data_size / 8;

    opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.ecx = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.eax = opr_src.val;
	cpu.esp += data_size / 8;
print_asm_0("popa", "", 1);
    return 1;



    
}
