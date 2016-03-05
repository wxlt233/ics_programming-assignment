#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_m_,SUFFIX))
{
	if (DATA_BYTE==1)
	{
		MEM_W(cpu.edi,cpu.gpr[0]._8[0],0);
	}
	else if (DATA_BYTE==2)
	{
		MEM_W(cpu.edi,cpu.gpr[0]._16,0);
	}
	else {
		MEM_W(cpu.edi,cpu.eax,0);
	}
	if  (cpu.eflags.DF==0)
	{
		cpu.edi+=DATA_BYTE;
	}
	else cpu.edi-=DATA_BYTE;
	print_asm_template1();
	return 1;
}

#include "cpu/exec/template-end.h"
