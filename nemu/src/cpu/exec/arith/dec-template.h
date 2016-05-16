#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);
    
//	DATA_TYPE a=op_dest->val;
//	DATA_TYPE b=1;	
//	if (MSB(a)==0&&MSB(b)==1&&MSB(result)==1 ) cpu.eflags.OF=1;
//	else if (MSB(a)==1&&MSB(b)==0&&MSB(result)==0) cpu.eflags.OF=1;
//	else cpu.eflags.OF=0;
	cpu.eflags.OF=MSB(op_src->val)^MSB(result);
	cpu.eflags.CF=!((DATA_TYPE)op_src->val);
	if (result==0)
	   	cpu.eflags.ZF=1;
	else 
		cpu.eflags.ZF=0;
	cpu.eflags.SF=MSB(result);

	short t=(result&1)+((result>>1)&1)+((result>>2)&1)
		+((result>>3)&1)+((result>>4)&1)+((result>>5)&1)
		+((result>>6)&1)+((result>>7)&1);
	if (t%2==0) 
		cpu.eflags.PF=1;
	else 
		cpu.eflags.PF=0;

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
