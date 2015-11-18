#include "cpu/exec/template-start.h"

#define instr je

static void do_execute() {
	//if (cpu.eflags.ZF==1) cpu.eip+=op_src->val;
 if (DATA_BYTE==1)
 {
 	 char t=op_src->val&0x000000ff;
	if (cpu.eflags.ZF==1) cpu.eip+=t;
 }
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

