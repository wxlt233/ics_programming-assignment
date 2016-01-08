#include "cpu/exec/template-start.h"

#define instr jl

static void do_execute() {
 if (DATA_BYTE==1)
 {
	 char t=op_src->val&0x000000ff;
	if ((cpu.eflags.SF!=cpu.eflags.OF)) cpu.eip+=t;
 }
 else if (DATA_BYTE)
 {
	short t=op_src->val&0x0000ffff;
	if (cpu.eflags.OF!=cpu.eflags.SF)  cpu.eip+=t;
	cpu.eip=cpu.eip&0x0000ffff;
 }
 else
 { 
	int t=op_src->val;
	if (cpu.eflags.OF!=cpu.eflags.SF)  cpu.eip+=t;
 }
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"





