#include "cpu/exec/helper.h"
make_helper(std)
{
	cpu.eflags.DF=1;
	return 1;
}
