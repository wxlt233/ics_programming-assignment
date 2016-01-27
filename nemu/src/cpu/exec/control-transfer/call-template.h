#include "cpu/exec/template-start.h"

#define instr call
extern int justtry;
static void do_execute() {
	if (op_src->type==2)
	{cpu.esp-=4;
	if (DATA_BYTE==4)swaddr_write(cpu.esp,4,cpu.eip+5);
	cpu.eip+=(int)(op_src->val);
	}
	else {
		justtry=1;
		cpu.esp-=4;
		if (DATA_BYTE==4) swaddr_write(cpu.esp,4,cpu.eip+5);
		cpu.eip=op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"

