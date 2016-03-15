#include "cpu/exec/template-start.h"

#define instr jmp
extern int justtry;
static void do_execute() {
	if (op_src->type==2)
	{
    	printf("op:%x",op_src->val);
		if (DATA_BYTE==1)
		 {
			 char t=op_src->val&0x000000ff;
			 cpu.eip+=t;
		 }
		else if (DATA_BYTE==2)
		{
			 short t=op_src->val&0x0000ffff;
			cpu.eip=(cpu.eip+t)&0x0000ffff;
		}
		else 
		{
			cpu.eip+=op_src->val;
		}
	}
	else 
	{
		justtry=1;
		cpu.eip=(op_src->val);
		if (DATA_BYTE==2)
			cpu.eip=cpu.eip&0x0000ffff;
	}
	print_asm_template1();
}

make_helper(concat(jmp_ptr_,SUFFIX))
{
	uint32_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	uint16_t sreg=instr_fetch(cpu.eip+1+DATA_BYTE,2);
	cpu.CS.val=sreg;
	cpu.eip=addr;
	if (DATA_BYTE==2) cpu.eip=cpu.eip&0x0000ffff;
	justtry=1;
	return DATA_BYTE+3;	
}

make_instr_helper(i)
make_instr_helper(rm)


#include "cpu/exec/template-end.h"


