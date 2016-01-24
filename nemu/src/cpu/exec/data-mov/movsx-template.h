#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	int result=(DATA_TYPE_S)op_src->val;
	reg_l(op_dest->reg)=result;
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"


