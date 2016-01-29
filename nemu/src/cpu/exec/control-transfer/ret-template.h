#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_,SUFFIX))
{
	cpu.eip=swaddr_read(cpu.esp,DATA_BYTE)-1;
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
	cpu.eip=swaddr_read(cpu.esp,4)-3;
	cpu.esp+=cpu.esp+addr;
	if (DATA_BYTE==2) cpu.eip=cpu.eip&0x0000ffff;
	print_asm_template1();
	return 3;
}

#include "cpu/exec/template-end.h"





