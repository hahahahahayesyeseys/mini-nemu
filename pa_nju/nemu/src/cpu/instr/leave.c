#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/
make_instr_func(leave)
{
    OPERAND pop;
    cpu.esp=cpu.ebp;
    pop.type=OPR_MEM;
    pop.addr=cpu.ebp;
   
    pop.data_size=data_size;
    pop.sreg=SREG_SS;
    
    operand_read(&pop);
    
    cpu.esp+=data_size/8;
    cpu.ebp=pop.val;
    return 1;


}