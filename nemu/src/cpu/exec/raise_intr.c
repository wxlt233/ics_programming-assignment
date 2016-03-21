#include <setjmp.h>
#include "nemu.h"
extern jmp_buf jbuf;

void raise_intr(uint8_t NO)
{ 

	lnaddr_t addr=cpu.IDTR.base+8*NO;
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eflags.val,2);
	cpu.eflags.IF=0;	
	cpu.eflags.TF=0;	
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.CS.val,2);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip,2);
	cpu.CS.val=lnaddr_read(addr+2,2);
	lnaddr_t descaddr=cpu.GDTR.base+8*cpu.CS.index;
	cpu.DESC[1].limit_15_0=lnaddr_read(descaddr,2)&0xffff;
	cpu.DESC[1].base_15_0=lnaddr_read(descaddr+2,2)&0xffff;
	cpu.DESC[1].base_23_16=lnaddr_read(descaddr+4,1)&0xff;
	cpu.DESC[1].limit_19_16=lnaddr_read(descaddr+6,1)&0xf;
	cpu.DESC[1].base_31_24=lnaddr_read(descaddr+7,1)&0xff;
	cpu.eip=lnaddr_read(addr,2)+(lnaddr_read(addr+6,2)<<16);
	longjmp(jbuf,1);
}
