#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	cpu.esp-=4;
	int result=op_src->val;
//	OPERAND_W(op_dest,result);
	swaddr_write(cpu.esp,4,result);
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
