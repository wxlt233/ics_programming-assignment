#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/deocde/modrm.h"


make_helper(hlt)
{
	if (!(cpu.INTR&cpu.eflags.IF))
		cpu.eip-=1;
	print_asm("hlt");
	return 1;
}
