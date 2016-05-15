#include "cpu/exec/template-start.h"

#define instr call
extern int justtry;
static void do_execute() {
	if (op_src->type==2)
 	{
		cpu.esp-=4;
		if (DATA_BYTE==4)swaddr_write(cpu.esp,4,cpu.eip+5,2);
		cpu.eip+=(int)(op_src->val);
		if (DATA_BYTE==2) cpu.eip=cpu.eip&0x0000ffff;
	} 
	print_asm_template1();
}

make_instr_helper(i)
//make_instr_helper(rm)

#if DATA_BYTE==4
make_helper(call_rm_l)
{
	int len=decode_rm_l(eip+1)+1;
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len,2);
	cpu.eip=op_src->val;
	cpu.eip-=len;
	return len;
}
#endif

#if DATA_BYTE==2
make_helper(call_rm_w)
{
	int len=decode_rm_w(eip+1)+1;
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len,2);
	cpu.eip=op_src->val;
	cpu.eip-=len;
	return len;
}
#endif
#include "cpu/exec/template-end.h"

