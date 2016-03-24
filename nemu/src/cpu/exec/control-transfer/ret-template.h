#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_,SUFFIX))
{
	if (cpu.eip==0xc0100cb6) printf("0x%x\n",cpu.esp);
	cpu.eip=swaddr_read(cpu.esp,4,2)-1;
	if (cpu.eip==0xc0100cb6) printf("0x%x\n",cpu.eip);
	if (DATA_BYTE==2)
	{
		cpu.eip=cpu.eip&0x0000ffff;
		cpu.esp=cpu.esp+2;
	}
	else cpu.esp+=4;
	print_asm("ret");
	return 1;
}

make_helper(concat(ret_i_,SUFFIX))
{
	swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=swaddr_read(cpu.esp,4,2);
	cpu.esp+=addr;
	if (DATA_BYTE==2) cpu.eip=cpu.eip&0x0000ffff;
	print_asm_template1();
	return 0;
}

#include "cpu/exec/template-end.h"





