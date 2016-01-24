#include "cpu/exec/template-start.h"

#define instr jnc

static void do_execute() {
 if (DATA_BYTE==1)
 {
 	 char t=op_src->val&0x000000ff;
	if (cpu.eflags.CF==0) cpu.eip+=t;
 }
 else if (DATA_BYTE==2)
 {
	short t=op_src->val&0x0000ffff;
	if (cpu.eflags.CF==0)  cpu.eip+=t;
	cpu.eip=cpu.eip&0x0000ffff;
 }
 else 
 { 
	int t=op_src->val;
	if (cpu.eflags.CF==0)  cpu.eip+=t;
 }
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"


