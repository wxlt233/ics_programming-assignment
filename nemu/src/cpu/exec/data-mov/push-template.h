#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	if (DATA_BYTE==2)
	{
		cpu.esp-=2;
		swaddr_write(cpu.esp,2,op_src->val,2);
	}
	else {
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,op_src->val,2);
	}
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)
#include "cpu/exec/template-end.h"
