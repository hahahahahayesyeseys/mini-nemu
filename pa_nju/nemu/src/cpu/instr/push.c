#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.

1.i386 push：


ELSE (* StackAddrSize = 32 *)//x86是32位机器
 IF OperandSize = 16
 THEN
 ESP ← ESP - 2; 等价于datasize/8
 (SS:ESP) ← (SOURCE); (* word assignment *)
 ELSE
 ESP ← ESP - 4;
 (SS:ESP) ← (SOURCE); (* dword assignment *)
 FI;
FI;

sreg:段寄存器
ss:栈段


2.instr_helper中宏定义

macro for generating the implementation of an instruction with one operand
#define make_instr_impl_1op(inst_name, src_type, suffix)   

3.CPU_STATE中寄存器定义

	uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
	
4.oprand

typedef struct
{
	int type;
	uint32_t addr;
	uint8_t sreg;
	uint32_t val;
	size_t data_size;
	MEM_ADDR mem_addr;
} OPERAND;

	
*/
static void instr_execute_1op()
{
    OPERAND get;
    operand_read(&opr_src);
    cpu.esp=cpu.esp-data_size/8;
    get.type=OPR_MEM;
    get.addr=cpu.esp;
    get.val=opr_src.val;
    get.data_size=data_size;
    get.sreg=SREG_SS;
    operand_write(&get);
}



make_instr_impl_1op(push,rm,v);
make_instr_impl_1op(push,rm,b);
make_instr_impl_1op(push,r,v);
make_instr_impl_1op(push,r,b);
make_instr_impl_1op(push, i,v);
make_instr_impl_1op(push, i,b);

make_instr_func(pusha){

    opr_dest.data_size = 32;
	opr_dest.type = OPR_MEM;
	opr_dest.sreg = SREG_DS;
    
    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.eax;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.ecx;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.edx;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.ebx;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.esp;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.ebp;
	operand_write(&opr_dest);
    
   cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.esi;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.edi;
	operand_write(&opr_dest);
    print_asm_0("pusha", "", 1);

    return 1;


}




















