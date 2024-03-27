#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/
make_instr_func(in_b){

cpu.eax=pio_read(cpu.edx,1);
return 1;

}
make_instr_func(in_v){
if(data_size==8){
cpu.eax=pio_read(cpu.edx,1);
}
else if(data_size==16){
cpu.eax=pio_read(cpu.edx,2);
}
else if(data_size==32){
cpu.eax=pio_read(cpu.edx,4);
}
else{
cpu.eax=pio_read(cpu.edx,data_size/8);
}

return 1;

}
