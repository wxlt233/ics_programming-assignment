#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
 	 DATA_TYPE t=op_src->val;
     	reg_l(op_dest->reg)=t;
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

