#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,op_src->val);
	print_asm_template1();
}

make_instr_helper(rm)
//make_instr_helper(m)

#include "cpu/exec/template-end.h"
