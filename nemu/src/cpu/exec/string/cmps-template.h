#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_m_,SUFFIX))
{
	DATA_TYPE src=swaddr_read(cpu.esi,DATA_BYTE,3);
	DATA_TYPE dest=swaddr_read(cpu.edi,DATA_BYTE,0);
	DATA_TYPE_S temp=dest+~(src)+1;

	DATA_TYPE_S s_dest=(dest>>(DATA_BYTE*8-1))&1;
	DATA_TYPE_S s_src=(src>>(DATA_BYTE*8-1))&1;
	DATA_TYPE_S s_temp=(temp>>(DATA_BYTE*8-1))&1;

	DATA_TYPE_S dest_and_src=!!(s_dest^s_src);
	DATA_TYPE_S dest_and_temp=!!(s_dest^s_temp);
	cpu.eflags.OF=!!(dest_and_src&dest_and_temp);
	
	cpu.eflags.SF=s_temp;

	if (temp==0) 
	{
		cpu.eflags.ZF=1;
	}
	else cpu.eflags.ZF=0;

	if (src<dest)
		cpu.eflags.CF=0;
	else cpu.eflags.CF=1;

	if (cpu.eflags.DF==0)
	{
		cpu.edi+=DATA_BYTE;
		cpu.esi+=DATA_BYTE;
	}
	else 
	{
		cpu.edi-=DATA_BYTE;
		cpu.esi-=DATA_BYTE;
	}
	print_asm_template2();
	return DATA_BYTE+1;
}
#include "cpu/exec/template-end.h"

