#ifndef __INSTR_ADD_H__
#define __INSTR_ADD_H__
/*
Put the declarations of `add' instructions here.
Sth About:↓

  #define make_instr_func(name) int name(uint32_t eip, uint8_t opcode)

   0x00 - 0x03 add_r2rm_b, add_r2rm_v, add_rm2r_b, add_rm2r_v,
   0x04 - 0x07 add_i2a_b, add_i2a_v, inv, inv,

*/
make_instr_func(add_r2rm_b);
make_instr_func(add_r2rm_v);
make_instr_func(add_rm2r_b);
make_instr_func(add_rm2r_v);
make_instr_func(add_rm2r_bv);
make_instr_func(add_i2a_b);
make_instr_func(add_i2a_v);
make_instr_func(add_i2rm_v);
make_instr_func(add_i2rm_b);
make_instr_func(add_i2rm_bv);


#endif
