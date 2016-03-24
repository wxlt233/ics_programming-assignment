#include "cpu/exec/template-start.h"
#define instr out

void pio_write(ioaddr_t addr,size_t len,uint32_t data);

make_helper(concat(out_a2i_,SUFFIX)){
	ioaddr_t addr=instr_fetch(cpu.eip+1,1);
	if (DATA_BYTE==1)
		pio_write(addr,1,cpu.gpr[0]._8[0]);
	else if (DATA_BYTE==2)
		pio_write(addr,2,cpu.gpr[0]._16);
	else 
		pio_write(addr,4,cpu.eax);
	printf("0x%x\n",cpu.esp);
	print_asm("out");
	return 2;
}

make_helper(concat(out_a2d_,SUFFIX)){
	if (DATA_BYTE==1)
		pio_write(cpu.gpr[2]._16,1,cpu.gpr[0]._8[0]);
	else if (DATA_BYTE==2)
		pio_write(cpu.gpr[2]._16,2,cpu.gpr[0]._16);
	else 
		pio_write(cpu.gpr[2]._16,4,cpu.eax);

	print_asm("out");
	return 2;

}

#include "cpu/exec/template-end.h"



