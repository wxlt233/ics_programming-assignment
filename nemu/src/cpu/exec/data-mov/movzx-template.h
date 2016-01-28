#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
 	   unsigned t=op_src->val&0x0000ffff;
     	reg_l(op_dest->reg)=t;
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

