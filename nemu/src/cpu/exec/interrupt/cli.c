#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(cli)
{
	cpu.eflags.IF=0;
	print_asm("cli");
	return 1;
}


