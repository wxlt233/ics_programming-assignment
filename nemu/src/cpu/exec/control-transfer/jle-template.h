#include "cpu/exec/template-start.h"

#define instr jle

static void do_execute() {
 if (DATA_BYTE==1)
 {
 	 char t=op_src->val&0x000000ff;
//	 printf("%d",t);
	if (cpu.eflags.ZF==1&&cpu.eflags.SF!=cpu.eflags.OF) cpu.eip+=t;
 }
	//	(int  )(op_src->val&0xff);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"




