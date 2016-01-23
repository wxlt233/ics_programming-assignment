#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	if (DATA_BYTE==2)
	{
		uint16_t t=swaddr_read(cpu.esp,2);
		OPERAND_W(op_src,t);
		cpu.esp+=2;
	}
	else {
		uint32_t t=swaddr_read(cpu.esp,4);
		OPERAND_W(op_src,t);
		cpu.esp+=4;
	}
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"

