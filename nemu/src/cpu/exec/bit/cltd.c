#include "cpu/exec/helper.h"

make_helper(cltd)
{
	if (cpu.eax>=0) cpu.edx=0;
	else cpu.edx=0xffffffff;
	return 1;
}

