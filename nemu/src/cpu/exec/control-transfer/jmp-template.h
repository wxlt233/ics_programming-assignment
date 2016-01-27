#include "cpu/exec/template-start.h"

#define instr jmp
extern int justtry;
static void do_execute() {
	if (op_src->type==2)
	{
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
	}
	else 
	{
		justtry=1;
		cpu.eip=(op_src->val);
	}
	print_asm_template1();
}


make_instr_helper(i)
make_instr_helper(rm)


#include "cpu/exec/template-end.h"


