#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	int t=op_src->val;
	op_src->val=t;
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"


