#include "cpu/exec/template-start.h"

#define instr ja

static void do_execute() {
 if (DATA_BYTE==1)
 {
 	 char t=op_src->val&0x000000ff;
	if (cpu.eflags.ZF==0&&cpu.eflags.CF==0) cpu.eip+=t;
 }
	//	(int  )(op_src->val&0xff);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"



