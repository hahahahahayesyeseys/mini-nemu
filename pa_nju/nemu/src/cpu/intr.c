#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"


void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	cpu.esp-=12;
   
     vaddr_write(cpu.esp, SREG_SS, 4, cpu.eip);
     vaddr_write(cpu.esp+8,SREG_SS,4,cpu.eflags.val);
     vaddr_write(cpu.esp + 4, SREG_SS, 2, cpu.cs.val);
uint32_t addr = (uint32_t)hw_mem + page_translate(segment_translate(cpu.idtr.base + 8 * intr_no, SREG_DS));
    GateDesc* tmp= (GateDesc*)addr;
   
    cpu.eip = (tmp->offset_31_16 << 16) | tmp->offset_15_0;
    cpu.cs.val = tmp->selector; 
    if (tmp->type == 0xf){
      cpu.eflags.IF = 1;}
    else if (tmp->type == 0xe){
      cpu.eflags.IF = 0;
}


#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
