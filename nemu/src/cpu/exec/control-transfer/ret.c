#include "cpu/exec/helper.h"


int  ret(swaddr_t eip) {
	cpu.eip=swaddr_read(cpu.esp,4);
	cpu.esp+=4;
	print_asm_template1();
	return 1;
}


