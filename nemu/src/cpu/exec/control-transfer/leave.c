#include "cpu/exec/helper.h"

#define instr leave
extern char assembly[80];

int  leave(swaddr_t eip) {
	cpu.esp=cpu.ebp;
	cpu.ebp=swaddr_read(cpu.esp,4);
	cpu.esp+=4;
	assembly[0]='l';
	assembly[1]='e';
	assembly[2]='a';
	assembly[3]='v';
	assembly[4]='e';
	assembly[5]='\0';
	return 1;
}





