#include "cpu/exec/helper.h"

#define instr leave


int  leave(swaddr_t eip) {
	cpu.esp=cpu.ebp;
	cpu.ebp=swaddr_read(cpu.esp,4);
	cpu.esp+=4;
	
	print_asm_template1();
	return 1;
}





