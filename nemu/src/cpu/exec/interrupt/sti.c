#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(sti)
{
	cpu.eflags.IF=1;
	print_asm("sti");
	return 1;
}



