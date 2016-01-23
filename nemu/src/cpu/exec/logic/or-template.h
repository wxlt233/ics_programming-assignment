#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);
	cpu.eflags.OF=0;
	if (result==0)
	   	cpu.eflags.ZF=1;
	else 
		cpu.eflags.ZF=0;
	cpu.eflags.SF=MSB(result);
	cpu.eflags.CF=0;
	short t=(result&1)+((result>>1)&1)+((result>>2)&1)
		+((result>>3)&1)+((result>>4)&1)+((result>>5)&1)
		+((result>>6)&1)+((result>>7)&1);
	if (t%2==0) 
		cpu.eflags.PF=1;
	else 
		cpu.eflags.PF=0;
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
