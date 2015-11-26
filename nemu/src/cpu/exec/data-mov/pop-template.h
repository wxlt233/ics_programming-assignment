#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	printf("%x\n",cpu.esp);
	uint32_t t=swaddr_read(cpu.esp,4);
	printf("%x\n",cpu.esp);
	OPERAND_W(op_dest,t);
	printf("%x\n",cpu.esp);

	cpu.esp+=4;
	print_asm_template1();
}

make_instr_helper(r)
//make_instr_helper(m)

#include "cpu/exec/template-end.h"

