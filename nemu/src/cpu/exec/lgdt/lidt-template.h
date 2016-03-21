#include "cpu/exec/template-start.h"

#define instr lidt

#if DATA_BYTE==2||DATA_BYTE==4
make_helper(concat(lidt_i_,SUFFIX))
{
	int x=6;
	uint32_t addr=instr_fetch(cpu.eip+3,4);
	if (instr_fetch(eip+1,1)==0x18)
	{
		printf("hahah0x%x\n",cpu.eax);
	//	addr=swaddr_read(cpu.eax,4,3);
	//	addr=instr_fetch(addr,4,3);
		x=2;
		addr=cpu.eax;
	}
	if (DATA_BYTE==2)
 	{
		uint16_t limit=swaddr_read(addr,2,3)&0xffff;
		uint32_t base=swaddr_read(addr+2,3,3)&0xffffff;
		cpu.IDTR.limit=limit;
		cpu.IDTR.base=base;
	}
	else 
	{ 
		uint16_t limit=swaddr_read(addr,2,3)&0xffff;
		uint32_t base=swaddr_read(addr+2,4,3)&0xfffffff;
		cpu.IDTR.limit=limit;
		cpu.IDTR.base=base;
	}
	print_asm("lidt $0x%x",addr);
	return x;
}

#endif

#include "cpu/exec/template-end.h"


