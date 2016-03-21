#include "cpu/exec/template-start.h"

#define instr lgdt

#if DATA_BYTE==2||DATA_BYTE==4
make_helper(concat(lgdt_i_,SUFFIX))
{
	int x=6;
	uint32_t addr=instr_fetch(cpu.eip+3,4);
	if (instr_fetch(eip+1,1)==0x10)
	{
		addr=cpu.eax;
		x=2;
	}
	if (DATA_BYTE==2)
	 {
		uint16_t limit=swaddr_read(addr,2,3)&0xffff;
		uint32_t base=swaddr_read(addr+2,3,3)&0xffffff;
		cpu.GDTR.limit=limit;
		cpu.GDTR.base=base;
	}
	else 
	{ 
		uint16_t limit=swaddr_read(addr,2,3)&0xffff;
		uint32_t base=swaddr_read(addr+2,4,3)&0xffffffff;
		cpu.GDTR.limit=limit;
		cpu.GDTR.base=base;
	}
	print_asm("lgdt $0x%x",addr);
	return x;
}

#endif

#include "cpu/exec/template-end.h"

