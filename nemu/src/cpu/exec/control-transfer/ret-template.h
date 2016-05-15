#include "cpu/exec/template-start.h"

#define instr ret
#if DATA_BYTE==2
//make_helper(concat(ret_,SUFFIX))
make_helper(ret)
{
	cpu.eip=swaddr_read(cpu.esp,4,2)-1;
//	if (DATA_BYTE==2)
//	{
//		cpu.eip=cpu.eip&0x0000ffff;
//		cpu.esp=cpu.esp+2;
//	}
//	else 
	cpu.esp+=4;
	print_asm("ret");
	return 1;
}

/*make_helper(concat(ret_i_,SUFFIX))
{
	swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=swaddr_read(cpu.esp,4,2);
	cpu.esp+=addr;
	if (DATA_BYTE==2) cpu.eip=cpu.eip&0x0000ffff;
	print_asm_template1();
	return 0;
}*/

static void do_execute()
{
	cpu.eip=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;
	cpu.eip-=3;
	cpu.esp+=(DATA_TYPE_S) op_src->val;
	print_asm_template1();
}
make_instr_helper(i)
#endif
#include "cpu/exec/template-end.h"





