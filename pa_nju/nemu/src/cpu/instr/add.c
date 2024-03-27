#include "cpu/instr.h"
/*
Put the implementations of `add' instructions here.
*/
/*
在instr.h里包含了浮点数和定点整数的逻辑和算数运算指令
可以直接用alu_add函数

该文件中可能用到的函数名如下：
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)

static void instr_execute_2op() {
    operand_read(&opr_src);
    opr_dest.val = opr_src.val;
    operand_write(&opr_dest);
}//mov.c中的实现

#define make_instr_impl_2op(inst_name, src_type, dest_type, suffix) 

*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val=sign_ext(opr_src.val,opr_src.data_size);
	opr_dest.val=sign_ext(opr_dest.val,opr_dest.data_size);
	
	opr_dest.val=alu_add(opr_src.val,opr_dest.val,opr_dest.data_size);
	operand_write(&opr_dest);
}
/*
* 0x00 - 0x03 add_r2rm_b, add_r2rm_v, add_rm2r_b, add_rm2r_v,
* 0x04 - 0x07 add_i2a_b, add_i2a_v, inv, inv,
*/
make_instr_impl_2op(add, r, rm, b)
make_instr_impl_2op(add, r, rm, v)
make_instr_impl_2op(add, rm, r, b)
make_instr_impl_2op(add, rm, r, v)
make_instr_impl_2op(add, rm, r, bv)
make_instr_impl_2op(add, i, a, b)
make_instr_impl_2op(add, i, a, v)
make_instr_impl_2op(add, i, rm, v)
make_instr_impl_2op(add, i, rm, b)
make_instr_impl_2op(add, i, rm, bv)


