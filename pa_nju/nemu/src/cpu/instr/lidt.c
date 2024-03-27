#include "cpu/instr.h"
/*
Put the implementations of `lidt' instructions here.
*/
 make_instr_func(lidt){
  
   int len=1;
   OPERAND limit;
  limit.type=OPR_MEM;
   limit.data_size=16;
   len+=modrm_rm(eip+1,&limit);
   operand_read(&limit);
  
   OPERAND base;
   base.data_size=32;
  base.addr=limit.addr+2;
   base.type=limit.type;
   operand_read(&base);
  
   cpu.idtr.limit=limit.val&0xffff;
   cpu.idtr.base=base.val&0xffffffff;
  
   return len;  
    
 }
