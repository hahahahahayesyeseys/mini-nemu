#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
1.CMP从第一个操作数中减去第二个操作数，
但与SUB指令不同的是:
    它不存储结果,只更改了标志。

2.CMP通常与条件跳转和SETcc指令一起使用。


3.如果将大于一个字节的操作数与直接字节进行比较，则该字节值首先进行符号扩展。

SETcc:
    如果条件为 r/m8←1 ELSE r/m8←0；
    FI；描述SETcc在有效地址或寄存器指定的目的地上存储一个字节，如果条件不满足条件，则存储一个0字节。
    标记受影响:无

*/
static void instr_execute_2op() {
    operand_read(&opr_src);
	operand_read(&opr_dest);
    opr_src.val=sign_ext(opr_src.val,opr_src.data_size);
	opr_dest.val=sign_ext(opr_dest.val,opr_dest.data_size);
	
   alu_sub(opr_src.val,opr_dest.val,data_size);
    

}

make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, rm, r, bv)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, a, v)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, i, rm, bv)