#include "cpu/exec/template-start.h"

#define instr sets

static void do_execute() {
 if (DATA_BYTE==1)
 {
	if (cpu.eflags.SF==1)
	{
		OPERAND_W(op_src,1);
	}
 }
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"






