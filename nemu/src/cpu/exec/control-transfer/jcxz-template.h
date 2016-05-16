#include "cpu/exec/template-start.h"

#define instr jcxz

static void do_execute() 
{
 	 char t=op_src->val&0x000000ff;
	if (reg_w(R_CX)==0) cpu.eip+=t;
	print_asm_template1();

}

make_instr_helper(i)

#include "cpu/exec/template-end.h"




