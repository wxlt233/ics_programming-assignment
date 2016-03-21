#include "cpu/exec/template-start.h"

#define instr lgdt
static void do_execute() {
	printf("0x%x\n",op_src->addr);
	uint32_t addr=op_src->addr;
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
	print_asm_template1();

}

make_instr_helper(rm)
#include "cpu/exec/template-end.h"







