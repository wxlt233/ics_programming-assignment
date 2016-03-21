#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(iret)
{
	cpu.eip=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;
	cpu.CS.val=swaddr_read(cpu.esp,4,2)&0xffff;
	lnaddr_t descaddr=cpu.GDTR.base+8*cpu.CS.index;
	cpu.DESC[1].limit_15_0=lnaddr_read(descaddr,2)&0xffff;
	cpu.DESC[1].base_15_0=lnaddr_read(descaddr+2,2)&0xffff;
	cpu.DESC[1].base_23_16=lnaddr_read(descaddr+4,1)&0xff;
	cpu.DESC[1].limit_19_16=lnaddr_read(descaddr+6,1)&0xff;
	cpu.DESC[1].base_31_24=lnaddr_read(descaddr+7,1)&0xff;
	cpu.esp+=4;
	cpu.eflags.val=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;
	return 0;
}



