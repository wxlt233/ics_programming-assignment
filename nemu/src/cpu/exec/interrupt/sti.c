#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(sti)
{
	cpu.eflags.IF=1;
	return 1;
}



