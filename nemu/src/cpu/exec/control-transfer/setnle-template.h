#include "cpu/exec/template-start.h"

#define instr setnle

static void do_execute() {
 if (DATA_BYTE==1)
 {
	if (cpu.eflags.ZF==1&&(cpu.eflags.SF==cpu.eflags.OF))
	{
		OPERAND_W(op_dest,1);
	}
 }
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"







