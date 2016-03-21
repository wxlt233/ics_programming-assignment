#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(iret)
{
	printf("0x%x\n",cpu.esp);
	cpu.eip=swaddr_read(cpu.esp,4,2);
	cpu.esp-=4;
	cpu.CS.val=swaddr_read(cpu.esp,4,2)&0xffff;
	cpu.esp-=4;
	cpu.eflags.val=swaddr_read(cpu.esp,4,2);
	return 0;
}



