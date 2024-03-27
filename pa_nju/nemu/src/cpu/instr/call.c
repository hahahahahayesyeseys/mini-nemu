#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
==CPU执行call指令时，会进行如下两个步骤：==

- (1) 将当前的IP或者CS:IP压入栈中；
- (2) 转移指令(jmp)

==CALL rel16和CALL rel32的形式是在CALL后面的指令地址上增加一个有符号的偏移量，以确定目的地。==


当指令的操作数属性为16位时，使用rel16形式；当操作数属性为16位时，使用rel32形式。
当操作数属性为32位时使用rel16形式。结果被存储在 32位EIP寄存器中。

==使用rel16，EIP的前16位被清空。
结果是一个偏移量，其值不超过16位。==

==CALL r/m16和CALL r/m32指定了一个寄存器或内存位置，从那里获取绝对的
段偏移量。==


I386:
rel是立即数之类的东西
IF rel16 or rel32 type of call
THEN (* near relative call *)
 IF OperandSize = 16
 THEN
 Push(IP);
 EIP ← (EIP + rel16) AND 0000FFFFH;
 ELSE (* OperandSize = 32 *)
 Push(EIP);
 EIP ← EIP + rel32;
 FI;
FI;

IF r/m16 or r/m32 type of call
THEN (* near absolute call *)
 IF OperandSize = 16
 THEN
 Push(IP);
 EIP ← [r/m16] AND 0000FFFFH;
 ELSE (* OperandSize = 32 *)
 Push(EIP);
 EIP ← [r/m32];
 FI;
FI;
*/


// 虚拟地址读写
//uint32_t vaddr_read(vaddr_t addr, uint8_t sreg, size_t len);

//CALL Av
make_instr_func(call_near){
        OPERAND imm;
       
        
        
        imm.type=OPR_IMM;
        imm.sreg=SREG_CS;
        imm.data_size=data_size;//因为是v后缀变长指令
        int len=1;
        
        imm.addr=eip+len;
        
        operand_read(&imm);//必须先有addr才能通过read把imm->val读下来
        
        //下面是push操作：
        
        
        //push:
        imm.val=sign_ext(imm.val,imm.data_size);
        
        if(data_size==16){
            cpu.esp-=2;
            vaddr_write(cpu.esp,SREG_SS,2,cpu.eip+len+data_size/8);
            cpu.eip=(cpu.eip+imm.val)&0xffff;
            
        }
        else{
            cpu.esp-=4;
            vaddr_write(cpu.esp,SREG_SS,4,cpu.eip+len+data_size/8);
            cpu.eip=cpu.eip+imm.val;
        }

        print_asm_1("call", "", 1 + data_size / 8, &imm);
        
    
    return (data_size/8)+1;
    
}


//CALL Ev(r/m) 有mod_r\m
//CALL r/m16和CALL r/m32指定了一个寄存器或内存位置，从那里获取绝对的段偏移量。

make_instr_func(call_near_indirect){
   OPERAND tmp;
  tmp.data_size=data_size;
  int len=1+modrm_rm(eip+1,&tmp);
   operand_read(&tmp);
cpu.eip+=len;

if(data_size==32){
cpu.esp-=4;
vaddr_write(cpu.esp,SREG_CS,4,cpu.eip);
cpu.eip=tmp.val;
}
else{
cpu.esp-=2;
vaddr_write(cpu.esp,SREG_CS,2,cpu.eip&0xffff);
cpu.eip=tmp.val&0xffff;

}
    
    
    return 0;
    
}
