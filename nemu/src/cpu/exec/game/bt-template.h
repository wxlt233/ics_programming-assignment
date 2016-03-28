#include "cpu/exec/template-start.h"
#define instr bt

static void do_execute() {
	DATA_TYPE a=op_dest->val;
	DATA_TYPE b=op_src->val;	
	cpu.eflags.CF=(a>>b)&1;

	print_asm_template2();
}

make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"



