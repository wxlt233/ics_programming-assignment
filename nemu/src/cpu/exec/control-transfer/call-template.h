#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip);
	cpu.eip+=op_src->val;
}

make_instr_helper(rel)


#include "cpu/exec/template-end.h"

