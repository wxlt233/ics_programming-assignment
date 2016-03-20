#include "cpu/exec/template-start.h"

#define instr lgdt

#if DATA_BYTE==2||DATA_BYTE==4
make_helper(concat(lgdt_i_,SUFFIX))
{
	uint32_t addr=instr_fetch(cpu.eip+3,4);
	printf("0x%x %x\n",cpu.eax,cpu.eip);
	uint8_t tt=instr_fetch(eip+2,1);
	printf("0x%x\n",tt);
	if (instr_fetch(eip+1,1)==0x10)
	{
		printf("hahah0x%x\n",cpu.eax);
		addr=swaddr_read(cpu.eax,4,3);
		addr=swaddr_read(addr,4,3);
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
		uint32_t base=swaddr_read(addr+2,4,3)&0xfffffff;
		cpu.GDTR.limit=limit;
		cpu.GDTR.base=base;
	}
	print_asm("lgdt $0x%x",addr);
	return 6;
}

#endif

#include "cpu/exec/template-end.h"

