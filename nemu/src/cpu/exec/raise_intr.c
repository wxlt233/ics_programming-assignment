#include <setjmp.h>
#include "nemu.h"
extern jmp_buf jbuf;

void raise_intr(uint8_t NO)
{ 
	cpu.eflags.IF=0;
	cpu.eflags.TF=0;
	lnaddr_t addr=cpu.IDTR.base+8*NO;
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eflags.val,2);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.CS.val,2);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip,2);
	cpu.CS.val=lnaddr_read(addr+2,2);
	cpu.eip=lnaddr_read(addr,2)+(lnaddr_read(addr,2)<<16);
	longjmp(jbuf,1);
}
