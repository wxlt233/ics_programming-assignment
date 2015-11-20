#include "cpu/exec/helper.h"

extern char assembly[80];

int  ret(swaddr_t eip) {
	cpu.eip=swaddr_read(cpu.esp,4)-1;
	cpu.esp+=4;
	assembly[0]='r';
	assembly[1]='e';
	assembly[2]='t';
	assembly[3]='\0';
	return 1;
}
