#include "cpu/exec/template-start.h"

#define instr setpe

static void do_execute() {
 if (DATA_BYTE==1)
 {
	if (cpu.eflags.PF==1)
	{
		OPERAND_W(op_dest,1);
	}
 }
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"






