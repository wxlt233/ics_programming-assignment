#include "cpu/exec/template-start.h"

#define instr setns

static void do_execute() {
 if (DATA_BYTE==1)
 {
	if (cpu.eflags.SF==0)
	{
		OPERAND_W(op_src,1);
	}
 }
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"






