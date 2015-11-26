#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	uint32_t t=swaddr_read(cpu.esp,4);
	OPERAND_W(op_dest,t);
	cpu.esp+=4;
	print_asm_template1();
}

make_instr_helper(r)
//make_instr_helper(m)

#include "cpu/exec/template-end.h"

