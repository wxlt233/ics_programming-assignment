#include "cpu/exec/template-start.h"
#define instr sub

static void do_execute() {
	DATA_TYPE result=op_dest->val-op_src->val;
    DATA_TYPE a=op_dest->val;
	DATA_TYPE b=op_src->val;	
	if (b>0&&a-b>a) cpu.eflags.OF=1;
	else if (b<0&&a-b<a) cpu.eflags.OF=1;
	else cpu.eflags.OF=0;
	if (result==0)
	   	cpu.eflags.ZF=1;
	else 
		cpu.eflags.ZF=0;
    if (result<0) 
		cpu.eflags.SF=1;
	else
		cpu.eflags.SF=0;
	if (op_dest->val<op_src->val) 
		cpu.eflags.CF=1;
	else 
		cpu.eflags.CF=0;
	short t=(result&1)+((result>>1)&1)+((result>>2)&1)
		+((result>>3)&1)+((result>>4)&1)+((result>>5)&1)
		+((result>>6)&1)+((result>>7)&1);
	if (t%2==0) 
		cpu.eflags.PF=1;
	else 
		cpu.eflags.PF=0;
	OPERAND_W(op_dest,result);
	panic("please implement me");
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
