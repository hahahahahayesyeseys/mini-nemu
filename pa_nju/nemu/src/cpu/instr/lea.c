#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.

IF OperandSize = 16 AND AddressSize = 16
THEN r16 ← Addr(m);
ELSE
 IF OperandSize = 16 AND AddressSize = 32
 THEN
 r16 ← Truncate_to_16bits(Addr(m)); (* 32-bit address *)
 ELSE
 IF OperandSize = 32 AND AddressSize = 16
 THEN
 r32 ← Truncate_to_16bits(Addr(m));
 ELSE
 IF OperandSize = 32 AND AddressSize = 32
 THEN r32 ← Addr(m);
 FI;
 FI;
 FI;
FI;
*/

static void instr_execute_2op() {

    opr_dest.val=opr_src.addr;
    operand_write(&opr_dest);
    
    
    
}

make_instr_impl_2op(lea,rm,r,v);