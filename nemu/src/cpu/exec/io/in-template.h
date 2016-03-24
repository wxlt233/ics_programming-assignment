#include "cpu/exec/template-start.h"
#define instr in

uint32_t pio_read(ioaddr_t addr,size_t len);

make_helper(concat(in_i2a_,SUFFIX)){
	ioaddr_t addr=instr_fetch(cpu.eip+1,1);
	cpu.eax=pio_read(addr,DATA_BYTE);
	print_asm("in");
	return 2;
}

make_helper(concat(in_d2a_,SUFFIX)){
	ioaddr_t addr=cpu.gpr[R_EDX]._16;
	cpu.eax=pio_read(addr,DATA_BYTE);
	print_asm("in");
	return 2;

}

#include "cpu/exec/template-end.h"


