#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	if (DATA_BYTE==2)
	{
		unsigned short t=op_src->val;
		OPERAND_W(op_dest,t);
	}
	if (DATA_BYTE==4)
	{
	unsigned t=op_src->val;
	OPERAND_W(op_dest,t);
	}
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

